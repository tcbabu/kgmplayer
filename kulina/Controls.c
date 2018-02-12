#include "kulina.h"
#include "ControlsCallbacks.h"
#include "cimages.c"
extern int AudioDelay;
extern int AllTracks;
extern int Volume;
extern float CurPerc;
extern int Speed,Mute,Play;
extern int FilePlay;
extern int FULLSCREEN;
extern int WMErr;
extern int WMEntry;
extern unsigned long MWIN;
extern int Cxres,Cyres,Dxres,Dyres,Pxres,Pyres,Hxres,Hyres;
extern float StartSec,TotSec,CurSec;
extern int rsig[2];
int runjob(char *job,int (*ProcessOut)(int,int));
int MakeaudiodelayGroup(DIALOG *D,void *arg);
void ModifyControlsGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}

int ControlsGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  BUT_STR  *butn0=NULL; 
  butn0= (BUT_STR *)malloc(sizeof(BUT_STR)*5);
  butn0[0].sw=1;
  strcpy(butn0[0].title,(char *)"");
  butn0[0].xpmn=NULL;
  butn0[0].xpmp=NULL;
  butn0[0].xpmh=NULL;
  butn0[0].xpmn=(char *)(&cancel_str);
  butn0[0].bkgr=-108108103;
  butn0[0].butncode='X';
  butn0[1].sw=1;
  strcpy(butn0[1].title,(char *)"");
  butn0[1].xpmn=NULL;
  butn0[1].xpmp=NULL;
  butn0[1].xpmh=NULL;
  if(Play)butn0[1].xpmn=(char *)(&pause_str);
  else butn0[1].xpmn=(char *)(&play_str);
  butn0[1].bkgr=-108108103;
  butn0[1].butncode='P';
  butn0[2].sw=1;
  strcpy(butn0[2].title,(char *)"");
  butn0[2].xpmn=NULL;
  butn0[2].xpmp=NULL;
  butn0[2].xpmh=NULL;
  butn0[2].xpmn=(char *)(&forward_str);
  butn0[2].bkgr=-108108103;
  butn0[2].butncode='';
  butn0[3].sw=1;
  strcpy(butn0[3].title,(char *)"");
  butn0[3].xpmn=NULL;
  butn0[3].xpmp=NULL;
  butn0[3].xpmh=NULL;
  if(!Mute)butn0[3].xpmn=(char *)(&muted_str);
  else butn0[3].xpmn=(char *)(&volume_str);
  butn0[3].bkgr=-108108103;
  butn0[3].butncode='';
  butn0[4].sw=1;
  if(FilePlay==1) butn0[4].sw=-1;
  if((FilePlay==0)&&(AllTracks==0)) butn0[4].sw=-1;
  strcpy(butn0[4].title,(char *)"");
  butn0[4].xpmn=NULL;
  butn0[4].xpmp=NULL;
  butn0[4].xpmh=NULL;
  butn0[4].xpmn=(char *)(&next_str); 
  butn0[4].bkgr=-108108103;
  butn0[4].butncode='';
  DIL h0 = { 
    'h',
    5,10,  
    293,72,
    2,0,  
    54, 
    54, 
    5,1, 
    2,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn0, 
    Controlssplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h0.Wid,(char *)"ControlsWidget1");
  DIHB d1 = { 
    'P',
    392,47,  
    606,69,   
    0,100,  
    184,  
    (int *)v[0],
    NULL,
    NULL,Controlsdslide1callback,
    0,0,7,-172235235, /* bordr,hide,type,color */
  };
  strcpy(d1.Wid,(char *)"ControlsWidget2");
  DIHB d2 = { 
    'P',
    391,16,  
    605,38,   
    0,100,  
    184,  
    (int *)v[1],
    NULL,
    NULL,Controlsdslide2callback,
    0,0,7,-172235235, /* bordr,hide,type,color */
  };
  strcpy(d2.Wid,(char *)"ControlsWidget3");
  DIM m3 = { 
    'm',
    292,45,  
    392,69,  
    1,0  
  };
  strncpy(m3.msg,(char *)"!c06Volume",499);
  strcpy(m3.Wid,(char *)"ControlsWidget4");
  DIM m4 = { 
    'm',
    291,14,  
    391,38,  
    1,0  
  };
  strncpy(m4.msg,(char *)"!c06Playing",499);
  strcpy(m4.Wid,(char *)"ControlsWidget5");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+6));
  d =dtmp+i; 
  d[5].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIL));
  Controlssplbutton1init(&h0,pt) ;
  *d[0].h = h0;
  d[0].h->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIHB));
  *d[1].B = d1;
  d[1].B->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIHB));
  *d[2].B = d2;
  d[2].B->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIM));
  *d[3].m = m3;
  d[3].m->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIM));
  *d[4].m = m4;
  d[4].m->item = -1;
  d[5].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 
/* One can also use the following code to add Widgets to an existing Dialog */

int MakeControlsGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Integerslidebar1  1 data value
    Integerslidebar2  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   void** v=(void **)malloc(sizeof(void*)*3);
   v[2]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = ControlsGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int Controls( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k,Agrp;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = ControlsGroup(&D,v,pt);
#if 0 
//Not working
  Agrp=  MakeaudiodelayGroup(&D,pt);
  kgShiftGrp(&D,Agrp,70,2);
  kgShiftWidget(&D,1,0,-17);
  kgShiftWidget(&D,3,0,-17);
  kgShiftWidget(&D,2,0,-10);
  kgShiftWidget(&D,4,0,-10);
#endif
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 2;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 437;   /* Position of Dialog */ 
  D.yo = 299;
  D.xl = 620;    /*  Length of Dialog */
  D.yl = 80;    /*  Width  of Dialog */
  D.Initfun = Controlsinit;    /*   init fuction for Dialog */
  D.Cleanupfun = Controlscleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 0;    /*  1 for Window Decorration */
  D.transparency = 0.750000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 1;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = ControlsCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = ControlsResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = ControlsWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 1;    /*  1 for not showing in task bar*/
  D.StackPos = 1;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
  if(WMErr) {
    if(MWIN != 0) D.PWIN=&MWIN;
  }
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
//     printf("Dxres: %d Dyres: %d\n",Dxres,Dyres);
     D.xo = (Dxres -D.xl)/2;
     D.yo = (Dyres -D.yl)/2;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyControlsGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void * RunControls(void *arg) {
/*************************************************

    Integerslidebar1  1 data value
    Integerslidebar2  1 data value

*************************************************/
   int ret,qsecs;
   int   v0 = 0;
   int   v1 = 0;
   v0 = Volume;
   v1 = (int)CurPerc;
   void* v[2];
   qsecs=5;
   if(qsecs> TotSec*0.005) qsecs= TotSec*0.005;
   if(qsecs>30) qsecs=30;
   if((TotSec -CurSec) < qsecs) {
     if(!WMEntry) write(rsig[1],"U0000\n",6);
     return NULL;
   }
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   void *pt=NULL; /* pointer to send any extra information */
   ret = Controls(arg,v,pt );
//   printf("ret= %d\n",ret);
#if 0
   if(ret == 1) return 1;
   else return 0;
#else
   return NULL;
#endif
}
