#include "commandes.h"

static DECLARE_WAIT_QUEUE_HEAD(wq_t);
static int is_over;

void thread_wait(struct work_struct *work_arg)
{
        struct task_struct *task;
        struct delayed_work *dw = container_of(work_arg, struct delayed_work, 
                                                                work);
        struct work_task *c_ptr = container_of(dw, struct work_task, 
                                                                dwork);
        int i, pid, status;
        bool alive = false;
        pid = -1;

        for(i=0; i<NB_MAX_PID; i++) {
                task = ((struct task_struct **)(c_ptr->sec))[i];
                
                if (task != NULL) {
                        task_lock(task);
                        alive = pid_alive(task);
                        if (!alive) {
                                pid = task->pid;
                                status = task->exit_code;
                                c_ptr->exit_code = status;
                                scnprintf((char*) c_ptr->thir, BUFFER_SIZE, "Process pid %d finished.", pid);
                                task_unlock(task);
                                put_task_struct(task);
                                c_ptr->ret_code = status;
                                is_over = 1;
                                wake_up(&wq_t);
                                return;
                        }else {
                                task_unlock(task);
                        }
                }
        }
        schedule_delayed_work(&c_ptr->dwork, 60);
}

int wait_handler(struct file *fichier, struct wait_data *data)
{
        int i;
        char tmp[NAME_SIZE];
        int c_pid;
        struct pid *p;
        int no_proc = 0;
        struct task_struct* task = NULL;
        struct work_task *wt = kmalloc(sizeof(struct work_task), GFP_KERNEL);

        wt->first = kmalloc(sizeof(int)*NB_MAX_PID, GFP_KERNEL);
        wt->sec = kmalloc(sizeof(struct task_struct*)*NB_MAX_PID, GFP_KERNEL);
        wt->thir = kmalloc(sizeof(char)*BUFFER_SIZE, GFP_KERNEL);

        is_over = 0;
        INIT_DELAYED_WORK(&wt->dwork, thread_wait);

        for(i=0; i<NB_MAX_PID; i++)
                ((int *)wt->first)[i] = data->pids[i];

        for (i=0; i<NB_MAX_PID; i++) {
                c_pid = (int)((int *)(wt->first))[i];
                p = find_get_pid(c_pid);
                if (c_pid > 0) {
                        task = get_pid_task(p, PIDTYPE_PID);
                        if (!task) {
                                ((struct task_struct **)(wt->sec))[i] = NULL;
                                pr_err("can't find task for pid %d\n", c_pid);
                        }
                        else {
                                no_proc++;
                                ((struct task_struct **)(wt->sec))[i] = task;
                                put_task_struct(((struct task_struct **)(wt->sec))[i]);
                        }
                        put_pid(p);
                }
                else {
                        ((struct task_struct **)(wt->sec))[i] = NULL;
                }
        }

        if (no_proc > 0) {
        	wt->is_bg = data->is_bg;
        	schedule_delayed_work(&wt->dwork, 60);
        	flush_delayed_work(&wt->dwork);
        	wait_event(wq_t, is_over);

                copy_to_user((char *) data->buf, (char *) wt->thir, strlen((char *) wt->thir)+1);
                data->exit_code = wt->exit_code;
        }
        else {
                scnprintf(tmp, BUFFER_SIZE, "No process found");
                copy_to_user((char *) data->buf, tmp, strlen(tmp)+1);
        }
	
        kfree(wt->first);
        kfree(wt->sec);
        kfree(wt->thir);
	kfree(wt);
        return 0;
}
