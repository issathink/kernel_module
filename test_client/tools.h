#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "ioctl_infos.h"

int is_string_entier(char *buf);
int get_kill_params(char *buffer, int *sig, int *pid);
int get_modinfo_param(char *buffer, char *name);
int get_wait_params(char *buffer, int params[], int *size);

