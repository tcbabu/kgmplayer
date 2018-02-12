#define LASER
#include <pthread.h>
#include "kulina.h"
#include "gprivate.h"
#include "gminterface.h"
#include "dlink.h"
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <magick/api.h>
static int POINTSIZE=1;
static int MFAC=2;
/*
  The code is not thread safe
  Hence only one person at a time;
*/
static pthread_mutex_t _Tmplock=PTHREAD_MUTEX_INITIALIZER;
#define Lock pthread_mutex_lock(&_Tmplock)
#define Unlock pthread_mutex_unlock(&_Tmplock)
static unsigned long BkColor=0x00000000;
//static unsigned long BkColor=0x00000000;
static unsigned long *buffer=NULL; /* image buffer */
static  PixelPacket *pixels;

static void *IEEEtoPC(void *val);
static void *IEEEtoPCs(void *val);

#ifdef UNIX_VERSION
#define min(x,y) (x <= y? x : y)
#define max(x,y) (x >= y? x : y)
#endif

#define FREE(a) if(a!=NULL){free(a);a=NULL;}

static float illm_fac = 1.0;
static int No_of_lights=0,St_sh_clr,No_sh_clrs=0;
static float Hue =0.,Satu=0.;

#define DSCALE 1
#define C1 0.25
#define C2 0.25
#define C3 0.5

#define scale_cos(a) (a)*illm_fac
#define sqr(x) ((x)*(x))
#define dist(d) (1.0*(d>1 ? 1: 1./(C1+C2*d+C3*d*d)))
static Dlink *ls_list=NULL;
void *Loadfontstruct(void);
static  void writetiff(char *flname,int sizex,int sizey);

extern short  kgIcode[1024][3];
static kgIcodeLoc[1024][3];
typedef struct 
	{
	  float x;
	  float y;
	  float z;
	}
	POINT;


typedef struct
        {
           float x,y,z;
        }
        LIGHT;


static Box3D box3d;
static unsigned long imgGetColor(int no,int loc) {
  unsigned long color=1;
//    color = 0xff000000|(kgIcodeLoc[no][0]<<16)|(kgIcodeLoc[no][1]<<8)|(kgIcodeLoc[no][2]);
    pixels[loc].red = kgIcodeLoc[no][0];
    pixels[loc].green = kgIcodeLoc[no][1];
    pixels[loc].blue = kgIcodeLoc[no][2];
    pixels[loc].opacity = 0x00;
    return color;
}

static  int InitBuff( int sizex, int sizey) {
        int i,ln;
        unsigned long Bcolr;
//        Bcolr=0xff000000|BkColor;
        Bcolr=BkColor;
        ln = (sizex+1)*(sizey+1);
	buffer = (unsigned long *) malloc(ln*sizeof(unsigned long));
	if ( buffer == NULL ) return(0);
        for(i=0;i<ln;i++) buffer[i]=Bcolr;
	return (1);
}

#define GetRGBA(x) {\
  a = ((x&0xff000000) >> 24);\
  r = ((x&0x00ff0000) >> 16);\
  g = ((x&0x0000ff00) >> 8);\
  b = ((x&0x000000ff) );\
}
static int WriteImg(int sizex, int sizey) {
  int i,j,xres,yres,k,loc,pos,v;
  unsigned long r,g,b,a,red[9],green[9],blue[9],alpha[9];
  xres = sizex/MFAC;
  yres = sizey/MFAC;
  for(j=0;j<yres;j++) {
    for(i=0;i<xres;i++){
      loc = MFAC*j*sizex+MFAC*i;
      pos=0;
      GetRGBA( buffer[loc]);
      red[pos]=r;green[pos]=g;blue[pos]=b;alpha[pos]=a;
      loc++;pos++;
      GetRGBA( (buffer[loc]));red[pos]=r;green[pos]=g;blue[pos]=b;alpha[pos]=a;
      loc++;pos++;
      GetRGBA( (buffer[loc]));red[pos]=r;green[pos]=g;blue[pos]=b;alpha[pos]=a;
      loc = (MFAC*j+1)*sizex+MFAC*i;pos++;
      GetRGBA( (buffer[loc]));red[pos]=r;green[pos]=g;blue[pos]=b;alpha[pos]=a;
      loc++;pos++;
      GetRGBA( (buffer[loc]));red[pos]=r;green[pos]=g;blue[pos]=b;alpha[pos]=a;
      loc++;pos++;
      GetRGBA( (buffer[loc]));red[pos]=r;green[pos]=g;blue[pos]=b;alpha[pos]=a;
      loc = (MFAC*j+2)*sizex+MFAC*i;pos++;
      GetRGBA( (buffer[loc]));red[pos]=r;green[pos]=g;blue[pos]=b;alpha[pos]=a;
      loc++;pos++;
      GetRGBA( (buffer[loc]));red[pos]=r;green[pos]=g;blue[pos]=b;alpha[pos]=a;
      loc++;pos++;
      GetRGBA( (buffer[loc]));red[pos]=r;green[pos]=g;blue[pos]=b;alpha[pos]=a;
#if 1
      a = (alpha[0]+alpha[1]+alpha[2]+alpha[3]+alpha[4]+alpha[5]+alpha[6]+alpha[7]+alpha[8])/9;
      r = (red[0]+red[1]+red[2]+red[3]+red[4]+red[5]+red[6]+red[7]+red[8])/9;
      g = (green[0]+green[1]+green[2]+green[3]+green[4]+green[5]+green[6]+green[7]+green[8])/9;
      b = (blue[0]+blue[1]+blue[2]+blue[3]+blue[4]+blue[5]+blue[6]+blue[7]+blue[8])/9;
#else
      a = (alpha[0]+alpha[1]+alpha[2]+alpha[3]+alpha[4]+alpha[5]+alpha[6]+alpha[7]+alpha[8])/9;
      r = (alph[0]*red[0]+red[1]+red[2]+red[3]+red[4]+red[5]+red[6]+red[7]+red[8])/9;
      g = (alph[0]*green[0]+green[1]+green[2]+green[3]+green[4]+green[5]+green[6]+green[7]+green[8])/9;
      b = (alph[0]*blue[0]+blue[1]+blue[2]+blue[3]+blue[4]+blue[5]+blue[6]+blue[7]+blue[8])/9;
#endif
      loc = j*xres+i;
      buffer[loc]= (a<<24)|(r<<16)|(g<<8)|b;
    }
  }
  writetiff("GRAF.c",xres,yres);
  return 1;
}
static int changebw(void) {
  kgIcodeLoc[0][0]=kgIcodeLoc[0][1]=kgIcodeLoc[0][2]=255;
  kgIcodeLoc[15][0]=kgIcodeLoc[15][1]=kgIcodeLoc[15][2]=0;
}
static int resetbw(void) {
  kgIcodeLoc[0][0]=kgIcodeLoc[0][1]=kgIcodeLoc[0][2]=0;
  kgIcodeLoc[15][0]=kgIcodeLoc[15][1]=kgIcodeLoc[15][2]=255;
}
static  void writetiff(char *flname,int sizex,int sizey)
{
  int w,h,i,size,rem,j,k;
  unsigned long r,g,b,a;
  FILE *fp;
  w = sizex*4;
  h = sizey*4;
  fp = fopen(flname,"w");
  fprintf(fp,"#ifndef D_%-s\n","GRAF");
  fprintf(fp,"#define D_%-s\n","GRAF");
  fprintf(fp,"  static unsigned char %-s_data[%d] = {\n  ","GRAF",w*h+1);
  size = (sizex*sizey)/3;
  rem = (sizex*sizey) - size*3;
  k=0;
  for(i=0;i<size;i++) {
    for(j=0;j<3;j++) {
      GetRGBA(buffer[k]);
      fprintf(fp,"0x%2.2x,",r);
      fprintf(fp,"0x%2.2x,",g);
      fprintf(fp,"0x%2.2x,",b);
      fprintf(fp,"0x%2.2x,",a);
      k++;
    }
    fprintf(fp,"\n  ");
  }
  for(j=0;j<rem;j++) {
      GetRGBA(buffer[k]);
      fprintf(fp,"0x%2.2x,",b);
      fprintf(fp,"0x%2.2x,",g);
      fprintf(fp,"0x%2.2x,",r);
      fprintf(fp,"0x%2.2x,",a);
      k++;
  }
  fprintf(fp,"   0x0 };\n");
  fprintf(fp,"  static PNGIMG  %-s_str = {\n","GRAF");
  fprintf(fp,"    \"PNG\",1,\"%-s.png\", %d,%d,%d,%d,%-s_data \n  };\n","GRAF",sizex,sizey,sizex*4,4,"GRAF","GRAF");
  fprintf(fp,"#endif\n");
  fclose(fp);
  return ;
}



static void *IEEEtoPC(void *val)
{
#define SWAP(a,b) {ch=a;a=b;b=ch;}

	char *C=(char*)val,ch;

	SWAP(C[0],C[3]);
	SWAP(C[1],C[2]);
	return (void*)C;

}
static void *IEEEtoPCs(void *val)
{
	char *C=(char*)val,ch;

	SWAP(C[0],C[1]);

	return (void*)C;
#undef SWAP
}
#include <stdio.h>
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
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <malloc.h>
#include "dlink.h"
#include <string.h>

typedef struct {
  char *fontname;
  int *icpos;
  int *icxv;
  int *icyv;
  float *m_f;
} FONT;

static unsigned long imgGetColor(int no,int loc);
static unsigned short *zbuf=NULL;
static int Z_max=193600L;
static Dlink *Fontlist=NULL;
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
static float xe,ye,ze,zc,clip_min=0,clip_max=65000;
static float CPCONS=1.0;
static int ZBUFF=0;
static float FM[16];
static int Entry=0;
static int page=0;
static float Ln_width=6.0;
static unsigned int ln_ptn[10]={0xffffffff,0x88888888, 0xcccccccc, 0xf0f0f0f0,
            0xff18ff18,0xffff0f0f, 0xe7e7e7e7, 0xfafafafa,
            0xffff0000,0xffffff00};
static short st_ptr[10]={0,0,0,0,0,0,0,0,0,0};

#define TX(x,y) (cx +(x)*cost+(y)*sint)
#define TY(x,y) (cy -(x)*sint+(y)*cost)
#define z_inter(x,m,c) (int)(((x)*(m) + c)+0.5)
#define fz_inter(x,m,c) (float)(((x)*(m) + c))

static L_N *O_L=NULL;
static B_K *O_P=NULL,*D_P;
typedef struct c_limit{ int cx1;int cy1;int cx2;int cy2;int * pt;} CLIP;
static CLIP *c_loc;
static short A4=0,LSCAPE=0;
static int *wkx1,*wkx2,*wky1,*wky2,*wkx,*wky;
#define SSF 0.6

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
#define scr_x(x) (int)((x-w_x1)*u_x+D_x+0.5)
#define scr_y(y) (int)((y-w_y1)*u_y+D_y+0.5)
#define scr_z(z) (int)((z-clip_min)*CPCONS+0.5);
#define fscr_z(z) (float)((z-clip_min)*CPCONS);
#define usr_x(x) (float)((x-D_x)/u_x+w_x1)
#define usr_y(y) (float)((y-D_y)/u_y+w_y1)
#define cnv_x(x) (x*fx-sx)
#define cnv_y(y) (y*fy-sy)
#define set_clip_limits(x1,y1,x2,y2,x3,y3) c_v_x1=x1,c_v_y1=y1,c_v_x2=x2,c_v_y2=y2
#define set_sup_clip_limits(x1,y1,x2,y2) s_v_x1=x1,s_v_y1=y1,s_v_x2=x2,s_v_y2=y2
#define pclr(p)  (float)(((p-pmin)*dfac+0.))

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
static unsigned char far *str_ptr;
static unsigned char far *addr;
static unsigned int c_color=0x01,p_color,p_bound,c_bound;
static unsigned char far *far_ptr, far *cur_mem,far *v_mem;
static int cur_x,cur_y,D_x,D_y,lcur_x,lcur_y,rcur_x,rcur_y;
static int cur_z;
static float V_x,V_y;
static short pr_txt=1,txt_bold=1,txt_bold_o=1,ln_width=1,ln_style,m_style,po,jmp;
static float jmpy,ylcur,yrcur;
static float w_x1,w_y1,w_x2,w_y2,v_r,u_x,u_y;
static float txt_ht,txt_wt,txt_sp,r_d,theta,txt_w42,txt_h42;
static float txt_htx,txt_wty,txt_spy;
static float txt_hty,txt_wtx,txt_spx;
static  int v_xt1,v_yt1,v_x1,v_x2,v_y1,v_y2;
static  int c_v_xt1,c_v_yt1,c_v_x1,c_v_x2,c_v_y1,c_v_y2;
static  int s_v_x1,s_v_x2,s_v_y1,s_v_y2;
static int buf;
static int bufl,bufr;
static short hbuf,rbuf,nmenu;
static short t_color,t_bkgr,t_pattern,t_fill,t_bodr=0,greek=0;
static int *icxv,*icyv;
static int *icpos;
static float *m_f;
static float cost=1.0,sint=0.0;
static int gcur_x,gcur_y;
static float gw_x1,gw_y1,gw_x2,gw_y2;
static short max_clr=64,ibdr=0,min_clr=64,Cutoff=1;
static short EVGAX=2399;
static short evgay,evgax;
static short MAXB=300;
static short EVGAY=1799;
static int Cur_page=0;
static int t_font;
static float xpos,ypos,txs=0.,txc=1.;
static float cx,cy,xp,yp;

