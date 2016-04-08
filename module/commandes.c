#ifndef __KERNEL__
#  define __KERNEL__
#endif
#ifndef MODULE
#  define MODULE
#endif

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <linux/fs.h>

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
        char tmp[255];
        struct work_task *c_ptr = container_of(work_arg, struct work_task, real_work);
        scnprintf(tmp, 15, "Je vais print\n");
        copy_to_user((char *) c_ptr->thir, tmp, strlen(tmp)+1);
        kill_pid(find_get_pid(c_ptr->first), c_ptr->sec, 1);
        pr_info("Thread kill end.\n");
}


void kill_handler(struct file *fichier, kill_data* data)
{
        struct work_task *wt = kmalloc(sizeof(struct work_task), GFP_KERNEL);
	INIT_WORK(&wt->real_work, thread_kill);
	wt->first = &data->pid;
	wt->sec = &data->sig;
	wt->thir = data->buf;
	// wt->sec = (char*) buf;
	wt->is_bg = 0;
	schedule_work(&wt->real_work);
}
/**************************** END KILL ***************************/

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


long commandes_ioctl(struct file*fichier, unsigned int req,unsigned long data)
{
	//int res;
	
	switch(req)
	{
		case LIST:
			list_handler(fichier, (no_data *)data);
			return 0; 
		case KILL:
		        kill_handler(fichier, (kill_data*) data);
		        return 0;
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


struct file_operations fop = { .unlocked_ioctl = commandes_ioctl };
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
