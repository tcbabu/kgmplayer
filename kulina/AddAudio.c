#include <kulina.h>
#include "AddAudioCallbacks.h"
char * MakeAddFile(void);
void ModifyAddAudioGc(void *Tmp) {
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
   kgMkgclr("AddAudio",Tmp);
*/
}

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeAddAudioGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    RadioButtons1  1 data value
    Text_Box3  1 data values

*************************************************/
   char  *v0 ;
   char *pt=NULL;
   v0 = (char *)malloc(sizeof(char)*500);
   v0[0] = '\0';
   char  *v1 ;
   v1 = (char *)malloc(sizeof(char)*500);
   v1[0] = '\0';
   pt = MakeAddFile();
   v1[0] = '\0';
   strcpy(v1,pt);
   free(pt);
   int  *v2 ;
   v2 = (int *)malloc(sizeof(int));
   *v2 = 2;
   char  *v3 ;
   v3 = (char *)malloc(sizeof(char)*500);
   v3[0] = '\0';
   void** v=(void **)malloc(sizeof(void*)*5);
   v[4]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   GrpId = AddAudioGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int AddAudio( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = AddAudioGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 7;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 599;    /*  Length of Dialog */
  D.yl = 240;    /*  Width  of Dialog */
  D.Initfun = AddAudioinit;    /*   init fuction for Dialog */
  D.Cleanupfun = AddAudiocleanup;    /*   init fuction for Dialog */
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
  D.Callback = AddAudioCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = AddAudioResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = AddAudioWaitCallBack;  /*  Wait callback */
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
//  ModifyAddAudioGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
int AddAudioGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(5);
  strcpy(e0[0].fmt,(char *)"%20s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  e0[0].img=NULL;
  DIT t0 = { 
    't',
    150,43,  
    375,77,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,AddAudiotextbox1callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"AddTbox1");
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
  butn1[0].butncode=2147395871;
  DIN b1 = { 
    'n',
    376,42,  
    458,76,
    2,2,  
    72, 
    24, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    AddAudiobutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"AddButn1");
  b1.item = -1;
  T_ELMT *e2  ; 
  e2 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e2[0].fmt = (char *)malloc(5);
  strcpy(e2[0].fmt,(char *)"%30s");
  e2[0].v=(void *)v[1];
  e2[0].sw=1;
  e2[0].noecho=0;
  e2[0].img=NULL;
  DIT t2 = { 
    't',
    150,86,  
    465,120,
    20, 
    1,1, 
    e2,
    1,1,
    NULL,AddAudiotextbox2callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t2.Wid,(char *)"AddTbox2");
  t2.pt=NULL;
  t2.type = 0;
  t2.item = -1;
  char *menu3[]  = { 
    (char *)"Very good",
    (char *)"Good",
    (char *)"Medium",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r3 = { 
    'r',
    106,128,  
    478,158,   
    8,0,  
    110, 
    25, 
    1,3, 
    0,1, 
    (int *)v[2], 
    NULL, 
    NULL ,
    NULL,AddAudiobrowser1callback, /* *args, callback */
    2,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    0  /*=1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu3);
  r3.list=(void **)th0;
  strcpy(r3.Wid,(char *)"AddQty");
  r3.item = -1;
  DIM m4 = { 
    'm',
    6,128,  
    106,152,  
    1,0  
  };
  strncpy(m4.msg,(char *)"Quality",499);
  strcpy(m4.Wid,(char *)"AddAudioWidget11");
  m4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(char *)"AddAudio");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-235255250;
  butn5[0].butncode=31;
  DIL h5 = { 
    'h',
    213,252,  
    307,285,
    2,0,  
    92, 
    25, 
    1,1, 
    7,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    AddAudiosplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h5.Wid,(char *)"AddAudioWidget12");
  h5.item = -1;
  T_ELMT *e6  ; 
  e6 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e6[0].fmt = (char *)malloc(5);
  strcpy(e6[0].fmt,(char *)"%20s");
  e6[0].v=(void *)v[3];
  e6[0].sw=1;
  e6[0].noecho=0;
  e6[0].img=NULL;
  DIT t6 = { 
    't',
    178,174,  
    403,208,
    20, 
    1,1, 
    e6,
    1,1,
    NULL,AddAudiotextbox3callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t6.Wid,(char *)"AddTbox3");
  t6.pt=NULL;
  t6.type = 0;
  t6.item = -1;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(char *)"Browse");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-1;
  butn7[0].butncode=31;
  DIN b7 = { 
    'n',
    404,172,  
    486,206,
    2,2,  
    72, 
    24, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    AddAudiobutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"AddAudioWidget15");
  b7.item = -1;
  DIM m8 = { 
    'm',
    11,48,  
    151,72,  
    1,0  
  };
  strncpy(m8.msg,(char *)"Audio Media",499);
  strcpy(m8.Wid,(char *)"AddAudioWidget12");
  m8.item = -1;
  DIM m9 = { 
    'm',
    7,89,  
    150,113,  
    1,0  
  };
  strncpy(m9.msg,(char *)"Video Media",499);
  strcpy(m9.Wid,(char *)"AddAudioWidget14");
  m9.item = -1;
  DIM m10 = { 
    'm',
    18,179,  
    178,203,  
    1,0  
  };
  strncpy(m10.msg,(char *)"Output File",499);
  strcpy(m10.Wid,(char *)"AddAudioWidget15");
  m10.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+12));
  d =dtmp+i; 
  d[11].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  AddAudiobutton1init(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIT));
  *d[2].t = t2;
  d[2].t->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIRA));
  *d[3].r = r3;
  d[3].r->item = -1;
  AddAudiobrowser1init(d[3].r,pt) ;
  d[4].t = (DIT *)malloc(sizeof(DIM));
  *d[4].m = m4;
  d[4].m->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIL));
  *d[5].h = h5;
  d[5].h->item = -1;
  AddAudiosplbutton1init(d[5].h,pt) ;
  d[6].t = (DIT *)malloc(sizeof(DIT));
  *d[6].t = t6;
  d[6].t->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  *d[7].N = b7;
  d[7].N->item = -1;
  AddAudiobutton2init(d[7].N,pt) ;
  d[8].t = (DIT *)malloc(sizeof(DIM));
  *d[8].m = m8;
  d[8].m->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIM));
  *d[9].m = m9;
  d[9].m->item = -1;
  d[10].t = (DIT *)malloc(sizeof(DIM));
  *d[10].m = m10;
  d[10].m->item = -1;
  d[11].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 
void *RunAddAudio(void *arg) {
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    RadioButtons1  1 data value
    Text_Box3  1 data values

*************************************************/
   char  v0[500]="" ;
   char  v1[500]="" ;
   int   v2 = 1;
   char  v3[500]="" ;
   void* v[4];
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(v3);
   void *pt=NULL; /* pointer to send any extra information */
   AddAudio(NULL,v,pt );
   return NULL;
}
