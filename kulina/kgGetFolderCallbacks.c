#include <kulina.h>
char *GetExtension( char *flname) {
  char *ext=NULL;
  int i,ln;
  ln = strlen(flname);
  i=ln-1;
  while( (i>=0) &&(flname[i]!='.') )i--;
  if(i>=0) ext= flname+i+1;
  return ext;
}
char *RemoveExtension( char *flname) {
  char *ext=NULL;
  int i,ln;
  ln = strlen(flname);
  i=ln-1;
  while( (i>=0) &&(flname[i]!='.') )i--;
  if(i>=0) {
    ext= flname+i+1;
    flname[i]='\0';
  }
  return ext;
}
int CheckMedia_d(char *flname) {
  char *ext;
  ext = GetExtension(flname);
  if(ext==NULL) return 0;
  if(strcmp(ext,"tar") == 0 ) return 0;
  if(strcmp(ext,"tgz") == 0 ) return 0;
  if(strcmp(ext,"bz2") == 0 ) return 0;
  if(strcmp(ext,"tbz") == 0 ) return 0;
  if(strcmp(ext,"gz") == 0 ) return 0;
  if(strcmp(ext,"zip") == 0 ) return 0;
  if(strcmp(ext,"c") == 0 ) return 0;
  if(strcmp(ext,"cpp")== 0  ) return 0;
  if(strcmp(ext,"o") == 0 ) return 0;
  if(strcmp(ext,"f") == 0 ) return 0;
  if(strcmp(ext,"ftn") == 0 ) return 0;
  if(strcmp(ext,"mp4") == 0 ) return 1;
  if(strcmp(ext,"mp3") == 0 ) return 1;
  if(strcmp(ext,"MP3") == 0 ) return 1;
  if(strcmp(ext,"MP4") == 0 ) return 1;
  if(strcmp(ext,"WAV") == 0 ) return 1;
  if(strcmp(ext,"wav") == 0 ) return 1;
  if(strcmp(ext,"avi") == 0 ) return 1;
  if(strcmp(ext,"AVI") == 0 ) return 1;
  if(strcmp(ext,"webm")== 0 )  return 1;
  if(strcmp(ext,"flv") == 0 ) return 1;
  if(strcmp(ext,"ogg") == 0 ) return 1;
  if(strcmp(ext,"mkv") == 0 ) return 1;
  if(strcmp(ext,"MKV") == 0 ) return 1;
  
  return 0;
}
char **ProcessFileMenu(char *Folder) {
   char *pt,**menu;
   int i,count,l,LEN=40;
   Dlink *L;
   menu = kgFileMenu(Folder,"*");
   if(menu!=NULL) {
     L = Dopen();
     i=0;
     while(menu[i]!=NULL) {
       if(CheckMedia_d(menu[i])) {
         l = strlen(menu[i]);
         if(l>LEN)strcpy(menu[i]+LEN-5,"...");
         Dadd(L,menu[i]);
       }
       i++;
     }
     free(menu);
     count = Dcount(L);
     menu= (char **)malloc(sizeof(char *)*(count+1));
     Resetlink(L);
     i=0;
     while( (pt = (char *)Getrecord(L)) != NULL) menu[i++]=pt;
     menu[i]= NULL;
     Dfree(L);
     
   }
   return menu;
}
int  kgGetFoldertextbox1callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;T_ELMT *e; 
  int ret=1;
  DIX *X;void *pt; 
  char buff[500];
  char **arg;
  DIS *S;
  DIT *T;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  arg = (char **)pt;
  X = (DIX *)kgGetNamedWidget(Tmp,"kgGetFolderWidget6");
  S = (DIS *)kgGetNamedWidget(Tmp,"kgGetFolderWidget8");
  T = (DIT *)kgGetNamedWidget(Tmp,"kgGetFolderWidget2");
  if(S->menu != NULL) kgFreeDouble((void **)(S->menu));
  S->menu= NULL;
  strcpy(buff,kgGetString(T,0));
  strcpy(arg[1],buff);
  S->menu = ProcessFileMenu(buff);
  kgFreeThumbNails((ThumbNail**)(X->list));
  X->list = (void **)kgFolderThumbNails((char *)buff);
  X->pt= X->list;
  kgUpdateWidget(S);
  kgUpdateWidget(X);
  kgUpdateWidget(T);
  kgUpdateOn(D);
  return ret;
}
int  kgGetFolderbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,k; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  DIX *X;void *pt; 
  char buff[500];
  char **arg;
  DIS *S;
  DIT *T;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  arg = (char **)pt;
  X = (DIX *)kgGetNamedWidget(Tmp,"kgGetFolderWidget6");
  S = (DIS *)kgGetNamedWidget(Tmp,"kgGetFolderWidget8");
  T = (DIT *)kgGetNamedWidget(Tmp,"kgGetFolderWidget2");
  if(S->menu != NULL) kgFreeDouble((void **)(S->menu));
  S->menu= NULL;
  strcpy(buff,arg[1]);
  k= strlen(buff);
  while((buff[k]!='/')&&(k>1)) {buff[k]='\0';k--;}
  buff[k]='\0';
  kgSetString(T,0,buff);
  strcpy(arg[1],buff);
  S->menu = ProcessFileMenu(buff);
  kgFreeThumbNails((ThumbNail**)(X->list));
  X->list = (void **)kgFolderThumbNails((char *)buff);
  X->pt= X->list;
  kgUpdateWidget(S);
  kgUpdateWidget(X);
  kgUpdateWidget(T);
  kgUpdateOn(D);
  return ret;
}
void  kgGetFolderbutton1init(DIN *B,void *pt) {
}
int  kgGetFolderbutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  
  DIX *X;void *pt; 
  char buff[500];
  char **arg;
  DIS *S;
  DIT *T;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  arg = (char **)pt;
  X = (DIX *)kgGetNamedWidget(Tmp,"kgGetFolderWidget6");
  S = (DIS *)kgGetNamedWidget(Tmp,"kgGetFolderWidget8");
  T = (DIT *)kgGetNamedWidget(Tmp,"kgGetFolderWidget2");
  if(S->menu != NULL) kgFreeDouble((void **)(S->menu));
  S->menu= NULL;
  strcpy(buff,arg[0]);
  kgSetString(T,0,buff);
  strcpy(arg[1],buff);
  S->menu = ProcessFileMenu(buff);
  kgFreeThumbNails((ThumbNail**)(X->list));
  X->list = (void **)kgFolderThumbNails((char *)buff);
  X->pt= X->list;
  kgUpdateWidget(S);
  kgUpdateWidget(X);
  kgUpdateWidget(T);
  kgUpdateOn(D);
  return ret;
}
void  kgGetFolderbutton2init(DIN *B,void *pt) {
}
int  kgGetFolderbrowser1callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;
  DIX *X;void *pt; 
  char buff[500];
  char **arg;
  DIS *S;
  DIT *T;
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  arg = (char **)pt;
  X = (DIX *)kgGetWidget(Tmp,i);
  S = (DIS *)kgGetNamedWidget(Tmp,"kgGetFolderWidget8");
  T = (DIT *)kgGetNamedWidget(Tmp,"kgGetFolderWidget2");
  if(S->menu != NULL) kgFreeDouble((void **)(S->menu));
  S->menu= NULL;
  strcpy(buff,kgGetString(T,0));
  if(strcmp(buff,"/") != 0)strcat(buff,"/");
  strcat(buff,kgGetSelectedString(X));
  kgSetString(T,0,buff);
  strcpy(arg[1],buff);
  S->menu = ProcessFileMenu(buff);
  kgFreeThumbNails((ThumbNail**)(X->list));
  X->list = (void **)kgFolderThumbNails((char *)buff);
  X->pt= X->list;
  kgUpdateWidget(S);
  kgUpdateWidget(X);
  kgUpdateWidget(T);
  kgUpdateOn(D);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  kgGetFolderbrowser1init(DIX *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
   char **arg;
   arg = (char **)pt;
   X->list = (void **)kgFolderThumbNails((char *)arg[1]);
   X->pt= X->list;
}
int  kgGetFoldersplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
  }
  return ret;
}
void  kgGetFoldersplbutton1init(DIL *B,void *pt) {
}
void  kgGetFolderbrowser2init(DIS *S,void *pt) {
   char **arg;
   arg = (char **)pt;
   S->menu = ProcessFileMenu(arg[1]);
}
int kgGetFolderinit(void *Tmp) {
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
int kgGetFoldercleanup(void *Tmp) {
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
int kgGetFolderCallBack(void *Tmp,void *tmp) {
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
int kgGetFolderResizeCallBack(void *Tmp) {
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
int kgGetFolderWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
