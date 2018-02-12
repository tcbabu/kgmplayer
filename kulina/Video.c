#include <kulina.h>
#include "VideoCallbacks.h"
extern int NoVideo;
void ModifyVideoGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int VideoGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  char *menu0[]  = { 
    (char *)"Yes",
    (char *)"No",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r0 = { 
    'r',
    442,142,  
    583,170,   
    8,0,  
    60, 
    25, 
    1,2, 
    1,1, 
    (int *)v[0], 
    NULL, 
    NULL ,
    NULL,Videobrowser1callback, /* *args, callback */
    1,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu0);
  r0.list=(void **)th0;
  strcpy(r0.Wid,(char *)"VideoWidget1");
  r0.item = -1;
  DIM m1 = { 
    'm',
    382,144,  
    442,165,  
    1,0  
  };
  strncpy(m1.msg,(char *)"!f12VIDEO",499);
  strcpy(m1.Wid,(char *)"VideoWidget2");
  m1.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+3));
  d =dtmp+i; 
  d[2].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIRA));
  Videobrowser1init(&r0,pt) ;
  *d[0].r = r0;
  d[0].r->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIM));
  *d[1].m = m1;
  d[1].m->item = -1;
  d[2].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeVideoGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    RadioButtons1  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   if(NoVideo==1) *v0=2;
   else *v0=1;
   void** v=(void **)malloc(sizeof(void*)*2);
   v[1]=NULL;
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = VideoGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int Video( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = VideoGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 0;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 401;   /* Position of Dialog */ 
  D.yo = 89;
  D.xl = 558;    /*  Length of Dialog */
  D.yl = 288;    /*  Width  of Dialog */
  D.Initfun = Videoinit;    /*   init fuction for Dialog */
  D.Cleanupfun = Videocleanup;    /*   init fuction for Dialog */
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
  D.Callback = VideoCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = VideoResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = VideoWaitCallBack;  /*  Wait callback */
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
//  ModifyVideoGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunVideo(void *arg) {
/*************************************************

    RadioButtons1  1 data value

*************************************************/
   int   v0 = 1;
   void* v[1];
   v[0]=(void *)(&v0);
   void *pt=NULL; /* pointer to send any extra information */
   if(NoVideo==1) v0=2;
   else v0=1;
   Video(NULL,v,pt );
   return NULL;
}
