#include "commandes.h"

/* **************************************************************
 ************************ Commande LIST ***************************
 ***************************************************************/
void thread_list(struct work_struct *work_arg)
{
	struct work_task *c_ptr = container_of(work_arg, struct work_task, 
	                                                real_work);
	struct work_task *tmp_wt;
	char tmp[BUFFER_SIZE];
	int res; 
	
	pr_info("COMMAND LIST TRACE\n");
	/* printk(KERN_INFO "[Deferred work]=> PID: %d; NAME: %s task ID: %d\n", current->pid, current->comm); */
	
	mutex_lock(&glbl->mut);
	list_for_each_entry(tmp_wt,&(glbl->head) ,list){
	        pr_info("Commande id : %d!\n", tmp_wt->id);
		res = scnprintf(tmp, BUFFER_SIZE, "commande id : %d!",tmp_wt->id);
		res= copy_to_user((char *) c_ptr->sec, tmp, strlen(tmp)+1);
		if(res == 0)
		        goto copy_pb;
		
	}
	/* fprintf(c_ptr->first, "Nopa\n"); */
	mutex_unlock(&glbl->mut);
	
	pr_info("COMMAND LIST END\n");
	return;
	/* printk(KERN_INFO "[Deferred work]=> I am going to sleep 2 seconds\n"); */
	/* set_current_state(TASK_INTERRUPTIBLE); */
	/* schedule_timeout(2 * HZ); //Wait 2 seconds */
	/* return; */
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
	flush_work(&wt->real_work);
	kfree(wt);
}
/************************** END LIST ****************************/
