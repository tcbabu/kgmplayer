#include <kulina.h>
#include <sys/stat.h>
#include "imgs2vCallbacks.h"
#include "mediainfo.h"
#include "images2videos.h"
IMGS2VDATA is2vdata;
extern MEDIAINFO Minfo;
static DIX *IX2=NULL;
static DIT *TOUT=NULL;
int ScrFit=1;

int ResetGrpVis(void *);
int FileStat(char *flname);
int MakeMp4File(char *Infile,char *Outfile,int id);
int GetBaseIndex(char *s);
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
void *RunkgGetFiles(void *arg,char *Filter);
void *RunReorderImages(void *arg);
static void *Args=NULL,*Rets=NULL;

static DIAINTR *It = NULL;


static MODINTERFACE ModFuns[] = { 
    (MODINTERFACE) NULL 
};
static Dlink *ModuleList=NULL;

ThumbNail **AddItemtoIlist(char *newitem) {
  char buff[500];
  MEDIAINFO *pt;
  FILE *fp;
  Dlink *L;
  int ln,i,Cond=0,index;
  char **menu;
  char *file;
  L = (Dlink *) is2vdata.List;
  if(L==NULL) L = Dopen();
  is2vdata.List= L;
  pt = (MEDIAINFO *)malloc(sizeof(MEDIAINFO));
  *pt = Minfo;
  strcpy(pt->Flname,newitem);

  Dappend(L,(void *)pt);
  ln = Dcount(L);
  menu = (char **)malloc(sizeof(char *)*(ln+1));
  menu[ln]=NULL;
  Resetlink(L);
  i=0;
  while( (pt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    index= GetBaseIndex(pt->Flname);
    menu[i]=(char *)malloc(strlen(pt->Flname+index)+6);
    strcpy(menu[i],pt->Flname+index);
    kgTruncateString(menu[i],50);
//    printf("%s\n",menu[i]);
    i++;
  }
  return kgStringToThumbNails(menu);
}
ThumbNail **DeleteItemsfromIlist(void) {
  char buff[500];
  MEDIAINFO *pt;
  FILE *fp;
  Dlink *L;
  int ln,i,index;
  char **menu;
  char *file;
//  L = Dopen();
  L = (Dlink *) is2vdata.List;
  if(L==NULL) L = Dopen();
  is2vdata.List = L;
  ln = Dcount(L);
  Resetlink(L);
  i=-1;
  while( (pt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    i++;
    if( kgGetSwitch(IX2,i)) {
      Dmove_back(L,1);
      Ddelete(L);
      if(i >=(ln-1)) break;
    }
  }
  Resetlink(L);
  ln = Dcount(L);
  menu = (char **)malloc(sizeof(char *)*(ln+1));
  menu[ln]=NULL;
  Resetlink(L);
  i=0;
  while( (pt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    index= GetBaseIndex(pt->Flname);
    menu[i]=(char *)malloc(strlen(pt->Flname+index)+6);
    strcpy(menu[i],pt->Flname+index);
    kgTruncateString(menu[i],50);
    i++;
  }
  return kgStringToThumbNails(menu);
}

int imgs2vimgs2vWidget2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,j=0; 
  char **names=NULL;
  static char filename[500]="";
  ThumbNail **th;
  char **Plist=NULL;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
#if 0
  if(IX2== NULL) {
    IX2 = kgGetNamedWidget(D,(char *)"imgs2vBrowser");
    TOUT = kgGetNamedWidget(D,(char *)"imgs2vVideo");
  }
#else
    IX2 = (DIX *)kgGetNamedWidget(D,(char *)"imgs2vBrowser");
    TOUT = (DIT *)kgGetNamedWidget(D,(char *)"imgs2vVideo");
#endif
  n = B->nx*B->ny;
  if(filename[0]=='\0') {
    strcpy(filename,getenv("HOME"));
    strcat(filename,"/ ");
  }
  switch(butno) {
    case 1: 
      Plist = (char **) RunkgGetFiles(NULL,(char *)"*.png *.jpg *.jpeg *.tiff *.PNG *.JPG *.JPEG");
      if(Plist != NULL) {
        j=0;
        while(Plist[j]!= NULL) {
          th = AddItemtoIlist(Plist[j]);
          j++;
        }
        kgFreeDouble((void **)Plist);
        Plist=NULL;
        if(j > 0) {
          kgFreeThumbNails((ThumbNail **)kgGetList(IX2));
          kgSetList(IX2,(void **)th);
          kgUpdateWidget(IX2);
          kgUpdateOn(Tmp);
        }
      }
      break;
    case 2: 
      th = (ThumbNail **) kgGetList(IX2);
      if(th==NULL) break;
      if(th[0]==NULL) break;
      th = DeleteItemsfromIlist();
      kgFreeThumbNails((ThumbNail **)kgGetList(IX2));
      kgSetList(IX2,(void **)th);
      kgUpdateWidget(IX2);
      kgUpdateOn(Tmp);

      break;
    case 3: 
      th = (ThumbNail **) kgGetList(IX2);
      if(th==NULL) break;
      if(th[0]==NULL) break;
      Plist = (char **)RunReorderImages(is2vdata.List);
      Dempty(is2vdata.List);
      is2vdata.List=NULL;
      th = NULL;
      if(Plist != NULL) {
        j=0;
        while(Plist[j]!= NULL) {
          th = AddItemtoIlist(Plist[j]);
          j++;
        }
        kgFreeDouble((void **)Plist);
        Plist=NULL;
        kgFreeThumbNails((ThumbNail **)kgGetList(IX2));
        kgSetList(IX2,(void **)th);
        kgUpdateWidget(IX2);
        kgUpdateOn(Tmp);
      }
      else {
      }
      break;
    default:
      break;  
  }
  return ret;
}
void  imgs2vimgs2vWidget2init (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}
 /* Callback for  imgs2vBrowser   */ 

int imgs2vimgs2vBrowsercallback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIX *X;void *pt; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  X = (DIX *)kgGetWidget(Tmp,i);
  IX2 = X;
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  imgs2vimgs2vBrowserinit (DIX *X,void *ptmp) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
 void **pt=(void **)ptmp; //pt[0] is arg 
}
 /* Callback for  imgs2vVideo   */ 

int imgs2vimgs2vVideocallback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  strcpy(is2vdata.Outfile,kgGetString(T,0));
  e = T->elmt;
  return ret;
}
int GetRadioValue(void *Tmp) {
  int ret=0,i;
  ThumbNail **th; 
  DIRA *R;
  R = (DIRA *)kgGetNamedWidget(Tmp,(char *)"FitWidget");
  th = (ThumbNail **) R->list;
  i=0;
  while(th[i]!= NULL) {
    if(th[i]->sw == 1)  ScrFit=i+1;
    i++;
  }
  
  return ScrFit;
}

 /* Callback for  Options   */ 

