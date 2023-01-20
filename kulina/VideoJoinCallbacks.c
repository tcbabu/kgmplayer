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
static char libvcodec[20];
int Jpipe[2];
int Jstat[2];

int FileStat(char *flname);
int CheckVideo(char *flname);
int CheckMedia(char *flname);
int FileStat(char *);
DIX *VX2=NULL;
extern char GrabFileName[300];
extern int Pval;
int kgLame(int,char **);
int kgffmpeg(int,char **);
int ffmpegfun(int,char **);
int Mplayer(int,char **);
int Mencoder(int,char **);
void *RunMonitorJoin(void *arg);
int WriteInfo(char *);
MEDIAINFO * GetMediaInfo(char *flname);
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
//         if(!GetTimedLine(StatusGrab[0],connection,300)) break;
//         if(!GetTimedLine(Jstat[0],connection,300)) break;
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
//         if(!GetTimedLine(StatusGrab[0],connection,300)) break;
//         if(!GetTimedLine(Jstat[0],connection,300)) break;
         if(ch< 0) continue;
//         printf("%s",buff);
     }
     return ret;
}
int ProcessToPipe(int pip0,int pip1,int Pid) {
     char buff[1000],work[1000];
     int ret =0;
     int ch,i=0,j,pos,len,OK=0,line=0;
     float per=0.0,csec,m,h,s,totsec;
     char *pt;
//     close(pip1);
     totsec = Minfo.TotSec;
     while((ch=GetLine(pip0,buff)) ) {
         line++;
#if 0
         if(!GetTimedLine(StatusGrab[0],work,500)){
		 printf("breaking got StatusGrab\n");
		 fflush(stdout);
		 break;
	 }
         if(!GetTimedLine(Jstat[0],work,500)) {
		 printf("breaking got Jstat\n");
		 fflush(stdout);
		 break;
	 }
#endif
         if(ch< 0) continue;
         if(SearchString(buff,(char *)"frame=")>=0)  {
             pos = SearchString(buff,(char *)"Duration:");
             if(pos>=0) {
		 int i= 0;
		 int hr,mt;
		 float sec;
		 pt = buff+pos+10;
		 sscanf(pt,"%s",work);
		 while( (work[i]!=',')){
			 if(work[i]==':') work[i]=' ';
			 i++;
		 } 
		 work[i]='\0';
//		 printf("%s\n",work);
		 sscanf(work,"%d%d%f",&hr,&mt,&sec);
                 totsec= hr*3600+mt*60+sec;
//		 printf("Totsec = %f\n",totsec);
//		 continue;
	     }
             pos = SearchString(buff,(char *)"time=");
             if(pos>=0) {
               pt = buff+pos+5;
               pos= kgSearchString(pt,(char *)":");
               pt[pos]=' ';
               pos= kgSearchString(pt,(char *)":");
               pt[pos]=' ';
               sscanf(pt,"%f%f%f",&h,&m,&s);
               s += (h*3600+m*60);
               per = s*100/totsec;
               sprintf(work,"Cur: %f \n",per);
               write(Jpipe[1],work,strlen(work));
//      printf("%s",work);
               continue;
             }
         }
         if(SearchString(buff,(char *)"size=")>=0)  {
             pos = SearchString(buff,(char *)"time=");
             if(pos>=0) {
               pt = buff+pos+5;
               pos= kgSearchString(pt,(char *)":");
               pt[pos]=' ';
               pos= kgSearchString(pt,(char *)":");
               pt[pos]=' ';
               sscanf(pt,"%f%f%f",&h,&m,&s);
               s += (h*3600+m*60);
               per = s*100/totsec;
               sprintf(work,"Cur: %f \n",per);
               write(Jpipe[1],work,strlen(work));
               continue;
             }
         }
     }
     return ret;
}
int ProcessData(int pip0,int pip1,int Pid) {
     char buff[1000],work[1000];
     int ret =0;
     int ch,i=0,j,pos,len,OK=0,line=0;
     float per=0.0,csec,m,h,s,totsec;
     char *pt;
//     close(pip1);
     totsec = Minfo.TotSec;
     printf("Inside ProcessTo Pipe\n");
     fflush(stdout);
     while((ch=GetLine(pip0,buff)) ) {
         line++;
#if 0
         if(!GetTimedLine(StatusGrab[0],work,500)){
		 printf("breaking got StatusGrab\n");
		 fflush(stdout);
		 break;
	 }
         if(!GetTimedLine(Jstat[0],work,500)) {
		 printf("breaking got Jstat\n");
		 fflush(stdout);
		 break;
	 }
#endif
         if(ch< 0) continue;
         if(SearchString(buff,(char *)"frame=")>=0)  {
             pos = SearchString(buff,(char *)"time=");
             if(pos>=0) {
               pt = buff+pos+5;
               pos= kgSearchString(pt,(char *)":");
               pt[pos]=' ';
               pos= kgSearchString(pt,(char *)":");
               pt[pos]=' ';
               sscanf(pt,"%f%f%f",&h,&m,&s);
               s += (h*3600+m*60);
               per = s*100/totsec;
               sprintf(work,"Cur: %f \n",per);
               write(Jpipe[1],work,strlen(work));
//      printf("%s",work);
               continue;
             }
         }
         if(SearchString(buff,(char *)"size=")>=0)  {
             pos = SearchString(buff,(char *)"time=");
             if(pos>=0) {
               pt = buff+pos+5;
               pos= kgSearchString(pt,(char *)":");
               pt[pos]=' ';
               pos= kgSearchString(pt,(char *)":");
               pt[pos]=' ';
               sscanf(pt,"%f%f%f",&h,&m,&s);
               s += (h*3600+m*60);
               per = s*100/totsec;
               sprintf(work,"Cur: %f \n",per);
               write(Jpipe[1],work,strlen(work));
      printf("%s",work);
               continue;
             }
         }
         printf("%s",buff);
	 fflush(stdout);
     }
     printf("Got %d \n",line);
     printf("Exiting ProcessTo Pipe\n");
     fflush(stdout);
     return ret;
}
int ProcessToInfo(int pip0,int pip1,int Pid) {
     char buff[1000],work[100];
     int ret =0;
     int ch,i=0,j,pos,len,OK=0;
     float per=0.0,csec,m,h,s;
     char *pt;
//     close(pip1);
     while((ch=GetLine(pip0,buff)) ) {
//         if(!GetTimedLine(StatusGrab[0],work,300)) break;
//         if(!GetTimedLine(Jstat[0],work,300)) break;
         if(ch< 0) continue;
         if(SearchString(buff,(char *)"frame=")>=0)  {
             pos = SearchString(buff,(char *)"time=");
             if(pos>=0) {
               pt = buff+pos+5;
               pos= kgSearchString(pt,(char *)":");
               pt[pos]=' ';
               pos= kgSearchString(pt,(char *)":");
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
         if(SearchString(buff,(char *)"size=")>=0)  {
             pos = SearchString(buff,(char *)"time=");
             if(pos>=0) {
               pt = buff+pos+5;
               pos= kgSearchString(pt,(char *)":");
               pt[pos]=' ';
               pos= kgSearchString(pt,(char *)":");
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
         else {
             sprintf(work,"%s\n",buff);
              write(Jpipe[1],work,strlen(work));
              continue;
         }
//         printf("%s",buff);
     }
     return ret;
}
 
int JoinToMp4( CONVDATA *cn)  {
  int Process =0;
  int pid,status,id,Qty;
  char Folder[500];
  int Audio=1;
  FILE *myl=NULL;
  Dlink *L;
  CONVDATA Cn;
  MEDIAINFO *mpt;
  Cn= *cn;
  Audio =1;
  L = (Dlink *)Cn.Vlist;
  Resetlink(L);
  while( (mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    if(mpt->Audio==0) Audio=0;
  }
  if ((pid=fork())==0) {
    char command[10000],File[500],options[5000],Fifo[500],Qstr[100];
    sprintf(Folder,"%-s/%-d",getenv("HOME"),getpid());
    if(FileStat(Folder)) kgCleanDir(Folder);
    mkdir(Folder,0700);
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
    sprintf(options,"%-s/mylist.txt",Folder);
    myl = fopen(options,"w");
    switch(Cn.Quality) {
      case 1:
        sprintf(Qstr,"3000K -crf 20  -preset medium -vcodec %s ",libvcodec);
        break;
      case 2:
        sprintf(Qstr,"2000K -crf 28 -preset fast -vcodec %s ",libvcodec);
        break;
      default:
      case 3:
        sprintf(Qstr,"1000K -crf 40 -preset superfast -vcodec %s ",libvcodec);
        break;
    }
    L = (Dlink *)Cn.Vlist;
    Process=0;
    Resetlink(L);
    while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
      if(mpt->Process != 0 ) Process=1;
    }
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
      sprintf(options,"fps = %f Xres: %d Yres: %d\n",Cn.fps,Cn.Xsize,Cn.Ysize);
      write(Jpipe[1],options,strlen(options));
        printf ("mpt->Process = %d %s  fps=%f\n", mpt->Process,mpt->Flname,mpt->fps);
	if(mpt->Process) {
          if(Audio) {
           sprintf(command,"ffmpegfun -r %-7.3f -i \"%s\" -f mp4 "
           " -video_track_timescale 90k -c:a libmp3lame -b:v %-s "
           " -s %-dx%-d -y %s/F%-4.4d ",
            Cn.fps,mpt->Flname,Qstr,Cn.Xsize,Cn.Ysize, Folder,id);
	  }
	  else {
           sprintf(command,"ffmpegfun -r %-7.3f -i \"%s\" -f mp4 "
           " -video_track_timescale 90k -an -b:v %-s "
           " -s %-dx%-d -y %s/F%-4.4d ",
            Cn.fps,mpt->Flname,Qstr,Cn.Xsize,Cn.Ysize, Folder,id);
	  }
	} // if mpt->Process
	else {
	 if(Audio) {
          sprintf(command,"ffmpegfun -i \"%s\" -f mp4  -c:a libmp3lame  -video_track_timescale 90k -c:v copy "
           " -y %s/F%-4.4d ", mpt->Flname, Folder,id);
	 }
	 else {
          sprintf(command,"ffmpegfun -i \"%s\" -f mp4  -an -video_track_timescale 90k -c:v copy "
           " -y %s/F%-4.4d ", mpt->Flname, Folder,id);
	 }
	}  // else mpt->process
        printf("%s\n",command);
        sprintf(options,"Processing %-s\n",mpt->Flname);
        write(Jpipe[1],options,strlen(options));
        runfunction(command,ProcessToPipe,ffmpegfun);
//        system(command);
        fprintf(myl,"file \'%-s/F%-4.4d\'\n",Folder,id);
//        printf("file \'%-s/F%-4.4d\'\n",Folder,id);
        fflush(myl);
//     printf("%s\n",command);
      id++;
    }
    fclose(myl);
    if(id==0) exit(0);
     Esec= Cn.EndSec;
     sprintf(options,"Esec: %lf\n",Cn.EndSec);
     write(Jpipe[1],options,strlen(options));
     sprintf(options,"Joining and Converting to mp4\n");
     write(Jpipe[1],options,strlen(options));
     if(Audio) {
       sprintf(command,"ffmpegfun -f concat -safe 0 -i \"%-s/mylist.txt\" "
        " -y  -f mp4  -video_track_timescale 90k -c:a libmp3lame  -c:v copy  "
        " \"%-s\" ", Folder ,Cn.outfile);
     }
     else {
       sprintf(command,"ffmpegfun -f concat -safe 0 -i \"%-s/mylist.txt\" "
        " -y  -f mp4  -video_track_timescale 90k -an  -c:v copy  "
        " \"%-s\" ", Folder ,Cn.outfile);
     }
        printf("\n\n\n\n%s\n\n\n",command);
     runfunction(command,ProcessToPipe,ffmpegfun);
//     runfunction(command,ProcessPrint,ffmpegfun);
     kgCleanDir(Folder);
     strcpy(options,"Joinded Video Files\n");
     write(Jpipe[1],options,strlen(options));
     close(Jpipe[1]);
     exit(0);
  }
  else {
     waitpid(pid,&status,0);
     sprintf(Folder,"%-s/%-d",getenv("HOME"),pid);
     // TCB
//     if(FileStat(Folder)) kgCleanDir(Folder);
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
  static char filename[500]="";
  ThumbNail **th;

  int n,ret =0; 
  D = (DIALOG *)Tmp;
  VX2 = (DIX *)kgGetNamedWidget(Tmp,(char *)"VideoList");
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
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
  int vcodec;
  DIT *T;
  Dlink *L;
  MEDIAINFO *mpt;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"VjoinOut");
  Of = kgGetString(T,0);
  strcpy(cndata.outfile,Of);
  L = (Dlink *)cndata.Vlist;
  cndata.Xsize = 0;
  cndata.Ysize = 0;
  cndata.fps=0.0;
  n=0;
  Qty = kgGetSelection(kgGetNamedWidget(Tmp,(char *)"VJQuality"));
  cndata.Quality = Qty;
  Resetlink(L);
  while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    n++;
    TotSec += (mpt->TotSec);
    if( mpt->fps> cndata.fps) 
          {cndata.fps = mpt->fps;}
    if( mpt->Axres> cndata.Xsize) 
          {cndata.Xsize = mpt->Axres;}
    if( mpt->Ayres> cndata.Ysize) 
          {cndata.Ysize = mpt->Ayres;}
  }
  n=0;
  Resetlink(L);
  while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
	  vcodec = mpt->vcodec;
	  if(vcodec != 2) break;
  }
  printf("======= vcodec = %d\n",vcodec);
  fflush(stdout);
  strcpy(libvcodec,(char *)"libx265");
  if(vcodec ==2) { // to  process if th libx264 if all are h264
    strcpy(libvcodec,(char *) "libx264");
    Resetlink(L);
    while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
      mpt->vcodec =1;
    }
  }
  n=0;
  Resetlink(L);
  while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    n++;
    mpt->Process=0;
    if( mpt->Axres != cndata.Xsize) {mpt->Process=1;}
    if( mpt->Ayres != cndata.Ysize) {mpt->Process=1;}
  //  if( mpt->Rxres != cndata.Xsize) {mpt->Process=1;}
//    if( mpt->Ryres != cndata.Ysize) {mpt->Process=1;}
    if( (int)(mpt->fps*1000)!= (int)(cndata.fps*1000)) mpt->Process=1;
    if(mpt->vcodec != 1) mpt->Process=1;
  }
