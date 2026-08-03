#include <kulina.h>
#include "textovervideoCallbacks.h"
#include "kgutils.h"

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

static int Vxres=400,Vyres=300;

static void *Args=NULL,*Rets=NULL;

static DIAINTR *It = NULL;


static MODINTERFACE ModFuns[] = { 
    (MODINTERFACE) NULL 
};
static Dlink *ModuleList=NULL;

static int Red=245,Green=245,Blue=230;
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
 /* Callback for  TOVinput1   */ 

int textovervideoTOVinput1callback(int cellno,int i,void *Tmp) {
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

 /* Callback for  TOVinput1browse   */ 

int textovervideoTOVinput1browsecallback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  MEDIAINFO *mpt=NULL;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  DIT *T,*TO,*TR;
  char FileName[500],OutFile[500];;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"TOVinput1");
  TR = (DIT *)kgGetNamedWidget(Tmp,(char *)"TOVtres");
  FileName[0]='\0';
  if(!FolderBrowser(FileName))return 0;
  mpt = GetMediaInfo(FileName);
  if(mpt->Video==0) {free(mpt);return ret;}
  kgSetInt(TR,0,mpt->Axres);
  kgSetInt(TR,1,mpt->Ayres);
  Vxres = mpt->Axres;
  Vyres = mpt->Ayres;
  kgUpdateWidget(TR);
  DII *I= (DII *)kgGetNamedWidget(D,(char *)"TOVIbox");
  sprintf(OutFile, "Video: Xres: %d Yres %d\n",mpt->Axres,mpt->Ayres);
  kgWrite(I,OutFile);
  sprintf(OutFile, "FPS: %f TotSecs: %f \n",mpt->fps,mpt->TotSec);
  kgWrite(I,OutFile);
  sprintf(OutFile, "Total Frames: %d\n",(int)(mpt->fps*mpt->TotSec));
  kgWrite(I,OutFile);
  free(mpt);
  kgSetString(T,0,FileName);
  kgUpdateWidget(T);
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"TOVout");
  MakeNewFileName(FileName,OutFile);
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  return ret;
}
void  textovervideoTOVinput1browseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}
 /* Callback for  TOVinput2   */ 

int textovervideoTOVinput2callback(int cellno,int i,void *Tmp) {
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

 /* Callback for  TOVinput2browse   */ 

int textovervideoTOVinput2browsecallback(int butno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"TOVinput2");
  FileName[0]='\0';
//  if(!FolderBrowser(FileName))return 0;
  if(!kgFolderBrowser(Tmp,10,10,FileName,"*"))return 0;
  kgSetString(T,0,FileName);
  kgUpdateWidget(T);
  kgUpdateOn(Tmp);
  return ret;
}
void  textovervideoTOVinput2browseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

int MakeTextOverVideo(char *infile1,char *infile2,char *outfile,DII *I) {


  int Xres1,Yres1,Xres2,Yres2,Mx,My;
  int Resize=0,ret=1;
  char Audio[300];
  char Tfolder[300],Vfile[300],Vout[300];
  MEDIAINFO *mt1,*mt2;
  char buff[500];
  MakeTmpFolderInHome(Tfolder);
  MakeFileInFolder("/tmp/Video.mp4",Tfolder,Vfile,"mp4");
  MakeFileInFolder("/tmp/Video.mp4",Tfolder,Vout,"mp4");
  MakeFileInFolder("/tmp/Audio.wav",Tfolder,Audio,"wav");
  mt1 = GetMediaInfo(infile1);
  Xres1= mt1->Axres;
  Yres1= mt1->Ayres;
  if(mt1->Video==0) {free(mt1); return 0;}
  mt2 = GetMediaInfo(infile2);
  if(mt2->Video==0) {free(mt2); free(mt1);return 0;}
  MixTwoAudios(infile1,infile2,Audio);
  Xres2= mt2->Axres;
  Yres2= mt2->Ayres;
  if ( Xres2 > Xres1 ) {
     Resize=2;
     Mx = Xres1;
     My = Yres1;
  }
  if ( Xres2 < Xres1 ) {
     Resize=1;
     Mx = Xres2;
     My = Yres2;
  }
  sprintf (buff,"Processing Top and Bottom ...\n");
  kgWrite(I,buff);
  ret =0;
  if(Resize==0) {
  sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"Vstack\" %s",
       infile1,infile2,Vout);
  remove(outfile);
//  kgWrite(I,buff);
//  runfunction(buff,ProcessPrint,ffmpegfun);
  RunMonitorAndWait(buff);
  }
  if(Resize == 1) {
    sprintf(buff,"!c01Processing %s\n",infile1);
    kgWrite(I,buff);
    ChangeVideoSizeAndFrate(infile1,Vfile,Mx,-2,(int)mt2->fps,1);
    sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"vstack\" %s",
       Vfile,infile2,Vout);
       remove(outfile);
