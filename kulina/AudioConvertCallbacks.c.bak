#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <kulina.h>
#include <fcntl.h>
#include "ConvertData.h"
#include "mediainfo.h"
extern MEDIAINFO Minfo;
extern int Tools;
extern CONVDATA cndata;
extern int AConGrp,RangeGrp,EnVoGrp;
extern int ToTools[2],FromTools[2],StatusTools[2];
extern int MonPipe;
double Ssec,Esec;
extern int Jpipe[2];
extern int Jstat[2];

extern char GrabFileName[300];
extern int Pval;
int CheckMedia(char *);
int kgLame(int,char **);
int kgffmpeg(int,char **);
int ffmpegfun(int,char **);
int Mplayer(int,char **);
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int ProcessToPipe(int pip0,int pip1,int Pid) ;
int ProcessSkip(int pip0,int pip1,int Pid) ;
int ProcessPrint(int pip0,int pip1,int Pid) ;
void *RunMonitorJoin(void *arg);
void *Runmonitor(void *arg);
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int GetBaseIndex(char *s);
int FileStat(char *flname);

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

int MakeOutputFile(char *Infile,char *Outfile,char *ext) {
   int index,i;
   char buff[500],*pt;
   int id=0;
   index = GetBaseIndex(Infile);
   strcpy(buff,Infile+index);
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
int  AudioConverttextbox1callback(int cellno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"InputWidget");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"OutputWidget");
  strcpy(FileName,kgGetString(T,0));
#if 0
  sprintf(OutFile,"%-s/Music/",getenv("HOME"));
  MakeOutputFile(FileName,OutFile+strlen(OutFile),"mp3");
#else
  sprintf(OutFile,"%-s/Music",getenv("HOME"));
  MakeFileInFolder(FileName,OutFile,OutFile,(char *)"mp3");
#endif
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  strcpy(cndata.infile,FileName);
  strcpy(cndata.outfile,OutFile);
  return ret;
}
int  AudioConvertbutton1callback(int butno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"InputWidget");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"OutputWidget");
  n = B->nx*B->ny;
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
//  kgFolderBrowser(NULL,100,100,FileName,"*");
  if(!FolderBrowser(FileName))return 0;
  kgSetString(T,0,FileName);
#if 0
  sprintf(OutFile,"%-s/Music/",getenv("HOME"));
  MakeOutputFile(FileName,OutFile+strlen(OutFile),"mp3");
#else
  sprintf(OutFile,"%-s/Music",getenv("HOME"));
  MakeFileInFolder(FileName,OutFile,OutFile,"mp3");
