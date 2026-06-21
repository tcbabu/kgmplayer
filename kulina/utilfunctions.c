#include <kulina.h>
#include <math.h>
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


int ChangeVideoSizeAndFrate(char *infile,char *outfile,int Xres,int Yres,int fs){
  char buff[500];
  sprintf(buff,"ffmpegfun -y  -i %s -vf \"scale=%d:%d:flags=lanczos,setsar=1,fps=%-d\" "
       " -crf 20  -preset medium -c:v libx265 %s",
       infile,(Xres/2)*2,(Yres/2)*2,fs,outfile);
  runfunction(buff,NULL,ffmpegfun);
  return 1;
}
int ChangeVideoSize(char *infile,char *outfile,int Xres,int Yres){
  char buff[500];
  sprintf(buff,"ffmpegfun -y  -i %s -vf \"scale=%d:%d:flags=lanczos,setsat=1\" "
       " -crf 20  -preset medium -c:v libx265 %s",
       infile,(Xres/2)*2,(Yres/2)*2,outfile);
  runfunction(buff,NULL,ffmpegfun);
  return 1;
}
int ChangeVideoFrate(char *infile,char *outfile,int fs){
  char buff[500];
  sprintf(buff,"ffmpegfun -y  -i %s -vf \"fps=%-d\" "
       " -crf 20  -preset medium-c:v libx265 %s",
       infile,fs,outfile);
  runfunction(buff,NULL,ffmpegfun);
  return 1;
}
int ConvertToLibx265(char *infile,char *outfile){
  char buff[500];
  sprintf(buff,"ffmpegfun -y  -i %s -f mp4"
       " -crf 20  -preset medium-c:v libx265 %s",
       infile,outfile);
  runfunction(buff,NULL,ffmpegfun);
  return 1;
}
int ConvertToLibx264(char *infile,char *outfile){
  char buff[500];
  sprintf(buff,"ffmpegfun -y  -i %s -f mp4"
       " -crf 18  -preset medium-c:v libx264 %s",
       infile,outfile);
  runfunction(buff,NULL,ffmpegfun);
  return 1;
}

