#include <kulina.h>
#include <sys/types.h>
#include <sys/stat.h>

static char infile[500]="",outfile[500]="";

int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int MakeOutputFile(char *Infile,char *Outfile,char *ext);
int GetBaseIndex(char *s);
int GetLine(int pip0,char *buff);
int SearchString(char *s1,char *s2);
int runjob(char *job,int (*ProcessOut)(int,int,int));
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int runfunctionbkgr(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int FileStat(char *flname);
int kgffmpeg(int,char **);
int ProcessSkip(int pip0,int pip1,int Pid);
int ProcessToPipe(int pip0,int pip1,int Pid);

static void *InfoBox=NULL,*Dia=NULL;

int  vtobwtextbox1callback(int cellno,int i,void *Tmp) {
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
  Dia = D;
  T = (DIT *)kgGetNamedWidget(Tmp,"v2bwInput");
  TO = (DIT *)kgGetNamedWidget(Tmp,"v2bwOutput");
  strcpy(FileName,kgGetString(T,0));
  sprintf(OutFile,"%-s/Video",getenv("HOME"));
  MakeFileInFolder(FileName,OutFile,OutFile,"mp4");
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  strcpy(infile,FileName);
  strcpy(outfile,OutFile);

  return ret;
}
int  vtobwtextbox2callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  DIT *TO;
  char FileName[500],OutFile[500];
  int ret=1;
  D = (DIALOG *)Tmp;
  Dia = D;
  T = (DIT *)kgGetNamedWidget(Tmp,"v2bwInput");
  TO = (DIT *)kgGetNamedWidget(Tmp,"v2bwOutput");
  strcpy(OutFile,kgGetString(TO,0));
  strcpy(outfile,OutFile);
  return ret;
}
int  vtobwbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  static char FileName[500],OutFile[500];
  D = (DIALOG *)Tmp;
  Dia = D;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  DIT *T,*TO;
  T = (DIT *)kgGetNamedWidget(Tmp,"v2bwInput");
  TO = (DIT *)kgGetNamedWidget(Tmp,"v2bwOutput");
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
  kgFolderBrowser(NULL,100,100,FileName,"*");
  kgSetString(T,0,FileName);
  sprintf(OutFile,"%-s/Video",getenv("HOME"));
  MakeFileInFolder(FileName,OutFile,OutFile,"mp4");
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  strcpy(infile,FileName);
  strcpy(outfile,OutFile);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  vtobwbutton1init(DIN *B,void *pt) {
}
int  vtobwsplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  char buff[1000];
  D = (DIALOG *)Tmp;
  Dia = D;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  InfoBox = kgGetNamedWidget(Tmp,"v2bwInfoBox");
  switch(butno) {
    case 1: 
       if((infile==NULL) || (infile[0]=='\0')) {
        kgWrite(InfoBox,(char *)"No Input file selected\n");
        kgUpdateOn(D);
      }
      else {
        kgWrite(InfoBox,(char *)"send for processing...\n");
        kgUpdateOn(D);
        strcpy(infile,kgGetString(kgGetNamedWidget(Dia,(char *)"v2bwInput"),0));
        strcpy(outfile,kgGetString(kgGetNamedWidget(Dia,(char *)"v2bwOutput"),0));
#ifdef D_X264
        sprintf(buff,"kgffmpeg  -i \"%-s\" -filter_complex "
                  "colorchannelmixer=.3:.4:.3:0:.3:.4:.3:0:.3:.4:.3 "
                  " -f mp4 -b:v 3000K -vcodec libx264 \"%-s\" ",
              infile,outfile);
#else
        sprintf(buff,"kgffmpeg  -i \"%-s\" -filter_complex "
                  "colorchannelmixer=.3:.4:.3:0:.3:.4:.3:0:.3:.4:.3 "
                  " -f mp4 -vcodec libx265 \"%-s\" ",
              infile,outfile);
#endif
//        printf("%s\n",buff);
        runfunctionbkgr (buff, ProcessToPipe,kgffmpeg);
      }
      ret = 0;
      break;
  }
  return ret;
}
void  vtobwsplbutton1init(DIL *B,void *pt) {
}
int vtobwinit(void *Tmp) {
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
int vtobwcleanup(void *Tmp) {
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
int vtobwCallBack(void *Tmp,void *tmp) {
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
int vtobwResizeCallBack(void *Tmp) {
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
int vtobwWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
