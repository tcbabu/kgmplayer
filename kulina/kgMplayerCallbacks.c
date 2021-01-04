#define D_PULSE
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <cdio/cdio.h>
#include "kulina.h"
#include "mediainfo.h"
#define _GNU_SOURCE
typedef struct _DevRec {
  char device[50];
  int hdmi;
  int Xres,Yres;
  int pos;
  Dlink *Resolist;
} DEVREC;
MEDIAINFO Minfo;
extern int DLOOP,DRANDOM;
extern char DirName[500];
extern float AudioTiming;
extern int AudioDelay;
extern int Kuclr;
extern char UrlOld[500];
extern int GRABING;
extern int UrlGrp;
extern int SubGrp;
extern int DirGrp;
extern int HAGrp;
extern int StoffGrp;
Dlink *Devlist=NULL;
int InAttn=1;
int NoVideo=0;
int WMEntry=0;
int Chapters=1;
int AllTracks=0;
int first,last;
int ResetSubTitle(MEDIAINFO *Minfo);
int DirPlayinit(void *Tmp);
int subtitleinit(void *Tmp);
int imgs2vinit(void *Tmp);
int cd_info(int,char **);
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int RunFunction(char *job,int (*ProcessOut)(void *,int,int,int),int
(*function)(int,char **),void *);
void * RunControls(void *);
int runjob(char *job,int (*ProcessOut)(int,int,int));
extern int BRK;
extern Dlink *Plink;
extern int WMErr;
int PULSE=0;
int LOOP=0;
int RANDOM=1;
int Three2Two=0;
int Vstep=10;
int AudioDelay=0;
unsigned long MWIN=0,RWIN=0;
int FULLSCREEN=1;
char *envrn[2];
char CurPlayList[200];
void *RunRip(void *arg);
void *RunFullScreen(void *arg);
void *RunAControls(void *arg) ;
int TryGrabing(void);
int CheckMedia(char *flname);
int CheckMedia_d(char *flname);

int FilePlay=1,Dvd=0,Vcd=0,HDMI=0,VIDEO=0,Audio=0,CDROM=0;
int HDMIAUDIO=1;
int Vxres=740,Vyres=520;
int SoftVol=0;
int Card=0,Device=0;
double vRate,vStart,vLen;
double aRate,aStart,aLen;
double DmuxTime=0.0;
char FileName[500];
float Rperc=0.0;
void *Info=NULL,*Dia=NULL,*Sld,*Hsld,*Wsld,*Vsld,*Brw,*B13,*B16,*B17,*C19,*R20;
char Line[500];
float StartSec=0.,TotSec=0.,CurSec=0.,ETot = 9000,OCurSec;
float CurPerc=0.;
extern char **Hlist;
extern char PrimDev[50],HdmiDev[50];
extern int PReso,HReso,PVis,HVis,Hpos;
int Cxres,Cyres,Dxres,Dyres,Pxres=-1,Pyres,Hxres=-1,Hyres;
static int OldV=0,CurV=0,Xres=1366,Yres=768,H,W,Xo=0,Yo=0;
float Xs=0,Ys=0;
static char dummy[100];
int Volume=100,Pid,pipew,Playing=0,Track=-2;
int SoftVolMax=1000;
int sig[2],rsig[2],rsig1[2],Pip0;
void KillDisplay(void);
int Speed=1,Mute=0,Play=1;
static char BS=0x08,Esc=27,UpAr[4]={27,'[','A','\0'},
      DownAr[4]={27,'[','B','\0'},
      RightAr[4]={27,'[','C','\0'},LeftAr[4]={27,'[','D','\0'},
      PgUp[4]={27,'[','5','~'},PgDown[4]={27,'[','6','~'},Slash[2]="/",
      Star[2]="*";
char hdmistring[100];
char sfvstring[100];
pthread_mutex_t  Mlock;
void *RunManage(void *arg);
void *RunPlayList(void *arg);
int CurrDevInfo(void);
int Mplayer(int argc,char *argv[]);
void WriteMessage(char *buff);
char *GetUrlCopy (void);


int eject_drive(const char * drive) {
    int err;
    err = cdio_eject_media_drive(drive);
    if(err) {
      fprintf(stderr,"Failed to eject\n");
      return 0;
    }
    return 1;
}
int close_drive(const char * drive) {
    int err;
    err = cdio_close_tray(drive,NULL);
    if(err) {
      fprintf(stderr,"Failed to close\n");
      return 0;
    }
    return 1;
}
int get_disc_info(const char *drive) {
    int ret=0;
    CdIo_t *p_cdio;
    p_cdio = cdio_open (drive, DRIVER_DEVICE);
    if (NULL == p_cdio) {
        fprintf(stderr,"Couldn't find CD\n");
        return ret;
    } else {
    discmode_t cd_discmode = cdio_get_discmode (p_cdio);
    
//    printf("-- Discmode: %d  %s\n\n",cd_discmode, discmode2str[cd_discmode]);
    
    cdio_destroy(p_cdio);
    switch(cd_discmode) {
       case 15:
       case 14:
       case 13:
       case 12:
       case 11:
       case 10:
       case 9:
       case 8:
       case 7:
       case 6:
       case 5:
       case 4:
         ret=2;
         break;
       case 2:
       case 1:
       case 3:
         ret=1;
         break;
       default:
         ret=0;
         break;
    }
    
    return ret;
    }

}
int get_disc_tracks(const char *drive,int *first,int *last) {
    int ret=0;
    CdIo_t *p_cdio;
    p_cdio = cdio_open (drive, DRIVER_DEVICE);
    if (NULL == p_cdio) {
        fprintf(stderr,"Couldn't find CD\n");
        return ret;
    } else {
    track_t i_first_track = cdio_get_first_track_num (p_cdio);
    track_t i_tracks = cdio_get_num_tracks (p_cdio);
    track_t i_last_track = i_first_track + i_tracks;
    *first = i_first_track;
    *last = i_last_track;
    
//    printf("-- tracks: %d to  %d\n",*first,*last);
    
    cdio_destroy(p_cdio);
    ret=1;
    return ret;
    }

}

int WaitPipes(int pipe1,int pipe2,int usecs) {
   char buf[2];
    int retval,pipe;
    struct timeval tv1;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(pipe1, &rfds);
    FD_SET(pipe2, &rfds);
    pipe = pipe1+1;
    if(pipe2 > pipe1 ) pipe=pipe2+1;
    tv1.tv_sec = 0;
    tv1.tv_usec = usecs;
    retval = select(pipe+1, &rfds, NULL, NULL, &tv1);
    if(retval>=1) {
      if(FD_ISSET(pipe2,&rfds)) {retval=2; read(pipe2,buf,1);}
      if(FD_ISSET(pipe1,&rfds)) {retval=1; read(pipe1,buf,1);}
    }
    else retval=0;
    return retval;
}
int getrandom(int max){
  static int entry=0;
  double v;
  int r;
  if(!entry) {
    entry=1;
    srand48(time(NULL));
  }
  if(max<=1) return 1;
  v = drand48();
//  return  (int)(v*(max-1)+1.5);
  r = (int)(v*(max)+1);
  if(r > max ) r=max;
  return r;
}
int GetBaseIndex(char *s) {
  int i=0,n=0,ret=0;
  if(s!=NULL) {
    n = strlen(s);
    i=n-1;
    while(i>=0) {
      if(s[i]=='/') break;
      i--;
    }
    ret=i+1;
  }
  return ret;
}
static int CheckString(char *s1,char *s2) {
   int ch,ln,ret=0;
   ln = strlen(s2);
   if(strlen(s1)<ln) ret=0;
   else {
     ch = s1[ln];
     s1[ln]='\0';
     if( strcmp(s1,s2)==0 ) ret=1;
     s1[ln]=ch;
   }
   return ret;
}
int SearchString(char *s1,char *s2) {
   int ch,ln,ret=-1,ln1,i=0;
   ln = strlen(s2);
   ln1 = strlen(s1);
   if(ln1<ln) ret=-1;
   else {
    i=0;
    for(i=0;i<=(ln1-ln);i++) {
      if(CheckString(s1+i,s2)){
        ret=i;
        break;
      }
    }
   }
   return ret;
}
  
int wordCount(char *buf) {
  int count=0,i;
  i=0;
  while(1) {
    while((buf[i]==' ') ||(buf[i]=='\t'))i++;
    if(buf[i]<' ') break;
    while((buf[i]>' ') )i++;
    count++;
  }
  return count;
}
int skiptoWord(char *buf){
  int i=0,ret=0;
  while((buf[i]==' ') ||(buf[i]=='\t'))i++;
  if(buf[i]>' ') ret=i;
  return ret;
}
int skipWord(char *buf){
  int i=0,ret=0;
  while((buf[i]>' '))i++;
  ret=i;
  return ret;
}

