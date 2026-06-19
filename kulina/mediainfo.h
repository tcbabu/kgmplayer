#ifndef D_MINFO
#define D_MINFO
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
  int vcodec;
  float rotation;
  float start;
  char SAR[32];
  char DAR[32];
  char vcodectype[40];
} MEDIAINFO;

MEDIAINFO * GetMediaInfo(char *flname); //in kgutils.h

#endif
