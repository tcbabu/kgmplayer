#include "kulina.h"
#include "FileListBrowserCallbacks.h"
#include "GclrFileListBrowser.c"

#define R 120
#define G 140
#define B 120
#define D_NOCLEANCC
int uiSetFileType(int type);

int FileListBrowserGroup( DIALOG *D,void **v,void *pt) {
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
    11,5,  
    95,47,
    2,2,  
    36, 
    36, 
    2,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn0, 
    FileListBrowserbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b0.Wid,(char *)"FileListBrowserWidget2");
  b0.item = -1;
  T_ELMT *e1  ; 
  e1 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e1[0].fmt = (char *)malloc(6);
  strcpy(e1[0].fmt,(char *)" %25s");
  e1[0].v=(void *)v[0];
  e1[0].sw=1;
  e1[0].noecho=0;
  e1[0].img=NULL;
  DIT t1 = { 
    't',
    77,13,  
    369,41,
    20, 
    1,1, 
    e1,
    1,1,
    NULL,FileListBrowsertextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t1.Wid,(char *)"TextBox");
  t1.pt=NULL;
  t1.type = 0;
  t1.item = -1;
  DIX x2 = { 
    'x',
    4,46,  
    168,542,   
    10,2,  
    108, 
    25, 
    1,1, 
    0,12, 
    (int *)v[1], 
    NULL, 
    NULL, 
    NULL,FileListBrowserbrowser1callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     2,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(x2.Wid,(char *)"Folders");
  x2.item = -1;
  DIY y3 = { 
    'y',
    169,46,  
    851,542,   
    135,24,  
    24, 
    24, 
    1,1, 
    0,5, 
    (int *)v[2], 
    NULL, 
    NULL, 
    NULL,FileListBrowserbrowser2callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y3.Wid,(char *)"FileList");
  y3.item = -1;
  DIY y4 = { 
    'y',
    852,46,  
    1152,542,   
    270,24,  
    -4, 
    24, 
    1,1, 
    0,10, 
    (int *)v[3], 
    NULL, 
    NULL, 
    NULL,FileListBrowserbrowser3callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y4.Wid,(char *)"Selections");
  y4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(char *)"!c03!f23!w32Remove");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-130160130;
  butn5[0].butncode='';
  DIN b5 = { 
    'n',
    1058,17,  
    1149,48,
    2,2,  
    84, 
    24, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    FileListBrowserbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"FileListBrowserWidget10");
  b5.item = -1;
  BUT_STR  *butn6=NULL; 
  butn6= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn6[0].sw=1;
  strcpy(butn6[0].title,(char *)"!c36!f23!w21Done");
  butn6[0].xpmn=NULL;
  butn6[0].xpmp=NULL;
  butn6[0].xpmh=NULL;
  butn6[0].bkgr=-255255255;
  butn6[0].butncode='';
  DIL h6 = { 
    'h',
    756,18,  
    851,48,
    2,0,  
    84, 
    24, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn6, 
    FileListBrowsersplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h6.Wid,(char *)"FileListBrowserWidget11");
  h6.item = -1;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(char *)"!f23!w32Clear");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-1;
  butn7[0].butncode='';
  butn7[1].sw=1;
  strcpy(butn7[1].title,(char *)"!f23!w32Select all");
  butn7[1].xpmn=NULL;
  butn7[1].xpmp=NULL;
  butn7[1].xpmh=NULL;
  butn7[1].bkgr=-1;
  butn7[1].butncode='';
  butn7[2].sw=1;
  strcpy(butn7[2].title,(char *)"!f23!w32Add Files");
  butn7[2].xpmn=NULL;
  butn7[2].xpmp=NULL;
  butn7[2].xpmh=NULL;
  butn7[2].bkgr=-1;
  butn7[2].butncode='';
  DIN b7 = { 
    'n',
    403,17,  
    680,48,
    2,2,  
    82, 
    24, 
    3,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    FileListBrowserbutton3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"FileListBrowserWidget8");
  b7.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+9));
  d =dtmp+i; 
  d[8].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIN));
  *d[0].N = b0;
  d[0].N->item = -1;
  FileListBrowserbutton1init(d[0].N,pt) ;
  d[1].t = (DIT *)malloc(sizeof(DIT));
  *d[1].t = t1;
  d[1].t->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIX));
  *d[2].x = x2;
  d[2].x->item = -1;
  FileListBrowserbrowser1init(d[2].x,pt) ;
  d[3].t = (DIT *)malloc(sizeof(DIY));
  *d[3].y = y3;
  d[3].y->item = -1;
  FileListBrowserbrowser2init(d[3].y,pt) ;
  d[4].t = (DIT *)malloc(sizeof(DIY));
  *d[4].y = y4;
  d[4].y->item = -1;
  FileListBrowserbrowser3init(d[4].y,pt) ;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  *d[5].N = b5;
  d[5].N->item = -1;
  FileListBrowserbutton2init(d[5].N,pt) ;
  d[6].t = (DIT *)malloc(sizeof(DIL));
  *d[6].h = h6;
  d[6].h->item = -1;
  FileListBrowsersplbutton1init(d[6].h,pt) ;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  *d[7].N = b7;
  d[7].N->item = -1;
  FileListBrowserbutton3init(d[7].N,pt) ;
  d[8].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

