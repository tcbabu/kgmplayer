#include <kulina.h>
#include "VRCallbacks.h"
#include "GclrVR.c"
int VRGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(16);
  strcpy(e0[0].fmt,(char *)"Input Video%25s");
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
    NULL,VRVRinputcallback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t0.Wid,(char *)"VRinput");
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
    VRVRinputbrowsecallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"VRinputbrowse");
  b1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"Go...");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=-1;
  butn2[0].butncode=127;
  butn2[1].sw=0;
  strcpy(butn2[1].title,(char *)"Okay");
  butn2[1].xpmn=NULL;
  butn2[1].xpmp=NULL;
  butn2[1].xpmh=NULL;
  butn2[1].bkgr=-1;
  butn2[1].butncode=16053374;
  DIL h2 = { 
    'h',
    151,325,  
    309,359,
    2,0,  
    72, 
    25, 
    2,1, 
    2,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn2, 
    VRVRgocallback ,  /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(h2.Wid,(char *)"VRgo");
  h2.item = -1;
  DII i3 = { 
    'i',
    55,239,  
    436,313,  
    41,3,0   
  };
  strcpy(i3.Wid,(char *)"VRIbox");
  i3.item = -1;
  T_ELMT *e4  ; 
  e4 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e4[0].fmt = (char *)malloc(11);
  strcpy(e4[0].fmt,(char *)"Output%30s");
  e4[0].v=(void *)v[1];
  e4[0].sw=1;
  e4[0].noecho=0;
  e4[0].img=NULL;
  DIT t4 = { 
    't',
    8,62,  
    385,96,
    20, 
    1,1, 
    e4,
    1,1,
    NULL,VRVRoutcallback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t4.Wid,(char *)"VRout");
  t4.pt=NULL;
  t4.type = 1;
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
    385,62,  
    466,95,
    2,2,  
    72, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn5, 
    VRVROutbrowsecallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"VROutbrowse");
  b5.item = -1;
  T_ELMT *e6  ; 
  e6 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e6[0].fmt = (char *)malloc(19);
  strcpy(e6[0].fmt,(char *)"Roatation Angle%5F");
  e6[0].v=(void *)v[2];
  e6[0].sw=1;
  e6[0].noecho=0;
  e6[0].img=NULL;
  DIT t6 = { 
    't',
    99,152,  
    332,186,
    20, 
    1,1, 
    e6,
    1,1,
    NULL,VRVRanglecallback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t6.Wid,(char *)"VRangle");
  t6.pt=NULL;
  t6.type = 0;
  t6.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+8));
  d =dtmp+i; 
  d[7].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  VRVRinputbrowseinit(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIL));
  *d[2].h = h2;
  d[2].h->item = -1;
  VRVRgoinit(d[2].h,pt) ;
  d[3].t = (DIT *)malloc(sizeof(DII));
  *d[3].i = i3;
  d[3].i->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIT));
  *d[4].t = t4;
  d[4].t->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  *d[5].N = b5;
  d[5].N->item = -1;
  VRVROutbrowseinit(d[5].N,pt) ;
  d[6].t = (DIT *)malloc(sizeof(DIT));
  *d[6].t = t6;
  d[6].t->item = -1;
  d[7].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeVRGroup(DIALOG *D,void *arg) {
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
   *v2 = 0.0;
   void** v=(void **)malloc(sizeof(void*)*4);
   v[3]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   void *pt=NULL; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   GrpId = VRGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int VR( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = VRGroup(&D,v,pt);
#else 
  GrpId = MakeVRGroup(&D,pt); // can try this also
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
  D.xo = 828;   /* Position of Dialog */ 
  D.yo = 148;
  D.xl = 494;    /*  Length of Dialog */
  D.yl = 374;    /*  Width  of Dialog */
  D.Initfun = VRinit;    /*   init fuction for Dialog */
  D.Cleanupfun = VRcleanup;    /*   cleanup fuction for Dialog */
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
  D.Callback = VRCallBack;    /*  default Callback  */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = VRResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = VRWaitCallBack;  /*  Wait callback */
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
  ModifyVRGc(&D);    /*  set colors for gui if do not like default*/
  ModifyVR(&D,GrpId);    /*  add extras to  gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunVR(void *parent ,void *args) {
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
   VR(parent,v,(void *)pt );
   return pt[1];
}
