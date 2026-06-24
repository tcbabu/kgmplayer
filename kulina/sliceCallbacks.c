#include <kulina.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "sliceCallbacks.h"
#include "kgutils.h"


extern MEDIAINFO Minfo;

extern int Jpipe[2];
extern int Jstat[2];
extern int MonPipe;


static char infile[500]="",outfile[500]="";

static void *Args=NULL,*Rets=NULL;

static DIAINTR *It = NULL;


static MODINTERFACE ModFuns[] = { 
    (MODINTERFACE) NULL 
};
static Dlink *ModuleList=NULL;


static void *InfoBox=NULL,*Dia=NULL;


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
int MakeFolder(char *Infile,char *Folder,char *Outfile) {
   int index,i;
   char buff[500],*pt;
   int id=0;
   index = GetBaseIndex(Infile);
   sprintf(buff,"%-s/",Folder);
   strcat(buff,Infile+index);
   i=0;
   while (buff[i]!='.') {
     if(buff[i]< ' ') break;
     if(buff[i]==' ') buff[i]='_';
     if(i>30) break;
     i++;
   }
#if 1
   pt= buff+i;
   while(1) {
     sprintf(pt,"_%-4.4d",id);
//    printf("%s\n",buff);
     if (!FileStat(buff)) break;
     id++;
   }
#else
   buff[i]='.';
   i++;
   buff[i]='\0';
   strcat(buff,ext);
#endif
   strcpy(Outfile,buff);
   return 1;
}

