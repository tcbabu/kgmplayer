#include "kulina.h"
#include "skeybrdCallbacks.h"
static int RED=120,GREEN=120,BLUE=120;
static int Btype=5;
static float  Rfac=0.5 ;
//#define Btype 8
//#define Brdr  0

//#define Btype 8
//#define Btype 7
//#define Btype 6
//#define Btype 5
//#define Btype 4
//#define Btype 2
#define Sfac1 "45"
#define Sfac2 "11"
static char BUFF[100];
static char Sfac[]="54";
static int Bfont=16,Bclr=0;
static int FillClr=101;
static float Btrans=0.1;
static unsigned char R=230,G=60,B=0;
static int ButClr=-1;

int kgShowKeybrd1(void *Tmp) {
   DIALOG *D;
   KEYBRD *Kbrd;
   D = (DIALOG *)Tmp;
   Kbrd = D->Kbrd;
   if(Kbrd->Vis) return 0;
   kgSetGrpVisibility(D,Kbrd->cgrp,0);
   kgSetGrpVisibility(D,Kbrd->symgrp,0);
   kgSetGrpVisibility(D,Kbrd->sgrp,1);
   Kbrd->ShiftPress=0;
   Kbrd->CapsLock=0;
   if(D->wc != NULL) {
     kgUpdateGrp(D,Kbrd->sgrp);
     kgUpdateOn(D);
   }
   Kbrd->Vis = 1;
   return Kbrd->GrpId;
}

int kgHideKeybrd1(void *Tmp) {
   DIALOG *D;
   KEYBRD *Kbrd;
   D = (DIALOG *)Tmp;
   Kbrd = D->Kbrd;
   if(Kbrd->Vis==0) return 0;
   kgSetGrpVisibility(D,Kbrd->GrpId,0);
   if(D->wc != NULL) {
     kgUpdateGrp(D,Kbrd->GrpId);
     kgUpdateOn(D);
   }
   Kbrd->ShiftPress=0;
   Kbrd->CapsLock=0;
   Kbrd->Vis = 0;
   return Kbrd->GrpId;
}

static char *kgButtonTitle(char * str) {
//  sprintf(BUFF,"!h15!d!h51!z%-s!f%2.2d!c%2.2d%-s",Sfac,Bfont,Bclr,str);
  sprintf(BUFF,"!z%-s!f%2.2d!c%2.2d%-s",Sfac,Bfont,Bclr,str);
//  printf("%s\n",BUFF);
  return BUFF;
}
static char *Procpy(char *des,char *src) {
   strcpy(des,kgButtonTitle(src));
   return des;
}

static void *MakeLightImage(int xl,int yl,int r,int g,int b,float fac) {
   void *Img,*fid,*Img1;
   float h,s,v,rf,gf,bf,vm;
   fid = kgInitImage(xl,yl,8);
   kgUserFrame(fid,-2.,-2.,(float)xl+2,(float)yl+2);
   kgChangeColor(fid,551,(int)r,(int)g,(int)b);
   kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,
                        (float)xl+2,(float)yl+2,0,551,fac);
   Img = kgGetSharpImage(fid);
   kgCloseImage(fid);
   return Img;
}
void ModifyskeybrdGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int skeybrdGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  int Red,Green,Blue;
  DIA *d=NULL,*dtmp;
  char *xpm0;// Pixmap info
#if 0
  xpm0 = (char *)malloc(3);
  strcpy(xpm0, (char *)"##");
#else
  kgGetDefaultRGB(FillClr,&Red,&Green,&Blue);
  xpm0 = (char *)MakeLightImage( 502,165,Red,Green,Blue,0.0);
