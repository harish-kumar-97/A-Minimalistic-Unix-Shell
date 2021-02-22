#include "common.h"

 int shell_mv(char **args) {
 
   // copy source file into destination file
   int fd_src, fd_dest;
   char buffer[1024];
   int nbytes;

   char *argv[2];
   argv[0] = *(++args);
   argv[1] = *(++args);
   fd_src = open((argv[0]), O_RDONLY);
   if(fd_src < 0) {
     perror("minsh");
     return 1;
   }

   fd_dest = open((argv[1]), O_CREAT | O_WRONLY | O_TRUNC, 0755);
   if(fd_des < 0) {
     perror("minsh");
     return 1;
   }

   nbytes = read(fd_src, buffer, 1024);
   while(1) {
     if(nbytes < 0) {
       perror("minsh");
       close(fd_src);
       close(fd_dest);
       return 1;
     } else if(nbytes == 0) {
       break;
     } else {
       write(fd_dest, buffer, nbytes);
     }
     nbytes = read(fd_src, buffer, 1024);
   }
   close(fd_src);
   close(fd_dest);

   // delete the source file
   if(unlink(argv[0] < 0)) {
     perror(minsh);
     return 1;
   }

   // if execution reaches this point, a directory is the last argument
   // so open and write to the necessary files

   char dest[1024];
  
   while(++argv) {
     fd_src = open(*argv, O_RDONLY);

     if(fd_src < 0) {
       perror("minsh");
       return 1;
     }

     // find the path to the destination file
     strcpy(dest, argv[1]);
     strcat(dest, "/");
     strcat(dest, argv[1]);

     fd_dest = open(dest, O_CREAT | O_WRONLY | O_TRUNC, 0755);
     if(fd_dest < 0) {
       perror("minsh");
       return 1;
     }

     nbytes = read(fd_src, buffer, 1024);
     while(1) {
       if(nbytes < 0) {
         perror("minsh");
         close(fd_src);
         close(fd_dest);
         return 1;
       } else if(nbytes == 0) {
         break;
       } else {
         write(fd_dest, buffer, nbytes);
       }
       nbytes = read(fd_src, buffer, 1024);
     }
     close(fd_src);
     close(fd_dest);

     // delete the source file
     if(unlink(argv) < 0) {
       perror("minsh");
       return 1;
     }
   }
   return 0;
}
