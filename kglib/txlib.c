/*
  Version 2.0
  Dated 28/04/97
  With improved GUI
*/
#define LINUX
#define ZBUFFER
#include "gl.h"
#include "font0.h"
#include "font1.h"
#include "font2.h"
#include "font3.h"
#include "fontps.h"
#include<stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef RS6000
#include<aio.h>
#endif
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/cursorfont.h>
#include <X11/keysym.h>
#define FONT "-adobe-courier-bold-r-*-*-13-*-*-*-*-*-*-*"
#define far 
#define SSF 0.6

#define Shift_key  ((Scan_code[code]==XK_Shift_L)||(Scan_code[code]==XK_Shift_R))
#define Capslock_key  (Scan_code[code]==XK_Caps_Lock)

#define Tab              XK_Tab
#define Up_Arrow         XK_Up
#define Up_Kp_Arrow      XK_KP_Up
#define Down_Arrow       XK_Down
#define Down_Kp_Arrow    XK_KP_Down
#define Left_Arrow       XK_Left
#define Left_Kp_Arrow    XK_KP_Left
#define Right_Arrow      XK_Right
#define Right_Kp_Arrow   XK_KP_Right
#define Backspace        XK_BackSpace
#define Linefeed         XK_Linefeed
#define Return           XK_Return

static int VGA16=0;
int NCLRS=16;
#define TX(x,y) (cx +(x)*cost+(y)*sint)
#define TY(x,y) (cy -(x)*sint+(y)*cost)
typedef struct line {float x1;float x2;int p;float ymax;float ymin;
                     struct line *Pr;struct line *Nx;} L_N;
typedef struct point {float x;float y;float xl;
                     struct point *Pr;struct point *Nx;} B_K;
#define DR_BT(x1,y1,x2,y2,t,s) draw_button((short)(x1),(short)(y2),(short)(x2),(short)(y1+2),t,(short)(s))
static int Pointer=1;
static L_N *O_L=NULL;
static B_K *O_P=NULL,*D_P;
static int DISCARD=0;
static int GR_ON=0,GUI_ON=0;
static int MSCROLL=0;
static int SCROLL=0;
static short Cur_On;
static float *m_F;
static short *icxvF,*icyvF;
static unsigned int ln_ptn[10]={0xffffffff,0x88888888, 0xcccccccc, 0xf0f0f0f0,
            0xff18ff18,0xffff0f0f, 0xe7e7e7e7, 0xfafafafa,
            0xffff0000,0xffffff00};
static Pixmap linebuf;
static short st_ptr[10]={0,0,0,0,0,0,0,0,0,0};
typedef struct c_limit{ int cx1;int cy1;int cx2;int cy2;int * pt;} CLIP;
static CLIP *c_loc;
typedef struct Scrn {int x1;int y1;int x2;int y2;Pixmap buf;
            struct Scrn *nx;struct Scrn *pv;} SC_BUF;
