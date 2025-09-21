#include <kulina.h>
#include "kgGetFolderCallbacks.h"
#include "GFimages.c"
void ModifykgGetFolderGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}

int kgGetFolderGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(11);
  strcpy(e0[0].fmt,(char *)"Folder%30s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  DIT t0 = { 
    't',
    7,13,  
    384,47,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,kgGetFoldertextbox1callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"kgGetFolderWidget2");
  t0.pt=NULL;
  t0.type = 0;
  t0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
#if 1
  butn1[0].xpmn= (char *)&up_str;
//  butn1[0].xpmn= (char *)malloc(100); 
//  strcpy(butn1[0].xpmn,(char *)"##/usr/share/icons/gnome/22x22/actions/up.png");

#else
  butn1[0].xpmn= kgGetProcessedImage("##/usr/share/icons/gnome/22x22/actions/up.png",32,0.8,0,0,0);
#endif
  butn1[0].bkgr=-1;
  butn1[0].butncode='';
  DIN b1 = { 
    'n',
    388,8,  
    430,50,
    2,2,  
    32, 
    32, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    kgGetFolderbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"kgGetFolderWidget4");
  b1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
#if 1
  butn2[0].xpmn= (char *)&gohome_str;
//  butn2[0].xpmn= (char *)malloc(100); 
//  strcpy(butn2[0].xpmn,(char *)"##/usr/share/icons/gnome/22x22/actions/gtk-home.png");
#else
  butn2[0].xpmn= kgGetProcessedImage("##/usr/share/icons/gnome/22x22/actions/gtk-home.png",32,0.8,0,0,0);
#endif
  butn2[0].bkgr=-1;
  butn2[0].butncode='';
  DIN b2 = { 
    'n',
    432,7,  
    474,49,
    2,2,  
    32, 
    32, 
    1,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    kgGetFolderbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b2.Wid,(char *)"kgGetFolderWidget5");
  b2.item = -1;
  DIX x3 = { 
    'x',
    5,51,  
    228,325,   
    2,0,  
    185, 
    20, 
    1,32, 
    0,9, 
    (int *)v[1], 
    NULL, 
    NULL, 
    NULL,kgGetFolderbrowser1callback, /* *args, callback */
    4,  /* Border Offset  */
     20,  /* Scroll width  */
     2,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(x3.Wid,(char *)"kgGetFolderWidget6");
  x3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
#if 1
  butn4[0].xpmn= (char *)&dialog_cancel_str;
//  butn4[0].xpmn= (char *)malloc(100); 
//  strcpy(butn4[0].xpmn,(char *)"##/usr/share/icons/CratOS_lion-icons/actions/scalable/application-exit.png");
#else
  butn4[0].xpmn= kgGetProcessedImage("##/usr/share/icons/CratOS_lion-icons/actions/scalable/application-exit.png",48,0.8,0,0,0);
#endif
  butn4[0].bkgr=-1;
  butn4[0].butncode='';
  butn4[1].sw=1;
  strcpy(butn4[1].title,(char *)"");
  butn4[1].xpmn=NULL;
  butn4[1].xpmp=NULL;
  butn4[1].xpmh=NULL;
#if 1
  butn4[1].xpmn= (char *)&dialog_ok_apply_str;
//  butn4[1].xpmn= (char *)malloc(100); 
//  strcpy(butn4[1].xpmn,(char *)"##/usr/share/icons/default.kde4/32x32/actions/mail-mark-notjunk.png");
#else
  butn4[1].xpmn= kgGetProcessedImage("##/usr/share/icons/default.kde4/32x32/actions/mail-mark-notjunk.png",48,0.8,0,0,0);
#endif
  butn4[1].bkgr=-1;
  butn4[1].butncode='';
  DIL h4 = { 
    'h',
    185,326,  
    295,379,
    2,0,  
    48, 
    48, 
    2,1, 
    2,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    kgGetFoldersplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h4.Wid,(char *)"kgGetFolderWidget7");
  h4.item = -1;
  DIS s5 = { 
    's',
    228,51,  
    492,325,   
    6,  
    NULL,
    NULL,
    NULL ,
    NULL,NULL, /* *args, callback */
    20,6,22,0,1,1,0
  };
  strcpy(s5.Wid,(char *)"kgGetFolderWidget8");
  s5.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+7));
  d =dtmp+i; 
  d[6].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  kgGetFolderbutton1init(&b1,pt) ;
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIN));
  kgGetFolderbutton2init(&b2,pt) ;
  *d[2].N = b2;
  d[2].N->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIX));
  kgGetFolderbrowser1init(&x3,pt) ;
  *d[3].x = x3;
  d[3].x->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIL));
  kgGetFoldersplbutton1init(&h4,pt) ;
  *d[4].h = h4;
  d[4].h->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIS));
  kgGetFolderbrowser2init(&s5,pt) ;
  *d[5].s = s5;
  d[5].s->item = -1;
  d[6].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 
/* One can also use the following code to add Widgets to an existing Dialog */

int MakekgGetFolderGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Selectmenu1  1 data value

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(char)*500);
   v0[0] = '\0';
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   void** v=(void **)malloc(sizeof(void*)*3);
   v[2]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = kgGetFolderGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int kgGetFolder( void *parent,char *Folder) {
  void *pt[2];
  void *v[3];
  int v1;
  int ret=1,GrpId,k;
  char HomeDir[500],CurDir[500];
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  strcpy(HomeDir,getenv("HOME"));
  if(Folder[0]=='\0') strcpy(Folder,HomeDir);
  if(Folder[0]==' ') strcpy(Folder,HomeDir);
  strcpy(CurDir,Folder);
  pt[0]=HomeDir;
  pt[1]=CurDir;
  v[0]=(void *)Folder;
  v[1]=&v1;
  v[2]=NULL;
  GrpId = kgGetFolderGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 4;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 100;   /* Position of Dialog */ 
  D.yo = 100;
  D.xl = 499;    /*  Length of Dialog */
  D.yl = 384;    /*  Width  of Dialog */
  D.Initfun = kgGetFolderinit;    /*   init fuction for Dialog */
  D.Cleanupfun = kgGetFoldercleanup;    /*   init fuction for Dialog */
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
  D.Callback = kgGetFolderCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = kgGetFolderResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = kgGetFolderWaitCallBack;  /*  Wait callback */
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
//  ModifykgGetFolderGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  ret--;
//  printf("ret= %d %s\n",ret,Folder);
  return ret;
}
