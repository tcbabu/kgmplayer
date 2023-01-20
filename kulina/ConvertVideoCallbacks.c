#include <kulina.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ConvertData.h"
#include "mediainfo.h"
#include "kgutils.h"

extern CONVDATA cndata;
extern int AConGrp,RangeGrp,EnVoGrp,VConGrp,VaspGrp,VsizeGrp,VrangeGrp;
extern int ToTools[2],FromTools[2],StatusTools[2];
extern int MonPipe;
extern double Ssec,Esec;
extern MEDIAINFO Minfo;
extern int Tools;

extern char GrabFileName[300];
extern int Pval;
int CheckMedia(char *);
int kgLame(int,char **);
int kgffmpeg(int,char **);
int ffmpegfun(int,char **);
int Mplayer(int,char **);
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));

int MakeOutputFile(char *Infile,char *Outfile,char *ext);
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int FileStat(char *flname) {
  int ret;
  struct stat buff;
  ret = stat(flname,&buff);
  if(ret < 0) return 0;
  else return 1;
}
int MakeMp4File(char *Infile,char *Outfile,int id) {
   int index,i;
   char buff[500],work[10];
   index = GetBaseIndex(Infile);
   strcpy(buff,Infile+index);
   i=0;
   while (buff[i]!='.') {
     if(buff[i]< ' ') break;
     if(buff[i]==' ') buff[i]='_';
     if(i>30) break;
     i++;
   }
   buff[i]='\0';
   sprintf(work,"_%4.4d",id);
   strcat(buff,work);
   i +=5;
   buff[i]='.';
   i++;
   buff[i]='\0';
   strcat(buff,"mp4");
   strcpy(Outfile,buff);
   return 1;
}
int  ConvertVideotextbox1callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  char FileName[500],OutFile[500];
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1,id;
  D = (DIALOG *)Tmp;
  DIT *TO;
  T = (DIT *)kgGetWidget(Tmp,i);
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"VOutputWidget");
  strcpy(FileName,kgGetString(T,0));
  if(CheckMedia(FileName) == 0) {
    return 0;
  }
#if 0
  sprintf(OutFile,"%-s/Video/",getenv("HOME"));
  MakeOutputFile(FileName,OutFile+strlen(OutFile),"mp4");
#else
  sprintf(OutFile,"%-s/Video",getenv("HOME"));
  MakeFileInFolder(FileName,OutFile,OutFile,(char *)"mp4");
#endif
  id =1;
  while(FileStat(OutFile) ) {
     sprintf(OutFile,"%-s/Video/",getenv("HOME"));
     MakeMp4File(FileName,OutFile+strlen(OutFile),id);
     id++;
  }
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  strcpy(cndata.infile,FileName);
  strcpy(cndata.outfile,OutFile);
  e = T->elmt;
  return ret;
}
int  ConvertVideobutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  char FileName[500],OutFile[500],buf[300];
  DIALOG *D;DIN *B; 
  int n,ret =0,id; 
  DIM *Msg;
  D = (DIALOG *)Tmp;
  DIT *T,*TO;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"VInputWidget");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"VOutputWidget");
  B = (DIN *)kgGetWidget(Tmp,i);
  Msg = (DIM *) kgGetNamedWidget(Tmp,(char *) "ConvertMsg");
  if( Msg == NULL) printf("=============> Failed to get Message Box\n");
  n = B->nx*B->ny;
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
  kgFolderBrowser(NULL,100,100,FileName,(char *)"*");
  if(CheckMedia(FileName) == 0) {
    sprintf(buf," NOT A VIDEO FILE");
    kgWrite(Msg,buf);
    kgUpdateOn(Tmp);
    return 0;
  }
  sprintf(buf," Video %dx%d fps: %f ",Minfo.Axres,Minfo.Ayres,Minfo.fps);
  if(Minfo.vcodec == 1) strcat(buf," H265");
  if(Minfo.vcodec == 2) strcat(buf," H264");
  kgWrite(Msg,buf);
  kgUpdateOn(Tmp);

  cndata.Xsize = Minfo.Axres;
  cndata.Ysize = Minfo.Ayres;
  kgSetString(T,0,FileName);
#if 0
  sprintf(OutFile,"%-s/Video/",getenv("HOME"));
  MakeOutputFile(FileName,OutFile+strlen(OutFile),"mp4");
#else
  sprintf(OutFile,"%-s/Video",getenv("HOME"));
  MakeFileInFolder(FileName,OutFile,OutFile,(char *)"mp4");