#endif
  DIP p0 = { 
    'p',
    14,7,  
    516,172,  
    (void *)xpm0,
    FillClr, /* bkgr colour */ 
      1,0,Btrans /* border hide transparency*/ 
  };
  strcpy(p0.Wid,(char *)"Keybrdbkgr");
  p0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn1[0].sw=1;
  Procpy(butn1[0].title,(char *)"End");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=ButClr;
  butn1[0].butncode=0;
  butn1[1].sw=1;
  Procpy(butn1[1].title,(char *)"Backspace");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=ButClr;
  butn1[1].butncode=0;
  butn1[2].sw=1;
  Procpy(butn1[2].title,(char *)"Enter");
  butn1[2].xpmn=NULL;
  butn1[2].xpmp=NULL;
  butn1[2].xpmh=NULL;
  butn1[2].bkgr=ButClr;
  butn1[2].butncode=0;
  butn1[3].sw=1;
  Procpy(butn1[3].title,(char *)"Space");
  butn1[3].xpmn=NULL;
  butn1[3].xpmp=NULL;
  butn1[3].xpmh=NULL;
  butn1[3].bkgr=ButClr;
  butn1[3].butncode=0;
  DIN b1 = { 
    'n',
    441,12,  
    502,164,
    1,1,
    56, 
    36, 
    1,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    skeybrdbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"enter+");
  b1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn2[0].sw=1;
  Procpy(butn2[0].title,(char *)"Home");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=ButClr;
  butn2[0].butncode=0;
  butn2[1].sw=1;
  Procpy(butn2[1].title,(char *)"Escape");
  butn2[1].xpmn=NULL;
  butn2[1].xpmp=NULL;
  butn2[1].xpmh=NULL;
  butn2[1].bkgr=ButClr;
  butn2[1].butncode=0;
  butn2[2].sw=1;
  Procpy(butn2[2].title,(char *)"CapsLock");
  butn2[2].xpmn=NULL;
  butn2[2].xpmp=NULL;
  butn2[2].xpmh=NULL;
  butn2[2].bkgr=ButClr;
  butn2[2].butncode=0;
  butn2[3].sw=1;
  Procpy(butn2[3].title,(char *)"@#$%");
  butn2[3].xpmn=NULL;
  butn2[3].xpmp=NULL;
  butn2[3].xpmh=NULL;
  butn2[3].bkgr=ButClr;
  butn2[3].butncode=0;
  DIN b2 = { 
    'n',
    18,12,  
    75,164,
    1,1,
    55, 
    36, 
    1,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    skeybrdbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b2.Wid,(char *)"capslocketc");
  b2.item = -1;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*40);
  butn3[0].sw=1;
  Procpy(butn3[0].title,(char *)"1");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=ButClr;
  butn3[0].butncode=0;
  butn3[1].sw=1;
  Procpy(butn3[1].title,(char *)"2");
  butn3[1].xpmn=NULL;
  butn3[1].xpmp=NULL;
  butn3[1].xpmh=NULL;
  butn3[1].bkgr=ButClr;
  butn3[1].butncode=0;
  butn3[2].sw=1;
  Procpy(butn3[2].title,(char *)"3");
  butn3[2].xpmn=NULL;
  butn3[2].xpmp=NULL;
  butn3[2].xpmh=NULL;
  butn3[2].bkgr=ButClr;
  butn3[2].butncode=0;
  butn3[3].sw=1;
  Procpy(butn3[3].title,(char *)"4");
  butn3[3].xpmn=NULL;
  butn3[3].xpmp=NULL;
  butn3[3].xpmh=NULL;
  butn3[3].bkgr=ButClr;
  butn3[3].butncode=0;
  butn3[4].sw=1;
  Procpy(butn3[4].title,(char *)"5");
  butn3[4].xpmn=NULL;
  butn3[4].xpmp=NULL;
  butn3[4].xpmh=NULL;
  butn3[4].bkgr=ButClr;
  butn3[4].butncode=0;
  butn3[5].sw=1;
  Procpy(butn3[5].title,(char *)"6");
  butn3[5].xpmn=NULL;
  butn3[5].xpmp=NULL;
  butn3[5].xpmh=NULL;
  butn3[5].bkgr=ButClr;
  butn3[5].butncode=0;
  butn3[6].sw=1;
  Procpy(butn3[6].title,(char *)"7");
  butn3[6].xpmn=NULL;
  butn3[6].xpmp=NULL;
  butn3[6].xpmh=NULL;
  butn3[6].bkgr=ButClr;
  butn3[6].butncode=0;
  butn3[7].sw=1;
  Procpy(butn3[7].title,(char *)"8");
  butn3[7].xpmn=NULL;
  butn3[7].xpmp=NULL;
  butn3[7].xpmh=NULL;
  butn3[7].bkgr=ButClr;
  butn3[7].butncode=0;
  butn3[8].sw=1;
  Procpy(butn3[8].title,(char *)"9");
  butn3[8].xpmn=NULL;
  butn3[8].xpmp=NULL;
  butn3[8].xpmh=NULL;
  butn3[8].bkgr=ButClr;
  butn3[8].butncode=0;
  butn3[9].sw=1;
  Procpy(butn3[9].title,(char *)"0");
  butn3[9].xpmn=NULL;
  butn3[9].xpmp=NULL;
  butn3[9].xpmh=NULL;
  butn3[9].bkgr=ButClr;
  butn3[9].butncode=0;
  butn3[10].sw=1;
  Procpy(butn3[10].title,(char *)"a");
  butn3[10].xpmn=NULL;
  butn3[10].xpmp=NULL;
  butn3[10].xpmh=NULL;
  butn3[10].bkgr=ButClr;
  butn3[10].butncode=0;
  butn3[11].sw=1;
  Procpy(butn3[11].title,(char *)"b");
  butn3[11].xpmn=NULL;
  butn3[11].xpmp=NULL;
  butn3[11].xpmh=NULL;
  butn3[11].bkgr=ButClr;
  butn3[11].butncode=0;
  butn3[12].sw=1;
  Procpy(butn3[12].title,(char *)"c");
  butn3[12].xpmn=NULL;
  butn3[12].xpmp=NULL;
  butn3[12].xpmh=NULL;
  butn3[12].bkgr=ButClr;
  butn3[12].butncode=0;
  butn3[13].sw=1;
  Procpy(butn3[13].title,(char *)"d");
  butn3[13].xpmn=NULL;
  butn3[13].xpmp=NULL;
  butn3[13].xpmh=NULL;
  butn3[13].bkgr=ButClr;
  butn3[13].butncode=0;
  butn3[14].sw=1;
  Procpy(butn3[14].title,(char *)"e");
  butn3[14].xpmn=NULL;
  butn3[14].xpmp=NULL;
  butn3[14].xpmh=NULL;
  butn3[14].bkgr=ButClr;
  butn3[14].butncode=0;
  butn3[15].sw=1;
  Procpy(butn3[15].title,(char *)"f");
  butn3[15].xpmn=NULL;
  butn3[15].xpmp=NULL;
  butn3[15].xpmh=NULL;
  butn3[15].bkgr=ButClr;
  butn3[15].butncode=0;
  butn3[16].sw=1;
  Procpy(butn3[16].title,(char *)"g");
  butn3[16].xpmn=NULL;
  butn3[16].xpmp=NULL;
  butn3[16].xpmh=NULL;
  butn3[16].bkgr=ButClr;
  butn3[16].butncode=0;
  butn3[17].sw=1;
  Procpy(butn3[17].title,(char *)"h");
  butn3[17].xpmn=NULL;
  butn3[17].xpmp=NULL;
  butn3[17].xpmh=NULL;
  butn3[17].bkgr=ButClr;
  butn3[17].butncode=0;
  butn3[18].sw=1;
  Procpy(butn3[18].title,(char *)"i");
  butn3[18].xpmn=NULL;
  butn3[18].xpmp=NULL;
  butn3[18].xpmh=NULL;
  butn3[18].bkgr=ButClr;
  butn3[18].butncode=0;
  butn3[19].sw=1;
  Procpy(butn3[19].title,(char *)"j");
  butn3[19].xpmn=NULL;
  butn3[19].xpmp=NULL;
  butn3[19].xpmh=NULL;
  butn3[19].bkgr=ButClr;
  butn3[19].butncode=0;
  butn3[20].sw=1;
  Procpy(butn3[20].title,(char *)"k");
  butn3[20].xpmn=NULL;
  butn3[20].xpmp=NULL;
  butn3[20].xpmh=NULL;
  butn3[20].bkgr=ButClr;
  butn3[20].butncode=0;
  butn3[21].sw=1;
  Procpy(butn3[21].title,(char *)"l");
  butn3[21].xpmn=NULL;
  butn3[21].xpmp=NULL;
  butn3[21].xpmh=NULL;
  butn3[21].bkgr=ButClr;
  butn3[21].butncode=0;
  butn3[22].sw=1;
  Procpy(butn3[22].title,(char *)"m");
  butn3[22].xpmn=NULL;
  butn3[22].xpmp=NULL;
  butn3[22].xpmh=NULL;
  butn3[22].bkgr=ButClr;
  butn3[22].butncode=0;
  butn3[23].sw=1;
  Procpy(butn3[23].title,(char *)"n");
  butn3[23].xpmn=NULL;
  butn3[23].xpmp=NULL;
  butn3[23].xpmh=NULL;
  butn3[23].bkgr=ButClr;
  butn3[23].butncode=0;
  butn3[24].sw=1;
  Procpy(butn3[24].title,(char *)"o");
  butn3[24].xpmn=NULL;
  butn3[24].xpmp=NULL;
  butn3[24].xpmh=NULL;
  butn3[24].bkgr=ButClr;
  butn3[24].butncode=0;
  butn3[25].sw=1;
  Procpy(butn3[25].title,(char *)"p");
  butn3[25].xpmn=NULL;
  butn3[25].xpmp=NULL;
  butn3[25].xpmh=NULL;
  butn3[25].bkgr=ButClr;
  butn3[25].butncode=0;
  butn3[26].sw=1;
  Procpy(butn3[26].title,(char *)"q");
  butn3[26].xpmn=NULL;
  butn3[26].xpmp=NULL;
  butn3[26].xpmh=NULL;
  butn3[26].bkgr=ButClr;
  butn3[26].butncode=0;
  butn3[27].sw=1;
  Procpy(butn3[27].title,(char *)"r");
  butn3[27].xpmn=NULL;
  butn3[27].xpmp=NULL;
  butn3[27].xpmh=NULL;
  butn3[27].bkgr=ButClr;
  butn3[27].butncode=0;
  butn3[28].sw=1;
  Procpy(butn3[28].title,(char *)"s");
  butn3[28].xpmn=NULL;
  butn3[28].xpmp=NULL;
  butn3[28].xpmh=NULL;
  butn3[28].bkgr=ButClr;
  butn3[28].butncode=0;
  butn3[29].sw=1;
  Procpy(butn3[29].title,(char *)"t");
  butn3[29].xpmn=NULL;
  butn3[29].xpmp=NULL;
  butn3[29].xpmh=NULL;
  butn3[29].bkgr=ButClr;
  butn3[29].butncode=0;
  butn3[30].sw=1;
  Procpy(butn3[30].title,(char *)"u");
  butn3[30].xpmn=NULL;
  butn3[30].xpmp=NULL;
  butn3[30].xpmh=NULL;
  butn3[30].bkgr=ButClr;
  butn3[30].butncode=0;
  butn3[31].sw=1;
  Procpy(butn3[31].title,(char *)"v");
  butn3[31].xpmn=NULL;
  butn3[31].xpmp=NULL;
  butn3[31].xpmh=NULL;
  butn3[31].bkgr=ButClr;
  butn3[31].butncode=0;
  butn3[32].sw=1;
  Procpy(butn3[32].title,(char *)"w");
  butn3[32].xpmn=NULL;
  butn3[32].xpmp=NULL;
  butn3[32].xpmh=NULL;
  butn3[32].bkgr=ButClr;
  butn3[32].butncode=0;
  butn3[33].sw=1;
  Procpy(butn3[33].title,(char *)"x");
  butn3[33].xpmn=NULL;
  butn3[33].xpmp=NULL;
  butn3[33].xpmh=NULL;
  butn3[33].bkgr=ButClr;
  butn3[33].butncode=0;
  butn3[34].sw=1;
  Procpy(butn3[34].title,(char *)"y");
  butn3[34].xpmn=NULL;
  butn3[34].xpmp=NULL;
  butn3[34].xpmh=NULL;
  butn3[34].bkgr=ButClr;
  butn3[34].butncode=0;
  butn3[35].sw=1;
  Procpy(butn3[35].title,(char *)"z");
  butn3[35].xpmn=NULL;
  butn3[35].xpmp=NULL;
  butn3[35].xpmh=NULL;
  butn3[35].bkgr=ButClr;
  butn3[35].butncode=0;
  butn3[36].sw=1;
  Procpy(butn3[36].title,(char *)",");
  butn3[36].xpmn=NULL;
  butn3[36].xpmp=NULL;
  butn3[36].xpmh=NULL;
  butn3[36].bkgr=ButClr;
  butn3[36].butncode=0;
  butn3[37].sw=1;
  Procpy(butn3[37].title,(char *)".");
  butn3[37].xpmn=NULL;
  butn3[37].xpmp=NULL;
  butn3[37].xpmh=NULL;
  butn3[37].bkgr=ButClr;
  butn3[37].butncode=0;
  butn3[38].sw=1;
  Procpy(butn3[38].title,(char *)"/");
  butn3[38].xpmn=NULL;
  butn3[38].xpmp=NULL;
  butn3[38].xpmh=NULL;
  butn3[38].bkgr=ButClr;
  butn3[38].butncode=0;
  butn3[39].sw=1;
  Procpy(butn3[39].title,(char *)"?");
  butn3[39].xpmn=NULL;
  butn3[39].xpmp=NULL;
  butn3[39].xpmh=NULL;
  butn3[39].bkgr=ButClr;
  butn3[39].butncode=0;
  DIN b3 = { 
    'n',
    77,12,  
    442,164,
    1,1,
    36, 
    36, 
    10,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    skeybrdbutton3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b3.Wid,(char *)"smallkeys");
  b3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn4[0].sw=1;
  Procpy(butn4[0].title,(char *)"Home");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=ButClr;
  butn4[0].butncode=0;
  butn4[1].sw=1;
  Procpy(butn4[1].title,(char *)"Escape");
  butn4[1].xpmn=NULL;
  butn4[1].xpmp=NULL;
  butn4[1].xpmh=NULL;
  butn4[1].bkgr=ButClr;
  butn4[1].butncode=0;
  butn4[2].sw=1;
  Procpy(butn4[2].title,(char *)"Capslock");
  butn4[2].xpmn=NULL;
  butn4[2].xpmp=NULL;
  butn4[2].xpmh=NULL;
  butn4[2].bkgr=ButClr;
  butn4[2].butncode=0;
  butn4[3].sw=1;
  Procpy(butn4[3].title,(char *)"@#$%");
  butn4[3].xpmn=NULL;
  butn4[3].xpmp=NULL;
  butn4[3].xpmh=NULL;
  butn4[3].bkgr=ButClr;
  butn4[3].butncode=0;
  DIN b4 = { 
    'n',
    18,12,  
    75,164,
    1,1,
    55, 
    36, 
    1,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    skeybrdbutton4callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b4.Wid,(char *)"CAPSLOCKETC");
  b4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*40);
  butn5[0].sw=1;
  Procpy(butn5[0].title,(char *)"1");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=ButClr;
  butn5[0].butncode=0;
  butn5[1].sw=1;
  Procpy(butn5[1].title,(char *)"2");
  butn5[1].xpmn=NULL;
  butn5[1].xpmp=NULL;
  butn5[1].xpmh=NULL;
  butn5[1].bkgr=ButClr;
  butn5[1].butncode=0;
  butn5[2].sw=1;
  Procpy(butn5[2].title,(char *)"3");
  butn5[2].xpmn=NULL;
  butn5[2].xpmp=NULL;
  butn5[2].xpmh=NULL;
  butn5[2].bkgr=ButClr;
  butn5[2].butncode=0;
  butn5[3].sw=1;
  Procpy(butn5[3].title,(char *)"4");
  butn5[3].xpmn=NULL;
  butn5[3].xpmp=NULL;
  butn5[3].xpmh=NULL;
  butn5[3].bkgr=ButClr;
  butn5[3].butncode=0;
  butn5[4].sw=1;
  Procpy(butn5[4].title,(char *)"5");
  butn5[4].xpmn=NULL;
  butn5[4].xpmp=NULL;
  butn5[4].xpmh=NULL;
  butn5[4].bkgr=ButClr;
  butn5[4].butncode=0;
  butn5[5].sw=1;
  Procpy(butn5[5].title,(char *)"6");
  butn5[5].xpmn=NULL;
  butn5[5].xpmp=NULL;
  butn5[5].xpmh=NULL;
  butn5[5].bkgr=ButClr;
  butn5[5].butncode=0;
  butn5[6].sw=1;
  Procpy(butn5[6].title,(char *)"7");
  butn5[6].xpmn=NULL;
  butn5[6].xpmp=NULL;
  butn5[6].xpmh=NULL;
  butn5[6].bkgr=ButClr;
  butn5[6].butncode=0;
  butn5[7].sw=1;
  Procpy(butn5[7].title,(char *)"8");
  butn5[7].xpmn=NULL;
  butn5[7].xpmp=NULL;
  butn5[7].xpmh=NULL;
  butn5[7].bkgr=ButClr;
  butn5[7].butncode=0;
  butn5[8].sw=1;
  Procpy(butn5[8].title,(char *)"9");
  butn5[8].xpmn=NULL;
  butn5[8].xpmp=NULL;
  butn5[8].xpmh=NULL;
  butn5[8].bkgr=ButClr;
  butn5[8].butncode=0;
  butn5[9].sw=1;
  Procpy(butn5[9].title,(char *)"0");
  butn5[9].xpmn=NULL;
  butn5[9].xpmp=NULL;
  butn5[9].xpmh=NULL;
  butn5[9].bkgr=ButClr;
  butn5[9].butncode=0;
  butn5[10].sw=1;
  Procpy(butn5[10].title,(char *)"A");
  butn5[10].xpmn=NULL;
  butn5[10].xpmp=NULL;
  butn5[10].xpmh=NULL;
  butn5[10].bkgr=ButClr;
  butn5[10].butncode=0;
  butn5[11].sw=1;
  Procpy(butn5[11].title,(char *)"B");
  butn5[11].xpmn=NULL;
  butn5[11].xpmp=NULL;
  butn5[11].xpmh=NULL;
  butn5[11].bkgr=ButClr;
  butn5[11].butncode=0;
  butn5[12].sw=1;
  Procpy(butn5[12].title,(char *)"C");
  butn5[12].xpmn=NULL;
  butn5[12].xpmp=NULL;
  butn5[12].xpmh=NULL;
  butn5[12].bkgr=ButClr;
  butn5[12].butncode=0;
  butn5[13].sw=1;
  Procpy(butn5[13].title,(char *)"D");
  butn5[13].xpmn=NULL;
  butn5[13].xpmp=NULL;
  butn5[13].xpmh=NULL;
  butn5[13].bkgr=ButClr;
  butn5[13].butncode=0;
  butn5[14].sw=1;
  Procpy(butn5[14].title,(char *)"E");
  butn5[14].xpmn=NULL;
  butn5[14].xpmp=NULL;
  butn5[14].xpmh=NULL;
  butn5[14].bkgr=ButClr;
  butn5[14].butncode=0;
  butn5[15].sw=1;
  Procpy(butn5[15].title,(char *)"F");
  butn5[15].xpmn=NULL;
  butn5[15].xpmp=NULL;
  butn5[15].xpmh=NULL;
  butn5[15].bkgr=ButClr;
  butn5[15].butncode=0;
  butn5[16].sw=1;
  Procpy(butn5[16].title,(char *)"G");
  butn5[16].xpmn=NULL;
  butn5[16].xpmp=NULL;
  butn5[16].xpmh=NULL;
  butn5[16].bkgr=ButClr;
  butn5[16].butncode=0;
  butn5[17].sw=1;
  Procpy(butn5[17].title,(char *)"H");
  butn5[17].xpmn=NULL;
  butn5[17].xpmp=NULL;
  butn5[17].xpmh=NULL;
  butn5[17].bkgr=ButClr;
  butn5[17].butncode=0;
  butn5[18].sw=1;
  Procpy(butn5[18].title,(char *)"I");
  butn5[18].xpmn=NULL;
  butn5[18].xpmp=NULL;
  butn5[18].xpmh=NULL;
  butn5[18].bkgr=ButClr;
  butn5[18].butncode=0;
  butn5[19].sw=1;
  Procpy(butn5[19].title,(char *)"J");
  butn5[19].xpmn=NULL;
  butn5[19].xpmp=NULL;
  butn5[19].xpmh=NULL;
  butn5[19].bkgr=ButClr;
  butn5[19].butncode=0;
  butn5[20].sw=1;
  Procpy(butn5[20].title,(char *)"K");
  butn5[20].xpmn=NULL;
  butn5[20].xpmp=NULL;
  butn5[20].xpmh=NULL;
  butn5[20].bkgr=ButClr;
  butn5[20].butncode=0;
  butn5[21].sw=1;
  Procpy(butn5[21].title,(char *)"L");
  butn5[21].xpmn=NULL;
  butn5[21].xpmp=NULL;
  butn5[21].xpmh=NULL;
  butn5[21].bkgr=ButClr;
  butn5[21].butncode=0;
  butn5[22].sw=1;
  Procpy(butn5[22].title,(char *)"M");
  butn5[22].xpmn=NULL;
  butn5[22].xpmp=NULL;
  butn5[22].xpmh=NULL;
  butn5[22].bkgr=ButClr;
  butn5[22].butncode=0;
  butn5[23].sw=1;
  Procpy(butn5[23].title,(char *)"N");
  butn5[23].xpmn=NULL;
  butn5[23].xpmp=NULL;
  butn5[23].xpmh=NULL;
  butn5[23].bkgr=ButClr;
  butn5[23].butncode=0;
  butn5[24].sw=1;
  Procpy(butn5[24].title,(char *)"O");
  butn5[24].xpmn=NULL;
  butn5[24].xpmp=NULL;
  butn5[24].xpmh=NULL;
  butn5[24].bkgr=ButClr;
  butn5[24].butncode=0;
  butn5[25].sw=1;
  Procpy(butn5[25].title,(char *)"P");
  butn5[25].xpmn=NULL;
  butn5[25].xpmp=NULL;
  butn5[25].xpmh=NULL;
  butn5[25].bkgr=ButClr;
  butn5[25].butncode=0;
  butn5[26].sw=1;
  Procpy(butn5[26].title,(char *)"Q");
  butn5[26].xpmn=NULL;
  butn5[26].xpmp=NULL;
  butn5[26].xpmh=NULL;
  butn5[26].bkgr=ButClr;
  butn5[26].butncode=0;
  butn5[27].sw=1;
  Procpy(butn5[27].title,(char *)"R");
  butn5[27].xpmn=NULL;
  butn5[27].xpmp=NULL;
  butn5[27].xpmh=NULL;
  butn5[27].bkgr=ButClr;
  butn5[27].butncode=0;
  butn5[28].sw=1;
  Procpy(butn5[28].title,(char *)"S");
  butn5[28].xpmn=NULL;
  butn5[28].xpmp=NULL;
  butn5[28].xpmh=NULL;
  butn5[28].bkgr=ButClr;
  butn5[28].butncode=0;
  butn5[29].sw=1;
  Procpy(butn5[29].title,(char *)"T");
  butn5[29].xpmn=NULL;
  butn5[29].xpmp=NULL;
  butn5[29].xpmh=NULL;
  butn5[29].bkgr=ButClr;
  butn5[29].butncode=0;
  butn5[30].sw=1;
  Procpy(butn5[30].title,(char *)"U");
  butn5[30].xpmn=NULL;
  butn5[30].xpmp=NULL;
  butn5[30].xpmh=NULL;
  butn5[30].bkgr=ButClr;
  butn5[30].butncode=0;
  butn5[31].sw=1;
  Procpy(butn5[31].title,(char *)"V");
  butn5[31].xpmn=NULL;
  butn5[31].xpmp=NULL;
  butn5[31].xpmh=NULL;
  butn5[31].bkgr=ButClr;
  butn5[31].butncode=0;
  butn5[32].sw=1;
  Procpy(butn5[32].title,(char *)"W");
  butn5[32].xpmn=NULL;
  butn5[32].xpmp=NULL;
  butn5[32].xpmh=NULL;
  butn5[32].bkgr=ButClr;
  butn5[32].butncode=0;
  butn5[33].sw=1;
  Procpy(butn5[33].title,(char *)"X");
  butn5[33].xpmn=NULL;
  butn5[33].xpmp=NULL;
  butn5[33].xpmh=NULL;
  butn5[33].bkgr=ButClr;
  butn5[33].butncode=0;
  butn5[34].sw=1;
  Procpy(butn5[34].title,(char *)"Y");
  butn5[34].xpmn=NULL;
  butn5[34].xpmp=NULL;
  butn5[34].xpmh=NULL;
  butn5[34].bkgr=ButClr;
  butn5[34].butncode=0;
  butn5[35].sw=1;
  Procpy(butn5[35].title,(char *)"Z");
  butn5[35].xpmn=NULL;
  butn5[35].xpmp=NULL;
  butn5[35].xpmh=NULL;
  butn5[35].bkgr=ButClr;
  butn5[35].butncode=0;
  butn5[36].sw=1;
  Procpy(butn5[36].title,(char *)",");
  butn5[36].xpmn=NULL;
  butn5[36].xpmp=NULL;
  butn5[36].xpmh=NULL;
  butn5[36].bkgr=ButClr;
  butn5[36].butncode=0;
  butn5[37].sw=1;
  Procpy(butn5[37].title,(char *)".");
  butn5[37].xpmn=NULL;
  butn5[37].xpmp=NULL;
  butn5[37].xpmh=NULL;
  butn5[37].bkgr=ButClr;
  butn5[37].butncode=0;
  butn5[38].sw=1;
  Procpy(butn5[38].title,(char *)"/");
  butn5[38].xpmn=NULL;
  butn5[38].xpmp=NULL;
  butn5[38].xpmh=NULL;
  butn5[38].bkgr=ButClr;
  butn5[38].butncode=0;
  butn5[39].sw=1;
  Procpy(butn5[39].title,(char *)"?");
  butn5[39].xpmn=NULL;
  butn5[39].xpmp=NULL;
  butn5[39].xpmh=NULL;
  butn5[39].bkgr=ButClr;
  butn5[39].butncode=0;
  DIN b5 = { 
    'n',
    77,12,  
    442,164,
    1,1,
    36, 
    36, 
    10,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    skeybrdbutton5callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"bigkeys");
  b5.item = -1;
  BUT_STR  *butn6=NULL; 
  butn6= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn6[0].sw=1;
  Procpy(butn6[0].title,(char *)"Home");
  butn6[0].xpmn=NULL;
  butn6[0].xpmp=NULL;
  butn6[0].xpmh=NULL;
  butn6[0].bkgr=ButClr;
  butn6[0].butncode=0;
  butn6[1].sw=1;
  Procpy(butn6[1].title,(char *)"Escape");
  butn6[1].xpmn=NULL;
  butn6[1].xpmp=NULL;
  butn6[1].xpmh=NULL;
  butn6[1].bkgr=ButClr;
  butn6[1].butncode=0;
  butn6[2].sw=1;
  Procpy(butn6[2].title,(char *)"ABCD");
  butn6[2].xpmn=NULL;
  butn6[2].xpmp=NULL;
  butn6[2].xpmh=NULL;
  butn6[2].bkgr=ButClr;
  butn6[2].butncode=0;
  butn6[3].sw=1;
  Procpy(butn6[3].title,(char *)"abcd");
  butn6[3].xpmn=NULL;
  butn6[3].xpmp=NULL;
  butn6[3].xpmh=NULL;
  butn6[3].bkgr=ButClr;
  butn6[3].butncode=0;
  DIN b6 = { 
    'n',
    18,12,  
    75,164,
    1,1,
    55, 
    36, 
    1,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn6, 
    skeybrdbutton6callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b6.Wid,(char *)"symboletc");
  b6.item = -1;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*40);
  butn7[0].sw=1;
  Procpy(butn7[0].title,(char *)"1");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=ButClr;
  butn7[0].butncode=0;
  butn7[1].sw=1;
  Procpy(butn7[1].title,(char *)"2");
  butn7[1].xpmn=NULL;
  butn7[1].xpmp=NULL;
  butn7[1].xpmh=NULL;
  butn7[1].bkgr=ButClr;
  butn7[1].butncode=0;
  butn7[2].sw=1;
  Procpy(butn7[2].title,(char *)"3");
  butn7[2].xpmn=NULL;
  butn7[2].xpmp=NULL;
  butn7[2].xpmh=NULL;
  butn7[2].bkgr=ButClr;
  butn7[2].butncode=0;
  butn7[3].sw=1;
  Procpy(butn7[3].title,(char *)"4");
  butn7[3].xpmn=NULL;
  butn7[3].xpmp=NULL;
  butn7[3].xpmh=NULL;
  butn7[3].bkgr=ButClr;
  butn7[3].butncode=0;
  butn7[4].sw=1;
  Procpy(butn7[4].title,(char *)"5");
  butn7[4].xpmn=NULL;
  butn7[4].xpmp=NULL;
  butn7[4].xpmh=NULL;
  butn7[4].bkgr=ButClr;
  butn7[4].butncode=0;
  butn7[5].sw=1;
  Procpy(butn7[5].title,(char *)"6");
  butn7[5].xpmn=NULL;
  butn7[5].xpmp=NULL;
  butn7[5].xpmh=NULL;
  butn7[5].bkgr=ButClr;
  butn7[5].butncode=0;
  butn7[6].sw=1;
  Procpy(butn7[6].title,(char *)"7");
  butn7[6].xpmn=NULL;
  butn7[6].xpmp=NULL;
  butn7[6].xpmh=NULL;
  butn7[6].bkgr=ButClr;
  butn7[6].butncode=0;
  butn7[7].sw=1;
  Procpy(butn7[7].title,(char *)"8");
  butn7[7].xpmn=NULL;
  butn7[7].xpmp=NULL;
  butn7[7].xpmh=NULL;
  butn7[7].bkgr=ButClr;
  butn7[7].butncode=0;
  butn7[8].sw=1;
  Procpy(butn7[8].title,(char *)"9");
  butn7[8].xpmn=NULL;
  butn7[8].xpmp=NULL;
  butn7[8].xpmh=NULL;
  butn7[8].bkgr=ButClr;
  butn7[8].butncode=0;
  butn7[9].sw=1;
  Procpy(butn7[9].title,(char *)"0");
  butn7[9].xpmn=NULL;
  butn7[9].xpmp=NULL;
  butn7[9].xpmh=NULL;
  butn7[9].bkgr=ButClr;
  butn7[9].butncode=0;
  butn7[10].sw=1;
  Procpy(butn7[10].title,(char *)"~");
  butn7[10].xpmn=NULL;
  butn7[10].xpmp=NULL;
  butn7[10].xpmh=NULL;
  butn7[10].bkgr=ButClr;
  butn7[10].butncode=0;
  butn7[11].sw=1;
  Procpy(butn7[11].title,(char *)"`");
  butn7[11].xpmn=NULL;
  butn7[11].xpmp=NULL;
  butn7[11].xpmh=NULL;
  butn7[11].bkgr=ButClr;
  butn7[11].butncode=0;
  butn7[12].sw=1;
  Procpy(butn7[12].title,(char *)"!!");
  butn7[12].xpmn=NULL;
  butn7[12].xpmp=NULL;
  butn7[12].xpmh=NULL;
  butn7[12].bkgr=ButClr;
  butn7[12].butncode=0;
  butn7[13].sw=1;
  Procpy(butn7[13].title,(char *)"@");
  butn7[13].xpmn=NULL;
  butn7[13].xpmp=NULL;
  butn7[13].xpmh=NULL;
  butn7[13].bkgr=ButClr;
  butn7[13].butncode=0;
  butn7[14].sw=1;
  Procpy(butn7[14].title,(char *)"#");
  butn7[14].xpmn=NULL;
  butn7[14].xpmp=NULL;
  butn7[14].xpmh=NULL;
  butn7[14].bkgr=ButClr;
  butn7[14].butncode=0;
  butn7[15].sw=1;
  Procpy(butn7[15].title,(char *)"$");
  butn7[15].xpmn=NULL;
  butn7[15].xpmp=NULL;
  butn7[15].xpmh=NULL;
  butn7[15].bkgr=ButClr;
  butn7[15].butncode=0;
  butn7[16].sw=1;
  Procpy(butn7[16].title,(char *)"%");
  butn7[16].xpmn=NULL;
  butn7[16].xpmp=NULL;
  butn7[16].xpmh=NULL;
  butn7[16].bkgr=ButClr;
  butn7[16].butncode=0;
  butn7[17].sw=1;
  Procpy(butn7[17].title,(char *)"^");
  butn7[17].xpmn=NULL;
  butn7[17].xpmp=NULL;
  butn7[17].xpmh=NULL;
  butn7[17].bkgr=ButClr;
  butn7[17].butncode=0;
  butn7[18].sw=1;
  Procpy(butn7[18].title,(char *)"&");
  butn7[18].xpmn=NULL;
  butn7[18].xpmp=NULL;
  butn7[18].xpmh=NULL;
  butn7[18].bkgr=ButClr;
  butn7[18].butncode=0;
  butn7[19].sw=1;
  Procpy(butn7[19].title,(char *)"*");
  butn7[19].xpmn=NULL;
  butn7[19].xpmp=NULL;
  butn7[19].xpmh=NULL;
  butn7[19].bkgr=ButClr;
  butn7[19].butncode=0;
  butn7[20].sw=1;
  Procpy(butn7[20].title,(char *)"(");
  butn7[20].xpmn=NULL;
  butn7[20].xpmp=NULL;
  butn7[20].xpmh=NULL;
  butn7[20].bkgr=ButClr;
  butn7[20].butncode=0;
  butn7[21].sw=1;
  Procpy(butn7[21].title,(char *)")");
  butn7[21].xpmn=NULL;
  butn7[21].xpmp=NULL;
  butn7[21].xpmh=NULL;
  butn7[21].bkgr=ButClr;
  butn7[21].butncode=0;
  butn7[22].sw=1;
  Procpy(butn7[22].title,(char *)"_");
  butn7[22].xpmn=NULL;
  butn7[22].xpmp=NULL;
  butn7[22].xpmh=NULL;
  butn7[22].bkgr=ButClr;
  butn7[22].butncode=0;
  butn7[23].sw=1;
  Procpy(butn7[23].title,(char *)"+");
  butn7[23].xpmn=NULL;
  butn7[23].xpmp=NULL;
  butn7[23].xpmh=NULL;
  butn7[23].bkgr=ButClr;
  butn7[23].butncode=0;
  butn7[24].sw=1;
  Procpy(butn7[24].title,(char *)"-");
  butn7[24].xpmn=NULL;
  butn7[24].xpmp=NULL;
  butn7[24].xpmh=NULL;
  butn7[24].bkgr=ButClr;
  butn7[24].butncode=0;
  butn7[25].sw=1;
  Procpy(butn7[25].title,(char *)"=");
  butn7[25].xpmn=NULL;
  butn7[25].xpmp=NULL;
  butn7[25].xpmh=NULL;
  butn7[25].bkgr=ButClr;
  butn7[25].butncode=0;
  butn7[26].sw=1;
  Procpy(butn7[26].title,(char *)"{");
  butn7[26].xpmn=NULL;
  butn7[26].xpmp=NULL;
  butn7[26].xpmh=NULL;
  butn7[26].bkgr=ButClr;
  butn7[26].butncode=0;
  butn7[27].sw=1;
  Procpy(butn7[27].title,(char *)"}");
  butn7[27].xpmn=NULL;
  butn7[27].xpmp=NULL;
  butn7[27].xpmh=NULL;
  butn7[27].bkgr=ButClr;
  butn7[27].butncode=0;
  butn7[28].sw=1;
  Procpy(butn7[28].title,(char *)"|");
  butn7[28].xpmn=NULL;
  butn7[28].xpmp=NULL;
  butn7[28].xpmh=NULL;
  butn7[28].bkgr=ButClr;
  butn7[28].butncode=0;
  butn7[29].sw=1;
  Procpy(butn7[29].title,(char *)"[");
  butn7[29].xpmn=NULL;
  butn7[29].xpmp=NULL;
  butn7[29].xpmh=NULL;
  butn7[29].bkgr=ButClr;
  butn7[29].butncode=0;
  butn7[30].sw=1;
  Procpy(butn7[30].title,(char *)"]");
  butn7[30].xpmn=NULL;
  butn7[30].xpmp=NULL;
  butn7[30].xpmh=NULL;
  butn7[30].bkgr=ButClr;
  butn7[30].butncode=0;
  butn7[31].sw=1;
  Procpy(butn7[31].title,(char *)"\\");
  butn7[31].xpmn=NULL;
  butn7[31].xpmp=NULL;
  butn7[31].xpmh=NULL;
  butn7[31].bkgr=ButClr;
  butn7[31].butncode=0;
  butn7[32].sw=1;
  Procpy(butn7[32].title,(char *)":");
  butn7[32].xpmn=NULL;
  butn7[32].xpmp=NULL;
  butn7[32].xpmh=NULL;
  butn7[32].bkgr=ButClr;
  butn7[32].butncode=0;
  butn7[33].sw=1;
  Procpy(butn7[33].title,(char *)"\"");
  butn7[33].xpmn=NULL;
  butn7[33].xpmp=NULL;
  butn7[33].xpmh=NULL;
  butn7[33].bkgr=ButClr;
  butn7[33].butncode=0;
  butn7[34].sw=1;
  Procpy(butn7[34].title,(char *)";");
  butn7[34].xpmn=NULL;
  butn7[34].xpmp=NULL;
  butn7[34].xpmh=NULL;
  butn7[34].bkgr=ButClr;
  butn7[34].butncode=0;
  butn7[35].sw=1;
  Procpy(butn7[35].title,(char *)"'");
  butn7[35].xpmn=NULL;
  butn7[35].xpmp=NULL;
  butn7[35].xpmh=NULL;
  butn7[35].bkgr=ButClr;
  butn7[35].butncode=0;
  butn7[36].sw=1;
  Procpy(butn7[36].title,(char *)",");
  butn7[36].xpmn=NULL;
  butn7[36].xpmp=NULL;
  butn7[36].xpmh=NULL;
  butn7[36].bkgr=ButClr;
  butn7[36].butncode=0;
  butn7[37].sw=1;
  Procpy(butn7[37].title,(char *)".");
  butn7[37].xpmn=NULL;
  butn7[37].xpmp=NULL;
  butn7[37].xpmh=NULL;
  butn7[37].bkgr=ButClr;
  butn7[37].butncode=0;
  butn7[38].sw=1;
  Procpy(butn7[38].title,(char *)"<");
  butn7[38].xpmn=NULL;
  butn7[38].xpmp=NULL;
  butn7[38].xpmh=NULL;
  butn7[38].bkgr=ButClr;
  butn7[38].butncode=0;
  butn7[39].sw=1;
  Procpy(butn7[39].title,(char *)">");
  butn7[39].xpmn=NULL;
  butn7[39].xpmp=NULL;
  butn7[39].xpmh=NULL;
  butn7[39].bkgr=ButClr;
  butn7[39].butncode=0;
  DIN b7 = { 
    'n',
    77,12,  
    442,164,
    1,1,
    36, 
    36, 
    10,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    skeybrdbutton7callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"simbolkeys");
  b7.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+9));
  d =dtmp+i; 
  d[8].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIP));
  *d[0].p = p0;
  d[0].p->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton1init(&b1,pt) ;
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton2init(&b2,pt) ;
  *d[2].N = b2;
  d[2].N->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton3init(&b3,pt) ;
  *d[3].N = b3;
  d[3].N->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton4init(&b4,pt) ;
  *d[4].N = b4;
  d[4].N->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton5init(&b5,pt) ;
  *d[5].N = b5;
  d[5].N->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton6init(&b6,pt) ;
  *d[6].N = b6;
  d[6].N->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton7init(&b7,pt) ;
  *d[7].N = b7;
  d[7].N->item = -1;
  d[8].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

