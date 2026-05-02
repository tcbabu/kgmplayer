#include <kulina.h>
#include "OverlayCallbacks.h"
#include "GclrOverlay.c"
int OverlayGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(12);
  strcpy(e0[0].fmt,(char *)"Video 1%29s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  e0[0].img=NULL;
  DIT t0 = { 
    't',
    9,25,  
    386,59,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,OverlayOLYinput1callback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t0.Wid,(char *)"OLYinput1");
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
    385,24,  
    467,58,
    2,2,  
    72, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn1, 
    OverlayOLYinput1browsecallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"OLYinput1browse");
  b1.item = -1;
  T_ELMT *e2  ; 
  e2 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e2[0].fmt = (char *)malloc(12);
  strcpy(e2[0].fmt,(char *)"Video 2%29s");
  e2[0].v=(void *)v[1];
  e2[0].sw=1;
  e2[0].noecho=0;
  e2[0].img=NULL;
  DIT t2 = { 
    't',
    9,65,  
    386,99,
    20, 
    1,1, 
    e2,
    1,1,
    NULL,OverlayOLYinput2callback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t2.Wid,(char *)"OLYinput2");
  t2.pt=NULL;
  t2.type = 1;
  t2.item = -1;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"Browse");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-1;
  butn3[0].butncode=31;
  DIN b3 = { 
    'n',
    385,62,  
    467,96,
    2,2,  
    72, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn3, 
    OverlayOLYinput2browsecallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b3.Wid,(char *)"OLYinput2browse");
  b3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"Go...");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-1;
  butn4[0].butncode=126;
  butn4[1].sw=0;
  strcpy(butn4[1].title,(char *)"Okay");
  butn4[1].xpmn=NULL;
  butn4[1].xpmp=NULL;
  butn4[1].xpmh=NULL;
  butn4[1].bkgr=-1;
  butn4[1].butncode=126;
  DIL h4 = { 
    'h',
    151,325,  
    309,359,
    2,0,  
    72, 
    25, 
    2,1, 
    2,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn4, 
    OverlayOLYgocallback ,  /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(h4.Wid,(char *)"OLYgo");
  h4.item = -1;
  DII i5 = { 
    'i',
    55,239,  
    436,313,  
    41,3,0   
  };
  strcpy(i5.Wid,(char *)"OLYIbox");
  i5.item = -1;
  T_ELMT *e6  ; 
  e6 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e6[0].fmt = (char *)malloc(11);
  strcpy(e6[0].fmt,(char *)"Output%30s");
  e6[0].v=(void *)v[2];
  e6[0].sw=1;
  e6[0].noecho=0;
  e6[0].img=NULL;
  DIT t6 = { 
    't',
    8,136,  
    385,170,
    20, 
    1,1, 
    e6,
    1,1,
    NULL,OverlayOLYoutcallback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t6.Wid,(char *)"OLYout");
  t6.pt=NULL;
  t6.type = 1;
  t6.item = -1;
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
    385,136,  
    466,169,
    2,2,  
    72, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn7, 
    OverlayOLYOutbrowsecallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"OLYOutbrowse");
  b7.item = -1;
  T_ELMT *e8  ; 
  e8 =(T_ELMT *)malloc(sizeof(T_ELMT)*2);
  e8[0].fmt = (char *)malloc(25);
  strcpy(e8[0].fmt,(char *)"Overlay pos(top left)%6d");
  e8[0].v=(void *)v[3];
  e8[0].sw=1;
  e8[0].noecho=0;
  e8[0].img=NULL;
  e8[1].fmt = (char *)malloc(4);
  strcpy(e8[1].fmt,(char *)"%6d");
  e8[1].v=(void *)v[4];
  e8[1].sw=1;
  e8[1].noecho=0;
  e8[1].img=NULL;
  DIT t8 = { 
    't',
    47,189,  
    438,223,
    20, 
    2,1, 
    e8,
    1,1,
    NULL,OverlayOLYloccallback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t8.Wid,(char *)"OLYloc");
  t8.pt=NULL;
  t8.type = 0;
  t8.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+10));
  d =dtmp+i; 
  d[9].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  OverlayOLYinput1browseinit(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIT));
  *d[2].t = t2;
  d[2].t->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIN));
  *d[3].N = b3;
  d[3].N->item = -1;
  OverlayOLYinput2browseinit(d[3].N,pt) ;
  d[4].t = (DIT *)malloc(sizeof(DIL));
  *d[4].h = h4;
  d[4].h->item = -1;
  OverlayOLYgoinit(d[4].h,pt) ;
  d[5].t = (DIT *)malloc(sizeof(DII));
  *d[5].i = i5;
  d[5].i->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIT));
  *d[6].t = t6;
  d[6].t->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  *d[7].N = b7;
  d[7].N->item = -1;
  OverlayOLYOutbrowseinit(d[7].N,pt) ;
  d[8].t = (DIT *)malloc(sizeof(DIT));
  *d[8].t = t8;
  d[8].t->item = -1;
  d[9].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeOverlayGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    Text_Box3  1 data values
    Text_Box4  2 data values

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(char)*500);
   v0[0] = '\0';
   char  *v1 ;
   v1 = (char *)malloc(sizeof(char)*500);
   v1[0] = '\0';
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
   GrpId = OverlayGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int Overlay( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = OverlayGroup(&D,v,pt);
#else 
  GrpId = MakeOverlayGroup(&D,pt); // can try this also
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
  D.xo = 828;   /* Position of Dialog */ 
  D.yo = 148;
  D.xl = 494;    /*  Length of Dialog */
  D.yl = 374;    /*  Width  of Dialog */
  D.Initfun = Overlayinit;    /*   init fuction for Dialog */
  D.Cleanupfun = Overlaycleanup;    /*   cleanup fuction for Dialog */
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
  D.Callback = OverlayCallBack;    /*  default Callback  */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = OverlayResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = OverlayWaitCallBack;  /*  Wait callback */
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
  ModifyOverlayGc(&D);    /*  set colors for gui if do not like default*/
  ModifyOverlay(&D,GrpId);    /*  add extras to  gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunOverlay(void *parent ,void *args) {
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    Text_Box3  1 data values
    Text_Box4  2 data values

*************************************************/
   char  v0[500]="" ;
   char  v1[500]="" ;
   char  v2[500]="" ;
   int   v3 = 1;
   int   v4 = 1;
   void* v[5];
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(&v3);
   v[4]=(void *)(&v4);
   void *pt[2]={NULL,NULL}; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   pt[0]=args;
   Overlay(parent,v,(void *)pt );
   return pt[1];
}
