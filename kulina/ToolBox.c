#define D_HELPGRP
#include <kulina.h>
#include "ToolBoxCallbacks.h"
#include "ConvertData.h"
#include "ToolGroup.h"


CONVDATA cndata;
int AConGrp,RangeGrp,EnVoGrp,VConGrp,
    VaspGrp,VsizeGrp,VrangeGrp,VJGrp,AJGrp,AddGrp,MixGrp,AmixGrp,KarGrp,
    SilGrp,CutGrp,Is2vGrp;
int ToolHelpGrp,HelpButnGrp,ImageBoxGrp;
char *kgCopyString(char *str);
int SetGrpVis(DIALOG *Tmp,TOOLGRP *T,int item);

void *RunHelper(void * arg);
extern char *HelpMsgs[4];

int MakeToolHelpGroup(DIALOG *D,void *arg);
int MakeHelpButnGroup(DIALOG *D,void *arg);
int MakeImageBoxGroup(DIALOG *D,void *arg);

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
int MakeAddSilenceGroup(DIALOG *D,void *arg);
int MakeMakeCutsGroup(DIALOG *D,void *arg);
int Makeimgs2vGroup(DIALOG *D,void *arg);
int MakenormalizeGroup(DIALOG *D,void *arg);
int MakevtobwGroup(DIALOG *D,void *arg);
int MakevflipGroup(DIALOG *D,void *arg);
int MakehflipGroup(DIALOG *D,void *arg);
int MakesliceGroup(DIALOG *D,void *arg);
int MakeNormaliseGroup(DIALOG *D,void *arg);

