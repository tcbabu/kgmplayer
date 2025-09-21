#include "kulina.h"
#include "keybrdCallbacks.h"
int RED=120,GREEN=120,BLUE=120;
int Btype=5;
static float  Rfac=0.2 ;

int kgShowKeybrd4(void *Tmp);
int kgShowKeybrd1(void *Tmp);
int kgShowKeybrd0(void *Tmp);
static char BUFF[100];
static char Sfac[]="54";
static int Bfont=16,Bclr=0;
static int FillClr=101;
static float Btrans=0.0;
static unsigned char R=230,G=60,B=0;
int ButClr=-1;
int kgMakeDefaultKeybrd0(DIALOG *D,int xo,int yo,int Vis);
static char *kgButtonTitle(char * str) {
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
static void ModifykeybrdGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/

   kgDefineColor(33,160,120,120);
   kgDefineColor(3,250,30,00);
   kgDefineColor(101,210,210,210);
   gc->ButtonFont=24;
   gc->ButtonFont=16;
   gc->bodr_clr= 33;
}



static int keybrdGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  int Red,Green,Blue;
  DIA *d=NULL,*dtmp;
  char *xpm0;// Pixmap info
#if 0
  xpm0 = (char *)malloc(3);
  strcpy(xpm0, (char *)"##");
#else
  kgGetDefaultRGB(FillClr,&Red,&Green,&Blue);
  xpm0 = (char *)MakeLightImage( 780,249,Red,Green,Blue,0.0);
