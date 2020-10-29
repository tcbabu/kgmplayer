#include <kulina.h>
#include "ConvertData.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mediainfo.h"
extern CONVDATA cndata;
extern int AConGrp,RangeGrp,EnVoGrp,VConGrp,
    VaspGrp,VsizeGrp,VrangeGrp,VJGrp,AJGrp,AddGrp;
extern int ToTools[2],FromTools[2],StatusTools[2];
extern int StatusGrab[2];
extern int MonPipe;
extern double Ssec,Esec;
extern MEDIAINFO Minfo;
extern int Tools;
extern int Jpipe[2];
extern int Jstat[2];

int FileStat(char *flname);
int CheckVideo(char *flname);
int CheckMedia(char *flname);
int FileStat(char *);
extern char GrabFileName[300];
int kgLame(int,char **);
int kgffmpeg(int,char **);
int Mplayer(int,char **);
int Mencoder(int,char **);
void *RunMonitorJoin(void *arg);
int WriteInfo(char *);
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int ProcessPrint(int pip0,int pip1,int Pid);
int ProcessSkip(int pip0,int pip1,int Pid);
int ProcessToPipe(int pip0,int pip1,int Pid);

char * MakeAddFile(void) {
  char buff[500],*pt;
  int id=0,ln;
  sprintf(buff,"%-s/Video/",getenv("HOME"));
  ln = strlen(buff);
  pt = buff+ln;
  while(1) {
    sprintf(pt,"NewVideo_%-4.4d.mp4",id);
//    printf("%s\n",buff);
    if (!FileStat(buff)) break;
    id++;
  }
  ln = strlen(buff);
  pt = (char *)malloc(ln+1);
  strcpy(pt,buff);
  return pt;
}

int AddAudioToVideo( CONVDATA *cn) {
  int pid,status,id,Qty;
  char Folder[500];
  Dlink *L;
  CONVDATA Cn;
  MEDIAINFO *mpt;
  Cn= *cn;
  if ((pid=fork())==0) {
    char command[10000],File[500],options[5000],Fifo[500],Qstr[100];
    sprintf(Folder,"%-s/%-d",getenv("HOME"),getpid());
    if(FileStat(Folder)) kgCleanDir(Folder);
    mkdir(Folder,0700);
    if(pipe(Jpipe) < 0) exit(0);
    if(pipe(Jstat) < 0) exit(0);
    sprintf(GrabFileName,"Adding audio to : %-s\n",Cn.outfile);
    CheckMedia(Cn.audiofile);
    Esec = Minfo.TotSec;
    CheckMedia(Cn.infile);
    if(Minfo.TotSec < Esec) Esec = Minfo.TotSec;
    MonPipe = Jpipe[0];
    if( fork()==0) {
      close(Jpipe[1]);
      close(Jstat[0]);
      RunMonitorJoin(NULL);
      exit(0);
    }
    close(Jpipe[0]);
    close(Jstat[1]);
    switch(Cn.Quality) {
      case 1:
        strcpy(Qstr,"3000K");
        break;
      case 2:
        strcpy(Qstr,"1000K");
        break;
      default:
      case 3:
        strcpy(Qstr,"500K");
        break;
    }
    sprintf(options,"!c01Adding/Changing audio "
       "and converting may take time!! Pl. be patient...\n");
    write(Jpipe[1],options,strlen(options));
    sprintf(options,"Processing audio  file\n");
    write(Jpipe[1],options,strlen(options));
    id =0;
    sprintf(command,"kgffmpeg -vn -i \"%s\" "
         " -vn -aq 2 -ac 2 -ar 44100 -acodec pcm_s32le -t %lf -y %s/F%-4.4d.wav ",
         Cn.audiofile,Esec,Folder,id);
//        printf("%s\n",command);
    runfunction(command,ProcessSkip,kgffmpeg);
    sprintf(options,"Esec: %lf\n",Esec);
    write(Jpipe[1],options,strlen(options));
    sprintf(options,"Adding audio and Converting to mp4\n");
    write(Jpipe[1],options,strlen(options));
    if(Cn.Quality==1) {
    sprintf(command,"kgffmpeg -vn -i %-s/F%-4.4d.wav -an -i \"%-s\" "
        " -y -f mp4 -vcodec libx265 "
        "  -t %lf \"%-s\" ", Folder,id ,Cn.infile,Esec,Cn.outfile);
    }
    else {
    sprintf(command,"kgffmpeg -vn -i %-s/F%-4.4d.wav -an -i \"%-s\" "
        " -y -f mp4 -vcodec libx264 "
        " -b:v %-s -t %lf \"%-s\" ", Folder,id ,Cn.infile,Qstr,Esec,Cn.outfile);
    }
//        printf("%s\n",command);
    runfunction(command,ProcessToPipe,kgffmpeg);
    kgCleanDir(Folder);
    close(Jpipe[1]);
    exit(0);
  }
  else {
     waitpid(pid,&status,0);
     sprintf(Folder,"%-s/%-d",getenv("HOME"),pid);
     if(FileStat(Folder)) kgCleanDir(Folder);
  }
}

