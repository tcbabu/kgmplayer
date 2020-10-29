#ifndef D_UTILS
#define D_UTILS
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int ProcessToPipe(int pip0,int pip1,int Pid) ;
void *RunMonitorJoin(void *arg);
void *Runmonitor(void *arg);
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int GetBaseIndex(char *s);
int FileStat(char *flname);
int SearchString(char *s1,char *s2);
int GetTimedLine(int pip0,char *buff,int usec);
int GetLine(int pip0,char *buff);
int CheckMedia(char *flname);
#endif

