#include <kulina.h>
#include "kgMplayerCallbacks.h"
//#define S1CLR -60220200
//#define S1CLR -69128181
#define S1CLR -1
//#define S2CLR -230200050
//#define S2CLR -69128181
#define S2CLR -1
char VER[]="Ver 5.2.0";
//int Thred=15,Thgreen=17,Thblue=15;
int Thred=165,Thgreen=177,Thblue=165;
char urlstring[500];
int UrlGrp;
int SubGrp;
int DirGrp;
int HAGrp;
int StoffGrp;
int Kuclr=9;
int FillClr=15;
float AudioTiming=0;
extern int AllTracks;
extern int SoftVol;
int Quality=1;
char **GetDrives(void);
extern char HomeDir[200];
extern FMGIMG pause_str,play_str,muted_str,sound_str;
int MakeurlGroup(DIALOG *D,void *arg);
int MakeDirPlayGroup(DIALOG *D,void *arg);
int MakeHdmiAudioGroup(DIALOG *D,void *arg);
int MaketoolsGroup(DIALOG *D,void *arg);
int MakeVideoGroup(DIALOG *D,void *arg);
int urlGroup( DIALOG *D,void **v,void *pt);
int DirPlayGroup( DIALOG *D,void **v,void *pt);
int MakeaudiodelayGroup(DIALOG *D,void *arg);
int MakeAudioTimeGroup(DIALOG *D,void *arg);
int MakesubtitleGroup(DIALOG *D,void *arg);
int MakeMakeKaraokeGroup(DIALOG *D,void *arg);
int MakeSubtitleoffGroup(DIALOG *D,void *arg);
int SetTheme(void) {
  FILE *fp=NULL;
  sprintf(urlstring,"%-s/.kgMplayer/Theme",HomeDir);
  fp = fopen(urlstring,"r");
  if(fp==NULL) {
    fp = fopen(urlstring,"w");
    if(fp != NULL){
      fprintf(fp,"%d %d %d\n",Thred,Thgreen,Thblue);
      fclose(fp);
    }
  }
  else {
    fscanf(fp,"%d%d%d",&Thred,&Thgreen,&Thblue);
    fclose(fp);
  }
  urlstring[0]='\0';
  return 1;
}
void ModifykgMplayerGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
   kgDefineColor(221,(Thred+50)%256,(Thgreen+50)%256,(Thblue+50)%256);
//   gc->char_clr=221;
   gc->ProgBodrColor =6;
   gc->MsgFont =27;
   gc->PromptFont =12;
   gc->ButtonFont =12;
   gc->GuiFontSize =9;
   gc->but_char = 9;
   Kuclr = gc->SplashCharColor;
   FillClr = gc->fill_clr;
}


int kgMplayerGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  BUT_STR  *butn0=NULL; 
  butn0= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn0[0].sw=1;
  strcpy(butn0[0].title,(char *)"");
  butn0[0].xpmn=(void *)(&play_str);
  butn0[0].xpmp=NULL;
  butn0[0].xpmh=NULL;
  butn0[0].bkgr=-100120100;
//  butn0[0].bkgr=-1;
  butn0[0].butncode='p';
  butn0[1].sw=1;
  strcpy(butn0[1].title,(char *)"");
  butn0[1].xpmn=kgPowerdownImage(36,147,0,0);
  butn0[1].xpmp=NULL;
  butn0[1].xpmh=NULL;
  butn0[1].bkgr=-100120100;
//  butn0[1].bkgr=-1;
  butn0[1].butncode='c';
