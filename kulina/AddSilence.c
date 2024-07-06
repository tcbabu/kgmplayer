#include <kulina.h>
#include "AddSilenceCallbacks.h"
char * MakeSilFile(void);

void ModifyAddSilenceGc(void *Tmp) {
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
   kgMkgclr("AddSilence",Tmp);
*/
}
int AddSilenceGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIX x0 = { 
    'x',
    32,113,  
    459,281,   
    10,2,  
    308, 
    25, 
    1,9812683, 
    -1077952577,5, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,AddSilencebrowser1callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(x0.Wid,(char *)"SilList");
  x0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"!w32!f23Add");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"!w32!f23Delete");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode='';
  DIN b1 = { 
    'n',
    301,79,  
    457,113,
    2,2,  
    72, 
    24, 
    2,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    AddSilencebutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"AddSilenceWidget2");
  b1.item = -1;
  DIM m2 = { 
    'm',
    33,81,  
    215,111,  
    -1,0  
  };
  strncpy(m2.msg,(char *)"!w32!f21Silence list",499);
  strcpy(m2.Wid,(char *)"AddSilenceWidget3");
  m2.item = -1;
  T_ELMT *e3  ; 
  e3 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e3[0].fmt = (char *)malloc(19);
  strcpy(e3[0].fmt,(char *)"!w32!f21Output%30s");
  e3[0].v=(void *)v[1];
  e3[0].sw=1;
  e3[0].noecho=0;
  e3[0].img=NULL;
  DIT t3 = { 
    't',
    19,289,  
    459,323,
    20, 
    1,1, 
    e3,
    1,1,
    NULL,AddSilencetextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t3.Wid,(char *)"SilOutput");
  t3.pt=NULL;
  t3.type = 0;
  t3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"!w32!f23Add Silences");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-235255250;
  butn4[0].butncode='';
  DIL h4 = { 
    'h',
    177,359,  
    293,393,
    2,0,  
    104, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    AddSilencesplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h4.Wid,(char *)"AddSilenceWidget11");
  h4.item = -1;
  T_ELMT *e5  ; 
  e5 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e5[0].fmt = (char *)malloc(5);
  strcpy(e5[0].fmt,(char *)"%20s");
  e5[0].v=(void *)v[2];
  e5[0].sw=1;
  e5[0].noecho=0;
  e5[0].img=NULL;
  DIT t5 = { 
    't',
    148,23,  
    373,57,
    20, 
    1,1, 
    e5,
    1,1,
    NULL,AddSilencetextbox2callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t5.Wid,(char *)"SilInput");
  t5.pt=NULL;
  t5.type = 0;
  t5.item = -1;
  BUT_STR  *butn6=NULL; 
  butn6= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn6[0].sw=1;
  strcpy(butn6[0].title,(char *)"!w32!f23Browse");
  butn6[0].xpmn=NULL;
  butn6[0].xpmp=NULL;
  butn6[0].xpmh=NULL;
  butn6[0].bkgr=-235255250;
  butn6[0].butncode='';
  DIN b6 = { 
    'n',
    374,22,  
    456,56,
    2,2,  
    72, 
    24, 
    1,1, 
    4,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn6, 
    AddSilencebutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b6.Wid,(char *)"AddSilenceWidget7");
  b6.item = -1;
  DIM m7 = { 
    'm',
    47,30,  
    147,54,  
    1,0  
  };
  strncpy(m7.msg,(char *)"!w32!f21Audio File",499);
  strcpy(m7.Wid,(char *)"AddSilenceWidget8");
  m7.item = -1;
  DIM m8 = { 
    'm',
    79,322,  
    460,340,  
    0,0  
  };
  strncpy(m8.msg,(char *)"!c01!f21Note:Extension decides output format",499);
  strcpy(m8.Wid,(char *)"AddSilenceWidget9");
  m8.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+10));
  d =dtmp+i; 
  d[9].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIX));
  *d[0].x = x0;
  d[0].x->item = -1;
  AddSilencebrowser1init(d[0].x,pt) ;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  AddSilencebutton1init(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIM));
  *d[2].m = m2;
  d[2].m->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIT));
  *d[3].t = t3;
  d[3].t->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIL));
  *d[4].h = h4;
  d[4].h->item = -1;
  AddSilencesplbutton1init(d[4].h,pt) ;
  d[5].t = (DIT *)malloc(sizeof(DIT));
  *d[5].t = t5;
  d[5].t->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIN));
  *d[6].N = b6;
  d[6].N->item = -1;
  AddSilencebutton2init(d[6].N,pt) ;
  d[7].t = (DIT *)malloc(sizeof(DIM));
  *d[7].m = m7;
  d[7].m->item = -1;
  d[8].t = (DIT *)malloc(sizeof(DIM));
  *d[8].m = m8;
  d[8].m->item = -1;
  d[9].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 
/* One can also use the following code to add Widgets to an existing Dialog */

int MakeAddSilenceGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  1 data values
    Text_Box2  1 data values

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   char  *v1 ;
   v1 = (char *)malloc(sizeof(char)*500);
   v1[0] = '\0';
   char  *v2 ;
   v2 = (char *)malloc(sizeof(char)*500);
   v2[0] = '\0';
   void** v=(void **)malloc(sizeof(void*)*4);
   v[3]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   void *pt=NULL; /* pointer to send any extra information */
   pt = MakeSilFile();
   strcpy(v1,(const char *)pt);
   GrpId = AddSilenceGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int AddSilence( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = AddSilenceGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 6;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 621;    /*  Length of Dialog */
  D.yl = 269;    /*  Width  of Dialog */
  D.Initfun = AddSilenceinit;    /*   init fuction for Dialog */
  D.Cleanupfun = AddSilencecleanup;    /*   init fuction for Dialog */
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
  D.Callback = AddSilenceCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = AddSilenceResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = AddSilenceWaitCallBack;  /*  Wait callback */
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
//  ModifyAddSilenceGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunAddSilence(void *arg) {
/*************************************************

    Selectmenu1  1 data value
    Text_Box1  1 data values
    Text_Box2  1 data values

*************************************************/
   int   v0 = 1;
   char  v1[500]="" ;
   char  v2[500]="" ;
   void* v[3];
   v[0]=(void *)(&v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   void *pt=NULL; /* pointer to send any extra information */
   AddSilence(NULL,v,pt );
   return NULL;
}
