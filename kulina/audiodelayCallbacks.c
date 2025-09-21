#include <kulina.h>
extern int AudioDelay;
extern int Volume,Pid,pipew,Playing;
extern int sig[2],rsig[2],rsig1[2],Pip0;
int  audiodelaydslide1callback(int val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIHB *SD; 
  int ret=1,OldA,Incr,k; 
  D = (DIALOG *)Tmp;
  SD = (DIHB *) kgGetWidget(Tmp,i);
  OldA=AudioDelay;
  AudioDelay=kgGetSlideValue(SD);
  Incr= (AudioDelay - OldA)/10;
//  printf("Incr= %d %d %d\n",Incr,AudioDelay,OldA);
  if(Incr > 0) {
    for(k=0;k<Incr;k++) write(pipew,"-",1);
  }
  else {
    Incr = -Incr;
    for(k=0;k<Incr;k++) write(pipew,"+",1);
  }
  return ret;
}
int audiodelayinit(void *Tmp) {
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
int audiodelaycleanup(void *Tmp) {
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
int audiodelayCallBack(void *Tmp,void *tmp) {
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
int audiodelayResizeCallBack(void *Tmp) {
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
int audiodelayWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
