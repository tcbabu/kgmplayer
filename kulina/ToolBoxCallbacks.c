#define D_HELPGRP
#include <kulina.h>
#include "ConvertData.h"
#include "images2videos.h"
#include "ToolGroup.h"
extern void *HelperImg;
extern char *HelperMsg;
extern IMGS2VDATA is2vdata;

extern int Tools;
extern CONVDATA cndata;
extern int AConGrp,RangeGrp,EnVoGrp,VConGrp,
           VaspGrp,VsizeGrp,VrangeGrp,VJGrp,AJGrp,AddGrp,MixGrp,AmixGrp,
           KarGrp,SilGrp,CutGrp,Is2vGrp;

extern int ToolHelpGrp,HelpButnGrp,ImageBoxGrp;

extern TOOLGRP ToolList[];

int SetGrpVis(DIALOG *Tmp,TOOLGRP *T,int item) {
  int k,id;
  id = item- 2;
  kgSetGrpVisibility(Tmp,HelpButnGrp,0);
  k=0;
  while(T[k].MakeGroup != NULL){
//   if(k != id) kgSetGrpVisibility(Tmp,T[k].GrpId,0);
   kgSetGrpVisibility(Tmp,T[k].GrpId,0);
   k++;
  }
  kgSetGrpVisibility(Tmp,ImageBoxGrp,0);
  kgSetGrpVisibility(Tmp,ImageBoxGrp,1);
  if((id >= 0) &&(id < k )) {
        HelperMsg= (char *) T[id].MsgImg;
#if 0
     if(T[id].RunHelp != NULL) {
        if(T[id].flag==1) T[id].RunHelp(NULL);
        else           T[id].RunHelp(Tmp);
        T[id].RunHelp = NULL;
     }
#endif
     kgSetGrpVisibility(Tmp,T[id].GrpId,1);
     kgSetGrpVisibility(Tmp,HelpButnGrp,1);
  }
  return 1;
}
int ResetGrpVis(DIALOG *Tmp,TOOLGRP *T) {
  int k,id;
  DIRA *ToolsBox = (DIRA *)kgGetNamedWidget(Tmp,"ToolsBox");
  kgSetGrpVisibility(Tmp,HelpButnGrp,0);
  k=0;
  while(T[k].MakeGroup != NULL){
   kgSetGrpVisibility(Tmp,T[k].GrpId,0);
   k++;
  }
  kgSetGrpVisibility(Tmp,ImageBoxGrp,0);
  kgSetSelection(ToolsBox,1);
  kgUpdateWidget(ToolsBox);
  kgUpdateOn(Tmp);
  return 1;
}

int  ToolBoxbrowser1callback(int item,int i,void *Tmp) {
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
  Tools=item -1;
#ifdef D_HELPGRP
  kgSetGrpVisibility(D,ToolHelpGrp,0);
#endif
  kgSetGrpVisibility(D,HelpButnGrp,0);
  kgSetGrpVisibility(D,EnVoGrp,0);
  kgSetGrpVisibility(D,RangeGrp,0);
  kgSetGrpVisibility(D,VaspGrp,0);
  kgSetGrpVisibility(D,VsizeGrp,0);
  kgSetGrpVisibility(D,VrangeGrp,0);
  switch(item) {
    case 2:
    SetGrpVis(D,ToolList,item);
    kgSetGrpVisibility(D,EnVoGrp,1);

    cndata.Enhfac=1.0;
    if(cndata.FullRange!=1) {
      kgSetGrpVisibility(D,RangeGrp,1);
    }
    break;
    case 3:
    SetGrpVis(D,ToolList,item);
    if(cndata.ChngAsp) kgSetGrpVisibility(D,VaspGrp,1);
    if(cndata.Scale) kgSetGrpVisibility(D,VsizeGrp,1);
    if(cndata.VFullRange!=1) {
      kgSetGrpVisibility(D,VrangeGrp,1);
    }
    break;
    case 1:
    SetGrpVis(D, ToolList,-1);
    break;
    default:
    SetGrpVis(D, ToolList,item);
    break;
  }
  kgUpdateOn(D);
  return ret;
}
void  ToolBoxbrowser1init(DIRA *R,void *pt) {
}
int  ToolBoxsplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***************:wq********************/ 
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
void  ToolBoxsplbutton1init(DIL *B,void *pt) {
}
int ToolBoxinit(void *Tmp) {
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
int ToolBoxcleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
#if 0
  if(cndata.Vlist != NULL) {
    Dempty((Dlink *) (cndata.Vlist));
    cndata.Vlist=NULL;
  }
  if(cndata.Alist != NULL) {
    Dempty((Dlink *) (cndata.Alist));
    cndata.Alist=NULL;
  }
#endif
  Dempty((Dlink *)is2vdata.List);
  is2vdata.List= NULL;
  return ret;
}
int ToolBoxCallBack(void *Tmp,void *tmp) {
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
int ToolBoxResizeCallBack(void *Tmp) {
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
int ToolBoxWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
