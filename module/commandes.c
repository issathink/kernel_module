#include "commandes.h"

#define AUTHOR "Issa & Etienne <pnl@p6.io>"
#define DESC   "Projet PNL"

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESC);

static int id_last = 0;

struct work_task {
	int id;
	void *first;
	void *sec;
	void *thir;
	int is_bg;
	int ret_code;
	struct work_struct real_work;
	struct list_head list;
} work_task;

struct global {
	int size;	
	struct list_head head;
	struct mutex mut;	
}  global;


static void thread_list(struct work_struct *work);

struct global *glbl;

/* **************************************************************
 ************************ Commande KILL ***************************
 ***************************************************************/
static void thread_kill(struct work_struct *work_arg)
{
        int ret_code;
        char tmp[255];
        struct pid *pid_val;
        struct work_task *c_ptr = container_of(work_arg, struct work_task, real_work);
        
        scnprintf(tmp, 15, "Je vais print");
        copy_to_user((char *) c_ptr->thir, tmp, strlen(tmp)+1);

        pid_val = find_get_pid(*(int*)c_ptr->first);
        if (pid_val == NULL) {
                pr_info("Process %d non trouve\n", *(int*)c_ptr->first);
                c_ptr->ret_code = -1;
                return;
        }

        pr_info("kill thread avant kill atomic: %d\n", atomic_read(&pid_val->count));
        if ((ret_code = kill_pid(pid_val, *(int*)c_ptr->sec, 1)))
                pr_info("Something went wrong ret: %d\n", ret_code);
        pr_info("kill thread atomic: %d\n", atomic_read(&pid_val->count));
        put_pid(pid_val);
        pr_info("Thread kill end atomic: %d\nret_code: %d\n", atomic_read(&pid_val->count), ret_code);
        c_ptr->ret_code = ret_code;
}

int kill_handler(struct file *fichier, kill_data *data)
{
        struct work_task *wt = kmalloc(sizeof(struct work_task), GFP_KERNEL);
	INIT_WORK(&wt->real_work, thread_kill);
	wt->first = &data->pid;
	wt->sec = &data->sig;
	wt->thir = data->buf;
	wt->is_bg = 0;
	schedule_work(&wt->real_work);
	flush_work(&wt->real_work);

        pr_info("Kill ret_code: %d\n", wt->ret_code);
	return wt->ret_code;
}
/**************************** END KILL ***************************/

/* **************************************************************
 *********************** Commande MEMINFO ************************
 ***************************************************************/
static void thread_meminfo(struct work_struct *work_arg)
{
        struct work_task *c_ptr = container_of(work_arg, struct work_task, real_work);
        char tmp[BUFFER_SIZE];
        struct sysinfo val;
        si_meminfo(&val);
        // si_swapinfo(&val); /** patcher le noyau en exportant la fonction **/
        
        scnprintf(tmp, BUFFER_SIZE, "Total RAM: %ld kB\nShared RAM: %ld kB\nFree RAM: %ld kB\nBuffer RAM: %ld kB\nTotal high pages: %ld kB\nFree high pages: %ld kB\nPage size: %d kB\nFree swap: %ld kB\nTotal swap: %ld kB\n", val.totalram, val.sharedram, val.freeram, val.bufferram, val.totalhigh, val.freehigh, val.mem_unit, val.freeswap, val.totalswap);
        copy_to_user((char *) c_ptr->thir, tmp, strlen(tmp)+1);
}

 int meminfo_handler(struct file *file, no_data *data) 
 {
        struct work_task *wt = kmalloc(sizeof(struct work_task), GFP_KERNEL);
        INIT_WORK(&wt->real_work, thread_meminfo);
        wt->thir = data->buf;
        wt->is_bg = data->is_bg;
        wt->ret_code = 0;
        schedule_work(&wt->real_work);
	flush_work(&wt->real_work);
	
        pr_info("Meminfo ret_code: %d\n", wt->ret_code);
	return 0;
 }
 /************************** END MEMINFO *************************/
 
/* **************************************************************
 *********************** Commande MODINFO ************************
 ***************************************************************/
static void thread_modinfo(struct work_struct *work_arg)
{
        struct work_task *c_ptr = container_of(work_arg, struct work_task, real_work);
        struct module *mod;
        mod = find_module();
}

