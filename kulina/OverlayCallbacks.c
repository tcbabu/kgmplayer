#include <kulina.h>
#include "OverlayCallbacks.h"
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int FileStat(char *flname);
int kgffmpeg(int,char **);
int ffmpegfun(int,char **);
int ProcessSkip(int pip0,int pip1,int Pid);
int ProcessToPipe(int pip0,int pip1,int Pid);
int ProcessPrint(int pip0,int pip1,int Pid);
int MakeNewFileName(char *Infile,char *OutFile);
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int GetFolderName(char *infile,char *folder);
int RunAndMonitor(char *);


static void *Args=NULL,*Rets=NULL;

static DIAINTR *It = NULL;


static int FolderBrowser(char *FileName) {
	char *Str=NULL;
	int ret=0,ln;
	if( (Str=kgGetVideoFile(NULL)) != NULL) {
		ln = strlen(Str);
		if(ln>1) {
			ret =1;
			strcpy(FileName,Str);
			free(Str);
		}
	}
	return ret;
}

static MODINTERFACE ModFuns[] = { 
    (MODINTERFACE) NULL 
};
static Dlink *ModuleList=NULL;

 /* Callback for  OLYinput1   */ 

int OverlayOLYinput1callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
  return ret;
}

 /* Callback for  OLYinput1browse   */ 

int OverlayOLYinput1browsecallback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  DIT *T,*TO;
  char FileName[500],OutFile[500];;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"OLYinput1");
  FileName[0]='\0';
  if(!FolderBrowser(FileName))return 0;
  kgSetString(T,0,FileName);
  kgUpdateWidget(T);
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"OLYout");
  MakeNewFileName(FileName,OutFile);
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  return ret;
}
void  OverlayOLYinput1browseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  OLYinput2   */ 

int OverlayOLYinput2callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
  return ret;
}

 /* Callback for  OLYinput2browse   */ 

int OverlayOLYinput2browsecallback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  DIT *T,*TO;
  char FileName[500],OutFile[500];;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"OLYinput2");
  FileName[0]='\0';
  if(!FolderBrowser(FileName))return 0;
  kgSetString(T,0,FileName);
  kgUpdateWidget(T);
  kgUpdateOn(Tmp);
  return ret;
}
void  OverlayOLYinput2browseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  OLYgo   */ 

int OverlayOLYgocallback( int butno,int i,void *Tmp) {
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
  DIT *T=(DIT *)kgGetNamedWidget(Tmp,(char *)"OLYinput1");
  DIT *TI=(DIT *)kgGetNamedWidget(Tmp,(char *)"OLYinput2");
  DIT *TO=(DIT *)kgGetNamedWidget(Tmp,(char *)"OLYout");
  DIT *TL=(DIT *)kgGetNamedWidget(Tmp,(char *)"OLYloc");
  char buff[500];
  DII *I= (DII *)kgGetNamedWidget(Tmp,(char *)"OLYIbox");
  sprintf (buff,"Processing Side by Side..\n");
  kgWrite(I,buff);
  int xloc,yloc;
  xloc = kgGetInt(TL,0);
  yloc = kgGetInt(TL,1);
  ret =0;
  sprintf(buff,"ffmpegfun -i %s  -i %s -filter_complex \"[0:v][1:v]overlay=%-d:%-d\" %s",
       kgGetString(T,0),kgGetString(TI,0),xloc,yloc,kgGetString(TO,0));
  kgWrite(I,buff);
//  runfunction(buff,ProcessPrint,ffmpegfun);
  RunAndMonitor(buff);
  return ret;
}
void  OverlayOLYgoinit (DIL *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  OLYout   */ 

int OverlayOLYoutcallback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
  return ret;
}

 /* Callback for  OLYOutbrowse   */ 

int OverlayOLYOutbrowsecallback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  char FileName[500];
  FileName[0]='\0';
  if(!kgFolderBrowser(Tmp,10,10,FileName,"*"))return 0;
  DIT *TO;
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"OLYout");
  kgSetString(TO,0,FileName);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  return ret;
}
void  OverlayOLYOutbrowseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  OLYloc   */ 

int OverlayOLYloccallback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
  return ret;
}
int OverlaySetup(void *Tmp,void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  /* you add any initialisation here */
  /* useful for setting is used as MakeGroup */
  return 1;
}
 
void * OverlayCleanDia(void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  
/* you add any cleaning  here */

  return NULL;
}
 
 
void *  OverlayAction(void *Tmp,void *Args) {
  return NULL;
} 
 
 
int   OverlayOn(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,1);
    else return 0;
    return 1;
  } 
  return 0;
} 
 
int   OverlayOff(void *itmp) {
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
 
 
void * OverlayInterface(void *args,void *rets) {
  /*********************************** 
   ***********************************/ 
  DIAINTR *it= (DIAINTR *)malloc(sizeof(DIAINTR));
  it->GrpId=0;
  // filled by MakeGroup  it->xsh=0;
  it->ysh=0;
  it->RunDia = RunOverlay;
  it->MakeGroup = MakeOverlayGroup;
  it->Title = GetPointer((char *)"Overlay");
  it->Help = GetPointer( (char *)"No help yet, request");
  it->Action = OverlayAction;
  it->Settings = OverlaySetup;
  it->Cleanup  = OverlayCleanDia;
  if(args != NULL) Args=args;
  if(rets != NULL) Rets=rets;
  it->args = Args;
  it->rets = Rets;
  it->SwitchOn = OverlayOn;
  it->SwitchOff = OverlayOff;
  it->Dtmp = NULL; // fiiled by MakeGroup 
  It = it;
  return it;
}
 
 
int Overlayinit(void *Tmp) {
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
int Overlaycleanup(void *Tmp) {
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
int ModifyOverlay(void *Tmp,int GrpId) {
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

int OverlayCallBack(void *Tmp,void *tmp) {
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
int OverlayResizeCallBack(void *Tmp) {
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
int OverlayWaitCallBack(void *Tmp) {
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
