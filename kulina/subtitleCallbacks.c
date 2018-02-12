#include <kulina.h>
#include "mediainfo.h"
#define _GNU_SOURCE
char *GetExtension( char *flname);
char *RemoveExtension( char *flname);
typedef struct _DevRec {
  char device[50];
  int hdmi;
  int Xres,Yres;
  int pos;
  Dlink *Resolist;
} DEVREC;
MEDIAINFO Minfo;
extern char FileName[500];
int GetBaseIndex(char *s);
DIN *SubBut=NULL;
int ResetSubTitle(MEDIAINFO *Minfo) {
  Minfo->SubFile[0]='\0';
  Minfo->SubTitle=0;
  Minfo->SubFolder=NULL;
  if(SubBut != NULL) {
      kgChangeButtonTitle(SubBut,0,"< Select Subtitle >");
      kgUpdateWidget(SubBut);
  }
  return 1;
}
int  subtitlebutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,index; 
  char buff[500];
  char *ext;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  SubBut=B;
  n = B->nx*B->ny;
  if(FileName[0]!='\0') {
    index = GetBaseIndex(FileName);
    strcpy(buff,FileName);
    if(index>1) index--;
    buff[index]='\0';
    chdir(buff);
//    printf("%s\n",buff);
  }
  if(kgFolderBrowser(NULL,100,100,Minfo.SubFile,"*")) {
//    printf("%s\n",FileName);
      index = GetBaseIndex(Minfo.SubFile);
      kgTruncateString(Minfo.SubFile+index,40);
      strcpy(buff,"");
      strcat(buff,Minfo.SubFile+index);
//      kgChangeButtonTitle(B,0,buff);
      kgChangeButtonTitle(SubBut,0,buff);
      kgRestoreString(Minfo.SubFile+index,40);
      kgUpdateWidget(B);
      Minfo.SubTitle =1;
      ext = GetExtension(Minfo.SubFile+index);
      if(ext != NULL) {
       if((strcmp(ext,"sub")==0) ||(strcmp(ext,"SUB")==0)) {
         Minfo.SubTitle =2;
         RemoveExtension(Minfo.SubFile+index);
       }
      }
  }
  return ret;
}
void  subtitlebutton1init(DIN *B,void *pt) {
 SubBut=B;
}
int subtitleinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  SubBut = (DIN *)kgGetNamedWidget(Tmp,(char *)"subtitlebutton");
  return ret;
}
int subtitlecleanup(void *Tmp) {
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
int subtitleCallBack(void *Tmp,void *tmp) {
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
int subtitleResizeCallBack(void *Tmp) {
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
int subtitleWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
