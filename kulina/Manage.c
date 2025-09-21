#include <kulina.h>
#include "ManageCallbacks.h"
#define _GNU_SOURCE
typedef struct _DevRec {
  char device[50];
  int hdmi;
  int Xres,Yres;
  int pos;
  Dlink *Resolist;
} DEVREC;
extern Dlink *Devlist;
char PrimDev[50],HdmiDev[50];
char **Plist=NULL,**Hlist=NULL;
int PReso,HReso,PVis=1,HVis=1,Hpos=0;
int ProcessXrandr(int pip0,int pip1,int Pid);
void ModifyManageGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int ManageGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  BUT_STR  *butn0=NULL; 
  butn0= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn0[0].sw=1;
  strcpy(butn0[0].title,(char *)"Go back");
  butn0[0].xpmn=NULL;
  butn0[0].xpmp=NULL;
  butn0[0].xpmh=NULL;
  butn0[0].bkgr=-1;
  butn0[0].butncode='';
  DIL h0 = { 
    'h',
    213,165,  
    295,198,
    2,0,  
    72, 
    25, 
    1,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn0, 
    Managesplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h0.Wid,(char *)"ManageWidget1");
  char **menu1 ; 
  menu1= Hlist;
  char *prompt1 ; 
  prompt1=(char *)malloc(16);
  strcpy(prompt1,(char *)"HDMI resolution");
  DIW w1 = { 
    'w',
    23,10,  
    239,39,   
    4,  
    (int *)v[0],
    prompt1 ,
    menu1 ,
    NULL,Managebrowser1callback, /* *args, callback */
    0 
  };
  strcpy(w1.Wid,(char *)"ManageWidget2");
  char *menu2[]  = { 
    (char *)"Clone",
    (char *)"Above",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r2 = { 
    'r',
    233,50,  
    430,82,   
    8,0,  
    80, 
    25, 
    1,2, 
    1,1, 
    (int *)v[1], 
    NULL, 
    NULL ,
    NULL,Managebrowser2callback, /* *args, callback */
    0,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu2);
  r2.list=(void **)th0;
  strcpy(r2.Wid,(char *)"ManageWidget3");
  DIM m3 = { 
    'm',
    24,50,  
    235,76,  
    1,0  
  };
  strncpy(m3.msg,(char *)"HDMI position relative to primary",499);
  strcpy(m3.Wid,(char *)"ManageWidget4");
  char *menu4[]  = { 
    (char *)"On",
    (char *)"Off",
    NULL 
  };
  ThumbNail **th1 ;
  DIRA r4 = { 
    'r',
    312,95,  
    471,122,   
    8,0,  
    70, 
    25, 
    1,2, 
    1,1, 
    (int *)v[2], 
    NULL, 
    NULL ,
    NULL,Managebrowser3callback, /* *args, callback */
    0,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th1 = (ThumbNail **)kgStringToThumbNails((char **)menu4);
  r4.list=(void **)th1;
  strcpy(r4.Wid,(char *)"ManageWidget5");
  DIM m5 = { 
    'm',
    210,96,  
    310,120,  
    1,0  
  };
  strncpy(m5.msg,(char *)"Primary Display",499);
  strcpy(m5.Wid,(char *)"ManageWidget6");
  char **menu6 ; 
  menu6= Plist;
  char *prompt6 ; 
  prompt6=(char *)malloc(19);
  strcpy(prompt6,(char *)"Primary Resolution");
  DIW w6 = { 
    'w',
    251,10,  
    475,39,   
    4,  
    (int *)v[3],
    prompt6 ,
    menu6 ,
    NULL,Managebrowser4callback, /* *args, callback */
    0 
  };
  strcpy(w6.Wid,(char *)"ManageWidget7");
  char *menu7[]  = { 
    (char *)"On",
    (char *)"Off",
    NULL 
  };
  ThumbNail **th2 ;
  DIRA r7 = { 
    'r',
    312,123,  
    471,151,   
    8,0,  
    70, 
    25, 
    1,2, 
    1,1, 
    (int *)v[4], 
    NULL, 
    NULL ,
    NULL,Managebrowser5callback, /* *args, callback */
    0,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th2 = (ThumbNail **)kgStringToThumbNails((char **)menu7);
  r7.list=(void **)th2;
  strcpy(r7.Wid,(char *)"ManageWidget8");
  DIM m8 = { 
    'm',
    210,124,  
    310,148,  
    1,0  
  };
  strncpy(m8.msg,(char *)"HDMI",499);
  strcpy(m8.Wid,(char *)"ManageWidget10");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+10));
  d =dtmp+i; 
  d[9].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIL));
  Managesplbutton1init(&h0,pt) ;
  *d[0].h = h0;
  d[0].h->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIW));
  *d[1].w = w1;
  d[1].w->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIRA));
  Managebrowser2init(&r2,pt) ;
  *d[2].r = r2;
  d[2].r->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIM));
  *d[3].m = m3;
  d[3].m->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIRA));
  Managebrowser3init(&r4,pt) ;
  *d[4].r = r4;
  d[4].r->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIM));
  *d[5].m = m5;
  d[5].m->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIW));
  *d[6].w = w6;
  d[6].w->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIRA));
  Managebrowser5init(&r7,pt) ;
  *d[7].r = r7;
  d[7].r->item = -1;
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

int MakeManageGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Browser1  1 data value
    RadioButtons2  1 data value
    RadioButtons3  1 data value
    Browser4  1 data value
    RadioButtons5  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   int  *v2 ;
   v2 = (int *)malloc(sizeof(int));
   *v2 = 1;
   int  *v3 ;
   v3 = (int *)malloc(sizeof(int));
   *v3 = 1;
   int  *v4 ;
   v4 = (int *)malloc(sizeof(int));
   *v4 = 1;
   void** v=(void **)malloc(sizeof(void*)*6);
   v[5]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   v[4]=(void *)(v4);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = ManageGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int Manage( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = ManageGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 0;
  D.df = 7;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 55;   /* Position of Dialog */ 
  D.yo = 4;
  D.xl = 571;    /*  Length of Dialog */
  D.yl = 271;    /*  Width  of Dialog */
  D.Initfun = Manageinit;    /*   init fuction for Dialog */
  D.Cleanupfun = Managecleanup;    /*   init fuction for Dialog */
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
  D.Callback = ManageCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = ManageResizeCallBack;  /*  Resize callback */
#if 1 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = ManageWaitCallBack;  /*  Wait callback */
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
//  ModifyManageGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
int CurrDevInfo(void) {
  int i,xr,yr,j,count;
  DEVREC *dpt;
  Dlink *Rlist;
  char *reso,buf[50];
  if(Plist != NULL) free(Plist);
  if(Hlist != NULL) free(Hlist);
  Plist=Hlist=NULL;
//  runjob("xrandr",ProcessXrandr);
  if(Devlist != NULL) {
    Resetlink(Devlist);
    while( (dpt=(DEVREC *)Getrecord(Devlist))!= NULL) {
      Rlist=dpt->Resolist;
      count=Dcount(Rlist);
      Resetlink(Rlist);
      if(dpt->hdmi==0) {
        if(Plist != NULL) continue;
        Plist = (char **)malloc(sizeof(char *)*(count+1));
        Plist[count]=NULL;
        strcpy(PrimDev,dpt->device);
        PReso=0;
        j=0;
        while((reso=(char *)Getrecord(Rlist))!= NULL) {
          strcpy(buf,reso);
          Plist[j]=(char *)malloc(strlen(reso)+1);
          strcpy(Plist[j],reso);
          i=0;
          while(buf[i]!='\0') {
           if(!isdigit(buf[i])) buf[i]=' ';
           i++;
          }
          sscanf(buf,"%d%d",&xr,&yr);
          if( (xr==dpt->Xres)&&(yr==dpt->Yres)) {PReso=j;}
          j++;
        }
      }
      else {
        if(Hlist != NULL) continue;
        Hlist = (char **)malloc(sizeof(char *)*(count+1));
        Hlist[count]=NULL;
        strcpy(HdmiDev,dpt->device);
        HReso=0;
        j=0;
        while((reso=(char *)Getrecord(Rlist))!= NULL) {
          strcpy(buf,reso);
          Hlist[j]=(char *)malloc(strlen(reso)+1);
          strcpy(Hlist[j],reso);
          i=0;
          while(buf[i]!='\0') {
           if(!isdigit(buf[i])) buf[i]=' ';
           i++;
          }
          sscanf(buf,"%d%d",&xr,&yr);
          if( (xr==dpt->Xres)&&(yr==dpt->Yres)) {HReso=j;}
          j++;
        }
      }
    }
  }
  return 1;
}
void *RunManage(void *arg) {
/*************************************************

    Browser1  1 data value
    RadioButtons2  1 data value
    RadioButtons3  1 data value
    Browser4  1 data value
    RadioButtons5  1 data value

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   int   v2 = 1;
   int   v3 = 1;
   int   v4 = 1;
   void* v[5];
   CurrDevInfo();
   v0=HReso+1;
   v3=PReso+1;
   if(Hpos==0) v1=1;
   else v1=2;
   if(PVis==1) v2=1;
   else v2=2;
   if(HVis==1) v4=1;
   else v4=2;
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   v[4]=(void *)(&v4);
   void *pt=NULL; /* pointer to send any extra information */
   Manage(arg,v,pt );
   Plist=Hlist=NULL;
   return NULL;
}