#endif
  id =1;
  while(FileStat(OutFile) ) {
     sprintf(OutFile,"%-s/Video/",getenv("HOME"));
     MakeMp4File(FileName,OutFile+strlen(OutFile),id);
     id++;
  }
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  strcpy(cndata.infile,FileName);
  strcpy(cndata.outfile,OutFile);
  return ret;
}
void  ConvertVideobutton1init(DIN *B,void *pt) {
}
int  ConvertVideotextbox2callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;
  char FileName[500],OutFile[500];
  int ret=1;
  D = (DIALOG *)Tmp;
  DIT *T,*TO;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"VInputWidget");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"VOutputWidget");
  strcpy(OutFile,kgGetString(TO,0));
  strcpy(cndata.outfile,OutFile);
  return ret;
}
int  ConvertVideobrowser1callback(int item,int i,void *Tmp) {
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
  cndata.VQuality=item;
  return ret;
}
void  ConvertVideobrowser1init(DIRA *R,void *pt) {
}
int  ConvertVideobrowser2callback(int item,int i,void *Tmp) {
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
  if(item==1) {
    cndata.ChngAsp=0;
    kgSetGrpVisibility(Tmp,VaspGrp,0);
  }
  else {
    cndata.ChngAsp=1;
    kgSetGrpVisibility(Tmp,VaspGrp,1);
  }
  kgUpdateOn(Tmp);
  return ret;
}
void  ConvertVideobrowser2init(DIRA *R,void *pt) {
}
int  ConvertVideobrowser3callback(int item,int i,void *Tmp) {
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
  if(item==1) {
    cndata.Scale=0;
    kgSetGrpVisibility(Tmp,VsizeGrp,0);
  }
  else {
    cndata.Scale=1;
    kgSetGrpVisibility(Tmp,VsizeGrp,1);
  }
  kgUpdateOn(Tmp);

  return ret;
}
void  ConvertVideobrowser3init(DIRA *R,void *pt) {
}
static int CheckString(char *s1,char *s2) {
   int ch,ln,ret=0;
   ln = strlen(s2);
   if(strlen(s1)<ln) ret=0;
   else {
     ch = s1[ln];
     s1[ln]='\0';
     if( strcmp(s1,s2)==0 ) ret=1;
     s1[ln]=ch;
   }
   return ret;
}
int ProcessMp4Conversion(int pip0,int pip1,int Pid) {
     int ch,Asp;
     char buff[1000],Fifo[100];
     float TotSec=0.,CurSec=0.;
     MonPipe=pip0;
#if 0
     TotSec=0;
//     sprintf(Fifo,"Fifo%-d",getpid());
     while((ch=GetLine(pip0,buff)) ) {
//         printf("%s\n",buff);
//         fflush(stdout);
         if(ch< 0) continue;
         if(buff[0]==' ') continue;
         if(CheckString(buff,(char *)"T:")) {
               sscanf(buff+2,"%f",&(TotSec));
               fprintf(stderr,"Totsec= %f\n",TotSec);
               continue;
         }
         if(CheckString(buff,(char *)"A:")) {
               sscanf(buff+2,"%f",&(CurSec));
               fprintf(stderr,"Totsec= %f %f\n",TotSec,CurSec);
               if(TotSec!=0 ) {
                 Pval = CurSec/TotSec*100.0;
               }
               continue;
         }
     }
#else
     Runmonitor(NULL);
#endif
//     fprintf(stderr,"mplayer over\n");
     return 1;
}
int ConvertToMp4( CONVDATA *cn) {
  int pid,status;
  int len;
  CONVDATA Cn;
  Cn= *cn;
  Ssec= Cn.VStartSec;
  Esec= Cn.VEndSec;
  if(!CheckMedia(Cn.infile)){
       return 0;
  }
 
  Cn.OldAsp = Minfo.AspectNu/Minfo.AspectDe;
  if(Esec < 0) {
    Cn.VEndSec = Minfo.TotSec +Esec;
    Esec= Cn.VEndSec;
  }
  if(Esec > Minfo.TotSec) {
    Cn.VEndSec = Minfo.TotSec ;
    Esec= Cn.VEndSec;
  }
  if(Esec==0.0) {
     Esec= Minfo.TotSec ;
  }
  
  strcpy(GrabFileName,Cn.outfile);
  if ((pid=fork())==0) {
    char command[500],Fifo[100],options[300],
         options1[300],options2[300];
      options[0] ='\0';
      if(Cn.FullRange != 1) {
        int sh,sm,ss,smil,secs,eh,em,es,emil;
        float frac;
        if(Cn.VEndSec > 0) {
          sprintf(options," -ss %-lf  -t %-lf ",
              Cn.VStartSec,(Cn.VEndSec-Cn.VStartSec));
        }
        else {
          sprintf(options," -ss %-lf ",
              Cn.VStartSec);
        }
      }
      options1[0]='\0';
// In fact formats other tham mp4 can be tried but not opened
//      strcpy(options1,(char *)"-f mp4 -vcodec libx264 ");
      switch(Cn.VQuality) {
        case 1:
//          strcat(options1,"  -b:v 3000K -aq 0 -c:a libmp3lame ");
          strcpy(options1,(char *)"-f mp4 -vcodec libx265 -crf 16 -preset medium  -video_track_timescale 90k");
          strcat(options1,"   -b:v 3000K -aq 0 -c:a libmp3lame ");
          break;
        case 2:
          strcpy(options1,(char *)"-f mp4 -vcodec libx265 -crf 24 -preset medium  -video_track_timescale 90k");
          strcat(options1,"  -b:v 2000K -aq 0 -c:a libmp3lame ");
          break;
        case 3:
          strcpy(options1,(char *)"-f mp4 -vcodec libx264 -crf 28 -preset fast  -video_track_timescale 90k");
          strcat(options1,"  -b:v 1000K -aq 2 -c:a libmp3lame ");
          break;
	default:
          strcpy(options1,(char *)"-f mp4 -vcodec libx264 -crf 32 -preset veryfast  -video_track_timescale 90k");
          strcat(options1,"  -b:v 800K -aq 2 -c:a libmp3lame ");
	  break;  
      }
      len = strlen(options1);
      if(Cn.ChngAsp) {
        sprintf(options1+len," -aspect 16:%-d ",
                     (int)(16./Cn.NewAsp+0.5));
      }
      else Cn.NewAsp= Cn.OldAsp;
      options2[0]='\0';
      len = strlen(options1);
      if(Cn.Scale) {
        Cn.Ysize = Cn.Xsize/Cn.NewAsp+0.1;
        Cn.Ysize = (Cn.Ysize/4)*4;
        
        sprintf(options1+len," -s %-dx%-d ", Cn.Xsize,Cn.Ysize);
      }
      else {Cn.Xsize = Minfo.Rxres; Cn.Ysize= Minfo.Ryres;}

      sprintf(command,"ffmpegfun %-s -i \"%-s\" -y %-s \"%-s\"",
             options,Cn.infile,options1,Cn.outfile);
//      printf("%s\n",command);
      runfunction(command,ProcessMp4Conversion,ffmpegfun);
//      close(open(Fifo,O_RDONLY));
//      fprintf(stderr,"Removing Fifo\n");
    exit(0);
  }
  else {
     waitpid(pid,&status,0);
  }
}
 
