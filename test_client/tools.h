#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

#define BUFFERSIZE                              255
#define NUM_MAGIQUE_HELLO           'I'
#define NB_MAX_PID                            10

typedef struct _kill_data {
        int pid;
        int signal;
} kill_data;

typedef struct _wait_data {
        int pids[NB_MAX_PID];
} wait_data;

typedef struct _modinfo_data {
        char *name;
        int is_bg;
} modinfo_data;

typedef struct _fg_data {
        char *name;
        int is_bg;
} fg_data;

typedef struct _no_data {
        int is_bg;
} no_data;

#define         LIST                      _IOR(NUM_MAGIQUE_HELLO, 0, no_data *)
#define         FG                        _IOR(NUM_MAGIQUE_HELLO, 1, fg_data *)
#define         KILL                      _IOR(NUM_MAGIQUE_HELLO, 2, kill_data *)
#define         WAIT                    _IOR(NUM_MAGIQUE_HELLO, 3, wait_data *)
#define         MEMINFO            _IOR(NUM_MAGIQUE_HELLO, 4, no_data *)
#define         MODINFO            _IOR(NUM_MAGIQUE_HELLO, 5, modinfo_data *)

int is_string_entier(char *buf);
int get_kill_params(char *buffer, int *sig, int *pid);
