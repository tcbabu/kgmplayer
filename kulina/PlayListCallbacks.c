#include <kulina.h>
extern char HomeDir[200];
void *X1,*X2;
extern char CurPlayList[200];
Dlink *Plink=NULL;
ThumbNail **MakeItemList(char *file);
int GetBaseIndex(char *s);
int DupCond(void *s1,void *s2) {
  if(strcmp((char *)s1,(char *)s2)==0) return 1;
  else return 0;
}
ThumbNail **AddPlayList(char *newlist) {
  char buff[300],*pt;
  FILE *fp;
  Dlink *L;
  int ln,i,Cond=0;
  char **menu;
  
  sprintf(buff,"%-s/.kgMplayer/Playlist",HomeDir);
//  printf("List: %s\n",buff);
  fp =fopen(buff,"r");
  L = Dopen();
  while( fgets(buff,299,fp) != NULL) {
    ln=strlen(buff);
    buff[ln-1]='\0';
    pt= (char *)malloc(ln);
    strcpy(pt,buff);
    Dadd(L,pt);
  }
  Resetlink(L);
  while( (pt=(char *)Getrecord(L))!= NULL) {
     if(DupCond(newlist,pt)) Cond=1;
  }
  if(Cond==0) Dappend(L,(void *)newlist);
//  Drmvdup_cond(L,DupCond);
  ln = Dcount(L);
  menu = (char **)malloc(sizeof(char *)*(ln+1));
  menu[ln]=NULL;
  Resetlink(L);
  i=0;
  fclose(fp);
  sprintf(buff,"%-s/.kgMplayer/Playlist",HomeDir);
  fp =fopen(buff,"w");
  while( (pt=(char *)Getrecord(L))!= NULL) {
    menu[i]=pt;
    fprintf(fp,"%s\n",pt);
    i++;
  }
  Dfree(L);
  fclose(fp);
  sprintf(buff,"%-s/.kgMplayer/%-s",HomeDir,newlist);
  fp =fopen(buff,"a");
  fclose(fp);
  return kgStringToThumbNails(menu);
}
ThumbNail **DeletePlayList(char *newlist) {
  char buff[300],*pt;
  FILE *fp;
  Dlink *L;
  int ln,i;
  char **menu;
  
  sprintf(buff,"%-s/.kgMplayer/Playlist",HomeDir);
  fp =fopen(buff,"r");
  L = Dopen();
  while( fgets(buff,299,fp) != NULL) {
    ln=strlen(buff);
    buff[ln-1]='\0';
    if( strcmp(buff,newlist)==0) continue;
    pt= (char *)malloc(ln);
    strcpy(pt,buff);
    Dadd(L,pt);
  }
  ln = Dcount(L);
  menu = (char **)malloc(sizeof(char *)*(ln+1));
  menu[ln]=NULL;
  Resetlink(L);
  i=0;
  fclose(fp);
  sprintf(buff,"%-s/.kgMplayer/Playlist",HomeDir);
  fp =fopen(buff,"w");
  while( (pt=(char *)Getrecord(L))!= NULL) {
    menu[i]=pt;
    fprintf(fp,"%s\n",pt);
    i++;
  }
  Dfree(L);
  fclose(fp);
  sprintf(buff,"%-s/.kgMplayer/%-s",HomeDir,newlist);
  remove(buff);
  return kgStringToThumbNails(menu);
}
ThumbNail **AddItemtoList(char *newlist) {
  char buff[300],*pt;
  FILE *fp;
  Dlink *L;
  int ln,i,Cond=0,index;
  char **menu;
  char *file;
  
  file = kgGetSelectedString(X1);
  if(file==NULL) return NULL;
  sprintf(buff,"%-s/.kgMplayer/%-s",HomeDir,file);
  fp =fopen(buff,"r");
  L = Dopen();
  while( fgets(buff,299,fp) != NULL) {
    ln=strlen(buff);
    buff[ln-1]='\0';
    pt= (char *)malloc(ln+5);
    strcpy(pt,buff);
    Dadd(L,pt);
  }
#if 0
  Resetlink(L);
  while( (pt=(char *)Getrecord(L))!= NULL) {
     if(DupCond(newlist,pt)) Cond=1;
  }
  if(Cond==0) Dappend(L,(void *)newlist);
#else
  Dappend(L,(void *)newlist);
#endif
  ln = Dcount(L);
  menu = (char **)malloc(sizeof(char *)*(ln+1));
  menu[ln]=NULL;
  Resetlink(L);
  i=0;
  fclose(fp);
  sprintf(buff,"%-s/.kgMplayer/%-s",HomeDir,file);
  fp =fopen(buff,"w");
  while( (pt=(char *)Getrecord(L))!= NULL) {
    index= GetBaseIndex(pt);
    fprintf(fp,"%s\n",pt);
    menu[i]=pt+index;
    kgTruncateString(pt+index,30);
    i++;
  }
  Dfree(L);
  fclose(fp);
  return kgStringToThumbNails(menu);
}
ThumbNail **AddItemstoList(char **newlist) {
  char buff[300],*pt;
  FILE *fp;
  Dlink *L;
  int ln,i,Cond=0,index;
  char **menu;
  char *file;
  
  file = kgGetSelectedString(X1);
  if(file==NULL) return NULL;
  sprintf(buff,"%-s/.kgMplayer/%-s",HomeDir,file);
  fp =fopen(buff,"r");
  L = Dopen();
  while( fgets(buff,299,fp) != NULL) {
    ln=strlen(buff);
    buff[ln-1]='\0';
    pt= (char *)malloc(ln+5);
    strcpy(pt,buff);
    Dadd(L,pt);
  }
#if 0
  Resetlink(L);
  while( (pt=(char *)Getrecord(L))!= NULL) {
     if(DupCond(newlist,pt)) Cond=1;
  }
  if(Cond==0) Dappend(L,(void *)newlist);
#else
  i=0;
  while(newlist[i] != NULL) {
    Dappend(L,(void *)(newlist[i]));
    i++;
  }
#endif
  ln = Dcount(L);
  menu = (char **)malloc(sizeof(char *)*(ln+1));
  menu[ln]=NULL;
  Resetlink(L);
  i=0;
  fclose(fp);
  sprintf(buff,"%-s/.kgMplayer/%-s",HomeDir,file);
  fp =fopen(buff,"w");
  while( (pt=(char *)Getrecord(L))!= NULL) {
    index= GetBaseIndex(pt);
    fprintf(fp,"%s\n",pt);
    menu[i]=pt+index;
    kgTruncateString(pt+index,30);
    i++;
  }
  Dfree(L);
  fclose(fp);
  return kgStringToThumbNails(menu);
}
ThumbNail **DeleteItemsfromList(void) {
  char buff[300],*pt;
  FILE *fp;
  Dlink *L;
  int ln,i,index;
  char **menu;
  char *file;
  
  file = kgGetSelectedString(X1);
  if(file==NULL) return NULL;
  sprintf(buff,"%-s/.kgMplayer/%-s",HomeDir,file);
  fp =fopen(buff,"r");
  L = Dopen();
  i=-1;
  while( fgets(buff,299,fp) != NULL) {
    i++;
    ln=strlen(buff);
    buff[ln-1]='\0';
    if( kgGetSwitch(X2,i)) continue;
    pt= (char *)malloc(ln+5);
    strcpy(pt,buff);
    Dadd(L,pt);
  }
  ln = Dcount(L);
  menu = (char **)malloc(sizeof(char *)*(ln+1));
  menu[ln]=NULL;
  Resetlink(L);
  i=0;
  fclose(fp);
  sprintf(buff,"%-s/.kgMplayer/%-s",HomeDir,file);
  fp =fopen(buff,"w");
  while( (pt=(char *)Getrecord(L))!= NULL) {
    fprintf(fp,"%s\n",pt);
    index= GetBaseIndex(pt);
    menu[i]=pt+index;
    kgTruncateString(pt+index,30);
    i++;
  }
  Dfree(L);
  fclose(fp);
  return kgStringToThumbNails(menu);
}
int  PlayListsplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=0,ln; 
  char filename[150],*list,*pt,buff[300];
  ThumbNail **th;
  FILE *fp;
  DIX *X;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  X = (DIX *)kgGetWidget(Tmp,1);
  strcpy(filename,(char *)"Newlist");
  switch(butno) {
    case 1: 
      th =(ThumbNail **)X->list;
      if(th==NULL) break;
      if(th[0]==NULL) break;
      th = DeletePlayList(kgGetSelectedString(X));
      kgFreeThumbNails((ThumbNail **)X->list);
      kgSetList(X,(void **)th);
      kgUpdateWidget(X);
      kgFreeThumbNails((ThumbNail **)kgGetList(X2));
      list= kgGetSelectedString(X1);
      if(list != NULL) {
       kgSetList(X2,(void **)MakeItemList(list));
      }
      else kgSetList(X2,NULL);
      kgUpdateWidget(X2);
      kgUpdateOn(Tmp);
      break;
    case 2: 
      gscanf(Tmp,(char *)"List Name:%30s",filename);
      th=AddPlayList(filename);
      kgFreeThumbNails((ThumbNail **)X->list);
      kgSetList(X,(void **)th);
      kgUpdateWidget(X);
      kgUpdateOn(Tmp);
      break;
    case 3: 
      list= kgGetSelectedString(X1);
      if(Plink!= NULL) Dempty(Plink);
      Plink=Dopen();
      if((list!=NULL)) {
        ThumbNail **th;
        th=(ThumbNail **) kgGetList(X2);
        if( (th!=NULL) &&(th[0]!= NULL) ){
          strcpy(CurPlayList,list);
          sprintf(buff,"%-s/.kgMplayer/%-s",HomeDir,list);
          fp =fopen(buff,"r");
          while( fgets(buff,299,fp) != NULL) {
            ln=strlen(buff);
             buff[ln-1]='\0';
             pt= (char *)malloc(ln);
             strcpy(pt,buff);
             Dadd(Plink,pt);
          }
        }
      }
      ret=1;
      break;
  }
  return ret;
}
void  PlayListsplbutton1init(DIL *B,void *pt) {
}
ThumbNail **MakePlayList(void) {
  char buff[300],*pt;
  FILE *fp;
  Dlink *L;
  int ln,i;
  char **menu;
  
  sprintf(buff,"%-s/.kgMplayer/Playlist",HomeDir);
  fp =fopen(buff,"r");
  L = Dopen();
  while( fgets(buff,299,fp) != NULL) {
    ln=strlen(buff);
    buff[ln-1]='\0';
    pt= (char *)malloc(ln);
    strcpy(pt,buff);
    Dadd(L,pt);
  }
  ln = Dcount(L);
  menu = (char **)malloc(sizeof(char *)*(ln+1));
  menu[ln]=NULL;
  Resetlink(L);
  i=0;
  while( (pt=(char *)Getrecord(L))!= NULL) {
    menu[i]=pt;
    i++;
  }
  Dfree(L);
  fclose(fp);
  return kgStringToThumbNails(menu);
}
ThumbNail **MakeItemList(char *file) {
  char buff[300],*pt;
  FILE *fp;
  Dlink *L;
  int ln,i,index;
  char **menu;
  
  sprintf(buff,"%-s/.kgMplayer/%-s",HomeDir,file);
  fp =fopen(buff,"r");
  L = Dopen();
  while( fgets(buff,299,fp) != NULL) {
    ln=strlen(buff);
    buff[ln-1]='\0';
    pt= (char *)malloc(ln+5);
    strcpy(pt,buff);
    Dadd(L,pt);
  }
  ln = Dcount(L);
  menu = (char **)malloc(sizeof(char *)*(ln+1));
  menu[ln]=NULL;
  Resetlink(L);
  i=0;
  while( (pt=(char *)Getrecord(L))!= NULL) {
    index= GetBaseIndex(pt);
    menu[i]=pt+index;
    kgTruncateString(pt+index,30);
    i++;
  }
  Dfree(L);
  fclose(fp);
  return kgStringToThumbNails(menu);
}
int  PlayListbrowser1callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIX *X;void *pt; 
  int ret=1; 
  char *list;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  X = (DIX *)kgGetWidget(Tmp,i);
  kgFreeThumbNails((ThumbNail **)kgGetList(X2));
  list= kgGetSelectedString(X1);
  if(list != NULL) {
       kgSetList(X2,(void **)MakeItemList(list));
  }
  else kgSetList(X2,NULL);
  kgUpdateWidget(X2);
  kgUpdateOn(Tmp);
  return ret;
}
void  PlayListbrowser1init(DIX *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
 kgSetList(X,(void **)MakePlayList());
}
int  PlayListbrowser2callback(int item,int i,void *Tmp) {
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
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  PlayListbrowser2init(DIX *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
  
}
int  PlayListbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,j=0; 
  char filename[200];
  ThumbNail **th;
  char **Mfiles=NULL;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  filename[0]='\0';
  switch(butno) {
    case 1: 
      th = (ThumbNail **) kgGetList(X2);
      if(th==NULL) break;
      if(th[0]==NULL) break;
      th = DeleteItemsfromList();
      kgFreeThumbNails((ThumbNail **)kgGetList(X2));
      kgSetList(X2,(void **)th);
      kgUpdateWidget(X2);
      kgUpdateOn(Tmp);
      break;
    case 2: 
#if 0
      if(kgFolderBrowser(NULL,100,100,filename,(char *)"*")) {
        th = AddItemtoList(filename);
        kgFreeThumbNails((ThumbNail **)kgGetList(X2));
        kgSetList(X2,(void **)th);
        kgUpdateWidget(X2);
        kgUpdateOn(Tmp);
      }
#else
      Mfiles = kgGetMediaFiles(NULL);
      if(Mfiles != NULL) {
              th = AddItemstoList(Mfiles);
      }
      kgFreeThumbNails((ThumbNail **)kgGetList(X2));
      kgSetList(X2,(void **)th);
      kgUpdateWidget(X2);
      kgUpdateOn(Tmp);
#endif
      break;
  }
  return ret;
}
void  PlayListbutton1init(DIN *B,void *pt) {
}
int PlayListinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  char *list;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  X1 = kgGetWidget(Tmp,1);
  X2 = kgGetWidget(Tmp,2);
  CurPlayList[0]='\0';
  list= kgGetSelectedString(X1);
  if(list != NULL) {
    kgSetList(X2,(void **)MakeItemList(list));
    kgUpdateWidget(X2);
    kgUpdateOn(Tmp);
  }
  return ret;
}
int PlayListcleanup(void *Tmp) {
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
int PlayListCallBack(void *Tmp,void *tmp) {
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
int PlayListResizeCallBack(void *Tmp) {
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
int PlayListWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
