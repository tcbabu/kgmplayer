#include "kulina.h"
#include "kgDrawingBoxCallbacks.h"

static void *Args=NULL,*Rets=NULL;

static DIAINTR *It = NULL;


static MODINTERFACE ModFuns[] = { 
    (MODINTERFACE) NULL 
};
static Dlink *ModuleList=NULL;
int kgDrawingBoxProcess(void *,int);
 /* Callback for  DTscroll   */ 

int kgDrawingBoxDTscrollcallback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIE *E; 
  int ret=1; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  DIG *G=(DIG *)kgGetNamedWidget(Tmp,Args);
  D = (DIALOG *)Tmp;
  E = (DIE *)kgGetWidget(Tmp,i);
  kgDrawingBoxProcess(G,item);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void * kgDrawingBoxDTscrollinit (DIE *E,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
}

 /* Callback for  DTexit   */ 

int kgDrawingBoxDTexitcallback( int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  if(It != NULL) {
    It->SwitchOff(NULL);
    kgUpdateOn(Tmp);
    ret =0;
  }
  switch(butno) {
    case 1: //  !w43!c02Close Tool 
      break;
  }
  return ret;
}
void  kgDrawingBoxDTexitinit (DIL *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}
int kgDrawingBoxSetup(void *Tmp,void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  /* you add any initialisation here */
  /* useful for setting is used as MakeGroup */
  return 1;
}
 
void * kgDrawingBoxCleanDia(void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  
/* you add any cleaning  here */

  return NULL;
}
 
 
void *  kgDrawingBoxAction(void *Tmp,void *Args) {
  return NULL;
} 
 
 
int   kgDrawingBoxOn(void *itmp) {
  if(itmp == NULL) {
    if(It != NULL) {
      if(It->Dtmp != NULL) kgSetGrpVisibility(It->Dtmp,It->GrpId,1);
      else return 0;
//      kgUpdateOn(It->Dtmp);
      return 1;
    } 
    return 0;
  }
  else {
      DIAINTR *It=(DIAINTR *)itmp;
      if(It->Dtmp != NULL) kgSetGrpVisibility(It->Dtmp,It->GrpId,1);
      else return 0;
//      kgUpdateOn(It->Dtmp);
      return 1;
  }
  return 0;
} 
int   kgDrawingBoxOff(void *itmp) {
  if(itmp == NULL) {
    if(It != NULL) {
      if(It->Dtmp != NULL) kgSetGrpVisibility(It->Dtmp,It->GrpId,0);
      else return 0;
//      kgUpdateOn(It->Dtmp);
      return 1;
    } 
    return 0;
  }
  else {
      DIAINTR *It=(DIAINTR *)itmp;
      if(It->Dtmp != NULL) kgSetGrpVisibility(It->Dtmp,It->GrpId,0);
      else return 0;
//      kgUpdateOn(It->Dtmp);
      return 1;
  }
  return 0;
} 
 
static char *GetPointer(char *str) { 
  char *pt; 
  pt = (char *)malloc(strlen(str)+1); 
  strcpy(pt,str); 
  return pt; 
} 
 
 
void * kgDrawingBoxInterface(void *args,void *rets) {
  /*********************************** 
   ***********************************/ 
  DIAINTR *it= (DIAINTR *)malloc(sizeof(DIAINTR));
  it->GrpId=0;
  it->xsh=0;
  it->ysh=0;
  it->RunDia = RunkgDrawingBox;
  it->MakeGroup = MakekgDrawingBoxGroup;
  it->Title = GetPointer((char *)"SomeTitle");
  it->Help = GetPointer( (char *)"Help");
  it->Action = kgDrawingBoxAction;
  it->Settings = kgDrawingBoxSetup;
  it->Cleanup  = kgDrawingBoxCleanDia;
  if(args != NULL) Args=args;
  if(rets != NULL) Rets=rets;
  it->args = Args;
  it->rets = Rets;
  it->SwitchOn = kgDrawingBoxOn;
  it->SwitchOff = kgDrawingBoxOff;
  it->Dtmp=NULL;
  It = it;
  return it;
}
 
 
int kgDrawingBoxinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
 /* pt[0] is inputs, given by caller */
  return ret;
}
int kgDrawingBoxcleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
 /* pt[1] is outputs, if any  to be given to caller */
 /* pt[0] is inputs, given by caller */
  return ret;
}
int ModifykgDrawingBox(void *Tmp,int GrpId) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
 /* pt[0] is inputs given by caller */
  DIA *d;
  int i,n;
  d = D->d;

  if( ModuleList == NULL) ModuleList = kgGetModuleList((void **)ModFuns);
  i=0;
  void *args=NULL;
  DIAINTR *Dt;
  Resetlink(ModuleList);
  while ( (Dt=(DIAINTR *)Getrecord(ModuleList)) != NULL) {
    Dt->GrpId = Dt->MakeGroup(Tmp,NULL);
    kgShiftGrp(Tmp,Dt->GrpId,Dt->xsh,Dt->ysh);
    Dt->Settings(Tmp,args);
    i++;
  };

  i=0;while(d[i].t!= NULL) {;
     i++;
  };
  n=1;
//  strcpy(D->name,"Kulina Designer ver 3.0");    /*  Dialog name you may change */
#if 0
  if(D->fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D->xo=D->yo=0; D->xl = xres-10; D->yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D->xo=D->yo=0; D->xl = xres; D->yl=yres;
//     D->StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
#endif
  return GrpId;
}

int kgDrawingBoxCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
int kgDrawingBoxResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
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
int kgDrawingBoxWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  return ret;
}
  

void *kgGetDrawingBox(void *Tmp,void *args,int xsh,int ysh) {
     DIAINTR *Dt=NULL;
     if( (Tmp != NULL)  ) {
         Dt = (DIAINTR *)kgDrawingBoxInterface(args,NULL);
         Dt->GrpId = ((int (*)(DIALOG *,void *))(Dt->MakeGroup))(Tmp,NULL);
         Dt->Dtmp = Tmp;
         Dt->xsh = xsh;
         Dt->ysh = ysh;         
         kgShiftGrp(Tmp,Dt->GrpId,xsh,ysh);
     }
     return Dt;
}
