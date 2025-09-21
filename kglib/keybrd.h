#ifndef D_KEYBRD
#define D_KEYBRD
typedef struct _keybrd {
  void *D;
  int GrpId;
  void *Coff,*Con,*Lon,*Loff;
  int sgrp,cgrp;
  int offgrp,ongrp;
  int CapsLock;
  int ShiftPress;
  int CurWid;
  int Vis;
  int Btype;
} KEYBRD;
int kgMakeKeybrd(DIALOG *D,int xo,int yo,int Vis,int btype,int bfont,int fontclr,float transparency) ;
int kgMakeDefaultKeybrd(DIALOG *D,int xo,int yo,int vis);
int kgSetKeybrdWidget(int curwid);
int kgHideKeybrd(void);
int kgShowKeybrd(void);
#endif
