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
int get_kill_params(char *buffer, int *sig, int *pid, int *is_bg);
int get_modinfo_params(char *buffer, char *name, int *is_bg);
int get_wait_params(char *buffer, int params[], int *size);
int get_fg_params(char *buffer, int *id);