void  ResetHdmi_o(void) {
  int i=0,j,xr,yr;
  char buff[200];
  CurrDevInfo();
//  printf("%s %s\n",HdmiDev,Hlist[HReso]);
  if(Hlist==NULL) return;
  if(HVis==0 ) {
    HReso=0;
    while(Hlist[i]!= NULL) {
      strcpy(buff,Hlist[i]);
      j=0; 
      while(buff[j]>= ' ') {if(!isdigit(buff[j]) ) buff[j]=' '; j++;}
      sscanf(buff,"%d%d",&xr,&yr);
      if(xr<= Dxres) {i++;continue;}
      sprintf(buff,"xrandr --output %-s --mode %s",HdmiDev,Hlist[i]);
//      printf("1: %s\n",buff);
      runjob(buff,NULL);
      break;
    }
    sprintf(buff,"xrandr --output %-s --mode %s",HdmiDev,Hlist[HReso]);
    runjob(buff,NULL);
  }
  sprintf(buff,"xrandr --output %-s --off",HdmiDev);
  runjob(buff,NULL);
  sprintf(buff,"xrandr --output %-s --auto",HdmiDev);
  runjob(buff,NULL);
  sprintf(buff,"alsactl nrestore %d",Card);
  runjob(buff,NULL);
  return ;
}
void  ResetHdmi(void) {
  int i=0,j,xr,yr;
  char buff[200];
  CurrDevInfo();
//  printf("%s %s\n",HdmiDev,Hlist[HReso]);
  if(Hlist==NULL) return;
  if(HVis==0 ) {
    HReso=0;
      sprintf(buff,"xrandr --output %-s --mode \"720x480\"",HdmiDev,Hlist[i]);
      runjob(buff,NULL);
    sprintf(buff,"xrandr --output %-s --mode %s",HdmiDev,Hlist[HReso]);
    runjob(buff,NULL);
  }
  sprintf(buff,"alsactl nrestore %d",Card);
  runjob(buff,NULL);
  return ;
}
char **GetDrives(void) {
  char **drv=NULL;
  char buf[500],name[30];
  FILE *fp;
  int ndrv=0,i,j,ret;
  fp = fopen("/proc/sys/dev/cdrom/info","r");
  if(fp != NULL) {
    while(fgets(buf,499,fp)!= NULL) {
      if(CheckString(buf,(char *)"drive name:")) {
         ndrv=wordCount(buf)-2;
         if(ndrv>0) {
           drv = (char **)malloc(sizeof(char *)*(ndrv+1));
           drv[ndrv]=NULL;
           for(i=0;i<ndrv;i++) drv[i]=(char *)malloc(30);
           ret=skiptoWord(buf);
           i=ret;
           ret=skipWord(buf+i);
           i=i+ret;
           ret=skiptoWord(buf+i);
           i=i+ret;
           ret=skipWord(buf+i);
           i=i+ret;
           for(j=0;j<ndrv;j++) {
             ret=skiptoWord(buf+i);
             i=i+ret;
             sscanf(buf+i,"%s",name);
             strcpy(drv[j],"/dev/");
             strcat(drv[j],name);
             ret=skipWord(buf+i);
             i=i+ret;
           }
         }
      }
    }
    fclose(fp);
  }
  if(ndrv==0) {
    drv = (char **)malloc(sizeof(char *)*(2));
    drv[1]=NULL;
    drv[0]=(char *)malloc(30);
    strcpy(drv[0],(char *)"<NO CD DRIVE>");
  }
  return drv;
}
int GetLine(int pip0,char *buff){
     unsigned char ch;
     fd_set rfds;
     struct timeval tv;
     int retval,chnl,status,i;
     int ret,val;
     chnl = pip0;
     i=0;
     FD_ZERO(&rfds);
     FD_SET(pip0,&rfds);
     while(1) {
       tv.tv_sec = 30;
       tv.tv_usec =0;
       ret=0;
       retval = select(chnl+1,&rfds,NULL,NULL,&tv);
       if(retval < 0) {ret=0;break;}
       val=1;
       if((retval> 0)&&(FD_ISSET(chnl,&rfds))){
         if(read(chnl,&ch,1) != 1){
           ret=0;
           break;
         }
         buff[i++]=ch;
         if(i>490) i=490;
         if( (ch=='\n')||(ch=='\r')) {ret=ch;break;}
       }
       else {ret=-1;break;}
     }
     buff[i]='\0';
     return ret;
}
int ProcessCode(char* Code) {
   char code;
   int i,count;
   code = Code[0];
   switch(code) {
      case '+': //audio  +0.1 secs
       write(pipew,"+",1);
       break;
      case '-': //audio  =0.1 sec
       write(pipew,"-",1);
       break;
      case 'V':
       write(pipew,Star,1);
//         write(pipew,"0",1);
       break;
      case 'v':
       write(pipew,Slash,1);
//         write(pipew,"9",1);
       break;
      case 'q':
       write(pipew,"q",1);
       break;
      case 'T':
       write(pipew,PgUp,4);
       break;
      case 'M':
       write(pipew,UpAr,3);
       break;
      case 'S':
       write(pipew,RightAr,3);
       break;
      case 't':
       write(pipew,PgDown,4);
       break;
      case 'm':
       write(pipew,DownAr,3);
       break;
      case 's':
       write(pipew,LeftAr,3);
       break;
      defaut:
       break;
   }
}
void * SkipToStart(void *Tmp) {
  char dummy[2];
  int m,tm,ts,ESIG=0,offset,j;
  int ret = 1;
  BRK=0;
  if(StartSec> 0) {
     offset=(int)StartSec;
     m = offset/60;
     ts = (offset%60)/10;
     tm = m/10;
     m = m%10;
     if(kgThreadWaitPipe(sig[0],0,100)==1) { ESIG=1 ;}
     for(j=0;j<tm;j++) { 
           if(!ESIG) write(rsig[1],"\nT\n",3);
           else break;
           if(kgThreadWaitPipe(sig[0],0,100)==1) { ESIG=1 ;}
           if(!ESIG) read(rsig1[0],&dummy,1);
     }
     for(j=0;j<m;j++) {
           if(!ESIG) write(rsig[1],"\nM\n",3);
           else break;
           if(kgThreadWaitPipe(sig[0],0,100)==1) { ESIG=1 ;}
           if(!ESIG) read(rsig1[0],&dummy,1);
     }
     for(j=0;j<ts;j++) {
           if(!ESIG) write(rsig[1],"\nS\n",3);
           else break;
           if(kgThreadWaitPipe(sig[0],0,100)==1) { ESIG=1 ;}
           if(!ESIG) read(rsig1[0],&dummy,1);
    }
  }
  return NULL;
}
int ProcessMplayer(int pip0,int pip1,int Pid) {
     int Begin=0,Thread=0;
     static int Entry=0,SIG=0;
     pthread_t Pth,Pths;
     int Loc;
     void *res;
     
     char buff[1000],Starting[100],CODE[10];
     int ch,dum;
//     close(pip1);
     VIDEO=Audio=0;
     vRate=1.0;vStart=0.0,vLen=0.0;
     aRate=1.0;aStart=0.0,aLen=0.0;
     DmuxTime=0.0;
     pipew= pip1;
     Pip0 = pip0;
     if(Entry) {
       while(kgThreadWaitPipe(rsig1[0],0,200));
       while(kgThreadWaitPipe(rsig[0],0,200));
       while(kgThreadWaitPipe(sig[0],0,200)) ;
#if 1
       close(sig[0]);
       close(sig[1]);
       close(rsig[0]);
       close(rsig[1]);
       close(rsig1[0]);
       close(rsig1[1]);
       Entry=0;
#endif
     }
     if( pipe(sig) < 0) return 0;
     if( pipe(rsig) < 0) return 0;
     if( pipe(rsig1) < 0) return 0;
     Entry=1;
     OCurSec=-1.0;
     while((ch=GetLine(pip0,buff)) ) {
//         if(ch< 0) { printf("Got -\n"); }
//         printf("%s\n",buff);
         if(ch> 0) {
          if(buff[0]==' ') continue;
          if(ch=='\n') {
              if(Begin) continue;
              if(CheckString(buff,(char *)"==> Found video stream:")) {
                if(NoVideo==0) VIDEO=1;
              }
              if(CheckString(buff,(char *)"==> Found audio stream:")) {
                Audio=1;
              }
              if(CheckString(buff,(char *)"T:")) {
               sscanf(buff+2,"%f",&TotSec);
              }
#if 0
              if(CheckString(buff,(char *)"DEMUX_MPG_CTRL")) {
                   int k=0;
//                   printf("%s\n",buff);
#if 1
                   while(buff[k]!=')')k++;
                   buff[k]=' ';
                   k=0;
                   while(buff[k]!='(')k++;
                   k++;
                   sscanf(buff+k,"%lf",&DmuxTime);
//                   printf("Demux: %lf \n",DmuxTime);
#endif
                   continue;
              }
              if(CheckString(buff,(char *)"[aviheader] Video stream found")) {
               while((ch=GetLine(pip0,buff)) ) {
                 if(ch< 0) continue;
                 if(CheckString(buff,(char *)"Rate:")) {
                   int k=0;
                   while(buff[k]!='=')k++;
                   k++;
                   sscanf(buff+k,"%lf",&vRate);
//                   printf("Rate: %lf \n",vRate);
                   continue;
                 }
                 if(CheckString(buff,(char *)"Start:")) {
                   int k=5;
                   while(buff[k]!=':')k++;
                   k++;
                   sscanf(buff+k,"%lf",&vStart);
                   while(buff[k]!=':')k++;
                   k++;
                   sscanf(buff+k,"%lf",&vLen);
//                   printf("Start: %lf Len: %lf\n",vStart,vLen);
                   continue;
                 }
                 if(CheckString(buff,(char *)"============"))break;
               }
               continue;
              }
              if(CheckString(buff,(char *)"[aviheader] Audio stream found")) {
               while((ch=GetLine(pip0,buff)) ) {
                 if(ch< 0) continue;
                 if(CheckString(buff,(char *)"Rate:")) {
                   int k=0;
                   while(buff[k]!='=')k++;
                   k++;
                   sscanf(buff+k,"%lf",&aRate);
                   continue;
                 }
                 if(CheckString(buff,(char *)"Start:")) {
                   int k=5;
                   while(buff[k]!=':')k++;
                   k++;
                   sscanf(buff+k,"%lf",&aStart);
                   while(buff[k]!=':')k++;
                   k++;
                   sscanf(buff+k,"%lf",&aLen);
                   continue;
                 }
                 if(CheckString(buff,(char *)"============"))break;
               }
               continue;
             }
#endif
             if(CheckString(buff,(char *)"Starting")) {
             }
          }
          if(ch=='\r') {
             if(CheckString(buff,(char *)"W:")) {
//               printf("%s\n",buff);
               sscanf(buff+2,"%d",&MWIN);
               continue;
             }
             Begin++;
             if(CheckString(buff,(char *)"B:")) {
//               printf("Mouse Press\n");
              if(FULLSCREEN) {
#if 1
               if((Thread==0)&& CheckString(buff,(char *)"B: 1")){ // esc press
                ProcessCode("q"); // stop playing
               }
               else { // mouse press
#else
               {
#endif
                 pthread_mutex_lock(&Mlock);
                 if(Thread==0) {
                   if(InAttn==0) WMErr=1;
                   pthread_create(&(Pth),NULL, RunControls,NULL);
                   Thread=1;
#if 1
                   if((!WMEntry)&&(WMErr==0)) {
                     read(rsig[0],CODE,6);
                     if(CODE[0]!='U') {
                       WMEntry=1;
//                     printf("%s",CODE);
                       if((CODE[0]=='E')) {
                         write(sig[1],"\n",1);
                         if(Thread) pthread_join(Pth,&res);
                         WMErr=1;
                         pthread_create(&(Pth),NULL, RunControls,NULL);
                       }
                     }
                   }
#endif
                 }
                 else {
                   write(sig[1],"\n",1);
                   if(Thread) pthread_join(Pth,&res);
                   Thread=0;
                 }
                 pthread_mutex_unlock(&Mlock);
                 if(CheckString(buff,(char *)"B: 1")){ // esc press
                   ProcessCode((char *)"q"); // stop playing
                 }
               }
              }
//               continue;
             }
             if(Begin==1) {
//              printf("%s\n",buff);
             if(CheckString(buff,(char *)"DEMUX_MPG_CTRL")) {
               int k=0;
               Begin=0;
                   while(buff[k]!=')')k++;
                   buff[k]=' ';
                   k=0;
                   while(buff[k]!='(')k++;
                   k++;
                   sscanf(buff+k,"%lf",&DmuxTime);
//                   printf("Demux: %lf \n",DmuxTime);
               continue;
             }
             if(!CheckString(buff,(char *)"A:")) {
               Begin=0;
               continue;
             }
//              RunFullScreen(NULL);pthread_mutex_lock(&
//              printf("Video=%d\n",VIDEO);
//              printf("Audio=%d\n",Audio);
              if( DmuxTime != 0.0) TotSec=DmuxTime;
              else {
                TotSec = (vLen-vStart)/vRate;
//              printf("TotSec=%f:%lf %lf %lf\n",TotSec,vStart,vLen,vRate);
                if(TotSec==0.0) TotSec=(aLen-aStart)/aRate;
                if(TotSec==0.0) sscanf(buff+2,"%f%s%s%f",&CurSec,dummy,dummy,&TotSec);
              }
              if(TotSec <=0.) TotSec=ETot;
//              printf("TotSec= %f\n",TotSec);
              if(CurPerc> 98) {StartSec=0;CurPerc=0;}
              StartSec= TotSec*CurPerc/100.0;
              if(VIDEO){
//                pthread_create(&(Pths),NULL, SkiptoStart,NULL);
                if(!FULLSCREEN) {
                 pthread_create(&(Pth),NULL, RunAControls,Dia);
                 Thread=1;
                }
              }
              else  {
                 pthread_create(&(Pth),NULL, RunAControls,Dia);
                 Thread=1;
              }
             }
             pthread_mutex_lock(&Mlock);
//            sscanf(buff,"%s%f%s%s%f",dummy,&CurSec,dummy,dummy,&TotSec);
//             sscanf(buff+2,"%f",&CurSec);
             sscanf(buff+2,"%f%s%s%s%d",&CurSec,dummy,dummy,dummy,&Volume);
             pthread_mutex_unlock(&Mlock);
             OCurSec=CurSec;
//             write(rsig1[1],"\n",1);
             CurPerc = CurSec/TotSec*100.;
             if(CurPerc > 99.99  ) {CurPerc=0;CurSec=0;OCurSec=0;}
             CurV = (int) CurPerc;
            }  // if(ch > 0)
            if(Thread) {
              if(SIG) {
               write(rsig1[1],"\n",1);
               SIG=0;
              }
              if(kgThreadWaitPipe(rsig[0],0,100)) {
               read(rsig[0],CODE,6);
               if(CODE[0]=='q') {
                 if((FULLSCREEN)&&(Thread)){
                   write(sig[1],"\n",1);
                   if(Thread) pthread_join(Pth,&res);
                   Thread=0;
                 }
               }
               ProcessCode(CODE);
               SIG=1;
             }
            }
            if(OldV!=CurV) {
              OldV = CurV;
//              printf("%f %f\n",CurSec,TotSec);
//              kgSetSlideValue(Sld,OldV);
//              kgUpdateOn(Dia);
            }
         }
     }
     write(rsig1[1],"\n",1);
     write(sig[1],"\n",1);
     write(sig[1],"\n",1);
     sprintf(buff,"!z54Stopped at %6.2f(%6.2f)",CurSec,TotSec);
     WriteMessage(buff);
//     pthread_cancel(Pth);
//     if(Threads) pthread_join(Pths,&res);
     if(Thread) pthread_join(Pth,&res);
     kgSetSlideValue(Sld,OldV);
     kgUpdateOn(Dia);
     return 1;
}
int ProcessListMplayer(int pip0,int pip1,int Pid) {
     int Begin=0,Thread=0;
     static int Entry=0,SIG=0;
     pthread_t Pth;
     void *res;
     
     char buff[1000],Starting[100],CODE[10];
     int ch,dum;
//     close(pip1);
     VIDEO=Audio=0;
     vRate=1.0;vStart=0.0,vLen=0.0;
     aRate=1.0;aStart=0.0,aLen=0.0;
     DmuxTime=0.0;
     pipew= pip1;
     Pip0 = pip0;
     if(Entry) {
       while(kgThreadWaitPipe(sig[0],0,200));
       while(kgThreadWaitPipe(rsig[0],0,200));
       while(kgThreadWaitPipe(rsig1[0],0,200));
       Entry=0;
     }
     if( pipe(sig) < 0) return 0;
     if( pipe(rsig) < 0) return 0;
     if( pipe(rsig1) < 0) return 0;
     Entry=1;
     OCurSec=-1.0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) { printf("Got -\n"); }
         if(ch> 0) {
          if(buff[0]==' ') continue;
          if(ch=='\n') {
              if(Begin) continue;
              if(CheckString(buff,(char *)"==> Found video stream:")) {
                VIDEO=1;
              }
              if(CheckString(buff,(char *)"==> Found audio stream:")) {
                Audio=1;
              }
              if(CheckString(buff,(char *)"T:")) {
               sscanf(buff+2,"%f",&TotSec);
              }
             if(CheckString(buff,(char *)"Starting")) {
             }
          }
          if(ch=='\r') {
             Begin++;
             if(Begin==1) {
//              printf("%s\n",buff);
             if(CheckString(buff,(char *)"DEMUX_MPG_CTRL")) {
               int k=0;
               Begin=0;
                   while(buff[k]!=')')k++;
                   buff[k]=' ';
                   k=0;
                   while(buff[k]!='(')k++;
                   k++;
                   sscanf(buff+k,"%lf",&DmuxTime);
//                   printf("Demux: %lf \n",DmuxTime);
               continue;
             }
             if(!CheckString(buff,(char *)"A:")) {
               Begin=0;
               continue;
             }
              if( DmuxTime != 0.0) TotSec=DmuxTime;
              else {
                TotSec = (vLen-vStart)/vRate;
//              printf("TotSec=%f:%lf %lf %lf\n",TotSec,vStart,vLen,vRate);
                if(TotSec==0.0) TotSec=(aLen-aStart)/aRate;
                if(TotSec==0.0) sscanf(buff+2,"%f%s%s%f",&CurSec,dummy,dummy,&TotSec);
              }
              if(TotSec <=0.) TotSec=ETot;
//              printf("TotSec= %f\n",TotSec);
              if(CurPerc> 98) {StartSec=0;CurPerc=0;}
              StartSec= TotSec*CurPerc/100.0;
              if(VIDEO) pthread_create(&(Pth),NULL, RunFullScreen,sig);
              else  pthread_create(&(Pth),NULL, RunAControls,Dia);
              Thread=1;
             }
             pthread_mutex_lock(&Mlock);
             sscanf(buff+2,"%f%s%s%s%d",&CurSec,dummy,dummy,dummy,&Volume);
             pthread_mutex_unlock(&Mlock);
             OCurSec=CurSec;
             if(SIG) {
              write(rsig1[1],"\n",1);
              SIG=0;
             }
             CurPerc = CurSec/TotSec*100.;
             if(CurPerc > 99  ) {CurPerc=0;CurSec=0;OCurSec=0;}
             CurV = (int) CurPerc;
            }  // if(ch > 0)
            if(kgThreadWaitPipe(rsig[0],0,100)) {
               read(rsig[0],CODE,6);
               ProcessCode(CODE);
               SIG=1;
            }
            if(OldV!=CurV) {
              OldV = CurV;
            }
         }
     }
     write(rsig1[1],"\n",1);
     write(sig[1],"\n",1);
     write(sig[1],"\n",1);
     if(Thread) pthread_join(Pth,&res);
     kgSetSlideValue(Sld,OldV);
     kgUpdateOn(Dia);
     return 1;
}
int ProcessXrandr(int pip0,int pip1,int Pid) {
     char buff[1000],device[20],connection[20],reso[30],*rpt;
     int ch,i=0,j,found=0,Vis;
     Dlink *Resolist;
     DEVREC *dpt;
     if(Devlist!= NULL) {
        Resetlink(Devlist);
        while((dpt=(DEVREC *)Getrecord(Devlist))!= NULL) {
           Dempty(dpt->Resolist);
        }
        Dempty(Devlist);
     }
     Devlist=Dopen();
//     close(pip1);
     Device=0;
     HDMI=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
         i++;
         if(i==1) {
//          if(SearchString(buff,(char *)"Screen")>= 0) {
             j = SearchString(buff,(char *)"current");
             j+=7;
             i=j;
             while((buff[i]!='\n')&&(buff[i]!='\r')) {if(!isdigit(buff[i])) buff[i]=' ';i++;}
             sscanf(buff+j,"%d %d",&Cxres,&Cyres);
//             printf("Cxres:Cyres %d %d\n",Cxres,Cyres);
             Dxres=Cxres;
             Dyres=Cyres;
             if(Hxres==-1) {Hxres=Cxres;Hyres=Cyres;}
             if(Pxres==-1) {Pxres=Cxres;Pyres=Cyres;}
             continue;
         }
         if(buff[0]==' '){
           if(found) {
             rpt=(char *)malloc(30);
             sscanf(buff,"%s",rpt);
             Dadd(Resolist,rpt);
           }
           continue;
         }
         found=0;
         if(ch=='\n') {
           sscanf(buff,"%s%s%s",device,connection,reso);
           if(strcmp(connection,(char *)"connected")==0) {
             found=1;
             dpt= (DEVREC *)malloc(sizeof(DEVREC));
             Resolist=Dopen();
             dpt->Resolist=Resolist;
             strcpy(dpt->device,device);
             j=0;
             if(reso[0]=='(') Vis=0;
             else Vis=1;
             if(Vis) {
               while(reso[j]>' ') {
                 if(!isdigit(reso[j])) reso[j]=' ';
                 j++;
               }
               sscanf(reso,"%d%d",&Xres,&Yres);
             }
             device[4]='\0';
             dpt->Xres=Xres;
             dpt->Yres=Yres;
             dpt->pos=0;
             dpt->hdmi=0;
             if( strcmp(device,(char *)"HDMI")==0) {
               HDMI=1;
               dpt->hdmi=1;
               if(Vis) {
                 Hxres=Xres;
                 Hyres=Yres;
                 Dxres=Xres;
                 Dyres=Yres;
               }
               HVis=Vis;
             }
             else {
               PVis=Vis;
               if(Vis) {
                 Pxres=Xres;
                 Pyres=Yres;
               }
             }
             Dadd(Devlist,dpt);
             j=0; while(buff[j]!= '(')j++;
             buff[j]='\0';
             WriteMessage(buff);
             kgUpdateOn(Dia);
           }
         }
     }
     if( (Cyres!=Hyres)&&(Cyres !=Pyres)) Hpos=1;
     else Hpos=0;
