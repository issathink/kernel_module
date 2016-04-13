#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

/* 
 * Check if buf is a string of digits.
 * return 1 everything is OK, 0 otherwise
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
 * Parse buffer and fill sig and pid.
 * return non zero value if OK, 0 otherwise
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

/*
 * Fill in name with the first parameter of modinfo.
 * return 1 everything is OK, 0 otherwise
 */
int get_modinfo_param(char *buffer, char *name) {
        if (sscanf(buffer, "MODINFO %s\n", name) == EOF)
                return 0;
              
        fprintf(stderr, "modinfo name: %s\n", name);
        return 1;
}

int get_wait_params(char *buffer, int params[], int *size) {
        int i = 0, j = 0, ind = 0, nb = 0;
        char tmp[11], *endptr;
        
        memset(tmp, 0, 11);
        while (buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\0') i++;
        if(buffer[i] == '\0' || buffer[i] == '\n') return 0;

        while (buffer[i] != '\n' && buffer[i] != '\0') {
                ind = 0;
                if(buffer[i] == ' ') {
                        i++;
                        while(buffer[i] != ' ' && buffer[i] != '\n' && ind <= 10)
                                tmp[ind++] = buffer[i++];

                        if (ind > 10) {
                                i++;
                                // fprintf(stderr, "Ignored %s\n", tmp);
                        } else if (buffer[i] == ' ' || buffer[i] == '\n') {                                
                                if (!is_string_entier(tmp)) {
                                        // fprintf(stderr, "Ignored %s\n", tmp);
                                 } else { 
                                        params[j++] = strtol(tmp, &endptr, 10);
                                       //  fprintf(stderr, "params[%d] = %d\n", j, params[j-1]);
                                 }
                        } else {
                                fprintf(stderr, "What %c\n", buffer[i]);
                                i++;
                        }
                }
                memset(tmp, 0, 11);
        }
        *size = j;
        return j;
}

/*** Self explanatory. ***/
int get_wait_params_size(char *buffer) {
        int i = 0, size = 0;
        
        while (buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\0') i++;
        if(buffer[i] == '\0' || buffer[i] == '\n') return 0;
        
        size = 0; i++;
        while (buffer[i] != '\0') {
                if((buffer[i] == ' ' || buffer[i] == '\n'))
                        if((buffer[i+1] != '\0' && buffer[i+1] == '\n'))
                                size++;
                i++;
        }
        
        return size;
}

/*** Self explanatory. ***/
int get_wait_number_of_params(char *buffer) {
        int i = 0, j = 0;
        
        while (buffer[i] != ' ') i++;
        if(j == '\0') return 0;
        
        j = 1;
        while (buffer[i] != '\0') {
                if(buffer[i] == ' ' || buffer[i] == '\n')
                        j++;
                i++;
        }
        
        return j;
}

