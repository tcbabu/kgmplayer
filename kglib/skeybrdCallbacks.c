#include "kulina.h"
static char BUFF[100];
#define NOBKGR 1

#if 0
static char Label1[]="1234567890abcdefghijklmnopqrstuvwxyz,./?";
static char Label2[]="1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ,./?";
#else
static char Label1[]="1234567890qwertyuiopasdfghjkl/zxcvbnm,.?";
static char Label2[]="1234567890QWERTYUIOPASDFGHJKL/ZXCVBNM,.?";
#endif
static char Label3[]="1234567890~`!@#$%^&*()_+-={}|[]\\:\";',.<>?";

void kgModifyTextWidget(void *Tmp,int ch);
int ProcessShift(void *D);
int kgShowKey(int ch);
static char *kgButtonTitle(char * str,void *pt) {
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)pt;
  sprintf(BUFF,"!h15!d!h51!z%-s!f%2.2d!c%2.2d%-s",Kbrd->Sfac,Kbrd->Bfont,Kbrd->Bclr,str);
//  printf("%s\n",BUFF);
  return BUFF;
}
static char *Procpy(char *des,char *src,void *pt) {
   strcpy(des,kgButtonTitle(src,pt));
   return des;
}
static int ProcessLabel(void *Tmp,int butno,char *Label) {
  int CurWid;
  char ch;
  DIALOG *D;
  KEYBRD *Kbrd;
  D = (DIALOG *)Tmp;
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) { 
     Kbrd->CurWid = D->InputWid;
  }
  CurWid = Kbrd->CurWid;
  if(D->InputWid >= 0) {
     Kbrd->CurWid = D->InputWid;
     CurWid = Kbrd->CurWid;
  }
  if(CurWid< 0) return 0;
  kgSetCurrentWidget(D,CurWid);
  ch= Label[butno-1];
//  kgSendKeyEvent(D,ch);
  kgModifyTextWidget(D,ch);
  if(Kbrd->ShiftPress) {
    Kbrd->ShiftPress=0;
    ProcessShift(D);
  }
  return 1;
}

int  skeybrdbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) {
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0) return 0;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      kgSetCurrentWidget(D,Kbrd->CurWid);
      kgSendEndKeyEvent(Tmp);
      break;
    case 2: 
      kgSetCurrentWidget(D,Kbrd->CurWid);
      kgSendBackspaceKeyEvent(Tmp);
      break;
    case 3: 
      kgSetCurrentWidget(D,Kbrd->CurWid);
      kgSendEnterKeyEvent(Tmp);
      break;
    case 4: 
      kgSetCurrentWidget(D,Kbrd->CurWid);
