#include <kulina.h>
#include "ToolHelpCallbacks.h"
extern int FillClr;
extern void *FillImg;
void ModifyToolHelpGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
void *RectFillImage(int w,int h,int clr) {
  DIG *wid;
  void *img = NULL;
  wid = (DIG *)kgInitImage(w,h,1);
  kgUserFrame(wid,0,0,(float)w,(float) h);
  kgBoxFill(wid,0,0,(float)w,(float) h,clr,0);
  img = kgGetResizedImage(wid);
  kgCloseImage(wid);
  return img;
}
int ToolHelpGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
//  char *xpm0= (char *)kgBorderedRectangle(444,286,FillClr,0.0);
  char *xpm0= (char *)RectFillImage(444,286,FillClr);
  DIP p0 = { 
    'p',
    1,2,  
    434,278,  
    (void *)xpm0,
    -1, /* bkgr colour */ 
      0,0,0.000000 /* border hide transparency*/ 
  };
  strcpy(p0.Wid,(char *)"ToolHelpBkgr");
  p0.item = -1;
  DIS s1 = { 
    's',
    8,6,  
    433,244,   
    6,  
    NULL,
    NULL,
    NULL ,
    NULL,NULL, /* *args, callback */
    20,6,22,1,1,1,0
  };
  strcpy(s1.Wid,(char *)"ToolHelpBox");
  s1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"Close Help");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=-216226216;
  butn2[0].butncode='';
  DIL h2 = { 
    'h',
    7,244,  
    89,277,
    2,0,  
    72, 
    25, 
    1,1, 
    2,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    ToolHelpsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h2.Wid,(char *)"ToolHelpButn");
  h2.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+4));
  d =dtmp+i; 
  d[3].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIP));
  *d[0].p = p0;
  d[0].p->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIS));
  ToolHelpbrowser1init(&s1,pt) ;
  *d[1].s = s1;
  d[1].s->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIL));
  ToolHelpsplbutton1init(&h2,pt) ;
  *d[2].h = h2;
  d[2].h->item = -1;
  d[3].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeToolHelpGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = ToolHelpGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int ToolHelp( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = ToolHelpGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 2;
  D.tw = 1;
  D.bw = 1;
  D.lw = 1;
  D.rw = 1;
  D.xo = 196;   /* Position of Dialog */ 
  D.yo = 2;
  D.xl = 435;    /*  Length of Dialog */
  D.yl = 280;    /*  Width  of Dialog */
  D.Initfun = ToolHelpinit;    /*   init fuction for Dialog */
  D.Cleanupfun = ToolHelpcleanup;    /*   init fuction for Dialog */
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
  D.Callback = ToolHelpCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = ToolHelpResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = ToolHelpWaitCallBack;  /*  Wait callback */
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
//  ModifyToolHelpGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunToolHelp(void *arg) {
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   ToolHelp(arg,v,pt );
   return NULL;
}
