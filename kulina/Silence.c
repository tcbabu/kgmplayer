#include <kulina.h>
#include "SilenceCallbacks.h"
#include "GclrSilence.c"
static double  meanlevel=-16;
int SilenceGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIX x0 = { 
    'x',
    5,36,  
    432,192,   
    10,2,  
    390, 
    25, 
    1,9812683, 
    -1077952577,5, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,Silencebrowser1callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(x0.Wid,(char *)"SXbox");
  x0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Delete");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode=127;
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"Delete All");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode=127;
  butn1[2].sw=1;
  strcpy(butn1[2].title,(char *)"Add");
  butn1[2].xpmn=NULL;
  butn1[2].xpmp=NULL;
  butn1[2].xpmh=NULL;
  butn1[2].bkgr=-1;
  butn1[2].butncode=127;
  DIN b1 = { 
    'n',
    171,2,  
    425,36,
    2,2,  
    80, 
    24, 
    3,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    Silencebutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"SBrwosebutns");
  b1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"!f23Silence");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=-235255250;
  butn2[0].butncode=127;
  DIL h2 = { 
    'h',
    316,232,  
    418,265,
    2,0,  
    92, 
    25, 
    1,1, 
    6,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    Silencesplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h2.Wid,(char *)"Silence");
  h2.item = -1;
#if 0
  T_ELMT *e3  ; 
  e3 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e3[0].fmt = (char *)malloc(5);
  strcpy(e3[0].fmt,(char *)"%3d");
  e3[0].v=(void *)v[1];
  e3[0].sw=1;
  e3[0].noecho=0;
  e3[0].img=NULL;
  DIT t3 = { 
    't',
    179,195,  
    267,229,
    20, 
    1,1, 
    e3,
    1,1,
    NULL,Silencetextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t3.Wid,(char *)"LMeanBox");
  t3.pt=NULL;
  t3.type = 1;
  t3.item = -1;
  t3.hide = 1;
  DIM m4 = { 
    'm',
    101,200,  
    187,227,  
    1,0  
  };
  strncpy(m4.msg,(char *)"Mean(-dB)",499);
  strcpy(m4.Wid,(char *)"SilenceWidget6");
  m4.item = -1;
#endif
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+6));
  d =dtmp+i; 
//  d[3].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIX));
  *d[0].x = x0;
  d[0].x->item = -1;
  Silencebrowser1init(d[0].x,pt) ;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  Silencebutton1init(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIL));
  *d[2].h = h2;
  d[2].h->item = -1;
  Silencesplbutton1init(d[2].h,pt) ;
#if 0
  d[3].t = (DIT *)malloc(sizeof(DIT));
  *d[3].t = t3;
  d[3].t->hide = 1;
  d[3].t->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIM));
  *d[4].m = m4;
  d[4].m->item = -1;
#endif
  d[3].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeSilenceGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  1 data values

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = meanlevel;
   void** v=(void **)malloc(sizeof(void*)*3);
   v[2]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = SilenceGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int Silence( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = SilenceGroup(&D,v,pt);
#else 
  GrpId = MakeSilenceGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 3;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 199;   /* Position of Dialog */ 
  D.yo = -1;
  D.xl = 438;    /*  Length of Dialog */
  D.yl = 274;    /*  Width  of Dialog */
  D.Initfun = Silenceinit;    /*   init fuction for Dialog */
  D.Cleanupfun = Silencecleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
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
  D.Callback = SilenceCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = SilenceResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = SilenceWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.NoWinMngr = 0;    /*  1 for no Window Manager*/
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
  ModifySilence(&D,GrpId);    /*  add extras to  gui*/
  ModifySilenceGc(&D);    /*  set colors for gui if don't like default*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunSilence(void *arg) {
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  1 data values

*************************************************/
   int   v0 = 1;
   int   v1 = -16;
   void* v[2];
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   void *pt=NULL; /* pointer to send any extra information */
   Silence(NULL,v,pt );
   return NULL;
}