//  WriteMessage("!c04!z54 Press 'Esc' to clear Input Box");
     sprintf(buff,"!z54 Press 'Esc' to clear Input Boxes");
     WriteMessage(buff);
     return 1;
}
int ProcessAplay(int pip0,int pip1,int Pid) {
     char buff[1000],device[20],connection[20],reso[30];
     int ch,i=0,j;
//     close(pip1);
     Card=0;
     Device=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
         if(SearchString(buff,(char *)"HDMI")>=0) {
           j = SearchString(buff,(char *)"card");
           j +=5;
           while(isdigit(buff[j])) {
              Card = Card*10+buff[j]-'0';
              j++;
           }
           j = SearchString(buff,(char *)"device");
           j +=7;
           while(isdigit(buff[j])) {
              Device = Device*10+buff[j]-'0';
              j++;
           }
           break;
         }
         else {
           j = SearchString(buff,(char *)"card");
           j +=5;
           while(isdigit(buff[j])) {
              Card = Card*10+buff[j]-'0';
              j++;
           }
           j = SearchString(buff,(char *)"device");
           j +=7;
           while(isdigit(buff[j])) {
              Device = Device*10+buff[j]-'0';
              j++;
           }
         }
     }
     return 1;
}
int ProcessPulse(int pip0,int pip1,int Pid) {
     char buff[1000],device[20],connection[20],reso[30];
     int ch,i=0,j;
     PULSE=0;
//     close(pip1);
     Card=0;
     Device=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
//           printf("%s\n",buff);
         if(SearchString(buff,(char *)"pulse")>=0) {
           PULSE=1;
         }
         if(SearchString(buff,(char *)"Pulse")>=0) {
           PULSE=1;
         }
     }
     return 1;
}
int ProcessPacmd_o(int pip0,int pip1,int Pid) {
     char buff[1000],device[20],connection[20],reso[30];
     int ch,i=0,j;
//     close(pip1);
     Card=0;
     Device=0;
     PULSE=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
//           printf("%s\n",buff);
         if(SearchString(buff,(char *)"Server String:")>=0) {
             PULSE=1;
         }
     }
     return 1;
}
int ProcessPacmd(int pip0,int pip1,int Pid) {
     char buff[1000],device[20],connection[20],reso[30];
     int ch,i=0,j,sinks=0;
//     close(pip1);
     Card=0;
     Device=0;
     PULSE=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
//           printf("%s\n",buff);
#if 0
         PULSE=1;
         if(SearchString(buff,(char *)"No PulseAudio")>=0) {
             PULSE=0;
         }
         if(SearchString(buff,(char *)"not running")>=0) {
             PULSE=0;
         }
#else
         if(sinks==1) {
           if(SearchString(buff,(char *)"device.string")>=0) {
             if(SearchString(buff,(char *)"hdmi")>=0) {
               fprintf(stderr,"Only HDMI sink... ignoring\n");
               PULSE=0;
               break;
             }
           }
         }
         else {
           if(SearchString(buff,(char *)"sink(s)")>=0) {
             sscanf(buff,"%d",&sinks);
//             fprintf(stderr,"Pulse sinks: %d\n",sinks);
#if 1
             if(sinks> 0) {
               PULSE=1;
               if(sinks==1) continue;
             }
#else
             if(sinks> 1) {
               PULSE=1;
             }
             else {
               PULSE=0;
               fprintf(stderr,"Only one sink: ignoring PULSE=%d\n",PULSE);
             }
#endif
             break;
           }
         }
#endif
     }
     return 1;
}
int ProcessPactl(int pip0,int pip1,int Pid) {
     char buff[1000],device[20],connection[20],reso[30];
     int ch,i=0,j;
//     close(pip1);
     Card=0;
     Device=0;
     PULSE=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
//           printf("%s\n",buff);
         PULSE=1;
         if(SearchString(buff,(char *)"Connection failure")>=0) {
             PULSE=0;
         }
     }
     return 1;
}
int ProcessPsme(int pip0,int pip1,int Pid) {
     char buff[1000],device[20],connection[20],reso[30];
     int ch,i=0,j;
//     close(pip1);
     Card=0;
     Device=0;
     PULSE=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
//           printf("%s\n",buff);
         if(SearchString(buff,(char *)"pulseaudio")>=0) {
             PULSE=1;
         }
     }
     return 1;
}
int runjobpe(char *job,int (*ProcessOut)(int,int,int)){
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2];
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0;
   if(job==NULL) return 0;
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
//   pipew =pip2[1];
   while(job[i]==' ') i++;
   strcpy(buff,job+i);
   i=0;
   while ( sscanf(buff+pos,"%s",pt) > 0 ) {
     args[i]=buff+pos;
     pos +=strlen(pt);
     i++;
     if(buff[pos]< ' ') break;
     buff[pos]='\0';
     pos++;
     while(buff[pos]==' ') pos++;
   }
   args[i]=NULL;
   if(i==0) return 0;
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
     dup(pip[1]);
     close(2);
     open("/dev/null",O_WRONLY|O_CREAT,0777);
     /*dup(pip[1]);*/
     close(pip[1]);
     execvpe(pgrpath,args,envrn);
     fprintf(stderr,"Failed to execute aplay\n");
     sleep(5);
     exit(1);
   }
   else {   /* parent process */
//     printf("Waiting pid: %d\n",pid);
     close(pip2[0]);
     close(pip[1]);
     if(ProcessOut != NULL)ProcessOut(pip[0],pip2[1],pid);
     waitpid(pid,&status,0);
     free(pgrpath);
     return 1;
   }
}
int runjob(char *job,int (*ProcessOut)(int,int,int)){
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2];
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0;
   if(job==NULL) return 0;
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
//   pipew =pip2[1];
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
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
     dup(pip[1]);
     close(2);
     open("/dev/null",O_WRONLY|O_CREAT,0777);
     /*dup(pip[1]);*/
     close(pip[1]);
     execv(pgrpath,args);
     fprintf(stderr,"Failed to execute \n");
     sleep(5);
     exit(1);
   }
   else {   /* parent process */
//     printf("Waiting pid: %d\n",pid);
     close(pip2[0]);
     close(pip[1]);
     if(ProcessOut != NULL)ProcessOut(pip[0],pip2[1],pid);
     waitpid(pid,&status,0);
     free(pgrpath);
     return 1;
   }
}
int changejob(char *job){
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2];
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0;
   if(job==NULL) return 0;
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
     if(pt[0]=='\\') {
      pos++;
      args[i]=buff+pos;
      while(buff[pos]!='\\')pos++;
      buff[pos]='\0';
      i++;
     }
     if(pt[0]=='\\') {
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
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;
   execv(pgrpath,args);
   fprintf(stderr,"Failed to execute \n");
   sleep(5);
   exit(1);
}

int runmplayer(char *job,int (*ProcessOut)(int,int,int)){
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2],argc;
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0;
   if(job==NULL) return 0;
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
   Mute=0;
//   pipew =pip2[1];
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
   argc=i;
#if 0
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;
#endif
#if 1
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
#if 0
     dup(pip[1]);
     close(2);
     open("/dev/null",O_WRONLY|O_CREAT,0777);
     /*dup(pip[1]);*/
     close(pip[1]);
#else
     open("/dev/null",O_WRONLY|O_CREAT,0777);
     close(2);
     dup(pip[1]);
     close(pip[1]);
#endif
#if 0
     execv(pgrpath,args);
#else
     Mplayer(argc,args);
#endif
     fprintf(stderr,"Failed to execute \n");
     exit(1);
   }
   else {   /* parent process */
//     printf("Waiting pid: %d\n",pid);
     close(pip2[0]);
     close(pip[1]);
     if(ProcessOut != NULL)ProcessOut(pip[0],pip2[1],pid);
     waitpid(pid,&status,0);
     if(pgrpath!= NULL) free(pgrpath);
     return 1;
   }
#else
     Mplayer(argc,args);
     return 1;
#endif
}
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **)){
   int ret =0;
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2],argc;
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0;
//   printf("Job= %s\n",job);
   if(job==NULL) return 0;
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
   Mute=0;
