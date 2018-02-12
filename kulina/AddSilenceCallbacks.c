#include <kulina.h>
#include <math.h>
#include "ConvertData.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mediainfo.h"

extern MEDIAINFO Minfo;
extern CONVDATA cndata;
extern int AConGrp,RangeGrp,EnVoGrp,VConGrp,
    VaspGrp,VsizeGrp,VrangeGrp,VJGrp,AJGrp,AddGrp,SilGrp;
extern int ToTools[2],FromTools[2],StatusTools[2];
extern int StatusGrab[2];
extern int MonPipe;
extern double Ssec,Esec;
extern MEDIAINFO Minfo;
extern int Tools;
extern int Jpipe[2];
extern int Jstat[2];

int FileStat(char *flname);
int CheckVideo(char *flname);
int CheckMedia(char *flname);
MEDIAINFO * GetMediaInfo(char *flname);
int FileStat(char *);
DIX *SX2=NULL;
extern char GrabFileName[300];
int Pval;
int kgLame(int,char **);
int kgffmpeg(int,char **);
int Mplayer(int,char **);
int Mencoder(int,char **);
void *RunMonitorJoin(void *arg);
int WriteInfo(char *);
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int ProcessPrint(int pip0,int pip1,int Pid);
int ProcessSkip(int pip0,int pip1,int Pid);
int ProcessToPipe(int pip0,int pip1,int Pid);
int ProcessToAudioPipe(int pip0,int pip1,int Pid);
int joinwav(char *Folder,int count);
int GetWavHeaderLength(char *flname);
void *RunGetSinfo(void *arg);
int MakeOutputFile(char *Infile,char *Outfile,char *ext);

