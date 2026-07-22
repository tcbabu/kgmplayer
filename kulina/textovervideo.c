#include <kulina.h>
#include "textovervideoCallbacks.h"
#include "Gclrtextovervideo.c"
int textovervideoGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(11);
  strcpy(e0[0].fmt,(char *)"Video %29s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  e0[0].img=NULL;
  DIT t0 = { 
    't',
    9,9,  
    377,43,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,textovervideoTOVinput1callback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t0.Wid,(char *)"TOVinput1");
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
    385,8,  
    467,42,
    2,2,  
    72, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn1, 
    textovervideoTOVinput1browsecallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"TOVinput1browse");
  b1.item = -1;
  T_ELMT *e2  ; 
  e2 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e2[0].fmt = (char *)malloc(9);
  strcpy(e2[0].fmt,(char *)"Text%29s");
  e2[0].v=(void *)v[1];
  e2[0].sw=1;
  e2[0].noecho=0;
  e2[0].img=NULL;
  DIT t2 = { 
    't',
    26,49,  
    376,83,
    20, 
    1,1, 
    e2,
    1,1,
    NULL,textovervideoTOVinput2callback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t2.Wid,(char *)"TOVinput2");
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
    385,46,  
    467,80,
    2,2,  
    72, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn3, 
    textovervideoTOVinput2browsecallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b3.Wid,(char *)"TOVinput2browse");
  b3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"Process");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-1;
  butn4[0].butncode=-2302850;
  DIL h4 = { 
    'h',
    187,401,  
    318,432,
    2,0,  
    120, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn4, 
    textovervideoTOVgocallback ,  /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(h4.Wid,(char *)"TOVgo");
  h4.item = -1;
  DII i5 = { 
    'i',
    8,304,  
    483,388,  
    51,3,0   
  };
  strcpy(i5.Wid,(char *)"TOVIbox");
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
    8,91,  
    385,125,
    20, 
    1,1, 
    e6,
    1,1,
    NULL,textovervideoTOVoutcallback ,0 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t6.Wid,(char *)"TOVout");
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
  butn7[0].butncode=126;
  DIN b7 = { 
    'n',
    385,90,  
    466,123,
    2,2,  
    72, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn7, 
    textovervideoTOVOutbrowsecallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"TOVOutbrowse");
  b7.item = -1;
  T_ELMT *e8  ; 
  e8 =(T_ELMT *)malloc(sizeof(T_ELMT)*2);
  e8[0].fmt = (char *)malloc(17);
  strcpy(e8[0].fmt,(char *)"Text Box Xres%4d");
  e8[0].v=(void *)v[3];
  e8[0].sw=1;
  e8[0].noecho=0;
  e8[0].img=NULL;
  e8[1].fmt = (char *)malloc(17);
  strcpy(e8[1].fmt,(char *)"Text Box Yres%4d");
  e8[1].v=(void *)v[4];
  e8[1].sw=1;
  e8[1].noecho=0;
  e8[1].img=NULL;
  DIT t8 = { 
    't',
    33,150,  
    460,180,
    20, 
    2,1, 
    e8,
    1,1,
    NULL,textovervideoTOVtrescallback ,1 ,0,18,9 
  };
    /* *args,Callback,border,hide,font,fontsize */
  strcpy(t8.Wid,(char *)"TOVtres");
  t8.pt=NULL;
  t8.type = 0;
  t8.item = -1;
  char *menu9[]  = { 
    (char *)"No",
    (char *)"Yes",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r9 = { 
    'r',
    241,218,  
    459,249,   
    8,0,  
    90, 
    25, 
    1,2, 
    0,1, 
    (int *)v[5], 
    NULL, 
    NULL ,
    NULL,textovervideoTOVradiocallback , /* *args, Callback  */
    1,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /*=1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu9);
  r9.list=(void **)th0;
  strcpy(r9.Wid,(char *)"TOVradio");
  r9.item = -1;
  DIM m10 = { 
    'm',
    17,223,  
    217,247,  
    1,0  
  };
  strncpy(m10.msg,(char *)"Fill Background",499);
  strcpy(m10.Wid,(char *)"TOVmsg3");
  m10.item = -1;
  BUT_STR  *butn11=NULL; 
  butn11= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn11[0].sw=1;
  strcpy(butn11[0].title,(char *)"");
  butn11[0].xpmn=NULL;
  butn11[0].xpmp=NULL;
  butn11[0].xpmh=NULL;
  butn11[0].bkgr=-255255255;
  butn11[0].butncode=31;
  DIN b11 = { 
    'n',
    107,254,  
    237,288,
    2,2,  
    120, 
    24, 
    1,1, 
    1,0.150000,0,1,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn11, 
    textovervideoTOVbclrcallback , /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(b11.Wid,(char *)"TOVbclr");
  b11.item = -1;
  DIM m12 = { 
    'm',
    16,256,  
    107,285,  
    1,1  
  };
  strncpy(m12.msg,(char *)"Select Color",499);
  strcpy(m12.Wid,(char *)"TOVmsg4");
  m12.item = -1;
  char **menu13 ; 
  menu13= (char **)malloc(sizeof(char *)*12);
  menu13[11]=NULL;
  menu13[0]=(char *)malloc(4);
  strcpy(menu13[0],(char *)"0.0");
  menu13[1]=(char *)malloc(5);
  strcpy(menu13[1],(char *)"0.05");
  menu13[2]=(char *)malloc(5);
  strcpy(menu13[2],(char *)"0.10");
  menu13[3]=(char *)malloc(5);
  strcpy(menu13[3],(char *)"0.15");
  menu13[4]=(char *)malloc(5);
  strcpy(menu13[4],(char *)"0.20");
  menu13[5]=(char *)malloc(5);
  strcpy(menu13[5],(char *)"0.25");
  menu13[6]=(char *)malloc(4);
  strcpy(menu13[6],(char *)"0.3");
  menu13[7]=(char *)malloc(5);
  strcpy(menu13[7],(char *)"0.35");
  menu13[8]=(char *)malloc(5);
  strcpy(menu13[8],(char *)"0.40");
  menu13[9]=(char *)malloc(5);
  strcpy(menu13[9],(char *)"0.45");
  menu13[10]=(char *)malloc(4);
  strcpy(menu13[10],(char *)"0.5");
  char *prompt13 ; 
  prompt13=(char *)malloc(17);
  strcpy(prompt13,(char *)"Rounding factor ");
  DIW w13 = { 
    'w',
    237,257,  
    475,286,   
    4,  
    (int *)v[6],
    prompt13 ,
    menu13 ,
    NULL,textovervideoTOVrfactcallback , /* *args, Callback  */
    1 
  };
  strcpy(w13.Wid,(char *)"TOVrfact");
  w13.item = -1;
  char **menu14 ; 
  menu14= (char **)malloc(sizeof(char *)*12);
  menu14[11]=NULL;
  menu14[0]=(char *)malloc(7);
  strcpy(menu14[0],(char *)"Center");
  menu14[1]=(char *)malloc(14);
  strcpy(menu14[1],(char *)"Bottom Center");
  menu14[2]=(char *)malloc(11);
  strcpy(menu14[2],(char *)"Top Center");
  menu14[3]=(char *)malloc(12);
  strcpy(menu14[3],(char *)"Bottom left");
  menu14[4]=(char *)malloc(13);
  strcpy(menu14[4],(char *)"Bottom right");
  menu14[5]=(char *)malloc(12);
  strcpy(menu14[5],(char *)"Center left");
  menu14[6]=(char *)malloc(13);
  strcpy(menu14[6],(char *)"Center right");
  menu14[7]=(char *)malloc(9);
  strcpy(menu14[7],(char *)"Top left");
  menu14[8]=(char *)malloc(10);
  strcpy(menu14[8],(char *)"Top right");
  menu14[9]=(char *)malloc(11);
  strcpy(menu14[9],(char *)"Top middle");
  menu14[10]=(char *)malloc(14);
  strcpy(menu14[10],(char *)"Bottom middle");
  char *prompt14 ; 
  prompt14=(char *)malloc(19);
  strcpy(prompt14,(char *)"Text Box position ");
  DIW w14 = { 
    'w',
    91,184,  
    419,213,   
    4,  
    (int *)v[7],
    prompt14 ,
    menu14 ,
    NULL,textovervideoTOVposcallback , /* *args, Callback  */
    0 
  };
  strcpy(w14.Wid,(char *)"TOVpos");
  w14.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+16));
  d =dtmp+i; 
  d[15].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  textovervideoTOVinput1browseinit(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIT));
  *d[2].t = t2;
  d[2].t->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIN));
  *d[3].N = b3;
  d[3].N->item = -1;
  textovervideoTOVinput2browseinit(d[3].N,pt) ;
  d[4].t = (DIT *)malloc(sizeof(DIL));
  *d[4].h = h4;
  d[4].h->item = -1;
  textovervideoTOVgoinit(d[4].h,pt) ;
  d[5].t = (DIT *)malloc(sizeof(DII));
  *d[5].i = i5;
  d[5].i->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIT));
  *d[6].t = t6;
  d[6].t->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  *d[7].N = b7;
  d[7].N->item = -1;
  textovervideoTOVOutbrowseinit(d[7].N,pt) ;
  d[8].t = (DIT *)malloc(sizeof(DIT));
  *d[8].t = t8;
  d[8].t->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIRA));
  *d[9].r = r9;
  d[9].r->item = -1;
  textovervideoTOVradioinit(d[9].r,pt) ;
  d[10].t = (DIT *)malloc(sizeof(DIM));
  *d[10].m = m10;
  d[10].m->item = -1;
  d[11].t = (DIT *)malloc(sizeof(DIN));
  *d[11].N = b11;
  d[11].N->item = -1;
  textovervideoTOVbclrinit(d[11].N,pt) ;
  d[12].t = (DIT *)malloc(sizeof(DIM));
  *d[12].m = m12;
  d[12].m->item = -1;
  d[13].t = (DIT *)malloc(sizeof(DIW));
  *d[13].w = w13;
  d[13].w->item = -1;
  d[14].t = (DIT *)malloc(sizeof(DIW));
  *d[14].w = w14;
  d[14].w->item = -1;
  d[15].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MaketextovervideoGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    Text_Box3  1 data values
    Text_Box4  2 data values
    RadioButtons1  1 data value
    Browser2  1 data value
    Browser3  1 data value

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
   int  *v5 ;
   v5 = (int *)malloc(sizeof(int));
   *v5 = 1;
   int  *v6 ;
   v6 = (int *)malloc(sizeof(int));
   *v6 = 1;
   int  *v7 ;
   v7 = (int *)malloc(sizeof(int));
   *v7 = 1;
   void** v=(void **)malloc(sizeof(void*)*9);
   v[8]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   v[4]=(void *)(v4);
   v[5]=(void *)(v5);
   v[6]=(void *)(v6);
   v[7]=(void *)(v7);
   void *pt=NULL; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   GrpId = textovervideoGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   textovervideoSetup(D,Gpt->arg);
   return GrpId;
}