TOOLGRP ToolList[ ] = {
  { 0, MakeAudioConvertGroup,"!w32!f23Convert/Extract Audio",346,40,RunHelper,0,
       "\nYou can extract audio from a media file\n"
       "One can also use it to convert audio to a \n"
       "different format."
       "The extension decides the \n"
       "OUTPUT format. "
       "If it is planned to cut and \n"
       "join audio for further use,\n"
       "then a lossless format like WAV or FLAC \n"
       "may be preferred\n"
       "\nOne can also extract a portion, \n"
       "a negetive value "
       "for end of selection can be \n"
       "used to discard from end\n"
       "If CUTS and JOINS are planned opt for \n"
       "a lossless "
       "format like wav or flac\n"
  },

  { 0, MakeConvertVideoGroup,"!w32!f23Convert Video to mp4",340,0,RunHelper,0,
      "\n\n\nConverting video to mp4 can reduce "
	      "the file size substantially\n"
      "without much loss of quality. One can opt \n"
      "for different compression\n"
      "levels by changing the quality level. \n"
      "Higher quality is at the expense\n"
      "of file size, but it is generally affordable.\n"
      " If there is a plan to edit further, \n"
      "opt for the best quality.\n"
      "\nThanks to libx264/265, generally a good job is done\n"
      "\n Aspect value 1 means the original aspect ratio,\n"
      "greater than (>) 1.0 means a decrease in y resolution\n"
      "less thsn (<) 1.0 means an increase in y resolution\n"
      "\n Aspect ratio is Xres/Yres and a factor 1.0 means\n"
      " fac = (Xres/Yres)/(AspRatio) where AspRatio is original\n"
      "aspect ratio, so a factor greater than 1.0 means a decrease\n"
      "new Yres and a value < 1.0 means an increase in new Yres\n"
      "So new Yres if adjusted to maintain the new aspect ratio\n"
  },
  { 0, MakeVideoJoinGroup,"!w32!f23Join Video Files",200,40 ,RunHelper,0,
      "\nMuliple video files can be joined together to a single one\n"
      "Files can be of diffrent aspect ratio or format and the output\n"
      "is a file with aspect ratio\n"
  },
  { 0, MakeAudioJoinGroup,"!w32!f23Join Audio from Media Files",200,40,RunHelper,0,
      "\n\n\nJoin audios from multiple input files (can be audio or video)\n"
      "If there is a plan to edit further, select output format as lossless\n"
      "audio, either wav or flac\n"
  },
  { 0, MakeAddAudioGroup,"!w32!f23Add/Change Media audio ",200,40,RunHelper,0,
      "\n\n\nAdd (or replace) audio to a video file\n"
      "If there is already an audio in the media it will be discarded\n"
      "and new audio will be inserted\n"
  },
  { 0, MakeMixAudioGroup,"!w32!f23Mix Audio to Media",200,40,RunHelper,0,
      "\n\n\nThis tool can be used to mix an audio to an existing\n"
      "audio in a media(video or audio) file.\n"
  },
  { 0, MakeAmixAudioGroup,"!w32!f23Mix Audios from two media",200,40,RunHelper,0,
      "\n\n\nThis tool can be used to mix two audio inputs from two media\n"
      "files (audio or video) and creates a new audio file\n"
      "If further editing to audio is planned, select a lossless format\n"
      "for output. By default it is given as mp3, one can change it to\n"
      "wav or flac by changing the output extension\n"
  },

  { 0, MakeMakeKaraokeGroup,"!w32!f23Karaoke from stereo audio",200,40,RunHelper,0,
      "\n\n\nKaraoke may not be successful always\n"
      "It works only in certain type of stero recording\n"
      "One can just make a try\n",
  },
  { 0, MakeAddSilenceGroup,"!w32!f23Insert Silences to audio",180,40,RunHelper,0,
       "\n\n\nInserting silence in audio is useful in mixing audio\n"
  },
  { 0, MakeMakeCutsGroup,"!w32!f23Make Cuts in  audio",340,40,RunHelper,0,
       "\n\n\nMaking cuts in audio is useful in mixing audio\n"
       "or adding audio to media\n"
  },

  { 0, Makeimgs2vGroup,"!w32!f23Images to Video",346,40,RunHelper,0,
       "Useful in creating a video from a set of photographs or images\n"
       "May not be the one for creating video from frames, though one\n"
       "can attempt by reducing time for each image, probably for low\n"
       "quality video for cartoons\n"
       "Another tool is there for making video from frames.\n"
  },
  /*
  { 0, MakenormalizeGroup,"!w32!f23Normalize Audio File Volume",346,40,RunHelper,0,
       "\nCan be used to normalize volume to a common level for audio\n"
       "the maximum volume level is 0, hence levels are always negetive\n"
       "only one file can be selected at a time\n"
       "Convert one by one to make same level for a set of media files\n"
       "Output is an audio file, in case one need to change in the media\n"
       "use the tool for changing media audio, using the output audio.\n"
       "Remember lossless formats are best for further editing.\n"
  },
  */
  { 0, MakeNormaliseGroup,"!w32!f23Normalize Audio Files Volume",346,40,RunHelper,0,
       "\nCan be used to normalize volume to a common level for audio\n"
       "the maximum volume level is 0, hence levels are always negetive\n"
       "Output is an audio file, in case one need to change in the media\n"
       "use the tool for extracting  media audio, and add output audio.\n"
       "Remember lossless formats are best for further editing.\n"
  },
  { 0, MakevtobwGroup,"!w32!f23Convert Video to Black&White", 346,40 ,RunHelper,0,
       "\n\n\nConverts a video to gray scale or Balck&White\n"
  },
  { 0, MakevflipGroup,"!w32!f23Vertically flip Video", 346,40,RunHelper,0,
       "\n\n\nFlips each frame vertically\n"
  },
  { 0, MakehflipGroup,"!w32!f23Horizontaly flip Video", 346,40,RunHelper,0,
       "\n\n\nFlips each frame Horizontally\n"
  },
  { 0, MakesliceGroup,"!w32!f23Make slices of Video", 346,40,RunHelper,0,
       "\nCreates slices of a video in to many slices\n"
       "of video of fixed time of slice, may be useful in\n"
       "editing further or converting to frames of images,\n"
       "which may be put back in to a video.\n"
       "Though it keeps the audio also, it may be better\n"
       "to keep the audio separate and add later, since it\n"
       "is better to keep it in a lossless format like wav or flac\n"
  },
#if 0
#endif
  { -1,NULL,NULL,0,0,NULL,0,NULL}
};
static int SetHelpMsgs(void) {
   return 1;
}
char **GetMenuList(TOOLGRP *T) {
  char **Ostrs=NULL;
  int k,j;
  k=0;
  while(T[k++].MakeGroup != NULL);
  k += 2;
  Ostrs = (char **)malloc(sizeof(char *)*(k));
  Ostrs[0] = kgCopyString("!w32!f23Select Action ");
  j=1;
  while(T[j-1].MakeGroup != NULL) {Ostrs[j] = kgCopyString(T[j-1].disc);j++;}
  Ostrs[j]=NULL;
  return Ostrs;
}
int InitConvertData(CONVDATA *cnd) {
  cnd->infile[0]='\0';
  cnd->outfile[0]='\0';
  cnd->Quality=2;
  cnd->VQuality=4;
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
  cnd->Xsize=1280;
  cnd->Ysize=720;
  cnd->fps=30;
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
  char **menu0 = NULL;
  ThumbNail **th0 ;
  DIRA r0 = { 
    'r',
    7,2,  
    340,440,   
    8,0,  
    340, 
    35, 
    1,6, 
    0,6, 
    (int *)v[0], 
    NULL, 
    NULL ,
    NULL,ToolBoxbrowser1callback, /* *args, callback */
    2,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  menu0 = GetMenuList(ToolList);
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu0);
  r0.list=(void **)th0;
  strcpy(r0.Wid,(char *)"ToolsBox");
  r0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"!c03!w32!f23Done");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-245255245;
  butn1[0].butncode='';
  DIL h1 = { 
    'h',
//    51,240,  
//    145,270,
    1,444,  
    95,472,
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
  ImageBoxGrp =  MakeImageBoxGroup(&D,NULL);
  RangeGrp= MakeRangeGroup(&D,NULL);
  EnVoGrp= MakeEnVolumeGroup(&D,NULL);
  VaspGrp = MakeVaspGroup(&D,NULL);
  VsizeGrp = MakeVsizeGroup(&D,NULL);
#ifdef D_HELPGRP
  ToolHelpGrp = MakeToolHelpGroup(&D,NULL);
#endif
  HelpButnGrp = MakeHelpButnGroup(&D,NULL);
#if 0
  AConGrp= MakeAudioConvertGroup(&D,NULL);
  VConGrp = MakeConvertVideoGroup(&D,NULL);
  VJGrp = MakeVideoJoinGroup(&D,NULL);
  AJGrp = MakeAudioJoinGroup(&D,NULL);
  AddGrp =  MakeAddAudioGroup(&D,NULL);
  MixGrp =  MakeMixAudioGroup(&D,NULL);
  AmixGrp =  MakeAmixAudioGroup(&D,NULL);
  KarGrp =  MakeMakeKaraokeGroup(&D,NULL);
  SilGrp =  MakeAddSilenceGroup(&D,NULL);
  CutGrp =  MakeMakeCutsGroup(&D,NULL);
  Is2vGrp = Makeimgs2vGroup(&D,NULL);
#endif
  AConGrp= ToolList[0].MakeGroup(&D,NULL);
  ToolList[0].GrpId = AConGrp;
  VConGrp = ToolList[1].MakeGroup(&D,NULL);
  ToolList[1].GrpId = VConGrp;
  VJGrp = ToolList[2].MakeGroup(&D,NULL);
  ToolList[2].GrpId = VJGrp;
  AJGrp = ToolList[3].MakeGroup(&D,NULL);
  ToolList[3].GrpId = AJGrp;
  AddGrp =  ToolList[4].MakeGroup(&D,NULL);
  ToolList[4].GrpId = AddGrp;
  MixGrp =  ToolList[5].MakeGroup(&D,NULL);
  ToolList[5].GrpId = MixGrp;
  AmixGrp =  ToolList[6].MakeGroup(&D,NULL);
  ToolList[6].GrpId = AmixGrp;
  KarGrp =  ToolList[7].MakeGroup(&D,NULL);
  ToolList[7].GrpId = KarGrp;
  SilGrp =  ToolList[8].MakeGroup(&D,NULL);
  ToolList[8].GrpId = SilGrp;
  CutGrp =  ToolList[9].MakeGroup(&D,NULL);
  ToolList[9].GrpId = CutGrp;
  Is2vGrp = ToolList[10].MakeGroup(&D,NULL);
  ToolList[10].GrpId = Is2vGrp;
  k=11;
  while(ToolList[k].MakeGroup != NULL) {
    ToolList[k].GrpId = ToolList[k].MakeGroup(&D,NULL);
    k++;
  }
  SetHelpMsgs();


  kgShiftGrp(&D,ImageBoxGrp,346,1);
  kgShiftGrp(&D,VaspGrp,0,-3);
  kgShiftGrp(&D,VsizeGrp,0,2);
//  kgShiftGrp(&D,VrangeGrp,10,10);
  kgShiftGrp(&D,VrangeGrp,35,9);
  kgShiftGrp(&D,EnVoGrp,0,10);
  kgShiftGrp(&D,RangeGrp,200,100);

#ifdef D_HELPGRP
  kgShiftGrp(&D,ToolHelpGrp,346,10);
#endif
//  kgShiftGrp(&D,HelpButnGrp,196,0);
  kgShiftGrp(&D,HelpButnGrp,300,204);

#if 0
  kgShiftGrp(&D,AConGrp,190,0);
  kgShiftGrp(&D,VConGrp,3,-2);
  kgShiftGrp(&D,Is2vGrp,190,0);
#else
  k=0;
  while(ToolList[k].MakeGroup != NULL) {
    if( (ToolList[k].xsh!=0)|| (ToolList[k].ysh!=0) ) {
      kgShiftGrp(&D,ToolList[k].GrpId, ToolList[k].xsh, ToolList[k].ysh);
    }
//    else kgMoveGrp(&D,ToolList[k].GrpId, 356,40);
    k++;
  }
  
#endif

  kgSetGrpVisibility(&D,ImageBoxGrp,1);
  kgSetGrpVisibility(&D,RangeGrp,0);
  kgSetGrpVisibility(&D,EnVoGrp,0);
  kgSetGrpVisibility(&D,VaspGrp,0);
  kgSetGrpVisibility(&D,VsizeGrp,0);
  kgSetGrpVisibility(&D,VrangeGrp,0);
#ifdef D_HELPGRP
  kgSetGrpVisibility(&D,ToolHelpGrp,0);
#endif
  kgSetGrpVisibility(&D,HelpButnGrp,0);
#if 0
  kgSetGrpVisibility(&D,AConGrp,0);
  kgSetGrpVisibility(&D,VConGrp,0);
  kgSetGrpVisibility(&D,VJGrp,0);
  kgSetGrpVisibility(&D,AJGrp,0);
  kgSetGrpVisibility(&D,AddGrp,0);
  kgSetGrpVisibility(&D,MixGrp,0);
  kgSetGrpVisibility(&D,AmixGrp,0);
  kgSetGrpVisibility(&D,KarGrp,0);
  kgSetGrpVisibility(&D,SilGrp,0);
  kgSetGrpVisibility(&D,CutGrp,0);
  kgSetGrpVisibility(&D,Is2vGrp,0);
#else
  SetGrpVis(&D,ToolList,-1);
#endif

  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 1;
  D.tw = 2;
  D.bw = 2;
  D.lw = 2;
  D.rw = 2;
  D.xo = 0;   /* Position of Dialog */ 
  D.yo = 0;
  D.xl = 635;    /*  Length of Dialog */
  D.yl = 280;    /*  Width  of Dialog */
  if(parent == NULL ) {
    D.xl = 840;    /*  Length of Dialog */
    D.yl = 480;    /*  Width  of Dialog */
  }
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
  strcpy(D.name,"Kulina Media Tool Box");    /*  Dialog name you may change */
  if(parent == NULL) {
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo = (xres-D.xl)/2;
     D.yo = (yres-D.yl)/2;
  }
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
//   return NULL;
   exit(0);
}
