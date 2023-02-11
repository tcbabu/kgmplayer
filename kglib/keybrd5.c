#include "kulina.h"
#include "keybrd5Callbacks.h"
#define D_TH 1
static float  Rfac= 0.0 ;
static int RED=120,GREEN=125,BLUE=115;
static int Btype=1;

static char BUFF[100];
static char Sfac[10]="32";
static int Bfont=24,Bclr=0;
static int FillClr=101;
static float Btrans=0.0;
static unsigned char R=230,G=60,B=0;
static int ButClr=-1;
void Modifykeybrd5Gc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int keybrd5Group( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  char *xpm0;// Pixmap info
  xpm0 = NULL;
  DIP p0 = { 
    'p',
    2,3,  
    579,236,  
    (void *)xpm0,
    -1, /* bkgr colour */ 
      3,0,0.000000 /* border hide transparency*/ 
  };
  strcpy(p0.Wid,(char *)"Keybrdbkgr");
  p0.item = -1;
  char *xpm1;// Pixmap info
  xpm1 = NULL;
  DIP p1 = { 
    'p',
    38,217,  
    49,227,  
    (void *)xpm1,
    4, /* bkgr colour */ 
      3,0,0.000000 /* border hide transparency*/ 
  };
  strcpy(p1.Wid,(char *)"LightOff");
  p1.item = -1;
  char *xpm2;// Pixmap info
  xpm2 = NULL;
  DIP p2 = { 
    'p',
    38,217,  
    49,227,  
    (void *)xpm2,
    4, /* bkgr colour */ 
      3,0,0.000000 /* border hide transparency*/ 
  };
  strcpy(p2.Wid,(char *)"LightOn");
  p2.item = -1;
  char *xpm3;// Pixmap info
  xpm3 = NULL;
  DIP p3 = { 
    'p',
    51,211,  
    151,235,  
    (void *)xpm3,
    -1, /* bkgr colour */ 
      0,0,0.000000 /* border hide transparency*/ 
  };
  strcpy(p3.Wid,(char *)"CapsMsg");
  p3.item = -1;
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"BackSpace");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=-1;
  butn4[0].butncode='';
  DIN b4 = { 
    'n',
    510,10,  
    572,54,
    2,0,  
    54, 
    36, 
    1,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    keybrd5button1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b4.Wid,(char *)"BackSpace");
  b4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(char *)"Esc");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-1;
  butn5[0].butncode='';
  DIN b5 = { 
    'n',
    14,50,  
    76,94,
    2,0,  
    54, 
    36, 
    1,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    keybrd5button2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"Esc");
  b5.item = -1;
  BUT_STR  *butn6=NULL; 
  butn6= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn6[0].sw=1;
  strcpy(butn6[0].title,(char *)"Home");
  butn6[0].xpmn=NULL;
  butn6[0].xpmp=NULL;
  butn6[0].xpmh=NULL;
  butn6[0].bkgr=-1;
  butn6[0].butncode='';
  butn6[1].sw=1;
  strcpy(butn6[1].title,(char *)"End");
  butn6[1].xpmn=NULL;
  butn6[1].xpmp=NULL;
  butn6[1].xpmh=NULL;
  butn6[1].bkgr=-1;
  butn6[1].butncode='';
  DIN b6 = { 
    'n',
    14,90,  
    98,134,
    2,0,  
    36, 
    36, 
    2,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn6, 
    keybrd5button3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b6.Wid,(char *)"HomeEnd");
  b6.item = -1;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(char *)"Enter");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-1;
  butn7[0].butncode='';
  DIN b7 = { 
    'n',
    511,89,  
    571,132,
    2,0,  
    54, 
    36, 
    1,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    keybrd5button4callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"Enter");
  b7.item = -1;
  BUT_STR  *butn8=NULL; 
  butn8= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn8[0].sw=1;
  strcpy(butn8[0].title,(char *)"CapsLock");
  butn8[0].xpmn=NULL;
  butn8[0].xpmp=NULL;
  butn8[0].xpmh=NULL;
  butn8[0].bkgr=-1;
  butn8[0].butncode='';
  DIN b8 = { 
    'n',
    14,130,  
    76,174,
    2,0,  
    54, 
    36, 
    1,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn8, 
    keybrd5button5callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b8.Wid,(char *)"CapsLock");
  b8.item = -1;
  BUT_STR  *butn9=NULL; 
  butn9= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn9[0].sw=1;
  strcpy(butn9[0].title,(char *)"Tab");
  butn9[0].xpmn=NULL;
  butn9[0].xpmp=NULL;
  butn9[0].xpmh=NULL;
  butn9[0].bkgr=-1;
  butn9[0].butncode='';
  DIN b9 = { 
    'n',
    72,130,  
    116,174,
    2,0,  
    36, 
    36, 
    1,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn9, 
    keybrd5button6callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b9.Wid,(char *)"Tab");
  b9.item = -1;
  BUT_STR  *butn10=NULL; 
  butn10= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn10[0].sw=1;
  strcpy(butn10[0].title,(char *)"Left");
  butn10[0].xpmn=NULL;
  butn10[0].xpmp=NULL;
  butn10[0].xpmh=NULL;
  butn10[0].bkgr=-1;
  butn10[0].butncode='';
  butn10[1].sw=1;
  strcpy(butn10[1].title,(char *)"Right");
  butn10[1].xpmn=NULL;
  butn10[1].xpmp=NULL;
  butn10[1].xpmh=NULL;
  butn10[1].bkgr=-1;
  butn10[1].butncode='';
  DIN b10 = { 
    'n',
    493,130,  
    572,174,
    2,0,  
    36, 
    36, 
    2,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn10, 
    keybrd5button7callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b10.Wid,(char *)"LeftRight");
  b10.item = -1;
  BUT_STR  *butn11=NULL; 
  butn11= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn11[0].sw=1;
  strcpy(butn11[0].title,(char *)"");
  butn11[0].xpmn=NULL;
  butn11[0].xpmp=NULL;
  butn11[0].xpmh=NULL;
  butn11[0].bkgr=-1;
  butn11[0].butncode='';
  DIN b11 = { 
    'n',
    241,170,  
    353,211,
    2,0,  
    108, 
    36, 
    1,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn11, 
    keybrd5button8callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b11.Wid,(char *)"SpaceBar");
  b11.item = -1;
  BUT_STR  *butn12=NULL; 
  butn12= (BUT_STR *)malloc(sizeof(BUT_STR)*10);
  butn12[0].sw=1;
  strcpy(butn12[0].title,(char *)"");
  butn12[0].xpmn=NULL;
  butn12[0].xpmp=NULL;
  butn12[0].xpmh=NULL;
  butn12[0].bkgr=-1;
  butn12[0].butncode='';
  butn12[1].sw=1;
  strcpy(butn12[1].title,(char *)"");
  butn12[1].xpmn=NULL;
  butn12[1].xpmp=NULL;
  butn12[1].xpmh=NULL;
  butn12[1].bkgr=-1;
  butn12[1].butncode='';
  butn12[2].sw=1;
  strcpy(butn12[2].title,(char *)"");
  butn12[2].xpmn=NULL;
  butn12[2].xpmp=NULL;
  butn12[2].xpmh=NULL;
  butn12[2].bkgr=-1;
  butn12[2].butncode='';
  butn12[3].sw=1;
  strcpy(butn12[3].title,(char *)"");
  butn12[3].xpmn=NULL;
  butn12[3].xpmp=NULL;
  butn12[3].xpmh=NULL;
  butn12[3].bkgr=-1;
  butn12[3].butncode='';
  butn12[4].sw=1;
  strcpy(butn12[4].title,(char *)"");
  butn12[4].xpmn=NULL;
  butn12[4].xpmp=NULL;
  butn12[4].xpmh=NULL;
  butn12[4].bkgr=-1;
  butn12[4].butncode='';
  butn12[5].sw=1;
  strcpy(butn12[5].title,(char *)"");
  butn12[5].xpmn=NULL;
  butn12[5].xpmp=NULL;
  butn12[5].xpmh=NULL;
  butn12[5].bkgr=-1;
  butn12[5].butncode='';
  butn12[6].sw=1;
  strcpy(butn12[6].title,(char *)"");
  butn12[6].xpmn=NULL;
  butn12[6].xpmp=NULL;
  butn12[6].xpmh=NULL;
  butn12[6].bkgr=-1;
  butn12[6].butncode='';
  butn12[7].sw=1;
  strcpy(butn12[7].title,(char *)"");
  butn12[7].xpmn=NULL;
  butn12[7].xpmp=NULL;
  butn12[7].xpmh=NULL;
  butn12[7].bkgr=-1;
  butn12[7].butncode='';
  butn12[8].sw=1;
  strcpy(butn12[8].title,(char *)"");
  butn12[8].xpmn=NULL;
  butn12[8].xpmp=NULL;
  butn12[8].xpmh=NULL;
  butn12[8].bkgr=-1;
  butn12[8].butncode='';
  butn12[9].sw=1;
  strcpy(butn12[9].title,(char *)"");
  butn12[9].xpmn=NULL;
  butn12[9].xpmp=NULL;
  butn12[9].xpmh=NULL;
  butn12[9].bkgr=-1;
  butn12[9].butncode='';
  DIN b12 = { 
    'n',
    72,50,  
    456,94,
    2,0,  
    36, 
    36, 
    10,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn12, 
    keybrd5button9callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b12.Wid,(char *)"qwert");
  b12.item = -1;
  BUT_STR  *butn13=NULL; 
  butn13= (BUT_STR *)malloc(sizeof(BUT_STR)*9);
  butn13[0].sw=1;
  strcpy(butn13[0].title,(char *)"");
  butn13[0].xpmn=NULL;
  butn13[0].xpmp=NULL;
  butn13[0].xpmh=NULL;
  butn13[0].bkgr=-1;
  butn13[0].butncode='';
  butn13[1].sw=1;
  strcpy(butn13[1].title,(char *)"");
  butn13[1].xpmn=NULL;
  butn13[1].xpmp=NULL;
  butn13[1].xpmh=NULL;
  butn13[1].bkgr=-1;
  butn13[1].butncode='';
  butn13[2].sw=1;
  strcpy(butn13[2].title,(char *)"");
  butn13[2].xpmn=NULL;
  butn13[2].xpmp=NULL;
  butn13[2].xpmh=NULL;
  butn13[2].bkgr=-1;
  butn13[2].butncode='';
  butn13[3].sw=1;
  strcpy(butn13[3].title,(char *)"");
  butn13[3].xpmn=NULL;
  butn13[3].xpmp=NULL;
  butn13[3].xpmh=NULL;
  butn13[3].bkgr=-1;
  butn13[3].butncode='';
  butn13[4].sw=1;
  strcpy(butn13[4].title,(char *)"");
  butn13[4].xpmn=NULL;
  butn13[4].xpmp=NULL;
  butn13[4].xpmh=NULL;
  butn13[4].bkgr=-1;
  butn13[4].butncode='';
  butn13[5].sw=1;
  strcpy(butn13[5].title,(char *)"");
  butn13[5].xpmn=NULL;
  butn13[5].xpmp=NULL;
  butn13[5].xpmh=NULL;
  butn13[5].bkgr=-1;
  butn13[5].butncode='';
  butn13[6].sw=1;
  strcpy(butn13[6].title,(char *)"");
  butn13[6].xpmn=NULL;
  butn13[6].xpmp=NULL;
  butn13[6].xpmh=NULL;
  butn13[6].bkgr=-1;
  butn13[6].butncode='';
  butn13[7].sw=1;
  strcpy(butn13[7].title,(char *)"");
  butn13[7].xpmn=NULL;
  butn13[7].xpmp=NULL;
  butn13[7].xpmh=NULL;
  butn13[7].bkgr=-1;
  butn13[7].butncode='';
  butn13[8].sw=1;
  strcpy(butn13[8].title,(char *)"");
  butn13[8].xpmn=NULL;
  butn13[8].xpmp=NULL;
  butn13[8].xpmh=NULL;
  butn13[8].bkgr=-1;
  butn13[8].butncode='';
  DIN b13 = { 
    'n',
    91,90,  
    437,134,
    2,0,  
    36, 
    36, 
    9,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn13, 
    keybrd5button10callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b13.Wid,(char *)"asdf");
  b13.item = -1;
  BUT_STR  *butn14=NULL; 
  butn14= (BUT_STR *)malloc(sizeof(BUT_STR)*7);
  butn14[0].sw=1;
  strcpy(butn14[0].title,(char *)"");
  butn14[0].xpmn=NULL;
  butn14[0].xpmp=NULL;
  butn14[0].xpmh=NULL;
  butn14[0].bkgr=-1;
  butn14[0].butncode='';
  butn14[1].sw=1;
  strcpy(butn14[1].title,(char *)"");
  butn14[1].xpmn=NULL;
  butn14[1].xpmp=NULL;
  butn14[1].xpmh=NULL;
  butn14[1].bkgr=-1;
  butn14[1].butncode='';
  butn14[2].sw=1;
  strcpy(butn14[2].title,(char *)"");
  butn14[2].xpmn=NULL;
  butn14[2].xpmp=NULL;
  butn14[2].xpmh=NULL;
  butn14[2].bkgr=-1;
  butn14[2].butncode='';
  butn14[3].sw=1;
  strcpy(butn14[3].title,(char *)"");
  butn14[3].xpmn=NULL;
  butn14[3].xpmp=NULL;
  butn14[3].xpmh=NULL;
  butn14[3].bkgr=-1;
  butn14[3].butncode='';
  butn14[4].sw=1;
  strcpy(butn14[4].title,(char *)"");
  butn14[4].xpmn=NULL;
  butn14[4].xpmp=NULL;
  butn14[4].xpmh=NULL;
  butn14[4].bkgr=-1;
  butn14[4].butncode='';
  butn14[5].sw=1;
  strcpy(butn14[5].title,(char *)"");
  butn14[5].xpmn=NULL;
  butn14[5].xpmp=NULL;
  butn14[5].xpmh=NULL;
  butn14[5].bkgr=-1;
  butn14[5].butncode='';
  butn14[6].sw=1;
  strcpy(butn14[6].title,(char *)"");
  butn14[6].xpmn=NULL;
  butn14[6].xpmp=NULL;
  butn14[6].xpmh=NULL;
  butn14[6].bkgr=-1;
  butn14[6].butncode='';
  DIN b14 = { 
    'n',
    110,130,  
    380,174,
    2,0,  
    36, 
    36, 
    7,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn14, 
    keybrd5button11callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b14.Wid,(char *)"zxcv");
  b14.item = -1;
  BUT_STR  *butn15=NULL; 
  butn15= (BUT_STR *)malloc(sizeof(BUT_STR)*10);
  butn15[0].sw=1;
  strcpy(butn15[0].title,(char *)"");
  butn15[0].xpmn=NULL;
  butn15[0].xpmp=NULL;
  butn15[0].xpmh=NULL;
  butn15[0].bkgr=-1;
  butn15[0].butncode='';
  butn15[1].sw=1;
  strcpy(butn15[1].title,(char *)"");
  butn15[1].xpmn=NULL;
  butn15[1].xpmp=NULL;
  butn15[1].xpmh=NULL;
  butn15[1].bkgr=-1;
  butn15[1].butncode='';
  butn15[2].sw=1;
  strcpy(butn15[2].title,(char *)"");
  butn15[2].xpmn=NULL;
  butn15[2].xpmp=NULL;
  butn15[2].xpmh=NULL;
  butn15[2].bkgr=-1;
  butn15[2].butncode='';
  butn15[3].sw=1;
  strcpy(butn15[3].title,(char *)"");
  butn15[3].xpmn=NULL;
  butn15[3].xpmp=NULL;
  butn15[3].xpmh=NULL;
  butn15[3].bkgr=-1;
  butn15[3].butncode='';
  butn15[4].sw=1;
  strcpy(butn15[4].title,(char *)"");
  butn15[4].xpmn=NULL;
  butn15[4].xpmp=NULL;
  butn15[4].xpmh=NULL;
  butn15[4].bkgr=-1;
  butn15[4].butncode='';
  butn15[5].sw=1;
  strcpy(butn15[5].title,(char *)"");
  butn15[5].xpmn=NULL;
  butn15[5].xpmp=NULL;
  butn15[5].xpmh=NULL;
  butn15[5].bkgr=-1;
  butn15[5].butncode='';
  butn15[6].sw=1;
  strcpy(butn15[6].title,(char *)"");
  butn15[6].xpmn=NULL;
  butn15[6].xpmp=NULL;
  butn15[6].xpmh=NULL;
  butn15[6].bkgr=-1;
  butn15[6].butncode='';
  butn15[7].sw=1;
  strcpy(butn15[7].title,(char *)"");
  butn15[7].xpmn=NULL;
  butn15[7].xpmp=NULL;
  butn15[7].xpmh=NULL;
  butn15[7].bkgr=-1;
  butn15[7].butncode='';
  butn15[8].sw=1;
  strcpy(butn15[8].title,(char *)"");
  butn15[8].xpmn=NULL;
  butn15[8].xpmp=NULL;
  butn15[8].xpmh=NULL;
  butn15[8].bkgr=-1;
  butn15[8].butncode='';
  butn15[9].sw=1;
  strcpy(butn15[9].title,(char *)"");
  butn15[9].xpmn=NULL;
  butn15[9].xpmp=NULL;
  butn15[9].xpmh=NULL;
  butn15[9].bkgr=-1;
  butn15[9].butncode='';
  DIN b15 = { 
    'n',
    72,50,  
    456,94,
    2,0,  
    36, 
    36, 
    10,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn15, 
    keybrd5button12callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b15.Wid,(char *)"QWERT");
  b15.item = -1;
  BUT_STR  *butn16=NULL; 
  butn16= (BUT_STR *)malloc(sizeof(BUT_STR)*9);
  butn16[0].sw=1;
  strcpy(butn16[0].title,(char *)"");
  butn16[0].xpmn=NULL;
  butn16[0].xpmp=NULL;
  butn16[0].xpmh=NULL;
  butn16[0].bkgr=-1;
  butn16[0].butncode='';
  butn16[1].sw=1;
  strcpy(butn16[1].title,(char *)"");
  butn16[1].xpmn=NULL;
  butn16[1].xpmp=NULL;
  butn16[1].xpmh=NULL;
  butn16[1].bkgr=-1;
  butn16[1].butncode='';
  butn16[2].sw=1;
  strcpy(butn16[2].title,(char *)"");
  butn16[2].xpmn=NULL;
  butn16[2].xpmp=NULL;
  butn16[2].xpmh=NULL;
  butn16[2].bkgr=-1;
  butn16[2].butncode='';
  butn16[3].sw=1;
  strcpy(butn16[3].title,(char *)"");
  butn16[3].xpmn=NULL;
  butn16[3].xpmp=NULL;
  butn16[3].xpmh=NULL;
  butn16[3].bkgr=-1;
  butn16[3].butncode='';
  butn16[4].sw=1;
  strcpy(butn16[4].title,(char *)"");
  butn16[4].xpmn=NULL;
  butn16[4].xpmp=NULL;
  butn16[4].xpmh=NULL;
  butn16[4].bkgr=-1;
  butn16[4].butncode='';
  butn16[5].sw=1;
  strcpy(butn16[5].title,(char *)"");
  butn16[5].xpmn=NULL;
  butn16[5].xpmp=NULL;
  butn16[5].xpmh=NULL;
  butn16[5].bkgr=-1;
  butn16[5].butncode='';
  butn16[6].sw=1;
  strcpy(butn16[6].title,(char *)"");
  butn16[6].xpmn=NULL;
  butn16[6].xpmp=NULL;
  butn16[6].xpmh=NULL;
  butn16[6].bkgr=-1;
  butn16[6].butncode='';
  butn16[7].sw=1;
  strcpy(butn16[7].title,(char *)"");
  butn16[7].xpmn=NULL;
  butn16[7].xpmp=NULL;
  butn16[7].xpmh=NULL;
  butn16[7].bkgr=-1;
  butn16[7].butncode='';
  butn16[8].sw=1;
  strcpy(butn16[8].title,(char *)"");
  butn16[8].xpmn=NULL;
  butn16[8].xpmp=NULL;
  butn16[8].xpmh=NULL;
  butn16[8].bkgr=-1;
  butn16[8].butncode='';
  DIN b16 = { 
    'n',
    91,90,  
    437,134,
    2,0,  
    36, 
    36, 
    9,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn16, 
    keybrd5button13callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b16.Wid,(char *)"ASDF");
  b16.item = -1;
  BUT_STR  *butn17=NULL; 
  butn17= (BUT_STR *)malloc(sizeof(BUT_STR)*7);
  butn17[0].sw=1;
  strcpy(butn17[0].title,(char *)"");
  butn17[0].xpmn=NULL;
  butn17[0].xpmp=NULL;
  butn17[0].xpmh=NULL;
  butn17[0].bkgr=-1;
  butn17[0].butncode='';
  butn17[1].sw=1;
  strcpy(butn17[1].title,(char *)"");
  butn17[1].xpmn=NULL;
  butn17[1].xpmp=NULL;
  butn17[1].xpmh=NULL;
  butn17[1].bkgr=-1;
  butn17[1].butncode='';
  butn17[2].sw=1;
  strcpy(butn17[2].title,(char *)"");
  butn17[2].xpmn=NULL;
  butn17[2].xpmp=NULL;
  butn17[2].xpmh=NULL;
  butn17[2].bkgr=-1;
  butn17[2].butncode='';
  butn17[3].sw=1;
  strcpy(butn17[3].title,(char *)"");
  butn17[3].xpmn=NULL;
  butn17[3].xpmp=NULL;
  butn17[3].xpmh=NULL;
  butn17[3].bkgr=-1;
  butn17[3].butncode='';
  butn17[4].sw=1;
  strcpy(butn17[4].title,(char *)"");
  butn17[4].xpmn=NULL;
  butn17[4].xpmp=NULL;
  butn17[4].xpmh=NULL;
  butn17[4].bkgr=-1;
  butn17[4].butncode='';
  butn17[5].sw=1;
  strcpy(butn17[5].title,(char *)"");
  butn17[5].xpmn=NULL;
  butn17[5].xpmp=NULL;
  butn17[5].xpmh=NULL;
  butn17[5].bkgr=-1;
  butn17[5].butncode='';
  butn17[6].sw=1;
  strcpy(butn17[6].title,(char *)"");
  butn17[6].xpmn=NULL;
  butn17[6].xpmp=NULL;
  butn17[6].xpmh=NULL;
  butn17[6].bkgr=-1;
  butn17[6].butncode='';
  DIN b17 = { 
    'n',
    110,130,  
    380,174,
    2,0,  
    36, 
    36, 
    7,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn17, 
    keybrd5button14callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b17.Wid,(char *)"ZXCV");
  b17.item = -1;
  BUT_STR  *butn18=NULL; 
  butn18= (BUT_STR *)malloc(sizeof(BUT_STR)*13);
  butn18[0].sw=1;
  strcpy(butn18[0].title,(char *)"");
  butn18[0].xpmn=NULL;
  butn18[0].xpmp=NULL;
  butn18[0].xpmh=NULL;
  butn18[0].bkgr=-1;
  butn18[0].butncode='';
  butn18[1].sw=1;
  strcpy(butn18[1].title,(char *)"");
  butn18[1].xpmn=NULL;
  butn18[1].xpmp=NULL;
  butn18[1].xpmh=NULL;
  butn18[1].bkgr=-1;
  butn18[1].butncode='';
  butn18[2].sw=1;
  strcpy(butn18[2].title,(char *)"");
  butn18[2].xpmn=NULL;
  butn18[2].xpmp=NULL;
  butn18[2].xpmh=NULL;
  butn18[2].bkgr=-1;
  butn18[2].butncode='';
  butn18[3].sw=1;
  strcpy(butn18[3].title,(char *)"");
  butn18[3].xpmn=NULL;
  butn18[3].xpmp=NULL;
  butn18[3].xpmh=NULL;
  butn18[3].bkgr=-1;
  butn18[3].butncode='';
  butn18[4].sw=1;
  strcpy(butn18[4].title,(char *)"");
  butn18[4].xpmn=NULL;
  butn18[4].xpmp=NULL;
  butn18[4].xpmh=NULL;
  butn18[4].bkgr=-1;
  butn18[4].butncode='';
  butn18[5].sw=1;
  strcpy(butn18[5].title,(char *)"");
  butn18[5].xpmn=NULL;
  butn18[5].xpmp=NULL;
  butn18[5].xpmh=NULL;
  butn18[5].bkgr=-1;
  butn18[5].butncode='';
  butn18[6].sw=1;
  strcpy(butn18[6].title,(char *)"");
  butn18[6].xpmn=NULL;
  butn18[6].xpmp=NULL;
  butn18[6].xpmh=NULL;
  butn18[6].bkgr=-1;
  butn18[6].butncode='';
  butn18[7].sw=1;
  strcpy(butn18[7].title,(char *)"");
  butn18[7].xpmn=NULL;
  butn18[7].xpmp=NULL;
  butn18[7].xpmh=NULL;
  butn18[7].bkgr=-1;
  butn18[7].butncode='';
  butn18[8].sw=1;
  strcpy(butn18[8].title,(char *)"");
  butn18[8].xpmn=NULL;
  butn18[8].xpmp=NULL;
  butn18[8].xpmh=NULL;
  butn18[8].bkgr=-1;
  butn18[8].butncode='';
  butn18[9].sw=1;
  strcpy(butn18[9].title,(char *)"");
  butn18[9].xpmn=NULL;
  butn18[9].xpmp=NULL;
  butn18[9].xpmh=NULL;
  butn18[9].bkgr=-1;
  butn18[9].butncode='';
  butn18[10].sw=1;
  strcpy(butn18[10].title,(char *)"");
  butn18[10].xpmn=NULL;
  butn18[10].xpmp=NULL;
  butn18[10].xpmh=NULL;
  butn18[10].bkgr=-1;
  butn18[10].butncode='';
  butn18[11].sw=1;
  strcpy(butn18[11].title,(char *)"");
  butn18[11].xpmn=NULL;
  butn18[11].xpmp=NULL;
  butn18[11].xpmh=NULL;
  butn18[11].bkgr=-1;
  butn18[11].butncode='';
  butn18[12].sw=1;
  strcpy(butn18[12].title,(char *)"");
  butn18[12].xpmn=NULL;
  butn18[12].xpmp=NULL;
  butn18[12].xpmh=NULL;
  butn18[12].bkgr=-1;
  butn18[12].butncode='';
  DIN b18 = { 
    'n',
    13,10,  
    515,54,
    2,0,  
    36, 
    36, 
    13,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn18, 
    keybrd5button15callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b18.Wid,(char *)"numbers");
  b18.item = -1;
  BUT_STR  *butn19=NULL; 
  butn19= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn19[0].sw=1;
  strcpy(butn19[0].title,(char *)"[");
  butn19[0].xpmn=NULL;
  butn19[0].xpmp=NULL;
  butn19[0].xpmh=NULL;
  butn19[0].bkgr=-1;
  butn19[0].butncode='';
  butn19[1].sw=1;
  strcpy(butn19[1].title,(char *)"");
  butn19[1].xpmn=NULL;
  butn19[1].xpmp=NULL;
  butn19[1].xpmh=NULL;
  butn19[1].bkgr=-1;
  butn19[1].butncode='';
  butn19[2].sw=1;
  strcpy(butn19[2].title,(char *)"");
  butn19[2].xpmn=NULL;
  butn19[2].xpmp=NULL;
  butn19[2].xpmh=NULL;
  butn19[2].bkgr=-1;
  butn19[2].butncode='';
  DIN b19 = { 
    'n',
    451,50,  
    573,94,
    2,0,  
    36, 
    36, 
    3,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn19, 
    keybrd5button16callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b19.Wid,(char *)"sym1");
  b19.item = -1;
  BUT_STR  *butn20=NULL; 
  butn20= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn20[0].sw=1;
  strcpy(butn20[0].title,(char *)";");
  butn20[0].xpmn=NULL;
  butn20[0].xpmp=NULL;
  butn20[0].xpmh=NULL;
  butn20[0].bkgr=-1;
  butn20[0].butncode='';
  butn20[1].sw=1;
  strcpy(butn20[1].title,(char *)";");
  butn20[1].xpmn=NULL;
  butn20[1].xpmp=NULL;
  butn20[1].xpmh=NULL;
  butn20[1].bkgr=-1;
  butn20[1].butncode='';
  DIN b20 = { 
    'n',
    434,89,  
    515,133,
    2,0,  
    36, 
    36, 
    2,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn20, 
    keybrd5button17callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b20.Wid,(char *)"sym2");
  b20.item = -1;
  BUT_STR  *butn21=NULL; 
  butn21= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn21[0].sw=1;
  strcpy(butn21[0].title,(char *)",");
  butn21[0].xpmn=NULL;
  butn21[0].xpmp=NULL;
  butn21[0].xpmh=NULL;
  butn21[0].bkgr=-1;
  butn21[0].butncode='';
  butn21[1].sw=1;
  strcpy(butn21[1].title,(char *)"");
  butn21[1].xpmn=NULL;
  butn21[1].xpmp=NULL;
  butn21[1].xpmh=NULL;
  butn21[1].bkgr=-1;
  butn21[1].butncode='';
  butn21[2].sw=1;
  strcpy(butn21[2].title,(char *)"");
  butn21[2].xpmn=NULL;
  butn21[2].xpmp=NULL;
  butn21[2].xpmh=NULL;
  butn21[2].bkgr=-1;
  butn21[2].butncode='';
  DIN b21 = { 
    'n',
    376,130,  
    493,174,
    2,0,  
    36, 
    36, 
    3,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn21, 
    keybrd5button18callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b21.Wid,(char *)"sym3");
  b21.item = -1;
  BUT_STR  *butn22=NULL; 
  butn22= (BUT_STR *)malloc(sizeof(BUT_STR)*13);
  butn22[0].sw=1;
  strcpy(butn22[0].title,(char *)"");
  butn22[0].xpmn=NULL;
  butn22[0].xpmp=NULL;
  butn22[0].xpmh=NULL;
  butn22[0].bkgr=-1;
  butn22[0].butncode='';
  butn22[1].sw=1;
  strcpy(butn22[1].title,(char *)"");
  butn22[1].xpmn=NULL;
  butn22[1].xpmp=NULL;
  butn22[1].xpmh=NULL;
  butn22[1].bkgr=-1;
  butn22[1].butncode='';
  butn22[2].sw=1;
  strcpy(butn22[2].title,(char *)"");
  butn22[2].xpmn=NULL;
  butn22[2].xpmp=NULL;
  butn22[2].xpmh=NULL;
  butn22[2].bkgr=-1;
  butn22[2].butncode='';
  butn22[3].sw=1;
  strcpy(butn22[3].title,(char *)"");
  butn22[3].xpmn=NULL;
  butn22[3].xpmp=NULL;
  butn22[3].xpmh=NULL;
  butn22[3].bkgr=-1;
  butn22[3].butncode='';
  butn22[4].sw=1;
  strcpy(butn22[4].title,(char *)"");
  butn22[4].xpmn=NULL;
  butn22[4].xpmp=NULL;
  butn22[4].xpmh=NULL;
  butn22[4].bkgr=-1;
  butn22[4].butncode='';
  butn22[5].sw=1;
  strcpy(butn22[5].title,(char *)"");
  butn22[5].xpmn=NULL;
  butn22[5].xpmp=NULL;
  butn22[5].xpmh=NULL;
  butn22[5].bkgr=-1;
  butn22[5].butncode='';
  butn22[6].sw=1;
  strcpy(butn22[6].title,(char *)"");
  butn22[6].xpmn=NULL;
  butn22[6].xpmp=NULL;
  butn22[6].xpmh=NULL;
  butn22[6].bkgr=-1;
  butn22[6].butncode='';
  butn22[7].sw=1;
  strcpy(butn22[7].title,(char *)"");
  butn22[7].xpmn=NULL;
  butn22[7].xpmp=NULL;
  butn22[7].xpmh=NULL;
  butn22[7].bkgr=-1;
  butn22[7].butncode='';
  butn22[8].sw=1;
  strcpy(butn22[8].title,(char *)"");
  butn22[8].xpmn=NULL;
  butn22[8].xpmp=NULL;
  butn22[8].xpmh=NULL;
  butn22[8].bkgr=-1;
  butn22[8].butncode='';
  butn22[9].sw=1;
  strcpy(butn22[9].title,(char *)"");
  butn22[9].xpmn=NULL;
  butn22[9].xpmp=NULL;
  butn22[9].xpmh=NULL;
  butn22[9].bkgr=-1;
  butn22[9].butncode='';
  butn22[10].sw=1;
  strcpy(butn22[10].title,(char *)"");
  butn22[10].xpmn=NULL;
  butn22[10].xpmp=NULL;
  butn22[10].xpmh=NULL;
  butn22[10].bkgr=-1;
  butn22[10].butncode='';
  butn22[11].sw=1;
  strcpy(butn22[11].title,(char *)"");
  butn22[11].xpmn=NULL;
  butn22[11].xpmp=NULL;
  butn22[11].xpmh=NULL;
  butn22[11].bkgr=-1;
  butn22[11].butncode='';
  butn22[12].sw=1;
  strcpy(butn22[12].title,(char *)"");
  butn22[12].xpmn=NULL;
  butn22[12].xpmp=NULL;
  butn22[12].xpmh=NULL;
  butn22[12].bkgr=-1;
  butn22[12].butncode='';
  DIN b22 = { 
    'n',
    13,10,  
    515,54,
    2,0,  
    36, 
    36, 
    13,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn22, 
    keybrd5button19callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b22.Wid,(char *)"NUMBERS");
  b22.item = -1;
  BUT_STR  *butn23=NULL; 
  butn23= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn23[0].sw=1;
  strcpy(butn23[0].title,(char *)"[");
  butn23[0].xpmn=NULL;
  butn23[0].xpmp=NULL;
  butn23[0].xpmh=NULL;
  butn23[0].bkgr=-1;
  butn23[0].butncode='';
  butn23[1].sw=1;
  strcpy(butn23[1].title,(char *)"");
  butn23[1].xpmn=NULL;
  butn23[1].xpmp=NULL;
  butn23[1].xpmh=NULL;
  butn23[1].bkgr=-1;
  butn23[1].butncode='';
  butn23[2].sw=1;
  strcpy(butn23[2].title,(char *)"");
  butn23[2].xpmn=NULL;
  butn23[2].xpmp=NULL;
  butn23[2].xpmh=NULL;
  butn23[2].bkgr=-1;
  butn23[2].butncode='';
  DIN b23 = { 
    'n',
    451,50,  
    573,94,
    2,0,  
    36, 
    36, 
    3,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn23, 
    keybrd5button20callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b23.Wid,(char *)"SYM1");
  b23.item = -1;
  BUT_STR  *butn24=NULL; 
  butn24= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn24[0].sw=1;
  strcpy(butn24[0].title,(char *)";");
  butn24[0].xpmn=NULL;
  butn24[0].xpmp=NULL;
  butn24[0].xpmh=NULL;
  butn24[0].bkgr=-1;
  butn24[0].butncode='';
  butn24[1].sw=1;
  strcpy(butn24[1].title,(char *)";");
  butn24[1].xpmn=NULL;
  butn24[1].xpmp=NULL;
  butn24[1].xpmh=NULL;
  butn24[1].bkgr=-1;
  butn24[1].butncode='';
  DIN b24 = { 
    'n',
    435,90,  
    517,132,
    2,0,  
    36, 
    36, 
    2,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn24, 
    keybrd5button21callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b24.Wid,(char *)"SYM2");
  b24.item = -1;
  BUT_STR  *butn25=NULL; 
  butn25= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn25[0].sw=1;
  strcpy(butn25[0].title,(char *)",");
  butn25[0].xpmn=NULL;
  butn25[0].xpmp=NULL;
  butn25[0].xpmh=NULL;
  butn25[0].bkgr=-1;
  butn25[0].butncode='';
  butn25[1].sw=1;
  strcpy(butn25[1].title,(char *)"");
  butn25[1].xpmn=NULL;
  butn25[1].xpmp=NULL;
  butn25[1].xpmh=NULL;
  butn25[1].bkgr=-1;
  butn25[1].butncode='';
  butn25[2].sw=1;
  strcpy(butn25[2].title,(char *)"");
  butn25[2].xpmn=NULL;
  butn25[2].xpmp=NULL;
  butn25[2].xpmh=NULL;
  butn25[2].bkgr=-1;
  butn25[2].butncode='';
  DIN b25 = { 
    'n',
    376,130,  
    493,174,
    2,0,  
    36, 
    36, 
    3,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn25, 
    keybrd5button22callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b25.Wid,(char *)"SYM3");
  b25.item = -1;
  BUT_STR  *butn26=NULL; 
  butn26= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn26[0].sw=1;
  strcpy(butn26[0].title,(char *)"Shift");
  butn26[0].xpmn=NULL;
  butn26[0].xpmp=NULL;
  butn26[0].xpmh=NULL;
  butn26[0].bkgr=-1;
  butn26[0].butncode='';
  butn26[1].sw=1;
  strcpy(butn26[1].title,(char *)"Cntl-C");
  butn26[1].xpmn=NULL;
  butn26[1].xpmp=NULL;
  butn26[1].xpmh=NULL;
  butn26[1].bkgr=-1;
  butn26[1].butncode='';
  butn26[2].sw=1;
  strcpy(butn26[2].title,(char *)"Cntl-D");
  butn26[2].xpmn=NULL;
  butn26[2].xpmp=NULL;
  butn26[2].xpmh=NULL;
  butn26[2].bkgr=-1;
  butn26[2].butncode='';
  butn26[3].sw=1;
  strcpy(butn26[3].title,(char *)"Cntl-S");
  butn26[3].xpmn=NULL;
  butn26[3].xpmp=NULL;
  butn26[3].xpmh=NULL;
  butn26[3].bkgr=-1;
  butn26[3].butncode='';
  DIN b26 = { 
    'n',
    14,169,  
    167,211,
    2,2,  
    36, 
    36, 
    4,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn26, 
    keybrd5button23callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b26.Wid,(char *)"keybrd5Widget27");
  b26.item = -1;
  BUT_STR  *butn27=NULL; 
  butn27= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn27[0].sw=1;
  strcpy(butn27[0].title,(char *)"Shift");
  butn27[0].xpmn=NULL;
  butn27[0].xpmp=NULL;
  butn27[0].xpmh=NULL;
  butn27[0].bkgr=-1;
  butn27[0].butncode='';
  DIN b27 = { 
    'n',
    514,170,  
    572,211,
    2,2,  
    54, 
    36, 
    1,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn27, 
    keybrd5button24callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b27.Wid,(char *)"keybrd5Widget28");
  b27.item = -1;
  BUT_STR  *butn28=NULL; 
  butn28= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn28[0].sw=1;
  strcpy(butn28[0].title,(char *)"Del");
  butn28[0].xpmn=NULL;
  butn28[0].xpmp=NULL;
  butn28[0].xpmh=NULL;
  butn28[0].bkgr=-1;
  butn28[0].butncode='';
  butn28[1].sw=1;
  strcpy(butn28[1].title,(char *)"Insert");
  butn28[1].xpmn=NULL;
  butn28[1].xpmp=NULL;
  butn28[1].xpmh=NULL;
  butn28[1].bkgr=-1;
  butn28[1].butncode='';
  DIN b28 = { 
    'n',
    165,169,  
    243,211,
    2,2,  
    36, 
    36, 
    2,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn28, 
    keybrd5button25callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b28.Wid,(char *)"DelInsert");
  b28.item = -1;
  BUT_STR  *butn29=NULL; 
  butn29= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn29[0].sw=1;
  strcpy(butn29[0].title,(char *)"PgUp");
  butn29[0].xpmn=NULL;
  butn29[0].xpmp=NULL;
  butn29[0].xpmh=NULL;
  butn29[0].bkgr=-1;
  butn29[0].butncode='';
  butn29[1].sw=1;
  strcpy(butn29[1].title,(char *)"PgDn");
  butn29[1].xpmn=NULL;
  butn29[1].xpmp=NULL;
  butn29[1].xpmh=NULL;
  butn29[1].bkgr=-1;
  butn29[1].butncode='';
  butn29[2].sw=1;
  strcpy(butn29[2].title,(char *)"Up");
  butn29[2].xpmn=NULL;
  butn29[2].xpmp=NULL;
  butn29[2].xpmh=NULL;
  butn29[2].bkgr=-1;
  butn29[2].butncode='';
  butn29[3].sw=1;
  strcpy(butn29[3].title,(char *)"Down");
  butn29[3].xpmn=NULL;
  butn29[3].xpmp=NULL;
  butn29[3].xpmh=NULL;
  butn29[3].bkgr=-1;
  butn29[3].butncode='';
  DIN b29 = { 
    'n',
    358,169,  
    514,210,
    2,2,  
    36, 
    36, 
    4,1, 
    2,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn29, 
    keybrd5button26callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b29.Wid,(char *)"UpDown");
  b29.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+31));
  d =dtmp+i; 
  d[30].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIP));
  *d[0].p = p0;
  d[0].p->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIP));
  *d[1].p = p1;
  d[1].p->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIP));
  *d[2].p = p2;
  d[2].p->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIP));
  *d[3].p = p3;
  d[3].p->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIN));
  *d[4].N = b4;
  d[4].N->item = -1;
  keybrd5button1init(d[4].N,pt) ;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  *d[5].N = b5;
  d[5].N->item = -1;
  keybrd5button2init(d[5].N,pt) ;
  d[6].t = (DIT *)malloc(sizeof(DIN));
  *d[6].N = b6;
  d[6].N->item = -1;
  keybrd5button3init(d[6].N,pt) ;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  *d[7].N = b7;
  d[7].N->item = -1;
  keybrd5button4init(d[7].N,pt) ;
  d[8].t = (DIT *)malloc(sizeof(DIN));
  *d[8].N = b8;
  d[8].N->item = -1;
  keybrd5button5init(d[8].N,pt) ;
  d[9].t = (DIT *)malloc(sizeof(DIN));
  *d[9].N = b9;
  d[9].N->item = -1;
  keybrd5button6init(d[9].N,pt) ;
  d[10].t = (DIT *)malloc(sizeof(DIN));
  *d[10].N = b10;
  d[10].N->item = -1;
  keybrd5button7init(d[10].N,pt) ;
  d[11].t = (DIT *)malloc(sizeof(DIN));
  *d[11].N = b11;
  d[11].N->item = -1;
  keybrd5button8init(d[11].N,pt) ;
  d[12].t = (DIT *)malloc(sizeof(DIN));
  *d[12].N = b12;
  d[12].N->item = -1;
  keybrd5button9init(d[12].N,pt) ;
  d[13].t = (DIT *)malloc(sizeof(DIN));
  *d[13].N = b13;
  d[13].N->item = -1;
  keybrd5button10init(d[13].N,pt) ;
  d[14].t = (DIT *)malloc(sizeof(DIN));
  *d[14].N = b14;
  d[14].N->item = -1;
  keybrd5button11init(d[14].N,pt) ;
  d[15].t = (DIT *)malloc(sizeof(DIN));
  *d[15].N = b15;
  d[15].N->item = -1;
  keybrd5button12init(d[15].N,pt) ;
  d[16].t = (DIT *)malloc(sizeof(DIN));
  *d[16].N = b16;
  d[16].N->item = -1;
  keybrd5button13init(d[16].N,pt) ;
  d[17].t = (DIT *)malloc(sizeof(DIN));
  *d[17].N = b17;
  d[17].N->item = -1;
  keybrd5button14init(d[17].N,pt) ;
  d[18].t = (DIT *)malloc(sizeof(DIN));
  *d[18].N = b18;
  d[18].N->item = -1;
  keybrd5button15init(d[18].N,pt) ;
  d[19].t = (DIT *)malloc(sizeof(DIN));
  *d[19].N = b19;
  d[19].N->item = -1;
  keybrd5button16init(d[19].N,pt) ;
  d[20].t = (DIT *)malloc(sizeof(DIN));
  *d[20].N = b20;
  d[20].N->item = -1;
  keybrd5button17init(d[20].N,pt) ;
  d[21].t = (DIT *)malloc(sizeof(DIN));
  *d[21].N = b21;
  d[21].N->item = -1;
  keybrd5button18init(d[21].N,pt) ;
  d[22].t = (DIT *)malloc(sizeof(DIN));
  *d[22].N = b22;
  d[22].N->item = -1;
  keybrd5button19init(d[22].N,pt) ;
  d[23].t = (DIT *)malloc(sizeof(DIN));
  *d[23].N = b23;
  d[23].N->item = -1;
  keybrd5button20init(d[23].N,pt) ;
  d[24].t = (DIT *)malloc(sizeof(DIN));
  *d[24].N = b24;
  d[24].N->item = -1;
  keybrd5button21init(d[24].N,pt) ;
  d[25].t = (DIT *)malloc(sizeof(DIN));
  *d[25].N = b25;
  d[25].N->item = -1;
  keybrd5button22init(d[25].N,pt) ;
  d[26].t = (DIT *)malloc(sizeof(DIN));
  *d[26].N = b26;
  d[26].N->item = -1;
  keybrd5button23init(d[26].N,pt) ;
  d[27].t = (DIT *)malloc(sizeof(DIN));
  *d[27].N = b27;
  d[27].N->item = -1;
  keybrd5button24init(d[27].N,pt) ;
  d[28].t = (DIT *)malloc(sizeof(DIN));
  *d[28].N = b28;
  d[28].N->item = -1;
  keybrd5button25init(d[28].N,pt) ;
  d[29].t = (DIT *)malloc(sizeof(DIN));
  *d[29].N = b29;
  d[29].N->item = -1;
  keybrd5button26init(d[29].N,pt) ;
  d[30].t = NULL;
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
   while(d[i].t != NULL) {
     if(xm > (d[i].t->x1) ) xm = d[i].t->x1;
     if(ym > (d[i].t->y1) ) ym = d[i].t->y1;
     i++;
   }
