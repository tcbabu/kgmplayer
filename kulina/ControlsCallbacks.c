#include "kulina.h"
extern int WMErr;
extern int InAttn;
static int WMEntry=0;
extern int BRK;
extern int Vstep;
extern float StartSec,TotSec,CurSec;
extern int Volume,Pid,pipew,Playing;
extern float CurPerc;
static char buff[500];
extern int sig[2],rsig[2],rsig1[2],Pip0;
extern pthread_mutex_t  Mlock;
extern FMGIMG pause_str,play_str,muted_str,volume_str;

int WaitPipes(int pipe1,int pipe2,int usecs);
int GetLine(int pip0,char *buff);
static char BS=0x08,Esc=27,UpAr[4]={27,'[','A','\0'},
      DownAr[4]={27,'[','B','\0'},
      RightAr[4]={27,'[','C','\0'},LeftAr[4]={27,'[','D','\0'},
      PgUp[4]={27,'[','5','~'},PgDown[4]={27,'[','6','~'};
extern int Speed,Mute,Play;
union kbinp { short kbint; char kbc[2];} kb;
int runjob(char *job,int (*ProcessOut)(int,int));
void SetButtonImages(void *Tmp) {
      void *B;
      B = kgGetWidget(Tmp,0);
      if(Play) kgChangeButtonNormalImage(B,1,(char *)(&pause_str));
      else kgChangeButtonNormalImage(B,1,(char *)(&play_str));
      if(Mute) kgChangeButtonNormalImage(B,3,(char *)(&volume_str));
      else kgChangeButtonNormalImage(B,3,(char *)(&muted_str));
      kgUpdateWidget(B);
      kgUpdateOn(Tmp);
}
int  Controlssplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  char buff[200];
  DIALOG *D;DIL *B; 
  int n,ret=0,ch; 
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  if(kgThreadWaitPipe(sig[0],0,10)==1){
    kgSetExit(D->parent);
    kgSetExit(D);
    return 1;
  }
  switch(butno) {
    case 2: 
      write(pipew,"p",1);
      Play++;
      Play= (Play%2);
      break;
    case 1: 
//      write(pipew,"q",1);
      if(Play==0) {write(pipew,"p",1);Play=1;}
      write(rsig[1]," q0000\n",7);
      ret=1;
      BRK=1;
      break;
    case 5: 
//      write(pipew,"q",1);
      if(Play==0) {write(pipew,"p",1);Play=1;}
      write(rsig[1]," q0000\n",7);
      ret=1;
      break;
    case 3: 
      if(Play==0) {write(pipew,"p",1);Play=1;}
      if(Speed==4) {write(pipew,&BS,1);Speed=1;}
      else {write(pipew,"}}}}",4); Speed=4;}
      Play=1;
      break;
    case 4:
      write(pipew,"m",1);
      Mute++;
      Mute %=2;
      Play= 1;
//      if(Mute) kgChangeButtonNormalImage(B,3,(char *)(&sound_str));
//      else kgChangeButtonNormalImage(B,3,(char *)(&muted_str));
      break;
  }
//  kgUpdateWidget(B);
//  kgUpdateOn(Tmp);
  SetButtonImages(Tmp);
#if 0
  if(Mute) {
        sprintf(buff,"amixer set Master %-d\%",0);
        runjob(buff,NULL);
  }
  else {
        sprintf(buff,"amixer set Master %-d\%",Volume);
        runjob(buff,NULL);
  }
#endif
  return ret;
}
int  AControlssplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  char buff[200];
  DIALOG *D;DIL *B; 
  int n,ret=0,ch; 
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  if(kgThreadWaitPipe(sig[0],0,10)==1){
    kgSetExit(D);
    return 1;
  }
  switch(butno) {
    case 2: 
      write(pipew,"p",1);
      Play++;
      Play= (Play%2);
      break;
    case 1: 
      write(pipew,"q",1);
      ret=1;
      BRK=1;
      break;
    case 5: 
      write(pipew,"q",1);
      ret=1;
      break;
    case 3: 
      if(Play==0) {write(pipew,"p",1);Play=1;}
      if(Speed==4) {write(pipew,&BS,1);Speed=1;}
      else {write(pipew,"}}}}",4); Speed=4;}
      Play=1;
      break;
    case 4:
      write(pipew,"m",1);
      Mute++;
      Mute %=2;
      Play= 1;
//      if(Mute) kgChangeButtonNormalImage(B,3,(char *)(&sound_str));
//      else kgChangeButtonNormalImage(B,3,(char *)(&muted_str));
      break;
  }