//   pipew =pip2[1];
   while(job[i]==' ') i++;
   strcpy(buff,job+i);
   i=0;
   while ( sscanf(buff+pos,"%s",pt) > 0 ) {
#if 0
//     printf("%s\n",pt);
     if(pt[0]=='\\') {
      pos++;
      args[i]=buff+pos;
      while(buff[pos]!='\\')pos++;
      buff[pos]='\0';
      i++;
     }
     else 
#endif
     if(pt[0]=='\"') {
      pos++;
      args[i]=buff+pos;
      while(buff[pos]!='\"')pos++;
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
 //    printf("%s ",args[i-1]);
   }
//   printf("\n");
   args[i]=NULL;
   if(i==0) return 0;
   argc=i;
#if 0
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;
#endif
#if 1
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
#if 0
     dup(pip[1]);
     close(2);
     open("/dev/null",O_WRONLY|O_CREAT,0777);
     /*dup(pip[1]);*/
     close(pip[1]);
#else
#if 0
//     open("/dev/null",O_WRONLY|O_CREAT,0777);
#else
     dup(pip[1]);
#endif
     close(2);
     dup(pip[1]);
     close(pip[1]);
#endif
#if 0
     execv(pgrpath,args);
#else
     if(function != NULL) function(argc,args);
#endif
     fflush(stderr);
     fflush(stdout);
     fprintf(stderr,"END:\n");
     exit(0);
   }
   else {   /* parent process */
//     printf("Waiting pid: %d\n",pid);
     close(pip2[0]);
     close(pip[1]);
     if(ProcessOut != NULL){
        ret=ProcessOut(pip[0],pip2[1],pid);
//        printf("Killing %d\n",pid);
        kill(pid,9);
     }
     waitpid(pid,&status,0);
     return ret;
   }
#else
     Mplayer(argc,args);
     return 1;
#endif
}
int RunFunction(char *job,int (*ProcessOut)(void *,int,int,int),
                         int (*function)(int,char **),void *Arg){
   int ret =0;
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2],argc;
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0;
   if(job==NULL) return 0;
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
   Mute=0;
//   pipew =pip2[1];
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
   argc=i;
#if 0
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;
#endif
#if 1
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
#if 0
     dup(pip[1]);
     close(2);
     open("/dev/null",O_WRONLY|O_CREAT,0777);
     /*dup(pip[1]);*/
     close(pip[1]);
#else
#if 0
//     open("/dev/null",O_WRONLY|O_CREAT,0777);
#else
     dup(pip[1]);
#endif
     close(2);
     dup(pip[1]);
     close(pip[1]);
#endif
#if 0
     execv(pgrpath,args);
#else
     if(function != NULL) function(argc,args);
#endif
     printf("END:\n");
     exit(0);
   }
   else {   /* parent process */
//     printf("Waiting pid: %d\n",pid);
     close(pip2[0]);
     close(pip[1]);
     if(ProcessOut != NULL){
        ret=ProcessOut(Arg,pip[0],pip2[1],pid);
//        printf("Killing %d\n",pid);
        kill(pid,9);
     }
     waitpid(pid,&status,0);
     return ret;
   }
#else
     Mplayer(argc,args);
     return 1;
#endif
}
int runjob_o(char *job,int (*ProcessOut)(int,int,int)){
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2];
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0;
   if(job==NULL) return 0;
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
//   pipew =pip2[1];
   while(job[i]==' ') i++;
   strcpy(buff,job+i);
   i=0;
   while ( sscanf(buff+pos,"%s",pt) > 0 ) {
     args[i]=buff+pos;
     pos +=strlen(pt);
     i++;
     if(buff[pos]< ' ') break;
     buff[pos]='\0';
     pos++;
     while(buff[pos]==' ') pos++;
   }
   args[i]=NULL;
   if(i==0) return 0;
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
     dup(pip[1]);
     close(2);
     open("/dev/null",O_WRONLY|O_CREAT,0777);
     /*dup(pip[1]);*/
     close(pip[1]);
     execv(pgrpath,args);
     fprintf(stderr,"Failed to execute aplay\n");
     sleep(5);
     exit(1);
   }
   else {   /* parent process */
//     printf("Waiting pid: %d\n",pid);
     close(pip2[0]);
     close(pip[1]);
     if(ProcessOut != NULL)ProcessOut(pip[0],pip2[1],pid);
     waitpid(pid,&status,0);
     free(pgrpath);
     return 1;
   }
}

int ProcessMediaInfo(int pip0,int pip1,int Pid) {
     int ch,Asp;
     char buff[1000],work[100],CODE[10];
     char *pt;
     float fps;
     int pos;
     Asp=0;
     Minfo.Video=Minfo.Audio=0;
     Minfo.TotSec=0;
     Minfo.AspectNu=Minfo.AspectDe=1.0;
     Minfo.Axres=Minfo.Ayres=1;
     Minfo.Rxres=Minfo.Ryres=1;
     Minfo.fps=0;
     while((ch=GetLine(pip0,buff)) ) {
//         printf("%s\n",buff);
//         fflush(stdout);
         if(ch< 0) continue;
         if( SearchString(buff,(char *)"VIDEO:")>=0) {
               if(SearchString(buff,(char *)"fps")>0) {
                 pos = SearchString(buff,(char *)"bpp");
                 if(pos>0) {
                    pt = buff+pos+4;
                    sscanf(pt,"%f",&fps);
                    Minfo.fps = fps;
                  }
               }
         }
         if(buff[0]==' ') continue;
             if(CheckString(buff,(char *)"==> Found video stream:")) {
                Minfo.Video=1;
                continue;
             }
             if(CheckString(buff,(char *)"==> Found audio stream:")) {
                Minfo.Audio=1;
                continue;
             }
             if(CheckString(buff,(char *)"T:")) {
               sscanf(buff+2,"%f",&(Minfo.TotSec));
               continue;
             }
             if(CheckString(buff,(char *)"Movie-Aspect")) {
                   int k=0;
                   if(CheckString(buff,(char *)"undefined")) continue;
                   k = SearchString(buff,(char *)"is");
                   k +=3;
                   sscanf(buff+k,"%s",work);
                   k=0;
                   while(work[k]>= ' ') {
                     if(work[k]==':') work[k]=' ';
                     k++;
                   }
                   sscanf(work,"%f%f",&(Minfo.AspectNu),&(Minfo.AspectDe));
                   Asp=1;
                   continue;
             }
             if(CheckString(buff,(char *)"VO:")) {
                 int k=0;
                 k = SearchString(buff,(char *)"]");
                 k++;
                 sscanf(buff+k,"%s",work);
                 k=0;
                 while(work[k]>= ' ') {
                     if(work[k]=='x') work[k]=' ';
                     k++;
                 }
                 sscanf(work,"%d%d",&(Minfo.Axres),&(Minfo.Ayres));
                 k = SearchString(buff,(char *)"=>");
                 k+=2;
                 sscanf(buff+k,"%s",work);
                 k=0;
                 while(work[k]>= ' ') {
                     if(work[k]=='x') work[k]=' ';
                     k++;
                 }
                 sscanf(work,"%d%d",&(Minfo.Rxres),&(Minfo.Ryres));
             }
             if(CheckString(buff,(char *)"AO:")) {
                 int k=0;
                 k = SearchString(buff,(char *)"ch");
                 if(k>=0) buff[k]='\0';
                 k = SearchString(buff,(char *)"Hz");
                 if(k>=0)  {
                   k+=3;
                   sscanf(buff+k,"%d",&(Minfo.Achnls));
//                   printf("Achnls= %d\n",Minfo.Achnls);
                 }
             }
     }
     if(!Asp) {
       Minfo.AspectNu=Minfo.Rxres;
       Minfo.AspectDe=Minfo.Ryres;
     }
     return 1;
}
int ProcessCheckVideo(int pip0,int pip1,int Pid) {
     int Begin=0;
     
     char buff[1000],Starting[100],status[50],Vreso[50];
     int ch,i;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
         if(ch=='\n') {
             if(CheckString(buff,(char *)"VIDEO:")) {
               sscanf(buff,"%s%s%s",Starting,status,Vreso);
               i=0;
               while(Vreso[i]!='\0') {
                 if(Vreso[i]=='x') Vreso[i]=' ';
                 i++;
               }
               sscanf(Vreso,"%d%d",&Vxres,&Vyres);
//               printf("Vxres- %d Vyres- %d\n",Vxres,Vyres);
             }
         }
     }
     return 0;
}
int ProcessRip(int pip0,int pip1,int Pid) {
   int Begin=0,Thread=0;
   static int Entry=0,SIG=0;
   pthread_t Pth;
   char buff[1000],dummy1[30],dummy2[30],CODE[10];
   int ch,i=0,j;
   float perc=0.0;
   void *res;
   Rperc=0.0;
   if( pipe(sig) < 0) return 0;
   if( pipe(rsig) < 0) return 0;
   while((ch=GetLine(pip0,buff)) ) {
//     printf("%s\n",buff);
     if(kgThreadWaitPipe(rsig[0],0,100)) {
          kill(Pid,9);
          break;
     }
     if(ch<= 0) continue;
     if(buff[0]==' ') continue;
     if(CheckString(buff,(char *)"dump:")){
       Begin++;
       if(Begin==1) {
          pthread_create(&(Pth),NULL, RunRip,Dia);
          Thread=1;
       }
       i=0;
       while(buff[i]!='~') {if(buff[i]<' ') break;i++;}
       if(buff[i]!='~') continue;
       i++;
       j=i;
       while(buff[i]!=')'){if(buff[i]<' ') break; i++;}
       if(buff[i]!=')') continue;
       buff[i]=' ';
       sscanf(buff+j,"%f",&perc);
       pthread_mutex_lock(&Mlock);
       Rperc=perc;
       pthread_mutex_unlock(&Mlock);
//       printf("%f\n",perc);
     }
   }
   write(sig[1],"\n",1);
   if(Thread)pthread_join(Pth,&res);
   return 1;
}
int ProcessCdrom(int pip0,int pip1,int Pid) {
   char buff[1000],dummy1[30],dummy2[30];
   int ch,i=0;
   CDROM=0;
   while((ch=GetLine(pip0,buff)) ) {
     if(ch< 0) continue;
     if(CheckString(buff,(char *)"Disc mode is listed as:")){
       i=0;
       while(buff[i]!=':') i++;
       i++;
       sscanf(buff+i,"%s",dummy);
       if(strcmp(dummy,(char *)"Error")==0) CDROM=0;
       else if(strcmp(dummy,(char *)"CD")==0) CDROM=1; //VCD
            else CDROM=2; //DVD
     }
   }
   return CDROM;
}
int CheckPulse(void){
   char buff[500],dummy[30];
   int i=0;
   sprintf(buff,"aplay -L");
   runjob(buff,ProcessPulse);             
   if(!PULSE){
          fprintf(stderr,"Pulseaudio daemon not running...\n");
   }
   if( PULSE) {
     sprintf(buff,"pacmd info");
     runjob(buff,ProcessPacmd);             
//     sprintf(buff,"ps -e");
//     runjob(buff,ProcessPsme);             
     if( PULSE) {
       sprintf(buff,"pactl info");
       runjob(buff,ProcessPactl);             
     }
     if(!PULSE){
          sprintf(buff,"No usable Pulseaudio ...");
          WriteMessage(buff);
     }
   }
//   printf("PULSE= %d\n",PULSE);
   return PULSE;
}
int CheckCdrom(void){
   char buff[500],dummy[30];
   int i=0;
   char *cdrom;
   cdrom = kgGetSelectedString(Brw);
#if 0
   sprintf(buff,"cd-info --cdrom-device=%-s --no-device-info -q --no-header",
            cdrom);
//   runjob(buff,ProcessCdrom);             
   runcdinfo(buff,ProcessCdrom);             
#else
   CDROM = get_disc_info((const char *)cdrom);
   first =0; last=-1;
   get_disc_tracks((const char *)cdrom,&first,&last);
#endif
    
//   printf("CDROM= %d\n",CDROM);
   return CDROM;
}
int CheckVideo(char *flname) {
   char buff[500];
   int i;
   i=0; while(flname[i]==' ')i++;
   if(flname[i]=='\"') 
    sprintf(buff,"Mplayer  -endpos 0 -vo null -ao null %s",flname);
   else sprintf(buff,"Mplayer  -endpos 0 -vo null -ao null \"%s\"",flname);
//   printf("%s\n",buff);
//   runjob(buff,ProcessCheckVideo);
//   runmplayer(buff,ProcessCheckVideo);
   runfunction(buff,ProcessMediaInfo,Mplayer);
   VIDEO=Minfo.Video;
   Vxres = Minfo.Axres;
   Vyres = Minfo.Ayres;
   return VIDEO;
}
int CheckMedia(char *flname) {
   char buff[500];
   int i;
   i=0; while(flname[i]==' ')i++;
   if(flname[i]=='\"') 
    sprintf(buff,"Mplayer  -endpos 0 -vo null -ao null %s",flname);
   else sprintf(buff,"Mplayer  -endpos 0 -vo null -ao null \"%s\"",flname);
//   printf("%s\n",buff);
   runfunction(buff,ProcessMediaInfo,Mplayer);
   VIDEO=Minfo.Video;
   Vxres = Minfo.Axres;
   Vyres = Minfo.Ayres;
//   printf("%s:  %d %d \n",flname,Minfo.Video,Minfo.Audio);
   if((!Minfo.Audio)&&(!Minfo.Video)) return 0;
   else return 1;
}
MEDIAINFO * GetMediaInfo(char *flname) {
   char buff[500];
   MEDIAINFO *mpt,mtmp;
   int i;
   mtmp = Minfo;
   i=0; while(flname[i]==' ')i++;
   if(flname[i]=='\"') 
    sprintf(buff,"Mplayer  -endpos 0 -vo null -ao null %s",flname);
   else sprintf(buff,"Mplayer  -endpos 0 -vo null -ao null \"%s\"",flname);
//   printf("%s\n",buff);
   runfunction(buff,ProcessMediaInfo,Mplayer);
   mpt = (MEDIAINFO *)malloc(sizeof(MEDIAINFO));
   *mpt= Minfo;
    Minfo=mtmp;
    return mpt;
}
int ProcessTotTime(int pip0,int pip1,int Pid) {
     int Begin=0,i=0;
     char *pt;
     char buff[1000],Starting[100],status[50],dummy1[30],dummy2[30];
     int ch;
     VIDEO=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
         if(ch=='\n') {
             i=0;while((buff[i]>=' ')||(buff[i]=='\t')) i++;
             buff[i]='\0';
//             printf("%s\n",buff);
             sscanf(buff,"%s",Starting);
             if(strcmp(Starting,(char *)"Video")==0) {
                sscanf(buff,"%s%s",Starting,status);
                if(strcmp(status,(char *)"stream")==0 ) {
//                  printf("%s\n",buff);
                  sscanf(buff,"%s%s%s", Starting,status,dummy1,dummy2);
                  if(strcmp(dummy2,(char *)"mandatory!")==0) {
                     VIDEO=0;
                  }
                }
                else {
                  VIDEO =1;
                  sscanf(buff,"%s%s%s%s%s%s%s%s%s%f",
                      Starting,status,status,status,status,status,
                      status,status,status,&TotSec);
                 if(TotSec <=0.) TotSec=ETot;
                }
             }
             else if(strcmp(Starting,(char *)"Exiting...")==0) {
                     VIDEO=0;
                  }
         }
         if(ch=='\r') {
            Begin++;
         }
     }
     return 0;
}
int ProcessGetRtime(int pip0,int pip1,int Pid) {
     int Begin=0;
     static int Entry=0,SIG=0;
     pthread_t Pth;
     
     char buff[1000],Starting[100],CODE[10];
     int ch,dum;
     float dumsec;
//     close(pip1);
     VIDEO=Audio=0;
     Chapters=1;
     vRate=1.0;vStart=0.0,vLen=0.0;
     aRate=1.0;aStart=0.0,aLen=0.0;
     DmuxTime=0.0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
//         printf("%s\n",buff);
         if(buff[0]==' ') continue;
         if(ch=='\n') {
             if(Begin) continue;
             if(CheckString(buff,(char *)"==> Found video stream:")) {
                VIDEO=1;
             }
             if(CheckString(buff,(char *)"==> Found audio stream:")) {
                Audio=1;
             }
             if(CheckString(buff,(char *)"T:")) {
               sscanf(buff+2,"%f",&TotSec);
             }
             if(CheckString(buff,(char *)"C:")) {
    //           sscanf(buff+2,"%d",&Chapters);
             }
             if(CheckString(buff,(char *)"DEMUX_MPG_CTRL")) {
                   int k=0;
//                   printf("%s\n",buff);
#if 1
                   while(buff[k]!=')')k++;
                   buff[k]=' ';
                   k=0;
                   while(buff[k]!='(')k++;
                   k++;
                   sscanf(buff+k,"%lf",&DmuxTime);
//                   printf("Demux: %lf \n",DmuxTime);
#endif
                   continue;
             }
             if(CheckString(buff,(char *)"[aviheader] Video stream found")) {
               while((ch=GetLine(pip0,buff)) ) {
                 if(ch< 0) continue;
                 if(CheckString(buff,(char *)"Rate:")) {
                   int k=0;
                   while(buff[k]!='=')k++;
                   k++;
                   sscanf(buff+k,"%lf",&vRate);
//                   printf("Rate: %lf \n",vRate);
                   continue;
                 }
                 if(CheckString(buff,(char *)"Start:")) {
                   int k=5;
                   while(buff[k]!=':')k++;
                   k++;
                   sscanf(buff+k,"%lf",&vStart);
                   while(buff[k]!=':')k++;
                   k++;
                   sscanf(buff+k,"%lf",&vLen);
//                   printf("Start: %lf Len: %lf\n",vStart,vLen);
                   continue;
                 }
                 if(CheckString(buff,(char *)"============"))break;
               }
               continue;
             }
             if(CheckString(buff,(char *)"[aviheader] Audio stream found")) {
               while((ch=GetLine(pip0,buff)) ) {
                 if(ch< 0) continue;
                 if(CheckString(buff,(char *)"Rate:")) {
                   int k=0;
                   while(buff[k]!='=')k++;
                   k++;
                   sscanf(buff+k,"%lf",&aRate);
                   continue;
                 }
                 if(CheckString(buff,(char *)"Start:")) {
                   int k=5;
                   while(buff[k]!=':')k++;
                   k++;
                   sscanf(buff+k,"%lf",&aStart);
                   while(buff[k]!=':')k++;
                   k++;
                   sscanf(buff+k,"%lf",&aLen);
                   continue;
                 }
                 if(CheckString(buff,(char *)"============"))break;
               }
               continue;
             }
             if(CheckString(buff,(char *)"Starting")) {
             }
         }
         if(ch=='\r') {
            Begin++;
            if(Begin==1) {
//              printf("%s\n",buff);
             if(CheckString(buff,(char *)"DEMUX_MPG_CTRL")) {
               int k=0;
               Begin=0;
                   while(buff[k]!=')')k++;
                   buff[k]=' ';
                   k=0;
                   while(buff[k]!='(')k++;
                   k++;
                   sscanf(buff+k,"%lf",&DmuxTime);
//                   printf("Demux: %lf \n",DmuxTime);
               continue;
             }
             if(!CheckString(buff,(char *)"A:")) {
               Begin=0;
               continue;
             }
                TotSec = (vLen-vStart)/vRate;
                if(TotSec==0.0) TotSec=(aLen-aStart)/aRate;
                if(TotSec==0.0) sscanf(buff+2,"%f%s%s%f",&dumsec,dummy,dummy,&TotSec);
//              printf("TotSec=%f:%lf %lf %lf\n",TotSec,vStart,vLen,vRate);
          }
         }
     }
