/*
   THis has a different view on Clipping
   But some more work is needed as it not working perfectly

*/
// A4 Size is 8.3inch x 11.7 inch
//#define LEVGAX 10159
//#define LEVGAY 7619
//#define LEVGAX 11882
//#define LEVGAY 8400
/*
#define MONO
*/

#define LEVGAX 10530
#define LEVGAY 7470

#define RS6000
#include <stdio.h>
#include <math.h>
#include <magick/api.h>
#include "kulina.h"
#include "gprivate.h"
#include "gminterface.h"
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
/*#include "fontps.h"*/
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <malloc.h>
#include "dlink.h"
char InFile[500],OutFile[500];
static char TX_HC[200];
typedef struct {
  char *fontname;
  int *icpos;
  int *icxv;
  int *icyv;
  float *m_f;
} FONT;

void *Loadfontstruct(void);
Dlink *uiGetFontlist(void);

static Dlink *Fontlist=NULL;
static float CFact=300000.0;
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
#if 0
typedef struct
    {
      float xstr;
      float ystr;
      float zstr;
    }str1;
#endif
static str1 newstr,trnstr;
static LINE *lt;
static POLY *pt;
static SHADE *st;
static float Cx,Cy,Cz;

#define scr_z(z) (int)((z-clip_min)*CPCONS+0.5);
#define TX(x,y) (cx +(x)*cost+(y)*sint)
#define TY(x,y) (cy -(x)*sint+(y)*cost)
#if 0
typedef struct line {float x1;float x2;int p;float ymax;float ymin;
                     struct line *Pr;struct line *Nx;} L_N;
typedef struct point {float x;float y;float xl;
                     struct point *Pr;struct point *Nx;} B_K;
#endif
static float xe,ye,ze,zc,clip_min=0,clip_max=65000;
static float CPCONS=1.0;
static int ZBUFF=0;
static float FM[16];
static int Entry=0;
static int page=0;
static float Ln_width=6.0;
static L_N *O_L=NULL;
static B_K *O_P=NULL,*D_P;
static float Slant=0.,Slant_o=0.,Slnt[2]={0.,0.25};
static float pattrn[16]={1.,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,
    0.7,0.8,0.9,0.0};
struct {int x,y,w,h;} Clip;
#define NCLRS 256
#define LPU(x,y) fprintf(hbuf," %d %d m\n",(x),(EVGAY-y))
#define LPD(x,y) fprintf(hbuf," %d %d l\n",(x),(EVGAY-y))
#define PPU(x,y) fprintf(hbuf," %d %d m\n",(y),(x))
#define PPD(x,y) fprintf(hbuf," %d %d l\n",(y),(x))
#define PPT(x,y,chr) fprintf(hbuf,"%-d %-d S (%c) W\n",(y),(x),(chr))
#define PCLIP(x,y,w,h) {\
 fprintf(hbuf," %-d %-d %-d %-d rectclip \n",(y),(x),(h),(w));\
}
#define LCLIP(x,y,w,h) {\
 fprintf(hbuf," %-d %-d %-d %-d rectclip \n",(x),(EVGAY-y-h),(w),(h));\
}
#define SETCLIP {\
  if(LSCAPE) fprintf(hbuf," %-d %-d %-d %-d rectclip \n", \
       (Clip.x),(EVGAY-Clip.y-Clip.h),(Clip.w),(Clip.h));\
  else fprintf(hbuf," %-d %-d %-d %-d rectclip \n", \
      (Clip.y),(Clip.x),(Clip.h),(Clip.w));\
}
#define GIN fprintf(TX_F,"ZG\n");
#define GSV fprintf(TX_F,"ZS\n");
#define GRS fprintf(TX_F,"ZR\n");
#define GMV(x,y) fprintf(TX_F,"ZU%d,%d;",(x),(y))
#define GDR(x,y) fprintf(TX_F,"ZD%d,%d;",(x),(y))
#define GCLIP(x,y,w,h) fprintf(TX_F,"ZP%d,%d,%d,%d;",(x),(y),(w),(h));
#define GIM(str,x1,y1,x2,y2) {\
 fprintf(TX_F,"ZM%d,%d,%d,%d,%s\n",(x1),(y1),(x2),(y2),(str));\
}
#define GLN(x,y,xx,yy){\
 fprintf(TX_F,"ZU%d,%d;",(x),(y));\
 fprintf(TX_F,"ZD%d,%d;",(xx),(yy));\
}
#define GFL(x) {\
 fprintf(TX_F,"ZI%d\n",(x));\
}
#define GFP {\
 fprintf(TX_F,"ZX\n");\
}
#define GTW(x) {\
 fprintf(TX_F,"ZC col%3.3d s  n\n",PEN);\
 fprintf(TX_F,"ZW%d\n",(x));\
}
#define GTY(x) {\
 fprintf(TX_F,"ZY%d\n",(x));\
}
#define GTX(x,y,c){\
  fprintf(TX_F,"ZU%d %d;",(x),(y));\
 if(t_fill==1) {\
  if(t_pattern==15) {\
    if((c=='(')||(c==')')||(c=='\\')) \
     fprintf(TX_F,"ZTgs %-6.2f %-6.2f S %-6.1f R (\\%c) W gr\n",\
                 yscale,xscale,t_angle,c);\
     else fprintf(TX_F,"ZTgs %-6.2f %-6.2f S %-6.1f R (%c) W gr\n",\
               yscale,xscale,t_angle,c);\
  }\
  else {\
    if((c=='(')||(c==')')||(c=='\\')) {\
     fprintf(TX_F,"ZTgs %-6.2f %-6.2f S %-6.1f R (\\%c) H col%3.3d fill gr\n",\
                 yscale,xscale,t_angle,c,t_pattern%NCLRS);\
     if(t_bodr) fprintf(TX_F,"ZTgs %-6.2f %-6.2f S %-6.1f R (\\%c) H col%3.3d s gr\n",\
                 yscale,xscale,t_angle,c,t_color%NCLRS);\
    }\
    else {\
       fprintf(TX_F,"ZTgs %-6.2f %-6.2f S %-6.1f R (%c) H col%3.3d fill gr\n",\
                 yscale,xscale,t_angle,c,t_pattern%NCLRS);\
       if(t_bodr) fprintf(TX_F,"ZTgs %-6.2f %-6.2f S %-6.1f R (%c) H col%3.3d s gr\n",\
                 yscale,xscale,t_angle,c,t_color%NCLRS);\
    }\
  }\
 }\
 else if(t_bodr) {\
    if((c=='(')||(c==')')||(c=='\\')) \
     fprintf(TX_F,"ZTgs %-6.2f %-6.2f S %-6.1f R (\\%c) H col%3.3d s gr\n",\
                 yscale,xscale,t_angle,c,t_color%NCLRS);\
     else fprintf(TX_F,"ZTgs %-6.2f %-6.2f S %-6.1f R (%c) H col%3.3d  s gr\n",\
                 yscale,xscale,t_angle,c,t_color%NCLRS);\
 }\
}
/*
#define GPEN(pen) {\
 fprintf(TX_F,"ZC col%3.3d s  n\n",PEN);\
 PEN=pen;\
}
*/
static int SWAP=0;
static unsigned int ln_ptn[10]={0xffffffff,0x88888888, 0xcccccccc, 0xf0f0f0f0,
            0xff18ff18,0xffff0f0f, 0xe7e7e7e7, 0xfafafafa,
            0xffff0000,0xffffff00};
static int st_ptr[10]={0,0,0,0,0,0,0,0,0,0};
typedef struct c_limit{ int cx1;int cy1;int cx2;int cy2;int * pt;} CLIP;
static CLIP *c_loc=NULL;
static int A4=0,LSCAPE=0;
static FILE *hbuf;
static FILE *tmppt;
static FILE *hcptr[17],*TX_F;
static int PEN;
static float  T_rot=0.,t_angle=0.;
static char FontName[30]={"Times-Roman"};

/* For UNIX  */
#define O_BINARY 0x00
/* End For UNIX */
#define PIX 8
#define mask 0xCF
#define rad 0.0174532
#define LN_STYL 0
#define M_STYL 0
#define true 1
#define set_clr(c) c_color=c
#define scr_x(x) (int)((x-w_x1)*u_x+D_x+0.5)
#define scr_y(y) (int)((y-w_y1)*u_y+D_y+0.5)
#define usr_x(x) (float)((x-D_x)/u_x+w_x1)
#define usr_y(y) (float)((y-D_y)/u_y+w_y1)
#define cnv_x(x) (x*fx-sx)
#define cnv_y(y) (y*fy-sy)
#define set_clip_limits(x1,y1,x2,y2,x3,y3) c_v_x1=x1,c_v_y1=y1,c_v_x2=x2,c_v_y2=y2
#define set_sup_clip_limits(x1,y1,x2,y2) s_v_x1=x1,s_v_y1=y1,s_v_x2=x2,s_v_y2=y2
#define pclr(p)  (float)(((p-pmin)*dfac+0.))

#define SCALE 0.7087
static float pmax,pmin,dfac;
static float x_mulf=1.0,y_mulf=1.0;
static unsigned char godr_ptn[13]={0,6,7,9,14,13,12,4,10,2,1,3,15};
static float font_size=188.0,font_size_o=0.;
static float xscale=-1,yscale=1;

