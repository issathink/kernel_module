#include "commandes.h"
 
/* **************************************************************
 *********************** Commande MODINFO ************************
 ***************************************************************/
void thread_modinfo(struct work_struct *work_arg)
{
        struct work_task *c_ptr = container_of(work_arg, struct work_task, 
                                                        real_work);
        struct module *mod;
        char tmp[BUFFER_SIZE];
        int res;
        
        res = copy_from_user(tmp, (char*)c_ptr->first, strlen(c_ptr->first)+1);
        mod = find_module(tmp);
        
        if (mod == NULL) {
                c_ptr->ret_code = -1;
                res = copy_to_user((char *) c_ptr->thir, tmp, strlen(tmp)+1);
                return;
        }
        res = scnprintf(tmp, BUFFER_SIZE, "Name: %s\nVersion: %s\nArgs: %s\nLoad adress: %p\n", mod->name, mod->version, mod->args, mod);
        res = copy_to_user((char *) c_ptr->thir, tmp, strlen(tmp)+1);
        c_ptr->ret_code = 0;
        pr_info("thread_modinfo END.\n");  
}

int modinfo_handler(struct file *file, modinfo_data *data) 
{
        struct work_task *wt = kmalloc(sizeof(struct work_task), GFP_KERNEL);
        INIT_WORK(&wt->real_work, thread_modinfo);
        wt->first = data->name;
        wt->thir = data->buf;
        wt->is_bg = data->is_bg;
        schedule_work(&wt->real_work);
	flush_work(&wt->real_work);
	
        pr_info("Modinfo ret_code: %d\n", wt->ret_code);
        kfree(wt);
	return wt->ret_code;
}
/************************** END MEMINFO *************************/
