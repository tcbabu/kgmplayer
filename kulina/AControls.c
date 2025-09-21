#include <kulina.h>
#include "ControlsCallbacks.h"
extern int AllTracks;
extern int Volume;
extern float CurPerc;
extern int Speed,Mute,Play;
extern int FilePlay;
extern FMGIMG next_str,cancel_str,close_str,pause_str,play_str,
              forward_str,muted_str,volume_str,quit_str;
int AControlsGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  BUT_STR  *butn0=NULL; 
  butn0= (BUT_STR *)malloc(sizeof(BUT_STR)*5);
  butn0[0].sw=1;
  strcpy(butn0[0].title,(char *)"");
//  butn0[0].xpmn=(char *)(&quit_str);
  butn0[0].xpmn=(char *)(&cancel_str);
  butn0[0].xpmp=NULL;
  butn0[0].xpmh=NULL;
  butn0[0].bkgr=-100120100;
  butn0[0].butncode='';
  butn0[1].sw=1;
  strcpy(butn0[1].title,(char *)"");
  if(Play)butn0[1].xpmn=(char *)(&pause_str);
  else butn0[1].xpmn=(char *)(&play_str);
  butn0[1].xpmp=NULL;
  butn0[1].xpmh=NULL;
  butn0[1].bkgr=-100120100;
  butn0[1].butncode='';
  butn0[2].sw=1;
  strcpy(butn0[2].title,(char *)"");
  butn0[2].xpmn=(char *)(&forward_str);
  butn0[2].xpmp=NULL;
  butn0[2].xpmh=NULL;
  butn0[2].bkgr=-100120100;
  butn0[2].butncode='';
  butn0[3].sw=1;
  strcpy(butn0[3].title,(char *)"");
  if(!Mute)butn0[3].xpmn=(char *)(&muted_str);
  else butn0[3].xpmn=(char *)(&volume_str);
  butn0[3].xpmp=NULL;
  butn0[3].xpmh=NULL;
  butn0[3].bkgr=-100120100;
  butn0[3].butncode='M';
  butn0[4].sw=1;
  if(FilePlay== 1) butn0[4].sw=-1;
  if((FilePlay==0)&&(AllTracks==0)) butn0[4].sw=-1;
  strcpy(butn0[4].title,(char *)"");
  butn0[4].xpmn=NULL;
  butn0[4].xpmp=NULL;
  butn0[4].xpmh=NULL;
  butn0[4].xpmn=(char *)(&next_str);
  butn0[4].bkgr=-100120100;
  butn0[4].butncode='N';
  DIL h0 = { 
    'h',
    107,110,
    428,173,
    5,0,
    54,
    54,
    5,1,
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn0, 
    AControlssplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h0.Wid,(char *)"ControlsWidget1");
  DIHB d1 = { 
    'P',
    94,65,  
    455,81,   
    0,100,  
    341,  
    (int *)v[0],
    NULL,
    NULL,AControlsdslide1callback, /* *args, callback */
    0,0,2,-102235135, /* bordr,hide,type,color */
  };
  strcpy(d1.Wid,(char *)"ControlsWidget2");
  DIHB d2 = { 
    'P',
    94,35,  
    454,51,   
    0,100,  
    340,  
    (int *)v[1],
    NULL,
    NULL,AControlsdslide2callback, /* *args, callback */
    0,0,2,-102235135, /* bordr,hide,type,color */
  };
  strcpy(d2.Wid,(char *)"ControlsWidget3");
  DIM m3 = { 
    'm',
    29,60,  
    90,84,  
    1,0  
  };
  strncpy(m3.msg,(char *)"Volume",499);
  strcpy(m3.Wid,(char *)"ControlsWidget4");
  DIM m4 = { 
    'm',
    28,30,  
    90,54,  
    1,0  
  };
  strncpy(m4.msg,(char *)"Playing",499);
  strcpy(m4.Wid,(char *)"ControlsWidget5");
  DIM m5 = { 
    'm',
    128,180,  
    340,220,  
    -1,0  
  };
  strncpy(m5.msg,(char *)"Position",499);
  strcpy(m5.Wid,(char *)"TimePos");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+7));
  d =dtmp+i; 
  d[6].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIL));
  Controlssplbutton1init(&h0,pt) ;
  *d[0].h = h0;
  d[0].h->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIHB));
  *d[1].B = d1;
  d[1].B->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIHB));
  *d[2].B = d2;
  d[2].B->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIM));
  *d[3].m = m3;
  d[3].m->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIM));
  *d[4].m = m4;
  d[4].m->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIM));
  *d[5].m = m5;
  d[5].m->item = -1;
  d[6].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 
int AControls( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = AControlsGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 2;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 57;   /* Position of Dialog */ 
  D.yo = 3;
  D.xl = 573;    /*  Length of Dialog */
  D.yl = 274;    /*  Width  of Dialog */
  D.Initfun = AControlsinit;    /*   init fuction for Dialog */
  D.Cleanupfun = Controlscleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 0;    /*  1 for Window Decorration */
  D.transparency = 0.00000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 1;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = AControlsCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = ControlsResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = AControlsWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 1;    /*  1 for not showing in task bar*/
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
//  ModifyControlsGc(&(D.gc));    /*  set colors for gui*/
//  Print_gui_data(&D,"AControls.rc");
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunAControls(void *arg) {
/*************************************************

    Integerslidebar1  1 data value
    Integerslidebar2  1 data value

*************************************************/
   int ret=0;
   int   v0 = 1;
   int   v1 = 1;
   void* v[2];
   v0 = Volume;
   v1 = (int)CurPerc;
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   void *pt=NULL; /* pointer to send any extra information */
   ret = AControls(arg,v,pt );
//   printf("ret= %d\n",ret);
   return NULL;
}
