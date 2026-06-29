#include <kulina.h>
#include "ArrangeVideoCallbacks.h"
#include "GclrArrangeVideo.c"
int ArrangeVideoGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIX x0 = { 
    'x',
    19,38,  
    496,174,   
    8,2,  
    300, 
    25, 
    1,0, 
    1,4, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,ArrangeVideoAVMlistcallback , /* *args, Callback  */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /*=1 hide  */
   };
  strcpy(x0.Wid,(char *)"AVMlist");
  x0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Add");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode=31;
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"Delete");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode=31;
  DIN b1 = { 
    'n',
    318,6,  
    496,37,
    2,2,  
    84, 
    24, 
    2,1, 
    4,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn1, 
    ArrangeVideoAVMbutnscallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"AVMbutns");
  b1.item = -1;
  T_ELMT *e2  ; 
  e2 =(T_ELMT *)malloc(sizeof(T_ELMT)*2);
  e2[0].fmt = (char *)malloc(8);
  strcpy(e2[0].fmt,(char *)"Xres%6d");
  e2[0].v=(void *)v[1];
  e2[0].sw=1;
  e2[0].noecho=0;
  e2[0].img=NULL;
  e2[1].fmt = (char *)malloc(10);
  strcpy(e2[1].fmt,(char *)"  Yres%6d");
  e2[1].v=(void *)v[2];
  e2[1].sw=1;
  e2[1].noecho=0;
  e2[1].img=NULL;
  DIT t2 = { 
    't',
    111,229,  
    413,261,
    20, 
    2,1, 
    e2,
    1,1,
    NULL,ArrangeVideoAVMrescallback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t2.Wid,(char *)"AVMres");
  t2.pt=NULL;
  t2.type = 0;
  t2.item = -1;
  DII i3 = { 
    'i',
    5,273,  
    500,355,  
    53,3,0   
  };
  strcpy(i3.Wid,(char *)"AVMinfo");
  i3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"Process");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-1;
  butn4[0].butncode=31;
  DIL h4 = { 
    'h',
    205,364,  
    335,397,
    2,0,  
    120, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn4, 
    ArrangeVideoAVMgocallback ,  /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(h4.Wid,(char *)"AVMgo");
  h4.item = -1;
  T_ELMT *e5  ; 
  e5 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e5[0].fmt = (char *)malloc(11);
  strcpy(e5[0].fmt,(char *)"Output%35s");
  e5[0].v=(void *)v[3];
  e5[0].sw=1;
  e5[0].noecho=0;
  e5[0].img=NULL;
  DIT t5 = { 
    't',
    7,186,  
    424,220,
    20, 
    1,1, 
    e5,
    1,1,
    NULL,ArrangeVideoAVMoutcallback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t5.Wid,(char *)"AVMout");
  t5.pt=NULL;
  t5.type = 0;
  t5.item = -1;
  BUT_STR  *butn6=NULL; 
  butn6= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn6[0].sw=1;
  strcpy(butn6[0].title,(char *)"Browse");
  butn6[0].xpmn=NULL;
  butn6[0].xpmp=NULL;
  butn6[0].xpmh=NULL;
  butn6[0].bkgr=-1;
  butn6[0].butncode=31;
  DIN b6 = { 
    'n',
    425,186,  
    503,220,
    2,2,  
    72, 
    24, 
    1,1, 
    5,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn6, 
    ArrangeVideoAVMbrowsecallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b6.Wid,(char *)"AVMbrowse");
  b6.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+8));
  d =dtmp+i; 
  d[7].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIX));
  *d[0].x = x0;
  d[0].x->item = -1;
  ArrangeVideoAVMlistinit(d[0].x,pt) ;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  ArrangeVideoAVMbutnsinit(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIT));
  *d[2].t = t2;
  d[2].t->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DII));
  *d[3].i = i3;
  d[3].i->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIL));
  *d[4].h = h4;
  d[4].h->item = -1;
  ArrangeVideoAVMgoinit(d[4].h,pt) ;
  d[5].t = (DIT *)malloc(sizeof(DIT));
  *d[5].t = t5;
  d[5].t->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIN));
  *d[6].N = b6;
  d[6].N->item = -1;
  ArrangeVideoAVMbrowseinit(d[6].N,pt) ;
  d[7].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeArrangeVideoGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  2 data values
    Text_Box2  1 data values

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   int  *v2 ;
   v2 = (int *)malloc(sizeof(int));
   *v2 = 1;
   char  *v3 ;
   v3 = (char *)malloc(sizeof(char)*500);
   v3[0] = '\0';
   void** v=(void **)malloc(sizeof(void*)*5);
   v[4]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   void *pt=NULL; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   GrpId = ArrangeVideoGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   ArrangeVideoSetup(D,Gpt->arg);
   return GrpId;
}

int ArrangeVideo( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = ArrangeVideoGroup(&D,v,pt);
#else 
  GrpId = MakeArrangeVideoGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 6;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 886;   /* Position of Dialog */ 
  D.yo = 174;
  D.xl = 509;    /*  Length of Dialog */
  D.yl = 411;    /*  Width  of Dialog */
  D.Initfun = ArrangeVideoinit;    /*   init fuction for Dialog */
  D.Cleanupfun = ArrangeVideocleanup;    /*   cleanup fuction for Dialog */
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
  D.Callback = ArrangeVideoCallBack;    /*  default Callback  */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = ArrangeVideoResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = ArrangeVideoWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.NoWinMngr = 0;    /*  1 for no Window Manager*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 3.0");    /*  Dialog name you may change */
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
  ModifyArrangeVideoGc(&D);    /*  set colors for gui if do not like default*/
  ModifyArrangeVideo(&D,GrpId);    /*  add extras to  gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunArrangeVideo(void *parent ,void *args) {
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  2 data values
    Text_Box2  1 data values

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   int   v2 = 1;
   char  v3[500]="" ;
   void* v[4];
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(v3);
   void *pt[2]={NULL,NULL}; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   pt[0]=args;
   ArrangeVideo(parent,v,(void *)pt );
   return pt[1];
}