SC_BUF *scpt=NULL;
static XWMHints xwmh={
        (InputHint|StateHint),
         True,
         NormalState,
         0,
         0,
         0,0,
         0,0,
};
static  Display *Dsp;
static   Window Win;
static   GC Gc;
static   Cursor Cur,Curo;
static   Colormap Cmap;
static   Visual *Vis;
char getch(void);
#define CopyPoint(x,y,i) XCopyArea(Dsp,Win,linebuf,Gc,(short)x,(short)(y) ,1,1,i,0)
#define GetPoint(x,y,i) XCopyArea(Dsp,linebuf,Win,Gc,i,0,1,1,(short)x,(short)(y))
static unsigned char KEY=0;
static int msg_btn;
static int PON_X=0,PON_Y=0;
#define set_clip_limits(x1,y1,x2,y2,x3,y3) c_v_x1=x1,c_v_y1=y1,c_v_x2=x2,c_v_y2=y2
#define bkup_clip_limits c_v_x1_o=c_v_x1,c_v_y1_o=c_v_y1,c_v_x2_o=c_v_x2,c_v_y2_o=c_v_y2
#define set_sup_clip_limits(x1,y1,x2,y2) s_v_x1=x1,s_v_y1=y1,s_v_x2=x2,s_v_y2=y2
#define rest_clip_limits c_v_x1=c_v_x1_o,c_v_y1=c_v_y1_o,c_v_x2=c_v_x2_o,c_v_y2=c_v_y2_o
#define set_full_scrn  c_v_x1=0,c_v_y1=0,c_v_x2=EVGAX,c_v_y2=EVGAY
/*#define Pointer BringPointer( (lcur_x+rcur_x)/2,(lcur_y+rcur_y)/2)*/
#define POSI 0x40
#define NEG  0x60
static float txt_hty,txt_wtx,txt_spx;
static int pr_txt;
static unsigned char  zm[8]={0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe,0x7f};
static unsigned char  hm[8]={0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x80};
static unsigned char  bt[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
static int vu_x1,vu_y1,vu_x2,vu_y2;
static int *ftbl[2];
static int *ftblc[2];
static int op=1;
char PAK_B = 0x18;
static short opc,lp,lser=0;
static int ierr,ierrc,bk;
static float pmax,pmin,dfac;
static int ibdr;
static float xshft=0,yshft=0,xmul=1,ymul=1;
static unsigned char esc=0x1b,G='g',N='n',C='c',P='p',Z='z',L='l',V='v';
static unsigned char S='s',T='t',R='r',M='m',D='d',Y='y',U='u';
static unsigned char O='o',B='b',X='x',F='f',I='i',A='a',E='e',W='w';
static unsigned char H='h',K='k',M3 ='M',D3='D',P3='P',G3='G',B3='B';
static unsigned char BELL=0x07;
static unsigned char Ch1=0;
#define pclr(p)  (float)(((p-pmin)*dfac+1.))
static int GRAPHIC=0;

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PIX 8
#define MAXB 80
#define MAXC 640
#define MAXR 350
#define setmode 0
#define video 0x10
#define mask 0xCF
#define rad 0.0174532
#define LN_STYL 0
#define M_STYL 0
#define true 1
#define scr_x(x) (int)((x-w_x1)*u_x+D_x+0.5)
#define scr_y(y) (int)((y-w_y1)*u_y+D_y+0.5)
#define usr_x(x) (float)((x-D_x)/u_x+w_x1)
#define usr_y(y) (float)((y-D_y)/u_y+w_y1)
#define cnv_x(x) (x*fx-sx)
#define cnv_y(y) (y*fy-sy)

static unsigned char MASK[8]={ 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
static unsigned char MASKB[8]={0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };
static unsigned char MASKE[8]={0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF };
static unsigned char RMASK[4]={ 0x00, 0x01, 0x10, 0x11 };
static unsigned char MMASK[4]={ 0x01, 0x02, 0x04, 0x08 };
static unsigned char RED[4]={0x00, 0x04, 0x20, 0x24 };
static unsigned char BLUE[4]={0x00, 0x01, 0x08, 0x09 };
static unsigned char GREEN[4]={0x00, 0x02, 0x10, 0x12 };
static unsigned char c_color=0x01,p_color,p_bound,c_bound;
static int cur_x,cur_y,D_x=0,D_y=0,lcur_x,lcur_y,rcur_x,rcur_y;
static int old_xp=0,old_yp=0;
static float V_x=600.,V_y=450.;
static short txt_bold=1,txt_bold_o=1,ln_width=1,ln_style,m_style,po,jmp;
static float PS_Slant=0.,Slant=0.,Slant_o=0.,Slnt[2]={0.0,0.25};
static fil_color=0,t_font=0;
static float jmpy,ylcur,yrcur;
static float w_x1=0.,w_y1=0.,w_x2=600.,w_y2=450.,v_r,u_x,u_y;
static float txt_ht,txt_wt,txt_sp,r_d,theta,txt_w42,txt_h42;
static float txt_htx=1.,txt_wty=1.,txt_spy=1.;
static  int v_xt1,v_yt1,v_x1=0.,v_x2=600.,v_y1=0.,v_y2=450.;
static  int c_v_xt1,c_v_yt1,c_v_x1,c_v_x2,c_v_y1,c_v_y2;
static  int s_v_y1,s_v_y2,s_v_x1,s_v_x2;
static  int c_v_x1_o,c_v_x2_o,c_v_y1_o,c_v_y2_o;
typedef struct twin
            {int xl;int yl;int xu;int yu;int nchr;
             int xp;int yp;Pixmap w;struct twin *Nx;
             struct twin *Pr;
} TWIN;
static TWIN *Twin=NULL;
static int xmin,ymin,xmax,ymax,xrbr,yrbr;
static short hbuf,rbuf,nmenu;
static short t_color,t_bkgr,t_pattern,t_fill,t_bodr,greek=0;
static short *icxv,*icyv;
static short *icpos;
static float *m_f;
static float fact;
static float cost=1.0,sint=0.0;
static int gcur_x=1,gcur_y=1,gcur_r=20;
static float gcur_t=0,gcur_ds=0;
static int po_cur_x,po_cur_y;
static float gw_x1,gw_y1,gw_x2,gw_y2;
#ifdef RS6000
static short max_clr=15,min_clr=15;
#else
#ifdef LINUX
static short max_clr=15,min_clr=15;
#else
static short max_clr=15,min_clr=0;
#endif
#endif
static short EVGAX=600;
static short EVGAY=450;
static short msg_x,msg_y;
static unsigned int Dpth=4;
static float xpos,ypos,txs=0.,txc=1.;
static float cx,cy,xp,yp;
void get_scan_code(void);
void char_hilit_off(void);
void char_hilit_on(void);
void draw_button(short x1,short y1,short x2,short y2,char *t,short s);
void  text_menu_n(int x,int y,int n,int *maxq,char **ch1,
                  int *maxf,char **ch2);
int menu(int ixx,int iyy,int imenu,int ichr,char *cmenu,int item);
void Scrn_back(short ix,short iy,short n);
void Scrn_recover(void);
void setnormal(void);
void  msg_menu(short ix,short iy,short max1,char *ch1);
void t_initialise();
void clear_clip(void);
void t_pop_clip(void);
void t_window(float x1,float y1,float x2,float y2);
void set_window(float x1,float y1,float x2,float y2);
void t_fillchr_o(float *x,float *y,int code,int n);
void t_move(float x,float y);
void t_txt_clr(int color);
void t_txt_wr(int n,char *txt);
void t_txtbodr(int bodr);
void t_txtfill(int fill);
void t_txtbkgr(int fill);
void t_txtpattern(int fill);
void t_ln_clr(int color);
void t_txt_font(int font);
char gincur(float *xx,float *yy);
char rect_gincur(float *xx,float *yy,float *xbgn,float *ybgn);
char cross_gincur(float *xx,float *yy);
void set_graphics(short n);
void make_area(short w,short h);
void t_graph_new(short w,short h);
void set_mode1();
void set_mode2();
void _move(int x,int y);
void _draw(int x,int y);
void draw_line(int xa,int ya,int xb,int yb);
void hori_line(int xa,int xb,int y);
void godr_line(int xa,int xb,int y,float pa,float pb);
void set_color(char color);
void put_pixel(int col, int row);
void put_byte(short col, short row);
void user_conv();
void _marker (int x,int y);
void _box_fill(int x1,int y1,int x2,int y2,unsigned char color);
void _draw_line(int x1,int y1,int x2,int y2);
void clear_screen();
void _poly_fill(short n,int *px,int *py,short flag,unsigned char b_color);
void _mpoly_fill(short n,int *px,int *py,int *px1,int *py1,unsigned char b_color);
void _godard_fill(short n,int *px,int *py,short flag,float *pv);
void scr_back(int x1,int y1,int x2, int y2);
void scr_recover(void);
void scr_copy(int x0,int y0);
void release_scrn_buffer(void);
void pixel_copy(int x1,int y1,int x2,int y2, int x3,int y3);
void _circle(int x,int y,int r);
void _circle_fill(int x,int y1,int r,unsigned char color);
void cursor_left();
void cursor_right();
void cursor_up();
void cursor_down();
void draw_cursor();
void draw_po_cursor();
void draw_box_cursor();
void draw_lcur();
void draw_rcur();
char lzoom();
void zoom_up();
void pointer_zoom();
void zoom_down();
char pan();
void pan_up();
void pan_down();
void pan_right();
void pan_left();
char stretch();
void vstretch();
void hstretch();
void vshrink();
void hshrink();
short clip_line(int *xa,int *ya,int *xb,int *yb);
unsigned char code(int x,int y);
void gbell(void);
void display_on(void);
void display_off(void);
void text_control(short n,short l,short ny,short txtpos,char *txtdat);
void mline( short ixb, short iyb, short ixe,short iye);
void menuch(char c,int ix1,int iy1,int char_clr);
void chng_clr(int l,int ir,int ib,int ig);
void _rect_fill(short x1,short y1,short x2,short y2,unsigned char color);
short p_control();
void _draw_bound(short x1,short y1,short x2,short y2);
void menu_clrs_menu(void);
void ghelp_menu (void);
void set_ghelp_menu( short lines,char * str);
void set_dfclrs(void);
void menu_clrs_menu(void);
void backup_view(void);
void restore_view(void);
char *h_short(float *x,float *y);
void get_view(unsigned char *bf);
void read_view(unsigned char *bf);
void horeview(void);
void Get_Attn(void);
int  strlngth(char *title,float *xdsp);
static short GHMAX=0,SCRN,S_OPEN=0;
static char *GHBUF;
static char df_gbuf[20]={"No help available"};
unsigned char far *Rbuff;
short MODE=18;
unsigned char far * RBUFF;
#define icde(i,j) icode[j-1][i-1]

static unsigned char char_clr=0x0f,fill_clr=0x08,bodr_clr=0x07,cur_clr=0x09,
            high_clr=0x04,char_hclr=11,char_hclr_bk=11;

/*
static short  icode[256][3]= {0,0,0, 0,100,0, 0,0,50, 10,70,30, 50,0,0,
                    50,50,0, 50,0,75, 65,65,65, 45,45,45, 20,100,40, 0,0,100,
                    0,100,100, 100,60,60, 100,100,75, 100,0,100, 100,100,100};
static short  icode_df[16][3]= {0,0,0, 0,100,0, 0,0,50, 10,70,30, 50,0,0,
                    50,50,0, 50,0,75, 65,65,65, 45,45,45, 20,100,40, 0,0,100,
                    0,100,100, 100,60,60, 100,100,75, 100,0,100, 100,100,100};
static short  icode[256][3]= {0,0,0, 0,100,0, 0,0,50, 10,70,30, 50,0,0,
                    50,50,0, 50,0,75, 81,44,35, 57,32,25, 20,100,40, 0,0,100,
                    0,100,100, 100,60,60, 100,100,75, 100,0,100, 100,100,100};
static short  icode_df[16][3]= {0,0,0, 0,100,0, 0,0,50, 10,70,30, 50,0,0,
                    50,50,0, 50,0,75, 81,44,35, 57,32,25, 20,100,40, 0,0,100,
                    0,100,100, 100,60,60, 100,100,75, 100,0,100, 100,100,100};
static short  icode[256][3]= {
  0,0,0 ,0,255,0 ,64,208,224 ,46,87,139 ,107,35,142 ,205,92,92
,255,147,20 ,255,255,0 ,205,205,0 ,28,238,134 ,139,0,0 ,144,144,238
 ,244,96,164 ,255,0,215 ,0,255,255 ,255,255,255};
static short  icode_df[16][3]= {
  0,0,0 ,0,255,0 ,64,208,224 ,46,87,139 ,107,35,142 ,205,92,92
,255,147,20 ,255,255,0 ,205,205,0 ,28,238,134 ,139,0,0 ,144,144,238
 ,244,96,164 ,255,0,215 ,0,255,255 ,255,255,255};
static short  icode[256][3]= {
       0,0,0, 0,255,0, 0,0,127, 25,180,190, 127,0,0,
       127,127,0, 127,0,190, 170,170,170, 110,110,110, 50,255,100, 0,0,255,
       0,255,255, 255,160,160, 255,255,190, 255,0,255, 255,255,255};
static short  icode_df[16][3]= {
       0,0,0, 0,255,0, 0,0,127, 25,180,190, 127,0,0,
       127,127,0, 127,0,190, 170,170,170, 110,110,110, 50,255,100, 0,0,255,
       0,255,255, 255,160,160, 255,255,190, 255,0,255, 255,255,255};
*/
static short  icode[256][3]= {
       0,0,0, 0,255,0, 0,0,127, 25,180,190, 127,0,0,
       127,127,0, 127,0,190, 170,170,170, 110,110,110, 50,255,100, 0,0,255,
       0,255,255, 255,160,160, 255,255,190, 255,0,255, 255,255,255};
static short  icode_df[16][3]= {
       0,0,0, 0,255,0, 0,0,127, 25,180,190, 127,0,0,
       127,127,0, 127,0,190, 170,170,170, 110,110,110, 50,255,100, 0,0,255,
       0,255,255, 255,160,160, 255,255,190, 255,0,255, 255,255,255};
static int clrtbl[256]={0,
                        1,2,3,4,5,6,7,
                          8,  9, 10, 11, 12, 13,14, 15, 16, 17, 18, 19,
                         20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                         32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                         44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
                         56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
                         68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
                         80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
                             92, 93, 94, 95, 96, 97, 98, 99,100,101,102,
                        103,104,105,106,107,108,109,110,111,112,113,114,
                        115,116,117,118,119,120,121,122,123,124,125,126,
                        127,128,129,130,131,132,133,134,135,136,137,138,
                        139,140,141,142,143,144,145,146,147,148,149,150,
                        151,152,153,154,155,156,157,158,159,160,161,162,
                        163,164,165,166,167,168,169,170,171,172,173,174,
                        175,176,177,178,179,180,181,182,183,184,185,186,
                        187,188,189,190,191,192,193,194,195,196,197,198,
                        199,200,201,202,203,204,205,206,207,208,209,210,
                        211,212,213,214,215,216,217,218,219,220,221,222,
                        223,224,225,226,227,228,229,230,231,232,233,234,
                        235,236,237,238,239,240,
                        241,242,243,244,245,246,
                        247,248,249,250,251,252,253,254,255
                       };
                       
                  
static char ch,bf[200]={" "};
static unsigned char cmds[1000];
static short step;
char PIXL=0xFF;
typedef struct r_c {int x1;int y1;int x2;int y2;} RECT;
static struct menu_str {
      int ixx;
      int iyy;
      int imenu;
      int ichr;
      char *cmenu;
      int item;
      int thirty;
      int ixl;
      int iyl;
      int ixu;
      int iyu;
      unsigned char color1;
      unsigned char color2;
 } MS;
int B_OFF_SET=0;
#define CURKEY 00
#define BIT_MASK(bits) outp(0x3CF,bits)
/*
#define EVGA_CN(y)  (short)(((y)*EVGAY)/479)
#define EVGA_CNX(x)  (short)(((x)*EVGAX)/639)
*/
#define EVGA_CN(y)  y
#define EVGA_CNX(x) x
unsigned char OFF=0x0F;
char C_CLR=0x0F;
char fname[15];
union kbinp { short kbint; char kbc[2];} kb;
union vd { short vdint; char vdc[2];} vd,bl;
unsigned char far *buffer;

unsigned char arr[] ={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
char  temp;
#define EIGHT 8
void goto_xy(void);

short kb_hit(void);
unsigned char get_ch(void);

static unsigned char pix=0xFF;
static unsigned short IPO1=0x3CE;
static unsigned short IPO2=0x3CF;
static unsigned short IPO3=0x3C4;
static unsigned short IPO4=0x3C5;
#define set_clr(x) XSetForeground(Dsp,Gc,(unsigned int)(clrtbl[x]))
#define XDraw(x1,y1,x2,y2) XDrawLine(Dsp,Win,Gc,x1,(EVGAY-y1),x2,(EVGAY-y2))
#define XDRAW(x1,y1,x2,y2) XDrawLine(Dsp,Win,Gc,(short)(x1),(short)(y1),(short)(x2),(short)(y2))
#define XSDraw(x1,y1,x2,y2) XDrawLine(Dsp,Win,Gc,x1,(y1),x2,(y2))
#define XCPix(x1,y1,x2,y2) XCreatePixmap(Dsp,Win,(short)(x2-(x1)+1),(short)(y2-(y1)+1),Dpth);
#include <termio.h>
static tcflag_t oldc_c,oldc_l,oldc_i,oldc_o;
static cc_t oldc_c4, oldc_c5;   
static struct termios tty;
static int Shift=0,Caplock=0;
static int Scan_code[256]={0,0,0,0,0,0,0,0,27,'1','2',
                 '3','4','5','6','7','8','9','0','-','=',
                  '\b',0,'q','w','e','r','t','y','u','i','o',
                  'p',0,0,'\r',0,'a','s','d','f','g','h','j',
                  'k','l',';','\'','`',0,'\\','z','x','c','v','b','n',
                  'm',',','.','/',0,0,0,' ',0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,
                  0,0,103,104,105,106};
static int Scan_sh_code[256]={0,0,0,0,0,0,0,0,27,'!',' ',
                 '#','$','%','^','&','*','(',')','_','+',
                  '\b',0,'Q','W','E','R','T','Y','U','I','O',
                  'P',0,0,'\r',0,'A','S','D','F','G','H','J',
                  'K','L',':','"','~',0,'|','Z','X','C','V','B','N',
                  'M','<','>','?',0,0,0,' ',0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,
                  0,0,103,104,105,106};
static S_STR *sld_ptr=NULL;
static BUTS  *but_ptr=NULL;
void DR_BUTNS(BUTS *bt);
int GetButton(BUTS *bt);
int pick_button(BUTS *bt);
void High_Button(BUTS *bt,int df);
int CheckButton(BUTS *bt);
void clear_but_str(void);
BUTS *get_but_str(int n);
int scroll_menu(int xp,int yp,char *x[],int por,int *pos,int df);
#define Menuch(c) {\
      _rect_fill(menu_x,menu_y-4,menu_x+nine,menu_y+9,c);\
      menuch(*ptr,menu_x,menu_y,char_clr);\
}
/* Dummy routines for DOS Compatibility */
void rmv_pointer()  {}
void dsp_pointer() {}
void warp_pointer(x,y) {}
void draw_pointer(x,y) {}
/* End of dummies */

int get_cur_clr(void) {return c_color;}
int GetPointerStatus(void) { return Pointer;}
void SetPointerStatus(int pointer) { Pointer=pointer;}
int get_pointer(int *x,int *y) {
  return GetPointer(x,y);
}

void scr_scroll_back(int x1,int y1,int x2,int y2,int width) {
  int i,j,addr;
  XCopyArea(Dsp,Win,Win,Gc,(short)x1,(short)(y1)
    ,(short)(x2-x1+1),(short)(y2-y1+1),(short)x1,(short)(y1-width));
}

void g_scroll_up(short x1,short y1,short x2,short y2,short jmp)
  {
    int yy1,yy2;
    yy1 = EVGAY-y1;
    yy2 = EVGAY-y2;
/*
    scr_back(x1,yy1+jmp,x2,yy2);
    scr_copy(-22,0);
    release_scrn_buffer();
*/
    scr_scroll_back(x1,yy1+jmp,x2,yy2,22);
  }
void Set_resolution (int x,int y) {
  if( !GR_ON ){
    EVGAX = x;
    EVGAY = y;
  }
}
void Backup_Scrn(int x1,int y1,int x2,int y2)
 {
  if(GR_ON) {
   Get_Attn();
   scr_back((int)x1,(int)y1,(int)x2,(int)y2);
  }
  /*else t_graph_new((x2-x1+1),(y2-y1+1));*/
  else {t_graph_new((x2-x1),(y2-y1));GR_ON=1;GUI_ON=1;}

 }
void Recover_Scrn()
 {
  if(GUI_ON) {setnormal();GUI_ON=0;}
  else {
   if(GR_ON) { scr_recover(); }
   else setnormal();
  }
 }
short key_press(int *c)
  {
   int ch;
   int keypress,i,code;
   int *scan;
   XEvent event;

   if( XPending(Dsp))
      {XNextEvent(Dsp,&event);
      if( (event.type==KeyPress)||(event.type==KeyRelease))
        { code=event.xkey.keycode;

          if((event.type==KeyPress)&&Shift_key)
                  { Shift=1;return(-1);}
          if((event.type==KeyRelease)&&Shift_key)
                  { Shift=0;return(-1);}
          if((event.type==KeyPress)&&Capslock_key)
                  { Caplock=1;return(-1);}
          if((event.type==KeyRelease)&&Capslock_key)
                  { Caplock=0;return(-1);}

          if(event.type==KeyPress) keypress=1;
          else  return(-1);
         }
       else return(-1);
       }
   else return(-1);
   if (Shift==0) scan=Scan_code;
   else scan = Scan_sh_code;
   ch = scan[code];
/*
   if((code>86)&&(code<110)) {*c=code+40;return(1);};
*/
    if((ch!=0)&&(Caplock!=0))
      {
            if  ((Shift!=0)&&(ch>='A') &&(ch<='Z')) ch+=('a'-'A');
            if  ((Shift==0)&&(ch>='a') &&(ch<='z')) ch-=('a'-'A');
      }
    *c=ch;
    return(keypress);
   }

 short get_kb(int t)
 {
   int ch,ch1;
   int i;
   union kbinp { short kbint; char kbc[2];} kb;
   kb.kbint =0;
   XSync(Dsp,False);
   if( key_press(&ch)<0) return(-1);
   if(ch==0) return(-1);
   if(ch>126)
     { kb.kbc[0]=0;
       switch (ch)
           {
             case Up_Arrow       :ch='H';break;
             case Up_Kp_Arrow    :ch='H';break;
             case Down_Arrow     :ch='P';break;
             case Down_Kp_Arrow  :ch='P';break;
             case Right_Arrow    :ch='M';break;
             case Right_Kp_Arrow :ch='M';break;
             case Left_Arrow     :ch='K';break;
             case Left_Kp_Arrow  :ch='K';break;
             case Tab            :
             case Backspace      :
             case Linefeed       :
             case Return         :
                   kb.kbc[0]=ch&0x00FF;
                   break;
             default:
                   gbell();
                   break;
           }
        kb.kbc[1]=ch&0x00FF;
      }
   else
      {
        kb.kbc[0]=ch&0x00FF;
       }
   return(kb.kbint);
 }
Window GetParent(Window win)
 {
   static Window Root,Par;
   static Window *Child[4];
   static unsigned int nchild;
   nchild=4;
   if( XQueryTree(Dsp,win,&Root,&Par,Child,&nchild))
       {
          return(Par);
       }
   else
       {
          printf( "XQueryTree Faild\n");
          exit(0);
       }
  }

int GetPointer(int *x,int *y)
 {
  Window Root,Root1,Child,Curwin;
  int x0,y0;
  unsigned int wd0,ht0,bwd0,depth0;
  int x1,y1,x2,y2;
  unsigned int pb,pbb,key,i;
  unsigned int wd1,ht1,bwd1,depth1;
  Curwin=GetParent(Win);
  if(XQueryPointer(Dsp,Curwin,&Root,&Child,&x1,&y1,x,y,&pb)==False)return(-100);
  if( Child != Win){ return(-1); };
  key=0;
  pbb=pb;if((pbb&=0x0100)!=0) key=1;
  pbb=pb;if((pbb&=0x0400)!=0) key=3;
  XGetGeometry(Dsp,Win,&Root,&x0,&y0,&wd0,&ht0,&bwd0,&depth0);
/*
  while( (pb&0x0500)>0)
   if(XQueryPointer(Dsp,Curwin,&Root,&Child,&x1,&y1,x,y,&pb)==False)return(-100);
  if( Child != Win){ return(-1); };
*/
  *x -= (x0);*y-=(y0);
  if( (*x>=0)&&(*y>=0)&&(*y<ht0)&&(*x<wd0)) {return(key);}
  else{ return(-1);}
 }
 void gsync(void)
  {
   int x,y;
   if(GR_ON) GetPointer(&x,&y);
  }
 void WaitRelease3(void)
    {
      int x,y;
      Window Root,Root1,Child,Curwin;
      int x0,y0;
      unsigned int wd0,ht0,bwd0,depth0;
      int x1,y1,x2,y2;
      unsigned int pb,pbb,key,i;
      unsigned int wd1,ht1,bwd1,depth1;
      Curwin=GetParent(Win);
      XQueryPointer(Dsp,Curwin,&Root,&Child,&x1,&y1,&x,&y,&pb);
      key=0;
      pbb=pb;if((pbb&=0x0100)!=0) key=1;
      pbb=pb;if((pbb&=0x0400)!=0) key=3;
      XGetGeometry(Dsp,Win,&Root,&x0,&y0,&wd0,&ht0,&bwd0,&depth0);
      while( (pb&0x0100)>0)
       XQueryPointer(Dsp,Curwin,&Root,&Child,&x1,&y1,&x,&y,&pb);
/*
      if((pbb&=0x0400)!=0) KEY=3;
      else KEY=0;
*/
      KEY=0;
    }
 void WaitRelease1(void)
    {
      int x,y;
      Window Root,Root1,Child,Curwin;
      int x0,y0;
      unsigned int wd0,ht0,bwd0,depth0;
      int x1,y1,x2,y2;
      unsigned int pb,pbb,key,i;
      unsigned int wd1,ht1,bwd1,depth1;
      Curwin=GetParent(Win);
      XQueryPointer(Dsp,Curwin,&Root,&Child,&x1,&y1,&x,&y,&pb);
      key=0;
      pbb=pb;if((pbb&=0x0100)!=0) key=1;
      pbb=pb;if((pbb&=0x0400)!=0) key=3;
      XGetGeometry(Dsp,Win,&Root,&x0,&y0,&wd0,&ht0,&bwd0,&depth0);
      while( (pb&0x0400)>0)
       XQueryPointer(Dsp,Curwin,&Root,&Child,&x1,&y1,&x,&y,&pb);
/*
      if((pbb&=0x0100)!=0) KEY=1;
      else KEY=0;
*/
      KEY=0;
    }
 void WaitRelease(void)
    {
      int x,y;
      Window Root,Root1,Child,Curwin;
      int x0,y0;
      unsigned int wd0,ht0,bwd0,depth0;
      int x1,y1,x2,y2;
      unsigned int pb,pbb,key,i;
      unsigned int wd1,ht1,bwd1,depth1;
      Curwin=GetParent(Win);
      XQueryPointer(Dsp,Curwin,&Root,&Child,&x1,&y1,&x,&y,&pb);
/*
      key=0;
      pbb=pb;if((pbb&=0x0100)!=0) key=1;
      pbb=pb;if((pbb&=0x0400)!=0) key=3;
      XGetGeometry(Dsp,Win,&Root,&x0,&y0,&wd0,&ht0,&bwd0,&depth0);
*/
      while( (pb&0x0500)>0)
       XQueryPointer(Dsp,Curwin,&Root,&Child,&x1,&y1,&x,&y,&pb);
      KEY=0;
    }
 void HoldPointer(int x ,int y)
    {
      int x0,y0,xx,yy;
      Window Root,Root1,Child,Curwin;
      unsigned int wd0,ht0,bwd0,depth0;
      int x1,y1,x2,y2;
      unsigned int pb,pbb,key,i;
      unsigned int wd1,ht1,bwd1,depth1;
      Curwin=GetParent(Win);
      XGetGeometry(Dsp,Win,&Root,&x0,&y0,&wd0,&ht0,&bwd0,&depth0);
      x +=x0; y +=y0;
      XWarpPointer(Dsp,Curwin,Curwin,0,0,EVGAX+1,EVGAY+1,x,y);
      XQueryPointer(Dsp,Curwin,&Root,&Child,&x1,&y1,&xx,&yy,&pb);
      key=0;
      pbb=pb;if((pbb&=0x0100)!=0) key=1;
      pbb=pb;if((pbb&=0x0400)!=0) key=3;
      XGetGeometry(Dsp,Win,&Root,&x0,&y0,&wd0,&ht0,&bwd0,&depth0);
      while( (pb&0x0500)>0) {
       XWarpPointer(Dsp,Curwin,Curwin,0,0,EVGAX+1,EVGAY+1,x,y);
       XQueryPointer(Dsp,Curwin,&Root,&Child,&x1,&y1,&xx,&yy,&pb);
      }
   
    }
 int GetIncr(int *dx,int *dy)
   {
     int key,x,y,xo,yo;
     key = GetPointer(&x,&y);
     if(key<0) return(key);
     xo=(lcur_x+rcur_x)/2;
     yo=(lcur_y+rcur_y)/2;
     /*XWarpPointer(Dsp,win,win,0,0,EVGAX+1,EVGAY+1,xo,yo);*/
     BringPointer(xo,yo);
     key = GetPointer(&x,&y);
     if(key!=0) return(key);
     *dx= x-xo,*dy=y-yo;
     if ((abs(*dx)<2)&&(abs(*dy)<2)) return(-1);
     else return(key);
   }
     
     

 int GetRelPos(Window win,int *x,int *y)
  {
    int x1,y1,x2,y2,key1,key2;
    if( (key1=GetPointer(&x1,&y1))<0) return(-1);
    if( (key2=GetPointer(&x2,&y2))<0) return(-1);
    while(key1!=key2)
       if( (key2=GetPointer(&x2,&y2))<0) return(-1);
    while( GetPointer(&x2,&y2)>0);

    *x=x2-x1;*y=y2-y1;
    
    return(key2);
  }
int BringPointer(int x,int y)
 {
   int xx,yy;
   if(GetPointer(&xx,&yy)<0) return(0);
   XWarpPointer(Dsp,Win,Win,0,0,EVGAX+1,EVGAY+1,x,y);
   return(1);
 }
int GetItem_o(int x1,int y1,int x2,int y2,int dy,int *item)
 {
   int xx,yy,key;
   y1 += (dy/2);
   y2 -= (dy/2);
   if((key=GetPointer(&xx,&yy))<=0)return(0);
   if(xx<x1) {if(key==1) return(1);else return(0);};
   if(yy<y1) {if(key==1) return(1);else return(0);};
   if(xx>x2) {if(key==1) return(1);else return(0);};
   if(yy>y2) {if(key==1) return(1);else return(0);};
   yy -=y1;
   *item= (float)yy/((float)dy)+1;
   return(key);
  }
void setparam( int t)
{
 int ierr;
/* op = open("/dev/tty",O_RDWR|O_SYNC);*/
 op=0;
 ierr=ioctl(op, TCGETS,  &tty);
 if(ierr== -1) printf("ERROR IN  IOCTL\n");
 oldc_c=tty.c_cflag;
 oldc_l=tty.c_lflag;
 oldc_i=tty.c_iflag;
 oldc_o=tty.c_oflag;
 oldc_c4=tty.c_cc[VMIN];
 oldc_c5=tty.c_cc[VTIME];

 tty.c_lflag=ISIG;
 tty.c_iflag=0;
 tty.c_oflag=0;
 tty.c_cc[VMIN]=t; 
 tty.c_cc[VTIME]=0; 
 
 ierr=ioctl(op, TCSETS, &tty);
 if(ierr==-1 )
   printf("ERROR IN IOCTL\n"); 
 return;
}

void restore_old_mode(void)
{ int ierr;

 tty.c_cflag=oldc_c;
 tty.c_lflag=oldc_l;
 tty.c_iflag=oldc_i;
 tty.c_oflag=oldc_o;
 tty.c_cc[VMIN]=oldc_c4;
 tty.c_cc[VTIME]=oldc_c5;
 ierr=ioctl(op, TCSETSW, &tty);
 if(ierr== -1)
   printf("ERROR IN  IOCTL\n");
}
void set_dfclrs(void)
 {
   short i,j;
   for(i=0;i<16;i++)
      for(j=0;j<3;j++) icode[i][j]=icode_df[i][j];
   for (i=0;i<16;i++) df_clr(i,(int)icode[i][0],(int)icode[i][1],(int)icode[i][2]);
 }
   void setwin(color)
   unsigned char color;
     {
        short ierr;
        t_ln_clr(color);
     }
   void g_dash(col, row)
   short col,row;
      {
      }
 int GetMIncr(int dx,int dy)
   {
    int incr;
    if(abs(dx)>abs(dy)) return(dx);
    else return(dy);
   }
 short get_key(int t)
 {
   unsigned char ch;
   int dx,dy;
   int i,key;
   union kbinp { short kbint; char kbc[2];} kb;
     if((kb.kbint=get_kb(0))>0) return(kb.kbint);
   return(0);
 }
 short get_key_pointer(int t)
 {
   unsigned char ch;
   int dx,dy;
   int i,key;
   union kbinp { short kbint; char kbc[2];} kb;
   for(;;)
   {
     if((kb.kbint=get_kb(0))>0) return(kb.kbint);
     if((key=GetPointer(&dx,&dy))>0){
       if(key==1) kb.kbc[0]='\r',kb.kbc[1]=0;
       if(key==3) kb.kbc[0]=0,kb.kbc[1]='p';
       return(kb.kbint); 
     }
   }
 }
 short get_text_position(RECT *r,int n,int *old_i)
 {
   unsigned char ch;
   int dx,dy,yy;
   int i,key;
   union kbinp { short kbint; char kbc[2];} kb;
   for(;;)
   {
     if((kb.kbint=get_kb(0))>0) return(kb.kbint);
     if((key=GetPointer(&dx,&dy))>=0){
       yy=EVGAY-dy;
       if(key==1){
        kb.kbc[0]='\r',kb.kbc[1]=0;
        return(kb.kbint); 
       }
       if(key==3){
        kb.kbc[1]=0,kb.kbc[0]='\n';
        return(kb.kbint); 
       }
       else{
         for(i=0;i<n;i++){
          if(((r[i].x1-dx)*(r[i].x2-dx)<=0)&&((r[i].y1-yy)*(r[i].y2-yy)<=0)){
           if(i!=*old_i){
            *old_i=i;
            kb.kbc[0]=0,kb.kbc[1]='p';
            return(kb.kbint); 
           }
          }
         }
       }
     }
   }
 }
 short get_kb_o(int t)
 {
   unsigned char ch;
   int i;
   union kbinp { short kbint; char kbc[2];} kb;
   XSync(Dsp,False);
   i=0;
   while(((ierr=read(0,&ch,1))!= 1)&&(i<100))i++;
  /*
   ierr=read(0,&ch,1);
  */
   if( ierr !=1) { kb.kbc[0]=27; return(kb.kbint);};
   if(ch!=27)kb.kbc[0]=ch;
   else
       { kb.kbc[0]='\0';
         while((ierr=read(0,&ch,1))!= 1);
         if(ch =='[')
            {
              while((ierr=read(0,&ch,1))!= 1);
              switch (ch)
                {
                  case 'A':ch='H';break;
                  case 'B':ch='P';break;
                  case 'C':ch='M';break;
                  case 'D':ch='K';break;
                  default:break;
                }
            };
         kb.kbc[1]=ch;
       }
   return(kb.kbint);
 }
 void chng_clr(int i,int ir,int ib,int ig)
 {
  XColor col;
    if( i>=NCLRS ) i = i%NCLRS +1;
    col.flags=DoRed|DoGreen|DoBlue;
    col.pixel=clrtbl[i];
    col.red=ir*65535/255;
    col.green=ig*65535/255;
    col.blue=ib*65535/255;
    if(XAllocColor(Dsp,Cmap,&col)==0 ){
     printf("Alloc error %d \n",col.pixel);
    };
    clrtbl[i]= col.pixel;
    icode[i][0] =   col.red*255./65535+0.5;
    icode[i][2] =   col.green*255./65535+0.5;
    icode[i][1] =   col.blue*255./65535+0.5;
}
void df_clr(int i,int ir,int ib,int ig)
{
  XColor col;
    if( i>=NCLRS ) i = i%NCLRS +1;
    col.flags=DoRed|DoGreen|DoBlue;
    col.pixel=clrtbl[i];
    col.red=ir*65535/255;
    col.green=ig*65535/255;
    col.blue=ib*65535/255;
    if(XAllocColor(Dsp,Cmap,&col)==0 ){
     printf("Alloc error %d \n",col.pixel);
    };
    clrtbl[i]= col.pixel;
    icode[i][0] =   col.red*255./65535+0.5;
    icode[i][2] =   col.green*255./65535+0.5;
    icode[i][1] =   col.blue*255./65535+0.5;
}
void get_color(int i,short *ir,short *ig,short *ib)
{
  XColor col;
  if( i>=NCLRS ) i = i%NCLRS +1;
  col.pixel=clrtbl[i];
  XQueryColor(Dsp,Cmap,&col);
  *ir = col.red*255./65535+0.5;
  *ig = col.green*255./65535+0.5;
  *ib = col.blue*255./65535+0.5;
  /* printf( "%d %d %d %d %d %d %d \n",
             clrtbl[i],col.red,col.green,col.blue,*ir,*ig,*ib); */
}

void get_match_color(void)
{
  XColor col;
  int ir,ig,ib,i,j,k,red=99999,blue=99999,green=99999;
  int ige,ire,ibe;
  int hit=0;
  int err=9999999;
    for(j=16;j<NCLRS;j++) {
    if(clrtbl[j]> 240) continue;
    col.pixel=clrtbl[j];
    XQueryColor(Dsp,Cmap,&col);
    ir =   col.red*255./65535+0.5;
    ig =   col.green*255./65535+0.5;
    ib =   col.blue*255./65535+0.5;
    hit=0; red=ir*1/10;green=ig*1/10;blue=ib*1/10;
    for(i=1;i<16;i++) {
     if(clrtbl[i]<241 ) continue;
     ire = abs ( icode[i][0]-ir);
     ige = abs ( icode[i][2]-ig);
     ibe = abs ( icode[i][1]-ib);
     if( (red>=ire) &&(green>=ige)&&(blue>=ibe) ) {
       red=ire;green=ige;blue=ibe;k=i;
       hit=1;
     }
    }
    /*printf( " %d  %d %d %d %d \n", j,hit,ir,ig,ib);*/
    if(hit) {i = clrtbl[k]; clrtbl[k]=clrtbl[j]; clrtbl[j]=i;}
  }
}

void t_DRAW_LINE(float x1,float y1,float x2,float y2)
{
  int xa,ya,xb,yb;
  xa=scr_x(x1);
  ya=scr_y(y1);
  xb=scr_x(x2);
  yb=scr_y(y2);
  XDraw((short)xa,(short)ya,(short)xb,(short)yb);
}



void t_initialise()
{
  short i;
  for(i=0;i<10;i++)st_ptr[i]=0;
  cur_x=320;cur_y=240;
  ln_style=LN_STYL;
  m_style=M_STYL;
  c_color = 0x0F;
  set_clr(c_color);
  c_bound =4;
  t_color = 14;
  t_pattern=15;
  t_fill = 1;
  t_bkgr = 0;
  t_bodr = 1;
  char_clr=0x0f,fill_clr=0x08,bodr_clr=0x07,cur_clr=0x07,high_clr=0x09;
  step=0;
  icpos = icposf0;icxv=icxvf0;icyv=icyvf0;m_f=m_f0;
  c_loc=NULL;
  ln_width=1;
  pr_txt=1;
  cost = 1.0;
  sint = 0.0;
  txt_ht=20;
  txt_wt=20;
  txt_htx=20;
  txt_wty=20;
  txt_hty=20;
  txt_wtx=20;
  txt_w42 = txt_wt/3000.0;
  txt_h42 = txt_ht/3000.0;
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
  for (i=0;i<NCLRS;i++) df_clr(i,(int)icode[i][0],(int)icode[i][1],(int)icode[i][2]);
  Twin=NULL;
}
void t_graph()
{
  set_graphics(2);
  t_initialise();
}
void t_graph_new(short width,short height)
{
  make_area((short) width,(short)height);
  t_initialise();
}




void user_conv()
{
  int x1,x2,y1,y2;
  x1=(int)((float)w_x1+(float)(lcur_x - v_x1)*(w_x2 - w_x1)/V_x);
  x2=(int)((float)w_x1+(float)(rcur_x - v_x1+1)*(w_x2 - w_x1)/V_x);
  y1=w_x2-(int)((float)w_y1+(float)(lcur_y - v_y2+1)*(w_y2 - w_y1)/V_y);
  y2=w_x2-(int)((float)w_y1+(float)(rcur_y - v_y2)*(w_y2 - w_y1)/V_y);
  t_window((float)x1,(float)y1,(float)x2,(float)y2);
}
void put_pixl(short col, short row)
{
  XDrawPoint(Dsp,Win,Gc,col,row);
  return;
}

void put_pixel(int col, int row)
{
  short pos,j;
  if ((col >c_v_x2) || (col <c_v_x1) || (row >c_v_y2) || (row <c_v_y1)) return;
  pos=col;
  j= EVGAY-row;
  XDrawPoint(Dsp,Win,Gc,pos,j);
  return;
}

void put_pix(int col,int row)
{
  XDrawPoint(Dsp,Win,Gc,(short)col,(short)row);
}

void put_dot(int col,int row,unsigned char clr) {
  XSetForeground(Dsp,Gc,(unsigned int)(clrtbl[clr]));
  XDrawPoint(Dsp,Win,Gc,(short)col,(short)row);
}
void set_visual(Visual *vis) {
 int visclass;
 visclass = vis->class;
 if( visclass == PseudoColor ) {/*printf("PseudoColor\n");*/VGA16=0;}
 else VGA16 =1;
 /*printf(" VGA16 = %d\n",VGA16);*/
}
void set_def_colors(void) {
 XColor col;
 int i;
 for(i=0;i<NCLRS;i++) {
    if( i>=NCLRS ) i = i%NCLRS +1;
    col.pixel=clrtbl[i];
    col.red=icode[i][0]*65535/255;
    col.green=icode[i][2]*65535/255;
    col.blue=icode[i][1]*65535/255;
    col.flags=DoRed|DoGreen|DoBlue;
    if(XAllocColor(Dsp,Cmap,&col)==0 ){
     printf("Alloc error %d \n",col.pixel);
    };
    clrtbl[i]= col.pixel;
    icode[i][0] =   col.red*255./65535+0.5;
    icode[i][2] =   col.green*255./65535+0.5;
    icode[i][1] =   col.blue*255./65535+0.5;
 }
 
}
void set_graphics(short n)

{
  static int ENTRY=1;
  static int argc=1;
  static char *argv[1]={"Testing"};
  int i;
  Window par;
  XGCValues gcv;
  XEvent event;
  XSizeHints xsh;
  XSetWindowAttributes xswa;
  XWindowAttributes xwa;
  XFontStruct *fontstruct;
  unsigned int fg,bg,bd;
  Pixmap Pix;
  XColor c15,c0;
  Dsp = XOpenDisplay(NULL);
  if(Dsp == NULL) {printf(" Error: in opening Display ..\n");exit(0);};
  get_scan_code();
  Vis = XDefaultVisual(Dsp,DefaultScreen(Dsp));
  set_visual(Vis);
  Dpth =  DisplayPlanes(Dsp,DefaultScreen(Dsp));
  NCLRS = DisplayCells(Dsp,DefaultScreen(Dsp));
  fontstruct= XLoadQueryFont(Dsp,FONT);
  if(fontstruct ==NULL) {
    printf("Error: Invalid Font\n");
    printf("       Trying fixed\n");
    fontstruct= XLoadQueryFont(Dsp,"fixed");
    if(fontstruct ==NULL) exit(1);
  }
  bd = WhitePixel(Dsp,DefaultScreen(Dsp));
  bg = BlackPixel(Dsp,DefaultScreen(Dsp));
  fg = WhitePixel(Dsp,DefaultScreen(Dsp));
  bd=7;
  EVGAX = 0.8*DisplayWidth(Dsp,DefaultScreen(Dsp));
  EVGAY= EVGAX*0.75;
  par=DefaultRootWindow(Dsp);
  xsh.flags=(PPosition|PSize);
  xsh.height=EVGAY;
  xsh.width=EVGAX;
  xsh.x=50;
  xsh.y=10;
  xswa.colormap=DefaultColormap(Dsp,DefaultScreen(Dsp));
  xswa.bit_gravity=NorthWestGravity;
  xswa.background_pixel=0;
  xswa.save_under=1;
  xswa.backing_store=WhenMapped;
  /*Win= XCreateSimpleWindow(Dsp,par,xsh.x,xsh.y,xsh.width,xsh.height,0,bd,bg);*/
  Win= XCreateWindow(Dsp,par,xsh.x,xsh.y,xsh.width,xsh.height,0,Dpth,InputOutput,Vis,
       (CWBackPixel|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
  XGetWindowAttributes(Dsp,Win,&xwa);
  EVGAX=xwa.width;
  EVGAY=xwa.height;
  msg_x = (EVGAX-316)/2;
  msg_y = EVGAY-24;
  XSetStandardProperties(Dsp,Win,"GLIB","GLIB",None,argv,argc,&xsh);
  XSetWMHints(Dsp,Win,&xwmh);
  Cmap=xswa.colormap;
  if(NCLRS<= 16)
    for(i=16;i<NCLRS;i++) get_color(i,&icode[i][0],&icode[i][2],&icode[i][1]);
  set_def_colors();
  xswa.colormap=Cmap;
  XChangeWindowAttributes(Dsp,Win,
    (CWBackPixel|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
  gcv.font=fontstruct->fid;
  gcv.foreground=10;
  gcv.background=0;
  gcv.line_width=0;
  Gc = XCreateGC(Dsp,Win,(GCFont|GCForeground|GCBackground),&gcv);
  c15.pixel=15;
  c0.pixel=0;
  Pix = XCreatePixmap(Dsp,Win,1,1,1);
  Curo= XCreateFontCursor(Dsp,59);
  Cur= XCreateFontCursor(Dsp,35);
/*Cur=XCreatePixmapCursor(Dsp,Pix,None,&c15,&c0,0,0);*/
  XDefineCursor(Dsp,Win,Curo);
  XSelectInput(Dsp,Win,ExposureMask);
  XMapWindow(Dsp,Win);
  XNextEvent(Dsp,&event);
/*
  XGrabPointer(Dsp,Win,False,
  PointerMotionMask|OwnerGrabButtonMask
  ,GrabModeAsync,GrabModeAsync,Win,Cur,CurrentTime);
  XSelectInput(Dsp,Win,PointerMotionMask);
  XGrabKey(Dsp,AnyKey,AnyModifier,Win,False,
  PointerMotionMask|OwnerGrabButtonMask
  ,GrabModeAsync,GrabModeAsync);
*/
/*
  XGrabButton(Dsp,AnyButton,AnyModifier,Win,False,
  PointerMotionMask|OwnerGrabButtonMask
  ,GrabModeAsync,GrabModeAsync,Win,Cur);
*/
/*XSelectInput(Dsp,Win,PointerMotionMask);*/
  XSelectInput(Dsp,Win,KeyPressMask|KeyReleaseMask);
  return;
}
void set_graphics_of_size(int xres,int yres)

{
  static int ENTRY=1;
  static int argc=1;
  static char *argv[1]={"Testing"};
  int i;
  Window par;
  XGCValues gcv;
  XEvent event;
  XSizeHints xsh;
  XSetWindowAttributes xswa;
  XWindowAttributes xwa;
  XFontStruct *fontstruct;
  unsigned int fg,bg,bd;
  Pixmap Pix;
  XColor c15,c0;
  GR_ON=1;
  Dsp = XOpenDisplay(NULL);
  if(Dsp == NULL) {printf(" Error: in opening Display ..\n");exit(0);};
  get_scan_code();
  Vis = XDefaultVisual(Dsp,DefaultScreen(Dsp));
  set_visual(Vis);
  Dpth =  DisplayPlanes(Dsp,DefaultScreen(Dsp));
  NCLRS = DisplayCells(Dsp,DefaultScreen(Dsp));
  fontstruct= XLoadQueryFont(Dsp,FONT);
  if(fontstruct ==NULL) {
    printf("Error: Invalid Font\n");
    printf("       Trying fixed\n");
    fontstruct= XLoadQueryFont(Dsp,"fixed");
    if(fontstruct ==NULL) exit(1);
  }
  bd = WhitePixel(Dsp,DefaultScreen(Dsp));
  bg = BlackPixel(Dsp,DefaultScreen(Dsp));
  fg = WhitePixel(Dsp,DefaultScreen(Dsp));
  bd=7;
  EVGAX = xres;
  EVGAY= yres;
  par=DefaultRootWindow(Dsp);
  xsh.flags=(PPosition|PSize);
  xsh.height=EVGAY;
  xsh.width=EVGAX;
  xsh.x=50;
  xsh.y=10;
  xswa.colormap=DefaultColormap(Dsp,DefaultScreen(Dsp));
  xswa.bit_gravity=NorthWestGravity;
  xswa.background_pixel=0;
  xswa.save_under=1;
  xswa.backing_store=WhenMapped;
  /*Win= XCreateSimpleWindow(Dsp,par,xsh.x,xsh.y,xsh.width,xsh.height,0,bd,bg);*/
  Win= XCreateWindow(Dsp,par,xsh.x,xsh.y,xsh.width,xsh.height,0,Dpth,InputOutput,Vis,
       (CWBackPixel|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
  XGetWindowAttributes(Dsp,Win,&xwa);
  EVGAX=xwa.width;
  EVGAY=xwa.height;
  msg_x = (EVGAX-316)/2;
  msg_y = EVGAY-24;
  XSetStandardProperties(Dsp,Win,"GLIB","GLIB",None,argv,argc,&xsh);
  XSetWMHints(Dsp,Win,&xwmh);
  Cmap=xswa.colormap;
  if(NCLRS<= 16)
    for(i=16;i<NCLRS;i++) get_color(i,&icode[i][0],&icode[i][2],&icode[i][1]);
  set_def_colors();
  xswa.colormap=Cmap;
  XChangeWindowAttributes(Dsp,Win,
    (CWBackPixel|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
  gcv.font=fontstruct->fid;
  gcv.foreground=10;
  gcv.background=0;
  gcv.line_width=0;
  Gc = XCreateGC(Dsp,Win,(GCFont|GCForeground|GCBackground),&gcv);
  c15.pixel=15;
  c0.pixel=0;
  Pix = XCreatePixmap(Dsp,Win,1,1,1);
  Curo= XCreateFontCursor(Dsp,59);
  Cur= XCreateFontCursor(Dsp,35);
  XDefineCursor(Dsp,Win,Curo);
  XSelectInput(Dsp,Win,ExposureMask);
  XMapWindow(Dsp,Win);
  XNextEvent(Dsp,&event);
  XSelectInput(Dsp,Win,KeyPressMask|KeyReleaseMask);
  t_initialise();
  return;
}
void Get_resolution(int *xres,int *yres)
{
  *xres=EVGAX;
  *yres = EVGAY;
}
void make_area(short width,short height)

{
  short n=2,i;
  static int ENTRY=1;
  static int argc=1;
  static char *argv[1]={"Testing"};
  Window par;
  XGCValues gcv;
  XEvent event;
  XSizeHints xsh;
  XSetWindowAttributes xswa;
  XWindowAttributes xwa;
  XFontStruct *fontstruct;
  unsigned int fg,bg,bd;
  Pixmap Pix;
  XColor c15,c0;
  Dsp = XOpenDisplay(NULL);
  if(Dsp == NULL) {printf(" Error: in opening Display ..\n");exit(0);};
  get_scan_code();
  Vis = XDefaultVisual(Dsp,DefaultScreen(Dsp));
  Dpth =  DisplayPlanes(Dsp,DefaultScreen(Dsp));
  NCLRS = DisplayCells(Dsp,DefaultScreen(Dsp));
  if(NCLRS>256) NCLRS=256;
  if(NCLRS==16) VGA16=1;
  fontstruct= XLoadQueryFont(Dsp,FONT);
  if(fontstruct ==NULL) {
    printf("Error: Invalid Font\n");
    printf("       Trying fixed\n");
    fontstruct= XLoadQueryFont(Dsp,"fixed");
    if(fontstruct ==NULL) exit(1);
  }
  bd = WhitePixel(Dsp,DefaultScreen(Dsp));
  bg = BlackPixel(Dsp,DefaultScreen(Dsp));
  fg = WhitePixel(Dsp,DefaultScreen(Dsp));
  bd=7;
  bg=8;
/*  EVGAY = DisplayHeight(Dsp,DefaultScreen(Dsp));
*/

  EVGAX = width;
  EVGAY= height;
  Dpth =  DisplayPlanes(Dsp,DefaultScreen(Dsp));
  par=DefaultRootWindow(Dsp);
  xsh.flags=(PPosition|PSize);
  xsh.height=EVGAY;
  xsh.width=EVGAX;
  xsh.x=200;
  xsh.y=200;
  xswa.colormap=DefaultColormap(Dsp,DefaultScreen(Dsp));
  xswa.bit_gravity=NorthWestGravity;
  xswa.background_pixel=0;
  xswa.save_under=1;
  xswa.backing_store=WhenMapped;
  /*Win= XCreateSimpleWindow(Dsp,par,xsh.x,xsh.y,xsh.width,xsh.height,0,bd,bg);*/
  Win= XCreateWindow(Dsp,par,xsh.x,xsh.y,xsh.width,xsh.height,0,Dpth,InputOutput,Vis,
       (CWBackPixel|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
  XGetWindowAttributes(Dsp,Win,&xwa);
  EVGAX=xwa.width;
  EVGAY=xwa.height;
  msg_x = (EVGAX-316)/2;
  msg_y = EVGAY-24;
  XSetStandardProperties(Dsp,Win," "," ",None,argv,argc,&xsh);
  XSetWMHints(Dsp,Win,&xwmh);
  xswa.colormap=DefaultColormap(Dsp,DefaultScreen(Dsp));
  Vis = XDefaultVisual(Dsp,DefaultScreen(Dsp));
  Cmap=xswa.colormap;
  if(ENTRY) {
   ENTRY=0;
   if(VGA16) for(i=0;i<16;i++) clrtbl[i]=i;
   else {
     get_color(22,&icode[8][0],&icode[8][2],&icode[8][1]);
     if( icode[8][0]>71 ) icode[7][0]=100;
     else icode[7][0] = icode[8][0]*14/10;
     if( icode[8][1]>71 ) icode[7][1]=100;
     else icode[7][1] = icode[8][1]*14/10;
     if( icode[8][2]>71 ) icode[7][2]=100;
     else icode[7][2] = icode[8][2]*14/10;
     get_match_color();
     for(i=16;i<NCLRS;i++) get_color(i,&icode[i][0],&icode[i][2],&icode[i][1]);
   }
  }
  if(!VGA16) Cmap = XCreateColormap(Dsp,Win,Vis,AllocAll);
  xswa.colormap=Cmap;
  XChangeWindowAttributes(Dsp,Win,
    (CWBackPixel|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
  gcv.font=fontstruct->fid;
  gcv.foreground=10;
  gcv.background=0;
  gcv.line_width=0;
  Gc = XCreateGC(Dsp,Win,(GCFont|GCForeground|GCBackground),&gcv);
  c15.pixel=15;
  c0.pixel=0;
  XQueryColor(Dsp,Cmap,&c15);
  XQueryColor(Dsp,Cmap,&c0);
  Pix = XCreatePixmap(Dsp,Win,1,1,1);
  Curo= XCreateFontCursor(Dsp,59);
  Cur= XCreateFontCursor(Dsp,35);
/*Cur=XCreatePixmapCursor(Dsp,Pix,None,&c15,&c0,0,0);*/
  XDefineCursor(Dsp,Win,Curo);
  XSelectInput(Dsp,Win,ExposureMask);
  XMapWindow(Dsp,Win);
  XNextEvent(Dsp,&event);
/*
  XGrabPointer(Dsp,Win,False,
  PointerMotionMask|OwnerGrabButtonMask
  ,GrabModeAsync,GrabModeAsync,Win,Cur,CurrentTime);
  XSelectInput(Dsp,Win,PointerMotionMask);
  XGrabKey(Dsp,AnyKey,AnyModifier,Win,False,
  PointerMotionMask|OwnerGrabButtonMask
  ,GrabModeAsync,GrabModeAsync);
*/
/*
  XGrabButton(Dsp,AnyButton,AnyModifier,Win,False,
  PointerMotionMask|OwnerGrabButtonMask
  ,GrabModeAsync,GrabModeAsync,Win,Cur);
*/
/*XSelectInput(Dsp,Win,PointerMotionMask);*/
  XSelectInput(Dsp,Win,KeyPressMask|KeyReleaseMask);
  return;
}

void setnormal()
{
  XCloseDisplay(Dsp);
  GR_ON=0;
  return;
}

void _move(int xp, int yp)
{
  cur_x=xp ;
  cur_y=yp ;
  return;
}

void _marker(int x,int y)
{
  short fac=1;
  int px[4],py[4];
  short style,mf;
  style = m_style%20;
  mf = m_style/20;
  fac +=mf;
  switch(style)
  {
	case 0 :
                 _circle_fill(x,y,(int)(3*fac),(unsigned char)(c_color));
                 break;
	case 1 :
                 _draw_line(x-3*fac,y-3*fac,x+3*fac,y+3*fac);
                 _draw_line(x-3*fac,y+3*fac,x+3*fac,y-3*fac);
                 break;
	case 2 : 
                 _box_fill(x-3*fac,y-3*fac,x+3*fac,y+3*fac,(unsigned char)c_color);
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
                 _poly_fill((short)3,px,py,(short)0,(unsigned char)c_color);
                 break;
        case 10:
                 px[0]=x-4*fac;py[0]=y;px[1]=x+4*fac;py[1]=y+4*fac;
                 px[2]=x+4*fac;py[2]=y-4*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned char)c_color);
                 break;
        case 11:
                 px[0]=x;py[0]=y+4*fac;px[1]=x-4*fac;py[1]=y-4*fac;
                 px[2]=x+4*fac;py[2]=y-4*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned char)c_color);
                 break;
        case 12:
                 px[0]=x;py[0]=y-4*fac;px[1]=x-4*fac;py[1]=y+4*fac;
                 px[2]=x+4*fac;py[2]=y+4*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned char)c_color);
                 break;
	case 13 :
                 _circle_fill(x,y,(int)(3*fac),(unsigned char)0);
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
                 _poly_fill((short)3,px,py,(short)0,(unsigned char)c_color);
                 break;
        case 17:
                 px[0]=x-2*fac;py[0]=y;px[1]=x+2*fac;py[1]=y+2*fac;
                 px[2]=x+2*fac;py[2]=y-2*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned char)c_color);
                 break;
        case 18:
                 px[0]=x;py[0]=y+2*fac;px[1]=x-2*fac;py[1]=y-2*fac;
                 px[2]=x+2*fac;py[2]=y-2*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned char)c_color);
                 break;
        case 19:
                 px[0]=x;py[0]=y-2*fac;px[1]=x-2*fac;py[1]=y+2*fac;
                 px[2]=x+2*fac;py[2]=y+2*fac;
                 _poly_fill((short)3,px,py,(short)0,(unsigned char)c_color);
                 break;
	default :break;
  }
}

unsigned char code(int x, int y)
{
  unsigned char t;
  t = 0;

  if (y > c_v_y2) t|=0x08;
  if (y < c_v_y1) t|=0x04;
  if (x > c_v_x2) t|=0x02;
  if (x < c_v_x1) t++;
  return(t);
}


short clip_line(int *xa,int *ya, int *xb, int *yb)
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

 void mline( short ixb, short iyb, short ixe,short iye)
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

       
void _draw_line(int x1,int y1,int x2,int y2)
{
 char far *addr;
 short i,dx,dy,f,g,inc1,inc2,ab,dx1,dy1,miny,maxy,minx,maxx;
 short clip,ch,rh,r,c;
 unsigned char temp;
 int xa,ya,xb,yb;
 xa =x1,xb=x2,ya=y1,yb=y2;
 clip=clip_line(&xa,&ya,&xb,&yb);
 if (clip==0) return;
 XDraw((short)xa,(short)ya,(short)xb,(short)yb);
 return;
}

void draw_line(int xa,int ya,int xb,int yb)
 {
   int i,i2,lnwidth;
   float dx,dy,ct,st,dl;
   int x1,y1,x2,y2,dx1,dy1;
   lnwidth= ln_width-2;
   if(lnwidth < 1) lnwidth=1;
   if( lnwidth ==1 ){ _draw_line( xa, ya, xb, yb);return;};
   if( xa>xb){x1=xa,xa=xb,xb=x1,y1=ya,ya=yb,yb=y1;};
   i2=lnwidth/2;
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
   for (i=1;i<lnwidth;i++)
        {
             dx1 =(int)( st*i+0.5);
             if( ct< 0.) dy1 =(int)( ct*i-0.5);
             else dy1 =(int)( ct*i+0.5);
            _draw_line( xa-dx1,ya+dy1,xb-dx1,yb+dy1);
        }
   return;
 }
void hori_line(int x1,int x2,int y)
{
  short i,clip;
  char far *addr;
  int xa,ya,xb,yb;
  int temp;
  if(x1==x2) return;
  xa =x1,xb=x2,ya=y,yb=y;
  clip=clip_line(&xa,&ya,&xb,&yb);
  if (clip==0) return;
  XDraw((short)xa,(short)ya,(short)xb,(short)yb);
}
void vert_line(int y1,int y2,int x)
{
  short i,clip;
  char far *addr;
  int xa,ya,xb,yb;
  int temp;
  if(y1==y2) return;
  xa =x,xb=x,ya=y1,yb=y2;
  clip=clip_line(&xa,&ya,&xb,&yb);
  if (clip==0) return;
  XDraw((short)xa,(short)ya,(short)xb,(short)yb);
}
void v_line(short ya,short yb,short x)
{
  XDraw((short)x,(short)ya,(short)x,(short)yb);

}
void h_line(short xa,short xb,short y)
{
  XDraw((short)xa,(short)y,(short)xb,(short)y);
}


void _draw_(int x, int y)
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

void _draw(int x, int y)
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
     if(ln_width==1) _draw_( x, y);
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
int berzier(int *xp,int *yp,float x1,float y1,float x4,float y4,float x2,
          float y2,float x3,float y3)
   {
     float t,tm,x,y,xm,ym,dist,dt;
     int n,notok,i;
     n=2;
     notok=1;
     while((n<20)&&notok)
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
int t_fillchr(float *x,float *y,int n,int *x1,int *y1,int *x2,int*y2)
  {
  int xp,yp,temp;
  float xx,yy,xx0,yy0,xx1,xx2,yy1,yy2,xx3,xx4,yy3,yy4;
  short i,j,j1,k;
  j=0;
  xx0=x[0]; yy0=y[0];
  x1[j] =(xx0)+0.5; y1[j] =(yy0)+0.5; j++;
  i=1;
  while(i<n)
  {
    xx =x[i]; yy =y[i]; i++;
    xx1 =x[i]; yy1 =y[i];i++;
    j1=j-1;
    if((xx0==xx1)&&(yy0==yy1))
        {
         xp=(xx+0.5);yp=(yy+0.5);
         if((xp!=x1[j1])||(yp!=y1[j1])){x1[j]=xp;y1[j]=yp; j++;};
        }
    else
        {
         xp=(xx+0.5);yp=(yy+0.5);
         if((xp!=x1[j1])||(yp!=y1[j1]))
            {
            xx2=(xx1);yy2=(yy1);
            /*xx1=x1[j1];yy1=y1[j1];*/
            xx1=xx0;yy1=yy0;
            xx4=xx,yy4=yy;
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
void t_fillchr_o(float *x,float *y,int code,int n)
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
             _move(x1[0],y1[0]);
             for(i=1;i<j;i++)_draw(x1[i],y1[i]);
             _draw(x1[0],y1[0]);
             set_clr(temp);
            }
/*
*/
  return;
}
void _mpoly_fill(short n,int *px,int *py,int *px1,int *py1,unsigned char b_color)
{
  short j,imd;
  short i,cnt,ind;
  int xcord[500];
  int xlow,xup;
  int txx;
  int ymax[500],ymin[500],xmax[500],xmin[500],tyx,tyn,txn,scan,ygrt;
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
	if (ygrt < ymax[i]) ygrt=ymax[i];
	if (scan > ymin[i]) scan=ymin[i];
	if (xup < xmax[i]) xup=xmax[i];
	if (xlow > xmin[i]) xlow=xmin[i];
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
  set_clr(c_color);
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
           hori_line(xcord[i],xcord[i+1],scan);
	 scan++;
  }
  c_color=tempc;
  set_clr(c_color);
}
void drawg0ch(c)
char c;
{
  float x[300],y[300];
  int x1[500],y1[500],x2[500],y2[500];
  int code,n,i,xdum,ydum,pnts;
  short ptr;
  short j,pnt,asc,tempc;
  float fact=1.0;
  int xo,yo;
  unsigned char color;
  if (t_pattern==15) color=t_color;
  else color=t_pattern;
  tempc=ln_width;
  ln_width=txt_bold;
/*
  txt_w42=txt_wt/3000.0;
  txt_h42=txt_ht/3000.0;
*/
  asc=c;
  pnt=asc-32+greek;
  fact = m_f[pnt];
  xo=(int)(cx+(xp));
  if( pr_txt==0) { fact=1.0;}
  else xo -=(1.-fact)*0.5*txt_wt;
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
        if(code !=0)
         {
         if(t_fill)
          {
          _mpoly_fill((short)pnts,x1,y1,x2,y2,color);
          }
         if(t_bodr){
             temp =c_color;set_clr(t_color);
             for(i=0;i<pnts;i++) draw_line(x1[i],y1[i],x2[i],y2[i]);
             set_clr(temp);
            }
          pnts=0;
         }
      }
     }
     xp+=(txt_wt*fact+txt_sp);
     ln_width=tempc;
     return;
 }
void drawgch(c)
char c;
{
  float x[300],y[300];
  int code,n,i;
  int x1[500],y1[500],x2[500],y2[500];
  int pnts;
  float xdum,ydum;
  short ptr;
  short j,pnt,asc,tempc;
  float xa,ya;
  float fact=1.0;
  float xo,yo;
  unsigned char color;
  if (t_pattern==15) color=t_color;
  else color=t_pattern;
  tempc=ln_width;
  ln_width=txt_bold;
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
        if(code !=0)
         {
         if(t_fill)
          {
          _mpoly_fill((short)pnts,x1,y1,x2,y2,color);
          }
         if(t_bodr){
             temp =c_color;set_clr(t_color);
             for(i=0;i<pnts;i++) draw_line(x1[i],y1[i],x2[i],y2[i]);
             set_clr(temp);
            }
          pnts=0;
         }
      }
     }
     xp+=(txt_wt*fact+txt_sp);
     ln_width=tempc;
     return;
 }
void drawg0co(c)
char c;
{
  float x[150],y[150];
  int code,n,i,xdum,ydum;
  short ptr;
  short j,pnt,asc,tempc;
  float fact=1.0;
  int xo,yo;
  tempc=ln_width;
  ln_width=txt_bold;
/*
  txt_w42=txt_wt/3000.0;
  txt_h42=txt_ht/3000.0;
*/
  asc=c;
  pnt=asc-32+greek;
  fact = m_f[pnt];
  xo=(int)(cx+(xp));
  if( pr_txt==0) { fact=1.0;}
  else xo -=(1.-fact)*0.5*txt_wt;
  yo=(int)(cy+yp);
  if (pnt<0) return;
  if(pnt>223) return;
  ptr= icpos[pnt];
  if((xo>=c_v_xt1)&&(xo<=c_v_x2)&& (yo>=c_v_yt1)&& (yo<=c_v_y2))
     {
     code= icxv[ptr];n=icyv[ptr];
     while ( code >= 0)
      {
        ptr++;
        for(i=0;i<n;i++){xdum=icxv[ptr];ydum=icyv[ptr];ptr++;
                         x[i]=xdum*txt_w42+xo;y[i]=yo+ydum*txt_h42;}
        t_fillchr_o(x,y,code,n);
/*
*/
        code= icxv[ptr];n=icyv[ptr];
      }
     }
     xp+=(txt_wt*fact+txt_sp);
     ln_width=tempc;
     return;
 }
void drawgcho(c)
char c;
{
  float x[150],y[150];
  int code,n,i;
  float xdum,ydum;
  short ptr;
  short j,pnt,asc,tempc;
  float xa,ya;
  float fact=1.0;
  float xo,yo;
  tempc=ln_width;
  ln_width=txt_bold;
/*
  txt_w42=txt_wt/3000.0;
  txt_h42=txt_ht/3000.0;
*/
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
  if((xa>=c_v_xt1)&&(xa<=c_v_x2)&& (ya>=c_v_yt1)&& (ya<=c_v_y2))
     {
     code= icxv[ptr];n=icyv[ptr];
     while ( code >= 0)
      {
        ptr++;
        for(i=0;i<n;i++){xdum=icxv[ptr];ydum=icyv[ptr];ptr++;
	     xdum=(xo+xdum*txt_w42);
	     ydum=(yo+ydum*txt_h42);
	     x[i]=(cx+(xdum)*cost+(ydum)*sint);
	     y[i]=(cy+(ydum)*cost-(xdum)*sint);
            }
        t_fillchr_o(x,y,code,n);
/*
*/
        code= icxv[ptr];n=icyv[ptr];
      }
     }
     xp+=(txt_wt*fact+txt_sp);
     ln_width=tempc;
     return;
 }
void drawg0ch_o(c)
char c;
{
  short j,pnt,asc,tempc;
  float fact=1.0;
  int x,y,xa,ya,xo,yo;
  float tempx,tempy;
  tempc=ln_width;
  ln_width=txt_bold;
  asc=c;
  pnt=asc-33+greek;
  fact = m_f[pnt+1];
  xo=(int)(cx+(xp));
  if( pr_txt==0)
            { xo +=(1.-fact)*0.5*txt_wt;
              fact=1.0;}
  yo=(int)(cy+yp);
  if((xo>=c_v_xt1)&&(xo<=c_v_x2)&& (yo>=c_v_yt1)&& (yo<=c_v_y2))
     {
      if (pnt>=0)
	 {
             j=icpos[pnt];
	     tempx=((float)icxv[j]*txt_w42);
	     tempy=((float)icyv[j]*txt_h42);
	     xa=(int)(xo+(tempx));
	     ya=(int)(yo+(tempy));
	     for(j=icpos[pnt]+1;j<icpos[pnt+1];j++)
		{
                  if ( (icxv[j]==icxv[j-1]) && (icyv[j]==icyv[j-1]) )
		  {
			 j++;
                	 tempx=((float)icxv[j]*txt_w42);
			 tempy=((float)icyv[j]*txt_h42);
			 xa=(int)(xo+(tempx));
			 ya=(int)(yo+(tempy));
			 continue;
		  }
		  tempx=((float)icxv[j]*txt_w42);
		  tempy=((float)icyv[j]*txt_h42);
		  x=(int)(xo+(tempx));
		  y=(int)(yo+(tempy));
                  draw_line(xa,ya,x,y);
		  xa=x;ya=y;
		}
          }
     }
     xp+=(txt_wt*fact+txt_sp);
     ln_width=tempc;
     return;
 }
void drawgch_o(c)
char c;
{
  short j,pnt,asc,tempc;
  int x,y,xa,ya;
  float fact=1.0;
  float tempx,tempy;
   
  fact =1.0;
  tempc=ln_width;
  ln_width=txt_bold;
  asc=c;
  pnt=asc-33+greek;
  if( pr_txt>0) fact = m_f[pnt+1];
  xa=(int)(cx+(xp)*cost+yp*sint);
  ya=(int)(cy-(xp)*sint+yp*cost);
  if((xa>=c_v_xt1)&&(xa<=c_v_x2)&& (ya>=c_v_yt1)&& (ya<=c_v_y2))
     {
      if (pnt>=0)
	 {
             j=icpos[pnt];
	     tempx=(xp+(float)icxv[j]*txt_w42);
	     tempy=(yp+(float)icyv[j]*txt_h42);
	     xa=(int)(cx+(tempx)*cost+(tempy)*sint);
	     ya=(int)(cy+(tempy)*cost-(tempx)*sint);
	     for(j=icpos[pnt]+1;j<icpos[pnt+1];j++)
		{
                  if ( (icxv[j]==icxv[j-1]) && (icyv[j]==icyv[j-1]) )
		  {
			 j++;
			 tempx=(xp+(float)icxv[j]*txt_w42);
			 tempy=(yp+(float)icyv[j]*txt_h42);
			 xa=(int)(cx+(tempx)*cost+(tempy)*sint);
			 ya=(int)(cy+(tempy)*cost-(tempx)*sint);
			 continue;
		  }
		  tempx=((float)icxv[j]*txt_w42+xp);
		  tempy=((float)icyv[j]*txt_h42+yp);
		  x=(int)(cx+(tempx)*cost+(tempy)*sint);
		  y=(int)(cy+(tempy)*cost-(tempx)*sint);
		  if((x!=xa)||(y!=ya))  draw_line(xa,ya,x,y);
		  xa=x;ya=y;
		}
          }
     }
     xp+=(txt_wt*fact+txt_sp);
     ln_width=tempc;
     return;
 }
   float get_val(char *ch)
    {
      char chr[4];
      short err;
      float val;
      chr[0]=*ch;chr[1]=*(ch+1);chr[2]=*(ch+2);chr[3]=' ';
      err = sscanf(chr ,"%f",&val);
      if (err<1) return(1.0);
      else return(val);
    }

  void setsubsup(float *fact,short *ishft,char cntl)
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
  void resetsubsup(fact,ishft)
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


void _box_fill(int x1,int y1,int x2,int y2,unsigned char color)
{
  int x_min,y_min,x_max,x_max1,x_min1,y_max,i,j;
  short po1;
  unsigned char temp;
  temp=c_color;
  c_color=color;
  set_clr(c_color);
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
      XFillRectangle(Dsp,Win,Gc,x_min,EVGAY-y_max,x_max1-x_min+1,y_max-y_min+1);
    }
}
  c_color=temp;
  set_clr(c_color);
  return;
}


