#include <kulina.h>
#include "MonitorJoinCallbacks.h"
extern char VER[];
extern int Tools;
extern int Thred,Thgreen,Thblue;
extern char GrabFileName[300];
extern int MonPipe;

void ModifyMonitorJoinGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int MonitorJoinGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIM m0 = { 
    'm',
    23,7,  
    591,35,  
    0,0  
  };
  strncpy(m0.msg,GrabFileName,499);
  strcpy(m0.Wid,(char *)"VJTitle");
  m0.item = -1;
  DII i1 = { 
    'i',
    24,42,  
    588,117,  
    61,3,0   
  };
  strcpy(i1.Wid,(char *)"VJInfo");
  i1.item = -1;
  DIO o2 = { 
    'o',
    45,130,  
    565,148,  
    0,0,0,3,-1,0  
       //hide,percent,border,type,color,direction 
  };
  strcpy(o2.Wid,(char *)"JoinBar");
  o2.item = -1;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"Cancel");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-1;
  butn3[0].butncode='';
  DIL h3 = { 
    'h',
    240,148,  
    322,181,
    2,0,  
    72, 
    25, 
    1,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    MonitorJoinsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h3.Wid,(char *)"MonitorJoinWidget4");
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
  d[1].t = (DIT *)malloc(sizeof(DII));
  *d[1].i = i1;
  d[1].i->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIO));
  *d[2].o = o2;
  d[2].o->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIL));
  MonitorJoinsplbutton1init(&h3,pt) ;
  *d[3].h = h3;
  d[3].h->item = -1;
  d[4].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeMonitorJoinGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = MonitorJoinGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int MonitorJoin( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = MonitorJoinGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 3;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 610;    /*  Length of Dialog */
  D.yl = 187;    /*  Width  of Dialog */
  D.Initfun = MonitorJoininit;    /*   init fuction for Dialog */
  D.Cleanupfun = MonitorJoincleanup;    /*   init fuction for Dialog */
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
  D.Callback = MonitorJoinCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = MonitorJoinResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = MonitorJoinWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
  strcpy(D.name,"kgMplayer ");    /*  Dialog name you may change */
  strcat(D.name,VER);
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
//  ModifyMonitorJoinGc(&(D.gc));    /*  set colors for gui*/
  kgColorTheme(&D,Thred,Thgreen,Thblue);
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunMonitorJoin(void *arg) {
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   MonitorJoin(NULL,v,pt );
   return NULL;
}
