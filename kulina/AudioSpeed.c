#include <kulina.h>
#include "AudioSpeedCallbacks.h"
#include "GclrAudioSpeed.c"
extern double SpeedFac;
int AudioSpeedGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(15);
  strcpy(e0[0].fmt,(char *)"Input File%25s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  e0[0].img=NULL;
  DIT t0 = { 
    't',
    9,17,  
    377,51,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,AudioSpeedtextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"ATInput");
  t0.pt=NULL;
  t0.type = 1;
  t0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Browse");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode=31;
  DIN b1 = { 
    'n',
    377,16,  
    459,50,
    2,2,  
    72, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    AudioSpeedbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"ATbrowse");
  b1.item = -1;
  T_ELMT *e2  ; 
  e2 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e2[0].fmt = (char *)malloc(13);
  strcpy(e2[0].fmt,(char *)"Out File%25s");
  e2[0].v=(void *)v[1];
  e2[0].sw=1;
  e2[0].noecho=0;
  e2[0].img=NULL;
  DIT t2 = { 
    't',
    28,58,  
    378,92,
    20, 
    1,1, 
    e2,
    1,1,
    NULL,AudioSpeedtextbox2callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t2.Wid,(char *)"ATOutput");
  t2.pt=NULL;
  t2.type = 1;
  t2.item = -1;
  T_ELMT *e3  ; 
  e3 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e3[0].fmt = (char *)malloc(16);
  strcpy(e3[0].fmt,(char *)"Speed Factor%5F");
  e3[0].v=(void *)v[2];
  e3[0].sw=1;
  e3[0].noecho=0;
  e3[0].img=NULL;
  DIT t3 = { 
    't',
    133,133,  
    339,167,
    20, 
    1,1, 
    e3,
    1,1,
    NULL,AudioSpeedtextbox3callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t3.Wid,(char *)"ATSpeed");
  t3.pt=NULL;
  t3.type = 0;
  t3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"!c01Change Speed");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-1;
  butn4[0].butncode=31;
  DIL h4 = { 
    'h',
    183,205,  
    286,234,
    2,0,  
    96, 
    25, 
    1,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    AudioSpeedsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h4.Wid,(char *)"ATdo");
  h4.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+6));
  d =dtmp+i; 
  d[5].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  AudioSpeedbutton1init(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIT));
  *d[2].t = t2;
  d[2].t->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIT));
  *d[3].t = t3;
  d[3].t->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIL));
  *d[4].h = h4;
  d[4].h->item = -1;
  AudioSpeedsplbutton1init(d[4].h,pt) ;
  d[5].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeAudioSpeedGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    Text_Box3  1 data values

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(char)*500);
   v0[0] = '\0';
   char  *v1 ;
   v1 = (char *)malloc(sizeof(char)*500);
   v1[0] = '\0';
   double *v2 ;
   v2 = (double *)malloc(sizeof(double));
   *v2 = SpeedFac;
   void** v=(void **)malloc(sizeof(void*)*4);
   v[3]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   void *pt=NULL; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   GrpId = AudioSpeedGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int AudioSpeed( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = AudioSpeedGroup(&D,v,pt);
#else 
  GrpId = MakeAudioSpeedGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 4;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 828;   /* Position of Dialog */ 
  D.yo = 148;
  D.xl = 463;    /*  Length of Dialog */
  D.yl = 363;    /*  Width  of Dialog */
  D.Initfun = AudioSpeedinit;    /*   init fuction for Dialog */
  D.Cleanupfun = AudioSpeedcleanup;    /*   init fuction for Dialog */
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
  D.Callback = AudioSpeedCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = AudioSpeedResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = AudioSpeedWaitCallBack;  /*  Wait callback */
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
  ModifyAudioSpeed(&D,GrpId);    /*  add extras to  gui*/
  ModifyAudioSpeedGc(&D);    /*  set colors for gui if do not like default*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunAudioSpeed(void *parent ,void *args) {
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    Text_Box3  1 data values

*************************************************/
   char  v0[500]="" ;
   char  v1[500]="" ;
   double v2 = 0.0;
   void* v[3];
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(&v2);
   void *pt[2]={NULL,NULL}; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   pt[0]=args;
   AudioSpeed(parent,v,(void *)pt );
   return pt[1];
}
