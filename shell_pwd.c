/*
 * shell_pwd: implements the working of pwd of Unix shell
 */
#ifndef COMMON_H
#define COMMON_H
#include "common.h"
#endif

extern char PWD[1024]; // present working directory
int shell_pwd(char **args) {
        printf("%s\n", PWD);
        return 1;
}
