#include <kulina.h>
#include "MixAudioCallbacks.h"
extern int Tools;
char * MakeMixFile(void);
char * MakeAudioMixFile(void);

void ModifyMixAudioGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int MixAudioGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(16);
  strcpy(e0[0].fmt,(const char *)"Audio Media%20s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  DIT t0 = { 
    't',
    192,34,  
    519,65,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,MixAudiotextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(const char *)"MixTbox1");
  t0.pt=NULL;
  t0.type = 0;
  t0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(const char *)"Browse");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='';
  DIN b1 = { 
    'n',
    520,33,  
    594,67,
    2,2,  
    64, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    MixAudiobutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(const char *)"MixAudioWidget3");
  b1.item = -1;
  T_ELMT *e2  ; 
  e2 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e2[0].fmt = (char *)malloc(11);
  strcpy(e2[0].fmt,(const char *)"Output%30s");
  e2[0].v=(void *)v[1];
  e2[0].sw=1;
  e2[0].noecho=0;
  DIT t2 = { 
    't',
    200,119,  
    575,149,
    20, 
    1,1, 
    e2,
    1,1,
    NULL,MixAudiotextbox2callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t2.Wid,(const char *)"MixTbox2");
  t2.pt=NULL;
  t2.type = 0;
  t2.item = -1;
  char *menu3[]  = { 
    (char *)"Very Good",
    (char *)"Good",
    (char *)"Medium",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r3 = { 
    'r',
    272,158,  
    583,185,   
    8,0,  
    80, 
    25, 
    1,3, 
    -2302756,1, 
    (int *)v[2], 
    NULL, 
    NULL ,
    NULL,MixAudiobrowser1callback, /* *args, callback */
    1,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu3);
  r3.list=(void **)th0;
  strcpy(r3.Wid,(const char *)"MixQty");
  r3.item = -1;
  DIM m4 = { 
    'm',
    200,159,  
    272,183,  
    1,0  
  };
  strncpy(m4.msg,(char *)"Quality",499);
  strcpy(m4.Wid,(const char *)"MixAudioWidget7");
  m4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(const char *)"!w32!f23Mix Media");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-240250240;
  butn5[0].butncode='';
  DIL h5 = { 
    'h',
    472,202,  
    586,235,
    2,0,  
    104, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    MixAudiosplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h5.Wid,(const char *)"MixAudioWidget13");
  h5.item = -1;
  T_ELMT *e6  ; 
  e6 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e6[0].fmt = (char *)malloc(16);
  strcpy(e6[0].fmt,(const char *)"2nd   Media%20s");
  e6[0].v=(void *)v[3];
  e6[0].sw=1;
  e6[0].noecho=0;
  DIT t6 = { 
    't',
    191,75,  
    519,109,
    20, 
    1,1, 
    e6,
    1,1,
    NULL,MixAudiotextbox3callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t6.Wid,(const char *)"MixTbox3");
  t6.pt=NULL;
  t6.type = 0;
  t6.item = -1;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(const char *)"Browse");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-1;
  butn7[0].butncode='';
  DIN b7 = { 
    'n',
    520,74,  
    591,106,
    2,2,  
    64, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    MixAudiobutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b7.Wid,(const char *)"MixAudioWidget9");
  b7.item = -1;
  DIM m8 = { 
    'm',
    194,3,  
    594,27,  
    0,0  
  };
  strncpy(m8.msg,(char *)"Mixing audio from first to audio of second media(audio/video)",499);
  strcpy(m8.Wid,(const char *)"MixAudioWidget10");
  m8.item = -1;
  DIM m9 = { 
    'm',
    200,206,  
    468,233,  
    0,0  
  };
  strncpy(m9.msg,(char *)"Output format is MP4",499);
  strcpy(m9.Wid,(const char *)"MixAudioWidget12");
  m9.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+11));
  d =dtmp+i; 
  d[10].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  MixAudiobutton1init(&b1,pt) ;
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIT));
  *d[2].t = t2;
  d[2].t->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIRA));
  MixAudiobrowser1init(&r3,pt) ;
  *d[3].r = r3;
  d[3].r->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIM));
  *d[4].m = m4;
  d[4].m->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIL));
  MixAudiosplbutton1init(&h5,pt) ;
  *d[5].h = h5;
  d[5].h->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIT));
  *d[6].t = t6;
  d[6].t->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  MixAudiobutton2init(&b7,pt) ;
  *d[7].N = b7;
  d[7].N->item = -1;
  d[8].t = (DIT *)malloc(sizeof(DIM));
  *d[8].m = m8;
  d[8].m->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIM));
  *d[9].m = m9;
  d[9].m->item = -1;
  d[10].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 


int MakeMixAudioGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    RadioButtons1  1 data value
    Text_Box3  1 data values

*************************************************/
   void *pt=NULL; /* pointer to send any extra information */
   char  *v0 ;
   v0 = (char *)malloc(sizeof(char)*500);
   v0[0] = '\0';
   char  *v1 ;
   v1 = (char *)malloc(sizeof(char)*500);
   pt = MakeMixFile();
   v1[0] = '\0';
   strcpy(v1,(const char *)pt);
   free(pt);

   int  *v2 ;
   v2 = (int *)malloc(sizeof(int));
   *v2 = 2;
   char  *v3 ;
   v3 = (char *)malloc(sizeof(char)*500);
   v3[0] = '\0';
   void** v=(void **)malloc(sizeof(void*)*5);
   v[4]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   GrpId = MixAudioGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int AmixAudioGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(16);
  strcpy(e0[0].fmt,(char *)"Audio Media%20s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  e0[0].noecho=0;
  DIT t0 = { 
    't',
    192,34,  
    519,65,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,AmixAudiotextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"AmixTbox1");
  t0.pt=NULL;
  t0.type = 0;
  t0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Browse");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='';
  DIN b1 = { 
    'n',
    520,33,  
    594,67,
    2,2,  
    64, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    AmixAudiobutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"AmixAudioWidget3");
  b1.item = -1;
  T_ELMT *e2  ; 
  e2 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e2[0].fmt = (char *)malloc(11);
  strcpy(e2[0].fmt,(char *)"Output%30s");
  e2[0].v=(void *)v[1];
  e2[0].sw=1;
  e2[0].noecho=0;
  DIT t2 = { 
    't',
    200,119,  
    575,149,
    20, 
    1,1, 
    e2,
    1,1,
    NULL,AmixAudiotextbox2callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t2.Wid,(char *)"AmixTbox2");
  t2.pt=NULL;
  t2.type = 0;
  t2.item = -1;
  char *menu3[]  = { 
    (char *)"Very Good",
    (char *)"Good",
    (char *)"Medium",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r3 = { 
    'r',
    272,158,  
    583,185,   
    8,0,  
    80, 
    25, 
    1,3, 
    -2302756,1, 
    (int *)v[2], 
    NULL, 
    NULL ,
    NULL,AmixAudiobrowser1callback, /* *args, callback */
    1,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu3);
  r3.list=(void **)th0;
  strcpy(r3.Wid,(char *)"AmixQty");
  r3.item = -1;
  DIM m4 = { 
    'm',
    200,159,  
    272,183,  
    1,0  
  };
  strncpy(m4.msg,(char *)"Quality",499);
  strcpy(m4.Wid,(char *)"AmixAudioWidget7");
  m4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(char *)"!w32!f23Mix Audio");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-250255250;
  butn5[0].butncode='';
  DIL h5 = { 
    'h',
    472,202,  
    586,235,
    2,0,  
    104, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    AmixAudiosplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h5.Wid,(char *)"AmixAudioWidget13");
  h5.item = -1;
  T_ELMT *e6  ; 
  e6 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e6[0].fmt = (char *)malloc(16);
  strcpy(e6[0].fmt,(char *)"2nd   Media%20s");
  e6[0].v=(void *)v[3];
  e6[0].sw=1;
  e6[0].noecho=0;
  DIT t6 = { 
    't',
    191,75,  
    519,109,
    20, 
    1,1, 
    e6,
    1,1,
    NULL,AmixAudiotextbox3callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t6.Wid,(char *)"AmixTbox3");
  t6.pt=NULL;
  t6.type = 0;
  t6.item = -1;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(char *)"Browse");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-1;
  butn7[0].butncode='';
  DIN b7 = { 
    'n',
    520,74,  
    591,106,
    2,2,  
    64, 
    24, 
    1,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    AmixAudiobutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"AmixAudioWidget9");
  b7.item = -1;
  DIM m8 = { 
    'm',
    194,3,  
    594,27,  
    0,0  
  };
  strncpy(m8.msg,(char *)"Mixing audios of first and "
        "second to make audio file",499);
  strcpy(m8.Wid,(char *)"AmixAudioWidget10");
  m8.item = -1;
  DIM m9 = { 
    'm',
    200,206,  
    468,233,  
    0,0  
  };
  strncpy(m9.msg,(char *)"!c01Extension decides Output format",499);
  strcpy(m9.Wid,(char *)"AmixAudioWidget12");
  m9.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+11));
  d =dtmp+i; 
  d[10].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  AmixAudiobutton1init(&b1,pt) ;
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIT));
  *d[2].t = t2;
  d[2].t->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIRA));
  AmixAudiobrowser1init(&r3,pt) ;
  *d[3].r = r3;
  d[3].r->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIM));
  *d[4].m = m4;
  d[4].m->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIL));
  AmixAudiosplbutton1init(&h5,pt) ;
  *d[5].h = h5;
  d[5].h->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIT));
  *d[6].t = t6;
  d[6].t->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  AmixAudiobutton2init(&b7,pt) ;
  *d[7].N = b7;
  d[7].N->item = -1;
  d[8].t = (DIT *)malloc(sizeof(DIM));
  *d[8].m = m8;
  d[8].m->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIM));
  *d[9].m = m9;
  d[9].m->item = -1;
  d[10].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

int MakeAmixAudioGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    RadioButtons1  1 data value
    Text_Box3  1 data values

*************************************************/
   void *pt=NULL; /* pointer to send any extra information */
   char  *v0 ;
   v0 = (char *)malloc(sizeof(char)*500);
   v0[0] = '\0';
   char  *v1 ;
   v1 = (char *)malloc(sizeof(char)*500);
   pt = MakeAudioMixFile();
   v1[0] = '\0';
   strcpy(v1,(const char *)pt);
   free(pt);

   int  *v2 ;
   v2 = (int *)malloc(sizeof(int));
   *v2 = 2;
   char  *v3 ;
   v3 = (char *)malloc(sizeof(char)*500);
   v3[0] = '\0';
   void** v=(void **)malloc(sizeof(void*)*5);
   v[4]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   GrpId = AmixAudioGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int MixAudio( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = MixAudioGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 7;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 599;    /*  Length of Dialog */
  D.yl = 240;    /*  Width  of Dialog */
  D.Initfun = MixAudioinit;    /*   init fuction for Dialog */
  D.Cleanupfun = MixAudiocleanup;    /*   init fuction for Dialog */
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
  D.Callback = MixAudioCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = MixAudioResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = MixAudioWaitCallBack;  /*  Wait callback */
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
//  ModifyMixAudioGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunMixAudio(void *arg) {
/*************************************************

    Text_Box1  1 data values
    Text_Box2  1 data values
    RadioButtons1  1 data value
    Text_Box3  1 data values

*************************************************/
   char  v0[500]="" ;
   char  v1[500]="" ;
   int   v2 = 1;
   char  v3[500]="" ;
   void* v[4];
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(v3);
   void *pt=NULL; /* pointer to send any extra information */
   MixAudio(NULL,v,pt );
   return NULL;
}
