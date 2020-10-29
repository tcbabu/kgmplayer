#define D_HELPGRP
#include <kulina.h>
char *HelperMsg;

extern int ToolHelpGrp,HelpButnGrp;

int MakeToolHelpGroup(DIALOG *D,void *arg);
char ** GetHelperMenu(void);



static char *GetNextString(char *str,int *pos) {
  int i=0,j,k,l,len;
  char *pt=NULL;
  i = *pos;
  k = *pos;
  j = 0;
  len = strlen(str);
  if( str[i]!='\0') {
    while(str[i]>= ' ') {i++; j++;}
    pt = (char *)malloc(j+2);
    *pos=i+1;
    if(*pos>len) *pos=len;
    l=0;
    while(l<=j) pt[l++]=str[k++];
    pt[l]='\0';
  }
  return pt;
  
}
char ** GetHelperMenu() {
  
  char **menu=NULL;
  int i=0,pos=0;
  char *spt;

  if(HelperMsg!= NULL) {
    Dlink *L=NULL;
    L = Dopen();
    while((spt=GetNextString(HelperMsg,&pos))!= NULL) {
       Dadd(L,spt);
    }
    menu = (char **)malloc(sizeof(char *)*(Dcount(L)+1));
    Resetlink(L);
    while ((spt=(char *)Getrecord(L)) != NULL) {
     menu[i++]=spt;
    }
    menu[i]=NULL;
    Dfree(L);
  }
  return menu;
}
int  ToolHelpsplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  void *Wid;
  DIS *S;
  DIP *P;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  switch(butno) {
    case 1: 
#ifdef D_HELPGRP
      Wid = kgGetNamedWidget(Tmp,(char *)"ToolHelpBox");
      kgSetWidgetVisibility(Wid,0);
      Wid = kgGetNamedWidget(Tmp,(char *)"ToolHelpButn");
      kgSetWidgetVisibility(Wid,0);
      Wid = kgGetNamedWidget(Tmp,(char *)"ToolHelpBkgr");
      kgSetWidgetVisibility(Wid,0);
      kgSetGrpVisibility(D,ToolHelpGrp,0);
#if 1
      kgRedrawDialog(D);
#endif
      kgUpdateOn(Tmp);
      ret=0;
#else 
      ret=1;
#endif
      break;
  }
  return ret;
}
void  ToolHelpsplbutton1init(DIL *B,void *pt) {
}
void  ToolHelpbrowser1init(DIS *S,void *pt) {
  int pos=0,i=0;
  char *spt;
  char **menu=NULL;
  int ret = 1;
#ifndef D_HELPGRP
  if(HelperMsg!= NULL) {
    Dlink *L=NULL;
    L = Dopen();
    while((spt=GetNextString(HelperMsg,&pos))!= NULL) {
       Dadd(L,spt);
    }
    menu = (char **)malloc(sizeof(char *)*(Dcount(L)+1));
    Resetlink(L);
    while ((spt=(char *)Getrecord(L)) != NULL) {
     menu[i++]=spt;
    }
    menu[i]=NULL;
    Dfree(L);
    S->menu =menu;
  }
#endif
}
int ToolHelpinit(void *Tmp) {
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
int ToolHelpcleanup(void *Tmp) {
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
int ToolHelpCallBack(void *Tmp,void *tmp) {
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
int ToolHelpResizeCallBack(void *Tmp) {
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
int ToolHelpWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
