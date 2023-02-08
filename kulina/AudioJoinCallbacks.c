#include <kulina.h>
#include <kulina.h>
#include "ConvertData.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mediainfo.h"
#include "kgutils.h"

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
extern int Jpipe[2];
extern int Jstat[2];

float GetTimeval(char *);
int FileStat(char *flname);
int CheckVideo(char *flname);
int CheckMedia(char *flname);
int FileStat(char *);
DIX *AX2=NULL;
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
int GetWavHeaderLength(char *flname);

int ProcessToAudioPipe(int pip0,int pip1,int Pid) {
     char buff[1000],work[100];
     int ret =0;
     int ch,i=0,j,pos,len,OK=0;
     float per=0.0,csec,m,h,s,totsec=0;;
     char *pt;
//     close(pip1);
     while((ch=GetLine(pip0,buff)) ) {
//         if(!GetTimedLine(StatusGrab[0],work,300)) break;
//         if(!GetTimedLine(Jstat[0],work,300)) break;
         if(ch< 0) continue;
         if ( pos = SearchString(buff,(char *)"Duration:") >=0 ) {
	       pt = buff+pos+10;
	       totsec = GetTimeval(pt);
               sprintf(work,"Esec: %f \n",totsec);
               write(Jpipe[1],work,strlen(work));
               continue;
	 }
         if(SearchString(buff,(char *)"frame=")>=0)  {
           if((pos=SearchString(buff,(char *)"%"))>=0)  {
             buff[pos]=' ';
             pos = SearchString(buff,(char *)"(");
             if(pos>=0) {
               pt = buff+pos+1;
               sscanf(pt,"%f",&per);
               sprintf(work,"Cur: %f \n",per);
               write(Jpipe[1],work,strlen(work));
             }
           }
           if(SearchString(buff,(char *)"size=")>=0)  {
             pos = SearchString(buff,(char *)"time=");
             if(pos>=0) {
               pt = buff+pos+5;
               s += (h*3600+m*60);
               per = GetTimeval(pt);
               sprintf(work,"Cur: %f \n",per);
               write(Jpipe[1],work,strlen(work));
//             printf("%s",work);
               continue;
             }
           }
	 }
     }
     return ret;
}
int joinwav(char *Folder,int count) {
  char buff[4098];
  union HEADER {char raw[256];int data[64];} header1,header2;
  FILE *fp,*fp2,*of;
  char *dpt;
  int ir,i,j,ln,ln2;
  int *ipt;
  int data,chnk1;
  sprintf(buff,"%-s/M%-4.4d.wav",Folder,0);
  ln = GetWavHeaderLength(buff);
  if(ln==0) return 0;
  fp = fopen(buff,"r");
  if(fp==NULL) return 0;
  fread(header1.raw,1,ln,fp);
  ipt = (int *)(header1.raw+ln-4);
  data = *ipt;
  chnk1 = header1.data[1];
  for(i=1;i<count;i++) {
    sprintf(buff,"%-s/M%-4.4d.wav",Folder,i);
    ln2 = GetWavHeaderLength(buff);
    if(ln2==0) continue;
    fp2 = fopen(buff,"r");
    if(fp2==NULL) return 0;
    fread(header2.raw,1,ln2,fp2);
    ipt = (int *)(header2.raw+ln2-4);
    data += (*ipt);
    chnk1 += (*ipt);
    fclose(fp2);
//    for(j=0;j<11;j++) printf("%d ",header1.data[j]);
//    printf("\n");
  }
  ipt = (int *)(header1.raw+ln-4);
  *ipt= data;
  header1.data[1]=chnk1;
  sprintf(buff,"%-s/out.wav",Folder);
  of = fopen(buff,"w");
  fwrite(header1.raw,1,ln,of);
  while( (ir=fread(buff,1,4098,fp))==4098) {
    fwrite(buff,1,4098,of);
  }
  if(ir> 0) fwrite(buff,1,ir,of);
  for(i=1;i<count;i++) {
    sprintf(buff,"%-s/M%-4.4d.wav",Folder,i);
    ln2 = GetWavHeaderLength(buff);
    if(ln2==0) continue;
    fp2 = fopen(buff,"r");
    if(fp2==NULL) return 0;
    fread(header2.raw,1,ln2,fp2);
    while( (ir=fread(buff,1,4098,fp2))==4098) {
      fwrite(buff,1,4098,of);
    }
    if(ir> 0) fwrite(buff,1,ir,of);
    fclose(fp2);
  }
  fclose(fp);
  fclose(of);
  return 1;
}
void * ProcessAudioInput_o(void *mtmp) {
  MEDIAINFO *mpt;
  char command[1000];
  mpt = (MEDIAINFO *)mtmp;
  sprintf(command,"ffmpegfun -i \"%s\" -vn -aq 2 -ac 2 -ar 44100 "
         "-acodec pcm_s32le -y %s/F%-4.4d.wav ",
         mpt->Flname, mpt->Folder,mpt->id);
//  printf("%s\n",command);
  runfunction(command,ProcessSkip,ffmpegfun);
  sprintf(command,"kgmplayer -ao pcm:file=%s/M%-4.4d.wav -vo null "
      " %s/F%-4.4d.wav " ,mpt->Folder,mpt->id,mpt->Folder,mpt->id);
  runfunction(command,ProcessSkip,Mplayer);
//  printf("%s\n",command);
  return NULL;
}
void * ProcessAudioInput(void *mtmp) {
  MEDIAINFO *mpt;
  char command[1000];
  mpt = (MEDIAINFO *)mtmp;
  sprintf(command,"ffmpegfun -i \"%s\" -vn -aq 2 -ac 2 -ar 44100 "
         "-acodec pcm_s32le -y %s/M%-4.4d.wav ",
         mpt->Flname, mpt->Folder,mpt->id);
//  printf("%s\n",command);
  runfunction(command,ProcessToPipe,ffmpegfun);
  return NULL;
}
int JoinToMp3( CONVDATA *cn) {
  int pid,status,id,Qty;
  char Folder[500];
  Dlink *L;
  CONVDATA Cn;
  MEDIAINFO *mpt;
  Cn= *cn;
  if ((pid=fork())==0) {
    char command[10000],File[500],options[5000],Fifo[500],Qstr[100];
    void *Thds;
    sprintf(Folder,"%-s/%-d",getenv("HOME"),getpid());
    if(FileStat(Folder)) kgCleanDir(Folder);
    mkdir(Folder,0700);
    sprintf(Fifo,"/tmp/Fifo%-d",getpid());
    mkfifo(Fifo,0600);
    if(pipe(Jpipe) < 0) exit(0);
    if(pipe(Jstat) < 0) exit(0);
    sprintf(GrabFileName,"Joining Audios to : %-s\n",Cn.outfile);
    MonPipe = Jpipe[0];
    if( fork()==0) {
      close(Jpipe[1]);
      close(Jstat[0]);
      RunMonitorJoin(NULL);
      exit(0);
    }
    close(Jpipe[0]);
    close(Jstat[1]);
    L = (Dlink *)Cn.Alist;
    Resetlink(L);
    id=0;
    sprintf(options,"!c01"
      "Audio joining may take time!! Pl. be patient...\n");
    write(Jpipe[1],options,strlen(options));
//    Thds = OpenThreads(0);
    while( (mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
      Esec = mpt->TotSec;
//      sprintf(options,"Esec: %f\n",mpt->TotSec);
//      write(Jpipe[1],options,strlen(options));
      sprintf(options,"Processing %-s\n",mpt->Flname);
      write(Jpipe[1],options,strlen(options));
      sprintf(options,"Esec: %f\n",mpt->TotSec);
      write(Jpipe[1],options,strlen(options));
#if 0
      sprintf(command,"ffmpegfun -i \"%s\" -vn -aq 2 -ac 2 -ar 44100 "
         "-acodec pcm_s32le -y %s/F%-4.4d.wav ",
         mpt->Flname, Folder,id);
        printf("%s\n",command);
      runfunction(command,ProcessSkip,ffmpegfun);
      sprintf(command,"kgmplayer -ao pcm:file=%s/M%-4.4d.wav -vo null %s/F%-4.4d.wav " 
         ,Folder,id,Folder,id);
      runfunction(command,ProcessSkip,Mplayer);
#else
      mpt->Folder=Folder;
      mpt->id=id;
      ProcessAudioInput(mpt);
//      DoInAnyThread(Thds,ProcessAudioInput,mpt);
#endif
 //    printf("%s\n",command);
      id++;
    }
//    sprintf(options,"!c01 Waiting for input processing\n");
//    write(Jpipe[1],options,strlen(options));
//    WaitThreads(Thds);
//    CloseThreads(Thds);
    if(id==0) exit(0);
    sprintf(options,"Joining...\n");
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
#if 0
     sprintf(command,"kglame    -q 2 -h %-s/out.wav "
        "  \"%-s\" ", Folder ,Cn.outfile);
//        printf("%s\n",command);
     runfunction(command,ProcessToAudioPipe,kgLame);
#else
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
#endif
     remove(Fifo);
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
 
char * MakeAjoinFile(void) {
  char buff[500],*pt;
  int id=0,ln;
  sprintf(buff,"%-s/Music/",getenv("HOME"));
  ln = strlen(buff);
  pt = buff+ln;
  while(1) {
    sprintf(pt,"JoinedAudio_%-4.4d.mp3",id);
//    printf("%s\n",buff);
    if (!FileStat(buff)) break;
    id++;
  }
  ln = strlen(buff);
  pt = (char *)malloc(ln+1);
  strcpy(pt,buff);
  return pt;
}
ThumbNail **AddItemtoAlist(char *newitem) {
  char buff[500];
  MEDIAINFO *pt;
  FILE *fp;
  Dlink *L;
  int ln,i,Cond=0,index;
  char **menu;
  char *file;

  L = (Dlink *) cndata.Alist;
  if(L==NULL) L = Dopen();
  cndata.Alist = L;
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
ThumbNail **DeleteItemsfromAlist(void) {
  char buff[500];
  MEDIAINFO *pt;
  FILE *fp;
  Dlink *L;
  int ln,i,index;
  char **menu;
  char *file;
  L = Dopen();
  L = (Dlink *) cndata.Alist;
  if(L==NULL) L = Dopen();
  cndata.Alist = L;
  ln = Dcount(L);
  Resetlink(L);
  i=-1;
  while( (pt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    i++;
    if( kgGetSwitch(AX2,i)) {
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

int  AudioJoinbrowser1callback(int item,int i,void *Tmp) {
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
  AX2=X;
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  AudioJoinbrowser1init(DIX *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
  AX2 = X;
}
int  AudioJoinbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  char filename[500];
  ThumbNail **th;

  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  AX2 = (DIX *)kgGetNamedWidget(Tmp,(char *)"AudioList");
  n = B->nx*B->ny;
  filename[0]='\0';
  switch(butno) {
    case 1:  
      if(kgFolderBrowser(NULL,100,100,filename,(char *)"*")) {
        CheckMedia(filename);
        if(Minfo.Audio !=0 ) { 
          th = AddItemtoAlist(filename);
          kgFreeThumbNails((ThumbNail **)kgGetList(AX2));
          kgSetList(AX2,(void **)th);
          kgUpdateWidget(AX2);
          kgUpdateOn(Tmp);
        }
      }
      break;
      case 2:
      th = (ThumbNail **) kgGetList(AX2);
      if(th==NULL) break;
      if(th[0]==NULL) break;
      th = DeleteItemsfromAlist();
      kgFreeThumbNails((ThumbNail **)kgGetList(AX2));
      kgSetList(AX2,(void **)th);
      kgUpdateWidget(AX2);
      kgUpdateOn(Tmp);
      break;
  }
  return ret;
}
void  AudioJoinbutton1init(DIN *B,void *pt) {
}
int  AudioJointextbox1callback(int cellno,int i,void *Tmp) {
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
int  AudioJoinsplbutton1callback(int butno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"AjoinOut");
  Of = kgGetString(T,0);
  strcpy(cndata.outfile,Of);
  L = (Dlink *)cndata.Alist;
  Resetlink(L);
  n=0;
  while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    n++;
    TotSec += (mpt->TotSec);
  }
//  printf("fps = %f\n",cndata.fps);
  cndata.Fcount=n;
  cndata.EndSec=TotSec;
  cndata.code=4;
  cndata.Eol='\n';
  sprintf(buff,"%d \"%-s\" %d  %f\n",
       cndata.code, cndata.outfile,cndata.Fcount,TotSec);
  write(ToTools[1],buff,strlen(buff));
  n=0;
  Resetlink(L);
  while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    sprintf(buff,"\"%-s\" %f\n",mpt->Flname,mpt->TotSec);
//    printf("%s",buff);
    write(StatusTools[1],buff,strlen(buff));
  }
 
  switch(butno) {
    case 1: 
      ret=0;
      break;
  }
  kgSplashMessage(Tmp,100,100,300,40,(char *)"Send for Processing",1,0,15);
  return ret;
}
void  AudioJoinsplbutton1init(DIL *B,void *pt) {
}
int AudioJoininit(void *Tmp) {
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
int AudioJoincleanup(void *Tmp) {
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
int AudioJoinCallBack(void *Tmp,void *tmp) {
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
int AudioJoinResizeCallBack(void *Tmp) {
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
int AudioJoinWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