int keybrd2Group( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  int Red,Green,Blue;
  DIA *d=NULL,*dtmp;
  char *xpm0=   NULL; /* pixmap info */ 
  kgGetDefaultRGB(FillClr,&Red,&Green,&Blue);
  xpm0 = (char *)MakeLightImage( 672,222,Red,Green,Blue,0.0);
  DIP p0 = { 
    'p',
    21,4,  
    693,226,  
    (void *)xpm0,
    FillClr, /* bkgr colour */ 
      1,0,Btrans /* border hide transparency*/ 
  };
  strcpy(p0.Wid,(char *)"Keybrdbkgr");
  p0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn1[0].sw=1;
  Procpy(butn1[0].title,(char *)"End");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode=0;
  butn1[1].sw=1;
  Procpy(butn1[1].title,(char *)"Backspace");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode=0;
  butn1[2].sw=1;
  Procpy(butn1[2].title,(char *)"Enter");
  butn1[2].xpmn=NULL;
  butn1[2].xpmp=NULL;
  butn1[2].xpmh=NULL;
  butn1[2].bkgr=-1;
  butn1[2].butncode=0;
  butn1[3].sw=1;
  Procpy(butn1[3].title,(char *)"Space");
  butn1[3].xpmn=NULL;
  butn1[3].xpmp=NULL;
  butn1[3].xpmh=NULL;
  butn1[3].bkgr=-1;
  butn1[3].butncode=0;
  DIN b1 = { 
    'n',
    612,12,  
    694,219,
    2,2,  
    72, 
    48, 
    1,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    skeybrdbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"enter+");
  b1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn2[0].sw=1;
  Procpy(butn2[0].title,(char *)"Home");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=-1;
  butn2[0].butncode=0;
  butn2[1].sw=1;
  Procpy(butn2[1].title,(char *)"Escape");
  butn2[1].xpmn=NULL;
  butn2[1].xpmp=NULL;
  butn2[1].xpmh=NULL;
  butn2[1].bkgr=-1;
  butn2[1].butncode=0;
  butn2[2].sw=1;
  Procpy(butn2[2].title,(char *)"CapsLock");
  butn2[2].xpmn=NULL;
  butn2[2].xpmp=NULL;
  butn2[2].xpmh=NULL;
  butn2[2].bkgr=-1;
  butn2[2].butncode=0;
  butn2[3].sw=1;
  Procpy(butn2[3].title,(char *)"!!@#$%");
  butn2[3].xpmn=NULL;
  butn2[3].xpmp=NULL;
  butn2[3].xpmh=NULL;
  butn2[3].bkgr=-1;
  butn2[3].butncode=0;
  DIN b2 = { 
    'n',
    21,12,  
    105,219,
    2,2,  
    74, 
    48, 
    1,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    skeybrdbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b2.Wid,(char *)"capslocketc");
  b2.item = -1;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*40);
  butn3[0].sw=1;
  Procpy(butn3[0].title,(char *)"1");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-1;
  butn3[0].butncode=0;
  butn3[1].sw=1;
  Procpy(butn3[1].title,(char *)"2");
  butn3[1].xpmn=NULL;
  butn3[1].xpmp=NULL;
  butn3[1].xpmh=NULL;
  butn3[1].bkgr=-1;
  butn3[1].butncode=0;
  butn3[2].sw=1;
  Procpy(butn3[2].title,(char *)"3");
  butn3[2].xpmn=NULL;
  butn3[2].xpmp=NULL;
  butn3[2].xpmh=NULL;
  butn3[2].bkgr=-1;
  butn3[2].butncode=0;
  butn3[3].sw=1;
  Procpy(butn3[3].title,(char *)"4");
  butn3[3].xpmn=NULL;
  butn3[3].xpmp=NULL;
  butn3[3].xpmh=NULL;
  butn3[3].bkgr=-1;
  butn3[3].butncode=0;
  butn3[4].sw=1;
  Procpy(butn3[4].title,(char *)"5");
  butn3[4].xpmn=NULL;
  butn3[4].xpmp=NULL;
  butn3[4].xpmh=NULL;
  butn3[4].bkgr=-1;
  butn3[4].butncode=0;
  butn3[5].sw=1;
  Procpy(butn3[5].title,(char *)"6");
  butn3[5].xpmn=NULL;
  butn3[5].xpmp=NULL;
  butn3[5].xpmh=NULL;
  butn3[5].bkgr=-1;
  butn3[5].butncode=0;
  butn3[6].sw=1;
  Procpy(butn3[6].title,(char *)"7");
  butn3[6].xpmn=NULL;
  butn3[6].xpmp=NULL;
  butn3[6].xpmh=NULL;
  butn3[6].bkgr=-1;
  butn3[6].butncode=0;
  butn3[7].sw=1;
  Procpy(butn3[7].title,(char *)"8");
  butn3[7].xpmn=NULL;
  butn3[7].xpmp=NULL;
  butn3[7].xpmh=NULL;
  butn3[7].bkgr=-1;
  butn3[7].butncode=0;
  butn3[8].sw=1;
  Procpy(butn3[8].title,(char *)"9");
  butn3[8].xpmn=NULL;
  butn3[8].xpmp=NULL;
  butn3[8].xpmh=NULL;
  butn3[8].bkgr=-1;
  butn3[8].butncode=0;
  butn3[9].sw=1;
  Procpy(butn3[9].title,(char *)"0");
  butn3[9].xpmn=NULL;
  butn3[9].xpmp=NULL;
  butn3[9].xpmh=NULL;
  butn3[9].bkgr=-1;
  butn3[9].butncode=0;
  butn3[10].sw=1;
  Procpy(butn3[10].title,(char *)"a");
  butn3[10].xpmn=NULL;
  butn3[10].xpmp=NULL;
  butn3[10].xpmh=NULL;
  butn3[10].bkgr=-1;
  butn3[10].butncode=0;
  butn3[11].sw=1;
  Procpy(butn3[11].title,(char *)"b");
  butn3[11].xpmn=NULL;
  butn3[11].xpmp=NULL;
  butn3[11].xpmh=NULL;
  butn3[11].bkgr=-1;
  butn3[11].butncode=0;
  butn3[12].sw=1;
  Procpy(butn3[12].title,(char *)"c");
  butn3[12].xpmn=NULL;
  butn3[12].xpmp=NULL;
  butn3[12].xpmh=NULL;
  butn3[12].bkgr=-1;
  butn3[12].butncode=0;
  butn3[13].sw=1;
  Procpy(butn3[13].title,(char *)"d");
  butn3[13].xpmn=NULL;
  butn3[13].xpmp=NULL;
  butn3[13].xpmh=NULL;
  butn3[13].bkgr=-1;
  butn3[13].butncode=0;
  butn3[14].sw=1;
  Procpy(butn3[14].title,(char *)"e");
  butn3[14].xpmn=NULL;
  butn3[14].xpmp=NULL;
  butn3[14].xpmh=NULL;
  butn3[14].bkgr=-1;
  butn3[14].butncode=0;
  butn3[15].sw=1;
  Procpy(butn3[15].title,(char *)"f");
  butn3[15].xpmn=NULL;
  butn3[15].xpmp=NULL;
  butn3[15].xpmh=NULL;
  butn3[15].bkgr=-1;
  butn3[15].butncode=0;
  butn3[16].sw=1;
  Procpy(butn3[16].title,(char *)"g");
  butn3[16].xpmn=NULL;
  butn3[16].xpmp=NULL;
  butn3[16].xpmh=NULL;
  butn3[16].bkgr=-1;
  butn3[16].butncode=0;
  butn3[17].sw=1;
  Procpy(butn3[17].title,(char *)"h");
  butn3[17].xpmn=NULL;
  butn3[17].xpmp=NULL;
  butn3[17].xpmh=NULL;
  butn3[17].bkgr=-1;
  butn3[17].butncode=0;
  butn3[18].sw=1;
  Procpy(butn3[18].title,(char *)"i");
  butn3[18].xpmn=NULL;
  butn3[18].xpmp=NULL;
  butn3[18].xpmh=NULL;
  butn3[18].bkgr=-1;
  butn3[18].butncode=0;
  butn3[19].sw=1;
  Procpy(butn3[19].title,(char *)"j");
  butn3[19].xpmn=NULL;
  butn3[19].xpmp=NULL;
  butn3[19].xpmh=NULL;
  butn3[19].bkgr=-1;
  butn3[19].butncode=0;
  butn3[20].sw=1;
  Procpy(butn3[20].title,(char *)"k");
  butn3[20].xpmn=NULL;
  butn3[20].xpmp=NULL;
  butn3[20].xpmh=NULL;
  butn3[20].bkgr=-1;
  butn3[20].butncode=0;
  butn3[21].sw=1;
  Procpy(butn3[21].title,(char *)"l");
  butn3[21].xpmn=NULL;
  butn3[21].xpmp=NULL;
  butn3[21].xpmh=NULL;
  butn3[21].bkgr=-1;
  butn3[21].butncode=0;
  butn3[22].sw=1;
  Procpy(butn3[22].title,(char *)"m");
  butn3[22].xpmn=NULL;
  butn3[22].xpmp=NULL;
  butn3[22].xpmh=NULL;
  butn3[22].bkgr=-1;
  butn3[22].butncode=0;
  butn3[23].sw=1;
  Procpy(butn3[23].title,(char *)"n");
  butn3[23].xpmn=NULL;
  butn3[23].xpmp=NULL;
  butn3[23].xpmh=NULL;
  butn3[23].bkgr=-1;
  butn3[23].butncode=0;
  butn3[24].sw=1;
  Procpy(butn3[24].title,(char *)"o");
  butn3[24].xpmn=NULL;
  butn3[24].xpmp=NULL;
  butn3[24].xpmh=NULL;
  butn3[24].bkgr=-1;
  butn3[24].butncode=0;
  butn3[25].sw=1;
  Procpy(butn3[25].title,(char *)"p");
  butn3[25].xpmn=NULL;
  butn3[25].xpmp=NULL;
  butn3[25].xpmh=NULL;
  butn3[25].bkgr=-1;
  butn3[25].butncode=0;
  butn3[26].sw=1;
  Procpy(butn3[26].title,(char *)"q");
  butn3[26].xpmn=NULL;
  butn3[26].xpmp=NULL;
  butn3[26].xpmh=NULL;
  butn3[26].bkgr=-1;
  butn3[26].butncode=0;
  butn3[27].sw=1;
  Procpy(butn3[27].title,(char *)"r");
  butn3[27].xpmn=NULL;
  butn3[27].xpmp=NULL;
  butn3[27].xpmh=NULL;
  butn3[27].bkgr=-1;
  butn3[27].butncode=0;
  butn3[28].sw=1;
  Procpy(butn3[28].title,(char *)"s");
  butn3[28].xpmn=NULL;
  butn3[28].xpmp=NULL;
  butn3[28].xpmh=NULL;
  butn3[28].bkgr=-1;
  butn3[28].butncode=0;
  butn3[29].sw=1;
  Procpy(butn3[29].title,(char *)"t");
  butn3[29].xpmn=NULL;
  butn3[29].xpmp=NULL;
  butn3[29].xpmh=NULL;
  butn3[29].bkgr=-1;
  butn3[29].butncode=0;
  butn3[30].sw=1;
  Procpy(butn3[30].title,(char *)"u");
  butn3[30].xpmn=NULL;
  butn3[30].xpmp=NULL;
  butn3[30].xpmh=NULL;
  butn3[30].bkgr=-1;
  butn3[30].butncode=0;
  butn3[31].sw=1;
  Procpy(butn3[31].title,(char *)"v");
  butn3[31].xpmn=NULL;
  butn3[31].xpmp=NULL;
  butn3[31].xpmh=NULL;
  butn3[31].bkgr=-1;
  butn3[31].butncode=0;
  butn3[32].sw=1;
  Procpy(butn3[32].title,(char *)"w");
  butn3[32].xpmn=NULL;
  butn3[32].xpmp=NULL;
  butn3[32].xpmh=NULL;
  butn3[32].bkgr=-1;
  butn3[32].butncode=0;
  butn3[33].sw=1;
  Procpy(butn3[33].title,(char *)"x");
  butn3[33].xpmn=NULL;
  butn3[33].xpmp=NULL;
  butn3[33].xpmh=NULL;
  butn3[33].bkgr=-1;
  butn3[33].butncode=0;
  butn3[34].sw=1;
  Procpy(butn3[34].title,(char *)"y");
  butn3[34].xpmn=NULL;
  butn3[34].xpmp=NULL;
  butn3[34].xpmh=NULL;
  butn3[34].bkgr=-1;
  butn3[34].butncode=0;
  butn3[35].sw=1;
  Procpy(butn3[35].title,(char *)"z");
  butn3[35].xpmn=NULL;
  butn3[35].xpmp=NULL;
  butn3[35].xpmh=NULL;
  butn3[35].bkgr=-1;
  butn3[35].butncode=0;
  butn3[36].sw=1;
  Procpy(butn3[36].title,(char *)",");
  butn3[36].xpmn=NULL;
  butn3[36].xpmp=NULL;
  butn3[36].xpmh=NULL;
  butn3[36].bkgr=-1;
  butn3[36].butncode=0;
  butn3[37].sw=1;
  Procpy(butn3[37].title,(char *)".");
  butn3[37].xpmn=NULL;
  butn3[37].xpmp=NULL;
  butn3[37].xpmh=NULL;
  butn3[37].bkgr=-1;
  butn3[37].butncode=0;
  butn3[38].sw=1;
  Procpy(butn3[38].title,(char *)"/");
  butn3[38].xpmn=NULL;
  butn3[38].xpmp=NULL;
  butn3[38].xpmh=NULL;
  butn3[38].bkgr=-1;
  butn3[38].butncode=0;
  butn3[39].sw=1;
  Procpy(butn3[39].title,(char *)"?");
  butn3[39].xpmn=NULL;
  butn3[39].xpmp=NULL;
  butn3[39].xpmh=NULL;
  butn3[39].bkgr=-1;
  butn3[39].butncode=0;
  DIN b3 = { 
    'n',
    105,12,  
    612,219,
    2,2,  
    48, 
    48, 
    10,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    skeybrdbutton3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b3.Wid,(char *)"smallkeys");
  b3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn4[0].sw=1;
  Procpy(butn4[0].title,(char *)"Home");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-1;
  butn4[0].butncode=0;
  butn4[1].sw=1;
  Procpy(butn4[1].title,(char *)"Escape");
  butn4[1].xpmn=NULL;
  butn4[1].xpmp=NULL;
  butn4[1].xpmh=NULL;
  butn4[1].bkgr=-1;
  butn4[1].butncode=0;
  butn4[2].sw=1;
  Procpy(butn4[2].title,(char *)"Capslock");
  butn4[2].xpmn=NULL;
  butn4[2].xpmp=NULL;
  butn4[2].xpmh=NULL;
  butn4[2].bkgr=-1;
  butn4[2].butncode=0;
  butn4[3].sw=1;
  Procpy(butn4[3].title,(char *)"!!@#$%");
  butn4[3].xpmn=NULL;
  butn4[3].xpmp=NULL;
  butn4[3].xpmh=NULL;
  butn4[3].bkgr=-1;
  butn4[3].butncode=0;
  DIN b4 = { 
    'n',
    21,12,  
    105,219,
    2,2,  
    74, 
    48, 
    1,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    skeybrdbutton4callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b4.Wid,(char *)"CAPSLOCKETC");
  b4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*40);
  butn5[0].sw=1;
  Procpy(butn5[0].title,(char *)"1");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-1;
  butn5[0].butncode=0;
  butn5[1].sw=1;
  Procpy(butn5[1].title,(char *)"2");
  butn5[1].xpmn=NULL;
  butn5[1].xpmp=NULL;
  butn5[1].xpmh=NULL;
  butn5[1].bkgr=-1;
  butn5[1].butncode=0;
  butn5[2].sw=1;
  Procpy(butn5[2].title,(char *)"3");
  butn5[2].xpmn=NULL;
  butn5[2].xpmp=NULL;
  butn5[2].xpmh=NULL;
  butn5[2].bkgr=-1;
  butn5[2].butncode=0;
  butn5[3].sw=1;
  Procpy(butn5[3].title,(char *)"4");
  butn5[3].xpmn=NULL;
  butn5[3].xpmp=NULL;
  butn5[3].xpmh=NULL;
  butn5[3].bkgr=-1;
  butn5[3].butncode=0;
  butn5[4].sw=1;
  Procpy(butn5[4].title,(char *)"5");
  butn5[4].xpmn=NULL;
  butn5[4].xpmp=NULL;
  butn5[4].xpmh=NULL;
  butn5[4].bkgr=-1;
  butn5[4].butncode=0;
  butn5[5].sw=1;
  Procpy(butn5[5].title,(char *)"6");
  butn5[5].xpmn=NULL;
  butn5[5].xpmp=NULL;
  butn5[5].xpmh=NULL;
  butn5[5].bkgr=-1;
  butn5[5].butncode=0;
  butn5[6].sw=1;
  Procpy(butn5[6].title,(char *)"7");
  butn5[6].xpmn=NULL;
  butn5[6].xpmp=NULL;
  butn5[6].xpmh=NULL;
  butn5[6].bkgr=-1;
  butn5[6].butncode=0;
  butn5[7].sw=1;
  Procpy(butn5[7].title,(char *)"8");
  butn5[7].xpmn=NULL;
  butn5[7].xpmp=NULL;
  butn5[7].xpmh=NULL;
  butn5[7].bkgr=-1;
  butn5[7].butncode=0;
  butn5[8].sw=1;
  Procpy(butn5[8].title,(char *)"9");
  butn5[8].xpmn=NULL;
  butn5[8].xpmp=NULL;
  butn5[8].xpmh=NULL;
  butn5[8].bkgr=-1;
  butn5[8].butncode=0;
  butn5[9].sw=1;
  Procpy(butn5[9].title,(char *)"0");
  butn5[9].xpmn=NULL;
  butn5[9].xpmp=NULL;
  butn5[9].xpmh=NULL;
  butn5[9].bkgr=-1;
  butn5[9].butncode=0;
  butn5[10].sw=1;
  Procpy(butn5[10].title,(char *)"A");
  butn5[10].xpmn=NULL;
  butn5[10].xpmp=NULL;
  butn5[10].xpmh=NULL;
  butn5[10].bkgr=-1;
  butn5[10].butncode=0;
  butn5[11].sw=1;
  Procpy(butn5[11].title,(char *)"B");
  butn5[11].xpmn=NULL;
  butn5[11].xpmp=NULL;
  butn5[11].xpmh=NULL;
  butn5[11].bkgr=-1;
  butn5[11].butncode=0;
  butn5[12].sw=1;
  Procpy(butn5[12].title,(char *)"C");
  butn5[12].xpmn=NULL;
  butn5[12].xpmp=NULL;
  butn5[12].xpmh=NULL;
  butn5[12].bkgr=-1;
  butn5[12].butncode=0;
  butn5[13].sw=1;
  Procpy(butn5[13].title,(char *)"D");
  butn5[13].xpmn=NULL;
  butn5[13].xpmp=NULL;
  butn5[13].xpmh=NULL;
  butn5[13].bkgr=-1;
  butn5[13].butncode=0;
  butn5[14].sw=1;
  Procpy(butn5[14].title,(char *)"E");
  butn5[14].xpmn=NULL;
  butn5[14].xpmp=NULL;
  butn5[14].xpmh=NULL;
  butn5[14].bkgr=-1;
  butn5[14].butncode=0;
  butn5[15].sw=1;
  Procpy(butn5[15].title,(char *)"F");
  butn5[15].xpmn=NULL;
  butn5[15].xpmp=NULL;
  butn5[15].xpmh=NULL;
  butn5[15].bkgr=-1;
  butn5[15].butncode=0;
  butn5[16].sw=1;
  Procpy(butn5[16].title,(char *)"G");
  butn5[16].xpmn=NULL;
  butn5[16].xpmp=NULL;
  butn5[16].xpmh=NULL;
  butn5[16].bkgr=-1;
  butn5[16].butncode=0;
  butn5[17].sw=1;
  Procpy(butn5[17].title,(char *)"H");
  butn5[17].xpmn=NULL;
  butn5[17].xpmp=NULL;
  butn5[17].xpmh=NULL;
  butn5[17].bkgr=-1;
  butn5[17].butncode=0;
  butn5[18].sw=1;
  Procpy(butn5[18].title,(char *)"I");
  butn5[18].xpmn=NULL;
  butn5[18].xpmp=NULL;
  butn5[18].xpmh=NULL;
  butn5[18].bkgr=-1;
  butn5[18].butncode=0;
  butn5[19].sw=1;
  Procpy(butn5[19].title,(char *)"J");
  butn5[19].xpmn=NULL;
  butn5[19].xpmp=NULL;
  butn5[19].xpmh=NULL;
  butn5[19].bkgr=-1;
  butn5[19].butncode=0;
  butn5[20].sw=1;
  Procpy(butn5[20].title,(char *)"K");
  butn5[20].xpmn=NULL;
  butn5[20].xpmp=NULL;
  butn5[20].xpmh=NULL;
  butn5[20].bkgr=-1;
  butn5[20].butncode=0;
  butn5[21].sw=1;
  Procpy(butn5[21].title,(char *)"L");
  butn5[21].xpmn=NULL;
  butn5[21].xpmp=NULL;
  butn5[21].xpmh=NULL;
  butn5[21].bkgr=-1;
  butn5[21].butncode=0;
  butn5[22].sw=1;
  Procpy(butn5[22].title,(char *)"M");
  butn5[22].xpmn=NULL;
  butn5[22].xpmp=NULL;
  butn5[22].xpmh=NULL;
  butn5[22].bkgr=-1;
  butn5[22].butncode=0;
  butn5[23].sw=1;
  Procpy(butn5[23].title,(char *)"N");
  butn5[23].xpmn=NULL;
  butn5[23].xpmp=NULL;
  butn5[23].xpmh=NULL;
  butn5[23].bkgr=-1;
  butn5[23].butncode=0;
  butn5[24].sw=1;
  Procpy(butn5[24].title,(char *)"O");
  butn5[24].xpmn=NULL;
  butn5[24].xpmp=NULL;
  butn5[24].xpmh=NULL;
  butn5[24].bkgr=-1;
  butn5[24].butncode=0;
  butn5[25].sw=1;
  Procpy(butn5[25].title,(char *)"P");
  butn5[25].xpmn=NULL;
  butn5[25].xpmp=NULL;
  butn5[25].xpmh=NULL;
  butn5[25].bkgr=-1;
  butn5[25].butncode=0;
  butn5[26].sw=1;
  Procpy(butn5[26].title,(char *)"Q");
  butn5[26].xpmn=NULL;
  butn5[26].xpmp=NULL;
  butn5[26].xpmh=NULL;
  butn5[26].bkgr=-1;
  butn5[26].butncode=0;
  butn5[27].sw=1;
  Procpy(butn5[27].title,(char *)"R");
  butn5[27].xpmn=NULL;
  butn5[27].xpmp=NULL;
  butn5[27].xpmh=NULL;
  butn5[27].bkgr=-1;
  butn5[27].butncode=0;
  butn5[28].sw=1;
  Procpy(butn5[28].title,(char *)"S");
  butn5[28].xpmn=NULL;
  butn5[28].xpmp=NULL;
  butn5[28].xpmh=NULL;
  butn5[28].bkgr=-1;
  butn5[28].butncode=0;
  butn5[29].sw=1;
  Procpy(butn5[29].title,(char *)"T");
  butn5[29].xpmn=NULL;
  butn5[29].xpmp=NULL;
  butn5[29].xpmh=NULL;
  butn5[29].bkgr=-1;
  butn5[29].butncode=0;
  butn5[30].sw=1;
  Procpy(butn5[30].title,(char *)"U");
  butn5[30].xpmn=NULL;
  butn5[30].xpmp=NULL;
  butn5[30].xpmh=NULL;
  butn5[30].bkgr=-1;
  butn5[30].butncode=0;
  butn5[31].sw=1;
  Procpy(butn5[31].title,(char *)"V");
  butn5[31].xpmn=NULL;
  butn5[31].xpmp=NULL;
  butn5[31].xpmh=NULL;
  butn5[31].bkgr=-1;
  butn5[31].butncode=0;
  butn5[32].sw=1;
  Procpy(butn5[32].title,(char *)"W");
  butn5[32].xpmn=NULL;
  butn5[32].xpmp=NULL;
  butn5[32].xpmh=NULL;
  butn5[32].bkgr=-1;
  butn5[32].butncode=0;
  butn5[33].sw=1;
  Procpy(butn5[33].title,(char *)"X");
  butn5[33].xpmn=NULL;
  butn5[33].xpmp=NULL;
  butn5[33].xpmh=NULL;
  butn5[33].bkgr=-1;
  butn5[33].butncode=0;
  butn5[34].sw=1;
  Procpy(butn5[34].title,(char *)"Y");
  butn5[34].xpmn=NULL;
  butn5[34].xpmp=NULL;
  butn5[34].xpmh=NULL;
  butn5[34].bkgr=-1;
  butn5[34].butncode=0;
  butn5[35].sw=1;
  Procpy(butn5[35].title,(char *)"Z");
  butn5[35].xpmn=NULL;
  butn5[35].xpmp=NULL;
  butn5[35].xpmh=NULL;
  butn5[35].bkgr=-1;
  butn5[35].butncode=0;
  butn5[36].sw=1;
  Procpy(butn5[36].title,(char *)",");
  butn5[36].xpmn=NULL;
  butn5[36].xpmp=NULL;
  butn5[36].xpmh=NULL;
  butn5[36].bkgr=-1;
  butn5[36].butncode=0;
  butn5[37].sw=1;
  Procpy(butn5[37].title,(char *)".");
  butn5[37].xpmn=NULL;
  butn5[37].xpmp=NULL;
  butn5[37].xpmh=NULL;
  butn5[37].bkgr=-1;
  butn5[37].butncode=0;
  butn5[38].sw=1;
  Procpy(butn5[38].title,(char *)"/");
  butn5[38].xpmn=NULL;
  butn5[38].xpmp=NULL;
  butn5[38].xpmh=NULL;
  butn5[38].bkgr=-1;
  butn5[38].butncode=0;
  butn5[39].sw=1;
  Procpy(butn5[39].title,(char *)"?");
  butn5[39].xpmn=NULL;
  butn5[39].xpmp=NULL;
  butn5[39].xpmh=NULL;
  butn5[39].bkgr=-1;
  butn5[39].butncode=0;
  DIN b5 = { 
    'n',
    105,12,  
    612,219,
    2,2,  
    48, 
    48, 
    10,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    skeybrdbutton5callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"bigkeys");
  b5.item = -1;
  BUT_STR  *butn6=NULL; 
  butn6= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn6[0].sw=1;
  Procpy(butn6[0].title,(char *)"Home");
  butn6[0].xpmn=NULL;
  butn6[0].xpmp=NULL;
  butn6[0].xpmh=NULL;
  butn6[0].bkgr=-1;
  butn6[0].butncode=0;
  butn6[1].sw=1;
  Procpy(butn6[1].title,(char *)"Escape");
  butn6[1].xpmn=NULL;
  butn6[1].xpmp=NULL;
  butn6[1].xpmh=NULL;
  butn6[1].bkgr=-1;
  butn6[1].butncode=0;
  butn6[2].sw=1;
  Procpy(butn6[2].title,(char *)"ABCD");
  butn6[2].xpmn=NULL;
  butn6[2].xpmp=NULL;
  butn6[2].xpmh=NULL;
  butn6[2].bkgr=-1;
  butn6[2].butncode=0;
  butn6[3].sw=1;
  Procpy(butn6[3].title,(char *)"abcd");
  butn6[3].xpmn=NULL;
  butn6[3].xpmp=NULL;
  butn6[3].xpmh=NULL;
  butn6[3].bkgr=-1;
  butn6[3].butncode=0;
  DIN b6 = { 
    'n',
    21,12,  
    105,219,
    2,2,  
    74, 
    48, 
    1,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn6, 
    skeybrdbutton6callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b6.Wid,(char *)"symboletc");
  b6.item = -1;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*40);
  butn7[0].sw=1;
  Procpy(butn7[0].title,(char *)"1");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-1;
  butn7[0].butncode=0;
  butn7[1].sw=1;
  Procpy(butn7[1].title,(char *)"2");
  butn7[1].xpmn=NULL;
  butn7[1].xpmp=NULL;
  butn7[1].xpmh=NULL;
  butn7[1].bkgr=-1;
  butn7[1].butncode=0;
  butn7[2].sw=1;
  Procpy(butn7[2].title,(char *)"3");
  butn7[2].xpmn=NULL;
  butn7[2].xpmp=NULL;
  butn7[2].xpmh=NULL;
  butn7[2].bkgr=-1;
  butn7[2].butncode=0;
  butn7[3].sw=1;
  Procpy(butn7[3].title,(char *)"4");
  butn7[3].xpmn=NULL;
  butn7[3].xpmp=NULL;
  butn7[3].xpmh=NULL;
  butn7[3].bkgr=-1;
  butn7[3].butncode=0;
  butn7[4].sw=1;
  Procpy(butn7[4].title,(char *)"5");
  butn7[4].xpmn=NULL;
  butn7[4].xpmp=NULL;
  butn7[4].xpmh=NULL;
  butn7[4].bkgr=-1;
  butn7[4].butncode=0;
  butn7[5].sw=1;
  Procpy(butn7[5].title,(char *)"6");
  butn7[5].xpmn=NULL;
  butn7[5].xpmp=NULL;
  butn7[5].xpmh=NULL;
  butn7[5].bkgr=-1;
  butn7[5].butncode=0;
  butn7[6].sw=1;
  Procpy(butn7[6].title,(char *)"7");
  butn7[6].xpmn=NULL;
  butn7[6].xpmp=NULL;
  butn7[6].xpmh=NULL;
  butn7[6].bkgr=-1;
  butn7[6].butncode=0;
  butn7[7].sw=1;
  Procpy(butn7[7].title,(char *)"8");
  butn7[7].xpmn=NULL;
  butn7[7].xpmp=NULL;
  butn7[7].xpmh=NULL;
  butn7[7].bkgr=-1;
  butn7[7].butncode=0;
  butn7[8].sw=1;
  Procpy(butn7[8].title,(char *)"9");
  butn7[8].xpmn=NULL;
  butn7[8].xpmp=NULL;
  butn7[8].xpmh=NULL;
  butn7[8].bkgr=-1;
  butn7[8].butncode=0;
  butn7[9].sw=1;
  Procpy(butn7[9].title,(char *)"0");
  butn7[9].xpmn=NULL;
  butn7[9].xpmp=NULL;
  butn7[9].xpmh=NULL;
  butn7[9].bkgr=-1;
  butn7[9].butncode=0;
  butn7[10].sw=1;
  Procpy(butn7[10].title,(char *)"~");
  butn7[10].xpmn=NULL;
  butn7[10].xpmp=NULL;
  butn7[10].xpmh=NULL;
  butn7[10].bkgr=-1;
  butn7[10].butncode=0;
  butn7[11].sw=1;
  Procpy(butn7[11].title,(char *)"`");
  butn7[11].xpmn=NULL;
  butn7[11].xpmp=NULL;
  butn7[11].xpmh=NULL;
  butn7[11].bkgr=-1;
  butn7[11].butncode=0;
  butn7[12].sw=1;
  Procpy(butn7[12].title,(char *)"!!");
  butn7[12].xpmn=NULL;
  butn7[12].xpmp=NULL;
  butn7[12].xpmh=NULL;
  butn7[12].bkgr=-1;
  butn7[12].butncode=0;
  butn7[13].sw=1;
  Procpy(butn7[13].title,(char *)"@");
  butn7[13].xpmn=NULL;
  butn7[13].xpmp=NULL;
  butn7[13].xpmh=NULL;
  butn7[13].bkgr=-1;
  butn7[13].butncode=0;
  butn7[14].sw=1;
  Procpy(butn7[14].title,(char *)"#");
  butn7[14].xpmn=NULL;
  butn7[14].xpmp=NULL;
  butn7[14].xpmh=NULL;
  butn7[14].bkgr=-1;
  butn7[14].butncode=0;
  butn7[15].sw=1;
  Procpy(butn7[15].title,(char *)"$");
  butn7[15].xpmn=NULL;
  butn7[15].xpmp=NULL;
  butn7[15].xpmh=NULL;
  butn7[15].bkgr=-1;
  butn7[15].butncode=0;
  butn7[16].sw=1;
  Procpy(butn7[16].title,(char *)"%");
  butn7[16].xpmn=NULL;
  butn7[16].xpmp=NULL;
  butn7[16].xpmh=NULL;
  butn7[16].bkgr=-1;
  butn7[16].butncode=0;
  butn7[17].sw=1;
  Procpy(butn7[17].title,(char *)"^");
  butn7[17].xpmn=NULL;
  butn7[17].xpmp=NULL;
  butn7[17].xpmh=NULL;
  butn7[17].bkgr=-1;
  butn7[17].butncode=0;
  butn7[18].sw=1;
  Procpy(butn7[18].title,(char *)"&");
  butn7[18].xpmn=NULL;
  butn7[18].xpmp=NULL;
  butn7[18].xpmh=NULL;
  butn7[18].bkgr=-1;
  butn7[18].butncode=0;
  butn7[19].sw=1;
  Procpy(butn7[19].title,(char *)"*");
  butn7[19].xpmn=NULL;
  butn7[19].xpmp=NULL;
  butn7[19].xpmh=NULL;
  butn7[19].bkgr=-1;
  butn7[19].butncode=0;
  butn7[20].sw=1;
  Procpy(butn7[20].title,(char *)"(");
  butn7[20].xpmn=NULL;
  butn7[20].xpmp=NULL;
  butn7[20].xpmh=NULL;
  butn7[20].bkgr=-1;
  butn7[20].butncode=0;
  butn7[21].sw=1;
  Procpy(butn7[21].title,(char *)")");
  butn7[21].xpmn=NULL;
  butn7[21].xpmp=NULL;
  butn7[21].xpmh=NULL;
  butn7[21].bkgr=-1;
  butn7[21].butncode=0;
  butn7[22].sw=1;
  Procpy(butn7[22].title,(char *)"_");
  butn7[22].xpmn=NULL;
  butn7[22].xpmp=NULL;
  butn7[22].xpmh=NULL;
  butn7[22].bkgr=-1;
  butn7[22].butncode=0;
  butn7[23].sw=1;
  Procpy(butn7[23].title,(char *)"+");
  butn7[23].xpmn=NULL;
  butn7[23].xpmp=NULL;
  butn7[23].xpmh=NULL;
  butn7[23].bkgr=-1;
  butn7[23].butncode=0;
  butn7[24].sw=1;
  Procpy(butn7[24].title,(char *)"-");
  butn7[24].xpmn=NULL;
  butn7[24].xpmp=NULL;
  butn7[24].xpmh=NULL;
  butn7[24].bkgr=-1;
  butn7[24].butncode=0;
  butn7[25].sw=1;
  Procpy(butn7[25].title,(char *)"=");
  butn7[25].xpmn=NULL;
  butn7[25].xpmp=NULL;
  butn7[25].xpmh=NULL;
  butn7[25].bkgr=-1;
  butn7[25].butncode=0;
  butn7[26].sw=1;
  Procpy(butn7[26].title,(char *)"{");
  butn7[26].xpmn=NULL;
  butn7[26].xpmp=NULL;
  butn7[26].xpmh=NULL;
  butn7[26].bkgr=-1;
  butn7[26].butncode=0;
  butn7[27].sw=1;
  Procpy(butn7[27].title,(char *)"}");
  butn7[27].xpmn=NULL;
  butn7[27].xpmp=NULL;
  butn7[27].xpmh=NULL;
  butn7[27].bkgr=-1;
  butn7[27].butncode=0;
  butn7[28].sw=1;
  Procpy(butn7[28].title,(char *)"|");
  butn7[28].xpmn=NULL;
  butn7[28].xpmp=NULL;
  butn7[28].xpmh=NULL;
  butn7[28].bkgr=-1;
  butn7[28].butncode=0;
  butn7[29].sw=1;
  Procpy(butn7[29].title,(char *)"[");
  butn7[29].xpmn=NULL;
  butn7[29].xpmp=NULL;
  butn7[29].xpmh=NULL;
  butn7[29].bkgr=-1;
  butn7[29].butncode=0;
  butn7[30].sw=1;
  Procpy(butn7[30].title,(char *)"]");
  butn7[30].xpmn=NULL;
  butn7[30].xpmp=NULL;
  butn7[30].xpmh=NULL;
  butn7[30].bkgr=-1;
  butn7[30].butncode=0;
  butn7[31].sw=1;
  Procpy(butn7[31].title,(char *)"\\");
  butn7[31].xpmn=NULL;
  butn7[31].xpmp=NULL;
  butn7[31].xpmh=NULL;
  butn7[31].bkgr=-1;
  butn7[31].butncode=0;
  butn7[32].sw=1;
  Procpy(butn7[32].title,(char *)":");
  butn7[32].xpmn=NULL;
  butn7[32].xpmp=NULL;
  butn7[32].xpmh=NULL;
  butn7[32].bkgr=-1;
  butn7[32].butncode=0;
  butn7[33].sw=1;
  Procpy(butn7[33].title,(char *)"\\");
  butn7[33].xpmn=NULL;
  butn7[33].xpmp=NULL;
  butn7[33].xpmh=NULL;
  butn7[33].bkgr=-1;
  butn7[33].butncode=0;
  butn7[34].sw=1;
  Procpy(butn7[34].title,(char *)";");
  butn7[34].xpmn=NULL;
  butn7[34].xpmp=NULL;
  butn7[34].xpmh=NULL;
  butn7[34].bkgr=-1;
  butn7[34].butncode=0;
  butn7[35].sw=1;
  Procpy(butn7[35].title,(char *)"'");
  butn7[35].xpmn=NULL;
  butn7[35].xpmp=NULL;
  butn7[35].xpmh=NULL;
  butn7[35].bkgr=-1;
  butn7[35].butncode=0;
  butn7[36].sw=1;
  Procpy(butn7[36].title,(char *)",");
  butn7[36].xpmn=NULL;
  butn7[36].xpmp=NULL;
  butn7[36].xpmh=NULL;
  butn7[36].bkgr=-1;
  butn7[36].butncode=0;
  butn7[37].sw=1;
  Procpy(butn7[37].title,(char *)".");
  butn7[37].xpmn=NULL;
  butn7[37].xpmp=NULL;
  butn7[37].xpmh=NULL;
  butn7[37].bkgr=-1;
  butn7[37].butncode=0;
  butn7[38].sw=1;
  Procpy(butn7[38].title,(char *)"<");
  butn7[38].xpmn=NULL;
  butn7[38].xpmp=NULL;
  butn7[38].xpmh=NULL;
  butn7[38].bkgr=-1;
  butn7[38].butncode=0;
  butn7[39].sw=1;
  Procpy(butn7[39].title,(char *)">");
  butn7[39].xpmn=NULL;
  butn7[39].xpmp=NULL;
  butn7[39].xpmh=NULL;
  butn7[39].bkgr=-1;
  butn7[39].butncode=0;
  DIN b7 = { 
    'n',
    105,12,  
    612,219,
    2,2,  
    48, 
    48, 
    10,4, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    skeybrdbutton7callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"simbolkeys");
  b7.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+9));
  d =dtmp+i; 
  d[8].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIP));
  *d[0].p = p0;
  d[0].p->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton1init(&b1,pt) ;
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton2init(&b2,pt) ;
  *d[2].N = b2;
  d[2].N->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton3init(&b3,pt) ;
  *d[3].N = b3;
  d[3].N->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton4init(&b4,pt) ;
  *d[4].N = b4;
  d[4].N->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton5init(&b5,pt) ;
  *d[5].N = b5;
  d[5].N->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton6init(&b6,pt) ;
  *d[6].N = b6;
  d[6].N->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  skeybrdbutton7init(&b7,pt) ;
  *d[7].N = b7;
  d[7].N->item = -1;
  d[8].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 
