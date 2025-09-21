#include <kulina.h>
static void *Dia;
extern float StartSec,TotSec,CurSec;
extern int sig[2],rsig[2],rsig1[2],Pip0;
int RunControls(void *arg) ;
int BRK=0;
void KillDisplay(void) {
  kgPushBackEvent((DIALOG *) Dia);
  kgPushBackEvent((DIALOG *)Dia);
//  printf("Pushed Back\n");
}
int FullScreeninit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  char dummy[2];
  int m,tm,ts,ESIG=0,offset,j;
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  kgHideCursor(D);
  Dia = Tmp;
  BRK=0;
  kgRaiseWindow(Tmp);
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
  return ret;
}
int FullScreencleanup(void *Tmp) {
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
int FullScreenCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
#if 0
  if( kgThreadWaitPipe(sig[0],0,100)){
      printf("kgThreadWaitPipe: Got\n");
      return 1;
  }
#endif
  if(kbe->event ==1) {
    if(kbe->button ==1) {
     kgVisibleCursor(D);
// TCB     if(RunControls(Tmp)){ ret=1;BRK=1;}
     if(RunControls(Tmp)){ ret=1;}
     kgHideCursor(D);
    }
  }
  kgSkipEvents((DIALOG *)Tmp);
  return ret;
}
int FullScreenResizeCallBack(void *Tmp) {
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
int FullScreenWaitCallback(void *Tmp) {
  int val;
// No effect
//  kgWindowStateAbove(Tmp);
  if(kgThreadWaitPipe(sig[0],0,100)==1) {
//    printf("FullScreenWaitCallback Got\n");
    return 1;
  }
  else {
   return 0;
  }
}