int  ConvertVideosplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
   char buff[5000];
  DIALOG *D;DIL *B;
  int n,ret=1;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  DIT *T,*TO,*TR1,*TR2,*TA,*TS;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"VInputWidget");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"VOutputWidget");
  TR1 = (DIT *)kgGetNamedWidget(Tmp,(char *)"VRangeWidget1");
  TR2 = (DIT *)kgGetNamedWidget(Tmp,(char *)"VRangeWidget2");
  TA = (DIT *)kgGetNamedWidget(Tmp,(char *)"VaspWidget");
  TS = (DIT *)kgGetNamedWidget(Tmp,(char *)"VsizeWidget");
  strcpy(cndata.outfile,kgGetString(TO,0));
  strcpy(cndata.infile,kgGetString(T,0));
  n=0;
  while(cndata.infile[n]==' ') n++;
  if(cndata.infile[n]< ' ') return 0;
  n=0;
  while(cndata.outfile[n]==' ') n++;
  if(cndata.outfile[n]< ' ') return 0;
  if(!CheckMedia(cndata.infile)){
     if(!kgCheckMenu(Tmp,10,10,(char *)"Wrong Media, Modify?",0)) return 1;
     else  return 0;
  }
  if(FileStat(cndata.outfile)) {
     if(!kgCheckMenu(Tmp,10,10,(char *)"File Exits, Overwrite ?",0)) return 0;
  }
  n = B->nx;
  cndata.VStartSec= kgGetDouble(TR1,0);
  cndata.VEndSec= kgGetDouble(TR2,0);
  cndata.NewAsp= kgGetDouble(TA,0);
  cndata.Xsize= kgGetInt(TS,0);
  cndata.code=2;
  cndata.Eol='\n';
  sprintf(buff,"%d \"%-s\" \"%-s\" %d %lf %d %d %lf %lf %d %d\n",
       cndata.code, cndata.infile,cndata.outfile,cndata.VQuality,
       cndata.NewAsp,cndata.Xsize,cndata.VFullRange,
       cndata.VStartSec,cndata.VEndSec,cndata.ChngAsp,cndata.Scale);
  write(ToTools[1],buff,strlen(buff));
  kgSplashMessage(Tmp,100,100,300,40,(char *)"Send for Processing",1,0,15);
  ret = 0;

  return ret;
}
void  ConvertVideosplbutton1init(DIL *B,void *pt) {
}
int  ConvertVideobrowser4callback(int item,int i,void *Tmp) {
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
  if(item==1) cndata.VFullRange=1;
  else cndata.VFullRange=0;
  if(item==2) {
    kgSetGrpVisibility(Tmp,VrangeGrp,1);
  }
  else {
    kgSetGrpVisibility(Tmp,VrangeGrp,0);
  }
  kgUpdateOn(Tmp);
  return ret;
}
void  ConvertVideobrowser4init(DIRA *R,void *pt) {
}
int ConvertVideoinit(void *Tmp) {
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
int ConvertVideocleanup(void *Tmp) {
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
int ConvertVideoCallBack(void *Tmp,void *tmp) {
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
int ConvertVideoResizeCallBack(void *Tmp) {
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
int ConvertVideoWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
