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
    VaspGrp,VsizeGrp,VrangeGrp,VJGrp,AJGrp,AddGrp,SilGrp,CutGrp;
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
DIX *SX3=NULL;
extern char GrabFileName[300];
extern int Pval;
int kgLame(int,char **);
int kgffmpeg(int,char **);
int ffmpegfun(int,char **);
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
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);

static int FolderBrowser(char *FileName) {
	char *Str=NULL;
	int ret=0,ln;
	if( (Str=kgGetMediaFile(NULL)) != NULL) {
		ln = strlen(Str);
		if(ln>1) {
			ret =1;
			strcpy(FileName,Str);
			free(Str);
		}
	}
	return ret;
}
int MakeAudioCuts( CONVDATA *cn) {
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
    L = (Dlink *)Cn.Cutlist;
    Resetlink(L);
    id=0;
    sprintf(options,"!c01"
      "Making audio cuts  Pl. be patient...\n");
    write(Jpipe[1],options,strlen(options));
//    Thds = OpenThreads(0);
    cpos =0;
    opos =0;
    while( (mpt=(char *)Getrecord(L))!= NULL) {
      sscanf(mpt,"%f%f",&pos,&duration);
      if((pos - opos)> 0.001) {
       sprintf(command,"ffmpegfun -i \"%s\" -vn -aq 2 -ac 2 -ar 44100 "
         "-acodec pcm_s32le -y -ss %f -to %f %s/M%-4.4d.wav ",
         Cn.infile, opos, pos ,Folder,id);
       id++;
       runfunction(command,ProcessSkip,ffmpegfun);
      }
      opos = pos+duration;
    }
    if(opos< Mesec) {
      sprintf(command,"ffmpegfun -i \"%s\" -vn -aq 2 -ac 2 -ar 44100 "
         "-acodec pcm_s32le -y -ss %f  %s/M%-4.4d.wav ",
         Cn.infile, opos, Folder,id);
         opos = pos;
      id++;
      runfunction(command,ProcessSkip,ffmpegfun);
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
     sprintf(command,"ffmpegfun    -i %-s/out.wav "
        " -ac 2 %s -y \"%-s\" ", Folder ,Qstr,Cn.outfile);
//        printf("%s\n",command);
     runfunction(command,ProcessToAudioPipe,ffmpegfun);
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
 
char * MakeCutFile(void) {
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
int  MakeCutsbrowser1callback(int item,int i,void *Tmp) {
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
  SX3 = X;
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  MakeCutsbrowser1init(DIX *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
 SX3 = X;
}
ThumbNail **AddItemtoClist(char *newitem) {
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
  L = (Dlink *) cndata.Cutlist;
  if(L==NULL) L = Dopen();
  cndata.Cutlist = L;
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
ThumbNail **DeleteItemsfromClist(void) {
  char buff[500];
  char  *pt;
  FILE *fp;
  Dlink *L;
  int ln,i,index;
  char **menu;
  char *file;
  L = Dopen();
  L = (Dlink *) cndata.Cutlist;
  if(L==NULL) L = Dopen();
  cndata.Cutlist = L;
  ln = Dcount(L);
  Resetlink(L);
  i=-1;
  while( (pt=(char *)Getrecord(L))!= NULL) {
    i++;
    if( kgGetSwitch(SX3,i)) {
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
int  MakeCutsbutton1callback(int butno,int i,void *Tmp) {
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
  SX3 = (DIX *)kgGetNamedWidget(Tmp,(char *)"CutList");
  n = B->nx*B->ny;
  switch(butno) {
    case 1:  
      if ((pt=(char *)RunGetSinfo(Tmp)) != NULL)  {
          th = AddItemtoClist(pt);
          if(th != NULL) {
            kgFreeThumbNails((ThumbNail **)kgGetList(SX3));
            kgSetList(SX3,(void **)th);
            kgUpdateWidget(SX3);
            kgUpdateOn(Tmp);
          }
      }
      break;
      case 2:
      th = (ThumbNail **) kgGetList(SX3);
      if(th==NULL) break;
      if(th[0]==NULL) break;
      th = DeleteItemsfromClist();
      kgFreeThumbNails((ThumbNail **)kgGetList(SX3));
      kgSetList(SX3,(void **)th);
      kgUpdateWidget(SX3);
      kgUpdateOn(Tmp);
      break;
  }
  return ret;
}
void  MakeCutsbutton1init(DIN *B,void *pt) {
}
int CutListCond(void *s1,void *s2) {
  float pos1,dur1,pos2,dur2;
  sscanf((char *) s1,"%f%f",&pos1,&dur1);
  sscanf((char *) s2,"%f%f",&pos2,&dur2);
  if(pos1 > pos2) return 1;
  else return -1;
}
int  MakeCutstextbox1callback(int cellno,int i,void *Tmp) {
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
int  MakeCutssplbutton1callback(int butno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"CutOutput");
  Of = kgGetString(T,0);
  strcpy(cndata.outfile,Of);
  Mif = GetMediaInfo(cndata.infile);
  if((Mif->Video==0)&&(Mif->Audio==0)) return 1;
  TotSec = Mif->TotSec;
  free(Mif);
  L = (Dlink *)cndata.Cutlist;
  Resetlink(L);
  if((mpt=(char *)Getrecord(L))!= NULL) {
   Resetlink(L);
   Dsort((Dlink *)cndata.Cutlist,CutListCond);
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
  cndata.code=10;
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
  cndata.Cutlist=NULL;
  
 
  switch(butno) {
    case 1: 
      break;
  }

  kgSplashMessage(Tmp,100,100,300,40,(char *)"Send for Processing",1,0,15);
  ret = 0;
  return ret;
}
void  MakeCutssplbutton1init(DIL *B,void *pt) {
}
int  MakeCutstextbox2callback(int cellno,int i,void *Tmp) {
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
int  MakeCutsbutton2callback(int butno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"CutInput");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"CutOutput");
  n = B->nx*B->ny;
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
//  kgFolderBrowser(NULL,100,100,FileName,"*");
  if(!FolderBrowser(FileName))return 0;
  kgSetString(T,0,FileName);
  sprintf(OutFile,"%-s/Music",getenv("HOME"));
//  MakeOutputFile(FileName,OutFile+strlen(OutFile),"mp3");
  MakeFileInFolder(FileName,OutFile,OutFile,(char *)"mp3");
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
void  MakeCutsbutton2init(DIN *B,void *pt) {
}
int MakeCutsinit(void *Tmp) {
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
int MakeCutscleanup(void *Tmp) {
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
int MakeCutsCallBack(void *Tmp,void *tmp) {
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
int MakeCutsResizeCallBack(void *Tmp) {
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
int MakeCutsWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