//  kgUpdateWidget(B);
//  kgUpdateOn(Tmp);
  SetButtonImages(Tmp);
#if 0
  if(Mute) {
        sprintf(buff,"amixer set Master %-d\%",0);
        runjob(buff,NULL);
  }
  else {
        sprintf(buff,"amixer set Master %-d\%",Volume);
        runjob(buff,NULL);
  }
#endif
  return ret;
}
void  Controlssplbutton1init(DIL *B,void *pt) {
}
int SetVolume(int val) {
  int ret=1,ic,j,vfac,retval; 
  float dvol;
  pthread_mutex_lock(&Mlock);
  dvol = val -Volume;
  pthread_mutex_unlock(&Mlock);
  vfac=Vstep;
  if(dvol> 0) {
          ic = dvol/vfac+0.5;
          for(j=0;j<ic;j++) {
           write(pipew,"0",1);
          }
  }
  else {
          ic = -dvol/vfac+0.5;
          for(j=0;j<ic;j++) {
           write(pipew,"9",1);
          }
  }
  return 1;
}
int  Controlsdslide1callback(int val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIHB *SD; 
  int ret=1,ic,j,ESIG=0,vfac,retval; 
  char buf[20];
  float dvol;
  char dummy;
  D = (DIALOG *)Tmp;
  if(kgThreadWaitPipe(sig[0],0,10)==1){
     kgSetExit(D->parent);
     kgSetExit(D);
     return 1;
  }
  vfac=Vstep;
  SD = (DIHB *) kgGetWidget(Tmp,i);
  val = kgGetSlideValue(SD);
  if(Mute) {
         Mute=0;
         write(pipew,"m",1);
         SetButtonImages(Tmp);
  }
  if(!Play) {
         write(pipew,"p",1);
         Play=1;
         SetButtonImages(Tmp);
  }
  if(kgThreadWaitPipe(sig[0],0,100)==1) { ESIG=1 ;}
  else {
          SetVolume(val);
  }
  if(ESIG) {
    kgRaiseWindow(D);
    kgSetExit(D);
    return 1;
  }
  return ret;
}
int  Controlsdslide2callback(int val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret=1,ic,j,ESIG=0,retval; 
  float Secs,Old;
  int offset;
  int tm,m,ts;
  char dummy;
  DIALOG *D;DIHB *SD; 
  D = (DIALOG *)Tmp;
  if(kgThreadWaitPipe(sig[0],0,10)==1){
     kgSetExit(D->parent);
     kgSetExit(D);
     return 1;
  }
  SD = (DIHB *) kgGetWidget(Tmp,i);
        if(!Play) {
         write(pipew,"p",1);
         Play=1;
         SetButtonImages(Tmp);
        }
        val = kgGetSlideValue(SD);
        pthread_mutex_lock(&Mlock);
        Secs = val/100.0*TotSec;
        Old = CurSec;
        pthread_mutex_unlock(&Mlock);
        
        offset = Secs - Old -2;
        if(abs(offset) < 20) return 0;
//        printf("%f %f :%f %d\n",TotSec,Old,Secs,offset);
        if(offset > 0) {
          m = offset/60;
          ts = (offset%60)/10;
          tm = m/10;
          m = m%10;
          if(kgThreadWaitPipe(sig[0],0,100)==1) { ESIG=1 ;}
          for(j=0;j<tm;j++) { 
           kgRaiseWindow(D);
           if(!ESIG) write(rsig[1]," T0000\n",7);
           else break;
           while((retval=WaitPipes(sig[0],rsig1[0],1000))< 1);
           if(retval==1) {ESIG=1;break;}
          }
          for(j=0;j<m;j++) {
           if(!ESIG) write(rsig[1]," M0000\n",7);
           else break;
           while((retval=WaitPipes(sig[0],rsig1[0],1000))< 1);
           if(retval==1) {ESIG=1;break;}
          }
          for(j=0;j<ts;j++) {
           kgRaiseWindow(D);
           if(!ESIG) write(rsig[1]," S    \n",7);
           else break;
           while((retval=WaitPipes(sig[0],rsig1[0],1000))< 1);
           if(retval==1) {ESIG=1;break;}
          }
        }
        else {
          offset = -offset;
          m = offset/60;
          ts = (offset%60)/10;
          tm = m/10;
          m = m%10;
          if(kgThreadWaitPipe(sig[0],0,100)==1) { ESIG=1 ;}
          for(j=0;j<tm;j++) { 
           kgRaiseWindow(D);
           if(!ESIG) write(rsig[1]," t    \n",7);
           else break;
           while((retval=WaitPipes(sig[0],rsig1[0],1000))< 1);
           if(retval==1) {ESIG=1;break;}
          }
          for(j=0;j<m;j++) {
           kgRaiseWindow(D);
           if(!ESIG) write(rsig[1]," m    \n",7);
           else break;
           while((retval=WaitPipes(sig[0],rsig1[0],1000))< 1);
           if(retval==1) {ESIG=1;break;}
          }
          for(j=0;j<ts;j++) {
           kgRaiseWindow(D);
           if(!ESIG) write(rsig[1]," s    \n",7);
           else break;
           while((retval=WaitPipes(sig[0],rsig1[0],1000))< 1);
           if(retval==1) {ESIG=1;break;}
          }
//          printf("Offset = %d\n",offset);
        }
//        kgSkipEvents(Tmp);
  if(ESIG) {
    kgRaiseWindow(D);
    kgSetExit(D);
    return 1;
  }
  pthread_mutex_lock(&Mlock);
  Secs = val/100.0*TotSec;
  CurSec=Secs;
  pthread_mutex_unlock(&Mlock);
  return ret;
}
int  AControlsdslide1callback(int val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIHB *SD; 
  int ret=1,ic,j,ESIG=0,vfac; 
  float dvol;
  char dummy;
  D = (DIALOG *)Tmp;
  if(kgThreadWaitPipe(sig[0],0,10)==1){
     kgSetExit(D);
     return 1;
  }
  vfac=Vstep;
  SD = (DIHB *) kgGetWidget(Tmp,i);
  val = kgGetSlideValue(SD);
  if(Mute) {
         Mute=0;
         write(pipew,"m",1);
         SetButtonImages(Tmp);
  }
  if(!Play) {
         write(pipew,"p",1);
         Play=1;
         SetButtonImages(Tmp);
  }
  if(kgThreadWaitPipe(sig[0],0,100)==1) { ESIG=1 ;}
  else SetVolume(val);
  if(ESIG) {
    kgRaiseWindow(D);
    kgSetExit(D);
    return 1;
  }
  return ret;
}
int  AControlsdslide2callback(int val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret=1,ic,j,ESIG=0,retval; 
  float Secs,Old;
  int offset;
  int tm,m,ts;
  char dummy;
  DIALOG *D;DIHB *SD; 
  D = (DIALOG *)Tmp;
  if(kgThreadWaitPipe(sig[0],0,10)==1){
     kgSetExit(D);
     return 1;
  }
  SD = (DIHB *) kgGetWidget(Tmp,i);
        if(!Play) {
         write(pipew,"p",1);
         Play=1;
         SetButtonImages(Tmp);
        }
        val = kgGetSlideValue(SD);
        pthread_mutex_lock(&Mlock);
        Secs = val/100.0*TotSec;
        Old = CurSec;
        pthread_mutex_unlock(&Mlock);
        
        offset = Secs - Old -2;
        if(abs(offset) < 20) return 0;
//        printf("%f %f :%f %d\n",TotSec,Old,Secs,offset);
        if(offset > 0) {
          m = offset/60;
          ts = (offset%60)/10;
          tm = m/10;
          m = m%10;
          if(kgThreadWaitPipe(sig[0],0,100)==1) { ESIG=1 ;}
          for(j=0;j<tm;j++) { 
           kgRaiseWindow(D);
           if(!ESIG) write(rsig[1]," T0000\n",7);
           else break;
           while((retval=WaitPipes(sig[0],rsig1[0],1000))< 1);
           if(retval==1) {ESIG=1;break;}
          }
          for(j=0;j<m;j++) {
           if(!ESIG) write(rsig[1]," M0000\n",7);
           else break;
           while((retval=WaitPipes(sig[0],rsig1[0],1000))< 1);
           if(retval==1) {ESIG=1;break;}
          }
          for(j=0;j<ts;j++) {
           kgRaiseWindow(D);
           if(!ESIG) write(rsig[1]," S    \n",7);
           else break;
           while((retval=WaitPipes(sig[0],rsig1[0],1000))< 1);
           if(retval==1) {ESIG=1;break;}
          }
        }
        else {
          offset = -offset;
          m = offset/60;
          ts = (offset%60)/10;
          tm = m/10;
          m = m%10;
          if(kgThreadWaitPipe(sig[0],0,100)==1) { ESIG=1 ;}
          for(j=0;j<tm;j++) { 
           kgRaiseWindow(D);
           if(!ESIG) write(rsig[1]," t    \n",7);
           else break;
           while((retval=WaitPipes(sig[0],rsig1[0],1000))< 1);
           if(retval==1) {ESIG=1;break;}
          }
          for(j=0;j<m;j++) {
           kgRaiseWindow(D);
           if(!ESIG) write(rsig[1]," m    \n",7);
           else break;
           while((retval=WaitPipes(sig[0],rsig1[0],1000))< 1);
           if(retval==1) {ESIG=1;break;}
          }
          for(j=0;j<ts;j++) {
           kgRaiseWindow(D);
           if(!ESIG) write(rsig[1]," s    \n",7);
           else break;
           while((retval=WaitPipes(sig[0],rsig1[0],1000))< 1);
           if(retval==1) {ESIG=1;break;}
          }
//          printf("Offset = %d\n",offset);
        }
//        kgSkipEvents(Tmp);
  if(ESIG) {
    kgRaiseWindow(D);
    kgSetExit(D);
    return 1;
  }
  pthread_mutex_lock(&Mlock);
  Secs = val/100.0*TotSec;
  CurSec=Secs;
  pthread_mutex_unlock(&Mlock);
  return ret;
}
int Controlsinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  char dummy[2];
  int ret = 1,val;
  int m,tm,ts,ESIG=0,offset,j;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  BRK=0;
