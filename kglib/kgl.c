/*
  Version 2.2
  DATE 13/07/98
  With file browser and improved GUI
  with home dirctory corrections
*/
#ifdef RS6000
#define sqrtf(x) sqrt((double)x)
#define cosf(x) cos((double)x)
#define sinf(x) sin((double)x)
#define atan2f(x,y) atan2((double)x,(double)y)
#endif
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <malloc.h>
#include <sys/stat.h>
#include <pthread.h>
#include <time.h>
#include <magick/api.h>
#include "kulina.h"
#include "gprivate.h"
#include "gminterface.h"
#include "dlink.h"
#define ZRES 65000
#define write_bf(x,n) write_buf( (unsigned char *) x, n)
#define uiwrite_bf(G,x,n) uiwrite_buf(G, (unsigned char *) x, n)
#define uicnv_x(dc,x) (x*dc->Fx-dc->Sx)
#define uicnv_y(dc,y) (y*dc->Fy-dc->Sy)
#define uicnv_z(dc,z) ( ((z)-dc->clip_min)/(dc->clip_max - dc->clip_min)*ZRES )
#define Fread(pf,c,n)  fread(c,1,n,pf)
#define FREAD(pf,c,n)  { \
  int ret; ret=fread(c,1,n,pf); \
  if(ret != n) {fprintf(stderr,"fread failed \n"); exit(0);} \
}
#ifdef UNIX
#include <unistd.h>
#include <dirent.h>
#define O_BINARY 0x00
#define far 
#include <malloc.h>
#else
#include <malloc.h>
#include <dos.h>
static int SLASH=92;
static char SLASHS[2]="\\";
#endif
#define GAP 0.05
#define HFAC 1.30
#define RESIZE 5
static pthread_mutex_t _Initlock=PTHREAD_MUTEX_INITIALIZER;
void CHDIR(char *);
static int NCLRS=256;
static float m_radius=100;
static float TI[16] = {1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.};
#define  check_mem_alloc(yy)  {\
     if( yy==NULL) {\
      printf("Error: In mem. allocation...\n"); \
      exit(0); \
     }\
   }
static unsigned char esc=0x1b,G='g',N='n',C='c',P='p',Z='z',L='l',V='v';
static unsigned char S='s',T='t',R='r',M='m',D='d',Y='y',U='u';
static unsigned char O='o',B='b',X='x',F='f',I='i',A='a',E='e',W='w';
static unsigned char H='h',K='k',M3 ='M',D3='D',P3='P',G3='G',B3='B',S3='S',S4='H',T3='T';
static unsigned char BELL=0x07;
static char *dp;
static unsigned char ch[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
static int B_max=990000,B_min=989000;
#define kgTransfrm( xp, yp,  zp) {\
   dc->trnstr.xstr = (xp * dc->FM[0]) + (yp * dc->FM[4]) + (zp * dc->FM[8]) + dc->FM[12];\
   dc->trnstr.ystr = (xp * dc->FM[1]) + (yp * dc->FM[5]) + (zp * dc->FM[9]) + dc->FM[13];\
   dc->trnstr.zstr = (xp * dc->FM[2]) + (yp * dc->FM[6]) + (zp * dc->FM[10]) + dc->FM[14];\
 }
#define kgProjection( oldstr) {\
     dc->newstr.xstr = -(2.* dc->ze/(oldstr.zstr)) * oldstr.xstr;\
     dc->newstr.ystr = -(2.* dc->ze/(oldstr.zstr)) * oldstr.ystr;\
     dc->newstr.zstr = oldstr.zstr;\
 }
#define WRITE(x,y,z) { \
	int ret; \
	ret = write(x,y,z); \
	if(ret != z) {fprintf(stderr,"Write Failed\n"); exit(0);}\
}
#define SYSTEM(cmd) { \
 int ret; ret = system(cmd); \
}
int pscript(char *,char *);
void remove_back_slash(char *buff) {
 int i=0;
 while(buff[i]!='\0') {
   if(buff[i]=='\\') buff[i]='/';
   i++;
 }
 i--;
 if(buff[i]=='\\') buff[i]='\0';
}

void uiDialogWindow(DIALOG *D) {
#if 0
    if((D->Sticky==1)||(D->Fixpos==1)||(D->NoTaskBar==1)) {
      D->wc =ui_set_graphics_sticky(D->xo,D->yo,D->xl,D->yl,D->name,D->Deco,D->transparency,D->Sticky,D->Fixpos,D->NoTaskBar,D->StackPos,D->Shapexpm);
      WC(D)->D=D;
    }
    else {
      D->wc =ui_set_graphics(D->xo,D->yo,D->xl,D->yl,D->name,D->Deco,D->transparency);
      WC(D)->D=D;
    }
#endif
//    D->wc =ui_set_graphics_sticky(D->xo,D->yo,D->xl,D->yl,D->name,D->Deco,D->transparency,D->Sticky,D->Fixpos,D->NoTaskBar,D->StackPos,D->Shapexpm);
    D->wc = kgSetupGraphics(D);
    WC(D)->D=D;
    WC(D)->Clip=(void *)Dopen();
    WC(D)->GuiFont=D->gc.PromptFont;
    WC(D)->GuiFontSize=D->gc.GuiFontSize;
    WC(D)->event.type=-100;
    WC(D)->eventback.type=-100;
}

void kgDefaultGuiTheme(Gclr *Gc) {
  float h,s,v,r,g,b;
  Gc->fill_clr=44;
  Gc->char_clr=49;
  Gc->bodr_clr=44;
  Gc->cur_clr=  6;
  Gc->high_clr= 45;
  Gc->char_hclr = 47;
  Gc->msg_fill = 45;
  Gc->msg_char = 49;
  Gc->msg_bodr = 47;
  Gc->txt_fill = 43;
  Gc->txt_char = 0;
  Gc->txt_char = 48;
  Gc->txt_pchar  = 49;
  Gc->tabl_fill = 43;
  Gc->tabl_char = 0;
  Gc->tabl_char = 49;
  Gc->tabl_hchar = 47;
  Gc->tabl_line = 41;
  Gc->v_dim= 40;
  Gc->dim =42;
  Gc->bright=46;
  Gc->vbright=47;
  Gc->twin_fill = 45;
  Gc->twin_char = 0;
  Gc->twin_char = 49;
  Gc->twin_bodr = 42;
  Gc->info_fill = 46;
  Gc->info_char = 0;
  Gc->info_char = 49;
  Gc->but_char=49;
  Gc->menu_char=49;
  Gc->c_bound = 46;
  Gc->GuiFontSize =9;
  Gc->SplashFillColor=40;
  Gc->SplashBodrColor=46;
  Gc->SplashCharColor=26;
  Gc->ProgFillColor=46;
  Gc->ProgBodrColor=6;
  Gc->ProgColor=8;
  Gc->ItemHighColor=6;
  Gc->InputFontSize=9;
  Gc->scroll_fill= Gc->fill_clr;
  Gc->scroll_dim= Gc->dim;
  Gc->scroll_bright= Gc->bright;
  Gc->scroll_vbright= Gc->vbright;
  Gc->MenuFont = 40;
  Gc->PromptFont = 40;
  Gc->ButtonFont = 40;
  Gc->MsgFont = 40;
  Gc->Font=40;
  Gc->SplashFont=23;
  return ;
}
void kgColorTheme2(DIALOG *D,unsigned char red,unsigned char green, unsigned char blue) {
  float h,s,v,r,g,b,vorg,dv;
  Gclr *Gc;
  Gc = &(D->gc);
  RGBtoHSV((float) red,(float) green,(float) blue,&h,&s,&v);
  HSVtoRGB(&r,&g,&b,h,s,v*0.3);
  kgDefineColor(40,(int)r,(int)g,(int)b);
  HSVtoRGB(&r,&g,&b,h,s,v*0.45);
  kgDefineColor(41,(int)r,(int)g,(int)b);
  HSVtoRGB(&r,&g,&b,h,s,v*0.65);
  kgDefineColor(42,(int)r,(int)g,(int)b);
  HSVtoRGB(&r,&g,&b,h,s,v*0.85);
  kgDefineColor(43,(int)r,(int)g,(int)b);
  HSVtoRGB(&r,&g,&b,h,s,v);
  kgDefineColor(44,(int)r,(int)g,(int)b);
//  v = v*(1.05+2*atan(1.-v)/1.57);
  vorg = v;
  v = v*(1.01+0.25*acos(v)/1.57);
  if(v> 1.) {
     v=1.0;
     if(s< 0.1 ){
       r = 0.93*r;
       g = 0.95*g;
       RGBtoHSV((float) r,(float) g,(float) b,&h,&s,&v);
     }
     v=1.0;
     HSVtoRGB(&r,&g,&b,0.8*h,0.8*s,v);
     kgDefineColor(45,(int)r,(int)g,(int)b);
     HSVtoRGB(&r,&g,&b,h,0.7*s,v);
     kgDefineColor(46,(int)r,(int)g,(int)b);
     HSVtoRGB(&r,&g,&b,h,0.6*s,v);
     kgDefineColor(47,(int)r,(int)g,(int)b);
  }
  else {
    dv = v -vorg;
    HSVtoRGB(&r,&g,&b,h,s,vorg+dv*0.4);
    kgDefineColor(45,(int)r,(int)g,(int)b);
    HSVtoRGB(&r,&g,&b,h,s,vorg+dv*0.8);
    kgDefineColor(46,(int)r,(int)g,(int)b);
    HSVtoRGB(&r,&g,&b,h,s,v);
    kgDefineColor(47,(int)r,(int)g,(int)b);
  }
  if(vorg < 0.5) {
#if 0
    HSVtoRGB(&r,&g,&b,h,s,0.95);
    kgDefineColor(48,(int)r,(int)g,(int)b);
    HSVtoRGB(&r,&g,&b,h,s,0.85);
    kgDefineColor(49,(int)r,(int)g,(int)b);
#else
    kgDefineColor(48,230,240,230);
    kgDefineColor(49,220,235,220);
    HSVtoRGB(&r,&g,&b,h,s,vorg*1.25);
    kgDefineColor(6,(int)r,(int)g,(int)b);
#endif
  }
  Gc->fill_clr=44;
  Gc->char_clr=49;
  Gc->bodr_clr=44;
  Gc->cur_clr=  6;
  Gc->high_clr= 45;
  Gc->char_hclr = 47;
  Gc->msg_fill = 45;
  Gc->msg_char = 49;
  Gc->msg_bodr = 47;
  Gc->txt_fill = 43;
  Gc->txt_char = 48;
  Gc->txt_pchar  = 49;
  Gc->tabl_fill = 43;
  Gc->tabl_char = 48;
  Gc->tabl_hchar = 47;
  Gc->tabl_line = 41;
  Gc->v_dim= 40;
  Gc->dim =42;
  Gc->bright=46;
  Gc->vbright=47;
  Gc->twin_fill = 46;
  Gc->twin_char = 48;
  Gc->twin_bodr = 40;
  Gc->info_fill = 42;
  Gc->info_char = 49;
  Gc->info_fill = 46;
  Gc->but_char=49;
  Gc->menu_char=49;
  Gc->c_bound = 46;
  Gc->GuiFontSize =9;
  Gc->FontSize =9;
  Gc->Font=23;
  Gc->SplashFont=4;
  Gc->SplashFillColor=40;
  Gc->SplashBodrColor=46;
  Gc->SplashCharColor=26;
  Gc->ItemHighColor=6;
  Gc->InputFontSize=9;
  Gc->scroll_fill= Gc->fill_clr;
  Gc->scroll_dim= Gc->dim;
  Gc->scroll_bright= Gc->bright;
  Gc->scroll_vbright= Gc->vbright;
  Gc->GuiFontSize =9;
  Gc->MenuFont = 40;
  Gc->PromptFont = 40;
  Gc->ButtonFont = 40;
  Gc->MsgFont = 40;
  Gc->Font=40;
  Gc->SplashFont=23;
  return ;
}
void kgColorTheme(DIALOG *D,unsigned char red,unsigned char green, unsigned char blue) {
  int R,G,B;
  float H,S,V,r,g,b,vorg,dv,cval,val,hval,lval;
  int type=0,vbrt;
  Gclr *Gc;
  Gc = &(D->gc);
  RGBtoHSV((float) red,(float) green,(float) blue,&H,&S,&V);
  if(V> 0.6) {
       type =0;
       val =0.6*V;
       lval = 0.8*V;
       cval=1.4*V;
       if(cval>1.0) cval=1.0;
       hval=1.5*V;
       if(hval>1.0) hval=1.0;
   }
   else {
       type =1;
       val = 1.80*V;
       if(val>1.0) val=1.0;
       cval=0.4*V;
       hval=0.3*V;
       lval = 0.7*V;
   }
//   printf("type= %d V=%f\n",type,V);
   R = red,G=green,B=blue;
   kgDefineColor(54,R,G,B);
   HSVtoRGB(&r,&g,&b,H,S,val);
   R = r,G=g,B=b;
   kgDefineColor(55,R,G,B);
   HSVtoRGB(&r,&g,&b,H,S,cval);
   R = r,G=g,B=b;
   kgDefineColor(56,R,G,B);
   HSVtoRGB(&r,&g,&b,H,S,hval);
   R = r,G=g,B=b;
   kgDefineColor(57,R,G,B);
   val = 1.3*V;
   if(val>1.0) val=1.0;
   HSVtoRGB(&r,&g,&b,H,S,val);
   R = r,G=g,B=b;
   kgDefineColor(58,R,G,B);
   val = 1.4*V;
   if(val>1.0) val=1.0;
   HSVtoRGB(&r,&g,&b,H,S,val);
   R = r,G=g,B=b;
   kgDefineColor(59,R,G,B);
   val = 0.3*V;
   if(val>1.0) val=1.0;
   HSVtoRGB(&r,&g,&b,H,S,val);
   R = r,G=g,B=b;
   kgDefineColor(60,R,G,B);
   val = 0.2*V;
   if(val>1.0) val=1.0;
   HSVtoRGB(&r,&g,&b,H,S,val);
   R = r,G=g,B=b;
   kgDefineColor(61,R,G,B);
   HSVtoRGB(&r,&g,&b,H,S,lval);
   R = r,G=g,B=b;
   kgDefineColor(62,R,G,B);
   val = 2.0*V;
   if(val>1.0) val=1.0;
   HSVtoRGB(&r,&g,&b,H,S,val);
   R = r,G=g,B=b;
   kgDefineColor(63,R,G,B);

  vbrt = 63;
  Gc->v_dim= 61;
  Gc->dim =60;
  Gc->bright=58;
  Gc->vbright=59;

  Gc->fill_clr=54;
  Gc->char_clr=61;
  if(type) Gc->char_clr= 59;
  Gc->bodr_clr=58;
  Gc->cur_clr=  57;
  Gc->high_clr= 59;
  Gc->char_hclr = 61;
  Gc->msg_fill = 55;
  Gc->msg_char = 56;
  Gc->msg_bodr = 58;
  Gc->txt_fill = 62;
  Gc->txt_char = Gc->char_clr;
  Gc->txt_pchar  = Gc->char_clr;
  Gc->tabl_fill = 62;
  Gc->tabl_char = Gc->char_clr;
  Gc->tabl_hchar = 57;
  Gc->tabl_line = 54;
  
  Gc->twin_fill = 55;
  Gc->twin_char = 56;
  Gc->twin_bodr = 58;
  Gc->info_fill = 55;
  Gc->info_char = 56;
  Gc->but_char=Gc->v_dim;
  if(type) Gc->but_char= vbrt;
  Gc->msg_char = Gc->but_char;
  Gc->menu_char=56;
  Gc->c_bound = 58;
  Gc->GuiFontSize =9;
  Gc->FontSize =9;
  Gc->SplashFillColor=55;
  Gc->SplashBodrColor=58;
  Gc->SplashCharColor=56;
  Gc->ProgFillColor=55;
  Gc->ProgBodrColor=58;
  Gc->ProgColor=56;
  Gc->InputFontSize=9;
  Gc->scroll_fill= Gc->fill_clr;
  Gc->scroll_dim= Gc->dim;
  Gc->scroll_bright= Gc->bright;
  Gc->scroll_vbright= Gc->vbright;
  Gc->GuiFontSize =9;
  Gc->MenuFont = 40;
  Gc->PromptFont = 40;
  Gc->ButtonFont = 40;
  Gc->MsgFont = 40;
  Gc->Font=40;
  Gc->SplashFont=23;
  return ;
}
void kgColorTheme1(DIALOG *D,unsigned char red,unsigned char green, unsigned char blue) {
  int R,G,B;
  float H,S,V,r,g,b,vorg,dv,cval,val,hval,lval;
  int type=0;
  Gclr *Gc;
  Gc = &(D->gc);
  RGBtoHSV((float) red,(float) green,(float) blue,&H,&S,&V);
  if(V> 0.6) {
       type =0;
       val =0.6*V;
       lval = 0.8*V;
       cval=1.2*V;
       if(cval>1.0) cval=1.0;
       hval=1.3*V;
       if(hval>1.0) hval=1.0;
   }
   else {
       type =1;
       val = 1.6*V;
       if(val>1.0) val=1.0;
       cval=0.4*V;
       hval=0.3*V;
       lval = 0.7*V;
   }
   R = red,G=green,B=blue;
   kgDefineColor(54,R,G,B);
   HSVtoRGB(&r,&g,&b,H,S,val);
   R = r,G=g,B=b;
   kgDefineColor(55,R,G,B);
   HSVtoRGB(&r,&g,&b,H,S,cval);
   R = r,G=g,B=b;
   kgDefineColor(56,R,G,B);
   HSVtoRGB(&r,&g,&b,H,S,hval);
   R = r,G=g,B=b;
   kgDefineColor(57,R,G,B);
   val = 1.2*V;
   if(val>1.0) val=1.0;
   HSVtoRGB(&r,&g,&b,H,S,val);
   R = r,G=g,B=b;
   kgDefineColor(58,R,G,B);
   val = 1.3*V;
   if(val>1.0) val=1.0;
   HSVtoRGB(&r,&g,&b,H,S,val);
   R = r,G=g,B=b;
   kgDefineColor(59,R,G,B);
   val = 0.3*V;
   if(val>1.0) val=1.0;
   HSVtoRGB(&r,&g,&b,H,S,val);
   R = r,G=g,B=b;
   kgDefineColor(60,R,G,B);
   val = 0.2*V;
   if(val>1.0) val=1.0;
   HSVtoRGB(&r,&g,&b,H,S,val);
   R = r,G=g,B=b;
   kgDefineColor(61,R,G,B);
   HSVtoRGB(&r,&g,&b,H,S,lval);
   R = r,G=g,B=b;
   kgDefineColor(62,R,G,B);

  Gc->v_dim= 61;
  Gc->dim =60;
  Gc->bright=58;
  Gc->vbright=59;

  Gc->fill_clr=54;
  Gc->char_clr=61;
  if(type) Gc->char_clr= 59;
  Gc->bodr_clr=58;
  Gc->cur_clr=  57;
  Gc->high_clr= 59;
  Gc->char_hclr = 61;
  Gc->msg_fill = 55;
  Gc->msg_char = 56;
  Gc->msg_bodr = 58;
  Gc->txt_fill = 62;
  Gc->txt_char = 59;
  if(type) Gc->txt_char = 61;
  Gc->txt_pchar  = Gc->char_clr;
  Gc->tabl_fill = 62;
  Gc->tabl_char = Gc->txt_char;
  Gc->tabl_hchar = 57;
  Gc->tabl_line = 54;
  
  Gc->twin_fill = 55;
  Gc->twin_char = 56;
  Gc->twin_bodr = 58;
  Gc->info_fill = 55;
  Gc->info_char = 56;
  Gc->but_char=59;
  if(type) Gc->but_char= 61;
  Gc->menu_char=56;
  Gc->c_bound = 58;
  Gc->GuiFontSize =9;
  Gc->MenuFont = 8;
  Gc->PromptFont = 24;
  Gc->ButtonFont = 16;
  Gc->FontSize =9;
  Gc->Font=16;
  Gc->SplashFont=24;
  Gc->SplashFillColor=55;
  Gc->SplashBodrColor=58;
  Gc->SplashCharColor=56;
  Gc->ProgFillColor=55;
  Gc->ProgBodrColor=58;
  Gc->ProgColor=56;
  Gc->InputFontSize=9;
  Gc->scroll_fill= Gc->fill_clr;
  Gc->scroll_dim= Gc->dim;
  Gc->scroll_bright= Gc->bright;
  Gc->scroll_vbright= Gc->vbright;
  Gc->GuiFontSize =9;
  Gc->MenuFont = 40;
  Gc->PromptFont = 40;
  Gc->ButtonFont = 40;
  Gc->MsgFont = 40;
  Gc->Font=40;
  Gc->SplashFont=23;
  return ;
}
void kgGrayGuiTheme(Gclr *Gc) {
  Gc->fill_clr=10;
  Gc->char_clr=49;
  Gc->bodr_clr=13;
  Gc->cur_clr=  6;
  Gc->high_clr= 13;
  Gc->char_hclr = 15;
  Gc->msg_fill = 9;
  Gc->msg_char = 14;
  Gc->msg_bodr = 12;
  Gc->txt_fill = 9;
  Gc->txt_char = 48;
  Gc->txt_pchar  = 49;
  Gc->tabl_fill = 11;
  Gc->tabl_char = 8;
  Gc->tabl_hchar = 15;
  Gc->tabl_line = 8;
  Gc->v_dim= 8;
  Gc->dim =9;
  Gc->bright=12;
  Gc->vbright=14;
  Gc->twin_fill = 15;
  Gc->twin_char = 49;
  Gc->twin_bodr = 8;
  Gc->info_fill = 9;
  Gc->info_char = 49;
  Gc->but_char=49;
  Gc->menu_char=49;
  Gc->c_bound = 13;
  Gc->GuiFontSize =9;
  Gc->MenuFont = 8;
  Gc->PromptFont = 32;
  Gc->ButtonFont = 23;
  Gc->MsgFont = 23;
  Gc->FontSize =8;
  Gc->Font=23;
  Gc->SplashFont=4;
  Gc->SplashFillColor=40;
  Gc->SplashBodrColor=46;
  Gc->SplashCharColor=26;
  Gc->ProgFillColor=46;
  Gc->ProgBodrColor=6;
  Gc->ProgColor=8;
  Gc->ItemHighColor=6;
  Gc->InputFontSize=8;
  Gc->scroll_fill= Gc->fill_clr;
  Gc->scroll_dim= Gc->dim;
  Gc->scroll_bright= Gc->bright;
  Gc->scroll_vbright= Gc->vbright;
  Gc->GuiFontSize =9;
  Gc->MenuFont = 40;
  Gc->PromptFont = 40;
  Gc->ButtonFont = 40;
  Gc->MsgFont = 40;
  Gc->Font=40;
  Gc->SplashFont=23;
  return ;
}
#if 0
void kgInit(void) {
 if(_Dsplocks==PTHREAD_MUTEX_INITIALIZER) {
    pthread_mutex_init(&_Dsplocks,NULL);
 }
}
#endif
void uiInitGbox(DIALOG *D,int i) {
  int l;
  static int entry=0;
  char flname[200],reviewfile[200];

  DIG *G;
  kgDC *dc;
  pthread_mutex_lock(&_Initlock);
  entry++;
  G = D->d[i].g;
  dc = (kgDC *) Malloc(sizeof(kgDC));
  G->D= D;
  G->wc = (kgWC *) Malloc(sizeof(kgWC));
  G->dc = dc;
  G->Obj_opn=0;
  dc->Fontlist=NULL;
  ui_initialise(G);
  for(l=0;l<10;l++) dc->st_ptr[l]=0;
  G->B_min = 10400;
  G->BACK_UP=0;
  G->OPEN=1;
  G->Byte=0;
  G->R_Byte=0;
  G->R_max=0;
  G->D_ON=1;
  G->MAG=1;
  G->img=NULL;
  G->rzimg=NULL;
  G->pixels=NULL;
  kgViewport(G,0.,0.,1.0,1.0);
  kgUserFrame(G,0.,0.,(float)(G->x2-G->x1),(float)(G->y2-G->y1));
  strcpy(flname,D->tmpdir);
  strcat(flname,"/objs");
  l = strlen(flname);
  sprintf(flname+l,"_%5.5d.%3.3d",getpid(),entry);
  strcpy(dc->objdir,flname);
//  printf("%s\n",dc->objdir);
  mkdir(dc->objdir,0700);
  strcpy(dc->reviewfile,dc->objdir);
  strcat(dc->reviewfile,"/reviewdat");
  G->rbuf = open(dc->reviewfile,O_CREAT|O_BINARY|O_TRUNC|O_RDWR,0666);
  strcpy(dc->plotfile,dc->objdir);
  strcat(dc->plotfile,"/plotgph");
  strcpy(dc->cmdsfile,dc->objdir);
  strcat(dc->cmdsfile,"/cmdsfile");
  dc->ls_list=NULL;
  dc->No_of_lights=0;
  G->Rbuff = (unsigned char  *) Malloc(B_max+100);
  G->hbuf =-1;
  uiinitialise(G);
  pthread_mutex_unlock(&_Initlock);
}


  /*_______________ESC SEQ BELOW THIS -----------*/

void uiwrite_buf(DIG *G,unsigned char *ch, int n )
 {
    int i,j;
    kgDC *dc;
    dc = G->dc;
    if( G->Rbuff == NULL) printf("G->Rbuff= NULL\n");
    for(i = 0; i <n; i++) G->Rbuff[G->Byte++] = *(ch+i);
     if (G->Byte > B_max) {
      if(G->rbuf== -1) {
        G->rbuf = open(dc->reviewfile,O_CREAT|O_BINARY|O_TRUNC|O_RDWR,0666);
        G->OPEN=1;
      }
       WRITE(G->rbuf,(void *)G->Rbuff,B_min);
      j = 0;G->BACK_UP=1;
      for(i=B_min;i<G->Byte;i++)
       G->Rbuff[j++]=G->Rbuff[i];
      G->Byte=j;
    }
 } 
void kgViewport(DIG *G,float x1, float y1, float x2, float y2)
 {
     if((G->D != NULL) )ui_vu_port(G,x1,y1,x2,y2);
     if(G->D_ON ==0 ) {
       GMIMG *png;
       Image *img;
       kgDC *dc;
       dc = G->dc;
       img_viewport(G,x1,y1,x2,y2);
       if(G->img != NULL) {
        png = G->img;
        if((png->image_width!= dc->EVGAX)||(png->image_height!=dc->EVGAY)){
          uiFreeImage(G->img); 
          if(dc->ZBUFF==1) { ui_release_buf(G); ui_set_zbuf(G); }
          G->img=uiCreateCleangmImage(dc->EVGAX,dc->EVGAY,0,0,0,255);
          png = G->img;
          img = png->image;
          G->pixels=GetImagePixels((Image *)(png->image),0,0,((Image *)(png->image))->columns,((Image *)(png->image))->rows);
        }
        else uiCleangmImage(png);
       }
       else {
         if(dc->ZBUFF==1) { ui_release_buf(G); ui_set_zbuf(G); }
         G->img=uiCreateCleangmImage(dc->EVGAX,dc->EVGAY,0,0,0,255);
         png = G->img;
         img = png->image;
         G->pixels=GetImagePixels((Image *)(png->image),0,0,((Image *)(png->image))->columns,((Image *)(png->image))->rows);
       }
     }
     else {
       kgDC *dc;
       dc = G->dc;
       if(dc->ZBUFF==1) { ui_release_buf(G); ui_set_zbuf(G); }
     }
 }
void kgSetAntialiasing(DIG *G,int val) {
  kgDC *dc;
  dc = G->dc;
  if(G->D == NULL) return;
  if(val != 0) {
     if(G->D_ON==0) return;
     G->D_ON=0;
  }
  else G->D_ON= 1;
  if(G->D_ON ==0 ) {
     GMIMG *png;
     int MAG=2;
     if(val > 0) MAG = val+1;
     G->MAG = MAG;
     dc->MAG=MAG;
     if(dc->ZBUFF==1) { ui_release_buf(G); ui_set_zbuf(G); }
     img_viewport(G,dc->vu_x1,dc->vu_y1,dc->vu_x2,dc->vu_y2);
     G->img=uiCreateCleangmImage(dc->EVGAX,dc->EVGAY,0,0,0,255);
     png = G->img;
     G->pixels=GetImagePixels((Image *)(png->image),0,0,((Image *)(png->image))->columns,((Image *)(png->image))->rows);
  }  
  else {
    G->MAG=1;
    kgViewport(G,dc->vu_x1,dc->vu_y1,dc->vu_x2,dc->vu_y2);
    if(G->img != NULL) {uiFreeImage(G->img); G->img=NULL;}
    if(G->rzimg != NULL) {uiFreeImage(G->rzimg); G->rzimg=NULL;}
  }
}
void kgAntialiasingOn(DIG *G,int val) {
  GMIMG *png;
  int MAG=2;
  kgDC *dc;
  dc = G->dc;
  if(G->D == NULL) return;
  if(val < 0) val =0;
  if(G->D_ON==0) {
    if(G->img != NULL) {uiFreeImage(G->img); G->img=NULL;}
    if(G->rzimg != NULL) {uiFreeImage(G->rzimg); G->rzimg=NULL;}
  }
  G->D_ON= 0;
  MAG = val+1;
  G->MAG = MAG;
  dc->MAG=MAG;
  if(dc->ZBUFF==1) { ui_release_buf(G); ui_set_zbuf(G); }
  img_viewport(G,dc->vu_x1,dc->vu_y1,dc->vu_x2,dc->vu_y2);
  G->img=uiCreateCleangmImage(dc->EVGAX,dc->EVGAY,0,0,0,255);
  png = G->img;
  G->pixels=GetImagePixels((Image *)(png->image),0,0,((Image *)(png->image))->columns,((Image *)(png->image))->rows);
}  
void kgAntialiasingOff(DIG *G) {
    kgDC *dc;
    if(G->D == NULL) return;
    dc = G->dc;
    G->D_ON= 1;
    G->MAG=1;
    kgViewport(G,dc->vu_x1,dc->vu_y1,dc->vu_x2,dc->vu_y2);
    if(G->img != NULL) {uiFreeImage(G->img); G->img=NULL;}
    if(G->rzimg != NULL) {uiFreeImage(G->rzimg); G->rzimg=NULL;}
}
void kgUserFrame(DIG *G,float x1,float y1, float x2, float y2)
 {
     ui_window(G,x1,y1,x2,y2);
 }
void kgGetWindow(DIG *G,float *x1,float *y1,float *x2,float *y2) {
  kgDC *dc;
  dc = G->dc;
  *x1= dc->w_x1; *y1= dc->w_y1;
  *x2= dc->w_x2; *y2= dc->w_y2;
}
void kgGetViewport(DIG *G,float *x1,float *y1,float *x2,float *y2) {
  kgDC *dc;
  dc = G->dc;
  *x1= dc->vu_x1; *y1= dc->vu_y1;
  *x2= dc->vu_x2; *y2= dc->vu_y2;
}
void gphUserFrame(int fid,float x1,float y1, float x2, float y2)
 {
     WRITE(fid,&x1,4);
     WRITE(fid,&y1,4);
     WRITE(fid,&x2,4);
     WRITE(fid,&y2,4);
 }
void kgMove2f(DIG *G,float x,float y)
 {
     kgDC *dc;
     dc= G->dc;
     if(G->Rbuff != NULL) {
       uiwrite_bf(G,&L,1);
       uiwrite_bf(G,&M,1);
       uiwrite_bf(G,&x,4);
       uiwrite_bf(G,&y,4);
     }
     if(G->D_ON)ui_move(G,x,y);
     else img_move(G,x,y);
 }
void gphMove2f(int fid,float x,float y)
 {
     WRITE(fid,&L,1);
     WRITE(fid,&M,1);
     WRITE(fid,&x,4);
     WRITE(fid,&y,4);
 }
void kgMarkerType(DIG *G,int mtype)
{
     kgDC *dc;
     dc= G->dc;
     if(G->Rbuff != NULL) {
       uiwrite_bf(G,&M,1);
       uiwrite_bf(G,&T,1);
       uiwrite_bf(G,&mtype,4);
     }
     if(G->D_ON)ui_smmark(G,mtype);
     dc->m_style=mtype;
}

void kgDraw2f(DIG *G,float x,float y)
 {
     kgDC *dc;
     dc= G->dc;
     if(G->Rbuff != NULL) {
       uiwrite_bf(G,&L,1);
       uiwrite_bf(G,&D,1);
       uiwrite_bf(G,&x,4);
       uiwrite_bf(G,&y,4);
     }
     if(G->D_ON)ui_draw(G,x,y);
     else img_draw(G,x,y);
 }
void gphDraw2f(int fid,float x,float y)
 {
     WRITE(fid,&L,1);
     WRITE(fid,&D,1);
     WRITE(fid,&x,4);
     WRITE(fid,&y,4);
 }
void kgCircle2f(DIG *G,float x,float y,float r)
 {
     if(r<=0) return;
     if(G->Rbuff != NULL) {
       uiwrite_bf(G,&L,1);
       uiwrite_bf(G,&R,1);
       uiwrite_bf(G,&x,4);
       uiwrite_bf(G,&y,4);
       uiwrite_bf(G,&r,4);
     }
     if(G->D_ON)ui_circle(G,x,y,r);
     else img_circle(G,x,y,r);
 }
void gphCircle2f(int fid,float x,float y,float r)
 {
     if(r<=0) return;
     WRITE(fid,&L,1);
     WRITE(fid,&R,1);
     WRITE(fid,&x,4);
     WRITE(fid,&y,4);
     WRITE(fid,&r,4);
 }
void kgMarker2f(DIG *G,float x,float y)
{
    if(G->Rbuff != NULL) {
      uiwrite_bf(G,&M,1);
      uiwrite_bf(G,&K,1);
      uiwrite_bf(G,&x,4);
      uiwrite_bf(G,&y,4);
    }
    if(G->D_ON)ui_marker(G,x,y);
    else img_marker(G,x,y);
}
void kgLineStyle(DIG *G,int style)
{
    if(G->Rbuff != NULL) {
     uiwrite_bf(G,&L,1);
     uiwrite_bf(G,&S,1);
     uiwrite_bf(G,&style,4);
    }
    ui_ln_style(G,style);
 }
void gphLineStyle(int fid,int style)
{
     WRITE(fid,&L,1);
     WRITE(fid,&S,1);
     WRITE(fid,&style,4);
 }
void kgLineColor(DIG *G,int colr)
 {
     kgDC *dc;
     dc = G->dc;

     if(G->Rbuff != NULL) {
       uiwrite_bf(G,&L,1);
       uiwrite_bf(G,&C,1);
       uiwrite_bf(G,&colr,4);
     }
     if(G->D_ON)ui_ln_clr(G,colr);
     else{ dc->ln_color=colr;dc->c_color=colr;}
 }
void gphLineColor(int fid,int colr)
 {
     WRITE(fid,&L,1);
     WRITE(fid,&C,1);
     WRITE(fid,&colr,4);
 }
void kgTextColor(DIG *G,int tcolr)
 {
     kgDC *dc;
     dc = G->dc;

     if(G->Rbuff != NULL) {
       uiwrite_bf(G,&T,1);
       uiwrite_bf(G,&C,1);
       uiwrite_bf(G,&tcolr,4);
     }
     if(G->D_ON)ui_txt_clr(G,tcolr);
     else dc->t_color=tcolr;
 }
void gphTextColor(int fid,int tcolr)
 {
     WRITE(fid,&T,1);
     WRITE(fid,&C,1);
     WRITE(fid,&tcolr,4);
 }
void kgTextSize(DIG *G,float h,float w,float g)
 {
     float txt_htx,txt_wty,txt_spy;
     kgDC *dc;
     dc = G->dc;
     if(G->Rbuff != NULL) {
       uiwrite_bf(G,&T,1);
       uiwrite_bf(G,&S,1);
       uiwrite_bf(G,&h,4);
       uiwrite_bf(G,&w,4);
       uiwrite_bf(G,&g,4);
     }
     txt_htx=h/(dc->w_y2-dc->w_y1)*(dc->w_x2-dc->w_x1);
     txt_wty=w/(dc->w_x2-dc->w_x1)*(dc->w_y2-dc->w_y1);
     txt_spy=g/(dc->w_x2-dc->w_x1)*(dc->w_y2-dc->w_y1);
     if(G->Rbuff != NULL) {
       uiwrite_bf(G,&txt_htx,4);
       uiwrite_bf(G,&txt_wty,4);
       uiwrite_bf(G,&txt_spy,4);
     }
     ui_txt_size(G,h,w,g,txt_htx,txt_wty,txt_spy);
    
 }
void gphTextSize(int fid,float h,float w,float g,float w_x1,float w_y1,float w_x2,float w_y2)
 {
     float txt_htx,txt_wty,txt_spy;
     WRITE(fid,&T,1);
     WRITE(fid,&S,1);
     WRITE(fid,&h,4);
     WRITE(fid,&w,4);
     WRITE(fid,&g,4);
     txt_htx=h/(w_y2-w_y1)*(w_x2-w_x1);
     txt_wty=w/(w_x2-w_x1)*(w_y2-w_y1);
     txt_spy=g/(w_x2-w_x1)*(w_y2-w_y1);
     WRITE(fid,&txt_htx,4);
     WRITE(fid,&txt_wty,4);
     WRITE(fid,&txt_spy,4);
 }
void uirevtxtsize(DIG *G,float h,float w,float g,float hx,float wy,float spy)
 {
     float txt_htx,txt_wty,txt_spy;
     if(G->Rbuff != NULL) {
     uiwrite_bf(G,&T,1);
     uiwrite_bf(G,&S,1);
     uiwrite_bf(G,&h,4);
     uiwrite_bf(G,&w,4);
     uiwrite_bf(G,&g,4);
     }
     txt_htx=hx;
     txt_wty=wy;
     txt_spy=spy;
     if(G->Rbuff != NULL) {
     uiwrite_bf(G,&txt_htx,4);
     uiwrite_bf(G,&txt_wty,4);
     uiwrite_bf(G,&txt_spy,4);
     }
     ui_txt_size(G,h,w,g,txt_htx,txt_wty,txt_spy);
 }
void kgTextAngle(DIG *G,float ang)
 {
     if(G->Rbuff != NULL) {
       uiwrite_bf(G,&T,1);
       uiwrite_bf(G,&R,1);
       uiwrite_bf(G,&ang,4);
     }
     ui_txt_rot(G,ang);
 }
void gphTextAngle(int fid,float ang)
 {
     WRITE(fid,&T,1);
     WRITE(fid,&R,1);
     WRITE(fid,&ang,4);
 }
