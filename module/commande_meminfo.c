#include "commandes.h"

void thread_meminfo(struct work_struct *work_arg)
{
	struct work_task *c_ptr = container_of(work_arg, struct work_task,
					       real_work);
	struct sysinfo val;

	si_meminfo(&val);
	si_swapinfo(&val); /** patcher le noyau en exportant la fonction **/

	if (c_ptr->is_bg)
		add_work_task(c_ptr);

	memset(c_ptr->tmp_buf, 0, BUFFER_SIZE);
	scnprintf(c_ptr->tmp_buf, BUFFER_SIZE, "Total RAM: %ld kB\nShared RAM: %ld kB\nFree RAM: %ld kB\nBuffer RAM: %ld kB\nTotal high pages: %ld kB\nFree high pages: %ld kB\nPage size: %d kB\nFree swap: %ld kB\nTotal swap: %ld kB\n", val.totalram, val.sharedram, val.freeram, val.bufferram, val.totalhigh, val.freehigh, val.mem_unit, val.freeswap, val.totalswap);
	copy_to_user((char *) c_ptr->thir, c_ptr->tmp_buf, strlen(c_ptr->tmp_buf)+1);
	c_ptr->is_over = 1;
	wake_up(&(glbl->wqh));
}

int meminfo_handler(struct file *file, struct no_data *data)
{
	struct work_task *wt = new_work_task();
	int ret_code;
	INIT_WORK(&wt->real_work, thread_meminfo);
	wt->thir = data->buf;
	wt->is_bg = data->is_bg;
	wt->ret_code = 0;
	schedule_work(&wt->real_work);
	if (!data->is_bg) {
		flush_work(&wt->real_work);
		ret_code = wt->ret_code;
		kfree(wt);
	}
	pr_info("Meminfo ret_code: %d\n", wt->ret_code);

	return 0;
}
