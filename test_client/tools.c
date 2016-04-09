#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

/* 
 * Fonction pour verifier si la chaine passee en parametre  est un entier. 
 * return valeur non nulle si la chaine est composee d'eniters 0 sinon
 */
int is_string_entier(char *buf) {
        unsigned int i;
        if(strlen(buf) <= 0)
                return 0;
        for(i=0; i<strlen(buf); i++)
                if(!isdigit(buf[i]))
                        return 0;
        return 1;
}

/* 
 * Rempli les variables sig et pid en parsant la chaine buffer.
 * return valeur non nulle si la chaine respecte la syntaxe du kill 0 sinon
 */
int get_kill_params(char *buffer, int *sig, int *pid) {
        char sig_c[20], pid_c[20], *endptr;
        memset(sig_c, 0, 20);;
        memset(pid_c, 0, 20);
        
        sscanf(buffer, "KILL %s %s\n", sig_c, pid_c);        
        if(!is_string_entier(sig_c) || !is_string_entier(pid_c))
                return 0;
                
        *sig = strtol(sig_c, &endptr, 10);
        if (errno == ERANGE || (errno != 0 && *sig == 0)) return 0;
        *pid = strtol(pid_c, &endptr, 10);
        if (errno == ERANGE || (errno != 0 && *pid == 0)) return 0;
        
        return 1;
}

int get_modinfo_param(char *buffer, char *name) {
        int i=0;
        
        if (sscanf(buffer, "MODINFO %s\n", name) == EOF)
                return 0;
              
        fprintf(stderr, "modinfo name: %s\n", name);
        return 1;
}

