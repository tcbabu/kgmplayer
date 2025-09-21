#include <kulina.h>
#include "kgGetFilesCallbacks.h"
void ModifykgGetFilesGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
char *kgCopyString(char *str) {
  char *ostr=NULL;
  if(str != NULL) {
    ostr = (char *)malloc(strlen(str)+1);
    strcpy(ostr,str);
  }
  return ostr;
}
int kgGetFilesGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  BUT_STR  *butn0=NULL; 
  butn0= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn0[0].sw=1;
  strcpy(butn0[0].title,(char *)"");
  butn0[0].xpmn=kgUpdirImage(16,245,245,245);
  butn0[0].xpmp=kgUpdirImage(16,215,215,215);
  butn0[0].xpmh=NULL;
  butn0[0].bkgr=-1;
  butn0[0].butncode='';
  butn0[1].sw=1;
  strcpy(butn0[1].title,(char *)"");
  butn0[1].xpmn=kgHomeImage(16,245,245,245);
  butn0[1].xpmp=kgHomeImage(16,215,215,215);
  butn0[1].xpmh=NULL;
  butn0[1].bkgr=-1;
  butn0[1].butncode='';
  DIN b0 = { 
    'n',
    24,10,  
    84,44,
    2,2,  
    24, 
    24, 
    2,1, 
    4,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn0, 
    kgGetFilesbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b0.Wid,(char *)"kgGetFilesWidget2");
  b0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Add files");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"Select all");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode='';
  butn1[2].sw=1;
  strcpy(butn1[2].title,(char *)"Clear");
  butn1[2].xpmn=NULL;
  butn1[2].xpmp=NULL;
  butn1[2].xpmh=NULL;
  butn1[2].bkgr=-1;
  butn1[2].butncode='';
  butn1[3].sw=1;
  strcpy(butn1[3].title,(char *)"Finish");
  butn1[3].xpmn=NULL;
  butn1[3].xpmp=NULL;
  butn1[3].xpmh=NULL;
  butn1[3].bkgr=6;
  butn1[3].butncode='';

  DIL h1 = { 
    'h',
    721,14,
    988,46,
    2,0,  
    64, 
    24, 
    4,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    kgGetFilessplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h1.Wid,(char *)"kgGetFilesWidget7");
  h1.item = -1;
  DIY y2 = { 
    'y',
    413,52,  
    1003,556,   
    8,20,  
    92, 
    92, 
    1,0, 
    0,3, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,kgGetFilesbrowser1callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y2.Wid,(char *)"kgGetFilesImages");
  y2.item = -1;
  char **menu3 ; 
  menu3= NULL;
  DIE e3 = { 
    'e',
    13,53,  
    405,208,   
    7,  
    (int *)v[1],
    NULL,
    menu3 ,
    NULL,kgGetFilesbrowser2callback, /* *args, callback */
    20,6,22,1,1,1,0 
  };
  strcpy(e3.Wid,(char *)"kgGetFilesFolder");
  e3.item = -1;
  DIY y4 = { 
    'y',
    5,234,  
    403,556,   
    8,20,  
    92, 
    92, 
    1,0,
    0,4, 
    (int *)v[2], 
    NULL, 
    NULL, 
    NULL,kgGetFilesbrowser3callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y4.Wid,(char *)"kgGetFilesItems");
  y4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(char *)"Select all");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-1;
  butn5[0].butncode='_';
  butn5[1].sw=1;
  strcpy(butn5[1].title,(char *)"Clear");
  butn5[1].xpmn=NULL;
  butn5[1].xpmp=NULL;
  butn5[1].xpmh=NULL;
  butn5[1].bkgr=-1;
  butn5[1].butncode=' ';
  butn5[2].sw=1;
  strcpy(butn5[2].title,(char *)"Delete");
  butn5[2].xpmn=NULL;
  butn5[2].xpmp=NULL;
  butn5[2].xpmh=NULL;
  butn5[2].bkgr=-1;
  butn5[2].butncode=' ';
  DIN b5 = { 
    'n',
    9,205,  
    215,233,
    2,2,  
    64, 
    22, 
    3,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    kgGetFilesbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"kgGetFilesWidget12");
  b5.item = -1;
  DII i6 = { 
    'i',
    83,11,  
    525,40,  
    48,1,0   
  };
  strcpy(i6.Wid,(char *)"kgGetFilesInfo");
  i6.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+8));
  d =dtmp+i; 
  d[7].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIN));
  kgGetFilesbutton1init(&b0,pt) ;
  *d[0].N = b0;
  d[0].N->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIL));
  kgGetFilessplbutton1init(&h1,pt) ;
  *d[1].h = h1;
  d[1].h->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIY));
  kgGetFilesbrowser1init(&y2,pt) ;
  *d[2].y = y2;
  d[2].y->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIE));
  *d[3].e = e3;
  d[3].e->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIY));
  kgGetFilesbrowser3init(&y4,pt) ;
  *d[4].y = y4;
  d[4].y->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  kgGetFilesbutton2init(&b5,pt) ;
  *d[5].N = b5;
  d[5].N->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DII));
  *d[6].i = i6;
  d[6].i->item = -1;
  d[7].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakekgGetFilesGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    ThumbnailBrowser1  1 data value
    Scrollmenu2  1 data value
    ThumbnailBrowser3  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   int  *v2 ;
   v2 = (int *)malloc(sizeof(int));
   *v2 = 1;
   void** v=(void **)malloc(sizeof(void*)*4);
   v[3]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = kgGetFilesGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int kgGetFilesOld( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = kgGetFilesGroup(&D,v,pt);
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
  D.xl = 1012;    /*  Length of Dialog */
  D.yl = 566;    /*  Width  of Dialog */
  D.Initfun = kgGetFilesinit;    /*   init fuction for Dialog */
  D.Cleanupfun = kgGetFilescleanup;    /*   init fuction for Dialog */
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
  D.Callback = kgGetFilesCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = kgGetFilesResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = kgGetFilesWaitCallBack;  /*  Wait callback */
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
//  ModifykgGetFilesGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunkgGetFiles(void *arg,char *Filter) {
/*************************************************

    ThumbnailBrowser1  1 data value
    Scrollmenu2  1 data value
    ThumbnailBrowser3  1 data value

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   int   v2 = 1;
   void *rpt[2];
   void* v[3];
   void *pt=NULL;
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   v[2]=(void *)(&v2);
   pt = (void *)rpt;
   rpt[0]= (void *) Filter;
   rpt[1]=NULL;
   kgGetFilesOld(NULL,v,pt );
   return rpt[1];
}
