#include <kulina.h>
#include "mediainfo.h"
#include "VideoReverseCallbacks.h"

static void *Args=NULL,*Rets=NULL;

static DIAINTR *It = NULL;


static MODINTERFACE ModFuns[] = { 
    (MODINTERFACE) NULL 
};
static Dlink *ModuleList=NULL;

 /* Callback for  VREVEVinput   */ 
extern MEDIAINFO Minfo;
static int Pway = 0;

#include "ConvertData.h"
int MakeOutputFile(char *Infile,char *Outfile,char *ext);
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int MakeNewFileName(char *Infile,char *OutFile);
int GetFolderName(char *infile,char *folder);
MEDIAINFO * GetMediaInfo(char *flname);
int RunMonitorAndWait(char *);

typedef struct _volstr {
	char Infile[200];
	char Outfile[200];
	int  MeanDb;
	double corval;
	double meanVol;
	double histVol;
	double maxVol;
	double duration;
} VOLSTR;
void *RunAudioExtraTool(void *);
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


int VideoReverseVREVinputcallback(int cellno,int i,void *Tmp) {
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
  char FileName[300],OutFile[300];
  DIT *TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"VREVoutput");
  strcpy(FileName,kgGetString(T,0));
  strcpy(OutFile,kgGetString(TO,0));
  GetFolderName(FileName,OutFile);
  MakeFileInFolder(FileName,OutFile,OutFile,"mp4");
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(TO);
  kgUpdateWidget(T);
  kgUpdateOn(Tmp);
  return ret;
}

 /* Callback for  VREVIbrowse   */ 

int VideoReverseVREVIbrowsecallback(int butno,int i,void *Tmp) {
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
  char FileName[300],OutFile[300];
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"VREVinput");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"VREVoutput");
  n = B->nx*B->ny;
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
  strcpy(OutFile,kgGetString(TO,0));
  if(!FolderBrowser(FileName))return 0;
  kgSetString(T,0,FileName);
  GetFolderName(FileName,OutFile);
  MakeFileInFolder(FileName,OutFile,OutFile,"mp4");
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(TO);
  kgUpdateWidget(T);
  kgUpdateOn(Tmp);
  return ret;
}
void  VideoReverseVREVIbrowseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  VREVoutput   */ 

int VideoReverseVREVoutputcallback(int cellno,int i,void *Tmp) {
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


 /* Callback for  VREVdo   */ 

int VideoReverseVREVdocallback( int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=0; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
  float Cfac=1.0;
  int fs;
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  DIT *T=(DIT *)kgGetNamedWidget(Tmp,(char *)"VREVinput");
  DIT *TO=(DIT *)kgGetNamedWidget(Tmp,(char *)"VREVoutput");
  DII *I  = (DII *)kgGetNamedWidget(Tmp,(char *)"VREVinfo");
  MEDIAINFO *Mi= GetMediaInfo(kgGetString(T,0));
  fs = (int)(Mi->fps+0.5);
  free(Mi);

  char buff[500];
  ret =0;
  if(Pway) {
    // Professional
       char OutFile[500],Intr1[500],Intr2[500],FileName[500];;
//       strcpy(Intr2,kgGetString(T,0));
       strcpy(FileName,kgGetString(T,0));
       strcpy(OutFile,kgGetString(TO,0));
       GetFolderName(FileName,OutFile);
       MakeFileInFolder(FileName,OutFile,Intr1,"avi");
       MakeFileInFolder(FileName,Intr1,Intr2,"avi");
       kgWrite(I,Intr1);
       kgWrite(I,Intr2);
       sprintf(buff,"Professional Way:  %d (frames)\n", fs);
       kgWrite(I,buff);
       sprintf(buff,"ffmpegfun -y -i %s  -c:v utvideo"
          " -an  %s" ,kgGetString(T,0),Intr1);
       kgWrite(I,buff);
       RunMonitorAndWait(buff);
       sprintf(buff,"ffmpegfun -y -i %s  -vf reverse -an "
          " %s" ,Intr1,Intr2);
       kgWrite(I,buff);
       RunMonitorAndWait(buff);
       sprintf(buff,"ffmpegfun -y -i %s -c:v libx265  -an  "
          " %s" ,Intr2,kgGetString(TO,0));
       kgWrite(I,buff);
  }
  else {
       sprintf(buff,"Quick Way (needs lot of resources; try for small files)  \n");
       kgWrite(I,buff);
       sprintf(buff,"ffmpegfun -y -i %s   -vf  reverse -an   %s",
       kgGetString(T,0),kgGetString(TO,0));
       kgWrite(I,buff);
  }
  RunMonitorAndWait(buff);
  sprintf(buff,"PROCESSED JOB...JOB may fail due to resorce crunch\n");
  kgWrite(I,buff);
  sprintf(buff,"!c01 Probably it is better to make small slices of VIDEO\n");
  kgWrite(I,buff);
  sprintf(buff,"!c01 AUDIO is ignored, processes it separately and mix\n");
  kgWrite(I,buff);
  return ret;
}
void  VideoReverseVREVdoinit (DIL *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  VREVObrowse   */ 

int VideoReverseVREVObrowsecallback(int butno,int i,void *Tmp) {
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
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"VREVoutput");
  kgSetString(TO,0,FileName);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  return ret;
}
void  VideoReverseVREVObrowseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}
int VideoReverseSetup(void *Tmp,void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  /* you add any initialisation here */
  /* useful for setting is used as MakeGroup */
  void **pt = (void **)args;
  return 1;
}
 
void * VideoReverseCleanDia(void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  
/* you add any cleaning  here */

  return NULL;
}
 
 
void *  VideoReverseAction(void *Tmp,void *Args) {
  return NULL;
} 
 
 
int   VideoReverseOn(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,1);
    else return 0;
    return 1;
  } 
  return 0;
} 
 
int   VideoReverseOff(void *itmp) {
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
 
 
void * VideoReverseInterface(void *args,void *rets) {
  /*********************************** 
   ***********************************/ 
  DIAINTR *it= (DIAINTR *)malloc(sizeof(DIAINTR));
  it->GrpId=0;
  // filled by MakeGroup  it->xsh=0;
  it->ysh=0;
  it->RunDia = RunVideoReverse;
  it->MakeGroup = MakeVideoReverseGroup;
  it->Title = GetPointer((char *)"VideoReverse");
  it->Help = GetPointer( (char *)"No help yet, request");
  it->Action = VideoReverseAction;
  it->Settings = VideoReverseSetup;
  it->Cleanup  = VideoReverseCleanDia;
  if(args != NULL) Args=args;
  if(rets != NULL) Rets=rets;
  it->args = Args;
  it->rets = Rets;
  it->SwitchOn = VideoReverseOn;
  it->SwitchOff = VideoReverseOff;
  it->Dtmp = NULL; // fiiled by MakeGroup 
  It = it;
  return it;
}
 
 
int VideoReverseVREVradiocallback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIRA *R;DIALOG *D; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  ThumbNail **th; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  R = (DIRA *)kgGetWidget(Tmp,i);
  th = (ThumbNail **) R->list;
  Pway = item -1;
  return ret;
}
void  VideoReverseVREVradioinit (DIRA *R,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
}
int VideoReverseinit(void *Tmp) {
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
int VideoReversecleanup(void *Tmp) {
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
int ModifyVideoReverse(void *Tmp,int GrpId) {
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

int VideoReverseCallBack(void *Tmp,void *tmp) {
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
int VideoReverseResizeCallBack(void *Tmp) {
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
int VideoReverseWaitCallBack(void *Tmp) {
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
