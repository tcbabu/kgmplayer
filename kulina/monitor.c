#include <kulina.h>
#include "monitorCallbacks.h"
extern char VER[];
extern int Tools;
extern int Thred,Thgreen,Thblue;
extern char GrabFileName[300];
extern int MonPipe;
void ModifymonitorGc(Gclr *gc) {
extern int ToGrab[2],FromGrab[2],StatusGrab[2];
int GetTimedLine(int pip0,char *buff,int usec);
int GetLine(int pip0,char *buff);
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int monitorGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  int Bclr;
  DIA *d=NULL,*dtmp;
  Bclr = (Thred+40)*1000000+(Thgreen+40)*1000+Thblue+40;
  Bclr = -Bclr;
  DIM m0 = { 
    'm',
    6,10,  
    386,33,  
    0,0  
  };
  strncpy(m0.msg,(char *)"!z43",499);
  strcat(m0.msg,GrabFileName);
  strcpy(m0.Wid,(char *)"monitorWidget1");
  m0.item = -1;
  DIM m1 = { 
    'm',
    7,33,  
    387,57,  
    0,0  
  };
  switch(Tools) {
    case 0:
      strncpy(m1.msg,(char *)"!c02!z65May play file after some download",499);
      break;
    case 1:
      strncpy(m1.msg,(char *)"!c02!z65May play file after some conversion",499);
      break;
    case 2:
      strncpy(m1.msg,(char *)"!c03!z65Play file only after finishing",499);
      break;
    break;
      strncpy(m1.msg,(char *)"!c02!z65May play file after some download/creation",499);
      break;
  }
  strcpy(m1.Wid,(char *)"monitorWidget3");
  m1.item = -1;
  DIO o2 = { 
    'o',
    19,69,  
    372,87,  
    0,0,0,0,-1,0  
  };
  strcpy(o2.Wid,(char *)"Bar");
  o2.item = -1;
  o2.type=0;
  o2.bordr = Bclr;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"!c03Cancel");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-246240160;
  butn3[0].butncode='';
  DIL h3 = { 
    'h',
    150,94,  
    232,127,
    2,0,  
    72, 
    25, 
    1,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    monitorsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h3.Wid,(char *)"monitorWidget6");
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
  d[2].t = (DIT *)malloc(sizeof(DIO));
  *d[2].o = o2;
  d[2].o->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIL));
  monitorsplbutton1init(&h3,pt) ;
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

int MakemonitorGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = monitorGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int monitor( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = monitorGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 3;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 435;   /* Position of Dialog */ 
  D.yo = 177;
  D.xl = 398;    /*  Length of Dialog */
  D.yl = 131;    /*  Width  of Dialog */
  D.Initfun = monitorinit;    /*   init fuction for Dialog */
  D.Cleanupfun = monitorcleanup;    /*   init fuction for Dialog */
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
  D.Callback = monitorCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = monitorResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = monitorWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
//  strcpy(D.name,"kgmplayer Grab/Tools");    /*  Dialog name you may change */
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
  kgColorTheme(&D,Thred,Thgreen,Thblue);
//  ModifymonitorGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *Runmonitor(void *arg) {
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   monitor(NULL,v,pt );
   return NULL;
}
