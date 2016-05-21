#include "commandes.h"

void thread_modinfo(struct work_struct *work_arg)
{
	struct work_task *c_ptr = container_of(work_arg, struct work_task,
					       real_work);
	struct module *mod;
	char tmp[BUFFER_SIZE];

	copy_from_user(tmp, (char *)c_ptr->first, strlen(c_ptr->first)+1);
	mod = find_module(tmp);

	if (c_ptr->is_bg)
		add_work_task(c_ptr);
	if (mod == NULL) {
		c_ptr->ret_code = -1;
		memset(c_ptr->tmp_buf, 0, BUFFER_SIZE);
		scnprintf(c_ptr->tmp_buf, BUFFER_SIZE, "Could not find module named : '%s'\n", tmp);
		copy_to_user((char *) c_ptr->thir, c_ptr->tmp_buf, strlen(c_ptr->tmp_buf)+1);
	} else {
		memset(c_ptr->tmp_buf, 0, BUFFER_SIZE);
		scnprintf(c_ptr->tmp_buf, BUFFER_SIZE, "Name: %s\nVersion: %s\nArgs: %s\nLoad adress: %p\n", mod->name, mod->version, mod->args, mod);
		copy_to_user((char *) c_ptr->thir, c_ptr->tmp_buf,
			     strlen(c_ptr->tmp_buf)+1);
		c_ptr->ret_code = 0;
	}
	c_ptr->is_over = 1;
	pr_info("Je vais wake quelqu'un is_over = %d\n", c_ptr->is_over);
	wake_up(&(glbl->wqh));
	pr_info("thread_modinfo END.\n");
}

int modinfo_handler(struct file *file, struct modinfo_data *data)
{
	int ret_code;
	struct work_task *wt = new_work_task();
	INIT_WORK(&wt->real_work, thread_modinfo);
	memset(wt->tmp_buf, 0, BUFFER_SIZE);
	wt->first = data->name;
	wt->thir = data->buf;
	wt->is_bg = data->is_bg;
	schedule_work(&wt->real_work);
	if (!data->is_bg) {
		flush_work(&wt->real_work);
		ret_code = wt->ret_code;
		kfree(wt);
		pr_info("Modinfo ret_code: %d\n", ret_code);
		return ret_code;
	}
	return 0;
}
