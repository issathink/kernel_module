#ifndef IOCTL_INFO_H
#define IOCTL_INFO_H

#define BUFFER_SIZE 1024
#define NUM_MAGIQUE_HELLO 'I'
#define NB_MAX_PID 10

struct kill_data {
	int pid;
	int sig;
	int is_bg;
	int is_over;
	int bg_id;
	char buf[BUFFER_SIZE];
};

struct wait_data {
	int is_bg;
	char is_over;
	int exit_code;
	int pids[NB_MAX_PID];
	char buf[BUFFER_SIZE];
};

struct modinfo_data {
	char name[100];
	int is_bg;
	int is_over;
	char buf[BUFFER_SIZE];
};

struct fg_data {
	int cmd_id;
	int is_bg;
	char buf[BUFFER_SIZE];
};

struct no_data {
	int is_bg;
	char buf[BUFFER_SIZE];
};

#define         LIST               _IOR(NUM_MAGIQUE_HELLO, 0, struct no_data *)
#define         FG                 _IOR(NUM_MAGIQUE_HELLO, 1, struct fg_data *)
#define         KILL             _IOR(NUM_MAGIQUE_HELLO, 2, struct kill_data *)
#define         WAIT             _IOR(NUM_MAGIQUE_HELLO, 3, struct wait_data *)
#define         MEMINFO     _IOR(NUM_MAGIQUE_HELLO, 4, struct no_data *)
#define         MODINFO     _IOR(NUM_MAGIQUE_HELLO, 5, struct modinfo_data *)

#endif