int InsertSilences( CONVDATA *cn) {
  int pid,status,id,Qty;
  char Folder[500];
  Dlink *L;
  MEDIAINFO *Mif;
  CONVDATA Cn;
  char *mpt;
  float opos,cpos,pos,duration,Mesec;
  Cn= *cn;
  Mif = GetMediaInfo(Cn.infile);
  Mesec= Mif->TotSec;
  free(Mif);
  if ((pid=fork())==0) {
    char command[10000],File[500],options[5000],Qstr[100];
    void *Thds;
    sprintf(Folder,"%-s/%-d",getenv("HOME"),getpid());
    if(FileStat(Folder)) kgCleanDir(Folder);
    mkdir(Folder,0700);
    if(pipe(Jpipe) < 0) exit(0);
    if(pipe(Jstat) < 0) exit(0);
    sprintf(GrabFileName,"Making : %-s\n",Cn.outfile);
    MonPipe = Jpipe[0];
    if( fork()==0) {
      close(Jpipe[1]);
      close(Jstat[0]);
      RunMonitorJoin(NULL);
      exit(0);
    }
    close(Jpipe[0]);
    close(Jstat[1]);
    L = Cn.Slist;
    Resetlink(L);
    id=0;
    sprintf(options,"!c01"
      "Inserting silence  Pl. be patient...\n");
    write(Jpipe[1],options,strlen(options));
//    Thds = OpenThreads(0);
    cpos =0;
    opos =0;
    while( (mpt=(char *)Getrecord(L))!= NULL) {
      sscanf(mpt,"%f%f",&pos,&duration);
      if(fabsf(pos - opos)> 0.001) {
       sprintf(command,"kgffmpeg -i \"%s\" -vn -aq 2 -ac 2 -ar 44100 "
         "-acodec pcm_s32le -y -ss %f -t %f %s/M%-4.4d.wav ",
         Cn.infile, opos, pos - opos,Folder,id);
         opos = pos;
       id++;
       runfunction(command,ProcessSkip,kgffmpeg);
      }
      sprintf(command,"kgffmpeg -ar 44100  -f s32le -acodec pcm_s32le "
        " -ac 2 -i /dev/zero -acodec pcm_s32le -t %f  %s/M%-4.4d.wav ",
         duration,Folder,id);
//      printf("%s\n",command);
      runfunction(command,ProcessSkip,kgffmpeg);
      id++;
    }
    if(opos< Mesec) {
      sprintf(command,"kgffmpeg -i \"%s\" -vn -aq 2 -ac 2 -ar 44100 "
         "-acodec pcm_s32le -y -ss %f  %s/M%-4.4d.wav ",
         Cn.infile, opos, Folder,id);
         opos = pos;
      id++;
      runfunction(command,ProcessSkip,kgffmpeg);
//      printf("%s\n",command);
    }
    if(id==0) exit(0);
    sprintf(options,"Joining...%d\n",id);
    write(Jpipe[1],options,strlen(options));
    sprintf(options,"Per: %f\n",0.0);
    write(Jpipe[1],options,strlen(options));
    joinwav(Folder,id);
    {
     Esec= Cn.EndSec;
     sprintf(options,"Esec: %lf\n",Cn.EndSec);
     write(Jpipe[1],options,strlen(options));
     sprintf(options,"Converting to Output...\n");
     write(Jpipe[1],options,strlen(options));
     Qstr[0]='\0';
     if(kgSearchString(Cn.outfile,(char *)".aac")>=0) {
       sprintf(Qstr," -c:a libfdk_aac ");
     }
     if(kgSearchString(Cn.outfile,(char *)".mp3")>=0) {
       sprintf(Qstr," -c:a libmp3lame -aq 0  ");
     }
     sprintf(command,"kgffmpeg    -i %-s/out.wav "
        " -ac 2 %s -y \"%-s\" ", Folder ,Qstr,Cn.outfile);
//        printf("%s\n",command);
     runfunction(command,ProcessToAudioPipe,kgffmpeg);
     kgCleanDir(Folder);
     strcpy(options,"Joinded Audio Files\n");
     write(Jpipe[1],options,strlen(options));
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
 
char * MakeSilFile(void) {
  char buff[500],*pt;
  int id=0,ln;
  sprintf(buff,"%-s/Music/",getenv("HOME"));
  ln = strlen(buff);
  pt = buff+ln;
  while(1) {
    sprintf(pt,"ModifieddAudio_%-4.4d.mp3",id);
//    printf("%s\n",buff);
    if (!FileStat(buff)) break;
    id++;
  }
  ln = strlen(buff);
  pt = (char *)malloc(ln+1);
  strcpy(pt,buff);
  return pt;
}
int  AddSilencebrowser1callback(int item,int i,void *Tmp) {
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
  SX2= X;
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  AddSilencebrowser1init(DIX *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
 SX2 = X;
}
ThumbNail **AddItemtoSlist(char *newitem) {
  char buff[500];
  char *pt;
  FILE *fp;
  Dlink *L;
  int ln,i,Cond=0,index;
  char **menu;
  char *file;
  float pos,newpos,dur;
  sscanf(newitem,"%f%f",&newpos,&dur);
  if(dur <=0.00001) return NULL;
  L = (Dlink *) cndata.Slist;
  if(L==NULL) L = Dopen();
  cndata.Slist = L;
  Resetlink(L);
  i=0;
  while( (pt=(char *)Getrecord(L))!= NULL) {
    sscanf(pt,"%f",&pos);
    if(fabsf(newpos-pos) < 0.01) return NULL;
  }
  pt = newitem;
  Dappend(L,(void *)pt);
  ln = Dcount(L);
  menu = (char **)malloc(sizeof(char *)*(ln+1));
  menu[ln]=NULL;
  Resetlink(L);
  i=0;
  while( (pt=(char *)Getrecord(L))!= NULL) {
    menu[i]=(char *)malloc(strlen(pt)+1);
    strcpy(menu[i],pt);
//    printf("%s\n",menu[i]);
    i++;
  }
  return kgStringToThumbNails(menu);
}
ThumbNail **DeleteItemsfromSlist(void) {
  char buff[500];
  char  *pt;
  FILE *fp;
  Dlink *L;
  int ln,i,index;
  char **menu;
  char *file;
  L = Dopen();
  L = (Dlink *) cndata.Slist;
  if(L==NULL) L = Dopen();
  cndata.Slist = L;
  ln = Dcount(L);
  Resetlink(L);
  i=-1;
  while( (pt=(char *)Getrecord(L))!= NULL) {
    i++;
    if( kgGetSwitch(SX2,i)) {
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
  while( (pt=(char *)Getrecord(L))!= NULL) {
    menu[i]=(char *)malloc(strlen(pt)+1);
    strcpy(menu[i],pt);
    i++;
  }
  return kgStringToThumbNails(menu);
}

int  AddSilencebutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  char *pt=NULL;
  ThumbNail **th;

  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  SX2 = (DIX *)kgGetNamedWidget(Tmp,(char *)"SilList");
  n = B->nx*B->ny;
  switch(butno) {
    case 1:  
      if ((pt=RunGetSinfo(Tmp)) != NULL)  {
          th = AddItemtoSlist(pt);
          if(th != NULL) {
            kgFreeThumbNails((ThumbNail **)kgGetList(SX2));
            kgSetList(SX2,(void **)th);
            kgUpdateWidget(SX2);
            kgUpdateOn(Tmp);
          }
      }
      break;
      case 2:
      th = (ThumbNail **) kgGetList(SX2);
      if(th==NULL) break;
      if(th[0]==NULL) break;
      th = DeleteItemsfromSlist();
      kgFreeThumbNails((ThumbNail **)kgGetList(SX2));
      kgSetList(SX2,(void **)th);
      kgUpdateWidget(SX2);
      kgUpdateOn(Tmp);
      break;
  }
  return ret;
}
void  AddSilencebutton1init(DIN *B,void *pt) {
}
int  AddSilencetextbox1callback(int cellno,int i,void *Tmp) {
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
int SilListCond(void *s1,void *s2) {
  float pos1,dur1,pos2,dur2;
  sscanf((char *) s1,"%f%f",&pos1,&dur1);
  sscanf((char *) s2,"%f%f",&pos2,&dur2);
  if(pos1 > pos2) return 1;
  else return -1;
}
int  AddSilencesplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  char buff[5000];
  DIALOG *D;DIL *B; 
  int n,ret=1,Qty; 
  float TotSec=0.0,pos,dur;
  char *Of;
  DIT *T;
  Dlink *L;
  char  *mpt;
  MEDIAINFO *Mif;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  T = (DIT *)kgGetNamedWidget(Tmp,"SilOutput");
  Of = kgGetString(T,0);
  strcpy(cndata.outfile,Of);
  Mif = GetMediaInfo(cndata.infile);
  if((Mif->Video==0)&&(Mif->Audio==0)) return 1;
  TotSec = Mif->TotSec;
  free(Mif);
  L = (Dlink *)cndata.Slist;
  Resetlink(L);
  if((mpt=(char *)Getrecord(L))!= NULL) {
   Resetlink(L);
   Dsort((Dlink *)cndata.Slist,SilListCond);
  }
  Resetlink(L);
  n=0;
  while ((mpt=(char *)Getrecord(L))!= NULL) {
    n++;
    sscanf(mpt,"%f%f",&pos,&dur);
//    printf("%s\n",mpt);
    TotSec += dur;
  }
//  printf("fps = %f\n",cndata.fps);
  cndata.Fcount=n;
  cndata.EndSec=TotSec;
  cndata.code=9;
  cndata.Eol='\n';
  sprintf(buff,"%d \"%-s\" \"%-s\" %d  %f\n",
       cndata.code, cndata.infile,cndata.outfile,cndata.Fcount,TotSec);
  write(ToTools[1],buff,strlen(buff));
  Resetlink(L);
  while ((mpt=(char *)Getrecord(L))!= NULL) {
    sprintf(buff,"%s \n",mpt);
//    printf("%s",buff);
    write(StatusTools[1],buff,strlen(buff));
  }
  Dempty(L);
  cndata.Slist=NULL;
  
 
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  AddSilencesplbutton1init(DIL *B,void *pt) {
}
int  AddSilencetextbox2callback(int cellno,int i,void *Tmp) {
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
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext) {
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
     sprintf(pt,"_%-4.4d.%-s",id,ext);
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
int  AddSilencebutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  char FileName[500],OutFile[500];
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  DIT *T,*TO;
  T = (DIT *)kgGetNamedWidget(Tmp,"SilInput");
  TO = (DIT *)kgGetNamedWidget(Tmp,"SilOutput");
  n = B->nx*B->ny;
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
  kgFolderBrowser(NULL,100,100,FileName,"*");
  kgSetString(T,0,FileName);
  sprintf(OutFile,"%-s/Music",getenv("HOME"));
//  MakeOutputFile(FileName,OutFile+strlen(OutFile),"mp3");
  MakeFileInFolder(FileName,OutFile,OutFile,"mp3");
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  strcpy(cndata.infile,FileName);
  strcpy(cndata.outfile,OutFile);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  AddSilencebutton2init(DIN *B,void *pt) {
}
int AddSilenceinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
//  kgSetCurrentWidget(Tmp,1);
  return ret;
}
int AddSilencecleanup(void *Tmp) {
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
int AddSilenceCallBack(void *Tmp,void *tmp) {
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
int AddSilenceResizeCallBack(void *Tmp) {
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
int AddSilenceWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
