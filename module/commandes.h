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
#include <linux/wait.h>
#include "ioctl_infos.h"

#define NAME_SIZE         100

struct work_task {
	int id;
	int is_bg;
	int ret_code;
	int exit_code;
	int is_over;
	void *first;
	void *sec;
	void *thir;
	char tmp_buf[BUFFER_SIZE];
	struct work_struct real_work;
	struct delayed_work dwork;
	struct list_head list;
};

struct global {
	int size;
	struct list_head head;
	struct mutex mut;
	wait_queue_head_t wqh;
};

extern struct global *glbl;
extern int last_id;

struct work_task *new_work_task(void);
void add_work_task(struct work_task *ts);

void thread_list(struct work_struct *work);
void list_handler(struct file *fichier, struct no_data *data);

void thread_fg(struct work_struct *work_arg);
int fg_handler(struct file *fichier, struct fg_data *data);

void thread_kill(struct work_struct *work_arg);
int kill_handler(struct file *fichier, struct kill_data *data);

void thread_meminfo(struct work_struct *work_arg);
int meminfo_handler(struct file *file, struct no_data *data);

void thread_modinfo(struct work_struct *work_arg);
int modinfo_handler(struct file *file, struct modinfo_data *data);

void thread_wait(struct work_struct *work_arg);
int wait_handler(struct file *fichier, struct wait_data *data);