#if 0
  DIL h0 = { 
    'h',
    382,156,  
    538,189,
    2,0,  
    72, 
    25, 
    2,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn0, 
    kgMplayersplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
#else
  DIL h0 = { 
    'h',
    463,180,  
    594,238,
    12,0,  
    42, 
    42, 
    2,1, 
    5,0.2500000,0,0,0,0, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn0, 
    kgMplayersplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
#endif
  strcpy(h0.Wid,(char *)"kgMplayerWidget4");
  char *menu1[]  = { 
    (char *)"File",
    (char *)"CD",
    (char *)"List",
//    (char *)"URL",
    (char *) "Dir",
    NULL 
  };
  ThumbNail **th0 ;
  DIRA r1 = { 
    'r',
    60,12,
    383,35,
    8,0,  
    55, 
    25, 
    1,2, 
    0,1, 
    (int *)v[0], 
    NULL, 
    NULL ,
    NULL,kgMplayerbrowser1callback, /* *args, callback */
    0,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th0 = (ThumbNail **)kgStringToThumbNails((char **)menu1);
  r1.list=(void **)th0;
  strcpy(r1.Wid,(char *)"kgMplayerWidget6");
  char **menu2 ; 
  menu2= GetDrives();
  char *prompt2 ; 
  prompt2=(char *)malloc(14);
  strcpy(prompt2,(char *)"Device");
  DIW w2 = { 
    'w',
    60,38,  
    210,68,   
    3,  
    (int *)v[1],
    prompt2 ,
    menu2 ,
    NULL,kgMplayerbrowser2callback, /* *args, callback */
    1 
  };
  strcpy(w2.Wid,(char *)"kgMplayerWidget8");
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"<Select Media File>");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-122176170;
  butn3[0].bkgr=-69128181;
  butn3[0].bkgr=-1;
  butn3[0].butncode='f';
  DIN b3 = { 
    'n',
    82,35,  
    340,70,
    2,2,  
    250, 
    24, 
    1,1, 
    5,0.50000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    kgMplayerbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b3.Wid,(char *)"Select");
  DII i4 = { 
    'i',
    58,190,  
    362,220,  
    32,1,0   
  };
  strcpy(i4.Wid,(char *)"kgMplayerWidget13");
  char *xpm5= NULL;
  DIP p5 = { 
    'p',
    5,10,  
    55,270,  
    (void *)xpm5,
    -1, /* bkgr colour */ 
      0,0 /* border hide */ 
  };
  p5.xpm = logo(50,190);
  strcpy(p5.Wid,(char *)"kgMplayerWidget10");
  DIHB d6 = { 
    'P',
    436,6,  
    590,26,   
    0,100,  
    121,  
    (int *)v[2],
    NULL,
    NULL,kgMplayerdslide1callback, /* *args, callback */
    0,0,7,S1CLR, /* bordr,hide,type,color */
  };
  strcpy(d6.Wid,(char *)"kgMplayerWidget7");
  DIHB d7 = { 
    'P',
    436,30,  
    590,50,   
    0,100,  
    121,  
    (int *)v[3],
    NULL,
    NULL,kgMplayerdslide2callback, /* *args, callback */
    0,0,7,S1CLR, /* bordr,hide,type,color */
  };
  strcpy(d7.Wid,(char *)"kgMplayerWidget8");
  DIHB d8 = {
    'P',
    414,83,
    591,107,
    0,100,
    163,
    (int *)v[4],
    NULL,
    NULL,kgMplayerdslide3callback, /* *args, callback */
    0,0,0,-1, /* bordr,hide,type,color */
  };

  strcpy(d8.Wid,(char *)"kgMplayerWidget9");
  DIM m9 = { 
    'm',
    353,3,  
    435,27,  
    0,0  
  };
  strncpy(m9.msg,(char *)"!f08Vert. Stretch",499);
  strcpy(m9.Wid,(char *)"kgMplayerWidget10");
  DIM m10 = { 
    'm',
    353,27,  
    435,51,  
    0,0  
  };
  strncpy(m10.msg,(char *)"!f08Hori. Stretch",499);
  strcpy(m10.Wid,(char *)"kgMplayerWidget11");
  DIHB d11 = { 
    'P',
//    436,58,  
//    590,80,   
    436,60,  
    590,74,   
    0,100,  
    121,  
    (int *)v[5],
    NULL,
    NULL,kgMplayerdslide4callback, /* *args, callback */
    0,0,5,S2CLR, /* bordr,hide,type,color */
  };
  strcpy(d11.Wid,(char *)"kgMplayerWidget14");
  DIM m12 = { 
    'm',
    378,56,  
    435,80,  
    0,0  
  };
  strncpy(m12.msg,(char *)"!f08Volume",499);
  strcpy(m12.Wid,(char *)"kgMplayerWidget15");
  BUT_STR  *butn13=NULL; 
  butn13= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn13[0].sw=1;
  strcpy(butn13[0].title,(char *)"Eject");
  butn13[0].xpmn=NULL;
  butn13[0].xpmp=NULL;
  butn13[0].xpmh=NULL;
  butn13[0].bkgr=-232146150;
  butn13[0].bkgr=S2CLR;
  butn13[0].butncode='';
  butn13[1].sw=1;
  strcpy(butn13[1].title,(char *)"Rip Media");
  butn13[1].xpmn=NULL;
  butn13[1].xpmp=NULL;
  butn13[1].xpmh=NULL;
  butn13[1].bkgr=-232146250;
  butn13[1].bkgr=S2CLR;
  butn13[1].bkgr=-1;
  butn13[1].butncode='';
  DIN b13 = { 
    'n',
    215,34,
    365,64,
    4,2,  
    64, 
    24, 
    2,1, 
    5,0.500000,0,1,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn13, 
    kgMplayerbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b13.Wid,(char *)"kgMplayerWidget14");
  char *menu14[]  = { 
    (char *)"Enhance Volume",
    NULL 
  };
  ThumbNail **th1 ;
  DICH c14 = { 
    'c',
    57,158,  
    189,186,   
    8,0,  
    120, 
    25, 
    1,1, 
    0,1, 
    (int *)v[6], 
    NULL, 
    NULL, 
    NULL,kgMplayerbrowser3callback, /* *args, callback */
    1,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    1, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th1 = (ThumbNail **)kgStringToThumbNails((char **)menu14);
  c14.list=(void **)th1;
  strcpy(c14.Wid,(char *)"kgMplayerWidget17");
  DIM m15 = { 
    'm',
    344,82,
    414,106,
    0,0  
  };
  strncpy(m15.msg,(char *)"!f08Play Pos.",499);
  strcpy(m15.Wid,(char *)"kgMplayerWidget17");
  BUT_STR  *butn16=NULL;
  butn16= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn16[0].sw=1;
  strcpy(butn16[0].title,(char *)"Manage Display");
  butn16[0].xpmn=NULL;
  butn16[0].xpmp=NULL;
  butn16[0].xpmh=NULL;
  butn16[0].bkgr=-232146150;
  butn16[0].bkgr=S1CLR;
  butn16[0].bkgr=-1;
  butn16[0].butncode='m';
  DIN b16 = {
    'n',
    92,175,
    200,205,
    2,2,
    98,
    24,
    1,1,
    5,0.50000,0,1,0,0, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/

    butn16,
    kgMplayerbutton3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b16.Wid,(char *)"Display");
  BUT_STR  *butn17=NULL;
  butn17= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn17[0].sw=1;
  strcpy(butn17[0].title,(char *)"< Select Play List >");
  butn17[0].xpmn=NULL;
  butn17[0].xpmp=NULL;
  butn17[0].xpmh=NULL;
  butn17[0].bkgr=-34172211;
  butn17[0].bkgr=-127207149;
  butn17[0].bkgr=-69128181;
  butn17[0].bkgr=-1;
  butn17[0].butncode='s';
  DIN b17 = {
    'n',
    93,60,
    345,94, 
    2,2,
    244,
    24,
    1,1,
    5,0.500000,0,1,0,0, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/

    butn17,
    kgMplayerbutton4callback, /*  Callbak */
    NULL  /* any args */
  };
  strcpy(b17.Wid,(char *)"kgMplayerWidget18");
  char *menu18[]  = {
    (char *)"3D to 2D",
    (char *)"FullScreen",
    NULL
  };
  ThumbNail **th2 ;
  DICH c18 = {
    'c',
    393,107,
    590,139,
    8,0,
    85,
    25,
    1,2,
    1,1,
    (int *)v[7],
    NULL,
    NULL,
    NULL,kgMplayerbrowser4callback, /* *args, callback */
    1,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    0  /* =1 hide  */
   };
  th2 = (ThumbNail **)kgStringToThumbNails((char **)menu18);
  c18.list=(void **)th2;
  strcpy(c18.Wid,(char *)"kgMplayerWidget19");
  char *menu19[]  = {
    (char *)"Random",
    (char *)"Loop",
    NULL
  };
  ThumbNail **th3 ;
  DICH c19 = {
    'c',
    98,103, 
    279,130,  
    8,0, 
    80,
    25,
    1,2,
    0,1,
    (int *)v[8],
    NULL,
    NULL,
    NULL,kgMplayerbrowser5callback, /* *args, callback */
    1,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
     1, /* bordr */
    0, /* bkgr */
    1  /* =1 hide  */
   };
  th3 = (ThumbNail **)kgStringToThumbNails((char **)menu19);
  c19.list=(void **)th3;
  strcpy(c19.Wid,(char *)"kgMplayerWidget20");
  char *menu20[]  = { 
    (char *)"All Tracks",
    (char *)"Main Track",
    NULL 
  };
  ThumbNail **th4 ;
  DIRA r20 = { 
    'r',
    107,128,  
    347,154,   
    8,0,  
    110, 
    23, 
    1,2, 
    -2302756,1, 
    (int *)v[9], 
    NULL, 
    NULL ,
    NULL,kgMplayerbrowser6callback, /* *args, callback */
    1,  /* Border Offset  */
     2,  /* Scroll width  */
     0,  /* Type  */
     0, /* item highlight */
    0, /* bordr */
    0, /* bkgr */
    1  /* =1 hide  */
   };
  th4 = (ThumbNail **)kgStringToThumbNails((char **)menu20);
  r20.list=(void **)th4;
  strcpy(r20.Wid,(char *)"kgMplayerWidget21");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+22));
 
  d =dtmp+i; 
  d[21].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIL));
  kgMplayersplbutton1init(&h0,pt) ;
  *d[0].h = h0;
  d[0].h->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIRA));
  kgMplayerbrowser1init(&r1,pt) ;
  *d[1].r = r1;
  d[1].r->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIW));
  *d[2].w = w2;
  d[2].w->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIN));
  kgMplayerbutton1init(&b3,pt) ;
  *d[3].N = b3;
  d[3].N->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DII));
  *d[4].i = i4;
  d[4].i->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIP));
  *d[5].p = p5;
  d[5].p->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIHB));
  *d[6].B = d6;
  d[6].B->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIHB));
  *d[7].B = d7;
  d[7].B->item = -1;
  d[8].t = (DIT *)malloc(sizeof(DIHB));
  *d[8].B = d8;
  d[8].B->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIM));
  *d[9].m = m9;
  d[9].m->item = -1;
  d[10].t = (DIT *)malloc(sizeof(DIM));
  *d[10].m = m10;
  d[10].m->item = -1;
  d[11].t = (DIT *)malloc(sizeof(DIHB));
  *d[11].B = d11;
  d[11].B->item = -1;
  d[12].t = (DIT *)malloc(sizeof(DIM));
  *d[12].m = m12;
  d[12].m->item = -1;
  d[13].t = (DIT *)malloc(sizeof(DIN));
  kgMplayerbutton2init(&b13,pt) ;
  *d[13].N = b13;
  d[13].N->item = -1;
  d[14].t = (DIT *)malloc(sizeof(DICH));
  kgMplayerbrowser3init(&c14,pt) ;
  *d[14].c = c14;
  d[14].c->item = -1;
  d[15].t = (DIT *)malloc(sizeof(DIM));
  *d[15].m = m15;
  d[15].m->item = -1;
  d[16].t = (DIT *)malloc(sizeof(DIN));
  kgMplayerbutton3init(&b16,pt) ;
  *d[16].N = b16;
  d[16].N->item = -1;
  d[17].t = (DIT *)malloc(sizeof(DIN));
  kgMplayerbutton4init(&b17,pt) ;
  *d[17].N = b17;
  d[17].N->item = -1;
  d[18].t = (DIT *)malloc(sizeof(DICH));
  kgMplayerbrowser4init(&c18,pt) ;
  *d[18].c = c18;
  d[18].c->item = -1;
  d[19].t = (DIT *)malloc(sizeof(DICH));
  kgMplayerbrowser5init(&c19,pt) ;
  *d[19].c = c19;
  d[19].c->item = -1;
  d[20].t = (DIT *)malloc(sizeof(DIRA));
  kgMplayerbrowser6init(&r20,pt) ;
  *d[20].r = r20;
  d[20].r->item = -1;
  d[21].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 
