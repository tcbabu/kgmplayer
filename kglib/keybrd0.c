#define D_TH 1
#include "kulina.h"
#include "keybrd0Callbacks.h"
int Modifykeybrd0(void *Tmp,int GrpId);
static float  Rfac= 0.0 ;
static int RED=120,GREEN=125,BLUE=115;
static int Btype=1;

static char BUFF[100];
static char Sfac[10]="53";
static int Bfont=16,Bclr=0;
static int FillClr=101;
static float Btrans=0.1;
static unsigned char R=230,G=60,B=0;
static int ButClr=-1;


int kgShowKeybrd0(void *Tmp) {
   DIALOG *D;
   KEYBRD *Kbrd;
   D = (DIALOG *)Tmp;
   Kbrd = D->Kbrd;
   if(Kbrd->Vis) return 0;
   kgSetGrpVisibility(D,Kbrd->cgrp,0);
   kgSetGrpVisibility(D,Kbrd->ongrp,0);
   kgSetGrpVisibility(D,Kbrd->grp2,0);
   kgSetGrpVisibility(D,Kbrd->grp3,1);
   kgSetGrpVisibility(D,Kbrd->sgrp,1);
   kgSetGrpVisibility(D,Kbrd->offgrp,1);
   kgSetGrpVisibility(D,Kbrd->grp1,1);
   Kbrd->ShiftPress=0;
   Kbrd->CapsLock=0;
   if(D->wc != NULL) {
     kgUpdateGrp(D,Kbrd->grp3);
     kgUpdateGrp(D,Kbrd->sgrp);
     kgUpdateGrp(D,Kbrd->offgrp);
     kgUpdateGrp(D,Kbrd->grp1);
     kgUpdateOn(D);
   }
   Kbrd->Vis = 1;
   return Kbrd->GrpId;
}

void Modifykeybrd0Gc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
#if 0
  KEYBRD *Kbrd;
  int Red,Green,Blue;
  Kbrd = (KEYBRD *)pt;
  kgGetDefaultRGB(Kbrd->FillClr,&Red,&Green,&Blue);
  xpm0 = (char *)MakeLightImage( 1177,276,Red,Green,Blue,0.0);
#endif

