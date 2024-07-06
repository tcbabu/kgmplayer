#include <kulina.h>
#include "imgs2vCallbacks.h"
#include "images2videos.h"
extern IMGS2VDATA is2vdata;
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
int ScrFit=1;

void Modifyimgs2vGc(void *Tmp) {
   DIALOG *D;
   Gclr *gc;
   D = (DIALOG *)Tmp;
   gc = &(D->gc);
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   kgColorTheme(D,220,220,200);
   kgColorTheme1(D,220,220,200);
   kgColorTheme2(D,220,220,200);
   kgDefaultGuiTheme(gc);
   kgGrayGuiTheme(gc);
   gc->FontSize =8;
   gc->Font=23;
   kgMkgclr("imgs2v",Tmp);
*/
}
int imgs2vGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIM m0 = { 
    'm',
    32,41,  
    132,65,  
    -1,0  
  };
  strncpy(m0.msg,(char *)"!w32!f21Image Files",499);
  strcpy(m0.Wid,(char *)"imgs2vWidget1");
  m0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"!w32!f23Add");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-240245255;
  butn1[0].butncode='';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"!w32!f23Delete");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-240245255;
  butn1[1].butncode='';
  butn1[2].sw=1;
  strcpy(butn1[2].title,(char *)"!w32!f23Reorder");
  butn1[2].xpmn=NULL;
  butn1[2].xpmp=NULL;
  butn1[2].xpmh=NULL;
  butn1[2].bkgr=-240245255;
  butn1[2].butncode='';
  DIN b1 = { 
    'n',
    227,30,  
    457,64,
    2,2,  
    72, 
    24, 
    3,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    imgs2vbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"imgs2vWidget2");
  b1.item = -1;
  DIX x2 = { 
    'x',
    32,65,  
    459,233,   
    10,2,  
    180, 
    25, 
    1,24, 
    0,5, 
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
  e3[0].fmt = (char *)malloc(5);
  strcpy(e3[0].fmt,(char *)"%30s");
  e3[0].v=(void *)v[1];
  e3[0].sw=1;
  e3[0].noecho=0;
  e3[0].img=NULL;
  DIT t3 = { 
    't',
    142,235,  
    457,269,
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
  DIM m4 = { 
    'm',
    8,242,  
    142,265,  
    1,0  
  };
  strncpy(m4.msg,(char *)"!w32!f21Video Output File",499);
  strcpy(m4.Wid,(char *)"imgs2vWidget5");
  m4.item = -1;
  T_ELMT *e5  ; 
  e5 =(T_ELMT *)malloc(sizeof(T_ELMT)*3);
  e5[0].fmt = (char *)malloc(10);
  strcpy(e5[0].fmt,(char *)"Xres  %4d");
  e5[0].v=(void *)v[2];
  e5[0].sw=1;
  e5[0].noecho=0;
  e5[0].img=NULL;
  e5[1].fmt = (char *)malloc(10);
  strcpy(e5[1].fmt,(char *)"Yres  %4d");
  e5[1].v=(void *)v[3];
  e5[1].sw=1;
  e5[1].noecho=0;
  e5[1].img=NULL;
  e5[2].fmt = (char *)malloc(14);
  strcpy(e5[2].fmt,(char *)"Image Time%3F");
  e5[2].v=(void *)v[4];
  e5[2].sw=1;
  e5[2].noecho=0;
  e5[2].img=NULL;
  DIT t5 = { 
    't',
    6,269,  
    459,304,
    20, 
    3,1, 
    e5,
    1,1,
    NULL,imgs2vtextbox2callback,0,0,28,20 /* args,Call back */
  };
  strcpy(t5.Wid,(char *)"OptionsWidget");
  t5.pt=NULL;
  t5.type = 0;
  t5.item = -1;
  char *menu6[]  = { 
    (char *)"!w32!f21Yes",
    (char *)"!w32!f21No",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r6 = { 
    'r',
    214,309,  
    458,344,   
    8,0,  
    90, 
    25, 
    1,2, 
    32584,1, 
    (int *)v[5], 
    NULL, 
    NULL ,
    NULL,imgs2vbrowser2callback, /* *args, callback */
    4,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu6);
  r6.list=(void **)th0;
  strcpy(r6.Wid,(char *)"FitWidget");
  r6.item = -1;
  DIM m7 = { 
    'm',
    9,316,  
    216,339,  
    1,0  
  };
  strncpy(m7.msg,(char *)"!w32!f21Cover Full Frame",499);
  strcpy(m7.Wid,(char *)"imgs2vWidget8");
  m7.item = -1;
  BUT_STR  *butn8=NULL; 
  butn8= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn8[0].sw=1;
  strcpy(butn8[0].title,(char *)"!w32!f23Create");
  butn8[0].xpmn=NULL;
  butn8[0].xpmp=NULL;
  butn8[0].xpmh=NULL;
  butn8[0].bkgr=-235255250;
  butn8[0].butncode='';
  butn8[1].sw=1;
  strcpy(butn8[1].title,(char *)"!c12!w32!f23Clear");
  butn8[1].xpmn=NULL;
  butn8[1].xpmp=NULL;
  butn8[1].xpmh=NULL;
  butn8[1].bkgr=-142142142;
  butn8[1].butncode='';
  DIL h8 = { 
    'h',
    160,360,  
    316,393,
    2,0,  
    72, 
    25, 
    2,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn8, 
    imgs2vsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h8.Wid,(char *)"imgs2vWidget9");
  h8.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+10));
  d =dtmp+i; 
  d[9].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIM));
  *d[0].m = m0;
  d[0].m->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  imgs2vbutton1init(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIX));
  *d[2].x = x2;
  d[2].x->item = -1;
  imgs2vbrowser1init(d[2].x,pt) ;
  d[3].t = (DIT *)malloc(sizeof(DIT));
  *d[3].t = t3;
  d[3].t->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIM));
  *d[4].m = m4;
  d[4].m->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIT));
  *d[5].t = t5;
  d[5].t->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIRA));
  *d[6].r = r6;
  d[6].r->item = -1;
  imgs2vbrowser2init(d[6].r,pt) ;
  d[7].t = (DIT *)malloc(sizeof(DIM));
  *d[7].m = m7;
  d[7].m->item = -1;
  d[8].t = (DIT *)malloc(sizeof(DIL));
  *d[8].h = h8;
  d[8].h->item = -1;
  imgs2vsplbutton1init(d[8].h,pt) ;
  d[9].t = NULL;
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
