#include "tools.h"

// #include "commandes.h"


int main() {
         int fd = open("/dev/commandes_ioctl", O_RDONLY);
        // char buf[255];
        char buffer[BUFFER_SIZE];

        while (fgets(buffer, BUFFER_SIZE , stdin)) {
                  if (strncmp("LIST", buffer, 4) == 0) {
                        no_data *data = malloc(sizeof(no_data));
                       if (ioctl(fd, LIST, data) == 0)
                                fprintf(stderr, "%s\n", buf);
                        else
                                fprintf(stderr, "ERREUR LIST\n\n");
                        free(data);
                  } else if (strncmp("KILL ", buffer, 5) == 0) {
                        int sig, pid, ret_code;
                        kill_data *data = malloc(sizeof(kill_data));
                        
                        if (!get_kill_params(buffer, &sig, &pid)) {
                                fprintf(stderr, "Usage : KILL <signal> <pid>\n");
                                memset(buffer, 0, BUFFER_SIZE);
                                continue;
                        }
                        data->pid = pid;
                        data->sig = sig;
                        if (pid == getpid()) {
                                fprintf(stderr, "Attempt to kill me, nice try.\n");
                                memset(buffer, 0, BUFFER_SIZE);
                                continue;
                        }
                        
                        if ((ret_code = ioctl(fd, KILL, data)) == 0) {
                                fprintf(stderr, "Successfuly killed process pid: %d.\n\n", pid);
                        } else if (ret_code == -1) { 
                                fprintf(stderr, "No process with pid : %d\n", pid);
                        } else {
                                fprintf(stderr, "Error when trying to kill : %d\n", pid);
                        }
                        free(data);
                  } /*else if (strncmp("FG", text, 4) == 0) {
                        fprintf(stderr, "FG\n");
                  } else if (strncmp("WAIT", text, 4) == 0) {
                        fprintf(stderr, "Commande inconnue.\n");
                  } */ else if (strncmp("MEMINFO", buffer, 7) == 0) {
                        no_data *data = malloc(sizeof(no_data));
                        
                        if (ioctl(fd, MEMINFO, data) == 0) {
                                fprintf(stderr, "%s\n", data->buf);
                        } else {
                                fprintf(stderr, "Unexpected error.\n");
                        }
                        free(data);
                  } else if (strncmp("MODINFO", text, 7) == 0) {
                        modinfo_data *data = malloc(sizeof(modinfo_data));
                        
                        if (!get_modinfo_param(buffer, data->name)) {
                                fprintf(stderr, "Usage : MODINFO <pid>\n");
                                free(data);
                                continue;
                        }

                        if (ioctl(fd, MODINFO, data) == 0) {
                                fprintf(stderr, "%s\n", data->buf);
                        } else {
                                fprintf(stderr, "No such module %s\n", data->name);
                        }
                        free(data);
                  } else {
                        fprintf(stderr, "Commande '%s' inconnue.\n", buffer);
                  }
                  
                /* printf("%s\n", buffer); */
                
                /* printf("\ntext:\n%s",text); */
                memset(buffer, 0, BUFFER_SIZE);
        }

        /*  if (ioctl(fd, LIST, buf) == 0)
        printf("list : %s\n", buf);
        else
        dprintf(2, "ERREUR\n");*/

        /* if (ioctl(fd, WHO, "beer") == 0)
        printf("Nom est changé\n");
        else
        dprintf(2, "ERREUR\n");

        if (ioctl(fd, HELLO, buf) == 0)
        printf("%s\n", buf);
        else
        dprintf(2, "ERREUR\n");*/

        close(fd);
        return 0;
}
