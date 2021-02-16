#include "common.h"

int shell_cat(char **args) {
        // if no arguments are specified read from standard input
        printf("%s\n", *args);
        if(*(++args) == NULL) {
                // display the data read from input on output
                char buffer[1024];
                int nbytes;
                while(1){
                        // read from input
                        nbytes = read(0, buffer, 1024);
                        if(nbytes < 0) {
                                perror("\nminsh");
                                break;
                        }
                        else if(nbytes == 0) { // end of file
                                write(1, "\n", 1);
                                break;
                        }
                        // write to output
                        if(write(1, buffer, nbytes) < 0) { // error
                                perror("\nminsh");
                                break;
                        }
                }
                return 1;
        }

        int fd;
        // loop through the available argument (files)
       while(*args != NULL) {
              // open file
              if ((fd = open(*args, O_RDONLY)) < 0) {
                      fprintf(stderr, "\nminsh: %s - ", *args);
                      perror("");
                      return 0;
              }
              printf("\n\n%s:\n\n", *args);

              // display data from file
              char buffer[1024];
              int nbytes;
              while(1) {
                // read from file
                nbytes = read(fd, buffer, 1024);
                if(nbytes < 0) {
                        perror("\nminsh");
                        break;
                }
                else if(nbytes == 0) { // end of file
                        write(1, "\n", 1);
                        break;
                }
                // write to output
                if(write(1, buffer, nbytes) < 0) { // error
                        perror("\nminsh");
                        break;
                }
                ++args;
              }
              close(fd);
       }
       printf("\n\n");
       return 1;
}
