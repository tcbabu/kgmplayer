#include <kulina.h>
#include <math.h>
#include <sys/stat.h>
#include "kgutils.h"
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int FileStat(char *flname);
int kgffmpeg(int,char **);
int ffmpegfun(int,char **);
int ProcessSkip(int pip0,int pip1,int Pid);
int ProcessToPipe(int pip0,int pip1,int Pid);
int ProcessPrint(int pip0,int pip1,int Pid);
int MakeNewFileName(char *Infile,char *OutFile);
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int GetFolderName(char *infile,char *folder);
int RunAndWait(char *);
int RunAndMonitor(char *);
int RunMonitorAndWait(char *);
int ExtractVideoInfo(char *FileName,int *xres,int *yes,float *duration);

extern MEDIAINFO Minfo;

extern int Jpipe[2];
extern int Jstat[2];
extern int MonPipe;

int AudioToWav(char *infile,char *outfile) {
  char command[500];
  fprintf(stderr,"Inside AudioToWav: %s %s\n",infile,outfile); 
  sprintf(command,"ffmpegfun -i \"%s\" -vn -aq 2 -ac 2 -ar 44100 "
         "-acodec pcm_s32le -y %s",
         infile, outfile);
//  printf("%s\n",command);
  runfunction(command,ProcessPrint,ffmpegfun);
  return 1;
}


int AudioReformat(char *infile,char *outfile) {
     char command[500];
     char Qstr[50];
     Qstr[0]='\0';

     if(kgSearchString(outfile,(char *)".aac")>=0) {
       sprintf(Qstr," -c:a libfdk_aac ");
     }
     if(kgSearchString(outfile,(char *)".mp3")>=0) {
       sprintf(Qstr," -c:a libmp3lame -aq 0  ");
     }
     sprintf(command,"ffmpegfun -y   -i %-s -vn "
        " -ac 2 %s  \"%-s\" ", infile ,Qstr,outfile);
//        printf("%s\n",command);
     runfunction(command,ProcessPrint,ffmpegfun);
     return 1;
}
int GetTimeString(float duration,char *Tstring){
  int hr,mi;
  float sec;
   sec = duration;
   mi = (int)(sec)/60;
   sec = sec - mi*60;
   hr = mi/60;
   mi = mi - hr*60;
  sprintf(Tstring," %-d:%-d:%-.3f ",hr,mi,sec);
  return 1;
}
int CreateSilentAudio(float duration,char *outfile) {
  char command[500];
  char Tstring[30];
  GetTimeString(duration,Tstring);
  sprintf(command,"ffmpegfun -y -ar 44100  -f s32le -acodec pcm_s32le "
        " -ac 2 -i /dev/zero -acodec pcm_s32le -t %s  %s",
         Tstring,outfile);
  runfunction(command,ProcessPrint,ffmpegfun);
  return 1;
}



