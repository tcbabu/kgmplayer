#include <kulina.h>
#include "AudioConvertCallbacks.h"
#include "ConvertData.h"
extern CONVDATA cndata;
extern int AConGrp,RangeGrp,EnVoGrp;
void ModifyAudioConvertGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int AudioConvertGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(10);
  strcpy(e0[0].fmt,(char *)"Input%22s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  DIT t0 = { 
    't',
    9,2,  
    302,33,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,AudioConverttextbox1callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"InputWidget");
  t0.pt=NULL;
  t0.type = 0;
  t0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Browse");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='';
  DIN b1 = { 
    'n',
    303,1,  
    397,31,
    2,2,  
    84, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    AudioConvertbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"BrowseWidget");
  b1.item = -1;
  T_ELMT *e2  ; 
  e2 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e2[0].fmt = (char *)malloc(11);
  strcpy(e2[0].fmt,(char *)"Output%31s");
  e2[0].v=(void *)v[1];
  e2[0].sw=1;
  DIT t2 = { 
    't',
    1,30,  
    386,64,
    20, 
    1,1, 
    e2,
    1,1,
    NULL,AudioConverttextbox2callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t2.Wid,(char *)"OutputWidget");
  t2.pt=NULL;
  t2.type = 0;
  t2.item = -1;
  char *menu3[]  = { 
    (char *)"Very good",
    (char *)"Good",
    (char *)"Normal",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r3 = { 
    'r',
    81,67,  
    392,93,   
    8,0,  
    90, 
    25, 
    1,3, 
    -2302756,1, 
    (int *)v[2], 
    NULL, 
    NULL ,
    NULL,AudioConvertbrowser1callback, /* *args, callback */
    0,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu3);
  r3.list=(void **)th0;
  strcpy(r3.Wid,(char *)"AudioConvertWidget8");
  r3.item = -1;
  DIM m4 = { 
    'm',
    5,67,  
    76,90,  
    1,0  
  };
  strncpy(m4.msg,(char *)"Quality",499);
  strcpy(m4.Wid,(char *)"AudioConvertWidget9");
  m4.item = -1;
  char *menu5[]  = { 
    (char *)"Full Media",
    (char *)"Select Range",
    NULL 
  };
  ThumbNail **th1 ;
  DIRA r5 = { 
    'r',
    101,124,  
    344,150,   
    8,0,  
    110, 
    25, 
    1,2, 
    -2302756,1, 
    (int *)v[3], 
    NULL, 
    NULL ,
    NULL,AudioConvertbrowser2callback, /* *args, callback */
    0,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th1 = (ThumbNail **)kgStringToThumbNails((char **)menu5);
  r5.list=(void **)th1;
  strcpy(r5.Wid,(char *)"AudioConvertWidget12");
  r5.item = -1;
  DIM m6 = { 
    'm',
    16,124,  
    96,148,  
    1,0  
  };
  strncpy(m6.msg,(char *)"Range",499);
  strcpy(m6.Wid,(char *)"AudioConvertWidget7");
  m6.item = -1;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(char *)"Convert Audio");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-132206230;
  butn7[0].butncode='';
  DIL h7 = { 
    'h',
    275,199,  
    395,232,
//    275,219,  
//    395,252,
    2,0,  
    110, 
    25, 
    1,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    AudioConvertsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h7.Wid,(char *)"AudioConvertWidget9");
  h7.item = -1;
#if 1
  DIM m7 = { 
    'm',
    16,246,  
    406,266,  
    0,0  
  };
  strncpy(m7.msg,(char *)"!c01Note:Select File Extension for output "
       "format; .wav/.flac  preferable for CUT and JOIN",499);
  strcpy(m7.Wid,(char *)"AudioConvertWidget10");
  m7.item = -1;
#endif
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
#if 0
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+9));
#else
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+10));
#endif
  d =dtmp+i; 
  d[9].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  AudioConvertbutton1init(&b1,pt) ;
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIT));
  *d[2].t = t2;
  d[2].t->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIRA));
  AudioConvertbrowser1init(&r3,pt) ;
  *d[3].r = r3;
  d[3].r->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIM));
  *d[4].m = m4;
  d[4].m->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIRA));
  AudioConvertbrowser2init(&r5,pt) ;
  *d[5].r = r5;
  d[5].r->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIM));
  *d[6].m = m6;
  d[6].m->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIL));
  AudioConvertsplbutton1init(&h7,pt) ;
  *d[7].h = h7;
  d[7].h->item = -1;
  d[8].t = (DIT *)malloc(sizeof(DIM));
  *d[8].m = m7;
  d[8].m->item = -1;
  d[9].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeAudioConvertGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    RadioButtons1  1 data value
    RadioButtons2  1 data value

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(char)*500);
   v0[0] = '\0';
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
   strcpy(v0,cndata.infile);
   strcpy(v1,cndata.infile);
   *v2= cndata.Quality;
   *v3 = 1;
   if(cndata.FullRange!=1) *v3=2;
   v[4]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = AudioConvertGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int AudioConvert( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = AudioConvertGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 7;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 204;   /* Position of Dialog */ 
  D.yo = 3;
  D.xl = 398;    /*  Length of Dialog */
  D.yl = 229;    /*  Width  of Dialog */
  D.Initfun = AudioConvertinit;    /*   init fuction for Dialog */
  D.Cleanupfun = AudioConvertcleanup;    /*   init fuction for Dialog */
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
  D.Callback = AudioConvertCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = AudioConvertResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = AudioConvertWaitCallBack;  /*  Wait callback */
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
//  ModifyAudioConvertGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunAudioConvert(void *arg) {
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    RadioButtons1  1 data value
    RadioButtons2  1 data value

*************************************************/
   char  v0[500]="" ;
   char  v1[500]="" ;
   int   v2 = 1;
   int   v3 = 1;
   void* v[4];
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   void *pt=NULL; /* pointer to send any extra information */
   AudioConvert(NULL,v,pt );
   return NULL;
}