#endif
  DIP p0 = { 
    'p',
    6,0,  
    786,249,  
    (void *)xpm0,
    FillClr, /* bkgr colour */ 
      1,0,Btrans /* border hide transparency*/ 
  };
  p0.transparency = Btrans;
  p0.bkgr_clr = FillClr;
  strcpy(p0.Wid,(char *)"Keybrdbkgr");
  p0.item = -1;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*10);
  butn1[0].sw=1;
  strcpy(butn1[0].title,kgButtonTitle((char *)"q"));
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=ButClr;
  butn1[0].butncode='';
  butn1[1].sw=1;
  strcpy(butn1[1].title,kgButtonTitle((char *)"w"));
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=ButClr;
  butn1[1].butncode='';
  butn1[2].sw=1;
  strcpy(butn1[2].title,kgButtonTitle((char *)"e"));
  butn1[2].xpmn=NULL;
  butn1[2].xpmp=NULL;
  butn1[2].xpmh=NULL;
  butn1[2].bkgr=ButClr;
  butn1[2].butncode='';
  butn1[3].sw=1;
  strcpy(butn1[3].title,kgButtonTitle((char *)"r"));
  butn1[3].xpmn=NULL;
  butn1[3].xpmp=NULL;
  butn1[3].xpmh=NULL;
  butn1[3].bkgr=ButClr;
  butn1[3].butncode='';
  butn1[4].sw=1;
  strcpy(butn1[4].title,kgButtonTitle((char *)"t"));
  butn1[4].xpmn=NULL;
  butn1[4].xpmp=NULL;
  butn1[4].xpmh=NULL;
  butn1[4].bkgr=ButClr;
  butn1[4].butncode='';
  butn1[5].sw=1;
  strcpy(butn1[5].title,kgButtonTitle((char *)"y"));
  butn1[5].xpmn=NULL;
  butn1[5].xpmp=NULL;
  butn1[5].xpmh=NULL;
  butn1[5].bkgr=ButClr;
  butn1[5].butncode='';
  butn1[6].sw=1;
  strcpy(butn1[6].title,kgButtonTitle((char *)"u"));
  butn1[6].xpmn=NULL;
  butn1[6].xpmp=NULL;
  butn1[6].xpmh=NULL;
  butn1[6].bkgr=ButClr;
  butn1[6].butncode='';
  butn1[7].sw=1;
  strcpy(butn1[7].title,kgButtonTitle((char *)"i"));
  butn1[7].xpmn=NULL;
  butn1[7].xpmp=NULL;
  butn1[7].xpmh=NULL;
  butn1[7].bkgr=ButClr;
  butn1[7].butncode='';
  butn1[8].sw=1;
  strcpy(butn1[8].title,kgButtonTitle((char *)"o"));
  butn1[8].xpmn=NULL;
  butn1[8].xpmp=NULL;
  butn1[8].xpmh=NULL;
  butn1[8].bkgr=ButClr;
  butn1[8].butncode='';
  butn1[9].sw=1;
  strcpy(butn1[9].title,kgButtonTitle((char *)"p"));
  butn1[9].xpmn=NULL;
  butn1[9].xpmp=NULL;
  butn1[9].xpmh=NULL;
  butn1[9].bkgr=ButClr;
  butn1[9].butncode='';
  DIN b1 = { 
    'n',
    90,64,  
    615,119,
    2,2,  
    50, 
    50, 
    10,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    keybrdbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"keybrdqwrt");
  b1.type=Btype;
  b1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*6);
  butn2[0].sw=1;
  strcpy(butn2[0].title,kgButtonTitle((char *)"{"));
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=ButClr;
  butn2[0].butncode='';
  butn2[1].sw=1;
  strcpy(butn2[1].title,kgButtonTitle((char *)"}"));
  butn2[1].xpmn=NULL;
  butn2[1].xpmp=NULL;
  butn2[1].xpmh=NULL;
  butn2[1].bkgr=ButClr;
  butn2[1].butncode='';
  butn2[2].sw=1;
  strcpy(butn2[2].title,kgButtonTitle((char *)"|"));
  butn2[2].xpmn=NULL;
  butn2[2].xpmp=NULL;
  butn2[2].xpmh=NULL;
  butn2[2].bkgr=ButClr;
  butn2[2].butncode='';
  butn2[3].sw=1;
  strcpy(butn2[3].title,kgButtonTitle((char *)"["));
  butn2[3].xpmn=NULL;
  butn2[3].xpmp=NULL;
  butn2[3].xpmh=NULL;
  butn2[3].bkgr=ButClr;
  butn2[3].butncode='';
  butn2[4].sw=1;
  strcpy(butn2[4].title,kgButtonTitle((char *)"]"));
  butn2[4].xpmn=NULL;
  butn2[4].xpmp=NULL;
  butn2[4].xpmh=NULL;
  butn2[4].bkgr=ButClr;
  butn2[4].butncode='';
  butn2[5].sw=1;
  strcpy(butn2[5].title,kgButtonTitle((char *)"\\"));
  butn2[5].xpmn=NULL;
  butn2[5].xpmp=NULL;
  butn2[5].xpmh=NULL;
  butn2[5].bkgr=ButClr;
  butn2[5].butncode='';
  DIN b2 = { 
    'n',
    612,64,  
    772,119,
    2,2,  
    50, 
    24, 
    3,2, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    keybrdbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b2.Wid,(char *)"keybrdWidget2");
  b2.item = -1;
  b2.type=Btype;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*26);
  butn3[0].sw=1;
  strcpy(butn3[0].title,kgButtonTitle((char *)"~"));
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=ButClr;
  butn3[0].butncode='';
  butn3[1].sw=1;
  strcpy(butn3[1].title,kgButtonTitle((char *)"!!"));
  butn3[1].xpmn=NULL;
  butn3[1].xpmp=NULL;
  butn3[1].xpmh=NULL;
  butn3[1].bkgr=ButClr;
  butn3[1].butncode='';
  butn3[2].sw=1;
  strcpy(butn3[2].title,kgButtonTitle((char *)"@"));
  butn3[2].xpmn=NULL;
  butn3[2].xpmp=NULL;
  butn3[2].xpmh=NULL;
  butn3[2].bkgr=ButClr;
  butn3[2].butncode='';
  butn3[3].sw=1;
  strcpy(butn3[3].title,kgButtonTitle((char *)"#"));
  butn3[3].xpmn=NULL;
  butn3[3].xpmp=NULL;
  butn3[3].xpmh=NULL;
  butn3[3].bkgr=ButClr;
  butn3[3].butncode='';
  butn3[4].sw=1;
  strcpy(butn3[4].title,kgButtonTitle((char *)"$"));
  butn3[4].xpmn=NULL;
  butn3[4].xpmp=NULL;
  butn3[4].xpmh=NULL;
  butn3[4].bkgr=ButClr;
  butn3[4].butncode='';
  butn3[5].sw=1;
  strcpy(butn3[5].title,kgButtonTitle((char *)"%"));
  butn3[5].xpmn=NULL;
  butn3[5].xpmp=NULL;
  butn3[5].xpmh=NULL;
  butn3[5].bkgr=ButClr;
  butn3[5].butncode='';
  butn3[6].sw=1;
  strcpy(butn3[6].title,kgButtonTitle((char *)"^"));
  butn3[6].xpmn=NULL;
  butn3[6].xpmp=NULL;
  butn3[6].xpmh=NULL;
  butn3[6].bkgr=ButClr;
  butn3[6].butncode='';
  butn3[7].sw=1;
  strcpy(butn3[7].title,kgButtonTitle((char *)"&"));
  butn3[7].xpmn=NULL;
  butn3[7].xpmp=NULL;
  butn3[7].xpmh=NULL;
  butn3[7].bkgr=ButClr;
  butn3[7].butncode='';
  butn3[8].sw=1;
  strcpy(butn3[8].title,kgButtonTitle((char *)"*"));
  butn3[8].xpmn=NULL;
  butn3[8].xpmp=NULL;
  butn3[8].xpmh=NULL;
  butn3[8].bkgr=ButClr;
  butn3[8].butncode='';
  butn3[9].sw=1;
  strcpy(butn3[9].title,kgButtonTitle((char *)"("));
  butn3[9].xpmn=NULL;
  butn3[9].xpmp=NULL;
  butn3[9].xpmh=NULL;
  butn3[9].bkgr=ButClr;
  butn3[9].butncode='';
  butn3[10].sw=1;
  strcpy(butn3[10].title,kgButtonTitle((char *)")"));
  butn3[10].xpmn=NULL;
  butn3[10].xpmp=NULL;
  butn3[10].xpmh=NULL;
  butn3[10].bkgr=ButClr;
  butn3[10].butncode='';
  butn3[11].sw=1;
  strcpy(butn3[11].title,kgButtonTitle((char *)"_"));
  butn3[11].xpmn=NULL;
  butn3[11].xpmp=NULL;
  butn3[11].xpmh=NULL;
  butn3[11].bkgr=ButClr;
  butn3[11].butncode='';
  butn3[12].sw=1;
  strcpy(butn3[12].title,kgButtonTitle((char *)"+"));
  butn3[12].xpmn=NULL;
  butn3[12].xpmp=NULL;
  butn3[12].xpmh=NULL;
  butn3[12].bkgr=ButClr;
  butn3[12].butncode='';
  butn3[13].sw=1;
  strcpy(butn3[13].title,kgButtonTitle((char *)"`"));
  butn3[13].xpmn=NULL;
  butn3[13].xpmp=NULL;
  butn3[13].xpmh=NULL;
  butn3[13].bkgr=ButClr;
  butn3[13].butncode='';
  butn3[14].sw=1;
  strcpy(butn3[14].title,kgButtonTitle((char *)"1"));
  butn3[14].xpmn=NULL;
  butn3[14].xpmp=NULL;
  butn3[14].xpmh=NULL;
  butn3[14].bkgr=ButClr;
  butn3[14].butncode='';
  butn3[15].sw=1;
  strcpy(butn3[15].title,kgButtonTitle((char *)"2"));
  butn3[15].xpmn=NULL;
  butn3[15].xpmp=NULL;
  butn3[15].xpmh=NULL;
  butn3[15].bkgr=ButClr;
  butn3[15].butncode='';
  butn3[16].sw=1;
  Procpy(butn3[16].title,(char *)"3");
  butn3[16].xpmn=NULL;
  butn3[16].xpmp=NULL;
  butn3[16].xpmh=NULL;
  butn3[16].bkgr=ButClr;
  butn3[16].butncode='';
  butn3[17].sw=1;
  Procpy(butn3[17].title,(char *)"4");
  butn3[17].xpmn=NULL;
  butn3[17].xpmp=NULL;
  butn3[17].xpmh=NULL;
  butn3[17].bkgr=ButClr;
  butn3[17].butncode='';
  butn3[18].sw=1;
  Procpy(butn3[18].title,(char *)"5");
  butn3[18].xpmn=NULL;
  butn3[18].xpmp=NULL;
  butn3[18].xpmh=NULL;
  butn3[18].bkgr=ButClr;
  butn3[18].butncode='';
  butn3[19].sw=1;
  Procpy(butn3[19].title,(char *)"6");
  butn3[19].xpmn=NULL;
  butn3[19].xpmp=NULL;
  butn3[19].xpmh=NULL;
  butn3[19].bkgr=ButClr;
  butn3[19].butncode='';
  butn3[20].sw=1;
  Procpy(butn3[20].title,(char *)"7");
  butn3[20].xpmn=NULL;
  butn3[20].xpmp=NULL;
  butn3[20].xpmh=NULL;
  butn3[20].bkgr=ButClr;
  butn3[20].butncode='';
  butn3[21].sw=1;
  Procpy(butn3[21].title,(char *)"8");
  butn3[21].xpmn=NULL;
  butn3[21].xpmp=NULL;
  butn3[21].xpmh=NULL;
  butn3[21].bkgr=ButClr;
  butn3[21].butncode='';
  butn3[22].sw=1;
  Procpy(butn3[22].title,(char *)"9");
  butn3[22].xpmn=NULL;
  butn3[22].xpmp=NULL;
  butn3[22].xpmh=NULL;
  butn3[22].bkgr=ButClr;
  butn3[22].butncode='';
  butn3[23].sw=1;
  Procpy(butn3[23].title,(char *)"0");
  butn3[23].xpmn=NULL;
  butn3[23].xpmp=NULL;
  butn3[23].xpmh=NULL;
  butn3[23].bkgr=ButClr;
  butn3[23].butncode='';
  butn3[24].sw=1;
  Procpy(butn3[24].title,(char *)"-");
  butn3[24].xpmn=NULL;
  butn3[24].xpmp=NULL;
  butn3[24].xpmh=NULL;
  butn3[24].bkgr=ButClr;
  butn3[24].butncode='';
  butn3[25].sw=1;
  Procpy(butn3[25].title,(char *)"=");
  butn3[25].xpmn=NULL;
  butn3[25].xpmp=NULL;
  butn3[25].xpmh=NULL;
  butn3[25].bkgr=ButClr;
  butn3[25].butncode='';
  DIN b3 = { 
    'n',
    20,10,  
    699,62,
    2,1,  
    50, 
    24, 
    13,2, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    keybrdbutton3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b3.Wid,(char *)"keybrdWidget3");
  b3.item = -1;
  b3.type=Btype;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=1;
  Procpy(butn4[0].title,(char *)"BackSpace");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=ButClr;
  butn4[0].butncode='';
  DIN b4 = { 
    'n',
    698,8,  
    772,60,
    2,2,  
    68, 
    50, 
    1,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    keybrdbutton4callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b4.Wid,(char *)"keybrdWidget4");
  b4.item = -1;
  b4.type=Btype;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn5[0].sw=1;
  Procpy(butn5[0].title,(char *)"Escape");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=ButClr;
  butn5[0].butncode='';
  DIN b5 = { 
    'n',
    20,64,  
    94,116,
    2,2,  
    62, 
    50, 
    1,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    keybrdbutton5callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"keybrdWidget5");
  b5.item = -1;
  b5.type=Btype;
  BUT_STR  *butn6=NULL; 
  butn6= (BUT_STR *)malloc(sizeof(BUT_STR)*9);
  butn6[0].sw=1;
  Procpy(butn6[0].title,(char *)"a");
  butn6[0].xpmn=NULL;
  butn6[0].xpmp=NULL;
  butn6[0].xpmh=NULL;
  butn6[0].bkgr=ButClr;
  butn6[0].butncode='';
  butn6[1].sw=1;
  Procpy(butn6[1].title,(char *)"s");
  butn6[1].xpmn=NULL;
  butn6[1].xpmp=NULL;
  butn6[1].xpmh=NULL;
  butn6[1].bkgr=ButClr;
  butn6[1].butncode='';
  butn6[2].sw=1;
  Procpy(butn6[2].title,(char *)"d");
  butn6[2].xpmn=NULL;
  butn6[2].xpmp=NULL;
  butn6[2].xpmh=NULL;
  butn6[2].bkgr=ButClr;
  butn6[2].butncode='';
  butn6[3].sw=1;
  Procpy(butn6[3].title,(char *)"f");
  butn6[3].xpmn=NULL;
  butn6[3].xpmp=NULL;
  butn6[3].xpmh=NULL;
  butn6[3].bkgr=ButClr;
  butn6[3].butncode='';
  butn6[4].sw=1;
  Procpy(butn6[4].title,(char *)"g");
  butn6[4].xpmn=NULL;
  butn6[4].xpmp=NULL;
  butn6[4].xpmh=NULL;
  butn6[4].bkgr=ButClr;
  butn6[4].butncode='';
  butn6[5].sw=1;
  Procpy(butn6[5].title,(char *)"h");
  butn6[5].xpmn=NULL;
  butn6[5].xpmp=NULL;
  butn6[5].xpmh=NULL;
  butn6[5].bkgr=ButClr;
  butn6[5].butncode='';
  butn6[6].sw=1;
  Procpy(butn6[6].title,(char *)"j");
  butn6[6].xpmn=NULL;
  butn6[6].xpmp=NULL;
  butn6[6].xpmh=NULL;
  butn6[6].bkgr=ButClr;
  butn6[6].butncode='';
  butn6[7].sw=1;
  Procpy(butn6[7].title,(char *)"k");
  butn6[7].xpmn=NULL;
  butn6[7].xpmp=NULL;
  butn6[7].xpmh=NULL;
  butn6[7].bkgr=ButClr;
  butn6[7].butncode='';
  butn6[8].sw=1;
  Procpy(butn6[8].title,(char *)"l");
  butn6[8].xpmn=NULL;
  butn6[8].xpmp=NULL;
  butn6[8].xpmh=NULL;
  butn6[8].bkgr=ButClr;
  butn6[8].butncode='';
  DIN b6 = { 
    'n',
    103,123,  
    575,180,
    2,2,  
    50, 
    50, 
    9,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn6, 
    keybrdbutton6callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b6.Wid,(char *)"keybrdasdfg");
  b6.item = -1;
  b6.type=Btype;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*7);
  butn7[0].sw=1;
  Procpy(butn7[0].title,(char *)"z");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=ButClr;
  butn7[0].butncode='';
  butn7[1].sw=1;
  Procpy(butn7[1].title,(char *)"x");
  butn7[1].xpmn=NULL;
  butn7[1].xpmp=NULL;
  butn7[1].xpmh=NULL;
  butn7[1].bkgr=ButClr;
  butn7[1].butncode='';
  butn7[2].sw=1;
  Procpy(butn7[2].title,(char *)"c");
  butn7[2].xpmn=NULL;
  butn7[2].xpmp=NULL;
  butn7[2].xpmh=NULL;
  butn7[2].bkgr=ButClr;
  butn7[2].butncode='';
  butn7[3].sw=1;
  Procpy(butn7[3].title,(char *)"v");
  butn7[3].xpmn=NULL;
  butn7[3].xpmp=NULL;
  butn7[3].xpmh=NULL;
  butn7[3].bkgr=ButClr;
  butn7[3].butncode='';
  butn7[4].sw=1;
  Procpy(butn7[4].title,(char *)"b");
  butn7[4].xpmn=NULL;
  butn7[4].xpmp=NULL;
  butn7[4].xpmh=NULL;
  butn7[4].bkgr=ButClr;
  butn7[4].butncode='';
  butn7[5].sw=1;
  Procpy(butn7[5].title,(char *)"n");
  butn7[5].xpmn=NULL;
  butn7[5].xpmp=NULL;
  butn7[5].xpmh=NULL;
  butn7[5].bkgr=ButClr;
  butn7[5].butncode='';
  butn7[6].sw=1;
  Procpy(butn7[6].title,(char *)"m");
  butn7[6].xpmn=NULL;
  butn7[6].xpmp=NULL;
  butn7[6].xpmh=NULL;
  butn7[6].bkgr=ButClr;
  butn7[6].butncode='';
  DIN b7 = { 
    'n',
    121,181,  
    490,237,
    2,2,  
    50, 
    50, 
    7,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    keybrdbutton7callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"keybrdzxcvb");
  b7.item = -1;
  b7.type=Btype;
  BUT_STR  *butn8=NULL; 
  butn8= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn8[0].sw=1;
  Procpy(butn8[0].title,(char *)"Caps Lock");
  butn8[0].xpmn=NULL;
  butn8[0].xpmp=NULL;
  butn8[0].xpmh=NULL;
  butn8[0].bkgr=ButClr;
  butn8[0].butncode='';
  DIN b8 = { 
    'n',
    19,123,  
    107,179,
    2,2,  
    80, 
    50, 
    1,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn8, 
    keybrdbutton8callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b8.Wid,(char *)"CapsOff");
  b8.item = -1;
  b8.type=Btype;
  BUT_STR  *butn9=NULL; 
  butn9= (BUT_STR *)malloc(sizeof(BUT_STR)*6);
  butn9[0].sw=1;
  Procpy(butn9[0].title,(char *)"<");
  butn9[0].xpmn=NULL;
  butn9[0].xpmp=NULL;
  butn9[0].xpmh=NULL;
  butn9[0].bkgr=ButClr;
  butn9[0].butncode='';
  butn9[1].sw=1;
  Procpy(butn9[1].title,(char *)">");
  butn9[1].xpmn=NULL;
  butn9[1].xpmp=NULL;
  butn9[1].xpmh=NULL;
  butn9[1].bkgr=ButClr;
  butn9[1].butncode='';
  butn9[2].sw=1;
  Procpy(butn9[2].title,(char *)"?");
  butn9[2].xpmn=NULL;
  butn9[2].xpmp=NULL;
  butn9[2].xpmh=NULL;
  butn9[2].bkgr=ButClr;
  butn9[2].butncode='';
  butn9[3].sw=1;
  Procpy(butn9[3].title,(char *)",");
  butn9[3].xpmn=NULL;
  butn9[3].xpmp=NULL;
  butn9[3].xpmh=NULL;
  butn9[3].bkgr=ButClr;
  butn9[3].butncode='';
  butn9[4].sw=1;
  Procpy(butn9[4].title,(char *)".");
  butn9[4].xpmn=NULL;
  butn9[4].xpmp=NULL;
  butn9[4].xpmh=NULL;
  butn9[4].bkgr=ButClr;
  butn9[4].butncode='';
  butn9[5].sw=1;
  Procpy(butn9[5].title,(char *)"/");
  butn9[5].xpmn=NULL;
  butn9[5].xpmp=NULL;
  butn9[5].xpmh=NULL;
  butn9[5].bkgr=ButClr;
  butn9[5].butncode='';
  DIN b9 = { 
    'n',
    559,181,  
    725,237,
    2,2,  
    50, 
    24, 
    3,2, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn9, 
    keybrdbutton9callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b9.Wid,(char *)"keybrdWidget9");
  b9.item = -1;
  b9.type=Btype;
  BUT_STR  *butn10=NULL; 
  butn10= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn10[0].sw=1;
  Procpy(butn10[0].title,(char *)":");
  butn10[0].xpmn=NULL;
  butn10[0].xpmp=NULL;
  butn10[0].xpmh=NULL;
  butn10[0].bkgr=ButClr;
  butn10[0].butncode='';
  butn10[1].sw=1;
  Procpy(butn10[1].title,(char *)"\"");
  butn10[1].xpmn=NULL;
  butn10[1].xpmp=NULL;
  butn10[1].xpmh=NULL;
  butn10[1].bkgr=ButClr;
  butn10[1].butncode='';
  butn10[2].sw=1;
  Procpy(butn10[2].title,(char *)":");
  butn10[2].xpmn=NULL;
  butn10[2].xpmp=NULL;
  butn10[2].xpmh=NULL;
  butn10[2].bkgr=ButClr;
  butn10[2].butncode='';
  butn10[3].sw=1;
  Procpy(butn10[3].title,(char *)"'");
  butn10[3].xpmn=NULL;
  butn10[3].xpmp=NULL;
  butn10[3].xpmh=NULL;
  butn10[3].bkgr=ButClr;
  butn10[3].butncode='';
  DIN b10 = { 
    'n',
    577,124,  
    685,179,
    2,2,  
    50, 
    24, 
    2,2, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn10, 
    keybrdbutton10callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b10.Wid,(char *)"keybrdWidget10");
  b10.item = -1;
  b10.type=Btype;
  BUT_STR  *butn11=NULL; 
  butn11= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn11[0].sw=1;
  Procpy(butn11[0].title,(char *)"Enter");
  butn11[0].xpmn=NULL;
  butn11[0].xpmp=NULL;
  butn11[0].xpmh=NULL;
  butn11[0].bkgr=ButClr;
  butn11[0].butncode='';
  DIN b11 = { 
    'n',
    687,124,  
    774,180,
    2,2,  
    80, 
    50, 
    1,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn11, 
    keybrdbutton11callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b11.Wid,(char *)"keybrdWidget11");
  b11.item = -1;
  b11.type=Btype;
  BUT_STR  *butn12=NULL; 
  butn12= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn12[0].sw=1;
  Procpy(butn12[0].title,(char *)"Space");
  butn12[0].xpmn=NULL;
  butn12[0].xpmp=NULL;
  butn12[0].xpmh=NULL;
  butn12[0].bkgr=ButClr;
  butn12[0].butncode='';
  DIN b12 = { 
    'n',
    489,181,  
    560,237,
    2,2,  
    64, 
    50, 
    1,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn12, 
    keybrdbutton12callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b12.Wid,(char *)"keybrdWidget12");
  b12.item = -1;
  b12.type=Btype;
  BUT_STR  *butn13=NULL; 
  butn13= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn13[0].sw=1;
  strcpy(butn13[0].title,(char *)"");
  butn13[0].xpmn=kgRightImage(12,RED,GREEN,BLUE);
  butn13[0].xpmp=NULL;
  butn13[0].xpmh=NULL;
  butn13[0].bkgr=ButClr;
  butn13[0].butncode='';
  butn13[1].sw=1;
  strcpy(butn13[1].title,(char *)"");
  butn13[1].xpmn=kgLeftImage(12,RED,GREEN,BLUE);
  butn13[1].xpmp=NULL;
  butn13[1].xpmh=NULL;
  butn13[1].bkgr=ButClr;
  butn13[1].butncode='';
  DIN b13 = { 
    'n',
    85,181,  
    123,237,
    2,2,  
    32, 
    24, 
    1,2, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn13, 
    keybrdbutton13callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b13.Wid,(char *)"keybrdWidget14");
  b13.item = -1;
  b13.type=Btype;
  BUT_STR  *butn14=NULL; 
  butn14= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn14[0].sw=1;
  Procpy(butn14[0].title,(char *)"Shift");
  butn14[0].xpmn=NULL;
  butn14[0].xpmp=NULL;
  butn14[0].xpmh=NULL;
  butn14[0].bkgr=ButClr;
  butn14[0].butncode='';
  DIN b14 = { 
    'n',
    18,181,  
    87,237,
    2,2,  
    64, 
    50, 
    1,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn14, 
    keybrdbutton14callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b14.Wid,(char *)"Shift");
  b14.item = -1;
  b14.type=Btype;
  BUT_STR  *butn15=NULL; 
  butn15= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn15[0].sw=1;
  strcpy(butn15[0].title,kgButtonTitle((char *)"End"));
  butn15[0].xpmn=NULL;
  butn15[0].xpmp=NULL;
  butn15[0].xpmh=NULL;
  butn15[0].bkgr=ButClr;
  butn15[0].butncode='';
  butn15[1].sw=1;
  strcpy(butn15[1].title,kgButtonTitle((char *)"Home"));
  butn15[1].xpmn=NULL;
  butn15[1].xpmp=NULL;
  butn15[1].xpmh=NULL;
  butn15[1].bkgr=ButClr;
  butn15[1].butncode='';
  DIN b15 = { 
    'n',
    723,181,  
    774,237,
    2,2,  
    44, 
    24, 
    1,2, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn15, 
    keybrdbutton15callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b15.Wid,(char *)"keybrdWidget15");
  b15.item = -1;
  b15.type=Btype;
  BUT_STR  *butn16=NULL; 
  butn16= (BUT_STR *)malloc(sizeof(BUT_STR)*10);
  butn16[0].sw=1;
  Procpy(butn16[0].title,(char *)"Q");
  butn16[0].xpmn=NULL;
  butn16[0].xpmp=NULL;
  butn16[0].xpmh=NULL;
  butn16[0].bkgr=ButClr;
  butn16[0].butncode='';
  butn16[1].sw=1;
  Procpy(butn16[1].title,(char *)"W");
  butn16[1].xpmn=NULL;
  butn16[1].xpmp=NULL;
  butn16[1].xpmh=NULL;
  butn16[1].bkgr=ButClr;
  butn16[1].butncode='';
  butn16[2].sw=1;
  Procpy(butn16[2].title,(char *)"E");
  butn16[2].xpmn=NULL;
  butn16[2].xpmp=NULL;
  butn16[2].xpmh=NULL;
  butn16[2].bkgr=ButClr;
  butn16[2].butncode='';
  butn16[3].sw=1;
  Procpy(butn16[3].title,(char *)"R");
  butn16[3].xpmn=NULL;
  butn16[3].xpmp=NULL;
  butn16[3].xpmh=NULL;
  butn16[3].bkgr=ButClr;
  butn16[3].butncode='';
  butn16[4].sw=1;
  Procpy(butn16[4].title,(char *)"T");
  butn16[4].xpmn=NULL;
  butn16[4].xpmp=NULL;
  butn16[4].xpmh=NULL;
  butn16[4].bkgr=ButClr;
  butn16[4].butncode='';
  butn16[5].sw=1;
  Procpy(butn16[5].title,(char *)"Y");
  butn16[5].xpmn=NULL;
  butn16[5].xpmp=NULL;
  butn16[5].xpmh=NULL;
  butn16[5].bkgr=ButClr;
  butn16[5].butncode='';
  butn16[6].sw=1;
  Procpy(butn16[6].title,(char *)"U");
  butn16[6].xpmn=NULL;
  butn16[6].xpmp=NULL;
  butn16[6].xpmh=NULL;
  butn16[6].bkgr=ButClr;
  butn16[6].butncode='';
  butn16[7].sw=1;
  Procpy(butn16[7].title,(char *)"I");
  butn16[7].xpmn=NULL;
  butn16[7].xpmp=NULL;
  butn16[7].xpmh=NULL;
  butn16[7].bkgr=ButClr;
  butn16[7].butncode='';
  butn16[8].sw=1;
  Procpy(butn16[8].title,(char *)"O");
  butn16[8].xpmn=NULL;
  butn16[8].xpmp=NULL;
  butn16[8].xpmh=NULL;
  butn16[8].bkgr=ButClr;
  butn16[8].butncode='';
  butn16[9].sw=1;
  Procpy(butn16[9].title,(char *)"P");
  butn16[9].xpmn=NULL;
  butn16[9].xpmp=NULL;
  butn16[9].xpmh=NULL;
  butn16[9].bkgr=ButClr;
  butn16[9].butncode='';
  DIN b16 = { 
    'n',
    90,64,  
    615,119,
    2,2,  
    50, 
    50, 
    10,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn16, 
    keybrdbutton16callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b16.Wid,(char *)"keybrDQWRT");
  b16.item = -1;
  b16.type=Btype;
  BUT_STR  *butn17=NULL; 
  butn17= (BUT_STR *)malloc(sizeof(BUT_STR)*9);
  butn17[0].sw=1;
  Procpy(butn17[0].title,(char *)"A");
  butn17[0].xpmn=NULL;
  butn17[0].xpmp=NULL;
  butn17[0].xpmh=NULL;
  butn17[0].bkgr=ButClr;
  butn17[0].butncode='';
  butn17[1].sw=1;
  Procpy(butn17[1].title,(char *)"S");
  butn17[1].xpmn=NULL;
  butn17[1].xpmp=NULL;
  butn17[1].xpmh=NULL;
  butn17[1].bkgr=ButClr;
  butn17[1].butncode='';
  butn17[2].sw=1;
  Procpy(butn17[2].title,(char *)"D");
  butn17[2].xpmn=NULL;
  butn17[2].xpmp=NULL;
  butn17[2].xpmh=NULL;
  butn17[2].bkgr=ButClr;
  butn17[2].butncode='';
  butn17[3].sw=1;
  Procpy(butn17[3].title,(char *)"F");
  butn17[3].xpmn=NULL;
  butn17[3].xpmp=NULL;
  butn17[3].xpmh=NULL;
  butn17[3].bkgr=ButClr;
  butn17[3].butncode='';
  butn17[4].sw=1;
  Procpy(butn17[4].title,(char *)"G");
  butn17[4].xpmn=NULL;
  butn17[4].xpmp=NULL;
  butn17[4].xpmh=NULL;
  butn17[4].bkgr=ButClr;
  butn17[4].butncode='';
  butn17[5].sw=1;
  Procpy(butn17[5].title,(char *)"H");
  butn17[5].xpmn=NULL;
  butn17[5].xpmp=NULL;
  butn17[5].xpmh=NULL;
  butn17[5].bkgr=ButClr;
  butn17[5].butncode='';
  butn17[6].sw=1;
  Procpy(butn17[6].title,(char *)"J");
  butn17[6].xpmn=NULL;
  butn17[6].xpmp=NULL;
  butn17[6].xpmh=NULL;
  butn17[6].bkgr=ButClr;
  butn17[6].butncode='';
  butn17[7].sw=1;
  Procpy(butn17[7].title,(char *)"K");
  butn17[7].xpmn=NULL;
  butn17[7].xpmp=NULL;
  butn17[7].xpmh=NULL;
  butn17[7].bkgr=ButClr;
  butn17[7].butncode='';
  butn17[8].sw=1;
  Procpy(butn17[8].title,(char *)"L");
  butn17[8].xpmn=NULL;
  butn17[8].xpmp=NULL;
  butn17[8].xpmh=NULL;
  butn17[8].bkgr=ButClr;
  butn17[8].butncode='';
  DIN b17 = { 
    'n',
    103,123,  
    575,180,
    2,2,  
    50, 
    50, 
    9,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn17, 
    keybrdbutton17callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b17.Wid,(char *)"keybrdASDFG");
  b17.item = -1;
  b17.type=Btype;
  BUT_STR  *butn18=NULL; 
  butn18= (BUT_STR *)malloc(sizeof(BUT_STR)*7);
  butn18[0].sw=1;
  Procpy(butn18[0].title,(char *)"Z");
  butn18[0].xpmn=NULL;
  butn18[0].xpmp=NULL;
  butn18[0].xpmh=NULL;
  butn18[0].bkgr=ButClr;
  butn18[0].butncode='';
  butn18[1].sw=1;
  Procpy(butn18[1].title,(char *)"X");
  butn18[1].xpmn=NULL;
  butn18[1].xpmp=NULL;
  butn18[1].xpmh=NULL;
  butn18[1].bkgr=ButClr;
  butn18[1].butncode='';
  butn18[2].sw=1;
  Procpy(butn18[2].title,(char *)"C");
  butn18[2].xpmn=NULL;
  butn18[2].xpmp=NULL;
  butn18[2].xpmh=NULL;
  butn18[2].bkgr=ButClr;
  butn18[2].butncode='';
  butn18[3].sw=1;
  Procpy(butn18[3].title,(char *)"V");
  butn18[3].xpmn=NULL;
  butn18[3].xpmp=NULL;
  butn18[3].xpmh=NULL;
  butn18[3].bkgr=ButClr;
  butn18[3].butncode='';
  butn18[4].sw=1;
  Procpy(butn18[4].title,(char *)"B");
  butn18[4].xpmn=NULL;
  butn18[4].xpmp=NULL;
  butn18[4].xpmh=NULL;
  butn18[4].bkgr=ButClr;
  butn18[4].butncode='';
  butn18[5].sw=1;
  Procpy(butn18[5].title,(char *)"N");
  butn18[5].xpmn=NULL;
  butn18[5].xpmp=NULL;
  butn18[5].xpmh=NULL;
  butn18[5].bkgr=ButClr;
  butn18[5].butncode='';
  butn18[6].sw=1;
  Procpy(butn18[6].title,(char *)"M");
  butn18[6].xpmn=NULL;
  butn18[6].xpmp=NULL;
  butn18[6].xpmh=NULL;
  butn18[6].bkgr=ButClr;
  butn18[6].butncode='';
  DIN b18 = { 
    'n',
    121,181,  
    490,237,
    2,2,  
    50, 
    50, 
    7,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn18, 
    keybrdbutton18callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b18.Wid,(char *)"keybrdZXCVB");
  b18.item = -1;
  b18.type=Btype;
  BUT_STR  *butn19=NULL; 
  butn19= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn19[0].sw=1;
  Procpy(butn19[0].title,(char *)"Caps Lock");
  butn19[0].xpmn=NULL;
  butn19[0].xpmp=NULL;
  butn19[0].xpmh=NULL;
  butn19[0].bkgr=ButClr;
  butn19[0].butncode='';
  DIN b19 = { 
    'n',
    19,123,  
    107,179,
    2,2,  
    80, 
    50, 
    1,1, 
    Btype,Rfac,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn19, 
    keybrdbutton19callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b19.Wid,(char *)"CapsOn");
  b19.item = -1;
  b19.type=Btype;
  char *xpm20;// Pixmap info