int JoinTwoAudio(char *infile1,char *infile2,char *outfile) {
  char buff[4098];
  char Tfolder[300],Afile1[300],Afile2[300],Afile3[300];
  int count =2;
  int Fstat =0;
  Fstat = MakeTmpFolderInHome(Tfolder);
  MakeFileInFolder("/tmp/Audio.wav",Tfolder,Afile1,"wav");
  union HEADER {char raw[256];int data[64];} header1,header2;
  FILE *fp,*fp2,*of;
  char *dpt;
  int ir,i,j,ln,ln2;
  int *ipt;
  int data,chnk1;
  AudioToWav(infile1,Afile1);
  MakeFileInFolder("/tmp/Audio.wav",Tfolder,Afile2,"wav");
  AudioToWav(infile2,Afile2);
  MakeFileInFolder("/tmp/Audio.wav",Tfolder,Afile3,"wav");
  sprintf(buff,"%-s",Afile1);
  ln = GetWavHeaderLength(buff);
  if(ln==0) return 0;
  fp = fopen(buff,"r");
  if(fp==NULL) return 0;
  fread(header1.raw,1,ln,fp);
  ipt = (int *)(header1.raw+ln-4);
  data = *ipt;
  chnk1 = header1.data[1];
  for(i=1;i<count;i++) {
    sprintf(buff,"%-s",Afile2);
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
  sprintf(buff,"%-s",Afile3);
  of = fopen(buff,"w");
  fwrite(header1.raw,1,ln,of);
  while( (ir=fread(buff,1,4098,fp))==4098) {
    fwrite(buff,1,4098,of);
  }
  if(ir> 0) fwrite(buff,1,ir,of);
  for(i=1;i<count;i++) {
    sprintf(buff,"%-s",Afile2);
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
  AudioReformat(Afile3,outfile);
  kgCleanDir(Tfolder);
  return 1;
}

int JoinWavFiles_bak(char *infile1,char *infile2,char *outfile) {
  char buff[4098];
  union HEADER {char raw[256];int data[64];} header1,header2;
  FILE *fp,*fp2,*of;
  char *dpt;
  char Tfolder[300],Afile1[300],Afile2[300],Afile3[300];
  int Fstat=0;
  Fstat = MakeTmpFolderInHome(Tfolder);
  int ir,i,j,ln,ln2;
  int *ipt;
  int data,chnk1;
  MakeFileInFolder("/tmp/Audio.wav",Tfolder,Afile1,"wav");
  MakeFileInFolder("/tmp/Audio.wav",Tfolder,Afile2,"wav");
  MakeFileInFolder("/tmp/Audio.wav",Tfolder,Afile3,"wav");
  AudioToWav(infile1,Afile1);
  AudioToWav(infile2,Afile2);
  ln = GetWavHeaderLength(Afile1);
  if(ln==0) return 0;
  fp = fopen(Afile1,"r");
  if(fp==NULL) return 0;
  fread(header1.raw,1,ln,fp);
  ipt = (int *)(header1.raw+ln-4);
  data = *ipt;
  chnk1 = header1.data[1];
    ln2 = GetWavHeaderLength(Afile2);
    if(ln2==0) return 0;
    fp2 = fopen(Afile2,"r");
    if(fp2==NULL) return 0;
    fread(header2.raw,1,ln2,fp2);
    ipt = (int *)(header2.raw+ln2-4);
    data += (*ipt);
    chnk1 += (*ipt);
    fclose(fp2);
  ipt = (int *)(header1.raw+ln-4);
  *ipt= data;
  header1.data[1]=chnk1;
  of = fopen(Afile3,"w");
  fwrite(header1.raw,1,ln,of);
  while( (ir=fread(buff,1,4098,fp))==4098) {
    fwrite(buff,1,4098,of);
  }
  if(ir> 0) fwrite(buff,1,ir,of);
  fclose(fp);
  fclose(of);
  AudioReformat(Afile3,outfile);
  kgCleanDir(Tfolder);
  return 1;
}

int AddSilenceAtStart(char *infile,float duration,char *outfile) {
  char command[500],Tfolder[300],Atmp1[200];
  int Fstat=0;
  char Tstring[50];
  GetTimeString(duration,Tstring);
  Fstat = MakeTmpFolderInHome(Tfolder);
  MakeFileInFolder("/tmp/Audio.wav",Tfolder,Atmp1,"wav");
  sprintf(command,"ffmpegfun -y -ar 44100 -f s32le  -acodec pcm_s32le "
        " -ac 2 -i /dev/zero -acodec pcm_s32le -t %s  %s",
         Tstring,Atmp1);
  runfunction(command,ProcessPrint,ffmpegfun);
  JoinTwoAudio(Atmp1,infile,outfile);
  kgCleanDir(Tfolder);
  return 1;
}
int AddSilenceAtEnd(char *infile,float duration,char *outfile) {
  char command[500],Tfolder[300],Atmp1[200];
  int Fstat=0;
  char Tstring[50];
  if ( fabsf(duration) <0.001) {
    strcpy(outfile,infile);
    return 0;
  }
  GetTimeString(duration,Tstring);
  Fstat = MakeTmpFolderInHome(Tfolder);
  MakeFileInFolder("/tmp/Audio.wav",Tfolder,Atmp1,"wav");
  sprintf(command,"ffmpegfun -y -ar 44100 -f s32le  -acodec pcm_s32le "
        " -ac 2 -i /dev/zero -acodec pcm_s32le -t %s  %s",
         Tstring,Atmp1);
  runfunction(command,ProcessPrint,ffmpegfun);
  JoinTwoAudio(infile,Atmp1,outfile);
  kgCleanDir(Tfolder);
  return 1;
}
int AudioExtract(char *infile,char *outfile) {
   char buff[500];
 //  sprintf(buff,"ffmpegfun -y -i %s -vn -ac 2 -ar 44100 -acodec aac %s", infile,outfile);
   sprintf(buff,"ffmpegfun -y -i %s -vn -ac 2 -ar 44100 -acodec pcm_s32le  %s", infile,outfile);
   runfunction(buff,ProcessPrint,ffmpegfun);
   return 1;
}
int AudioChange(char *media,char *audio,char *outfile) {
   char buff[500];
   sprintf(buff,"ffmpegfun -y -vn -i %s -an -i %s -f mp4  -c:v copy -c:a copy  -aq 0  -ar 44100 "
     "  -acodec aac   %s",audio,media,outfile);
   runfunction(buff,NULL,ffmpegfun);
   return 1;
}
int VideoCrop(char *infile,int xo,int yo,int xres,int yres,char *outfile) {
  char buff[500];
  sprintf(buff,"ffmpegfun -y -i %s   -vf \"crop=%-d:%-d:%-d:%-d\" %s",
       infile,xres,yres,xo,yo,outfile);
  runfunction(buff,ProcessPrint,ffmpegfun);
  return 1;
}
int RunVideoCrop(int argc,char **argv) {
   int xo,yo,xres,yres;
   sscanf(argv[2],"%d",&xo);
   sscanf(argv[3],"%d",&yo);
   sscanf(argv[4],"%d",&xres);
   sscanf(argv[5],"%d",&yres );
   VideoCrop(argv[1],xo,yo,xres,yres,argv[6]);
   return 1;
}
int ChangeVideoSizeAndFrate(char *infile,char *outfile,int Xres,int Yres,int fs,int Qty){
  char buff[500];
  char Qstr[200];
  switch(Qty) {
       case 1:
         sprintf(Qstr," -crf 20 -preset medium ");
         break;
       case 2:
         sprintf(Qstr," -crf 22  -preset fast ");
         break;
       case 3:
       default:
         sprintf(Qstr," -crf 24  -preset fast ");
         break;
   
  }
  sprintf(buff,"ffmpegfun -y  -i %s -vf \"scale=%d:%d:flags=lanczos,setsar=1,fps=%-d\" "
       " %s  -c:v libx264 %s -af aresample=44100 -c:a aac  ",
       infile,(Xres/2)*2,(Yres/2)*2,fs,Qstr,outfile);
  printf("%s\n",buff);
  fflush(stdout);
//  sleep(20);
  runfunction(buff,ProcessPrint,ffmpegfun);
 //  RunString(buff,ffmpegfun);
  return 1;
}
int ChangeVideoSize(char *infile,char *outfile,int Xres,int Yres,int Qty){
  char buff[500];
  char Qstr[200];
  switch(Qty) {
       case 1:
         sprintf(Qstr," -crf 20 -preset medium ");
         break;
       case 2:
         sprintf(Qstr," -crf 22  -preset fast ");
         break;
       case 3:
       default:
         sprintf(Qstr," -crf 24  -preset fast ");
         break;
   
  }
  sprintf(buff,"ffmpegfun -y  -i %s -f mp4 -vf \"scale=%d:%d:flags=lanczos,setsar=1\" "
       " %s  -c:v libx264 %s -af aresample=44100 -c:a aac ",
       infile,(Xres/2)*2,(Yres/2)*2,Qstr,outfile);
  runfunction(buff,ProcessPrint,ffmpegfun);
  return 1;
}
int ChangeVideoFrate(char *infile,char *outfile,int fs,int Qty){
  char buff[500];
  char Qstr[200];
  switch(Qty) {
       case 1:
         sprintf(Qstr," -crf 20 -preset medium ");
         break;
       case 2:
         sprintf(Qstr," -crf 22  -preset fast ");
         break;
       case 3:
       default:
         sprintf(Qstr," -crf 24  -preset fast ");
         break;
   
  }
  sprintf(buff,"ffmpegfun -y  -i %s -vf \"fps=%-d\" "
       " %s  -c:v libx264 %s -af aresample=44100 -c:a aac ",
       infile,fs,Qstr,outfile);
  runfunction(buff,ProcessPrint,ffmpegfun);
  return 1;
}
int ConvertToLibx265(char *infile,int Qty,char *outfile){
  char buff[500];
  char Qstr[200];
  char Astr[100];
  strcpy(Astr," -af aresample=44100 -c:a aac ");
  switch(Qty) {
       case 1:
         sprintf(Qstr," -crf 20 -preset medium ");
         break;
       case 2:
         sprintf(Qstr," -crf 22  -preset fast ");
         break;
       case 3:
       default:
         sprintf(Qstr," -crf 24  -preset fast ");
         break;
   
  }
  sprintf(buff,"ffmpegfun -y  -i %s -f mp4"
       " %s  -c:v libx265 %s -af aresample=44100 -c:a aac ",
       infile,Qstr,outfile);
  runfunction(buff,ProcessPrint,ffmpegfun);
  return 1;
}
int ConvertToLibx264(char *infile,int Qty,char *outfile){
  char buff[500];
  char Qstr[200];
  switch(Qty) {
       case 1:
         sprintf(Qstr," -crf 18 -preset medium ");
         break;
       case 2:
         sprintf(Qstr," -crf 22  -preset medium ");
         break;
       case 3:
       default:
         sprintf(Qstr," -crf 24  -preset fast ");
         break;
   
  }
  sprintf(buff,"ffmpegfun -y  -i %s -f mp4"
       " %s  -c:v libx264 %s -af aresample=44100 -c:a aac ",
       infile,Qstr,outfile);
  runfunction(buff,ProcessPrint,ffmpegfun);
  return 1;
}

int OverlayVideos(char *base,char *olay,int Qty,char *outfile) {
  /*
     Overlays olay over base, in centralised way
     in case olay is bigger in dimension it will be resize to fit
     aspect ratio will be maintained
  */
  int Bxres,Byres,Oxres,Oyres;
  float Bfps,Ofps;
  int xloc,yloc;
  MEDIAINFO *mpt;
  char Bvcodec[30],Ovcodec[30];
  char Tmp1[300],Tmp2[300],Tmp3[300],Otmp[200],Btmp[200];
  char buff[500];
  char Tfolder[300],Err[400];
  int Fstat=1;
  int cx =0,cy=0;
  double cxfact=1.0,cyfact=1.0;
  Fstat = MakeTmpFolderInHome(Tfolder);
  printf("Inside OverlayVideos\n");
  fflush(stdout);
  strcpy(Otmp,olay);
  mpt = GetMediaInfo(base);
  if(mpt->Video != 1) return 0;
  Bxres =mpt->Axres;
  Byres =mpt->Ayres;
  Bfps  = mpt->fps;
  strcpy(Bvcodec,mpt->vcodectype);
  Btmp[0]='\0';
  strcpy(Btmp,base);
  free(mpt);
  mpt= NULL;
  if ((strcmp(Bvcodec,"h264") != 0)) {
     MakeFileInFolder(base,Tfolder,Btmp,"mp4");
     printf("Calling Libx264 %s %s\n",base,Btmp);
     fflush(stdout);
     ConvertToLibx264(base,Qty,Btmp);
     printf("Created: %s\n",Btmp);
     fflush(stdout);
     mpt = GetMediaInfo(Btmp);
     if(mpt->Video != 1) return 0;
     Bxres =mpt->Axres;
     Byres =mpt->Ayres;
     Bfps  = mpt->fps;
     strcpy(Bvcodec,mpt->vcodectype);
     free(mpt);
     mpt=NULL;
  }
  printf("Opened Err 2\n");
  fflush(stdout);
  mpt = GetMediaInfo(olay);
  if(mpt->Video != 1) return 0;
  Oxres = mpt->Axres;
  Oyres =mpt->Ayres;
  Ofps  = mpt->fps;
  strcpy(Ovcodec,mpt->vcodectype);
  free(mpt);
  mpt = NULL;
  Tmp1[0]='\0';
  Tmp2[0]='\0';
  printf("Oxres = %d Bxres = %d\n",Oxres,Bxres);
  fflush(stdout);
  cxfact =1;
  cx =0;
  if(Bxres< Oxres) {
    cxfact = (float)Bxres/Oxres;
    Oxres = ((int)(Oxres*cxfact))/2*2;
    Oyres  = ((int)(Oyres*cxfact))/2*2;
    cx =1;
  }
  cyfact = 1.0;
  cy = 0;
  if(Byres< Oyres) {
    cyfact = (float)Byres/Oyres;
    Oxres = ((int)(Oxres*cyfact))/2*2;
    Oyres  = ((int)(Oyres*cyfact))/2*2;
    cy =1;
  }
//  if( (Oxres > Bxres )||( fabsf(Bfps -Ofps)> 1)){
  if (cx||cy||( fabsf(Bfps -Ofps)> 1)){

     MakeFileInFolder(olay,Tfolder,Tmp1,"mp4");
     printf("ChangeVideoSizeAndFrate\n");
     fflush(stdout);
     ChangeVideoSizeAndFrate(Otmp,Tmp1,Oxres,Oyres,(int)(Bfps+0.5),Qty);
     strcpy(Otmp,Tmp1);
     mpt = GetMediaInfo(Otmp);
     if(mpt->Video != 1) return 0;
     Oxres =mpt->Axres;
     Oyres =mpt->Ayres;
     Ofps  = mpt->fps;
     strcpy(Ovcodec,mpt->vcodectype);
     free(mpt);
     mpt = NULL;
  }   
#if 0
  if( Oyres > Byres ){
//     MakeNewFileName(olay,Tmp2);
     MakeFileInFolder(olay,Tfolder,Tmp2,"mp4");
     ChangeVideoSize(Otmp,Tmp2,-2,Byres,Qty);
     strcpy(Otmp,Tmp2);
     mpt = GetMediaInfo(Otmp);
     if(mpt->Video != 1) return 0;
     Oxres =mpt->Axres;
     Oyres =mpt->Ayres;
     Ofps  = mpt->fps;
     strcpy(Ovcodec,mpt->vcodectype);
     free(mpt);
     mpt = NULL;
  }   
#endif
  Tmp3[0]='\0';  
  if ((strcmp(Ovcodec,"h264") != 0)){
//     MakeNewFileName(olay,Tmp3);
     MakeFileInFolder(olay,Tfolder,Tmp3,"mp4");
     ConvertToLibx264(Otmp,Qty,Tmp3);
     strcpy(Otmp,Tmp3);
     mpt = GetMediaInfo(Otmp);
     if(mpt->Video != 1) return 0;
     Oxres =mpt->Axres;
     Oyres =mpt->Ayres;
     Ofps  = mpt->fps;
     strcpy(Ovcodec,mpt->vcodectype);
     free(mpt);
     mpt = NULL;
  }
  xloc = (Bxres - Oxres)/2;
  yloc = (Byres - Oyres)/2;
  printf("Btmp : %s Otmp : %s : %d %d %s\n",Btmp,Otmp,xloc,yloc,outfile);
  fflush(stdout);
  sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"[0:v][1:v]overlay=%-d:%-d\" %s",
       Btmp,Otmp,xloc,yloc,outfile);
  printf("buff : %s\n",buff);
  fflush(stdout);
  runfunction(buff,ProcessPrint,ffmpegfun);
  kgCleanDir(Tfolder);
#if 0
  if(Tmp1[0] != '\0') remove(Tmp1);
  if(Tmp3[0] != '\0') remove(Tmp2);
  if(Tmp3[0] != '\0') remove(Tmp3);
  if(Btmp[0] != '\0') remove(Btmp);
#endif  
  return 1;
}
int OverlayToSize(int Bxres,int Byres,float fs,int Qty,char *olay,char *outfile) {
  /*
     Overlays olay over base, in centralised way
     in case olay is bigger in dimension it will be resize to fit
     aspect ratio will be maintained
  */
  int Oxres,Oyres;
  MEDIAINFO *mpt;
  float Bfps,Ofps,xloc,yloc;
  char Bvcodec[30],Ovcodec[30];
  char Tmp1[200],Tmp2[200],Tmp3[200],Otmp[200],Btmp[200];
  char buff[500],Tmpbase[300],base[300];
  char Tfolder[300],Tmpfile[300],Afile[300];;
  int Fstat=1,Audio=1;
  Fstat = MakeTmpFolderInHome(Tfolder);
  MakeFileInFolder("/tmp/Blank.mp4",Tfolder,Tmpfile,"mp4");
  printf("Tmpfile: %s: %s\n",Tmpfile,olay);
  fflush(stdout);
  mpt = GetMediaInfo(olay);
  if(mpt->Video != 1) return 0;
  if(mpt->Audio ) {
     MakeFileInFolder("/tmp/Audio.mp4",Tfolder,Afile,"wav");
     AudioExtract(olay,Afile);       
  }
  else Audio=0;;
  printf("Calling CreateBlankVideo: %-.3f\n",mpt->TotSec);
  fflush(stdout);
  CreateBlankVideo(Bxres,Byres,(float)mpt->TotSec,fs,Tmpfile);
  printf("Tmpfile: %s\n",Tmpfile);
  fflush(stdout);
  free(mpt);
  mpt = NULL;
  
  printf("Calling OverlayVideos\n");
  fflush(stdout);
  if(!Audio) OverlayVideos(Tmpfile,olay,Qty,outfile);
  else {
     MakeFileInFolder("/tmp/Video.mp4",Tfolder,Tmp3,"mp4");
     OverlayVideos(Tmpfile,olay,Qty,Tmp3);
     AudioChange(Tmp3,Afile,outfile);
  }
  printf("OverlayVideos: %s\n",outfile);
  fflush(stdout);
  kgCleanDir(Tfolder);
  return 1;
}

//int RunOverlayToSize(int Bxres,int Byres,float fs,int Qty,char *olay,char *outfile) {
int RunOverlayToSize(int argc,char **argv) {
    int Bxres,Byres,Qty;
    float fs;
    char olay[300];
    char outfile[300];
    int i=1;
    sscanf(argv[1],"%d",&Bxres);
    sscanf(argv[2],"%d",&Byres);
    sscanf(argv[3],"%f",&fs);
    sscanf(argv[4],"%d",&Qty);
    sscanf(argv[5],"%s",olay);
    sscanf(argv[6],"%s",outfile);
    return OverlayToSize(Bxres,Byres,(float)((int)(fs+0.5)),Qty,olay,outfile);
    
}
int CreateStillVideo(char *infile,float duration,float fps,char *outfile) {
    char buff[500];
  char Tstring[50];
  GetTimeString(duration,Tstring);
    sprintf(buff,"ffmpegfun  -y  -loop 1 -i %s  -t %s -f mp4  -vf fps=%-.3f -vcodec libx264  %s",
      infile,Tstring,fps,outfile);
    runfunction(buff,NULL,ffmpegfun);
    return 1;
}
int CreateBlankVideo(int Xsize,int Ysize,float duration,float fps,char *outfile) {
    char buff[500],Infile[300], folder[300],Tmpfile[300];
    int Fstat=1;
  char Tstring[50];
  GetTimeString(duration,Tstring);
  Fstat = MakeTmpFolderInHome(folder);
    MakeFileInFolder(outfile,folder,Infile,"png");
    printf("Image: %s : %s %f %f \n",Infile,outfile,duration,fps);
    fflush(stdout);
#if 0
    void *img=kgInitImage(Xsize,Ysize,1);
    kgWriteImage(kgGetResizedImage(img),Infile);
    kgCloseImage(img);
#else
    void *img= kgCreateImage(Xsize,Ysize);
    kgWriteImage(img,Infile);
    kgFreeImage(img);
#endif
    if(FileStat(Infile))printf("Created Image: %s\n",Infile);
    else printf("Failed to Create %s\n",Infile);
    fflush(stdout);
    sprintf(buff,"ffmpegfun  -y  -loop 1 -i %s  -t %s -f mp4 -vf fps=%-.3f -vcodec libx264  %s",
      Infile,Tstring,fps,outfile);
    printf("%s\n",buff);
    fflush(stdout);
    runfunction(buff,ProcessPrint,ffmpegfun);
//    RunString(buff,ffmpegfun);
    if(FileStat(outfile))printf("created BLANK VIDEO: %s  sleeping..\n",outfile);
    else printf("Failed tp create %s\n",outfile);
    fflush(stdout);
    kgCleanDir(folder);
    return 1;
}
int GetFirstFrame(char *infile,char *outfile) {
   char buff[500],tbuff[30];
   float sec=0.0,offset=0.05;
   int id=1;
   MEDIAINFO *mt = GetMediaInfo(infile);
   if(mt->Video != 1) {free(mt);return 0;}
   free(mt);
//   sprintf(buff,"ffmpegfun  -y  -ss 00:00:0.01 -i %s -frames:v 1 %s",infile,outfile);
//   RunAndWait(buff);
   while(1) {
   sprintf(tbuff,"00:00:%-.3f",sec+offset*id);
   sprintf(buff,"ffmpegfun  -y  -ss %s  -i %s -frames:v 1 %s",tbuff,infile,outfile);
   RunAndWait(buff);
     if(!FileSize(outfile)){
        fprintf(stderr,"Failed to get Last Frame\n");
        id++;
     }
     else break;
   }
   return 1;
}
int GetLastFrame(char *infile,char *outfile) {
   char buff[500],tbuff[30];
   int hr=0,mi=0;
   float sec,offset=0.05;
   int id;
   MEDIAINFO *mt = GetMediaInfo(infile);
   if(mt->Video != 1) {free(mt);return 0;}
   free(mt);
   sec = mt->TotSec;
   mi = (int)(sec)/60;
   sec = sec - mi*60;
   hr = mi/60;
   mi = mi - hr*60;
   id =1;   
//   while(1) {
//   sprintf(tbuff,"%-d:%-d:%-.3f",hr,mi,sec-offset*id);
 //  sprintf(buff,"ffmpegfun  -y  -ss %s  -i %s -frames:v 1 %s",tbuff,infile,outfile);
   sprintf (buff,"ffmpegfun -y -sseof -3 -i %s -update true -q:v 2 %s",infile,outfile);
   RunAndWait(buff);
     if(!FileSize(outfile)){
        fprintf(stderr,"Failed to get Last Frame\n");
        id++;
     }
//     else break;
//   }
   return 1;
}
int JoinTwoVideos(char *infile1,char *infile2,char *outfile){
   char buff[500],Tmp[100],Txt[100];
   strcpy(Tmp,(char *)"/tmp");
   sprintf (buff,"ffmpegfun -y -i %s -i %s -filter_complex "
      "\"[0:v][1:v]concat=n=2:v=1[outv]\" -map \"[outv]\" %s",
      infile1,infile2,outfile);
   RunAndWait(buff);
   return 1;
}
int AddStillAtStart(char *infile,float  duration,char *outfile) {
   char buff[500],Tmp[100],Ffile[200],Sfile[500],
              Afile[300],NAfile[300],Tfolder[300];
   MEDIAINFO *mpt,*mtmp;
   int Fstat=1;
   if(duration <= 0.0 ) {
      strcpy(outfile,infile);
      return 0;
   }
   mpt = GetMediaInfo(infile);
   if(mpt->Video != 1){free(mpt); return 0;}
   Fstat = MakeTmpFolderInHome(Tfolder);
   MakeFileInFolder(infile,Tfolder,Ffile,(char *)"png");
   GetFirstFrame(infile,Ffile);  
   MakeFileInFolder(infile,Tfolder,Sfile,(char *)"mp4");
   CreateStillVideo(Ffile,duration,mpt->fps,Sfile);
   mtmp = GetMediaInfo(Sfile);
   MakeFileInFolder("/tmp/Audio.wav",Tfolder,Afile,(char *)"wav");
   AudioExtract(infile,Afile);
   MakeFileInFolder("/tmp/Audio.wav",Tfolder,NAfile,(char *)"wav");
   AddSilenceAtStart(Afile,mtmp->TotSec,NAfile);
   free(mtmp);   
   MakeFileInFolder(infile,Tfolder,Ffile,(char *)"mp4");
   JoinTwoVideos(Sfile,infile,Ffile);   
   AudioChange(Ffile,NAfile,outfile);
//   remove(Ffile);
//   remove(Sfile);
   free(mpt);
   mpt = NULL;
   kgCleanDir(Tfolder);
   return 1;
}
 int RunAddStillAtStart(int argc,char **argv) {
    float duration;
    sscanf(argv[2],"%f",&duration);
    return AddStillAtStart(argv[1],duration,argv[3]);
 }
int AddStillAtEnd(char *infile,float  duration,char *outfile) {
   char buff[500],Tmp[100],Ffile[200],Sfile[500],
              Afile[300],NAfile[300],Tfolder[300];
   MEDIAINFO *mpt,*mtmp;
   int Fstat=1;
   if ( (fabsf(duration) <0.001) ||(duration <= 0.0)){
    strcpy(outfile,infile);
    return 0;
   }
   mpt = GetMediaInfo(infile);
   if(mpt->Video != 1){free(mpt); return 0;}
   Fstat = MakeTmpFolderInHome(Tfolder);
   MakeFileInFolder(infile,Tfolder,Ffile,(char *)"png");
   GetLastFrame(infile,Ffile);  
   MakeFileInFolder(infile,Tfolder,Sfile,(char *)"mp4");
   CreateStillVideo(Ffile,duration,mpt->fps,Sfile);
   mtmp = GetMediaInfo(Sfile);
   MakeFileInFolder("/tmp/Audio.wav",Tfolder,Afile,(char *)"wav");
   AudioExtract(infile,Afile);
   MakeFileInFolder("/tmp/Audio.wav",Tfolder,NAfile,(char *)"wav");
   AddSilenceAtEnd(Afile,mtmp->TotSec,NAfile);
   free(mtmp);   
   MakeFileInFolder(infile,Tfolder,Ffile,(char *)"mp4");
   JoinTwoVideos(infile,Sfile,Ffile);   
   AudioChange(Ffile,NAfile,outfile);
//   remove(Ffile);
//   remove(Sfile);
   free(mpt);
   mpt = NULL;
   kgCleanDir(Tfolder);
   return 1;
}
 int RunAddStillAtEnd(int argc,char **argv) {
    float duration;
    sscanf(argv[2],"%f",&duration);
    return AddStillAtEnd(argv[1],duration,argv[3]);
 }
int MixTwoAudios( char *infile1,char *infile2,char *outfile) {
  int Qty=1;
  int same=0;
  char Folder[500];
  char Atmp1[300],Atmp2[300],Atmp3[300],Atmp[300],Aextra[300],Tstring[30];

    MEDIAINFO *mt1,*mt2;
    char command[10000];

    float  Asec,Msec,Esec;

    MakeTmpFolderInHome(Folder);
    mt1 = GetMediaInfo(infile1);
    mt2 = GetMediaInfo(infile2);
    Asec = mt1->TotSec;
    Esec = mt2->TotSec;
    Msec = Asec;
    if(Esec< Asec) {
       Msec = Esec;
    }
    GetTimeString((float)Msec,Tstring);
    MakeFileInFolder("/tmp/Audio.wav",Folder,Atmp1,"wav");
    MakeFileInFolder("/tmp/Audio.wav",Folder,Atmp2,"wav");
    MakeFileInFolder("/tmp/Audio.wav",Folder,Atmp3,"wav");
    MakeFileInFolder("/tmp/Audio.wav",Folder,Atmp,"wav");
    MakeFileInFolder("/tmp/Audio.wav",Folder,Aextra,"wav");
    AudioToWav(infile1,Atmp1);
    AudioToWav(infile2,Atmp2);
    sprintf(command,"ffmpegfun -vn -i %s -vn -i "
       " %s  -t %s -ac 2 -ar 44100 -acodec pcm_s32le -lavfi amix "
       "  -y %s ",
       Atmp1,Atmp2,Tstring,Atmp3);
    runfunction(command,ProcessPrint,ffmpegfun);
    sprintf(command,"ffmpegfun  -i %s -ac 2 -ar 44100 "
       " -acodec pcm_s32le -filter_complex "
       " \"aeval=val(0)/2*3|val(1)/2*3:c=same\" "
       "  -y %s ",
       Atmp3,Atmp);
//       printf("%s\n",command);
    runfunction(command,ProcessPrint,ffmpegfun);
    same =0;
    if( (int)(Asec*1000) == (int)(Esec*1000) ) same=1;
    if(same) {
       AudioReformat(Atmp,outfile);
    }
    else {    
      if(Msec == Asec) {
      sprintf(command,"ffmpegfun -vn -i \"%s\" -ss %s "
       " -vn -aq 2 -ac 2 -ar 44100 -acodec pcm_s32le -y %s ",
       Atmp2,Tstring,Aextra);
      }
      else {
      sprintf(command,"ffmpegfun -vn -i \"%s\" -ss %s "
       " -vn -aq 2 -ac 2 -ar 44100 -acodec pcm_s32le -y %s ",
       Atmp1,Tstring,Aextra);
      }
      runfunction(command,ProcessPrint,ffmpegfun);
      JoinTwoAudio(Atmp,Aextra,Atmp1);
      AudioReformat(Atmp1,outfile);
    }
    free(mt1);
    free(mt2);
    kgCleanDir(Folder);
    return 1;
}
int VideoSideBySide(char *infile1,char *infile2,char *outfile) {



  int Xres1,Yres1,Xres2,Yres2,Mx,My;
  int Resize=0,ret=1;
  char Audio[300];
  char Tfolder[300],Vfile[300],Vout[300];
  MEDIAINFO *mt1,*mt2;
  char buff[500];
  MakeTmpFolderInHome(Tfolder);
  MakeFileInFolder("/tmp/Video.mp4",Tfolder,Vfile,"mp4");
  MakeFileInFolder("/tmp/Video.mp4",Tfolder,Vout,"mp4");
  MakeFileInFolder("/tmp/Audio.wav",Tfolder,Audio,"wav");
  mt1 = GetMediaInfo(infile1);
  Xres1= mt1->Axres;
  Yres1= mt1->Ayres;
  if(mt1->Video==0) {free(mt1); return 0;}
  mt2 = GetMediaInfo(infile2);
  if(mt2->Video==0) {free(mt2); free(mt1);return 0;}
  fprintf(stderr,"Going for Aixing Audios\n");
  fflush(stderr);
  sleep(5);
  MixTwoAudios(infile1,infile2,Audio);
  Xres2= mt2->Axres;
  Yres2= mt2->Ayres;
  if ( Yres2 > Yres1 ) {
     Resize=2;
     Mx = Xres1;
     My = Yres1;
  }
  if ( Yres2 < Yres1 ) {
     Resize=1;
     Mx = Xres2;
     My = Yres2;
  }
  sprintf (buff,"Processing Side by Side..\n");
//  kgWrite(I,buff);
  fprintf(stderr,"%s\n",buff);
  ret =0;
  if(Resize==0) {
  sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"hstack\" %s",
       infile1,infile2,Vout);
  remove(outfile);
//  kgWrite(I,buff);
//  runfunction(buff,ProcessPrint,ffmpegfun);
  fprintf(stderr,"%s\n",buff);
  RunMonitorAndWait(buff);
  }
  if(Resize == 1) {
    sprintf(buff,"!c01Processing %s\n",infile1);
//    kgWrite(I,buff);
  fprintf(stderr,"%s\n",buff);
    ChangeVideoSizeAndFrate(infile1,Vfile,-2,My,(int)mt2->fps,1);
    sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"hstack\" %s",
       Vfile,infile2,Vout);
       remove(outfile);
//       kgWrite(I,buff);
  fprintf(stderr,"%s\n",buff);
       RunMonitorAndWait(buff);
  }
  if(Resize == 2) {
    sprintf(buff,"!c05Processing %s\n",infile2);
//    kgWrite(I,buff);
  fprintf(stderr,"%s\n",buff);
    ChangeVideoSizeAndFrate(infile2,Vfile,-2,My,(int)mt1->fps,1);
    sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"hstack\" %s",
       infile1,Vfile,Vout);
    remove(outfile);