int textovervideo( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = textovervideoGroup(&D,v,pt);
#else 
  GrpId = MaketextovervideoGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 14;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 828;   /* Position of Dialog */ 
  D.yo = 148;
  D.xl = 490;    /*  Length of Dialog */
  D.yl = 435;    /*  Width  of Dialog */
  D.Initfun = textovervideoinit;    /*   init fuction for Dialog */
  D.Cleanupfun = textovervideocleanup;    /*   cleanup fuction for Dialog */
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
  D.Callback = textovervideoCallBack;    /*  default Callback  */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = textovervideoResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = textovervideoWaitCallBack;  /*  Wait callback */
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
  ModifytextovervideoGc(&D);    /*  set colors for gui if do not like default*/
  Modifytextovervideo(&D,GrpId);    /*  add extras to  gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *Runtextovervideo(void *parent ,void *args) {
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    Text_Box3  1 data values
    Text_Box4  2 data values
    RadioButtons1  1 data value
    Browser2  1 data value
    Browser3  1 data value

*************************************************/
   char  v0[500]="" ;
   char  v1[500]="" ;
   char  v2[500]="" ;
   int   v3 = 1;
   int   v4 = 1;
   int   v5 = 1;
   int   v6 = 1;
   int   v7 = 1;
   void* v[8];
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(&v3);
   v[4]=(void *)(&v4);
   v[5]=(void *)(&v5);
   v[6]=(void *)(&v6);
   v[7]=(void *)(&v7);
   void *pt[2]={NULL,NULL}; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   pt[0]=args;
   textovervideo(parent,v,(void *)pt );
   return pt[1];
}
