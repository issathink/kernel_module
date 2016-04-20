#include "commandes.h"

static DECLARE_WAIT_QUEUE_HEAD(wq_t);
static int is_over;

void thread_wait(struct work_struct *work_arg)
{
        struct task_struct *task;
        struct pid *p;
        struct delayed_work *dw = container_of(work_arg, struct delayed_work, 
                                                                work);
        struct work_task *c_ptr = container_of(dw, struct work_task, 
                                                                dwork);
        int i, pid, c_pid, status, no_proc;
        char tmp[NAME_SIZE];
        bool alive = false;
        pid = -1;
        //c_ptr->ret_code = -1;
        no_proc = 0;
        
        no_proc = 0;
        c_pid = 0;
        for(i=0; i<NB_MAX_PID; i++) {
                c_pid = (int)((int *)(c_ptr->first))[i];
                p = find_get_pid(c_pid);
                
                if (c_pid > 0) {
                        
                        task = get_pid_task(p, PIDTYPE_PID);
                        
                        //task = get_pid_task(p, PIDTYPE_PID);
                        //pr_info("Atomic APRES : %d\n", atomic_read(&task->usage));
	                if (!task) {
	                        
	                        ((int *)(c_ptr->first))[i] = 0;
		                pr_err("can't find task for pid %d\n", c_pid);
		                put_pid(p);
		                task = (struct task_struct *)(c_ptr->sec);
		                if(!task)
		                        pr_err("Wahahhahhzhhzhez \n");
		                 else 
		                        pr_err("Yessssssssssssssssss %d \n", task->exit_code);
		                
		                continue;
	                } else 
	                        pr_info("Atomic avant : %d\n", atomic_read(&task->usage));
                        /* pr_info("Waiting for pid: %d\n", c_pid); */
                        c_ptr->sec = task;
                        task_lock(task);
                        set_task_state(task, TASK_TRACED);
	                alive = pid_alive(task);
                        status = task->exit_code;
                        c_ptr->ret_code = status;
	                if (!alive) {
                                pr_info("Pas alive!\n");
		                pid = c_pid;
		                status = task->exit_code;
		                c_ptr->ret_code = 0;
		                scnprintf(tmp, BUFFER_SIZE, "Process pid %d finished with code: %d\n", c_pid, status);
		                copy_to_user((char *) c_ptr->thir, tmp, strlen(tmp)+1);
		                task_unlock(task);
	                        put_task_struct(task);
                                put_pid(p);
                                c_ptr->ret_code = status;
                                is_over = 1;
                                wake_up(&wq_t);
                                pr_info("Someone is not alive\n");
		                return;
		        }else {
                                
                                pr_info("Je passe dans alive\n");
                        }
	                task_unlock(task);
	                // put_task_struct(task);
                } else {
                        no_proc++;
                }
                // put_pid(p);
        }
                
        if(no_proc == NB_MAX_PID) {
                pr_info("Pas trouve!\n");
                scnprintf(tmp, BUFFER_SIZE, "No process found");
		copy_to_user((char *) c_ptr->thir, tmp, strlen(tmp)+1);
		//c_ptr->ret_code = 0;
                is_over = 1;
                wake_up(&wq_t);
                return;
        }
        
        schedule_delayed_work(&c_ptr->dwork, 60);
        
}

int wait_handler(struct file *fichier, wait_data *data)
{
        int i;
        struct work_task *wt = kmalloc(sizeof(struct work_task), GFP_KERNEL);
// 	INIT_WORK(&wt->real_work, thread_wait);
        is_over = 0;
        INIT_DELAYED_WORK(&wt->dwork, thread_wait);
        
        wt->first = kmalloc(sizeof(int)*NB_MAX_PID, GFP_KERNEL);
        for(i=0; i<NB_MAX_PID; i++)
                ((int *)wt->first)[i] = data->pids[i];
                
        struct pid *p;
        struct task_struct *task;
        int c_pid;
        
        for(i=0; i<NB_MAX_PID; i++) {
                c_pid = (int)((int *)(wt->first))[i];
                p = find_get_pid(c_pid);
                
                if (p) {
                        task = get_pid_task(p, PIDTYPE_PID);
                }
        }
         
	// wt->first = data->pids;
	wt->thir = data->buf;
	wt->is_bg = data->is_bg;
	pr_info("wait_handler\n");
	schedule_delayed_work(&wt->dwork, 60);
	flush_delayed_work(&wt->dwork);
	wait_event(wq_t, is_over);
	
	/*kfree(wt);*/
	pr_info("Processus %d terminé avec %d\n", is_over, wt->ret_code);
        return wt->ret_code;
}