void clear_screen()
{
 _box_fill(v_x1,v_y1,v_x2,v_y2,0x00);
 return;
}


void _poly_fill(short n,int *px,int *py,short flag,unsigned char b_color)
{
  short j,imd;
  short i,cnt,ind;
  int xcord[15];
  int xlow,xup;
  int txx;
  int ymax[300],ymin[300],xmax[300],xmin[300],tyx,tyn,txn,scan,ygrt;
  unsigned char tempc;
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
	if (ygrt < ymax[i]) ygrt=ymax[i];
	if (scan > ymin[i]) scan=ymin[i];
	if (xup < xmax[i]) xup=xmax[i];
	if (xlow > xmin[i]) xlow=xmin[i];
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
  set_clr(c_color);
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
           hori_line(xcord[i],xcord[i+1],scan);
	 scan++;
  }
  c_color=tempc;
  set_clr(c_color);
  if (flag==1)
      for(i=0;i<n;i++) _draw_line(px[i],py[i],px[(i+1)%n],py[(i+1)%n]);
}


void _circle(int x,int y,int r)
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

void _circle_fill(int x,int y,int r,unsigned char color)
{
  int i,j;
  int xm,ym;
  double t,sqrd;
  unsigned char temp;
  sqrd=(double)r*(double)r;
  ym=y-(short)(r/sqrt((double)2.));
  temp=c_color;
  
  put_pixel(x,y+r),put_pixel(x,y-r);
  put_pixel(x+r,y),put_pixel(x-r,y);
  c_color=color;
  set_clr(c_color);
  hori_line(x-r+1,x+r-1,y);
  for(j=1;j<r;j++)
    {
	t=sqrd-(float)(j*j);
	i=(sqrt(t))+0.5;
        c_color=temp;
        set_clr(c_color);
        put_pixel(x-i,y-j);
        put_pixel(x+i,y-j);
        put_pixel(x-i,y+j);
        put_pixel(x+i,y+j);
        c_color=color;
        set_clr(c_color);
        hori_line(x-i+1,x+i-1,y-j);
        hori_line(x-i+1,x+i-1,y+j);
    } 
  c_color=temp;
  set_clr(c_color);
}
void alloc_scrn_buffer(void)
 {
  SC_BUF *tmp;
  if(scpt==NULL){
    scpt=(SC_BUF *)malloc(sizeof(SC_BUF));
    scpt->nx=NULL;
    scpt->pv=NULL;
  }
  else {
    tmp=(SC_BUF *)malloc(sizeof(SC_BUF));
    tmp->nx=NULL;
    tmp->pv=scpt;
    scpt->nx=tmp;
    scpt=scpt->nx;
  }
 }