//  printf("fps = %f\n",cndata.fps);
  cndata.Fcount=n;
  cndata.EndSec=TotSec;
  if(vcodec ==2) { // putting it back
    Resetlink(L);
    while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
      mpt->vcodec =2;
    }
  }
#if 1  
  Resetlink(L);
  while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    printf("=======>> %s : %d %d %d %f\n",mpt->Flname,mpt->Process,mpt->Rxres,mpt->Ryres,mpt->fps);
  }
  printf("%d %d %f\n",cndata.Xsize,cndata.Ysize,cndata.fps);
#endif
  cndata.code=3;
  cndata.Eol='\n';
  sprintf(buff,"%d \"%-s\" %d %d %d %f %d %f\n",
       cndata.code, cndata.outfile,cndata.Xsize,
       cndata.Ysize,cndata.Fcount,cndata.fps,Qty,TotSec);
#if 0
  write(ToTools[1],buff,strlen(buff));
  n=0;
  Resetlink(L);
  while ((mpt=(MEDIAINFO *)Getrecord(L))!= NULL) {
    sprintf(buff,"\"%-s\" %d %f %d\n",
       mpt->Flname,mpt->Process,mpt->TotSec,mpt->Audio);
    write(StatusTools[1],buff,strlen(buff));
  }
#else
  JoinToMp4(&cndata);
#endif
 
  switch(butno) {
    case 1: 
      ret=1;
      break;
  }
  kgSplashMessage(Tmp,100,100,300,40,(char *)"Send for Processing",1,0,15);
  ret=0;
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