//     printf("Got EOF\n");
     return 1;
}
int GetVcdChannel(int start) {
   int chnl=1,rchnl=0,TCur,TOld=-1;
   char *cdrom;
   cdrom = kgGetSelectedString(Brw);
   chnl=start+1;
   if(chnl<first) chnl=first;
   if(AllTracks) {
     if(chnl <= last) rchnl=chnl;
   }
   else {
     while(chnl<=last) {
      sprintf(Line,"mplayer_kg -quiet -endpos 0 -vo null -ao null -cdrom-device %s vcd://%-d",cdrom,chnl);
      runmplayer(Line,ProcessGetRtime);
      if((VIDEO==0)&&(Audio==0)) {chnl++;continue;}
      TCur = (int)TotSec;
      if(TCur>TOld) {TOld=TCur;rchnl=chnl;}
//      if(TCur > 120) { rchnl=chnl;break;}
      chnl++;
     }
   }
   CurPerc = kgGetSlideValue(Sld);
   TotSec= TOld;
   StartSec= TotSec*CurPerc/100.0;
   return rchnl;
}
int GetDvdChannel_o(int Track) {
   int chnl=1,rchnl=0,TCur,TOld=-1;
   char *cdrom;
   cdrom = kgGetSelectedString(Brw);
   while(1) {
      sprintf(Line,"mplayer -quiet -endpos 0 -vo null -ao null -dvd-device %s dvd://%-d",cdrom,chnl);
      runjob(Line,ProcessGetRtime);
      if((VIDEO==0)&&(Audio==0)) break;
      TCur = (int)TotSec;
//      printf("TCur TOld : %d %d %f\n",TCur,TOld,TotSec);
      if(TCur>TOld) {TOld=TCur;rchnl=chnl;}
      chnl++;
   }
   return rchnl;
}
int GetDvdChannel(int start) {
   int chnl=1,rchnl=0,TCur,TOld=-1;
   char *cdrom;
   cdrom = kgGetSelectedString(Brw);
   chnl=start+Chapters;

   if(chnl<first) chnl=first;
   if(AllTracks) {
    while(1) {
//      sprintf(Line,"mplayer -quiet -endpos 0 -vo null -ao null -dvd-device %s dvdnav://%-d -identify",cdrom,chnl);
      sprintf(Line,"mplayer -quiet -endpos 0 -vo null -ao null -dvd-device %s dvd://%-d -identify",cdrom,chnl);
      runmplayer(Line,ProcessGetRtime);
      if((VIDEO==0)&&(Audio==0)) {
        printf("VIDEO=0 Audio=0 chnl=%d Chapters=%d\n",chnl,Chapters);
        if(chnl> 3) break;
        else {chnl+=Chapters;continue;}
      }
      rchnl=chnl;
      break;
    }
   }
   else {
     while(1) {
      sprintf(Line,"mplayer -quiet -endpos 0 -vo null -ao null -dvd-device %s dvd://%-d -identify",cdrom,chnl);
      runmplayer(Line,ProcessGetRtime);
      if((VIDEO==0)&&(Audio==0)) {
        if(chnl> 3) break;
        else {chnl+=Chapters;continue;}
      }
      TCur = (int)TotSec;
      if(TCur>TOld) {TOld=TCur;rchnl=chnl;}
      chnl+=Chapters;
     }
   }
   CurPerc = kgGetSlideValue(Sld);
   TotSec= TOld;
   StartSec= TotSec*CurPerc/100.0;
   return rchnl;
}
int GetMedia(char *media) {
   char *cdrom;
   int chnl=0;
  media[0]='\0';
  switch(FilePlay) {
//   if(FilePlay==1) {
   case 2:  //Playlist
   case 1:
      sprintf(media,"%-s",FileName);
   break;
   case 3:
   {
      char *url;
      url = GetUrlCopy();
      if(url==NULL) break;
      if(strcmp(url,UrlOld)==0) {
        if(GRABING) free(url);
      }
      else {
        GRABING=0;
      }
      if(GRABING) {
        url=FileName;
      }
      else {
//        url = GetUrlCopy();
        if(url != NULL) {
          if(kgSearchString(url,(char *)"youtube.")>=0 ) {
             if(!TryGrabing()) break;
             else { free(url);url=FileName;}
          }
        }
      }
      if(url==NULL) break;
      sprintf(media,"\"%-s\" ",url);
      WriteMessage(media);
      if(!GRABING) free(url);
   }
   break;
   case 0:
   {
    cdrom = kgGetSelectedString(Brw);
    Vcd=Dvd=0;
    CheckCdrom();
    switch(CDROM) {
       case 1:
        Vcd=1; Dvd=0;
        chnl = GetVcdChannel(Track);
        Track=chnl;
        chnl = Track;
        if(chnl) {
          sprintf(media," -cdrom-device %s vcd://%-d ",cdrom,chnl);
        }
        else return 0;
        break;
       case 2:
        Vcd=0; Dvd=1;
        chnl = GetDvdChannel(Track);
        Track=chnl;
        chnl = Track;
        if(chnl) {
          sprintf(media," -dvd-device %s dvd//%-d ",cdrom,chnl);
        }
        else return 0;
        break;
       default:
        Vcd=0;Dvd=0;
        return 0;
    }
   }
   break;
   } //switch
   return CheckMedia(media);
}
int SetMplayer(void){
   char *cdrom;
   int chnl=0,Xasp,Yasp,Ws,Hs,Tracko;
   float Fac=10,Asp;
   char media[500];
   char fscstring[200];
   char vostring[200];
   if(NoVideo) {
      strcpy(vostring," -vo null");
   }
   else strcpy(vostring," -vo xv,x11");
#ifdef D_PULSE
   if((HDMI==0)||(HDMIAUDIO==0)) {
     CheckPulse();
     printf("CheckPulse : %d\n",PULSE);
   }
#endif
   cdrom = kgGetSelectedString(Brw);
   unsetenv((char *)"ALSADEV");
   if(HDMI&&HDMIAUDIO) {
//        setenv((char *)"ALSADEV","hw:0,3",1);
          printf("HDMI identified: Setting ALSADEV\n");
          sprintf(envrn[0],"ALSADEV=hw:%-d,%-d",Card,Device);  
          putenv(envrn[0]);
          printf("ALSADEV: %s\n",getenv("ALSADEV"));
          sprintf(media,"amixer sset IEC958  on");
          runjob(media,NULL);
   }
   else {
    envrn[0][0]='\0';
    Device=0;
    sprintf(envrn[0],"ALSADEV=hw:%-d,%-d",Card,Device);  
    putenv(envrn[0]);
    printf("ALSADEV: %s\n",getenv("ALSADEV"));
    if(!PULSE) {
          sprintf(media,"amixer sset IEC958  on");
          runjob(media,NULL);
    }
   }
   if(FULLSCREEN) {
#if 1
    if((Xo<=2)&&(Yo<=2)) {
     sprintf(fscstring," -fs  -fstype stay_on_top,above,fullscreen -ontop  -volume %-d -volstep %-d "
          ,Volume,Vstep);
    }
    else {
#if 0
#else
   
     Xasp = Xres*(1-Ys);
     Yasp = Yres*(1-Xs);
     Xasp = (Xasp/16+2)*16;
     Yasp = (Yasp/16+2)*16;
     Hs = Yres*(1+Ys/2);
     Ws = Xres*(1+Xs/2);
     Hs = Yasp;
     Ws = Xasp;
//     Asp = (float)Ws/(float)Hs;
     Asp = (float)Xres/(float)Yres;
#if 0
// Old Working Method
     sprintf(fscstring," -fs -fstype stay_on_top,above,fullscreen "
      "-ontop  -volume %-d -volstep %-d  -aspect %-5.2f -vf-add "
      "crop=%-d:%-d -vf-add crop=%-d:%-d "
          ,Volume,Vstep,Asp,Xres,Yasp,Xasp,Yres);
#else
     Tracko=Track;
     GetMedia(media);
     Track=Tracko;
     if(VIDEO) {
       Asp = (Minfo.AspectNu*(1+Xs))/(Minfo.AspectDe*(1+Ys));
       Hs = Minfo.Ryres*(1+Ys);
       Ws = Minfo.Rxres*(1+Xs);
       Asp = (float)Ws/(float)Hs;
     }
     sprintf(fscstring," -fs -fstype stay_on_top,above,fullscreen -ontop "
      "-volume %-d -volstep %-d   "
      "  -aspect %-5.2f -vf-add crop=%-d:%-d "
//          ,Volume,Vstep,Asp,Xres+128,(int)((Yres)+128));
          ,Volume,Vstep,Asp,Xres,((int)((Xres)/Asp+1))/2*2);
//     printf("%s\n",fscstring);
#endif
#endif
    }
#else
     sprintf(fscstring,"-volume %-d -volstep %-d -ontop -noborder  -fs "
          ,Volume,Vstep);
#endif
   }
   else {
     sprintf(fscstring," -volume %-d -volstep %-d ",Volume,Vstep);
   }
   switch(FilePlay) {
//   if(FilePlay==1) {
   case 4:  //Playlist
   case 2:  //Playlist
   case 1:
   {
      sprintf(media,"\"%-s\" ",FileName);
      if(HDMI&&HDMIAUDIO) {
//        sprintf(Line,"mplayer -ao alsa:device=default -quiet "
        sprintf(Line,"mplayer -ao alsa:device=hdmi -quiet "
          "%-s %-s \"%-s\" -sspos %-d",vostring,fscstring,FileName,(int)CurPerc);
        if(SoftVol) strcat(Line,sfvstring);
        else strcat(Line,hdmistring);
      }
      else {
//        sprintf(Line,"mplayer -ao alsa:device=default -quiet  -ontop  -noborder "
       if(PULSE) {
        sprintf(Line,"mplayer -ao pulse -quiet "
          "%s %-s \"%-s\" -sspos %-d",vostring,fscstring,FileName,(int)CurPerc);
       }
       else {
        sprintf(Line,"mplayer -ao alsa:device=default -quiet "
          "%s %-s \"%-s\" -sspos %-d",vostring,fscstring,FileName,(int)CurPerc);
       }
        if(SoftVol) strcat(Line,sfvstring);
        else strcat(Line,hdmistring);
      }
//      fprintf(stderr,"%s\n",Line);
   }
   break;
   case 3:
   {
      char *url;
      url = GetUrlCopy();
      if(url==NULL) break;
      if(strcmp(url,UrlOld)==0) {
        if(GRABING) free(url);
      }
      else {
        GRABING=0;
      }
      if(GRABING) {
        url=FileName;
      }
      else {
//        url = GetUrlCopy();
        if(url != NULL) {
          if(kgSearchString(url,(char *)"youtube.")>=0 ) {
             if(!TryGrabing()) break;
             else { free(url);url=FileName;}
          }
        }
      }
      if(url==NULL) break;
      sprintf(media,"\"%-s\" ",url);
      WriteMessage(media);
//      printf("%s\n",media);
      if(HDMI&&HDMIAUDIO) {
//        sprintf(Line,"mplayer -ao alsa:device=default -quiet "
        sprintf(Line,"mplayer -ao alsa:device=hdmi -quiet "
          "%s %-s \"%-s\" -sspos %-d",vostring,fscstring,url,(int)CurPerc);
        if(SoftVol) strcat(Line,sfvstring);
        else strcat(Line,hdmistring);
      }
      else {
//        sprintf(Line,"mplayer -ao alsa:device=default -quiet  -ontop  -noborder "
       if(PULSE) {
        sprintf(Line,"mplayer -ao pulse -quiet "
          "%s %-s \"%-s\" -sspos %-d",vostring,fscstring,url,(int)CurPerc);
       }
       else {
        sprintf(Line,"mplayer -ao alsa:device=default -quiet "
          "%s %-s \"%-s\" -sspos %-d",vostring,fscstring,url,(int)CurPerc);
       }
        if(SoftVol) strcat(Line,sfvstring);
        else strcat(Line,hdmistring);
      }
      if(!GRABING) free(url);
//      fprintf(stderr,"%s\n",Line);
   }
   break;
   case 0:
   {
    Vcd=Dvd=0;
    CheckCdrom();
    switch(CDROM) {
       case 1:
        Vcd=1; Dvd=0;
        chnl = GetVcdChannel(Track);
        Track=chnl;
        if(chnl) {
         if(HDMI&&HDMIAUDIO) {
//          sprintf(Line,"mplayer -ao alsa:device=default -quiet "
          sprintf(Line,"mplayer -ao alsa:device=hdmi -quiet "
          "%s -cdrom-device %s %-s vcd://%-d -sspos %-d"
           ,vostring,cdrom,fscstring,chnl,(int)CurPerc);
          if(SoftVol) strcat(Line,sfvstring);
          else strcat(Line,hdmistring);
         }
         else {
         if(PULSE) {
          sprintf(Line,"mplayerg -ao pulse -quiet "
         " %s -cdrom-device "
         " %s %-s vcd://%-d -sspos %-d"
           ,vostring,cdrom,fscstring,chnl,(int)CurPerc);
         }
         else {
          sprintf(Line,"mplayerg -ao alsa:device=default -quiet "
         "%s -cdrom-device "
         " %s %-s vcd://%-d -sspos %-d"
           ,vostring,cdrom,fscstring,chnl,(int)CurPerc);
         }
          if(SoftVol) strcat(Line,sfvstring);
          else strcat(Line,hdmistring);
          sprintf(media," -cdrom-device %s vcd://%-d ",cdrom,chnl);
         }
        }
        else return 0;
        break;
       case 2:
        Vcd=0; Dvd=1;
        chnl = GetDvdChannel(Track);
        Track=chnl;
        if(chnl) {
         if(HDMI&&HDMIAUDIO) {
//          sprintf(Line,"mplayer -ao alsa:device=default -quiet "
          sprintf(Line,"mplayer -ao alsa:device=hdmi -quiet "
          "%s -dvd-device "
          "%s %-s dvd://%-d -sspos %-d "
           ,vostring,cdrom,fscstring,chnl,(int)CurPerc);
         if(SoftVol) strcat(Line,sfvstring);
         else strcat(Line,hdmistring);
         sprintf(media," -dvd-device %s dvd://%-d ",cdrom,chnl);
         }
         else {
           if(PULSE) {
            sprintf(Line,"mplayer -ao pulse -quiet "
            "%s  -dvd-device "
            "%s %-s dvd://%-d -sspos %-d "
           ,vostring,cdrom,fscstring,chnl,(int)CurPerc);
           }
           else {
            sprintf(Line,"mplayer -ao alsa:device=default -quiet "
            "%s -dvd-device "
            "%s %-s dvd://%-d -sspos %-d "
           ,vostring,cdrom,fscstring,chnl,(int)CurPerc);
           }
           if(SoftVol) strcat(Line,sfvstring);
           else strcat(Line,hdmistring);
         }// Not HMDI
        }
        else return 0;
        break;
       default:
        Vcd=0;Dvd=0;
        return 0;
    }
   }
   break;
   } //switch
//   printf("%s\n",envrn[0]);
//   printf("Line: %s\n",Line);
   if(Three2Two) {
//     strcat(Line,"-vo gl:stereo=3");
       CheckVideo(media);
       if(Vxres>Vyres) sprintf(media," -vf crop=%-d:%-d:0:0",Vxres/2,Vyres);
       else sprintf(media," -vf crop=%-d:%-d:0:0",Vxres,Vyres/2);
       strcat(Line,media);
   }
#if 0
   else {
       sprintf(media," -geometry  %-d:%-d:12:12",Xres-24,Yres-24);
       strcat(Line,media);
   }
#endif
   strcat(Line," -autosync 30 ");
   if(fabsf(AudioTiming)>0.01) {
     sprintf(dummy," -delay %6.2f ",-AudioTiming);
     strcat(Line,dummy);
   }
   else {
     if(HDMI&&HDMIAUDIO) {
       sprintf(dummy," -delay .0 ");
       strcat(Line,dummy);
     }
     else if(PULSE) {
//       sprintf(dummy," -delay .625 ");
       sprintf(dummy," -delay .0 ");
       strcat(Line,dummy);
     }
   }
   if( Minfo.SubTitleOff) {
//      printf("Sub Title off\n");
      sprintf(media," -vobsub \"%-s\" ","empty");
      strcat(Line,media);
      strcpy(media,"/usr/share/kgmplayer");
      chdir(media);
   }
   else if(((FilePlay==1)||(FilePlay==0))&&(Minfo.SubTitle)) {
     int index=0;
     {
//       printf("Sub Title on\n");
       index = GetBaseIndex(Minfo.SubFile);
       switch(Minfo.SubTitle) {
        default:
         sprintf(media," -sub \"%-s\" ",Minfo.SubFile+index);
         strcat(Line,media);
         strcpy(media,Minfo.SubFile);
         media[index-1]='\0';
         chdir(media);
         break;
        case 2:
         sprintf(media," -vobsub \"%-s\" ",Minfo.SubFile+index);
         strcat(Line,media);
         strcpy(media,Minfo.SubFile);
         media[index-1]='\0';
         chdir(media);
         break;
       }
     }
   }
//   printf("%s\n",Line);
   return 1;
}
int PrintDevlist(void) {
  DEVREC *dpt;
  Dlink *Rlist;
  char *reso;
  if(Devlist != NULL) {
    Resetlink(Devlist);
    while( (dpt=(DEVREC *)Getrecord(Devlist))!= NULL) {
      printf("%s %d %-dx%-d %d\n",
           dpt->device,dpt->hdmi,dpt->Xres,dpt->Yres,dpt->pos);
      Rlist=dpt->Resolist;
      Resetlink(Rlist);
      while((reso=(char *)Getrecord(Rlist))!= NULL) {
        printf("     %s\n",reso);
      }
    }
  }
  return 1;
}
int PlayFolder(void *Tmp,char *Folder) {
   int i,items;
   Dlink *Plink;
   char * pt=NULL;
   char **Files=NULL;
   char buff[500];
   if(Folder==NULL) return 0;
   if(Folder[0]=='\0') return 0;
   Files =  kgFileMenu(Folder,(char *)"*");
   if(Files==NULL) return 0;
   Plink=Dopen();
   i=0;
   while(Files[i]!= NULL) {
//     printf("%s\n",Files[i]);
     if(CheckMedia_d(Files[i]) ) Dadd(Plink,Files[i]);
     i++;
   }
   i=0;
        BRK=0;
        Resetlink(Plink);
        pt=(char *)Getrecord(Plink);
        if(pt==NULL) {
         Dfree(Plink);
         kgFreeDouble((void **)Files);
         return 0;
        }
        if(!DRANDOM) {
        while(1) {
//        while( (pt=(char *)Getrecord(Plink))!= NULL) {
          CurPerc = kgGetSlideValue(Sld);
          strcpy(FileName,Folder);
          if(strcmp(Folder,"/")!=0) strcat(FileName,"/");
          strcat(FileName,pt);
//          printf("%s\n",FileName);
          SetMplayer();
          sprintf(buff,"!f24Playing :!f25%s",FileName+GetBaseIndex(FileName));
          kgTruncateString(buff,50);
          WriteMessage(buff);
          kgUpdateOn(Tmp);
//`          printf("%s\n",Line);
          runmplayer(Line,ProcessMplayer);
          kgSetSlideValue(Sld,0);
          kgUpdateWidget(Sld);
          if(BRK) { BRK=0;break;}
          pt=(char *)Getrecord(Plink);
          if(pt==NULL) {
            if(DLOOP) {
              Resetlink(Plink);
              pt=(char *)Getrecord(Plink);
            }
            else break;
          }
        }
        }
        else {
         items=Dcount(Plink);
         if(DLOOP) {
         int opos=-1,pos;
         while(1) {
          Resetlink(Plink);
          pos= getrandom(items);
          if(items>1) while(pos==opos) pos=getrandom(items);
          opos=pos;
          Dposition(Plink,pos);
          pt=(char *)Getrecord(Plink);
          CurPerc = kgGetSlideValue(Sld);
          strcpy(FileName,Folder);
          if(strcmp(Folder,"/")!=0) strcat(FileName,"/");
          strcat(FileName,pt);
          SetMplayer();
          sprintf(buff,"!f24Playing :!f25%s",FileName+GetBaseIndex(FileName));
          kgTruncateString(buff,50);
//          WriteMessage(buff);
          kgUpdateOn(Tmp);
          runmplayer(Line,ProcessMplayer);
          kgSetSlideValue(Sld,0);
          kgUpdateWidget(Sld);
          if(BRK) { BRK=0;break;}
         }
         }
         else {
          Dlink *Clist,*Rlist=NULL;
          Clist = Dcopy(Plink);
          Rlist=Dopen();
          for(i=0;i<items;i++) {
            Resetlink(Clist);
            Dposition(Clist,getrandom(items-i));
            pt = (char *)Dpick(Clist);
            Dadd(Rlist,pt);
          }
          Dfree(Clist);
          Resetlink(Rlist);
          pt = (char *)Getrecord(Rlist);
         while(1) {
          CurPerc = kgGetSlideValue(Sld);
          strcpy(FileName,Folder);
          if(strcmp(Folder,"/")!=0) strcat(FileName,"/");
          strcat(FileName,pt);
          SetMplayer();
          sprintf(buff,"!f24Playing :!f25%s",FileName+GetBaseIndex(FileName));
          kgTruncateString(buff,50);
          WriteMessage(buff);
          kgUpdateOn(Tmp);
          runmplayer(Line,ProcessMplayer);
          kgSetSlideValue(Sld,0);
          kgUpdateWidget(Sld);
          if(BRK) { BRK=0;break;}
          pt=(char *)Getrecord(Rlist);
          if(pt==NULL) {
            break;
          }
         }
         Dfree(Rlist);
          
         } //if LOOP
        } // if(!RANDOM)
        Dfree(Plink);
        kgFreeDouble((void **)Files);
        return 1;
}
int  kgMplayersplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  char buff[500],*pt;
  int n,ret=1,items,HDMIO; 
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  AudioDelay=0;
  switch(butno) {
    case 1: 
      Play=1,Mute=0;Speed=1;
      HDMIO=HDMI;
      runjob((char *)"xrandr",ProcessXrandr);
//      PrintDevlist();
#if 1
      if(HDMI ) {
       if(HDMIO==0) {
         ResetHdmi();
         kgSetWidgetVisibility(B16,1);
         kgUpdateWidget(B16);
         kgSetGrpVisibility(Tmp,HAGrp,1);
         kgUpdateOn(Tmp);
         runjob((char *)"aplay -l",ProcessAplay);
         return 0;
       }
       else runjob((char *)"aplay -l",ProcessAplay);
//        printf("Card: %-d Device: %-d\n",Card,Device);
      }
      else {
        runjob((char *)"aplay -l",ProcessAplay);
      }
#else
      runjob((char *)"aplay -l",ProcessAplay);
#endif
      Xs = (float)kgGetSlideValue(Wsld)/100.0;
      Ys = (float)kgGetSlideValue(Hsld)/100.0;
//      printf("Xs:Ys %f %f\n",Xs,Ys);
      W= Xres+Xs*Xres+4;
      H= Yres+Ys*Yres+4;
      Xo = Xs*Xres*0.5+2;
      Yo = Ys*Yres*0.5+2;
      Volume = kgGetSlideValue(Vsld);
#if 0
      sprintf(Line,"amixer set Master %-d\%",Volume);
      runjob(Line,NULL);
#else
      sprintf(Line,"amixer set Master 100\%");
//      runjob(Line,NULL);
#endif
      CurPerc = kgGetSlideValue(Sld);
//      printf("Fileplay=%d\n",FilePlay);
//      fflush(stdout);
      switch(FilePlay) {
        case 2:  //PLaylist
        BRK=0;
        if(Plink==NULL) break;
        Resetlink(Plink);
        pt=(char *)Getrecord(Plink);
        if(pt==NULL) break;
        if(!RANDOM) {
        while(1) {
//        while( (pt=(char *)Getrecord(Plink))!= NULL) {
          CurPerc = kgGetSlideValue(Sld);
          strcpy(FileName,pt);
          SetMplayer();
          sprintf(buff,"!f24Playing :!f25%s",FileName+GetBaseIndex(FileName));
          kgTruncateString(buff,50);
          WriteMessage(buff);
          kgUpdateOn(Tmp);
          runmplayer(Line,ProcessMplayer);
          kgSetSlideValue(Sld,0);
          kgUpdateWidget(Sld);
          if(BRK) { BRK=0;break;}
          pt=(char *)Getrecord(Plink);
          if(pt==NULL) {
            if(LOOP) {
              Resetlink(Plink);
              pt=(char *)Getrecord(Plink);
            }
            else break;
          }
        }
        }
        else {
         items=Dcount(Plink);
         if(LOOP) {
         int opos=-1,pos;
         while(1) {
          Resetlink(Plink);
          pos= getrandom(items);
          if(items>1) while(pos==opos) pos=getrandom(items);
          opos=pos;
          Dposition(Plink,pos);
          pt=(char *)Getrecord(Plink);
          CurPerc = kgGetSlideValue(Sld);
          strcpy(FileName,pt);
          SetMplayer();
          sprintf(buff,"!f24Playing :!f25%s",FileName+GetBaseIndex(FileName));
          kgTruncateString(buff,50);
//          WriteMessage(buff);
          kgUpdateOn(Tmp);
          runmplayer(Line,ProcessMplayer);
          kgSetSlideValue(Sld,0);
          kgUpdateWidget(Sld);
          if(BRK) { BRK=0;break;}
         }
         }
         else {
          int i;
          Dlink *Clist,*Rlist=NULL;
          Clist = Dcopy(Plink);
          Rlist=Dopen();
          for(i=0;i<items;i++) {
            Resetlink(Clist);
            Dposition(Clist,getrandom(items-i));
            pt = (char *)Dpick(Clist);
            Dadd(Rlist,pt);
          }
          Dfree(Clist);
          Resetlink(Rlist);
          pt = (char *)Getrecord(Rlist);
         while(1) {
          CurPerc = kgGetSlideValue(Sld);
          strcpy(FileName,pt);
          SetMplayer();
          sprintf(buff,"!f24Playing :!f25%s",FileName+GetBaseIndex(FileName));
          kgTruncateString(buff,50);
          WriteMessage(buff);
          kgUpdateOn(Tmp);
          runmplayer(Line,ProcessMplayer);
          kgSetSlideValue(Sld,0);
          kgUpdateWidget(Sld);
          if(BRK) { BRK=0;break;}
          pt=(char *)Getrecord(Rlist);
          if(pt==NULL) {
            break;
          }
         }
         Dfree(Rlist);
          
         } //if LOOP
        } // if(!RANDOM)
        break;
        case 1:
        SetMplayer();
        sprintf(buff,"!f24Playing :!f25%s",FileName+GetBaseIndex(FileName));
        kgTruncateString(buff,50);
        WriteMessage(buff);
        kgUpdateOn(Tmp);
//      printf("%s\n",Line);
        runmplayer(Line,ProcessMplayer);
#if 0
        sprintf(buff,"!f24Playing FILE FINISHED or FAILED");
        kgTruncateString(buff,50);
        WriteMessage(buff);
#endif
        break;
        case 3:
        SetMplayer();
        sprintf(buff,"!f24Playing :URL");
        kgTruncateString(buff,50);
        WriteMessage(buff);
        kgUpdateOn(Tmp);
//      printf("%s\n",Line);
        runmplayer(Line,ProcessMplayer);
        if(GRABING) sprintf(buff,"!f24Playing :URL FINISHED/"
                                 "WAITING FOR GRAB");
        else sprintf(buff,"!f24Playing :URL FINISHED/FAILED");
        kgTruncateString(buff,50);
        WriteMessage(buff);
        break;
        case 0:
        Track=-2;
        Chapters=1;
        BRK=0;
        sprintf(buff,"!f24Searching Media & Track Pl. Wait...");
        WriteMessage(buff);
        kgUpdateOn(Tmp);
        while (1) {
          if(SetMplayer()<=0) break;
          printf("Track= %d\n",Track);
          if(Vcd) sprintf(buff,"!f24Playing :!f25VCD Track %-d",Track);
          else if(Dvd) sprintf(buff,"!f24Playing :!f25DVD Track %-d",Track);
               else {
                 sprintf(buff,"!f24NO MEDIA FOUND");
                 WriteMessage(buff);
                 kgUpdateOn(Tmp);
                 break;
               }
          WriteMessage(buff);
          kgUpdateOn(Tmp);
//        printf("%s\n",Line);
          runmplayer(Line,ProcessMplayer);
          if(OldV>=99){
           OldV=0;
           kgSetSlideValue(Sld,0);
           kgUpdateWidget(Sld);
          }
          if(!AllTracks) {BRK=0;break;}
          if(BRK) { BRK=0;break;}
          else {
           kgSetSlideValue(Sld,0);
           kgUpdateWidget(Sld);
          }
        }
        break;
        case 4:
         PlayFolder(Tmp,DirName);
        break;
      }
      ret=0;
      break;
    case 2: 
      break;
  }
  kgSkipEvents((DIALOG *)Tmp);
  return ret;
}
void  kgMplayersplbutton1init(DIL *B,void *pt) {
}
int  kgMplayerbrowser1callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIRA *R;DIALOG *D;void *pt; 
  ThumbNail **th; 
  void *Wid;
  int ret=1; 
  static int oitem=-1;
  if(item== oitem) return ret;
  oitem = item;