static void t_txt_font( int font);
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
static void box_fill(float x1,float y1,float x2,float y2,unsigned int color);
static void win_chng_clr();
static void win_box_fill(void);
static void poly_fill(short n,float *x,float *y,short flag,unsigned int color);
static void godard_fill(short n,float *px,float *py,short flag,float *pv);
static void win_godard_fill(void);
static void win_poly_fill(void);
static void circle(float x1,float y1,float r);
static void circle_fill(float x1,float y1,float r,unsigned int color);
static void init_graph();
static void initialise();
static void set_graphics(short n);
static void set_mode1();
static void set_mode2();
static void _move(int x,int y);
static void _draw(int x,int y);
static void draw_line(int xa,int ya,int xb,int yb);
static void hori_line(int xa,int xb,int y);
static void phori_line(int xa,int xb,int y);
static void godr_line(int xa,int xb,int y,float pa,float pb);
static void set_color(int color);
static void win_set_godard_clrs(void);
static void win_set_shade_clrs(void);
static void win_set_color(void);
static void put_pixel(int col, int row);
static void txt_write(char *txt);
static void txt_incl(char *txt);
static void txtwrt(char *txt);
static void win_txtwrt(void);
static void txt_color(short color);
static void win_txt_color(void);
static void win_txt_pattern(void);
static void win_txt_fill(void);
static void win_txt_slant(void);
static void win_txt_bodr(void);
static void win_txt_bkgr(void);
static void txt_font(short font);
static void win_txt_font(void);
static void set_angle(float t);
static void win_set_angle(void);
static void txt_size(void);
static void win_txt_size(void);
static void win_clip_limit();
static void win_pop_clip();
static void win_sup_clip_limit();
static void set_lnstyle(short styl);
static short  strlngth(char *title,float *xdsp);
static void win_set_lnstyle(void);
static void win_resetstyle(void);
static void user_cord(float x1,float y1,float x2,float y2);
static void set_viewport(short x1,short y1,short x2,short y2);
static void set_markstyle(short style);
static void win_set_markstyle(void);
static void win_set_lnwidth(void);
static void win_set_txtbold(void);
static void _marker (int x,int y);
static void _box_fill(int x1,int y1,int x2,int y2,unsigned int color);
static void clear_screen();
static void _poly_fill(short n,int *px,int *py,short flag,unsigned int b_color);
static void _mpoly_fill(short n,int *px,int *py,int *px1,int *py1,unsigned int b_color);
static void _godard_fill(short n,int *px,int *py,short flag,float *pv);
static void scr_back(int x1,int y1,int x2, int y2, int *mem);
static void scr_recover(int x1,int y1,int x2,int y2, int *mem);
static void pixel_copy(int x1,int y1,int x2,int y2, int x3,int y3);
static void _circle(int x,int y,int r);
static void _circle_fill(int x,int y1,int r,unsigned int color);
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
static void mline( short ixb, short iyb, short ixe,short iye);
static void write_file(short rbuf);
static void review_file(char *str);
static void read_file(short rbuf);
static void menuch(char c,short ix1,short iy1);
static short review(void);
static void _rect_fill(short x1,short y1,short x2,short y2,unsigned int color);
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
static void godr_line3(int xa,int xb,int y,int za,int zb,float pa,float pb);
static void clear_clip(void);


static short GHMAX=0,SCRN,S_OPEN=0;
static char *GHBUF;
static char df_gbuf[20]={"No help available"};
static unsigned char far *Rbuff;
static int B_max=31400,B_min=31000,Byte=0,R_max=0,R_Byte=0;
static unsigned char BACK_UP=0;
/*unsigned char gr_buf[5200];*/
static short OPEN=0,ENTRY=0;
static short MODE=18;
static unsigned char far * RBUFF;
static unsigned int char_clr=0x0F,fill_clr=0x08,bodr_clr=0x0E,cur_clr=0x09,
            high_clr=0x04;

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
static int  temp;
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
static unsigned char far *dp;
static unsigned short IPO1=0x3CE;
static unsigned short IPO2=0x3CF;
static unsigned short IPO3=0x3C4;
static unsigned short IPO4=0x3C5;
static char Print_com[60]={" "};
static char Check_com[60]={" "};
static char *Printer;
static int *icpostmp, *icxvtmp,*icyvtmp;
static int *icposf0, *icxvf0,*icyvf0;
static float *m_f0;

#define set_greek {\
 if(greek> 0) {\
   icpostmp = icpos;\
   icxvtmp = icxv;\
   icyvtmp = icyv;\
   icpos = icposf0;\
   icxv = icxvf0;\
   icyv = icyvf0;\
 }\
}

#define reset_greek {\
 if(greek> 0) {\
   icpos = icpostmp;\
   icxv = icxvtmp;\
   icyv = icyvtmp;\
 }\
}
static void SetIcode(void) {
 int i;
 for(i=0;i<1024;i++) {
  kgIcodeLoc[i][0]= kgIcode[i][0];
  kgIcodeLoc[i][1]= kgIcode[i][1];
  kgIcodeLoc[i][2]= kgIcode[i][2];
 }
}
static int t_godr_cutoff_on (void) {
  Cutoff=0;
}
static int t_godr_cutoff_off (void) {
  Cutoff=1;
}
static void transfrm(float xp, float yp, float zp)
 {

   trnstr.xstr = (xp * FM[0]) + (yp * FM[4]) + (zp * FM[8]) + FM[12];
   trnstr.ystr = (xp * FM[1]) + (yp * FM[5]) + (zp * FM[9]) + FM[13];
   trnstr.zstr = (xp * FM[2]) + (yp * FM[6]) + (zp * FM[10]) + FM[14];
   return;

 }
static void projection(str1 oldstr)
 {

     newstr.xstr = -(2.* ze/(oldstr.zstr)) * oldstr.xstr;
     newstr.ystr = -(2.* ze/(oldstr.zstr)) * oldstr.ystr;
     newstr.zstr = oldstr.zstr;
     return;
 }
static void transfrm_new(float xp, float yp, float zp)
 {

   trnstr.xstr = xp;
   trnstr.ystr = yp;
   trnstr.zstr = zp;
   return;

 }
static void projection_new(str1 oldstr)
 {

     newstr.xstr =  oldstr.xstr;
     newstr.ystr =  oldstr.ystr;
     newstr.zstr = oldstr.zstr;
     return;
 }
static  void setclr(unsigned int color) {
   
   PIXL =color&0xFF;
   return;
 }
static       void g_fill(short ix,short iy,int col)
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
static void gphCopyImage(int x0,int y0,GMIMG *img) {
  GMIMG *Dimg,*Simg;
  PixelPacket *spixels;
  int w,h,iw,ih,i,j,ii,jj,sloc,dloc,cx0,cx1,cy0,cy1;
  Simg = (GMIMG *)img;
  iw = EVGAX;
  ih = EVGAY;
  w  = Simg->image_width;
  h  = Simg->image_height;
  cx0=c_v_x1;
  cx1=c_v_x2;
  cy0=EVGAY-c_v_y2;
  cy1=EVGAY-c_v_y1;
//  printf("%d %d %d %d\n",cx0,cy0,cx1,cy1);
  spixels = GetImagePixels((Image *)(Simg->image),0,0,((Image *)(Simg->image))->columns,((Image *)(Simg->image))->rows);
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
    }
  }
  return;

}

static  void gph_drawimage(void *imgfile,float x1,float y1,float x2,float y2)
  {
  float fac;
  GMIMG *img,*rzimg;
  char *pt;
  int IMG =1,iw,ih,w,h,temp;
  int X1,Y1,X2,Y2;
  if(imgfile== NULL) return;
  X1=scr_x(x1);
  Y1=EVGAY-scr_y(y1);
  X2=scr_x(x2);
  Y2=EVGAY-scr_y(y2);
  if(Y1>Y2) {temp=Y2; Y2=Y1;Y1=temp;}
  if(X1>X2) {temp=X2; X2=X1;X1=temp;}
  pt = (char *)imgfile;
  if((pt[0]=='#')&&(pt[1]=='#')) { IMG=0; img = uiGetImage((void *)(pt+2));}
  else img = imgfile;
  if(img== NULL) return;
  w = X2-X1+1;
  h = Y2 -Y1+1;
  iw = img->image_width;
  ih = img->image_height;
  rzimg= (GMIMG *)kgChangeSizeImage(img,w,h);
  gphCopyImage(X1,Y1,rzimg);
  uiFreeImage(rzimg);
  if(!IMG) uiFreeImage(img);
  
  }
static void win_drawimage()
{
  char buff[100];
  float x1,y1,x2,y2;
  read_buf(buff,100);
  read_buf(&x1,4);
  read_buf(&y1,4);
  read_buf(&x2,4);
  read_buf(&y2,4);
//  _draw(x1,y1);
  gph_drawimage(buff,x1,y1,x2,y2);
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
  
//  write_buf(&ch,1);
// write_buf(&x1,4);
//  write_buf(&y1,4);
//  write_buf(&x2,4);
//  write_buf(&y2,4);
  xa=(int)(x1*u_x+D_x);
  ya=(int)(y1*u_y+D_y);
  xb=(int)(x2*u_x+D_x);
  yb=(int)(y2*u_y+D_y);
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
  xa=(int)(x1*u_x+D_x);
  ya=(int)(y1*u_y+D_y);
  xb=(int)(x2*u_x+D_x);
  yb=(int)(y2*u_y+D_y);
  DRAW_LINE(xa,ya,xb,yb);
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
  _box_fill(xa,ya,xb,yb,(unsigned int)color);
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
  int flag;
  int color;
  int *x1,*y1;
  float *x,*y;
  short i,j=0;
  int xo,yo,xv,yv;
  read_buf(&n,4);
  x = (float *) malloc(sizeof(float)*n);
  y = (float *) malloc(sizeof(float)*n);
  x1 = (int *) malloc(sizeof(float)*n);
  y1 = (int *) malloc(sizeof(float)*n);
  if(y1==NULL) {
    printf("Error: in allocating memory for panel\n");
    exit(0);
  }
  read_buf(x,4*n);
  read_buf(y,4*n);
  read_buf(&flag,4);
  read_buf(&color,4);
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
  _poly_fill((int)j,x1,y1,(int)flag,(unsigned int)color);
  free(x),free(y);
  free(x1),free(y1);
}
static void initialise()
{
  FONT *pt;
  int i,count,font;
  buffer=NULL;
#if 0
  if(!InitBuff(EVGAX,EVGAY)) {
    printf("Failed to Alloc Memory\n");
    exit(0);
  }
#endif
  for(i=0;i<10;i++) st_ptr[i]=0;
  for(i=0;i<16;i++) FM[i]=0; FM[0]=1,FM[5]=1,FM[10]=1,FM[15]=1;
  ln_style=LN_STYL;
  m_style=M_STYL;
  Slant = Slnt[0];
  Slant_o = Slant;
  if(O_L != NULL) {
    printf("O_L != NULL\n");
  }
  if (ENTRY==0) {
     RBUFF = (unsigned char far *) malloc(B_max+100);
     ENTRY=1;
     wkx1 = (int *) malloc(1500L*sizeof(int));
     wkx2 = (int *) malloc(1500L*sizeof(int));
     wky1 = (int *) malloc(1500L*sizeof(int));
     wky2 = (int *) malloc(1500L*sizeof(int));
     wkx  = (int *) malloc(1500L*sizeof(int));
     wky  = (int *) malloc(1500L*sizeof(int));
  }
  step=0;
  t_pattern=15;
  t_fill = 1;
  t_bkgr = 0;
  t_bodr = 1;
  c_loc=NULL;
  Rbuff= RBUFF;
  BACK_UP=0;
  GHMAX=1;
  GHBUF=df_gbuf;
  Byte=0;
  R_Byte=0;
  dp = far_ptr;
//  icpos = icposf0;icxv=icxvf0;icyv=icyvf0;m_f=m_f0;
  if(Fontlist == NULL ) Fontlist = (Dlink *)Loadfontstruct();
  count = Dcount(Fontlist);
  font =0;
  Dposition(Fontlist,font+1);
  pt = (FONT *)Getrecord(Fontlist);
  icpos = pt->icpos;
  icxv = pt->icxv;
  icyv = pt->icyv;
  m_f =  pt->m_f;
  icposf0 = icpos;icxvf0=icxv;
  icyvf0=icyv;m_f0=m_f;
  ln_width=POINTSIZE;
  pr_txt=1;
  cost = 1.0;
  sint = 0.0;
  txt_ht=20;
  txt_wt=20;
  txt_htx=20;
  txt_wty=20;
  txt_hty=20;
  txt_wtx=20;
  txt_w42 = txt_wt/CFact;
  txt_h42 = txt_ht/CFact;
  txt_sp=2;
  txt_spy=2;
  txt_spx=2;
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
}
static void reinitialise()
{
  int i;
//  InitTiffBuf(EVGAX,EVGAY);
  for(i=0;i<10;i++) st_ptr[i]=0;
  for(i=0;i<16;i++) FM[i]=0; FM[0]=1,FM[5]=1,FM[10]=1,FM[15]=1;
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
  icpos = icposf0;icxv=icxvf0;icyv=icyvf0;m_f=m_f0;
  ln_width=POINTSIZE;
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
  return;
}

static void set_viewport(short x1,short y1,short x2,short y2)
{
   float t;
   v_x1=x1;
   v_y1= y1;
   v_y1= (int) (v_y1+0.1);
   v_x2=x2;
   v_y2= y2;
   v_y2=(int) ( v_y2+0.1);
   if( v_y2 <= 0) {printf("Wrong View Port\n");exit(0);}
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
}

static void set_n_view(void)
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
  set_viewport((short)x1,(short)y1,(short)x2,(short)y2);
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
#ifndef UNIX
#define Fflush
static void wait_for_printing(void)
 {
   return;
 }
#else
#define Fflush fflush(NULL)
#ifndef RS6000
static void wait_for_printing(void)
 {
   int i,Wait=0;
   FILE *Fp;
   char buf[30],buf1[30],command[60];
   if( (Fp=fopen("JoB_Fil","r"))==NULL) return;
   fscanf(Fp,"%s",buf);
   fscanf(Fp,"%s",buf);
   fscanf(Fp,"%s",buf);
   fscanf(Fp,"%s",buf);
   fclose(Fp);
   sprintf(command,"lpstat -o %s>JoB_Fil 2>JoB_Fil\0",buf);
   printf( "Waiting..\r");
   fflush(NULL);
   for(;;){
     Wait++;
     sleep(5);
     system(command);
     if( (Fp=fopen("JoB_Fil","r"))==NULL) return;
     fscanf(Fp,"%s",buf1);
     fclose(Fp);
     i=0;
     while( buf[i] != '\0') {
        if(buf[i] != buf1[i]) return;
        i++;
     }
     if(Wait>100){
       printf( " There may be a problem with the printer\n");
       printf( " Please Check...\n");
       fflush(NULL);
     }
    if(Wait>150) exit(0);
   }  
   return;
 }
