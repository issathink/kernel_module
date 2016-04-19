#include "commandes.h"

void thread_kill(struct work_struct *work_arg)
{
        int ret_code, res;
        char tmp[BUFFER_SIZE];
        struct pid *pid_val;
        struct work_task *c_ptr = container_of(work_arg, struct work_task,
                                                        real_work);
        scnprintf(tmp, BUFFER_SIZE, "Je vais print");
        res = copy_to_user((char *) c_ptr->thir, tmp, strlen(tmp)+1);

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
        kfree(wt);
	return wt->ret_code;
}
