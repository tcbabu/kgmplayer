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
       " %s  -c:v libx264 %s",
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
       " %s  -c:v libx264 %s",
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
       " %s  -c:v libx264 %s",
       infile,fs,Qstr,outfile);
  runfunction(buff,ProcessPrint,ffmpegfun);
  return 1;
}
int ConvertToLibx265(char *infile,int Qty,char *outfile){
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
  sprintf(buff,"ffmpegfun -y  -i %s -f mp4"
       " %s  -c:v libx265 %s",
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
       " %s  -c:v libx264 %s",
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
  sprintf(Tfolder,"%-s/%-d",getenv("HOME"),getpid());
  if(!FileStat(Tfolder)) {
    mkdir(Tfolder,0700);
    printf("Created: %s\n",Tfolder);
    Fstat=0;
  }
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
  if( (Oxres > Bxres )||( fabsf(Bfps -Ofps)> 1)){

     MakeFileInFolder(olay,Tfolder,Tmp1,"mp4");
     printf("ChangeVideoSizeAndFrate\n");
     fflush(stdout);
     ChangeVideoSizeAndFrate(Otmp,Tmp1,Bxres,-2,(int)(Bfps+0.5),Qty);
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
  if(Fstat==0) kgCleanDir(Tfolder);
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
  char Tfolder[300],Tmpfile[300];
  int Fstat=1;
  sprintf(Tfolder,"%-s/%-d",getenv("HOME"),getpid());
  if(!FileStat(Tfolder)) {
    mkdir(Tfolder,0700);
    printf ("Created : %s\n",Tfolder);
    Fstat=0;
  }
  MakeFileInFolder("/tmp/Blank.mp4",Tfolder,Tmpfile,"mp4");
  printf("Tmpfile: %s: %s\n",Tmpfile,olay);
  fflush(stdout);
  mpt = GetMediaInfo(olay);
  if(mpt->Video != 1) return 0;
  printf("Calling CreateBlankVideo: %-.3f\n",mpt->TotSec);
  fflush(stdout);
  CreateBlankVideo(Bxres,Byres,(float)mpt->TotSec,fs,Tmpfile);
  printf("Tmpfile: %s\n",Tmpfile);
  fflush(stdout);
  free(mpt);
  mpt = NULL;
  
  printf("Calling OverlayVideos\n");
  fflush(stdout);
  OverlayVideos(Tmpfile,olay,Qty,outfile);
  printf("OverlayVideos: %s\n",outfile);
  fflush(stdout);
//  if(Fstat==0) kgCleanDir(Tfolder);
  return 1;
}
int CreateStillVideo(char *infile,float duration,float fps,char *outfile) {
    char buff[500];
    sprintf(buff,"ffmpegfun  -y  -loop 1 -i %s  -t %.2f -f mp4  -vf fps=%-.3f -vcodec libx264  %s",
      infile,duration,fps,outfile);
    runfunction(buff,NULL,ffmpegfun);
    return 1;
}
int CreateBlankVideo(int Xsize,int Ysize,float duration,float fps,char *outfile) {
    char buff[500],Infile[300], folder[300],Tmpfile[300];
    int Fstat=1;
    sprintf(folder,"%-s/%-d",getenv("HOME"),getpid());
    if(!FileStat(folder)) {
      mkdir(folder,0700);
      printf("Created: %s\n",folder);
      sleep(5);
      Fstat=0;
    }
    else printf("Folder %s exists..\n");
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
    sprintf(buff,"ffmpegfun  -y  -loop 1 -i %s  -t %.2f -f mp4 -vf fps=%-.3f -vcodec libx264  %s",
      Infile,duration,fps,outfile);
    printf("%s\n",buff);
    fflush(stdout);
    runfunction(buff,ProcessPrint,ffmpegfun);
//    RunString(buff,ffmpegfun);
    if(FileStat(outfile))printf("created BLANK VIDEO: %s  sleeping..\n",outfile);
    else printf("Failed tp create %s\n",outfile);
    fflush(stdout);
//    if(Fstat==0) kgCleanDir(folder);
    return 1;
}
int GetFirstFrame(char *infile,char *outfile) {
   char buff[500];
   sprintf(buff,"ffmpegfun  -y  -ss 00:00:0.1 -i %s -frames:v 1 %s",infile,outfile);
   RunAndWait(buff);
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
   char buff[500],Tmp[100],Ffile[200],Sfile[500];
   MEDIAINFO *mpt;
   mpt = GetMediaInfo(infile);
   if(mpt->Video != 1) return 0;
   strcpy(Tmp,(char *)"/tmp");
   MakeFileInFolder(infile,Tmp,Ffile,(char *)"png");
   GetFirstFrame(infile,Ffile);  
   MakeFileInFolder(infile,Tmp,Sfile,(char *)"mp4");
   CreateStillVideo(Ffile,duration,mpt->fps,Sfile);
   JoinTwoVideos(Sfile,infile,outfile);   
   remove(Ffile);
   remove(Sfile);
  free(mpt);
  mpt = NULL;
  return 1;
}