int modinfo_handler(struct file *file, no_data *data) 
{
        struct work_task *wt = kmalloc(sizeof(struct work_task), GFP_KERNEL);
        INIT_WORK(&wt->real_work, thread_modinfo);
        wt->thir = data->buf;
        wt->is_bg = data->is_bg;
        schedule_work(&wt->real_work);
	flush_work(&wt->real_work);
	
        pr_info("Meminfo ret_code: %d\n", wt->ret_code);
	return 0;
}
/************************** END MEMINFO *************************/

/* **************************************************************
 ************************ Commande LIST ***************************
 ***************************************************************/
static void thread_list(struct work_struct *work_arg)
{
	struct work_task *c_ptr = container_of(work_arg, struct work_task, real_work);
	struct work_task *tmp_wt;
	char tmp[255];
	int res; 
	
	pr_info("COMMAND LIST TRACE\n");
	//printk(KERN_INFO "[Deferred work]=> PID: %d; NAME: %s task ID: %d\n", current->pid, current->comm);
	
	mutex_lock(&glbl->mut);
	list_for_each_entry(tmp_wt,&(glbl->head) ,list){
	        pr_info("Commande id : %d!\n", tmp_wt->id);
		scnprintf(tmp, 255, "commande id : %d!",tmp_wt->id);
		/*res= copy_to_user((char *) c_ptr->sec, tmp, strlen(tmp)+1);
		if(res == 0)
		        goto copy_pb;*/
		
	}
	// fprintf(c_ptr->first, "Nopa\n");
	mutex_unlock(&glbl->mut);
	
	pr_info("COMMAND LIST END\n");
	return;
	// printk(KERN_INFO "[Deferred work]=> I am going to sleep 2 seconds\n");
	// set_current_state(TASK_INTERRUPTIBLE);
	// schedule_timeout(2 * HZ); //Wait 2 seconds
	// return;
	copy_pb:
	        mutex_unlock(&glbl->mut);
	        pr_info("ERROR\n");
	        return;
}

void list_handler(struct file *fichier, no_data *data) 
{
	struct work_task *wt = kmalloc(sizeof(struct work_task), GFP_KERNEL);
	INIT_WORK(&wt->real_work, thread_list);
	wt->first = fichier;
	wt->thir = data->buf;
	wt->is_bg = data->is_bg;
	schedule_work(&wt->real_work);
}
/************************** END LIST ****************************/

long cmd_ioctl(struct file *fichier, unsigned int req, unsigned long data)
{
	//int res;
	
	switch (req) {
	case LIST:
		list_handler(fichier, (no_data *)data);
		return 0; 
	case KILL:
	        return kill_handler(fichier, (kill_data*)data);
	case MEMINFO:
	        return meminfo_handler(fichier, (no_data*)data);
	default:
		pr_info("Commande inconnue: %s\n", (char*) data);
		return -ENOTTY;
	}

     /*if (req == HELLO) {
        if ((res = copy_to_user((char *) buf, tampon, strlen(tampon) + 1)) == 0)
            return 0;
        else
            pr_err("ERRUR %i\n", res);
    } else if (req == WHO) {
        char tmp[255];
        if ((res = copy_from_user(tmp, (char *) buf, strlen((char *) buf) + 1)) == 0) {
            scnprintf(tampon, 255, "Hello, %s!", tmp);
           
        }
        else
            pr_err("ERRUR %i\n", res);
    } else {
        pr_err("AUTRE\n");
    }
    return -ENOTTY;*/
}


struct file_operations fop = { .unlocked_ioctl = cmd_ioctl };
int major;

static int __init entry_point(void) 
{

	major = register_chrdev(0, "commandes_ioctl", &fop);

	glbl = kmalloc(sizeof(*glbl), GFP_KERNEL);
	glbl->size = 0;
	mutex_init(&(glbl->mut));
	INIT_LIST_HEAD(&(glbl->head));

	// INIT_WORK(&glbl->real_work, thread_function);
	// schedule_work(&test_wq->real_work);

	pr_info("Load module major: %d\n", major);

	return 0;
}

static void __exit exit_point(void) 
{
	unregister_chrdev(major, "commandes_ioctl");
	kfree(glbl);
	pr_info("Unload\n");
	return;
}

module_init(entry_point);
module_exit(exit_point);

