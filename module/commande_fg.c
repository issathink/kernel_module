#include "commandes.h"

void thread_fg(struct work_struct *work_arg)
{
        struct work_task *c_ptr = container_of(work_arg, struct work_task, 
                                                        real_work);
        pr_info("Avant que j'attende le type is_over = %d (id: %d)\n", c_ptr->is_over, c_ptr->id);
        if (!c_ptr->is_over)
                wait_event(glbl->wqh, c_ptr->is_over);
        pr_info("J'ai fini d'attendre chaine de retour : %s\n", c_ptr->tmp_buf);
        copy_to_user((char *) c_ptr->thir, c_ptr->tmp_buf, strlen(c_ptr->tmp_buf)+1);
        c_ptr->ret_code = 0;
}

int fg_handler(struct file *fichier, struct fg_data *data)
{
        int ret_code;
        bool found;
        struct work_task *pos, *tmp_wt;
        char tmp[BUFFER_SIZE];

        tmp_wt = NULL;
        found = false;
        mutex_lock(&glbl->mut);
        list_for_each_entry(pos, &(glbl->head), list) {
                if (pos->id == data->cmd_id) {
                        pr_info("Trouve!\n");
                        tmp_wt = pos;
                        found = true;
                        break;
                }
        }
        if (!tmp_wt) {
                pr_info("Task with id: %d not found\n", data->cmd_id);
                scnprintf(tmp, BUFFER_SIZE, "Task with id: %d not found\n", data->cmd_id);
                copy_to_user(data->buf, tmp, strlen(tmp)+1);
        } else {
                pr_info("Avant de supp : %d\n", tmp_wt->id);
                list_del(&tmp_wt->list); /* Supprimer le work de liste des taches en bg */
                tmp_wt->thir = data->buf;
                pr_info("Apres supp : %d\n", tmp_wt->id);
                if (found && glbl->size > 0)
                        glbl->size--;
        }
        mutex_unlock(&glbl->mut);
        if (!found)
                return 0;
        
        INIT_WORK(&tmp_wt->real_work, thread_fg);
        schedule_work(&tmp_wt->real_work);
        flush_work(&tmp_wt->real_work);

        ret_code = tmp_wt->ret_code;
        kfree(tmp_wt);
        return ret_code;
}
