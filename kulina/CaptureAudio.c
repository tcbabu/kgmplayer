#include <kulina.h>
#include "CaptureAudioCallbacks.h"
#include "GclrCaptureAudio.c"
int CaptureAudioGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIM m0 = { 
    'm',
    65,55,  
    345,89,  
    0,0  
  };
  strncpy(m0.msg,(char *)"To capture playing audio",499);
  strcpy(m0.Wid,(char *)"CaptureAudioWidget3");
  m0.item = -1;
  DIM m1 = { 
    'm',
    56,91,  
    360,122,  
    0,0  
  };
  strncpy(m1.msg,(char *)"Output should be saved as Wav file",499);
  strcpy(m1.Wid,(char *)"CaptureAudioWidget4");
  m1.item = -1;
  DIM m2 = { 
    'm',
    24,122,  
    375,152,  
    0,0  
  };
  strncpy(m2.msg,(char *)"Capture audio is saved only if Save button is pressed",499);
  strcpy(m2.Wid,(char *)"CaptureAudioWidget5");
  m2.item = -1;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"!z32!c01Start Capturing");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-1;
  butn3[0].butncode=31;
  DIL h3 = { 
    'h',
    102,230,  
    311,266,
    2,0,  
    200, 
    32, 
    1,1, 
    7,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    CaptureAudiosplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h3.Wid,(char *)"CaptureAudioWidget6");
  h3.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+5));
  d =dtmp+i; 
  d[4].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIM));
  *d[0].m = m0;
  d[0].m->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIM));
  *d[1].m = m1;
  d[1].m->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIM));
  *d[2].m = m2;
  d[2].m->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIL));
  *d[3].h = h3;
  d[3].h->item = -1;
  CaptureAudiosplbutton1init(d[3].h,pt) ;
  d[4].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeCaptureAudioGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   GrpId = CaptureAudioGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int CaptureAudio( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = CaptureAudioGroup(&D,v,pt);
#else 
  GrpId = MakeCaptureAudioGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 3;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 928;   /* Position of Dialog */ 
  D.yo = 230;
  D.xl = 398;    /*  Length of Dialog */
  D.yl = 330;    /*  Width  of Dialog */
  D.Initfun = CaptureAudioinit;    /*   init fuction for Dialog */
  D.Cleanupfun = CaptureAudiocleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.NoTabProcess = 0;    /*  1 for disabling Tab use */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = CaptureAudioCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = CaptureAudioResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = CaptureAudioWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.NoWinMngr = 0;    /*  1 for no Window Manager*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 2.0");    /*  Dialog name you may change */
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
  ModifyCaptureAudio(&D,GrpId);    /*  add extras to  gui*/
  ModifyCaptureAudioGc(&D);    /*  set colors for gui if do not like default*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunCaptureAudio(void *parent ,void *args) {
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt[2]={NULL,NULL}; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   pt[0]=args;
   CaptureAudio(parent,v,(void *)pt );
   return pt[1];
}
