#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include "tools.h"

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
int get_kill_params(char *buffer, int *sig, int *pid, int *is_bg) {
        char sig_c[20], pid_c[20], *endptr, c_bg;
        memset(sig_c, 0, 20);
        memset(pid_c, 0, 20);
        
        sscanf(buffer, "KILL %s %s %c\n", sig_c, pid_c, &c_bg);        
        if(!is_string_entier(sig_c) || !is_string_entier(pid_c))
                return 0;
                
        *sig = strtol(sig_c, &endptr, 10);
        if (errno == ERANGE || (errno != 0 && *sig == 0)) return 0;
        *pid = strtol(pid_c, &endptr, 10);
        if (errno == ERANGE || (errno != 0 && *pid == 0)) return 0;
        
        *is_bg = 0;
        if(c_bg == '&')
                *is_bg = 1;
        
        return 1;
}

/*
 * Fill in name with the first parameter of modinfo.
 * return 1 everything is OK, 0 otherwise
 */
int get_modinfo_params(char *buffer, char *name, int *is_bg) {
        char c_bg;

        if (sscanf(buffer, "MODINFO %s %c\n", name, &c_bg) == EOF)
                return 0;

        *is_bg = 0;
        if(c_bg =='&')
                *is_bg = 1;
        
        return 1;
}

/*
 * Decode buffer string and fill the params table with pids.
 * return number of parameters 
 */
int get_wait_params(char *buffer, int params[], int *size) {
        int i = 0, j = 0, ind = 0;
        char tmp[NB_MAX_PID+1], *endptr;
        
        memset(tmp, 0, NB_MAX_PID+1);
        while (buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\0') i++;
        if(buffer[i] == '\0' || buffer[i] == '\n') return 0;

        while (buffer[i] != '\n' && buffer[i] != '\0') {
                ind = 0;
                if(buffer[i] == ' ') {
                        i++;
                        while(buffer[i] != ' ' && buffer[i] != '\n' && ind <= NB_MAX_PID)
                                tmp[ind++] = buffer[i++];

                        if (ind > NB_MAX_PID) {
                                i++;
                                // fprintf(stderr, "Ignored %s\n", tmp);
                        } else if (buffer[i] == ' ' || buffer[i] == '\n') {                                
                                if (!is_string_entier(tmp)) {
                                        // fprintf(stderr, "Ignored %s\n", tmp);
                                 } else { 
                                        params[j++] = strtol(tmp, &endptr, 10);
                                        if (j >= 10)
                                                break;
                                       //  fprintf(stderr, "params[%d] = %d\n", j, params[j-1]);
                                 }
                        } else {
                                fprintf(stderr, "What %c\n", buffer[i]);
                                i++;
                        }
                }
                memset(tmp, 0, NB_MAX_PID+1);
        }
        *size = j;
        return j;
}

/*
 * Fill in id with the parameter if it's a real integer.
 * return 1 OK, 0 first parameter not an int
 */
int get_fg_params(char *buffer, int *id)
{
        char id_c[20], *endptr;
        memset(id_c, 0, 20);

        if (sscanf(buffer, "FG %s\n", id_c) == EOF)
                return 0;
        if(!is_string_entier(id_c))
                return 0;
                
        *id = strtol(id_c, &endptr, 10);
        if (errno == ERANGE || (errno != 0 && *id == 0)) return 0;

        return 1;
}
