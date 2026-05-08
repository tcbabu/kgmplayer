#include <kulina.h>
#include "ConvertVideoCallbacks.h"
#include "GclrConvertVideo.c"
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
    76,28,  
    399,63,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,ConvertVideoVCinboxcallback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t0.Wid,(char *)"VCinbox");
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
  butn1[0].butncode=31;
  DIN b1 = { 
    'n',
    402,27,  
    486,61,
    2,2,  
    64, 
    24, 
    1,1, 
    5,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn1, 
    ConvertVideoVCinbrcallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"VCinbr");
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
    24,69,  
    401,103,
    20, 
    1,1, 
    e2,
    1,1,
    NULL,ConvertVideoVCoutboxcallback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t2.Wid,(char *)"VCoutbox");
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
    105,122,  
    480,159,   
    8,0,  
    95, 
    25, 
    1,3, 
    0,1, 
    (int *)v[2], 
    NULL, 
    NULL ,
    NULL,ConvertVideoVCradio1callback , /* *args, Callback  */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    0  /*=1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu3);
  r3.list=(void **)th0;
  strcpy(r3.Wid,(char *)"VCradio1");
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
    150,172,  
    442,209,   
    8,0,  
    95, 
    25, 
    1,2, 
    0,1, 
    (int *)v[3], 
    NULL, 
    NULL ,
    NULL,ConvertVideoVCradio2callback , /* *args, Callback  */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    0  /*=1 hide  */
   };
  th1 = (ThumbNail **)kgStringToThumbNails((char **)menu5);
  r5.list=(void **)th1;
  strcpy(r5.Wid,(char *)"VCradio2");
  r5.item = -1;
  DIM m6 = { 
    'm',
    50,178,  
    150,202,  
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
    150,242,  
    434,279,   
    8,0,  
    95, 
    25, 
    1,2, 
    0,1, 
    (int *)v[4], 
    NULL, 
    NULL ,
    NULL,ConvertVideoVCradio3callback , /* *args, Callback  */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    0  /*=1 hide  */
   };
  th2 = (ThumbNail **)kgStringToThumbNails((char **)menu7);
  r7.list=(void **)th2;
  strcpy(r7.Wid,(char *)"VCradio3");
  r7.item = -1;
  DIM m8 = { 
    'm',
    50,249,  
    150,273,  
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
  butn9[0].butncode=31;
  DIL h9 = { 
    'h',
    210,408,  
    310,441,
    2,0,  
    90, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn9, 
    ConvertVideoVconvertcallback ,  /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(h9.Wid,(char *)"Vconvert");
  h9.item = -1;
  char *menu10[]  = { 
    (char *)"!w32!f23Full",
    (char *)"!w32!f23Select",
    NULL 
  };
  ThumbNail **th3 ;
  DIRA r10 = { 
    'r',
    150,311,  
    396,348,   
    8,0,  
    95, 
    25, 
    1,2, 
    0,1, 
    (int *)v[5], 
    NULL, 
    NULL ,
    NULL,ConvertVideoVCradio4callback , /* *args, Callback  */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    0  /*=1 hide  */
   };
  th3 = (ThumbNail **)kgStringToThumbNails((char **)menu10);
  r10.list=(void **)th3;
  strcpy(r10.Wid,(char *)"VCradio4");
  r10.item = -1;
  DIM m11 = { 
    'm',
    50,316,  
    150,340,  
    1,0  
  };
  strncpy(m11.msg,(char *)"!w32!f23Range",499);
  strcpy(m11.Wid,(char *)"ConvertVideoWidget12");
  m11.item = -1;
  DIM m12 = { 
    'm',
    106,354,  
    435,368,  
    0,0  
  };
  strncpy(m12.msg,(char *)"!w32!f21!c01(Note: Play and Stop to detemine range in secs)",499);
  strcpy(m12.Wid,(char *)"ConvertMsg");
  m12.item = -1;
  BUT_STR  *butn13=NULL; 
  butn13= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn13[0].sw=1;
  strcpy(butn13[0].title,(char *)"Browse");
  butn13[0].xpmn=NULL;
  butn13[0].xpmp=NULL;
  butn13[0].xpmh=NULL;
  butn13[0].bkgr=-1;
  butn13[0].butncode=31;
  DIN b13 = { 
    'n',
    402,68,  
    476,102,
    2,2,  
    64, 
    24, 
    1,1, 
    5,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn13, 
    ConvertVideoVCoutbrcallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b13.Wid,(char *)"VCoutbr");
  b13.item = -1;
  T_ELMT *e14  ; 
  e14 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e14[0].fmt = (char *)malloc(17);
  strcpy(e14[0].fmt,(char *)"Aspect Value%10F");
  e14[0].v=(void *)v[6];
  e14[0].sw=1;
  e14[0].noecho=0;
  e14[0].img=NULL;
  DIT t14 = { 
    't',
    190,205,  
    441,239,
    20, 
    1,1, 
    e14,
    1,1,
    NULL,ConvertVideoCVaspcallback ,0 ,1,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t14.Wid,(char *)"CVasp");
  t14.pt=NULL;
  t14.type = 1;
  t14.item = -1;
  t14.hide = 1;
  T_ELMT *e15  ; 
  e15 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e15[0].fmt = (char *)malloc(10);
  strcpy(e15[0].fmt,(char *)"Xreso%10d");
  e15[0].v=(void *)v[7];
  e15[0].sw=1;
  e15[0].noecho=0;
  e15[0].img=NULL;
  DIT t15 = { 
    't',
    247,275,  
    435,309,
    20, 
    1,1, 
    e15,
    1,1,
    NULL,ConvertVideoCVxrescallback ,0 ,1,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t15.Wid,(char *)"CVxres");
  t15.pt=NULL;
  t15.type = 1;
  t15.item = -1;
  t15.hide = 1;
  T_ELMT *e16  ; 
  e16 =(T_ELMT *)malloc(sizeof(T_ELMT)*2);
  e16[0].fmt = (char *)malloc(10);
  strcpy(e16[0].fmt,(char *)"Start%10F");
  e16[0].v=(void *)v[8];
  e16[0].sw=1;
  e16[0].noecho=0;
  e16[0].img=NULL;
  e16[1].fmt = (char *)malloc(8);
  strcpy(e16[1].fmt,(char *)"End%10F");
  e16[1].v=(void *)v[9];
  e16[1].sw=1;
  e16[1].noecho=0;
  e16[1].img=NULL;
  DIT t16 = { 
    't',
    80,370,  
    434,404,
    20, 
    2,1, 
    e16,
    1,1,
    NULL,ConvertVideoCVrangecallback ,0 ,1,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t16.Wid,(char *)"CVrange");
  t16.pt=NULL;
  t16.type = 1;
  t16.item = -1;
  t16.hide = 1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+18));
  d =dtmp+i; 
  d[17].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  ConvertVideoVCinbrinit(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIT));
  *d[2].t = t2;
  d[2].t->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIRA));
  *d[3].r = r3;
  d[3].r->item = -1;
  ConvertVideoVCradio1init(d[3].r,pt) ;
  d[4].t = (DIT *)malloc(sizeof(DIM));
  *d[4].m = m4;
  d[4].m->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIRA));
  *d[5].r = r5;
  d[5].r->item = -1;
  ConvertVideoVCradio2init(d[5].r,pt) ;
  d[6].t = (DIT *)malloc(sizeof(DIM));
  *d[6].m = m6;
  d[6].m->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIRA));
  *d[7].r = r7;
  d[7].r->item = -1;
  ConvertVideoVCradio3init(d[7].r,pt) ;
  d[8].t = (DIT *)malloc(sizeof(DIM));
  *d[8].m = m8;
  d[8].m->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIL));
  *d[9].h = h9;
  d[9].h->item = -1;
  ConvertVideoVconvertinit(d[9].h,pt) ;
  d[10].t = (DIT *)malloc(sizeof(DIRA));
  *d[10].r = r10;
  d[10].r->item = -1;
  ConvertVideoVCradio4init(d[10].r,pt) ;
  d[11].t = (DIT *)malloc(sizeof(DIM));
  *d[11].m = m11;
  d[11].m->item = -1;
  d[12].t = (DIT *)malloc(sizeof(DIM));
  *d[12].m = m12;
  d[12].m->item = -1;
  d[13].t = (DIT *)malloc(sizeof(DIN));
  *d[13].N = b13;
  d[13].N->item = -1;
  ConvertVideoVCoutbrinit(d[13].N,pt) ;
  d[14].t = (DIT *)malloc(sizeof(DIT));
  *d[14].t = t14;
  d[14].t->item = -1;
  d[15].t = (DIT *)malloc(sizeof(DIT));
  *d[15].t = t15;
  d[15].t->item = -1;
  d[16].t = (DIT *)malloc(sizeof(DIT));
  *d[16].t = t16;
  d[16].t->item = -1;
  d[17].t = NULL;
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
    Text_Box3  1 data values
    Text_Box4  1 data values
    Text_Box5  2 data values

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
   *v4 = 0;
   int  *v5 ;
   v5 = (int *)malloc(sizeof(int));
   *v5 = 0;
   double *v6 ;
   v6 = (double *)malloc(sizeof(double));
   *v6 = 1.0;
   int  *v7 ;
   v7 = (int *)malloc(sizeof(int));
   *v7 = 1280;
   double *v8 ;
   v8 = (double *)malloc(sizeof(double));
   *v8 = 0.0;
   double *v9 ;
   v9 = (double *)malloc(sizeof(double));
   *v9 = 0.0;
   void** v=(void **)malloc(sizeof(void*)*11);
   v[10]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   v[4]=(void *)(v4);
   v[5]=(void *)(v5);
   v[6]=(void *)(v6);
   v[7]=(void *)(v7);
   v[8]=(void *)(v8);
   v[9]=(void *)(v9);
   void *pt=NULL; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   GrpId = ConvertVideoGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int ConvertVideo( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = ConvertVideoGroup(&D,v,pt);
#else 
  GrpId = MakeConvertVideoGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 13;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 1355;   /* Position of Dialog */ 
  D.yo = 387;
  D.xl = 490;    /*  Length of Dialog */
  D.yl = 450;    /*  Width  of Dialog */
  D.Initfun = ConvertVideoinit;    /*   init fuction for Dialog */
  D.Cleanupfun = ConvertVideocleanup;    /*   cleanup fuction for Dialog */
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
  D.Callback = ConvertVideoCallBack;    /*  default Callback  */
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
  ModifyConvertVideoGc(&D);    /*  set colors for gui if do not like default*/
  ModifyConvertVideo(&D,GrpId);    /*  add extras to  gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunConvertVideo(void *parent ,void *args) {
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    RadioButtons1  1 data value
    RadioButtons2  1 data value
    RadioButtons3  1 data value
    RadioButtons4  1 data value
    Text_Box3  1 data values
    Text_Box4  1 data values
    Text_Box5  2 data values

*************************************************/
   char  v0[500]="" ;
   char  v1[500]="" ;
   int   v2 = 1;
   int   v3 = 1;
   int   v4 = 1;
   int   v5 = 1;
   double v6 = 0.0;
   int   v7 = 1;
   double v8 = 0.0;
   double v9 = 0.0;
   void* v[10];
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   v[4]=(void *)(&v4);
   v[5]=(void *)(&v5);
   v[6]=(void *)(&v6);
   v[7]=(void *)(&v7);
   v[8]=(void *)(&v8);
   v[9]=(void *)(&v9);
   void *pt[2]={NULL,NULL}; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   pt[0]=args;
   ConvertVideo(parent,v,(void *)pt );
   return pt[1];
}
