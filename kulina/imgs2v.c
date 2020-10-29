#include <kulina.h>
#include "imgs2vCallbacks.h"
#include "images2videos.h"
extern IMGS2VDATA is2vdata;
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int ScrFit=1;
void Modifyimgs2vGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int imgs2vGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIM m0 = { 
    'm',
    18,11,  
    118,35,  
    -1,0  
  };
  strncpy(m0.msg,(char *)"Input Images",499);
  strcpy(m0.Wid,(char *)"imgs2vWidget1");
  m0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Add");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"Del");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode='';
  DIN b1 = { 
    'n',
    296,9,  
    404,41,
    2,2,  
    48, 
    22, 
    2,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    imgs2vbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"imgs2vWidget2");
  b1.item = -1;
  DIX x2 = { 
    'x',
    12,41,  
    412,166,   
    10,2,  
    358, 
    25, 
    1,22, 
    0,4, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,imgs2vbrowser1callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(x2.Wid,(char *)"imgs2vBrowser");
  x2.item = -1;
  T_ELMT *e3  ; 
  e3 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e3[0].fmt = (char *)malloc(15);
  strcpy(e3[0].fmt,(char *)"Video File%30s");
  e3[0].v=(void *)v[1];
  e3[0].sw=1;
  e3[0].noecho=0;
  DIT t3 = { 
    't',
    3,167,  
    411,199,
    20, 
    1,1, 
    e3,
    1,1,
    NULL,imgs2vtextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t3.Wid,(char *)"imgs2vVideo");
  t3.pt=NULL;
  t3.type = 0;
  t3.item = -1;
  T_ELMT *e4  ; 
  e4 =(T_ELMT *)malloc(sizeof(T_ELMT)*3);
  e4[0].fmt = (char *)malloc(8);
  strcpy(e4[0].fmt,(char *)"Xres%4d");
  e4[0].v=(void *)v[2];
  e4[0].sw=1;
  e4[0].noecho=0;
  e4[1].fmt = (char *)malloc(8);
  strcpy(e4[1].fmt,(char *)"Yres%4d");
  e4[1].v=(void *)v[3];
  e4[1].sw=1;
  e4[1].noecho=0;
  e4[2].fmt = (char *)malloc(14);
  strcpy(e4[2].fmt,(char *)"Image Time%3F");
  e4[2].v=(void *)v[4];
  e4[2].sw=1;
  e4[2].noecho=0;
  DIT t4 = { 
    't',
    7,197,  
    415,228,
    20, 
    3,1, 
    e4,
    1,1,
    NULL,imgs2vtextbox2callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t4.Wid,(char *)"OptionsWidget");
  t4.pt=NULL;
  t4.type = 0;
  t4.item = -1;
  char *menu5[]  = { 
    (char *)"Yes",
    (char *)"No",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r5 = { 
    'r',
    124,233,  
    282,260,   
    8,0,  
    50, 
    25, 
    1,2, 
    -1800891242,1, 
    (int *)v[5], 
    NULL, 
    NULL ,
    NULL,imgs2vbrowser2callback, /* *args, callback */
    1,  /* Border Offset  */
     3,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu5);
  r5.list=(void **)th0;
  strcpy(r5.Wid,(char *)"FitWidget");
  r5.item = -1;
  DIM m6 = { 
    'm',
    21,232,  
    126,260,  
    1,0  
  };
  strncpy(m6.msg,(char *)"Cover Full Frame",499);
  strcpy(m6.Wid,(char *)"imgs2vWidget8");
  m6.item = -1;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(char *)"Create");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-177196221;
  butn7[0].butncode='';
  butn7[1].sw=1;
  strcpy(butn7[1].title,(char *)"Clear");
  butn7[1].xpmn=NULL;
  butn7[1].xpmp=NULL;
  butn7[1].xpmh=NULL;
  butn7[1].bkgr=-255162162;
  butn7[1].butncode='';
  DIL h7 = { 
    'h',
    277,229,  
    417,262,
    2,0,  
    64, 
    25, 
    2,1, 
    2,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    imgs2vsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h7.Wid,(char *)"imgs2vWidget9");
  h7.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+9));
  d =dtmp+i; 
  d[8].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIM));
  *d[0].m = m0;
  d[0].m->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  imgs2vbutton1init(&b1,pt) ;
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIX));
  imgs2vbrowser1init(&x2,pt) ;
  *d[2].x = x2;
  d[2].x->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIT));
  *d[3].t = t3;
  d[3].t->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIT));
  *d[4].t = t4;
  d[4].t->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIRA));
  imgs2vbrowser2init(&r5,pt) ;
  *d[5].r = r5;
  d[5].r->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIM));
  *d[6].m = m6;
  d[6].m->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIL));
  imgs2vsplbutton1init(&h7,pt) ;
  *d[7].h = h7;
  d[7].h->item = -1;
  d[8].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int Makeimgs2vGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  1 data values
    Text_Box2  3 data values
    RadioButtons2  1 data value

*************************************************/
   char Folder[500],Output[500];
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   char  *v1 ;
   v1 = (char *)malloc(sizeof(char)*500);
   v1[0] = '\0';
   int  *v2 ;
   v2 = (int *)malloc(sizeof(int));
   *v2 = 1280;
   int  *v3 ;
   v3 = (int *)malloc(sizeof(int));
   *v3 = 720;
   double *v4 ;
   v4 = (double *)malloc(sizeof(double));
   *v4 = 5.0;
   int  *v5 ;
   v5 = (int *)malloc(sizeof(int));
   *v5 = 1;
   void** v=(void **)malloc(sizeof(void*)*7);
   sprintf(Output,"%-s/Images.jpg",getenv("HOME"));
   sprintf(Folder,"%-s/Video",getenv("HOME"));
   MakeFileInFolder(Output,Folder,Output,"mp4");
   strcpy(v1,Output);
   is2vdata.List = NULL;
   
   v[6]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   v[4]=(void *)(v4);
   v[5]=(void *)(v5);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = imgs2vGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int imgs2v( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = imgs2vGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 7;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 427;    /*  Length of Dialog */
  D.yl = 271;    /*  Width  of Dialog */
  D.Initfun = imgs2vinit;    /*   init fuction for Dialog */
  D.Cleanupfun = imgs2vcleanup;    /*   init fuction for Dialog */
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
  D.Callback = imgs2vCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = imgs2vResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = imgs2vWaitCallBack;  /*  Wait callback */
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
//  Modifyimgs2vGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *Runimgs2v(void *arg) {
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  1 data values
    Text_Box2  3 data values
    RadioButtons2  1 data value

*************************************************/
   int   v0 = 1;
   char  v1[500]="" ;
   int   v2 = 1280;
   int   v3 = 720;
   double v4 = 5.0;
   int   v5 = ScrFit;
   sprintf(v1,"%-s/Video/Pictures.mp4",getenv("HOME"));
   void* v[6];
   v[0]=(void *)(&v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   v[4]=(void *)(&v4);
   v[5]=(void *)(&v5);
   void *pt=NULL; /* pointer to send any extra information */
   imgs2v(NULL,v,pt );
   return NULL;
}
