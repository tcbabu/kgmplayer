#include <kulina.h>
extern int ToGrab[2],FromGrab[2];
extern int Quality;

int runjob(char *job,int (*ProcessOut)(int,int,int));
int changejob(char *job);
int GetLine(int pip0,char *buff);
int GetBaseIndex(char *s);

extern void *Dia;
extern char FileName[500];
int GRABING=0;
char UrlOld[500];
int  urltextbox1callback(int cellno,int i,void *Tmp) {
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
int  urlbrowser1callback(int item,int i,void *Tmp) {
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
  Quality = kgGetSelection(R);
  kgSetCurrentWidget(D,kgGetWidgetId(D,kgGetNamedWidget(D,(char *)"url")));
  return ret;
}
void  urlbrowser1init(DIRA *R,void *pt) {
}
void WriteMessage(char *buff) {
  if(Dia==NULL) return;
  kgPrintf(Dia,4,buff);
  kgUpdateOn(Dia);
}
char *GetUrlCopy (void) {
   /* caller should free res if it is no NULL */
   char *res=NULL;
   char buff[500],*url,*pt;
   int pos;
   pt  = (char *)kgGetString(kgGetNamedWidget(Dia,(char *)"url"),0);
   strcpy(buff,pt);
   url = buff;
   pos = kgSearchString(url,(char *)"www.");
   if(pos >= 0)  {
     int i=0;
     while(url[i]==' ') i++;
     url= url+i;
     pos = kgSearchString(url,(char *)"http");
     res= (char *)malloc(sizeof(char)*(strlen(url)+8));
     res[0]='\0';
     if(pos < 0) {
       strcpy(res,"http://");
     }
     strcat(res,url);
   }
   else {
     pos = kgSearchString(url,(char *)"http");
     if(pos>= 0) {
       pos = kgSearchString(url,(char *)"://");
       if(pos>=0) {
         int ch;
         pos +=3;
         res= (char *)malloc(sizeof(char)*(strlen(url)+8));
         ch = url[pos];
         url[pos]='\0';
         res[0]='\0';
         strcpy(res,url);
         strcat(res,(char *)"www.");
         url[pos]=ch;
         strcat(res,url+pos);
       }
     }
     else {
       int ch;
       res= (char *)malloc(sizeof(char)*(strlen(url)+8));
       sscanf(url,"%s",res);
     }
   }
   if(res==NULL) {
      WriteMessage((char *)"!z32Sorry!!!!!! Invalid URL");
      kgUpdateOn(Dia);
   }
   return res;
}
static float get_mbsize(char *pt) {
   float sz =0.0;
   int pos;
   pos = kgSearchString(pt,(char *)"(");
   if(pos >=0) {
     pt +=(pos+1);
     pos = kgSearchString(pt,(char *)"M");
     if (pos >=0) {
       pt[pos]=' ';
       sscanf(pt,"%f",&sz);
       pt[pos]='M';
     }
   }
   return sz;
}
static int compsize(void *pt1,void *pt2) {
   if( get_mbsize((char *)pt2) > get_mbsize((char *)pt1)) return 1;
   else return 0;
}
char *GetFormatSize(char *buff,int Qly,int *size) {
     char *pt1,*pt2,*fmt=NULL;
     int pos,i;
     float fsize=0;
     int count;
     Dlink *L;
     L = Dopen();
     i=0;
     while(buff[i]>=' ') i++;
     buff[i]='\0';
     pt1=buff;
     pos = kgSearchString(pt1,(char *)",");
     while(pos>=0) {
       pt2= pt1+pos+1;
       pt1[pos]='\0';
       Dadd(L,(void *)pt1);
       pt1=pt2;
       pos = kgSearchString(pt1,(char *)",");
     }
     count = Dcount(L);
     if(count > 0) {
        fmt= (char *)malloc(sizeof(char )*20);
        switch(Qly) {
          default:
          case 1:
            Dposition(L,1);
            pt1 = (char *)Getrecord(L);
            sscanf((pt1),"%s",fmt);
            fsize = get_mbsize(pt1);
            break;
          case 2:
            if(count >1) Dposition(L,2);
            else Dend(L);
            pt1 = (char *)Getrecord(L);
            sscanf((pt1),"%s",fmt);
            fsize = get_mbsize(pt1);
            break;
          case 3:
            if(count >2) Dposition(L,3);
            else Dend(L);
            pt1 = (char *)Getrecord(L);
            sscanf((pt1),"%s",fmt);
            fsize = get_mbsize(pt1);
            break;
        }
        
     }
     *size = (int)fsize;
     Dfree(L);
     return fmt;
}
int CheckFileExists(char *folder,char *file) {
   int ret=0,i=0;
   char **m=NULL;
   m = (char **)kgFileMenu(folder,(char *)"*");
   if(m != NULL) {
     i=0;
     while(m[i]!= NULL) {
      if(kgSearchString(m[i],file)>= 0) {ret=1;break;}
      i++;
     }
     kgFreeDouble((void **)m);
   }
   return ret;
}
char *GetGrabFile(char *buff,char *fmt) {
    char *flname=NULL;
    char Word[500],vstr[10],folder[200];
    int i,j;
    int ver=0,off=0;
    int pos,ch;
    if(fmt != NULL) {
      pos= kgSearchString(buff,(char *)"Size:");
      i=0;
      while(buff[i]==' ') i++;
      j=0;
      while((i<pos) &&(((ch=buff[i])==' ')||isalnum(ch))) {
        if(ch==' ')Word[j]='_';
        else Word[j]=ch;
        j++;
        i++;
        if(j> 20) break;
      }
      Word[j]='.';
      Word[j+1]='\0';
//      strcpy(Word,"new.");
      ver=0;
      sprintf(folder,"%-s/Video",getenv("HOME"));
      while(CheckFileExists(folder,Word)) {
       int l;
       ver++;
       l = strlen(Word);
       Word[l-1-off]='\0';
       sprintf(vstr,"_%3.3d.",ver);
       off=4;
       strcat(Word,vstr);
      }
      pos= kgSearchString(fmt,(char *)":");
      if(pos>=0) {
        fmt[pos]='\0';
        strcat(Word,fmt);
        fmt[pos]=':';
      }
      else strcat(Word,fmt);
      flname= (char *) malloc(strlen(Word)+1+5);
      strcpy(flname,Word);
    }
    return flname;
}
int SetPlayFile(void *Tmp,char *flname) {
      int index;
      char buff[500];
      DIN *B;
      B = (DIN *) kgGetNamedWidget(Tmp,(char *)"Select");
      strcpy(FileName,flname);
      index = GetBaseIndex(flname);
      kgTruncateString(flname+index,40);
      strcpy(buff,"!c37");
      strcat(buff,flname+index);
      kgChangeButtonTitle(B,0,buff);
      kgRestoreString(flname+index,40);
      return 1;
}
int TryGrabing(void) {
  int n,ret =0,size,ch; 
  char *fmt=NULL;
  char *url,buff[500],reply[500];
  void *Id;
  url = GetUrlCopy();
  GRABING=0;
  if(url != NULL) {
    WriteMessage((char *)"!z43Trying movgrab");
    switch(Quality) {
      case 1:
       strcpy(buff,"chkgrab -f flv,3gp,webm,mp4 -o /tmp/new.mp4 ");
       break;
      case 2:
       strcpy(buff,"chkgrab -f flv,3gp,mp4,webm -o /tmp/new.mp4 ");
       break;
      case 3:
       strcpy(buff,"chkgrab -f mp4,webm,flv,3gp -o /tmp/new.mp4 ");
       break;
      default:
       strcpy(buff,"chkgrab -f flv,3gp,webm,mp4 -o /tmp/new.mp4 ");
       break;
    }
    strcat(buff,url);
    strcat(buff," \n");
//    printf("%s",buff);
    WriteMessage((char *)"!z43Waiting for Grab response");
    write(ToGrab[1],buff,strlen(buff));
//    read(FromGrab[0],reply,500);
    Id =kgOpenBusy(Dia,200,100);
    if((ch=GetLine(FromGrab[0],reply))<=0) {
//      printf("reply: %s",reply);
      kgCloseBusy(Id);
      WriteMessage((char *)"!z43GRAB FAILED");
      free(url);return 0;
    }
//    printf("reply: %s",reply);
    kgCloseBusy(Id);
    if(kgSearchString(reply,(char *)"OKAY:") >=0) {
      char *flname=NULL;
//      read(FromGrab[0],reply,500);
      if((ch=GetLine(FromGrab[0],reply))<=0) {
//        printf("reply: %s",reply);
        free(url);return 0;
      }
//      printf("reply: %s",reply);
//      printf("%s\n",reply);
      fmt = GetFormatSize(reply,Quality,&size);
      sprintf(buff,"!z54Format = %s  :  Size= %dMB\n",fmt,size);
      WriteMessage(buff);
//      read(FromGrab[0],reply,500);
      if((ch=GetLine(FromGrab[0],reply))<=0) {
//        printf("reply: %s",reply);
        free(url);return 0;
      }
//      printf("reply: %s",reply);
      flname=GetGrabFile(reply,fmt);
      sprintf(buff,"!z43File: %-s (%dMB)",flname,size);
      WriteMessage(buff);
      sprintf(buff,"movgrab -f %-s -o %s/Video/%-s %-s \n",
            fmt,getenv("HOME"),flname,url);
//      printf("%s",buff);
      write(ToGrab[1],buff,strlen(buff));
//      printf("Written to Grab\n");
      SetPlayFile(Dia,flname);
      GRABING=1;
      strcpy(UrlOld,url);
      if(fmt != NULL) free(fmt);
      if(flname != NULL) free(flname);
    }
    else {
//      printf("Got Failed\n");
      if(kgSearchString(reply,(char *)"FAILED:") >=0) WriteMessage((char *)"FAILED TO GRAB");
      else WriteMessage((char *)"UNKNOWN RESPONSE");
    }
    free(url);
  }
  return GRABING;
}
int  urlbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,size; 
  char *fmt=NULL;
  char *url,buff[500],reply[500];
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
//  url = (char *)kgGetString(kgGetNamedWidget(D,(char *)"url"),0);
#if 1
  TryGrabing();
#else
  url = GetUrlCopy();
  GRABING=0;
  if(url != NULL) {
    WriteMessage((char *)"!c04!z43Trying movgrab");
    switch(Quality) {
      case 1:
       strcpy(buff,"chkgrab -f flv,3gp,webm,mp4 -o /tmp/new.mp4 ");
       break;
      case 2:
       strcpy(buff,"chkgrab -f flv,3gp,mp4,webm -o /tmp/new.mp4 ");
       break;
      case 3:
       strcpy(buff,"chkgrab -f mp4,webm,flv,3gp -o /tmp/new.mp4 ");
       break;
      default:
       strcpy(buff,"chkgrab -f flv,3gp,webm,mp4 -o /tmp/new.mp4 ");
       break;
    }
    strcat(buff,url);
    strcat(buff," \n");
//    printf("%s",buff);
    WriteMessage((char *)"!c04!z43Waiting for Grab response");
    write(ToGrab[1],buff,strlen(buff));
    read(FromGrab[0],reply,500);
    if(kgSearchString(reply,(char *)"OKAY:") >=0) {
      char *flname;
      read(FromGrab[0],reply,500);
//      printf("%s\n",reply);
      fmt = GetFormatSize(reply,Quality,&size);
      sprintf(buff,"!z54!c04Format = %s  :  Size= %dMB\n",fmt,size);
      WriteMessage(buff);
      read(FromGrab[0],reply,500);
      flname=GetGrabFile(reply,fmt);
      sprintf(buff,"!c04!z43File: %-s (%dMB)",flname,size);
      WriteMessage(buff);
      sprintf(buff,"movgrab -f %-s -o %s/Video/%-s %-s \n",
            fmt,getenv("HOME"),flname,url);
//      printf("%s",buff);
      write(ToGrab[1],buff,strlen(buff));
//      printf("Written to Grab\n");
      SetPlayFile(Tmp,flname);
      GRABING=1;
      strcpy(UrlOld,url);
      if(fmt != NULL) free(fmt);
      if(flname != NULL) free(flname);
    }
    else {
//      printf("Got Failed\n");
      WriteMessage((char *)"!c04FAILED TO GRAB");
    }
    free(url);
  }
#endif
  kgSetCurrentWidget(D,kgGetWidgetId(D,kgGetNamedWidget(D,(char *)"url")));
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  urlbutton1init(DIN *B,void *pt) {
}
int  urlbutton2callback(int butno,int i,void *Tmp) {
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
  kgSetCurrentWidget(D,kgGetWidgetId(D,kgGetNamedWidget(D,(char *)"url")));
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  urlbutton2init(DIN *B,void *pt) {
}
int  urlbutton3callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  char Command[600];
  char *url,*brw;
  int n,ret =0,l; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  brw = kgWhich((char *)"firefox");
  if(brw==NULL) kgWhich((char *)"chrome");
  if(brw== NULL) return ret;
  kgSetCurrentWidget(D,kgGetWidgetId(D,kgGetNamedWidget(D,(char *)"url")));
  strcpy(Command,brw);
  strcat(Command," https://www.google.com/search?q=");
  free(brw);
  url = (char *)kgGetString(kgGetNamedWidget(D,(char *)"url"),0);
  if(url[0]=='\0') return ret;
  n=0;
  l = strlen(Command);
  while(url[n]>= ' ') {
    if(url[n]== ' ') Command[l]='+';
    else Command[l]=url[n];
    n++;l++;
  }
  Command[l]='\0';
//  printf("%s\n",Command);
//  system(Command);
  kgSetString(kgGetNamedWidget(D,(char *)"url"),0,(char *)"");
  kgUpdateWidget(kgGetNamedWidget(D,(char *)"url"));
//  runjob(Command,NULL);
  if(fork()==0) {
   changejob(Command); 
  }
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  urlbutton3init(DIN *B,void *pt) {
}
int urlinit(void *Tmp) {
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
int urlcleanup(void *Tmp) {
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
int urlCallBack(void *Tmp,void *tmp) {
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
int urlResizeCallBack(void *Tmp) {
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
int urlWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
