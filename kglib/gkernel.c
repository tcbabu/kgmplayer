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
  static int Xid = 0;
  extern int TextSize , Ht , Wd , Gap , Bt; // It is Okay For Thread;
  static long EventMask = ExposureMask | KeyPressMask  \
      | KeyReleaseMask | ButtonPressMask \
   | ButtonReleaseMask | PointerMotionMask | VisibilityChangeMask \
   | StructureNotifyMask | PropertyChangeMask;
  static long EventMaskNoResize = ExposureMask | KeyPressMask | KeyReleaseMask \
   | ButtonPressMask | ButtonReleaseMask | PointerMotionMask \
   | VisibilityChangeMask | PropertyChangeMask;
//  ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|VisibilityChangeMask|ResizeRedirectMask|StructureNotifyMask;
  static long InputEventMask = KeyPressMask | KeyReleaseMask | ButtonPressMask \
   | ButtonReleaseMask | PointerMotionMask | PropertyChangeMask;
  static long ButtonMask = ExposureMask | ButtonPressMask | ButtonReleaseMask \
   | VisibilityChangeMask | StructureNotifyMask;
  static long KeyMask = ExposureMask | KeyPressMask | KeyReleaseMask \
   | VisibilityChangeMask | StructureNotifyMask;
  static int BackStore = 0;
  char SharedPicDir [ 500 ] = "/usr/share/kulina/Pictures";
  char SystemPicDir [ 500 ] = "/usr/share/gllib/Pictures";
  static tcflag_t oldc_c , oldc_l , oldc_i , oldc_o;
  static cc_t oldc_c4 , oldc_c5;
  static struct termios tty;
  static void *root = NULL; /* for back ground setting */
  static int PIX_CLR = 255; /* colour allocation for pixmaps */
  static int FontSize = 15;
  int syncfs ( int fd ) ;
#define IMAGE_BLUE_VAL  (((blue)*(wc->IMAGE->blue_mask)+(1<<(wc->BLUEMASKPOS-1)))/255)
#define IMAGE_GREEN_VAL ((((green) * (wc->IMAGE->green_mask) \
   + ( 1 << ( wc->GREENMASKPOS -1 ) ) ) /255 ) & ( wc->IMAGE->green_mask ) )
#define IMAGE_RED_VAL ((((red) * (wc->IMAGE->red_mask) \
   + ( 1 << ( wc->REDMASKPOS -1 ) ) ) /255 ) & ( wc->IMAGE->red_mask ) )
#define BLUE_VAL  (((val&0x0000ff)*(uiImage->blue_mask)+(1<<(BlueMaskPos-1)))/255)
#define GREEN_VAL (((((val>>8)&0x0000ff) * (uiImage->green_mask) \
   + ( 1 << ( GreenMaskPos -1 ) ) ) /255 ) & ( uiImage->green_mask ) )
#define RED_VAL (((((val>>16)&0x0000ff) * (uiImage->red_mask) \
   + ( 1 << ( RedMaskPos -1 ) ) ) /255 ) & ( uiImage->red_mask ) )
#define Image_BLUE_VAL  ((blue*(uiImage->blue_mask)+(1<<(BlueMaskPos-1)))/255)
#define Image_GREEN_VAL (((green * (uiImage->green_mask) \
   + ( 1 << ( GreenMaskPos -1 ) ) ) /255 ) & ( uiImage->green_mask ) )
#define Image_RED_VAL (((red * (uiImage->red_mask) \
   + ( 1 << ( RedMaskPos -1 ) ) ) /255 ) & ( uiImage->red_mask ) )
#define Img_BLUE_VAL  ((blue*(uiImage->blue_mask)+(1<<(BlueMaskPos-1)))/255)
#define Img_Color ((red<<16)+ (green<<8)+blue)
/*
static char FONTSTRF[60]= "-adobe-courier-medium-r-*-*-";
static char FONTSTRV[60]= "-adobe-courier-medium-r-*-*-";
static char FONTSTRV[60]= "-adobe-helvetica-bold-r-*-*-";
*/
  static char FONTSTRF [ 60 ] = "-adobe-courier-bold-r-*-*-";
  static char FONTSTRV [ 60 ] = "-adobe-helvetica-bold-r-*-*-";
  static char FONTSTRV1 [ 60 ] = "-adobe-new century schoolbook-medium-r-*-*-";
  static char FONTSTRV2 [ 60 ] = "-adobe-times-medium-r-*-*-";
  static char FONTSTRV3 [ 60 ] = "-adobe-utopia-medium-r-*-*-";
  static char FONTSTR2 [ 20 ] = "-*-*-*-*-*-*-*";
  static XFontStruct *fontstruct;
#define FONT "-adobe-courier-medium-r-*-*-16-*-*-*-*-*-*-*"
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
#define Escape           XK_Escape
#define Delete           XK_Delete
#define Insert           XK_Insert
#define Kp_Delete           XK_KP_Delete
#define Pageup           XK_Page_Up
#define Kp_Pageup           XK_KP_Page_Up
#define Pagedown         XK_Page_Down
#define Kp_Pagedown         XK_KP_Page_Down
#define Home             XK_Home
#define Kp_Home             XK_KP_Home
#define Clear              XK_Clear
#define Print              XK_Print
#define End              XK_End
#define Kp_End              XK_KP_End
#define Shiftleft            XK_Shift_L
#define Shiftright            XK_Shift_R
#define Space            XK_KP_Space
#define Capslock         XK_Caps_Lock
#define Shiftlock         XK_Shift_Lock
#define Control           XK_Control_L
#define Alt           XK_Alt_L
  static union kbinp { short kbint; char kbc [ 2 ] ;} kb;
  int NCLRS = 1024;
#define CopyPoint(x,y,i) XCopyArea(Dsp,Win,linebuf,Gc,(short)x,(short)(y) ,1,1,i,0)
#define GetPoint(x,y,i) XCopyArea(Dsp,linebuf,Win,Gc,i,0,1,1,(short)x,(short)(y))
#define icde(i,j) kgIcode[j-1][i-1]
  short kgIcode [ 1024 ] [ 3 ] = {
      0 , 0 , 0 , /* black */
      0 , 0 , 255 , /* blue */
      0 , 255 , 0 , /* green */
      255 , 0 , 0 , 255 , 255 , 0 , /* yellow */
      49 , 101 , 255 , /* blue1 */
      0 , 255 , 255 , /* meroon */
      255 , 162 , 162 , /* gray3 */
      0 , 190 , 255 , /* deep */
      0 , 0 , 130 , /* navy */
      101 , 150 , 239 , /* cornflower */
      0 , 220 , 0 , 180 , 0 , 0 , 220 , 0 , 220 , 134 , 113 , 255 , /* light */
      255 , 255 , 255 , /* whilte */
      0 , 0 , 206 , /* medium */
      65 , 105 , 227 , /* royal */
      28 , 146 , 255 , /* dodger */
      24 , 24 , 113 , /* midnight */
      134 , 206 , 235 , /* sky */
      134 , 206 , 251 , /* light */
      69 , 130 , 182 , /* steel */
      178 , 198 , 223 , /* light */
      174 , 219 , 231 , /* light */
      178 , 227 , 231 , /* powder */
      174 , 239 , 239 , /* pale */
      0 , 206 , 210 , /* dark */
      73 , 210 , 206 , /* medium */
      65 , 227 , 210 , /* turquoise */
      227 , 255 , 255 , /* light */
      220 , 0 , 0 , 105 , 89 , 206 , /* slate */
      150 , 0 , 0 , 120 , 0 , 0 , 73 , 61 , 138 , /* dark */
      0 , 180 , 0 , 0 , 150 , 0 , 0 , 120 , 0 , 0 , 0 , 220 , 66 , 66 , 66 , 99 , \
           99 , 99 , 143 , 143 , 143 , 187 , 187 , 187 , 220 , 220 , 220 , 240 , \
           255 , 254 , 242 , 246 , 255 , 244 , 247 , 255 , /* light */
      40 , 45 , 40 , 50 , 55 , 50 , 220 , 230 , 220 , 180 , 190 , 180 , 20 , 30 , \
           20 , 40 , 50 , 40 , 60 , 65 , 60 , 220 , 220 , 0 , 180 , 180 , 0 , \
           150 , 150 , 0 , 120 , 120 , 0 , 121 , 105 , 239 , /* medium */
  180 , 0 , 180 , 150 , 0 , 150 , 120 , 0 , 120 , 93 , 158 , 162 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
       /* whilte */
  static XColor DefClrs [ 1024 ] ;
  static unsigned long clrtbl [ 1024 ] = {0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , \
       8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , 17 , 18 , 19 , 20 , 21 , 22 , \
       23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , 36 , 37 , \
       38 , 39 , 40 , 41 , 42 , 43 , 44 , 45 , 46 , 47 , 48 , 49 , 50 , 51 , 52 , \
       53 , 54 , 55 , 56 , 57 , 58 , 59 , 60 , 61 , 62 , 63 , 64 , 65 , 66 , 67 , \
       68 , 69 , 70 , 71 , 72 , 73 , 74 , 75 , 76 , 77 , 78 , 79 , 80 , 81 , 82 , \
       83 , 84 , 85 , 86 , 87 , 88 , 89 , 90 , 91 , 92 , 93 , 94 , 95 , 96 , 97 , \
       98 , 99 , 100 , 101 , 102 , 103 , 104 , 105 , 106 , 107 , 108 , 109 , 110 , \
       111 , 112 , 113 , 114 , 115 , 116 , 117 , 118 , 119 , 120 , 121 , 122 , \
       123 , 124 , 125 , 126 , 127 , 128 , 129 , 130 , 131 , 132 , 133 , 134 , \
       135 , 136 , 137 , 138 , 139 , 140 , 141 , 142 , 143 , 144 , 145 , 146 , \
       147 , 148 , 149 , 150 , 151 , 152 , 153 , 154 , 155 , 156 , 157 , 158 , \
       159 , 160 , 161 , 162 , 163 , 164 , 165 , 166 , 167 , 168 , 169 , 170 , \
       171 , 172 , 173 , 174 , 175 , 176 , 177 , 178 , 179 , 180 , 181 , 182 , \
       183 , 184 , 185 , 186 , 187 , 188 , 189 , 190 , 191 , 192 , 193 , 194 , \
       195 , 196 , 197 , 198 , 199 , 200 , 201 , 202 , 203 , 204 , 205 , 206 , \
       207 , 208 , 209 , 210 , 211 , 212 , 213 , 214 , 215 , 216 , 217 , 218 , \
       219 , 220 , 221 , 222 , 223 , 224 , 225 , 226 , 227 , 228 , 229 , 230 , \
       231 , 232 , 233 , 234 , 235 , 236 , 237 , 238 , 239 , 240 , 241 , 242 , \
       243 , 244 , 245 , 246 , 247 , 248 , 249 , 250 , \
       251 , 252 , 253 , 254 , 255 , -1 };
#define uiXDRAW(wc,x1,y1,x2,y2) XDrawLine(wc->Dsp,wc->ActWin,wc->Gc,(short)(x1),(short)(wc->EVGAY-y1),(short)(x2),(short)(wc->EVGAY-y2))
#define dvXDRAW(wc,x1,y1,x2,y2) XDrawLine(wc->Dsp,wc->ActWin,wc->Gc,(short)(x1),(short)(y1),(short)(x2),(short)(y2))
#define XCPix(x1,y1,x2,y2) XCreatePixmap(Dsp,Win,(short)(x2-(x1)+1),(short)(y2-(y1)+1),Dpth);
#define XCPix1(x1,y1,x2,y2) XCreatePixmap(Dsp,Win,(short)(x2-(x1)+1),(short)(y2-(y1)+1),1);
#define kgXCPix(x1,y1,x2,y2) XCreatePixmap((Display *)WC(D)->Dsp, (Window)WC(D)->Win,(short)(x2-(x1)+1),(short)(y2-(y1)+1),WC(D)->Dpth);
#define wcXCPix(x1,y1,x2,y2) XCreatePixmap(wc->Dsp, wc->Win,(short)(x2-(x1)+1),(short)(y2-(y1)+1),wc->Dpth);
  static int Shift = 0 , Caplock = 0;
  static int Revscan_code [ 256 ] ;
  static int Scan_code [ 256 ] = {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 27 , '1' , \
       '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' , '-' , '=' , '\b' , \
       0 , 'q' , 'w' , 'e' , 'r' , 't' , 'y' , 'u' , 'i' , 'o' , 'p' , 0 , 0 , \
       '\r' , 0 , 'a' , 's' , 'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l' , ';' , \
       '\'' , '`' , 0 , '\\' , 'z' , 'x' , 'c' , 'v' , 'b' , 'n' , 'm' , ',' , \
       '.' , '/' , 0 , 0 , 0 , ' ' , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 103 , 104 , 105 , 106};
  static int Scan_sh_code [ 256 ] = {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 27 , \
       '!' , ' ' , '#' , '$' , '%' , '^' , '&' , '*' , '(' , ')' , '_' , '+' , \
       '\b' , 0 , 'Q' , 'W' , 'E' , 'R' , 'T' , 'Y' , 'U' , 'I' , 'O' , 'P' , \
       0 , 0 , '\r' , 0 , 'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L' , \
       ':' , '"' , '~' , 0 , '|' , 'Z' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M' , '<' , \
       '>' , '?' , 0 , 0 , 0 , ' ' , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , \
       0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 103 , 104 , 105 , 106};
  static int GetShift ( unsigned long val ) ;
  XImage * kg_GetImage ( DIALOG *D , int x , int y , int width , int height ) ;
  void kgSync ( void *Tmp ) {
      kgWC *wc;
      wc = ( ( DIALOG * ) Tmp )->wc;
      XSync ( wc->Dsp , False ) ;
  }
  void rmv_pointer ( ) {}
  void dsp_pointer ( ) {}
  void draw_pointer ( int x , int y ) {}
  void _uiTileImage ( kgWC *wc , Pixmap pix , \
       XImage *ximage , int wd , int ht ) ;
  void * kgProcessSelectionRequest ( void *Tmp ) ;
  int kgEnableSelection ( void *Tmp ) ;
/*
  Getting Current Color
*/
  void uigetcurrgb ( kgWC *wc , int *r , int *g , int *b ) {
      kgColor *kgcolors;
      int i;
      i = wc->c_color;
      kgcolors = ( kgColor * ) ( wc->kgcolors ) ;
      *r = kgcolors [ i ] .red;
      *g = kgcolors [ i ] .green;
      *b = kgcolors [ i ] .blue;
  }
  void kgGetDefaultRGB ( int color , int *r , int *g , int *b ) {
      kgColor *kgcolors;
      int i;
      i = color;
      *r = kgIcode [ i ] [ 0 ] ;
      *g = kgIcode [ i ] [ 1 ] ;
      *b = kgIcode [ i ] [ 2 ] ;
  }
/*
  Getting pointer display is on or not. Only useful
  in DOS mode; otherwise a dummy routine is OK
*/
/*
  Setting current color
*/
  void uiset_clr ( DIALOG *D , int x ) {
      kgWC *wc;
      kgColor *kgcolors;
      wc = WC ( D ) ;
      wc->c_color = x;
      kgcolors = ( kgColor * ) wc->kgcolors;
      XSetForeground ( wc->Dsp , wc->Gc , ( unsigned int ) \
       ( kgcolors [ x%NCLRS ] .clrtbl ) ) ;
  }
  void wcset_clr ( kgWC *wc , int x ) {
      kgColor *kgcolors;
      wc->c_color = x;
      kgcolors = ( kgColor * ) wc->kgcolors;
      XSetForeground ( ( Display * ) wc->Dsp , ( GC ) wc->Gc , \
       ( unsigned int ) ( kgcolors [ x%NCLRS ] .clrtbl ) ) ;
  }
/*
  to scroll a rectangular area backward to a given width
*/
  void uiscr_scroll_back ( DIALOG *D , int x1 , \
       int y1 , int x2 , int y2 , int width ) \
  {
      int i , j , addr;
      XCopyArea ( ( Display * ) WC ( D )->Dsp , ( Pixmap ) WC ( D )->DspWin , \
       ( Pixmap ) WC ( D )->DspWin , ( GC ) WC ( D )->Gc , ( short ) x1 , ( short ) \
       ( y1 ) , ( short ) ( x2-x1+1 ) , ( short ) ( y2-y1+1 ) , \
       ( short ) x1 , ( short ) ( y1-width ) ) ;
  }
  int kgRunJob ( char *job , int ( *ProcessOut ) ( int , int , int ) ) {
      FILE *fp , *fp1;
      int pip [ 2 ] , pid , status , pip2 [ 2 ] ;
      char *args [ 100 ] , buff [ 1000 ] , pt [ 300 ] ;
      char *pgrpath = NULL;
      int i = 0 , pos = 0;
      if ( job == NULL ) return 0;
      if ( pipe ( pip ) < 0 ) return 0;
      if ( pipe ( pip2 ) < 0 ) return 0;
//   pipew =pip2[1];
      while ( job [ i ] == ' ' ) i++;
      strcpy ( buff , job+i ) ;
      i = 0;
      while ( sscanf ( buff+pos , "%s" , pt ) > 0 ) {
          if ( pt [ 0 ] == '\"' ) {
              pos++;
              args [ i ] = buff+pos;
              while ( buff [ pos ] != '\"' ) pos++;
              buff [ pos ] = '\0';
              i++;
          }
          else if ( pt [ 0 ] == '\\' ) {
              pos++;
              args [ i ] = buff+pos;
              while ( buff [ pos ] != '\\' ) pos++;
              buff [ pos ] = '\0';
              i++;
          }
          else {
              args [ i ] = buff+pos;
              pos += strlen ( pt ) ;
              i++;
              if ( buff [ pos ] < ' ' ) break;
              buff [ pos ] = '\0';
          }
          pos++;
          while ( buff [ pos ] == ' ' ) pos++;
      }
      args [ i ] = NULL;
      if ( i == 0 ) return 0;
      pgrpath = kgWhich ( args [ 0 ] ) ;
      if ( pgrpath == NULL ) return 0;
      pid = fork ( ) ;
      if ( pid == 0 ) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
          close ( 0 ) ;
          dup ( pip2 [ 0 ] ) ;
          close ( pip2 [ 0 ] ) ;
          close ( 1 ) ;
          dup ( pip [ 1 ] ) ;
          close ( 2 ) ;
#if 1
          open ( "/dev/null" , O_WRONLY | O_CREAT , 0777 ) ;
#else
          dup ( pip [ 1 ] ) ;
#endif
          close ( pip [ 1 ] ) ;
          execv ( pgrpath , args ) ;
          fprintf ( stderr , "Failed to execute \n" ) ;
          exit ( 1 ) ;
      }
      else { /* parent process */
//     printf("Waiting pid: %d\n",pid);
          int ret = 0;
          close ( pip2 [ 0 ] ) ;
          close ( pip [ 1 ] ) ;
          if ( ProcessOut != NULL ) ret = ProcessOut ( pip [ 0 ] , pip2 [ 1 ] , pid ) ;
          WAIT ( pid ) ;
          free ( pgrpath ) ;
          return pid;
      }
  }
  int kgChangeJob ( char *job ) {
      FILE *fp , *fp1;
      int pip [ 2 ] , pid , status , pip2 [ 2 ] ;
      char *args [ 100 ] , buff [ 1000 ] , pt [ 300 ] ;
      char *pgrpath = NULL;
      int i = 0 , pos = 0;
      if ( job == NULL ) return 0;
      while ( job [ i ] == ' ' ) i++;
      strcpy ( buff , job+i ) ;
      i = 0;
      while ( sscanf ( buff+pos , "%s" , pt ) > 0 ) {
          if ( pt [ 0 ] == '\"' ) {
              pos++;
              args [ i ] = buff+pos;
              while ( buff [ pos ] != '\"' ) pos++;
              buff [ pos ] = '\0';
              i++;
          }
          else if ( pt [ 0 ] == '\\' ) {
              pos++;
              args [ i ] = buff+pos;
              while ( buff [ pos ] != '\\' ) pos++;
              buff [ pos ] = '\0';
              i++;
          }
          else {
              args [ i ] = buff+pos;
              pos += strlen ( pt ) ;
              i++;
              if ( buff [ pos ] < ' ' ) break;
              buff [ pos ] = '\0';
          }
          pos++;
          while ( buff [ pos ] == ' ' ) pos++;
      }
      args [ i ] = NULL;
      if ( i == 0 ) return 0;
      pgrpath = kgWhich ( args [ 0 ] ) ;
      if ( pgrpath == NULL ) return 0;
      execv ( pgrpath , args ) ;
      fprintf ( stderr , "Failed to execute \n" ) ;
      exit ( 1 ) ;
  }
  static int isunixsocketinuse ( char *sock ) {
      FILE *fp;
      char buff1 [ 300 ] ;
      char *line = NULL;
      int ret = 0 , no , id;
      size_t len = 0;
      ssize_t read;
      fp = fopen ( "/proc/net/unix" , "r" ) ;
      if ( fp == NULL ) return -1;
//  read = getline(&line, &len, fp);
      while ( fscanf ( fp , "%s" , buff1 ) > 0 ) {
          if ( strcmp ( buff1 , sock ) == 0 ) {ret = 1; break;}
      }
      fclose ( fp ) ;
      free ( line ) ;
      return ret;
  }
  static int isdisplayinuse ( int num ) {
      char buff [ 200 ] ;
      sprintf ( buff , "/tmp/.X11-unix/X%-d" , num ) ;
      return isunixsocketinuse ( buff ) ;
  }
  int kgStartX ( char *logfile ) {
      Display *Dsp;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp != NULL ) {
          XCloseDisplay ( Dsp ) ;
          fprintf ( stderr , "Xorg Active \n" ) ;
          fflush ( stderr ) ;
          return 1;
      }
      kgRunJob ( "killall Xorg" , NULL ) ;
      kgRunJob ( "killall X" , NULL ) ;
      remove ( "/tmp/.X0-lock" ) ;
      remove ( "/tmp/.X11-unix/X0" ) ;
      if ( ( Xid = fork ( ) ) == 0 ) {
          if ( logfile == NULL ) {
//	  kgChangeJob( "Xorg :0.0 vt7 -quiet -allowMouseOpenFail -terminate -reset -nopn   -retro  ");
              kgChangeJob ( "Xorg :0.0 vt7 -quiet -allowMouseOpenFail -reset -nopn   -retro  " ) ;
                  
          }
          else {
              char command [ 500 ] ;
//		sprintf(command,"Xorg :0.0 vt7 -quiet -allowMouseOpenFail -terminate -reset -nopn   -retro   -logfile %s",logfile);
              sprintf ( command , "Xorg :0.0 vt7 -quiet -allowMouseOpenFail  -reset -nopn   -retro   -logfile %s" , \
                   logfile ) ;
              kgChangeJob ( command ) ;
          }
          exit ( 1 ) ;
      }
      while ( ! isdisplayinuse ( 0 ) ) ;
      return Xid;
  }
  int kgCloseX ( void ) {
      if ( Xid ) {
          kill ( Xid , SIGTERM ) ;
//		kill(Xid,SIGHUP);
//		kill(Xid,SIGKILL);
      }
      return 1;
  }
/*
  Getting parent of a given routine; used only in this file
*/
  void kgDefineColor ( int clr , unsigned char red , \
       unsigned char green , unsigned char blue ) \
  {
      kgIcode [ clr ] [ 0 ] = red;
      kgIcode [ clr ] [ 1 ] = green;
      kgIcode [ clr ] [ 2 ] = blue;
  }
  int uichng_clr ( DIALOG *Dtmp , int i , int ir , int ig , int ib ) {
      int ret = 1 , ser;
      DIALOG *D;
      Display *Dsp;
      Colormap Cmap;
      kgColor *kgcolors;
      D = ( DIALOG * ) getParentDisplay ( Dtmp ) ;
      kgcolors = ( kgColor * ) ( WC ( D )->kgcolors ) ;
      Dsp = ( Display * ) WC ( D )->Dsp;
      Cmap = ( Colormap ) WC ( D )->Cmap;
      XColor col;
      if ( i < 0 ) return -1;
      if ( i >= NCLRS ) i = i%NCLRS +1;
      col.flags = DoRed | DoGreen | DoBlue;
      col.pixel = kgcolors [ i ] .clrtbl;
      col.red = ir*65535/255;
      col.green = ig*65535/255;
      col.blue = ib*65535/255;
    /*XFreeColors(Dsp,Cmap,&(col.pixel),1,0);*/
      if ( XAllocColor ( Dsp , Cmap , & col ) == 0 ) {
       /* printf("Alloc error %d \n",col.pixel);*/
          ret = 0;
          ser = uiSearchClr ( kgcolors , col ) ;
          col = kgcolors [ ser ] .Col;
      };
      kgcolors [ i ] .Col = col;
      kgcolors [ i ] .clrtbl = col.pixel;
      kgcolors [ i ] .red = col.red*255./65535+0.5;
      kgcolors [ i ] .green = col.green*255./65535+0.5;
      kgcolors [ i ] .blue = col.blue*255./65535+0.5;
      return ret;
  }
/*
  Getting the color definition of a given index
*/
  void uiget_color ( Display *Dsp , Colormap Cmap , \
       int i , short *ir , short *ig , short *ib ) \
  {
      XColor col;
      if ( i >= NCLRS ) i = i%NCLRS +1;
      col.pixel = clrtbl [ i ] ;
      XQueryColor ( Dsp , Cmap , & col ) ;
      *ir = col.red*255./65535+0.5;
      *ig = col.green*255./65535+0.5;
      *ib = col.blue*255./65535+0.5;
  }
  void uiput_pixl ( kgWC *wc , short col , short row ) {
      XDrawPoint ( wc->Dsp , wc->ActWin , wc->Gc , ( short ) col , ( short ) row ) ;
      return;
  }
  int kgCheckTrueColor ( void ) {
      int ret = 0;
      Display *Dsp;
      XVisualInfo visualinfo ;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
      if ( Dsp != NULL ) {
          ret = XMatchVisualInfo ( Dsp , DefaultScreen \
           ( Dsp ) , 32 , TrueColor , & visualinfo ) ;
          XCloseDisplay ( Dsp ) ;
      }
      return ret;
  }
/*
   Setting visual type ; used in this file only
*/
  int set_visual ( Visual *vis ) {
      int visclass , VGA16;
      visclass = vis->class;
      if ( visclass == PseudoColor ) {/*printf ( "PseudoColor\n" ) ;*/VGA16 = 0;}
      else VGA16 = 1;
      return VGA16;
  }
/*
  Setting colors to the default value
*/
  void uiset_def_colors ( Display *Dsp , Colormap Cmap ) {
      XColor col;
      int i , ser;
      for ( i = 0;i < NCLRS;i++ ) {
          col.pixel = clrtbl [ i ] ;
          col.red = kgIcode [ i ] [ 0 ] *65535/255;
          col.green = kgIcode [ i ] [ 1 ] *65535/255;
          col.blue = kgIcode [ i ] [ 2 ] *65535/255;
          col.flags = DoRed | DoGreen | DoBlue;
          if ( XAllocColor ( Dsp , Cmap , & col ) == 0 ) {
       /*printf("Alloc error %d \n",col.pixel);*/
              ser = SearchClr ( col ) ;
              col = DefClrs [ ser ] ;
          };
          DefClrs [ i ] = col;
          clrtbl [ i ] = col.pixel;
          kgIcode [ i ] [ 0 ] = col.red*255./65535+0.5;
          kgIcode [ i ] [ 1 ] = col.green*255./65535+0.5;
          kgIcode [ i ] [ 2 ] = col.blue*255./65535+0.5;
      }
  }
  void uiSetDefClr ( kgWC *wc , int loc , XColor C ) {
      kgColor *kgcolors;
      kgcolors = ( kgColor * ) wc->kgcolors;
      kgcolors [ loc ] .Col = C;
  }
  int uiSetGuiFixFontSize ( DIALOG *D , int size ) {
      XGCValues gcv;
      char FONTNEW [ 300 ] ;
      Display *Dsp;
      kgWC *wc;
      Dsp = ( Display * ) WC ( D )->Dsp;
      wc = D->wc;
//  if(size > 10) size=9;
      D->gc.Font = 18;
      D->gc.FontSize = size;
//  wc->GuiFont=18;
//  wc->GuiFontSize=size;
      sprintf ( FONTNEW , "%-s%-2.2d%-s" , FONTSTRF , size , FONTSTR2 ) ;
      fontstruct = XLoadQueryFont ( Dsp , FONTNEW ) ;
      if ( fontstruct == NULL ) {
//    printf("Error: Invalid Font\n");
//    printf("       Trying fixed\n");
          fontstruct = XLoadQueryFont ( Dsp , "fixed" ) ;
          if ( fontstruct == NULL ) exit ( 1 ) ;
      }
      gcv.font = fontstruct->fid;
      XChangeGC ( Dsp , ( GC ) WC ( D )->Gc , GCFont , & gcv ) ;
      XFreeFont ( Dsp , fontstruct ) ;
      return 1;
  }
  int uiSetNoechoFontSize ( DIALOG *D , int size ) {
      XGCValues gcv;
      char FONTNEW [ 300 ] ;
      Display *Dsp;
      kgWC *wc;
      Dsp = ( Display * ) WC ( D )->Dsp;
      wc = D->wc;
//  if(size > 10) size=9;
      D->gc.Font = 35;
      D->gc.FontSize = size;
//  wc->GuiFont=18;
//  wc->GuiFontSize=size;
      sprintf ( FONTNEW , "%-s%-2.2d%-s" , FONTSTRF , size , FONTSTR2 ) ;
      fontstruct = XLoadQueryFont ( Dsp , FONTNEW ) ;
      if ( fontstruct == NULL ) {
//    printf("Error: Invalid Font\n");
//    printf("       Trying fixed\n");
          fontstruct = XLoadQueryFont ( Dsp , "fixed" ) ;
          if ( fontstruct == NULL ) exit ( 1 ) ;
      }
      gcv.font = fontstruct->fid;
      XChangeGC ( Dsp , ( GC ) WC ( D )->Gc , GCFont , & gcv ) ;
      XFreeFont ( Dsp , fontstruct ) ;
      return 1;
  }
  int uiDefaultGuiFontSize ( DIALOG *D ) {
      static XGCValues gcv;
      char FONTNEW [ 300 ] ;
      static XFontStruct *fontstruct;
      Display *Dsp;
      kgWC *wc;
      Dsp = ( Display * ) WC ( D )->Dsp;
      wc = D->wc;
      D->gc.Font = D->gc.PromptFont;
      D->gc.FontSize = D->gc.GuiFontSize;
      sprintf ( FONTNEW , "%-s%-2.2d%-s" , FONTSTRV , FontSize , FONTSTR2 ) ;
      fontstruct = XLoadQueryFont ( Dsp , FONTNEW ) ;
      if ( fontstruct == NULL ) {
          sprintf ( FONTNEW , "%-s%-2.2d%-s" , FONTSTRV1 , FontSize , FONTSTR2 ) ;
          fontstruct = XLoadQueryFont ( Dsp , FONTNEW ) ;
      }
      if ( fontstruct == NULL ) {
          sprintf ( FONTNEW , "%-s%-2.2d%-s" , FONTSTRV2 , FontSize , FONTSTR2 ) ;
          fontstruct = XLoadQueryFont ( Dsp , FONTNEW ) ;
      }
      if ( fontstruct == NULL ) {
          sprintf ( FONTNEW , "%-s%-2.2d%-s" , FONTSTRV3 , FontSize , FONTSTR2 ) ;
          fontstruct = XLoadQueryFont ( Dsp , FONTNEW ) ;
      }
      if ( fontstruct == NULL ) {
//      printf("Error: Invalid Font\n");
//      printf("       Trying fixed\n");
          fontstruct = XLoadQueryFont ( Dsp , "fixed" ) ;
          if ( fontstruct == NULL ) exit ( 1 ) ;
      }
      gcv.font = fontstruct->fid;
      XChangeGC ( Dsp , ( GC ) WC ( D )->Gc , GCFont , & gcv ) ;
      XFreeFont ( Dsp , fontstruct ) ;
      return 1;
  }
  int uiDefaultDisplayFontSize ( Display *Dsp , GC Gc ) {
      static XGCValues gcv;
      char FONTNEW [ 300 ] ;
      static XFontStruct *fontstruct;
      sprintf ( FONTNEW , "%-s%-2.2d%-s" , FONTSTRV , FontSize , FONTSTR2 ) ;
      fontstruct = XLoadQueryFont ( Dsp , FONTNEW ) ;
      if ( fontstruct == NULL ) {
          sprintf ( FONTNEW , "%-s%-2.2d%-s" , FONTSTRV1 , FontSize , FONTSTR2 ) ;
          fontstruct = XLoadQueryFont ( Dsp , FONTNEW ) ;
      }
      if ( fontstruct == NULL ) {
          sprintf ( FONTNEW , "%-s%-2.2d%-s" , FONTSTRV2 , FontSize , FONTSTR2 ) ;
          fontstruct = XLoadQueryFont ( Dsp , FONTNEW ) ;
      }
      if ( fontstruct == NULL ) {
          sprintf ( FONTNEW , "%-s%-2.2d%-s" , FONTSTRV3 , FontSize , FONTSTR2 ) ;
          fontstruct = XLoadQueryFont ( Dsp , FONTNEW ) ;
      }
      if ( fontstruct == NULL ) {
//     printf("Error: Invalid Font\n");
//      printf("       Trying fixed\n");
          fontstruct = XLoadQueryFont ( Dsp , "fixed" ) ;
          if ( fontstruct == NULL ) exit ( 1 ) ;
      }
      gcv.font = fontstruct->fid;
      XChangeGC ( Dsp , Gc , GCFont , & gcv ) ;
      XFreeFont ( Dsp , fontstruct ) ;
      return 1;
  }
  static int GetNoOfBits ( unsigned long val ) {
      unsigned int mask = 1;
      int nbit = 0;
      while ( 1 ) {
          if ( val == 0 ) break;
          val = val >> mask;
          nbit++;
      }
      return nbit;
  }
  static int GetShift ( unsigned long val ) {
      unsigned int mask = 1;
      int nbit = 0;
      if ( val != 0 ) {
          while ( 1 ) {
              if ( val & 1 ) break;
              val = val >> mask;
              nbit++;
          }
      }
      return nbit;
  }
  int GetDisplaySize ( int *xres , int *yres ) {
      Display *Dsp;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
      if ( Dsp == NULL ) {printf ( " Error: in opening Display ..\n" ) ;
      exit ( 0 ) ;};
      *xres = DisplayWidth ( Dsp , DefaultScreen ( Dsp ) ) ;
      *yres = DisplayHeight ( Dsp , DefaultScreen ( Dsp ) ) ;
      XCloseDisplay ( Dsp ) ;
      return 1;
  }
  int kgDisplaySize ( int *xres , int *yres ) {
      Display *Dsp;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
      if ( Dsp == NULL ) {printf ( " Error: in opening Display ..\n" ) ;
      exit ( 0 ) ;};
      *xres = DisplayWidth ( Dsp , DefaultScreen ( Dsp ) ) ;
      *yres = DisplayHeight ( Dsp , DefaultScreen ( Dsp ) ) ;
      XCloseDisplay ( Dsp ) ;
      return 1;
  }
  int wcSendEvent ( kgWC *wc , XEvent *event ) {
      int status;
#if 1
      status = XSendEvent ( wc->Dsp , DefaultRootWindow ( wc->Dsp ) , False , \
       ( SubstructureNotifyMask | SubstructureRedirectMask ) , event ) ;
//  if(status==0) printf("Send Request Failed\n");
#endif
      return status;
  }
  int kgSendKeyEvent ( void *Tmp , int ch ) {
      int code;
      XEvent *e;
      XKeyEvent *k;
      DIALOG *D;
      kgWC *wc;
      int status;
      int state = 0;
      e = ( XEvent * ) Malloc ( sizeof ( XEvent ) ) ;
      k = ( XKeyEvent * ) e;
      D = ( DIALOG * ) Tmp;
      wc = WC ( D ) ;
      k->type = KeyRelease;
      k->send_event = False;
      k->display = wc->Dsp;
      k->window = wc->Win;
      k->root = DefaultRootWindow ( wc->Dsp ) ;
      k->subwindow = wc->Win;
      k->state = state;
      code = Revscan_code [ ch ] ;
      if ( code < 0 ) {
          fprintf ( stderr , "code < 0 %c %x\n" , ch , ch ) ;
          return 0;
      }
      {
          k->keycode = code;
          k->same_screen = 1;
          k->type = KeyPress;
          k->state = state;
          status = XSendEvent ( wc->Dsp , wc->Win , True , 0 , e ) ;
          k->type = KeyRelease;
          k->state = state;
          status = XSendEvent ( wc->Dsp , wc->Win , True , 0 , e ) ;
//    fprintf(stderr,"Send Key: %c %x\n",ch,ch);
      }
      free ( e ) ;
      return status;
  }
  int kgSendTabKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , TabKey ) ;
  }
  int kgSendSpaceKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , ' ' ) ;
  }
  int kgSendDeleteKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , DeleteKey ) ;
  }
  int kgSendInsertKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , InsertKey ) ;
  }
  int kgSendPageupKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , PageupKey ) ;
  }
  int kgSendPagedownKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , PagedownKey ) ;
  }
  int kgSendEscapeKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , EscapeKey ) ;
  }
  int kgSendClearKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , ClearKey ) ;
//  return kgSendKeyEvent(Tmp,PrintKey);
  }
  int kgSendHomeKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , HomeKey ) ;
  }
  int kgSendEndKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , EndKey ) ;
  }
  int kgSendBackspaceKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , BackspaceKey ) ;
  }
  int kgSendLeftKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , LeftKey ) ;
  }
  int kgSendRightKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , RightKey ) ;
  }
  int kgSendUpKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , UpKey ) ;
  }
  int kgSendDownKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , DownKey ) ;
  }
  int kgSendEnterKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , LinefeedKey ) ;
  }
  int kgSendLinefeedKeyEvent ( void *Tmp ) {
      return kgSendKeyEvent ( Tmp , LinefeedKey ) ;
  }
  int kgSendControlKeyEvent ( void *Tmp , int ch ) {
      int code;
      if ( ch >= 'a' ) ch = ( ch-'a'+'A' ) ;
      if ( ( ch < 'A' ) || ( ch > '_' ) ) return 0;
      XEvent *e;
      XKeyEvent *k;
      DIALOG *D;
      kgWC *wc;
      e = ( XEvent * ) Malloc ( sizeof ( XEvent ) ) ;
      k = ( XKeyEvent * ) e;
      D = ( DIALOG * ) Tmp;
      wc = WC ( D ) ;
      int status;
      k->state = ControlMask;
      k->type = KeyRelease;
      k->send_event = 1;
      k->display = wc->Dsp;
      k->window = wc->Win;
      k->root = DefaultRootWindow ( wc->Dsp ) ;
      k->subwindow = wc->Win;
      k->state = KeyRelease;
      code = Revscan_code [ ch ] ;
      if ( code < 0 ) {
          fprintf ( stderr , "code < 0 %c %x\n" , ch , ch ) ;
          return 0;
      }
      {
          k->keycode = code;
          k->same_screen = 1;
          k->type = KeyPress;
          status = XSendEvent ( wc->Dsp , wc->Win , False , 0 , e ) ;
          k->type = KeyRelease;
          status = XSendEvent ( wc->Dsp , wc->Win , False , 0 , e ) ;
      }
      free ( e ) ;
  }
  int uiCheckChildern ( void *Tmp , Window Win , void *wtmp ) {
/*
  Checks Window exists or not
*/
      int ret = 0;
      DIALOG *D;
      kgWC *wc;
      Window win , mywin , root;
      int nChild , i;
      XWindowAttributes xwa;
      D = ( DIALOG * ) Tmp;
      wc = WC ( D ) ;
      mywin = wc->Win;
      win = * ( ( Window * ) wtmp ) ;
      Window Root , Par , *Child;
//  XQueryTree(Dsp,Win,&Root,&Par,&Child,&nChild);
//     printf("Win:par,Root,Par: %d %d %d %d,%d\n",Win,par,Root,Par,nChild);
#if 1
      if ( ! XQueryTree ( wc->Dsp , Win , & Root , & Par , & Child , & nChild ) ) {
          fprintf ( stderr , "XQueryTree Failed\n" ) ;
          return 0;
      }
//  fprintf(stderr,"nChild = %d\n",nChild);
      for ( i = 0;i < nChild;i++ ) {
          if ( win == ( Child [ i ] ) ) {
//       fprintf(stderr,"Got Window\n");
              ret = 1;break;
          }
          else {
              if ( uiCheckChildern ( Tmp , Child [ i ] , wtmp ) ) {
//       fprintf(stderr,"Got from Children\n");
                  ret = 1;break;
              }
          }
      }
      if ( nChild != 0 ) XFree ( Child ) ;
#else
      if ( XGetWindowAttributes ( wc->Dsp , win , & xwa ) ) ret = 1;
#endif
      return ret;
  }
  int kgCheckWindow ( void *Tmp , void *wtmp ) {
/*
  Checks Window exists or not
*/
      int ret = 0;
      DIALOG *D;
      kgWC *wc;
      Window win , mywin , root;
      int nChild , i;
      XWindowAttributes xwa;
      D = ( DIALOG * ) Tmp;
      wc = WC ( D ) ;
      mywin = wc->Win;
      win = * ( ( Window * ) wtmp ) ;
      Window Root , Par , *Child;
//  XQueryTree(Dsp,Win,&Root,&Par,&Child,&nChild);
//     printf("Win:par,Root,Par: %d %d %d %d,%d\n",Win,par,Root,Par,nChild);
#if 1
      if ( ! XQueryTree ( wc->Dsp , DefaultRootWindow ( wc->Dsp ) , \
           & Root , & Par , & Child , & nChild ) ) \
      {
          fprintf ( stderr , "XQueryTree Failed\n" ) ;
          return 0;
      }
//  fprintf(stderr,"nChild = %d\n",nChild);
      for ( i = 0;i < nChild;i++ ) {
          if ( win == ( Child [ i ] ) ) {
//       fprintf(stderr,"Got Window\n");
              ret = 1;break;
          }
          else {
              if ( uiCheckChildern ( Tmp , Child [ i ] , wtmp ) ) {
//       fprintf(stderr,"Got from Children\n");
                  ret = 1;break;
              }
          }
      }
      if ( nChild != 0 ) XFree ( Child ) ;
#else
      if ( XGetWindowAttributes ( wc->Dsp , win , & xwa ) ) ret = 1;
#endif
      return ret;
  }
  int kgSendKeyToWindow ( void *Tmp , void *wtmp , int ch ) {
      int code;
      XEvent *e;
      XKeyEvent *k;
      DIALOG *D;
      kgWC *wc;
      Window win , mywin , root;
      D = ( DIALOG * ) Tmp;
      wc = WC ( D ) ;
      mywin = wc->Win;
      win = * ( ( Window * ) wtmp ) ;
      if ( ! kgCheckWindow ( Tmp , wtmp ) ) return 0;
      kgSetInputFocus ( Tmp , wtmp ) ; // Can be used
      e = ( XEvent * ) Malloc ( sizeof ( XEvent ) ) ;
      k = ( XKeyEvent * ) e;
      int status;
      int state = 0;
      k->type = KeyRelease;
      k->send_event = True;
      k->send_event = False;
      k->display = wc->Dsp;
      k->window = root;
      k->window = win;
      k->root = DefaultRootWindow ( wc->Dsp ) ;
      k->subwindow = mywin;
      k->state = state;
      code = Revscan_code [ ch ] ;
  //TCB
//     fprintf(stderr,"code : %c %x\n",ch,ch);
      if ( code < 0 ) {
          fprintf ( stderr , "code < 0 %c %x\n" , ch , ch ) ;
          return 0;
      }
      {
          k->keycode = code;
          k->same_screen = 1;
          k->type = KeyPress;
          k->state = state;
//    status= XSendEvent(wc->Dsp,win,False,KeyPress|KeyRelease,e);
//    status= XSendEvent(wc->Dsp,win,False,KeyPress|KeyRelease,e);
          status = XSendEvent ( wc->Dsp , win , True , 0 , e ) ;
          k->type = KeyRelease;
          k->state = state;
//    status= XSendEvent(wc->Dsp,win,False,KeyPress|KeyRelease,e);
          status = XSendEvent ( wc->Dsp , win , True , 0 , e ) ;
      }
      free ( e ) ;
      return status;
  }
  int kgSendTabKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , TabKey ) ;
  }
  int kgSendSpaceKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , ' ' ) ;
  }
  int kgSendDeleteKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , DeleteKey ) ;
  }
  int kgSendInsertKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , InsertKey ) ;
  }
  int kgSendPageupKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , PageupKey ) ;
  }
  int kgSendPagedownKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , PagedownKey ) ;
  }
  int kgSendEscapeKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , EscapeKey ) ;
  }
  int kgSendClearKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , ClearKey ) ;
//  return kgSendKeyToWindow(Tmp,win,PrintKey);
  }
  int kgSendHomeKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , HomeKey ) ;
  }
  int kgSendEndKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , EndKey ) ;
  }
  int kgSendBackspaceKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , BackspaceKey ) ;
  }
  int kgSendLeftKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , LeftKey ) ;
  }
  int kgSendRightKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , RightKey ) ;
  }
  int kgSendUpKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , UpKey ) ;
  }
  int kgSendDownKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , DownKey ) ;
  }
  int kgSendEnterKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , ReturnKey ) ;
  }
  int kgSendLinefeedKeyToWindow ( void *Tmp , void *win ) {
      return kgSendKeyToWindow ( Tmp , win , LinefeedKey ) ;
  }
  int kgSendControlKeyToWindow ( void *Tmp , void *wtmp , int ch ) {
      int code;
      if ( ch >= 'a' ) ch = ( ch-'a'+'A' ) ;
      if ( ( ch < 'A' ) || ( ch > '_' ) ) {
          fprintf ( stderr , "Wrong Control char: %c:%d\n" , ch , ch ) ;
          return 0;
      }
      XEvent *e;
      XKeyEvent *k;
      DIALOG *D;
      kgWC *wc;
      Window win , mywin , root;
      D = ( DIALOG * ) Tmp;
      wc = WC ( D ) ;
      mywin = wc->Win;
      win = * ( ( Window * ) wtmp ) ;
      if ( ! kgCheckWindow ( Tmp , wtmp ) ) return 0;
      kgSetInputFocus ( Tmp , wtmp ) ;
      e = ( XEvent * ) Malloc ( sizeof ( XEvent ) ) ;
      k = ( XKeyEvent * ) e;
      int status;
      int state = ControlMask;
      k->type = KeyRelease;
      k->send_event = True;
      k->display = wc->Dsp;
      k->window = root;
      k->window = win;
      k->root = DefaultRootWindow ( wc->Dsp ) ;
      k->subwindow = mywin;
      k->state = state;
      code = Revscan_code [ ch ] ;
      k->same_screen = 1;
      k->type = KeyPress;
      k->state = state;
      k->keycode = code;
//    status= XSendEvent(wc->Dsp,win,False,KeyPress|KeyRelease,e);
      status = XSendEvent ( wc->Dsp , win , True , 0 , e ) ;
      k->type = KeyRelease;
//    status= XSendEvent(wc->Dsp,win,False,KeyPress|KeyRelease,e);
      status = XSendEvent ( wc->Dsp , win , True , 0 , e ) ;
      free ( e ) ;
      return status;
  }
  void kgPutBackEvent ( void *tmp , XEvent *event ) {
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) tmp;
      wc = WC ( D ) ;
      XPutBackEvent ( wc->Dsp , event ) ;
      return ;
  }
  void kgMoveWindow ( void *tmp , int x , int y ) {
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) tmp;
      wc = WC ( D ) ;
      XMoveWindow ( wc->Dsp , wc->Win , x , y ) ;
      return ;
  }
  void kgResizeWindow ( void *tmp , int l , int w ) {
      kgWC *wc;
      DIALOG *D;
      DIA *d;
      DIG *G;
      int EVGAX , EVGAY;
      int i;
      unsigned int Dpth , Offset = 5;
      Window Win;
      float transparency;
      Display *Dsp;
      D = ( DIALOG * ) tmp;
      wc = WC ( D ) ;
      d = D->d;
      Dsp = wc->Dsp;
      Win = wc->Win;
      transparency = D->transparency;
      EVGAX = l;
      EVGAY = w;
      Dpth = wc->Dpth;
      XResizeWindow ( wc->Dsp , wc->Win , l , w ) ;
      _uiTileImage ( wc , wc->Pix , wc->IMAGEBAK , l , w ) ;
      _uiTileImage ( wc , wc->Piximg , wc->IMAGEBAK , l , w ) ;
      wc->EVGAX = EVGAX;
      wc->EVGAY = EVGAY;
      D->evgax = EVGAX;
      D->evgay = EVGAY;
      uiset_sup_clip_limits ( wc , 0 , 0 , wc->EVGAX , wc->EVGAY ) ;
      uiset_clip_limits ( wc , 0 , 0 , wc->EVGAX , wc->EVGAY ) ;
      return ;
  }
  int _uiAdjustResize ( void *tmp , int l , int w ) {
      kgWC *wc;
      DIALOG *D;
      DIA *d;
      DIG *G;
      int EVGAX , EVGAY;
      int i;
      unsigned int Dpth , Offset = 5;
      Window Win;
      Display *Dsp;
      float transparency;
      float x1 , y1 , x2 , y2;
      D = ( DIALOG * ) tmp;
      d = D->d;
      wc = WC ( D ) ;
      Dsp = wc->Dsp;
      Win = wc->Win;
      transparency = D->transparency;
      EVGAX = l;
      EVGAY = w;
      Dpth = wc->Dpth;
//  printf("Adjust: %d %d %d %d\n",l,w,D->xl,D->yl);
      if ( ( abs ( wc->EVGAX - ( l ) ) > Offset ) \
       || ( abs ( wc->EVGAY- ( w ) ) > Offset ) ) {
          _uiTileImage ( wc , wc->Pix , wc->IMAGEBAK , l , w ) ;
          _uiTileImage ( wc , wc->Piximg , wc->IMAGEBAK , l , w ) ;
          wc->EVGAX = EVGAX;
          wc->EVGAY = EVGAY;
          D->evgax = EVGAX;
          D->evgay = EVGAY;
          uiset_sup_clip_limits ( wc , 0 , 0 , wc->EVGAX , wc->EVGAY ) ;
          uiset_clip_limits ( wc , 0 , 0 , wc->EVGAX , wc->EVGAY ) ;
          i = 0;
          while ( d [ i ] .t != NULL ) {
              if ( d [ i ] .t->code == 'g' ) {
                  G = d [ i ] .g;
                  kgGetViewport ( G , & x1 , & y1 , & x2 , & y2 ) ;
                  kgViewport ( G , x1 , y1 , x2 , y2 ) ;
                  kgGetWindow ( G , & x1 , & y1 , & x2 , & y2 ) ;
                  kgUserFrame ( G , x1 , y1 , x2 , y2 ) ;
              }
              i++;
          }
//    printf("Adjust: %d %d %d %d\n",l,w,D->xl,D->yl);
          return 1;
      }
      else return 0 ;
  }
  void kgDropFocus ( void *tmp ) {
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) tmp;
      wc = WC ( D ) ;
      XSetInputFocus ( wc->Dsp , PointerRoot , RevertToPointerRoot , CurrentTime ) ;
      return ;
  }
  void * kgGetInputFocus ( void *Tmp ) {
      static Window *owin = NULL;
      static Window *mywin = NULL;
      void *tpt;
      Window Cw;
      DIALOG *D;
      Display *Dsp;
      int ret;
      if ( owin == NULL ) owin = ( Window * ) Malloc ( sizeof ( Window ) ) ;
      if ( mywin == NULL ) mywin = ( Window * ) Malloc ( sizeof ( Window ) ) ;
      if ( Tmp != NULL ) {
          D = ( DIALOG * ) Tmp;
          Dsp = ( Display * ) WC ( D )->Dsp;
      }
      else {
          Dsp = XOpenDisplay ( NULL ) ;
          if ( Dsp == NULL ) {
              setenv ( "DISPLAY" , ":0" , 1 ) ;
              Dsp = XOpenDisplay ( NULL ) ;
          }
          if ( Dsp == NULL ) {
              fprintf ( stderr , "Failed to Open Display\n" ) ;
              return NULL;
          }
      }
      XGetInputFocus ( Dsp , & Cw , & ret ) ;
      if ( Tmp == NULL ) {
          XCloseDisplay ( Dsp ) ;
          memcpy ( owin , & Cw , sizeof ( Window ) ) ;
          return owin;
      }
      if ( Cw == ( Window ) ( WC ( D )->Win ) ) {
          memcpy ( mywin , & Cw , sizeof ( Window ) ) ;
          return mywin;
      }
      else {
          memcpy ( owin , & Cw , sizeof ( Window ) ) ;
          return owin;
      }
  }
  int kgSetInputFocus ( void *Tmp , void *wtmp ) {
      Window *win;
      int ret;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      win = ( Window * ) wtmp;
      XSetInputFocus ( ( Display * ) WC ( D )->Dsp , *win , \
           RevertToPointerRoot , CurrentTime ) ;
      return 1;
  }
  int kgCheckMyWindow ( void *Tmp , void *wtmp ) {
/* Check a given window is my window */
      Window *win;
      int ret;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      win = ( Window * ) wtmp;
      if ( *win == ( Window ) ( WC ( D )->Win ) ) return 1;
      else return 0;
  }
  int kg_image_bitmap ( Display *Dsp , GC Gc , Pixmap bmp , \
       void *tmp , int width , int height ) \
  {
      float transparency = 0 , highfac = 1.0;
      int x0 = 0 , y0 = 0;
      int i , j , k , m , row , color , sx , sy , dx , dy , xoffset , yoffset , \
           xm , ym , xl , yl , xu , yu , xc , yc , xdl , xdu , ydl , ydu;
      unsigned long val;
      unsigned char *Imgdata = NULL , *dest;
      unsigned char r , g , b;
      unsigned int a;
      unsigned long lastrow = 0;
      unsigned long pixel;
      unsigned int red , green , blue;
      unsigned char bg_red = 0xff , bg_green = 0xff , bg_blue = 0xff;
      double f , f1;
      int EVGAX , EVGAY;
      PixelPacket *pixels , *src;
      GMIMG *png , *pngrz = NULL;
      int ximage_rowbytes;
      int w , h;
      char *cpt;
      int imgFile = 0;
      if ( tmp == NULL ) return 0;
      EVGAX = width+1;
      EVGAY = height+1;
      cpt = ( char * ) tmp;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) {
          png = ( GMIMG * ) _uiGetFileImage ( ( char * ) ( cpt+2 ) ) ;
          if ( png == NULL ) {
              fprintf ( stderr , "Failed to get %s\n" , ( char * ) ( cpt+2 ) ) ;
              return 0;
          }
          imgFile = 1;
//         else printf("Got Image\n");
      }
      else{
          FMGIMG *img;
          png = ( GMIMG * ) tmp;
          if ( png == NULL ) return 0;
          if ( strcmp ( png->Sign , "IMG" ) != 0 ) {
              img = ( FMGIMG * ) tmp;
              png = ( GMIMG * ) kgGetInlineImage ( img->image_data , img->size ) ;
              imgFile = 1;
          }
      }
      xoffset = png->xoffset;
      yoffset = png->yoffset;
      xc = x0+width/2;
      yc = y0+height/2;
//    printf("Calling gm_image :%s\n",png->Sign);
      pixels = ( PixelPacket * ) uiPixelsgmImage ( png ) ;
      w = png->image_width;
      h = png->image_height;
      xm = ( w-xoffset ) /2+xoffset;
      ym = ( h-yoffset ) /2+yoffset;
      xl = xm -width/2; xu = xm+width-width/2;
      yl = ym -height/2; yu = ym+height -height/2;
      if ( ( xu < 0 ) || ( xl > w ) || ( yu < 0 ) || ( yl > h ) ) {
          if ( imgFile ) uiFreeImage ( png ) ;
          return 0;
      }
// There is something to display
      if ( xl < 0 ) xl = 0;
      if ( xu > w ) xu = w;
      if ( yl < 0 ) yl = 0;
      if ( yu > h ) yu = h;
      dx = xm - xc;
      dy = ym - yc;
      xdl = xl-dx;
      xdu = xu-dx;
      ydl = yl -dy;
      ydu = yu -dy;
      row = -1;
      for ( i = 0; i < h; ++i ) {
          if ( i < ( yl ) ) continue;
          if ( i >= yu ) break;
          src = pixels + i*w;
          row++;
          m = 0;
          k = -1;
          if ( png->image_channels == 3 ) {
              for ( j = 0;j < w; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src++;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  red = src->red;
                  green = src->green;
                  blue = src->blue;
                  src++;
                  if ( ( blue != 0 ) || ( green != 0 ) || ( blue != 0 ) ) XDrawPoint \
                   ( Dsp , bmp , Gc , m , row ) ;
                  m++;
              }
              } else /* if ( image_channels == 4 ) */ {
              for ( j = 0;j < w; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src++;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  red = src->red;
                  green = src->green;
                  blue = src->blue;
                  a = 255 - src->opacity;
                  src++;
                  if ( a == 0 ) { m++; continue; }
                  if ( ( blue != 0 ) || ( green != 0 ) || ( blue != 0 ) ) XDrawPoint \
                   ( Dsp , bmp , Gc , m , row ) ;
                  m++;
              }
          }
      }
      if ( imgFile ) uiFreeImage ( png ) ;
      return 1;
  }
  Window kgGetUiWindow ( void *D ) {
      return WC ( D )->Win;
  }
  Display *kgGetUiDisplay ( void *D ) {
      return WC ( D )->Dsp;
  }
  void kgSubWindow ( void *Gtmp ) {
      DIALOG *D;
      DIG *G;
      int xpos , ypos , xres , yres;
      char *title;
      kgWC *wc;
      int i;
      Display *Dsp;
      Visual *Vis;
      unsigned int Dpth = 32;
      Window par , Win , Parent , *sWin;
      XSetWindowAttributes xswa;
      XVisualInfo visualinfo ;
      int TrClr = 0;
      G = ( DIG * ) Gtmp;
      D = ( DIALOG * ) ( G->D ) ;
      xpos = G->x1; ypos = G->y1; xres = ( G->x2-G->x1+1 ) ;
      yres = ( G->y2-G->y1+1 ) ;
      title = D->name;
      wc = D->wc;
      Dsp = wc->Dsp;
      if ( XMatchVisualInfo ( Dsp , DefaultScreen ( Dsp ) , \
           32 , TrueColor , & visualinfo ) == 0 ) \
      {
          printf ( "Could not find a True Color Match\n" ) ;
          Vis = XDefaultVisual ( Dsp , DefaultScreen ( Dsp ) ) ;
          Dpth = DisplayPlanes ( Dsp , DefaultScreen ( Dsp ) ) ;
          TrClr = 0;
      }
      else {
          TrClr = 1;
          Vis = visualinfo.visual;
          Dpth = visualinfo.depth;
      }
      Parent = WC ( D )->Win;
      xswa.colormap = wc->Cmap;
      xswa.bit_gravity = NorthWestGravity;
      xswa.background_pixel = 0xffffffff;
      xswa.background_pixel = 0xff000000;
      xswa.background_pixel = 0x00000000;
      xswa.save_under = 1;
      xswa.backing_store = Always;
      xswa.cursor = None;
      xswa.background_pixmap = None;
      xswa.border_pixel = 0;
      xswa.override_redirect = False;
      Win = XCreateWindow ( Dsp , Parent , xpos , ypos , \
           xres , yres , 0 , Dpth , InputOutput , Vis , \
       ( CWBackPixel | CWBackingStore | CWSaveUnder \
       | CWColormap | CWBitGravity ) , & xswa ) ;
      sWin = ( void * ) Malloc ( sizeof ( Window ) ) ;
      fprintf ( stderr , "glWindow= %ld\n" , Win ) ;
      *sWin = Win;
      G->glWindow = ( void * ) sWin;
      XMapWindow ( Dsp , Win ) ;
      return ;
  }
  static void vo_hidecursor ( Display * disp , Window win ) {
      Cursor no_ptr;
      Pixmap bm_no;
      XColor black , dummy;
      Colormap colormap;
      static char bm_no_data [ ] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
      colormap = DefaultColormap ( disp , DefaultScreen ( disp ) ) ;
      if ( ! XAllocNamedColor ( disp , colormap , "black" , & black , & dummy ) ) {
          return; // color alloc failed , give up
      }
      bm_no = XCreateBitmapFromData ( disp , win , bm_no_data , 8 , 8 ) ;
      no_ptr = XCreatePixmapCursor ( disp , bm_no , \
           bm_no , & black , & black , 0 , 0 ) ;
      XDefineCursor ( disp , win , no_ptr ) ;
      XFreeCursor ( disp , no_ptr ) ;
      if ( bm_no != None ) XFreePixmap ( disp , bm_no ) ;
      XFreeColors ( disp , colormap , & black.pixel , 1 , 0 ) ;
  }
  void kgUnmapWindow ( void *Tmp ) {
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      XUnmapWindow ( WC ( D )->Dsp , WC ( D )->Win ) ;
      return;
  }
  void kgMapWindow ( void *Tmp ) {
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      XMapWindow ( WC ( D )->Dsp , WC ( D )->Win ) ;
      return;
  }
  void *kgCreateSubWindow ( void *Tmp ) {
      DIALOG *D , *Parent;
      int xpos , ypos , xres , yres;
      char *title;int dec;float transparency;int sticky;int fixpos;int NoTaskBar;
      int StackPos;void *ShapeXpm;
      int Resize;
      kgWC *wc , *PARWC;
      static int ENTRY = 1;
      static int argc = 1;
      static char *argv [ 1 ] = {"Testing"};
      int i;
      Display *Dsp;
      Colormap Cmap;
      Visual *Vis;
      unsigned char data [ 5 ] = {0 , 0 , 0 , 0 , 0};
      unsigned int Dpth = 32;
      Dlink *SBlist;
      Window par , Win , parwin;
      Pixmap CurPix , CurMask;
      GC Gc , ShapeGc;
      XGCValues gcv;
      XEvent event;
      XSizeHints xsh;
      XSetWindowAttributes xswa;
      XWindowAttributes xwa;
      unsigned int fg , bg , bd;
/* Added as on 18/11/12/ */
      XWMHints xwmh , *xwmpt;
      XVisualInfo visualinfo ;
      Atom property , args [ 4 ] ;
      XClientMessageEvent cEvent;
      Cursor Cur , Curo , HideCur;
      int TrClr = 0;
      int EVGAX , EVGAY;
      int event_base , error_base;
      int xresmax , yresmax;
      static char bm_no_data [ ] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
      XRectangle Rect;
      const char *dspname;
      typedef struct {
          unsigned long flags;
          unsigned long functions;
          unsigned long decorations;
          long inputMode;
          unsigned long status;
      } Hints;
      Hints hints;
      XColor c15 , c0;
      kgColor *kgcolors;
      D = ( DIALOG * ) Tmp;
      dspname = getenv ( "DISPLAY" ) ;
      if ( ! dspname ) {
          dspname = DISPLAY;
          setenv ( "DISPLAY" , dspname , 1 ) ;
      }
      Parent = ( DIALOG * ) ( D->parent ) ;
      xpos = D->xo; ypos = D->yo; xres = D->xl; yres = D->yl;
      title = D->name;
      transparency = D->transparency;
      dec = D->Deco;
      sticky = D->Sticky;
      fixpos = D->Fixpos;NoTaskBar = D->NoTaskBar;
      StackPos = D->StackPos;ShapeXpm = D->Shapexpm;
      Resize = D->Resize;
      hints.flags = 2; // Specify that we are changing the window decorations.
      hints.decorations = dec; // 0 ( false ) means that window decorations should go bye-bye.
      wc = ( kgWC * ) Malloc ( sizeof ( kgWC ) ) ;
      PARWC = ( kgWC * ) ( Parent->wc ) ;
      D->wc = wc;
//  XInitThreads(); // created problem for True Color
      XInitThreads ( ) ;
      Dsp = PARWC->Dsp;
      if ( Dsp == NULL ) {printf ( " Error: in opening Display ..\n" ) ;
      exit ( 0 ) ;};
      xresmax = DisplayWidth ( Dsp , DefaultScreen ( Dsp ) ) ;
      yresmax = DisplayHeight ( Dsp , DefaultScreen ( Dsp ) ) ;
      if ( XMatchVisualInfo ( Dsp , DefaultScreen ( Dsp ) , \
           32 , TrueColor , & visualinfo ) == 0 ) \
      {
          printf ( "Could not find a True Color Match\n" ) ;
          Vis = XDefaultVisual ( Dsp , DefaultScreen ( Dsp ) ) ;
          Dpth = DisplayPlanes ( Dsp , DefaultScreen ( Dsp ) ) ;
          TrClr = 0;
      }
      else {
          TrClr = 1;
          Vis = visualinfo.visual;
          Dpth = visualinfo.depth;
      }
      get_scan_code ( Dsp ) ;
#if 1
      if ( clrtbl [ 256 ] == -1 ) {
          for ( i = 256;i < NCLRS;i++ ) clrtbl [ i ] = i;
      }
#endif
      EVGAX = xres;
      EVGAY = yres;
      par = RootWindow ( Dsp , DefaultScreen ( Dsp ) ) ;
      parwin = PARWC->Win;
      xsh.flags = ( PPosition | PSize | PMinSize | PMaxSize | PResizeInc ) ;
      xsh.height = EVGAY;
      xsh.width = EVGAX;
      xsh.x = xpos;
      xsh.y = ypos;
      xswa.colormap = XCreateColormap ( Dsp , DefaultRootWindow \
       ( Dsp ) , Vis , AllocNone ) ;
      xswa.bit_gravity = NorthWestGravity;
      xswa.background_pixel = 0xffffffff;
      xswa.background_pixel = 0x00000000;
      xswa.save_under = 1;
//  xswa.backing_store=Always;
      xswa.backing_store = NotUseful;
      xswa.cursor = None;
      xswa.background_pixmap = None;
      xswa.border_pixel = 0;
      xswa.override_redirect = False;
/* As on 18/11/12 */
      xwmpt = XAllocWMHints ( ) ;
      xwmpt->flags = AllHints;
      xwmpt->initial_state = NormalState;
      xwmpt->input = True;
      property = XInternAtom ( Dsp , "_MOTIF_WM_HINTS" , True ) ;
      Win = XCreateWindow ( Dsp , parwin , xsh.x , xsh.y , xsh.width , \
           xsh.height , 0 , Dpth , InputOutput , Vis , \
       ( CWColormap | CWBackingStore | CWBorderPixel | CWBitGravity \
       | CWOverrideRedirect ) , & xswa ) ;
      EVGAX = xwa.width;
      EVGAY = xwa.height;
      wc->msg_x = ( EVGAX-316 ) /2;
      wc->msg_y = EVGAY-24;
/* added on 8-3-01 */
      xsh.flags = ( PPosition | PSize | PMinSize | PMaxSize | PResizeInc ) ;
      xsh.height = EVGAY;
      xsh.width = EVGAX;
      xsh.x = xwa.x;
      xsh.y = xwa.y;
// TCBTCB
      if ( Resize != 1 ) {
          xsh.min_width = xsh.width;
          xsh.min_height = xsh.height;
          xsh.max_width = xsh.width;
          xsh.max_height = xsh.height;
          xsh.width_inc = xsh.height_inc = 0;
      }
      else {
          xsh.min_width = D->MinWidth;
          xsh.min_height = D->MinHeight;
          xsh.max_width = xresmax;
          xsh.max_height = yresmax;
          xsh.width_inc = xsh.height_inc = 0;
      }
/* As on 18/11/12  for no decorations */ 
      XSetStandardProperties ( Dsp , Win , title , \
           title , None , argv , argc , & xsh ) ;
      if ( TrClr ) {
          XChangeProperty ( Dsp , Win , property , property , 32 , PropModeReplace , \
           ( unsigned char * ) & hints , 5 ) ;
      }
      XSetWMHints ( Dsp , Win , xwmpt ) ;
      XFree ( xwmpt ) ;
      Cmap = xswa.colormap;
      uiset_def_colors ( Dsp , Cmap ) ;
      xswa.colormap = Cmap;
      XChangeWindowAttributes ( Dsp , Win , ( CWColormap ) , & xswa ) ;
//       (CWBackPixmap|CWBackPixel|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
//       (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
//     (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity|CWOverrideRedirect|CWCursor),&xswa);
      XGetWindowAttributes ( Dsp , Win , & xwa ) ;
      if ( xwa.backing_store == Always ) {
          BackStore = 1;
      }
//  if(!XCompositeQueryExtension(Dsp,&event_base,&error_base)) {
      {
//    printf("NO COMPOSITE EXTENSION\n");
          if ( ShapeXpm != NULL ) {
              CurMask = XCPix1 ( 0 , 0 , EVGAX , EVGAY ) ;
              ShapeGc = XCreateGC ( Dsp , CurMask , ( GCForeground \
               | GCBackground ) , & gcv ) ;
              XSetBackground ( Dsp , ShapeGc , 0 ) ;
              XSetForeground ( Dsp , ShapeGc , 0 ) ;
              XFillRectangle ( Dsp , CurMask , ShapeGc , 0 , 0 , EVGAX , EVGAY ) ;
              XSetForeground ( Dsp , ShapeGc , 1 ) ;
              kg_image_bitmap ( Dsp , ShapeGc , CurMask , ShapeXpm , EVGAX , EVGAY ) ;
              XShapeCombineMask ( Dsp , Win , ShapeBounding , 0 , 0 , CurMask , ShapeSet ) ;
              XFreePixmap ( Dsp , CurMask ) ;
              XFreeGC ( Dsp , ShapeGc ) ;
          }
          else {
              Rect.x = 0;
              Rect.y = 0;
              Rect.width = EVGAX;
              Rect.height = EVGAY;
              if ( Resize != 1 ) XShapeCombineRectangles ( Dsp , Win , \
                   ShapeClip , 0 , 0 , & Rect , 1 , ShapeSet , 1 ) ;
          }
      }
      gcv.foreground = 10;
      gcv.background = 0;
      gcv.line_width = 0;
      wc->Gc = XCreateGC ( Dsp , Win , ( GCForeground | GCBackground ) , & gcv ) ;
      Gc = wc->Gc;
      uiDefaultDisplayFontSize ( Dsp , Gc ) ;
      c15.pixel = 15;
      c0.pixel = 0;
#if 0
      CurPix = XCreateBitmapFromData ( Dsp , win , bm_no_data , 8 , 8 ) ;
      wc->HideCur = XCreatePixmapCursor ( Dsp , CurPix , \
           CurPix , & c0 , & c0 , 0 , 0 ) ;
#else
      CurPix = XCPix1 ( 0 , 0 , 0 , 0 ) ;
      CurMask = XCPix1 ( 0 , 0 , 0 , 0 ) ;
//  wc->HideCur =  XCreatePixmapCursor(Dsp,CurPix,CurMask,&c15,&c0,1,1);
      wc->HideCur = XCreatePixmapCursor ( Dsp , CurPix , \
           CurMask , & c0 , & c0 , 0 , 0 ) ;
#endif
      XFreePixmap ( Dsp , CurPix ) ;
      XFreePixmap ( Dsp , CurMask ) ;
      wc->Pix = XCPix ( 0 , 0 , xsh.max_width , xsh.max_height ) ;
      wc->Piximg = XCPix ( 0 , 0 , xsh.max_width , xsh.max_height ) ;
      wc->EVGAX = EVGAX;
      wc->EVGAY = EVGAY;
      wc->Dsp = Dsp;
      wc->Dpth = Dpth;
      wc->Win = Win;
      wc->ActWin = wc->Pix;
      wc->DspWin = wc->Pix;
      wc->Cmap = Cmap;
      wc->Gc = Gc;
// One can use it; if needed
//  XFillRectangle(Dsp,Pix,Gc,0,0,EVGAX,EVGAY);
//  wcset_clr(wc,0);
//  Curo= XCreateFontCursor(Dsp,59);
//  Curo= XCreateFontCursor(Dsp,2);
      Curo = XCreateFontCursor ( Dsp , 94 ) ;
      Cur = XCreateFontCursor ( Dsp , 35 ) ;
      XMapWindow ( Dsp , Win ) ;
      XDefineCursor ( Dsp , Win , Curo ) ;
      wc->CurrCur = Curo;
      XSelectInput ( Dsp , Win , EventMask ) ;
#if 0
      XNextEvent ( Dsp , & event ) ;
      cEvent.type = ClientMessage;
      cEvent.window = Win;
      cEvent.format = 32;
      cEvent.data.l [ 0 ] = 0xFFFFFFFF;
      cEvent.data.l [ 1 ] = 1;
      cEvent.data.l [ 2 ] = 0;
      cEvent.data.l [ 3 ] = 0;
      cEvent.data.l [ 4 ] = 0;
      cEvent.message_type = XInternAtom ( Dsp , "_NET_WM_DESKTOP" , True ) ;
//TCB
      if ( sticky ) wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
      cEvent.message_type = XInternAtom ( Dsp , "_NET_WM_ALLOWED_ACTIONS" , True ) ;
      cEvent.data.l [ 0 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_STICK" , True ) ;
      cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_ABOVE" , True ) ;
//    cEvent.data.l[1]= XInternAtom(Dsp,"_NET_WM_ACTION_STAYS_ON_TOP",True);
      cEvent.data.l [ 2 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_BELOW" , True ) ;
      cEvent.data.l [ 3 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_CHANGE_DESKTOP" , \
           True ) ;
      cEvent.data.l [ 4 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_MOVE" , True ) ;
      wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
      cEvent.data.l [ 0 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_STAYS_ON_TOP" , \
           True ) ;
      cEvent.data.l [ 1 ] = 0;
      cEvent.data.l [ 2 ] = 0;
      cEvent.data.l [ 3 ] = 0;
      cEvent.data.l [ 4 ] = 0;
      wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
      cEvent.message_type = XInternAtom ( Dsp , "_NET_WM_STATE" , True ) ;
      cEvent.data.l [ 0 ] = 1;
      cEvent.data.l [ 3 ] = 1;
      cEvent.data.l [ 4 ] = 0;
//    if(dec==0) {
      if ( NoTaskBar == 1 ) {
          cEvent.data.l [ 1 ] = XInternAtom ( Dsp , \
               "_NET_WM_STATE_SKIP_PAGER" , True ) ;
          cEvent.data.l [ 2 ] = XInternAtom ( Dsp , "_NET_WM_STATE_SKIP_TASKBAR" , \
               True ) ;
          wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
      }
      if ( fixpos == 1 ) {
          cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_STATE_STICKY" , True ) ;
          cEvent.data.l [ 2 ] = 0;
          wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
      }
#endif
#if 0
      switch ( StackPos ) {
          case 1:
          cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_STATE_STAYS_ON_TOP" , \
               True ) ;
          cEvent.data.l [ 2 ] = 0;
          wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
          cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_STATE_ABOVE" , True ) ;
          wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
          break;
          case -1:
          cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_STATE_BELOW" , True ) ;
          cEvent.data.l [ 2 ] = 0;
          wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
          break;
          default:
          break;
      }
#endif
      wc->IMAGE = wc->IMAGEBAK = NULL;
#if 1
//  wc->IMAGE = XGetImage(Dsp,Win,0,0,xsh.min_width,xsh.min_height,0xffffffff,ZPixmap);
//  wc->IMAGEBAK = XGetImage(Dsp,Win,0,0,xsh.min_width,xsh.min_height,0xffffffff,ZPixmap);
      wc->IMAGE = XGetImage ( Dsp , Win , 0 , 0 , 20 , 20 , 0xffffffff , ZPixmap ) ;
      wc->IMAGEBAK = XGetImage ( Dsp , Win , 0 , \
           0 , 20 , 20 , 0xffffffff , ZPixmap ) ;
      uiMakeImageTransparent ( wc->IMAGE , transparency ) ;
      uiMakeImageTransparent ( wc->IMAGEBAK , transparency ) ;
#if 0
      XPutImage ( Dsp , wc->Pix , Gc , wc->IMAGE , 0 , 0 , 0 , 0 , EVGAX , EVGAY ) ;
      XPutImage ( Dsp , wc->Piximg , Gc , wc->IMAGE , \
           0 , 0 , 0 , 0 , EVGAX , EVGAY ) ;
#else
      _uiTileImage ( wc , wc->Pix , wc->IMAGE , xresmax , yresmax ) ;
      _uiTileImage ( wc , wc->Piximg , wc->IMAGE , xresmax , yresmax ) ;
#endif
      uiMakeImageTransparent ( wc->IMAGE , 0.0 ) ;
      wc->REDMASKPOS = GetNoOfBits ( wc->IMAGE->red_mask ) ;
      wc->GREENMASKPOS = GetNoOfBits ( wc->IMAGE->green_mask ) ;
      wc->BLUEMASKPOS = GetNoOfBits ( wc->IMAGE->blue_mask ) ;
#endif
//TCB
      XRaiseWindow ( Dsp , Win ) ;
      wc->DefGuiFontEntry = 0;
// Needs Modification;
      pthread_mutex_init ( & ( wc->Dsplock ) , NULL ) ;
      wc->EVGAX = EVGAX;
      wc->EVGAY = EVGAY;
      wc->Dsp = Dsp;
      wc->Dpth = Dpth;
      wc->Win = Win;
      wc->Gc = Gc;
      wc->FullScreen = 0;
      wc->c_color = 0;
      wc->Clip = Dopen ( ) ;
      wc->SBlist = Dopen ( ) ;
      wc->TLIST = Dopen ( ) ;
      wc->ExposeWin = 0;
      wc->kgcolors = ( kgColor * ) Malloc ( sizeof ( kgColor ) *1024 ) ;
      wc->GuiFont = 23;
      wc->GuiFontSize = 8;
      kgcolors = ( kgColor * ) wc->kgcolors;
      for ( i = 0;i < 256;i++ ) {
          kgcolors [ i ] .red = kgIcode [ i ] [ 0 ] ;
          kgcolors [ i ] .green = kgIcode [ i ] [ 1 ] ;
          kgcolors [ i ] .blue = kgIcode [ i ] [ 2 ] ;
          kgcolors [ i ] .Col = DefClrs [ i ] ;
          kgcolors [ i ] .clrtbl = clrtbl [ i ] ;
      }
      for ( i = 256;i < NCLRS;i++ ) {
          kgcolors [ i ] .red = kgIcode [ i ] [ 0 ] ;
          kgcolors [ i ] .green = kgIcode [ i ] [ 1 ] ;
          kgcolors [ i ] .blue = kgIcode [ i ] [ 2 ] ;
          kgcolors [ i ] .Col = DefClrs [ i ] ;
          kgcolors [ i ] .clrtbl = clrtbl [ i ] ;
      }
      wcset_clr ( wc , 0 ) ;
      RefreshWindowThread ( wc ) ;
      wc->Rth = 0;
  //kgEnableSelection(D);
      wc->Hlt = 0;
      wc->Pstr = NULL;
      wc->Cstr = NULL;
      wc->cpmenu = NULL;
      wc->cpCallback = NULL;
      return wc;
  }
  void * kgGetGeometry ( void *Tmp , int *xo , int *yo , \
       int *l , int *h , int *borwidth ) \
  {
/* Caller should free RootWindow Pointer (return value) */
/* NOT USEFUL WHEN Windoe Managet is Active */
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) Tmp;
      XWindowAttributes xwa;
      wc = WC ( D ) ;
      Window *Rwin;
      int bwidth , depth;
      Rwin = ( Window * ) Malloc ( sizeof ( Window ) ) ;
//  XGetGeometry(wc->Dsp,wc->Win,Rwin,xo,yo,l,h,borwidth,&depth);
      XGetWindowAttributes ( wc->Dsp , wc->Win , & xwa ) ;
      *xo = xwa.x;
      *yo = xwa.y;
      *l = xwa.width;
      *h = xwa.height;
      *Rwin = xwa.root;
      if ( ( *xo == 0 ) && ( *yo == 0 ) ) {*xo = wc->msg_x;*yo = wc->msg_y;}
      return Rwin;
  }
  int kgCheckCompositor ( void ) {
      int ret = 1;
      int event_base , error_base;
#if 1
      Display *Dsp;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
      Atom cm_atom = XInternAtom ( Dsp , "_NET_WM_CM_S0" , 0 ) ;
//  if(!XCompositeQueryExtension(Dsp,&event_base,&error_base)) {
      if ( XGetSelectionOwner ( Dsp , cm_atom ) == None ) {
          ret = 0;
      }
      XCloseDisplay ( Dsp ) ;
#endif
      return ret;
  }
  char * kgCheckWindowManager ( void ) {
      char * ret = NULL;
      Window wmwin;
      Window win;
      Atom pty , target , prop , sel;
      Atom prop_type;
      int prop_format , count = 0;
      unsigned long prop_size , prop_items;
      unsigned long len;
      unsigned char *txt;
      Display *Dsp;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
      Atom cm_atom = XInternAtom ( Dsp , "_NET_SUPPORTING_WM_CHECK" , 1 ) ;
      if ( cm_atom == None ) {
//     printf("No Window Manager\n");
          return ret;
      }
//	  printf("Window Manager atom  present\n");
      prop = cm_atom;
      win = RootWindow ( Dsp , DefaultScreen ( Dsp ) ) ;
      target = XA_STRING;
      wmwin = ( Window ) XGetWindowProperty ( Dsp , win , prop , 0 , 0 , False , \
           AnyPropertyType , & prop_type , & prop_format , \
           & prop_items , & prop_size , & txt ) ;
//    printf("%ld %ld %ld %s\n",prop_format,prop_items,prop_size,txt);
      if ( prop_type == None ) {
//     printf("No Window Manager\n");
          XFree ( txt ) ;
          txt = NULL;
          return ret;
      }
//    printf("type %d \n",prop_type);
      len = prop_size;
//    printf("len = %d\n",len);
#if 1
      XGetWindowProperty ( Dsp , win , prop , 0 , len , False , prop_type , & prop_type , \
           & prop_format , & prop_items , & prop_size , & txt ) ;
//    printf("%ld %ld %ld %d\n",prop_format,prop_items,prop_size,*((long*)txt));
      wmwin = * ( ( long * ) txt ) ;
      prop = XInternAtom ( Dsp , "_NET_WM_NAME" , 1 ) ;
      if ( prop == None ) {
//     printf("No Window Manager\n");
          return ret;
      }
      XGetWindowProperty ( Dsp , wmwin , prop , 0 , 0 , False , AnyPropertyType , \
           & prop_type , & prop_format , & prop_items , & prop_size , & txt ) ;
//    printf("%ld %ld %ld %s\n",prop_format,prop_items,prop_size,txt);
      if ( prop_type == None ) {
//     printf("No Window Manager\n");
          XFree ( txt ) ;
          txt = NULL;
          return ret;
      }
      len = prop_size;
//    printf("len = %d\n",len);
      XGetWindowProperty ( Dsp , wmwin , prop , 0 , len , False , prop_type , \
           & prop_type , & prop_format , & prop_items , & prop_size , & txt ) ;
//    printf("%ld %ld %ld %s\n",prop_format,prop_items,prop_size,txt);
      ret = ( char * ) malloc ( strlen ( txt ) +1 ) ;
      strcpy ( ret , txt ) ;
      XFree ( txt ) ;
#endif
      XCloseDisplay ( Dsp ) ;
      return ret;
  }
  void *kgCreateWindow ( void *Tmp ) {
      DIALOG *D;
      int xpos , ypos , xres , yres;
      char *title;int dec;float transparency;int sticky;int fixpos;int NoTaskBar;
      const char *dspname;
      int StackPos;void *ShapeXpm;
      int Resize;
      int NoWinMngr = 1;
      kgWC *wc;
      static int ENTRY = 1;
      static int argc = 1;
      static char *argv [ 1 ] = {"Testing"};
      int i;
      Display *Dsp;
      Colormap Cmap;
      Visual *Vis;
      unsigned char data [ 5 ] = {0 , 0 , 0 , 0 , 0};
      unsigned int Dpth = 32;
      char *WinMngr = NULL;
      Dlink *SBlist;
      Window par , Win , Parent;
      Pixmap CurPix , CurMask;
      GC Gc , ShapeGc;
      XGCValues gcv;
      XEvent event;
      XSizeHints xsh;
      XSetWindowAttributes xswa;
      XWindowAttributes xwa;
      unsigned int fg , bg , bd;
/* Added as on 18/11/12/ */
      XWMHints xwmh , *xwmpt;
      XVisualInfo visualinfo ;
      Atom property , args [ 4 ] ;
      XClientMessageEvent cEvent;
      Cursor Cur , Curo , HideCur;
      int TrClr = 0;
      int EVGAX , EVGAY;
      int event_base , error_base;
      int xresmax , yresmax;
      static char bm_no_data [ ] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
      XRectangle Rect;
      typedef struct {
          unsigned long flags;
          unsigned long functions;
          unsigned long decorations;
          long inputMode;
          unsigned long status;
      } Hints;
      Hints hints;
      XColor c15 , c0;
      kgColor *kgcolors;
      D = ( DIALOG * ) Tmp;
      dspname = getenv ( "DISPLAY" ) ;
      if ( ! dspname ) {
          dspname = DISPLAY;
          setenv ( "DISPLAY" , dspname , 1 ) ;
      }
      xpos = D->xo; ypos = D->yo; xres = D->xl; yres = D->yl;
      title = D->name;
      transparency = D->transparency;
      dec = D->Deco;
      sticky = D->Sticky;
      fixpos = D->Fixpos;NoTaskBar = D->NoTaskBar;
      StackPos = D->StackPos;ShapeXpm = D->Shapexpm;
      Resize = D->Resize;
      NoWinMngr = D->NoWinMngr;
      hints.flags = 2; // Specify that we are changing the window decorations.
      hints.decorations = dec; // 0 ( false ) means that window decorations should go bye-bye.
      wc = ( kgWC * ) Malloc ( sizeof ( kgWC ) ) ;
      D->wc = wc;
//  XInitThreads(); // created problem for True Color
      XInitThreads ( ) ;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
      if ( ( WinMngr = kgCheckWindowManager ( ) ) == NULL ) {
          NoWinMngr = 1;
          D->NoWinMngr = 1;
      }
      else {
          NoWinMngr = 0;
          D->NoWinMngr = 0;
          free ( WinMngr ) ;
      }
      if ( Dsp == NULL ) {printf ( " Error: in opening Display ..\n" ) ;
      exit ( 0 ) ;}
      xresmax = DisplayWidth ( Dsp , DefaultScreen ( Dsp ) ) ;
      yresmax = DisplayHeight ( Dsp , DefaultScreen ( Dsp ) ) ;
      if ( XMatchVisualInfo ( Dsp , DefaultScreen ( Dsp ) , \
           32 , TrueColor , & visualinfo ) == 0 ) \
      {
          printf ( "Could not find a True Color Match\n" ) ;
          fflush ( stdout ) ;
          Vis = XDefaultVisual ( Dsp , DefaultScreen ( Dsp ) ) ;
          Dpth = DisplayPlanes ( Dsp , DefaultScreen ( Dsp ) ) ;
          TrClr = 0;
      }
      else {
          TrClr = 1;
          Vis = visualinfo.visual;
          Dpth = visualinfo.depth;
      }
      get_scan_code ( Dsp ) ;
#if 1
      if ( clrtbl [ 256 ] == -1 ) {
          for ( i = 256;i < NCLRS;i++ ) clrtbl [ i ] = i;
      }
#endif
      EVGAX = xres;
      EVGAY = yres;
      par = DefaultRootWindow ( Dsp ) ;
      par = RootWindow ( Dsp , DefaultScreen ( Dsp ) ) ;
      Parent = par;
#if 1
// Not needed ; it is better to window with Newwindow=0;
      if ( D->parent != NULL ) {
          Parent = WC ( D->parent )->Win;
//    printf ("Got Parent\n");
      }
      if ( D->PWIN != NULL ) {
          Parent = * ( ( Window * ) ( D->PWIN ) ) ;
//    fprintf(stderr, "Set PWIN\n");
      }
#endif
      xsh.flags = ( PPosition | PSize | PMinSize | PMaxSize | PResizeInc ) ;
      xsh.height = EVGAY;
      xsh.width = EVGAX;
      xsh.x = xpos;
      xsh.y = ypos;
      xswa.colormap = DefaultColormap ( Dsp , DefaultScreen ( Dsp ) ) ;
      xswa.colormap = XCreateColormap ( Dsp , DefaultRootWindow \
       ( Dsp ) , Vis , AllocNone ) ;
      xswa.bit_gravity = NorthWestGravity;
      xswa.background_pixel = 0xffffffff;
      xswa.background_pixel = 0x00000000;
      xswa.save_under = 1;
//  xswa.backing_store=Always;
      xswa.backing_store = NotUseful;
      xswa.cursor = None;
      xswa.background_pixmap = None;
      xswa.border_pixel = 0;
      xswa.override_redirect = False;
/* As on 18/11/12 */
      xwmpt = XAllocWMHints ( ) ;
      xwmpt->flags = AllHints;
      xwmpt->initial_state = NormalState;
      xwmpt->input = True;
      property = XInternAtom ( Dsp , "_MOTIF_WM_HINTS" , True ) ;
      Win = XCreateWindow ( Dsp , Parent , xsh.x , xsh.y , xsh.width , \
           xsh.height , 0 , Dpth , InputOutput , Vis , \
       ( CWColormap | CWBackingStore | CWBorderPixel | CWBitGravity \
       | CWOverrideRedirect ) , & xswa ) ;
//       (CWBackPixel|CWColormap|CWBorderPixel|CWBitGravity),&xswa);
//       (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBorderPixel|CWBitGravity),&xswa);
//       (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBorderPixel|CWBitGravity|CWOverrideRedirect|CWCursor),&xswa);
//  printf("Winid = %ld Parid= %ld\n",Win,Parent);
      XGetWindowAttributes ( Dsp , Win , & xwa ) ;
      EVGAX = xwa.width;
      EVGAY = xwa.height;
      wc->msg_x = ( EVGAX-316 ) /2;
      wc->msg_y = EVGAY-24;
      wc->msg_x = xpos;
      wc->msg_y = ypos;
/* added on 8-3-01 */
      xsh.flags = ( PPosition | PSize | PMinSize | PMaxSize | PResizeInc ) ;
      xsh.height = EVGAY;
      xsh.width = EVGAX;
      xsh.x = xwa.x;
      xsh.y = xwa.y;
// TCBTCB
      if ( Resize != 1 ) {
          xsh.min_width = xsh.width;
          xsh.min_height = xsh.height;
          xsh.max_width = xsh.width;
          xsh.max_height = xsh.height;
          xsh.width_inc = xsh.height_inc = 0;
      }
      else {
          xsh.min_width = D->MinWidth;
          xsh.min_height = D->MinHeight;
          xsh.max_width = xresmax;
          xsh.max_height = yresmax;
//    xsh.width_inc = xsh.height_inc = 0;
          xsh.width_inc = xsh.height_inc = 2;
      }
/* As on 18/11/12  for no decorations */ 
      XSetStandardProperties ( Dsp , Win , title , \
           title , None , argv , argc , & xsh ) ;
      D->Maxl = xsh.max_width;
      D->Maxw = xsh.max_height;
      if ( NoWinMngr != 1 ) {
          if ( TrClr ) {
              XChangeProperty ( Dsp , Win , property , property , 32 , PropModeReplace , \
               ( unsigned char * ) & hints , 5 ) ;
          }
          XSetWMHints ( Dsp , Win , xwmpt ) ;
      }
      XFree ( xwmpt ) ;
      Cmap = xswa.colormap;
      uiset_def_colors ( Dsp , Cmap ) ;
      xswa.colormap = Cmap;
      XChangeWindowAttributes ( Dsp , Win , ( CWColormap ) , & xswa ) ;
//       (CWBackPixmap|CWBackPixel|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
//       (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
//     (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity|CWOverrideRedirect|CWCursor),&xswa);
      XGetWindowAttributes ( Dsp , Win , & xwa ) ;
      if ( xwa.backing_store == Always ) {
          BackStore = 1;
      }
//  if(!XCompositeQueryExtension(Dsp,&event_base,&error_base)) {
      {
//    printf("NO COMPOSITE EXTENSION\n");
          if ( ShapeXpm != NULL ) {
              CurMask = XCPix1 ( 0 , 0 , EVGAX , EVGAY ) ;
              ShapeGc = XCreateGC ( Dsp , CurMask , ( GCForeground \
               | GCBackground ) , & gcv ) ;
              XSetBackground ( Dsp , ShapeGc , 0 ) ;
              XSetForeground ( Dsp , ShapeGc , 0 ) ;
              XFillRectangle ( Dsp , CurMask , ShapeGc , 0 , 0 , EVGAX , EVGAY ) ;
              XSetForeground ( Dsp , ShapeGc , 1 ) ;
              kg_image_bitmap ( Dsp , ShapeGc , CurMask , ShapeXpm , EVGAX , EVGAY ) ;
              XShapeCombineMask ( Dsp , Win , ShapeBounding , 0 , 0 , CurMask , ShapeSet ) ;
              XFreePixmap ( Dsp , CurMask ) ;
              XFreeGC ( Dsp , ShapeGc ) ;
          }
          else {
              Rect.x = 0;
              Rect.y = 0;
              Rect.width = EVGAX;
              Rect.height = EVGAY;
              if ( Resize != 1 ) XShapeCombineRectangles ( Dsp , Win , \
                   ShapeClip , 0 , 0 , & Rect , 1 , ShapeSet , 1 ) ;
          }
      }
      gcv.foreground = 10;
      gcv.background = 0;
      gcv.line_width = 0;
      Gc = XCreateGC ( Dsp , Win , ( GCForeground | GCBackground ) , & gcv ) ;
      uiDefaultDisplayFontSize ( Dsp , Gc ) ;
      c15.pixel = 15;
      c0.pixel = 0;
#if 0
      CurPix = XCreateBitmapFromData ( Dsp , win , bm_no_data , 8 , 8 ) ;
      wc->HideCur = XCreatePixmapCursor ( Dsp , CurPix , \
           CurPix , & c0 , & c0 , 0 , 0 ) ;
#else
      CurPix = XCPix1 ( 0 , 0 , 0 , 0 ) ;
      CurMask = XCPix1 ( 0 , 0 , 0 , 0 ) ;
//  wc->HideCur =  XCreatePixmapCursor(Dsp,CurPix,CurMask,&c15,&c0,1,1);
      wc->HideCur = XCreatePixmapCursor ( Dsp , CurPix , \
           CurMask , & c0 , & c0 , 0 , 0 ) ;
#endif
      XFreePixmap ( Dsp , CurPix ) ;
      XFreePixmap ( Dsp , CurMask ) ;
      wc->Pix = XCPix ( 0 , 0 , xsh.max_width , xsh.max_height ) ;
      wc->Piximg = XCPix ( 0 , 0 , xsh.max_width , xsh.max_height ) ;
      wc->EVGAX = EVGAX;
      wc->EVGAY = EVGAY;
      wc->Dsp = Dsp;
      wc->Dpth = Dpth;
      wc->Win = Win;
      wc->ActWin = wc->Pix;
      wc->DspWin = wc->Pix;
      wc->Cmap = Cmap;
      wc->Gc = Gc;
// One can use it; if needed
//  XFillRectangle(Dsp,Pix,Gc,0,0,EVGAX,EVGAY);
//  wcset_clr(wc,0);
//  Curo= XCreateFontCursor(Dsp,59);
//  Curo= XCreateFontCursor(Dsp,2);
      Curo = XCreateFontCursor ( Dsp , 94 ) ;
      Cur = XCreateFontCursor ( Dsp , 35 ) ;
      XMapWindow ( Dsp , Win ) ;
      XDefineCursor ( Dsp , Win , Curo ) ;
      wc->CurrCur = Curo;
      XSelectInput ( Dsp , Win , EventMask ) ;
      if ( NoWinMngr != 1 ) {
//  XNextEvent(Dsp,&event);
          cEvent.type = ClientMessage;
          cEvent.window = Win;
          cEvent.format = 32;
          cEvent.data.l [ 0 ] = 0xFFFFFFFF;
          cEvent.data.l [ 1 ] = 1;
          cEvent.data.l [ 2 ] = 0;
          cEvent.data.l [ 3 ] = 0;
          cEvent.data.l [ 4 ] = 0;
          cEvent.message_type = XInternAtom ( Dsp , "_NET_WM_DESKTOP" , True ) ;
//TCB
          if ( sticky ) wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
          cEvent.message_type = XInternAtom ( Dsp , "_NET_WM_ALLOWED_ACTIONS" , True ) ;
          cEvent.data.l [ 0 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_STICK" , True ) ;
          cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_ABOVE" , True ) ;
          cEvent.data.l [ 2 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_BELOW" , True ) ;
          cEvent.data.l [ 3 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_CHANGE_DESKTOP" , \
               True ) ;
          cEvent.data.l [ 4 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_MOVE" , True ) ;
          wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
          cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_STAY_ON_TOP" , \
               True ) ;
          wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
          cEvent.message_type = XInternAtom ( Dsp , "_NET_WM_STATE" , True ) ;
          cEvent.data.l [ 0 ] = 1;
          cEvent.data.l [ 3 ] = 1;
          cEvent.data.l [ 4 ] = 0;
//    if(dec==0) {
          if ( NoTaskBar == 1 ) {
              cEvent.data.l [ 1 ] = XInternAtom ( Dsp , \
                   "_NET_WM_STATE_SKIP_PAGER" , True ) ;
              cEvent.data.l [ 2 ] = XInternAtom ( Dsp , "_NET_WM_STATE_SKIP_TASKBAR" , \
                   True ) ;
              wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
          }
          if ( fixpos == 1 ) {
              cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_STATE_STICKY" , True ) ;
              cEvent.data.l [ 2 ] = 0;
              wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
          }
          switch ( StackPos ) {
              case 1:
              cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_STATE_ABOVE" , True ) ;
              cEvent.data.l [ 2 ] = 0;
              wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
              cEvent.data.l [ 1 ] = XInternAtom ( Dsp , \
                   "_NET_WM_STATE_STAY_ON_TOP" , True ) ;
              cEvent.data.l [ 2 ] = 0;
              wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
              break;
              case -1:
              cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_STATE_BELOW" , True ) ;
              cEvent.data.l [ 2 ] = 0;
              wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
              break;
              default:
              break;
          }
          if ( D->fullscreen ) {
              cEvent.data.l [ 1 ] = XInternAtom ( Dsp , \
                   "_NET_WM_STATE_FULLSCREEN" , True ) ;
              cEvent.data.l [ 2 ] = 0;
              wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
          }
      }// end NoWinMngr != 1
      wc->IMAGE = wc->IMAGEBAK = NULL;
#if 0
      wc->IMAGE = XGetImage ( Dsp , Win , 0 , 0 , 20 , 20 , 0xffffffff , ZPixmap ) ;
      wc->IMAGEBAK = XGetImage ( Dsp , Win , 0 , \
           0 , 20 , 20 , 0xffffffff , ZPixmap ) ;
#else
      wc->IMAGE = XGetImage ( Dsp , wc->Pix , 0 , \
           0 , 20 , 20 , 0xffffffff , ZPixmap ) ;
      wc->IMAGEBAK = XGetImage ( Dsp , wc->Pix , 0 , \
           0 , 20 , 20 , 0xffffffff , ZPixmap ) ;
#endif
      uiMakeImageTransparent ( wc->IMAGE , transparency ) ;
      uiMakeImageTransparent ( wc->IMAGEBAK , transparency ) ;
#if 0
      XPutImage ( Dsp , wc->Pix , Gc , wc->IMAGE , 0 , 0 , 0 , 0 , EVGAX , EVGAY ) ;
      XPutImage ( Dsp , wc->Piximg , Gc , wc->IMAGE , \
           0 , 0 , 0 , 0 , EVGAX , EVGAY ) ;
#else
      _uiTileImage ( wc , wc->Pix , wc->IMAGE , xresmax , yresmax ) ;
      _uiTileImage ( wc , wc->Piximg , wc->IMAGE , xresmax , yresmax ) ;
#endif
      uiMakeImageTransparent ( wc->IMAGE , 0.0 ) ;
//TCB
      wc->REDMASKPOS = GetNoOfBits ( wc->IMAGE->red_mask ) ;
      wc->GREENMASKPOS = GetNoOfBits ( wc->IMAGE->green_mask ) ;
      wc->BLUEMASKPOS = GetNoOfBits ( wc->IMAGE->blue_mask ) ;
      XRaiseWindow ( Dsp , Win ) ;
      wc->DefGuiFontEntry = 0;
// Needs Modification;
      pthread_mutex_init ( & ( wc->Dsplock ) , NULL ) ;
      wc->EVGAX = EVGAX;
      wc->EVGAY = EVGAY;
      wc->Dsp = Dsp;
      wc->Dpth = Dpth;
      wc->Win = Win;
      wc->Gc = Gc;
      wc->FullScreen = 0;
      wc->c_color = 0;
      wc->Clip = Dopen ( ) ;
      wc->SBlist = Dopen ( ) ;
      wc->TLIST = Dopen ( ) ;
      wc->ExposeWin = 0;
      wc->kgcolors = ( kgColor * ) Malloc ( sizeof ( kgColor ) *1024 ) ;
      wc->GuiFont = 23;
      wc->GuiFontSize = 8;
      kgcolors = ( kgColor * ) wc->kgcolors;
      for ( i = 0;i < 256;i++ ) {
          kgcolors [ i ] .red = kgIcode [ i ] [ 0 ] ;
          kgcolors [ i ] .green = kgIcode [ i ] [ 1 ] ;
          kgcolors [ i ] .blue = kgIcode [ i ] [ 2 ] ;
          kgcolors [ i ] .Col = DefClrs [ i ] ;
          kgcolors [ i ] .clrtbl = clrtbl [ i ] ;
      }
      for ( i = 256;i < NCLRS;i++ ) {
          kgcolors [ i ] .red = kgIcode [ i ] [ 0 ] ;
          kgcolors [ i ] .green = kgIcode [ i ] [ 1 ] ;
          kgcolors [ i ] .blue = kgIcode [ i ] [ 2 ] ;
          kgcolors [ i ] .Col = DefClrs [ i ] ;
          kgcolors [ i ] .clrtbl = clrtbl [ i ] ;
      }
      {
          Window Root , Par , *Child;
          int nChild , i;
          XQueryTree ( Dsp , Win , & Root , & Par , & Child , & nChild ) ;
//     printf("Win:par,Root,Par: %d %d %d %d,%d\n",Win,par,Root,Par,nChild);
          wc->Root = Root;
          wc->Parent = Par;
          if ( nChild != 0 ) XFree ( Child ) ;
#if 0
          XQueryTree ( Dsp , Par , & Root , & Par , & Child , & nChild ) ;
          printf ( "Win:par,Root,Par: %d %d %d %d,%d\n" , \
               Win , par , Root , Par , nChild ) ;
          for ( i = 0;i < nChild;i++ ) {
              if ( Child [ i ] == Win ) printf ( "Child: %d %d\n" , Win , Child [ i ] ) ;
          }
          if ( nChild != 0 ) XFree ( Child ) ;
#endif
      }
      wcset_clr ( wc , 0 ) ;
      XSync ( Dsp , False ) ;
      RefreshWindowThread ( wc ) ;
      wc->Rth = 0;
  //kgEnableSelection(D);
      wc->Hlt = 0;
      wc->Pstr = NULL;
      wc->Cstr = NULL;
      wc->cpmenu = NULL;
      wc->cpCallback = NULL;
      return wc;
  }
  void *ui_create_window ( int xpos , int ypos , int xres , int yres , char *title , \
       int dec , float transparency , int sticky , int fixpos , \
       int NoTaskBar , int StackPos , void *ShapeXpm ) \
  {
      kgWC *wc;
      static int ENTRY = 1;
      static int argc = 1;
      static char *argv [ 1 ] = {"Testing"};
      int i;
      Display *Dsp;
      Colormap Cmap;
      Visual *Vis;
      unsigned char data [ 5 ] = {0 , 0 , 0 , 0 , 0};
      unsigned int Dpth = 32;
      Dlink *SBlist;
      Window par , Win;
      Pixmap CurPix , CurMask;
      GC Gc , ShapeGc;
      XGCValues gcv;
      XEvent event;
      XSizeHints xsh;
      XSetWindowAttributes xswa;
      XWindowAttributes xwa;
      unsigned int fg , bg , bd;
/* Added as on 18/11/12/ */
      XWMHints xwmh , *xwmpt;
      XVisualInfo visualinfo ;
      Atom property , args [ 4 ] ;
      XClientMessageEvent cEvent;
      Cursor Cur , Curo , HideCur;
      int TrClr = 0;
      int EVGAX , EVGAY;
      int event_base , error_base;
      int xresmax , yresmax;
      XRectangle Rect;
      typedef struct {
          unsigned long flags;
          unsigned long functions;
          unsigned long decorations;
          long inputMode;
          unsigned long status;
      } Hints;
      Hints hints;
      hints.flags = 2; // Specify that we are changing the window decorations.
      hints.decorations = dec; // 0 ( false ) means that window decorations should go bye-bye.
/* End of Additions */
//  Pixmap Pix;
      XColor c15 , c0;
      kgColor *kgcolors;
//  XInitThreads(); // created problem for True Color
      wc = ( kgWC * ) Malloc ( sizeof ( kgWC ) ) ;
      XInitThreads ( ) ;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
      if ( Dsp == NULL ) {printf ( " Error: in opening Display ..\n" ) ;
      exit ( 0 ) ;};
      xresmax = DisplayWidth ( Dsp , DefaultScreen ( Dsp ) ) ;
      yresmax = DisplayHeight ( Dsp , DefaultScreen ( Dsp ) ) ;
      if ( XMatchVisualInfo ( Dsp , DefaultScreen ( Dsp ) , \
           32 , TrueColor , & visualinfo ) == 0 ) \
      {
          fprintf ( stderr , "Could not find a True Color Match\n" ) ;
          Vis = XDefaultVisual ( Dsp , DefaultScreen ( Dsp ) ) ;
          Dpth = DisplayPlanes ( Dsp , DefaultScreen ( Dsp ) ) ;
          TrClr = 0;
      }
      else {
//       printf("Got True Color\n");
          TrClr = 1;
          Vis = visualinfo.visual;
          Dpth = visualinfo.depth;
//       printf("DisplayPlanes: %d\n",DisplayPlanes(Dsp,DefaultScreen(Dsp)));
      }
      get_scan_code ( Dsp ) ;
      if ( clrtbl [ 256 ] == -1 ) {
          for ( i = 256;i < NCLRS;i++ ) clrtbl [ i ] = i;
      }
      EVGAX = xres;
      EVGAY = yres;
//  Vx1 = EVGAX, Vy1 = EVGAY;
      par = DefaultRootWindow ( Dsp ) ;
      par = RootWindow ( Dsp , DefaultScreen ( Dsp ) ) ;
      xsh.flags = ( PPosition | PSize | PMinSize | PMaxSize | PResizeInc ) ;
      xsh.height = EVGAY;
      xsh.width = EVGAX;
      xsh.x = xpos;
      xsh.y = ypos;
      xswa.colormap = DefaultColormap ( Dsp , DefaultScreen ( Dsp ) ) ;
      xswa.colormap = XCreateColormap ( Dsp , DefaultRootWindow \
       ( Dsp ) , Vis , AllocNone ) ;
      xswa.bit_gravity = NorthWestGravity;
      xswa.background_pixel = 0xffffffff;
      xswa.background_pixel = 0x00000000;
      xswa.save_under = 1;
//  xswa.backing_store=Always;
      xswa.backing_store = NotUseful;
      xswa.cursor = None;
//  xswa.cursor= XCreateFontCursor(Dsp,50);
//  xswa.background_pixmap=ParentRelative;
      xswa.background_pixmap = None;
      xswa.border_pixel = 0;
      xswa.override_redirect = False;
/* As on 18/11/12 */
//  xwmpt=XGetWMHints(Dsp,par);
//  if(xwmpt==NULL) xwmpt=XAllocWMHints();
      xwmpt = XAllocWMHints ( ) ;
      xwmpt->flags = AllHints;
      xwmpt->initial_state = NormalState;
      xwmpt->input = True;
//TCB
//  xwmpt->input=False;
//  xwmpt->initial_state=WithdrawnState;
      property = XInternAtom ( Dsp , "_MOTIF_WM_HINTS" , True ) ;
/*
  XSizeHints sizehints ;
  sizehints.flags = PPosition | PSize ;
  sizehints.x = 50 ; sizehints.y = 300 ;
  sizehints.width = 400 ; sizehints.height = 100 ;
  XSetWMNormalHints( Dsp, win, &sizehints ) ;
*/
//  xsh.flags=(PSize|PMinSize|PMaxSize|PResizeInc);
/* Over */
//  xswa.backing_store=NotUseful;
      Win = XCreateWindow ( Dsp , par , xsh.x , xsh.y , xsh.width , \
           xsh.height , 0 , Dpth , InputOutput , Vis , \
       ( CWColormap | CWBackingStore | CWBorderPixel | CWBitGravity \
       | CWOverrideRedirect ) , & xswa ) ;
//       (CWBackPixel|CWColormap|CWBorderPixel|CWBitGravity),&xswa);
//       (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBorderPixel|CWBitGravity),&xswa);
//       (CWBackPixel|CWBackingStore|CWSaveUnder|CWColormap|CWBorderPixel|CWBitGravity),&xswa);
//       (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBorderPixel|CWBitGravity),&xswa);
//       (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBorderPixel|CWBitGravity|CWOverrideRedirect|CWCursor),&xswa);
//       (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
//       (CWBackPixel|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
      XGetWindowAttributes ( Dsp , Win , & xwa ) ;
      EVGAX = xwa.width;
      EVGAY = xwa.height;
      wc->msg_x = ( EVGAX-316 ) /2;
      wc->msg_y = EVGAY-24;
/* added on 8-3-01 */
      xsh.flags = ( PPosition | PSize | PMinSize | PMaxSize | PResizeInc ) ;
      xsh.height = EVGAY;
      xsh.width = EVGAX;
      xsh.x = xwa.x;
      xsh.y = xwa.y;
// TCBTCB
#if 0
      xsh.min_width = xsh.width;
      xsh.min_height = xsh.height;
      xsh.max_width = xsh.width;
      xsh.max_height = xsh.height;
      xsh.width_inc = xsh.height_inc = 0;
#else
      xsh.min_width = xsh.width/8+8;
      xsh.min_height = xsh.height/8+8;
      xsh.max_width = xresmax;
      xsh.max_height = yresmax;
      xsh.width_inc = xsh.height_inc = 0;
#endif
#if 0
/* end of add on 8-3-01 */
      XSetStandardProperties ( Dsp , Win , title , \
           title , None , argv , argc , & xsh ) ;
      XSetWMHints ( Dsp , Win , & xwmh ) ;
/*
  xswa.colormap=DefaultColormap(Dsp,DefaultScreen(Dsp));
  Vis = XDefaultVisual(Dsp,DefaultScreen(Dsp));
*/
#else
/* As on 18/11/12  for no decorations */ 
      XSetStandardProperties ( Dsp , Win , title , \
           title , None , argv , argc , & xsh ) ;
      if ( TrClr ) {
          XChangeProperty ( Dsp , Win , property , property , 32 , PropModeReplace , \
           ( unsigned char * ) & hints , 5 ) ;
#if 0
          if ( property = XInternAtom ( Dsp , "_WM_WINDOW_OPACITY" , False ) != None ) ;
          if ( ( property = XInternAtom ( Dsp , "WM_DELETE_WINDOW" , \
               False ) ) != None ) \
          {
          }
          if ( property = XInternAtom ( Dsp , "_NET_WM_STATE" , True ) != None ) {
              printf ( "Changing type\n" ) ;
              args [ 0 ] = XInternAtom ( Dsp , "_NET_WM_STATE_BELOW" , True ) ;
              XChangeProperty ( Dsp , Win , property , property , sizeof \
               ( Atom ) PropModeReplace , ( unsigned char * ) args , 1 ) ;
          }
          if ( property = XInternAtom ( Dsp , "_WM_WINDOW_OPACITY" , True ) != None ) {
              printf ( "Changing Opacity\n" ) ;
              data [ 0 ] = 0;
              XChangeProperty ( Dsp , Win , property , property , 8 , PropModeReplace , \
               ( unsigned char * ) data , 1 ) ;
          }
#endif
      }
      XSetWMHints ( Dsp , Win , xwmpt ) ;
      XFree ( xwmpt ) ;
#endif
      Cmap = xswa.colormap;
//TCB removed as on 12 Aug 13
//  for(i=64;i<NCLRS;i++) uiget_color(Dsp,Cmap,i,&kgIcode[i][0],&kgIcode[i][1],&kgIcode[i][2]);
      uiset_def_colors ( Dsp , Cmap ) ;
      xswa.colormap = Cmap;
      XChangeWindowAttributes ( Dsp , Win , ( CWColormap ) , & xswa ) ;
//       (CWBackPixmap|CWBackPixel|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
//       (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
//     (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity|CWOverrideRedirect|CWCursor),&xswa);
//    (CWBackPixmap|CWColormap|CWBitGravity),&xswa);
//    (CWBackPixmap|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
//    (CWBackPixel|CWBackingStore|CWSaveUnder|CWColormap|CWBitGravity),&xswa);
      XGetWindowAttributes ( Dsp , Win , & xwa ) ;
      if ( xwa.backing_store == Always ) {
          BackStore = 1;
      }
#if 1
//  if(!XCompositeQueryExtension(Dsp,&event_base,&error_base)) {
      {
//    printf("NO COMPOSITE EXTENSION\n");
          if ( ShapeXpm != NULL ) {
              CurMask = XCPix1 ( 0 , 0 , EVGAX , EVGAY ) ;
              ShapeGc = XCreateGC ( Dsp , CurMask , ( GCForeground \
               | GCBackground ) , & gcv ) ;
              XSetBackground ( Dsp , ShapeGc , 0 ) ;
              XSetForeground ( Dsp , ShapeGc , 0 ) ;
              XFillRectangle ( Dsp , CurMask , ShapeGc , 0 , 0 , EVGAX , EVGAY ) ;
              XSetForeground ( Dsp , ShapeGc , 1 ) ;
              kg_image_bitmap ( Dsp , ShapeGc , CurMask , ShapeXpm , EVGAX , EVGAY ) ;
              XShapeCombineMask ( Dsp , Win , ShapeBounding , 0 , 0 , CurMask , ShapeSet ) ;
              XFreePixmap ( Dsp , CurMask ) ;
              XFreeGC ( Dsp , ShapeGc ) ;
          }
          else {
#if 0
              CurMask = XCPix1 ( 0 , 0 , EVGAX , EVGAY ) ;
              ShapeGc = XCreateGC ( Dsp , CurMask , ( GCForeground \
               | GCBackground ) , & gcv ) ;
              XSetForeground ( Dsp , ShapeGc , 1 ) ;
              XFillRectangle ( Dsp , CurMask , ShapeGc , 0 , 0 , EVGAX , EVGAY ) ;
              XShapeCombineMask ( Dsp , Win , ShapeClip , 0 , 0 , CurMask , ShapeSet ) ;
              XFreePixmap ( Dsp , CurMask ) ;
#else
              Rect.x = 0;
              Rect.y = 0;
              Rect.width = EVGAX;
              Rect.height = EVGAY;
//TCBTCB 
// for resizing this should not be there
#ifdef D_NORESIZE
              XShapeCombineRectangles ( Dsp , Win , ShapeClip , \
                   0 , 0 , & Rect , 1 , ShapeSet , 1 ) ;
#endif
#endif
          }
      }
#endif
      gcv.foreground = 10;
      gcv.background = 0;
      gcv.line_width = 0;
      Gc = XCreateGC ( Dsp , Win , ( GCForeground | GCBackground ) , & gcv ) ;
      uiDefaultDisplayFontSize ( Dsp , Gc ) ;
/*
  XQueryColor(Dsp,Cmap,&c15);
  XQueryColor(Dsp,Cmap,&c0);
*/
      c15.pixel = 15;
      c0.pixel = 0;
//  Pix = XCreatePixmap(Dsp,Win,1,1,1);
      CurPix = XCPix1 ( 0 , 0 , 2 , 2 ) ;
      CurMask = XCPix1 ( 0 , 0 , 2 , 2 ) ;
      wc->HideCur = XCreatePixmapCursor ( Dsp , CurPix , \
           CurMask , & c15 , & c0 , 1 , 1 ) ;
      XFreePixmap ( Dsp , CurPix ) ;
      XFreePixmap ( Dsp , CurMask ) ;
      wc->Pix = XCPix ( 0 , 0 , xsh.max_width , xsh.max_height ) ;
      wc->Piximg = XCPix ( 0 , 0 , xsh.max_width , xsh.max_height ) ;
      wc->EVGAX = EVGAX;
      wc->EVGAY = EVGAY;
      wc->Dsp = Dsp;
      wc->Dpth = Dpth;
      wc->Win = Win;
      wc->ActWin = wc->Pix;
      wc->DspWin = wc->Pix;
      wc->Cmap = Cmap;
      wc->Gc = Gc;
// One can use it; if needed
//  XFillRectangle(Dsp,Pix,Gc,0,0,EVGAX,EVGAY);
//  wcset_clr(wc,0);
      Curo = XCreateFontCursor ( Dsp , 59 ) ;
      Cur = XCreateFontCursor ( Dsp , 35 ) ;
#if 1
      XMapWindow ( Dsp , Win ) ;
      XDefineCursor ( Dsp , Win , Curo ) ;
//  XUndefineCursor(Dsp,Win);
      XSelectInput ( Dsp , Win , EventMask ) ;
//  XNextEvent(Dsp,&event);
      cEvent.type = ClientMessage;
      cEvent.window = Win;
      cEvent.format = 32;
      cEvent.data.l [ 0 ] = 0xFFFFFFFF;
      cEvent.data.l [ 1 ] = 1;
      cEvent.data.l [ 2 ] = 0;
      cEvent.data.l [ 3 ] = 0;
      cEvent.data.l [ 4 ] = 0;
      cEvent.message_type = XInternAtom ( Dsp , "_NET_WM_DESKTOP" , True ) ;
//TCB
      if ( sticky ) wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
      cEvent.message_type = XInternAtom ( Dsp , "_NET_WM_ALLOWED_ACTIONS" , True ) ;
      cEvent.data.l [ 0 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_STICK" , True ) ;
      cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_ABOVE" , True ) ;
      cEvent.data.l [ 2 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_BELOW" , True ) ;
      cEvent.data.l [ 3 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_CHANGE_DESKTOP" , \
           True ) ;
      cEvent.data.l [ 4 ] = XInternAtom ( Dsp , "_NET_WM_ACTION_MOVE" , True ) ;
      wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
      cEvent.message_type = XInternAtom ( Dsp , "_NET_WM_STATE" , True ) ;
      cEvent.data.l [ 0 ] = 1;
      cEvent.data.l [ 3 ] = 1;
      cEvent.data.l [ 4 ] = 0;
//    if(dec==0) {
      if ( NoTaskBar == 1 ) {
          cEvent.data.l [ 1 ] = XInternAtom ( Dsp , \
               "_NET_WM_STATE_SKIP_PAGER" , True ) ;
          cEvent.data.l [ 2 ] = XInternAtom ( Dsp , "_NET_WM_STATE_SKIP_TASKBAR" , \
               True ) ;
          wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
      }
      if ( fixpos == 1 ) {
          cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_STATE_STICKY" , True ) ;
          cEvent.data.l [ 2 ] = 0;
          wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
      }
#if 1
//sample
      switch ( StackPos ) {
          case 1:
          cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_STATE_ABOVE" , True ) ;
          cEvent.data.l [ 2 ] = 0;
//        printf("Sending ABOVE request\n");
          wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
          break;
          case -1:
          cEvent.data.l [ 1 ] = XInternAtom ( Dsp , "_NET_WM_STATE_BELOW" , True ) ;
          cEvent.data.l [ 2 ] = 0;
          wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
          break;
          default:
          break;
      }
#endif
//  XLowerWindow(Dsp,Win);
#endif
      wc->IMAGE = wc->IMAGEBAK = NULL;
      wc->IMAGE = XGetImage ( Dsp , Win , 0 , 0 , 20 , 20 , 0xffffffff , ZPixmap ) ;
      wc->IMAGEBAK = XGetImage ( Dsp , Win , 0 , \
           0 , 20 , 20 , 0xffffffff , ZPixmap ) ;
      uiMakeImageTransparent ( wc->IMAGE , transparency ) ;
      uiMakeImageTransparent ( wc->IMAGEBAK , transparency ) ;
#if 0
      XPutImage ( Dsp , wc->Pix , Gc , wc->IMAGE , 0 , 0 , 0 , 0 , EVGAX , EVGAY ) ;
      XPutImage ( Dsp , wc->Piximg , Gc , wc->IMAGE , \
           0 , 0 , 0 , 0 , EVGAX , EVGAY ) ;
#else
      _uiTileImage ( wc , wc->Pix , wc->IMAGE , xresmax , yresmax ) ;
      _uiTileImage ( wc , wc->Piximg , wc->IMAGE , xresmax , yresmax ) ;
#endif
      uiMakeImageTransparent ( wc->IMAGE , 0.0 ) ;
//TCB
      XRaiseWindow ( Dsp , Win ) ;
      wc->DefGuiFontEntry = 0;
      wc->REDMASKPOS = GetNoOfBits ( wc->IMAGE->red_mask ) ;
      wc->GREENMASKPOS = GetNoOfBits ( wc->IMAGE->green_mask ) ;
      wc->BLUEMASKPOS = GetNoOfBits ( wc->IMAGE->blue_mask ) ;
// Needs Modification;
      pthread_mutex_init ( & ( wc->Dsplock ) , NULL ) ;
      wc->EVGAX = EVGAX;
      wc->EVGAY = EVGAY;
      wc->Dsp = Dsp;
      wc->Dpth = Dpth;
      wc->Win = Win;
      wc->Gc = Gc;
      wc->FullScreen = 0;
      wc->c_color = 0;
      wc->Clip = Dopen ( ) ;
      wc->SBlist = Dopen ( ) ;
      wc->TLIST = Dopen ( ) ;
      wc->ExposeWin = 0;
      wc->kgcolors = ( kgColor * ) Malloc ( sizeof ( kgColor ) *1024 ) ;
      wc->GuiFont = 23;
      wc->GuiFontSize = 8;
      kgcolors = ( kgColor * ) wc->kgcolors;
      for ( i = 0;i < 64;i++ ) {
          kgcolors [ i ] .red = kgIcode [ i ] [ 0 ] ;
          kgcolors [ i ] .green = kgIcode [ i ] [ 1 ] ;
          kgcolors [ i ] .blue = kgIcode [ i ] [ 2 ] ;
          kgcolors [ i ] .Col = DefClrs [ i ] ;
          kgcolors [ i ] .clrtbl = clrtbl [ i ] ;
      }
      for ( i = 64;i < 256;i++ ) {
          kgcolors [ i ] .red = kgIcode [ i ] [ 0 ] ;
          kgcolors [ i ] .green = kgIcode [ i ] [ 1 ] ;
          kgcolors [ i ] .blue = kgIcode [ i ] [ 2 ] ;
      }
//  wc_alloc_scrn_buffer(wc);
//  printf("Calling wcset_clr\n");
      wcset_clr ( wc , 0 ) ;
      RefreshWindowThread ( wc ) ;
      return wc;
  }
  void kgIgnoreResize ( void *tmp ) {
      XClientMessageEvent cEvent;
      kgWC *wc;
      wc = WC ( tmp ) ;
      cEvent.type = ClientMessage;
      cEvent.window = wc->Win;
      cEvent.format = 32;
      cEvent.message_type = XInternAtom ( wc->Dsp , \
           "_NET_WM_ALLOWED_ACTIONS" , True ) ;
      cEvent.data.l [ 0 ] = XInternAtom ( wc->Dsp , \
           "_NET_WM_ACTION_STICK" , True ) ;
      cEvent.data.l [ 1 ] = XInternAtom ( wc->Dsp , \
           "_NET_WM_ACTION_ABOVE" , True ) ;
      cEvent.data.l [ 2 ] = XInternAtom ( wc->Dsp , \
           "_NET_WM_ACTION_BELOW" , True ) ;
      cEvent.data.l [ 3 ] = XInternAtom ( wc->Dsp , \
           "_NET_WM_ACTION_MOVE" , False ) ;
      cEvent.data.l [ 4 ] = XInternAtom ( wc->Dsp , \
           "_NET_WM_ACTION_RESIZE" , False ) ;
      wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
      return;
  }
  void kgCatchResize ( void *tmp ) {
      XClientMessageEvent cEvent;
      XEvent event;
      kgWC *wc;
      wc = WC ( tmp ) ;
      cEvent.type = ClientMessage;
      cEvent.window = wc->Win;
      cEvent.format = 32;
      cEvent.message_type = XInternAtom ( wc->Dsp , \
           "_NET_WM_ALLOWED_ACTIONS" , True ) ;
      cEvent.data.l [ 0 ] = XInternAtom ( wc->Dsp , \
           "_NET_WM_ACTION_STICK" , True ) ;
      cEvent.data.l [ 1 ] = XInternAtom ( wc->Dsp , \
           "_NET_WM_ACTION_ABOVE" , True ) ;
      cEvent.data.l [ 2 ] = XInternAtom ( wc->Dsp , \
           "_NET_WM_ACTION_BELOW" , True ) ;
      cEvent.data.l [ 3 ] = XInternAtom ( wc->Dsp , "_NET_WM_ACTION_MOVE" , True ) ;
      cEvent.data.l [ 4 ] = XInternAtom ( wc->Dsp , \
           "_NET_WM_ACTION_RESIZE" , True ) ;
      wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
      return;
  }
  void kgHideCursor ( DIALOG *D ) {
      kgWC *wc;
      wc = WC ( D ) ;
      XDefineCursor ( wc->Dsp , wc->Win , wc->HideCur ) ;
  }
  void kgVisibleCursor ( DIALOG *D ) {
      kgWC *wc;
      wc = WC ( D ) ;
      XDefineCursor ( wc->Dsp , wc->Win , wc->CurrCur ) ;
  }
/*
  Getting the resolution of the window
*/
  void Get_resolution ( int *xres , int *yres ) {
  }
/*
  Drawing a line
*/
  void _uiLine ( kgWC *wc , int x1 , int y1 , int x2 , int y2 ) {
      uiXDRAW ( wc , x1 , y1 , x2 , y2 ) ;
// XDrawLine(wc->Dsp,wc->WIN,wc->Gc,(short)(x1),(short)(wc->EVGAY-1-y1),(short)(x2),(short)(wc->EVGAY-1-y2));
  }
  void _dvLine ( kgWC *wc , int x1 , int y1 , int x2 , int y2 ) {
      dvXDRAW ( wc , x1 , y1 , x2 , y2 ) ;
  }
/*
  Filling a rectangular area
  using the currently set color
*/
  void _uiBox_Fill ( kgWC *wc , int xmin , int ymin , int xmax , int ymax ) {
      XFillRectangle ( wc->Dsp , wc->ActWin , wc->Gc , \
           xmin , ymin , xmax-xmin+1 , ymax-ymin+1 ) ;
  }
  void _dvBox_Fill ( kgWC *wc , int xmin , int ymin , int xmax , int ymax ) {
      XFillRectangle ( wc->Dsp , wc->ActWin , wc->Gc , \
           xmin , ymin , xmax-xmin+1 , ymax-ymin+1 ) ;
  }
  int kgDragRegion ( void *Tmp , int xmin , int ymin , \
       int xmax , int ymax , int *x , int *y ) \
  {
/* returns the last mid point */
      DIALOG *D;
      KBEVENT kb , kbe;
      int xp , yp , xl , yl , xpold = -100 , ypold = -100 , count , evnt;
      int EVGAX , EVGAY;
      XImage *Img , *bkup = NULL;
      D = ( DIALOG * ) Tmp;
      kgWC *wc;
      EVGAX = D->evgax;
      EVGAY = D->evgay;
      wc = WC ( Tmp ) ;
      xl = ( xmax-xmin+1 ) ;
      yl = ( ymax-ymin+1 ) ;
      if ( xmin < 0 ) {xmin = 0;xmax = xl;}
      if ( ymin < 0 ) {ymin = 0;ymax = yl;}
      if ( xmax > EVGAX ) {xmin = EVGAX-xl; xmax = EVGAX;}
      if ( ymax > EVGAY ) {ymin = EVGAY-yl; ymax = EVGAY;}
      Img = XGetImage ( wc->Dsp , wc->ActWin , xmin , \
           ymin , xl , yl , 0xffffffff , ZPixmap ) ;
      do {
          count = 0;
          kbe.event = 0;
          while ( ( ( evnt = kgCheckEvent ( D , & kb ) ) != 0 ) && ( kb.event == 3 ) ) {
              count++;
              kbe = kb;
              if ( count == 1 ) break;
          }
          if ( evnt ) kbe = kb;
          if ( kbe.event == 3 ) {
              xp = kbe.x-xl/2;
              yp = kbe.y-yl/2;
              if ( xp < 0 ) xp = 0;
              if ( yp < 0 ) yp = 0;
              if ( ( xp+xl ) > EVGAX ) xp = EVGAX-xl;
              if ( ( yp+yl ) > EVGAY ) yp = EVGAY-yl;
              if ( ( xp == xpold ) && ( yp == ypold ) ) continue;
              if ( bkup != NULL ) {
                  XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , bkup , \
                       0 , 0 , xpold , ypold , xl , yl ) ;
                  XDestroyImage ( bkup ) ;
                  bkup = NULL;
              }
              bkup = XGetImage ( wc->Dsp , wc->ActWin , xp , \
                   yp , xl , yl , 0xffffffff , ZPixmap ) ;
              XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
                   Img , 0 , 0 , xp , yp , xl , yl ) ;
              kgUpdateOn ( D ) ;
              xpold = xp; ypold = yp;
          }
      } while ( ( kbe ) .event != 2 ) ;
      if ( bkup != NULL ) {
          XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
               bkup , 0 , 0 , xp , yp , xl , yl ) ;
          XDestroyImage ( bkup ) ;
          kgUpdateOn ( D ) ;
      }
      XDestroyImage ( Img ) ;
      *x = kb.x;
      *y = kb.y;
      return 1;
  }
  int kgDragImage_o ( void *Tmp , void *Itmp , int xl , \
       int yl , int ymin , int ymax , int *x , int *y ) \
  {
      DIALOG *D;
      KBEVENT kb , kbe;
      int xp , yp , xpold = -100 , ypold = -100 , count , evnt , ret = 0 , no = 0;
      int EVGAX , EVGAY;
      XImage *Img = NULL , *bkup = NULL;
      D = ( DIALOG * ) Tmp;
      kgWC *wc;
      EVGAX = D->evgax;
      EVGAY = D->evgay;
      Img = ( XImage * ) Itmp;
      wc = WC ( Tmp ) ;
      if ( ymin < 0 ) ymin = 0;
      if ( ymax > EVGAY ) { ymax = EVGAY;}
      xp = *x;
      yp = *y;
      if ( xp < 0 ) xp = 0;
      if ( yp < 0 ) yp = 0;
      if ( ( xp+xl ) > EVGAX ) xp = EVGAX-xl;
      if ( ( yp+yl ) > EVGAY ) yp = EVGAY-yl;
      if ( yp < ymin ) yp = ymin;
      if ( ( yp+yl ) > ymax ) yp = ymax-yl;
      do {
          count = 0;
          kbe.event = 0;
          kbe.x = xp;
          kbe.y = yp;
          while ( ( ( evnt = kgCheckEvent ( D , & kb ) ) != 0 ) && ( kb.event == 3 ) ) {
              count++;
              kbe = kb;
              if ( count == 1 ) break;
          }
          if ( evnt ) kbe = kb;
          if ( kbe.event == 3 ) {
              xp = kbe.x-xl/2;
              yp = kbe.y-yl/2;
              if ( xp < 0 ) xp = 0;
              if ( yp < 0 ) yp = 0;
              if ( ( xp+xl ) > EVGAX ) xp = EVGAX-xl;
              if ( ( yp+yl ) > EVGAY ) yp = EVGAY-yl;
              if ( yp < ymin ) yp = ymin;
              if ( ( yp+yl ) > ymax ) yp = ymax-yl;
              if ( yp <= ymin ) {
                  ret = -1;
                  if ( bkup != NULL ) {
                      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , bkup , \
                           0 , 0 , xpold , ypold , xl , yl ) ;
                      XDestroyImage ( bkup ) ;
                      bkup = NULL;
                      kgUpdateOn ( D ) ;
                  }
                  else {
#if 0
                      bkup = XGetImage ( wc->Dsp , wc->ActWin , xp , \
                           yp , xl , yl , 0xffffffff , ZPixmap ) ;
                      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
                           Img , 0 , 0 , xp , yp , xl , yl ) ;
                      kgUpdateOn ( D ) ;
                      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
                           bkup , 0 , 0 , xp , yp , xl , yl ) ;
                      XDestroyImage ( bkup ) ;
                      kgUpdateOn ( D ) ;
#endif
                  }
                  *x = xp;
                  *y = yp;
                  return -1;
              }
              if ( ( yp+yl ) >= ymax ) {
                  ret = 1;
                  if ( bkup != NULL ) {
                      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , bkup , \
                           0 , 0 , xpold , ypold , xl , yl ) ;
                      XDestroyImage ( bkup ) ;
                      bkup = NULL;
                      kgUpdateOn ( D ) ;
                  }
                  else {
#if 0
                      bkup = XGetImage ( wc->Dsp , wc->ActWin , xp , \
                           yp , xl , yl , 0xffffffff , ZPixmap ) ;
                      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
                           Img , 0 , 0 , xp , yp , xl , yl ) ;
                      kgUpdateOn ( D ) ;
                      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
                           bkup , 0 , 0 , xp , yp , xl , yl ) ;
                      XDestroyImage ( bkup ) ;
                      kgUpdateOn ( D ) ;
#endif
                  }
                  *x = xp;
                  *y = yp;
                  return 1;
              }
              if ( ( xp == xpold ) && ( yp == ypold ) ) {
                  if ( ( bkup == NULL ) ) {
                      bkup = XGetImage ( wc->Dsp , wc->ActWin , xp , \
                           yp , xl , yl , 0xffffffff , ZPixmap ) ;
                      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
                           Img , 0 , 0 , xp , yp , xl , yl ) ;
                      kgUpdateOn ( D ) ;
                      xpold = xp;
                      ypold = yp;
                  }
                  continue;
              }
              if ( bkup != NULL ) {
                  XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , bkup , \
                       0 , 0 , xpold , ypold , xl , yl ) ;
                  XDestroyImage ( bkup ) ;
                  bkup = NULL;
              }
              bkup = XGetImage ( wc->Dsp , wc->ActWin , xp , \
                   yp , xl , yl , 0xffffffff , ZPixmap ) ;
              XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
                   Img , 0 , 0 , xp , yp , xl , yl ) ;
              kgUpdateOn ( D ) ;
              xpold = xp; ypold = yp;
          }
          else {
#if 0
              if ( ( evnt == 0 ) ) {
                  if ( ( bkup == NULL ) ) {
                      bkup = XGetImage ( wc->Dsp , wc->ActWin , xp , \
                           yp , xl , yl , 0xffffffff , ZPixmap ) ;
                      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
                           Img , 0 , 0 , xp , yp , xl , yl ) ;
                      kgUpdateOn ( D ) ;
                      xpold = xp;
                      ypold = yp;
                  }
                  else {
                      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , Img , \
                           0 , 0 , xpold , ypold , xl , yl ) ;
                      kgUpdateOn ( D ) ;
                  }
              }
#endif
          }
      } while ( ( kbe ) .event != 2 ) ;
      if ( bkup != NULL ) {
          XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
               bkup , 0 , 0 , xp , yp , xl , yl ) ;
          XDestroyImage ( bkup ) ;
          kgUpdateOn ( D ) ;
      }
      *x = kb.x;
      *y = kb.y;
      return ret;
  }
  int kgDragImage ( void *Tmp , void *Itmp , int xl , int yl , int xmin , \
       int ymin , int xmax , int ymax , int *x , int *y ) \
  {
      DIALOG *D;
      KBEVENT kb , kbe;
      int xp , yp , xpold = -100 , ypold = -100 , \
           count , evnt , ret = 0 , no = 0 , ymid;
      int EVGAX , EVGAY;
      XImage *Img = NULL , *bkup = NULL;
      D = ( DIALOG * ) Tmp;
      kgWC *wc;
      EVGAX = D->evgax;
      EVGAY = D->evgay;
      Img = ( XImage * ) Itmp;
      wc = WC ( Tmp ) ;
      if ( ymin < 0 ) ymin = 0;
      if ( ymax > EVGAY ) { ymax = EVGAY;}
      xp = *x;
      yp = *y;
      if ( xp < 0 ) xp = 0;
      if ( yp < 0 ) yp = 0;
      if ( ( xp+xl ) > EVGAX ) xp = EVGAX-xl;
      if ( ( yp+yl ) > EVGAY ) yp = EVGAY-yl;
      if ( ( xp > xmin ) && ( xp < xmax ) ) {
          if ( yp < ymin ) yp = ymin;
          if ( ( yp+yl ) > ymax ) yp = ymax-yl;
      }
      ymid = yp+ ( yl ) /2;
      do {
          count = 0;
          kbe.event = 0;
          kbe.x = xp;
          kbe.y = yp;
          while ( ( ( evnt = kgCheckEvent ( D , & kb ) ) != 0 ) && ( kb.event == 3 ) ) {
              count++;
              kbe = kb;
              if ( count == 1 ) break;
          }
          if ( evnt ) kbe = kb;
          if ( kbe.event == 3 ) {
              xp = kbe.x-xl/2;
              yp = kbe.y-yl/2;
              if ( xp < 0 ) xp = 0;
              if ( yp < 0 ) yp = 0;
              if ( ( xp+xl ) > EVGAX ) xp = EVGAX-xl;
              if ( ( yp+yl ) > EVGAY ) yp = EVGAY-yl;
              ymid = yp+ ( yl ) /2;
              if ( ( xp > xmin ) && ( xp < xmax ) && ( ymid > ymin ) && ( ymid < ymax ) ) {
                  if ( yp < ymin ) yp = ymin;
                  if ( ( yp+yl ) > ymax ) yp = ymax-yl;
                  if ( yp <= ymin ) {
                      ret = -1;
                      if ( bkup != NULL ) {
                          XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , bkup , \
                               0 , 0 , xpold , ypold , xl , yl ) ;
                          XDestroyImage ( bkup ) ;
                          bkup = NULL;
                          kgUpdateOn ( D ) ;
                      }
                      *x = xp;
                      *y = yp;
                      return -1;
                  }
                  if ( ( yp+yl ) >= ymax ) {
                      ret = 1;
                      if ( bkup != NULL ) {
                          XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , bkup , \
                               0 , 0 , xpold , ypold , xl , yl ) ;
                          XDestroyImage ( bkup ) ;
                          bkup = NULL;
                          kgUpdateOn ( D ) ;
                      }
                      *x = xp;
                      *y = yp;
                      return 1;
                  }
              }
              if ( ( xp == xpold ) && ( yp == ypold ) ) {
                  if ( ( bkup == NULL ) ) {
                      bkup = XGetImage ( wc->Dsp , wc->ActWin , xp , \
                           yp , xl , yl , 0xffffffff , ZPixmap ) ;
                      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
                           Img , 0 , 0 , xp , yp , xl , yl ) ;
                      kgUpdateOn ( D ) ;
                      xpold = xp;
                      ypold = yp;
                  }
                  continue;
              }
              if ( bkup != NULL ) {
                  XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , bkup , \
                       0 , 0 , xpold , ypold , xl , yl ) ;
                  XDestroyImage ( bkup ) ;
                  bkup = NULL;
              }
              bkup = XGetImage ( wc->Dsp , wc->ActWin , xp , \
                   yp , xl , yl , 0xffffffff , ZPixmap ) ;
              XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
                   Img , 0 , 0 , xp , yp , xl , yl ) ;
              kgUpdateOn ( D ) ;
              xpold = xp; ypold = yp;
          }
      } while ( ( kbe ) .event != 2 ) ;
      if ( bkup != NULL ) {
          XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
               bkup , 0 , 0 , xp , yp , xl , yl ) ;
          XDestroyImage ( bkup ) ;
          kgUpdateOn ( D ) ;
      }
      *x = kb.x;
      *y = kb.y;
      return ret;
  }
  void kgFreeXImage ( void *Img ) {
      if ( Img != NULL ) XDestroyImage ( ( XImage * ) Img ) ;
  }
  void * kgRegionImage ( void *Tmp , int xmin , \
       int ymin , int xmax , int ymax ) \
  {
      DIALOG *D;
      KBEVENT kb , kbe;
      int xp , yp , xl , yl , xpold = -100 , ypold = -100 , count , evnt;
      int EVGAX , EVGAY;
      XImage *Img , *bkup = NULL;
      D = ( DIALOG * ) Tmp;
      kgWC *wc;
      EVGAX = D->evgax;
      EVGAY = D->evgay;
      wc = WC ( Tmp ) ;
      xl = ( xmax-xmin+1 ) ;
      yl = ( ymax-ymin+1 ) ;
      if ( xmin < 0 ) {xmin = 0;xmax = xl;}
      if ( ymin < 0 ) {ymin = 0;ymax = yl;}
      if ( xmax > EVGAX ) {xmin = EVGAX-xl; xmax = EVGAX;}
      if ( ymax > EVGAY ) {ymin = EVGAY-yl; ymax = EVGAY;}
      Img = XGetImage ( wc->Dsp , wc->ActWin , xmin , \
           ymin , xl , yl , 0xffffffff , ZPixmap ) ;
      return ( void * ) Img;
  }
  void _uiBox_Fill_Transparent ( kgWC *wc , int xmin , int ymin , \
       int xmax , int ymax , float transparency ) \
  {
      unsigned long ALPHA;
      XImage *Img;
      ALPHA = 255* ( 1.-transparency ) ;
      ALPHA = ( ALPHA << 24 ) ;
      XFillRectangle ( wc->Dsp , wc->ActWin , wc->Gc , \
           xmin , ymin , xmax-xmin+1 , ymax-ymin+1 ) ;
      Img = XGetImage ( wc->Dsp , wc->ActWin , xmin , ymin , xmax-xmin+1 , \
           ymax-ymin+1 , 0x00ffffff , ZPixmap ) ;
      XAddPixel ( Img , ALPHA ) ;
      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , Img , 0 , \
           0 , xmin , ymin , xmax-xmin+1 , ymax-ymin+1 ) ;
      XDestroyImage ( Img ) ;
  }
  void _dvBox_Fill_Transparent ( kgWC *wc , int xmin , int ymin , \
       int xmax , int ymax , float transparency ) \
  {
      unsigned long ALPHA = 0xff000000 , pixl , val , color;
      int i , j , w , h , clr;
      float fac;
      XImage *uiImage;
      unsigned char *dest , *Imgdata , Alpha;
      unsigned long blue , green , red , ximage_rowbytes;
      unsigned long RedMaskPos , GreenMaskPos , BlueMaskPos , \
           Rshift , Gshift , Bshift;
      if ( transparency != 0.0 ) {
#if 1
          fac = 1-transparency;
          Alpha = ( unsigned long ) ( 255* ( 1-transparency*transparency ) ) ;
          ALPHA = ( Alpha << 24 ) ;
          clr = wc->c_color;
          red = kgIcode [ clr ] [ 0 ] *fac;
          green = kgIcode [ clr ] [ 1 ] *fac;
          blue = kgIcode [ clr ] [ 2 ] *fac;
          uiImage = XGetImage ( wc->Dsp , wc->ActWin , xmin , ymin , \
               xmax-xmin+1 , ymax-ymin+1 , 0xffffffff , ZPixmap ) ;
#if 1
#if 0
          Rshift = GetShift ( uiImage->red_mask ) ;
          Gshift = GetShift ( uiImage->green_mask ) ;
          Bshift = GetShift ( uiImage->blue_mask ) ;
//     printf("RShift: %d %d %d:%d %d %d %d %d\n",Rshift,Gshift,Bshift,uiImage->xoffset,uiImage->format,uiImage->depth,uiImage->bytes_per_line,uiImage->bits_per_pixel);
#endif
          ximage_rowbytes = uiImage->bytes_per_line;
          Imgdata = ( unsigned char * ) uiImage->data;
          w = uiImage->width;
          h = uiImage->height;
          for ( j = 0;j < h;j++ ) {
              dest = ( unsigned char * ) ( Imgdata +j*ximage_rowbytes ) ;
              for ( i = 0;i < w;i++ ) {
#if 0
                  color = blue+ ( green << 8 ) + ( red << 16 ) + ( ALPHA ) ;
                  XPutPixel ( uiImage , i , j , color ) ;
#else
                  *dest = blue; dest++;
                  *dest = green; dest++;
                  *dest = red ; dest++;
                  *dest = Alpha ; dest++;
#endif
              }
          }
#else
          XAddPixel ( uiImage , ALPHA ) ;
#endif
#else
//     printf("CallinguiChangeImageTransparency\n");
          XFillRectangle ( wc->Dsp , wc->ActWin , wc->Gc , \
               xmin , ymin , xmax-xmin+1 , ymax-ymin+1 ) ;
          uiImage = XGetImage ( wc->Dsp , wc->ActWin , xmin , ymin , \
               xmax-xmin+1 , ymax-ymin+1 , 0x00ffffff , ZPixmap ) ;
          uiChangeImageTransparency ( uiImage , transparency ) ;
#endif
          XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , uiImage , 0 , \
               0 , xmin , ymin , xmax-xmin+1 , ymax-ymin+1 ) ;
          XDestroyImage ( uiImage ) ;
      }
      else XFillRectangle ( wc->Dsp , wc->ActWin , wc->Gc , \
           xmin , ymin , xmax-xmin+1 , ymax-ymin+1 ) ;
  }
/*
  routines to maintain screen backups
*/
/*
  Backing up a given area
*/
//TCB
  void Bell ( void ) {
  }
  void uiDraw_String ( DIALOG *D , int x , int y , char *str , int justfic ) {
      int ln , i , maxchar;
      void *img = NULL;
      float length;
      kgWC *wc;
#if 1
//   XDrawString(Dsp,ActWin,Gc,x,y,str,strlen(str)-1);
      wc = D->wc;
//   ln = strlen(str);
      ln = uiStringLength ( str , D->gc.FontSize ) +1.0;
      maxchar = ( ln+wc->GuiFontSize ) /D->gc.FontSize;
#if 0
      if ( ln <= 0 ) return;
      while ( str [ ln-1 ] <= ' ' ) ln--;
      if ( ln <= 0 ) return;
#endif
//   img = gphStringToImage(str,ln*10,20,3,wc->c_color);
      img = kgStringToImage ( str , NULL , ln , 20 , D->gc.Font , \
           wc->c_color , justfic , D->gc.FontSize , -1 ) ;
      if ( img != NULL ) {
          kgImage ( D , img , x , y-16 , ln , 20 , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
      }
#else
      XTextItem *item;
      ln = strlen ( str ) ;
      if ( ln <= 0 ) return;
      while ( str [ ln-1 ] <= ' ' ) ln--;
      if ( ln <= 0 ) return;
      item = ( XTextItem * ) malloc ( sizeof ( XTextItem ) *ln ) ;
      if ( item == NULL ) {
          printf ( "Error: IN MALLOC....\n" ) ;
          exit ( 1 ) ;
      }
      item [ 0 ] .chars = str;
      item [ 0 ] .nchars = 1;
      item [ 0 ] .delta = 0;
      item [ 0 ] .font = None;
      for ( i = 1;i < ln;i++ ) {
          item [ i ] .chars = str+i;
          item [ i ] .nchars = 1;
          item [ i ] .delta = 0;
          item [ i ] .font = None;
      }
      XDrawText ( ( Display * ) WC ( D )->Dsp , WC ( D )->ActWin , \
       ( GC ) WC ( D )->Gc , x , y , item , ln ) ;
      free ( item ) ;
#endif
//TCB
//   UpdateScreen();
  }
  void uiDraw_XString ( DIALOG *D , int x , int y , char *str , int gap ) {
      int ln , i;
      void *img = NULL;
      kgWC *wc;
#if 0
//   XDrawString(Dsp,ActWin,Gc,x,y,str,strlen(str)-1);
      wc = D->wc;
      ln = strlen ( str ) ;
      if ( ln <= 0 ) return;
      while ( str [ ln-1 ] <= ' ' ) ln--;
      if ( ln <= 0 ) return;
//   img = gphStringToImage(str,ln*10,20,3,wc->c_color);
      img = gphStringToImage ( str , ln*wc->GuiFontSize , \
           20 , wc->GuiFont , wc->c_color , -1 , ln ) ;
      if ( img != NULL ) {
          kgImage ( D , img , x , y-16 , ln*10 , 20 , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
      }
#else
      XTextItem *item;
      ln = strlen ( str ) ;
      if ( ln <= 0 ) return;
      while ( str [ ln-1 ] <= ' ' ) ln--;
      if ( ln <= 0 ) return;
      item = ( XTextItem * ) Malloc ( sizeof ( XTextItem ) *ln ) ;
      if ( item == NULL ) {
          printf ( "Error: IN MALLOC....\n" ) ;
          exit ( 1 ) ;
      }
      item [ 0 ] .chars = str;
      item [ 0 ] .nchars = 1;
      item [ 0 ] .delta = 0;
      item [ 0 ] .font = None;
      for ( i = 1;i < ln;i++ ) {
          item [ i ] .chars = str+i;
          item [ i ] .nchars = 1;
          item [ i ] .delta = gap;
          item [ i ] .font = None;
      }
      XDrawText ( ( Display * ) WC ( D )->Dsp , WC ( D )->ActWin , \
       ( GC ) WC ( D )->Gc , x , y , item , ln ) ;
      free ( item ) ;
#endif
//TCB
//   UpdateScreen();
  }
  void kgCloseUi ( DIALOG *D ) {
      pthread_mutex_unlock ( & ( WC ( D )->Dsplock ) ) ;
      pthread_mutex_destroy ( & ( WC ( D )->Dsplock ) ) ;
      if ( WC ( D )->IMAGE != NULL ) {
          XDestroyImage ( ( XImage * ) ( WC ( D )->IMAGE ) ) ;
          WC ( D )->IMAGE = NULL;
          XDestroyImage ( ( XImage * ) ( WC ( D )->IMAGEBAK ) ) ;
          WC ( D )->IMAGEBAK = NULL;
      }
      XFreePixmap ( ( Display * ) ( WC ( D )->Dsp ) , \
       ( Pixmap ) ( WC ( D )->Pix ) ) ;
      XFreePixmap ( ( Display * ) ( WC ( D )->Dsp ) , \
       ( Pixmap ) ( WC ( D )->Piximg ) ) ;
      XFreeGC ( ( Display * ) ( WC ( D )->Dsp ) , ( WC ( D )->Gc ) ) ;
      XDestroyWindow ( ( Display * ) ( WC ( D )->Dsp ) , \
       ( Window ) ( WC ( D )->Win ) ) ;
#if 1
//  printf("Calling XCloseDisplay\n");
//  fflush(stdout);
      XCloseDisplay ( ( Display * ) ( WC ( D )->Dsp ) ) ;
      WC ( D )->Dsp = NULL;
//  printf("Okay\n");
//  fflush(stdout);
#endif
  }
/*
  Internal routine used in backup_line
*/
  void copy_area ( kgWC *wc , int x1 , int y1 , \
       int x2 , int y2 , int i , int nx ) \
  {
      int xmin , ymin , xmax , ymax;
      if ( x1 < x2 ) {xmin = x1 , xmax = x2;}
      else {xmin = x2;xmax = x1;}
      if ( y1 < y2 ) {ymin = y1 , ymax = y2;}
      else {ymin = y2;ymax = y1;}
      XCopyArea ( wc->Dsp , wc->DspWin , wc->linebuf , wc->Gc , ( short ) xmin , \
       ( short ) ( ymin ) , ( short ) ( xmax-xmin+1 ) , \
       ( short ) ( ymax-ymin+1 ) , i*nx , 0 ) ;
  }
/*
  Backing up the area under a line
*/
#if 1
  void _uibackup_line ( kgWC *wc , int x1 , int y1 , int x2 , int y2 ) {
      int i , dx , dy , nb , nx , ny , length , width , xl , yl , xu , yu , nxp;
      float dxx , dyy;
      dx = x2-x1;
      dy = y2-y1;
      if ( abs ( dx ) > abs ( dy ) ) { nb = abs ( dy ) /100+1; }
      else { nb = abs ( dx ) /100+1;}
      ny = dy/nb , nx = dx/nb;
      if ( ny < 0 ) ny -= 2; else ny+= 2;
      if ( nx < 0 ) nx -= 2; else nx+= 2;
      nxp = abs ( nx ) +1;
      length = nb*nxp;
      width = abs ( ny ) +1;
      wc->linebuf = XCreatePixmap ( wc->Dsp , wc->DspWin , ( short ) \
       ( length ) , ( short ) ( width ) , wc->Dpth ) ;
      dxx = ( ( float ) dx ) / ( float ) nb;
      dyy = ( ( float ) dy ) / ( float ) nb;
      for ( i = 0;i < ( nb-1 ) ;i++ ) {
          xl = x1 +i*dxx;
          yl = y1 +i*dyy;
          copy_area ( wc , xl , yl , xl+nx , yl+ny , i , nxp ) ;
      }
      xl = x1 + ( nb-1 ) *dxx;
      yl = y1 + ( nb-1 ) *dyy;
      copy_area ( wc , xl , yl , x2 , y2 , nb-1 , nxp ) ;
      return;
  }
#endif
/*
  Internal routine used in restore_line
*/
  void rest_area ( kgWC *wc , int x1 , int y1 , \
       int x2 , int y2 , int i , int nx ) \
  {
      int xmin , ymin , xmax , ymax;
      if ( x1 < x2 ) {xmin = x1 , xmax = x2;}
      else {xmin = x2;xmax = x1;}
      if ( y1 < y2 ) {ymin = y1 , ymax = y2;}
      else {ymin = y2;ymax = y1;}
      XCopyArea ( wc->Dsp , wc->linebuf , wc->DspWin , \
           wc->Gc , i*nx , 0 , ( short ) \
       ( xmax-xmin+1 ) , ( short ) ( ymax-ymin+1 ) , \
       ( short ) xmin , ( short ) ( ymin ) ) ;
  }
/*
  Restoring area under a line
*/
  void _uirecover_line ( kgWC *wc , int x1 , int y1 , int x2 , int y2 ) {
      int i , dx , dy , nb , nx , ny , length , width , xl , yl , xu , yu , nxp;
      float dxx , dyy;
      dx = x2-x1;
      dy = y2-y1;
      if ( abs ( dx ) > abs ( dy ) ) { nb = abs ( dy ) /100+1; }
      else { nb = abs ( dx ) /100+1;}
      ny = dy/nb , nx = dx/nb;
      if ( ny < 0 ) ny -= 2; else ny+= 2;
      if ( nx < 0 ) nx -= 2; else nx+= 2;
      nxp = abs ( nx ) +1;
      length = nb*nxp;
      width = abs ( ny ) ;
      dxx = ( ( float ) dx ) / ( float ) nb;
      dyy = ( ( float ) dy ) / ( float ) nb;
      for ( i = 0;i < ( nb-1 ) ;i++ ) {
          xl = x1 +i*dxx;
          yl = y1 +i*dyy;
          rest_area ( wc , xl , yl , xl+nx , yl+ny , i , nxp ) ;
      }
      xl = x1 + ( nb-1 ) *dxx;
      yl = y1 + ( nb-1 ) *dyy;
      rest_area ( wc , xl , yl , x2 , y2 , nb-1 , nxp ) ;
      XFreePixmap ( wc->Dsp , wc->linebuf ) ;
      uiUpdateScreen ( wc ) ;
      return;
  }
  void ui_doublebuffer ( DIG *G ) {
      int tmpc;
      int EVGAX , EVGAY;
      DIALOG *D;
      kgWC *wc , *gwc;
      kgDC *dc;
      D = ( DIALOG * ) G->D;
      wc = D->wc;
      gwc = G->wc;
      dc = G->dc;
      EVGAY = wc->EVGAY;
      EVGAX = wc->EVGAX;
      if ( dc->DOUBLE ) return;
      wc->WIN = wcXCPix ( 0 , 0 , EVGAX , EVGAY ) ;
      tmpc = wc->c_color;
#if 1
      uiMakeImageTransparent ( wc->IMAGE , 0.0 ) ;
//     XPutImage(wc->Dsp,wc->WIN,wc->Gc,wc->IMAGE,0,0,0,0,EVGAX,EVGAY);
      _uiTileImage ( wc , wc->WIN , wc->IMAGE , EVGAX , EVGAY ) ;
#endif
      wc->ActWin = wc->WIN;
      wc->DspWin = wc->WIN;
      gwc->WIN = wc->WIN;
      gwc->ActWin = wc->WIN;
      wcset_clr ( wc , 0 ) ;
      XFillRectangle ( wc->Dsp , wc->ActWin , wc->Gc , 0 , 0 , EVGAX , EVGAY ) ;
      wcset_clr ( wc , tmpc ) ;
      dc->DOUBLE = 1;
      uiUpdateOff ( D ) ;
  }
  void ui_singlebuffer ( DIG *G ) {
      int EVGAY;
      DIALOG *D;
      kgWC *wc , *gwc;
      kgDC *dc;
      D = ( DIALOG * ) G->D;
      wc = WC ( D ) ;
      gwc = G->wc;
      dc = G->dc;
      EVGAY = D->evgay;
      if ( dc->DOUBLE != 1 ) return;
      wc->ActWin = wc->DspWin = wc->Pix;
      gwc->ActWin = gwc->DspWin = wc->Pix;
      XFreePixmap ( wc->Dsp , wc->WIN ) ;
      wc->Update = 1; // UpdateOn ( ) need not be used
      dc->DOUBLE = 0;
  }
  void ui_swapbuffers_old ( DIG *G ) {
      int x0 , y0 , x1 , y1;
      int EVGAY , EVGAX;
      DIALOG *D;
      kgWC *wc;
      kgDC *dc;
      D = ( DIALOG * ) G->D;
      wc = WC ( D ) ;
      dc = G->dc;
      EVGAX = G->x2-G->x1+1;
      EVGAY = abs ( G->y2-G->y1 ) +1;
      x0 = dc->vu_x1* ( float ) EVGAX;
      y1 = EVGAY-1 - dc->vu_y1* ( float ) EVGAY;
      x1 = dc->vu_x2* ( float ) EVGAX-1;
      y0 = EVGAY-1 - dc->vu_y2* ( float ) EVGAY+1;
      x0 += ( G->x1+D->xo ) ;
      x1 += ( G->x1+D->xo ) ;
      y0 += ( G->y1+D->yo ) ;
      y1 += ( G->y1+D->yo ) ;
      if ( G->D_ON == 0 ) {
          uiCopyResizedImage ( G ) ;
      }
      XCopyArea ( wc->Dsp , wc->WIN , wc->Pix , wc->Gc , x0 , y0 , \
       ( short ) ( x1-x0 ) , ( short ) ( y1-y0 ) , x0 , y0 ) ;
      uiUpdateOn ( D ) ;
      return;
  }
  void uiUpdateOff ( DIALOG *D ) {
      kgWC *wc;
      wc = WC ( D ) ;
      wc->Update = 0;
  }
  void uiUpdateOn ( DIALOG *D ) {
      kgWC *wc;
      wc = WC ( D ) ;
      wc->Update = 1;
      uiUpdateScreen ( WC ( D ) ) ;
  }
  void uiUpdateScreen ( kgWC *wc ) {
      XEvent event;
      int EVGAX , EVGAY;
      EVGAX = wc->EVGAX;
      EVGAY = wc->EVGAY;
#if 1
      if ( wc->Update ) {
          if ( wc->FullScreen ) {
              XCopyArea ( wc->Dsp , wc->Pix , wc->Win , wc->Gc , \
                   0 , 0 , EVGAX , EVGAY , 0 , 0 ) ;
//       XCopyArea(wc->Dsp,wc->ActWin,wc->Win,wc->Gc,0,0,EVGAX,EVGAY,0,0);
              XSync ( wc->Dsp , False ) ;
          }
          else {
              pthread_mutex_lock ( & ( wc->Dsplock ) ) ;
              XLockDisplay ( wc->Dsp ) ;
              XCopyArea ( wc->Dsp , wc->Pix , wc->Piximg , wc->Gc , \
                   0 , 0 , EVGAX , EVGAY , 0 , 0 ) ;
              XCopyArea ( wc->Dsp , wc->Piximg , wc->Win , wc->Gc , \
                   0 , 0 , EVGAX , EVGAY , 0 , 0 ) ;
              XSync ( wc->Dsp , False ) ;
              XUnlockDisplay ( wc->Dsp ) ;
              pthread_mutex_unlock ( & ( wc->Dsplock ) ) ;
          }
      }
#endif
      return;
  }
  void SetDirectDraw ( void ) {
#if 0
      ActWin = Win;
      DspWin = Win;
#endif
  }
  void SetIndirectDraw ( void ) {
#if 0
      ActWin = Pix;
      DspWin = Pix;
#endif
  }
  void ui_swapbuffers ( DIG *G ) {
      int x0 , y0 , x1 , y1 , i , j , pos , red , green , blue;
      unsigned long val1 , max = 1;
      double temp;
      float fac , fac1 , cmax , fac2;
      int EVGAX , EVGAY;
      int RedMask , GreenMask , BlueMask , RedMaskPos , GreenMaskPos , BlueMaskPos;
      DIALOG *D;
      kgWC *wc;
      XImage *uiImage;
      kgDC *dc;
      D = ( DIALOG * ) G->D;
      wc = WC ( D ) ;
      dc = G->dc;
      if ( dc->DOUBLE != 1 ) return;
      ui_swapbuffers_old ( G ) ;
      return;
  }
  void setparam ( int t ) {
      int ierr , op;
/* op = open("/dev/tty",O_RDWR|O_SYNC);*/
      op = 0;
      ierr = ioctl ( op , TCGETS , & tty ) ;
      if ( ierr == -1 ) printf ( "ERROR IN  IOCTL\n" ) ;
      oldc_c = tty.c_cflag;
      oldc_l = tty.c_lflag;
      oldc_i = tty.c_iflag;
      oldc_o = tty.c_oflag;
      oldc_c4 = tty.c_cc [ VMIN ] ;
      oldc_c5 = tty.c_cc [ VTIME ] ;
      tty.c_lflag = ISIG;
      tty.c_iflag = 0;
      tty.c_oflag = 0;
      tty.c_cc [ VMIN ] = t;
      tty.c_cc [ VTIME ] = 0;
      ierr = ioctl ( op , TCSETS , & tty ) ;
      if ( ierr == -1 ) printf ( "ERROR IN IOCTL\n" ) ;
      return;
  }
  void restore_old_mode ( void ) {
      int ierr , op = 0;
      tty.c_cflag = oldc_c;
      tty.c_lflag = oldc_l;
      tty.c_iflag = oldc_i;
      tty.c_oflag = oldc_o;
      tty.c_cc [ VMIN ] = oldc_c4;
      tty.c_cc [ VTIME ] = oldc_c5;
      ierr = ioctl ( op , TCSETSW , & tty ) ;
      if ( ierr == -1 ) printf ( "ERROR IN  IOCTL\n" ) ;
  }
  void get_scan_code ( Display *Dsp ) {
      int i , j , k = 0 , ch , off = 0 , ch1;
      KeySym *keysym;
      int K_min , K_max , code;
      XDisplayKeycodes ( Dsp , & K_min , & K_max ) ;
      keysym = XGetKeyboardMapping ( Dsp , K_min , K_max-K_min+1 , & code ) ;
      for ( i = 0;i < 256;i++ ) Revscan_code [ i ] = -1;
#if 0
      printf ( "K_min : %d K_max : %d,%d\n" , K_min , K_max , code ) ;
      for ( i = K_min;i <= K_max;i++ ) {
      }
//TCB 07
      XModifierKeymap *xm;
      KeyCode *mpt;
      xm = XGetModifierMapping ( Dsp ) ;
//      xm->max_keypermod = 1; //should not be done
      XSetModifierMapping ( Dsp , xm ) ;
      free ( xm ) ;
      xm = XGetModifierMapping ( Dsp ) ;
      k = 0;
      fprintf ( stderr , "Max mod: %d\n" , xm->max_keypermod ) ;
      for ( i = 0;i < xm->max_keypermod;i++ ) {
          mpt = ( xm->modifiermap+k ) ;
          fprintf ( stderr , " %d %d %d %d %d %d %d %d\n" , mpt [ 0 ] , mpt [ 1 ] , \
               mpt [ 2 ] , mpt [ 3 ] , mpt [ 4 ] , mpt [ 5 ] , mpt [ 6 ] , mpt [ 7 ] ) ;
          k += 8;
      }
#endif
#if 0
      printf ( "K_min : %d K_max : %d,%d\n" , K_min , K_max , code ) ;
      k = ( K_min-1 ) *code;
      k = 0;
      for ( i = K_min;i < K_max;i++ ) {
          printf ( "Kesym:%d  %d %x:%c %x:%c %x:%c\n" , i , k , keysym [ k ] , keysym [ k ] , \
               keysym [ k+1 ] , keysym [ k+1 ] , keysym [ k+2 ] , keysym [ k+2 ] ) ;
          k+= code;
      }
#endif
// For Shift characters from 161 onwards
      k = 0;
      for ( i = K_min;i < ( K_max-160 ) ;i++ ) {
          keysym [ k+160*code ] = keysym [ k+1 ] ;
          k = k+code;
      }
// For control charactres Cntl-A to Cntl-_
// Stored from 224 onwards
#if 0  // Not Needed
      k = 0;
      for ( i = K_min;i < ( K_max-224 ) ;i++ ) {
          keysym [ k+224*code ] = 0x0000+ ( ( i-K_min ) +1 ) ;
//    keysym[k+224*code]=0xff00|((i-K_min)+'A');
//    keysym[k+224*code]=0x1008ff00+(i-K_min)+1;
          k = k+code;
      }
#endif
      XChangeKeyboardMapping ( Dsp , K_min , code , keysym , K_max-K_min+1 ) ;
#if 0
      printf ( "K_min : %d K_max : %d,%d\n" , K_min , K_max , code ) ;
      k = ( K_min-1 ) *code;
      k = 0;
      for ( i = K_min;i < K_max;i++ ) {
          printf ( "Kesym:%d  %d %x:%c %x:%c %x:%c\n" , i , k , keysym [ k ] , keysym [ k ] , \
               keysym [ k+1 ] , keysym [ k+1 ] , keysym [ k+2 ] , keysym [ k+2 ] ) ;
          k+= code;
      }
#endif
      k = 0;
//  for(i=K_min;i<(256);i++) {
      for ( i = K_min;i < ( K_max ) ;i++ ) {
          ch = keysym [ k ] ;
          k+= code;
          if ( ch != NoSymbol ) Scan_code [ i ] = ch;
          if ( ch != NoSymbol ) switch ( ch ) {
              case Home:
              if ( Revscan_code [ HomeKey ] < 0 ) Revscan_code [ HomeKey ] = i;
              break;
              case Return:
              if ( Revscan_code [ ReturnKey ] < 0 ) Revscan_code [ ReturnKey ] = i;
              break;
              case Linefeed:
              if ( Revscan_code [ LinefeedKey ] < 0 ) Revscan_code [ LinefeedKey ] = i;
              break;
              case Escape:
              if ( Revscan_code [ EscapeKey ] < 0 ) Revscan_code [ EscapeKey ] = i;
              break;
              case Control:
              if ( Revscan_code [ CntlKey ] < 0 ) Revscan_code [ CntlKey ] = i;
              break;
              case Alt:
              if ( Revscan_code [ AltKey ] < 0 ) Revscan_code [ AltKey ] = i;
              break;
              case Tab:
              if ( Revscan_code [ TabKey ] < 0 ) Revscan_code [ TabKey ] = i;
              break;
              case End:
              if ( Revscan_code [ EndKey ] < 0 ) Revscan_code [ EndKey ] = i;
              break;
              case Delete:
              if ( Revscan_code [ DeleteKey ] < 0 ) Revscan_code [ DeleteKey ] = i;
              break;
              case Insert:
              if ( Revscan_code [ InsertKey ] < 0 ) Revscan_code [ InsertKey ] = i;
              break;
              case Clear:
              if ( Revscan_code [ ClearKey ] < 0 ) Revscan_code [ ClearKey ] = i;
              break;
              case Print:
              if ( Revscan_code [ PrintKey ] < 0 ) Revscan_code [ PrintKey ] = i;
              break;
              case Backspace:
              if ( Revscan_code [ BackspaceKey ] < 0 ) Revscan_code [ BackspaceKey ] = i;
              break;
              case Left_Arrow:
              if ( Revscan_code [ LeftKey ] < 0 ) Revscan_code [ LeftKey ] = i;
              break;
              case Right_Arrow:
              if ( Revscan_code [ RightKey ] < 0 ) Revscan_code [ RightKey ] = i;
              break;
              case Up_Arrow:
              if ( Revscan_code [ UpKey ] < 0 ) Revscan_code [ UpKey ] = i;
              break;
              case Down_Arrow:
              if ( Revscan_code [ DownKey ] < 0 ) Revscan_code [ DownKey ] = i;
              break;
              case Shiftleft:
              if ( Revscan_code [ ShiftKey ] < 0 ) Revscan_code [ ShiftKey ] = i;
//        fprintf(istderr,"Got Shift Left key %d\n",i);
              break;
              case Shiftright:
              if ( Revscan_code [ ShiftRightKey ] < 0 ) Revscan_code [ ShiftRightKey ] = i;
              break;
              case Shiftlock:
              if ( Revscan_code [ ShiftLockKey ] < 0 ) Revscan_code [ ShiftLockKey ] = i;
              break;
              case Capslock:
              if ( Revscan_code [ CapslockKey ] < 0 ) Revscan_code [ CapslockKey ] = i;
              break;
              case Pageup:
              if ( Revscan_code [ PageupKey ] < 0 ) Revscan_code [ PageupKey ] = i;
              break;
              case Pagedown:
              if ( Revscan_code [ PagedownKey ] < 0 ) Revscan_code [ PagedownKey ] = i;
              break;
              case Space:
              if ( Revscan_code [ SpaceKey ] < 0 ) Revscan_code [ SpaceKey ] = i;
              break;
              default :
              if ( ch < 128 ) {
//            if(ch==32) printf("Got Space as Ascii\n");
                  if ( Revscan_code [ ch ] < 0 ) Revscan_code [ ch ] = i;
              }
              break;
          }
      }
      k = 0;
//  for(i=K_min;i<(256);i++) {
      for ( i = K_min;i < ( K_max ) ;i++ ) {
          ch1 = keysym [ k+1 ] ;
          k += code;
          if ( ch1 != NoSymbol ) {
              Scan_sh_code [ i ] = ch1;
          }
          if ( ch1 != NoSymbol ) {
#if 1
              switch ( ch1 ) {
                  case Home:
                  if ( Revscan_code [ HomeKey ] < 0 ) Revscan_code [ HomeKey ] = i;
                  break;
                  case Return:
                  if ( Revscan_code [ ReturnKey ] < 0 ) Revscan_code [ ReturnKey ] = i;
                  break;
                  case Linefeed:
                  if ( Revscan_code [ LinefeedKey ] < 0 ) Revscan_code [ LinefeedKey ] = i;
                  break;
                  case Escape:
                  if ( Revscan_code [ EscapeKey ] < 0 ) Revscan_code [ EscapeKey ] = i;
                  break;
                  case End:
                  if ( Revscan_code [ EndKey ] < 0 ) Revscan_code [ EndKey ] = i;
                  break;
                  case Clear:
                  if ( Revscan_code [ ClearKey ] < 0 ) Revscan_code [ ClearKey ] = i;
                  break;
                  case Tab:
                  if ( Revscan_code [ TabKey ] < 0 ) Revscan_code [ TabKey ] = i;
                  break;
                  case Backspace:
                  if ( Revscan_code [ BackspaceKey ] < 0 ) Revscan_code [ BackspaceKey ] = i;
                  break;
                  case Left_Arrow:
                  if ( Revscan_code [ LeftKey ] < 0 ) Revscan_code [ LeftKey ] = i;
                  break;
                  case Right_Arrow:
                  if ( Revscan_code [ RightKey ] < 0 ) Revscan_code [ RightKey ] = i;
                  break;
                  case Up_Arrow:
                  if ( Revscan_code [ UpKey ] < 0 ) Revscan_code [ UpKey ] = i;
                  break;
                  case Down_Arrow:
                  if ( Revscan_code [ DownKey ] < 0 ) Revscan_code [ DownKey ] = i;
                  break;
                  case Shiftleft:
                  if ( Revscan_code [ ShiftKey ] < 0 ) Revscan_code [ ShiftKey ] = i;
                  break;
                  case Shiftright:
                  if ( Revscan_code [ ShiftRightKey ] < 0 ) Revscan_code [ ShiftRightKey ] = i;
                  break;
                  case Shiftlock:
                  if ( Revscan_code [ ShiftLockKey ] < 0 ) Revscan_code [ ShiftLockKey ] = i;
                  break;
                  case Capslock:
                  if ( Revscan_code [ CapslockKey ] < 0 ) Revscan_code [ CapslockKey ] = i;
                  break;
                  case Space:
                  if ( Revscan_code [ SpaceKey ] < 0 ) Revscan_code [ SpaceKey ] = i;
                  break;
                  case Delete:
                  if ( Revscan_code [ DeleteKey ] < 0 ) Revscan_code [ DeleteKey ] = i;
                  break;
                  default :
                  if ( ch1 < 128 ) {
                      if ( Revscan_code [ ch1 ] < 0 ) Revscan_code [ ch1 ] = i+128;
                  }
                  break;
              }
#else
              if ( ch1 < 128 ) {
                  if ( Revscan_code [ ch1 ] < 0 ) Revscan_code [ ch1 ] = i+128;
              }
#endif
          }
      }
// For Control Characters
      for ( i = 224;i < ( K_max-K_min ) ;i++ ) {
          Revscan_code [ i ] = i+K_min;
      }
//TCB 07
#if 0
      for ( i = 0;i < K_max;i++ ) {
          k = Revscan_code [ i ] ;
          if ( k == -1 ) continue;
          ch = ' ';
//    if(k>=128) ch= Scan_sh_code[k-128];
          ch = Scan_code [ k ] ;
          printf ( "%d: %d %x:%c \n" , i , k , ch , ch ) ;
      }
#endif
      XFree ( keysym ) ;
  }
#if 0
  short key_press ( int *c ) { /* Modification as on 29-12-99 */
      int ch;
      int keypress = -1 , i , code;
      int *scan;
      XEvent event;
      while ( XCheckMaskEvent ( Dsp , KeyPress | KeyRelease , & event ) ) {
          code = event.xkey.keycode;
          if ( ( event.type == KeyPress ) && Shift_key ) { Shift = 1;continue;}
          if ( ( event.type == KeyRelease ) && Shift_key ) { Shift = 0;continue;}
          if ( ( event.type == KeyPress ) && Capslock_key ) \
          { Caplock = ( Caplock+1 ) %2;continue;}
          if ( ( event.type == KeyRelease ) && Capslock_key ) { continue;}
          if ( event.type == KeyPress ) {keypress = 1;break;}
          else continue;
      }
      if ( keypress <= 0 ) return ( -1 ) ;
      if ( Shift == 0 ) scan = Scan_code;
      else scan = Scan_sh_code;
      ch = scan [ code ] ;
      if ( ( ch != 0 ) && ( Caplock != 0 ) ) {
          if ( ( Shift != 0 ) && ( ch >= 'A' ) && ( ch <= 'Z' ) ) ch+= ( 'a'-'A' ) ;
          if ( ( Shift == 0 ) && ( ch >= 'a' ) && ( ch <= 'z' ) ) ch-= ( 'a'-'A' ) ;
      }
      *c = ch;
      return ( keypress ) ;
  }
#endif
  short ui_get_kb ( void ) {
      int ch , ch1;
      int i;
      union kbinp { short kbint; char kbc [ 2 ] ;} kb;
      kb.kbint = 0;
#if 0
      XSync ( Dsp , False ) ;
      if ( key_press ( & ch ) < 0 ) return ( -1 ) ;
      if ( ch == 0 ) return ( -1 ) ;
      if ( ch > 126 ) { kb.kbc [ 0 ] = 0;
          switch ( ch ) {
              case Up_Arrow :ch = 'H';break;
              case Up_Kp_Arrow :ch = 'H';break;
              case Down_Arrow :ch = 'P';break;
              case Down_Kp_Arrow :ch = 'P';break;
              case Right_Arrow :ch = 'M';break;
              case Right_Kp_Arrow :ch = 'M';break;
              case Left_Arrow :ch = 'K';break;
              case Left_Kp_Arrow :ch = 'K';break;
              case Tab :
              case Backspace :
              case Linefeed :
              case Return :
              case Escape :
              case Delete :
              kb.kbc [ 0 ] = ch & 0x00FF;
              break;
              default:
              Bell ( ) ;
              break;
          }
          kb.kbc [ 1 ] = ch & 0x00FF;
      }
      else {
          kb.kbc [ 0 ] = ch & 0x00FF;
      }
#endif
      return ( kb.kbint ) ;
  }
  char getch ( void ) {
#if 0
      int ierr;
      char ch;
      union kbinp { short kbint; char kbc [ 2 ] ;} kb;
      while ( ( kb.kbint = ui_get_kb ( ) ) <= 0 ) {
//    CheckWindowExposure();
          usleep ( 50000 ) ;
      }
      ch = kb.kbc [ 0 ] ;
      return ( ch ) ;
#else
#if 0
      KBEVENT kb;
      kb = ui_getevent ( ) ;
      while ( ( kb.event != 2 ) && ( kb.event != 5 ) ) kb = ui_getevent ( ) ;
      if ( kb.event == 5 ) return kb.key;
      else return kb.button;
#endif
#endif
  }
/********************************************************
  Mouse routines
*********************************************************/
  void kgChangeCursor ( DIALOG *D , int n ) {
      kgWC *wc;
      Cursor Cur;
      wc = D->wc;
      Cur = XCreateFontCursor ( wc->Dsp , n ) ;
      wc->CurrCur = Cur;
      XDefineCursor ( wc->Dsp , wc->Win , Cur ) ;
  }
  int GetPointer ( int *x , int *y ) {
#if 0
      Window Root , Root1 , Child , Curwin;
      int x0 , y0;
      unsigned int wd0 , ht0 , bwd0 , depth0;
      int x1 , y1 , x2 , y2;
      unsigned int pb , pbb , key , i;
      unsigned int wd1 , ht1 , bwd1 , depth1;
      XEvent e;
      XButtonEvent b;
      XKeyEvent k;
      KBEVENT kbe;
      kbe = ui_getevent ( ) ;
      *x = kbe.x;
      *y = kbe.y;
      if ( kbe.event == 2 ) return kbe.button;
      else return 0;
      while ( 1 ) {
          XWindowEvent ( Dsp , Win , EventMask , & e ) ;
          switch ( e.type ) {
              case Expose:
              pthread_mutex_lock ( & _Dsplock ) ;
              ExposeWin = 1;
              pthread_mutex_unlock ( & _Dsplock ) ;
              continue;
              case NoExpose:
              continue;
              case ButtonPress:
              b = * ( ( XButtonEvent * ) & e ) ;
              printf ( "Mouse Button: %d:%d\n" , b.button , b.state ) ;
              *x = b.x;
              *y = b.y;
              continue;
              case ButtonRelease:
              b = * ( ( XButtonEvent * ) & e ) ;
              printf ( "Mouse Button: %d:%d\n" , b.button , b.state ) ;
              *x = b.x;
              *y = b.y;
              return b.button;
              case KeyPress:
              printf ( "Key Pressed\n" ) ;
              continue;
              case KeyRelease:
              break;
              case MotionNotify:
              b = * ( ( XButtonEvent * ) & e ) ;
              printf ( "Mouse Moved: %d:%d\n" , b.button , b.state/256 ) ;
              *x = b.x;
              *y = b.y;
              return 0;
              default:
              pthread_mutex_lock ( & _Dsplock ) ;
              ExposeWin = 1;
              pthread_mutex_unlock ( & _Dsplock ) ;
              continue;
          }
      }
      while ( ! input_attention ( ) ) {usleep ( 300 ) ;}
      Curwin = GetParent ( Win ) ;
      if ( XQueryPointer ( Dsp , Curwin , & Root , & Child , \
           & x1 , & y1 , x , y , & pb ) == False ) return \
       ( -100 ) ;
      if ( Child != Win ) { return ( -1 ) ; };
      key = 0;
      pbb = pb;if ( ( pbb &= 0x0100 ) != 0 ) key = 1;
      pbb = pb;if ( ( pbb &= 0x0200 ) != 0 ) key = 2;
      pbb = pb;if ( ( pbb &= 0x0400 ) != 0 ) key = 3;
      pbb = pb;if ( ( pbb &= 0x0500 ) == 0x0500 ) key = 2;
      XGetGeometry ( Dsp , Win , & Root , & x0 , & y0 , \
           & wd0 , & ht0 , & bwd0 , & depth0 ) ;
      *x -= ( x0 ) ;*y-= ( y0 ) ;
      if ( ( *x >= 0 ) && ( *y >= 0 ) && ( *y < ht0 ) && \
       ( *x < wd0 ) ) {return ( key ) ;}
      else{ return ( -1 ) ;}
#endif
  }
  int ui_Pageup ( int ch ) {
      int ret = 0;
      if ( ( ch == Pageup ) || ( ch == Kp_Pageup ) ) ret = 1;
      return ret;
  }
  int ui_Pagedown ( int ch ) {
      int ret = 0;
      if ( ( ch == Pagedown ) || ( ch == Kp_Pagedown ) ) ret = 1;
      return ret;
  }
  int ui_End ( int ch ) {
      int ret = 0;
      if ( ( ch == End ) || ( ch == Kp_End ) ) ret = 1;
      return ret;
  }
  int ui_Home ( int ch ) {
      int ret = 0;
      if ( ( ch == Home ) || ( ch == Kp_Home ) ) ret = 1;
      return ret;
  }
  int ui_Uparrow ( int ch ) {
      int ret = 0;
      if ( ( ch == Up_Arrow ) || ( ch == Up_Kp_Arrow ) ) ret = 1;
      return ret;
  }
  int ui_Downarrow ( int ch ) {
      int ret = 0;
      if ( ( ch == Down_Arrow ) || ( ch == Down_Kp_Arrow ) ) ret = 1;
      return ret;
  }
  int ui_Leftarrow ( int ch ) {
      int ret = 0;
      if ( ( ch == Left_Arrow ) || ( ch == Left_Kp_Arrow ) ) ret = 1;
      return ret;
  }
  int ui_Rightarrow ( int ch ) {
      int ret = 0;
      if ( ( ch == Right_Arrow ) || ( ch == Right_Kp_Arrow ) ) ret = 1;
      return ret;
  }
  int ui_Tab ( int ch ) {
      int ret = 0;
      if ( ( ch == Tab ) ) ret = 1;
      return ret;
  }
  int ui_Backspace ( int ch ) {
      int ret = 0;
      if ( ( ch == Backspace ) ) ret = 1;
      return ret;
  }
  int ui_Linefeed ( int ch ) {
      int ret = 0;
      if ( ( ch == Linefeed ) ) ret = 1;
      return ret;
  }
  int ui_Return ( int ch ) {
      int ret = 0;
      if ( ( ch == Return ) ) ret = 1;
      return ret;
  }
  int ui_Escape ( int ch ) {
      int ret = 0;
      if ( ( ch == Escape ) ) ret = 1;
      return ret;
  }
  int ui_Clear ( int ch ) {
      int ret = 0;
      if ( ( ch == Clear ) ) ret = 1;
      if ( ( ch == Print ) ) ret = 1;
      return ret;
  }
  int ui_Delete ( int ch ) {
      int ret = 0;
      if ( ( ch == Delete ) ) ret = 1;
      if ( ( ch == Kp_Delete ) ) ret = 1;
      return ret;
  }
  void kgPushBackEvent ( DIALOG *D ) {
      kgWC *wc;
      wc = WC ( D ) ;
      XEvent *e;
      if ( wc->eventback.type < 0 ) return;
      e = ( XEvent * ) Malloc ( sizeof ( XEvent ) ) ;
      *e = wc->eventback;
      XPutBackEvent ( wc->Dsp , e ) ;
      wc->eventback.type = -100;
      return;
  }
  void kgGetWindowSize ( DIALOG *D , int *length , int *height ) {
      int bw , dpth , x0 , y0;
      kgWC *wc;
      Window win;
      wc = WC ( D ) ;
      XGetGeometry ( wc->Dsp , wc->Win , & win , & x0 , \
           & y0 , length , height , & bw , & dpth ) ;
      return;
  }
  KBEVENT kgGetEvent ( DIALOG *D ) {
      long BUTNMASK;
      int code , ch , *scan;
      int x , y , w , h , bw , dpth;
      Window win;
      Time time0 , time1;
      KBEVENT kbevent;
      XWindowAttributes xwa;
      XEvent e , eo;
      XButtonEvent b;
      XMotionEvent B , Btmp;
      XResizeRequestEvent R;
      XConfigureEvent xce;
      XKeyEvent k;
      kgWC *wc;
      wc = WC ( D ) ;
      BUTNMASK = ( Button1Mask | Button2Mask | Button3Mask \
       | Button4Mask | Button5Mask ) ;
      while ( 1 ) {
          XWindowEvent ( wc->Dsp , wc->Win , EventMask , & e ) ;
          wc->event = e;
          switch ( e.type ) {
              case ResizeRequest:
//          XPeekEvent(wc->Dsp,&eo);
              while ( XCheckTypedEvent ( wc->Dsp , ResizeRequest , & eo ) ) {
//          while((eo.type == ResizeRequest)||(eo.type ==MotionNotify))  {
                  if ( eo.type == ResizeRequest ) {
                      e = eo;
                      XNextEvent ( wc->Dsp , & eo ) ;
                      XPeekEvent ( wc->Dsp , & eo ) ;
                  }
              }
              R = * ( ( XResizeRequestEvent * ) & e ) ;
              if ( ! _uiAdjustResize ( D , R.width , R.height ) ) continue;
              kbevent.event = 101;
              kbevent.x = R.width;
              kbevent.y = R.height;
              XGetGeometry ( wc->Dsp , wc->Win , & win , & x , \
                   & y , & w , & h , & bw , & dpth ) ;
              return kbevent;
              case ConfigureRequest:
              continue;
              case ConfigureNotify:
              while ( XCheckTypedEvent ( wc->Dsp , ConfigureNotify , & eo ) ) {e = eo;}
              xce = e.xconfigure;
//          printf("Got Configure Notify %d %d:%d %d \n",D->xl,D->yl,xce.width,xce.height);
              if ( ! _uiAdjustResize ( D , xce.width , xce.height ) ) continue;
//          printf("Got Configure Notify %d %d:%d %d \n",D->xl,D->yl,xce.width,xce.height);
              kbevent.event = 101;
              kbevent.x = xce.width;
              kbevent.y = xce.height;
              return kbevent;
              case Expose:
              pthread_mutex_lock ( & ( wc->Dsplock ) ) ;
              wc->ExposeWin = 1;
              pthread_mutex_unlock ( & ( wc->Dsplock ) ) ;
              continue;
              case NoExpose:
              continue;
              case ButtonPress:
              b = * ( ( XButtonEvent * ) & e ) ;
//          printf("Mouse Button: %d:%d\n",b.button,b.state);
              kbevent.event = 1;
              kbevent.button = b.button;
              kbevent.x = b.x;
              kbevent.y = b.y;
              kbevent.rootx = b.x_root;
              kbevent.rooty = b.y_root;
              return kbevent;
              case ButtonRelease:
              b = * ( ( XButtonEvent * ) & e ) ;
//          printf("Mouse Button: %d:%d\n",b.button,b.state);
              kbevent.event = 2;
              kbevent.button = b.button;
              kbevent.x = b.x;
              kbevent.y = b.y;
              kbevent.rootx = b.x_root;
              kbevent.rooty = b.y_root;
              return kbevent;
              case KeyPress:
              k = * ( ( XKeyEvent * ) & e ) ;
              code = k.keycode;
              if ( Shift_key ) { Shift = 1;continue;}
              if ( Capslock_key ) { Caplock = ( Caplock+1 ) %2;continue;}
//          printf("Key Pressed: %d:%d\n",k.keycode,k.state);
              kbevent.event = 4;
              continue;
              case KeyRelease:
//          printf("Key Released: %d:%d\n",k.keycode,k.state);
              k = * ( ( XKeyEvent * ) & e ) ;
              code = k.keycode;
              if ( Shift_key ) { Shift = 0;continue;}
              if ( Capslock_key ) { continue;}
              kbevent.event = 5;
              if ( Shift == 0 ) scan = Scan_code;
              else scan = Scan_sh_code;
              ch = scan [ code ] ;
              if ( ( ch != 0 ) && ( Caplock != 0 ) ) {
                  if ( ( Shift != 0 ) && ( ch >= 'A' ) && ( ch <= 'Z' ) ) ch+= ( 'a'-'A' ) ;
                  if ( ( Shift == 0 ) && ( ch >= 'a' ) && ( ch <= 'z' ) ) ch-= ( 'a'-'A' ) ;
              }
              kbevent.key = ch;
//          if(ui_Uparrow(ch)) printf("UpArrow\n");
//          if(ui_Tab(ch)) printf("Tab\n");
              return kbevent;
              case MotionNotify:
              B = * ( ( XMotionEvent * ) & e ) ;
//          printf("Mouse Moved: %d:%d\n",b.button,b.state);
              if ( B.state & ( Button1Mask | Button2Mask | Button3Mask \
               | Button4Mask | Button5Mask ) ) {
                  kbevent.event = 3;
              }
              else kbevent.event = 0;
              kbevent.x = B.x;
              kbevent.y = B.y;
              kbevent.rootx = B.x_root;
              kbevent.rooty = B.y_root;
              time0 = B.time;
#if 1
              if ( kbevent.event == 3 ) {
                  while ( 1 ) {
                      XPeekEvent ( wc->Dsp , & eo ) ;
                      while ( ( eo.type != MotionNotify ) && ( eo.type != ButtonRelease ) ) {
                          XNextEvent ( wc->Dsp , & eo ) ;
                          XPeekEvent ( wc->Dsp , & eo ) ;
                      }
                      if ( eo.type == MotionNotify ) {
                          XNextEvent ( wc->Dsp , & e ) ;
#if 0
                          if ( XCheckWindowEvent ( wc->Dsp , wc->Win , EventMask , & e ) ) {
                              while ( e.type != MotionNotify ) {
                                  XWindowEvent ( wc->Dsp , wc->Win , EventMask , & e ) ;
                              }
                          }
#endif
                          Btmp = * ( ( XMotionEvent * ) & e ) ;
                          kbevent.x = Btmp.x;
                          kbevent.y = Btmp.y;
                          if ( ( Btmp.time -time0 ) > 25 ) { break;}
                      }
                      else {
//                printf("Not MotionNotify(%d): %d\n",MotionNotify,eo.type );
                          b = * ( ( XButtonEvent * ) & eo ) ;
                          kbevent.button = b.button;
                          kbevent.x = b.x;
                          kbevent.y = b.y;
                          break;
                      }
                  }
              }
#endif
              kbevent.button = 0;
              if ( B.state & Button1Mask ) kbevent.button += 1;
              if ( B.state & Button2Mask ) kbevent.button += 2;
              if ( B.state & Button3Mask ) kbevent.button += 4;
              if ( B.state & Button4Mask ) kbevent.button += 8;
              if ( B.state & Button5Mask ) kbevent.button += 16;
              return kbevent;
              default:
              pthread_mutex_lock ( & ( wc->Dsplock ) ) ;
              wc->ExposeWin = 1;
              pthread_mutex_unlock ( & ( wc->Dsplock ) ) ;
              continue;
          }
      }
  }
  int kgRespondSelection ( void *Tmp , XEvent evt ) {
      DIALOG *D = ( DIALOG * ) Tmp;
      kgWC *wc;
      XEvent res; /* response to event */
      Window win;
      Atom pty , target , prop , sel;
      Atom prop_type;
      int prop_format , count = 0;
      unsigned long prop_size , prop_items;
      wc = WC ( D ) ;
      unsigned long len;
      unsigned char *txt;
      target = XA_STRING;
      sel = evt.xselectionrequest.selection;
      if ( evt.type != SelectionRequest ) return ( 0 ) ;
      win = evt.xselectionrequest.requestor;
      pty = evt.xselectionrequest.property;
      prop = None;
      if ( sel == XA_PRIMARY ) {
//      printf("pty is Primary\n");
          prop = XInternAtom ( wc->Dsp , "XPRI_IN" , True ) ;
      }
      else {
//     printf("pty is Clipboard\n");
          prop = XInternAtom ( wc->Dsp , "XCLIP_IN" , True ) ;
      }
      XGetWindowProperty ( wc->Dsp , wc->Win , prop , 0 , 0 , False , AnyPropertyType , \
           & prop_type , & prop_format , & prop_items , & prop_size , & txt ) ;
      XFree ( txt ) ;
      txt = NULL;
      len = prop_size;
//   printf("len = %d\n",len);
      XGetWindowProperty ( wc->Dsp , wc->Win , prop , 0 , len , False , AnyPropertyType , \
           & prop_type , & prop_format , & prop_items , & prop_size , & txt ) ;
      XChangeProperty ( wc->Dsp , win , pty , target , 8 , PropModeReplace , \
       ( unsigned char * ) txt , ( int ) len ) ;
      XFree ( txt ) ;
      txt = NULL;
	/* set values for the response event */
      res.xselection.property = pty;
      res.xselection.type = SelectionNotify;
      res.xselection.display = evt.xselectionrequest.display;
      res.xselection.requestor = win;
      res.xselection.selection = evt.xselectionrequest.selection;
      res.xselection.target = target;
      res.xselection.time = evt.xselectionrequest.time;
      XSendEvent ( wc->Dsp , evt.xselectionrequest.requestor , 0 , 0 , & res ) ;
      XFlush ( wc->Dsp ) ;
      return ( 1 ) ;
  }
  Bool _uiselection ( Display *dsp , XEvent *e , char *arg ) {
      if ( ( e->type == SelectionNotify ) || ( e->type == SelectionRequest ) ) return True;
          
      else return False;
  }
  int kgWaitSelection ( void *Tmp ) {
      int code , ch , *scan , ret = 0;
      int x , y , w , h , bw , dpth;
      DIALOG *D;
      Window win;
      XEvent e , eo;
      kgWC *wc;
      D = ( DIALOG * ) Tmp;
      wc = WC ( D ) ;
      int count;
      while ( 1 ) {
/* Need Checking */
//    XNextEvent(wc->Dsp,&e);
//    if(!XCheckIfEvent(wc->Dsp,&e,_uiselection,NULL)) return -1;
          count = 0;
          while ( ! ( XCheckTypedWindowEvent ( wc->Dsp , \
               wc->Win , SelectionNotify , & e ) ) ) \
          {
              usleep ( 500 ) ;
              count++;
              if ( count > 3 ) return -1;
          }
          eo = e;
          while ( XCheckTypedWindowEvent ( wc->Dsp , \
               wc->Win , SelectionNotify , & e ) ) \
          {
              eo = e;
          }
          e = eo;
          switch ( e.type ) {
              case SelectionNotify:
              if ( e.xselection.property == None ) return 0;
              else return 1;
              break;
#if 0
              case SelectionRequest:
              printf ( "Got Selection Request\n" ) ;
              kgRespondSelection ( Tmp , e ) ;
              return 2;
              break;
#endif
              default:
              continue;
          }
      }
  }
  void * kgProcessSelectionRequest ( void *Tmp ) {
      int code , ch , *scan , ret = 0;
      int x , y , w , h , bw , dpth;
      DIALOG *D;
      XEvent e , eo;
      kgWC *wc;
      D = ( DIALOG * ) Tmp;
      wc = WC ( D ) ;
      while ( 1 ) {
/* Need Checking */
//    XNextEvent(wc->Dsp,&e);
//    if(!XCheckIfEvent(wc->Dsp,&e,_uiselection,NULL)) return -1;
          if ( ! XCheckTypedWindowEvent ( wc->Dsp , \
               wc->Win , SelectionRequest , & e ) ) \
          {
              usleep ( 400 ) ;
              continue;
          }
          switch ( e.type ) {
              case SelectionRequest:
//            printf("Got Selection Request\n");
              pthread_mutex_lock ( & ( WC ( D )->Rlock ) ) ;
              kgRespondSelection ( Tmp , e ) ;
              pthread_mutex_unlock ( & ( WC ( D )->Rlock ) ) ;
              break;
              default:
              break;
          }
      }
  }
  int kgCheckMousePressEvent ( DIALOG *D , KBEVENT *kbe ) {
      int code , ch , *scan , ret = 0;
      int x , y , w , h , bw , dpth;
      Window win;
      XWindowAttributes xwa;
      KBEVENT kbevent;
      XEvent e , eo;
      XButtonEvent b;
      XKeyEvent k;
      XResizeRequestEvent R;
      XConfigureEvent xce;
      kgWC *wc;
      wc = WC ( D ) ;
      while ( 1 ) {
//    if(!XCheckWindowEvent(wc->Dsp,wc->Win,EventMask,&e))return 0;
          if ( ! XCheckWindowEvent ( wc->Dsp , wc->Win , ButtonMask , & e ) ) return 0;
          wc->event = e;
          switch ( e.type ) {
              case ResizeRequest:
              R = * ( ( XResizeRequestEvent * ) & e ) ;
              kbevent.event = 101;
              kbevent.x = R.width;
              kbevent.y = R.height;
              _uiAdjustResize ( D , R.width , R.height ) ;
              XGetGeometry ( wc->Dsp , wc->Win , & win , & x , \
                   & y , & w , & h , & bw , & dpth ) ;
              continue;
              case ConfigureRequest:
              continue;
              case ConfigureNotify:
              while ( XCheckTypedEvent ( wc->Dsp , ConfigureNotify , & eo ) ) {e = eo;}
              xce = e.xconfigure;
//          printf("Got Configure Notify %d %d:%d %d \n",xce.width,xce.height,wc->EVGAX,wc->EVGAY);
              if ( ! _uiAdjustResize ( D , xce.width , xce.height ) ) continue;
              kbevent.event = 101;
              kbevent.x = xce.width;
              kbevent.y = xce.height;
              *kbe = kbevent;
              return 1;
              case Expose:
              pthread_mutex_lock ( & ( wc->Dsplock ) ) ;
              wc->ExposeWin = 1;
              pthread_mutex_unlock ( & ( wc->Dsplock ) ) ;
              continue;
              case NoExpose:
              continue;
              case ButtonPress:
              b = * ( ( XButtonEvent * ) & e ) ;
//          printf("Mouse Button: %d:%d\n",b.button,b.state);
              kbevent.event = 1;
              kbevent.button = b.button;
              kbevent.x = b.x;
              kbevent.y = b.y;
              kbevent.rootx = b.x_root;
              kbevent.rooty = b.y_root;
              *kbe = kbevent;
              return 1;
              case ButtonRelease:
              kbevent.x = b.x;
              kbevent.y = b.y;
              kbevent.rootx = b.x_root;
              kbevent.rooty = b.y_root;
              *kbe = kbevent;
              continue;
              case KeyPress:
              continue;
              case KeyRelease:
//          printf("Key Released: %d:%d\n",k.keycode,k.state);
              continue;
              case MotionNotify:
              continue;
              default:
              pthread_mutex_lock ( & ( wc->Dsplock ) ) ;
              wc->ExposeWin = 1;
              pthread_mutex_unlock ( & ( wc->Dsplock ) ) ;
              continue;
          }
      }
  }
  int kgCheckEscape ( DIALOG *D ) {
      KBEVENT *kbe , kb;
      int code , ch , *scan , ret = 0;
      int x , y , w , h , bw , dpth;
      Window win;
      XWindowAttributes xwa;
      KBEVENT kbevent;
      XEvent e , eo;
      XButtonEvent b;
      XKeyEvent k;
      XResizeRequestEvent R;
      XConfigureEvent xce;
      kgWC *wc;
      wc = WC ( D ) ;
      kbe = & kb;
      while ( 1 ) {
          if ( ! XCheckWindowEvent ( wc->Dsp , wc->Win , KeyMask , & e ) ) return 0;
          wc->event = e;
          switch ( e.type ) {
              case ResizeRequest:
              R = * ( ( XResizeRequestEvent * ) & e ) ;
              kbevent.event = 101;
              kbevent.x = R.width;
              kbevent.y = R.height;
              _uiAdjustResize ( D , R.width , R.height ) ;
              XGetGeometry ( wc->Dsp , wc->Win , & win , & x , \
                   & y , & w , & h , & bw , & dpth ) ;
              continue;
              case ConfigureRequest:
              continue;
              case ConfigureNotify:
              while ( XCheckTypedEvent ( wc->Dsp , ConfigureNotify , & eo ) ) {e = eo;}
              xce = e.xconfigure;
//          printf("Got Configure Notify %d %d:%d %d \n",xce.width,xce.height,wc->EVGAX,wc->EVGAY);
              if ( ! _uiAdjustResize ( D , xce.width , xce.height ) ) continue;
              kbevent.event = 101;
              kbevent.x = xce.width;
              kbevent.y = xce.height;
              *kbe = kbevent;
              return 1;
              case Expose:
              pthread_mutex_lock ( & ( wc->Dsplock ) ) ;
              wc->ExposeWin = 1;
              pthread_mutex_unlock ( & ( wc->Dsplock ) ) ;
              continue;
              case NoExpose:
              *kbe = kbevent;
              continue;
              case ButtonPress:
              continue;
              case ButtonRelease:
              *kbe = kbevent;
              continue;
              case KeyPress:
              continue;
              case KeyRelease:
//          printf("Key Released: %d:%d\n",k.keycode,k.state);
              k = * ( ( XKeyEvent * ) & e ) ;
              code = k.keycode;
              if ( Shift_key ) { Shift = 0;continue;}
              if ( Capslock_key ) { continue;}
              kbevent.event = 5;
              if ( Shift == 0 ) scan = Scan_code;
              else scan = Scan_sh_code;
              ch = scan [ code ] ;
              if ( ( ch != 0 ) && ( Caplock != 0 ) ) {
                  if ( ( Shift != 0 ) && ( ch >= 'A' ) && ( ch <= 'Z' ) ) ch+= ( 'a'-'A' ) ;
                  if ( ( Shift == 0 ) && ( ch >= 'a' ) && ( ch <= 'z' ) ) ch-= ( 'a'-'A' ) ;
              }
              kbevent.key = ch;
//          if(ui_Uparrow(ch)) printf("UpArrow\n");
//          if(ui_Tab(ch)) printf("Tab\n");
              *kbe = kbevent;
              if ( ui_Escape ( kbevent.key ) ) return 1;
              continue;
              case MotionNotify:
              continue;
              default:
              pthread_mutex_lock ( & ( wc->Dsplock ) ) ;
              wc->ExposeWin = 1;
              pthread_mutex_unlock ( & ( wc->Dsplock ) ) ;
              continue;
          }
      }
  }
  int kgCheckRootEvent ( DIALOG *D , KBEVENT *kbe ) {
      long BUTNMASK;
      int code , ch , *scan , ret = 0 , count;
      int x , y , w , h , bw , dpthi , rx , ry;
      Window win , Root , Child = 0;
      XWindowAttributes xwa;
      Time time0;
      KBEVENT kbevent;
      XEvent e , eo;
      XButtonEvent b;
      XMotionEvent B , Btmp;
      XKeyEvent k;
      XResizeRequestEvent R;
      XConfigureEvent xce;
      unsigned int pb;
      kgWC *wc;
      wc = WC ( D ) ;
      win = RootWindow ( wc->Dsp , DefaultScreen ( wc->Dsp ) ) ;
      BUTNMASK = ( Button1Mask | Button2Mask | Button3Mask \
       | Button4Mask | Button5Mask ) ;
      if ( XQueryPointer ( wc->Dsp , wc->Parent , & Root , & Child , \
           & rx , & ry , & x , & y , & pb ) == False ) \
      {
          return -1;
      }
      kbe->rootx = rx;
      kbe->rooty = ry;
      kbe->x = x;
      kbe->y = y;
//  if( Child != wc->Win){ return(-1); };
      kbe->event = 0;
      kbe->button = 0;
      if ( pb & BUTNMASK ) {
          kbe->event = 1;
          if ( pb & Button1Mask ) kbe->button = 1;
          if ( pb & Button2Mask ) kbe->button = 2;
          if ( pb & Button3Mask ) kbe->button = 3;
          if ( pb & Button4Mask ) kbe->button = 4;
          if ( pb & Button5Mask ) kbe->button = 5;
          if ( ( wc->Win == Child ) ) ret = 1;
          else ret = 3;
      }
      else {
          if ( ( wc->Win == Child ) ) ret = 0;
          else ret = 2;
      }
      if ( ( wc->Win == Child ) ) {
          XQueryPointer ( wc->Dsp , wc->Win , & Root , & Child , \
               & rx , & ry , & x , & y , & pb ) ;
          kbe->rootx = rx;
          kbe->rooty = ry;
          kbe->x = x;
          kbe->y = y;
      }
      return ret;
  }
  int kgCheckEvent ( DIALOG *D , KBEVENT *kbe ) {
      long BUTNMASK;
      int code , ch , *scan , ret = 0 , count;
      int x , y , w , h , bw , dpth;
      Window win;
      XWindowAttributes xwa;
      Time time0;
      KBEVENT kbevent;
      XEvent e , eo;
      XButtonEvent b;
      XMotionEvent B , Btmp;
      XKeyEvent k;
      XResizeRequestEvent R;
      XConfigureEvent xce;
      kgWC *wc;
      wc = WC ( D ) ;
      BUTNMASK = ( Button1Mask | Button2Mask | Button3Mask \
       | Button4Mask | Button5Mask ) ;
      while ( 1 ) {
          if ( ! XCheckWindowEvent ( wc->Dsp , wc->Win , EventMask , & e ) ) return 0;
          wc->event = e;
          switch ( e.type ) {
              case ResizeRequest:
//          XPeekEvent(wc->Dsp,&eo);
#if 1
              while ( XCheckTypedEvent ( wc->Dsp , ResizeRequest , & eo ) ) {
//          while((eo.type == ResizeRequest)||(eo.type ==MotionNotify))  {
                  if ( eo.type == ResizeRequest ) {
                      e = eo;
                      XNextEvent ( wc->Dsp , & eo ) ;
                      XPeekEvent ( wc->Dsp , & eo ) ;
                  }
              }
              R = * ( ( XResizeRequestEvent * ) & e ) ;
              kbevent.event = 101;
              kbevent.x = R.width;
              kbevent.y = R.height;
              if ( ! _uiAdjustResize ( D , R.width , R.height ) ) continue;;
              XGetGeometry ( wc->Dsp , wc->Win , & win , & x , \
                   & y , & w , & h , & bw , & dpth ) ;
              *kbe = kbevent;
#else
              R = * ( ( XResizeRequestEvent * ) & e ) ;
              kbevent.event = 102;
              kbevent.x = R.width;
              kbevent.y = R.height;
              *kbe = kbevent;
#endif
              return 1;
              case ConfigureRequest:
              continue;
              case ConfigureNotify:
              while ( XCheckTypedEvent ( wc->Dsp , ConfigureNotify , & eo ) ) {e = eo;}
              xce = e.xconfigure;
//          printf("Got Configure Notify %d %d:%d %d \n",xce.width,xce.height,wc->EVGAX,wc->EVGAY);
              if ( ! _uiAdjustResize ( D , xce.width , xce.height ) ) continue;
              kbevent.event = 101;
              kbevent.x = xce.width;
              kbevent.y = xce.height;
              *kbe = kbevent;
              return 1;
              case Expose:
              pthread_mutex_lock ( & ( wc->Dsplock ) ) ;
              wc->ExposeWin = 1;
              pthread_mutex_unlock ( & ( wc->Dsplock ) ) ;
              continue;
              case NoExpose:
              continue;
              case ButtonPress:
              b = * ( ( XButtonEvent * ) & e ) ;
//          printf("Mouse Button: %d:%d\n",b.button,b.state);
              kbevent.event = 1;
              kbevent.button = b.button;
              kbevent.x = b.x;
              kbevent.y = b.y;
              kbevent.rootx = b.x_root;
              kbevent.rooty = b.y_root;
              *kbe = kbevent;
              return 1;
              case ButtonRelease:
              b = * ( ( XButtonEvent * ) & e ) ;
//          printf("Mouse Button: %d:%d\n",b.button,b.state);
              kbevent.event = 2;
              kbevent.button = b.button;
              kbevent.x = b.x;
              kbevent.y = b.y;
              kbevent.rootx = b.x_root;
              kbevent.rooty = b.y_root;
              *kbe = kbevent;
              return 1;
              case KeyPress:
              k = * ( ( XKeyEvent * ) & e ) ;
              code = k.keycode;
              if ( Shift_key ) { Shift = 1;continue;}
              if ( Capslock_key ) { Caplock = ( Caplock+1 ) %2;continue;}
//          printf("Key Pressed: %d:%d\n",k.keycode,k.state);
              kbevent.event = 4;
              continue;
              case KeyRelease:
//          printf("Key Released: %d:%d\n",k.keycode,k.state);
              k = * ( ( XKeyEvent * ) & e ) ;
              code = k.keycode;
              if ( Shift_key ) { Shift = 0;continue;}
              if ( Capslock_key ) { continue;}
              kbevent.event = 5;
              if ( Shift == 0 ) scan = Scan_code;
              else scan = Scan_sh_code;
              ch = scan [ code ] ;
              if ( ( ch != 0 ) && ( Caplock != 0 ) ) {
                  if ( ( Shift != 0 ) && ( ch >= 'A' ) && ( ch <= 'Z' ) ) ch+= ( 'a'-'A' ) ;
                  if ( ( Shift == 0 ) && ( ch >= 'a' ) && ( ch <= 'z' ) ) ch-= ( 'a'-'A' ) ;
              }
              kbevent.key = ch;
//          if(ui_Uparrow(ch)) printf("UpArrow\n");
//          if(ui_Tab(ch)) printf("Tab\n");
              *kbe = kbevent;
              return 1;
              case MotionNotify:
              B = * ( ( XMotionEvent * ) & e ) ;
//          printf("Mouse Moved: %d:%d\n",b.button,b.state);
              if ( B.state & ( BUTNMASK ) ) {
                  kbevent.event = 3;
              }
              else kbevent.event = 0;
              time0 = B.time;
//          while(XCheckWindowEvent(wc->Dsp,wc->Win,EventMask,&e)) {
              kbevent.x = B.x;
              kbevent.y = B.y;
              kbevent.rootx = B.x_root;
              kbevent.rooty = B.y_root;
#if 1
              if ( kbevent.event == 3 ) {
                  while ( 1 ) {
                      XPeekEvent ( wc->Dsp , & eo ) ;
                      while ( ( eo.type != MotionNotify ) && ( eo.type != ButtonRelease ) ) {
                          XNextEvent ( wc->Dsp , & eo ) ;
                          XPeekEvent ( wc->Dsp , & eo ) ;
                      }
                      if ( eo.type == MotionNotify ) {
                          XNextEvent ( wc->Dsp , & e ) ;
#if 0
                          XWindowEvent ( wc->Dsp , wc->Win , EventMask , & e ) ;
                          while ( e.type != MotionNotify ) XWindowEvent \
                           ( wc->Dsp , wc->Win , EventMask , & e ) ;
#endif
                          Btmp = * ( ( XMotionEvent * ) & e ) ;
                          kbevent.x = Btmp.x;
                          kbevent.y = Btmp.y;
                          if ( ( Btmp.time -time0 ) > 25 ) { break;}
                      }
                      else {
                          b = * ( ( XButtonEvent * ) & eo ) ;
                          kbevent.button = b.button;
                          kbevent.x = b.x;
                          kbevent.y = b.y;
                          break;
                      }
                  }
              }
#endif
              kbevent.button = 0;
              if ( B.state & Button1Mask ) kbevent.button += 1;
              if ( B.state & Button2Mask ) kbevent.button += 2;
              if ( B.state & Button3Mask ) kbevent.button += 4;
              if ( B.state & Button4Mask ) kbevent.button += 8;
              if ( B.state & Button5Mask ) kbevent.button += 16;
              *kbe = kbevent;
              return 1;
              default:
              pthread_mutex_lock ( & ( wc->Dsplock ) ) ;
              wc->ExposeWin = 1;
              pthread_mutex_unlock ( & ( wc->Dsplock ) ) ;
              continue;
          }
      }
  }
  void uiPositionPointer ( kgWC *wc , int x , int y ) {
      XWarpPointer ( wc->Dsp , wc->Win , wc->Win , 0 , \
           0 , wc->EVGAX+1 , wc->EVGAY+1 , x , y ) ;
  }
  int getintval ( char x ) {
      switch ( x ) {
          case '0': return 0;
          case '1': return 1;
          case '2': return 2;
          case '3': return 3;
          case '4': return 4;
          case '5': return 5;
          case '6': return 6;
          case '7': return 7;
          case '8': return 8;
          case '9': return 9;
          case 'A': return 10;
          case 'B': return 11;
          case 'C': return 12;
          case 'D': return 13;
          case 'E': return 14;
          case 'F': return 15;
          case 'a': return 10;
          case 'b': return 11;
          case 'c': return 12;
          case 'd': return 13;
          case 'e': return 14;
          case 'f': return 15;
          default: return -1;
      }
  }
  void * create_xpmview ( char **buf , int back_gr ) {
      return ( void * ) buf;
  }
  int SearchClr ( XColor C ) {
      int i , err , col , err1 , er , eg , eb;
      err = 100000;
      col = -1;
      for ( i = 1023;i >= 0;i-- ) {
          er = abs ( C.red - DefClrs [ i ] .red ) ;
          eg = abs ( C.green - DefClrs [ i ] .green ) ;
          eb = abs ( C.blue - DefClrs [ i ] .blue ) ;
          err1 = er + eg + eb;
          if ( err1 < err ) {err = err1;col = i;};
      }
  /*if( err > 6000 ) col=-col;*/
      return col;
  }
  int uiSearchClr ( kgColor *kgcolors , XColor C ) {
      int i , err , col , err1 , er , eg , eb;
      err = 100000;
      col = -1;
      for ( i = 1023;i >= 0;i-- ) {
          er = abs ( C.red - kgcolors [ i ] .red ) ;
          eg = abs ( C.green - kgcolors [ i ] .green ) ;
          eb = abs ( C.blue - kgcolors [ i ] .blue ) ;
          err1 = er + eg + eb;
          if ( err1 < err ) {err = err1;col = i;};
      }
  /*if( err > 6000 ) col=-col;*/
      return col;
  }
  int get_total_colors ( void ) {
      return NCLRS;
  }
  void kgRaiseWindow ( void *Tmp ) {
      kgWC *wc;
      XClientMessageEvent cEvent;
      wc = ( ( DIALOG * ) Tmp )->wc;
#if 0
      cEvent.message_type = XInternAtom ( wc->Dsp , "_NET_WM_STATE" , True ) ;
      cEvent.data.l [ 0 ] = 1;
      cEvent.data.l [ 3 ] = 1;
      cEvent.data.l [ 4 ] = 0;
      cEvent.data.l [ 1 ] = XInternAtom ( wc->Dsp , "_NET_WM_STATE_ABOVE" , True ) ;
      cEvent.data.l [ 2 ] = 0;
      wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
#else
      XRaiseWindow ( wc->Dsp , wc->Win ) ;
#endif
      XSync ( wc->Dsp , False ) ;
  }
  void kgLowerWindow ( void *Tmp ) {
      kgWC *wc;
      XClientMessageEvent cEvent;
      wc = ( ( DIALOG * ) Tmp )->wc;
#if 0
      cEvent.message_type = XInternAtom ( wc->Dsp , "_NET_WM_STATE" , True ) ;
      cEvent.data.l [ 0 ] = 1;
      cEvent.data.l [ 3 ] = 1;
      cEvent.data.l [ 4 ] = 0;
      cEvent.data.l [ 1 ] = XInternAtom ( wc->Dsp , "_NET_WM_STATE_BELOW" , True ) ;
      cEvent.data.l [ 2 ] = 0;
      wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
#else
      XLowerWindow ( wc->Dsp , wc->Win ) ;
#endif
      XSync ( wc->Dsp , False ) ;
  }
  void kgWindowStateAbove ( void *Tmp ) {
      kgWC *wc;
      XClientMessageEvent cEvent;
      wc = ( ( DIALOG * ) Tmp )->wc;
#if 1
      cEvent.message_type = XInternAtom ( wc->Dsp , "_NET_WM_STATE" , True ) ;
      cEvent.data.l [ 0 ] = 1;
      cEvent.data.l [ 3 ] = 1;
      cEvent.data.l [ 4 ] = 0;
      cEvent.data.l [ 1 ] = XInternAtom ( wc->Dsp , "_NET_WM_STATE_ABOVE" , True ) ;
      cEvent.data.l [ 2 ] = 0;
      wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
#else
      XRaiseWindow ( wc->Dsp , wc->Win ) ;
#endif
      XSync ( wc->Dsp , False ) ;
  }
  void kgWindowStateBelow ( void *Tmp ) {
      kgWC *wc;
      XClientMessageEvent cEvent;
      wc = ( ( DIALOG * ) Tmp )->wc;
#if 1
      cEvent.message_type = XInternAtom ( wc->Dsp , "_NET_WM_STATE" , True ) ;
      cEvent.data.l [ 0 ] = 1;
      cEvent.data.l [ 3 ] = 1;
      cEvent.data.l [ 4 ] = 0;
      cEvent.data.l [ 1 ] = XInternAtom ( wc->Dsp , "_NET_WM_STATE_BELOW" , True ) ;
      cEvent.data.l [ 2 ] = 0;
      wcSendEvent ( wc , ( XEvent * ) & cEvent ) ;
#else
      XLowerWindow ( wc->Dsp , wc->Win ) ;
#endif
      XSync ( wc->Dsp , False ) ;
  }
  int kgCheckAttention ( void *Tmp ) {
      kgWC *wc;
      Window w;
      int rev , ret = 0;
      wc = ( ( DIALOG * ) Tmp )->wc;
      XGetInputFocus ( wc->Dsp , & w , & rev ) ;
      if ( w == wc->Win ) ret = 1;
      return ret;
  }
  int kgMovePointer ( void *Tmp , int x , int y ) {
      DIALOG *D;
      kgWC *wc;
      Window Root , Root1 , Child , Curwin , Win;
      int x0 , y0 , ret;
      int x1 , y1 , x2 , y2;
      unsigned int pb;
      D = ( DIALOG * ) Tmp;
      wc = ( ( DIALOG * ) Tmp )->wc;
      Curwin = wc->Root;
      Win = wc->Win;
      XQueryPointer ( wc->Dsp , Curwin , & Root , & Child , \
           & x1 , & y1 , & x2 , & y2 , & pb ) ;
      x0 = x1; y0 = y1;
      if ( Child != Win ) {
          XWarpPointer ( wc->Dsp , Child , Win , 0 , 0 , 0 , 0 , x , y ) ;
      }
      else {
          XWarpPointer ( wc->Dsp , Win , Win , 0 , 0 , 0 , 0 , x , y ) ;
      }
      XQueryPointer ( wc->Dsp , Curwin , & Root , & Child , \
           & x1 , & y1 , & x2 , & y2 , & pb ) ;
      XQueryPointer ( wc->Dsp , Child , & Root , & Child , \
           & x1 , & y1 , & x2 , & y2 , & pb ) ;
      if ( ( x2 != x ) || ( y2 != y ) ) {
          ret = 0;
      }
      else ret = 1;
      return ret;
  }
/*************************************************
  New Addtion Dated 4th July 2002
  Image managing routines
**************************************************/
#if 0
#define GREEN_VAL (((((val>>8)&0x0000ff) * ((Image->green_mask)>> \
   ( BlueMaskPos ) ) + ( 1 << ( ( Image->green_mask >>  \
       ( BlueMaskPos ) ) -1 ) ) ) /256 ) << \
   ( BlueMaskPos ) )
#define RED_VAL (((((val>>16)&0x0000ff) * ((Image->red_mask)>> \
   ( GreenMaskPos ) ) + ( 1 << ( ( Image->red_mask >>  \
       ( GreenMaskPos ) ) -1 ) ) ) /256 ) << \
   ( GreenMaskPos ) )
#endif
/*****************************************************************
  Dated 12th July 2002
  Introduced to display an xpm image
  It clips the center portion of the image if the image
  is bigger than the given rectangle;
  otherwise it places the
  picture in the middle of the rectangle.
  colour none in the image is not drawn and the original background 
  is retained
******************************************************************/
#if 0
  int SetImageClip ( int vx0 , int vy0 , int vx1 , int vy1 ) {
      Vx0 = vx0; Vy0 = vy0;
      Vx1 = vx1; Vy1 = vy1;
      return 1;
  }
#endif
  void kgCleanBackground ( void *Tmp , int xo , int yo , \
       int width , int height , float transparency ) \
  {
      kgWC *wc;
      XImage *IMAGE;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      wc = D->wc;
// Tiling maybe tried but may not work
//  IMAGE = XGetImage(wc->Dsp,wc->Win,xo,yo,width,height,0xffffffff,ZPixmap);
//TCB 11/21
      IMAGE = kg_GetImage ( D , xo , yo , width , height ) ;
      uiMakeImageTransparent ( IMAGE , transparency ) ;
      XPutImage ( wc->Dsp , wc->Pix , wc->Gc , IMAGE , \
           0 , 0 , xo , yo , width , height ) ;
//  _uiTileImage(wc,wc->WIN,wc->IMAGE,EVGAX,EVGAY);
      XDestroyImage ( IMAGE ) ;
  }
  void uiMakeImageTransparent ( XImage *ximage , float transparency ) {
      unsigned long ALPHA = 0xff000000;
      unsigned long color = 0x00000000;
      unsigned long ximage_rowbytes;
      unsigned char *dest;
      unsigned char *Imgdata;
      int i , j;
      if ( transparency > 1.0 ) transparency = 1.0;
      if ( transparency < 0.0 ) transparency = 0.0;
      ALPHA = ( 1. -transparency ) *255;
      color = ( ALPHA << 24 ) ;
//  color = ALPHA;
      ximage_rowbytes = ximage->bytes_per_line;
      Imgdata = ( unsigned char * ) ximage->data;
      for ( j = 0;j < ximage->height;j++ ) {
          dest = ( unsigned char * ) ( Imgdata +j*ximage_rowbytes ) ;
          for ( i = 0;i < ximage->width;i++ ) {
              *dest++= 0;
              *dest++= 0;
              *dest++= 0;
              *dest++= ALPHA;
          }
      }
      return;
  }
  void uiChangeImageTransparency ( XImage *uiImage , float transparency ) {
      unsigned long ALPHA = 0xff000000;
      unsigned long color = 0x00000000;
      unsigned long ximage_rowbytes;
      unsigned char *dest;
      unsigned char *Imgdata;
      int RedMaskPos , GreenMaskPos , BlueMaskPos;
      unsigned long val;
      float fac;
      int i , j;
      if ( transparency > 1.0 ) transparency = 1.0;
      if ( transparency < 0.0 ) transparency = 0.0;
      fac = ( 1-transparency*transparency ) ;
      ALPHA = fac*255;
      ALPHA = ( ALPHA << 24 ) ;
      fac = ( 1-transparency ) ;
      ximage_rowbytes = uiImage->bytes_per_line;
      Imgdata = ( unsigned char * ) uiImage->data;
      for ( j = 0;j < uiImage->height;j++ ) {
          dest = ( unsigned char * ) ( Imgdata +j*ximage_rowbytes ) ;
          for ( i = 0;i < uiImage->width;i++ ) {
              *dest = ( *dest*fac ) ; dest++;
              *dest = ( *dest*fac ) ; dest++;
              *dest = ( *dest*fac ) ; dest++;
              *dest = ALPHA; dest++;
          }
      }
      return;
  }
  void _uiTileImage ( kgWC *wc , Pixmap pix , \
       XImage *ximage , int wd , int ht ) \
  {
      int i , j , h , w , ix , iy , ixr , iyr , xo , yo;
      h = ximage->height;
      w = ximage->width;
      ix = wd/w;
      ixr = wd%w;
      iy = ht/h;
      iyr = ht%h;
      yo = 0;
      for ( j = 0;j < iy;j++ ) {
          xo = 0;
          for ( i = 0;i < ix;i++ ) {
              XPutImage ( wc->Dsp , pix , wc->Gc , ximage , 0 , 0 , xo , yo , w , h ) ;
              xo += w;
          } // for i = 0
          if ( ixr != 0 ) XPutImage ( wc->Dsp , pix , wc->Gc , \
               ximage , 0 , 0 , xo , yo , ixr , h ) ;
          yo+= h;
      } // for j = 0
      if ( iyr != 0 ) {
          xo = 0;
          for ( i = 0;i < ix;i++ ) {
              XPutImage ( wc->Dsp , pix , wc->Gc , ximage , 0 , 0 , xo , yo , w , iyr ) ;
              xo += w;
          } // for i = 0
          if ( ixr != 0 ) XPutImage ( wc->Dsp , pix , wc->Gc , \
               ximage , 0 , 0 , xo , yo , ixr , iyr ) ;
      }
      return;
  }
  void FreePng ( void *tmp ) {
      PNGIMG *png;
      png = ( PNGIMG * ) tmp;
      if ( tmp == NULL ) return;
      free ( png->image_data ) ;
      free ( png ) ;
  }
  int _uiCheckXpm ( char *flname ) {
      int ret = 0;
      FILE *fp;
      char buf [ 50 ] ;
      char sharedfile [ 1000 ] ;
//  printf("Checking XPM file : %s\n",flname);
      fp = fopen ( flname , "r" ) ;
      if ( fp == NULL ) {
          strcpy ( sharedfile , SharedPicDir ) ;
          strcat ( sharedfile , "/" ) ;
          strcat ( sharedfile , flname ) ;
          fp = fopen ( sharedfile , "rb" ) ;
          if ( fp == NULL ) {
              strcpy ( sharedfile , SystemPicDir ) ;
              strcat ( sharedfile , "/" ) ;
              strcat ( sharedfile , flname ) ;
              fp = fopen ( sharedfile , "rb" ) ;
          }
      }
      if ( fp != NULL ) {
          char * rval;
          rval = fgets ( buf , 7 , fp ) ;
          buf [ 7 ] = '\0';
//    printf("%s\n",buf);
          if ( ( buf [ 3 ] == 'X' ) && ( buf [ 4 ] == 'P' ) && \
           ( buf [ 5 ] == 'M' ) ) ret = 1;
          else ret = 0;
          fclose ( fp ) ;
      }
      return ret;
  }
  void * _uiGetFileImage ( char *flname ) {
      FILE *infile;
      char sharedfilename [ 500 ] ;
      strcpy ( sharedfilename , flname ) ;
      if ( ! ( infile = fopen ( sharedfilename , "rb" ) ) ) {
          strcpy ( sharedfilename , SharedPicDir ) ;
          strcat ( sharedfilename , "/" ) ;
          strcat ( sharedfilename , flname ) ;
          if ( ! ( infile = fopen ( sharedfilename , "rb" ) ) ) {
              strcpy ( sharedfilename , SystemPicDir ) ;
              strcat ( sharedfilename , "/" ) ;
              strcat ( sharedfilename , flname ) ;
              if ( ! ( infile = fopen ( sharedfilename , "rb" ) ) ) {
                  return NULL;
              }
          }
      }
      fclose ( infile ) ;
      return uiGetgmImage ( sharedfilename ) ;
  }
  char * _uiGetFileImageName ( DIALOG *D , char *flname ) {
      FILE *infile;
      char *fullname = NULL;
      char sharedfilename [ 500 ] ;
      strcpy ( sharedfilename , flname ) ;
      if ( ! ( infile = fopen ( sharedfilename , "rb" ) ) ) {
          Dlink *L;
          char *pt;
          L = ( Dlink * ) D->SearchList;
          Resetlink ( L ) ;
          if ( L != NULL ) {
              while ( ( pt = ( char * ) Getrecord ( L ) ) != NULL ) {
                  strcpy ( sharedfilename , pt ) ;
                  strcat ( sharedfilename , flname ) ;
                  if ( ( infile = fopen ( sharedfilename , "rb" ) ) ) {
                      fclose ( infile ) ;
                      fullname = ( char * ) Malloc ( strlen ( sharedfilename ) +1 ) ;
                      strcpy ( fullname , sharedfilename ) ;
                      break;
                  }
              }
          }
      }
      else {
          fclose ( infile ) ;
          fullname = ( char * ) Malloc ( strlen ( sharedfilename ) +1 ) ;
          strcpy ( fullname , sharedfilename ) ;
      }
      return fullname;
  }
#if 0
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#include "jpeglib.h"
#include <setjmp.h>
  struct my_error_mgr {
      struct jpeg_error_mgr pub; /* "public" fields */
      jmp_buf setjmp_buffer; /* for return to caller */
  };
  typedef struct my_error_mgr * my_error_ptr;
/*
 * Here's the routine that will replace the standard error_exit method:
 */
  METHODDEF ( void ) my_error_exit ( j_common_ptr cinfo ) {
      my_error_ptr myerr = ( my_error_ptr ) cinfo->err;
//  (*cinfo->err->output_message) (cinfo);
      longjmp ( myerr->setjmp_buffer , 1 ) ;
  }
  void SetSharedPicDir ( char *dirname ) {
      strcpy ( SharedPicDir , dirname ) ;
      return;
  }
  void * ReadJpegFile ( char * flname ) {
      struct jpeg_decompress_struct cinfo;
      struct my_error_mgr jerr;
      FILE * infile; /* source file */
      unsigned char * buffer; /* Output row buffer */
      int row_stride , j , i; /* physical row width in output buffer */
      JPGIMG *img;
      int *pt , r , g , b;
      unsigned char *bufpt;
      char sharedfilename [ 1000 ] ;
      if ( ! ( infile = fopen ( flname , "rb" ) ) ) {
          strcpy ( sharedfilename , SharedPicDir ) ;
          strcat ( sharedfilename , "/" ) ;
          strcat ( sharedfilename , flname ) ;
          if ( ! ( infile = fopen ( sharedfilename , "rb" ) ) ) {
              strcpy ( sharedfilename , SystemPicDir ) ;
              strcat ( sharedfilename , "/" ) ;
              strcat ( sharedfilename , flname ) ;
              if ( ! ( infile = fopen ( sharedfilename , "rb" ) ) ) {
                  return NULL;
              }
          }
      }
      img = ( JPGIMG * ) Malloc ( sizeof ( JPGIMG ) ) ;
      strcpy ( img->Sign , "JPG" ) ;
      img->incode = 0;
      cinfo.err = jpeg_std_error ( & jerr.pub ) ;
      jerr.pub.error_exit = my_error_exit;
      if ( setjmp ( jerr.setjmp_buffer ) ) {
          jpeg_destroy_decompress ( & cinfo ) ;
          fclose ( infile ) ;
          return NULL;
      }
      jpeg_create_decompress ( & cinfo ) ;
      jpeg_stdio_src ( & cinfo , infile ) ;
       ( void ) jpeg_read_header ( & cinfo , TRUE ) ;
       ( void ) jpeg_start_decompress ( & cinfo ) ;
      row_stride = cinfo.output_width * cinfo.output_components;
      img->col = cinfo.output_width;
      img->row = cinfo.output_height;
      img->colors = cinfo.output_components;
      img->matchcol = ( int * ) Malloc ( sizeof ( int ) *img->row*img->col ) ;
      j = 0;
      buffer = ( char * ) Malloc ( row_stride ) ;
      while ( cinfo.output_scanline < cinfo.output_height ) {
           ( void ) jpeg_read_scanlines ( & cinfo , ( JSAMPARRAY ) & buffer , 1 ) ;
          pt = img->matchcol+j*img->col;
          for ( i = 0;i < img->col;i++ ) {
              bufpt = buffer+ ( i*img->colors ) ;
              if ( img->colors != 3 ) { r = g = b = bufpt [ 0 ] ;}
              else {
                  r = bufpt [ 0 ] ;
                  g = bufpt [ 1 ] ;
                  b = bufpt [ 2 ] ;
              }
              pt [ i ] = ( r << 16 ) + ( g << 8 ) +b;
          }
          j++;
      }
      free ( buffer ) ;
       ( void ) jpeg_finish_decompress ( & cinfo ) ;
      jpeg_destroy_decompress ( & cinfo ) ;
      fclose ( infile ) ;
      return ( void * ) img;
  }
  void FreeJpegImg ( void *tmp ) {
      JPGIMG *img = ( JPGIMG * ) tmp;
      if ( img != NULL ) {
          free ( img->matchcol ) ;
          free ( img ) ;
      }
      return;
  }
#if 0  
/*********************************************

  Reads an xpm file and returns a pointer (char **)
  to xpm data with a NULL pointer at the
  end. NULL is returned if there is
  an error

***********************************************/
  char **ReadXpmFile ( char *flname ) {
      FILE *fp;
      char buf [ 5000 ] ;
      int i , ch , ch1 , j = 0 , h , w , ncol;
      char **pt = NULL;
      char sharedfile [ 1000 ] ;
      fp = fopen ( flname , "r" ) ;
      if ( fp == NULL ) {
          strcpy ( sharedfile , SharedPicDir ) ;
          strcat ( sharedfile , "/" ) ;
          strcat ( sharedfile , flname ) ;
          fp = fopen ( sharedfile , "rb" ) ;
          if ( fp == NULL ) {
              strcpy ( sharedfile , SystemPicDir ) ;
              strcat ( sharedfile , "/" ) ;
              strcat ( sharedfile , flname ) ;
              fp = fopen ( sharedfile , "rb" ) ;
          }
      }
      if ( fp != NULL ) {
          while ( ( ch = fgetc ( fp ) ) != '{' ) {
              process_comment ( fp , ch ) ;
          }
          j = 0;
          while ( ( ch = fgetc ( fp ) ) != '}' ) {
              process_comment ( fp , ch ) ;
              if ( ch == '\"' ) {
                  i = 0;
                  while ( ( ch = fgetc ( fp ) ) != '\"' ) {
                      process_comment ( fp , ch ) ;
                      buf [ i++ ] = ch;
                  }
                  buf [ i ] = '\0';
                  if ( j == 0 ) {
                      sscanf ( buf , "%d%d%d" , & w , & h , & ncol ) ;
                      pt = ( char ** ) malloc ( sizeof ( char * ) * ( h+ncol+2 ) ) ;
                  }
                  pt [ j ] = ( char * ) malloc ( i+1 ) ;
                  strcpy ( pt [ j++ ] , buf ) ;
              }
          }
          fclose ( fp ) ;
      }
      if ( pt != NULL ) pt [ j ] = NULL;
      return pt;
  }
#endif
/********************************************************************
    stuff for tiff image writting
*********************************************************************/
#define FUNC
#define COMPRESSION_NONE 1
#define COMPRESSION_LZW 5
#define COMPRESSION 1
#define BYTE 1
#define ASCII 2
#define SHORT 3
#define LONG 4
#define RATIONAL 5
  static FUNC void WrShortField ( FILE *fp , short tag , \
       short type , int length , short value ) \
  {
      fwrite ( & tag , ( size_t ) 2 , ( size_t ) 1 , fp ) ;
      fwrite ( & type , ( size_t ) 2 , ( size_t ) 1 , fp ) ;
      fwrite ( & length , ( size_t ) 4 , ( size_t ) 1 , fp ) ;
      fwrite ( & value , ( size_t ) 2 , ( size_t ) 1 , fp ) ;
      fwrite ( & value , ( size_t ) 2 , ( size_t ) 1 , fp ) ; /* dummy write... */
  }
  static FUNC void WrLongField ( FILE *fp , short tag , \
       short type , int length , int value ) \
  {
      fwrite ( & tag , ( size_t ) 2 , ( size_t ) 1 , fp ) ;
      fwrite ( & type , ( size_t ) 2 , ( size_t ) 1 , fp ) ;
      fwrite ( & length , ( size_t ) 4 , ( size_t ) 1 , fp ) ;
      fwrite ( & value , ( size_t ) 4 , ( size_t ) 1 , fp ) ;
  }
  static short * setpal ( Dlink *Tlist ) {
      short *palette = NULL;
      int no , k = 0 , *val;
      no = Dcount ( Tlist ) ;
      palette = ( short * ) Malloc ( sizeof ( short ) *no*3 ) ;
      Resetlink ( Tlist ) ;
      while ( ( val = ( int * ) Getrecord ( Tlist ) ) != NULL ) {
          palette [ k ] = ( ( ( *val ) *0x00ff0000 ) >> 16 ) *65535/255;
          palette [ k+no ] = ( ( ( *val ) *0x0000ff00 ) >> 8 ) *65535/255;
          palette [ k+no+no ] = ( ( ( *val ) *0x000000ff ) ) *65535/255;
          k++;
      }
      return palette;
  }
  static FUNC void writetiff ( char *flname , Dlink *Tlist , \
       short *buffer , int SIZEX , int SIZEY ) \
  {
      short *palette;
      short NumFields = ( COMPRESSION == 5 ) ? 16: 12;
      int offset = 8L;
      int NextOffset = 0L;
      int ResolutionOffset = offset+2+NumFields*12+4;
      int resolution [ 4 ] = {
      300 , 1 , 300 , 1 };
      int PaletteOffset = sizeof ( resolution ) +ResolutionOffset;
      int StripOffsets = PaletteOffset+256*3*2;
      int StripLens = StripOffsets+4;
      int DataOffset = /*StripLens+4; */StripOffsets;
      int i;
      int *StripOffset;
      int *StripLen;
      short version = 0x002a;
      FILE *out;
      out = fopen ( flname , "w" ) ;
      palette = setpal ( Tlist ) ;
/* write header */
        /*printf("Version = %d\n",version);*/
        /*fwrite("\x4d\x4d", (size_t)1, (size_t)2, out);*/
      fwrite ( "II" , ( size_t ) 1 , ( size_t ) 2 , out ) ;
      fwrite ( & version , sizeof ( short ) , ( size_t ) 1 , out ) ;
      fwrite ( & offset , ( size_t ) 1 , ( size_t ) 4 , out ) ;
/* write directory */
      fwrite ( & NumFields , ( size_t ) 1 , ( size_t ) 2 , out ) ;
      WrLongField ( out , ( short ) 254 , LONG , ( int ) 1 , ( int ) 0 ) ;
      WrLongField ( out , ( short ) 256 , LONG , ( int ) 1 , ( int ) SIZEX ) ;
                                	/* ImageWidth */
      WrLongField ( out , ( short ) 257 , LONG , ( int ) 1 , ( int ) SIZEY ) ;
                                	/* ImageLength */
      WrShortField ( out , ( short ) 258 , SHORT , ( int ) 1 , ( short ) 16 ) ;
                                	/* BitsPerSample */
      WrShortField ( out , ( short ) 259 , SHORT , \
       ( int ) 1 , ( short ) COMPRESSION ) ;
                                        /* Compression=LZW */
      WrShortField ( out , ( short ) 262 , SHORT , ( int ) 1 , ( short ) 3 ) ;
                        		/* PhotometricInterpretation */
      WrLongField ( out , ( short ) 273 , LONG , 1 , DataOffset ) ;
                                        	/* SamplesPerPixel */
      WrShortField ( out , ( short ) 277 , SHORT , ( int ) 1 , ( short ) 1 ) ;
      WrLongField ( out , ( short ) 282 , RATIONAL , \
       ( int ) 1 , ( int ) ResolutionOffset ) ;
                                             /* XResolution */
      WrLongField ( out , ( short ) 283 , RATIONAL , \
       ( int ) 1 , ( int ) ResolutionOffset+8 ) ;
                                             /* YResolution */
      WrShortField ( out , ( short ) 284 , SHORT , ( int ) 1 , ( short ) 1 ) ;
                         		/* PlanarConfiguration */
      WrLongField ( out , ( short ) 320 , SHORT , \
       ( int ) 256*3 , ( int ) PaletteOffset ) ;
      fwrite ( & NextOffset , ( size_t ) 1 , ( size_t ) 4 , out ) ;
      fwrite ( resolution , ( size_t ) sizeof ( resolution [ 0 ] ) , \
       ( size_t ) 4 , out ) ;
      fwrite ( palette , sizeof ( unsigned short ) , \
       ( size_t ) Dcount ( Tlist ) *3 , out ) ;
      fwrite ( buffer , ( size_t ) ( sizeof ( short ) ) , \
       ( size_t ) SIZEX*SIZEY , out ) ;
      fclose ( out ) ;
  }
  static int add_palette ( Dlink *Tlist , int val ) {
      int count = 0;
      int *pt , *tmp;
      count = 1;
      Resetlink ( Tlist ) ;
      while ( ( pt = ( int * ) Getrecord ( Tlist ) ) != NULL ) {
          if ( val != *pt ) {count++;continue;}
          break;
      }
      if ( pt == NULL ) {
          tmp = ( int * ) Malloc ( sizeof ( int ) ) ;
          *tmp = val;
          Dappend ( Tlist , tmp ) ;
      }
      return count;
  }
  void * RefreshWindow ( void *junk ) {
      int rev;
      struct timeval tv;
      int retval , status , oldtype;
      Window w;
      XEvent event;
      kgWC *wc;
      wc = ( kgWC * ) junk;
      pthread_setcanceltype ( PTHREAD_CANCEL_ASYNCHRONOUS , & oldtype ) ;
      if ( wc->FullScreen ) return NULL;
//     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
//     while(XCheckWindowEvent(Dsp,Win,ExposureMask,&event));
      while ( 1 ) {
          tv.tv_sec = 1;
          tv.tv_usec = 0;
          retval = select ( 0 , NULL , NULL , NULL , & tv ) ;
          event.type = 0;
          if ( wc->ExposeWin ) {
              pthread_mutex_lock ( & ( wc->Dsplock ) ) ;
              XLockDisplay ( wc->Dsp ) ;
              XCopyArea ( wc->Dsp , wc->Piximg , wc->Win , wc->Gc , \
                   0 , 0 , wc->EVGAX , wc->EVGAY , 0 , 0 ) ;
              XUnlockDisplay ( wc->Dsp ) ;
              wc->ExposeWin = 0;
              pthread_mutex_unlock ( & ( wc->Dsplock ) ) ;
          }
      }
      return NULL;
  }
  int RefreshWindowThread ( void *junk ) {
//    pthread_t Pth;
      kgWC *wc;
      wc = junk;
      if ( wc == NULL ) return 0;
      if ( wc->FullScreen ) return 0;
      pthread_create ( & ( wc->Pth ) , NULL , RefreshWindow , wc ) ;
      return 1;
  }
  int StartMonitorExposure ( void ) {
//    pthread_mutex_unlock(&_Dsplock);
      return 1;
  }
  int StopMonitorExposure ( void ) {
//    pthread_mutex_lock(&_Dsplock);
      return 1;
  }
  int CheckExposure ( void ) {
      XEvent event;
#if 0
      if ( XCheckWindowEvent ( Dsp , Win , ExposureMask , & event ) ) {
//           printf("Got Event\n");
          while ( XCheckWindowEvent ( Dsp , Win , ExposureMask , & event ) ) ;
          XCopyArea ( Dsp , Pix , Win , Gc , 0 , 0 , EVGAX , EVGAY , 0 , 0 ) ;
      }
#endif
      return 1;
  }
  void *uiWriteJpg ( char *jpgfile ) {
      char name [ 100 ] , fname [ 100 ] ;
      JPGIMG *img;
      int w , h , i , size , rem , j , k;
      int *pt;
      FILE *fp;
      img = ( JPGIMG * ) ReadJpegFile ( jpgfile ) ;
      if ( img == NULL ) return NULL;
      i = 0;
      i = strlen ( jpgfile ) ;
      i--;
      while ( jpgfile [ i ] != '/' ) {
          i--;
          if ( i < 0 ) break;
      }
      i++;
      j = 0;
      while ( ( jpgfile [ i ] != '.' ) && ( jpgfile [ i ] > ' ' ) ) name [ j++ ] = jpgfile [ i++ ] ;
          
      name [ j ] = '\0';
      strcpy ( fname , name ) ;
      strcat ( fname , ".c" ) ;
      fp = fopen ( fname , "w" ) ;
      h = img->row;
      w = img->col;
      pt = img->matchcol;
      fprintf ( fp , "#ifndef D_%-s\n" , name ) ;
      fprintf ( fp , "#define D_%-s\n" , name ) ;
      fprintf ( fp , "  static int %-s_data[%d] = {\n  " , name , w*h+1 ) ;
      size = ( w*h ) /12;
      rem = ( w*h ) - size*12;
      k = 0;
      for ( i = 0;i < size;i++ ) {
          for ( j = 0;j < 12;j++ ) {
              fprintf ( fp , "0x%8.8x," , pt [ k++ ] ) ;
          }
          fprintf ( fp , "\n  " ) ;
      }
      for ( j = 0;j < rem;j++ ) fprintf ( fp , "0x%8.8x," , pt [ k++ ] ) ;
      fprintf ( fp , "   0x0 };\n" ) ;
      fprintf ( fp , "  static JPGIMG  %-s_str = {\n" , name ) ;
      fprintf ( fp , "    \"JPG\", 1,\"%-s.jpg\",%d,%d,0,NULL,%-s_data \n  };\n" , \
           name , h , w , name ) ;
      fprintf ( fp , "#endif\n" ) ;
      fclose ( fp ) ;
      FreeJpegImg ( img ) ;
      return img;
  }
  void *uiWritePng ( char *pngfile ) {
      char name [ 100 ] , fname [ 100 ] ;
      PNGIMG *img;
/*
  unsigned long image_width ,image_height ,image_rowbytes;
  int  image_channels;
  unsigned char *image_data;
*/
      int w , h , i , size , rem , j , k;
      unsigned char *pt;
      FILE *fp;
      img = ( PNGIMG * ) ReadPngImage ( pngfile ) ;
      if ( img == NULL ) return NULL;
      i = 0;
      i = strlen ( pngfile ) ;
      i--;
      while ( pngfile [ i ] != '/' ) {
          i--;
          if ( i < 0 ) break;
      }
      i++;
      j = 0;
      while ( ( pngfile [ i ] != '.' ) && ( pngfile [ i ] > ' ' ) ) name [ j++ ] = pngfile [ i++ ] ;
          
      name [ j ] = '\0';
      strcpy ( fname , name ) ;
      strcat ( fname , ".c" ) ;
      fp = fopen ( fname , "w" ) ;
      w = img->image_width*img->image_rowbytes;
      w = img->image_rowbytes;
      h = img->image_height;
      pt = img->image_data;
      fprintf ( fp , "#ifndef D_%-s\n" , name ) ;
      fprintf ( fp , "#define D_%-s\n" , name ) ;
      fprintf ( fp , "  static unsigned char %-s_data[%d] = {\n  " , \
           name , w*h+1 ) ;
      size = ( w*h ) /12;
      rem = ( w*h ) - size*12;
      k = 0;
      for ( i = 0;i < size;i++ ) {
          for ( j = 0;j < 12;j++ ) {
              fprintf ( fp , "0x%2.2x," , pt [ k++ ] ) ;
          }
          fprintf ( fp , "\n  " ) ;
      }
      for ( j = 0;j < rem;j++ ) fprintf ( fp , "0x%2.2x," , pt [ k++ ] ) ;
      fprintf ( fp , "   0x0 };\n" ) ;
      fprintf ( fp , "  static PNGIMG  %-s_str = {\n" , name ) ;
      fprintf ( fp , "    \"PNG\",1,\"%-s.png\", %ld,%ld,%ld,%d,%-s_data \n  };\n" , \
           name , img->image_width , img->image_height , img->image_rowbytes , \
           img->image_channels , name ) ;
      fprintf ( fp , "#endif\n" ) ;
      fclose ( fp ) ;
//  FreeJpegImg(img);
      return img;
  }
  int uiFmgToFile ( unsigned char *array , int sz , char * flname ) {
      int rval;
#if 1
      int ot;
      ot = open ( flname , O_RDWR | O_CREAT | O_TRUNC , 0777 ) ;
      if ( ot < 0 ) return -1;
      rval = write ( ot , array , sz ) ;
      syncfs ( ot ) ;
      close ( ot ) ;
#else
      FILE *ot;
      ot = fopen ( flname , "w" ) ;
      rval = fwrite ( array , sz , 1 , ot ) ;
      fclose ( ot ) ;
#endif
      return 1;
  }
  int kgFmgToFile ( unsigned char *array , int sz , char * flname ) {
      int ot;
      int rval;
      ot = open ( flname , O_RDWR | O_CREAT | O_TRUNC , 0777 ) ;
      if ( ot < 0 ) return -1;
      rval = write ( ot , array , sz ) ;
      close ( ot ) ;
      return 1;
  }
  int uiExtractBaseName ( char *flname , char *basename ) {
      int i , j;
      i = 0;
      i = strlen ( flname ) ;
      i--;
      while ( flname [ i ] != '/' ) { i--; if ( i < 0 ) break; }
      i++;
      j = 0;
      while ( ( flname [ i ] != '.' ) && ( flname [ i ] > ' ' ) ) basename [ j++ ] = flname [ i++ ] ;
          
      basename [ j ] = '\0';
  }
  int kgExtractBaseName ( char *flname , char *basename ) {
      int i , j;
      i = 0;
      i = strlen ( flname ) ;
      i--;
      while ( flname [ i ] != '/' ) { i--; if ( i < 0 ) break; }
      i++;
      j = 0;
      while ( ( flname [ i ] != '.' ) && ( flname [ i ] > ' ' ) ) basename [ j++ ] = flname [ i++ ] ;
          
      basename [ j ] = '\0';
  }
  int uiWriteFmg ( char *infile ) {
      int inp , i , ret , sum = 0;
      FILE *fp;
      unsigned char buff [ 64 ] ;
      char flname [ 500 ] , basename [ 500 ] ;
      inp = open ( infile , O_RDONLY ) ;
      if ( inp < 0 ) return 0;
      uiExtractBaseName ( infile , basename ) ;
      strcpy ( flname , basename ) ;
//  sprintf(flname,"%-s.c",infile);
      strcat ( flname , ".c" ) ;
      fp = fopen ( flname , "w" ) ;
      if ( fp == NULL ) {
          fprintf ( stderr , "Failed to open %s for writting\n" , flname ) ;
          return 0;
      }
      fprintf ( fp , "#ifndef D_%-s\n" , basename ) ;
      fprintf ( fp , "#define D_%-s\n" , basename ) ;
      fprintf ( fp , "  unsigned char %-s_data[]= {\n" , basename ) ;
      while ( ( ret = read ( inp , buff , 16 ) ) == 16 ) {
          sum += ret;
          fprintf ( fp , "        " ) ;
          for ( i = 0;i < ret;i++ ) fprintf ( fp , "0x%2.2x," , buff [ i ] ) ;
          fprintf ( fp , "\n" ) ;
      }
      sum += ret;
      fprintf ( fp , "        " ) ;
      for ( i = 0;i < ret;i++ ) fprintf ( fp , "0x%2.2x," , buff [ i ] ) ;
      fprintf ( fp , "0xff\n  };\n" ) ;
      fprintf ( fp , "  FMGIMG %-s_str = { \"FMG\",1,\"%-s\",%d,%-s_data,NULL};\n" , \
           basename , basename , sum , basename ) ;
      fprintf ( fp , "#endif\n" ) ;
      fclose ( fp ) ;
      close ( inp ) ;
      return 1;
  }
  int kgImagetoC ( char *flname ) {
      return uiWriteFmg ( flname ) ;
  }
  void uiFreeImage ( void *tmp ) {
      JPGIMG *img;
      char *cpt;
      if ( tmp == NULL ) return;
      cpt = ( char * ) tmp;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) return;
      img = ( JPGIMG * ) tmp;
#if 1
      if ( strcmp ( img->Sign , "JPG" ) == 0 ) {
          if ( img->incode == 1 ) return;
          free ( img->matchcol ) ;
          free ( tmp ) ;
          tmp = NULL;
      }
      else {
          if ( strcmp ( img->Sign , "PNG" ) == 0 ) {
              PNGIMG *png;
              png = ( PNGIMG * ) tmp;
              if ( png->incode == 1 ) return;
              free ( png->image_data ) ;
              free ( tmp ) ;
              tmp = NULL;
          }
          else {
              if ( strcmp ( img->Sign , "IMG" ) == 0 ) {
                  GMIMG *png;
                  png = ( GMIMG * ) tmp;
                  if ( png->incode == 1 ) return;
                  uiFreeGmImage ( tmp ) ;
                  free ( tmp ) ;
                  tmp = NULL;
              }
              else if ( strcmp ( img->Sign , "FMG" ) == 0 ) {
                  FMGIMG *png;
                  png = ( FMGIMG * ) tmp;
                  if ( png->image != NULL ) {uiFreeFmgImage ( tmp ) ;png->image == NULL;}
                  if ( png->incode == 1 ) return;
                  free ( png->image_data ) ;
                  free ( tmp ) ;
                  tmp = NULL;
              }
          }
      }
#endif
      return ;
  }
  void kgFreeImage ( void *tmp ) {
      if ( tmp != NULL ) uiFreeImage ( tmp ) ;
  }
  int uiDataToArea ( DIALOG *D , unsigned long *data , \
       int xl , int yl , int w , int h ) \
  {
      int i , j , k;
      unsigned char *dest , *Imgdata;
      unsigned long r , g , b , a , v;
      XImage *ximage;
      int ximage_rowbytes;
      kgWC *wc;
      wc = WC ( D ) ;
      ximage = XGetImage ( wc->Dsp , wc->ActWin , xl , \
           yl , w , h , 0xffffffff , ZPixmap ) ;
      ximage_rowbytes = ximage->bytes_per_line;
      Imgdata = ( unsigned char * ) ximage->data;
      k = 0;
      for ( j = 0;j < h;j++ ) {
          dest = Imgdata + j*ximage_rowbytes;
          for ( i = 0;i < w;i++ ) {
              v = data [ k ] ;
              b = v & 0x000000ff; v = v >> 8;
              g = v & 0x000000ff; v = v >> 8;
              r = v & 0x000000ff; v = v >> 8;
              a = v & 0x000000ff;
#if 0
//       if( (r==0)&&(g==0)&&(b==0)&&(a==0)) {
              if ( ( a == 255 ) ) {
                  dest+= 4;
              }
              else {
                  *dest = b; dest++;
                  *dest = g; dest++;
                  *dest = r; dest++;
                  *dest = a; dest++;
              }
#else
              *dest = b; dest++;
              *dest = g; dest++;
              *dest = r; dest++;
              *dest = a; dest++;
#endif
              k++;
          }
      }
      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
           ximage , 0 , 0 , xl , yl , w , h ) ;
      XDestroyImage ( ximage ) ;
      return 1;
  }
  int uiAddDataToArea ( DIALOG *D , unsigned long *data , \
       int xl , int yl , int w , int h ) \
  {
      int i , j , k;
      unsigned char *dest , *Imgdata;
      unsigned long r , g , b , a , v;
      float f;
      XImage *ximage;
      int ximage_rowbytes;
      kgWC *wc;
      wc = WC ( D ) ;
      ximage = XGetImage ( wc->Dsp , wc->Pix , xl , \
           yl , w , h , 0xffffffff , ZPixmap ) ;
      ximage_rowbytes = ximage->bytes_per_line;
      Imgdata = ( unsigned char * ) ximage->data;
      k = 0;
      for ( j = 0;j < h;j++ ) {
          dest = Imgdata + j*ximage_rowbytes;
          for ( i = 0;i < w;i++ ) {
              v = data [ k ] ;
              b = v & 0x000000ff; v = v >> 8;
              g = v & 0x000000ff; v = v >> 8;
              r = v & 0x000000ff; v = v >> 8;
              a = v & 0x000000ff;
              f = ( float ) ( a ) /255.0;
#if 0
//       if( (r==0)&&(g==0)&&(b==0)&&(a==0)) {
              if ( ( a == 255 ) ) {
                  dest+= 4;
              }
              else {
                  *dest = ( *dest+b ) ; dest++;
                  *dest = g; dest++;
                  *dest = r; dest++;
                  *dest = a; dest++;
              }
#else
              v = ( *dest+b ) ;
              *dest = v > 255?255:v; dest++;
              v = ( *dest+g ) ;
              *dest = v > 255?255:v; dest++;
              v = ( *dest+r ) ;
              *dest = v > 255?255:v; dest++;
              v = ( *dest+a ) ;
              *dest = v > 255?255:v; dest++;
#if 0
              *dest = b; dest++;
              *dest = g; dest++;
              *dest = r; dest++;
              *dest = a; dest++;
#endif
#endif
              k++;
          }
      }
      XPutImage ( wc->Dsp , wc->ActWin , wc->Gc , \
           ximage , 0 , 0 , xl , yl , w , h ) ;
      XDestroyImage ( ximage ) ;
      return 1;
  }
/* new code for kglib    */
  SC_BUF * wc_alloc_scrn_buffer ( kgWC *wc ) {
      SC_BUF *tmp;
      Dlink *SBlist;
      SBlist = wc->SBlist;
      if ( SBlist == NULL ) {
          SBlist = Dopen ( ) ;
          wc->SBlist = SBlist;
      }
      tmp = ( SC_BUF * ) Malloc ( sizeof ( SC_BUF ) ) ;
      Dappend ( SBlist , tmp ) ;
      return tmp;
  }
  SC_BUF * kg_alloc_scrn_buffer ( kgWC *wc ) {
      SC_BUF *tmp;
      Dlink *SBlist;
      SBlist = wc->SBlist;
      if ( SBlist == NULL ) {
          SBlist = Dopen ( ) ;
          wc->SBlist = SBlist;
      }
      tmp = ( SC_BUF * ) Malloc ( sizeof ( SC_BUF ) ) ;
      Dappend ( SBlist , tmp ) ;
      return tmp;
  }
  SC_BUF * kg_get_scrn_buffer ( kgWC *wc ) {
      SC_BUF *tmp = NULL;
      Dlink *SBlist;
      SBlist = wc->SBlist;
      if ( SBlist == NULL ) {
          SBlist = Dopen ( ) ;
          wc->SBlist = SBlist;
      }
      Dend ( SBlist ) ;
      tmp = ( SC_BUF * ) Getrecord ( SBlist ) ;
      if ( tmp == NULL ) {
          fprintf ( stderr , "Failed to get Scren buffer\n" ) ;
          exit ( 0 ) ;
      }
      return tmp;
  }
  int kg_release_scrn_buffer ( kgWC *wc ) {
      SC_BUF *tmp;
      Dlink *SBlist;
      SBlist = wc->SBlist;
      if ( SBlist == NULL ) {
          return 0;
      }
      else {
          Dend ( SBlist ) ;
          tmp = ( SC_BUF * ) Getrecord ( SBlist ) ;
          XFreePixmap ( wc->Dsp , tmp->buf ) ;
          Dend ( SBlist ) ;
          Ddelete ( SBlist ) ;
          Dend ( SBlist ) ;
          return 1;
      }
  }
  void kg_clear_scrn_buffer ( kgWC *wc ) {
      SC_BUF *tmp;
      Dlink *SBlist;
      SBlist = wc->SBlist;
      if ( SBlist == NULL ) {
          return ;
      }
      else {
          Resetlink ( SBlist ) ;
          while ( ( tmp = ( SC_BUF * ) Getrecord ( SBlist ) ) != NULL ) {
              XFreePixmap ( wc->Dsp , tmp->buf ) ;
          }
          Dempty ( SBlist ) ;
          wc->SBlist = NULL;
      }
  }
/*
  Backing up a given area
*/
  SC_BUF * kg_scr_back ( kgWC *wc , int x1 , int y1 , int x2 , int y2 ) {
      int i , j , addr;
      SC_BUF *scpt;
      scpt = kg_alloc_scrn_buffer ( wc ) ;
      scpt->x1 = x1; scpt->x2 = x2;
      scpt->y1 = y1; scpt->y2 = y2;
      scpt->buf = wcXCPix ( x1 , y1 , x2 , y2 ) ;
      XCopyArea ( wc->Dsp , wc->DspWin , ( scpt->buf ) , wc->Gc , ( short ) x1 , \
       ( short ) ( y1 ) , ( short ) ( x2-x1+1 ) , ( short ) ( y2-y1+1 ) , 0 , 0 ) ;
      return scpt;
  }
/*
  Recovering just previous backed up area
*/
  void kg_scr_recover ( kgWC *wc ) {
      int i , j , addr;
      int x1 , x2 , y1 , y2;
      SC_BUF *scpt;
      scpt = kg_get_scrn_buffer ( wc ) ;
      x1 = scpt->x1; x2 = scpt->x2;
      y1 = scpt->y1; y2 = scpt->y2;
      XCopyArea ( wc->Dsp , ( scpt->buf ) , wc->DspWin , \
           wc->Gc , 0 , 0 , ( short ) \
       ( x2-x1+1 ) , ( short ) ( y2-y1+1 ) , ( short ) x1 , ( short ) y1 ) ;
      kg_release_scrn_buffer ( wc ) ;
      uiUpdateScreen ( wc ) ;
  }
  int kgPutImage ( DIALOG *D , XImage *uiImage , int sx , int sy , \
       int dx , int dy , int width , int height ) \
  {
      XPutImage ( ( Display * ) ( WC ( D )->Dsp ) , WC ( D )->ActWin , ( GC ) \
       ( WC ( D )->Gc ) , uiImage , sx , sy , dx , dy , width , height ) ;
      return 1;
  }
  XImage * kg_GetImage ( DIALOG *D , int x , int y , int width , int height ) {
      XImage *uiImage;
#if 1
      SC_BUF *scpt;
      if ( D->NoWinMngr ) {
          scpt = kg_scr_back ( WC ( D ) , x , y , x+width , y+height ) ;
          uiImage = XGetImage ( ( Display * ) WC ( D )->Dsp , scpt->buf , \
               0 , 0 , width , height , 0xffffffff , ZPixmap ) ;
          kg_release_scrn_buffer ( WC ( D ) ) ;
      }
      else {
          if ( WC ( D )->FullScreen ) {
              uiImage = XGetImage ( ( Display * ) WC ( D )->Dsp , WC ( D )->ActWin , \
                   x , y , width , height , 0xffffffff , ZPixmap ) ;
          }
          else {
              uiImage = XGetImage ( ( Display * ) WC ( D )->Dsp , WC ( D )->Pix , \
                   x , y , width , height , 0xffffffff , ZPixmap ) ;
          }
      }
      if ( uiImage == NULL ) {
          printf ( "Error:... In GetImage\n" ) ;
          fflush ( stdout ) ;
      }
      else {
          if ( WC ( D )->IMAGE != NULL ) {
              uiImage->red_mask = ( ( XImage * ) ( WC ( D )->IMAGE ) )->red_mask;
              uiImage->green_mask = ( ( XImage * ) ( WC ( D )->IMAGE ) )->green_mask;
              uiImage->blue_mask = ( ( XImage * ) ( WC ( D )->IMAGE ) )->blue_mask;
          }
      }
#else  /* Old Version; wont work when window part is not visible */
//  uiImage = XGetImage((Display *)(WC(D)->Dsp),WC(D)->ActWin,x,y,width,height,0xffffffff,ZPixmap);
#endif
      return uiImage;
  }
  int kg_png_image ( DIALOG *D , void *tmp , int x0 , int y0 , int width , \
       int height , float transparency , float highfac ) \
  {
      int i , j , k , m , row;
      unsigned long color;
      int sx , sy , dx , dy;
      int xoffset = 0 , yoffset = 0 , xm , ym , xl , yl , \
           xu , yu , xc , yc , xdl , xdu , ydl , ydu;
      float rzfac;
      unsigned long *dest , *Imgdata , val;
      unsigned long r , g , b , a;
      unsigned long lastrow = 0;
      unsigned long pixel;
      unsigned long red , green , blue;
      unsigned long bg_red , bg_green , bg_blue;
      double f , f1;
      unsigned char *pixels , *src;
      float transfac = 0.1;
      PNGIMG *png , *pngrz = NULL;
      int ximage_rowbytes;
      int w , h;
      unsigned int ALPHA;
      int RedMask , GreenMask , BlueMask , RedMaskPos , GreenMaskPos , BlueMaskPos;
      XImage *uiImage = NULL;
      int EVGAX , EVGAY;
      png = ( PNGIMG * ) tmp;
      if ( png == NULL ) return 0;
      EVGAX = WC ( D )->EVGAX;
      EVGAY = WC ( D )->EVGAY;
      if ( x0 < 0 ) x0 = 0;
      if ( y0 < 0 ) y0 = 0;
      if ( ( x0+width ) > EVGAX ) width = EVGAX-x0;
      if ( ( y0+height ) > EVGAY ) height = EVGAY-y0;
      xc = x0+width/2;
      yc = y0+height/2;
      ALPHA = 255* ( 1.-transparency*transfac ) ;
      ALPHA = 255;
      ALPHA = ALPHA << 24;
      pixels = png->image_data;
      w = png->image_width;
      h = png->image_height;
      xm = ( w-xoffset ) /2+xoffset;
      ym = ( h-yoffset ) /2+yoffset;
      xl = xm -width/2; xu = xm+width-width/2;
      yl = ym -height/2; yu = ym+height -height/2;
      if ( ( xu < 0 ) || ( xl > w ) || ( yu < 0 ) || ( yl > h ) ) {
          return 0;
      }
// There is something to display
      if ( xl < 0 ) xl = 0;
      if ( xu > w ) xu = w;
      if ( yl < 0 ) yl = 0;
      if ( yu > h ) yu = h;
      dx = xm - xc;
      dy = ym - yc;
      xdl = xl-dx;
      xdu = xu-dx;
      ydl = yl -dy;
      ydu = yu -dy;
      uiImage = kg_GetImage ( D , xdl , ydl , xdu-xdl+1 , ydu-ydl+1 ) ;
      RedMaskPos = GetNoOfBits ( uiImage->red_mask ) ;
      GreenMaskPos = GetNoOfBits ( uiImage->green_mask ) ;
      BlueMaskPos = GetNoOfBits ( uiImage->blue_mask ) ;
//    uiImage = XGetImage((Display *)WC(D)->Dsp,WC(D)->ActWin,xdl,ydl,xdu-xdl+1,ydu-ydl+1,0xffffffff,ZPixmap);
      ximage_rowbytes = uiImage->bytes_per_line/4;
      Imgdata = ( unsigned long * ) uiImage->data;
      row = -1;
      for ( i = 0; i < h; ++i ) {
          if ( i < ( yl ) ) continue;
          if ( i >= yu ) break;
          src = pixels + i*png->image_rowbytes;
          dest = Imgdata + i*ximage_rowbytes;
          row++;
          m = 0;
          k = -1;
          if ( png->image_channels == 3 ) {
              for ( j = 0;j < w; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src+= 3;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  red = *src;src++;
                  green = *src;src++;
                  blue = *src;src++;
                  color = Image_BLUE_VAL + Image_GREEN_VAL + Image_RED_VAL ;
                  if ( color >= 0 ) XPutPixel ( uiImage , m , row , color | ALPHA ) ;
                  m++;
              }
              } else /* if ( image_channels == 4 ) */ {
              for ( j = 0;j < w; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src+= 4;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  r = *src;src++;
                  g = *src;src++;
                  b = *src;src++;
                  a = *src;src++;
                  if ( a == 255 ) {
                      red = r;
                      green = g;
                      blue = b;
                      color = Image_BLUE_VAL + Image_GREEN_VAL + Image_RED_VAL ;
                      color = color | ALPHA;
                      } else if ( a == 0 ) {
#if 1
                      m++;
                      continue;
#else
                      red = bg_red;
                      green = bg_green;
                      blue = bg_blue;
#endif
                      } else {
                      f = a/255.0;
                      f1 = 1-f;
//                        f1 = f1*(1.-transparency);
                      val = XGetPixel ( uiImage , m , row ) ;
                      bg_red = ( val >> 16 ) & 0x000000ff;
                      bg_green = ( val >> 8 ) & 0x000000ff;
                      bg_blue = ( val ) & 0x000000ff;
                      red = f*r+f1 *bg_red;
                      green = f*g+f1 *bg_green;
                      blue = f*b+f1 *bg_blue;
                      red *= highfac;if ( red > 255 ) red = 255;
                      green *= highfac;if ( green > 255 ) green = 255;
                      blue *= highfac;if ( blue > 255 ) blue = 255;
                      val = val >> 24;
                      val = val+a* ( 1.-transparency*transfac ) ;
                      if ( val > 255 ) val = 255;
                      val = val << 24;
                      color = Image_BLUE_VAL + Image_GREEN_VAL + Image_RED_VAL ;
                      color = color | val;
                  }
                  if ( color >= 0 ) {
                      XPutPixel ( uiImage , m , row , color ) ;
                  }
                  m++;
              }
          }
      }
//   XPutImage((Display *)WC(D)->Dsp,WC(D)->ActWin,(GC)WC(D)->Gc,uiImage,0,0,xdl,ydl,xdu,ydu);
      kgPutImage ( D , uiImage , 0 , 0 , xdl , ydl , xdu-xdl+1 , ydu-ydl+1 ) ;
      XDestroyImage ( uiImage ) ;
      return 1;
  }
  int kg_gm_image ( DIALOG *D , void *tmp , int x0 , int y0 , int width , \
       int height , float transparency , float highfac ) \
  {
      int i , j , k , m , row , color , sx , sy , dx , dy , xoffset , yoffset , \
           xm , ym , xl , yl , xu , yu , xc , yc , xdl , xdu , ydl , ydu;
      float rzfac;
//    unsigned long *dest,*Imgdata,val;
      unsigned long val;
      unsigned char *Imgdata = NULL , *dest;
      unsigned char r , g , b;
      unsigned int a;
      unsigned long lastrow = 0;
      unsigned long pixel;
      unsigned int red , green , blue;
      unsigned char bg_red = 0xff , bg_green = 0xff , bg_blue = 0xff;
      double f , f1;
      unsigned int ALPHA = 255;
      int EVGAX , EVGAY;
      char *fullname = NULL;
      PixelPacket *pixels , *src;
      GMIMG *png = NULL , *pngrz = NULL;
      Image *img;
      int ximage_rowbytes;
      int w , h , ww;
      XImage *uiImage;
      char *cpt;
      int imgFile = 0;
      kgWC *wc;
      if ( tmp == NULL ) return 0;
      wc = D->wc;
      EVGAX = WC ( D )->EVGAX;
      EVGAY = WC ( D )->EVGAY;
      cpt = ( char * ) tmp;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) {
          fullname = _uiGetFileImageName ( D , ( char * ) ( cpt+2 ) ) ;
          if ( fullname == NULL ) return 0;
          else {
              png = ( GMIMG * ) _uiGetFileImage ( fullname ) ;
              free ( fullname ) ;
              if ( png == NULL ) {
                  printf ( "Failed to get %s\n" , ( char * ) ( cpt+2 ) ) ;
                  return 0;
              }
          }
//         else printf("Got Image\n");
          imgFile = 1;
      }
      else png = ( GMIMG * ) tmp;
      if ( x0 < 0 ) x0 = 0;
      if ( y0 < 0 ) y0 = 0;
      if ( ( x0+width ) > EVGAX ) width = EVGAX-x0-1;
      if ( ( y0+height ) > EVGAY ) height = EVGAY-y0-1;
      if ( png == NULL ) return 0;
      if ( png->bkgrclr >= 0 ) {
          color = wc->c_color;
          wcset_clr ( wc , png->bkgrclr ) ;
          _uiBox_Fill ( wc , x0 , y0 , x0+width , y0+height ) ;
          wcset_clr ( wc , color ) ;
      }
      ALPHA = 255* ( 1.-transparency ) +0.1;
      rzfac = png->rzfac;
      if ( rzfac != 1.0 ) {
          pngrz = uiResizegmImage ( png , rzfac ) ;
          if ( imgFile ) uiFreeImage ( png ) ;
          if ( pngrz == NULL ) {
              return 0;
          }
          png = pngrz;
      }
      xoffset = png->xoffset;
      yoffset = png->yoffset;
      xc = x0+width/2;
      yc = y0+height/2;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( png ) ;
      img = png->image;
//    w = png->image_width;
//    h = png->image_height;
      w = img->columns;
      h = img->rows;
      xm = ( w-xoffset ) /2+xoffset;
      ym = ( h-yoffset ) /2+yoffset;
      xl = xm -width/2; xu = xm+width-width/2;
      yl = ym -height/2; yu = ym+height -height/2;
      if ( ( xu < 0 ) || ( xl > w ) || ( yu < 0 ) || ( yl > h ) ) {
          if ( pngrz != NULL ) uiFreeImage ( pngrz ) ;
          else if ( imgFile ) uiFreeImage ( png ) ;
          return 0;
      }
// There is something to display
      if ( xl < 0 ) xl = 0;
      if ( xu > w ) xu = w;
      if ( yl < 0 ) yl = 0;
      if ( yu > h ) yu = h;
      dx = xm - xc;
      dy = ym - yc;
      xdl = xl-dx;
      xdu = xu-dx;
      ydl = yl -dy;
      ydu = yu -dy;
//    uiImage = XGetImage((Display *)WC(D)->Dsp,WC(D)->ActWin,xdl,ydl,xdu-xdl+1,ydu-ydl+1,0xffffffff,ZPixmap);
//TCBTCB
      uiImage = kg_GetImage ( D , xdl , ydl , xdu-xdl+1 , ydu-ydl+1 ) ;
      ximage_rowbytes = uiImage->bytes_per_line;
      Imgdata = ( unsigned char * ) uiImage->data;
      row = -1;
      for ( i = 0; i < h; ++i ) {
          if ( i < ( yl ) ) continue;
          if ( i >= yu ) break;
          src = pixels + i*w;
          row++;
          dest = Imgdata + row*ximage_rowbytes;
          m = 0;
          k = -1;
          ww = w;
          if ( png->image_channels == 3 ) {
              for ( j = 0;j < ww; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src++;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  red = src->red;
                  green = src->green;
                  blue = src->blue;
                  src++;
                  *dest = blue; *dest++;
                  *dest = green; *dest++;
                  *dest = red; *dest++;
                  *dest = 0xff; *dest++;
                  m++;
              }
              } else /* if ( image_channels == 4 ) */ {
              for ( j = 0;j < ww; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src++;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  r = src->red;
                  g = src->green;
                  b = src->blue;
                  a = 255 - src->opacity;
                  src++;
//                    a =a*(1.-transparency);
                  if ( a == 255 ) {
                      red = r;
                      green = g;
                      blue = b;
                      } else if ( a == 0 ) {
#if 1
                      dest += 4;
                      m++;
                      continue;
#else
                      red = bg_red;
                      green = bg_green;
                      blue = bg_blue;
#endif
                      } else {
                      f = a/255.0;
                      f1 = 1-f;
                      bg_red = dest [ 2 ] ;
                      bg_green = dest [ 1 ] ;
                      bg_blue = dest [ 0 ] ;
                      red = f*r+f1 *bg_red;
                      green = f*g+f1 *bg_green;
                      blue = f*b+f1 *bg_blue;
                  }
                  blue *= highfac; if ( blue > 255 ) blue = 255;
                  green *= highfac; if ( green > 255 ) green = 255;
                  red *= highfac; if ( red > 255 ) red = 255;
                  *dest = blue; *dest++;
                  *dest = green; *dest++;
                  *dest = red; *dest++;
//TCB
//                    *dest = 0xff;  *dest++;
                  a += *dest; if ( a > 255 ) a = 255;
                  *dest = a; *dest++;
                  m++;
              }
          }
      }
//   XPutImage((Display *)WC(D)->Dsp,WC(D)->ActWin,(GC)WC(D)->Gc,uiImage,0,0,xdl,ydl,xdu,ydu);
//   kgPutImage(D,uiImage,0,0,xdl,ydl,xdu,ydu);
      kgPutImage ( D , uiImage , 0 , 0 , xdl , ydl , xdu-xdl+1 , ydu-ydl+1 ) ;
      XDestroyImage ( uiImage ) ;
      if ( pngrz != NULL ) uiFreeImage ( pngrz ) ;
      else if ( imgFile ) uiFreeImage ( png ) ;
      return 1;
  }
  void *kgGetImageCopy_o ( void *D , void *img ) {
/* 
   if img is an image a copy is provided 
   else image from file is got and returned
*/
      char *cpt , *fullname = NULL;
      FMGIMG *fmg;
      void *retimg;
      if ( img == NULL ) return NULL;
      cpt = ( char * ) img;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) {
          if ( D != NULL ) {
              fullname = _uiGetFileImageName ( D , ( char * ) ( cpt+2 ) ) ;
              if ( fullname == NULL ) return NULL;
              retimg = kgGetImage ( fullname ) ;
              free ( fullname ) ;
              return retimg;
          }
          else return kgGetImage ( cpt+2 ) ;
      }
      else {
          fmg = ( FMGIMG * ) img;
          if ( strcmp ( fmg->Sign , "FMG" ) == 0 ) {
              char tmpfile [ 100 ] , *tmpdir;
              void *gmimg;
              tmpdir = ui_mktmpdir ( ) ;
              strcpy ( tmpfile , tmpdir ) ;
              strcat ( tmpfile , "/" ) ;
              strcat ( tmpfile , fmg->flname ) ;
              uiFmgToFile ( fmg->image_data , fmg->size , tmpfile ) ;
              gmimg = ( GMIMG * ) kgGetImage ( tmpfile ) ;
              remove ( tmpfile ) ;
              rmdir ( tmpdir ) ;
              free ( tmpdir ) ;
              return gmimg;
          }
          else return kgResizeImage ( img , 1.0 ) ;
      }
  }
  void *kgGetImageCopy ( void *D , void *img ) {
/* 
   if img is an image a copy is provided 
   else image from file is got and returned
*/
      char *cpt , *fullname = NULL;
      FMGIMG *fmg;
      void *retimg;
      if ( img == NULL ) return NULL;
      cpt = ( char * ) img;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) {
          if ( D != NULL ) {
              fullname = _uiGetFileImageName ( D , ( char * ) ( cpt+2 ) ) ;
              if ( fullname == NULL ) return NULL;
              retimg = kgGetImage ( fullname ) ;
              free ( fullname ) ;
              return retimg;
          }
          else return kgGetImage ( cpt+2 ) ;
      }
      else {
          fmg = ( FMGIMG * ) img;
          if ( strcmp ( fmg->Sign , "FMG" ) == 0 ) {
              GMIMG *gmimg;
              gmimg = ( GMIMG * ) kgGetInlineImage ( fmg->image_data , fmg->size ) ;
              return gmimg;
          }
          else return kgResizeImage ( img , 1.0 ) ;
      }
  }
  void kgImage_o ( void *Dtmp , void *tmp , int x0 , int y0 , int width , \
       int height , float transparency , float highfac ) \
  {
/*
    width is X-direction
   height is Y direction
*/
      DIALOG *D;
      int EVGAX , EVGAY;
      char buf [ 30 ] ;
      int h , w , i , j , k , m , row , color , sx , sy , dx , dy , val;
      int *pt;
      char *cpt , *fullname = NULL;
      JPGIMG *img;
      FMGIMG *fmg;
      PNGIMG *png;
      XImage *uiImage;
      D = ( DIALOG * ) Dtmp;
      int XpmR = 0;
      unsigned long ALPHA;
      int RedMaskPos , GreenMaskPos , BlueMaskPos;
      if ( tmp == NULL ) return;
      EVGAX = D->evgax;
      EVGAY = D->evgay;
      ALPHA = 255* ( 1.-transparency ) ;
      ALPHA = 255;
      ALPHA = ALPHA << 24;
      cpt = ( char * ) tmp;
      if ( x0 < 0 ) x0 = 0;
      if ( y0 < 0 ) y0 = 0;
      if ( ( x0+width ) > EVGAX ) width = EVGAX-x0-1;
      if ( ( y0+height ) > EVGAY ) height = EVGAY-y0-1;
      if ( ( width <= 0 ) || ( height <= 0 ) ) return;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) {
// NEW CODE using gm
          fullname = _uiGetFileImageName ( D , ( char * ) ( cpt+2 ) ) ;
          if ( fullname == NULL ) return;
          else {
#if 0
#if 1
              png = ReadPngImage ( fullname ) ;
              if ( png != NULL ) {
                  kg_png_image ( D , png , x0 , y0 , width , height , transparency , highfac ) ;
                  uiFreeImage ( png ) ;
                  free ( fullname ) ;
                  return;
              }
#endif
              img = ( JPGIMG * ) _uiGetFileImage ( fullname ) ;
              free ( fullname ) ;
              if ( img == NULL ) {
//            printf("Failed to get %s\n",(char *)(cpt+2));
                  return;
              }
              kg_gm_image ( D , ( void * ) img , x0 , y0 , width , \
                   height , transparency , highfac ) ;
              uiFreeImage ( img ) ;
#else
              GMIMG *gmimg;
              gmimg = ( GMIMG * ) uiGetgmImage ( fullname ) ;
              kg_gm_image ( D , gmimg , x0 , y0 , width , \
                   height , transparency , highfac ) ;
              uiFreeImage ( gmimg ) ;
              free ( fullname ) ;
#endif
              return;
          }
      }
      else fmg = ( FMGIMG * ) tmp;
      if ( fmg == NULL ) return;
      if ( strcmp ( fmg->Sign , "IMG" ) == 0 ) {kg_gm_image ( D , tmp , \
           x0 , y0 , width , height , transparency , highfac ) ;
      return;}
      if ( strcmp ( fmg->Sign , "PNG" ) == 0 ) {kg_png_image ( D , tmp , \
           x0 , y0 , width , height , transparency , highfac ) ;
      return;}
      if ( strcmp ( fmg->Sign , "FMG" ) == 0 ) {
          char tmpfile [ 100 ] , *tmpdir;
          FMGIMG *img;
          img = ( FMGIMG * ) tmp;
          tmpdir = ui_mktmpdir ( ) ;
          strcpy ( tmpfile , tmpdir ) ;
          strcat ( tmpfile , "/" ) ;
          strcat ( tmpfile , img->flname ) ;
          uiFmgToFile ( img->image_data , img->size , tmpfile ) ;
#if 0
          if ( CheckPng ( ( unsigned char * ) ( img->image_data ) ) ) {
              PNGIMG *gmimg;
              gmimg = ( PNGIMG * ) ReadPngImage ( tmpfile ) ;
              if ( gmimg != NULL ) {
                  kg_png_image ( D , gmimg , x0 , y0 , width , \
                       height , transparency , highfac ) ;
                  uiFreeImage ( gmimg ) ;
              }
              else printf ( "ReadPngImage: Image NULL\n" ) ;
          }
//       else {
#else
          {
#endif
              GMIMG *gmimg;
              gmimg = ( GMIMG * ) uiGetgmImage ( tmpfile ) ;
              kg_gm_image ( D , gmimg , x0 , y0 , width , \
                   height , transparency , highfac ) ;
              uiFreeImage ( gmimg ) ;
          }
          remove ( tmpfile ) ;
          rmdir ( tmpdir ) ;
          free ( tmpdir ) ;
          return;
      }
#if 0
      if ( strcmp ( img->Sign , "JPG" ) != 0 ) {
//       return xpm_image(tmp,x0,y0,width,height);
          printf ( "Unsupported Image\n" ) ;
          return ;
      }
//TCB as on 13/10/12
      h = img->row;
      w = img->col;
      {
          pt = img->matchcol;
          sx = sy = 0;
          dx = ( width - w ) /2;
          dy = ( height - h ) /2;
          if ( dx > 0 ) x0 += dx;
          else sx = -dx;
          if ( dy > 0 ) y0 += dy;
          else sy = -dy;
          uiImage = kg_GetImage ( D , x0 , y0 , w-2*sx , h-2*sy ) ;
          RedMaskPos = GetNoOfBits ( uiImage->red_mask ) ;
          GreenMaskPos = GetNoOfBits ( uiImage->green_mask ) ;
          BlueMaskPos = GetNoOfBits ( uiImage->blue_mask ) ;
          row = -1;
          for ( i = 0;i < h;i++ ) {
              pt = img->matchcol+ i*w;
              if ( i < sy ) continue;
              if ( i >= ( h-sy ) ) break;
              row++;
              m = 0;
              k = -1;
              for ( j = 0;j < w;j++ ) {
                  k++;
                  if ( ( k ) < sx ) continue;
                  if ( ( k ) >= ( w-sx ) ) break;
                  val = pt [ j ] ;
                  color = BLUE_VAL + GREEN_VAL + RED_VAL ;
                  color = color | ALPHA;
                  if ( color >= 0 ) XPutPixel ( uiImage , m , row , color ) ;
                  m++;
              }
          }
          kgPutImage ( D , uiImage , 0 , 0 , x0 , y0 , w-2*sx , h-2*sy ) ;
          XDestroyImage ( uiImage ) ;
      }
#if 1
      if ( XpmR ) {
          free ( img->matchcol ) ;
          free ( img ) ;
      }
#endif
//   UpdateScreen();
#else
      kg_gm_image ( D , tmp , x0 , y0 , width , height , transparency , highfac ) ;
#endif
      return ;
  }
  void kgImage ( void *Dtmp , void *tmp , int x0 , int y0 , int width , \
       int height , float transparency , float highfac ) \
  {
/*
    width is X-direction
   height is Y direction
*/
      DIALOG *D;
      int EVGAX , EVGAY;
      char buf [ 30 ] ;
      int h , w , i , j , k , m , row , color , sx , sy , dx , dy , val;
      int *pt;
      char *cpt , *fullname = NULL;
      JPGIMG *img;
      FMGIMG *fmg;
      PNGIMG *png;
      XImage *uiImage;
      D = ( DIALOG * ) Dtmp;
      int XpmR = 0;
      unsigned long ALPHA;
      int RedMaskPos , GreenMaskPos , BlueMaskPos;
      if ( tmp == NULL ) return;
      EVGAX = D->evgax;
      EVGAY = D->evgay;
      ALPHA = 255* ( 1.-transparency ) ;
      ALPHA = 255;
      ALPHA = ALPHA << 24;
      cpt = ( char * ) tmp;
      if ( x0 < 0 ) x0 = 0;
      if ( y0 < 0 ) y0 = 0;
      if ( ( x0+width ) > EVGAX ) width = EVGAX-x0-1;
      if ( ( y0+height ) > EVGAY ) height = EVGAY-y0-1;
      if ( ( width <= 0 ) || ( height <= 0 ) ) return;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) {
// NEW CODE using gm
          fullname = _uiGetFileImageName ( D , ( char * ) ( cpt+2 ) ) ;
          if ( fullname == NULL ) return;
          else {
#if 0
#if 1
              png = ReadPngImage ( fullname ) ;
              if ( png != NULL ) {
                  kg_png_image ( D , png , x0 , y0 , width , height , transparency , highfac ) ;
                  uiFreeImage ( png ) ;
                  free ( fullname ) ;
                  return;
              }
#endif
              img = ( JPGIMG * ) _uiGetFileImage ( fullname ) ;
              free ( fullname ) ;
              if ( img == NULL ) {
//            printf("Failed to get %s\n",(char *)(cpt+2));
                  return;
              }
              kg_gm_image ( D , ( void * ) img , x0 , y0 , width , \
                   height , transparency , highfac ) ;
              uiFreeImage ( img ) ;
#else
              GMIMG *gmimg;
              gmimg = ( GMIMG * ) uiGetgmImage ( fullname ) ;
              kg_gm_image ( D , gmimg , x0 , y0 , width , \
                   height , transparency , highfac ) ;
              uiFreeImage ( gmimg ) ;
              free ( fullname ) ;
#endif
              return;
          }
      }
      else fmg = ( FMGIMG * ) tmp;
      if ( fmg == NULL ) return;
      if ( strcmp ( fmg->Sign , "IMG" ) == 0 ) {kg_gm_image ( D , tmp , \
           x0 , y0 , width , height , transparency , highfac ) ;
      return;}
      if ( strcmp ( fmg->Sign , "PNG" ) == 0 ) {kg_png_image ( D , tmp , \
           x0 , y0 , width , height , transparency , highfac ) ;
      return;}
      if ( strcmp ( fmg->Sign , "FMG" ) == 0 ) {
          GMIMG *gmimg;
          FMGIMG *img;
          img = ( FMGIMG * ) tmp;
          gmimg = ( GMIMG * ) kgGetInlineImage ( img->image_data , img->size ) ;
          kg_gm_image ( D , gmimg , x0 , y0 , width , \
               height , transparency , highfac ) ;
          uiFreeImage ( gmimg ) ;
          return ;
      }
#if 0
      if ( strcmp ( img->Sign , "JPG" ) != 0 ) {
//       return xpm_image(tmp,x0,y0,width,height);
          printf ( "Unsupported Image\n" ) ;
          return ;
      }
//TCB as on 13/10/12
      h = img->row;
      w = img->col;
      {
          pt = img->matchcol;
          sx = sy = 0;
          dx = ( width - w ) /2;
          dy = ( height - h ) /2;
          if ( dx > 0 ) x0 += dx;
          else sx = -dx;
          if ( dy > 0 ) y0 += dy;
          else sy = -dy;
          uiImage = kg_GetImage ( D , x0 , y0 , w-2*sx , h-2*sy ) ;
          RedMaskPos = GetNoOfBits ( uiImage->red_mask ) ;
          GreenMaskPos = GetNoOfBits ( uiImage->green_mask ) ;
          BlueMaskPos = GetNoOfBits ( uiImage->blue_mask ) ;
          row = -1;
          for ( i = 0;i < h;i++ ) {
              pt = img->matchcol+ i*w;
              if ( i < sy ) continue;
              if ( i >= ( h-sy ) ) break;
              row++;
              m = 0;
              k = -1;
              for ( j = 0;j < w;j++ ) {
                  k++;
                  if ( ( k ) < sx ) continue;
                  if ( ( k ) >= ( w-sx ) ) break;
                  val = pt [ j ] ;
                  color = BLUE_VAL + GREEN_VAL + RED_VAL ;
                  color = color | ALPHA;
                  if ( color >= 0 ) XPutPixel ( uiImage , m , row , color ) ;
                  m++;
              }
          }
          kgPutImage ( D , uiImage , 0 , 0 , x0 , y0 , w-2*sx , h-2*sy ) ;
          XDestroyImage ( uiImage ) ;
      }
#if 1
      if ( XpmR ) {
          free ( img->matchcol ) ;
          free ( img ) ;
      }
#endif
//   UpdateScreen();
#else
      kg_gm_image ( D , tmp , x0 , y0 , width , height , transparency , highfac ) ;
#endif
      return ;
  }
  void *kgFMGtogmImage_o ( void *tmp ) {
      char tmpfile [ 100 ] , *tmpdir;
      FMGIMG *img;
      GMIMG *gmimg = NULL;
      tmpdir = ui_mktmpdir ( ) ;
      strcpy ( tmpfile , tmpdir ) ;
      strcat ( tmpfile , "/" ) ;
      img = ( FMGIMG * ) tmp;
      strcat ( tmpfile , "img->flname" ) ;
      uiFmgToFile ( img->image_data , img->size , tmpfile ) ;
      gmimg = ( GMIMG * ) uiGetgmImage ( tmpfile ) ;
      remove ( tmpfile ) ;
      rmdir ( tmpdir ) ;
      free ( tmpdir ) ;
      return gmimg;
  }
  void *kgFMGtogmImage ( void *tmp ) {
      FMGIMG *img;
      GMIMG *gmimg = NULL;
      img = ( FMGIMG * ) tmp;
      gmimg = ( GMIMG * ) kgGetInlineImage ( img->image_data , img->size ) ;
      return gmimg;
  }
  int kgRootImage ( void *tmp ) {
      int i , j , k , m , row , color , sx , sy , dx , xoffset , yoffset , \
           xm , ym , xl , yl , xu , yu , xc , yc , xdl , xdu , ydl , ydu;
      float rzfac;
      float highfac = 1.0 , transparency = 0.0;
      unsigned long val;
      unsigned char*Imgdata = NULL , *dest;
      unsigned char r , g , b;
      unsigned int a;
      unsigned long lastrow = 0;
      unsigned long pixel;
      unsigned int red , green , blue;
      unsigned char bg_red = 0xff , bg_green = 0xff , bg_blue = 0xff;
      double f , f1;
      unsigned int ALPHA = 255;
      int EVGAX , EVGAY;
      char *fullname = NULL;
      PixelPacket *pixels , *src;
      GMIMG *png = NULL , *pngrz = NULL;
      Image *img;
      int ximage_rowbytes;
      int w , h , ww;
      XImage *uiImage;
      char *cpt;
      int imgFile = 0;
      kgWC *wc;
      Display *Dsp;
      Window Root;
      Pixmap Pix;
      Atom BackgroundPixmapId;
      unsigned int Dpth = 32;
      int x0 = 0 , y0 = 0 , width , height , dy;
      if ( tmp == NULL ) return 0;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
      Root = RootWindow ( Dsp , DefaultScreen ( Dsp ) ) ;
      Dpth = DisplayPlanes ( Dsp , DefaultScreen ( Dsp ) ) ;
      EVGAX = DisplayWidth ( Dsp , DefaultScreen ( Dsp ) ) ;
      EVGAY = DisplayHeight ( Dsp , DefaultScreen ( Dsp ) ) ;
      width = EVGAX;
      height = EVGAY;
      cpt = ( char * ) tmp;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) {
          fullname = ( char * ) ( cpt+2 ) ;
          if ( fullname [ 0 ] == '\0' ) {
              XCloseDisplay ( Dsp ) ;
              return 0;
          }
          else {
              png = ( GMIMG * ) _uiGetFileImage ( fullname ) ;
              if ( png == NULL ) {
                  printf ( "Failed to get %s\n" , ( char * ) ( cpt+2 ) ) ;
                  XCloseDisplay ( Dsp ) ;
                  return 0;
              }
          }
//         else printf("Got Image\n");
          imgFile = 1;
      }
      else png = ( GMIMG * ) tmp;
      if ( x0 < 0 ) x0 = 0;
      if ( y0 < 0 ) y0 = 0;
      if ( ( x0+width ) > EVGAX ) width = EVGAX-x0-1;
      if ( ( y0+height ) > EVGAY ) height = EVGAY-y0-1;
      if ( png == NULL ) return 0;
      ALPHA = 255* ( 1.-transparency ) ;
      rzfac = png->rzfac;
      xoffset = png->xoffset;
      yoffset = png->yoffset;
      xc = x0+width/2;
      yc = y0+height/2;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( png ) ;
      img = png->image;
      w = img->columns;
      h = img->rows;
      xm = ( w-xoffset ) /2+xoffset;
      ym = ( h-yoffset ) /2+yoffset;
      xl = xm -width/2; xu = xm+width-width/2;
      yl = ym -height/2; yu = ym+height -height/2;
      if ( ( xu < 0 ) || ( xl > w ) || ( yu < 0 ) || ( yl > h ) ) {
          if ( imgFile ) uiFreeImage ( png ) ;
          XCloseDisplay ( Dsp ) ;
          return 0;
      }
// There is something to display
      if ( xl < 0 ) xl = 0;
      if ( xu > w ) xu = w;
      if ( yl < 0 ) yl = 0;
      if ( yu > h ) yu = h;
      dx = xm - xc;
      dy = ym - yc;
      xdl = xl-dx;
      xdu = xu-dx;
      ydl = yl -dy;
      ydu = yu -dy;
//    uiImage = kg_GetImage(D,xdl,ydl,xdu-xdl+1,ydu-ydl+1);
      Pix = XCreatePixmap ( Dsp , Root , ( short ) ( xdu-xdl+1 ) , \
       ( short ) ( ydu-ydl+1 ) , Dpth ) ;
      uiImage = XGetImage ( Dsp , Pix , xdl , ydl , xdu-xdl+1 , \
           ydu-ydl+1 , 0xffffffff , ZPixmap ) ;
      ximage_rowbytes = uiImage->bytes_per_line;
      Imgdata = ( unsigned char * ) uiImage->data;
      row = -1;
      for ( i = 0; i < h; ++i ) {
          if ( i < ( yl ) ) continue;
          if ( i >= yu ) break;
          src = pixels + i*w;
          row++;
          dest = Imgdata + row*ximage_rowbytes;
          m = 0;
          k = -1;
          ww = w;
          if ( png->image_channels == 3 ) {
              for ( j = 0;j < ww; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src++;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  red = src->red;
                  green = src->green;
                  blue = src->blue;
                  src++;
                  *dest = blue; *dest++;
                  *dest = green; *dest++;
                  *dest = red; *dest++;
                  *dest = 0xff; *dest++;
                  m++;
              }
              } else /* if ( image_channels == 4 ) */ {
              for ( j = 0;j < ww; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src++;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  r = src->red;
                  g = src->green;
                  b = src->blue;
                  a = 255 - src->opacity;
                  src++;
//                    a =a*(1.-transparency);
                  if ( a == 255 ) {
                      red = r;
                      green = g;
                      blue = b;
                      } else if ( a == 0 ) {
#if 1
                      dest += 4;
                      m++;
                      continue;
#else
                      red = bg_red;
                      green = bg_green;
                      blue = bg_blue;
#endif
                      } else {
                      f = a/255.0;
                      f1 = 1-f;
                      bg_red = dest [ 2 ] ;
                      bg_green = dest [ 1 ] ;
                      bg_blue = dest [ 0 ] ;
                      red = f*r+f1 *bg_red;
                      green = f*g+f1 *bg_green;
                      blue = f*b+f1 *bg_blue;
                  }
                  blue *= highfac; if ( blue > 255 ) blue = 255;
                  green *= highfac; if ( green > 255 ) green = 255;
                  red *= highfac; if ( red > 255 ) red = 255;
                  *dest = blue; *dest++;
                  *dest = green; *dest++;
                  *dest = red; *dest++;
//TCB
//                    *dest = 0xff;  *dest++;
                  a += *dest; if ( a > 255 ) a = 255;
                  *dest = a; *dest++;
                  m++;
              }
          }
      }
//   kgPutImage(D,uiImage,0,0,xdl,ydl,xdu-xdl+1,ydu-ydl+1);
      GC Gc = XCreateGC ( Dsp , Root , 0 , NULL ) ;
      XPutImage ( Dsp , Pix , Gc , uiImage , 0 , 0 , \
           xdl , ydl , xdu-xdl+1 , ydu-ydl+1 ) ;
      BackgroundPixmapId = XInternAtom ( Dsp , "_XROOTPMAP_ID" , False ) ;
      XSetWindowBackgroundPixmap ( Dsp , Root , Pix ) ;
      XChangeProperty ( Dsp , Root , BackgroundPixmapId , \
           XA_PIXMAP , 32 , PropModeReplace , \
       ( unsigned char * ) & Pix , 1 ) ;
      XClearWindow ( Dsp , Root ) ;
      XFlush ( Dsp ) ;
      XDestroyImage ( uiImage ) ;
      XFreePixmap ( Dsp , Pix ) ;
      XFreeGC ( Dsp , Gc ) ;
      if ( imgFile ) uiFreeImage ( png ) ;
      XCloseDisplay ( Dsp ) ;
      return 1;
  }
  int kgWindowImage ( Window Root , void *tmp ) {
      int i , j , k , m , row , color , sx , sy , dx , xoffset , yoffset , \
           xm , ym , xl , yl , xu , yu , xc , yc , xdl , xdu , ydl , ydu;
      float rzfac;
      float highfac = 1.0 , transparency = 0.0;
      unsigned long val;
      unsigned char*Imgdata = NULL , *dest;
      unsigned char r , g , b;
      unsigned int a;
      unsigned long lastrow = 0;
      unsigned long pixel;
      unsigned int red , green , blue;
      unsigned char bg_red = 0xff , bg_green = 0xff , bg_blue = 0xff;
      double f , f1;
      unsigned int ALPHA = 255;
      int EVGAX , EVGAY;
      char *fullname = NULL;
      PixelPacket *pixels , *src;
      GMIMG *png = NULL , *pngrz = NULL;
      Image *img;
      int ximage_rowbytes;
      int w , h , ww;
      XImage *uiImage;
      char *cpt;
      int imgFile = 0;
      kgWC *wc;
      Display *Dsp;
//    Window Root;
      Pixmap Pix;
      Atom BackgroundPixmapId;
      unsigned int Dpth = 32;
      int x0 = 0 , y0 = 0 , width , height , dy;
      if ( tmp == NULL ) return 0;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
//    Root = RootWindow(Dsp,DefaultScreen(Dsp));
      Dpth = DisplayPlanes ( Dsp , DefaultScreen ( Dsp ) ) ;
      EVGAX = DisplayWidth ( Dsp , DefaultScreen ( Dsp ) ) ;
      EVGAY = DisplayHeight ( Dsp , DefaultScreen ( Dsp ) ) ;
      width = EVGAX;
      height = EVGAY;
      cpt = ( char * ) tmp;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) {
          fullname = ( char * ) ( cpt+2 ) ;
          if ( fullname [ 0 ] == '\0' ) {
              XCloseDisplay ( Dsp ) ;
              return 0;
          }
          else {
              png = ( GMIMG * ) _uiGetFileImage ( fullname ) ;
              if ( png == NULL ) {
                  printf ( "Failed to get %s\n" , ( char * ) ( cpt+2 ) ) ;
                  XCloseDisplay ( Dsp ) ;
                  return 0;
              }
          }
//         else printf("Got Image\n");
          imgFile = 1;
      }
      else png = ( GMIMG * ) tmp;
      if ( x0 < 0 ) x0 = 0;
      if ( y0 < 0 ) y0 = 0;
      if ( ( x0+width ) > EVGAX ) width = EVGAX-x0-1;
      if ( ( y0+height ) > EVGAY ) height = EVGAY-y0-1;
      if ( png == NULL ) return 0;
      ALPHA = 255* ( 1.-transparency ) ;
      rzfac = png->rzfac;
      xoffset = png->xoffset;
      yoffset = png->yoffset;
      xc = x0+width/2;
      yc = y0+height/2;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( png ) ;
      img = png->image;
      w = img->columns;
      h = img->rows;
      xm = ( w-xoffset ) /2+xoffset;
      ym = ( h-yoffset ) /2+yoffset;
      xl = xm -width/2; xu = xm+width-width/2;
      yl = ym -height/2; yu = ym+height -height/2;
      if ( ( xu < 0 ) || ( xl > w ) || ( yu < 0 ) || ( yl > h ) ) {
          if ( imgFile ) uiFreeImage ( png ) ;
          XCloseDisplay ( Dsp ) ;
          return 0;
      }
// There is something to display
      if ( xl < 0 ) xl = 0;
      if ( xu > w ) xu = w;
      if ( yl < 0 ) yl = 0;
      if ( yu > h ) yu = h;
      dx = xm - xc;
      dy = ym - yc;
      xdl = xl-dx;
      xdu = xu-dx;
      ydl = yl -dy;
      ydu = yu -dy;
//    uiImage = kg_GetImage(D,xdl,ydl,xdu-xdl+1,ydu-ydl+1);
      Pix = XCreatePixmap ( Dsp , Root , ( short ) ( xdu-xdl+1 ) , \
       ( short ) ( ydu-ydl+1 ) , Dpth ) ;
      uiImage = XGetImage ( Dsp , Pix , xdl , ydl , xdu-xdl+1 , \
           ydu-ydl+1 , 0xffffffff , ZPixmap ) ;
      ximage_rowbytes = uiImage->bytes_per_line;
      Imgdata = ( unsigned char * ) uiImage->data;
      row = -1;
      for ( i = 0; i < h; ++i ) {
          if ( i < ( yl ) ) continue;
          if ( i >= yu ) break;
          src = pixels + i*w;
          row++;
          dest = Imgdata + row*ximage_rowbytes;
          m = 0;
          k = -1;
          ww = w;
          if ( png->image_channels == 3 ) {
              for ( j = 0;j < ww; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src++;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  red = src->red;
                  green = src->green;
                  blue = src->blue;
                  src++;
                  *dest = blue; *dest++;
                  *dest = green; *dest++;
                  *dest = red; *dest++;
                  *dest = 0xff; *dest++;
                  m++;
              }
              } else /* if ( image_channels == 4 ) */ {
              for ( j = 0;j < ww; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src++;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  r = src->red;
                  g = src->green;
                  b = src->blue;
                  a = 255 - src->opacity;
                  src++;
//                    a =a*(1.-transparency);
                  if ( a == 255 ) {
                      red = r;
                      green = g;
                      blue = b;
                      } else if ( a == 0 ) {
#if 1
                      dest += 4;
                      m++;
                      continue;
#else
                      red = bg_red;
                      green = bg_green;
                      blue = bg_blue;
#endif
                      } else {
                      f = a/255.0;
                      f1 = 1-f;
                      bg_red = dest [ 2 ] ;
                      bg_green = dest [ 1 ] ;
                      bg_blue = dest [ 0 ] ;
                      red = f*r+f1 *bg_red;
                      green = f*g+f1 *bg_green;
                      blue = f*b+f1 *bg_blue;
                  }
                  blue *= highfac; if ( blue > 255 ) blue = 255;
                  green *= highfac; if ( green > 255 ) green = 255;
                  red *= highfac; if ( red > 255 ) red = 255;
                  *dest = blue; *dest++;
                  *dest = green; *dest++;
                  *dest = red; *dest++;
//TCB
//                    *dest = 0xff;  *dest++;
                  a += *dest; if ( a > 255 ) a = 255;
                  *dest = a; *dest++;
                  m++;
              }
          }
      }
//   kgPutImage(D,uiImage,0,0,xdl,ydl,xdu-xdl+1,ydu-ydl+1);
      GC Gc = XCreateGC ( Dsp , Root , 0 , NULL ) ;
#if 1
      XPutImage ( Dsp , Pix , Gc , uiImage , 0 , 0 , \
           xdl , ydl , xdu-xdl+1 , ydu-ydl+1 ) ;
      BackgroundPixmapId = XInternAtom ( Dsp , "_XROOTPMAP_ID" , False ) ;
      XSetWindowBackgroundPixmap ( Dsp , Root , Pix ) ;
      XChangeProperty ( Dsp , Root , BackgroundPixmapId , \
           XA_PIXMAP , 32 , PropModeReplace , \
       ( unsigned char * ) & Pix , 1 ) ;
#endif
      XClearWindow ( Dsp , Root ) ;
      XFlush ( Dsp ) ;
      XDestroyImage ( uiImage ) ;
      XFreePixmap ( Dsp , Pix ) ;
      XFreeGC ( Dsp , Gc ) ;
      if ( imgFile ) uiFreeImage ( png ) ;
      XCloseDisplay ( Dsp ) ;
      return 1;
  }
  int kgSetParentImage ( void *Tmp , void *Img ) {
/* Really working yet to find a method for
   Desktop top window 
*/
      DIALOG *D;
      Display *Dsp;
      Window Parent;
      D = ( DIALOG * ) Tmp;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
      Parent = ( Window ) D->parent;
      if ( Parent == 0 ) Parent = RootWindow ( Dsp , DefaultScreen ( Dsp ) ) ;
      kgWindowImage ( Parent , Img ) ;
      XCloseDisplay ( Dsp ) ;
/* right now useless code */
      return 1;
  }
  void *kgGetBackground ( void *Tmp , int x1 , int y1 , int x2 , int y2 ) {
  /* Getting the background of Current Window as image */
      int w , h , i , j , k;
      void *Img = NULL;
      XImage *uiImage;
      int ximage_rowbytes;
      PixelPacket *pixels , *dest;
      unsigned char*Imgdata = NULL , *src;
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      wc = D->wc;
      w = ( x2-x1 ) +1;
      h = ( y2-y1 ) +1;
//   uiImage = XGetImage(wc->Dsp,wc->ActWin,x1,y1,w,h,0xffffffff,ZPixmap);
      uiImage = kg_GetImage ( D , x1 , y1 , w , h ) ;
      Img = uiCreateCleangmImage ( w , h , 0 , 0 , 0 , 255 ) ;
      ximage_rowbytes = uiImage->bytes_per_line;
      Imgdata = ( unsigned char * ) uiImage->data;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( Img ) ;
      for ( i = 0; i < h; ++i ) {
          dest = pixels + i*w;
          src = Imgdata + i*ximage_rowbytes;
          for ( j = 0;j < w; j++ ) {
#if 1
              dest->blue = *src;src++;
              dest->green = *src;src++;
              dest->red = *src;src++;
              dest->opacity = 255- ( *src ) ;src++;
#else
              dest->red = 255;src++;
              dest->green = 100;src++;
              dest->blue = 100;src++;
              dest->opacity = 125 ;src++;
#endif
              dest++;
          }
      }
      XDestroyImage ( uiImage ) ;
      return Img;
  }
#if 0
/*
 * Copyright (c) 2003 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/* ====================================================================
 * Copyright (c) 1995-1999 The Apache Group.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the Apache Group
 *    for use in the Apache HTTP server project (http://www.apache.org/)."
 *
 * 4. The names "Apache Server" and "Apache Group" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache"
 *    nor may "Apache" appear in their names without prior written
 *    permission of the Apache Group.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the Apache Group
 *    for use in the Apache HTTP server project (http://www.apache.org/)."
 *
 * THIS SOFTWARE IS PROVIDED BY THE APACHE GROUP ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE APACHE GROUP OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Group and was originally based
 * on public domain software written at the National Center for
 * Supercomputing Applications, University of Illinois, Urbana-Champaign.
 * For more information on the Apache Group and the Apache HTTP server
 * project, please see <http://www.apache.org/>.
 *
 */
/* Base64 encoder/decoder. Originally Apache file ap_base64.c
 */
#include <string.h>
/* aaaack but it's fast and const should make it shared text page. */
  static const unsigned char pr2six [ 256 ] = {
    /* ASCII table */
  64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , \
       64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , \
       64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 62 , 64 , \
       64 , 64 , 63 , 52 , 53 , 54 , 55 , 56 , 57 , 58 , 59 , 60 , 61 , 64 , 64 , \
       64 , 64 , 64 , 64 , 64 , 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , \
       12 , 13 , 14 , 15 , 16 , 17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 64 , \
       64 , 64 , 64 , 64 , 64 , 26 , 27 , 28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , \
       36 , 37 , 38 , 39 , 40 , 41 , 42 , 43 , 44 , 45 , 46 , 47 , 48 , 49 , 50 , \
       51 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , \
       64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , \
       64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , \
       64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , \
       64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , \
       64 , 64 , 64 , 64 , 6 4 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , \
       64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , \
       64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , \
       64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
  int Base64decode_len ( const char *bufcoded ) {
      int nbytesdecoded;
      register const unsigned char *bufin;
      register int nprbytes;
      bufin = ( const unsigned char * ) bufcoded;
      while ( pr2six [ * ( bufin++ ) ] <= 63 ) ;
      nprbytes = ( bufin - ( const unsigned char * ) bufcoded ) - 1;
      nbytesdecoded = ( ( nprbytes + 3 ) / 4 ) * 3;
      return nbytesdecoded + 1;
  }
  int Base64decode ( char *bufplain , const char *bufcoded ) {
      int nbytesdecoded;
      register const unsigned char *bufin;
      register unsigned char *bufout;
      register int nprbytes;
      bufin = ( const unsigned char * ) bufcoded;
      while ( pr2six [ * ( bufin++ ) ] <= 63 ) ;
      nprbytes = ( bufin - ( const unsigned char * ) bufcoded ) - 1;
      nbytesdecoded = ( ( nprbytes + 3 ) / 4 ) * 3;
      bufout = ( unsigned char * ) bufplain;
      bufin = ( const unsigned char * ) bufcoded;
      while ( nprbytes > 4 ) {
          * ( bufout++ ) = ( unsigned char ) ( pr2six [ *bufin ] << 2 \
           | pr2six [ bufin [ 1 ] ] >> 4 ) ;
          * ( bufout++ ) = ( unsigned char ) ( pr2six [ bufin [ 1 ] ] << 4 \
           | pr2six [ bufin [ 2 ] ] >> 2 ) ;
          * ( bufout++ ) = ( unsigned char ) ( pr2six [ bufin [ 2 ] ] << 6 \
           | pr2six [ bufin [ 3 ] ] ) ;
          bufin += 4;
          nprbytes -= 4;
      }
    /* Note: (nprbytes == 1) would be an error, so just ingore that case */
      if ( nprbytes > 1 ) {
          * ( bufout++ ) = ( unsigned char ) ( pr2six [ *bufin ] << 2 \
           | pr2six [ bufin [ 1 ] ] >> 4 ) ;
      }
      if ( nprbytes > 2 ) {
          * ( bufout++ ) = ( unsigned char ) ( pr2six [ bufin [ 1 ] ] << 4 \
           | pr2six [ bufin [ 2 ] ] >> 2 ) ;
      }
      if ( nprbytes > 3 ) {
          * ( bufout++ ) = ( unsigned char ) ( pr2six [ bufin [ 2 ] ] << 6 \
           | pr2six [ bufin [ 3 ] ] ) ;
      }
      * ( bufout++ ) = '\0';
      nbytesdecoded -= ( 4 - nprbytes ) & 3;
      return nbytesdecoded;
  }
  static const char basis_64 [ ] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
      
  int Base64encode_len ( int len ) {
      return ( ( len + 2 ) / 3 * 4 ) + 1;
  }
  int Base64encode ( char *encoded , const char *string , int len ) {
      int i , elen;
      char *p;
      p = encoded;
      for ( i = 0; i < len - 2; i += 3 ) {
          *p++= basis_64 [ ( string [ i ] >> 2 ) & 0x3F ] ;
          *p++= basis_64 [ ( ( string [ i ] & 0x3 ) << 4 ) | \
           ( ( int ) ( string [ i + 1 ] & 0xF0 ) >> 4 ) ] ;
          *p++= basis_64 [ ( ( string [ i + 1 ] & 0xF ) << 2 ) \
           | ( ( int ) ( string [ i + 2 ] & 0xC0 ) >> 6 ) ] ;
          *p++= basis_64 [ string [ i + 2 ] & 0x3F ] ;
      }
      if ( i < len ) {
          *p++= basis_64 [ ( string [ i ] >> 2 ) & 0x3F ] ;
          if ( i == ( len - 1 ) ) {
              *p++= basis_64 [ ( ( string [ i ] & 0x3 ) << 4 ) ] ;
              *p++= '=';
          }
          else {
              *p++= basis_64 [ ( ( string [ i ] & 0x3 ) << 4 ) | \
               ( ( int ) ( string [ i + 1 ] & 0xF0 ) >> 4 ) ] ;
              *p++= basis_64 [ ( ( string [ i + 1 ] & 0xF ) << 2 ) ] ;
          }
          *p++= '=';
      }
      *p++= '\0';
      return p - encoded;
  }
#else
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
  static char encoding_table [ ] = {'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , \
       'H' , 'I' , 'J' , 'K' , 'L' , 'M' , 'N' , 'O' , 'P' , 'Q' , 'R' , 'S' , \
       'T' , 'U' , 'V' , 'W' , 'X' , 'Y' , 'Z' , 'a' , 'b' , 'c' , 'd' , 'e' , \
       'f' , 'g' , 'h' , 'i' , 'j' , 'k' , 'l' , 'm' , 'n' , 'o' , 'p' , 'q' , \
       'r' , 's' , 't' , 'u' , 'v' , 'w' , 'x' , 'y' , 'z' , '0' , '1' , \
       '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '+' , '/'};
  static char *decoding_table = NULL;
  static int mod_table [ ] = {0 , 2 , 1};
  void build_decoding_table ( ) {
      int i;
      decoding_table = Malloc ( 256 ) ;
      for ( i = 0; i < 64; i++ ) decoding_table [ ( unsigned char ) encoding_table [ i ] ] = i;
          
  }
  int Base64encode_len ( int len ) {
      return ( 4 * ( ( len + 2 ) / 3 ) ) ;
  }
  int Base64encode ( char *encoded_data , const unsigned char *data , \
       int input_length ) \
  {
      size_t output_length;
      int i , j;
      uint32_t octet_a , octet_b , octet_c , triple;
      output_length = 4 * ( ( input_length + 2 ) / 3 ) ;
      if ( encoded_data == NULL ) return 0;
      for ( i = 0 , j = 0; i < input_length; ) {
          octet_a = i < input_length ? ( unsigned char ) data [ i++ ] : 0;
          octet_b = i < input_length ? ( unsigned char ) data [ i++ ] : 0;
          octet_c = i < input_length ? ( unsigned char ) data [ i++ ] : 0;
          triple = ( octet_a << 0x10 ) + ( octet_b << 0x08 ) + octet_c;
          encoded_data [ j++ ] = encoding_table [ ( triple >> 3 * 6 ) & 0x3F ] ;
          encoded_data [ j++ ] = encoding_table [ ( triple >> 2 * 6 ) & 0x3F ] ;
          encoded_data [ j++ ] = encoding_table [ ( triple >> 1 * 6 ) & 0x3F ] ;
          encoded_data [ j++ ] = encoding_table [ ( triple >> 0 * 6 ) & 0x3F ] ;
      }
      for ( i = 0; i < mod_table [ input_length % 3 ] ; i++ ) encoded_data [ output_length - 1 - i ] = '=';
      return output_length;
  }
#endif
  void * kgGetRootRawImage ( int xo , int yo , int wd , int ht ) {
      void *Img;
      int i , j , k , m , row , color , sx , sy , dx , xoffset , yoffset , \
           xm , ym , xl , yl , xu , yu , xc , yc , xdl , xdu , ydl , ydu;
      float rzfac;
      float highfac = 1.0 , transparency = 0.0;
      unsigned long val;
      unsigned char*Imgdata = NULL , *src;
      unsigned char r , g , b;
      unsigned int a;
      unsigned long lastrow = 0;
      unsigned long pixel;
      unsigned int red , green , blue;
      unsigned char bg_red = 0xff , bg_green = 0xff , bg_blue = 0xff;
      double f , f1;
      unsigned int ALPHA = 255;
      int EVGAX , EVGAY;
      char *fullname = NULL;
      unsigned char *rdest , *gdest , *bdest;
      unsigned char *img;
      int ximage_rowbytes;
      int w , h , ww , gpos , bpos;
      XImage *uiImage;
      char *cpt;
      int imgFile = 0;
      kgWC *wc;
      Display *Dsp;
      Window Root;
      Pixmap Pix;
      Atom BackgroundPixmapId;
      unsigned int Dpth = 32;
      int x0 = 0 , y0 = 0 , width , height , dy;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
      Root = RootWindow ( Dsp , DefaultScreen ( Dsp ) ) ;
      Dpth = DisplayPlanes ( Dsp , DefaultScreen ( Dsp ) ) ;
      EVGAX = DisplayWidth ( Dsp , DefaultScreen ( Dsp ) ) ;
      EVGAY = DisplayHeight ( Dsp , DefaultScreen ( Dsp ) ) ;
      width = EVGAX;
      height = EVGAY;
      h = height; w = width;
      Pix = XCreatePixmap ( Dsp , Root , ( short ) \
       ( EVGAX ) , ( short ) ( EVGAY ) , Dpth ) ;
      uiImage = XGetImage ( Dsp , Root , 0 , 0 , EVGAX , \
           EVGAY , 0xffffffff , ZPixmap ) ;
      ximage_rowbytes = uiImage->bytes_per_line;
      Imgdata = ( unsigned char * ) uiImage->data;
      Img = ( unsigned char * ) Malloc ( ht*wd*3 ) ;
      rdest = Img;
      k = 0;
      for ( i = 0; i < h; ++i ) {
          src = Imgdata + i*ximage_rowbytes;
          if ( i < yo ) continue;
          if ( i >= ( yo+ht ) ) break;
          for ( j = 0;j < w; j++ ) {
              if ( j < xo ) {src+= 4;continue;}
              if ( j >= ( xo+wd ) ) break;
              blue = *src;src++; //blue
              green = *src;src++; //green
              red = *src;src++; //red
              src++;
              rdest [ k++ ] = red;
              rdest [ k++ ] = green;
              rdest [ k++ ] = blue;
          }
      }
      XDestroyImage ( uiImage ) ;
      XFreePixmap ( Dsp , Pix ) ;
      XCloseDisplay ( Dsp ) ;
      return Img;
  }
  void * kgMakeImageFromRaw ( unsigned char *Imgdata , int wd , int ht ) {
      void *Img;
      int i , j , k , m , row , color , sx , sy , dx , xoffset , yoffset , \
           xm , ym , xl , yl , xu , yu , xc , yc , xdl , xdu , ydl , ydu;
      float rzfac;
      float highfac = 1.0 , transparency = 0.0;
      unsigned long val;
      unsigned char *src , *rsrc , *gsrc , *bsrc;
      unsigned char r , g , b;
      unsigned int a;
      unsigned long lastrow = 0;
      unsigned long pixel;
      unsigned int red , green , blue;
      unsigned char bg_red = 0xff , bg_green = 0xff , bg_blue = 0xff;
      double f , f1;
      unsigned int ALPHA = 255;
      int EVGAX , EVGAY;
      char *fullname = NULL;
      PixelPacket *pixels , *dest;
      GMIMG *png = NULL , *pngrz = NULL;
      Image *img;
      int ximage_rowbytes;
      int w , h , ww;
      char *cpt;
      int imgFile = 0;
      kgWC *wc;
      Display *Dsp;
      Window Root;
      Pixmap Pix;
      Atom BackgroundPixmapId;
      unsigned int Dpth = 32;
      int x0 = 0 , y0 = 0 , width , height , dy;
#if 0
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
      Root = RootWindow ( Dsp , DefaultScreen ( Dsp ) ) ;
      Dpth = DisplayPlanes ( Dsp , DefaultScreen ( Dsp ) ) ;
      EVGAX = DisplayWidth ( Dsp , DefaultScreen ( Dsp ) ) ;
      EVGAY = DisplayHeight ( Dsp , DefaultScreen ( Dsp ) ) ;
      width = EVGAX;
      height = EVGAY;
      h = height; w = width;
#endif
      Img = uiCreateCleangmImage ( wd , ht , 0 , 0 , 0 , 255 ) ;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( Img ) ;
      rsrc = Imgdata;
      k = 0;
      for ( i = 0; i < ht; ++i ) {
          dest = pixels + i*w;
          src = Imgdata + i*ximage_rowbytes;
          for ( j = 0;j < wd; j++ ) {
              dest->red = *rsrc;rsrc++;
              dest->green = *rsrc;rsrc++;
              dest->blue = *rsrc;rsrc++;
              dest->opacity = 255;
              dest++;
          }
      }
#if 0
      XCloseDisplay ( Dsp ) ;
#endif
      return Img;
  }
  void * kgGetRootImage ( void ) {
      void *Img;
      int i , j , k , m , row , color , sx , sy , dx , xoffset , yoffset , \
           xm , ym , xl , yl , xu , yu , xc , yc , xdl , xdu , ydl , ydu;
      float rzfac;
      float highfac = 1.0 , transparency = 0.0;
      unsigned long val;
      unsigned char*Imgdata = NULL , *src;
      unsigned char r , g , b;
      unsigned int a;
      unsigned long lastrow = 0;
      unsigned long pixel;
      unsigned int red , green , blue;
      unsigned char bg_red = 0xff , bg_green = 0xff , bg_blue = 0xff;
      double f , f1;
      unsigned int ALPHA = 255;
      int EVGAX , EVGAY;
      char *fullname = NULL;
      PixelPacket *pixels , *dest;
      GMIMG *png = NULL , *pngrz = NULL;
      Image *img;
      int ximage_rowbytes;
      int w , h , ww;
      XImage *uiImage;
      char *cpt;
      int imgFile = 0;
      kgWC *wc;
      Display *Dsp;
      Window Root;
      Pixmap Pix;
      Atom BackgroundPixmapId;
      unsigned int Dpth = 32;
      int x0 = 0 , y0 = 0 , width , height , dy;
      Dsp = XOpenDisplay ( NULL ) ;
      if ( Dsp == NULL ) {
          setenv ( "DISPLAY" , ":0" , 1 ) ;
          Dsp = XOpenDisplay ( NULL ) ;
      }
      Root = RootWindow ( Dsp , DefaultScreen ( Dsp ) ) ;
      Dpth = DisplayPlanes ( Dsp , DefaultScreen ( Dsp ) ) ;
      EVGAX = DisplayWidth ( Dsp , DefaultScreen ( Dsp ) ) ;
      EVGAY = DisplayHeight ( Dsp , DefaultScreen ( Dsp ) ) ;
      width = EVGAX;
      height = EVGAY;
      h = height; w = width;
      Pix = XCreatePixmap ( Dsp , Root , ( short ) \
       ( EVGAX ) , ( short ) ( EVGAY ) , Dpth ) ;
      uiImage = XGetImage ( Dsp , Root , 0 , 0 , EVGAX , \
           EVGAY , 0xffffffff , ZPixmap ) ;
      ximage_rowbytes = uiImage->bytes_per_line;
      Imgdata = ( unsigned char * ) uiImage->data;
      Img = uiCreateCleangmImage ( EVGAX , EVGAY , 0 , 0 , 0 , 255 ) ;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( Img ) ;
      for ( i = 0; i < h; ++i ) {
          dest = pixels + i*w;
          src = Imgdata + i*ximage_rowbytes;
          for ( j = 0;j < w; j++ ) {
              dest->blue = *src;src++;
              dest->green = *src;src++;
              dest->red = *src;src++;
              dest->opacity = 255- ( *src ) ;src++;
              dest++;
          }
      }
      XDestroyImage ( uiImage ) ;
      XFreePixmap ( Dsp , Pix ) ;
      XCloseDisplay ( Dsp ) ;
      return Img;
  }
  int kgRestoreImage ( DIALOG *D , void *tmp , int x0 , \
       int y0 , int width , int height ) \
  {
      int i , j , k , m , row , color , sx , sy , dx , dy , xoffset , yoffset , \
           xm , ym , xl , yl , xu , yu , xc , yc , xdl , xdu , ydl , ydu;
      float rzfac;
//    unsigned long *dest,*Imgdata,val;
      unsigned long val;
      unsigned char *Imgdata = NULL , *dest;
      unsigned char r , g , b;
      unsigned int a;
      unsigned long lastrow = 0;
      unsigned long pixel;
      unsigned int red , green , blue;
      unsigned char bg_red = 0xff , bg_green = 0xff , bg_blue = 0xff;
      double f , f1;
      unsigned int ALPHA = 255;
      int EVGAX , EVGAY;
      char *fullname = NULL;
      PixelPacket *pixels , *src;
      GMIMG *png = NULL , *pngrz = NULL;
      Image *img;
      int ximage_rowbytes;
      int w , h , ww;
      XImage *uiImage;
      char *cpt;
      int imgFile = 0;
      kgWC *wc;
      if ( tmp == NULL ) return 0;
      wc = D->wc;
      EVGAX = WC ( D )->EVGAX;
      EVGAY = WC ( D )->EVGAY;
      cpt = ( char * ) tmp;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) {
          fullname = _uiGetFileImageName ( D , ( char * ) ( cpt+2 ) ) ;
          if ( fullname == NULL ) return 0;
          else {
              png = ( GMIMG * ) _uiGetFileImage ( fullname ) ;
              free ( fullname ) ;
              if ( png == NULL ) {
                  printf ( "Failed to get %s\n" , ( char * ) ( cpt+2 ) ) ;
                  return 0;
              }
          }
//         else printf("Got Image\n");
          imgFile = 1;
      }
      else png = ( GMIMG * ) tmp;
      if ( x0 < 0 ) x0 = 0;
      if ( y0 < 0 ) y0 = 0;
      if ( ( x0+width ) > EVGAX ) width = EVGAX-x0-1;
      if ( ( y0+height ) > EVGAY ) height = EVGAY-y0-1;
      if ( png == NULL ) return 0;
      rzfac = png->rzfac;
      if ( rzfac != 1.0 ) {
          pngrz = uiResizegmImage ( png , rzfac ) ;
          if ( imgFile ) uiFreeImage ( png ) ;
          if ( pngrz == NULL ) {
              return 0;
          }
          png = pngrz;
      }
      xoffset = png->xoffset;
      yoffset = png->yoffset;
      xc = x0+width/2;
      yc = y0+height/2;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( png ) ;
      img = png->image;
      w = img->columns;
      h = img->rows;
      xm = ( w-xoffset ) /2+xoffset;
      ym = ( h-yoffset ) /2+yoffset;
      xl = xm -width/2; xu = xm+width-width/2;
      yl = ym -height/2; yu = ym+height -height/2;
      if ( ( xu < 0 ) || ( xl > w ) || ( yu < 0 ) || ( yl > h ) ) {
          if ( pngrz != NULL ) uiFreeImage ( pngrz ) ;
          else if ( imgFile ) uiFreeImage ( png ) ;
          return 0;
      }
// There is something to display
      if ( xl < 0 ) xl = 0;
      if ( xu > w ) xu = w;
      if ( yl < 0 ) yl = 0;
      if ( yu > h ) yu = h;
      dx = xm - xc;
      dy = ym - yc;
      xdl = xl-dx;
      xdu = xu-dx;
      ydl = yl -dy;
      ydu = yu -dy;
      uiImage = kg_GetImage ( D , xdl , ydl , xdu-xdl+1 , ydu-ydl+1 ) ;
      if ( uiImage == NULL ) {
          if ( pngrz != NULL ) uiFreeImage ( pngrz ) ;
          else if ( imgFile ) uiFreeImage ( png ) ;
          return 0;
      }
      ximage_rowbytes = uiImage->bytes_per_line;
      Imgdata = ( unsigned char * ) uiImage->data;
      row = -1;
      for ( i = 0; i < h; ++i ) {
          if ( i < ( yl ) ) continue;
          if ( i >= yu ) break;
          src = pixels + i*w;
          row++;
          dest = Imgdata + row*ximage_rowbytes;
          m = 0;
          k = -1;
          ww = w;
          if ( png->image_channels == 3 ) {
              for ( j = 0;j < ww; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src++;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  red = src->red;
                  green = src->green;
                  blue = src->blue;
                  src++;
                  *dest = blue; *dest++;
                  *dest = green; *dest++;
                  *dest = red; *dest++;
                  *dest = 0xff; *dest++;
                  m++;
              }
              } else /* if ( image_channels == 4 ) */ {
              for ( j = 0;j < ww; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src++;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  r = src->red;
                  g = src->green;
                  b = src->blue;
                  a = 255 - src->opacity;
                  src++;
                  *dest = b; *dest++;
                  *dest = g; *dest++;
                  *dest = r; *dest++;
                  *dest = a; *dest++;
                  m++;
              }
          }
      }
      kgPutImage ( D , uiImage , 0 , 0 , xdl , ydl , xdu-xdl+1 , ydu-ydl+1 ) ;
      XDestroyImage ( uiImage ) ;
      if ( pngrz != NULL ) uiFreeImage ( pngrz ) ;
      else if ( imgFile ) uiFreeImage ( png ) ;
      return 1;
  }
  int kgRestoreImagePart ( DIALOG *D , void *tmp , int x0 , int y0 , \
       int xoffset , int yoffset , int width , int height ) \
  {
      int i , j , k , m , row , color , sx , sy , dx , dy , xm , ym , \
           xl , yl , xu , yu , xc , yc , xdl , xdu , ydl , ydu;
      float rzfac;
//    unsigned long *dest,*Imgdata,val;
      unsigned long val;
      unsigned char *Imgdata = NULL , *dest;
      unsigned char r , g , b;
      unsigned int a;
      unsigned long lastrow = 0;
      unsigned long pixel;
      unsigned int red , green , blue;
      unsigned char bg_red = 0xff , bg_green = 0xff , bg_blue = 0xff;
      double f , f1;
      unsigned int ALPHA = 255;
      int EVGAX , EVGAY;
      char *fullname = NULL;
      PixelPacket *pixels , *src;
      GMIMG *png = NULL , *pngrz = NULL;
      Image *img;
      int ximage_rowbytes;
      int w , h , ww;
      XImage *uiImage;
      char *cpt;
      int imgFile = 0;
      kgWC *wc;
      if ( tmp == NULL ) return 0;
      wc = D->wc;
      EVGAX = WC ( D )->EVGAX;
      EVGAY = WC ( D )->EVGAY;
      cpt = ( char * ) tmp;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) {
          fullname = _uiGetFileImageName ( D , ( char * ) ( cpt+2 ) ) ;
          if ( fullname == NULL ) return 0;
          else {
              png = ( GMIMG * ) _uiGetFileImage ( fullname ) ;
              free ( fullname ) ;
              if ( png == NULL ) {
                  printf ( "Failed to get %s\n" , ( char * ) ( cpt+2 ) ) ;
                  return 0;
              }
          }
//         else printf("Got Image\n");
          imgFile = 1;
      }
      else png = ( GMIMG * ) tmp;
      x0 = x0-xoffset;
      y0 = y0-yoffset;
      if ( x0 < 0 ) x0 = 0;
      if ( y0 < 0 ) y0 = 0;
      if ( ( x0+width ) > EVGAX ) width = EVGAX-x0-1;
      if ( ( y0+height ) > EVGAY ) height = EVGAY-y0-1;
      if ( png == NULL ) return 0;
      rzfac = png->rzfac;
      if ( rzfac != 1.0 ) {
          pngrz = uiResizegmImage ( png , rzfac ) ;
          if ( imgFile ) uiFreeImage ( png ) ;
          if ( pngrz == NULL ) {
              return 0;
          }
          png = pngrz;
      }
      xc = x0+xoffset;
      yc = y0+yoffset;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( png ) ;
      img = png->image;
      w = img->columns;
      h = img->rows;
      xm = xoffset;
      ym = yoffset;
      xl = xm ; xu = xm+width;
      yl = ym ; yu = ym+height ;
      if ( ( xu < 0 ) || ( xl > w ) || ( yu < 0 ) || ( yl > h ) ) {
          if ( pngrz != NULL ) uiFreeImage ( pngrz ) ;
          else if ( imgFile ) uiFreeImage ( png ) ;
          return 0;
      }
// There is something to display
      if ( xl < 0 ) xl = 0;
      if ( xu > w ) xu = w;
      if ( yl < 0 ) yl = 0;
      if ( yu > h ) yu = h;
      dx = ( xu -xl ) ;
      dy = ( yu-yl ) ;
      xdl = x0+xoffset;
      xdu = xdl+dx;
      ydl = y0+yoffset;
      ydu = ydl+dy;
      uiImage = kg_GetImage ( D , xdl , ydl , xdu-xdl+1 , ydu-ydl+1 ) ;
      if ( uiImage == NULL ) {
          if ( pngrz != NULL ) uiFreeImage ( pngrz ) ;
          else if ( imgFile ) uiFreeImage ( png ) ;
          return 0;
      }
      ximage_rowbytes = uiImage->bytes_per_line;
      Imgdata = ( unsigned char * ) uiImage->data;
      row = -1;
      for ( i = 0; i < h; ++i ) {
          if ( i < ( yl ) ) continue;
          if ( i >= yu ) break;
          src = pixels + i*w;
          row++;
          dest = Imgdata + row*ximage_rowbytes;
          m = 0;
          k = -1;
          ww = w;
          if ( png->image_channels == 3 ) {
              for ( j = 0;j < ww; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src++;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  red = src->red;
                  green = src->green;
                  blue = src->blue;
                  src++;
                  *dest = blue; *dest++;
                  *dest = green; *dest++;
                  *dest = red; *dest++;
                  *dest = 0xff; *dest++;
                  m++;
              }
              } else /* if ( image_channels == 4 ) */ {
              for ( j = 0;j < ww; j++ ) {
                  k++;
                  if ( ( k ) < ( xl ) ) {src++;continue;}
                  if ( ( k ) >= ( xu ) ) break;
                  r = src->red;
                  g = src->green;
                  b = src->blue;
                  a = 255 - src->opacity;
                  src++;
                  *dest = b; *dest++;
                  *dest = g; *dest++;
                  *dest = r; *dest++;
                  *dest = a; *dest++;
                  m++;
              }
          }
      }
      kgPutImage ( D , uiImage , 0 , 0 , xdl , ydl , xdu-xdl+1 , ydu-ydl+1 ) ;
      XDestroyImage ( uiImage ) ;
      if ( pngrz != NULL ) uiFreeImage ( pngrz ) ;
      else if ( imgFile ) uiFreeImage ( png ) ;
      return 1;
  }
/* Clip board routeines */
  unsigned char *_uiGetPrimary ( void * Tmp ) {
/***************************
  Caller should free the retrned pointer
 ****************************/
      unsigned char *ret = NULL , *buff = NULL;
      int len = 0;
      unsigned int context = 0;
      DIALOG *D;
      kgWC *wc;
      Window Owner;
      D = ( DIALOG * ) Tmp;
      static Atom prop = 0;
      Atom Inc;
      Atom prop_type;
      Atom Val;
      int prop_format , count = 0 , wret;
      unsigned long prop_size , prop_items;
      KBEVENT kbe;
      Atom sel = XA_PRIMARY; /* X selection to work with */
      Atom target = XA_STRING;
      wc = WC ( D ) ;
      if ( ! prop ) prop = XInternAtom ( wc->Dsp , "XPRI_OUT" , False ) ;
//   Inc = XInternAtom(wc->Dsp, "INCR", False);
//   ret = ( unsigned char *) XFetchBuffer(wc->Dsp, (int *) &len, 0);
//   kgGetEvent(D);
      Owner = XGetSelectionOwner ( wc->Dsp , sel ) ;
//   printf("Owner : %d %d %d\n",Owner,wc->Win,wc->Root);
      if ( XConvertSelection ( wc->Dsp , sel , target , \
           prop , wc->Win , CurrentTime ) == 0 ) \
      {
//     XDeleteProperty(wc->Dsp, wc->Win, prop);
          return NULL;
      }
//   if(Owner== wc->Win)  kgWaitSelectionNotify(Tmp);
      wret = kgWaitSelection ( Tmp ) ;
      if ( wret == -1 ) return 0;
      while ( wret == 2 ) {
          wret = kgWaitSelection ( Tmp ) ;
          if ( wret == -1 ) return 0;
      }
      Val = sel;
      if ( ( Owner != None ) ) {
          Val = prop;
      }
      XGetWindowProperty ( wc->Dsp , wc->Win , Val , 0 , 0 , False , AnyPropertyType , \
           & prop_type , & prop_format , & prop_items , & prop_size , & buff ) ;
      XFree ( buff ) ;
      buff = NULL;
      len = prop_size;
//   printf("LEN= %d\n",prop_size);
      XFlush ( wc->Dsp ) ;
      count = 0;
      while ( len == 0 ) {
          count++;
          XGetWindowProperty ( wc->Dsp , wc->Win , Val , 0 , 0 , False , AnyPropertyType , \
               & prop_type , & prop_format , & prop_items , & prop_size , & buff ) ;
          XFree ( buff ) ;
          buff = NULL;
          len = prop_size;
//     printf("LEN= %d\n",prop_size);
          if ( len ) break;
          if ( count > 3 ) return NULL;
          usleep ( 150 ) ;
      }
      len = prop_size;
      if ( prop_format == 8 ) { /* hell wit other formats */
          XGetWindowProperty ( wc->Dsp , wc->Win , Val , 0 , ( long ) prop_size , \
               False , AnyPropertyType , & prop_type , & prop_format , \
               & prop_items , & prop_size , & buff ) ;
      }
//   XDeleteProperty(wc->Dsp, wc->Win, prop);
      XFlush ( wc->Dsp ) ;
      if ( buff != NULL ) {
          ret = ( unsigned char * ) Malloc ( len+1 ) ;
          memcpy ( ret , buff , len ) ;
//     printf("len= %d\n",len);
          ret [ len ] = '\0';
          XFree ( buff ) ;
      }
      return ret;
  }
  unsigned char *kgGetPrimary ( void * Tmp ) {
      unsigned char *ptr = NULL;
      ptr = _uiGetPrimary ( Tmp ) ;
      if ( ptr != NULL ) free ( ptr ) ;
      return _uiGetPrimary ( Tmp ) ;
  }
  unsigned char *_uiGetClipBoard ( void * Tmp ) {
/***************************
  Caller should free the retrned pointer
 ****************************/
      unsigned char *ret = NULL , *buff = NULL;
      int len = 0;
      unsigned int context = 0;
      DIALOG *D;
      kgWC *wc;
      Window Owner;
      D = ( DIALOG * ) Tmp;
      static Atom prop = 0;
      Atom Inc;
      Atom prop_type;
      Atom Val;
      int prop_format , count = 0 , wret;
      unsigned long prop_size , prop_items;
      KBEVENT kbe;
      wc = WC ( D ) ;
//   Atom sel = XA_PRIMARY;	/* X selection to work with */
      Atom sel = XInternAtom ( wc->Dsp , "CLIPBOARD" , 0 ) ;
      Atom target = XA_STRING;
      if ( ! prop ) prop = XInternAtom ( wc->Dsp , "XCLIP_OUT" , False ) ;
      Owner = XGetSelectionOwner ( wc->Dsp , sel ) ;
//   printf("Owner : %d %d %d\n",Owner,wc->Win,wc->Root);
      if ( XConvertSelection ( wc->Dsp , sel , target , \
           prop , wc->Win , CurrentTime ) == 0 ) \
      {
//     XDeleteProperty(wc->Dsp, wc->Win, prop);
          return NULL;
      }
      wret = kgWaitSelection ( Tmp ) ;
      if ( wret == -1 ) return 0;
      while ( wret == 2 ) {
          wret = kgWaitSelection ( Tmp ) ;
          if ( wret == -1 ) return 0;
      }
      Val = sel;
      if ( ( Owner != None ) ) {
          Val = prop;
      }
      XFlush ( wc->Dsp ) ;
      XGetWindowProperty ( wc->Dsp , wc->Win , Val , 0 , 0 , False , AnyPropertyType , \
           & prop_type , & prop_format , & prop_items , & prop_size , & buff ) ;
      XFree ( buff ) ;
      buff = NULL;
      len = prop_size;
//   printf("LEN= %d\n",prop_size);
      while ( len == 0 ) {
          count++;
          XGetWindowProperty ( wc->Dsp , wc->Win , Val , 0 , 0 , False , AnyPropertyType , \
               & prop_type , & prop_format , & prop_items , & prop_size , & buff ) ;
          XFree ( buff ) ;
          buff = NULL;
          len = prop_size;
//     printf("LEN= %d\n",prop_size);
          if ( len ) break;
          if ( count > 3 ) return NULL;
          usleep ( 150 ) ;
      }
      len = prop_size;
      if ( prop_format == 8 ) { /* hell wit other formats */
          XGetWindowProperty ( wc->Dsp , wc->Win , Val , 0 , ( long ) prop_size , \
               False , AnyPropertyType , & prop_type , & prop_format , \
               & prop_items , & prop_size , & buff ) ;
      }
//   XDeleteProperty(wc->Dsp, wc->Win, prop);
      XFlush ( wc->Dsp ) ;
      if ( buff != NULL ) {
          ret = ( unsigned char * ) Malloc ( len+1 ) ;
          memcpy ( ret , buff , len ) ;
//     printf("len= %d\n",len);
          ret [ len ] = '\0';
          XFree ( buff ) ;
      }
      return ret;
  }
  unsigned char *kgGetClipBoard ( void * Tmp ) {
      unsigned char *ptr = NULL;
      ptr = _uiGetClipBoard ( Tmp ) ;
      if ( ptr != NULL ) free ( ptr ) ;
      return _uiGetClipBoard ( Tmp ) ;
  }
  int kgSetPrimary ( void * Tmp , unsigned char *data ) {
      DIALOG *D;
      kgWC *wc;
      static Atom prop = 0;
      Window w;
      char *res;
      D = ( DIALOG * ) Tmp;
      wc = WC ( D ) ;
      int ret = 1 , len;
#if 0
      if ( wc->Rth == 0 ) return 0;
#endif
      Atom sel = XA_PRIMARY;
      Atom target = XA_STRING;
      if ( ! prop ) prop = XInternAtom ( wc->Dsp , "XPRI_IN" , False ) ;
#if 1
//      XSetSelectionOwner ( wc->Dsp , sel , wc->Win , CurrentTime ) ;
      XSetSelectionOwner ( wc->Dsp , sel , None , CurrentTime ) ;
//   w = XGetSelectionOwner(wc->Dsp,sel);
      w = wc->Win;
      pthread_mutex_lock ( & ( WC ( D )->Rlock ) ) ;
      XChangeProperty ( wc->Dsp , w , sel , target , \
           8 , PropModeReplace , data , strlen \
       ( data ) +1 ) ;
      XChangeProperty ( wc->Dsp , w , prop , target , \
           8 , PropModeReplace , data , strlen \
       ( data ) +1 ) ;
      pthread_mutex_unlock ( & ( WC ( D )->Rlock ) ) ;
#endif
#if 0
      printf ( "Return:%s : %s\n" , data , kgGetPrimary ( Tmp ) ) ;
      res = kgGetPrimary ( Tmp ) ;
      printf ( "Return:%s : %s\n" , data , res ) ;
      if ( ! res ) free ( res ) ;
#endif
      return ret;
  }
  int kgSetClipBoard ( void * Tmp , unsigned char *data ) {
      DIALOG *D;
      kgWC *wc;
      static Atom prop = 0;
      Window w;
      char *res;
      D = ( DIALOG * ) Tmp;
      wc = WC ( D ) ;
      int ret = 1 , len;
#if 0
      if ( wc->Rth == 0 ) return 0;
#endif
      Atom sel = XInternAtom ( wc->Dsp , "CLIPBOARD" , 0 ) ;
      Atom target = XA_STRING;
      if ( ! prop ) prop = XInternAtom ( wc->Dsp , "XCLIP_IN" , False ) ;
#if 1
//      XSetSelectionOwner ( wc->Dsp , sel , wc->Win , CurrentTime ) ;
      XSetSelectionOwner ( wc->Dsp , sel , None , CurrentTime ) ;
      w = wc->Win;
      pthread_mutex_lock ( & ( WC ( D )->Rlock ) ) ;
      XChangeProperty ( wc->Dsp , w , sel , target , \
           8 , PropModeReplace , data , strlen \
       ( data ) +1 ) ;
      XChangeProperty ( wc->Dsp , w , prop , target , \
           8 , PropModeReplace , data , strlen \
       ( data ) +1 ) ;
      pthread_mutex_unlock ( & ( WC ( D )->Rlock ) ) ;
#endif
//   printf("Return:%s : %s\n",data,kgGetPrimary(Tmp));
//   res = kgGetClipBoard(Tmp);
//   printf("Return:%s : %s\n",data,res);
//   if(!res) free(res);
      return ret;
  }
  int kgEnableSelection ( void *Tmp ) {
//    pthread_t Pth;
      DIALOG *D = ( DIALOG * ) Tmp;
      kgWC *wc;
      wc = WC ( D ) ;
      if ( wc == NULL ) return 0;
      if ( wc->Rth != 0 ) return 0;
      pthread_create ( & ( wc->Rth ) , NULL , kgProcessSelectionRequest , Tmp ) ;
      return 1;
  }
  int kgDisableSelection ( void *junk ) {
//    pthread_t Pth;
      DIALOG *D = ( DIALOG * ) junk;
      kgWC *wc;
      wc = WC ( D ) ;
      if ( wc == NULL ) return 0;
      if ( wc->Rth != 0 ) {
          pthread_cancel ( WC ( D )->Rth ) ;
          pthread_join ( WC ( D )->Rth , NULL ) ;
      }
      wc->Rth = 0;
      return 1;
  }
