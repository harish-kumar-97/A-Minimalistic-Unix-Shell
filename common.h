#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int shell_cd(char **args);
int shell_exit(char **args);
int shell_pwd(char **args);
int shell_echo(char **args);
int shell_mkdir(char **args);
int shell_clear(char **args);
int shell_cat(char **args);

#define BUILTIN_COMMANDS 8