static unsigned char MASK[8]={ 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
static unsigned char MASKB[8]={0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };
static unsigned char MASKE[8]={0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF };
static unsigned char RMASK[4]={ 0x00, 0x01, 0x10, 0x11 };
static unsigned char MMASK[4]={ 0x01, 0x02, 0x04, 0x08 };
static unsigned char RED[4]={0x00, 0x04, 0x20, 0x24 };
static unsigned char BLUE[4]={0x00, 0x01, 0x08, 0x09 };
static unsigned char GREEN[4]={0x00, 0x02, 0x10, 0x12 };
static unsigned char *str_ptr;
static unsigned char *addr;
static unsigned char c_color=15,p_color,p_bound,c_bound;
static unsigned char *far_ptr, *cur_mem,*v_mem;
static int cur_x,cur_y,D_x,D_y,lcur_x,lcur_y,rcur_x,rcur_y;
static float V_x=1.0,V_y=1.0;
static short pr_txt=1,txt_bold=1,txt_bold_o=1,ln_width=1,ln_style,m_style,po,jmp;
static float jmpy,ylcur,yrcur;
static float w_x1,w_y1,w_x2,w_y2,v_r,u_x,u_y;
static float txt_ht=200,txt_wt=200,txt_sp=20,r_d,txt_w42,txt_h42;
static double theta;
static float txt_htx=200,txt_wty=200,txt_spy=20;
static float txt_hty=200,txt_wtx=200,txt_spx=20;
static  int v_xt1,v_yt1,v_x1,v_x2,v_y1,v_y2;
static  int c_v_xt1,c_v_yt1,c_v_x1,c_v_x2,c_v_y1,c_v_y2;
static  int b_v_xt1,b_v_yt1,b_v_x1,b_v_x2,b_v_y1,b_v_y2;
static  int s_v_x1,s_v_x2,s_v_y1,s_v_y2;
static int buf,index0;
static int bufl,bufr;
static short rbuf,nmenu;
static short t_color,t_bkgr,t_pattern,t_fill,t_bodr=0,greek=0;
static int *icxv,*icyv;
static int *icpos;
static float *m_f;
static float cost=1.0,sint=0.0;
static int gcur_x,gcur_y;
static float gw_x1,gw_y1,gw_x2,gw_y2;
static short max_clr=64,ibdr=0,min_clr=64;
static short EVGAX=2399;
static short evgay,evgax;
static short MAXB=300;
static short EVGAY=1799;
static float xpos,ypos,txs=0.,txc=1.;
static int t_font,t_font_o=-1;
static float cx,cy,xp,yp;
static void cpy_hdr(void);
static void cpy_hdr1(void);
static void put_pixl(short col, short row);
static void put_dot(short col, short row);
static void put_blnk(short col, short row);
static void hardcopy(void);
static void  getwin(float w[]);
static void  getvu( float v[]);
static void port_file(char flname[]);
static void  back_gr(char flname[]);
static void store_cmds(void);
static void stop_store(void);
static void read_cmds(void);
static void set_godard_clrs(short maxc);
static void clr_view(void);
static void clr_buf(void);
static void move(float x,float y);
static void marker(float x,float y);
static void set_lnwidth( short dw);
static void set_txtbold( short dw);
static void draw(float x,float y);
static void DRAW_LINE(float x1,float y1,float x2,float y2);
static void box_fill(float x1,float y1,float x2,float y2,unsigned char color);
static void win_box_fill(void);
static void poly_fill(short n,float *x,float *y,short flag,unsigned char color);
static void godard_fill(short n,float *px,float *py,short flag,float *pv);
static void win_godard_fill(void);
static void win_poly_fill(void);
static void circle(float x1,float y1,float r);
static void circle_fill(float x1,float y1,float r,unsigned char color);
static void init_graph();
static void initialise();
static void reinitialise();
static void set_graphics(short n);
static void set_mode1();
static void set_mode2();
static void _move(int x,int y);
static void _draw(int x,int y);
static void draw_line(int xa,int ya,int xb,int yb);
static void hori_line(int xa,int xb,int y);
static void phori_line(int xa,int xb,int y);
static void godr_line(int xa,int xb,int y,float pa,float pb);
static void set_color(char color);
static void win_set_godard_clrs(void);
static void win_set_shade_clrs(void);
static void win_set_color(void);
static void put_pixel(int col, int row);
static void put_byte(short col, short row);
static void txt_write(char *txt);
static void txt_incl(char *txt);
static void txtwrt(char *txt);
static void win_txtwrt(void);
static void win_chng_clr(void);
static void txt_color(short color);
static void win_txt_color(void);
static void win_txt_slant(void);
static void win_txt_pattern(void);
static void win_txt_fill(void);
static void win_txt_bodr(void);
static void win_txt_bkgr(void);
static void txt_font(short font);
static void set_txt_font( int font);
static void win_txt_font(void);
static void set_angle(float t);
static void win_set_angle(void);
static void txt_size(void);
static void win_txt_size(void);
static void clear_clip(void);
static void win_clip_limit();
static void win_pop_clip();
static void win_sup_clip_limit();
static void set_lnstyle(short styl);
static short  strlngth(char *title,float *xdsp);
static void win_set_lnstyle(void);
static void user_cord(float x1,float y1,float x2,float y2);
static void set_viewport(int x1,int y1,int x2,int y2);
static void set_markstyle(short style);
static void win_set_markstyle(void);
static void win_set_lnwidth(void);
static void win_set_txtbold(void);
static void _marker (int x,int y);
static void _box_fill(int x1,int y1,int x2,int y2,unsigned char color);
static void _box_fill_ps(int x1,int y1,int x2,int y2,unsigned char color);
static void clear_screen();
static void _poly_fill(short n,int *px,int *py,short flag,unsigned char b_color);
static void _poly_fill_ps(short n,int *px,int *py,short flag,unsigned char b_color);
static void _mpoly_fill(short n,int *px,int *py,int *px1,int *py1,unsigned char b_color);
static void _godard_fill(short n,int *px,int *py,short flag,float *pv);
static void scr_back(int x1,int y1,int x2, int y2, int *mem);
static void scr_recover(int x1,int y1,int x2,int y2, int *mem);
static void pixel_copy(int x1,int y1,int x2,int y2, int x3,int y3);
static void _circle(int x,int y,int r);
static void _circle_fill(int x,int y1,int r,unsigned char color);
static char cursor_move(float *xx,float*yy);
static void cursor_left();
static void cursor_right();
static void cursor_up();
static void cursor_down();
static void draw_cursor();
static void draw_lcur();
static void draw_rcur();
static void win_select(float *x1,float *y1,float *x2,float *y2);
static void  m_intract(float tww,float thh,float tgg);
static short clip_line(int *xa,int *ya,int *xb,int *yb);
static unsigned char code(int x,int y);
static void unclip_files(void);
static short unclip_line(int *xa,int *ya,int *xb,int *yb);
static void win_set_prtxt( );
static void win_move();
static void win_draw();
static void win_marker();
static void win_boxfill();
static void display_on(void);
static void display_off(void);
static void review_cmds(void);
static void write_buf(void *ch, short n);
static void close_file(short rbuf);
static void read_buf(void *ch, short n);
static void write_file(short rbuf);
static void review_file(char *str);
static void read_file(short rbuf);
static void menuch(char c,short ix1,short iy1);
static short review(void);
static void _rect_fill(short x1,short y1,short x2,short y2,unsigned char color);
static void _draw_bound(short x1,short y1,short x2,short y2);
static void _draw_line(int x1,int y1,int x2,int y2);
static void backup_view(void);
static void restore_view(void);
static char *h_short(float *x,float *y);
static void  txt_backup(char *txt);
static void store_view(void);
static void get_view(unsigned char *bf);
static void read_view(unsigned char *bf);
static void pick_box(float *x1,float *y1,float *x2,float *y2);
static void  set_landscape(void);
static void  set_a4copy(void);
static void t_fillchr_o(float *x,float *y,int code,int n);
static int t_fillchr(float *x,float *y,int n,int *x1,int *y1,int *x2,int*y2);
static short GHMAX=0,SCRN,S_OPEN=0;
static char *GHBUF;
static char df_gbuf[20]={"No help available"};
static unsigned char *Rbuff;
static int B_max=31400,B_min=31000,Byte=0,R_max=0,R_Byte=0;
static unsigned char BACK_UP=0;
/*unsigned char gr_buf[5200];*/
static short OPEN=0,ENTRY=0;
static short MODE=18;
static unsigned char * RBUFF;
#define icde(l,m) icode[m-1][l-1]
static unsigned char char_clr=0x0F,fill_clr=0x08,bodr_clr=0x0E,cur_clr=0x09,
            high_clr=0x04;

static short  icode[256][3]= {
255,255,255,   /* whilte */
0,0,255,   /* blue */
0,255,0,   /* green */
60,60,60,
/*125,125,125,*/ 255,0,0,
200,200,200,
90,90,90,
170,170,170,
110,110,110,
160,160,160,
49,101,255,   /* blue1 */
0,255,255,   /* meroon */
255,162,162,   /* gray3 */
216,216,220,
255,255,0,   /* yellow */
0,0,0,   /* black */
220,0,0, 
180,0,0, 
150,0,0, 
120,0,0, 
0,220,0, 
0,180,0, 
0,150,0, 
0,120,0, 
0,0,220,
0,0,180,
0,0,150,
0,0,120,
0,220,220,
0,180,180,
0,150,150,
0,120,120,
220,220,0,
180,180,0,
150,150,0,
120,120,0,
220,0,220,
180,0,180,
150,0,150,
120,0,120,
210,210,210,   /* light */
24,24,113,   /* midnight */
0,0,130,   /* navy */
101,150,239,   /* cornflower */
73,61,138,   /* dark */
105,89,206,   /* slate */
121,105,239,   /* medium */
134,113,255,   /* light */
0,0,206,   /* medium */
65,105,227,   /* royal */
28,146,255,   /* dodger */
0,190,255,   /* deep */
134,206,235,   /* sky */
134,206,251,   /* light */
69,130,182,   /* steel */
178,198,223,   /* light */
174,219,231,   /* light */
178,227,231,   /* powder */
174,239,239,   /* pale */
0,206,210,   /* dark */
73,210,206,   /* medium */
65,227,210,   /* turquoise */
227,255,255,   /* light */
93,158,162,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
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
static unsigned char *buffer;

static unsigned char arr[] ={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
static unsigned char  temp;
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
static unsigned char pix=0xFF;
static unsigned char *dp;
static unsigned short IPO1=0x3CE;
static unsigned short IPO2=0x3CF;
static unsigned short IPO3=0x3C4;
static unsigned short IPO4=0x3C5;
static void GPEN(int pen) {
 fprintf(TX_F,"ZC col%3.3d s  n\n",PEN);
/*
#ifdef MONO
 if (pen != 0) pen=15;
#endif
*/
 PEN=pen;
}
static void transfrm_o(float xp, float yp, float zp)
 {

   trnstr.xstr = (xp * FM[0]) + (yp * FM[4]) + (zp * FM[8]) + FM[12];
   trnstr.ystr = (xp * FM[1]) + (yp * FM[5]) + (zp * FM[9]) + FM[13];
   trnstr.zstr = (xp * FM[2]) + (yp * FM[6]) + (zp * FM[10]) + FM[14];
   return;

 }
static void projection_o(str1 oldstr)
 {

     newstr.xstr = -(2.* ze/(oldstr.zstr)) * oldstr.xstr;
     newstr.ystr = -(2.* ze/(oldstr.zstr)) * oldstr.ystr;
     newstr.zstr = oldstr.zstr;
     return;
 }
static void transfrm(float xp, float yp, float zp)
 {

   trnstr.xstr = xp;
   trnstr.ystr = yp;
   trnstr.zstr = zp;
   return;

 }
static void projection(str1 oldstr)
 {

     newstr.xstr =  oldstr.xstr;
     newstr.ystr =  oldstr.ystr;
     newstr.zstr =  oldstr.zstr;
     return;
 }
static  void setclr( color)

  unsigned char color;
 {
   
   PIXL =color&OFF;
   return;
 }
static    void put_byte(short col,short  row)
      {
        char temp;
        short mnt;
        mnt = (col)>>3;
        temp=*(dp+(row)*MAXB+mnt);
        *(dp+(row)*MAXB+mnt) = c_color;
      }
static       void g_fill(short ix,short iy,char col)
       {
        _rect_fill((short)ix,(short)(iy+15),(short)(ix+11),(short)(iy),col);
       }
static void win_move()
{
  int x1,y1;
  float x,y;
  read_buf(&x,4);
  read_buf(&y,4);
  x1=scr_x(x);
  y1=scr_y(y);
  _move(x1,y1);
}

static void win_draw()
{
  int x1,y1;
  float x,y;
  read_buf(&x,4);
  read_buf(&y,4);
  x1=scr_x(x);
  y1= scr_y(y);
  _draw(x1,y1);
}

static void win_marker()
{
  int x1,y1;
  float x,y;
  read_buf(&x,4);
  read_buf(&y,4);
  x1=scr_x(x);
  y1= scr_y(y);
  _marker(x1,y1);
}
static void DRAW_LINE(float x1,float y1,float x2,float y2)
{
  int xa,ya,xb,yb;
  char ch='l';
  write_buf(&ch,1);
  write_buf(&x1,4);
  write_buf(&y1,4);
  write_buf(&x2,4);
  write_buf(&y2,4);
#if 1
  xa=(int)(x1*u_x+D_x);
  ya=(int)(y1*u_y+D_y);
  xb=(int)(x2*u_x+D_x);
  yb=(int)(y2*u_y+D_y);
#endif
  draw_line(xa,ya,xb,yb);
}


static void win_drawline()
{
  int xa,ya,xb,yb;
  float x1,y1,x2,y2;
  read_buf(&x1,4);
  read_buf(&y1,4);
  read_buf(&x2,4);
  read_buf(&y2,4);
  xa=scr_x(x1);
  ya=scr_y(y1);
  xb=scr_x(x2);
  yb=scr_y(y2);
  DRAW_LINE(xa,ya,xb,yb);
}
#if 1


static int WriteImgData_ci(FILE *fp,void *img) {
  int i,j,w,h,k=0;
  int channels;
  GMIMG *Simg;
  PixelPacket *spixels;
  Simg = (GMIMG *)img; 
  w  = Simg->image_width;
  h  = Simg->image_height;
  channels= Simg->image_channels;
  spixels = GetImagePixels((Image *)(Simg->image),0,0,
           ((Image *)(Simg->image))->columns,
           ((Image *)(Simg->image))->rows);
  fprintf(fp,"  /picstr %-d string def\n",3*w);
  fprintf(fp,"  %d %d 8\n",w,h);
  fprintf(fp,"  [ %d 0 0 -%-d 0 %d ]\n",w,h,h);
  fprintf(fp,"  { currentfile \n");
  fprintf(fp,"    picstr readhexstring pop\n");
  fprintf(fp,"  }\n");
  fprintf(fp,"  false 3\n");
  fprintf(fp,"  colorimage \n");
  k=0;
  for(j=0;j<(h);j++)  {
    for(i=0;i<w;i++) {
       if((channels!=4)||(spixels[k].opacity!=255)) {
         fprintf(fp,"%2.2x%2.2x%2.2x",spixels[k].red,spixels[k].green,
                                     spixels[k].blue);
       }
       else fprintf(fp,"%2.2x%2.2x%2.2x",255,255,255);
       k++;
    }
    fprintf(fp,"\n");
  }
  fprintf(fp,"\n");
  
}
static int WriteImgData_dir(FILE *fp,void *img) {
  int i,j,w,h,k=0;
  int channels;
  GMIMG *Simg;
  PixelPacket *spixels;
  Simg = (GMIMG *)img; 
  w  = Simg->image_width;
  h  = Simg->image_height;
  channels= Simg->image_channels;
  spixels = GetImagePixels((Image *)(Simg->image),0,0,
           ((Image *)(Simg->image))->columns,
           ((Image *)(Simg->image))->rows);
  fprintf(fp,"  /DeviceRGB setcolorspace\n");
  fprintf(fp,"  <<\n");
  if(channels==4) {
    fprintf(fp,"   /ImageType 4\n");
    fprintf(fp,"   /MaskColor [1 2 3]\n");
  }
  else {
    fprintf(fp,"   /ImageType 1\n");
  }
  fprintf(fp,"   /Width %d\n",w);
  fprintf(fp,"   /Height %d\n",h);
  fprintf(fp,"   /BitsPerComponent 8\n");
  fprintf(fp,"   /Decode [0 1 0 1 0 1]\n");
  fprintf(fp,"   /ImageMatrix [%d 0  0 -%-d 0 %d]\n",w,h,h);
  fprintf(fp,"   /DataSource  currentfile /ASCIIHexDecode filter\n");
  fprintf(fp,"  >>\n");
  fprintf(fp,"  image \n");
  k=0;
  for(j=0;j<(h);j++)  {
    for(i=0;i<w;i++) {
       if((channels!=4)||(spixels[k].opacity!=255)) {
         fprintf(fp,"%2.2x%2.2x%2.2x",spixels[k].red,spixels[k].green,
                                     spixels[k].blue);
       }
       else fprintf(fp,"%2.2x%2.2x%2.2x",1,2,3);
       k++;
    }
//    fprintf(fp,"\n");
  }
  fprintf(fp,">\n");
  
}
static int WriteMaskData_o(FILE *fp,void *img) {
  int i,j,w,h,k=0,loc,bitpos;
  int channels;
  unsigned char Mask;
  unsigned char MV[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
  GMIMG *Simg;
  PixelPacket *spixels;
  Simg = (GMIMG *)img; 
  w  = Simg->image_width;
  h  = Simg->image_height;
  channels= Simg->image_channels;
  if(channels==3) return 0;
  spixels = GetImagePixels((Image *)(Simg->image),0,0,
           ((Image *)(Simg->image))->columns,
           ((Image *)(Simg->image))->rows);
  fprintf(fp,"  /maskstr 8 string def\n");
  fprintf(fp,"  %d %d\n",w,h);
  fprintf(fp,"  true\n");
  fprintf(fp,"  [ %d 0 0 -%-d 0 %d ]\n",w,h,h);
  fprintf(fp,"  { currentfile \n");
  fprintf(fp,"    maskstr readhexstring pop\n");
  fprintf(fp,"  }\n");
//  fprintf(fp,"  true 1\n");
  fprintf(fp,"  imagemask \n");
  k=0;
  Mask=0xff;
  for(j=0;j<(h);j++)  {
    for(i=0;i<w;i++) {
       loc = k/8;
       bitpos = k%8;
       if((spixels[k].opacity==255)) {
         Mask &= MV[bitpos];
       }
       if(bitpos==7) {
         fprintf(fp,"%2.2x",Mask);
         Mask=0xff;
       }
       k++;
    }
  }
  fprintf(fp,"\n");
  
}
static int WriteMaskData(FILE *fp,void *img) {
  int i,j,w,h,k=0,loc,bitpos;
  int channels;
  unsigned char Mask;
  unsigned char MV[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
  GMIMG *Simg;
  int count;
  
  PixelPacket *spixels;
  count=0;
  Simg = (GMIMG *)img; 
  w  = Simg->image_width;
  h  = Simg->image_height;
  channels= Simg->image_channels;
  if(channels==3) return 0;
  spixels = GetImagePixels((Image *)(Simg->image),0,0,
           ((Image *)(Simg->image))->columns,
           ((Image *)(Simg->image))->rows);
  fprintf(fp,"  %d %d\n",w,h);
  fprintf(fp,"  false\n");
  fprintf(fp,"  [ %d 0 0 -%-d 0 %d ]\n",w,h,h);
  fprintf(fp,"  {<");
//  fprintf(fp,"  true 1\n");
  k=0;
  Mask=0xff;
  for(j=0;j<(h);j++)  {
    for(i=0;i<w;i++) {
       loc = k/8;
       bitpos = k%8;
       if((spixels[k].opacity==255)) {
         Mask &= MV[bitpos];
       }
       if(bitpos==7) {
         fprintf(fp,"%2.2x",Mask);
//         printf("Mask= %2.2x\n",Mask);
         Mask=0xff;
         count++;
         if(count==64){fprintf(fp,"\n");count=0;}
       }
       k++;
    }
  }
  fprintf(fp,">}\n");
  fprintf(fp,"  imagemask \n");
  
}
#if 0
static void psCopyImage(int x0,int y0,GMIMG *img) {
  GMIMG *Dimg,*Simg;
  PixelPacket *spixels;
  int w,h,iw,ih,i,j,ii,jj,sloc,dloc,cx0,cx1,cy0,cy1;
  dc= G->dc;
  wc =G->wc;
  Simg = (GMIMG *)img;
  Dimg = G->img;
  iw = Dimg->image_width;
  ih = Dimg->image_height; 
  w  = Simg->image_width;
  h  = Simg->image_height;
  cx0=wc->c_v_x1;
  cx1=wc->c_v_x2;
  cy0=dc->EVGAY-1-wc->c_v_y2;
  cy1=dc->EVGAY-1-wc->c_v_y1;
//  printf("%d %d %d %d\n",cx0,cy0,cx1,cy1);
  spixels = GetImagePixels((Image *)(Simg->image),0,0,
           ((Image *)(Simg->image))->columns,
           ((Image *)(Simg->image))->rows);
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
      pixels[dloc]=spixels[sloc];
      pixels[dloc].blue = spixels[sloc].blue;
      pixels[dloc].green = spixels[sloc].green;
      pixels[dloc].red = spixels[sloc].red;
      pixels[dloc].opacity = spixels[sloc].opacity;
    }
  }
  return;
}
#endif
static  void DRAW_IMAGE(void *imgfile,int X1,int Y1,int X2,int Y2) {
  float fac;
  GMIMG *img=NULL,*rzimg;
  char *pt;
  int IMG =1,iw,ih,w,h,temp;
  int xl,yl,xu,yu;
  float lper,rper,tper,bper;
  if(imgfile== NULL) return;
  if(X1>X2) {temp=X2; X2=X1;X1=temp;}
  if(Y1>Y2) {temp=Y2; X2=Y1;Y1=temp;}
  xl=X1,yl=Y1,xu=X2,yu=Y2;
  if(xl < c_v_x1) xl=c_v_x1;
  if(xu < c_v_x1) xu=c_v_x1;
  if(xl > c_v_x2) xl=c_v_x2;
  if(xu > c_v_x2) xu=c_v_x2;
  if(yl < c_v_y1) yl=c_v_y1;
  if(yu < c_v_y1) yu=c_v_y1;
  if(yl > c_v_y2) yl=c_v_y2;
  if(yu > c_v_y2) yu=c_v_y2;
  w = xu -xl+1;
  h = yu -yl+1;
  if((w<=0)||(h<=0)) return;
  lper = (xl-X1)/(float)(X2-X1);
  rper = (X2-xu)/(float)(X2-X1);
  tper = (yl-Y1)/(float)(Y2-Y1);
  bper = (Y2-yu)/(float)(Y2-Y1);
  pt = (char *)imgfile;
  if((pt[0]=='#')&&(pt[1]=='#')) { 
    IMG=0; img = uiGetImage((void *)(pt+2));
  }
  else img = kgGetImage((void *) pt);
  if(img== NULL) return;
  if(strcmp(img->Sign,"IMG")!=0) return;
  iw = img->image_width;
  ih = img->image_height;
  if ( (lper!=0) ||(rper!=0) ||(tper!=0) ||(bper!=0)) {
    int l,r,b,t;
    l= iw*lper+1;r=iw*(1-rper);
    b= ih*bper+1;t=ih*(1-tper);
    rzimg = kgCropImage(img,l,b,r,t);
    if(!IMG) uiFreeImage(img);
    IMG=0;
    img=rzimg;
  }
  if(img != NULL) {
    FILE *fp;
//    psCopyImage(G,X1,Y1,rzimg);
    fp = hbuf;
    fprintf(fp," %f -%-f scale\n",1.0/0.07087,1.0/0.07087);
    fprintf(fp," -90 rotate\n");
//    fprintf(fp," -30  -60 translate\n");
    fprintf(fp,"0.07087 0.07087 scale\n");
    if(LSCAPE) {
         fprintf(fp," %d %d translate\n",EVGAY-yl,xl);
         fprintf(fp,"  %d %d scale\n",(yu-yl),(xu-xl));
    }
    else {
         fprintf(fp," %d %d translate\n",xl,yl);
         fprintf(fp,"  %d %d scale\n",(xu-xl),(yu-yl));
    }
    if(LSCAPE) fprintf(fp,"  90 rotate\n");
//    WriteMaskData(fp,img); // no use
    WriteImgData_dir(fp,img);
    if(LSCAPE) fprintf(fp,"  -90 rotate\n");
    if(LSCAPE) {
      fprintf(fp," %f %f scale\n",1.0/(yu-yl),1.0/(xu-xl));
      fprintf(fp," -%-d -%-d translate\n",EVGAY-yl,xl);
    }
    else {
      fprintf(fp," %f %f scale\n",1.0/(xu-xl),1.0/(yu-yl));
      fprintf(fp," -%-d -%-d translate\n",xl,yl);
    }
    fprintf(fp," %f %f scale\n",1.0/0.07087,1.0/0.07087);
//    fprintf(fp," 30  60 translate\n");
    fprintf(fp," 90 rotate\n");
    fprintf(fp,"0.07087 -0.07087 scale\n");
    if(!IMG) uiFreeImage(img);
  }
 }
#endif
static void win_drawimage()
{
  int xa,ya,xb,yb;
  float x1,y1,x2,y2;
  char buff[100];
  read_buf(buff,100);
  read_buf(&x1,4);
  read_buf(&y1,4);
  read_buf(&x2,4);
  read_buf(&y2,4);
/*
  xa=(int)(x1*u_x+D_x);
  ya=(int)(y1*u_y+D_y);
  xb=(int)(x2*u_x+D_x);
  yb=(int)(y2*u_y+D_y);
*/
  xa=scr_x(x1);
  ya=scr_y(y1);
  xb=scr_x(x2);
  yb=scr_y(y2);
//  DRAW_IMAGE(buff,xa,ya,xb,yb);
  GIM(buff,xa,ya,xb,yb);
  
}

static void win_boxfill()
{
  float x1,y1,x2,y2;
  int color;
  int xa,ya,xb,yb;
  read_buf(&x1,4);
  read_buf(&y1,4);
  read_buf(&x2,4);
  read_buf(&y2,4);
  read_buf(&color,4);
  xa=scr_x(x1);
  ya=scr_y(y1);
  xb=scr_x(x2);
  yb=scr_y(y2);
  if(xa>xb){b_v_x1=xb;b_v_x2=xa;}
  else {b_v_x1=xa;b_v_x2=xb;}
  if(ya>yb){b_v_y1=yb;b_v_y2=ya;}
  else {b_v_y1=ya;b_v_y2=yb;}
  /*unclip_files();
  if(color !=0)_box_fill(xa,ya,xb,yb,(unsigned char)color);
  */
  _box_fill_ps(xa,ya,xb,yb,(unsigned char)color);
  read_buf(&color,4);
}

static win_circle()
{
  float x1,y1,r;
  int xa,ya,rd;
  read_buf(&x1,4);
  read_buf(&y1,4);
  read_buf(&r,4);
  xa=scr_x(x1);
  ya=scr_y(y1);
  rd=scr_x(x1+r);
  rd-=xa;
  _circle(xa,ya,rd);
  return(0);
}

static win_circlefill()
{
  float x1,y1,r;
  unsigned char color;
  int xa,ya,rd;
  read_buf(&x1,4);
  read_buf(&y1,4);
  read_buf(&r,4);
  read_buf(&color,1);
  xa=(int)(x1*u_x+D_x);
  ya=(int)(y1*u_y+D_y);
  rd=(int)(r/r_d*v_r);
  _circle_fill(xa,ya,rd,color);
  return(0);
}



static void win_poly_fill(void)
{
  int n;
  float *x;
  float *y;
  int dx1,dx2,dy1,dy2;
  int flag;
  int color;
  
  int *x1,*y1;
  short i;
  read_buf(&n,4);
  x = (float *)malloc(sizeof(float)*n);
  y = (float *)malloc(sizeof(float)*n);
  x1 = (int  *)malloc(sizeof(int )*n);
  y1 = (int  *)malloc(sizeof(int )*n);
  read_buf(x,4*n);
  read_buf(y,4*n);
  read_buf(&flag,4);
  read_buf(&color,4);
  for(i=0;i<n;i++)
  {
    x1[i] =scr_x(x[i]); 
    y1[i] =scr_y(y[i]);
  }
  if(n==4){
   dx1= x1[1]-x1[0];
   dy1= y1[1]-y1[0];
   dx2= x1[3]-x1[2];
   dy2= y1[3]-y1[2];
   if( ((dx1*dy1)&( dx2*dy2)) ==0){
     if(x1[0]>x1[2]) {b_v_x1=x1[2];b_v_x2=x1[0];}
     else {b_v_x1=x1[0];b_v_x2=x1[2];}
     if(y1[0]>y1[2]) {b_v_y1=y1[2];b_v_y2=y1[0];}
     else {b_v_y1=y1[0];b_v_y2=y1[2];}
     /*unclip_files();*/
   }
  }
  _poly_fill_ps((int)n,x1,y1,(int)flag,(unsigned char)color);
  free(x); free(y); free(x1); free(y1);
}
static int get_int()
 {
   int ch,n,Sign;
    n=0;
    Sign=1;
    ch = getc(tmppt);
    /*while((ch>='0')&&(ch<='9')){*/
     if(ch=='-') {Sign=-1;ch = getc(tmppt);}
     while((isdigit(ch))){
     n = n*10+(ch-'0');
     ch = getc(tmppt);
    }
    return(Sign*n);
 }
static void rmv_files(void)
 {
  char flname[NCLRS];
  int i;
/*
  for(i=0;i<NCLRS;i++)
    {
     sprintf(flname,"Hc_pen0.%3.3d\0",i);
     remove(flname);
     sprintf(flname,"Hc_pen1.%3.3d\0",i);
     remove(flname);
    }
*/
 }
static void opn_files(void)
 {
  char flname[NCLRS];
  int i;
/*
  for(i=0;i<NCLRS;i++)
    {
     sprintf(flname,"Hc_pen0.%3.3d\0",i);
     hcptr[i]=fopen(flname,"wb");
    }
*/
     TX_F=fopen(TX_HC,"wb");
     if(TX_F==NULL) printf("Failed to open: %-s\n",TX_HC);
 }
#if 0
static void cpy_text()
{
 int ch,i,x,y;
 float angle;
 char buf1[60],buf2[60];
// fclose(TX_F);
 TX_F=fopen(TX_HC,"rb");
 ch=getc(TX_F);
 while( (ch!=EOF)){
   if(ch=='/'){
    i=0;
    buf2[i]=ch; while((buf2[++i]=getc(TX_F))!='\n');buf2[++i]='\0';
    ch = getc(TX_F);
    if(ch=='/') continue;
    else fprintf(hbuf,"%s",buf2);
   }
   else {
    ungetc(ch,TX_F);
    fscanf(TX_F,"%d %d",&x,&y);while( (ch=getc(TX_F))!='\n');
    if(LSCAPE) fprintf(hbuf,"%d %d m\n",x,(EVGAY-y));
    else fprintf(hbuf,"%d %d m\n",y,x);
    i=0;while((ch=getc(TX_F))!='\n') buf1[i++]=ch;
    buf1[i++]='\n';buf1[i]='\0';
    sscanf(&buf1[19],"%f",&angle);
    sprintf(&buf1[19],"%-6.1f",T_rot+angle);
    buf1[25]=' ';
    fprintf(hbuf,"%s",buf1);
    ch = getc(TX_F);
   }
 }
 fclose(TX_F);
 
}
#endif
static void cpy_files(void)
 {
  int i,swap1;
  int ch,xo,yo,x,y,xx,yy,PENUP=1,chr;
  float angle,xs,ys;
  int x1,y1,x2,y2;
  char buf1[200],buf2[200];
  char flname[NCLRS];
  unsigned char Esc=0x1b,Per='%',o=97,a='&';
  /*hbuf=fopen("GRAF","wb");*/
  fprintf(hbuf,"%%%%Page: %2d %d\n",page,page);
  cpy_hdr1();
  swap1=(SWAP+1)%2;
  fclose(TX_F);
  tmppt=fopen(TX_HC,"rb");
    {
     /*fprintf(hbuf,"%%Polyline \n n ");*/
     xo=-100;yo=-100;
     PENUP=1;
     ch=getc(tmppt);
     while(ch != EOF) {
      if(ch=='Z'){
        ch = getc(tmppt);
        switch (ch){
         case 'M': {
                x1=get_int();
                y1=get_int();
                x2=get_int();
                y2=get_int();
                i=0;while((ch=getc(tmppt))!='\n') buf1[i++]=ch;
                buf1[i]='\0';
                DRAW_IMAGE(buf1,x1,y1,x2,y2);
               }
               break;
         case 'U':
              x=get_int();
              y=get_int();
     /*         if((x!=xo) || (y!=yo)) {*/
              {
                if(LSCAPE) LPU(x,y);
                else PPU(x,y);
                PENUP=1;
              }
              xo=x;yo=y;
              break;
         case 'D':
              x=get_int();
              y=get_int();
              if((x!=xo) || (y!=yo)||(PENUP)){
                 PENUP=0;
                if(LSCAPE) LPD(x,y);
                else PPD(x,y);
              }
              xo=x;yo=y;
              break;
         case 'L':
              x=get_int();
              y=get_int();
              xx=get_int();
              yy=get_int();
              if((x!=xo) || (y!=yo)) {
                PENUP=1;
                if(LSCAPE) LPU(x,y);
                else PPU(x,y);
              }
              if((x!=xx) || (y!=yy)||(PENUP)){
                 PENUP=0;
                if(LSCAPE) LPD(xx,yy);
                else PPD(xx,yy);
              }
              xo=xx;yo=yy;
              break;
         case 'W':
              x=get_int();
              fprintf(hbuf,"%-8.2f setlinewidth\n",(x*Ln_width));
              break;
         case 'Y':
              x=get_int();
              fprintf(hbuf,"%-d setlinecap 0 setlinejoin\n",(x));
              break;
         case 'I':
              i=get_int()%16;
              fprintf(hbuf,"clp col%3.3d 1.0 graycol gs fill gr\n",i);
              break;
         case 'S':
              fprintf(hbuf,"save\n");
              break;
         case 'R':
              fprintf(hbuf,"restore\n");
              break;
         case 'G':
              fprintf(hbuf,"initgraphics\n");
              break;
         case 'X':
              fprintf(hbuf,"\n");
              SETCLIP;
              break;
         case 'p': /* text pattern */
              t_pattern=get_int();
              break;
         case 'b': /* text bodr */
              t_bodr=get_int();
              break;
         case 'c': /* text colour */
              t_color=get_int();
              break;
         case 'f': /* text fill condition */
              t_fill=get_int();
              break;
         case 'F':
              putc('/',hbuf);
         case 'C':
              while ( (ch=getc(tmppt)) != '\n') putc(ch,hbuf);
              putc(ch,hbuf);
              break;
         case 'P':
              {
                int x,y,w,h;
                x=get_int();
                y=get_int();
                w =get_int();
                h=get_int();
                /*if(LSCAPE) LCLIP(x,y,w,h) else PCLIP(x,y,w,h);*/
                Clip.x = x, Clip.y = y, Clip.w = w, Clip.h =h;
                SETCLIP;
              }
              break;
         case 'T':
              i=0;while((ch=getc(tmppt))!='\n') buf1[i++]=ch;
              buf1[i++]='\n';buf1[i]='\0';
              sscanf(&buf1[19],"%f",&angle);
              sscanf(&buf1[3],"%f",&xs);
              sscanf(&buf1[10],"%f",&ys);
              sprintf(&buf1[19],"%-6.1f",T_rot+angle);
              buf1[25]=' ';
              if(LSCAPE) {
                sprintf(&buf1[3],"%-6.2f",xs);
                sprintf(&buf1[10],"%-6.2f",-ys);
              }
              else {
                sprintf(&buf1[3],"%-6.2f",ys);
                sprintf(&buf1[10],"%-6.2f",-xs);
              }
              buf1[9]=' ';
              buf1[16]=' ';
              fprintf(hbuf,"%s",buf1);
              break;
        }
      }
     /*else putc(ch,hbuf);*/
     ch=getc(tmppt);
     }
     fprintf(hbuf,"gs col%3.3d s gr n\n",PEN);
     fclose(tmppt);
//     remove(TX_HC);
    }
 }
static void unclip_files(void)
 {
  int i;
  int ch,xo,yo,x,y,xx,yy,PENUP=1;
  int xd,yd,xxd,yyd;
  char flname[NCLRS];
  int swap1,clip,cond=0;
  if( b_v_x1<c_v_x1) {cond++;b_v_x1=c_v_x1;};
  if( b_v_y1<c_v_y1) {cond++;b_v_y1=c_v_y1;};
  if( b_v_x2>c_v_x2) {cond++;b_v_x2=c_v_x2;};
  if( b_v_y2>c_v_y2) {cond++;b_v_y2=c_v_y2;};
  swap1=(SWAP+1)%2;
  for(i=0;i<NCLRS;i++)
    {
     fclose(hcptr[i]);
     sprintf(flname,"Hc_pen%1.1d.%3.3d\0",SWAP,i);
     tmppt=fopen(flname,"rb");
     sprintf(flname,"Hc_pen%1.1d.%3.3d\0",swap1,i);
     hcptr[i]=fopen(flname,"wb");
     xo=-100;yo=-100;
     PENUP=1;
     ch=getc(tmppt);
     while(ch != EOF) {
      if(ch=='Z'){
        ch = getc(tmppt);
        if( ch=='L'){
              x=get_int();
              y=get_int();
              xx=get_int();
              yy=get_int();
              xd=x,yd=y,xxd=xx,yyd=yy;
              clip=unclip_line(&xd,&yd,&xxd,&yyd);
              if(clip==0){
                fprintf(hcptr[i],"ZL%d,%d,%d,%d;",x,y,xx,yy);
              }
              else if(clip!=1){
                if((x!=xd)||(y!=yd))
                  fprintf(hcptr[i],"ZL%d,%d,%d,%d;",x,y,xd,yd);
                if((xx!=xxd)||(yy!=yyd))
                  fprintf(hcptr[i],"ZL%d,%d,%d,%d;",xx,yy,xxd,yyd);
              }

        }
      }
     ch=getc(tmppt);
     }
     fclose(tmppt);
    }
   SWAP=swap1;
 }
static void initialise()
{
  int i;
  FONT *pt;
  int count,font;
//  page=0;
  SWAP=0;
  for(i=0;i<10;i++) st_ptr[i]=0;
  ln_style=LN_STYL;
  m_style=M_STYL;
  if (ENTRY==0) { RBUFF = (unsigned char *) malloc(B_max+100);
                  ENTRY=1;};
  Entry =0;
  Byte=0,R_max=0,R_Byte=0;
  BACK_UP=0;

  remove(OutFile);
  hbuf=fopen(OutFile,"wb");
//  Entry=0;
  step=0;
  font_size=188.0,font_size_o=0.;
  xscale=-1,yscale=1;
  t_font_o=-1;
  t_pattern=15;
  t_fill = 1;
  t_bkgr = 0;
  t_bodr = 0;
  c_loc=NULL;
  Rbuff= RBUFF;
  BACK_UP=0;
  GHMAX=1;
  GHBUF=df_gbuf;
  Byte=0;
  R_Byte=0;
  dp = far_ptr;
  index0=0;
  if(Fontlist == NULL ) Fontlist=(Dlink *)Loadfontstruct();
//  icpos = icposf0;icxv=icxvf0;icyv=icyvf0;m_f=m_f0;
  count = Dcount(Fontlist);
  font =0;
  Dposition(Fontlist,font+1);
  pt = (FONT *)Getrecord(Fontlist);
  icpos = pt->icpos;
  icxv = pt->icxv;
  icyv = pt->icyv;
  m_f =  pt->m_f;
//  icposf0 = icpos;icxvf0=icxv;
/*
  icyvf0=icyv;m_f0=m_f;
  icpos = pt->icpos;
  icxv = pt->icxv;
  icyv = pt->icyv;
  m_f =  pt->m_f;
*/
  t_angle=0.;
  ln_width=1;
  pr_txt=1;
  cost = 1.0;
  sint = 0.0;
  txt_ht=200;
  txt_wt=200;
  txt_htx=200;
  txt_wty=200;
  txt_hty=200;
  txt_wtx=200;
  txt_w42 = txt_wt/CFact;
  txt_h42 = txt_ht/CFact;
  txt_sp=20;
  txt_spy=20;
  txt_spx=20;
  v_x1=0;
  v_y1=0;
  v_x2=EVGAX;
  v_y2=EVGAY;
  w_x1=0;
  w_y1=0;
  w_x2=EVGAX;
  w_y2=EVGAY;
  u_x=1;
  u_y=1;
  V_x=EVGAX;
  V_y=EVGAY;
  D_x=0;
  D_y=0;
  jmp=8;
  lcur_x=0;
  lcur_y=EVGAY;
  rcur_x=EVGAX;
  rcur_y=0;
  v_xt1=(int)(v_x1-txt_wt-txt_sp);
  v_yt1=(int)(v_y1-txt_ht);
  set_sup_clip_limits(v_x1,v_y1,v_x2,v_y2);
  set_clip_limits(v_x1,v_y1,v_x2,v_y2,v_xt1,v_yt1);
  rmv_files();
  opn_files();
  GPEN(1);
  return;
}
static void reinitialise()
{
  int i;
  FONT *pt;
  int count,font;
  for(i=0;i<10;i++) st_ptr[i]=0;
  ln_style=LN_STYL;
  m_style=M_STYL;
  Entry=0;
  step=0;
  t_pattern=15;
  t_fill = 1;
  t_bkgr = 0;
  t_bodr = 1;
  clear_clip();
  c_loc=NULL;
//  icpos = icposf0;icxv=icxvf0;icyv=icyvf0;m_f=m_f0;
  if(Fontlist == NULL ) Fontlist=(Dlink *)Loadfontstruct();
  count = Dcount(Fontlist);
  font =0;
  Dposition(Fontlist,font+1);
  pt = (FONT *)Getrecord(Fontlist);
  icpos = pt->icpos;
  icxv = pt->icxv;
  icyv = pt->icyv;
  m_f =  pt->m_f;
  ln_width=1;
  pr_txt=1;
  cost = 1.0;
  sint = 0.0;
  txt_ht=200;
  txt_wt=200;
  txt_htx=200;
  txt_wty=200;
  txt_hty=200;
  txt_wtx=200;
  txt_w42 = txt_wt/CFact;
  txt_h42 = txt_ht/CFact;
  txt_sp=20;
  txt_spy=20;
  txt_spx=20;
  v_x1=0;
  v_y1=0;
  v_x2=EVGAX;
  v_y2=EVGAY;
  w_x1=0;
  w_y1=0;
  w_x2=EVGAX;
  w_y2=EVGAY;
  u_x=1;
  u_y=1;
  V_x=EVGAX;
  V_y=EVGAY;
  D_x=0;
  D_y=0;
  jmp=8;
  lcur_x=0;
  lcur_y=EVGAY;
  rcur_x=EVGAX;
  rcur_y=0;
  v_xt1=(int)(v_x1-txt_wt-txt_sp);
  v_yt1=(int)(v_y1-txt_ht);
  set_sup_clip_limits(v_x1,v_y1,v_x2,v_y2);
  set_clip_limits(v_x1,v_y1,v_x2,v_y2,v_xt1,v_yt1);
#if 1
//  rmv_files();
  opn_files();
  GPEN(1);
#endif
  return;
}

static void set_viewport(int x1,int y1,int x2,int y2)
{
   float t;
   v_x1=x1;
   v_y1= y1;
   v_y1= (int) (v_y1+0.1);
   v_x2=x2;
   v_y2= y2;
   v_y2=(int) ( v_y2+0.1);
   if( v_y2 <= 0) {exit(0);}
   D_x=v_x1;
   D_y=v_y1;
   V_x=v_x2-v_x1;
   V_y=v_y2-v_y1;
   t=(V_x*V_x+V_y*V_y);
   u_x=(V_x/(w_x2-w_x1));
   u_y=(V_y/(w_y2-w_y1));
   v_r=sqrt((double)t);
   lcur_x=v_x1;
   lcur_y=EVGAY-v_y1;
   rcur_x=v_x2;
   rcur_y=EVGAY-v_y2;
   set_sup_clip_limits(v_x1,v_y1,v_x2,v_y2);
   set_clip_limits(v_x1,v_y1,v_x2,v_y2,v_xt1,v_yt1);
}


static void user_cord(float x1,float y1,float x2,float y2)
{
  w_x1=x1;
  w_y1=y1;
  w_x2=x2;
  w_y2=y2;
  u_x=(V_x/(x2-x1));
  u_y=(V_y/(y2-y1));
  r_d=(((x2-x1)*(x2-x1)+
       (y2-y1)*(y2-y1)));
  r_d = sqrt((double)r_d);
  txt_ht = (w_y2-w_y1)*0.03;
  txt_wt = (w_x2-w_x1)*0.03;
  txt_sp = (w_x2-w_x1)*0.003;
  txt_htx=txt_ht;
  txt_hty=txt_ht;
  txt_wtx = txt_wt;
  txt_wty = txt_wt;
  txt_spx=txt_sp;
  txt_spy=txt_sp;

}

static void set_n_view_o(void)
 {
  int x1,y1,x2,y2;
  read_buf(&x1,4);
  read_buf(&y1,4);
  read_buf(&x2,4);
  read_buf(&y2,4);
  x1 = ((float)x1)/639.0*(float)EVGAX +0.5;
  y1 = ((float)y1)/479.0 *(float)EVGAY+0.5;
  x2 = ((float)x2)/639.0 *(float)EVGAX+0.5;
  y2 = ((float)y2)/479.0 *(float)EVGAY+0.5;
  set_viewport(x1,y1,x2,y2);
 }
static void set_n_view(void)
 {
  int x1,y1,x2,y2;
  float vx1,vy1,vx2,vy2;
  read_buf(&vx1,4);
  read_buf(&vy1,4);
  read_buf(&vx2,4);
  read_buf(&vy2,4);
  x1 = ((float)vx1)*(float)EVGAX +0.5;
  y1 = ((float)vy1)*(float)EVGAY+0.5;
  x2 = ((float)vx2)*(float)EVGAX+0.5;
  y2 = ((float)vy2)*(float)EVGAY+0.5;
  set_viewport(x1,y1,x2,y2);
 }
static void set_usrcord(void)
 {
  float x1,y1,x2,y2;
  read_buf(&x1,4);
  read_buf(&y1,4);
  read_buf(&x2,4);
  read_buf(&y2,4);
  user_cord(x1,y1,x2,y2);
 }
static void set_3d(void) {
   float x1,y1,x2,y2;
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
}
static void t_move(float x,float y)
  {
  int x1,y1;
  x1=scr_x(x);
  y1=scr_y(y);
  _move(x1,y1);
  }
static  void t_draw(float ix,float iy)
  {
  int x1,y1;
  x1=scr_x(ix);
  y1=scr_y(iy);
  _draw(x1,y1);
  }
static void win_3_move()
{
    float x,y,z;

    read_buf(&x,4);
    read_buf(&y,4);
    read_buf(&z,4);
    transfrm(x,y,z);
    projection(trnstr);
    if (ZBUFF == 1)
    { Cx = newstr.xstr, Cy = newstr.ystr, Cz = newstr.zstr;}
    else
     t_move(newstr.xstr,newstr.ystr);
}
static void win_3_draw()
{
    float x,y,z;

    read_buf(&x,4);
    read_buf(&y,4);
    read_buf(&z,4);
    transfrm(x,y,z);
    projection(trnstr);
    if (ZBUFF == 1) {
       lt = (LINE *) malloc(sizeof(LINE));
       lt->code ='l';
       if( Cz > newstr.zstr ) { lt->zmax = Cz; lt->zmin = newstr.zstr;}
       else { lt->zmin = Cz; lt->zmax = newstr.zstr;}
       lt->clr = c_color; lt->style = ln_style; lt->width = ln_width;
       Cx = newstr.xstr, Cy = newstr.ystr, Cz = newstr.zstr;
       Dadd(Pnlist,lt);
    }
    else t_draw(newstr.xstr,newstr.ystr);
}
static void t_panel(float *x,float *y,int color,int flag,int n)
  {
  int *x1,*y1;
  int xo,yo,xv,yv;
  short i,j=0;
  x1 = (int *) malloc(sizeof(int)*(n+1));
  y1 = (int *) malloc(sizeof(int)*(n+1));
  if( (y1==NULL) ){
    printf(" Error: Not enough buffer for polyfill..\n");
    exit(0);
  }
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
  if( (j>2)&&(j<500)) _poly_fill_ps((short)j,x1,y1,(short)flag,(unsigned char)color);
  free(x1),free(y1);
}
static void win_3_polyfill()
{
    float x[200],y[200],z[200];
    float *xt,*yt,*zt;
    float zmax=-1.e+20,zmin = 1.e+20;
    int n,flag,color;
    int i;

    read_buf(&n,4);
    read_buf(x,4*n);
    read_buf(y,4*n);
    read_buf(z,4*n);
    read_buf(&flag,4);
    read_buf(&color,4);
    for(i=0;i<n;i++)
    {
     transfrm(*(x+i),*(y+i),*(z+i));
     projection(trnstr);
     *(x+i) = newstr.xstr;
     *(y+i) = newstr.ystr;
     *(z+i) = newstr.zstr;
    } 
    if (ZBUFF == 1) {
      pt = (POLY *) malloc(sizeof(POLY));
      pt->code = 'p';
      pt->n = n;
      xt = (float *)malloc(sizeof(float)*n);
      yt = (float *)malloc(sizeof(float)*n);
      zt = (float *)malloc(sizeof(float)*n);
      for(i=0;i<n;i++) {
        if(zmin > *(z+i) ) zmin = *(z+i);
        if(zmax < *(z+i) ) zmax = *(z+i);
        *(xt+i) = *(x+i);
        *(yt+i) = *(y+i);
        *(zt+i) = *(z+i);
      }
      pt->x = xt, pt->y = yt, pt->z = zt;
      pt->zmax = zmax, pt->zmin = zmin;
      pt->fclr = color;
      pt->bclr = c_color;
      pt->flag = flag;
      Dadd(Pnlist,pt);
    }
    else 
    t_panel(x,y,(int)color,flag,(int)n);
}

static void t_box_fill(float x1,float y1,float x2,float y2,int color,int ib)
  {
    int xa,ya,xb,yb;
    xa=scr_x(x1);
    ya=scr_y(y1);
    xb=scr_x(x2);
    yb=scr_y(y2);
    _box_fill(xa,ya,xb,yb,color);
  }
static void win_3_boxfill()
{
   float  x1,x2,y1,y2,z1,z2,fl,ib;
   float *xt,*yt,*zt;
   float zmax=-1.e+20,zmin = 1.e+20;

   read_buf(&x1,4);
   read_buf(&y1,4);
   read_buf(&z1,4);
   read_buf(&x2,4);
   read_buf(&y2,4);
   read_buf(&z2,4);
   read_buf(&fl,4);
   read_buf(&ib,4);
   transfrm(x1,y1,z1);
   projection(trnstr);
   x1 = newstr.xstr;
   y1 = newstr.ystr;
   z1 = newstr.zstr;
   transfrm(x2,y2,z2);
   projection(trnstr);
   x2 = newstr.xstr;
   y2 = newstr.ystr;
   z2 = newstr.zstr;
   if (ZBUFF == 1) {
      pt = (POLY *) malloc(sizeof(POLY));
      pt->code = 'p';
      pt->n = 4;
      xt = (float *)malloc(sizeof(float)*4);
      yt = (float *)malloc(sizeof(float)*4);
      yt = (float *)malloc(sizeof(float)*4);
      if( z1 >z2 ) { zmin = z2;zmax = z1;}
      else { zmin = z1;zmax = z2;}
      xt[0] = x1; yt[0] = y1; zt[0] = z1;
      xt[1] = x2; yt[1] = y1; zt[1] = z1;
      xt[2] = x2; yt[2] = y2; zt[2] = z2;
      xt[3] = x1; yt[3] = y2; zt[3] = z2;
      pt->x = xt, pt->y = yt, pt->z = zt;
      pt->zmax = zmax, pt->zmin = zmin;
      pt->fclr = fl;
      pt->bclr = c_color;
      pt->flag = ib;
      Dadd(Pnlist,pt);
   }
   else
   t_box_fill(x1,y1,x2,y2,fl,ib);
}
static  void t_godr_fill(int n,float *x,float *y,float *p)
 {
  int x1[200],y1[200];
  float p1[20],pv;
  short i;
  for(i=0;i<n;i++)
  {
    x1[i] =scr_x(x[i]); 
    y1[i] =scr_y(y[i]);
  }
   for (i=0;i<n;i++) {pv=*(p++), p1[i] = pclr(pv); }
  _godard_fill((short)n,x1,y1,(short)ibdr,p1);
 }
static void win_3_godrfill()
{
  float x[200],y[200],z[200],v[200];
   float *xt,*yt,*zt,*vt;
   float zmax=-1.e+20,zmin = 1.e+20;
  int i;
  int n;

  read_buf(&n,4);
  read_buf(x,4*n);
  read_buf(y,4*n);
  read_buf(z,4*n);
  read_buf(v,4*n);
  for(i=0;i<n;i++)
   {
     transfrm(*(x+i),*(y+i),*(z+i));
     projection(trnstr);
     *(x+i) = newstr.xstr;
     *(y+i) = newstr.ystr;
     *(z+i) = newstr.zstr;
   }
 if (ZBUFF == 1) {
      st = (SHADE *) malloc(sizeof(SHADE));
      st->code = 's';
      st->n = n;
      xt = (float *)malloc(sizeof(float)*n);
      yt = (float *)malloc(sizeof(float)*n);
      zt = (float *)malloc(sizeof(float)*n);
      vt = (float *)malloc(sizeof(float)*n);
      for(i=0;i<n;i++) {
        if(zmin > *(z+i) ) zmin = *(z+i);
        if(zmax < *(z+i) ) zmax = *(z+i);
        *(xt+i) = *(x+i);
        *(yt+i) = *(y+i);
        *(zt+i) = *(z+i);
        *(vt+i) = *(v+i);
      }
      st->x = xt, st->y = yt, st->z = zt, st->v = vt;
      st->zmax = zmax, st->zmin = zmin;
      st->bclr = c_color;
      Dadd(Pnlist,st);
 }
 else
 t_godr_fill(n,x,y,v);
} 
static int CompPanel(void *pt1,void *pt2){
   LINE *lt1,*lt2;
   lt1 = (LINE *)pt1;
   lt2 = (LINE *)pt2;
   if( (lt2->zmax < lt1->zmax )) return 1;
   else return 0;
}
static int Displaylist(Dlink *Pnlist) {

  if( Pnlist != NULL) {
    Dsort(Pnlist,CompPanel);
    Resetlink(Pnlist);
    while( (lt = (LINE *)Getrecord(Pnlist)) != NULL) {
       switch(lt->code) {
          case 'l':
            c_color  = lt->clr;
            ln_style = lt->style;
            ln_width = lt->width;
            t_move(lt->x1,lt->y1);
            t_draw(lt->x2,lt->y2);
            break;
          case 'p':
            pt = (POLY *)lt;
            c_color  = pt->bclr;
            t_panel(pt->x,pt->y,pt->fclr,pt->flag,pt->n);
            free(pt->x),free(pt->y),free(pt->z);
            break;
          case 's':
            st = (SHADE *)lt;
            c_color  = st->bclr;
            t_godr_fill(st->n,st->x,st->y,st->v);
            free(st->x),free(st->y),free(st->z),free(st->v);
            break;
       }
    }
  }
}
static int Sort_Display(Dlink *Pnlist) {
 int i=0,count,n,Nsize = 3000;
 Dlink *tmp1,*tmp2,*tmp;
 double Zmax=-1e+20,Zmin=1e+20,dz,zu,zl;
 static int entry=0;
 if( Pnlist != NULL) {
    Resetlink(Pnlist);
    while( (lt = (LINE *)Getrecord(Pnlist)) != NULL) {
      i++;
      if(lt->zmax > Zmax ) Zmax = lt->zmax;
      if(lt->zmin < Zmin ) Zmin = lt->zmin;
    }
    if(i < Nsize) Displaylist(Pnlist);
    else {
      entry++;
      n = i/Nsize+1;
      dz = (Zmax-Zmin)/n;
      tmp = Pnlist;
      zl = Zmin; zu = zl+dz;
      for(i=0;i<n;i++) {
        Resetlink(tmp);
        tmp1 = Dopen();
        tmp2 = Dopen();
        while( (lt = (LINE *)Getrecord(tmp)) != NULL) {
          if((lt->zmax < zu) &&( lt->zmax >= zl) ) Dadd(tmp1,lt);
          else Dadd(tmp2,lt);
        }
        if( (entry > 5) &&((tmp1->st==NULL)||(tmp2->st==NULL)) ){
          Dfree(tmp1);
          Dfree(tmp2);
          Displaylist(tmp);
          if( tmp != Pnlist ) Dfree(tmp);
          entry--;
          return 1;         
        }
        else {
          Sort_Display(tmp1);
          zl = zu;
          zu += dz;
          if( tmp != Pnlist ) Dfree(tmp);
          tmp = tmp2;
          Dfree(tmp1);
        }
      }
      Sort_Display(tmp);
      if( tmp != Pnlist ) Dfree(tmp);
      entry--;
      return 1;
   }
 }
}

static void check_and_do(char ch)
 {
  char ch2;
//    printf("Code : %c\n",ch);
    switch(ch)
    {
      case 'v':
                /*printf("view \n");*/
                set_n_view();
                break;
      case 'C':
                /*printf("window \n");*/
                set_usrcord();
                break;
      case 'Z':
        Pnlist = Dopen();
        /*set_3d();*/
        break;
      case 'z':
        ZBUFF=1;
        break;
      case 'L':
                clr_view();
                break;
      case 'l':
       read_buf(&ch2,1);
       /*printf("line \n");*/
       switch(ch2)
       {
        case 'm': win_move();
                  break;
        case 'd':
                  win_draw();
                  break;
        case 'l': win_drawline();
            break;
        case 'i': win_drawimage();
            break;
        case 's': win_set_lnstyle();
            break;
        case 'w': win_set_lnwidth();
            break;
        case 'c': win_set_color();
            break;
        case 'r': win_circle();
                  break;
        case 'M': win_3_move(); break;
        case 'D': win_3_draw(); break;
       }
       break;
      case 'm':
       /*printf("marker \n");*/
       read_buf(&ch2,1);
       switch(ch2)
       {
        case 'k': win_marker();
                  break;
        case 't': win_set_markstyle();
                  break;
       }
       break;
      case 't':
       read_buf(&ch2,1);
       switch(ch2)
       {
        case 'w': win_txtwrt(); break;
        case 'h': 
        case 'o': win_set_txtbold(); break;
        case 'c': win_txt_color(); break;
        case 'l': win_txt_fill(); break;
        case 'k': win_txt_bkgr(); break;
        case 'p': win_txt_pattern(); break;
        case 'b': win_txt_bodr(); break;
        case 'f': win_txt_font(); break;
        case 's': win_txt_size(); break;
        case 'r': win_set_angle(); break;
        case 'x': win_set_prtxt(); break;
        case 'a': win_txt_slant(); break;
        default :
               printf(" Wrong Code in text :%c%c \n",ch,ch2);
               exit(0);
       }
       break;
      
     case 'p' :
      read_buf(&ch2,1);
      switch(ch2)
      {
        case 'b': win_boxfill(); break;
        case 'r': win_circlefill(); break;
        case 'p': win_poly_fill(); break;
        case 'g': win_godard_fill(); break;
        case 's': win_set_godard_clrs(); break;
        case 'h': win_set_shade_clrs(); break;
        case 'B' : win_3_boxfill(); break;
        case 'G' : win_3_godrfill(); break;  
        case 'P' : win_3_polyfill(); break;  
       }
      break;
     case 'c' :
      read_buf(&ch2,1);
      switch(ch2)
      {
        case 'h': win_chng_clr(); break;
      }
       break;
     case 's' :
      read_buf(&ch2,1);
      switch(ch2)
      {
        case 'c': win_clip_limit(); break;
        case 'p': win_pop_clip(); break;
        case 's': win_sup_clip_limit(); break;
      }
       break;
      case 'A': 
                if( (A4==0))
                 {
                  T_rot=-90.0;
                  A4=1;
                  LSCAPE=0;
                  printf(" A4 case\n");
                  y_mulf*=40./22.5;
                  set_a4copy();
                  evgay=EVGAY;
                 }
                break;
      case 'S': 
                if( (LSCAPE==0))
                 {
                  T_rot=0.0;
                  A4=0;
                  LSCAPE=1;
                  y_mulf=1.;
                  printf(" Landscape case\n");
                  set_landscape();
                 }
                break;
      case 'P': 
                Sort_Display(Pnlist);
                page++;
                printf("Page %4d \n",page);
                hardcopy();
                Pnlist = NULL;
                reinitialise();
                break;
      default : 
                printf(" Wrong Code %c \n",ch);
    }
 }

static void review_file(char *str)
{
  char ch;
  int x1,x2,y1,y2,color,flag,n;
  rbuf=open(str,O_RDWR|O_BINARY, S_IREAD|S_IWRITE);
  read_file(rbuf); 
  R_Byte=0;
  R_max=Byte;
  cpy_hdr();
  read_buf(&ch,1);
  while ( R_max>0)
  {
  check_and_do(ch);
  read_buf(&ch,1);
  }
 close(rbuf);
 fprintf(hbuf,"%%%%EOF\n");
 fclose(TX_F);
// remove(TX_HC);
}

static void _move(int xp, int yp)
{
  cur_x=xp ;
  cur_y=yp ;
  return;
}

static void win_set_markstyle()
{
  int style;
  read_buf(&style,4);
  m_style=style;
  return;
}
static void _marker(int x,int y)
{
  short fac=10;
  int px[4],py[4];
  short style,mf;
  if(code(x,y) != 0) return;
  style = m_style%20;
  mf = m_style/20;
  fac +=mf;
  switch(style)
  {
case 0 :
                 _circle_fill(x,y,(int)(3*fac),(unsigned char)c_color);
                 break;
case 1 :
                 _draw_line(x-3*fac,y-3*fac,x+3*fac,y+3*fac);
                 _draw_line(x-3*fac,y+3*fac,x+3*fac,y-3*fac);
                 break;
case 2 : 
                 _box_fill_ps(x-3*fac,y-3*fac,x+3*fac,y+3*fac,(unsigned char)15);
                 break;
case 3 : draw_line(x,y-4*fac,x,y+4*fac);
draw_line(x-4*fac,y,x+4*fac,y);
draw_line(x-2*fac,y-4*fac,x+2*fac,y-4*fac);
draw_line(x-2*fac,y+4*fac,x+2*fac,y+4*fac);
draw_line(x-4*fac,y-2*fac,x-4*fac,y+2*fac);
draw_line(x+4*fac,y-2*fac,x+4*fac,y+2*fac);
break;
case 4 : draw_line(x-4*fac,y-4*fac,x+4*fac,y+4*fac);
draw_line(x-4*fac,y+4*fac,x+4*fac,y-4*fac);
draw_line(x-4*fac,y,x+4*fac,y);
draw_line(x,y-4*fac,x,y+4*fac);
break;
case 5 : draw_line(x+3*fac,y,x,y-8*fac);
draw_line(x-3*fac,y,x,y-8*fac);
draw_line(x+3*fac,y,x,y+8*fac);
draw_line(x-3*fac,y,x,y+8*fac);
put_pixel(x,y);
break;
case 6 : draw_line(x-4*fac,y,x+4*fac,y);
draw_line(x,y-4*fac,x,y+4*fac);
draw_line(x+2*fac,y-2*fac,x+2*fac,y+2*fac);
draw_line(x+2*fac,y+2*fac,x-2*fac,y+2*fac);
draw_line(x-2*fac,y+2*fac,x-2*fac,y-2*fac);
draw_line(x-2*fac,y-2*fac,x+2*fac,y-2*fac);
break;
case 7 : put_pixel(x,y);
draw_line(x+7*fac,y,x+3*fac,y-3*fac);
draw_line(x+3*fac,y-3*fac,x,y-7*fac);
draw_line(x,y-7*fac,x-3*fac,y-3*fac);
draw_line(x-3*fac,y-3*fac,x-7*fac,y);
draw_line(x-7*fac,y,x-3*fac,y+3*fac);
draw_line(x-3*fac,y+3*fac,x,y+7*fac);
draw_line(x,y+7*fac,x+3*fac,y+3*fac);
draw_line(x+3*fac,y+3*fac,x+7*fac,y);
break;
case 8 : draw_line(x-4*fac,y-4*fac,x+4*fac,y+4*fac);
draw_line(x+4*fac,y-4*fac,x-4*fac,y+4*fac);
draw_line(x+4*fac,y+4*fac,x+4*fac,y+2*fac);
draw_line(x+4*fac,y+4*fac,x+2*fac,y+4*fac);
draw_line(x+4*fac,y-4*fac,x+4*fac,y-2*fac);
draw_line(x+4*fac,y-4*fac,x+2*fac,y-4*fac);
draw_line(x-4*fac,y-4*fac,x-2*fac,y-4*fac);
draw_line(x-4*fac,y-4*fac,x-4*fac,y-2*fac);
draw_line(x-4*fac,y+4*fac,x-4*fac,y+2*fac);
draw_line(x-4*fac,y+4*fac,x-2*fac,y+4*fac);
break;
        case 9 :
                 px[0]=x+4*fac;py[0]=y;px[1]=x-4*fac;py[1]=y+4*fac;
                 px[2]=x-4*fac;py[2]=y-4*fac;
                 _poly_fill_ps((short)3,px,py,(short)0,(unsigned char)15);
                 break;
        case 10:
                 px[0]=x-4*fac;py[0]=y;px[1]=x+4*fac;py[1]=y+4*fac;
                 px[2]=x+4*fac;py[2]=y-4*fac;
                 _poly_fill_ps((short)3,px,py,(short)0,(unsigned char)15);
                 break;
        case 11:
                 px[0]=x;py[0]=y+4*fac;px[1]=x-4*fac;py[1]=y-4*fac;
                 px[2]=x+4*fac;py[2]=y-4*fac;
                 _poly_fill_ps((short)3,px,py,(short)0,(unsigned char)15);
                 break;
        case 12:
                 px[0]=x;py[0]=y-4*fac;px[1]=x-4*fac;py[1]=y+4*fac;
                 px[2]=x+4*fac;py[2]=y+4*fac;
                 _poly_fill_ps((short)3,px,py,(short)0,(unsigned char)15);
                 break;
case 13 :
                 _circle(x,y,(int)(3*fac));
                 break;
case 14 :
                 _circle_fill(x,y,(int)(1*fac),(unsigned char)(c_color));
                 break;
case 15:
                 _draw_line(x-2*fac,y-2*fac,x+2*fac,y+2*fac);
                 _draw_line(x-2*fac,y+2*fac,x+2*fac,y-2*fac);
                 break;
        case 16:
                 px[0]=x+2*fac;py[0]=y;px[1]=x-2*fac;py[1]=y+2*fac;
                 px[2]=x-2*fac;py[2]=y-2*fac;
                 _poly_fill_ps((short)3,px,py,(short)0,(unsigned char)15);
                 break;
        case 17:
                 px[0]=x-2*fac;py[0]=y;px[1]=x+2*fac;py[1]=y+2*fac;
                 px[2]=x+2*fac;py[2]=y-2*fac;
                 _poly_fill_ps((short)3,px,py,(short)0,(unsigned char)15);
                 break;
        case 18:
                 px[0]=x;py[0]=y+2*fac;px[1]=x-2*fac;py[1]=y-2*fac;
                 px[2]=x+2*fac;py[2]=y-2*fac;
                 _poly_fill_ps((short)3,px,py,(short)0,(unsigned char)15);
                 break;
        case 19:
                 px[0]=x;py[0]=y-2*fac;px[1]=x-2*fac;py[1]=y+2*fac;
                 px[2]=x+2*fac;py[2]=y+2*fac;
                 _poly_fill_ps((short)3,px,py,(short)0,(unsigned char)15);
                 break;
default :break;
  }
}

static void win_set_color(void)
{
  int color;
  read_buf(&color,4);
#ifdef MONO
  if(color != 0 ) color =15;
#endif
  c_color=color;
  GPEN(color);
  return;
}

static void win_set_lnstyle(void)
{
  int style;
  int i,gap;
  unsigned int pos;
  read_buf(&style,4);
  if(style != ln_style) {
    if(style) GTY(0)
    else GTY(2);
  }
  ln_style=style;
/*
  fprintf(hbuf,"UL%d",style);
  if(style==0){fprintf(hbuf,";");return;}
  else fprintf(hbuf,",");
  gap=0;
  for(i=0;i<32;i++)
   {
    pos = ln_ptn[style%10]&(0x80000000>>i);
    if( pos==0){
     fprintf(hbuf,"%d,",gap);
     gap=0;
    }
    else gap++;
   }
  fprintf(hbuf,"%d;",gap);
*/
  return;
}

static void size_font_correction(void)
{
  double xs,ys,cs,ss;
  font_size=txt_ht/SCALE;
  /*fprintf(TX_F,"/%-s findfont %-6.1f scalefont setfont\n",FontName,font_size);*/
  xscale=1;
  yscale=1;
  cs = 1. -acos( fabs((double)cost))/1.5708;
  ss = 1.- cs;
  /*if( txt_wt != 0.) yscale= txt_ht/txt_wt;*/
  if( txt_ht != 0.) yscale= txt_wt/txt_ht;
  xs = ss*yscale + cs*xscale;
  ys = ss*xscale + cs*yscale;
  xscale =(xs); yscale = (ys);
}
static void change_size_font(void)
{
  double xs,ys,cs,ss;
  font_size=txt_ht/SCALE;
  fprintf(TX_F,"ZF%-s findfont %-6.1f scalefont setfont\n",
                 FontName,font_size);
  xscale=1;
  yscale=1;
  cs = 1. -acos( fabs((double)cost))/1.5708;
  ss = 1.- cs;
  /*if( txt_wt != 0.) yscale= txt_ht/txt_wt;*/
  if( txt_ht != 0.) yscale= txt_wt/txt_ht;
  xs = ss*yscale + cs*xscale;
  ys = ss*xscale + cs*yscale;
  xscale =(xs); yscale = (ys);
}
static void win_set_angle(void)
{
  float t;
  float fact,xfact,yfact;
  read_buf(&t,4);
  t_angle=t;
  theta=-t*rad;
  cost = cos(theta);
  sint = sin(theta);
  xfact = (w_x2-w_x1)/x_mulf;
  yfact = (w_y2-w_y1)/y_mulf;
  fact = xfact/yfact;
  txt_wt= sint*sint*txt_wty*fact+cost*cost*txt_wtx;
  txt_sp= sint*sint*txt_spy*fact+cost*cost*txt_spx;
  txt_ht= sint*sint*txt_htx/fact+cost*cost*txt_hty;
  txt_ht=txt_ht*V_y/((w_y2-w_y1));
  txt_wt=txt_wt*V_x/((w_x2-w_x1));
  txt_sp=txt_sp*V_x/((w_x2-w_x1));
  v_xt1=(int)(v_x1-txt_wt-txt_sp);
  v_yt1=(int)(v_y1-txt_ht);
  txt_w42 = txt_wt/CFact;
  txt_h42 = txt_ht/CFact;
  size_font_correction();
}
static unsigned char code(int x, int y)
{
  unsigned char t;
  t = 0;
/*  if (y > v_y2) t++;
  t<<=1;
  if (y < v_y1) t++;
  t<<=1;
  if (x > v_x2) t++;
  t<<=1;
  if (x < v_x1) t++;*/

  if (y > c_v_y2) t|=0x08;
  if (y < c_v_y1) t|=0x04;
  if (x > c_v_x2) t|=0x02;
  if (x < c_v_x1) t++;
  return(t);
}


static short clip_line(int *xa,int *ya, int *xb, int *yb)
{
  int x,y,tx,ty,xs=c_v_x1,ys=c_v_y1;
  unsigned char c1,c2,c;
  c1=code(*xa,*ya);
  c2=code(*xb,*yb);
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
y=(int)(*ya+(c_v_x1-(*xa))*ty/tx);
if ((y==ys) && ((ys>c_v_y2) || (ys<c_v_y1))) return(0);
ys=y;
x=c_v_x1;
                goto jump;
 }
 if (c & 2)
 {
                if(tx==0) return(0);
y=(int)(*ya+(c_v_x2-(*xa))*ty/tx);
                if ((y==ys) && ((ys>c_v_y2) || (ys<c_v_y1))) return(0);
ys=y;
x=c_v_x2;
                goto jump;
 }
 if (c & 8)
 {
                if(ty==0) return(0);
x=(int)(*xa+(c_v_y2-(*ya))*tx/ty);
if (x==xs && ((xs<c_v_x1) || (xs>c_v_x2))) return(0);
xs=x;
y=c_v_y2;
                goto jump;
 }
 if (c & 4)
 {
                if(ty==0) return(0);
x=(int)(*xa+(c_v_y1-(*ya))*tx/ty);
              if (x==xs && ((xs<c_v_x1) || (xs>c_v_x2))) return(0);
xs=x;
y=c_v_y1;
 }
 jump:
 if (c==c1)
 {
c1=code(x,y);
*xa=x;
*ya=y;
 }
 else
 {
c2=code(x,y);
*xb=x;
*yb=y;
 }
  }
  return(1);
}

static unsigned char uncode(int x, int y)
{
  unsigned char t;
  t = 0;
  if (y > b_v_y2) t|=0x08;
  if (y < b_v_y1) t|=0x04;
  if (x > b_v_x2) t|=0x02;
  if (x < b_v_x1) t++;
  return(t);
}


static short unclip_line(int *xa,int *ya, int *xb, int *yb)
{
  int x,y,tx,ty,xs=b_v_x1,ys=b_v_y1;
  unsigned char c1,c2,c;
  c1=uncode(*xa,*ya);
  c2=uncode(*xb,*yb);
  if ((c1 & c2) != 0) return(0);
  if ((c1 | c2) == 0) return(1);
  while ((c1!=0) ||( c2!=0))
  {
 if (c1==0) c=c2;
 else c=c1;
         tx = ((*xb)-(*xa));
         ty = ((*yb)-(*ya));
 if (c & 1)
 {
                if(tx==0) return(0);
y=(int)(*ya+(b_v_x1-(*xa))*ty/tx);
if ((y==ys) && ((ys>b_v_y2) || (ys<b_v_y1))) return(0);
ys=y;
x=b_v_x1;
                goto jump;
 }
 if (c & 2)
 {
                if(tx==0) return(0);
y=(int)(*ya+(b_v_x2-(*xa))*ty/tx);
                if ((y==ys) && ((ys>b_v_y2) || (ys<b_v_y1))) return(0);
ys=y;
x=b_v_x2;
                goto jump;
 }
 if (c & 8)
 {
                if(ty==0) return(0);
x=(int)(*xa+(b_v_y2-(*ya))*tx/ty);
if (x==xs && ((xs<b_v_x1) || (xs>b_v_x2))) return(0);
xs=x;
y=b_v_y2;
                goto jump;
 }
 if (c & 4)
 {
                if(ty==0) return(0);
x=(int)(*xa+(b_v_y1-(*ya))*tx/ty);
              if (x==xs && ((xs<b_v_x1) || (xs>b_v_x2))) return(0);
xs=x;
y=b_v_y1;
 }
 jump:
 if (c==c1)
 {
c1=uncode(x,y);
*xa=x;
*ya=y;
 }
 else
 {
c2=uncode(x,y);
*xb=x;
*yb=y;
 }
  }
  return(2);
}


       
static void _draw_line(int x1,int y1,int x2,int y2)
{
 char *addr;
 short i,dx,dy,f,g,inc1,inc2,ab,dx1,dy1,miny,maxy,minx,maxx;
 short clip,ch,rh,r,c;
 unsigned char temp;
 int xa,ya,xb,yb;
 xa =x1,xb=x2,ya=y1,yb=y2;
 clip=clip_line(&xa,&ya,&xb,&yb);
 if (clip==0) return;
 GLN(xa,ya,xb,yb);
 return;
}

static void draw_line(int xa,int ya,int xb,int yb)
 {
   int i,i2;
   float dx,dy,ct,st,dl;
   int x1,y1,x2,y2,dx1,dy1;
    _draw_line( xa, ya, xb, yb);
   return;
 }
static void hori_line(int xa,int xb,int y)
{
  short i,clip;
  char *addr;
  int temp;
  if(xa==xb) return;
  clip=clip_line(&xa,&y,&xb,&y);
  if (xb<xa)
  {
    temp=xa;
    xa=xb;
    xb=temp;
  }
  if (clip==0) return;
  GLN(xa,y,xb,y);
}
static void phori_line(int xa,int xb,int y)
{
  short i,j,ii,ip,ib,jp,clip,pt_no;
  char *addr;
  unsigned char dot;
  int temp;
  if(xa==xb) return;
  clip=clip_line(&xa,&y,&xb,&y);
  if (xb<xa)
  {
    temp=xa;
    xa=xb;
    xb=temp;
  }
  if (clip==0) return;
  temp =c_color;
  GLN(xa,y,xb,y);
/*
#ifdef MONO  
  pt_no=(unsigned char)c_color%16;
  if(pt_no>15) pt_no=0;
  if(pt_no==15) {GLN(xa,y,xb,y);}
  else {
   j=EVGAY-y;
   jp=(j/5)%8;
   for(i=xa;i<=xb;i+=5){
    ii=i/5;
    ip=(ii)%8;ib=ip/8;ip %=8;
    ii *=5;
    dot = fil_pt[pt_no][jp][ib]&MASK[ip];
    if(dot!=0) { GPEN(temp); GLN(ii,y,ii+5,y); }
    else { GPEN(0); GLN(ii,y,ii+5,y); }
   }
   GPEN(temp);
  }
#else
  GLN(xa,y,xb,y);
#endif
*/
}

static void vert_line(int ya,int yb,int x)
{
  short i,clip;
  char *addr;
  int temp;
  if(ya==yb) return;
  clip=clip_line(&x,&ya,&x,&yb);
  if (yb<ya)
  {
    temp=ya;
    ya=yb;
    yb=temp;
  }
  if (clip==0) return;
  GLN(x,ya,x,yb);
}

static void _draw_o(int x, int y)
{
  int xa, ya, xb, yb;
  unsigned int ptn;
  short  dx, dy,  f, inc1, inc2, g,dx1,dy1,ab;
  short clip,i,ch,rh,c,r;
  float h;

  xa=cur_x;
  ya=cur_y;
  xb=x;
  yb=y;
  clip=clip_line(&xa,&ya,&xb,&yb);
   if (clip == 0)
   {
cur_x=x;
cur_y=y;
return;
   }
  GLN(xa,ya,xb,yb);
   return;
}
static void _draw_(int x, int y)
{
  int xa, ya, xb, yb;
  unsigned int ptn;
  float  dxx, dyy,xx,yy;
  short  dx, dy,  f, inc1, inc2, g,dx1,dy1,ab;
  short clip,i,ch,rh,c,r,c1,r1;
  float h;

  xa=cur_x;
  ya=cur_y;
  xb=x;
  yb=y;
   clip=clip_line(&xa,&ya,&xb,&yb);
   if (clip == 0)
   {
cur_x=x;
cur_y=y;
return;
   }
   ptn=ln_ptn[ln_style%10];
   dx=(xb-xa);
   dy=(yb-ya);
   dx1 = abs(dx);
   dy1 = abs(dy);
   i=st_ptr[ln_style%10];
   if ( dx1 > dy1 )
   {
     dxx = dx/((float)(dx1))*5.0;
     dyy = dy/((float)(dx1))*5.0;
     xx=xa+0.5+dxx;
     yy=ya+0.5+dyy;
     c=xa;r=ya;
     /*while (abs(c-xb)>5)*/
     do
      {
       i %=32;
       c1=xx;r1=yy;
       if( (ptn&(((unsigned int)0x80000000)>>i)) !=0){
        GLN(c,r,c1,r1);
       }
       c=c1,r=r1;
       xx +=dxx;yy +=dyy;
       i++;
      } while ( abs(c-xb) > 5);
   }
   else
   {
     dxx =5;dyy=0;
     if(dy1>1) {
      dxx = dx/((float)(dy1))*5.0;
      dyy = dy/((float)(dy1))*5.0;
     }
     xx=xa+0.5+dxx;
     yy=ya+0.5+dyy;
     c=xa;r=ya;
     /*while (abs(r-yb)>5)*/
     do
      {
       i %=32;
       c1=xx;r1=yy;
       if( (ptn&(((unsigned int)0x80000000)>>i)) !=0){
       GLN(c,r,c1,r1);
       }
       c=c1,r=r1;
       xx +=dxx;yy +=dyy;
       i++;
      } while ( abs(r-yb) > 5);
   }
   st_ptr[ln_style%10]=i;
   return;
}

static void _draw(int x, int y)
 {
   int i,i2,stloc;
   float dx,dy,ct,st,dl;
   int x1,y1,xa,ya,xb,yb,dx1,dy1;
   xa=cur_x;
   ya=cur_y;
   xb=x;
   yb=y;
   if ( ln_style == 0) {draw_line(xa,ya,xb,yb);}
   else
    {
     _draw_( x, y);
    }
   cur_x=x;
   cur_y=y;
   return;
 }
static void txt_size(void)
{
  float ht, wt,sp;
  char ch;
  read_buf(&ht,4);
  read_buf(&wt,4);
  read_buf(&sp,4);
  read_buf(&txt_htx,4);
  read_buf(&txt_wty,4);
  read_buf(&txt_spy,4);
  txt_hty=ht;
  txt_wtx=wt;
  txt_spx=sp;
  txt_htx=ht/(w_y2-w_y1)*(w_x2-w_x1);
  txt_wty=wt/(w_x2-w_x1)*(w_y2-w_y1);
  txt_spy=sp/(w_x2-w_x1)*(w_y2-w_y1);
  txt_wt= sint*sint*txt_wty*(w_x2-w_x1)/(w_y2-w_y1)+cost*cost*txt_wtx;
  txt_sp= sint*sint*txt_spy*(w_x2-w_x1)/(w_y2-w_y1)+cost*cost*txt_spx;
  txt_ht= sint*sint*txt_htx/(w_x2-w_x1)*(w_y2-w_y1)+cost*cost*txt_hty;
  txt_ht=txt_ht*V_y/((w_y2-w_y1));
  txt_wt=txt_wt*V_x/((w_x2-w_x1));
  txt_sp=txt_sp*V_x/((w_x2-w_x1));
  v_xt1=(int)(v_x1-txt_wt-txt_sp);
  v_yt1=(int)(v_y1-txt_ht);
  txt_w42 = txt_wt/CFact;
  txt_h42 = txt_ht/CFact;
  return;
}
static void win_txt_size(void)
{
  float ht;
  float wt;
  float sp;
  float fact,xfact,yfact;
  read_buf(&txt_hty,4);
  read_buf(&txt_wtx,4);
  read_buf(&txt_spx,4);
  read_buf(&txt_htx,4);
  read_buf(&txt_wty,4);
  read_buf(&txt_spy,4);
  xfact = (w_x2-w_x1)/x_mulf;
  yfact = (w_y2-w_y1)/y_mulf;
  fact = xfact/yfact;
  txt_wt= sint*sint*txt_wty*fact+cost*cost*txt_wtx;
  txt_sp= sint*sint*txt_spy*fact+cost*cost*txt_spx;
  txt_ht= sint*sint*txt_htx/fact+cost*cost*txt_hty;
  txt_ht=txt_ht*V_y/(w_y2-w_y1);
  txt_wt=txt_wt*V_x/(w_x2-w_x1);
  txt_sp=txt_sp*V_x/(w_x2-w_x1);
  v_xt1=(int)(v_x1-txt_wt-txt_sp);
  v_yt1=(int)(v_y1-txt_ht);
  size_font_correction();
  txt_w42 = txt_wt/CFact;
  txt_h42 = txt_ht/CFact;
  return;
}
static void win_txt_slant( void)
 {
  float  slant;
  read_buf(&slant,4);
/*
  Slant=tan((double)slant*3.14159265/180.0);
  Slant_o=Slant;
*/
 }
static void win_txt_color( void)
 {
  int  color;
  read_buf(&color,4);
#ifdef MONO
  if(color != 0 ) color =15;
#endif
  t_color= color;
  fprintf(TX_F,"Zc%d\n",t_color);
 }
static void win_txt_fill( void)
 {
  int  color;
  read_buf(&color,4);
  t_fill= color;
  fprintf(TX_F,"Zf%d\n",t_fill);
 }
static void win_txt_bodr( void)
 {
  int  color;
  read_buf(&color,4);
  t_bodr= color;
  fprintf(TX_F,"Zb%d\n",t_bodr);
 }
static void win_txt_bkgr( void)
 {
  int  color;
  read_buf(&color,4);
  t_bkgr= color;
 }
static void win_txt_pattern( void)
 {
  int  color;
  read_buf(&color,4);
  t_pattern= color;
  fprintf(TX_F,"Zp%d\n",t_pattern);
 }
#if 0
static int Loadfontstruct(void) {
  FONT *pt;
  Fontlist = Dopen();
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf0;
  pt->m_f = m_f0;
  pt->icxv = icxvf0;
  pt->icyv = icyvf0;
  pt->fontname = fontname0;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf1; pt->m_f = m_f1; pt->icxv = icxvf1;
  pt->icyv = icyvf1; pt->fontname = fontname1;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf2; pt->m_f = m_f2; pt->icxv = icxvf2;
  pt->icyv = icyvf2; pt->fontname = fontname2;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf3; pt->m_f = m_f3; pt->icxv = icxvf3;
  pt->icyv = icyvf3; pt->fontname = fontname3;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf4; pt->m_f = m_f4; pt->icxv = icxvf4;
  pt->icyv = icyvf4; pt->fontname = fontname4;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf5; pt->m_f = m_f5; pt->icxv = icxvf5;
  pt->icyv = icyvf5; pt->fontname = fontname5;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf6; pt->m_f = m_f6; pt->icxv = icxvf6;
  pt->icyv = icyvf6; pt->fontname = fontname6;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf7; pt->m_f = m_f7; pt->icxv = icxvf7;
  pt->icyv = icyvf7; pt->fontname = fontname7;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf8; pt->m_f = m_f8; pt->icxv = icxvf8;
  pt->icyv = icyvf8; pt->fontname = fontname8;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf9; pt->m_f = m_f9; pt->icxv = icxvf9;
  pt->icyv = icyvf9; pt->fontname = fontname9;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf10; pt->m_f = m_f10; pt->icxv = icxvf10;
  pt->icyv = icyvf10; pt->fontname = fontname10;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf11; pt->m_f = m_f11; pt->icxv = icxvf11;
  pt->icyv = icyvf11; pt->fontname = fontname11;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf12; pt->m_f = m_f12; pt->icxv = icxvf12;
  pt->icyv = icyvf12; pt->fontname = fontname12;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf13; pt->m_f = m_f13; pt->icxv = icxvf13;
  pt->icyv = icyvf13; pt->fontname = fontname13;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf14; pt->m_f = m_f14; pt->icxv = icxvf14;
  pt->icyv = icyvf14; pt->fontname = fontname14;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf15; pt->m_f = m_f15; pt->icxv = icxvf15;
  pt->icyv = icyvf15; pt->fontname = fontname15;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf16; pt->m_f = m_f16; pt->icxv = icxvf16;
  pt->icyv = icyvf16; pt->fontname = fontname16;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf17; pt->m_f = m_f17; pt->icxv = icxvf17;
  pt->icyv = icyvf17; pt->fontname = fontname17;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf18; pt->m_f = m_f18; pt->icxv = icxvf18;
  pt->icyv = icyvf18; pt->fontname = fontname18;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf19; pt->m_f = m_f19; pt->icxv = icxvf19;
  pt->icyv = icyvf19; pt->fontname = fontname19;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf20; pt->m_f = m_f20; pt->icxv = icxvf20;
  pt->icyv = icyvf20; pt->fontname = fontname20;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf21; pt->m_f = m_f21; pt->icxv = icxvf21;
  pt->icyv = icyvf21; pt->fontname = fontname21;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf22; pt->m_f = m_f22; pt->icxv = icxvf22;
  pt->icyv = icyvf22; pt->fontname = fontname22;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf23; pt->m_f = m_f23; pt->icxv = icxvf23;
  pt->icyv = icyvf23; pt->fontname = fontname23;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf24; pt->m_f = m_f24; pt->icxv = icxvf24;
  pt->icyv = icyvf24; pt->fontname = fontname24;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf25; pt->m_f = m_f25; pt->icxv = icxvf25;
  pt->icyv = icyvf25; pt->fontname = fontname25;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf26; pt->m_f = m_f26; pt->icxv = icxvf26;
  pt->icyv = icyvf26; pt->fontname = fontname26;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf27; pt->m_f = m_f27; pt->icxv = icxvf27;
  pt->icyv = icyvf27; pt->fontname = fontname27;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf28; pt->m_f = m_f28; pt->icxv = icxvf28;
  pt->icyv = icyvf28; pt->fontname = fontname28;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf29; pt->m_f = m_f29; pt->icxv = icxvf29;
  pt->icyv = icyvf29; pt->fontname = fontname29;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf30; pt->m_f = m_f30; pt->icxv = icxvf30;
  pt->icyv = icyvf30; pt->fontname = fontname30;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf31; pt->m_f = m_f31; pt->icxv = icxvf31;
  pt->icyv = icyvf31; pt->fontname = fontname31;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf32; pt->m_f = m_f32; pt->icxv = icxvf32;
  pt->icyv = icyvf32; pt->fontname = fontname32;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf33; pt->m_f = m_f33; pt->icxv = icxvf33;
  pt->icyv = icyvf33; pt->fontname = fontname33;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf34; pt->m_f = m_f34; pt->icxv = icxvf34;
  pt->icyv = icyvf34; pt->fontname = fontname34;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf35; pt->m_f = m_f35; pt->icxv = icxvf35;
  pt->icyv = icyvf35; pt->fontname = fontname35;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf36; pt->m_f = m_f36; pt->icxv = icxvf36;
  pt->icyv = icyvf36; pt->fontname = fontname36;
  Dadd(Fontlist,pt);
  return 1;
}
#endif

static void win_txt_font( void)
 {
  int font;
  FONT *pt;
  int count;
  read_buf(&font,4);
  if(Fontlist == NULL ) Fontlist=(Dlink *)Loadfontstruct();
  count = Dcount(Fontlist);
  if(font >= count ) font =0;
  Dposition(Fontlist,font+1);
  pt = (FONT *)Getrecord(Fontlist);
  icpos = pt->icpos;
  icxv = pt->icxv;
  icyv = pt->icyv;
  m_f =  pt->m_f;
  t_font =font;
  strcpy(FontName,pt->fontname);
}
static void set_txt_font( int font)
 {
  int t_font;
  FONT *pt;
  int count;
  if(Fontlist == NULL ) Fontlist=(Dlink *)Loadfontstruct();
  count = Dcount(Fontlist);
  if(font >= count ) font =0;
  Dposition(Fontlist,font+1);
  pt = (FONT *)Getrecord(Fontlist);
  icpos = pt->icpos;
  icxv = pt->icxv;
  icyv = pt->icyv;
  m_f =  pt->m_f;
  t_font =font;
  strcpy(FontName,pt->fontname);
  fprintf(TX_F,"ZF%-s findfont %-6.1f scalefont setfont\n",FontName,font_size);
 }
static void t_txt_font( int font)
 {
  FONT *pt;
  int count;
  if(Fontlist == NULL ) Fontlist=(Dlink *)Loadfontstruct();
  count = Dcount(Fontlist);
  if(font >= count ) font =0;
  Dposition(Fontlist,font+1);
  pt = (FONT *)Getrecord(Fontlist);
  icpos = pt->icpos;
  icxv = pt->icxv;
  icyv = pt->icyv;
  m_f =  pt->m_f;
  t_font =font;
  strcpy(FontName,pt->fontname);
 }



static void drawg0ch_o(c)
char c;
{
  float x[300],y[300];
  int x1[1500],y1[1500],x2[1500],y2[1500];
  int code,n,i,xdum,ydum,pnts;
  short ptr;
  short j,pnt,asc,tempc;
  float fact=1.0;
  int xo,yo;
  tempc=ln_width;
  ln_width=txt_bold;
  if(ln_width!=tempc) GTW(ln_width);
/*
  txt_w42=txt_wt/CFact;
  txt_h42=txt_ht/CFact;
*/
  asc=c;
  pnt=asc-32+greek;
  fact = m_f[pnt];
  xo=(int)(cx+(xp));
  if( pr_txt==0) { fact=1.0;}
  else xo -=(1.-fact)*0.5*txt_wt;
  yo=(int)(cy+yp);
  yo=(int)(cy+yp);
  if (pnt<0) return;
  if(pnt>223) return;
  ptr= icpos[pnt];
  if((xo>=c_v_x1)&&(xo<=c_v_x2)&& (yo>=c_v_y1)&& (yo<=c_v_y2))
     {
     code= icxv[ptr];n=icyv[ptr];
     pnts=0;
     while ( code >= 0)
      {
        ptr++;
        for(i=0;i<n;i++){xdum=icxv[ptr];ydum=icyv[ptr];ptr++;
                         x[i]=xdum*txt_w42+xo;y[i]=yo+ydum*txt_h42;}
        pnts +=t_fillchr(x,y,n,&x1[pnts],&y1[pnts],&x2[pnts],&y2[pnts]);
        code= icxv[ptr];n=icyv[ptr];
        if(code !=0)
         {
/*
*/
         if(t_fill)
          {
            if(t_pattern==15)
             _mpoly_fill((short)pnts,x1,y1,x2,y2,(unsigned char)t_color);
            else 
             _mpoly_fill((short)pnts,x1,y1,x2,y2,(unsigned char)t_pattern);
          }
         if(t_bodr) 
            {
             temp =c_color;set_clr(t_color);
             GPEN(c_color);
             for(i=0;i<pnts;i++)draw_line(x1[i],y1[i],x2[i],y2[i]);
             set_clr(temp);
             GPEN(temp);
            }
          pnts=0;
         }
      }
     }
     xp+=(txt_wt*fact+txt_sp);
     if(ln_width!=tempc) GTW(tempc);
     ln_width=tempc;
     return;
 }
static void drawg0ch_n(c)
char c;
{
  float x[300],y[300];
  int x1[1500],y1[1500],x2[1500],y2[1500];
  int code,n,i,xdum,ydum,pnts;
  short ptr;
  short j,pnt,asc,tempc;
  float fact=1.0;
  int xo,yo;
  asc=c;
  pnt=asc-32+greek;
  fact = m_f[pnt];
  xo=(int)(cx+(xp));
  if( pr_txt==0) { fact=1.0;}
  /*else xo -=(1.-fact)*0.5*txt_wt;*/
  yo=(int)(cy+yp);
  if (pnt<0) return;
  if(pnt>223) return;
  ptr= icpos[pnt];
  if((xo>=c_v_x1)&&(xo<=c_v_x2)&& (yo>=c_v_y1)&& (yo<=c_v_y2))
     {
        GTX(xo,yo,c);
     }
  xp+=(txt_wt*fact+txt_sp);
  return;
 }
static void drawgch_o(c)
char c;
{
  float x[300],y[300];
  int code,n,i;
  int x1[1500],y1[1500],x2[1500],y2[1500];
  int pnts;
  float xdum,ydum;
  short ptr;
  short j,pnt,asc,tempc;
  float xa,ya;
  float fact=1.0;
  float xo,yo;
  tempc=ln_width;
  ln_width=txt_bold;
  if(ln_width!=tempc) GTW(ln_width);
  asc=c;
  pnt=asc-32+greek;
  fact = m_f[pnt];
  xo=(int)((xp));
  if( pr_txt==0) { fact=1.0;}
  else xo -=(1.-fact)*0.5*txt_wt;
  yo=(int)(yp);
  if (pnt<0) return;
  if(pnt>223) return;
  ptr= icpos[pnt];
  xa=(cx+(xo)*cost+yo*sint);
  ya=(cy-(xo)*sint+yo*cost);
  if((xa>=c_v_x1)&&(xa<=c_v_x2)&& (ya>=c_v_y1)&& (ya<=c_v_y2))
     {
     code= icxv[ptr];n=icyv[ptr];
     pnts=0;
     while ( code >= 0)
      {
        ptr++;
        for(i=0;i<n;i++){xdum=icxv[ptr];ydum=icyv[ptr];ptr++;
     xdum=(xo+xdum*txt_w42);
     ydum=(yo+ydum*txt_h42);
     x[i]=(cx+(xdum)*cost+(ydum)*sint);
     y[i]=(cy+(ydum)*cost-(xdum)*sint);
            }
        pnts +=t_fillchr(x,y,n,&x1[pnts],&y1[pnts],&x2[pnts],&y2[pnts]);
        code= icxv[ptr];n=icyv[ptr];
        if(code !=0)
         {
/*
*/
         if(t_fill)
          {
            if(t_pattern==15)
             _mpoly_fill((short)pnts,x1,y1,x2,y2,(unsigned char)t_color);
            else 
             _mpoly_fill((short)pnts,x1,y1,x2,y2,(unsigned char)t_pattern);
          }
         if(t_bodr)
            {
             temp =c_color;set_clr(t_color);
             GPEN(t_color);
             for(i=0;i<pnts;i++)draw_line(x1[i],y1[i],x2[i],y2[i]);
             set_clr(temp);
             GPEN(temp);
            }
          pnts=0;
         }
      }
     }
     xp+=(txt_wt*fact+txt_sp);
     if(ln_width!=tempc) GTW(tempc);
     ln_width=tempc;
     return;
 }
static void drawgch_n(c)
char c;
{
  float x[300],y[300];
  int code,n,i;
  int x1[1500],y1[1500],x2[1500],y2[1500];
  int pnts;
  float xdum,ydum;
  short ptr;
  short j,pnt,asc,tempc;
  float xa,ya;
  float fact=1.0;
  float xo,yo;
  asc=c;
  pnt=asc-32+greek;
  fact = m_f[pnt];
  xo=(int)((xp));
  if( pr_txt==0) { fact=1.0;}
  yo=(int)(yp);
  if (pnt<0) return;
  if(pnt>223) return;
  ptr= icpos[pnt];
  xa=(cx+(xo)*cost+yo*sint);
  ya=(cy-(xo)*sint+yo*cost);
  if((xa>=c_v_x1)&&(xa<=c_v_x2)&& (ya>=c_v_y1)&& (ya<=c_v_y2))
  {
        GTX((int)xa,(int)ya,c);
  }
     xp+=(txt_wt*fact+txt_sp);
     return;
 }
static void drawgch(char c)
{
 if( greek>0) {
  set_txt_font(0);
  drawgch_o(c);
  set_txt_font(t_font);
  return;
 }
 drawgch_n(c);
/*
 if(t_font>3) drawgch_n(c);
 else drawgch_o(c);
*/
}
static void drawg0ch(char c)
{
 if( greek>0) {
  set_txt_font(0);
  drawg0ch_o(c);
  set_txt_font(t_font);
  return;
 }
 drawg0ch_n(c);
/*
 if(t_font>3) drawg0ch_n(c);
 else drawg0ch_o(c);
*/
}
static int berzier(int *xp,int *yp,float x1,float y1,float x4,float y4,float x2,
          float y2,float x3,float y3)
   {
     float t,tm,x,y,xm,ym,dist,dt;
     int n,notok,i;
     n=2;
     notok=1;
     while((n<80)&&notok)
     {
       t=1.0/n;
       tm=1.-t;
       x= x1*tm*tm*tm + 3.*x2*tm*tm*t+3.*x3*tm*t*t+x4*t*t*t;
       y= y1*tm*tm*tm + 3.*y2*tm*tm*t+3.*y3*tm*t*t+y4*t*t*t;
       dist = (x-x1)*(x-x1)+(y-y1)*(y-y1);
       if(dist>2.0) n++;
       else notok=0;
     }
     t=0;dt=1.0/n;
     for(i=1;i<(n);i++)
     {
      t+=dt;
      tm=1.-t;
      x= x1*tm*tm*tm + 3.*x2*tm*tm*t+3.*x3*tm*t*t+x4*t*t*t;
      y= y1*tm*tm*tm + 3.*y2*tm*tm*t+3.*y3*tm*t*t+y4*t*t*t;
      *xp=x+0.5,*yp=y+0.5;
      xp++,yp++;
     }
      *xp=x4+0.5,*yp=y4+0.5;
      xp++,yp++;
     return(n);
   }
static int t_fillchr(float *x,float *y,int n,int *x1,int *y1,int *x2,int*y2)
  {
  int xp,yp,temp;
  float xx,yy,xx0,yy0,xx1,xx2,yy1,yy2,xx3,xx4,yy3,yy4;
  short i,j,j1,k;
  j=0;
  xx0=x[0]; yy0=y[0];
  x1[j] =(xx0); y1[j] =(yy0); j++;
  i=1;
  while(i<n)
  {
    xx =x[i]; yy =y[i]; i++;
    xx1 =x[i]; yy1 =y[i];i++;
    j1=j-1;
    if((xx0==xx1)&&(yy0==yy1))
        {
         xp=(xx);yp=(yy);
         if((xp!=x1[j1])||(yp!=y1[j1])){x1[j]=xp+0.5;y1[j]=yp+0.5; j++;};
        }
    else
        {
         xp=(xx);yp=(yy);
         if((xp!=x1[j1])||(yp!=y1[j1]))
            {
            xx2=(xx1);yy2=(yy1);
            xx1=x1[j1];yy1=y1[j1];
            xx4=xp,yy4=yp;
            xx3 =(x[i]); yy3 =(y[i]);i++;
            j +=berzier(&x1[j],&y1[j],xx1,yy1,xx4,yy4,xx2,yy2,xx3,yy3);
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
static void t_fillchr_o(float *x,float *y,int code,int n)
  {
  int xp,yp,temp;
  int x1[300],y1[300];
  float xx,yy,xx0,yy0,xx1,xx2,yy1,yy2,xx3,xx4,yy3,yy4;
  short i,j,j1;
  j=0;
  xx0=x[0]; yy0=y[0];
  x1[j] =(xx0); y1[j] =(yy0); j++;
  i=1;
  while(i<n)
  {
    xx =x[i]; yy =y[i]; i++;
    xx1 =x[i]; yy1 =y[i];i++;
    j1=j-1;
    if((xx0==xx1)&&(yy0==yy1))
        {
         xp=(xx);yp=(yy);
         if((xp!=x1[j1])||(yp!=y1[j1])){x1[j]=xp;y1[j]=yp; j++;};
        }
    else
        {
         xp=(xx);yp=(yy);
         if((xp!=x1[j1])||(yp!=y1[j1]))
            {
            xx2=(xx1);yy2=(yy1);
            xx1=x1[j1];yy1=y1[j1];
            xx4=xp,yy4=yp;
            xx3 =(x[i]); yy3 =(y[i]);i++;
            j +=berzier(&x1[j],&y1[j],xx1,yy1,xx4,yy4,xx2,yy2,xx3,yy3);
            }
         else i++;
        }
    xx0=xx;yy0=yy;
  }
  if(t_fill)
   {
   if(code==0) _poly_fill((short)j,x1,y1,(short)0,(unsigned char)t_bkgr);
   else _poly_fill((short)j,x1,y1,(short)0,(unsigned char)t_pattern);
   }
  if(t_bodr){
             temp =c_color;set_clr(t_color);
             GPEN(t_color);
             _move(x1[0],y1[0]);
             for(i=1;i<j;i++)_draw(x1[i],y1[i]);
             _draw(x1[0],y1[0]);
             set_clr(temp);
             GPEN(temp);
            }
/*
*/
  return;
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

static   void setsubsup(fact,ishft,cntl)
  char cntl;
  float *fact;
  short *ishft;
      {
      xp = xp -0.5*txt_sp;
      *fact = (*fact)*0.6;
      txt_w42 = 0.6*txt_w42;
      txt_h42 = 0.6*txt_h42;
      txt_wt = 0.6*txt_wt;
      txt_ht = 0.6*txt_ht;
      if( cntl=='S'){
                      *ishft = (*ishft)+1;
                      yp = yp+1.4*txt_ht;
                    }
       else yp= yp-0.7*txt_ht;
      *ishft = (*ishft)*10;
      return;
      }
static   void resetsubsup(fact,ishft)
  float *fact;
  short *ishft;
   {
     short is;
      xp = xp +0.5*txt_sp;
      *ishft = (*ishft)/10;
      is = *ishft-((*ishft)/10)*10;
      if( is==1){
                      *ishft = (*ishft)-1;
                      yp = yp-1.4*txt_ht;
                }
      else yp= yp+0.7*txt_ht;
      *fact = (*fact)/0.6;
      txt_w42 = txt_w42/0.6;
      txt_h42 = txt_h42/0.6;
      txt_wt = txt_wt/0.6;
      txt_ht = txt_ht/0.6;
      return;
   }

static void  win_txtwrt(void) {
 
  short i=0,bold,tempc,ishft,trot,Nu,De,gap,lnwidth_o;
  float fact,val,xl1,xl2,hfact=1.0,slant;
  int font_o;
  L_N *FO_L=NULL,*pt=NULL;
  B_K *FB_P=NULL;
  char ch,cntl;
  int nchr;
  unsigned char *txt;
  if( (t_font!=t_font_o) || (font_size!=font_size_o)) {
    fprintf(TX_F,"ZF%-s findfont %-6.1f scalefont setfont\n",
                  FontName,font_size);
    t_font_o=t_font;
    font_size_o=font_size;
  }
  font_o=t_font;
  read_buf(&nchr,4);
  txt= (unsigned char *) malloc((nchr+1)*sizeof(unsigned char));
  read_buf(txt,nchr);
  txt[nchr]='\0';
  bold=txt_bold;
  O_L=NULL;
  bold = txt_bold;
  slant=Slant;
  trot = (cost<0.999999);
  tempc=c_color;
  c_color = t_color;
  GPEN(t_color);
  GPEN(t_color);
  cx=(int)(cur_x+0.5);
  cy= (int)(cur_y+0.5);
  xp=0.0;
  yp =0.0;
  fact =1.0;
  hfact =1.0;
  ishft =0;
  greek=0;
  while (txt[i]!='\0')
  {
        {
         if ( txt[i]!= '!') { if( trot) drawgch(txt[i]);
                              else drawg0ch(txt[i]);
                              greek=0;
                            }
         else
           {
           i++;
           cntl = txt[i];
           if( (cntl=='S')|| (cntl=='s') ){
                 setsubsup(&fact,&ishft,cntl);
                 change_size_font();
           }
           else
             {
               switch (cntl)
                 {
                   case 'e':
                            resetsubsup(&fact,&ishft);
                            change_size_font();
                            break;
                   case '!':
                            greek=0;
                            if(trot)  drawgch(txt[i]);
                            else drawg0ch(txt[i]);
                            break;
                   case 'b':
                            xp =xp -txt_wt -txt_sp;
                            break;
                   case 'B':
                            txt_bold=2;
                            break;
                   case 'I':
                            Slant=Slnt[1];
                            break;
                   case 'N':
                            Slant = Slant_o; txt_bold=txt_bold_o;
                            if(pt!=NULL){
                             while( (pt->x2) >=0.) pt=pt->Pr;
                             (pt->x2)=xp-txt_sp;
                             pt=pt->Pr;
                            }
                            break;
                   case 'g':
                            greek=128;
                            break;
                   case 'r':
                            if(O_P!=NULL){
                              xp=O_P->x;
                              yp=O_P->y;
                              D_P=O_P;
                              O_P=O_P->Pr;
                              free(D_P);
                              if(O_P==NULL) FB_P=NULL;
                            }
                            break;
                   case 'k':
                            if(FB_P==NULL) {
                              FB_P=(B_K *) malloc((int)sizeof(B_K));
                              O_P=FB_P;
                              O_P->Nx=NULL;O_P->Pr=NULL;
                            }
                            else {
                              O_P->Nx=(B_K *) malloc((int)sizeof(B_K));
                              O_P->Nx->Pr=O_P;
                              O_P=O_P->Nx;
                              O_P->Nx=NULL;
                            }
                            (O_P->x) = xp;
                            (O_P->y) = yp;
                            break;
                   case 'x':
                            yp +=0.2*txt_ht;
                            break;
                   case 'y':
                            yp -=0.2*txt_ht;
                            break;
                   case 'u':
                            yp +=0.9*txt_ht;
                            break;
                   case 'd':
                            yp -=0.9*txt_ht;
                            break;
                   case 'O':
                   case 'U':
                            if(FO_L==NULL) {
                              FO_L=(L_N *) malloc((int)sizeof(L_N));
                              O_L=FO_L;
                              O_L->Nx=NULL;O_L->Pr=NULL;
                            }
                            else {
                              O_L->Nx=(L_N *) malloc((int)sizeof(L_N));
                              O_L->Nx->Pr=O_L;
                              O_L=O_L->Nx;
                              O_L->Nx=NULL;
                            }
                            (O_L->x1) = xp;
                            (O_L->x2) = -1.0;
                            O_L->ymax = yp+1.4*txt_ht;
                            O_L->ymin = yp-0.4*txt_ht;
                            O_L->p=cntl;
                            pt=O_L;
                            break;
                   case '%':
                            ch=txt[i+1];txt[i+1]='\0';
                            gap=strlngth((char *)txt,&xl2);
                            txt[i+1]=ch;
                            xp = (gap+1)*(txt_wt+txt_sp);
                            break;
                   case 'f':
                            Nu= (txt[i+1] -'0')*10+(txt[i+2]-'0');
                            i+=2;
                            t_txt_font((int)Nu);
                            change_size_font();
                            break;
                   case 'z':
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            fact = fact*val;
                            hfact =hfact*val;
                            i +=2;
                            txt_w42 = txt_w42*val;
                            txt_h42 = txt_h42*val;
                            txt_wt = txt_wt*val;
                            txt_ht = txt_ht*val;
                            change_size_font();
                            break;
                   case 'h':
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            hfact =hfact*val;
                            i +=2;
                            txt_h42 = txt_h42*val;
                            txt_ht = txt_ht*val;
                            change_size_font();
                            break;
                   case 'w':
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            fact = fact*val;
                            i +=2;
                            txt_w42 = txt_w42*val;
                            txt_wt = txt_wt*val;
                            change_size_font();
                            break;
                   default :
                            break;
                 }
             }
         }
        i++;
        }
  }                
      txt_w42 = txt_w42/fact;
      txt_h42 = txt_h42/hfact;
      txt_wt = txt_wt/fact;
      txt_ht = txt_ht/hfact;
      if( (fact!=1.) ||(hfact!=1.)) change_size_font();
/*    txt_bold=bold;*/
      c_color= tempc;
      GPEN(c_color);
      pt=FO_L;
      if(pt!=NULL) {
       lnwidth_o=ln_width;
       ln_width=2;
       GTW(ln_width);
       while (pt!=NULL){
        if(pt->x2 <0.) pt->x2=xp;
        if(pt->p =='U') {
         _move(TX((pt->x1),pt->ymin),TY((pt->x1),pt->ymin));
         _draw(TX((pt->x2),pt->ymin),TY((pt->x2),pt->ymin));
        }
        else{
         _move(TX((pt->x1),pt->ymax),TY((pt->x1),pt->ymax));
         _draw(TX((pt->x2),pt->ymax),TY((pt->x2),pt->ymax));
        }
        O_L=pt;
        pt=pt->Nx;
        free(O_L);
       }
       ln_width=lnwidth_o;
       GTW(ln_width);
      }
      O_P=FB_P;
      while(O_P != NULL) {
       D_P=O_P;
       O_P=O_P->Pr;
       free(D_P);
      }
      if(t_font!=font_o) t_txt_font(font_o);
      free(txt);
  }
static void  win_txtwrt_o(void)
{
  short i=0,bold,j,pnt,tempc,ishft,trot,Nu,De,gap;
  float fact,val,xl1,xl2;
  int nchr;
  unsigned char txt[200],cntl;
  unsigned char *tx;
  if( (t_font!=t_font_o) || (font_size!=font_size_o)) {
    fprintf(TX_F,"ZF%-s findfont %-6.1f scalefont setfont\n",
                  FontName,font_size);
    t_font_o=t_font;
    font_size_o=font_size;
  }
  tx= (unsigned char *) txt;
  read_buf(&nchr,4);
  read_buf(tx,nchr);
  txt[nchr]='\0';
  bold=txt_bold;
  tempc=c_color;
  c_color = t_color;
  GPEN(t_color);
  trot = (cost<0.99);
  cx=cur_x+0.5;
  cy=cur_y+0.5;
  xp=0.0;
  yp =0.0;
  fact =1.0;
  ishft =0;
  greek=0;
  while (txt[i]!='\0')
  {
        {
         if ( txt[i]!= '!') { if( trot) drawgch(txt[i]);
                              else drawg0ch(txt[i]);
                              greek=0;
                            }
         else
           {
           i++;
           cntl = txt[i];
           if( (cntl=='S')|| (cntl=='s')) {
                 setsubsup(&fact,&ishft,cntl);
                 change_size_font();
           }
           else
             {
               switch (cntl)
                 {
                   case 'e':
                            resetsubsup(&fact,&ishft);
                            change_size_font();
                            break;
                   case '!':
                            if(trot)  drawgch(txt[i]);
                            else drawg0ch(txt[i]);
                            break;
                   case 'b':
                            xp =xp -txt_wt -txt_sp;
                            break;
                   case 'B':
                            txt_bold=2;
                            break;
                   case 'N':
                            txt_bold=1;
                            break;
                   case 'g':
                            greek=128;
                            break;
                   case '%':
                            ch=txt[i+1];txt[i+1]='\0';
                            gap=strlngth((char *)txt,&xl2);
                            txt[i+1]=ch;
                            xp = (gap+1)*(txt_wt+txt_sp);
                            break;
                   case 'z':
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            fact = fact*val;
                            i +=2;
                            txt_w42 = txt_w42*val;
                            txt_h42 = txt_h42*val;
                            txt_wt = txt_wt*val;
                            txt_ht = txt_ht*val;
                            change_size_font();
                            break;
                   default :
                            break;
                 }
             }
         }
        i++;
        }
    }              
      txt_wt = txt_wt/fact;
      txt_ht = txt_ht/fact;
      txt_w42 = txt_w42/fact;
      txt_h42 = txt_h42/fact;
      c_color= tempc;
      GPEN(c_color);
      txt_bold=bold;
} 

static void _box_fill(int x1,int y1,int x2,int y2,unsigned char color)
{
  int x_min,y_min,x_max,x_max1,x_min1,y_max,i,j;
  short po1;
  unsigned char temp;
  int width=5;
  temp=c_color;
  c_color=color;
 /* GTW(width);*/
  GPEN(c_color);
  if( x1<x2 ) { x_min= x1; x_max1= x2; }
  else{ x_min= x2; x_max1= x1; }
  if(( x_min<=c_v_x2) && (x_max1>=c_v_x1))
   {
    if( y1<y2 ) { y_min= y1; y_max= y2; }
    else{ y_min= y2; y_max= y1; }
       
    if(( y_min<=c_v_y2) && (y_max>=c_v_y1))
     {
      if(x_max1>c_v_x2) x_max1 = c_v_x2;
      if(x_min<c_v_x1) x_min = c_v_x1;
      if(y_max>c_v_y2) y_max = c_v_y2;
      if(y_min<c_v_y1) y_min = c_v_y1;
      for (j=y_min;j<=y_max;j=j+5) phori_line(x_min,x_max1,j);
    }
   }
  c_color=temp;
  GPEN(c_color);
/*GTW(ln_width);*/
  return;
}
static void _box_fill_ps(int x1,int y1,int x2,int y2,unsigned char color)
{
  int x_min,y_min,x_max,x_max1,x_min1,y_max,i,j;
  short po1;
  unsigned char temp;
  int width=5;
  temp=c_color;
  c_color=color;
 /* GTW(width);*/
  GPEN(c_color);
  if( x1<x2 ) { x_min= x1; x_max1= x2; }
  else{ x_min= x2; x_max1= x1; }
  if(( x_min<=c_v_x2) && (x_max1>=c_v_x1))
   {
    if( y1<y2 ) { y_min= y1; y_max= y2; }
    else{ y_min= y2; y_max= y1; }
       
    if(( y_min<=c_v_y2) && (y_max>=c_v_y1))
     {
      if(x_max1>c_v_x2) x_max1 = c_v_x2;
      if(x_min<c_v_x1) x_min = c_v_x1;
      if(y_max>c_v_y2) y_max = c_v_y2;
      if(y_min<c_v_y1) y_min = c_v_y1;
      GMV(x_min,y_min);
      GDR(x_max1,y_min);
      GDR(x_max1,y_max);
      GDR(x_min,y_max);
      GDR(x_min,y_min);
      GFL(color);
/*
      for (j=y_min;j<=y_max;j=j+5) phori_line(x_min,x_max1,j);
*/
     }
   }
  c_color=temp;
  GPEN(c_color);
/*GTW(ln_width);*/
  return;
}


static void clear_screen()
{
 _box_fill(v_x1,v_y1,v_x2,v_y2,0x00);
 return;
}


static void _poly_fill(short n,int *px,int *py,short flag,unsigned char b_color)
{
  short j,imd;
  short i,cnt,ind,pcrl=0;
  int xcord[15];
  int xlow,xup;
  int txx;
  int ymax[300],ymin[300],xmax[300],xmin[300],tyx,tyn,txn,scan,ygrt;
  unsigned char tempc;
  int width=5;
  xup =-100000;
  xlow = 1000000;
  scan=100000;
  ygrt=-1239000;
  /*GTW(width);*/
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
  if((scan> c_v_y2) || (ygrt< c_v_y1) || (xlow> c_v_x2) || (xup< c_v_x1)) return;
  /*if (b_color==0) goto skip;*/
  tempc = c_color;
  c_color= b_color;
  GPEN(c_color);
  if (ymax[ind]==scan) ind++;
  while (scan<=ygrt)
  {
         pcrl++;
 cnt=0;
 while (ymax[ind]==scan) ++ind;  /*  excluding upper edges  */
         if((pcrl%5)==0){
          for(i=ind;i<n;++i) {
      if (ymin[i]<=scan) {
     if (ymax[i]!=ymin[i])
 {
                   xcord[cnt]=(((float)(xmax[i]-xmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +xmin[i])+0.5);
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
          /*if((pcrl%10)==0) for (i=0;i<cnt;i+=2)*/
          for (i=0;i<cnt;i+=2)
               phori_line(xcord[i],xcord[i+1],scan);
         }
 scan++;
  }
  c_color=tempc;
  GPEN(c_color);
skip:
/*  GTW(ln_width);*/
  if (flag==1)
      for(i=0;i<n;i++) draw_line(px[i],py[i],px[(i+1)%n],py[(i+1)%n]);
}
static void _poly_fill_ps(short n,int *px,int *py,short flag,unsigned char b_color)
{
  short j,imd;
  short i,cnt,ind,pcrl=0;
  int xcord[25];
  int xlow,xup;
  int txx;
  unsigned char tempc;
  int width=5;
  tempc=c_color;
  c_color = b_color;
  GPEN(c_color);
  GMV(px[n-1],py[n-1]);
  for(i=0;i<n;i++) GDR(px[i],py[i]);
  GFL(b_color);
  fprintf(TX_F,"ZCgs col%3.3d s gr n\n",b_color);
skip:
  c_color=tempc;
  GPEN(c_color);
/*  GTW(ln_width);*/
  if (flag==1)
      for(i=0;i<n;i++) draw_line(px[i],py[i],px[(i+1)%n],py[(i+1)%n]);
}

static void _mpoly_fill(short n,int *px,int *py,int *px1,int *py1,unsigned char b_color)
{
  short j,imd;
  short i,cnt,ind,pcrl=0;
  int xcord[1500];
  int xlow,xup;
  int txx;
  int ymax[1500],ymin[1500],xmax[1500],xmin[1500],tyx,tyn,txn,scan,ygrt;
  unsigned char tempc;
  xup =-100000;
  xlow = 1000000;
  scan=10000;
  ygrt=-1239000;
  for (i=0;i<n;++i)
  {
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
  if((scan> c_v_y2) || (ygrt< c_v_y1) || (xlow> c_v_x2) || (xup< c_v_x1)) return;
  tempc = c_color;
  c_color= b_color;
  GPEN(c_color);
  if (ymax[ind]==scan) ind++;
  while (scan<=ygrt)
  {
 cnt=0;
         pcrl++;
         pcrl %=10;
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
while((j>=0) && (txx < xcord[j])){ xcord[j+1]=xcord[j];j--;}
xcord[j+1]=(txx);
 }
 /*displaying scan lines*/
 if(pcrl==0) for (i=0;i<cnt;i+=2)
           phori_line(xcord[i],xcord[i+1],scan);
 scan++;
  }
  c_color=tempc;
  GPEN(c_color);
}

static void _circle(int x,int y,int r)
{
  short i,j;
  int xx,yy;
  double pi;
  pi=3.14159265/12.0;
  _move(x+r,y);
  for(i=1;i<=24;i++)
    {
     xx=x+r*cos(i*pi)+0.5;
     yy=y+r*sin(i*pi)+0.5;
     _draw_(xx,yy);
    }
}

static void _circle_fill(int x,int y,int r,unsigned char color)
{
  int i,j;
  int xx[24],yy[24];
  double pi;
  pi=3.14159265/12.0;
  xx[0]=x+r;
  yy[0]=y;
  for(i=1;i<24;i++)
    {
     xx[i]=x+r*cos(i*pi)+0.5;
     yy[i]=y+r*sin(i*pi)+0.5;
    }
  _poly_fill_ps(24,xx,yy,1,15);
}

static void clr_view(void)
 {
  _box_fill(v_x1,v_y1,v_x2,v_y2,0);
 }
static void clr_buf(void)
 {
  if(OPEN)close(rbuf);
  rbuf=open("review.dat",O_CREAT|O_RDWR|O_TRUNC|O_BINARY, S_IREAD|S_IWRITE);
  Byte=0;
  OPEN=1;
  BACK_UP=0;
 }
static void _rect_fill(short x1,short y1,short x2,short y2,unsigned char color)
{
  short x_min,y_min,x_max,x_max1,x_min1,y_max,i,j,po1;
  unsigned char temp;
  temp=c_color;
  c_color=color;
  GPEN(c_color);
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
      for (j=y_min;j<=y_max;j=j+10) phori_line(x_min,x_max1,j);
    }
}
  c_color=temp;
  GPEN(c_color);
  return;
}
static void read_buf(void *ch,short n)
  {
    short i;
    unsigned char *ptr;
    ptr = (unsigned char *)ch;
    for(i=0;i<n;i++)
     {
       if ( R_Byte>=R_max){ R_max=read(rbuf,(void *)Rbuff,B_min);R_Byte=0;}
       *(ptr+i)=Rbuff[R_Byte++];
     }
  }
static   short  strlngth(char *title,float *xdsp)
    {
      float wd,gp,fj,gj,val,fact,fact1=1.0;
      short ngp,n,i,j,k,greek=0;
      wd = txt_wtx;
      gp = txt_spx;
      ngp =1;
      j =0; while( title[j] != '\0')j++;
      if(j==0){ *xdsp=0.;return(0);}
      fj =0.0; gj = j-1; fact = 1.0;
      i=0;
      while(i<j) 
      {
       if( title[i] != '!')
          { if( pr_txt>0) fact1=m_f[title[i]-32+greek]*fact;
            else fact1=1.0;
            fj +=fact1;greek=0;}
       else { 
             i=i+1; gj = gj-2;
             if((title[i] == 's')|| (title[i] == 'S'))
                           {  fact = fact*0.6;};
             if(title[i] == 'e') fact = fact/0.6;
             if(title[i] == 'b') { fj = fj-fact1; gj=gj-1;};
             if(title[i] == 'g') greek=128;
             if(title[i] == '!') { fj = fj+fact1; gj = gj+1;};
             if(title[i] == 'z') {
                            val = title[i+2]-'0';
                            if(val == 0.0) val=1.0;
                            val = (float)(title[i+1]-'0')/val;
                            fact = fact*val;
                            i = i+2;
                            gj = gj-2;
                           };
            }
       i = i+1;
      }
      *xdsp =(fj*wd+gj*gp);
      ngp = gj+0.1;
      return(ngp);
  }
static void win_set_lnwidth(void)
{
  int width;
  read_buf(&width,4);
  if(width<=0) width=1;
  if(ln_width != width)GTW(width);
  ln_width=width;
  return;
}
static void win_set_txtbold(void)
{
  int bold;
  read_buf(&bold,4);
  txt_bold=bold;
  return;
}
static  void read_file(short rbuf)
  {
   Byte=read(rbuf,(void *)Rbuff,B_min);
  }
static  void write_buf( void *ch, short n)
  {
    short i;
    int j;
    unsigned char * ptr;
    ptr = (unsigned char *) ch;
    for(i=0;i<n;i++) Rbuff[Byte++]=*(ptr+i);
    if ( Byte> B_max)
      { write(rbuf,(const void *)Rbuff,Byte);Byte=0;BACK_UP=1;}
  }
static void _godard_fill(short n,int *px,int *py,short flag,float *pv)
{
  short j,imd,tempc;
  short i,cnt,ind;
  int pcrl=0;
  int xcord[15];
  int xlow,xup;
  int txx;
  int ymax[15],ymin[15],xmax[15],xmin[15],tyx,tyn,txn,scan,ygrt;
  float pmax[15],pmin[15],pcord[15],tpx,tpn,tpp;
  int width=5;
  xup =-100000;
  xlow = 1000000;
  scan=10000;
  ygrt=-1239000;
  tempc=c_color;
  /*GTW(width);*/
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
  if((scan> c_v_y2) || (ygrt< c_v_y1) || (xlow> c_v_x2) || (xup< c_v_x1)) return;
  if (ymax[ind]==scan) ind++;
  while (scan<=ygrt)
  {
 cnt=0;
 while (ymax[ind]==scan) ++ind;  /*  excluding upper edges  */
         if(pcrl%5==0) {
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
    godr_line(xcord[i],xcord[i+1],scan,pcord[i],pcord[i+1]);
         }
         pcrl++;
 scan++;
  }
  c_color=tempc; set_clr(c_color);
  GPEN(c_color);
  if (flag==1)
      for(i=0;i<n;i++) _draw_line(px[i],py[i],px[(i+1)%n],py[(i+1)%n]);
  /*GTW(ln_width);*/
}
static void godr_line_o(int xa,int xb,int y,float pa,float pb)
{
  int i,pos,p1,p2,yy;
  int temp;
  int tempc,clr;
  float tempp,pval,pfac;
  if((y>c_v_y2) || (y<c_v_y1) ) return;
  yy = EVGAY-y;
  p1 = pa+0.5;
  p2 = pb+0.5;
  if(p1<min_clr) p1=min_clr;
  if(p2<min_clr) p2=min_clr;
  if(p1>max_clr) p1=max_clr;
  if(p2>max_clr) p2=max_clr;
  if( p1==p2)
       {
        p1 +=min_clr;
        c_color=p1;
        GPEN(c_color);
        set_clr(c_color);
        hori_line(xa,xb,y);
        return;
       }
  if (xb<xa)
  {
    temp=xa;
    tempp=pa;
    xa=xb;
    xb=temp;
    pa=pb;
    pb=tempp;
  }
  if (xa>c_v_x2) return;
  if (xb<c_v_x1) return;
  pfac = 0;
  if(xb != xa) pfac = (pb-pa)/(xb-xa);
  if(xa<c_v_x1)
      {pval = pa+(c_v_x1-xa)*pfac;
       xa= c_v_x1;
       pa= pval;}
  if(xb>c_v_x2)
      {pval = pa+(c_v_x2-xa)*pfac;
       xb= c_v_x2;
       pb= pval;}
  for(i=xa;i<=xb;i+=10) {
    tempc=(int)(pa+(i-xa)*pfac+0.5);
    if(tempc<1) tempc=1;
    if(tempc>max_clr) tempc=max_clr;
    tempc +=min_clr;
    c_color=tempc;
    set_clr(tempc);
    GPEN(tempc);
    put_pixl(i,y);
  }
}
static void godr_line(int xa,int xb,int y,float pa,float pb)
{
  int i,pos,p1,p2,yy;
  int temp;
  int tempc,clr;
  float tempp,pval,pfac;
  if((y>c_v_y2) || (y<c_v_y1) ) return;
  if (xb<xa)
  {
    temp=xa;
    tempp=pa;
    xa=xb;
    xb=temp;
    pa=pb;
    pb=tempp;
  }
  if (xa>c_v_x2) return;
  if (xb<c_v_x1) return;
  yy = EVGAY-y;
  p1 = pa+0.5;
  p2 = pb+0.5;
  if(p1<1) p1=1;
  if(p2<1) p2=1;
  if(p1>max_clr) p1=max_clr;
  if(p2>max_clr) p2=max_clr;
  if( p1==p2)
       {
        p1 +=min_clr;
        if(c_color !=p1) {
         c_color=p1;
         GPEN(c_color);
         set_clr(c_color);
        }
        GLN(xa,y,xb,y);
        return;
       }
  if ((xb-xa)<10)
  {
        p1=(p1+p2)*.5+min_clr+0.5;
        if(c_color !=p1) {
         c_color=p1;
         GPEN(c_color);
         set_clr(c_color);
        }
        GLN(xa,y,xb,y);
        return;
  }
  else {
    godr_line(xa,(xa+xb)/2,y,pa,(pa+pb)*.5);
    godr_line((xa+xb)/2,xb,y,(pa+pb)*.5,pb);
  }
  return;
}
static void win_godard_fill(void)
{
  int n,i;
  float x[20];
  float y[20],p[20],p1[20];
  int x1[20],y1[20];
  int color;
  float pv;
  read_buf(&n,4);
  read_buf(x,4*n);
  read_buf(y,4*n);
  read_buf(p,4*n);
  for(i=0;i<n;i++)
  {
    x1[i] =scr_x(x[i]); 
    y1[i] =scr_y(y[i]);
  }
   for (i=0;i<n;i++) {pv=*(p+i), p1[i] = pclr(pv); }
  _godard_fill((short)n,x1,y1,(short)ibdr,p1);
}
static  void win_set_godard_clrs(void)
  {
   int n,ib;
   read_buf(&pmax,4);
   read_buf(&pmin,4);
   read_buf(&n,4);
   read_buf(&ib,4);
   min_clr = 64;
   max_clr=n;
   ibdr = ib;
   dfac = (n -1)/(pmax-pmin);
  }
static  void win_set_shade_clrs(void)
  {
   int n,ib,sclr;
   read_buf(&pmax,4);
   read_buf(&pmin,4);
   read_buf(&sclr,4);
   read_buf(&n,4);
   read_buf(&ib,4);
   min_clr = sclr;
   max_clr=n;
   ibdr = ib;
   dfac = (n -1)/(pmax-pmin);
  }
static void win_set_prtxt( )
 {
  int txt;
  read_buf(&txt,4);
  pr_txt=txt;
 }
static void  win_sup_clip_limit(void)
  {
    float x1,y1,x2,y2;
    int ix1,ix2,iy1,iy2;
    read_buf(&x1,4);
    read_buf(&y1,4);
    read_buf(&x2,4);
    read_buf(&y2,4);
    ix1=scr_x(x1);
    ix2=scr_x(x2);
    iy1=scr_y(y1);
    iy2=scr_y(y2);
    if(ix1>=ix2) {
                  printf(" Wrong Clip limits\n");
                  exit(0);
                 }
    if(iy1>=iy2) {
                  printf(" Wrong Clip limits\n");
                  exit(0);
                 }
     if(ix1>=v_x1) s_v_x1= ix1;
     else s_v_x1=v_x1;
     if(ix2<=v_x2) s_v_x2= ix2;
     else s_v_x2=v_x2;
     if(iy1>=v_y1) s_v_y1= iy1;
     else s_v_y1=v_y1;
     if(iy2<=v_y2) s_v_y2= iy2;
     else s_v_y2=v_y2;
    }
static void  win_clip_limit(void)
  {
    float x1,y1,x2,y2;
    int ix1,ix2,iy1,iy2;
    CLIP * temp;
    read_buf(&x1,4);
    read_buf(&y1,4);
    read_buf(&x2,4);
    read_buf(&y2,4);
    temp = (CLIP *) malloc(sizeof(CLIP));
    if( temp ==NULL) {
                      printf(" Error: memory allocation in clip\n");
                      exit(0);
                     }
                
    temp->pt= (int *)c_loc;
    temp->cx1=c_v_x1;
    temp->cy1=c_v_y1;
    temp->cx2=c_v_x2;
    temp->cy2=c_v_y2;
    c_loc=(CLIP *)temp;
    ix1=scr_x(x1);
    ix2=scr_x(x2);
    iy1=scr_y(y1);
    iy2=scr_y(y2);
    if(ix1>=ix2) {
                  printf(" Wrong Clip limits\n");
                  exit(0);
                 }
    if(iy1>=iy2) {
                  printf(" Wrong Clip limits\n");
                  exit(0);
                 }
     if( ix2<c_v_x1) {c_v_x1=1,c_v_x2=0,c_v_y1=1,c_v_y2=0;};
     if( ix1>c_v_x2) {c_v_x1=1,c_v_x2=0,c_v_y1=1,c_v_y2=0;};
     if( iy2<c_v_y1) {c_v_x1=1,c_v_x2=0,c_v_y1=1,c_v_y2=0;};
     if( iy1>c_v_y2) {c_v_x1=1,c_v_x2=0,c_v_y1=1,c_v_y2=0;};
     if(ix1>=c_v_x1) c_v_x1= ix1;
     if(ix2<=c_v_x2) c_v_x2= ix2;
     if(iy1>=c_v_y1) c_v_y1= iy1;
     if(iy2<=c_v_y2) c_v_y2= iy2;
     GSV;
     GCLIP(c_v_x1,c_v_y1,(c_v_x2-c_v_x1),(c_v_y2-c_v_y1));
    }
static void clear_clip(void)
  {
    CLIP *temp;
    while (c_loc !=NULL)
       {
        temp=(CLIP *)c_loc->pt;
        free( c_loc);
        c_loc=(CLIP *)temp;
       }
   }
static void win_pop_clip(void)
  {
    CLIP *temp;
    if (c_loc !=NULL)
       {
        c_v_x1=c_loc->cx1;
        c_v_y1=c_loc->cy1;
        c_v_x2=c_loc->cx2;
        c_v_y2=c_loc->cy2;
        GPEN(PEN);
        GRS;
        temp=(CLIP *)c_loc->pt;
        free( c_loc);
        c_loc=(CLIP *)temp;
       }
   }
static void win_chng_clr()
{
  int no,ir,ig,ib;
  read_buf(&no,4);
  read_buf(&ir,4);
  read_buf(&ib,4);
  read_buf(&ig,4);
  if((ir==255)&&(ib==255)&&(ig==255) ) {ir=0;ib=0;ig=0;}
  else if((ir==0)&&(ib==0)&&(ig==0) ) {ir=255;ib=255;ig=255;}
  icode[no][0]=ir;
  icode[no][1]=ig;
  icode[no][2]=ib;
}

static void set_landscape(void)
   {
       EVGAX=LEVGAX;
       EVGAY = LEVGAY;
       evgax=EVGAY,evgay=EVGAX;
       MAXB= (evgax+1)>>3;
       return;
   }
static void set_a4copy(void)
   {
       EVGAY = LEVGAX;
       EVGAX= LEVGAY;
       evgay=EVGAY;
       evgax=EVGAX;
       MAXB= (evgax+1)>>3;
   }
static short ichk_blnk( unsigned char  *c,int n)

  {
    int i;
    for (i=0;i<n;i++) if( c[i] !=0 ) return(1);
    return(0);
  }
static void cpy_hdr(void)
{
  int i;
  fprintf(hbuf,"%%!PS-Adobe-3.0\n");
  fprintf(hbuf,"%%%%Title: Gph Convert\n");
  fprintf(hbuf,"%%%%Creator: Kulina Graphics\n");
  /*fprintf(hbuf,"%%%%BoundingBox: 146 196 465 596\n");*/
  fprintf(hbuf,"%%%%EndComments\n");
  fprintf(hbuf,"/$F2psDict 200 dict def \n");
}
static void cpy_hdr1(void)
{
  int i;
  fprintf(hbuf,"$F2psDict begin\n");
  fprintf(hbuf,"$F2psDict /mtrx matrix put\n");
  fprintf(hbuf,"/l {lineto} bind def\n");
  fprintf(hbuf,"/m {moveto} bind def\n");
  fprintf(hbuf,"/s {stroke} bind def\n");
  fprintf(hbuf,"/n {newpath} bind def\n");
  fprintf(hbuf,"/S {scale} bind def\n");
  fprintf(hbuf,"/R {rotate} bind def\n");
  fprintf(hbuf,"/W {show } bind def\n");
  fprintf(hbuf,"/c {setcolor} bind def\n");
  fprintf(hbuf,"/H {false charpath } bind def\n");
  fprintf(hbuf,"/K {false charpath } bind def\n");
  fprintf(hbuf,"/T {gsave 1 -1 scale} bind def\n");
  fprintf(hbuf,"/gs {gsave} bind def\n");
  fprintf(hbuf,"/gr {grestore} bind def\n");
  fprintf(hbuf,"/clp {closepath} bind def\n");
  fprintf(hbuf,
   "/graycol {dup dup currentrgbcolor 4 -2 roll mul 4 -2 roll mul ");
  fprintf(hbuf,"4 -2 roll mul setrgbcolor} bind def\n");
  fprintf(hbuf,"/col-1 {} def\n");
/* modification for poly_fill_ps
#ifdef MONO
   fprintf(hbuf,"/col000 {1 1 1 setrgbcolor} bind def\n", i);
  for(i=1;i<NCLRS;i++){
   fprintf(hbuf,"/col%3.3d {0 0 0 setrgbcolor} bind def\n", i);
  }
#else
  for(i=0;i<NCLRS;i++){
   fprintf(hbuf,"/col%3.3d {%4.2f %4.2f %4.2f setrgbcolor} bind def\n",
              i,(float)icode[i][0]/255.,(float)icode[i][1]/255.,
              (float)icode[i][2]/255.);
  }
#endif
*/
#ifdef MONO
  for(i=0;i<NCLRS;i++){
    icode[i][1]=icode[i][2];
    icode[i][0]=icode[i][2];
  }
#endif
  for(i=0;i<NCLRS;i++){
   fprintf(hbuf,"/col%3.3d {%4.2f %4.2f %4.2f setrgbcolor} bind def\n",
              i,(float)icode[i][0]/255.,(float)icode[i][1]/255.,
              (float)icode[i][2]/255.);
  }
  fprintf(hbuf,"end\n");
  fprintf(hbuf,
    "/$F2psBegin {$F2psDict begin /$F2psEnteredState save def} def\n");
  fprintf(hbuf,"/$F2psEnd {$F2psEnteredState restore end} def\n");
  fprintf(hbuf,"%%%%EndProlog\n");
  fprintf(hbuf,"\n");
  fprintf(hbuf,"$F2psBegin\n");
  fprintf(hbuf,"2 setlinecap 1 setlinejoin\n");
  fprintf(hbuf,"30.0 60.0  translate 90 rotate  0.07087 -0.07087 scale\n");
  fprintf(hbuf,"%-10.2f setlinewidth\n",Ln_width);
  fprintf(hbuf,"/Times-Roman findfont 188.0 scalefont setfont\n");
  fprintf(hbuf,"gsave\n");
}
static void cpy_tail(void)
{
 fprintf(hbuf,"showpage\n");
 fprintf(hbuf,"$F2psEnd\n");
}
static  void hardcopy()
   {
      unsigned char Esc=0x1b,Per='%';
      cpy_files();
/*
#ifndef RS6000
      fwrite(&Esc,1,1,hbuf);
      fwrite(&Per,1,1,hbuf);
      fprintf(hbuf,"0A");
#endif
      fwrite(&Esc,1,1,hbuf);
      fprintf(hbuf,"E");
*/
      cpy_tail();
      fclose(hbuf);
      return;
   }
static void put_pixl(short col, short row)
{
  short pos,j,temp;
  int addr;
  if(LSCAPE){temp=col,col=EVGAY-row,row=temp;};
/*
  addr = ((row)*MAXB + (col>>3));
  pos=col%8;
  if( c_color != 0) *(buffer+addr) |= MASK[pos];
  else *(buffer+addr) &= ~MASK[pos];
*/
  GLN(col,row,col,row);
  return;
}

static void put_pixel(int col, int row)
{
  short pos,j,temp;
  int addr;
  if ((col >c_v_x2) || (col <c_v_x1) || (row >c_v_y2) || (row <c_v_y1)) return;
  if(LSCAPE){temp=col,col=row,row=evgay-temp;};
/*
  pos=col%8;
  addr = (evgay-row)*MAXB + (col>>3);
  if( c_color != 0) *(buffer+addr) |= MASK[pos];
  else *(buffer+addr) &= ~MASK[pos];
*/
  GLN(col,row,col,row);
  return;
}
int  pscript(char *inf,char *outf) {
  int i,ln,ret=1;
  char ch;
  float fact;
  FILE *fp;
  char *TmpDir;
  T_rot=0;
  LSCAPE=0;
  A4 =0;
  EVGAX=LEVGAX;
  EVGAY=LEVGAY;
  strcpy(InFile,inf);
  strcpy(OutFile,outf);
  MAXB= (EVGAX+1)>>3;
  fact=1.95;
  evgax=EVGAX,evgay=EVGAY;
  ln = ((int)(evgax+1))*((int)(evgay+1))*fact+8;
  ln >>=3;
  y_mulf=1.0;
  if (LSCAPE){
   T_rot=0.0;
   A4=0;
   LSCAPE=1;
   y_mulf=1.;
   printf("Pscript:  Landscape case\n");
   set_landscape();
  }
  if (A4) {
   T_rot=-90.0;
   A4=1;
   LSCAPE=0;
   printf("Pscript:  A4 case\n");
   y_mulf*=40./22.5;
   set_a4copy();
   evgay=EVGAY;
  }

  MAXB= (evgax+1)>>3;
  TmpDir=kgMakeTmpDir();
  sprintf(TX_HC,"%-s/TX_HC",TmpDir);
  initialise();
  fp = fopen(InFile,"r");
  if(fp != NULL) {
    fclose(fp);
    review_file(InFile);
    ret=1;
  }
  else ret=0;
  kgCleanDir(TmpDir);
  free(TmpDir);
  return ret;
 }