//       kgWrite(I,buff);
       RunMonitorAndWait(buff);
  }
  if(Resize == 2) {
    sprintf(buff,"!c05Processing %s\n",infile2);
    kgWrite(I,buff);
    ChangeVideoSizeAndFrate(infile2,Vfile,Mx,-2,(int)mt1->fps,1);
    sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"vstack\" %s",
       infile1,Vfile,Vout);
    remove(outfile);
//    kgWrite(I,buff);
    RunMonitorAndWait(buff);
  }
  sprintf(buff,"!c01Processing Audio... Pl. Wait\n");
  kgWrite(I,buff);
  AudioChange(Vout,Audio,outfile);
  free(mt1);
  free(mt2);
  kgCleanDir(Tfolder);
  return 1;
}

 /* Callback for  TOVgo   */ 

int textovervideoTOVgocallback( int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=0; 
  int Type;
  int Sxres,Syres,Xoff,Yoff,Pxres,Pyres;
  MEDIAINFO *mt;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  int Fsel=1,Tpos=1;
  float Rfact=0.0,fact=1.0;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  DIT *T=(DIT *)kgGetNamedWidget(Tmp,(char *)"TOVinput1");
  DIT *TI=(DIT *)kgGetNamedWidget(Tmp,(char *)"TOVinput2");
  DIT *TO=(DIT *)kgGetNamedWidget(Tmp,(char *)"TOVout");
  DIT *TR=(DIT *)kgGetNamedWidget(Tmp,(char *)"TOVtres");
  DII *I= (DII *)kgGetNamedWidget(Tmp,(char *)"TOVIbox");
  char infile1[300],infile2[300],outfile[300],Pinfile1[300],Pinfile2[300];
  char Tfolder[30],buff[200],Opt[200];
  Type = kgGetSelection(kgGetNamedWidget(Tmp,(char *)"TOVradio"));
  Fsel = kgGetSelection(kgGetNamedWidget(Tmp,(char *)"TOVrfact"));
  Tpos = kgGetSelection(kgGetNamedWidget(Tmp,(char *)"TOVpos"));
  Rfact = (Fsel-1)*0.1;
  strcpy(infile1,kgGetString(T,0));
  mt = GetMediaInfo(infile1);
  strcpy(infile2,kgGetString(TI,0));
  strcpy(outfile,kgGetString(TO,0));
  Sxres =kgGetInt(TR,0);
  Syres =kgGetInt(TR,1);
  if(Sxres> mt->Axres){
    float fact = mt->Axres/(float)Sxres;
    Sxres = Sxres*fact;
    Syres = Syres*fact;
 }
  if(Syres> mt->Ayres){
    float fact = mt->Ayres/(float)Syres;
    Sxres = Sxres*fact;
    Syres = Syres*fact;
 }
 int dx = (mt->Axres -Sxres)/2;
 int dy = (mt->Ayres -Syres)/2;
//  Xoff =kgGetInt(TR,2);
//  Yoff =kgGetInt(TR,3);
  switch(Tpos) {
       case 1: Xoff=0;Yoff=0;break;
       case 2: Xoff=0;Yoff=dy-10;break;
       case 3: Xoff=0;Yoff=-dy+10;break;
       case 4: Xoff=-dx+10;Yoff=+dy-10;break;
       case 5: Xoff=+dx-10;Yoff=+dy-10;break;
       case 6: Xoff=-dx+10;Yoff=0;break;
       case 7: Xoff=+dx-10;Yoff=0;break;
       case 8: Xoff=-dx+10;Yoff=-dy+10;break;
       case 9: Xoff=+dx-10;Yoff=-dy+10;break;
       case 10: Xoff=0;Yoff=-dy/2;break;
       case 11: Xoff=0;Yoff=dy/2;break;
   }
#if 0
  fact = Syres/(float)Vyres;
  Pyres = 297*fact;
  fact = Sxres/(float)Vxres;
  Pxres = fact*297;   
  sprintf(Opt," -p%-d:%-d:0:0",Pxres,Pyres);
#endif
  strcpy(Opt,(char *)" ");
  if(Type==1) {
      sprintf(buff,"kgwrite -v%-s %s -s%-d:%-d:%-d:%-d -o%-s %-s",infile1,Opt,
           Sxres,Syres,Xoff,Yoff,outfile,infile2);
  }
  else {
      sprintf(buff,"kgwrite -v%-s %s  -k%-d:%-d:%-d:%-0.2f -s%-d:%-d:%-d:%-d -o%-s %-s",
          infile1,Opt,Red,Green,Blue,Rfact,
           Sxres,Syres,Xoff,Yoff,outfile,infile2);
  }
//      runfunction(buff,ProcessPrint,kgwrite);
//      ExecFunction(buff,kgwrite);
      RunFunctionAndWait(buff,kgwrite);
      kgWrite(I,buff);
  return ret;
}
 /* Callback for  TOVgo   */ 

