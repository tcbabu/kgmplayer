#include <stdio.h>
#include <kulina.h>
#include <signal.h>
#include <fcntl.h>

int RunMonitorJoin(void *arg);

int MonitorJob(void *arg,int Pipe,int Pid);
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int kgffmpeg(int,char **);
int ProcessSkip(int pip0,int pip1,int Pid);
int ProcessPrint(int pip0,int pip1,int Pid);
int ProcessToPipe(int pip0,int pip1,int Pid);
int Mencoder(int,char **);
void *RunkgGetFiles(void *arg,char *Filter);
int updateoutfile(char *outfile);
int ffmpegfun(int,char **);
int GetBaseIndex(char *s);
int GetLine(int pip0,char *buff);
int SearchString(char *s1,char *s2);
int ProcessVolumeNew(void *stmp,int pip0,int Pid);


typedef struct _volstr {
	char Infile[200];
	char Outfile[200];
	int  MeanDb;
	double corval;
	double meanVol;
	double histVol;
	double maxVol;
	double duration;
} VOLSTR;

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
int RunNormalisebkgr_o(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **),char *infile,char *outfile) {
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
     KillJob(pid);
     sprintf(buff,"cp %s %s",outfile,infile);
     system(buff);
     remove(outfile);
     exit(0);
  }
}

int RunNormalisebkgr(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **),char *infile,char *outfile) {
  int pid=0,id;
  int status;
  char buff[500];

  
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
     MonitorJob(NULL,Jpipe[0],pid);
//     KillJob(pid);
     WAIT(pid);
        sprintf(buff,"cp %s %s",outfile,infile);
        system(buff);
        remove(outfile);
     return pid;
  }
}
char *AddMonMessage(Dlink *L,char *msg) {
	char *pt;
	pt = (char *)malloc(strlen(msg)+1);
	strcpy(pt,msg);
	Dadd(L,pt);
	return pt;
}

int RunVolJob(char *job,void *Msglines,int (*ProcessOut)(void *,int,int)){
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2];
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0,argc;
   char *Infile=NULL,*Outfile=NULL;
   Dlink *L = (Dlink *)Msglines;
   if(job==NULL) return 0;
   if(L != NULL) {
       Resetlink(L);
       Outfile= (char *)Getrecord(L);
       Infile= (char *)Getrecord(L);
       Resetlink(L);
   }
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
   while(job[i]==' ') i++;
   strcpy(buff,job+i);
   i=0;
   while ( sscanf(buff+pos,"%s",pt) > 0 ) {
     if(pt[0]=='\"') {
      pos++;
      args[i]=buff+pos;
      while(buff[pos]!='\"')pos++;
      buff[pos]='\0';
      i++;
     }
     else if(pt[0]=='\\') {
      pos++;
      args[i]=buff+pos;
      while(buff[pos]!='\\')pos++;
      buff[pos]='\0';
      i++;
     }
     else {
       args[i]=buff+pos;
       pos +=strlen(pt);
       i++;
       if(buff[pos]< ' ') break;
       buff[pos]='\0';
     }
     pos++;
     while(buff[pos]==' ') pos++;
   }
   args[i]=NULL;
   if(i==0) return 0;
   argc =i;
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
     dup(pip[1]);
     close(2);
     dup(pip[1]);
     close(pip[1]);
     ffmpegfun(argc,args);
     printf("END:\n");
     if((Outfile!=NULL)&&(Outfile[0]>=' ')){
	  sprintf(buff,"cp %s %s",Outfile,Infile);
          system(buff);
          remove(Outfile);
     }
     exit(0);
   }
   else {   /* parent process */
//     printf("Waiting pid: %d\n",pid);
     int ret =0;
     close(pip2[0]);
     close(pip[1]);
     if(ProcessOut != NULL){
	     ret=ProcessOut(Msglines,pip[0],pid);
//             KillJob(pid);
     }
//     WAIT(pid);
     waitpid(pid,&status,0);
     return pid;
   }
}
static int MakeOutFile(char Infile[],char Outfile[]) {
	int i=0,ln=strlen(Infile);
	char buff[200];
	i = ln-1;
	strcpy(Outfile,"/tmp/out");
	while((i>=0) &&(Infile[i] != '/')) i--;
	strcat(Outfile,Infile+i+1);
	return 1;
}
void *RunVolumeNormalise(void *stmp) {
	VOLSTR *Istr=(VOLSTR *)stmp;
	char Infile[200],Outfile[200],buff[500];
	double corval=0.0;
	int meanlevel;
	int pid;
	Dlink *L=Dopen();
	char *tpt;
	int hr,min,sec,tot;
	strcpy(Infile,Istr->Infile);
	strcpy(Outfile,Istr->Outfile);
	meanlevel = Istr->MeanDb;
	corval = Istr->corval;
	tot = (int)Istr->duration;
	hr = tot/3600;
	min = tot %3600;
	sec = min%60;
	min = min/60;

	AddMonMessage(L,Outfile);
	AddMonMessage(L,Infile);
#if 1
	sprintf(buff,"Meandb: %d Corval: %lf",meanlevel,corval);
	AddMonMessage(L,buff);
	sprintf(buff,"meanVol: %lf histVol-%lf maxVol=%lf",Istr->meanVol,
			Istr->histVol,Istr->maxVol);
	AddMonMessage(L,buff);
	sprintf(buff,"Processing file: %s",Infile);
	AddMonMessage(L,buff);
	sprintf(buff,"Duration: %2.2d:%2.2d:%2.2d ",hr,min,sec);
	AddMonMessage(L,buff);
	strcpy(buff,"!c08 Press !c03Cancel!c08 to kill");
	AddMonMessage(L,buff);
        sprintf(buff,"ffmpegfun -i \"%s\" -af \"volume=%lfdB\" -y \"%s\"", 
                      Infile,corval,Outfile);
//        RunNormalisebkgr(buff,ProcessToPipe,ffmpegfun,Infile,Outfile);
        Resetlink(L);
        pid = RunVolJob(buff,L,MonitorJob);
//        pid = RunVolJob(buff,NULL,NULL);
	free(stmp);
#if 0
        sprintf(buff,"cp %s %s",Outfile,Infile);
        system(buff);
        remove(Outfile);
#endif
	Dempty(L);
#endif
	return NULL;
}
void *RunVolumeDetect(void *stmp) {
	VOLSTR *Istr=(VOLSTR *)stmp;
	char Infile[200],Outfile[200],buff[500];
	double corval=0.0;
	int meanlevel;
	int pid;
	Dlink *L=Dopen();
	char *tpt;
	FILE *fp;
	fp = fopen("/home/kulina/Junk","w");
	strcpy(Infile,Istr->Infile);
	strcpy(Outfile,Istr->Outfile);
	meanlevel = Istr->MeanDb;
	corval = Istr->corval;

	AddMonMessage(L,Infile);
#if 1
	sprintf(buff,"Processing file: %s",Infile);
	AddMonMessage(L,buff);
	strcpy(buff,"!c08 Press !c03Cancel!c08 to kill");
	AddMonMessage(L,buff);
        sprintf(buff,"ffmpegfun -i \"%s\" -af \"volumedetect\" -vn -sn "
            " -dn -f null /dev/null", Infile);
	fprintf(fp,"%s\n",buff);
	fclose(fp);
	AddMonMessage(L,buff);
        Resetlink(L);
        pid = RunVolJob(buff,L,ProcessVolumeNew);
//	free(stmp);
#if 0
        sprintf(buff,"cp %s %s",Outfile,Infile);
        system(buff);
        remove(Outfile);
#endif
	Dempty(L);
#endif
	return NULL;
}
