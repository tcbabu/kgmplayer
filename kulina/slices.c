#include <stdio.h>
#include <kulina.h>
#include "mediainfo.h"


int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int kgffmpeg(int,char **);
int ffmpegfun(int,char **);
int ProcessSkip(int pip0,int pip1,int Pid);
int ProcessPrint(int pip0,int pip1,int Pid);
int ProcessToPipe(int pip0,int pip1,int Pid);
int Mencoder(int,char **);
void *RunkgGetFiles(void *arg,char *Filter);
MEDIAINFO * GetMediaInfo(char *flname);

extern MEDIAINFO Minfo;

extern int Jpipe[2];
extern int Jstat[2];
extern int MonPipe;

int MakeVideoSlices(char * flname,char *folder,int tslice) {

  int pid=0,id;
  int status;
  char buff[5000];
  int totsec=0;
  int ssec,esec;
  int n;
 
  if( CheckMedia(flname) == 0 ) {
    kgSplashMessage(NULL,100,100,300,40,"Error: Not Video",1,0,15);
    return 0;
  }
  if( (pid =fork())!= 0) {
    kgSplashMessage(NULL,100,100,300,40,"Send for Processing",1,0,15);
    sleep(2);
    return pid;
  }
  totsec = Minfo.TotSec+0.1;
  id = getpid();
  if(pipe(Jpipe) < 0) exit(0);
  if(pipe(Jstat) < 0) exit(0);
  MonPipe = Jpipe[0];
  
  if ((pid=fork())==0) {
    close(Jpipe[0]);
    close(Jstat[1]);
#if 1  // modify as required
     sprintf(buff,"Execiting... PLEASE WAIT\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"PLEASE WAIT till the window closes\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"You can cancel job if you wish\n");
     write(Jpipe[1],buff,strlen(buff));
     ssec=0;
     esec = tslice;
     n=1;
     while (ssec < totsec) {
#ifdef D_X264
       if(esec < totsec) {
        sprintf(buff,"ffmpegfun -ss %-d -t %-d -i \"%-s\" "
           " -y -f mp4 -vcodec libx264  -b:v 3000K -aq 0 "
           " -c:a libmp3lame \"%-s/Frm%-5.5d\"",
           ssec,tslice,flname,folder,n);
       }
       else {
        sprintf(buff,"ffmpegfun -ss %-d  -i \"%-s\" "
           " -y -f mp4 -vcodec libx264  -b:v 3000K -aq 0 "
           " -c:a libmp3lame \"%-s/Frm%-5.5d\"",
           ssec,flname,folder,n);
       }
#else
       if(esec < totsec) {
        sprintf(buff,"ffmpegfun -ss %-d -t %-d -i \"%-s\" "
           " -y -f mp4 -vcodec libx265  -aq 0 "
           " -c:a libmp3lame \"%-s/Frm%-5.5d\"",
           ssec,tslice,flname,folder,n);
       }
       else {
        sprintf(buff,"ffmpegfun -ss %-d  -i \"%-s\" "
           " -y -f mp4 -vcodec libx265  -aq 0 "
           " -c:a libmp3lame \"%-s/Frm%-5.5d\"",
           ssec,flname,folder,n);
       }
#endif
       runfunction(buff,ProcessToPipe,ffmpegfun);
       ssec += tslice;
       esec += tslice;
     }
#endif 
     close(Jpipe[1]);
     close(Jstat[0]);
     exit(0);
  }
  else {
     close(Jpipe[1]);
     close(Jstat[0]);
     RunMonitorJoin(NULL);
     kill(pid,9);
     waitpid(pid,&status,0);
     exit(0);
  }
}