void kgWriteText( DIG *G,char *c)
 {
     int n=0;
     while(*(c+n) != '\0') n++;
     n++;
     if(G->Rbuff != NULL) {
       uiwrite_bf(G,&T,1);
       uiwrite_bf(G,&W,1);
       uiwrite_bf(G,&n,4);
       uiwrite_bf(G,c,n);
     }
     if(G->D_ON)ui_txt_wr(G,n,c);
     else img_txt_wr(G,n,c);
     return;
 }
void gphWriteText( int fid,char *c)
 {
     int n=0;
     while(*(c+n) != '\0') n++;
     n++;
     WRITE(fid,&T,1);
     WRITE(fid,&W,1);
     WRITE(fid,&n,4);
     WRITE(fid,c,n);
     return;
 }
void kgTextFont(DIG *G,int tf)
 {

     if(G->Rbuff != NULL) {
      uiwrite_bf(G,&T,1);
      uiwrite_bf(G,&F,1);
      uiwrite_bf(G,&tf,4);
     }
     ui_txt_font(G,tf);
 }
void gphTextFont(int fid,int tf) {
     WRITE(fid,&T,1);
     WRITE(fid,&F,1);
     WRITE(fid,&tf,4);
}
void kgPolyFill(DIG *G,int n, float *x, float *y, int flag,int color)
 {
     if( n<3) return;
     if(G->Rbuff != NULL) {
      uiwrite_bf(G,&P,1);
      uiwrite_bf(G,&P,1);
      uiwrite_bf(G,&n,4);
      uiwrite_bf(G,x,4*n);
      uiwrite_bf(G,y,4*n);
      uiwrite_bf(G,&flag,4);
      uiwrite_bf(G,&color,4);
     }
     if(G->D_ON)ui_panel(G,x,y,(int)color,flag,n);
     else img_poly_fill(G,n,x,y,flag,color);
  }
void gphPolyFill(int fid,int n, float *x, float *y, int flag,int color)
 {
      if( n<3) return;
      WRITE(fid,&P,1);
      WRITE(fid,&P,1);
      WRITE(fid,&n,4);
      WRITE(fid,x,4*n);
      WRITE(fid,y,4*n);
      WRITE(fid,&flag,4);
      WRITE(fid,&color,4);
  }
void kgBoxFill(DIG *G,float x1,float y1, float x2, float y2,int fl,int ib)
 {

     if(G->Rbuff != NULL) {
      uiwrite_bf(G,&P,1);
      uiwrite_bf(G,&B,1);
      uiwrite_bf(G,&x1,4);
      uiwrite_bf(G,&y1,4);
      uiwrite_bf(G,&x2,4);
      uiwrite_bf(G,&y2,4);
      uiwrite_bf(G,&fl,4);
      uiwrite_bf(G,&ib,4);
     }
     if(G->D_ON)ui_box_fill(G,x1,y1,x2,y2,fl,ib);
     else img_boxfill(G,x1,y1,x2,y2,fl);

 }
void gphBoxFill(int fid,float x1,float y1, float x2, float y2,int fl,int ib)
 {

      WRITE(fid,&P,1);
      WRITE(fid,&B,1);
      WRITE(fid,&x1,4);
      WRITE(fid,&y1,4);
      WRITE(fid,&x2,4);
      WRITE(fid,&y2,4);
      WRITE(fid,&fl,4);
      WRITE(fid,&ib,4);

 }
void kgSetGouraudParams(DIG *G,float p2, float p1, int n, int ib)
 {
     int startclr=64;
     if(G->Rbuff != NULL) {
      uiwrite_bf(G,&P,1);
      uiwrite_bf(G,&S,1);
      uiwrite_bf(G,&p2,4);
      uiwrite_bf(G,&p1,4);
      uiwrite_bf(G,&n,4);
      uiwrite_bf(G,&ib,4);
     }
     ui_set_godr(G,p2,p1,startclr,n,ib);
 }
void kgSetShadeParams(DIG *G,float p2, float p1,int startclr, int n, int ib)
 {
     if(G->Rbuff != NULL) {
      uiwrite_bf(G,&P,1);
      uiwrite_bf(G,&H,1);
      uiwrite_bf(G,&p2,4);
      uiwrite_bf(G,&p1,4);
      uiwrite_bf(G,&startclr,4);
      uiwrite_bf(G,&n,4);
      uiwrite_bf(G,&ib,4);
     }
     ui_set_godr(G,p2,p1,startclr,n,ib);
 }
void kgGouraudFill(DIG *G,int n, float *x, float *y,float *v)
 {
     if(G->Rbuff != NULL) {
       uiwrite_bf(G,&P,1);
       uiwrite_bf(G,&G,1);
       uiwrite_bf(G,&n,4);
       uiwrite_bf(G,x,4*n);
       uiwrite_bf(G,y,4*n);
       uiwrite_bf(G,v,4*n);
     }
     if(G->D_ON)ui_godr_fill(G,n,x,y,v);
     else img_godard_fill(G,n,x,y,v);
 }
void gphSetShadeParams(int fid,float p2, float p1,int startclr, int n, int ib)
 {
      WRITE(fid,&P,1);
      WRITE(fid,&H,1);
      WRITE(fid,&p2,4);
      WRITE(fid,&p1,4);
      WRITE(fid,&startclr,4);
      WRITE(fid,&n,4);
      WRITE(fid,&ib,4);
 }
void gphGouraudFill(int fid,int n, float *x, float *y,float *v)
 {
     WRITE(fid,&P,1);
     WRITE(fid,&G,1);
     WRITE(fid,&n,4);
     WRITE(fid,x,4*n);
     WRITE(fid,y,4*n);
     WRITE(fid,v,4*n);
 }
void uiinitialise(DIG *G)
 {
     DIALOG *D;
     D = G->D;
     kgLineColor(G,1);
     if(G->D_ON) { 
      NCLRS= get_total_colors();
//      Application_Gclr("glib");
     }

 }
void kgClearView(DIG *G)
 {
     char ch='L';
     /*write_bf(&ch,1);*/
     if(G->D != NULL) {
       ui_clr_vu(G);
     }
     if(G->D_ON==0)uiCleangmImage(G->img);
 }
 void kgDisplayGphFile(DIALOG *D,char *flname,int xo,int yo,int xl,int yl,unsigned long bkcolor) {
#if 0
    unsigned long *data;
    data = uiGetPixelData(flname,xl,yl,bkcolor);
    if(data != NULL) {
      uiAddDataToArea(D,data,xo,yo,xl,yl);
      free(data);
    }
#else
    void *img;
    img=kgGphtoImage(flname,xl,yl,bkcolor);
    kgImage(D,img,xo,yo,xl,yl,0.0,1.0);
    uiFreeImage(img);
#endif
 } 
 void kgImportGphFile(DIG *G,char *flname,float gx1,float gy1,float gx2,float gy2)
  {
      float w[4];
      FILE * pf;
      int p,ans=1;
      float temp;
      float fx,sx,fy,sy;
      pf=fopen(flname,"r");
      if( pf== NULL) {
        return;
      };
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      if(gx1 >gx2) {temp=gx1;gx1=gx2;gx2=temp;}
      if(gy1 >gy2) {temp=gy1;gy1=gy2;gy2=temp;}
      if ( Fread(pf,(void *)w,16) != 16 ) {
        if(G->D != NULL) {
          ans=kgCheckMenu(G->D,G->x1,(G->y1+G->y2)/2,
                      "Error in File CONTINUE?",ans);
          fclose(pf);
          if(ans) return;
        }
        printf("Error in file '%s'\n",flname);
        exit(0);
      };
// Checking needed here
//TCB
//      kgDoubleBuffer(G);
      kgSetClip(G,gx1,gy1,gx2,gy2);
      fx = (gx2-gx1)/(w[2]-w[0]);
      sx = -(gx1-w[0]*fx);
      fy = (gy2-gy1)/(w[3]-w[1]);
      sy = -(gy1-w[1]*fy);
      uirev_file(pf,G,sx,fx,sy,fy);
      fclose(pf);
      kgPopClip(G);
#if 0
      if(dc->ZBUFF) {
         kgSwapBuffers(G);
      }
#endif
//      kgSwapBuffers(G);
//      kgSingleBuffer(G);
//      kgReview(G);
      if(G->D != NULL) kgUpdateOn(G->D);
      return;
 }
#if 0
 void  back_gr(void *parent,char flnamei[])
  {
   int ans=1;
   int pf;
   float clip_max=65000,clip_min =0;
   char flname[250];
   if(PROJ)update_gr_info();
   strcpy(flname,flnamei);
   pf=0;
   while(flname[pf]>' ')pf++;
   flname[pf]='\0';
   pf=open(flname,O_CREAT|O_RDWR|O_BINARY|O_TRUNC, S_IREAD|S_IWRITE);
   if( pf<= 0) {
     ans=kgCheckMenu(parent,50,200,"Error: In file name CONTINUE?",ans);
     if(ans) return;
     exit(0);
   };
   WRITE(pf,(void *)&w_x1,4);
   WRITE(pf,(void *)&w_y1,4);
   WRITE(pf,(void *)&w_x2,4);
   WRITE(pf,(void *)&w_y2,4);
   if((ZBUFF==1)||(TIFF)) {
     WRITE(pf,(void *)&Z,1);
/*
     WRITE(pf,(void *)FM,64);
     WRITE(pf,(void *)&xe,4);
     WRITE(pf,(void *)&ye,4);
     WRITE(pf,(void *)&ze,4);
     WRITE(pf,(void *)&zc,4);
     WRITE(pf,(void *)&clip_min,4);
     WRITE(pf,(void *)&clip_max,4);
*/
   }
   Fixshadecolors_hsv(pf);
   BACK_UP=1;
   close_file(rbuf);
   rbuf=open(reviewfile,O_RDWR|O_BINARY, S_IREAD|S_IWRITE);
   if( rbuf<0) return;
   read_file(rbuf); 
   while(Byte==B_min){WRITE(pf,(void *)Rbuff,Byte); read_file(rbuf); }
   WRITE(pf,(void *)Rbuff,Byte);
   OPEN=1;
   R_Byte=0;
   Byte=0;R_max=0;
   close(pf);
   return;
  }
#endif
 void  kgBackupGph(DIG *G,char *flname)
  {
   unsigned char CZ='Z';
   int ans=1;
   int pf;
   float clip_max=65000,clip_min =0;
   DIALOG *D;
   kgDC *dc;
   kgWC *wc;
   D = G->D;
   dc = G->dc;
   wc = G->wc;
   if(G->Rbuff== NULL) {
    fprintf(stderr,"Empty Buffer...\n");
    return;
   }
   if(dc->PROJ){
//     printf( "PROJ = %d\n",dc->PROJ);
//TCBTCB
//     update_gr_info(G);
//     printf( "PROJ = %d\n",dc->PROJ);
   }
   remove(flname);
   pf=open(flname,O_CREAT|O_RDWR, S_IREAD|S_IWRITE);
   if( pf< 0) {
     if(D== NULL) {
       fprintf(stderr,"Failed to open: %s\n",flname);
       exit(0);
     }
     ans=kgCheckMenu(D,D->xo+G->x1,D->yo+(G->y1+G->y2)/2,
                     "Error: In file name CONTINUE?",ans);
     if(ans) return;
     exit(0);
   };
   WRITE(pf,(void *)&dc->w_x1,4);
   WRITE(pf,(void *)&dc->w_y1,4);
   WRITE(pf,(void *)&dc->w_x2,4);
   WRITE(pf,(void *)&dc->w_y2,4);
#if 1
   if(dc->PROJ) {
     WRITE(pf,(void *)&CZ,1);
     WRITE(pf,(void *)&dc->w_x1,4);
     WRITE(pf,(void *)&dc->w_y1,4);
     WRITE(pf,(void *)&dc->w_x2,4);
     WRITE(pf,(void *)&dc->w_y2,4);
     WRITE(pf,(void *)dc->FM,64);
     WRITE(pf,(void *)&(dc->xe),4);
     WRITE(pf,(void *)&(dc->ye),4);
     WRITE(pf,(void *)&(dc->ze),4);
     WRITE(pf,(void *)&(dc->zc),4);
     WRITE(pf,(void *)&(dc->clip_min),4);
     WRITE(pf,(void *)&(dc->clip_max),4);
     if(dc->ZBUFF) WRITE(pf,(void *)&Z,1);
   }
//TCBTCB
//   Fixshadecolors_hsv(pf);

#endif
   G->BACK_UP=1;
   uiclose_file(G,&(G->rbuf),dc->reviewfile);
   G->rbuf=open(dc->reviewfile,O_RDWR|O_BINARY, S_IREAD|S_IWRITE);
   if( G->rbuf<0) return;
   uiread_file(G,G->rbuf); 
   while(G->Byte==G->B_min){
    WRITE(pf,(void *)G->Rbuff,G->Byte);
    uiread_file(G,G->rbuf);
   }
   WRITE(pf,(void *)G->Rbuff,G->Byte);
   G->OPEN=1;
   G->R_Byte=0;
   G->Byte=0;G->R_max=0;
   close(pf);
   return;
  }
 void  uiSetGboxBackup(void *Tmp,int gindex,char *flname){
   DIALOG *D;
   DIG *G;
   kgDC *dc;
   D = (DIALOG *)Tmp;
   G = D->d[gindex].g;
   dc = G->dc;
   strcpy(dc->backup,flname);
   return;
 }
 int  update_gr_info(DIG *G) {
/*******************************************************************
   for updating 3d-calls to current rotation and clipping
   so that backing up need not contain transformation information
   this function has to be called by back_gr
 *******************************************************************/
   FILE *fp;
   char JunkFile[300];
   int ans=1,set3d,zbuff,tiff;
   int pf;
   kgDC *dc;
   dc= G->dc;
   strcpy(JunkFile,dc->objdir);
   strcat(JunkFile,"/Junk.Fil");
   pf=open(JunkFile,O_CREAT|O_RDWR|O_BINARY|O_TRUNC, S_IREAD|S_IWRITE);
   if( pf<= 0) {
     printf("Error: In opening JunK.Fil check:\n");
     exit(0);
   };
   G->BACK_UP=1;
   uiclose_file(G,&(G->rbuf),dc->reviewfile);
   G->rbuf=open(dc->reviewfile,O_RDWR|O_BINARY, S_IREAD|S_IWRITE);
   if( G->rbuf<0) return 0;
   uiread_file(G,G->rbuf); 
   while(G->Byte==B_min){WRITE(pf,(void *)G->Rbuff,G->Byte); uiread_file(G,G->rbuf); }
   WRITE(pf,(void *)G->Rbuff,G->Byte);
   G->OPEN=1;
   G->R_Byte=0;
   G->Byte=0;G->R_max=0;
   close(pf);
   zbuff = dc->ZBUFF; set3d = dc->SET3D;
   kgClearBuffer(G);
   fp = fopen(JunkFile,"r");
   G->D_ON = 0;
   dc->ZBUFF = zbuff, dc->SET3D = set3d;
//TCB
   kgViewport(G,dc->vu_x1,dc->vu_y1,dc->vu_x2,dc->vu_y2);
   kgUserFrame(G,dc->w_x1,dc->w_y1,dc->w_x2,dc->w_y2);
   kgSetZbuffer(G,zbuff);
   uicnv_file(G,fp);
   fclose(fp);
   remove(JunkFile);
   G->D_ON = 1;
   return 1;
  }
void kgLineWidth( DIG *G,int dw)
 {
   if(G->Rbuff != NULL) {
    uiwrite_bf(G,&L,1);
    uiwrite_bf(G,&W,1);
    uiwrite_bf(G,&dw,4);
   }
  ui_lnwidth(G,dw);
 }
void gphLineWidth( int fid,int dw)
 {
  WRITE(fid,&L,1);
  WRITE(fid,&W,1);
  WRITE(fid,&dw,4);
 }
 int uiDraw_arc(DIG *G,float xo,float yo,float r,float ang1,float ang2)
  {
    double a1,a2,a3,rd,th,t;
    int n,i;
    float x,y;
    a1 = ang1*3.14159265/180.0;
    a2 = ang2*3.14159265/180.0;
    th= 5.0*3.14159265/180.0;
    n = fabs(a2-a1)/th+0.5;
    th = (a2-a1)/n;
    t =a1;
    x= r*cos(t)+xo;
    y = r*sin(t)+yo;
    kgMove2f(G,x,y);
    for(i=1;i<n;i++){
      t +=th;
      x= r*cos(t)+xo;
      y = r*sin(t)+yo;
      kgDraw2f(G,x,y);
    }
    t =a2;
    x= r*cos(t)+xo;
    y = r*sin(t)+yo;
    kgDraw2f(G,x,y);
    return((n+1)*10);
  }
int uiProcess_arc (DIG *G,float *xo,float *yo)
 {
    float ang1,ang2,r;
    float x1,y1,x2,y2;
    gscanf(G->D,"Give Arc Radius=%10f",&m_radius);
    y1=*yo;
    x1= *xo+m_radius;
//    put_message(100,50,(char *)"Fix First Point on Arc:");  
//    gbell(); 
    kgArcCursor(G,&x1,&y1,xo,yo);
//    clear_message();  
//    put_message(100,50,(char *)"Fix Second Point on Arc:");  
//    gbell(); 
    y2=*yo;
    x2= *xo+m_radius;
    kgArcCursor(G,&x2,&y2,xo,yo);
//    clear_message();  
/*
    ang2= (float)Get_intv("Give Arc angle : "); 
    ang1= atan2( (double)(y1-*yo),(double)(x1-*xo))/3.14159265*180.0; 
    ang2 +=ang1; 
    r = sqrt((*xo-x1)*(*xo-x1)+(*yo-y1)*(*yo-y1)); 
*/
    r= m_radius;
    ang1= atan2( (double)(y1-*yo),(double)(x1-*xo))/3.14159265*180.0; 
    ang2= atan2( (double)(y2-*yo),(double)(x2-*xo))/3.14159265*180.0; 
    if(ang1<0.) ang1+=360.;
    if(ang2<0.) ang2+=360.;
    if(ang1==ang2) ang2=ang1+360;
    if( ang2<ang1) ang2+=360;
    return(uiDraw_arc( G,*xo,*yo,r,ang1,ang2)); 
 }
 int gphArcFill(int fid,float xo,float yo,float r,float ang1,float ang2,int flag ,int color)
  {
    double a1,a2,a3,rd,th,t;
    int n,i,full=0;
    float *x,*y;
    if( fabs((double)(ang1-ang2))> 359.0) full=1;
    a1 = ang1*3.14159265/180.0;
    a2 = ang2*3.14159265/180.0;
    th= 2.0*3.14159265/180.0;
    n = fabs(a2-a1)/th+0.5;
    x = Malloc(sizeof(float)*(n+2));
    y = Malloc(sizeof(float)*(n+2));
    if(y==NULL) {
      printf("Error: In mem allocation..\n");
      exit(0);
    }
    th = (a2-a1)/n;
    t =a1;
    i=0;
    *(x+i)= r*cos(t)+xo;
    *(y+i) = r*sin(t)+yo;
    for(i=1;i<n;i++){
      t +=th;
      *(x+i)= r*cos(t)+xo;
      *(y+i) = r*sin(t)+yo;
    }
    t =a2;
    if(!full) {
      *(x+n)= r*cos(t)+xo;
      *(y+n) = r*sin(t)+yo;
       n++;
      *(x+n)= xo;
      *(y+n) = yo;
      n++;
    }
    gphPolyFill(fid,n,x,y,flag,color);
    free(x),free(y);
    return(8*n+14);
  }
 int kgArcFill(void *fid,float xo,float yo,float r,float ang1,float ang2,int flag ,int color)
  {
    double a1,a2,a3,rd,th,t;
    int n,i,full=0;
    float *x,*y;
    if( fabs((double)(ang1-ang2))> 359.0) full=1;
    a1 = ang1*3.14159265/180.0;
    a2 = ang2*3.14159265/180.0;
    th= 2.0*3.14159265/180.0;
    n = fabs(a2-a1)/th+0.5;
    x = Malloc(sizeof(float)*(n+2));
    y = Malloc(sizeof(float)*(n+2));
    if(y==NULL) {
      printf("Error: In mem allocation..\n");
      exit(0);
    }
    th = (a2-a1)/n;
    t =a1;
    i=0;
    *(x+i)= r*cos(t)+xo;
    *(y+i) = r*sin(t)+yo;
    for(i=1;i<n;i++){
      t +=th;
      *(x+i)= r*cos(t)+xo;
      *(y+i) = r*sin(t)+yo;
    }
    t =a2;
    if(!full) {
      *(x+n)= r*cos(t)+xo;
      *(y+n) = r*sin(t)+yo;
       n++;
      *(x+n)= xo;
      *(y+n) = yo;
      n++;
    }
    kgPolyFill(fid,n,x,y,flag,color);
    free(x),free(y);
    return(8*n+14);
  }
int kgGetEllipsePoints(float xsize,float ysize,float fac,int n,float *x,float *y) {
   int i;
   float a,b,e,emax,xx,yy,dx,dy,ba,xp,yp;
   xsize *=0.5;
   ysize *=0.5;
   if(xsize >= ysize) {
     emax = sqrtf((xsize+ysize)*(xsize-ysize))/xsize;
     e = emax*fac;
     b= ysize;
     ba = sqrtf(1-e*e);
     a = b/ba;
     xp = xsize -a;
     yp = 0.0;
     xx = a;
     dx = xx/(n-1);
     i=0;
     x[i]=xx+xp; y[i]=0.0;
     for(i=1;i<n;i++) {
       xx -=dx;
       x[i]=xx+xp;
       y[i]= ba*sqrtf((a+xx)*(a-xx));
     }
     x[n]=0.0;
     y[n]= ysize;
     x[n+1]=0.0;
     y[n+1]= ysize*0.5;
     x[n+2]=0.0;
     y[n+2]= 0.0;
   }
   else {
     emax = sqrtf((xsize+ysize)*(ysize-xsize))/ysize;
     e = emax*fac;
     b= xsize;
     ba = sqrtf(1-e*e);
     a = b/ba;
     yp = ysize -a;
     xp = 0.0;
     xx =b;
     yy=0;
     dy = a/(n-1);
     x[0] = xx;
     y[0]=  0;
     x[1]= xx;
     y[1]= yp;
     i=0;
     for(i=2;i< n;i++) {
       yy +=dy;
       xx = ba*sqrtf((a+yy)*(a-yy));
       x[i]=xx;
       y[i] = yp+yy;
     }
       x[n]= 0.0;
       y[n]= ysize ;
       x[n+1]= 0.0;
       y[n+1]= ysize -a;
       x[n+2]=0;
       y[n+2]=0;
       
   }
   return n+3;

}
int gphRoundedRectangleRing2(int fid,float xo,float yo,float xl,float yl,float red,float green,float blue,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   float vmin,vmax;
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float xd1[800],yd1[800],vd1[800];
   float dl,xp,yp,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1,fac2;
   double dl1,dl2;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   if(bordrsize > size*0.5) bordrsize=size*0.5;
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize*0.5;
   fac1 = dl1/(size-bordrsize);
   dl2 = dl -bordrsize;
   fac2 = dl2/(size-2*bordrsize);
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   RGBtoHSV(red,green,blue,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   vmax=vv*1.8;
   if(vmax > 1.) vmax=1.;
   if(vmax < 0.4 ) vmax=0.4;
   vmin=vv+(vmax -vv)*.2;
   if(vmin > 1.0) vmin=1.0;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin1= vv;
       vmax1 = vv;
       vmin = vlow;
       vmax = vlow;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,xd,yd) - 2;
       np =kgGetEllipsePoints( xl-bordrsize,yl-bordrsize,fac1, count,x, y)-2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, count,xd1, yd1)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         vd[i]= vmin;
         v[i]= vmax;
         vd1[i]= vmin;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, xd, yd, vd,vv,vmin, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, x, y, vd,vv,vmax, count);
   }
   gphMixHsvColors(fid,200,64,h,s,vmin,vmax);
   gphSetShadeParams(fid,vmax,vmin,200, 64, 0);
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int gphRoundedRectangleRing1(int fid,float xo,float yo,float xl,float yl,float red,float green,float blue,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   float vmin,vmax;
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float xd1[800],yd1[800],vd1[800];
   float dl,xp,yp,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1,fac2;
   double dl1,dl2;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   if(bordrsize > size*0.5) bordrsize=size*0.5;
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize*0.5;
   fac1 = dl1/(size-bordrsize);
   dl2 = dl -bordrsize;
   fac2 = dl2/(size-2*bordrsize);
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   RGBtoHSV(red,green,blue,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   if(vv > 0.7) vv=.7;
   vmax=vv*1.8;
   if(vmax > 1.) vmax=1.;
   if(vmax < 0.4 ) vmax=0.4;
   vmin=vv+(vmax -vv)*.6;
   if(vmin > 1.0) vmin=1.0;
   vmid = vmax*0.95;
   vlow= vmin;
   if(vv > 1 ) vv=1;
   if(vlow > 1 ) vlow=1;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin1= vv;
       vmax1 = vv;
       vmin = vlow;
       vmax = vlow;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,xd,yd) - 2;
       np =kgGetEllipsePoints( xl-bordrsize,yl-bordrsize,fac1, count,x, y)-2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, count,xd1, yd1)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         vd[i]= vmin;
         v[i]= vmax;
         vd1[i]= vmin;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, xd, yd, vd,vv,vv, count);
       np =kgGetCirclePoints( xl-bordrsize,yl-bordrsize,fac1, x, y, v,vmin,vmax, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, xd1, yd1, vd1,vv,vv, count);
   }
   gphMixHsvColors(fid,200,64,h,s,vv,vmax);
   gphSetShadeParams(fid,vmax,vv,200, 64, 0);
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd1[npj];
     y[npi]= yd1[npj];
     v[npi]= vd1[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int gphRoundedRectangleRing3(int fid,float xo,float yo,float xl,float yl,float red,float green,float blue,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   float vmin,vmax;
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float xd1[800],yd1[800],vd1[800];
   float dl,xp,yp,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1,fac2;
   double dl1,dl2;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   if(bordrsize > size*0.5) bordrsize=size*0.5;
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize*0.5;
   fac1 = dl1/(size-bordrsize);
   dl2 = dl -bordrsize;
   fac2 = dl2/(size-2*bordrsize);
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   RGBtoHSV(red,green,blue,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   if(vv > 0.7) vv=.7;
   vmax=vv*1.8;
   if(vmax > 1.) vmax=1.;
   if(vmax < 0.4 ) vmax=0.4;
   vmin=vv+(vmax -vv)*.6;
   if(vmin > 1.0) vmin=1.0;
   vmid = vmax*0.95;
   vlow= vmin;
   if(vv > 1 ) vv=1;
   if(vlow > 1 ) vlow=1;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin1= vv;
       vmax1 = vv;
       vmin = vlow;
       vmax = vlow;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,xd,yd) - 2;
       np =kgGetEllipsePoints( xl-1.8*bordrsize,yl-1.8*bordrsize,fac1, count,x, y)-2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, count,xd1, yd1)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         vd[i]= vmin;
         v[i]= vmax;
         vd1[i]= vmin;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, xd, yd, vd,vv,vv, count);
       np =kgGetCirclePoints( xl-1.8*bordrsize,yl-1.8*bordrsize,fac1, x, y, v,vmin,vmax, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, xd1, yd1, vd1,vv,vv, count);
   }
   gphMixHsvColors(fid,200,64,h,s,vv,vmax);
   gphSetShadeParams(fid,vmax,vv,200, 64, 0);
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd1[npj];
     y[npi]= yd1[npj];
     v[npi]= vd1[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int gphRoundedRectangleRing(int fid,float xo,float yo,float xl,float yl,float red,float green,float blue,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   float vmin,vmax;
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float xd1[800],yd1[800],vd1[800];
   float dl,xp,yp,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1,fac2;
   double dl1,dl2;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   if(bordrsize > size*0.5) bordrsize=size*0.5;
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize*0.5;
   fac1 = dl1/(size-bordrsize);
   dl2 = dl -bordrsize;
   fac2 = dl2/(size-2*bordrsize);
//   fac1=0.5;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   RGBtoHSV(red,green,blue,&h,&s,&vv);
//   printf("%f %f %f %f %f %f\n",red ,green,blue,h,s,vv);
   horg = h;
   sorg =s;
   vorg = vv;
   vmax=vv*1.4;
   if(vmax > 1.0) vmax=1.0;
   vmid = vmax*0.95;
//   vmid = vmax;
   vmin = 0.7*vmax;
   vv = vmax;
   vlow= vmin;
   if(vv > 1 ) vv=1;
   if(vlow > 1 ) vlow=1;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin1= vv;
       vmax1 = vv;
       vmin = vlow;
       vmax = vlow;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,xd,yd) - 2;
       np =kgGetEllipsePoints( xl-bordrsize,yl-bordrsize,fac1, count,x, y)-2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, count,xd1, yd1)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         vd[i]= vmin;
         v[i]= vmax;
         vd1[i]= vmin;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, xd, yd, vd,vlow,vlow, count);
       np =kgGetCirclePoints( xl-bordrsize,yl-bordrsize,fac1, x, y, v,vmax,vmax, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, xd1, yd1, vd1,vlow,vlow, count);
   }
   gphMixHsvColors(fid,200,128,h,s,vmin,vmax);
   gphSetShadeParams(fid,vmax,vmin,200, 128, 0);
   for(i=0;i<np;i++) {
    v[i]=vmid;
   }
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   vd[3]= vmax;
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
    v[i]=vmax;
   }
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   vd[1]= vmid;
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
    v[i]=vmid;
   }
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd1[npj];
     y[npi]= yd1[npj];
     v[npi]= vd1[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   vd[3]= vmax;
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
    v[i]=vmax;
   }
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= vmid;
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
    v[i]=vmid;
   }
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int gphRoundedRectangleFill(int fid,float xo,float yo,float xl,float yl,int ib,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp;
   double ang,dang;
   int i,j;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   count = 4*count2 ;
   for(i=0;i<count;i++) {
     x[i] +=xo;
     y[i] +=yo;
     
   }
   gphPolyFill(fid,count,x,y,ib,clr);
   return 1;
}
int gphRoundedRectangleShade_o(int fid,float xo,float yo,float xl,float yl,int type,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp;
   int r,g,b;
   float h,s,vv;
   double ang,dang;
   int i,j;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   kgGetDefaultRGB(clr,&r,&g,&b);
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   if(vv < 0.5) vv=0.5;
   if(vv>0.6) vv=0.6;
   vv =0.5;
   count1 = count+1;
   count2 = count+2;
//   gphmixshadecolors_hsv(fid,48,16, h, s, vv) ;
   gphMixHsvColors(fid,200,16,h,s,0.5,1.0);
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   x[0]=dl +xp;
   y[0]= yp;
   for(i=1;i<count1;i++) {
     ang +=dang;
     x[i]= dl*cos(ang)+xp;
     y[i]= dl*sin(ang)+yp;
   }
   x[count]= xp;
   y[count]= dl+yp;
   for(i=1;i<count2;i++) {
     x[count+i]= -x[count1-i];
     y[count+i] = y[count1-i];
   }
   for(i=1;i<(count1+count2);i++) {
     x[2*count+1+i]= x[2*count1-i];
     y[2*count+1+i] = -y[2*count1-i];
   }
   for(i=0;i<4*count1;i++) {
     v[i] = y[i];
     x[i] +=xo;
     y[i] +=yo;
     
   }
   gphSetShadeParams(fid,yl*0.5, -yl*0.5,200, 16, 0);
   gphGouraudFill(fid,4*count1,x,y,v);
   return 1;
}

int gphRoundedRectangleShade4(int fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
   float bordrsize=3;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1;
   double dl1;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   bordrsize = size*0.1;
   if(bordrsize< 4) bordrsize=4;
   if(bordrsize > size*0.3) bordrsize=size*0.3;
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize;
   fac1 = dl1/(size-2*bordrsize);
//   fac1=0.5;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   kgGetDefaultRGB(clr,&r,&g,&b);
//TCB
//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   vmid = vmax;
   vv = (vmin+0.4*(vmax-vmin));
//   if(vmid>vorg) vmid=vorg;
   vlow= vmin;
   if(vv > 1 ) vv=1;
   if(vlow > 1 ) vlow=1;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin1= vv;
       vmax1 = vv;
       vmin = vlow;
       vmax = vlow;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, count,xd, yd)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         v[i]= vmin+ss*ss*dv;
         vd[i]= vmid+ss*ss*dv1;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, x, y, v,vlow,vlow, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vmid,vmid, count);
   }
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   gphMixHsvColors(fid,200,128,h,s,vmin,vmax);
   gphSetShadeParams(fid,vmax,vmin,200, 128, 0);
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i]= xo;
     yd[i]= yo;
     vd[i]= vv;
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= xo;
   yd[0]= yo;
   vd[0]= vv;
   xd[3]= xo;
   yd[3]= yo;
   vd[3]= vv;
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i]= xo;
     yd[i]= yo;
     vd[i]= vv;
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= xo;
   yd[0]= yo;
   vd[0]= vv;
   xd[1]= xo;
   yd[1]= yo;
   vd[1]= vv;
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= xo;
   yd[0]= yo;
   vd[0]= vv;
   xd[1]= xo;
   yd[1]= yo;
   vd[1]= vv;
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i]= xo;
     yd[i]= yo;
     vd[i]= vv;
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
//   gphPolyFill(fid,np,x,y,0,clr);
   for(i=0;i<np;i++) {
     xd[i]= xo;
     yd[i]= yo;
     vd[i]= vv;
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= xo;
   yd[0]= yo;
   vd[0]= vv;
   xd[3]= xo;
   yd[3]= yo;
   vd[3]= vv;
   gphGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int gphRoundedRectangleShade0(int fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp;
   int r,g,b;
   float h,s,vv;
   double ang,dang;
   int i,j;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   kgGetDefaultRGB(clr,&r,&g,&b);
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   if(vv < 0.5) vv=0.5;
   if(vv>0.6) vv=0.6;
   vv =0.5;
   count1 = count+1;
   count2 = count+2;
   gphMixHsvColors(fid,200,128,h,s,vmin,vmax);
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   count = 4*count2 ;
   for(i=0;i<count;i++) {
     v[i] = +y[i];
     x[i] +=xo;
     y[i] +=yo;
     
   }
   gphSetShadeParams(fid,yl*0.5, -yl*0.5,200, 128, 0);
   gphGouraudFill(fid,count,x,y,v);
   return 1;
}
int gphRoundedRectangleShade1(int fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp,dis,D;
   int r,g,b;
   float h,s,vv;
   double ang,dang;
   int i,j;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   kgGetDefaultRGB(clr,&r,&g,&b);
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   gphMixHsvColors(fid,200,128,h,s,vmin,vmax);
   count1 = count+1;
   count2 = count+2;
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) ;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
     x[count2+1]= 0.0;
     y[count2+1]= yp;
     x[count2+2]= 0.0;
     y[count2+2]= 0.0;
     count = count2+3;
   }
   for(i=0;i<count;i++) {
     v[i] = yl*0.5 - fabsf(y[i]);
     x[i] +=xo;
     y[i] +=yo;
     
   }
   gphSetShadeParams(fid,yl*0.5, 0.0,200, 128, 0);
   gphGouraudFill(fid,count,x,y,v);
   for(i=0;i<count;i++) {
     x[i] =2*xo-x[i];
   }
   gphGouraudFill(fid,count,x,y,v);
   for(i=0;i<count;i++) {
     y[i] =2*yo-y[i];
   }
   gphGouraudFill(fid,count,x,y,v);
   for(i=0;i<count;i++) {
     x[i] =2*xo-x[i];
   }
   gphGouraudFill(fid,count,x,y,v);
   return 1;
}
int gphRoundedRectangleShade(int fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp,dis,D;
   int r,g,b;
   float h,s,vv;
   double ang,dang;
   int i,j;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   kgGetDefaultRGB(clr,&r,&g,&b);
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   gphMixHsvColors(fid,200,128,h,s,vmin,vmax);
   count1 = count+1;
   count2 = count+2;
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) ;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
     x[count2+1]= 0.0;
     y[count2+1]= yp;
     x[count2+2]= 0.0;
     y[count2+2]= 0.0;
     count = count2+3;
   }
   D = (xl*xl+yl*yl)*0.25;
   D = sqrtf(D);
   for(i=0;i<count;i++) {
     dis = x[i]*x[i]+y[i]*y[i];
     dis = sqrtf(dis);
     v[i] = 1.0 -dis/D;
     x[i] +=xo;
     y[i] +=yo;
     
   }
   gphSetShadeParams(fid,1.0, 0.0,200, 128, 0);
   gphGouraudFill(fid,count,x,y,v);
   for(i=0;i<count;i++) {
     x[i] =2*xo-x[i];
   }
   gphGouraudFill(fid,count,x,y,v);
   for(i=0;i<count;i++) {
     y[i] =2*yo-y[i];
   }
   gphGouraudFill(fid,count,x,y,v);
   for(i=0;i<count;i++) {
     x[i] =2*xo-x[i];
   }
   gphGouraudFill(fid,count,x,y,v);
   return 1;
}
int gphRoundedRectangle(int fid,float xo,float yo,float xl,float yl,int clr,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
//   float bordrsize=2.5;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1;
   double dl1;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
//   if(fac > 0.5) fac=0.5;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize;
   fac1 = dl1/(size-2*bordrsize);
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   kgGetDefaultRGB(clr,&r,&g,&b);
//TCB

//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   if(vv > 0.9)  { s = 0.5*sorg; }
   vv =vorg +(1.-vorg)*0.9;
   if(vv > 1 ) vv=1;
   vmid = vorg +(1-vorg)*0.5;
   if(vv== vmid) vmid=0.95*vv;
   vmid1 = vorg +(1-vorg)*0.45;
   if(vmid > 1 ) vmid=1;
   if(vmid1 > 1 ) vmid1=1;

   vlow = vorg*0.8;
   vlow1 = vorg*0.20;
#if 1
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin= vlow1;
       vmax = vmid;
       vmin1 = vlow;
       vmax1 = vv;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, count,xd, yd)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         v[i]= vmin+ss*ss*dv;
         vd[i]= vmin1+ss*ss*dv1;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, x, y, v,vlow1,vmid, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vlow,vv, count);
   }
#else
#if 1
   np =kgGetCirclePoints( xl,yl,fac, x, y, v,vlow1,vmid, count);
   np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vlow,vv, count);
#else
   np =kgGetCirclePoints( xl,yl,fac, x, y, v,vv,vlow, count);
   np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vmid,vlow1, count);
#endif
#endif
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
   }