#endif
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
void  AudioConvertbutton1init(DIN *B,void *pt) {
}
int  AudioConverttextbox2callback(int cellno,int i,void *Tmp) {
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
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"InputWidget");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"OutputWidget");
  strcpy(OutFile,kgGetString(TO,0));
  strcpy(cndata.outfile,OutFile);
  return ret;
}
int  AudioConvertbrowser1callback(int item,int i,void *Tmp) {
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
  cndata.Quality=item;
  return ret;
}
void  AudioConvertbrowser1init(DIRA *R,void *pt) {
}
int  AudioConvertbrowser2callback(int item,int i,void *Tmp) {
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
  if(item==1) cndata.FullRange=1;
  else cndata.FullRange=0;
  if(item==2) {
    kgSetGrpVisibility(Tmp,RangeGrp,1);
  }
  else {
    kgSetGrpVisibility(Tmp,RangeGrp,0);
  }
  kgUpdateOn(Tmp);
  return ret;
}
void  AudioConvertbrowser2init(DIRA *R,void *pt) {
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
int ProcessMp3Conversion(int pip0,int pip1,int Pid) {
     int ch,Asp;
     char buff[1000],Fifo[100];
     float TotSec=0.,CurSec=0.;
     Tools=1;
     TotSec=0;
     Pval=0;
     sprintf(Fifo,"Fifo%-d",getpid());
     MonPipe=pip0;
#if 0
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
int ConvertToMp3( CONVDATA *cn) {
  int pid,status;
  CONVDATA Cn;
  Cn= *cn;
  Ssec= cn->StartSec;
  Esec= cn->EndSec;
  if(!CheckMedia(cn->infile)){
       return 0;
  }
  if(Esec < 0) {
    cn->EndSec = Minfo.TotSec +Esec;
    Esec= cn->EndSec;
  }
  if(Esec > Minfo.TotSec) {
    cn->EndSec = Minfo.TotSec ;
    Esec= cn->EndSec;
  }
  
  strcpy(GrabFileName,cn->outfile);
  fflush(stdout);
  fflush(stderr);
  if ((pid=fork())==0) {
    char command[500],Str1[500],Str2[500],options[500],Qstr[500];
     options[0] ='\0';
     Qstr[0]='\0';
     sprintf(Str1,"Quality as original\n");
     if((kgSearchString(Cn.outfile,(char *)".mp3")>= 0)||
          (kgSearchString(Cn.outfile,(char *)".MP3")>= 0)) {
       switch(cn->Quality) {
         case 1:
          strcat(Qstr," -aq 0 ");
          sprintf(Str1,"Quality Very Good\n");
          break;
         case 2:
          strcat(Qstr," -aq 2 ");
          sprintf(Str1,"Quality Good\n");
          break;
         case 3:
          strcat(Qstr," -aq 4 ");
          sprintf(Str1,"Quality Normal\n");
          break;
        }
     }
     if((kgSearchString(Cn.outfile,(char *)".aac")>= 0)) {
       sprintf(Qstr," -c:a libfdk_aac ");
     }
     sprintf(Str2,"Volume as original\n");
     if(fabs(cn->Enhfac-1.0)> 0.1) {
       switch(cn->VolEnh) {
         case 1:
          sprintf(Str2,"  -af \"pan=stereo|c0=%-lf*c0|c1=c1\" %s"
             ,cn->Enhfac,Qstr);
          strcpy(Qstr,Str2);
          sprintf(Str2,"Volume left:%6.2lf right: normal\n",cn->Enhfac);
          break;
         case 2:
          sprintf(Str2,"  -af \"pan=stereo|c0=c0|c1=%-lf*c1\" %s"
             ,cn->Enhfac,Qstr);
          strcpy(Qstr,Str2);
          sprintf(Str2,"Volume left: normal right: %6.2lf\n",cn->Enhfac);
          break;
         case 3:
          sprintf(Str2,"  -af \"pan=stereo|c0=%-lf*c0|c1=%-lf*c1\" %s"
            ,cn->Enhfac,cn->Enhfac,Qstr);
          strcpy(Qstr,Str2);
          sprintf(Str2,"Volume left: %6.2lf right: %6.2lf\n",
            cn->Enhfac,cn->Enhfac);
          break;
       }
     }
      if(cn->FullRange != 1) {
        int sh,sm,ss,smil,secs,eh,em,es,emil;
        float frac;
        secs = cn->StartSec;
        smil = (cn->StartSec - secs)*1000;
        ss = secs%60;
        secs = secs/60;
        sm = secs%60;
        sh = secs/60;
        if(cn->EndSec <=cn->StartSec ) cn->EndSec=0;
        secs = cn->EndSec-cn->StartSec;
        emil = (cn->EndSec-cn->StartSec -secs)*1000;
        es = secs%60;
        secs = secs/60;
        em = secs%60;
        eh = secs/60;
      } 
      if(pipe(Jpipe) < 0) exit(0);
      if(pipe(Jstat) < 0) exit(0);
      MonPipe = Jpipe[0];
      fflush(stdout);
      fflush(stderr);
      if( fork()==0) {
        MonPipe = Jpipe[0];
        close(Jpipe[1]);
        close(Jstat[0]);
        RunMonitorJoin(NULL);
	printf("Exit RunMonitorJoin\n");
        exit(0);
      }
      close(Jpipe[0]);
      close(Jstat[1]);
      if(cn->EndSec > 0) {
         sprintf(options,"Esec: %f\n",(float)(Esec-Cn.StartSec));
         write(Jpipe[1],options,strlen(options));
         sprintf(options,"Converted Media Length: %f\n",
                             (float)(Esec-Cn.StartSec));
         write(Jpipe[1],options,strlen(options));
         sprintf(command,"ffmpegfun -nostdin -i \"%-s\" -vn -t %-8.2lf -ss %-8.2lf -ac 2 %s -y \"%-s\"",
             Cn.infile,Esec-Cn.StartSec,Cn.StartSec,Qstr,Cn.outfile);
      }
      else {
         sprintf(options,"Esec: %f\n",(float)(Minfo.TotSec-Cn.StartSec));
         sprintf(options,"Converted Media Length: %f\n",
                          (float)(Minfo.TotSec-Cn.StartSec));
         write(Jpipe[1],options,strlen(options));
         sprintf(command,"ffmpegfun -nostdin -i \"%-s\" -vn  -ss %-8.2lf -ac 2 %s -y \"%-s\"",
             Cn.infile,Cn.StartSec,Qstr,Cn.outfile);
      }
      printf("%s\n",command);
      fflush(stdout);
      write(Jpipe[1],Str1,strlen(Str1));
      write(Jpipe[1],Str2,strlen(Str2));
      sprintf(Str1,"!c01Output format is selected as per file extension\n");
      write(Jpipe[1],Str1,strlen(Str1));
      sprintf(Str1,"!c01If CUT and JOIN is planned !z54.wav/.flac!z45 is better\n");
      write(Jpipe[1],Str1,strlen(Str1));
#if 1
      runfunction(command, ProcessToPipe ,ffmpegfun);
#else
      runfunction(command, ProcessPrint ,ffmpegfun);
#endif
    exit(0);
  }
  else {
     waitpid(pid,&status,0);
  }
}
int ConvertToMp3_o( CONVDATA *cn) {
  int pid,status;
  CONVDATA Cn;
  Cn= *cn;
  Ssec= cn->StartSec;
  Esec= cn->EndSec;
  if(!CheckMedia(cn->infile)){
       return 0;
  }
  if(Esec < 0) {
    cn->EndSec = Minfo.TotSec +Esec;
    Esec= cn->EndSec;
  }
  if(Esec > Minfo.TotSec) {
    cn->EndSec = Minfo.TotSec ;
    Esec= cn->EndSec;
  }
  
  strcpy(GrabFileName,cn->outfile);
  if ((pid=fork())==0) {
    char command[500],Fifo[100],options[100];
    sprintf(Fifo,"/tmp/Fifo%-d",getpid());
    mkfifo(Fifo,0600);
    if ((pid=fork())==0) {
     options[0] ='\0';
     if(fabs(cn->Enhfac-1.0)> 0.1) {
       switch(cn->VolEnh) {
         case 1:
          sprintf(options," --scale-l %lf ",cn->Enhfac);
          break;
         case 2:
          sprintf(options," --scale-r %lf ",cn->Enhfac);
          break;
         case 3:
          sprintf(options," --scale %lf ",cn->Enhfac);
          break;
       }
     }
     switch(cn->Quality) {
       case 1:
          strcat(options," -V0 -q 0 ");
          break;
       case 2:
          strcat(options," -V2 -q 2 ");
          break;
       case 3:
          strcat(options," -V4 -q 4 ");
          break;
     }
     sprintf(command,"kglame --quiet %-s  %-s \"%-s\"",options,Fifo,Cn.outfile); 
//     printf("%s\n",command);
     runfunction(command,NULL,kgLame);
//     fprintf(stderr,"Lame over\n");
     exit(0);
    }
    else {
      options[0] ='\0';
      if(cn->FullRange != 1) {
        int sh,sm,ss,smil,secs,eh,em,es,emil;
        float frac;
        secs = cn->StartSec;
        smil = (cn->StartSec - secs)*1000;
        ss = secs%60;
        secs = secs/60;
        sm = secs%60;
        sh = secs/60;
        if(cn->EndSec <=cn->StartSec ) cn->EndSec=0;
        secs = cn->EndSec-cn->StartSec;
        emil = (cn->EndSec-cn->StartSec -secs)*1000;
        es = secs%60;
        secs = secs/60;
        em = secs%60;
        eh = secs/60;
        if(cn->EndSec > 0) {
          sprintf(options," -ss %-d:%-d:%-d.%-d -endpos %-d:%-d:%-d.%-d",
              sh,sm,ss,smil,eh,em,es,emil);
        }
        else {
          sprintf(options," -ss %-d:%-d:%-d.%-d ",
              sh,sm,ss,smil);
        }
      }
      sprintf(command,"Mplayer -vo null -ao pcm:file=%-s %s \"%-s\"",
             Fifo,options,Cn.infile);
//      printf("%s\n",command);
      runfunction(command,ProcessMp3Conversion,Mplayer);
//      close(open(Fifo,O_RDONLY));
//      fprintf(stderr,"Removing Fifo\n");
      remove(Fifo);
      waitpid(pid,&status,0);
    }
    exit(0);
  }
  else {
     waitpid(pid,&status,0);
  }
}
 
int  AudioConvertsplbutton1callback(int butno,int i,void *Tmp) {
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
  DIT *T,*TO,*TR;
  T = (DIT *)kgGetNamedWidget(Tmp,(char *)"InputWidget");
  TO = (DIT *)kgGetNamedWidget(Tmp,(char *)"OutputWidget");
  TR = (DIT *)kgGetNamedWidget(Tmp,(char *)"RangeWidget");
  strcpy(cndata.outfile,kgGetString(TO,0));
  strcpy(cndata.infile,kgGetString(T,0));
  n=0;
  while(cndata.infile[n]==' ') n++;
  if(cndata.infile[n]< ' ') return 0;
  n=0;
  while(cndata.outfile[n]==' ') n++;
  if(cndata.outfile[n]< ' ') return 0;
  n = B->nx;
  cndata.StartSec= kgGetDouble(TR,0);
  cndata.EndSec= kgGetDouble(TR,1);
  cndata.code=1;
  cndata.Eol='\n';
  sprintf(buff,"%d \"%-s\" \"%-s\" %d %d %lf %d %lf %lf \n",
       cndata.code, cndata.infile,cndata.outfile,cndata.Quality,
       cndata.VolEnh,cndata.Enhfac,cndata.FullRange,
       cndata.StartSec,cndata.EndSec);
  write(ToTools[1],buff,strlen(buff));
  kgSplashMessage(Tmp,100,100,300,40,(char *)"Send for Processing",1,0,15);
  ret = 0;
  return ret;
}
void  AudioConvertsplbutton1init(DIL *B,void *pt) {
}
int AudioConvertinit(void *Tmp) {
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
int AudioConvertcleanup(void *Tmp) {
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
int AudioConvertCallBack(void *Tmp,void *tmp) {
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
int AudioConvertResizeCallBack(void *Tmp) {
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
int AudioConvertWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
