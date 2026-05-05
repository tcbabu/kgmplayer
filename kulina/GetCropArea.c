#include <kulina.h>
#include "GetCropAreaCallbacks.h"
#include "GclrGetCropArea.c"
int GetCropAreaGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  char *xpm0=   NULL; /* pixmap info */ 
  DIG g0 = { 
    'g',
    86,5,  
    561,407,  
    (void *)xpm0,
    0, /* bkgr colour */ 
    GetCropAreaGCAgboxinit, /* void *initgraph(int,void *)  */ 
    NULL,0,0 /* *data border hide*/
  };
  strcpy(g0.Wid,(char *)"GCAgbox");
  g0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn1[0].sw=0;
  strcpy(butn1[0].title,(char *)"Select Area");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-177186177;
  butn1[0].butncode=126;
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"Select Location");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-216226216;
  butn1[1].butncode=126;
  butn1[2].sw=1;
  strcpy(butn1[2].title,(char *)"Okay");
  butn1[2].xpmn=NULL;
  butn1[2].xpmp=NULL;
  butn1[2].xpmh=NULL;
  butn1[2].bkgr=-216226216;
  butn1[2].butncode=126;
  DIL h1 = { 
    'h',
    3,409,  
    379,442,
    2,5,  
    120, 
    24, 
    3,1, 
    5,0.500000,0,0,0,1,/* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/ 
    butn1, 
    GetCropAreaGCAbutncallback ,  /* *args, Callback */
    NULL  /* any args */
  };
  strcpy(h1.Wid,(char *)"GCAbutn");
  h1.item = -1;
  DII i2 = { 
    'i',
    379,410,  
    627,442,  
    26,1,0   
  };
  strcpy(i2.Wid,(char *)"GCAdsp");
  i2.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+4));
  d =dtmp+i; 
  d[3].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIG));
  *d[0].g = g0;
  d[0].g->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIL));
  *d[1].h = h1;
  d[1].h->item = -1;
  GetCropAreaGCAbutninit(d[1].h,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DII));
  *d[2].i = i2;
  d[2].i->item = -1;
  d[3].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeGetCropAreaGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   GrpId = GetCropAreaGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int GetCropArea( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = GetCropAreaGroup(&D,v,pt);
#else 
  GrpId = MakeGetCropAreaGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 834;   /* Position of Dialog */ 
  D.yo = 239;
  D.xl = 639;    /*  Length of Dialog */
  D.yl = 450;    /*  Width  of Dialog */
  D.Initfun = GetCropAreainit;    /*   init fuction for Dialog */
  D.Cleanupfun = GetCropAreacleanup;    /*   cleanup fuction for Dialog */
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
  D.Callback = GetCropAreaCallBack;    /*  default Callback  */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = GetCropAreaResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = GetCropAreaWaitCallBack;  /*  Wait callback */
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
  ModifyGetCropAreaGc(&D);    /*  set colors for gui if do not like default*/
  ModifyGetCropArea(&D,GrpId);    /*  add extras to  gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunGetCropArea(void *parent ,void *args) {
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt[2]={NULL,NULL}; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   pt[0]=args;
//   pt[1]=(void *)malloc(sizeof(int)*4);
   GetCropArea(parent,v,(void *)pt );
   int *Vals = (int *)pt[1];
   printf("Loc: %d:%d %d:%d\n",Vals[0],Vals[1],Vals[2],Vals[3]);
   return pt[1];
}
