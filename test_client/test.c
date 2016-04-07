#include "tools.h"

// #include "commandes.h"


int main() {
         int fd = open("/dev/commandes_ioctl", O_RDONLY);
        char buf[255];
        char buffer[BUFFERSIZE];

        while( fgets(buffer, BUFFERSIZE , stdin) ) {
                  if(strncmp("LIST", buffer, 4) == 0) {
                        
                       /* if (ioctl(fd, LIST, buf) == 0)
                                fprintf(stderr, "list : %s\n", buf);
                        else
                                fprintf(stderr, "ERREUR LIST\n");*/
                        fprintf(stderr, "LIST command\n");
                  } else if(strncmp("KILL ", buffer, 5) == 0) {
                        int sig, pid;
                        if(!get_kill_params(buffer, &sig, &pid)) {
                                fprintf(stderr, "Usage : kill <signal> <pid>\n");
                                continue;
                        }
                  
                        if(ioctl(fd, LIST, buf) == 0) 
                  
                        fprintf(stderr, "KILL.\n");
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
