#define LASER
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <malloc.h>
#include <unistd.h>
#include <malloc.h>
#include <magick/api.h>
#include <string.h>
#include "dlink.h"
#include "kulina.h"
#include "gprivate.h"
#include "gminterface.h"

static int B_max=990000,B_min=989000;
static int POINTSIZE=1;
static int MFAC=2;
char * ui_mktmpdir(void);
#define SSF 0.6
#define MAXZ 65000
#define uireset_greek {\
 if(dc->greek> 0) {\
   dc->icpos = dc->icpostmp;\
   dc->icxv = dc->icxvtmp;\
   dc->icyv = dc->icyvtmp;\
 }\
}
#define uiset_greek {\
 if(dc->greek> 0) {\
   dc->icpostmp = dc->icpos;\
   dc->icxvtmp = dc->icxv;\
   dc->icyvtmp = dc->icyv;\
   dc->icpos = dc->icposf0;\
   dc->icxv = dc->icxvf0;\
   dc->icyv = dc->icyvf0;\
 }\
}

#define uiset_sup_clip_limits(wc,x1,y1,x2,y2) wc->s_v_x1=x1,wc->s_v_y1=y1,wc->s_v_x2=x2,wc->s_v_y2=y2
#define uiset_clip_limits(wc,x1,y1,x2,y2) wc->c_v_x1=x1,wc->c_v_y1=y1,wc->c_v_x2=x2,wc->c_v_y2=y2
 Dlink *uiGetFontlist(void);
 void img_move(DIG *G,float x,float y);
 void img_draw(DIG *G,float x, float y);
 void img_marker(DIG *G,float x,float y);
 void img_DRAW_LINE(DIG *G,float x1,float y1,float x2,float y2);
 void img_drawline(DIG *G,float x1,float y1,float x2,float y2);
 void img_boxfill(DIG *G,float x1,float y1,float x2,float y2,int color);
 void img_circle(DIG *G,float x1,float y1,float r);
 void img_circlefill(DIG *G,float x1,float y1,float r,int color);
 void img_poly_fill(DIG *G,int n,float *x,float *y,int flag,int color);
 void img_viewport(DIG *G,float x1,float y1,float x2,float y2);
 void img_userframe(DIG *G,float x1,float y1,float x2,float y2);
 void img_userframe3d(DIG *G,float x1,float y1,float x2,float y2,float xe,float ye,float ze,float zc,float clmin,float clmax) ;
 void img_move3f(DIG *G,float x,float y,float z);
 void img_draw3f(DIG *G,float x,float y,float z);
 void img_polyfill3f(DIG *G,int n,float *x,float *y,float *z,int flag,int color);
 void img_boxfill3f(DIG *G,float x1,float y1,float z1,float x2,float y2,float z2,int ib,int color);
 void img_godrfill3f(DIG *G,int n,float *x,float *y,float *z,float *v);
 void img_set_markstyle(DIG *G,int style);
 void img_set_color(DIG *G,int color);
 void img_set_lnstyle(DIG *G,int style);
 void img_set_angle(DIG *G,float t);
 void img_txt_color(DIG *G, int color);
 void img_txt_fill(DIG *G, int color);
 void img_txt_slant(DIG *G, float slant);
 void img_txt_bodr(DIG *G, int color);
 void img_txt_bkgr(DIG *G, int color);
 void img_txt_pattern(DIG *G, int color);
 void img_txt_font(DIG *G, int font);
 void  img_txtwrt(DIG *G,char *txt);
 void img_set_lnwidth(DIG *G,int width);
 void img_set_txtbold(DIG *G,int bold);
 void img_godard_fill(DIG *G,int n,float *x,float *y,float *p);
  void img_set_godard_clrs(DIG *G,float max,float min,int ib,int startclr,int nclrs);
  void img_set_shade_clrs(DIG *G,float max,float min,int ib,int startclr,int nclrs);
 void img_set_prtxt(DIG *G,int txt );
 void  img_sup_clip_limit(DIG *G,float x1,float y1,float x2,float y2);
 void img_chng_clr(DIG *G,int no,int ir,int ig,int ib);
static pthread_mutex_t _Tmplock=PTHREAD_MUTEX_INITIALIZER;
#define Lock pthread_mutex_lock(&_Tmplock)
#define Unlock pthread_mutex_unlock(&_Tmplock)
//static unsigned long BkColor=0x00000000;
//static unsigned long BkColor=0x00000000;
//static unsigned long *buffer=NULL; /* image buffer */
//static  PixelPacket *pixels;

static void *IEEEtoPC(void *val);
static void *IEEEtoPCs(void *val);

#ifdef UNIX_VERSION
#define min(x,y) (x <= y? x : y)
#define max(x,y) (x >= y? x : y)
#endif

#define FREE(a) if(a!=NULL){free(a);a=NULL;}

extern short  kgIcode[1024][3];
//static kgIcodeLoc[1024][3];
#if 0
static unsigned long _uiPlotPixel(DIG *G,int x,int y,float v) {
  unsigned long color=1,no,loc;
  kgWC *wc;
  kgColor *kgcolors;
  PixelPacket *pixels;
  pixels = G->pixels;
  wc = G->wc;
  no = wc->c_color;
  loc = (y*(wc->EVGAY)+x);
  kgcolors = wc->kgcolors;
  pixels[loc].red = kgcolors[no].red*255*v;
  pixels[loc].green = kgcolors[no].green*255*v;
  pixels[loc].blue = kgcolors[no].blue*255*v;
  pixels[loc].opacity = 0x00;
  return color;
}
#endif
static unsigned long imgGetColor(DIG *G,int no,int loc) {
  unsigned long color=1;
  kgWC *wc;
  kgColor *kgcolors;
  PixelPacket *pixels;
  pixels = G->pixels;
  wc = G->wc;
  kgcolors = wc->kgcolors;
#if 0
  pixels[loc].red = kgIcode[no][0];
  pixels[loc].green = kgIcode[no][1];
  pixels[loc].blue = kgIcode[no][2];
  pixels[loc].opacity = 0x00;
#else
  pixels[loc].red = kgcolors[no].red;
  pixels[loc].green = kgcolors[no].green;
  pixels[loc].blue = kgcolors[no].blue;
  pixels[loc].opacity = 0x00;
#endif
  return color;
}
void imgCopyImage(DIG *G,int x0,int y0,GMIMG *img) {
  GMIMG *Dimg,*Simg;
  kgDC *dc;
  kgWC *wc;
  PixelPacket *pixels,*spixels;
  int w,h,iw,ih,i,j,ii,jj,sloc,dloc,cx0,cx1,cy0,cy1;
  int channels;
  dc= G->dc;
  wc =G->wc;
  Simg = (GMIMG *)img;
  Dimg = G->img;
  iw = Dimg->image_width;
  ih = Dimg->image_height; 
  w  = Simg->image_width;
  h  = Simg->image_height;
  channels= Simg->image_channels;
  cx0=wc->c_v_x1;
  cx1=wc->c_v_x2;
  cy0=dc->EVGAY-1-wc->c_v_y2;
  cy1=dc->EVGAY-1-wc->c_v_y1;
//  printf("%d %d %d %d\n",cx0,cy0,cx1,cy1);
  spixels = GetImagePixels((Image *)(Simg->image),0,0,((Image *)(Simg->image))->columns,((Image *)(Simg->image))->rows);
  pixels = G->pixels;
  for(j=0;j<(h);j++) {
    jj = j+y0;
    if(jj<cy0)continue;
    if(jj>=cy1) break;
    if(jj>= ih) break;
    
    for(i=0;i<w;i++) {
      ii =i+x0;
      if(ii<cx0) continue;
      if(ii>cx1) continue;
      if(ii >= iw) continue;
      sloc = j*w+i;
      dloc= jj*iw+ii;
      if( (channels!=4)||(spixels[sloc].opacity != 255)) {
        pixels[dloc]=spixels[sloc];
        pixels[dloc].blue = spixels[sloc].blue;
        pixels[dloc].green = spixels[sloc].green;
        pixels[dloc].red = spixels[sloc].red;
        pixels[dloc].opacity = spixels[sloc].opacity;
      }
//TCB
//      printf("%x %x %x %x\n",pixels[dloc].blue,pixels[dloc].green,pixels[dloc].red,pixels[dloc].opacity);
//      pixels[dloc].red=255;
    }
  }
  return;
}
//function plot(x, y, c) is
//     plot the pixel at (x, y) with brightness c (where 0 ≤ c ≤ 1)
 
#define Ipart(x) ((int)(x))
#define Round(x) ((int)(x+0.5))
#define Fpart(x) ((x)-Ipart(x))
#define Rfpart(x) ( 1-Fpart(x))
#define Swap(x,y) {temp=x;x=y;y=temp;}
#if 0
static void _uialine (DIG *G,float x0,float y0,float x1,float y1){
/*
  Wu's algorithm
*/
   int steep;
   int yend,xpxl1,ypxl1;
   float xend,temp;
   int xpxl2,ypxl2,x;
   float dx,dy,gradient,xgap,intery;
   steep= (abs(y1 - y0) > abs(x1 - x0))?1:0;
   if(steep) {Swap(x0,y0);Swap(x1,y1);}
   if(x0 > x1) {Swap(x0,y0);Swap(x1,y1);}
   dx = x1 - x0;
   dy = y1 - y0;
   gradient = dy / dx;
   xend = Round(x0);
   yend = y0 + gradient * (xend - x0);
   xgap = Rfpart(x0 + 0.5);
   xpxl1 = xend ;
   ypxl1 = Ipart(yend);
   if (steep) { 
         _uiPlotPixel(G,ypxl1,   xpxl1, Rfpart(yend) * xgap);
         _uiPlotPixel(G,ypxl1+1, xpxl1,  Fpart(yend) * xgap);
   } 
   else {
         _uiPlotPixel(G,xpxl1, ypxl1  , Rfpart(yend) * xgap);
         _uiPlotPixel(G,xpxl1, ypxl1+1,  Fpart(yend) * xgap);
   }
   intery = yend + gradient ;
   xend = Round(x1);
   yend = y1 + gradient * (xend - x1);
   xgap = Fpart(x1 + 0.5);
   xpxl2 = xend;
   ypxl2 = Ipart(yend);
   if (steep) {
        _uiPlotPixel(G,ypxl2  , xpxl2, Rfpart(yend) * xgap);
        _uiPlotPixel(G,ypxl2+1, xpxl2,  Fpart(yend) * xgap);
   }
   else {
       _uiPlotPixel(G,xpxl2, ypxl2,  Rfpart(yend) * xgap);
       _uiPlotPixel(G,xpxl2, ypxl2+1, Fpart(yend) * xgap);
   }
   for (x =xpxl1 + 1 ;x<=xpxl2 - 1;x++ ) {
         if (steep) {
             _uiPlotPixel(G,Ipart(intery)  , x, Rfpart(intery));
             _uiPlotPixel(G,Ipart(intery)+1, x,  Fpart(intery));
         }
         else {
             _uiPlotPixel(G,x, Ipart (intery),  Rfpart(intery));
             _uiPlotPixel(G,x, Ipart (intery)+1, Fpart(intery));
         }
         intery = intery + gradient;
   }
}
#endif
#if 0
#include "font0.h"
#include "font1.h"
#include "font2.h"
#include "font3.h"
#include "font4.h"
#include "font5.h"
#include "font6.h"
#include "font7.h"
#include "font8.h"
#include "font9.h"
#include "font10.h"
#include "font11.h"
#include "font12.h"
#include "font13.h"
#include "font14.h"
#include "font15.h"
#include "font16.h"
#include "font17.h"
#include "font18.h"
#include "font19.h"
#include "font20.h"
#include "font21.h"
#include "font22.h"
#include "font23.h"
#include "font24.h"
#include "font25.h"
#include "font26.h"
#include "font27.h"
#include "font28.h"
#include "font29.h"
#include "font30.h"
#include "font31.h"
#include "font32.h"
#include "font33.h"
#include "font34.h"
#include "font35.h"
#include "font36.h"
#endif

typedef struct {
  char *fontname;
  int *icpos;
  int *icxv;
  int *icyv;
  float *m_f;
} FONT;
static unsigned long imgGetColor(DIG *G,int no,int loc);
static unsigned short *zbuf=NULL;
static int Z_max=193600L;
static char FontName[30]={"Times-Roman"};
static const float CFact=300000.0;
static Dlink *Pnlist = NULL;
typedef struct Line {
  char code; /* l for line */
  float zmax,zmin;
  float x1,y1,z1,x2,y2,z2;
  int clr,width,style;
} LINE;

typedef struct Poly {
  char code; /* p for poly */
  float zmax,zmin;
  int n;
  float *x,*y,*z;
  int fclr,bclr,flag;
} POLY;
typedef struct Shade {
  char code; /* s for shaded polygon */
  float zmax,zmin;
  int n;
  float *x,*y,*z,*v;
  int bclr;
  void *shadeinfo;
} SHADE;
static const int Mag=1;
static str1 newstr,trnstr;
static LINE *lt;
static POLY *pt;
static SHADE *st;
static float Cx,Cy,Cz;
static unsigned int ln_ptn[10]={0xffffffff,0x88888888, 0xcccccccc, 0xf0f0f0f0,
            0xff18ff18,0xffff0f0f, 0xe7e7e7e7, 0xfafafafa,
            0xffff0000,0xffffff00};
static short st_ptr[10]={0,0,0,0,0,0,0,0,0,0};

#define TX(x,y) (dc->cx +(x)*dc->cost+(y)*dc->sint)
#define TY(x,y) (dc->cy -(x)*dc->sint+(y)*dc->cost)
#define z_inter(x,m,c) (int)(((x)*(m) + c)+0.5)
#define fz_inter(x,m,c) (float)(((x)*(m) + c))

static L_N *O_L=NULL;
static B_K *O_P=NULL,*D_P;
typedef struct c_limit{ int cx1;int cy1;int cx2;int cy2;int * pt;} CLIP;
static CLIP *c_loc;

/* For UNIX  */
#ifdef UNIX
#define far  
#define O_BINARY 0x00
#endif
/* End For UNIX */
#define PIX 8
#define mask 0xCF
#define rad 0.0174532
#define LN_STYL 0
#define M_STYL 0
#define true 1
#define set_clr(c) c_color=c
#define scr_x(x) (int)((x-dc->w_x1)*dc->u_x+0.5)
#define fscr_y(y) (float)((y-dc->w_y1)*dc->u_y)
#define fscr_x(x) (float)((x-dc->w_x1)*dc->u_x)
#define scr_y(y) (int)((y-dc->w_y1)*dc->u_y+0.5)
#define scr_z(z) (int)((z-dc->clip_min)*dc->CPCONS+0.5);
#define fscr_z(z) (float)((z-dc->clip_min)*dc->CPCONS);
#define cnv_x(x) (x*fx-sx)
#define cnv_y(y) (y*fy-sy)
#define set_clip_limits(x1,y1,x2,y2,x3,y3) wc->c_v_x1=x1,wc->c_v_y1=y1,wc->c_v_x2=x2,wc->c_v_y2=y2
#define set_sup_clip_limits(x1,y1,x2,y2) wc->s_v_x1=x1,wc->s_v_y1=y1,wc->s_v_x2=x2,wc->s_v_y2=y2
#define pclr(p)  (float)(((p-dc->pmin)*dc->dfac+0.))

static float PS_Slant=0.,Slant=0.,Slant_o=0.,Slnt[2]={0.0,0.25};
static float *m_F;
static int *icxvF,*icyvF;
static float pmax,pmin,dfac;
static float x_mulf=1.0,y_mulf=1.0;
static unsigned char godr_ptn[13]={0,6,7,9,14,13,12,4,10,2,1,3,15};

