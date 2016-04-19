#ifndef __KERNEL__
#  define __KERNEL__
#endif
#ifndef MODULE
#  define MODULE
#endif

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/swap.h>
#include <linux/mm.h>
#include "ioctl_infos.h"

#define NAME_SIZE         100

typedef struct work_task {
	int id;
	void *first;
	void *sec;
	void *thir;
	int is_bg;
	int ret_code;
	struct work_struct real_work;
	struct delayed_work dwork;
	struct list_head list;
} work_task;

typedef struct global {
	int size;	
	struct list_head head;
	struct mutex mut;
}  global;

extern struct global *glbl;

void thread_list(struct work_struct *work);
void list_handler(struct file *fichier, no_data *data);

void thread_kill(struct work_struct *work_arg);
int kill_handler(struct file *fichier, kill_data *data);

void thread_meminfo(struct work_struct *work_arg);
int meminfo_handler(struct file *file, no_data *data);

void thread_modinfo(struct work_struct *work_arg);
int modinfo_handler(struct file *file, modinfo_data *data);

void thread_wait(struct work_struct *work_arg);
int wait_handler(struct file *fichier, wait_data *data);
