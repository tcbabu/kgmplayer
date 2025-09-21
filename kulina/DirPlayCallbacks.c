#include <kulina.h>
int DRANDOM=0,DLOOP=0;
char DirName[500];
int kgGetFolder(void *,char *);
int  DirPlaybutton1callback(int butno,int i,void *Tmp) {
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
  char buff[500];
  int index; 
  n = B->nx*B->ny;
 // if(kgFolderBrowser(NULL,100,100,DirName,"*")) {
  strcpy(buff,DirName);
  if(kgGetFolder(NULL,buff)) {
      strcpy(DirName,buff);
//    printf("%s\n",DirName);
      kgTruncateString(DirName,40);
      strcpy(buff,"!c01");
      strcat(buff,DirName);
      kgChangeButtonTitle(B,0,buff);
      kgRestoreString(DirName,40);
      kgUpdateWidget(B);
  }
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  DirPlaybutton1init(DIN *B,void *pt) {
}
int  DirPlaybrowser1callback(int item,int i,void *Tmp) {
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
  DRANDOM=kgGetSwitch(C,0);
  DLOOP=kgGetSwitch(C,1);
  return ret;
}
void  DirPlaybrowser1init(DICH *C,void *pt) {
  kgSetSwitch(C,0,DRANDOM);
  kgSetSwitch(C,1,DLOOP);
}
int DirPlayinit(void *Tmp) {
  /*********************************** 
    Tmp :  :wqPointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  strcpy(DirName,"");
  return ret;
}
int DirPlaycleanup(void *Tmp) {
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
int DirPlayCallBack(void *Tmp,void *tmp) {
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
int DirPlayResizeCallBack(void *Tmp) {
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
int DirPlayWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