//   gphMixHsvColors(fid,200,128,h,s,vlow1,vv);
//   gphSetShadeParams(fid,vv,vlow1,200, 128, 0);
   gphPolyFill(fid,np2,xd,yd,0,clr);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
//   gphGouraudFill(fid,4,xd,yd,vd);
   gphPolyFill(fid,4,xd,yd,0,clr);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   gphPolyFill(fid,np2,xd,yd,0,clr);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphPolyFill(fid,4,xd,yd,0,clr);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[3]= x[0];
   yd[3]= -y[0];
   vd[3]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[1]= x[np2-1];
   yd[1]=  y[np2-1];
   vd[1]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphPolyFill(fid,4,xd,yd,0,clr);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   gphPolyFill(fid,np2,xd,yd,0,clr);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   gphPolyFill(fid,np2,xd,yd,0,clr);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   gphPolyFill(fid,4,xd,yd,0,clr);
   return 1;
}
int gphRoundedRectangle_o(int fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp;
   double ang,dang;
   int i,j;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   count = 4*count2 ;
   for(i=0;i<count;i++) {
     x[i] +=xo;
     y[i] +=yo;
     
   }
   gphLineColor(fid,clr);
   gphMove2f(fid,x[0],y[0]);
   for(i=1;i<count;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphDraw2f(fid,x[0],y[0]);
   return 1;
}
int kgGetCirclePoints(float xl,float yl,float fac,float *x,float *y,float *v,float vmin,float vmax,int count) {
   int i,j;
   int count1,count2,hcount;
   float dl,xp,yp;
   double ang,dang,Dang,dv,ss,cs;
   dl = fac*yl;
   if(dl > fac*xl){ dl=fac*xl;}
   dv = vmax-vmin;
   Dang = 90.0/count;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   x[0]=dl +xp;
   y[0]= yp;
   v[0]=vmin;
   for(i=1;i< count;i++) {
     ang +=dang;
     ss = sin(ang);
     cs = cos(ang);
     x[i]= dl*cs+xp;
     y[i]= dl*ss+yp;
     v[i]= vmin+ss*ss*dv;
   }
   x[count]= xp;
   y[count]= yp+dl;
   v[count]= vmax;
/*
   for(i=0;i<=count;i++) {
     x[count1+i]= -x[count-i];
     y[count1+i] = y[count-i];
     v[count1+i]= vmax;
   }
   for(i=0;i<(count1);i++) {
     x[2*count1+i]= x[2*count1-1-i];
     y[2*count1+i] = -y[2*count1-1-i];
     v[2*count1+i]= v[count-i];
   }
   for(i=0;i<(count1);i++) {
     x[3*count1+i]=  -x[3*count1-1-i];
     y[3*count1+i] = y[3*count1-1-i];
     v[3*count1+i]= vmin;
   }
   x[4*count1]=x[0];
   y[4*count1]=y[0];
   v[4*count1]=v[0];
   return (count1*4+1);
*/
  return count1;
}
int gphRoundedRectanglePressed(int fid,float xo,float yo,float xl,float yl,int clr,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
//   float bordrsize=3;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1;
   double dl1;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize;
   fac1 = dl1/(size-2*bordrsize);
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   kgGetDefaultRGB(clr,&r,&g,&b);
//TCB
//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   vv = 1.3*vv;
   if(vv > 1 ) vv=1;
   vlow = vorg*0.8;
   vlow1 = vorg*0.40;
   vmid = vorg;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin= vv;
       vmax = vlow;
       vmin1 = vmid;
       vmax1 = vlow1;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, count,xd, yd)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         v[i]= vmin+ss*ss*dv;
         vd[i]= vmin1+ss*ss*dv1;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
     np =kgGetCirclePoints( xl,yl,fac, x, y, v,vv,vlow, count);
     np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vmid,vlow1, count);
   }
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
   }
   gphMixHsvColors(fid,200,128,h,s,vlow1,vv);
   gphSetShadeParams(fid,vv,vlow1,200, 128, 0);
   gphGouraudFill(fid,np2,xd,yd,v);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
//   gphPolyFill(fid,np,x,y,0,clr);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int gphRoundedRectangleRaised(int fid,float xo,float yo,float xl,float yl,int clr,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
//   float bordrsize=2.5;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1;
   double dl1;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
//   if(fac > 0.5) fac=0.5;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize;
   fac1 = dl1/(size-2*bordrsize);
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   kgGetDefaultRGB(clr,&r,&g,&b);
//TCB

//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   vv = vorg*(1.05+acos(vorg)/1.57);
//   vv = vorg*(1.05+2*atan(1.-vorg)/1.57);
   if(vv > 1.0)  {
      if(sorg < 0.1 ) {
//       printf("%f %f %f\n",h,s,vv);
       r = 0.93*r;
       g = 0.97*g;
       RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
      }
      else {
        s = 0.8*sorg;
      }
      vv =1.0;
   }
   vmid = vv*0.9;
   vmid1 = vv*0.8;

   vlow = vorg*0.95;
   vlow = vlow*vlow;
   vlow1 = vlow*0.75;
#if 1
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin= vlow1;
       vmax = vmid;
       vmin1 = vlow;
       vmax1 = vv;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, count,xd, yd)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         v[i]= vmin+ss*ss*dv;
         vd[i]= vmin1+ss*ss*dv1;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, x, y, v,vlow1,vmid, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vlow,vv, count);
   }
#else
#if 1
   np =kgGetCirclePoints( xl,yl,fac, x, y, v,vlow1,vmid, count);
   np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vlow,vv, count);
#else
   np =kgGetCirclePoints( xl,yl,fac, x, y, v,vv,vlow, count);
   np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vmid,vlow1, count);
#endif
#endif
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
   }
   gphMixHsvColors(fid,200,128,h,s,vlow1,vv);
   gphSetShadeParams(fid,vv,vlow1,200, 128, 0);
   gphGouraudFill(fid,np2,xd,yd,v);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[3]= x[0];
   yd[3]= -y[0];
   vd[3]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[1]= x[np2-1];
   yd[1]=  y[np2-1];
   vd[1]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
//   gphPolyFill(fid,np,x,y,0,clr);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int gphRoundedRectangleRaised_1(int fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np,pos;
   float x[450],y[450],xx,yy;
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1;
   double ang,dang;
   int i,j,r,g,b;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   kgGetDefaultRGB(clr,&r,&g,&b);
//TCB

//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
#if 1
   if(vv > 0.9)  { s = 0.5*sorg; }
//   if(s < 0.3 ) s=0.3;
#else
   s = 0.3*sorg;
   if(s < 0.2 ) s=0.2;
#endif
   vv =vorg +(1.-vorg)*0.6;
//   if(vv > 1.2*vorg) vv= 1.2*vorg;
//   if(vv > 2.5*vorg) vv= 2.5*vorg;
   if(vv > 1 ) vv=1;
   vmid = vorg +(1-vorg)*0.55;
   vmid1 = vorg +(1-vorg)*0.45;
//   if(vmid > 1.6*vorg) vmid= 1.6*vorg;
   if(vmid > 1 ) vmid=1;
   if(vmid1 > 1 ) vmid1=1;

   vlow = vorg*0.9;
   vlow1 = vorg*0.80;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 3;
       count2 = count+2;
       count1=  count+1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= yp;
     for(i=1;i<=count;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
   }
   for(i=0;i<=count;i++) {
     x[count1+i]= -x[count-i];
     y[count1+i] = y[count-i];
   }
   for(i=0;i<(2*count1);i++) {
     x[2*count1+i]= x[2*count1-1-i];
     y[2*count1+i] = -y[2*count1-1-i];
   }
   
   for(i=0;i<4*count1;i++) {
     x[i] +=xo;
     y[i] +=yo;
   }
   hcount = count1/4;
   if(vorg > 0.8)HSVtoRGB(&red,&green,&blue,h,s,vorg*0.6);
   else HSVtoRGB(&red,&green,&blue,h,s,0.4);
   HSVtoRGB(&red,&green,&blue,h,s,vorg*0.6);
   gphChangeColor(fid,245,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vlow1);
   gphChangeColor(fid,246,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vlow);
   gphChangeColor(fid,247,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid1);
   gphChangeColor(fid,248,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid);
   gphChangeColor(fid,249,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vv);
   gphChangeColor(fid,250,(int)red,(int)green,(int)blue);
   gphLineColor(fid,246);
   gphLineWidth(fid,4);
   gphMove2f(fid,x[0],y[0]);
#if 1
   for(i=1;i<hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos=hcount;
   gphLineColor(fid,247);
   for(i=pos;i<pos+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos+=hcount;
   gphLineColor(fid,248);
   for(i=pos;i<pos+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos+=hcount;
   gphLineColor(fid,249);
   for(i=pos;i<count1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos=count1;
   gphLineWidth(fid,6);
   gphLineColor(fid,250);
   for(i=pos;i< (pos+count1+1);i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineWidth(fid,4);
   pos =(2*count1+1);
   gphLineColor(fid,249);
   for(i=pos;i<pos+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos +=hcount;
   gphLineColor(fid,248);
   for(i=pos;i<pos+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos +=hcount;
   gphLineColor(fid,247);
   for(i=pos;i<pos+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos +=hcount;
   gphLineColor(fid,246);
   for(i=pos;i< 3*count1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos = 3*count1;
   gphLineColor(fid,245);
   for(i=pos;i<4*count1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
#else
   for(i=1;i<4*count1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
#endif
   gphDraw2f(fid,x[0],y[0]);
   return 1;
}
int gphRoundedRectangleRaised_o(int fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np;
   float x[450],y[450],xx,yy;
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1;
   double ang,dang;
   int i,j,r,g,b;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   hcount = count/2;
   kgGetDefaultRGB(clr,&r,&g,&b);
//TCB

//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   horg = h;
   sorg =s;
#if 1
   if(vv > 0.7)  { s = 0.3*sorg; }
   if(s < 0.2 ) s=0.2;
#else
   s = 0.3*sorg;
   if(s < 0.2 ) s=0.2;
#endif
   vorg = vv;
   vv =vorg +(1.-vorg)*0.95;
   vv =vorg +(1.-vorg)*0.65;
//   if(vv > 1.2*vorg) vv= 1.2*vorg;
   if(vv > 1.8*vorg) vv= 1.8*vorg;
   if(vv > 1 ) vv=1;
   vmid = vorg +(1-vorg)*0.55;
   vmid1 = vorg +(1-vorg)*0.5;
//   if(vmid > 1.6*vorg) vmid= 1.6*vorg;
   if(vmid > 1 ) vmid=1;
   if(vmid1 > 1 ) vmid1=1;

   vlow = vorg*0.8;
   vlow1 = vorg*0.5;
//   vv = 0.9;
//   if(vlow > 0.3 ) vlow=0.3;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   hcount = (count2-2)/2;
   count = 4*count2 ;
   
   for(i=0;i<count;i++) {
     x[i] +=xo;
     y[i] +=yo;
     
   }
   HSVtoRGB(&red,&green,&blue,h,s,vmid1);
   gphChangeColor(fid,246,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vlow1);
   gphChangeColor(fid,247,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vlow);
   gphChangeColor(fid,248,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid);
   gphChangeColor(fid,249,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vv);
   gphChangeColor(fid,250,(int)red,(int)green,(int)blue);
   if(vorg > 0.3)HSVtoRGB(&red,&green,&blue,h,s,vorg*0.3);
   else HSVtoRGB(&red,&green,&blue,h,s,0.0);
   gphChangeColor(fid,251,(int)red,(int)green,(int)blue);
   gphLineColor(fid,251);
   gphMove2f(fid,x[0],y[0]);
   gphDraw2f(fid,x[1],y[1]);
   gphLineColor(fid,247);
   gphMove2f(fid,x[1],y[1]);
   for(i=2;i<hcount+2;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   xx = x[i],yy= y[i];
   gphLineColor(fid,246);
//   gphLineColor(fid,250);
   for(i=hcount+2;i<count2-1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,250);
   for(i=count2-1;i<2*count2+2;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,250);
   for(i=2*count2+2;i<2*count2+2+hcount/2;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,246);
   for(i=2*count2+2+hcount/2;i<2*count2+2+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   np = 3*count2-1-(2*count2+2+hcount);
   np=np/2;
   gphLineColor(fid,247);
   for(i=2*count2+2+hcount;i<3*count2-1-np;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,251);
   for(i=3*count2-1-np;i<3*count2-1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,251);
   for(i=3*count2-1;i<count;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphDraw2f(fid,x[0],y[0]);
   return 1;
}
int gphRoundedRectangleLowered(int fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount;
   float x[450],y[450];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow;
   double ang,dang;
   int i,j,r,g,b;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   hcount = count/2;
   kgGetDefaultRGB(clr,&r,&g,&b);
   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   vorg = vv;
   vv =vorg +(1.-vorg)*0.75;
   if(vv > 1.5*vorg) vv= 1.5*vorg;
   vmid = vorg +(1-vorg)*0.3;
   if(vmid > 1.3*vorg) vmid= 1.3*vorg;
   vlow = vorg*0.6;
//   vv = 0.9;
//   if(vlow > 0.3 ) vlow=0.3;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   hcount = (count2-2)/2;
   count = 4*count2 ;
   
   for(i=0;i<count;i++) {
     x[i] +=xo;
     y[i] +=yo;
     
   }
   HSVtoRGB(&red,&green,&blue,h,s,vlow);
   gphChangeColor(fid,248,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid);
   gphChangeColor(fid,249,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vv);
   gphChangeColor(fid,250,(int)red,(int)green,(int)blue);
   if(vorg > 0.4)HSVtoRGB(&red,&green,&blue,h,s,vorg*0.4);
   else HSVtoRGB(&red,&green,&blue,h,s,0.0);
   gphChangeColor(fid,251,(int)red,(int)green,(int)blue);
   gphLineColor(fid,250);
   gphMove2f(fid,x[0],y[0]);
   gphDraw2f(fid,x[1],y[1]);
   gphLineColor(fid,249);
   for(i=2;i<hcount+2;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,248);
   for(i=hcount+2;i<count2-1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,251);
   for(i=count2-1;i<2*count2+2;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,248);
   for(i=2*count2+2;i<2*count2+2+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,249);
   for(i=2*count2+2+hcount;i<3*count2-1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,250);
   for(i=3*count2-1;i<count;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphDraw2f(fid,x[0],y[0]);
   return 1;
}
int gphRoundedRectanglePressed_o(int fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount;
   float x[450],y[450];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow;
   double ang,dang;
   int i,j,r,g,b;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   hcount = count/2;
   kgGetDefaultRGB(clr,&r,&g,&b);
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   vorg = vv;
   vv =vorg +(1.-vorg)*0.75;
   if(vv > 1.5*vorg) vv= 1.5*vorg;
   vmid = vorg +(1-vorg)*0.3;
   if(vmid > 1.3*vorg) vmid= 1.3*vorg;
   vlow = vorg*0.6;
//   vv = 0.9;
//   if(vlow > 0.3 ) vlow=0.3;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   hcount = (count2-2)/2;
   count = 4*count2 ;
   
   for(i=0;i<count;i++) {
     x[i] +=xo;
     y[i] +=yo;
     
   }
   HSVtoRGB(&red,&green,&blue,h,s,vlow);
   gphChangeColor(fid,248,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid);
   gphChangeColor(fid,249,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vv);
   gphChangeColor(fid,250,(int)red,(int)green,(int)blue);
   if(vorg > 0.4)HSVtoRGB(&red,&green,&blue,h,s,vorg*0.4);
   else HSVtoRGB(&red,&green,&blue,h,s,0.0);
   gphChangeColor(fid,251,(int)red,(int)green,(int)blue);
   gphLineColor(fid,250);
   gphMove2f(fid,x[0],y[0]);
   gphDraw2f(fid,x[1],y[1]);
   gphLineColor(fid,249);
   for(i=2;i<hcount+2;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,248);
   for(i=hcount+2;i<count2-1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,251);
   for(i=count2-1;i<2*count2+2;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,248);
   for(i=2*count2+2;i<2*count2+2+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,249);
   for(i=2*count2+2+hcount;i<3*count2-1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineColor(fid,250);
   for(i=3*count2-1;i<count;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphDraw2f(fid,x[0],y[0]);
   return 1;
}
int gphRoundedRectangleLowered_o(int fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount;
   float x[450],y[450];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow;
   double ang,dang;
   int i,j,r,g,b;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   hcount = count/2;
   kgGetDefaultRGB(clr,&r,&g,&b);
   r=g=b=255;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   vorg = vv;
   vv =0.9;
   vmid = 0.9;
   vlow = 0.0;
//   vv = 0.9;
//   if(vlow > 0.3 ) vlow=0.3;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   x[0]=dl +xp;
   y[0]= yp;
   for(i=1;i<count1;i++) {
     ang +=dang;
     x[i]= dl*cos(ang)+xp;
     y[i]= dl*sin(ang)+yp;
   }
   x[count]= xp;
   y[count]= dl+yp;
   for(i=1;i<count2;i++) {
     x[count+i]= -x[count1-i];
     y[count+i] = y[count1-i];
   }
   for(i=1;i<(count1+count2);i++) {
     x[count+count1+i]= x[2*count1-i];
     y[count+count1+i] = -y[2*count1-i];
   }
   for(i=0;i<4*count1;i++) {
     x[i] +=xo;
     y[i] +=yo;
   }
   HSVtoRGB(&red,&green,&blue,h,s,vlow);
   gphChangeColor(fid,248,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid);
   gphChangeColor(fid,249,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vv);
   gphChangeColor(fid,250,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,0.0);
   gphChangeColor(fid,251,(int)red,(int)green,(int)blue);
   gphLineWidth(fid,3);
   gphLineColor(fid,249);
   gphMove2f(fid,x[0],y[0]);
   for(i=1;i<hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineWidth(fid,2);
   gphLineColor(fid,248);
   for(i=hcount;i<count1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineWidth(fid,4);
   gphLineColor(fid,251);
   for(i=count1;i<2*count1+1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineWidth(fid,3);
   gphLineColor(fid,248);
   for(i=2*count1+1;i<2*count1+1+hcount+1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineWidth(fid,2);
   gphLineColor(fid,249);
   for(i=2*count1+1+hcount+1;i<3*count1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineWidth(fid,4);
   gphLineColor(fid,250);
   for(i=3*count1;i<4*count1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphDraw2f(fid,x[0],y[0]);
   return 1;
}
 int uiArc_fill(DIG *G,float xo,float yo,float r,float ang1,float ang2,int flag
      ,int color)
  {
    double a1,a2,a3,rd,th,t;
    int n,i,full=0;
    float *x,*y;
    if( fabs((double)(ang1-ang2))> 359.0) full=1;
    a1 = ang1*3.14159265/180.0;
    a2 = ang2*3.14159265/180.0;
    th= 5.0*3.14159265/180.0;
    n = fabs(a2-a1)/th+0.5;
    x = Malloc(sizeof(float)*(n+2));
    y = Malloc(sizeof(float)*(n+2));
    if(y==NULL) {
      printf("Error: In mem allocation..\n");
      exit(0);
    }
    th = (a2-a1)/n;
    t =a1;
    i=0;
    *(x+i)= r*cos(t)+xo;
    *(y+i) = r*sin(t)+yo;
    for(i=1;i<n;i++){
      t +=th;
      *(x+i)= r*cos(t)+xo;
      *(y+i) = r*sin(t)+yo;
    }
    t =a2;
    if(!full) {
      *(x+n)= r*cos(t)+xo;
      *(y+n) = r*sin(t)+yo;
       n++;
      *(x+n)= xo;
      *(y+n) = yo;
      n++;
    }
    kgPolyFill(G,n,x,y,flag,color);
    free(x),free(y);
    return(8*n+14);
  }
int uiProcess_arc_fill (DIG *G,float *xo,float *yo,int fil_col)
 {
    float ang1,ang2,r;
    float x1,y1,x2,y2;
    gscanf(G->D,"Give Arc Radius=%10f",&m_radius);
    y1=*yo;
    x1= *xo+m_radius;
//    put_message(150,50,(char *)"Fix First Point on Arc:");  
//    gbell(); 
    kgArcCursor(G,&x1,&y1,xo,yo);
//    clear_message();  
//    put_message(150,50,(char *)"Fix Second Point on Arc:");  
//    gbell(); 
    y2=*yo;
    x2= *xo+m_radius;
    kgArcCursor(G,&x2,&y2,xo,yo);
//    clear_message();  
    r= m_radius;
    ang1= atan2( (double)(y1-*yo),(double)(x1-*xo))/3.14159265*180.0; 
    ang2= atan2( (double)(y2-*yo),(double)(x2-*xo))/3.14159265*180.0; 
    if(ang1<0.) ang1+=360.;
    if(ang2<0.) ang2+=360.;
    if(ang1==ang2) ang2=ang1+360;
    if( ang2<ang1) ang2+=360;
    return(uiArc_fill(G, *xo,*yo,r,ang1,ang2,1L,fil_col)); 
 }
void kgrev_shade_o(DIG *G,FILE *fp)
{
  int i;

  int n,l;
  float *x,*y,*z,*v;
  kgDC *dc;
  dc= G->dc;
  FREAD(fp,(void *)&n,4);
  l = sizeof(float)*n;
  x = (float *)Malloc(l);
  y = (float *)Malloc(l);
  z = (float *)Malloc(l);
  v = (float *)Malloc(l);
  FREAD(fp,(void *)x,l);
  FREAD(fp,(void *)y,l);
  FREAD(fp,(void *)z,l);
  uiwrite_bf(G,&P,1);
  uiwrite_bf(G,&S3,1);
  uiwrite_bf(G,&n,4);
  for(i=0;i<n;i++)
   {
    *(x+i) = uicnv_x(dc,x[i]);
    *(y+i) = uicnv_y(dc,y[i]);
   }
  kgcomputeV(G,n,x,y,z);
  uiwrite_bf(G,x,4*n);
  uiwrite_bf(G,y,4*n);
  uiwrite_bf(G,z,4*n);
  if (!G->D_ON) return;
  if (dc->ZBUFF == 1) 
  ui_3_godr_fill(G,n,x,y,z,v);
  else
  ui_godr_fill(G,n,x,y,v);
  free(x),free(y),free(z),free(v);
}
void kgrev_move3f(DIG *G,FILE *fp) 
 {
     float x,y,z;
     kgDC *dc;
     dc = G->dc;
     FREAD(fp,(void *)&x,4);
     FREAD(fp,(void *)&y,4);
     FREAD(fp,(void *)&z,4);
     uiwrite_bf(G,&L,1);
     uiwrite_bf(G,&M3,1);
     x = uicnv_x(dc,x);
     y = uicnv_y(dc,y);
     uiwrite_bf(G,&x,4);
     uiwrite_bf(G,&y,4);
     uiwrite_bf(G,&z,4);
     if (!G->D_ON) return;
     if (dc->ZBUFF == 1)
     ui_3_move(G,x , y , z);
     else
     ui_move(G,x , y);
 }
void kgrev_draw3f(DIG *G,FILE *fp)
 {
     float x,y,z;
     kgDC *dc;
     dc = G->dc;
     FREAD(fp,(void *)&x,4);
     FREAD(fp,(void *)&y,4);
     FREAD(fp,(void *)&z,4);

     uiwrite_bf(G,&L,1);
     uiwrite_bf(G,&D3,1);
     x = uicnv_x(dc,x);
     y = uicnv_y(dc,y);
     uiwrite_bf(G,&x,4);
     uiwrite_bf(G,&y,4);
     uiwrite_bf(G,&z,4);
     if (!G->D_ON) return;
     if (dc->ZBUFF == 1)
     ui_3_draw(G,x , y , z);
     else
     ui_draw(G,x,y);
 }
void kgrev_godr_fill3f(DIG *G,FILE *fp)
{
  int i;

  int n,l;
  float *x,*y,*z,*v;
     kgDC *dc;
     dc = G->dc;
  FREAD(fp,(void *)&n,4);
  l = sizeof(float)*n;
  x = (float *)Malloc(l);
  y = (float *)Malloc(l);
  z = (float *)Malloc(l);
  v = (float *)Malloc(l);
  FREAD(fp,(void *)x,l);
  FREAD(fp,(void *)y,l);
  FREAD(fp,(void *)z,l);
  FREAD(fp,(void *)v,l);
  uiwrite_bf(G,&P,1);
  uiwrite_bf(G,&G3,1);
  uiwrite_bf(G,&n,4);
  for(i=0;i<n;i++)
   {
    *(x+i) = uicnv_x(dc,x[i]);
    *(y+i) = uicnv_y(dc,y[i]);
   }
  uiwrite_bf(G,x,4*n);
  uiwrite_bf(G,y,4*n);
  uiwrite_bf(G,z,4*n);
  uiwrite_bf(G,v,4*n);
  if (!G->D_ON) return;
  if (dc->ZBUFF == 1) ui_3_godr_fill(G,n,x,y,z,v);
  else ui_godr_fill(G,n,x,y,v);
  free(x),free(y),free(z),free(v);
}
void kgrev_poly_fill3f(DIG *G,FILE *fp)
 {
  int i;
  int n,l,flag,color;
  float *x,*y,*z;
     kgDC *dc;
     dc = G->dc;
  FREAD(fp,(void *)&n,4);
  l = sizeof(float)*n;
  x = (float *)Malloc(l);
  y = (float *)Malloc(l);
  z = (float *)Malloc(l);
  FREAD(fp,(void *)x,l);
  FREAD(fp,(void *)y,l);
  FREAD(fp,(void *)z,l);
  FREAD(fp,(void *)&flag,4);
  FREAD(fp,(void *)&color,4);

  uiwrite_bf(G,&P,1);
  uiwrite_bf(G,&P3,1);
  for(i=0;i<n;i++) {
       *(x+i) = uicnv_x(dc,x[i]);
       *(y+i) = uicnv_y(dc,y[i]);
  }
  uiwrite_bf(G,&n,4);
  uiwrite_bf(G,x,4*n);
  uiwrite_bf(G,y,4*n);
  uiwrite_bf(G,z,4*n);
  uiwrite_bf(G,&flag,4);
  uiwrite_bf(G,&color,4);
  if (!G->D_ON) return;
  if (dc->ZBUFF == 1) ui_3_panel(G,x,y,z,(int)color,flag,n);
  else ui_panel(G,x,y,(int)color,flag,n);
  free(x),free(y),free(z);
}
void kgrev_box_fill3f(DIG *G,FILE *fp)
 {
   float x1,y1,z1,x2,y2,z2;
   int fl,ib;
     kgDC *dc;
     dc = G->dc;
   FREAD(fp,(void *)&x1,4);
   FREAD(fp,(void *)&y1,4);
   FREAD(fp,(void *)&z1,4);
   FREAD(fp,(void *)&x2,4);
   FREAD(fp,(void *)&y2,4);
   FREAD(fp,(void *)&z2,4);
   FREAD(fp,(void *)&fl,4);
   FREAD(fp,(void *)&ib,4);
      x1 = uicnv_x(dc,x1);
      y1 = uicnv_y(dc,y1);
      x2 = uicnv_x(dc,x2);
      y2 = uicnv_y(dc,y2);
      uiwrite_bf(G,&P,1);
      uiwrite_bf(G,&B3,1);
      uiwrite_bf(G,&x1,4);
      uiwrite_bf(G,&y1,4);
      uiwrite_bf(G,&z1,4);
      uiwrite_bf(G,&x2,4);
      uiwrite_bf(G,&y2,4);
      uiwrite_bf(G,&z2,4);
      uiwrite_bf(G,&fl,4);
      uiwrite_bf(G,&ib,4);
      if (!G->D_ON) return;
      if (dc->ZBUFF == 1)
      ui_3_box_fill(G,x1,y1,z1,x2,y2,z2,fl,ib);
      else
      ui_box_fill(G,x1,y1,x2,y2,fl,ib);
 }
 void uirev_file(FILE * pf,DIG *G,float sx,float fx,float sy, float fy)
  {
    char ch,ch2;
    int clr,n,i,flag;
    int no,ir,ig,ib;
    float x,y;
    float x1,y1,x2,y2;
    float *xx,*yy,*p;
    float FMO[16];
    char imgfile[200];
    int color;
    unsigned char cntl;
    unsigned char *tx;
    float xeo,yeo,zeo;
    float clip_maxo,clip_mino;
    int Clips=0;
    kgDC *dc;
    kgWC *wc;
    dc = G->dc;
    wc = G->wc;
    dc->Sx=sx,dc->Sy=sy,dc->Fx=fx,dc->Fy=fy;
    for(i=0;i<16;i++) FMO[i]=dc->FM[i];
    for(i=0;i<16;i++) dc->FM[i]=0;
    xeo= dc->xe,yeo = dc->ye,zeo = dc->ze,clip_mino= dc->clip_min, clip_maxo = dc->clip_max;
    while( Fread(pf,(void *)&ch,1) > 0) 
    {
    switch((int)ch)
    {
     case 'z' :
        for(i=0;i<16;i++) dc->FM[i]=0; dc->FM[0]=1,dc->FM[5]=1,dc->FM[10]=1,dc->FM[15]=1;
        dc->clip_min=0; dc->clip_max=65000;
        ui_clip_const(G,dc->clip_min,dc->clip_max);
        dc->TIFF=1;
        dc->PROJ=0;
//        printf("Zbuffer\n");
        kgSetZbuffer(G,1);
        break;
        
     case 'l' :
      FREAD(pf,(void *)&ch2,1);
      switch((int)ch2)
      {
        case 'm':
                FREAD(pf,(void *)&x,4);
                FREAD(pf,(void *)&y,4);
                kgMove2f(G,uicnv_x(dc,x),uicnv_y(dc,y));
         break;
        case 'd':
                FREAD(pf,(void *)&x,4);
                FREAD(pf,(void *)&y,4);
                kgDraw2f(G,uicnv_x(dc,x),uicnv_y(dc,y));
                break;
        case 'l':
                FREAD(pf,(void *)&x1,4);
                FREAD(pf,(void *)&y1,4);
                FREAD(pf,(void *)&x2,4);
                FREAD(pf,(void *)&y2,4);
/*              draw_line(cnv_x(x1),cnv_y(y1),cnv_x(x2),cnv_y(y2));*/
        break;
        case 'i':
                FREAD(pf,(void *)imgfile,100);
                FREAD(pf,(void *)&x1,4);
                FREAD(pf,(void *)&y1,4);
                FREAD(pf,(void *)&x2,4);
                FREAD(pf,(void *)&y2,4);
//                printf("Drawing Image: %-s\n",imgfile);
                kgDrawImage(G,imgfile,uicnv_x(dc,x1),uicnv_y(dc,y1),
                             uicnv_x(dc,x2),uicnv_y(dc,y2));
        break;
        case 's':
                FREAD(pf,(void *)&clr,4);
                kgLineStyle(G,clr);
        break;
        case 'y':
//                resetstyle();
        break;
        case 'w':
                FREAD(pf,(void *)&clr,4);
                kgLineWidth(G,clr);
                break;
        case 'c':
                FREAD(pf,(void *)&color,4);
                kgLineColor(G,color);
        break;
        case 'r': 
                FREAD(pf,(void *)&x1,4);
                FREAD(pf,(void *)&y1,4);
                FREAD(pf,(void *)&x2,4);
                kgCircle2f(G,uicnv_x(dc,x1),uicnv_y(dc,y1),uicnv_x(dc,x2));
        break;
        case 'M': kgrev_move3f(G,pf);
         break;
        case 'D': kgrev_draw3f(G,pf);
         break;
        default :
               printf(" Wrong Code in line :%c%c \n",ch,ch2);
               exit(0);
      }
      break;
     case 'm' :
     FREAD(pf,(void *)&ch2,1);
      switch((int)ch2)
      {
        case 'k':
                FREAD(pf,(void *)&x,4);
                FREAD(pf,(void *)&y,4);
                kgMarker2f(G,uicnv_x(dc,x),uicnv_y(dc,y));
         break;
        case 't':
                FREAD(pf,(void *)&clr,4);
                kgMarkerType(G,clr);
         break;
        default :
               printf(" Wrong Code in mark :%c%c \n",ch,ch2);
               exit(0);
      }
     break;
     case 't' :
     FREAD(pf,(void *)&ch2,1);
      switch((int)ch2)
      {
        case 'w':
                FREAD(pf,(void *)&n,4);
                tx= (unsigned char *) Malloc((n+1)*sizeof(unsigned));
                FREAD(pf,(void *)tx,n);
                *(tx+n)='\0';
                kgWriteText(G,(char *)tx);
                free(tx);
                break;
        case 'o': 
                FREAD(pf,(void *)&clr,4);
//                set_txtbold(clr);
                break;
        case 'h':
                FREAD(pf,(void *)&clr,4);
//                set_htxtbold(clr);
                break;
        case 'c':
                FREAD(pf,(void *)&clr,4);
                kgTextColor(G,clr);
                break;
        case 'l':
                FREAD(pf,(void *)&clr,4);
//                txtfill(clr);
                break;
        case 'k':
                FREAD(pf,(void *)&clr,4);
//                txtbkgr(clr);
                break;
        case 'p':
                FREAD(pf,(void *)&clr,4);
//                txtpattern(clr);
                break;
        case 'b':
                FREAD(pf,(void *)&clr,4);
//                txtbodr(clr);
                break;
        case 'f':
                FREAD(pf,(void *)&clr,4);
                kgTextFont(G,clr);
                break;
        case 'a':
                FREAD(pf,(void *)&x,4);
//                txtslant(x);
                break;
        case 's':
                FREAD(pf,(void *)&y,4);
                FREAD(pf,(void *)&x1,4);
                FREAD(pf,(void *)&x2,4);
                FREAD(pf,(void *)&x,4);
                FREAD(pf,(void *)&y1,4);
                FREAD(pf,(void *)&y2,4);
                uirevtxtsize(G,y*fy,x1*fx,x2*fx,x*fx,y1*fy,y2*fy);
                break;
        case 'r':
                FREAD(pf,(void *)&x1,4);
                kgTextAngle(G,x1);
                break;
        case 'x':
                FREAD(pf,(void *)&clr,4);
//                set_prtxt(clr);
                break;
        default :
               printf(" Wrong Code in text :%c%c \n",ch,ch2);
               exit(0);
       }
      break;
     case 'p' :
     FREAD(pf,(void *)&ch2,1);
      switch((int)ch2)
      {
        case 'b':
                FREAD(pf,(void *)&x1,4);
                FREAD(pf,(void *)&y1,4);
                FREAD(pf,(void *)&x2,4);
                FREAD(pf,(void *)&y2,4);
                FREAD(pf,(void *)&color,4);
                FREAD(pf,(void *)&flag,4);
                kgBoxFill(G,uicnv_x(dc,x1),uicnv_y(dc,y1),uicnv_x(dc,x2),uicnv_y(dc,y2),color,flag);
                break;
        case 'r': 
                uiwin_circlefill(G);  // Will not work
                break;
        case 'p':
                FREAD(pf,(void *)&n,4);
                xx=(float *)Malloc(sizeof(float)*n);
                yy=(float *)Malloc(sizeof(float)*n);
                check_mem_alloc(yy);
                FREAD(pf,(void *)xx,4*n);
                FREAD(pf,(void *)yy,4*n);
                FREAD(pf,(void *)&flag,4);
                FREAD(pf,(void *)&color,4);
                for(i=0;i<n;i++)
                {
                  xx[i] =uicnv_x(dc,xx[i]); 
                  yy[i] =uicnv_y(dc,yy[i]);
                }
                kgPolyFill(G,n,xx,yy,flag,color);
                free(xx),free(yy);
                break;
        case 'g': 
                FREAD(pf,(void *)&n,4);
                xx=(float *)Malloc(sizeof(float)*n);
                yy=(float *)Malloc(sizeof(float)*n);
                p=(float *)Malloc(sizeof(float)*n);
                check_mem_alloc(p);
                FREAD(pf,(void *)xx,4*n);
                FREAD(pf,(void *)yy,4*n);
                FREAD(pf,(void *)p,4*n);
                for(i=0;i<n;i++)
                {
                  xx[i] =uicnv_x(dc,xx[i]); 
                  yy[i] =uicnv_y(dc,yy[i]);
                }
                kgGouraudFill(G,n,xx,yy,p);
                free(xx),free(yy);
                free(p);
                break;
        case 's':
                {
                float p1,p2;
                int n,ib;
                FREAD(pf,(void *)&p2,4);
                FREAD(pf,(void *)&p1,4);
                FREAD(pf,(void *)&n,4);
                FREAD(pf,(void *)&ib,4);
                kgSetGouraudParams(G,p2,p1,n,ib);
                }
                break;
        case 'h':
                {
                float p1,p2;
                int sclr,n,ib;
                FREAD(pf,(void *)&p2,4);
                FREAD(pf,(void *)&p1,4);
                FREAD(pf,(void *)&sclr,4);
                FREAD(pf,(void *)&n,4);
                FREAD(pf,(void *)&ib,4);
                kgSetShadeParams(G,p2,p1,sclr,n,ib);
                }
                break;
        case 'B' : kgrev_box_fill3f(G,pf);
                   break;

        case 'G' : kgrev_godr_fill3f(G,pf);
                   break;  
        case 'S' : kgrev_shade_o(G,pf);
                   break;  
        case 'P' : kgrev_poly_fill3f(G,pf);
                   break;  
        case 'T' : //win_transperancy();
                   break;  
        default :
               printf(" Wrong Code in fill :%c%c \n",ch,ch2);
               exit(0);
       }
      break;
     case 's' :
     FREAD(pf,(void *)&ch2,1);
      switch((int)ch2)
      {
        case 'c':
                FREAD(pf,(void *)&x1,4);
                FREAD(pf,(void *)&y1,4);
                FREAD(pf,(void *)&x2,4);
                FREAD(pf,(void *)&y2,4);
                kgSetClip(G,uicnv_x(dc,x1),uicnv_y(dc,y1),uicnv_x(dc,x2),uicnv_y(dc,y2));
                Clips++;
                break;
        case 'p':
                kgPopClip(G);
                Clips--;
                break;
      }
      break;
     case 'c' :
     FREAD(pf,(void *)&ch2,1);
      switch((int)ch2)
      {
        case 'h':
                FREAD(pf,(void *)&no,4);
                FREAD(pf,(void *)&ir,4);
                FREAD(pf,(void *)&ib,4);
                FREAD(pf,(void *)&ig,4);
                kgChangeColor(G,no,ir,ig,ib);
               break;
      }
      break;
     default :
               printf(" Wrong Code:%c%c \n",ch,ch2);
               exit(0);
     }
    }
    for(i=0;i<16;i++) dc->FM[i]=FMO[i];
    if(Clips != 0) printf("IN rev_file : Clips= %d\n",Clips);
// TCB
//    for(i=0;i<Clips;i++) kgPopClip(G);
/*
    dc->xe= xeo,dc->ye = yeo,dc->ze = zeo,dc->clip_min= clip_mino, dc->clip_max = clip_maxo;
*/
    
 }
void uicnv_move3f(DIG *G,FILE *fp) 
 {
     float x,y,z;
     kgDC *dc;
     dc = G->dc;
     FREAD(fp,(void *)&x,4);
     FREAD(fp,(void *)&y,4);
     FREAD(fp,(void *)&z,4);
     uiwrite_bf(G,&L,1);
     uiwrite_bf(G,&M3,1);
     kgtransfrm(dc,x,y,z);
     kgprojection(dc,dc->trnstr);
     x = uicnv_x(dc,dc->newstr.xstr);
     y = uicnv_y(dc,dc->newstr.ystr);
     z = uicnv_z(dc,dc->newstr.zstr);
     uiwrite_bf(G,&x,4);
     uiwrite_bf(G,&y,4);
     uiwrite_bf(G,&z,4);
     if (!G->D_ON) return;
 }
void uicnv_draw3f(DIG *G,FILE *fp)
 {
     float x,y,z;
     kgDC *dc;
     dc = G->dc;
     FREAD(fp,(void *)&x,4);
     FREAD(fp,(void *)&y,4);
     FREAD(fp,(void *)&z,4);

     uiwrite_bf(G,&L,1);
     uiwrite_bf(G,&D3,1);
     kgtransfrm(dc,x,y,z);
     kgprojection(dc,dc->trnstr);
     x = uicnv_x(dc,dc->newstr.xstr);
     y = uicnv_y(dc,dc->newstr.ystr);
     z = uicnv_z(dc,dc->newstr.zstr);
     uiwrite_bf(G,&x,4);
     uiwrite_bf(G,&y,4);
     uiwrite_bf(G,&z,4);
     if (!G->D_ON) return;
 }
void uicnv_godr_fill3f(DIG *G,FILE *fp)
{
  int i;

  int n,l;
  float *x,*y,*z,*v;
  kgDC *dc;
  dc = G->dc;
  FREAD(fp,(void *)&n,4);
  l = sizeof(float)*n;
  x = (float *)Malloc(l);
  y = (float *)Malloc(l);
  z = (float *)Malloc(l);
  v = (float *)Malloc(l);
  FREAD(fp,(void *)x,l);
  FREAD(fp,(void *)y,l);
  FREAD(fp,(void *)z,l);
  FREAD(fp,(void *)v,l);
  uiwrite_bf(G,&P,1);
  uiwrite_bf(G,&G3,1);
  uiwrite_bf(G,&n,4);
  for(i=0;i<n;i++)
   {
    kgtransfrm(dc,*(x+i), *(y+i),*(z+i));
    kgprojection(dc,dc->trnstr);
    *(x+i) = uicnv_x(dc,dc->newstr.xstr);
    *(y+i) = uicnv_y(dc,dc->newstr.ystr);
    *(z+i) = uicnv_z(dc,dc->newstr.zstr);
   }
  uiwrite_bf(G,x,4*n);
  uiwrite_bf(G,y,4*n);
  uiwrite_bf(G,z,4*n);
  uiwrite_bf(G,v,4*n);
  free(x),free(y),free(z),free(v);
  if (!G->D_ON) return;
}
void uicnv_shade_o(DIG *G,FILE *fp)
{
  int i;

  int n,l;
  float *x,*y,*z,*v;
  kgDC *dc;
  dc = G->dc;
  FREAD(fp,(void *)&n,4);
  l = sizeof(float)*n;
  x = (float *)Malloc(l);
  y = (float *)Malloc(l);
  z = (float *)Malloc(l);
  FREAD(fp,(void *)x,l);
  FREAD(fp,(void *)y,l);
  FREAD(fp,(void *)z,l);
  uiwrite_bf(G,&P,1);
  uiwrite_bf(G,&G3,1);
  uiwrite_bf(G,&n,4);
  for(i=0;i<n;i++)
   {
    kgtransfrm(dc,*(x+i), *(y+i),*(z+i));
    *(x+i) = dc->trnstr.xstr;
    *(y+i) = dc->trnstr.ystr;
    *(z+i) = dc->trnstr.zstr;
   }
  v = kgcomputeV(G,n,x,y,z);
  for(i=0;i<n;i++)
   {
    dc->trnstr.xstr = x[i];
    dc->trnstr.ystr = y[i];
    dc->trnstr.zstr = z[i];
    kgprojection(dc,dc->trnstr);
    *(x+i) = uicnv_x(dc,dc->newstr.xstr);
    *(y+i) = uicnv_y(dc,dc->newstr.ystr);
    *(z+i) = uicnv_z(dc,dc->newstr.zstr);
   }
  uiwrite_bf(G,x,4*n);
  uiwrite_bf(G,y,4*n);
  uiwrite_bf(G,z,4*n);
  uiwrite_bf(G,v,4*n);
  free(x),free(y),free(z),free(v);
  if (!G->D_ON) return;
}
void uicnv_poly_fill3f(DIG *G,FILE *fp)
 {
  int i;
  int n,l,flag,color;
  float *x,*y,*z;
  kgDC *dc;
  dc = G->dc;
  FREAD(fp,(void *)&n,4);
  l = sizeof(float)*n;
  x = (float *)Malloc(l);
  y = (float *)Malloc(l);
  z = (float *)Malloc(l);
  FREAD(fp,(void *)x,l);
  FREAD(fp,(void *)y,l);
  FREAD(fp,(void *)z,l);
  FREAD(fp,(void *)&flag,4);
  FREAD(fp,(void *)&color,4);

  uiwrite_bf(G,&P,1);
  uiwrite_bf(G,&P3,1);
  for(i=0;i<n;i++) {
       kgtransfrm(dc,*(x+i),*(y+i),*(z+i));
       kgprojection(dc,dc->trnstr);
       *(x+i) = uicnv_x(dc,dc->newstr.xstr);
       *(y+i) = uicnv_y(dc,dc->newstr.ystr);
       *(z+i) = uicnv_z(dc,dc->newstr.zstr);
  }
  uiwrite_bf(G,&n,4);
  uiwrite_bf(G,x,4*n);
  uiwrite_bf(G,y,4*n);
  uiwrite_bf(G,z,4*n);
  uiwrite_bf(G,&flag,4);
  uiwrite_bf(G,&color,4);
  free(x),free(y),free(z);
  if (!G->D_ON) return;
}
void uicnv_box_fill3f(DIG *G,FILE *fp)
 {
   float x1,y1,z1,x2,y2,z2;
   int fl,ib;
   kgDC *dc;
   dc = G->dc;
   FREAD(fp,(void *)&x1,4);
   FREAD(fp,(void *)&y1,4);
   FREAD(fp,(void *)&z1,4);
   FREAD(fp,(void *)&x2,4);
   FREAD(fp,(void *)&y2,4);
   FREAD(fp,(void *)&z2,4);
   FREAD(fp,(void *)&fl,4);
   FREAD(fp,(void *)&ib,4);
      kgtransfrm(dc,x1,y1,z1);
      kgprojection(dc,dc->trnstr);
      x1 = uicnv_x(dc,dc->newstr.xstr);
      y1 = uicnv_y(dc,dc->newstr.ystr);
      z1 = uicnv_z(dc,dc->newstr.zstr);
      kgtransfrm(dc,x2,y2,z2);
      kgprojection(dc,dc->trnstr);
      x2 = uicnv_x(dc,dc->newstr.xstr);
      y2 = uicnv_y(dc,dc->newstr.ystr);
      z2 = uicnv_z(dc,dc->newstr.zstr);
      uiwrite_bf(G,&P,1);
      uiwrite_bf(G,&B3,1);
      uiwrite_bf(G,&x1,4);
      uiwrite_bf(G,&y1,4);
      uiwrite_bf(G,&z1,4);
      uiwrite_bf(G,&x2,4);
      uiwrite_bf(G,&y2,4);
      uiwrite_bf(G,&z2,4);
      uiwrite_bf(G,&fl,4);
      uiwrite_bf(G,&ib,4);
      if (!G->D_ON) return;
 }
 void uicnv_file(DIG *G,FILE *pf)
  {
    float sx=0.,sy=0.,fx=1.,fy=1.;
    char ch,ch2;
    int clr,n,i,flag;
    int no,ir,ig,ib;
    float x,y;
    float x1,y1,x2,y2;
    float *xx,*yy,*p;
    float FMO[16];
    int color;
    unsigned char cntl;
    unsigned char *tx;
    float xeo,yeo,zeo;
    float clip_maxo,clip_mino;
    kgDC *dc;
    kgWC *wc;
    dc = G->dc;
    wc = G->wc;
    dc->Sx=0.,dc->Sy=0.,dc->Fx=1.,dc->Fy=1.;
    for(i=0;i<16;i++) FMO[i]=dc->FM[i];
    xeo= dc->xe,yeo = dc->ye,zeo = dc->ze,clip_mino= dc->clip_min, clip_maxo = dc->clip_max;
    while( Fread(pf,(void *)&ch,1) > 0) 
    {
    switch((int)ch)
    {
     case 'z' :
/*
        pop_clip();
        set_window(W_x1,W_y1,W_x2,W_y2);
        clip_limit(W_x1,W_y1,W_x2,W_y2);
        FREAD(pf,(void *)FM,64);
        FREAD(pf,(void *)&xe,4);
        FREAD(pf,(void *)&ye,4);
        FREAD(pf,(void *)&ze,4);
        FREAD(pf,(void *)&zc,4);
        FREAD(pf,(void *)&clip_min,4);
        FREAD(pf,(void *)&clip_max,4);
        t_clip_const(clip_min,clip_max);
*/
        kgSetZbuffer(G,1);
        break;
        
     case 'l' :
      FREAD(pf,(void *)&ch2,1);
      switch((int)ch2)
      {
        case 'm':
                FREAD(pf,(void *)&x,4);
                FREAD(pf,(void *)&y,4);
                kgMove2f(G,uicnv_x(dc,x),uicnv_y(dc,y));
         break;
        case 'd':
                FREAD(pf,(void *)&x,4);
                FREAD(pf,(void *)&y,4);
                kgDraw2f(G,uicnv_x(dc,x),uicnv_y(dc,y));
                break;
        case 'l':
                FREAD(pf,(void *)&x1,4);
                FREAD(pf,(void *)&y1,4);
                FREAD(pf,(void *)&x2,4);
                FREAD(pf,(void *)&y2,4);
/*              draw_line(cnv_x(x1),cnv_y(y1),cnv_x(x2),cnv_y(y2));*/
        break;
        case 's':
                FREAD(pf,(void *)&clr,4);
                kgLineStyle(G,clr);
        break;
        case 'y':
//                resetstyle();
        break;
        case 'w':
                FREAD(pf,(void *)&clr,4);
                kgLineWidth(G,clr);
                break;
        case 'c':
                FREAD(pf,(void *)&color,4);
                kgLineColor(G,clr);
        break;
        case 'r': 
                FREAD(pf,(void *)&x1,4);
                FREAD(pf,(void *)&y1,4);
                FREAD(pf,(void *)&x2,4);
                kgCircle2f(G,uicnv_x(dc,x1),uicnv_y(dc,y1),uicnv_x(dc,x2));
        break;
        case 'M': uicnv_move3f(G,pf);
         break;
        case 'D': uicnv_draw3f(G,pf);
         break;
       /* case 
                FREAD(pf,(void *)&x1,4);
                FREAD(pf,(void *)&y1,4);
                FREAD(pf,(void *)&x2,4);
                FREAD(pf,(void *)&color,4);*/
                /*circle_fill(cnv_x(x1),cnv_y(y1),cnv_x(x2),color);*/
        default :
               printf(" Wrong Code in line :%c%c \n",ch,ch2);
               exit(0);
      }
      break;
     case 'm' :
     FREAD(pf,(void *)&ch2,1);
      switch((int)ch2)
      {
        case 'k':
                FREAD(pf,(void *)&x,4);
                FREAD(pf,(void *)&y,4);
                kgMarker2f(G,uicnv_x(dc,x),uicnv_y(dc,y));
         break;
        case 't':
                FREAD(pf,(void *)&clr,4);
                kgMarkerType(G,clr);
         break;
        default :
               printf(" Wrong Code in mark :%c%c \n",ch,ch2);
               exit(0);
      }
     break;
     case 't' :
     FREAD(pf,(void *)&ch2,1);
      switch((int)ch2)
      {
        case 'w':
                FREAD(pf,(void *)&n,4);
                tx= (unsigned char *) Malloc((n+1)*sizeof(unsigned));
                FREAD(pf,(void *)tx,n);
                *(tx+n)='\0';
                kgWriteText(G,(char *)tx);
                free(tx);
                break;
        case 'o': 
                FREAD(pf,(void *)&clr,4);
                break;
        case 'h':
                FREAD(pf,(void *)&clr,4);
                break;
        case 'c':
                FREAD(pf,(void *)&clr,4);
                kgTextColor(G,clr);
                break;
        case 'l':
                FREAD(pf,(void *)&clr,4);
                break;
        case 'k':
                FREAD(pf,(void *)&clr,4);
                break;
        case 'p':
                FREAD(pf,(void *)&clr,4);
                break;
        case 'b':
                FREAD(pf,(void *)&clr,4);
                break;
        case 'f':
                FREAD(pf,(void *)&clr,4);
                kgTextFont(G,clr);
                break;
        case 'a':
                FREAD(pf,(void *)&x,4);
                break;
        case 's':
                FREAD(pf,(void *)&y,4);
                FREAD(pf,(void *)&x1,4);
                FREAD(pf,(void *)&x2,4);
                FREAD(pf,(void *)&x,4);
                FREAD(pf,(void *)&y1,4);
                FREAD(pf,(void *)&y2,4);
                uirevtxtsize(G,y*fy,x1*fx,x2*fx,x*fx,y1*fy,y2*fy);
                break;
        case 'r':
                FREAD(pf,(void *)&x1,4);
                kgTextAngle(G,x1);
                break;
        case 'x':
                FREAD(pf,(void *)&clr,4);
//                set_prtxt(clr);
                break;
        default :
               printf(" Wrong Code in text :%c%c \n",ch,ch2);
               exit(0);
       }
      break;
     case 'p' :
     FREAD(pf,(void *)&ch2,1);
      switch((int)ch2)
      {
        case 'b':
                FREAD(pf,(void *)&x1,4);
                FREAD(pf,(void *)&y1,4);
                FREAD(pf,(void *)&x2,4);
                FREAD(pf,(void *)&y2,4);
                FREAD(pf,(void *)&color,4);
                FREAD(pf,(void *)&flag,4);
                kgBoxFill(G,uicnv_x(dc,x1),uicnv_y(dc,y1),uicnv_x(dc,x2),uicnv_y(dc,y2),color,flag);
                
                break;
        case 'r': //win_circlefill();
                break;
        case 'p':
                FREAD(pf,(void *)&n,4);
                xx=(float *)Malloc(sizeof(float)*n);
                yy=(float *)Malloc(sizeof(float)*n);
                check_mem_alloc(yy);
                FREAD(pf,(void *)xx,4*n);
                FREAD(pf,(void *)yy,4*n);
                FREAD(pf,(void *)&flag,4);
                FREAD(pf,(void *)&color,4);
                for(i=0;i<n;i++)
                {
                  xx[i] =uicnv_x(dc,xx[i]); 
                  yy[i] =uicnv_y(dc,yy[i]);
                }
                kgPolyFill(G,n,xx,yy,flag,color);
                free(xx),free(yy);
                break;
        case 'g': 
                FREAD(pf,(void *)&n,4);
                xx=(float *)Malloc(sizeof(float)*n);
                yy=(float *)Malloc(sizeof(float)*n);
                p=(float *)Malloc(sizeof(float)*n);
                check_mem_alloc(p);
                FREAD(pf,(void *)xx,4*n);
                FREAD(pf,(void *)yy,4*n);
                FREAD(pf,(void *)p,4*n);
                for(i=0;i<n;i++)
                {
                  xx[i] =uicnv_x(dc,xx[i]); 
                  yy[i] =uicnv_y(dc,yy[i]);
                }
                kgGouraudFill(G,n,xx,yy,p);
                free(xx),free(yy);
                free(p);
                break;
        case 's':
                {
                float p1,p2;
                int n,ib;
                FREAD(pf,(void *)&p2,4);
                FREAD(pf,(void *)&p1,4);
                FREAD(pf,(void *)&n,4);
                FREAD(pf,(void *)&ib,4);
//                set_godr(p2,p1,n,ib);
                kgSetShadeParams(G,p2,p1,64,n,ib);
                }
                break;
        case 'h':
                {
                float p1,p2;
                int sclr,n,ib;
                FREAD(pf,(void *)&p2,4);
                FREAD(pf,(void *)&p1,4);
                FREAD(pf,(void *)&sclr,4);
                FREAD(pf,(void *)&n,4);
                FREAD(pf,(void *)&ib,4);
                kgSetShadeParams(G,p2,p1,sclr,n,ib);
                }
                break;
        case 'B' : uicnv_box_fill3f(G,pf);
                   break;

        case 'G' : uicnv_godr_fill3f(G,pf);
                   break;  
        case 'S' : uicnv_shade_o(G,pf);
                   break;  
        case 'P' : uicnv_poly_fill3f(G,pf);
                   break;  
        case 'T' : //win_transperancy();
                   break;  
        default :
               printf(" Wrong Code in fill :%c%c \n",ch,ch2);
               exit(0);
       }
      break;
     case 's' :
     FREAD(pf,(void *)&ch2,1);
      switch((int)ch2)
      {
        case 'c':
                FREAD(pf,(void *)&x1,4);
                FREAD(pf,(void *)&y1,4);
                FREAD(pf,(void *)&x2,4);
                FREAD(pf,(void *)&y2,4);
                kgSetClip(G,uicnv_x(dc,x1),uicnv_y(dc,y1),uicnv_x(dc,x2),uicnv_y(dc,y2));
break;
        case 'p':
                kgPopClip(G);
break;
      }
      break;
     case 'c' :
     FREAD(pf,(void *)&ch2,1);
      switch((int)ch2)
      {
        case 'h':
                FREAD(pf,(void *)&no,4);
                FREAD(pf,(void *)&ir,4);
                FREAD(pf,(void *)&ib,4);
                FREAD(pf,(void *)&ig,4);
                kgChangeColor(G,no,ir,ib,ig);
               break;
      }
      break;
     default :
               printf(" Wrong Code:%c%c \n",ch,ch2);
               exit(0);
     }
    }
    for(i=0;i<16;i++) dc->FM[i]=FMO[i];
/*
    xe= xeo,ye = yeo,ze = zeo,clip_min= clip_mino, clip_max = clip_maxo;
*/
    
 }
 void uiclose_file(DIG *G,int *rbuf,char *reviewfile)
  {
   kgDC *dc;
   dc = G->dc;
   if(G->Byte != 0) {
     if(*rbuf== -1) {
        *rbuf = open(reviewfile,O_CREAT|O_BINARY|O_TRUNC|O_RDWR,0666);
        G->OPEN=1;
     }
     G->BACK_UP=1;
     WRITE(*rbuf,(void *)G->Rbuff,G->Byte);
   }
   G->Byte=0;G->R_max=0;
   close(*rbuf);
   *rbuf=-1;
  }
 void uiread_buf(DIG *G, void *ch, int n)
  {
    int i;
    unsigned char *ptr;
    ptr = (unsigned char *)ch;
//    printf("G->R_Byte:G->R_max: %d %d\n",G->R_Byte,G->R_max);
    for(i=0;i<n;i++)
     {
       if ( G->R_Byte>=G->R_max){ G->R_max=read(G->review,(void *)G->Rbuff,B_min);G->R_Byte=0;}
       if(G->R_max ==0) {
//         printf("Trying to read beyond file\n");
//         exit(0);
           return;
       }
       *(ptr+i)=G->Rbuff[G->R_Byte++];
     }
  }
 void uiwrite_file(DIG *G,int *rbuf,char *flname)
  {
   kgDC *dc;
   dc = G->dc;
   if(*rbuf== -1) {
        *rbuf = open(flname,O_CREAT|O_BINARY|O_TRUNC|O_RDWR,0666);
        G->OPEN=1;
   }
   if(G->Byte!=0){
//     printf("Write: %s : %d\n",flname,G->Byte);
     WRITE(*rbuf,(void *)G->Rbuff,G->Byte);
   }
   G->Byte=0;G->R_max=0;
   G->BACK_UP=1;
  }
 void uiwrite_cmds(DIG *G)
  {
   kgDC *dc;
   dc = G->dc;
   G->cmdbuf = open(dc->cmdsfile,O_CREAT|O_BINARY|O_TRUNC|O_RDWR,0666);
   if(G->Byte!=0){
     WRITE(G->cmdbuf,(void *)G->Rbuff,G->Byte);
   }
   G->Byte=0;G->R_max=0;
  }
 void uiread_file(DIG *G,int rbuf)
  {
   G->Byte=read(rbuf,(void *)G->Rbuff,B_min);
  }
void kgClearBuffer(DIG *G)
 {
  kgDC *dc;
  dc = G->dc;
  if(G->OPEN){close(G->rbuf);remove(dc->reviewfile);}
  G->rbuf=-1;
  G->OPEN=1;
  G->rbuf=open(dc->reviewfile,O_CREAT|O_RDWR|O_TRUNC|O_BINARY, S_IREAD|S_IWRITE);
//  t_color=1,t_font=0,t_fill=1,t_bodr=0,t_pattern=15,t_bold=1;
  G->Byte=0;
  /*OPEN=1;*/
  G->BACK_UP=0;
//  G->SET3D=0;
  dc->TIFF = 0;
  kgSetZbuffer(G,0);
//  if(G->D_ON)ui_clrbuf(G);
 }
void kgCallObject(DIG *G,int obj,float sx,float sy,float fx,float fy)
 {
  FILE *pf;
  int cmd,ierr;
  unsigned char ch;
  char flname[250];
  kgDC *dc;
  dc = G->dc;
  if(G->Obj_opn==obj) return;
  sprintf(flname,"%-s/%-d",dc->objdir,obj);
   pf=fopen(flname,"r");
  if(pf !=NULL){
//      rev_obj_file(pf,-sx,fx,-sy,fy);
      uirev_file(pf,G,-sx,fx,-sy,fy);
      fclose(pf);
  }
 }
 void kgCopyObject(DIG *G,int obj) {
   int in,out,bytes;
   char buf[1024];
   char flname[250];
   kgDC *dc;
   dc = G->dc;
   if(G->Obj_opn==obj) return;
   sprintf(flname,"%-s/%-d",dc->objdir,obj);
   in = open( flname,O_RDONLY|O_BINARY,S_IREAD|S_IWRITE);
   if (in <=0) return;
   uiclose_file(G,&(G->rbuf),dc->reviewfile);
   G->rbuf=open(dc->reviewfile,O_RDWR|O_BINARY|O_APPEND, S_IREAD|S_IWRITE);
   while( (bytes=read(in,buf,1024))==1024) WRITE(G->rbuf,buf,1024);
   WRITE(G->rbuf,buf,bytes);
   close(in);
 }
void kgOpenObject(DIG *G,int obj) {
  char flname[250];
  kgDC *dc;
  dc = G->dc;
  if( (obj <=0))
        {
          printf("Wrong Object No: %d \n",obj);
          exit(0);
        }
  if(G->Obj_opn) kgCloseObject(G);
  G->Obj_opn=obj;
  sprintf(flname,"%-s/%-d",dc->objdir,obj);
  uiclose_file(G,&(G->rbuf),dc->reviewfile);
  G->rbuf=open(flname,O_CREAT|O_TRUNC|O_BINARY|O_RDWR, S_IREAD|S_IWRITE);
 }
void kgExtObject(DIG *G,int obj) {
  char flname[250];
  kgDC *dc;
  dc = G->dc;
  if( (obj <=0))
        {
          printf("Wrong Object No: %d \n",obj);
          exit(0);
        }
  if(G->Obj_opn) kgCloseObject(G);
  G->Obj_opn=obj;
  sprintf(flname,"%-s/%-d",dc->objdir,obj);
  uiclose_file(G,&(G->rbuf),dc->reviewfile);
  G->rbuf=open(flname,O_CREAT|O_APPEND|O_BINARY|O_RDWR, S_IREAD|S_IWRITE);
 }
void kgCloseObject(DIG *G) {
  char flname[250];
  int cmd,ierr,obj;
  unsigned char ch;
  kgDC *dc;
  dc = G->dc;
  if(G->Obj_opn==0) return;
  obj = G->Obj_opn;
  sprintf(flname,"%-s/%-d",dc->objdir,obj);
  uiclose_file(G,&(G->rbuf),flname);
  G->Obj_opn=0;
  G->rbuf=open(dc->reviewfile,O_RDWR|O_BINARY|O_APPEND, S_IREAD|S_IWRITE);
 }
void check_and_do(char ch)
 {
 }
void uiwin_move(DIG *G)
{
  int x1,y1;
  float x,y;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&x,4);
  uiread_buf(G,&y,4);
  if(G->D_ON)ui_move(G,x,y);
}

void uiwin_draw(DIG *G)
{
  int x1,y1;
  float x,y;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&x,4);
  uiread_buf(G,&y,4);
  if(G->D_ON)ui_draw(G,x,y);
}

void uiwin_drawimage(DIG *G)
{
  float x1,y1,x2,y2;
  char buff[100];
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,buff,100);
  uiread_buf(G,&x1,4);
  uiread_buf(G,&y1,4);
  uiread_buf(G,&x2,4);
  uiread_buf(G,&y2,4);
  if(G->D_ON)ui_drawimage(G,buff,x1,y1,x2,y2);
}

void uiwin_marker(DIG *G)
{
  int x1,y1;
  float x,y;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&x,4);
  uiread_buf(G,&y,4);
  if(G->D_ON)ui_marker(G,x,y);
}
void uiwin_drawline(DIG *G)
{
  int xa,ya,xb,yb;
  float x1,y1,x2,y2;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&x1,4);
  uiread_buf(G,&y1,4);
  uiread_buf(G,&x2,4);
  uiread_buf(G,&y2,4);
}

void uiwin_boxfill(DIG *G)
{
  float x1,y1,x2,y2;
  int color,ib;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&x1,4);
  uiread_buf(G,&y1,4);
  uiread_buf(G,&x2,4);
  uiread_buf(G,&y2,4);
  uiread_buf(G,&color,4);
  uiread_buf(G,&ib,4);
  if(G->D_ON)ui_box_fill(G,x1,y1,x2,y2,color,ib);
}

void uiwin_circle(DIG *G)
{
  float x1,y1,r;
  int xa,ya,rd;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&x1,4);
  uiread_buf(G,&y1,4);
  uiread_buf(G,&r,4);
//  if(G->D_ON)ui_circle(G,x1,y1,r);
  return;
}

void uiwin_circlefill(DIG *G)
{
  float x1,y1,r;
  unsigned int color;
  int xa,ya,rd;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&x1,4);
  uiread_buf(G,&y1,4);
  uiread_buf(G,&r,4);
  uiread_buf(G,&color,4);
  /*t_circle_fill(xa,ya,rd,color);*/
  return;
}

void uiwin_poly_fill(DIG *G)
{
  int n,flag,color;
  float *x,*y;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&n,4);
  x = (float *) Malloc(sizeof(float)*n);
  y = (float *) Malloc(sizeof(float)*n);
  if(y==NULL) {
    printf("Error: in allocating memory for panel\n");
    exit(0);
  }
  uiread_buf(G,x,4*n);
  uiread_buf(G,y,4*n);
  uiread_buf(G,&flag,4);
  uiread_buf(G,&color,4);
  if(G->D_ON)ui_panel(G,x,y,(int)color,(int)flag,(int)n);
  free(x),free(y);
}
void uiwin_godard_fill(DIG *G)
{
  int n;
  float x[20];
  float y[20],p[20];
  int flag;
  int color;
  
  int i;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&n,4);
  uiread_buf(G,x,4*n);
  uiread_buf(G,y,4*n);
  uiread_buf(G,p,4*n);
  if(G->D_ON)ui_godr_fill(G,n,x,y,p);
}
void uiwin_set_markstyle(DIG *G)
{
  int style;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&style,4);
  ui_smmark(G,(int)(style));
  return;
}
void uiwin_set_color(DIG *G)
{
  int l_color;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&l_color,4);
  ui_ln_clr(G,(int) l_color);
  return;
}
void uiwin_resetstyle(DIG *G)
 {
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
//  t_resetstyle();
 }
void uiwin_set_lnstyle(DIG *G)
{
 int ln_style;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&ln_style,4);
  ui_ln_style(G,(int) ln_style);
  return;
}
void uiwin_set_angle(DIG *G)
{
  float t;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&t,4);
  ui_txt_rot(G,t);
}
void uiwin_txt_size(DIG *G)
{
  float ht,htx;
  float wt,wty;
  float sp,spy;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&ht,4);
  uiread_buf(G,&wt,4);
  uiread_buf(G,&sp,4);
  uiread_buf(G,&htx,4);
  uiread_buf(G,&wty,4);
  uiread_buf(G,&spy,4);
  ui_txt_size(G,ht,wt,sp,htx,wty,spy);
  return;
}
void uiwin_txt_color( DIG *G)
 {
  int t_color;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&t_color,4);
//  ui_txt_clr(G, t_color);
  dc->t_color=t_color;
 }
void uiwin_txt_slant( DIG *G)
 {
  int txt_slant;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&txt_slant,4);
//  t_txt_slant( txt_slant);
 }
void uiwin_txt_fill( DIG *G)
 {
  int t_color;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&t_color,4);
//  t_txtfill( t_color);
//  t_fill=t_color;
 }
void uiwin_txt_bkgr( DIG *G)
 {
  int t_color;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&t_color,4);
//  t_txtbkgr( t_color);
 }
void uiwin_txt_pattern( DIG *G)
 {
  int t_color;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&t_color,4);
//  t_txtpattern( t_color);
 }
void uiwin_txt_bodr( DIG *G)
 {
  int t_color;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&t_color,4);
//  t_txtbodr( t_color);
//  t_bodr=t_color;
 }
void uiwin_txt_font( DIG *G)
 {
  int font;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&font,4);
  ui_txt_font(G,(int)font);
//  t_font=font;
 }