int  AddAudiotextbox1callback(int cellno,int i,void *Tmp) {
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
int  AddAudiobutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  char FileName[500];
  int n,ret =0; 
  DIT *T;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  T = (DIT *)kgGetNamedWidget(Tmp,"AddTbox1");
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
  kgFolderBrowser(NULL,100,100,FileName,"*");
  CheckMedia(FileName);
  if(Minfo.Audio !=0 ) {
    kgSetString(T,0,FileName);
    kgUpdateWidget(T);
    kgUpdateOn(Tmp);
  }


  return ret;
}
void  AddAudiobutton1init(DIN *B,void *pt) {
}
int  AddAudiotextbox2callback(int cellno,int i,void *Tmp) {
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
int  AddAudiobrowser1callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIRA *R;DIALOG *D;void *pt; 
  ThumbNail **th; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  R = (DIRA *)kgGetWidget(Tmp,i);
  th = (ThumbNail **) R->list;
  return ret;
}
void  AddAudiobrowser1init(DIRA *R,void *pt) {
}
int  AddAudiosplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  char buff[2000];
  DIALOG *D;DIL *B; 
  int n,ret=1,Qty; 
  D = (DIALOG *)Tmp;
  DIT *T;
  DIRA *R;
  B = (DIL *) kgGetWidget(Tmp,i);
  T = (DIT *)kgGetNamedWidget(Tmp,"AddTbox1");
  strcpy(cndata.audiofile,kgGetString(T,0));
//  printf("%s\n",cndata.audiofile);
  n=0;
  while(cndata.audiofile[n]==' ') n++;
  if(cndata.audiofile[n]< ' ') return 0;
  T = (DIT *)kgGetNamedWidget(Tmp,"AddTbox3");
  strcpy(cndata.infile,kgGetString(T,0));
  n=0;
  while(cndata.infile[n]==' ') n++;
  if(cndata.infile[n]< ' ') return 0;
  T = (DIT *)kgGetNamedWidget(Tmp,"AddTbox2");
  strcpy(cndata.outfile,kgGetString(T,0));
  n=0;
  while(cndata.outfile[n]==' ') n++;
  if(cndata.outfile[n]< ' ') return 0;
  R = (DIRA *) kgGetNamedWidget(Tmp,"AddQty");
  Qty = kgGetSelection(R);
  cndata.code =5;
  cndata.Eol='\n';
  sprintf(buff,"%d \"%-s\" \"%-s\" \"%-s\" %d \n",
       cndata.code, cndata.audiofile,cndata.infile,cndata.outfile,Qty);
  write(ToTools[1],buff,strlen(buff));
  
  kgSplashMessage(NULL,100,100,300,40,"Send for Processing",1,0,15);
  ret =0;

  return ret;
}
void  AddAudiosplbutton1init(DIL *B,void *pt) {
}
int  AddAudiotextbox3callback(int cellno,int i,void *Tmp) {
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
int  AddAudiobutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  char FileName[500];
  D = (DIALOG *)Tmp;
  DIT *T;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  T = (DIT *)kgGetNamedWidget(Tmp,"AddTbox3");
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
  kgFolderBrowser(NULL,100,100,FileName,"*");
  CheckMedia(FileName);
  if(Minfo.Video !=0 ) {
    kgSetString(T,0,FileName);
    kgUpdateWidget(T);
    kgUpdateOn(Tmp);
  }
  return ret;
}
void  AddAudiobutton2init(DIN *B,void *pt) {
}
int AddAudioinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  DIT *T;
  pt = D->pt;
  T = (DIT *)kgGetNamedWidget(Tmp,"AddTbox2");
  return ret;
}
int AddAudiocleanup(void *Tmp) {
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
int AddAudioCallBack(void *Tmp,void *tmp) {
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
int AddAudioResizeCallBack(void *Tmp) {
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
int AddAudioWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
