#include <stdlib.h>
#include "gl.h"
#include "string.h"
Gclr gc,gcold;
int clrs[24];

void Setgc(void) {
  gc.char_clr = clrs[0];
  gc.fill_clr = clrs[2];
  gc.bodr_clr = clrs[4];
  gc.cur_clr = clrs[6];
  gc.high_clr = clrs[8];
  gc.char_hclr = clrs[10];
  gc.msg_char = clrs[12];
  gc.msg_fill = clrs[14];
  gc.msg_bodr = clrs[16];
  gc.txt_fill = clrs[18];
  gc.txt_char = clrs[20];
  gc.txt_pchar = clrs[22];
  gc.tabl_fill = clrs[1];
  gc.tabl_line = clrs[3];
  gc.tabl_char = clrs[5];
  gc.tabl_hchar = clrs[7];
  gc.v_dim = clrs[9];
  gc.dim = clrs[11];
  gc.bright = clrs[13];
  gc.vbright = clrs[15];
  gc.twin_fill = clrs[17];
  gc.twin_char = clrs[19];
  gc.twin_bodr = clrs[21];
}
int config(  void *v0, void *v1, void *v2, void *v3, 
 void *v4, void *v5, void *v6, void *v7, void *v8, 
 void *v9, void *v10, void *v11, void *v12, void *v13, 
 void *v14, void *v15, void *v16, void *v17, void *v18 ){
  int ret=1;
  DIALOG D;
  DIA d[13];
  int butn =1;
  int sw0[1]  = { 
    1  
  };
  char *titles0[]  = { 
    "Okay", 
    NULL
  };
  char *butncode0  = NULL ; 
  DIL h0 = { 
    'h',
    162,376,  
    9,3,
    62, 
    25, 
    1,1, 
    &butn, 
    sw0,
    titles0,
    236,406,
    butncode0,
    NULL,NULL /* arg, Callbak */
  };
  DIM m1 = { 
    'm',
    58,37,  
    "      Min          Max        Grids",
    387,57  
  };
  T_ELMT e2[9]  = { 
    {"X%12f", v0, 1},
    {"%12f", v1, 1},
    {"%3d", v2, 1},
    {"Y%12f", v3, 1},
    {"%12f", v4, 1},
    {"%3d", v5, 1},
    {"Z%12f", v6, 1},
    {"%12f", v7, 1},
    {"%3d", v8, 1}
  };
  DIT t2 = { 
    't',
    21,50,  
    20, 
    3,3, 
    e2,
    378,129,
    0,0,
    NULL,NULL /* args,Call back */
  };
  char *menu3[]  = { 
    "Bond_cond",
    "",
    "",
    "",
    "",
    "",
    NULL 
  };
  DIW w3 = { 
    'w',
    22,154,  
    4,  
    v9,
    "X" ,
    menu3 ,
    164,174,   
    NULL,NULL /* *args, callback */
  };
  char *menu4[]  = { 
    "Bond_Cond",
    "",
    "",
    "",
    "",
    "",
    NULL 
  };
  DIW w4 = { 
    'w',
    21,184,  
    4,  
    v10,
    "Y" ,
    menu4 ,
    163,204,   
    NULL,NULL /* *args, callback */
  };
  char *menu5[]  = { 
    "Bond_Cond",
    "",
    "",
    "",
    "",
    "",
    NULL 
  };
  DIW w5 = { 
    'w',
    20,214,  
    4,  
    v11,
    "Z" ,
    menu5 ,
    162,234,   
    NULL,NULL /* *args, callback */
  };
  char *menu6[]  = { 
    "Bond_Cond",
    "",
    "",
    "",
    "",
    "",
    NULL 
  };
  DIW w6 = { 
    'w',
    177,153,  
    4,  
    v12,
    "" ,
    menu6 ,
    310,173,   
    NULL,NULL /* *args, callback */
  };
  char *menu7[]  = { 
    "Bond_Cond",
    "",
    "",
    "",
    "",
    "",
    NULL 
  };
  DIW w7 = { 
    'w',
    176,183,  
    4,  
    v13,
    "" ,
    menu7 ,
    309,203,   
    NULL,NULL /* *args, callback */
  };
  char *menu8[]  = { 
    "Bond_Cond",
    "",
    "",
    "",
    "",
    "",
    NULL 
  };
  DIW w8 = { 
    'w',
    175,214,  
    4,  
    v14,
    "" ,
    menu8 ,
    308,234,   
    NULL,NULL /* *args, callback */
  };
  char *titles9[]  = { 
    "CLSTR", 
    "CLSTR", 
    "CLSTR", 
    NULL
  };
  char *butncode9  = NULL;
  DIBN n9 = { 
    'n',
    314,147,  
    9,9,  
    71, 
    25, 
    1,3, 
    v15, 
    "",
    titles9,
    403,246,
    butncode9,
    NULL,NULL, /* args, Callbak */
    NULL,NULL
  };
  char *menu10[]  = { 
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    NULL 
  };
  DIW w10 = { 
    'w',
    21,262,  
    4,  
    v16,
    "Grid Level" ,
    menu10 ,
    172,282,   
    NULL,NULL /* *args, callback */
  };
  T_ELMT e11[2]  = { 
    {"Refine. Criterion%10f", v17, 1},
    {"Unrefi. Criterion%10f", v18, 1}
  };
  DIT t11 = { 
    't',
    22,299,  
    20, 
    1,2, 
    e11,
    299,349,
    0,0,
    NULL,NULL /* args,Call back */
  };
  d[0].h = &h0;
  d[1].m = &m1;
  d[2].t = &t2;
  d[3].w = &w3;
  d[4].w = &w4;
  d[5].w = &w5;
  d[6].w = &w6;
  d[7].w = &w7;
  d[8].w = &w8;
  d[9].n = &n9;
  d[10].w = &w10;
  d[11].t = &t11;
  d[12].t = NULL;
  D.d = d;
  D.bkup = 1;
  D.bor_type = 2;
  D.df = -1;
  D.tw = 4;
  D.bw = 51;
  D.lw = 4;
  D.rw = 4;
  D.xo = 30;
  D.yo = 10;
  D.xl = 415;
  D.yl = 414;
 D.Initfun = NULL;
  ret= Dialog(&D);
  return ret;
}
void Runconfig(void) {
   float v0 = 0.0;
   float v1 = 0.0;
   int   v2 = 1;
   float v3 = 0.0;
   float v4 = 0.0;
   int   v5 = 1;
   float v6 = 0.0;
   float v7 = 0.0;
   int   v8 = 1;
   int   v9 = 1;
   int   v10 = 1;
   int   v11 = 1;
   int   v12 = 1;
   int   v13 = 1;
   int   v14 = 1;
   int   v15 = 1;
   int   v16 = 1;
   float v17 = 0.0;
   float v18 = 0.0;
  config( &v0, &v1, &v2, &v3, 
     &v4, &v5, &v6, &v7, &v8, 
     &v9, &v10, &v11, &v12, &v13, 
     &v14, &v15, &v16, &v17, &v18 );
}
int colorbox(int df) {
 int v0=df;
 int ret,i;
 DIA d[2];
 DIALOG D;
 DIBN b1 ={'n',10L,10L,5,5,50,25,1,1,NULL,NULL,NULL,0L,0L,NULL,NULL,NULL};
 char *titles0[] = {
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        NULL};
 int clrs[64];
 for(i=0;i<64;i++) clrs[i]=i;
 b1.prompt =(char *)malloc(sizeof(char)*1);
 b1.prompt[0]='\0';
 b1.xgap=9;
 b1.ygap=9;
 b1.width=15;
 b1.nx=8;
 b1.ny=8;
 b1.df=(void *)&v0;
 b1.titles=titles0;
 b1.lngth=15;
 b1.x1=14;
 b1.y1=14;
 b1.x2=317;
 b1.y2=299;
 b1.xpm=NULL;
 b1.bkgr=clrs;
 d[0].n = &b1;
 d[0].b->x1 =11;
 d[0].b->y1 =13;
 d[0].b->x2 =328;
 d[0].b->y2 =190;
 d[1].t=NULL;
 D.d =d; D.xo =157; D.yo =52; D.xl = 220; D.yl =210;
 D.bkup =1;
 D.bor_type=2;D.tw=4;D.lw=4;D.rw=4;D.bw=4;
 D.df = -1;
 D.Initfun = NULL;
 ret = Dialog(&D);
 ret =  v0-1;
 free(b1.prompt);
 return ret;
}
int update_color(int code ,int i,void *U){
  int df;
  DIALOG *D;
  DIBN * n;
  D = (DIALOG *)U;
  n = D->d[i].n;
  df = n->bkgr[code-1]+1;
  n->bkgr[code-1]=colorbox(df);
  Setgc();
  return 1;
}
int TestGclr(int key,int i,void *U){
   if((key=='O')||(key=='o')) return 1;
   else {
     gcold = get_gui_colours();
     set_gui_colours(gc);
     Runconfig();
     set_gui_colours(gcold);
     return 0;
   }

}
int Gclrdia(  void *v0, int *clrs){
  int ret=1;
  DIALOG D;
  DIA d[26];
  int butn =1;
  int sw0[2]  = { 
    1, 
    1  
  };
  char *titles0[]  = { 
    "Test", 
    "Okay", 
    NULL
  };
  char *butncode0  = "TOto" ; 
  DIL h0 = { 
    'h',
    175,384,  
    9,3,
    62, 
    25, 
    2,1, 
    &butn, 
    sw0,
    titles0,
    324,417,
    butncode0,
    NULL,TestGclr /* arg, Callbak */
  };
  char *titles1[]  = { 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    NULL
  };
  char *butncode1  = NULL;
  DIBN n1 = { 
    'n',
    205,15,  
    9,9,  
    15, 
    20, 
    2,12, 
    v0, 
    "",
    titles1,
    262,361,
    butncode1,
    NULL,update_color, /* args, Callbak */
    NULL,clrs
  };
  DIM m2 = { 
    'm',
    91,30,  
    "Char colour",
    204,50  
  };
  DIM m3 = { 
    'm',
    91,59,  
    "Fill colour",
    204,79  
  };
  DIM m4 = { 
    'm',
    27,87,  
    "Menu border Colour",
    203,107  
  };
  DIM m5 = { 
    'm',
    55,117,  
    "Textbox cur clr",
    204,137  
  };
  DIM m6 = { 
    'm',
    18,173,  
    "menu item char hclr",
    203,193  
  };
  DIM m7 = { 
    'm',
    46,202,  
    "message char clr",
    204,222  
  };
  DIM m8 = { 
    'm',
    45,230,  
    "message fill clr",
    203,250  
  };
  DIM m9 = { 
    'm',
    45,260,  
    "message bodr clr",
    203,280  
  };
  DIM m10 = { 
    'm',
    45,290,  
    "Textbox fill clr",
    202,310  
  };
  DIM m11 = { 
    'm',
    10,318,  
    " Textbox char colour",
    204,338  
  };
  DIM m12 = { 
    'm',
    10,348,  
    "Text prompt char clr",
    204,368  
  };
  DIM m13 = { 
    'm',
    263,29,  
    "table fill clr",
    403,49  
  };
  DIM m14 = { 
    'm',
    263,58,  
    "table line clr",
    403,78  
  };
  DIM m15 = { 
    'm',
    263,87,  
    "table char clr",
    403,107  
  };
  DIM m16 = { 
    'm',
    263,115,  
    "table out char clr",
    439,135  
  };
  DIM m17 = { 
    'm',
    263,145,  
    "very dim colour",
    412,165  
  };
  DIM m18 = { 
    'm',
    263,175,  
    "dim clr for 3D",
    403,195  
  };
  DIM m19 = { 
    'm',
    263,203,  
    "bright clr for 3D",
    430,223  
  };
  DIM m20 = { 
    'm',
    263,231,  
    "clr for key attn",
    421,251  
  };
  DIM m21 = { 
    'm',
    263,261,  
    "text window fill",
    421,281  
  };
  DIM m22 = { 
    'm',
    263,290,  
    "text window char",
    421,310  
  };
  DIM m23 = { 
    'm',
    263,319,  
    "text window border",
    439,339  
  };
  DIM m24 = { 
    'm',
    28,144,  
    "menu item high clr",
    204,164  
  };
  d[0].h = &h0;
  d[1].n = &n1;
  d[2].m = &m2;
  d[3].m = &m3;
  d[4].m = &m4;
  d[5].m = &m5;
  d[6].m = &m6;
  d[7].m = &m7;
  d[8].m = &m8;
  d[9].m = &m9;
  d[10].m = &m10;
  d[11].m = &m11;
  d[12].m = &m12;
  d[13].m = &m13;
  d[14].m = &m14;
  d[15].m = &m15;
  d[16].m = &m16;
  d[17].m = &m17;
  d[18].m = &m18;
  d[19].m = &m19;
  d[20].m = &m20;
  d[21].m = &m21;
  d[22].m = &m22;
  d[23].m = &m23;
  d[24].m = &m24;
  d[25].t = NULL;
  D.d = d;
  D.bkup = 0;
  D.bor_type = 2;
  D.df = -1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 30;
  D.yo = 30;
  D.xl = 481;
  D.yl = 427;
 D.Initfun = NULL;
  ret= Dialog(&D);
  return ret;
}
void Setarr(void) {
  clrs[0]= gc.char_clr;
  clrs[2]= gc.fill_clr;
  clrs[4]= gc.bodr_clr;
  clrs[6]= gc.cur_clr;
  clrs[8]= gc.high_clr;
  clrs[10]= gc.char_hclr;
  clrs[12]= gc.msg_char;
  clrs[14]= gc.msg_fill;
  clrs[16]= gc.msg_bodr;
  clrs[18]= gc.txt_fill;
  clrs[20]= gc.txt_char;
  clrs[22]= gc.txt_pchar;
  clrs[1]= gc.tabl_fill;
  clrs[3]= gc.tabl_line;
  clrs[5]= gc.tabl_char;
  clrs[7]= gc.tabl_hchar;
  clrs[9]= gc.v_dim;
  clrs[11]= gc.dim;
  clrs[13]= gc.bright;
  clrs[15]= gc.vbright;
  clrs[17]= gc.twin_fill;
  clrs[19]= gc.twin_char;
  clrs[21]= gc.twin_bodr;
  clrs[23]= 0;
}
void RunGclrdia(char *app) {
  int   v0 = 1;
  gc = get_gui_colours();
  Read_Gclr(&gc,app);  
  Setarr();
  Gclrdia( &v0,clrs );
  Write_Gclr(gc,app);
}
void main(int argc,char **argv) {
   if(argc <2) exit(1);
   RunGclrdia(argv[1]);
}
