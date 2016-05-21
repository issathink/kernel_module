#include "commandes.h"

void thread_kill(struct work_struct *work_arg)
{
	int ret_code, pid;
	struct pid *pid_val;
	struct work_task *c_ptr = container_of(work_arg, struct work_task,
					       real_work);

	pid = *(int *)c_ptr->first;
	pid_val = find_get_pid(pid);
	if (pid_val == NULL) {
		pr_info("Process %d not found\n", pid);
		c_ptr->ret_code = -1;
		return;
	}
	if (c_ptr->is_bg) {
		memset(c_ptr->tmp_buf, 0, BUFFER_SIZE);
		add_work_task(c_ptr);
	}

	ret_code = kill_pid(pid_val, *(int *)c_ptr->sec, 1);
	if (ret_code) {
		pr_info("Something went wrong ret: %d\n", ret_code);
		scnprintf(c_ptr->tmp_buf, BUFFER_SIZE, "Something went wrong exit code: %d", ret_code);
		copy_to_user((char *) c_ptr->thir, c_ptr->tmp_buf,
			     strlen(c_ptr->tmp_buf)+1);
	} else {
		scnprintf(c_ptr->tmp_buf, BUFFER_SIZE, "Successfuly killed process pid: %d.", pid);
		if (!c_ptr->is_bg)
			copy_to_user((char *) c_ptr->thir, c_ptr->tmp_buf,
				     strlen(c_ptr->tmp_buf)+1);
	}
	put_pid(pid_val);

	c_ptr->ret_code = ret_code;
	c_ptr->is_over = 1;
	wake_up(&(glbl->wqh));
}

int kill_handler(struct file *fichier, struct kill_data *data)
{
	struct work_task *wt = new_work_task();
	int ret_code;

	INIT_WORK(&wt->real_work, thread_kill);
	wt->first = &data->pid;
	wt->sec = &data->sig;
	wt->thir = data->buf;
	wt->is_bg = data->is_bg;
	memset(wt->tmp_buf, 0, BUFFER_SIZE);
	schedule_work(&wt->real_work);

	pr_info("Kill ret_code: %d\n", wt->ret_code);
	/** Free data if it's on foreground otherwise it will be freed when
	 * it's deleted from the list. */
	if (!data->is_bg) {
		flush_work(&wt->real_work);
		ret_code = wt->ret_code;
		kfree(wt);
		return ret_code;
	}
	return 0;
}