void  uiwin_txtwrt(DIG *G)
{
  int n;
  char cntl;
  char *tx;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&n,4);
  tx = (char *)Malloc((n+1)*sizeof(char));
  uiread_buf(G,tx,n);
  tx[n]='\0';
  if(G->D_ON)ui_txt_wr(G,n,tx);
  free(tx);
}
void uiwin_set_godr(DIG *G)
 {
      float p2; float p1; int n; int ib;
      int startclr=64;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
      uiread_buf(G,&p2,4);
      uiread_buf(G,&p1,4);
      uiread_buf(G,&n,4);
      uiread_buf(G,&ib,4);
      ui_set_godr(G,p2,p1,startclr,n,ib);
 }
void uiwin_set_shade_params(DIG *G)
 {
      float p2; float p1; int startclr;int n; int ib;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
      uiread_buf(G,&p2,4);
      uiread_buf(G,&p1,4);
      uiread_buf(G,&startclr,4);
      uiread_buf(G,&n,4);
      uiread_buf(G,&ib,4);
      ui_set_godr(G,p2,p1,startclr,n,ib);
 }
void uiwin_set_prtxt(DIG *G)
 {
  int pr_txt;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&pr_txt,4);
//  t_prtxt(pr_txt);
 }
void uiwin_set_lnwidth(DIG *G)
{
  int l_width;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&l_width,4);
  ui_lnwidth(G,l_width);
  return;
}
void uiwin_set_txtbold(DIG *G)
{
  int t_bold;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&t_bold,4);
//  t_txtbold(t_bold);
  return;
}
void uiwin_set_htxtbold(DIG *G)
{
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  int t_bold;
  uiread_buf(G,&t_bold,4);
  return;
}
void uiwin_3_move(DIG *G)
{
    float x,y,z;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;

    uiread_buf(G,&x,4);
    uiread_buf(G,&y,4);
    uiread_buf(G,&z,4);
    kgtransfrm(dc,x,y,z);
    kgprojection(dc,dc->trnstr);
    if (!G->D_ON) return;
    if (dc->ZBUFF == 1)
     ui_3_move(G,dc->newstr.xstr,dc->newstr.ystr,dc->newstr.zstr);
    else
    ui_move(G,dc->newstr.xstr,dc->newstr.ystr);
}
void uiwin_3_draw(DIG *G)
{
    float x,y,z;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;

    uiread_buf(G,&x,4);
    uiread_buf(G,&y,4);
    uiread_buf(G,&z,4);
    kgtransfrm(dc,x,y,z);
    kgprojection(dc,dc->trnstr);
    if (!G->D_ON) return;
    if (dc->ZBUFF == 1)
     ui_3_draw(G,dc->newstr.xstr,dc->newstr.ystr,dc->newstr.zstr);
    else
    ui_draw(G,dc->newstr.xstr,dc->newstr.ystr);
}
void uiwin_3_polyfill(DIG *G)
{
    float x[200],y[200],z[200];
    int n,flag,color;
    int i;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;

    uiread_buf(G,&n,4);
    uiread_buf(G,x,4*n);
    uiread_buf(G,y,4*n);
    uiread_buf(G,z,4*n);
    uiread_buf(G,&flag,4);
    uiread_buf(G,&color,4);
    for(i=0;i<n;i++)
    {
     kgtransfrm(dc,*(x+i),*(y+i),*(z+i));
     kgprojection(dc,dc->trnstr);
     *(x+i) = dc->newstr.xstr;
     *(y+i) = dc->newstr.ystr;
     *(z+i) = dc->newstr.zstr;
    } 
    if (!G->D_ON) return;
    if (dc->ZBUFF == 1)
    ui_3_panel(G,x,y,z,(int)color,flag,(int)n);
    else 
    ui_panel(G,x,y,(int)color,flag,(int)n);
}

void uiwin_3_boxfill(DIG *G)
{
   float  x1,x2,y1,y2,z1,z2,fl,ib;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;

   uiread_buf(G,&x1,4);
   uiread_buf(G,&y1,4);
   uiread_buf(G,&z1,4);
   uiread_buf(G,&x2,4);
   uiread_buf(G,&y2,4);
   uiread_buf(G,&z2,4);
   uiread_buf(G,&fl,4);
   uiread_buf(G,&ib,4);
   kgtransfrm(dc,x1,y1,z1);
   kgprojection(dc,dc->trnstr);
   x1 = dc->newstr.xstr;
   y1 = dc->newstr.ystr;
   z1 = dc->newstr.zstr;
   kgtransfrm(dc,x2,y2,z2);
   kgprojection(dc,dc->trnstr);
   x2 = dc->newstr.xstr;
   y2 = dc->newstr.ystr;
   z2 = dc->newstr.zstr;
   if (!G->D_ON) return;
   if (dc->ZBUFF == 1)
   ui_3_box_fill(G,x1,y1,z1,x2,y2,z2,fl,ib);
   else
   ui_box_fill(G,x1,y1,x2,y2,fl,ib);
}
void uiwin_3_godrfill(DIG *G)
{
  float x[200],y[200],z[200],v[200];
  int i;
  int n;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;

  uiread_buf(G,&n,4);
  uiread_buf(G,x,4*n);
  uiread_buf(G,y,4*n);
  uiread_buf(G,z,4*n);
  uiread_buf(G,v,4*n);
  for(i=0;i<n;i++)
   {
     kgtransfrm(dc,*(x+i),*(y+i),*(z+i));
     kgprojection(dc,dc->trnstr);
     *(x+i) = dc->newstr.xstr;
     *(y+i) = dc->newstr.ystr;
     *(z+i) = dc->newstr.zstr;
   }
 if (!G->D_ON) return;
 if (dc->ZBUFF == 1)
 ui_3_godr_fill(G,n,x,y,z,v);
 else
 ui_godr_fill(G,n,x,y,v);
} 

void uiwin_transperancy(DIG *G)
{
   int val;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
   uiread_buf(G,&val,4);
   uiwrite_bf(G,&P,1);
   uiwrite_bf(G,&T3,1);
   uiwrite_bf(G,&val,4);
#if 0
   if ((TRANSPERANCY=val)==1) {
        if(ZBUFF!= 1) set_zbuf(1);
   }
#endif
   return;
}
void uiwin_clip_limit(DIG *G)
{
  float x1,y1,x2,y2;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  uiread_buf(G,&x1,4);
  uiread_buf(G,&y1,4);
  uiread_buf(G,&x2,4);
  uiread_buf(G,&y2,4);
  ui_clip_limit(G, x1, y1, x2, y2);
}
void uiwin_pop_clip(DIG *G)
{
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  ui_pop_clip(G);
}
void uiwin_chng_clr(DIG *G)
{
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc = G->wc;
  int no,ir,ig,ib;
  uiread_buf(G,&no,4);
  uiread_buf(G,&ir,4);
  uiread_buf(G,&ib,4);
  uiread_buf(G,&ig,4);
  uichng_clr(G->D,(int)no,(int)ir,(int)ig,(int)ib);
}
void uicheck_and_do(DIG *G,char ch)
 {
   char ch2;
   uiread_buf(G,&ch2,1);
//   printf("uicheck: %c%c \n",ch,ch2);
   switch((int)ch)
    { 
     case 'l' :
      switch((int)ch2)
      {
        case 'm': uiwin_move(G);
           break;
        case 'd': uiwin_draw(G);
           break;
        case 'i': uiwin_drawimage(G);
           break;
        case 'l': uiwin_drawline(G);
           break;
        case 's': uiwin_set_lnstyle(G);
           break;
        case 'y': uiwin_resetstyle(G);
           break;
        case 'w': uiwin_set_lnwidth(G);
           break;
        case 'c': uiwin_set_color(G);
           break;
        case 'r': uiwin_circle(G);
           break;
        case 'M': uiwin_3_move(G);
         break;
         case 'D': uiwin_3_draw(G);
         break;
        default :
               printf(" Wrong Code in line :%c%c \n",ch,ch2);
               exit(0);
      }
      break;
     case 'm' :
      switch((int)ch2)
      {
        case 'k': uiwin_marker(G);
           break;
        case 't': uiwin_set_markstyle(G);
           break;
        default :
               printf(" Wrong Code in mark :%c%c \n",ch,ch2);
               exit(0);
      }
      break;
     case 't' :
      switch((int)ch2)
      {
        case 'w': uiwin_txtwrt(G);
         break;
        case 'o': uiwin_set_txtbold(G);
  break;
        case 'h': uiwin_set_htxtbold(G);
  break;
        case 'c': uiwin_txt_color(G);
  break;
        case 'l': uiwin_txt_fill(G);
  break;
        case 'k': uiwin_txt_bkgr(G);
  break;
        case 'p': uiwin_txt_pattern(G);
  break;
        case 'b': uiwin_txt_bodr(G);
  break;
        case 'f': uiwin_txt_font(G);
  break;
        case 's': uiwin_txt_size(G);
  break;
        case 'r': uiwin_set_angle(G);
  break;
        case 'x': uiwin_set_prtxt(G);
  break;
        case 'a': uiwin_txt_slant(G);
  break;
        default :
               printf(" Wrong Code in text :%c%c \n",ch,ch2);
               exit(0);
       }
      break;
     case 'p' :
      switch((int)ch2)
      {
        case 'b': uiwin_boxfill(G); break;
        case 'r': uiwin_circlefill(G); break;
        case 'p': uiwin_poly_fill(G); break;
        case 'g': uiwin_godard_fill(G); break;
        case 's': uiwin_set_godr(G); break;
        case 'h': uiwin_set_shade_params(G); break;
        case 'B' : uiwin_3_boxfill(G); break;
        case 'G' : uiwin_3_godrfill(G); break;  
        case 'S' : uiwin_3_shade_o(G); break;  
        case 'H' : uiwin_3_shade(G); break;  
        case 'P' : uiwin_3_polyfill(G); break;  
        case 'T' : uiwin_transperancy(G); break;  
        default :
               printf(" Wrong Code in fill :%c%c \n",ch,ch2);
               exit(0);
       }
      break;
     case 's' :
      switch((int)ch2)
      {
        case 'c': uiwin_clip_limit(G);
  break;
        case 'p':
                uiwin_pop_clip(G);
break;
      }
       break;
     case 'c' :
      switch((int)ch2)
      {
        case 'h': uiwin_chng_clr(G);
  break;
      }
       break;
     default :
               printf(" Wrong Code:%c%c \n",ch,ch2);
               exit(0);
    }
 }