int imgs2vOptionscallback(int cellno,int i,void *Tmp) {
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
 /* Callback for  FitWidget   */ 

int imgs2vFitWidgetcallback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIRA *R;DIALOG *D;void *pt; 
  ThumbNail **th; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  R = (DIRA *)kgGetWidget(Tmp,i);
  th = (ThumbNail **) R->list;
  ScrFit=item;
  return ret;
}
void  imgs2vFitWidgetinit (DIRA *R,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
}

 /* Callback for  imgs2vWidget9   */ 

int imgs2vimgs2vWidget9callback( int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  DIT *T=NULL;
  void *id;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"Options");
  n = B->nx;
  IX2 = (DIX *)kgGetNamedWidget(D,(char *)"imgs2vBrowser");
  TOUT = (DIT *)kgGetNamedWidget(D,(char *)"imgs2vVideo");
  switch(butno) {
    case 1: 
      strcpy(is2vdata.Outfile,kgGetString(TOUT,0));     
      is2vdata.Xsize = kgGetInt(T,0);
      is2vdata.Ysize = kgGetInt(T,1);
      is2vdata.imagetime = kgGetDouble(T,2);
      GetRadioValue(Tmp);
      is2vdata.fittoscrn = ScrFit%2;;
      Imgs2Videos(&is2vdata);
      kgFreeThumbNails((ThumbNail **)kgGetList(IX2));
      kgSetList(IX2,NULL);
      ResetGrpVis(Tmp);

      ret=0;

      break;
    case 2: 
      kgFreeThumbNails((ThumbNail **)kgGetList(IX2));
      kgSetList(IX2,NULL);
      kgUpdateWidget(IX2);
      kgUpdateOn(Tmp);
      if( is2vdata.List== NULL) Dempty((Dlink *)is2vdata.List);
      is2vdata.List= NULL;
      ret=0;
      break;
  }
  return ret;
}
void  imgs2vimgs2vWidget9init (DIL *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}
 