/* One can also use the following code to add Widgets to an existing Dialog */

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
static int CreateKeybrdGrps(DIALOG *D,KEYBRD *Kbrd,int offset,int Vis,int xo,int yo) {
   int xm=10000,ym=10000;
   int rgrp,lgrp;
   int i;
   DIA *d;
   d = (DIA *)(D->d);
   i = offset;
   while(d[i].t != NULL) {
     if(xm > (d[i].t->x1) ) xm = d[i].t->x1;
     if(ym > (d[i].t->y1) ) ym = d[i].t->y1;
     i++;
   }
   kgShiftGrp(D,Kbrd->GrpId,xo-xm,yo-ym);
   d = D->d;
   lgrp = kgOpenGrp(D);
   kgAddtoGrp(D,lgrp,d[2+offset].t);
   kgAddtoGrp(D,lgrp,d[4+offset].t);
   kgAddtoGrp(D,lgrp,d[6+offset].t);
   rgrp = kgOpenGrp(D);
   kgAddtoGrp(D,rgrp,d[1+offset].t);
   if(Kbrd->kbtype==2) {
     kgShiftGrp(D,lgrp,8,0);
     kgShiftGrp(D,rgrp,-7,0);
   }
   else {
     kgShiftGrp(D,lgrp,2,0);
     kgShiftGrp(D,rgrp,7,0);
   }
   Kbrd->sgrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->sgrp,d[0+offset].t);
   kgAddtoGrp(D,Kbrd->sgrp,d[1+offset].t);
   kgAddtoGrp(D,Kbrd->sgrp,d[3+offset].t);
   kgAddtoGrp(D,Kbrd->sgrp,d[2+offset].t);
   Kbrd->cgrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->cgrp,d[0+offset].t);
   kgAddtoGrp(D,Kbrd->cgrp,d[1+offset].t);
   kgAddtoGrp(D,Kbrd->cgrp,d[5+offset].t);
   kgAddtoGrp(D,Kbrd->cgrp,d[4+offset].t);
   Kbrd->symgrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->symgrp,d[0+offset].t);
   kgAddtoGrp(D,Kbrd->symgrp,d[1+offset].t);
   kgAddtoGrp(D,Kbrd->symgrp,d[7+offset].t);
   kgAddtoGrp(D,Kbrd->symgrp,d[6+offset].t);
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
    kgSetGrpVisibility(D,Kbrd->symgrp,0);
    kgSetGrpVisibility(D,Kbrd->sgrp,1);
   }
   return 1;
}
int kgMakeDefaultSkeybrd(DIALOG *D,int xo,int yo,int Vis) {
   int GrpId;
   WIDGETGRP *Gpt;
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
    int i=0,offset;
   DIA *dtmp,*d;
   Gclr gc;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)malloc(sizeof(KEYBRD));
   D->Kbrd = Kbrd;
   Kbrd->TargetWindow = NULL;

   gc = D->gc;
   D->wc=NULL;
   FillClr = gc.fill_clr;
   FillClr = ConvertColor(FillClr,81);
   Bclr = gc.but_char;
   Bclr = ConvertColor(Bclr,83);
   Btrans = 0.0;
   if(Bclr >= 100) Bclr =0;
   strcpy(Sfac,Sfac1);
   strcpy(Kbrd->Sfac,Sfac);
   Kbrd->Bclr = Bclr;
   Kbrd->Bfont = Bfont;
   ButClr = FillClr;
   Kbrd->Btype=Btype;
   Kbrd->ButClr = ButClr;
   dtmp = D->d;
   pt = Kbrd;
   i=0;
   if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
   offset = i;

   GrpId = skeybrdGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   Kbrd->kbtype=1;
   Kbrd->GrpId = GrpId;
   CreateKeybrdGrps(D,Kbrd,offset,Vis, xo,yo);
   return GrpId;
}
int kgMakeDefaultKeybrd1(DIALOG *D,int xo,int yo,int Vis) {
   return kgMakeDefaultSkeybrd(D,xo,yo,Vis);
}
int kgMakeSkeybrd(DIALOG *D,int xo,int yo,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency) {

   int GrpId;
   WIDGETGRP *Gpt;
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
    int i=0,offset;
   int Red,Green,Blue;
   DIA *dtmp,*d;
   Gclr gc;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)malloc(sizeof(KEYBRD));
   D->Kbrd = Kbrd;
   Kbrd->TargetWindow = NULL;
   if(btype < 1) btype=1;
   gc = D->gc;
   FillClr = bkgrclr;
   FillClr = ConvertColor(FillClr,81);
   D->wc=NULL;
   Btype=btype;
   Bfont=bfont;
   Bclr = fontclr;
   Bclr = ConvertColor(Bclr,83);
   Btrans = transparency;
   ButClr = butclr;
   ButClr = ConvertColor(ButClr,82);
   strcpy(Sfac,Sfac1);
   strcpy(Kbrd->Sfac,Sfac);
   Kbrd->Bclr = Bclr;
   Kbrd->Bfont = Bfont;
   Rfac = rfac;
   Kbrd->Btype=Btype;
   Kbrd->Rfac=rfac;
   Kbrd->ButClr = ButClr;
   if(Bclr >= 100) Bclr =0;
   dtmp = D->d;
   pt = Kbrd;
   i=0;
   if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
   offset = i;
