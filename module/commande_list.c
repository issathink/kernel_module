#include "commandes.h"

void thread_list(struct work_struct *work_arg)
{
	struct work_task *c_ptr = container_of(work_arg, struct work_task,
						real_work);
	struct work_task *tmp_wt;

	char tmp[BUFFER_SIZE];
	int res, size;
	pr_info("COMMAND LIST TRACE (size: %d)\n", glbl->size);
	size = scnprintf(tmp, BUFFER_SIZE, "List (ids) of running processes: \n");

	mutex_lock(&glbl->mut);
	list_for_each_entry(tmp_wt, &(glbl->head), list) {
		size += scnprintf(tmp+size, BUFFER_SIZE-size, "%d\n", tmp_wt->id);
	}
	mutex_unlock(&glbl->mut);
	res = copy_to_user((char *) (c_ptr->thir), tmp, strlen(tmp)+1);
	if (res != 0)
		goto copy_pb;

	pr_info("COMMAND LIST END\n");
	return;

copy_pb:
	mutex_unlock(&glbl->mut);
	pr_info("ERROR\n");
	return;
}

void list_handler(struct file *fichier, struct no_data *data)
{
	struct work_task *wt = new_work_task();
	INIT_WORK(&wt->real_work, thread_list);
	wt->first = fichier;
	wt->thir = data->buf;
	wt->is_bg = data->is_bg;
	schedule_work(&wt->real_work);
	flush_work(&wt->real_work);
	kfree(wt);
}
