#ifndef D_UTILS
#define D_UTILS
#ifndef D_MINFO
#include "mediainfo.h"
#endif
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int ProcessSkip(int pip0,int pip1,int Pid) ;
int ProcessToPipe(int pip0,int pip1,int Pid) ;
void *RunMonitorJoin(void *arg);
void *Runmonitor(void *arg);
void *RunAndMonitor(void *arg);
void *RunMonitorAndWait(void *arg);
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
int ChangeVideoSizeAndFrate(char *infile,char *outfile,int Xres,int Yres,int fs);
int ChangeVideoSize(char *infile,char *outfile,int Xres,int Yres);
int ChangeVideoFrate(char *infile,char *outfile,int fs);
#endif

