#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define WAIT(pid) {\
  pid_t w;\
  int status;\
  do { \
                   w = waitpid(pid, &status, WUNTRACED | WCONTINUED); \
                   if (w == -1) { \
                       perror("waitpid"); \
                       exit(EXIT_FAILURE); \
                   }\
               } while (!WIFEXITED(status) && !WIFSIGNALED(status));\
}
int main(int argc,char **argv) {
  int pid,status;
  char command[500];
  if(argc<3) {
      printf("Usage: cleantmpdir  <pid of process to wait for> <dir>\n");
      return 1;
  }
  sscanf(argv[1],"%d",&pid);
  WAIT(pid);
  sprintf(command,"rm -r -f %-s",argv[2]);
//  printf("%s\n",command);
  system(command);
  return 0;
}