int textovervideoTOVgocallback_org( int butno,int i,void *Tmp) {
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
  DIT *T=(DIT *)kgGetNamedWidget(Tmp,(char *)"TOVinput1");
  DIT *TI=(DIT *)kgGetNamedWidget(Tmp,(char *)"TOVinput2");
  DIT *TO=(DIT *)kgGetNamedWidget(Tmp,(char *)"TOVout");
  char buff[500];
  DII *I= (DII *)kgGetNamedWidget(Tmp,(char *)"TOVIbox");
  sprintf (buff,"Processing Side by Side..\n");
  kgWrite(I,buff);
  ret =0;
  sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"vstack\" %s",
       kgGetString(T,0),kgGetString(TI,0),kgGetString(TO,0));
  kgWrite(I,buff);
//  runfunction(buff,ProcessPrint,ffmpegfun);
  RunAndMonitor(buff);
  return ret;
}
void  textovervideoTOVgoinit (DIL *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  TOVout   */ 

int textovervideoTOVoutcallback(int cellno,int i,void *Tmp) {
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

 /* Callback for  TOVOutbrowse   */ 

int textovervideoTOVOutbrowsecallback(int butno,int i,void *Tmp) {
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
//  if(!kgFolderBrowser(Tmp,10,10,FileName,"*"))return 0;
  if(!FolderBrowser(FileName))return 0;
  DIT *TO;
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"TOVout");
  kgSetString(TO,0,FileName);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  return ret;
}
void  textovervideoTOVOutbrowseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}
int textovervideoSetup(void *Tmp,void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  /* you add any initialisation here */
  /* useful for setting is used as MakeGroup */
  void **pargs =(void **)args;
  int *ipt=NULL;
  ipt= (int *)pargs[3];
  *ipt=Vxres;
  ipt= (int *)pargs[4];
  *ipt=Vyres;
  ipt= (int *)pargs[5];
  *ipt=2;
#if 0
   DIM *M=(DIM *) kgGetNamedWidget(Tmp,(char *)"TOVmsg4");
   kgSetWidgetVisibility(M,0);
   DIN *B = (DIN *)kgGetNamedWidget(Tmp,(char *)"TOVbclr");
   kgSetWidgetVisibility(B,0);
   DIW *W = (DIW *)kgGetNamedWidget(Tmp,(char *)"TOVrfact");
   kgSetWidgetVisibility(W,0);
#endif
  return 1;
}
 
