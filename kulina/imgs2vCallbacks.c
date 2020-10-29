#include <kulina.h>
#include <sys/stat.h>
#include "mediainfo.h"
#include "images2videos.h"
IMGS2VDATA is2vdata;
extern MEDIAINFO Minfo;
static DIX *IX2=NULL;
static DIT *TOUT=NULL;
extern int ScrFit;

int FileStat(char *flname);
int MakeMp4File(char *Infile,char *Outfile,int id);
int GetBaseIndex(char *s);
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
void *RunkgGetFiles(void *arg,char *Filter);

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
  L = Dopen();
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

int  imgs2vbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,j=0; 
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
  }
  return ret;
}
void  imgs2vbutton1init(DIN *B,void *pt) {
}
int  imgs2vbrowser1callback(int item,int i,void *Tmp) {
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
void  imgs2vbrowser1init(DIX *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
}
int  imgs2vtextbox1callback(int cellno,int i,void *Tmp) {
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
int  imgs2vtextbox2callback(int cellno,int i,void *Tmp) {
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
int  imgs2vbrowser2callback(int item,int i,void *Tmp) {
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
void  imgs2vbrowser2init(DIRA *R,void *pt) {
}
int  imgs2vsplbutton1callback(int butno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"OptionsWidget");
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
void  imgs2vsplbutton1init(DIL *B,void *pt) {
}
int imgs2vinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  char Output[500],Folder[500];
  D = (DIALOG *)Tmp;
  pt = D->pt;
  is2vdata.List = NULL;
  IX2 = (DIX *)kgGetNamedWidget(D,(char *)"imgs2vBrowser");
  TOUT = (DIT *)kgGetNamedWidget(D,(char *)"imgs2vVideo");
  sprintf(Output,"%-s/ ",getenv("HOME"));
  sprintf(Folder,"%-s/Video",getenv("HOME"));
  MakeFileInFolder(Output,Folder,Output,(char *)"mp4");
#if 0
  kgSetString(TOUT,0,Output);
  kgUpdateWidget(TOUT);
#endif
//  kgSetDefaultWidget(D,3);
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
