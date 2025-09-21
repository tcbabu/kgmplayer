#include <kulina.h>
#include "VideoJoinCallbacks.h"
char * MakeVjoinFile(void);
int VideoJoinGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIX x0 = { 
    'x',
    209,40,  
    579,140,   
    10,2,  
    352, 
    20, 
    1,1, 
    0,3, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,VideoJoinbrowser1callback, /* *args, callback */
    4,  /* Border Offset  */
     16,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(x0.Wid,(char *)"VideoList");
  x0.item = -1;
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
  strcpy(butn1[1].title,(char *)"Delete");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode='';
  DIN b1 = { 
    'n',
    435,8,  
    575,38,
    2,2,  
    64, 
    20, 
    2,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    VideoJoinbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"VideoJoinWidget2");
  b1.item = -1;
  DIM m2 = { 
    'm',
    208,14,  
    338,34,  
    -1,0  
  };
  strncpy(m2.msg,(char *)"Input Files(Video)",499);
  strcpy(m2.Wid,(char *)"VideoJoinWidget3");
  m2.item = -1;
  T_ELMT *e3  ; 
  e3 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e3[0].fmt = (char *)malloc(11);
  strcpy(e3[0].fmt,(char *)"Output%30s");
  e3[0].v=(void *)v[1];
  e3[0].sw=1;
  e3[0].noecho=0;
  DIT t3 = { 
    't',
    207,143,  
    584,177,
    20, 
    1,1, 
    e3,
    1,1,
    NULL,VideoJointextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t3.Wid,(char *)"VjoinOut");
  t3.pt=NULL;
  t3.type = 0;
  t3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"Join Videos");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-132206230;
  butn4[0].butncode='';
  DIL h4 = { 
    'h',
    468,199,
    574,232,
    2,0,  
    96, 
    25, 
    1,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    VideoJoinsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h4.Wid,(char *)"VideoJoinWidget11");
  h4.item = -1;
  char *menu5[]  = { 
    (char *)"Very Good",
    (char *)"Good",
    (char *)"Mediaum",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r5 = { 
    'r',
    279,174,  
    568,200,   
    8,0,  
    80, 
    25, 
    1,3, 
    0,1, 
    (int *)v[2], 
    NULL, 
    NULL ,
    NULL,VideoJoinbrowser2callback, /* *args, callback */
    0,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu5);
  r5.list=(void **)th0;
  strcpy(r5.Wid,(char *)"VJQuality");
  r5.item = -1;
  DIM m6 = { 
    'm',
    206,173,  
    278,198,  
    1,0  
  };
  strncpy(m6.msg,(char *)"Quality",499);
  strcpy(m6.Wid,(char *)"VideoJoinWidget8");
  m6.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+8));
  d =dtmp+i; 
  d[7].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIX));
  VideoJoinbrowser1init(&x0,pt) ;
  *d[0].x = x0;
  d[0].x->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  VideoJoinbutton1init(&b1,pt) ;
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIM));
  *d[2].m = m2;
  d[2].m->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIT));
  *d[3].t = t3;
  d[3].t->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIL));
  VideoJoinsplbutton1init(&h4,pt) ;
  *d[4].h = h4;
  d[4].h->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIRA));
  VideoJoinbrowser2init(&r5,pt) ;
  *d[5].r = r5;
  d[5].r->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIM));
  *d[6].m = m6;
  d[6].m->item = -1;
  d[7].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeVideoJoinGroup(DIALOG *D,void *arg) {
   int GrpId;
   char *pt;
   WIDGETGRP *Gpt;
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  1 data values
    RadioButtons2  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   char  *v1 ;
   v1 = (char *)malloc(sizeof(char)*500);
   pt = MakeVjoinFile();
   v1[0] = '\0';
   strcpy(v1,pt);
   free(pt);
   pt=NULL;

   int  *v2 ;
   v2 = (int *)malloc(sizeof(int));
   *v2 = 2;
   void** v=(void **)malloc(sizeof(void*)*4);
   v[3]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   GrpId = VideoJoinGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int VideoJoin( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = VideoJoinGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 5;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 587;    /*  Length of Dialog */
  D.yl = 256;    /*  Width  of Dialog */
  D.Initfun = VideoJoininit;    /*   init fuction for Dialog */
  D.Cleanupfun = VideoJoincleanup;    /*   init fuction for Dialog */
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
  D.Callback = VideoJoinCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = VideoJoinResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = VideoJoinWaitCallBack;  /*  Wait callback */
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
//  ModifyVideoJoinGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunVideoJoin(void *arg) {
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  1 data values
    RadioButtons2  1 data value

*************************************************/
   int   v0 = 1;
   char  v1[500]="" ;
   int   v2 = 1;
   void* v[3];
   v[0]=(void *)(&v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(&v2);
   void *pt=NULL; /* pointer to send any extra information */
   VideoJoin(NULL,v,pt );
   return NULL;
}
