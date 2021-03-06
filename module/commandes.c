#include "commandes.h"

#define AUTHOR "Issa & Thomas <pnl@p6.io>"
#define DESC "Projet PNL"

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESC);
MODULE_VERSION("1.0");

int last_id;
struct global *glbl;

struct work_task *new_work_task()
{
	struct work_task *wt = kmalloc(sizeof(struct work_task), GFP_KERNEL);

	mutex_lock(&(glbl->mut));
	wt->id = ++last_id;
	mutex_unlock(&(glbl->mut));

	return wt;
}

void add_work_task(struct work_task *ts)
{
	mutex_lock(&(glbl->mut));
	glbl->size++;
	list_add(&(ts->list), &(glbl->head));
	mutex_unlock(&(glbl->mut));
}

long cmd_ioctl(struct file *fichier, unsigned int req, unsigned long data)
{
	switch (req) {
	case LIST:
		list_handler(fichier, (struct no_data *)data);
		return 0;
	case FG:
		return fg_handler(fichier, (struct fg_data *)data);
	case KILL:
		return kill_handler(fichier, (struct kill_data *)data);
	case MEMINFO:
		return meminfo_handler(fichier, (struct no_data *)data);
	case MODINFO:
		return modinfo_handler(fichier, (struct modinfo_data *)data);
	case WAIT:
		return wait_handler(fichier, (struct wait_data *)data);
	default:
		pr_info("Commande inconnue: %s\n", (char *)data);
		return -ENOTTY;
	}
}

struct file_operations fop = {.unlocked_ioctl = cmd_ioctl};
int major;

static int __init entry_point(void)
{
	major = register_chrdev(0, "commandes_ioctl", &fop);
	glbl = kmalloc(sizeof(*glbl), GFP_KERNEL);
	glbl->size = 0;
	mutex_init(&(glbl->mut));
	init_waitqueue_head(&(glbl->wqh));
	last_id = 0;
	INIT_LIST_HEAD(&(glbl->head));
	pr_info("Load module major: %d\n", major);
	return 0;
}
static void __exit exit_point(void)
{
	unregister_chrdev(major, "commandes_ioctl");
	kfree(glbl);
	pr_info("Unload\n");
}

module_init(entry_point);
module_exit(exit_point);
