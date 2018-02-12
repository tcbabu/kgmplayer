#include "kulina.h"
extern char PrimDev[50],HdmiDev[50];
extern char **Plist,**Hlist;
extern int PReso,HReso,PVis,HVis,Hpos;
int runjob(char *job,int (*ProcessOut)(int,int,int));
int  Managesplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  Managesplbutton1init(DIL *B,void *pt) {
}
int  Managebrowser1callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIW *B; 
  int ret=1; 
  char buff[200];
  D = (DIALOG *)Tmp;
  B = (DIW *) kgGetWidget(Tmp,i);
  HReso=item-1;
  sprintf(buff,"xrandr --output %-s --mode %s",HdmiDev,Hlist[item-1]);
  runjob(buff,NULL);
  
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
int  Managebrowser2callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIRA *R;DIALOG *D;void *pt; 
  ThumbNail **th; 
  char buff[200];
  int ret=1,Posold; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  R = (DIRA *)kgGetWidget(Tmp,i);
  th = (ThumbNail **) R->list;
  Posold= Hpos;
  if(item==1) Hpos=0;
  else Hpos=1;
  if(Hpos!= Posold) {
    if(Hpos!=0){
      sprintf(buff,"xrandr --output %-s --above %-s",HdmiDev,PrimDev);
    }
    else {
      sprintf(buff,"xrandr --output %-s --same-as %-s",HdmiDev,PrimDev);
    }
    runjob(buff,NULL);
  }
  return ret;
}
void  Managebrowser2init(DIRA *R,void *pt) {
}
int  Managebrowser3callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIRA *R;DIALOG *D;void *pt; 
  ThumbNail **th; 
  char buff[200];
  int Old;
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  R = (DIRA *)kgGetWidget(Tmp,i);
  th = (ThumbNail **) R->list;
  Old=PVis;
  if(item==1) PVis=1;
  else        PVis=0;
  if(Old!=PVis) {
    if(PVis==0) {
      if(HVis !=0) sprintf(buff,"xrandr --output %-s --off",PrimDev);
      else PVis=1;
    }
    else   {
      sprintf(buff,"xrandr --output %-s --auto",PrimDev);
    }
    runjob(buff,NULL);
  }
  return ret;
}
void  Managebrowser3init(DIRA *R,void *pt) {
}
int  Managebrowser4callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIW *B; 
  int ret=1; 
  char buff[200];
  D = (DIALOG *)Tmp;
  B = (DIW *) kgGetWidget(Tmp,i);
  PReso=item-1;
  sprintf(buff,"xrandr --output %-s --mode %s",PrimDev,Plist[item-1]);
  runjob(buff,NULL);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
int  Managebrowser5callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIRA *R;DIALOG *D;void *pt; 
  ThumbNail **th; 
  int ret=1; 
  char buff[200];
  int Old;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  R = (DIRA *)kgGetWidget(Tmp,i);
  th = (ThumbNail **) R->list;
  Old=HVis;
  if(item==1) HVis=1;
  else        HVis=0;
  if(Old!=HVis) {
    if(HVis==0) {
      if(PVis !=0) sprintf(buff,"xrandr --output %-s --off",HdmiDev);
      else HVis=1;
    }
    else   {
      sprintf(buff,"xrandr --output %-s --auto",HdmiDev);
    }
    runjob(buff,NULL);
  }
  return ret;
}
void  Managebrowser5init(DIRA *R,void *pt) {
}
int Manageinit(void *Tmp) {
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
int Managecleanup(void *Tmp) {
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
int ManageCallBack(void *Tmp,void *tmp) {
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
int ManageResizeCallBack(void *Tmp) {
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
int ManageWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