int keybrd0Group( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  char *xpm0=   NULL; /* pixmap info */ 
  KEYBRD *Kbrd;
  int Red,Green,Blue;
  Kbrd = (KEYBRD *)pt;
//  xpm0 = (char *)MakeLightImage( 1177,260,Red,Green,Blue,0.0);
  DIP p0 = { 
    'p',
    43,28,  
    1220,288,  
    (void *)xpm0,
    -1, /* bkgr colour */ 
      0,0,Btrans /* border hide transparency*/ 
  };
  p0.transparency = Btrans;
  strcpy(p0.Wid,(char *)"Keybrdbkgr");
  p0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*13);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode='';
  butn1[2].sw=1;
  strcpy(butn1[2].title,(char *)"");
  butn1[2].xpmn=NULL;
  butn1[2].xpmp=NULL;
  butn1[2].xpmh=NULL;
  butn1[2].bkgr=-1;
  butn1[2].butncode='';
  butn1[3].sw=1;
  strcpy(butn1[3].title,(char *)"");
  butn1[3].xpmn=NULL;
  butn1[3].xpmp=NULL;
  butn1[3].xpmh=NULL;
  butn1[3].bkgr=-1;
  butn1[3].butncode='';
  butn1[4].sw=1;
  strcpy(butn1[4].title,(char *)"");
  butn1[4].xpmn=NULL;
  butn1[4].xpmp=NULL;
  butn1[4].xpmh=NULL;
  butn1[4].bkgr=-1;
  butn1[4].butncode='';
  butn1[5].sw=1;
  strcpy(butn1[5].title,(char *)"");
  butn1[5].xpmn=NULL;
  butn1[5].xpmp=NULL;
  butn1[5].xpmh=NULL;
  butn1[5].bkgr=-1;
  butn1[5].butncode='';
  butn1[6].sw=1;
  strcpy(butn1[6].title,(char *)"");
  butn1[6].xpmn=NULL;
  butn1[6].xpmp=NULL;
  butn1[6].xpmh=NULL;
  butn1[6].bkgr=-1;
  butn1[6].butncode='';
  butn1[7].sw=1;
  strcpy(butn1[7].title,(char *)"");
  butn1[7].xpmn=NULL;
  butn1[7].xpmp=NULL;
  butn1[7].xpmh=NULL;
  butn1[7].bkgr=-1;
  butn1[7].butncode='';
  butn1[8].sw=1;
  strcpy(butn1[8].title,(char *)"");
  butn1[8].xpmn=NULL;
  butn1[8].xpmp=NULL;
  butn1[8].xpmh=NULL;
  butn1[8].bkgr=-1;
  butn1[8].butncode='';
  butn1[9].sw=1;
  strcpy(butn1[9].title,(char *)"");
  butn1[9].xpmn=NULL;
  butn1[9].xpmp=NULL;
  butn1[9].xpmh=NULL;
  butn1[9].bkgr=-1;
  butn1[9].butncode='';
  butn1[10].sw=1;
  strcpy(butn1[10].title,(char *)"");
  butn1[10].xpmn=NULL;
  butn1[10].xpmp=NULL;
  butn1[10].xpmh=NULL;
  butn1[10].bkgr=-1;
  butn1[10].butncode='';
  butn1[11].sw=1;
  strcpy(butn1[11].title,(char *)"");
  butn1[11].xpmn=NULL;
  butn1[11].xpmp=NULL;
  butn1[11].xpmh=NULL;
  butn1[11].bkgr=-1;
  butn1[11].butncode='';
  butn1[12].sw=1;
  strcpy(butn1[12].title,(char *)"");
  butn1[12].xpmn=NULL;
  butn1[12].xpmp=NULL;
  butn1[12].xpmh=NULL;
  butn1[12].bkgr=-1;
  butn1[12].butncode='';
  DIN b1 = { 
    'n',
    135,44,  
    1101,104,
    6,2,  
    68, 
    48, 
    13,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    keybrd0button1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"numeric");
  b1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*13);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=-1;
  butn2[0].butncode='';
  butn2[1].sw=1;
  strcpy(butn2[1].title,(char *)"");
  butn2[1].xpmn=NULL;
  butn2[1].xpmp=NULL;
  butn2[1].xpmh=NULL;
  butn2[1].bkgr=-1;
  butn2[1].butncode='';
  butn2[2].sw=1;
  strcpy(butn2[2].title,(char *)"");
  butn2[2].xpmn=NULL;
  butn2[2].xpmp=NULL;
  butn2[2].xpmh=NULL;
  butn2[2].bkgr=-1;
  butn2[2].butncode='';
  butn2[3].sw=1;
  strcpy(butn2[3].title,(char *)"");
  butn2[3].xpmn=NULL;
  butn2[3].xpmp=NULL;
  butn2[3].xpmh=NULL;
  butn2[3].bkgr=-1;
  butn2[3].butncode='';
  butn2[4].sw=1;
  strcpy(butn2[4].title,(char *)"");
  butn2[4].xpmn=NULL;
  butn2[4].xpmp=NULL;
  butn2[4].xpmh=NULL;
  butn2[4].bkgr=-1;
  butn2[4].butncode='';
  butn2[5].sw=1;
  strcpy(butn2[5].title,(char *)"");
  butn2[5].xpmn=NULL;
  butn2[5].xpmp=NULL;
  butn2[5].xpmh=NULL;
  butn2[5].bkgr=-1;
  butn2[5].butncode='';
  butn2[6].sw=1;
  strcpy(butn2[6].title,(char *)"");
  butn2[6].xpmn=NULL;
  butn2[6].xpmp=NULL;
  butn2[6].xpmh=NULL;
  butn2[6].bkgr=-1;
  butn2[6].butncode='';
  butn2[7].sw=1;
  strcpy(butn2[7].title,(char *)"");
  butn2[7].xpmn=NULL;
  butn2[7].xpmp=NULL;
  butn2[7].xpmh=NULL;
  butn2[7].bkgr=-1;
  butn2[7].butncode='';
  butn2[8].sw=1;
  strcpy(butn2[8].title,(char *)"");
  butn2[8].xpmn=NULL;
  butn2[8].xpmp=NULL;
  butn2[8].xpmh=NULL;
  butn2[8].bkgr=-1;
  butn2[8].butncode='';
  butn2[9].sw=1;
  strcpy(butn2[9].title,(char *)"");
  butn2[9].xpmn=NULL;
  butn2[9].xpmp=NULL;
  butn2[9].xpmh=NULL;
  butn2[9].bkgr=-1;
  butn2[9].butncode='';
  butn2[10].sw=1;
  strcpy(butn2[10].title,(char *)"");
  butn2[10].xpmn=NULL;
  butn2[10].xpmp=NULL;
  butn2[10].xpmh=NULL;
  butn2[10].bkgr=-1;
  butn2[10].butncode='';
  butn2[11].sw=1;
  strcpy(butn2[11].title,(char *)"");
  butn2[11].xpmn=NULL;
  butn2[11].xpmp=NULL;
  butn2[11].xpmh=NULL;
  butn2[11].bkgr=-1;
  butn2[11].butncode='';
  butn2[12].sw=1;
  strcpy(butn2[12].title,(char *)"");
  butn2[12].xpmn=NULL;
  butn2[12].xpmp=NULL;
  butn2[12].xpmh=NULL;
  butn2[12].bkgr=-1;
  butn2[12].butncode='';
  DIN b2 = { 
    'n',
    149,102,  
    1115,162,
    6,2,  
    68, 
    48, 
    13,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    keybrd0button2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b2.Wid,(char *)"qwert");
  b2.item = -1;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*11);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-1;
  butn3[0].butncode='';
  butn3[1].sw=1;
  strcpy(butn3[1].title,(char *)"");
  butn3[1].xpmn=NULL;
  butn3[1].xpmp=NULL;
  butn3[1].xpmh=NULL;
  butn3[1].bkgr=-1;
  butn3[1].butncode='';
  butn3[2].sw=1;
  strcpy(butn3[2].title,(char *)"");
  butn3[2].xpmn=NULL;
  butn3[2].xpmp=NULL;
  butn3[2].xpmh=NULL;
  butn3[2].bkgr=-1;
  butn3[2].butncode='';
  butn3[3].sw=1;
  strcpy(butn3[3].title,(char *)"");
  butn3[3].xpmn=NULL;
  butn3[3].xpmp=NULL;
  butn3[3].xpmh=NULL;
  butn3[3].bkgr=-1;
  butn3[3].butncode='';
  butn3[4].sw=1;
  strcpy(butn3[4].title,(char *)"");
  butn3[4].xpmn=NULL;
  butn3[4].xpmp=NULL;
  butn3[4].xpmh=NULL;
  butn3[4].bkgr=-1;
  butn3[4].butncode='';
  butn3[5].sw=1;
  strcpy(butn3[5].title,(char *)"");
  butn3[5].xpmn=NULL;
  butn3[5].xpmp=NULL;
  butn3[5].xpmh=NULL;
  butn3[5].bkgr=-1;
  butn3[5].butncode='';
  butn3[6].sw=1;
  strcpy(butn3[6].title,(char *)"");
  butn3[6].xpmn=NULL;
  butn3[6].xpmp=NULL;
  butn3[6].xpmh=NULL;
  butn3[6].bkgr=-1;
  butn3[6].butncode='';
  butn3[7].sw=1;
  strcpy(butn3[7].title,(char *)"");
  butn3[7].xpmn=NULL;
  butn3[7].xpmp=NULL;
  butn3[7].xpmh=NULL;
  butn3[7].bkgr=-1;
  butn3[7].butncode='';
  butn3[8].sw=1;
  strcpy(butn3[8].title,(char *)"");
  butn3[8].xpmn=NULL;
  butn3[8].xpmp=NULL;
  butn3[8].xpmh=NULL;
  butn3[8].bkgr=-1;
  butn3[8].butncode='';
  butn3[9].sw=1;
  strcpy(butn3[9].title,(char *)"");
  butn3[9].xpmn=NULL;
  butn3[9].xpmp=NULL;
  butn3[9].xpmh=NULL;
  butn3[9].bkgr=-1;
  butn3[9].butncode='';
  butn3[10].sw=1;
  strcpy(butn3[10].title,(char *)"");
  butn3[10].xpmn=NULL;
  butn3[10].xpmp=NULL;
  butn3[10].xpmh=NULL;
  butn3[10].bkgr=-1;
  butn3[10].butncode='';
  DIN b3 = { 
    'n',
    168,150,  
    986,220,
    6,2,  
    68, 
    48, 
    11,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    keybrd0button3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b3.Wid,(char *)"asdf");
  b3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*5);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-1;
  butn4[0].butncode='';
  butn4[1].sw=1;
  strcpy(butn4[1].title,(char *)"");
  butn4[1].xpmn=NULL;
  butn4[1].xpmp=NULL;
  butn4[1].xpmh=NULL;
  butn4[1].bkgr=-1;
  butn4[1].butncode='';
  butn4[2].sw=1;
  strcpy(butn4[2].title,(char *)"");
  butn4[2].xpmn=NULL;
  butn4[2].xpmp=NULL;
  butn4[2].xpmh=NULL;
  butn4[2].bkgr=-1;
  butn4[2].butncode='';
  butn4[3].sw=1;
  strcpy(butn4[3].title,(char *)"");
  butn4[3].xpmn=NULL;
  butn4[3].xpmp=NULL;
  butn4[3].xpmh=NULL;
  butn4[3].bkgr=-1;
  butn4[3].butncode='';
  butn4[4].sw=1;
  strcpy(butn4[4].title,(char *)"");
  butn4[4].xpmn=NULL;
  butn4[4].xpmp=NULL;
  butn4[4].xpmh=NULL;
  butn4[4].bkgr=-1;
  butn4[4].butncode='';
  DIN b4 = { 
    'n',
    203,198,  
    561,274,
    6,2,  
    68, 
    48, 
    5,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    keybrd0button4callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b4.Wid,(char *)"zxcv");
  b4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*5);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(char *)"");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-1;
  butn5[0].butncode='';
  butn5[1].sw=1;
  strcpy(butn5[1].title,(char *)"");
  butn5[1].xpmn=NULL;
  butn5[1].xpmp=NULL;
  butn5[1].xpmh=NULL;
  butn5[1].bkgr=-1;
  butn5[1].butncode='';
  butn5[2].sw=1;
  strcpy(butn5[2].title,(char *)"");
  butn5[2].xpmn=NULL;
  butn5[2].xpmp=NULL;
  butn5[2].xpmh=NULL;
  butn5[2].bkgr=-1;
  butn5[2].butncode='';
  butn5[3].sw=1;
  strcpy(butn5[3].title,(char *)"");
  butn5[3].xpmn=NULL;
  butn5[3].xpmp=NULL;
  butn5[3].xpmh=NULL;
  butn5[3].bkgr=-1;
  butn5[3].butncode='';
  butn5[4].sw=1;
  strcpy(butn5[4].title,(char *)"");
  butn5[4].xpmn=NULL;
  butn5[4].xpmp=NULL;
  butn5[4].xpmh=NULL;
  butn5[4].bkgr=-1;
  butn5[4].butncode='';
  DIN b5 = { 
    'n',
    674,198,  
    1052,274,
    6,2,  
    68, 
    48, 
    5,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    keybrd0button5callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"nmetc");
  b5.item = -1;
  BUT_STR  *butn6=NULL; 
  butn6= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn6[0].sw=1;
  strcpy(butn6[0].title,(char *)"Capslock");
  butn6[0].xpmn=NULL;
  butn6[0].xpmp=NULL;
  butn6[0].xpmh=NULL;
  butn6[0].bkgr=-1;
  butn6[0].butncode='';
  DIN b6 = { 
    'n',
    53,150,  
    171,220,
    4,2,  
    110, 
    48, 
    1,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn6, 
    keybrd0button6callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b6.Wid,(char *)"CapsLock");
  b6.item = -1;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*13);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(char *)"");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-1;
  butn7[0].butncode='';
  butn7[1].sw=1;
  strcpy(butn7[1].title,(char *)"");
  butn7[1].xpmn=NULL;
  butn7[1].xpmp=NULL;
  butn7[1].xpmh=NULL;
  butn7[1].bkgr=-1;
  butn7[1].butncode='';
  butn7[2].sw=1;
  strcpy(butn7[2].title,(char *)"");
  butn7[2].xpmn=NULL;
  butn7[2].xpmp=NULL;
  butn7[2].xpmh=NULL;
  butn7[2].bkgr=-1;
  butn7[2].butncode='';
  butn7[3].sw=1;
  strcpy(butn7[3].title,(char *)"");
  butn7[3].xpmn=NULL;
  butn7[3].xpmp=NULL;
  butn7[3].xpmh=NULL;
  butn7[3].bkgr=-1;
  butn7[3].butncode='';
  butn7[4].sw=1;
  strcpy(butn7[4].title,(char *)"");
  butn7[4].xpmn=NULL;
  butn7[4].xpmp=NULL;
  butn7[4].xpmh=NULL;
  butn7[4].bkgr=-1;
  butn7[4].butncode='';
  butn7[5].sw=1;
  strcpy(butn7[5].title,(char *)"");
  butn7[5].xpmn=NULL;
  butn7[5].xpmp=NULL;
  butn7[5].xpmh=NULL;
  butn7[5].bkgr=-1;
  butn7[5].butncode='';
  butn7[6].sw=1;
  strcpy(butn7[6].title,(char *)"");
  butn7[6].xpmn=NULL;
  butn7[6].xpmp=NULL;
  butn7[6].xpmh=NULL;
  butn7[6].bkgr=-1;
  butn7[6].butncode='';
  butn7[7].sw=1;
  strcpy(butn7[7].title,(char *)"");
  butn7[7].xpmn=NULL;
  butn7[7].xpmp=NULL;
  butn7[7].xpmh=NULL;
  butn7[7].bkgr=-1;
  butn7[7].butncode='';
  butn7[8].sw=1;
  strcpy(butn7[8].title,(char *)"");
  butn7[8].xpmn=NULL;
  butn7[8].xpmp=NULL;
  butn7[8].xpmh=NULL;
  butn7[8].bkgr=-1;
  butn7[8].butncode='';
  butn7[9].sw=1;
  strcpy(butn7[9].title,(char *)"");
  butn7[9].xpmn=NULL;
  butn7[9].xpmp=NULL;
  butn7[9].xpmh=NULL;
  butn7[9].bkgr=-1;
  butn7[9].butncode='';
  butn7[10].sw=1;
  strcpy(butn7[10].title,(char *)"");
  butn7[10].xpmn=NULL;
  butn7[10].xpmp=NULL;
  butn7[10].xpmh=NULL;
  butn7[10].bkgr=-1;
  butn7[10].butncode='';
  butn7[11].sw=1;
  strcpy(butn7[11].title,(char *)"");
  butn7[11].xpmn=NULL;
  butn7[11].xpmp=NULL;
  butn7[11].xpmh=NULL;
  butn7[11].bkgr=-1;
  butn7[11].butncode='';
  butn7[12].sw=1;
  strcpy(butn7[12].title,(char *)"");
  butn7[12].xpmn=NULL;
  butn7[12].xpmp=NULL;
  butn7[12].xpmh=NULL;
  butn7[12].bkgr=-1;
  butn7[12].butncode='';
  DIN b7 = { 
    'n',
    135,44,  
    1101,104,
    6,2,  
    68, 
    48, 
    13,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    keybrd0button7callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"NUMERIC");
  b7.item = -1;
  BUT_STR  *butn8=NULL; 
  butn8= (BUT_STR *)malloc(sizeof(BUT_STR)*13);
  butn8[0].sw=1;
  strcpy(butn8[0].title,(char *)"");
  butn8[0].xpmn=NULL;
  butn8[0].xpmp=NULL;
  butn8[0].xpmh=NULL;
  butn8[0].bkgr=-1;
  butn8[0].butncode='';
  butn8[1].sw=1;
  strcpy(butn8[1].title,(char *)"");
  butn8[1].xpmn=NULL;
  butn8[1].xpmp=NULL;
  butn8[1].xpmh=NULL;
  butn8[1].bkgr=-1;
  butn8[1].butncode='';
  butn8[2].sw=1;
  strcpy(butn8[2].title,(char *)"");
  butn8[2].xpmn=NULL;
  butn8[2].xpmp=NULL;
  butn8[2].xpmh=NULL;
  butn8[2].bkgr=-1;
  butn8[2].butncode='';
  butn8[3].sw=1;
  strcpy(butn8[3].title,(char *)"");
  butn8[3].xpmn=NULL;
  butn8[3].xpmp=NULL;
  butn8[3].xpmh=NULL;
  butn8[3].bkgr=-1;
  butn8[3].butncode='';
  butn8[4].sw=1;
  strcpy(butn8[4].title,(char *)"");
  butn8[4].xpmn=NULL;
  butn8[4].xpmp=NULL;
  butn8[4].xpmh=NULL;
  butn8[4].bkgr=-1;
  butn8[4].butncode='';
  butn8[5].sw=1;
  strcpy(butn8[5].title,(char *)"");
  butn8[5].xpmn=NULL;
  butn8[5].xpmp=NULL;
  butn8[5].xpmh=NULL;
  butn8[5].bkgr=-1;
  butn8[5].butncode='';
  butn8[6].sw=1;
  strcpy(butn8[6].title,(char *)"");
  butn8[6].xpmn=NULL;
  butn8[6].xpmp=NULL;
  butn8[6].xpmh=NULL;
  butn8[6].bkgr=-1;
  butn8[6].butncode='';
  butn8[7].sw=1;
  strcpy(butn8[7].title,(char *)"");
  butn8[7].xpmn=NULL;
  butn8[7].xpmp=NULL;
  butn8[7].xpmh=NULL;
  butn8[7].bkgr=-1;
  butn8[7].butncode='';
  butn8[8].sw=1;
  strcpy(butn8[8].title,(char *)"");
  butn8[8].xpmn=NULL;
  butn8[8].xpmp=NULL;
  butn8[8].xpmh=NULL;
  butn8[8].bkgr=-1;
  butn8[8].butncode='';
  butn8[9].sw=1;
  strcpy(butn8[9].title,(char *)"");
  butn8[9].xpmn=NULL;
  butn8[9].xpmp=NULL;
  butn8[9].xpmh=NULL;
  butn8[9].bkgr=-1;
  butn8[9].butncode='';
  butn8[10].sw=1;
  strcpy(butn8[10].title,(char *)"");
  butn8[10].xpmn=NULL;
  butn8[10].xpmp=NULL;
  butn8[10].xpmh=NULL;
  butn8[10].bkgr=-1;
  butn8[10].butncode='';
  butn8[11].sw=1;
  strcpy(butn8[11].title,(char *)"");
  butn8[11].xpmn=NULL;
  butn8[11].xpmp=NULL;
  butn8[11].xpmh=NULL;
  butn8[11].bkgr=-1;
  butn8[11].butncode='';
  butn8[12].sw=1;
  strcpy(butn8[12].title,(char *)"");
  butn8[12].xpmn=NULL;
  butn8[12].xpmp=NULL;
  butn8[12].xpmh=NULL;
  butn8[12].bkgr=-1;
  butn8[12].butncode='';
  DIN b8 = { 
    'n',
    149,102,  
    1115,162,
    6,2,  
    68, 
    48, 
    13,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn8, 
    keybrd0button8callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b8.Wid,(char *)"QWERT");
  b8.item = -1;
  BUT_STR  *butn9=NULL; 
  butn9= (BUT_STR *)malloc(sizeof(BUT_STR)*11);
  butn9[0].sw=1;
  strcpy(butn9[0].title,(char *)"");
  butn9[0].xpmn=NULL;
  butn9[0].xpmp=NULL;
  butn9[0].xpmh=NULL;
  butn9[0].bkgr=-1;
  butn9[0].butncode='';
  butn9[1].sw=1;
  strcpy(butn9[1].title,(char *)"");
  butn9[1].xpmn=NULL;
  butn9[1].xpmp=NULL;
  butn9[1].xpmh=NULL;
  butn9[1].bkgr=-1;
  butn9[1].butncode='';
  butn9[2].sw=1;
  strcpy(butn9[2].title,(char *)"");
  butn9[2].xpmn=NULL;
  butn9[2].xpmp=NULL;
  butn9[2].xpmh=NULL;
  butn9[2].bkgr=-1;
  butn9[2].butncode='';
  butn9[3].sw=1;
  strcpy(butn9[3].title,(char *)"");
  butn9[3].xpmn=NULL;
  butn9[3].xpmp=NULL;
  butn9[3].xpmh=NULL;
  butn9[3].bkgr=-1;
  butn9[3].butncode='';
  butn9[4].sw=1;
  strcpy(butn9[4].title,(char *)"");
  butn9[4].xpmn=NULL;
  butn9[4].xpmp=NULL;
  butn9[4].xpmh=NULL;
  butn9[4].bkgr=-1;
  butn9[4].butncode='';
  butn9[5].sw=1;
  strcpy(butn9[5].title,(char *)"");
  butn9[5].xpmn=NULL;
  butn9[5].xpmp=NULL;
  butn9[5].xpmh=NULL;
  butn9[5].bkgr=-1;
  butn9[5].butncode='';
  butn9[6].sw=1;
  strcpy(butn9[6].title,(char *)"");
  butn9[6].xpmn=NULL;
  butn9[6].xpmp=NULL;
  butn9[6].xpmh=NULL;
  butn9[6].bkgr=-1;
  butn9[6].butncode='';
  butn9[7].sw=1;
  strcpy(butn9[7].title,(char *)"");
  butn9[7].xpmn=NULL;
  butn9[7].xpmp=NULL;
  butn9[7].xpmh=NULL;
  butn9[7].bkgr=-1;
  butn9[7].butncode='';
  butn9[8].sw=1;
  strcpy(butn9[8].title,(char *)"");
  butn9[8].xpmn=NULL;
  butn9[8].xpmp=NULL;
  butn9[8].xpmh=NULL;
  butn9[8].bkgr=-1;
  butn9[8].butncode='';
  butn9[9].sw=1;
  strcpy(butn9[9].title,(char *)"");
  butn9[9].xpmn=NULL;
  butn9[9].xpmp=NULL;
  butn9[9].xpmh=NULL;
  butn9[9].bkgr=-1;
  butn9[9].butncode='';
  butn9[10].sw=1;
  strcpy(butn9[10].title,(char *)"");
  butn9[10].xpmn=NULL;
  butn9[10].xpmp=NULL;
  butn9[10].xpmh=NULL;
  butn9[10].bkgr=-1;
  butn9[10].butncode='';
  DIN b9 = { 
    'n',
    168,150,  
    986,220,
    6,2,  
    68, 
    48, 
    11,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn9, 
    keybrd0button9callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b9.Wid,(char *)"ASDF");
  b9.item = -1;
  BUT_STR  *butn10=NULL; 
  butn10= (BUT_STR *)malloc(sizeof(BUT_STR)*5);
  butn10[0].sw=1;
  strcpy(butn10[0].title,(char *)"");
  butn10[0].xpmn=NULL;
  butn10[0].xpmp=NULL;
  butn10[0].xpmh=NULL;
  butn10[0].bkgr=-1;
  butn10[0].butncode='';
  butn10[1].sw=1;
  strcpy(butn10[1].title,(char *)"");
  butn10[1].xpmn=NULL;
  butn10[1].xpmp=NULL;
  butn10[1].xpmh=NULL;
  butn10[1].bkgr=-1;
  butn10[1].butncode='';
  butn10[2].sw=1;
  strcpy(butn10[2].title,(char *)"");
  butn10[2].xpmn=NULL;
  butn10[2].xpmp=NULL;
  butn10[2].xpmh=NULL;
  butn10[2].bkgr=-1;
  butn10[2].butncode='';
  butn10[3].sw=1;
  strcpy(butn10[3].title,(char *)"");
  butn10[3].xpmn=NULL;
  butn10[3].xpmp=NULL;
  butn10[3].xpmh=NULL;
  butn10[3].bkgr=-1;
  butn10[3].butncode='';
  butn10[4].sw=1;
  strcpy(butn10[4].title,(char *)"");
  butn10[4].xpmn=NULL;
  butn10[4].xpmp=NULL;
  butn10[4].xpmh=NULL;
  butn10[4].bkgr=-1;
  butn10[4].butncode='';
  DIN b10 = { 
    'n',
    213,198,  
    561,274,
    6,2,  
    68, 
    48, 
    5,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn10, 
    keybrd0button10callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b10.Wid,(char *)"ZXCV");
  b10.item = -1;
  BUT_STR  *butn11=NULL; 
  butn11= (BUT_STR *)malloc(sizeof(BUT_STR)*5);
  butn11[0].sw=1;
  strcpy(butn11[0].title,(char *)"");
  butn11[0].xpmn=NULL;
  butn11[0].xpmp=NULL;
  butn11[0].xpmh=NULL;
  butn11[0].bkgr=-1;
  butn11[0].butncode='';
  butn11[1].sw=1;
  strcpy(butn11[1].title,(char *)"");
  butn11[1].xpmn=NULL;
  butn11[1].xpmp=NULL;
  butn11[1].xpmh=NULL;
  butn11[1].bkgr=-1;
  butn11[1].butncode='';
  butn11[2].sw=1;
  strcpy(butn11[2].title,(char *)"");
  butn11[2].xpmn=NULL;
  butn11[2].xpmp=NULL;
  butn11[2].xpmh=NULL;
  butn11[2].bkgr=-1;
  butn11[2].butncode='';
  butn11[3].sw=1;
  strcpy(butn11[3].title,(char *)"");
  butn11[3].xpmn=NULL;
  butn11[3].xpmp=NULL;
  butn11[3].xpmh=NULL;
  butn11[3].bkgr=-1;
  butn11[3].butncode='';
  butn11[4].sw=1;
  strcpy(butn11[4].title,(char *)"");
  butn11[4].xpmn=NULL;
  butn11[4].xpmp=NULL;
  butn11[4].xpmh=NULL;
  butn11[4].bkgr=-1;
  butn11[4].butncode='';
  DIN b11 = { 
    'n',
    674,198,  
    1052,274,
    6,2,  
    68, 
    48, 
    5,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn11, 
    keybrd0button11callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b11.Wid,(char *)"NMETC");
  b11.item = -1;
  BUT_STR  *butn12=NULL; 
  butn12= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn12[0].sw=1;
  strcpy(butn12[0].title,(char *)"CapsUnlock");
  butn12[0].xpmn=NULL;
  butn12[0].xpmp=NULL;
  butn12[0].xpmh=NULL;
  butn12[0].bkgr=-1;
  butn12[0].butncode='';
  DIN b12 = { 
    'n',
    53,150,  
    171,220,
    4,2,  
    110, 
    48, 
    1,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn12, 
    keybrd0button12callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b12.Wid,(char *)"CAPSLOCK");
  b12.item = -1;
  BUT_STR  *butn13=NULL; 
  butn13= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn13[0].sw=1;
  strcpy(butn13[0].title,(char *)"Backspace");
  butn13[0].xpmn=NULL;
  butn13[0].xpmp=NULL;
  butn13[0].xpmh=NULL;
  butn13[0].bkgr=-1;
  butn13[0].butncode='';
  DIN b13 = { 
    'n',
    1098,44,  
    1211,104,
    4,2,  
    103, 
    48, 
    1,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn13, 
    keybrd0button13callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b13.Wid,(char *)"backspace");
  b13.item = -1;
  BUT_STR  *butn14=NULL; 
  butn14= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn14[0].sw=1;
  strcpy(butn14[0].title,(char *)"Left");
  butn14[0].xpmn=NULL;
  butn14[0].xpmp=NULL;
  butn14[0].xpmh=NULL;
  butn14[0].bkgr=-1;
  butn14[0].butncode='';
  butn14[1].sw=1;
  strcpy(butn14[1].title,(char *)"Right");
  butn14[1].xpmn=NULL;
  butn14[1].xpmp=NULL;
  butn14[1].xpmh=NULL;
  butn14[1].bkgr=-1;
  butn14[1].butncode='';
  DIN b14 = { 
    'n',
    1046,198,  
    1211,274,
    5,2,  
    74, 
    48, 
    2,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn14, 
    keybrd0button14callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b14.Wid,(char *)"leftright");
  b14.item = -1;
  BUT_STR  *butn15=NULL; 
  butn15= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn15[0].sw=1;
  strcpy(butn15[0].title,(char *)"Esc");
  butn15[0].xpmn=NULL;
  butn15[0].xpmp=NULL;
  butn15[0].xpmh=NULL;
  butn15[0].bkgr=-1;
  butn15[0].butncode='';
  DIN b15 = { 
    'n',
    53,44,  
    140,104,
    4,2,  
    77, 
    48, 
    1,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn15, 
    keybrd0button15callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b15.Wid,(char *)"escape");
  b15.item = -1;
  BUT_STR  *butn16=NULL; 
  butn16= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn16[0].sw=1;
  strcpy(butn16[0].title,(char *)"Enter");
  butn16[0].xpmn=NULL;
  butn16[0].xpmp=NULL;
  butn16[0].xpmh=NULL;
  butn16[0].bkgr=-1;
  butn16[0].butncode='';
  DIN b16 = { 
    'n',
    1090,150,  
    1211,220,
    4,2,  
    110, 
    48, 
    1,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn16, 
    keybrd0button16callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b16.Wid,(char *)"enter");
  b16.item = -1;
  BUT_STR  *butn17=NULL; 
  butn17= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn17[0].sw=1;
  strcpy(butn17[0].title,(char *)"Tab");
  butn17[0].xpmn=NULL;
  butn17[0].xpmp=NULL;
  butn17[0].xpmh=NULL;
  butn17[0].bkgr=-1;
  butn17[0].butncode='';
  DIN b17 = { 
    'n',
    984,150,  
    1090,220,
    4,2,  
    110, 
    48, 
    1,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn17, 
    keybrd0button17callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b17.Wid,(char *)"tab");
  b17.item = -1;
  BUT_STR  *butn18=NULL; 
  butn18= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn18[0].sw=1;
  strcpy(butn18[0].title,(char *)"Home");
  butn18[0].xpmn=NULL;
  butn18[0].xpmp=NULL;
  butn18[0].xpmh=NULL;
  butn18[0].bkgr=-1;
  butn18[0].butncode='';
  DIN b18 = { 
    'n',
    53,102,  
    154,162,
    4,2,  
    91, 
    48, 
    1,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn18, 
    keybrd0button18callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b18.Wid,(char *)"home");
  b18.item = -1;
  BUT_STR  *butn19=NULL; 
  butn19= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn19[0].sw=1;
  strcpy(butn19[0].title,(char *)"End");
  butn19[0].xpmn=NULL;
  butn19[0].xpmp=NULL;
  butn19[0].xpmh=NULL;
  butn19[0].bkgr=-1;
  butn19[0].butncode='';
  DIN b19 = { 
    'n',
    1112,102,  
    1211,162,
    4,2,  
    110, 
    48, 
    1,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn19, 
    keybrd0button19callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b19.Wid,(char *)"end");
  b19.item = -1;
  BUT_STR  *butn20=NULL; 
  butn20= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn20[0].sw=1;
  strcpy(butn20[0].title,(char *)"Shift");
  butn20[0].xpmn=NULL;
  butn20[0].xpmp=NULL;
  butn20[0].xpmh=NULL;
  butn20[0].bkgr=-1;
  butn20[0].butncode='';
  DIN b20 = { 
    'n',
    53,198,  
    185,274,
    4,2,  
    135, 
    48, 
    1,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn20, 
    keybrd0button20callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b20.Wid,(char *)"shift");
  b20.item = -1;
  BUT_STR  *butn21=NULL; 
  butn21= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn21[0].sw=1;
  strcpy(butn21[0].title,(char *)"Space");
  butn21[0].xpmn=NULL;
  butn21[0].xpmp=NULL;
  butn21[0].xpmh=NULL;
  butn21[0].bkgr=-1;
  butn21[0].butncode='';
  DIN b21 = { 
    'n',
    585,198,  
    676,274,
    4,2,  
    113, 
    48, 
    1,1, 
    2,0.000000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn21, 
    keybrd0button21callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b21.Wid,(char *)"space");
  b21.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+23));
  d =dtmp+i; 
  d[22].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIP));
  *d[0].p = p0;
  d[0].p->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  keybrd0button1init(d[1].N,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIN));
  *d[2].N = b2;
  d[2].N->item = -1;
  keybrd0button2init(d[2].N,pt) ;
  d[3].t = (DIT *)malloc(sizeof(DIN));
  *d[3].N = b3;
  d[3].N->item = -1;
  keybrd0button3init(d[3].N,pt) ;
  d[4].t = (DIT *)malloc(sizeof(DIN));
  *d[4].N = b4;
  d[4].N->item = -1;
  keybrd0button4init(d[4].N,pt) ;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  *d[5].N = b5;
  d[5].N->item = -1;
  keybrd0button5init(d[5].N,pt) ;
  d[6].t = (DIT *)malloc(sizeof(DIN));
  *d[6].N = b6;
  d[6].N->item = -1;
  keybrd0button6init(d[6].N,pt) ;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  *d[7].N = b7;
  d[7].N->item = -1;
  keybrd0button7init(d[7].N,pt) ;
  d[8].t = (DIT *)malloc(sizeof(DIN));
  *d[8].N = b8;
  d[8].N->item = -1;
  keybrd0button8init(d[8].N,pt) ;
  d[9].t = (DIT *)malloc(sizeof(DIN));
  *d[9].N = b9;
  d[9].N->item = -1;
  keybrd0button9init(d[9].N,pt) ;
  d[10].t = (DIT *)malloc(sizeof(DIN));
  *d[10].N = b10;
  d[10].N->item = -1;
  keybrd0button10init(d[10].N,pt) ;
  d[11].t = (DIT *)malloc(sizeof(DIN));
  *d[11].N = b11;
  d[11].N->item = -1;
  keybrd0button11init(d[11].N,pt) ;
  d[12].t = (DIT *)malloc(sizeof(DIN));
  *d[12].N = b12;
  d[12].N->item = -1;
  keybrd0button12init(d[12].N,pt) ;
  d[13].t = (DIT *)malloc(sizeof(DIN));
  *d[13].N = b13;
  d[13].N->item = -1;
  keybrd0button13init(d[13].N,pt) ;
  d[14].t = (DIT *)malloc(sizeof(DIN));
  *d[14].N = b14;
  d[14].N->item = -1;
  keybrd0button14init(d[14].N,pt) ;
  d[15].t = (DIT *)malloc(sizeof(DIN));
  *d[15].N = b15;
  d[15].N->item = -1;
  keybrd0button15init(d[15].N,pt) ;
  d[16].t = (DIT *)malloc(sizeof(DIN));
  *d[16].N = b16;
  d[16].N->item = -1;
  keybrd0button16init(d[16].N,pt) ;
  d[17].t = (DIT *)malloc(sizeof(DIN));
  *d[17].N = b17;
  d[17].N->item = -1;
  keybrd0button17init(d[17].N,pt) ;
  d[18].t = (DIT *)malloc(sizeof(DIN));
  *d[18].N = b18;
  d[18].N->item = -1;
  keybrd0button18init(d[18].N,pt) ;
  d[19].t = (DIT *)malloc(sizeof(DIN));
  *d[19].N = b19;
  d[19].N->item = -1;
  keybrd0button19init(d[19].N,pt) ;
  d[20].t = (DIT *)malloc(sizeof(DIN));
  *d[20].N = b20;
  d[20].N->item = -1;
  keybrd0button20init(d[20].N,pt) ;
  d[21].t = (DIT *)malloc(sizeof(DIN));
  *d[21].N = b21;
  d[21].N->item = -1;
  keybrd0button21init(d[21].N,pt) ;
  d[22].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 
