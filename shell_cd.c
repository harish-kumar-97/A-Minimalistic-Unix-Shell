/*
 * shell_cd: implements the working of change directory
 */
#ifndef COMMON_H
#define COMMON_H
#include "common.h"
#endif

extern char PWD[1024]; // present working directory

int shell_cd(char **args) {
        if (args[1] == NULL) {
                fprintf(stderr, "minsh: atleast one argument is required.\n");
        } else if (chdir(args[1]) < 0) {
                perror("minsh");
        }
        // update present working directory
        getcwd(PWD, sizeof(PWD));
        return 1;
}

