/****************************************************************
 * File: miniShell.c
 * Author: Harish
 * Description: A minimalistic version of the unix shell
 *              implemented using C programming language.
 ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUILTIN_COMMANDS 6

/* environmental variables */
char PWD[1024]; // present working directory
char PATH[1024]; // path to find the commands

/* built-in command names */
char *builtin[] = {"cd", "exit", "help", "pwd", "echo", "mkdir"};

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

int shell_exit(char **args) {
        return 0;
}

int shell_pwd(char **args) {
        printf("%s\n", PWD);
        return 1;
}

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

int shell_help(char **args) {
        printf("\nA mini implementation of the Unix Shell.\n");
        printf("\nCommands implemented: ");
        printf("\n\t- help");
        printf("\n\t- exit");
        printf("\n\t- cd dir");
        printf("\n\t- pwd");
        printf("\n\t- echo [string to echo]");
        printf("\n\t- clear");
        printf("\n\t- ls [-all] [dir1 dir2 ...]");
        printf("\n\t- cp source target (or) cp file1 [file2 ...] dir");
        printf("\n\t- mv source target (or) mv file1 [file2 ...] dir");
        printf("\n\t- rm file1 [file2 ...]");
        printf("\n\t- mkdir dir1 [dir2 ...]");
        printf("\n\t- rmdir dir1 [dir2 ...]");
        printf("\n\t- ln [-s] source target");
        printf("\n\t- cat [file1 file2 ...]");
        printf("\n\n");
        printf("\n\n");
        printf("Other features: ");
        printf("\n\t* Input, Output and Error Redirection (<, <<, >, >>, 2>, 2>>, respectively");
        printf("\n\t* Example: ls -i >> outfile 2> errfile [space mandatory around redirection operators!]");
        printf("\n\n");
        return 1;
}

char *read_command_line(void) {
        int position = 0;
        int buf_size = 1024;
        char *command = (char *)malloc(sizeof(char) * 1024);
        char c;

        c = getchar();
        while(c != EOF && c != '\n') {
                command[position] = c;

                // reallocate buffer as and when needed
                if(position >= buf_size) {
                        buf_size += 64;
                        command = realloc(command, buf_size);
                }
                position++;
                c = getchar();
        }
        return command;
}

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

int (* builtin_function[]) (char **) = {
        &shell_cd, 
        &shell_exit,
        &shell_help,
        &shell_pwd,
        &shell_echo,
        &shell_mkdir
};

char **split_command_line(char *command) {
        int position = 0;
        int no_of_tokens = 64;
        char **tokens = malloc(sizeof(char *) * no_of_tokens);
        char delim[2] = " ";

        // split the command line  into tokens with space as delimiter
        char *token = strtok(command, delim);
        while(token != NULL) {
                tokens[position] = token;
                position++;
                token = strtok(NULL, delim);
        }
        tokens[position] = NULL;
        return tokens;
}