static int CreateKeybrdGrps(DIALOG *D,KEYBRD *Kbrd,int offset,int Vis,int xo,int yo) {
   int xm=10000,ym=10000;
   int i;
   DIA *d;
   d = (DIA *)(D->d);
   i = offset;
#if 1
   while(d[i].t != NULL) {
     if(xm > (d[i].t->x1) ) xm = d[i].t->x1;
     if(ym > (d[i].t->y1) ) ym = d[i].t->y1;
     i++;
   }
//   kgShiftGrp(D,GrpId,xo-43,yo-28);
//   kgShiftGrp(D,Kbrd->GrpId,xo-xm,yo-ym);
#endif
   Kbrd->kbtype=0;
   Kbrd->grp3 = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->grp3,d[0+offset].t);
   kgAddtoGrp(D,Kbrd->grp3,d[13+offset].t);
   kgAddtoGrp(D,Kbrd->grp3,d[14+offset].t);
   kgAddtoGrp(D,Kbrd->grp3,d[15+offset].t);
   kgAddtoGrp(D,Kbrd->grp3,d[16+offset].t);
   kgAddtoGrp(D,Kbrd->grp3,d[17+offset].t);
   kgAddtoGrp(D,Kbrd->grp3,d[18+offset].t);
   kgAddtoGrp(D,Kbrd->grp3,d[19+offset].t);
   kgAddtoGrp(D,Kbrd->grp3,d[20+offset].t);
   kgAddtoGrp(D,Kbrd->grp3,d[21+offset].t);
   Kbrd->offgrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->offgrp,d[1+offset].t);
   Kbrd->ongrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->ongrp,d[7+offset].t);
   Kbrd->grp1 = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->grp1,d[6+offset].t);
   Kbrd->grp2 = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->grp2,d[12+offset].t);
   Kbrd->sgrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->sgrp,d[2+offset].t);
   kgAddtoGrp(D,Kbrd->sgrp,d[3+offset].t);
   kgAddtoGrp(D,Kbrd->sgrp,d[4+offset].t);
   kgAddtoGrp(D,Kbrd->sgrp,d[5+offset].t);
   Kbrd->cgrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->cgrp,d[8+offset].t);
   kgAddtoGrp(D,Kbrd->cgrp,d[9+offset].t);
   kgAddtoGrp(D,Kbrd->cgrp,d[10+offset].t);
   kgAddtoGrp(D,Kbrd->cgrp,d[11+offset].t);
   Kbrd->CurWid = -1;
   Kbrd->Vis = Vis;
   Kbrd->D = D;
   Kbrd->ShiftPress=0;
   Kbrd->CapsLock=0;
   if(Vis==0) {
    kgSetGrpVisibility(D,Kbrd->GrpId,0);
   }
   else {
    kgSetGrpVisibility(D,Kbrd->cgrp,0);
    kgSetGrpVisibility(D,Kbrd->ongrp,0);
    kgSetGrpVisibility(D,Kbrd->grp2,0);
    kgSetGrpVisibility(D,Kbrd->grp3,1);
    kgSetGrpVisibility(D,Kbrd->sgrp,1);
    kgSetGrpVisibility(D,Kbrd->offgrp,1);
    kgSetGrpVisibility(D,Kbrd->grp1,1);
   }
   return 1;
}
/* One can also use the following code to add Widgets to an existing Dialog */

