#ifndef D_UTILS
#define D_UTILS
#ifndef D_MINFO
#include "mediainfo.h"
#endif
int CreateImagesVideo ( char **L ,float fps,char *Output);
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int ExecFunction(char *job,int (*function)(int,char **));
int runfunctionbkgr(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int RunString(char *job,int (*function)(int,char **));
int ProcessSkip(int pip0,int pip1,int Pid) ;
int ProcessToPipe(int pip0,int pip1,int Pid) ;
int ProcessOutput(int pip0,int pip1,int Pid) ;
int ProcessData(int pip0,int pip1,int Pid) ;
void *RunMonitorJoin(void *arg);
void *Runmonitor(void *arg);
int RunAndMonitor(char  *arg);
int RunMonitorAndWait(char *arg);
int RunAndWait(char * job);
int RunFunctionAndMonitor(char * job,int (*function)(int,char **));
int RunFunctionAndWait(char * job,int (*function)(int,char **));
int MakeNewFileName(char *infile,char *outfile);
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int MakeFileNameInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int GetFolderName(char *infile,char *folder);
char *MakeTmpFolder(void) ;
int GetBaseIndex(char *s);
int FileStat(char *flname);
int SearchString(char *s1,char *s2);
int GetTimedLine(int pip0,char *buff,int usec);
int GetLine(int pip0,char *buff);
int CheckMedia(char *flname);
int GetVideoInfo(char *flname); // Info in Minfo
//MEDIAINFO *GetMediaInfo(char *flname);
int AudioExtract(char *infile,char *outfile);
int AudioChange(char *media,char *audio,char *outfile);
int ChangeVideoSizeAndFrate(char *infile,char *outfile,int Xres,int Yres,int fs,int Qty);
int ChangeVideoSize(char *infile,char *outfile,int Xres,int Yres,int Qty);
int ChangeVideoFrate(char *infile,char *outfile,int fs,int Qty);
int ConverToLibx265(char *infile,int Qty,char *outfile);
int ConverToLibx264(char *infile,int Qty,char *outfile);
int OverlayVideos(char *base,char *olay,int Qty,char *outfile);
int MergeVideos(char *base,char *olay,float transparency,char *outfile);
int OverlayToSize(int Bxres,int Byres,float fps,int Qty,int ir,int ig,int ib,char *olay,char *outfile) ;
int RunOverlayToSize(int argc,char **argv);
int CreateStillVideo(char *infile,float duration,float fps,char *outfile);
int CreateBlankVideo(int Xsize,int Ysize,float duration,float fps,char *outfile);
int CreateColorVideo(int Xsize,int Ysize,float duration,float fps,int ir,int ig,int ib,char *outfile);
int GetFirstFrame(char *infile,char *outfile) ;
int GetLastFrame(char *infile,char *outfile) ;
int JoinTwoVideos(char *infile1,char *infile2,char *outfile);
int AddStillAtStart(char *infile,float  duration,char *outfile);
int AddStillAtEnd(char *infile,float  duration,char *outfile);
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int RunFunction(char *job,int (*ProcessOut)(void *,int,int,int),int
             (*function)(int,char **),void *);
int kgffmpeg(int,char **);
int ffmpegfun(int,char **);
int ProcessSkip(int pip0,int pip1,int Pid);
int ProcessPrint(int pip0,int pip1,int Pid);
int ProcessData(int pip0,int pip1,int Pid);
int ResetGrpVis(void *Tmp);
int MakeTmpFolderInHome(char *Tfolder);
int GetWavHeaderLength(char *flname);
int AudioToWav(char *infile,char *outfile) ;
int AudioReformat(char *infile,char *outfile) ;
int CreateSilentAudio(float duration,char *outfile) ;
int JoinTwoAudio(char *infile1,char *infile2,char *outfile) ;
int AddSilenceAtStart(char *infile,float duration,char *outfile) ;
int AddSilenceAtEnd(char *infile,float duration,char *outfile) ;
int GetTimeString(float duration,char *Tstring);
int GetFirstFrame(char *infile,char *outfile);
int GetlastFrame(char *infile,char *outfile);
 int RunAddStillAtStart(int argc,char **argv);
 int RunAddStillAtEnd(int argc,char **argv);
int MixTwoAudios( char *infile1,char *infile2,char *outfile);
int FileSize(char *flname);
int VideoCrop(char *infile,int xo,int yo,int xres,int yres,char *outfile);
int RunVideoCrop(int argc,char **argv);
ThumbNail **AddItemtoVlist(char *newitem) ;
ThumbNail **AddItemstoVlist(char **newitems) ;
ThumbNail **DeleteItemsfromVlist(void) ;
int VideoTopBottom(char *infile1,char *infile2,char *outfile);
int VideoSideBySide(char *infile1,char *infile2,char *outfile);
void *CreateColorImage(int Xsize,int Ysize,int red,int green,int blue);
int kgwrite(int argc,char *argv[]);
int GetTimeStamps(char *vfile,char *ptsfile);
int UpdateVideoImages ( char *Vfile,char **L, char *Outfile );
#endif

