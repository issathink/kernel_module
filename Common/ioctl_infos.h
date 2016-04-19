#ifndef IOCTL_INFO_H
#define IOCTL_INFO_H

#define BUFFER_SIZE 1024
#define NUM_MAGIQUE_HELLO 'I'
#define NB_MAX_PID 10

typedef struct _kill_data {
        int pid;
        int sig;
        char buf[BUFFER_SIZE];
} kill_data;

typedef struct _wait_data {
        int is_bg;
        int pids[NB_MAX_PID];
        char buf[BUFFER_SIZE];
} wait_data;

typedef struct _modinfo_data {
        char name[100];
        int is_bg;
        char buf[BUFFER_SIZE];
} modinfo_data;

typedef struct _fg_data {
        int cmd_id;
        int is_bg;
        char buf[BUFFER_SIZE];
} fg_data;

typedef struct _no_data {
        int is_bg;
        char buf[BUFFER_SIZE];
} no_data;

#define         LIST                      _IOR(NUM_MAGIQUE_HELLO, 0, no_data *)
#define         FG                        _IOR(NUM_MAGIQUE_HELLO, 1, fg_data *)
#define         KILL                      _IOR(NUM_MAGIQUE_HELLO, 2, kill_data *)
#define         WAIT                    _IOR(NUM_MAGIQUE_HELLO, 3, wait_data *)
#define         MEMINFO            _IOR(NUM_MAGIQUE_HELLO, 4, no_data *)
#define         MODINFO            _IOR(NUM_MAGIQUE_HELLO, 5, modinfo_data *)

#endif