int kgMakeDefaultKeybrd0(DIALOG *D,int xo,int yo,int Vis) {
   return 1;
}
static int ConvertColor(int clr,int no) {
   int r,g,b;
   int fac;
   if(clr >=0 ) {
      kgGetDefaultRGB(clr,&r,&g,&b);
   }
   else {
     clr = -clr;
     b = clr%1000;
     fac = clr/1000;
     g = fac%1000;
     r= fac/1000;
   }
   kgDefineColor(no,(unsigned char)r,(unsigned char)g,(unsigned char)b);
   return no;
}

int kgMakeKeybrd0(DIALOG *D,int Bx,int By,int Xg,int Yg,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency,int bodr,float Brfac) {

   int GrpId;
   WIDGETGRP *Gpt;
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
    int i=0,offset;
   int Red,Green,Blue;
   DIA *dtmp,*d;
   Gclr gc;
   int xo=0,yo=0;
   int Bodr;
   int Bxmin,Bymin;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)Malloc(sizeof(KEYBRD));
   kgInitGm();
   D->Kbrd = Kbrd;
   Kbrd->D = D;
   Kbrd->TargetWindow = NULL;
   Kbrd->XpmList = NULL;
   Kbrd->Thds=NULL;
   Kbrd->Brfac =Brfac;
   gc = D->gc;
   Kbrd->FillClr = ConvertColor(bkgrclr,80);
   D->wc=NULL;
   Btype=btype;
   Bfont=bfont;
   Bclr = ConvertColor(fontclr,81);
   Kbrd->trans = transparency;
   Btrans = transparency;
   ButClr = ConvertColor(butclr,82);
   if (By<30) strcpy(Kbrd->Sfac,"11");
   else if(By < 42) strcpy(Kbrd->Sfac,"54");
        else strncpy(Kbrd->Sfac,Sfac,3);
   Kbrd->Sfac[2]='\0';
   Kbrd->Bclr = Bclr;
   Kbrd->Bfont = Bfont;
   Kbrd->kbtype=0;
   Kbrd->Btype=btype;
   Kbrd->ButClr = ButClr;
   Rfac = rfac;
   Kbrd->Rfac = rfac;
   Kbrd->Bx = Bx;
   Kbrd->By = By;
   Kbrd->Xg = Xg;
   Kbrd->Yg = Yg;
   Bodr = bodr;
