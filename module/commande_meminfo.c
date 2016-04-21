#include "commandes.h"

void thread_meminfo(struct work_struct *work_arg)
{
        int res;
        struct work_task *c_ptr = container_of(work_arg, struct work_task, 
                                                        real_work);
        char tmp[BUFFER_SIZE];
        struct sysinfo val;
        si_meminfo(&val);
        // si_swapinfo(&val); /** patcher le noyau en exportant la fonction **/
        
        scnprintf(tmp, BUFFER_SIZE, "Total RAM: %ld kB\nShared RAM: %ld kB\nFree RAM: %ld kB\nBuffer RAM: %ld kB\nTotal high pages: %ld kB\nFree high pages: %ld kB\nPage size: %d kB\nFree swap: %ld kB\nTotal swap: %ld kB\n", val.totalram, val.sharedram, val.freeram, val.bufferram, val.totalhigh, val.freehigh, val.mem_unit, val.freeswap, val.totalswap);
        res = copy_to_user((char *) c_ptr->thir, tmp, strlen(tmp)+1);
}

int meminfo_handler(struct file *file, struct no_data *data) 
{
        struct work_task *wt = new_work_task();
        INIT_WORK(&wt->real_work, thread_meminfo);
        wt->thir = data->buf;
        wt->is_bg = data->is_bg;
        wt->ret_code = 0;
        schedule_work(&wt->real_work);
        flush_work(&wt->real_work);

        pr_info("Meminfo ret_code: %d\n", wt->ret_code);
        return 0;
}