//  ResetSubTitle(&Minfo);
  D = (DIALOG *)Tmp;
  pt = D->pt;
  R = (DIRA *)kgGetWidget(Tmp,i);
  th = (ThumbNail **) R->list;
  Wid = kgGetWidget(Tmp,3);
  CurSec=0;
  CurPerc=0;
  OldV=0;
  kgSetSlideValue(Sld,OldV);
  kgUpdateOn(Tmp);
  switch(item) {
    case 1:
      FilePlay=1;Vcd=0;Dvd=0;
      kgSetGrpVisibility(Tmp,StoffGrp,0);
      kgSetGrpVisibility(Tmp,SubGrp,0);
      kgSetGrpVisibility(Tmp,UrlGrp,0);
      kgSetGrpVisibility(Tmp,DirGrp,0);
//      kgUpdateGrp(Tmp,UrlGrp);
      kgSetWidgetVisibility(B17,0);
      kgSetWidgetVisibility(C19,0);
      kgSetWidgetVisibility(R20,0);
      kgSetWidgetVisibility(B13,0);
      kgSetWidgetVisibility(Brw,0);
      kgUpdateWidget(Brw);
      kgUpdateWidget(B13);
      kgUpdateWidget(R20);
      kgUpdateWidget(C19);
      kgUpdateWidget(B17);
      kgSetWidgetVisibility(Wid,1);
      kgUpdateWidget(Wid);
      kgSetGrpVisibility(Tmp,StoffGrp,1);
      if(!Minfo.SubTitleOff)kgSetGrpVisibility(Tmp,SubGrp,1);
      break;
    case 2: 
      FilePlay=0;Vcd=1;Dvd=0;
      kgSetGrpVisibility(Tmp,StoffGrp,0);
      kgSetGrpVisibility(Tmp,SubGrp,0);
      kgSetGrpVisibility(Tmp,UrlGrp,0);
      kgSetGrpVisibility(Tmp,DirGrp,0);
//      kgUpdateGrp(Tmp,UrlGrp);
      kgSetWidgetVisibility(Wid,0);
      kgSetWidgetVisibility(B17,0);
      kgSetWidgetVisibility(C19,0);
      kgUpdateWidget(Wid);
      kgUpdateWidget(B17);
      kgUpdateWidget(C19);
      kgSetWidgetVisibility(R20,1);
      kgSetWidgetVisibility(B13,1);
      kgSetWidgetVisibility(Brw,1);
      kgUpdateWidget(R20);
      kgUpdateWidget(B13);
      kgUpdateWidget(Brw);
      kgSetGrpVisibility(Tmp,StoffGrp,1);
      if(!Minfo.SubTitleOff)kgSetGrpVisibility(Tmp,SubGrp,1);
      break;
    case 3:  // Playlist
      FilePlay=2;Vcd=0;Dvd=0;
      kgSetGrpVisibility(Tmp,UrlGrp,0);
      kgSetGrpVisibility(Tmp,DirGrp,0);
      kgSetGrpVisibility(Tmp,StoffGrp,0);
      kgSetGrpVisibility(Tmp,SubGrp,0);
//      kgUpdateGrp(Tmp,UrlGrp);
      kgSetWidgetVisibility(Wid,0);
      kgSetWidgetVisibility(B13,0);
      kgSetWidgetVisibility(Brw,0);
      kgSetWidgetVisibility(R20,0);
      kgUpdateWidget(Wid);
      kgUpdateWidget(B13);
      kgUpdateWidget(Brw);
      kgUpdateWidget(R20);
      kgSetWidgetVisibility(B17,1);
      kgSetWidgetVisibility(C19,1);
      kgUpdateWidget(B17);
      kgUpdateWidget(C19);
      break;
    case 4:  // URL
      FilePlay=3;Vcd=0;Dvd=0;
      kgSetGrpVisibility(Tmp,DirGrp,0);
      kgSetGrpVisibility(Tmp,StoffGrp,0);
      kgSetGrpVisibility(Tmp,SubGrp,0);
      kgSetWidgetVisibility(Wid,0);
      kgSetWidgetVisibility(B13,0);
      kgSetWidgetVisibility(Brw,0);
      kgSetWidgetVisibility(R20,0);
      kgSetWidgetVisibility(B17,0);
      kgSetWidgetVisibility(C19,0);
      kgUpdateWidget(Wid);
      kgUpdateWidget(B13);
      kgUpdateWidget(Brw);
      kgUpdateWidget(R20);
      kgUpdateWidget(B17);
      kgUpdateWidget(C19);
      kgSetGrpVisibility(Tmp,UrlGrp,1);
//      kgSetCurrentWidget(D,kgGetWidgetId(D,kgGetNamedWidget(D,"url")));
      WriteMessage((char *)"may SEARCH for URL using string in url");
//      kgUpdateGrp(Tmp,UrlGrp);
      break;
    case 5:  // URL
      FilePlay=4;Vcd=0;Dvd=0;
      kgSetGrpVisibility(Tmp,UrlGrp,0);
      kgSetGrpVisibility(Tmp,StoffGrp,0);
      kgSetGrpVisibility(Tmp,SubGrp,0);
      kgSetWidgetVisibility(Wid,0);
      kgSetWidgetVisibility(B13,0);
      kgSetWidgetVisibility(Brw,0);
      kgSetWidgetVisibility(R20,0);
      kgSetWidgetVisibility(B17,0);
      kgSetWidgetVisibility(C19,0);
      kgUpdateWidget(Wid);
      kgUpdateWidget(B13);
      kgUpdateWidget(Brw);
      kgUpdateWidget(R20);
      kgUpdateWidget(B17);
      kgUpdateWidget(C19);
      kgSetGrpVisibility(Tmp,DirGrp,1);
      kgSetCurrentWidget(D,kgGetWidgetId(D,kgGetNamedWidget(D,(char *)"url")));
      WriteMessage((char *)"may play all media files in Folder");
//      kgUpdateGrp(Tmp,UrlGrp);
      break;
  }
  kgUpdateOn(Tmp);
  return ret;
}
void  kgMplayerbrowser1init(DIRA *R,void *pt) {
}
int  kgMplayerbrowser2callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIW *B; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  B = (DIW *) kgGetWidget(Tmp,i);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
int  kgMplayerbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  char buff[50];
  int n,ret =0,index; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  if(kgFolderBrowser(NULL,100,100,FileName,(char *)"*")) {
      ResetSubTitle(&Minfo);
      index = GetBaseIndex(FileName);
      kgTruncateString(FileName+index,40);
      strcpy(buff,"");
      strcat(buff,FileName+index);
      kgChangeButtonTitle(B,0,buff);
      kgRestoreString(FileName+index,40);
      kgUpdateWidget(B);
  }
  CurSec=0;
  CurPerc=0;
  OldV=0;
  kgSetSlideValue(Sld,OldV);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  kgMplayerbutton1init(DIN *B,void *pt) {
}
int  kgMplayerdslide1callback(int val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIHB *SD; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  SD = (DIHB *) kgGetWidget(Tmp,i);
  return ret;
}
int  kgMplayerdslide2callback(int val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIHB *SD; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  SD = (DIHB *) kgGetWidget(Tmp,i);
  return ret;
}
int  kgMplayerdslide3callback(int val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIHB *SD; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  SD = (DIHB *) kgGetWidget(Tmp,i);
  return ret;
}
int  kgMplayerdslide4callback(int val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIHB *SD; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  SD = (DIHB *) kgGetWidget(Tmp,i);
  Volume = val;
#if 0
  sprintf(Line,"amixer set Master %-d\%",Volume);
  runjob(Line,NULL);
#endif
  return ret;
}
int  kgMplayerbrowser4callback(int item,int i,void *Tmp) {
  /***********************************
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1)
    Tmp :  Pointer to DIALOG
   ***********************************/
  DICH *C;DIALOG *D;void *pt;
  ThumbNail **th;
  int ret=1;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  C = (DICH *)kgGetWidget(Tmp,i);
  th = (ThumbNail **) C->list;
  Three2Two=kgGetSwitch(C,0);
  FULLSCREEN=kgGetSwitch(C,1);
  return ret;
}
void  kgMplayerbrowser4init(DICH *C,void *pt) {
  kgSetSwitch(C,0,Three2Two);
  kgSetSwitch(C,1,FULLSCREEN);
}
int  kgMplayerbrowser5callback(int item,int i,void *Tmp) {
  /***********************************
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1)
    Tmp :  Pointer to DIALOG
   ***********************************/
  DICH *C;DIALOG *D;void *pt;
  ThumbNail **th;
  int ret=1;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  C = (DICH *)kgGetWidget(Tmp,i);
  th = (ThumbNail **) C->list;
  RANDOM=kgGetSwitch(C,0);
  LOOP=kgGetSwitch(C,1);
  return ret;
}
void  kgMplayerbrowser5init(DICH *C,void *pt) {
  kgSetSwitch(C,0,RANDOM);
  kgSetSwitch(C,1,LOOP);
}


int kgMplayerinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  FileName[0]='\0';
  envrn[1]=NULL;
  envrn[0]=(char *)malloc(100);
  UrlOld[0]='\0';
  Minfo.SubTitle=0;
  Minfo.SubFile[0]='\0';
  Info = kgGetWidget(Tmp,4);
  Sld = kgGetWidget(Tmp,8);
  Wsld = kgGetWidget(Tmp,7);
  Hsld = kgGetWidget(Tmp,6);
  Vsld = kgGetWidget(Tmp,11);
  Brw = kgGetWidget(Tmp,2);
  B13 = kgGetWidget(Tmp,13);
  B16 = kgGetWidget(Tmp,16);
  B17 = kgGetWidget(Tmp,17);
  C19 = kgGetNamedWidget(Tmp,(char *)"kgMplayerWidget20");
  R20 = kgGetWidget(Tmp,20);
  Dia = Tmp;
  DirPlayinit(Tmp);
  subtitleinit(Tmp);
  imgs2vinit(Tmp);

#ifdef D_PULSE
  CheckPulse();
#endif
  Volume = kgGetSlideValue(Vsld);
//  sprintf(hdmistring," -ao alsa:device=hdmi -softvol -softvol-max"
  sprintf(hdmistring," -softvol -softvol-max"
     " 200 ");