void uireview_file(DIG *G,char *stri)
{
  char ch;
  char str[250];
  int x1,x2,y1,y2,color,flag,n;
  kgDC *dc;
  dc = G->dc;
  strcpy(str,stri);
  n=0;
  while(str[n]>' ')n++;
  str[n]='\0';
  G->review=open(str,O_RDWR|O_BINARY, S_IREAD|S_IWRITE);
  uiread_file(G,G->review); 
  G->R_Byte=0;
  G->R_max=G->Byte;
  uiread_buf(G,&ch,1);
  while ( G->R_max>0)
  {
    uicheck_and_do(G,ch);
    uiread_buf(G,&ch,1);
  }
 close(G->review);
 G->review=-1;
// G->rbuf=open(dc->reviewfile,O_RDWR|O_BINARY|O_APPEND, S_IREAD|S_IWRITE);
}
 void uidrarrow(DIG *G,float x1,float y1, float x2,float y2,float fac)
  {
      float ct,st,size,xf,yf,s1,s2,s3,s4,s5,s6,
            sx1,sy1,sx2,sy2,sx3,sy3;
      float wxd,wyd;
      float ax[4],ay[4];
      double th,dxx,dyy;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      wxd = dc->w_x2-dc->w_x1;
      wyd = dc->w_y2-dc->w_y1;
/*
      xf = wxd/100.;
      yf = wyd/75.;
*/
      xf = wxd/160.*fac;
      yf = wyd/120.*fac;
      dxx = x2-x1;
      dyy = y2-y1;
      dxx = dxx/xf;
      dyy = dyy/yf;
      th=0.;
      if( (dyy!=0) ||(dxx!=0))th = atan2(dyy,dxx);
      ct = cos(th);
      st = sin(th);
      s1 = 2;
      kgDraw2f(G,x2,y2);
      s2 = sqrt((double)0.75)*s1;
      s3 = -s2*ct+s1*0.5*st;
      s4 = -s2*st-s1*0.5*ct;
      s5 = -s2*ct-s1*0.5*st;
      s6 = -s2*st+s1*0.5*ct;
      sx1=x2+s3*xf;
      sy1=y2+s4*yf;
      sx2=x2+s5*xf;
      sy2=y2+s6*yf;
      sx3 = (sx1+sx2)*0.25 + x2*0.5;
      sy3 = (sy1+sy2)*0.25 + y2*0.5;
      ax[0]=sx1;ax[1]=x2;ax[2]=sx2;ax[3]=sx3;
      ay[0]=sy1;ay[1]=y2;ay[2]=sy2;ay[3]=sy3;
      kgPolyFill(G,(int)4,ax,ay,(int)1,dc->fil_color);
      return;
/*
*/
    }
void uirectgl(DIG *G, float xx,float yy,float x,float y)
{
  kgMove2f(G,xx,yy);kgDraw2f(G,xx,y),kgDraw2f(G,x,y),kgDraw2f(G,x,yy),kgDraw2f(G,xx,yy);
}
void uipa_gram( DIG *G,float x1,float y1,float x2,float y2)
{
 float xx,yy,x,y;
  xx=x1;yy=y2-(y1-y2);
  x =x1+(x1-x2);y=y2;
  kgMove2f(G,xx,yy);kgDraw2f(G,x,y),kgDraw2f(G,x1,y1),kgDraw2f(G,x2,y2),kgDraw2f(G,xx,yy);
}
void uiborder(DIG *G,float x1,float y1,float x2,float y2,int wd,int clr)
 {
  float dx,dy,w[4],fac;
  float xmin,ymin,xmax,ymax;
  int nu,de;
  kgDC *dc;
  kgWC *wc;
  dc = G->dc;
  wc = G->wc;
  if(wd<10) wd=11;
  nu = wd/10;
  de = wd%10;
  fac = (float)nu/((float)de);
//  getwin(w);
  w[0]=dc->w_x1;
  w[1]=dc->w_y1;
  w[2]=dc->w_x2;
  w[3]=dc->w_y2;
  dx= (w[2]-w[0])/120.0*fac;
  dy= (w[3]-w[1])/90.0*fac;
  if( x1<x2) {xmin=x1;xmax=x2;}
  else       {xmin=x2;xmax=x1;}
  if( y1<y2) {ymin=y1;ymax=y2;}
  else       {ymin=y2;ymax=y1;}
  kgBoxFill(G,xmin,ymin,xmax,ymin+dy,clr,0);
  kgBoxFill(G,xmin,ymin,xmin+dx,ymax,clr,0);
  kgBoxFill(G,xmax,ymin,xmax-dx,ymax,clr,0);
  kgBoxFill(G,xmin,ymax,xmax,ymax-dy,clr,0);
 }
int  uiset_atribs_o(DIG *G) {
/*************************************************

    Text_Box1  16 data values

*************************************************/
   int angle;
   kgDC *dc;
   dc = G->dc;
   angle=dc->trot;
   _ui_setattrib(G->D,G->x1,G->y1, &(dc->ln_style), &(dc->ln_color), &(dc->ln_width), &(dc->m_style), 
     &(dc->fil_color), &(dc->t_color), &(dc->t_font), &(angle), 
     &(dc->bod_color) );
     dc->trot=angle;
#if 0
     ui_ln_clr(G,dc->ln_color);
     ui_txt_rot(G,dc->trot);
     ui_txt_font(G,dc->t_font);
     ui_smmark(G,dc->m_style);
#else
     kgLineColor(G,dc->ln_color);
     kgTextAngle(G,dc->trot);
     kgTextFont(G,dc->t_font);
     kgMarkerType(G,dc->m_style);
     kgLineWidth(G,dc->ln_width);
     return 30;
#endif
}
int uipolygon_fill(DIG *G,float *x,float *y,int fil_color)
 {
  float xx,yy;
  int i=0;
  unsigned char key;
  kgDC *dc;
  dc = G->dc;
  xx =x[0];yy=y[0];
  key = kgCrossCursor(G,&xx,&yy);
  while( (key !='\r')&&(i<27)){
    x[i]=xx,y[i]=yy;
    key = kgRbrCursor(G,&xx,&yy,&x[i],&y[i]);
    ui_DRAW_LINE(G,x[i],y[i],xx,yy);
    i++;
  }
  if(key =='\r') { x[i]=xx,y[i]=yy;i++;};
  if(i<3) return(0);
  kgPolyFill(G,(int) i, x, y, 1L,fil_color);
//  return( 26+8*i);
  return( 14+8*i);
 }
void uihoreview(DIG *G)
 {
  int cmds;
  char ch;
  DIALOG *D;
  D= (DIALOG *)G->D;
  kgWC *wc;
  kgDC *dc;
  wc = WC(D);
  dc = G->dc;
  if(G->D_ON)ui_clr_vu(G); 
//TCBTCB
#if 1
//  if( ZBUFF==1)clear_zbuf();
  if (G->BACK_UP == 1) { 
      uiwrite_file(G,&(G->rbuf),dc->reviewfile);
      lseek(G->rbuf,0L,0);
      G->review= G->rbuf;
      uiread_file(G,G->review); 
  }
  else G->review= G->rbuf;
  G->R_Byte=0;
  G->R_max=G->Byte;
  uiread_buf(G,&ch,1);
  while ( G->R_max>0)
  {
    uicheck_and_do(G,ch);
    uiread_buf(G,&ch,1);
  }
  if( G->BACK_UP ==1) {  G->Byte=0;G->R_max=0;}
  G->review= -1;
#else
//   uiwrite_file(G,&(G->rbuf),dc->reviewfile);
   uiclose_file(G,&(G->rbuf),dc->reviewfile);
   uireview_file(G,dc->reviewfile);
   if( G->BACK_UP ==1) {  G->Byte=0;G->R_max=0;}
#endif
 }
void uiupdate_view(DIG *G)
 {
  int cmds,rbufbk;
  kgDC *dc;
  dc = G->dc;
  G->cmdbuf=open(dc->cmdsfile,O_CREAT|O_TRUNC|O_BINARY|O_RDWR, S_IREAD|S_IWRITE);
  uiwrite_cmds(G);
  G->Byte=0;G->R_max=0;
  if(G->D_ON)ui_clr_vu(G);
  uiclose_file(G,&(G->rbuf),dc->reviewfile);
  uireview_file(G,dc->reviewfile);
  G->rbuf=open(dc->reviewfile,O_RDWR|O_BINARY|O_APPEND, S_IREAD|S_IWRITE);
#if 0
  lseek(G->cmdbuf,(int)0,0);
  uiread_file(G,G->cmdbuf);
  G->R_max=G->Byte;;
  G->R_Byte=0;
  G->review= G->cmdbuf;
  uireview_cmds(G);
  close(G->cmdbuf);
  remove(dc->cmdsfile);
  G->review=-1;
#else
  close(G->cmdbuf);
  G->Byte=0;G->R_max=0;
  uireview_file(G,dc->cmdsfile);
  remove(dc->cmdsfile);
#endif
 }
void uireview_cmds(DIG *G)
{
  char ch;
  char flname[250];
  int x1,x2,y1,y2,color,flag,n;
  kgDC *dc;
  dc = G->dc;
  G->R_Byte=0;
  if(G->Byte== 0) return;
  G->R_max=B_max;
  uiread_buf(G,&ch,1);
  while ( G->R_Byte<G->Byte)
  {
   printf("uireview_cmds:R_Byte: %d\n",G->R_Byte);
   uicheck_and_do(G,ch);
   uiread_buf(G,&ch,1);
  }
}
 void  NewPage(int hbuf)
  {
   char ch;
   ch ='P';
   WRITE(hbuf,(void *)&ch,1);
//   NO_HCOPY=0;
  }
#if 0
 void  hard_copy(void *parent,int gbox)
  {
   char ch;
   ch ='P';
   if(ZBUFF) TIFF=1;
   WRITE(hbuf,(void *)&ch,1);
   close(hbuf);
#if 1
   Runprinterdia(parent);
#else
   if(TIFF != 1) SYSTEM("kgpscript");
   else {
      SYSTEM("tiff 300");
   }
   uiPrintFile("GRAF");
#endif
   TIFF=0;
//   remove("plot.gph");
   hbuf=open(".plotgph",O_CREAT|O_RDWR|O_TRUNC|O_BINARY, 0666);
  }
 #endif
 void kgSaveAsPng(DIG *G,char *flname) {
   char ch;
   char command[500];
   int xsize,ysize,xdim=1024;
   float fac =1.0;
   kgDC *dc;
   dc = G->dc;
   xsize = 640;
//   ysize = xsize*(float)(dc->EVGAY)/(float)( dc->EVGAX);
   ysize = xsize;
   kgGetWidgetSize(G,&xsize,&ysize);
   gscanf(G->D,"Dimension in X direction %6d",&xdim);
   fac = (float)xdim/xsize;
   xsize = xdim;
   ysize = ysize*fac;
#if 0
   uilscopy(G);
   ch ='P';
   dc->TIFF =0;
   if(dc->ZBUFF) dc->TIFF=1;
   WRITE(G->hbuf,(void *)&ch,1);
   close(G->hbuf);
   {
      char *dir,pngfile[300];
      void *png;
      printf("Creating Png Image File...\n");
      dir = kgMakeTmpDir();
      sprintf(pngfile,"%-s/gph.png",dir);
      kgBackupGph(G,dc->plotfile);
      png=kgGphtoAntialiasedImage(dc->plotfile,dc->EVGAX*2,dc->EVGAY*2,0,4);
      kgWriteImage(png,pngfile);
      kgFreeImage(png);
      sprintf(command,"mv %-s %-s",pngfile,flname);
      SYSTEM(command);
      kgCleanDir(dir);
      free(dir);
   }
   dc->TIFF=0;
   remove(dc->plotfile);
   G->hbuf =-1;
#else
      char *dir,pngfile[300];
      void *png;
      printf("Creating Png Image File...\n");
      dir = kgMakeTmpDir();
      sprintf(pngfile,"%-s/gph.png",dir);
      kgBackupGph(G,dc->plotfile);
      png=kgGphtoAntialiasedImage(dc->plotfile,xsize,ysize,0,4);
      kgWriteImage(png,pngfile);
      kgFreeImage(png);
      sprintf(command,"mv %-s %-s",pngfile,flname);
      SYSTEM(command);
      kgCleanDir(dir);
      free(dir);
#endif
 }
 void kgHardCopy(DIG *G,char *flname) {
   char ch;
   char command[500];
   kgDC *dc;
   dc = G->dc;
   uilscopy(G);
   ch ='P';
   dc->TIFF =0;
   if(dc->ZBUFF) dc->TIFF=1;
   WRITE(G->hbuf,(void *)&ch,1);
   close(G->hbuf);
   if(dc->TIFF != 1){
      printf("Creating PostScript File...\n");
//      sprintf(command,"kgpscript  -i%-s -o%-s",dc->plotfile,flname);
//      SYSTEM(command);
      pscript(dc->plotfile,flname);
   }
   else {
      char *dir,pngfile[300];
      void *png;
      printf("Creating Png Image File...\n");
      dir = kgMakeTmpDir();
      sprintf(pngfile,"%-s/gph.png",dir);
      kgBackupGph(G,dc->plotfile);
      png=kgGphtoAntialiasedImage(dc->plotfile,dc->EVGAX,dc->EVGAY,0,4);
      kgWriteImage(png,pngfile);
      kgFreeImage(png);
      sprintf(command,"mv %-s %-s",pngfile,flname);
      SYSTEM(command);
      kgCleanDir(dir);
      free(dir);
   }
   dc->TIFF=0;
   remove(dc->plotfile);
   G->hbuf =-1;
 }
 void kgLandscapeCopy(DIG *G,char *flname) {
   char ch;
   char command[500];
   kgDC *dc;
   dc = G->dc;
   uilscopy(G);
   ch ='P';
   dc->TIFF =0;
   if(dc->ZBUFF) dc->TIFF=1;
   WRITE(G->hbuf,(void *)&ch,1);
   close(G->hbuf);
   if(dc->TIFF != 1){
      printf("Creating PostScript File...\n");
//      sprintf(command,"kgpscript   -i%-s -o%-s",dc->plotfile,flname);
//      SYSTEM(command);
      pscript(dc->plotfile,flname);
   }
   else {
      char *dir,pngfile[300];
      void *png;
      printf("Creating Png Image File...\n");
      dir = kgMakeTmpDir();
      sprintf(pngfile,"%-s/gph.png",dir);
      kgBackupGph(G,dc->plotfile);
      png=kgGphtoAntialiasedImage(dc->plotfile,dc->EVGAX,dc->EVGAY,0,4);
      kgWriteImage(png,pngfile);
      kgFreeImage(png);
      sprintf(command,"mv %-s %-s",pngfile,flname);
      SYSTEM(command);
      kgCleanDir(dir);
      free(dir);
   }
   dc->TIFF=0;
   remove(dc->plotfile);
   G->hbuf =-1;
 }
 void kgA4Copy(DIG *G,char *flname) {
   char ch;
   char command[500];
   kgDC *dc;
   dc = G->dc;
   uia4copy(G);
   ch ='P';
   dc->TIFF =0;
   if(dc->ZBUFF) dc->TIFF=1;
   WRITE(G->hbuf,(void *)&ch,1);
   close(G->hbuf);
   if(dc->TIFF != 1){
      printf("Creating PostScript File...\n");
//      sprintf(command,"kgpscript  -i%-s -o%-s",dc->plotfile,flname);
//      SYSTEM(command);
      pscript(dc->plotfile,flname);
   }
   else {
      char *dir,pngfile[300];
      void *png;
      printf("Creating Png Image File...\n");
      dir = kgMakeTmpDir();
      sprintf(pngfile,"%-s/gph.png",dir);
      kgBackupGph(G,dc->plotfile);
      png=kgGphtoAntialiasedImage(dc->plotfile,dc->EVGAX,dc->EVGAY,0,4);
      kgWriteImage(png,pngfile);
      kgFreeImage(png);
      sprintf(command,"mv %-s %-s",pngfile,flname);
      SYSTEM(command);
      kgCleanDir(dir);
      free(dir);
   }
   dc->TIFF=0;
   remove(dc->plotfile);
   G->hbuf =-1;
 }
#if 0
/*
 For printing multiple pages
 Go on calling with '0' as argument
 After all pages call once with argument '1'
 to finish and reset
*/
 int  print_pages(void *parent,int hbuf,int last)
  {
   char ch;
   ch ='P';
   if(last ) {
     PrintPages(parent,1);
   }
   else {
     if(ZBUFF) TIFF=1;
     WRITE(hbuf,(void *)&ch,1);
     close(hbuf);
     PrintPages(parent,0);
     TIFF=0;
//     remove("plot.gph");
     hbuf=open(".plotgph",O_CREAT|O_RDWR|O_TRUNC|O_BINARY, 0666);
   }
   return 1;
  }
 int  make_page(int bw,int hbuf)
  {
   char ch;
   ch ='P';
     if(ZBUFF) TIFF=1;
     WRITE(hbuf,(void *)&ch,1);
     close(hbuf);
     MakePage(bw,hbuf);
     TIFF=0;
//     remove("plot.gph");
     hbuf=open(".plotgph",O_CREAT|O_RDWR|O_TRUNC|O_BINARY, 0666);
   return 1;
  }
#endif
 void  uihcopy(DIG *G)
  {
   int pf;
   char ch;
   kgDC *dc;
   dc = G->dc;
   if(G->hbuf<0) G->hbuf=open(dc->plotfile,O_CREAT|O_RDWR|O_TRUNC|O_BINARY, 0666);
   pf=G->hbuf;
   if( pf< 0) { exit(0);};
   ch='v';
   WRITE(pf,(void *)&ch,1);
   WRITE(pf,(void *)&dc->vu_x1,4);
   WRITE(pf,(void *)&dc->vu_y1,4);
   WRITE(pf,(void *)&dc->vu_x2,4);
   WRITE(pf,(void *)&dc->vu_y2,4);
   ch='C';
   WRITE(pf,(void *)&ch,1);
   WRITE(pf,(void *)&dc->w_x1,4);
   WRITE(pf,(void *)&dc->w_y1,4);
   WRITE(pf,(void *)&dc->w_x2,4);
   WRITE(pf,(void *)&dc->w_y2,4);
   if((dc->ZBUFF)||(dc->TIFF)) {
     if(dc->PROJ) update_gr_info(G);
     WRITE(pf,(void *)&Z,1);
   }
#if 0
   Fixshadecolors_hsv(pf);
#endif
   G->BACK_UP=1;
   uiclose_file(G,&(G->rbuf),dc->reviewfile);
   G->rbuf=open(dc->reviewfile,O_RDWR|O_BINARY, S_IREAD|S_IWRITE);
   if( G->rbuf<0) return;
   uiread_file(G,G->rbuf); 
   while(G->Byte==B_min){WRITE(pf,(void *)G->Rbuff,G->Byte); uiread_file(G,G->rbuf); }
   WRITE(pf,(void *)G->Rbuff,G->Byte);
   G->OPEN=1;
   G->R_Byte=0;
   G->Byte=0;G->R_max=0;
   return;
  }
 void  uia4copy(DIG *G) {
   int pf;
   char ch;
   kgDC *dc;
   dc = G->dc;
   G->hbuf=open(dc->plotfile,O_CREAT|O_RDWR|O_TRUNC|O_BINARY, 0666);
   pf=G->hbuf;
   if( pf< 0) { exit(0);};
   ch='A';
   WRITE(pf,(void *)&ch,1);
   uihcopy(G);
   return;
 }
 void  uilscopy(DIG *G) {
   int pf;
   char ch;
   kgDC *dc;
   dc = G->dc;
   G->hbuf=open(dc->plotfile,O_CREAT|O_RDWR|O_TRUNC|O_BINARY, 0666);
   pf=G->hbuf;
   if( pf< 0) { exit(0);};
   ch='S';
   WRITE(pf,(void *)&ch,1);
   uihcopy(G);
   return;
 }
void stripblnk( char *ch)
  {
   int n=0;
   while( *(ch+n) != '\0') n++;
   n--;
   while((n>=0)&&( *(ch+n) == ' '))  *(ch+n--)='\0';
   n = strlen(ch);
  }
void mul_matrix(float *A,float *B,float *C)
 {

   C[0] = A[0]* B[0] + A[1]*B[4] + A[2]*B[8] + A[3]*B[12];
   C[1] = A[0]*B[1] + A[1]*B[5] + A[2]*B[9] + A[3]*B[13];
   C[2] = A[0]*B[2] + A[1]*B[6] + A[2]*B[10] + A[3]*B[14];
   C[3] = A[0]*B[3] + A[1]*B[7] + A[2]*B[11] + A[3]*B[15];
   C[4] = A[4]*B[0] + A[5]*B[4] + A[6]*B[8] + A[7] *B[12];
   C[5] = A[4]*B[1] + A[5]*B[5] + A[6]*B[9] + A[7]*B[13];
   C[6] = A[4]*B[2] + A[5]*B[6] + A[6]*B[10] + A[7]*B[14];
   C[7] = A[4]*B[3] + A[5]*B[7] + A[6]*B[11] + A[7]*B[15];
   C[8] = A[8]*B[0] + A[9]*B[4] + A[10]*B[8] + A[11]*B[12];
   C[9] = A[8]*B[1] + A[9]*B[5] + A[10]*B[9] + A[11]*B[13];
   C[10] = A[8]*B[2] + A[9]*B[6] + A[10]*B[10] + A[11]*B[14];
   C[11] = A[8]*B[3] + A[9]*B[7] + A[10]*B[11] + A[11]*B[15];
   C[12] = A[12]*B[0] + A[13]*B[4] + A[14]*B[8] + A[15]*B[12];
   C[13] = A[12]*B[1] + A[13]*B[5] + A[14]*B[9] + A[15]*B[13];
   C[14] = A[12]*B[2] + A[13]*B[6] + A[14]*B[10] + A[15]*B[14];
   C[15] = A[12]*B[3] + A[13]*B[7] + A[14]*B[11] + A[15]*B[15];
 }
  
 
void kgtransfrm(kgDC *dc,float xp, float yp, float zp)
 {
   float *FM;
   FM = dc->FM;
   dc->trnstr.xstr = (xp * FM[0]) + (yp * FM[4]) + (zp * FM[8]) + FM[12];
   dc->trnstr.ystr = (xp * FM[1]) + (yp * FM[5]) + (zp * FM[9]) + FM[13];
   dc->trnstr.zstr = (xp * FM[2]) + (yp * FM[6]) + (zp * FM[10]) + FM[14];
   return;

 }

void kgtranslate(kgDC *dc,float tx, float ty, float tz)
 {
    int i;
    
    dc->TR[12] = -tx; dc->TR[13] = -ty; dc->TR[14] = -tz;
    mul_matrix(dc->TM,dc->TR,dc->T1);
    for(i=0;i<16;i++) dc->TM[i] = dc->T1[i];
    mul_matrix(dc->TC,dc->TM,dc->T1);
    mul_matrix(dc->T1,dc->TE,dc->FM);
 }
void kgTranslate(DIG *G,float tx, float ty, float tz)
 {
    int i;
    kgDC *dc;
    dc = G->dc;
    
    dc->TR[12] = +tx; dc->TR[13] = +ty; dc->TR[14] = +tz;
    mul_matrix(dc->TM,dc->TR,dc->T1);
    for(i=0;i<16;i++) dc->TM[i] = dc->T1[i];
    mul_matrix(dc->TC,dc->TM,dc->T1);
    mul_matrix(dc->T1,dc->TE,dc->FM);
 }

void kgrotatx(kgDC *dc,float ang)
 {
    int i;
    dc->TX[5] = cos(ang); dc->TX[6] = sin(ang); dc->TX[9] = -(dc->TX[6]);
    dc->TX[10] = dc->TX[5];
    mul_matrix(dc->TM,dc->TX,dc->T1);
    for(i=0;i<16;i++) dc->TM[i] = dc->T1[i]; 
 }

void kgrotaty(kgDC *dc,float ang)
 {
    int i;
   
    dc->TY[0] = cos(ang); dc->TY[2] = -(sin(ang)); dc->TY[8] = sin(ang);
    dc->TY[10] = dc->TY[0];
    mul_matrix(dc->TM,dc->TY,dc->T1); 
    for(i=0;i<16;i++) dc->TM[i] = dc->T1[i]; 
 }

void kgrotatz(kgDC *dc,float ang)
 {
    int i;

    dc->TZ[0] = cos(ang); dc->TZ[1] = sin(ang); dc->TZ[4] = -dc->TZ[1];
    dc->TZ[5] = dc->TZ[0];
    mul_matrix(dc->TM,dc->TZ,dc->T1); 
    for(i=0;i<16;i++) dc->TM[i] = dc->T1[i]; 
 }

void kgrotate(kgDC *dc,unsigned char ch, float ang)
 {
    float rdg; 
    rdg = (M_PI * ang)/180;
    if (ch == 'X') kgrotatx(dc,rdg);
    if (ch == 'Y') kgrotaty(dc,rdg);
    if (ch == 'Z') kgrotatz(dc,rdg);
    mul_matrix(dc->TC,dc->TM,dc->T1);
    mul_matrix(dc->T1,dc->TE,dc->FM);
 }
void kgRotate(DIG *G,unsigned char ch, float ang)
 {
    kgDC *dc;
    float rdg; 
    dc = G->dc;
    rdg = (M_PI * ang)/180;
    if (ch == 'X') kgrotatx(dc,rdg);
    if (ch == 'Y') kgrotaty(dc,rdg);
    if (ch == 'Z') kgrotatz(dc,rdg);
    mul_matrix(dc->TC,dc->TM,dc->T1);
    mul_matrix(dc->T1,dc->TE,dc->FM);
 }
void kgprojection(kgDC *dc,str1 oldstr)
 {
   if(dc->PROJ) {
     dc->newstr.xstr = -(2.0* dc->ze/(oldstr.zstr)) * oldstr.xstr;
     dc->newstr.ystr = -(2.0* dc->ze/(oldstr.zstr)) * oldstr.ystr;
     dc->newstr.zstr = oldstr.zstr;
   }
   else {
     dc->newstr.xstr =  oldstr.xstr;
     dc->newstr.ystr =  oldstr.ystr;
     dc->newstr.zstr =  oldstr.zstr;
   }
     return;
 }

 
void kgUserFrame3D(DIG *G,float x_min,float y_min,float z_min,float x_max,float y_max,float z_max)
 {
     kgDC *dc;
     float x1,x2,y1,y2;
     int i;
     dc= G->dc;
     dc->SET3D=1;
     dc->PROJ =1;
     dc->xe = (x_max + x_min) / 2;
     dc->ye = (y_max + y_min) / 2;
     /*ze = z_max+20.0*(z_max-z_min);*/
     dc->zc = (z_max + z_min)/2;
     dc->ze = (z_max - z_min)*3.0;
     for(i=0;i<16;i++)
     {
      dc->TM[i] = TI[i];
      dc->TR[i] = TI[i];
      dc->TX[i] = TI[i];
      dc->TY[i] = TI[i];
      dc->TZ[i] = TI[i];
      dc->FM[i] = TI[i];
      dc->TE[i] = TI[i];
      dc->TC[i] = TI[i];
     }
     dc->TE[12] = 0;dc->TE[13] = 0;dc->TE[14] = -dc->ze;
     dc->TC[12] = -dc->xe;dc->TC[13] = -dc->ye;dc->TC[14] =-dc->zc;
     mul_matrix(dc->TC,dc->TE,dc->FM);
     kgtransfrm(dc,x_min,y_min,z_min);
     dc->clip_min=dc->trnstr.zstr;
     kgtransfrm(dc,x_min,y_min,z_max);
     dc->clip_max=dc->trnstr.zstr;
     kgprojection(dc,dc->trnstr);
     x1=dc->newstr.xstr;
     y1=dc->newstr.ystr;
     kgtransfrm(dc,x_max,y_max,z_max);
     kgprojection(dc,dc->trnstr);
     x2=dc->newstr.xstr;
     y2=dc->newstr.ystr;
     kgUserFrame(G,x1,y1,x2,y2);
//     dc->PROJ =1;
     ui_clip_const(G,dc->clip_min,dc->clip_max);
     dc->box3d.xmin =  x_min;
     dc->box3d.ymin =  y_min;
     dc->box3d.zmin =  z_min;
     dc->box3d.xmax =  x_max;
     dc->box3d.ymax =  y_max;
     dc->box3d.zmax =  z_max;
     dc->box3d.xe   =  dc->xe;
     dc->box3d.ye   =  dc->ye;
     dc->box3d.ze   =  dc->ze;
     dc->box3d.xc   =  dc->xe;
     dc->box3d.yc   =  dc->ye;
     dc->box3d.zc   =  dc->zc;
     dc->box3d.d = (x_max -x_min)*(x_max- x_min) +
               (y_max -y_min)*(y_max- y_min) +
               (z_max -z_min)*(z_max- z_min) ;
     dc->box3d.d = sqrt(dc->box3d.d);
  } 
void kgMove3f(DIG *G,float x, float y, float z)
 {
     kgDC *dc;
     dc = G->dc;
   if(G->Rbuff != NULL) {
     uiwrite_bf(G,&L,1);
     uiwrite_bf(G,&M3,1);
     uiwrite_bf(G,&x,4);
     uiwrite_bf(G,&y,4);
     uiwrite_bf(G,&z,4);
   }
     kgtransfrm(dc,x,y,z);
     kgprojection(dc,dc->trnstr);
     if (!G->D_ON) {
       if (dc->ZBUFF == 1) img_move3f(G,dc->newstr.xstr , dc->newstr.ystr , dc->newstr.zstr);
       else  img_move(G,dc->newstr.xstr , dc->newstr.ystr);
     }
     else {
       if (dc->ZBUFF == 1) ui_3_move(G,dc->newstr.xstr , dc->newstr.ystr , dc->newstr.zstr);
       else ui_move(G,dc->newstr.xstr , dc->newstr.ystr);
     }
     return;
 }
void kgDraw3f(DIG *G,float x, float y, float z)
 {

     kgDC *dc;
     dc = G->dc;
     DIALOG *D;
     D = G->D;
   if(G->Rbuff != NULL) {
     uiwrite_bf(G,&L,1);
     uiwrite_bf(G,&D3,1);
     uiwrite_bf(G,&x,4);
     uiwrite_bf(G,&y,4);
     uiwrite_bf(G,&z,4);
   }
     kgtransfrm(dc,x,y,z);
     kgprojection(dc,dc->trnstr);
     if (!G->D_ON) {
       if (dc->ZBUFF == 1) { img_draw3f(G,dc->newstr.xstr , dc->newstr.ystr , dc->newstr.zstr); }
       else      img_draw(G,dc->newstr.xstr , dc->newstr.ystr);
//          uiCopyResizedImage(G);
     }
     else {
       if (dc->ZBUFF == 1) { ui_3_draw(G,dc->newstr.xstr , dc->newstr.ystr , dc->newstr.zstr); }
       else { ui_draw(G,dc->newstr.xstr,dc->newstr.ystr); }
     }
     return;
 }

void kgPolyFill3f(DIG *G,int n, float *x, float *y, float *z, int flag, int color)
 {
      float x1[2000],y1[2000],z1[2000];
      int i;

     kgDC *dc;
     dc = G->dc;
   if(G->Rbuff != NULL) {
      uiwrite_bf(G,&P,1);
      uiwrite_bf(G,&P3,1);
      uiwrite_bf(G,&n,4);
      uiwrite_bf(G,x,4*n);
      uiwrite_bf(G,y,4*n);
      uiwrite_bf(G,z,4*n);
      uiwrite_bf(G,&flag,4);
      uiwrite_bf(G,&color,4);
   }
      for(i=0;i<n;i++) {
         kgtransfrm(dc,*(x+i),*(y+i),*(z+i));
         kgprojection(dc,dc->trnstr);
         *(x1+i) = dc->newstr.xstr;
         *(y1+i) = dc->newstr.ystr;
         *(z1+i) = dc->newstr.zstr;
      }
      if (!G->D_ON) {
          if (dc->ZBUFF == 1) img_polyfill3f(G,n,x1,y1,z1,flag,color);
          else    img_poly_fill(G,n,x1,y1,flag,color);
          return;
      }
      else {
        if (dc->ZBUFF == 1) ui_3_panel(G,x1,y1,z1,(int)color,flag,n);
        else ui_panel(G,x1,y1,(int)color,flag,n);
      }
  }
void kgBoxFill3f(DIG *G,float x1,float y1, float z1, float x2, float y2,float z2, int fl,int ib)
 {

     kgDC *dc;
     dc = G->dc;

   if(G->Rbuff != NULL) {
      uiwrite_bf(G,&P,1);
      uiwrite_bf(G,&B3,1);
      uiwrite_bf(G,&x1,4);
      uiwrite_bf(G,&y1,4);
      uiwrite_bf(G,&z1,4);
      uiwrite_bf(G,&x2,4);
      uiwrite_bf(G,&y2,4);
      uiwrite_bf(G,&z2,4);
      uiwrite_bf(G,&fl,4);
      uiwrite_bf(G,&ib,4);
   }
      if (!G->D_ON) return;
      kgtransfrm(dc,x1,y1,z1);
      kgprojection(dc,dc->trnstr);
      x1 = dc->newstr.xstr;
      y1 = dc->newstr.ystr;
      z1 = dc->newstr.zstr;
      kgtransfrm(dc,x2,y2,z2);
      kgprojection(dc,dc->trnstr);
      x2 = dc->newstr.xstr;
      y2 = dc->newstr.ystr;
      z2 = dc->newstr.zstr;
      if (!G->D_ON) {
        if (dc->ZBUFF == 1) img_boxfill3f(G,x1,y1,z1,x2,y2,z2,ib,fl);
        else img_boxfill(G,x1,y1,x2,y2,fl);
      }
      else {
        if (dc->ZBUFF == 1) ui_3_box_fill(G,x1,y1,z1,x2,y2,z2,fl,ib);
        else ui_box_fill(G,x1,y1,x2,y2,fl,ib);
      }

 }
void kgGouraudFill3f(DIG *G,int n,float *x,float *y,float *z,float *v)
{
  int i;

  kgDC *dc;
  dc = G->dc;
  if(G->Rbuff != NULL) {
  uiwrite_bf(G,&P,1);
  uiwrite_bf(G,&G3,1);
  uiwrite_bf(G,&n,4);
  uiwrite_bf(G,x,4*n);
  uiwrite_bf(G,y,4*n);
  uiwrite_bf(G,z,4*n);
  uiwrite_bf(G,v,4*n);
  }
  for(i=0;i<n;i++)
   {
    kgtransfrm(dc,*(x+i), *(y+i),*(z+i));
    kgprojection(dc,dc->trnstr);
    *(x+i) = dc->newstr.xstr;
    *(y+i) = dc->newstr.ystr;
    *(z+i) = dc->newstr.zstr;
   }
  if (!G->D_ON) {
    if (dc->ZBUFF == 1) img_godrfill3f(G,n,x,y,z,v);
    else img_godard_fill(G,n,x,y,v);
  }
  else {
    if (dc->ZBUFF == 1) ui_3_godr_fill(G,n,x,y,z,v);
    else ui_godr_fill(G,n,x,y,v);
  }
}
    
void kgSwapBuffers(DIG *G) 
 {
  kgDC *dc;
  dc= G->dc;
  if(G->D==NULL) return;
  if(dc->DOUBLE== 1) ui_swapbuffers(G);
  else kgUpdateOn(G->D);
 }
void kgDoubleBuffer(DIG *G) 
 {
  kgDC *dc;
  DIALOG *D;
  if(G->D==NULL) return;
  dc= G->dc;
#if 0
  D = G->D;
  ui_doublebuffer(G);
  dc->DOUBLE=1;
#else
  dc->DOUBLE=0;
  if(G->D_ON!= 0) kgAntialiasingOn(G,0);
#endif
 }
void kgSingleBuffer(DIG *G) 
 {
  kgDC *dc;
  dc= G->dc;
  if(G->D==NULL) return;
  if(G->MAG <=1 ) kgAntialiasingOff(G);
  if(dc->DOUBLE== 1)ui_singlebuffer(G);
  dc->DOUBLE=0;
 }

void kgSetZbuffer(DIG *G,int val)
 {
    kgDC *dc;
    dc = G->dc;
     if( dc->ZBUFF==val){
       if(dc->ZBUFF==1) {dc->TIFF =1;ui_set_zbuf(G);}
       return;
     }
     dc->ZBUFF = val;
     if (dc->ZBUFF == 1)
      {
       dc->TIFF=1;
       ui_set_zbuf(G);
      }
     else {
       dc->TIFF=0;
       ui_release_buf(G);
     }
 } 
void kgSetNearClip(DIG *G,int val)
 {
    val = -val;
    ui_near_clip(G,val);
 }