//   fprintf(stderr,"Offset= %d\n",offset);

   GrpId = skeybrdGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   kgShiftGrp(D,GrpId,xo-14,yo-7);
   Gpt->arg= v; // kulina will double free this; you may modify
   Kbrd->GrpId=GrpId;
   Kbrd->kbtype=1;
   CreateKeybrdGrps(D,Kbrd,offset,Vis, xo,yo);
   return GrpId;
}
int kgMakeKeybrd1(DIALOG *D,int xo,int yo,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency) {
  return kgMakeSkeybrd(D,xo,yo,Vis,btype,bfont,fontclr,butclr,
                       bkgrclr,rfac,transparency);
}
int kgMakeDefaultKeybrd2(DIALOG *D,int xo,int yo,int Vis) {
   int GrpId;
   WIDGETGRP *Gpt;
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
    int i=0,offset;
   DIA *dtmp,*d;
   Gclr gc;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)malloc(sizeof(KEYBRD));
   D->Kbrd = Kbrd;
   Kbrd->TargetWindow = NULL;
   gc = D->gc;
   D->wc=NULL;
   FillClr = gc.fill_clr;
   FillClr = ConvertColor(FillClr,81);
   Bclr = gc.but_char;
   Bclr = ConvertColor(Bclr,83);
   Btrans = 0.0;
   if(Bclr >= 100) Bclr =0;
   strcpy(Sfac,Sfac2);
   strcpy(Kbrd->Sfac,Sfac);
   Kbrd->Bclr = Bclr;
   Kbrd->Bfont = Bfont;
   ButClr = FillClr;
   Kbrd->Btype=Btype;
   Kbrd->ButClr = ButClr;
   Kbrd->Rfac = Rfac;
   dtmp = D->d;
   pt = Kbrd;
   i=0;
   if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
   offset = i;

   GrpId = keybrd2Group(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   Kbrd->kbtype=2;
   Kbrd->GrpId = GrpId;
   CreateKeybrdGrps(D,Kbrd,offset,Vis, xo,yo);
   return GrpId;
}
int kgMakeKeybrd2(DIALOG *D,int xo,int yo,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency) {

   int GrpId;
   WIDGETGRP *Gpt;
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
    int i=0,offset;
   int Red,Green,Blue;
   DIA *dtmp,*d;
   Gclr gc;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)malloc(sizeof(KEYBRD));
   D->Kbrd = Kbrd;
   Kbrd->TargetWindow = NULL;
   gc = D->gc;
   FillClr = bkgrclr;
   FillClr = ConvertColor(FillClr,81);
   D->wc=NULL;
   if(btype < 1) btype=1;
   Btype=btype;
   Bfont=bfont;
   Bclr = fontclr;
   Bclr = ConvertColor(Bclr,83);
   Rfac=rfac;
   Btrans = transparency;
   ButClr = butclr;
   ButClr = ConvertColor(ButClr,82);
   strcpy(Sfac,Sfac2);
   strcpy(Kbrd->Sfac,Sfac);
   Kbrd->Bclr = Bclr;
   Kbrd->Bfont = Bfont;
   Kbrd->Rfac = rfac;
   Kbrd->kbtype=2;
   Kbrd->Btype=Btype;
   Kbrd->ButClr = ButClr;
   if(Bclr >= 100) Bclr =0;
   dtmp = D->d;
   pt = Kbrd;
   i=0;
   if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
   offset = i;
//   fprintf(stderr,"Offset= %d\n",offset);

   GrpId = keybrd2Group(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   Kbrd->kbtype=2;
   Kbrd->GrpId = GrpId;
   CreateKeybrdGrps(D,Kbrd,offset,Vis, xo,yo);
   return GrpId;
}

int skeybrd( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = skeybrdGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 7;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 521;    /*  Length of Dialog */
  D.yl = 176;    /*  Width  of Dialog */
  D.Initfun = skeybrdinit;    /*   init fuction for Dialog */
  D.Cleanupfun = skeybrdcleanup;    /*   init fuction for Dialog */
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
  D.Callback = skeybrdCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = skeybrdResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = skeybrdWaitCallBack;  /*  Wait callback */
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
//  ModifyskeybrdGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
