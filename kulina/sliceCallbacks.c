#include <kulina.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "mediainfo.h"
#include "kgutils.h"


int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int kgffmpeg(int,char **);
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


static char infile[500]="",outfile[500]="";

int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int MakeOutputFile(char *Infile,char *Outfile,char *ext);
int GetBaseIndex(char *s);
int GetLine(int pip0,char *buff);
int SearchString(char *s1,char *s2);
int runjob(char *job,int (*ProcessOut)(int,int,int));
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int runfunctionbkgr(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int FileStat(char *flname);
int kgffmpeg(int,char **);
int ProcessSkip(int pip0,int pip1,int Pid);
int ProcessToPipe(int pip0,int pip1,int Pid);

static void *InfoBox=NULL,*Dia=NULL;


int MakeFolder(char *Infile,char *Folder,char *Outfile) {
   int index,i;
   char buff[500],*pt;
   int id=0;
   index = GetBaseIndex(Infile);
   sprintf(buff,"%-s/",Folder);
   strcat(buff,Infile+index);
   i=0;
   while (buff[i]!='.') {
     if(buff[i]< ' ') break;
     if(buff[i]==' ') buff[i]='_';
     if(i>30) break;
     i++;
   }
#if 1
   pt= buff+i;
   while(1) {
     sprintf(pt,"_%-4.4d",id);
//    printf("%s\n",buff);
     if (!FileStat(buff)) break;
     id++;
   }
#else
   buff[i]='.';
   i++;
   buff[i]='\0';
   strcat(buff,ext);
#endif
   strcpy(Outfile,buff);
   return 1;
}

int MakeVideoSlices(char * flname,char *folder,int tslice) {

  int pid=0,id;
  int status;
  char buff[5000];
  int totsec=0;
  int ssec,esec;
  int n;
 

  if( CheckMedia(flname) == 0 ) {
    kgSplashMessage(NULL,100,100,300,40,(char *)"Error: Not Video",1,0,15);
    return 0;
  }
  if( (pid =fork())!= 0) {
    kgSplashMessage(NULL,100,100,300,40,(char *)"Send for Processing",1,0,15);
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
     sprintf(buff,"Esec: %d\n",tslice);
     write(Jpipe[1],buff,strlen(buff));
     ssec=0;
     esec = tslice;
     n=1;
     while (ssec < totsec) {
       sprintf(buff,"  %-s/Frm%-5.5d\n",folder,n);
       write(Jpipe[1],buff,strlen(buff));
#ifdef D_X264
       if(esec < totsec) {
        sprintf(buff,"kgffmpeg -ss %-d -t %-d -i \"%-s\" "
           " -y -f mp4 -vcodec libx264  -b:v 3000K -aq 0 "
           " -c:a libmp3lame \"%-s/Frm%-5.5d\"",
           ssec,tslice,flname,folder,n);
       }
       else {
        sprintf(buff,"kgffmpeg -ss %-d  -i \"%-s\" "
           " -y -f mp4 -vcodec libx264  -b:v 3000K -aq 0 "
           " -c:a libmp3lame \"%-s/Frm%-5.5d\"",
           ssec,flname,folder,n);
       }
#else
       if(esec < totsec) {
        sprintf(buff,"kgffmpeg -ss %-d -t %-d -i \"%-s\" "
           " -y -f mp4 -vcodec libx265 -aq 0 "
           " -c:a libmp3lame \"%-s/Frm%-5.5d\"",
           ssec,tslice,flname,folder,n);
       }
       else {
        sprintf(buff,"kgffmpeg -ss %-d  -i \"%-s\" "
           " -y -f mp4 -vcodec libx265 -aq 0 "
           " -c:a libmp3lame \"%-s/Frm%-5.5d\"",
           ssec,flname,folder,n);
       }
#endif
       n++;
       runfunction(buff,ProcessToPipe,kgffmpeg);
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
int  slicetextbox1callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  DIT *TO;
  int ret=1;
  static char FileName[500],OutFile[500];
  D = (DIALOG *)Tmp;
  Dia = D;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"sliceInput");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"sliceOutput");
  strcpy(FileName,kgGetString(T,0));
  sprintf(OutFile,"%-s/Video",getenv("HOME"));
  MakeFolder(FileName,OutFile,OutFile);
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  strcpy(infile,FileName);
  strcpy(outfile,OutFile);
  return ret;
}
int  slicetextbox2callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e;
  DIT *TO;
  char FileName[500],OutFile[500];
  int ret=1;
  D = (DIALOG *)Tmp;
  Dia = D;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"sliceInput");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"sliceOutput");
  strcpy(OutFile,kgGetString(TO,0));
  strcpy(outfile,OutFile);

  return ret;
}
int  slicebutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B;
  int n,ret =0;
  static char FileName[500],OutFile[500];
  D = (DIALOG *)Tmp;
  Dia = D;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  DIT *T,*TO;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"sliceInput");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"sliceOutput");
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
  kgFolderBrowser(NULL,100,100,FileName,(char *)"*");
  kgSetString(T,0,FileName);
  sprintf(OutFile,"%-s/Video",getenv("HOME"));
  MakeFolder(FileName,OutFile,OutFile);
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  strcpy(infile,FileName);
  strcpy(outfile,OutFile);
  
  return ret;
}
void  slicebutton1init(DIN *B,void *pt) {
}
int  slicesplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B;
  int n,ret=1;
  char buff[1000];
  int tslice=1;
  D = (DIALOG *)Tmp;
  Dia = D;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  InfoBox = kgGetNamedWidget(Tmp,(char *)"sliceInfoBox");
  switch(butno) {
    case 1:
       if((infile==NULL) || (infile[0]=='\0')) {
        kgWrite(InfoBox,(char *)"No Input file selected\n");
        kgUpdateOn(D);
      }
      else {
        kgWrite(InfoBox,(char *)"send for processing...\n");
        kgUpdateOn(D);
        strcpy(infile,kgGetString(kgGetNamedWidget(Dia,(char *)"sliceInput"),0));
        strcpy(outfile,kgGetString(kgGetNamedWidget(Dia,(char *)"sliceOutput"),0));
        mkdir(outfile,0744);
        tslice = kgGetInt(kgGetNamedWidget(Dia,(char *)"sliceTime"),0);
        MakeVideoSlices(infile,outfile,tslice);
      }
      ret = 0;
      break;
  }
  return ret;
}
void  slicesplbutton1init(DIL *B,void *pt) {
}
int  slicetextbox3callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
  return ret;
}
int sliceinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int slicecleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int sliceCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
int sliceResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  kgGetWindowSize(D,&xres,&yres);
  dx = xres - D->xl;
  dy = yres - D->yl;
  /* extra code */
  D->xl= xres;
  D->yl= yres;
  kgRedrawDialog(D);
  return ret;
}
int sliceWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}