//      kgSendSpaceKeyEvent(Tmp);
      kgModifyTextWidget(D,32);
      break;
  }
  kgShowKey('a');
  return ret;
}
void  skeybrdbutton1init(DIN *B,void *pt) {
   int i,n;
   char buff[3];
   BUT_STR * but;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)pt;
   but = B->buts;
   n = B->nx * B->ny;
   B->nodrawbkgr = NOBKGR;
   for(i=0;i<n;i++) {
    but[i].bkgr = Kbrd->ButClr;
   }
}
int  skeybrdbutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) {
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0) return 0;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      kgSetCurrentWidget(D,Kbrd->CurWid);
      kgSendHomeKeyEvent(Tmp);
      break;
    case 2: 
      kgSetCurrentWidget(D,Kbrd->CurWid);
      kgSendEscapeKeyEvent(Tmp);
      break;
    case 3: 
      kgSetGrpVisibility(D,Kbrd->sgrp,0);
      kgSetGrpVisibility(D,Kbrd->symgrp,0);
      kgSetGrpVisibility(D,Kbrd->cgrp,1);
      kgUpdateGrp(D,Kbrd->cgrp);
      kgUpdateOn(D);
      break;
    case 4: 
      kgSetGrpVisibility(D,Kbrd->sgrp,0);
      kgSetGrpVisibility(D,Kbrd->cgrp,0);
      kgSetGrpVisibility(D,Kbrd->symgrp,1);
      kgUpdateGrp(D,Kbrd->symgrp);
      kgUpdateOn(D);
      break;
  }
  kgShowKey('a');
 
  return ret;
}
void  skeybrdbutton2init(DIN *B,void *pt) {
   int i,n;
   char buff[3];
   BUT_STR * but;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)pt;
   but = B->buts;
   n = B->nx * B->ny;
   B->nodrawbkgr = NOBKGR;
   for(i=0;i<n;i++) {
    but[i].bkgr = Kbrd->ButClr;
   }
}
int  skeybrdbutton3callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) { 
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0) return 0;
  ProcessLabel(D,butno,Label1);
  kgShowKey('a');
  return ret;
}
void  skeybrdbutton3init(DIN *B,void *pt) {
   int i,n;
   char buff[3];
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)pt;
   BUT_STR * but;
   but = B->buts;
   n = B->nx * B->ny;
   B->nodrawbkgr = NOBKGR;
   for(i=0;i<n;i++) {
    buff[0]= Label1[i];
    buff[1]='\0';
    if(buff[0]=='!') {buff[1]='!';buff[2]='\0';}
    Procpy(but[i].title,buff,pt);
    but[i].bkgr = Kbrd->ButClr;
   }
}
int  skeybrdbutton4callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) {
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0) return 0;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      kgSetCurrentWidget(D,Kbrd->CurWid);
      kgSendHomeKeyEvent(Tmp);
      break;
    case 2: 
      kgSetCurrentWidget(D,Kbrd->CurWid);
      kgSendEscapeKeyEvent(Tmp);
      break;
    case 3: 
      kgSetGrpVisibility(D,Kbrd->cgrp,0);
      kgSetGrpVisibility(D,Kbrd->symgrp,0);
      kgSetGrpVisibility(D,Kbrd->sgrp,1);
      kgUpdateGrp(D,Kbrd->sgrp);
      kgUpdateOn(D);
      break;
    case 4: 
      kgSetGrpVisibility(D,Kbrd->cgrp,0);
      kgSetGrpVisibility(D,Kbrd->sgrp,0);
      kgSetGrpVisibility(D,Kbrd->symgrp,1);
      kgUpdateGrp(D,Kbrd->symgrp);
      kgUpdateOn(D);
      break;
  }
  kgShowKey('a');
  return ret;
}
void  skeybrdbutton4init(DIN *B,void *pt) {
   int i,n;
   char buff[3];
   BUT_STR * but;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)pt;
   but = B->buts;
   B->nodrawbkgr = NOBKGR;
   n = B->nx * B->ny;
   for(i=0;i<n;i++) {
    but[i].bkgr = Kbrd->ButClr;
   }
}
int  skeybrdbutton5callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) { 
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0) return 0;
  B = (DIN *)kgGetWidget(Tmp,i);
  ProcessLabel(D,butno,Label2);
  kgShowKey('a');
  return ret;
}
void  skeybrdbutton5init(DIN *B,void *pt) {
   int i,n;
   char buff[3];
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)pt;
   BUT_STR * but;
   but = B->buts;
   n = B->nx * B->ny;
   B->nodrawbkgr = NOBKGR;
   for(i=0;i<n;i++) {
    buff[0]= Label2[i];
    buff[1]='\0';
    if(buff[0]=='!') {buff[1]='!';buff[2]='\0';}
    Procpy(but[i].title,buff,pt);
    but[i].bkgr = Kbrd->ButClr;
   }
}
int  skeybrdbutton6callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) {
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0) return 0;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      kgSetCurrentWidget(D,Kbrd->CurWid);
      kgSendHomeKeyEvent(Tmp);
      break;
    case 2: 
      kgSetCurrentWidget(D,Kbrd->CurWid);
      kgSendEscapeKeyEvent(Tmp);
      break;
    case 3: 
      kgSetGrpVisibility(D,Kbrd->sgrp,0);
      kgSetGrpVisibility(D,Kbrd->symgrp,0);
      kgSetGrpVisibility(D,Kbrd->cgrp,1);
      kgUpdateGrp(D,Kbrd->cgrp);
      kgUpdateOn(D);
      break;
    case 4: 
      kgSetGrpVisibility(D,Kbrd->cgrp,0);
      kgSetGrpVisibility(D,Kbrd->symgrp,0);
      kgSetGrpVisibility(D,Kbrd->sgrp,1);
      kgUpdateGrp(D,Kbrd->sgrp);
      kgUpdateOn(D);
      break;
  }
  kgShowKey('a');
  return ret;
}
void  skeybrdbutton6init(DIN *B,void *pt) {
   int i,n;
   char buff[3];
   BUT_STR * but;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)pt;
   but = B->buts;
   n = B->nx * B->ny;
   B->nodrawbkgr = NOBKGR;
   for(i=0;i<n;i++) {
    but[i].bkgr = Kbrd->ButClr;
   }
}
int  skeybrdbutton7callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) { 
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0) return 0;
  B = (DIN *)kgGetWidget(Tmp,i);
  ProcessLabel(D,butno,Label3);
  kgShowKey('a');
  return ret;
}
void  skeybrdbutton7init(DIN *B,void *pt) {
   int i,n;
   char buff[3];
   BUT_STR * but;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)pt;
   but = B->buts;
   n = B->nx * B->ny;
   B->nodrawbkgr = NOBKGR;
   for(i=0;i<n;i++) {
    buff[0]= Label3[i];
    buff[1]='\0';
    if(buff[0]=='!') {buff[1]='!';buff[2]='\0';}
    Procpy(but[i].title,buff,pt);
    but[i].bkgr = Kbrd->ButClr;
   }
}
int skeybrdinit(void *Tmp) {
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
int skeybrdcleanup(void *Tmp) {
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
int skeybrdCallBack(void *Tmp,void *tmp) {
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
int skeybrdResizeCallBack(void *Tmp) {
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
int skeybrdWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
