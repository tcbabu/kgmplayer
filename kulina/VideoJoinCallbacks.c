#include <kulina.h>
#include "ConvertData.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mediainfo.h"

extern MEDIAINFO Minfo;
extern CONVDATA cndata;
extern int AConGrp,RangeGrp,EnVoGrp,VConGrp,
    VaspGrp,VsizeGrp,VrangeGrp,VJGrp,AJGrp,AddGrp;
extern int ToTools[2],FromTools[2],StatusTools[2];
extern int StatusGrab[2];
extern int MonPipe;
extern double Ssec,Esec;
extern MEDIAINFO Minfo;
extern int Tools;
int Jpipe[2];
int Jstat[2];

int FileStat(char *flname);
int CheckVideo(char *flname);
int CheckMedia(char *flname);
int FileStat(char *);
DIX *VX2=NULL;
extern char GrabFileName[300];
int Pval;
int kgLame(int,char **);
int kgffmpeg(int,char **);
int Mplayer(int,char **);
int Mencoder(int,char **);
void *RunMonitorJoin(void *arg);
int WriteInfo(char *);
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int ProcessPrint(int pip0,int pip1,int Pid) {
     char buff[1000],connection[500],Formats[500],Selected[500];
     int ret =0;
     int ch,i=0,j,pos,len,OK=0;
     float per=0.0;
     char *pt;
     Formats[0]='\0';
     Selected[0]='\0';
//     close(pip1);
     while((ch=GetLine(pip0,buff)) ) {
         if(!GetTimedLine(StatusGrab[0],connection,300)) break;
         if(!GetTimedLine(Jstat[0],connection,300)) break;
         if(ch< 0) continue;
         printf("%s",buff);
     }
     return ret;
}
int ProcessSkip(int pip0,int pip1,int Pid) {
     char buff[1000],connection[500],Formats[500],Selected[500];
     int ret =0;
     int ch,i=0,j,pos,len,OK=0;
     float per=0.0;
     char *pt;
     Formats[0]='\0';
     Selected[0]='\0';
//     close(pip1);
     while((ch=GetLine(pip0,buff)) ) {
         if(!GetTimedLine(StatusGrab[0],connection,300)) break;
         if(!GetTimedLine(Jstat[0],connection,300)) break;
         if(ch< 0) continue;
//         printf("%s",buff);
     }
     return ret;
}
int ProcessToPipe(int pip0,int pip1,int Pid) {
     char buff[1000],work[100];
     int ret =0;
     int ch,i=0,j,pos,len,OK=0;
     float per=0.0,csec,m,h,s;
     char *pt;
//     close(pip1);
     while((ch=GetLine(pip0,buff)) ) {
         if(!GetTimedLine(StatusGrab[0],work,300)) break;
         if(!GetTimedLine(Jstat[0],work,300)) break;
         if(ch< 0) continue;
         if(SearchString(buff,"frame=")>=0)  {
             pos = SearchString(buff,"time=");
             if(pos>=0) {
               pt = buff+pos+5;
               pos= kgSearchString(pt,":");
               pt[pos]=' ';
               pos= kgSearchString(pt,":");
               pt[pos]=' ';
               sscanf(pt,"%f%f%f",&h,&m,&s);
               s += (h*3600+m*60);
               per = s;
               sprintf(work,"Cur: %f \n",per);
               write(Jpipe[1],work,strlen(work));
//      printf("%s",work);
               continue;
             }
         }
         if(SearchString(buff,"size=")>=0)  {
             pos = SearchString(buff,"time=");
             if(pos>=0) {
               pt = buff+pos+5;
               pos= kgSearchString(pt,":");
               pt[pos]=' ';
               pos= kgSearchString(pt,":");
               pt[pos]=' ';
               sscanf(pt,"%f%f%f",&h,&m,&s);
               s += (h*3600+m*60);
               per = s;
               sprintf(work,"Cur: %f \n",per);
               write(Jpipe[1],work,strlen(work));
//      printf("%s",work);
               continue;
             }
         }
//         printf("%s",buff);
     }
     return ret;
}
int JoinToMp4( CONVDATA *cn) {
  int pid,status,id,Qty;
  char Folder[500];
  int Audio=1;
  Dlink *L;
  CONVDATA Cn;
  MEDIAINFO *mpt;
  Cn= *cn;
  Audio =1;
  L = Cn.Vlist;
  Resetlink(L);
  while( (mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    if(mpt->Audio==0) Audio=0;
  }
  if ((pid=fork())==0) {
    char command[10000],File[500],options[5000],Fifo[500],Qstr[100];
    sprintf(Folder,"%-s/%-d",getenv("HOME"),getpid());
    if(FileStat(Folder)) kgCleanDir(Folder);
    mkdir(Folder,0700);
    sprintf(Fifo,"/tmp/Fifo%-d",getpid());
    mkfifo(Fifo,0600);
    if(pipe(Jpipe) < 0) exit(0);
    if(pipe(Jstat) < 0) exit(0);
    sprintf(GrabFileName,"Joining Videos to : %-s\n",Cn.outfile);
    MonPipe = Jpipe[0];
    if( fork()==0) {
      close(Jpipe[1]);
      close(Jstat[0]);
      RunMonitorJoin(NULL);
      exit(0);
    }
    close(Jpipe[0]);
    close(Jstat[1]);
    switch(Cn.Quality) {
      case 1:
        strcpy(Qstr,"3000K");
        break;
      case 2:
        strcpy(Qstr,"1000K");
        break;
      default:
      case 3:
        strcpy(Qstr,"500K");
        break;
    }
    L = Cn.Vlist;
    Resetlink(L);
    id=0;
    sprintf(options,"!c01Importance is for QUALITY,"
      " Video joining is a slow Process!! Pl. be patient...\n");
    write(Jpipe[1],options,strlen(options));
    if(!Audio) {
      sprintf(options,"!c01Atleast one media is without audio"
            " so NO AUDIO in output\n");
      write(Jpipe[1],options,strlen(options));
    }
    while( (mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
      Esec = mpt->TotSec;
      sprintf(options,"Esec: %f\n",mpt->TotSec);
      write(Jpipe[1],options,strlen(options));
#if 0
      if(mpt->Process) {
        sprintf(command,"kgffmpeg -i \"%s\" -f mp4 -b:v %-s -vcodec "
         "libx264 -r %-7.3f -s %-dx%-d -aspect %-d:%-d -y %s/F%-4.4d ",
         mpt->Flname,Qstr,Cn.fps,Cn.Xsize,Cn.Ysize,Cn.Xsize,Cn.Ysize, Folder,id);
        printf("%s\n",command);
        runfunction(command,ProcessSkip,kgffmpeg);
      }
      else {
        sprintf(command,"cp \"%s\" %s/F%-4.4d",mpt->Flname,Folder,id);
        printf("%s\n",command);
        system(command);
      }
#else
       if(Audio) {
        sprintf(command,"kgffmpeg -i \"%s\" -f mp4 -b:v %-s -vcodec "
         "libx264 -r %-7.3f -s %-dx%-d -aspect %-d:%-d -y %s/F%-4.4d ",
         mpt->Flname,Qstr,Cn.fps,Cn.Xsize,Cn.Ysize,Cn.Xsize,Cn.Ysize, Folder,id);
       }
       else {
        sprintf(command,"kgffmpeg -i \"%s\" -an -f mp4 -b:v %-s -vcodec "
         "libx264 -r %-7.3f -s %-dx%-d -aspect %-d:%-d -y %s/F%-4.4d ",
         mpt->Flname,Qstr,Cn.fps,Cn.Xsize,Cn.Ysize,Cn.Xsize,Cn.Ysize, Folder,id);
       }
//        printf("%s\n",command);
        sprintf(options,"Processing %-s\n",mpt->Flname);
        write(Jpipe[1],options,strlen(options));
        runfunction(command,ProcessToPipe,kgffmpeg);
#endif
//     printf("%s\n",command);
      id++;
    }
    if(id==0) exit(0);
    if((pid=fork())!=0) {
     Esec= Cn.EndSec;
     sprintf(options,"Esec: %lf\n",Cn.EndSec);
     write(Jpipe[1],options,strlen(options));
     sprintf(options,"Joining and Converting to mp4\n");
     write(Jpipe[1],options,strlen(options));
     sprintf(command,"kgffmpeg -i %-s -y -f mp4 -vcodec libx264 "
        " -b:v %-s \"%-s\" ", Fifo ,Qstr,Cn.outfile);
//        printf("%s\n",command);
     runfunction(command,ProcessToPipe,kgffmpeg);
     remove(Fifo);
     kgCleanDir(Folder);
     strcpy(options,"Joinded Video Files\n");
     write(Jpipe[1],options,strlen(options));
     close(Jpipe[1]);
     exit(0);
    }
    else {
      int id;
      options[0] ='\0';
      strcpy(command,"Mencoder ");
      for(id=0;id<Cn.Fcount;id++) {
        sprintf(options," \"%-s/F%-4.4d\" ",Folder,id);
        strcat(command,options);
      }
      sprintf(options," -o %s -oac mp3lame -lameopts "
          " q=0:aq=0 "
          " -ovc copy -idx", Fifo);
      strcat(command,options);
//      printf("%s\n",command);
      runfunction(command,ProcessSkip,Mencoder);
//      waitpid(pid,&status,0);
      close(Jpipe[1]);
      exit(0);
    }
  }
  else {
     waitpid(pid,&status,0);
     sprintf(Folder,"%-s/%-d",getenv("HOME"),pid);
     if(FileStat(Folder)) kgCleanDir(Folder);
  }
}
 
char * MakeVjoinFile(void) {
  char buff[500],*pt;
  int id=0,ln;
  sprintf(buff,"%-s/Video/",getenv("HOME"));
  ln = strlen(buff);
  pt = buff+ln;
  while(1) {
    sprintf(pt,"JoinedVideo_%-4.4d.mp4",id);
//    printf("%s\n",buff);
    if (!FileStat(buff)) break;
    id++;
  }
  ln = strlen(buff);
  pt = (char *)malloc(ln+1);
  strcpy(pt,buff);
  return pt;
}
ThumbNail **AddItemtoVlist(char *newitem) {
  char buff[500];
  MEDIAINFO *pt;
  FILE *fp;
  Dlink *L;
  int ln,i,Cond=0,index;
  char **menu;
  char *file;

  L = (Dlink *) cndata.Vlist;
  if(L==NULL) L = Dopen();
  cndata.Vlist = L;
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
ThumbNail **DeleteItemsfromVlist(void) {
  char buff[500];
  MEDIAINFO *pt;
  FILE *fp;
  Dlink *L;
  int ln,i,index;
  char **menu;
  char *file;
  L = Dopen();
  L = (Dlink *) cndata.Vlist;
  if(L==NULL) L = Dopen();
  cndata.Vlist = L;
  ln = Dcount(L);
  Resetlink(L);
  i=-1;
  while( (pt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    i++;
    if( kgGetSwitch(VX2,i)) {
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


int  VideoJoinbrowser1callback(int item,int i,void *Tmp) {
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
  VX2 = X;
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  VideoJoinbrowser1init(DIX *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
 VX2 = X;
}
int  VideoJoinbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  char filename[500];
  ThumbNail **th;

  int n,ret =0; 
  D = (DIALOG *)Tmp;
  VX2 = (DIX *)kgGetNamedWidget(Tmp,(char *)"VideoList");
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  filename[0]='\0';
  switch(butno) {
    case 1: 
      if(kgFolderBrowser(NULL,100,100,filename,(char *)"*")) {
        CheckMedia(filename);
        if(Minfo.Video !=0 ) {
          th = AddItemtoVlist(filename);
          kgFreeThumbNails((ThumbNail **)kgGetList(VX2));
          kgSetList(VX2,(void **)th);
          kgUpdateWidget(VX2);
          kgUpdateOn(Tmp);
        }
      }
      break;
    case 2: 
      th = (ThumbNail **) kgGetList(VX2);
      if(th==NULL) break;
      if(th[0]==NULL) break;
      th = DeleteItemsfromVlist();
      kgFreeThumbNails((ThumbNail **)kgGetList(VX2));
      kgSetList(VX2,(void **)th);
      kgUpdateWidget(VX2);
      kgUpdateOn(Tmp);
      break;
  }
  return ret;
}
void  VideoJoinbutton1init(DIN *B,void *pt) {
}
int  VideoJointextbox1callback(int cellno,int i,void *Tmp) {
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
int  VideoJoinsplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  char buff[5000];
  DIALOG *D;DIL *B; 
  int n,ret=1,Qty; 
  float TotSec=0.0;
  char *Of;
  DIT *T;
  Dlink *L;
  MEDIAINFO *mpt;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  T = (DIT *)kgGetNamedWidget(Tmp,"VjoinOut");
  Of = kgGetString(T,0);
  strcpy(cndata.outfile,Of);
  L = (Dlink *)cndata.Vlist;
  Resetlink(L);
  cndata.Xsize = 0;
  cndata.Ysize = 0;
  cndata.fps=0.0;
  n=0;
  Qty = kgGetSelection(kgGetNamedWidget(Tmp,"VJQuality"));
  while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    n++;
    TotSec += (mpt->TotSec);
    mpt->Process=0;
    if( mpt->Axres> cndata.Xsize) 
          {cndata.Xsize = mpt->Axres;mpt->Process=1;}
    if( mpt->Ayres> cndata.Ysize) 
          {cndata.Ysize = mpt->Ayres;mpt->Process=1;}
    if( mpt->Rxres> cndata.Xsize) 
          {cndata.Xsize = mpt->Rxres;mpt->Process=1;}
    if( mpt->Ryres> cndata.Ysize) 
          {cndata.Ysize = mpt->Ryres;mpt->Process=1;}
    if( mpt->fps> cndata.fps) 
          {cndata.fps = mpt->fps;mpt->Process=1;}
    if( mpt->Rxres!= mpt->Axres) mpt->Process=1;
    if( mpt->Ryres!= mpt->Ayres) mpt->Process=1;
    if( mpt->Rxres!= cndata.Xsize) mpt->Process=1;
    if( mpt->Ryres!= cndata.Ysize) mpt->Process=1;
    if( (int)(mpt->fps*1000)!= (int)(cndata.fps*1000)) mpt->Process=1;
  }
//  printf("fps = %f\n",cndata.fps);
  cndata.Fcount=n;
  cndata.EndSec=TotSec;
#if 0  
  Resetlink(L);
  while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    printf("%s : %d\n",mpt->Flname,mpt->Process);
  }
  printf("%d %d %f\n",cndata.Xsize,cndata.Ysize,cndata.fps);
#endif
  cndata.code=3;
  cndata.Eol='\n';
  sprintf(buff,"%d \"%-s\" %d %d %d %f %d %f\n",
       cndata.code, cndata.outfile,cndata.Xsize,
       cndata.Ysize,cndata.Fcount,cndata.fps,Qty,TotSec);
  write(ToTools[1],buff,strlen(buff));
  n=0;
  Resetlink(L);
  while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    sprintf(buff,"\"%-s\" %d %f %d\n",
       mpt->Flname,mpt->Process,mpt->TotSec,mpt->Audio);
//    printf("%s",buff);
    write(StatusTools[1],buff,strlen(buff));
  }
 
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  VideoJoinsplbutton1init(DIL *B,void *pt) {
}
int VideoJoininit(void *Tmp) {
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
int VideoJoincleanup(void *Tmp) {
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
int VideoJoinCallBack(void *Tmp,void *tmp) {
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
int VideoJoinResizeCallBack(void *Tmp) {
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
int VideoJoinWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
void  VideoJoinbrowser2init(DIRA *R,void *pt) {
}
int  VideoJoinbrowser2callback(int item,int i,void *Tmp) {
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
  return ret;
}