int MakeVideoSlices_org(char * flname,char *folder,int tslice) {

  int pid=0,id;
  int status;
  char buff[5000];
  int totsec=0;
  float  ssec,esec,crr=-0.001;
  int n;
  float per=0;
  char outfile[500];

  if( CheckMedia(flname) == 0 ) {
    kgSplashMessage(NULL,100,100,300,40,(char *)"Error: Not Video",1,0,15);
    return 0;
  }
  if( (pid =fork())!= 0) {
    kgSplashMessage(NULL,100,100,300,40,(char *)"Send for Processing",1,0,15);
    sleep(2);
    return pid;
  }
  totsec = Minfo.TotSec+0.1;
  id = getpid();
  if(pipe(Jpipe) < 0) exit(0);
  if(pipe(Jstat) < 0) exit(0);
  MonPipe = Jpipe[0];
  
  if ((pid=fork())==0) {
    fflush(stdout);
    fflush(stderr);
    close(Jpipe[0]);
    close(Jstat[1]);
     sprintf(buff,"Executing... PLEASE WAIT\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"PLEASE WAIT till the window closes\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"You can cancel job if you wish\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"slice : %d\n",tslice);
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"Esec: %d\n",tslice);
     write(Jpipe[1],buff,strlen(buff));
     ssec=0;
     esec = tslice;
     n=1;
#if 1
     while (ssec < totsec) {
       sprintf(buff,"  %-s/Frm%-5.5d\n",folder,n);
       write(Jpipe[1],buff,strlen(buff));
       per = ssec*100.0/totsec;
       sprintf(buff,"Per: %f \n",per);
       write(Jpipe[1],buff,strlen(buff));
       sprintf(outfile,"%-s/Frm%-5.5d.mp4",folder,n);
#ifdef D_X264
       if(esec < totsec) {
        sprintf(buff,"ffmpegfun -accurate_seek -ss %-f  -i \"%-s\" -ss 0 -t %-f"
           " -y -video_track_timescale 90k -f mp4 -vcodec libx264  -b:v 3000K -aq 0 "
           " -c:a aac \"%-s/Frm%-5.5d\"",
           ssec,flname,tslice+crr,folder,n);
       }
       else {
        sprintf(buff,"ffmpegfun -accurate_seek -ss %-f  -i \"%-s\" -ss 0 "
           " -y -video_track_timescale 90k -f mp4 -vcodec libx264  -b:v 3000K -aq 0 "
           " -c:a aac \"%-s/Frm%-5.5d\"",
           ssec,flname,folder,n);
       }
#else
       if(esec < totsec) {
        sprintf(buff,"ffmpegfun  -accurate_seek -ss %-f   -i \"%-s\" -ss 0   -to %-f  "
           "  -y -f mp4 -video_track_timescale 90k    -aq 0 -preset medium -crf 18 -c:v libx264  "
           " -c:a copy  \"%-s/Frm%-5.5d.mp4\"",
           ssec,flname,(float)tslice+crr,folder,n);
       }
       else {
        sprintf(buff,"ffmpegfun  -accurate_seek -ss %-f -i \"%-s\" -ss 0 "
           "  -y -f mp4 -video_track_timescale 90k  -aq 0 -preset medium -crf 18 -c:v libx264  "
           " -c:a copy  \"%-s/Frm%-5.5d.mp4\"",
           ssec,flname,folder,n);
       }
#endif
       runfunction(buff,ProcessData,ffmpegfun);
       CheckMedia(outfile);
//       ssec += tslice;
//       crr=0.00;
       sprintf(buff,"info:  %f %f %f %f\n",ssec,esec,Minfo.start,Minfo.TotSec);
       write(Jpipe[1],buff,strlen(buff));
       ssec +=tslice;
       esec = ssec + tslice;
       if(Minfo.TotSec < 1 ) break;
       n++;

     } //while
#endif
     close(Jpipe[1]);
     close(Jstat[0]);
     exit(0);
  }  //fork
  else {
     close(Jpipe[1]);
     close(Jstat[0]);
     RunMonitorJoin(NULL);
     kill(pid,9);
     waitpid(pid,&status,0);
     exit(0);
  }
}
int MakeVideoSlices(char * flname,char *folder,int tslice) {

  int pid=0,id;
  int status;
  char buff[5000];
  int totsec=0;
  float  ssec,esec,crr=-0.001;
  int n;
  float per=0;
  char outfile[500];
  char *Tfolder = MakeTmpFolder();
  char Tfile[500];
  int ofs;

  if( GetVideoInfo(flname) == 0 ) {
    kgSplashMessage(NULL,100,100,300,40,(char *)"Error: Not Video",1,0,15);
    return 0;
  }
  if( (pid =fork())!= 0) {
    kgSplashMessage(NULL,100,100,300,40,(char *)"Send for Processing",1,0,15);
    sleep(2);
    return pid;
  }
  totsec = Minfo.TotSec+0.01;
  ofs = (int)(Minfo.fps+0.5);
  id = getpid();
  if(pipe(Jpipe) < 0) exit(0);
  if(pipe(Jstat) < 0) exit(0);
  MonPipe = Jpipe[0];
  
  if ((pid=fork())==0) {
    fflush(stdout);
    fflush(stderr);
    close(Jpipe[0]);
    close(Jstat[1]);
     sprintf(buff,"Executing... PLEASE WAIT\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"PLEASE WAIT till the window closes\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"You can cancel job if you wish\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"slice : %d\n",tslice);
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"Esec: %d\n",tslice);
     write(Jpipe[1],buff,strlen(buff));
     ssec=0;
     esec = tslice;
     n=1;
     while (ssec < totsec) {
       sprintf(buff,"  %-s/Slice%-5.5d.mp4 ofs=%d\n",folder,n,ofs);
       write(Jpipe[1],buff,strlen(buff));
       per = ssec*100.0/totsec;
       sprintf(buff,"Per: %f \n",per);
       write(Jpipe[1],buff,strlen(buff));
       sprintf(outfile,"%-s/Slice%-5.5d.mp4",folder,n);
       if(esec < totsec) {
        sprintf(buff,"ffmpegfun -accurate_seek -ss %-f  -i \"%-s\" -ss 0 -t %-f"
           " -y  -f mp4 -vf  fps=%-d -vcodec libx264 -crf 18 -preset faster  -af aresample=44100 -c:a aac  "
           "  \"%-s\"",
           ssec,flname,(tslice)+crr,ofs, outfile);
       }
       else {
        sprintf(buff,"ffmpegfun -accurate_seek -ss %-f  -i \"%-s\" -ss 0 "
           " -y -f mp4 -vfx fps=%-d  -vcodec libx264  -crf 18 -preset faster  "
           " -af aresample=44100 -c:a aac \"%-s\"",
           ssec,flname,ofs,outfile);
       }
       runfunction(buff,ProcessData,ffmpegfun);
//       CheckMedia(outfile);
#if 0 
//Not Needed
         MakeFileInFolder(outfile,Tfolder,Tfile,".mp4");
         ChangeVideoFrate(outfile,Tfile,ofs);
         rename(Tfile,outfile);
#endif
         GetVideoInfo(outfile);
//       ssec += tslice;
//       crr=0.00;
       sprintf(buff,"info:  %f %f %f %f\n",ssec,esec,Minfo.start,Minfo.TotSec);
       write(Jpipe[1],buff,strlen(buff));
       esec = ssec + tslice+tslice;
       ssec +=tslice;
//       if(Minfo.TotSec < 1 ) break;
       n++;

     } //while
     kgCleanDir(Tfolder);
     close(Jpipe[1]);
     close(Jstat[0]);
     exit(0);
  }  //fork
  else {
     close(Jpipe[1]);
     close(Jstat[0]);
     RunMonitorJoin(NULL);
     kill(pid,9);
     waitpid(pid,&status,0);
     exit(0);
  }
}

 /* Callback for  SLCinput   */ 

int sliceSLCinputcallback(int cellno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"SLCinput");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"SLCout");
  strcpy(FileName,kgGetString(T,0));
#if 0
  sprintf(OutFile,"%-s/Video",getenv("HOME"));