#else
static int Look_printer(char *b)
 {
  int i=0;
  while ( b[i] !='\0'){
    if( b[i] != Printer[i] ) return(0);
    else i++;
  }
  if( Printer[i] != '\0') return(0);
  else return(1);
 }
static int Printer_ready(char *b)
 {
  char Printer[7]={"READY"};
  int i=0;
  while ( b[i] !='\0'){
    if( b[i] != Printer[i] ) return(0);
    else i++;
  }
  if( Printer[i] != '\0') return(0);
  else return(1);
 }
static void wait_for_printing(void)
 {
   int i,Wait=0;
   FILE *Fp;
   char buf[30],buf1[30];
   printf( "Waiting..\n");
   fflush(NULL);
   for(;;){
     Wait++;
     sleep(5);
     system(Check_com);
     if( (Fp=fopen("JoB_Fil","r"))==NULL) return;
     fgets(buf,20,Fp);
     fgets(buf,20,Fp);
     while ( !Look_printer(buf) ){
      fscanf(Fp,"%s",buf1);
      fscanf(Fp,"%s",buf);
      fscanf(Fp,"%s",buf1);
     }
     fclose(Fp);
     if( Printer_ready(buf1) )return;
     if(Wait>100){
       printf( " There may be a problem with the printer\n");
       printf( " Please Check...\n");
       fflush(NULL);
     }
    if(Wait>150) exit(0);
   }  
   return;
 }
#endif
#endif
static void clear_zbuf(void)
 {
  int i,max;
  int far *pt;
  pt = (int far *)zbuf;
  max= Z_max/2;
  for(i=0;i<max;i++) *(pt+i)=0x00000000;
}
static void t_set_zbuf()
 {
  int i;
  Z_max=(EVGAY+1)*(EVGAX+1);
  if(zbuf==NULL) zbuf = (unsigned short *) malloc((unsigned) (2*Z_max));
  if(zbuf== NULL) {
     printf(" Error: Zbuffer allocation\n");
     exit(0);
  }
  clear_zbuf();
 }
static void t_clip_const(float cpmin,float cpmax)
 {
  clip_min=cpmin;
  clip_max = cpmax;
  CPCONS = 65000./(float)(clip_max-clip_min);
 }
static void t_near_clip(float cp)
 {
  clip_min = cp;
  CPCONS = 65000./(float)(clip_max-clip_min);
 }
static void t_far_clip(float cp)
 {
  clip_max = cp;
  CPCONS = 65000./(float)(clip_max-clip_min);
 }
static void t_release_zbuf()
 {
  clip_min=0;
  CPCONS=1.;
  if(zbuf != NULL) free((void *)zbuf);
  zbuf=NULL;
 }
static void put_pix3(int col,int row,int z)
{
  int r,c;
  unsigned short  *loc;
  if( (z<0) || (z>65000L)) return;
  loc=zbuf+row*EVGAX+col;
  if (z < *(loc)) return;
  *(loc) =z;
  put_pixl(col,row);

}

static void set_3d(void) {
   FILE *fp;
   float x1,y1,x2,y2;
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
     box3d.xmin = x1;
     box3d.ymin =  y1;
     box3d.zmin =  clip_min;
     box3d.xmax =  x2;
     box3d.ymax =  y2;
     box3d.zmax =  clip_max;
     box3d.xe   =  xe;
     box3d.ye   =  ye;
     box3d.ze   =  ze;
     box3d.xc   =  xe;
     box3d.yc   =  ye;
     box3d.zc   =  zc;
     box3d.d = (x2 -x1)*(x2- x1) +
               (y2 -y1)*(y2- y1) +
               (clip_max -clip_min)*(clip_max- clip_min) ;
     box3d.d = sqrt(box3d.d);
#if 0
   fp = fopen("InfoFil","w");
   fprintf(fp," %f %f %f\n",clip_min,clip_max,CPCONS);
   fclose(fp);
#endif
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
static void _move3(int xp, int yp,int zp)
{
  cur_x=xp ;
  cur_y=yp ;
  cur_z=zp;
  return;
}
static void t_3_move(float x,float y,float z) 
  {
  int x1,y1,z1;
  x1=scr_x(x);
  y1=scr_y(y);
  z1=scr_z(z);
  _move3(x1,y1,z1);
  }
static void win_3_move()
{
    float x,y,z;

    read_buf(&x,4);
    read_buf(&y,4);
    read_buf(&z,4);
    transfrm(x,y,z);
    projection(trnstr);
    if (ZBUFF == 1) { 
     Cx = newstr.xstr, Cy = newstr.ystr, Cz = newstr.zstr;
     t_3_move(newstr.xstr,newstr.ystr,newstr.zstr);
    }
    else
     t_move(newstr.xstr,newstr.ystr);
}

static void _draw_line3(int x1,int y1,int z1,int x2,int y2,int z2)
{
 char far *addr;
 short i,dx,dy,f,g,inc1,inc2,ab,dx1,dy1,miny,maxy,minx,maxx;
 int clip,ch,rh,r,c;
 unsigned int temp;
 int xa,ya,xb,yb,za,zb,z;
 float cons,m;
 double xf,yf;
 xa =x1,xb=x2,ya=y1,yb=y2,za=z1,zb=z2;
 xf=x2-x1;yf=y2-y1;
 if ((((z1)<0)&&((z2)<0))||(((z1)>65000L)&&((z2)>65000L))) return;
 clip=clip_line(&xa,&ya,&xb,&yb);
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
		    put_pix3(c,r,z);
		    c+=ch;
		    if (g > 0) { r-=rh; g+=inc2; }
		    else g+=inc1;
		  }
	  z = z_inter(xb,m,cons);
	  put_pix3(xb,(EVGAY-yb),z);
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
		   put_pix3(r,c,z);
		   c-=ch;
		   if (g > 0) { r+=rh; g+=inc2; }
		   else g+=inc1;
		 }
	  z = z_inter(yb,m,cons);
	  put_pix3(xb,(EVGAY-yb),z);
	  }
  }
  else
     {
	if (xa==xb)
	{
	  if ( ya < yb){ miny = ya;maxy = yb;}
	  else { miny = yb; maxy = ya;}
  	  for (i=miny;i<=maxy;++i)
            { z = z_inter(i,m,cons);
              put_pix3(xa,(EVGAY-i),z);}
	}
       else 
	{
	  if ( xa < xb){ minx = xa;maxx = xb;}
	  else { minx = xb; maxx = xa;}
          for (i=minx;i<=maxx;++i)
          {  
	    z = z_inter(i,m,cons);
	    put_pix3(i,(EVGAY-ya),z);
          }
	 }
     }
	return;
}

static void draw_line3(int xa,int ya,int za,int xb,int yb,int zb)
 {
   int i,i2;
   float dx,dy,ct,st,dl;
   int x1,y1,z1,x2,y2,z2,dx1,dy1;
   if( ln_width ==1){ _draw_line3( xa, ya, za, xb, yb, zb);return;};
   if( xa>xb){x1=xa,xa=xb,xb=x1,y1=ya,ya=yb,yb=y1;};
   i2=ln_width/2;
   dx = (float)(xb-xa);
   dy = (float)(yb-ya);
   dl = (dx*dx+dy*dy);
   if( dx==0.) {ct=0.0;st=1.;}
   else { dl =sqrt(dl),ct = (dx)/dl, st = (dy)/dl;}
   dx1 =(int)( st*i2+0.5);
   if( ct< 0.) dy1 =(int)( ct*i2-0.5);
   else dy1 =(int)( ct*i2+0.5);
   xa +=dx1,xb +=dx1,ya -=dy1,yb -=dy1;
   _draw_line3( xa, ya, za, xb, yb, zb);
   for (i=1;i<ln_width;i++)
	{
	     dx1 =(int)( st*i+0.5);
	     if( ct< 0.) dy1 =(int)( ct*i-0.5);
	     else dy1 =(int)( ct*i+0.5);
	    _draw_line3( xa-dx1,ya+dy1,za,xb-dx1,yb+dy1,zb);
	}
   return;
 }
static void hori_line3(int xa,int xb,int y,int za,int zb)
{
  short i,clip;
  char far *addr;
  int temp;
  int x1,x2,z1,z2;
  float cons,m;
  int z;
  x1=xa;x2=xb;z1=za;z2=zb;
  if (((za<0)&&(zb<0))||((za>65000L)&&(zb>65000L))) return;
  clip=clip_line(&xa,&y,&xb,&y);
  if (clip==0) return;
  if(xa==xb){
     put_pix3(xa,(EVGAY-y),za);
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
  for(i=xa;i<=xb;i++) {z=z_inter(i,m,cons);put_pix3(i,(EVGAY-y),z);}
}


static void _draw3_(int x, int y, int z)
{
  int xa, ya, za, xb, yb,zb;
  short  dx, dy,  f, inc1, inc2, g,dx1,dy1,ab;
  short clip,i,ch,rh,c,r;
  float h,m,cons;

  xa=cur_x;
  ya=cur_y;
  za=cur_z;

  xb=x;
  yb=y;
  zb=z;
  if (((za<0)&&(zb<0))||((za>65000L)&&(zb>65000L))) return;
  clip=clip_line(&xa,&ya,&xb,&yb);
   if (clip == 0)
   {
	cur_x=x;
	cur_y=y;
	cur_z=z;
	return;
   }
  if (cur_x == x)
   {
    m = (float)(y-cur_y)/(float)(z-cur_z);
    cons = cur_z - cur_y*m;  
    za=z_inter(ya,m,cons);zb=z_inter(yb,m,cons);
   }
   else
   {
    m = (float)(x-cur_x)/(float)(z-cur_z);
    cons = cur_z - cur_x*m;  
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
     while ((i<=ln_style) && (abs(c-f)>0))
	 {
	 if ( dx1 > dy1 ) {z=z_inter(c,m,cons);put_pix3(c,(EVGAY-r),z);}
	 else   {z=z_inter(r,m,cons);put_pix3(r,(EVGAY-c),z);}
	 c+=ch;
         if (g > 0) { r+=rh; g+=inc2; }
         else g+=inc1;
         i++;
	 }
    i=0;
    while ((i<=ln_style) && (abs(c-f)>0))
	 {
	 c+=ch;
         if (g > 0) { r+=rh; g+=inc2; }
         else g+=inc1;
         i++;
	 }
    }
    cur_x=x;
    cur_y=y;
    cur_z=z;
   return;
}


static void _draw3(int x, int y, int z)
 {
   int i,i2;
   float dx,dy,ct,st,dl;
   int x1,y1,z1,xa,ya,za,xb,yb,zb,dx1,dy1;
   xa=cur_x;
   ya=cur_y;
   za=cur_z;
   xb=x;
   yb=y;
   zb=z;
   if ( ln_style == 0) {draw_line3(xa,ya,za,xb,yb,zb);}
   else
    {
     if(ln_width==1) _draw3_( x,y,z);
     else
      {
      if( xa>xb){x1=xa,xa=xb,xb=x1,y1=ya,ya=yb,yb=y1;};
      i2=ln_width/2;
      dx = (float)(xb-xa);
      dy = (float)(yb-ya);
      dl = (dx*dx+dy*dy);
      if( dx==0.) {ct=0.0;st=1.;}
      else { dl =sqrt(dl),ct = (dx)/dl, st = (dy)/dl;}
      dx1 =(int)( st*i2+0.5);
      if( ct< 0.) dy1 =(int)( ct*i2-0.5);
      else dy1 =(int)( ct*i2+0.5);
      xa +=dx1,xb +=dx1,ya -=dy1,yb -=dy1;
      cur_x=xa,cur_y=ya; cur_z = za;
      _draw3_( xb, yb,zb);
      for (i=1;i<ln_width;i++)
        {
             dx1 =(int)( st*i+0.5);
             if( ct< 0.) dy1 =(int)( ct*i-0.5);
             else dy1 =(int)( ct*i+0.5);
             cur_x=xa-dx1,cur_y=ya+dy1;
            _draw3_( xb-dx1,yb+dy1,zb);
        }
      }
    }
   cur_x=x;
   cur_y=y;
   cur_z=z;
   return;
 }

static  void t_3_draw(float ix,float iy,float iz)
  {
  int x1,y1,z1;
  x1=scr_x(ix);
  y1=scr_y(iy);
  z1=scr_z(iz);
  _draw3(x1,y1,z1);
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
       t_3_draw(newstr.xstr,newstr.ystr,newstr.zstr);
    }
    else t_draw(newstr.xstr,newstr.ystr);
}

static void _poly_fill3(short n,int *px,int *py,int *pz,short flag,unsigned int b_color)
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
  xup =-100000;
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
  if((scan> v_y2) || (ygrt< v_y1) || (xlow> v_x2) || (xup< v_x1)) return;
  tempc = c_color;
  c_color= b_color;
  set_clr(c_color);
  if (ymax[ind]==scan) ind++;
  while (scan<=ygrt)
  {
	 cnt=0;
	 while (ymax[ind]==scan) ++ind;  /*  excluding upper edges  */
         if(scan<v_y1) {scan++;continue;}
         if(scan>v_y2) break;
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
           hori_line3(xcord[i],xcord[i+1],scan,
                      (int)(zcord[i]+0.5),(int)(zcord[i+1]+0.5));
	 scan++;
  }
  c_color=tempc;
  set_clr(c_color);
  if (flag==1)
      for(i=0;i<n;i++) _draw_line3(px[i],py[i],pz[i],px[(i+1)%n],py[(i+1)%n],pz[(i+1)%n]);
}


