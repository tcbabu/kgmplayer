#include <kulina.h>
#include "urlCallbacks.h"
//#define BCLR -69128181
#define BCLR -1
void ModifyurlGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int urlGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(8);
  strcpy(e0[0].fmt,(char *)"URL%20s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  DIT t0 = { 
    't',
    54,40,  
    313,70,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,urltextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"url");
  t0.pt=NULL;
  t0.type = 0;
  t0.item = -1;
  char *menu1[]  = { 
    (char *)"High",
    (char *)"Medium",
    (char *)"Low",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r1 = { 
    'r',
    107,78,  
    344,100,   
    8,0,  
    70, 
    25, 
    1,3, 
    0,1, 
    (int *)v[1], 
    NULL, 
    NULL ,
    NULL,urlbrowser1callback, /* *args, callback */
    0,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu1);
  r1.list=(void **)th0;
  strcpy(r1.Wid,(char *)"urlWidget8");
  r1.item = -1;
  DIM m2 = { 
    'm',
    54,78,  
    107,100,  
    1,0  
  };
  strncpy(m2.msg,(char *)"QUALITY",499);
  strcpy(m2.Wid,(char *)"urlWidget3");
  m2.item = -1;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn3[0].sw=0;
  strcpy(butn3[0].title,(char *)"GraB");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-166246166;
  butn3[0].bkgr= BCLR;
  butn3[0].butncode='B';
  DIN b3 = { 
    'n',
    151,100,  
    223,129,
    2,2,  
    64, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    urlbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b3.Wid,(char *)"urlWidget4");
  b3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=0;
  strcpy(butn4[0].title,(char *)"Settings");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-246246166;
  butn4[0].bkgr= BCLR;
  butn4[0].butncode='';
  DIN b4 = { 
    'n',
    63,100,  
    138,129,
    2,2,  
    64, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    urlbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b4.Wid,(char *)"urlWidget6");
  b4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(char *)"Search");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-246166246;
  butn5[0].bkgr= BCLR;
  butn5[0].butncode='';
  DIN b5 = { 
    'n',
    233,100,  
    310,129,
    2,2,  
    64, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    urlbutton3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"urlWidget7");
  b5.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+7));
  d =dtmp+i; 
  d[6].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIRA));
  urlbrowser1init(&r1,pt) ;
  *d[1].r = r1;
  d[1].r->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIM));
  *d[2].m = m2;
  d[2].m->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIN));
  urlbutton1init(&b3,pt) ;
  *d[3].N = b3;
  d[3].N->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIN));
  urlbutton2init(&b4,pt) ;
  *d[4].N = b4;
  d[4].N->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  urlbutton3init(&b5,pt) ;
  *d[5].N = b5;
  d[5].N->item = -1;
  d[6].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeurlGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    RadioButtons1  1 data value

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(char)*500);
   v0[0] = '\0';
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   void** v=(void **)malloc(sizeof(void*)*3);
   v[2]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = urlGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int url( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = urlGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 5;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 322;   /* Position of Dialog */ 
  D.yo = 166;
  D.xl = 339;    /*  Length of Dialog */
  D.yl = 185;    /*  Width  of Dialog */
  D.Initfun = urlinit;    /*   init fuction for Dialog */
  D.Cleanupfun = urlcleanup;    /*   init fuction for Dialog */
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
  D.Callback = urlCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = urlResizeCallBack;  /*  Resize callback */
#if 1 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = urlWaitCallBack;  /*  Wait callback */
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
//  ModifyurlGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *Runurl(void *arg) {
/*************************************************

    Text_Box1  1 data values
    RadioButtons1  1 data value

*************************************************/
   char  v0[500]="" ;
   int   v1 = 1;
   void* v[2];
   v[0]=(void *)(v0);
   v[1]=(void *)(&v1);
   void *pt=NULL; /* pointer to send any extra information */
   url(NULL,v,pt );
   return NULL;
}
