#include <kulina.h>
#include "GetSinfoCallbacks.h"
void ModifyGetSinfoGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int GetSinfoGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*2);
  e0[0].fmt = (char *)malloc(14);
  strcpy(e0[0].fmt,(char *)"Pos(sec):%12F");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  e0[1].fmt = (char *)malloc(14);
  strcpy(e0[1].fmt,(char *)"Duration:%12F");
  e0[1].v=(void *)v[1];
  e0[1].sw=1;
  e0[1].noecho=0;
  DIT t0 = { 
    't',
    49,20,  
    291,84,
    20, 
    1,2, 
    e0,
    1,1,
    NULL,GetSinfotextbox1callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"GetSinfoWidget2");
  t0.pt=NULL;
  t0.type = 0;
  t0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Cancel");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"Okay");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode='';
  DIL h1 = { 
    'h',
    92,90,  
    248,123,
    2,0,  
    72, 
    25, 
    2,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    GetSinfosplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h1.Wid,(char *)"GetSinfoWidget5");
  h1.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+3));
  d =dtmp+i; 
  d[2].t=NULL;
  d[1].t = (DIT *)malloc(sizeof(DIT));
  *d[1].t = t0;
  d[1].t->item = -1;
  d[0].t = (DIT *)malloc(sizeof(DIL));
  GetSinfosplbutton1init(&h1,pt) ;
  *d[0].h = h1;
  d[0].h->item = -1;
  d[2].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeGetSinfoGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  2 data values

*************************************************/
   double *v0 ;
   v0 = (double *)malloc(sizeof(double));
   *v0 = 0.0;
   double *v1 ;
   v1 = (double *)malloc(sizeof(double));
   *v1 = 0.0;
   void** v=(void **)malloc(sizeof(void*)*3);
   v[2]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = GetSinfoGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int GetSinfo( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = GetSinfoGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 207;   /* Position of Dialog */ 
  D.yo = 72;
  D.xl = 351;    /*  Length of Dialog */
  D.yl = 130;    /*  Width  of Dialog */
  D.Initfun = GetSinfoinit;    /*   init fuction for Dialog */
  D.Cleanupfun = GetSinfocleanup;    /*   init fuction for Dialog */
  D.kbattn = 1;    /*  1 for drawing keyborad attention */
  D.butattn = 1;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = GetSinfoCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = GetSinfoResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = GetSinfoWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
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
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyGetSinfoGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunGetSinfo(void *arg) {
/*************************************************

    Text_Box1  2 data values

*************************************************/
   char *buff;
   double v0 = 0.0;
   double v1 = 0.0;
   void* v[2];
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   void *pt=NULL; /* pointer to send any extra information */
   if(GetSinfo(arg,v,pt )==1) return NULL;
   if( v0 < 0 ) return NULL;
   else {
    buff = (char *) malloc(100);
    sprintf(buff,"%10lf  %10lf",v0,v1);
   }
   return buff;
}