int start_process(char **args) {
        int status;
        pid_t pid, wpid;
        
        pid = fork();

        if(pid == 0) { // it's the child process
                // find the path of the command
                char cmd_dir[1024];
                strcpy(cmd_dir, PATH);
                strcat(cmd_dir, args[0]);

                // execute the required process
                if(execv(cmd_dir, args) == -1) {
                        perror("minsh");
                }
                exit(EXIT_FAILURE);
        }
        else if(pid < 0) {
                perror("minsh");
        }
        else {
                do {
                        wpid = waitpid(pid, &status, WUNTRACED);
                } while(!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        return 1;
}

int shell_execute(char **args) {
        if(args[0] == NULL) {
                return 1;
        }

        int std_in, std_out, std_err;
        std_in = dup(0);
        std_out = dup(1);
        std_err = dup(2);

        // check if redirection operators are present
        int i = 1;
        while(args[i] != NULL) {
                if(strcmp(args[i], "<") == 0) { // input redirection
                        int inp = open(args[i+1], O_RDONLY);
                        if(inp < 0) {
                                perror("minsh");
                                return 1;
                        }

                        if(dup2(inp, 0) < 0) {
                                perror("minsh");
                                return 1;
                        }
                        close(inp);
                        args[i] = NULL;
                        args[i+1] = NULL;
                        i += 2;
                }
                else if (strcmp(args[i], "<<") == 0) {  // input redirection
                        int inp = open(args[i+1], O_RDONLY);
                        if(inp < 0) {
                                perror("minsh");
                                return 1;
                        }
                        if(dup2(inp, 0) < 0) {
                                perror("minsh");
                                return 1;
                        }
                        close(inp);
                        args[i] = NULL;
                        args[i+1] = NULL;
                        i += 2;
                }
                else if (strcmp(args[i], ">") == 0) {   // output redirection
                        int out = open(args[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0755);
                        if(out < 0) {
                                perror("minsh");
                                return 1;
                        }
                        if(dup2(out, 1) < 0) {
                                perror("minsh");
                                return 1;
                        }
                        close(out);
                        args[i] = NULL;
                        args[i+1] = NULL;
                        i += 2;
                }
                else if (strcmp(args[i], ">>") == 0) {  // output redirection (append)
                        int out = open(args[i+1], O_WRONLY | O_APPEND | O_CREAT, 0755);
                        if(out < 0) {
                                perror("minsh");
                                return 1;
                        }
                        if(dup2(out, 1) < 0) {
                                perror("minsh");
                                return 1;
                        }
                        close(out);
                        args[i] = NULL;
                        args[i+1] = NULL;
                        i += 2;
                }
                else if(strcmp(args[i], "2>") == 0) {   // error redirection
                        int err = open(args[i+1], O_WRONLY | O_CREAT, 0755);
                        if(err < 0) {
                                perror("minsh");
                                return 1;
                        }
                        if(dup2(err, 2) < 0) {
                                perror("minsh");
                                return 1;
                        }
                        close(err);
                        args[i] = NULL;
                        args[i+1] = NULL;
                        i += 2;
                }
                else if(strcmp(args[i], "2>>") == 0) { // error redirection
                        int err = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0755);
                        if(err < 0) {
                                perror("minsh");
                                return 1;
                        }
                        if(dup2(err, 2) < 0) {
int (* builtin_function[]) (char **) = {
        &shell_cd, 
        &shell_exit,
        &shell_help,
        &shell_pwd,
        &shell_echo
};
                                perror("minsh");
                                return 1;
                        }
                        close(err);
                        args[i] = NULL;
                        args[i+1] = NULL;
                        i += 2;
                }
                else {
                        i++;
                }
        }
        // if the command is a built-in command, execute that function
        for(i = 0; i < BUILTIN_COMMANDS; i++) {
                if(strcmp(args[0], builtin[i]) == 0) {
                        int ret_status = (* builtin_function[i])(args);

                        // restore the standard input and output file descriptors
                        dup2(std_in, 0);
                        dup2(std_out, 1);
                        dup2(std_err, 2);

                        return ret_status;
                }
        }
        // for other commands, execute a child process
        int ret_status = start_process(args);
        // restore the standard input and output file descriptors
        dup2(std_in, 0);
        dup2(std_out, 1);
        dup2(std_err, 2);

        return ret_status;
}

void shell_loop(void) {
        // display help at startup
        int status = shell_help(NULL);
        char *command_line;
        char **arguments;
        status = 1;

        while(status) {
                printf("MINI-SHELL: ");
                command_line = read_command_line();
                if(strcmp(command_line, "") == 0) {
                        continue;
                }
                arguments = split_command_line(command_line);
                status = shell_execute(arguments);
        }
}

/*driver function: main */
int main(int argc, char **argv) {
        // shell initialization
        getcwd(PWD, sizeof(PWD));
        strcpy(PATH, PWD);
        strcat(PATH, "/cmds/");

        shell_loop();

        return 0;
}

