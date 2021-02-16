/*
 * shell_echo: implements the functionality of echo of Unix shell
 */
#ifndef COMMON_H
#define COMMON_H
#include "common.h"
#endif

int shell_echo(char **args) {
        int i = 1;
        while(1) {
                // end of arguments
                if(args[i] == NULL) {
                        break;
                }
                printf("%s ", args[i]);
                i++;
        }
        printf("\n");
}