static void _godard_fill3(short n,int *px,int *py,int *pz ,short flag,float *pv)
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
  unsigned int tempc;
  xup =-100000;
  xlow = 1000000;
  scan=10000;
  ygrt=-1239000;
  i=0;flg=0;
  while((i<n)&&(flg==0)) {
      if ((pz[i]>=0)&&(pz[i]<=65000L))  flg=1;
      i++;
  }
  if (flg==0) return;
  temp =c_color;
  for (i=0;i<n;++i)

  {
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
	tpx=pmax[i];
	tpn=pmin[i];
	while (j>=0 && tyx<ymax[j])
	{
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
  if((scan> v_y2) || (ygrt< v_y1) || (xlow> v_x2) || (xup< v_x1)) return;
  while ((ind<n)&&(ymax[ind]==scan)) ind++;
  if ( scan == ygrt ) { /* ie a single line panel */
    for(i=0;i<n;i++) if(xmax[i]!=xmin[i])break;
    if(i==n) i=0; /* ie for single point panel */
    godr_line3(xmin[i],xmax[i],scan,zmin[i],zmax[i],pmin[i],pmax[i]);
    scan++;
  }
  else {
    while (scan<=ygrt) {
	 cnt=0;
         if( scan < ygrt ) {
           while ((ind<n)&&(ymax[ind]==scan)) ++ind;  /*  excluding upper edges  */
         }
         if(scan<v_y1) {scan++;continue;}
         if(scan>v_y2) break;
	 for(i=ind;i<n;++i)
	 {
	  if (ymin[i]<=scan)
	  {
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
  	 godr_line3(xcord[i],xcord[i+1],scan,zcord[i],zcord[i+1],pcord[i],pcord[i+1]);
	 scan++;
    }
  }
  c_color=temp;
  set_clr(c_color);
  if (flag==1)
  for(i=0;i<n;i++) _draw_line3(px[i],py[i],pz[i],px[(i+1)%n],py[(i+1)%n],pz[(i+1)%n]);
}
static void godr_line3(int xa,int xb,int y,int za,int zb,float pa,float pb)
{
  short i,pos,p1,p2;
  char far* addr;
  int tempc;
  float tempp,pval,pfac,yy;
  short clip;
  int temp;
  int x1,x2,z1,z2;
  float cons,m;
  int z;
  yy = EVGAY-y;
  if((y>c_v_y2) || (y<c_v_y1) ) return;
  x1=xa;x2=xb;z1=za;z2=zb;
  if (((za<0)&&(zb<0))||((za>65000L)&&(zb>65000L))) return;
  clip=clip_line(&xa,&y,&xb,&y);
  if (clip==0) return;
  p1 = pa+0.5;
  p2 = pb+0.5;
  if(p1<1) p1=Cutoff;
  if(p2<1) p2=Cutoff;
  if(p1>max_clr) p1=max_clr;
  if(p2>max_clr) p2=max_clr;
  if(xa==xb) { p2=p1; }
  if( p1==p2) {
        tempc=c_color;
        if(p1> 0) c_color=p1+min_clr;
        else c_color=0;
        set_clr(c_color);
        hori_line3(xa,xb,y,za,zb);
        c_color=tempc;
        set_clr(c_color);
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
  m = (float)(z2-z1)/(float)(x2-x1);
  cons = z1 - x1*m;
  za=z_inter(xa,m,cons);zb=z_inter(xb,m,cons);
  for(i=xa;i<=xb;i++) {
    temp=(int)(pa+(i-xa)*pfac+0.5);
    if(temp<1) temp=Cutoff;
    if(temp>max_clr) temp=max_clr;
    if(temp > 0 ) temp +=min_clr;
    set_clr(temp);
    z=z_inter(i,m,cons);put_pix3(i,yy,z);
  }
  set_clr(c_color);
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
  if( (j>2)&&(j<500)) _poly_fill((short)j,x1,y1,(short)flag,(unsigned int)color);
  free(x1),free(y1);
}
static void t_3_panel(float *x,float *y,float *z,int color,int flag,int n)
  {
  int x1[20],y1[20],z1[20];
  short i;
  for(i=0;i<n;i++) 
  {
    x1[i] =scr_x(x[i]); 
    y1[i] =scr_y(y[i]);
    z1[i] =scr_z(z[i]);
  }
  _poly_fill3(n,x1,y1,z1,flag,color);
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
    t_3_panel(x,y,z,(int)color,flag,(int)n);
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


static void _box_fill3(int x1,int y1,int z1,int x2,int y2,int z2, unsigned int color)
{
  int x_min,y_min,x_max,x_max1,x_min1,y_max,i,j;
  short po1;
  unsigned int temp;
  float cons,m;
  int z;
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
  temp=c_color;
  c_color=color;
  set_clr(c_color);
  if( x1<x2 ) { x_min= x1; x_max1= x2; }
  else{ x_min= x2; x_max1= x1; }
  if(( x_min<=v_x2) && (x_max1>=v_x1))
   {
    if( y1<y2 ) { y_min= y1; y_max= y2; }
    else{ y_min= y2; y_max= y1; }
       
    if(( y_min<=v_y2) && (y_max>=v_y1))
     {
      if(x_max1>v_x2) x_max1 = v_x2;
      if(x_min<v_x1) x_min = v_x1;
      if(y_max>v_y2) y_max = v_y2;
      if(y_min<v_y1) y_min = v_y1;
      for(i=x_min;i<=x_max1;i++) {
        for (j=y_min;j<=y_max;++j)
           {z=z_inter(x_min,m,cons);put_pix3(x_min,EVGAY-j,z);}
      }
    }
}
  c_color=temp;
  set_clr(c_color);
  return;
}


static void t_3_box_fill(float x1,float y1,float z1,float x2,float y2,float z2,int color,int ib)
  {
  int xa,ya,za,xb,yb,zb;
  xa=scr_x(x1);
  ya=scr_y(y1);
  za=scr_z(z1);
  xb=scr_x(x2);
  yb=scr_y(y2);
  zb=scr_z(z1);
  _box_fill3(xa,ya,za,xb,yb,zb,color);
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
   t_3_box_fill(x1,y1,z1,x2,y2,z2,fl,ib);
   }
   else
   t_box_fill(x1,y1,x2,y2,fl,ib);
}
static void _godard_fill(short n,int *px,int *py,short flag,float *pv)
{
  short j,imd,tempc;
  short i,cnt,ind;
  int pcrl=0;
  int xcord[15];
  int xlow,xup;
  int txx;
  int ymax[500],ymin[500],xmax[500],xmin[500],tyx,tyn,txn,scan,ygrt;
  float pmax[500],pmin[500],pcord[500],tpx,tpn,tpp;
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
 scan++;
  }
  c_color=tempc; set_clr(c_color);
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
         set_clr(c_color);
        }
        hori_line(xa,xb,y);
        return;
       }
  if ((xb-xa)<10)
  {
        for(i=xa;i<=xb;i+=1) {
          tempc=(int)(pa+(i-xa)*pfac+0.5);
          if(tempc<1) tempc=1;
          if(tempc>max_clr) tempc=max_clr;
          tempc +=min_clr;
          c_color=tempc;
          set_clr(tempc);
          put_pixl(i,yy);
        }
        return;
  }
  else {
    godr_line(xa,(xa+xb)/2,y,pa,(pa+pb)*.5);
    godr_line((xa+xb)/2,xb,y,(pa+pb)*.5,pb);
  }
  return;
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
static  void t_3_godr_fill(int n,float *x,float *y,float *z,float *p)
 {
  int x1[20],y1[20],z1[20];
  float p1[20],pv;
  short i;
  for(i=0;i<n;i++)
  {
    x1[i] =scr_x(x[i]); 
    y1[i] =scr_y(y[i]);
    z1[i] =scr_z(z[i]);
  }
   for (i=0;i<n;i++) {pv=*(p++), p1[i] = pclr(pv); }
  _godard_fill3((short)n,x1,y1,z1,(short)ibdr,p1);
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
   t_3_godr_fill(n,x,y,z,v);
 }
 else
 t_godr_fill(n,x,y,v);
} 
static double comp_angle(POINT n,POINT v)
{
   double cosval,ang;
   float cp,m1,m2;
   
   cp = ((n.x * v.x)+(n.y * v.y)+(n.z * v.z));
   m1 = sqrt((double) ((n.x * n.x)+(n.y * n.y)+(n.z * n.z)));
   m2 = sqrt((double) ((v.x * v.x)+(v.y * v.y)+(v.z * v.z)));
   cosval = 0.0;
   if(m1*m2 != 0 )cosval = cp / (m1 * m2);
   if(cosval < 0. ) cosval = -cosval;
   return(cosval);
}

/* --------------   Cross Product of two vectors   ------------- */
static POINT cros_prd(POINT p,POINT p1,POINT p2)
{
   POINT n;
   n.x = -(p2.y - p.y)*(p1.z - p.z) + (p2.z - p.z)*(p1.y - p.y);
   n.y = ((p2.x - p.x)*(p1.z - p.z) - (p1.x - p.x)*(p2.z - p.z));
   n.z = -(p2.x - p.x)*(p1.y - p.y) + (p1.x - p.x)*(p2.y - p.y);
   return (n);
}
static float * computeV(int no,float *x,float *y,float *z,Dlink *ls_list) {

  int i,j,no_ls ,count;
  POINT p1,p2,p3,n,v;
  double cosval,d;
  LIGHT *lpt;
  float *s,diagonal;
  float xc,yc,zc,illu,dpcue;
  int j1,j0;
  POINT *pt,na={0,0,0},n0={0,0,0};
  
  xc = +box3d.xe;
  yc = box3d.ye;
  zc = +box3d.zc +box3d.ze;
  
  s = (float *) malloc(sizeof(float)*no);
  if(ls_list==NULL) {
   for ( i = 0; i < no;  i++) s[i]=1.0;
  }
  else {
  pt = (POINT *) malloc(sizeof(POINT)*no);

  for ( i = 0; i < no;  i++)
     /*s[i] = scale(z[i]);*/
     s[i] = 0.;
  
    for ( j = 0; j < (no) ; j++)
    {
        p1.x = x[j];    
        p1.y = y[j];    
        p1.z = z[j];
        j1 = (j+1)%no;
        j0 = (no+j-1)%no;
        p2.x = x[j1];  
        p2.y = y[j1];  
        p2.z = z[j1];
        p3.x = x[j0];  
        p3.y = y[j0];  
        p3.z = z[j0];
        pt[j] = cros_prd(p1,p2,p3);
    }
    count = no;
    for ( j = 0; j < (no) ; j++){
        n = pt[j];
        na.x += pt[j].x;
        na.y += pt[j].y;
        na.z += pt[j].z;
        if( (n.x==0)&&(n.y==0)&&(n.z==0) ) count--;
    }
    if(count !=0) {
      na.x /=count; na.y /=count; na.z /=count;
    }
    for ( j = 0; j < (no) ; j++)
    {
        n = pt[j];
        if( (n.x==0)&&(n.y==0)&&(n.z==0) ) n = na;
        illu =0;  
        Resetlink(ls_list);
        while((lpt = (LIGHT *) Getrecord(ls_list)) != NULL)
        {
           //v.x = lpt->x - x[j] - xc;
           //v.y = lpt->y - y[j] - yc;
           //v.z = lpt->z - z[j] - zc;
           v.x = lpt->x - x[j] ;
           v.y = lpt->y - y[j] ;
           v.z = lpt->z - z[j] ;

           cosval = comp_angle(v,n);
   
           d = v.x * v.x + v.y * v.y + v.z * v.z;
	   d = (sqrt(d)/(DSCALE*(box3d.d)));
	   illu +=   (scale_cos(cosval)*(dist(d)));
        }
        if(No_of_lights > 1) illu /= No_of_lights;
#if 0  /* removed from here due to the new shading scheme */
        dpcue = scale(z[j]);
        if (dpcue > 1.) dpcue=1.;
        illu *= dpcue;
#endif
        s[j] += ((illu > illm_fac) ? illm_fac :illu);
        /*if((s[j] <0) ||(s[j] >1.)) printf("Error:\n");*/
        if((s[j] <0) ) s[j]=0.;
        if((s[j] > 1) ) s[j]=1.;
    } 
  free(pt);
  }
  return s;
}
void t_win_3_shade_o()
{
  int i,l;
  float *x,*y,*z,*v;
  int n;

  read_buf(&n,4);
  l = sizeof(float)*n;
  x= (float *)malloc(l);
  y= (float *)malloc(l);
  z= (float *)malloc(l);
  read_buf(x,l);
  read_buf(y,l);
  read_buf(z,l);
  for(i=0;i<n;i++)
   {
    transfrm(*(x+i), *(y+i),*(z+i));
    *(x+i) = trnstr.xstr;
    *(y+i) = trnstr.ystr;
    *(z+i) = trnstr.zstr;
   }
  /* Compute v */
  v = computeV(n,x,y,z,ls_list);
  for(i=0;i<n;i++)
   {
    trnstr.xstr = x[i];
    trnstr.ystr = y[i];
    trnstr.zstr = z[i];
    projection(trnstr);
    *(x+i) = newstr.xstr;
    *(y+i) = newstr.ystr;
    *(z+i) = newstr.zstr;
   }
   if (ZBUFF == 1) t_3_godr_fill(n,x,y,z,v);
   else t_godr_fill(n,x,y,v);
   free(x),free(y),free(z),free(v);
} 
void t_win_3_shade()
{
  int i,l;
  float *x,*y,*z,*v;
  int n,baseclr;

  read_buf(&n,4);
  l = sizeof(float)*n;
  x= (float *)malloc(l);
  y= (float *)malloc(l);
  z= (float *)malloc(l);
  read_buf(x,l);
  read_buf(y,l);
  read_buf(z,l);
  read_buf(&baseclr,4);
  for(i=0;i<n;i++)
   {
    transfrm(*(x+i), *(y+i),*(z+i));
    *(x+i) = trnstr.xstr;
    *(y+i) = trnstr.ystr;
    *(z+i) = trnstr.zstr;
   }
  /* Compute v */
  v = computeV(n,x,y,z,ls_list);
  for(i=0;i<n;i++)
   {
    trnstr.xstr = x[i];
    trnstr.ystr = y[i];
    trnstr.zstr = z[i];
    projection(trnstr);
    *(x+i) = newstr.xstr;
    *(y+i) = newstr.ystr;
    *(z+i) = newstr.zstr;
   }
   if (ZBUFF == 1) t_3_godr_fill(n,x,y,z,v);
   else t_godr_fill(n,x,y,v);
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
//        Pnlist = Dopen();
        set_3d();
        break;
      case 'z':
        ZBUFF=1;
        t_set_zbuf();
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
        case 'i':
                  win_drawimage();
                  break;
        case 'l': win_drawline();
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
        case 'S' : t_win_3_shade_o(); break;  
        case 'H' : t_win_3_shade(); break;  
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
#if 0
                if( (A4==0))
                 {
                  A4=1;
                  LSCAPE=0;
                  printf(" A4 case\n");
                  y_mulf*=40./22.5;
                  set_a4copy();
                  evgay=EVGAY;
                 }
#endif
                break;
      case 'S': 
#if 0
                if( (LSCAPE==0))
                 {
                  A4=0;
                  LSCAPE=1;
                  y_mulf=1.;
                  printf(" Landscape case\n");
                  set_landscape();
                 }
#endif
                break;
      case 'P': 
/*
                Sort_Display(Pnlist);
                page++;
                printf("Page %4d \n",page);
                Pnlist = NULL;
                reinitialise();
                hardcopy();
                t_release_zbuf();
*/
                break;
      default : 
                printf(" Wrong Code %c \n",ch);
    }
 }