void kgSetFarClip(DIG *G,int val)
 {
    val = -val;
    ui_far_clip(G,val);
 }
void normal(void)
  {
  }
#if 0
void close_hardcopy(void) {
  close(hbuf);
  return;
}
#endif
void kgSetClip(DIG *G,float x1,float y1,float x2,float y2)
 {
      float xmin,ymin,xmax,ymax;
      if(x1<x2) {xmin=x1;xmax=x2;}
      else {xmin=x2,xmax=x1;}
      if(y1<y2) {ymin=y1;ymax=y2;}
      else {ymin=y2,ymax=y1;}
  if(G->Rbuff != NULL) {
      uiwrite_bf(G,&S,1);
      uiwrite_bf(G,&C,1);
      uiwrite_bf(G,&xmin,4);
      uiwrite_bf(G,&ymin,4);
      uiwrite_bf(G,&xmax,4);
      uiwrite_bf(G,&ymax,4);
   }
      ui_clip_limit(G, xmin, ymin, xmax, ymax);
 }
void kgPopClip(DIG *G)
 {
  
  if(G->Rbuff != NULL) {
    uiwrite_bf(G,&S,1);
    uiwrite_bf(G,&P,1);
  }
    ui_pop_clip(G);
 }
void put_message(int ix,int iy,char *ch)
 {
#if 0
   int i;
   i=0;
   while( *(ch+i)!='\0')i++;
   i++;
//   t_message(ix,iy,i,ch);
   uiPrintf(ix,iy,ch);
#endif
 }
void clear_message()
 {
#if 0
   int i;
   i=0;
   while( *(ch+i)!='\0')i++;
   i++;
//   t_clear_message();
#endif
 }
void kgGetRGB(DIG *G,int no,int *r,int *g,int *b) {
  kgColor *kgcolors;
  kgWC *wc;
  wc = (kgWC *)(G->wc);
  kgcolors = wc->kgcolors;
  if(no > 1023 ) no=15;
  *r = kgcolors[no].red;
  *g = kgcolors[no].green;
  *b = kgcolors[no].blue;
}
 void kgChangeColor(DIG *G,int no,int ir,int ig,int ib)
  {
   if(G->Rbuff != NULL) {
     uiwrite_bf(G,&C,1);
     uiwrite_bf(G,&H,1);
     uiwrite_bf(G,&no,4);
     uiwrite_bf(G,&ir,4);
     uiwrite_bf(G,&ib,4);
     uiwrite_bf(G,&ig,4);
   }
   if(G->D_ON)uichng_clr(G->D,(int) no,(int) ir,(int) ig,(int) ib);
   else img_chng_clr(G,(int) no,(int) ir,(int) ig,(int) ib);
  }
 void kgResetColor(void *D,int no,int ir,int ig,int ib)
  {
     uichng_clr((DIALOG *)D,(int) no,(int) ir,(int) ig,(int) ib);
  }
 void gphChangeColor(int fid,int no,int ir,int ig,int ib)
  {
     WRITE(fid,&C,1);
     WRITE(fid,&H,1);
     WRITE(fid,&no,4);
     WRITE(fid,&ir,4);
     WRITE(fid,&ib,4);
     WRITE(fid,&ig,4);
  }
 int Chng_sh_clr(int no,int ir,int ib,int ig,int file)
  {
     int ret=1;
     WRITE(file,(void *)&C,1);
     WRITE(file,(void *)&H,1);
     WRITE(file,(void *)&no,4);
     WRITE(file,(void *)&ir,4);
     WRITE(file,(void *)&ib,4);
     WRITE(file,(void *)&ig,4);
  }
#if 0
void check_abort(void *parent)
 {
   union kbinp { short kbint; char kbc[2];} kb;
   if((kb.kbint=get_kb(0))<0)return;
   if(kb.kbc[0] != 27) return;
   if( kgCheckMenu(parent,100L,200L," DO YOU WANT TO ABORT THE JOB ?",1L)){
     exit(0);
   }
   return;
 }
#endif
void kgPointerZoom(DIG *G) {
 float xmin,ymin,xmax,ymax;
 int MAG=0;
 kgSkipEvents(G->D);
#if 0
 if(G->D_ON== 0) {
   kgPointerAntialiasedZoom(G);
 }
 else {
   ui_PointerZoom(G,&xmin,&ymin,&xmax,&ymax);
   kgUserFrame(G,xmin,ymin,xmax,ymax);
 }
#else
   ui_PointerZoom(G,&xmin,&ymin,&xmax,&ymax);
   kgUserFrame(G,xmin,ymin,xmax,ymax);
#endif
}
void kgPointerAntialiasedZoom(DIG *G) {
 float xmin,ymin,xmax,ymax;
 int D_ON;
 D_ON = G->D_ON;
 if(D_ON== 0) kgSetAntialiasing(G,0);
 kgSkipEvents(G->D);
 ui_PointerAntialiasedZoom(G,&xmin,&ymin,&xmax,&ymax);
 kgUserFrame(G,xmin,ymin,xmax,ymax);
 kgAntialiasedReview(G);
 if(D_ON== 0) kgSetAntialiasing(G,1);
}
#if 0
void PointerZoom_dum(void) {
 float w[4];
 float xmin,ymin,xmax,ymax;
 getwin(w);
 xmin = w[0];ymin = w[1];
 xmax = w[2];ymax = w[3];
 t_PointerZoomPan(&xmin,&ymin,&xmax,&ymax);
 set_window(xmin,ymin,xmax,ymax);
}
#endif
#if 0
void PointerZoomPan(void) {
 float w[4];
 float xmin,ymin,xmax,ymax;
 getwin(w);
 xmin = w[0];ymin = w[1];
 xmax = w[2];ymax = w[3];
 t_PointerZoomPan(&xmin,&ymin,&xmax,&ymax);
 set_window(xmin,ymin,xmax,ymax);
}
void PointerRotate(float ang) {
 t_PointerRotate(ang);
}
void PointerMove(void) {
 float dz;
 dz = (box3d.zmax - box3d.zmin)/100.;
 t_PointerMove(dz);
}
#endif
char **kgFontNames(void) {
  return t_fontnames();
}
void kgFreeFontNames(char **pt) {
  int i=0;
  while(pt[i] != NULL) free(pt[i++]);
  free(pt);
}
void  kgReview(DIG *G) {
    int v[4];
    int EVGAY;
    DIALOG *D;
    kgWC *wc;
    kgDC *dc;
    if(G->D == NULL) return;
    if(G->D_ON==0) {
//      kgSetAntialiasing(G,0);
      kgAntialiasedReview(G);
//      kgSetAntialiasing(G,1);
      return;
    }
    D = (DIALOG *)G->D;
    EVGAY=D->evgay;
    wc = G->wc;
    dc = G->dc;
    v[0]=dc->D_x;
    v[1] = EVGAY - (dc->D_y+dc->V_y);
    v[2] = dc->D_x+dc->V_x;
    v[3]=  EVGAY - dc->D_y;
    if(dc->ZBUFF==1) {uiclear_zbuf(G);}
    uiUpdateOff(D);
    uihoreview(G);
    if (dc->DOUBLE!=1) uiUpdateOn(D);
}
void uiUpdateViewport(DIG *G,void *img) {
  int v_x1,v_y1,v_x2,v_y2,D_x,D_y,V_x,V_y;
  kgDC *dc;
  kgWC *wc;
  DIALOG *D;
  int EVGAY,EVGAX;
  GMIMG *png=NULL;
  png = img;
  if(png==NULL) return;
  printf("uiUpdateViewport\n");
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
  if(G->D==NULL) return;
  else {
    D= G->D;
    D_x=G->x1+D->xo+v_x1;
    D_y = D->yo+G->y1+EVGAY-1-v_y2;
    kgImage(D,png,D_x,D_y,png->image_width,png->image_height,0.0,1.0);
  }
  return;
}
void  kgAntialiasedReview(DIG *G) {
    int v_x1,v_y1,v_x2,v_y2,D_x,D_y,V_x,V_y;
    int EVGAY,EVGAX;
    int xl,yl,w,h;
    int v[4];
    char flname[20]={"Junk.Gph"},Tmpdir[200];
    unsigned long *data=NULL;
    char *tpt;
    void *img;
    DIALOG *D;
    int Evgay;
    kgWC *wc;
    kgDC *dc;
    D = G->D;
    wc = G->wc;
    dc = G->dc;
    Evgay=D->evgay;
    if(dc->ZBUFF==1) {uiclear_zbuf(G);}
#if 0
    v[0]=dc->D_x;
    v[1] = Evgay - (dc->D_y+dc->V_y-1);
    v[2] = dc->D_x+dc->V_x-1;
    v[3]=  Evgay - dc->D_y;
    xl = v[0];
    yl = v[1]-1;
    w = (v[2]-v[0]);
    h = (v[3] -v[1]+1);
#else
    EVGAX= G->x2-G->x1+1;
    EVGAY= abs(G->y2-G->y1)+1;
    v_x1=dc->vu_x1*(float)EVGAX;
    v_y1= dc->vu_y1*(float)EVGAY;
    v_x2=dc->vu_x2*(float)EVGAX-1;
    v_y2= dc->vu_y2*(float)EVGAY-1;
    V_x=v_x2-v_x1+1;
    V_y=v_y2-v_y1+1;
    D_x=G->x1+D->xo+v_x1;
    D_y = D->yo+G->y1+EVGAY-1-v_y2;
    w = V_x;
    h = V_y;
    xl = D_x;
    yl = D_y;
#endif
    tpt = D->tmpdir;
    strcpy(flname,tpt);
    strcat(flname,"/Junk.Gph");
    kgBackupGph(G,flname);
    uiUpdateOff(D);
   img = uiGetAntialiasedImage(flname,w,h,dc->ls_list,G->MAG);
#if 1
//   kgClearView(G);
   if(img!=NULL) {
     ((GMIMG *)img)->bkgrclr=0;
     kgImage(D,img,xl,yl,w,h,0.0,1.0);
     uiFreeImage(img);
   }
   else printf( "img==NULL\n");
#else
   uiUpdateViewport(G,img);
   uiFreeImage(img);
#endif
   remove(flname);
//   if (dc->DOUBLE!=1) kgUpdateOn(D);
   uiUpdateOn(D);
}
void kgUpdateOn(void *D) {
#if 1
  DIG *g;
  DIA *d;
  int i=0;
  d = ((DIALOG *)D)->d;
  while((g=d[i].g) != NULL) {
    i++;
    if(g->code!='g') continue;
    if(g->D_ON==0) {
//       printf("CopyResized\n");
       uiCopyResizedImage(g);
    }
  }
#endif
  uiUpdateOn((DIALOG *)D);
  kgSync((DIALOG *)D);
}
void kgUpdateOff(void *D) {
  uiUpdateOff((DIALOG *)D);
}
int kgCheckButtonPress(void *D) {
  int ret;
  KBEVENT kbe;
  ret=  kgCheckEvent(D,&kbe);
  if(ret) {
    ret=0;
    if(kbe.event == 1) {
       ret=1;
       while(kgCheckEvent(D,&kbe));
    }
    else {
      while(1) {
         ret=  kgCheckEvent(D,&kbe);
         if(ret==0) return ret;
         if(kbe.event == 1) return 1;
      }
    }
  }
  return ret;
}
void kgDrawImage(DIG *G,void *imgfile,float x1,float y1,float x2, float y2){
     char buff[100];
     kgDC *dc;
     dc= G->dc;
     buff[99]='\0';
     strncpy(buff,imgfile,99);
     if(G->Rbuff != NULL) {
       uiwrite_bf(G,&L,1);
       uiwrite_bf(G,&I,1);
       uiwrite_bf(G,buff,100);
       uiwrite_bf(G,&x1,4);
       uiwrite_bf(G,&y1,4);
       uiwrite_bf(G,&x2,4);
       uiwrite_bf(G,&y2,4);
     }
     if(G->D_ON){
       if((buff[0]!='#')||(buff[1]!='#')) {
        fprintf(stderr,"For Screen drawing filename should be ##<file>\n");
        fprintf(stderr,"Trying inserting ##\n");
	strcpy(buff,"##");
	strncat(buff,imgfile,97);
       }
       ui_drawimage(G,imgfile,x1,y1,x2,y2);
//       kgReview(G);
     }
     else img_drawimage(G,imgfile,x1,y1,x2,y2);
 }
void * gphStringToImage(char *Str,int xsize,int ysize,int font,int txtcolor,int justification,int width,int bkgr){
/* -1 left justification ; 0 center ;1 right */
   int fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw;
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   if(fid >=0 ) {
      gphUserFrame(fid,0.,0.,(float)xsize,(float)ysize);
      gphTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(th> 0.6*ysize) th = 0.6*ysize;
      gphTextSize(fid,th,tw,0.0,0.0,0.0,(float)xsize,(float)ysize);
      gphStringLength(Str,&length,font,tw,0.0);
      if(length>xsize) {
        fac = xsize/length;
        gphTextSize(fid,th,tw*fac,0.0,0.0,0.0,(float)xsize,(float)ysize);
        gphStringLength(Str,&length,font,tw*fac,0.0);
      }
      if(bkgr>=0) gphBoxFill(fid,0.,0.,(float)xsize,(float)ysize,bkgr,0);
      gphTextColor(fid,txtcolor);
      switch(justification) {
        case -1:
         gphMove2f(fid,0.0,0.25*ysize);
         break;
        case  0:
         gphMove2f(fid,(xsize-length)*.5,0.25*ysize);
         break;
        case  1:
         gphMove2f(fid,(xsize-length-tw*0.5),0.25*ysize);
         break;
        default:
         gphMove2f(fid,(xsize-length)*.5,0.25*ysize);
         break;
      }
      gphWriteText(fid,Str);
      close(fid);
      img=kgGphtoImage(flname,xsize,ysize,0x00000000);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return img;
}
void * gphFilledStringToImagePressed(char *Str,void *image,int xsize,int ysize,int font,int fillcolor,int highli,int color,int justification,int width,float rfac,int state){
/* -1 left justification ; 0 center ;1 right */
   int fid,r,g,b,aspc;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,h,s,v,red,blue,green,size;
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   size = xsize;
   aspc=1;
   if(ysize > size) {size=ysize;aspc=0;}
   if(fid >=0 ) {
      gphUserFrame(fid,-3.,-3.,(float)xsize+3.,(float)ysize+3.);
      gphTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
      gphLineWidth(fid,4);
      kgGetDefaultRGB(fillcolor,&r,&g,&b);
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      HSVtoRGB(&red,&green,&blue,h,s,0.95*v);
      gphChangeColor(fid,252,(int)red,(int)green,(int)blue);
      gphRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+3,(float)ysize+3,0,0,rfac);
//TCB
      gphLineWidth(fid,7);
      gphLineWidth(fid,5);
      gphRoundedRectangleLowered(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+3,(float)ysize+3,fillcolor,rfac);
//      gphLineWidth(fid,3);
//      gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+1.0,(float)ysize+2.0,0,rfac);
//      gphRoundedRectangleFill(fid,(float)xsize*0.5-0.5,(float)ysize*0.5+0.5,(float)xsize-1,(float)ysize-1,0,252,rfac);
      gphRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,252,rfac);
      gphLineColor(fid,color);
      if((Str != NULL) &&(Str[0]!='\0')) {
        gphTextSize(fid,th,tw,0.0,0.0,0.0,(float)xsize,(float)ysize);
        gphStringLength(Str,&length,font,tw,0.0);
        if(length>size) {
          fac = size/length;
          gphTextSize(fid,th,tw*fac,0.0,0.0,0.0,(float)xsize,(float)ysize);
          gphStringLength(Str,&length,font,tw*fac,0.0);
        }
        if(!aspc) gphTextAngle(fid,90.);
        gphTextColor(fid,color);
        switch(justification) {
          case -1:
           if(aspc) gphMove2f(fid,0.0+1.0,0.5*ysize-th*.5-1.0);
           else     gphMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) gphMove2f(fid,(xsize-length)*.5+1.0,0.5*ysize-th*.5-1.0);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) gphMove2f(fid,(xsize-length-tw*0.5)+1.0,0.5*ysize-th*.5-1.0);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) gphMove2f(fid,(xsize-length)*.5+1.0,0.5*ysize-th*0.5-1.0);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        gphWriteText(fid,Str);
      }
      close(fid);
      img=kgGphtoImage(flname,xsize,ysize,0x00000000);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return img;
}
void * gphFilledStringToImage1(char *Str,int xsize,int ysize,int font,int fillcolor,int highli,int color,int bodr,int justification,int width,float rfac,int state){
/* -1 left justification ; 0 center ;1 right */
   int fid,aspc;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green;
   int r,g,b;
#if 0
   if(state == -1) {
     return gphFilledStringToImagePressed(Str,xsize,ysize,font,fillcolor,highli,color,justification,width,rfac,state);
   }
#endif
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   aspc=1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   if(fid >=0 ) {
      gphUserFrame(fid,-3.,-3.,(float)xsize+3.,(float)ysize+3.);
      gphTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
      gphLineWidth(fid,4);
      gphRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,rfac);
      if(highli){
         gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+2,(float)ysize+2,0,rfac,1.0);
      }
      else {
         gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+1,(float)ysize+1,0,rfac,0.5);
      }
      gphLineWidth(fid,4);
      if(state==-1)
      gphRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+2,(float)ysize+2,bodr,rfac,3.0);
      else 
      gphRoundedRectangleRaised(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,3.0);
      if((Str != NULL) &&(Str[0]!='\0')) {
        gphTextSize(fid,th,tw,0.0,0.0,0.0,(float)xsize,(float)ysize);
        gphStringLength(Str,&length,font,tw,0.0);
        if(length>size) {
          fac = size/length;
          gphTextSize(fid,th,tw*fac,0.0,0.0,0.0,(float)xsize,(float)ysize);
          gphStringLength(Str,&length,font,tw*fac,0.0);
        }
        if(!aspc) gphTextAngle(fid,90.);
        gphTextColor(fid,color);
        switch(justification) {
          case -1:
           if(aspc) gphMove2f(fid,0.0,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) gphMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        gphWriteText(fid,Str);
      }
      close(fid);
      img=kgGphtoImage(flname,xsize,ysize,0x00000000);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return img;
}
void * gphFilledStringToImage2(char *Str,int xsize,int ysize,int font,int fillcolor,int highli,int color,int bodr,int justification,int width,float rfac,int state){
/* -1 left justification ; 0 center ;1 right */
   int fid,aspc;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green;
   int r,g,b;
#if 0
   if(state == -1) {
     return gphFilledStringToImagePressed(Str,xsize,ysize,font,fillcolor,highli,color,justification,width,rfac,state);
   }
#endif
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   aspc=1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   if(fid >=0 ) {
      gphUserFrame(fid,-3.,-3.,(float)xsize+3.,(float)ysize+3.);
      gphTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
      gphLineWidth(fid,1);
      if(highli){
        gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+5.0,(float)ysize+5.0,0,rfac,0.5);
      }
      gphLineWidth(fid,4);
      gphRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,rfac);
      gphLineWidth(fid,4);
      if(state==-1)
      gphRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+4,(float)ysize+4,bodr,rfac,3.0);
      else 
      gphRoundedRectangleRaised(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,3.0);
//      gphRoundedRectangleRaised(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize-1,(float)ysize-1,fillcolor,rfac);
      if((Str != NULL) &&(Str[0]!='\0')) {
        gphTextSize(fid,th,tw,0.0,0.0,0.0,(float)xsize,(float)ysize);
        gphStringLength(Str,&length,font,tw,0.0);
        if(length>size) {
          fac = size/length;
          gphTextSize(fid,th,tw*fac,0.0,0.0,0.0,(float)xsize,(float)ysize);
          gphStringLength(Str,&length,font,tw*fac,0.0);
        }
        if(!aspc) gphTextAngle(fid,90.);
        gphTextColor(fid,color);
        switch(justification) {
          case -1:
           if(aspc) gphMove2f(fid,0.0,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) gphMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        gphWriteText(fid,Str);
      }
      close(fid);
      img=kgGphtoImage(flname,xsize,ysize,0x00000000);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return img;
}
void * gphShadedStringToImage(char *Str,int xsize,int ysize,int font,int fillcolor,int highli,int color,int justification,int width,float rfac,int state,int type){
/* -1 left justification ; 0 center ;1 right */
   int fid,aspc=1;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,vmin,vmax,size,lnwidth=2;
   float h,s,v,red,blue,green;
   int r,g,b;
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   aspc =1;
   size = xsize;
   kgGetDefaultRGB(fillcolor,&r,&g,&b);
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
//   v =(v*v*2.0)*(2-v);
//   v *=1.4;
   if(v> 1) v=1.0;
   if(ysize > size) {size=ysize;aspc=0;}
   if(fid >=0 ) {
      gphUserFrame(fid,-2.,-2.,(float)xsize+2.,(float)ysize+2.);
      gphTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
      gphLineColor(fid,color);
      gphLineWidth(fid,2);
      lnwidth=2;
//TCB
      vmin = 0.85*v;
      vmax =1.2*v;
      if(vmax >1.0) vmax=1.0;
      vmin= vmax*0.75;
      if(state==-1) {
        highli=0; gphLineWidth(fid,5);
        vmin =0.4*v;vmax=0.8*v;
        lnwidth=5;
      }
      else {
        if(highli) gphLineWidth(fid,5);
        lnwidth=5;
      }
//      gphRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+lnwidth*0.5,(float)ysize+lnwidth*0.5,1,color,rfac);
      switch(type) {
        case 0:
         gphRoundedRectangleShade0(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,vmin,vmax);
        break;
        case 1:
         gphRoundedRectangleShade1(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,vmin,vmax);
        break;
        case 2:
         gphRoundedRectangleShade4(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,vmin,vmax);
        break;
        case 3:
         gphRoundedRectangleShade(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,vmin,vmax);
        break;
        default:
         gphRoundedRectangleShade4(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,vmin,vmax);
        break;
           
      }
//TCB
      if(highli)gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,color,rfac,1.0);
      else gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,color,rfac,0.5);
      if((Str != NULL) &&(Str[0]!='\0')) {
        gphTextSize(fid,th,tw,0.0,0.0,0.0,(float)xsize,(float)ysize);
        gphStringLength(Str,&length,font,tw,0.0);
        if(length>size) {
          fac = size/length;
          gphTextSize(fid,th,tw*fac,0.0,0.0,0.0,(float)xsize,(float)ysize);
          gphStringLength(Str,&length,font,tw*fac,0.0);
        }
  //      gphBoxFill(fid,0.,0.,(float)xsize,(float)ysize,bkcolor,0);
        if(!aspc) gphTextAngle(fid,90.);
        gphTextColor(fid,color);
        switch(justification) {
          case -1:
           if(aspc) gphMove2f(fid,0.0,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) gphMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        gphWriteText(fid,Str);
      }
      close(fid);
      img=kgGphtoImage(flname,xsize,ysize,0x00000000);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return img;
}
void * gphBoxedStringToImage(char *Str,int xsize,int ysize,int font,int border,int highli,int color,int justification,int width,float rfac,int state){
/* -1 left justification ; 0 center ;1 right */
   int fid,aspc=1;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green,vorg;
   int r,g,b;
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   aspc =1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   if(fid >=0 ) {
      gphUserFrame(fid,-0.5,-0.5,(float)xsize+0.5,(float)ysize+0.5);
      gphTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
      gphLineWidth(fid,11);
      kgGetDefaultRGB(border,&r,&g,&b);
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      vorg =v;
      if(state==-1) {
        v = 0.5*v;
        gphLineWidth(fid,7);
      }
      else {
        if(highli) {
           v *=1.15; 
           if(v > 1.0) {
             v=1.0;
             h *=0.8;
             s*=0.8;
           }
        }
      }
      vorg =v;
      HSVtoRGB(&red,&green,&blue,h,s,v);
      gphChangeColor(fid,252,(int)red,(int)green,(int)blue);
      gphRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,border,rfac);
      gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,252,rfac,1.0);
#if 0
      HSVtoRGB(&red,&green,&blue,h,s,vorg);
      gphChangeColor(fid,252,(int)red,(int)green,(int)blue);
      gphLineColor(fid,252);
      gphLineWidth(fid,3);
      gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize-1,(float)ysize-1,252,rfac,1.0);
#endif
      if((Str != NULL) &&(Str[0]!='\0')) {
        gphTextSize(fid,th,tw,0.0,0.0,0.0,(float)xsize,(float)ysize);
        gphStringLength(Str,&length,font,tw,0.0);
        if(length>size) {
          fac = size/length;
          gphTextSize(fid,th,tw*fac,0.0,0.0,0.0,(float)xsize,(float)ysize);
          gphStringLength(Str,&length,font,tw*fac,0.0);
        }
        if(!aspc) gphTextAngle(fid,90.);
  //      gphBoxFill(fid,0.,0.,(float)xsize,(float)ysize,bkcolor,0);
        gphTextColor(fid,color);
        switch(justification) {
          case -1:
           if(aspc) gphMove2f(fid,0.0,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) gphMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        gphWriteText(fid,Str);
      }
      close(fid);
      img=kgGphtoImage(flname,xsize,ysize,0x00000000);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return img;
}
void  gphStringToImagefile(char *Imgfile,char *Str,int xsize,int ysize,int font,int txtcolor,int justification,int width){
/* -1 left justification ; 0 center ;1 right */
   int fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw;
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   if(fid >=0 ) {
      gphUserFrame(fid,0.,0.,(float)xsize,(float)ysize);
      gphTextFont(fid,font);
      th = (float)ysize*.5;
      tw = ((float)width);
      th = HFAC*tw;
      if(th> 0.6*ysize) th = 0.6*ysize;
      gphTextSize(fid,th,tw,0.0,0.0,0.0,(float)xsize,(float)ysize);
      gphStringLength(Str,&length,font,tw,0.0);
      if(length>xsize) {
        fac = xsize/length;
        gphTextSize(fid,th,tw*fac,0.0,0.0,0.0,(float)xsize,(float)ysize);
        gphStringLength(Str,&length,font,tw*fac,0.0);
      }
//      gphBoxFill(fid,0.,0.,(float)xsize,(float)ysize,bkcolor,0);
      gphTextColor(fid,txtcolor);
      switch(justification) {
        case -1:
         gphMove2f(fid,0.0,0.25*ysize);
         break;
        case  0:
         gphMove2f(fid,(xsize-length)*.5,0.25*ysize);
         break;
        case  1:
         gphMove2f(fid,(xsize-length-tw*0.5),0.25*ysize);
         break;
        default:
         gphMove2f(fid,(xsize-length)*.5,0.25*ysize);
         break;
      }
      gphWriteText(fid,Str);
      close(fid);
      kgGphtoImagefile(Imgfile,flname,xsize,ysize,0x00000000);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return ;
}
float  kgStringLength(void *G,char *title) {
       float length=0;
       uistrlngth(G,title,&length);
       return length;
}
float  uiStringLength(char *title,int width) {
      float length=0,*xdsp;
      float wd,gp,fj,fjl,gj,val,fact,fact1=1.0,hfact=1.0;
      short ngp,n,i,j,k,greek=0;
      int font_o,Nu,De;
      wd= width;
      xdsp = &length;
      gp = 0.0;
      ngp =1;
      j =0; while( title[j] != '\0')j++;
      if(j==0){ *xdsp=0.;return(0.0);}
      fj =0.0; gj = -1; fact = 1.0;
      fjl=0.;
      i=0;
      while(i<j) 
      {
       if( title[i] != '!') {
            fact1=1.0*fact;
            gj +=1.;
            fjl +=1.0;
            fj +=(fact1);greek=0;
       }
       else { 
             i=i+1; 
             if(i>=j)break;
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
                        break;
               case 'k':
                        break;
               case '!':
                         fj = fj+fact1;fjl+=1.; gj = gj+1;
                         break;
               case '%':
                         fj = fjl;
                         break;
               case 'f':
                         i+=2;
                         break;
               case 'c':
                         i+=2;
                         break;
               case 'z':
                         if(i+2 >= j) break;
                         Nu= (title[i+1] -'0');
                         De= (title[i+2] -'0');
                         if( De == 0) De=1;
                         val = (float)Nu/(float) De;
                         if(val == 0.0) val=1.0;
                         fact = fact*val;
                         i = i+2;
                         break;
               case 'h':
                         if(i+2 >= j) break;
                         Nu= (title[i+1] -'0');
                         De= (title[i+2] -'0');
                         if( De == 0) De=1;
                         val = (float)Nu/(float) De;
                         if(val == 0.0) val=1.0;
                         hfact = hfact*val;
                         i = i+2;
                         break;
               case 'w':
                         if(i+2 >= j) break;
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
      *xdsp =(fj*wd);
      ngp = gj+0.1;
      return length;
  }
void * gphSplashStringToImage_o(char *Str,int xsize,int ysize,int font,int fillcolor,int color,int bodrcolor,int justification,int width,float rfac){
/* -1 left justification ; 0 center ;1 right */
   int fid,aspc;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green;
   int r,g,b;
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   aspc=1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   if(fid >=0 ) {
      gphUserFrame(fid,-0.,-0.,(float)xsize+0.,(float)ysize+0.);
      gphTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
#if 0
      gphLineWidth(fid,3);
      if(highli) gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+4.0,(float)ysize+4.0,0,rfac);
#else
//      gphRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,0,rfac);
//      gphLineWidth(fid,2);
//      gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,rfac);
#endif
//      gphLineWidth(fid,4);
      gphRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize-3,(float)ysize-3,0,fillcolor,rfac);
//TCB
      gphLineWidth(fid,9);
      gphRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize-3,(float)ysize-3,bodrcolor,rfac,5.0);
      gphLineColor(fid,color);
      if((Str != NULL) &&(Str[0]!='\0')) {
        gphTextSize(fid,th,tw,0.0,0.0,0.0,(float)xsize-2,(float)ysize-2);
        gphStringLength(Str,&length,font,tw,0.0);
        if(length>size) {
          fac = size/length;
          gphTextSize(fid,th,tw*fac,0.0,0.0,0.0,(float)xsize-2,(float)ysize-2);
          gphStringLength(Str,&length,font,tw*fac,0.0);
        }
        if(!aspc) gphTextAngle(fid,90.);
        gphTextColor(fid,color);
        switch(justification) {
          case -1:
           if(aspc) gphMove2f(fid,0.0,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) gphMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        gphWriteText(fid,Str);
      }
      close(fid);
      img=kgGphtoImage(flname,xsize,ysize,0x00000000);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return img;
}
void * gphSplashStringToImage(char *Str,int xsize,int ysize,int font,int fillcolor,int color,int bodrcolor,int justification,int width,float rfac){
/* -1 left justification ; 0 center ;1 right */
   int fid,aspc;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green,vmax,vmin;
   int r,g,b;
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   aspc=1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   if(fid >=0 ) {
      gphUserFrame(fid,-0.,-0.,(float)xsize+0.,(float)ysize+0.);
      kgGetDefaultRGB(fillcolor,&r,&g,&b);
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      vmax = v*1.8;
      if(vmax > 1.0) vmax=1.0;
      vmin = v*0.4;
      HSVtoRGB(&red,&green,&blue,h,s,vmax);
      gphChangeColor(fid,251,(int)red,(int)green,(int)blue);
      HSVtoRGB(&red,&green,&blue,h,s,vmin);
      gphChangeColor(fid,252,(int)red,(int)green,(int)blue);
      gphTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
      gphRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize-1,(float)ysize-1,0,252,rfac);
      gphRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,bodrcolor,rfac,3.0);
      gphLineColor(fid,color);
      if((Str != NULL) &&(Str[0]!='\0')) {
        gphTextSize(fid,th,tw,0.0,0.0,0.0,(float)xsize-2,(float)ysize-2);
        gphStringLength(Str,&length,font,tw,0.0);
        if(length>size) {
          fac = size/length;
          gphTextSize(fid,th,tw*fac,0.0,0.0,0.0,(float)xsize-2,(float)ysize-2);
          gphStringLength(Str,&length,font,tw*fac,0.0);
        }
        if(!aspc) gphTextAngle(fid,90.);
        gphTextColor(fid,color);
        switch(justification) {
          case -1:
           if(aspc) gphMove2f(fid,0.0,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) gphMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        gphWriteText(fid,Str);
      }
      close(fid);
      img=kgGphtoImage(flname,xsize,ysize,0x00000000);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return img;
}
void * gphSplashStringToImage_n(char *Str,int xsize,int ysize,int font,int fillcolor,int color,int bodrcolor,int justification,int width,float rfac){
/* -1 left justification ; 0 center ;1 right */
   int fid,aspc;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green,vmax,vmin,x[6],y[6];
   int r,g,b;
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   aspc=1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   if(fid >=0 ) {
      gphUserFrame(fid,-1.,-1.,(float)xsize+1.,(float)ysize+1.);
      kgGetDefaultRGB(fillcolor,&r,&g,&b);
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      vmax = v*1.5;
      if(vmax > 1.0) vmax=1.0;
      vmin = v*0.4;
      HSVtoRGB(&red,&green,&blue,h,s,vmax);
      gphChangeColor(fid,251,(int)red,(int)green,(int)blue);
      HSVtoRGB(&red,&green,&blue,h,s,vmin);
      gphChangeColor(fid,252,(int)red,(int)green,(int)blue);
      vmax = v*1.2;
      if(vmax > 1.0) vmax=1.0;
      vmin = v*0.1;
      HSVtoRGB(&red,&green,&blue,h,s,vmax);
      gphChangeColor(fid,253,(int)red,(int)green,(int)blue);
      HSVtoRGB(&red,&green,&blue,h,s,vmin);
      gphChangeColor(fid,254,(int)red,(int)green,(int)blue);
      gphTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
#if 0
      gphLineWidth(fid,3);
      if(highli) gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+4.0,(float)ysize+4.0,0,rfac);
#else
//      gphLineWidth(fid,2);
//      gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+2.0,(float)ysize+2.0,0,0.0);
#endif
//      gphLineWidth(fid,4);
      gphRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,0.0);
      x[1]=-2.0; x[2]=xsize+2,x[5]=xsize+2,x[4]=xsize-2;x[3]=xsize-2;x[0]=+2;
      y[1]=-2.0; y[2]=-2.0,y[5]=ysize+2,y[4]=ysize-2;y[3]=2;y[0]=2;
      gphPolyFill(fid,4,x,y,0,251);
      x[1]=-2.0; x[2]=xsize+2,x[3]=xsize+2,x[4]=xsize-2;x[5]=xsize-2;x[0]=+2;
      y[1]=-2.0; y[2]=-2.0,y[3]=ysize+2,y[4]=ysize-2;y[5]=2;y[0]=2;
      gphPolyFill(fid,4,x+2,y+2,0,253);
      x[1]=-2.0; x[2]=-2,x[5]=xsize+2,x[4]=xsize-2;x[3]=+2;x[0]=+2;
      y[1]=-2.0; y[2]=ysize+2.0,y[5]=ysize+2,y[4]=ysize-2;y[3]=ysize-2;y[0]=2;
      gphPolyFill(fid,4,x,y,0,254);
      x[1]=-2.0; x[2]=-2,x[3]=xsize+2,x[4]=xsize-2;x[5]=+2;x[0]=+2;
      y[1]=-2.0; y[2]=ysize+2.0,y[3]=ysize+2,y[4]=ysize-2;y[5]=ysize-2;y[0]=2;
      gphPolyFill(fid,4,x+2,y+2,0,252);
//TCB
//      gphLineWidth(fid,12);
//      gphRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,0.0);
      gphLineColor(fid,color);
      if((Str != NULL) &&(Str[0]!='\0')) {
        gphTextSize(fid,th,tw,0.0,0.0,0.0,(float)xsize,(float)ysize);
        gphStringLength(Str,&length,font,tw,0.0);
        if(length>size) {
          fac = size/length;
          gphTextSize(fid,th,tw*fac,0.0,0.0,0.0,(float)xsize,(float)ysize);
          gphStringLength(Str,&length,font,tw*fac,0.0);
        }
        if(!aspc) gphTextAngle(fid,90.);
        gphTextColor(fid,color);
        switch(justification) {
          case -1:
           if(aspc) gphMove2f(fid,0.0,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) gphMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) gphMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     gphMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        gphWriteText(fid,Str);
      }
      close(fid);
      img=kgGphtoImage(flname,xsize,ysize,0x00000000);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return img;
}
int uiPressedBoxFill_o(void *Dialog,int xo,int yo,int xsize,int ysize,int fillcolor,int bodr,int bodrsize,float rfac){
/* -1 left justification ; 0 center ;1 right */
   int fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   if(fid >=0 ) {
      gphUserFrame(fid,0.,0.,(float)xsize,(float)ysize);
      gphRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,rfac);
      gphRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,bodr,rfac,(float)bodrsize);
      close(fid);
      img=kgGphtoImage(flname,xsize,ysize,0x00000000);
      kgImage(Dialog,img,xo,yo,xsize,ysize,0.0,1.0);
      uiFreeImage(img);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return 1;
}
int uiBoxFill_o(void *Dialog,int xo,int yo,int xsize,int ysize,int fillcolor,float rfac){
/* -1 left justification ; 0 center ;1 right */
   int fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   if(fid >=0 ) {
      gphUserFrame(fid,0.,0.,(float)xsize,(float)ysize);
      gphRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,rfac);
      close(fid);
      img=kgGphtoImage(flname,xsize,ysize,0x00000000);
      kgImage(Dialog,img,xo,yo,xsize,ysize,0.0,1.0);
      uiFreeImage(img);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return 1;
}
int uiRoundedBorder_o(void *Dialog,int xo,int yo,int xsize,int ysize,int bodrcolor,int bodrsize,float rfac){
/* -1 left justification ; 0 center ;1 right */
   int fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   tmpdir=kgMakeTmpDir();
   sprintf(flname,"%-s/Tmp.gph",tmpdir);
   fid = open(flname,O_CREAT|O_TRUNC|O_RDWR,0700);
   if(fid >=0 ) {
      gphUserFrame(fid,0.,0.,(float)xsize,(float)ysize);
      gphRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,bodrcolor,rfac,(float)bodrsize);
      close(fid);
      img=kgGphtoImage(flname,xsize,ysize,0x00000000);
      kgImage(Dialog,img,xo,yo,xsize,ysize,0.0,1.0);
      uiFreeImage(img);
   }
   kgCleanDir(tmpdir);
   free(tmpdir);
   return 1;
}
int kgRoundedRectangleRing2(void * fid,float xo,float yo,float xl,float yl,float red,float green,float blue,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   float vmin,vmax;
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float xd1[800],yd1[800],vd1[800];
   float dl,xp,yp,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1,fac2;
   double dl1,dl2;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   if(bordrsize > size*0.5) bordrsize=size*0.5;
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize*0.5;
   fac1 = dl1/(size-bordrsize);
   dl2 = dl -bordrsize;
   fac2 = dl2/(size-2*bordrsize);
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   RGBtoHSV(red,green,blue,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   vmax=vv*1.8;
   if(vmax > 1.) vmax=1.;
   if(vmax < 0.4 ) vmax=0.4;
   vmin=vv+(vmax -vv)*.2;
   if(vmin > 1.0) vmin=1.0;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin1= vv;
       vmax1 = vv;
       vmin = vlow;
       vmax = vlow;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,xd,yd) - 2;
       np =kgGetEllipsePoints( xl-bordrsize,yl-bordrsize,fac1, count,x, y)-2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, count,xd1, yd1)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         vd[i]= vmin;
         v[i]= vmax;
         vd1[i]= vmin;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, xd, yd, vd,vv,vmin, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, x, y, vd,vv,vmax, count);
   }
   kgMixHsvColors(fid,200,64,h,s,vmin,vmax);
   kgSetShadeParams(fid,vmax,vmin,200, 64, 0);
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int kgRoundedRectangleRing1(void * fid,float xo,float yo,float xl,float yl,float red,float green,float blue,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   float vmin,vmax;
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float xd1[800],yd1[800],vd1[800];
   float dl,xp,yp,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1,fac2;
   double dl1,dl2;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   if(bordrsize > size*0.5) bordrsize=size*0.5;
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize*0.5;
   fac1 = dl1/(size-bordrsize);
   dl2 = dl -bordrsize;
   fac2 = dl2/(size-2*bordrsize);
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   RGBtoHSV(red,green,blue,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   if(vv > 0.7) vv=.7;
   vmax=vv*1.8;
   if(vmax > 1.) vmax=1.;
   if(vmax < 0.4 ) vmax=0.4;
   vmin=vv+(vmax -vv)*.6;
   if(vmin > 1.0) vmin=1.0;
   vmid = vmax*0.95;
   vlow= vmin;
   if(vv > 1 ) vv=1;
   if(vlow > 1 ) vlow=1;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin1= vv;
       vmax1 = vv;
       vmin = vlow;
       vmax = vlow;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,xd,yd) - 2;
       np =kgGetEllipsePoints( xl-bordrsize,yl-bordrsize,fac1, count,x, y)-2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, count,xd1, yd1)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         vd[i]= vmin;
         v[i]= vmax;
         vd1[i]= vmin;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, xd, yd, vd,vv,vv, count);
       np =kgGetCirclePoints( xl-bordrsize,yl-bordrsize,fac1, x, y, v,vmin,vmax, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, xd1, yd1, vd1,vv,vv, count);
   }
   kgMixHsvColors(fid,200,64,h,s,vv,vmax);
   kgSetShadeParams(fid,vmax,vv,200, 64, 0);
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd1[npj];
     y[npi]= yd1[npj];
     v[npi]= vd1[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int kgRoundedRectangleRing3(void * fid,float xo,float yo,float xl,float yl,float red,float green,float blue,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   float vmin,vmax;
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float xd1[800],yd1[800],vd1[800];
   float dl,xp,yp,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1,fac2;
   double dl1,dl2;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   if(bordrsize > size*0.5) bordrsize=size*0.5;
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize*0.5;
   fac1 = dl1/(size-bordrsize);
   dl2 = dl -bordrsize;
   fac2 = dl2/(size-2*bordrsize);
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   RGBtoHSV(red,green,blue,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   if(vv > 0.7) vv=.7;
   vmax=vv*1.8;
   if(vmax > 1.) vmax=1.;
   if(vmax < 0.4 ) vmax=0.4;
   vmin=vv+(vmax -vv)*.6;
   if(vmin > 1.0) vmin=1.0;
   vmid = vmax*0.95;
   vlow= vmin;
   if(vv > 1 ) vv=1;
   if(vlow > 1 ) vlow=1;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin1= vv;
       vmax1 = vv;
       vmin = vlow;
       vmax = vlow;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,xd,yd) - 2;
       np =kgGetEllipsePoints( xl-1.8*bordrsize,yl-1.8*bordrsize,fac1, count,x, y)-2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, count,xd1, yd1)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         vd[i]= vmin;
         v[i]= vmax;
         vd1[i]= vmin;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, xd, yd, vd,vv,vv, count);
       np =kgGetCirclePoints( xl-1.8*bordrsize,yl-1.8*bordrsize,fac1, x, y, v,vmin,vmax, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, xd1, yd1, vd1,vv,vv, count);
   }
   kgMixHsvColors(fid,200,64,h,s,vv,vmax);
   kgSetShadeParams(fid,vmax,vv,200, 64, 0);
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd1[npj];
     y[npi]= yd1[npj];
     v[npi]= vd1[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int kgRoundedRectangleRing(void *fid,float xo,float yo,float xl,float yl,float red,float green,float blue,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   float vmin,vmax;
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float xd1[800],yd1[800],vd1[800];
   float dl,xp,yp,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1,fac2;
   double dl1,dl2;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   if(bordrsize > size*0.5) bordrsize=size*0.5;
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize*0.5;
   fac1 = dl1/(size-bordrsize);
   dl2 = dl -bordrsize;
   fac2 = dl2/(size-2*bordrsize);
//   fac1=0.5;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   RGBtoHSV(red,green,blue,&h,&s,&vv);
//   printf("%f %f %f %f %f %f\n",red ,green,blue,h,s,vv);
   horg = h;
   sorg =s;
   vorg = vv;
   vmax=vv*1.4;
   if(vmax > 1.0) vmax=1.0;
   vmid = vmax*0.95;
//   vmid = vmax;
   vmin = 0.7*vmax;
   vv = vmax;
   vlow= vmin;
   if(vv > 1 ) vv=1;
   if(vlow > 1 ) vlow=1;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin1= vv;
       vmax1 = vv;
       vmin = vlow;
       vmax = vlow;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,xd,yd) - 2;
       np =kgGetEllipsePoints( xl-bordrsize,yl-bordrsize,fac1, count,x, y)-2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, count,xd1, yd1)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         vd[i]= vmin;
         v[i]= vmax;
         vd1[i]= vmin;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, xd, yd, vd,vlow,vlow, count);
       np =kgGetCirclePoints( xl-bordrsize,yl-bordrsize,fac1, x, y, v,vmax,vmax, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac2, xd1, yd1, vd1,vlow,vlow, count);
   }
   kgMixHsvColors(fid,200,128,h,s,vmin,vmax);
   kgSetShadeParams(fid,vmax,vmin,200, 128, 0);
   for(i=0;i<np;i++) {
    v[i]=vmid;
   }
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   vd[3]= vmax;
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
    v[i]=vmax;
   }
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   vd[1]= vmid;
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
    v[i]=vmid;
   }
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd1[npj];
     y[npi]= yd1[npj];
     v[npi]= vd1[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
   vd[3]= vmax;
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
    v[i]=vmax;
   }
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= vmid;
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
    v[i]=vmid;
   }
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int kgRoundedRectangleFill(void * fid,float xo,float yo,float xl,float yl,int ib,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp;
   double ang,dang;
   int i,j;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   count = 4*count2 ;
   for(i=0;i<count;i++) {
     x[i] +=xo;
     y[i] +=yo;
     
   }
   if(clr <0 ){
         int r,g,b;
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 77;
         kgChangeColor(fid,clr,r,g,b);
   }
   kgPolyFill(fid,count,x,y,ib,clr);
   return 1;
}
int kgSmallRectangleFill(void * fid,float xo,float yo,float xl,float yl,int ib,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=15,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp;
   double ang,dang;
   int i,j;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=6;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   count = 4*count2 ;
   for(i=0;i<count;i++) {
     x[i] +=xo;
     y[i] +=yo;
     
   }
   if(clr <0 ){
         int r,g,b;
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 77;
         kgChangeColor(fid,clr,r,g,b);
   }
   kgPolyFill(fid,count,x,y,ib,clr);
   return 1;
}
int kgRoundedRectangleShade_o(void * fid,float xo,float yo,float xl,float yl,int type,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp;
   int r,g,b;
   float h,s,vv;
   double ang,dang;
   int i,j;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   if(vv < 0.5) vv=0.5;
   if(vv>0.6) vv=0.6;
   vv =0.5;
   count1 = count+1;
   count2 = count+2;
//   kgmixshadecolors_hsv(fid,48,16, h, s, vv) ;
   kgMixHsvColors(fid,200,16,h,s,0.5,1.0);
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   x[0]=dl +xp;
   y[0]= yp;
   for(i=1;i<count1;i++) {
     ang +=dang;
     x[i]= dl*cos(ang)+xp;
     y[i]= dl*sin(ang)+yp;
   }
   x[count]= xp;
   y[count]= dl+yp;
   for(i=1;i<count2;i++) {
     x[count+i]= -x[count1-i];
     y[count+i] = y[count1-i];
   }
   for(i=1;i<(count1+count2);i++) {
     x[2*count+1+i]= x[2*count1-i];
     y[2*count+1+i] = -y[2*count1-i];
   }
   for(i=0;i<4*count1;i++) {
     v[i] = y[i];
     x[i] +=xo;
     y[i] +=yo;
     
   }
   kgSetShadeParams(fid,yl*0.5, -yl*0.5,200, 16, 0);
   kgGouraudFill(fid,4*count1,x,y,v);
   return 1;
}

