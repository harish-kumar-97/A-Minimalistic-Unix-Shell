/*
 * shell_mkdir: implements make directory of Unix shell
 */
#ifndef COMMON_H
#define COMMON_H
#include "common.h"
#endif

int shell_mkdir(char **args) {
        ++args;
       if(*args == NULL) {
                fprintf(stderr, "minsh; missing directory name\n");
               return 1;
       }
       while(*args != NULL) {
               // create the directory
                if(mkdir(*args, 0755) < 0) {
                        fprintf(stderr, "minsh: %s - ", *args);
                        perror("");
                        continue;
                }
                ++args;
       }
       return 1;
}
