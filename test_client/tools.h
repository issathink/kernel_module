#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

#define BUFFERSIZE      255

int is_string_entier(char *buf);
int get_kill_params(char *buffer, int *sig, int *pid);
