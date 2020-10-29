#include <kulina.h>
#include "normalizeCallbacks.h"
double meanlevel=-16;
void ModifynormalizeGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}

int normalizeGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(5);
  strcpy(e0[0].fmt,(char *)"%25s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  DIT t0 = { 
    't',
    96,24,  
    362,55,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,normalizetextbox1callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"NomInputWidget");
  t0.pt=NULL;
  t0.type = 0;
  t0.item = -1;
  T_ELMT *e1  ; 
  e1 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e1[0].fmt = (char *)malloc(5);
  strcpy(e1[0].fmt,(char *)"%25s");
  e1[0].v=(void *)v[1];
  e1[0].sw=1;
  e1[0].noecho=0;
  DIT t1 = { 
    't',
    97,58,  
    363,88,
    20, 
    1,1, 
    e1,
    1,1,
    NULL,normalizetextbox2callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t1.Wid,(char *)"NomOutputWidget");
  t1.pt=NULL;
  t1.type = 0;
  t1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"Browse");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=-1;
  butn2[0].butncode='';
  DIN b2 = { 
    'n',
    360,21,  
    415,55,
    2,2,  
    48, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    normalizebutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b2.Wid,(char *)"normalizeWidget5");
  b2.item = -1;
  T_ELMT *e3  ; 
  e3 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e3[0].fmt = (char *)malloc(9);
  strcpy(e3[0].fmt,(char *)"Mean%10F");
  e3[0].v=(void *)v[2];
  e3[0].sw=1;
  e3[0].noecho=0;
  DIT t3 = { 
    't',
    187,94,  
    361,126,
    20, 
    1,1, 
    e3,
    1,1,
    NULL,normalizetextbox3callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t3.Wid,(char *)"NomMeanWidget");
  t3.pt=NULL;
  t3.type = 0;
  t3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"Do it");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-1;
  butn4[0].butncode='';
  DIL h4 = { 
    'h',
    195,211,  
    280,242,
    2,0,  
    72, 
    25, 
    1,1, 
    2,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    normalizesplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h4.Wid,(char *)"normalizeWidget12");
  h4.item = -1;
  DII i5 = { 
    'i',
    35,133,  
    420,203,  
    41,2,0   
  };
  strcpy(i5.Wid,(char *)"NomInfoBox");
  i5.item = -1;
  DIM m6 = { 
    'm',
    29,30,  
    95,54,  
    1,0  
  };
  strncpy(m6.msg,(char *)"Input File",499);
  strcpy(m6.Wid,(char *)"normalizeWidget7");
  m6.item = -1;
  DIM m7 = { 
    'm',
    29,64,  
    100,87,  
    1,0  
  };
  strncpy(m7.msg,(char *)"Output File",499);
  strcpy(m7.Wid,(char *)"normalizeWidget8");
  m7.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+9));
  d =dtmp+i; 
  d[8].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIT));
  *d[1].t = t1;
  d[1].t->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIN));
  normalizebutton1init(&b2,pt) ;
  *d[2].N = b2;
  d[2].N->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIT));
  *d[3].t = t3;
  d[3].t->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIL));
  normalizesplbutton1init(&h4,pt) ;
  *d[4].h = h4;
  d[4].h->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DII));
  *d[5].i = i5;
  d[5].i->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIM));
  *d[6].m = m6;
  d[6].m->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIM));
  *d[7].m = m7;
  d[7].m->item = -1;
  d[8].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakenormalizeGroup(DIALOG *D,void *arg) {
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
   *v2 = meanlevel;
   void** v=(void **)malloc(sizeof(void*)*4);
   v[3]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = normalizeGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int normalize( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = normalizeGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 2;
  D.df = 4;
  D.tw = 4;
  D.bw = 6;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 492;    /*  Length of Dialog */
  D.yl = 257;    /*  Width  of Dialog */
  D.Initfun = normalizeinit;    /*   init fuction for Dialog */
  D.Cleanupfun = normalizecleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
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
  D.Callback = normalizeCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = normalizeResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = normalizeWaitCallBack;  /*  Wait callback */
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
//  ModifynormalizeGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *Runnormalize(void *arg) {
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    Text_Box3  1 data values

*************************************************/
   char  v0[500]="" ;
   char  v1[500]="" ;
   double v2 = 0.0;
   void* v[3];
   v2 = meanlevel;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(&v2);
   void *pt=NULL; /* pointer to send any extra information */
   normalize(NULL,v,pt );
   return NULL;
}