#if 0
  xpm20 = (char *)malloc(3);
  strcpy(xpm20, (char *)"##");
#endif
  xpm20 = MakeLightImage(6,6,140,140,140,0.5);
  DIP p20 = { 
    'p',
    85,134,  
    93,142,  
    (void *)xpm20,
    -1, /* bkgr colour */ 
      0,0,0.000000 /* border hide transparency*/ 
  };
  strcpy(p20.Wid,(char *)"Capslockoff");
  p20.item = -1;
  char *xpm21;// Pixmap info
#if 0
  xpm21 = (char *)malloc(3);
  strcpy(xpm21, (char *)"##");
#endif
  xpm21 = MakeLightImage(8,8,250,90,0,0.5);
  DIP p21 = { 
    'p',
    85,134,  
    93,142,  
    (void *)xpm21,
    -3, /* bkgr colour */ 
      0,0,0.0 /* border hide transparency*/ 
  };
  strcpy(p21.Wid,(char *)"Capslockon");
  p21.item = -1;
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
  keybrdbutton1init(&b1,pt) ;
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton2init(&b2,pt) ;
  *d[2].N = b2;
  d[2].N->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton3init(&b3,pt) ;
  *d[3].N = b3;
  d[3].N->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton4init(&b4,pt) ;
  *d[4].N = b4;
  d[4].N->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton5init(&b5,pt) ;
  *d[5].N = b5;
  d[5].N->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton6init(&b6,pt) ;
  *d[6].N = b6;
  d[6].N->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton7init(&b7,pt) ;
  *d[7].N = b7;
  d[7].N->item = -1;
  d[8].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton8init(&b8,pt) ;
  *d[8].N = b8;
  d[8].N->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton9init(&b9,pt) ;
  *d[9].N = b9;
  d[9].N->item = -1;
  d[10].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton10init(&b10,pt) ;
  *d[10].N = b10;
  d[10].N->item = -1;
  d[11].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton11init(&b11,pt) ;
  *d[11].N = b11;
  d[11].N->item = -1;
  d[12].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton12init(&b12,pt) ;
  *d[12].N = b12;
  d[12].N->item = -1;
  d[13].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton13init(&b13,pt) ;
  *d[13].N = b13;
  d[13].N->item = -1;
  d[14].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton14init(&b14,pt) ;
  *d[14].N = b14;
  d[14].N->item = -1;
  d[15].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton15init(&b15,pt) ;
  *d[15].N = b15;
  d[15].N->item = -1;
  d[16].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton16init(&b16,pt) ;
  *d[16].N = b16;
  d[16].N->item = -1;
  d[17].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton17init(&b17,pt) ;
  *d[17].N = b17;
  d[17].N->item = -1;
  d[18].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton18init(&b18,pt) ;
  *d[18].N = b18;
  d[18].N->item = -1;
  d[19].t = (DIT *)malloc(sizeof(DIN));
  keybrdbutton19init(&b19,pt) ;
  *d[19].N = b19;
  d[19].N->item = -1;
  d[20].t = (DIT *)malloc(sizeof(DIP));
  *d[20].p = p20;
  d[20].p->item = -1;
  d[21].t = (DIT *)malloc(sizeof(DIP));
  *d[21].p = p21;
  d[21].p->item = -1;
  d[22].t = NULL;
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
   int i;
   DIA *d;
   d = (DIA *)(D->d);
   i = offset;
   while(d[i].t != NULL) {
     if(xm > (d[i].t->x1) ) xm = d[i].t->x1;
     if(ym > (d[i].t->y1) ) ym = d[i].t->y1;
     i++;
   }