void release_scrn_buffer(void)
 {
  SC_BUF *tmp;
  if(scpt==NULL){
    return;
  }
  else {
    tmp=scpt;
    scpt=scpt->pv;
    free(tmp);
  }
 }
void clear_scrn_buffer(void)
 {
  SC_BUF *tmp;
  while(scpt !=NULL) {
    tmp=scpt;
    scpt=scpt->pv;
    free(tmp);
  }
 }
void scr_back(int x1,int y1,int x2,int y2)
{
  int i,j,addr;
  alloc_scrn_buffer();
  scpt->x1=x1; scpt->x2=x2;
  scpt->y1=y1; scpt->y2=y2;
  scpt->buf=XCPix(x1,y1,x2,y2);
  XCopyArea(Dsp,Win,(scpt->buf),Gc,(short)x1,(short)(y1)
    ,(short)(x2-x1+1),(short)(y2-y1+1),0,0);
}

void scr_recover()
{
  int i,j,addr;
  int x1,x2,y1,y2;
  x1 = scpt->x1; x2 = scpt->x2;
  y1 = scpt->y1; y2 = scpt->y2;
  XCopyArea(Dsp,(scpt->buf),Win,Gc,0,0
    ,(short)(x2-x1+1),(short)(y2-y1+1),(short)x1,(short)y1);
  XFreePixmap(Dsp,scpt->buf);
  release_scrn_buffer();
}
/* copying from off screen to screen
   no clearing of off screen memory
   made for moving logo
*/
void scr_copy(int x0,int y0)
{
  int i,j,addr,index_bk;
  int x1,x2,y1,y2;
  x1 = scpt->x1 + x0; x2 = scpt->x2 + x0;
  y1 = scpt->y1 + y0; y2 = scpt->y2 + y0;
  XCopyArea(Dsp,(scpt->buf),Win,Gc,0,0
    ,(short)(x2-x1+1),(short)(y2-y1+1),(short)x1,(short)y1);
}


