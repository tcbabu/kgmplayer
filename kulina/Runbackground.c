#include <stdio.h>
#include <kulina.h>
#include <signal.h>

void *RunMonitorJoin(void *arg);

int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int kgffmpeg(int,char **);
int ProcessSkip(int pip0,int pip1,int Pid);
int ProcessPrint(int pip0,int pip1,int Pid);
int ProcessToPipe(int pip0,int pip1,int Pid);
int Mencoder(int,char **);
void *RunkgGetFiles(void *arg,char *Filter);
int updateoutfile(char *outfile);

extern int Jpipe[2];
extern int Jstat[2];
extern int MonPipe;

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
int KillJob(int pid) {
	if(pid > 1) {
	  kill(pid,SIGTERM);
	  WAIT(pid);
	}
	return 1;
}
int runfunctionbkgr(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **)) {
  int pid=0,id;
  int status;
  char buff[500];

  if( (pid =fork())!= 0) {
//    kgSplashMessage(NULL,100,100,300,40,(char *)"Send for Processing",1,0,15);
//    sleep(2);
    return pid;
  }
  
  id = getpid();
  if(pipe(Jpipe) < 0) exit(0);
  if(pipe(Jstat) < 0) exit(0);
  MonPipe = Jpipe[0];
  
  if ((pid=fork())==0) {
    close(Jpipe[0]);
    close(Jstat[1]);
#if 1  // modify as required
     sprintf(buff,"Executing... PLEASE WAIT\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"PLEASE WAIT till the window closes\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"You can cancel job if you wish\n");
     write(Jpipe[1],buff,strlen(buff));
     runfunction(job,ProcessOut,function);
#endif 
     close(Jpipe[1]);
     close(Jstat[0]);
     exit(0);
  }
  else {
     close(Jpipe[1]);
     close(Jstat[0]);
     RunMonitorJoin(NULL);
#if 0
     kill(pid,9);
     waitpid(pid,&status,0);
#else
     KillJob(pid);
#endif
     exit(0);
  }
}

int runnormalisebkgr(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **),char *outfile) {
  int pid=0,id;
  int status;
  char buff[500];

  if( (pid =fork())!= 0) {
//TCB
//    kgSplashMessage(NULL,100,100,300,40,(char *)"Send for Processing",1,0,15);
//    sleep(2);
    return pid;
  }
  
  id = getpid();
  if(pipe(Jpipe) < 0) exit(0);
  if(pipe(Jstat) < 0) exit(0);
  MonPipe = Jpipe[0];
  
  if ((pid=fork())==0) {
    close(Jpipe[0]);
    close(Jstat[1]);
#if 1  // modify as required
     sprintf(buff,"Executing... PLEASE WAIT\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"PLEASE WAIT till the window closes\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"You can cancel job if you wish\n");
     write(Jpipe[1],buff,strlen(buff));
     runfunction(job,ProcessOut,function);
#endif 
     close(Jpipe[1]);
     close(Jstat[0]);
     exit(0);
  }
  else {
     close(Jpipe[1]);
     close(Jstat[0]);
     RunMonitorJoin(NULL);
#if 0
     kill(pid,9);
     waitpid(pid,&status,0);
#else
     KillJob(pid);
#endif
     updateoutfile(outfile);
     exit(0);
  }
}
int RunNormalisebkgr(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **),char *infile,char *outfile) {
  int pid=0,id;
  int status;
  char buff[500];

  if( (pid =fork())!= 0) {
//TCB
//    kgSplashMessage(NULL,100,100,300,40,(char *)"Send for Processing",1,0,15);
//    sleep(2);
    return pid;
  }
  
  id = getpid();
  if(pipe(Jpipe) < 0) exit(0);
  if(pipe(Jstat) < 0) exit(0);
  MonPipe = Jpipe[0];
  
  if ((pid=fork())==0) {
    close(Jpipe[0]);
    close(Jstat[1]);
#if 1  // modify as required
     sprintf(buff,"Executing... PLEASE WAIT\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"Files: %s %s\n",infile,outfile);
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"PLEASE WAIT till the window closes\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"You can cancel job if you wish\n");
     write(Jpipe[1],buff,strlen(buff));
     runfunction(job,ProcessOut,function);
#endif 
     close(Jpipe[1]);
     close(Jstat[0]);
     exit(0);
  }
  else {
     char buff[500];
     close(Jpipe[1]);
     close(Jstat[0]);
     RunMonitorJoin(NULL);
#if 0
     kill(pid,9);
     waitpid(pid,&status,0);
#else
     KillJob(pid);
#endif
     sprintf(buff,"cp %s %s",outfile,infile);
     system(buff);
     remove(outfile);
     exit(0);
  }
}
