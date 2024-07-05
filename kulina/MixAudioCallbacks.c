#include <kulina.h>
#include "ConvertData.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mediainfo.h"
extern CONVDATA cndata;
extern int AConGrp,RangeGrp,EnVoGrp,VConGrp,
    VaspGrp,VsizeGrp,VrangeGrp,VJGrp,AJGrp,AddGrp,MixGrp,AmixGrp,KarGrp;
extern int ToTools[2],FromTools[2],StatusTools[2];
extern int StatusGrab[2];
extern int MonPipe;
extern double Ssec,Esec;
extern MEDIAINFO Minfo;
extern int Tools;
extern int Jpipe[2];
extern int Jstat[2];

int ResetGrpVis(void *);
int FileStat(char *flname);
int CheckVideo(char *flname);
int CheckMedia(char *flname);
int FileStat(char *);
extern char GrabFileName[300];
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
char * MakeMixFile(void) {
  char buff[500],*pt;
  int id=0,ln;
  sprintf(buff,"%-s/Video/",getenv("HOME"));
  ln = strlen(buff);
  pt = buff+ln;
  while(1) {
    sprintf(pt,"MixedVideo_%-4.4d.mp4",id);
//    printf("%s\n",buff);
    if (!FileStat(buff)) break;
    id++;
  }
  ln = strlen(buff);
  pt = (char *)malloc(ln+1);
  strcpy(pt,buff);
  return pt;
}
char * MakeAudioMixFile(void) {
  char buff[500],*pt;
  int id=0,ln;
  sprintf(buff,"%-s/Music/",getenv("HOME"));
  ln = strlen(buff);
  pt = buff+ln;
  while(1) {
    sprintf(pt,"MixedAudio_%-4.4d.mp3",id);
//    printf("%s\n",buff);
    if (!FileStat(buff)) break;
    id++;
  }
  ln = strlen(buff);
  pt = (char *)malloc(ln+1);
  strcpy(pt,buff);
  return pt;
}
int GetWavHeaderLength(char *flname) {
  char buff[512];
  FILE *fp;
  int ln=0,i;
  fp = fopen(flname,"r");
  if(fp != NULL) {
    if(fread(buff,1,256,fp)==256) {
      if((buff[0]=='R')&&(buff[1]=='I') &&
         (buff[2]=='F') &&(buff[3]=='F')) {
         if((buff[8]=='W')&&(buff[9]=='A') &&
           (buff[10]=='V') &&(buff[11]=='E')) {
             for(i=12;i<256;i++) {
               if((buff[i+0]=='d')&&(buff[i+1]=='a') &&
                 (buff[i+2]=='t') &&(buff[i+3]=='a')) {
                 ln = i+8;
                 break;
               }
             }
         }
      }
    }
    fclose(fp);
  }
  return ln;
}
int joinaudio(char *infile1,char *infile2,char *outfile) {
  char buff[4098];
  union HEADER {char raw[256];unsigned int data[64];} header1,header2;
  FILE *fp,*fp2,*of;
  int ln1,ln2;
  char *dpt;
  int *ipt;
  int ir,i,ch,pos;
  unsigned int data,chnk1,chnk2;
  ln1 = GetWavHeaderLength(infile1);
  ln2 = GetWavHeaderLength(infile2);
//  printf("ln1=%d ln2=%d\n",ln1,ln2);
  if(ln1==0) return 0;
  if(ln2==0) return 0;
  fp = fopen(infile1,"r");
  if(fp==NULL) return 0;
  fread(header1.raw,1,ln1,fp);
  ipt = (int *)(header1.raw+ln1-4);
  data = *ipt;
  chnk1=header1.data[1];
#if 0  
  printf("data = %u\n",data);
  for(i=0;i<25;i++) printf("%u ",header1.data[i]);
  printf("\n");
  for(i=0;i<102;i++) printf("%d:%c",i,header1.raw[i]);
  printf("\n");
#endif
  fp2 = fopen(infile2,"r");
  if(fp2==NULL) return 0;
  fread(header2.raw,1,ln2,fp2);
#if 0
  for(i=0;i<25;i++) printf("%u ",header2.data[i]);
  printf("\n");
  for(i=0;i<102;i++) printf("%c",header2.raw[i]);
  printf("\n");
#endif
  ipt = (int *)(header2.raw+ln2-4);
  data  += (*ipt);
  chnk1 = chnk1+(*ipt);
  ipt = (int *)(header1.raw+ln1-4);
  *ipt = data;
//  printf("data = %u\n",data);
  header1.data[1]= chnk1;
  of = fopen(outfile,"w");
  fwrite(header1.raw,1,ln1,of);
  while( (ir=fread(buff,1,4098,fp))==4098) {
    fwrite(buff,1,4098,of);
  }
  if(ir> 0) fwrite(buff,1,ir,of);
  while( (ir=fread(buff,1,4098,fp2))==4098) {
      fwrite(buff,1,4098,of);
  }
  if(ir> 0) fwrite(buff,1,ir,of);
  fclose(fp2);
  fclose(fp);
  fclose(of);
  return 1;
}