//     " 120 ");
  sprintf(sfvstring," -softvol -softvol-max"
     " %d ",SoftVolMax);
  sprintf(Line,"amixer set Master unmute");
  runjob(Line,NULL);
  sprintf(Line,"amixer set Master %-d\%",100);
  runjob(Line,NULL);
  sprintf(Line,"amixer set Master playback %-d",65536);
  runjob(Line,NULL);
  sprintf(Line,"amixer set Speaker playback %-d",65536);
  runjob(Line,NULL);
  runjob((char *)"xrandr",ProcessXrandr);
  if(HDMI) {
    ResetHdmi();
    kgSetWidgetVisibility(B16,1);
    kgUpdateWidget(B16);
    kgSetGrpVisibility(Tmp,HAGrp,1);
    kgUpdateOn(Tmp);
  }
  if(Minfo.SubTitleOff) {
    kgSetGrpVisibility(Tmp,SubGrp,0);
    kgUpdateOn(Tmp);
  }
//  PrintDevlist();
  W= Xres+Xs*Xres;
  H= Yres+Ys*Yres;
  Xo = Xs*Xres*0.5;
  Yo = Ys*Yres*0.5;
  pthread_mutex_init(&(Mlock),NULL);
  D->df=3;
  return ret;
}
int kgMplayercleanup(void *Tmp) {
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
int kgMplayerCallBack(void *Tmp,void *tmp) {
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
int kgMplayerResizeCallBack(void *Tmp) {
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
int  kgMplayerbutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,chnl; 
  char buff[500],FileName[200];
  char *cdrom;
  cdrom = kgGetSelectedString(Brw);
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  FileName[0]='\0';
  switch(butno) {
    case 1: 
#if 0
      sprintf(buff,"eject -r %-s",cdrom);
      runjob(buff,NULL);
#else
     eject_drive((const char *)cdrom);
#endif
      OldV=0;
      kgSetSlideValue(Sld,OldV);
      break;
    case 2: 
      WriteMessage((char *)"Checking Media: Pl. Wait...");
      kgUpdateOn(Dia);
      switch(CheckCdrom()) {
        case 1: //VCD
         Track=-2;
         Chapters=1;
         while(1) {
         BRK=0;
         chnl= GetVcdChannel(Track);
         if(chnl==0) break;
         Track=chnl;
         sprintf(buff,(char *)"Select output file(.mpg) for Track:%d",Track);
         WriteMessage((char *)buff);
         kgUpdateOn(Dia);
         if(kgFolderBrowser(NULL,100,100,FileName,(char *)"*")) {
          sprintf(buff,"mplayer_kg -cdrom-device %s -dumpstream -dumpfile %s vcd://%-d -ss %d",
                  cdrom,FileName,chnl,(int)StartSec);
//          printf("%s\n",buff);
          
          runmplayer(buff,ProcessRip);
         }
         if(!AllTracks) {BRK=0;break;}
         if(BRK) break;
         }
         break;
        case 2: //DVD
         Track=-2;
         Chapters=1;
         while(1) {
         chnl= GetDvdChannel(Track);
         if(chnl==0) break;
         Track=chnl;
         sprintf(buff,(char *)"Select output file(.mpg) for Track:%d",Track);
         WriteMessage((char *)buff);
         kgUpdateOn(Dia);
         if(kgFolderBrowser(NULL,100,100,FileName,(char *)"*")) {
          sprintf(buff,"mplayer_kg -dvd-device %s -dumpstream -dumpfile %s dvd://%-d -ss %d",
                  cdrom,FileName,chnl,(int)StartSec);
//          printf("%s\n",buff);
          runmplayer(buff,ProcessRip);
         }
         if(!AllTracks) {BRK=0;break;}
         if(BRK) break;
         }
         break;
        default:
         WriteMessage((char *)"NO MEDIA FOUND");
         kgUpdateOn(Dia);
         break;
      }
      break;
  }
  return ret;
}
void  kgMplayerbutton2init(DIN *B,void *pt) {
}
void  kgMplayerbutton3init(DIN *B,void *pt) {
}
int  kgMplayerbutton3callback(int butno,int i,void *Tmp) {
  /***********************************
    butno : selected item (1 to max_item)
    i :  Index of Widget  (0 to max_widgets-1)
    Tmp :  Pointer to DIALOG
   ***********************************/
  DIALOG *D;DIN *B;
  char buff[200];
  int n,ret =0;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1:
      if(Dcount(Devlist) >= 2) {
         sprintf(buff,"xrandr --output %-s --off",HdmiDev);
         runjob(buff,NULL);
         sprintf(buff,"xrandr --output %-s --auto",HdmiDev);
         runjob(buff,NULL);
          RunManage(Tmp);
          runjob((char *)"xrandr",ProcessXrandr);
      }
      break;
  }
  return ret;
}

int  kgMplayerbrowser3callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DICH *C;DIALOG *D;void *pt; 
  ThumbNail **th; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  C = (DICH *)kgGetWidget(Tmp,i);
  th = (ThumbNail **) C->list;

  SoftVol=kgGetSwitch(C,0);
  return ret;
}
void  kgMplayerbrowser3init(DICH *C,void *pt) {
  ThumbNail **th; 
  th = (ThumbNail **) C->list;
  kgSetSwitch(C,0,SoftVol);
}
int  kgMplayerbutton4callback(int butno,int i,void *Tmp) {
  /***********************************
    butno : selected item (1 to max_item)
    i :  Index of Widget  (0 to max_widgets-1)
    Tmp :  Pointer to DIALOG
   ***********************************/
  DIALOG *D;DIN *B;
  int n,ret =0;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1:
      RunPlayList(Tmp);
      if(CurPlayList[0]!='\0') {
        kgChangeButtonTitle(B,0,CurPlayList);
        kgUpdateWidget(B);
      }
      break;
  }
  return ret;
}
void  kgMplayerbutton4init(DIN *B,void *pt) {
}

int  kgMplayerbrowser6callback(int item,int i,void *Tmp) {
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
  AllTracks=item%2;
  Track=-2;
  Chapters=1;
  return ret;
}
void  kgMplayerbrowser6init(DIRA *R,void *pt) {
}
void *logo(int l,int w){
  DIG *fid;
  void *logoimg=NULL;
  fid = (DIG *)kgInitImage(l,w,8);
  kgUserFrame(fid,0.,0.,(float)l,(float)w);
  kgChangeColor(fid,40,70,70,70);
//  kgRoundedRectangleFill(fid,l*0.5,w*0.5,(float)l-1.,(float)w-1.,0,40,0.1);
  kgTextAngle(fid,90.);
  kgTextSize(fid,l*0.5,l*0.45,0.0);
  kgTextFont(fid,23);
  kgTextColor(fid,0);
  kgMove2f(fid,l*0.55+1,(float)l*0.5+1);
  kgWriteText(fid,(char *)"kgMplayer");
  kgTextColor(fid,15);
  kgMove2f(fid,l*0.55,(float)l*0.5-1);
  kgWriteText(fid,(char *)"kgMplayer");
  kgChangeColor(fid,30,245,245,255);
  kgTextColor(fid,30);
  kgMove2f(fid,l*0.55,(float)l*0.5);
  kgWriteText(fid,(char *)"kgMplayer");
  kgTextSize(fid,l*0.15,l*0.2,0.0);
  kgMove2f(fid,l*0.90,(float)l*0.05);
  kgTextFont(fid,8);
  kgChangeColor(fid,6,0,190,190);
  kgChangeColor(fid,9,20,25,15);
  kgTextColor(fid,Kuclr);
  kgWriteText(fid,(char *)"a KULINA frontend for Mplayer");
  logoimg = kgGetResizedImage(fid);
  kgCloseImage(fid);
  return logoimg;
}
