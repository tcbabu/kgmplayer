#include <kulina.h>

#include "ConvertData.h"
int MakeOutputFile(char *Infile,char *Outfile,char *ext);
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int MakeNewFileName(char *Infile,char *OutFile);
int GetFolderName(char *infile,char *folder);
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
void *RunAudioEnhance(void *);
static int FolderBrowser(char *FileName) {
	char *Str=NULL;
	int ret=0,ln;
	if( (Str=kgGetAudioFile(NULL)) != NULL) {
		ln = strlen(Str);
		if(ln>1) {
			ret =1;
			strcpy(FileName,Str);
			free(Str);
		}
	}
	return ret;
}
 /* Callback for  EVinput   */ 

int EnhanceAudiotextbox1callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T,*TO;T_ELMT *e; 
  int ret=1;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  char FileName[500],OutFile[500];
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"EVoutput");
  strcpy(FileName,kgGetString(T,0));
  strcpy(OutFile,kgGetString(TO,0));
      GetFolderName(FileName,OutFile);
//      MakeNewFileName(FileName,OutFile);
      MakeFileInFolder(FileName,OutFile,OutFile,"wav");
      kgSetString(TO,0,OutFile);
      kgUpdateWidget(TO);
 // sprintf(OutFile,"%-s/Music",getenv("HOME"));
//  MakeFileInFolder(FileName,OutFile,OutFile,(char *)"mp3");
//  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
//  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  return ret;
}

 /* Callback for  EVbrowse   */ 

int EnhanceAudiobutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  char FileName[500],OutFile[500];
  int n,ret =0; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  DIT *T,*TO;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"EVinput");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"EVoutput");
  n = B->nx*B->ny;
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
  strcpy(OutFile,kgGetString(TO,0));
//  kgFolderBrowser(NULL,100,100,FileName,"*");
  if(!FolderBrowser(FileName))return 0;
  kgSetString(T,0,FileName);
#if 0
  sprintf(OutFile,"%-s/Music/",getenv("HOME"));
  MakeOutputFile(FileName,OutFile+strlen(OutFile),"mp3");
#else
      GetFolderName(FileName,OutFile);
      MakeFileInFolder(FileName,OutFile,OutFile,"wav");
      kgSetString(TO,0,OutFile);
      kgUpdateWidget(TO);
#endif
//  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
//  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  switch(butno) {
    case 1: //  Browse 
      break;
  }
  return ret;
}
void  EnhanceAudiobutton1init(DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  EVoutput   */ 

int EnhanceAudiotextbox2callback(int cellno,int i,void *Tmp) {
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

 /* Callback for  EVdo   */ 

int EnhanceAudiosplbutton1callback( int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=0,j; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  char buff[500],outfile[200];
  double corval=0.0,meanlevel;
  VOLSTR *vstr=NULL;
  DIRA *ToolsBox;
  void *Thds;
  void *Busyid;
  DIT *TI,*TO;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  ToolsBox = (DIRA *)kgGetNamedWidget(Tmp,"ToolsBox");
  Busyid = kgOpenBusy(Tmp,400,400);
  vstr = (VOLSTR *)malloc(sizeof(VOLSTR));
  TI = (DIT *)kgGetNamedWidget(Tmp,(char *)"EVinput");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"EVoutput");
  
  strcpy(vstr->Infile,kgGetString(TI,0));
  strcpy(vstr->Outfile,kgGetString(TO,0));
  RunAudioEnhance(vstr);
  kgCloseBusy(Busyid);
  switch(butno) {
    case 1: //  !c01Enhance 
      break;
  }
  return ret;
}
void  EnhanceAudiosplbutton1init(DIL *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}
int EnhanceAudioinit(void *Tmp) {
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
int EnhanceAudiocleanup(void *Tmp) {
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
int ModifyEnhanceAudio(void *Tmp,int GrpId) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
 /* pt[0] is inputs given by caller */
  DIA *d;
  int i,n;
  d = D->d;
  i=0;while(d[i].t!= NULL) {;
     i++;
  };
  n=1;
//  strcpy(D->name,"Kulina Designer ver 2.0");    /*  Dialog name you may change */
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

int EnhanceAudioCallBack(void *Tmp,void *tmp) {
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
int EnhanceAudioResizeCallBack(void *Tmp) {
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
int EnhanceAudioWaitCallBack(void *Tmp) {
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
