#include <kulina.h>
#include "ConvertData.h"
#include "mediainfo.h"
extern CONVDATA cndata;
extern int AConGrp,RangeGrp,EnVoGrp,VConGrp,
    VaspGrp,VsizeGrp,VrangeGrp,VJGrp,AJGrp,AddGrp,MixGrp,AmixGrp,KarGrp;
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
int ffmpegfun(int,char **);
int Mplayer(int,char **);
int Mencoder(int,char **);
void *RunMonitorJoin(void *arg);
int WriteInfo(char *);
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int ProcessPrint(int pip0,int pip1,int Pid);
int ProcessSkip(int pip0,int pip1,int Pid);
int ProcessToPipe(int pip0,int pip1,int Pid);

char * MakeKaraokeFile(void) {
  char buff[500],*pt;
  int id=0,ln;
  sprintf(buff,"%-s/Music/",getenv("HOME"));
  ln = strlen(buff);
  pt = buff+ln;
  while(1) {
    sprintf(pt,"Karaoke_%-4.4d.mp3",id);
//    printf("%s\n",buff);
    if (!FileStat(buff)) break;
    id++;
  }
  ln = strlen(buff);
  pt = (char *)malloc(ln+1);
  strcpy(pt,buff);
  return pt;
}
int ConvertToKaraoke( CONVDATA *cn) {
  int pid,status,id,Qty;
  char Folder[500];
  Dlink *L;
  CONVDATA Cn;
  MEDIAINFO *mpt;
  Cn= *cn;
#if 1
  if ((pid=fork())==0) {
    char command[10000],File[500],options[5000],Fifo[500],Qstr[100];
    double Asec,Msec;
    if(pipe(Jpipe) < 0) exit(0);
    if(pipe(Jstat) < 0) exit(0);
    sprintf(GrabFileName,"Making karaoke to : %-s\n",Cn.outfile);
    CheckMedia(Cn.audiofile);
    Esec = Minfo.TotSec;
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
        strcpy(Qstr," -aq 0");
        break;
      case 2:
        strcpy(Qstr," -aq 2");
        break;
      default:
      case 3:
        strcpy(Qstr," -aq 4");
        break;
    }
    sprintf(options,"!c01"
       "Converting to karaoke may take time!! Pl. be patient...\n");
    write(Jpipe[1],options,strlen(options));
    sprintf(options,"Esec: %lf\n",Esec);
    write(Jpipe[1],options,strlen(options));
    if(kgSearchString (Cn.outfile,(char *)".mp3")>=0 ) {
      sprintf(command,"ffmpegfun -vn -i \"%-s\" "
        "-af \"pan=stereo|c0=c0+-1*c1|c1=c1+-1*c0\" -y  -c:a libmp3lame -ac 2 "
        "  %-s -t %lf \"%-s\" ", Cn.audiofile ,Qstr,Esec,Cn.outfile);
    }
    else {
      if(kgSearchString (Cn.outfile,(char *)".aac")>=0 ) {
       sprintf(command,"ffmpegfun -vn -i \"%-s\" "
        " -af \"pan=stereo|c0=c0+-1*c1|c1=c0+-1*c1\"-y  -ac 2 -c:a libfdk_aac "
        " -t %lf \"%-s\" ", Cn.audiofile  ,Esec,Cn.outfile);
      }
      else {
      sprintf(command,"ffmpegfun -vn -i \"%-s\" "
        "-af \"pan=stereo|c0=c0+-1*c1|c1=c0+-1*c1\" -y  -ac 2 "
        "  %-s -t %lf \"%-s\" ", Cn.audiofile ,Qstr,Esec,Cn.outfile);
      }
    }
//    printf("%s\n",command);
    runfunction(command,ProcessToPipe,ffmpegfun);
    close(Jpipe[1]);
    exit(0);
  }
  else {
     waitpid(pid,&status,0);
  }
#endif
}

int  MakeKaraoketextbox1callback(int cellno,int i,void *Tmp) {
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
int  MakeKaraokebutton1callback(int butno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,"KaraokeTextBox1");
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
void  MakeKaraokebutton1init(DIN *B,void *pt) {
}
int  MakeKaraoketextbox2callback(int cellno,int i,void *Tmp) {
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
int  MakeKaraokebrowser1callback(int item,int i,void *Tmp) {
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
void  MakeKaraokebrowser1init(DIRA *R,void *pt) {
}
int  MakeKaraokesplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 

  DIALOG *D;DIL *B; 
  char buff[2000];
  int n,ret=1,Qty; 
  D = (DIALOG *)Tmp;
  DIT *T;
  DIRA *R;
  B = (DIL *) kgGetWidget(Tmp,i);
  T = (DIT *)kgGetNamedWidget(Tmp,"KaraokeTextBox1");
  strcpy(cndata.audiofile,kgGetString(T,0));
//  printf("%s\n",cndata.audiofile);
  n=0;
  while(cndata.audiofile[n]==' ') n++;
  if(cndata.audiofile[n]< ' ') return 0;
  T = (DIT *)kgGetNamedWidget(Tmp,"KaraokeTextBox2");
  strcpy(cndata.outfile,kgGetString(T,0));
  n=0;
  while(cndata.outfile[n]==' ') n++;
  if(cndata.outfile[n]< ' ') return 0;
  R = (DIRA *) kgGetNamedWidget(Tmp,"KaraokeRadio1");
  Qty = kgGetSelection(R);
  printf("Tools= %d\n",Tools);
  cndata.code =8;
  cndata.Eol='\n';
  strcpy(cndata.infile,(char *)"null");
  sprintf(buff,"%d \"%-s\" \"%-s\" \"%-s\" %d \n",
       cndata.code, cndata.audiofile,cndata.infile,cndata.outfile,Qty);
  write(ToTools[1],buff,strlen(buff));
  kgSplashMessage(Tmp,100,100,300,40,"Send for Processing",1,0,15);
  ret =0;
  return ret;
}
void  MakeKaraokesplbutton1init(DIL *B,void *pt) {
}
int MakeKaraokeinit(void *Tmp) {
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
int MakeKaraokecleanup(void *Tmp) {
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
int MakeKaraokeCallBack(void *Tmp,void *tmp) {
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
int MakeKaraokeResizeCallBack(void *Tmp) {
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
int MakeKaraokeWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
