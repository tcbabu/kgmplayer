#ifndef D_MINFO
#define M_INFO
typedef struct _mediainfo {
  char Flname[500];
  int Video,Audio;
  float TotSec;
  float AspectNu,AspectDe;
  int Axres,Ayres;
  int Rxres,Ryres;
  float fps;
  int Process;
  char *Folder;
  int id;
  int Achnls;
  int SubTitle; // specify subtitle
  char SubFile[500];
  char *SubFolder;
  int SubTitleOff; // swich off subtitle
} MEDIAINFO;
#endif