int OverlayVideos(char *base,char *olay,char *outfile) {
  /*
     Overlays olay over base, in centralised way
     in case olay is bigger in dimension it will be resize to fit
     aspect ratio will be maintained
  */
  int Bxres,Byres,Oxres,Oyres;
  float Bfps,Ofps;
  int xloc,yloc;
  char Bvcodec[30],Ovcodec[30];
  char Tmp1[200],Tmp2[200],Tmp3[200],Otmp[200],Btmp[200];
  char buff[500];
  char *Tfolder,Err[400];
  Tfolder = MakeTmpFolder();
  strcpy(Err,Tfolder);
  strcat(Err,"/Err");
  FILE *fp = fopen(Err,"w");
  fprintf(fp,"Opened Err\n");
  printf("Opened Err\n");
  fflush(stdout);
  fclose(fp);
  fp = fopen(Err,"a");
  strcpy(Otmp,olay);
  GetVideoInfo(base);
  if(Minfo.Video != 1) return 0;
  Bxres =Minfo.Axres;
  Byres =Minfo.Ayres;
  Bfps  = Minfo.fps;
  strcpy(Bvcodec,Minfo.vcodectype);
  Btmp[0]='\0';
  fprintf(fp,"%s \n",Bvcodec);
  fclose(fp);
  fp = fopen(Err,"a");
  strcpy(Btmp,base);
  if ((strcmp(Bvcodec,"h264") != 0)&&(strcmp(Bvcodec,"h265")!= 0)) {
     fprintf(fp,"not h264 or h265\n");
     fflush(fp);
     MakeNewFileName(base,Btmp);
     ConvertToLibx264(base,Btmp);
     GetVideoInfo(Btmp);
     if(Minfo.Video != 1) return 0;
     Bxres =Minfo.Axres;
     Byres =Minfo.Ayres;
     Bfps  = Minfo.fps;
     strcpy(Bvcodec,Minfo.vcodectype);
  }
  fprintf(fp,"Processing:%s: \n",olay);
  fclose(fp);
  printf("Opened Err 2\n");
  fflush(stdout);
  fp = fopen(Err,"a");
  GetVideoInfo(olay);
  if(Minfo.Video != 1) return 0;
  fprintf(fp,"%s: %d %d %d %s\n",olay,Minfo.Video,Minfo.Axres,Minfo.Ayres,Minfo.vcodectype);
  fclose(fp);
  printf("Opened Err 3\n");
  fflush(stdout);
  fp = fopen(Err,"a");
  Oxres =Minfo.Axres;
  printf("Oxres= %d\n",Oxres);
  fflush(stdout);
  fprintf(Err,"Opened1 :%s\n",olay);
  fclose(fp);
  printf("Opened Err 4\n");
  fflush(stdout);
  
  fp = fopen(Err,"a");
  Oyres =Minfo.Ayres;
  fprintf(Err,"Opened 2 :%s\n",olay);
  fclose(fp);
  fp = fopen(Err,"a");
  Ofps  = Minfo.fps;
  fprintf(Err,"Opened3 :%s\n",olay);
  fclose(fp);
  fp = fopen(Err,"a");
  strcpy(Ovcodec,Minfo.vcodectype);
  fprintf(Err,"Opened 4 :%s\n",olay);
  fclose(fp);
  fp = fopen(Err,"a");
  Tmp1[0]='\0';
  Tmp2[0]='\0';
  printf("Opened Err 5\n");
  fflush(stdout);
  if( (Oxres > Bxres )||( fabsf(Bfps -Ofps)> 1)){

     fprintf(fp,"(Oxres > Bxres )||( fabsf(Bfps -Ofps)> 1)\n");
     fflush(fp);
     MakeNewFileName(olay,Tmp1);
     ChangeVideoSizeAndFrate(Otmp,Tmp1,Bxres,-2,(int)(Bfps+0.5));
     strcpy(Otmp,Tmp1);
     GetVideoInfo(Otmp);
     if(Minfo.Video != 1) return 0;
     Oxres =Minfo.Axres;
     Oyres =Minfo.Ayres;
     Ofps  = Minfo.fps;
     strcpy(Ovcodec,Minfo.vcodectype);
  }   
  printf("Opened Err 6\n");
  fflush(stdout);
  fprintf(Err,"I am here :%s\n",olay);
  fclose(fp);
  fp = fopen(Err,"a");
  if( Oyres > Byres ){
     fprintf(fp,"Oyres > Byres \n");
     fflush(fp);
     MakeNewFileName(olay,Tmp2);
     ChangeVideoSize(Otmp,Tmp2,-2,Byres);
     strcpy(Otmp,Tmp2);
     GetVideoInfo(Otmp);
     if(Minfo.Video != 1) return 0;
     Oxres =Minfo.Axres;
     Oyres =Minfo.Ayres;
     Ofps  = Minfo.fps;
     strcpy(Ovcodec,Minfo.vcodectype);
  }   
  Tmp3[0]='\0';  
  printf("Opened Err 7\n");
  fflush(stdout);
  if ((strcmp(Ovcodec,"h264") != 0)&&(strcmp(Ovcodec,"h265")!= 0) ){
     MakeNewFileName(olay,Tmp3);
     ConvertToLibx264(Otmp,Tmp3);
     strcpy(Otmp,Tmp3);
     GetVideoInfo(Otmp);
     if(Minfo.Video != 1) return 0;
     Oxres =Minfo.Axres;
     Oyres =Minfo.Ayres;
     Ofps  = Minfo.fps;
     strcpy(Ovcodec,Minfo.vcodectype);
  }
  printf("Opened Err 7\n");
  fflush(stdout);
  fprintf(fp,"Creating buff\n");
  xloc = (Bxres - Oxres)/2;
  yloc = (Byres - Oyres)/2;
  printf("Btmp : %s Otmp : %s : %d %d %s\n",Btmp,Otmp,xloc,yloc,outfile);
  fflush(stdout);
  sprintf(buff,"ffmpegfun -y -i %s  -i %s -filter_complex \"[0:v][1:v]overlay=%-d:%-d\" %s",
       Btmp,Otmp,xloc,yloc,outfile);
  printf("buff : %s\n",buff);
  fflush(stdout);
     fprintf(fp,"%s\n",buff);
     fclose(fp);
  runfunction(buff,ProcessPrint,ffmpegfun);
#if 0
  if(Tmp1[0] != '\0') remove(Tmp1);
  if(Tmp3[0] != '\0') remove(Tmp2);
  if(Tmp3[0] != '\0') remove(Tmp3);
  if(Btmp[0] != '\0') remove(Btmp);
#endif  
  return 1;
}
int OverlayToSize(int Bxres,int Byres,float fs,char *olay,char *outfile) {
  /*
     Overlays olay over base, in centralised way
     in case olay is bigger in dimension it will be resize to fit
     aspect ratio will be maintained
  */
  int Oxres,Oyres;
  float Bfps,Ofps,xloc,yloc;
  char Bvcodec[30],Ovcodec[30];
  char Tmp1[200],Tmp2[200],Tmp3[200],Otmp[200],Btmp[200];
  char buff[500],Tmpbase[300],base[300];
  char *Tfolder,Tmpfile[300];
  Tfolder = MakeTmpFolder();
  strcpy(Tmpfile,Tfolder);
  strcat(Tmpfile,"/Blank.mp4");
  GetVideoInfo(olay);
  if(Minfo.Video != 1) return 0;
  CreateBlankVideo(Bxres,Byres,(float)Minfo.TotSec,fs,Tmpfile);
  
//  kgCleanDir(Tfolder);
  free(Tfolder);
  OverlayVideos(Tmpfile,olay,outfile);
  return 1;
}
int CreateStillVideo(char *infile,float duration,float fps,char *outfile) {
    char buff[500];
    sprintf(buff,"ffmpegfun  -y  -loop 1 -i %s  -t %.2f -f mp4  -vf fps=%-.3f -vcodec libx264  %s",
      infile,duration,fps,outfile);
//    RunMonitorAndWait(buff);
    runfunction(buff,NULL,ffmpegfun);
    return 1;
}
int CreateBlankVideo(int Xsize,int Ysize,float duration,float fps,char *outfile) {
    char buff[500],Infile[300], *folder,Err[300];
    void *img=kgCreateImage(Xsize,Ysize);
    folder = MakeTmpFolder();
    MakeFileInFolder(outfile,folder,Infile,"png");
    kgWriteImage(img,Infile);
    kgCloseImage(img);
    MakeFileInFolder(outfile,folder,Err,"txt");
//    kgCleanDir(folder);
//    free(folder);
    sprintf(buff,"ffmpegfun  -y  -loop 1 -i %s  -t %.2f -f mp4 -vf fps=%-.3f -vcodec libx264  %s",
      Infile,duration,fps,outfile);
    FILE *fp=fopen(Err,"w");
    fprintf(fp,"%s\n",buff);
    fclose(fp);
    RunMonitorAndWait(buff);
//    runfunction(buff,NULL,ffmpegfun);
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
   RunMonitorAndWait(buff);
   return 1;
}
int AddStillAtStart(char *infile,float  duration,char *outfile) {
   char buff[500],Tmp[100],Ffile[200],Sfile[500];
   GetVideoInfo(infile);
   if(Minfo.Video != 1) return 0;
   strcpy(Tmp,(char *)"/tmp");
   MakeFileInFolder(infile,Tmp,Ffile,(char *)"png");
   GetFirstFrame(infile,Ffile);  
   MakeFileInFolder(infile,Tmp,Sfile,(char *)"mp4");
   CreateStillVideo(Ffile,duration,Minfo.fps,Sfile);
   JoinTwoVideos(Sfile,infile,outfile);   
   remove(Ffile);
   remove(Sfile);
  return 1;
}