int kgRoundedRectangleShade4(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
   float bordrsize=3;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1;
   double dl1;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   bordrsize = size*0.1;
   if(bordrsize< 4) bordrsize=4;
   if(bordrsize > size*0.3) bordrsize=size*0.3;
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize;
   fac1 = dl1/(size-2*bordrsize);
//   fac1=0.5;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
//TCB
//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   vmid = vmax;
   vv = (vmin+0.4*(vmax-vmin));
//   if(vmid>vorg) vmid=vorg;
   vlow= vmin;
   if(vv > 1 ) vv=1;
   if(vlow > 1 ) vlow=1;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin1= vv;
       vmax1 = vv;
       vmin = vlow;
       vmax = vlow;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, count,xd, yd)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         v[i]= vmin+ss*ss*dv;
         vd[i]= vmid+ss*ss*dv1;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, x, y, v,vlow,vlow, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vmid,vmid, count);
   }
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   kgMixHsvColors(fid,200,128,h,s,vmin,vmax);
   kgSetShadeParams(fid,vmax,vmin,200, 128, 0);
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i]= xo;
     yd[i]= yo;
     vd[i]= vv;
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   xd[0]= xo;
   yd[0]= yo;
   vd[0]= vv;
   xd[3]= xo;
   yd[3]= yo;
   vd[3]= vv;
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i]= xo;
     yd[i]= yo;
     vd[i]= vv;
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   xd[0]= xo;
   yd[0]= yo;
   vd[0]= vv;
   xd[1]= xo;
   yd[1]= yo;
   vd[1]= vv;
   kgGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   xd[0]= xo;
   yd[0]= yo;
   vd[0]= vv;
   xd[1]= xo;
   yd[1]= yo;
   vd[1]= vv;
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i]= xo;
     yd[i]= yo;
     vd[i]= vv;
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
//   kgPolyFill(fid,np,x,y,0,clr);
   for(i=0;i<np;i++) {
     xd[i]= xo;
     yd[i]= yo;
     vd[i]= vv;
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   xd[0]= xo;
   yd[0]= yo;
   vd[0]= vv;
   xd[3]= xo;
   yd[3]= yo;
   vd[3]= vv;
   kgGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int kgRoundedRectangleShade0(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp;
   int r,g,b;
   float h,s,vv;
   double ang,dang;
   int i,j;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   if(vv < 0.5) vv=0.5;
   if(vv>0.6) vv=0.6;
   vv =0.5;
   count1 = count+1;
   count2 = count+2;
   kgMixHsvColors(fid,200,128,h,s,vmin,vmax);
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   count = 4*count2 ;
   for(i=0;i<count;i++) {
     v[i] = +y[i];
     x[i] +=xo;
     y[i] +=yo;
     
   }
   kgSetShadeParams(fid,yl*0.5, -yl*0.5,200, 128, 0);
   kgGouraudFill(fid,count,x,y,v);
   return 1;
}
int kgRoundedRectangleShade1(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp,dis,D;
   int r,g,b;
   float h,s,vv;
   double ang,dang;
   int i,j;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   kgMixHsvColors(fid,200,128,h,s,vmin,vmax);
   count1 = count+1;
   count2 = count+2;
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) ;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
     x[count2+1]= 0.0;
     y[count2+1]= yp;
     x[count2+2]= 0.0;
     y[count2+2]= 0.0;
     count = count2+3;
   }
   for(i=0;i<count;i++) {
     v[i] = yl*0.5 - fabsf(y[i]);
     x[i] +=xo;
     y[i] +=yo;
     
   }
   kgSetShadeParams(fid,yl*0.5, 0.0,200, 128, 0);
   kgGouraudFill(fid,count,x,y,v);
   for(i=0;i<count;i++) {
     x[i] =2*xo-x[i];
   }
   kgGouraudFill(fid,count,x,y,v);
   for(i=0;i<count;i++) {
     y[i] =2*yo-y[i];
   }
   kgGouraudFill(fid,count,x,y,v);
   for(i=0;i<count;i++) {
     x[i] =2*xo-x[i];
   }
   kgGouraudFill(fid,count,x,y,v);
   return 1;
}
int kgRoundedRectangleShade(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp,dis,D;
   int r,g,b;
   float h,s,vv;
   double ang,dang;
   int i,j;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   kgMixHsvColors(fid,200,128,h,s,vmin,vmax);
   count1 = count+1;
   count2 = count+2;
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) ;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
     x[count2+1]= 0.0;
     y[count2+1]= yp;
     x[count2+2]= 0.0;
     y[count2+2]= 0.0;
     count = count2+3;
   }
   D = (xl*xl+yl*yl)*0.25;
   D = sqrtf(D);
   for(i=0;i<count;i++) {
     dis = x[i]*x[i]+y[i]*y[i];
     dis = sqrtf(dis);
     v[i] = 1.0 -dis/D;
     x[i] +=xo;
     y[i] +=yo;
     
   }
   kgSetShadeParams(fid,1.0, 0.0,200, 128, 0);
   kgGouraudFill(fid,count,x,y,v);
   for(i=0;i<count;i++) {
     x[i] =2*xo-x[i];
   }
   kgGouraudFill(fid,count,x,y,v);
   for(i=0;i<count;i++) {
     y[i] =2*yo-y[i];
   }
   kgGouraudFill(fid,count,x,y,v);
   for(i=0;i<count;i++) {
     x[i] =2*xo-x[i];
   }
   kgGouraudFill(fid,count,x,y,v);
   return 1;
}
int kgRoundedRectangle(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
//   float bordrsize=2.5;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1;
   double dl1;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
//   if(fac > 0.5) fac=0.5;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize;
   fac1 = dl1/(size-2*bordrsize);
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr=127;
         kgChangeColor(fid,clr,r,g,b);
   }
//TCB

//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   if(vv > 0.9)  { s = 0.5*sorg; }
   vv =vorg +(1.-vorg)*0.9;
   if(vv > 1 ) vv=1;
   vmid = vorg +(1-vorg)*0.5;
   if(vv== vmid) vmid=0.95*vv;
   vmid1 = vorg +(1-vorg)*0.45;
   if(vmid > 1 ) vmid=1;
   if(vmid1 > 1 ) vmid1=1;

   vlow = vorg*0.8;
   vlow1 = vorg*0.20;
#if 1
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin= vlow1;
       vmax = vmid;
       vmin1 = vlow;
       vmax1 = vv;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, count,xd, yd)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         v[i]= vmin+ss*ss*dv;
         vd[i]= vmin1+ss*ss*dv1;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, x, y, v,vlow1,vmid, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vlow,vv, count);
   }
#else
#if 1
   np =kgGetCirclePoints( xl,yl,fac, x, y, v,vlow1,vmid, count);
   np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vlow,vv, count);
#else
   np =kgGetCirclePoints( xl,yl,fac, x, y, v,vv,vlow, count);
   np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vmid,vlow1, count);
#endif
#endif
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
   }
//   kgMixHsvColors(fid,200,128,h,s,vlow1,vv);
//   kgSetShadeParams(fid,vv,vlow1,200, 128, 0);
   if(clr <0 ){
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
   kgLineColor(fid,clr);
   kgPolyFill(fid,np2,xd,yd,1,clr);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
//   kgGouraudFill(fid,4,xd,yd,vd);
   kgPolyFill(fid,4,xd,yd,1,clr);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   kgPolyFill(fid,np2,xd,yd,1,clr);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgPolyFill(fid,4,xd,yd,1,clr);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[3]= x[0];
   yd[3]= -y[0];
   vd[3]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[1]= x[np2-1];
   yd[1]=  y[np2-1];
   vd[1]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgPolyFill(fid,4,xd,yd,1,clr);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   kgPolyFill(fid,np2,xd,yd,1,clr);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   kgPolyFill(fid,np2,xd,yd,1,clr);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   kgPolyFill(fid,4,xd,yd,1,clr);
   return 1;
}
int kgRoundedRectangle_o(void * fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2;
   float x[450],y[450],v[450];
   float dl,xp,yp;
   double ang,dang;
   int i,j;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   count = 4*count2 ;
   for(i=0;i<count;i++) {
     x[i] +=xo;
     y[i] +=yo;
     
   }
   kgLineColor(fid,clr);
   kgMove2f(fid,x[0],y[0]);
   for(i=1;i<count;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgDraw2f(fid,x[0],y[0]);
   return 1;
}
int kgRoundedRectanglePressed(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
//   float bordrsize=3;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1;
   double dl1;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize;
   fac1 = dl1/(size-2*bordrsize);
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
//TCB
//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   vv = 1.3*vv;
   if(vv > 1 ) vv=1;
   vlow = vorg*0.8;
   vlow1 = vorg*0.40;
   vmid = vorg;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin= vv;
       vmax = vlow;
       vmin1 = vmid;
       vmax1 = vlow1;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, count,xd, yd)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         v[i]= vmin+ss*ss*dv;
         vd[i]= vmin1+ss*ss*dv1;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
     np =kgGetCirclePoints( xl,yl,fac, x, y, v,vv,vlow, count);
     np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vmid,vlow1, count);
   }
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
   }
   kgMixHsvColors(fid,200,128,h,s,vlow1,vv);
   kgSetShadeParams(fid,vv,vlow1,200, 128, 0);
   kgGouraudFill(fid,np2,xd,yd,v);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
//   kgPolyFill(fid,np,x,y,0,clr);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int kgRoundedRectangleRaised(void * fid,float xo,float yo,float xl,float yl,int clr,float fac,float bordrsize) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
//   float bordrsize=2.5;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1;
   double dl1;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
//   if(fac > 0.5) fac=0.5;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize;
   fac1 = dl1/(size-2*bordrsize);
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
//TCB

//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);

   if(vv>0.85) RGBtoHSV((float)r*0.9,(float)g,(float)b ,&h,&s,&vv);
   if(vv>0.9) RGBtoHSV((float)190.,(float)190.,(float)160. ,&h,&s,&vv);
//   if(vv>0.9 ) vv=0.9;
   horg = h;
   sorg =s;
   vorg = vv;
//TCBTCB
   vv = vorg*(1.1+0.5*acos(vorg)/1.57);
//   vv = vorg*(1.10+acos(vorg)/1.57);
//   vv = vorg*(1.05+2*atan(1.-vorg)/1.57);
   if(vv > 1.0)  {
#if 1
      if(sorg < 0.1 ) {
//       printf("%f %f %f\n",h,s,vv);
//       r = 0.93*r;
//       g = 0.97*g;
         r = 0.93*r;
         g = 0.97*g;
       RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
      }
//      else s = 0.8*sorg;
      else s = 0.9*sorg;
#else
   RGBtoHSV((float)255.0,(float)255.0,(float)255.0,&h,&s,&vv);
#endif
      vv =1.0;
   }
   vmid = vv*0.9;
   vmid1 = vv*0.8;

   vlow = vorg*0.95;
   vlow = vlow*vlow;
   vlow1 = vlow*0.75;
#if 1
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin= vlow1;
       vmax = vmid;
       vmin1 = vlow;
       vmax1 = vv;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, count,xd, yd)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         v[i]= vmin+ss*ss*dv;
         vd[i]= vmin1+ss*ss*dv1;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, x, y, v,vlow1,vmid, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vlow,vv, count);
   }
#else
#if 1
   np =kgGetCirclePoints( xl,yl,fac, x, y, v,vlow1,vmid, count);
   np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vlow,vv, count);
#else
   np =kgGetCirclePoints( xl,yl,fac, x, y, v,vv,vlow, count);
   np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vmid,vlow1, count);
#endif
#endif
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
   }
   kgMixHsvColors(fid,200,128,h,s,vlow1,vv);
   kgSetShadeParams(fid,vv,vlow1,200, 128, 0);
   kgGouraudFill(fid,np2,xd,yd,v);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[3]= x[0];
   yd[3]= -y[0];
   vd[3]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[1]= x[np2-1];
   yd[1]=  y[np2-1];
   vd[1]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   kgGouraudFill(fid,np2,xd,yd,vd);
