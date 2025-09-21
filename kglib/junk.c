/*
  Version 2.2
  Dated 08/03/2001
  Version 5.0
  Dated 26/11/10
*/
#define LINUX
#define ZBUFFER
#define DISPLAY ":0.0"
#include <unistd.h>
#include "kulina.h"
#include "gprivate.h"
#include "uievent.h"
#include "dlink.h"
#include "keycode.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <magick/api.h>
#ifdef RS6000
#include<aio.h>
#endif
#include<X11/Xlib.h>
#include<X11/X.h>
#include<X11/Xutil.h>
#include<X11/extensions/shape.h>
//#include<X11/extensions/Xcomposite.h>
#include<X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <termio.h>

#define ILLUFAC 1.0
#define PI 3.14159265
#define UNSIGNED unsigned short
#define uiset_clip_limits(wc,x1,y1,x2,y2) wc->c_v_x1=x1,wc->c_v_y1=y1,wc->c_v_x2=x2,wc->c_v_y2=y2
#define uibkup_clip_limits wc->c_v_x1_o=wc->c_v_x1,wc->c_v_y1_o=wc->c_v_y1,wc->c_v_x2_o=wc->c_v_x2,wc->c_v_y2_o=wc->c_v_y2
#define uiset_sup_clip_limits(wc,x1,y1,x2,y2) wc->s_v_x1=x1,wc->s_v_y1=y1,wc->s_v_x2=x2,wc->s_v_y2=y2
#define uirest_clip_limits wc->c_v_x1=wc->c_v_x1_o,wc->c_v_y1=wc->c_v_y1_o,wc->c_v_x2=wc->c_v_x2_o,wc->c_v_y2=wc->c_v_y2_o
static int Xid=0;
extern int TextSize,Ht,Wd,Gap,Bt;  // It is Okay For Thread;
static long EventMask =
  ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|VisibilityChangeMask|StructureNotifyMask|PropertyChangeMask;
static long EventMaskNoResize =
  ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|VisibilityChangeMask|PropertyChangeMask;
//  ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|VisibilityChangeMask|ResizeRedirectMask|StructureNotifyMask;
static long InputEventMask =
  KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|PropertyChangeMask;
static long ButtonMask =
  ExposureMask|ButtonPressMask|ButtonReleaseMask|VisibilityChangeMask|StructureNotifyMask;
static long KeyMask =
  ExposureMask|KeyPressMask|KeyReleaseMask|VisibilityChangeMask|StructureNotifyMask;
static int BackStore=0;
char SharedPicDir[500]="/usr/share/kulina/Pictures";
char SystemPicDir[500]="/usr/share/gllib/Pictures";
static tcflag_t oldc_c,oldc_l,oldc_i,oldc_o;
static cc_t oldc_c4, oldc_c5;   
static struct termios tty;

static void *root=NULL; /* for back ground setting */
static int PIX_CLR=255; /* colour allocation for pixmaps */

static int FontSize=15;
int syncfs(int fd);
#define IMAGE_BLUE_VAL  (((blue)*(wc->IMAGE->blue_mask)+(1<<(wc->BLUEMASKPOS-1)))/255)
#define IMAGE_GREEN_VAL ((((green) * (wc->IMAGE->green_mask) \
                  +(1<<(wc->GREENMASKPOS -1)))/255)&(wc->IMAGE->green_mask))
#define IMAGE_RED_VAL ((((red) * (wc->IMAGE->red_mask) \
                  +(1<<(wc->REDMASKPOS -1)))/255)&(wc->IMAGE->red_mask))
#define BLUE_VAL  (((val&0x0000ff)*(uiImage->blue_mask)+(1<<(BlueMaskPos-1)))/255)
#define GREEN_VAL (((((val>>8)&0x0000ff) * (uiImage->green_mask) \
                  +(1<<(GreenMaskPos -1)))/255)&(uiImage->green_mask))
#define RED_VAL (((((val>>16)&0x0000ff) * (uiImage->red_mask) \
                  +(1<<(RedMaskPos -1)))/255)&(uiImage->red_mask))
#define Image_BLUE_VAL  ((blue*(uiImage->blue_mask)+(1<<(BlueMaskPos-1)))/255)
#define Image_GREEN_VAL (((green * (uiImage->green_mask) \
                  +(1<<(GreenMaskPos -1)))/255)&(uiImage->green_mask))
#define Image_RED_VAL (((red * (uiImage->red_mask) \
                  +(1<<(RedMaskPos -1)))/255)&(uiImage->red_mask))
#define Img_BLUE_VAL  ((blue*(uiImage->blue_mask)+(1<<(BlueMaskPos-1)))/255)
#define Img_Color ((red<<16)+ (green<<8)+blue)
/*
static char FONTSTRF[60]= "-adobe-courier-medium-r-*-*-";
static char FONTSTRV[60]= "-adobe-courier-medium-r-*-*-";
static char FONTSTRV[60]= "-adobe-helvetica-bold-r-*-*-";
*/
static char FONTSTRF[60]= "-adobe-courier-bold-r-*-*-";
static char FONTSTRV[60]= "-adobe-helvetica-bold-r-*-*-";
static char FONTSTRV1[60]= "-adobe-new century schoolbook-medium-r-*-*-";
static char FONTSTRV2[60]= "-adobe-times-medium-r-*-*-";
static char FONTSTRV3[60]= "-adobe-utopia-medium-r-*-*-";
static char FONTSTR2[20]= "-*-*-*-*-*-*-*";
static  XFontStruct *fontstruct;
#define FONT "-adobe-courier-medium-r-*-*-16-*-*-*-*-*-*-*"
#define far 
#define SSF 0.6
