#include <kulina.h>
#include "ArrangeVideoCallbacks.h"
#include "GclrArrangeVideo.c"
int ArrangeVideoGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  BUT_STR  *butn0=NULL; 
  butn0= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn0[0].sw=1;
  strcpy(butn0[0].title,(char *)"Add");
  butn0[0].xpmn=NULL;
  butn0[0].xpmp=NULL;
  butn0[0].xpmh=NULL;
  butn0[0].bkgr=-221221221;
  butn0[0].butncode=15198078;
  butn0[1].sw=1;
  strcpy(butn0[1].title,(char *)"Delete");
  butn0[1].xpmn=NULL;
  butn0[1].xpmp=NULL;
  butn0[1].xpmh=NULL;
  butn0[1].bkgr=-1;
  butn0[1].butncode=-2302850;
  DIN b0 = { 
    'n',
    297,10,  
    475,41,
    2,2,  
    84, 
    24, 
    2,1, 
    4,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn0, 
    ArrangeVideoAVMbutnscallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b0.Wid,(char *)"AVMbutns");
  b0.item = -1;
  T_ELMT *e1  ; 
  e1 =(T_ELMT *)malloc(sizeof(T_ELMT)*2);
  e1[0].fmt = (char *)malloc(8);
  strcpy(e1[0].fmt,(char *)"Xres%5d");
  e1[0].v=(void *)v[0];
  e1[0].sw=1;
  e1[0].noecho=0;
  e1[0].img=NULL;
  e1[1].fmt = (char *)malloc(10);
  strcpy(e1[1].fmt,(char *)"  Yres%5d");
  e1[1].v=(void *)v[1];
  e1[1].sw=1;
  e1[1].noecho=0;
  e1[1].img=NULL;
  DIT t1 = { 
    't',
    181,246,  
    345,311,
    20, 
    1,2, 
    e1,
    1,1,
    NULL,ArrangeVideoAVMrescallback ,1 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t1.Wid,(char *)"AVMres");
  t1.pt=NULL;
  t1.type = 0;
  t1.item = -1;
  DII i2 = { 
    'i',
    8,320,  
    488,400,  
    52,3,0   
  };
  strcpy(i2.Wid,(char *)"AVMinfo");
  i2.item = -1;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"Process");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-1;
  butn3[0].butncode=31;
  DIL h3 = { 
    'h',
    207,405,  
    335,433,
    2,0,  
    120, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn3, 
    ArrangeVideoAVMgocallback ,  /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(h3.Wid,(char *)"AVMgo");
  h3.item = -1;
  T_ELMT *e4  ; 
  e4 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e4[0].fmt = (char *)malloc(11);
  strcpy(e4[0].fmt,(char *)"Output%32s");
  e4[0].v=(void *)v[2];
  e4[0].sw=1;
  e4[0].noecho=0;
  e4[0].img=NULL;
  DIT t4 = { 
    't',
    14,178,  
    409,212,
    20, 
    1,1, 
    e4,
    1,1,
    NULL,ArrangeVideoAVMoutcallback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t4.Wid,(char *)"AVMout");
  t4.pt=NULL;
  t4.type = 0;
  t4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(char *)"Browse");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-1;
  butn5[0].butncode=31;
  DIN b5 = { 
    'n',
    409,178,  
    487,212,
    2,2,  
    72, 
    24, 
    1,1, 
    5,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn5, 
    ArrangeVideoAVMbrowsecallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"AVMbrowse");
  b5.item = -1;
  char *menu6[]  = { 
    (char *)"Simultaneous",
    (char *)"One by One",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r6 = { 
    'r',
    9,247,  
    170,305,   
    8,0,  
    150, 
    25, 
    1,2, 
    0,2, 
    (int *)v[3], 
    NULL, 
    NULL ,
    NULL,ArrangeVideoAVMradiocallback , /* *args, Callback  */
    1,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /*=1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu6);
  r6.list=(void **)th0;
  strcpy(r6.Wid,(char *)"AVMradio");
  r6.item = -1;
  DIX x7 = { 
    'x',
    25,41,  
    475,171,   
    10,2,  
    408, 
    25, 
    1,24, 
    0,4, 
    (int *)v[4], 
    NULL, 
    NULL, 
    NULL,ArrangeVideoAVMlistcallback , /* *args, Callback  */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    0, /* bordr */
    1, /* bkgr */
    0  /*=1 hide  */
   };
  strcpy(x7.Wid,(char *)"AVMlist");
  x7.item = -1;
  BUT_STR  *butn8=NULL; 
  butn8= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn8[0].sw=1;
  strcpy(butn8[0].title,(char *)"");
  butn8[0].xpmn=NULL;
  butn8[0].xpmp=NULL;
  butn8[0].xpmh=NULL;
  butn8[0].bkgr=0;
  butn8[0].butncode=124;
  DIN b8 = { 
    'n',
    361,238,  
    439,316,
    2,2,  
    72, 
    72, 
    1,1, 
    1,0.000000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn8, 
    ArrangeVideoAVMcolorcallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b8.Wid,(char *)"AVMcolor");
  b8.item = -1;
  DIM m9 = { 
    'm',
    323,216,  
    493,238,  
    1,0  
  };
  strncpy(m9.msg,(char *)"!c03Select !c38Background !c01Color",499);
  strcpy(m9.Wid,(char *)"AVMmsg1");
  m9.item = -1;
  DIM m10 = { 
    'm',
    26,223,  
    176,247,  
    -1,0  
  };
  strncpy(m10.msg,(char *)"Video playing style",499);
  strcpy(m10.Wid,(char *)"AVMmsg3");
  m10.item = -1;
  DIM m11 = { 
    'm',
    180,224,  
    320,244,  
    -1,0  
  };
  strncpy(m11.msg,(char *)"Output dimension",499);
  strcpy(m11.Wid,(char *)"AVMmsg2");
  m11.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+13));
  d =dtmp+i; 
  d[12].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIN));
  *d[0].N = b0;
  d[0].N->item = -1;
  ArrangeVideoAVMbutnsinit(d[0].N,pt) ;
  d[1].t = (DIT *)malloc(sizeof(DIT));
  *d[1].t = t1;
  d[1].t->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DII));
  *d[2].i = i2;
  d[2].i->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIL));
  *d[3].h = h3;
  d[3].h->item = -1;
  ArrangeVideoAVMgoinit(d[3].h,pt) ;
  d[4].t = (DIT *)malloc(sizeof(DIT));
  *d[4].t = t4;
  d[4].t->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  *d[5].N = b5;
  d[5].N->item = -1;
  ArrangeVideoAVMbrowseinit(d[5].N,pt) ;
  d[6].t = (DIT *)malloc(sizeof(DIRA));
  *d[6].r = r6;
  d[6].r->item = -1;
  ArrangeVideoAVMradioinit(d[6].r,pt) ;
  d[7].t = (DIT *)malloc(sizeof(DIX));
  *d[7].x = x7;
  d[7].x->item = -1;
  ArrangeVideoAVMlistinit(d[7].x,pt) ;
  d[8].t = (DIT *)malloc(sizeof(DIN));
  *d[8].N = b8;
  d[8].N->item = -1;
  ArrangeVideoAVMcolorinit(d[8].N,pt) ;
  d[9].t = (DIT *)malloc(sizeof(DIM));
  *d[9].m = m9;
  d[9].m->item = -1;
  d[10].t = (DIT *)malloc(sizeof(DIM));
  *d[10].m = m10;
  d[10].m->item = -1;
  d[11].t = (DIT *)malloc(sizeof(DIM));
  *d[11].m = m11;
  d[11].m->item = -1;
  d[12].t = NULL;
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

    Text_Box1  2 data values
    Text_Box2  1 data values
    RadioButtons1  1 data value
    Selectmenu2  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   char  *v2 ;
   v2 = (char *)malloc(sizeof(char)*500);
   v2[0] = '\0';
   int  *v3 ;
   v3 = (int *)malloc(sizeof(int));
   *v3 = 1;
   int  *v4 ;
   v4 = (int *)malloc(sizeof(int));
   *v4 = 1;
   void** v=(void **)malloc(sizeof(void*)*6);
   v[5]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   v[4]=(void *)(v4);
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
  D.df = 8;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 886;   /* Position of Dialog */ 
  D.yo = 174;
  D.xl = 498;    /*  Length of Dialog */
  D.yl = 438;    /*  Width  of Dialog */
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

    Text_Box1  2 data values
    Text_Box2  1 data values
    RadioButtons1  1 data value
    Selectmenu2  1 data value

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   char  v2[500]="" ;
   int   v3 = 1;
   int   v4 = 1;
   void* v[5];
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(&v3);
   v[4]=(void *)(&v4);
   void *pt[2]={NULL,NULL}; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   pt[0]=args;
   ArrangeVideo(parent,v,(void *)pt );
   return pt[1];
}
