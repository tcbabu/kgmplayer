#include <kulina.h>
#include "mediainfo.h"
#include "ReorderImagesCallbacks.h"
void ModifyReorderImagesGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int MakeThumbNailImages(ThumbNail ** th,int ln,int wd){
	int i=0,ret=1;
	while(th[i]!= NULL) {
		printf("%s\n",th[i]->name);
		th[i]->img = kgThumbNailImage(kgGetImage(th[i]->name),ln,wd);
	        i++;
	}
	return ret;
}
int ClearThumbNailImages(ThumbNail ** th){
	int i=0,ret=1;
	while(th[i]!= NULL) {
		if(th[i]->img != NULL) {
			kgFreeImage(th[i]->img);
			th[i]->img = NULL;
		}
		i++;
	}
	return ret;
}
int ReorderImagesGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  BUT_STR  *butn0=NULL; 
  butn0= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn0[0].sw=1;
  strcpy(butn0[0].title,(char *)"MovetoEnd");
  butn0[0].xpmn=NULL;
  butn0[0].xpmp=NULL;
  butn0[0].xpmh=NULL;
  butn0[0].bkgr=-1;
  butn0[0].butncode='';
  butn0[1].sw=1;
  strcpy(butn0[1].title,(char *)"Clear");
  butn0[1].xpmn=NULL;
  butn0[1].xpmp=NULL;
  butn0[1].xpmh=NULL;
  butn0[1].bkgr=-1;
  butn0[1].butncode='';
  butn0[2].sw=1;
  strcpy(butn0[2].title,(char *)"Delete");
  butn0[2].xpmn=NULL;
  butn0[2].xpmp=NULL;
  butn0[2].xpmh=NULL;
  butn0[2].bkgr=-1;
  butn0[2].butncode='';
  butn0[3].sw=1;
  strcpy(butn0[3].title,(char *)"Finish");
  butn0[3].xpmn=NULL;
  butn0[3].xpmp=NULL;
  butn0[3].xpmh=NULL;
  butn0[3].bkgr=-1;
  butn0[3].butncode='';
  DIL h0 = { 
    'h',
    692,7,  
    996,40,
    2,0,  
    72, 
    25, 
    4,1, 
    2,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn0, 
    ReorderImagessplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h0.Wid,(char *)"ReorderImagesWidget1");
  h0.item = -1;
  DIY y1 = { 
    'y',
    6,41,  
    1094,737,   
    40,20,  
    256, 
    256, 
    1,25, 
    0,2, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,ReorderImagesbrowser1callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y1.Wid,(char *)"ImageBox");
  y1.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+3));
  d =dtmp+i; 
  d[2].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIL));
  *d[0].h = h0;
  d[0].h->item = -1;
  ReorderImagessplbutton1init(d[0].h,pt) ;
  d[1].t = (DIT *)malloc(sizeof(DIY));
  *d[1].y = y1;
  d[1].y->item = -1;
  ReorderImagesbrowser1init(d[1].y,pt) ;
  d[2].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeReorderImagesGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    ThumbnailBrowser1  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   void** v=(void **)malloc(sizeof(void*)*2);
   v[1]=NULL;
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = ReorderImagesGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int ReorderImages( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = ReorderImagesGroup(&D,v,pt);
#else 
  GrpId = MakeReorderImagesGroup(&D,pt); // can try this also
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
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 1101;    /*  Length of Dialog */
  D.yl = 745;    /*  Width  of Dialog */
  D.Initfun = ReorderImagesinit;    /*   init fuction for Dialog */
  D.Cleanupfun = ReorderImagescleanup;    /*   init fuction for Dialog */
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
  D.Callback = ReorderImagesCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = ReorderImagesResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = ReorderImagesWaitCallBack;  /*  Wait callback */
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
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyReorderImagesGc(&(D.gc));    /*  set colors for gui*/
  ModifyReorderImages(&D,GrpId);    /*  add extras to  gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunReorderImages(void *arg) {
/*************************************************

    ThumbnailBrowser1  1 data value

*************************************************/
   int   v0 = 1,i;
   void* v[1];
   char **names;
   Dlink *L;
   MEDIAINFO *mpt;
   v[0]=(void *)(&v0);

   void *pt[2]; /* pointer to send any extra information */
   if (arg == NULL) return NULL;
   L = (Dlink *)arg;
   names = (char **) malloc(sizeof(char *)*(Dcount(L)+1));
   Resetlink(L);
   i=0;
   while((mpt = (MEDIAINFO *)Getrecord(L)) != NULL) {
	   names[i] = mpt->Flname;
	   printf("%s\n",names[i]);fflush(stdout);
	   i++;
   }
   names[i]=NULL;

   pt[0]=names;
   pt[1]=NULL;
   ReorderImages(NULL,v,pt );
   free(names);
   return pt[0];
}
