#include <kulina.h>
#include "SidebySideCallbacks.h"
#include "mediainfo.h"
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
int ExtractVideoInfo(char *FileName,int *xres,int *yes,float *duration);

static void *Args=NULL,*Rets=NULL;

static DIAINTR *It = NULL;


static MODINTERFACE ModFuns[] = { 
    (MODINTERFACE) NULL 
};
static Dlink *ModuleList=NULL;


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

 /* Callback for  SBSinput1   */ 

int SidebySideSBSinput1callback(int cellno,int i,void *Tmp) {
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

 /* Callback for  SBSinput1browse   */ 

int SidebySideSBSinput1browsecallback(int butno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"SBSinput1");
  FileName[0]='\0';
  if(!FolderBrowser(FileName))return 0;
  kgSetString(T,0,FileName);
  kgUpdateWidget(T);
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"SBSout");
  MakeNewFileName(FileName,OutFile);
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  return ret;
}
void  SidebySideSBSinput1browseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  SBSinput2   */ 

int SidebySideSBSinput2callback(int cellno,int i,void *Tmp) {
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

 /* Callback for  SBSinput2browse   */ 

int SidebySideSBSinput2browsecallback(int butno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"SBSinput2");
  FileName[0]='\0';
  if(!FolderBrowser(FileName))return 0;
  kgSetString(T,0,FileName);
  kgUpdateWidget(T);
  kgUpdateOn(Tmp);
  return ret;
}
void  SidebySideSBSinput2browseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  SBSgo   */ 

int SidebySideSBSgocallback( int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  int Resize=0;
  char infile1[300],infile2[30],outfile[300];
  MEDIAINFO *mt1,*mt2;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  DIT *T=(DIT *)kgGetNamedWidget(Tmp,(char *)"SBSinput1");
  DIT *TI=(DIT *)kgGetNamedWidget(Tmp,(char *)"SBSinput2");
  DIT *TO=(DIT *)kgGetNamedWidget(Tmp,(char *)"SBSout");
  DII *I= (DII *)kgGetNamedWidget(Tmp,(char *)"SBSIbox");
  int Xres1,Yres1,Xres2,Yres2,Mx,My;
  char buff[500];
  strcpy(infile1,kgGetString(T,0));
  strcpy(infile2,kgGetString(TI,0));
  strcpy(outfile,kgGetString(TO,0));
  mt1 = GetMediaInfo(infile1);
  Xres1= mt1->Axres;
  Yres1= mt1->Ayres;
  sprintf(buff,"Inside go: %d %d %d %f\n",mt1->Axres,mt1->Ayres,mt1->Video,mt1->fps);
  kgWrite(I,buff);
  if(mt1->Video==0) {free(mt1); return 0;}
  mt2 = GetMediaInfo(infile2);
  sprintf(buff,"Inside go: %d %d %d %f\n",mt2->Axres,mt2->Ayres,mt2->Video,mt2->fps);
  kgWrite(I,buff);
  if(mt2->Video==0) {free(mt2); free(mt1);return 0;}
  Xres2= mt2->Axres;
  Yres2= mt2->Ayres;
  if ( Yres2 > Yres1 ) {
     Resize=2;
     Mx = Xres1;
     My = Yres1;
  }
  if ( Yres2 < Yres1 ) {
     Resize=1;
     Mx = Xres2;
     My = Yres2;
  }
  sprintf (buff,"Processing Side by Side..\n");
  kgWrite(I,buff);
  ret =0;
  if(Resize==0) {
  sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"hstack\" %s",
       kgGetString(T,0),kgGetString(TI,0),kgGetString(TO,0));
  remove(kgGetString(TO,0));
  kgWrite(I,buff);
//  runfunction(buff,ProcessPrint,ffmpegfun);
  RunMonitorAndWait(buff);
  }
  char Tfolder[300],Vfile[300];
  MakeTmpFolderInHome(Tfolder);
  MakeFileInFolder("/tmp/Video.mp4",Tfolder,Vfile,"mp4");
  if(Resize == 1) {
    sprintf(buff,"!c01Processing %s\n",infile1);
    kgWrite(I,buff);
    kgUpdateOn(Tmp);
    ChangeVideoSizeAndFrate(infile1,Vfile,-2,My,(int)mt2->fps,1);
    sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"hstack\" %s",
       Vfile,infile2,kgGetString(TO,0));
       remove(kgGetString(TO,0));
       kgWrite(I,buff);
       RunMonitorAndWait(buff);
  }
  if(Resize == 2) {
    sprintf(buff,"!c05Processing %s\n",infile2);
    kgWrite(I,buff);
    kgUpdateOn(Tmp);
    ChangeVideoSizeAndFrate(infile2,Vfile,-2,My,(int)mt1->fps,1);
    sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"hstack\" %s",
       infile1,Vfile,kgGetString(TO,0));
    remove(kgGetString(TO,0));
    kgWrite(I,buff);
    RunMonitorAndWait(buff);
  }
  free(mt1);
  free(mt2);
  kgCleanDir(Tfolder);
  return ret;
}
void  SidebySideSBSgoinit (DIL *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  SBSout   */ 

int SidebySideSBSoutcallback(int cellno,int i,void *Tmp) {
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

 /* Callback for  SBSOutbrowse   */ 

int SidebySideSBSOutbrowsecallback(int butno,int i,void *Tmp) {
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
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"SBSout");
  kgSetString(TO,0,FileName);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  return ret;
}
void  SidebySideSBSOutbrowseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}
int SidebySideSetup(void *Tmp,void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  /* you add any initialisation here */
  /* useful for setting is used as MakeGroup */
  return 1;
}
 
void * SidebySideCleanDia(void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  
/* you add any cleaning  here */

  return NULL;
}
 
 
void *  SidebySideAction(void *Tmp,void *Args) {
  return NULL;
} 
 
 
int   SidebySideOn(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,1);
    else return 0;
    return 1;
  } 
  return 0;
} 
 
int   SidebySideOff(void *itmp) {
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
 
 
void * SidebySideInterface(void *args,void *rets) {
  /*********************************** 
   ***********************************/ 
  DIAINTR *it= (DIAINTR *)malloc(sizeof(DIAINTR));
  it->GrpId=0;
  // filled by MakeGroup  it->xsh=0;
  it->ysh=0;
  it->RunDia = RunSidebySide;
  it->MakeGroup = MakeSidebySideGroup;
  it->Title = GetPointer((char *)"SidebySide");
  it->Help = GetPointer( (char *)"No help yet, request");
  it->Action = SidebySideAction;
  it->Settings = SidebySideSetup;
  it->Cleanup  = SidebySideCleanDia;
  if(args != NULL) Args=args;
  if(rets != NULL) Rets=rets;
  it->args = Args;
  it->rets = Rets;
  it->SwitchOn = SidebySideOn;
  it->SwitchOff = SidebySideOff;
  it->Dtmp = NULL; // fiiled by MakeGroup 
  It = it;
  return it;
}
 
 
int SidebySideinit(void *Tmp) {
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
int SidebySidecleanup(void *Tmp) {
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
int ModifySidebySide(void *Tmp,int GrpId) {
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

int SidebySideCallBack(void *Tmp,void *tmp) {
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
int SidebySideResizeCallBack(void *Tmp) {
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
int SidebySideWaitCallBack(void *Tmp) {
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
