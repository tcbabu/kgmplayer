#ifndef D_UTILS
#define D_UTILS
#ifndef D_MINFO
#include "mediainfo.h"
#endif
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int RunString(char *job,int (*function)(int,char **));
int ProcessSkip(int pip0,int pip1,int Pid) ;
int ProcessToPipe(int pip0,int pip1,int Pid) ;
void *RunMonitorJoin(void *arg);
void *Runmonitor(void *arg);
int RunAndMonitor(char  *arg);
int RunMonitorAndWait(char *arg);
int RunAndWait(char * job);
int MakeNewFileName(char *infile,char *outfile);
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
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
int ChangeVideoSizeAndFrate(char *infile,char *outfile,int Xres,int Yres,int fs,int Qty);
int ChangeVideoSize(char *infile,char *outfile,int Xres,int Yres,int Qty);
int ChangeVideoFrate(char *infile,char *outfile,int fs,int Qty);
int ConverToLibx265(char *infile,int Qty,char *outfile);
int ConverToLibx264(char *infile,int Qty,char *outfile);
int OverlayVideos(char *base,char *olay,int Qty,char *outfile);
int OverlayToSize(int Bxres,int Byres,float fps,int Qty,char *olay,char *outfile) ;
int CreateStillVideo(char *infile,float duration,float fps,char *outfile);
int CreateBlankVideo(int Xsize,int Ysize,float duration,float fps,char *outfile);
int GetFirstFrame(char *infile,char *outfile) ;
int JoinTwoVideos(char *infile1,char *infile2,char *outfile);
int AddStillAtStart(char *infile,float  duration,char *outfile);
#endif