#if 1
  if((!WMEntry)&&(WMErr==0)) {
    WMEntry=1;
    if(kgCheckAttention(Tmp)==0) {
      InAttn=0;
      printf("WM Issues:Controls Not Visible\n");
      write(rsig[1],"E0000\n",6);
      return 1;
    }
    else {
      if(kgMovePointer(D,40,40)==0) {
        InAttn=0;
        printf("WM Issues:Controls Not Visible\n");
        write(rsig[1],"E0000\n",6);
        return 1;
      }
      else {
        write(rsig[1],"O0000\n",6);
      }
    }
  }
#endif
  if(kgThreadWaitPipe(sig[0],0,10000)==1) {
//    printf("ControlsWaitCallBack Got\n");
    kgSetExit(D);
    kgRaiseWindow(D);
    return 1;
  }
  pthread_mutex_lock(&Mlock);
  val = CurSec/TotSec*100;
  kgSetSlideValue(kgGetWidget(Tmp,2),val);
  kgSetSlideValue(kgGetWidget(Tmp,1),Volume);
  pthread_mutex_unlock(&Mlock);
  kgUpdateOn(Tmp);
  return ret;
}
int Controlscleanup(void *Tmp) {
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
int ControlsWaitCallBack(void *Tmp) {
  int val,V,qsecs;
  static int count=0;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  qsecs=3;
  if( qsecs < TotSec*0.005) qsecs=TotSec*0.005;
  if(qsecs>30) qsecs=30;
//  printf ("qsec = %d %f %f\n",qsecs,TotSec,CurSec);
  if((TotSec - CurSec) < qsecs ){
    if(D->parent!= NULL) kgSetExit(D->parent);
    else kgSetExit(D);
    return 1;
  }
  if(kgThreadWaitPipe(sig[0],0,10000)==1) {
//    printf("ControlsWaitCallBack Got\n");
    if(D->parent!= NULL) kgSetExit(D->parent);
    else kgSetExit(D);
    return 1;
  }
  else {
   count%=100;
   if(count==0) {
     pthread_mutex_lock(&Mlock);
     val = CurSec/TotSec*100;
     if(val>99)  {
       if(D->parent!= NULL) kgSetExit(D->parent);
       else kgSetExit(D);
     }
     V = Volume;
     pthread_mutex_unlock(&Mlock);
     kgSetSlideValue(kgGetWidget(Tmp,2),val);
     val= kgGetSlideValue(kgGetWidget(Tmp,1));
     if(abs(V-val)> 2) kgSetSlideValue(kgGetWidget(Tmp,1),V);
     kgUpdateOn(Tmp);
   }
   count++;
   return 0;
  }
}
int AControlsWaitCallBack(void *Tmp) {
  static int count=0;
  int val,V;
  DIALOG *D;
  char buff[100];
  DIM *m5=NULL;
  D = (DIALOG *)Tmp;
  if(kgThreadWaitPipe(sig[0],0,10000)==1) {
//    printf("ControlsWaitCallBack Got\n");
    count=0;
    return 1;
  }
#if 1
  else {
   count%=50;
   if(count==0) {
     m5 = (DIM *)kgGetNamedWidget(Tmp,"TimePos");
     pthread_mutex_lock(&Mlock);
     val = CurSec/TotSec*100;
     sprintf(buff,"Position %-10.3f\n",CurSec);
     kgWrite(m5,buff);
     V = Volume;
     pthread_mutex_unlock(&Mlock);
     kgSetSlideValue(kgGetWidget(Tmp,2),val);
     val= kgGetSlideValue(kgGetWidget(Tmp,1));
     if(abs(V-val)> 2) kgSetSlideValue(kgGetWidget(Tmp,1),V);
     kgUpdateOn(Tmp);
   }
   count++;
   return 0;
  }
#endif
}
int WaitFor(void) {
   char dummy[50];
   GetLine(Pip0,buff);
   sscanf(buff,"%s%f",dummy,&CurSec);
   return 1;
}
int ControlsCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  char dummy[20];
  float Secs,Old;
  int offset,ch,ESIG=0;
  int tm,m,ts,j,val,vfac=10,ic;
  float dvol;
  int ret = 0;
  DIALOG *D;
  void *SD;
  KBEVENT *kbe;
  vfac=Vstep;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kgThreadWaitPipe(sig[0],0,100)==1) {
//    printf("Got ESIG\n");
    kgRaiseWindow(D);
    kgSetExit(D->parent);
    kgSetExit(D);
    ESIG=1;
    return 1;
  }
  return ret;
}
int AControlsCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  char dummy[20];
  float Secs,Old;
  int offset,ch,ESIG=0;
  int tm,m,ts,j,val,vfac=10,ic;
  float dvol;
  int ret = 0;
  DIALOG *D;
  void *SD;
  KBEVENT *kbe;
  vfac=Vstep;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kgThreadWaitPipe(sig[0],0,100)==1) {
//    printf("Got ESIG\n");
    kgSetExit(D);
    ESIG=1;
    return 1;
  }
  return ret;
}
int ControlsResizeCallBack(void *Tmp) {
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
int AControlsinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  char dummy[2];
  int ret = 1,val;
  int m,tm,ts,ESIG=0,offset,j;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  BRK=0;
  if(kgThreadWaitPipe(sig[0],0,10000)==1) {
    kgSetExit(D);
    kgRaiseWindow(D);
    return 1;
  }
  pthread_mutex_lock(&Mlock);
  val = CurSec/TotSec*100;
  kgSetSlideValue(kgGetWidget(Tmp,2),val);
  kgSetSlideValue(kgGetWidget(Tmp,1),Volume);
  pthread_mutex_unlock(&Mlock);
  kgUpdateOn(Tmp);
  return ret;
}
