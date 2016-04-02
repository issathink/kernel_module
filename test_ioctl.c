
//#include <linux/kernel.h>
//#include <linux/fs.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

#include "commandes.h"
#define BUFFERSIZE      255

int main(int argc, char *argv[]) {
         int fd = open("/dev/commandes_ioctl", O_RDONLY);
        char buf[255];
        char *text = calloc(1,1), buffer[BUFFERSIZE];

         //    printf("Enter a message: \n");

        while( fgets(buffer, BUFFERSIZE , stdin) ) {

                  if(text == NULL) {
                        fprintf(stderr, "error realloc\n");
                        return EXIT_FAILURE;
                  }
                  strcat(text, buffer);
                  
                  
                  if(strncmp("LIST", buffer, 4) == 0) {
                        
                       /* if (ioctl(fd, LIST, buf) == 0)
                                fprintf(stderr, "list : %s\n", buf);
                        else
                                fprintf(stderr, "ERREUR LIST\n");*/
                        fprintf(stderr, "LIST command\n");
                  } /*else if(strncmp("FG", text, 4) == 0) {
                        fprintf(stderr, "FG\n");
                  } else if(strncmp("KILL", text, 4) == 0) {
                        fprintf(stderr, "KILL.\n");
                  } else if(strncmp("WAIT", text, 4) == 0) {
                        fprintf(stderr, "Commande inconnue.\n");
                  } else if(strncmp("MEMINFO", text, 4) == 0) {
                        fprintf(stderr, "Commande inconnue.\n");
                  } else if(strncmp("MODINFO", text, 4) == 0) {
                        fprintf(stderr, "Commande inconnue.\n");
                  }*/ else {
                        fprintf(stderr, "Commande inconnue.\n");
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
