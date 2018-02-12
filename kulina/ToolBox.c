#include <kulina.h>
#include "ToolBoxCallbacks.h"
#include "ConvertData.h"
CONVDATA cndata;
int AConGrp,RangeGrp,EnVoGrp,VConGrp,
    VaspGrp,VsizeGrp,VrangeGrp,VJGrp,AJGrp,AddGrp,MixGrp,AmixGrp,KarGrp,
    SilGrp,CutGrp;
int MakeAudioConvertGroup(DIALOG *D,void *arg);
int MakeRangeGroup(DIALOG *D,void *arg);
int MakeEnVolumeGroup(DIALOG *D,void *arg);
int MakeConvertVideoGroup(DIALOG *D,void *arg);
int MakeVaspGroup(DIALOG *D,void *arg);
int MakeVsizeGroup(DIALOG *D,void *arg);
int MakeVrangeGroup(DIALOG *D,void *arg);
int MakeVideoJoinGroup(DIALOG *D,void *arg);
int MakeAudioJoinGroup(DIALOG *D,void *arg);
int MakeAddAudioGroup(DIALOG *D,void *arg);
int MakeMixAudioGroup(DIALOG *D,void *arg);
int MakeAmixAudioGroup(DIALOG *D,void *arg);
int MakeMakeKaraokeGroup(DIALOG *D,void *arg);
int MakeAddSillenceGrp(DIALOG *D,void *arg);
int MakeMakeCutseGrp(DIALOG *D,void *arg);
int InitConvertData(CONVDATA *cnd) {
  cnd->infile[0]='\0';
  cnd->outfile[0]='\0';
  cnd->Quality=2;
  cnd->VQuality=2;
  cnd->VolEnh=3;
  cnd->Enhfac=1.0;
  cnd->FullRange=1;
  cnd->StartSec=0;
  cnd->EndSec=0;
  cnd->VFullRange=1;
  cnd->VStartSec=0;
  cnd->VEndSec=0;
  cnd->ChngAsp=0;
  cnd->Scale=0;
  cnd->OldAsp=1;
  cnd->NewAsp=1;
  cnd->Xsize=640;
  cnd->Ysize=480;
  cnd->fps=24;
  cnd->Vlist=NULL;
  cnd->Alist=NULL;
}
void ModifyToolBoxGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int ToolBoxGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  char *menu0[]  = { 
    (char *)"Select Action",
    (char *)"Convert/Extract Audio",
    (char *)"Convert Video to mp4",
    (char *)"Join Video Files",
    (char *)"Join Audio from Media Files",
    (char *)"Add/Change Media audio ",
    (char *)"Mix Audio to Media",
    (char *)"Mix Audios from two media",
    (char *)"Karaoke from stereo audio",
    (char *)"Insert Silences to audio",
    (char *)"Make Cuts in  audio",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r0 = { 
    'r',
    7,2,  
    195,240,   
    8,0,  
    90, 
    25, 
    1,6, 
    0,6, 
    (int *)v[0], 
    NULL, 
    NULL ,
    NULL,ToolBoxbrowser1callback, /* *args, callback */
    2,  /* Border Offset  */
     16,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu0);
  r0.list=(void **)th0;
  strcpy(r0.Wid,(char *)"ToolsWidget1");
  r0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Go Back");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-83201083;
  butn1[0].butncode='';
  DIL h1 = { 
    'h',
    51,240,  
    145,270,
    2,0,  
    84, 
    25, 
    1,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    ToolBoxsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h1.Wid,(char *)"ToolsWidget2");
  h1.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+3));
  d =dtmp+i; 
  d[2].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIRA));
  ToolBoxbrowser1init(&r0,pt) ;
  *d[0].r = r0;
  d[0].r->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIL));
  ToolBoxsplbutton1init(&h1,pt) ;
  *d[1].h = h1;
  d[1].h->item = -1;
  d[2].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeToolBoxGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    RadioButtons1  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   void** v=(void **)malloc(sizeof(void*)*2);
   v[1]=NULL;
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = ToolBoxGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int ToolBox( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  InitConvertData(&cndata);
  D.d=NULL;
  GrpId = ToolBoxGroup(&D,v,pt);
  AConGrp= MakeAudioConvertGroup(&D,NULL);
  RangeGrp= MakeRangeGroup(&D,NULL);
  EnVoGrp= MakeEnVolumeGroup(&D,NULL);
  VConGrp = MakeConvertVideoGroup(&D,NULL);
  VaspGrp = MakeVaspGroup(&D,NULL);
  VsizeGrp = MakeVsizeGroup(&D,NULL);
  VrangeGrp = MakeVrangeGroup(&D,NULL);
  VJGrp = MakeVideoJoinGroup(&D,NULL);
  AJGrp = MakeAudioJoinGroup(&D,NULL);
  AddGrp =  MakeAddAudioGroup(&D,NULL);
  MixGrp =  MakeMixAudioGroup(&D,NULL);
  AmixGrp =  MakeAmixAudioGroup(&D,NULL);
  KarGrp =  MakeMakeKaraokeGroup(&D,NULL);
  SilGrp =  MakeAddSilenceGroup(&D,NULL);
  CutGrp =  MakeMakeCutsGroup(&D,NULL);
  kgShiftGrp(&D,VaspGrp,0,-3);
  kgShiftGrp(&D,VsizeGrp,0,2);
  kgShiftGrp(&D,VrangeGrp,0,-5);
  kgShiftGrp(&D,VConGrp,3,-2);
  kgShiftGrp(&D,AConGrp,190,0);
  kgShiftGrp(&D,EnVoGrp,0,10);
  kgSetGrpVisibility(&D,AConGrp,0);
  kgSetGrpVisibility(&D,RangeGrp,0);
  kgSetGrpVisibility(&D,EnVoGrp,0);
  kgSetGrpVisibility(&D,VConGrp,0);
  kgSetGrpVisibility(&D,VaspGrp,0);
  kgSetGrpVisibility(&D,VsizeGrp,0);
  kgSetGrpVisibility(&D,VrangeGrp,0);
  kgSetGrpVisibility(&D,VJGrp,0);
  kgSetGrpVisibility(&D,AJGrp,0);
  kgSetGrpVisibility(&D,AddGrp,0);
  kgSetGrpVisibility(&D,MixGrp,0);
  kgSetGrpVisibility(&D,AmixGrp,0);
  kgSetGrpVisibility(&D,KarGrp,0);
  kgSetGrpVisibility(&D,SilGrp,0);
  kgSetGrpVisibility(&D,CutGrp,0);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 2;   /* Position of Dialog */ 
  D.yo = 2;
  D.xl = 631;    /*  Length of Dialog */
  D.yl = 276;    /*  Width  of Dialog */
  D.Initfun = ToolBoxinit;    /*   init fuction for Dialog */
  D.Cleanupfun = ToolBoxcleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = ToolBoxCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = ToolBoxResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = ToolBoxWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyToolBoxGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunToolBox(void *arg) {
/*************************************************

    RadioButtons1  1 data value

*************************************************/
   int   v0 = 1;
   void* v[1];
   v[0]=(void *)(&v0);
   void *pt=NULL; /* pointer to send any extra information */
   ToolBox(arg,v,pt );
   return NULL;
}
