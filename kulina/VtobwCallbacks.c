#include <kulina.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "VtobwCallbacks.h"
#include "kgutils.h"


extern MEDIAINFO Minfo;

extern int Jpipe[2];
extern int Jstat[2];
extern int MonPipe;


//static char infile[500]="",outfile[500]="";

static void *Args=NULL,*Rets=NULL;

static DIAINTR *It = NULL;


static MODINTERFACE ModFuns[] = { 
    (MODINTERFACE) NULL 
};
static Dlink *ModuleList=NULL;




static char *GetPointer(char *str) { 
  char *pt; 
  pt = (char *)malloc(strlen(str)+1); 
  strcpy(pt,str); 
  return pt; 
} 

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


 /* Callback for  VBWinput   */ 

int VtobwVBWinputcallback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  DIT *TO;
  int ret=1;
  static char FileName[500],OutFile[500];
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"VBWinput");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"VBWout");
  strcpy(FileName,kgGetString(T,0));
  MakeNewFileName(FileName,OutFile);
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  return ret;
}

 /* Callback for  VBWinputbrowse   */ 

int VtobwVBWinputbrowsecallback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B;
  int n,ret =0;
  static char FileName[500],OutFile[500];
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  DIT *T,*TO;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"VBWinput");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"VBWout");
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
  if(! kgFolderBrowser(Tmp,20,20,FileName,(char *)"*") ) return 0;;
//  if(!FolderBrowser(FileName))return 0;
  kgSetString(T,0,FileName);
  MakeNewFileName(FileName,OutFile);
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  
  return ret;
}
void  VtobwVBWinputbrowseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  VBWgo   */ 

int VtobwVBWgocallback( int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B;
  int n,ret=1;
  char buff[1000];
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  DII *InfoBox = (DII *)kgGetNamedWidget(Tmp,(char *)"VBWIbox");
  char infile[300],outfile[300];
  DIT *T = (DIT *)kgGetNamedWidget(Tmp,(char *)"VBWinput");
  DIT *TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"VBWout");
  infile[0]='\0';
  strcpy(infile,kgGetString(T,0));
  strcpy(outfile,kgGetString(TO,0));
  switch(butno) {
    case 1:
       if((infile==NULL) || (infile[0]=='\0')) {
        kgWrite(InfoBox,(char *)"No Input file selected\n");
        kgUpdateOn(D);
      }
      else {
        kgWrite(InfoBox,(char *)"send for processing...\n");
        kgUpdateOn(D);
#ifdef D_X264
        sprintf(buff,"ffmpegfun  -i \"%-s\" -vf hue=s=0  "
                  " -c:a copy -vcodec libx264 \"%-s\" ",
              infile,outfile);
#else
        sprintf(buff,"ffmpegfun  -i \"%-s\" -vf hue=s=0  "
                  " -c:a copy -vcodec libx265 \"%-s\" ",
              infile,outfile);
#endif
//        printf("%s\n",buff);
        kgWrite(InfoBox,buff);
        kgUpdateOn(D);
//        runfunctionbkgr (buff, ProcessToPipe,ffmpegfun);
        RunMonitorAndWait(buff);
      }
      ret = 0;
      break;
  }
#if 0
  kgSetString(T,0,(char *)"");
  kgSetString(TO,0,(char *)"");
  ResetGrpVis(Tmp);
#endif
  return ret;
}
void  VtobwVBWgoinit (DIL *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  VBWout   */ 

int VtobwVBWoutcallback(int cellno,int i,void *Tmp) {
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

 /* Callback for  VBWOutbrowse   */ 

int VtobwVBWOutbrowsecallback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  char File[200],Folder[200];
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  if(kgFolderBrowser(Tmp,200,200,File,(char *)"*")) {
    DIT *TO=(DIT *)kgGetNamedWidget(Tmp,"VBWout");
    kgSetString(TO,0,File);
    kgUpdateWidget(TO);
    kgUpdateOn(Tmp);
  }
  switch(butno) {
    case 1: //  Browse 
      break;
  }
  return ret;
}
void  VtobwVBWOutbrowseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  VBWsec   */ 

int VtobwVBWseccallback(int cellno,int i,void *Tmp) {
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
int VtobwSetup(void *Tmp,void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  /* you add any initialisation here */
  /* useful for setting is used as MakeGroup */
  return 1;
}
 
void * VtobwCleanDia(void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  
/* you add any cleaning  here */

  return NULL;
}
 
 
void *  VtobwAction(void *Tmp,void *Args) {
  return NULL;
} 
 
 
int   VtobwOn(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,1);
    else return 0;
    return 1;
  } 
  return 0;
} 
 
int   VtobwOff(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,0);
    else return 0;
    return 1;
  } 
  return 0;
} 
 
 
 
void * VtobwInterface(void *args,void *rets) {
  /*********************************** 
   ***********************************/ 
  DIAINTR *it= (DIAINTR *)malloc(sizeof(DIAINTR));
  it->GrpId=0;
  // filled by MakeGroup  it->xsh=0;
  it->ysh=0;
  it->RunDia = RunVtobw;
  it->MakeGroup = MakeVtobwGroup;
  it->Title = GetPointer((char *)"Vtobw");
  it->Help = GetPointer( (char *)"No help yet, request");
  it->Action = VtobwAction;
  it->Settings = VtobwSetup;
  it->Cleanup  = VtobwCleanDia;
  if(args != NULL) Args=args;
  if(rets != NULL) Rets=rets;
  it->args = Args;
  it->rets = Rets;
  it->SwitchOn = VtobwOn;
  it->SwitchOff = VtobwOff;
  it->Dtmp = NULL; // fiiled by MakeGroup 
  It = it;
  return it;
}
 
 
int Vtobwinit(void *Tmp) {
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
int Vtobwcleanup(void *Tmp) {
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
int ModifyVtobw(void *Tmp,int GrpId) {
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

int VtobwCallBack(void *Tmp,void *tmp) {
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
int VtobwResizeCallBack(void *Tmp) {
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
int VtobwWaitCallBack(void *Tmp) {
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