int kgMplayer( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k,ToolsGrp,VideoGrp,AudioGrp;
  int BGrpId,RGrpId,PGrpId;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = kgMplayerGroup(&D,v,pt);
//  UrlGrp = MakeurlGroup(&D,NULL);
  UrlGrp =urlGroup( &D,v+10,NULL);
  DirGrp =MakeDirPlayGroup( &D,NULL);
  HAGrp =MakeHdmiAudioGroup( &D,NULL);
  StoffGrp = MakeSubtitleoffGroup( &D,NULL);
  kgShiftGrp(&D,StoffGrp,20,-30);
  kgSetGrpVisibility(&D,UrlGrp,0);
  kgSetGrpVisibility(&D,DirGrp,0);
  kgSetGrpVisibility(&D,HAGrp,0);
  kgSetGrpVisibility(&D,StoffGrp,1);
  ToolsGrp = MaketoolsGroup(&D,NULL);
  VideoGrp = MakeVideoGroup(&D,NULL);
  kgShiftWidget(&D,16,-40,-25);
  AudioGrp = MakeAudioTimeGroup(&D,NULL);
  SubGrp = MakesubtitleGroup(&D,NULL);
  kgShiftGrp(&D,SubGrp,0,-35);
  kgShiftGrp(&D,HAGrp,30,5);
  kgShiftGrp(&D,AudioGrp,-150,-10);
  kgShiftGrp(&D,UrlGrp,30,0);
//  kgSetGrpVisibility(&D,ToolsGrp,0);
  BGrpId = kgOpenGrp(&D);
  kgAddtoGrp(&D,BGrpId,kgGetWidget(&D,0));
  kgAddtoGrp(&D,BGrpId,kgGetWidget(&D,4));
  kgAddtoGrp(&D,BGrpId,kgGetWidget(&D,14));
  kgAddtoGrp(&D,BGrpId,kgGetNamedWidget(&D,"tools"));
  kgAddtoGrp(&D,BGrpId,kgGetNamedWidget(&D,"AudioTime"));
  kgAddtoGrp(&D,BGrpId,kgGetNamedWidget(&D,"AudioTimeWidget2"));
//  kgAddtoGrp(&D,GrpId,kgGetWidget(&D,19));
  kgShiftGrp(&D,BGrpId,0,40);
  RGrpId = kgOpenGrp(&D);
  kgAddtoGrp(&D,RGrpId,kgGetWidget(&D,6));
  kgAddtoGrp(&D,RGrpId,kgGetWidget(&D,7));
  kgAddtoGrp(&D,RGrpId,kgGetWidget(&D,8));
  kgAddtoGrp(&D,RGrpId,kgGetWidget(&D,9));
  kgAddtoGrp(&D,RGrpId,kgGetWidget(&D,10));
  kgAddtoGrp(&D,RGrpId,kgGetWidget(&D,11));
  kgAddtoGrp(&D,RGrpId,kgGetWidget(&D,12));
  kgAddtoGrp(&D,RGrpId,kgGetWidget(&D,15));
  kgAddtoGrp(&D,RGrpId,kgGetWidget(&D,18));
  kgAddtoGrp(&D,RGrpId,kgGetNamedWidget(&D,"VideoWidget1"));
  kgAddtoGrp(&D,RGrpId,kgGetNamedWidget(&D,"VideoWidget2"));
  kgShiftGrp(&D,RGrpId,30,30);
  PGrpId = kgOpenGrp(&D);
  kgAddtoGrp(&D,PGrpId,kgGetNamedWidget(&D,"tools"));
  kgAddtoGrp(&D,PGrpId,kgGetWidget(&D,0));
  kgShiftGrp(&D,PGrpId,30,-10);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 13;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 605;    /*  Length of Dialog */
  D.yl = 240;    /*  Width  of Dialog */
  D.xl = 635;    /*  Length of Dialog */
  D.yl = 280;    /*  Width  of Dialog */
  D.Initfun = kgMplayerinit;    /*   init fuction for Dialog */
  D.Cleanupfun = kgMplayercleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = kgMplayerCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = kgMplayerResizeCallBack;  /*  Resize callback */
#if 1 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = kgMplayerWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 0;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
  strcpy(D.name,"kgMplayer ");    /*  Dialog name you may change */
  strcat(D.name,VER);
//  printf("%s\n",D.name);
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
       D.xo=(xres-D.xl)/3;
       D.yo=(yres-D.yl)/3;

  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
//  kgColorTheme(&D,80,90,80);    /*  set colors for gui*/
#if 1
//  kgColorTheme(&D,Thred,Thgreen,Thblue);   /*  set colors for gui*/
//  ModifykgMplayerGc(&(D.gc));    /*  set colors for gui*/
#endif
  FillClr = D.gc.fill_clr;
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunkgMplayer(void *arg) {
/*************************************************

    RadioButtons1  1 data value
    Browser2  1 data value
    Integerslidebar1  1 data value
    Integerslidebar2  1 data value
    Integerslidebar3  1 data value
    Integerslidebar4  1 data value

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   int   v2 = 0;
   int   v3 = 0;
   int   v4 = 0;
   int   v5 = 80;
   int   v6 = 0;
   int   v7 = 1;
   int   v8 = 1;
   int   v9 = 1;
   void* v[12];
   char v10[500]="";
   int v11 =1;
   v11 = Quality;
   if(AllTracks==0) v9=2;
   strcpy(urlstring,"");
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   v[4]=(void *)(&v4);
   v[5]=(void *)(&v5);
   v[6]=(void *)(&v6);
   v[7]=(void *)(&v7);
   v[8]=(void *)(&v8);
   v[9]=(void *)(&v9);
   v[10]=(void *)(urlstring);;
   v[11]=(void *)(&v11);
   void *pt=NULL; /* pointer to send any extra information */
   kgMplayer(NULL,v,pt );
   return NULL;
}