//   kgShiftGrp(D,GrpId,xo-43,yo-28);
   kgShiftGrp(D,Kbrd->GrpId,xo-xm,yo-ym);
   d = D->d;
   Kbrd->sgrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->sgrp,d[0+offset].t);
   kgAddtoGrp(D,Kbrd->sgrp,d[5+offset].t);
   kgAddtoGrp(D,Kbrd->sgrp,d[6+offset].t);
   Kbrd->offgrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->offgrp,d[7+offset].t);
   kgAddtoGrp(D,Kbrd->offgrp,d[19+offset].t);
   Kbrd->cgrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->cgrp,d[16+offset].t);
   kgAddtoGrp(D,Kbrd->cgrp,d[15+offset].t);
   kgAddtoGrp(D,Kbrd->cgrp,d[17+offset].t);
   Kbrd->ongrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->ongrp,d[18+offset].t);
   kgAddtoGrp(D,Kbrd->ongrp,d[20+offset].t);
   kgSetGrpVisibility(D,Kbrd->cgrp,0);
   kgSetGrpVisibility(D,Kbrd->ongrp,0);
   Kbrd->Coff = d[7+offset].t;
   Kbrd->Con = d[18+offset].t;
   Kbrd->Loff = d[19+offset].t;
   Kbrd->Lon = d[20+offset].t;
   Kbrd->CurWid = -1;
   Kbrd->Vis = Vis;
   Kbrd->D = D;
   Kbrd->ShiftPress=0;
   Kbrd->CapsLock=0;
   if(Vis==0) {
    kgSetGrpVisibility(D,Kbrd->GrpId,0);
   }
   return 1;
}
int kgMakeKeybrd3(DIALOG *D,int xo,int yo,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency) {
   int GrpId;
   int i=0,offset;
   DIA *dtmp,*d;
   WIDGETGRP *Gpt;
   Gclr gc;
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   int Red,Green,Blue;
   KEYBRD *Kbrd;
   D->Kbrd = (void *) malloc(sizeof(KEYBRD));
   if(btype<1) btype=1;
   Kbrd = D->Kbrd;
   Kbrd->TargetWindow = NULL;
   Kbrd->kbtype=3;
#if 0
   kgDefineColor(93,R,G,B);
   kgDefineColor(3,250,230,00);
   kgDefineColor(101,210,210,210);
#endif
   Rfac = rfac;
   gc = D->gc;
   FillClr = ConvertColor(bkgrclr,81);
   Btype=btype;
   Bfont=bfont;
   Bclr = fontclr;
   Bclr = ConvertColor(fontclr,83);
   Btrans = transparency;
   ButClr = ConvertColor(butclr,82);
   Kbrd->Btype=Btype;
   Kbrd->Rfac = rfac;
   if(Bclr >= 100) Bclr =0;
   Kbrd->Bclr = Bclr;
   Kbrd->FillClr= FillClr;
   Kbrd->ButClr= ButClr;
   dtmp = D->d;
   i=0;
   if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
   offset = i+1;

   GrpId = keybrdGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   Kbrd->GrpId = GrpId;
   CreateKeybrdGrps(D,Kbrd,offset,Vis,xo,yo);
   return GrpId;
}
int kgMakeDefaultKeybrd3(DIALOG *D,int xo,int yo,int Vis) {
   int GrpId;
   int i=0,offset;
   DIA *dtmp,*d;
   WIDGETGRP *Gpt;
   Gclr gc;
   KEYBRD *Kbrd;
   D->Kbrd = (void *) malloc(sizeof(KEYBRD));
   Kbrd = D->Kbrd;
   Kbrd->TargetWindow = NULL;
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   Kbrd->kbtype=3;
   gc = D->gc;
   FillClr = gc.fill_clr;
   Bfont=gc.ButtonFont;
   Bclr = gc.but_char;
   Btrans = 0.0;
   if(Bclr >= 100) Bclr =0;
   ButClr = FillClr;
   Btype=5;
   Kbrd->Btype=Btype;
   Kbrd->Bclr = Bclr;
   Kbrd->FillClr= FillClr;
   Kbrd->ButClr= ButClr;
   dtmp = D->d;
   i=0;
   if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
   offset = i+1;

   GrpId = keybrdGroup(D,v,pt);
   kgShiftGrp(D,GrpId,xo-6,yo);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   Kbrd->GrpId = GrpId;
   CreateKeybrdGrps(D,Kbrd,offset,Vis,xo,yo);
   return GrpId;
}