static void check_and_do_o(char ch)
 {
  int i;
  char ch2;
    switch(ch)
    {
      case 'v':
                /*printf("view \n");*/
                set_n_view();
                break;
      case 'C':
                /*printf("window \n");*/
                set_usrcord();
      case 'Z':
        Pnlist = Dopen();
        set_3d();
        break;
      case 'z':
        ZBUFF=1;
        break;
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
        case 's': win_set_lnstyle();
  	          break;
        case 'y': win_resetstyle();
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
        case 'w': win_txtwrt();
		  break;
        case 'h': 
        case 'o': win_set_txtbold();
		  break;
        case 'c': win_txt_color();
		  break;
        case 'l': win_txt_fill();
		  break;
        case 'k': win_txt_bkgr();
		  break;
        case 'p': win_txt_pattern();
		  break;
        case 'b': win_txt_bodr();
		  break;
        case 'f': win_txt_font();
		  break;
        case 'a': win_txt_slant();
		  break;
        case 's': win_txt_size();
		  break;
        case 'r': win_set_angle();
		  break;
        case 'x': win_set_prtxt();
		  break;
        default :
               printf(" Wrong Code in text :%c%c \n",ch,ch2);
               exit(0);
       }
       break;
      
     case 'p' :
       /*printf("fill \n");*/
      read_buf(&ch2,1);
      switch(ch2)
      {
        case 'b': win_boxfill();
		  break;
        case 'r': win_circlefill();
		  break;
        case 'p': win_poly_fill();
		  break;
        case 'g': win_godard_fill();
		  break;
        case 's': win_set_godard_clrs();
		  break;
        case 'h': win_set_shade_clrs();
		  break;
        case 'M': win_3_move(); break;
        case 'D': win_3_draw(); break;
       }
      break;
     case 's' :
      read_buf(&ch2,1);
      switch(ch2)
      {
        case 'c': win_clip_limit();
		  break;
        case 'p': win_pop_clip();
		  break;
        case 's': win_sup_clip_limit();
		  break;
      }
       break;
     case 'c' :
      read_buf(&ch2,1);
      switch(ch2)
      {
        case 'h': win_chng_clr();
		  break;
      }
       break;
      case 'A': 
      case 'S': 
      case 'P': 
                break;
      default : 
                printf(" Wrong Code %c \n",ch);
    }
 }

static void review_gph_file(char *str)
{
  char ch;
  float w[4];
  int x1,x2,y1,y2,color,flag,n;
  R_Byte=0;
  rbuf=open(str,O_RDWR|O_BINARY, S_IREAD|S_IWRITE);
  read(rbuf,w,16);
  user_cord(w[0],w[1],w[2],w[3]);
  read_file(rbuf); 
  R_max=Byte;
  read_buf(&ch,1);

  while ( R_max>0)
  {
  check_and_do(ch);
  read_buf(&ch,1);
  }
 close(rbuf);
}
static void review_file(char *str)
{
  char ch;
  int x1,x2,y1,y2,color,flag,n;
  rbuf=open(str,O_RDWR|O_BINARY, S_IREAD|S_IWRITE);
  read_file(rbuf); 
  R_Byte=0;
  R_max=Byte;
  read_buf(&ch,1);
  while ( R_max>0)
  {
  check_and_do(ch);
  read_buf(&ch,1);
  }
 close(rbuf);
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
  short fac=2;
  int px[4],py[4];
  short style,mf;
  style = m_style%20;
  mf = m_style/20;
  fac +=mf;
  switch(style)
  {
	case 0 :
                 _circle_fill(x,y,(int)(3*fac),(unsigned int)15);
                 break;
	case 1 :
                 _draw_line(x-3*fac,y-3*fac,x+3*fac,y+3*fac);
                 _draw_line(x-3*fac,y+3*fac,x+3*fac,y-3*fac);
                 break;
	case 2 : 
                 _box_fill(x-3*fac,y-3*fac,x+3*fac,y+3*fac,(unsigned int)15);
                 break;
	case 3 : _draw_line(x,y-4*fac,x,y+4*fac);
				_draw_line(x-4*fac,y,x+4*fac,y);
				_draw_line(x-2*fac,y-4*fac,x+2*fac,y-4*fac);
				_draw_line(x-2*fac,y+4*fac,x+2*fac,y+4*fac);
				_draw_line(x-4*fac,y-2*fac,x-4*fac,y+2*fac);
				_draw_line(x+4*fac,y-2*fac,x+4*fac,y+2*fac);
				break;
	case 4 : _draw_line(x-4*fac,y-4*fac,x+4*fac,y+4*fac);
				_draw_line(x-4*fac,y+4*fac,x+4*fac,y-4*fac);
				_draw_line(x-4*fac,y,x+4*fac,y);
				_draw_line(x,y-4*fac,x,y+4*fac);
				break;
	case 5 : _draw_line(x+3*fac,y,x,y-8*fac);
				_draw_line(x-3*fac,y,x,y-8*fac);
				_draw_line(x+3*fac,y,x,y+8*fac);
				_draw_line(x-3*fac,y,x,y+8*fac);
				put_pixel(x,y);
				break;
	case 6 : _draw_line(x-4*fac,y,x+4*fac,y);
				_draw_line(x,y-4*fac,x,y+4*fac);
				_draw_line(x+2*fac,y-2*fac,x+2*fac,y+2*fac);
				_draw_line(x+2*fac,y+2*fac,x-2*fac,y+2*fac);
				_draw_line(x-2*fac,y+2*fac,x-2*fac,y-2*fac);
				_draw_line(x-2*fac,y-2*fac,x+2*fac,y-2*fac);
				break;
	case 7 : put_pixel(x,y);
				_draw_line(x+7*fac,y,x+3*fac,y-3*fac);
				_draw_line(x+3*fac,y-3*fac,x,y-7*fac);
				_draw_line(x,y-7*fac,x-3*fac,y-3*fac);
				_draw_line(x-3*fac,y-3*fac,x-7*fac,y);
				_draw_line(x-7*fac,y,x-3*fac,y+3*fac);
				_draw_line(x-3*fac,y+3*fac,x,y+7*fac);
				_draw_line(x,y+7*fac,x+3*fac,y+3*fac);
				_draw_line(x+3*fac,y+3*fac,x+7*fac,y);
				break;
	case 8 : _draw_line(x-4*fac,y-4*fac,x+4*fac,y+4*fac);
				_draw_line(x+4*fac,y-4*fac,x-4*fac,y+4*fac);
				_draw_line(x+4*fac,y+4*fac,x+4*fac,y+2*fac);
				_draw_line(x+4*fac,y+4*fac,x+2*fac,y+4*fac);
				_draw_line(x+4*fac,y-4*fac,x+4*fac,y-2*fac);
				_draw_line(x+4*fac,y-4*fac,x+2*fac,y-4*fac);
				_draw_line(x-4*fac,y-4*fac,x-2*fac,y-4*fac);
				_draw_line(x-4*fac,y-4*fac,x-4*fac,y-2*fac);
				_draw_line(x-4*fac,y+4*fac,x-4*fac,y+2*fac);
				_draw_line(x-4*fac,y+4*fac,x-2*fac,y+4*fac);
				break;
        case 9 :
                 px[0]=x+4*fac;py[0]=y;px[1]=x-4*fac;py[1]=y+4*fac;
                 px[2]=x-4*fac;py[2]=y-4*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned int)15);
                 break;
        case 10:
                 px[0]=x-4*fac;py[0]=y;px[1]=x+4*fac;py[1]=y+4*fac;
                 px[2]=x+4*fac;py[2]=y-4*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned int)15);
                 break;
        case 11:
                 px[0]=x;py[0]=y+4*fac;px[1]=x-4*fac;py[1]=y-4*fac;
                 px[2]=x+4*fac;py[2]=y-4*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned int)15);
                 break;
        case 12:
                 px[0]=x;py[0]=y-4*fac;px[1]=x-4*fac;py[1]=y+4*fac;
                 px[2]=x+4*fac;py[2]=y+4*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned int)15);
                 break;
	case 13 :
                 _circle_fill(x,y,(int)(3*fac),(unsigned int)0);
                 _circle(x,y,(int)(3*fac));
                 break;
	case 14 :
                 _circle_fill(x,y,(int)(1*fac),(unsigned int)(15));
                 break;
	case 15:
                 _draw_line(x-2*fac,y-2*fac,x+2*fac,y+2*fac);
                 _draw_line(x-2*fac,y+2*fac,x+2*fac,y-2*fac);
                 break;
        case 16:
                 px[0]=x+2*fac;py[0]=y;px[1]=x-2*fac;py[1]=y+2*fac;
                 px[2]=x-2*fac;py[2]=y-2*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned int)15);
                 break;
        case 17:
                 px[0]=x-2*fac;py[0]=y;px[1]=x+2*fac;py[1]=y+2*fac;
                 px[2]=x+2*fac;py[2]=y-2*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned int)15);
                 break;
        case 18:
                 px[0]=x;py[0]=y+2*fac;px[1]=x-2*fac;py[1]=y-2*fac;
                 px[2]=x+2*fac;py[2]=y-2*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned int)15);
                 break;
        case 19:
                 px[0]=x;py[0]=y-2*fac;px[1]=x-2*fac;py[1]=y+2*fac;
                 px[2]=x+2*fac;py[2]=y+2*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned int)15);
                 break;
	default :break;
  }
}

static void win_set_color(void)
{
  int color;
  read_buf(&color,4);
  c_color=color;
  return;
}

static void win_set_lnstyle(void)
{
  int style;
  read_buf(&style,4);
  ln_style=style;
  return;
}
void win_resetstyle(void)
{
 int i;
 for(i=0;i<10;i++) st_ptr[i]=0;
}

static void win_set_angle(void)
{
  float t;
  float fact,xfact,yfact;
  read_buf(&t,4);
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
	 if (c & 0x01)
	 {
                if(tx==0) return(0);
		y=(int)(*ya+(c_v_x1-(*xa))*ty/tx);
		if ((y==ys) && ((ys>c_v_y2) || (ys<c_v_y1))) return(0);
		ys=y;
		x=c_v_x1;
                goto jump;
	 }
	 if (c & 0x02)
	 {
                if(tx==0) return(0);
		y=(int)(*ya+(c_v_x2-(*xa))*ty/tx);
                if ((y==ys) && ((ys>c_v_y2) || (ys<c_v_y1))) return(0);
		ys=y;
		x=c_v_x2;
                goto jump;
	 }
	 if (c & 0x08)
	 {
                if(ty==0) return(0);
		x=(int)(*xa+(c_v_y2-(*ya))*tx/ty);
		if (x==xs && ((xs<c_v_x1) || (xs>c_v_x2))) return(0);
		xs=x;
		y=c_v_y2;
                goto jump;
	 }
	 if (c & 0x04)
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


static  void mline( short ixb, short iyb, short ixe,short iye)
   {
     short i,ix,iy,ixdf,iydf,id,ixl,ixu,iyl,iyu;
     float dx,dy,xx,yy;

      ixl = ixb;
      ixu = ixe;
      ixdf = ixe - ixb;
      iydf = iye - iyb;
      if ( ixb > ixe ) { ixl = ixe; ixu = ixb;}
      iyl = iyb;
      iyu = iye;
      if ( iyb > iye ) { iyl = iye; iyu = iyb;}
      if ( ixl > EVGAX) return;
      if( ixu < 0) return;
      if ( iyl > EVGAY ) return;
      if( iyu <0) return;
      id = ixu-ixl;
      if (id < iyu-iyl ) id = iyu-iyl;
      if (id != 0)
      {
       dx = ixdf/(float)id;
       dy = iydf/(float)id;
       xx = ixb+0.5;
       yy = iyb+0.5;
       for (i=0;i<id;i++)
      {
           ix = xx;
           iy = yy;
           put_pixl(ix,EVGAY-iy);
           xx = xx +dx;
           yy = yy +dy;
      }
     }
      return;
 }

       
static void _draw_line(int x1,int y1,int x2,int y2)
{
 char far *addr;
 short i,dx,dy,f,g,inc1,inc2,ab,dx1,dy1,miny,maxy,minx,maxx;
 short clip,ch,rh,r,c;
 unsigned int temp;
 int xa,ya,xb,yb,dum;
 xa =x1,xb=x2,ya=y1,yb=y2;
 if( xa>xb){dum=xa,xa=xb,xb=dum,dum=ya,ya=yb,yb=dum;};
 clip=clip_line(&xa,&ya,&xb,&yb);
 if (clip==0) return;
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
	            put_pixl(c,r);
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
         	   put_pixl(r,c);
                   c-=ch;
        	   if (g > 0) { r+=rh; g+=inc2; }
         	   else g+=inc1;
	         }
          }
          put_pixl(xb,(EVGAY-yb));
  }
  else
     {
	if (xa==xb)
	{
          if ( ya < yb){ miny = ya;maxy = yb;}
          else { miny = yb; maxy = ya;}
  	  for (i=miny;i<=maxy;++i) put_pixl(xa,(EVGAY-i));
	}
       else if (ya==yb)
	{
          if ( xa < xb){ minx = xa;maxx = xb;}
          else { minx = xb; maxx = xa;}
          for (i=minx;i<=maxx;++i) put_pixl(i,(EVGAY-ya));
	}
     }
	return;
}

