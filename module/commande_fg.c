#include "commandes.h"

void thread_fg(struct work_struct *work_arg)
{
        struct work_task *c_ptr = container_of(work_arg, struct work_task, 
                                                        real_work);
        pr_info("Avant que j'attende le type is_over = %d (id: %d)\n", c_ptr->is_over, c_ptr->id);
        wait_event(glbl->wqh, c_ptr->is_over);
        pr_info("J'ai fini d'attendre\n");
        copy_to_user((char *) c_ptr->thir, c_ptr->tmp_buf, strlen(c_ptr->tmp_buf)+1);
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
	                pr_info("Trouve!\n");
	                tmp_wt = pos;
	                break;
	        }
        }
        pr_info("Avant de supp : %d\n", tmp_wt->id);
        list_del(&tmp_wt->list); /* Supprimer le work de liste des taches en bg */
        pr_info("Apres supp : %d\n", tmp_wt->id);
        if(glbl->size > 0)
                glbl->size--;
        mutex_unlock(&glbl->mut);
        if (!tmp_wt) {
                pr_info("Task with id: %d not found\n", data->cmd_id);
                kfree(wt);
                return -1;
        }
        wt->first = &data->cmd_id;
        
        INIT_WORK(&wt->real_work, thread_fg);
        pr_info("Avant que je schedule le truc, id = %d\n", wt->id);
        schedule_work(&wt->real_work);
	flush_work(&wt->real_work);
	
	ret_code = wt->ret_code;
        kfree(wt);
	return ret_code;
}