#define D_NOCLEANCC
/* One can also use the following code to add Widgets to an existing Dialog */

int MakeFileListBrowserGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Selectmenu1  1 data value
    ThumbnailBrowser2  1 data value
    ThumbnailBrowser3  1 data value

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(char)*500);
   v0[0] = '\0';
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   int  *v2 ;
   v2 = (int *)malloc(sizeof(int));
   *v2 = 1;
   int  *v3 ;
   v3 = (int *)malloc(sizeof(int));
   *v3 = 1;
   void** v=(void **)malloc(sizeof(void*)*5);
   v[4]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = FileListBrowserGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int FileListBrowser( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = FileListBrowserGroup(&D,v,pt);
#else 
  GrpId = MakeFileListBrowserGroup(&D,pt); // can try this also
#endif 
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
  D.yo = 7;
  D.xl = 1155;    /*  Length of Dialog */
  D.yl = 546;    /*  Width  of Dialog */
  D.Initfun = FileListBrowserinit;    /*   init fuction for Dialog */
  D.Cleanupfun = FileListBrowsercleanup;    /*   init fuction for Dialog */
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
  D.Callback = FileListBrowserCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = FileListBrowserResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = FileListBrowserWaitCallBack;  /*  Wait callback */
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
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
  ModifyFileListBrowser(&D,GrpId);    /*  add extras to  gui*/
  ModifyFileListBrowserGc(&D);    /*  set colors for gui if don't like default*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
char **kgGetFiles(void *arg) {
/*************************************************

    Text_Box1  1 data values
    Selectmenu1  1 data value
    ThumbnailBrowser2  1 data value
    ThumbnailBrowser3  1 data value

*************************************************/
   char  v0[500]="" ;
   int   v1 = 1;
   int   v2 = 1;
   int   v3 = 1;
   void* v[4];
   char **Strs=NULL;
   int i=0;
   v[0]=(void *)(v0);
   v[1]=(void *)(&v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   uiSetFileType(0);
   void *pt=NULL; /* pointer to send any extra information */
   FileListBrowser(NULL,v,(void *)&Strs);
#if 0
   i=0;
   while(Strs[i] != NULL) {
	   printf("%d : %s\n",i,Strs[i]);
	   i++;
   }
#endif
   return Strs;
}
char **kgGetMediaFiles(void *arg) {
/*************************************************

    Text_Box1  1 data values
    Selectmenu1  1 data value
    ThumbnailBrowser2  1 data value
    ThumbnailBrowser3  1 data value

*************************************************/
   char  v0[500]="" ;
   int   v1 = 1;
   int   v2 = 1;
   int   v3 = 1;
   void* v[4];
   char **Strs=NULL;
   int i=0;
   v[0]=(void *)(v0);
   v[1]=(void *)(&v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   uiSetFileType(1);
   void *pt=NULL; /* pointer to send any extra information */
   FileListBrowser(NULL,v,(void *)&Strs);
#if 0
   i=0;
   while(Strs[i] != NULL) {
	   printf("%d : %s\n",i,Strs[i]);
	   i++;
   }
#endif
   return Strs;
}