int MixAudioToVideo( CONVDATA *cn) {
  int pid,status,id,Qty;
  char Folder[500];
  Dlink *L;
  CONVDATA Cn;
  MEDIAINFO *mpt;
  Cn= *cn;
  if ((pid=fork())==0) {
    char command[10000],File[500],options[5000],Fifo[500],Qstr[100];
    sprintf(Folder,"%-s/%-d",getenv("HOME"),getpid());
    double Asec,Msec;
    if(FileStat(Folder)) kgCleanDir(Folder);
    mkdir(Folder,0700);
    if(pipe(Jpipe) < 0) exit(0);
    if(pipe(Jstat) < 0) exit(0);
    sprintf(GrabFileName,"Mixing audio to : %-s\n",Cn.outfile);
    CheckMedia(Cn.audiofile);
    Asec = Minfo.TotSec;
    CheckMedia(Cn.infile);
    Esec = Minfo.TotSec;
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
        strcpy(Qstr,"3000K -aq 0");
        break;
      case 2:
        strcpy(Qstr,"2000K -aq 0");
        break;
      default:
      case 3:
        strcpy(Qstr,"1000K -aq 2");
        break;
    }
    sprintf(options,"!c01Mixing audio "
       "and converting may take time!! Pl. be patient...\n");
    write(Jpipe[1],options,strlen(options));
    sprintf(options,"Processing audios...\n");
    write(Jpipe[1],options,strlen(options));
    id =0;
    Msec = Asec;
    if(Esec< Asec) {
       Msec = Esec;
    }
    printf("%lf,%lf,%lf\n",Asec,Esec,Msec);
    sprintf(options,"Esec: %lf\n",Msec);
    write(Jpipe[1],options,strlen(options));
    sprintf(command,"ffmpegfun -vn -i \"%s\" "
       " -vn -aq 2 -ac 2 -ar 44100 -acodec pcm_s32le -t %lf -y %s/F%-4.4d.wav ",
       Cn.audiofile,Msec,Folder,0);
//       printf("%s\n",command);
    runfunction(command,ProcessToPipe,ffmpegfun);
    id=1;
    sprintf(options,"Esec: %lf\n",Msec);
    write(Jpipe[1],options,strlen(options));
    sprintf(command,"ffmpegfun -vn -i \"%s\" "
       " -vn -aq 2 -ac 2 -ar 44100 -acodec pcm_s32le -t %lf -y %s/F%-4.4d.wav ",
       Cn.infile,Msec,Folder,1);
    runfunction(command,ProcessToPipe,ffmpegfun);
//       printf("%s\n",command);
    sprintf(options,"Mixing audios...1\n");
    write(Jpipe[1],options,strlen(options));
    sprintf(options,"Esec: %lf\n",Msec);
    write(Jpipe[1],options,strlen(options));
    sprintf(command,"ffmpegfun -vn -i %s/F%-4.4d.wav -vn -i "
       " %s/F%-4.4d.wav -ac 2 -ar 44100 -acodec pcm_s32le -lavfi amix "
       "  -y %s/F%-4.4d.wav ",
       Folder,1,Folder,0,Folder,2);
//       printf("%s\n",command);
    runfunction(command,ProcessToPipe,ffmpegfun);
    sprintf(options,"Mixing audios...2\n");
    write(Jpipe[1],options,strlen(options));
    sprintf(options,"Esec: %lf\n",Msec);
    write(Jpipe[1],options,strlen(options));
    sprintf(command,"ffmpegfun  -i %s/F%-4.4d.wav -ac 2 -ar 44100 "
       " -acodec pcm_s32le -filter_complex "
       " \"aeval=val(0)/2*3|val(1)/2*3:c=same\" "
       "  -y %s/F%-4.4d.wav ",
       Folder,2,Folder,3);
//       printf("%s\n",command);
    runfunction(command,ProcessToPipe,ffmpegfun);
    id = 3;
    if(Msec == Asec) {
      char infile1[500],infile2[500],outfile[500];
      sprintf(options,"Mixing audios...3\n");
      write(Jpipe[1],options,strlen(options));
      sprintf(options,"Esec: %lf\n",Esec-Asec);
      write(Jpipe[1],options,strlen(options));
      sprintf(command,"ffmpegfun -vn -i \"%s\" -ss %lf "
       " -vn -aq 2 -ac 2 -ar 44100 -acodec pcm_s32le -y %s/F%-4.4d.wav ",
       Cn.infile,Msec,Folder,4);
//       printf("%s\n",command);
      runfunction(command,ProcessToPipe,ffmpegfun);
      sprintf(infile1,"%s/F%-4.4d.wav",Folder,3);
      sprintf(infile2,"%s/F%-4.4d.wav",Folder,4);
      sprintf(outfile,"%s/F%-4.4d.wav",Folder,5);
      printf("joining: %s %s %s\n",infile1,infile2,outfile);
      sprintf(options,"Mixing audios...4\n");
      write(Jpipe[1],options,strlen(options));
      sprintf(options,"Per: %f\n",0.0);
      write(Jpipe[1],options,strlen(options));
      joinaudio(infile1,infile2,outfile);
      id=5;
    }
    sprintf(options,"Esec: %lf\n",Esec);
    write(Jpipe[1],options,strlen(options));
    sprintf(options,"Converting to mp4\n");
    write(Jpipe[1],options,strlen(options));
    sprintf(command,"ffmpegfun -vn -i %-s/F%-4.4d.wav -an -i \"%-s\" "
        " -y -f mp4 -vcodec copy -c:a libmp3lame -ac 2 "
        " -b:v %-s -t %lf \"%-s\" ", Folder,id ,Cn.infile,Qstr,Esec,Cn.outfile);
//    printf("%s\n",command);
    runfunction(command,ProcessToPipe,ffmpegfun);
    kgCleanDir(Folder);
    close(Jpipe[1]);
    exit(0);
  }
  else {
     waitpid(pid,&status,0);
     sprintf(Folder,"%-s/%-d",getenv("HOME"),pid);
     if(FileStat(Folder)) kgCleanDir(Folder);
  }
}