static unsigned char MASK[8]={ 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
static unsigned char MASKB[8]={0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };
static unsigned char MASKE[8]={0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF };
static unsigned char RMASK[4]={ 0x00, 0x01, 0x10, 0x11 };
static unsigned char MMASK[4]={ 0x01, 0x02, 0x04, 0x08 };
static unsigned char RED[4]={0x00, 0x04, 0x20, 0x24 };
static unsigned char BLUE[4]={0x00, 0x01, 0x08, 0x09 };
static unsigned char GREEN[4]={0x00, 0x02, 0x10, 0x12 };

static void put_pixl(DIG *G,short col, short row);
static void put_dot(DIG *G,short col, short row);
static void move(DIG *G,float x,float y);
static void marker(DIG *G,float x,float y);
static void draw(DIG *G,float x,float y);
static void DRAW_LINE(DIG *G,float x1,float y1,float x2,float y2);
static void box_fill(DIG *G,float x1,float y1,float x2,float y2,unsigned int color);
static void poly_fill(DIG *G,short n,float *x,float *y,short flag,unsigned int color);
static void godard_fill(DIG *G,short n,float *px,float *py,short flag,float *pv);
static void circle(DIG *G,float x1,float y1,float r);
static void circle_fill(DIG *G,float x1,float y1,float r,unsigned int color);
static void _move(DIG *G,int x,int y);
static void _draw(DIG *G,int x,int y);
static void draw_line(DIG *G,int xa,int ya,int xb,int yb);
static void hori_line(DIG *G,int xa,int xb,int y);
static void phori_line(DIG *G,int xa,int xb,int y);
static void godr_line(DIG *G,int xa,int xb,int y,float pa,float pb);
static void put_pixel(DIG *G,int col, int row);
static void txt_write(DIG *G,char *txt);
static void txtwrt(DIG *G,char *txt);
static void user_cord(float x1,float y1,float x2,float y2);
static void _box_fill(DIG *G,int x1,int y1,int x2,int y2,unsigned int color);
static void _poly_fill(DIG *G,short n,int *px,int *py,short flag,unsigned int b_color);
static void _mpoly_fill(DIG *G,short n,int *px,int *py,int *px1,int *py1,unsigned int b_color);
static void _godard_fill(DIG *G,short n,int *px,int *py,short flag,float *pv);
static void _circle(DIG *G,int x,int y,int r);
static void _circle_fill(DIG *G,int x,int y1,int r,unsigned int color);
static short clip_line(DIG *G,int *xa,int *ya,int *xb,int *yb);
static unsigned char code(kgWC *wc,int x,int y);
static void _rect_fill(DIG *G,short x1,short y1,short x2,short y2,unsigned int color);
static void _draw_bound(DIG *G,short x1,short y1,short x2,short y2);
static void _draw_line(DIG *G,int x1,int y1,int x2,int y2);
static int t_fillchr(kgWC *wc,float *x,float *y,int n,int *x1,int *y1,int *x2,int*y2);
static void godr_line3(DIG *G,int xa,int xb,int y,int za,int zb,float pa,float pb);
static void _marker(DIG *G,int x,int y);
static void shade_line3(DIG *G,int xa,int xb,int y,int za,int zb,float pa,float pb);
static int  uistrlngth_loc(kgDC *dc,char *title,float *xdsp);


static short GHMAX=0,SCRN,S_OPEN=0;
static char *GHBUF;
static char df_gbuf[20]={"No help available"};
static unsigned char far *Rbuff;

static char ch,bf[200]={" "};
static unsigned char cmds[1000];
static short step;
static unsigned char PIXL=0xFF;
static int B_OFF_SET=0;
#define CURKEY 00
#define EVGA_CN(y)  (short)(y*((float)EVGAY/1799.0))
static unsigned char OFF=0xFF;
static unsigned char C_CLR=0xFF;
static char PAK_B=0x18;
static char fname[15];

static unsigned char arr[] ={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
#define EIGHT 8
static unsigned char fil_pt[16][16][2]={
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,

    0x11,0x11, 0x22,0x22, 0x44,0x44, 0x88,0x88,
    0x11,0x11, 0x22,0x22, 0x44,0x44, 0x88,0x88,
    0x11,0x11, 0x22,0x22, 0x44,0x44, 0x88,0x88,
    0x11,0x11, 0x22,0x22, 0x44,0x44, 0x88,0x88,

    0x33,0x33, 0x66,0x66, 0xcc,0xcc, 0x99,0x99,
    0x33,0x33, 0x66,0x66, 0xcc,0xcc, 0x99,0x99,
    0x33,0x33, 0x66,0x66, 0xcc,0xcc, 0x99,0x99,
    0x33,0x33, 0x66,0x66, 0xcc,0xcc, 0x99,0x99,

    0x55,0x55, 0xaa,0xaa, 0x55,0x55, 0xaa,0xaa,
    0x55,0x55, 0xaa,0xaa, 0x55,0x55, 0xaa,0xaa,
    0x55,0x55, 0xaa,0xaa, 0x55,0x55, 0xaa,0xaa,
    0x55,0x55, 0xaa,0xaa, 0x55,0x55, 0xaa,0xaa,

    0x44,0x44, 0x44,0x44, 0x44,0x44, 0x44,0x44,
    0x44,0x44, 0x44,0x44, 0x44,0x44, 0x44,0x44,
    0x44,0x44, 0x44,0x44, 0x44,0x44, 0x44,0x44,
    0x44,0x44, 0x44,0x44, 0x44,0x44, 0x44,0x44,

    0x00,0x01, 0x00,0x02, 0x00,0x04, 0x00,0x08,
    0x00,0x10, 0x00,0x20, 0x00,0x40, 0x00,0x80,
    0x01,0x00, 0x02,0x00, 0x04,0x00, 0x08,0x00,
    0x10,0x00, 0x20,0x00, 0x40,0x00, 0x80,0x00,

    0x80,0x00, 0x40,0x00, 0x20,0x00, 0x10,0x00,
    0x08,0x00, 0x04,0x00, 0x02,0x00, 0x01,0x00,
    0x00,0x80, 0x00,0x40, 0x00,0x20, 0x00,0x10,
    0x00,0x08, 0x00,0x04, 0x00,0x02, 0x00,0x01,

    0xff,0xff, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0xff,0xff, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
   
    0x80,0x00, 0x80,0x00, 0x80,0x00, 0x80,0x00, 
    0x80,0x00, 0x80,0x00, 0x80,0x00, 0x80,0x00, 
    0x80,0x00, 0x80,0x00, 0x80,0x00, 0x80,0x00, 
    0x80,0x00, 0x80,0x00, 0x80,0x00, 0x80,0x00, 

    0x10,0x00, 0x20,0x00, 0x40,0x00, 0x80,0x00,
    0x01,0x00, 0x02,0x00, 0x04,0x00, 0x08,0x00,
    0x00,0x10, 0x00,0x20, 0x00,0x40, 0x00,0x80,
    0x00,0x01, 0x00,0x02, 0x00,0x04, 0x00,0x08,

    0x81,0x81, 0x42,0x42, 0x24,0x24, 0x18,0x18,
    0x18,0x18, 0x24,0x24, 0x42,0x42, 0x81,0x81,
    0x81,0x81, 0x42,0x42, 0x24,0x24, 0x18,0x18,
    0x18,0x18, 0x24,0x24, 0x42,0x42, 0x81,0x81,

    0x80,0x00, 0x80,0x00, 0x80,0x00, 0x80,0x00,
    0x80,0x00, 0x80,0x00, 0x80,0x00, 0x80,0x00,
    0x80,0x00, 0x80,0x00, 0x80,0x00, 0x80,0x00,
    0x80,0x00, 0x80,0x00, 0x80,0x00, 0xff,0xff,

    0x80,0x00, 0x80,0x00, 0x80,0x00, 0x80,0x00,
    0x80,0x00, 0x80,0x00, 0x80,0x00, 0xff,0xff,
    0x08,0x00, 0x08,0x00, 0x08,0x00, 0x08,0x00,
    0x08,0x00, 0x08,0x00, 0x08,0x00, 0xff,0xff,

    0x00,0x00, 0x01,0x01, 0x00,0x00, 0x10,0x10,
    0x00,0x00, 0x01,0x01, 0x00,0x00, 0x10,0x10,
    0x00,0x00, 0x01,0x01, 0x00,0x00, 0x10,0x10,
    0x00,0x00, 0x01,0x01, 0x00,0x00, 0x10,0x10,

    0x00,0x00, 0x01,0x01, 0x00,0x00, 0x04,0x04,
    0x00,0x00, 0x20,0x20, 0x00,0x00, 0x80,0x80,
    0x00,0x00, 0x01,0x01, 0x00,0x00, 0x04,0x04,
    0x00,0x00, 0x20,0x20, 0x00,0x00, 0x80,0x80,

    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
   };
#if 0
static void SetIcode(void) {
 int i;
 for(i=0;i<1024;i++) {
  kgIcodeLoc[i][0]= kgIcode[i][0];
  kgIcodeLoc[i][1]= kgIcode[i][1];
  kgIcodeLoc[i][2]= kgIcode[i][2];
 }
}
#endif
void img_move(DIG *G,float x,float y)
{
  int x1,y1;
  kgDC *dc;
  dc= G->dc;
  x1=scr_x(x);
  y1=scr_y(y);
  _move(G,x1,y1);
}

void img_draw(DIG *G,float x, float y)
{
  int x1,y1;
  kgDC *dc;
  dc= G->dc;
  x1=scr_x(x);
  y1= scr_y(y);
  dc->c_color=dc->ln_color;
  _draw(G,x1,y1);
}

void img_marker(DIG *G,float x,float y)
{
  int x1,y1;
  kgDC *dc;
  dc= G->dc;
  x1=scr_x(x);
  y1= scr_y(y);
  dc->c_color=dc->ln_color;
  _marker(G,x1,y1);
}
void img_DRAW_LINE(DIG *G,float x1,float y1,float x2,float y2)
{
  int xa,ya,xb,yb;
  kgDC *dc;
  dc= G->dc;
  
  xa=(int)(x1*dc->u_x);
  ya=(int)(y1*dc->u_y);
  xb=(int)(x2*dc->u_x);
  yb=(int)(y2*dc->u_y);
  dc->c_color=dc->ln_color;
  _draw_line(G,xa,ya,xb,yb);
}


void img_drawline(DIG *G,float x1,float y1,float x2,float y2)
{
  int xa,ya,xb,yb;
  kgDC *dc;
  dc= G->dc;
  xa=(int)(x1*dc->u_x);
  ya=(int)(y1*dc->u_y);
  xb=(int)(x2*dc->u_x);
  yb=(int)(y2*dc->u_y+0);
  img_DRAW_LINE(G,xa,ya,xb,yb);
}

void img_boxfill(DIG *G,float x1,float y1,float x2,float y2,int color)
{
  int xa,ya,xb,yb;
  kgDC *dc;
  dc= G->dc;
  xa=scr_x(x1);
  ya=scr_y(y1);
  xb=scr_x(x2);
  yb=scr_y(y2);
  _box_fill(G,xa,ya,xb,yb,(unsigned int)color);
}

void img_circle(DIG *G,float x1,float y1,float r)
{
  int xa,ya,rd;
  kgDC *dc;
  dc= G->dc;
  xa=scr_x(x1);
  ya=scr_y(y1);
  rd=scr_x(x1+r);
  rd-=xa;
  dc->c_color=dc->ln_color;
  _circle(G,xa,ya,rd);
  return;
}

void img_circlefill(DIG *G,float x1,float y1,float r,int color)
{
  int xa,ya,rd;
  kgDC *dc;
  dc= G->dc;
  xa=(int)(x1*dc->u_x);
  ya=(int)(y1*dc->u_y+dc->v_y1);
  rd=(int)(r/dc->r_d*dc->v_r);
  _circle_fill(G,xa,ya,rd,color);
  return;
}



void img_poly_fill(DIG *G,int n,float *x,float *y,int flag,int color)
{
  int x1[1000],y1[1000];
  short i,j=0;
  int xo,yo,xv,yv;
  kgDC *dc;
  dc= G->dc;
  x1[0] =scr_x(x[0]); 
  y1[0] =scr_y(y[0]);
  j++;xo=x1[0],yo=y1[0];
  for(i=1;i<n;i++)
  {
    xv =scr_x(x[i]); 
    yv =scr_y(y[i]);
    if( (xv!=xo) || (yv!=yo)){
      x1[j] =xv;
      y1[j] =yv;
      xo=xv,yo=yv;
      j++;
    }
  }
  _poly_fill(G,(int)j,x1,y1,(int)flag,(unsigned int)color);
}

static void put_pix3(DIG *G,int col,int row,int z)
{
  int r,c,EVGAX,row1;
  unsigned short  *loc;
  unsigned short *zbuf;
  kgDC *dc;
  kgWC *wc;
  dc = G->dc;
  if( (z<0) || (z>65000L)) return;
  wc = G->wc;
  row1 = dc->EVGAY-1-row;
  if ((col > wc->c_v_x2) || (col <wc->c_v_x1) || (row1 > wc->c_v_y2) || (row1 <wc->c_v_y1)) return;
  zbuf = ((kgDC *)(G->dc))->zbuf;
  EVGAX = dc->EVGAX;
  loc=zbuf+row*EVGAX+col;
  if (z < *(loc)) return;
  *(loc) =z;
  put_pixl(G,col,row);

}

void img_userframe3d(DIG *G,float x1,float y1,float x2,float y2,float xe,float ye,float ze,float zc,float clmin,float clmax) {
   FILE *fp;
#if 0
//   printf("Set_3d\n");
   read_buf(&x1,4);
   read_buf(&y1,4);
   read_buf(&x2,4);
   read_buf(&y2,4);
   /*user_cord(x1,y1,x2,y2);*/
   /*clip_limit(x1,y1,x2,y2); */
   read_buf(FM,64);
   read_buf(&xe,4);
   read_buf(&ye,4);
   read_buf(&ze,4);
   read_buf(&zc,4);
   read_buf(&clip_min,4);
   read_buf(&clip_max,4);
   CPCONS = 65000./(float)(clip_max-clip_min);
   fp = fopen("InfoFil","w");
   fprintf(fp," %f %f %f\n",clip_min,clip_max,CPCONS);
   fclose(fp);
#endif
}
static void t_move(DIG *G,float x,float y)
  {
  int x1,y1;
  kgDC *dc;
  dc= G->dc;
  x1=scr_x(x);
  y1=scr_y(y);
  _move(G,x1,y1);
  }
static  void t_draw(DIG *G,float ix,float iy)
  {
  int x1,y1;
  kgDC *dc;
  dc= G->dc;
  x1=scr_x(ix);
  y1=scr_y(iy);
  _draw(G,x1,y1);
  }
static void _move3(DIG *G,int xp, int yp,int zp)
{
  kgDC *dc;
  dc= G->dc;
  dc->cur_x=xp ;
  dc->cur_y=yp ;
  dc->cur_z=zp;
  return;
}
static void t_3_move(DIG *G,float x,float y,float z) 
{
  int x1,y1,z1;
  kgDC *dc;
  dc= G->dc;
  x1=scr_x(x);
  y1=scr_y(y);
  z1=scr_z(z);
  dc->cur_x=x1 ;
  dc->cur_y=y1 ;
  dc->cur_z=z1;
}
void img_move3f(DIG *G,float x,float y,float z)
{
  t_3_move(G,x,y,z);
}

static void _draw_line3(DIG *G,int x1,int y1,int z1,int x2,int y2,int z2)
{
 char far *addr;
 short i,dx,dy,f,g,inc1,inc2,ab,dx1,dy1,miny,maxy,minx,maxx;
 int clip,ch,rh,r,c;
 unsigned int temp;
 int xa,ya,xb,yb,za,zb,z;
 float cons,m;
 double xf,yf;
 int EVGAY;
 kgDC *dc;
 dc= G->dc;
 EVGAY= dc->EVGAY-1;
 xa =x1,xb=x2,ya=y1,yb=y2,za=z1,zb=z2;
 xf=x2-x1;yf=y2-y1;
 if ((((z1)<0)&&((z2)<0))||(((z1)>65000L)&&((z2)>65000L))) return;
 clip=clip_line(G,&xa,&ya,&xb,&yb);
 if (clip==0) return;
 if (abs(xf)<= abs(yf))
 {
  if( y1==y2) m=0;
  else m = (float)(z2-z1)/(float)(yf);
  cons = z1 - y1*m;
  za=z_inter(ya,m,cons);zb=z_inter(yb,m,cons);
 }
 else
 {
  m = (float)(z2-z1)/(float)(xf);
  cons = z1 - x1*m;
  za=z_inter(xa,m,cons);zb=z_inter(xb,m,cons);
 }
 if ( (xa!=xb) && (ya!=yb) )
	{
	  dx=xb-xa;
	  dy=yb-ya;
	  dx1=abs(dx);
	  dy1=abs(dy);
	  if ( dx1 > dy1 )
	  {
		 c=xa;
		 r=ya;
		 f=xb;
		 inc1=(dy1<<1);
		 g=inc1-dx1;
		 inc2=g-dx1;
		 if (xa < xb) ch=1;
		 else ch=-1;
		 if (ya < yb) rh=1;
		 else rh=-1;
/*        while (labs(c-f)>0)*/
		 r = EVGAY-r;
		 while ((c!=f))
		  {
		    z = z_inter(c,m,cons);
		    put_pix3(G,c,r,z);
		    c+=ch;
		    if (g > 0) { r-=rh; g+=inc2; }
		    else g+=inc1;
		  }
	  z = z_inter(xb,m,cons);
	  put_pix3(G,xb,(EVGAY-yb),z);
	  }
	  else
	  {
		 c=ya;
		 r=xa;
		 f=yb;
		 inc1=(dx1<<1);
		 g=inc1-dy1;
		 inc2=g-dy1;
		 if (ya < yb) ch=1;
		 else ch=-1;
		 if (xa < xb) rh=1;
		 else rh=-1;
/*        while (labs(c-f)>0)*/
		 c = EVGAY-c;
		 f = EVGAY-f;
		 while ((c!=f))
		 {
		   z = z_inter((EVGAY-c),m,cons);
		   put_pix3(G,r,c,z);
		   c-=ch;
		   if (g > 0) { r+=rh; g+=inc2; }
		   else g+=inc1;
		 }
	  z = z_inter(yb,m,cons);
	  put_pix3(G,xb,(EVGAY-yb),z);
	  }
  }
  else {
	if (xa==xb)
	{
	  if ( ya < yb){ miny = ya;maxy = yb;}
	  else { miny = yb; maxy = ya;}
  	  for (i=miny;i<=maxy;++i)
            { z = z_inter(i,m,cons);
              put_pix3(G,xa,(EVGAY-i),z);}
	}
       else 
	{
	  if ( xa < xb){ minx = xa;maxx = xb;}
	  else { minx = xb; maxx = xa;}
          for (i=minx;i<=maxx;++i)
          {  
	    z = z_inter(i,m,cons);
	    put_pix3(G,i,(EVGAY-ya),z);
          }
	 }
  }
  return;
}

static void draw_line3(DIG *G,int xa,int ya,int za,int xb,int yb,int zb)
 {
   int i,i2;
   float dx,dy,ct,st,dl;
   int x1,y1,z1,x2,y2,z2,dx1,dy1;
  kgDC *dc;
  dc= G->dc;
   if( dc->ln_width ==1){ _draw_line3( G,xa, ya, za, xb, yb, zb);return;};
   if( xa>xb){x1=xa,xa=xb,xb=x1,y1=ya,ya=yb,yb=y1;};
   i2=dc->ln_width/2;
   dx = (float)(xb-xa);
   dy = (float)(yb-ya);
   dl = (dx*dx+dy*dy);
   if( dx==0.) {ct=0.0;st=1.;}
   else { dl =sqrt(dl),ct = (dx)/dl, st = (dy)/dl;}
   dx1 =(int)( st*i2+0.5);
   if( ct< 0.) dy1 =(int)( ct*i2-0.5);
   else dy1 =(int)( ct*i2+0.5);
   xa +=dx1,xb +=dx1,ya -=dy1,yb -=dy1;
   _draw_line3( G,xa, ya, za, xb, yb, zb);
   for (i=1;i<dc->ln_width;i++)
	{
	     dx1 =(int)( st*i+0.5);
	     if( ct< 0.) dy1 =(int)( ct*i-0.5);
	     else dy1 =(int)( ct*i+0.5);
	    _draw_line3( G,xa-dx1,ya+dy1,za,xb-dx1,yb+dy1,zb);
	}
   return;
 }
static void hori_line3(DIG *G,int xa,int xb,int y,int za,int zb)
{
  short i,clip;
  char far *addr;
  int temp;
  int x1,x2,z1,z2;
  float cons,m;
  int z;
  int EVGAY;
  kgDC *dc;
  dc= G->dc;
  EVGAY = dc->EVGAY-1;
  x1=xa;x2=xb;z1=za;z2=zb;
  if (((za<0)&&(zb<0))||((za>65000L)&&(zb>65000L))) return;
  clip=clip_line(G,&xa,&y,&xb,&y);
  if (clip==0) return;
  if(xa==xb){
     put_pix3(G,xa,(EVGAY-y),za);
     return;
  }
  m = (float)(z2-z1)/(float)(x2-x1);
  cons = z1 - x1*m;
  za=z_inter(xa,m,cons);zb=z_inter(xb,m,cons);
  if (xb<xa)
  {
    temp=xa;
    xa=xb;
    xb=temp;
  }
  for(i=xa;i<=xb;i++) {z=z_inter(i,m,cons);put_pix3(G,i,(EVGAY-y),z);}
}


static void _draw3_(DIG *G,int x, int y, int z)
{
  int xa, ya, za, xb, yb,zb;
  short  dx, dy,  f, inc1, inc2, g,dx1,dy1,ab;
  short clip,i,ch,rh,c,r;
  float h,m,cons;
  int EVGAY;
  kgDC *dc;
  dc= G->dc;
  EVGAY = dc->EVGAY-1;

  xa=dc->cur_x;
  ya=dc->cur_y;
  za=dc->cur_z;

  xb=x;
  yb=y;
  zb=z;
  if (((za<0)&&(zb<0))||((za>65000L)&&(zb>65000L))) return;
  clip=clip_line(G,&xa,&ya,&xb,&yb);
   if (clip == 0)
   {
	dc->cur_x=x;
	dc->cur_y=y;
	dc->cur_z=z;
	return;
   }
  if (dc->cur_x == x)
   {
    m = (float)(y-dc->cur_y)/(float)(z-dc->cur_z);
    cons = dc->cur_z - dc->cur_y*m;  
    za=z_inter(ya,m,cons);zb=z_inter(yb,m,cons);
   }
   else
   {
    m = (float)(x-dc->cur_x)/(float)(z-dc->cur_z);
    cons = dc->cur_z - dc->cur_x*m;  
    za=z_inter(xa,m,cons);zb=z_inter(xb,m,cons);
   }
   dx=xb-xa;
   dy=yb-ya;
   dx1 = abs(dx);
   dy1 = abs(dy);
   if ( dx1 > dy1 )
   {
    c=xa;
    r=ya;
    f=xb;
    inc1=2*dy1;
    g=inc1-dx1;
    inc2=g-dx1;
    if (xa < xb) ch=1;
    else ch=-1;
    if (ya < yb) rh=1;
    else rh=-1;
   }
   else
   {
    c=ya;
    r=xa;
    f=yb;
    inc1=2*dx1;
    g=inc1-dy1;
    inc2=g-dy1;
    if (ya < yb) ch=1;
    else ch=-1;
    if (xa < xb) rh=1;
    else rh=-1;
   }
   while (abs(c-f)>0)
   { i=0;
     while ((i<=dc->ln_style) && (abs(c-f)>0))
	 {
	 if ( dx1 > dy1 ) {z=z_inter(c,m,cons);put_pix3(G,c,(EVGAY-r),z);}
	 else   {z=z_inter(r,m,cons);put_pix3(G,r,(EVGAY-c),z);}
	 c+=ch;
         if (g > 0) { r+=rh; g+=inc2; }
         else g+=inc1;
         i++;
	 }
    i=0;
    while ((i<=dc->ln_style) && (abs(c-f)>0))
	 {
	 c+=ch;
         if (g > 0) { r+=rh; g+=inc2; }
         else g+=inc1;
         i++;
	 }
    }
    dc->cur_x=x;
    dc->cur_y=y;
    dc->cur_z=z;
   return;
}


static void _draw3(DIG *G,int x, int y, int z)
 {
   int i,i2;
   float dx,dy,ct,st,dl;
   int x1,y1,z1,xa,ya,za,xb,yb,zb,dx1,dy1;
   kgDC *dc;
   dc= G->dc;
   xa=dc->cur_x;
   ya=dc->cur_y;
   za=dc->cur_z;
   xb=x;
   yb=y;
   zb=z;
   if ( dc->ln_style == 0) {
     draw_line3(G,xa,ya,za,xb,yb,zb);
   }
   else
    {
     if(dc->ln_width==1) _draw3_(G, x,y,z);
     else
      {
      if( xa>xb){x1=xa,xa=xb,xb=x1,y1=ya,ya=yb,yb=y1;};
      i2=dc->ln_width/2;
      dx = (float)(xb-xa);
      dy = (float)(yb-ya);
      dl = (dx*dx+dy*dy);
      if( dx==0.) {ct=0.0;st=1.;}
      else { dl =sqrt(dl),ct = (dx)/dl, st = (dy)/dl;}
      dx1 =(int)( st*i2+0.5);
      if( ct< 0.) dy1 =(int)( ct*i2-0.5);
      else dy1 =(int)( ct*i2+0.5);
      xa +=dx1,xb +=dx1,ya -=dy1,yb -=dy1;
      dc->cur_x=xa,dc->cur_y=ya; dc->cur_z = za;
      _draw3_(G, xb, yb,zb);
      for (i=1;i<dc->ln_width;i++)
        {
             dx1 =(int)( st*i+0.5);
             if( ct< 0.) dy1 =(int)( ct*i-0.5);
             else dy1 =(int)( ct*i+0.5);
             dc->cur_x=xa-dx1,dc->cur_y=ya+dy1;
            _draw3_(G, xb-dx1,yb+dy1,zb);
        }
      }
    }
   dc->cur_x=x;
   dc->cur_y=y;
   dc->cur_z=z;
   return;
 }

static  void t_3_draw(DIG *G,float ix,float iy,float iz)
{
  int x1,y1,z1;
  kgDC *dc;
  dc= G->dc;
  x1=scr_x(ix);
  y1=scr_y(iy);
  z1=scr_z(iz);
  _draw3(G,x1,y1,z1);
}
void img_draw3f(DIG *G,float x,float y,float z)
{
    t_3_draw(G,x,y,z);
}

static void _poly_fill3(DIG *G,short n,int *px,int *py,int *pz,short flag,unsigned int b_color)
{
  short j,imd;
  short i,cnt,ind,flg;
  int xcord[15];
  float zcord[15];
  int xlow,xup;
  int txx;
  int ymax[15],ymin[15],xmax[15],xmin[15],zmax[15],zmin[15],
       tyx,tyn,txn,scan,ygrt;
  float  tzn,tzx;
  unsigned int tempc;
  kgDC *dc;
  kgWC *wc;
  dc = G->dc;
  wc = G->wc;
  xup =-1000000;
  xlow = 1000000;
  scan=10000;
  ygrt=-1239000;
  i=0;flg=0;
  while((i<n)&&(flg==0))
  { if ((pz[i++]>0)) flg=1;}
  if (flg==0) return;
  i=0;flg=0;
  while((i<n)&&(flg==0)) {
      if ((pz[i]>= 0)&&(pz[i]<=65000L)) flg=1;
      i++;
  }
  if (flg==0) return;
  for (i=0;i<n;++i)
  {
	imd=(i+1)%n;
	if(py[i]>py[imd])
	{
	  ymax[i]=py[i];
	  xmax[i]=px[i];
	  zmax[i]=pz[i];
	  ymin[i]=py[imd];
	  xmin[i]=px[imd];
	  zmin[i]=pz[imd];
	}
	else
	{
	  ymin[i]=py[i];
	  xmin[i]=px[i];
	  zmin[i]=pz[i];
	  ymax[i]=py[imd];
	  xmax[i]=px[imd];
	  zmax[i]=pz[imd];
	}
	if (ygrt < py[i]) ygrt=py[i];
	if (scan > py[i]) scan=py[i];
	if (xup < px[i]) xup=px[i];
	if (xlow > px[i]) xlow=px[i];
	j=i-1;
	tyx=ymax[i];
	tyn=ymin[i];
	txx=xmax[i];
	txn=xmin[i];
	tzx=zmax[i];
	tzn=zmin[i];
	while (j>=0 && tyx<ymax[j])
	{
	  ymax[j+1]=ymax[j];
	  ymin[j+1]=ymin[j];
	  xmax[j+1]=xmax[j];
	  xmin[j+1]=xmin[j];
	  zmax[j+1]=zmax[j];
	  zmin[j+1]=zmin[j];
	  j--;
	}
	ymax[j+1]=tyx;
	ymin[j+1]=tyn;
	xmax[j+1]=txx;
	xmin[j+1]=txn;
	zmax[j+1]=tzx;
	zmin[j+1]=tzn;
  }
  ind=0;
  if((scan> (dc->EVGAY-1)) || (ygrt< 0) || (xlow> (dc->EVGAX-1)) || (xup< 0)) return;
  tempc = dc->c_color;
  dc->c_color= b_color;
  if (ymax[ind]==scan) ind++;
  while (scan<=ygrt)
  {
	 cnt=0;
	 while (ymax[ind]==scan) ++ind;  /*  excluding upper edges  */
         if(scan<0) {scan++;continue;}
         if(scan>(dc->EVGAY-1)) break;
	 for(i=ind;i<n;++i)
	 {
	  if (ymin[i]<=scan)
	  {
		 if (ymax[i]!=ymin[i])
		 {
                   xcord[cnt]=(((float)(xmax[i]-xmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +xmin[i])+0.5);
                   zcord[cnt]=(((float)(zmax[i]-zmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +zmin[i]));
		  ++cnt;
		 }
	  }
	 }
	 /*sorting scan line intersection xcords of edges*/
	 for (i=0;i<cnt;++i)
	 {
		j=i-1;
		txx=xcord[i];
		tzx=zcord[i];
		while(j>=0 && txx < xcord[j]){ zcord[j+1]=zcord[j];xcord[j+1]=xcord[j];j--;}
		xcord[j+1]=(txx);
		zcord[j+1]=(tzx);
	 }
	 /*displaying scan lines*/
        
	 for (i=0;i<cnt;i+=2)
           hori_line3(G,xcord[i],xcord[i+1],scan,
                      (int)(zcord[i]+0.5),(int)(zcord[i+1]+0.5));
	 scan++;
  }
  dc->c_color=dc->ln_color;
  if (flag==1)
      for(i=0;i<n;i++) _draw_line3(G,px[i],py[i],pz[i],px[(i+1)%n],py[(i+1)%n],pz[(i+1)%n]);
}



static void _godard_fill3(DIG *G,short n,int *px,int *py,int *pz ,short flag,float *pv)
{
  short j,imd;
  short i,cnt,ind,flg;
  int xcord[15];
  int zcord[15];
  int xlow,xup;
  int txx,tzx,tzn,temp;
  int ymax[15],ymin[15],xmax[15],xmin[15],zmax[15],zmin[15];
  int tyx,tyn,txn,scan,ygrt;
  float pmax[15],pmin[15],pcord[15],tpx,tpn,tpp;
  unsigned char tempc;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  xup =-100000000;
  xlow = 100000000;
  scan=1000000000;
  ygrt=-123900000;
  i=0;flg=0;
  while((i<n)&&(flg==0)) {
      if ((pz[i]>=0)&&(pz[i]<=MAXZ))  flg=1;
      i++;
  }
  if (flg==0) return;
  temp =wc->c_color;
  for (i=0;i<n;++i) {
	imd=(i+1)%n;
	if(py[i]>py[imd])
	{
	  ymax[i]=py[i];
	  xmax[i]=px[i];
          zmax[i]=pz[i];
	  pmax[i]=pv[i];
	  ymin[i]=py[imd];
	  xmin[i]=px[imd];
          zmin[i]=pz[imd];
	  pmin[i]=pv[imd];
	}
	else
	{
	  ymin[i]=py[i];
	  xmin[i]=px[i];
	  zmin[i]=pz[i];
	  pmin[i]=pv[i];
	  ymax[i]=py[imd];
	  xmax[i]=px[imd];
	  zmax[i]=pz[imd];
	  pmax[i]=pv[imd];
	}
	if (ygrt < py[i]) ygrt = py[i];
	if (scan > py[i]) scan = py[i];
	if (xup  < px[i]) xup  = px[i];
	if (xlow > px[i]) xlow = px[i];
	j=i-1;
	tyx=ymax[i];
	tyn=ymin[i];
	txx=xmax[i];
	txn=xmin[i];
	tzx=zmax[i];
	tzn=zmin[i];
	tpx=pmax[i];
	tpn=pmin[i];
	while (j>=0 && tyx<ymax[j]) {
	  ymax[j+1]=ymax[j];
	  ymin[j+1]=ymin[j];
	  xmax[j+1]=xmax[j];
	  xmin[j+1]=xmin[j];
	  zmax[j+1]=zmax[j];
	  zmin[j+1]=zmin[j];
	  pmax[j+1]=pmax[j];
	  pmin[j+1]=pmin[j];
	  j--;
	}
	ymax[j+1]=tyx;
	ymin[j+1]=tyn;
	xmax[j+1]=txx;
	xmin[j+1]=txn;
	zmax[j+1]=tzx;
	zmin[j+1]=tzn;
	pmax[j+1]=tpx;
	pmin[j+1]=tpn;
  }
  ind=0;
  if((scan> wc->c_v_y2)||(ygrt< wc->c_v_y1)||(xlow> wc->c_v_x2)||(xup< wc->c_v_x1)) return; 
  while ((ind<n)&&(ymax[ind]==scan)) ind++;
  if ( scan == ygrt ) { /* ie a single line panel */
    for(i=0;i<n;i++) if(xmax[i]!=xmin[i])break;
    if(i==n) i=0; /* ie for single point panel */
    godr_line3(G,xmin[i],xmax[i],scan,zmin[i],zmax[i],pmin[i],pmax[i]);
    scan++;
  }
  else {
    while (scan<=ygrt) {
	 cnt=0;
         if( scan < ygrt ) {
           while ((ind<n)&&(ymax[ind]==scan)) ++ind; /* excluding upper edges */
         }
         if(scan<wc->c_v_y1) {scan++;continue;}
         if(scan>wc->c_v_y2) break;
	 for(i=ind;i<n;++i) {
	  if (ymin[i]<=scan) {
		 if (ymax[i]!=ymin[i])
		 {
                   xcord[cnt]=(((float)(xmax[i]-xmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +xmin[i])+0.5);
                   zcord[cnt]=(((float)(zmax[i]-zmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +zmin[i])+0.5);
                   pcord[cnt]=(((float)(pmax[i]-pmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +pmin[i]));
		  ++cnt;
		 }
	  }
	 }
	 /*sorting scan line intersection xcords of edges*/
	 for (i=0;i<cnt;++i) {
		j=i-1;
		txx=xcord[i];
		tzx=zcord[i];
		tpp=pcord[i];
		while(j>=0 && txx < xcord[j]) {
                     xcord[j+1]=xcord[j];
                     zcord[j+1]=zcord[j];
                     pcord[j+1]=pcord[j];j--;}
		xcord[j+1]=(txx);
		zcord[j+1]=(tzx);
		pcord[j+1]=(tpp);
	 }
	 /*displaying scan lines*/
	 for (i=0;i<cnt;i+=2)
  	 godr_line3(G,xcord[i],xcord[i+1],scan,zcord[i],zcord[i+1],pcord[i],pcord[i+1]);
	 scan++;
    }
  }
  dc->c_color=dc->ln_color;
  if (flag==1)
  for(i=0;i<n;i++) _draw_line3(G,px[i],py[i],pz[i],px[(i+1)%n],py[(i+1)%n],pz[(i+1)%n]);
}
static void godr_line3(DIG *G,int xa,int xb,int y,int za,int zb,float pa,float pb)
{
  short i,pos,p1,p2;
  char * addr;
  char tempc;
  float tempp,pval,pfac,yy;
  short clip;
  int temp,c_color;
  int x1,x2,z1,z2;
  double cons,m;
  int z;
  int EVGAY;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  EVGAY = dc->EVGAY-1;
  yy = EVGAY-y;
  if((y>wc->c_v_y2) || (y<wc->c_v_y1) ) return;
  x1=xa;x2=xb;z1=za;z2=zb;
  if (((za<0)&&(zb<0))||((za>MAXZ)&&(zb>MAXZ))) return;
  clip=clip_line(G,&xa,&y,&xb,&y);
  if (clip==0) return;
  p1 = pa+0.5;
  p2 = pb+0.5;
  if(p1<1) p1=dc->Cutoff;
  if(p2<1) p2=dc->Cutoff;
  if(p1>dc->max_clr) p1=dc->max_clr;
  if(p2>dc->max_clr) p2=dc->max_clr;
  if(xa==xb) { p2=p1; }
  if( p1==p2) {
        tempc=dc->c_color;
        if(p1>= 0) c_color=p1+dc->min_clr;
        else c_color=dc->min_clr;
        dc->c_color=c_color;
        hori_line3(G,xa,xb,y,za,zb);
        dc->c_color=tempc;
        return;
  }
  if (xb<xa) {
    temp=xa;
    xa=xb;
    xb=temp;
    tempp=pa;
    pa=pb;
    pb=tempp;
  }
  if (xa>wc->c_v_x2) return;
  if (xb<wc->c_v_x1) return;
  pfac = 0;
  if(xb != xa) pfac = (pb-pa)/(xb-xa);
  if(xa<wc->c_v_x1) {
       pval = pa+(wc->c_v_x1-xa)*pfac;
       xa= wc->c_v_x1;
       pa= pval;
       if(xb != xa) pfac = (pb-pa)/(xb-xa);
  }
  if(xb>wc->c_v_x2) {
       pval = pa+(wc->c_v_x2-xa)*pfac;
       xb= wc->c_v_x2;
       pb= pval;
       if(xb != xa) pfac = (pb-pa)/(xb-xa);
  }
  m = (double)(z2-z1)/(double)(x2-x1);
  cons = z1 - x1*m;
  za=z_inter(xa,m,cons);zb=z_inter(xb,m,cons);
  for(i=xa;i<=xb;i++) {
    temp=(char)(pa+(i-xa)*pfac+0.5);
    if(temp<1) temp=dc->Cutoff;
    if(temp>dc->max_clr) temp=dc->max_clr;
    if(temp >= 0 ) temp +=dc->min_clr;
    dc->c_color=temp;
    z=z_inter(i,m,cons);put_pix3(G,i,yy,z);
  }
  dc->c_color=c_color;
}

static void _shade_fill3(DIG *G,short n,int *px,int *py,int *pz ,short flag,float *pv)
{
  short j,imd;
  short i,cnt,ind,flg;
  int xcord[15];
  int zcord[15];
  int xlow,xup;
  int txx,tzx,tzn,temp;
  int ymax[15],ymin[15],xmax[15],xmin[15],zmax[15],zmin[15];
  int tyx,tyn,txn,scan,ygrt;
  float pmax[15],pmin[15],pcord[15],tpx,tpn,tpp;
  unsigned char tempc;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  xup =-100000000;
  xlow = 100000000;
  scan=1000000000;
  ygrt=-123900000;
  i=0;flg=0;
  while((i<n)&&(flg==0)) {
      if ((pz[i]>=0)&&(pz[i]<=MAXZ))  flg=1;
      i++;
  }
  if (flg==0) return;
  temp =dc->c_color;
  for (i=0;i<n;++i) {
	imd=(i+1)%n;
	if(py[i]>py[imd])
	{
	  ymax[i]=py[i];
	  xmax[i]=px[i];
          zmax[i]=pz[i];
	  pmax[i]=pv[i];
	  ymin[i]=py[imd];
	  xmin[i]=px[imd];
          zmin[i]=pz[imd];
	  pmin[i]=pv[imd];
	}
	else
	{
	  ymin[i]=py[i];
	  xmin[i]=px[i];
	  zmin[i]=pz[i];
	  pmin[i]=pv[i];
	  ymax[i]=py[imd];
	  xmax[i]=px[imd];
	  zmax[i]=pz[imd];
	  pmax[i]=pv[imd];
	}
	if (ygrt < py[i]) ygrt = py[i];
	if (scan > py[i]) scan = py[i];
	if (xup  < px[i]) xup  = px[i];
	if (xlow > px[i]) xlow = px[i];
	j=i-1;
	tyx=ymax[i];
	tyn=ymin[i];
	txx=xmax[i];
	txn=xmin[i];
	tzx=zmax[i];
	tzn=zmin[i];
	tpx=pmax[i];
	tpn=pmin[i];
	while (j>=0 && tyx<ymax[j]) {
	  ymax[j+1]=ymax[j];
	  ymin[j+1]=ymin[j];
	  xmax[j+1]=xmax[j];
	  xmin[j+1]=xmin[j];
	  zmax[j+1]=zmax[j];
	  zmin[j+1]=zmin[j];
	  pmax[j+1]=pmax[j];
	  pmin[j+1]=pmin[j];
	  j--;
	}
	ymax[j+1]=tyx;
	ymin[j+1]=tyn;
	xmax[j+1]=txx;
	xmin[j+1]=txn;
	zmax[j+1]=tzx;
	zmin[j+1]=tzn;
	pmax[j+1]=tpx;
	pmin[j+1]=tpn;
  }
  ind=0;
  if((scan> wc->c_v_y2)||(ygrt< wc->c_v_y1)||(xlow> wc->c_v_x2)||(xup< wc->c_v_x1)) return; 
  while ((ind<n)&&(ymax[ind]==scan)) ind++;
  if ( scan == ygrt ) { /* ie a single line panel */
    for(i=0;i<n;i++) if(xmax[i]!=xmin[i])break;
    if(i==n) i=0; /* ie for single point panel */
    shade_line3(G,xmin[i],xmax[i],scan,zmin[i],zmax[i],pmin[i],pmax[i]);
    scan++;
  }
  else {
    while (scan<=ygrt) {
	 cnt=0;
         if( scan < ygrt ) {
           while ((ind<n)&&(ymax[ind]==scan)) ++ind; /* excluding upper edges */
         }
         if(scan<wc->c_v_y1) {scan++;continue;}
         if(scan>wc->c_v_y2) break;
	 for(i=ind;i<n;++i) {
	  if (ymin[i]<=scan) {
		 if (ymax[i]!=ymin[i])
		 {
                   xcord[cnt]=(((float)(xmax[i]-xmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +xmin[i])+0.5);
                   zcord[cnt]=(((float)(zmax[i]-zmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +zmin[i])+0.5);
                   pcord[cnt]=(((float)(pmax[i]-pmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +pmin[i]));
		  ++cnt;
		 }
	  }
	 }
	 /*sorting scan line intersection xcords of edges*/
	 for (i=0;i<cnt;++i) {
		j=i-1;
		txx=xcord[i];
		tzx=zcord[i];
		tpp=pcord[i];
		while(j>=0 && txx < xcord[j]) {
                     xcord[j+1]=xcord[j];
                     zcord[j+1]=zcord[j];
                     pcord[j+1]=pcord[j];j--;}
		xcord[j+1]=(txx);
		zcord[j+1]=(tzx);
		pcord[j+1]=(tpp);
	 }
	 /*displaying scan lines*/
	 for (i=0;i<cnt;i+=2)
  	 shade_line3(G,xcord[i],xcord[i+1],scan,zcord[i],zcord[i+1],pcord[i],pcord[i+1]);
	 scan++;
    }
  }
  dc->c_color = dc->ln_color;
  if (flag==1)
  for(i=0;i<n;i++) _draw_line3(G,px[i],py[i],pz[i],px[(i+1)%n],py[(i+1)%n],pz[(i+1)%n]);
}
static void shade_line3(DIG *G,int xa,int xb,int y,int za,int zb,float pa,float pb)
{
  short i,pos,p1,p2;
  char * addr;
  char tempc;
  float tempp,pval,pfac,yy;
  short clip;
  int temp;
  int x1,x2,z1,z2;
  double cons,m;
  int z;
  int EVGAY;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  EVGAY = dc->EVGAY-1;
  yy = EVGAY-y;
  if((y>wc->c_v_y2) || (y<wc->c_v_y1) ) return;
  x1=xa;x2=xb;z1=za;z2=zb;
  if (((za<0)&&(zb<0))||((za>MAXZ)&&(zb>MAXZ))) return;
  clip=clip_line(G,&xa,&y,&xb,&y);
  if (clip==0) return;
  if(xa==xb) { pa=pb; }
  if (xb<xa) {
    temp=xa;
    xa=xb;
    xb=temp;
    tempp=pa;
    pa=pb;
    pb=tempp;
  }
  if (xa>wc->c_v_x2) return;
  if (xb<wc->c_v_x1) return;
  pfac = 0;
  if(xb != xa) pfac = (pb-pa)/(xb-xa);
  if(xa<wc->c_v_x1) {
       pval = pa+(wc->c_v_x1-xa)*pfac;
       xa= wc->c_v_x1;
       pa= pval;
       if(xb != xa) pfac = (pb-pa)/(xb-xa);
  }
  if(xb>wc->c_v_x2) {
       pval = pa+(wc->c_v_x2-xa)*pfac;
       xb= wc->c_v_x2;
       pb= pval;
       if(xb != xa) pfac = (pb-pa)/(xb-xa);
  }
  if(x2==x1) m = 0;
  else m = (double)(z2-z1)/(double)(x2-x1);
  cons = z1 - x1*m;
  za=z_inter(xa,m,cons);zb=z_inter(xb,m,cons);
  for(i=xa;i<=xb;i++) {
    tempp=(pa+(i-xa)*pfac);
    z=z_inter(i,m,cons);put_shade_pix3(G,i,yy,z,tempp);
  }
}

static void t_panel(DIG *G,float *x,float *y,int color,int flag,int n)
  {
  int x1[1000],y1[1000];
  int xo,yo,xv,yv;
  short i,j=0;
  kgDC *dc;
  dc= G->dc;
  x1[0] =scr_x(x[0]); 
  y1[0] =scr_y(y[0]);
  j++;xo=x1[0],yo=y1[0];
  for(i=1;i<n;i++) 
  {
    xv =scr_x(x[i]); 
    yv =scr_y(y[i]);
    if( (xo!=xv) || (yo!=yv) ){
      x1[j] =xv;
      y1[j] =yv;
      xo=xv,yo=yv;j++;
    }
  }
  if( (j>2)&&(j<500)) _poly_fill(G,(short)j,x1,y1,(short)flag,(unsigned int)color);
}
static void t_3_panel(DIG *G,float *x,float *y,float *z,int color,int flag,int n)
  {
  int x1[1000],y1[1000],z1[1000];
  short i;
  kgDC *dc;
  dc= G->dc;
  for(i=0;i<n;i++) 
  {
    x1[i] =scr_x(x[i]); 
    y1[i] =scr_y(y[i]);
    z1[i] =scr_z(z[i]);
  }
  _poly_fill3(G,n,x1,y1,z1,flag,color);
}
void img_polyfill3f(DIG *G,int n,float *x,float *y,float *z,int flag,int color)
{
     t_3_panel(G,x,y,z,color,flag,n);
}

static void t_box_fill(DIG *G,float x1,float y1,float x2,float y2,int color,int ib)
  {
    int xa,ya,xb,yb;
  kgDC *dc;
  dc= G->dc;
    xa=scr_x(x1);
    ya=scr_y(y1);
    xb=scr_x(x2);
    yb=scr_y(y2);
    _box_fill(G,xa,ya,xb,yb,color);
  }


static void _box_fill3(DIG *G,int x1,int y1,int z1,int x2,int y2,int z2, unsigned int color)
{
  int x_min,y_min,x_max,x_max1,x_min1,y_max,i,j;
  short po1;
  unsigned int temp;
  float cons,m;
  int z;
  int EVGAY;
  kgDC *dc;
  kgWC *wc;
  dc = G->dc;
  wc = G->wc;
  EVGAY = dc->EVGAY-1;
  if (x1 == x2)
   {
    m = (float)(z2-z1)/(float)(y2-y1);
    cons = z1 - y1*m;
   }
  else
   {
    m = (float)(z2-z1)/(float)(x2-x1);
    cons = z1 - x1*m;
   }
  temp=dc->c_color;
  dc->c_color=color;
  if( x1<x2 ) { x_min= x1; x_max1= x2; }
  else{ x_min= x2; x_max1= x1; }
  if(( x_min<=(dc->EVGAX-1)) && (x_max1>=0))
   {
    if( y1<y2 ) { y_min= y1; y_max= y2; }
    else{ y_min= y2; y_max= y1; }
       
    if(( y_min<=(dc->EVGAY-1)) && (y_max>=0))
     {
      if(x_max1>(dc->EVGAX-1)) x_max1 = (dc->EVGAX-1);
      if(x_min<0) x_min = 0;
      if(y_max>(dc->EVGAY-1)) y_max = (dc->EVGAY-1);
      if(y_min<0) y_min = 0;
      for(i=x_min;i<=x_max1;i++) {
        for (j=y_min;j<=y_max;++j)
           {z=z_inter(x_min,m,cons);put_pix3(G,x_min,EVGAY-j,z);}
      }
    }
}
  dc->c_color=temp;
  return;
}


static void t_3_box_fill(DIG *G,float x1,float y1,float z1,float x2,float y2,float z2,int color,int ib)
  {
  int xa,ya,za,xb,yb,zb;
  kgDC *dc;
  dc= G->dc;
  xa=scr_x(x1);
  ya=scr_y(y1);
  za=scr_z(z1);
  xb=scr_x(x2);
  yb=scr_y(y2);
  zb=scr_z(z1);
  _box_fill3(G,xa,ya,za,xb,yb,zb,color);
  }
void img_boxfill3f(DIG *G,float x1,float y1,float z1,float x2,float y2,float z2,int ib,int color)
{
   t_3_box_fill(G,x1,y1,z1,x2,y2,z2,color,ib);
}
static void _godard_fill(DIG *G,short n,int *px,int *py,short flag,float *pv)
{
  short j,imd,tempc;
  short i,cnt,ind;
  int pcrl=0;
  int xcord[15];
  int xlow,xup;
  int txx;
  int ymax[1000],ymin[1000],xmax[1000],xmin[1000],tyx,tyn,txn,scan,ygrt;
  float pmax[1000],pmin[1000],pcord[1000],tpx,tpn,tpp;
  int width=5;
  kgDC *dc;
  kgWC *wc;
  dc = G->dc;
  wc = G->wc;
  xup =-100000;
  xlow = 1000000;
  scan=10000;
  ygrt=-1239000;
  tempc=dc->c_color;
  for (i=0;i<n;++i)

  {
imd=(i+1)%n;
if(py[i]>py[imd])
{
  ymax[i]=py[i];
  xmax[i]=px[i];
  pmax[i]=pv[i];
  ymin[i]=py[imd];
  xmin[i]=px[imd];
  pmin[i]=pv[imd];
}
else
{
  ymin[i]=py[i];
  xmin[i]=px[i];
  pmin[i]=pv[i];
  ymax[i]=py[imd];
  xmax[i]=px[imd];
  pmax[i]=pv[imd];
}
if (ygrt < py[i]) ygrt=py[i];
if (scan > py[i]) scan=py[i];
if (xup < px[i]) xup=px[i];
if (xlow > px[i]) xlow=px[i];
j=i-1;
tyx=ymax[i];
tyn=ymin[i];
txx=xmax[i];
txn=xmin[i];
tpx=pmax[i];
tpn=pmin[i];
while (j>=0 && tyx<ymax[j])
{
  ymax[j+1]=ymax[j];
  ymin[j+1]=ymin[j];
  xmax[j+1]=xmax[j];
  xmin[j+1]=xmin[j];
  pmax[j+1]=pmax[j];
  pmin[j+1]=pmin[j];
  j--;
}
ymax[j+1]=tyx;
ymin[j+1]=tyn;
xmax[j+1]=txx;
xmin[j+1]=txn;
pmax[j+1]=tpx;
pmin[j+1]=tpn;
  }
  ind=0;
  if((scan> wc->c_v_y2) || (ygrt< wc->c_v_y1) || (xlow> wc->c_v_x2) || (xup< wc->c_v_x1)) return;
  if (ymax[ind]==scan) ind++;
  while (scan<=ygrt)
  {
 cnt=0;
 while (ymax[ind]==scan) ++ind;  /*  excluding upper edges  */
         for(i=ind;i<n;++i) {
   if (ymin[i]<=scan) {
 if (ymax[i]!=ymin[i])
 {
                   xcord[cnt]=(((float)(xmax[i]-xmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +xmin[i])+0.5);
                   pcord[cnt]=(((float)(pmax[i]-pmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +pmin[i]));
  ++cnt;
 }
   }
  }
  /*sorting scan line intersection xcords of edges*/
  for (i=0;i<cnt;++i) {
j=i-1;
txx=xcord[i];
tpp=pcord[i];
while(j>=0 && txx < xcord[j])
                   { xcord[j+1]=xcord[j],
                     pcord[j+1]=pcord[j];j--;}
xcord[j+1]=(txx);
pcord[j+1]=(tpp);
  }
  /*displaying scan lines*/
  for (i=0;i<cnt;i+=2)
    godr_line(G,xcord[i],xcord[i+1],scan,pcord[i],pcord[i+1]);
 scan++;
  }
  dc->c_color = dc->ln_color;
  if (flag==1)
      for(i=0;i<n;i++) _draw_line(G,px[i],py[i],px[(i+1)%n],py[(i+1)%n]);
  /*GTW(ln_width);*/
}
static void godr_line(DIG *G,int xa,int xb,int y,float pa,float pb)
{
  int i,pos,p1,p2,yy;
  int temp;
  int tempc,clr,c_color;
  float tempp,pval,pfac;
  int EVGAY;
  kgDC *dc;
  kgWC *wc;
  dc = G->dc;
  wc = G->wc;
  c_color=dc->c_color;
  EVGAY = dc->EVGAY-1;
  if((y>wc->c_v_y2) || (y<wc->c_v_y1) ) return;
  yy = EVGAY-y;
  p1 = pa+0.5;
  p2 = pb+0.5;
  if(p1<1) p1=dc->Cutoff;
  if(p2<1) p2=dc->Cutoff;
  if(p1>dc->max_clr) p1=dc->max_clr;
  if(p2>dc->max_clr) p2=dc->max_clr;
  if( p1==p2)
       {
        if(p1>=0)p1 +=dc->min_clr;
        tempc=dc->c_color;
        dc->c_color = p1;
        hori_line(G,xa,xb,y);
        dc->c_color = tempc;
        return;
       }
  if (xb<xa) {
    temp=xa;
    tempp=pa;
    xa=xb;
    xb=temp;
    pa=pb;
    pb=tempp;
  }
  if( xa < xb) {
    if (xa>wc->c_v_x2) return;
    if (xb<wc->c_v_x1) return;
  }
  else {
    if (xb>wc->c_v_x2) return;
    if (xa<wc->c_v_x1) return;
  }
  pfac = 0;
  if(xb != xa) pfac = (pb-pa)/(xb-xa);
  if(xa<wc->c_v_x1) {
       pval = pa+(wc->c_v_x1-xa)*pfac;
       xa= wc->c_v_x1;
       pa= pval;
  }
  if(xb>wc->c_v_x2) {
       pval = pa+(wc->c_v_x2-xa)*pfac;
       xb= wc->c_v_x2;
       pb= pval;
  }
  for(i=xa;i<=xb;i++) {
    tempc=(int)(pa+(i-xa)*pfac+0.5);
    if(tempc<1) tempc=dc->Cutoff;
    if(tempc>dc->max_clr) tempc=dc->max_clr;
    if(tempc >= 0) tempc +=dc->min_clr;
    dc->c_color =tempc;
    put_pixl(G,i,yy);
  }
  dc->c_color=c_color;
}
static  void t_godr_fill(DIG *G,int n,float *x,float *y,float *p)
 {
  int x1[200],y1[200];
  float p1[200],pv;
  short i;
  kgDC *dc;
  dc= G->dc;
  for(i=0;i<n;i++)
  {
    x1[i] =scr_x(x[i]); 
    y1[i] =scr_y(y[i]);
  }
   for (i=0;i<n;i++) {pv=*(p++), p1[i] = pclr(pv); }
  _godard_fill(G,(short)n,x1,y1,0,p1);
 }
static  void t_3_godr_fill(DIG *G,int n,float *x,float *y,float *z,float *p)
 {
  int x1[1000],y1[1000],z1[1000];
  float p1[1000],pv;
  short i;
  kgDC *dc;
  dc= G->dc;
  for(i=0;i<n;i++)
  {
    x1[i] =scr_x(x[i]); 
    y1[i] =scr_y(y[i]);
    z1[i] =scr_z(z[i]);
  }
   for (i=0;i<n;i++) {pv=*(p++), p1[i] = pclr(pv); }
  _godard_fill3(G,(short)n,x1,y1,z1,(short)dc->ibdr,p1);
 }

void img_godrfill3f(DIG *G,int n,float *x,float *y,float *z,float *v)
{
   t_3_godr_fill(G,n,x,y,z,v);
} 
static int CompPanel(void *pt1,void *pt2){
   LINE *lt1,*lt2;
   lt1 = (LINE *)pt1;
   lt2 = (LINE *)pt2;
   if( (lt2->zmax < lt1->zmax )) return 1;
   else return 0;
}

static void _move(DIG *G,int xp, int yp)
{
  
  kgDC *dc;
  dc = G->dc;
  dc->cur_x=xp ;
  dc->cur_y=yp ;
  return;
}

static void _marker(DIG *G,int x,int y)
{
  short fac=2;
  int px[4],py[4];
  short style,mf;
  kgDC *dc;
  kgWC *wc;
  dc = G->dc;
  wc = G->wc;
  style = dc->m_style%20;
  mf = dc->m_style/20;
  fac +=mf;
  switch(style)
  {
	case 0 :
                 _circle_fill(G,x,y,(int)(3*fac),(unsigned int)15);
                 break;
	case 1 :
                 _draw_line(G,x-3*fac,y-3*fac,x+3*fac,y+3*fac);
                 _draw_line(G,x-3*fac,y+3*fac,x+3*fac,y-3*fac);
                 break;
	case 2 : 
                 _box_fill(G,x-3*fac,y-3*fac,x+3*fac,y+3*fac,(unsigned int)15);
                 break;
	case 3 : _draw_line(G,x,y-4*fac,x,y+4*fac);
				_draw_line(G,x-4*fac,y,x+4*fac,y);
				_draw_line(G,x-2*fac,y-4*fac,x+2*fac,y-4*fac);
				_draw_line(G,x-2*fac,y+4*fac,x+2*fac,y+4*fac);
				_draw_line(G,x-4*fac,y-2*fac,x-4*fac,y+2*fac);
				_draw_line(G,x+4*fac,y-2*fac,x+4*fac,y+2*fac);
				break;
	case 4 : _draw_line(G,x-4*fac,y-4*fac,x+4*fac,y+4*fac);
				_draw_line(G,x-4*fac,y+4*fac,x+4*fac,y-4*fac);
				_draw_line(G,x-4*fac,y,x+4*fac,y);
				_draw_line(G,x,y-4*fac,x,y+4*fac);
				break;
	case 5 : _draw_line(G,x+3*fac,y,x,y-8*fac);
				_draw_line(G,x-3*fac,y,x,y-8*fac);
				_draw_line(G,x+3*fac,y,x,y+8*fac);
				_draw_line(G,x-3*fac,y,x,y+8*fac);
				put_pixel(G,x,y);
				break;
	case 6 : _draw_line(G,x-4*fac,y,x+4*fac,y);
				_draw_line(G,x,y-4*fac,x,y+4*fac);
				_draw_line(G,x+2*fac,y-2*fac,x+2*fac,y+2*fac);
				_draw_line(G,x+2*fac,y+2*fac,x-2*fac,y+2*fac);
				_draw_line(G,x-2*fac,y+2*fac,x-2*fac,y-2*fac);
				_draw_line(G,x-2*fac,y-2*fac,x+2*fac,y-2*fac);
				break;
	case 7 : put_pixel(G,x,y);
				_draw_line(G,x+7*fac,y,x+3*fac,y-3*fac);
				_draw_line(G,x+3*fac,y-3*fac,x,y-7*fac);
				_draw_line(G,x,y-7*fac,x-3*fac,y-3*fac);
				_draw_line(G,x-3*fac,y-3*fac,x-7*fac,y);
				_draw_line(G,x-7*fac,y,x-3*fac,y+3*fac);
				_draw_line(G,x-3*fac,y+3*fac,x,y+7*fac);
				_draw_line(G,x,y+7*fac,x+3*fac,y+3*fac);
				_draw_line(G,x+3*fac,y+3*fac,x+7*fac,y);
				break;
	case 8 : _draw_line(G,x-4*fac,y-4*fac,x+4*fac,y+4*fac);
				_draw_line(G,x+4*fac,y-4*fac,x-4*fac,y+4*fac);
				_draw_line(G,x+4*fac,y+4*fac,x+4*fac,y+2*fac);
				_draw_line(G,x+4*fac,y+4*fac,x+2*fac,y+4*fac);
				_draw_line(G,x+4*fac,y-4*fac,x+4*fac,y-2*fac);
				_draw_line(G,x+4*fac,y-4*fac,x+2*fac,y-4*fac);
				_draw_line(G,x-4*fac,y-4*fac,x-2*fac,y-4*fac);
				_draw_line(G,x-4*fac,y-4*fac,x-4*fac,y-2*fac);
				_draw_line(G,x-4*fac,y+4*fac,x-4*fac,y+2*fac);
				_draw_line(G,x-4*fac,y+4*fac,x-2*fac,y+4*fac);
				break;
        case 9 :
                 px[0]=x+4*fac;py[0]=y;px[1]=x-4*fac;py[1]=y+4*fac;
                 px[2]=x-4*fac;py[2]=y-4*fac;
                 _poly_fill(G,(short)3,px,py,(short)0,(unsigned int)15);
                 break;
        case 10:
                 px[0]=x-4*fac;py[0]=y;px[1]=x+4*fac;py[1]=y+4*fac;
                 px[2]=x+4*fac;py[2]=y-4*fac;
                 _poly_fill(G,(short)3,px,py,(short)0,(unsigned int)15);
                 break;
        case 11:
                 px[0]=x;py[0]=y+4*fac;px[1]=x-4*fac;py[1]=y-4*fac;
                 px[2]=x+4*fac;py[2]=y-4*fac;
                 _poly_fill(G,(short)3,px,py,(short)0,(unsigned int)15);
                 break;
        case 12:
                 px[0]=x;py[0]=y-4*fac;px[1]=x-4*fac;py[1]=y+4*fac;
                 px[2]=x+4*fac;py[2]=y+4*fac;
                 _poly_fill(G,(short)3,px,py,(short)0,(unsigned int)15);
                 break;
	case 13 :
                 _circle_fill(G,x,y,(int)(3*fac),(unsigned int)0);
                 _circle(G,x,y,(int)(3*fac));
                 break;
	case 14 :
                 _circle_fill(G,x,y,(int)(1*fac),(unsigned int)(15));
                 break;
	case 15:
                 _draw_line(G,x-2*fac,y-2*fac,x+2*fac,y+2*fac);
                 _draw_line(G,x-2*fac,y+2*fac,x+2*fac,y-2*fac);
                 break;
        case 16:
                 px[0]=x+2*fac;py[0]=y;px[1]=x-2*fac;py[1]=y+2*fac;
                 px[2]=x-2*fac;py[2]=y-2*fac;
                 _poly_fill(G,(short)3,px,py,(short)0,(unsigned int)15);
                 break;
        case 17:
                 px[0]=x-2*fac;py[0]=y;px[1]=x+2*fac;py[1]=y+2*fac;
                 px[2]=x+2*fac;py[2]=y-2*fac;
                 _poly_fill(G,(short)3,px,py,(short)0,(unsigned int)15);
                 break;
        case 18:
                 px[0]=x;py[0]=y+2*fac;px[1]=x-2*fac;py[1]=y-2*fac;
                 px[2]=x+2*fac;py[2]=y-2*fac;
                 _poly_fill(G,(short)3,px,py,(short)0,(unsigned int)15);
                 break;
        case 19:
                 px[0]=x;py[0]=y-2*fac;px[1]=x-2*fac;py[1]=y+2*fac;
                 px[2]=x+2*fac;py[2]=y+2*fac;
                 _poly_fill(G,(short)3,px,py,(short)0,(unsigned int)15);
                 break;
	default :break;
  }
}

void img_set_color(DIG *G,int color)
{
  kgDC *dc;
  dc = G->dc;
  dc->c_color=color;
  return;
}

void img_set_lnstyle(DIG *G,int style)
{
  kgDC *dc;
  dc = G->dc;
  dc->ln_style=style;
  return;
}
void win_resetstyle(void)
{
 int i;
 for(i=0;i<10;i++) st_ptr[i]=0;
}

static unsigned char code(kgWC *wc,int x, int y)

{
  unsigned char t;
  t = 0;

  if (y > wc->c_v_y2) t|=0x08;
  if (y < wc->c_v_y1) t|=0x04;
  if (x > wc->c_v_x2) t|=0x02;
  if (x < wc->c_v_x1) t++;
  return(t);
}
static short clip_line(DIG *G,int *xa,int *ya, int *xb, int *yb)
{
  int x,y,tx,ty,xs,ys;
  unsigned char c1,c2,c;
  kgWC *wc;
  wc = G->wc;
  xs = wc->c_v_x1;
  ys = wc->c_v_y1;
  c1=code(wc,*xa,*ya);
  c2=code(wc,*xb,*yb);
  if ((c1 & c2) != 0) return(0);
  while ((c1!=0) ||( c2!=0))
  {
     if (c1==0) c=c2;
     else c=c1;
         tx = ((*xb)-(*xa));
         ty = ((*yb)-(*ya));
     if (c & 1)
     {
                if(tx==0) return(0);
                y=(int)(*ya+(wc->c_v_x1-(*xa))*ty/tx);
                if ((y==ys) && ((ys>wc->c_v_y2) || (ys<wc->c_v_y1))) return(0);
                ys=y;
                x=wc->c_v_x1;
                goto jump;
     }
     if (c & 2)
     {
                if(tx==0) return(0);
                y=(int)(*ya+(wc->c_v_x2-(*xa))*ty/tx);
                if ((y==ys) && ((ys>wc->c_v_y2) || (ys<wc->c_v_y1))) return(0);
                ys=y;
                x=wc->c_v_x2;
                goto jump;
     }
     if (c & 8)
     {
                if(ty==0) return(0);
                x=(int)(*xa+(wc->c_v_y2-(*ya))*tx/ty);
                if (x==xs && ((xs<wc->c_v_x1) || (xs>wc->c_v_x2))) return(0);
                xs=x;
                y=wc->c_v_y2;
                goto jump;
     }
     if (c & 4)
     {
                if(ty==0) return(0);
                x=(int)(*xa+(wc->c_v_y1-(*ya))*tx/ty);
                if (x==xs && ((xs<wc->c_v_x1) || (xs>wc->c_v_x2))) return(0);
                xs=x;
                y=wc->c_v_y1;
     }
     jump:
     if (c==c1)
     {
                c1=code(wc,x,y);
                *xa=x;
                *ya=y;
     }
     else
     {
                c2=code(wc,x,y);
                *xb=x;
                *yb=y;
     }
  }
  return(1);
}

       
static void _draw_line(DIG *G,int x1,int y1,int x2,int y2)
{
 char far *addr;
 short i,dx,dy,f,g,inc1,inc2,ab,dx1,dy1,miny,maxy,minx,maxx;
 short clip,ch,rh,r,c;
 unsigned int temp;
 int xa,ya,xb,yb,dum;
 int EVGAY;
 kgDC *dc;
 dc = G->dc;
 EVGAY = dc->EVGAY-1;
 xa =x1,xb=x2,ya=y1,yb=y2;
 if( xa>xb){dum=xa,xa=xb,xb=dum,dum=ya,ya=yb,yb=dum;};
 clip=clip_line(G,&xa,&ya,&xb,&yb);
 if (clip==0) return;
#if 0
 if(G->MAG==1) {
   _uialine(G,(float)xa,(float)ya,(float)xb,(float)yb);
 }
#endif
 if ( (xa!=xb) && (ya!=yb) )
	{
	  dx=xb-xa;
	  dy=yb-ya;
	  dx1=abs(dx);
	  dy1=abs(dy);
	  if ( dx1 > dy1 )
	  {
		 c=xa;
		 r=ya;
		 f=xb;
		 inc1=(dy1<<1);
		 g=inc1-dx1;
		 inc2=g-dx1;
		 if (xa < xb) ch=1;
		 else ch=-1;
		 if (ya < yb) rh=1;
		 else rh=-1;
/*        while (labs(c-f)>0)*/
                 r = EVGAY-r;
                 while ((c!=f))
	          {
	            put_pixl(G,c,r);
                    c+=ch;
	            if (g > 0) { r-=rh; g+=inc2; }
                    else g+=inc1;
	          }
	  }
	  else
	  {
		 c=ya;
		 r=xa;
		 f=yb;
		 inc1=(dx1<<1);
		 g=inc1-dy1;
		 inc2=g-dy1;
		 if (ya < yb) ch=1;
		 else ch=-1;
		 if (xa < xb) rh=1;
		 else rh=-1;
/*        while (labs(c-f)>0)*/
                 c = EVGAY-c;
                 f = EVGAY-f;
                 while ((c!=f))
	         {
         	   put_pixl(G,r,c);
                   c-=ch;
        	   if (g > 0) { r+=rh; g+=inc2; }
         	   else g+=inc1;
	         }
          }
          put_pixl(G,xb,(EVGAY-yb));
  }
  else
     {
	if (xa==xb)
	{
          if ( ya < yb){ miny = ya;maxy = yb;}
          else { miny = yb; maxy = ya;}
  	  for (i=miny;i<=maxy;++i) put_pixl(G,xa,(EVGAY-i));
	}
       else if (ya==yb)
	{
          if ( xa < xb){ minx = xa;maxx = xb;}
          else { minx = xb; maxx = xa;}
          for (i=minx;i<=maxx;++i) put_pixl(G,i,(EVGAY-ya));
	}
     }
	return;
}

static void draw_line(DIG *G,int xa,int ya,int xb,int yb)
 {
   int i,i2;
   float dx,dy,ct,st,dl;
   int x1,y1,x2,y2,dx1,dy1;
   kgDC *dc;
   dc = G->dc;
   if( dc->ln_width <=1 ){ _draw_line(G, xa, ya, xb, yb);return;};
   if( xa>xb){x1=xa,xa=xb,xb=x1,y1=ya,ya=yb,yb=y1;};
   i2=dc->ln_width/2;
   dx = (float)(xb-xa);
   dy = (float)(yb-ya);
   dl = (dx*dx+dy*dy);
   if( dx==0.) {ct=0.0;st=1.;}
   else { dl =sqrt((double)dl),ct = (dx)/dl, st = (dy)/dl;}
   dx1 =(int)( st*i2+0.5);
   if( ct< 0.) dy1 =(int)( ct*i2-0.5);
   else dy1 =(int)( ct*i2+0.5);
   xa +=dx1,xb +=dx1,ya -=dy1,yb -=dy1;
   _draw_line( G,xa, ya, xb, yb);
   for (i=1;i<dc->ln_width;i++)
        {
             dx1 =(int)( st*i+0.5);
             if( ct< 0.) dy1 =(int)( ct*i-0.5);
             else dy1 =(int)( ct*i+0.5);
            _draw_line(G, xa-dx1,ya+dy1,xb-dx1,yb+dy1);
        }
   return;
 }
static void hori_line(DIG *G,int xa,int xb,int y)
{
  short i,clip;
  char far *addr;
  int temp;
  int EVGAY;
   kgDC *dc;
   dc = G->dc;
  if(xa==xb) return;
  EVGAY = dc->EVGAY-1;
  clip=clip_line(G,&xa,&y,&xb,&y);
  if (xb<xa)
  {
    temp=xa;
    xa=xb;
    xb=temp;
  }
  if (clip==0) return;
  for(i=xa;i<=xb;i++) put_pixl(G,i,(EVGAY-y));
}
static void phori_line(DIG *G,int xa,int xb,int y)
{
  short i,j,ip,ib,jp,clip,pt_no;
  char far *addr;
  unsigned char dot;
  int temp;
  int EVGAY;
  kgDC *dc;
  dc = G->dc;
  EVGAY = dc->EVGAY-1;
  if(xa==xb) return;
  clip=clip_line(G,&xa,&y,&xb,&y);
  if (xb<xa)
  {
    temp=xa;
    xa=xb;
    xb=temp;
  }
  if (clip==0) return;
  pt_no=(unsigned int)dc->c_color;
  if(pt_no>15) pt_no=0;
  j=EVGAY-y;
  jp=j%16;
  for(i=xa;i<=xb;i++){
                    /* 
                       ip=i%16;ib=ip/8;ip %=8;
                       dot = fil_pt[pt_no][jp][ib]&MASK[ip];
                       if(dot!=0) put_dot(i,j);
                       else put_blnk(i,j);
                    */
                        put_dot(G,i,j);
                      }
}

static void vert_line(DIG *G,int ya,int yb,int x)
{
  short i,clip;
  char far *addr;
  int temp;
  int EVGAY;
  kgDC *dc;
  dc = G->dc;
  EVGAY = dc->EVGAY-1;
  if(ya==yb) return;
  clip=clip_line(G,&x,&ya,&x,&yb);
  if (yb<ya)
  {
    temp=ya;
    ya=yb;
    yb=temp;
  }
  if (clip==0) return;
  for(i=ya;i<=yb;i++) put_pixl(G,x,(EVGAY-i));
}

static void _draw_(DIG *G,int x, int y)
{
  int xa, ya, xb, yb;
  unsigned int ptn;
  short  dx, dy,  f, inc1, inc2, g,dx1,dy1,ab;
  short clip,i,ch,rh,c,r;
  float h;
  int EVGAY;
  kgDC *dc;
  dc = G->dc;
  EVGAY = dc->EVGAY-1;
  xa=dc->cur_x;
  ya=dc->cur_y;
  xb=x;
  yb=y;
  {
   clip=clip_line(G,&xa,&ya,&xb,&yb);
   if (clip == 0)
   {
	dc->cur_x=x;
	dc->cur_y=y;
        return;
   }
   ptn=ln_ptn[dc->ln_style%10];
   dx=xb-xa;
   dy=yb-ya;
   dx1 = abs(dx);
   dy1 = abs(dy);
   if ( dx1 > dy1 )
   {
    c=xa;
    r=ya;
    f=xb;
    inc1=2*dy1;
    g=inc1-dx1;
    inc2=g-dx1;
    if (xa < xb) ch=1;
    else ch=-1;
    if (ya < yb) rh=1;
    else rh=-1;
   }
   else
   {
    c=ya;
    r=xa;
    f=yb;
    inc1=2*dx1;
    g=inc1-dy1;
    inc2=g-dy1;
    if (ya < yb) ch=1;
    else ch=-1;
    if (xa < xb) rh=1;
    else rh=-1;
   }
   i=dc->st_ptr[dc->ln_style%10];
   while (abs(c-f)>0)
   { 
      i %=32;
      if( (ptn&(((unsigned int)0x80000000)>>i)) !=0){
        if ( dx1 > dy1 ) put_pixl(G,c,(EVGAY-r));
        else   put_pixl(G,r,(EVGAY-c));
      }
      c+=ch;
      if (g > 0) { r+=rh; g+=inc2; }
      else g+=inc1;
      i++;
    }
 }
   dc->st_ptr[dc->ln_style%10]=i;
   return;
}

static void _draw(DIG *G,int x, int y)
 {
   int i,i2,stloc;
   float dx,dy,ct,st,dl;
   int x1,y1,xa,ya,xb,yb,dx1,dy1;
  int EVGAY;
  kgDC *dc;
  dc = G->dc;
  EVGAY = dc->EVGAY-1;
   xa=dc->cur_x;
   ya=dc->cur_y;
   xb=x;
   yb=y;
   if ( dc->ln_style == 0) {_draw_line(G,xa,ya,xb,yb);}
   else
    {
     if(dc->ln_width<=1) _draw_(G, x, y);
     else
      {
      stloc=dc->st_ptr[dc->ln_style%10];
      if( xa>xb){x1=xa,xa=xb,xb=x1,y1=ya,ya=yb,yb=y1;};
      i2=dc->ln_width/2;
      dx = (float)(xb-xa);
      dy = (float)(yb-ya);
      dl = (dx*dx+dy*dy);
      if( dx==0.) {ct=0.0;st=1.;}
      else { dl =sqrt((double)dl),ct = (dx)/dl, st = (dy)/dl;}
      dx1 =(int)( st*i2+0.5);
      if( ct< 0.) dy1 =(int)( ct*i2-0.5);
      else dy1 =(int)( ct*i2+0.5);
      xa +=dx1,xb +=dx1,ya -=dy1,yb -=dy1;
      dc->cur_x=xa,dc->cur_y=ya;
      _draw_( G,xb, yb);
      for (i=1;i<dc->ln_width;i++)
        {
             dc->st_ptr[dc->ln_style%10]=stloc;
             dx1 =(int)( st*i+0.5);
             if( ct< 0.) dy1 =(int)( ct*i-0.5);
             else dy1 =(int)( ct*i+0.5);
             dc->cur_x=xa-dx1,dc->cur_y=ya+dy1;
            _draw_(G, xb-dx1,yb+dy1);
        }
      }
    }
   dc->cur_x=x;
   dc->cur_y=y;
   return;
 }

static void uidrawg0ch(DIG *G,char c)
{
#if 1
  float x[300],y[300];
  int x1[5000],y1[5000],x2[5000],y2[5000];
  int code,n,i,xdum,ydum,pnts;
  short ptr;
  short j,pnt,asc,tempc;
  float fact=1.0;
  int xo,yo,temp;
  unsigned char color;
  kgDC *dc;
  kgWC *wc;
  dc = G->dc;
  wc = G->wc;
  uiset_greek;
  color=dc->t_color;
  asc=c;
  pnt=asc-32+dc->greek;
  fact = dc->m_f[pnt];
  xo=(int)(dc->cx+(dc->xp));
  yo=(int)(dc->cy+dc->yp);
  if (pnt<0) return;
  if(pnt>223) return;
  ptr= dc->icpos[pnt];
//  if((xo>=c_v_xt1)&&(xo<=c_v_x2)&& (yo>=c_v_yt1)&& (yo<=c_v_y2))
     {
     code= dc->icxv[ptr];n=dc->icyv[ptr];
     pnts=0;
     while ( code >= 0)
      {
        ptr++;
        for(i=0;i<n;i++){
                   xdum=dc->icxv[ptr];ydum=dc->icyv[ptr];ptr++;
                   x[i]=xdum*dc->txt_w42+xo;y[i]=yo+ydum*dc->txt_h42;
        }
        pnts +=t_fillchr(wc,x,y,n,&x1[pnts],&y1[pnts],&x2[pnts],&y2[pnts]);
        code= dc->icxv[ptr];n=dc->icyv[ptr];
        if(code < 0)
         {
          _mpoly_fill(G,pnts,x1,y1,x2,y2,color);
          temp =dc->c_color;dc->c_color=dc->t_color;
          for(i=0;i<pnts;i++) _draw_line(G,x1[i]/Mag,y1[i]/Mag,x2[i]/Mag,y2[i]/Mag);
          dc->c_color=temp;
          pnts=0;
         }
      }
     }
     dc->xp+=(dc->txt_wt*fact+dc->txt_sp);
     uireset_greek;
     return;
#endif
 }
static void uidrawgch(DIG *G,char c)
{
#if 1
  float x[300],y[300];
  int code,n,i;
  int x1[5000],y1[5000],x2[5000],y2[5000];
  int pnts,temp;
  float xdum,ydum;
  short ptr;
  short j,pnt,asc,tempc;
  float xa,ya;
  float fact=1.0;
  float xo,yo;
  unsigned char color;
  kgDC *dc;
  kgWC *wc;
  dc = G->dc;
  wc = G->wc;
  uiset_greek;
  color = dc->t_color;
  asc=c;
  pnt=asc-32+dc->greek;
  fact = dc->m_f[pnt];
  xo=(int)((dc->xp));
  yo=(int)(dc->yp);
  if (pnt<0) return;
  if(pnt>223) return;
  ptr= dc->icpos[pnt];
  xa=(dc->cx+(xo)*dc->cost+yo*dc->sint);
  ya=(dc->cy-(xo)*dc->sint+yo*dc->cost);
 // if((xa>=c_v_xt1)&&(xa<=c_v_x2)&& (ya>=c_v_yt1)&& (ya<=c_v_y2))
     {
     code= dc->icxv[ptr];n=dc->icyv[ptr];
     pnts=0;
     while ( code >= 0)
      {
        ptr++;
        for(i=0;i<n;i++){
          xdum=dc->icxv[ptr];ydum=dc->icyv[ptr];ptr++;
          xdum=(xo+xdum*dc->txt_w42);
          ydum=(yo+ydum*dc->txt_h42);
          x[i]=(dc->cx+(xdum)*dc->cost+(ydum)*dc->sint);
          y[i]=(dc->cy+(ydum)*dc->cost-(xdum)*dc->sint);
        }
        pnts +=t_fillchr(wc,x,y,n,&x1[pnts],&y1[pnts],&x2[pnts],&y2[pnts]);
        code= dc->icxv[ptr];n=dc->icyv[ptr];
        if(code <0) {
          _mpoly_fill(G,pnts,x1,y1,x2,y2,color);
          temp =dc->c_color;dc->c_color=dc->t_color;
          for(i=0;i<pnts;i++) _draw_line(G,x1[i]/Mag,y1[i]/Mag,x2[i]/Mag,y2[i]/Mag);
          dc->c_color=temp;
          pnts=0;
        }
      }
     }
     dc->xp+=(dc->txt_wt*fact+dc->txt_sp);
     uireset_greek;
     return;
#endif
 }


static   void uisetsubsup(DIG *G,float *fact,short *ishft,char cntl)
     {
      L_N *pt;
     kgWC *wc;
     kgDC *dc;
     wc = G->wc;
     dc = G->dc;
      *fact = (*fact)*SSF;
      dc->txt_w42 = SSF*dc->txt_w42;
      dc->txt_h42 = SSF*dc->txt_h42;
      dc->txt_wt = SSF*dc->txt_wt;
      dc->txt_ht = SSF*dc->txt_ht;
      if( cntl=='S'){
                      *ishft = (*ishft)+1;
                      dc->yp = dc->yp+1.4*dc->txt_ht;
                    }
       else dc->yp= dc->yp-0.7*dc->txt_ht;
       pt=dc->O_L;
       while(pt!=NULL){
          if((pt->x2) < 0.){
           if((pt->ymax)< (dc->yp+1.4*dc->txt_ht)) pt->ymax=dc->yp+1.4*dc->txt_ht;
           if((pt->ymin)> (dc->yp-0.4*dc->txt_ht)) pt->ymin=dc->yp-0.4*dc->txt_ht;
          }
          pt=pt->Pr;
       }
      *ishft = (*ishft)*10;
      return;
      }
static   void uiresetsubsup(DIG *G,float *fact,short *ishft)
   {
     short is;
     kgWC *wc;
     kgDC *dc;
     wc = G->wc;
     dc = G->dc;
      *ishft = (*ishft)/10;
      is = *ishft-((*ishft)/10)*10;
      if( is==1){
                      *ishft = (*ishft)-1;
                      dc->yp = dc->yp-1.4*dc->txt_ht;
                }
      else dc->yp= dc->yp+0.7*dc->txt_ht;
      *fact = (*fact)/SSF;
      dc->txt_w42 = dc->txt_w42/SSF;
      dc->txt_h42 = dc->txt_h42/SSF;
      dc->txt_wt = dc->txt_wt/SSF;
      dc->txt_ht = dc->txt_ht/SSF;
      return;
   }


static int find_roots(float a,float b,float c,float *r1,float *r2) {
/* this for special use expecting roots between (0-1) */
    double disc;
    if(a==0.) {
      *r1=0; *r2=0;
    }
    else {
      disc = b*b - 4*a*c;
      if(disc < 0 ){
        *r1=*r2= 0;
      }
      else {
        if(disc == 0 ){
          *r1=*r2= -b/(2.*a);
          if(*r1*(1-*r1)<= 0 ) *r1=*r2=0;
        }
        else {
          disc = sqrt(disc);
          *r1 = (-b+disc)/(2.*a);
          if(*r1*(1-*r1)<= 0 ) *r1=0;
          *r2 = (-b-disc)/(2.*a);
          if(*r2*(1-*r2)<= 0 ) *r2=0;
        }
      }
    }
    return 1;
 
}
static int compute_n(float y1,float y4,float y2,float y3) {
  int n=1;
  if( fabs(y1-y2)*Mag > n) n = fabs(y1 - y2)*Mag;
  if( fabs(y1-y3)*Mag > n) n = fabs(y1 - y3)*Mag;
  if( fabs(y1-y4)*Mag > n) n = fabs(y1 - y4)*Mag;
  if( fabs(y2-y3)*Mag > n) n = fabs(y2 - y3)*Mag;
  if( fabs(y2-y4)*Mag > n) n = fabs(y2 - y4)*Mag;
  if( fabs(y3-y4)*Mag > n) n = fabs(y3 - y4)*Mag;
  if(n>20) n=20;
  return n;
}
static int berzier(int *xp,int *yp,float x1,float y1,float x4,float y4,float x2,
          float y2,float x3,float y3)
   {
     float t,tm,x,y,xm,ym,dist,dt,r1,r2;
     float ax,bx,cx,ay,by,cy;
     int n,notok,i,nx;
     n=2;
     notok=1;
     cx = 3*(x2-x1);
     cy = 3*(y2-y1);
     bx = 3*(x3-x2)-cx;
     by = 3*(y3-y2)-cy;
     ax = x4-x1-cx-bx;
     ay = y4-y1-cy-by;
     find_roots(3*ay,2*by,cy,&r1,&r2);
     t = r1;
     y = ay*t+by;
     y = y*t+cy;
     r1 = y*t+y1;
     t = r2;
     y = ay*t+by;
     y = y*t+cy;
     r2 = y*t+y1;
     n = compute_n(y1,y4,r1,r2);
/*
     while((n<100)&&notok)
     {
       t=1.0/n;
       x = ax*t+bx;
       x = x*t+cx;
       x = x*t+x1;
       y = ay*t+by;
       y = y*t+cy;
       y = y*t+y1;
       dist = (x-x1)*(x-x1)+(y-y1)*(y-y1);
       if(dist>2.0) n++;
       else notok=0;
     }
*/
     t=0;dt=1.0/n;
     for(i=1;i<(n);i++)
     {
       t+=dt;
       x = ax*t+bx;
       x = x*t+cx;
       x = x*t+x1;
       y = ay*t+by;
       y = y*t+cy;
       y = y*t+y1;
      *xp=x*Mag+0.5,*yp=y*Mag+0.5;
      xp++,yp++;
     }
      *xp=x4*Mag+0.5,*yp=y4*Mag+0.5;
      xp++,yp++;
     return(n);
   }
static int t_fillchr(kgWC *wc,float *x,float *y,int n,int *x1,int *y1,int *x2,int*y2)
  {
  int xp,yp,temp;
  float xx,yy,xx0,yy0,xx1,xx2,yy1,yy2,xx3,xx4,yy3,yy4;
  short i,j,j1,k;
  j=0;
  xx0=x[0]; yy0=y[0];
  x1[j] =(xx0)*Mag+0.5; y1[j] =(yy0)*Mag+0.5; j++;
  i=1;
  while(i<n)
  {
    xx =x[i]; yy =y[i]; i++;
    xx1 =x[i]; yy1 =y[i];i++;
    j1=j-1;
    if((xx0==xx1)&&(yy0==yy1))
        {
         xp=(xx*Mag+0.5);yp=(yy*Mag+0.5);
         if((xp!=x1[j1])||(yp!=y1[j1])){x1[j]=xp;y1[j]=yp; j++;};
        }
    else
        {
         xp=(xx*Mag+0.5);yp=(yy*Mag+0.5);
         if((xp!=x1[j1])||(yp!=y1[j1]))
            {
            xx2=(xx1);yy2=(yy1);
            /*xx1=x1[j1];yy1=y1[j1];*/
            xx1=xx0;yy1=yy0;
            xx4=xx,yy4=yy;
            xx3 =(x[i]); yy3 =(y[i]);i++;
            j +=berzier(&x1[j],&y1[j],xx1,yy1,xx4,yy4,xx2,yy2,xx3,yy3);
            if(j>4800) { printf("Not enough buffer\n");}
            }
         else i++;
        }
    xx0=xx;yy0=yy;
  }
   i=1;
   while(i<j)
      { if( (x1[i]==x1[i-1]) &&(y1[i]==y1[i-1]))
          { j--; for(k=i;k<j;k++) {x1[k]=x1[k+1];y1[k]=y1[k+1];}}
        else i++;
      }
   for(i=1;i<j;i++){x2[i-1]=x1[i];y2[i-1]=y1[i];}
   x2[j-1]=x1[0];y2[j-1]=y1[0];
  return(j);
}
static    float get_val(char *ch)
    {
      char chr[4];
      short err;
      float val;
      chr[0]=*ch;chr[1]=*(ch+1);chr[2]=*(ch+2);chr[3]=' ';
      err = sscanf(chr ,"%f",&val);
      if (err<1) return(1.0);
      else return(val);
    }

 void img_txt_wr(DIG *G,int n,char *txt) {
  short i=0,bold,tempc,ishft,trot,Nu,De,gap,lnwidth_o,j;
  int font_o;
  float fact,val,xl1,xl2,hfact=1.0,slant;
  int txt_bold_o;
  L_N *FO_L=NULL,*pt=NULL;
  B_K *FB_P=NULL;
  char ch,cntl;
  unsigned char *tx;
  float Slnt[2]={0.0,0.25},Slant_o;;
  kgDC *dc;
  kgWC *wc;
  dc = G->dc;
  wc = G->wc;
  tx = (unsigned char *) txt;
  j = strlen(tx);
  dc->O_L=NULL;
  bold = dc->txt_bold;
  slant=0;
  font_o=dc->t_font;
  dc->trot = (dc->cost<0.99);
  dc->c_color = dc->t_color;
  dc->cx=(int)(dc->cur_x+0.5);
  dc->cy= (int)(dc->cur_y+0.5);
  dc->xp=0.0;
  dc->yp =0.0;
  fact =1.0;
  hfact =1.0;
  ishft =0;
  dc->greek=0;
  lnwidth_o=dc->ln_width;
  dc->ln_width=1;
  while (txt[i]!='\0') {
        {
         if ( txt[i]!= '!') { if( dc->trot) uidrawgch(G,txt[i]);
                              else uidrawg0ch(G,txt[i]);
                              dc->greek=0;
                            }
         else
           {
           i++;
           if(txt[i]=='\0') break;
           cntl = txt[i];
           if( (cntl=='S')|| (cntl=='s') )
                 uisetsubsup(G,&fact,&ishft,cntl);
           else
             {
               switch (cntl)
                 {
                   case 'e':
                            uiresetsubsup(G,&fact,&ishft);
                            break;
                   case '!':
                            dc->greek=0;
                            if(dc->trot)  uidrawgch(G,txt[i]);
                            else uidrawg0ch(G,txt[i]);
                            break;
                   case 'b':
                            dc->xp =dc->xp -dc->txt_wt -dc->txt_sp;
                            break;
                   case 'B':
                            dc->txt_bold=2;
                            break;
                   case 'I':
                            dc->Slant=Slnt[1];
                            break;
                   case 'N':
                            dc->Slant = Slant_o; dc->txt_bold=txt_bold_o;
                            if(pt!=NULL){
                             while( (pt->x2) >=0.) pt=pt->Pr;
                             (pt->x2)=dc->xp-dc->txt_sp;
                             pt=pt->Pr;
                            }
                            break;
                   case 'g':
                            dc->greek=128;
                            break;
                   case 'r':
                            if(dc->O_P!=NULL){
                              dc->xp=dc->O_P->x;
                              dc->yp=dc->O_P->y;
                              dc->D_P=dc->O_P;
                              dc->O_P=dc->O_P->Pr;
                              free(dc->D_P);
                              if(dc->O_P==NULL) FB_P=NULL;
                            }
                            break;
                   case 'k':
                            if(FB_P==NULL) {
                              FB_P=(B_K *) malloc((int)sizeof(B_K));
                              dc->O_P=FB_P;
                              dc->O_P->Nx=NULL;dc->O_P->Pr=NULL;
                            }
                            else {
                              dc->O_P->Nx=(B_K *) malloc((int)sizeof(B_K));
                              dc->O_P->Nx->Pr=dc->O_P;
                              dc->O_P=dc->O_P->Nx;
                              dc->O_P->Nx=NULL;
                            }
                            (dc->O_P->x) = dc->xp;
                            (dc->O_P->y) = dc->yp;
                            break;
                   case 'x':
                            dc->yp +=0.2*dc->txt_ht;
                            break;
                   case 'y':
                            dc->yp -=0.2*dc->txt_ht;
                            break;
                   case 'u':
                            dc->yp +=0.9*dc->txt_ht;
                            break;
                   case 'd':
                            dc->yp -=0.9*dc->txt_ht;
                            break;
                   case 'O':
                   case 'U':
                            if(FO_L==NULL) {
                              FO_L=(L_N *) malloc((int)sizeof(L_N));
                              dc->O_L=FO_L;
                              dc->O_L->Nx=NULL;dc->O_L->Pr=NULL;
                            }
                            else {
                              dc->O_L->Nx=(L_N *) malloc((int)sizeof(L_N));
                              dc->O_L->Nx->Pr=dc->O_L;
                              dc->O_L=dc->O_L->Nx;
                              dc->O_L->Nx=NULL;
                            }
                            (dc->O_L->x1) = dc->xp;
                            (dc->O_L->x2) = -1.0;
                            dc->O_L->ymax = dc->yp+1.4*dc->txt_ht;
                            dc->O_L->ymin = dc->yp-0.4*dc->txt_ht;
                            dc->O_L->p=cntl;
                            pt=dc->O_L;
                            break;
                   case '%':
                            if(i+2 >= j) break;
                            ch=txt[i+1];txt[i+1]='\0';
                            gap=uistrlngth(G,txt,&xl2);
                            txt[i+1]=ch;
                            dc->xp = (gap+1)*(dc->txt_wt+dc->txt_sp);
                            break;
                   case 'z':
                            if(i+2 >= j) break;
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            fact = fact*val;
                            hfact =hfact*val;
                            i +=2;
                            dc->txt_w42 = dc->txt_w42*val;
                            dc->txt_h42 = dc->txt_h42*val;
                            dc->txt_wt = dc->txt_wt*val;
                            dc->txt_ht = dc->txt_ht*val;
                            break;
                   case 'f':
                            if(i+2 >= j) break;
                            Nu= (txt[i+1] -'0')*10+(txt[i+2]-'0');
                            ui_txt_font(G,(int)Nu);
                            i+=2;
                            break;
                   case 'c':
                            if(i+2 >= j) break;
                            Nu= (txt[i+1] -'0')*10+(txt[i+2]-'0');
//                            wcset_clr(wc,Nu);
                            ui_txt_clr(G,(int)Nu);
                            i+=2;
                            break;
                   case 'h':
                            if(i+2 >= j) break;
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            hfact =hfact*val;
                            i +=2;
                            dc->txt_h42 = dc->txt_h42*val;
                            dc->txt_ht = dc->txt_ht*val;
                            break;
                   case 'w':
                            if(i+2 >= j) break;
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            fact = fact*val;
                            i +=2;
                            dc->txt_w42 = dc->txt_w42*val;
                            dc->txt_wt = dc->txt_wt*val;
                            break;
                   default :
                            break;
                 }
             }
         }
        i++;
        }
  }                
      dc->txt_w42 = dc->txt_w42/fact;
      dc->txt_h42 = dc->txt_h42/hfact;
      dc->txt_wt = dc->txt_wt/fact;
      dc->txt_ht = dc->txt_ht/hfact;
/*    txt_bold=bold;*/
      dc->ln_width=2;
      dc->c_color = dc->ln_color;
      pt=FO_L;
      while (pt!=NULL){
       if(pt->x2 <0. ) pt->x2=dc->xp;
       if(pt->p =='U') {
        _move(G,TX((pt->x1),pt->ymin),TY((pt->x1),pt->ymin));
        _draw(G,TX((pt->x2),pt->ymin),TY((pt->x2),pt->ymin));
       }
       else{
        _move(G,TX((pt->x1),pt->ymax),TY((pt->x1),pt->ymax));
        _draw(G,TX((pt->x2),pt->ymax),TY((pt->x2),pt->ymax));
       }
       dc->O_L=pt;
       pt=pt->Nx;
       free(dc->O_L);
      }
      dc->O_P=FB_P;
      while(dc->O_P != NULL) {
       dc->D_P=dc->O_P;
       dc->O_P=dc->O_P->Pr;
       free(dc->D_P);
      }
      dc->ln_width=lnwidth_o;
      if(dc->t_font!=font_o) ui_txt_font(G,font_o);
  }

static void _box_fill(DIG *G,int x1,int y1,int x2,int y2,unsigned int color)
{
  int x_min,y_min,x_max,x_max1,x_min1,y_max,i,j;
  short po1;
  unsigned int temp;
  kgDC *dc;
  kgWC *wc;
  dc= G->dc;
  wc= G->wc;
  temp=dc->c_color;
  dc->c_color=color;
  if( x1<x2 ) { x_min= x1; x_max1= x2; }
  else{ x_min= x2; x_max1= x1; }
  if(( x_min<=wc->c_v_x2) && (x_max1>=wc->c_v_x1))
   {
    if( y1<y2 ) { y_min= y1; y_max= y2; }
    else{ y_min= y2; y_max= y1; }
       
    if(( y_min<=wc->c_v_y2) && (y_max>=wc->c_v_y1))
     {
      if(x_max1>wc->c_v_x2) x_max1 = wc->c_v_x2;
      if(x_min<wc->c_v_x1) x_min = wc->c_v_x1;
      if(y_max>wc->c_v_y2) y_max = wc->c_v_y2;
      if(y_min<wc->c_v_y1) y_min = wc->c_v_y1;
      for (j=y_min;j<=y_max;++j) phori_line(G,x_min,x_max1,j);
    }
   }
  dc->c_color=temp;
  return;
}




static void _poly_fill(DIG *G,short n,int *px,int *py,short flag,unsigned int b_color)
{
  short j,imd;
  short i,cnt,ind;
  int xcord[50];
  int xlow,xup;
  int txx;
  int ymax[500],ymin[500],xmax[500],xmin[500],tyx,tyn,txn,scan,ygrt;
  unsigned int tempc;
  kgDC *dc;
  kgWC *wc;
  dc= G->dc;
  wc= G->wc;
  xup =-100000;
  xlow = 1000000;
  scan=10000;
  ygrt=-1239000;
  for (i=0;i<n;++i)
  {
	imd=(i+1)%n;
	if(py[i]>py[imd])
	{
	  ymax[i]=py[i];
	  xmax[i]=px[i];
	  ymin[i]=py[imd];
	  xmin[i]=px[imd];
	}
	else
	{
	  ymin[i]=py[i];
	  xmin[i]=px[i];
	  ymax[i]=py[imd];
	  xmax[i]=px[imd];
	}
	if (ygrt < py[i]) ygrt=py[i];
	if (scan > py[i]) scan=py[i];
	if (xup < px[i]) xup=px[i];
	if (xlow > px[i]) xlow=px[i];
	j=i-1;
	tyx=ymax[i];
	tyn=ymin[i];
	txx=xmax[i];
	txn=xmin[i];
	while (j>=0 && tyx<ymax[j])
	{
	  ymax[j+1]=ymax[j];
	  ymin[j+1]=ymin[j];
	  xmax[j+1]=xmax[j];
	  xmin[j+1]=xmin[j];
	  j--;
	}
	ymax[j+1]=tyx;
	ymin[j+1]=tyn;
	xmax[j+1]=txx;
	xmin[j+1]=txn;
  }
  ind=0;
  if((scan> wc->c_v_y2) || (ygrt< wc->c_v_y1) || (xlow> wc->c_v_x2) || (xup< wc->c_v_x1)) return;
  tempc = dc->c_color;
  dc->c_color= b_color;
  if (ymax[ind]==scan) ind++;
  while (scan<=ygrt)
  {
	 cnt=0;
	 while (ymax[ind]==scan) ++ind;  /*  excluding upper edges  */
	 for(i=ind;i<n;++i)
	 {
	  if (ymin[i]<=scan)
	  {
		 if (ymax[i]!=ymin[i])
		 {
                   xcord[cnt]=(((float)(xmax[i]-xmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +xmin[i])+0.5);
		  ++cnt;
		 }
	  }
	 }
	 /*sorting scan line intersection xcords of edges*/
	 for (i=0;i<cnt;++i)
	 {
		j=i-1;
		txx=xcord[i];
		while(j>=0 && txx < xcord[j]){ xcord[j+1]=xcord[j];j--;}
		xcord[j+1]=(txx);
	 }
	 /*displaying scan lines*/
	 for (i=0;i<cnt;i+=2)
           phori_line(G,xcord[i],xcord[i+1],scan);
	 scan++;
  }
  dc->c_color = dc->ln_color;
  if (flag==1)
      for(i=0;i<n;i++) _draw_line(G,px[i],py[i],px[(i+1)%n],py[(i+1)%n]);
}

static void _mpoly_fill(DIG *G,short n,int *px,int *py,int *px1,int *py1,unsigned int b_color)
{
  short j,imd;
  short i,cnt,ind;
  int xcord[5000];
  int txx;
  int xlow,xup;
  int ymax[5000],ymin[5000],xmax[5000],xmin[5000],tyx,tyn,txn,scan,ygrt;
  unsigned int tempc;
  kgDC *dc;
  kgWC *wc;
  dc= G->dc;
  wc= G->wc;
  xup =-1000000;
  xlow = 1000000;
  scan=1000000;
  ygrt=-1239000;
  for (i=0;i<n;++i) {
    imd=i;
    if(py[i]>py1[imd])
    {
      ymax[i]=py[i];
      xmax[i]=px[i];
      ymin[i]=py1[imd];
      xmin[i]=px1[imd];
    }
    else
    {
      ymin[i]=py[i];
      xmin[i]=px[i];
      ymax[i]=py1[imd];
      xmax[i]=px1[imd];
    }
    if (ygrt < py[i]) ygrt=py[i];
    if (scan > py[i]) scan=py[i];
    if (xup < px[i]) xup=px[i];
    if (xlow > px[i]) xlow=px[i];
    j=i-1;
    tyx=ymax[i];
    tyn=ymin[i];
    txx=xmax[i];
    txn=xmin[i];
    while (j>=0 && tyx<ymax[j])
    {
      ymax[j+1]=ymax[j];
      ymin[j+1]=ymin[j];
      xmax[j+1]=xmax[j];
      xmin[j+1]=xmin[j];
      j--;
    }
    ymax[j+1]=tyx;
    ymin[j+1]=tyn;
    xmax[j+1]=txx;
    xmin[j+1]=txn;
  }
  ind=0;
  if((scan/Mag> wc->c_v_y2) || (ygrt/Mag< wc->c_v_y1) || (xlow/Mag> wc->c_v_x2) || (xup/Mag< wc->c_v_x1)) return;
  tempc = dc->c_color;
  dc->c_color=b_color;
  if (ymax[ind]==scan) ind++;
  while (scan<=ygrt)
  {
 cnt=0;
 while (ymax[ind]==scan) ++ind;  /*  excluding upper edges  */
 for(i=ind;i<n;++i)
 {
  if (ymin[i]<=scan) {
     if (ymax[i]!=ymin[i]) {
       xcord[cnt]=(((float)(xmax[i]-xmin[i])/(ymax[i]-ymin[i])
                  *(scan-ymin[i]) +xmin[i]+0.5));
       ++cnt;
     }
  }
 }
 /*sorting scan line intersection xcords of edges*/
 for (i=0;i<cnt;++i) {
   j=i-1;
   txx=xcord[i];
   while(j>=0 && txx < xcord[j]){ xcord[j+1]=xcord[j];j--;}
   xcord[j+1]=(txx);
 }
 /*displaying scan lines*/
 for (i=0;i<cnt;i+=2) {
     phori_line(G,(int)(xcord[i]/Mag+0.5),(int)(xcord[i+1]/Mag),
               ((float)scan/Mag+0.5));
 }
 scan++;
 }
 dc->c_color=tempc;
}
static void _circle(DIG *G,int x,int y,int r)
{
  short i,j;
  int ym,sqrd;
  sqrd=r*r;
  ym=y-(short)(r/sqrt((double)2.));
  for (j=y;j>=ym;--j)
  {
	i=x+(short)sqrt((double)sqrd-((y-j)*(y-j)));
	put_pixel(G,i,j);
	put_pixel(G,i,2*y-j);
	put_pixel(G,x+y-j,y+i-x);
	put_pixel(G,x-y+j,y+i-x);
	put_pixel(G,2*x-i,2*y-j);
	put_pixel(G,2*x-i,j);
	put_pixel(G,x-y+j,y-i+x);
	put_pixel(G,x+y-j,y-i+x);
  }
}

static void _circle_fill(DIG *G,int x,int y,int r,unsigned int color)
{
  int i,j;
  int xm,ym;
  double t,sqrd;
  unsigned int temp;
  kgDC *dc;
  dc = G->dc;
  sqrd=(double)r*(double)r;
  ym=y-(short)(r/sqrt((double)2.));
  temp=dc->c_color;
  put_pixel(G,x,y+r),put_pixel(G,x,y-r);
  put_pixel(G,x+r,y),put_pixel(G,x-r,y);
  dc->c_color=color;
  phori_line(G,x-r+1,x+r-1,y);
  for(j=1;j<r;j++)
    {
	t=sqrd-(float)(j*j);
	i=(sqrt((double)t))+0.5;
        dc->c_color=temp;
        put_pixel(G,x-i,y-j);
        put_pixel(G,x+i,y-j);
        put_pixel(G,x-i,y+j);
        put_pixel(G,x+i,y+j);
        dc->c_color=color;
        phori_line(G,x-i+1,x+i-1,y-j);
        phori_line(G,x-i+1,x+i-1,y+j);
    } 
  dc->c_color=temp;
}

static void _rect_fill(DIG *G,short x1,short y1,short x2,short y2,unsigned int color)
{
  int EVGAX,EVGAY;
  short x_min,y_min,x_max,x_max1,x_min1,y_max,i,j,po1;
  unsigned int temp;
  kgDC *dc;
  dc = G->dc;
  temp=dc->c_color;
  dc->c_color=color;
  EVGAX= dc->EVGAX-1;
  EVGAY = dc->EVGAY-1;
  if( x1<x2 ) { x_min= x1; x_max1= x2; }
  else{ x_min= x2; x_max1= x1; }
  if(( x_min<=EVGAX) && (x_max1>=0))
   {
    if( y1<y2 ) { y_min= y1; y_max= y2; }
    else{ y_min= y2; y_max= y1; }
       
    if(( y_min<=EVGAY) && (y_max>=0))
     {
      if(x_max1>EVGAX) x_max1 = EVGAX;
      if(x_min<0) x_min = 0;
      if(y_max>EVGAY) y_max = EVGAY;
      if(y_min<0) y_min = 0;
      for (j=y_min;j<=y_max;++j) phori_line(G,x_min,x_max1,j);
    }
}
  dc->c_color=temp;
  return;
}
static int  uistrlngth_loc(kgDC *dc,char *title,float *xdsp)
    {
      float wd,gp,fj,fjl,gj,val,fact,fact1=1.0,hfact=1.0;
      short ngp,n,i,j,k,greek=0;
      int font_o;
      B_K *FB_P=NULL;
      font_o=dc->t_font;
      wd = dc->txt_wtx;
      gp = dc->txt_spx;
      ngp =1;
      j =0; while( title[j] != '\0')j++;
      if(j==0){ *xdsp=0.;return(0);}
      fj =0.0; gj = -1; fact = 1.0;
      fjl=0.;
      i=0;
      while(i<j) 
      {
       if( title[i] != '!') {
            if( dc->pr_txt>0) fact1=dc->m_f[title[i]-32+greek]*fact;
            else fact1=1.0;
            gj +=1.;
            fjl +=1.0;
            fj +=fact1;greek=0;
       }
       else { 
             i=i+1; 
             if(i>= j) break;
             switch( title[i])
              {
               case 's':
               case 'S':
                         fact = fact*0.6;
                         break;
                           
               case 'e':
                         fact =fact/0.6;
                         break;
               case 'b':
                         fj = fj-fact1;fjl-=1.; gj=gj-1;
                         break;
               case 'g':
                         greek=128;
                         break;
               case 'r':
                        if(dc->O_P!=NULL){
                          fj=dc->O_P->x;
                          fjl=dc->O_P->xl;
                          gj=dc->O_P->y;
                          dc->D_P=dc->O_P;
                          dc->O_P=dc->O_P->Pr;
                          free(dc->D_P);
                          if(dc->O_P==NULL) FB_P=NULL;
                        }
                        break;
               case 'k':
                        if(FB_P==NULL) {
                          FB_P=(B_K *) malloc((int)sizeof(B_K));
                          dc->O_P=FB_P;
                          dc->O_P->Nx=NULL;dc->O_P->Pr=NULL;
                        }
                        else {
                          dc->O_P->Nx=(B_K *) malloc((int)sizeof(B_K));
                          dc->O_P->Nx->Pr=dc->O_P;
                          dc->O_P=dc->O_P->Nx;
                          dc->O_P->Nx=NULL;
                        }
                        (dc->O_P->x) = fj;
                        (dc->O_P->xl) = fjl;
                        (dc->O_P->y) = gj;
                        break;
               case '!':
                         fj = fj+fact1;fjl+=1.; gj = gj+1;
                         break;
               case '%':
                         fj = fjl;
                         break;
               case 'f':
                         if(i+2 >= j) break;
                         val = title[i+1]-'0';
                         val =  val*10+( title[i+2]-'0');
                         i+=2;
                         /*gj = gj-2;*/
                         break;
               case 'z':
                         if(i+2 >= j) break;
                         val = title[i+2]-'0';
                         if(val == 0.0) val=1.0;
                         val = (float)(title[i+1]-'0')/val;
                         fact = fact*val;
                         i = i+2;
                         /*gj = gj-2;*/
                         break;
               case 'h':
                         if(i+2 >= j) break;
                         val = title[i+2]-'0';
                         if(val == 0.0) val=1.0;
                         val = (float)(title[i+1]-'0')/val;
                         hfact = hfact*val;
                         i = i+2;
                         /*gj = gj-2;*/
                         break;
               case 'w':
                         if(i+2 >= j) break;
                         val = title[i+2]-'0';
                         if(val == 0.0) val=1.0;
                         val = (float)(title[i+1]-'0')/val;
                         fact = fact*val;
                         i = i+2;
                         /*gj = gj-2;*/
                         break;
               default :
                         break;
              }
            }
       i = i+1;
      }
      *xdsp =(fj*wd+gj*gp);
      ngp = gj+0.1;
      dc->O_P=FB_P;
      while(dc->O_P != NULL) {
       dc->D_P=dc->O_P;
       dc->O_P=dc->O_P->Pr;
       free(dc->D_P);
      }
      return(ngp);
  }
void img_set_lnwidth(DIG *G,int width)
{
  kgDC *dc;
  dc= G->dc;
  dc->ln_width= width;
  return;
}
void img_godard_fill(DIG *G,int n,float *x,float *y,float *p)
{
  int i;
  int x1[1000],y1[1000];
  float p1[1000];
  int color;
  float pv;
  kgDC *dc;
  dc= G->dc;
  for(i=0;i<n;i++)
  {
    x1[i] =scr_x(x[i]); 
    y1[i] =scr_y(y[i]);
  }
   for (i=0;i<n;i++) {pv=*(p+i), p1[i] = pclr(pv); }
  _godard_fill(G,(short)n,x1,y1,(short)dc->ibdr,p1);
}
static void setpal(kgWC *wc,int r,int g, int b, int i){
    kgColor *kgcolors;
    kgcolors = (kgColor *)(wc->kgcolors);
    kgcolors[i].red=r;
    kgcolors[i].green=g;
    kgcolors[i].blue=b;
    return;
}
void img_chng_clr(DIG *G,int no,int ir,int ig,int ib)
{
  kgWC *wc;
  if(G->D==NULL) wc= G->wc;
  else wc= WC(G->D);
  setpal(wc,(int)ir,(int)ig,(int)ib,(int)no);
}

static void put_pixl(DIG *G,short col, short row)
{
  int addr,MAXB,row1;
  kgWC *wc;
  kgDC *dc;
  dc = G->dc;
//  if(G->D != NULL) wc = WC(G->D);
//  else wc = G->wc; 
  wc = G->wc; 
  MAXB = dc->EVGAX;
  if(col >= MAXB) return;
  row1 = dc->EVGAY-1-row;
  if ((col > wc->c_v_x2) || (col <wc->c_v_x1) || (row1 > wc->c_v_y2) || (row1 <wc->c_v_y1)) return;
  addr = ((row)*MAXB + col);
  imgGetColor(G,dc->c_color,addr);
  return;
}

static void put_dot(DIG *G,short col, short row)
{
  short pos,j,temp;
  int addr,MAXB,row1;
  kgDC *dc;
  kgWC *wc;
  dc = G->dc;
  wc = G->wc;
  MAXB = dc->EVGAX;
  row1 = dc->EVGAY-1-row;
  if ((col > wc->c_v_x2) || (col <wc->c_v_x1) || (row1 > wc->c_v_y2) || (row1 <wc->c_v_y1)) return;
  addr = ((row)*MAXB + (col));
  imgGetColor(G,dc->c_color,addr);
  return;
}
static void put_pixel(DIG *G,int col, int row)
{
  short pos,j,temp;
  int addr,MAXB,EVGAY;
  kgWC *wc;
  kgDC *dc;
  dc = G->dc;
  wc = G->wc;
  MAXB = dc->EVGAX;
  if(col >= MAXB) return;
  EVGAY = dc->EVGAY-1;
  if ((col > wc->c_v_x2) || (col <wc->c_v_x1) || (row > wc->c_v_y2) || (row <wc->c_v_y1)) return;
  addr = (EVGAY-row)*MAXB + (col);
  imgGetColor(G,dc->c_color,addr);
  return;
}
void img_viewport(DIG *G,float x1,float y1,float x2,float y2) {
   float t;
   int EVGAX,EVGAY;
   kgDC *dc;
   kgWC *wc;
   wc = G->wc;
   dc = G->dc;
   dc->vu_x1=x1,dc->vu_y1=y1,dc->vu_x2=x2,dc->vu_y2=y2;
   EVGAX= (G->x2-G->x1+1)*G->MAG+0.1;
   EVGAY= (abs(G->y2-G->y1)+1)*G->MAG+0.1;
   dc->v_x1=x1*(float)EVGAX;
   dc->v_y1= y1*(float)EVGAY;
   dc->v_x2=x2*(float)EVGAX-1+0.1;
   dc->v_y2= y2*(float)EVGAY-1+0.1;
//   if( v_y2 <= 0) {setnormal();exit(0);}
   if( dc->v_y2 <= 0) {printf("dc->v_y2<=0:%d\n",dc->v_y2);exit(0);}
   dc->V_x=dc->v_x2-dc->v_x1;
   dc->V_y=dc->v_y2-dc->v_y1;
   t=(dc->V_x*dc->V_x+dc->V_y*dc->V_y);
   dc->u_x=(dc->V_x/(dc->w_x2-dc->w_x1));
   dc->u_y=(dc->V_y/(dc->w_y2-dc->w_y1));
   dc->v_r=sqrt((double)t);
   dc->EVGAX=dc->v_x2-dc->v_x1+1;
   dc->EVGAY=dc->v_y2-dc->v_y1+1;
   
  uiset_sup_clip_limits(wc,0,0,dc->EVGAX-1,dc->EVGAY-1);
  uiset_clip_limits(wc,0,0,dc->EVGAX-1,dc->EVGAY-1);
}
void uiCopyResizedImage(DIG *G) {
  int v_x1,v_y1,v_x2,v_y2,D_x,D_y,V_x,V_y;
  kgDC *dc;
  kgWC *wc;
  DIALOG *D;
  int EVGAY,EVGAX;
  GMIMG *png=NULL;
  png = G->img;
  if(png==NULL) return;
  SyncImagePixels((Image *)(png->image));
  wc = G->wc;
  dc = G->dc;
  EVGAX= G->x2-G->x1+1;
  EVGAY= abs(G->y2-G->y1)+1;
  v_x1=dc->vu_x1*(float)EVGAX;
  v_y1= dc->vu_y1*(float)EVGAY;
  v_x2=dc->vu_x2*(float)EVGAX-1;
  v_y2= dc->vu_y2*(float)EVGAY-1;
  V_x=v_x2-v_x1+1;
  V_y=v_y2-v_y1+1;
  if( G->MAG > 1) {
     if(G->MAG==2) G->rzimg = uiHalfSizegmImage(png);
     else G->rzimg = uiChangeSizegmImage(png,V_x,V_y,1);
     png = G->rzimg;
  }
  else png = G->img;
  if(G->D==NULL) {D_x=D_y=0;}
  else {
    D= G->D;
    D_x=G->x1+D->xo+v_x1;
    D_y = D->yo+G->y1+EVGAY-1-v_y2;
//    printf("Image: %d %d %d %d\n",D_x,D_y,png->image_width,png->image_height);
//    printf("%d %d %d %d %d %d\n",D->xo,D->yo,G->x1,G->y1,G->x2,G->y2);
    kgImage(D,png,D_x,D_y,png->image_width,png->image_height,0.0,1.0);
  }
//  uiWriteImage(G->img,"junk.png");
  if( G->MAG > 1){uiFreeImage(G->rzimg); G->rzimg = NULL;}
}
void *kgGetResizedImage(void *Gtmp) {
  int v_x1,v_y1,v_x2,v_y2,D_x,D_y,V_x,V_y;
  int MAG;
  DIG *G;
  kgDC *dc;
  kgWC *wc;
  DIALOG *D;
  int EVGAY,EVGAX;
  GMIMG *png=NULL,*png1,*png2;
  G = (DIG *) Gtmp;
  png = G->img;
  MAG = G->MAG+0.000001;
  if(png==NULL) return NULL;
  SyncImagePixels((Image *)(png->image));
  wc = G->wc;
  dc = G->dc;
  EVGAX= G->x2-G->x1+1;
  EVGAY= abs(G->y2-G->y1)+1;
  v_x1=dc->vu_x1*(float)EVGAX;
  v_y1= dc->vu_y1*(float)EVGAY;
  v_x2=dc->vu_x2*(float)EVGAX-1;
  v_y2= dc->vu_y2*(float)EVGAY-1;
  V_x=v_x2-v_x1+1;
  V_y=v_y2-v_y1+1;
// G->img will be freed elsewhere if MAG > 1
  if( MAG > 1) {
#if 1
     if(MAG==2) {
       G->rzimg = uiHalfSizegmImage(png);
     }
     else if(MAG==4) {
            png1 =png;
            png  = uiHalfSizegmImage(png1);
            G->rzimg = uiHalfSizegmImage(png);
            kgFreeImage(png);
          }
          else  {
            png  = uiHalfSizegmImage(png);
            G->rzimg = uiChangeSizegmImage(png,V_x,V_y,1);
            kgFreeImage(png);
          }
#else
     G->rzimg = uiChangeSizegmImage(png,V_x,V_y,9);
#endif
     png = G->rzimg;
  }
  else png = G->img;

//  kgImage(D,png,D_x,D_y,png->image_width,png->image_height,0.0,1.0);
//  uiWriteImage(G->img,"junk1.png");
  return (void *) png;
}
void *kgGetSharpImage(void *Gtmp) {
  int v_x1,v_y1,v_x2,v_y2,D_x,D_y,V_x,V_y;
  DIG *G;
  kgDC *dc;
  kgWC *wc;
  DIALOG *D;
  int EVGAY,EVGAX;
  GMIMG *png=NULL;
  G = (DIG *) Gtmp;
  png = G->img;
  if(png==NULL) return NULL;
  SyncImagePixels((Image *)(png->image));
  wc = G->wc;
  dc = G->dc;
  EVGAX= G->x2-G->x1+1;
  EVGAY= abs(G->y2-G->y1)+1;
  v_x1=dc->vu_x1*(float)EVGAX;
  v_y1= dc->vu_y1*(float)EVGAY;
  v_x2=dc->vu_x2*(float)EVGAX-1;
  v_y2= dc->vu_y2*(float)EVGAY-1;
  V_x=v_x2-v_x1+1;
  V_y=v_y2-v_y1+1;
  if( G->MAG > 1) {
     G->rzimg = uiChangeSizegmImage(png,V_x,V_y,4);
     png = G->rzimg;
  }
  else png = G->img;

//  kgImage(D,png,D_x,D_y,png->image_width,png->image_height,0.0,1.0);
//  uiWriteImage(G->img,"junk1.png");
  return (void *) png;
}
void *kgGetSmoothImage(void *Gtmp) {
  int v_x1,v_y1,v_x2,v_y2,D_x,D_y,V_x,V_y;
  DIG *G;
  kgDC *dc;
  kgWC *wc;
  DIALOG *D;
  int EVGAY,EVGAX;
  GMIMG *png=NULL;
  G = (DIG *) Gtmp;
  png = G->img;
  if(png==NULL) return NULL;
  SyncImagePixels((Image *)(png->image));
  wc = G->wc;
  dc = G->dc;
  EVGAX= G->x2-G->x1+1;
  EVGAY= abs(G->y2-G->y1)+1;
  v_x1=dc->vu_x1*(float)EVGAX;
  v_y1= dc->vu_y1*(float)EVGAY;
  v_x2=dc->vu_x2*(float)EVGAX-1;
  v_y2= dc->vu_y2*(float)EVGAY-1;
  V_x=v_x2-v_x1+1;
  V_y=v_y2-v_y1+1;
  if( G->MAG > 1) {
     G->rzimg = uiChangeSizegmImage(png,V_x,V_y,7);
     png = G->rzimg;
  }
  else png = G->img;

//  kgImage(D,png,D_x,D_y,png->image_width,png->image_height,0.0,1.0);
//  uiWriteImage(G->img,"junk1.png");
  return (void *) png;
}

void _img_initialise(DIG *G)
{
  FONT *pt;
  int i,font,count;
  int EVGAX,EVGAY;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc=G->wc;
  EVGAX=G->x2 -G->x1+1;
  EVGAY=G->y2 - G->y1+1;
  for(i=0;i<10;i++)st_ptr[i]=0;
  dc->ln_style=0;
  dc->m_style=0;
  dc->ln_color = 0x0F;
  dc->c_bound =4;
  dc->t_color = 14;
  dc->t_pattern=15;
  dc->t_fill = 1;
  dc->zbuf= NULL;
  dc->redbuf=NULL;
  dc->greenbuf=NULL;
  dc->bluebuf=NULL;
  dc->clrbuf=NULL;
  dc->DOUBLE=0;
  dc->Fontlist= uiGetFontlist();
//  dc->icpos = icposf0;dc->icxv=icxvf0;dc->icyv=icyvf0;dc->m_f=m_f0;
  count = Dcount(dc->Fontlist);
  font =0;
  Dposition(dc->Fontlist,font+1);
  pt = (FONT *)Getrecord(dc->Fontlist);
  dc->icpos = pt->icpos;
  dc->icxv = pt->icxv;
  dc->icyv = pt->icyv;
  dc->m_f =  pt->m_f;
  dc->icposf0 = dc->icpos;dc->icxvf0=dc->icxv;
  dc->icyvf0=dc->icyv;dc->m_f0=dc->m_f;
  dc->t_font=font;
  dc->t_bkgr = 0;
  dc->t_bodr = 0;
  dc->fil_color=0;
  dc->bod_width=11;
  dc->bod_color=1;
  dc->ln_width=1;
  dc->pr_txt=1;
  dc->cost = 1.0;
  dc->sint = 0.0;
  dc->txt_ht=20;
  dc->txt_wt=20;
  dc->txt_htx=20;
  dc->txt_wty=20;
  dc->txt_hty=20;
  dc->txt_wtx=20;
  dc->txt_w42 = dc->txt_wt/CFact;
  dc->txt_h42 = dc->txt_ht/CFact;
  dc->txt_sp=2;
  dc->txt_spy=2;
  dc->txt_spx=2;
  dc->v_x1=0;
  dc->v_y1=0;
  dc->v_x2=EVGAX-1;
  dc->v_y2=EVGAY-1;
  dc->w_x1=0;
  dc->w_y1=0;
  dc->w_x2=1.0;
  dc->w_y2=1.0;
  dc->u_x=EVGAX-1;
  dc->u_y=EVGAY-1;
  dc->V_x=EVGAX-1;
  dc->V_y=EVGAY-1;
  dc->D_x=0;
  dc->D_y=0;
  dc->ZBUFF=dc->SET3D=dc->TIFF=dc->PROJ=0;
  dc->DBL_BUF=0;
  dc->D_ON=0;
  dc->Cutoff=1;
  wc->Clip = Dopen();
  wc->SBlist=Dopen();
  dc->EVGAX=EVGAX;
  dc->EVGAY=EVGAY;
  uiset_sup_clip_limits(wc,0,0,dc->EVGAX-1,dc->EVGAY-1);
  uiset_clip_limits(wc,0,0,dc->EVGAX-1,dc->EVGAY-1);
  for(i=0;i<10;i++) {
    dc->ln_ptn[i]=ln_ptn[i];
    dc->st_ptr[i]=st_ptr[i];
  }
}
void *kgInitImage(int width,int height,int mag) {
  int l,i;
  static entry=0;
  char flname[200],reviewfile[200];

  DIG *G;
  kgDC *dc;
  kgWC *wc;
  G = (DIG *) malloc(sizeof(DIG));
  G->D=NULL;
  dc = (kgDC *) malloc(sizeof(kgDC));
  G->wc = (kgWC *)malloc(sizeof(kgWC));
  wc = G->wc;
  G->dc = dc;
  G->x1=G->y1=0;
  G->x2 = (int)(width-1);
  G->y2 = (int)(height-1);
  G->MAG=mag;
  _img_initialise(G);
  wc->kgcolors=(kgColor *) malloc(sizeof(kgColor)*1024);
  for(i=0;i<1024;i++) {
    wc->kgcolors[i].red = kgIcode[i][0];
    wc->kgcolors[i].green = kgIcode[i][1];
    wc->kgcolors[i].blue = kgIcode[i][2];
  }
  for(l=0;l<10;l++) dc->st_ptr[l]=0;
  G->B_min = 10400;
  G->BACK_UP=0;
  G->OPEN=1;
  G->Byte=0;
  G->R_Byte=0;
  G->R_max=0;
  G->D_ON=0;
  G->img=NULL;
  G->rzimg=NULL;
  G->pixels=NULL;
  kgViewport(G,0.,0.,1.0,1.0);
  kgUserFrame(G,0.,0.,(float)(width-1),(float)(height-1));
  dc->ls_list=NULL;
  dc->No_of_lights=0;
  G->Rbuff = NULL;
  return G;
}
void kgCloseImage(void *Gtmp) {
  DIG *G;
  kgDC *dc;
  kgWC *wc;
  G= (DIG *)Gtmp;
  if(G == NULL) return;
  if(G->D != NULL) return;
  dc = G->dc;
  wc = G->wc;
#if 1
//   printf("G->MAG = %d\n",G->MAG);
  if((G->MAG > 1)&&(G->img != NULL)) {
       uiFreeImage(G->img);
  }
//  if(G->rzimg != NULL) uiFreeImage(G->rzimg); //user should take care of this after use
  Dempty((Dlink *)(wc->Clip));
  Dempty((Dlink *)(wc->SBlist));
  Dfree((Dlink *)(dc->Fontlist));
  free(wc->kgcolors);
  free(G->dc);
  free(G->wc);
  free(Gtmp);
#endif
}
void *kgInitGph(int width,int height) {
  int mag = 1;
  int l,i;
  static entry=0;
  char flname[200],reviewfile[200];

  DIG *G;
  kgDC *dc;
  kgWC *wc;
  char *dir;
  G = (DIG *) malloc(sizeof(DIG));
  G->D=NULL;
  dc = (kgDC *) malloc(sizeof(kgDC));
  G->wc = (kgWC *)malloc(sizeof(kgWC));
  wc = G->wc;
  G->dc = dc;
  G->x1=G->y1=0;
  G->x2 = (int)(width-1);
  G->y2 = (int)(height-1);
  G->MAG=mag;
  _img_initialise(G);
  wc->kgcolors=(kgColor *) malloc(sizeof(kgColor)*1024);
  for(i=0;i<1024;i++) {
    wc->kgcolors[i].red = kgIcode[i][0];
    wc->kgcolors[i].green = kgIcode[i][1];
    wc->kgcolors[i].blue = kgIcode[i][2];
  }
  G->Obj_opn=0;
  for(l=0;l<10;l++) dc->st_ptr[l]=0;
  G->B_min = 10400;
  G->BACK_UP=0;
  G->OPEN=1;
  G->Byte=0;
  G->R_Byte=0;
  G->R_max=0;
  G->D_ON=0;
  G->img=NULL;
  G->rzimg=NULL;
  G->pixels=NULL;
  kgViewport(G,0.,0.,1.0,1.0);
  kgUserFrame(G,0.,0.,(float)(width-1),(float)(height-1));
  dir= kgMakeTmpDir();
  strcpy(flname,dir);
  free(dir);
  strcpy(dc->objdir,flname);
  entry++;
  strcpy(dc->reviewfile,dc->objdir);
  strcat(dc->reviewfile,"/reviewdat");
  G->rbuf = open(dc->reviewfile,O_CREAT|O_BINARY|O_TRUNC|O_RDWR,0666);
  strcpy(dc->plotfile,dc->objdir);
  strcat(dc->plotfile,"/plotgph");
  strcpy(dc->cmdsfile,dc->objdir);
  strcat(dc->cmdsfile,"/cmdsfile");

  dc->ls_list=NULL;
  dc->No_of_lights=0;
  G->Rbuff = (unsigned char  *) malloc(B_max+100);
  G->hbuf =-1;

  return G;
}
void kgCloseGph(void *Gtmp) {
  DIG *G;
  kgDC *dc;
  kgWC *wc;
  G= (DIG *)Gtmp;
  if(G == NULL) return;
  if(G->D != NULL) return;
  dc = G->dc;
  wc = G->wc;
#if 1
//   printf("G->MAG = %d\n",G->MAG);
  if((G->MAG > 1)&&(G->img != NULL)) {
       uiFreeImage(G->img);
  }
//  if(G->rzimg != NULL) uiFreeImage(G->rzimg); //user should take care of this after use
  free(G->Rbuff);
  kgCleanDir(dc->objdir);
  Dempty((Dlink *)(wc->Clip));
  Dempty((Dlink *)(wc->SBlist));
  Dfree((Dlink *)(dc->Fontlist));
  free(wc->kgcolors);
  free(G->dc);
  free(G->wc);
  free(Gtmp);
#endif
}
 void img_drawimage(DIG *G,void *imgfile,float x1,float y1,float x2,float y2) {
  float fac;
  GMIMG *img,*rzimg;
  char *pt;
  int IMG =1,iw,ih,w,h,temp;
  int X1,Y1,X2,Y2,EVGAY;
  kgDC *dc;
  if(imgfile== NULL) return;
  dc = G->dc;
  EVGAY = dc->EVGAY-1;
  X1=scr_x(x1);
  Y1=EVGAY-scr_y(y1);
  X2=scr_x(x2);
  Y2=EVGAY-scr_y(y2);
  if(Y1>Y2) {temp=Y2; Y2=Y1;Y1=temp;}
  if(X1>X2) {temp=X2; X2=X1;X1=temp;}
  pt = (char *)imgfile;
  if((pt[0]=='#')&&(pt[1]=='#')) { 
    IMG=0; img = uiGetImage((void *)(pt+2));
  }
  else img = imgfile;
  if(img== NULL) return;
  if(strcmp(img->Sign,"FMG")==0){
       FMGIMG *fmg;
       GMIMG *gmimg;
       fmg = (FMGIMG *) imgfile;
       IMG=0;
       img = (GMIMG *)kgGetInlineImage(fmg->image_data,fmg->size);
  }
  if(strcmp(img->Sign,"IMG")!=0) return;
  w = X2-X1+1;
  h = Y2 -Y1+1;
  iw = img->image_width;
  ih = img->image_height;
  rzimg= (GMIMG *)uiChangeSizegmImage(img,w,h,1);
  if(rzimg != NULL) {
//    printf(" Calling imgCopyImage\n");
    imgCopyImage(G,X1,Y1,rzimg);
    uiFreeImage(rzimg);
    if(!IMG) uiFreeImage(img);
  }
 }