static void draw_line(int xa,int ya,int xb,int yb)
 {
   int i,i2;
   float dx,dy,ct,st,dl;
   int x1,y1,x2,y2,dx1,dy1;
   if( ln_width <=1 ){ _draw_line( xa, ya, xb, yb);return;};
   if( xa>xb){x1=xa,xa=xb,xb=x1,y1=ya,ya=yb,yb=y1;};
   i2=ln_width/2;
   dx = (float)(xb-xa);
   dy = (float)(yb-ya);
   dl = (dx*dx+dy*dy);
   if( dx==0.) {ct=0.0;st=1.;}
   else { dl =sqrt((double)dl),ct = (dx)/dl, st = (dy)/dl;}
   dx1 =(int)( st*i2+0.5);
   if( ct< 0.) dy1 =(int)( ct*i2-0.5);
   else dy1 =(int)( ct*i2+0.5);
   xa +=dx1,xb +=dx1,ya -=dy1,yb -=dy1;
   _draw_line( xa, ya, xb, yb);
   for (i=1;i<ln_width;i++)
        {
             dx1 =(int)( st*i+0.5);
             if( ct< 0.) dy1 =(int)( ct*i-0.5);
             else dy1 =(int)( ct*i+0.5);
            _draw_line( xa-dx1,ya+dy1,xb-dx1,yb+dy1);
        }
   return;
 }
static void hori_line(int xa,int xb,int y)
{
  short i,clip;
  char far *addr;
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
  for(i=xa;i<=xb;i++) put_pixl(i,(EVGAY-y));
}
static void phori_line(int xa,int xb,int y)
{
  short i,j,ip,ib,jp,clip,pt_no;
  char far *addr;
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
  pt_no=(unsigned int)c_color;
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
                        put_dot(i,j);
                      }
}

static void vert_line(int ya,int yb,int x)
{
  short i,clip;
  char far *addr;
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
  for(i=ya;i<=yb;i++) put_pixl(x,(EVGAY-i));
}

static void _draw_(int x, int y)
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
  {
   clip=clip_line(&xa,&ya,&xb,&yb);
   if (clip == 0)
   {
	cur_x=x;
	cur_y=y;
        return;
   }
   ptn=ln_ptn[ln_style%10];
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
   i=st_ptr[ln_style%10];
   while (abs(c-f)>0)
   { 
      i %=32;
      if( (ptn&(((unsigned int)0x80000000)>>i)) !=0){
        if ( dx1 > dy1 ) put_pixl(c,(EVGAY-r));
        else   put_pixl(r,(EVGAY-c));
      }
      c+=ch;
      if (g > 0) { r+=rh; g+=inc2; }
      else g+=inc1;
      i++;
    }
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
     if(ln_width<=1) _draw_( x, y);
     else
      {
      stloc=st_ptr[ln_style%10];
      if( xa>xb){x1=xa,xa=xb,xb=x1,y1=ya,ya=yb,yb=y1;};
      i2=ln_width/2;
      dx = (float)(xb-xa);
      dy = (float)(yb-ya);
      dl = (dx*dx+dy*dy);
      if( dx==0.) {ct=0.0;st=1.;}
      else { dl =sqrt((double)dl),ct = (dx)/dl, st = (dy)/dl;}
      dx1 =(int)( st*i2+0.5);
      if( ct< 0.) dy1 =(int)( ct*i2-0.5);
      else dy1 =(int)( ct*i2+0.5);
      xa +=dx1,xb +=dx1,ya -=dy1,yb -=dy1;
      cur_x=xa,cur_y=ya;
      _draw_( xb, yb);
      for (i=1;i<ln_width;i++)
        {
             st_ptr[ln_style%10]=stloc;
             dx1 =(int)( st*i+0.5);
             if( ct< 0.) dy1 =(int)( ct*i-0.5);
             else dy1 =(int)( ct*i+0.5);
             cur_x=xa-dx1,cur_y=ya+dy1;
            _draw_( xb-dx1,yb+dy1);
        }
      }
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
  txt_w42 = txt_wt/CFact;
  txt_h42 = txt_ht/CFact;
  return;
}
static void win_txt_color( void)
 {
  int  color;
  read_buf(&color,4);
  t_color= color;
 }
static void win_txt_fill( void)
 {
  int  color;
  read_buf(&color,4);
  t_fill= color;
 }
static void win_txt_slant( void)
 {
  float  slant;
  read_buf(&slant,4);
  Slant=tan((double)slant*3.14159265/180.0);
  Slant_o=Slant;
 }
static void win_txt_bodr( void)
 {
  int  color;
  read_buf(&color,4);
  t_bodr= color;
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
 }
#if 0
static int Loadfontstruct_o(void) {
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
/*
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf37; pt->m_f = m_f37; pt->icxv = icxvf37;
  pt->icyv = icyvf37; pt->fontname = fontname37;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf38; pt->m_f = m_f38; pt->icxv = icxvf38;
  pt->icyv = icyvf38; pt->fontname = fontname38;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf39; pt->m_f = m_f39; pt->icxv = icxvf39;
  pt->icyv = icyvf39; pt->fontname = fontname39;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf40; pt->m_f = m_f40; pt->icxv = icxvf40;
  pt->icyv = icyvf40; pt->fontname = fontname40;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf41; pt->m_f = m_f41; pt->icxv = icxvf41;
  pt->icyv = icyvf41; pt->fontname = fontname41;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf42; pt->m_f = m_f42; pt->icxv = icxvf42;
  pt->icyv = icyvf42; pt->fontname = fontname42;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf43; pt->m_f = m_f43; pt->icxv = icxvf43;
  pt->icyv = icyvf43; pt->fontname = fontname43;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf44; pt->m_f = m_f44; pt->icxv = icxvf44;
  pt->icyv = icyvf44; pt->fontname = fontname44;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf45; pt->m_f = m_f45; pt->icxv = icxvf45;
  pt->icyv = icyvf45; pt->fontname = fontname45;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf46; pt->m_f = m_f46; pt->icxv = icxvf46;
  pt->icyv = icyvf46; pt->fontname = fontname46;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf47; pt->m_f = m_f47; pt->icxv = icxvf47;
  pt->icyv = icyvf47; pt->fontname = fontname47;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf48; pt->m_f = m_f48; pt->icxv = icxvf48;
  pt->icyv = icyvf48; pt->fontname = fontname48;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf49; pt->m_f = m_f49; pt->icxv = icxvf49;
  pt->icyv = icyvf49; pt->fontname = fontname49;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf50; pt->m_f = m_f50; pt->icxv = icxvf50;
  pt->icyv = icyvf50; pt->fontname = fontname50;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf51; pt->m_f = m_f51; pt->icxv = icxvf51;
  pt->icyv = icyvf51; pt->fontname = fontname51;
  Dadd(Fontlist,pt);
  pt = (FONT *)malloc(sizeof(FONT));
  pt->icpos = icposf52; pt->m_f = m_f52; pt->icxv = icxvf52;
  pt->icyv = icyvf52; pt->fontname = fontname52;
  Dadd(Fontlist,pt);
*/
  return 1;
}
#endif

