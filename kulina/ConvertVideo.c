#include <kulina.h>
#include "ConvertVideoCallbacks.h"
#include "ConvertData.h"
extern CONVDATA cndata;
extern int AConGrp,RangeGrp,EnVoGrp,VConGrp,VaspGrp,VsizeGrp,VrangeGrp;

void ModifyConvertVideoGc(void *Tmp) {
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
   kgMkgclr("ConvertVideo",Tmp);
*/
}
int ConvertVideoGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(18);
  strcpy(e0[0].fmt,(char *)"!w32!f23Input%25s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  e0[0].img=NULL;
  DIT t0 = { 
    't',
    7,25,  
    398,59,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,ConvertVideotextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"VinputWidget");
  t0.pt=NULL;
  t0.type = 0;
  t0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"!w32!f23Browse");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='';
  DIN b1 = { 
    'n',
    397,25,  
    481,59,
    2,2,  
    74, 
    24, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    ConvertVideobutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"VButtonWidget");
  b1.item = -1;
  T_ELMT *e2  ; 
  e2 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e2[0].fmt = (char *)malloc(19);
  strcpy(e2[0].fmt,(char *)"!w32!f23Output%30s");
  e2[0].v=(void *)v[1];
  e2[0].sw=1;
  e2[0].noecho=0;
  e2[0].img=NULL;
  DIT t2 = { 
    't',
    17,68,  
    466,102,
    20, 
    1,1, 
    e2,
    1,1,
    NULL,ConvertVideotextbox2callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t2.Wid,(char *)"VOutputWidget");
  t2.pt=NULL;
  t2.type = 0;
  t2.item = -1;
  char *menu3[]  = { 
    (char *)"!w32!f23Very Good",
    (char *)"!w32!f23Good",
    (char *)"!w32!f23Low",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r3 = { 
    'r',
    107,122,  
    482,159,   
    8,0,  
    95, 
    25, 
    1,3, 
    0,1, 
    (int *)v[2], 
    NULL, 
    NULL ,
    NULL,ConvertVideobrowser1callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu3);
  r3.list=(void **)th0;
  strcpy(r3.Wid,(char *)"VRadioWidget");
  r3.item = -1;
  DIM m4 = { 
    'm',
    8,128,  
    108,152,  
    1,0  
  };
  strncpy(m4.msg,(char *)"!w32!f23Quality",499);
  strcpy(m4.Wid,(char *)"ConvertVideoWidget7");
  m4.item = -1;
  char *menu5[]  = { 
    (char *)"!w32!f23Original",
    (char *)"!w32!f23Change",
    NULL 
  };
  ThumbNail **th1 ;
  DIRA r5 = { 
    'r',
    146,172,  
    438,209,   
    8,0,  
    95, 
    25, 
    1,2, 
    0,1, 
    (int *)v[3], 
    NULL, 
    NULL ,
    NULL,ConvertVideobrowser2callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th1 = (ThumbNail **)kgStringToThumbNails((char **)menu5);
  r5.list=(void **)th1;
  strcpy(r5.Wid,(char *)"VRadioWidget2");
  r5.item = -1;
  DIM m6 = { 
    'm',
    48,178,  
    148,202,  
    1,0  
  };
  strncpy(m6.msg,(char *)"!w32!f23Aspect Ratio",499);
  strcpy(m6.Wid,(char *)"ConvertVideoWidget10");
  m6.item = -1;
  char *menu7[]  = { 
    (char *)"!w32!f23No",
    (char *)"!w32!f23Yes",
    NULL 
  };
  ThumbNail **th2 ;
  DIRA r7 = { 
    'r',
    146,218,  
    431,260,   
    8,0,  
    95, 
    25, 
    1,2, 
    0,1, 
    (int *)v[4], 
    NULL, 
    NULL ,
    NULL,ConvertVideobrowser3callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th2 = (ThumbNail **)kgStringToThumbNails((char **)menu7);
  r7.list=(void **)th2;
  strcpy(r7.Wid,(char *)"VRadioWidget3");
  r7.item = -1;
  DIM m8 = { 
    'm',
    51,227,  
    151,251,  
    1,0  
  };
  strncpy(m8.msg,(char *)"!w32!f23Scale",499);
  strcpy(m8.Wid,(char *)"ConvertVideoWidget9");
  m8.item = -1;
  BUT_STR  *butn9=NULL; 
  butn9= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn9[0].sw=1;
  strcpy(butn9[0].title,(char *)"!w32!f23Convert");
  butn9[0].xpmn=NULL;
  butn9[0].xpmp=NULL;
  butn9[0].xpmh=NULL;
  butn9[0].bkgr=-190250190;
  butn9[0].butncode='';
  DIL h9 = { 
    'h',
    210,408,  
    310,441,
    2,0,  
    90, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn9, 
    ConvertVideosplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h9.Wid,(char *)"ConvertVideoWidget10");
  h9.item = -1;
  char *menu10[]  = { 
    (char *)"!w32!f23Full",
    (char *)"!w32!f23Select",
    NULL 
  };
  ThumbNail **th3 ;
  DIRA r10 = { 
    'r',
    146,263,  
    396,308,   
    8,0,  
    95, 
    25, 
    1,2, 
    0,1, 
    (int *)v[5], 
    NULL, 
    NULL ,
    NULL,ConvertVideobrowser4callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th3 = (ThumbNail **)kgStringToThumbNails((char **)menu10);
  r10.list=(void **)th3;
  strcpy(r10.Wid,(char *)"ConvertVideoWidget11");
  r10.item = -1;
  DIM m11 = { 
    'm',
    52,272,  
    152,296,  
    1,0  
  };
  strncpy(m11.msg,(char *)"!w32!f23Range",499);
  strcpy(m11.Wid,(char *)"ConvertVideoWidget12");
  m11.item = -1;
  DIM m12 = { 
    'm',
    108,307,  
    437,321,  
    0,0  
  };
  strncpy(m12.msg,(char *)"!w32!f21!c01(Note: Play and Stop to detemine range in secs)",499);
  strcpy(m12.Wid,(char *)"ConvertMsg");
  m12.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+14));
  d =dtmp+i; 
  d[13].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  ConvertVideobutton1init(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIT));
  *d[2].t = t2;
  d[2].t->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIRA));
  *d[3].r = r3;
  d[3].r->item = -1;
  ConvertVideobrowser1init(d[3].r,pt) ;
  d[4].t = (DIT *)malloc(sizeof(DIM));
  *d[4].m = m4;
  d[4].m->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIRA));
  *d[5].r = r5;
  d[5].r->item = -1;
  ConvertVideobrowser2init(d[5].r,pt) ;
  d[6].t = (DIT *)malloc(sizeof(DIM));
  *d[6].m = m6;
  d[6].m->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIRA));
  *d[7].r = r7;
  d[7].r->item = -1;
  ConvertVideobrowser3init(d[7].r,pt) ;
  d[8].t = (DIT *)malloc(sizeof(DIM));
  *d[8].m = m8;
  d[8].m->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIL));
  *d[9].h = h9;
  d[9].h->item = -1;
  ConvertVideosplbutton1init(d[9].h,pt) ;
  d[10].t = (DIT *)malloc(sizeof(DIRA));
  *d[10].r = r10;
  d[10].r->item = -1;
  ConvertVideobrowser4init(d[10].r,pt) ;
  d[11].t = (DIT *)malloc(sizeof(DIM));
  *d[11].m = m11;
  d[11].m->item = -1;
  d[12].t = (DIT *)malloc(sizeof(DIM));
  *d[12].m = m12;
  d[12].m->item = -1;
  d[13].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeConvertVideoGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    RadioButtons1  1 data value
    RadioButtons2  1 data value
    RadioButtons3  1 data value
    RadioButtons4  1 data value

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
   int  *v4 ;
   v4 = (int *)malloc(sizeof(int));
   *v4 = 1;
   int  *v5 ;
   v5 = (int *)malloc(sizeof(int));
   *v5 = 1;
   void** v=(void **)malloc(sizeof(void*)*7);
   strcpy(v0,cndata.infile);
   strcpy(v1,cndata.infile);
   *v2= cndata.VQuality;
   *v3 =1;
   if(cndata.ChngAsp!=0) *v3=2;
   *v4 =1;
   if(cndata.Scale!=0) *v4=2;
   *v5 =1;
   if(cndata.VFullRange!=1) *v5=2;

   v[6]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   v[4]=(void *)(v4);
   v[5]=(void *)(v5);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = ConvertVideoGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int ConvertVideo( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = ConvertVideoGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 10;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 599;    /*  Length of Dialog */
  D.yl = 240;    /*  Width  of Dialog */
  D.Initfun = ConvertVideoinit;    /*   init fuction for Dialog */
  D.Cleanupfun = ConvertVideocleanup;    /*   init fuction for Dialog */
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
  D.Callback = ConvertVideoCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = ConvertVideoResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = ConvertVideoWaitCallBack;  /*  Wait callback */
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
//  ModifyConvertVideoGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunConvertVideo(void *arg) {
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    RadioButtons1  1 data value
    RadioButtons2  1 data value
    RadioButtons3  1 data value
    RadioButtons4  1 data value

*************************************************/
   char  v0[500]="" ;
   char  v1[500]="" ;
   int   v2 = 1;
   int   v3 = 1;
   int   v4 = 1;
   int   v5 = 1;
   void* v[6];
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   v[4]=(void *)(&v4);
   v[5]=(void *)(&v5);
   void *pt=NULL; /* pointer to send any extra information */
   ConvertVideo(NULL,v,pt );
   return NULL;
}