void draw_o_cursor()
{
  unsigned char tempc;
  tempc=c_color;
  c_color=0x0F;
  set_clr(c_color);
  hori_line(gcur_x-8,gcur_x+8,EVGAY-gcur_y);
  vert_line(EVGAY-gcur_y-8,EVGAY-gcur_y+8,gcur_x);
  c_color=tempc;
  set_clr(c_color);
}
void draw_cursor()
{
  unsigned char tempc;
  char nbuf[35];
  float x,y;
  if(gcur_x>v_x2 ){gcur_x=v_x2;gbell();};
  if(gcur_y>EVGAY-v_y1-2 ){gcur_y=EVGAY-v_y1-2;gbell();};
  if(gcur_x<v_x1 ){gcur_x=v_x1;gbell();};
  if(gcur_y<EVGAY-v_y2 ){gcur_y=EVGAY-v_y2;gbell();};
  x= usr_x(gcur_x);
  y= usr_y(EVGAY-gcur_y);
  sprintf(nbuf,"X=%14.5e Y=%14.5e",x,y);
  nbuf[34]='\0';
  msg_menu(msg_x,msg_y,34,nbuf);
  XWarpPointer(Dsp,Win,Win,0,0,EVGAX+1,EVGAY+1,gcur_x,gcur_y);
}
void draw_box_cursor()
{
  unsigned char tempc;
  char nbuf[35]={"POSITION FOR PLACING TEXT......"};
  float x,y;
  if(gcur_x>v_x2 ){gcur_x=v_x2;gbell();};
  if(gcur_y>EVGAY-v_y1-2 ){gcur_y=EVGAY-v_y1-2;gbell();};
  if(gcur_x<v_x1 ){gcur_x=v_x1;gbell();};
  if(gcur_y<EVGAY-v_y2 ){gcur_y=EVGAY-v_y2;gbell();};
  x= usr_x(gcur_x);
  y= usr_y(EVGAY-gcur_y);
  msg_menu(msg_x,msg_y,34,nbuf);
  XWarpPointer(Dsp,Win,Win,0,0,EVGAX+1,EVGAY+1,gcur_x,gcur_y);
}
void draw_po_cursor()
{
  unsigned char tempc;
  char nbuf[35];
  float x,y;
/*  if(gcur_x>v_x2 ){gcur_x=v_x2;gbell();};
  if(gcur_y>EVGAY-v_y1-2 ){gcur_y=EVGAY-v_y1-2;gbell();};
  if(gcur_x<v_x1 ){gcur_x=v_x1;gbell();};
  if(gcur_y<EVGAY-v_y2 ){gcur_y=EVGAY-v_y2;gbell();};
*/
  x= usr_x(gcur_x);
  y= usr_y(EVGAY-gcur_y);
  sprintf(nbuf,"X=%14.5e Y=%14.5e",x,y);
  nbuf[34]='\0';
  msg_menu(msg_x,msg_y,34,nbuf);
 /* XWarpPointer(Dsp,Win,Win,0,0,EVGAX+1,EVGAY+1,gcur_x,gcur_y);
*/
}


char cursor_move(float *xx,float*yy)
{
  char ch,key;
  gcur_x = scr_x(*xx);
  gcur_y = EVGAY-scr_y(*yy);
  scr_back(gcur_x-8,gcur_y-8,gcur_x+8,gcur_y+8);
  draw_cursor();
  kb.kbint = get_key(1);
  ch=kb.kbc[0];
  key=ch;
  while (ch!='\r')
  {
    if (ch!='\0')
    {
     if ((isdigit(ch))&&(ch!='0')) jmp=ch-'0';
     else
     {
      scr_recover();
      *xx = usr_x(gcur_x);
      *yy = usr_y((EVGAY-gcur_y));
      return(key);
     }
    }
   else
   {
    ch=kb.kbc[1];
    switch(ch)
    {
      case 'H' : cursor_up();
		 break;
      case 'P' : cursor_down();
		 break;
      case 'K' : cursor_left();
		 break;
      case 'M' : cursor_right();
		 break;
      default  : ch=getch();
		 continue;
     }
    }
     kb.kbint = get_key(1);
     ch=kb.kbc[0];
     key = ch;
  }
  scr_recover();
  *xx = usr_x(gcur_x);
  *yy = usr_y((EVGAY-gcur_y));
  return(key);
}

void cursor_left()
{
  gcur_x-=jmp;
  draw_cursor();
}
void cursor_right()
{
  gcur_x+=jmp;
  draw_cursor();
}

void cursor_up()
{
  gcur_y-=jmp;
  draw_cursor();
}
void cursor_down()
{
   gcur_y+=jmp;
   draw_cursor();
}

void draw_lcur()
{
  unsigned char temp;
  temp=c_color;
  c_color=0x0F;
  set_clr(c_color);
  hori_line(lcur_x,lcur_x+16,EVGAY-lcur_y);
  vert_line(EVGAY-lcur_y,EVGAY-lcur_y+16,lcur_x);
  c_color=temp;
  set_clr(c_color);
}

void draw_rcur()
{
  unsigned char temp;
  temp=c_color;
  c_color=0x0F;
  set_clr(c_color);
  hori_line(rcur_x,rcur_x-16,EVGAY-rcur_y);
  vert_line(EVGAY-rcur_y,EVGAY-rcur_y-16,rcur_x);
  c_color=temp;
  set_clr(c_color);
}

char lzoom()
{
  char ch,key;
  int pointer,dx,dy,incr;
  jmp = 8;
  jmpy = (float)jmp/(v_x2-v_x1)*(v_y2-v_y1);
  ylcur = (float)lcur_y;
  yrcur = (float)rcur_y;
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
/*  kb.kbint = get_key(1);
  ch=kb.kbc[0];
  key=ch;
  while (ch!='\r')*/
  for(;;)
  {
   if((kb.kbint=get_kb(0))>0)
   {
    ch=kb.kbc[0];
    if (ch!='\0')
    {
     if ((isdigit(ch))&&(ch!='0'))
      {jmp=ch-'0'; jmpy = (float)jmp/(v_x2-v_x1)*(v_y2-v_y1);};
     if (ch=='\r') goto jump;
    }
    else
    {
     ch=kb.kbc[1];
     switch(ch)
     {
      case 'H' :
      case 'M' : zoom_up();
		 break;
      case 'P' :
      case 'K' : zoom_down();
		 break;
      default  : 
		 break;
     }
    }

   }
  }
 jump:
  key=ch;
  scr_recover();
  scr_recover();
  return(key);
}

void pointer_zoom()
{
  int key,entry,x0,y0,x1,y1,xmin,ymin,xmax,ymax;
  int x2,y2;
  entry=0;
  while(GetPointer(&x1,&y1)>0);
  while(GetPointer(&x0,&y0)<=0);
  x2=x0,y2=y0;
  while(GetPointer(&x1,&y1)>0);
  gbell();
  while((key=GetPointer(&x1,&y1))<=0)
    {
     if((key==0)&&((x2!=x1)||(y2!=y1)))
       {
        if(entry)
          {
           scr_recover();
           scr_recover();
           scr_recover();
           scr_recover();
          };
        entry=1;
        if(x0<x1)xmin=x0,xmax=x1;
        else     xmin=x1,xmax=x0;
        if(y0<y1)ymin=y0,ymax=y1;
        else     ymin=y1,ymax=y0;
        scr_back(xmin,ymin,xmax,ymin+1);
        scr_back(xmin,ymax-1,xmax,ymax);
        scr_back(xmin,ymin,xmin+1,ymax);
        scr_back(xmax-1,ymin,xmax,ymax);
        _draw_bound(xmin,EVGAY-ymin,xmax,EVGAY-ymax);
        x2=x1,y2=y1;
       }
   }
  if(entry)
          {
           scr_recover();
           scr_recover();
           scr_recover();
           scr_recover();
          };
   if(x0<x1)xmin=x0,xmax=x1;
   else     xmin=x1,xmax=x0;
   if(y0<y1)ymin=y0,ymax=y1;
   else     ymin=y1,ymax=y0;
      
   if( (xmin!=xmax)&&(ymin!=ymax))
      lcur_x=xmin,lcur_y=ymax,rcur_x=xmax,rcur_y=ymin;
   while((GetPointer(&x1,&y1))>0);
   return;
}

void zoom_up()
{
  scr_recover();
  scr_recover();
  lcur_x-=jmp;
/*  lcur_y+=jmp-2;*/
  ylcur += jmpy;
  lcur_y = ylcur+0.5;
  rcur_x+=jmp;
  yrcur -= jmpy;
  rcur_y = yrcur+0.5;
/*  rcur_y-=jmp-2;*/
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
}

void zoom_down()
{
  scr_recover();
  scr_recover();
  lcur_x+=jmp;
/*  lcur_y-=jmp-2;*/
  ylcur -= jmpy;
  lcur_y = ylcur+0.5;
  rcur_x-=jmp;
/*  rcur_y+=jmp-2;*/
  yrcur += jmpy;
  rcur_y = yrcur+0.5;
  if (lcur_y<=rcur_y)
  {
    gbell();
    lcur_x-=jmp;
    ylcur +=jmpy;
    lcur_y = ylcur+0.5;
    rcur_x+=jmp;
    yrcur -= jmpy;
    rcur_y = yrcur+0.5;
  }
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
}

char pan()
{
  char ch,key;
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
  kb.kbint = get_key(1);
  ch=kb.kbc[0];
  key=ch;
  while (ch!='\r')
  {
    while (ch!='\0')
    {
     if ((isdigit(ch))&&(ch!='0'))
     jmp=ch-'0';
     key=ch;
     kb.kbint = get_key(1);
     ch=kb.kbc[0];
     if (ch=='\r')
     {
       scr_recover();
       scr_recover();
       return(key);
     }
    }
    ch=kb.kbc[1];
    switch(ch)
    {
      case 'H' : pan_up();
		 break;
      case 'K' : pan_left();
		 break;
      case 'P' : pan_down();
		 break;
      case 'M' : pan_right();
		 break;
      default  : 
		 break;
     }
     kb.kbint = get_key(1);
     ch=kb.kbc[0];
  }
  scr_recover();
  scr_recover();
  return(key);
}

void pan_up()
{
  scr_recover();
  scr_recover();
  lcur_y-=jmp;
  rcur_y-=jmp;
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
}

void pan_down()
{
  scr_recover();
  scr_recover();
  lcur_y+=jmp;
  rcur_y+=jmp;
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
}

void pan_left()
{
  scr_recover();
  scr_recover();
  lcur_x-=jmp;
  rcur_x-=jmp;
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
}

void pan_right()
{
  scr_recover();
  scr_recover();
  lcur_x+=jmp;
  rcur_x+=jmp;
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
}

char stretch()
{
  char ch,key;
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
  kb.kbint = get_key(1);
  ch=kb.kbc[0];
  key=ch;
  while (ch!='\r')
  {
    while (ch!='\0')
    {
     if ((isdigit(ch))&&(ch!='0'))
     jmp=ch-'0';
     key=ch;
     kb.kbint = get_key(1);
     ch=kb.kbc[0];
     if (ch=='\r')
     {
       scr_recover();
       scr_recover();
       return(key);
     }
     }
    ch=kb.kbc[1];
    switch(ch)
    {
      case 'H' : vstretch();
		 break;
      case 'M' : hstretch();
		 break;
      case 'P' : vshrink();
		 break;
      case 'K' : hshrink();
		 break;
      default  : 
		 break;
     }
     kb.kbint = get_key(1);
     ch=kb.kbc[0];
  }
  scr_recover();
  scr_recover();
  return(key);
}

void vstretch()
{ 
  scr_recover();
  scr_recover();
  lcur_y+=jmp;
  rcur_y-=jmp;
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
}

void vshrink()
{
  scr_recover();
  scr_recover();
  lcur_y-=jmp;
  rcur_y+=jmp;
  if (lcur_y<=rcur_y)
  {
    gbell();
    lcur_y+=jmp;
    rcur_y-=jmp;
  }
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
}

void hstretch()
{
  scr_recover();
  scr_recover();
  lcur_x-=jmp;
  rcur_x+=jmp;
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
}

void hshrink()
{
  scr_recover();
  scr_recover();
  lcur_x+=jmp;
  rcur_x-=jmp;
  if (lcur_x>=rcur_x)
  {
    gbell();
    lcur_x-=jmp;
    rcur_x+=jmp;
  }
  scr_back(lcur_x,lcur_y-16,lcur_x+16,lcur_y);
  scr_back(rcur_x-16,rcur_y,rcur_x,rcur_y+16);
  draw_lcur();
  draw_rcur();
}
void gbell(void)
{
 if(GR_ON) XBell(Dsp,1);
}

void clr_view(void)
 {
   char ch;
  _box_fill(v_x1,v_y1,v_x2,v_y2,0);
 }