//   kgShiftGrp(D,GrpId,xo-43,yo-28);
   kgShiftGrp(D,Kbrd->GrpId,xo-xm,yo-ym);
   Kbrd->grp1 = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->grp1,d[0+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[3+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[4+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[5+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[6+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[7+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[8+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[9+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[10+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[11+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[26+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[27+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[28+offset].t);
   kgAddtoGrp(D,Kbrd->grp1,d[29+offset].t);
   Kbrd->sgrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->sgrp,d[12+offset].t);
   kgAddtoGrp(D,Kbrd->sgrp,d[13+offset].t);
   kgAddtoGrp(D,Kbrd->sgrp,d[14+offset].t);
   Kbrd->cgrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->cgrp,d[15+offset].t);
   kgAddtoGrp(D,Kbrd->cgrp,d[16+offset].t);
   kgAddtoGrp(D,Kbrd->cgrp,d[17+offset].t);
   Kbrd->grp3 = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->grp3,d[18+offset].t);
   kgAddtoGrp(D,Kbrd->grp3,d[19+offset].t);
   kgAddtoGrp(D,Kbrd->grp3,d[20+offset].t);
   kgAddtoGrp(D,Kbrd->grp3,d[21+offset].t);
   Kbrd->grp2 = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->grp2,d[22+offset].t);
   kgAddtoGrp(D,Kbrd->grp2,d[23+offset].t);
   kgAddtoGrp(D,Kbrd->grp2,d[24+offset].t);
   kgAddtoGrp(D,Kbrd->grp2,d[25+offset].t);
   Kbrd->offgrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->offgrp,d[1+offset].t);
   Kbrd->ongrp = kgOpenGrp(D);
   kgAddtoGrp(D,Kbrd->ongrp,d[2+offset].t);

   Kbrd->CurWid = -1;
   Kbrd->Vis = Vis;
   Kbrd->D = D;
   Kbrd->ShiftPress=0;
   Kbrd->CapsLock=0;
   if(Vis==0) {
    kgSetGrpVisibility(D,Kbrd->GrpId,0);
   }
   else {
    kgSetGrpVisibility(D,Kbrd->grp2,0);
    kgSetGrpVisibility(D,Kbrd->cgrp,0);
    kgSetGrpVisibility(D,Kbrd->ongrp,0);
    kgSetGrpVisibility(D,Kbrd->grp1,1);
    kgSetGrpVisibility(D,Kbrd->grp3,1);
    kgSetGrpVisibility(D,Kbrd->sgrp,1);
    kgSetGrpVisibility(D,Kbrd->offgrp,1);
   }
   return 1;
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

int kgMakeKeybrd5(DIALOG *D,int Bx,int By,int Xg,int Yg,int Vis,int btype,int bfont,int fontclr,int butclr,int bkgrclr,float rfac,float transparency,int bodr,float Brfac) {

   int GrpId;
   WIDGETGRP *Gpt;
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
    int i=0,offset;
   int xo=0,yo=0;
   int Red,Green,Blue;
   int Bodr;
   int Bxmin,Bymin;
   DIA *dtmp,*d;
   Gclr gc;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)Malloc(sizeof(KEYBRD));
   D->Kbrd = Kbrd;
   gc = D->gc;
   Kbrd->TargetWindow = NULL;
   Kbrd->FillClr = ConvertColor(bkgrclr,80);
   Kbrd->XpmList = NULL;
   Kbrd->Thds=NULL;
   Kbrd->Brfac =Brfac;
   D->wc=NULL;
   Btype=btype;
   Bfont=bfont;
   Bclr = ConvertColor(fontclr,81);
   Btrans = transparency;
   ButClr = ConvertColor(butclr,82);
   if (By<30) strcpy(Kbrd->Sfac,"11");
   else if(By < 42) strcpy(Kbrd->Sfac,"54");
        else strncpy(Kbrd->Sfac,Sfac,3);
   Kbrd->Sfac[2]='\0';
   Kbrd->Bclr = Bclr;
   Kbrd->Bfont = Bfont;
   Kbrd->kbtype=5;
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
   gc = D->gc;
   D->wc=NULL;
   if(Bclr >= 100) Bclr =0;
   Kbrd->Btype=Btype;
   Kbrd->Rfac = Rfac;
   Kbrd->trans = transparency;
   
   dtmp = D->d;
   pt = Kbrd;
   i=0;
   if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
   offset = i;
   Kbrd->Thds=NULL;
   if(Kbrd->Btype < 1) {
#ifdef D_TH
     void *Thds;
     Thds = OpenThreads(getCores());
     Kbrd->Thds = Thds;
     GrpId = keybrd5Group(D,v,Kbrd);
     WaitThreads(Thds);
     CloseThreads(Thds);
     Kbrd->Thds= NULL;
#else
     GrpId = keybrd5Group(D,v,pt);
#endif
   }
   else {
     GrpId = keybrd5Group(D,v,pt);
   }
   Kbrd->GrpId= GrpId;
   
   Modifykeybrd5(D,offset);    /*  add extras to  gui*/
   Gpt = kgGetWidgetGrp(D,GrpId);
   kgSetGrpCleanup(D,GrpId,keybrd5cleanup);
   Gpt->arg= v; // kulina will double free this; you may modify
   CreateKeybrdGrps(D,Kbrd,offset,Vis,xo,yo);
   
   return GrpId;
}
int kgKeybrd(void *Tmp,int Vis,void *ktmp) {
  KBINFO *ki;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  ki = (KBINFO *)ktmp;
  kgInitGm();
  switch(ki->kbtype) {
    case 5:
      kgMakeKeybrd5(D,ki->xl,ki->yl,ki->xg,ki->yg,Vis,ki->btype,ki->font,
        ki->charclr,ki->butclr,ki->fillclr,ki->rfac,ki->trans,ki->Bodr,ki->Brfac);
      break;
    case 4:
      kgMakeKeybrd4(D,ki->xl,ki->yl,ki->xg,ki->yg,Vis,ki->btype,ki->font,
        ki->charclr,ki->butclr,ki->fillclr,ki->rfac,ki->trans,ki->Bodr,ki->Brfac);
      break;
    case 0:
      kgMakeKeybrd0(D,ki->xl,ki->yl,ki->xg,ki->yg,Vis,ki->btype,ki->font,
        ki->charclr,ki->butclr,ki->fillclr,ki->rfac,ki->trans,ki->Bodr,ki->Brfac);
      break;
    case 1:
      kgMakeKeybrd1(D,0,0,Vis,ki->btype,ki->font,
        ki->charclr,ki->butclr,ki->fillclr,ki->rfac,ki->trans);
      break;
    case 2:
      kgMakeKeybrd2(D,0,0,Vis,ki->btype,ki->font,
        ki->charclr,ki->butclr,ki->fillclr,ki->rfac,ki->trans);
      break;
    case 3:
      kgMakeKeybrd3(D,0,0,Vis,ki->btype,ki->font,
        ki->charclr,ki->butclr,ki->fillclr,ki->rfac,ki->trans);
      break;
    default:
      kgMakeKeybrd5(D,ki->xl,ki->yl,ki->xg,ki->yg,Vis,ki->btype,ki->font,
        ki->charclr,ki->butclr,ki->fillclr,ki->rfac,ki->trans,ki->Bodr,ki->Brfac);
      break;
  }
  return 1;
}

int keybrd5( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = keybrd5Group(&D,v,pt);
#else 
  GrpId = Makekeybrd5Group(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 29;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 583+200;    /*  Length of Dialog */
  D.yl = 239+50;    /*  Width  of Dialog */
  D.Initfun = keybrd5init;    /*   init fuction for Dialog */
  D.Cleanupfun = keybrd5cleanup;    /*   init fuction for Dialog */
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
  D.Callback = keybrd5CallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = keybrd5ResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = keybrd5WaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 0;    /*  1 for Fixing Position */
  D.NoTaskBar = 1;    /*  1 for not showing in task bar*/
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
//  Modifykeybrd5Gc(&(D.gc));    /*  set colors for gui*/
  Modifykeybrd5(&D,GrpId);    /*  add extras to  gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *Runkeybrd5(void *arg) {
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   keybrd5(NULL,v,pt );
   return NULL;
}
