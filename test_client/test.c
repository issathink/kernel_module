#include "tools.h"

// #include "commandes.h"


int main() {
         int fd = open("/dev/commandes_ioctl", O_RDONLY);
        // char buf[255];
        char buffer[BUFFER_SIZE];

        while( fgets(buffer, BUFFER_SIZE , stdin) ) {
                  if(strncmp("LIST", buffer, 4) == 0) {
                        
                       /* if (ioctl(fd, LIST, buf) == 0)
                                fprintf(stderr, "list : %s\n", buf);
                        else
                                fprintf(stderr, "ERREUR LIST\n");*/
                        fprintf(stderr, "LIST command\n");
                  } else if(strncmp("KILL ", buffer, 5) == 0) {
                        int sig, pid;
                        kill_data *data = malloc(sizeof(kill_data));
                        
                        if(!get_kill_params(buffer, &sig, &pid)) {
                                fprintf(stderr, "Usage : kill <signal> <pid>\n");
                                continue;
                        }
                        data->pid = pid;
                        data->sig = sig;
                        
                        if(ioctl(fd, KILL, data) == 0) {
                                fprintf(stderr, "KILL OK.\n");
                                fprintf(stderr, "Chaine: '%s'\n", data->buf);
                        } else { 
                                fprintf(stderr, "Error kill\n");
                        }        
                  } /*else if(strncmp("FG", text, 4) == 0) {
                        fprintf(stderr, "FG\n");
                  } else if(strncmp("WAIT", text, 4) == 0) {
                        fprintf(stderr, "Commande inconnue.\n");
                  } else if(strncmp("MEMINFO", text, 4) == 0) {
                        fprintf(stderr, "Commande inconnue.\n");
                  } else if(strncmp("MODINFO", text, 4) == 0) {
                        fprintf(stderr, "Commande inconnue.\n");
                  }*/ else {
                        fprintf(stderr, "Commande '%s' inconnue.\n", buffer);
                  }
                  
                /* printf("%s\n", buffer); */
                
                /* printf("\ntext:\n%s",text); */
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

    return 0;
}