int imgs2vObrowsecallback(int butno,int i,void *Tmp) {
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
  switch(butno) {
    case 1: //  Browse 
       char Flname[300];
       Flname[0]='\0';
       if(kgFolderBrowser(Tmp,2,2,Flname,"*") ) {
         DIT *TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"imgs2vVideo");
         kgSetString (TO,0,Flname);
         kgUpdateWidget(TO);
         kgUpdateOn(Tmp);
       }
      break;
  }
  return ret;
}
void  imgs2vObrowseinit (DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
// may use kgChangeButtonNormalImage etc...
 BUT_STR *buts;
 buts = (BUT_STR *) (B->buts);
}
int imgs2vSetup(void *Tmp,void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  /* you add any initialisation here */
  /* useful for setting is used as MakeGroup */
  return 1;
}
 
void * imgs2vCleanDia(void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  
/* you add any cleaning  here */

  return NULL;
}
 
 
void *  imgs2vAction(void *Tmp,void *Args) {
  return NULL;
} 
 
 
int   imgs2vOn(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,1);
    else return 0;
    return 1;
  } 
  return 0;
} 
 
int   imgs2vOff(void *itmp) {
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
 
 
void * imgs2vInterface(void *args,void *rets) {
  /*********************************** 
   ***********************************/ 
  DIAINTR *it= (DIAINTR *)malloc(sizeof(DIAINTR));
  it->GrpId=0;
  // filled by MakeGroup  it->xsh=0;
  it->ysh=0;
  it->RunDia = Runimgs2v;
  it->MakeGroup = Makeimgs2vGroup;
  it->Title = GetPointer((char *)"imgs2v");
  it->Help = GetPointer( (char *)"No help yet, request");
  it->Action = imgs2vAction;
  it->Settings = imgs2vSetup;
  it->Cleanup  = imgs2vCleanDia;
  if(args != NULL) Args=args;
  if(rets != NULL) Rets=rets;
  it->args = Args;
  it->rets = Rets;
  it->SwitchOn = imgs2vOn;
  it->SwitchOff = imgs2vOff;
  it->Dtmp = NULL; // fiiled by MakeGroup 
  It = it;
  return it;
}
 
 
int imgs2vinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  char Folder[500],Infile[200];
  D = (DIALOG *)Tmp;
  pt = D->pt;
  IX2 = (DIX *)kgGetNamedWidget(D,(char *)"imgs2vBrowser");
  DIT *TO = (DIT *)kgGetNamedWidget(D,(char *)"imgs2vVideo");
  sprintf(Infile,"%-s/Images.mp4",getenv("HOME"));
  sprintf(Folder,"%-s",getenv("HOME"));
  MakeFileInFolder(Infile,Folder,Folder,(char *)"mp4");
#if 0
  kgSetString(TO,0,Folder);
  kgUpdateWidget(TO);
#endif
//  kgSetDefaultWidget(D,3);
  is2vdata.List = NULL;
  return ret;
}
int imgs2vcleanup(void *Tmp) {
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
int Modifyimgs2v(void *Tmp,int GrpId) {
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
  char Infile[500],Folder[500];
  DIT *TO = (DIT *)kgGetNamedWidget(D,(char *)"imgs2vVideo");
  sprintf(Infile,"%-s/Images.mp4",getenv("HOME"));
  sprintf(Folder,"%-s",getenv("HOME"));
  MakeFileInFolder(Infile,Folder,Folder,(char *)"mp4");
#if 1
  kgSetString(TO,0,Folder);
  kgUpdateWidget(TO);
#endif
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
int imgs2vCallBack(void *Tmp,void *tmp) {
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
int imgs2vResizeCallBack(void *Tmp) {
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
int imgs2vWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