#if 1
    if(Bodr < (Xg+4) ) Bodr = Xg+4;
    if(Bodr < (Yg+4) ) Bodr = Yg+4;
    Bxmin = 0.2*Bx+Xg;
    Bymin = 0.2*By+Yg;
    if(Bodr < Bxmin ) Bodr = Bxmin;
    if(Bodr < Bymin ) Bodr = Bymin;
#endif
   Kbrd->Bodr = Bodr;
   Kbrd->Bclr = Bclr;
   Kbrd->Bfont = Bfont;
   Kbrd->kbtype=0;
   Kbrd->Btype=btype;
   Rfac = rfac;
   Kbrd->Rfac = rfac;
   dtmp = D->d;
   pt = Kbrd;
   i=0;
   if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
   offset = i;
   Kbrd->Thds= NULL;
#ifdef D_TH
   if(Kbrd->Btype ==  0) {
     void *Thds;
     Thds = OpenThreads(getCores());
     Kbrd->Thds = Thds;
     GrpId = keybrd0Group(D,v,Kbrd);
     WaitThreads(Thds);
     CloseThreads(Thds);
     Kbrd->Thds= NULL;
   }
   else {
     GrpId = keybrd0Group(D,v,pt);
   }
#else
   GrpId = keybrd0Group(D,v,pt);
#endif
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   Kbrd->GrpId = GrpId;
   Modifykeybrd0(D,offset);
   CreateKeybrdGrps(D,Kbrd,offset,Vis,xo,yo);
   kgSetGrpCleanup(D,GrpId,keybrd0cleanup);
   return GrpId;

}
int keybrd0( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = keybrd0Group(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 21;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 1315;    /*  Length of Dialog */
  D.yl = 392;    /*  Width  of Dialog */
  D.Initfun = keybrd0init;    /*   init fuction for Dialog */
  D.Cleanupfun = keybrd0cleanup;    /*   init fuction for Dialog */
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
  D.Callback = keybrd0CallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = keybrd0ResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = keybrd0WaitCallBack;  /*  Wait callback */
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
//  Modifykeybrd0Gc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *Runkeybrd0(void *arg) {
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   keybrd0(NULL,v,pt );
   return NULL;
}
