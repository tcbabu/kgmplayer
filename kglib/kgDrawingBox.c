#include <kulina.h>
#include "kgDrawingBoxCallbacks.h"
#include "GclrkgDrawingBox.c"
int kgDrawingBoxGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  char *xpm0=   NULL; /* pixmap info */ 
  DIP p0 = { 
    'p',
    2,2,  
    115,188,  
    (void *)xpm0,
    -1, /* bkgr colour */ 
      2,0,0.000000 /* border hide transparency*/ 
  };
  strcpy(p0.Wid,(char *)"DTbkgr");
  p0.item = -1;
  char **menu1 ; 
  menu1= (char **)malloc(sizeof(char *)*17);
  menu1[16]=NULL;
  menu1[0]=(char *)malloc(5);
  strcpy(menu1[0],(char *)"Move");
  menu1[1]=(char *)malloc(5);
  strcpy(menu1[1],(char *)"Draw");
  menu1[2]=(char *)malloc(6);
  strcpy(menu1[2],(char *)"Arrow");
  menu1[3]=(char *)malloc(5);
  strcpy(menu1[3],(char *)"Rect");
  menu1[4]=(char *)malloc(5);
  strcpy(menu1[4],(char *)"Wipe");
  menu1[5]=(char *)malloc(9);
  strcpy(menu1[5],(char *)"Polyfill");
  menu1[6]=(char *)malloc(7);
  strcpy(menu1[6],(char *)"Rombus");
  menu1[7]=(char *)malloc(10);
  strcpy(menu1[7],(char *)"FixRadius");
  menu1[8]=(char *)malloc(4);
  strcpy(menu1[8],(char *)"Arc");
  menu1[9]=(char *)malloc(8);
  strcpy(menu1[9],(char *)"Arcfill");
  menu1[10]=(char *)malloc(5);
  strcpy(menu1[10],(char *)"Text");
  menu1[11]=(char *)malloc(7);
  strcpy(menu1[11],(char *)"Marker");
  menu1[12]=(char *)malloc(7);
  strcpy(menu1[12],(char *)"Border");
  menu1[13]=(char *)malloc(5);
  strcpy(menu1[13],(char *)"Undo");
  menu1[14]=(char *)malloc(7);
  strcpy(menu1[14],(char *)"Repeat");
  menu1[15]=(char *)malloc(11);
  strcpy(menu1[15],(char *)"Attributes");
  DIE e1 = { 
    'e',
    6,5,  
    111,185,   
    8,  
    (int *)v[0],
    NULL,
    menu1 ,
    NULL,kgDrawingBoxDTscrollcallback , /* *args, Callback  */
    20,6,22,1,1,1,0 
  };
  strcpy(e1.Wid,(char *)"DTscroll");
  e1.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+3));
  d =dtmp+i; 
  d[2].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIP));
  *d[0].p = p0;
  d[0].p->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIE));
  *d[1].e = e1;
  d[1].e->item = -1;
  d[2].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakekgDrawingBoxGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Scrollmenu1  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   void** v=(void **)malloc(sizeof(void*)*2);
   v[1]=NULL;
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   GrpId = kgDrawingBoxGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int kgDrawingBox( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = kgDrawingBoxGroup(&D,v,pt);
#else 
  GrpId = MakekgDrawingBoxGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 2;
  D.df = 1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 112;    /*  Length of Dialog */
  D.yl = 185;    /*  Width  of Dialog */
  D.Initfun = kgDrawingBoxinit;    /*   init fuction for Dialog */
  D.Cleanupfun = kgDrawingBoxcleanup;    /*   cleanup fuction for Dialog */
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
  D.Callback = kgDrawingBoxCallBack;    /*  default Callback  */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = kgDrawingBoxResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = kgDrawingBoxWaitCallBack;  /*  Wait callback */
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
  ModifykgDrawingBox(&D,GrpId);    /*  add extras to  gui*/
  ModifykgDrawingBoxGc(&D);    /*  set colors for gui if do not like default*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunkgDrawingBox(void *parent ,void *args) {
/*************************************************

    Scrollmenu1  1 data value

*************************************************/
   int   v0 = 1;
   void* v[1];
   v[0]=(void *)(&v0);
   void *pt[2]={NULL,NULL}; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   pt[0]=args;
   kgDrawingBox(parent,v,(void *)pt );
   return pt[1];
}
