#include "commandes.h"

/* **************************************************************
 ************************ Commande WAIT **************************
 ***************************************************************/
void thread_wait(struct work_struct *work_arg)
{
        struct task_struct *task;
        struct pid *p;
        struct work_task *c_ptr = container_of(work_arg, struct work_task, 
                                                                real_work);
        int i, pid, c_pid, status, no_proc;
        char tmp[NAME_SIZE];
        bool alive = false, is_over = false;
        pid = -1;
        c_ptr->ret_code = -1;
        no_proc = 0;
	
	pr_err("Initialisation ok\n");
        
        while(1) {
                no_proc = 0;
                for(i=0; i<NB_MAX_PID; i++) {
                        c_pid = (int)((int *)(c_ptr->first))[i];
                        // pr_err("Well c_pid %d\n", c_pid);
                        p = find_get_pid(c_pid);
                
                        if (c_pid > 0) {
                                task = get_pid_task(p, PIDTYPE_PID);
	                        if (!task) {
	                                ((int *)(c_ptr->first))[i] = 0;
		                        pr_err("can't find task for pid %u\n", pid_nr(p));
		                        put_pid(p);
		                        put_task_struct(task);
		                        continue;
	                        }
                                // pr_info("Waiting for pid: %d\n", c_pid);
                                task_lock(task);
	                        alive = pid_alive(task);
	                        if (!alive) {
		                         pid = c_pid;
		                         status = task->exit_code;
		                         c_ptr->ret_code = 0;
		                         scnprintf(tmp, BUFFER_SIZE, "Process pid %d finished with code: %d\n", c_pid, status);
		                         copy_to_user((char *) c_ptr->thir, tmp, strlen(tmp)+1);
		                         is_over = true; 
		                         goto out;
		                }
	                        task_unlock(task);
	                        put_task_struct(task);
                        } else {
                                // pr_info("Oh no pid: %d\n", pid);
                                no_proc++;
                        }
                        put_pid(p);
                }
                
                if(is_over || no_proc == 10)
                        break;
        }
        /*INIT_DELAYED_WORK(&c_ptr->dwork, thread_wait);
        pr_info("Avant le schedule_delayed_work\n");
        schedule_delayed_work(&c_ptr->dwork, 10);*/
        return;
        
out:
        task_unlock(task);
	put_task_struct(task);
        put_pid(p);
}

int wait_handler(struct file *fichier, wait_data *data)
{
        struct work_task *wt = kmalloc(sizeof(struct work_task), GFP_KERNEL);
	INIT_WORK(&wt->real_work, thread_wait);
	wt->first = data->pids;
	wt->thir = data->buf;
	wt->is_bg = data->is_bg;
	pr_info("wait_handler\n");
	schedule_work(&wt->real_work);
	flush_work(&wt->real_work);
	/*kfree(wt);*/
	pr_info("i'm out %d\n", wt->ret_code);
        return wt->ret_code;
}