#else
  GetFolderName(FileName,OutFile);
#endif
  MakeFolder(FileName,OutFile,OutFile);
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  strcpy(infile,FileName);
  strcpy(outfile,OutFile);
  return ret;
}

 /* Callback for  SLCinputbrowse   */ 

int sliceSLCinputbrowsecallback(int butno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"SLCinput");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"SLCout");
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
//  kgFolderBrowser(NULL,100,100,FileName,(char *)"*");
  if(!FolderBrowser(FileName))return 0;
  kgSetString(T,0,FileName);
#if 0
  sprintf(OutFile,"%-s/Video",getenv("HOME"));
#else
  GetFolderName(FileName,OutFile);
#endif
  MakeFolder(FileName,OutFile,OutFile);
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  strcpy(infile,FileName);
  strcpy(outfile,OutFile);
  
  return ret;
}
void  sliceSLCinputbrowseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  SLCgo   */ 

int sliceSLCgocallback( int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B;
  int n,ret=1;
  char buff[1000];
  int tslice=1;
  D = (DIALOG *)Tmp;
  Dia = D;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  InfoBox = kgGetNamedWidget(Tmp,(char *)"SLCIbox");
  switch(butno) {
    case 1:
       if((infile==NULL) || (infile[0]=='\0')) {
        kgWrite(InfoBox,(char *)"No Input file selected\n");
        kgUpdateOn(D);
      }
      else {
        kgWrite(InfoBox,(char *)"send for processing...\n");
        kgUpdateOn(D);
        strcpy(infile,kgGetString(kgGetNamedWidget(Dia,(char *)"SLCinput"),0));
        strcpy(outfile,kgGetString(kgGetNamedWidget(Dia,(char *)"SLCout"),0));
        if(!FileStat) mkdir(outfile,0744);
        tslice = kgGetInt(kgGetNamedWidget(Dia,(char *)"SLCsec"),0);
        MakeVideoSlices(infile,outfile,tslice);
      }
      ret = 0;
      break;
  }
  DIT *T,*TO;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"SLCinput");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"SLCout");
  kgSetString(T,0,(char *)"");
  kgSetString(TO,0,(char *)"");
  ResetGrpVis(Tmp);
  return ret;
}
void  sliceSLCgoinit (DIL *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  SLCout   */ 

int sliceSLCoutcallback(int cellno,int i,void *Tmp) {
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

 /* Callback for  SLCOutbrowse   */ 

int sliceSLCOutbrowsecallback(int butno,int i,void *Tmp) {
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
    GetFolderName(File,Folder);
    DIT *TO=(DIT *)kgGetNamedWidget(Tmp,"SLCout");
    kgSetString(TO,0,Folder);
    kgUpdateWidget(TO);
    kgUpdateOn(Tmp);
  }
  switch(butno) {
    case 1: //  Browse 
      break;
  }
  return ret;
}
void  sliceSLCOutbrowseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}

 /* Callback for  SLCsec   */ 

int sliceSLCseccallback(int cellno,int i,void *Tmp) {
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
int sliceSetup(void *Tmp,void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  /* you add any initialisation here */
  /* useful for setting is used as MakeGroup */
  return 1;
}
 
void * sliceCleanDia(void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  
/* you add any cleaning  here */

  return NULL;
}
 
 
void *  sliceAction(void *Tmp,void *Args) {
  return NULL;
} 
 
 
int   sliceOn(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,1);
    else return 0;
    return 1;
  } 
  return 0;
} 
 
int   sliceOff(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,0);
    else return 0;
    return 1;
  } 
  return 0;
} 
 
 
 
void * sliceInterface(void *args,void *rets) {
  /*********************************** 
   ***********************************/ 
  DIAINTR *it= (DIAINTR *)malloc(sizeof(DIAINTR));
  it->GrpId=0;
  // filled by MakeGroup  it->xsh=0;
  it->ysh=0;
  it->RunDia = Runslice;
  it->MakeGroup = MakesliceGroup;
  it->Title = GetPointer((char *)"slice");
  it->Help = GetPointer( (char *)"No help yet, request");
  it->Action = sliceAction;
  it->Settings = sliceSetup;
  it->Cleanup  = sliceCleanDia;
  if(args != NULL) Args=args;
  if(rets != NULL) Rets=rets;
  it->args = Args;
  it->rets = Rets;
  it->SwitchOn = sliceOn;
  it->SwitchOff = sliceOff;
  it->Dtmp = NULL; // fiiled by MakeGroup 
  It = it;
  return it;
}
 
 
int sliceinit(void *Tmp) {
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
int slicecleanup(void *Tmp) {
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
int Modifyslice(void *Tmp,int GrpId) {
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

int sliceCallBack(void *Tmp,void *tmp) {
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
int sliceResizeCallBack(void *Tmp) {
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
int sliceWaitCallBack(void *Tmp) {
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
