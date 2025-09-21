#include <kulina.h>
#include "acapCallbacks.h"
#include "Gclracap.c"
#include "acapimages.c"
int acapGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(13);
  strcpy(e0[0].fmt,(char *)"Out File%30s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  e0[0].img=NULL;
  DIT t0 = {
    't',
    4,15,
    397,48,
    20,
    1,1,
    e0,
    1,1,
    NULL,acaptextbox1callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"FileName");
  t0.pt=NULL;
  t0.type = 0;
  t0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"");
  butn1[0].xpmn=kgRightdirImage(14,120,70,120);
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='\0';
  DIN b1 = {
    'n',
    395,15,
    429,49,
    2,2,
    24,
    24,
    1,1,
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/

    butn1,
    acapbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"Browse");
  b1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"");
  butn2[0].xpmn=(void *)(&Saveimg_str);
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=-1;
  butn2[0].butncode='\0';
   DIN b2 = {
    'n',
    192,63,
    266,137,
    2,2,
    64,
    64,
    1,1,
    1,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/

    butn2,
    acapbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b2.Wid,(char *)"Start");
  b2.item = -1;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"!c12!z32Save");
  butn3[0].xpmn=(void *)(&save_str);
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-1;
  butn3[0].butncode='\0';
  butn3[1].sw=1;
  strcpy(butn3[1].title,(char *)"");
  butn3[1].xpmn=(void *)(&Saveimg_str);
  butn3[1].xpmp=NULL;
  butn3[1].xpmh=NULL;
  butn3[1].bkgr=-1;
  butn3[1].butncode='\0';
  butn3[2].sw=1;
  strcpy(butn3[2].title,(char *)"");
  butn3[2].xpmn=kgPowerdownImage(42,230,50,50);
  butn3[2].xpmp=NULL;
  butn3[2].xpmh=NULL;
  butn3[2].bkgr=-1;
  butn3[2].butncode='\0';
  DIN b3 = {
    'n',
    125,63,
    331,137,
    2,2,
    64,
    64,
    3,1,
    1,0.500000,0,1,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/

    butn3,
    acapbutton3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b3.Wid,(char *)"Pause");
  b3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"");
  butn4[0].xpmn=(void *)(&Pauseimg_str);
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-1;
  butn4[0].butncode='\0';
  DIN b4 = {
    'n',
    192,63,
    266,137,
    2,2,
    64,
    64,
    1,1,
    1,0.500000,0,1,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/

    butn4,
    acapbutton4callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b4.Wid,(char *)"Continue");
  b4.item = -1;
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
  acapbutton1init(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIN));
  *d[2].N = b2;
  d[2].N->item = -1;
  acapbutton2init(d[2].N,pt) ;
  d[3].t = (DIT *)malloc(sizeof(DIN));
  *d[3].N = b3;
  d[3].N->item = -1;
  acapbutton3init(d[3].N,pt) ;
  d[4].t = (DIT *)malloc(sizeof(DIN));
  *d[4].N = b4;
  d[4].N->item = -1;
  acapbutton4init(d[4].N,pt) ;
  d[5].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeacapGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(char)*500);
   v0[0] = '\0';
   void** v=(void **)malloc(sizeof(void*)*2);
   v[1]=NULL;
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = acapGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int acap( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = acapGroup(&D,v,pt);
#else 
  GrpId = MakeacapGroup(&D,pt); // can try this also
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
  D.xo = 745;   /* Position of Dialog */ 
  D.yo = 210;
  D.xl = 446;    /*  Length of Dialog */
  D.yl = 153;    /*  Width  of Dialog */
  D.Initfun = acapinit;    /*   init fuction for Dialog */
  D.Cleanupfun = acapcleanup;    /*   init fuction for Dialog */
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
  D.Callback = acapCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = acapResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = acapWaitCallBack;  /*  Wait callback */
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
  Modifyacap(&D,GrpId);    /*  add extras to  gui*/
  ModifyacapGc(&D);    /*  set colors for gui if don't like default*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *Runacap(void *arg) {
/*************************************************

    Text_Box1  1 data values

*************************************************/
   char  v0[500]="" ;
   void* v[1];
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
   acap(NULL,v,pt );
   return NULL;
}
