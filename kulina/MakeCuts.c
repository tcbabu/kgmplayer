#include <kulina.h>
#include "MakeCutsCallbacks.h"
#include "GclrMakeCuts.c"
int MakeCutsGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(6);
  strcpy(e0[0].fmt,(char *)"%30s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  e0[0].img=NULL;
  DIT t0 = { 
    't',
    72,30,  
    398,64,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,MakeCutstextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"CutInput");
  t0.pt=NULL;
  t0.type = 0;
  t0.item = -1;
  DIM m1 = { 
    'm',
    7,36,  
    91,60,  
    1,0  
  };
  strncpy(m1.msg,(char *)"Audio Media",499);
  strcpy(m1.Wid,(char *)"MakeAuCutsWidget3");
  m1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"Browse");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=-1;
  butn2[0].butncode=127;
  DIN b2 = { 
    'n',
    400,28,  
    474,62,
    2,2,  
    64, 
    24, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    MakeCutsbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b2.Wid,(char *)"MakeAuCutsWidget4");
  b2.item = -1;
  DIX x3 = { 
    'x',
    44,101,  
    442,225,   
    10,2,  
    308, 
    25, 
    1,24, 
    0,4, 
    (int *)v[1], 
    NULL, 
    NULL, 
    NULL,MakeCutsbrowser1callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(x3.Wid,(char *)"CutList");
  x3.item = -1;
  DIM m4 = { 
    'm',
    44,74,  
    144,98,  
    -1,0  
  };
  strncpy(m4.msg,(char *)"Cuts Info",499);
  strcpy(m4.Wid,(char *)"MakeAuCutsWidget7");
  m4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(char *)"Add");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-1;
  butn5[0].butncode=127;
  butn5[1].sw=1;
  strcpy(butn5[1].title,(char *)"Delete");
  butn5[1].xpmn=NULL;
  butn5[1].xpmp=NULL;
  butn5[1].xpmh=NULL;
  butn5[1].bkgr=-1;
  butn5[1].butncode=127;
  DIN b5 = { 
    'n',
    306,68,  
    443,97,
    2,2,  
    64, 
    24, 
    2,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    MakeCutsbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"AcutsBbox2");
  b5.item = -1;
  DIM m6 = { 
    'm',
    93,300,  
    410,326,  
    0,0  
  };
  strncpy(m6.msg,(char *)"!c01!f29Note:Extension decides output format",499);
  strcpy(m6.Wid,(char *)"MakeAuCutsWidget9");
  m6.item = -1;
  T_ELMT *e7  ; 
  e7 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e7[0].fmt = (char *)malloc(6);
  strcpy(e7[0].fmt,(char *)"%30s");
  e7[0].v=(void *)v[2];
  e7[0].sw=1;
  e7[0].noecho=0;
  e7[0].img=NULL;
  DIT t7 = { 
    't',
    90,229,  
    421,260,
    20, 
    1,1, 
    e7,
    1,1,
    NULL,MakeCutstextbox2callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t7.Wid,(char *)"CutOutput");
  t7.pt=NULL;
  t7.type = 0;
  t7.item = -1;
  DIM m8 = { 
    'm',
    9,234,  
    109,258,  
    1,0  
  };
  strncpy(m8.msg,(char *)"Output:",499);
  strcpy(m8.Wid,(char *)"MakeAuCutsWidget13");
  m8.item = -1;
  BUT_STR  *butn9=NULL; 
  butn9= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn9[0].sw=1;
  strcpy(butn9[0].title,(char *)"!f29!c38Make Cuts");
  butn9[0].xpmn=NULL;
  butn9[0].xpmp=NULL;
  butn9[0].xpmh=NULL;
  butn9[0].bkgr=-235255250;
  butn9[0].butncode=127;
  DIL h9 = { 
    'h',
    190,263,  
    281,296,
    2,0,  
    84, 
    25, 
    1,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn9, 
    MakeCutssplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h9.Wid,(char *)"MakeCutsWidget10");
  h9.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+11));
  d =dtmp+i; 
  d[10].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIM));
  *d[1].m = m1;
  d[1].m->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIN));
  *d[2].N = b2;
  d[2].N->item = -1;
  MakeCutsbutton1init(d[2].N,pt) ;
  d[3].t = (DIT *)malloc(sizeof(DIX));
  *d[3].x = x3;
  d[3].x->item = -1;
  MakeCutsbrowser1init(d[3].x,pt) ;
  d[4].t = (DIT *)malloc(sizeof(DIM));
  *d[4].m = m4;
  d[4].m->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  *d[5].N = b5;
  d[5].N->item = -1;
  MakeCutsbutton2init(d[5].N,pt) ;
  d[6].t = (DIT *)malloc(sizeof(DIM));
  *d[6].m = m6;
  d[6].m->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIT));
  *d[7].t = t7;
  d[7].t->item = -1;
  d[8].t = (DIT *)malloc(sizeof(DIM));
  *d[8].m = m8;
  d[8].m->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIL));
  *d[9].h = h9;
  d[9].h->item = -1;
  MakeCutssplbutton1init(d[9].h,pt) ;
  d[10].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeMakeCutsGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Selectmenu1  1 data value
    Text_Box2  1 data values

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(char)*500);
   v0[0] = '\0';
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   char  *v2 ;
   v2 = (char *)malloc(sizeof(char)*500);
   v2[0] = '\0';
   void** v=(void **)malloc(sizeof(void*)*4);
   v[3]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = MakeCutsGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int MakeCuts( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = MakeCutsGroup(&D,v,pt);
#else 
  GrpId = MakeMakeCutsGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 9;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 480;    /*  Length of Dialog */
  D.yl = 331;    /*  Width  of Dialog */
  D.Initfun = MakeCutsinit;    /*   init fuction for Dialog */
  D.Cleanupfun = MakeCutscleanup;    /*   init fuction for Dialog */
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
  D.Callback = MakeCutsCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = MakeCutsResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = MakeCutsWaitCallBack;  /*  Wait callback */
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
  ModifyMakeCuts(&D,GrpId);    /*  add extras to  gui*/
  ModifyMakeCutsGc(&D);    /*  set colors for gui if don't like default*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunMakeCuts(void *arg) {
/*************************************************

    Text_Box1  1 data values
    Selectmenu1  1 data value
    Text_Box2  1 data values

*************************************************/
   char  v0[500]="" ;
   int   v1 = 1;
   char  v2[500]="" ;
   void* v[3];
   v[0]=(void *)(v0);
   v[1]=(void *)(&v1);
   v[2]=(void *)(v2);
   void *pt=NULL; /* pointer to send any extra information */
   MakeCuts(NULL,v,pt );
   return NULL;
}
