#ifndef D_ToolGroup
#define D_ToolGrp
#include "kulina.h"
typedef struct toolgrp_ {
  int GrpId;
  int (*MakeGroup)(DIALOG *D,void *arg);
  char *disc; // menu title
  int xsh,ysh; // shift info
  void * (*RunHelp)(void *arg);
  int flag; // 0 or 1 , 1 means independent dialog
  char *MsgImg;
} TOOLGRP;
#endif