//    kgWrite(I,buff);
  fprintf(stderr,"%s\n",buff);
    RunMonitorAndWait(buff);
  }
  sprintf(buff,"!c05Processing Audio... !c01 pl Wait\n");
//  kgWrite(I,buff);
  fprintf(stderr,"%s\n",buff);
  AudioChange(Vout,Audio,outfile);
  free(mt1);
  free(mt2);
  kgCleanDir(Tfolder);
  return 1;
}
int VideoTopBottom(char *infile1,char *infile2,char *outfile) {


  int Xres1,Yres1,Xres2,Yres2,Mx,My;
  int Resize=0,ret=1;
  char Audio[300];
  char Tfolder[300],Vfile[300],Vout[300];
  MEDIAINFO *mt1,*mt2;
  char buff[500];
  MakeTmpFolderInHome(Tfolder);
  MakeFileInFolder("/tmp/Video.mp4",Tfolder,Vfile,"mp4");
  MakeFileInFolder("/tmp/Video.mp4",Tfolder,Vout,"mp4");
  MakeFileInFolder("/tmp/Audio.wav",Tfolder,Audio,"wav");
  mt1 = GetMediaInfo(infile1);
  Xres1= mt1->Axres;
  Yres1= mt1->Ayres;
  if(mt1->Video==0) {free(mt1); return 0;}
  mt2 = GetMediaInfo(infile2);
  if(mt2->Video==0) {free(mt2); free(mt1);return 0;}
  MixTwoAudios(infile1,infile2,Audio);
  Xres2= mt2->Axres;
  Yres2= mt2->Ayres;
  if ( Xres2 > Xres1 ) {
     Resize=2;
     Mx = Xres1;
     My = Yres1;
  }
  if ( Xres2 < Xres1 ) {
     Resize=1;
     Mx = Xres2;
     My = Yres2;
  }
  sprintf (buff,"Processing Top and Bottom ...\n");
  //kgWrite(I,buff);
  fprintf(stderr,"%s\n",buff);
  ret =0;
  if(Resize==0) {
  sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"Vstack\" %s",
       infile1,infile2,Vout);
  remove(outfile);
//  kgWrite(I,buff);
  fprintf(stderr,"%s\n",buff);
//  runfunction(buff,ProcessPrint,ffmpegfun);
  RunMonitorAndWait(buff);
  }
  if(Resize == 1) {
    sprintf(buff,"!c01Processing %s\n",infile1);
    //kgWrite(I,buff);
  fprintf(stderr,"%s\n",buff);
    ChangeVideoSizeAndFrate(infile1,Vfile,Mx,-2,(int)mt2->fps,1);
    sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"vstack\" %s",
       Vfile,infile2,Vout);
       remove(outfile);