int kgShowKeybrd3(void *Tmp) {
   DIALOG *D;
   KEYBRD *Kbrd;
   D = (DIALOG *) Tmp;
   Kbrd = D->Kbrd;
   if(Kbrd->Vis) return 0;
//   D= Kbrd->D;
   if(Kbrd->kbtype ==3) {
     kgSetGrpVisibility(D,Kbrd->GrpId,1);
     kgSetGrpVisibility(D,Kbrd->cgrp,0);
     kgSetGrpVisibility(D,Kbrd->ongrp,0);
     kgSetGrpVisibility(D,Kbrd->sgrp,1);
     kgSetGrpVisibility(D,Kbrd->offgrp,1);
     if(D->wc != NULL) {
       kgUpdateGrp(D,Kbrd->GrpId);
       kgUpdateGrp(D,Kbrd->sgrp);
       kgUpdateGrp(D,Kbrd->offgrp);
       kgUpdateOn(D);
     }
   }
   Kbrd->ShiftPress=0;
   Kbrd->CapsLock=0;
   Kbrd->Vis = 1;
   return Kbrd->GrpId;
}
int kgShowKeybrd4(void *Tmp) {
   DIALOG *D;
   KEYBRD *Kbrd;
   D = (DIALOG *)Tmp;
   Kbrd = (KEYBRD *)D->Kbrd;
   if(Kbrd->Vis) return 0;
    kgSetGrpVisibility(D,Kbrd->grp2,0);
    kgSetGrpVisibility(D,Kbrd->cgrp,0);
    kgSetGrpVisibility(D,Kbrd->ongrp,0);
    kgSetGrpVisibility(D,Kbrd->grp1,1);
    kgSetGrpVisibility(D,Kbrd->grp3,1);
    kgSetGrpVisibility(D,Kbrd->sgrp,1);
    kgSetGrpVisibility(D,Kbrd->offgrp,1);
   Kbrd->ShiftPress=0;
   Kbrd->CapsLock=0;
   if(D->wc != NULL) {
     kgUpdateGrp(D,Kbrd->grp1);
     kgUpdateGrp(D,Kbrd->grp3);
     kgUpdateGrp(D,Kbrd->sgrp);
     kgUpdateGrp(D,Kbrd->offgrp);
     kgUpdateOn(D);
   }
   Kbrd->Vis = 1;
   return Kbrd->GrpId;
}
int kgShowKeybrd(void *Tmp) {
   DIALOG *D;
   KEYBRD *Kbrd;
   D = (DIALOG *) Tmp;
   Kbrd = D->Kbrd;
   if(Kbrd->Vis) return 0;
   switch(Kbrd->kbtype) {
     case 0:
      kgShowKeybrd0(Tmp);
      break;
     case 1:
     case 2:
      kgShowKeybrd1(Tmp);
      break;
     case 3:
      kgShowKeybrd3(Tmp);
      break;
     default:
//      kgShowKeybrd0(Tmp);
      kgShowKeybrd4(Tmp);
      break;
   }
   Kbrd->Vis =1;
   return Kbrd->GrpId;
}

