#include <X11/X.h>
#include <X11/Xlib.h>
#include <kulina.h>
#include "MwinCallbacks.h"
void  *kgCreateWindow (void *Tmp);
void kgSubWindow(void *Gtmp);
void uiDialogWindow(DIALOG *D);
int kgGetUiWindow(void *);
Display * kgGetUiDisplay(void *);
DIALOG *Dtmp=NULL;

void ModifyMwinGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int MwinGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  char *xpm0=   NULL; /* pixmap info */ 
  DIG g0 = { 
    'g',
    0,114,  
    315,596,  
    (void *)xpm0,
    0, /* bkgr colour */ 
    Mwingbox1init,/* void *initgraph(int,void *)  */ 
    NULL,0,0 /* *data border hide*/
  };
  strcpy(g0.Wid,(char *)"MwinWidget1");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+2));
  d =dtmp+i; 
  d[1].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIG));
  *d[0].g = g0;
  d[0].g->item = -1;
  d[1].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeMwinGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = MwinGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

void * Mwin( void *parent,void **v,void *pt) {
  long long ret=1;
  int GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  DIG *g0;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = MwinGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 0;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 0;   /* Position of Dialog */ 
  D.yo = 0;
  D.xl = 1321;    /*  Length of Dialog */
  D.yl = 657;    /*  Width  of Dialog */
  D.Initfun = Mwininit;    /*   init fuction for Dialog */
  D.Cleanupfun = Mwincleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 1;    /*  1 for for fullscreen mode */
  D.Deco = 0;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
  g0 = (DIG *) d[0].g;
#if 1 
  D.Callback = MwinCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = MwinResizeCallBack;  /*  Resize callback */
#if 1 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = MwinWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 0;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 1;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
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
//     g0->x1=g0->y1=0;
 //    g0->x2=xres;
  //   g0->y2=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyMwinGc(&(D.gc));    /*  set colors for gui*/
//  ret= kgUi(&D);
//  kgCleanUi(&D);
    fprintf(stderr,"Calling uiDialogWindow\n");
    Dtmp =(DIALOG *)malloc(sizeof(DIALOG));
    *Dtmp = D;
    uiDialogWindow(Dtmp);
    g0->D=Dtmp;
    fprintf(stderr,"Calling kgSubWindow\n");
    kgSubWindow(g0);
    fprintf(stderr,"Called\n");
//    ret= (long long)kgGetUiWindow(&D);
  return (void *) g0;
}


Window kgGetKulinaWindow(void *g0) {
  return (*((Window *)((DIG *)g0)->glWindow));
}
Display *kgGetKulinaDisplay(void *g0 ) {
  DIALOG *D;
  D = (DIALOG *)((DIG *)g0)->D;
  return kgGetUiDisplay(D);
}
void *RunMwin(void *arg) {
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   Mwin(NULL,v,pt );
   return NULL;
}