//       kgWrite(I,buff);
  fprintf(stderr,"%s\n",buff);
       RunMonitorAndWait(buff);
  }
  if(Resize == 2) {
    sprintf(buff,"!c05Processing %s\n",infile2);
    //kgWrite(I,buff);
  fprintf(stderr,"%s\n",buff);
    ChangeVideoSizeAndFrate(infile2,Vfile,Mx,-2,(int)mt1->fps,1);
    sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"vstack\" %s",
       infile1,Vfile,Vout);
    remove(outfile);
//    kgWrite(I,buff);
  printf("%s\n",buff);
    RunMonitorAndWait(buff);
  }
  sprintf(buff,"!c01Processing Audio... Pl. Wait\n");
  //kgWrite(I,buff);
  fprintf(stderr,"%s\n",buff);
  AudioChange(Vout,Audio,outfile);
  free(mt1);
  free(mt2);
  kgCleanDir(Tfolder);
  return 1;
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
   FILE *fp = fopen(Outfile,"w");
   fclose(fp);
   fprintf(stderr,"Touched: MakeFileInFolder: %s\n",Outfile);
   return 1;
}
int MakeFileNameInFolder(char *Infile,char *Folder,char *Outfile,char *ext) {
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
   fprintf(stderr," MakeFileNameInFolder: %s\n",Outfile);
   return 1;
}