//   kgPolyFill(fid,np,x,y,0,clr);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   kgGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
int kgRoundedRectangleRaised_1(void * fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np,pos;
   float x[450],y[450],xx,yy;
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1;
   double ang,dang;
   int i,j,r,g,b;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
//TCB

//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
#if 1
   if(vv > 0.9)  { s = 0.5*sorg; }
//   if(s < 0.3 ) s=0.3;
#else
   s = 0.3*sorg;
   if(s < 0.2 ) s=0.2;
#endif
   vv =vorg +(1.-vorg)*0.6;
//   if(vv > 1.2*vorg) vv= 1.2*vorg;
//   if(vv > 2.5*vorg) vv= 2.5*vorg;
   if(vv > 1 ) vv=1;
   vmid = vorg +(1-vorg)*0.55;
   vmid1 = vorg +(1-vorg)*0.45;
//   if(vmid > 1.6*vorg) vmid= 1.6*vorg;
   if(vmid > 1 ) vmid=1;
   if(vmid1 > 1 ) vmid1=1;

   vlow = vorg*0.9;
   vlow1 = vorg*0.80;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 3;
       count2 = count+2;
       count1=  count+1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= yp;
     for(i=1;i<=count;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
   }
   for(i=0;i<=count;i++) {
     x[count1+i]= -x[count-i];
     y[count1+i] = y[count-i];
   }
   for(i=0;i<(2*count1);i++) {
     x[2*count1+i]= x[2*count1-1-i];
     y[2*count1+i] = -y[2*count1-1-i];
   }
   
   for(i=0;i<4*count1;i++) {
     x[i] +=xo;
     y[i] +=yo;
   }
   hcount = count1/4;
   if(vorg > 0.8)HSVtoRGB(&red,&green,&blue,h,s,vorg*0.6);
   else HSVtoRGB(&red,&green,&blue,h,s,0.4);
   HSVtoRGB(&red,&green,&blue,h,s,vorg*0.6);
   kgChangeColor(fid,245,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vlow1);
   kgChangeColor(fid,246,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vlow);
   kgChangeColor(fid,247,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid1);
   kgChangeColor(fid,248,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid);
   kgChangeColor(fid,249,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vv);
   kgChangeColor(fid,250,(int)red,(int)green,(int)blue);
   kgLineColor(fid,246);
   kgLineWidth(fid,4);
   kgMove2f(fid,x[0],y[0]);
#if 1
   for(i=1;i<hcount;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   pos=hcount;
   kgLineColor(fid,247);
   for(i=pos;i<pos+hcount;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   pos+=hcount;
   kgLineColor(fid,248);
   for(i=pos;i<pos+hcount;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   pos+=hcount;
   kgLineColor(fid,249);
   for(i=pos;i<count1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   pos=count1;
   kgLineWidth(fid,6);
   kgLineColor(fid,250);
   for(i=pos;i< (pos+count1+1);i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineWidth(fid,4);
   pos =(2*count1+1);
   kgLineColor(fid,249);
   for(i=pos;i<pos+hcount;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   pos +=hcount;
   kgLineColor(fid,248);
   for(i=pos;i<pos+hcount;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   pos +=hcount;
   kgLineColor(fid,247);
   for(i=pos;i<pos+hcount;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   pos +=hcount;
   kgLineColor(fid,246);
   for(i=pos;i< 3*count1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   pos = 3*count1;
   kgLineColor(fid,245);
   for(i=pos;i<4*count1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
#else
   for(i=1;i<4*count1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
#endif
   kgDraw2f(fid,x[0],y[0]);
   return 1;
}
int kgRoundedRectangleRaised_o(void * fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np;
   float x[450],y[450],xx,yy;
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1;
   double ang,dang;
   int i,j,r,g,b;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   hcount = count/2;
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
//TCB

//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   horg = h;
   sorg =s;
#if 1
   if(vv > 0.7)  { s = 0.3*sorg; }
   if(s < 0.2 ) s=0.2;
#else
   s = 0.3*sorg;
   if(s < 0.2 ) s=0.2;
#endif
   vorg = vv;
   vv =vorg +(1.-vorg)*0.95;
   vv =vorg +(1.-vorg)*0.65;
//   if(vv > 1.2*vorg) vv= 1.2*vorg;
   if(vv > 1.8*vorg) vv= 1.8*vorg;
   if(vv > 1 ) vv=1;
   vmid = vorg +(1-vorg)*0.55;
   vmid1 = vorg +(1-vorg)*0.5;
//   if(vmid > 1.6*vorg) vmid= 1.6*vorg;
   if(vmid > 1 ) vmid=1;
   if(vmid1 > 1 ) vmid1=1;

   vlow = vorg*0.8;
   vlow1 = vorg*0.5;
//   vv = 0.9;
//   if(vlow > 0.3 ) vlow=0.3;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   hcount = (count2-2)/2;
   count = 4*count2 ;
   
   for(i=0;i<count;i++) {
     x[i] +=xo;
     y[i] +=yo;
     
   }
   HSVtoRGB(&red,&green,&blue,h,s,vmid1);
   kgChangeColor(fid,246,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vlow1);
   kgChangeColor(fid,247,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vlow);
   kgChangeColor(fid,248,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid);
   kgChangeColor(fid,249,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vv);
   kgChangeColor(fid,250,(int)red,(int)green,(int)blue);
   if(vorg > 0.3)HSVtoRGB(&red,&green,&blue,h,s,vorg*0.3);
   else HSVtoRGB(&red,&green,&blue,h,s,0.0);
   kgChangeColor(fid,251,(int)red,(int)green,(int)blue);
   kgLineColor(fid,251);
   kgMove2f(fid,x[0],y[0]);
   kgDraw2f(fid,x[1],y[1]);
   kgLineColor(fid,247);
   kgMove2f(fid,x[1],y[1]);
   for(i=2;i<hcount+2;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   xx = x[i],yy= y[i];
   kgLineColor(fid,246);
//   kgLineColor(fid,250);
   for(i=hcount+2;i<count2-1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,250);
   for(i=count2-1;i<2*count2+2;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,250);
   for(i=2*count2+2;i<2*count2+2+hcount/2;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,246);
   for(i=2*count2+2+hcount/2;i<2*count2+2+hcount;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   np = 3*count2-1-(2*count2+2+hcount);
   np=np/2;
   kgLineColor(fid,247);
   for(i=2*count2+2+hcount;i<3*count2-1-np;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,251);
   for(i=3*count2-1-np;i<3*count2-1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,251);
   for(i=3*count2-1;i<count;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgDraw2f(fid,x[0],y[0]);
   return 1;
}
int kgRoundedRectangleLowered(void * fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount;
   float x[450],y[450];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow;
   double ang,dang;
   int i,j,r,g,b;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   hcount = count/2;
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   vorg = vv;
   vv =vorg +(1.-vorg)*0.75;
   if(vv > 1.5*vorg) vv= 1.5*vorg;
   vmid = vorg +(1-vorg)*0.3;
   if(vmid > 1.3*vorg) vmid= 1.3*vorg;
   vlow = vorg*0.6;
//   vv = 0.9;
//   if(vlow > 0.3 ) vlow=0.3;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   hcount = (count2-2)/2;
   count = 4*count2 ;
   
   for(i=0;i<count;i++) {
     x[i] +=xo;
     y[i] +=yo;
     
   }
   HSVtoRGB(&red,&green,&blue,h,s,vlow);
   kgChangeColor(fid,248,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid);
   kgChangeColor(fid,249,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vv);
   kgChangeColor(fid,250,(int)red,(int)green,(int)blue);
   if(vorg > 0.4)HSVtoRGB(&red,&green,&blue,h,s,vorg*0.4);
   else HSVtoRGB(&red,&green,&blue,h,s,0.0);
   kgChangeColor(fid,251,(int)red,(int)green,(int)blue);
   kgLineColor(fid,250);
   kgMove2f(fid,x[0],y[0]);
   kgDraw2f(fid,x[1],y[1]);
   kgLineColor(fid,249);
   for(i=2;i<hcount+2;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,248);
   for(i=hcount+2;i<count2-1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,251);
   for(i=count2-1;i<2*count2+2;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,248);
   for(i=2*count2+2;i<2*count2+2+hcount;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,249);
   for(i=2*count2+2+hcount;i<3*count2-1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,250);
   for(i=3*count2-1;i<count;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgDraw2f(fid,x[0],y[0]);
   return 1;
}
int kgRoundedRectanglePressed_o(void * fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount;
   float x[450],y[450];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow;
   double ang,dang;
   int i,j,r,g,b;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   hcount = count/2;
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   vorg = vv;
   vv =vorg +(1.-vorg)*0.75;
   if(vv > 1.5*vorg) vv= 1.5*vorg;
   vmid = vorg +(1-vorg)*0.3;
   if(vmid > 1.3*vorg) vmid= 1.3*vorg;
   vlow = vorg*0.6;
//   vv = 0.9;
//   if(vlow > 0.3 ) vlow=0.3;
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
//       printf("Getting kgGetEllipsePoints\n");
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       count2 = count -1;
       count1=  count2-1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= 0.0;
     x[1]=dl +xp;
     y[1]= yp;
     for(i=2;i<count2;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
     x[count1]= xp;
     y[count1]= dl+yp;
     x[count2]= 0.0;
     y[count2]= dl+yp;
   }
   for(i=1;i<=count2;i++) {
     x[count2+i]= -x[count2-i];
     y[count2+i] = y[count2-i];
   }
   for(i=1;i<=(count1+count2);i++) {
     x[2*count2+i]= x[2*count2-i];
     y[2*count2+i] = -y[2*count2-i];
   }
   hcount = (count2-2)/2;
   count = 4*count2 ;
   
   for(i=0;i<count;i++) {
     x[i] +=xo;
     y[i] +=yo;
     
   }
   HSVtoRGB(&red,&green,&blue,h,s,vlow);
   kgChangeColor(fid,248,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid);
   kgChangeColor(fid,249,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vv);
   kgChangeColor(fid,250,(int)red,(int)green,(int)blue);
   if(vorg > 0.4)HSVtoRGB(&red,&green,&blue,h,s,vorg*0.4);
   else HSVtoRGB(&red,&green,&blue,h,s,0.0);
   kgChangeColor(fid,251,(int)red,(int)green,(int)blue);
   kgLineColor(fid,250);
   kgMove2f(fid,x[0],y[0]);
   kgDraw2f(fid,x[1],y[1]);
   kgLineColor(fid,249);
   for(i=2;i<hcount+2;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,248);
   for(i=hcount+2;i<count2-1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,251);
   for(i=count2-1;i<2*count2+2;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,248);
   for(i=2*count2+2;i<2*count2+2+hcount;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,249);
   for(i=2*count2+2+hcount;i<3*count2-1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineColor(fid,250);
   for(i=3*count2-1;i<count;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgDraw2f(fid,x[0],y[0]);
   return 1;
}
int kgRoundedRectangleLowered_o(void * fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount;
   float x[450],y[450];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow;
   double ang,dang;
   int i,j,r,g,b;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   if(dl > fac*xl) dl=fac*xl;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   hcount = count/2;
//   kgGetDefaultRGB(clr,&r,&g,&b);
//   kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   if(clr >=0 ) kgGetRGB((DIG *)fid,clr,&r,&g,&b);
   else {
         clr = -clr;
         b = (clr%1000);
         g = (clr/1000)%1000;
         r = (clr/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         clr = 127;
         kgChangeColor(fid,clr,r,g,b);
   }
   r=g=b=255;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   vorg = vv;
   vv =0.9;
   vmid = 0.9;
   vlow = 0.0;
//   vv = 0.9;
//   if(vlow > 0.3 ) vlow=0.3;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   x[0]=dl +xp;
   y[0]= yp;
   for(i=1;i<count1;i++) {
     ang +=dang;
     x[i]= dl*cos(ang)+xp;
     y[i]= dl*sin(ang)+yp;
   }
   x[count]= xp;
   y[count]= dl+yp;
   for(i=1;i<count2;i++) {
     x[count+i]= -x[count1-i];
     y[count+i] = y[count1-i];
   }
   for(i=1;i<(count1+count2);i++) {
     x[count+count1+i]= x[2*count1-i];
     y[count+count1+i] = -y[2*count1-i];
   }
   for(i=0;i<4*count1;i++) {
     x[i] +=xo;
     y[i] +=yo;
   }
   HSVtoRGB(&red,&green,&blue,h,s,vlow);
   kgChangeColor(fid,248,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid);
   kgChangeColor(fid,249,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vv);
   kgChangeColor(fid,250,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,0.0);
   kgChangeColor(fid,251,(int)red,(int)green,(int)blue);
   kgLineWidth(fid,3);
   kgLineColor(fid,249);
   kgMove2f(fid,x[0],y[0]);
   for(i=1;i<hcount;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineWidth(fid,2);
   kgLineColor(fid,248);
   for(i=hcount;i<count1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineWidth(fid,4);
   kgLineColor(fid,251);
   for(i=count1;i<2*count1+1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineWidth(fid,3);
   kgLineColor(fid,248);
   for(i=2*count1+1;i<2*count1+1+hcount+1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineWidth(fid,2);
   kgLineColor(fid,249);
   for(i=2*count1+1+hcount+1;i<3*count1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgLineWidth(fid,4);
   kgLineColor(fid,250);
   for(i=3*count1;i<4*count1;i++) {
     kgDraw2f(fid,x[i],y[i]);
   }
   kgDraw2f(fid,x[0],y[0]);
   return 1;
}
void * kgStringToImage_old(char *Str,void *image,int xsize,int ysize,int font,int txtcolor,int justification,int width,int bkgr){
/* -1 left justification ; 0 center ;1 right */
   void * fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw;
#if 1
   {
      fid = kgInitImage((int)(xsize),ysize,RESIZE);
      kgUserFrame(fid,0.,0.,(float)xsize,(float)ysize);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(th> 0.6*ysize) th = 0.6*ysize;
      kgTextFont(fid,font);
      kgTextSize(fid,th,tw,GAP*tw);
      length=kgStringLength(fid,Str);
      if(length>xsize*0.95) {
        fac = xsize*0.95/length;
        kgTextSize(fid,th,tw*fac,GAP*tw);
        length=kgStringLength(fid,Str);
      }
      if(bkgr>=0) kgBoxFill(fid,0.,0.,(float)xsize,(float)ysize,bkgr,0);
      kgTextColor(fid,txtcolor);
      if(image != NULL) kgDrawImage(fid,image,3.0,3.0,xsize-3.,ysize-3.);
      switch(justification) {
        case -1:
         kgMove2f(fid,0.0,0.25*ysize);
         break;
        case  0:
         kgMove2f(fid,(xsize-length)*.5,0.25*ysize);
         break;
        case  1:
         kgMove2f(fid,(xsize-length-tw*0.5),0.25*ysize);
         break;
        default:
         kgMove2f(fid,(xsize-length)*.5,0.25*ysize);
         break;
      }
      kgWriteText(fid,Str);
//      img = kgGetResizedImage(fid);
      img = kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
#endif
   return img;
}
void * kgStringToImage(char *Str,void *image,int xsize,int ysize,int font,int txtcolor,int justification,int width,int bkgr){
/* -1 left justification ; 0 center ;1 right */
   void * fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw;
#if 1
   {
      fid = kgInitImage((int)(xsize),ysize,RESIZE);
      kgUserFrame(fid,0.,0.,(float)xsize,(float)ysize);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(th> 0.6*ysize) th = 0.6*ysize;
      kgTextFont(fid,font);
      kgTextSize(fid,th,tw,GAP*tw);
      if(bkgr>=0) kgBoxFill(fid,0.,0.,(float)xsize,(float)ysize,bkgr,0);
      kgTextColor(fid,txtcolor);
      if(image != NULL) kgDrawImage(fid,image,3.0,3.0,xsize-3.,ysize-3.);
      _uiWriteStrings(fid,0.0,0.0,xsize,ysize,Str,tw,th,justification);
//      img = kgGetResizedImage(fid);
      img = kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
#endif
   return img;
}
void * kgFilledStringToImagePressed(char *Str,void *image,int xsize,int ysize,int font,int fillcolor,int highli,int color,int justification,int width,float rfac,int state){
/* -1 left justification ; 0 center ;1 right */
   int r,g,b,aspc;
   void *fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,h,s,v,red,blue,green,size;
   size = xsize;
   aspc=1;
   if(ysize > size) {size=ysize;aspc=0;}
   {
      fid = kgInitImage(xsize,ysize,RESIZE);
      kgUserFrame(fid,-3.,-3.,(float)xsize+3.,(float)ysize+3.);
      kgTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
      kgLineWidth(fid,4);
//      kgGetDefaultRGB(fillcolor,&r,&g,&b);
//      kgGetRGB((DIG *)fid,fillcolor,&r,&g,&b);
      if(fillcolor >=0 ) kgGetRGB((DIG *)fid,fillcolor,&r,&g,&b);
      else {
         fillcolor = -fillcolor;
         b = (fillcolor%1000);
         g = (fillcolor/1000)%1000;
         r = (fillcolor/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         fillcolor = 127;
         kgChangeColor(fid,fillcolor,r,g,b);
      }
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      HSVtoRGB(&red,&green,&blue,h,s,0.95*v);
      kgChangeColor(fid,252,(int)red,(int)green,(int)blue);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+3,(float)ysize+3,0,0,rfac);
//TCB
      kgLineWidth(fid,7);
      kgLineWidth(fid,5);
      kgRoundedRectangleLowered(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+3,(float)ysize+3,fillcolor,rfac);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,252,rfac);
      kgLineColor(fid,color);
      if(image != NULL) kgDrawImage(fid,image,3.0,3.0,xsize-3.,ysize-3.);
      if((Str != NULL) &&(Str[0]!='\0')) {
        kgTextColor(fid,color);
        if(aspc) _uiWriteStrings(fid,0.0,0.0,xsize,ysize,Str,tw,th,justification);
        else {
        kgTextSize(fid,th,tw,GAP*tw);
        length = kgStringLength(fid,Str);
        if(length>size*0.9) {
          fac = size*0.9/length;
          kgTextSize(fid,th,tw*fac,GAP*tw);
          length = kgStringLength(fid,Str);
        }
        if(!aspc) kgTextAngle(fid,90.);
        switch(justification) {
          case -1:
           if(aspc) kgMove2f(fid,0.0+1.0,0.5*ysize-th*.5-1.0);
           else     kgMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) kgMove2f(fid,(xsize-length)*.5+1.0,0.5*ysize-th*.5-1.0);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) kgMove2f(fid,(xsize-length-tw*0.5)+1.0,0.5*ysize-th*.5-1.0);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) kgMove2f(fid,(xsize-length)*.5+1.0,0.5*ysize-th*0.5-1.0);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        kgWriteText(fid,Str);
        }
      }
//      img=kgGetResizedImage(fid);
      img=kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return img;
}
void * kgFilledStringToImage1(char *Str,void *image,int xsize,int ysize,int font,int fillcolor,int highli,int color,int bodr,int justification,int width,float rfac,int state,float depthfac){
/* -1 left justification ; 0 center ;1 right */
   int aspc;
   void *fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green;
   int r,g,b;
   aspc=1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   if(depthfac> xsize*0.4 ) depthfac=xsize*0.4;
   if(depthfac> ysize*0.4 ) depthfac=ysize*0.4;
   fid = kgInitImage(xsize,ysize,RESIZE);
   {
      kgUserFrame(fid,-3.,-3.,(float)xsize+3.,(float)ysize+3.);
      kgTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
      kgLineWidth(fid,4);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,rfac);
      kgLineWidth(fid,4);
      if(state==-1) {
        kgRoundedRectangleRaised(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,depthfac);
        kgRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+2,(float)ysize+2,bodr,rfac,depthfac*0.7);
      }
      else  {
        kgRoundedRectangleRaised(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,depthfac);
//        kgRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+2,(float)ysize+2,bodr,rfac,depthfac*0.3);
      }
#if 1
      kgLineWidth(fid,1);
      if(state != -1) {
//        kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+2,(float)ysize+2,0,rfac,0.5);
        if(highli){
           kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+3,(float)ysize+3,0,rfac,1.0);
           kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+4,(float)ysize+4,0,rfac,1.0);
        }
      }
#endif
      if(image != NULL) kgDrawImage(fid,image,3.0,3.0,xsize-3.,ysize-3.);
      if((Str != NULL) &&(Str[0]!='\0')) {
        kgTextColor(fid,color);
        if(aspc) _uiWriteStrings(fid,0.0,0.0,xsize,ysize,Str,tw,th,justification);
        else {
        kgTextSize(fid,th,tw,GAP*tw);
        length = kgStringLength(fid,Str);
        if(length>size*0.9) {
          fac = size*0.9/length;
          kgTextSize(fid,th,tw*fac,GAP*tw);
           length = kgStringLength(fid,Str);
        }
        if(!aspc) kgTextAngle(fid,90.);
        switch(justification) {
          case -1:
           if(aspc) kgMove2f(fid,0.0,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) kgMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        kgWriteText(fid,Str);
        }
      }
//      img=kgGetResizedImage(fid);
      img=kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return img;
}
void * kgFilledStringToImage2(char *Str,void *image,int xsize,int ysize,int font,int fillcolor,int highli,int color,int bodr,int justification,int width,float rfac,int state){
/* -1 left justification ; 0 center ;1 right */
   int aspc;
   void *fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green;
   int r,g,b;
   aspc=1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   fid = kgInitImage(xsize,ysize,RESIZE);
   if(fid != NULL ) {
      kgUserFrame(fid,-3.,-3.,(float)xsize+3.,(float)ysize+3.);
      kgTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
      kgLineWidth(fid,1);
      if(highli){
        kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+5.0,(float)ysize+5.0,0,rfac,0.5);
      }
      kgLineWidth(fid,4);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,rfac);
      kgLineWidth(fid,4);
      if(state==-1)
      kgRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+4,(float)ysize+4,bodr,rfac,3.0);
      else 
      kgRoundedRectangleRaised(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,3.0);
      if(image != NULL) kgDrawImage(fid,image,3.0,3.0,xsize-3.,ysize-3.);
      if((Str != NULL) &&(Str[0]!='\0')) {
        kgTextColor(fid,color);
        if(aspc) _uiWriteStrings(fid,0.0,0.0,xsize,ysize,Str,tw,th,justification);
        else {
        kgTextSize(fid,th,tw,GAP*tw);
        length = kgStringLength(fid,Str);
        if(length>size*0.9) {
          fac = size*0.9/length;
          kgTextSize(fid,th,tw*fac,GAP*tw);
          length = kgStringLength(fid,Str);
        }
        if(!aspc) kgTextAngle(fid,90.);
        switch(justification) {
          case -1:
           if(aspc) kgMove2f(fid,0.0,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) kgMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        kgWriteText(fid,Str);
        }
      }
//      img=kgGetResizedImage(fid);
      img=kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return img;
}
void * kgFilledStringToImage3(char *Str,void *image,int xsize,int ysize,int font,int fillcolor,int highli,int color,int bodr,int justification,int width,float rfac,int state,float depthfac){
/* -1 left justification ; 0 center ;1 right */
   int aspc;
   void *fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green;
   int r,g,b,darkclr=177;
   aspc=1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   if(depthfac> xsize*0.4 ) depthfac=xsize*0.4;
   if(depthfac> ysize*0.4 ) depthfac=ysize*0.4;
   fid = kgInitImage(xsize,ysize,RESIZE);
   if(fid != NULL ) {
      kgUserFrame(fid,-3.,-3.,(float)xsize+3.,(float)ysize+3.);
      kgTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
      kgLineWidth(fid,1);
      if(highli){
        kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+5.0,(float)ysize+5.0,0,rfac,0.5);
      }
      kgLineWidth(fid,4);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,rfac);
      kgLineWidth(fid,4);
      if(state==-1) {
#if 0
        kgRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,
                         (float)xsize+4,(float)ysize+4,bodr,rfac,depthfac);
#else
//      kgChangeColor(fid,darkclr,80,80,80);
//      kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,darkclr,rfac,0.25);
      kgRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,
                  (float)xsize+2,(float)ysize+2,bodr,rfac,depthfac*0.3);
      kgLineWidth(fid,4);
      kgRoundedRectangleRaised(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize-4,(float)ysize-4,fillcolor,rfac,depthfac*0.3);
      if(image != NULL) kgDrawImage(fid,image,3.0-2,3.0-2,xsize-3.,ysize-3.);
//        kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,
//                         (float)xsize-2,(float)ysize-2,0,rfac,0.25);
#endif
      }
      else 
      kgRoundedRectangleRaised(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,depthfac);
      if(image != NULL) kgDrawImage(fid,image,3.0,3.0,xsize-3.,ysize-3.);
      if((Str != NULL) &&(Str[0]!='\0')) {
        kgTextColor(fid,color);
        if(aspc) _uiWriteStrings(fid,0.0,0.0,xsize,ysize,Str,tw,th,justification);
        else {
        kgTextSize(fid,th,tw,GAP*tw);
        length = kgStringLength(fid,Str);
        if(length>size*0.9) {
          fac = size*0.9/length;
          kgTextSize(fid,th,tw*fac,GAP*tw);
          length = kgStringLength(fid,Str);
        }
        if(!aspc) kgTextAngle(fid,90.);
        switch(justification) {
          case -1:
           if(aspc) kgMove2f(fid,0.0,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) kgMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        kgWriteText(fid,Str);
        }
      }
//      img=kgGetResizedImage(fid);
      img=kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return img;
}
void * kgShadedStringToImage(char *Str,void *image,int xsize,int ysize,int font,int fillcolor,int highli,int color,int justification,int width,float rfac,int state,int type){
/* -1 left justification ; 0 center ;1 right */
   int aspc=1;
   void *fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,vmin,vmax,size,lnwidth=2,xm,ym;
   float h,s,v,red,blue,green;
   int r,g,b;
   fid = kgInitImage(xsize,ysize,RESIZE);
   aspc =1;
   size = xsize;
//   kgGetDefaultRGB(fillcolor,&r,&g,&b);
//   kgGetRGB((DIG *)fid,fillcolor,&r,&g,&b);
      if(fillcolor >=0 ) kgGetRGB((DIG *)fid,fillcolor,&r,&g,&b);
      else {
         fillcolor = -fillcolor;
         b = (fillcolor%1000);
         g = (fillcolor/1000)%1000;
         r = (fillcolor/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         fillcolor = 127;
         kgChangeColor(fid,fillcolor,r,g,b);
      }
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
   if(v> 1) v=1.0;
   if(state==-1) { v=0.6*v; }
   if(highli) {v *=1.2; if(v>1.) v=1.0;}
   if(ysize > size) {size=ysize;aspc=0;}
   if(fid != NULL ) {
      kgUserFrame(fid,-3.,-3.,(float)xsize+3.,(float)ysize+3.);
      kgTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
      kgLineColor(fid,color);
      kgLineWidth(fid,2);
      lnwidth=2;
      vmin = 0.85*v;
      vmax =1.3*v;
//TCBTCB
      if(vmax >1.0) vmax=1.0;
      HSVtoRGB(&red,&green,&blue,h,s,vmax);
      kgChangeColor(fid,150,red,green,blue);
      vmax =1.1*v;
      if(vmax >1.0) vmax=1.0;
      HSVtoRGB(&red,&green,&blue,h,s,vmax);
      kgChangeColor(fid,151,red,green,blue);
      HSVtoRGB(&red,&green,&blue,h,s,v*0.5);
      kgChangeColor(fid,149,red,green,blue);
      HSVtoRGB(&red,&green,&blue,h,s,v*0.8);
      kgChangeColor(fid,148,red,green,blue);
      vmax =1.5*v;
      if(vmax >1.0) vmax=1.0;
      vmin= vmax*0.75;
      HSVtoRGB(&red,&green,&blue,h,s,vmin);
      kgChangeColor(fid,152,red,green,blue);
      if(state==-1) {
        highli=0; kgLineWidth(fid,5);
        lnwidth=5;
      }
      else {
        if(highli) kgLineWidth(fid,5);
        lnwidth=5;
      }
//TCBTCB
      if(type > 0) {
//      kgRoundedRectangleFill(fid,(float)xsize*0.5-1.0,(float)ysize*0.5+0.5,(float)xsize,(float)ysize,0,148,rfac);
      kgRoundedRectangleFill(fid,(float)xsize*0.5+1.0,(float)ysize*0.5-1.0,(float)xsize,(float)ysize,0,0,rfac);
//      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,151,rfac);
//      kgRoundedRectangleFill(fid,(float)xsize*0.5+1,(float)ysize*0.5+1,(float)xsize-1,(float)ysize-1,0,152,rfac);
//      xsize -=2.0;
//      ysize -=2.0;
      }
      else {
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+1.0,(float)ysize+1.0,0,149,rfac);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,150,rfac);
      xsize-=2.0;
      ysize-=2.0;
      }
      switch(type) {
        case 0:
        case 2:
         kgRoundedRectangleShade0(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,vmin,vmax);
        break;
        case 1:
         kgRoundedRectangleShade1(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,vmin,vmax*0.95);
        break;
        case 3:
         kgRoundedRectangleFill(fid,(float)xsize*0.5-1.0,(float)ysize*0.5+1.0,(float)xsize,(float)ysize,0,15,rfac);
         kgRoundedRectangleShade(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,vmin*1.2,vmax);
        break;
        case 4:
         kgRoundedRectangleFill(fid,(float)xsize*0.5-1.0,(float)ysize*0.5+1.0,(float)xsize,(float)ysize,0,15,rfac);
//         kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,rfac);
         kgRoundedRectangleShade0(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,v,vmax*0.95);
        break;
        case 5:
         kgRoundedRectangleFill(fid,(float)xsize*0.5-1.0,(float)ysize*0.5+1.0,(float)xsize,(float)ysize,0,15,rfac);
         kgRoundedRectangleShade(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,rfac,vmin,vmax*0.95);
        break;
        default:
         kgRoundedRectangleFill(fid,(float)xsize*0.5-1.0,(float)ysize*0.5+0.5,(float)xsize,(float)ysize,0,15,rfac);
         kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,rfac);
        break;
           
      }
//TCB
#if 0
      if(highli)kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+1,(float)ysize+1,color,rfac,1.0);
      else kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+1,(float)ysize+1,color,rfac,0.5);
#endif
      if(!aspc) {
//          printf("Text Angle= 90.\n");
          kgTextAngle(fid,90.);
      }
      xm = xsize*0.5;
      ym = ysize*0.5;
      if(image != NULL) kgDrawImage(fid,image,3.0,3.0,xsize-3.,ysize-3.);
      if((Str != NULL) &&(Str[0]!='\0')) {
        kgTextColor(fid,color);
        if(aspc) _uiWriteStrings(fid,0.0,0.0,xsize,ysize,Str,tw,th,justification);
        else {
        kgTextSize(fid,th,tw,GAP*tw);
        length = kgStringLength(fid,Str);
        if(length>size*0.9) {
          fac = size*0.9/length;
          if(!aspc) th = th*fac;
          tw = tw*fac;
          kgTextSize(fid,th,tw,GAP*tw);
          length = kgStringLength(fid,Str);
        }
        switch(justification) {
          case -1:
           if(aspc) kgMove2f(fid,0.0,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
//           else     kgMove2f(fid,xm-th*0.5,ym-length*0.5+th*.5);
           else     kgMove2f(fid,0.5*xsize+tw*0.4,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) kgMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        kgWriteText(fid,Str);
        }
      }
      img=kgGetResizedImage(fid);
//      img=kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return img;
}
void * kgBoxedStringToImage(char *Str,void *image,int xsize,int ysize,int font,int border,int highli,int color,int justification,int width,float rfac,int state){
/* -1 left justification ; 0 center ;1 right */
   int aspc=1;
   void *fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green,vorg;
   int r,g,b;
   fid = kgInitImage(xsize,ysize,RESIZE);
   aspc =1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   if(fid != NULL ) {
      kgUserFrame(fid,-0.5,-0.5,(float)xsize+0.5,(float)ysize+0.5);
      kgTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
      kgLineWidth(fid,11);
//      kgGetDefaultRGB(border,&r,&g,&b);
      if(border >=0 ) kgGetRGB((DIG *)fid,border,&r,&g,&b);
      else {
         border = -border;
         b = (border%1000);
         g = (border/1000)%1000;
         r = (border/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         border = 127;
         kgChangeColor(fid,border,r,g,b);
      }
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      vorg =v;
      if(state==-1) {
        v = 0.5*v;
        kgLineWidth(fid,7);
      }
      else {
        if(highli) {
           v *=1.15; 
           if(v > 1.0) {
             v=1.0;
             h *=0.8;
             s*=0.8;
           }
        }
      }
      vorg =v;
      HSVtoRGB(&red,&green,&blue,h,s,v);
      kgChangeColor(fid,252,(int)red,(int)green,(int)blue);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,border,rfac);
      kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,252,rfac,1.0);
#if 0
      HSVtoRGB(&red,&green,&blue,h,s,vorg);
      kgChangeColor(fid,252,(int)red,(int)green,(int)blue);
      kgLineColor(fid,252);
      kgLineWidth(fid,3);
      kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize-1,(float)ysize-1,252,rfac,1.0);
#endif
      if(image != NULL) kgDrawImage(fid,image,3.0,3.0,xsize-3.,ysize-3.);
      if((Str != NULL) &&(Str[0]!='\0')) {
        kgTextColor(fid,color);
        if(aspc) _uiWriteStrings(fid,0.0,0.0,xsize,ysize,Str,tw,th,justification);
        else {
        kgTextSize(fid,th,tw,GAP*tw);
        length = kgStringLength(fid,Str);
        if(length>size*0.9) {
          fac = size*0.9/length;
          kgTextSize(fid,th,tw*fac,GAP*tw);
          length = kgStringLength(fid,Str);
        }
        if(!aspc) kgTextAngle(fid,90.);
        switch(justification) {
          case -1:
           if(aspc) kgMove2f(fid,0.0,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) kgMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        kgWriteText(fid,Str);
        }
      }
//      img=kgGetResizedImage(fid);
      img=kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return img;
}
void  kgStringToImagefile(char *Imgfile,char *Str,int xsize,int ysize,int font,int txtcolor,int justification,int width){
/* -1 left justification ; 0 center ;1 right */
   void * fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw;
   fid = kgInitImage(xsize,ysize,RESIZE);
   if(fid != NULL ) {
      kgUserFrame(fid,0.,0.,(float)xsize,(float)ysize);
      kgTextFont(fid,font);
      th = (float)ysize*.5;
      tw = ((float)width);
      th = HFAC*tw;
      if(th> 0.6*ysize) th = 0.6*ysize;
      kgTextSize(fid,th,tw,GAP*tw);
      length = kgStringLength(fid,Str);
      if(length>xsize*0.9) {
        fac = xsize*0.9/length;
        kgTextSize(fid,th,tw*fac,GAP*tw);
        length = kgStringLength(fid,Str);
      }
      kgTextColor(fid,txtcolor);
      switch(justification) {
        case -1:
         kgMove2f(fid,0.0,0.25*ysize);
         break;
        case  0:
         kgMove2f(fid,(xsize-length)*.5,0.25*ysize);
         break;
        case  1:
         kgMove2f(fid,(xsize-length-tw*0.5),0.25*ysize);
         break;
        default:
         kgMove2f(fid,(xsize-length)*.5,0.25*ysize);
         break;
      }
      kgWriteText(fid,Str);
//      img=kgGetResizedImage(fid);
      img=kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return ;
}
void * kgSplashStringToImage_o(char *Str,int xsize,int ysize,int font,int fillcolor,int color,int bodrcolor,int justification,int width,float rfac){
/* -1 left justification ; 0 center ;1 right */
   int aspc;
   void *fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green;
   int r,g,b;
   aspc=1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   fid = kgInitImage(xsize,ysize,RESIZE);
   if(fid != NULL ) {
      kgUserFrame(fid,-0.,-0.,(float)xsize+0.,(float)ysize+0.);
      kgTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
#if 0
      kgLineWidth(fid,3);
      if(highli) kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+4.0,(float)ysize+4.0,0,rfac);
#else
//      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,0,rfac);
//      kgLineWidth(fid,2);
//      kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,rfac);
#endif
//      kgLineWidth(fid,4);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize-3,(float)ysize-3,0,fillcolor,rfac);
//TCB
      kgLineWidth(fid,9);
      kgRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize-3,(float)ysize-3,bodrcolor,rfac,5.0);
      kgLineColor(fid,color);
      if((Str != NULL) &&(Str[0]!='\0')) {
        kgTextSize(fid,th,tw,GAP*tw);
//        length = kgStringLength(Str,(int) tw);
//        uistrlngth(fid,Str,&length);
        length = kgStringLength(fid,Str);
        if(length>size*0.9) {
          fac = size*0.9/length;
          kgTextSize(fid,th,tw*fac,GAP*tw);
//          length = kgStringLength(Str,(int)(tw*fac));
//          uistrlngth(fid,Str,&length);
          length = kgStringLength(fid,Str);
        }
        if(!aspc) kgTextAngle(fid,90.);
        kgTextColor(fid,color);
        switch(justification) {
          case -1:
           if(aspc) kgMove2f(fid,0.0,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) kgMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        kgWriteText(fid,Str);
      }
      img=kgGetResizedImage(fid);
//      img=kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return img;
}
void * kgSplashStringToImage(char *Str,int xsize,int ysize,int font,int fillcolor,int color,int bodrcolor,int justification,int width,float rfac){
/* -1 left justification ; 0 center ;1 right */
   int aspc;
   void *fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green,vmax,vmin;
   int r,g,b;
   aspc=1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   fid = kgInitImage(xsize,ysize,RESIZE);
   if(fid != NULL ) {
//      kgUserFrame(fid,-0.,-0.,(float)xsize+0.,(float)ysize+0.);
      kgUserFrame(fid,-1.0,-1.0,(float)xsize+1.0,(float)ysize+1.0);
//      kgGetRGB((DIG *)fid,fillcolor,&r,&g,&b);
      if(color >=0 ) kgGetRGB((DIG *)fid,color,&r,&g,&b);
      else {
         color = -color;
         b = (color%1000);
         g = (color/1000)%1000;
         r = (color/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         color = 128;
         kgChangeColor(fid,color,r,g,b);
      }
      if(bodrcolor >=0 ) kgGetRGB((DIG *)fid,bodrcolor,&r,&g,&b);
      else {
         bodrcolor = -bodrcolor;
         b = (bodrcolor%1000);
         g = (bodrcolor/1000)%1000;
         r = (bodrcolor/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         bodrcolor = 129;
         kgChangeColor(fid,bodrcolor,r,g,b);
      }
      if(fillcolor >=0 ) kgGetRGB((DIG *)fid,fillcolor,&r,&g,&b);
      else {
         fillcolor = -fillcolor;
         b = (fillcolor%1000);
         g = (fillcolor/1000)%1000;
         r = (fillcolor/1000000)%1000;
         b = b%256;g=g%256;r=r%256;
         fillcolor = 127;
         kgChangeColor(fid,fillcolor,r,g,b);
      }
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      vmax = v*1.8;
      if(vmax > 1.0) vmax=1.0;
      vmin = v*0.4;
      HSVtoRGB(&red,&green,&blue,h,s,vmax);
      kgChangeColor(fid,251,(int)red,(int)green,(int)blue);
      HSVtoRGB(&red,&green,&blue,h,s,vmin);
      kgChangeColor(fid,252,(int)red,(int)green,(int)blue);
      kgTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
//TCBTCB
      th = 1.30*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
//      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+6,(float)ysize+6,0,bodrcolor,rfac);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+1.,(float)ysize+1.,0,bodrcolor,rfac);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize-4,(float)ysize-4,0,fillcolor,rfac);
      kgRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,bodrcolor,rfac,3.0);
//      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize-6,(float)ysize-6,0,fillcolor,rfac);
      kgLineColor(fid,color);
      if((Str != NULL) &&(Str[0]!='\0')) {
        kgTextSize(fid,th,tw,GAP*tw);
//        length = kgStringLength(Str,(int)tw);
//        uistrlngth(fid,Str,&length);
        length = kgStringLength(fid,Str);
        if(length>size*0.9) {
          fac = size*0.9/length;
          kgTextSize(fid,th,tw*fac,GAP*tw);
//          length = kgStringLength(Str,(int)(tw*fac));
//          uistrlngth(fid,Str,&length);
          length = kgStringLength(fid,Str);
        }
        if(!aspc) kgTextAngle(fid,90.);
        kgTextColor(fid,color);
        switch(justification) {
          case -1:
           if(aspc) kgMove2f(fid,0.0,0.5*ysize-th*.35);
           else     kgMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.35);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) kgMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.35);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.35);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        kgWriteText(fid,Str);
      }
      img=kgGetResizedImage(fid);
//      img=kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return img;
}
void * kgSplashStringToImage_n(char *Str,int xsize,int ysize,int font,int fillcolor,int color,int bodrcolor,int justification,int width,float rfac){
/* -1 left justification ; 0 center ;1 right */
   int aspc;
   void *fid=NULL;
   void *img=NULL;
   char *tmpdir,flname[200];
   float length=0.0,fac,th,tw,size;
   float h,s,v,red,blue,green,vmax,vmin,x[6],y[6];
   int r,g,b;
   aspc=1;
   size = xsize;
   if(ysize > size) {size=ysize;aspc=0;}
   fid = kgInitImage(xsize,ysize,RESIZE);
   if(fid != NULL ) {
      kgUserFrame(fid,-1.,-1.,(float)xsize+1.,(float)ysize+1.);
//      kgGetDefaultRGB(fillcolor,&r,&g,&b);
//      kgGetRGB((DIG *)fid,fillcolor,&r,&g,&b);
      if(fillcolor >=0 ) kgGetRGB((DIG *)fid,fillcolor,&r,&g,&b);
      else {
         fillcolor = -fillcolor;
         b = (fillcolor%1000);
         g = (fillcolor/1000)%1000;
         r = (fillcolor/1000000)%1000;
         fillcolor= 127;
         b = b%256;g=g%256;r=r%256;
         kgChangeColor(fid,fillcolor,r,g,b);
      }
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      vmax = v*1.5;
      if(vmax > 1.0) vmax=1.0;
      vmin = v*0.4;
      HSVtoRGB(&red,&green,&blue,h,s,vmax);
      kgChangeColor(fid,251,(int)red,(int)green,(int)blue);
      HSVtoRGB(&red,&green,&blue,h,s,vmin);
      kgChangeColor(fid,252,(int)red,(int)green,(int)blue);
      vmax = v*1.2;
      if(vmax > 1.0) vmax=1.0;
      vmin = v*0.1;
      HSVtoRGB(&red,&green,&blue,h,s,vmax);
      kgChangeColor(fid,253,(int)red,(int)green,(int)blue);
      HSVtoRGB(&red,&green,&blue,h,s,vmin);
      kgChangeColor(fid,254,(int)red,(int)green,(int)blue);
      kgTextFont(fid,font);
      th = (float)ysize*.5;
      tw = (float)width;
      th = HFAC*tw;
      if(aspc) {if(th> 0.6*ysize) th = 0.6*ysize;}
      else     { if(th>0.6*xsize )th= 0.6*xsize;}
#if 0
      kgLineWidth(fid,3);
      if(highli) kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+4.0,(float)ysize+4.0,0,rfac);
#else
//      kgLineWidth(fid,2);
//      kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize+2.0,(float)ysize+2.0,0,0.0);
#endif
//      kgLineWidth(fid,4);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,0.0);
      x[1]=-2.0; x[2]=xsize+2,x[5]=xsize+2,x[4]=xsize-2;x[3]=xsize-2;x[0]=+2;
      y[1]=-2.0; y[2]=-2.0,y[5]=ysize+2,y[4]=ysize-2;y[3]=2;y[0]=2;
      kgPolyFill(fid,4,x,y,0,251);
      x[1]=-2.0; x[2]=xsize+2,x[3]=xsize+2,x[4]=xsize-2;x[5]=xsize-2;x[0]=+2;
      y[1]=-2.0; y[2]=-2.0,y[3]=ysize+2,y[4]=ysize-2;y[5]=2;y[0]=2;
      kgPolyFill(fid,4,x+2,y+2,0,253);
      x[1]=-2.0; x[2]=-2,x[5]=xsize+2,x[4]=xsize-2;x[3]=+2;x[0]=+2;
      y[1]=-2.0; y[2]=ysize+2.0,y[5]=ysize+2,y[4]=ysize-2;y[3]=ysize-2;y[0]=2;
      kgPolyFill(fid,4,x,y,0,254);
      x[1]=-2.0; x[2]=-2,x[3]=xsize+2,x[4]=xsize-2;x[5]=+2;x[0]=+2;
      y[1]=-2.0; y[2]=ysize+2.0,y[3]=ysize+2,y[4]=ysize-2;y[5]=ysize-2;y[0]=2;
      kgPolyFill(fid,4,x+2,y+2,0,252);
//TCB
//      kgLineWidth(fid,12);
//      kgRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,fillcolor,0.0);
      kgLineColor(fid,color);
      if((Str != NULL) &&(Str[0]!='\0')) {
        kgTextSize(fid,th,tw,GAP*tw);
//        length = kgStringLength(Str,(int)tw);
//        uistrlngth(fid,Str,&length);
        length = kgStringLength(fid,Str);
        if(length>size*0.9) {
          fac = size*0.9/length;
          kgTextSize(fid,th,tw*fac,GAP*tw);
//          length = kgStringLength(Str,(int)(tw*fac));
//          uistrlngth(fid,Str,&length);
          length = kgStringLength(fid,Str);
        }
        if(!aspc) kgTextAngle(fid,90.);
        kgTextColor(fid,color);
        switch(justification) {
          case -1:
           if(aspc) kgMove2f(fid,0.0,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,0.0);
           break;
          case  0:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
          case  1:
           if(aspc) kgMove2f(fid,(xsize-length-tw*0.5),0.5*ysize-th*.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length));
           break;
          default:
           if(aspc) kgMove2f(fid,(xsize-length)*.5,0.5*ysize-th*0.5);
           else     kgMove2f(fid,0.5*xsize+th*0.5,(ysize-length)*0.5);
           break;
        }
        kgWriteText(fid,Str);
      }
//      img=kgGetResizedImage(fid);
      img=kgGetSharpImage(fid);
      kgCloseImage(fid);
   }
   return img;
}
int uiPressedBoxFill(void *Dialog,int xo,int yo,int xsize,int ysize,int fillcolor,int bodr,int bodrsize,float rfac){
/* -1 left justification ; 0 center ;1 right */
   void *fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   fid = kgInitImage(xsize,ysize,RESIZE);
   if(fid != NULL ) {
      kgUserFrame(fid,0.,0.,(float)xsize,(float)ysize);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,rfac);
      kgRoundedRectanglePressed(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,bodr,rfac,(float)bodrsize);
      img=kgGetResizedImage(fid);
      kgCloseImage(fid);
      kgImage(Dialog,img,xo,yo,xsize,ysize,0.0,1.0);
      uiFreeImage(img);
   }
   return 1;
}
int uiBoxFill(void *Dialog,int xo,int yo,int xsize,int ysize,int fillcolor,float rfac){
/* -1 left justification ; 0 center ;1 right */
   void * fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   fid = kgInitImage(xsize,ysize,RESIZE);
   if(fid != NULL ) {
      kgUserFrame(fid,0.,0.,(float)xsize,(float)ysize);
      kgRoundedRectangleFill(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,0,fillcolor,rfac);
      img=kgGetResizedImage(fid);
      kgCloseImage(fid);
      kgImage(Dialog,img,xo,yo,xsize,ysize,0.0,1.0);
      uiFreeImage(img);
   }
   return 1;
}
int uiRoundedBorder(void *Dialog,int xo,int yo,int xsize,int ysize,int bodrcolor,int bodrsize,float rfac){
/* -1 left justification ; 0 center ;1 right */
   void * fid;
   void *img=NULL;
   char *tmpdir,flname[200];
   fid = kgInitImage(xsize,ysize,RESIZE);
   if(fid != NULL ) {
      kgUserFrame(fid,0.,0.,(float)xsize,(float)ysize);
      kgRoundedRectangle(fid,(float)xsize*0.5,(float)ysize*0.5,(float)xsize,(float)ysize,bodrcolor,rfac,(float)bodrsize);
      img=kgGetResizedImage(fid);
      kgCloseImage(fid);
      kgImage(Dialog,img,xo,yo,xsize,ysize,0.0,1.0);
      uiFreeImage(img);
   }
   return 1;
}
void kgAddSearchDir(void *Tmp,char *Dir) {
     DIALOG *D;
     Dlink *L;
     char *dname;
     D = (DIALOG *)Tmp;
     L = (Dlink *)(D->SearchList);
     if(L==NULL) {
       L=Dopen();
       D->SearchList=L;
     }
     Resetlink(L);
     dname = (char *)Malloc(strlen(Dir)+2);
     strcpy(dname,Dir);
     strcat(dname,"/");
     Dinsert(L,dname);
}
void *kgBorderedRectangle(int width,int height,int fillclr,float rfac) {
  void *fid;
  void *img,*img1;
  float l,w,xo,yo;
  int r,g,b;
  float h,s,v,rf,gf,bf,Vb;
  int small=0;
  l=width;
  w = height;
  xo = l*0.5;
  yo = w*0.5;
  if(height<100) small=1;
  fid = kgInitImage(width,height,RESIZE);
   if(fid != NULL ) {
      kgUserFrame(fid,-3.0,-3.0,(float)l+3,(float)w+3);
      if(fillclr>=0)
          kgGetRGB((DIG *)fid,fillclr,&r,&g,&b);
      else {
         fillclr =-fillclr;
         b = fillclr%1000; fillclr= fillclr/1000;
         g = fillclr%1000; fillclr= fillclr/1000;
         r = fillclr;
      }
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      Vb = 1.5*v;
      if(Vb >1.) Vb=1.;
      HSVtoRGB(&rf,&gf,&bf,h,s,Vb);
      kgChangeColor(fid,152,(int)rf,(int)gf,(int)bf);
      HSVtoRGB(&rf,&gf,&bf,h,s,0.5*v);
      kgChangeColor(fid,153,(int)rf,(int)gf,(int)bf);
      kgChangeColor(fid,151,(int)r,(int)g,(int)b);
      if(small) {
      kgSmallRectangleFill(fid,xo,yo,(float)l+2,(float)w+2,0,153,rfac);
      kgSmallRectangleFill(fid,xo,yo,(float)l+1,(float)w+1,0,152,rfac);
      kgSmallRectangleFill(fid,xo,yo,(float)l-3,(float)w-3,0,151,rfac);
      }
      else {
      kgRoundedRectangleFill(fid,xo,yo,(float)l+2,(float)w+2,0,153,rfac);
      kgRoundedRectangleFill(fid,xo,yo,(float)l+1,(float)w+1,0,152,rfac);
      kgRoundedRectangleFill(fid,xo,yo,(float)l-3,(float)w-3,0,151,rfac);
      }
//      kgRoundedRectangle(fid,xo,yo,(float)l,(float)w,bodrclr,0.5,1);
      img=kgGetResizedImage(fid);
//      img = kgResizeImage(img1,0.25);
//      kgFreeImage(img1);
      
      kgCloseImage(fid);
   }
   return img;
}
void *kgPressedRectangle(int width,int height,int fillclr,float rfac) {
  void *fid;
  void *img,*img1;
  float l,w,xo,yo;
  int r,g,b;
  float h,s,v,rf,gf,bf,Vb;
  int small=0;
  l=width;
  w = height;
  xo = l*0.5;
  yo = w*0.5;
  if(height<100) small=1;
  fid = kgInitImage(width,height,RESIZE);
   if(fid != NULL ) {
      kgUserFrame(fid,-0.5,-0.5,(float)l+0.5,(float)w+0.5);
      if(fillclr>0)
          kgGetRGB((DIG *)fid,fillclr,&r,&g,&b);
      else {
         fillclr =-fillclr;
         b = fillclr%1000; fillclr= fillclr/1000;
         g = fillclr%1000; fillclr= fillclr/1000;
         r = fillclr;
      }
      RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      Vb = 2.0*v;
      if(Vb >1.) Vb=1.;
      HSVtoRGB(&rf,&gf,&bf,h,s,Vb);
      kgChangeColor(fid,253,(int)rf,(int)gf,(int)bf);
      HSVtoRGB(&rf,&gf,&bf,h,s,0.7*Vb);
      kgChangeColor(fid,252,(int)rf,(int)gf,(int)bf);
      kgChangeColor(fid,251,(int)r,(int)g,(int)b);
      if(small) {
      kgSmallRectangleFill(fid,xo,yo,(float)l,(float)w,0,252,rfac);
      kgSmallRectangleFill(fid,xo,yo,(float)l-1.5,(float)w-1.5,0,253,rfac);
      kgSmallRectangleFill(fid,xo,yo,(float)l-1.0,(float)w-1.0,0,253,rfac);
      kgSmallRectangleFill(fid,xo,yo,(float)l-0.5,(float)w-0.5,0,253,rfac);
      kgSmallRectangleFill(fid,xo,yo,(float)l-2,(float)w-2,0,251,rfac);
      }
      else {
      kgSmallRectangleFill(fid,xo,yo,(float)l,(float)w,0,252,rfac);
      kgSmallRectangleFill(fid,xo,yo,(float)l-1.5,(float)w-1.5,0,253,rfac);
      kgSmallRectangleFill(fid,xo,yo,(float)l-1.0,(float)w-1.0,0,253,rfac);
      kgSmallRectangleFill(fid,xo,yo,(float)l-0.5,(float)w-0.5,0,253,rfac);
      kgSmallRectangleFill(fid,xo,yo,(float)l-2,(float)w-2,0,251,rfac);
      }
//      kgRoundedRectangle(fid,xo,yo,(float)l,(float)w,bodrclr,0.5,1);
//      img=kgGetSharpImage(fid);
      img=kgGetResizedImage(fid);
//      img = kgResizeImage(img1,0.25);
//      kgFreeImage(img1);
      
      kgCloseImage(fid);
   }
   return img;
}
int kgImageCopy(void *Tmp,char *imgname) {
  DIG *G;
    int v_x1,v_y1,v_x2,v_y2,D_x,D_y,V_x,V_y;
    int EVGAY,EVGAX;
    int xl,yl,w,h;
    int v[4];
    char flname[20]={"Junk.Gph"},Tmpdir[200];
    unsigned long *data=NULL;
    char *tpt;
    void *img;
    DIALOG *D;
    int Evgay;
    kgWC *wc;
    kgDC *dc;
    G = (DIG *)Tmp;
    D = G->D;
    wc = G->wc;
    dc = G->dc;
    Evgay=D->evgay;
    if(dc->ZBUFF==1) {uiclear_zbuf(G);}
    EVGAX= G->x2-G->x1+1;
    EVGAY= abs(G->y2-G->y1)+1;
    v_x1=dc->vu_x1*(float)EVGAX;
    v_y1= dc->vu_y1*(float)EVGAY;
    v_x2=dc->vu_x2*(float)EVGAX-1;
    v_y2= dc->vu_y2*(float)EVGAY-1;
    V_x=v_x2-v_x1+1;
    V_y=v_y2-v_y1+1;
    D_x=G->x1+D->xo+v_x1;
    D_y = D->yo+G->y1+EVGAY-1-v_y2;
    w = V_x;
    h = V_y;
    xl = D_x;
    yl = D_y;
    tpt = D->tmpdir;
    strcpy(flname,tpt);
    strcat(flname,"/Junk.Gph");
    kgBackupGph(G,flname);
    uiUpdateOff(D);
    img = uiGetAntialiasedImage(flname,w,h,dc->ls_list,4);
    remove(flname);
   if(img!=NULL) {
     char buff[500];
     int pos;
     strcpy(buff,imgname);
     pos = kgSearchString(buff,".png");
     if(pos < 0) {
        pos = kgSearchString(buff,".jpg");
        if(pos < 0) {
          strcat(buff,".png");
        }
        else buff[pos+4]='\0';
     }
     else buff[pos+4]='\0';
     kgWriteImage(img,buff);
     uiFreeImage(img);
   }
   else printf( "img==NULL\n");
   uiUpdateOn(D);
   return 1;
}