void _godard_fill(short n,int *px,int *py,short flag,float *pv)
{
  short j,imd;
  short i,cnt,ind;
  int xcord[15];
  int xlow,xup;
  int txx;
  int ymax[15],ymin[15],xmax[15],xmin[15],tyx,tyn,txn,scan,ygrt;
  float pmax[15],pmin[15],pcord[15],tpx,tpn,tpp;
  unsigned char tempc;
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
	if (ygrt < ymax[i]) ygrt=ymax[i];
	if (scan > ymin[i]) scan=ymin[i];
	if (xup < xmax[i]) xup=xmax[i];
	if (xlow > xmin[i]) xlow=xmin[i];
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
	 for(i=ind;i<n;++i)
	 {
	  if (ymin[i]<=scan)
	  {
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
	 for (i=0;i<cnt;++i)
	 {
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
  if (flag==1)
      for(i=0;i<n;i++) _draw_line(px[i],py[i],px[(i+1)%n],py[(i+1)%n]);
}
void godr_line(int xa,int xb,int y,float pa,float pb)
{
  int i,pos,p1,p2,yy;
  int temp;
  int tempc,clr;
  float tempp,pval,pfac;
  if((y>c_v_y2) || (y<c_v_y1) ) return;
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
        tempc=c_color;
        c_color=p1;
        set_clr(c_color);
        hori_line(xa,xb,y);
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
  for(i=xa;i<=xb;i++) 
  {
  tempc=(int)(pa+(i-xa)*pfac+0.5);
  if(tempc<1) tempc=1;
  if(tempc>max_clr) tempc=max_clr;
  tempc +=min_clr;
  set_clr(tempc);
  put_pixl(i,yy);
  }
  set_clr(c_color);
}
void _rect_fill(short x1,short y1,short x2,short y2,unsigned char color)
{
  short x_min,y_min,x_max,x_max1,x_min1,y_max,i,j,po1;
  unsigned char temp;
  temp=c_color;
  c_color=color;
  set_clr(c_color);
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
      XFillRectangle(Dsp,Win,Gc,x_min,EVGAY-y_max,x_max1-x_min+1,y_max-y_min+1);
    }
}
  c_color=temp;
  set_clr(c_color);
  return;
}

void menuch(char c,int ix1,int iy1,int char_clr)
{
 short jj,k,i,iy;
 char str[2];
 unsigned char temp;
 temp=c_color;
 c_color=char_clr;
 set_clr(c_color);
 if (c < 33 | c> 127) goto L100;
   iy=EVGAY-iy1-11;
   str[0]=c,str[1]='\0';
   XDrawString(Dsp,Win,Gc,ix1+2,EVGAY-iy1,str,1);
  L100:
  c_color=temp;
  set_clr(c_color);
  return;
}

short p_control()
{
  short sam,b_x,b_y;
  char ch;
  b_y=462;
  b_x=5;
  kb.kbint = get_key(1);
  ch=kb.kbc[0];
  sam=462-(nmenu-1)*28;
  while ((ch!='\r'))
  {
    while(ch!='\0')
    {
     kb.kbint = get_key(1);
     ch=kb.kbc[0];
     if (ch=='\r') return(b_y);
    }
   ch=kb.kbc[1];
    switch(ch)
    {
      case 'H' : _box_fill(b_x+1,b_y+1,b_x+4,b_y+7,0x0F);
		 if (b_y==462)
		 b_y=sam;
		 else b_y+=28;
		 _box_fill(b_x+1,b_y+1,b_x+4,b_y+7,0x04);
		 break;
      case 'P' : _box_fill(b_x+1,b_y+1,b_x+4,b_y+7,0x0F);
		 if (b_y==sam)
		   b_y=462;
		 else b_y-=28;
		 _box_fill(b_x+1,b_y+1,b_x+4,b_y+7,0x04);
		 break;
      default  : 
                 kb.kbint = get_key(1);
                 ch=kb.kbc[0];
		 continue;
     }
  }
  return(b_y);
}

char *h_int(float *x,float *y)
  {
      unsigned char *loc,*loco;
      char key,cr,bsp;
      short ihori,i,count,k,counto;
      int itxclr_o,pattern_o;
      float xx,yy,ytwtg;
      float tw,th,tg;
      tw=txt_wt*(w_x2-w_x1)/V_x;
      tg=txt_sp*(w_x2-w_x1)/V_x;
      itxclr_o=t_color;
      pattern_o=t_pattern;
      cr = 13;
      bsp = 8;
      if( fabs((double)sint)<0.0001) ihori=1;
      else ihori=0;
      loc = cmds;
      ytwtg = ( tw+tg)/(w_x2-w_x1)*(w_y2-w_y1)/0.75;
      if((key = gincur(x,y)) == '.')
      { 
       bf[0]=key;
       bf[1]='\0';
       }
     else
      {
       xx = *x;
       yy = *y;
       i = 0;
       bf[i]='\0';
       counto=count;loco=loc;
      while((ch = getch()) != cr)
       {
       if( ch == bsp ) {
                      if(i!=0){ 
                       if( ihori==1) xx = xx-tw-tg;
                       else yy = yy -ytwtg;
                       t_move(xx,yy);
                       t_txt_clr(0);
                       t_txtpattern(0);
                       t_txt_wr(1,&bf[i-1]);
                       t_txt_clr(itxclr_o);
                       t_txtpattern(pattern_o);
                       loc--;count--;
                       i--; bf[i]='\0';};
                      }
       else
         {
           bf[i] = ch;
           bf[++i]='\0';
           t_move(xx,yy);
           t_txt_wr(1,&bf[i-1]);
           if (ihori == 1) xx = xx+tw+tg;
           else yy = yy+ytwtg;
           count++;*(loc++)=12;
          }
      }
    }
     return(bf);
  }
char *h_int_o(float *x,float *y)
  {
      unsigned char *loc,*loco;
      char key,cr,bsp;
      short ihori,i,count,k,counto;
      int itxclr_o;
      float xx,yy,ytwtg;
      float tw,th,tg;
      tw=txt_wt*(w_x2-w_x1)/V_x;
      tg=txt_sp*(w_x2-w_x1)/V_x;
      itxclr_o=t_color;
      cr = 13;
      bsp = 8;
      if( fabs((double)sint)<0.0001) ihori=1;
      else ihori=0;
      loc = cmds;
      ytwtg = ( tw+tg)/(w_x2-w_x1)*(w_y2-w_y1)/0.75;
      if((key = gincur(x,y)) == '.')
      { 
       bf[0]=key;
       bf[1]='\0';
       }
     else
      {
       xx = *x;
       yy = *y;
       i = 0;
       bf[i]='\0';
       counto=count;loco=loc;
      while((ch = getch()) != cr)
       {
       if( ch == bsp ) {
                      if(i!=0){ 
                       if( ihori==1) xx = xx-tw-tg;
                       else yy = yy -ytwtg;
                       t_move(xx,yy);
                       t_txt_clr(0);
                       t_txt_wr(1,&bf[i-1]);
                       t_txt_clr(itxclr_o);
                       loc--;count--;
                       i--; bf[i]='\0';};
                      }
       else
         {
           bf[i] = ch;
           bf[++i]='\0';
           t_move(xx,yy);
           t_txt_wr(1,&bf[i-1]);
           if (ihori == 1) xx = xx+tw+tg;
           else yy = yy+ytwtg;
           count++;*(loc++)=12;
          }
      }
    }
     return((char *)&bf[0]);
  }
void vu_port(int x1,int y1,int x2,int y2)
   {
   float t;
   vu_x1=x1,vu_y1=y1,vu_x2=x2,vu_y2=y2;
   v_x1=x1*(float)EVGAX/639.0;
   v_y1= y1*(float)EVGAY/479.0;
   v_y1= (int) (v_y1);
   v_x2=x2*(float)EVGAX/639.0;
   v_y2= y2*(float)EVGAY/479.0;
   v_y2=(int) ( v_y2);
   if( v_y2 <= 0) {setnormal();exit(0);}
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
   clear_clip();
   set_sup_clip_limits(v_x1,v_y1,v_x2,v_y2);
   set_clip_limits(v_x1,v_y1,v_x2,v_y2,v_xt1,v_yt1);
}
void t_window(float x1,float y1,float x2,float y2)
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
void t_prtxt(int dw)
  {
  pr_txt=dw;
  }
void t_lnwidth(int styl)
  {
  ln_width=styl;
  }
void t_txtbold(int dw)
  {
  txt_bold=dw;
  txt_bold_o=dw;
  }
void t_txtfill(int dw)
  {
  t_fill=dw;
  }
void t_txtpattern(int dw)
  {
  t_pattern=dw;
  }
void t_txtbkgr(int dw)
  {
  t_bkgr=dw;
  }
void t_txtbodr(int dw)
  {
  t_bodr=dw;
  }
void t_move(float x,float y)
  {
  int x1,y1;
  x1=scr_x(x);
  y1=scr_y(y);
  _move(x1,y1);
  }
 void t_draw(float ix,float iy)
  {
  int x1,y1;
  x1=scr_x(ix);
  y1=scr_y(iy);
  _draw(x1,y1);
  }
void t_resetstyle(void)
 {
  int i;
  for(i=0;i<10;i++) st_ptr[i]=0;
 }
void t_ln_style(int istl)
  {
  ln_style=istl;
  }
void t_ln_clr(int color)
   {
    c_color=color;
    set_clr(c_color);
   }
void t_txt_clr(int color)
      {
       t_color= color;
      }
void t_txt_size(float ht,float wt,float sp,float htx,float wty,float spy)
  {
  txt_hty=ht;
  txt_wtx=wt;
  txt_spx=sp;
/*
  txt_htx=ht/(w_y2-w_y1)*(w_x2-w_x1);
  txt_wty=wt/(w_x2-w_x1)*(w_y2-w_y1);
  txt_spy=sp/(w_x2-w_x1)*(w_y2-w_y1);
*/
  txt_htx=htx;
  txt_wty=wty;
  txt_spy=spy;
  txt_wt= sint*sint*txt_wty*(w_x2-w_x1)/(w_y2-w_y1)+cost*cost*txt_wtx;
  txt_sp= sint*sint*txt_spy*(w_x2-w_x1)/(w_y2-w_y1)+cost*cost*txt_spx;
  txt_ht= sint*sint*txt_htx/(w_x2-w_x1)*(w_y2-w_y1)+cost*cost*txt_hty;
  txt_ht=txt_ht*V_y/(w_y2-w_y1);
  txt_wt=txt_wt*V_x/(w_x2-w_x1);
  txt_sp=txt_sp*V_x/(w_x2-w_x1);
  v_xt1=(int)(v_x1-txt_wt-txt_sp);
  v_yt1=(int)(v_y1-txt_ht);
  txt_w42 = txt_wt/3000.0;
  txt_h42 = txt_ht/3000.0;
  return;
  }

void t_txt_slant(float ang)
{
 Slant=tan((double)ang*3.14159265/180.0);
 Slant_o=Slant;
}
void t_txt_rot(float t)
{
  theta=-t*rad;
  cost = cos(theta);
  sint = sin(theta);
  txt_wt= sint*sint*txt_wty*(w_x2-w_x1)/(w_y2-w_y1)+cost*cost*txt_wtx;
  txt_sp= sint*sint*txt_spy*(w_x2-w_x1)/(w_y2-w_y1)+cost*cost*txt_spx;
  txt_ht= sint*sint*txt_htx/(w_x2-w_x1)*(w_y2-w_y1)+cost*cost*txt_hty;
  txt_ht=txt_ht*V_y/(w_y2-w_y1);
  txt_wt=txt_wt*V_x/(w_x2-w_x1);
  txt_sp=txt_sp*V_x/(w_x2-w_x1);
  v_xt1=(int)(v_x1-txt_wt-txt_sp);
  v_yt1=(int)(v_y1-txt_ht);
  txt_w42 = txt_wt/3000.0;
  txt_h42 = txt_ht/3000.0;
}
  void t_txt_wr(int n,char *txt)
  {
  short i=0,bold,tempc,ishft,trot,Nu,De,gap,lnwidth_o;
  int font_o;
  float fact,val,xl1,xl2,hfact=1.0,slant;
  L_N *FO_L=NULL,*pt=NULL;
  B_K *FB_P=NULL;
  char ch,cntl;
  unsigned char *tx;
  tx = (unsigned char *) txt;
  O_L=NULL;
  bold = txt_bold;
  slant=Slant;
  font_o=t_font;
  trot = (cost<0.99);
  tempc=c_color;
  c_color = t_color;
  set_clr(c_color);
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
                            ch=txt[i+1];txt[i+1]='\0';
                            gap=strlngth(txt,&xl2);
                            txt[i+1]=ch;
                            xp = (gap+1)*(txt_wt+txt_sp);
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
                            break;
                   case 'f':
                            Nu= (txt[i+1] -'0')*10+(txt[i+2]-'0');
                            t_txt_font((int)Nu);
                            i+=2;
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
      ln_width=2;
      c_color= tempc;
      set_clr(c_color);
      pt=FO_L;
      while (pt!=NULL){
       if(pt->x2 <0. ) pt->x2=xp;
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
      O_P=FB_P;
      while(O_P != NULL) {
       D_P=O_P;
       O_P=O_P->Pr;
       free(D_P);
      }
      ln_width=lnwidth_o;
      if(t_font!=font_o) t_txt_font(font_o);
  }
void t_txtb_wr(int n,char *c)
  {
  }

void t_normal(void)
  {
  if(GR_ON)XCloseDisplay(Dsp);
  GR_ON=0;
  return;
  }

char gincur(float *xx,float *yy)
  {
  char ch,key;
  int xpo,ypo,but,pointer;
  gcur_x = scr_x(*xx);
  gcur_y = EVGAY-scr_y(*yy);
  gbell();
  XDefineCursor(Dsp,Win,Cur);
  Scrn_back(msg_x,msg_y,34);
  while( GetPointer(&xpo,&ypo)<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to THIS WINDOW........"); 
  draw_cursor();
  for(;;)
  {
  while( (pointer=GetPointer(&xpo,&ypo))<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to Graphic Window........"); 
  while((pointer==0))
    {
      if((xpo!=gcur_x)||(ypo!=gcur_y))
        {gcur_x =xpo,gcur_y=ypo;
         draw_po_cursor();
         while( (pointer=GetPointer(&xpo,&ypo))<0)
          msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to Graphic Window........"); 
        }
      else pointer=-1;
    }
  if(pointer==1){gcur_x=xpo,gcur_y=ypo, key='\r';};
  if(pointer==3){gcur_x=xpo,gcur_y=ypo, key='.';};
  if(pointer>0) break;
/*
*/
    kb.kbint = get_kb(0);
    ch=kb.kbc[0];
    key=ch;
    while ((ch!='\r')&&(kb.kbint>=0)&&(ch!='\n'))
    {
      if (ch!='\0')
      {
       if ((isdigit(ch))&&(ch!='0')) jmp=ch-'0';
       else
        if((ch=='.')||(ch=='u')||(ch=='U'))
        {
        *xx = usr_x(gcur_x);
        *yy = usr_y((EVGAY-gcur_y));
        Scrn_recover();
        XDefineCursor(Dsp,Win,Curo);
        XSync(Dsp,False);
        return(key);
        };
      }
     else
     {
      ch=kb.kbc[1];
      switch(ch)
      {
      case 'H' : cursor_up();
		 break;
      case 'P' : cursor_down(); 
		 break;
      case 'K' : cursor_left();
		 break;
      case 'M' : cursor_right();
		 break;
      default  : 
		 break;
		
       }
      }
     kb.kbint = get_kb(0);
     ch=kb.kbc[0];
     key = ch;
    }
    if(ch=='\n') ch='\r';
    if(ch == '\r')break;
 }
  Scrn_recover();
  XDefineCursor(Dsp,Win,Curo);
  *xx = usr_x(gcur_x);
  *yy = usr_y((EVGAY-gcur_y));
  return(key);
  }

void  review()
   {
    XWindowAttributes xwa;
    XGetWindowAttributes(Dsp,Win,&xwa);
    EVGAX=xwa.width;
    EVGAY=xwa.height;
    msg_x = (EVGAX-316)/2;
    msg_y = EVGAY-24;
    vu_port(vu_x1,vu_y1,vu_x2,vu_y2);
    horeview();
    }
void  back_view()
   {
    }
void  rest_view()
   {
    }
void  store_view()
   {
    }
void  t_clrbuf()
   {
   }
  void t_clr_vu(void)
   {
  _box_fill(v_x1,v_y1,v_x2,v_y2,0);
   }
void bell(void)
  {
  }
void t_trans(float xs,float ys,float xm,float ym)
  {
  }
  
void t_panel(float *x,float *y,int color,int flag,int n)
  {
  int *x1,*y1;
  int xo,yo,xv,yv;
  short i,j=0;
  x1 = (int *) malloc(sizeof(int)*(n+1));
  y1 = (int *) malloc(sizeof(int)*(n+1));
  if( (y1==NULL) ){
    normal();
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
  if( (j>2)&&(j<500)) _poly_fill((short)j,x1,y1,(short)flag,(unsigned char)color);
  free(x1),free(y1);
}
void t_box_fill(float x1,float y1,float x2,float y2,int color,int ib)
  {
  int xa,ya,xb,yb;
  xa=scr_x(x1);
  ya=scr_y(y1);
  xb=scr_x(x2);
  yb=scr_y(y2);
  _box_fill(xa,ya,xb,yb,color);
  }
 void t_set_godr(float p2,float p1,int n,int ib)
 {
  pmax= p2;
  pmin= p1;
  max_clr=n;
  ibdr = ib;
  dfac = (n -1)/(pmax-pmin);
  }
 void t_godr_fill(int n,float *x,float *y,float *p)
 {
  int x1[20],y1[20];
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

void t_marker(float x,float y)
  {
  int x1,y1;
  x1=scr_x(x);
  y1= scr_y(y);
  _marker(x1,y1);
  }
void t_smmark(int istl)
  {
  m_style=istl;
  }


void t_zoom(float *x1,float *y1,float *x2,float *y2)
   {
      short it=7,item=1;
      char cmenu[49] ={
            "Zoom   Pan    StretchView   Quit   MenuoffpOinter"};
      lcur_x=v_x1;
      lcur_y=EVGAY-v_y1;
      rcur_x=v_x2;
      rcur_y=EVGAY-v_y2;
      gw_x1= usr_x(lcur_x);
      gw_x2= usr_x(rcur_x);
      gw_y1= usr_y((EVGAY-lcur_y));
      gw_y2= usr_y((EVGAY-rcur_y));
      while(item != 5)
       {
         XDefineCursor(Dsp,Win,Curo);
         item = menu(5,200,7,7,cmenu,it);
         XDefineCursor(Dsp,Win,Cur);
         switch(item)
            {
              case 1:
                      lzoom();
                      it=4;
                      break;
              case 2:
                      pan();
                      it=4;
                      break;
              case 3:
                      stretch();
                      it=4;
                      break;
              case 7:
                      pointer_zoom();
              case 4:
                     gw_x1= usr_x(lcur_x);
                     gw_x2= usr_x(rcur_x);
                     gw_y1= usr_y((EVGAY-lcur_y));
                     gw_y2= usr_y((EVGAY-rcur_y));
                     t_window(gw_x1,gw_y1,gw_x2,gw_y2);
                     review();
                     lcur_x=v_x1;
                     lcur_y=EVGAY-v_y1;
                     rcur_x=v_x2;
                     rcur_y=EVGAY-v_y2;
                     it = 5;
                     break;
              case 5:
                      it = item;
                      break;
              case 6:
                      it = item;
                      get_key_pointer(1);
                      break;
            }
       }
    *x1 = gw_x1;
    *y1 = gw_y1;
    *x2 = gw_x2;
    *y2 = gw_y2;
    XDefineCursor(Dsp,Win,Curo);
   }

void pick_box(float *x1,float *y1,float *x2,float *y2)
{
      short it=6,item=1;
      char cmenu[66] ={
            "Diagonal   move_AroundStretch    Quit       menuOff    Usepointer "};
      XSync(Dsp,False);
      gw_x1= usr_x(lcur_x);
      gw_x2= usr_x(rcur_x);
      gw_y1= usr_y((EVGAY-lcur_y));
      gw_y2= usr_y((EVGAY-rcur_y));
      while(item != 4)
       {
         XDefineCursor(Dsp,Win,Curo);
         item = menu(5,200,6,11,cmenu,it);
         XDefineCursor(Dsp,Win,Cur);
         switch(item)
            {
              case 1:
                      lzoom();
                      it=4;
                      break;
              case 2:
                      pan();
                      it=4;
                      break;
              case 3:
                      stretch();
                      it=4;
                      break;
              case 6:
                      pointer_zoom();
                      item=4;
              case 4:
                     gw_x1= usr_x(lcur_x);
                     gw_x2= usr_x(rcur_x);
                     gw_y1= usr_y((EVGAY-lcur_y));
                     gw_y2= usr_y((EVGAY-rcur_y));
                      it = item;
                      break;
              case 5:
                      it = item;
                      get_key_pointer(1);
                      break;
            }
       }
    *x1 = gw_x1;
    *y1 = gw_y1;
    *x2 = gw_x2;
    *y2 = gw_y2;
    XDefineCursor(Dsp,Win,Curo);
}

 void clr_scrn(void)
  {
 _box_fill(v_x1,v_y1,v_x2,v_y2,0x00);
  }
 void wipe_space( char *ptr,int ln)
  {
   int i;
   i=ln-1;
   *(ptr+i)='\0';i--;
   while((i>=0)&&(*(ptr+i)<=' ')) {*(ptr+i)='\0';i--;}
  }

void Scrn_back(short ix,short iy,short n)
 {
   int x2,y,y2;
   x2=ix+n*9+10;
   scr_back((int)ix,(int)(iy),x2,(int)(22+iy));
 }
void Scrn_recover()
 {
   int x2;
   scr_recover();
   XSync(Dsp,False);
   gsync();
 }
void transch(int c)
{
  int code,n,i;
  float xdum,ydum;
  short ptr;
  short j,pnt,asc,tempc;
  float fact=1.0;
  asc=c;
  pnt=asc-32;
  if( asc > 127 ) {
   fact=1.;
   m_f[pnt]= m_f0[pnt];
  }
  else {
    if(nm_f[pnt]==1.){ 
     fact=1.;
     m_f[pnt]= m_F[pnt];
    }
    else{
     fact = nm_f[pnt]/m_F[pnt];
     m_f[pnt]=nm_f[pnt];
    }
  }
  if (pnt<0) return;
  if(pnt>223) return;
  ptr= icpos[pnt];
  code= icxvF[ptr];n=icyvF[ptr];
  icxv[ptr]=code; icyv[ptr]=n;
  while ( code >= 0)
  {
     ptr++;
     for(i=0;i<n;i++){
          xdum=(icxvF[ptr]-1500.)*fact+1500;
          ydum=icyvF[ptr];
          xdum +=ydum*PS_Slant;
          icxv[ptr]= xdum+0.5;
          icyv[ptr]= icyvF[ptr];
          ptr++;
      }
     code= icxvF[ptr];n=icyvF[ptr];
     icxv[ptr]=code; icyv[ptr]=n;
   }
   return;
 }
 void modify_char_width(void)
 {
   int i,j;
   for(i=32;i<256;i++) {
    transch(i);
   }
 }
void t_txt_font(int font)
 {
  switch (font)
   {
     case 0:
          icpos = icposf0;icxv=icxvf0;icyv=icyvf0;m_f=m_f0;
          break;
     case 1:
          icpos = icposf1;icxv=icxvf1;icyv=icyvf1;m_f=m_f1;
          break;
     case 2:
          icpos = icposf2;icxv=icxvf2;icyv=icyvf2;m_f=m_f2;
          break;
     case 3:
          icpos = icposf3;icxv=icxvf3;icyv=icyvf3;m_f=m_f3;
          break;
     default:
          PS_Slant=0.;
          icpos = icposf2;
          icxvF=icxvf2,icyvF=icyvf2,m_F=m_f2;
          icxv=icxvfps;icyv=icyvfps;m_f=M_F;
          nm_f=m_fps[0];
          modify_char_width();
          break;
     case 4:
     case 12:
     case 16:
     case 24:
     case 28:
          PS_Slant=0.;
          icpos = icposf2;
          icxvF=icxvf2,icyvF=icyvf2,m_F=m_f2;
          icxv=icxvfps;icyv=icyvfps;m_f=M_F;
          nm_f=m_fps[font-4];
          modify_char_width();
          break;
     case 5:
     case 13:
     case 17:
     case 25:
     case 29:
          PS_Slant=0.;
          icpos = icposf0;
          icxvF=icxvf0,icyvF=icyvf0,m_F=m_f0;
          icxv=icxvfps;icyv=icyvfps;m_f=M_F;
          nm_f=m_fps[font-4];
          modify_char_width();
          break;
     case 6:
     case 14:
     case 18:
     case 26:
     case 30:
     case 32:
     case 33:
          PS_Slant=tan((double)3.14159265/12.0);
          icpos = icposf2;
          icxvF=icxvf2,icyvF=icyvf2,m_F=m_f2;
          icxv=icxvfps;icyv=icyvfps;m_f=M_F;
          nm_f=m_fps[font-4];
          modify_char_width();
          break;
     case 7:
     case 15:
     case 19:
     case 27:
     case 31:
          PS_Slant=tan((double)3.14159265/12.0);
          icpos = icposf0;
          icxvF=icxvf0,icyvF=icyvf0,m_F=m_f2;
          icxv=icxvfps;icyv=icyvfps;m_f=M_F;
          nm_f=m_fps[font-4];
          modify_char_width();
          break;
     case 8:
     case 20:
          PS_Slant=0.;
          icpos = icposf3;
          icxvF=icxvf3,icyvF=icyvf3,m_F=m_f3;
          icxv=icxvfps;icyv=icyvfps;m_f=M_F;
          nm_f=m_fps[font-4];
          modify_char_width();
          break;
     case 9:
     case 21:
          PS_Slant=0.;
          icpos = icposf1;
          icxvF=icxvf1,icyvF=icyvf1,m_F=m_f1;
          icxv=icxvfps;icyv=icyvfps;m_f=M_F;
          nm_f=m_fps[font-4];
          modify_char_width();
          break;
     case 10:
     case 22:
          PS_Slant=tan((double)3.14159265/12.0);
          icpos = icposf3;
          icxvF=icxvf3,icyvF=icyvf3,m_F=m_f3;
          icxv=icxvfps;icyv=icyvfps;m_f=M_F;
          nm_f=m_fps[font-4];
          modify_char_width();
          break;
     case 11:
     case 23:
          PS_Slant=tan((double)3.14159265/12.0);
          icpos = icposf1;
          icxvF=icxvf1,icyvF=icyvf1,m_F=m_f1;
          icxv=icxvfps;icyv=icyvfps;m_f=M_F;
          nm_f=m_fps[font-4];
          modify_char_width();
          break;
   }
  t_font=font;
 }
void t_circle(float x,float y,float r)
 {
  int xa,ya,rd;
  xa=scr_x(x);
  ya=scr_y(y);
/*  rd=(int)(r/r_d*v_r);*/
  rd = scr_x((x+r));
  rd -=xa;
  _circle(xa,ya,rd);
 } 
  int  strlngth(char *title,float *xdsp)
    {
      float wd,gp,fj,fjl,gj,val,fact,fact1=1.0,hfact=1.0;
      short ngp,n,i,j,k,greek=0;
      int font_o;
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
                         val = title[i+1]-'0';
                         val =  val*10+( title[i+2]-'0');
                         i+=2;
                         t_txt_font((int)val);
                         /*gj = gj-2;*/
                         break;
               case 'z':
                         val = title[i+2]-'0';
                         if(val == 0.0) val=1.0;
                         val = (float)(title[i+1]-'0')/val;
                         fact = fact*val;
                         i = i+2;
                         /*gj = gj-2;*/
                         break;
               case 'h':
                         val = title[i+2]-'0';
                         if(val == 0.0) val=1.0;
                         val = (float)(title[i+1]-'0')/val;
                         hfact = hfact*val;
                         i = i+2;
                         /*gj = gj-2;*/
                         break;
               case 'w':
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
      O_P=FB_P;
      while(O_P != NULL) {
       D_P=O_P;
       O_P=O_P->Pr;
       free(D_P);
      }
      if(t_font!=font_o)t_txt_font(font_o);
      return(ngp);
  }
  void h_intr(int *n,char *ch,float *xx,float *yy)
  {
   char *bf;
   int i;
   bf= h_int(xx,yy);
   i=0;
   while( *(bf+i) != '\0'){ *(ch+i)=*(bf+i),i++;}
   *(ch+i) = '\0';
   *n =i++;
  }
void user_frame(float xl,float yl,float xu,float yu)
 {
 }
void graphics(void)
 {
  GR_ON=1;
  set_graphics(2);
  t_initialise();
  }
void clr_menu(int ix,int iy)

  {
    return;
  }
char getch(void)
  {
   int ierr;
   char ch;
   union kbinp { short kbint; char kbc[2];} kb;
   while((kb.kbint=get_kb(1))<=0);
   ch=kb.kbc[0];
   return(ch);
  }
void  t_sup_clip_limit(float x1,float y1,float x2,float y2)
  {
    int ix1,ix2,iy1,iy2;
    ix1=scr_x(x1);
    ix2=scr_x(x2);
    iy1=scr_y(y1);
    iy2=scr_y(y2);
    if(ix1>=ix2) {
                  setnormal();
                  printf(" Wrong Clip limits\n");
                  exit(0);
                 }
    if(iy1>=iy2) {
                  setnormal();
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
void  t_clip_limit(float x1,float y1,float x2,float y2)
  {
    int ix1,ix2,iy1,iy2;
    CLIP *temp;
    temp = (CLIP *) malloc(sizeof(CLIP));
    if( temp ==NULL) {
                      setnormal();
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
                  setnormal();
                  printf(" Wrong Clip limits\n");
                  exit(0);
                 }
    if(iy1>=iy2) {
                  setnormal();
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
void clear_clip(void)
  {
    CLIP *temp;
    while (c_loc !=NULL)
       {
        temp=(CLIP *)c_loc->pt;
        free( c_loc);
        c_loc=(CLIP *)temp;
       }
   }
void t_pop_clip(void)
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
 void Get_Attn(void)
  {
   int xpo,ypo;
   Scrn_back(msg_x,msg_y,34);
   while( GetPointer(&xpo,&ypo)<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to THIS WINDOW........"); 
   Scrn_recover();
 }
int rect_pointer(int x0,int y0,int *xx,int *yy,int *entry)
{
  static int key,x1,y1;
  static int x2,y2;
  if((key=GetPointer(&x1,&y1))<0) return(-1);
  if((key==0)&&((x2!=x1)||(y2!=y1)))
       {
        if(*entry)
          {
           scr_recover();
           scr_recover();
           scr_recover();
           scr_recover();
          };
        *xx=x1,*yy=y1;
        *entry=1;
        if(x0<x1)xmin=x0,xmax=x1;
        else     xmin=x1,xmax=x0;
        if(y0<y1)ymin=y0,ymax=y1;
        else     ymin=y1,ymax=y0;
        scr_back(xmin,ymin,xmax,ymin+1);
        scr_back(xmin,ymax-1,xmax,ymax);
        scr_back(xmin,ymin,xmin+1,ymax);
        scr_back(xmax-1,ymin,xmax,ymax);
        _draw_bound(xmin,EVGAY-ymin,xmax,EVGAY-ymax);
        x2=x1,y2=y1;
       }
   if(key==0) return(key);
   *xx=x1,*yy=y1;
   while(GetPointer(&x1,&y1)>0);
   return(key);
}
int box_pointer(int x0,int y0,int *xx,int *yy,int *entry)
{
  static int key,x1,y1;
  static int x2,y2;
  if((key=GetPointer(&x1,&y1))<0) return(-1);
  if((key==0)&&((x2!=x1)||(y2!=y1)))
       {
        if(*entry)
          {
           scr_recover();
           scr_recover();
           scr_recover();
           scr_recover();
          };
        *xx=x1,*yy=y1;
        *entry=1;
        xmin=x1,xmax=x1+x0;
        ymin=y1;ymax=y1-y0;
        scr_back(xmin,ymin,xmax,ymin+1);
        scr_back(xmin,ymax-1,xmax,ymax);
        scr_back(xmin,ymin,xmin+1,ymax);
        scr_back(xmax-1,ymin,xmax,ymax);
        _draw_bound(xmin,EVGAY-ymin,xmax,EVGAY-ymax);
        x2=x1,y2=y1;
       }
   if(key==0) return(key);
   *xx=x1,*yy=y1;
   while(GetPointer(&x1,&y1)>0);
   return(key);
}
char box_gincur(float *xx,float *yy,float *xbgn,float *ybgn)
  {
  int ch,key;
  int xpo,ypo,but,pointer,xorg,yorg,xorg1,yorg1;
  int entry;
  int temp;
  temp=c_bound;
  c_bound =14;
  entry=0;
  xorg1 = scr_x(*xx);
  yorg1 = EVGAY-scr_y(*yy);
  xorg= scr_x(*xbgn);
  yorg= EVGAY-scr_y(*ybgn);
  gcur_x=xorg1;
  gcur_y=yorg1;
  xorg -=xorg1;
  yorg = yorg1-yorg;
  Scrn_back(msg_x,msg_y,34);
  xpo=gcur_x,ypo=gcur_y;
  while( GetPointer(&xpo,&ypo)<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to THIS WINDOW........"); 
  while( GetPointer(&xpo,&ypo)>0);
  XDefineCursor(Dsp,Win,Cur);
  draw_box_cursor();
  gbell();
  for(;;)
  {
  while( (pointer=box_pointer(xorg,yorg,&xpo,&ypo,&entry))<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to Graphic Window........"); 
  while((pointer==0))
    {
      if((xpo!=gcur_x)||(ypo!=gcur_y))
        {
         gcur_x =xpo,gcur_y=ypo;
         draw_box_cursor();
         while( (pointer=box_pointer(xorg,yorg,&xpo,&ypo,&entry))<0)
          msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to Graphic Window........"); 
        }
      else pointer=-1;
    }
  if(pointer==1){gcur_x=xpo,gcur_y=ypo, key='\r';WaitRelease();goto jump;};
  if(pointer==3){gcur_x=xpo,gcur_y=ypo, key='.';WaitRelease();goto jump;};


    kb.kbint = get_kb(0);
    ch=kb.kbc[0];
    key=ch;
    while ((ch!='\r')&&(kb.kbint>=0)&&(ch!='\n'))
    {
      if (ch!='\0')
      {
       if ((isdigit(ch))&&(ch!='0')) jmp=ch-'0';
       else
         goto jump;
      }
     else
     {
      ch=kb.kbc[1];
      switch(ch)
      {
      case 'H' : 
                 gcur_y-=jmp;
                 draw_box_cursor();
		 break;
      case 'P' :
                 gcur_y+=jmp;
                 draw_box_cursor();
		 break;
      case 'K' :
                 gcur_x-=jmp;
                 draw_box_cursor();
		 break;
      case 'M' :
                 gcur_x+=jmp;
                 draw_box_cursor();
		 break;
      default  : 
		 break;
		
       }
      }
     kb.kbint = get_kb(0);
     ch=kb.kbc[0];
     key = ch;
    }
    if(ch=='\n') ch='\r';
    if(ch == '\r')break;
 }
jump:
  c_bound=temp;
  set_clr(c_color);
  if(entry)
          {
           scr_recover();
           scr_recover();
           scr_recover();
           scr_recover();
          };
  Scrn_recover();
  XDefineCursor(Dsp,Win,Curo);
  *xx = usr_x(gcur_x);
  *yy = usr_y((EVGAY-gcur_y));
  return(key);
  }
char rect_gincur(float *xx,float *yy,float *xbgn,float *ybgn)
  {
  int ch,key;
  int xpo,ypo,but,pointer,xorg,yorg;
  int entry;
  int temp;
  temp=c_color;
  c_color=4;
  set_clr(c_color);
  entry=0;
  gcur_x = scr_x(*xx);
  gcur_y = EVGAY-scr_y(*yy);
  xorg= scr_x(*xbgn);
  yorg= EVGAY-scr_y(*ybgn);
  Scrn_back(msg_x,msg_y,34);
  xpo=xorg,ypo=yorg;
  while( GetPointer(&xpo,&ypo)<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to THIS WINDOW........"); 
  while( GetPointer(&xpo,&ypo)>0);
  gbell();
  XDefineCursor(Dsp,Win,Cur);
  draw_cursor();
  for(;;)
  {
  while( (pointer=rect_pointer(xorg,yorg,&xpo,&ypo,&entry))<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to Graphic Window........"); 
  while((pointer==0))
    {
      if((xpo!=gcur_x)||(ypo!=gcur_y))
        {gcur_x =xpo,gcur_y=ypo;
         draw_po_cursor();
         while( (pointer=rect_pointer(xorg,yorg,&xpo,&ypo,&entry))<0)
          msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to Graphic Window........"); 
        }
      else pointer=-1;
    }
  if(pointer==1){gcur_x=xpo,gcur_y=ypo, key='\r';goto jump;};
  if(pointer==3){gcur_x=xpo,gcur_y=ypo, key='.';goto jump;};


    kb.kbint = get_kb(0);
    ch=kb.kbc[0];
    key=ch;
    while ((ch!='\r')&&(kb.kbint>=0)&&(ch!='\n'))
    {
      if (ch!='\0')
      {
       if ((isdigit(ch))&&(ch!='0')) jmp=ch-'0';
       else
        if((ch=='.')||(ch=='u')||(ch=='U')) goto jump;
      }
     else
     {
      ch=kb.kbc[1];
      switch(ch)
      {
      case 'H' : cursor_up();
		 break;
      case 'P' : cursor_down(); 
		 break;
      case 'K' : cursor_left();
		 break;
      case 'M' : cursor_right();
		 break;
      default  : 
		 break;
		
       }
      }
     kb.kbint = get_kb(0);
     ch=kb.kbc[0];
     key = ch;
    }
    if(ch=='\n') ch='\r';
    if(ch == '\r')break;
 }
jump:
  c_color=temp;
  set_clr(temp);
  if(entry)
          {
           scr_recover();
           scr_recover();
           scr_recover();
           scr_recover();
          };
  Scrn_recover();
  XDefineCursor(Dsp,Win,Curo);
  *xx = usr_x(gcur_x);
  *yy = usr_y((EVGAY-gcur_y));
  return(key);
  }
int cross_pointer(int *xx,int *yy,int *entry)
{
  static int key,x1,y1;
  static int x2,y2;
  if(*entry == 0) {x2=-1;y2=-1;};
  if((key=GetPointer(&x1,&y1))<0) return(-1);
  if((key==0)&&((x2!=x1)||(y2!=y1)))
       {
        if(*entry)
          {
           scr_recover();
           scr_recover();
          };
        *xx=x1,*yy=y1;
        *entry=1;
        xmin=x1;xmax=x1+1;
        if( xmin==0) xmax=1;
        if( xmin==EVGAX) {xmax=x1;xmin=xmax-1;};
        ymin=y1;ymax=y1+1;
        if( ymin==0) ymax=1;
        if( ymin==EVGAY) {ymax=y1;ymin=ymax-1;};
        scr_back(xmin,0,xmax,EVGAY);
        scr_back(0,ymin,EVGAX,ymax);
        XDRAW(0,y1,EVGAX,y1);
        XDRAW(x1,0,x1,EVGAY);
        x2=x1,y2=y1;
       }
   if(key==0) return(key);
   *xx=x1,*yy=y1;
   while(GetPointer(&x1,&y1)>0);
   return(key);
}
char cross_gincur(float *xx,float *yy)
  {
  int ch,key;
  int xpo,ypo,but,pointer,xorg,yorg;
  int entry,temp;
  temp=c_color;
  c_color=14;
  set_clr(c_color);
  entry=0;
  gcur_x = scr_x(*xx);
  gcur_y = EVGAY-scr_y(*yy);
  Scrn_back(msg_x,msg_y,34);
  xpo=xorg,ypo=yorg;
  while( GetPointer(&xpo,&ypo)<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to THIS WINDOW........"); 
  while( GetPointer(&xpo,&ypo)>0);
  gbell();
  XDefineCursor(Dsp,Win,Cur);
  draw_cursor();
  for(;;)
  {
  while( (pointer=cross_pointer(&xpo,&ypo,&entry))<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to Graphic Window........"); 
  while((pointer==0))
    {
      if((xpo!=gcur_x)||(ypo!=gcur_y))
        {gcur_x =xpo,gcur_y=ypo;
         draw_po_cursor();
         while( (pointer=cross_pointer(&xpo,&ypo,&entry))<0)
          msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to Graphic Window........"); 
        }
      else pointer=-1;
    }
  if(pointer==1){gcur_x=xpo,gcur_y=ypo, key='\r';goto jump;};
  if(pointer==3){gcur_x=xpo,gcur_y=ypo, key='.';goto jump;};


    kb.kbint = get_kb(0);
    ch=kb.kbc[0];
    key=ch;
    while ((ch!='\r')&&(kb.kbint>=0)&&(ch!='\n'))
    {
      if (ch!='\0')
      {
       if ((isdigit(ch))&&(ch!='0')) jmp=ch-'0';
       else
        if((ch=='.')||(ch=='u')||(ch=='U')) goto jump;
      }
     else
     {
      ch=kb.kbc[1];
      switch(ch)
      {
      case 'H' : cursor_up();
		 break;
      case 'P' : cursor_down(); 
		 break;
      case 'K' : cursor_left();
		 break;
      case 'M' : cursor_right();
		 break;
      default  : 
		 break;
		
       }
      }
     kb.kbint = get_kb(0);
     ch=kb.kbc[0];
     key = ch;
    }
    if(ch=='\n') ch='\r';
    if(ch == '\r')break;
 }
jump:
  c_color=temp;
  set_clr(c_color);
  if(entry)
          {
           scr_recover();
           scr_recover();
          };
  Scrn_recover();
  XDefineCursor(Dsp,Win,Curo);
  *xx = usr_x(gcur_x);
  *yy = usr_y((EVGAY-gcur_y));
  return(key);
  }
char dbl_gincur(float *xx,float *yy,float *xbgn,float *ybgn)
  {
  int ch,key;
  int xpo,ypo,but,pointer,xorg,yorg;
  int entry;
  int temp;
  temp=c_color;
  c_color=9;
  set_clr(c_color);
  entry=0;
  gcur_x = scr_x(*xx);
  gcur_y = EVGAY-scr_y(*yy);
  xorg= scr_x(*xbgn);
  yorg= EVGAY-scr_y(*ybgn);
  Scrn_back(msg_x,msg_y,34);
  scr_back(0,yorg,EVGAX,yorg);
  scr_back(xorg,0,xorg,EVGAY);
  XDRAW(0,yorg,EVGAX,yorg);
  XDRAW(xorg,0,xorg,EVGAY);
  c_color=14;
  set_clr(c_color);
  xpo=xorg,ypo=yorg;
  while( GetPointer(&xpo,&ypo)<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to THIS WINDOW........"); 
  while( GetPointer(&xpo,&ypo)>0);
  gbell();
  XDefineCursor(Dsp,Win,Cur);
  draw_cursor();
  for(;;)
  {
  while( (pointer=cross_pointer(&xpo,&ypo,&entry))<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to Graphic Window........"); 
  while((pointer==0))
    {
      if((xpo!=gcur_x)||(ypo!=gcur_y))
        {gcur_x =xpo,gcur_y=ypo;
         draw_po_cursor();
         while( (pointer=cross_pointer(&xpo,&ypo,&entry))<0)
          msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to Graphic Window........"); 
        }
      else pointer=-1;
    }
  if(pointer==1){gcur_x=xpo,gcur_y=ypo, key='\r';goto jump;};
  if(pointer==3){gcur_x=xpo,gcur_y=ypo, key='.';goto jump;};


    kb.kbint = get_kb(0);
    ch=kb.kbc[0];
    key=ch;
    while ((ch!='\r')&&(kb.kbint>=0)&&(ch!='\n'))
    {
      if (ch!='\0')
      {
       if ((isdigit(ch))&&(ch!='0')) jmp=ch-'0';
       else
        if((ch=='.')||(ch=='u')||(ch=='U')) goto jump;
      }
     else
     {
      ch=kb.kbc[1];
      switch(ch)
      {
      case 'H' : cursor_up();
		 break;
      case 'P' : cursor_down(); 
		 break;
      case 'K' : cursor_left();
		 break;
      case 'M' : cursor_right();
		 break;
      default  : 
		 break;
		
       }
      }
     kb.kbint = get_kb(0);
     ch=kb.kbc[0];
     key = ch;
    }
    if(ch=='\n') ch='\r';
    if(ch == '\r')break;
 }
jump:
  c_color=temp;
  set_clr(c_color);
  if(entry)
          {
           scr_recover();
           scr_recover();
          };
  scr_recover();
  scr_recover();
  Scrn_recover();
  XDefineCursor(Dsp,Win,Curo);
  *xx = usr_x(gcur_x);
  *yy = usr_y((EVGAY-gcur_y));
  return(key);
  }
/*
void backup_line(int x0,int y0,int x, int y)
{
  int xa, ya, xb, yb,ptn;
  short  dx, dy,  f, inc1, inc2, g,dx1,dy1,ab;
  short clip,i,ch,rh,c,r;
  float h;

  xa=x0;
  ya=y0;
  xb=x;
  yb=y;
  {
   clip=clip_line(&xa,&ya,&xb,&yb);
   if (clip == 0)
   {
	return;
   }
   gsync();
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
   i=0;
   while (abs(c-f)>0)
   { 
      if ( dx1 > dy1 ){
       CopyPoint(c,r,i++);
      }
      else {
       CopyPoint(r,c,i++);
      }
      c+=ch;
      if (g > 0) { r+=rh; g+=inc2; }
      else g+=inc1;
    }
 }
   gsync();
   return;
}
void recover_line(int x0,int y0,int x, int y)
{
  int xa, ya, xb, yb,ptn;
  short  dx, dy,  f, inc1, inc2, g,dx1,dy1,ab;
  short clip,i,ch,rh,c,r;
  float h;

  xa=x0;
  ya=y0;
  xb=x;
  yb=y;
  {
   clip=clip_line(&xa,&ya,&xb,&yb);
   if (clip == 0)
   {
	return;
   }
   gsync();
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
   i=0;
   while (abs(c-f)>0)
   { 
      if ( dx1 > dy1 ){
       GetPoint(c,r,i++);
      }
      else {
       GetPoint(r,c,i++);
      }
      c+=ch;
      if (g > 0) { r+=rh; g+=inc2; }
      else g+=inc1;
    }
 }
   gsync();
   return;
 }
*/
 copy_area(int x1,int y1,int x2,int y2,int i,int nx)
 {
  int xmin,ymin,xmax,ymax;
  if(x1<x2) {xmin=x1,xmax=x2;}
  else {xmin =x2;xmax =x1;}
  if(y1<y2) {ymin=y1,ymax=y2;}
  else {ymin =y2;ymax =y1;}
  XCopyArea(Dsp,Win,linebuf,Gc,(short)xmin,(short)(ymin)
 ,(short)(xmax-xmin+1),(short)(ymax-ymin+1),i*nx,0);
 }
 void backup_line(int x1,int y1,int x2,int y2)
 {
  int i,dx,dy,nb,nx,ny,length,width,xl,yl,xu,yu,nxp;
  float dxx,dyy;
  dx = x2-x1;
  dy = y2-y1;
  if ( abs(dx) > abs(dy) ) { nb = abs(dy)/100+1; }
  else { nb = abs(dx)/100+1;}
  ny = dy/nb,nx=dx/nb;
  if(ny < 0)   ny -=2; else ny+=2;
  if(nx < 0)   nx -=2; else nx+=2;
  nxp = abs(nx)+1;
  length=nb*nxp;
  width = abs(ny)+1;
  linebuf = XCreatePixmap(Dsp,Win,(short)(length),(short)(width),Dpth);
  dxx = ((float)dx)/(float)nb;
  dyy = ((float)dy)/(float)nb;
  for(i=0;i<(nb-1);i++) {
   xl = x1 +i*dxx;
   yl = y1 +i*dyy;
   copy_area(xl,yl,xl+nx,yl+ny,i,nxp);
  }
   xl = x1 +(nb-1)*dxx;
   yl = y1 +(nb-1)*dyy;
   copy_area(xl,yl,x2,y2,nb-1,nxp);
   return;
  }
 rest_area(int x1,int y1,int x2,int y2,int i,int nx)
 {
  int xmin,ymin,xmax,ymax;
  if(x1<x2) {xmin=x1,xmax=x2;}
  else {xmin =x2;xmax =x1;}
  if(y1<y2) {ymin=y1,ymax=y2;}
  else {ymin =y2;ymax =y1;}
  XCopyArea(Dsp,linebuf,Win,Gc,i*nx,0,
  (short)(xmax-xmin+1),(short)(ymax-ymin+1), (short)xmin,(short)(ymin));
 }
 void recover_line(int x1,int y1,int x2,int y2)
 {
  int i,dx,dy,nb,nx,ny,length,width,xl,yl,xu,yu,nxp;
  float dxx,dyy;
  dx = x2-x1;
  dy = y2-y1;
  if ( abs(dx) > abs(dy) ) { nb = abs(dy)/100+1; }
  else { nb = abs(dx)/100+1;}
  ny = dy/nb,nx=dx/nb;
  if(ny < 0)   ny -=2; else ny+=2;
  if(nx < 0)   nx -=2; else nx+=2;
  nxp = abs(nx)+1;
  length=nb*nxp;
  width = abs(ny);
  dxx = ((float)dx)/(float)nb;
  dyy = ((float)dy)/(float)nb;
  for(i=0;i<(nb-1);i++) {
   xl = x1 +i*dxx;
   yl = y1 +i*dyy;
   rest_area(xl,yl,xl+nx,yl+ny,i,nxp);
  }
   xl = x1 +(nb-1)*dxx;
   yl = y1 +(nb-1)*dyy;
   rest_area(xl,yl,x2,y2,nb-1,nxp);
   XFreePixmap(Dsp,linebuf);
   return;
  }
int rbr_pointer(int xorg,int yorg,int *xx,int *yy,int *entry)
{
  static int key,x1,y1;
  static int x2,y2;
  if((key=GetPointer(&x1,&y1))<0) return(-1);
  if((key==0)&&((x2!=x1)||(y2!=y1)))
       {
        if(*entry)
          {
           scr_recover();
           scr_recover();
           recover_line(xorg,yorg,xrbr,yrbr);
          };
        *xx=x1,*yy=y1;
        *entry=1;
        xmin=x1;xmax=x1+1;
        if( xmin==0) xmax=1;
        if( xmin==EVGAX) {xmax=x1;xmin=xmax-1;};
        ymin=y1;ymax=y1+1;
        if( ymin==0) ymax=1;
        if( ymin==EVGAY) {ymax=y1;ymin=ymax-1;};
        backup_line(xorg,yorg,x1,y1);
        scr_back(xmin,0,xmax,EVGAY);
        scr_back(0,ymin,EVGAX,ymax);
        XDRAW(xorg,yorg,x1,y1);
        XDRAW(0,y1,EVGAX,y1);
        XDRAW(x1,0,x1,EVGAY);
        x2=x1,y2=y1;
        xrbr=x1,yrbr=y1;
       }
   if(key==0) return(key);
   *xx=x1,*yy=y1;
   while(GetPointer(&x1,&y1)>0);
   return(key);
}
char rbr_gincur(float *xx,float *yy,float *xbgn,float *ybgn)
  {
  int ch,key;
  int xpo,ypo,but,pointer,xorg,yorg;
  int entry;
  int temp;
  temp=c_color;
  c_color=9;
  set_clr(c_color);
  entry=0;
  gcur_x = scr_x(*xx);
  gcur_y = EVGAY-scr_y(*yy);
  xorg= scr_x(*xbgn);
  yorg= EVGAY-scr_y(*ybgn);
  Scrn_back(msg_x,msg_y,34);
  scr_back(0,yorg,EVGAX,yorg);
  scr_back(xorg,0,xorg,EVGAY);
  XDRAW(0,yorg,EVGAX,yorg);
  XDRAW(xorg,0,xorg,EVGAY);
  c_color=14;
  set_clr(c_color);
  xpo=xorg,ypo=yorg;
  while( GetPointer(&xpo,&ypo)<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to THIS WINDOW........"); 
  while( GetPointer(&xpo,&ypo)>0);
  gbell();
  XDefineCursor(Dsp,Win,Cur);
  draw_cursor();
  for(;;)
  {
  while( (pointer=rbr_pointer(xorg,yorg,&xpo,&ypo,&entry))<0)
    msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to Graphic Window........"); 
  while((pointer==0))
    {
      if((xpo!=gcur_x)||(ypo!=gcur_y))
        {gcur_x =xpo,gcur_y=ypo;
         draw_po_cursor();
         while( (pointer=rbr_pointer(xorg,yorg,&xpo,&ypo,&entry))<0)
          msg_menu(msg_x,msg_y,34,(char *)"Bring Pointer to Graphic Window........"); 
        }
      else pointer=-1;
    }
  if(pointer==1){gcur_x=xpo,gcur_y=ypo, key='\r';goto jump;};
  if(pointer==3){gcur_x=xpo,gcur_y=ypo, key='.';goto jump;};


    kb.kbint = get_kb(0);
    ch=kb.kbc[0];
    key=ch;
    while ((ch!='\r')&&(kb.kbint>=0)&&(ch!='\n'))
    {
      if (ch!='\0')
      {
       if ((isdigit(ch))&&(ch!='0')) jmp=ch-'0';
       else
        if((ch=='.')||(ch=='u')||(ch=='U')) goto jump;
      }
     else
     {
      ch=kb.kbc[1];
      switch(ch)
      {
      case 'H' : cursor_up();
		 break;
      case 'P' : cursor_down(); 
		 break;
      case 'K' : cursor_left();
		 break;
      case 'M' : cursor_right();
		 break;
      default  : 
		 break;
		
       }
      }
     kb.kbint = get_kb(0);
     ch=kb.kbc[0];
     key = ch;
    }
    if(ch=='\n') ch='\r';
    if(ch == '\r')break;
 }
jump:
  c_color=temp;
  set_clr(c_color);
  if(entry)
          {
           scr_recover();
           scr_recover();
           recover_line(xorg,yorg,xrbr,yrbr);
          };
  scr_recover();
  scr_recover();
  Scrn_recover();
  XDefineCursor(Dsp,Win,Curo);
  *xx = usr_x(gcur_x);
  *yy = usr_y((EVGAY-gcur_y));
  return(key);
  }

/* NEW CODE */

void draw_arc_cursor()
{
  unsigned char tempc;
  char nbuf[35];
  tempc=c_color;
  c_color=0x0F;
  sprintf(nbuf,"Angle = %6.2f",gcur_t);
  nbuf[34]='\0';
  msg_menu(msg_x,msg_y,34L,nbuf);
  c_color=tempc;
  set_clr(c_color);
}
int limit_cur(int *x1,int *y1,int xorg,int yorg)
{
  static double t=0;
  t= atan2((double)(*y1-yorg),(double)(*x1-xorg));
  *x1 =xorg+gcur_r*cos(t);
  *y1 =yorg+gcur_r*sin(t);
/*  if((*x1<0)||(*x1>EVGAX)||(*y1<0)||(*y1>EVGAY)) return -1;*/
  gcur_t= 180*t/3.14159265;
  if(gcur_t<0.) gcur_t=360+gcur_t;
  gcur_t=360-gcur_t;
  return 1;
}
int arc_pointer(int xorg,int yorg,int *xx,int *yy,int *entry)
{
  static int key,x1,y1;
  static int x2,y2;
  if((key=GetPointer(&x1,&y1))<0) return(-1);
  if(limit_cur(&x1,&y1,xorg,yorg)<0 ) return -1;
  if((key==0)&&((x2!=x1)||(y2!=y1))) {
        if(*entry) {
           scr_recover();
           scr_recover();
           recover_line(xorg,yorg,xrbr,(yrbr));
        };
        *xx=x1,*yy=y1;
        *entry=1;
        xmin=x1;xmax=x1+1;
        if( xmin==0) xmax=1;
        if( xmin==EVGAX) {xmax=x1;xmin=xmax-1;};
        ymin=y1;ymax=y1+1;
        if( ymin==0) ymax=1;
        if( ymin==EVGAY) {ymax=y1;ymin=ymax-1;};
        backup_line(xorg,yorg,x1,y1);
        scr_back(xmin,0,xmax,EVGAY);
        scr_back(0,ymin,EVGAX,ymax);
        XDRAW(xorg,yorg,x1,y1);
        XDRAW(0,y1,EVGAX,y1);
        XDRAW(x1,0,x1,EVGAY);
        x2=x1,y2=y1;
        xrbr=x1,yrbr=y1;
   }
   *xx=x1,*yy=y1;
   if(key==0) return(key);
   while(GetPointer(&x1,&y1)>0);
   return(key);
}

char arc_gincur(float *xx,float *yy,float *xbgn,float *ybgn)
  {
  int ch,key;
  int xpo,ypo,but,pointer,xorg,yorg;
  int gcurx,gcury;
  int msg;
  int entry;
  int temp;
  temp=c_color;
  c_color=9;
  set_clr(c_color);
  entry=0;
  gcur_x = scr_x(*xx);
  gcur_y = EVGAY-scr_y(*yy);
  xorg= scr_x(*xbgn);
  yorg= EVGAY-scr_y(*ybgn);
  gcur_r= sqrt((double)((gcur_x-xorg)*(gcur_x-xorg)+
                        (gcur_y-yorg)*(gcur_y-yorg)));
  Scrn_back(msg_x,msg_y,34);
  scr_back(0,yorg,EVGAX,yorg);
  scr_back(xorg,0,xorg,EVGAY);
  Cur_On=0;
  XDRAW(0,yorg,EVGAX,yorg);
  XDRAW(xorg,0,xorg,EVGAY);
  c_color=14;
  set_clr(c_color);
  xpo=gcur_x,ypo=gcur_y;
  while( GetPointer(&xpo,&ypo)<0)
    msg_menu(msg_x,msg_y,34L,(char *)"Bring Pointer to THIS WINDOW........"); 
  while( GetPointer(&xpo,&ypo)>0);
  XDefineCursor(Dsp,Win,Cur);
  warp_pointer(gcur_x,gcur_y);
  draw_arc_cursor();
  gbell();
  for(;;)
  {
  while( (pointer=arc_pointer(xorg,yorg,&xpo,&ypo,&entry))<0)
    msg_menu(msg_x,msg_y,34L,(char *)"Bring Pointer to Graphic Window........"); 
  while((pointer==0))
    {
      if((xpo!=gcur_x)||(ypo!=gcur_y))
        {gcur_x =xpo,gcur_y=ypo;
         draw_arc_cursor();
         while( (pointer=arc_pointer(xorg,yorg,&xpo,&ypo,&entry))<0)
          msg_menu(msg_x,msg_y,34L,(char *)"Bring Pointer to Graphic Window........"); 
        }
      else pointer=-1;
    }
  if(pointer==1){gcur_x=xpo,gcur_y=ypo, key='\r';WaitRelease();goto jump;};
  if(pointer==3){gcur_x=xpo,gcur_y=ypo, key='.';WaitRelease();goto jump;};


    kb.kbint = get_kb(0);
    ch=kb.kbc[0];
    key=ch;
    while ((ch!='\r')&&(kb.kbint>=0)&&(ch!='\n'))
    {
      if (ch!='\0')
      {
       if ((isdigit(ch))&&(ch!='0')) jmp=ch-'0';
       else
         goto jump;
      }
     else
     {
      ch=kb.kbc[1];
      switch(ch)
      {
      case 'H' :
                 gcurx=gcur_x;gcury=gcur_y-jmp;
                 if(limit_cur(&gcurx,&gcury,xorg,yorg)<0) break;
                 gcur_x=gcurx,gcur_y=gcury;
                 warp_pointer(gcur_x,gcur_y);
                 draw_arc_cursor();
		 break;
      case 'P' :
                 gcurx=gcur_x;gcury=gcur_y+jmp;
                 if(limit_cur(&gcurx,&gcury,xorg,yorg)<0) break;
                 gcur_x=gcurx,gcur_y=gcury;
                 warp_pointer(gcur_x,gcur_y);
                 draw_arc_cursor();
		 break;
      case 'K' : 
                 gcurx=gcur_x-jmp;gcury=gcur_y;
                 if(limit_cur(&gcurx,&gcury,xorg,yorg)<0) break;
                 gcur_x=gcurx,gcur_y=gcury;
                 warp_pointer(gcur_x,gcur_y);
                 draw_arc_cursor();
		 break;
      case 'M' :
                 gcurx=gcur_x+jmp;gcury=gcur_y;
                 if(limit_cur(&gcurx,&gcury,xorg,yorg)<0) break;
                 gcur_x=gcurx,gcur_y=gcury;
                 warp_pointer(gcur_x,gcur_y);
                 draw_arc_cursor();
		 break;
      default  : 
		 break;
		
       }
      }
     kb.kbint = get_kb(0);
     ch=kb.kbc[0];
     key = ch;
    }
    if(ch=='\n') ch='\r';
    if(ch == '\r')break;
 }
jump:
  c_color=temp;
  set_clr(c_color);
  if(entry)
          {
           scr_recover();
           scr_recover();
           recover_line(xorg,yorg,xrbr,yrbr);
          };
  scr_recover();
  scr_recover();
  Scrn_recover();
  XDefineCursor(Dsp,Win,Curo);
  *xx = usr_x(gcur_x);
  *yy = usr_y((EVGAY-gcur_y));
  Cur_On=1;
  return(key);
  }
void draw_dst_cursor(float xo,float yo)
{
  unsigned char tempc;
  char nbuf[35];
  float x,y,dist,t;
  tempc=c_color;
  c_color=0x0F;
  x= usr_x(gcur_x);
  y= usr_y(EVGAY-gcur_y);
  dist = sqrt((double)(xo-x)*(xo-x)+(yo-y)*(yo-y));
  t= atan2((double)(y-yo),(double)(x-xo));
  t= 180*t/3.14159265;
  if(t<0.0) t=360+t;
  sprintf(nbuf,"length=%9.2f   Ang=%6.2f",dist,t);
  nbuf[34]='\0';
  msg_menu(msg_x,msg_y,34L,nbuf);
  c_color=tempc;
}
char dst_gincur(float *xx,float *yy,float *xbgn,float *ybgn)
  {
  int ch,key;
  int xpo,ypo,but,pointer,xorg,yorg;
  int msg;
  int entry;
  int temp;
  temp=c_color;
  c_color=9;
  set_clr(c_color);
  entry=0;
  gcur_x = scr_x(*xx);
  gcur_y = EVGAY-scr_y(*yy);
  xorg= scr_x(*xbgn);
  yorg= EVGAY-scr_y(*ybgn);
  Scrn_back(150,1,34L);
  scr_back(0,yorg,EVGAX,yorg);
  scr_back(xorg,0,xorg,EVGAY);
  Cur_On=0;
  XDRAW(0,yorg,EVGAX,yorg);
  XDRAW(xorg,0,xorg,EVGAY);
  c_color=14;
  set_clr(c_color);
  xpo=xorg,ypo=yorg;
  warp_pointer(xorg,yorg);
  while( GetPointer(&xpo,&ypo)<0)
    msg_menu(msg_x,msg_y,34L,(char *)"Bring Pointer to THIS WINDOW........"); 
  while( GetPointer(&xpo,&ypo)>0);
  draw_dst_cursor(*xbgn,*ybgn);
  gbell();
  for(;;)
  {
  while( (pointer=rbr_pointer(xorg,yorg,&xpo,&ypo,&entry))<0)
    msg_menu(msg_x,msg_y,34L,(char *)"Bring Pointer to Graphic Window........"); 
  while((pointer==0))
    {
      if((xpo!=gcur_x)||(ypo!=gcur_y))
        {gcur_x =xpo,gcur_y=ypo;
         draw_dst_cursor(*xbgn,*ybgn);
         while( (pointer=rbr_pointer(xorg,yorg,&xpo,&ypo,&entry))<0)
          msg_menu(msg_x,msg_y,34L,(char *)"Bring Pointer to Graphic Window........"); 
        }
      else pointer=-1;
    }
  if(pointer==1){gcur_x=xpo,gcur_y=ypo, key='\r';WaitRelease();goto jump;};
  if(pointer==3){gcur_x=xpo,gcur_y=ypo, key='.';WaitRelease();goto jump;};


    kb.kbint = get_kb(0);
    ch=kb.kbc[0];
    key=ch;
    while ((ch!='\r')&&(kb.kbint>=0)&&(ch!='\n'))
    {
      if (ch!='\0')
      {
       if ((isdigit(ch))&&(ch!='0')) jmp=ch-'0';
       else
         goto jump;
      }
     else
     {
      ch=kb.kbc[1];
      switch(ch)
      {
      case 'H' :
                 gcur_y-=jmp;
                 warp_pointer(gcur_x,gcur_y);
                 draw_dst_cursor(*xbgn,*ybgn);
		 break;
      case 'P' :
                 gcur_y+=jmp;
                 warp_pointer(gcur_x,gcur_y);
                 draw_dst_cursor(*xbgn,*ybgn);
		 break;
      case 'K' : 
                 gcur_x-=jmp;
                 warp_pointer(gcur_x,gcur_y);
                 draw_dst_cursor(*xbgn,*ybgn);
		 break;
      case 'M' :
                 gcur_x+=jmp;
                 warp_pointer(gcur_x,gcur_y);
                 draw_dst_cursor(*xbgn,*ybgn);
		 break;
      default  : 
		 break;
		
       }
      }
     kb.kbint = get_kb(0);
     ch=kb.kbc[0];
     key = ch;
    }
    if(ch=='\n') ch='\r';
    if(ch == '\r')break;
 }
jump:
  c_color=temp;
  set_clr(c_color);
  if(entry)
          {
           scr_recover();
           scr_recover();
           recover_line(xorg,yorg,xrbr,yrbr);
          };
  scr_recover();
  scr_recover();
  Scrn_recover();
  *xx = usr_x(gcur_x);
  *yy = usr_y((EVGAY-gcur_y));
  Cur_On=1;
  return(key);
  }
void t_get_resolution(int *x,int *y)
{
 *x= EVGAX+1;
 *y= EVGAY+1;
}
void get_scan_code(void) {
  int i,j,k=0;
  KeySym *keysym;
  int K_min,K_max,code;
  XDisplayKeycodes(Dsp,&K_min,&K_max);
  keysym = XGetKeyboardMapping(Dsp,K_min,K_max-K_min+1,&code);
  for(i=K_min;i<=K_max;i++) {
    Scan_code[i]= keysym[k];
    Scan_sh_code[i]= keysym[k+1];
    k+=code;
  }
  XFree(keysym);
}
void Bkup_clip_limits(void) {
  bkup_clip_limits;
}
void Set_full_scrn(void) {
  set_full_scrn;
}
void Rest_clip_limits(void) {
  rest_clip_limits;
}
#ifdef ZBUFFER
#include "txlib3d.c"
#else
#include "txlib3ddum.c"
#endif