int MixAudioToAudio( CONVDATA *cn) {
  int pid,status,id,Qty;
  char Folder[500];
  Dlink *L;
  CONVDATA Cn;
  MEDIAINFO *mpt;
  Cn= *cn;
  if ((pid=fork())==0) {
    char command[10000],File[500],options[5000],Fifo[500],Qstr[100];
    sprintf(Folder,"%-s/%-d",getenv("HOME"),getpid());
    double Asec,Msec;
    if(FileStat(Folder)) kgCleanDir(Folder);
    mkdir(Folder,0700);
    if(pipe(Jpipe) < 0) exit(0);
    if(pipe(Jstat) < 0) exit(0);
    sprintf(GrabFileName,"Mixing audio to : %-s\n",Cn.outfile);
    CheckMedia(Cn.audiofile);
    Asec = Minfo.TotSec;
    CheckMedia(Cn.infile);
    Esec = Minfo.TotSec;
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
        strcpy(Qstr," -aq 0");
        break;
      case 2:
        strcpy(Qstr," -aq 2");
        break;
      default:
      case 3:
        strcpy(Qstr," -aq 4");
        break;
    }
    sprintf(options,"!c01Mixing audio "
       "and converting may take time!! Pl. be patient...\n");
    write(Jpipe[1],options,strlen(options));
    sprintf(options,"Processing audios...\n");
    write(Jpipe[1],options,strlen(options));
    id =0;
    Msec = Asec;
    if(Esec< Asec) {
       Msec = Esec;
    }
//    printf("%lf,%lf,%lf\n",Asec,Esec,Msec);
    sprintf(options,"Esec: %lf\n",Msec);
    write(Jpipe[1],options,strlen(options));
    sprintf(command,"ffmpegfun -vn -i \"%s\" "
       " -vn -aq 2 -ac 2 -ar 44100 -acodec pcm_s32le -t %lf -y %s/F%-4.4d.wav ",
       Cn.audiofile,Msec,Folder,0);
//       printf("%s\n",command);
    runfunction(command,ProcessToPipe,ffmpegfun);
    id=1;
    sprintf(options,"Esec: %lf\n",Msec);
    write(Jpipe[1],options,strlen(options));
    sprintf(command,"ffmpegfun -vn -i \"%s\" "
       " -vn -aq 2 -ac 2 -ar 44100 -acodec pcm_s32le -t %lf -y %s/F%-4.4d.wav ",
       Cn.infile,Msec,Folder,1);
    runfunction(command,ProcessToPipe,ffmpegfun);
//       printf("%s\n",command);
    sprintf(options,"Mixing audios...1\n");
    write(Jpipe[1],options,strlen(options));
    sprintf(options,"Esec: %lf\n",Msec);
    write(Jpipe[1],options,strlen(options));
    sprintf(command,"ffmpegfun -vn -i %s/F%-4.4d.wav -vn -i "
       " %s/F%-4.4d.wav -ac 2 -ar 44100 -acodec pcm_s32le -lavfi amix "
       "  -y %s/F%-4.4d.wav ",
       Folder,1,Folder,0,Folder,2);
//       printf("%s\n",command);
    runfunction(command,ProcessToPipe,ffmpegfun);
    sprintf(options,"Mixing audios...2\n");
    write(Jpipe[1],options,strlen(options));
    sprintf(options,"Esec: %lf\n",Msec);
    write(Jpipe[1],options,strlen(options));
    sprintf(command,"ffmpegfun  -i %s/F%-4.4d.wav -ac 2 -ar 44100 "
       " -acodec pcm_s32le -filter_complex "
       " \"aeval=val(0)/2*3|val(1)/2*3:c=same\" "
       "  -y %s/F%-4.4d.wav ",
       Folder,2,Folder,3);
//       printf("%s\n",command);
    runfunction(command,ProcessToPipe,ffmpegfun);
    id = 3;
    if(Msec == Asec) {
      char infile1[500],infile2[500],outfile[500];
      sprintf(options,"Mixing audios...3\n");
      write(Jpipe[1],options,strlen(options));
      sprintf(options,"Esec: %lf\n",Esec-Asec);
      write(Jpipe[1],options,strlen(options));
      sprintf(command,"ffmpegfun -vn -i \"%s\" -ss %lf "
       " -vn -aq 2 -ac 2 -ar 44100 -acodec pcm_s32le -y %s/F%-4.4d.wav ",
       Cn.infile,Msec,Folder,4);
//       printf("%s\n",command);
      runfunction(command,ProcessToPipe,ffmpegfun);
      sprintf(infile1,"%s/F%-4.4d.wav",Folder,3);
      sprintf(infile2,"%s/F%-4.4d.wav",Folder,4);
      sprintf(outfile,"%s/F%-4.4d.wav",Folder,5);
//      printf("joining: %s %s %s\n",infile1,infile2,outfile);
      sprintf(options,"Mixing audios...4\n");
      write(Jpipe[1],options,strlen(options));
      sprintf(options,"Per: %f\n",0.0);
      write(Jpipe[1],options,strlen(options));
      joinaudio(infile1,infile2,outfile);
      id=5;
    }
    sprintf(options,"Esec: %lf\n",Esec);
    write(Jpipe[1],options,strlen(options));
    sprintf(options,"Converting to audio output\n");
    write(Jpipe[1],options,strlen(options));
    if(kgSearchString (Cn.outfile,(char *)".mp3")>=0 ) {
      sprintf(command,"ffmpegfun -vn -i %-s/F%-4.4d.wav "
        " -y  -c:a libmp3lame -ac 2 "
        "  %-s -t %lf \"%-s\" ", Folder,id ,Qstr,Esec,Cn.outfile);
    }
    else {
      if(kgSearchString (Cn.outfile,(char *)".aac")>=0 ) {
       sprintf(command,"ffmpegfun -vn -i %-s/F%-4.4d.wav "
        " -y  -ac 2 -c:a libfdk_aac "
        " -t %lf \"%-s\" ", Folder,id ,Esec,Cn.outfile);
      }
      else {
       sprintf(command,"ffmpegfun -vn -i %-s/F%-4.4d.wav "
        " -y  -ac 2 "
        "  %-s -t %lf \"%-s\" ", Folder,id ,Qstr,Esec,Cn.outfile);
      }
    }
//    printf("%s\n",command);
    runfunction(command,ProcessToPipe,ffmpegfun);
    kgCleanDir(Folder);
    close(Jpipe[1]);
    exit(0);
  }
  else {
     waitpid(pid,&status,0);
     sprintf(Folder,"%-s/%-d",getenv("HOME"),pid);
     if(FileStat(Folder)) kgCleanDir(Folder);
  }
}