static void win_txt_font( void)
 {
  int font;
  FONT *pt;
  int count;
  read_buf(&font,4);
  if(Fontlist == NULL ) Fontlist =Loadfontstruct();
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
  if(Fontlist == NULL ) Fontlist=Loadfontstruct();
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
static void t_txt_font( int font)
 {
  FONT *pt;
  int count;
  if(Fontlist == NULL ) Loadfontstruct();
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

static void drawg0ch(c)
char c;
{
  float x[3000],y[3000];
  int x1[10000],y1[10000],x2[10000],y2[10000];
  int code,n,i,xdum,ydum,pnts;
  short ptr;
  short j,pnt,asc,tempc;
  float fact=1.0;
  int xo,yo;
  unsigned int color;
  set_greek;
  if (t_pattern==15) color=t_color;
  else color=t_pattern;
  tempc=ln_width;
  ln_width=txt_bold;
  asc=c;
  pnt=asc-32+greek;
  fact = m_f[pnt];
  xo=(int)(cx+(xp));
  if( pr_txt==0) { fact=1.0;}
  /*else if(greek> 0) xo -=(1.-fact)*0.5*txt_wt;*/
  yo=(int)(cy+yp);
  if (pnt<0) return;
  if(pnt>223) return;
  ptr= icpos[pnt];
  if((xo>=c_v_xt1)&&(xo<=c_v_x2)&& (yo>=c_v_yt1)&& (yo<=c_v_y2))
     {
     code= icxv[ptr];n=icyv[ptr];
     pnts=0;
     while ( code >= 0)
      {
        ptr++;
        for(i=0;i<n;i++){xdum=icxv[ptr];ydum=icyv[ptr];ptr++;
                         xdum +=ydum*Slant;
                         x[i]=xdum*txt_w42+xo;y[i]=yo+ydum*txt_h42;}
        pnts +=t_fillchr(x,y,n,&x1[pnts],&y1[pnts],&x2[pnts],&y2[pnts]);
        code= icxv[ptr];n=icyv[ptr];
        if(code < 0)
         {
         if(t_fill)
          {
          _mpoly_fill((short)pnts,x1,y1,x2,y2,color);
          }
         if(t_bodr){
             temp =c_color;set_clr(t_color);
             for(i=0;i<pnts;i++) draw_line(x1[i]/Mag,y1[i]/Mag,x2[i]/Mag,y2[i]/Mag);
             set_clr(temp);
            }
          pnts=0;
         }
      }
     }
     xp+=(txt_wt*fact+txt_sp);
     ln_width=tempc;
     reset_greek;
     return;
 }
static void drawgch(c)
char c;
{
  float x[300],y[300];
  int code,n,i;
  int x1[5000],y1[5000],x2[5000],y2[5000];
  int pnts;
  float xdum,ydum;
  short ptr;
  short j,pnt,asc,tempc;
  float xa,ya;
  float fact=1.0;
  float xo,yo;
  unsigned int color;
  set_greek;
  if (t_pattern==15) color=t_color;
  else color=t_pattern;
  tempc=ln_width;
  ln_width=txt_bold;
  asc=c;
  pnt=asc-32+greek;
  fact = m_f[pnt];
  xo=(int)((xp));
  if( pr_txt==0) { fact=1.0;}
  /*else if(greek> 0) xo -=(1.-fact)*0.5*txt_wt;*/
  yo=(int)(yp);
  if (pnt<0) return;
  if(pnt>223) return;
  ptr= icpos[pnt];
  xa=(cx+(xo)*cost+yo*sint);
  ya=(cy-(xo)*sint+yo*cost);
  if((xa>=c_v_xt1)&&(xa<=c_v_x2)&& (ya>=c_v_yt1)&& (ya<=c_v_y2))
     {
     code= icxv[ptr];n=icyv[ptr];
     pnts=0;
     while ( code >= 0)
      {
        ptr++;
        for(i=0;i<n;i++){xdum=icxv[ptr];ydum=icyv[ptr];ptr++;
             xdum += ydum*Slant;
     xdum=(xo+xdum*txt_w42);
     ydum=(yo+ydum*txt_h42);
     x[i]=(cx+(xdum)*cost+(ydum)*sint);
     y[i]=(cy+(ydum)*cost-(xdum)*sint);
            }
        pnts +=t_fillchr(x,y,n,&x1[pnts],&y1[pnts],&x2[pnts],&y2[pnts]);
        code= icxv[ptr];n=icyv[ptr];
        if(code <0)
         {
         if(t_fill)
          {
          _mpoly_fill((short)pnts,x1,y1,x2,y2,color);
          }
         if(t_bodr){
             temp =c_color;set_clr(t_color);
             for(i=0;i<pnts;i++) draw_line(x1[i]/Mag,y1[i]/Mag,x2[i]/Mag,y2[i]/Mag);
             set_clr(temp);
            }
          pnts=0;
         }
      }
     }
     xp+=(txt_wt*fact+txt_sp);
     ln_width=tempc;
     reset_greek;
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
static int t_fillchr(float *x,float *y,int n,int *x1,int *y1,int *x2,int*y2)
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
   if(code==0) _poly_fill((short)j,x1,y1,(short)0,(unsigned int)t_bkgr);
   else _poly_fill((short)j,x1,y1,(short)0,(unsigned int)t_pattern);
   }
  if(t_bodr){
             temp =c_color;set_clr(t_color);
             _move(x1[0],y1[0]);
             for(i=1;i<j;i++)_draw(x1[i],y1[i]);
             _draw(x1[0],y1[0]);
             set_clr(temp);
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

static   void setsubsup(float *fact,short *ishft,char cntl)
     {
      L_N *pt;
      *fact = (*fact)*SSF;
      txt_w42 = SSF*txt_w42;
      txt_h42 = SSF*txt_h42;
      txt_wt = SSF*txt_wt;
      txt_ht = SSF*txt_ht;
      if( cntl=='S'){
                      *ishft = (*ishft)+1;
                      yp = yp+1.4*txt_ht;
                    }
       else yp= yp-0.7*txt_ht;
       pt=O_L;
       while(pt!=NULL){
          if((pt->x2) < 0.){
           if((pt->ymax)< (yp+1.4*txt_ht)) pt->ymax=yp+1.4*txt_ht;
           if((pt->ymin)> (yp-0.4*txt_ht)) pt->ymin=yp-0.4*txt_ht;
          }
          pt=pt->Pr;
       }
      *ishft = (*ishft)*10;
      return;
      }
static   void resetsubsup(fact,ishft)
  float *fact;
  short *ishft;
   {
     short is;
      *ishft = (*ishft)/10;
      is = *ishft-((*ishft)/10)*10;
      if( is==1){
                      *ishft = (*ishft)-1;
                      yp = yp-1.4*txt_ht;
                }
      else yp= yp+0.7*txt_ht;
      *fact = (*fact)/SSF;
      txt_w42 = txt_w42/SSF;
      txt_h42 = txt_h42/SSF;
      txt_wt = txt_wt/SSF;
      txt_ht = txt_ht/SSF;
      return;
   }

static void  win_txtwrt(void)
  {
  short i=0,bold,tempc,ishft,trot,Nu,De,gap,lnwidth_o,t_color_o;
  float fact,val,xl1,xl2,hfact=1.0,slant;
  int font_o;
  L_N *FO_L=NULL,*pt=NULL;
  B_K *FB_P=NULL;
  char ch,cntl;
  int nchr,j;
  unsigned char *txt;
  font_o=t_font;
  read_buf(&nchr,4);
  txt= (unsigned char *) malloc((nchr+1)*sizeof(unsigned char));
  read_buf(txt,nchr);
  txt[nchr]='\0';
  j=strlen(txt);
  bold=txt_bold;
  O_L=NULL;
  bold = txt_bold;
  slant=Slant;
  trot = (cost<0.999999);
  tempc=c_color;
  c_color = t_color;
  t_color_o=t_color;
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
           if(i >= j) break;
           cntl = txt[i];
           if( (cntl=='S')|| (cntl=='s') )
                 setsubsup(&fact,&ishft,cntl);
           else
             {
               switch (cntl)
                 {
                   case 'e':
                            resetsubsup(&fact,&ishft);
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
                            if((i+1) >=j ) break;
                            ch=txt[i+1];txt[i+1]='\0';
                            gap=strlngth((char *)txt,&xl2);
                            txt[i+1]=ch;
                            xp = (gap+1)*(txt_wt+txt_sp);
                            break;
                   case 'f':
                            if((i+2) >=j ) break;
                            Nu= (txt[i+1] -'0');
                            Nu= Nu*10+(txt[i+2] -'0');
                            i+=2;
                            t_txt_font((int)Nu);
                            break;
                   case 'c':
                            if((i+2) >=j ) break;
                            Nu= (txt[i+1] -'0');
                            Nu= Nu*10+(txt[i+2] -'0');
                            i+=2;
                            c_color=(int)Nu;
                            t_color=c_color;
                            break;
                   case 'z':
                            if((i+2) >=j ) break;
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            if(val==0.0) val=1.0;
                            fact = fact*val;
                            hfact =hfact*val;
                            i +=2;
                            txt_w42 = txt_w42*val;
                            txt_h42 = txt_h42*val;
                            txt_wt = txt_wt*val;
                            txt_ht = txt_ht*val;
                            break;
                   case 'h':
                            if((i+2) >=j ) break;
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            if(val==0.0) val=1.0;
                            hfact =hfact*val;
                            i +=2;
                            txt_h42 = txt_h42*val;
                            txt_ht = txt_ht*val;
                            break;
                   case 'w':
                            if((i+2) >=j ) break;
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            if(val==0.0) val=1.0;
                            fact = fact*val;
                            i +=2;
                            txt_w42 = txt_w42*val;
                            txt_wt = txt_wt*val;
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
/*    txt_bold=bold;*/
      lnwidth_o=ln_width;
      t_color =t_color_o;
      ln_width=2*POINTSIZE;
      pt=FO_L;
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
       O_L=NULL;
      }
      O_P=FB_P;
      while(O_P != NULL) {
       D_P=O_P;
       O_P=O_P->Pr;
       free(D_P);
       D_P=NULL;
      }
      ln_width=lnwidth_o;
      c_color= tempc;
      if(t_font!=font_o) t_txt_font(font_o);
      free(txt);
  }
static void  win_txtwrt_o(void)
{
  short i=0,bold,j,pnt,tempc,ishft,trot,Nu,De,gap;
  float fact,val,xl1,xl2,hfact,slant;
  int nchr;
  unsigned char txt[200],cntl;
  unsigned char *tx;
  tx= (unsigned char *) txt;
  read_buf(&nchr,4);
  read_buf(tx,nchr);
  txt[nchr]='\0';
  bold=txt_bold;
  tempc=c_color;
  c_color = t_color;
  slant=Slant;
  trot = (cost<0.999999);
  cx=cur_x+0.5;
  cy=cur_y+0.5;
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
           if((i) >=j ) break;
           cntl = txt[i];
           if( (cntl=='S')|| (cntl=='s')) 
                 setsubsup(&fact,&ishft,cntl);
           else
             {
               switch (cntl)
                 {
                   case 'e':
                            resetsubsup(&fact,&ishft);
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
                   case 'I':
                            Slant = Slnt[1];
                            break;
                   case 'N':
                            Slant = Slant_o; txt_bold=txt_bold_o;
                            break;
                   case 'g':
                            greek=128;
                            break;
                   case '%':
                            if((i+1) >=j ) break;
                            ch=txt[i+1];txt[i+1]='\0';
                            gap=strlngth((char *)txt,&xl2);
                            txt[i+1]=ch;
                            xp = (gap+1)*(txt_wt+txt_sp);
                            break;
                   case 'z':
                            if((i+2) >=j ) break;
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            fact = fact*val;
                            hfact = hfact*val;
                            i +=2;
                            txt_w42 = txt_w42*val;
                            txt_h42 = txt_h42*val;
                            txt_wt = txt_wt*val;
                            txt_ht = txt_ht*val;
                            break;
                   case 'h':
                            if((i+2) >=j ) break;
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            hfact = hfact*val;
                            i +=2;
                            txt_h42 = txt_h42*val;
                            txt_ht = txt_ht*val;
                            break;
                   case 'w':
                            if((i+2) >=j ) break;
                            Nu= (txt[i+1] -'0');
                            De= (txt[i+2] -'0');
                            if( De == 0) De=1;
                            val = (float)Nu/(float) De;
                            fact = fact*val;
                            i +=2;
                            txt_w42 = txt_w42*val;
                            txt_wt = txt_wt*val;
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
      txt_ht = txt_ht/hfact;
      txt_w42 = txt_w42/fact;
      txt_h42 = txt_h42/hfact;
      c_color= tempc;
/*    txt_bold=bold;*/
} 

static void _box_fill(int x1,int y1,int x2,int y2,unsigned int color)
{
  int x_min,y_min,x_max,x_max1,x_min1,y_max,i,j;
  short po1;
  unsigned int temp;
  temp=c_color;
  c_color=color;
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
      for (j=y_min;j<=y_max;++j) phori_line(x_min,x_max1,j);
    }
   }
  c_color=temp;
  return;
}


static void clear_screen()
{
 _box_fill(v_x1,v_y1,v_x2,v_y2,0x00);
 return;
}


static void _poly_fill(short n,int *px,int *py,short flag,unsigned int b_color)
{
  short j,imd;
  short i,cnt,ind;
  int xcord[50];
  int xlow,xup;
  int txx;
  int ymax[500],ymin[500],xmax[500],xmin[500],tyx,tyn,txn,scan,ygrt;
  unsigned int tempc;
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
  if((scan> c_v_y2) || (ygrt< c_v_y1) || (xlow> c_v_x2) || (xup< c_v_x1)) return;
  tempc = c_color;
  c_color= b_color;
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
           phori_line(xcord[i],xcord[i+1],scan);
	 scan++;
  }
  c_color=tempc;
  if (flag==1)
      for(i=0;i<n;i++) _draw_line(px[i],py[i],px[(i+1)%n],py[(i+1)%n]);
}

static void _mpoly_fill_o(short n,int *px,int *py,int *px1,int *py1,unsigned int b_color)
{
  short j,imd;
  short i,cnt,ind;
  int xcord[1500];
  int xlow,xup;
  int txx;
  int ymax[1500],ymin[1500],xmax[1500],xmin[1500],tyx,tyn,txn,scan,ygrt;
  unsigned int tempc;
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
		while((j>=0) && (txx < xcord[j])){ xcord[j+1]=xcord[j];j--;}
		xcord[j+1]=(txx);
	 }
	 /*displaying scan lines*/
	 for (i=0;i<cnt;i+=2)
           phori_line(xcord[i],xcord[i+1],scan);
	 scan++;
  }
  c_color=tempc;
}

static void _mpoly_fill(short n,int *px,int *py,int *px1,int *py1,unsigned int b_color)
{
  short j,imd;
  short i,cnt,ind;
  int xcord[5000];
  int xlow,xup;
  int txx;
  int ymax[5000],ymin[5000],xmax[5000],xmin[5000],tyx,tyn,txn,scan,ygrt;
  unsigned int tempc;
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
  if((scan/Mag> c_v_y2) || (ygrt/Mag< c_v_y1) || (xlow/Mag> c_v_x2) || (xup/Mag< c_v_x1)) return;
  tempc = c_color;
  set_clr(b_color);
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
     phori_line((int)(xcord[i]/Mag+0.5),(int)(xcord[i+1]/Mag),
               ((float)scan/Mag+0.5));
 }
 scan++;
 }
 set_clr(tempc);
}
static void _circle(int x,int y,int r)
{
  short i,j;
  int ym,sqrd;
  sqrd=r*r;
  ym=y-(short)(r/sqrt((double)2.));
  for (j=y;j>=ym;--j)
  {
	i=x+(short)sqrt((double)sqrd-((y-j)*(y-j)));
	put_pixel(i,j);
	put_pixel(i,2*y-j);
	put_pixel(x+y-j,y+i-x);
	put_pixel(x-y+j,y+i-x);
	put_pixel(2*x-i,2*y-j);
	put_pixel(2*x-i,j);
	put_pixel(x-y+j,y-i+x);
	put_pixel(x+y-j,y-i+x);
  }
}

static void _circle_fill(int x,int y,int r,unsigned int color)
{
  int i,j;
  int xm,ym;
  double t,sqrd;
  unsigned int temp;
  sqrd=(double)r*(double)r;
  ym=y-(short)(r/sqrt((double)2.));
  temp=c_color;
  put_pixel(x,y+r),put_pixel(x,y-r);
  put_pixel(x+r,y),put_pixel(x-r,y);
  c_color=color;
  phori_line(x-r+1,x+r-1,y);
  for(j=1;j<r;j++)
    {
	t=sqrd-(float)(j*j);
	i=(sqrt((double)t))+0.5;
        c_color=temp;
        put_pixel(x-i,y-j);
        put_pixel(x+i,y-j);
        put_pixel(x-i,y+j);
        put_pixel(x+i,y+j);
        c_color=color;
        phori_line(x-i+1,x+i-1,y-j);
        phori_line(x-i+1,x+i-1,y+j);
    } 
  c_color=temp;
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
static void _rect_fill(short x1,short y1,short x2,short y2,unsigned int color)
{
  short x_min,y_min,x_max,x_max1,x_min1,y_max,i,j,po1;
  unsigned int temp;
  temp=c_color;
  c_color=color;
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
      for (j=y_min;j<=y_max;++j) phori_line(x_min,x_max1,j);
    }
}
  c_color=temp;
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
      float wd,gp,fj,fjl,gj,val,fact,fact1=1.0,hfact=1.0;
      short ngp,n,i,j,k,greek=0;
      int font_o,Nu,De;
      B_K *FB_P=NULL;
      font_o=t_font;
      wd = txt_wtx;
      gp = txt_spx;
      ngp =1;
      j =0; while( title[j] != '\0')j++;
      if(j==0){ *xdsp=0.;return(0);}
      fj =0.0; gj = -1; fact = 1.0;
      fjl=0.;
      i=0;
      while(i<j) 
      {
       if( title[i] != '!') {
            if( pr_txt>0) fact1=m_f[title[i]-32+greek]*fact;
            else fact1=1.0;
            gj +=1.;
            fjl +=1.0;
            fj +=fact1;greek=0;
       }
       else { 
             i=i+1; 
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
                        if(O_P!=NULL){
                          fj=O_P->x;
                          fjl=O_P->xl;
                          gj=O_P->y;
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
                        (O_P->x) = fj;
                        (O_P->xl) = fjl;
                        (O_P->y) = gj;
                        break;
               case '!':
                         fj = fj+fact1;fjl+=1.; gj = gj+1;
                         break;
               case '%':
                         fj = fjl;
                         break;
               case 'f':
                         Nu= (title[i+1] -'0');
                         Nu= Nu*10+(title[i+2] -'0');
                         set_txt_font((int)Nu);
                         i+=2;
                         break;
               case 'c':
                         i+=2;
                         break;
               case 'z':
                         Nu= (title[i+1] -'0');
                         De= (title[i+2] -'0');
                         if( De == 0) De=1;
                         val = (float)Nu/(float) De;
                         if(val == 0.0) val=1.0;
                         fact = fact*val;
                         i = i+2;
                         break;
               case 'h':
                         Nu= (title[i+1] -'0');
                         De= (title[i+2] -'0');
                         if( De == 0) De=1;
                         val = (float)Nu/(float) De;
                         if(val == 0.0) val=1.0;
                         hfact = hfact*val;
                         i = i+2;
                         break;
               case 'w':
                         Nu= (title[i+1] -'0');
                         De= (title[i+2] -'0');
                         if( De == 0) De=1;
                         val = (float)Nu/(float) De;
                         if(val == 0.0) val=1.0;
                         fact = fact*val;
                         i = i+2;
                         break;
               default :
                         break;
              }
            }
       i = i+1;
      }
      *xdsp =(fj*wd+gj*gp);
      ngp = gj+0.1;
      O_P=FB_P;
      while(O_P != NULL) {
       D_P=O_P;
       O_P=O_P->Pr;
       free(D_P);
      }
      if(t_font!=font_o) t_txt_font(font_o);
      return(ngp);
  }
int gphStringLength(char *str,float *lngth,int font,float width,float gap) {
/* returns no of Gaps */
   int ret =0,t_fonto;
   
  *lngth=0;
   set_txt_font(font);
   txt_wtx=width;
   txt_spx=gap;
   pr_txt=1;

   ret= strlngth(str,lngth);
   return ret;
}
static void win_set_lnwidth(void)
{
  int width;
  read_buf(&width,4);
  ln_width= width*POINTSIZE;
  return;
}
static void win_set_txtbold(void)
{
  int bold;
  read_buf(&bold,4);
  txt_bold=bold;
  txt_bold_o=bold;
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
    printf("In Write_buf\n");
    for(i=0;i<n;i++) Rbuff[Byte++]=*(ptr+i);
    if ( Byte> B_max)
      { write(rbuf,(const void *)Rbuff,Byte);Byte=0;BACK_UP=1;}
  }
