#include "kulina.h"
#include "GetFileCallbacks.h"
#include "GclrGetFile.c"
int uiSetGetFileType(int type);

int GetFileGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  BUT_STR  *butn0=NULL; 
  butn0= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn0[0].sw=1;
  strcpy(butn0[0].title,(char *)"");
  butn0[0].xpmn=NULL;
  butn0[0].xpmp=NULL;
  butn0[0].xpmh=NULL;
  butn0[0].bkgr=-1;
  butn0[0].butncode='';
  butn0[1].sw=1;
  strcpy(butn0[1].title,(char *)"");
  butn0[1].xpmn=NULL;
  butn0[1].xpmp=NULL;
  butn0[1].xpmh=NULL;
  butn0[1].bkgr=-1;
  butn0[1].butncode='';
  DIN b0 = { 
    'n',
    11,6,  
    95,48,
    2,2,  
    36, 
    36, 
    2,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn0, 
    GetFilebutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b0.Wid,(char *)"FileListBrowserWidget2");
  b0.item = -1;
  DIX x1 = { 
    'x',
    7,56,  
    256,471,   
    10,2,  
    210, 
    25, 
    1,36, 
    0,12, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,GetFilebrowser1callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     2,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(x1.Wid,(char *)"Folders");
  x1.item = -1;
  DIY y2 = { 
    'y',
    256,56,  
    895,471,   
    100,40,  
    48, 
    48, 
    1,36, 
    0,4, 
    (int *)v[1], 
    NULL, 
    NULL, 
    NULL,GetFilebrowser2callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     1,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y2.Wid,(char *)"FileList");
  y2.item = -1;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"!c12!f23!w21Cancel");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-155155155;
  butn3[0].butncode='';
  butn3[1].sw=1;
  strcpy(butn3[1].title,(char *)"!c36!f23!w21Done");
  butn3[1].xpmn=NULL;
  butn3[1].xpmp=NULL;
  butn3[1].xpmh=NULL;
  butn3[1].bkgr=-255255255;
  butn3[1].butncode='';
  DIL h3 = { 
    'h',
    363,478,  
    540,509,
    2,0,  
    84, 
    24, 
    2,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    GetFilesplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h3.Wid,(char *)"FileListBrowserWidget11");
  h3.item = -1;
  DIM m4 = { 
    'm',
    499,20,  
    885,44,  
    -1,0  
  };
  strncpy(m4.msg,(char *)"",499);
  strcpy(m4.Wid,(char *)"MsgBox");
  m4.item = -1;
  DIM m5 = { 
    'B',
    97,18,  
    329,42,  
    -1,0  
  };
  strncpy(m5.msg,(char *)"",499);
  strcpy(m5.Wid,(char *)"FolderMsg");
  m5.item = -1;
  DIM m6 = { 
    'm',
    435,20,  
    495,44,  
    1,0  
  };
  strncpy(m6.msg,(char *)"!w32!f23File:",499);
  strcpy(m6.Wid,(char *)"GetFileWidget7");
  m6.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+8));
  d =dtmp+i; 
  d[7].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIN));
  *d[0].N = b0;
  d[0].N->item = -1;
  GetFilebutton1init(d[0].N,pt) ;
  d[1].t = (DIT *)malloc(sizeof(DIX));
  *d[1].x = x1;
  d[1].x->item = -1;
  GetFilebrowser1init(d[1].x,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIY));
  *d[2].y = y2;
  d[2].y->item = -1;
  GetFilebrowser2init(d[2].y,pt) ;
  d[3].t = (DIT *)malloc(sizeof(DIL));
  *d[3].h = h3;
  d[3].h->item = -1;
  GetFilesplbutton1init(d[3].h,pt) ;
  d[4].t = (DIT *)malloc(sizeof(DIM));
  *d[4].m = m4;
  d[4].m->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIM));
  *d[5].m = m5;
  d[5].m->item = -1;
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

int MakeGetFileGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Selectmenu1  1 data value
    ThumbnailBrowser2  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   void** v=(void **)malloc(sizeof(void*)*3);
   v[2]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = GetFileGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int GetFile( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = GetFileGroup(&D,v,pt);
#else 
  GrpId = MakeGetFileGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 3;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 7;
  D.xl = 906;    /*  Length of Dialog */
  D.yl = 520;    /*  Width  of Dialog */
  D.Initfun = GetFileinit;    /*   init fuction for Dialog */
  D.Cleanupfun = GetFilecleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
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
  D.Callback = GetFileCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = GetFileResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = GetFileWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.NoWinMngr = 0;    /*  1 for no Window Manager*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
      D.xo= (xres-D.xl)/2;
      D.yo= (yres-D.yl)/2; 
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
  ModifyGetFile(&D,GrpId);    /*  add extras to  gui*/
  ModifyGetFileGc(&D);    /*  set colors for gui if don't like default*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
char *kgGetFile(void *arg) {
/*************************************************

    Selectmenu1  1 data value
    ThumbnailBrowser2  1 data value

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   void* v[2];
   char *Str=NULL;
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   void *pt=(void *)(&Str);
   uiSetGetFileType(0);
   GetFile(NULL,v,pt );
   return Str;
}
char *kgGetMediaFile(void *arg) {
/*************************************************

    Selectmenu1  1 data value
    ThumbnailBrowser2  1 data value

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   void* v[2];
   char *Str=NULL;
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   void *pt=(void *)(&Str);
   uiSetGetFileType(1);
   GetFile(NULL,v,pt );
   return Str;
}
char *kgGetAudioFile(void *arg) {
/*************************************************

    Selectmenu1  1 data value
    ThumbnailBrowser2  1 data value

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   void* v[2];
   char *Str=NULL;
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   void *pt=(void *)(&Str);
   uiSetGetFileType(2);
   GetFile(NULL,v,pt );
   return Str;
}
char *kgGetVideoFile(void *arg) {
/*************************************************

    Selectmenu1  1 data value
    ThumbnailBrowser2  1 data value

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   void* v[2];
   char *Str=NULL;
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   void *pt=(void *)(&Str);
   uiSetGetFileType(3);
   GetFile(NULL,v,pt );
   return Str;
}
char *kgGetImageFile(void *arg) {
/*************************************************

    Selectmenu1  1 data value
    ThumbnailBrowser2  1 data value

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   void* v[2];
   char *Str=NULL;
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   void *pt=(void *)(&Str);
   uiSetGetFileType(4);
   GetFile(NULL,v,pt );
   return Str;
}
