#include <kulina.h>
#include "VrangeCallbacks.h"
#include "ConvertData.h"
extern CONVDATA cndata;
extern int AConGrp,RangeGrp,EnVoGrp,VConGrp,VaspGrp,VsizeGrp,VrangeGrp;

void ModifyVrangeGc(void *Tmp) {
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
   kgMkgclr("Vrange",Tmp);
*/
}
int VrangeGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(4);
  strcpy(e0[0].fmt,(char *)"%8F");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  e0[0].img=NULL;
  DIT t0 = { 
    't',
    162,41,  
    279,75,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,Vrangetextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"VRangeWidget1");
  t0.pt=NULL;
  t0.type = 0;
  t0.item = -1;
  T_ELMT *e1  ; 
  e1 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e1[0].fmt = (char *)malloc(4);
  strcpy(e1[0].fmt,(char *)"%8F");
  e1[0].v=(void *)v[1];
  e1[0].sw=1;
  e1[0].noecho=0;
  e1[0].img=NULL;
  DIT t1 = { 
    't',
    280,41,  
    397,75,
    20, 
    1,1, 
    e1,
    1,1,
    NULL,Vrangetextbox2callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t1.Wid,(char *)"VRangeWidget2");
  t1.pt=NULL;
  t1.type = 0;
  t1.item = -1;
  DIM m2 = { 
    'm',
    67,46,  
    167,70,  
    1,0  
  };
  strncpy(m2.msg,(char *)"!f21Start",499);
  strcpy(m2.Wid,(char *)"VRangeWidget3");
  m2.item = -1;
  DIM m3 = { 
    'm',
    398,45,  
    498,69,  
    -1,0  
  };
  strncpy(m3.msg,(char *)"!f21End",499);
  strcpy(m3.Wid,(char *)"VRangeWidget4");
  m3.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+5));
  d =dtmp+i; 
  d[4].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIT));
  *d[1].t = t1;
  d[1].t->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIM));
  *d[2].m = m2;
  d[2].m->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIM));
  *d[3].m = m3;
  d[3].m->item = -1;
  d[4].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 
/* One can also use the following code to add Widgets to an existing Dialog */

int MakeVrangeGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values

*************************************************/
   double *v0 ;
   v0 = (double *)malloc(sizeof(double));
   *v0 = cndata.VStartSec;
   double *v1 ;
   v1 = (double *)malloc(sizeof(double));
   *v1 = cndata.VEndSec;
   void** v=(void **)malloc(sizeof(void*)*3);
   v[2]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = VrangeGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int Vrange( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = VrangeGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 599;    /*  Length of Dialog */
  D.yl = 240;    /*  Width  of Dialog */
  D.Initfun = Vrangeinit;    /*   init fuction for Dialog */
  D.Cleanupfun = Vrangecleanup;    /*   init fuction for Dialog */
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
  D.Callback = VrangeCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = VrangeResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = VrangeWaitCallBack;  /*  Wait callback */
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
//  ModifyVrangeGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunVrange(void *arg) {
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values

*************************************************/
   double v0 = 0.0;
   double v1 = 0.0;
   void* v[2];
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   void *pt=NULL; /* pointer to send any extra information */
   Vrange(NULL,v,pt );
   return NULL;
}
