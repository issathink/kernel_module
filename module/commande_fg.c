#include "commandes.h"

void thread_fg(struct work_struct *work_arg)
{
        struct work_task *c_ptr = container_of(work_arg, struct work_task, 
                                                        real_work);
        
}

int fg_handler(struct file *fichier, struct fg_data *data)
{
        int ret_code;
        struct work_task *wt = kmalloc(sizeof(struct work_task), GFP_KERNEL);
        struct work_task *pos, *tmp_wt;
        
        if (!wt) {
                pr_err("Error no memory\n");
                return -ENOMEM;
        }
        tmp_wt = NULL;
        mutex_lock(&glbl->mut);
        list_for_each_entry(pos, &(glbl->head), list){
	        if (pos->id == data->cmd_id) {
	                pr_info("Bingo c'est le bon\n");
	                tmp_wt = pos;
	                break;
	        }
        }
        mutex_unlock(&glbl->mut);
        
        if (!tmp) {
                pr_info("Task with id: %d not found\n", data->cmd_id);
                kfree(wt);
                return -1;
        }
        
        wt->first = &data->cmd_id;
        
        INIT_WORK(&wt->real_work, thread_fg);
        schedule_work(&wt->real_work);
	flush_work(&wt->real_work);
	
	ret_code = wt->ret_code;
        free(wt);
	return ret_code;
}
