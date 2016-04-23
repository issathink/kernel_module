#include "commandes.h"
 
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

int modinfo_handler(struct file *file, struct modinfo_data *data) 
{
        int ret_code;
        struct work_task *wt = new_work_task();
        INIT_WORK(&wt->real_work, thread_modinfo);
        wt->first = data->name;
        wt->thir = data->buf;
        wt->is_bg = data->is_bg;
        schedule_work(&wt->real_work);
	flush_work(&wt->real_work);
	
        pr_info("Modinfo ret_code: %d\n", wt->ret_code);
        
        ret_code = wt->ret_code;
        kfree(wt);
	return ret_code;
}
