#include <kulina.h>
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
int CreateStillVideo(char *infile,float duration,char *outfile) {
    char buff[500];
    sprintf(buff,"ffmpegfun  -y  -loop 1 -i %s  -t %.2f -f mp4  %s",
      infile,duration,outfile);
    RunMonitorAndWait(buff);
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
   strcpy(Tmp,(char *)"/tmp");
   MakeFileInFolder(infile,Tmp,Ffile,(char *)"png");
   GetFirstFrame(infile,Ffile);  
   MakeFileInFolder(infile,Tmp,Sfile,(char *)"mp4");
   CreateStillVideo(Ffile,duration,Sfile);
   JoinTwoVideos(Sfile,infile,outfile);   
   remove(Ffile);
   remove(Sfile);
  return 1;
}
