#include "tools.h"

int main() {
         int fd = open("/dev/commandes_ioctl", O_RDONLY);
        char buffer[BUFFER_SIZE];

        while (fgets(buffer, BUFFER_SIZE , stdin)) {
                  if (strncmp("LIST", buffer, 4) == 0) {
                       struct no_data *data = malloc(sizeof(*data));
                       if (ioctl(fd, LIST, data) == 0)
                                fprintf(stderr, "%s\n", data->buf);
                        else
                                fprintf(stderr, "ERREUR LIST\n\n");
                        free(data);
                  } else if (strncmp("KILL ", buffer, 5) == 0) {
                        int sig, pid, ret_code, is_bg;
                        struct kill_data *data = malloc(sizeof(*data));
                        
                        if (!get_kill_params(buffer, &sig, &pid, &is_bg)) {
                                fprintf(stderr, "Usage : KILL <signal> <pid> [&]\n");
                                memset(buffer, 0, BUFFER_SIZE);
                                continue;
                        }
                        
                        data->pid = pid;
                        data->sig = sig;
                        data->is_bg = is_bg;
                        
                        if (pid == getpid()) {
                                fprintf(stderr, "Attempt to kill me, nice try.\n");
                                memset(buffer, 0, BUFFER_SIZE);
                                free(data);
                                continue;
                        }
                        
                        if ((ret_code = ioctl(fd, KILL, data)) == 0) {
                                fprintf(stderr, "Successfuly killed process pid: %d.\n\n", pid);
                        } else if (ret_code == -1) { 
                                fprintf(stderr, "No process with pid : %d\n", pid);
                        } else {
                                fprintf(stderr, "Error when trying to kill : %d\n", pid);
                        }
                        if(!is_bg)
                                free(data);
                       // fprintf(stderr, "Background kill id: %d.\n\n", data->bg_id);
                  
                  } else if (strncmp("FG", buffer, 4) == 0) {
                        fprintf(stderr, "FG\n");
                  } else if (strncmp("WAIT", buffer, 4) == 0) {
                        int size, i, ret_code;
                        struct wait_data *data = malloc(sizeof(*data));
                        
                        for(i=0; i<NB_MAX_PID; i++)
                                data->pids[i] = 0;
                        size = get_wait_params(buffer, data->pids, &size);
                        if (size <= 0) {
                                fprintf(stderr, "Usage : WAIT <pid> [<pid> ...]\n");
                                free(data);
                                continue;
                        } else if(size > 10) {
                                fprintf(stderr, "Only ten first pids are taken in account.\n");
                        }
                        
                        if ((ret_code = ioctl(fd, WAIT, data)) == 0) {
                                fprintf(stderr, "End wait: %s Exit code: %d.\n\n", data->buf, WEXITSTATUS(data->exit_code));
                        } else if (ret_code == -1) {
                                fprintf(stderr, "He said %s\n", data->buf);
                        } else {
                                fprintf(stderr, "Oups unexpected error.\n");
                        }
                        free(data);
                  } else if (strncmp("MEMINFO", buffer, 7) == 0) {
                        char c_bg;
                        int ret_code;
                        struct no_data *data = malloc(sizeof(*data));
                        sscanf(buffer, "MEMINFO %c\n", &c_bg);
                        data->is_bg = c_bg == '&' ? 1 : 0;
                        ret_code = ioctl(fd, MEMINFO, data);
                        
                        if (ret_code == 0) {
                                fprintf(stderr, "%s\n", data->buf);
                        } else {
                                fprintf(stderr, "Unexpected error.\n");
                        }
                        if (!data->is_bg)
                                free(data);
                  } else if (strncmp("MODINFO", buffer, 7) == 0) {
                        struct modinfo_data *data = malloc(sizeof(*data));
                        int is_bg;
                        
                        if (!get_modinfo_params(buffer, data->name, &is_bg)) {
                                fprintf(stderr, "Usage : MODINFO <pid> [&]\n");
                                free(data);
                                continue;
                        }
                        data->is_bg = is_bg;

                        if (ioctl(fd, MODINFO, data) == 0) {
                                fprintf(stderr, "%s\n", data->buf);
                        } else {
                                fprintf(stderr, "No such module %s\n", data->name);
                        }
                        free(data);
                  } else {
                        buffer[strlen(buffer)-1] = 0;
                        fprintf(stderr, "Unknown command:  '%s'\n\n", buffer);
                  }

                memset(buffer, 0, BUFFER_SIZE);
        }

        close(fd);
        return 0;
}
