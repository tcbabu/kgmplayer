#include <kulina.h>
#include "VideoJoinCallbacks.h"
#include "GclrVideoJoin.c"
int VideoJoinGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIX x0 = { 
    'x',
    32,41,  
    459,209,   
    10,2,  
    308, 
    25, 
    1,0, 
    0,5, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,VideoJoinVideoListcallback , /* *args, Callback  */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /*=1 hide  */
   };
  strcpy(x0.Wid,(char *)"VideoList");
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
  butn1[1].butncode=127;
  DIN b1 = { 
    'n',
    301,6,  
    457,40,
    2,2,  
    72, 
    24, 
    2,1, 
    4,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn1, 
    VideoJoinVideoJoinWidget2callback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"VideoJoinWidget2");
  b1.item = -1;
  DIM m2 = { 
    'm',
    34,7,  
    216,37,  
    -1,0  
  };
  strncpy(m2.msg,(char *)"!w32!f21Input Files(Video)",499);
  strcpy(m2.Wid,(char *)"VideoJoinWidget3");
  m2.item = -1;
  T_ELMT *e3  ; 
  e3 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e3[0].fmt = (char *)malloc(19);
  strcpy(e3[0].fmt,(char *)"!w32!f21Output%30s");
  e3[0].v=(void *)v[1];
  e3[0].sw=1;
  e3[0].noecho=0;
  e3[0].img=NULL;
  DIT t3 = { 
    't',
    12,213,  
    385,248,
    20, 
    1,1, 
    e3,
    1,1,
    NULL,VideoJoinVjoinOutcallback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t3.Wid,(char *)"VjoinOut");
  t3.pt=NULL;
  t3.type = 0;
  t3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"!w32!f23 Join Videos");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-235255250;
  butn4[0].butncode=31;
  DIL h4 = { 
    'h',
    193,409,  
    287,442,
    2,0,  
    84, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn4, 
    VideoJoinJoinVideoscallback ,  /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(h4.Wid,(char *)"JoinVideos");
  h4.item = -1;
  char *menu5[]  = { 
    (char *)"!w32!f21Very Good",
    (char *)"!w32!f21Good",
    (char *)"!w32!f21Medium",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r5 = { 
    'r',
    84,250,  
    463,291,   
    8,0,  
    100, 
    25, 
    1,3, 
    0,1, 
    (int *)v[2], 
    NULL, 
    NULL ,
    NULL,VideoJoinVJQualitycallback , /* *args, Callback  */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /*=1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu5);
  r5.list=(void **)th0;
  strcpy(r5.Wid,(char *)"VJQuality");
  r5.item = -1;
  DIM m6 = { 
    'm',
    5,257,  
    84,281,  
    1,0  
  };
  strncpy(m6.msg,(char *)"!w32!f21Quality",499);
  strcpy(m6.Wid,(char *)"VideoJoinWidget8");
  m6.item = -1;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(char *)"Browse");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-1;
  butn7[0].butncode=31;
  DIN b7 = { 
    'n',
    389,214,  
    460,243,
    2,2,  
    64, 
    22, 
    1,1, 
    5,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn7, 
    VideoJoinVJObrowsecallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"VJObrowse");
  b7.item = -1;
  char *menu8[]  = { 
    (char *)"Resize to Full Area",
    (char *)"Keep Aspectratio",
    NULL 
  };
  ThumbNail **th1 ;
  DIRA r8 = { 
    'r',
    58,300,  
    417,327,   
    8,0,  
    165, 
    25, 
    1,2, 
    1,1, 
    (int *)v[3], 
    NULL, 
    NULL ,
    NULL,VideoJoinVJFcovercallback , /* *args, Callback  */
    1,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    0  /*=1 hide  */
   };
  th1 = (ThumbNail **)kgStringToThumbNails((char **)menu8);
  r8.list=(void **)th1;
  strcpy(r8.Wid,(char *)"VJFcover");
  r8.item = -1;
  DII i9 = { 
    'i',
    8,330,  
    460,402,  
    49,3,0   
  };
  strcpy(i9.Wid,(char *)"VJInfo");
  i9.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+11));
  d =dtmp+i; 
  d[10].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIX));
  *d[0].x = x0;
  d[0].x->item = -1;
  VideoJoinVideoListinit(d[0].x,pt) ;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  VideoJoinVideoJoinWidget2init(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIM));
  *d[2].m = m2;
  d[2].m->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIT));
  *d[3].t = t3;
  d[3].t->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIL));
  *d[4].h = h4;
  d[4].h->item = -1;
  VideoJoinJoinVideosinit(d[4].h,pt) ;
  d[5].t = (DIT *)malloc(sizeof(DIRA));
  *d[5].r = r5;
  d[5].r->item = -1;
  VideoJoinVJQualityinit(d[5].r,pt) ;
  d[6].t = (DIT *)malloc(sizeof(DIM));
  *d[6].m = m6;
  d[6].m->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  *d[7].N = b7;
  d[7].N->item = -1;
  VideoJoinVJObrowseinit(d[7].N,pt) ;
  d[8].t = (DIT *)malloc(sizeof(DIRA));
  *d[8].r = r8;
  d[8].r->item = -1;
  VideoJoinVJFcoverinit(d[8].r,pt) ;
  d[9].t = (DIT *)malloc(sizeof(DII));
  *d[9].i = i9;
  d[9].i->item = -1;
  d[10].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeVideoJoinGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  1 data values
    RadioButtons2  1 data value
    RadioButtons3  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   char  *v1 ;
   v1 = (char *)malloc(sizeof(char)*500);
   v1[0] = '\0';
   int  *v2 ;
   v2 = (int *)malloc(sizeof(int));
   *v2 = 1;
   int  *v3 ;
   v3 = (int *)malloc(sizeof(int));
   *v3 = 1;
   void** v=(void **)malloc(sizeof(void*)*5);
   v[4]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   void *pt=NULL; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   GrpId = VideoJoinGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   VideoJoinSetup(D,Gpt->arg);
   return GrpId;
}

int VideoJoin( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = VideoJoinGroup(&D,v,pt);
#else 
  GrpId = MakeVideoJoinGroup(&D,pt); // can try this also
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
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 474;    /*  Length of Dialog */
  D.yl = 448;    /*  Width  of Dialog */
  D.Initfun = VideoJoininit;    /*   init fuction for Dialog */
  D.Cleanupfun = VideoJoincleanup;    /*   cleanup fuction for Dialog */
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
  D.Callback = VideoJoinCallBack;    /*  default Callback  */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = VideoJoinResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = VideoJoinWaitCallBack;  /*  Wait callback */
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
  ModifyVideoJoinGc(&D);    /*  set colors for gui if do not like default*/
  ModifyVideoJoin(&D,GrpId);    /*  add extras to  gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunVideoJoin(void *parent ,void *args) {
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  1 data values
    RadioButtons2  1 data value
    RadioButtons3  1 data value

*************************************************/
   int   v0 = 1;
   char  v1[500]="" ;
   int   v2 = 1;
   int   v3 = 1;
   void* v[4];
   v[0]=(void *)(&v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   void *pt[2]={NULL,NULL}; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   pt[0]=args;
   VideoJoin(parent,v,(void *)pt );
   return pt[1];
}