static void win_godard_fill(void)
{
  int n,i;
  float x[500];
  float y[500],p[500],p1[500];
  int x1[500],y1[500];
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
   max_clr = n;
   ibdr = ib;
   dfac=(n-1)/(pmax-pmin);
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
   max_clr = n;
   ibdr = ib;
   dfac=(n-1)/(pmax-pmin);
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
     if( ix2<c_v_x1) {c_v_x1=1,c_v_x2=0,c_v_y1=1,c_v_y2=0;return;};
     if( ix1>c_v_x2) {c_v_x1=1,c_v_x2=0,c_v_y1=1,c_v_y2=0;return;};
     if( iy2<c_v_y1) {c_v_x1=1,c_v_x2=0,c_v_y1=1,c_v_y2=0;return;};
     if( iy1>c_v_y2) {c_v_x1=1,c_v_x2=0,c_v_y1=1,c_v_y2=0;return;};
     if(ix1>=c_v_x1) c_v_x1= ix1;
     if(ix2<=c_v_x2) c_v_x2= ix2;
     if(iy1>=c_v_y1) c_v_y1= iy1;
     if(iy2<=c_v_y2) c_v_y2= iy2;
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
        temp=(CLIP *)c_loc->pt;
        free( c_loc);
        c_loc=(CLIP *)temp;
       }
   }
static void setpal(int r,int g, int b, int i){
    kgIcodeLoc[i][0]=r;
    kgIcodeLoc[i][1]=g;
    kgIcodeLoc[i][2]=b;
    return;
}
static void win_chng_clr()
{
  int no,ir,ig,ib;
  read_buf(&no,4);
  read_buf(&ir,4);
  read_buf(&ib,4);
  read_buf(&ig,4);
  setpal((int)ir,(int)ig,(int)ib,(int)no);
}

static int DPI=75;
static  void set_landscape(void)
   {
#if 0
       EVGAX=10*DPI;
       EVGAY = 7.5*DPI;
       evgax=EVGAX,evgay=EVGAY;
       MAXB= evgax;
       InitTiffBuf(EVGAX,EVGAY);
#endif
       return;
   }
static  void set_a4copy(void)
   {
#if 0
       EVGAY = 10*DPI;
       EVGAX=7.5*DPI;
       evgax=EVGAX,evgay=EVGAY;
       MAXB= evgax;
       InitTiffBuf(EVGAX,EVGAY);
#endif
   }
static  short ichk_blnk( unsigned char  *c,int n)

  {
    int i;
    for (i=0;i<n;i++) if( c[i] !=0 ) return(1);
    return(0);
  }
static  void hardcopy()
   {
//      WriteTiff();
      return;
   }
static void put_pixl(short col, short row)
{
  int addr,Row;
  if(col >= MAXB) return;
  if ((col >=c_v_x2) || (col <c_v_x1) ) return;
  Row= row;
  addr = ((Row)*MAXB + col);
//  *(buffer+addr) = imgGetColor(c_color);
  imgGetColor(c_color,addr);
  return;
}

static void put_dot(short col, short row)
{
  short pos,j,temp;
  int addr;
  addr = ((row)*MAXB + (col));
//  *(buffer+addr) = imgGetColor(c_color);
  imgGetColor(c_color,addr);
  return;
}
static void put_blnk(short col, short row)
{
  short pos,j,temp;
  int addr;
  addr = ((row)*MAXB + (col));
//  *(buffer+addr) =imgGetColor(0);
  imgGetColor(c_color,addr);
  return;
}
static void put_pixel(int col, int row)
{
  short pos,j,temp;
  int addr;
  if(col >= MAXB) return;
  if ((col >= c_v_x2) || (col <c_v_x1) || (row > c_v_y2) || (row <c_v_y1)) return;
  addr = (evgay-row)*MAXB + (col);
//  *(buffer+addr)  = imgGetColor(c_color);
  imgGetColor(c_color,addr);
  return;
}
unsigned long *uiGetAntialiasedData_nogm(char *flname,int xsize,int ysize) {
//For Creating a high resolution Image From gphfile and 
//Convert back to original so that antialiased image is created
// Args are review file, xsize,ysize
  int i,ln;
  char ch;
  float fact;
  FILE *fp;
  void *img,*rzimg;
  unsigned long *data=NULL;
  Lock;
  EVGAX=xsize*MFAC;
  EVGAY = ysize*MFAC;
  MAXB= EVGAX;
  fact=1;
  evgax=EVGAX,evgay=EVGAY;
  MAXB= evgax;
  fp = fopen(flname,"r");
  if(fp != NULL) {
    fclose(fp);
//    printf("Calling review_gph_file :%s\n",flname);
    SetIcode();
    initialise();
    review_gph_file(flname);
//    printf("Finished review\n");
  }
  else  {
     printf("Failed open %s\n",flname);
     Unlock;
     return NULL;
  }
  data = buffer;
  buffer=NULL;
  Unlock;
  return data;
 }
unsigned long *uiGetPixelData(char *flname,int xsize,int ysize,unsigned long bkcolor) {
//For Creating a high resolution Image data From gphfile and 
//Convert back to original so that antialiased image is created
// Args are review file, xsize,ysize and background color (argb)
// Caller must free data
  int i,ln;
  char ch;
  float fact;
  FILE *fp;
  unsigned long *data=NULL;
  void *img,*rzimg;
  Image *image;
  GMIMG *png=NULL;
  Lock;
  EVGAX=xsize*MFAC;
  EVGAY = ysize*MFAC;
  MAXB= EVGAX;
  fact=1;
  evgax=EVGAX,evgay=EVGAY;
  MAXB= evgax;
  BkColor=bkcolor;
  fp = fopen(flname,"r");
  if(fp != NULL) {
    fclose(fp);
    SetIcode();
    initialise();
    png = (GMIMG *)uiCreateCleangmImage(EVGAX,EVGAY,0,0,0,255);
    pixels =GetImagePixels((Image *)(png->image),0,0,((Image *)(png->image))->columns,((Image *)(png->image))->rows);
    review_gph_file(flname);
    SyncImagePixels((Image *)(png->image));
    img = png;
    if(img==NULL) {
      free(buffer);
      Unlock;
      return NULL;
    }
    if(MFAC==2)  rzimg = uiHalfSizegmImage(img);
    else rzimg = kgChangeSizeImage(img,xsize,ysize);
    if(rzimg == NULL) {
      printf("Failed to Change size\n");
      free(buffer);
      Unlock;
      return NULL;
    }
    buffer = realloc(buffer,(xsize*ysize)*sizeof(unsigned long));
    uiCopyPixelsgmImage(rzimg,buffer);
    uiFreeImage(img);
    uiFreeImage(rzimg);
  }
  else  {
     printf("Failed open %s\n",flname);
     Unlock;
     return NULL;
  }
  data = buffer;
  buffer=NULL;
  Unlock;
  return data;
 }
unsigned long *uiGetAntialiasedData(char *flname,int xsize,int ysize) {
//For Creating a high resolution Image data From gphfile and 
//Convert back to original so that antialiased image is created
// Args are review file, xsize,ysize 
  int i,ln;
  char ch;
  float fact;
  FILE *fp;
  void *img,*rzimg;
  unsigned long *data=NULL;
  Image *image;
  GMIMG *png=NULL;
  Lock;
  EVGAX=xsize*MFAC;
  EVGAY = ysize*MFAC;
  MAXB= EVGAX;
  fact=1;
  evgax=EVGAX,evgay=EVGAY;
  MAXB= evgax;
  BkColor=0xff000000;
  fp = fopen(flname,"r");
  if(fp != NULL) {
    fclose(fp);
    SetIcode();
    initialise();
    png = (GMIMG *)uiCreateCleangmImage(EVGAX,EVGAY,0,0,0,0);
    pixels =GetImagePixels((Image *)(png->image),0,0,((Image *)(png->image))->columns,((Image *)(png->image))->rows);
    review_gph_file(flname);
    SyncImagePixels((Image *)(png->image));
    img = png;
    if(img==NULL) {
      Unlock;
      return NULL;
    }
    rzimg = kgChangeSizeImage(img,xsize,ysize);
    if(rzimg == NULL) {
      printf("Failed to Change size\n");
      Unlock;
      return NULL;
    }
    buffer = realloc(buffer,(xsize*ysize)*sizeof(unsigned long));
    uiCopyPixelsgmImage(rzimg,buffer);
    uiFreeImage(img);
    uiFreeImage(rzimg);
  }
  else  {
     printf("Failed open %s\n",flname);
     Unlock;
     return NULL;
  } 
  data=buffer;
  buffer=NULL;
  Unlock;
  return data;
 }
void *uiGetAntialiasedImage(char *flname,int xsize,int ysize,void *lights,int Mag) {
//For Creating a high resolution Image data From gphfile and 
//Convert back to original so that antialiased image is created
// Args are review file, xsize,ysize 
  int i,ln;
  char ch;
  float fact;
  FILE *fp;
  void *img,*rzimg;
  unsigned long *data=NULL;
//  int Mag=2;
  Image *image;
  GMIMG *png=NULL;
  ls_list=lights;
  if(ls_list!= NULL) {
   No_of_lights = Dcount(ls_list);
  }
  Lock;
  if(Mag < 1) Mag=1;
  EVGAX=xsize*Mag;
  EVGAY = ysize*Mag;
  MAXB= EVGAX;
  fact=1;
  evgax=EVGAX,evgay=EVGAY;
  MAXB= evgax;
  BkColor=0xff000000;
  fp = fopen(flname,"r");
  if(fp != NULL) {
    fclose(fp);
    SetIcode();
    initialise();
    png = (GMIMG *)uiCreateCleangmImage(EVGAX,EVGAY,0,0,0,0);
    pixels =GetImagePixels((Image *)(png->image),0,0,((Image *)(png->image))->columns,((Image *)(png->image))->rows);
    review_gph_file(flname);
    img = png;
    if(img==NULL) {
      rzimg=NULL;
    }
    else {
      SyncImagePixels((Image *)(png->image));
      switch (Mag) {
        case 1:
          rzimg=img;
          break;
        case 2:
          rzimg = uiHalfSizegmImage(img);
          uiFreeImage(img);
          break;
        default:
          rzimg = kgChangeSizeImage(img,xsize,ysize);
          uiFreeImage(img);
          break;
      }
      if(rzimg == NULL) {
        printf("Failed to Change size\n");
      }
    }
  }
  else  {
     printf("Failed open %s\n",flname);
     rzimg=NULL;
  } 
  Unlock;
  ls_list=NULL;
  return rzimg;
 }
void *uiGphtogmImage(char *flname,int xsize,int ysize,unsigned long bkcolor) {
//For Creating a high resolution Image From gphfile and 
//Convert back to original so that antialiased image is created
// Args are review file, xsize,ysize
  int i,ln;
  char ch;
  float fact;
  FILE *fp;
  void *img=NULL,*rzimg;
  Image *image;
  GMIMG *png=NULL;
  Lock;
  EVGAX=xsize;
  EVGAY = ysize;
  MAXB= EVGAX;
  fact=1;
  BkColor = bkcolor;
  evgax=EVGAX,evgay=EVGAY;
  MAXB= evgax;
  fp = fopen(flname,"r");
  if(fp != NULL) {
    fclose(fp);
    SetIcode();
//    if((bkcolor&0xffffff) != 0 ) changebw();
    if((bkcolor&0xffffff) == 0xffffff ) changebw();
    initialise();
    png = (GMIMG *)uiCreateCleangmImage(EVGAX,EVGAY,0,0,0,255);
    image =(Image *)(png->image);
    pixels =GetImagePixels(image,0,0,image->columns,image->rows);
    review_gph_file(flname);
    SyncImagePixels(image);
    img = png;
//    free(buffer);
    buffer=NULL;
  }
  else  {
     printf("Failed open %s\n",flname);
     Unlock;
     return NULL;
  }
  Unlock;
  return img;
 }
void kgGphtoImagefile(char *Imagefile,char *gphfile,int xsize,int ysize,unsigned long bkcolor) {
/* 
   Converts a gph file to image file; Image file should have
   proper extention like .png ,.jpg
*/
 int Mag=4;
 void *img=NULL,*rzimg=NULL;
 img=uiGphtogmImage(gphfile, xsize*Mag,ysize*Mag, bkcolor);
 rzimg = kgChangeSizeImage(img,xsize,ysize);
 uiWritegmImage(rzimg,Imagefile);
 uiFreeImage(img);
 uiFreeImage(rzimg);
 return;
}
void *kgGphtoImage(char *gphfile,int xsize,int ysize,unsigned long bkcolor) {
/* 
   Converts a gph file to image 
   Receiving end should use uiFreeImage(img);
*/
 int Mag=4;
 void *img=NULL,*rzimg=NULL;
 img=uiGphtogmImage(gphfile, xsize*Mag,ysize*Mag, bkcolor);
 if(img != NULL) {
   rzimg = kgChangeSizeImage(img,xsize,ysize);
   uiFreeImage(img);
 }
 else rzimg=NULL;
 return rzimg;
}
void *kgGphtoImage_half(char *gphfile,int xsize,int ysize,unsigned long bkcolor) {
/* 
   Converts a gph file to image 
   Receiving end should use uiFreeImage(img);
*/
 int Mag=2;
 void *img=NULL,*rzimg=NULL;
 img=uiGphtogmImage(gphfile, xsize*Mag,ysize*Mag, bkcolor);
 if(img != NULL) {
   rzimg = uiHalfSizegmImage(img);
   uiFreeImage(img);
 }
 else rzimg=NULL;
 return rzimg;
}
void *kgGphtoAntialiasedImage(char *gphfile,int xsize,int ysize,unsigned long bkcolor ,int Mag) {
/* 
   Converts a gph file to image 
   Receiving end should use uiFreeImage(img);
*/
// int Mag=4;
 void *img=NULL,*rzimg=NULL;
 if(Mag==2) return kgGphtoImage_half(gphfile,xsize,ysize,bkcolor);
 img=uiGphtogmImage(gphfile, xsize*Mag,ysize*Mag, bkcolor);
 if(Mag==1) return img;
 if(img != NULL) {
   rzimg = kgChangeSizeImage(img,xsize,ysize);
   uiFreeImage(img);
 }
 else rzimg=NULL;
 return rzimg;
}
//void kgFreeImgBuffer(void) {uiFreeImgBuffer();}
#if 0
void  main(int argc,char * argv[]) {
  int xsize,ysize;
  if(argc < 4) {
    printf("Usage: %s <Gphfile> <xsize> <ysize>\n",argv[0]);
    return ;
  }
  sscanf(argv[2],"%d",&xsize);
  sscanf(argv[3],"%d",&ysize);
  uiGetAntialiasedData(argv[1],xsize,ysize);
  return ;
 }
#endif