void * textovervideoCleanDia(void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  
/* you add any cleaning  here */

  return NULL;
}
 
 
void *  textovervideoAction(void *Tmp,void *Args) {
  return NULL;
} 
 
 
int   textovervideoOn(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,1);
    else return 0;
    return 1;
  } 
  return 0;
} 
 
int   textovervideoOff(void *itmp) {
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
 
 
void * textovervideoInterface(void *args,void *rets) {
  /*********************************** 
   ***********************************/ 
  DIAINTR *it= (DIAINTR *)malloc(sizeof(DIAINTR));
  it->GrpId=0;
  // filled by MakeGroup  it->xsh=0;
  it->ysh=0;
  it->RunDia = Runtextovervideo;
  it->MakeGroup = MaketextovervideoGroup;
  it->Title = GetPointer((char *)"textovervideo");
  it->Help = GetPointer( (char *)"No help yet, request");
  it->Action = textovervideoAction;
  it->Settings = textovervideoSetup;
  it->Cleanup  = textovervideoCleanDia;
  if(args != NULL) Args=args;
  if(rets != NULL) Rets=rets;
  it->args = Args;
  it->rets = Rets;
  it->SwitchOn = textovervideoOn;
  it->SwitchOff = textovervideoOff;
  it->Dtmp = NULL; // fiiled by MakeGroup 
  It = it;
  return it;
}
 
 
int textovervideoTOVradiocallback(int item,int i,void *Tmp) {
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
   DIM *M=(DIM *) kgGetNamedWidget(Tmp,(char *)"TOVmsg4");
   DIN *B = (DIN *)kgGetNamedWidget(Tmp,(char *)"TOVbclr");
   DIW *W = (DIW *)kgGetNamedWidget(Tmp,(char *)"TOVrfact");
  switch(item){
       case 1:
       kgSetWidgetVisibility(M,0);
       kgSetWidgetVisibility(B,0);
       kgSetWidgetVisibility(W,0);
       break;
       case 2:
       kgSetWidgetVisibility(M,1);
       kgSetWidgetVisibility(B,1);
       kgSetWidgetVisibility(W,1);
       break;
  }
  kgUpdateWidget(M);
  kgUpdateWidget(B);
  kgUpdateWidget(W);
  kgUpdateOn(Tmp);

  return ret;
}
void  textovervideoTOVradioinit (DIRA *R,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
}
int textovervideoTOVtrescallback(int cellno,int i,void *Tmp) {
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
int textovervideoTOVbclrcallback(int butno,int i,void *Tmp) {
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
  kgGetColor(Tmp,10,10,&Red,&Green,&Blue);
  kgChangeButtonColor(B,0,Red,Green,Blue);
  kgUpdateWidget(B);
  kgUpdateOn(Tmp);
  switch(butno) {
    case 1: //   
      break;
  }
  return ret;
}
void  textovervideoTOVbclrinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
 kgChangeButtonColor(B,0,Red,Green,Blue);
}
int textovervideoTOVrfactcallback(int item ,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIW *B; 
  int ret=1; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  B = (DIW *) kgGetWidget(Tmp,i);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
int textovervideoTOVposcallback(int item ,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIW *B; 
  int ret=1; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  D = (DIALOG *)Tmp;
  B = (DIW *) kgGetWidget(Tmp,i);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
int textovervideoinit(void *Tmp) {
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
int textovervideocleanup(void *Tmp) {
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
int Modifytextovervideo(void *Tmp,int GrpId) {
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

int textovervideoCallBack(void *Tmp,void *tmp) {
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
int textovervideoResizeCallBack(void *Tmp) {
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
int textovervideoWaitCallBack(void *Tmp) {
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