int  MixAudiotextbox1callback(int cellno,int i,void *Tmp) {
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
int  MixAudiobutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  char FileName[500];
  int n,ret =0;
  DIT *T;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  T = (DIT *)kgGetNamedWidget(Tmp,"MixTbox1");
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
//  kgFolderBrowser(NULL,100,100,FileName,"*");
  if(!FolderBrowser(FileName))return 0;
  CheckMedia(FileName);
  if(Minfo.Audio !=0 ) {
    kgSetString(T,0,FileName);
    kgUpdateWidget(T);
    kgUpdateOn(Tmp);
  }

  return ret;
}
void  MixAudiobutton1init(DIN *B,void *pt) {
}
int  MixAudiotextbox2callback(int cellno,int i,void *Tmp) {
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
int  MixAudiobrowser1callback(int item,int i,void *Tmp) {
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
void  MixAudiobrowser1init(DIRA *R,void *pt) {
}
int  MixAudiosplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  char buff[2000];
  int n,ret=1,Qty; 
  D = (DIALOG *)Tmp;
  DIT *T,*T1,*T2,*T3;
  DIRA *R;
  B = (DIL *) kgGetWidget(Tmp,i);
  T = (DIT *)kgGetNamedWidget(Tmp,"MixTbox1");
  T1 = T;
  strcpy(cndata.audiofile,kgGetString(T,0));
//  printf("%s\n",cndata.audiofile);
  n=0;
  while(cndata.audiofile[n]==' ') n++;
  if(cndata.audiofile[n]< ' ') return 0;
  T = (DIT *)kgGetNamedWidget(Tmp,"MixTbox3");
  T2 = T;
  strcpy(cndata.infile,kgGetString(T,0));
  n=0;
  while(cndata.infile[n]==' ') n++;
  if(cndata.infile[n]< ' ') return 0;
  T = (DIT *)kgGetNamedWidget(Tmp,"MixTbox2");
  T3 = T;
  strcpy(cndata.outfile,kgGetString(T,0));
  n=0;
  while(cndata.outfile[n]==' ') n++;
  if(cndata.outfile[n]< ' ') return 0;
  R = (DIRA *) kgGetNamedWidget(Tmp,"MixQty");
  Qty = kgGetSelection(R);
//  printf("Tools= %d\n",Tools);
  cndata.code =6;
  if(Tools== 7) cndata.code = 7;
  cndata.Eol='\n';
  sprintf(buff,"%d \"%-s\" \"%-s\" \"%-s\" %d \n",
       cndata.code, cndata.audiofile,cndata.infile,cndata.outfile,Qty);
  write(ToTools[1],buff,strlen(buff));
  kgSplashMessage(Tmp,100,100,300,40,"Send for Processing",1,0,15);
  ret =0;
  kgSetString(T1,0,(char *)"");
  kgSetString(T2,0,(char *)"");
  ResetGrpVis(Tmp);
  return ret;
}
void  MixAudiosplbutton1init(DIL *B,void *pt) {
}
int  MixAudiotextbox3callback(int cellno,int i,void *Tmp) {
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
int  MixAudiobutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  
  int n,ret =0; 
  char FileName[500];
  D = (DIALOG *)Tmp;
  DIT *T;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  T = (DIT *)kgGetNamedWidget(Tmp,"MixTbox3");
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
//  kgFolderBrowser(NULL,100,100,FileName,"*");
  if(!FolderBrowser(FileName))return 0;
  CheckMedia(FileName);
  if((Minfo.Video !=0)||(Minfo.Audio !=0) ) {
    kgSetString(T,0,FileName);
    kgUpdateWidget(T);
    kgUpdateOn(Tmp);
  }
  return ret;
}
void  MixAudiobutton2init(DIN *B,void *pt) {
}
int MixAudioinit(void *Tmp) {
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
int MixAudiocleanup(void *Tmp) {
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
int MixAudioCallBack(void *Tmp,void *tmp) {
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
int MixAudioResizeCallBack(void *Tmp) {
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
int MixAudioWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
/* Amix modifications */
int  AmixAudiotextbox1callback(int cellno,int i,void *Tmp) {
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
int  AmixAudiobutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  char FileName[500];
  int n,ret =0;
  DIT *T;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  T = (DIT *)kgGetNamedWidget(Tmp,"AmixTbox1");
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
//  kgFolderBrowser(NULL,100,100,FileName,"*");
  if(!FolderBrowser(FileName))return 0;
  CheckMedia(FileName);
  if(Minfo.Audio !=0 ) {
    kgSetString(T,0,FileName);
    kgUpdateWidget(T);
    kgUpdateOn(Tmp);
  }

  return ret;
}
void  AmixAudiobutton1init(DIN *B,void *pt) {
}
int  AmixAudiotextbox2callback(int cellno,int i,void *Tmp) {
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
int  AmixAudiobrowser1callback(int item,int i,void *Tmp) {
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
void  AmixAudiobrowser1init(DIRA *R,void *pt) {
}
int  AmixAudiosplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  char buff[2000];
  int n,ret=1,Qty; 
  D = (DIALOG *)Tmp;
  DIT *T,*T1,*T2,*T3;
  DIRA *R;
  B = (DIL *) kgGetWidget(Tmp,i);
  T = (DIT *)kgGetNamedWidget(Tmp,"AmixTbox1");
  T1 = T;
  strcpy(cndata.audiofile,kgGetString(T,0));
//  printf("%s\n",cndata.audiofile);
  n=0;
  while(cndata.audiofile[n]==' ') n++;
  if(cndata.audiofile[n]< ' ') return 0;
  T = (DIT *)kgGetNamedWidget(Tmp,"AmixTbox3");
  T2 = T;
  strcpy(cndata.infile,kgGetString(T,0));
  n=0;
  while(cndata.infile[n]==' ') n++;
  if(cndata.infile[n]< ' ') return 0;
  T = (DIT *)kgGetNamedWidget(Tmp,"AmixTbox2");
  T3 = T;
  strcpy(cndata.outfile,kgGetString(T,0));
  n=0;
  while(cndata.outfile[n]==' ') n++;
  if(cndata.outfile[n]< ' ') return 0;
  R = (DIRA *) kgGetNamedWidget(Tmp,"AmixQty");
  Qty = kgGetSelection(R);
  printf("Tools= %d\n",Tools);
  cndata.code =6;
  if(Tools== 7) cndata.code = 7;
  cndata.Eol='\n';
  sprintf(buff,"%d \"%-s\" \"%-s\" \"%-s\" %d \n",
       cndata.code, cndata.audiofile,cndata.infile,cndata.outfile,Qty);
  write(ToTools[1],buff,strlen(buff));
  kgSetString(T1,0,(char *)"");
  kgSetString(T2,0,(char *)"");
  ResetGrpVis(Tmp);
  ret = 0;
  return ret;
}
void  AmixAudiosplbutton1init(DIL *B,void *pt) {
}
int  AmixAudiotextbox3callback(int cellno,int i,void *Tmp) {
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
int  AmixAudiobutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  
  int n,ret =0; 
  char FileName[500];
  D = (DIALOG *)Tmp;
  DIT *T;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  T = (DIT *)kgGetNamedWidget(Tmp,"AmixTbox3");
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
//  kgFolderBrowser(NULL,100,100,FileName,"*");
  if(!FolderBrowser(FileName))return 0;
  CheckMedia(FileName);
  if((Minfo.Video !=0)||(Minfo.Audio !=0) ) {
    kgSetString(T,0,FileName);
    kgUpdateWidget(T);
    kgUpdateOn(Tmp);
  }
  return ret;
}
void  AmixAudiobutton2init(DIN *B,void *pt) {
}