int kgHideKeybrd(void *Tmp) {
   DIALOG *D;
   KEYBRD *Kbrd;
   D = (DIALOG *) Tmp;
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
int kgMakeKeybrd(void *Tmp,int Type,int Vis,int Btype,int Bfont,int Charclr,int Butclr,int Fillclr,float Rfac,float Trans) {
    int xo=0,yo=0;
    switch(Type) {
       case 0:
       default:
       break;
       case 1:
       return kgMakeKeybrd1((DIALOG *)Tmp,xo,yo,Vis,Btype,Bfont,
                             Charclr,Butclr,Fillclr,Rfac,Trans);
       break;
       case 2:
       return kgMakeKeybrd2((DIALOG *)Tmp,xo,yo,Vis,Btype,Bfont,
                             Charclr,Butclr,Fillclr,Rfac,Trans);
       break;
       case 3:
       return kgMakeKeybrd3((DIALOG *)Tmp,xo,yo,Vis,Btype,Bfont,
                             Charclr,Butclr,Fillclr,Rfac,Trans);
       break;
    }
}
int kgMakeDefaultKeybrd(void *Tmp,int Type,int Vis) {
    int xo=0,yo=0;
    switch(Type) {
       case 0:
       default:
       return kgMakeDefaultKeybrd0((DIALOG *)Tmp,xo,yo,Vis);
       break;
       case 1:
       return kgMakeDefaultKeybrd1((DIALOG *)Tmp,xo,yo,Vis);
       break;
       case 2:
       return kgMakeDefaultKeybrd2((DIALOG *)Tmp,xo,yo,Vis);
       break;
       case 3:
       return kgMakeDefaultKeybrd3((DIALOG *)Tmp,xo,yo,Vis);
       break;
    }
}

int keybrd( void *parent,void **v,void *pt) {
  int offset=1;
  int ret=1,GrpId,k;
  KEYBRD *Kbrd;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  Kbrd = (KEYBRD *)malloc(sizeof(KEYBRD));
  D.Kbrd = Kbrd;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = keybrdGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 19;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 331;   /* Position of Dialog */ 
  D.yo = 274;
  D.xl = 796;    /*  Length of Dialog */
  D.yl = 258;    /*  Width  of Dialog */
  Kbrd->sgrp = kgOpenGrp(&D);
  kgAddtoGrp(&D,Kbrd->sgrp,d[0+offset].t);
  kgAddtoGrp(&D,Kbrd->sgrp,d[5+offset].t);
  kgAddtoGrp(&D,Kbrd->sgrp,d[6+offset].t);
  Kbrd->cgrp = kgOpenGrp(&D);
  kgAddtoGrp(&D,Kbrd->cgrp,d[16+offset].t);
  kgAddtoGrp(&D,Kbrd->cgrp,d[15+offset].t);
  kgAddtoGrp(&D,Kbrd->cgrp,d[17+offset].t);
  kgSetGrpVisibility(&D,Kbrd->cgrp,0);
  Kbrd->Coff = d[7+offset].t;
  Kbrd->Con = d[18+offset].t;
  Kbrd->Loff = d[19+offset].t;
  Kbrd->Lon = d[20+offset].t;
  kgSetWidgetVisibility(Kbrd->Loff,1);
  kgSetWidgetVisibility(Kbrd->Coff,1);
  kgSetWidgetVisibility(Kbrd->Lon,0);
  kgSetWidgetVisibility(Kbrd->Con,0);
  D.Initfun = keybrdinit;    /*   init fuction for Dialog */
  D.Cleanupfun = keybrdcleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 0;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = keybrdCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = keybrdResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = keybrdWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 1;    /*  1 for not showing in task bar*/
  D.NoWinMngr = 1;    /*  1 for no Window Manager*/
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
  ModifykeybrdGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *Runkeybrd(void *arg) {
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   keybrd(NULL,v,pt );
   return NULL;
}