int RunAndMonitor(char * job)  {

  int pid=0,id;
  int status;
  if(fork() != 0)return 1;
  id = getpid();
  if(pipe(Jpipe) < 0) exit(0);
  if(pipe(Jstat) < 0) exit(0);
  MonPipe = Jpipe[0];
  char buff[500];
  
  if ((pid=fork())==0) {
    fflush(stdout);
    fflush(stderr);
    close(Jpipe[0]);
    close(Jstat[1]);
     sprintf(buff,"Executing... PLEASE WAIT\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"!c01Sorry... Progress Bar may not be correct\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"PLEASE WAIT till the window closes\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"You can cancel job if you wish\n");
     write(Jpipe[1],buff,strlen(buff));
     runfunction(job,ProcessOutput,ffmpegfun);
     close(Jpipe[1]);
     close(Jstat[0]);
     exit(0);
  }  //fork
  else {
     close(Jpipe[1]);
     close(Jstat[0]);
     RunMonitorJoin(NULL);
     kill(pid,9);
     waitpid(pid,&status,0);
     close(Jpipe[0]);
     close(Jstat[1]);
     exit(0);
  }
}
int RunFunctionAndMonitor(char * job,int (*function)(int,char **))  {

  int pid=0,id;
  int status;
  if(fork() != 0)return 1;
  id = getpid();
  if(pipe(Jpipe) < 0) exit(0);
  if(pipe(Jstat) < 0) exit(0);
  MonPipe = Jpipe[0];
  char buff[500];
  
  if ((pid=fork())==0) {
    fflush(stdout);
    fflush(stderr);
    close(Jpipe[0]);
    close(Jstat[1]);
     sprintf(buff,"Executing... PLEASE WAIT\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"!c01Sorry... Progress Bar may not be correct\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"PLEASE WAIT till the window closes\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"You can cancel job if you wish\n");
     write(Jpipe[1],buff,strlen(buff));
     runfunction(job,ProcessOutput,function);
     close(Jpipe[1]);
     close(Jstat[0]);
     exit(0);
  }  //fork
  else {
     close(Jpipe[1]);
     close(Jstat[0]);
     RunMonitorJoin(NULL);
     kill(pid,9);
     waitpid(pid,&status,0);
     close(Jpipe[0]);
     close(Jstat[1]);
     exit(0);
  }
}
int RunFunctionAndWait(char * job,int (*function)(int,char **))  {

  int pid=0,id;
  int status;
  id = getpid();
  if(pipe(Jpipe) < 0) exit(0);
  if(pipe(Jstat) < 0) exit(0);
  MonPipe = Jpipe[0];
  char buff[500];
  
  if ((pid=fork())==0) {
    fflush(stdout);
    fflush(stderr);
    close(Jpipe[0]);
    close(Jstat[1]);
     sprintf(buff,"Executing... PLEASE WAIT\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"!c01Sorry... Progress Bar may not be correct\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"PLEASE WAIT till the window closes\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"You can cancel job if you wish\n");
     write(Jpipe[1],buff,strlen(buff));
     runfunction(job,ProcessOutput,function);
     close(Jpipe[1]);
     close(Jstat[0]);
     exit(0);
  }  //fork
  else {
     close(Jpipe[1]);
     close(Jstat[0]);
     RunMonitorJoin(NULL);
     kill(pid,9);
     waitpid(pid,&status,0);
     close(Jpipe[0]);
     close(Jstat[1]);
  }
  return 1;
}
int RunMonitorAndWait(char * job)  {

  int pid=0,id;
  int status;
  id = getpid();
  if(pipe(Jpipe) < 0) exit(0);
  if(pipe(Jstat) < 0) exit(0);
  MonPipe = Jpipe[0];
  char buff[500];
  
  if ((pid=fork())==0) {
    fflush(stdout);
    fflush(stderr);
    close(Jpipe[0]);
    close(Jstat[1]);
     sprintf(buff,"Executing... PLEASE WAIT\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"!c01Sorry... Progress Bar may not be correct\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"PLEASE WAIT till the window closes\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"You can cancel job if you wish\n");
     write(Jpipe[1],buff,strlen(buff));
     runfunction(job,ProcessOutput,ffmpegfun);
     close(Jpipe[1]);
     close(Jstat[0]);
     exit(0);
  }  //fork
  else {
     close(Jpipe[1]);
     close(Jstat[0]);
     RunMonitorJoin(NULL);
     kill(pid,9);
     waitpid(pid,&status,0);
     close(Jpipe[0]);
     close(Jstat[1]);
  }
  return 1;
}
int RunAndWait(char * job)  {
   runfunction(job,ProcessPrint,ffmpegfun);
   return 1;
}
char *MakeTmpFolder(void) {
    char Folder[500];
    char *pt;
    int id=1;
    sprintf(Folder,"%-s/%-d_%-3.3d",getenv("HOME"),getpid(),id);
    while(FileStat(Folder)) {
      id++;
      sprintf(Folder,"%-s/%-d_%-3.3d",getenv("HOME"),getpid(),id);
    }
    mkdir(Folder,0700);
    pt = (char *)malloc(strlen(Folder)+1);
    strcpy(pt,Folder);
    return pt;
}
int MakeTmpFolderInHome(char *Tfolder) {
  int Fstat = 0;
  int id=0;
  sprintf(Tfolder,"%-s/%-d_%-3.3d",getenv("HOME"),getpid(),id);
  while(FileStat(Tfolder)) {
    id++;
    sprintf(Tfolder,"%-s/%-d_%-3.3d",getenv("HOME"),getpid(),id);
  }
   mkdir(Tfolder,0700);
    printf("Created: %s\n",Tfolder);
    Fstat=1;
  return Fstat;
}
 int FileSize(char *flname) {
  int ret;
  struct stat buff;
  ret = stat(flname,&buff);
  if(ret < 0) return 0;
  else return (int)buff.st_size;
}
