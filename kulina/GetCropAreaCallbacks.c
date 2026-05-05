#include <kulina.h>
#include <math.h>
#include "GetCropAreaCallbacks.h"

static void *Args=NULL,*Rets=NULL;

static DIAINTR *It = NULL;

static int Xi,Yi;
static MODINTERFACE ModFuns[] = { 
    (MODINTERFACE) NULL 
};
static Dlink *ModuleList=NULL;

 /* InitFunction for  GCAgbox   */ 

void GetCropAreaGCAgboxinit (int i,void *Tmp) {
  /*********************************** 
    int routine for grahics area 
   ***********************************/ 
  DIALOG *D;
  DIG *G;
  D = (DIALOG *)Tmp;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  G = D->d[i].g;
  G->D = (void *)(Tmp);
  char flname[500],buff[500];;
  void *img=NULL;
  char *imgfile = (char *)(pt[0]);
  float Vx=0.0,Vy=0.0;
  printf("File: %s\n",imgfile);
  strcpy(flname,"##");
  strcat(flname,imgfile);
  img = kgGetImage(imgfile);
  kgGetImageSize(img,&Xi,&Yi);
  printf("Res: %d %d\n",Xi,Yi);
  if(Xi > Yi) Vy = ((float)Yi/(float)Xi)*0.5;
  if(Xi < Yi) Vx = ((float)Xi/(float)Yi)*0.5;
  kgFreeImage(img);
  kgViewport(G,0.0+Vx,0.0+Vy,1.0-Vx,1.0-Vy);
  kgUserFrame(G,0.,0.,(float)Xi,(float) Yi);
  kgDrawImage(G,flname,0.,0.,(float)Xi,(float) Yi);
  DII *Ibox = (DII *)kgGetNamedWidget(D,(char *)"GCAdsp");
  sprintf(buff,"%s",flname+2);
  kgWrite(Ibox,buff);
  kgUpdateOn(Tmp);
  return ;
}

 /* Callback for  GCAbutn   */ 

int GetCropAreaGCAbutncallback( int butno,int i,void *Tmp) {
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
  DIG *G= (DIG *)kgGetNamedWidget(Tmp,(char *)"GCAgbox");
  char *imgfile = (char *)(pt[0]);
  if (imgfile == NULL) return 1;
  int *Vals;
  if(pt[1]==NULL ) pt[1]=malloc(sizeof(int)*4);
  Vals = (int *)(pt[1]);
  n = B->nx;
  float xx=100,yy=100,xo=10,yo=10;
  char flname[500];
  DII *Ibox = (DII *)kgGetNamedWidget(D,(char *)"GCAdsp");
  switch(butno) {
    case 1: //  Select Area 
      break;
    case 2: //  Select Area 
      ret = 0;
      kgCrossCursor(G,&xo,&yo);
      kgRectCursor(G,&xx,&yy,&xo,&yo);
      Vals[0] =(int)(xo+0.5);
      Vals[1] =  Yi - (int) (yo+0.5);
      Vals[2] = (int)(fabsf(xo-xx)+0.5);
      Vals[3] = (int) (fabsf(yo-yy)+0.5); 
      sprintf (flname,"Loc %d:%d Reso %d:%d",Vals[0],Vals[1],Vals[2],Vals[3]);
      kgWrite(Ibox,flname);
      kgUpdateOn(Tmp);
      
//    kgBoxCursor(G,&xx,&yy,&xo,&yo);  //Problem
      break;
    default:
      break;
    case 3:
      break;
  }
  return ret;
}
void  GetCropAreaGCAbutninit (DIL *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}
int GetCropAreaSetup(void *Tmp,void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  /* you add any initialisation here */
  /* useful for setting is used as MakeGroup */
  return 1;
}
 
void * GetCropAreaCleanDia(void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  
/* you add any cleaning  here */

  return NULL;
}
 
 
void *  GetCropAreaAction(void *Tmp,void *Args) {
  return NULL;
} 
 
 
int   GetCropAreaOn(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,1);
    else return 0;
    return 1;
  } 
  return 0;
} 
 
int   GetCropAreaOff(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,0);
    else return 0;
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
 
 
void * GetCropAreaInterface(void *args,void *rets) {
  /*********************************** 
   ***********************************/ 
  DIAINTR *it= (DIAINTR *)malloc(sizeof(DIAINTR));
  it->GrpId=0;
  // filled by MakeGroup  it->xsh=0;
  it->ysh=0;
  it->RunDia = RunGetCropArea;
  it->MakeGroup = MakeGetCropAreaGroup;
  it->Title = GetPointer((char *)"GetCropArea");
  it->Help = GetPointer( (char *)"No help yet, request");
  it->Action = GetCropAreaAction;
  it->Settings = GetCropAreaSetup;
  it->Cleanup  = GetCropAreaCleanDia;
  if(args != NULL) Args=args;
  if(rets != NULL) Rets=rets;
  it->args = Args;
  it->rets = Rets;
  it->SwitchOn = GetCropAreaOn;
  it->SwitchOff = GetCropAreaOff;
  it->Dtmp = NULL; // fiiled by MakeGroup 
  It = it;
  return it;
}
 
 
int GetCropAreainit(void *Tmp) {
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
int GetCropAreacleanup(void *Tmp) {
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
int ModifyGetCropArea(void *Tmp,int GrpId) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
 /* pt[0] is inputs given by caller */
  DIA *d;
  int i,n;
  kgCheckParentPosition(Tmp);
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

int GetCropAreaCallBack(void *Tmp,void *tmp) {
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
int GetCropAreaResizeCallBack(void *Tmp) {
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
int GetCropAreaWaitCallBack(void *Tmp) {
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
