#define D_TH
#define D_NEWTBL
/*
  Version 2.1
  Dated 22nd Oct 1998 Time 12:15
  Further Modified on 16th Oct 1999
  Ver 8.0
  Dated 31 July 2012
  Total rewrite of gui interface
*/
#define D_RESTORE
#define LINUX
#define ZBUFFER
#define MINV 20
#define GAP 0.05
#define HFAC 1.30
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#ifdef RS6000
#include<aio.h>
#endif
#include "kulina.h"
#include "gprivate.h"
#include "gminterface.h"
#include "dlink.h"
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
#define  RESIZE 5
#define  TRESIZE 3
/*
#include "font37.h"
#include "font38.h"
#include "font39.h"
#include "font40.h"
#include "font41.h"
#include "font42.h"
#include "font43.h"
#include "font44.h"
#include "font45.h"
#include "font46.h"
#include "font47.h"
#include "font48.h"
#include "font49.h"
#include "font50.h"
#include "font51.h"
#include "font52.h"
*/
  static Dlink *MonoList = NULL;
  static Dlink *FontList = NULL;
#define D_NOCLEANCC
 static char *MonoFonts [ ] = {
  "Anonymous.ttf" ,
  "Anonymous_Pro.ttf" ,
  "Anonymous_Pro_I.ttf" ,
  "Anonymous_Pro_B.ttf" ,
  "Anonymous_Pro_BI.ttf" ,
  "Cousine-Regular.ttf" ,
  "Cousine-Italic.ttf" ,
  "Cousine-Bold.ttf" ,
  "Cousine-BoldItalic.ttf" ,
  "DroidSansMono.ttf" ,
  "FreeMono.ttf",
  "FreeMonoOblique.ttf",
  "FreeMonoBold.ttf",
  "FreeMonoBoldOblique.ttf",
  "UbuntuMono-R.ttf",
  "UbuntuMono-RI.ttf",
  "VeraMono.ttf",
  "VeraMono-Italic.ttf",
  "VeraMono-Bold.ttf",
  "VeraMono-Bold-Italic.ttf",
   NULL
}; 
static char *OthFonts []= {
"CharterRegular.ttf",
"CharterItalic.ttf",
"CharterBold.ttf",
"CharterBoldItalic.ttf",
"Cousine-Regular.ttf",
"Cousine-Italic.ttf",
"Cousine-Bold.ttf",
"Cousine-BoldItalic.ttf",
"NimbusRomNo9L-Reg.otf",
"NimbusRomNo9L-RegIta.otf",
"NimbusSanL-Reg.otf",
"NimbusSanL-BolIta.otf",
"Ubuntu-R.ttf",
"Ubuntu-RI.ttf",
"ZapfChanceryMediumItalic.otf",
"s050000l.pfb"
"Symbols.ttf",
"geek.ttf",
"Madina.ttf",
"Wasichu.ttf",
"stewartsans.ttf",
"LindenHill-Regular.ttf",
"LindenHill-Italic.ttf",
"Ubuntu-L.ttf",
"Ubuntu-C.ttf",
"Ubuntu-M.tt",
"FreeSerif.ttf",
"MathSymbolsNormal.ttf",
"LucidaMathSymbolRegular.otf",
  NULL
};
#define D_CLEANCC      
  static IMG_STR **Imgs = NULL , **Bimgs , **Mimgs , **Nimgs , **Pimgs;
/*#include "fontps.h"*/
#define far 
#define SSF 0.6
  int GetPointer ( int *x , int *y ) ;
  const int Mag = 3;
  const float CFact = 300000.;
  typedef struct {
      char *fontname;
      int *icpos;
      int *icxv;
      int *icyv;
      float *m_f;
  } FONT;
  static int Sldwd = 20 , Sw = 8 , Sdpw = 7;
  int TextSize = 8 , Ht = 12 , Wd = 7 , Gap = 2 , Bt = 1;
#define CHECKLIMITS(w) {\
   DIALOG *D; \
       int x1 , y1 , x2 , y2; \
       D = ( DIALOG * ) ( ( w )->D ) ; \
       if ( w->x1 < 0 ) return 0; \
       if ( w->y1 < 0 ) return 0; \
       if ( w->x2 > D->xl ) return 0; \
       if ( w->y2 > D->yl ) return 0; \
   }
#define BACKUPWIDGETAREA(w) {\
   DIALOG *D; \
       int x1 , y1 , x2 , y2; \
       D = ( DIALOG * ) ( ( w )->D ) ; \
       x1 = D->xo+ ( w )->x1; \
       y1 = D->yo+ ( w )->y1; \
       x2 = D->xo+ ( w )->x2; \
       y2 = D->yo+ ( w )->y2; \
       if ( ( w )->Bimg == NULL ) ( w )->Bimg = kgGetBackground  \
           ( D , x1 , y1 , x2 , y2 ) ; \
   }
#define RESTOREWIDGETAREA(w) {\
   DIALOG *D; \
       int x1 , y1 , xl , yl; \
       D = ( DIALOG * ) ( ( w )->D ) ; \
       x1 = D->xo+ ( w )->x1; \
       y1 = D->yo+ ( w )->y1; \
       xl = ( w )->x2- ( w )->x1+1; \
       yl = ( w )->y2- ( w )->y1+1; \
       if ( ( w )->Bimg != NULL ) kgRestoreImage ( D , \
            ( w )->Bimg , x1 , y1 , xl , yl ) ; \
   }
#define RESTOREWIDGETAREAPART(w,xp1,yp1,xp2,yp2) {\
   DIALOG *D; \
       int x1 , y1 , xl , yl , xoff , yoff; \
       D = ( DIALOG * ) ( ( w )->D ) ; \
       x1 = D->xo+ ( w )->x1; \
       y1 = D->yo+ ( w )->y1; \
       xoff = ( xp1 ) -x1; \
       yoff = ( yp1 ) -y1; \
       xl = ( xp2 ) - ( xp1 ) +1; \
       yl = ( yp2 ) - ( yp1 ) +1; \
       if ( ( w )->Bimg != NULL ) kgRestoreImagePart ( D , ( w )->Bimg , \
            xp1 , yp1 , xoff , yoff , xl , yl ) ; \
   }
#define set_greek {\
   if ( greek > 0 ) {\
           icpostmp = icpos; \
           icxvtmp = icxv; \
           icyvtmp = icyv; \
           icpos = icposf0; \
           icxv = icxvf0; \
           icyv = icyvf0; \
       }\
   }
#define uireset_greek {\
   if ( dc->greek > 0 ) {\
           dc->icpos = dc->icpostmp; \
           dc->icxv = dc->icxvtmp; \
           dc->icyv = dc->icyvtmp; \
       }\
   }
#define uiset_greek {\
   if ( dc->greek > 0 ) {\
           dc->icpostmp = dc->icpos; \
           dc->icxvtmp = dc->icxv; \
           dc->icyvtmp = dc->icyv; \
           dc->icpos = dc->icposf0; \
           dc->icxv = dc->icxvf0; \
           dc->icyv = dc->icyvf0; \
       }\
   }
#define reset_greek {\
   if ( greek > 0 ) {\
           icpos = icpostmp; \
           icxv = icxvtmp; \
           icyv = icyvtmp; \
       }\
   }
  extern int NCLRS;
#define uiTX(x,y) (dc->cx +(x)*dc->cost+(y)*dc->sint)
#define uiTY(x,y) (dc->cy -(x)*dc->sint+(y)*dc->cost)
#define _uiLINE(wc,x1,y1,x2,y2) uiCliped_Line(wc,x1,y1,x2,y2)
  static unsigned int ln_ptn [ 10 ] = {0xffffffff , 0x88888888 , 0xcccccccc , \
       0xf0f0f0f0 , 0xff18ff18 , 0xffff0f0f , 0xe7e7e7e7 , \
       0xfafafafa , 0xffff0000 , 0xffffff00};
  static short st_ptr [ 10 ] = {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
  typedef struct c_limit{ int cx1; int cy1; int cx2; int cy2; int * pt; } CLIP;
#define uiset_clip_limits(wc,x1,y1,x2,y2) wc->c_v_x1=x1,wc->c_v_y1=y1,wc->c_v_x2=x2,wc->c_v_y2=y2
#define uibkup_clip_limits wc->c_v_x1_o=wc->c_v_x1,wc->c_v_y1_o=wc->c_v_y1,wc->c_v_x2_o=wc->c_v_x2,wc->c_v_y2_o=wc->c_v_y2
#define uiset_sup_clip_limits(wc,x1,y1,x2,y2) wc->s_v_x1=x1,wc->s_v_y1=y1,wc->s_v_x2=x2,wc->s_v_y2=y2
#define uirest_clip_limits wc->c_v_x1=wc->c_v_x1_o,wc->c_v_y1=wc->c_v_y1_o,wc->c_v_x2=wc->c_v_x2_o,wc->c_v_y2=wc->c_v_y2_o
#define uiset_full_scrn  wc->c_v_x1=0,wc->c_v_y1=0,wc->c_v_x2=(wc->EVGAX),wc->c_v_y2=(wc->EVGAY)
  static unsigned char esc = 0x1b , G = 'g' , N = 'n' , \
       C = 'c' , P = 'p' , Z = 'z' , L = 'l' , V = 'v';
  static unsigned char S = 's' , T = 't' , R = 'r' , \
       M = 'm' , D = 'd' , Y = 'y' , U = 'u';
  static unsigned char O = 'o' , B = 'b' , X = 'x' , F = 'f' , \
       I = 'i' , A = 'a' , E = 'e' , W = 'w';
  static unsigned char H = 'h' , K = 'k' , M3 = 'M' , \
       D3 = 'D' , P3 = 'P' , G3 = 'G' , B3 = 'B';
  static unsigned char BELL = 0x07;
  static unsigned char Ch1 = 0;
#define uipclr(p)  (float)(((p-dc->pmin)*dc->dfac+1.))
#define mask 0xCF
#define rad 0.0174532
#define LN_STYL 0
#define M_STYL 0
#define uiscr_x(x) (int)((x-dc->w_x1)*dc->u_x+dc->D_x+dc->v_x1+0.5)
#define uiscr_y(y) (int)(((y-dc->w_y1)*dc->u_y+dc->D_y+dc->v_y1)+0.5)
#define uiusr_x(x) (float)((x-dc->D_x-dc->v_x1)/dc->u_x+dc->w_x1)
#define uiusr_y(y) (float)((y-dc->D_y-dc->v_y1)/dc->u_y+dc->w_y1)
  static unsigned char MASK [ 8 ] = { 0x80 , 0x40 , \
       0x20 , 0x10 , 0x08 , 0x04 , 0x02 , 0x01 };
  static unsigned char MASKB [ 8 ] = {0xFF , 0x7F , \
       0x3F , 0x1F , 0x0F , 0x07 , 0x03 , 0x01 };
  static unsigned char MASKE [ 8 ] = {0x80 , 0xC0 , \
       0xE0 , 0xF0 , 0xF8 , 0xFC , 0xFE , 0xFF };
  static unsigned char RMASK [ 4 ] = { 0x00 , 0x01 , 0x10 , 0x11 };
  static unsigned char MMASK [ 4 ] = { 0x01 , 0x02 , 0x04 , 0x08 };
  static unsigned char RED [ 4 ] = {0x00 , 0x04 , 0x20 , 0x24 };
  static unsigned char BLUE [ 4 ] = {0x00 , 0x01 , 0x08 , 0x09 };
  static unsigned char GREEN [ 4 ] = {0x00 , 0x02 , 0x10 , 0x12 };
  static char Posfmt [ 200 ] = {"%14.5g , %-14.5g"} , Xfmt [ 7 ] = \
  {" 14.5g"} , Yfmt [ 7 ] = {"-14.5g"};
  static union kbinp { short kbint; char kbc [ 2 ] ; } kb;
  void *kgBorderedRectangle ( int width , int height , \
       int fillclr , float rfac ) ;
  void *kgPressedRectangle ( int width , int height , \
       int fillclr , float rfac ) ;
  void uiMessageString ( DIALOG *D , char *str , int x , int y , int width , \
       int height , int font , int color , int FontSize , \
       int justfic , int bkcolor ) ;
  void *uiMakeTextBoxImage ( void *Tmp ) ;
//void _uiMakeButnImages(DIN *B,int butno);
  void *_uiMakeButnImages ( void *butstr ) ;
  void _uiInitButs ( DIN *B ) ;
#define EIGHT 8
  char *uiWhichFont (char *FontBase, char *pgr ) {
      int i = 0 , j , End = 0 , k;
      char *pt , **m , *res = NULL , *cpt;
      char path [ 5000 ] ;
      char buf [ 200 ] ;
      char **folders;
      Dlink *L;
      pt = FontBase ;
      strcpy ( path , FontBase ) ;
      folders = ( char ** ) kgFolderTree ( FontBase) ;
      if ( folders == NULL ) return NULL;
      k = 0;
      while ( ( pt = ( char * ) folders [ k++ ] ) != NULL ) {
	  strcpy (buf,pt);
//          printf ( "%s: %s\n" , pt , buf ) ;
          m = kgFileMenu ( buf , ( char * ) "*.ttf" ) ;
          if ( m != NULL ) {
              i = 0;
              while ( m [ i ] != NULL ) {
                  if ( strstr ( m [ i ] , pgr ) != NULL ) {
                      End = 1;
                      break;
                  }
                  i++;
              }
          }
          if ( End == 0 ) {
              j = 0;
              if ( m != NULL ) {
                  while ( m [ j ] != NULL ) {free ( m [ j ] ) ;j++;}
                  free ( m ) ;
              }
              m = kgFileMenu ( buf , ( char * ) "*.t1" ) ;
              if ( m != NULL ) {
                  i = 0;
                  while ( m [ i ] != NULL ) {
                      if ( strstr ( m [ i ] , pgr ) != NULL ) {
                          End = 1;
                          break;
                      }
                      i++;
                  }
              }
          }
          if ( End == 0 ) {
              j = 0;
              if ( m != NULL ) {
                  while ( m [ j ] != NULL ) {free ( m [ j ] ) ;j++;}
                  free ( m ) ;
              }
              m = kgFileMenu ( buf , ( char * ) "*.otf" ) ;
              if ( m != NULL ) {
                  i = 0;
                  while ( m [ i ] != NULL ) {
                      if ( strstr ( m [ i ] , pgr ) != NULL ) {
                          End = 1;
                          break;
                      }
                      i++;
                  }
              }
          }
          if ( End == 0 ) {
              j = 0;
              if ( m != NULL ) {
                  while ( m [ j ] != NULL ) {free ( m [ j ] ) ;j++;}
                  free ( m ) ;
              }
              m = kgFileMenu ( buf , ( char * ) "*.pfb" ) ;
              if ( m != NULL ) {
                  i = 0;
                  while ( m [ i ] != NULL ) {
                      if ( strstr ( m [ i ] , pgr ) != NULL ) {
                          End = 1;
                          break;
                      }
                      i++;
                  }
              }
          }
          if ( End == 0 ) {
              j = 0;
              if ( m != NULL ) {
                  while ( m [ j ] != NULL ) {free ( m [ j ] ) ;j++;}
                  free ( m ) ;
              }
              continue;
          }
          res = ( char * ) Malloc ( strlen ( path ) +1+strlen \
           ( pt ) +1+strlen ( m [ i ] ) +1 ) ;
	  strcpy( res,buf);
	  strcat(res,"/");
          strcat ( res , m [ i ] ) ;
          j = 0;
          while ( m [ j ] != NULL ) {free ( m [ j ] ) ;j++;}
          free ( m ) ;
          break;
      }
      kgFreeDouble ( ( void ** ) folders ) ;
//      printf ( "res: %s\n" , res ) ;
      return res;
  }
  char *kgWhichFont ( char *pgr ) {
/* Caller must free result if it is not NULL */
      int i = 0 , j , End = 0 , k;
      char *pt , **m , *res = NULL , *cpt;
      char path [ 5000 ] ;
      char buf [ 200 ] ;
      char FontBase [500 ] ;
      char **folders;
      strcpy(FontBase,(char *)"/usr/share/fonts");
      Dlink *L;
      if ( pgr [ 0 ] == '/' ) { // full path is given
          int l;
          char **m , *pt1;
          strcpy ( path , pgr ) ;
          pt = path;
          l = strlen ( pt ) ;
          while ( pt [ l ] != '/' ) l--;
          if ( l == 0 ) {
              m = kgFileMenu ( "/" , pt+1 ) ;
          }
          else {
              pt1 = pt+l+1;
              pt [ l ] = '\0';
              m = kgFileMenu ( pt , pt1 ) ;
          }
          if ( ( m == NULL ) ) {return NULL;}
          if ( ( m [ 0 ] == NULL ) ) {kgFreeDouble ( ( void ** ) m ) ;return NULL;}
          kgFreeDouble ( ( void ** ) m ) ;
          res = ( char * ) Malloc ( strlen ( pgr ) +1 ) ;
          strcpy ( res , pgr ) ;
          return res;
      }
      if(res == NULL) {
         folders = ( char ** ) kgFolderMenu (FontBase  ) ;
         if(folders==NULL) return NULL;
         while ( ( pt = ( char * ) folders [ k++ ] ) != NULL ) {
             strcpy ( path , FontBase ) ;
             strcat (path, (char *)"/");
             strcat (path,pt);
             res = uiWhichFont(path,pgr);
             if(res != NULL) break;
         }
      }
#if 0
      folders = ( char ** ) kgFolderTree ( FontBase ) ;
      if ( folders == NULL ) return NULL;
      k = 0;
      while ( ( pt = ( char * ) folders [ k++ ] ) != NULL ) {
	  strcpy (buf,pt);
//          printf ( "%s: %s\n" , pt , buf ) ;
          m = kgFileMenu ( buf , ( char * ) "*.ttf" ) ;
          if ( m != NULL ) {
              i = 0;
              while ( m [ i ] != NULL ) {
                  if ( strstr ( m [ i ] , pgr ) != NULL ) {
                      End = 1;
                      break;
                  }
                  i++;
              }
          }
          if ( End == 0 ) {
              j = 0;
              if ( m != NULL ) {
                  while ( m [ j ] != NULL ) {free ( m [ j ] ) ;j++;}
                  free ( m ) ;
              }
              m = kgFileMenu ( buf , ( char * ) "*.t1" ) ;
              if ( m != NULL ) {
                  i = 0;
                  while ( m [ i ] != NULL ) {
                      if ( strstr ( m [ i ] , pgr ) != NULL ) {
                          End = 1;
                          break;
                      }
                      i++;
                  }
              }
          }
          if ( End == 0 ) {
              j = 0;
              if ( m != NULL ) {
                  while ( m [ j ] != NULL ) {free ( m [ j ] ) ;j++;}
                  free ( m ) ;
              }
              m = kgFileMenu ( buf , ( char * ) "*.otf" ) ;
              if ( m != NULL ) {
                  i = 0;
                  while ( m [ i ] != NULL ) {
                      if ( strstr ( m [ i ] , pgr ) != NULL ) {
                          End = 1;
                          break;
                      }
                      i++;
                  }
              }
          }
          if ( End == 0 ) {
              j = 0;
              if ( m != NULL ) {
                  while ( m [ j ] != NULL ) {free ( m [ j ] ) ;j++;}
                  free ( m ) ;
              }
              m = kgFileMenu ( buf , ( char * ) "*.pfb" ) ;
              if ( m != NULL ) {
                  i = 0;
                  while ( m [ i ] != NULL ) {
                      if ( strstr ( m [ i ] , pgr ) != NULL ) {
                          End = 1;
                          break;
                      }
                      i++;
                  }
              }
          }
          if ( End == 0 ) {
              j = 0;
              if ( m != NULL ) {
                  while ( m [ j ] != NULL ) {free ( m [ j ] ) ;j++;}
                  free ( m ) ;
              }
              continue;
          }
          res = ( char * ) Malloc ( strlen ( path ) +1+strlen \
           ( pt ) +1+strlen ( m [ i ] ) +1 ) ;
	  strcpy( res,buf);
	  strcat(res,"/");
          strcat ( res , m [ i ] ) ;
          j = 0;
          while ( m [ j ] != NULL ) {free ( m [ j ] ) ;j++;}
          free ( m ) ;
          break;
      }
#endif
      kgFreeDouble ( ( void ** ) folders ) ;
//      printf ( "res: %s\n" , res ) ;
      return res;
  }
  int kgCheckFont ( char *Font ) {
      int ret = -1 , i = 0;
      char BaseName [ 300 ] ;
      char *Fn = NULL;
      kgExtractBaseName ( Font , BaseName ) ;
      if ( FontList == NULL ) return -1; ;
      Resetlink ( FontList ) ;
      while ( ( Fn = ( char * ) Getrecord ( FontList ) ) != NULL ) {
          if ( strstr ( Fn , BaseName ) != NULL ) return i;
          i++;
      }
      return -1;
  }
  int kgAddFont ( char *Font ) {
      int ret = -1;
      char *Fn = NULL;
      if ( FontList == NULL ) FontList = Dopen ( ) ;
      if ( ( Fn = ( char * ) kgWhichFont ( Font ) ) != NULL ) {
          Dappend ( FontList , Fn ) ;
          ret = Dcount ( FontList ) -1;
      }
      else {
          if ( ( Fn = ( char * ) kgWhichFont ( ( char * ) "DejaVuSerif.ttf" ) ) != NULL )  \
              {
              Dappend ( FontList , Fn ) ;
              ret = Dcount ( FontList ) -1;
          }
          else {
              if ( ( Fn = ( char * ) kgWhichFont ( ( char * ) "Serif" ) ) != NULL ) {
                  Dappend ( FontList , Fn ) ;
                  ret = Dcount ( FontList ) -1;
              }
          }
      }
      return ret;
  }
  int uiAddFonts ( ) {
      int i = 0;
      char *Fn = NULL;
      while ( MonoFonts [ i ] != NULL ) {
          kgAddFont ( MonoFonts [ i ] ) ;
          i++;
      }
      i = 0;
      while ( OthFonts [ i ] != NULL ) {
          kgAddFont ( OthFonts [ i ] ) ;
          i++;
      }
      return 1;
  }
  int kgAddFixedFont ( char *Font ) {
      int ret = -1;
      char *Fn = NULL;
      if ( MonoList == NULL ) MonoList = Dopen ( ) ;
      if ( ( Fn = ( char * ) kgWhichFont ( Font ) ) != NULL ) {
          Dappend ( MonoList , Fn ) ;
          ret = Dcount ( MonoList ) -1;
      }
      else {
          if ( ( Fn = ( char * ) kgWhichFont ( ( char * ) "DejaVuSansMono.ttf" ) ) != NULL )  \
              {
              Dappend ( MonoList , Fn ) ;
              ret = Dcount ( MonoList ) -1;
          }
          else {
              if ( ( Fn = ( char * ) kgWhichFont ( ( char * ) "Mono" ) ) != NULL ) {
                  Dappend ( MonoList , Fn ) ;
                  ret = Dcount ( MonoList ) -1;
              }
          }
      }
      return ret;
  }
  char *kgGetMonoFont ( int Font ) {
      char *Ft = NULL , *pt;
      int count;
      if ( MonoList == NULL ) return NULL;
      count = Dcount ( MonoList ) ;
      if ( count == 0 ) return NULL;
//      printf("count : %d\n",count);
      Resetlink ( MonoList ) ;
      if ( count == 1 ) {
          pt = ( char * ) Drecord ( MonoList , 0 ) ;
      }
      else {
          Font = Font%count;
          pt = ( char * ) Drecord ( MonoList , Font ) ;
      }
      Ft = ( char * ) malloc ( strlen ( pt ) +1 ) ;
      strcpy ( Ft , pt ) ;
      return Ft;
  }
  char *kgGetOthFont ( int Font ) {
      char *Ft = NULL , *pt;
      int count;
      if ( FontList == NULL ) return NULL;
      count = Dcount ( FontList ) ;
      Resetlink ( FontList ) ;
      if ( count == 0 ) return NULL;
      if ( count == 1 ) {
          pt = ( char * ) Drecord ( FontList , 0 ) ;
      }
      else {
          Font = Font%count;
          pt = ( char * ) Drecord ( FontList , Font ) ;
      }
      Ft = ( char * ) malloc ( strlen ( pt ) +1 ) ;
      strcpy ( Ft , pt ) ;
      return Ft;
  }
  int uiAddFixedFonts ( ) {
      int i = 0;
      char *Fn = NULL;
      while ( MonoFonts [ i ] != NULL ) {
          kgAddFixedFont ( MonoFonts [ i ] ) ;
          i++;
      }
  }
  char **kgGetFontList ( void ) {
      char **Fonts , *pt , *spt;
      int count , i;
      char Buff [ 500 ] ;
      if ( FontList == NULL ){
//        return NULL;
          uiAddFonts();
      }
      count = Dcount ( FontList ) ;
      Fonts = ( char ** ) malloc ( sizeof ( char * ) * ( count+1 ) ) ;
      Fonts [ count ] = NULL;
      i = 0;
      Resetlink ( FontList ) ;
      while ( ( pt = ( char * ) Getrecord ( FontList ) ) != NULL ) {
          sprintf ( Buff , "!f%-2.2d %4d %s" , i , i , pt ) ;
          spt = ( char * ) malloc ( strlen ( Buff ) +1 ) ;
          strcpy ( spt , Buff ) ;
          Fonts [ i ] = spt;
          i++;
      }
      return Fonts;
  }
  char **kgGetMonoList ( void ) {
      char **Fonts , *pt , *spt;
      int count , i;
      char Buff [ 500 ] ;
      if ( MonoList == NULL ) return NULL;
      count = Dcount ( MonoList ) ;
      Fonts = ( char ** ) malloc ( sizeof ( char * ) * ( count+1 ) ) ;
      Fonts [ count ] = NULL;
      i = 0;
      Resetlink ( MonoList ) ;
      while ( ( pt = ( char * ) Getrecord ( MonoList ) ) != NULL ) {
          sprintf ( Buff , "%4d !f%-2.2d%s" , i , i , pt ) ;
          spt = ( char * ) malloc ( strlen ( Buff ) +1 ) ;
          strcpy ( spt , Buff ) ;
          Fonts [ i ] = spt;
          i++;
      }
      return Fonts;
  }
  int uiInitFontLists ( void *Tmp ) {
      DIALOG *D = ( DIALOG * ) Tmp;
      char FontFile [ 500 ] ;
      char *pt;
      int Font , FontSize;
      int count = 0;
      if ( MonoList == NULL ) uiAddFixedFonts ( ) ;
      if ( FontList == NULL ) uiAddFonts ( ) ;
      count = Dcount ( MonoList ) ;
      if ( ( MonoList == NULL ) || ( count == 0 ) ) {
          Nimgs = NULL;
          Pimgs = NULL;
          Bimgs = NULL;
          Mimgs = NULL;
          return 1;
      }
      count = Dcount ( FontList ) ;
      if ( ( FontList == NULL ) || ( count == 0 ) ) {
          Nimgs = NULL;
          Pimgs = NULL;
          Bimgs = NULL;
          Mimgs = NULL;
          return 1;
      }
      FontSize = D->gc.GuiFontSize;
      Font = D->gc.MenuFont%count;
      strcpy ( FontFile , ( char * ) Drecord ( FontList , Font ) ) ;
      Mimgs = ( IMG_STR ** ) kgFontChars ( FontFile , FontSize ) ;
      Font = D->gc.PromptFont%count;
      strcpy ( FontFile , ( char * ) Drecord ( FontList , Font ) ) ;
      Pimgs = ( IMG_STR ** ) kgFontChars ( FontFile , FontSize ) ;
      Font = D->gc.ButtonFont%count;
      strcpy ( FontFile , ( char * ) Drecord ( FontList , Font ) ) ;
      Bimgs = ( IMG_STR ** ) kgFontChars ( FontFile , FontSize ) ;
      count = Dcount ( MonoList ) ;
      Font = D->gc.MsgFont%count;
      strcpy ( FontFile , ( char * ) Drecord ( FontList , Font ) ) ;
      Nimgs = ( IMG_STR ** ) kgFontChars ( FontFile , FontSize ) ;
      return 1;
  }
  int uiFreeFontLists ( ) {
    /* Should npot be called */
      return 0;
      if ( MonoList != NULL ) {
          Dempty ( MonoList ) ;
          MonoList = NULL;
      }
      if ( FontList != NULL ) {
          Dempty ( FontList ) ;
          FontList = NULL;
      }
      return 1;
  }
  DIALOG *getParentDisplay ( void *D ) {
      DIALOG *Dtmp;
      Dtmp = ( DIALOG * ) D;
      if ( Dtmp->Newwin != 1 ) {
          Dtmp = Dtmp->parent;
          if ( Dtmp == NULL ) {printf ( "Parent=NULL\n" ) ; exit ( 0 ) ; }
      }
//  printf("Got Parent\n");
      return Dtmp;
  }
  int kgChangeCurFormat ( DIG *G , char *xfmt , char *yfmt ) {
      kgDC *dc;
      dc = G->dc;
      sprintf ( dc->Posfmt , "%%%s , %%%s" , xfmt , yfmt ) ;
      return 1;
  }
#if 0
  int reset_cur_format ( void ) {
      sprintf ( Posfmt , "%%%s , %%%s" , Xfmt , Yfmt ) ;
      return 1;
  }
  int get_pointer ( int *x , int *y ) {
      return GetPointer ( x , y ) ;
  }
#endif
  void uig_scroll_up ( DIALOG *D , short x1 , short y1 , \
       short x2 , short y2 , short jmp ) \
  {
      int yy1 , yy2;
      int EVGAY;
      EVGAY = D->evgay;
      yy1 = EVGAY-y1;
      yy2 = EVGAY-y2;
      if ( yy1+jmp > yy2 ) return;
      uiscr_scroll_back ( D , x1 , yy1+jmp , x2 , yy2 , jmp ) ;
  }
  short get_key ( int t ) {
      union kbinp { short kbint; char kbc [ 2 ] ; } kb;
      if ( ( kb.kbint = ui_get_kb ( ) ) > 0 ) return ( kb.kbint ) ;
      return ( 0 ) ;
  }
#if 0
  short get_text_position ( RECT *r , int n , int *old_i ) {
      unsigned char ch;
      int dx , dy , yy;
      int i , key;
      union kbinp { short kbint; char kbc [ 2 ] ; } kb;
      for ( ; ; ) {
          if ( ( kb.kbint = ui_get_kb ( ) ) > 0 ) return ( kb.kbint ) ;
          if ( ( key = GetPointer ( & dx , & dy ) ) >= 0 ) {
              yy = EVGAY-dy;
              if ( key == 1 ) {
                  kb.kbc [ 0 ] = '\r' , kb.kbc [ 1 ] = 0;
                  return ( kb.kbint ) ;
              }
              if ( key == 3 ) {
                  kb.kbc [ 1 ] = 0 , kb.kbc [ 0 ] = '\n';
                  return ( kb.kbint ) ;
              }
              else{
                  for ( i = 0; i < n; i++ ) {
                      if ( ( ( r [ i ] .x1-dx ) * ( r [ i ] .x2-dx ) <= 0 ) && \
                       ( ( r [ i ] .y1-yy ) * ( r [ i ] .y2-yy ) <= 0 ) ) {
                          if ( i != *old_i ) {
                              *old_i = i;
                              kb.kbc [ 0 ] = 0 , kb.kbc [ 1 ] = 'p';
                              return ( kb.kbint ) ;
                          }
                      }
                  }
              }
          }
      }
  }
#endif
  short get_kb_o ( int t ) {
      unsigned char ch;
      int i , ierr;
      union kbinp { short kbint; char kbc [ 2 ] ; } kb;
      i = 0;
      while ( ( ( ierr = read ( 0 , & ch , 1 ) ) != 1 ) && ( i < 100 ) ) i++;
  /*
   ierr=read(0,&ch,1);
  */
      if ( ierr != 1 ) { kb.kbc [ 0 ] = 27; return ( kb.kbint ) ; };
      if ( ch != 27 ) kb.kbc [ 0 ] = ch;
      else { kb.kbc [ 0 ] = '\0';
          while ( ( ierr = read ( 0 , & ch , 1 ) ) != 1 ) ;
          if ( ch == '[' ) {
              while ( ( ierr = read ( 0 , & ch , 1 ) ) != 1 ) ;
              switch ( ch ) {
                  case 'A':ch = 'H'; break;
                  case 'B':ch = 'P'; break;
                  case 'C':ch = 'M'; break;
                  case 'D':ch = 'K'; break;
                  default:break;
              }
          };
          kb.kbc [ 1 ] = ch;
      }
      return ( kb.kbint ) ;
  }
#if 0
  void uiMakeFontlist ( void ) {
      if ( Fontlist == NULL ) Fontlist = ( Dlink * ) Loadfontstruct ( ) ;
  }
#endif
  Dlink *uiGetFontlist ( void ) {
#if 0
      if ( Fontlist == NULL ) Fontlist = ( Dlink * ) Loadfontstruct ( ) ;
      return Dcopy ( Fontlist ) ;
#else
      return ( Dlink * ) Loadfontstruct ( ) ;
#endif
  }
  void ui_initialise ( DIG *G ) {
      int i , count , font;
      int EVGAX , EVGAY;
      DIALOG *D;
      kgDC *dc;
      kgWC *wc;
      FONT *pt;
      D = G->D;
/* a copy of wc is maintained for each Gbox for different clipping reasons */
      wc = G->wc;
      *wc = * ( WC ( D ) ) ;
      dc = G->dc;
      dc->wc = wc;
      EVGAX = G->x2 -G->x1;
      EVGAY = G->y2 - G->y1;
      dc->A_size = 11;
      dc->msg_x = 0;
      dc->msg_y = 0;
      dc->msg_x = ( EVGAX-316 ) /2;
      dc->msg_y = EVGAY-24;
      dc->gcur_x = 1 , dc->gcur_y = 1 , dc->gcur_r = 20;
      for ( i = 0; i < 10; i++ ) st_ptr [ i ] = 0;
      dc->cur_x = 320; dc->cur_y = 240;
      dc->ln_style = LN_STYL;
      dc->m_style = M_STYL;
      dc->ln_color = 0x0F;
      wcset_clr ( wc , dc->ln_color ) ;
      dc->c_bound = 4;
      dc->t_color = 14;
      dc->t_pattern = 15;
      dc->t_fill = 1;
      dc->t_font = 0;
      dc->t_bkgr = 0;
      dc->t_bodr = 0;
      dc->fil_color = 0;
      dc->bod_width = 11;
      dc->bod_color = 1;
      dc->DOUBLE = 0;
//  if(Fontlist == NULL ) Fontlist = (Dlink *)Loadfontstruct();
//  dc->Fontlist=uiGetFontlist(); // need further modification
      dc->Fontlist = ( Dlink * ) Loadfontstruct ( ) ;
      count = Dcount ( dc->Fontlist ) ;
      font = 0;
      Dposition ( dc->Fontlist , font+1 ) ;
      pt = ( FONT * ) Getrecord ( dc->Fontlist ) ;
      dc->icpos = pt->icpos;
      dc->icxv = pt->icxv;
      dc->icyv = pt->icyv;
      dc->m_f = pt->m_f;
      dc->icposf0 = dc->icpos; dc->icxvf0 = dc->icxv;
      dc->icyvf0 = dc->icyv; dc->m_f0 = dc->m_f;
      dc->ln_width = 1;
      dc->pr_txt = 1;
      dc->cost = 1.0;
      dc->sint = 0.0;
      dc->txt_ht = 20;
      dc->txt_wt = 20;
      dc->txt_htx = 20;
      dc->txt_wty = 20;
      dc->txt_hty = 20;
      dc->txt_wtx = 20;
      dc->txt_w42 = dc->txt_wt/CFact;
      dc->txt_h42 = dc->txt_ht/CFact;
      dc->txt_sp = 2;
      dc->txt_spy = 2;
      dc->txt_spx = 2;
      dc->v_x1 = 0;
      dc->v_y1 = 0;
      dc->v_x2 = EVGAX;
      dc->v_y2 = EVGAY;
      dc->w_x1 = 0;
      dc->w_y1 = 0;
      dc->w_x2 = 1.0;
      dc->w_y2 = 1.0;
      dc->u_x = EVGAX;
      dc->u_y = EVGAY;
      dc->V_x = EVGAX;
      dc->V_y = EVGAY;
      dc->D_x = 0;
      dc->D_y = 0;
      dc->jmp = 8;
      dc->lcur_x = 0;
      dc->lcur_y = EVGAY;
      dc->rcur_x = EVGAX;
      dc->rcur_y = 0;
      dc->ZBUFF = dc->SET3D = dc->TIFF = dc->PROJ = 0;
      dc->DBL_BUF = 0;
      dc->zbuf = dc->redbuf = dc->greenbuf = dc->bluebuf = dc->clrbuf = NULL;
      dc->D_ON = 1;
      dc->Cutoff = 1;
      dc->v_xt1 = ( int ) ( dc->v_x1-dc->txt_wt-dc->txt_sp ) ;
      dc->v_yt1 = ( int ) ( dc->v_y1-dc->txt_ht ) ;
#if 0
      wc->Clip = Dcopy ( WC ( D )->Clip ) ;
      wc->SBlist = Dcopy ( WC ( D )->SBlist ) ;
#else
      wc->Clip = Dopen ( ) ;
      wc->SBlist = Dopen ( ) ;
#endif
      uiset_sup_clip_limits ( wc , ( dc->v_x1+dc->D_x ) , \
           D->evgay- ( dc->v_y2+dc->D_y ) , \
       ( dc->v_x2+dc->D_x ) , D->evgay- ( dc->v_y1+dc->D_y ) ) ;
      uiset_clip_limits ( wc , ( dc->v_x1+dc->D_x ) , \
           D->evgay- ( dc->v_y2+dc->D_y ) , \
       ( dc->v_x2+dc->D_x ) , D->evgay- ( dc->v_y1+dc->D_y ) ) ;
      uiset_clip_limits ( wc , ( dc->v_x1+dc->D_x ) , \
           D->evgay- ( dc->v_y2+dc->D_y ) , \
       ( dc->v_x2+dc->D_x ) , D->evgay- ( dc->v_y1+dc->D_y ) ) ;
      uiset_sup_clip_limits ( wc , ( dc->v_x1+dc->D_x ) , ( dc->v_y1+dc->D_y ) , \
       ( dc->v_x2+dc->D_x ) , ( dc->v_y2+dc->D_y ) ) ;
      uiset_clip_limits ( wc , ( dc->v_x1+dc->D_x ) , ( dc->v_y1+dc->D_y ) , \
       ( dc->v_x2+dc->D_x ) , ( dc->v_y2+dc->D_y ) ) ;
  /*for (i=0;i<NCLRS;i++) df_clr(i,(int)icode[i][0],(int)icode[i][1],(int)icode[i][2]);*/
      dc->msg_x += G->x1+D->xo;
      dc->msg_y += G->y1+D->yo;
      strcpy ( dc->Posfmt , Posfmt ) ;
      strcpy ( dc->Xfmt , Xfmt ) ;
      strcpy ( dc->Yfmt , Yfmt ) ;
      for ( i = 0; i < 10; i++ ) {
          dc->ln_ptn [ i ] = ln_ptn [ i ] ;
          dc->st_ptr [ i ] = st_ptr [ i ] ;
      }
  }
  void _uimove ( DIG *G , int xp , int yp ) {
      kgDC *dc;
      dc = G->dc;
      dc->cur_x = xp ;
      dc->cur_y = yp ;
      return;
  }
  void _marker ( int x , int y ) {
#if 0
      short fac = 1;
      int px [ 4 ] , py [ 4 ] ;
      short style , mf;
      style = m_style%20;
      mf = m_style/20;
      fac += mf;
      switch ( style ) {
          case 0 :
          _circle_fill ( x , y , ( int ) ( 3*fac ) , ( unsigned int ) ( c_color ) ) ;
          break;
          case 1 :
          _draw_line ( x-3*fac , y-3*fac , x+3*fac , y+3*fac ) ;
          _draw_line ( x-3*fac , y+3*fac , x+3*fac , y-3*fac ) ;
          break;
          case 2 :
          _box_fill ( x-3*fac , y-3*fac , x+3*fac , y+3*fac , \
           ( unsigned int ) c_color ) ;
          break;
          case 3 : _draw_line ( x , y-4*fac , x , y+4*fac ) ;
          _draw_line ( x-4*fac , y , x+4*fac , y ) ;
          _draw_line ( x-2*fac , y-4*fac , x+2*fac , y-4*fac ) ;
          _draw_line ( x-2*fac , y+4*fac , x+2*fac , y+4*fac ) ;
          _draw_line ( x-4*fac , y-2*fac , x-4*fac , y+2*fac ) ;
          _draw_line ( x+4*fac , y-2*fac , x+4*fac , y+2*fac ) ;
          break;
          case 4 : _draw_line ( x-4*fac , y-4*fac , x+4*fac , y+4*fac ) ;
          _draw_line ( x-4*fac , y+4*fac , x+4*fac , y-4*fac ) ;
          _draw_line ( x-4*fac , y , x+4*fac , y ) ;
          _draw_line ( x , y-4*fac , x , y+4*fac ) ;
          break;
          case 5 : _draw_line ( x+3*fac , y , x , y-8*fac ) ;
          _draw_line ( x-3*fac , y , x , y-8*fac ) ;
          _draw_line ( x+3*fac , y , x , y+8*fac ) ;
          _draw_line ( x-3*fac , y , x , y+8*fac ) ;
          put_pixel ( x , y ) ;
          break;
          case 6 : _draw_line ( x-4*fac , y , x+4*fac , y ) ;
          _draw_line ( x , y-4*fac , x , y+4*fac ) ;
          _draw_line ( x+2*fac , y-2*fac , x+2*fac , y+2*fac ) ;
          _draw_line ( x+2*fac , y+2*fac , x-2*fac , y+2*fac ) ;
          _draw_line ( x-2*fac , y+2*fac , x-2*fac , y-2*fac ) ;
          _draw_line ( x-2*fac , y-2*fac , x+2*fac , y-2*fac ) ;
          break;
          case 7 : put_pixel ( x , y ) ;
          _draw_line ( x+7*fac , y , x+3*fac , y-3*fac ) ;
          _draw_line ( x+3*fac , y-3*fac , x , y-7*fac ) ;
          _draw_line ( x , y-7*fac , x-3*fac , y-3*fac ) ;
          _draw_line ( x-3*fac , y-3*fac , x-7*fac , y ) ;
          _draw_line ( x-7*fac , y , x-3*fac , y+3*fac ) ;
          _draw_line ( x-3*fac , y+3*fac , x , y+7*fac ) ;
          _draw_line ( x , y+7*fac , x+3*fac , y+3*fac ) ;
          _draw_line ( x+3*fac , y+3*fac , x+7*fac , y ) ;
          break;
          case 8 : _draw_line ( x-4*fac , y-4*fac , x+4*fac , y+4*fac ) ;
          _draw_line ( x+4*fac , y-4*fac , x-4*fac , y+4*fac ) ;
          _draw_line ( x+4*fac , y+4*fac , x+4*fac , y+2*fac ) ;
          _draw_line ( x+4*fac , y+4*fac , x+2*fac , y+4*fac ) ;
          _draw_line ( x+4*fac , y-4*fac , x+4*fac , y-2*fac ) ;
          _draw_line ( x+4*fac , y-4*fac , x+2*fac , y-4*fac ) ;
          _draw_line ( x-4*fac , y-4*fac , x-2*fac , y-4*fac ) ;
          _draw_line ( x-4*fac , y-4*fac , x-4*fac , y-2*fac ) ;
          _draw_line ( x-4*fac , y+4*fac , x-4*fac , y+2*fac ) ;
          _draw_line ( x-4*fac , y+4*fac , x-2*fac , y+4*fac ) ;
          break;
          case 9 :
          px [ 0 ] = x+4*fac; py [ 0 ] = y; px [ 1 ] = x-4*fac; py [ 1 ] = y+4*fac;
          px [ 2 ] = x-4*fac; py [ 2 ] = y-4*fac;
          _poly_fill ( ( short ) 3 , px , py , ( short ) 0 , \
           ( unsigned int ) c_color ) ;
          break;
          case 10:
          px [ 0 ] = x-4*fac; py [ 0 ] = y; px [ 1 ] = x+4*fac; py [ 1 ] = y+4*fac;
          px [ 2 ] = x+4*fac; py [ 2 ] = y-4*fac;
          _poly_fill ( ( short ) 3 , px , py , ( short ) 0 , \
           ( unsigned int ) c_color ) ;
          break;
          case 11:
          px [ 0 ] = x; py [ 0 ] = y+4*fac; px [ 1 ] = x-4*fac; py [ 1 ] = y-4*fac;
          px [ 2 ] = x+4*fac; py [ 2 ] = y-4*fac;
          _poly_fill ( ( short ) 3 , px , py , ( short ) 0 , \
           ( unsigned int ) c_color ) ;
          break;
          case 12:
          px [ 0 ] = x; py [ 0 ] = y-4*fac; px [ 1 ] = x-4*fac; py [ 1 ] = y+4*fac;
          px [ 2 ] = x+4*fac; py [ 2 ] = y+4*fac;
          _poly_fill ( ( short ) 3 , px , py , ( short ) 0 , \
           ( unsigned int ) c_color ) ;
          break;
          case 13 :
          _circle_fill ( x , y , ( int ) ( 3*fac ) , ( unsigned int ) 0 ) ;
          _circle ( x , y , ( int ) ( 3*fac ) ) ;
          break;
          case 14 :
          _circle_fill ( x , y , ( int ) ( 1*fac ) , ( unsigned int ) ( c_color ) ) ;
          break;
          case 15:
          _draw_line ( x-2*fac , y-2*fac , x+2*fac , y+2*fac ) ;
          _draw_line ( x-2*fac , y+2*fac , x+2*fac , y-2*fac ) ;
          break;
          case 16:
          px [ 0 ] = x+2*fac; py [ 0 ] = y; px [ 1 ] = x-2*fac; py [ 1 ] = y+2*fac;
          px [ 2 ] = x-2*fac; py [ 2 ] = y-2*fac;
          _poly_fill ( ( short ) 3 , px , py , ( short ) 0 , \
           ( unsigned int ) c_color ) ;
          break;
          case 17:
          px [ 0 ] = x-2*fac; py [ 0 ] = y; px [ 1 ] = x+2*fac; py [ 1 ] = y+2*fac;
          px [ 2 ] = x+2*fac; py [ 2 ] = y-2*fac;
          _poly_fill ( ( short ) 3 , px , py , ( short ) 0 , \
           ( unsigned int ) c_color ) ;
          break;
          case 18:
          px [ 0 ] = x; py [ 0 ] = y+2*fac; px [ 1 ] = x-2*fac; py [ 1 ] = y-2*fac;
          px [ 2 ] = x+2*fac; py [ 2 ] = y-2*fac;
          _poly_fill ( ( short ) 3 , px , py , ( short ) 0 , \
           ( unsigned int ) c_color ) ;
          break;
          case 19:
          px [ 0 ] = x; py [ 0 ] = y-2*fac; px [ 1 ] = x-2*fac; py [ 1 ] = y+2*fac;
          px [ 2 ] = x+2*fac; py [ 2 ] = y+2*fac;
          _poly_fill ( ( short ) 3 , px , py , ( short ) 0 , \
           ( unsigned int ) c_color ) ;
          break;
          default :break;
      }
#endif
  }
  void _uimarker ( DIG *G , int x , int y ) {
#if 1
      DIALOG *D;
      int EVGAY;
      kgWC *wc;
      kgDC *dc;
      D = G->D;
      wc = G->wc;
      dc = G->dc;
      EVGAY = D->evgay;
      short fac = 1;
      int px [ 4 ] , py [ 4 ] ;
      short style , mf;
      dc->cur_x = x ;
      dc->cur_y = y ;
      style = dc->m_style%20;
      mf = dc->m_style/20;
      fac += mf;
//  printf("style: %d\n",style);
      switch ( style ) {
          case 0 :
          _uicircle_fill ( G , x , y , ( int ) ( 3*fac ) , dc->ln_color ) ;
          break;
          case 1 :
          _uidraw_line ( wc , x-3*fac , y-3*fac , x+3*fac , y+3*fac ) ;
          _uidraw_line ( wc , x-3*fac , y+3*fac , x+3*fac , y-3*fac ) ;
          break;
          case 2 :
          _uibox_fill ( wc , x-3*fac , y-3*fac , x+3*fac , y+3*fac , dc->ln_color ) ;
          break;
          case 3 : _uidraw_line ( wc , x , y-4*fac , x , y+4*fac ) ;
          _uidraw_line ( wc , x-4*fac , y , x+4*fac , y ) ;
          _uidraw_line ( wc , x-2*fac , y-4*fac , x+2*fac , y-4*fac ) ;
          _uidraw_line ( wc , x-2*fac , y+4*fac , x+2*fac , y+4*fac ) ;
          _uidraw_line ( wc , x-4*fac , y-2*fac , x-4*fac , y+2*fac ) ;
          _uidraw_line ( wc , x+4*fac , y-2*fac , x+4*fac , y+2*fac ) ;
          break;
          case 4 : _uidraw_line ( wc , x-4*fac , y-4*fac , x+4*fac , y+4*fac ) ;
          _uidraw_line ( wc , x-4*fac , y+4*fac , x+4*fac , y-4*fac ) ;
          _uidraw_line ( wc , x-4*fac , y , x+4*fac , y ) ;
          _uidraw_line ( wc , x , y-4*fac , x , y+4*fac ) ;
          break;
          case 5 : _uidraw_line ( wc , x+3*fac , y , x , y-8*fac ) ;
          _uidraw_line ( wc , x-3*fac , y , x , y-8*fac ) ;
          _uidraw_line ( wc , x+3*fac , y , x , y+8*fac ) ;
          _uidraw_line ( wc , x-3*fac , y , x , y+8*fac ) ;
          uiput_pixel ( wc , x , y ) ;
          break;
          case 6 : _uidraw_line ( wc , x-4*fac , y , x+4*fac , y ) ;
          _uidraw_line ( wc , x , y-4*fac , x , y+4*fac ) ;
          _uidraw_line ( wc , x+2*fac , y-2*fac , x+2*fac , y+2*fac ) ;
          _uidraw_line ( wc , x+2*fac , y+2*fac , x-2*fac , y+2*fac ) ;
          _uidraw_line ( wc , x-2*fac , y+2*fac , x-2*fac , y-2*fac ) ;
          _uidraw_line ( wc , x-2*fac , y-2*fac , x+2*fac , y-2*fac ) ;
          break;
          case 7 : uiput_pixel ( wc , x , y ) ;
          _uidraw_line ( wc , x+7*fac , y , x+3*fac , y-3*fac ) ;
          _uidraw_line ( wc , x+3*fac , y-3*fac , x , y-7*fac ) ;
          _uidraw_line ( wc , x , y-7*fac , x-3*fac , y-3*fac ) ;
          _uidraw_line ( wc , x-3*fac , y-3*fac , x-7*fac , y ) ;
          _uidraw_line ( wc , x-7*fac , y , x-3*fac , y+3*fac ) ;
          _uidraw_line ( wc , x-3*fac , y+3*fac , x , y+7*fac ) ;
          _uidraw_line ( wc , x , y+7*fac , x+3*fac , y+3*fac ) ;
          _uidraw_line ( wc , x+3*fac , y+3*fac , x+7*fac , y ) ;
          break;
          case 8 : _uidraw_line ( wc , x-4*fac , y-4*fac , x+4*fac , y+4*fac ) ;
          _uidraw_line ( wc , x+4*fac , y-4*fac , x-4*fac , y+4*fac ) ;
          _uidraw_line ( wc , x+4*fac , y+4*fac , x+4*fac , y+2*fac ) ;
          _uidraw_line ( wc , x+4*fac , y+4*fac , x+2*fac , y+4*fac ) ;
          _uidraw_line ( wc , x+4*fac , y-4*fac , x+4*fac , y-2*fac ) ;
          _uidraw_line ( wc , x+4*fac , y-4*fac , x+2*fac , y-4*fac ) ;
          _uidraw_line ( wc , x-4*fac , y-4*fac , x-2*fac , y-4*fac ) ;
          _uidraw_line ( wc , x-4*fac , y-4*fac , x-4*fac , y-2*fac ) ;
          _uidraw_line ( wc , x-4*fac , y+4*fac , x-4*fac , y+2*fac ) ;
          _uidraw_line ( wc , x-4*fac , y+4*fac , x-2*fac , y+4*fac ) ;
          break;
          case 9 :
          px [ 0 ] = x+4*fac; py [ 0 ] = y; px [ 1 ] = x-4*fac; py [ 1 ] = y+4*fac;
          px [ 2 ] = x-4*fac; py [ 2 ] = y-4*fac;
          _uipoly_fill ( G , ( short ) 3 , px , py , ( short ) 0 , dc->ln_color ) ;
          break;
          case 10:
          px [ 0 ] = x-4*fac; py [ 0 ] = y; px [ 1 ] = x+4*fac; py [ 1 ] = y+4*fac;
          px [ 2 ] = x+4*fac; py [ 2 ] = y-4*fac;
          _uipoly_fill ( G , ( short ) 3 , px , py , ( short ) 0 , dc->ln_color ) ;
          break;
          case 11:
          px [ 0 ] = x; py [ 0 ] = y+4*fac; px [ 1 ] = x-4*fac; py [ 1 ] = y-4*fac;
          px [ 2 ] = x+4*fac; py [ 2 ] = y-4*fac;
          _uipoly_fill ( G , ( short ) 3 , px , py , ( short ) 0 , dc->ln_color ) ;
          break;
          case 12:
          px [ 0 ] = x; py [ 0 ] = y-4*fac; px [ 1 ] = x-4*fac; py [ 1 ] = y+4*fac;
          px [ 2 ] = x+4*fac; py [ 2 ] = y+4*fac;
          _uipoly_fill ( G , ( short ) 3 , px , py , ( short ) 0 , dc->ln_color ) ;
          break;
          case 13 :
          _uicircle_fill ( G , x , y , ( int ) ( 3*fac ) , dc->ln_color ) ;
          _uicircle ( wc , x , y , ( int ) ( 3*fac ) ) ;
          break;
          case 14 :
          _uicircle_fill ( G , x , y , ( int ) ( 1*fac ) , dc->ln_color ) ;
          break;
          case 15:
          _uidraw_line ( wc , x-2*fac , y-2*fac , x+2*fac , y+2*fac ) ;
          _uidraw_line ( wc , x-2*fac , y+2*fac , x+2*fac , y-2*fac ) ;
          break;
          case 16:
          px [ 0 ] = x+2*fac; py [ 0 ] = y; px [ 1 ] = x-2*fac; py [ 1 ] = y+2*fac;
          px [ 2 ] = x-2*fac; py [ 2 ] = y-2*fac;
          _uipoly_fill ( G , ( short ) 3 , px , py , ( short ) 0 , dc->ln_color ) ;
          break;
          case 17:
          px [ 0 ] = x-2*fac; py [ 0 ] = y; px [ 1 ] = x+2*fac; py [ 1 ] = y+2*fac;
          px [ 2 ] = x+2*fac; py [ 2 ] = y-2*fac;
          _uipoly_fill ( G , ( short ) 3 , px , py , ( short ) 0 , dc->ln_color ) ;
          break;
          case 18:
          px [ 0 ] = x; py [ 0 ] = y+2*fac; px [ 1 ] = x-2*fac; py [ 1 ] = y-2*fac;
          px [ 2 ] = x+2*fac; py [ 2 ] = y-2*fac;
          _uipoly_fill ( G , ( short ) 3 , px , py , ( short ) 0 , dc->ln_color ) ;
          break;
          case 19:
          px [ 0 ] = x; py [ 0 ] = y-2*fac; px [ 1 ] = x-2*fac; py [ 1 ] = y+2*fac;
          px [ 2 ] = x+2*fac; py [ 2 ] = y+2*fac;
          _uipoly_fill ( G , ( short ) 3 , px , py , ( short ) 0 , dc->ln_color ) ;
          break;
          default :break;
      }
#endif
  }
  unsigned char uicode ( kgWC *wc , int x , int y ) {
      unsigned char t;
      t = 0;
      if ( y > wc->c_v_y2 ) t |= 0x08;
      if ( y < wc->c_v_y1 ) t |= 0x04;
      if ( x > wc->c_v_x2 ) t |= 0x02;
      if ( x < wc->c_v_x1 ) t++;
      return ( t ) ;
  }
  short uiclip_line ( kgWC *wc , int *xa , int *ya , int *xb , int *yb ) {
      int x , y , tx , ty , xs , ys;
      unsigned char c1 , c2 , c;
//  kgWC *wc;
//  D= (DIALOG *)(D);
//  wc = WC(D);
      xs = wc->c_v_x1;
      ys = wc->c_v_y1;
      c1 = uicode ( wc , *xa , *ya ) ;
      c2 = uicode ( wc , *xb , *yb ) ;
      if ( ( c1 & c2 ) != 0 ) return ( 0 ) ;
      while ( ( c1 != 0 ) || ( c2 != 0 ) ) {
          if ( c1 == 0 ) c = c2;
          else c = c1;
          tx = ( ( *xb ) - ( *xa ) ) ;
          ty = ( ( *yb ) - ( *ya ) ) ;
          if ( c & 1 ) {
              if ( tx == 0 ) return ( 0 ) ;
              y = ( int ) ( *ya+ ( wc->c_v_x1- ( *xa ) ) *ty/tx ) ;
              if ( ( y == ys ) && ( ( ys > wc->c_v_y2 ) \
               || ( ys < wc->c_v_y1 ) ) ) return ( 0 ) ;
              ys = y;
              x = wc->c_v_x1;
              goto jump;
          }
          if ( c & 2 ) {
              if ( tx == 0 ) return ( 0 ) ;
              y = ( int ) ( *ya+ ( wc->c_v_x2- ( *xa ) ) *ty/tx ) ;
              if ( ( y == ys ) && ( ( ys > wc->c_v_y2 ) \
               || ( ys < wc->c_v_y1 ) ) ) return ( 0 ) ;
              ys = y;
              x = wc->c_v_x2;
              goto jump;
          }
          if ( c & 8 ) {
              if ( ty == 0 ) return ( 0 ) ;
              x = ( int ) ( *xa+ ( wc->c_v_y2- ( *ya ) ) *tx/ty ) ;
              if ( x == xs && ( ( xs < wc->c_v_x1 ) || \
               ( xs > wc->c_v_x2 ) ) ) return ( 0 ) ;
              xs = x;
              y = wc->c_v_y2;
              goto jump;
          }
          if ( c & 4 ) {
              if ( ty == 0 ) return ( 0 ) ;
              x = ( int ) ( *xa+ ( wc->c_v_y1- ( *ya ) ) *tx/ty ) ;
              if ( x == xs && ( ( xs < wc->c_v_x1 ) || \
               ( xs > wc->c_v_x2 ) ) ) return ( 0 ) ;
              xs = x;
              y = wc->c_v_y1;
          }
          jump:
          if ( c == c1 ) {
              c1 = uicode ( wc , x , y ) ;
              *xa = x;
              *ya = y;
          }
          else {
              c2 = uicode ( wc , x , y ) ;
              *xb = x;
              *yb = y;
          }
      }
      return ( 1 ) ;
  }
  void uidraw_line ( DIG *G , int xa , int ya , int xb , int yb ) {
      int i , i2 , lnwidth;
      float dx , dy , ct , st , dl;
      int x1 , y1 , x2 , y2 , dx1 , dy1;
      kgWC *wc;
      kgDC *dc;
      wc = G->wc;
      dc = G->dc;
      lnwidth = dc->ln_width-2;
      if ( lnwidth < 1 ) lnwidth = 1;
      if ( lnwidth == 1 ) { _uidraw_line ( wc , xa , ya , xb , yb ) ; return; };
      if ( xa > xb ) {x1 = xa , xa = xb , xb = x1 , y1 = ya , ya = yb , yb = y1; };
      i2 = lnwidth/2;
      dx = ( float ) ( xb-xa ) ;
      dy = ( float ) ( yb-ya ) ;
      dl = ( dx*dx+dy*dy ) ;
      if ( dx == 0. ) {ct = 0.0; st = 1.; }
      else { dl = sqrt ( ( double ) dl ) , ct = ( dx ) /dl , st = ( dy ) /dl; }
      dx1 = ( int ) ( st*i2+0.5 ) ;
      if ( ct < 0. ) dy1 = ( int ) ( ct*i2-0.5 ) ;
      else dy1 = ( int ) ( ct*i2+0.5 ) ;
      xa += dx1 , xb += dx1 , ya -= dy1 , yb -= dy1;
      _uidraw_line ( wc , xa , ya , xb , yb ) ;
      for ( i = 1; i < lnwidth; i++ ) {
          dx1 = ( int ) ( st*i+0.5 ) ;
          if ( ct < 0. ) dy1 = ( int ) ( ct*i-0.5 ) ;
          else dy1 = ( int ) ( ct*i+0.5 ) ;
          _uidraw_line ( wc , xa-dx1 , ya+dy1 , xb-dx1 , yb+dy1 ) ;
      }
      return;
  }
  void _uidraw_ ( DIG *G , int x , int y ) {
      int xa , ya , xb , yb;
      unsigned int ptn;
      short dx , dy , f , inc1 , inc2 , g , dx1 , dy1 , ab;
      short clip , i , ch , rh , c , r;
      float h;
      int EVGAY;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      EVGAY = ( ( DIALOG * ) ( G->D ) )->evgay;
      xa = dc->cur_x;
      ya = dc->cur_y;
      xb = x;
      yb = y;
      {
          clip = uiclip_line ( wc , & xa , & ya , & xb , & yb ) ;
          if ( clip == 0 ) {
              dc->cur_x = x;
              dc->cur_y = y;
              return;
          }
          ptn = dc->ln_ptn [ dc->ln_style%10 ] ;
          dx = xb-xa;
          dy = yb-ya;
          dx1 = abs ( dx ) ;
          dy1 = abs ( dy ) ;
          if ( dx1 > dy1 ) {
              c = xa;
              r = ya;
              f = xb;
              inc1 = 2*dy1;
              g = inc1-dx1;
              inc2 = g-dx1;
              if ( xa < xb ) ch = 1;
              else ch = -1;
              if ( ya < yb ) rh = 1;
              else rh = -1;
          }
          else {
              c = ya;
              r = xa;
              f = yb;
              inc1 = 2*dx1;
              g = inc1-dy1;
              inc2 = g-dy1;
              if ( ya < yb ) ch = 1;
              else ch = -1;
              if ( xa < xb ) rh = 1;
              else rh = -1;
          }
          i = dc->st_ptr [ dc->ln_style%10 ] ;
          while ( abs ( c-f ) > 0 ) {
              i %= 32;
              if ( ( ptn & ( ( ( unsigned int ) 0x80000000 ) >> i ) ) != 0 ) {
                  if ( dx1 > dy1 ) uiput_pixl ( wc , c , ( EVGAY-r ) ) ;
                  else uiput_pixl ( wc , r , ( EVGAY-c ) ) ;
              }
              c+= ch;
              if ( g > 0 ) { r+= rh; g+= inc2; }
              else g+= inc1;
              i++;
          }
      }
      dc->st_ptr [ dc->ln_style%10 ] = i;
      return;
  }
  void _uidraw ( DIG *G , int x , int y ) {
      int i , i2 , stloc;
      float dx , dy , ct , st , dl;
      int x1 , y1 , xa , ya , xb , yb , dx1 , dy1;
      DIALOG *D;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      D = G->D;
      wc = WC ( D ) ;
      xa = dc->cur_x;
      ya = dc->cur_y;
      xb = x;
      yb = y;
      if ( dc->ln_style == 0 ) {uidraw_line ( G , xa , ya , xb , yb ) ; }
      else {
          if ( dc->ln_width == 1 ) _uidraw_ ( G , x , y ) ;
          else {
              stloc = dc->st_ptr [ dc->ln_style%10 ] ;
              if ( xa > xb ) {x1 = xa , xa = xb , xb = x1 , y1 = ya , ya = yb , yb = y1; };
              i2 = dc->ln_width/2;
              dx = ( float ) ( xb-xa ) ;
              dy = ( float ) ( yb-ya ) ;
              dl = ( dx*dx+dy*dy ) ;
              if ( dx == 0. ) {ct = 0.0; st = 1.; }
              else { dl = sqrt ( ( double ) dl ) , ct = ( dx ) /dl , st = ( dy ) /dl; }
              dx1 = ( int ) ( st*i2+0.5 ) ;
              if ( ct < 0. ) dy1 = ( int ) ( ct*i2-0.5 ) ;
              else dy1 = ( int ) ( ct*i2+0.5 ) ;
              xa += dx1 , xb += dx1 , ya -= dy1 , yb -= dy1;
              dc->cur_x = xa , dc->cur_y = ya;
              _uidraw_ ( G , xb , yb ) ;
              for ( i = 1; i < dc->ln_width; i++ ) {
                  dc->st_ptr [ dc->ln_style%10 ] = stloc;
                  dx1 = ( int ) ( st*i+0.5 ) ;
                  if ( ct < 0. ) dy1 = ( int ) ( ct*i-0.5 ) ;
                  else dy1 = ( int ) ( ct*i+0.5 ) ;
                  dc->cur_x = xa-dx1 , dc->cur_y = ya+dy1;
                  _uidraw_ ( G , xb-dx1 , yb+dy1 ) ;
              }
          }
      }
      dc->cur_x = x;
      dc->cur_y = y;
      return;
  }
  int berzier_o ( int *xp , int *yp , float x1 , float y1 , float x4 , \
       float y4 , float x2 , float y2 , float x3 , float y3 ) \
  {
      float t , tm , x , y , xm , ym , dist , dt;
      int n , notok , i;
      n = 2;
      notok = 1;
      while ( ( n < 20 ) && notok ) {
          t = 1.0/n;
          tm = 1.-t;
          x = x1*tm*tm*tm + 3.*x2*tm*tm*t+3.*x3*tm*t*t+x4*t*t*t;
          y = y1*tm*tm*tm + 3.*y2*tm*tm*t+3.*y3*tm*t*t+y4*t*t*t;
          dist = ( x-x1 ) * ( x-x1 ) + ( y-y1 ) * ( y-y1 ) ;
          if ( dist > 2.0 ) n++;
          else notok = 0;
      }
      t = 0; dt = 1.0/n;
      for ( i = 1; i < ( n ) ; i++ ) {
          t+= dt;
          tm = 1.-t;
          x = x1*tm*tm*tm + 3.*x2*tm*tm*t+3.*x3*tm*t*t+x4*t*t*t;
          y = y1*tm*tm*tm + 3.*y2*tm*tm*t+3.*y3*tm*t*t+y4*t*t*t;
          *xp = x+0.5 , *yp = y+0.5;
          xp++, yp++;
      }
      *xp = x4+0.5 , *yp = y4+0.5;
      xp++, yp++;
      return ( n ) ;
  }
  int find_roots ( double a , double b , double c , double *r1 , double *r2 ) {
/* this for special use expecting roots between (0-1) */
      double disc;
      if ( a == 0. ) {
          *r1 = 0; *r2 = 0;
      }
      else {
          disc = b*b - 4*a*c;
          if ( disc < 0 ) {
              *r1 = *r2 = 0;
          }
          else {
              if ( disc == 0 ) {
                  *r1 = *r2 = -b/ ( 2.*a ) ;
                  if ( *r1* ( 1-*r1 ) <= 0 ) *r1 = *r2 = 0;
              }
              else {
                  disc = sqrt ( disc ) ;
                  *r1 = ( -b+disc ) / ( 2.*a ) ;
                  if ( *r1* ( 1-*r1 ) <= 0 ) *r1 = 0;
                  *r2 = ( -b-disc ) / ( 2.*a ) ;
                  if ( *r2* ( 1-*r2 ) <= 0 ) *r2 = 0;
              }
          }
      }
      return 1;
  }
  int compute_n ( float y1 , float y4 , float y2 , float y3 ) {
      int n = 1;
      if ( fabs ( y1-y2 ) *Mag > n ) n = fabs ( y1 - y2 ) *Mag;
      if ( fabs ( y1-y3 ) *Mag > n ) n = fabs ( y1 - y3 ) *Mag;
      if ( fabs ( y1-y4 ) *Mag > n ) n = fabs ( y1 - y4 ) *Mag;
      if ( fabs ( y2-y3 ) *Mag > n ) n = fabs ( y2 - y3 ) *Mag;
      if ( fabs ( y2-y4 ) *Mag > n ) n = fabs ( y2 - y4 ) *Mag;
      if ( fabs ( y3-y4 ) *Mag > n ) n = fabs ( y3 - y4 ) *Mag;
      if ( n > 20 ) n = 20;
      return n;
  }
  int berzier ( int *xp , int *yp , float x1 , float y1 , float x4 , \
       float y4 , float x2 , float y2 , float x3 , float y3 ) \
  {
      float t , tm , x , y , xm , ym , dist , dt , r1 , r2;
      double ax , bx , cx , ay , by , cy , dum1 , dum2;
      int n , notok , i , nx;
      n = 2;
      notok = 1;
      cx = 3* ( x2-x1 ) ;
      cy = 3* ( y2-y1 ) ;
      bx = 3* ( x3-x2 ) -cx;
      by = 3* ( y3-y2 ) -cy;
      ax = x4-x1-cx-bx;
      ay = y4-y1-cy-by;
      find_roots ( 3*ay , 2*by , cy , & dum1 , & dum2 ) ;
      r1 = dum1;
      r2 = dum2;
      t = r1;
      y = ay*t+by;
      y = y*t+cy;
      r1 = y*t+y1;
      t = r2;
      y = ay*t+by;
      y = y*t+cy;
      r2 = y*t+y1;
      n = compute_n ( y1 , y4 , r1 , r2 ) ;
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
      t = 0; dt = 1.0/n;
      for ( i = 1; i < ( n ) ; i++ ) {
          t+= dt;
          x = ax*t+bx;
          x = x*t+cx;
          x = x*t+x1;
          y = ay*t+by;
          y = y*t+cy;
          y = y*t+y1;
          *xp = x*Mag+0.5 , *yp = y*Mag+0.5;
          xp++, yp++;
      }
      *xp = x4*Mag+0.5 , *yp = y4*Mag+0.5;
      xp++, yp++;
      return ( n ) ;
  }
  int ui_fillchr ( kgWC *wc , float *x , float *y , int n , \
       int *x1 , int *y1 , int *x2 , int*y2 ) \
  {
      int xp , yp , temp;
      float xx , yy , xx0 , yy0 , xx1 , xx2 , yy1 , yy2 , xx3 , xx4 , yy3 , yy4;
      short i , j , j1 , k;
      j = 0;
      xx0 = x [ 0 ] ; yy0 = y [ 0 ] ;
      x1 [ j ] = ( xx0 ) *Mag+0.5; y1 [ j ] = ( yy0 ) *Mag+0.5; j++;
      i = 1;
      while ( i < n ) {
          xx = x [ i ] ; yy = y [ i ] ; i++;
          xx1 = x [ i ] ; yy1 = y [ i ] ; i++;
          j1 = j-1;
          if ( ( xx0 == xx1 ) && ( yy0 == yy1 ) ) {
              xp = ( xx*Mag+0.5 ) ; yp = ( yy*Mag+0.5 ) ;
              if ( ( xp != x1 [ j1 ] ) || ( yp != y1 [ j1 ] ) ) \
              {x1 [ j ] = xp; y1 [ j ] = yp; j++; };
          }
          else {
              xp = ( xx*Mag+0.5 ) ; yp = ( yy*Mag+0.5 ) ;
              if ( ( xp != x1 [ j1 ] ) || ( yp != y1 [ j1 ] ) ) {
                  xx2 = ( xx1 ) ; yy2 = ( yy1 ) ;
            /*xx1=x1[j1];yy1=y1[j1];*/
                  xx1 = xx0; yy1 = yy0;
                  xx4 = xx , yy4 = yy;
                  xx3 = ( x [ i ] ) ; yy3 = ( y [ i ] ) ; i++;
                  j += berzier ( & x1 [ j ] , & y1 [ j ] , xx1 , \
                       yy1 , xx4 , yy4 , xx2 , yy2 , xx3 , yy3 ) ;
                  if ( j > 4800 ) { printf ( "Not enough buffer\n" ) ; }
              }
              else i++;
          }
          xx0 = xx; yy0 = yy;
      }
      i = 1;
      while ( i < j ) { if ( ( x1 [ i ] == x1 [ i-1 ] ) &&  \
          ( y1 [ i ] == y1 [ i-1 ] ) ) \
          { j--; for ( k = i; k < j; k++ ) {x1 [ k ] = x1 [ k+1 ] ;
          y1 [ k ] = y1 [ k+1 ] ; }}
          else i++;
      }
      for ( i = 1; i < j; i++ ) {x2 [ i-1 ] = x1 [ i ] ; y2 [ i-1 ] = y1 [ i ] ; }
      x2 [ j-1 ] = x1 [ 0 ] ; y2 [ j-1 ] = y1 [ 0 ] ;
      return ( j ) ;
  }
  void _uimpoly_fill ( kgWC *wc , int n , int *px , int *py , \
       int *px1 , int *py1 , unsigned int b_color ) \
  {
#if 1
      short j , imd;
      short i , cnt , ind;
      int xcord [ 5000 ] ;
      int xlow , xup;
      int txx;
      int ymax [ 5000 ] , ymin [ 5000 ] , xmax [ 5000 ] , \
           xmin [ 5000 ] , tyx , tyn , txn , scan , ygrt;
      unsigned int tempc;
      xup = -1000000;
      xlow = 1000000;
      scan = 1000000;
      ygrt = -1239000;
      for ( i = 0; i < n; ++i ) {
          imd = i;
          if ( py [ i ] > py1 [ imd ] ) {
              ymax [ i ] = py [ i ] ;
              xmax [ i ] = px [ i ] ;
              ymin [ i ] = py1 [ imd ] ;
              xmin [ i ] = px1 [ imd ] ;
          }
          else {
              ymin [ i ] = py [ i ] ;
              xmin [ i ] = px [ i ] ;
              ymax [ i ] = py1 [ imd ] ;
              xmax [ i ] = px1 [ imd ] ;
          }
          if ( ygrt < py [ i ] ) ygrt = py [ i ] ;
          if ( scan > py [ i ] ) scan = py [ i ] ;
          if ( xup < px [ i ] ) xup = px [ i ] ;
          if ( xlow > px [ i ] ) xlow = px [ i ] ;
          j = i-1;
          tyx = ymax [ i ] ;
          tyn = ymin [ i ] ;
          txx = xmax [ i ] ;
          txn = xmin [ i ] ;
          while ( j >= 0 && tyx < ymax [ j ] ) {
              ymax [ j+1 ] = ymax [ j ] ;
              ymin [ j+1 ] = ymin [ j ] ;
              xmax [ j+1 ] = xmax [ j ] ;
              xmin [ j+1 ] = xmin [ j ] ;
              j--;
          }
          ymax [ j+1 ] = tyx;
          ymin [ j+1 ] = tyn;
          xmax [ j+1 ] = txx;
          xmin [ j+1 ] = txn;
      }
      ind = 0;
      if ( ( scan/Mag > wc->c_v_y2 ) || ( ygrt/Mag < wc->c_v_y1 ) || \
       ( xlow/Mag > wc->c_v_x2 ) || ( xup/Mag < wc->c_v_x1 ) ) return;
      tempc = wc->c_color;
      wcset_clr ( wc , b_color ) ;
      if ( ymax [ ind ] == scan ) ind++;
      while ( scan <= ygrt ) {
          cnt = 0;
          while ( ymax [ ind ] == scan ) ++ind; /* excluding upper edges */
          for ( i = ind; i < n; ++i ) {
              if ( ymin [ i ] <= scan ) {
                  if ( ymax [ i ] != ymin [ i ] ) {
                      xcord [ cnt ] = ( ( ( float ) ( xmax [ i ] -xmin [ i ] ) /  \
                          ( ymax [ i ] -ymin [ i ] ) * \
                       ( scan-ymin [ i ] ) +xmin [ i ] +0.5 ) ) ;
                      ++cnt;
                  }
              }
          }
 /*sorting scan line intersection xcords of edges*/
          for ( i = 0; i < cnt; ++i ) {
              j = i-1;
              txx = xcord [ i ] ;
              while ( j >= 0 && txx < xcord [ j ] ) { xcord [ j+1 ] = xcord [ j ] ; j--; }
              xcord [ j+1 ] = ( txx ) ;
          }
 /*displaying scan lines*/
          for ( i = 0; i < cnt; i+= 2 ) {
              _uimhori_line ( wc , ( int ) ( xcord [ i ] /Mag+0.5 ) , ( int ) \
               ( xcord [ i+1 ] /Mag ) , ( ( float ) scan/Mag+0.5 ) ) ;
          }
          scan++;
      }
      wcset_clr ( wc , tempc ) ;
#endif
  }
  void uidrawg0ch ( DIG *G , char c ) {
#if 1
      float x [ 300 ] , y [ 300 ] ;
      int x1 [ 5000 ] , y1 [ 5000 ] , x2 [ 5000 ] , y2 [ 5000 ] ;
      int code , n , i , xdum , ydum , pnts;
      short ptr;
      short j , pnt , asc , tempc;
      float fact = 1.0;
      int xo , yo , temp;
      unsigned int color;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      uiset_greek;
      color = dc->t_color;
      asc = c;
      pnt = asc-32+dc->greek;
      fact = dc->m_f [ pnt ] ;
      xo = ( int ) ( dc->cx+ ( dc->xp ) ) ;
      yo = ( int ) ( dc->cy+dc->yp ) ;
      if ( pnt < 0 ) return;
      if ( pnt > 223 ) return;
      ptr = dc->icpos [ pnt ] ;
//  if((xo>=c_v_xt1)&&(xo<=c_v_x2)&& (yo>=c_v_yt1)&& (yo<=c_v_y2))
      {
          code = dc->icxv [ ptr ] ; n = dc->icyv [ ptr ] ;
          pnts = 0;
          while ( code >= 0 ) {
              ptr++;
              for ( i = 0; i < n; i++ ) {
                  xdum = dc->icxv [ ptr ] ; ydum = dc->icyv [ ptr ] ; ptr++;
                  x [ i ] = xdum*dc->txt_w42+xo; y [ i ] = yo+ydum*dc->txt_h42;
              }
              pnts += ui_fillchr ( wc , x , y , n , & x1 [ pnts ] , \
                   & y1 [ pnts ] , & x2 [ pnts ] , & y2 [ pnts ] ) ;
              code = dc->icxv [ ptr ] ; n = dc->icyv [ ptr ] ;
              if ( code < 0 ) {
                  _uimpoly_fill ( wc , pnts , x1 , y1 , x2 , y2 , color ) ;
                  temp = wc->c_color; wcset_clr ( wc , dc->t_color ) ;
                  for ( i = 0; i < pnts; i++ ) uidraw_line ( G , x1 [ i ] /Mag , \
                       y1 [ i ] /Mag , x2 [ i ] /Mag , y2 [ i ] /Mag ) ;
                  wcset_clr ( wc , temp ) ;
                  pnts = 0;
              }
          }
      }
      dc->xp+= ( dc->txt_wt*fact+dc->txt_sp ) ;
      uireset_greek;
      return;
#endif
  }
  void uidrawgch ( DIG *G , char c ) {
#if 1
      float x [ 300 ] , y [ 300 ] ;
      int code , n , i;
      int x1 [ 5000 ] , y1 [ 5000 ] , x2 [ 5000 ] , y2 [ 5000 ] ;
      int pnts , temp;
      float xdum , ydum;
      short ptr;
      short j , pnt , asc , tempc;
      float xa , ya;
      float fact = 1.0;
      float xo , yo;
      unsigned int color;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      uiset_greek;
      color = dc->t_color;
      asc = c;
      pnt = asc-32+dc->greek;
      fact = dc->m_f [ pnt ] ;
      xo = ( int ) ( ( dc->xp ) ) ;
      yo = ( int ) ( dc->yp ) ;
      if ( pnt < 0 ) return;
      if ( pnt > 223 ) return;
      ptr = dc->icpos [ pnt ] ;
      xa = ( dc->cx+ ( xo ) *dc->cost+yo*dc->sint ) ;
      ya = ( dc->cy- ( xo ) *dc->sint+yo*dc->cost ) ;
 // if((xa>=c_v_xt1)&&(xa<=c_v_x2)&& (ya>=c_v_yt1)&& (ya<=c_v_y2))
      {
          code = dc->icxv [ ptr ] ; n = dc->icyv [ ptr ] ;
          pnts = 0;
          while ( code >= 0 ) {
              ptr++;
              for ( i = 0; i < n; i++ ) {
                  xdum = dc->icxv [ ptr ] ; ydum = dc->icyv [ ptr ] ; ptr++;
                  xdum = ( xo+xdum*dc->txt_w42 ) ;
                  ydum = ( yo+ydum*dc->txt_h42 ) ;
                  x [ i ] = ( dc->cx+ ( xdum ) *dc->cost+ ( ydum ) *dc->sint ) ;
                  y [ i ] = ( dc->cy+ ( ydum ) *dc->cost- ( xdum ) *dc->sint ) ;
              }
              pnts += ui_fillchr ( wc , x , y , n , & x1 [ pnts ] , \
                   & y1 [ pnts ] , & x2 [ pnts ] , & y2 [ pnts ] ) ;
              code = dc->icxv [ ptr ] ; n = dc->icyv [ ptr ] ;
              if ( code < 0 ) {
                  _uimpoly_fill ( wc , pnts , x1 , y1 , x2 , y2 , color ) ;
                  temp = wc->c_color; wcset_clr ( wc , dc->t_color ) ;
                  for ( i = 0; i < pnts; i++ ) uidraw_line ( G , x1 [ i ] /Mag , \
                       y1 [ i ] /Mag , x2 [ i ] /Mag , y2 [ i ] /Mag ) ;
                  wcset_clr ( wc , temp ) ;
                  pnts = 0;
              }
          }
      }
      dc->xp+= ( dc->txt_wt*fact+dc->txt_sp ) ;
      uireset_greek;
      return;
#endif
  }
  void drawg0co ( c ) char c;
  {
#if 0
      float x [ 150 ] , y [ 150 ] ;
      int code , n , i , xdum , ydum;
      short ptr;
      short j , pnt , asc , tempc;
      float fact = 1.0;
      int xo , yo;
      tempc = ln_width;
      ln_width = txt_bold;
/*
  txt_w42=txt_wt/CFact;
  txt_h42=txt_ht/CFact;
*/
      asc = c;
      pnt = asc-32+greek;
      fact = m_f [ pnt ] ;
      xo = ( int ) ( cx+ ( xp ) ) ;
      if ( pr_txt == 0 ) { fact = 1.0; }
      else xo -= ( 1.-fact ) *0.5*txt_wt;
      yo = ( int ) ( cy+yp ) ;
      if ( pnt < 0 ) return;
      if ( pnt > 223 ) return;
      ptr = icpos [ pnt ] ;
      if ( ( xo >= c_v_xt1 ) && ( xo <= c_v_x2 ) && \
       ( yo >= c_v_yt1 ) && ( yo <= c_v_y2 ) ) {
          code = icxv [ ptr ] ; n = icyv [ ptr ] ;
          while ( code >= 0 ) {
              ptr++;
              for ( i = 0; i < n; i++ ) {xdum = icxv [ ptr ] ; ydum = icyv [ ptr ] ; ptr++;
              x [ i ] = xdum*txt_w42+xo; y [ i ] = yo+ydum*txt_h42; }
              t_fillchr_o ( x , y , code , n ) ;
/*
*/
              code = icxv [ ptr ] ; n = icyv [ ptr ] ;
          }
      }
      xp+= ( txt_wt*fact+txt_sp ) ;
      ln_width = tempc;
      return;
#endif
  }
  void drawgcho ( c ) char c;
  {
#if 0
      float x [ 150 ] , y [ 150 ] ;
      int code , n , i;
      float xdum , ydum;
      short ptr;
      short j , pnt , asc , tempc;
      float xa , ya;
      float fact = 1.0;
      float xo , yo;
      tempc = ln_width;
      ln_width = txt_bold;
/*
  txt_w42=txt_wt/CFact;
  txt_h42=txt_ht/3000.0;
*/
      asc = c;
      pnt = asc-32+greek;
      fact = m_f [ pnt ] ;
      xo = ( int ) ( ( xp ) ) ;
      if ( pr_txt == 0 ) { fact = 1.0; }
      else xo -= ( 1.-fact ) *0.5*txt_wt;
      yo = ( int ) ( yp ) ;
      if ( pnt < 0 ) return;
      if ( pnt > 223 ) return;
      ptr = icpos [ pnt ] ;
      xa = ( cx+ ( xo ) *cost+yo*sint ) ;
      ya = ( cy- ( xo ) *sint+yo*cost ) ;
      if ( ( xa >= c_v_xt1 ) && ( xa <= c_v_x2 ) && \
       ( ya >= c_v_yt1 ) && ( ya <= c_v_y2 ) ) {
          code = icxv [ ptr ] ; n = icyv [ ptr ] ;
          while ( code >= 0 ) {
              ptr++;
              for ( i = 0; i < n; i++ ) {xdum = icxv [ ptr ] ; ydum = icyv [ ptr ] ; ptr++;
                  xdum = ( xo+xdum*txt_w42 ) ;
                  ydum = ( yo+ydum*txt_h42 ) ;
                  x [ i ] = ( cx+ ( xdum ) *cost+ ( ydum ) *sint ) ;
                  y [ i ] = ( cy+ ( ydum ) *cost- ( xdum ) *sint ) ;
              }
              t_fillchr_o ( x , y , code , n ) ;
/*
*/
              code = icxv [ ptr ] ; n = icyv [ ptr ] ;
          }
      }
      xp+= ( txt_wt*fact+txt_sp ) ;
      ln_width = tempc;
#endif
      return;
  }
  void drawg0ch_o ( c ) char c;
  {
#if 0
      short j , pnt , asc , tempc;
      float fact = 1.0;
      int x , y , xa , ya , xo , yo;
      float tempx , tempy;
      tempc = ln_width;
      ln_width = txt_bold;
      asc = c;
      pnt = asc-33+greek;
      fact = m_f [ pnt+1 ] ;
      xo = ( int ) ( cx+ ( xp ) ) ;
      if ( pr_txt == 0 ) { xo += ( 1.-fact ) *0.5*txt_wt;
      fact = 1.0; }
      yo = ( int ) ( cy+yp ) ;
      if ( ( xo >= c_v_xt1 ) && ( xo <= c_v_x2 ) && \
       ( yo >= c_v_yt1 ) && ( yo <= c_v_y2 ) ) {
          if ( pnt >= 0 ) {
              j = icpos [ pnt ] ;
              tempx = ( ( float ) icxv [ j ] *txt_w42 ) ;
              tempy = ( ( float ) icyv [ j ] *txt_h42 ) ;
              xa = ( int ) ( xo+ ( tempx ) ) ;
              ya = ( int ) ( yo+ ( tempy ) ) ;
              for ( j = icpos [ pnt ] +1; j < icpos [ pnt+1 ] ; j++ ) {
                  if ( ( icxv [ j ] == icxv [ j-1 ] ) && ( icyv [ j ] == icyv [ j-1 ] ) ) {
                      j++;
                      tempx = ( ( float ) icxv [ j ] *txt_w42 ) ;
                      tempy = ( ( float ) icyv [ j ] *txt_h42 ) ;
                      xa = ( int ) ( xo+ ( tempx ) ) ;
                      ya = ( int ) ( yo+ ( tempy ) ) ;
                      continue;
                  }
                  tempx = ( ( float ) icxv [ j ] *txt_w42 ) ;
                  tempy = ( ( float ) icyv [ j ] *txt_h42 ) ;
                  x = ( int ) ( xo+ ( tempx ) ) ;
                  y = ( int ) ( yo+ ( tempy ) ) ;
                  draw_line ( xa , ya , x , y ) ;
                  xa = x; ya = y;
              }
          }
      }
      xp+= ( txt_wt*fact+txt_sp ) ;
      ln_width = tempc;
#endif
      return;
  }
  void drawgch_o ( c ) char c;
  {
#if 0
      short j , pnt , asc , tempc;
      int x , y , xa , ya;
      float fact = 1.0;
      float tempx , tempy;
      fact = 1.0;
      tempc = ln_width;
      ln_width = txt_bold;
      asc = c;
      pnt = asc-33+greek;
      if ( pr_txt > 0 ) fact = m_f [ pnt+1 ] ;
      xa = ( int ) ( cx+ ( xp ) *cost+yp*sint ) ;
      ya = ( int ) ( cy- ( xp ) *sint+yp*cost ) ;
      if ( ( xa >= c_v_xt1 ) && ( xa <= c_v_x2 ) && \
       ( ya >= c_v_yt1 ) && ( ya <= c_v_y2 ) ) {
          if ( pnt >= 0 ) {
              j = icpos [ pnt ] ;
              tempx = ( xp+ ( float ) icxv [ j ] *txt_w42 ) ;
              tempy = ( yp+ ( float ) icyv [ j ] *txt_h42 ) ;
              xa = ( int ) ( cx+ ( tempx ) *cost+ ( tempy ) *sint ) ;
              ya = ( int ) ( cy+ ( tempy ) *cost- ( tempx ) *sint ) ;
              for ( j = icpos [ pnt ] +1; j < icpos [ pnt+1 ] ; j++ ) {
                  if ( ( icxv [ j ] == icxv [ j-1 ] ) && ( icyv [ j ] == icyv [ j-1 ] ) ) {
                      j++;
                      tempx = ( xp+ ( float ) icxv [ j ] *txt_w42 ) ;
                      tempy = ( yp+ ( float ) icyv [ j ] *txt_h42 ) ;
                      xa = ( int ) ( cx+ ( tempx ) *cost+ ( tempy ) *sint ) ;
                      ya = ( int ) ( cy+ ( tempy ) *cost- ( tempx ) *sint ) ;
                      continue;
                  }
                  tempx = ( ( float ) icxv [ j ] *txt_w42+xp ) ;
                  tempy = ( ( float ) icyv [ j ] *txt_h42+yp ) ;
                  x = ( int ) ( cx+ ( tempx ) *cost+ ( tempy ) *sint ) ;
                  y = ( int ) ( cy+ ( tempy ) *cost- ( tempx ) *sint ) ;
                  if ( ( x != xa ) || ( y != ya ) ) draw_line ( xa , ya , x , y ) ;
                  xa = x; ya = y;
              }
          }
      }
      xp+= ( txt_wt*fact+txt_sp ) ;
      ln_width = tempc;
#endif
      return;
  }
  float get_val ( char *ch ) {
      char chr [ 4 ] ;
      short err;
      float val;
      chr [ 0 ] = *ch; chr [ 1 ] = * ( ch+1 ) ; chr [ 2 ] = * \
       ( ch+2 ) ; chr [ 3 ] = ' ';
      err = sscanf ( chr , "%f" , & val ) ;
      if ( err < 1 ) return ( 1.0 ) ;
      else return ( val ) ;
  }
  void uisetsubsup ( DIG *G , float *fact , short *ishft , char cntl ) {
      L_N *pt;
      kgWC *wc;
      kgDC *dc;
      wc = G->wc;
      dc = G->dc;
      *fact = ( *fact ) *SSF;
      dc->txt_w42 = SSF*dc->txt_w42;
      dc->txt_h42 = SSF*dc->txt_h42;
      dc->txt_wt = SSF*dc->txt_wt;
      dc->txt_ht = SSF*dc->txt_ht;
      if ( cntl == 'S' ) {
          *ishft = ( *ishft ) +1;
          dc->yp = dc->yp+1.4*dc->txt_ht;
      }
      else dc->yp = dc->yp-0.7*dc->txt_ht;
      pt = dc->O_L;
      while ( pt != NULL ) {
          if ( ( pt->x2 ) < 0. ) {
              if ( ( pt->ymax ) < ( dc->yp+1.4*dc->txt_ht ) ) pt->ymax = dc->yp+1.4*dc->txt_ht;
                  
              if ( ( pt->ymin ) > ( dc->yp-0.4*dc->txt_ht ) ) pt->ymin = dc->yp-0.4*dc->txt_ht;
                  
          }
          pt = pt->Pr;
      }
      *ishft = ( *ishft ) *10;
      return;
  }
  void uiresetsubsup ( DIG *G , float *fact , short *ishft ) {
      short is;
      kgWC *wc;
      kgDC *dc;
      wc = G->wc;
      dc = G->dc;
      *ishft = ( *ishft ) /10;
      is = *ishft- ( ( *ishft ) /10 ) *10;
      if ( is == 1 ) {
          *ishft = ( *ishft ) -1;
          dc->yp = dc->yp-1.4*dc->txt_ht;
      }
      else dc->yp = dc->yp+0.7*dc->txt_ht;
      *fact = ( *fact ) /SSF;
      dc->txt_w42 = dc->txt_w42/SSF;
      dc->txt_h42 = dc->txt_h42/SSF;
      dc->txt_wt = dc->txt_wt/SSF;
      dc->txt_ht = dc->txt_ht/SSF;
      return;
  }
  void _uipoly_fill ( DIG *G , int n , int *px , \
       int *py , int flag , unsigned int color ) \
  {
      short j , imd;
      short i , cnt , ind;
      int xcord [ 15 ] ;
      int xlow , xup;
      int txx;
      int ymax [ 300 ] , ymin [ 300 ] , xmax [ 300 ] , \
           xmin [ 300 ] , tyx , tyn , txn , scan , ygrt;
      unsigned int tempc;
      kgWC *wc;
      kgDC *dc;
      dc = G->dc;
      wc = G->wc;
      xup = -100000;
      xlow = 1000000;
      scan = 10000;
      ygrt = -1239000;
      for ( i = 0; i < n; ++i ) {
          imd = ( i+1 ) %n;
          if ( py [ i ] > py [ imd ] ) {
              ymax [ i ] = py [ i ] ;
              xmax [ i ] = px [ i ] ;
              ymin [ i ] = py [ imd ] ;
              xmin [ i ] = px [ imd ] ;
          }
          else {
              ymin [ i ] = py [ i ] ;
              xmin [ i ] = px [ i ] ;
              ymax [ i ] = py [ imd ] ;
              xmax [ i ] = px [ imd ] ;
          }
          if ( ygrt < py [ i ] ) ygrt = py [ i ] ;
          if ( scan > py [ i ] ) scan = py [ i ] ;
          if ( xup < px [ i ] ) xup = px [ i ] ;
          if ( xlow > px [ i ] ) xlow = px [ i ] ;
          j = i-1;
          tyx = ymax [ i ] ;
          tyn = ymin [ i ] ;
          txx = xmax [ i ] ;
          txn = xmin [ i ] ;
          while ( j >= 0 && tyx < ymax [ j ] ) {
              ymax [ j+1 ] = ymax [ j ] ;
              ymin [ j+1 ] = ymin [ j ] ;
              xmax [ j+1 ] = xmax [ j ] ;
              xmin [ j+1 ] = xmin [ j ] ;
              j--;
          }
          ymax [ j+1 ] = tyx;
          ymin [ j+1 ] = tyn;
          xmax [ j+1 ] = txx;
          xmin [ j+1 ] = txn;
      }
      ind = 0;
      if ( ( scan > wc->c_v_y2 ) || ( ygrt < wc->c_v_y1 ) || \
       ( xlow > wc->c_v_x2 ) || ( xup < wc->c_v_x1 ) ) return;
      tempc = wc->c_color;
      wcset_clr ( wc , color ) ;
      if ( ymax [ ind ] == scan ) ind++;
      while ( scan <= ygrt ) {
          cnt = 0;
          while ( ymax [ ind ] == scan ) ++ind; /* excluding upper edges */
          for ( i = ind; i < n; ++i ) {
              if ( ymin [ i ] <= scan ) {
                  if ( ymax [ i ] != ymin [ i ] ) {
                      xcord [ cnt ] = ( ( ( float ) ( xmax [ i ] -xmin [ i ] ) /  \
                          ( ymax [ i ] -ymin [ i ] ) * \
                       ( scan-ymin [ i ] ) +xmin [ i ] ) +0.5 ) ;
                      ++cnt;
                  }
              }
          }
 /*sorting scan line intersection xcords of edges*/
          for ( i = 0; i < cnt; ++i ) {
              j = i-1;
              txx = xcord [ i ] ;
              while ( j >= 0 && txx < xcord [ j ] ) { xcord [ j+1 ] = xcord [ j ] ; j--; }
              xcord [ j+1 ] = ( txx ) ;
          }
 /*displaying scan lines*/
          for ( i = 0; i < cnt; i+= 2 ) _uihori_line ( wc , \
               xcord [ i ] , xcord [ i+1 ] , scan ) ;
          scan++;
      }
      wcset_clr ( wc , dc->ln_color ) ;
      if ( flag == 1 ) for ( i = 0; i < n; i++ ) _uidraw_line  \
          ( wc , px [ i ] , py [ i ] , px [ \
       ( i+1 ) %n ] , py [ ( i+1 ) %n ] ) ;
      wcset_clr ( wc , tempc ) ;
  }
  void _uicircle ( kgWC *wc , int x , int y , int r ) {
      short i , j;
      int ym , sqrd;
      sqrd = r*r;
      ym = y- ( short ) ( r/sqrt ( ( double ) 2. ) ) ;
      for ( j = y; j >= ym; --j ) {
          i = x+ ( short ) sqrt ( ( double ) sqrd- ( ( y-j ) * ( y-j ) ) ) ;
          uiput_pixel ( wc , i , j ) ;
          uiput_pixel ( wc , i , 2*y-j ) ;
          uiput_pixel ( wc , x+y-j , y+i-x ) ;
          uiput_pixel ( wc , x-y+j , y+i-x ) ;
          uiput_pixel ( wc , 2*x-i , 2*y-j ) ;
          uiput_pixel ( wc , 2*x-i , j ) ;
          uiput_pixel ( wc , x-y+j , y-i+x ) ;
          uiput_pixel ( wc , x+y-j , y-i+x ) ;
      }
  }
  void _uicircle_fill ( DIG *G , int x , int y , int r , unsigned int color ) {
      int i , j;
      int xm , ym;
      double t , sqrd;
      unsigned int temp;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      sqrd = ( double ) r* ( double ) r;
      ym = y- ( short ) ( r/sqrt ( ( double ) 2. ) ) ;
      temp = wc->c_color;
      uiput_pixel ( wc , x , y+r ) , uiput_pixel ( wc , x , y-r ) ;
      uiput_pixel ( wc , x+r , y ) , uiput_pixel ( wc , x-r , y ) ;
      wcset_clr ( wc , color ) ;
      _uihori_line ( wc , x-r+1 , x+r-1 , y ) ;
      for ( j = 1; j < r; j++ ) {
          t = sqrd- ( float ) ( j*j ) ;
          i = ( sqrt ( t ) ) +0.5;
          wcset_clr ( wc , temp ) ;
          uiput_pixel ( wc , x-i , y-j ) ;
          uiput_pixel ( wc , x+i , y-j ) ;
          uiput_pixel ( wc , x-i , y+j ) ;
          uiput_pixel ( wc , x+i , y+j ) ;
          wcset_clr ( wc , color ) ;
          _uihori_line ( wc , x-i+1 , x+i-1 , y-j ) ;
          _uihori_line ( wc , x-i+1 , x+i-1 , y+j ) ;
      }
      wcset_clr ( wc , temp ) ;
  }
/* copying from off screen to screen
   no clearing of off screen memory
   made for moving logo
*/
  char cursor_move ( float *xx , float*yy ) {
      char ch , key;
#if 0
      gcur_x = scr_x ( *xx ) ;
      gcur_y = EVGAY-scr_y ( *yy ) ;
      scr_back ( gcur_x-8 , gcur_y-8 , gcur_x+8 , gcur_y+8 ) ;
      draw_cursor ( ) ;
      kb.kbint = get_key ( 1 ) ;
      ch = kb.kbc [ 0 ] ;
      key = ch;
      while ( ch != '\r' ) {
          if ( ch != '\0' ) {
              if ( ( isdigit ( ch ) ) && ( ch != '0' ) ) jmp = ch-'0';
              else {
                  scr_recover ( ) ;
                  *xx = usr_x ( gcur_x ) ;
                  *yy = usr_y ( ( EVGAY-gcur_y ) ) ;
                  return ( key ) ;
              }
          }
          else {
              ch = kb.kbc [ 1 ] ;
              switch ( ch ) {
                  case 'H' : cursor_up ( ) ;
                  break;
                  case 'P' : cursor_down ( ) ;
                  break;
                  case 'K' : cursor_left ( ) ;
                  break;
                  case 'M' : cursor_right ( ) ;
                  break;
                  default : ch = getch ( ) ;
                  continue;
              }
          }
          kb.kbint = get_key ( 1 ) ;
          ch = kb.kbc [ 0 ] ;
          key = ch;
      }
      scr_recover ( ) ;
      *xx = usr_x ( gcur_x ) ;
      *yy = usr_y ( ( EVGAY-gcur_y ) ) ;
#endif
      return ( key ) ;
  }
#if 1
  void cursor_left ( kgDC *dc ) {
      dc->gcur_x-= dc->jmp;
//  draw_cursor();
  }
  void cursor_right ( kgDC *dc ) {
      dc->gcur_x+= dc->jmp;
//  draw_cursor();
  }
  void cursor_up ( kgDC *dc ) {
      dc->gcur_y-= dc->jmp;
//  draw_cursor();
  }
  void cursor_down ( kgDC *dc ) {
      dc->gcur_y+= dc->jmp;
//   draw_cursor();
  }
#endif
  void draw_lcur ( DIG *G ) {
      unsigned int temp , c_color;
      int EVGAY;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      EVGAY = wc->EVGAY;
      temp = wc->c_color;
      c_color = 0x0F;
      uiset_clr ( wc->D , c_color ) ;
      _uihori_line ( wc , dc->lcur_x , dc->lcur_x+16 , EVGAY-dc->lcur_y ) ;
      _uivert_line ( wc , EVGAY-dc->lcur_y , EVGAY-dc->lcur_y+16 , dc->lcur_x ) ;
      c_color = temp;
      uiset_clr ( wc->D , c_color ) ;
  }
  void draw_rcur ( DIG *G ) {
      unsigned int temp , c_color;
      int EVGAY;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      EVGAY = wc->EVGAY;
      temp = wc->c_color;
      c_color = 0x0F;
      uiset_clr ( wc->D , c_color ) ;
      _uihori_line ( wc , dc->rcur_x , dc->rcur_x-16 , EVGAY-dc->rcur_y ) ;
      _uivert_line ( wc , EVGAY-dc->rcur_y , EVGAY-dc->rcur_y-16 , dc->rcur_x ) ;
      c_color = temp;
      uiset_clr ( wc->D , c_color ) ;
  }
  char lzoom ( DIG *G ) {
      char ch , key;
#if 0
      int pointer , dx , dy , incr;
      DIALOG *D;
      kgWC *wc;
      D = G->D;
      wc = G->wc;
      jmp = 8;
      jmpy = ( float ) jmp/ ( v_x2-v_x1 ) * ( v_y2-v_y1 ) ;
      ylcur = ( float ) lcur_y;
      yrcur = ( float ) rcur_y;
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( wc ) ;
      draw_rcur ( wc ) ;
/*  kb.kbint = get_key(1);
  ch=kb.kbc[0];
  key=ch;
  while (ch!='\r')*/
      for ( ; ; ) {
          if ( ( kb.kbint = get_kb ( ) ) > 0 ) {
              ch = kb.kbc [ 0 ] ;
              if ( ch != '\0' ) {
                  if ( ( isdigit ( ch ) ) && ( ch != '0' ) ) {jmp = ch-'0'; jmpy = \
                   ( float ) jmp/ ( v_x2-v_x1 ) * ( v_y2-v_y1 ) ; };
                  if ( ch == '\r' ) goto jump;
              }
              else {
                  ch = kb.kbc [ 1 ] ;
                  switch ( ch ) {
                      case 'H' :
                      case 'M' : zoom_up ( G ) ;
                      break;
                      case 'P' :
                      case 'K' : zoom_down ( G ) ;
                      break;
                      default :
                      break;
                  }
              }
          }
      }
      jump:
      key = ch;
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
#endif
      return ( key ) ;
  }
  void _uiDraw_Bound ( kgWC *wc , int x1 , int y1 , int x2 , int y2 ) {
      _uiLINE ( wc , x1 , y1 , x2 , y1 ) ;
      _uiLINE ( wc , x2 , y1 , x2 , y2 ) ;
      _uiLINE ( wc , x2 , y2 , x1 , y2 ) ;
      _uiLINE ( wc , x1 , y2 , x1 , y1 ) ;
  }
#if 0
  void pointer_zoom ( DIG *G ) {
      int key , x0 , y0 , x1 , y1 , xmin , ymin , xmax , ymax;
      int x2 , y2;
      static int entry;
      entry = 0;
      while ( GetPointer ( & x1 , & y1 ) > 0 ) ;
  /*while(GetPointer(&x0,&y0)<=0);*/
      cross_getpointer ( G , & x0 , & y0 ) ;
      x2 = x0 , y2 = y0;
      while ( GetPointer ( & x1 , & y1 ) > 0 ) ;
      gbell ( ) ;
      while ( ( key = GetPointer ( & x1 , & y1 ) ) <= 0 ) {
          if ( ( key == 0 ) && ( ( x2 != x1 ) || ( y2 != y1 ) ) ) {
              if ( entry ) {
                  kg_scr_recover ( G->wc ) ;
                  kg_scr_recover ( G->wc ) ;
                  kg_scr_recover ( G->wc ) ;
                  kg_scr_recover ( G->wc ) ;
              };
              entry = 1;
              if ( x0 < x1 ) xmin = x0 , xmax = x1;
              else xmin = x1 , xmax = x0;
              if ( y0 < y1 ) ymin = y0 , ymax = y1;
              else ymin = y1 , ymax = y0;
              kg_scr_back ( G->wc , xmin , ymin , xmax , ymin+1 ) ;
              kg_scr_back ( G->wc , xmin , ymax-1 , xmax , ymax ) ;
              kg_scr_back ( G->wc , xmin , ymin , xmin+1 , ymax ) ;
              kg_scr_back ( G->wc , xmax-1 , ymin , xmax , ymax ) ;
              _uiDraw_Bound ( G->wc , xmin , ymin , xmax , ymax ) ;
              x2 = x1 , y2 = y1;
          }
      }
      if ( entry ) {
          kg_scr_recover ( G->wc ) ;
          kg_scr_recover ( G->wc ) ;
          kg_scr_recover ( G->wc ) ;
          kg_scr_recover ( G->wc ) ;
      };
      if ( x0 < x1 ) xmin = x0 , xmax = x1;
      else xmin = x1 , xmax = x0;
      if ( y0 < y1 ) ymin = y0 , ymax = y1;
      else ymin = y1 , ymax = y0;
      if ( ( xmin != xmax ) && ( ymin != ymax ) ) lcur_x = xmin , \
           lcur_y = ymax , rcur_x = xmax , rcur_y = ymin;
      while ( ( GetPointer ( & x1 , & y1 ) ) > 0 ) ;
      rmv_pointer ( ) ;
      return;
  }
#endif
  void zoom_up ( DIG *G ) {
#if 0
      DIALOG *D;
      kgWC *wc;
      wc = G->wc;
      D = G->D;
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
      lcur_x-= jmp;
/*  lcur_y+=jmp-2;*/
      ylcur += jmpy;
      lcur_y = ylcur+0.5;
      rcur_x+= jmp;
      yrcur -= jmpy;
      rcur_y = yrcur+0.5;
/*  rcur_y-=jmp-2;*/
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( wc ) ;
      draw_rcur ( wc ) ;
#endif
  }
  void zoom_down ( DIG *G ) {
#if 0
      DIALOG *D;
      kgWC *wc;
      wc = G->wc;
      D = G->D;
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
      lcur_x+= jmp;
/*  lcur_y-=jmp-2;*/
      ylcur -= jmpy;
      lcur_y = ylcur+0.5;
      rcur_x-= jmp;
/*  rcur_y+=jmp-2;*/
      yrcur += jmpy;
      rcur_y = yrcur+0.5;
      if ( lcur_y <= rcur_y ) {
          gbell ( ) ;
          lcur_x-= jmp;
          ylcur += jmpy;
          lcur_y = ylcur+0.5;
          rcur_x+= jmp;
          yrcur -= jmpy;
          rcur_y = yrcur+0.5;
      }
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( wc ) ;
      draw_rcur ( wc ) ;
#endif
  }
  char pan ( DIG *G ) {
      char ch , key;
#if 0
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( D ) ;
      draw_rcur ( D ) ;
      kb.kbint = get_key ( 1 ) ;
      ch = kb.kbc [ 0 ] ;
      key = ch;
      while ( ch != '\r' ) {
          while ( ch != '\0' ) {
              if ( ( isdigit ( ch ) ) && ( ch != '0' ) ) jmp = ch-'0';
              key = ch;
              kb.kbint = get_key ( 1 ) ;
              ch = kb.kbc [ 0 ] ;
              if ( ch == '\r' ) {
                  kg_scr_recover ( D ) ;
                  kg_scr_recover ( D ) ;
                  return ( key ) ;
              }
          }
          ch = kb.kbc [ 1 ] ;
          switch ( ch ) {
              case 'H' : pan_up ( D ) ;
              break;
              case 'K' : pan_left ( D ) ;
              break;
              case 'P' : pan_down ( D ) ;
              break;
              case 'M' : pan_right ( D ) ;
              break;
              default :
              break;
          }
          kb.kbint = get_key ( 1 ) ;
          ch = kb.kbc [ 0 ] ;
      }
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
#endif
      return ( key ) ;
  }
#if 0
  void pan_up ( DIALOG *D ) {
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
      lcur_y-= jmp;
      rcur_y-= jmp;
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( D ) ;
      draw_rcur ( D ) ;
  }
  void pan_down ( DIALOG *D ) {
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
      lcur_y+= jmp;
      rcur_y+= jmp;
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( D ) ;
      draw_rcur ( D ) ;
  }
  void pan_left ( DIALOG *D ) {
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
      lcur_x-= jmp;
      rcur_x-= jmp;
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( D ) ;
      draw_rcur ( D ) ;
  }
  void pan_right ( DIALOG *D ) {
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
      lcur_x+= jmp;
      rcur_x+= jmp;
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( D ) ;
      draw_rcur ( D ) ;
  }
  char stretch ( DIALOG *D ) {
      char ch , key;
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( D ) ;
      draw_rcur ( D ) ;
      kb.kbint = get_key ( 1 ) ;
      ch = kb.kbc [ 0 ] ;
      key = ch;
      while ( ch != '\r' ) {
          while ( ch != '\0' ) {
              if ( ( isdigit ( ch ) ) && ( ch != '0' ) ) jmp = ch-'0';
              key = ch;
              kb.kbint = get_key ( 1 ) ;
              ch = kb.kbc [ 0 ] ;
              if ( ch == '\r' ) {
                  kg_scr_recover ( D ) ;
                  kg_scr_recover ( D ) ;
                  return ( key ) ;
              }
          }
          ch = kb.kbc [ 1 ] ;
          switch ( ch ) {
              case 'H' : vstretch ( D ) ;
              break;
              case 'M' : hstretch ( D ) ;
              break;
              case 'P' : vshrink ( D ) ;
              break;
              case 'K' : hshrink ( D ) ;
              break;
              default :
              break;
          }
          kb.kbint = get_key ( 1 ) ;
          ch = kb.kbc [ 0 ] ;
      }
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
      return ( key ) ;
  }
  void vstretch ( DIALOG *D ) {
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
      lcur_y+= jmp;
      rcur_y-= jmp;
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( D ) ;
      draw_rcur ( D ) ;
  }
  void vshrink ( DIALOG *D ) {
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
      lcur_y-= jmp;
      rcur_y+= jmp;
      if ( lcur_y <= rcur_y ) {
          gbell ( ) ;
          lcur_y+= jmp;
          rcur_y-= jmp;
      }
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( D ) ;
      draw_rcur ( D ) ;
  }
  void hstretch ( DIALOG *D ) {
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
      lcur_x-= jmp;
      rcur_x+= jmp;
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( D ) ;
      draw_rcur ( D ) ;
  }
  void hshrink ( DIALOG *D ) {
      kg_scr_recover ( D ) ;
      kg_scr_recover ( D ) ;
      lcur_x+= jmp;
      rcur_x-= jmp;
      if ( lcur_x >= rcur_x ) {
          gbell ( ) ;
          lcur_x-= jmp;
          rcur_x+= jmp;
      }
      kg_scr_back ( D , lcur_x , lcur_y-16 , lcur_x+16 , lcur_y ) ;
      kg_scr_back ( D , rcur_x-16 , rcur_y , rcur_x , rcur_y+16 ) ;
      draw_lcur ( D ) ;
      draw_rcur ( D ) ;
  }
#endif
  void _uigodard_fill ( DIG *G , int n , int *px , \
       int *py , int flag , float *pv ) \
  {
      short j , imd;
      short i , cnt , ind;
      int xcord [ 15 ] ;
      int xlow , xup;
      int txx;
      int ymax [ 15 ] , ymin [ 15 ] , xmax [ 15 ] , xmin [ 15 ] , \
           tyx , tyn , txn , scan , ygrt;
      float pmax [ 15 ] , pmin [ 15 ] , pcord [ 15 ] , tpx , tpn , tpp;
      unsigned int tempc;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      xup = -100000;
      xlow = 1000000;
      scan = 10000;
      ygrt = -1239000;
      for ( i = 0; i < n; ++i ) {
          imd = ( i+1 ) %n;
          if ( py [ i ] > py [ imd ] ) {
              ymax [ i ] = py [ i ] ;
              xmax [ i ] = px [ i ] ;
              pmax [ i ] = pv [ i ] ;
              ymin [ i ] = py [ imd ] ;
              xmin [ i ] = px [ imd ] ;
              pmin [ i ] = pv [ imd ] ;
          }
          else {
              ymin [ i ] = py [ i ] ;
              xmin [ i ] = px [ i ] ;
              pmin [ i ] = pv [ i ] ;
              ymax [ i ] = py [ imd ] ;
              xmax [ i ] = px [ imd ] ;
              pmax [ i ] = pv [ imd ] ;
          }
          if ( ygrt < py [ i ] ) ygrt = py [ i ] ;
          if ( scan > py [ i ] ) scan = py [ i ] ;
          if ( xup < px [ i ] ) xup = px [ i ] ;
          if ( xlow > px [ i ] ) xlow = px [ i ] ;
          j = i-1;
          tyx = ymax [ i ] ;
          tyn = ymin [ i ] ;
          txx = xmax [ i ] ;
          txn = xmin [ i ] ;
          tpx = pmax [ i ] ;
          tpn = pmin [ i ] ;
          while ( j >= 0 && tyx < ymax [ j ] ) {
              ymax [ j+1 ] = ymax [ j ] ;
              ymin [ j+1 ] = ymin [ j ] ;
              xmax [ j+1 ] = xmax [ j ] ;
              xmin [ j+1 ] = xmin [ j ] ;
              pmax [ j+1 ] = pmax [ j ] ;
              pmin [ j+1 ] = pmin [ j ] ;
              j--;
          }
          ymax [ j+1 ] = tyx;
          ymin [ j+1 ] = tyn;
          xmax [ j+1 ] = txx;
          xmin [ j+1 ] = txn;
          pmax [ j+1 ] = tpx;
          pmin [ j+1 ] = tpn;
      }
      ind = 0;
      if ( ( scan > wc->c_v_y2 ) || ( ygrt < wc->c_v_y1 ) || \
       ( xlow > wc->c_v_x2 ) || ( xup < wc->c_v_x1 ) ) return;
      if ( ymax [ ind ] == scan ) ind++;
      while ( scan <= ygrt ) {
          cnt = 0;
          while ( ymax [ ind ] == scan ) ++ind; /* excluding upper edges */
          for ( i = ind; i < n; ++i ) {
              if ( ymin [ i ] <= scan ) {
                  if ( ymax [ i ] != ymin [ i ] ) {
                      xcord [ cnt ] = ( ( ( float ) ( xmax [ i ] -xmin [ i ] ) /  \
                          ( ymax [ i ] -ymin [ i ] ) * \
                       ( scan-ymin [ i ] ) +xmin [ i ] ) +0.5 ) ;
                      pcord [ cnt ] = ( ( ( float ) ( pmax [ i ] -pmin [ i ] ) /  \
                          ( ymax [ i ] -ymin [ i ] ) * \
                       ( scan-ymin [ i ] ) +pmin [ i ] ) ) ;
                      ++cnt;
                  }
              }
          }
 /*sorting scan line intersection xcords of edges*/
          for ( i = 0; i < cnt; ++i ) {
              j = i-1;
              txx = xcord [ i ] ;
              tpp = pcord [ i ] ;
              while ( j >= 0 && txx < xcord [ j ] ) {
                  xcord [ j+1 ] = xcord [ j ] , pcord [ j+1 ] = pcord [ j ] ;
                  j--;
              }
              xcord [ j+1 ] = ( txx ) ;
              pcord [ j+1 ] = ( tpp ) ;
          }
 /*displaying scan lines*/
          for ( i = 0; i < cnt; i+= 2 ) uigodr_line ( G , xcord [ i ] , \
               xcord [ i+1 ] , scan , pcord [ i ] , pcord [ i+1 ] ) ;
          scan++;
      }
      if ( flag == 1 ) for ( i = 0; i < n; i++ ) _uidraw_line  \
          ( wc , px [ i ] , py [ i ] , px [ \
       ( i+1 ) %n ] , py [ ( i+1 ) %n ] ) ;
  }
  void uigodr_line ( DIG *G , int xa , int xb , int y , float pa , float pb ) {
      int i , pos , p1 , p2 , yy;
      int temp;
      int tempc , clr , c_color;
      float tempp , pval , pfac;
      int EVGAY;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      c_color = wc->c_color;
      EVGAY = wc->EVGAY;
      if ( ( y > wc->c_v_y2 ) || ( y < wc->c_v_y1 ) ) return;
      yy = EVGAY-y;
      p1 = pa+0.5;
      p2 = pb+0.5;
      if ( p1 < 1 ) p1 = dc->Cutoff;
      if ( p2 < 1 ) p2 = dc->Cutoff;
      if ( p1 > dc->max_clr ) p1 = dc->max_clr;
      if ( p2 > dc->max_clr ) p2 = dc->max_clr;
      if ( p1 == p2 ) {
          if ( p1 > 0 ) p1 += dc->min_clr;
          tempc = wc->c_color;
          wcset_clr ( wc , p1 ) ;
          _uihori_line ( wc , xa , xb , y ) ;
          wcset_clr ( wc , tempc ) ;
          return;
      }
      if ( xb < xa ) {
          temp = xa;
          tempp = pa;
          xa = xb;
          xb = temp;
          pa = pb;
          pb = tempp;
      }
      if ( xa < xb ) {
          if ( xa > wc->c_v_x2 ) return;
          if ( xb < wc->c_v_x1 ) return;
      }
      else {
          if ( xb > wc->c_v_x2 ) return;
          if ( xa < wc->c_v_x1 ) return;
      }
      pfac = 0;
      if ( xb != xa ) pfac = ( pb-pa ) / ( xb-xa ) ;
      if ( xa < wc->c_v_x1 ) {
          pval = pa+ ( wc->c_v_x1-xa ) *pfac;
          xa = wc->c_v_x1;
          pa = pval;
      }
      if ( xb > wc->c_v_x2 ) {
          pval = pa+ ( wc->c_v_x2-xa ) *pfac;
          xb = wc->c_v_x2;
          pb = pval;
      }
      for ( i = xa; i <= xb; i++ ) {
          tempc = ( int ) ( pa+ ( i-xa ) *pfac+0.5 ) ;
          if ( tempc < 1 ) tempc = dc->Cutoff;
          if ( tempc > dc->max_clr ) tempc = dc->max_clr;
          if ( tempc > 0 ) tempc += dc->min_clr;
          wcset_clr ( wc , tempc ) ;
          uiput_pixl ( wc , i , yy ) ;
      }
      wcset_clr ( wc , c_color ) ;
  }
  void ui_vu_port_o ( DIG *G , float x1 , float y1 , float x2 , float y2 ) {
      float t;
      int EVGAX , EVGAY;
      DIALOG *D , *Parent;
      kgDC *dc;
      kgWC *wc;
      D = ( G->D ) ;
      Parent = ( G->D ) ;
      if ( D == NULL ) return;
      wc = G->wc;
      dc = G->dc;
      dc->vu_x1 = x1 , dc->vu_y1 = y1 , dc->vu_x2 = x2 , dc->vu_y2 = y2;
      EVGAX = G->x2-G->x1+1;
      EVGAY = abs ( G->y2-G->y1 ) +1;
      dc->v_x1 = x1* ( float ) EVGAX;
      dc->v_y1 = y1* ( float ) EVGAY;
      dc->v_x2 = x2* ( float ) EVGAX;
      dc->v_y2 = y2* ( float ) EVGAY;
//   if( v_y2 <= 0) {setnormal();exit(0);}
      if ( dc->v_y2 <= 0 ) {printf ( "dc->v_y2<=0:%d\n" , dc->v_y2 ) ;
      exit ( 0 ) ; }
      dc->D_x = dc->v_x1+G->x1+D->xo;
      dc->D_y = dc->v_y1-G->y2+D->evgay- ( D->yo ) ;
      dc->V_x = dc->v_x2-dc->v_x1;
      dc->V_y = dc->v_y2-dc->v_y1;
      t = ( dc->V_x*dc->V_x+dc->V_y*dc->V_y ) ;
      dc->u_x = ( dc->V_x/ ( dc->w_x2-dc->w_x1 ) ) ;
      dc->u_y = ( dc->V_y/ ( dc->w_y2-dc->w_y1 ) ) ;
      dc->v_r = sqrt ( ( double ) t ) ;
      dc->lcur_x = dc->D_x;
      dc->lcur_y = dc->D_y;
      dc->rcur_x = dc->D_x+dc->V_x;
      dc->rcur_y = dc->D_y+dc->V_y;
//   Dempty(wc->Clip);
//   wc->Clip=Dopen();
//   uiset_sup_clip_limits(wc,(dc->v_x1+dc->D_x),(dc->v_y1+dc->D_y),(dc->v_x2+dc->D_x),(dc->v_y2+dc->D_y));
//   uiset_clip_limits(wc,(dc->v_x1+dc->D_x),(dc->v_y1+dc->D_y),(dc->v_x2+dc->D_x),(dc->v_y2+dc->D_y));
// uiset_sup_clip_limits(wc,(dc->v_x1+dc->D_x),D->evgay-(dc->v_y2+dc->D_y),(dc->v_x2+dc->D_x),D->evgay-(dc->v_y1+dc->D_y));
// uiset_clip_limits(wc,(dc->v_x1+dc->D_x),D->evgay-(dc->v_y2+dc->D_y),(dc->v_x2+dc->D_x),D->evgay-(dc->v_y1+dc->D_y));
      uiset_sup_clip_limits ( wc , ( dc->D_x ) , D->evgay- ( dc->V_y+dc->D_y ) , \
       ( dc->V_x+dc->D_x ) , D->evgay- ( dc->D_y ) ) ;
      uiset_clip_limits ( wc , ( dc->D_x ) , D->evgay- ( dc->V_y+dc->D_y ) , \
       ( dc->V_x+dc->D_x ) , D->evgay- ( dc->D_y ) ) ;
//  uiset_sup_clip_limits(wc,(dc->D_x),(dc->D_y),(dc->V_x+dc->D_x),(dc->V_y+dc->D_y));
//  uiset_clip_limits(wc,(dc->D_x),(dc->D_y),(dc->V_x+dc->D_x),(dc->V_y+dc->D_y));
  }
  void ui_vu_port ( DIG *G , float x1 , float y1 , float x2 , float y2 ) {
      float t;
      int EVGAX , EVGAY;
      DIALOG *D , *Parent;
      kgDC *dc;
      kgWC *wc;
      D = ( G->D ) ;
      if ( D == NULL ) return;
      Parent = ( G->D ) ;
      wc = G->wc;
      dc = G->dc;
      dc->vu_x1 = x1 , dc->vu_y1 = y1 , dc->vu_x2 = x2 , dc->vu_y2 = y2;
      EVGAX = G->x2-G->x1+1;
      EVGAY = abs ( G->y2-G->y1 ) +1;
      dc->v_x1 = x1* ( float ) EVGAX;
      dc->v_y1 = y1* ( float ) EVGAY;
      dc->v_x2 = x2* ( float ) EVGAX;
      dc->v_y2 = y2* ( float ) EVGAY;
//   if( v_y2 <= 0) {setnormal();exit(0);}
      if ( dc->v_y2 <= 0 ) {printf ( "dc->v_y2<=0:%d\n" , dc->v_y2 ) ;
      exit ( 0 ) ; }
      dc->D_x = G->x1+D->xo;
//   dc->D_y=D->yl-G->y2+D->evgay-(D->yo+D->yl);
      dc->D_y = G->y2+D->yo;
      dc->D_y = D->evgay - dc->D_y;
      dc->V_x = dc->v_x2-dc->v_x1;
      dc->V_y = dc->v_y2-dc->v_y1;
      t = ( dc->V_x*dc->V_x+dc->V_y*dc->V_y ) ;
      dc->u_x = ( dc->V_x/ ( dc->w_x2-dc->w_x1 ) ) ;
      dc->u_y = ( dc->V_y/ ( dc->w_y2-dc->w_y1 ) ) ;
      dc->v_r = sqrt ( ( double ) t ) ;
      dc->lcur_x = dc->D_x;
      dc->lcur_y = dc->D_y;
      dc->rcur_x = dc->D_x+dc->V_x;
      dc->rcur_y = dc->D_y+dc->V_y;
      uiset_sup_clip_limits ( wc , ( dc->v_x1+dc->D_x ) , ( dc->v_y1+dc->D_y ) , \
       ( dc->v_x2+dc->D_x ) , ( dc->v_y2+dc->D_y ) ) ;
      uiset_clip_limits ( wc , ( dc->v_x1+dc->D_x ) , ( dc->v_y1+dc->D_y ) , \
       ( dc->v_x2+dc->D_x ) , ( dc->v_y2+dc->D_y ) ) ;
  }
  void ui_window ( DIG *G , float x1 , float y1 , float x2 , float y2 ) {
      float r_d;
      kgDC *dc;
      dc = ( kgDC * ) G->dc;
      dc->w_x1 = x1;
      dc->w_y1 = y1;
      dc->w_x2 = x2;
      dc->w_y2 = y2;
      dc->u_x = ( dc->V_x/ ( x2-x1 ) ) ;
      dc->u_y = ( dc->V_y/ ( y2-y1 ) ) ;
      r_d = ( ( ( x2-x1 ) * ( x2-x1 ) + ( y2-y1 ) * ( y2-y1 ) ) ) ;
      dc->r_d = sqrt ( ( double ) r_d ) ;
      dc->txt_ht = ( dc->w_y2-dc->w_y1 ) *0.03;
      dc->txt_wt = ( dc->w_x2-dc->w_x1 ) *0.03;
      dc->txt_sp = ( dc->w_x2-dc->w_x1 ) *0.003;
      dc->txt_htx = dc->txt_ht;
      dc->txt_hty = dc->txt_ht;
      dc->txt_wtx = dc->txt_wt;
      dc->txt_wty = dc->txt_wt;
      dc->txt_spx = dc->txt_sp;
      dc->txt_spy = dc->txt_sp;
      if ( ( G->D != NULL ) ) ui_vu_port ( G , dc->vu_x1 , \
           dc->vu_y1 , dc->vu_x2 , dc->vu_y2 ) ;
      if ( G->D_ON == 0 ) img_viewport ( G , dc->vu_x1 , \
           dc->vu_y1 , dc->vu_x2 , dc->vu_y2 ) ;
  }
  void ui_lnwidth ( DIG *G , int width ) {
      kgDC *dc;
      dc = G->dc;
      dc->ln_width = width;
  }
  void ui_move ( DIG *G , float x , float y ) {
      int x1 , y1;
      kgDC *dc;
      dc = G->dc;
      x1 = uiscr_x ( x ) ;
      y1 = uiscr_y ( y ) ;
      _uimove ( G , x1 , y1 ) ;
  }
  void ui_draw ( DIG *G , float ix , float iy ) {
      int x1 , y1;
      kgDC *dc;
      dc = G->dc;
      x1 = uiscr_x ( ix ) ;
      y1 = uiscr_y ( iy ) ;
      uiset_clr ( G->D , dc->ln_color ) ;
      _uidraw ( G , x1 , y1 ) ;
  }
  void ui_drawimage_org ( DIG *G , void *imgfile , \
       float x1 , float y1 , float x2 , float y2 ) \
  {
      float fac;
      GMIMG *img , *rzimg;
      char *pt;
      int IMG = 1 , iw , ih , w , h , temp , wo , ho;
      int X1 , Y1 , X2 , Y2 , EVGAY , X0 , Y0;
      DIALOG *D;
      kgDC *dc;
      kgWC *wc;
      if ( imgfile == NULL ) return;
      dc = G->dc;
      wc = G->wc;
      D = G->D;
      EVGAY = D->evgay;
      X1 = uiscr_x ( x1 ) ;
      X2 = uiscr_x ( x2 ) ;
      if ( X1 > X2 ) {temp = X2; X2 = X1; X1 = temp; }
      if ( X1 > wc->c_v_x2 ) return;
      if ( X2 < wc->c_v_x1 ) return;
      Y1 = uiscr_y ( y1 ) ;
      Y2 = uiscr_y ( y2 ) ;
      if ( Y1 > Y2 ) {temp = Y2; Y2 = Y1; Y1 = temp; }
      if ( Y1 > wc->c_v_y2 ) return;
      if ( Y2 < wc->c_v_y1 ) return;
      pt = ( char * ) imgfile;
      if ( ( pt [ 0 ] == '#' ) && ( pt [ 1 ] == '#' ) ) {
          IMG = 0;
          img = uiGetImage ( ( void * ) ( pt+2 ) ) ;
      }
      else img = imgfile;
      if ( img == NULL ) return;
      w = X2-X1+1;
      h = Y2 -Y1+1;
      iw = img->image_width;
      ih = img->image_height;
      rzimg = ( GMIMG * ) kgChangeSizeImage ( img , w , h ) ;
//  rzimg= (GMIMG *)uiThumbnailgmImage(img,w,h);
      X0 = X1+w*0.5;
      Y0 = EVGAY-Y2+h*0.5;
      wo = w;
      ho = h;
      if ( X2 > wc->c_v_x2 ) X2 = wc->c_v_x2;
      if ( X1 < wc->c_v_x1 ) X1 = wc->c_v_x1;
      if ( Y2 > wc->c_v_y2 ) Y2 = wc->c_v_y2;
      if ( Y1 < wc->c_v_y1 ) Y1 = wc->c_v_y1;
      temp = EVGAY-Y1;
      Y1 = EVGAY -Y2;
      Y2 = temp;
      w = X2-X1+1;
      h = Y2 -Y1+1;
      rzimg->xoffset = + ( X1+w/2 -X0 ) *2;
      rzimg->yoffset = + ( Y1+h/2 -Y0 ) *2;
//  printf("Getting rzimg %d %d %d %d\n",rzimg->image_width,rzimg->image_height,rzimg->xoffset,rzimg->yoffset);
//  uiWriteImage(rzimg,"Junk.png");
      kgImage ( D , rzimg , X1 , Y1 , w , h , 0.0 , 1.0 ) ;
      uiFreeImage ( rzimg ) ;
      if ( ! IMG ) uiFreeImage ( img ) ;
  }
  void ui_drawimage ( DIG *G , void *imgfile , float x1 , \
       float y1 , float x2 , float y2 ) \
  {
      float fac;
      GMIMG *img , *rzimg;
      char *pt;
      int IMG = 1 , iw , ih , w , h , temp , wo , ho;
      int X1 , Y1 , X2 , Y2 , EVGAY , X0 , Y0;
      int xl , yl , xu , yu;
      float lper , rper , tper , bper;
      DIALOG *D;
      kgDC *dc;
      kgWC *wc;
      if ( imgfile == NULL ) return;
      dc = G->dc;
      wc = G->wc;
      D = G->D;
      EVGAY = D->evgay;
      X1 = uiscr_x ( x1 ) ;
      X2 = uiscr_x ( x2 ) ;
      if ( X1 > X2 ) {temp = X2; X2 = X1; X1 = temp; }
      if ( X1 > wc->c_v_x2 ) return;
      if ( X2 < wc->c_v_x1 ) return;
      Y1 = uiscr_y ( y1 ) ;
      Y2 = uiscr_y ( y2 ) ;
      if ( Y1 > Y2 ) {temp = Y2; Y2 = Y1; Y1 = temp; }
      if ( Y1 > wc->c_v_y2 ) return;
      if ( Y2 < wc->c_v_y1 ) return;
      xl = X1 , yl = Y1 , xu = X2 , yu = Y2;
      if ( xl < wc->c_v_x1 ) xl = wc->c_v_x1;
      if ( xu < wc->c_v_x1 ) xu = wc->c_v_x1;
      if ( xl > wc->c_v_x2 ) xl = wc->c_v_x2;
      if ( xu > wc->c_v_x2 ) xu = wc->c_v_x2;
      if ( yl < wc->c_v_y1 ) yl = wc->c_v_y1;
      if ( yu < wc->c_v_y1 ) yu = wc->c_v_y1;
      if ( yl > wc->c_v_y2 ) yl = wc->c_v_y2;
      if ( yu > wc->c_v_y2 ) yu = wc->c_v_y2;
      w = xu -xl+1;
      h = yu -yl+1;
      if ( ( w <= 0 ) || ( h <= 0 ) ) return;
      lper = ( xl-X1 ) / ( float ) ( X2-X1 ) ;
      rper = ( X2-xu ) / ( float ) ( X2-X1 ) ;
      tper = ( yl-Y1 ) / ( float ) ( Y2-Y1 ) ;
      bper = ( Y2-yu ) / ( float ) ( Y2-Y1 ) ;
      pt = ( char * ) imgfile;
      if ( ( pt [ 0 ] == '#' ) && ( pt [ 1 ] == '#' ) ) {
          IMG = 0;
          img = uiGetImage ( ( void * ) ( pt+2 ) ) ;
      }
      else img = imgfile;
      if ( img == NULL ) return;
      iw = img->image_width;
      ih = img->image_height;
      rzimg = ( GMIMG * ) kgChangeSizeImage ( img , ( X2-X1 ) , ( Y2-Y1 ) ) ;
      if ( IMG == 0 ) {kgFreeImage ( img ) ; IMG = 1; }
      img = rzimg;
      iw = img->image_width;
      ih = img->image_height;
      if ( ( lper != 0 ) || ( rper != 0 ) || ( tper != 0 ) || ( bper != 0 ) ) {
          int l , r , b , t;
          l = iw*lper+1; r = iw* ( 1-rper ) ;
          b = ih*bper+1; t = ih* ( 1-tper ) ;
          rzimg = kgCropImage ( img , l , b , r , t ) ;
          uiFreeImage ( img ) ;
          IMG = 0;
          img = rzimg;
      }
      temp = EVGAY-yl;
      Y1 = EVGAY -yu;
      Y2 = temp;
      X1 = xl;
      X2 = xu;
      w = X2-X1+1;
      h = Y2 -Y1+1;
      kgImage ( D , img , X1 , Y1 , img->image_width , \
           img->image_height , 0.0 , 1.0 ) ;
      if ( ! IMG ) uiFreeImage ( img ) ;
  }
  void ui_ln_style ( DIG *G , int istl ) {
      kgDC *dc;
      dc = G->dc;
      dc->ln_style = istl;
  }
  void ui_ln_clr ( DIG *G , int color ) {
      kgDC *dc;
      dc = G->dc;
      dc->ln_color = color;
      dc->c_color = color;
      uiset_clr ( G->D , color ) ;
  }
  void ui_txt_clr ( DIG *G , int color ) {
      kgDC *dc;
      dc = G->dc;
      dc->t_color = color;
  }
  void ui_txt_size ( DIG *G , float ht , float wt , float sp , \
       float htx , float wty , float spy ) \
  {
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      dc->txt_hty = ht;
      dc->txt_wtx = wt;
      dc->txt_spx = sp;
      dc->txt_htx = htx;
      dc->txt_wty = wty;
      dc->txt_spy = spy;
      dc->txt_wt = dc->sint*dc->sint*dc->txt_wty* ( dc->w_x2-dc->w_x1 ) / \
       ( dc->w_y2-dc->w_y1 ) +dc->cost*dc->cost*dc->txt_wtx;
      dc->txt_sp = dc->sint*dc->sint*dc->txt_spy* ( dc->w_x2-dc->w_x1 ) / \
       ( dc->w_y2-dc->w_y1 ) +dc->cost*dc->cost*dc->txt_spx;
      dc->txt_ht = dc->sint*dc->sint*dc->txt_htx/ ( dc->w_x2-dc->w_x1 ) * \
       ( dc->w_y2-dc->w_y1 ) +dc->cost*dc->cost*dc->txt_hty;
      dc->txt_ht = dc->txt_ht*dc->V_y/ ( dc->w_y2-dc->w_y1 ) ;
      dc->txt_wt = dc->txt_wt*dc->V_x/ ( dc->w_x2-dc->w_x1 ) ;
      dc->txt_sp = dc->txt_sp*dc->V_x/ ( dc->w_x2-dc->w_x1 ) ;
      dc->v_xt1 = ( int ) ( dc->v_x1-dc->txt_wt-dc->txt_sp ) ;
      dc->v_yt1 = ( int ) ( dc->v_y1-dc->txt_ht ) ;
      dc->txt_w42 = dc->txt_wt/CFact;
      dc->txt_h42 = dc->txt_ht/CFact;
      return;
  }
  void ui_txt_rot ( DIG *G , float t ) {
      kgDC *dc;
      dc = G->dc;
      dc->trot = t;
      dc->theta = -t*rad;
      dc->cost = cos ( dc->theta ) ;
      dc->sint = sin ( dc->theta ) ;
      dc->txt_wt = dc->sint*dc->sint*dc->txt_wty* ( dc->w_x2-dc->w_x1 ) / \
       ( dc->w_y2-dc->w_y1 ) +dc->cost*dc->cost*dc->txt_wtx;
      dc->txt_sp = dc->sint*dc->sint*dc->txt_spy* ( dc->w_x2-dc->w_x1 ) / \
       ( dc->w_y2-dc->w_y1 ) +dc->cost*dc->cost*dc->txt_spx;
      dc->txt_ht = dc->sint*dc->sint*dc->txt_htx/ ( dc->w_x2-dc->w_x1 ) * \
       ( dc->w_y2-dc->w_y1 ) +dc->cost*dc->cost*dc->txt_hty;
      dc->txt_ht = dc->txt_ht*dc->V_y/ ( dc->w_y2-dc->w_y1 ) ;
      dc->txt_wt = dc->txt_wt*dc->V_x/ ( dc->w_x2-dc->w_x1 ) ;
      dc->txt_sp = dc->txt_sp*dc->V_x/ ( dc->w_x2-dc->w_x1 ) ;
      dc->v_xt1 = ( int ) ( dc->v_x1-dc->txt_wt-dc->txt_sp ) ;
      dc->v_yt1 = ( int ) ( dc->v_y1-dc->txt_ht ) ;
      dc->txt_w42 = dc->txt_wt/CFact;
      dc->txt_h42 = dc->txt_ht/CFact;
  }
  void ui_txt_wr ( DIG *G , int n , char *txt ) {
      short i = 0 , bold , tempc , ishft , trot , Nu , De , gap , lnwidth_o , j;
      int font_o;
      float fact , val , xl1 , xl2 , hfact = 1.0 , slant;
      int txt_bold_o;
      L_N *FO_L = NULL , *pt = NULL;
      B_K *FB_P = NULL;
      char ch , cntl;
      unsigned char *tx;
      float Slnt [ 2 ] = {0.0 , 0.25} , Slant_o; ;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      tx = ( unsigned char * ) txt;
      j = strlen ( tx ) ;
      dc->O_L = NULL;
      bold = dc->txt_bold;
      slant = 0;
      font_o = dc->t_font;
      dc->trot = ( dc->cost < 0.99 ) ;
      tempc = wc->c_color;
      wcset_clr ( wc , dc->t_color ) ;
      dc->cx = ( int ) ( dc->cur_x+0.5 ) ;
      dc->cy = ( int ) ( dc->cur_y+0.5 ) ;
      dc->xp = 0.0;
      dc->yp = 0.0;
      fact = 1.0;
      hfact = 1.0;
      ishft = 0;
      dc->greek = 0;
      lnwidth_o = dc->ln_width;
      dc->ln_width = 1;
      while ( txt [ i ] != '\0' ) {
          {
              if ( txt [ i ] != '!' ) { if ( dc->trot ) uidrawgch ( G , txt [ i ] ) ;
                  else uidrawg0ch ( G , txt [ i ] ) ;
                  dc->greek = 0;
              }
              else {
                  i++;
                  if ( txt [ i ] == '\0' ) break;
                  cntl = txt [ i ] ;
                  if ( ( cntl == 'S' ) || ( cntl == 's' ) ) uisetsubsup \
                   ( G , & fact , & ishft , cntl ) ;
                  else {
                      switch ( cntl ) {
                          case 'e':
                          uiresetsubsup ( G , & fact , & ishft ) ;
                          break;
                          case '!':
                          dc->greek = 0;
                          if ( dc->trot ) uidrawgch ( G , txt [ i ] ) ;
                          else uidrawg0ch ( G , txt [ i ] ) ;
                          break;
                          case 'b':
                          dc->xp = dc->xp -dc->txt_wt -dc->txt_sp;
                          break;
                          case 'B':
                          dc->txt_bold = 2;
                          break;
                          case 'I':
                          dc->Slant = Slnt [ 1 ] ;
                          break;
                          case 'N':
                          dc->Slant = Slant_o; dc->txt_bold = txt_bold_o;
                          if ( pt != NULL ) {
                              while ( ( pt->x2 ) >= 0. ) pt = pt->Pr;
                               ( pt->x2 ) = dc->xp-dc->txt_sp;
                              pt = pt->Pr;
                          }
                          break;
                          case 'g':
                          dc->greek = 128;
                          break;
                          case 'r':
                          if ( dc->O_P != NULL ) {
                              dc->xp = dc->O_P->x;
                              dc->yp = dc->O_P->y;
                              dc->D_P = dc->O_P;
                              dc->O_P = dc->O_P->Pr;
                              free ( dc->D_P ) ;
                              if ( dc->O_P == NULL ) FB_P = NULL;
                          }
                          break;
                          case 'k':
                          if ( FB_P == NULL ) {
                              FB_P = ( B_K * ) Malloc ( ( int ) sizeof ( B_K ) ) ;
                              dc->O_P = FB_P;
                              dc->O_P->Nx = NULL; dc->O_P->Pr = NULL;
                          }
                          else {
                              dc->O_P->Nx = ( B_K * ) Malloc ( ( int ) sizeof ( B_K ) ) ;
                              dc->O_P->Nx->Pr = dc->O_P;
                              dc->O_P = dc->O_P->Nx;
                              dc->O_P->Nx = NULL;
                          }
                           ( dc->O_P->x ) = dc->xp;
                           ( dc->O_P->y ) = dc->yp;
                          break;
                          case 'x':
                          dc->yp += 0.2*dc->txt_ht;
                          break;
                          case 'y':
                          dc->yp -= 0.2*dc->txt_ht;
                          break;
                          case 'u':
                          dc->yp += 0.9*dc->txt_ht;
                          break;
                          case 'd':
                          dc->yp -= 0.9*dc->txt_ht;
                          break;
                          case 'O':
                          case 'U':
                          if ( FO_L == NULL ) {
                              FO_L = ( L_N * ) Malloc ( ( int ) sizeof ( L_N ) ) ;
                              dc->O_L = FO_L;
                              dc->O_L->Nx = NULL; dc->O_L->Pr = NULL;
                          }
                          else {
                              dc->O_L->Nx = ( L_N * ) Malloc ( ( int ) sizeof ( L_N ) ) ;
                              dc->O_L->Nx->Pr = dc->O_L;
                              dc->O_L = dc->O_L->Nx;
                              dc->O_L->Nx = NULL;
                          }
                           ( dc->O_L->x1 ) = dc->xp;
                           ( dc->O_L->x2 ) = -1.0;
                          dc->O_L->ymax = dc->yp+1.4*dc->txt_ht;
                          dc->O_L->ymin = dc->yp-0.4*dc->txt_ht;
                          dc->O_L->p = cntl;
                          pt = dc->O_L;
                          break;
                          case '%':
                          if ( i+2 >= j ) break;
                          ch = txt [ i+1 ] ; txt [ i+1 ] = '\0';
                          gap = uistrlngth ( G , txt , & xl2 ) ;
                          txt [ i+1 ] = ch;
                          dc->xp = ( gap+1 ) * ( dc->txt_wt+dc->txt_sp ) ;
                          break;
                          case 'z':
                          if ( i+2 >= j ) break;
                          Nu = ( txt [ i+1 ] -'0' ) ;
                          De = ( txt [ i+2 ] -'0' ) ;
                          if ( De == 0 ) De = 1;
                          val = ( float ) Nu/ ( float ) De;
                          fact = fact*val;
                          hfact = hfact*val;
                          i += 2;
                          dc->txt_w42 = dc->txt_w42*val;
                          dc->txt_h42 = dc->txt_h42*val;
                          dc->txt_wt = dc->txt_wt*val;
                          dc->txt_ht = dc->txt_ht*val;
                          break;
                          case 'f':
                          if ( i+2 >= j ) break;
                          Nu = ( txt [ i+1 ] -'0' ) *10+ ( txt [ i+2 ] -'0' ) ;
                          ui_txt_font ( G , ( int ) Nu ) ;
                          i+= 2;
                          break;
                          case 'c':
                          if ( i+2 >= j ) break;
                          Nu = ( txt [ i+1 ] -'0' ) *10+ ( txt [ i+2 ] -'0' ) ;
//                            wcset_clr(wc,Nu);
                          ui_txt_clr ( G , ( int ) Nu ) ;
                          i+= 2;
                          break;
                          case 'h':
                          if ( i+2 >= j ) break;
                          Nu = ( txt [ i+1 ] -'0' ) ;
                          De = ( txt [ i+2 ] -'0' ) ;
                          if ( De == 0 ) De = 1;
                          val = ( float ) Nu/ ( float ) De;
                          hfact = hfact*val;
                          i += 2;
                          dc->txt_h42 = dc->txt_h42*val;
                          dc->txt_ht = dc->txt_ht*val;
                          break;
                          case 'w':
                          if ( i+2 >= j ) break;
                          Nu = ( txt [ i+1 ] -'0' ) ;
                          De = ( txt [ i+2 ] -'0' ) ;
                          if ( De == 0 ) De = 1;
                          val = ( float ) Nu/ ( float ) De;
                          fact = fact*val;
                          i += 2;
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
      dc->ln_width = 2;
      wcset_clr ( wc , tempc ) ;
      pt = FO_L;
      while ( pt != NULL ) {
          if ( pt->x2 < 0. ) pt->x2 = dc->xp;
          if ( pt->p == 'U' ) {
              _uimove ( G , uiTX ( ( pt->x1 ) , pt->ymin ) , uiTY \
               ( ( pt->x1 ) , pt->ymin ) ) ;
              _uidraw ( G , uiTX ( ( pt->x2 ) , pt->ymin ) , uiTY \
               ( ( pt->x2 ) , pt->ymin ) ) ;
          }
          else{
              _uimove ( G , uiTX ( ( pt->x1 ) , pt->ymax ) , uiTY \
               ( ( pt->x1 ) , pt->ymax ) ) ;
              _uidraw ( G , uiTX ( ( pt->x2 ) , pt->ymax ) , uiTY \
               ( ( pt->x2 ) , pt->ymax ) ) ;
          }
          dc->O_L = pt;
          pt = pt->Nx;
          free ( dc->O_L ) ;
      }
      dc->O_P = FB_P;
      while ( dc->O_P != NULL ) {
          dc->D_P = dc->O_P;
          dc->O_P = dc->O_P->Pr;
          free ( dc->D_P ) ;
      }
      dc->ln_width = lnwidth_o;
      if ( dc->t_font != font_o ) ui_txt_font ( G , font_o ) ;
  }
  void ui_clr_vu ( DIG *G ) {
      kgDC *dc;
      DIALOG *D;
      D = G->D;
      dc = G->dc;
      if ( G->D_ON == 0 ) {
          GMIMG *png;
          uiCleangmImage ( G->img ) ;
          ui_vu_port ( G , dc->vu_x1 , dc->vu_y1 , dc->vu_x2 , dc->vu_y2 ) ;
          _uibox_fill ( G->wc , dc->D_x+dc->v_x1 , dc->D_y+dc->v_y1 , \
               dc->v_x2+dc->D_x , dc->v_y2+dc->D_y , 0 ) ;
          img_viewport ( G , dc->vu_x1 , dc->vu_y1 , dc->vu_x2 , dc->vu_y2 ) ;
      }
      else _uibox_fill ( G->wc , dc->D_x+dc->v_x1 , dc->D_y+dc->v_y1 , \
           dc->v_x2+dc->D_x , dc->v_y2+dc->D_y , 0 ) ;
  }
  void ui_panel ( DIG *G , float *x , float *y , \
       int color , int flag , int n ) \
  {
      int *x1 , *y1;
      int xo , yo , xv , yv;
      short i , j = 0;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      x1 = ( int * ) Malloc ( sizeof ( int ) * ( n+1 ) ) ;
      y1 = ( int * ) Malloc ( sizeof ( int ) * ( n+1 ) ) ;
      if ( ( y1 == NULL ) ) {
          normal ( ) ;
          printf ( " Error: Not enough buffer for polyfill..\n" ) ;
          exit ( 0 ) ;
      }
      x1 [ 0 ] = uiscr_x ( x [ 0 ] ) ;
      y1 [ 0 ] = uiscr_y ( y [ 0 ] ) ;
      j++; xo = x1 [ 0 ] , yo = y1 [ 0 ] ;
      for ( i = 1; i < n; i++ ) {
          xv = uiscr_x ( x [ i ] ) ;
          yv = uiscr_y ( y [ i ] ) ;
          if ( ( xo != xv ) || ( yo != yv ) ) {
              x1 [ j ] = xv;
              y1 [ j ] = yv;
              xo = xv , yo = yv; j++;
          }
      }
      if ( ( j > 2 ) && ( j < 500 ) ) _uipoly_fill ( G , j , x1 , y1 , flag , \
       ( unsigned int ) color ) ;
      free ( x1 ) , free ( y1 ) ;
  }
  void ui_box_fill ( DIG *G , float x1 , float y1 , \
       float x2 , float y2 , int color , int ib ) \
  {
      int xa , ya , xb , yb;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      xa = uiscr_x ( x1 ) ;
      ya = uiscr_y ( y1 ) ;
      xb = uiscr_x ( x2 ) ;
      yb = uiscr_y ( y2 ) ;
      _uibox_fill ( wc , xa , ya , xb , yb , color ) ;
  }
  void ui_set_godr ( DIG *G , float p2 , float p1 , \
       int sclr , int n , int ib ) \
  {
      kgDC *dc;
      dc = G->dc;
      dc->pmax = p2;
      dc->pmin = p1;
      dc->min_clr = sclr;
      dc->max_clr = n;
      dc->ibdr = ib;
      dc->dfac = ( n -1 ) / ( dc->pmax-dc->pmin ) ;
//  printf("%f %f %f\n",dc->pmax,dc->pmin,dc->dfac);
  }
  void ui_godr_fill ( DIG *G , int n , float *x , float *y , float *p ) {
      int x1 [ 100 ] , y1 [ 100 ] ;
      float p1 [ 100 ] , pv;
      short i;
      int xmin = 10000000 , xmax = -10000000 , ymin = 10000000 , ymax = -10000000;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      for ( i = 0; i < n; i++ ) {
          x1 [ i ] = uiscr_x ( x [ i ] ) ;
          y1 [ i ] = uiscr_y ( y [ i ] ) ;
          if ( xmin > x1 [ i ] ) xmin = x1 [ i ] ;
          if ( ymin > y1 [ i ] ) ymin = y1 [ i ] ;
          if ( xmax < x1 [ i ] ) xmax = x1 [ i ] ;
          if ( ymax < y1 [ i ] ) ymax = y1 [ i ] ;
      }
      if ( ( xmin == xmax ) || ( ymin == ymax ) ) return;
      if ( ( xmax < dc->v_x1 ) || ( xmin > dc->v_x2 ) || ( ymax < dc->v_y1 ) \
       || ( ymin > dc->v_y2 ) ) return;
      for ( i = 0; i < n; i++ ) {pv = * ( p++ ) , p1 [ i ] = uipclr ( pv ) ; }
      _uigodard_fill ( G , n , x1 , y1 , dc->ibdr , p1 ) ;
  }
  void ui_marker ( DIG *G , float x , float y ) {
      int x1 , y1;
      kgWC *wc;
      kgDC *dc;
      wc = G->wc;
      dc = G->dc;
      wcset_clr ( wc , dc->ln_color ) ;
      x1 = uiscr_x ( x ) ;
      y1 = uiscr_y ( y ) ;
      _uimarker ( G , x1 , y1 ) ;
  }
  void ui_smmark ( DIG *G , int istl ) {
      kgDC *dc;
      dc = G->dc;
      dc->m_style = istl;
  }
  void wipe_space ( char *ptr , int ln ) {
      int i;
      i = ln-1;
      * ( ptr+i ) = '\0'; i--;
      while ( ( i >= 0 ) && ( * ( ptr+i ) <= ' ' ) ) {* ( ptr+i ) = '\0'; i--; }
  }
#if 0
  void Scrn_back ( short ix , short iy , short n ) {
      int x2 , y , y2;
      x2 = ix+n*9+10;
      scr_back ( ( int ) ix , ( int ) ( iy ) , x2 , ( int ) ( 22+iy ) ) ;
  }
#endif
  void uiScrn_back ( kgWC *wc , short ix , short iy , short n ) {
      int x2 , y , y2;
      x2 = ix+n*9+10;
      kg_scr_back ( wc , ( int ) ix , ( int ) ( iy ) , x2 , ( int ) ( 22+iy ) ) ;
  }
/*
void transch(int c) {
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
*/
  void * Loadfontstruct ( void ) {
      FONT *pt;
      Dlink *Fontlist = NULL;
      if ( Fontlist == NULL ) {
          Fontlist = Dopen ( ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf0;
          pt->m_f = m_f0;
          pt->icxv = icxvf0;
          pt->icyv = icyvf0;
          pt->fontname = fontname0;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf1; pt->m_f = m_f1; pt->icxv = icxvf1;
          pt->icyv = icyvf1; pt->fontname = fontname1;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf2; pt->m_f = m_f2; pt->icxv = icxvf2;
          pt->icyv = icyvf2; pt->fontname = fontname2;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf3; pt->m_f = m_f3; pt->icxv = icxvf3;
          pt->icyv = icyvf3; pt->fontname = fontname3;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf4; pt->m_f = m_f4; pt->icxv = icxvf4;
          pt->icyv = icyvf4; pt->fontname = fontname4;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf5; pt->m_f = m_f5; pt->icxv = icxvf5;
          pt->icyv = icyvf5; pt->fontname = fontname5;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf6; pt->m_f = m_f6; pt->icxv = icxvf6;
          pt->icyv = icyvf6; pt->fontname = fontname6;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf7; pt->m_f = m_f7; pt->icxv = icxvf7;
          pt->icyv = icyvf7; pt->fontname = fontname7;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf8; pt->m_f = m_f8; pt->icxv = icxvf8;
          pt->icyv = icyvf8; pt->fontname = fontname8;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf9; pt->m_f = m_f9; pt->icxv = icxvf9;
          pt->icyv = icyvf9; pt->fontname = fontname9;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf10; pt->m_f = m_f10; pt->icxv = icxvf10;
          pt->icyv = icyvf10; pt->fontname = fontname10;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf11; pt->m_f = m_f11; pt->icxv = icxvf11;
          pt->icyv = icyvf11; pt->fontname = fontname11;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf12; pt->m_f = m_f12; pt->icxv = icxvf12;
          pt->icyv = icyvf12; pt->fontname = fontname12;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf13; pt->m_f = m_f13; pt->icxv = icxvf13;
          pt->icyv = icyvf13; pt->fontname = fontname13;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf14; pt->m_f = m_f14; pt->icxv = icxvf14;
          pt->icyv = icyvf14; pt->fontname = fontname14;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf15; pt->m_f = m_f15; pt->icxv = icxvf15;
          pt->icyv = icyvf15; pt->fontname = fontname15;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf16; pt->m_f = m_f16; pt->icxv = icxvf16;
          pt->icyv = icyvf16; pt->fontname = fontname16;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf17; pt->m_f = m_f17; pt->icxv = icxvf17;
          pt->icyv = icyvf17; pt->fontname = fontname17;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf18; pt->m_f = m_f18; pt->icxv = icxvf18;
          pt->icyv = icyvf18; pt->fontname = fontname18;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf19; pt->m_f = m_f19; pt->icxv = icxvf19;
          pt->icyv = icyvf19; pt->fontname = fontname19;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf20; pt->m_f = m_f20; pt->icxv = icxvf20;
          pt->icyv = icyvf20; pt->fontname = fontname20;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf21; pt->m_f = m_f21; pt->icxv = icxvf21;
          pt->icyv = icyvf21; pt->fontname = fontname21;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf22; pt->m_f = m_f22; pt->icxv = icxvf22;
          pt->icyv = icyvf22; pt->fontname = fontname22;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf23; pt->m_f = m_f23; pt->icxv = icxvf23;
          pt->icyv = icyvf23; pt->fontname = fontname23;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf24; pt->m_f = m_f24; pt->icxv = icxvf24;
          pt->icyv = icyvf24; pt->fontname = fontname24;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf25; pt->m_f = m_f25; pt->icxv = icxvf25;
          pt->icyv = icyvf25; pt->fontname = fontname25;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf26; pt->m_f = m_f26; pt->icxv = icxvf26;
          pt->icyv = icyvf26; pt->fontname = fontname26;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf27; pt->m_f = m_f27; pt->icxv = icxvf27;
          pt->icyv = icyvf27; pt->fontname = fontname27;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf28; pt->m_f = m_f28; pt->icxv = icxvf28;
          pt->icyv = icyvf28; pt->fontname = fontname28;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf29; pt->m_f = m_f29; pt->icxv = icxvf29;
          pt->icyv = icyvf29; pt->fontname = fontname29;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf30; pt->m_f = m_f30; pt->icxv = icxvf30;
          pt->icyv = icyvf30; pt->fontname = fontname30;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf31; pt->m_f = m_f31; pt->icxv = icxvf31;
          pt->icyv = icyvf31; pt->fontname = fontname31;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf32; pt->m_f = m_f32; pt->icxv = icxvf32;
          pt->icyv = icyvf32; pt->fontname = fontname32;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf33; pt->m_f = m_f33; pt->icxv = icxvf33;
          pt->icyv = icyvf33; pt->fontname = fontname33;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf34; pt->m_f = m_f34; pt->icxv = icxvf34;
          pt->icyv = icyvf34; pt->fontname = fontname34;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf35; pt->m_f = m_f35; pt->icxv = icxvf35;
          pt->icyv = icyvf35; pt->fontname = fontname35;
          Dadd ( Fontlist , pt ) ;
          pt = ( FONT * ) Malloc ( sizeof ( FONT ) ) ;
          pt->icpos = icposf36; pt->m_f = m_f36; pt->icxv = icxvf36;
          pt->icyv = icyvf36; pt->fontname = fontname36;
          Dadd ( Fontlist , pt ) ;
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
      }
      return Fontlist;
  }
  char **t_fontnames ( void ) {
      FONT *pt;
      char **fnames;
      int count , i = 0 , ln;
      Dlink *Fontlist = NULL;
      if ( Fontlist == NULL ) Fontlist = Loadfontstruct ( ) ;
      count = Dcount ( Fontlist ) ;
      fnames = ( char ** ) Malloc ( sizeof ( char * ) * ( count+1 ) ) ;
      fnames [ count ] = NULL;
      Resetlink ( Fontlist ) ;
      while ( ( pt = ( FONT * ) Getrecord ( Fontlist ) ) != NULL ) {
          fnames [ i ] = ( char * ) Malloc ( strlen ( pt->fontname ) +20 ) ;
          sprintf ( fnames [ i ] , "!f18%2.2d !f%-2.2d %-s" , i , i , pt->fontname ) ;
//    fnames[i][strlen(fnames[i])]='\0';
//    printf("%s\n",fnames[i]);
          i++;
      }
      Dempty ( Fontlist ) ;
      Fontlist = NULL;
      return fnames;
  }
  void ui_txt_font ( DIG *G , int font ) {
      FONT *pt;
      int count;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
//  if(Fontlist == NULL ) Fontlist = Loadfontstruct();
      if ( dc->Fontlist == NULL ) dc->Fontlist = Loadfontstruct ( ) ;
      count = Dcount ( dc->Fontlist ) ;
      if ( font >= count ) font = 0;
      Dposition ( dc->Fontlist , font+1 ) ;
      pt = ( FONT * ) Getrecord ( dc->Fontlist ) ;
      dc->icpos = pt->icpos;
      dc->icxv = pt->icxv;
      dc->icyv = pt->icyv;
      dc->m_f = pt->m_f;
      dc->t_font = font;
  }
  void ui_circle ( DIG *G , float x , float y , float r ) {
      int xa , ya , rd;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      xa = uiscr_x ( x ) ;
      ya = uiscr_y ( y ) ;
/*  rd=(int)(r/r_d*v_r);*/
      rd = uiscr_x ( ( x+r ) ) ;
      rd -= xa;
      _uicircle ( wc , xa , ya , rd ) ;
  }
  int uistrlngth ( void *Gtmp , char *title , float *xdsp ) {
      float wd , gp , fj , fjl , gj , val , fact , fact1 = 1.0 , hfact = 1.0;
      short ngp , n , i , j , k , greek = 0;
      int font_o , Nu , De;
      B_K *FB_P = NULL;
      DIG *G;
      kgDC *dc;
      G = ( DIG * ) Gtmp;
      dc = G->dc;
      dc->O_L = NULL;
      dc->greek = 0;
      font_o = dc->t_font;
      wd = dc->txt_wtx;
      gp = dc->txt_spx;
      ngp = 1;
      *xdsp = 0;
      j = 0; while ( title [ j ] != '\0' ) j++;
      if ( j == 0 ) { *xdsp = 0.; return ( 0 ) ; }
      fj = 0.0; gj = -1; fact = 1.0;
      fjl = 0.;
      i = 0;
      while ( i < j ) {
          if ( title [ i ] != '!' ) {
              if ( dc->pr_txt > 0 ) fact1 = dc->m_f [ title [ i ] -32+dc->greek ] *fact;
              else fact1 = 1.0;
              gj += 1.;
              fjl += 1.0;
              fj += fact1; dc->greek = 0;
          }
          else {
              i = i+1;
              switch ( title [ i ] ) {
                  case 's':
                  case 'S':
                  fact = fact*0.6;
                  break;
                  case 'e':
                  fact = fact/0.6;
                  break;
                  case 'b':
                  fj = fj-fact1; fjl-= 1.; gj = gj-1;
                  break;
                  case 'g':
                  dc->greek = 128;
                  break;
                  case 'r':
                  if ( dc->O_P != NULL ) {
                      fj = dc->O_P->x;
                      fjl = dc->O_P->xl;
                      gj = dc->O_P->y;
                      dc->D_P = dc->O_P;
                      dc->O_P = dc->O_P->Pr;
                      free ( dc->D_P ) ;
                      if ( dc->O_P == NULL ) FB_P = NULL;
                  }
                  break;
                  case 'k':
                  if ( FB_P == NULL ) {
                      FB_P = ( B_K * ) Malloc ( ( int ) sizeof ( B_K ) ) ;
                      dc->O_P = FB_P;
                      dc->O_P->Nx = NULL; dc->O_P->Pr = NULL;
                  }
                  else {
                      dc->O_P->Nx = ( B_K * ) Malloc ( ( int ) sizeof ( B_K ) ) ;
                      dc->O_P->Nx->Pr = dc->O_P;
                      dc->O_P = dc->O_P->Nx;
                      dc->O_P->Nx = NULL;
                  }
                   ( dc->O_P->x ) = fj;
                   ( dc->O_P->xl ) = fjl;
                   ( dc->O_P->y ) = gj;
                  break;
                  case '!':
                  fj = fj+fact1; fjl+= 1.; gj = gj+1;
                  break;
                  case '%':
                  fj = fjl;
                  break;
                  case 'f':
                  Nu = ( title [ i+1 ] -'0' ) ;
                  Nu = Nu*10+ ( title [ i+2 ] -'0' ) ;
                  ui_txt_font ( G , ( int ) Nu ) ;
                  i+= 2;
                  break;
                  case 'c':
                  i+= 2;
                  break;
                  case 'z':
                  Nu = ( title [ i+1 ] -'0' ) ;
                  De = ( title [ i+2 ] -'0' ) ;
                  if ( De == 0 ) De = 1;
                  val = ( float ) Nu/ ( float ) De;
                  if ( val == 0.0 ) val = 1.0;
                  fact = fact*val;
                  i = i+2;
                  break;
                  case 'h':
                  Nu = ( title [ i+1 ] -'0' ) ;
                  De = ( title [ i+2 ] -'0' ) ;
                  if ( De == 0 ) De = 1;
                  val = ( float ) Nu/ ( float ) De;
                  if ( val == 0.0 ) val = 1.0;
                  hfact = hfact*val;
                  i = i+2;
                  break;
                  case 'w':
                  Nu = ( title [ i+1 ] -'0' ) ;
                  De = ( title [ i+2 ] -'0' ) ;
                  if ( De == 0 ) De = 1;
                  val = ( float ) Nu/ ( float ) De;
                  if ( val == 0.0 ) val = 1.0;
                  fact = fact*val;
                  i = i+2;
                  break;
                  default :
                  break;
              }
          }
          i = i+1;
      }
      *xdsp = ( fj*wd+gj*gp ) ;
      ngp = gj+0.1;
      dc->O_P = FB_P;
      while ( dc->O_P != NULL ) {
          dc->D_P = dc->O_P;
          dc->O_P = dc->O_P->Pr;
          free ( dc->D_P ) ;
      }
      if ( dc->t_font != font_o ) ui_txt_font ( G , ( int ) font_o ) ;
      return ( ngp ) ;
  }
  int uistrlngth_o ( kgDC *dc , char *title , float *xdsp ) {
      float wd , gp , fj , fjl , gj , val , fact , fact1 = 1.0 , hfact = 1.0;
      short ngp , n , i , j , k , greek = 0;
      int font_o;
      B_K *FB_P = NULL;
      font_o = dc->t_font;
      wd = dc->txt_wtx;
      gp = dc->txt_spx;
      ngp = 1;
      j = 0; while ( title [ j ] != '\0' ) j++;
      if ( j == 0 ) { *xdsp = 0.; return ( 0 ) ; }
      fj = 0.0; gj = -1; fact = 1.0;
      fjl = 0.;
      i = 0;
      while ( i < j ) {
          if ( title [ i ] != '!' ) {
              if ( dc->pr_txt > 0 ) fact1 = dc->m_f [ title [ i ] -32+greek ] *fact;
              else fact1 = 1.0;
              gj += 1.;
              fjl += 1.0;
              fj += fact1; greek = 0;
          }
          else {
              i = i+1;
              if ( i >= j ) break;
              switch ( title [ i ] ) {
                  case 's':
                  case 'S':
                  fact = fact*0.6;
                  break;
                  case 'e':
                  fact = fact/0.6;
                  break;
                  case 'b':
                  fj = fj-fact1; fjl-= 1.; gj = gj-1;
                  break;
                  case 'g':
                  greek = 128;
                  break;
                  case 'r':
                  if ( dc->O_P != NULL ) {
                      fj = dc->O_P->x;
                      fjl = dc->O_P->xl;
                      gj = dc->O_P->y;
                      dc->D_P = dc->O_P;
                      dc->O_P = dc->O_P->Pr;
                      free ( dc->D_P ) ;
                      if ( dc->O_P == NULL ) FB_P = NULL;
                  }
                  break;
                  case 'k':
                  if ( FB_P == NULL ) {
                      FB_P = ( B_K * ) Malloc ( ( int ) sizeof ( B_K ) ) ;
                      dc->O_P = FB_P;
                      dc->O_P->Nx = NULL; dc->O_P->Pr = NULL;
                  }
                  else {
                      dc->O_P->Nx = ( B_K * ) Malloc ( ( int ) sizeof ( B_K ) ) ;
                      dc->O_P->Nx->Pr = dc->O_P;
                      dc->O_P = dc->O_P->Nx;
                      dc->O_P->Nx = NULL;
                  }
                   ( dc->O_P->x ) = fj;
                   ( dc->O_P->xl ) = fjl;
                   ( dc->O_P->y ) = gj;
                  break;
                  case '!':
                  fj = fj+fact1; fjl+= 1.; gj = gj+1;
                  break;
                  case '%':
                  fj = fjl;
                  break;
                  case 'f':
                  if ( i+2 >= j ) break;
                  val = title [ i+1 ] -'0';
                  val = val*10+ ( title [ i+2 ] -'0' ) ;
                  i+= 2;
                         /*gj = gj-2;*/
                  break;
                  case 'z':
                  if ( i+2 >= j ) break;
                  val = title [ i+2 ] -'0';
                  if ( val == 0.0 ) val = 1.0;
                  val = ( float ) ( title [ i+1 ] -'0' ) /val;
                  fact = fact*val;
                  i = i+2;
                         /*gj = gj-2;*/
                  break;
                  case 'h':
                  if ( i+2 >= j ) break;
                  val = title [ i+2 ] -'0';
                  if ( val == 0.0 ) val = 1.0;
                  val = ( float ) ( title [ i+1 ] -'0' ) /val;
                  hfact = hfact*val;
                  i = i+2;
                         /*gj = gj-2;*/
                  break;
                  case 'w':
                  if ( i+2 >= j ) break;
                  val = title [ i+2 ] -'0';
                  if ( val == 0.0 ) val = 1.0;
                  val = ( float ) ( title [ i+1 ] -'0' ) /val;
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
      *xdsp = ( fj*wd+gj*gp ) ;
      ngp = gj+0.1;
      dc->O_P = FB_P;
      while ( dc->O_P != NULL ) {
          dc->D_P = dc->O_P;
          dc->O_P = dc->O_P->Pr;
          free ( dc->D_P ) ;
      }
      return ( ngp ) ;
  }
  void ui_clip_limit ( DIG *G , float x1 , float y1 , float x2 , float y2 ) {
      int ix1 , ix2 , iy1 , iy2;
      kgWC *wc;
      kgDC *dc;
      wc = G->wc;
      dc = G->dc;
      uiBkup_clip_limits ( wc ) ;
      ix1 = uiscr_x ( x1 ) ;
      ix2 = uiscr_x ( x2 ) ;
      iy1 = uiscr_y ( y1 ) ;
      iy2 = uiscr_y ( y2 ) ;
      if ( ix1 >= ix2 ) {
          printf ( "%d %d %d %d\n" , ix1 , iy1 , ix2 , iy2 ) ;
          printf ( " Wrong Clip limits\n" ) ;
          exit ( 0 ) ;
      }
      if ( iy1 >= iy2 ) {
          printf ( "%d %d %d %d\n" , ix1 , iy1 , ix2 , iy2 ) ;
          printf ( " Wrong Clip limits\n" ) ;
          exit ( 0 ) ;
      }
      if ( ix2 < wc->c_v_x1 ) {wc->c_v_x1 = 1 , wc->c_v_x2 = 0 , \
           wc->c_v_y1 = 1 , wc->c_v_y2 = 0;
      return; };
      if ( ix1 > wc->c_v_x2 ) {wc->c_v_x1 = 1 , wc->c_v_x2 = 0 , \
           wc->c_v_y1 = 1 , wc->c_v_y2 = 0;
      return; };
      if ( iy2 < wc->c_v_y1 ) {wc->c_v_x1 = 1 , wc->c_v_x2 = 0 , \
           wc->c_v_y1 = 1 , wc->c_v_y2 = 0;
      return; };
      if ( iy1 > wc->c_v_y2 ) {wc->c_v_x1 = 1 , wc->c_v_x2 = 0 , \
           wc->c_v_y1 = 1 , wc->c_v_y2 = 0;
      return; };
      if ( ix1 >= wc->c_v_x1 ) wc->c_v_x1 = ix1;
      if ( ix2 <= wc->c_v_x2 ) wc->c_v_x2 = ix2;
      if ( iy1 >= wc->c_v_y1 ) wc->c_v_y1 = iy1;
      if ( iy2 <= wc->c_v_y2 ) wc->c_v_y2 = iy2;
  }
  void ui_clear_clip ( DIG *G ) {
      kgWC *wc;
      wc = G->wc;
      Dempty ( wc->Clip ) ;
      wc->Clip = Dopen ( ) ;
  }
  void ui_pop_clip ( DIG *G ) {
      kgWC *wc;
      wc = G->wc;
      uiRest_clip_limits ( wc ) ;
  }
#if 0
  int rect_pointer ( kgWC *wc , int x0 , int y0 , \
       int *xx , int *yy , int *entry ) \
  {
      static int key , x1 , y1;
      static int x2 , y2;
      if ( wc->entry == 0 ) {x2 = -100; y2 = -100; }
      if ( ( key = GetPointer ( & x1 , & y1 ) ) < 0 ) return ( -1 ) ;
      if ( ( key == 0 ) && ( ( x2 != x1 ) || ( y2 != y1 ) ) ) {
          if ( wc->entry ) {
              kg_scr_recover ( wc ) ;
              kg_scr_recover ( wc ) ;
              kg_scr_recover ( wc ) ;
              kg_scr_recover ( wc ) ;
          };
          *xx = x1 , *yy = y1;
          wc->entry = 1;
          if ( x0 < x1 ) xmin = x0 , xmax = x1;
          else xmin = x1 , xmax = x0;
          if ( y0 < y1 ) ymin = y0 , ymax = y1;
          else ymin = y1 , ymax = y0;
          kg_scr_back ( wc , xmin , ymin , xmax , ymin+1 ) ;
          kg_scr_back ( wc , xmin , ymax-1 , xmax , ymax ) ;
          kg_scr_back ( wc , xmin , ymin , xmin+1 , ymax ) ;
          kg_scr_back ( wc , xmax-1 , ymin , xmax , ymax ) ;
          _uiDraw_Bound ( wc , xmin , ymin , xmax , ymax ) ;
          x2 = x1 , y2 = y1;
      }
      if ( key == 0 ) return ( key ) ;
      *xx = x1 , *yy = y1;
      while ( GetPointer ( & x1 , & y1 ) > 0 ) ;
      return ( key ) ;
  }
#endif
  int draw_rect_cursor ( DIG *G , int x0 , int y0 , int x1 , int y1 ) {
      int key = 1;
      int x2 , y2;
      int xmin , ymin , xmax , ymax;
      DIALOG *D;
      int EVGAY;
      kgWC *wc;
      kgDC *dc;
      D = G->D;
      wc = G->wc;
      dc = G->dc;
      EVGAY = D->evgay;
      x2 = dc->x2 , y2 = dc->y2;
      if ( dc->entry == 0 ) {x2 = -100; y2 = -100; }
      if ( dc->entry ) {
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
      };
      dc->entry = 1;
      if ( x0 < x1 ) xmin = x0 , xmax = x1;
      else xmin = x1 , xmax = x0;
      if ( y0 < y1 ) ymin = y0 , ymax = y1;
      else ymin = y1 , ymax = y0;
      kg_scr_back ( wc , xmin , ymin , xmax , ymin+1 ) ;
      kg_scr_back ( wc , xmin , ymax-1 , xmax , ymax ) ;
      kg_scr_back ( wc , xmin , ymin , xmin+1 , ymax ) ;
      kg_scr_back ( wc , xmax-1 , ymin , xmax , ymax ) ;
      _uiDraw_Bound ( wc , xmin , ymin , xmax , ymax ) ;
      x2 = x1 , y2 = y1;
      dc->x2 = x2 , dc->y2 = y2;
      uiUpdateOn ( wc->D ) ;
      return ( key ) ;
  }
  int draw_box_cursor ( DIG *G , int x0 , int y0 , int x1 , int y1 ) {
      int key;
#if 1
      int x2 , y2;
      int xmin , ymin , xmax , ymax;
      DIALOG *D;
      int EVGAY;
      kgWC *wc;
      kgDC *dc;
      D = G->D;
      wc = G->wc;
      dc = G->dc;
      EVGAY = D->evgay;
      x2 = dc->x2;
      y2 = dc->y2;
      if ( dc->entry == 0 ) {x2 = -100; y2 = -100; }
      if ( dc->entry ) {
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
      };
      dc->entry = 1;
      xmin = x1 , xmax = x1+x0;
      ymin = y1; ymax = y1-y0;
      kg_scr_back ( wc , xmin , ymin , xmax , ymin+1 ) ;
      kg_scr_back ( wc , xmin , ymax-1 , xmax , ymax ) ;
      kg_scr_back ( wc , xmin , ymin , xmin+1 , ymax ) ;
      kg_scr_back ( wc , xmax-1 , ymin , xmax , ymax ) ;
      _uiDraw_Bound ( wc , xmin , ymin , xmax , ymax ) ;
      x2 = x1 , y2 = y1;
      uiUpdateOn ( wc->D ) ;
#endif
      dc->x2 = x2 , dc->y2 = y2;
      return ( key ) ;
  }
#if 0
  int box_pointer ( DIALOG *D , int x0 , int y0 , \
       int *xx , int *yy , int *entry ) \
  {
      static int key , x1 , y1;
      static int x2 , y2;
      if ( D->entry == 0 ) {x2 = -100; y2 = -100; }
      if ( ( key = GetPointer ( & x1 , & y1 ) ) < 0 ) return ( -1 ) ;
      if ( ( key == 0 ) && ( ( x2 != x1 ) || ( y2 != y1 ) ) ) {
          if ( D->entry ) {
              kg_scr_recover ( D ) ;
              kg_scr_recover ( D ) ;
              kg_scr_recover ( D ) ;
              kg_scr_recover ( D ) ;
          };
          *xx = x1 , *yy = y1;
          D->entry = 1;
          xmin = x1 , xmax = x1+x0;
          ymin = y1; ymax = y1-y0;
          kg_scr_back ( D , xmin , ymin , xmax , ymin+1 ) ;
          kg_scr_back ( D , xmin , ymax-1 , xmax , ymax ) ;
          kg_scr_back ( D , xmin , ymin , xmin+1 , ymax ) ;
          kg_scr_back ( D , xmax-1 , ymin , xmax , ymax ) ;
          _uiDraw_Bound ( WC ( D ) , xmin , ymin , xmax , ymax ) ;
          x2 = x1 , y2 = y1;
      }
      if ( key == 0 ) return ( key ) ;
      *xx = x1 , *yy = y1;
      while ( GetPointer ( & x1 , & y1 ) > 0 ) ;
      return ( key ) ;
  }
#endif
/* NEW CODE */
  void draw_arc_cursor ( DIG *G ) {
      unsigned int tempc , c_color;
      char nbuf [ 35 ] ;
      kgDC *dc;
      dc = G->dc;
      sprintf ( nbuf , "Angle = %6.2f" , dc->gcur_t ) ;
      nbuf [ 34 ] = '\0';
      uimsg_menu ( G , dc->msg_x , dc->msg_y , 34L , nbuf ) ;
      uiUpdateOn ( G->D ) ;
  }
  int limit_cur ( DIG *G , int *x1 , int *y1 , int xorg , int yorg ) {
      double t = 0;
      DIALOG *D;
      kgWC *wc;
      kgDC *dc;
      D = G->D;
      wc = G->wc;
      dc = G->dc;
      t = atan2 ( ( double ) ( *y1-yorg ) , ( double ) ( *x1-xorg ) ) ;
      *x1 = xorg+dc->gcur_r*cos ( t ) ;
      *y1 = yorg+dc->gcur_r*sin ( t ) ;
/*  if((*x1<0)||(*x1>EVGAX)||(*y1<0)||(*y1>EVGAY)) return -1;*/
      dc->gcur_t = 180*t/3.14159265;
      if ( dc->gcur_t < 0. ) dc->gcur_t = 360+dc->gcur_t;
      dc->gcur_t = 360-dc->gcur_t;
      return 1;
  }
  void draw_dst_cursor ( DIG *G , float xo , float yo ) {
      unsigned int tempc , c_color;
      char nbuf [ 35 ] ;
      float x , y , dist , t;
      int EVGAY;
      DIALOG *D;
      kgWC *wc;
      kgDC *dc;
      D = G->D;
      wc = G->wc;
      dc = G->dc;
      EVGAY = wc->EVGAY;
      x = uiusr_x ( dc->gcur_x ) ;
      y = uiusr_y ( EVGAY-dc->gcur_y ) ;
      dist = sqrt ( ( double ) ( xo-x ) * ( xo-x ) + ( yo-y ) * ( yo-y ) ) ;
      t = atan2 ( ( double ) ( y-yo ) , ( double ) ( x-xo ) ) ;
      t = 180*t/3.14159265;
      if ( t < 0.0 ) t = 360+t;
      sprintf ( nbuf , "length=%9.2f   Ang=%6.2f" , dist , t ) ;
      nbuf [ 34 ] = '\0';
      uimsg_menu ( G , dc->msg_x , dc->msg_y , 34L , nbuf ) ;
      uiUpdateOn ( D ) ;
  }
  void uiBkup_clip_limits ( kgWC *wc ) {
      CLIP *tmp;
//  printf("Backup\n");
      if ( wc->Clip == NULL ) wc->Clip = Dopen ( ) ;
      tmp = ( CLIP * ) Malloc ( sizeof ( CLIP ) ) ;
      tmp->cx1 = wc->c_v_x1;
      tmp->cy1 = wc->c_v_y1;
      tmp->cy2 = wc->c_v_y2;
      tmp->cx2 = wc->c_v_x2;
      Dappend ( wc->Clip , tmp ) ;
  }
  void uiSet_full_scrn ( kgWC *wc ) {
      int EVGAY;
      EVGAY = wc->EVGAY ;
      uiset_full_scrn;
  }
  void uiRest_clip_limits ( kgWC *wc ) {
      CLIP *tmp;
      if ( wc->Clip == NULL ) {
          printf ( " TRYING RESTORE FROM NULL  Clip\n" ) ;
          return;
      }
#if 0
      if ( wc->Clip->st == NULL ) {
          printf ( " TRYING RESTORE FROM EMPTY Clip\n" ) ;
          return;
      }
#endif
      Dend ( wc->Clip ) ;
      tmp = ( CLIP * ) Getrecordrev ( wc->Clip ) ;
      if ( tmp == NULL ) {
          printf ( "Error: Failed to restore clip\n" ) ;
          exit ( 0 ) ;
      }
      wc->c_v_x1 = tmp->cx1 ;
      wc->c_v_y1 = tmp->cy1 ;
      wc->c_v_y2 = tmp->cy2 ;
      wc->c_v_x2 = tmp->cx2 ;
      Dend ( wc->Clip ) ;
      Ddelete ( wc->Clip ) ;
//  printf("Restore:%d\n",Dcount(wc->Clip));
  }
  void _box_fill ( int x1 , int y1 , int x2 , int y2 , unsigned int color ) {
#if 0
      int x_min , y_min , x_max , x_max1 , x_min1 , y_max , i , j;
      short po1;
      unsigned int temp;
      temp = get_cur_clr ( ) ;
      set_clr ( color ) ;
      if ( x1 < x2 ) { x_min = x1; x_max1 = x2; }
      else{ x_min = x2; x_max1 = x1; }
      if ( ( x_min <= c_v_x2 ) && ( x_max1 >= c_v_x1 ) ) {
          if ( y1 < y2 ) { y_min = y1; y_max = y2; }
          else{ y_min = y2; y_max = y1; }
          if ( ( y_min <= c_v_y2 ) && ( y_max >= c_v_y1 ) ) {
              if ( x_max1 > c_v_x2 ) x_max1 = c_v_x2;
              if ( x_min < c_v_x1 ) x_min = c_v_x1;
              if ( y_max > c_v_y2 ) y_max = c_v_y2;
              if ( y_min < c_v_y1 ) y_min = c_v_y1;
              _Box_Fill ( x_min , EVGAY-y_max , x_max1 , EVGAY-y_min ) ;
          }
      }
      set_clr ( temp ) ;
#endif
      return;
  }
  void _uibox_fill ( kgWC *wc , int x1 , int y1 , \
       int x2 , int y2 , unsigned int color ) \
  {
      int x_min , y_min , x_max , x_max1 , x_min1 , y_max , i , j;
      short po1;
      unsigned int temp;
      DIALOG *D;
      D = wc->D;
      temp = wc->c_color;
      wcset_clr ( wc , color ) ;
      if ( x1 < x2 ) { x_min = x1; x_max1 = x2; }
      else{ x_min = x2; x_max1 = x1; }
      if ( ( x_min <= wc->c_v_x2 ) && ( x_max1 >= wc->c_v_x1 ) ) {
          if ( y1 < y2 ) { y_min = y1; y_max = y2; }
          else{ y_min = y2; y_max = y1; }
          if ( ( y_min <= wc->c_v_y2 ) && ( y_max >= wc->c_v_y1 ) ) {
              if ( x_max1 > wc->c_v_x2 ) x_max1 = wc->c_v_x2;
              if ( x_min < wc->c_v_x1 ) x_min = wc->c_v_x1;
              if ( y_max > wc->c_v_y2 ) y_max = wc->c_v_y2;
              if ( y_min < wc->c_v_y1 ) y_min = wc->c_v_y1;
              _uiBox_Fill ( wc , x_min , D->evgay-y_max , x_max1 , D->evgay-y_min ) ;
          }
      }
      wcset_clr ( wc , temp ) ;
      return;
  }
  void ui_PointerZoom ( DIG *G , float *xmin , float *ymin , \
       float *xmax , float *ymax ) \
  {
      short it = 7 , item = 1;
      int EVGAY , DBLBUF = 0 , MAG = 0;
      float X0 , Y0 , X1 , Y1;
      DIALOG *D;
      D = ( DIALOG * ) G->D;
      kgWC *wc;
      kgDC *dc;
      wc = G->wc;
      dc = G->dc;
      if ( dc->DOUBLE == 1 ) {
          DBLBUF = 1;
          kgSingleBuffer ( G ) ;
      }
      if ( G->D_ON == 0 ) {
          MAG = G->MAG;
          kgAntialiasingOff ( G ) ;
      }
      EVGAY = D->evgay;
      dc->lcur_x = dc->v_x1;
      dc->lcur_y = EVGAY-dc->v_y1;
      dc->rcur_x = dc->v_x2;
      dc->rcur_y = EVGAY-dc->v_y2;
      dc->gw_x1 = uiusr_x ( dc->lcur_x ) ;
      dc->gw_x2 = uiusr_x ( dc->rcur_x ) ;
      dc->gw_y1 = uiusr_y ( ( EVGAY-dc->lcur_y ) ) ;
      dc->gw_y2 = uiusr_y ( ( EVGAY-dc->rcur_y ) ) ;
      X0 = dc->gw_x1;
      Y0 = dc->gw_y1;
      kgCrossCursor ( G , & X0 , & Y0 ) ;
      X1 = X0;
      Y1 = Y0;
      kgRectCursor ( G , & X1 , & Y1 , & X0 , & Y0 ) ;
      if ( X0 < X1 ) {dc->gw_x1 = X0; dc->gw_x2 = X1; }
      else {dc->gw_x1 = X1; dc->gw_x2 = X0; }
      if ( Y0 < Y1 ) {dc->gw_y1 = Y0; dc->gw_y2 = Y1; }
      else {dc->gw_y1 = Y1; dc->gw_y2 = Y0; }
#if 0
      if ( dc->SET3D == 1 ) {
          if ( dc->DBL_BUF == 1 ) {
              kgReview ( G ) ;
          }
          else {
              kgDoubleBuffer ( G ) ;
              kgReview ( G ) ;
              kgSwapBuffers ( G ) ;
              kgSingleBuffer ( G ) ;
          }
      }
      else kgReview ( G ) ;
#else
      if ( MAG == 0 ) {
//         ui_window(G,dc->gw_x1,dc->gw_y1,dc->gw_x2,dc->gw_y2);
          kgUserFrame ( G , dc->gw_x1 , dc->gw_y1 , dc->gw_x2 , dc->gw_y2 ) ;
#if 1
          if ( dc->SET3D == 1 ) {
              G->MAG = 1;
              kgAntialiasedReview ( G ) ;
          }
          else kgReview ( G ) ;
#else
          kgReview ( G ) ;
#endif
      }
      else {
          kgUserFrame ( G , dc->gw_x1 , dc->gw_y1 , dc->gw_x2 , dc->gw_y2 ) ;
//         kgReview(G);
          kgAntialiasingOn ( G , MAG-1 ) ;
          kgAntialiasedReview ( G ) ;
      }
#endif
      if ( DBLBUF ) kgDoubleBuffer ( G ) ;
      dc->lcur_x = dc->v_x1;
      dc->lcur_y = EVGAY-dc->v_y1;
      dc->rcur_x = dc->v_x2;
      dc->rcur_y = EVGAY-dc->v_y2;
      *xmin = dc->gw_x1 , *xmax = dc->gw_x2;
      *ymin = dc->gw_y1 , *ymax = dc->gw_y2;
  }
  void ui_PointerAntialiasedZoom ( DIG *G , float *xmin , \
       float *ymin , float *xmax , float *ymax ) \
  {
      int EVGAY , DBLBUF = 0; ;
      short it = 7 , item = 1;
      float X0 , Y0 , X1 , Y1;
      DIALOG *D;
      D = ( DIALOG * ) G->D;
      kgWC *wc;
      kgDC *dc;
      wc = WC ( D ) ;
      dc = G->dc;
      EVGAY = D->evgay;
      if ( dc->DOUBLE == 1 ) {
          DBLBUF = 1;
          kgSingleBuffer ( G ) ;
      }
      dc->lcur_x = dc->v_x1;
      dc->lcur_y = EVGAY-dc->v_y1;
      dc->rcur_x = dc->v_x2;
      dc->rcur_y = EVGAY-dc->v_y2;
      dc->gw_x1 = uiusr_x ( dc->lcur_x ) ;
      dc->gw_x2 = uiusr_x ( dc->rcur_x ) ;
      dc->gw_y1 = uiusr_y ( ( EVGAY-dc->lcur_y ) ) ;
      dc->gw_y2 = uiusr_y ( ( EVGAY-dc->rcur_y ) ) ;
      X0 = dc->gw_x1;
      Y0 = dc->gw_y1;
      kgCrossCursor ( G , & X0 , & Y0 ) ;
      X1 = X0;
      Y1 = Y0;
      kgRectCursor ( G , & X1 , & Y1 , & X0 , & Y0 ) ;
      if ( X0 < X1 ) {dc->gw_x1 = X0; dc->gw_x2 = X1; }
      else {dc->gw_x1 = X1; dc->gw_x2 = X0; }
      if ( Y0 < Y1 ) {dc->gw_y1 = Y0; dc->gw_y2 = Y1; }
      else {dc->gw_y1 = Y1; dc->gw_y2 = Y0; }
      ui_window ( G , dc->gw_x1 , dc->gw_y1 , dc->gw_x2 , dc->gw_y2 ) ;
#if 0
      if ( dc->SET3D == 1 ) {
          if ( dc->DBL_BUF == 1 ) {
              kgAntialiasedReview ( G ) ;
          }
          else {
              kgDoubleBuffer ( G ) ;
              kgAntialiasedReview ( G ) ;
              kgSwapBuffers ( G ) ;
              kgSingleBuffer ( G ) ;
          }
      }
      else kgAntialiasedReview ( G ) ;
#endif
      if ( DBLBUF ) kgDoubleBuffer ( G ) ;
      dc->lcur_x = dc->v_x1;
      dc->lcur_y = EVGAY-dc->v_y1;
      dc->rcur_x = dc->v_x2;
      dc->rcur_y = EVGAY-dc->v_y2;
      *xmin = dc->gw_x1 , *xmax = dc->gw_x2;
      *ymin = dc->gw_y1 , *ymax = dc->gw_y2;
  }
/*************************************************
 AS ON 25th Sept 2002
 This routine is added for interactive zooming
*************************************************/
  void ui_PointerZoomPan ( DIG *G , float *xmin , \
       float *ymin , float *xmax , float *ymax ) \
  {
      int xpo , ypo , key , xpo1 , ypo1;
      float xm , ym , dx , dy , sx , sy;
      int EVGAY;
      DIALOG *D;
      D = ( DIALOG * ) G->D;
      kgWC *wc;
      kgDC *dc;
      wc = WC ( D ) ;
      dc = G->dc;
      EVGAY = D->evgay;
      xm = ( *xmax + *xmin ) *0.5;
      ym = ( *ymax + *ymin ) *0.5;
      dx = ( *xmax - *xmin ) *0.5;
      dy = ( *ymax - *ymin ) *0.5;
      while ( GetPointer ( & xpo , & ypo ) > 0 ) ;
      while ( 1 ) {
          key = GetPointer ( & xpo , & ypo ) ;
          if ( key <= 0 ) continue;
          if ( ( xpo < dc->v_x1 ) || ( xpo > dc->v_x2 )  \
              || ( ( EVGAY -ypo ) < dc->v_y1 ) \
           || ( ( EVGAY -ypo ) > dc->v_y2 ) ) break;
          switch ( key ) {
              case 1:
              dx *= 0.95;
              dy *= 0.95;
              *xmin = xm -dx;
              *xmax = xm +dx;
              *ymin = ym -dy;
              *ymax = ym +dy;
              ui_window ( G , *xmin , *ymin , *xmax , *ymax ) ;
              kgDoubleBuffer ( G ) ;
              kgReview ( G ) ;
              kgSwapBuffers ( G ) ;
              kgSingleBuffer ( G ) ;
              break;
              case 2:
              key = GetPointer ( & xpo1 , & ypo1 ) ;
              while ( key == 2 ) {
                  if ( ( xpo1 != xpo ) && ( ypo1 != ypo ) ) {
                      if ( ( xpo < dc->v_x1 ) || ( xpo > dc->v_x2 )  \
                          || ( ( EVGAY -ypo ) < dc->v_y1 ) \
                       || ( ( EVGAY -ypo ) > dc->v_y2 ) ) break;
                      sx = uiusr_x ( xpo1 ) - uiusr_x ( xpo ) ;
                      sy = uiusr_y ( EVGAY -ypo1 ) - uiusr_y ( EVGAY -ypo ) ;
                      xm -= sx;
                      ym -= sy;
                      *xmin = xm -dx;
                      *xmax = xm +dx;
                      *ymin = ym -dy;
                      *ymax = ym +dy;
                      ui_window ( G , *xmin , *ymin , *xmax , *ymax ) ;
                      kgDoubleBuffer ( G ) ;
                      kgReview ( G ) ;
                      kgSwapBuffers ( G ) ;
                      kgSingleBuffer ( G ) ;
                      xpo = xpo1; ypo = ypo1;
                  }
                  key = GetPointer ( & xpo1 , & ypo1 ) ;
              }
              break;
              case 3:
              dx *= 1.05;
              dy *= 1.05;
              *xmin = xm -dx;
              *xmax = xm +dx;
              *ymin = ym -dy;
              *ymax = ym +dy;
              ui_window ( G , *xmin , *ymin , *xmax , *ymax ) ;
              kgDoubleBuffer ( G ) ;
              kgReview ( G ) ;
              kgSwapBuffers ( G ) ;
              kgSingleBuffer ( G ) ;
              break;
          }
      }
  }
/*************************************************
 AS ON 10th Feb 2005
 This routine is added for interactive move forward/Backward
*************************************************/
#if 0
#endif
  void pick_box ( DIG *G , float *x1 , float *y1 , float *x2 , float *y2 ) {
#if 0
      short it = 6 , item = 1;
      char *cmenu [ ] = {
      "Diagonal" , "move_Around" , "Stretch" , "Quit" , \
           "menuOff" , "Usepointer" , NULL};
      DIALOG *D;
      kgWC *wc;
      wc = G->wc;
      D = G->D;
//      Sync();
      gw_x1 = usr_x ( lcur_x ) ;
      gw_x2 = usr_x ( rcur_x ) ;
      gw_y1 = usr_y ( ( EVGAY-lcur_y ) ) ;
      gw_y2 = usr_y ( ( EVGAY-rcur_y ) ) ;
      while ( item != 4 ) {
          ChangeCursor ( 59 ) ;
          item = uiMenu ( D , 5 , 200 , 6 , 11 , cmenu , it ) ;
          ChangeCursor ( 35 ) ;
          switch ( item ) {
              case 1:
              lzoom ( G ) ;
              it = 4;
              break;
              case 2:
              pan ( G ) ;
              it = 4;
              break;
              case 3:
              stretch ( G ) ;
              it = 4;
              break;
              case 6:
              pointer_zoom ( G ) ;
              item = 4;
              case 4:
              gw_x1 = usr_x ( lcur_x ) ;
              gw_x2 = usr_x ( rcur_x ) ;
              gw_y1 = usr_y ( ( EVGAY-lcur_y ) ) ;
              gw_y2 = usr_y ( ( EVGAY-rcur_y ) ) ;
              it = item;
              break;
              case 5:
              it = item;
              get_key_pointer ( 1 ) ;
              break;
          }
      }
      *x1 = gw_x1;
      *y1 = gw_y1;
      *x2 = gw_x2;
      *y2 = gw_y2;
      ChangeCursor ( 59 ) ;
#endif
  }
  void draw_cursor ( DIG *G ) {
#if 1
      unsigned int tempc;
      char nbuf [ 50 ] ;
      int i , l , EVGAY;
      float x , y;
      int vx1 , vy1 , vx2 , vy2;
      DIALOG *D;
      kgDC *dc;
      kgWC *wc;
      D = G->D;
      dc = G->dc;
      wc = G->wc;
      EVGAY = D->evgay;
      vx1 = dc->v_x1+dc->D_x; vy1 = EVGAY- ( dc->v_y2+dc->D_y ) ;
      vx2 = ( dc->v_x2+dc->D_x ) ; vy2 = EVGAY- ( dc->v_y1+dc->D_y ) ;
      if ( dc->gcur_x > vx2-1 ) {dc->gcur_x = vx2-1; }
      if ( dc->gcur_y > vy2-1 ) {dc->gcur_y = vy2-1; }
      if ( dc->gcur_x < vx1 ) {dc->gcur_x = vx1; }
      if ( dc->gcur_y < vy1 ) {dc->gcur_y = vy1; }
      x = uiusr_x ( dc->gcur_x ) ;
      y = uiusr_y ( EVGAY-dc->gcur_y ) ;
      sprintf ( nbuf , dc->Posfmt , x , y ) ;
      l = strlen ( nbuf ) ;
      for ( i = l; i < 49; i++ ) nbuf [ i ] = ' ';
      nbuf [ 34 ] = '\0';
      uimsg_menu ( G , dc->msg_x , dc->msg_y , 34 , nbuf ) ;
//  Position_Pointer(dc->gcur_x,dc->gcur_y);
      uiUpdateOn ( D ) ;
#endif
  }
  void draw_box_cursor_org ( ) {
#if 0
      unsigned int tempc;
      char nbuf [ 35 ] = {"POSITION FOR PLACING TEXT......"};
      float x , y;
      if ( gcur_x > v_x2 ) {gcur_x = v_x2; gbell ( ) ; };
      if ( gcur_y > EVGAY-v_y1-2 ) {gcur_y = EVGAY-v_y1-2; gbell ( ) ; };
      if ( gcur_x < v_x1 ) {gcur_x = v_x1; gbell ( ) ; };
      if ( gcur_y < EVGAY-v_y2 ) {gcur_y = EVGAY-v_y2; gbell ( ) ; };
      x = usr_x ( gcur_x ) ;
      y = usr_y ( EVGAY-gcur_y ) ;
      msg_menu ( msg_x , msg_y , 34 , nbuf ) ;
      Position_Pointer ( gcur_x , gcur_y ) ;
#endif
  }
  void draw_po_cursor ( DIG *G ) {
      unsigned int tempc;
      int i , l;
      char nbuf [ 50 ] ;
      float x , y;
      DIALOG *D;
      int EVGAY;
      int vx1 , vy1 , vx2 , vy2;
      kgWC *wc;
      kgDC *dc;
      D = G->D;
      wc = G->wc;
      dc = G->dc;
      EVGAY = D->evgay;
      vx1 = dc->v_x1+dc->D_x; vy1 = EVGAY- ( dc->v_y2+dc->D_y ) ;
      vx2 = ( dc->v_x2+dc->D_x ) ; vy2 = EVGAY- ( dc->v_y1+dc->D_y ) ;
      if ( dc->gcur_x >= vx2 ) {dc->gcur_x = vx2-1; }
      if ( dc->gcur_y > vy2-1 ) {dc->gcur_y = vy2-1; }
      if ( dc->gcur_x < vx1 ) {dc->gcur_x = vx1; }
      if ( dc->gcur_y < vy1 ) {dc->gcur_y = vy1; }
      x = uiusr_x ( dc->gcur_x ) ;
      y = uiusr_y ( EVGAY-dc->gcur_y ) ;
      sprintf ( nbuf , dc->Posfmt , x , y ) ;
      l = strlen ( nbuf ) ;
      for ( i = l; i < 49; i++ ) nbuf [ i ] = ' ';
      nbuf [ 34 ] = '\0';
//  printf("%s %d %d\n",nbuf,dc->msg_x,dc->msg_y);
      uimsg_menu ( G , dc->msg_x , dc->msg_y , 34 , nbuf ) ;
      uiUpdateOn ( D ) ;
 /* XWarpPointer(Dsp,Win,Win,0,0,EVGAX+1,EVGAY+1,gcur_x,gcur_y);
*/
  }
  int kgCheckEscapeOld ( DIALOG *D ) {
      KBEVENT kb , kbo;
      if ( kgCheckEvent ( D , & kbo ) == 0 ) return 0;
      while ( kbo.event != 5 ) {
          if ( kgCheckEvent ( D , & kbo ) == 0 ) return 0;
      }
      return ui_Escape ( kbo.key ) ;
  }
  int kgEscape ( KBEVENT kb ) {
      return ui_Escape ( kb.key ) ;
  }
  int kgLeftarrow ( KBEVENT kb ) {
      return ui_Leftarrow ( kb.key ) ;
  }
  int kgRightarrow ( KBEVENT kb ) {
      return ui_Rightarrow ( kb.key ) ;
  }
  int kgUparrow ( KBEVENT kb ) {
      return ui_Uparrow ( kb.key ) ;
  }
  int kgDownarrow ( KBEVENT kb ) {
      return ui_Downarrow ( kb.key ) ;
  }
  int kgEnter ( KBEVENT kb ) {
      return ui_Return ( kb.key ) ;
  }
  void kgSkipEvents ( DIALOG *D ) {
      KBEVENT kb , kbo;
      if ( kgCheckEvent ( D , & kbo ) == 0 ) return ;
      while ( kbo.event != 0 ) {
          if ( kgCheckEvent ( D , & kbo ) == 0 ) return ;
      }
      return ;
  }
  void kgSkipButtonRelease ( DIALOG *D ) {
      KBEVENT kb , kbo;
      int count = 0 , press = 0;
      kgWC *wc;
      wc = WC ( D ) ;
//TCB
      if ( wc->event.type != ButtonPress ) return;
      while ( kgCheckEvent ( D , & kbo ) == 0 ) { count++;
      usleep ( 10000 ) ; if ( count > 100 ) return; }
      usleep ( 10000 ) ;
      count = 0;
      while ( kbo.event == 1 ) { press = 1; kbo = kgGetEvent ( D ) ; }
      if ( press ) while ( kbo.event != 2 ) {kbo = kgGetEvent ( D ) ; }
      if ( kbo.event == 2 ) return;
      while ( kbo.event != 2 ) {
          count++;
//   kbo=ui_getevent();
          if ( kgCheckEvent ( D , & kbo ) == 0 ) return ;
          if ( count > 100 ) break;
      }
      return ;
  }
  void kgWaitButtonRelease ( DIALOG *D ) {
      KBEVENT kb , kbo;
      int count = 0 , press = 0;
      while ( kgCheckEvent ( D , & kbo ) == 0 ) { count++;
      usleep ( 10000 ) ; if ( count > 100 ) return; }
      usleep ( 10000 ) ;
      count = 0;
      while ( kbo.event == 3 ) { press = 1; kbo = kgGetEvent ( D ) ; }
      if ( press ) while ( kbo.event != 2 ) {kbo = kgGetEvent ( D ) ; }
#if 0
      if ( kbo.event == 2 ) return;
      while ( kbo.event != 2 ) {
//   kbo=ui_getevent();
          if ( kgCheckEvent ( D , & kbo ) == 0 ) return ;
      }
#else
#endif
      return ;
  }
  KBEVENT kgSkipMouseMove ( DIALOG *D ) {
      int i , SKIP = 20;
      KBEVENT kb , kbo;
      kbo = kgGetEvent ( D ) ;
      kb = kbo;
      for ( i = 0; i < SKIP; i++ ) {
          if ( ( kbo.event == 0 ) || ( kbo.event == 3 ) ) {
              kb = kbo;
              if ( kgCheckEvent ( D , & kbo ) == 0 ) break;
              continue;
          }
          else return kb;
      }
      return kb;
  }
  int kgGetTimedEvent_o ( DIALOG *D , KBEVENT *e ) {
      int i , SKIP = 20;
      KBEVENT kb , kbo;
      if ( kgCheckEvent ( D , & kbo ) == 0 ) {
//    kgThreadSleep(0,250);
          kgThreadSleep ( 0 , 2000 ) ;
          if ( kgCheckEvent ( D , & kbo ) == 0 ) return 0;
      }
      kb = kbo;
      for ( i = 0; i < SKIP; i++ ) {
          kb = kbo;
          if ( ( kbo.event == 0 ) || ( kbo.event == 3 ) ) {
              if ( kgCheckEvent ( D , & kbo ) == 0 ) break;
              continue;
          }
          else { break; }
      }
      *e = kb;
      return 1;
  }
  int kgGetTimedEvent ( DIALOG *D , KBEVENT *e ) {
      int i , SKIP = 20;
      KBEVENT kb , kbo;
      if ( kgCheckEvent ( D , & kbo ) == 0 ) {
//    kgThreadSleep(0,250);
          kgThreadSleep ( 0 , 2000 ) ;
          if ( kgCheckEvent ( D , & kbo ) == 0 ) return 0;
      }
      kb = kbo;
      i = 0;
      while ( ( kbo.event == 0 ) || ( kbo.event == 3 ) ) {
          if ( kgCheckEvent ( D , & kbo ) == 0 ) break;
          kb = kbo;
          i++;
          if ( i == SKIP ) break;
      }
      *e = kb;
      return 1;
  }
  DIG *uigetGbox ( void *Tmp , int gbox ) {
      DIALOG *D;
      DIG *G;
      G = ( DIG * ) ( ( DIALOG * ) Tmp )->d [ gbox ] .g;
      return G;
  }
  char kgCursor ( DIG *G , float *xx , float *yy ) {
      KBEVENT kbevent;
      int kb , button , event , OK = 0;
      char ch , key , MAG = 0;
      int xpo , ypo , but , pointer;
      DIALOG *D;
      D = ( DIALOG * ) G->D;
      kgWC *wc;
      kgDC *dc;
      wc = WC ( D ) ;
      dc = G->dc;
      if ( G->D_ON == 0 ) {
          MAG = G->MAG;
          kgAntialiasingOff ( G ) ;
      }
      dc->entry = 0;
      dc->gcur_x = uiscr_x ( *xx ) ;
      dc->gcur_y = D->evgay-uiscr_y ( *yy ) ;
      uiScrn_back ( wc , dc->msg_x , dc->msg_y , 34 ) ;
      draw_cursor ( G ) ;
      while ( ! OK ) {
          kbevent = kgSkipMouseMove ( D ) ;
          event = kbevent.event;
          button = kbevent.button;
          kb = kbevent.key;
          xpo = kbevent.x;
          ypo = kbevent.y;
          switch ( event ) {
              case 0: // pointer movement
//     case 2:  // button release
              case 3: // button press and movement
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  draw_po_cursor ( G ) ;
              }
              break;
              case 1: // button press
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  draw_po_cursor ( G ) ;
              }
              if ( button == 1 ) key = '\r';
              if ( button == 3 ) key = '.';
              OK = 1;
              continue;
              case 5: // key release
              key = kb;
              if ( ui_Linefeed ( kb ) ) { key = '\r'; OK = 1; continue; }
              if ( ui_Return ( kb ) ) {key = '\r'; OK = 1; continue; }
//       if(isdigit(kb)) {jmp = kb-'0';continue;}
              if ( ( kb >= '0' ) && ( kb <= '9' ) ) {dc->jmp = kb-'0'; continue; }
              if ( ( kb == '.' ) || ( kb == 'u' ) || ( kb == 'U' ) ) {
                  OK = 1; continue;
              }
              if ( ui_Uparrow ( kb ) ) { cursor_up ( dc ) ; break; }
              if ( ui_Downarrow ( kb ) ) { cursor_down ( dc ) ; break; }
              if ( ui_Leftarrow ( kb ) ) { cursor_left ( dc ) ; break; }
              if ( ui_Rightarrow ( kb ) ) { cursor_right ( dc ) ; break; }
              break;
              default:
              break;
          }
      }
      uiScrn_recover ( wc ) ;
      *xx = uiusr_x ( dc->gcur_x ) ;
      *yy = uiusr_y ( ( D->evgay-dc->gcur_y ) ) ;
      if ( MAG ) {
          kgAntialiasingOn ( G , MAG-1 ) ;
      }
      return ( key ) ;
  }
  void uiput_pixel ( kgWC *wc , int col , int row ) {
      if ( ( col > wc->c_v_x2 ) || ( col < wc->c_v_x1 ) || ( row > wc->c_v_y2 ) \
       || ( row < wc->c_v_y1 ) ) return;
      uiput_pixl ( wc , col , wc->EVGAY-row ) ;
      return;
  }
  void _uidraw_line ( kgWC *wc , int x1 , int y1 , int x2 , int y2 ) {
      char far *addr;
      short i , dx , dy , f , g , inc1 , inc2 , ab , \
           dx1 , dy1 , miny , maxy , minx , maxx;
      short clip , ch , rh , r , c;
      unsigned int temp;
      int xa , ya , xb , yb;
      xa = x1 , xb = x2 , ya = y1 , yb = y2;
      clip = uiclip_line ( wc , & xa , & ya , & xb , & yb ) ;
      if ( clip == 0 ) return;
      _uiLine ( wc , xa , ( ya ) , xb , ( yb ) ) ;
      return;
  }
  void uiCliped_Line ( kgWC *wc , int x1 , int y1 , int x2 , int y2 ) {
      short clip;
      int xa , ya , xb , yb;
      int EVGAY;
      DIALOG *D;
      D = ( DIALOG * ) ( wc->D ) ;
      EVGAY = D->evgay;
      xa = x1 , xb = x2 , ya = y1 , yb = y2;
      ya = EVGAY-ya , yb = EVGAY-yb;
      clip = uiclip_line ( wc , & xa , & ya , & xb , & yb ) ;
      if ( clip == 0 ) return;
      _uiLine ( wc , xa , ( ya ) , xb , ( yb ) ) ;
      return;
  }
  void uiCliped_Line_n ( kgWC *wc , int x1 , int y1 , int x2 , int y2 ) {
      short clip;
      int xa , ya , xb , yb;
      DIALOG *D;
      D = ( DIALOG * ) ( wc->D ) ;
      xa = x1 , xb = x2 , ya = D->evgay-y1 , yb = D->evgay-y2;
      clip = uiclip_line ( wc , & xa , & ya , & xb , & yb ) ;
      if ( clip == 0 ) return;
      _uiLine ( wc , xa , ( ya ) , xb , ( yb ) ) ;
      return;
  }
  void _uihori_line ( kgWC *wc , int x1 , int x2 , int y ) {
      short i , clip;
      int xa , ya , xb , yb;
      if ( x1 == x2 ) return;
      xa = x1 , xb = x2 , ya = y , yb = y;
      clip = uiclip_line ( wc , & xa , & ya , & xb , & yb ) ;
      if ( clip == 0 ) return;
      _uiLine ( wc , xa , ( ya ) , xb , ( yb ) ) ;
  }
  void _uimhori_line ( kgWC *wc , int x1 , int x2 , int y ) {
      short i , clip;
      int xa , ya , xb , yb;
      if ( ( x2-x1 ) > 1 ) {x1++; }
      xa = x1 , xb = x2 , ya = y , yb = y;
      clip = uiclip_line ( wc , & xa , & ya , & xb , & yb ) ;
      if ( clip == 0 ) return;
      _uiLine ( wc , xa , ( ya ) , xb , ( yb ) ) ;
  }
  void _uivert_line ( kgWC *wc , int y1 , int y2 , int x ) {
      short i , clip;
      int xa , ya , xb , yb;
      if ( y1 == y2 ) return;
      xa = x , xb = x , ya = y1 , yb = y2;
      clip = uiclip_line ( wc , & xa , & ya , & xb , & yb ) ;
      if ( clip == 0 ) return;
      _uiLine ( wc , xa , ( ya ) , xb , ( yb ) ) ;
  }
  void _ui_v_line ( kgWC *wc , int ya , int yb , int x ) {
  /*XDraw((short)x,(short)ya,(short)x,(short)yb);*/
      _uiLine ( wc , x , ( ya ) , x , ( yb ) ) ;
  }
  void _ui_h_line ( kgWC *wc , int xa , int xb , int y ) {
  /*XDraw((short)xa,(short)y,(short)xb,(short)y);*/
      _uiLine ( wc , xa , ( y ) , xb , ( y ) ) ;
  }
  void _dv_v_line ( kgWC *wc , int ya , int yb , int x ) {
  /*XDraw((short)x,(short)ya,(short)x,(short)yb);*/
      _dvLine ( wc , x , ( ya ) , x , ( yb ) ) ;
  }
  void _dv_h_line ( kgWC *wc , int xa , int xb , int y ) {
  /*XDraw((short)xa,(short)y,(short)xb,(short)y);*/
      _dvLine ( wc , xa , ( y ) , xb , ( y ) ) ;
  }
  char kgBoxCursor ( DIG *G , float *xx , float *yy , \
       float *xbgn , float *ybgn ) \
  {
      KBEVENT kbevent;
      DIALOG *D;
      int kb , button , event , OK = 0 , c_color; ;
      int ch , key , MAG = 0;
      int xpo , ypo , but , pointer , xorg , yorg , xorg1 , yorg1;
      int temp;
      D = ( DIALOG * ) G->D;
      kgWC *wc;
      kgDC *dc;
      wc = G->wc;
      dc = G->dc;
      if ( G->D_ON == 0 ) {
          MAG = G->MAG;
          kgAntialiasingOff ( G ) ;
      }
      dc->entry = 0;
      temp = dc->c_bound;
      dc->c_bound = 14;
      uiset_clr ( D , 8 ) ;
      c_color = wc->c_color;
      xorg1 = uiscr_x ( *xx ) ;
      yorg1 = D->evgay-uiscr_y ( *yy ) ;
      xorg = uiscr_x ( *xbgn ) ;
      yorg = D->evgay-uiscr_y ( *ybgn ) ;
      dc->gcur_x = xorg1;
      dc->gcur_y = yorg1;
      xorg -= xorg1;
      yorg = yorg1-yorg;
      uiScrn_back ( wc , dc->msg_x , dc->msg_y , 34 ) ;
      xpo = dc->gcur_x , ypo = dc->gcur_y;
//  gbell();
//  ChangeCursor(35);
      draw_cursor ( G ) ;
      draw_box_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
      while ( ! OK ) {
//    kbevent = ui_getevent();
          kbevent = kgSkipMouseMove ( D ) ;
          event = kbevent.event;
          button = kbevent.button;
          kb = kbevent.key;
          xpo = kbevent.x;
          ypo = kbevent.y;
          switch ( event ) {
              case 0: // pointer movement
//     case 2:  // button release
              case 3: // button press and movement
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  draw_box_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
                  draw_po_cursor ( G ) ;
              }
              break;
              case 1: // button press
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  draw_box_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
                  draw_po_cursor ( G ) ;
              }
              if ( button == 1 ) key = '\r';
              if ( button == 3 ) key = '.';
              OK = 1;
              continue;
              case 5: // key release
              key = kb;
              if ( ui_Linefeed ( kb ) ) { key = '\r'; OK = 1; continue; }
              if ( ui_Return ( kb ) ) {key = '\r'; OK = 1; continue; }
//       if(isdigit(kb)) {jmp = kb-'0';continue;}
              if ( ( kb >= '0' ) && ( kb <= '9' ) ) {dc->jmp = kb-'0'; continue; }
              if ( ( kb == '.' ) || ( kb == 'u' ) || ( kb == 'U' ) ) {
                  OK = 1; continue;
              }
              if ( ui_Uparrow ( kb ) ) { cursor_up ( dc ) ; draw_box_cursor \
               ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Downarrow ( kb ) ) { cursor_down ( dc ) ; draw_box_cursor \
               ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Leftarrow ( kb ) ) { cursor_left ( dc ) ; draw_box_cursor \
               ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Rightarrow ( kb ) ) { cursor_right ( dc ) ; draw_box_cursor \
               ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ; break; }
              break;
              default:
              break;
          }
      }
      jump:
      dc->c_bound = temp;
      uiset_clr ( D , c_color ) ;
      if ( dc->entry ) {
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
      };
      uiScrn_recover ( wc ) ;
//  ChangeCursor(59);
      *xx = uiusr_x ( dc->gcur_x ) ;
      *yy = uiusr_y ( ( D->evgay-dc->gcur_y ) ) ;
      if ( MAG ) {
          kgAntialiasingOn ( G , MAG-1 ) ;
      }
      return ( key ) ;
  }
  char kgRectCursor ( DIG *G , float *xx , float *yy , \
       float *xbgn , float *ybgn ) \
  {
      KBEVENT kbevent;
      int kb , button , event , OK = 0;
      int ch , key , MAG = 0;
      int xpo , ypo , but , pointer , xorg , yorg;
      int temp;
      DIALOG *D;
      D = ( DIALOG * ) G->D;
      kgWC *wc;
      kgDC *dc;
      wc = G->wc;
      dc = G->dc;
      if ( G->D_ON == 0 ) {
          MAG = G->MAG;
          kgAntialiasingOff ( G ) ;
      }
      temp = WC ( D )->c_color;
      uiset_clr ( D , 8 ) ;
      dc->entry = 0;
      dc->gcur_x = uiscr_x ( *xx ) ;
      dc->gcur_y = D->evgay-uiscr_y ( *yy ) ;
      xorg = uiscr_x ( *xbgn ) ;
      yorg = D->evgay-uiscr_y ( *ybgn ) ;
      uiScrn_back ( wc , dc->msg_x , dc->msg_y , 34 ) ;
      xpo = xorg , ypo = yorg;
//  gbell();
//  ChangeCursor(35);
      draw_cursor ( G ) ;
      draw_rect_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
      while ( ! OK ) {
//    kbevent = ui_getevent();
          kbevent = kgSkipMouseMove ( D ) ;
          event = kbevent.event;
          button = kbevent.button;
          kb = kbevent.key;
          xpo = kbevent.x;
          ypo = kbevent.y;
          switch ( event ) {
              case 0: // pointer movement
//     case 2:  // button release
              case 3: // button press and movement
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  draw_rect_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
                  draw_po_cursor ( G ) ;
              }
              break;
              case 1: // button press
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  draw_rect_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
                  draw_po_cursor ( G ) ;
              }
              if ( button == 1 ) key = '\r';
              if ( button == 3 ) key = '.';
              OK = 1;
              continue;
              case 5: // key release
              key = kb;
              if ( ui_Linefeed ( kb ) ) { key = '\r'; OK = 1; continue; }
              if ( ui_Return ( kb ) ) {key = '\r'; OK = 1; continue; }
//       if(isdigit(kb)) {jmp = kb-'0';continue;}
              if ( ( kb >= '0' ) && ( kb <= '9' ) ) {dc->jmp = kb-'0'; continue; }
              if ( ( kb == '.' ) || ( kb == 'u' ) || ( kb == 'U' ) ) {
                  OK = 1; continue;
              }
              if ( ui_Uparrow ( kb ) ) { cursor_up ( dc ) ; draw_rect_cursor \
               ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Downarrow ( kb ) ) { cursor_down ( dc ) ; draw_rect_cursor \
               ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Leftarrow ( kb ) ) { cursor_left ( dc ) ; draw_rect_cursor \
               ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Rightarrow ( kb ) ) { cursor_right ( dc ) ; draw_rect_cursor \
               ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ; break; }
              break;
              default:
              break;
          }
      }
      jump:
      uiset_clr ( D , temp ) ;
      if ( dc->entry ) {
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
      };
      uiScrn_recover ( wc ) ;
//  ChangeCursor(59);
      *xx = uiusr_x ( dc->gcur_x ) ;
      *yy = uiusr_y ( ( D->evgay-dc->gcur_y ) ) ;
      if ( MAG ) {
          kgAntialiasingOn ( G , MAG-1 ) ;
      }
      return ( key ) ;
  }
#if 0
#endif
  int draw_cross_cursor ( DIG *G , int x1 , int y1 ) {
      int key;
      int x2 , y2;
      int xmin , xmax , ymin , ymax;
      DIALOG *D;
      kgDC *dc;
      kgWC *wc;
      wc = G->wc;
      D = G->D;
      dc = G->dc;
      x2 = dc->x2; y2 = dc->y2;
      if ( dc->entry == 0 ) {x2 = -1; y2 = -1; };
      if ( dc->entry ) {
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
      };
      dc->entry = 1;
      xmin = x1; xmax = x1+1;
      if ( xmin <= 0 ) {xmax = 1; xmin = 0; }
      if ( xmin >= ( D->evgax+1 ) ) {xmax = D->evgax+1; xmin = xmax-1; };
      ymax = y1; ymin = ymax-1;
      if ( ymax <= 0 ) {ymax = 1 , ymin = 0; }
      if ( ymax >= ( D->evgay ) ) {ymax = D->evgay; ymin = ymax-1; };
      kg_scr_back ( wc , xmin , 0 , xmax , ( D->evgay ) ) ;
      kg_scr_back ( wc , 0 , ymin , ( D->evgax ) +1 , ymax ) ;
      _uiLINE ( wc , 0 , y1 , ( D->evgax+1 ) , y1 ) ;
      _uiLINE ( wc , x1 , 0 , x1 , ( D->evgay ) ) ;
      dc->x2 = x1 , dc->y2 = y1;
      uiUpdateOn ( D ) ;
      return ( key ) ;
  }
  char kgCrossCursor ( DIG *G , float *xx , float *yy ) {
      KBEVENT kbevent;
      int kb , button , event , OK = 0;
      int ch , key , MAG = 0;
      int xpo , ypo , but , pointer , xorg , yorg;
      int temp;
      DIALOG *D;
      D = ( DIALOG * ) G->D;
      kgWC *wc;
      kgDC *dc;
      wc = G->wc;
      dc = G->dc;
      if ( G->D_ON == 0 ) {
          MAG = G->MAG;
          kgAntialiasingOff ( G ) ;
      }
      temp = wc->c_color;
      uiset_clr ( D , 8 ) ;
      dc->entry = 0;
      dc->gcur_x = uiscr_x ( *xx ) ;
      dc->gcur_y = D->evgay-uiscr_y ( *yy ) ;
      uiScrn_back ( wc , dc->msg_x , dc->msg_y , 34 ) ;
      xpo = xorg , ypo = yorg;
//  gbell();
//  ChangeCursor(35);
      draw_cursor ( G ) ;
      draw_cross_cursor ( G , dc->gcur_x , dc->gcur_y ) ;
      while ( ! OK ) {
//    kbevent = ui_getevent();
          kbevent = kgSkipMouseMove ( D ) ;
          event = kbevent.event;
          button = kbevent.button;
          kb = kbevent.key;
          xpo = kbevent.x;
          ypo = kbevent.y;
//    printf("event=%d\n",event);
          switch ( event ) {
              case 0: // pointer movement
//     case 2:  // button release
              case 3: // button press and movement
//       printf("Mouse Move %d\n",event);
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  draw_cross_cursor ( G , dc->gcur_x , dc->gcur_y ) ;
                  draw_po_cursor ( G ) ;
              }
              break;
              case 1: // button press
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  draw_cross_cursor ( G , dc->gcur_x , dc->gcur_y ) ;
                  draw_po_cursor ( G ) ;
              }
              if ( button == 1 ) key = '\r';
              if ( button == 3 ) key = '.';
              OK = 1;
              continue;
              case 5: // key release
              key = kb;
              if ( ui_Linefeed ( kb ) ) { key = '\r'; OK = 1; continue; }
              if ( ui_Return ( kb ) ) {key = '\r'; OK = 1; continue; }
//       if(isdigit(kb)) {jmp = kb-'0';continue;}
              if ( ( kb >= '0' ) && ( kb <= '9' ) ) {dc->jmp = kb-'0'; continue; }
              if ( ( kb == '.' ) || ( kb == 'u' ) || ( kb == 'U' ) ) {
                  OK = 1; continue;
              }
              if ( ui_Uparrow ( kb ) ) { cursor_up ( dc ) ; draw_cross_cursor \
               ( G , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Downarrow ( kb ) ) { cursor_down ( dc ) ; draw_cross_cursor \
               ( G , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Leftarrow ( kb ) ) { cursor_left ( dc ) ; draw_cross_cursor \
               ( G , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Rightarrow ( kb ) ) { cursor_right ( dc ) ; draw_cross_cursor \
               ( G , dc->gcur_x , dc->gcur_y ) ; break; }
              break;
              default:
              break;
          }
      }
      jump:
      if ( dc->entry ) {
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
      };
      uiScrn_recover ( wc ) ;
//  ChangeCursor(59);
      *xx = uiusr_x ( dc->gcur_x ) ;
      *yy = uiusr_y ( ( D->evgay-dc->gcur_y ) ) ;
      uiset_clr ( D , temp ) ;
      if ( MAG ) {
          kgAntialiasingOn ( G , MAG-1 ) ;
      }
      return ( key ) ;
  }
#if 0
  char cross_getpointer ( DIG *G , int *xx , int *yy ) {
      int ch , key;
      int xpo , ypo , but , pointer , xorg , yorg , gccur_x , gccur_y;
      static int entry;
      int temp;
      DIALOG *D;
      D = ( DIALOG * ) G->D;
      kgWC *wc;
      kgDC *dc;
      wc = WC ( D ) ;
      dc = G->dc;
      temp = WC ( D )->c_color;
      uiset_clr ( D , 14 ) ;
      entry = 0;
      gcur_x = ( *xx ) ;
      gcur_y = ( *yy ) ;
      uiScrn_back ( D , msg_x , msg_y , 34 ) ;
      xpo = xorg , ypo = yorg;
      while ( GetPointer ( & xpo , & ypo ) < 0 ) CALL_ATTN;
      while ( GetPointer ( & xpo , & ypo ) > 0 ) ;
      gbell ( ) ;
      ChangeCursor ( 35 ) ;
      draw_cursor ( G ) ;
      for ( ; ; ) {
          while ( ( pointer = cross_pointer ( D , & xpo , \
               & ypo , & entry ) ) < 0 ) CALL_ATTN;
          while ( ( pointer == 0 ) ) {
              if ( ( xpo != gcur_x ) || ( ypo != gcur_y ) ) {gcur_x = xpo , gcur_y = ypo;
                  draw_po_cursor ( G ) ;
                  while ( ( pointer = cross_pointer ( D , & xpo , \
                       & ypo , & entry ) ) < 0 ) CALL_ATTN;
              }
              else pointer = -1;
          }
          if ( pointer == 1 ) {gcur_x = xpo , gcur_y = ypo , key = '\r'; goto jump; };
          if ( pointer == 3 ) {gcur_x = xpo , gcur_y = ypo , key = '.'; goto jump; };
          kb.kbint = ui_get_kb ( ) ;
          ch = kb.kbc [ 0 ] ;
          key = ch;
          while ( ( ch != '\r' ) && ( kb.kbint >= 0 ) && ( ch != '\n' ) ) {
              if ( ch != '\0' ) {
                  if ( ( isdigit ( ch ) ) && ( ch != '0' ) ) jmp = ch-'0';
                  else if ( ( ch == '.' ) || ( ch == 'u' ) || ( ch == 'U' ) ) goto jump;
              }
              else {
                  ch = kb.kbc [ 1 ] ;
                  switch ( ch ) {
                      case 'H' : cursor_up ( dc ) ; break;
                      case 'P' : cursor_down ( dc ) ; break;
                      case 'K' : cursor_left ( dc ) ; break;
                      case 'M' : cursor_right ( dc ) ; break;
                      default : break;
                  }
              }
              kb.kbint = ui_get_kb ( ) ;
              ch = kb.kbc [ 0 ] ;
              key = ch;
          }
          if ( ch == '\n' ) ch = '\r';
          if ( ch == '\r' ) break;
      }
      jump:
      if ( entry == 1 ) {
          kg_scr_recover ( D ) ;
          kg_scr_recover ( D ) ;
      };
      uiScrn_recover ( D ) ;
      ChangeCursor ( 59 ) ;
      *xx = ( gcur_x ) ;
      *yy = gcur_y;
      uiset_clr ( D , temp ) ;
      rmv_pointer ( ) ;
      return ( key ) ;
  }
#endif
  char kgDblCursor ( DIG *G , float *xx , float *yy , \
       float *xbgn , float *ybgn ) \
  {
      int ch , key , c_color;
      int xpo , ypo , but , pointer , xorg , yorg;
      int temp , MAG = 0;
      KBEVENT kbevent;
      int kb , button , event , OK = 0;
      DIALOG *D;
      D = ( DIALOG * ) G->D;
      kgWC *wc;
      kgDC *dc;
      wc = G->wc;
      dc = G->dc;
      if ( G->D_ON == 0 ) {
          MAG = G->MAG;
          kgAntialiasingOff ( G ) ;
      }
      temp = WC ( D )->c_color;
      uiset_clr ( D , 8 ) ;
      dc->entry = 0;
      dc->gcur_x = uiscr_x ( *xx ) ;
      dc->gcur_y = D->evgay-uiscr_y ( *yy ) ;
      xorg = uiscr_x ( *xbgn ) ;
      yorg = D->evgay-uiscr_y ( *ybgn ) ;
      uiScrn_back ( wc , dc->msg_x , dc->msg_y , 34 ) ;
      kg_scr_back ( wc , 0 , yorg , D->evgax+1 , yorg ) ;
      kg_scr_back ( wc , xorg , 0 , xorg , D->evgay ) ;
      _uiLINE ( wc , 0 , yorg , D->evgax+1 , yorg ) ;
      _uiLINE ( wc , xorg , 0 , xorg , D->evgay ) ;
      c_color = 14;
      uiset_clr ( D , c_color ) ;
      xpo = xorg , ypo = yorg;
//  gbell();
//  ChangeCursor(35);
      draw_cursor ( G ) ;
      draw_cross_cursor ( G , dc->gcur_x , dc->gcur_y ) ;
      while ( ! OK ) {
//    kbevent = ui_getevent();
          kbevent = kgSkipMouseMove ( D ) ;
          event = kbevent.event;
          button = kbevent.button;
          kb = kbevent.key;
          xpo = kbevent.x;
          ypo = kbevent.y;
          switch ( event ) {
              case 0: // pointer movement
//     case 2:  // button release
              case 3: // button press and movement
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  draw_cross_cursor ( G , dc->gcur_x , dc->gcur_y ) ;
                  draw_po_cursor ( G ) ;
              }
              break;
              case 1: // button press
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  draw_cross_cursor ( G , dc->gcur_x , dc->gcur_y ) ;
                  draw_po_cursor ( G ) ;
              }
              if ( button == 1 ) key = '\r';
              if ( button == 3 ) key = '.';
              OK = 1;
              continue;
              case 5: // key release
              key = kb;
              if ( ui_Linefeed ( kb ) ) { key = '\r'; OK = 1; continue; }
              if ( ui_Return ( kb ) ) {key = '\r'; OK = 1; continue; }
//       if(isdigit(kb)) {jmp = kb-'0';continue;}
              if ( ( kb >= '0' ) && ( kb <= '9' ) ) {dc->jmp = kb-'0'; continue; }
              if ( ( kb == '.' ) || ( kb == 'u' ) || ( kb == 'U' ) ) {
                  OK = 1; continue;
              }
              if ( ui_Uparrow ( kb ) ) { cursor_up ( dc ) ; draw_cross_cursor \
               ( G , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Downarrow ( kb ) ) { cursor_down ( dc ) ; draw_cross_cursor \
               ( G , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Leftarrow ( kb ) ) { cursor_left ( dc ) ; draw_cross_cursor \
               ( G , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Rightarrow ( kb ) ) { cursor_right ( dc ) ; draw_cross_cursor \
               ( G , dc->gcur_x , dc->gcur_y ) ; break; }
              break;
              default:
              break;
          }
      }
      jump:
      c_color = temp;
      uiset_clr ( D , c_color ) ;
      if ( dc->entry ) {
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
      };
      kg_scr_recover ( wc ) ;
      kg_scr_recover ( wc ) ;
      uiScrn_recover ( wc ) ;
//  ChangeCursor(59);
      *xx = uiusr_x ( dc->gcur_x ) ;
      *yy = uiusr_y ( ( D->evgay-dc->gcur_y ) ) ;
      if ( MAG ) {
          kgAntialiasingOn ( G , MAG-1 ) ;
      }
      return ( key ) ;
  }
#if 0
#endif
  int draw_rbr_cursor ( DIG *G , int xorg , int yorg , int x1 , int y1 ) {
      int key = 1;
      int x2 , y2;
      int xmin , ymin , xmax , ymax;
      DIALOG *D;
      kgDC *dc;
      kgWC *wc;
      D = G->D;
      dc = G->dc;
      wc = G->wc;
      x2 = dc->x2;
      y2 = dc->y2;
      if ( dc->entry == 0 ) {x2 = -100; y2 = -100; }
      if ( dc->entry ) {
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
          _uirecover_line ( wc , xorg , yorg , dc->xrbr , dc->yrbr ) ;
      };
      dc->entry = 1;
      xmin = x1; xmax = x1+1;
      if ( xmin <= 0 ) xmax = 1;
      if ( xmin >= ( D->evgax+1 ) ) {xmax = x1; xmin = xmax-1; };
      ymin = y1; ymax = y1+1;
      if ( ymin <= 0 ) ymax = 1;
      if ( ymin >= ( D->evgay ) ) {ymax = y1; ymin = ymax-1; };
      _uibackup_line ( wc , xorg , yorg , x1 , y1 ) ;
      kg_scr_back ( wc , xmin , 0 , xmax , ( D->evgay ) ) ;
      kg_scr_back ( wc , 0 , ymin , ( D->evgax+1 ) , ymax ) ;
      _uiLINE ( wc , xorg , yorg , x1 , y1 ) ;
      _uiLINE ( wc , 0 , y1 , ( D->evgax+1 ) , y1 ) ;
      _uiLINE ( wc , x1 , 0 , x1 , ( D->evgay ) ) ;
      dc->x2 = x1 , dc->y2 = y1;
      dc->xrbr = x1 , dc->yrbr = y1;
      uiUpdateOn ( D ) ;
      return ( key ) ;
  }
  char kgRbrCursor ( DIG *G , float *xx , float *yy , \
       float *xbgn , float *ybgn ) \
  {
      KBEVENT kbevent;
      int kb , button , event , OK = 0 , c_color;
      int ch , key , MAG = 0;
      int xpo , ypo , but , pointer , xorg , yorg;
      int temp;
      DIALOG *D;
      D = ( DIALOG * ) G->D;
      kgWC *wc;
      kgDC *dc;
      wc = G->wc;
      dc = G->dc;
      if ( G->D_ON == 0 ) {
          MAG = G->MAG;
          kgAntialiasingOff ( G ) ;
      }
      temp = WC ( D )->c_color;
      uiset_clr ( D , 8 ) ;
      dc->entry = 0;
      dc->gcur_x = uiscr_x ( *xx ) ;
      dc->gcur_y = ( D->evgay ) -uiscr_y ( *yy ) ;
      xorg = uiscr_x ( *xbgn ) ;
      yorg = ( D->evgay ) -uiscr_y ( *ybgn ) ;
      uiScrn_back ( wc , dc->msg_x , dc->msg_y , 34 ) ;
      kg_scr_back ( wc , 0 , yorg , ( D->evgax+1 ) , yorg ) ;
      kg_scr_back ( wc , xorg , 0 , xorg , ( D->evgay ) ) ;
      _uiLINE ( wc , 0 , yorg , ( D->evgax+1 ) , yorg ) ;
      _uiLINE ( wc , xorg , 0 , xorg , ( D->evgay ) ) ;
      c_color = 14;
      uiset_clr ( D , c_color ) ;
      xpo = xorg , ypo = yorg;
//  gbell();
//  ChangeCursor(35);
      draw_cursor ( G ) ;
      draw_rbr_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
      while ( ! OK ) {
//    kbevent = ui_getevent();
          kbevent = kgSkipMouseMove ( D ) ;
          event = kbevent.event;
          button = kbevent.button;
          kb = kbevent.key;
          xpo = kbevent.x;
          ypo = kbevent.y;
          switch ( event ) {
              case 0: // pointer movement
//     case 2:  // button release
              case 3: // button press and movement
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  draw_rbr_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
                  draw_po_cursor ( G ) ;
              }
              break;
              case 1: // button press
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  draw_rbr_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
                  draw_po_cursor ( G ) ;
              }
              if ( button == 1 ) key = '\r';
              if ( button == 3 ) key = '.';
              OK = 1;
              continue;
              case 5: // key release
              key = kb;
              if ( ui_Linefeed ( kb ) ) { key = '\r'; OK = 1; continue; }
              if ( ui_Return ( kb ) ) {key = '\r'; OK = 1; continue; }
//       if(isdigit(kb)) {jmp = kb-'0';continue;}
              if ( ( kb >= '0' ) && ( kb <= '9' ) ) {dc->jmp = kb-'0'; continue; }
              if ( ( kb == '.' ) || ( kb == 'u' ) || ( kb == 'U' ) ) {
                  OK = 1; continue;
              }
              if ( ui_Uparrow ( kb ) ) { cursor_up ( dc ) ; draw_rbr_cursor \
               ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Downarrow ( kb ) ) { cursor_down ( dc ) ; draw_rbr_cursor \
               ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Leftarrow ( kb ) ) { cursor_left ( dc ) ; draw_rbr_cursor \
               ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ; break; }
              if ( ui_Rightarrow ( kb ) ) { cursor_right ( dc ) ; draw_rbr_cursor \
               ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ; break; }
              break;
              default:
              break;
          }
      }
      jump:
      c_color = temp;
      uiset_clr ( D , c_color ) ;
      if ( dc->entry ) {
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
          _uirecover_line ( wc , xorg , yorg , dc->xrbr , dc->yrbr ) ;
      };
      kg_scr_recover ( wc ) ;
      kg_scr_recover ( wc ) ;
      uiScrn_recover ( wc ) ;
//  ChangeCursor(59);
      *xx = uiusr_x ( dc->gcur_x ) ;
      *yy = uiusr_y ( ( ( D->evgay ) -dc->gcur_y ) ) ;
      uiset_clr ( D , temp ) ;
      if ( MAG ) {
          kgAntialiasingOn ( G , MAG-1 ) ;
      }
      return ( key ) ;
  }
#if 0
#endif
  char kgArcCursor ( DIG *G , float *xx , float *yy , \
       float *xbgn , float *ybgn ) \
  {
      KBEVENT kbevent;
      int kb , button , event , OK = 0;
      int ch , key , c_color;
      int xpo , ypo , but , pointer , xorg , yorg;
      int gcurx , gcury;
      int msg;
      int temp , MAG = 0;
      DIALOG *D;
      D = ( DIALOG * ) G->D;
      kgWC *wc;
      kgDC *dc;
      wc = G->wc;
      dc = G->dc;
      if ( G->D_ON == 0 ) {
          MAG = G->MAG;
          kgAntialiasingOff ( G ) ;
      }
      temp = WC ( D )->c_color;
      c_color = 8;
      uiset_clr ( D , c_color ) ;
      dc->entry = 0;
      dc->gcur_x = uiscr_x ( *xx ) ;
      dc->gcur_y = ( D->evgay ) -uiscr_y ( *yy ) ;
      xorg = uiscr_x ( *xbgn ) ;
      yorg = ( D->evgay ) -uiscr_y ( *ybgn ) ;
      dc->gcur_r = sqrt ( ( double ) ( ( dc->gcur_x-xorg ) * ( dc->gcur_x-xorg ) + \
       ( dc->gcur_y-yorg ) * ( dc->gcur_y-yorg ) ) ) ;
      uiScrn_back ( wc , dc->msg_x , dc->msg_y , 34 ) ;
      kg_scr_back ( wc , 0 , yorg , ( D->evgax+1 ) , yorg ) ;
      kg_scr_back ( wc , xorg , 0 , xorg , ( D->evgay ) ) ;
      _uiLINE ( wc , 0 , yorg , ( D->evgax+1 ) , yorg ) ;
      _uiLINE ( wc , xorg , 0 , xorg , ( D->evgay ) ) ;
      c_color = 14;
      uiset_clr ( D , c_color ) ;
      xpo = dc->gcur_x , ypo = dc->gcur_y;
      gcurx = dc->gcur_x; gcury = dc->gcur_y;
      limit_cur ( G , & gcurx , & gcury , xorg , yorg ) ;
      dc->gcur_x = gcurx , dc->gcur_y = gcury;
//  uiPositionPointer(wc,dc->gcur_x,dc->gcur_y);
      draw_rbr_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
      draw_arc_cursor ( G ) ;
      while ( ! OK ) {
//    kbevent = ui_getevent();
          kbevent = kgSkipMouseMove ( D ) ;
          event = kbevent.event;
          button = kbevent.button;
          kb = kbevent.key;
          xpo = kbevent.x;
          ypo = kbevent.y;
          switch ( event ) {
              case 0: // pointer movement
//     case 2:  // button release
              case 3: // button press and movement
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  gcurx = dc->gcur_x; gcury = dc->gcur_y;
                  if ( limit_cur ( G , & gcurx , & gcury , xorg , yorg ) < 0 ) break;
                  dc->gcur_x = gcurx , dc->gcur_y = gcury;
//  uiPositionPointer(wc,dc->gcur_x,dc->gcur_y);
                  draw_rbr_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
                  draw_arc_cursor ( G ) ;
              }
              break;
              case 1: // button press
              if ( ( xpo != dc->gcur_x ) || ( ypo != dc->gcur_y ) ) {
                  dc->gcur_x = xpo , dc->gcur_y = ypo;
                  gcurx = dc->gcur_x; gcury = dc->gcur_y;
                  if ( limit_cur ( G , & gcurx , & gcury , xorg , yorg ) < 0 ) break;
                  dc->gcur_x = gcurx , dc->gcur_y = gcury;
//  uiPositionPointer(wc,dc->gcur_x,dc->gcur_y);
                  draw_rbr_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
                  draw_arc_cursor ( G ) ;
              }
              if ( button == 1 ) key = '\r';
              if ( button == 3 ) key = '.';
              OK = 1;
              continue;
              case 5: // key release
              key = kb;
              if ( ui_Linefeed ( kb ) ) { key = '\r'; OK = 1; continue; }
              if ( ui_Return ( kb ) ) {key = '\r'; OK = 1; continue; }
//       if(isdigit(kb)) {jmp = kb-'0';continue;}
              if ( ( kb >= '0' ) && ( kb <= '9' ) ) {dc->jmp = kb-'0'; continue; }
              if ( ( kb == '.' ) || ( kb == 'u' ) || ( kb == 'U' ) ) {
                  OK = 1; continue;
              }
              if ( ui_Uparrow ( kb ) ) {
                  gcurx = dc->gcur_x; gcury = dc->gcur_y-dc->jmp;
                  if ( limit_cur ( G , & gcurx , & gcury , xorg , yorg ) < 0 ) break;
                  dc->gcur_x = gcurx , dc->gcur_y = gcury;
//  uiPositionPointer(wc,dc->gcur_x,dc->gcur_y);
                  draw_rbr_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
                  draw_arc_cursor ( G ) ;
              }
              if ( ui_Downarrow ( kb ) ) {
                  gcurx = dc->gcur_x; gcury = dc->gcur_y+dc->jmp;
                  if ( limit_cur ( G , & gcurx , & gcury , xorg , yorg ) < 0 ) break;
                  dc->gcur_x = gcurx , dc->gcur_y = gcury;
//  uiPositionPointer(wc,dc->gcur_x,dc->gcur_y);
                  draw_rbr_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
                  draw_arc_cursor ( G ) ;
              }
              if ( ui_Leftarrow ( kb ) ) {
                  gcurx = dc->gcur_x-dc->jmp; gcury = dc->gcur_y;
                  if ( limit_cur ( G , & gcurx , & gcury , xorg , yorg ) < 0 ) break;
                  dc->gcur_x = gcurx , dc->gcur_y = gcury;
//  uiPositionPointer(wc,dc->gcur_x,dc->gcur_y);
                  draw_rbr_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
                  draw_arc_cursor ( G ) ;
              }
              if ( ui_Rightarrow ( kb ) ) {
                  gcurx = dc->gcur_x+dc->jmp; gcury = dc->gcur_y;
                  if ( limit_cur ( G , & gcurx , & gcury , xorg , yorg ) < 0 ) break;
                  dc->gcur_x = gcurx , dc->gcur_y = gcury;
//  uiPositionPointer(wc,dc->gcur_x,dc->gcur_y);
                  draw_rbr_cursor ( G , xorg , yorg , dc->gcur_x , dc->gcur_y ) ;
                  draw_arc_cursor ( G ) ;
              }
              break;
              default:
              break;
          }
      }
      jump:
      c_color = temp;
      uiset_clr ( D , c_color ) ;
#if 1
      if ( dc->entry ) {
          kg_scr_recover ( wc ) ;
          kg_scr_recover ( wc ) ;
          _uirecover_line ( wc , xorg , yorg , dc->xrbr , dc->yrbr ) ;
      };
      kg_scr_recover ( wc ) ;
      kg_scr_recover ( wc ) ;
      uiScrn_recover ( wc ) ;
#endif
//  ChangeCursor(59);
      *xx = uiusr_x ( dc->gcur_x ) ;
      *yy = uiusr_y ( ( ( D->evgay ) -dc->gcur_y ) ) ;
      if ( MAG ) {
          kgAntialiasingOn ( G , MAG-1 ) ;
      }
      return ( key ) ;
  }
#if 0
#endif
  int BringPointer ( int x , int y ) {
#if 0
      int xx , yy;
      if ( GetPointer ( & xx , & yy ) < 0 ) return ( 0 ) ;
      Position_Pointer ( x , y ) ;
#endif
      return ( 1 ) ;
  }
  void ui_DRAW_LINE ( DIG *G , float x1 , float y1 , float x2 , float y2 ) {
      int xa , ya , xb , yb , EVGAY;
      int tempc;
      kgDC *dc;
      kgWC *wc;
      dc = G->dc;
      wc = G->wc;
      EVGAY = wc->EVGAY;
      xa = uiscr_x ( x1 ) ;
      ya = uiscr_y ( y1 ) ;
      xb = uiscr_x ( x2 ) ;
      yb = uiscr_y ( y2 ) ;
      tempc = wc->c_color;
      wcset_clr ( wc , dc->ln_color ) ;
  /*XDraw((short)xa,(short)ya,(short)xb,(short)yb);*/
      _uiLINE ( wc , xa , ( EVGAY-ya ) , xb , ( EVGAY-yb ) ) ;
      wcset_clr ( wc , tempc ) ;
  }
  void _uirect_fill ( kgWC *wc , int x1 , int y1 , \
       int x2 , int y2 , unsigned int color ) \
  {
      short x_min , y_min , x_max , x_max1 , x_min1 , y_max , i , j , po1;
      unsigned int temp;
      DIALOG *D;
      D = wc->D;
      temp = WC ( D )->c_color;
      uiset_clr ( D , color ) ;
      if ( x1 < x2 ) { x_min = x1; x_max1 = x2; }
      else{ x_min = x2; x_max1 = x1; }
      if ( ( x_min <= ( D->evgax+1 ) ) && ( x_max1 >= 0 ) ) {
          if ( y1 < y2 ) { y_min = y1; y_max = y2; }
          else{ y_min = y2; y_max = y1; }
          if ( ( y_min <= ( D->evgay ) ) && ( y_max >= 0 ) ) {
              if ( x_max1 > ( D->evgax+1 ) ) x_max1 = ( D->evgax+1 ) ;
              if ( x_min < 0 ) x_min = 0;
              if ( y_max > ( D->evgay ) ) y_max = ( D->evgay ) ;
              if ( y_min < 0 ) y_min = 0;
              _uiBox_Fill ( wc , x_min , ( D->evgay ) -y_max , x_max1 , \
               ( D->evgay ) -y_min ) ;
          }
      }
      uiset_clr ( D , temp ) ;
      return;
  }
  void _dvrect_fill ( kgWC *wc , int x1 , int y1 , \
       int x2 , int y2 , unsigned int color ) \
  {
      short x_min , y_min , x_max , x_max1 , x_min1 , y_max , i , j , po1;
      unsigned int temp;
      DIALOG *D;
      D = wc->D;
      temp = WC ( D )->c_color;
      uiset_clr ( D , color ) ;
      if ( x1 < x2 ) { x_min = x1; x_max1 = x2; }
      else{ x_min = x2; x_max1 = x1; }
      if ( ( x_min < ( D->evgax ) ) && ( x_max1 >= 0 ) ) {
          if ( y1 < y2 ) { y_min = y1; y_max = y2; }
          else{ y_min = y2; y_max = y1; }
          if ( ( y_min < ( D->evgay ) ) && ( y_max >= 0 ) ) {
              if ( x_max1 > ( D->evgax-1 ) ) x_max1 = ( D->evgax-1 ) ;
              if ( x_min < 0 ) x_min = 0;
              if ( y_max > ( D->evgay-1 ) ) y_max = ( D->evgay-1 ) ;
              if ( y_min < 0 ) y_min = 0;
              _uiBox_Fill ( wc , x_min , y_min , x_max1 , y_max ) ;
          }
      }
      uiset_clr ( D , temp ) ;
      return;
  }
  void _uirect_fill_transparent ( kgWC *wc , int x1 , int y1 , int x2 , \
       int y2 , unsigned int color , float transparency ) \
  {
      short x_min , y_min , x_max , x_max1 , x_min1 , y_max , i , j , po1;
      unsigned int temp;
      DIALOG *D;
      D = wc->D;
// As yon 22nd June 2015
      if ( ( D->NoWinMngr == 1 ) && ( D->transparency == 1.0 ) ) return;
//End
      temp = WC ( D )->c_color;
      uiset_clr ( D , color ) ;
      if ( x1 < x2 ) { x_min = x1; x_max1 = x2; }
      else{ x_min = x2; x_max1 = x1; }
      if ( ( x_min <= ( D->evgax+1 ) ) && ( x_max1 >= 0 ) ) {
          if ( y1 < y2 ) { y_min = y1; y_max = y2; }
          else{ y_min = y2; y_max = y1; }
          if ( ( y_min <= ( D->evgay ) ) && ( y_max >= 0 ) ) {
              if ( x_max1 > ( D->evgax+1 ) ) x_max1 = ( D->evgax+1 ) ;
              if ( x_min < 0 ) x_min = 0;
              if ( y_max > ( D->evgay ) ) y_max = ( D->evgay ) ;
              if ( y_min < 0 ) y_min = 0;
              _dvBox_Fill_Transparent ( wc , x_min , ( D->evgay ) -y_max , x_max1 , \
               ( D->evgay ) -y_min , transparency ) ;
          }
      }
      uiset_clr ( D , temp ) ;
      return;
  }
  void _dvrect_fill_transparent ( kgWC *wc , int x1 , int y1 , int x2 , \
       int y2 , unsigned int color , float transparency ) \
  {
      short x_min , y_min , x_max , x_max1 , x_min1 , y_max , i , j , po1;
      unsigned int temp;
      DIALOG *D;
      D = wc->D;
      temp = WC ( D )->c_color;
      uiset_clr ( D , color ) ;
// As yon 3rdMay 2014
//  if((D->parent != NULL) &&(D->transparency==1.0)) return;
// Modified on 3rd July 2015
      if ( ( D->parent != NULL ) && ( transparency == 1.0 ) ) return;
// As yon 22nd June 2015
      if ( ( D->NoWinMngr == 1 ) && ( transparency == 1.0 ) ) return;
//End
      if ( x1 < x2 ) { x_min = x1; x_max1 = x2; }
      else{ x_min = x2; x_max1 = x1; }
      if ( ( x_min <= ( D->evgax ) ) && ( x_max1 >= 0 ) ) {
          if ( y1 < y2 ) { y_min = y1; y_max = y2; }
          else{ y_min = y2; y_max = y1; }
          if ( ( y_min <= ( D->evgay ) ) && ( y_max >= 0 ) ) {
              if ( x_max1 > ( D->evgax ) ) x_max1 = ( D->evgax ) ;
              if ( x_min < 0 ) x_min = 0;
              if ( y_max > ( D->evgay ) ) y_max = ( D->evgay ) ;
              if ( y_min < 0 ) y_min = 0;
              _dvBox_Fill_Transparent ( wc , x_min , y_min , \
                   x_max1 , y_max , transparency ) ;
          }
      }
      uiset_clr ( D , temp ) ;
      return;
  }
  void uiMenuString ( DIALOG *D , char *Str , int x , int y , int width , \
       int height , int font , int color , int FontSize , int status ) \
  {
      void * fid;
      int ln , i , maxchar , temp , justification = -1;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length;
      float fac , th , tw , BxSize , width1 , xp;
      kgWC *wc;
      wc = D->wc;
      ln = width;
      if ( Str == NULL ) return;
//   if(Str[0]=='\0') return;
      fid = kgInitImage ( width , height , RESIZE ) ;
      BxSize = FontSize*2;
      BxSize = FontSize*1.6;
      xp = height*0.5-FontSize*0.5;
      width1 = width-1.5*BxSize;
      if ( fid != NULL ) {
          kgUserFrame ( fid , 0. , 0. , ( float ) width , ( float ) height ) ;
          kgTextFont ( fid , font ) ;
          th = ( float ) height*.5;
          tw = ( float ) FontSize;
          th = HFAC*tw;
          if ( th > 0.6*height ) th = 0.6*height;
          kgTextSize ( fid , th , tw , GAP*tw ) ;
          length = kgStringLength ( fid , Str ) ;
          if ( length > width1 ) {
              fac = width1/length;
              kgTextSize ( fid , th , tw*fac , GAP*tw ) ;
              length = kgStringLength ( fid , Str ) ;
          }
          kgTextColor ( fid , color ) ;
          switch ( justification ) {
              case -1:
              kgMove2f ( fid , 0.0 , xp ) ;
              break;
              case 0:
              kgMove2f ( fid , ( width1-length ) *.5 , xp ) ;
              break;
              case 1:
              kgMove2f ( fid , ( width1-length-tw*0.5 ) , xp ) ;
              break;
              default:
              kgMove2f ( fid , ( width1-length ) *.5 , xp ) ;
              break;
          }
          kgWriteText ( fid , Str ) ;
          if ( status >= 0 ) {
              width1 = width-1.25*BxSize;
              kgLineColor ( fid , color ) ;
              kgMove2f ( fid , width1 , xp-0.2*BxSize ) ;
              kgDraw2f ( fid , width1 , xp+0.8*BxSize ) ;
              kgDraw2f ( fid , width1+BxSize , xp+0.8*BxSize ) ;
              kgDraw2f ( fid , width1+BxSize , xp-0.2*BxSize ) ;
              kgDraw2f ( fid , width1 , xp-0.2*BxSize ) ;
              if ( status > 0 ) {
                  kgMove2f ( fid , width1+0.2*FontSize , xp+0.1*BxSize ) ;
                  kgTextSize ( fid , th , tw , GAP*tw ) ;
                  kgWriteText ( fid , "!f354" ) ;
              }
          }
//      img=kgGetResizedImage(fid);
          img = kgGetSharpImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      if ( img != NULL ) {
          kgImage ( D , img , x , y , ln , height , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
      }
      return ;
  }
  void *uiMenuStringImage ( DIALOG *D , char *Str , int width , int height , \
       int font , int color , int FontSize , int justification ) \
  {
      void * fid;
      int ln , i , maxchar , temp;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length;
      float fac , th , tw , BxSize , width1 , yp , xp;
      kgWC *wc;
      wc = D->wc;
      ln = width;
      if ( Str == NULL ) return NULL;
      BxSize = FontSize*1.6;
      if ( BxSize > ( height ) ) {
          BxSize = height;
          FontSize = ( int ) ( ( float ) BxSize/1.6+0.5 ) ;
      }
      fid = kgInitImage ( width , height , RESIZE ) ;
      yp = height*0.5-FontSize*0.5;
      width1 = width-FontSize;
      xp = FontSize*0.5;
      if ( fid != NULL ) {
          kgUserFrame ( fid , 0. , 0. , ( float ) width , ( float ) height ) ;
          kgTextFont ( fid , font ) ;
          th = ( float ) height*.5;
          tw = ( float ) FontSize;
          th = HFAC*tw;
          if ( th > 0.6*height ) th = 0.6*height;
          kgTextSize ( fid , th , tw , GAP*tw ) ;
          length = kgStringLength ( fid , Str ) ;
          if ( length > width1 ) {
              fac = width1/length;
              kgTextSize ( fid , th , tw*fac , GAP*tw ) ;
              length = kgStringLength ( fid , Str ) ;
          }
          kgTextColor ( fid , color ) ;
          kgLineColor ( fid , color ) ;
          switch ( justification ) {
              case -1:
              kgMove2f ( fid , xp , yp ) ;
              break;
              case 0:
              kgMove2f ( fid , xp+ ( width1-length ) *.5 , yp ) ;
              break;
              case 1:
              kgMove2f ( fid , xp+ ( width1-length-tw*0.5 ) , yp ) ;
              break;
              default:
              kgMove2f ( fid , xp+ ( width1-length ) *.5 , yp ) ;
              break;
          }
          kgWriteText ( fid , Str ) ;
//      img=kgGetResizedImage(fid);
          img = kgGetSharpImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      return img;
  }
  char *uiCleanOldString ( char *Str , char *Buf , int *cval , \
       int *fval , float *wfac , float *zfac ) {
      int i , k;
      char *pt = Str;
//      printf("Str: %s\n",Str);
//      fflush(stdout);
      while ( Str [ 0 ] == '!' ) {
          switch ( Str [ 1 ] ) {
              case 'f':
              *fval = ( Str [ 2 ] -'0' ) *10+ ( Str [ 3 ] -'0' ) ;
              break;
              case 'c':
              *cval = ( Str [ 2 ] -'0' ) *10+ ( Str [ 3 ] -'0' ) ;
              break;
              case 'w':
              *wfac = ( float ) ( Str [ 2 ] -'0' ) / ( Str [ 3 ] -'0' ) ;
              break;
              case 'z':
              *zfac = ( float ) ( Str [ 2 ] -'0' ) / ( Str [ 3 ] -'0' ) ;
              break;
              case 'h':
              break;
              case 'S':
              case 's':
              Str += 2;
              continue;
              default:
              Str += 2;
              continue;
          }
          Str += 4;
      }
      i = 0;
      k = 0;
      Str = pt;
      while ( Str [ i ] != '\0' ) {
          if ( Str [ i ] == '!' ) {
              switch ( Str [ i+1 ] ) {
                  case 'f':
                  case 'c':
                  case 'w':
                  case 'z':
                  case 'h':
                  i = i+4;
                  break;
                  case 'S':
                  i = i+2;
                  break;
                  case 's':
                  i = i+2;
                  break;
                  case '!':
                  Buf [ k ] = '!';
                  k++;i+= 2;
                  break;
                  default:
                  i+= 2;
                  break;
              }
          }
          else {
              Buf [ k++ ] = Str [ i++ ] ;
              Buf [ k ] = '\0';
//      printf("Buf: %s\n",Buf);
//      fflush(stdout);
          }
      }
      Buf [ k ] = '\0';
//      printf("Buf: %s\n",Buf);
//      fflush(stdout);
      return Buf;
  }
  void **uiMenuStringImages ( DIALOG *D , char **Strs , int width , int height , \
       int font , int color , int FontSize , int justification , int Mag ) \
  {
      void * fid;
      void **imgs;
      char *Str;
      char Buf [ 1000 ] ;
      int ln , i , maxchar , temp , item;
      void *img = NULL , *imgbk = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length;
      float fac , th , tw , BxSize , width1 , yp , xp;
      int x1 , ln1;
      FONT_STR F;
      IMG_STR *IMG;
      int rd , gr , bl;
      int fval , cval;
      float wfac , zfac;
      int Fz = FontSize;
      kgWC *wc;
      wc = D->wc;
      ln = width;
      if ( Strs == NULL ) return NULL;
      item = 0;
      while ( Strs [ item ] != NULL ) item++;
      if ( item == 0 ) return NULL;
      imgs = ( void ** ) Malloc ( sizeof ( void * ) * ( item+1 ) ) ;
      for ( i = 0; i <= item; i++ ) imgs [ i ] = NULL;
      BxSize = FontSize*2.0;
      if ( BxSize > ( height ) ) {
          BxSize = height;
          FontSize = ( int ) ( ( float ) BxSize/2.0 ) ;
      }
//TCB
      yp = height*0.5-FontSize*0.5;
      width1 = width-FontSize;
      xp = FontSize*0.5;
#if 0
#else
      for ( i = 0; i < item; i++ ) {
          Str = Strs [ i ] ;
          wfac = 1.0;
          cval = color;
          fval = font;
          Str = uiCleanOldString ( Str , Buf , & cval , & fval , & wfac , & zfac ) ;
          kgGetDefaultRGB ( cval , & rd , & gr , & bl ) ;
          F.code = 't';
          F.name = kgGetOthFont ( fval ) ;
          F.Imgs = Mimgs;
          if ( FontSize <= 0 ) F.Size = ( height-2 ) /2;
          else F.Size = FontSize;
          if ( fval != font ) F.code = 'f';
          if ( F.Size != FontSize ) F.code = 'f';
          if ( wfac != 1.0 ) F.code = 'f';
          if ( F.Imgs == NULL ) F.code = 'i';
          IMG = uiMakeString ( & ( F ) , Buf , ( int ) height , 0 ) ;
          if ( IMG->xln > width-F.Size ) {
              float fac = ( float ) ( width-F.Size ) /IMG->xln;
//            img = kgResizeImage(IMG->img,fac);
              img = kgChangeSizeImage ( IMG->img , ( int ) ( width-F.Size ) , height ) ;
              kgFreeImage ( IMG->img ) ;
              IMG->img = img;
              IMG->xln = IMG->xln*fac;
          }
          kgSetImageColor ( IMG->img , rd , gr , bl ) ;
          imgbk = kgCreateImage ( width , height ) ;
          x1 = F.Size/2;
          if ( justification == 1 ) x1 = ( width-IMG->xln-2 ) ;
          else if ( justification == 0 ) x1 = ( width-IMG->xln ) /2;
          ln1 = IMG->xln+1;
          img = IMG->img;
          if ( F.name != NULL ) free ( F.name ) ;
          if ( img != NULL ) {
              kgAddImages ( imgbk , img , x1 , 0 ) ;
              uiFreeImage ( img ) ;
              free ( IMG ) ;
          }
          imgs [ i ] = imgbk;
      }
#endif
      return imgs;
  }
  void **orguiMenuStringImages ( DIALOG *D , char **Strs , int width , int height , \
       int font , int color , int FontSize , int justification , int Mag ) \
  {
      void * fid;
      void **imgs;
      char *Str;
      int ln , i , maxchar , temp , item;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length;
      float fac , th , tw , BxSize , width1 , yp , xp;
      kgWC *wc;
      wc = D->wc;
      ln = width;
      if ( Strs == NULL ) return NULL;
      item = 0;
      while ( Strs [ item ] != NULL ) item++;
      if ( item == 0 ) return NULL;
      imgs = ( void ** ) Malloc ( sizeof ( void * ) * ( item+1 ) ) ;
      for ( i = 0; i <= item; i++ ) imgs [ i ] = NULL;
      BxSize = FontSize*1.6;
      if ( BxSize > ( height ) ) {
          BxSize = height;
          FontSize = ( int ) ( ( float ) BxSize/1.6+0.5 ) ;
      }
//TCB
      fid = kgInitImage ( width , height , Mag ) ;
      yp = height*0.5-FontSize*0.5;
      width1 = width-FontSize;
      xp = FontSize*0.5;
      if ( fid != NULL ) {
          kgUserFrame ( fid , 0. , 0. , ( float ) width , ( float ) height ) ;
          kgTextFont ( fid , font ) ;
          for ( i = 0; i < item; i++ ) {
              kgClearView ( fid ) ;
              Str = Strs [ i ] ;
              th = ( float ) height*.5;
              tw = ( float ) FontSize;
              th = HFAC*tw;
              if ( th > 0.6*height ) th = 0.6*height;
              kgTextSize ( fid , th , tw , GAP*tw ) ;
              length = kgStringLength ( fid , Str ) ;
              if ( length > width1 ) {
                  fac = width1/length;
                  kgTextSize ( fid , th , tw*fac , GAP*tw ) ;
                  length = kgStringLength ( fid , Str ) ;
              }
              kgTextColor ( fid , color ) ;
              kgLineColor ( fid , color ) ;
              switch ( justification ) {
                  case -1:
                  kgMove2f ( fid , xp , yp ) ;
                  break;
                  case 0:
                  kgMove2f ( fid , xp+ ( width1-length ) *.5 , yp ) ;
                  break;
                  case 1:
                  kgMove2f ( fid , xp+ ( width1-length-tw*0.5 ) , yp ) ;
                  break;
                  default:
                  kgMove2f ( fid , xp+ ( width1-length ) *.5 , yp ) ;
                  break;
              }
              kgWriteText ( fid , Str ) ;
//        imgs[i]=kgGetResizedImage(fid);
              imgs [ i ] = kgGetSharpImage ( fid ) ;
          }
          kgCloseImage ( fid ) ;
      }
      return imgs;
  }
  void **uiMenuNailImagesOrg ( DIALOG *D , ThumbNail **Strs , int width , \
       int height , int font , int color , int FontSize , \
       int justification , int Mag ) \
  {
      void * fid;
      void **imgs;
      char *Str;
      int ln , i , maxchar , temp , item;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length;
      float fac , th , tw , BxSize , width1 , yp , xp;
      kgWC *wc;
      wc = D->wc;
      ln = width;
      if ( Strs == NULL ) return NULL;
      item = 0;
      while ( Strs [ item ] != NULL ) item++;
      if ( item == 0 ) return NULL;
      imgs = ( void ** ) Malloc ( sizeof ( void * ) * ( item+1 ) ) ;
      for ( i = 0; i <= item; i++ ) imgs [ i ] = NULL;
      BxSize = FontSize*1.6;
      if ( BxSize > ( height ) ) {
          BxSize = height;
          FontSize = ( int ) ( ( float ) BxSize/1.6+0.5 ) ;
      }
//TCBTCB
      fid = kgInitImage ( width , height , Mag ) ;
      yp = height*0.5-FontSize*0.5;
      width1 = width-FontSize;
      xp = FontSize*0.5;
      if ( fid != NULL ) {
          kgUserFrame ( fid , 0. , 0. , ( float ) width , ( float ) height ) ;
          kgTextFont ( fid , font ) ;
          for ( i = 0; i < item; i++ ) {
              kgClearView ( fid ) ;
              Str = Strs [ i ]->name;
              th = ( float ) height*.5;
              tw = ( float ) FontSize;
              th = HFAC*tw;
              if ( th > 0.6*height ) th = 0.6*height;
              kgTextSize ( fid , th , tw , GAP*tw ) ;
              length = kgStringLength ( fid , Str ) ;
              if ( length > width1 ) {
                  fac = width1/length;
                  kgTextSize ( fid , th , tw*fac , GAP*tw ) ;
                  length = kgStringLength ( fid , Str ) ;
              }
              kgBoxFill ( fid , 0. , 0. , ( float ) width , ( float ) height , 1 , 1 ) ;
              kgTextColor ( fid , color ) ;
              kgLineColor ( fid , color ) ;
              switch ( justification ) {
                  case -1:
                  kgMove2f ( fid , xp , yp ) ;
                  break;
                  case 0:
                  kgMove2f ( fid , xp+ ( width1-length ) *.5 , yp ) ;
                  break;
                  case 1:
                  kgMove2f ( fid , xp+ ( width1-length-tw*0.5 ) , yp ) ;
                  break;
                  default:
                  kgMove2f ( fid , xp+ ( width1-length ) *.5 , yp ) ;
                  break;
              }
              kgWriteText ( fid , Str ) ;
//TCB
//        imgs[i]=kgGetResizedImage(fid);
              imgs [ i ] = kgGetSharpImage ( fid ) ;
          }
          kgCloseImage ( fid ) ;
      }
      return imgs;
  }
  void **uiMenuNailImages ( DIALOG *D , ThumbNail **Strs , int width , int height , \
       int font , int color , int FontSize , int justification , int Mag ) \
  {
      void * fid;
      void **imgs;
      char *Str;
      int ln , i , maxchar , temp , item;
      void *img = NULL , *imgbk = NULL;
      char *tmpdir , flname [ 200 ] ;
      char Buf [ 1000 ] ;
      float length;
      float fac , th , tw , BxSize , width1 , yp , xp;
      kgWC *wc;
      wc = D->wc;
      int ln1 , x1 , rd , gr , bl;
      int fval , cval;
      int Fz = FontSize;
      float wfac , zfac;
      FONT_STR F;
      IMG_STR *IMG = NULL;
      ln = width;
      if ( Strs == NULL ) return NULL;
      item = 0;
      while ( Strs [ item ] != NULL ) item++;
      if ( item == 0 ) return NULL;
      imgs = ( void ** ) Malloc ( sizeof ( void * ) * ( item+1 ) ) ;
      for ( i = 0; i <= item; i++ ) imgs [ i ] = NULL;
      BxSize = FontSize*2.0;
      if ( BxSize > ( height ) ) {
          BxSize = height;
          FontSize = ( int ) ( ( float ) ( BxSize-2 ) /2.0+0.5 ) ;
      }
      kgGetDefaultRGB ( color , & rd , & gr , & bl ) ;
      F.code = 't';
      F.name = NULL ;
      F.Imgs = Mimgs;
      if ( FontSize <= 0 ) F.Size = ( height-2 ) /2;
      else F.Size = FontSize;
      for ( i = 0; i < item; i++ ) {
          fval = font;
          cval = color;
          F.code = 't';
          Str = Strs [ i ]->name ;
          wfac = 1.0;
          Str = uiCleanOldString ( Str , Buf , & cval , & fval , & wfac , & zfac ) ;
          if ( fval != font ) F.code = 'f';
          if ( F.Size != Fz ) F.code = 'f';
          if ( wfac != 1.0 ) F.code = 'f';
          if ( F.Imgs == NULL ) F.code = 'i';
          if ( F.name != NULL ) free ( F.name ) ;
          F.name = kgGetOthFont ( fval ) ;
#if 0
          if ( F.code == 'f' ) {
              printf ( "Code: %c :%s\n" , F.code , Str ) ;
          }
#endif
          kgGetDefaultRGB ( cval , & rd , & gr , & bl ) ;
          IMG = uiMakeString ( & ( F ) , Buf , ( int ) ( height ) , 0 ) ;
          if ( wfac != 1.0 ) {
              IMG->xln = IMG->xln*wfac;
              img = kgChangeSizeImage ( IMG->img , IMG->xln , height ) ;
              kgFreeImage ( IMG->img ) ;
              IMG->img = img;
          }
          if ( IMG->xln > width-F.Size ) {
              float fac = ( float ) ( width-F.Size ) /IMG->xln;
              img = kgChangeSizeImage ( IMG->img , width-F.Size , height ) ;
              kgFreeImage ( IMG->img ) ;
              IMG->img = img;
              IMG->xln = IMG->xln*fac;
          }
          kgSetImageColor ( IMG->img , rd , gr , bl ) ;
#if 1
          imgbk = kgCreateImage ( width , height ) ;
#else
          fid = kgInitImage ( width , height , 1 ) ;
          kgBoxFill ( fid , 0. , 0. , ( float ) width , ( float ) height , 1 , 1 ) ;
          imgbk = kgGetResizedImage ( fid ) ;
          kgCloseImage ( fid ) ;
#endif
//          kgSetImageColor ( imgbk , 0 , 0 , bl ) ;
          x1 = F.Size/2;
          if ( justification == 1 ) x1 = ( width-IMG->xln-2 ) ;
          else if ( justification == 0 ) x1 = ( width-IMG->xln-2 ) /2;
          ln1 = IMG->xln+1;
          img = IMG->img;
          if ( img != NULL ) {
              kgAddImages ( imgbk , img , x1 , ( int ) ( height/2 - 0.9* FontSize+0.5 ) ) ;
              uiFreeImage ( img ) ;
              free ( IMG ) ;
          }
          imgs [ i ] = imgbk;
      }
      if ( F.name != NULL ) free ( F.name ) ;
      return imgs;
  }
  void *uiMakeXSymbol ( DIX *w , int color , int FontSize , int status ) {
      void * fid;
      int ln , i , maxchar , temp , justification = -1;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length , width , height;
      float fac , th , tw , BxSize , width1 , xp;
      DIALOG *D;
      kgWC *wc;
      D = w->D;
      wc = D->wc;
      width = FontSize*2;
      height = width;
      BxSize = FontSize*1.6;
      tw = ( float ) FontSize;
      th = HFAC*tw;
      fid = kgInitImage ( width , height , RESIZE ) ;
      xp = height*0.5-BxSize*0.5;
      width1 = width*0.5-0.5*BxSize;
      if ( fid != NULL ) {
          kgUserFrame ( fid , 0. , 0. , ( float ) width , ( float ) height ) ;
          kgLineColor ( fid , color ) ;
          if ( status >= 0 ) {
              kgMove2f ( fid , width1 , xp ) ;
              kgDraw2f ( fid , width1 , xp+BxSize ) ;
              kgDraw2f ( fid , width1+BxSize , xp+BxSize ) ;
              kgDraw2f ( fid , width1+BxSize , xp ) ;
              kgDraw2f ( fid , width1 , xp ) ;
#if 0
              kgMove2f ( fid , width1+1 , xp+1 ) ;
              kgDraw2f ( fid , width1+1 , xp+BxSize-1 ) ;
              kgDraw2f ( fid , width1+BxSize-1 , xp+BxSize-1 ) ;
              kgDraw2f ( fid , width1+BxSize-1 , xp+1 ) ;
              kgDraw2f ( fid , width1+1 , xp+1 ) ;
#endif
              if ( status > 0 ) {
                  kgTextColor ( fid , color ) ;
                  kgMove2f ( fid , width1+0.2*FontSize , xp+0.3*BxSize ) ;
                  kgTextSize ( fid , th , tw , GAP*tw ) ;
                  kgWriteText ( fid , "!f354" ) ;
              }
          }
//      img=kgGetResizedImage(fid);
          img = kgGetSharpImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      return img;
  }
  void *uiMakeRSymbol ( DIRA *w , int color , int FontSize , int status ) {
      void * fid;
      int ln , i , maxchar , temp , justification = -1;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length , width , height;
      float fac , th , tw , BxSize , width1 , yp , xp;
      DIALOG *D;
      kgWC *wc;
      D = w->D;
      wc = D->wc;
      width = FontSize*4;
      height = w->width+w->ygap;
//   if(width>height) width=height;
      BxSize = FontSize*2;
      tw = ( float ) FontSize;
      th = 1.25*tw;
      fid = kgInitImage ( width , height , RESIZE ) ;
      xp = 0.0;
      yp = height*0.5-FontSize*0.5;
      width1 = width*0.5-0.5*BxSize;
      if ( fid != NULL ) {
          kgUserFrame ( fid , 0. , 0. , ( float ) width , ( float ) height ) ;
          kgLineColor ( fid , color ) ;
          if ( status >= 0 ) {
              kgLineColor ( fid , color ) ;
              kgChangeColor ( fid , 0 , 0 , 0 , 0 ) ;
              kgChangeColor ( fid , 15 , 255 , 255 , 255 ) ;
              kgRoundedRectangleFill ( fid , xp+ ( float ) BxSize , yp+0.25*BxSize , \
               ( float ) BxSize , ( float ) BxSize , 0 , 15 , 0.5 ) ;
              kgRoundedRectanglePressed ( fid , xp+ ( float ) BxSize , yp+0.25*BxSize , \
               ( float ) BxSize , ( float ) BxSize , color , 0.5 , 4 ) ;
              kgMove2f ( fid , xp+ ( float ) BxSize+0.5*FontSize , yp ) ;
              if ( status > 0 ) {
                  kgArcFill ( fid , xp+BxSize , yp+0.25*BxSize , \
                   ( float ) BxSize*0.2 , 0. , 360. , 0 , 0 ) ;
              }
          }
          img = kgGetResizedImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      return img;
  }
  void *uiMakeCSymbol ( DICH *w , int color , int FontSize , int status ) {
      void * fid;
      int ln , i , maxchar , temp , justification = -1 , r , g , b;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length , width , height , rf , gf , bf;
      float fac , th , tw , BxSize , width1 , yp , xp;
      DIALOG *D;
      kgWC *wc;
      D = w->D;
      wc = D->wc;
      width = FontSize*4;
      height = w->width+w->ygap;
//   if(width>height) width=height;
      BxSize = FontSize*2;
      tw = ( float ) FontSize;
      th = HFAC*tw;
      fid = kgInitImage ( width , height , RESIZE ) ;
      xp = 0.0;
      yp = height*0.5-FontSize*0.5;
      width1 = width*0.5-0.5*BxSize;
      if ( fid != NULL ) {
          kgUserFrame ( fid , 0. , 0. , ( float ) width , ( float ) height ) ;
          kgLineColor ( fid , color ) ;
          kgTextColor ( fid , color ) ;
          kgTextSize ( fid , th , tw , GAP*tw ) ;
          if ( status >= 0 ) {
#if 0
              kgLineColor ( fid , color ) ;
              kgCircle2f ( fid , 1.0*BxSize , yp+0.25*BxSize , ( float ) BxSize*0.35 ) ;
              kgCircle2f ( fid , 1.0*BxSize , yp+0.25*BxSize , \
               ( float ) ( BxSize+2 ) *0.35 ) ;
              kgCircle2f ( fid , 1.0*BxSize , yp+0.25*BxSize , \
               ( float ) ( BxSize+3 ) *0.35 ) ;
#else
              kgGetDefaultRGB ( color , & r , & g , & b ) ;
              rf = r , gf = g , bf = b;
              kgRoundedRectangleRing ( fid , 1.0*BxSize , yp+0.25*BxSize , \
                   ( float ) BxSize*0.7 , \
               ( float ) BxSize*0.7 , rf , gf , bf , 0.5 , 0.7 ) ;
#endif
              kgMove2f ( fid , ( float ) BxSize-0.4*FontSize , yp+0.1*BxSize ) ;
              if ( status > 0 ) kgWriteText ( fid , "!z34!f354" ) ;
          }
          img = kgGetResizedImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      return img;
  }
  void uiCheckString ( DIALOG *D , char *Str , int x , int y , int width , \
       int height , int font , int color , int FontSize , int status ) \
  {
      void * fid;
      int ln , i , maxchar , temp , justification = -1;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length;
      float fac , th , tw , BxSize , width1 , xp;
      kgWC *wc;
      wc = D->wc;
      ln = width;
      if ( Str == NULL ) return;
//   if(Str[0]=='\0') return;
      fid = kgInitImage ( width , height , RESIZE ) ;
      BxSize = FontSize*2;
      xp = height*0.5-FontSize*0.5;
      width1 = width-2*BxSize;
      if ( fid != NULL ) {
          kgUserFrame ( fid , 0. , 0. , ( float ) width , ( float ) height ) ;
          kgTextFont ( fid , font ) ;
          th = ( float ) height*.5;
          tw = ( float ) FontSize;
          th = HFAC*tw;
          if ( th > 0.6*height ) th = 0.6*height;
          kgTextSize ( fid , th , tw , GAP*tw ) ;
          length = kgStringLength ( fid , Str ) ;
          if ( length > width1 ) {
              fac = width1/length;
              kgTextSize ( fid , th , tw*fac , GAP*tw ) ;
              length = kgStringLength ( fid , Str ) ;
          }
          kgTextColor ( fid , color ) ;
          switch ( justification ) {
              case -1:
              kgMove2f ( fid , 2.0*BxSize , xp ) ;
              break;
              case 0:
              kgMove2f ( fid , ( width1-length ) *.5 , xp ) ;
              break;
              case 1:
              kgMove2f ( fid , ( width1-length-tw*0.5 ) , xp ) ;
              break;
              default:
              kgMove2f ( fid , ( width1-length ) *.5 , xp ) ;
              break;
          }
          kgWriteText ( fid , Str ) ;
          if ( status >= 0 ) {
              kgLineColor ( fid , color ) ;
#if 0
#else
              kgCircle2f ( fid , 1.0*BxSize , xp+0.25*BxSize , ( float ) BxSize*0.35 ) ;
              kgCircle2f ( fid , 1.0*BxSize , xp+0.25*BxSize , \
               ( float ) ( BxSize+2 ) *0.35 ) ;
              kgCircle2f ( fid , 1.0*BxSize , xp+0.25*BxSize , \
               ( float ) ( BxSize+3 ) *0.35 ) ;
#endif
              kgMove2f ( fid , ( float ) BxSize-0.4*FontSize , xp+0.1*BxSize ) ;
              if ( status > 0 ) kgWriteText ( fid , "!z34!f354" ) ;
          }
//      img=kgGetResizedImage(fid);
          img = kgGetSharpImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      if ( img != NULL ) {
          kgImage ( D , img , x , y , ln , height , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
      }
      return ;
  }
  void uiRadioString ( DIALOG *D , char *Str , int x , int y , int width , \
       int height , int font , int color , int FontSize , int status ) \
  {
      void * fid;
      int ln , i , maxchar , temp , justification = -1;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length;
      float fac , th , tw , BxSize , width1 , xp;
      kgWC *wc;
      wc = D->wc;
      ln = width;
      if ( Str == NULL ) return;
//   if(Str[0]=='\0') return;
//TCBTCB  
      fid = kgInitImage ( width , height , RESIZE ) ;
      BxSize = FontSize*2;
      xp = height*0.5-FontSize*0.5;
      width1 = width-2*BxSize;
      if ( fid != NULL ) {
          kgUserFrame ( fid , 0. , 0. , ( float ) width , ( float ) height ) ;
          kgTextFont ( fid , font ) ;
          th = ( float ) height*.5;
          tw = ( float ) FontSize;
          th = HFAC*tw;
          if ( th > 0.6*height ) th = 0.6*height;
          kgTextSize ( fid , th , tw , GAP*tw ) ;
//      length =uiStringLength(Str,(int)tw);
          length = kgStringLength ( fid , Str ) ;
          if ( length > width1 ) {
              fac = ( float ) width1/length;
              kgTextSize ( fid , th , tw*fac , GAP*tw ) ;
//        length = uiStringLength(Str,(int)(tw*fac));
              length = kgStringLength ( fid , Str ) ;
          }
          kgTextColor ( fid , color ) ;
          switch ( justification ) {
              case -1:
              kgMove2f ( fid , 2.0*BxSize , xp ) ;
              break;
              case 0:
              kgMove2f ( fid , ( width1-length ) *.5 , xp ) ;
              break;
              case 1:
              kgMove2f ( fid , ( width1-length-tw*0.5 ) , xp ) ;
              break;
              default:
              kgMove2f ( fid , ( width1-length ) *.5 , xp ) ;
              break;
          }
          kgWriteText ( fid , Str ) ;
          if ( status >= 0 ) {
              kgLineColor ( fid , color ) ;
#if 0
#else
              kgChangeColor ( fid , 0 , 0 , 0 , 0 ) ;
              kgChangeColor ( fid , 15 , 255 , 255 , 255 ) ;
              kgRoundedRectangleFill ( fid , ( float ) 1.0*BxSize , xp+0.25*BxSize , \
               ( float ) BxSize , ( float ) BxSize , 0 , 15 , 0.5 ) ;
              kgRoundedRectanglePressed ( fid , ( float ) 1.0*BxSize , xp+0.25*BxSize , \
               ( float ) BxSize , ( float ) BxSize , D->gc.fill_clr , 0.5 , 4 ) ;
#endif
              kgMove2f ( fid , ( float ) BxSize+0.5*FontSize , xp ) ;
              if ( status > 0 ) {
                  kgArcFill ( fid , 1.0*BxSize , xp+0.25*BxSize , \
                   ( float ) BxSize*0.2 , 0. , 360. , 0 , 0 ) ;
              }
          }
//      img=kgGetResizedImage(fid);
          img = kgGetSharpImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      if ( img != NULL ) {
          kgImage ( D , img , x , y , ln , height , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
      }
      return ;
  }
  void uiString ( DIALOG *D , char *str , int x , int y , int width , int height , \
       int font , int color , int FontSize , int justfic , int bkcolor ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    bkcolor : background color ; < 0 background will not be painted
*/
      char Buf [ 1000 ] ;
      char *Str;
      int ln , i , maxchar , temp;
      int x1 , ln1 , old = 0;
      void *img = NULL;
      float length;
      kgWC *wc;
      FONT_STR F;
      IMG_STR *IMG;
      void *imgbk , *fid;
      GMIMG *gimg;
      int w , h;
      int rd , gr , bl;
      int fval , cval;
      float wfac , zfac;
      int Fz = font;
      wc = D->wc;
      ln = width;
      if ( str == NULL ) return;
      if ( str [ 0 ] == '\0' ) return;
      wfac = 1.0;
      cval = color;
      fval = font;
      old = 0;
#if 0
      old = 1;
      uiCleanOldString ( str , Buf , & cval , & fval , & wfac , & zfac ) ;
      kgGetDefaultRGB ( cval , & rd , & gr , & bl ) ;
      F.code = 'f';
      F.name = kgGetOthFont ( fval ) ;
      if ( D->gc.MsgFont == font ) F.Imgs = Mimgs;
      else F.Imgs = Pimgs;
      if ( FontSize <= 0 ) F.Size = ( height-4 ) /2;
      else F.Size = FontSize;
      if ( F.Size > ( ( height-4 ) /2 ) ) F.Size = ( height-4 ) /2 ;
      if ( F.Size != Fz ) F.code = 'f';
      if ( fval != font ) F.code = 'f';
      if ( wfac != 1.0 ) F.code = 'f';
      if ( F.Imgs == NULL ) F.code = 'i';
      IMG = uiMakeString ( & ( F ) , Buf , ( int ) height , 0 ) ;
#if 1
      if ( wfac != 1.0 ) {
          IMG->xln = IMG->xln*wfac;
          img = kgChangeSizeImage ( IMG->img , IMG->xln , height*3/2 ) ;
          kgFreeImage ( IMG->img ) ;
          IMG->img = img;
      }
#endif
      kgSetImageColor ( IMG->img , rd , gr , bl ) ;
#else
      if ( D->gc.MsgFont == font ) F.Imgs = Mimgs;
      else F.Imgs = Pimgs;
      if ( FontSize <= 0 ) F.Size = ( height-4 ) /2;
      else F.Size = FontSize;
      IMG = ( IMG_STR * ) uiComplexString ( str , F.Imgs , \
           font , cval , F.Size , height ) ;
//     if(F.Imgs == Mimgs ) IMG->img  = NULL;
#endif
      if ( bkcolor >= 0 ) {
          fid = kgInitImage ( ln , height , 1 ) ;
          kgBoxFill ( fid , 0. , 0. , ( float ) ln , ( float ) height , bkcolor , 0 ) ;
          imgbk = kgGetResizedImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      else imgbk = NULL;
      gimg = ( GMIMG * ) ( IMG->img ) ;
      w = gimg->image_width;
      h = gimg->image_height;
      if ( ( w > ln-FontSize ) || ( h > height ) ) {
          float fac;
          if ( h > height ) h = height;
          if ( w > ( ln-FontSize ) ) w = ln-FontSize;
          fac = ( float ) ( ln-FontSize ) /w ;
          img = kgChangeSizeImage ( IMG->img , w , h ) ;
//            img = kgResizeImage(IMG->img,fac);
          kgFreeImage ( IMG->img ) ;
          IMG->xln = w;
          IMG->img = img;
      }
      x1 = 0;
      if ( justfic == 1 ) x1 = ( ln-IMG->xln-2 ) ;
      else if ( justfic == 0 ) x1 = ( ln-IMG->xln ) /2;
      ln1 = IMG->xln+1;
      img = IMG->img;
      if ( F.name != NULL ) free ( F.name ) ;
      if ( img != NULL ) {
          if ( imgbk != NULL ) {
              if ( old ) kgAddImages ( imgbk , img , x1 , height/2-FontSize*1.5 ) ;
              else kgAddImages ( imgbk , img , x1 , 0 ) ;
              kgImage ( D , imgbk , x , y , ln , height , 0.0 , 1.0 ) ;
              uiFreeImage ( imgbk ) ;
          }
          else {
              kgImage ( D , img , x+x1 , y , ln1 , ( height ) , 0.0 , 1.0 ) ;
          }
          uiFreeImage ( img ) ;
          free ( IMG ) ;
      }
      else printf ( "img == NULL\n" ) ;
  }
  void uiMessageString ( DIALOG *D , char *str , int x , int y , int width , \
       int height , int font , int color , int FontSize , \
       int justfic , int bkcolor ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    bkcolor : background color ; < 0 background will not be painted
*/
      char Buf [ 1000 ] ;
      char *Str;
      int ln , i , maxchar , temp;
      int x1 , ln1 , old = 0;
      void *img = NULL;
      float length;
      kgWC *wc;
      FONT_STR F;
      IMG_STR *IMG;
      void *imgbk , *fid;
      GMIMG *gimg;
      int w , h;
      int rd , gr , bl;
      int fval , cval;
      float wfac , zfac;
      int Fz = font;
      wc = D->wc;
      ln = width;
      if ( str == NULL ) return;
      if ( str [ 0 ] == '\0' ) return;
      wfac = 1.0;
      cval = color;
      fval = font;
      old = 0;
#if 0
      old = 1;
      uiCleanOldString ( str , Buf , & cval , & fval , & wfac , & zfac ) ;
      kgGetDefaultRGB ( cval , & rd , & gr , & bl ) ;
      F.code = 'f';
      F.name = kgGetOthFont ( fval ) ;
      if ( D->gc.MsgFont == font ) F.Imgs = Mimgs;
      else F.Imgs = Pimgs;
      if ( FontSize <= 0 ) F.Size = ( height-4 ) /2;
      else F.Size = FontSize;
      if ( F.Size > ( ( height-4 ) /2 ) ) F.Size = ( height-4 ) /2 ;
      if ( F.Size != Fz ) F.code = 'f';
      if ( fval != font ) F.code = 'f';
      if ( wfac != 1.0 ) F.code = 'f';
      if ( F.Imgs == NULL ) F.code = 'i';
      IMG = uiMakeString ( & ( F ) , Buf , ( int ) height , 0 ) ;
#if 1
      if ( wfac != 1.0 ) {
          IMG->xln = IMG->xln*wfac;
          img = kgChangeSizeImage ( IMG->img , IMG->xln , height*3/2 ) ;
          kgFreeImage ( IMG->img ) ;
          IMG->img = img;
      }
#endif
      kgSetImageColor ( IMG->img , rd , gr , bl ) ;
#else
      if ( D->gc.MsgFont == font ) F.Imgs = Mimgs;
      else F.Imgs = Pimgs;
      if ( FontSize <= 0 ) F.Size = ( height-4 ) /2;
      else F.Size = FontSize;
      IMG = ( IMG_STR * ) uiComplexString ( str , F.Imgs , \
           font , cval , F.Size , height ) ;
//     if(F.Imgs == Mimgs ) IMG->img  = NULL;
#endif
      if ( bkcolor >= 0 ) {
          fid = kgInitImage ( ln , height , 1 ) ;
          kgBoxFill ( fid , 0. , 0. , ( float ) ln , ( float ) height , bkcolor , 0 ) ;
          imgbk = kgGetResizedImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      else imgbk = NULL;
      gimg = ( GMIMG * ) ( IMG->img ) ;
      w = gimg->image_width;
      h = gimg->image_height;
      if ( ( w > ln-FontSize ) || ( h > height ) ) {
          float fac;
          if ( h > height ) h = height;
          if ( w > ( ln-FontSize ) ) w = ln-FontSize;
          fac = ( float ) ( ln-FontSize ) /w ;
          img = kgChangeSizeImage ( IMG->img , w , h ) ;
//            img = kgResizeImage(IMG->img,fac);
          kgFreeImage ( IMG->img ) ;
          IMG->xln = w;
          IMG->img = img;
      }
      x1 = 0;
      if ( justfic == 1 ) x1 = ( ln-IMG->xln-2 ) ;
      else if ( justfic == 0 ) x1 = ( ln-IMG->xln ) /2;
      ln1 = IMG->xln+1;
      img = IMG->img;
      if ( F.name != NULL ) free ( F.name ) ;
      if ( img != NULL ) {
          if ( imgbk != NULL ) {
              if ( old ) kgAddImages ( imgbk , img , x1 , height/2-FontSize*1.2 ) ;
              else kgAddImages ( imgbk , img , x1 , 0 ) ;
              kgImage ( D , imgbk , x , y , ln , height , 0.0 , 1.0 ) ;
              uiFreeImage ( imgbk ) ;
          }
          else {
              kgImage ( D , img , x+x1 , y , ln1 , ( height ) , 0.0 , 1.0 ) ;
          }
          uiFreeImage ( img ) ;
          free ( IMG ) ;
      }
      else printf ( "img == NULL\n" ) ;
  }
  void uiString_bkup ( DIALOG *D , char *str , int x , int y , int width , \
       int height , int font , int color , int FontSize , \
       int justfic , int bkcolor ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    bkcolor : background color ; < 0 background will not be painted
*/
      char Buf [ 1000 ] ;
      char *Str;
      int ln , i , maxchar , temp;
      int x1 , ln1 , old = 0;
      void *img = NULL;
      float length;
      kgWC *wc;
      FONT_STR F;
      IMG_STR *IMG;
      void *imgbk , *fid;
      int rd , gr , bl;
      int fval , cval;
      float wfac , zfac;
      int Fz = font;
      wc = D->wc;
      ln = width;
      if ( str == NULL ) return;
      if ( str [ 0 ] == '\0' ) return;
      wfac = 1.0;
      cval = color;
      fval = font;
      old = 0;
#if 0
      old = 1;
      uiCleanOldString ( str , Buf , & cval , & fval , & wfac , & zfac ) ;
      kgGetDefaultRGB ( cval , & rd , & gr , & bl ) ;
      F.code = 'f';
      F.name = kgGetOthFont ( fval ) ;
      if ( D->gc.MsgFont == font ) F.Imgs = Mimgs;
      else F.Imgs = Pimgs;
      if ( FontSize <= 0 ) F.Size = ( height-4 ) /2;
      else F.Size = FontSize;
      if ( F.Size > ( ( height-4 ) /2 ) ) F.Size = ( height-4 ) /2 ;
      if ( F.Size != Fz ) F.code = 'f';
      if ( fval != font ) F.code = 'f';
      if ( wfac != 1.0 ) F.code = 'f';
      if ( F.Imgs == NULL ) F.code = 'i';
      IMG = uiMakeString ( & ( F ) , Buf , ( int ) height , 0 ) ;
#if 1
      if ( wfac != 1.0 ) {
          IMG->xln = IMG->xln*wfac;
          img = kgChangeSizeImage ( IMG->img , IMG->xln , height*3/2 ) ;
          kgFreeImage ( IMG->img ) ;
          IMG->img = img;
      }
#endif
      kgSetImageColor ( IMG->img , rd , gr , bl ) ;
#else
      if ( D->gc.MsgFont == font ) F.Imgs = Mimgs;
      else F.Imgs = Pimgs;
      if ( FontSize <= 0 ) F.Size = ( height-4 ) /2;
      else F.Size = FontSize;
      IMG = ( IMG_STR * ) uiComplexString ( str , F.Imgs , \
           font , cval , F.Size , height ) ;
#endif
      if ( bkcolor >= 0 ) {
          fid = kgInitImage ( ln , height , 1 ) ;
          kgBoxFill ( fid , 0. , 0. , ( float ) ln , ( float ) height , bkcolor , 0 ) ;
          imgbk = kgGetResizedImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      else imgbk = NULL;
      if ( IMG->xln > ln-FontSize ) {
          float fac;
          fac = ( float ) ( ln-FontSize ) /IMG->xln ;
          img = kgChangeSizeImage ( IMG->img , ln-FontSize , ( height ) ) ;
          kgFreeImage ( IMG->img ) ;
          IMG->xln = IMG->xln *fac;
          IMG->img = img;
      }
      x1 = 0;
      if ( justfic == 1 ) x1 = ( ln-IMG->xln-2 ) ;
      else if ( justfic == 0 ) x1 = ( ln-IMG->xln ) /2;
      ln1 = IMG->xln+1;
      img = IMG->img;
      if ( F.name != NULL ) free ( F.name ) ;
      if ( img != NULL ) {
          if ( imgbk != NULL ) {
              if ( old ) kgAddImages ( imgbk , img , x1 , height/2-FontSize*1.2 ) ;
              else kgAddImages ( imgbk , img , x1 , 0 ) ;
              kgImage ( D , imgbk , x , y , ln , height , 0.0 , 1.0 ) ;
              uiFreeImage ( imgbk ) ;
          }
          else {
              kgImage ( D , img , x+x1 , y , ln1 , ( height ) , 0.0 , 1.0 ) ;
          }
          uiFreeImage ( img ) ;
          free ( IMG ) ;
      }
      else printf ( "img == NULL\n" ) ;
  }
  void *uiStringToImage ( DIALOG *D , char *str , int x , int y , int width , \
       int height , int font , int color , int FontSize , \
       int justfic , int bkcolor ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    bkcolor : background color ; < 0 background will not be painted
*/
      char Buf [ 1000 ] ;
      char *Str;
      int ln , i , maxchar , temp;
      int x1 , ln1;
      void *img = NULL;
      float length;
      kgWC *wc;
      FONT_STR F;
      IMG_STR *IMG;
      void *imgbk , *fid;
      int rd , gr , bl;
      int fval , cval;
      float wfac , zfac;
      int Ht = height;
      int Fz = FontSize;
      wc = D->wc;
      ln = width;
      if ( str == NULL ) return NULL;
      if ( str [ 0 ] == '\0' ) return NULL;
      wfac = 1.0;
      cval = color;
      fval = font;
//      if(Ht> 2*FontSize+6) Ht =2*FontSize+6 ;
      if ( bkcolor < 0 ) height = Ht;
//      printf("uiStringToImage: %s\n",str);
#if 0
      Str = uiCleanOldString ( str , Buf , & cval , & fval , & wfac , & zfac ) ;
      kgGetDefaultRGB ( cval , & rd , & gr , & bl ) ;
      F.code = 't';
      F.name = kgGetOthFont ( fval ) ;
      F.Imgs = Bimgs;
      if ( FontSize <= 0 ) F.Size = ( height-6 ) /2;
      else F.Size = FontSize;
      if ( F.Size > ( ( height-6 ) /2 ) ) F.Size = ( height-6 ) /2 ;
//      F.Size = ( height-6 ) /2 ;
      if ( F.Size != Fz ) F.code = 'f';
      if ( fval != font ) F.code = 'f';
      if ( wfac != 1.0 ) F.code = 'f';
      if ( F.Imgs == NULL ) F.code = 'i';
      IMG = uiMakeString ( & ( F ) , Buf , ( int ) height , 0 ) ;
#if 1
      if ( wfac != 1.0 ) {
          IMG->xln = IMG->xln*wfac;
          img = kgChangeSizeImage ( IMG->img , IMG->xln , height ) ;
          kgFreeImage ( IMG->img ) ;
          IMG->img = img;
      }
#endif
      kgSetImageColor ( IMG->img , rd , gr , bl ) ;
#else
      F.Imgs = Bimgs;
      if ( FontSize <= 0 ) F.Size = ( height-6 ) /2;
      else F.Size = FontSize;
      if ( F.Size > ( ( height-6 ) /2 ) ) F.Size = ( height-6 ) /2 ;
      IMG = ( IMG_STR * ) uiComplexString ( str , F.Imgs , \
           font , cval , F.Size , height ) ;
#endif
      if ( bkcolor >= 0 ) {
          fid = kgInitImage ( ln , height , 1 ) ;
          kgBoxFill ( fid , 0. , 0. , ( float ) ln , ( float ) height , bkcolor , 0 ) ;
          imgbk = kgGetResizedImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      else imgbk = NULL;
      if ( IMG->xln > ln-FontSize ) {
          float fac;
          fac = ( float ) ( ln-FontSize ) /IMG->xln ;
          img = kgChangeSizeImage ( IMG->img , ln-FontSize , ( height ) ) ;
          kgFreeImage ( IMG->img ) ;
          IMG->xln = IMG->xln *fac;
          IMG->img = img;
      }
      x1 = 0;
      if ( justfic == 1 ) x1 = ( ln-IMG->xln-2 ) ;
      else if ( justfic == 0 ) x1 = ( ln-IMG->xln ) /2;
      ln1 = IMG->xln+1;
      img = IMG->img;
      if ( F.name != NULL ) free ( F.name ) ;
      if ( img != NULL ) {
          if ( imgbk != NULL ) {
              kgAddImages ( imgbk , img , x1 , height/2-FontSize ) ;
              uiFreeImage ( img ) ;
              img = imgbk ;
          }
          free ( IMG ) ;
      }
      else printf ( "img == NULL\n" ) ;
      return img;
  }
  void uiMsgString ( DIALOG *D , char *str , int x1 , int y1 , \
       int char_clr , int Font , int FontSize ) \
  {
 /* writes a string in fixed font mod */
      void *fid , *img;
      int jj , k , i , iyi , ln;
      unsigned int temp;
      char stmp [ 2 ] ;
      int xsize , ysize;
      float th , tw , tg , xx , yy; ;
      ln = strlen ( str ) ;
      if ( ln > 0 ) {
          xsize = ln*FontSize;
          ysize = 1.80*FontSize;
          stmp [ 1 ] = '\0';
//   fid = kgInitImage((int)(xsize),ysize,2);
          fid = kgInitImage ( ( int ) ( xsize ) , ysize , RESIZE ) ;
          kgUserFrame ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize ) ;
          th = FontSize*1.1;
          tw = FontSize;
          kgTextFont ( fid , Font ) ;
          kgTextSize ( fid , th , tw , GAP*tw ) ;
          kgTextColor ( fid , char_clr ) ;
          xx = 0.0;
          yy = 0.5*FontSize;
          i = 0;
          kgMove2f ( fid , xx , yy ) ;
          kgWriteText ( fid , str ) ;
//   img = kgGetResizedImage(fid);
          img = kgGetSharpImage ( fid ) ;
          kgCloseImage ( fid ) ;
          kgImage ( D , img , x1 , y1 , xsize , ysize , 0.0 , 1.0 ) ;
          kgFreeImage ( img ) ;
      }
      return;
  }
  void uiBoxedString ( DIALOG *D , char *str , int x , int y , int width , \
       int height , int font , int border , int highli , int charclr , \
       int FontSize , int justfic , float rfac , int state ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    Meaning of State
    -1 pressed
     0 normal
     1 Highlight
*/
      int ln , i , maxchar , temp;
      void *img = NULL;
      float length;
      kgWC *wc;
      wc = D->wc;
      ln = width;
//   if(str==NULL) return;
//   if(str[0]=='\0') return;
      img = kgBoxedStringToImage ( str , NULL , ln , height , font , border , \
           highli , charclr , justfic , FontSize , rfac , state ) ;
      if ( img != NULL ) {
          kgImage ( D , img , x , y , ln , height , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
      }
  }
  void uiFilledString ( DIALOG *D , char *str , int x , int y , int width , \
       int height , int font , int fillcolor , int highli , int color , int FontSize , \
       int justfic , float rfac , int state , float depthfac ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    Meaning of State
    -1 pressed
     0 normal
     1 Highlight
*/
      int ln , i , maxchar , temp;
      void *img = NULL;
      float length;
      kgWC *wc;
      wc = D->wc;
      ln = width;
//   if(str==NULL) return;
//   if(str[0]=='\0') return;
      img = kgFilledStringToImage3 ( str , NULL , ln , height , font , fillcolor , \
           highli , color , D->gc.fill_clr , justfic , \
           FontSize , rfac , state , depthfac ) ;
      if ( img != NULL ) {
          kgImage ( D , img , x , y , ln , height , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
      }
  }
  void uiFilledString1 ( DIALOG *D , char *str , int x , int y , int width , \
       int height , int font , int fillcolor , int highli , int color , int FontSize , \
       int justfic , float rfac , int state , float depthfac ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    Meaning of State
    -1 pressed
     0 normal
     1 Highlight
*/
      int ln , i , maxchar , temp;
      void *img = NULL;
      float length;
      kgWC *wc;
      wc = D->wc;
      ln = width;
//   if(str==NULL) return;
//   if(str[0]=='\0') return;
      img = kgFilledStringToImage1 ( str , NULL , ln , height , font , fillcolor , \
           highli , color , D->gc.fill_clr , justfic , \
           FontSize , rfac , state , depthfac ) ;
      if ( img != NULL ) {
          kgImage ( D , img , x , y , ln , height , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
      }
  }
  void uiSplashString ( DIALOG *D , char *str , int x , int y , int width , \
       int height , int font , int fillcolor , int color , \
       int FontSize , int justfic , float rfac ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    Meaning of State
    -1 pressed
     0 normal
     1 Highlight
*/
      int ln , i , maxchar , temp;
      void *img = NULL;
      float length;
      kgWC *wc;
      wc = D->wc;
      ln = width;
//   if(str==NULL) return;
//   if(str[0]=='\0') return;
      img = kgSplashStringToImage ( str , ln , height , font , fillcolor , \
           color , D->gc.fill_clr , justfic , FontSize , rfac ) ;
      if ( img != NULL ) {
          kgImage ( D , img , x , y , ln , height , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
      }
  }
  void uiShadedString ( DIALOG *D , char *str , int x , int y , int width , \
       int height , int font , int fillcolor , int highli , int color , int FontSize , \
       int justfic , float rfac , int state , int type ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    Meaning of State
    -1 pressed
     0 normal
     1 Highlight
*/
      int ln , i , maxchar , temp;
      void *img = NULL;
      float length;
      kgWC *wc;
      wc = D->wc;
      ln = width;
      switch ( type ) {
          case 0:
          uiString ( D , str , x , y+height+5 , ln , 20 , font , \
               color , FontSize , justfic , fillcolor ) ;
          return;
          case 1:
          uiBoxedString ( D , str , x , y , width , height , font , fillcolor , \
               highli , color , FontSize , justfic , rfac , state ) ;
          return;
          case 2:
          uiFilledString ( D , str , x , y , width , height , font , fillcolor , \
               highli , color , FontSize , justfic , rfac , state , 3.0 ) ;
          return;
          case 3:
          uiFilledString1 ( D , str , x , y , width , height , font , fillcolor , \
               highli , color , FontSize , justfic , rfac , state , 5.0 ) ;
          return;
          case 4:
          case 5:
          case 6:
          img = kgShadedStringToImage ( str , NULL , ln , height , font , fillcolor , \
               highli , color , justfic , FontSize , rfac , state , type-4 ) ;
          if ( img != NULL ) {
              kgImage ( D , img , x , y , ln , height , 0.0 , 1.0 ) ;
              uiFreeImage ( img ) ;
          }
          return;
          default:
          uiFilledString ( D , str , x , y , width , height , font , fillcolor , \
               highli , color , FontSize , justfic , rfac , state , \
           ( float ) ( type-6 ) ) ;
          return;
      }
  }
  void uiShadedStringImage ( DIALOG *D , char *str , void *image , int x , \
       int y , int width , int height , int font , int fillcolor , int highli , \
       int color , int FontSize , int justfic , \
       float rfac , int state , int type ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    Meaning of State
    -1 pressed
     0 normal
     1 Highlight
*/
      int ln , i , maxchar , temp , off;
      void *img = NULL;
      float length;
      kgWC *wc;
#if 0
      if ( ( image == NULL ) && ( type != 0 ) ) {
          uiShadedString ( D , str , x , y , width , height , font , fillcolor , \
               highli , color , FontSize , justfic , rfac , state , type ) ;
          return;
      }
#endif
      wc = D->wc;
      ln = width*0.85+0.5;
      off = ( width-ln ) *0.5;
      switch ( type ) {
          case 0:
          uiString ( D , str , x , y+height+5 , width , 20 , font , \
               color , FontSize , justfic , fillcolor ) ;
          if ( image != NULL ) { kgImage ( D , image , x+1 , \
               y+1 , width-2 , height-2 , 0.0 , 1.0 ) ;
          }
          return;
          case 1:
          uiBoxedString ( D , NULL , x , y , width , height , font , fillcolor , \
               highli , color , FontSize , justfic , rfac , state ) ;
          if ( image != NULL ) { kgImage ( D , image , x+1 , \
               y+1 , width-2 , height-2 , 0.0 , 1.0 ) ;
          }
          if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) uiString ( D , str , x+off , \
               y , ln , height , font , color , FontSize , justfic , -1 ) ;
          return;
          case 2:
          uiFilledString ( D , NULL , x , y , width , height , font , fillcolor , \
               highli , color , FontSize , justfic , rfac , state , 3.0 ) ;
          if ( image != NULL ) { kgImage ( D , image , x+1 , \
               y+1 , width-2 , height-2 , 0.0 , 1.0 ) ;
          }
          if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) uiString ( D , str , x+off , \
               y , ln , height , font , color , FontSize , justfic , -1 ) ;
          return;
          case 3:
          uiFilledString1 ( D , NULL , x , y , width , height , font , fillcolor , \
               highli , color , FontSize , justfic , rfac , state , 3.0 ) ;
          if ( image != NULL ) { kgImage ( D , image , x+1 , \
               y+1 , width-2 , height-2 , 0.0 , 1.0 ) ;
          }
          if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) uiString ( D , str , x+off , \
               y , ln , height , font , color , FontSize , justfic , -1 ) ;
          return;
          case 4:
          case 5:
          case 6:
          img = kgShadedStringToImage ( NULL , NULL , width , height , font , fillcolor , \
               highli , color , justfic , FontSize , rfac , state , type-4 ) ;
          if ( img != NULL ) {
              kgImage ( D , img , x , y , width , height , 0.0 , 1.0 ) ;
              uiFreeImage ( img ) ;
          }
          if ( image != NULL ) { kgImage ( D , image , x+1 , \
               y+1 , width-2 , height-2 , 0.0 , 1.0 ) ;
          }
          if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) uiString ( D , str , x+off , \
               y , ln , height , font , color , FontSize , justfic , -1 ) ;
          return;
          default:
          uiFilledString1 ( D , NULL , x , y , width , height , font , fillcolor , \
               highli , color , FontSize , justfic , rfac , state , \
           ( float ) ( type-3. ) ) ;
          if ( image != NULL ) { kgImage ( D , image , x+1 , \
               y+1 , width-2 , height-2 , 0.0 , 1.0 ) ;
          }
          if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) uiString ( D , str , x+off , \
               y , ln , height , font , color , FontSize , justfic , -1 ) ;
          return;
      }
  }
  void *uiMakeButtonImage ( DIALOG *D , int width , int height , int fillcolor , \
       int state , int highli , float rfac , int type ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    Meaning of State
    -1 pressed
     0 normal
     1 Highlight
*/
      int ln , i , maxchar , temp , off , poff = 0;
      void *img = NULL , *cimg , *img1 = NULL;
      int font = 1 , FontSize = 10 , color = 1 , justfic = 0;
      float length;
      GMIMG *gimg;
      kgWC *wc;
      wc = D->wc;
      ln = width*0.85+0.5;
      off = ( width-ln ) *0.5;
      if ( state == -1 ) poff = -1;
//      printf("TCB: type : %d\n",type);
      switch ( type ) {
          case 0:
          return NULL;
          case 9:
          return NULL;
          case 1:
          img = kgBoxedStringToImage ( NULL , NULL , width , height , font , fillcolor , \
               highli , color , justfic , FontSize , rfac , state ) ;
          return img;
          case 2:
          switch ( state ) {
              case -1:
              img = kgBorderedRectangle ( width , height , D->gc.dim , rfac ) ;
              break;
              case 1:
              img = kgBorderedRectangle ( width , height , fillcolor , rfac ) ;
              break;
              default:
              img = kgBorderedRectangle ( width , height , fillcolor , rfac ) ;
              break;
          }
          return img;
          case 3:
          img = kgFilledStringToImage3 ( NULL , NULL , width , height , font , fillcolor , \
               highli , color , D->gc.fill_clr , justfic , FontSize , rfac , state , 2.0 ) ;
          img1 = kgFilledStringToImage3 ( NULL , NULL , width-2 , height-2 , font , \
               fillcolor , highli , color , D->gc.fill_clr , \
               justfic , FontSize , rfac , state , 3.0 ) ;
          img = kgMergeImages ( img , img1 , 0 , 0 ) ;
          kgFreeImage ( img1 ) ;
          return img;
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
//              printf("TCB:rfac = %f\n",rfac);
          img = kgShadedStringToImage ( NULL , NULL , width , height , font , fillcolor , \
               highli , color , justfic , FontSize , rfac , state , type-4 ) ;
          return img;
          default:
          img = kgFilledStringToImage1 ( NULL , NULL , width , height , font , fillcolor , \
               highli , color , D->gc.fill_clr , justfic , FontSize , rfac , state , 3.0 ) ;
          return img;
      }
  }
  void *uiAddButtonString ( DIALOG *D , char *str , void *image , void *xpm , \
       int width , int height , int font , int color , int FontSize , int type ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    Meaning of State
    -1 pressed
     0 normal
     1 Highlight
*/
      int ln , i , maxchar , temp , off , poff = 0;
      int justfic = 0;
      void *img = NULL , *cimg , *img1 = NULL , *simg = NULL;
      float length;
      int resize = 0;
      GMIMG *gimg;
      kgWC *wc;
      wc = D->wc;
      ln = width*0.85+0.5;
      off = ( width-ln ) *0.5;
      if ( type > 9 ) return NULL;
      if ( xpm != NULL ) {
          gimg = ( GMIMG * ) xpm;
          if ( ( gimg->image_width > width ) || ( gimg->image_height > height ) ) {
              xpm = kgChangeSizeImage ( xpm , width , height ) ;
              resize = 1;
          }
      }
      cimg = kgGetImageCopy ( D , image ) ;
      if ( cimg == NULL ) cimg = xpm;
      else {
          if ( xpm != NULL ) {
              cimg = kgMergeImages ( cimg , xpm , off , 0 ) ;
          }
      }
      switch ( type ) {
          case 0:
          return cimg;
          case 9:
          return cimg;
          default:
          if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) {
              simg = uiStringToImage ( D , str , 0 , 0 , ln , height , \
                   font , color , FontSize , justfic , -1 ) ;
              cimg = kgMergeImages ( cimg , simg , 0 , 0 ) ;
              kgFreeImage ( simg ) ;
          }
          return cimg;
      }
  }
  void *uiMakeStringImage ( DIALOG *D , char *str , void *image , int x , \
       int y , int width , int height , int font , int fillcolor , int highli , \
       int color , int FontSize , int justfic , \
       float rfac , int state , int type ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    Meaning of State
    -1 pressed
     0 normal
     1 Highlight
*/
      int ln , i , maxchar , temp , off , poff = 0;
      void *img = NULL , *cimg , *img1 = NULL;
      float length;
      GMIMG *gimg;
      kgWC *wc;
      wc = D->wc;
      ln = width*0.85+0.5;
      off = ( width-ln ) *0.5;
      if ( state == -1 ) poff = -1;
      cimg = kgGetImageCopy ( D , image ) ;
      switch ( type ) {
          case 0:
//       uiString(D,str,x,y+height+5,width,20,font,color,FontSize,justfic,fillcolor);
          if ( cimg == NULL ) return NULL;
          gimg = ( GMIMG * ) cimg;
          if ( ( gimg->image_width > width ) || ( gimg->image_height > height ) ) {
              img = kgChangeSizeImage ( cimg , width , height ) ;
              kgFreeImage ( cimg ) ;
              return img;
          }
          else return cimg;
          case 9:
//       uiString(D,str,x,y+height+5,width,20,font,color,FontSize,justfic,fillcolor);
          if ( cimg == NULL ) {
              if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) {
 //                 cimg = kgStringToImage ( str , NULL , ln , height , font , color , justfic , FontSize , -1 ) ;
                  cimg = uiStringToImage ( D , str , 0 , 0 , ln , height , \
                       font , color , FontSize , justfic , -1 ) ;
              }
              return cimg;
          }
          gimg = ( GMIMG * ) cimg;
          if ( ( gimg->image_width > width ) || ( gimg->image_height > height ) ) {
              img = kgChangeSizeImage ( cimg , width , height ) ;
              kgFreeImage ( cimg ) ;
              if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) {
                  cimg = kgStringToImage ( str , NULL , ln , height , \
                       font , color , justfic , FontSize , -1 ) ;
                  if ( cimg != NULL ) {
                      img = kgMergeImages ( img , cimg , off , 0 ) ;
                      kgFreeImage ( cimg ) ;
                  }
              }
              return img;
          }
          else {
              if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) {
                  img = kgStringToImage ( str , NULL , ln , height , \
                       font , color , justfic , FontSize , -1 ) ;
                  if ( img != NULL ) {
                      cimg = kgMergeImages ( cimg , img , off , 0 ) ;
                      kgFreeImage ( img ) ;
                  }
              }
              return cimg;
          }
          break;
          case 1:
          if ( cimg != NULL ) {
              img = kgBoxedStringToImage ( NULL , NULL , width , height , font , fillcolor , \
                   highli , color , justfic , FontSize , rfac , state ) ;
              img = kgMergeImages ( img , cimg , 0 , 0 ) ;
              kgFreeImage ( cimg ) ;
              if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) {
 //                 cimg = kgStringToImage ( str , NULL , ln , height , font , color , justfic , FontSize , -1 ) ;
                  cimg = uiStringToImage ( D , str , 0 , 0 , ln , height , \
                       font , color , FontSize , justfic , -1 ) ;
                  if ( cimg != NULL ) {
                      img = kgMergeImages ( img , cimg , off , 0 ) ;
                      kgFreeImage ( cimg ) ;
                  }
              }
          }
          else {
//             img = kgBoxedStringToImage ( str , NULL , width , height , font , fillcolor , highli , color , justfic , FontSize , rfac , state ) ;
              img = kgBoxedStringToImage ( NULL , NULL , width , height , font , fillcolor , \
                   highli , color , justfic , FontSize , rfac , state ) ;
              cimg = uiStringToImage ( D , str , 0 , 0 , width , height , \
                   font , color , FontSize , justfic , -1 ) ;
              img = kgMergeImages ( img , cimg , 0 , 0 ) ;
              kgFreeImage ( cimg ) ;
          }
          return img;
          case 2:
          switch ( state ) {
              case -1:
              img = kgBorderedRectangle ( width , height , D->gc.dim , rfac ) ;
              break;
              case 1:
              img = kgBorderedRectangle ( width , height , fillcolor , rfac ) ;
              break;
              default:
              img = kgBorderedRectangle ( width , height , fillcolor , rfac ) ;
              break;
          }
          if ( cimg != NULL ) {
              img = kgMergeImages ( img , cimg , poff , poff ) ;
              kgFreeImage ( cimg ) ;
          }
          if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) {
//              cimg = kgStringToImage ( str , NULL , ln , height , font , color , justfic , FontSize , -1 ) ;
              cimg = uiStringToImage ( D , str , 0 , 0 , width , height , \
                   font , color , FontSize , justfic , -1 ) ;
              if ( cimg != NULL ) {
//              img = kgMergeImages(img,cimg,off,0);
                  img = kgMergeImages ( img , cimg , 0 , 0 ) ;
                  kgFreeImage ( cimg ) ;
              }
          }
          return img;
          case 3:
          if ( cimg != NULL ) {
              img = kgFilledStringToImage3 ( NULL , NULL , width , height , font , fillcolor , \
                   highli , color , D->gc.fill_clr , justfic , FontSize , rfac , state , 2.0 ) ;
              img1 = kgFilledStringToImage3 ( NULL , NULL , width-2 , height-2 , font , \
                   fillcolor , highli , color , D->gc.fill_clr , \
                   justfic , FontSize , rfac , state , 3.0 ) ;
              img = kgMergeImages ( img , img1 , 0 , 0 ) ;
              img = kgMergeImages ( img , cimg , poff , poff ) ;
              kgFreeImage ( cimg ) ;
              if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) {
//                  cimg = kgStringToImage ( str , NULL , ln , height , font , color , justfic , FontSize , -1 ) ;
                  cimg = uiStringToImage ( D , str , 0 , 0 , ln , height , \
                       font , color , FontSize , justfic , -1 ) ;
                  if ( cimg != NULL ) {
                      img = kgMergeImages ( img , cimg , off , 0 ) ;
                      kgFreeImage ( cimg ) ;
                  }
              }
          }
          else {
              img = kgFilledStringToImage3 ( NULL , NULL , width , height , font , fillcolor , \
                   highli , color , D->gc.fill_clr , justfic , FontSize , rfac , state , 2.0 ) ;
              img1 = kgFilledStringToImage3 ( str , NULL , width-2 , height-2 , font , \
                   fillcolor , highli , color , D->gc.fill_clr , \
                   justfic , FontSize , rfac , state , 3.0 ) ;
              img = kgMergeImages ( img , img1 , 0 , 0 ) ;
          }
          return img;
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
          if ( cimg != NULL ) {
              img = kgShadedStringToImage ( NULL , NULL , width , height , font , fillcolor , \
                   highli , color , justfic , FontSize , rfac , state , type-4 ) ;
              img = kgMergeImages ( img , cimg , 0 , 0 ) ;
              kgFreeImage ( cimg ) ;
              if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) {
//                  cimg = kgStringToImage ( str , NULL , ln , height , font , color , justfic , FontSize , -1 ) ;
                  cimg = uiStringToImage ( D , str , 0 , 0 , ln , height , \
                       font , color , FontSize , justfic , -1 ) ;
                  if ( cimg != NULL ) {
//                      img = kgMergeImages ( img , cimg , off , 0 ) ;
                      img = kgMergeImages ( img , cimg , 0 , 0 ) ;
                      kgFreeImage ( cimg ) ;
                  }
              }
          }
          else {
//              img = kgShadedStringToImage ( str , NULL , width , height , font , fillcolor , highli , color , justfic , FontSize , rfac , state , type-4 ) ;
              img = kgShadedStringToImage ( NULL , NULL , width , height , font , fillcolor , \
                   highli , color , justfic , FontSize , rfac , state , type-4 ) ;
              cimg = uiStringToImage ( D , str , 0 , 0 , width , height , \
                   font , color , FontSize , justfic , -1 ) ;
              img = kgMergeImages ( img , cimg , 0 , 0 ) ;
              kgFreeImage ( cimg ) ;
          }
          return img;
          default:
          if ( cimg != NULL ) {
              img = kgFilledStringToImage1 ( NULL , NULL , width , height , font , fillcolor , \
                   highli , color , D->gc.fill_clr , justfic , FontSize , rfac , state , 3.0 ) ;
              img = kgMergeImages ( img , cimg , 0 , 0 ) ;
              kgFreeImage ( cimg ) ;
              if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) {
 //                 cimg = kgStringToImage ( str , NULL , ln , height , font , color , justfic , FontSize , -1 ) ;
                  cimg = uiStringToImage ( D , str , 0 , 0 , ln , height , \
                       font , color , FontSize , justfic , -1 ) ;
                  if ( cimg != NULL ) {
                      img = kgMergeImages ( img , cimg , off , 0 ) ;
                      kgFreeImage ( cimg ) ;
                  }
              }
          }
          else {
              img = kgFilledStringToImage1 ( str , NULL , width , height , font , fillcolor , \
                   highli , color , D->gc.fill_clr , justfic , FontSize , rfac , state , 3.0 ) ;
          }
          return img;
      }
  }
  void uimenuch ( DIALOG *D , char c , int ix1 , int iy1 , int char_clr ) {
      short jj , k , i , iy;
      char str [ 3 ] ;
      unsigned int temp;
      temp = WC ( D )->c_color;
      uiset_clr ( D , char_clr ) ;
      if ( ( c >= 33 ) ) {
          iy = D->evgay-iy1-11;
          str [ 0 ] = c , str [ 1 ] = '\0';
          if ( c == '!' ) {str [ 1 ] = '!' , str [ 2 ] = '\0'; }
          uiDraw_String ( D , ix1+2 , D->evgay-iy1 , str , -1 ) ;
      }
      uiset_clr ( D , temp ) ;
      return;
  }
  void uiPutString ( DIALOG *D , char *str , int x1 , int y1 , \
       int char_clr , int Font , int FontSize ) \
  {
 /* writes a string in fixed font mod */
      void *fid , *img;
      int jj , k , i , iyi , ln;
      unsigned int temp;
      char stmp [ 2 ] ;
      int xsize , ysize;
      float th , tw , tg , xx , yy; ;
      ln = strlen ( str ) ;
      if ( ln > 0 ) {
          xsize = ln*FontSize+FontSize/2;
          ysize = 2.2*FontSize;
          stmp [ 1 ] = '\0';
          fid = kgInitImage ( ( int ) ( xsize ) , ysize , TRESIZE ) ;
          kgUserFrame ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize ) ;
          th = FontSize*1.3;
          tw = FontSize+1;
          kgTextFont ( fid , Font ) ;
          kgTextSize ( fid , th , tw , GAP*tw ) ;
          kgTextColor ( fid , char_clr ) ;
          xx = 0.0;
          xx = FontSize/2;
          yy = 0.8*FontSize;
          i = 0;
          while ( str [ i ] >= ' ' ) {
              kgMove2f ( fid , xx , yy ) ;
              stmp [ 0 ] = str [ i ] ;
              kgWriteText ( fid , stmp ) ;
              xx += FontSize;
              i++;
          }
//          img = kgGetResizedImage(fid);
          img = kgGetSharpImage ( fid ) ;
//          img = kgGetSmoothImage ( fid ) ;
          kgCloseImage ( fid ) ;
          kgImage ( D , img , x1 , y1-th , xsize , ysize , 0.0 , 1.0 ) ;
          kgFreeImage ( img ) ;
      }
      return;
  }
  void uiwrite_string ( DIALOG *D , char* c , \
       int ix1 , int iy1 , int char_clr ) \
  {
      short jj , k , i , iy;
      unsigned int temp;
      temp = WC ( D )->c_color;
      uiset_clr ( D , char_clr ) ;
      iy = D->evgay-iy1-11;
 /* Draw_String(ix1+2,EVGAY-iy1,c,-1);*/
      uiDraw_String ( D , ix1+2 , D->evgay-iy1 , c , -1 ) ;
      uiset_clr ( D , temp ) ;
      return;
  }
  void *ui_set_graphics ( int xpos , int ypos , int xres , int yres , \
       char *title , int dec , float transparency ) \
  {
      kgWC *wc;
      wc = ui_create_window ( xpos , ypos , xres , yres , title , \
           dec , transparency , 0 , 0 , 0 , 0 , NULL ) ;
      uiset_sup_clip_limits ( wc , 0 , 0 , wc->EVGAX , wc->EVGAY ) ;
      uiset_clip_limits ( wc , 0 , 0 , wc->EVGAX , wc->EVGAY ) ;
//  ui_initialise(wc);
      return wc;
  }
  void *ui_set_graphics_sticky ( int xpos , int ypos , int xres , int yres , \
       char *title , int dec , float transparency , int sticky , int fixpos , \
       int NoTaskBar , int StackPos , void *Shapexpm ) \
  {
      kgWC *wc;
      wc = ui_create_window ( xpos , ypos , xres , yres , title , dec , transparency , \
           sticky , fixpos , NoTaskBar , StackPos , Shapexpm ) ;
      uiset_sup_clip_limits ( wc , 0 , 0 , wc->EVGAX , wc->EVGAY ) ;
      uiset_clip_limits ( wc , 0 , 0 , wc->EVGAX , wc->EVGAY ) ;
//  ui_initialise(wc);
      return wc;
  }
  void * kgSetupGraphics ( void *Tmp ) {
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      if ( ( D->parent != NULL ) && ( D->SUBWIN == 1 ) ) wc = kgCreateSubWindow \
       ( Tmp ) ;
      else wc = kgCreateWindow ( Tmp ) ;
      uiset_sup_clip_limits ( wc , 0 , 0 , wc->EVGAX , wc->EVGAY ) ;
      uiset_clip_limits ( wc , 0 , 0 , wc->EVGAX , wc->EVGAY ) ;
      return wc;
  }
#if 0
  void Scrn_recover ( ) {
      int x2;
      scr_recover ( ) ;
      Sync ( ) ;
      gsync ( ) ;
  }
#endif
  void uiScrn_recover ( kgWC *wc ) {
      int x2;
      kg_scr_recover ( wc ) ;
  }
#if 0
  void df_clr ( int i , int ir , int ib , int ig ) {
      chng_clr ( i , ir , ib , ig ) ;
  }
#endif
  void uiget_viewport ( DIG *G , int *x0 , int *y0 , int *x1 , int *y1 ) {
      kgDC *dc;
      dc = G->dc;
      *x0 = dc->v_x1 , *x1 = dc->v_x2;
      *y0 = dc->v_y1 , *y1 = dc->v_y2;
  }
/********************************************************
 Displaying an xpm image in the rectangle defined
 in user coordinate
*********************************************************/
#if 0
  int XpmImage ( char *flname , float x0 , float y0 , float x1 , float y1 ) {
      int xo , yo , w , h , ix0 , iy0 , ix1 , iy1;
      int i;
      char **pt;
      ix0 = scr_x ( x0 ) ;
      iy0 = EVGAY-scr_y ( y0 ) ;
      ix1 = scr_x ( x1 ) ;
      iy1 = EVGAY-scr_y ( y1 ) ;
      w = abs ( ix0 - ix1 ) ;
      h = abs ( iy0 - iy1 ) ;
      xo = ix0;
      if ( ix1 < xo ) xo = ix1;
      yo = iy0;
      if ( iy1 < yo ) yo = iy1;
      pt = ReadXpmFile ( flname ) ;
      if ( pt != NULL ) {
          SetImageClip ( c_v_x1 , c_v_y1 , c_v_x2 , c_v_y2 ) ;
          xpm_image ( pt , xo , yo , w , h ) ;
          i = 0;
          while ( pt [ i ] != NULL ) free ( pt [ i++ ] ) ;
          free ( pt ) ;
      }
      return 1;
  }
  int XpmImageScaled ( char *flname , float x0 , \
       float y0 , float x1 , float y1 ) \
  {
      int xo , yo , w , h , ix0 , iy0 , ix1 , iy1;
      int i;
      char **pt;
      ix0 = scr_x ( x0 ) ;
      iy0 = EVGAY-scr_y ( y0 ) ;
      ix1 = scr_x ( x1 ) ;
      iy1 = EVGAY-scr_y ( y1 ) ;
      w = abs ( ix0 - ix1 ) ;
      h = abs ( iy0 - iy1 ) ;
      xo = ix0;
      if ( ix1 < xo ) xo = ix1;
      yo = iy0;
      if ( iy1 < yo ) yo = iy1;
      pt = ReadXpmFile ( flname ) ;
      if ( pt != NULL ) {
          SetImageClip ( c_v_x1 , c_v_y1 , c_v_x2 , c_v_y2 ) ;
          xpm_image_scaled ( pt , xo , yo , w , h ) ;
          i = 0;
          while ( pt [ i ] != NULL ) free ( pt [ i++ ] ) ;
          free ( pt ) ;
      }
      return 1;
  }
  int XpmImageRotated ( char *flname , float x0 , float y0 , \
       float x1 , float y1 , float ang ) \
  {
      int xo , yo , w , h , ix0 , iy0 , ix1 , iy1;
      int i;
      char **pt;
      ix0 = scr_x ( x0 ) ;
      iy0 = EVGAY-scr_y ( y0 ) ;
      ix1 = scr_x ( x1 ) ;
      iy1 = EVGAY-scr_y ( y1 ) ;
      w = abs ( ix0 - ix1 ) ;
      h = abs ( iy0 - iy1 ) ;
      xo = ix0;
      if ( ix1 < xo ) xo = ix1;
      yo = iy0;
      if ( iy1 < yo ) yo = iy1;
      pt = ReadXpmFile ( flname ) ;
      if ( pt != NULL ) {
    /* xpm_image_scaled(pt,xo,yo,w,h);*/
          SetImageClip ( c_v_x1 , c_v_y1 , c_v_x2 , c_v_y2 ) ;
          xpm_image_rotated ( pt , xo , yo , w , h , ang ) ;
          i = 0;
          while ( pt [ i ] != NULL ) free ( pt [ i++ ] ) ;
          free ( pt ) ;
      }
      return 1;
  }
#endif
#if 0
  int ImgImageScaled ( char *flname , float x0 , \
       float y0 , float x1 , float y1 ) \
  {
      int xo , yo , w , h , ix0 , iy0 , ix1 , iy1;
      int i;
      ix0 = scr_x ( x0 ) ;
      iy0 = EVGAY-scr_y ( y0 ) ;
      ix1 = scr_x ( x1 ) ;
      iy1 = EVGAY-scr_y ( y1 ) ;
      w = abs ( ix0 - ix1 ) ;
      h = abs ( iy0 - iy1 ) ;
      xo = ix0;
      if ( ix1 < xo ) xo = ix1;
      yo = iy0;
      if ( iy1 < yo ) yo = iy1;
      SetImageClip ( c_v_x1 , c_v_y1 , c_v_x2 , c_v_y2 ) ;
      img_image_scaled ( flname , xo , yo , w , h ) ;
      return 1;
  }
  int ImgImageRotated ( char *flname , float x0 , float y0 , \
       float x1 , float y1 , float ang ) \
  {
      int xo , yo , w , h , ix0 , iy0 , ix1 , iy1;
      int i;
      ix0 = scr_x ( x0 ) ;
      iy0 = EVGAY-scr_y ( y0 ) ;
      ix1 = scr_x ( x1 ) ;
      iy1 = EVGAY-scr_y ( y1 ) ;
      w = abs ( ix0 - ix1 ) ;
      h = abs ( iy0 - iy1 ) ;
      xo = ix0;
      if ( ix1 < xo ) xo = ix1;
      yo = iy0;
      if ( iy1 < yo ) yo = iy1;
      SetImageClip ( c_v_x1 , c_v_y1 , c_v_x2 , c_v_y2 ) ;
      img_image_rotated ( flname , xo , yo , w , h , ang ) ;
      return 1;
  }
#endif
/**********************************************
 Freeing an image data created using
 ReadXpmFile
***********************************************/
  int FreeImage ( char **pt ) {
      int i = 0;
      if ( pt != NULL ) {
          i = 0;
          while ( pt [ i ] != NULL ) {
              free ( pt [ i++ ] ) ;
          }
          free ( pt ) ;
      }
      return 1;
  }
#if 0
  int GrabImage ( float x0 , float y0 , float x1 , float y1 ) {
      int xo , yo , xu , yu , w , h , ix0 , iy0 , ix1 , iy1;
      int i , ret;
      ix0 = scr_x ( x0 ) ;
      iy0 = EVGAY-scr_y ( y0 ) ;
      ix1 = scr_x ( x1 ) ;
      iy1 = EVGAY-scr_y ( y1 ) ;
      xo = ix0; xu = ix1;
      if ( ix1 < xo ) {xo = ix1; xu = ix0; }
      yo = iy0; yu = iy1;
      if ( iy1 < yo ) {yo = iy1; yu = iy0; }
      if ( xo < c_v_x1 ) xo = c_v_x1;
      if ( yo < c_v_y1 ) yo = c_v_y1;
      if ( xu > c_v_x2 ) xu = c_v_x2;
      if ( yu > c_v_y2 ) yu = c_v_y2;
      w = xu - xo;
      h = yu - yo;
      ret = grab_image ( xo , yo , w , h ) ;
      return ret;
  }
  int CopyImage ( int no , float x0 , float y0 , float x1 , float y1 ) {
      int xo , yo , xu , yu , w , h , ix0 , iy0 , ix1 , iy1;
      int i , ret;
      ix0 = scr_x ( x0 ) ;
      iy0 = EVGAY-scr_y ( y0 ) ;
      ix1 = scr_x ( x1 ) ;
      iy1 = EVGAY-scr_y ( y1 ) ;
      xo = ix0; xu = ix1;
      if ( ix1 < xo ) {xo = ix1; xu = ix0; }
      yo = iy0; yu = iy1;
      if ( iy1 < yo ) {yo = iy1; yu = iy0; }
      if ( xo < c_v_x1 ) xo = c_v_x1;
      if ( yo < c_v_y1 ) yo = c_v_y1;
      if ( xu > c_v_x2 ) xu = c_v_x2;
      if ( yu > c_v_y2 ) yu = c_v_y2;
      w = xu - xo;
      h = yu - yo;
      put_image ( no , xo , yo , w , h ) ;
      return no;
  }
#endif
#ifdef ZBUFFER
#include "txlib3d.c"
#else
#include "txlib3ddum.c"
#endif
/******************************************************
   JPEG CODE
*******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*  GUI drawing            */
  void uidia_message ( DIALOG *D , int x , int y , char *m ) {
      int temp;
      uiwrite_string ( D , m , x , D->evgay-y , D->gc.txt_pchar ) ;
  }
  void _uiborder3d ( void *parent , int xmin , \
       int ymin , int xmax , int ymax ) \
  {
      int temp;
      DIALOG *D;
      D = ( DIALOG * ) ( parent ) ;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.vbright ) ;
      _ui_h_line ( WC ( D ) , xmin , xmax , ymin ) ;
      _ui_v_line ( WC ( D ) , ymin , ymax , xmin ) ;
      _ui_h_line ( WC ( D ) , xmin+4 , xmax-4 , ymax+4 ) ;
      _ui_v_line ( WC ( D ) , ymin-4 , ymax+4 , xmax-4 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _ui_h_line ( WC ( D ) , xmin , xmax , ymax ) ;
      _ui_v_line ( WC ( D ) , ymin , ymax , xmax ) ;
      _ui_h_line ( WC ( D ) , xmin+4 , xmax-4 , ymin-4 ) ;
      _ui_v_line ( WC ( D ) , ymin-4 , ymax+4 , xmin+4 ) ;
      uiset_clr ( D , temp ) ;
  }
  void _uibordertype0 ( DIALOG *D , int xmin , int ymin , \
       int xmax , int ymax , int bright ) \
  {
      int temp;
      temp = WC ( D )->c_color;
      uiset_clr ( D , 0 ) ;
      _ui_h_line ( WC ( D ) , xmin , xmax , ymin ) ;
      _ui_v_line ( WC ( D ) , ymin , ymax , xmin ) ;
      uiset_clr ( D , bright ) ;
      _ui_h_line ( WC ( D ) , xmin , xmax , ymax ) ;
      _ui_v_line ( WC ( D ) , ymin , ymax , xmax ) ;
      uiset_clr ( D , temp ) ;
  }
  void _uibordertype1 ( DIALOG *D , int xmin , \
       int ymin , int xmax , int ymax ) \
  {
      int temp;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.bright ) ;
      _ui_h_line ( WC ( D ) , xmin , xmax , ymin ) ;
      _ui_v_line ( WC ( D ) , ymin , ymax , xmin ) ;
      _ui_h_line ( WC ( D ) , xmin , xmax , ymax ) ;
      _ui_v_line ( WC ( D ) , ymin , ymax , xmax ) ;
      uiset_clr ( D , temp ) ;
  }
  void _uibordertype2 ( DIALOG *D , int xmin , int ymin , int xmax , \
       int ymax , int tw , int lw , int rw , int bw ) \
  {
      int temp;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.vbright ) ;
      _ui_h_line ( WC ( D ) , xmin , xmax , ymin ) ;
      _ui_v_line ( WC ( D ) , ymin , ymax , xmin ) ;
      _ui_h_line ( WC ( D ) , xmin+lw , xmax-rw , ymax-bw ) ;
      _ui_v_line ( WC ( D ) , ymin+tw , ymax-bw , xmax-rw ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _ui_h_line ( WC ( D ) , xmin , xmax , ymax ) ;
      _ui_v_line ( WC ( D ) , ymin , ymax , xmax ) ;
      _ui_h_line ( WC ( D ) , xmin+lw , xmax-rw , ymin+tw ) ;
      _ui_v_line ( WC ( D ) , ymin+tw , ymax-bw , xmin+lw ) ;
      uiset_clr ( D , temp ) ;
  }
  void _uibordertype3 ( DIALOG *D , int xmin , int ymin , int xmax , \
       int ymax , int tw , int lw , int rw , int bw ) \
  {
      int temp;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.vbright ) ;
      _ui_h_line ( WC ( D ) , xmin , xmax , ymin ) ;
      _ui_v_line ( WC ( D ) , ymax , ymin , xmin ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _ui_h_line ( WC ( D ) , xmin+1 , xmax-1 , ymin+1 ) ;
      _ui_v_line ( WC ( D ) , ymax-1 , ymin+1 , xmin+1 ) ;
      _ui_h_line ( WC ( D ) , xmin+2 , xmax-2 , ymin+2 ) ;
      _ui_v_line ( WC ( D ) , ymax-2 , ymin+2 , xmin+2 ) ;
      uiset_clr ( D , D->gc.fill_clr ) ;
      _ui_h_line ( WC ( D ) , xmin+3 , xmax-3 , ymin+3 ) ;
      _ui_v_line ( WC ( D ) , ymax-3 , ymin+3 , xmin+3 ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _ui_h_line ( WC ( D ) , xmin+4 , xmax-4 , ymin-4 ) ;
      _ui_v_line ( WC ( D ) , ymax+4 , ymin-4 , xmin+4 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _ui_h_line ( WC ( D ) , xmin , xmax , ymax ) ;
      _ui_v_line ( WC ( D ) , ymax , ymin , xmax ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _ui_h_line ( WC ( D ) , xmin+1 , xmax-1 , ymax+1 ) ;
      _ui_v_line ( WC ( D ) , ymax+1 , ymin-1 , xmax-1 ) ;
      uiset_clr ( D , D->gc.fill_clr ) ;
      _ui_h_line ( WC ( D ) , xmin+2 , xmax-2 , ymax+2 ) ;
      _ui_v_line ( WC ( D ) , ymax+2 , ymin-2 , xmax-2 ) ;
      uiset_clr ( D , D->gc.bright ) ;
      _ui_h_line ( WC ( D ) , xmin+3 , xmax-3 , ymax+3 ) ;
      _ui_v_line ( WC ( D ) , ymax+3 , ymin-3 , xmax-3 ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _ui_h_line ( WC ( D ) , xmin+4 , xmax-4 , ymax+4 ) ;
      _ui_v_line ( WC ( D ) , ymax+4 , ymin-4 , xmax-4 ) ;
      uiset_clr ( D , temp ) ;
  }
  void _uibordertype4 ( DIALOG *D , int xmin , \
       int ymin , int xmax , int ymax ) \
  {
      int temp;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.bright ) ;
      _ui_h_line ( WC ( D ) , xmin , xmax , ymin ) ;
      _ui_v_line ( WC ( D ) , ymin , ymax+1 , xmin ) ;
      uiset_clr ( D , D->gc.bright ) ;
      _ui_h_line ( WC ( D ) , xmin+1 , xmax-1 , ymin-1 ) ;
      _ui_v_line ( WC ( D ) , ymin-1 , ymax+2 , xmin+1 ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _ui_h_line ( WC ( D ) , xmin+2 , xmax-2 , ymin-1 ) ;
      _ui_v_line ( WC ( D ) , ymin-2 , ymax+3 , xmin+1 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _ui_h_line ( WC ( D ) , xmin , xmax , ymax ) ;
      _ui_v_line ( WC ( D ) , ymin , ymax , xmax ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _ui_h_line ( WC ( D ) , xmin+1 , xmax-1 , ymax+1 ) ;
      _ui_v_line ( WC ( D ) , ymin-1 , ymax+1 , xmax-1 ) ;
      uiset_clr ( D , temp ) ;
  }
  void _dvbordertype0 ( DIALOG *D , int xmin , int ymin , \
       int xmax , int ymax , int bright ) \
  {
      int temp;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.dim ) ;
      _dv_h_line ( WC ( D ) , xmin , xmax , ymin ) ;
      _dv_v_line ( WC ( D ) , ymin , ymax , xmin ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _dv_h_line ( WC ( D ) , xmin+1 , xmax-1 , ymin+1 ) ;
      _dv_v_line ( WC ( D ) , ymin+1 , ymax-1 , xmin+1 ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _dv_h_line ( WC ( D ) , xmin , xmax , ymax ) ;
      _dv_v_line ( WC ( D ) , ymin , ymax , xmax ) ;
      uiset_clr ( D , D->gc.bright ) ;
      _dv_h_line ( WC ( D ) , xmin+1 , xmax-1 , ymax-1 ) ;
      _dv_v_line ( WC ( D ) , ymin+1 , ymax-1 , xmax-1 ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _dv_h_line ( WC ( D ) , xmin+2 , xmax-2 , ymax-2 ) ;
      _dv_v_line ( WC ( D ) , ymin+2 , ymax-2 , xmax-2 ) ;
      uiset_clr ( D , temp ) ;
  }
  void _dvbordertype1 ( DIALOG *D , int xmin , \
       int ymin , int xmax , int ymax ) \
  {
      int temp;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.bright ) ;
      _dv_h_line ( WC ( D ) , xmin , xmax , ymin ) ;
      _dv_v_line ( WC ( D ) , ymin , ymax , xmin ) ;
      _dv_h_line ( WC ( D ) , xmin , xmax , ymax ) ;
      _dv_v_line ( WC ( D ) , ymin , ymax , xmax ) ;
      uiset_clr ( D , temp ) ;
  }
  void _dvbordertype2 ( DIALOG *D , int xmin , int ymin , int xmax , \
       int ymax , int tw , int lw , int rw , int bw ) \
  {
      int temp;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.vbright ) ;
      _dv_h_line ( WC ( D ) , xmin , xmax , ymin ) ;
      _dv_v_line ( WC ( D ) , ymin , ymax , xmin ) ;
      _dv_h_line ( WC ( D ) , xmin+lw , xmax-rw , ymax-bw ) ;
      _dv_v_line ( WC ( D ) , ymin+tw , ymax-bw , xmax-rw ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _dv_h_line ( WC ( D ) , xmin , xmax , ymax ) ;
      _dv_v_line ( WC ( D ) , ymin , ymax , xmax ) ;
      _dv_h_line ( WC ( D ) , xmin+lw , xmax-rw , ymin+tw ) ;
      _dv_v_line ( WC ( D ) , ymin+tw , ymax-bw , xmin+lw ) ;
      uiset_clr ( D , temp ) ;
  }
  void _dvbordertype3 ( DIALOG *D , int xmin , \
       int ymin , int xmax , int ymax ) \
  {
      int temp;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.vbright ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _dv_h_line ( WC ( D ) , xmin , xmax , ymin ) ;
      _dv_v_line ( WC ( D ) , ymax , ymin , xmin ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _dv_h_line ( WC ( D ) , xmin+1 , xmax-1 , ymin+1 ) ;
      _dv_v_line ( WC ( D ) , ymax-1 , ymin+1 , xmin+1 ) ;
      _dv_h_line ( WC ( D ) , xmin+2 , xmax-2 , ymin+2 ) ;
      _dv_v_line ( WC ( D ) , ymax-2 , ymin+2 , xmin+2 ) ;
      uiset_clr ( D , D->gc.fill_clr ) ;
      _dv_h_line ( WC ( D ) , xmin+3 , xmax-3 , ymin+3 ) ;
      _dv_v_line ( WC ( D ) , ymax-3 , ymin+3 , xmin+3 ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _dv_h_line ( WC ( D ) , xmin+4 , xmax-4 , ymin+4 ) ;
      _dv_v_line ( WC ( D ) , ymax-4 , ymin+4 , xmin+4 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _dv_h_line ( WC ( D ) , xmin , xmax , ymax ) ;
      _dv_v_line ( WC ( D ) , ymax , ymin , xmax ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _dv_h_line ( WC ( D ) , xmin+1 , xmax-1 , ymax-1 ) ;
      _dv_v_line ( WC ( D ) , ymax-1 , ymin+1 , xmax-1 ) ;
      uiset_clr ( D , D->gc.fill_clr ) ;
      _dv_h_line ( WC ( D ) , xmin+2 , xmax-2 , ymax-2 ) ;
      _dv_v_line ( WC ( D ) , ymax-2 , ymin+2 , xmax-2 ) ;
      uiset_clr ( D , D->gc.bright ) ;
      _dv_h_line ( WC ( D ) , xmin+3 , xmax-3 , ymax-3 ) ;
      _dv_v_line ( WC ( D ) , ymax-3 , ymin+3 , xmax-3 ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _dv_h_line ( WC ( D ) , xmin+4 , xmax-4 , ymax-4 ) ;
      _dv_v_line ( WC ( D ) , ymax-4 , ymin+4 , xmax-4 ) ;
      uiset_clr ( D , temp ) ;
  }
  void _dvbordertype4 ( DIALOG *D , int xmin , \
       int ymin , int xmax , int ymax ) \
  {
      int temp;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.bright ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _dv_h_line ( WC ( D ) , xmin , xmax , ymin ) ;
      _dv_v_line ( WC ( D ) , ymin , ymax , xmin ) ;
      uiset_clr ( D , D->gc.bright ) ;
      _dv_h_line ( WC ( D ) , xmin+1 , xmax-1 , ymin+1 ) ;
      _dv_v_line ( WC ( D ) , ymin+1 , ymax-1 , xmin+1 ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _dv_h_line ( WC ( D ) , xmin+2 , xmax-2 , ymin+2 ) ;
      _dv_v_line ( WC ( D ) , ymin+2 , ymax-2 , xmin+2 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _dv_h_line ( WC ( D ) , xmin , xmax , ymax ) ;
      _dv_v_line ( WC ( D ) , ymin , ymax , xmax ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _dv_h_line ( WC ( D ) , xmin+1 , xmax-1 , ymax-1 ) ;
      _dv_v_line ( WC ( D ) , ymin+1 , ymax-1 , xmax-1 ) ;
      uiset_clr ( D , temp ) ;
  }
  void _ui_Draw_Dialog_Area ( DIALOG *D ) {
      unsigned int temp;
      int cx1 , cy1 , cx2 , cy2;
      int x1 , y1 , x2 , y2;
      kgWC *wc;
      wc = WC ( D ) ;
      x1 = D->xo , y1 = D->yo , x2 = D->xo+D->xl-1 , y2 = D->yo+D->yl-1;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _dvrect_fill ( wc , x1 , y1 , x2 , y2 , D->gc.fill_clr ) ;
      switch ( D->bor_type ) {
          case 2:
          _dvbordertype2 ( D , x1 , y1 , x2 , y2 , D->tw , D->lw , D->rw , D->bw ) ;
          break;
          case 3:
          _dvbordertype3 ( D , x1 , y1 , x2 , y2 ) ;
          break;
          case 4:
          _dvbordertype4 ( D , x1 , y1 , x2 , y2 ) ;
          break;
          case 1:
          _dvbordertype1 ( D , x1 , y1 , x2 , y2 ) ;
          break;
          default:
          break;
      }
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void _ui_Draw_Dialog_Area_Transparent ( DIALOG *D ) {
      unsigned int temp;
      int cx1 , cy1 , cx2 , cy2;
      int x1 , y1 , x2 , y2;
      kgWC *wc;
      wc = WC ( D ) ;
      x1 = D->xo , y1 = D->yo , x2 = D->xo+D->xl-1 , y2 = D->yo+D->yl-1;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _dvrect_fill_transparent ( wc , x1 , y1 , x2 , \
           y2 , D->gc.fill_clr , D->transparency ) ;
      switch ( D->bor_type ) {
          case 2:
          _dvbordertype2 ( D , x1 , y1 , x2 , y2 , D->tw , D->lw , D->rw , D->bw ) ;
          break;
          case 3:
          _dvbordertype3 ( D , x1 , y1 , x2 , y2 ) ;
          break;
          case 4:
          _dvbordertype4 ( D , x1 , y1 , x2 , y2 ) ;
          break;
          case 1:
          _dvbordertype1 ( D , x1 , y1 , x2 , y2 ) ;
          break;
          case 0:
          default:
          break;
      }
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void _ui_rmv_attention ( DIALOG *D , int x1 , int y1 , int x2 , int y2 ) {
      unsigned int temp;
      int EVGAY;
      D = ( DIALOG * ) ( D ) ;
      kgWC *wc;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.fill_clr ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _dv_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) ( y1 ) ) ;
      _dv_v_line ( wc , ( int ) ( y1 ) , ( int ) ( y2 ) , ( int ) x1 ) ;
      _dv_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) ( y2 ) ) ;
      _dv_v_line ( wc , ( int ) ( y1 ) , ( int ) ( y2 ) , ( int ) x2 ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
      return ;
  }
  void _ui_draw_attention ( DIALOG *D , int x1 , int y1 , int x2 , int y2 ) {
      unsigned int temp;
      DIALOG *Parent;
      kgWC *wc;
      wc = WC ( D ) ;
      Parent = ( DIALOG * ) ( D ) ;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.v_dim ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _dv_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) ( y1 ) ) ;
      _dv_v_line ( wc , ( int ) ( y1 ) , ( int ) ( y2 ) , ( int ) x1 ) ;
      _dv_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) ( y2 ) ) ;
      _dv_v_line ( wc , ( int ) ( y1 ) , ( int ) ( y2 ) , ( int ) x2 ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
      return ;
  }
  void * make_but_str ( DIALOG *D , int x1 , int y1 , int xgap , int ygap , \
       int nxb , int nyb , int b_w , int b_h , int df , char **title , \
       void **xpm , int *bkgr , int *sw ) \
  {
      int i = 0 , k , x2 , y2;
      int EVGAY;
      BUTS *ptr;
      ptr = ( BUTS * ) Malloc ( sizeof ( BUTS ) ) ;
      EVGAY = D->evgay;
      x2 = nxb* ( b_w+xgap ) +x1-xgap;
      y2 = nyb* ( b_h+ygap ) +y1-ygap;
      ptr->x2 = x2;
      ptr->y1 = EVGAY-y2;
      ptr->nxb = nxb;
      ptr->nyb = nyb;
      ptr->b_w = b_w;
      ptr->b_h = b_h;
      ptr->df = df;
      ptr->x1 = x1;
      ptr->y2 = EVGAY-y1;
      ptr->title = title;
      ptr->xgap = xgap;
      ptr->ygap = ygap;
      ptr->xpm = xpm;
      ptr->bkgr = bkgr;
      if ( sw == NULL ) {
          ptr->sw = ( int * ) Malloc ( sizeof ( int ) *nxb*nyb ) ;
          for ( k = 0; k < ( nxb*nyb ) ; k++ ) ptr->sw [ k ] = 1;
      }
      else {
          ptr->sw = sw;
      }
      ptr->Hsize = 1;
      return ( void * ) ptr;
  }
  void _ui_jpg_draw ( BUTS *bt , int k , int x1 , \
       int y1 , int x2 , int y2 , int color ) \
  {
      char **buf;
      int x0 , y0 , Hsize;
      int l , w;
      DIALOG *D;
      int EVGAY;
      kgWC *wc;
      D = ( DIALOG * ) ( bt->D ) ;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      buf = ( char ** ) ( bt->xpm [ k ] ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      Hsize = bt->Hsize;
      l = x2 -x1;
      w = y2 -y1;
      if ( l < 0 ) l = -l;
      if ( w < 0 ) w = -w;
      x0 = x1; y0 = y1;
      if ( x2 < x0 ) x0 = x2;
      if ( ( y2 ) < y0 ) y0 = y2;
      if ( color >= 0 ) _uibox_fill ( wc , x1+2 , EVGAY-y1+2 , x2-2 , EVGAY-y2-2 , \
       ( unsigned int ) color ) ;
      if ( buf != NULL ) {
//        printf("Calling kgImage\n");
          kgImage ( D , bt->xpm [ k ] , x0+1 , y0+1 , l-2 , w-2 , 0.0 , 1.0 ) ;
      }
      uiRest_clip_limits ( wc ) ;
  }
  void _ui_jpg_newdraw ( BUTS *bt , int k , int x1 , int y1 , int x2 , \
       int y2 , int color , float transparency , float highfac ) \
  {
      char **buf;
      int x0 , y0 , Hsize;
      int l , w;
      DIALOG *D;
      int EVGAY;
      kgWC *wc;
      D = ( DIALOG * ) ( bt->D ) ;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      buf = ( char ** ) ( bt->xpm [ k ] ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      Hsize = bt->Hsize;
      l = x2 -x1;
      w = y2 -y1;
      if ( l < 0 ) l = -l;
      if ( w < 0 ) w = -w;
      x0 = x1; y0 = y1;
      if ( x2 < x0 ) x0 = x2;
      if ( ( y2 ) < y0 ) y0 = y2;
// Should not be done
//   if(color >= 0) _uibox_fill(wc,x1+2,EVGAY-y1+2,x2-2,EVGAY-y2-2,(unsigned int) color);
      if ( buf != NULL ) kgImage ( D , bt->xpm [ k ] , x0+1 , \
           y0+1 , l-2 , w-2 , transparency , highfac ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _ui_rmv_inset_protru ( DIALOG *D , int x1 , int y1 , int x2 , int y2 ) {
      unsigned int temp;
      kgWC *wc;
      wc = WC ( D ) ;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.fill_clr ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y1 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x1 ) ;
      _ui_h_line ( wc , ( int ) x1+1 , ( int ) x2-1 , ( int ) y2+1 ) ;
      _ui_v_line ( wc , ( int ) y1-1 , ( int ) y2+1 , ( int ) x2-1 ) ;
      uiset_clr ( D , D->gc.fill_clr ) ;
      _ui_h_line ( wc , ( int ) x1+1 , ( int ) x2-1 , ( int ) y1-1 ) ;
      _ui_v_line ( wc , ( int ) y1-1 , ( int ) y2+1 , ( int ) x1+1 ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y2 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x2 ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void uidraw_depre ( DIALOG *Dtmp , int x1 , int y1 , int x2 , int y2 ) {
#if 0
      unsigned int temp;
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) ( Dtmp ) ;
      wc = WC ( D ) ;
      temp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y1 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x1 ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y2 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x2 ) ;
      uiset_clr ( D , temp ) ;
//TCB  doublt full
      uiRest_clip_limits ( wc ) ;
#else
      int EVGAY;
      EVGAY = Dtmp->evgay;
      dvdraw_depre ( Dtmp , x1 , EVGAY-y1 , x2 , EVGAY-y2 ) ;
#endif
  }
  void dvdraw_depre ( DIALOG *Dtmp , int x1 , int y1 , int x2 , int y2 ) {
      unsigned int temp;
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) ( Dtmp ) ;
      wc = WC ( D ) ;
      temp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _dv_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y1 ) ;
      _dv_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x1 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _dv_h_line ( wc , ( int ) x1+1 , ( int ) x2-1 , ( int ) y1+1 ) ;
      _dv_v_line ( wc , ( int ) y1+1 , ( int ) y2-1 , ( int ) x1+1 ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _dv_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y2 ) ;
      _dv_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x2 ) ;
      uiset_clr ( D , D->gc.bright ) ;
      _dv_h_line ( wc , ( int ) x1+1 , ( int ) x2-1 , ( int ) y2-1 ) ;
      _dv_v_line ( wc , ( int ) y1+1 , ( int ) y2-1 , ( int ) x2-1 ) ;
      uiset_clr ( D , temp ) ;
//TCB  doublt full
      uiRest_clip_limits ( wc ) ;
  }
  void dvdraw_protru ( DIALOG *Dtmp , int x1 , int y1 , int x2 , int y2 ) {
      unsigned int temp;
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) ( Dtmp ) ;
      wc = WC ( D ) ;
      temp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _dv_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y1 ) ;
      _dv_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x1 ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _dv_h_line ( wc , ( int ) x1+1 , ( int ) x2-1 , ( int ) y1+1 ) ;
      _dv_v_line ( wc , ( int ) y1+1 , ( int ) y2-1 , ( int ) x1+1 ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _dv_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y2 ) ;
      _dv_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x2 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _dv_h_line ( wc , ( int ) x1+1 , ( int ) x2-1 , ( int ) y2-1 ) ;
      _dv_v_line ( wc , ( int ) y1+1 , ( int ) y2-1 , ( int ) x2-1 ) ;
      uiset_clr ( D , temp ) ;
//TCB  doublt full
      uiRest_clip_limits ( wc ) ;
  }
  void uidraw_proj ( DIALOG *Dtmp , int x1 , int y1 , int x2 , int y2 ) {
      unsigned int temp;
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) ( Dtmp ) ;
      wc = WC ( D ) ;
      temp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y1 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x1 ) ;
      uiset_clr ( D , D->gc.bright ) ;
      _ui_h_line ( wc , ( int ) x1+1 , ( int ) x2-1 , ( int ) y1+1 ) ;
      _ui_v_line ( wc , ( int ) y1+1 , ( int ) y2-1 , ( int ) x1+1 ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _ui_h_line ( wc , ( int ) x1+1 , ( int ) x2-1 , ( int ) y2-1 ) ;
      _ui_v_line ( wc , ( int ) y1+1 , ( int ) y2-1 , ( int ) x2-1 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y2 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x2 ) ;
      uiset_clr ( D , temp ) ;
//TCB  doublt full
      uiRest_clip_limits ( wc ) ;
  }
  void uidraw_inset ( DIALOG *D , int x1 , int y1 , int x2 , int y2 ) {
      unsigned int temp;
      kgWC *wc;
      wc = WC ( D ) ;
      temp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y1 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x1 ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _ui_h_line ( wc , ( int ) x1+1 , ( int ) x2-1 , ( int ) y1-1 ) ;
      _ui_v_line ( wc , ( int ) y1-1 , ( int ) y2+1 , ( int ) x1+1 ) ;
      uiset_clr ( D , D->gc.bright ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y2 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x2 ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2-1 , ( int ) y2+1 ) ;
      _ui_v_line ( wc , ( int ) y1-1 , ( int ) y2+1 , ( int ) x2-1 ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void uidraw_protru ( DIALOG *D , int x1 , int y1 , int x2 , int y2 ) {
      unsigned int temp;
      kgWC *wc;
      wc = WC ( D ) ;
      temp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y1 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x1 ) ;
      uiset_clr ( D , D->gc.bright ) ;
      _ui_h_line ( wc , ( int ) x1+1 , ( int ) x2-1 , ( int ) y2+1 ) ;
      _ui_v_line ( wc , ( int ) y1-1 , ( int ) y2+1 , ( int ) x2-1 ) ;
      uiset_clr ( D , D->gc.dim ) ;
      _ui_h_line ( wc , ( int ) x1+1 , ( int ) x2-2 , ( int ) y1-1 ) ;
      _ui_v_line ( wc , ( int ) y1-1 , ( int ) y2+2 , ( int ) x1+1 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y2 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x2 ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void uirmv_inset_protru ( DIALOG *D , int x1 , int y1 , int x2 , int y2 ) {
      unsigned int temp;
      kgWC *wc;
      wc = WC ( D ) ;
      temp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.fill_clr ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y1 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x1 ) ;
      _ui_h_line ( wc , ( int ) x1+1 , ( int ) x2-1 , ( int ) y2+1 ) ;
      _ui_v_line ( wc , ( int ) y1-1 , ( int ) y2+1 , ( int ) x2-1 ) ;
      uiset_clr ( D , D->gc.fill_clr ) ;
      _ui_h_line ( wc , ( int ) x1+1 , ( int ) x2-1 , ( int ) y1-1 ) ;
      _ui_v_line ( wc , ( int ) y1-1 , ( int ) y2+1 , ( int ) x1+1 ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y2 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x2 ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void _uiHighButton ( BUTS *bt , int df ) {
      int xgap = 0 , ygap = 0 , i , j , k , xx , \
           yy , width , height , xd , yd , Hsize;
      int *sw;
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) ( bt->D ) ;
      wc = WC ( D ) ;
      Hsize = bt->Hsize;
      width = bt->b_w; height = bt->b_h;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
//   Get_resolution(&EVGAX,&EVGAY);
      if ( ( bt->nyb ) > 1 ) ygap = ( ( float )  \
          ( ( bt->y2-bt->y1 ) -height*bt->nyb ) / \
       ( bt->nyb-1.0 ) +0.5 ) ;
      if ( ( bt->nxb ) > 1 ) xgap = ( float )  \
          ( ( bt->x2-bt->x1 ) -width*bt->nxb ) / \
       ( bt->nxb-1.0 ) +0.5;
      sw = bt->sw;
      k = 0;
      if ( df != bt->df ) {
          for ( j = 0; j < ( bt->nyb ) ; j++ ) {
              for ( i = 0; i < ( bt->nxb ) ; i++ ) {
                  if ( k+1 == bt->df ) {
                      yy = bt->y2- j* ( bt->b_h+ygap ) -bt->b_h;
                      xx = bt->x1+ i* ( bt->b_w+xgap ) ;
                      if ( sw [ k ] == 1 ) uirmv_inset_protru ( D , xx-Hsize , \
                           yy+height+Hsize , xx+width+Hsize , yy-Hsize ) ;
                  }
                  if ( k+1 == df ) {
                      yy = bt->y2- j* ( bt->b_h+ygap ) -bt->b_h;
                      xx = bt->x1+ i* ( bt->b_w+xgap ) ;
                      if ( sw [ k ] == 1 ) uidraw_depre ( D , xx-Hsize , \
                           yy+height+Hsize , xx+width+Hsize , yy-Hsize ) ;
                  }
                  k++;
              }
          }
      }
      bt->df = df;
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void _uiHighNewButton ( BUTS *bt , int df ) {
      int xgap = 0 , ygap = 0 , i , j , k , xx , yy , width , height;
      int pointer;
      DIALOG *D;
      kgWC *wc;
      int EVGAY;
      D = ( DIALOG * ) ( bt->D ) ;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      width = bt->b_w; height = bt->b_h;
      if ( ( bt->nyb ) > 1 ) ygap = ( ( float )  \
          ( ( bt->y2-bt->y1 ) -height*bt->nyb ) / \
       ( bt->nyb-1.0 ) +0.5 ) ;
      if ( ( bt->nxb ) > 1 ) xgap = ( float )  \
          ( ( bt->x2-bt->x1 ) -width*bt->nxb ) / \
       ( bt->nxb-1.0 ) +0.5;
      k = 0;
      for ( j = 0; j < ( bt->nyb ) ; j++ ) {
          yy = bt->y2- j* ( bt->b_h+ygap ) -bt->b_h;
          for ( i = 0; i < ( bt->nxb ) ; i++ ) {
              xx = bt->x1+ i* ( bt->b_w+xgap ) ;
              _ui_draw_newbutton ( xx , yy+height , xx+width , \
                   yy , bt , k , D->transparency ) ;
#if 0
              if ( ( bt->sw [ k ] == 1 ) || ( bt->sw [ k ] == 3 ) ) {
                  if ( bt->D->butattn ) {
                      if ( k+1 == df ) {
                          uidraw_depre ( D , xx-bt->Hsize , yy+height+bt->Hsize , \
                               xx+width+bt->Hsize , yy-bt->Hsize ) ;
                      }
                      else {
                          uirmv_inset_protru ( D , xx-bt->Hsize , yy+height+bt->Hsize , \
                               xx+width+bt->Hsize , yy-bt->Hsize ) ;
                      }
                  }
              }
#endif
              k++;
          }
      }
      bt->df = df;
  }
  int uiCheckClickPosition ( int x1 , int y1 , int x2 , \
       int y2 , int PON_X , int PON_Y ) \
  {
      int xp , yp;
      xp = PON_X;
      yp = PON_Y;
      if ( ( x1 > ( xp ) ) || ( x2 < ( xp ) ) || ( y2 < \
       ( yp ) ) || ( y1 > ( yp ) ) ) return ( 0 ) ;
      else return ( 1 ) ;
  }
  void uiMenustr ( DIALOG *D , int ixp , int iyp , \
       char *cmenu , int char_clr ) \
  {
      uiwrite_string ( D , cmenu , ( int ) ixp , ( int ) iyp , char_clr ) ;
  }
  void _ui_draw_pressed_button ( int x1 , int y1 , \
       int x2 , int y2 , BUTS *bt , int k ) \
  {
      unsigned int temp , pointer , tempch;
      int bkgr , n;
      DIALOG *D;
      kgWC *wc;
      if ( bt->sw [ k ] < 1 ) return; //not active
//   if(bt->sw[k]!=  1) return; //not active
      D = ( DIALOG * ) ( bt->D ) ;
      wc = WC ( D ) ;
      n = bt->nxb*bt->nyb;
      temp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _uirect_fill ( wc , x1+1 , y1-1 , x2-1 , y2+1 , D->gc.fill_clr ) ;
      if ( bt->xpm != NULL ) {
          if ( bt->bkgr == NULL ) bkgr = -1;
          else bkgr = bt->bkgr [ k ] ;
          if ( ( bt->xpm != NULL ) ) {
              if ( bt->xpm [ k ] != NULL ) _ui_jpg_draw ( bt , k , x1 , \
                   D->evgay-y2+1 , x2 , D->evgay-y1+1 , bkgr ) ;
              else _uirect_fill ( wc , x1+4 , y1-4 , x2-4 , y2+4 , bkgr ) ;
          }
          else if ( bkgr >= 0 ) _uirect_fill ( wc , x1+4 , y1-4 , x2-4 , y2+4 , bkgr ) ;
      }
      if ( bt->sw [ k ] == 1 ) {
//     uiMenustr(D,x1+15,y2+8,bt->title[k],D->gc.but_char);
          uiString ( D , bt->title [ k ] , x1+4 , D->evgay- ( y2+24 ) , ( x2-x1-8 ) , \
               22 , D->gc.ButtonFont , D->gc.but_char , D->gc.FontSize , 0 , -1 ) ;
          uiset_clr ( D , D->gc.v_dim ) ;
          _uidraw_line ( wc , x1+1 , y1-1 , x2-1 , y1-1 ) ;
          _uidraw_line ( wc , x1+1 , y1-1 , x1+1 , y2+1 ) ;
          uiset_clr ( D , D->gc.vbright ) ;
          _uidraw_line ( wc , x1+1 , y2+1 , x2-1 , y2+1 ) ;
          _uidraw_line ( wc , x2-1 , y1-1 , x2-1 , y2+1 ) ;
      }
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _ui_draw_pressed_newbutton ( int x1 , int y1 , int x2 , \
       int y2 , BUTS *bt , int k , float transparency ) \
  {
      unsigned int temp , pointer , tempch;
      int bkgr , lnwidth = 1 , highli = 0 , xgap , ygap;
      int n , l , w , x0 , y0;
      void *xpm = NULL;
      int EVGAY;
      DIALOG *D;
      kgWC *wc;
      if ( bt->sw [ k ] < 1 ) return; //not active
      D = ( DIALOG * ) ( bt->D ) ;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      temp = WC ( D )->c_color;
      x0 = x1; y0 = EVGAY-y1;
      l = abs ( x2-x1 ) ; w = abs ( y2 -y1 ) ;
      n = bt->nxb*bt->nyb;
      if ( ( bt->xpm != NULL ) && ( bt->xpm [ k ] != NULL ) ) xpm = bt->xpm [ k ] ;
      if ( ( xpm != NULL ) ) {
          if ( bt->xpm [ k+n ] != NULL ) xpm = bt->xpm [ k+n ] ;
      }
      highli = 0;
      if ( k+1 == bt->df ) highli = 1;
      if ( D->butattn == 0 ) highli = 0;
      xgap = bt->xgap/2;
      ygap = bt->ygap/2;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _uirect_fill_transparent ( wc , x1-xgap , y1+ygap , x2+xgap , \
           y2-ygap , D->gc.fill_clr , transparency ) ;
      if ( bt->bkgr == NULL ) bkgr = D->gc.fill_clr;
      else bkgr = bt->bkgr [ k ] ;
      if ( bkgr < 0 ) {
          if ( bkgr == -1 ) bkgr = D->gc.fill_clr;
      }
      uiShadedStringImage ( D , bt->title [ k ] , xpm , x0 , ( y0 ) , l , w , \
           D->gc.ButtonFont , bkgr , highli , D->gc.but_char , \
           D->gc.FontSize , 0 , bt->fac , -1 , bt->type ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _ui_draw_bound ( DIALOG *D , int x1 , int y1 , \
       int x2 , int y2 , int c_bound ) \
  {
      unsigned int temp;
      kgWC *wc;
      wc = WC ( D ) ;
      temp = WC ( D )->c_color;
      uiset_clr ( D , c_bound ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y1 ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y2 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x2 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x1 ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void _dv_draw_bound ( DIALOG *D , int x1 , int y1 , \
       int x2 , int y2 , int c_bound ) \
  {
      unsigned int temp;
      kgWC *wc;
      wc = WC ( D ) ;
      temp = WC ( D )->c_color;
      uiset_clr ( D , c_bound ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _dv_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y1 ) ;
      _dv_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y2 ) ;
      _dv_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x2 ) ;
      _dv_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x1 ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void _ui_draw_button ( int x1 , int y1 , int x2 , \
       int y2 , BUTS *bt , int k ) \
  {
      unsigned int temp , pointer , tempch;
      int bkgr;
      DIALOG *D , *Parent;
      kgWC *wc;
      if ( bt->sw [ k ] < 1 ) return; //not active
//   if(bt->sw[k]!=  1) return; //not active
//     printf("Inside _ui_draw_button:title:%s\n",bt->title[k]);
      D = ( DIALOG * ) ( bt->D ) ;
      wc = WC ( D ) ;
      Parent = ( D ) ;
      temp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _uirect_fill ( wc , x1+1 , y1-1 , x2-1 , y2+1 , D->gc.fill_clr ) ;
      if ( bt->xpm != NULL ) {
          if ( bt->bkgr == NULL ) bkgr = -1;
          else bkgr = bt->bkgr [ k ] ;
          if ( ( bt->xpm != NULL ) && ( bt->xpm [ k ] != NULL ) ) _ui_jpg_draw \
           ( bt , k , x1 , D->evgay-y2+1 , x2 , D->evgay-y1+1 , bkgr ) ;
          else if ( bkgr >= 0 ) _uirect_fill ( wc , x1+4 , y1-4 , x2-4 , y2+4 , bkgr ) ;
      }
      if ( bt->sw [ k ] == 1 ) {
//     uiMenustr(D,x1+15,y2+8,bt->title[k],D->gc.but_char);
          uiString ( D , bt->title [ k ] , x1+4 , D->evgay- ( y2+24 ) , ( x2-x1-8 ) , \
               22 , D->gc.ButtonFont , D->gc.but_char , D->gc.FontSize , 0 , -1 ) ;
          uiset_clr ( Parent , D->gc.vbright ) ;
          _uidraw_line ( wc , x1+1 , y1-1 , x2-1 , y1-1 ) ;
          _uidraw_line ( wc , x1+1 , y1-1 , x1+1 , y2+1 ) ;
          uiset_clr ( D , D->gc.v_dim ) ;
          _uidraw_line ( wc , x1+1 , y2+1 , x2-1 , y2+1 ) ;
          _uidraw_line ( wc , x2-1 , y1-1 , x2-1 , y2+1 ) ;
      }
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _ui_redraw_button ( int x1 , int y1 , \
       int x2 , int y2 , BUTS *bt , int k ) \
  {
      unsigned int temp , pointer , tempch;
      DIALOG *D , *Parent;
      kgWC *wc;
//   if(bt->sw[k]< 1) return; //not active
      if ( bt->sw [ k ] != 1 ) return; //Need not Draw
      D = ( DIALOG * ) ( bt->D ) ;
      wc = WC ( D ) ;
      Parent = ( D ) ;
      temp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _uidraw_line ( wc , x1+1 , y1-1 , x2-1 , y1-1 ) ;
      _uidraw_line ( wc , x1+1 , y1-1 , x1+1 , y2+1 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _uidraw_line ( wc , x1+1 , y2+1 , x2-1 , y2+1 ) ;
      _uidraw_line ( wc , x2-1 , y1-1 , x2-1 , y2+1 ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _dv_draw_newbutton ( void *tmp , int k , int state ) {
      DIB *B;
      DIN *N;
      BUT_STR *butn;
      unsigned int temp , pointer , tempch;
      int bkgr , lnwidth = 1 , highli = 0 , n , ix , iy , ln , wd , xgap , ygap;
      int offset = 4;
      float transparency;
      int x0 , y0 , l , w , x1 , y1 , x2 , y2 , nb , df;
      int EVGAY;
      DIALOG *D;
      kgWC *wc;
      void **xpm = NULL , *xpmp = NULL , *xpmpr = NULL , *xpmhi = NULL , *img;
      N = ( DIN * ) tmp;
      B = ( DIB * ) tmp;
      if ( N->code != 'n' ) {
          D = ( DIALOG * ) ( B->D ) ;
          df = * ( B->df ) ;
      }
      else {
          D = N->D;
          df = * ( N->df ) ;
      }
      transparency = D->transparency;
      wc = WC ( D ) ;
      highli = 0;
      if ( k+1 == df ) highli = 1;
      if ( D->butattn == 0 ) highli = 0;
      switch ( B->code ) {
          case 'n':
          butn = ( BUT_STR * ) ( N->buts ) ;
          if ( butn [ k ] .sw < 1 ) return;
          x1 = butn [ k ] .x1; y1 = butn [ k ] .y1;
          x2 = butn [ k ] .x2; y2 = butn [ k ] .y2;
          if ( D->DrawBkgr != 0 ) _dvrect_fill_transparent ( wc , x1 , \
               y1 , x2 , y2 , D->gc.fill_clr , transparency ) ;
//       switch(butn[k].state) {
          switch ( state ) {
              case 0:
              img = butn [ k ] .imgn;
              if ( highli ) img = butn [ k ] .imgh;
              kgImage ( D , img , x1 , y1 , x2-x1 , y2-y1 , 0.0 , 1.0 ) ;
              break;
              case 1:
              case -1:
              img = butn [ k ] .imgp;
              kgImage ( D , img , x1 , y1 , x2-x1 , y2-y1 , 0.0 , 1.0 ) ;
              break;
          }
          break;
          default:
          if ( B->sw [ k ] < 1 ) return; //not active
          EVGAY = D->evgay;
          temp = WC ( D )->c_color;
          xpm = B->xpm;
          nb = B->nx*B->ny;
          ix = k%B->nx;
          iy = k/B->nx;
          xgap = B->xgap;
          ygap = B->ygap;
          ln = B->lngth;
          wd = B->width;
          df = * ( B->df ) ;
          x1 = D->xo+B->x1+offset+ix* ( xgap+ln ) ;
          y1 = D->yo+B->y1+offset+iy* ( ygap+wd ) ;
          if ( xpm != NULL ) {
              xpmp = xpm [ k ] ;
              xpmpr = xpm [ nb+k ] ;
              xpmhi = xpm [ nb+nb+k ] ;
              if ( xpmpr == NULL ) xpmpr = xpmp;
              if ( xpmhi == NULL ) xpmhi = xpmp;
          }
          img = xpmp;
          if ( highli ) img = xpmhi;
          if ( state ) img = xpmpr;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          if ( D->DrawBkgr != 0 ) _dvrect_fill_transparent ( wc , x1 , y1 , \
               x1+ln+xgap , y1+wd+ygap , D->gc.fill_clr , transparency ) ;
          if ( B->bkgr == NULL ) bkgr = D->gc.fill_clr;
          else bkgr = B->bkgr [ k ] ;
          if ( bkgr < 0 ) {
              if ( bkgr == -1 ) bkgr = D->gc.fill_clr;
          }
          uiShadedStringImage ( D , B->titles [ k ] , img , x1+xgap/2 , y1+ygap/2 , \
               ln , wd , D->gc.ButtonFont , bkgr , highli , D->gc.but_char , \
               D->gc.FontSize , 0 , B->fac , state , B->type ) ;
          uiset_clr ( D , temp ) ;
          uiRest_clip_limits ( wc ) ;
          break;
      }
  }
  void _dv_draw_button ( void *tmp , int k , int state ) {
      DIB *B;
      DIN *N;
      BUT_STR *butn;
      unsigned int temp , pointer , tempch;
      int bkgr , lnwidth = 1 , highli = 0 , n , ix , iy , ln , wd , xgap , ygap;
      int offset = 4;
      float transparency;
      int x0 , y0 , l , w , x1 , y1 , x2 , y2 , nb , df;
      int bkgrdraw = 0;
      int EVGAY;
      DIALOG *D;
      kgWC *wc;
      void **xpm = NULL , *xpmp = NULL , *xpmpr = NULL , *xpmhi = NULL , *img;
      N = ( DIN * ) tmp;
      D = N->D;
      df = * ( N->df ) ;
      if ( N->type == 10 ) return;
      xgap = N->xgap;
      ygap = N->ygap;
      transparency = D->transparency;
      wc = WC ( D ) ;
      highli = 0;
      if ( k+1 == df ) highli = 1;
      if ( D->butattn == 0 ) highli = 0;
      butn = ( BUT_STR * ) ( N->buts ) ;
      if ( butn [ k ] .sw < 1 ) return;
      x1 = butn [ k ] .x1; y1 = butn [ k ] .y1;
      x2 = butn [ k ] .x2; y2 = butn [ k ] .y2;
      if ( D->DrawBkgr != 0 ) {
          if ( N->nodrawbkgr != 1 ) {
              if ( transparency < 0.0001 ) bkgrdraw = 1;
//     _dvrect_fill_transparent(wc,x1,y1,x2,y2,D->gc.fill_clr,transparency);
          }
      }
      bkgrdraw = 0; //forcing ; no more required
      if ( bkgrdraw ) {
          _dvrect_fill_transparent ( wc , x1 , y1 , x2 , \
               y2 , D->gc.fill_clr , transparency ) ;
      }
      else {
          if ( butn [ k ] .Bimg == NULL ) butn [ k ] .Bimg = kgGetBackground \
           ( D , x1 , y1 , x2 , y2 ) ;
//     else kgImage(D,butn[k].Bimg,x1,y1,(x2-x1+1),(y2-y1+1),0.0,1.0);
          else kgRestoreImage ( D , butn [ k ] .Bimg , x1 , y1 , \
           ( x2-x1+1 ) , ( y2-y1+1 ) ) ;
      }
//       switch(butn[k].state) {
      switch ( state ) {
          case 0:
          img = butn [ k ] .imgn;
          if ( highli ) img = butn [ k ] .imgh;
          kgImage ( D , img , x1 , y1 , x2-x1 , y2-y1 , 0.0 , 1.0 ) ;
          break;
          case 1:
          case -1:
          img = butn [ k ] .imgp;
          kgImage ( D , img , x1 , y1 , x2-x1 , y2-y1 , 0.0 , 1.0 ) ;
          break;
      }
  }
  void _ui_draw_newbutton ( int x1 , int y1 , int x2 , int y2 , \
       BUTS *bt , int k , float transparency ) \
  {
      unsigned char temp , pointer , tempch;
      int bkgr , lnwidth = 1 , highli = 0 , xgap , ygap , n;
      int x0 , y0 , l , w;
      int EVGAY;
      DIALOG *D;
      kgWC *wc;
      void *xpm = NULL;
      if ( bt->sw [ k ] < 1 ) return; //not active
      D = ( DIALOG * ) ( bt->D ) ;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      temp = WC ( D )->c_color;
      x0 = x1; y0 = EVGAY-y1;
      l = abs ( x2-x1 ) ; w = abs ( y2 -y1 ) ;
      n = bt->nxb*bt->nyb;
      if ( ( bt->xpm != NULL ) && ( bt->xpm [ k ] != NULL ) ) xpm = bt->xpm [ k ] ;
      highli = 0;
      if ( k+1 == bt->df ) highli = 1;
      if ( ( xpm != NULL ) && ( highli ) ) {
          if ( bt->xpm [ k+2*n ] != NULL ) xpm = bt->xpm [ k+2*n ] ;
      }
      if ( D->butattn == 0 ) highli = 0;
      xgap = bt->xgap/2;
      ygap = bt->ygap/2;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _uirect_fill_transparent ( wc , x1-xgap , y1+ygap , x2+xgap , \
           y2-ygap , D->gc.fill_clr , transparency ) ;
      if ( bt->bkgr == NULL ) bkgr = D->gc.fill_clr;
      else bkgr = bt->bkgr [ k ] ;
      if ( bkgr < 0 ) {
          if ( bkgr == -1 ) bkgr = D->gc.fill_clr;
      }
      uiShadedStringImage ( D , bt->title [ k ] , xpm , x0 , y0 , l , w , D->gc.ButtonFont , \
           bkgr , highli , D->gc.but_char , D->gc.FontSize , \
           0 , bt->fac , 0 , bt->type ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _ui_rmvattn_newbutton ( int x1 , int y1 , int x2 , \
       int y2 , BUTS *bt , int k , float transparency ) \
  {
      unsigned char temp , pointer , tempch;
      int bkgr , lnwidth = 1 , highli = 0 , xgap , ygap; ;
      int EVGAY;
      DIALOG *D;
      kgWC *wc;
      if ( bt->sw [ k ] < 1 ) return; //not active
      xgap = bt->xgap/2;
      ygap = bt->ygap/2;
      D = ( DIALOG * ) ( bt->D ) ;
      if ( D->kbattn != 1 ) return;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      temp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
//   _uirect_fill_transparent(wc,x1+1,y1-1,x2-1,y2+1,D->gc.fill_clr,transparency);
      _uirect_fill_transparent ( wc , x1-xgap , y1+ygap , x2+xgap , \
           y2-ygap , D->gc.fill_clr , transparency ) ;
      if ( bt->bkgr == NULL ) bkgr = D->gc.fill_clr;
      else bkgr = bt->bkgr [ k ] ;
      if ( bkgr < 0 ) bkgr = D->gc.fill_clr;
      highli = 0;
      uiShadedString ( D , bt->title [ k ] , x1 , D->evgay- ( y1 ) , ( x2-x1 ) , \
       ( y1-y2 ) , D->gc.ButtonFont , bkgr , highli , D->gc.but_char , \
            D->gc.FontSize , 0 , bt->fac , 0 , bt->type ) ;
      if ( ( bt->xpm != NULL ) && ( bt->xpm [ k ] != NULL ) ) {
          _ui_jpg_newdraw ( bt , k , x1 , EVGAY-y2 , \
               x2 , EVGAY-y1 , bkgr , 0.0 , 1.0 ) ;
      }
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _ui_redraw_newbutton ( int x1 , int y1 , \
       int x2 , int y2 , BUTS *bt , int k ) \
  {
      unsigned char temp , pointer , tempch;
      DIALOG *D;
      kgWC *wc;
//   if(bt->sw[k]< 1) return; //not active
      if ( bt->sw [ k ] != 1 ) return; //Need not Draw
      D = ( DIALOG * ) ( bt->D ) ;
      wc = WC ( D ) ;
      temp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _uidraw_line ( wc , x1+1 , y1-1 , x2-1 , y1-1 ) ;
      _uidraw_line ( wc , x1+1 , y1-1 , x1+1 , y2+1 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _uidraw_line ( wc , x1+1 , y2+1 , x2-1 , y2+1 ) ;
      _uidraw_line ( wc , x2-1 , y1-1 , x2-1 , y2+1 ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _uiDrawButns ( BUTS *bt ) {
      int xgap = 0 , ygap = 0 , i , j , k , xx , yy , width , height;
      int pointer;
      width = bt->b_w; height = bt->b_h;
      if ( ( bt->nyb ) > 1 ) ygap = ( ( float )  \
          ( ( bt->y2-bt->y1 ) -height*bt->nyb ) / \
       ( bt->nyb-1.0 ) +0.5 ) ;
      if ( ( bt->nxb ) > 1 ) xgap = ( float )  \
          ( ( bt->x2-bt->x1 ) -width*bt->nxb ) / \
       ( bt->nxb-1.0 ) +0.5;
      k = 0;
      for ( j = 0; j < ( bt->nyb ) ; j++ ) {
          yy = bt->y2- j* ( bt->b_h+ygap ) -bt->b_h;
          for ( i = 0; i < ( bt->nxb ) ; i++ ) {
              xx = bt->x1+ i* ( bt->b_w+xgap ) ;
              _ui_draw_button ( xx , yy+height , xx+width , yy , bt , k ) ;
              if ( ( bt->sw [ k ] == 1 ) || ( bt->sw [ k ] == 3 ) ) {
                  if ( bt->D->butattn ) {
                      if ( k+1 == bt->df ) {
                          uidraw_depre ( ( bt->D ) , xx-bt->Hsize , yy+height+bt->Hsize , \
                               xx+width+bt->Hsize , yy-bt->Hsize ) ;
                      }
                      else {
                          _ui_rmv_inset_protru ( ( bt->D ) , xx-bt->Hsize , yy+height+bt->Hsize , \
                               xx+width+bt->Hsize , yy-bt->Hsize ) ;
                      }
                  }
              }
              k++;
          }
      }
  }
  void _uiDrawPressedNewButn ( BUTS *bt , int k ) {
      int xgap = 0 , ygap = 0 , i , j , xx , yy , width , height;
      int pointer , kk;
      width = bt->b_w; height = bt->b_h;
      if ( ( bt->nyb ) > 1 ) ygap = ( ( float )  \
          ( ( bt->y2-bt->y1 ) -height*bt->nyb ) / \
       ( bt->nyb-1.0 ) +0.5 ) ;
      if ( ( bt->nxb ) > 1 ) xgap = ( float )  \
          ( ( bt->x2-bt->x1 ) -width*bt->nxb ) / \
       ( bt->nxb-1.0 ) +0.5;
      kk = 0;
      for ( j = 0; j < ( bt->nyb ) ; j++ ) {
          for ( i = 0; i < ( bt->nxb ) ; i++ ) {
              if ( k == kk ) {
                  yy = bt->y2- j* ( bt->b_h+ygap ) -bt->b_h;
                  xx = bt->x1+ i* ( bt->b_w+xgap ) ;
                  _ui_draw_pressed_newbutton ( xx , yy+height , xx+width , \
                       yy , bt , k , bt->D->transparency ) ;
              }
              kk++;
          }
      }
  }
  void _uiRedrawButns ( BUTS *bt ) {
      int xgap = 0 , ygap = 0 , i , j , k , xx , yy , width , height;
      int pointer;
      uiUpdateOff ( bt->D ) ;
      width = bt->b_w; height = bt->b_h;
      if ( ( bt->nyb ) > 1 ) ygap = ( ( float )  \
          ( ( bt->y2-bt->y1 ) -height*bt->nyb ) / \
       ( bt->nyb-1.0 ) +0.5 ) ;
      if ( ( bt->nxb ) > 1 ) xgap = ( float )  \
          ( ( bt->x2-bt->x1 ) -width*bt->nxb ) / \
       ( bt->nxb-1.0 ) +0.5;
      k = 0;
      for ( j = 0; j < ( bt->nyb ) ; j++ ) {
          yy = bt->y2- j* ( bt->b_h+ygap ) -bt->b_h;
          for ( i = 0; i < ( bt->nxb ) ; i++ ) {
              xx = bt->x1+ i* ( bt->b_w+xgap ) ;
              _ui_redraw_button ( xx , yy+height , xx+width , yy , bt , k ) ;
              if ( ( bt->sw [ k ] == 1 ) || ( bt->sw [ k ] == 3 ) ) {
                  if ( bt->D->butattn ) {
                      if ( k+1 == bt->df ) {
                          uidraw_depre ( bt->D , xx-bt->Hsize , yy+height+bt->Hsize , \
                               xx+width+bt->Hsize , yy-bt->Hsize ) ;
                      }
                      else {
                          _ui_rmv_inset_protru ( bt->D , xx-bt->Hsize , yy+height+bt->Hsize , \
                               xx+width+bt->Hsize , yy-bt->Hsize ) ;
                      }
                  }
              }
              k++;
          }
      }
      uiUpdateOn ( bt->D ) ;
  }
  void _uiDrawNewButns ( BUTS *bt ) {
      int xgap = 0 , ygap = 0 , i , j , k , xx , yy , width , height;
      int pointer;
      DIALOG *D;
      int EVGAY;
      D = ( DIALOG * ) ( bt->D ) ;
      EVGAY = D->evgay;
      width = bt->b_w; height = bt->b_h;
      ygap = bt->ygap;
      xgap = bt->xgap;
      k = 0;
      for ( j = 0; j < ( bt->nyb ) ; j++ ) {
          yy = bt->y2- j* ( bt->b_h+ygap ) -bt->b_h;
          for ( i = 0; i < ( bt->nxb ) ; i++ ) {
              xx = bt->x1+ i* ( bt->b_w+xgap ) ;
              _ui_draw_newbutton ( xx , yy+height , xx+width , \
                   yy , bt , k , bt->D->transparency ) ;
#if 0
#endif
              k++;
          }
      }
  }
  void _uiDrawButtons ( void *tmp ) {
      DIB *B;
      DIN *N;
      int i , j , k;
      B = ( DIB * ) tmp;
      if ( B->code == 'n' ) {
          N = ( DIN * ) B;
          if ( N->hide == 1 ) return;
      }
      else if ( B->hide == 1 ) return;
      k = 0;
      for ( j = 0; j < ( B->ny ) ; j++ ) {
          for ( i = 0; i < ( B->nx ) ; i++ ) {
              _dv_draw_newbutton ( B , k , 0 ) ;
              k++;
          }
      }
  }
  void _uiDrawButtons_n ( void *tmp ) {
/* used for DIL also */
      DIN *N;
      int i , j , k;
      N = ( DIN * ) tmp;
      if ( N->hide == 1 ) return;
      k = 0;
      for ( j = 0; j < ( N->ny ) ; j++ ) {
          for ( i = 0; i < ( N->nx ) ; i++ ) {
              _dv_draw_button ( N , k , 0 ) ;
              k++;
          }
      }
  }
  void _uiRedrawNewButns ( BUTS *bt ) {
      int xgap = 1 , ygap = 1 , i , j , k , xx , yy , width , height;
      int pointer;
      DIALOG *D;
      int EVGAY;
      D = ( DIALOG * ) ( bt->D ) ;
      EVGAY = D->evgay;
      uiUpdateOff ( D ) ;
      width = bt->b_w; height = bt->b_h;
      if ( ( bt->nyb ) > 1 ) ygap = ( ( float )  \
          ( ( bt->y2-bt->y1 ) -height*bt->nyb ) / \
       ( bt->nyb-1.0 ) +0.5 ) ;
      if ( ( bt->nxb ) > 1 ) xgap = ( float )  \
          ( ( bt->x2-bt->x1 ) -width*bt->nxb ) / \
       ( bt->nxb-1.0 ) +0.5;
      k = 0;
      for ( j = 0; j < ( bt->nyb ) ; j++ ) {
          yy = bt->y2- j* ( bt->b_h+ygap ) -bt->b_h;
          for ( i = 0; i < ( bt->nxb ) ; i++ ) {
              xx = bt->x1+ i* ( bt->b_w+xgap ) ;
              _ui_draw_newbutton ( xx , yy+height , xx+width , \
                   yy , bt , k , D->transparency ) ;
              k++;
          }
      }
      uiUpdateOn ( D ) ;
  }
  int Make_but_box_new ( DIBN *b , DIALOG *D ) {
      int x1; int y1;
      int Hsize;
      int ret , n = 0 , lngth , i = 0;
      BUTS *buts;
      Hsize = D->Hsize;
      x1 = D->xo;
      y1 = D->yo;
      while ( ( b->prompt [ i ] ) == ' ' ) i++;
      while ( ( b->prompt [ i+n ] ) != '\0' ) n++;
      if ( n != 0 ) n++;
      lngth = n*9+Hsize+4;
//   uiDefaultGuiFontSize((D));
      if ( b->nx <= 1 ) b->xgap = 0;
      if ( b->ny <= 1 ) b->ygap = 0;
//   uidia_message(D,b->x1+x1,b->y1+12+y1+b->width/2,b->prompt+i);
      uiString ( D , b->prompt+i , b->x1+x1 , b->y1+y1+b->width/2-10 , n*9-5 , \
           20 , D->gc.PromptFont , D->gc.txt_pchar , \
           D->gc.FontSize , 1 , D->gc.fill_clr ) ;
      b->buts = make_but_str ( D , b->x1+x1+lngth , b->y1+y1+Hsize+2 , \
           b->xgap , b->ygap , b->nx , b->ny , b->lngth , b->width , * \
       ( b->df ) , b->titles , b->xpm , b->bkgr , NULL ) ;
      buts = ( BUTS * ) ( b->buts ) ;
      buts->D = D;
      buts->Hsize = D->Hsize;
//   b->x2 = b->x1+lngth+b->nx*(b->lngth+b->xgap)-b->xgap+(Hsize+2);
//   b->y2 = b->y1+2*Hsize+2+b->ny*(b->width+b->ygap)-b->ygap+(Hsize+2);
      if ( b->code == 'N' ) {
          n = b->nx*b->ny;
          for ( i = 0; i < n; i++ ) buts->sw [ i ] = 2;
      }
//   printf("Calling _uiDrawButns\n");
      _uiDrawButns ( ( BUTS * ) ( b->buts ) ) ;
//   DR_BUTNS((BUTS *)(b->buts),0);
      return ( ret ) ;
  }
  int Make_new_but_box ( DIBN *b , DIALOG *D ) {
      int x1; int y1;
      int Hsize;
      int ret , n = 0 , lngth , i = 0;
      BUTS *buts;
      Hsize = D->Hsize;
      x1 = D->xo;
      y1 = D->yo;
      while ( ( b->prompt [ i ] ) == ' ' ) i++;
      while ( ( b->prompt [ i+n ] ) != '\0' ) n++;
      if ( n != 0 ) n++;
      lngth = n*9+Hsize+2;
      if ( b->nx <= 1 ) b->xgap = 0;
      if ( b->ny <= 1 ) b->ygap = 0;
//   uiString(D,b->prompt+i,b->x1+x1,b->y1+y1+b->width/2-10,n*9-5,20,D->gc.PromptFont,D->gc.txt_pchar,D->gc.FontSize,1,D->gc.fill_clr);
      uiString ( D , b->prompt+i , b->x1+x1 , b->y1+y1 , n*9-5 , b->width , D->gc.PromptFont , \
           D->gc.txt_pchar , D->gc.FontSize , 1 , D->gc.fill_clr ) ;
      b->buts = make_but_str ( D , b->x1+x1+lngth , b->y1+y1+Hsize+2 , \
           b->xgap , b->ygap , b->nx , b->ny , b->lngth , b->width , * \
       ( b->df ) , b->titles , b->xpm , b->bkgr , NULL ) ;
      buts = ( BUTS * ) ( b->buts ) ;
      buts->D = D;
      buts->Hsize = D->Hsize;
      buts->type = b->type;
      buts->fac = b->fac;
      if ( b->code == 'N' ) {
          n = b->nx*b->ny;
          for ( i = 0; i < n; i++ ) buts->sw [ i ] = 2;
      }
      _uiDrawNewButns ( ( BUTS * ) ( b->buts ) ) ;
      return ( ret ) ;
  }
  int _uiMake_B ( DIBN *b ) {
      BUTS *buts;
      int ret = 1 , k;
      int x1 , y1 , x2 , y2;
      DIALOG *D;
      kgWC *wc;
      D = b->D;
      x1 = b->x1+D->xo;
      x2 = b->x2+D->xo;
      y1 = b->y1+D->yo;
      y2 = b->y2+D->yo;
      wc = WC ( D ) ;
      if ( b->code != 'n' ) {
          buts = ( BUTS * ) ( b->buts ) ;
          if ( buts != NULL ) {
              buts->xpm = b->xpm;
              buts->bkgr = b->bkgr;
          }
      }
      if ( b->hide != 1 ) {
          CHECKLIMITS ( b ) ;
          BACKUPWIDGETAREA ( b ) ;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          if ( b->bordr == 1 ) {
              _dv_draw_bound ( D , ( x1+2 ) , ( y1+2 ) , \
               ( x2-2 ) , ( y2-2 ) , D->gc.high_clr ) ;
          }
          _uiDrawButtons ( b ) ;
          uiRest_clip_limits ( wc ) ;
      }
      else {
#if 0
          ret = 0;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( x1 ) , ( y1 ) , ( x2 ) , \
           ( y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          RESTOREWIDGETAREA ( b ) ;
      }
      return ( ret ) ;
  }
  int _uiMake_N ( void *tmp ) {
      DIN *b;
      BUT_STR *buts;
      int ret = 1 , k , n , backgr;
      int x1 , y1 , x2 , y2;
      int xgap , ygap;
      int xoff , yoff;
      DIALOG *D;
      kgWC *wc;
      void *cimgn = NULL , *cimgp = NULL , *cimgh = NULL , \
           *simg = NULL , *xpm , *cimg;
      void *xpmp , *timg , *xpmh;
      GMIMG *gimg;
      IMG_STR *IMG = NULL;
      char *str;
      int type , mf , mfp , bkgr , i , ln , wd , length , height;
      int font , color , FontSize;
      b = ( DIN * ) tmp;
      D = b->D;
      x1 = b->x1+D->xo;
      x2 = b->x2+D->xo;
      y1 = b->y1+D->yo;
      y2 = b->y2+D->yo;
      wc = WC ( D ) ;
      buts = ( BUT_STR * ) b->buts;
      backgr = 1;
      _uiInitButs ( b ) ;
      i = 0;
      type = b->type;
      if ( b->type == 10 ) return ret;
      if ( ( type == 0 ) || ( type == 9 ) ) {
          mf = 0;
          mfp = 2;
      }
      else {
          mf = 1;
          mfp = 1;
      }
      if ( type == 1 ) mfp = 0;
      i = 0;
      x1 = buts [ i ] .x1;
      y1 = buts [ i ] .y1;
      x2 = buts [ i ] .x2;
      y2 = buts [ i ] .y2;
      wd = y2 -y1;
      ln = x2 - x1;
      length = ln+mfp;
      height = wd+mfp;
      if ( ( type > 0 ) && ( type < 9 ) ) {
          length = ln+mfp-10;
          height = wd+mfp-10;
      }
      bkgr = buts [ i ] .bkgr;
      if ( bkgr < 0 ) {
          if ( bkgr == -1 ) bkgr = D->gc.fill_clr;
      }
      font = D->gc.ButtonFont;
      FontSize = D->gc.GuiFontSize;
      color = D->gc.but_char;
      cimgn = NULL;
      cimgp = NULL;
      if ( buts [ 0 ] .imgn == NULL ) {
          cimgn = uiMakeButtonImage ( D , ln+mfp , wd+mfp , \
               bkgr , 0 , 0 , b->fac , type ) ;
          cimgp = uiMakeButtonImage ( D , ln+mfp , wd+mfp , \
               bkgr , -1 , 0 , b->fac , type ) ;
      }
//      printf("TCB: ln: %d %d %f\n",ln,height,b->fac);
#if 1
      n = b->nx*b->ny;
      if ( Bimgs == NULL ) {printf ( "Bimgs==NULL\n" ) ;fflush ( stdout ) ;}
      for ( k = 0; k < n; k++ ) {
          if ( buts [ k ] .imgn == NULL ) {
              str = buts [ k ] .title;
              simg = NULL;
              if ( ( type > 0 ) && ( str != NULL ) && ( str [ 0 ] >= ' ' ) ) {
                  IMG = ( IMG_STR * ) uiComplexString ( str , \
                       Bimgs , font , color , FontSize , wd ) ;
                  simg = IMG->img;
                  free ( IMG ) ;
              }
              backgr = buts [ k ] .bkgr;
              if ( backgr < 0 ) {
                  if ( backgr == -1 ) backgr = D->gc.fill_clr;
              }
              cimg = NULL;
              if ( bkgr != backgr ) {
                  cimg = uiMakeButtonImage ( D , ln+mfp , wd+mfp , \
                       backgr , 0 , 0 , b->fac , b->type ) ;
              }
              else if ( cimgn != NULL ) cimg = kgGetImageCopy ( NULL , cimgn ) ;
              xpm = NULL;
              if ( buts [ k ] .xpmn != NULL ) xpm = kgGetImageCopy  \
                  ( NULL , buts [ k ] .xpmn ) ;
              if ( xpm != NULL ) {
                  gimg = ( GMIMG * ) xpm;
                  if ( ( gimg->image_width > length ) || ( gimg->image_height > height ) ) {
                      xpm = kgChangeSizeImage ( xpm , length , height ) ;
                  }
                  if ( cimg != NULL ) cimg = kgMergeImages ( cimg , xpm , 0 , 0 ) ;
                  else cimg = kgGetImageCopy ( NULL , xpm ) ;
              }
              if ( simg != NULL ) {
                  gimg = ( GMIMG * ) simg;
                  if ( ( gimg->image_width > ln-10 ) || ( gimg->image_height > wd-4 ) ) {
                      int xl , yl;
                      xl = gimg->image_width;
                      if ( xl > ln-10 ) xl = ln -10;
                      yl = gimg->image_height ;
                      if ( yl > wd-4 ) yl = wd-4;
                      simg = kgChangeSizeImage ( simg , xl , yl ) ;
                  }
                  if ( cimg != NULL ) cimg = kgMergeImages ( cimg , simg , 0 , 0 ) ;
                  else cimg = kgGetImageCopy ( NULL , simg ) ;
              }
              buts [ k ] .imgn = cimg;
              cimg = NULL;
              if ( bkgr != backgr ) {
                  cimg = uiMakeButtonImage ( D , ln+mfp , wd+mfp , \
                       backgr , -1 , 0 , b->fac , b->type ) ;
              }
              else if ( cimgp != NULL ) cimg = kgGetImageCopy ( NULL , cimgp ) ;
              xpmp = NULL;
              xpmh = NULL;
#if 1
              if ( buts [ k ] .xpmp != NULL ) {
                  xpmp = kgGetImageCopy ( NULL , buts [ k ] .xpmp ) ;
                  gimg = ( GMIMG * ) xpmp;
                  if ( ( gimg->image_width > length ) || ( gimg->image_height > height ) ) {
                      xpmp = kgChangeSizeImage ( xpmp , length , height ) ;
                  }
                  if ( cimg != NULL ) cimg = kgMergeImages ( cimg , xpmp , 0 , 0 ) ;
                  else cimg = kgGetImageCopy ( NULL , xpmp ) ;
                  kgFreeImage ( xpmp ) ;
                  xpmp = NULL;
              }
              else {
                  timg = NULL;
                  if ( xpm != NULL ) {
                      timg = kgGetImageCopy ( NULL , xpm ) ;
                      xpmp = kgChangeBrightness ( timg , 0.5 ) ;
                      if ( cimg != NULL ) cimg = kgMergeImages ( cimg , xpmp , 0 , 0 ) ;
                      else cimg = kgGetImageCopy ( NULL , xpmp ) ;
                      kgFreeImage ( xpmp ) ;
                      xpmp = NULL;
                  }
              }
              if ( simg != NULL ) {
                  if ( cimg != NULL ) cimg = kgMergeImages ( cimg , simg , 0 , 0 ) ;
                  else cimg = kgGetImageCopy ( NULL , simg ) ;
              }
              buts [ k ] .imgp = cimg;
              xpmh = NULL;
              cimg = NULL;
              if ( cimgn != NULL ) cimg = kgGetImageCopy ( NULL , cimgn ) ;
              if ( buts [ k ] .xpmh != NULL ) {
                  xpmh = kgGetImageCopy ( NULL , buts [ k ] .xpmh ) ;
                  gimg = ( GMIMG * ) xpmh;
                  if ( ( gimg->image_width > length ) || ( gimg->image_height > height ) ) {
                      xpmh = kgChangeSizeImage ( xpmh , length , height ) ;
                  }
                  if ( cimg != NULL ) cimg = kgMergeImages ( cimg , xpmh , 0 , 0 ) ;
                  else cimg = kgGetImageCopy ( NULL , xpmh ) ;
                  kgFreeImage ( xpmh ) ;
                  xpmh = NULL;
              }
              else {
                  timg = NULL;
                  if ( xpm != NULL ) {
                      if ( cimg != NULL ) cimg = kgMergeImages ( cimg , xpm , 0 , 0 ) ;
                      else cimg = kgGetImageCopy ( NULL , xpm ) ;
                      timg = cimg ;
                      cimg = kgChangeBrightness ( timg , 1.2 ) ;
                  }
              }
              if ( simg != NULL ) {
                  if ( cimg != NULL ) cimg = kgMergeImages ( cimg , simg , 0 , 0 ) ;
                  else cimg = kgGetImageCopy ( NULL , simg ) ;
              }
              buts [ k ] .imgh = cimg;
#else
              buts [ k ] .imgp = kgGetImageCopy ( NULL , buts [ k ] .imgn ) ;
              buts [ k ] .imgh = kgGetImageCopy ( NULL , buts [ k ] .imgn ) ;
#endif
              if ( xpm != NULL ) kgFreeImage ( xpm ) ;
              if ( simg != NULL ) kgFreeImage ( simg ) ;
              xpm = NULL;xpmp = NULL;xpmh = NULL;simg = NULL;
          }
//             printf("TCB:k=%d\n",k);
//             fflush(stdout);
      }
#else
      n = b->nx*b->ny;
      for ( k = 0; k < n; k++ ) {
          if ( buts [ k ] .imgn == NULL ) {
//         _uiMakeButnImages(b,k);
//         _uiMakeButnImages(buts+k);
#ifdef D_TH
              DoInAnyThread ( D->ThInfo , _uiMakeButnImages , buts+k ) ;
#else
              _uiMakeButnImages ( buts+k ) ;
#endif
              backgr = 0;
          }
      }
      WaitThreads ( D->ThInfo ) ;
#endif
#if 0
      else {
          n = b->nx*b->ny;
          for ( k = 0; k < n; k++ ) {
              kgFreeImage ( buts [ k ] .imgn ) ;
              kgFreeImage ( buts [ k ] .imgp ) ;
              kgFreeImage ( buts [ k ] .imgh ) ;
          }
          _uiMakeButs ( b ) ;
      }
#endif
      if ( b->type == 10 ) return ret;
      if ( b->hide != 1 ) {
          CHECKLIMITS ( b ) ;
          BACKUPWIDGETAREA ( b ) ;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _uiUpdateType0Buts ( b ) ;
          if ( b->bordr == 1 ) {
              _dv_draw_bound ( D , ( x1+2 ) , ( y1+2 ) , \
               ( x2-2 ) , ( y2-2 ) , D->gc.high_clr ) ;
          }
          _uiDrawButtons_n ( b ) ;
          uiRest_clip_limits ( wc ) ;
      }
      else {
#if 0
          ret = 0;
          if ( backgr ) {
              uiBkup_clip_limits ( wc ) ;
              uiSet_full_scrn ( wc ) ;
              _dvrect_fill_transparent ( wc , ( x1 ) , ( y1 ) , ( x2 ) , \
               ( y2 ) , D->gc.fill_clr , D->transparency ) ;
              uiRest_clip_limits ( wc ) ;
          }
#endif
          RESTOREWIDGETAREA ( b ) ;
      }
      return ( ret ) ;
  }
  int Make_hori_box_new ( DILN *b , DIALOG *D ) {
      int ret , lngth;
      int Hsize;
      int x1; int y1;
      BUTS *buts;
      x1 = D->xo;
      y1 = D->yo;
      lngth = D->Hsize+2;
//   uiDefaultGuiFontSize(D);
      b->buts = make_but_str ( D , b->x1+x1+lngth , b->y1+y1+lngth , b->xgap , \
           b->ygap , b->nx , b->ny , b->lngth , b->width , * \
       ( b->df ) , b->titles , b->xpm , b->bkgr , b->sw ) ;
      buts = ( BUTS * ) ( b->buts ) ;
      buts->D = D;
      buts->Hsize = D->Hsize;
      buts->type = b->type;
      buts->fac = b->fac;
//   _uiDrawNewButns((BUTS *)(b->buts));
      _uiDrawButtons ( b ) ;
      return ( ret ) ;
  }
  int _uiMake_H ( DILN *b ) {
      int ret;
      if ( b->hide != 1 ) {
          CHECKLIMITS ( b ) ;
          BACKUPWIDGETAREA ( b ) ;
          _uiDrawButtons ( b ) ;
      }
      else {
#if 0
          DIALOG *D;
          kgWC *wc;
          int x1 , y1 , x2 , y2;
          D = b->D;
          wc = WC ( D ) ;
          ret = 0;
          x1 = b->x1+D->xo;
          x2 = b->x2+D->xo;
          y1 = b->y1+D->yo;
          y2 = b->y2+D->yo;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( x1 ) , ( y1 ) , ( x2 ) , \
           ( y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          RESTOREWIDGETAREA ( b ) ;
      }
      return ( ret ) ;
  }
  int uidraw_g_box ( DIALOG *D , int x1 , int y1 , \
       int x2 , int y2 , int clr , void *xpm ) \
  {
      int EVGAY;
//    int offset=4;
      int offset = 0;
      Gclr gc;
      kgWC *wc;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      if ( xpm != NULL ) kgImage ( D , xpm , x1 , y1 , x2 , y2 , 0.0 , 1.0 ) ;
      else if ( clr >= 0 ) _uibox_fill ( wc , x1 , \
           EVGAY-y1 , x2 , EVGAY-y2 , clr ) ;
      if ( D->DrawBkgr != 0 ) {
          _dvbordertype0 ( D , x1- ( offset ) , y1- ( offset ) , x2+ \
           ( offset ) , y2+ ( offset ) , D->gc.vbright ) ;
      }
      uiRest_clip_limits ( wc ) ;
      return 1;
  }
  int Make_viewport ( DIALOG *D , int i ) {
      DIG *g;
      char buf [ 50 ] ;
      int ret = 1;
      int x1 , y1 , x2 , y2;
      int xmin , ymin , xmax , ymax;
      int evgax , evgay;
      g = D->d [ i ] .g;
      kgDC *dc;
      dc = g->dc;
      evgax = D->evgax;
      evgay = D->evgay;
      x1 = g->x1+D->xo;
      x2 = g->x2+D->xo;
      y1 = ( g->y2+D->yo ) ;
      y2 = ( g->y1+D->yo ) ;
//   y1 =  evgay-y1;
//   y2 =  evgay-y2;
      uidraw_g_box ( ( D ) , x1 , y2 , x2 , y1 , g->bkgr_clr , g->xpm ) ;
      return ( ret ) ;
  }
  int _uiMake_G ( DIG *g ) {
      DIALOG *D;
      char buf [ 50 ] ;
      int ret = 1;
      int x1 , y1 , x2 , y2 , offset = 2;
      int xmin , ymin , xmax , ymax;
      int evgax , evgay;
      kgDC *dc;
      D = g->D;
      if ( g->hide != 1 ) {
          CHECKLIMITS ( g ) ;
          BACKUPWIDGETAREA ( g ) ;
          dc = g->dc;
          evgax = D->evgax;
          evgay = D->evgay;
          x1 = g->x1+D->xo;
          x2 = g->x2+D->xo;
          y1 = ( g->y2+D->yo ) ;
          y2 = ( g->y1+D->yo ) ;
//   y1 =  evgay-y1;
//   y2 =  evgay-y2;
          uidraw_g_box ( ( D ) , x1 , y2 , x2 , y1 , g->bkgr_clr , g->xpm ) ;
      }
      else {
#if 0
          kgWC *wc;
          int x1 , y1 , x2 , y2;
          D = g->D;
          wc = WC ( D ) ;
          ret = 0;
          x1 = g->x1+D->xo-offset-1;
          x2 = g->x2+D->xo+offset+1;
          y1 = g->y1+D->yo-offset-1;
          y2 = g->y2+D->yo+offset+1;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( x1 ) , ( y1 ) , ( x2 ) , \
           ( y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          RESTOREWIDGETAREA ( g ) ;
      }
      return ( ret ) ;
  }
  int _ui_insertchar ( char *s , int pos , int maxlen , int ch ) {
      int i , l;
      if ( pos >= maxlen ) {
          printf ( "pos>= maxlen: %d %d\n" , pos , maxlen ) ;
          return 0;
      }
      l = strlen ( s ) ;
      if ( l > maxlen ) {
          printf ( "l > maxlen : %d %d\n" , l , maxlen ) ;
          return 0;
      }
      if ( pos > l ) {
          printf ( "pos >l :%d %d\n" , pos , l ) ;
          return 0;
      }
      i = l;
      while ( i >= pos ) { s [ i+1 ] = s [ i ] ; i--; }
      s [ pos ] = ch;
      return 1;
  }
  int _ui_deletechar ( char *s , int pos ) {
      int i , l;
      l = strlen ( s ) ;
      if ( pos >= l ) return 0;
      i = pos;
      while ( i < l ) { s [ i ] = s [ i+1 ] ; i++; }
      return 1;
  }
  double _ui_getfslidevalue ( S_STR *pt ) {
      return pt->df*pt->cf+pt->sh;
  }
  int _ui_getdslidevalue ( S_STR *pt ) {
      return ( int ) ( pt->df*pt->cf+pt->sh+0.5 ) ;
  }
  void _ui_slidevalue ( DIALOG *D , S_STR *pt ) {
      int x , y , w , l;
      char buf [ 50 ] ;
      double val;
      Gclr *gc;
      int EVGAY;
      kgWC *wc;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      gc = & ( D->gc ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      w = pt->Sldwd;
      l = pt->l+2*w+6;
      x = pt->x;
      y = pt->y;
      _dvrect_fill_transparent ( wc , x , y-w , x+l , \
           y-1 , D->gc.fill_clr , D->transparency ) ;
      y = EVGAY-y;
//   _uirect_fill(wc,x,y+w,x+l,y+1,pt->fill_clr);
      if ( pt->code == 'f' ) {
          val = pt->df*pt->cf+pt->sh;
          sprintf ( buf , "%-lg" , val ) ;
      }
      else {
          val = pt->df*pt->cf+pt->sh;
//TCB
//     sprintf(buf,"%-ld",(int)(val+0.5));
          sprintf ( buf , "%-d" , ( int ) ( val+0.5 ) ) ;
      }
      x = x + ( l ) /2;
      uiwrite_string ( D , buf , x , EVGAY-pt->y+4 , pt->char_clr ) ;
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void _ui_left_dir_o ( DIALOG *D , int x , int y , int w , int bright ) {
      char tmp;
      int xx , yy , ww , i;
      Gclr *gc;
      int EVGAY;
      kgWC *wc;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      gc = & ( D->gc ) ;
      ww = w/2; xx = x+ ( 2*w ) /5; yy = y-w/4;
      tmp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _uivert_line ( wc , y , y-w , x ) ;
      _uihori_line ( wc , x , x+w , y ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _uivert_line ( wc , y , y-w , x+w ) ;
      _uihori_line ( wc , x , x+w , y-w ) ;
      for ( i = -1; i < ww/2; i++ ) {
          _uivert_line ( wc , yy-i , yy-ww+i , xx+i ) ;
      }
      uiset_clr ( D , tmp ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _ui_right_dir_o ( DIALOG *D , int x , int y , int w , int bright ) {
      char tmp;
      int xx , yy , ww , i;
      kgWC *wc;
      wc = WC ( D ) ;
      ww = w/2; xx = x+w- ( 2*w ) /5; yy = y-w/4;
      tmp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.vbright ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _uivert_line ( wc , y , y-w , x ) ;
      _uihori_line ( wc , x , x+w , y ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _uivert_line ( wc , y , y-w , x+w ) ;
      _uihori_line ( wc , x , x+w , y-w ) ;
      for ( i = -1; i < ww/2; i++ ) {
          _uivert_line ( wc , yy-i , yy-ww+i , xx-i ) ;
      }
      uiset_clr ( D , tmp ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _ui_left_dir ( DIALOG *D , int x , int y , int w , int bright ) {
      char tmp;
      int xx , yy , ww , i;
      float rfac = 0.2;
      int type = 4;
      Gclr *gc;
      kgWC *wc;
      int EVGAY;
      EVGAY = D->evgay;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiShadedString ( D , "!f35t" , x-3 , EVGAY-y-1 , w , w+1 , 35 , D->gc.scroll_fill , \
           0 , D->gc.v_dim , D->gc.FontSize-1 , 0 , rfac , 1 , type ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _ui_right_dir ( DIALOG *D , int x , int y , int w , int bright ) {
      char tmp;
      int xx , yy , ww , i;
      float rfac = 0.2;
      int type = 4;
      kgWC *wc;
      int EVGAY;
      EVGAY = D->evgay;
      wc = WC ( D ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiset_clr ( D , tmp ) ;
//   uiShadedString(D,"!f35!w32!xs", x+3, EVGAY-y-1, w,w+1,35,D->gc.fill_clr,0,D->gc.v_dim,D->gc.FontSize-1,0,rfac,1,type);
      uiShadedString ( D , "!f35s" , x+3 , EVGAY-y-1 , w , w+1 , 35 , D->gc.scroll_fill , \
           0 , D->gc.v_dim , D->gc.FontSize-1 , 0 , rfac , 1 , type ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _dvleft_dir ( DIALOG *D , int x , int y , \
       int w , float rfac , int type ) \
  {
      char tmp;
      int xx , yy , ww , i;
      Gclr *gc;
      kgWC *wc;
      int EVGAY;
      wc = WC ( D ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiShadedString ( D , "!f35t" , x , y , w-1 , w , 35 , D->gc.scroll_fill , \
           0 , D->gc.v_dim , D->gc.FontSize-1 , 0 , rfac , 1 , type ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _dvright_dir ( DIALOG *D , int x , int y , \
       int w , float rfac , int type ) \
  {
      char tmp;
      int xx , yy , ww , i;
      kgWC *wc;
      int EVGAY;
      EVGAY = D->evgay;
      wc = WC ( D ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiset_clr ( D , tmp ) ;
//   uiShadedString(D,"!f35!w32!xs", x+3, EVGAY-y-1, w,w+1,35,D->gc.fill_clr,0,D->gc.v_dim,D->gc.FontSize-1,0,rfac,1,type);
      uiShadedString ( D , "!f35s" , x , y , w-1 , w , 35 , D->gc.scroll_fill , \
           0 , D->gc.v_dim , D->gc.FontSize-1 , 0 , rfac , 1 , type ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _ui_slidebar_o ( DIALOG *D , S_STR *pt ) {
      int sw , x , y , l , w ;
      Gclr gc;
      int EVGAY;
      kgWC *wc;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      gc = D->gc;
      x = pt->x;
      y = pt->y;
      w = pt->Sldwd;
      sw = pt->Sw;
      l = pt->l+2*w+6;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      y = EVGAY-y;
      _uirect_fill ( wc , x , y+w , x+l , y-w , gc.fill_clr ) ;
      _uirect_fill ( wc , x+w+1+sw/2 , y-pt->Sdpw , x+l-w-1-sw/2 , \
           y+pt->Sdpw-w , gc.fill_clr ) ;
      uidraw_depre ( D , x+w+1 , y-Sdpw , x+l-w-1 , y+Sdpw-w ) ;
      _ui_right_dir ( D , x+2 , y-2 , w-4 , gc.vbright ) ;
      _ui_left_dir ( D , x+l-w+2 , y-2 , w-4 , gc.vbright ) ;
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void _ui_slidebar ( DIALOG *D , S_STR *pt ) {
      int sw , x , y , l , w ;
      Gclr gc;
      int x1 , y1;
      int EVGAY;
      void *Bimg;
      DID *Widget;
      kgWC *wc;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      gc = D->gc;
      x = pt->x;
      y = pt->y;
      Widget = ( DID * ) pt->Widget;
      Bimg = _uiGetWidgetBimg ( Widget ) ;
      x1 = D->xo+Widget->x1;
      y1 = D->yo+Widget->y1;
      w = pt->Sldwd;
      sw = pt->Sw;
      l = pt->l+2*w+6;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
//TCBTCB
//   _dvrect_fill_transparent(wc,x,y-w-3,x+l,y+w+3,gc.fill_clr,D->transparency);
      kgRestoreImagePart ( D , Bimg , x , y-w-3 , \
       ( x-x1 ) , - ( y1-y+w+3 ) , l+1 , w+w+7 ) ;
      y = EVGAY-y;
//   _uirect_fill(wc,x,y+w,x+l,y-w,gc.fill_clr);
//   _uirect_fill(wc,x+w+1+sw/2,y-pt->Sdpw,x+l-w-1-sw/2,y+pt->Sdpw-w,gc.fill_clr);
//   uidraw_depre(D,x+w+1,y-Sdpw,x+l-w-1,y+Sdpw-w);
      _ui_right_dir ( D , x+2 , y-2 , w-4 , gc.vbright ) ;
      _ui_left_dir ( D , x+l-w+2 , y-2 , w-4 , gc.vbright ) ;
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void _ui_hbar ( DIHB *f ) {
      DIALOG *D; S_STR *pt;
      int sw , x , y , l , w ;
      Gclr *gc;
      int EVGAY;
      kgWC *wc;
      D = f->D;
      pt = f->sptr;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      gc = & ( D->gc ) ;
      x = pt->x;
      y = pt->y;
      w = pt->Sldwd;
      sw = pt->Sw;
      l = pt->l+2*w+6;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
//   _rect_fill(x,y+w,x+l,y-w,pt->fill_clr);
      switch ( f->type ) {
          default:
          y = EVGAY-y;
          _uirect_fill_transparent ( wc , x+w+1+sw/2 , y-pt->Sdpw , x+l-w-1-sw/2 , \
               y+pt->Sdpw-w , pt->fill_clr , D->transparency ) ;
//      uidraw_depre(D,x+w+1,y-pt->Sdpw,x+l-w-1,y+pt->Sdpw-w);
          break;
          case 1:
//     _dvrect_fill(wc,x+w+1+sw/2,y+w/2-1,x+l-w-1-sw/2,y+w/2+1,gc->v_dim);
          break;
      }
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void _ui_movehslidepointer ( DIHB *B ) {
      DIALOG *D; S_STR *pt;
      char tmp;
      int x1 , y1 , x2 , y2;
      int w , l , offset = 2 , Wd = 6 , v , Bw = 5 , Wh = 10 , of = 4;
      int x , y , sw , sx;
      Gclr *gc;
      kgWC *wc;
      int EVGAY;
      void *Bimg;
      D = B->D;
      pt = B->sptr;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      gc = & ( D->gc ) ;
      Bimg = B->Bimg;
      x = pt->x , y = pt->y , l = pt->l;
      sx = pt->df;
      w = pt->Sldwd;
      sw = pt->Sw;
      tmp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      x1 = B->x1+offset+D->xo;
      y1 = B->y1+offset+D->yo;
      x2 = B->x2-offset+D->xo;
      y2 = B->y2-offset+D->yo;
#ifndef D_RESTORE
      _dvrect_fill_transparent ( wc , x1 , y1 , x2 , \
           y2 , gc->fill_clr , D->transparency ) ;
#else
      kgRestoreImage ( D , Bimg , x1 , y1 , ( x2-x1+1 ) , ( y2-y1+1 ) ) ;
#endif
      Wd = pt->Sldwd;
      w = pt->Sw;
      l = pt->l;
      x1 = pt->x;
      y1 = pt->y;
      switch ( B->type ) {
          case 0:
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          case 7:
          default:
          switch ( B->direction ) {
              default:
              case 0:
              x1 = pt->x-Wd/2-of/2;
              y1 = pt->y - w/2-of/2;
              kgImage ( D , pt->bimg , x1 , y1 , l+Wd+of , w+of , 0.0 , 1.0 ) ;
              x1 = pt->x+pt->df-Wd/2-of/2;
              y1 = pt->y -w/2-of/2;
              kgImage ( D , pt->fimg , x1 , y1 , Wd+of , w+of , 0.0 , 1.0 ) ;
              break;
              case 1:
              y1 = pt->y-Wd/2-of/2;
              x1 = pt->x - w/2-of/2;
              kgImage ( D , pt->bimg , x1 , y1 , w+of , l+Wd+of , 0.0 , 1.0 ) ;
              y1 = pt->y+B->lngth-pt->df-Wd/2-of/2;
              x1 = pt->x -w/2-of/2;
              kgImage ( D , pt->fimg , x1 , y1 , w+of , Wd+of , 0.0 , 1.0 ) ;
              break;
          }
          break;
      }
  }
  void _ui_moveslidepointer ( DIALOG *D , S_STR *pt ) {
      char tmp;
      int x1 , y1 , x2 , y2;
      int x , y , l , w , sw , sx , xo , yo;
      DID *Widget;
      void *Bimg;
      Gclr *gc;
      kgWC *wc;
      int EVGAY;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      gc = & ( D->gc ) ;
      Widget = ( DID * ) pt->Widget;
      Bimg = _uiGetWidgetBimg ( Widget ) ;
      xo = D->xo+Widget->x1;
      yo = D->yo+Widget->y1;
      x = pt->x , y = pt->y , l = pt->l;
      sx = pt->df;
      w = pt->Sldwd;
      sw = pt->Sw;
      l = l+2*w+6;
      tmp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      y = EVGAY-y;
      x1 = x+w+2; y1 = y-4; x2 = x+l-w-2; y2 = y-w+4;
//TCBTCB
#ifndef D_RESTORE
      _uirect_fill_transparent ( wc , x1-2 , y1+5 , x2+2 , \
           y2-5 , gc->fill_clr , D->transparency ) ;
      _uirect_fill ( wc , x1+1 , y1-1 , x2-2 , y2+2 , gc->fill_clr ) ;
#else
      kgRestoreImagePart ( D , Bimg , x1-2 , EVGAY-y1-5 , ( x1-2-xo ) , \
       ( EVGAY-y1-4-yo ) , ( x2+5-x1 ) , abs ( y2-9-y1 ) ) ;
      kgRestoreImagePart ( D , Bimg , x1+1 , EVGAY-y1+1 , ( x1+1-xo ) , \
       ( EVGAY-y1+2-yo ) , ( x2-2-x1 ) , abs ( y2+4-y1 ) ) ;
#endif
      _uirect_fill ( wc , x+w+1+sw/2 , y-pt->Sdpw , \
           x+l-w-1-sw/2 , y+pt->Sdpw-w , gc->dim ) ;
      uidraw_depre ( D , x+w+1+sw/2 , y-pt->Sdpw , x+l-w-1-sw/2 , y+pt->Sdpw-w ) ;
      sx = x1+1+sx;
      if ( sx < x1 ) sx = x1+1;
      if ( sx+sw > x2 ) sx = x2-sw-1;
      _uirect_fill ( wc , sx , y1+2 , sx+sw , y2-2 , gc->fill_clr ) ;
      uiset_clr ( D , gc->vbright ) ;
      _uihori_line ( wc , sx , sx+sw , y1+2 ) ;
      _uihori_line ( wc , sx+1 , sx+sw-1 , y1+1 ) ;
      _uivert_line ( wc , y1+2 , y2-2 , sx ) ;
      _uivert_line ( wc , y1+1 , y2-1 , sx+1 ) ;
      uiset_clr ( D , gc->v_dim ) ;
      _uihori_line ( wc , sx , sx+sw , y2-2 ) ;
      _uivert_line ( wc , y1+2 , y2-2 , sx+sw ) ;
      uiset_clr ( D , gc->dim ) ;
      _uihori_line ( wc , sx+1 , sx+sw-1 , y2-1 ) ;
      _uivert_line ( wc , y1+1 , y2-1 , sx+sw-1 ) ;
      uiset_clr ( D , gc->vbright ) ;
      _uivert_line ( wc , y1-1 , y2+1 , ( int ) ( sx+sw/2-0.5 ) ) ;
      uiset_clr ( D , gc->v_dim ) ;
      _uivert_line ( wc , y1-1 , y2+1 , ( int ) ( sx+sw/2+0.5 ) ) ;
      uiset_clr ( D , tmp ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void * _ui_setslide ( void *tmp , int min , int max , int x , int y , \
       int l , int df , char ch , double cf , double sh ) \
  {
      DIALOG *D;
      DIF *f;
      S_STR *ptr;
      double fac , val;
      int scr_ln , wd = Sldwd;
      Gclr *gc;
      int EVGAY;
      f = ( DIF * ) tmp;
      if ( f->code == 'f' ) {
          ptr = f->sptr;
          D = f->D;
      }
      else {
          ptr = ( ( DID * ) tmp )->sptr;
          D = ( ( DID * ) tmp )->D;
      }
      EVGAY = D->evgay;
      gc = & ( D->gc ) ;
      if ( min >= max ) {
          normal ( ) ;
          printf ( "Error: Wrong slide-bar limits: min=%d max=%d\n" , min , max ) ;
          exit ( 0 ) ;
      }
      if ( ( df-min ) * ( max-df ) < 0 ) df = ( max+min ) /2;
      scr_ln = l+2*wd+6;
      ptr->x = x;
      ptr->y = y;
      ptr->l = l;
      ptr->df = df;
      ptr->min = min;
      ptr->max = max;
      ptr->fac = fac;
      ptr->code = ch;
      ptr->cf = cf;
      ptr->sh = sh;
      ptr->Sdpw = Sdpw;
      ptr->Sw = Sw;
      ptr->Sldwd = Sldwd;
      ptr->D = D;
      ptr->fill_clr = gc->fill_clr;
      ptr->char_clr = gc->char_clr;
      ptr->slide_fill_clr = gc->txt_fill;
      ptr->bright_clr = gc->vbright;
      return ptr;
  }
  int _uiMake_SF ( DIF *f ) {
      int ret = 1;
      DIALOG *D;
      int x1 , y1 , x2 , y2;
      D = f->D;
      x1 = f->x1+D->xo;
      x2 = f->x2+D->xo;
      y1 = f->y1+D->yo;
      y2 = f->y2+D->yo;
      if ( f->hide != 1 ) {
          int n = 0 , lngth = 0 , i = 0;
          S_STR *ptr;
          CHECKLIMITS ( f ) ;
          if ( f->Bimg == NULL ) f->Bimg = kgGetBackground ( D , x1 , y1 , x2 , y2 ) ;
//  else kgRestoreImage(D,f->Bimg,x1,y1,(x2-x1+1),(y2-y1+1));
          Make_f_bar ( f ) ;
          ptr = f->sptr;
          if ( f->prompt != NULL ) {
              while ( ( f->prompt [ i ] ) == ' ' ) i++;
              while ( ( f->prompt [ n+i ] ) >= ' ' ) n++;
              if ( n != 0 ) {
                  lngth = n*9+4;
//        uiString(D,f->prompt+i,f->x1+D->xo+2,f->y1+D->yo+21,n*9-5,20,D->gc.PromptFont,D->gc.txt_pchar,D->gc.FontSize,1,D->gc.fill_clr);
                  uiString ( D , f->prompt+i , f->x1+D->xo+2 , f->y1+D->yo+21 , n*9-5 , 20 , \
                       D->gc.PromptFont , D->gc.txt_pchar , D->gc.FontSize , 1 , -1 ) ;
              }
          }
          _ui_slidebar ( D , ptr ) ;
          _ui_moveslidepointer ( D , ptr ) ;
          _ui_slidevalue ( D , ptr ) ;
      }
      else {
#if 0
          DIALOG *D;
          kgWC *wc;
          int x1 , y1 , x2 , y2;
          D = f->D;
          wc = WC ( D ) ;
          ret = 0;
          x1 = f->x1+D->xo;
          x2 = f->x2+D->xo;
          y1 = f->y1+D->yo;
          y2 = f->y2+D->yo;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( x1 ) , ( y1 ) , ( x2 ) , \
           ( y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          if ( f->Bimg != NULL ) kgRestoreImage ( D , f->Bimg , x1 , y1 , \
           ( x2-x1+1 ) , ( y2-y1+1 ) ) ;
      }
      return ret;
  }
  int _uiMake_SD ( DID *f ) {
      int ret = 1;
      DIALOG *D;
      int x1 , y1 , x2 , y2;
      D = f->D;
      x1 = f->x1+D->xo;
      x2 = f->x2+D->xo;
      y1 = f->y1+D->yo;
      y2 = f->y2+D->yo;
      if ( f->hide != 1 ) {
          int n = 0 , lngth = 0 , i = 0;
          S_STR *ptr;
          CHECKLIMITS ( f ) ;
          if ( f->Bimg == NULL ) f->Bimg = kgGetBackground ( D , x1 , y1 , x2 , y2 ) ;
//  else kgRestoreImage(D,f->Bimg,x1,y1,(x2-x1+1),(y2-y1+1));
          Make_d_bar ( f ) ;
          ptr = f->sptr;
          if ( f->prompt != NULL ) {
              while ( ( f->prompt [ i ] ) == ' ' ) i++;
              while ( ( f->prompt [ n+i ] ) >= ' ' ) n++;
              if ( n != 0 ) {
                  lngth = n*9+4;
//        uiString(D,f->prompt+i,f->x1+D->xo+2,f->y1+D->yo+21,n*9-5,20,D->gc.PromptFont,D->gc.txt_pchar,D->gc.FontSize,1,D->gc.fill_clr);
                  uiString ( D , f->prompt+i , f->x1+D->xo+2 , f->y1+D->yo+21 , n*9-5 , 20 , \
                       D->gc.PromptFont , D->gc.txt_pchar , D->gc.FontSize , 1 , -1 ) ;
              }
          }
          _ui_slidebar ( D , ptr ) ;
          _ui_moveslidepointer ( D , ptr ) ;
          _ui_slidevalue ( D , ptr ) ;
      }
      else {
#if 0
          DIALOG *D;
          kgWC *wc;
          D = f->D;
          wc = WC ( D ) ;
          ret = 0;
          x1 = f->x1+D->xo;
          x2 = f->x2+D->xo;
          y1 = f->y1+D->yo;
          y2 = f->y2+D->yo;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( x1 ) , ( y1 ) , ( x2 ) , \
           ( y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          if ( f->Bimg != NULL ) kgRestoreImage ( D , f->Bimg , x1 , y1 , \
           ( x2-x1+1 ) , ( y2-y1+1 ) ) ;
      }
      return ret;
  }
  int _uiMake_SH ( DIHB *f ) {
      int ret = 1;
      int x1 , y1 , x2 , y2;
      DIALOG *D;
      D = f->D;
      x1 = f->x1+D->xo;
      x2 = f->x2+D->xo;
      y1 = f->y1+D->yo;
      y2 = f->y2+D->yo;
      if ( f->hide != 1 ) {
          int n = 0 , lngth = 0 , i = 0;
          S_STR *ptr;
          CHECKLIMITS ( f ) ;
          if ( f->Bimg == NULL ) f->Bimg = kgGetBackground ( D , x1 , y1 , x2 , y2 ) ;
//  else kgRestoreImage(D,f->Bimg,x1,y1,(x2-x1+1),(y2-y1+1));
          Make_h_bar ( f ) ;
          ptr = f->sptr;
  //_ui_hbar(f);
          _ui_movehslidepointer ( f ) ;
      }
      else {
#if 0
          DIALOG *D;
          kgWC *wc;
          int x1 , y1 , x2 , y2;
          D = f->D;
          wc = WC ( D ) ;
          ret = 0;
          x1 = f->x1+D->xo;
          x2 = f->x2+D->xo;
          y1 = f->y1+D->yo;
          y2 = f->y2+D->yo;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( x1 ) , ( y1 ) , ( x2 ) , \
           ( y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          if ( f->Bimg != NULL ) kgRestoreImage ( D , f->Bimg , x1 , y1 , \
           ( x2-x1+1 ) , ( y2-y1+1 ) ) ;
      }
      return ret;
  }
  void * _ui_sethbar ( DIHB *B , int min , int max , int x , int y , \
       int l , int df , char ch , double cf , double sh ) \
  {
      DIALOG *D;
      S_STR *ptr;
      double fac , val;
      int scr_ln , wd = Sldwd;
      Gclr *gc;
      D = B->D;
      gc = & ( D->gc ) ;
      if ( min >= max ) {
          normal ( ) ;
          printf ( "Error: Wrong slide-bar limits: min=%d max=%d\n" , min , max ) ;
          exit ( 0 ) ;
      }
      if ( ( df-min ) * ( max-df ) < 0 ) df = ( max+min ) /2;
      scr_ln = l+6;
      ptr = ( S_STR * ) B->sptr;
      ptr->x = x-wd;
      ptr->y = y;
      ptr->l = l;
      ptr->df = df;
      ptr->min = min;
      ptr->max = max;
      ptr->fac = fac;
      ptr->code = ch;
      ptr->cf = cf;
      ptr->sh = sh;
      ptr->Sdpw = Sdpw;
      ptr->Sw = Sw;
      ptr->Sldwd = Sldwd;
      ptr->D = D;
      ptr->fill_clr = gc->fill_clr;
      ptr->char_clr = gc->char_clr;
      ptr->slide_fill_clr = gc->txt_fill;
      ptr->bright_clr = gc->vbright;
      return ptr;
  }
  int Make_m_bar ( DIS *v ) {
      BRW_STR *br;
      DIALOG *D;
      int x1 , y1;
      int min = 0 , max , df;
      double ch , sh;
      int ret , n = 0 , lngth = 0 , i = 0 , offset , w;
      D = v->D;
      br = v->bwsr;
      x1 = D->xo;
      y1 = D->yo;
      v->offset = 0;
      offset = v->offset;
      w = v->w;
      br->tbx1 = br->x2-w-offset ;
      br->tby1 = br->y1+offset;
      br->tbx2 = br->tbx1+w;
      br->tby2 = br->tby1+w;
      br->bbx1 = br->tbx1;
//   v->bby1=v->y2-v->w+y1+offset/2;
      br->bby1 = br->y2-w-offset;
      br->bbx2 = br->bbx1+w;
      br->bby2 = br->bby1+w;
      br->rsx1 = br->tbx2+offset;
      br->rsy1 = br->tby2;
      br->rsx2 = br->rsx1+w-offset;
      br->rsy2 = br->bby1;
      return ( ret ) ;
  }
  int Make_v_bar ( DIV *v ) {
      DIALOG *D;
      int x1 , y1;
      int min = 0 , max , df;
      double ch , sh;
      int ret , n = 0 , lngth = 0 , i = 0 , offset;
      D = v->D;
      x1 = D->xo;
      y1 = D->yo;
      v->offset = 0;
      v->w = ( v->x2-v->x1 ) ;
      offset = ( v->w ) *0.2;
      v->tbx1 = v->x1 +x1;
      v->tby1 = v->y1+y1;
      v->tbx2 = v->tbx1+v->w;
      v->tby2 = v->tby1+v->w;
      v->bbx1 = v->tbx1;
//   v->bby1=v->y2-v->w+y1+offset/2;
      v->bby1 = v->y2-v->w+y1;
      v->bbx2 = v->bbx1+v->w;
      v->bby2 = v->bby1+v->w;
      v->rsx1 = v->x1+x1+offset;
      v->rsy1 = v->tby2;
      v->rsx2 = v->x2+x1-offset;
      v->rsy2 = v->bby1;
      v->offset = offset;
      return ( ret ) ;
  }
  int Make_z_bar ( DIZ *v ) {
      DIALOG *D;
      int x1 , y1;
      int min = 0 , max , df;
      double ch , sh;
      int ret , n = 0 , lngth = 0 , i = 0 , offset = 0;
      D = v->D;
      x1 = D->xo;
      y1 = D->yo;
      v->offset = 0;
      v->w = ( v->y2-v->y1 ) ;
      offset = ( v->w ) *0.2;
      v->lby1 = v->y1+y1;
      v->lbx1 = v->x1 +x1;
      v->lbx2 = v->lbx1+v->w;
      v->lby2 = v->lby1+v->w;
      v->rby1 = v->lby1;
//   v->rbx1=x1+v->x2-v->w+offset/2;
      v->rbx1 = x1+v->x2-v->w+1;
      v->rbx2 = v->rbx1+v->w;
      v->rby2 = v->rby1+v->w;
      v->rsx1 = v->lbx2;
      v->rsy1 = v->lby1+offset;
      v->rsx2 = v->x2+x1-v->w;
      v->rsy2 = v->rby2-offset;
      v->offset = offset;
      return ( ret ) ;
  }
  int Make_f_bar ( DIF *f ) {
      int x1 , y1;
      S_STR *sptr;
      int min = 0 , max , df;
      double ch , sh;
      int ret , n = 0 , lngth = 0 , i = 0;
      DIALOG *D;
      D = f->D;
      x1 = D->xo;
      y1 = D->yo;
      max = f->lngth;
      sh = f->min;
      ch = ( f->max -sh ) /max;
      df = ( * ( f->df ) -sh ) /ch+0.5;
      sptr = ( S_STR * ) f->sptr;
      sptr->Widget = f;
      _ui_setslide ( f , min , max , ( int ) f->x1+lngth+x1+2 , \
           ( int ) f->y1+y1+22 , \
       ( int ) f->lngth , df , 'f' , ch , sh ) ;
      return ( ret ) ;
  }
  int Make_d_bar ( DID *d ) {
      int x1 , y1;
      S_STR *sptr;
      int min = 0 , max , df;
      double ch , sh;
      int ret , n = 0 , lngth = 0 , i = 0;
      DIALOG *D;
      D = d->D;
      x1 = D->xo;
      y1 = D->yo;
      max = d->lngth;
      sh = d->min;
      ch = ( d->max -sh ) /max;
      df = ( * ( d->df ) -sh ) /ch+0.5;
      sptr = ( S_STR * ) d->sptr;
      sptr->Widget = d;
      _ui_setslide ( d , min , max , ( int ) d->x1+lngth+x1+2 , \
           ( int ) d->y1+y1+22 , \
       ( int ) d->lngth , df , 'd' , ch , sh ) ;
      return ( ret ) ;
  }
  void uiSetHslide ( void *tmp , int i , int val ) {
      S_STR *pt;
      DIHB *B;
      B = ( ( DIALOG * ) tmp )->d [ i ] .B;
      if ( val < B->min ) val = B->min;
      if ( val > B->max ) val = B->max;
      * ( B->df ) = val;
      pt = ( S_STR * ) B->sptr;
      pt->df = ( ( val -B->min ) *B->lngth ) / ( B->max - B->min ) ;
      _ui_movehslidepointer ( B ) ;
      return;
  }
  void uiSetDslide ( void *tmp , int i , int val ) {
      S_STR *pt;
      DID *B;
      B = ( ( DIALOG * ) tmp )->d [ i ] .d;
      if ( val < B->min ) val = B->min;
      if ( val > B->max ) val = B->max;
      * ( B->df ) = val;
      pt = ( S_STR * ) B->sptr;
      pt->df = ( ( val -B->min ) *B->lngth ) / ( B->max - B->min ) ;
      _ui_moveslidepointer ( ( tmp ) , pt ) ;
      _ui_slidevalue ( tmp , pt ) ;
      return;
  }
  int uiGetHslide ( void *tmp , int i ) {
      int val;
      S_STR *pt;
      DIHB *B;
      B = ( ( DIALOG * ) tmp )->d [ i ] .B;
      * ( B->df ) = val;
      pt = ( S_STR * ) B->sptr;
//   pt->df = ((val -B->min)*B->lngth)/(B->max - B->min);
      val = ( ( pt->df ) * ( B->max - B->min ) ) / ( B->lngth ) +B->min;
      return val;
  }
  int uiGetDslide ( void *tmp , int i ) {
      int val;
      S_STR *pt;
      DID *B;
      B = ( ( DIALOG * ) tmp )->d [ i ] .d;
      * ( B->df ) = val;
      pt = ( S_STR * ) B->sptr;
//   pt->df = ((val -B->min)*B->lngth)/(B->max - B->min);
      val = ( ( pt->df ) * ( B->max - B->min ) ) / ( B->lngth ) +B->min;
      return val;
  }
  static void _uiMakeSBarImages_o ( DIHB *B ) {
      DIG *fid;
      int type = 0 , color;
      int w , l , of = 4 , fillcolor , r , g , b , xsize , ysize;
      float h , s , v , vmax , vmin , rfac = 0.5;
      DIALOG *D;
      S_STR *pt;
      type = B->type-2;
      color = B->sldclr;
      pt = B->sptr;
      D = ( DIALOG * ) B->D;
      if ( color < 0 ) {
          if ( color == -1 ) color = D->gc.fill_clr;
      }
      w = pt->Sw+of;
      l = pt->l+pt->Sldwd+of;
      fid = ( DIG * ) kgInitImage ( l , w , RESIZE ) ;
      kgUserFrame ( fid , - ( float ) l*0.5 , - ( float ) w*0.5 , \
       ( float ) l*0.5 , ( float ) w*0.5 ) ;
      kgRoundedRectangleFill ( fid , ( float ) .0 , .0 , ( float ) ( l+of*0.5 ) , \
       ( float ) ( w+of*0.5 ) , 0 , D->gc.fill_clr , 0.5 ) ;
      kgRoundedRectangleFill ( fid , ( float ) 1.0 , -1.0 , ( float ) \
       ( l-of ) , ( float ) ( w-of ) , 0 , D->gc. vbright , 0.5 ) ;
      kgRoundedRectangleFill ( fid , ( float ) -1.0 , 0.0 , ( float ) \
       ( l-of ) , ( float ) ( w-of ) , 0 , D->gc.v_dim , 0.5 ) ;
      kgRoundedRectangleFill ( fid , ( float ) 0.0 , 0.0 , ( float ) \
       ( l-of ) , ( float ) ( w-of ) , 0 , D->gc.dim , 0.5 ) ;
// kgRoundedRectanglePressed(fid,0.,0.,(float)l,(float)w,D->gc.v_dim,0.5,1);
      pt->bimg = ( void * ) kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      w = w;
      l = pt->Sldwd+of;
      fid = ( DIG * ) kgInitImage ( l , w , RESIZE ) ;
      kgUserFrame ( fid , - ( float ) l*0.5 , - ( float ) w*0.5 , \
       ( float ) l*0.5 , ( float ) w*0.5 ) ;
      fillcolor = color;
      xsize = ( l-of ) -0.5*of; ysize = ( w-of ) -0.5*of;
      if ( fillcolor >= 0 ) kgGetRGB ( ( DIG * ) fid , \
           fillcolor , & r , & g , & b ) ;
      else {
          fillcolor = -fillcolor;
          b = ( fillcolor%1000 ) ;
          g = ( fillcolor/1000 ) %1000;
          r = ( fillcolor/1000000 ) %1000;
          b = b%256; g = g%256; r = r%256;
          fillcolor = 127;
          kgChangeColor ( fid , fillcolor , r , g , b ) ;
      }
// kgRoundedRectangleFill(fid,(float)0.0,0.0,(float)(l-of),(float)(w-of),0,fillcolor,0.5);
// kgRoundedRectangleRaised(fid,0.,0.,(float)(l-of),(float)(w-of),fillcolor,0.5,1);
      RGBtoHSV ( ( float ) r , ( float ) g , ( float ) b , & h , & s , & v ) ;
      if ( v > 1 ) v = 1.0;
      vmin = 0.85*v;
      vmax = 1.2*v;
      if ( vmax > 1.0 ) vmax = 1.0;
      vmin = vmax*0.75;
#if 0
      kgRoundedRectangleFill ( fid , -1.0 , 0.0 , ( float ) xsize , \
       ( float ) ysize , 0 , 15 , rfac ) ;
      kgRoundedRectangleFill ( fid , 1.0 , -1.0 , ( float ) xsize , \
       ( float ) ysize , 0 , 0 , rfac ) ;
#endif
      switch ( type ) {
          case 0:
          kgRoundedRectangleShade0 ( fid , 0.0 , 0.0 , ( float ) xsize , \
           ( float ) ysize , fillcolor , rfac , vmin , vmax ) ;
          break;
          case 1:
          kgRoundedRectangleShade1 ( fid , 0.0 , 0.0 , ( float ) xsize , \
           ( float ) ysize , fillcolor , rfac , vmin , vmax*0.95 ) ;
          break;
          case 2:
          kgRoundedRectangleFill ( fid , -1.0 , 1.0 , ( float ) xsize , \
           ( float ) ysize , 0 , 15 , rfac ) ;
          kgRoundedRectangleShade ( fid , 0.0 , 1.0 , ( float ) xsize , \
           ( float ) ysize , fillcolor , rfac , vmin*1.2 , vmax ) ;
          break;
          case 3:
          kgRoundedRectangleFill ( fid , -1.0 , 1.0 , ( float ) xsize , \
           ( float ) ysize , 0 , 15 , rfac ) ;
          kgRoundedRectangleShade0 ( fid , 0.0 , 0.0 , ( float ) xsize , \
           ( float ) ysize , fillcolor , rfac , v , vmax*0.95 ) ;
          break;
          case 4:
          kgRoundedRectangleFill ( fid , -1.0 , +1.0 , ( float ) xsize , \
           ( float ) ysize , 0 , 15 , rfac ) ;
          kgRoundedRectangleShade ( fid , 0.0 , 0.0 , ( float ) xsize , \
           ( float ) ysize , fillcolor , rfac , vmin , vmax*0.95 ) ;
          break;
          default:
          kgRoundedRectangleFill ( fid , -1.0 , 0.5 , ( float ) xsize , \
           ( float ) ysize , 0 , 15 , rfac ) ;
          kgRoundedRectangleFill ( fid , 0.0 , 0.0 , ( float ) xsize , \
           ( float ) ysize , 0 , fillcolor , rfac ) ;
          break;
      }
//TCB
#if 0
      kgRoundedRectangle ( fid , 0.0 , .0 , ( float ) xsize+1.0 , \
       ( float ) ysize+1.0 , D->gc.v_dim , rfac , 0.5 ) ;
#endif
      kgLineColor ( fid , 0 ) ;
      kgMove2f ( fid , -1.0 , ysize*.5-1 ) ;
      kgDraw2f ( fid , -1.0 , -ysize*.5+1 ) ;
      kgLineColor ( fid , 15 ) ;
      kgMove2f ( fid , -.0 , ysize*.5-1 ) ;
      kgDraw2f ( fid , -.0 , -ysize*.5+1 ) ;
      pt->fimg = ( void * ) kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
  }
  static void _uiMakeSBarImages ( DIHB *B ) {
      DIG *fid;
      int type = 0 , color;
      int w , l , of = 4 , fillcolor , r , g , b , xsize , ysize , dummy;
      float h , s , v , vmax , vmin , rfac = 0.5 , red , green , blue;
      DIALOG *D;
      S_STR *pt;
      type = B->type-2;
      color = B->sldclr;
      pt = B->sptr;
      D = ( DIALOG * ) B->D;
      if ( color < 0 ) {
          if ( color == -1 ) color = D->gc.fill_clr;
      }
      w = pt->Sw+of;
      l = pt->l+pt->Sldwd+of;
      fid = ( DIG * ) kgInitImage ( l , w , RESIZE ) ;
      kgUserFrame ( fid , - ( float ) l*0.5 , - ( float ) w*0.5 , \
       ( float ) l*0.5 , ( float ) w*0.5 ) ;
      fillcolor = D->gc.fill_clr;
      if ( fillcolor >= 0 ) kgGetRGB ( ( DIG * ) fid , \
           fillcolor , & r , & g , & b ) ;
      else {
          fillcolor = -fillcolor;
          b = ( fillcolor%1000 ) ;
          g = ( fillcolor/1000 ) %1000;
          r = ( fillcolor/1000000 ) %1000;
          b = b%256; g = g%256; r = r%256;
          fillcolor = 127;
          kgChangeColor ( fid , fillcolor , r , g , b ) ;
      }
      RGBtoHSV ( ( float ) r , ( float ) g , ( float ) b , & h , & s , & v ) ;
      vmax = v*1.3;
      if ( vmax > 1. ) vmax = 1.0;
      HSVtoRGB ( & red , & green , & blue , h , s , vmax ) ;
      kgChangeColor ( fid , 150 , ( int ) red , ( int ) green , ( int ) blue ) ;
      kgChangeColor ( fid , 0 , 0 , 0 , 0 ) ;
      kgChangeColor ( fid , 15 , 255 , 255 , 255 ) ;
      kgRoundedRectanglePressed ( fid , 0. , 0. , ( float ) ( l-of ) , \
       ( float ) ( w-of ) , D->gc.fill_clr , 0.5 , 3 ) ;
      kgRoundedRectangleFill ( fid , 0. , 0. , ( float ) ( l-of-4.5 ) , \
       ( float ) ( w-of-4.5 ) , 0 , 15 , 0.5 ) ;
      pt->bimg = ( void * ) kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      w = w;
      l = pt->Sldwd+of;
      fid = ( DIG * ) kgInitImage ( l , w , RESIZE ) ;
      kgUserFrame ( fid , - ( float ) l*0.5 , - ( float ) w*0.5 , \
       ( float ) l*0.5 , ( float ) w*0.5 ) ;
      xsize = ( l-of ) -0.5*of; ysize = ( w-of ) -0.5*of;
      fillcolor = color;
      if ( fillcolor >= 0 ) kgGetRGB ( ( DIG * ) fid , \
           fillcolor , & r , & g , & b ) ;
      else {
          fillcolor = -fillcolor;
          b = ( fillcolor%1000 ) ;
          g = ( fillcolor/1000 ) %1000;
          r = ( fillcolor/1000000 ) %1000;
          b = b%256; g = g%256; r = r%256;
          fillcolor = 127;
          kgChangeColor ( fid , fillcolor , r , g , b ) ;
      }
      RGBtoHSV ( ( float ) r , ( float ) g , ( float ) b , & h , & s , & v ) ;
      if ( v > 1 ) v = 1.0;
      vmin = 0.85*v;
      vmax = 1.5*v;
      if ( vmax > 1.0 ) vmax = 1.0;
      HSVtoRGB ( & red , & green , & blue , h , s , vmax ) ;
      kgChangeColor ( fid , 150 , ( int ) red , ( int ) green , ( int ) blue ) ;
      vmin = vmax*0.75;
      kgRoundedRectangleRaised ( fid , 0. , 0. , ( float ) ( l-of ) , \
       ( float ) ( w-of ) , fillcolor , 0.5 , 1 ) ;
      switch ( type ) {
          case 0:
          kgRoundedRectangleShade0 ( fid , 0.0 , 0.0 , ( float ) xsize , \
           ( float ) ysize , fillcolor , rfac , vmin , vmax ) ;
          break;
          case 1:
          kgRoundedRectangleShade1 ( fid , 0.0 , 0.0 , ( float ) xsize , \
           ( float ) ysize , fillcolor , rfac , vmin , vmax*0.95 ) ;
          break;
          case 2:
          kgRoundedRectangleFill ( fid , -1.0 , 1.0 , ( float ) xsize , \
           ( float ) ysize , 0 , 150 , rfac ) ;
          kgRoundedRectangleShade ( fid , 0.0 , 1.0 , ( float ) xsize , \
           ( float ) ysize , fillcolor , rfac , vmin*1.2 , vmax ) ;
          break;
          case 3:
          kgRoundedRectangleFill ( fid , -1.0 , 1.0 , ( float ) xsize , \
           ( float ) ysize , 0 , 150 , rfac ) ;
          kgRoundedRectangleShade0 ( fid , 0.0 , 0.0 , ( float ) xsize , \
           ( float ) ysize , fillcolor , rfac , v , vmax*0.95 ) ;
          break;
          case 4:
          kgRoundedRectangleFill ( fid , -1.0 , +1.0 , ( float ) xsize , \
           ( float ) ysize , 0 , 150 , rfac ) ;
          kgRoundedRectangleShade ( fid , 0.0 , 0.0 , ( float ) xsize , \
           ( float ) ysize , fillcolor , rfac , vmin , vmax*0.95 ) ;
          break;
          default:
          kgRoundedRectangleFill ( fid , -1.0 , 0.5 , ( float ) xsize , \
           ( float ) ysize , 0 , 150 , rfac ) ;
          kgRoundedRectangleFill ( fid , 0.0 , 0.0 , ( float ) xsize , \
           ( float ) ysize , 0 , fillcolor , rfac ) ;
          break;
      }
      kgLineColor ( fid , 0 ) ;
      kgMove2f ( fid , -0.0 , ysize*.5-1 ) ;
      kgDraw2f ( fid , -0.0 , -ysize*.5+1 ) ;
      kgLineColor ( fid , 15 ) ;
      kgMove2f ( fid , 1.0 , ysize*.5-1 ) ;
      kgDraw2f ( fid , 1.0 , -ysize*.5+1 ) ;
      pt->fimg = ( void * ) kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      if ( B->direction == 1 ) {
          void *timg;
          timg = kgRotateImage ( pt->fimg , 90 ) ;
          kgFreeImage ( pt->fimg ) ;
          pt->fimg = timg;
          kgFlipImage ( pt->fimg ) ;
          timg = kgRotateImage ( pt->bimg , 90 ) ;
          kgFreeImage ( pt->bimg ) ;
          pt->bimg = timg;
          kgFlipImage ( pt->bimg ) ;
      }
  }
  static void _uiMakeSBarImagesType7 ( DIHB *B ) {
      DIG *fid;
      int type = 0 , color;
      int w , l , of = 4 , fillcolor , r , g , b , xsize , ysize;
      float h , s , v , vmax , vmin , rfac = 0.5 , red , green , blue;
      DIALOG *D;
      S_STR *pt;
      type = B->type;
      color = B->sldclr;
      pt = B->sptr;
      D = ( DIALOG * ) B->D;
      if ( color < 0 ) {
          if ( color == -1 ) color = D->gc.fill_clr;
      }
      w = 3+of;
      l = pt->l+pt->Sldwd+of;
      fid = ( DIG * ) kgInitImage ( l , w , RESIZE ) ;
      kgUserFrame ( fid , - ( float ) l*0.5-0.1 , - ( float ) w*0.5-0.1 , \
       ( float ) l*0.5+0.1 , ( float ) w*0.5+0.1 ) ;
      fillcolor = color;
      if ( fillcolor >= 0 ) kgGetRGB ( ( DIG * ) fid , \
           fillcolor , & r , & g , & b ) ;
      else {
          fillcolor = -fillcolor;
          b = ( fillcolor%1000 ) ;
          g = ( fillcolor/1000 ) %1000;
          r = ( fillcolor/1000000 ) %1000;
          b = b%256; g = g%256; r = r%256;
          fillcolor = 127;
          kgChangeColor ( fid , fillcolor , r , g , b ) ;
      }
      RGBtoHSV ( ( float ) r , ( float ) g , ( float ) b , & h , & s , & v ) ;
      HSVtoRGB ( & red , & green , & blue , h , s , vmax*0.6 ) ;
      kgChangeColor ( fid , 140 , ( int ) red , ( int ) green , ( int ) blue ) ;
// kgRoundedRectangleFill(fid,(float).5,-.5,(float)(pt->l+3),(float)(3),0,140,0.5);
      kgRoundedRectangleFill ( fid , ( float ) .0 , .0 , ( float ) \
       ( pt->l+3 ) , ( float ) ( 3.0 ) , 0 , fillcolor , 0.5 ) ;
      pt->bimg = ( void * ) kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      w = pt->Sw+of;
      l = pt->Sldwd+of;
#if 1
      fid = ( DIG * ) kgInitImage ( l , w , RESIZE ) ;
      kgUserFrame ( fid , - ( float ) l*0.5 , - ( float ) w*0.5 , \
       ( float ) l*0.5 , ( float ) w*0.5 ) ;
      xsize = ( w-of ) -.75*of; ysize = ( w-of ) -.75*of;
      fillcolor = color;
      if ( fillcolor >= 0 ) kgGetRGB ( ( DIG * ) fid , \
           fillcolor , & r , & g , & b ) ;
      else {
          fillcolor = -fillcolor;
          b = ( fillcolor%1000 ) ;
          g = ( fillcolor/1000 ) %1000;
          r = ( fillcolor/1000000 ) %1000;
          b = b%256; g = g%256; r = r%256;
          fillcolor = 128;
          kgChangeColor ( fid , fillcolor , r , g , b ) ;
      }
      color = fillcolor;
      RGBtoHSV ( ( float ) r , ( float ) g , ( float ) b , & h , & s , & v ) ;
      if ( v > 1 ) v = 1.0;
      vmin = 0.85*v;
      vmax = 1.5*v;
      if ( vmax > 1.0 ) vmax = 1.0;
#if 0
      HSVtoRGB ( & red , & green , & blue , h , s , vmax ) ;
      kgChangeColor ( fid , 150 , ( int ) red , ( int ) green , ( int ) blue ) ;
      kgRoundedRectangleFill ( fid , ( float ) -1.0 , 0.0 , ( float ) \
       ( w-of ) , ( float ) ( w-of ) , 0 , 150 , 0.5 ) ;
      kgRoundedRectangleFill ( fid , ( float ) 0.5 , -0.5 , ( float ) \
       ( w-of ) , ( float ) ( w-of ) , 0 , 0 , 0.5 ) ;
      kgRoundedRectangleFill ( fid , ( float ) 0.0 , 0.0 , ( float ) \
       ( w-of ) , ( float ) ( w-of ) , 0 , fillcolor , 0.5 ) ;
// kgRoundedRectangleRaised(fid,0.,0.,(float)(w-of),(float)(w-of),fillcolor,0.5,1);
// RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&v);
      kgChangeColor ( fid , fillcolor , 220 , 220 , 220 ) ;
      RGBtoHSV ( ( float ) 220 , ( float ) 220 , ( float ) 220 , & h , & s , & v ) ;
      if ( v > 1 ) v = 1.0;
      vmin = 0.85*v;
      vmax = 1.5*v;
      if ( vmax > 1.0 ) vmax = 1.0;
      vmin = vmax*0.85;
      kgRoundedRectangleShade0 ( fid , 0.0 , 0.0 , ( float ) xsize , \
       ( float ) ysize , fillcolor , rfac , v , vmax*0.95 ) ;
// kgRoundedRectangleShade1(fid,0.0,0.0,(float)xsize,(float)ysize,fillcolor,rfac,vmin,vmin);
#else
      HSVtoRGB ( & red , & green , & blue , h , s , vmax*0.85 ) ;
      kgChangeColor ( fid , 150 , ( int ) red , ( int ) green , ( int ) blue ) ;
      HSVtoRGB ( & red , & green , & blue , h , s , vmax ) ;
      kgChangeColor ( fid , 151 , ( int ) red , ( int ) green , ( int ) blue ) ;
      HSVtoRGB ( & red , & green , & blue , h , s , vmax*0.9 ) ;
      kgChangeColor ( fid , 153 , ( int ) red , ( int ) green , ( int ) blue ) ;
      HSVtoRGB ( & red , & green , & blue , h , s , vmax*0.95 ) ;
      kgChangeColor ( fid , 152 , ( int ) red , ( int ) green , ( int ) blue ) ;
      kgRoundedRectangleFill ( fid , ( float ) 0.0 , -.0 , ( float ) \
       ( w-of ) , ( float ) ( w-of ) , 0 , 150 , 0.5 ) ;
      kgRoundedRectangleFill ( fid , ( float ) 0.0 , 0.0 , ( float ) \
       ( w-of-1 ) , ( float ) ( w-of-1 ) , 0 , 153 , 0.5 ) ;
      kgRoundedRectangleFill ( fid , ( float ) 0.0 , 0.0 , ( float ) \
       ( w-of-3.0 ) , ( float ) ( w-of-3.0 ) , 0 , 152 , 0.5 ) ;
      kgRoundedRectangleFill ( fid , ( float ) 0.0 , 0.0 , ( float ) \
       ( w-of-6 ) , ( float ) ( w-of-6 ) , 0 , 151 , 0.5 ) ;
#endif
      kgLineColor ( fid , 0 ) ;
      kgMove2f ( fid , 0.0 , ysize*.5 ) ;
      kgDraw2f ( fid , 0.0 , -ysize*.5 ) ;
      kgLineColor ( fid , 15 ) ;
      kgMove2f ( fid , 1.0 , ysize*.5 ) ;
      kgDraw2f ( fid , 1.0 , -ysize*.5 ) ;
// pt->fimg =  (void *)kgGetResizedImage(fid);
      pt->fimg = ( void * ) kgGetSmoothImage ( fid ) ;
      kgCloseImage ( fid ) ;
#else
      pt->fimg = kgBorderedRectangle ( ( int ) ( w-of ) , \
       ( int ) ( w-of ) , color , 0.5 ) ;
#endif
      if ( B->direction == 1 ) {
          void *timg;
          timg = kgRotateImage ( pt->fimg , 90 ) ;
          kgFreeImage ( pt->fimg ) ;
          pt->fimg = timg;
          kgFlipImage ( pt->fimg ) ;
          timg = kgRotateImage ( pt->bimg , 90 ) ;
          kgFreeImage ( pt->bimg ) ;
          pt->bimg = timg;
          kgFlipImage ( pt->bimg ) ;
      }
  }
  static void _uiMakeSBarImagesType0 ( DIHB *B ) {
      DIG *fid;
      int type = 0 , color;
      int w , l , of = 4 , fillcolor , r , g , b , xsize , ysize;
      float h , s , v , vmax , vmin , rfac = 0.15 , red , green , blue;
      DIALOG *D;
      S_STR *pt;
      type = B->type;
      color = B->sldclr;
      pt = B->sptr;
      D = ( DIALOG * ) B->D;
      if ( color < 0 ) {
          if ( color == -1 ) color = D->gc.fill_clr;
      }
      w = 7+of;
      l = pt->l+pt->Sldwd+of;
      fid = ( DIG * ) kgInitImage ( l , w , RESIZE ) ;
      kgUserFrame ( fid , - ( float ) l*0.5 , - ( float ) w*0.5 , \
       ( float ) l*0.5 , ( float ) w*0.5 ) ;
      b = ( 30 ) ;
      g = ( 30 ) ;
      r = ( 30 ) ;
      fillcolor = 127;
      kgChangeColor ( fid , fillcolor , r , g , b ) ;
      kgRoundedRectangleFill ( fid , 0. , 0. , ( float ) ( l-of ) , \
       ( float ) ( w-of ) , 0 , fillcolor , 0.2 ) ;
      kgRoundedRectanglePressed ( fid , 0. , 0. , ( float ) ( l-of ) , \
       ( float ) ( w-of ) , D->gc.fill_clr , 0.2 , 2 ) ;
      pt->bimg = ( void * ) kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      w = pt->Sw+of;
      l = pt->Sldwd+of;
      fid = ( DIG * ) kgInitImage ( l , w , RESIZE ) ;
      kgUserFrame ( fid , - ( float ) l*0.5 , - ( float ) w*0.5 , \
       ( float ) l*0.5 , ( float ) w*0.5 ) ;
      xsize = ( l ) -.75*of; ysize = ( w-of ) -.75*of;
      fillcolor = color;
      if ( fillcolor >= 0 ) kgGetRGB ( ( DIG * ) fid , \
           fillcolor , & r , & g , & b ) ;
      else {
          fillcolor = -fillcolor;
          b = ( fillcolor%1000 ) ;
          g = ( fillcolor/1000 ) %1000;
          r = ( fillcolor/1000000 ) %1000;
          b = b%256; g = g%256; r = r%256;
          fillcolor = 127;
          kgChangeColor ( fid , fillcolor , r , g , b ) ;
      }
#if 0
      kgRoundedRectangleFill ( fid , ( float ) -1.0 , 0.0 , ( float ) \
       ( w-of ) , ( float ) ( w-of ) , 0 , 15 , 0.5 ) ;
      kgRoundedRectangleFill ( fid , ( float ) 0.5 , -0.5 , ( float ) \
       ( w-of ) , ( float ) ( w-of ) , 0 , 0 , 0.5 ) ;
      kgRoundedRectangleFill ( fid , ( float ) 0.0 , 0.0 , ( float ) \
       ( w-of ) , ( float ) ( w-of ) , 0 , fillcolor , 0.5 ) ;
#endif
// kgChangeColor(fid,fillcolor,220,220,220);
      RGBtoHSV ( ( float ) r , ( float ) g , ( float ) b , & h , & s , & v ) ;
      if ( v > 1 ) v = 1.0;
      vmin = 0.85*v;
      vmax = 1.5*v;
      if ( vmax > 1.0 ) vmax = 1.0;
      HSVtoRGB ( & red , & green , & blue , h , s , vmax ) ;
      kgChangeColor ( fid , 150 , ( int ) red , ( int ) green , ( int ) blue ) ;
      vmin = vmax*0.75;
      kgRoundedRectangleFill ( fid , ( float ) -0.5 , 0.5 , \
       ( float ) xsize , ( float ) ysize , 0 , 150 , rfac ) ;
      kgRoundedRectangleFill ( fid , ( float ) 0.5 , -0.5 , \
       ( float ) xsize , ( float ) ysize , 0 , 0 , rfac ) ;
      kgRoundedRectangleShade0 ( fid , 0.0 , 0.0 , ( float ) xsize , \
       ( float ) ysize , fillcolor , rfac , vmin , vmax*0.95 ) ;
      kgLineColor ( fid , 0 ) ;
      kgMove2f ( fid , 0.0 , ysize*.5 ) ;
      kgDraw2f ( fid , 0.0 , -ysize*.5 ) ;
      kgLineColor ( fid , 15 ) ;
      kgMove2f ( fid , 1.0 , ysize*.5 ) ;
      kgDraw2f ( fid , 1.0 , -ysize*.5 ) ;
      pt->fimg = ( void * ) kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      if ( B->direction == 1 ) {
          void *timg;
          timg = kgRotateImage ( pt->fimg , 90 ) ;
          kgFreeImage ( pt->fimg ) ;
          pt->fimg = timg;
          kgFlipImage ( pt->fimg ) ;
          timg = kgRotateImage ( pt->bimg , 90 ) ;
          kgFreeImage ( pt->bimg ) ;
          pt->bimg = timg;
          kgFlipImage ( pt->bimg ) ;
      }
  }
  static void _uiMakeSBarImagesType1 ( DIHB *B ) {
      DIG *fid;
      int type = 0 , color;
      int w , l , of = 4 , fillcolor , r , g , b , xsize , ysize;
      float h , s , v , vmax , vmin , rfac = 0.15 , red , green , blue;
      DIALOG *D;
      S_STR *pt;
      type = B->type;
      color = B->sldclr;
      pt = B->sptr;
      D = ( DIALOG * ) B->D;
      if ( color < 0 ) {
          if ( color == -1 ) color = D->gc.fill_clr;
      }
      w = 5+of;
      l = pt->l+pt->Sldwd+of;
      fid = ( DIG * ) kgInitImage ( l , w , RESIZE ) ;
      kgUserFrame ( fid , - ( float ) l*0.5 , - ( float ) w*0.5 , \
       ( float ) l*0.5 , ( float ) w*0.5 ) ;
      fillcolor = color;
      if ( fillcolor >= 0 ) kgGetRGB ( ( DIG * ) fid , \
           fillcolor , & r , & g , & b ) ;
      else {
          fillcolor = -fillcolor;
          b = ( fillcolor%1000 ) ;
          g = ( fillcolor/1000 ) %1000;
          r = ( fillcolor/1000000 ) %1000;
          b = b%256; g = g%256; r = r%256;
          fillcolor = 127;
          kgChangeColor ( fid , fillcolor , r , g , b ) ;
      }
      kgRoundedRectangleFill ( fid , 0. , 0. , ( float ) ( l-of ) , \
       ( float ) ( w-of ) , 0 , fillcolor , 0.2 ) ;
// kgRoundedRectanglePressed(fid,0.,0.,(float)(l-of),(float)(w-of),D->gc.fill_clr,0.2,2);
      pt->bimg = ( void * ) kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      w = pt->Sw+of;
      l = pt->Sldwd+of;
      fid = ( DIG * ) kgInitImage ( l , w , RESIZE ) ;
      kgUserFrame ( fid , - ( float ) l*0.5 , - ( float ) w*0.5 , \
       ( float ) l*0.5 , ( float ) w*0.5 ) ;
      xsize = ( l ) -.75*of; ysize = ( w-of ) -.75*of;
      fillcolor = color;
      if ( fillcolor >= 0 ) kgGetRGB ( ( DIG * ) fid , \
           fillcolor , & r , & g , & b ) ;
      else {
          fillcolor = -fillcolor;
          b = ( fillcolor%1000 ) ;
          g = ( fillcolor/1000 ) %1000;
          r = ( fillcolor/1000000 ) %1000;
          b = b%256; g = g%256; r = r%256;
          fillcolor = 127;
          kgChangeColor ( fid , fillcolor , r , g , b ) ;
      }
      RGBtoHSV ( ( float ) r , ( float ) g , ( float ) b , & h , & s , & v ) ;
      if ( v > 1 ) v = 1.0;
      vmin = 0.85*v;
      vmax = 1.5*v;
      if ( vmax > 1.0 ) vmax = 1.0;
      vmin = vmax*0.75;
      HSVtoRGB ( & red , & green , & blue , h , s , vmax ) ;
      kgChangeColor ( fid , 150 , ( int ) red , ( int ) green , ( int ) blue ) ;
      vmin = vmax*0.75;
      kgRoundedRectangleFill ( fid , ( float ) -0.5 , 0.5 , \
       ( float ) xsize , ( float ) ysize , 0 , 150 , rfac ) ;
      kgRoundedRectangleFill ( fid , ( float ) 0.5 , -0.5 , \
       ( float ) xsize , ( float ) ysize , 0 , 0 , rfac ) ;
      kgRoundedRectangleShade0 ( fid , 0.0 , 0.0 , ( float ) xsize , \
       ( float ) ysize , fillcolor , rfac , vmin , vmax*0.95 ) ;
      kgLineColor ( fid , 0 ) ;
      kgMove2f ( fid , 0.0 , ysize*.5 ) ;
      kgDraw2f ( fid , 0.0 , -ysize*.5 ) ;
      kgLineColor ( fid , 15 ) ;
      kgMove2f ( fid , 1.0 , ysize*.5 ) ;
      kgDraw2f ( fid , 1.0 , -ysize*.5 ) ;
      pt->fimg = ( void * ) kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      if ( B->direction == 1 ) {
          void *timg;
          timg = kgRotateImage ( pt->fimg , 90 ) ;
          kgFreeImage ( pt->fimg ) ;
          pt->fimg = timg;
          kgFlipImage ( pt->fimg ) ;
          timg = kgRotateImage ( pt->bimg , 90 ) ;
          kgFreeImage ( pt->bimg ) ;
          pt->bimg = timg;
          kgFlipImage ( pt->bimg ) ;
      }
  }
  int Make_h_bar ( DIHB *d ) {
      int x1 , y1 , x , y;
      int min = 0 , max , df , w;
      double ch , sh;
      int ret , n = 0 , lngth = 0 , i = 0 , offset = 2;
      int scr_ln , wd = Sldwd , l;
      S_STR *ptr;
      DIALOG *D;
      D = d->D;
      df = * ( d->df ) ;
      if ( df < d->min ) df = d->min;
      if ( df > d->max ) df = d->max;
      * ( d->df ) = df;
      ptr = ( S_STR * ) d->sptr;
      x1 = D->xo;
      y1 = D->yo;
      l = d->x2 -d->x1-4*offset;
      w = d->y2 -d->y1-4*offset;
      kgFreeImage ( ptr->bimg ) ;
      kgFreeImage ( ptr->fimg ) ;
      ptr->bimg = ptr->fimg = NULL;
      if ( l >= w ) d->direction = 0;
      else d->direction = 1;
      if ( d->direction == 0 ) {
          if ( w < 8 ) {
              w = 8;
              d->y2 = w+d->y1+4*offset;
          }
          lngth = d->x2 -d->x1-4*offset-w*1.61;
          if ( lngth < 2 ) {lngth = 2; w = ( d->x2 -d->x1-4*offset-lngth ) /1.61; }
          switch ( d->type ) {
              case 0:
              ptr->Sldwd = 6;
              break;
              case 1:
              ptr->Sldwd = 6;
              break;
              default:
              ptr->Sldwd = w*1.61;
              break;
          }
          lngth = d->x2 -d->x1-4*offset-ptr->Sldwd;
      }
      else {
          w = d->x2 -d->x1-4*offset;
          if ( w < 8 ) {
              w = 8;
              d->x2 = w+d->x1+4*offset;
          }
          lngth = d->y2 -d->y1-4*offset-w*1.61;
          if ( lngth < 2 ) {lngth = 2; w = ( d->y2 -d->y1-4*offset-lngth ) /1.61; }
          switch ( d->type ) {
              case 0:
              ptr->Sldwd = 8;
              break;
              case 1:
              ptr->Sldwd = 6;
              break;
              default:
              ptr->Sldwd = w*1.61;
              break;
          }
          lngth = d->y2 -d->y1-4*offset-ptr->Sldwd;
      }
      d->lngth = lngth;
      max = d->lngth;
      sh = d->min;
      ch = ( d->max -sh ) /max;
      df = ( * ( d->df ) -sh ) /ch+0.5;
      switch ( d->direction ) {
          case 0:
          default:
          x = d->x1+x1+2*offset+ptr->Sldwd/2;
          y = d->y1+y1+2*offset+w/2;
          break;
          case 1:
          x = d->x1+x1+2*offset+w/2;
          y = d->y1+y1+2*offset+ptr->Sldwd/2;
          break;
      }
      l = max;
      if ( min >= max ) {
          printf ( "Error: Wrong slide-bar limits: min=%d max=%d\n" , min , max ) ;
          exit ( 0 ) ;
      }
      if ( ( df-min ) * ( max-df ) < 0 ) df = ( max+min ) /2;
      ptr->x = x;
      ptr->y = y;
      ptr->l = l;
      ptr->df = df;
      ptr->cf = ch;
      ptr->sh = sh;
      ptr->min = min;
      ptr->max = max;
      ptr->Sdpw = Sdpw;
      ptr->Sw = w;
      ptr->D = D;
      switch ( d->type ) {
          case 0:
          _uiMakeSBarImagesType0 ( d ) ;
          break;
          case 1:
          _uiMakeSBarImagesType1 ( d ) ;
          break;
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          _uiMakeSBarImages ( d ) ;
          break;
          default:
          case 7:
          _uiMakeSBarImagesType7 ( d ) ;
          break;
      }
      return ( ret ) ;
  }
#if 0
  int Make_fbrowser ( DIX *x , int x1 , int y1 , int XX ) {
      int ret , n = 0 , lngth;
      lngth = XX - ( x->x1+x1 ) +10;
      ret = put_fbrowser ( x->x1+x1 , x->y1+y1-21 , lngth ) ;
      return ( ret ) ;
  }
#endif
  void _uiup_dir_o ( DIALOG *D , int x , int y , int w ) {
      char tmp;
      int xx , yy , ww , i;
      int EVGAY;
      kgWC *wc;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      ww = w/2; xx = x+w/4; yy = y-w+w/2;
      tmp = WC ( D )->c_color;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _uirect_fill ( wc , x , y , x+w , y-w , D->gc.fill_clr ) ;
      uiset_clr ( D , D->gc.bright ) ;
      _uivert_line ( wc , y , y-w , x ) ;
      _uihori_line ( wc , x , x+w , y ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _uivert_line ( wc , y , y-w , x+w ) ;
      _uihori_line ( wc , x , x+w , y-w ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      for ( i = -1; i < ww/2; i++ ) {
          _uihori_line ( wc , xx+i , xx+ww-i , yy+i ) ;
      }
      uiset_clr ( D , tmp ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _uiup_dir ( DIALOG *D , int x , int y , int w , float rfac , int type ) {
      kgWC *wc;
      wc = WC ( D ) ;
      int EVGAY;
      EVGAY = D->evgay;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiShadedString ( D , "!f35!w32!xs" , x-1 , EVGAY-y-1 , w+2 , w+2 , 35 , \
           D->gc.fill_clr , 0 , D->gc.v_dim , D->gc.FontSize-1 , 0 , rfac , 1 , type ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _uidown_dir ( DIALOG *D , int x , int y , \
       int w , float rfac , int type ) \
  {
      kgWC *wc;
      wc = WC ( D ) ;
      int EVGAY;
      EVGAY = D->evgay;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiShadedString ( D , "!f35!w32!xt" , x-1 , EVGAY-y-1 , w+2 , w+2 , 35 , \
           D->gc.fill_clr , 0 , D->gc.v_dim , D->gc.FontSize-1 , 0 , rfac , 1 , type ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _dvup_dir ( DIALOG *D , int x , int y , int w , float rfac , int type ) {
      kgWC *wc;
      wc = WC ( D ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiShadedString ( D , "!f35!w32!xs" , x , y , w , w , 35 , D->gc.scroll_fill , \
           0 , D->gc.v_dim , D->gc.FontSize-1 , 0 , rfac , 1 , type ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _dvdown_dir ( DIALOG *D , int x , int y , \
       int w , float rfac , int type ) \
  {
      kgWC *wc;
      wc = WC ( D ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiShadedString ( D , "!f35!w32!xt" , x , y , w , w , 35 , D->gc.scroll_fill , \
           0 , D->gc.v_dim , D->gc.FontSize-1 , 0 , rfac , 1 , type ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _uidown_dir_o ( DIALOG *D , int x , int y , int w ) {
      char tmp;
      int xx , yy , ww , i;
      int EVGAY;
      kgWC *wc;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      ww = w/2; xx = x+w/4; yy = y-w/2;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _uirect_fill ( wc , x , y , x+w , y-w , D->gc.fill_clr ) ;
      tmp = WC ( D )->c_color;
      uiset_clr ( D , D->gc.bright ) ;
      _uivert_line ( wc , y , y-w , x ) ;
      _uihori_line ( wc , x , x+w , y ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _uivert_line ( wc , y , y-w , x+w ) ;
      _uihori_line ( wc , x , x+w , y-w ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      for ( i = -1; i < ww/2; i++ ) {
          _uihori_line ( wc , xx+i , xx+ww-i , yy-i ) ;
      }
      uiset_clr ( D , tmp ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void ui_draw_browser_o ( BRW_STR *br ) {
      DIALOG *D;
      kgWC *wc;
      char *menu;
      int x1 , y1 , x2 , y2 , yoff , ln;
      int EVGAY;
      D = br->D;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      x1 = br->x1;
      y1 = EVGAY-br->y1;
      x2 = br->x2-br->width-br->offset;
      y2 = EVGAY-br->y2;
      yoff = ( br->y2-br->y1-br->width ) *0.5;
      y1-= yoff;
      y2+= yoff;
      ln = br->x2-br->x1-32;
      if ( ln < 12 ) ln = 12;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _uirect_fill ( wc , x1+1 , y1-1 , x2-1 , y2+1 , D->gc.dim ) ;
      uidraw_depre ( D , x1 , y1 , x2 , y2 ) ;
      menu = br->menu [ br->df-1 ] ;
//  uiMenustr(D,x1+4,y2+5,br->menu[br->df-1],D->gc.menu_char);
      uiString ( D , menu , x1 , br->y1+yoff , ln , br->width , D->gc.MenuFont , \
           D->gc.menu_char , D->gc.FontSize , -1 , -1 ) ;
      _uidown_dir ( D , x2+3 , y1 , 20 , 0.2 , 4 ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void ui_draw_browser ( DIW *w , int n , int lngth ) {
      DIALOG *D;
      kgWC *wc;
      char *menu;
      void *img;
      int x1 , y1 , x2 , y2 , yoff , ln , xoff = 0;
      int EVGAY;
      D = w->D;
      wc = WC ( D ) ;
      x1 = w->x1+D->xo;
      y1 = w->y1+D->yo;
      x2 = w->x2+D->xo;
      y2 = w->y2+D->yo;
      if ( w->Bimg == NULL ) w->Bimg = kgGetBackground ( D , x1 , y1 , x2 , y2 ) ;
      if ( w->Bimg != NULL ) kgRestoreImage ( D , w->Bimg , x1 , y1 , \
       ( x2-x1+1 ) , ( y2-y1+1 ) ) ;
      if ( n > 0 ) {
          uiString ( D , w->prompt , x1 , y1 , lngth , w->y2-w->y1 , D->gc.PromptFont , \
               D->gc.txt_pchar , D->gc.FontSize , 1 , D->gc.fill_clr ) ;
      }
      EVGAY = D->evgay;
      x1 = w->xb+D->xo+2;
      x2 = w->x2+D->xo-w->width-w->offset;
  //NEW
#if 0
      y1 = w->y1+D->yo+2;
      y2 = w->y2+D->yo-2;
      y1 = w->y1+D->yo;
      y2 = w->y2+D->yo;
      yoff = ( y2-y1-w->width ) *0.5;
      x1+= xoff;
      y1+= yoff;
      y2-= yoff;
#else 
      yoff = w->width*0.5-1;
      y1 = ( w->y1+w->y2 ) *0.5+D->yo-yoff;
      y2 = ( w->y1+w->y2 ) *0.5+D->yo+yoff;
#endif
      ln = x2-x1-5;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _dvrect_fill ( wc , x1+1 , y1+1 , x2-1 , y2-1 , D->gc.dim ) ;
      dvdraw_depre ( D , x1 , y1 , x2 , y2 ) ;
      menu = w->menu [ * ( w->df ) -1 ] ;
      img = w->imgs [ * ( w->df ) -1 ] ;
//  uiMenustr(D,x1+4,y2+5,br->menu[br->df-1],D->gc.menu_char);
#if 1
      uiString ( D , menu , x1+5 , y1-1 , ln , w->width-2 , D->gc.MenuFont , \
           D->gc.menu_char , D->gc.FontSize , -1 , -1 ) ;
//  kgImage(D,img,x1+5,y1,ln,w->width,0.0,1.0);
#endif
#if 0
      y1-= yoff;
      y2+= yoff;
      yoff = ( y2-y1-w->w ) *0.5;
      _dvdown_dir ( D , x2 , y1+yoff , w->w , 0.2 , 5 ) ;
      _dvdown_dir ( D , x2 , y1 , w->w , 0.2 , 5 ) ;
#else
      _dvdown_dir ( D , x2 , y1-3 , w->width+4 , 0.2 , 4 ) ;
#endif
      uiRest_clip_limits ( wc ) ;
  }
  void uiCleanEbrowserImages ( DIE *w ) {
      int i , items , lng;
      DIALOG *D;
      D = w->D;
      if ( w->item == -1 ) return;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          w->imgs = NULL;
      }
  }
  void uiMakeEbrowserImages_o ( DIE *w , int lng ) {
      int i , items;
      DIALOG *D;
      D = w->D;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          w->imgs = NULL;
      }
      i = 0;
      if ( w->menu != NULL ) {
          while ( ( w->menu [ i ] ) != NULL ) i++;
          items = i;
          w->imgs = ( void ** ) Malloc ( sizeof ( void * ) * ( i+1 ) ) ;
          w->imgs [ i ] = NULL;
          for ( i = 0; i < items; i++ ) {
              w->imgs [ i ] = ( void * ) kgStringToImage ( w->menu [ i ] , NULL , lng , \
                   w->width , D->gc.MenuFont , D->gc.menu_char , -1 , D->gc.FontSize , -1 ) ;
          }
      }
      return;
  }
  void uiMakeEbrowserImages ( DIE *w , int lng ) {
      int i , items;
      DIALOG *D;
      D = w->D;
      char Buf [ 1000 ] ;
      int fval , cval;
      float wfac , zfac;
      FONT_STR F;
      IMG_STR *IMG;
      int rd , gr , bl;
      int color;
      int Fz;
      char *Str;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          w->imgs = NULL;
      }
#if 0
      i = 0;
      if ( w->menu != NULL ) {
          while ( ( w->menu [ i ] ) != NULL ) i++;
          items = i;
          color = D->gc.menu_char;
          font = D->gc.MenuFont ;
          Fz = D->gc.FontSize;
          w->imgs = ( void ** ) malloc ( sizeof ( void * ) * ( i+1 ) ) ;
          w->imgs [ i ] = NULL;
          for ( i = 0; i < items; i++ ) {
              fval = font;
              cval = color;
              wfac = 1.0;
              Str = w->menu [ i ] ;
              Str = uiCleanOldString ( Str , Buf , & cval , & fval , & wfac , & zfac ) ;
//              w->imgs [ i ] = ( void * ) kgStringToImage ( w->menu [ i ] , NULL , lng , w->width , D->gc.MenuFont , D->gc.menu_char , -1 , D->gc.FontSize , -1 ) ;
              kgGetDefaultRGB ( cval , & rd , & gr , & bl ) ;
              F.code = 't';
              F.name = kgGetOthFont ( fval ) ;
              F.Imgs = Mimgs;
              if ( D->gc.FontSize <= 0 ) F.Size = ( w->width - 4 ) /2;
              else F.Size = D->gc.FontSize;
              if ( F.Size > ( w->width -4 ) /2 ) F.Size = ( w->width -4 ) /2;
              if ( fval != font ) F.code = 'f';
              if ( F.Size != Fz ) F.code = 'f';
              if ( wfac != 1.0 ) F.code = 'f';
              IMG = uiMakeString ( & ( F ) , Buf , ( int ) w->width , 0 ) ;
              kgSetImageColor ( IMG->img , rd , gr , bl ) ;
              w->imgs [ i ] = IMG->img;
              free ( F.name ) ;
              free ( IMG ) ;
          }
      }
#else
      w->imgs = ( void ** ) uiMenuStringImages ( D , w->menu , lng , w->width , \
           D->gc.MenuFont , D->gc.menu_char , D->gc.FontSize , -1 , 8 ) ;
#endif
      return;
  }
  void uiCleanYImages ( DIY *w ) {
      int i , items , lng;
      DIALOG *D;
      D = w->D;
      if ( w->item == -1 ) return;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          w->imgs = NULL;
      }
  }
  void uiMakeYImages ( DIY *w ) {
      int i , items , lng , width , FontSize;
      ThumbNail **menu;
      DIALOG *D;
      D = w->D;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          w->imgs = NULL;
      }
      i = 0;
      FontSize = D->gc.FontSize;
      lng = w->lngth+w->xgap;
      width = w->ygap-6;
      width = w->ygap;
      width = 2*FontSize+2;
      menu = ( ThumbNail ** ) w->list;
#if 0
      if ( menu != NULL ) {
          while ( ( menu [ i ] ) != NULL ) i++;
          items = i;
          w->imgs = ( void ** ) malloc ( sizeof ( void * ) * ( i+1 ) ) ;
          w->imgs [ i ] = NULL;
          for ( i = 0; i < items; i++ ) {
              w->imgs [ i ] = ( void * ) uiMenuStringImage ( D , menu [ i ]->name , lng , \
                   width , D->gc.MsgFont , D->gc.info_char , D->gc.FontSize , 0 ) ;
          }
      }
#else
      w->imgs = ( void ** ) uiMenuNailImages ( D , menu , lng , width , \
           D->gc.MenuFont , D->gc.twin_char , D->gc.FontSize , 0 , 8 ) ;
#endif
      return;
  }
  void uiCleanXImages ( DIX *w ) {
      int i , items , lng;
      DIALOG *D;
      D = w->D;
      if ( w->item == -1 ) return;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          kgFreeImage ( w->nimg ) ;
          kgFreeImage ( w->himg ) ;
          w->nimg = NULL;
          w->himg = NULL;
          w->imgs = NULL;
      }
  }
  void uiMakeXImages ( DIX *w , int lng ) {
      ThumbNail **menu;
      int i , items , TextQua , Mag;
      DIALOG *D;
      D = w->D;
      TextQua = ( w->type/10 ) ;
      Mag = 8;
      if ( TextQua ) Mag = 8;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          w->imgs = NULL;
          kgFreeImage ( w->nimg ) ;
          kgFreeImage ( w->himg ) ;
          w->nimg = NULL;
          w->himg = NULL;
      }
      i = 0;
      w->nimg = uiMakeXSymbol ( w , D->gc.twin_char , D->gc.FontSize , 0 ) ;
      w->himg = uiMakeXSymbol ( w , D->gc.twin_char , D->gc.FontSize , 1 ) ;
      menu = ( ThumbNail ** ) w->list;
      if ( menu != NULL ) {
#if 0
          while ( ( menu [ i ] ) != NULL ) i++;
          items = i;
          w->imgs = ( void ** ) malloc ( sizeof ( void * ) * ( i+1 ) ) ;
          w->imgs [ i ] = NULL;
          for ( i = 0; i < items; i++ ) {
              w->imgs [ i ] = ( void * ) uiMenuStringImage ( D , menu [ i ]->name , lng , \
                   w->width , D->gc.MenuFont , D->gc.menu_char , D->gc.FontSize , -1 ) ;
          }
#else
          w->imgs = ( void ** ) uiMenuNailImages ( D , menu , lng , w->width , \
               D->gc.MenuFont , D->gc.twin_char , D->gc.FontSize , -1 , Mag ) ;
#endif
      }
      return;
  }
  void uiCleanRImages ( DIRA *w ) {
      int i , items , lng;
      DIALOG *D;
      D = w->D;
      if ( w->item == -1 ) return;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          kgFreeImage ( w->nimg ) ;
          kgFreeImage ( w->himg ) ;
          w->nimg = NULL;
          w->himg = NULL;
          w->imgs = NULL;
      }
  }
  void uiMakeRImages ( DIRA *w , int lng ) {
      ThumbNail **menu;
      int i , items , th , BxSize;
      DIALOG *D;
      D = w->D;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          w->imgs = NULL;
          kgFreeImage ( w->nimg ) ;
          kgFreeImage ( w->himg ) ;
          w->nimg = NULL;
          w->himg = NULL;
      }
      th = w->width+w->ygap;
      i = 0;
      BxSize = 2*D->gc.FontSize;
      w->nimg = uiMakeRSymbol ( w , D->gc.fill_clr , D->gc.FontSize , 0 ) ;
      w->himg = uiMakeRSymbol ( w , D->gc.fill_clr , D->gc.FontSize , 1 ) ;
      menu = ( ThumbNail ** ) w->list;
      if ( menu != NULL ) {
#if 0
          while ( ( menu [ i ] ) != NULL ) i++;
          items = i;
          w->imgs = ( void ** ) malloc ( sizeof ( void * ) * ( i+1 ) ) ;
          w->imgs [ i ] = NULL;
          for ( i = 0; i < items; i++ ) {
              w->imgs [ i ] = ( void * ) uiMenuStringImage ( D , menu [ i ]->name , lng-2*BxSize , \
                   th , D->gc.Font , D->gc.menu_char , D->gc.FontSize , -1 ) ;
          }
#else
          w->imgs = ( void ** ) uiMenuNailImages ( D , menu , lng-2*BxSize , th , \
               D->gc.MenuFont , D->gc.menu_char , D->gc.FontSize , -1 , 8 ) ;
#endif
      }
      return;
  }
  void uiCleanCImages ( DICH *w ) {
      int i , items , lng;
      DIALOG *D;
      D = w->D;
      if ( w->item == -1 ) return;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          kgFreeImage ( w->nimg ) ;
          kgFreeImage ( w->himg ) ;
          w->nimg = NULL;
          w->himg = NULL;
          w->imgs = NULL;
      }
  }
  void uiMakeCImages ( DICH *w , int lng ) {
      ThumbNail **menu;
      int i , items , th , BxSize;
      DIALOG *D;
      D = w->D;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          w->imgs = NULL;
          kgFreeImage ( w->nimg ) ;
          kgFreeImage ( w->himg ) ;
          w->nimg = NULL;
          w->himg = NULL;
      }
      th = w->width+w->ygap;
      i = 0;
      BxSize = 2*D->gc.FontSize;
      w->nimg = uiMakeCSymbol ( w , D->gc.menu_char , D->gc.FontSize , 0 ) ;
      w->himg = uiMakeCSymbol ( w , D->gc.menu_char , D->gc.FontSize , 1 ) ;
      menu = ( ThumbNail ** ) w->list;
      if ( menu != NULL ) {
#if 0
          while ( ( menu [ i ] ) != NULL ) i++;
          items = i;
          w->imgs = ( void ** ) malloc ( sizeof ( void * ) * ( i+1 ) ) ;
          w->imgs [ i ] = NULL;
          for ( i = 0; i < items; i++ ) {
              w->imgs [ i ] = ( void * ) uiMenuStringImage ( D , menu [ i ]->name , lng-2*BxSize , \
                   th , D->gc.Font , D->gc.menu_char , D->gc.FontSize , -1 ) ;
          }
#else
          w->imgs = ( void ** ) uiMenuNailImages ( D , menu , lng-2*BxSize , th , \
               D->gc.MenuFont , D->gc.menu_char , D->gc.FontSize , -1 , 8 ) ;
#endif
      }
      return;
  }
  void uiCleanBrowserImages ( DIW *w ) {
      int i , items , lng;
      DIALOG *D;
      D = w->D;
      if ( w->item == -1 ) return;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          w->imgs = NULL;
      }
  }
  void uiMakeBrowserImages ( DIW *w ) {
      int i , items , lng;
      DIALOG *D;
      D = w->D;
      if ( w->imgs != NULL ) {
          i = 0;
          while ( w->imgs [ i ] != NULL ) kgFreeImage ( w->imgs [ i++ ] ) ;
          free ( w->imgs ) ;
          w->imgs = NULL;
      }
      lng = w->x2-w->width-w->offset - w->xb-2;
#if 0
      i = 0;
      if ( w->menu != NULL ) {
          while ( ( w->menu [ i ] ) != NULL ) i++;
          items = i;
          w->imgs = ( void ** ) malloc ( sizeof ( void * ) * ( i+1 ) ) ;
          w->imgs [ i ] = NULL;
          for ( i = 0; i < items; i++ ) {
              w->imgs [ i ] = ( void * ) kgStringToImage ( w->menu [ i ] , NULL , lng , \
                   w->width , D->gc.MenuFont , D->gc.menu_char , -1 , D->gc.FontSize , -1 ) ;
          }
      }
#else
      w->imgs = ( void ** ) uiMenuStringImages ( D , w->menu , lng , w->width , \
           D->gc.MenuFont , D->gc.menu_char , D->gc.FontSize , -1 , 8 ) ;
#endif
      return;
  }
  int _uiMake_W ( DIW *w ) {
      int xx2 , yy1 , x1 , y1 , ret = 1;
      char **menu;
      DIALOG *D;
      int pos , df;
      int i = 0 , n = 0;
      int lngth = 0. , lng = 0. , bwsize , items;
      int x2 , y2;
      BRW_STR *br;
      char **m;
      int EVGAY;
      kgWC *wc;
      D = w->D;
      wc = WC ( D ) ;
      x1 = D->xo;
      y1 = D->yo;
      br = w->bwsr;
      br->df = * ( w->df ) ;
      if ( w->hide != 1 ) {
          CHECKLIMITS ( w ) ;
          x1 = w->x1+D->xo;
          y1 = w->y1+D->yo;
          x2 = w->x2+D->xo;
          y2 = w->y2+D->yo;
          if ( w->Bimg == NULL ) w->Bimg = kgGetBackground ( D , x1 , y1 , x2 , y2 ) ;
          n = 0;
          i = 0;
          if ( w->prompt != NULL ) {
              while ( ( w->prompt [ n ] ) != '\0' ) n++;
          }
          lngth = 2;
          if ( n != 0 ) {n++; lngth = 8; }
          lngth += n*6;
          if ( lngth > ( ( w->x2-w->x1 ) *0.8+32 ) ) lngth = ( w->x2-w->x1 ) *0.8+32;
          w->xb = w->x1+lngth;
          i = 0;
          while ( ( w->menu [ i ] ) != NULL ) i++;
//   uiMakeBrowserImages(w);
          df = * ( w->df ) ;
          if ( df < 1 ) df = 1;
          if ( df > i ) df = i;
          * ( w->df ) = df;
#if 0
          if ( n > 0 ) {
              uiString ( D , w->prompt , x1 , y1 , lngth , w->y2-w->y1 , D->gc.PromptFont , \
                   D->gc.txt_pchar , D->gc.FontSize , 1 , D->gc.fill_clr ) ;
          }
#endif
          ui_draw_browser ( w , n , lngth ) ;
  // _uiMake_Wbwsr(w);
      }
      else {
          DIALOG *D;
          kgWC *wc;
          int x1 , y1 , x2 , y2;
          D = w->D;
          wc = WC ( D ) ;
          ret = 0;
          x1 = w->x1+D->xo;
          y1 = w->y1+D->yo;
          x2 = w->x2+D->xo;
          y2 = w->y2+D->yo;
#if 0
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( x1 ) , ( y1 ) , ( x2 ) , \
           ( y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          if ( w->Bimg != NULL ) kgRestoreImage ( D , w->Bimg , x1 , y1 , \
           ( x2-x1+1 ) , ( y2-y1+1 ) ) ;
      }
      return ret;
  }
  int Make_browser ( DIW *w ) {
      BRW_STR *br;
      DIALOG *D;
      int x1 , y1;
      int ret , n = 0 , lngth = 0 , i = 0;
      BRW_STR *bwsr;
      D = w->D;
      x1 = D->xo;
      y1 = D->yo;
      bwsr = ( BRW_STR * ) ( w->bwsr ) ;
      w->bwsr = bwsr;
      bwsr->D = D;
      return ( ret ) ;
  }
  void * make_menu_str ( DIALOG *D , int xx1 , int yy1 , \
       int pos , int df , char **menu , int size ) \
  {
      int i = 0 , ret , n = 0;
      int lngth = 0. , lng = 0.;
      int x1 , y1 , x2 , y2;
      float length;
      BRW_STR *br;
      char **m;
      kgWC *wc;
      while ( menu [ n ] != NULL ) n++;
      if ( df > n ) df = n;
      if ( pos >= n ) pos = n-1;
      if ( pos < 0 ) pos = 0;
      br = ( BRW_STR * ) Malloc ( sizeof ( BRW_STR ) ) ;
      br->D = D;
      br->x1 = xx1;
      br->y1 = yy1;
      br->pos = pos;
      br->df = df;
      br->menu = menu;
      br->size = size;
      br->D = ( DIALOG * ) D;
      wc = WC ( D ) ;
      m = br->menu;
      while ( m [ i ] != NULL ) {
          length = uiStringLength ( m [ i ] , D->gc.FontSize ) ;
//    lng = strlen(m[i]);
          lng = ( int ) ( length ) ;
          if ( lngth < lng ) lngth = lng;
          i++;
      }
      x1 = br->x1;
      y1 = br->D->evgay-br->y1+1;
//   x2 = x1 +lngth*9 +10;
      x2 = x1 +lngth +10;
      y2 = y1-20;
      br->xb = x2+2;
      br->width = 20;
      br->x2 = br->xb;
      _uiDrawMenu ( br ) ;
      return ( void * ) br;
  }
//void * make_dialog_menu_str(int xx1,int yy1,int pos,int df,char **menu,int size)
  void * make_dialog_menu_str ( DIALOG *D , DIE *w , \
       int pos , int df , char **menu , int size ) \
  {
      int i = 0 , ret , n = 0;
      int lngth = 0. , lng = 0.;
      int x1 , y1 , x2 , y2;
      BRW_STR *br;
      char **m;
      while ( menu [ n ] != NULL ) n++;
      if ( df > n ) df = n;
      if ( pos >= n ) pos = n-1;
      if ( pos < 0 ) pos = 0;
      br = ( BRW_STR * ) Malloc ( sizeof ( BRW_STR ) ) ;
      br->D = D;
      br->x1 = w->x1;
      br->y1 = w->y1;
      br->x2 = w->x2;
      br->y2 = w->y2;
      br->pos = pos;
      br->df = df;
      br->menu = menu;
      br->size = size;
      br->xb = br->x2;
      br->width = 20;
      return ( void * ) br;
  }
  void * make_dialog_newmenu_str ( DIALOG *D , DIX *x , \
       int pos , int df , char **menu , int size ) \
  {
      int i = 0 , ret , n = 0;
      int lngth = 0. , lng = 0.;
      int x1 , y1 , x2 , y2;
      BRW_STR *br;
      char **m;
      while ( menu [ n ] != NULL ) n++;
      if ( x->nx < 1 ) x->nx = 1;
      x->ny = ( n-x->nx+1 ) / ( x->nx ) ;
      if ( df > n ) df = n;
      if ( pos >= n ) pos = n-1;
      if ( pos < 0 ) pos = 0;
      br = ( BRW_STR * ) Malloc ( sizeof ( BRW_STR ) ) ;
      br->D = D;
      br->x1 = x->x1;
      br->y1 = x->y1;
      br->x2 = x->x2;
      br->y2 = x->y2;
      br->pos = pos;
      br->df = df;
      br->menu = menu;
      br->size = size;
      br->xb = br->x2;
      br->width = x->width+x->ygap;
      return ( void * ) br;
  }
  void * make_dialog_y_str ( DIALOG *D , DIY *y , int pos , \
       int df , char **menu , int size ) \
  {
      int i = 0 , ret , n = 0;
      int lngth = 0. , lng = 0.;
      int x1 , y1 , x2 , y2;
      BRW_STR *br;
      char **m;
      if ( menu != NULL ) while ( menu [ n ] != NULL ) n++;
      y->nitems = n;
      if ( y->nx < 1 ) y->nx = 1;
      y->ny = ( n ) / ( y->nx ) ;
      if ( ( n%y->nx ) != 0 ) y->ny+= 1;
      if ( df < 1 ) df = 1;
      if ( df > n ) df = n;
      if ( pos >= y->nitems ) pos = y->nitems-1;
      if ( pos < 0 ) pos = 0;
      br = ( BRW_STR * ) Malloc ( sizeof ( BRW_STR ) ) ;
      br->D = D;
      br->x1 = y->x1;
      br->y1 = y->y1;
      br->x2 = y->x2;
      br->y2 = y->y2;
      br->pos = pos;
      br->df = df;
      br->menu = menu;
      br->size = y->ny;
      br->xb = br->x2;
      br->width = y->width+y->ygap;
      y->size = ( y->y2-y->y1-2*y->offset ) /br->width;
      br->w = y->w;
      br->offset = y->offset;
      return ( void * ) br;
  }
  void * make_msg_str ( DIS *w ) {
      DIALOG *D;
      int pos , df , size;
      char **menu;
      int i = 0 , ret , n = 0;
      int lngth = 0. , lng = 0.;
      int x1 , y1 , x2 , y2;
      BRW_STR *br;
      int EVGAY;
      char **m;
      D = w->D;
      EVGAY = D->evgay;
      menu = w->menu;
      df = pos = 1;
      size = w->size;
      if ( menu != NULL ) while ( menu [ n ] != NULL ) n++;
      else n = 1;
      if ( df > n ) df = n;
      if ( df < 1 ) df = 1;
      if ( pos >= n ) pos = n-1;
      if ( pos < 0 ) pos = 0;
      if ( size > n ) size = n;
      br = w->bwsr;
      br->gc = D->gc;
      br->x1 = w->x1+D->xo;
      br->y1 = w->y1+D->yo;
      br->x2 = w->x2+D->xo;
      br->y2 = w->y2+D->yo;
      br->pos = pos;
      br->df = df;
      br->hitem = df;
      br->menu = menu;
      br->size = size;
      br->xb = br->x2;
      br->width = w->w;
      return ( void * ) br;
  }
  int uiclean_gui_area ( DIALOG *D , int i , int x1 , int y1 , DIA *d ) {
      int ret = 0 , ch;
      int X1 , Y1 , X2 , Y2;
      ch = ( d [ i ] .t->code ) ;
      switch ( ch ) {
          case 't':
          X1 = d [ i ] .t->x1+x1;
          Y1 = d [ i ] .t->y1+y1;
          X2 = d [ i ] .t->x2+x1;
          Y2 = d [ i ] .t->y2+y1;
          break;
          case 'T':
          X1 = d [ i ] .t->x1+x1;
          Y1 = d [ i ] .t->y1+y1;
          X2 = d [ i ] .t->x2+x1;
          Y2 = d [ i ] .t->y2+y1;
          break;
          case 'x':
          case 'y':
          X1 = d [ i ] .x->x1+x1;
          Y1 = d [ i ] .x->y1+y1;
          X2 = d [ i ] .x->x2+x1;
          Y2 = d [ i ] .x->y2+y1;
          break;
          case 'h':
          case 'H':
          X1 = d [ i ] .h->x1+x1;
          Y1 = d [ i ] .h->y1+y1;
          X2 = d [ i ] .h->x2+x1;
          Y2 = d [ i ] .h->y2+y1;
          break;
          case 'b':
          case 'n':
          case 'N':
          X1 = d [ i ] .b->x1+x1;
          Y1 = d [ i ] .b->y1+y1;
          X2 = d [ i ] .b->x2+x1;
          Y2 = d [ i ] .b->y2+y1;
          break;
          case 'f':
          X1 = d [ i ] .f->x1+x1;
          Y1 = d [ i ] .f->y1+y1;
          X2 = d [ i ] .f->x2+x1;
          Y2 = d [ i ] .f->y2+y1;
          break;
          case 'd':
          X1 = d [ i ] .d->x1+x1;
          Y1 = d [ i ] .d->y1+y1;
          X2 = d [ i ] .d->x2+x1;
          Y2 = d [ i ] .d->y2+y1;
          break;
          case 'P':
          X1 = d [ i ] .B->x1+x1;
          Y1 = d [ i ] .B->y1+y1;
          X2 = d [ i ] .B->x2+x1;
          Y2 = d [ i ] .B->y2+y1;
          break;
          case 'w':
          case 'e':
          case 's':
          X1 = d [ i ] .w->x1+x1;
          Y1 = d [ i ] .w->y1+y1;
          X2 = d [ i ] .w->x2+x1;
          Y2 = d [ i ] .w->y2+y1;
          break;
          default:
          return -1;
      }
//   _uiclean_gui_area(D,X1,Y1,X2,Y2);
      _uirect_fill_transparent ( WC ( D ) , X1 , D->evgay-Y1 , X2 , \
           D->evgay-Y2 , D->gc.fill_clr , D->transparency ) ;
      return -1;
  }
  void _ui_updatemenu ( void *tmp , int item , char **m ) {
      int i;
      char **menu;
      DIALOG *D;
      DIA *d; DIE *E;
      BRW_STR *br;
      kgWC *wc;
      D = ( DIALOG * ) tmp;
      wc = WC ( D ) ;
      d = D->d;
      E = d [ item ] .e;
      br = ( BRW_STR * ) ( E->bwsr ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      menu = br->menu;
      if ( m != NULL ) {
          i = 0;
          if ( menu != NULL ) {
              while ( menu [ i ] != NULL ) {free ( menu [ i ] ) ; i++; }
              free ( menu ) ;
          }
          br->menu = m;
          br->size = E->size;
      }
      menu = br->menu;
      uiCleanEbrowserImages ( E ) ;
      E->menu = menu;
      uiclean_gui_area ( D , item , D->xo , D->yo , D->d ) ;
      _uiDrawDialogMenu ( br ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void ui_updatemenu ( void *tmp , int item , char **m ) {
      int i;
      char **menu;
      DIALOG *D;
      DIA *d; DIE *E;
      BRW_STR *br;
      kgWC *wc;
      D = ( DIALOG * ) tmp;
      wc = WC ( D ) ;
      d = D->d;
      E = d [ item ] .e;
      br = ( BRW_STR * ) ( E->bwsr ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiCleanEbrowserImages ( E ) ;
      E->menu = m;
      br->menu = m;
      br->size = E->size;
      uiclean_gui_area ( D , item , D->xo , D->yo , D->d ) ;
      _uiDrawDialogMenu ( br ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void uiUpdatemenu ( void *tmp , int item ) {
      int i;
      char **menu;
      DIALOG *D;
      DIA *d; DIE *E;
      BRW_STR *br;
      kgWC *wc;
      D = ( DIALOG * ) tmp;
      wc = WC ( D ) ;
      d = D->d;
      E = d [ item ] .e;
      br = ( BRW_STR * ) ( E->bwsr ) ;
      br->size = E->size;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      uiclean_gui_area ( D , item , D->xo , D->yo , D->d ) ;
      _uiDrawDialogMenu ( br ) ;
      uiRest_clip_limits ( wc ) ;
  }
  int Make_menu ( DIALOG *D , DIE *w , int x1 , int y1 ) {
      BRW_STR *bwsr;
      int ret , n = 0 , lngth = 0 , i = 0;
      kgWC *wc;
      wc = WC ( D ) ;
      if ( n != 0 ) {n++; lngth = 8; }
      lngth += n*9;
      w->bwsr = make_dialog_menu_str ( D , w , * ( w->df ) , * \
       ( w->df ) , w->menu , w->size ) ;
      bwsr = ( BRW_STR * ) w->bwsr;
      bwsr->D = ( DIALOG* ) D;
      bwsr->x1 += x1;
      bwsr->y1 += y1;
      bwsr->x2 += x1;
      bwsr->y2 += y1;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _uiDrawDialogMenu ( w->bwsr ) ;
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  void uiUpdateWidget ( void *widget ) {
      kgUpdateWidget ( widget ) ;
  }
  void kgUpdateWidget ( void *widget ) {
      DIX *x;
      x = ( DIX * ) widget;
      switch ( x->code ) {
          case 'g':
          _uiMake_G ( ( DIG * ) x ) ;
          break;
          case 'P':
          _uiMake_SH ( ( DIHB * ) x ) ;
          break;
          case 's':
          _uiMake_MS ( ( DIS * ) x ) ;
          break;
          case 'i':
          _uiMake_I ( ( DII * ) x ) ;
          break;
          case 'b':
          case 'N':
          _uiMake_B ( ( DIBN * ) x ) ;
          break;
          case 'n':
          _uiMake_N ( ( DIB * ) x ) ;
          break;
          case 'h':
          _uiMake_N ( ( DIB * ) x ) ;
          break;
          case 'H':
          _uiMake_H ( ( DILN * ) x ) ;
          break;
          case 'm':
          case 'M':
          case 'B':
          _uiMake_M ( ( DIM * ) x ) ;
          break;
          case 't':
          _uiMake_Tx ( ( DIT * ) x ) ;
          break;
          case 'T':
          _uiMake_Ta ( ( DIT * ) x ) ;
          break;
          case 'v':
          _uiMake_V ( ( DIV * ) x ) ;
          break;
          case 'z':
          _uiMake_Z ( ( DIZ * ) x ) ;
          break;
          case 'x':
          uiCleanXImages ( x ) ;
          _uiMake_X ( x ) ;
          break;
          case 'y':
//     uiCleanYImages((DIY *)x);
          _uiMake_Y ( ( DIY * ) x ) ;
          break;
          case 'r':
//     uiCleanRImages((DIRA*)x);
          _uiMake_RadioButton ( ( DIRA * ) x ) ;
          break;
          case 'c':
//     uiCleanCImages((DICH*)x);
          _uiMake_CheckBox ( ( DICH * ) x ) ;
          break;
          case 'o':
          _uiMake_O ( ( DIO * ) x ) ;
          break;
          case 'p':
          _uiMake_P ( ( DIP * ) x ) ;
          break;
          case 'e':
//     uiCleanEbrowserImages((DIE *)x);
          _uiMake_E ( ( DIE * ) x ) ;
          break;
          case 'w':
//     uiCleanBrowserImages((DIW *)x);
          _uiMake_W ( ( DIW * ) x ) ;
          break;
          case 'd':
          _uiMake_SD ( ( DID * ) x ) ;
          break;
          case 'f':
          _uiMake_SF ( ( DIF * ) x ) ;
          break;
          default:
          break;
      }
  }
  int _uiMake_Y ( DIY *y ) {
      BRW_STR *bwsr;
      int ret , n = 0 , i = 0 , x1 , y1 , df;
      int height , extra , w , xshift , size;
      ThumbNail **menu;
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) ( y->D ) ;
      wc = WC ( D ) ;
      x1 = D->xo;
      y1 = D->yo;
      menu = ( ThumbNail ** ) y->list;
      if ( menu != NULL ) while ( menu [ n ] != NULL ) {
          if ( ( ( y->type ) %10 ) != 0 ) menu [ n ]->sw = 0;
          n++;
      }
      y->nitems = n;
      if ( * ( y->df ) < 1 ) * ( y->df ) = 0;
      if ( * ( y->df ) > n ) * ( y->df ) = n;
      df = * ( y->df ) -1;
      if ( ( ( ( y->type ) %10 ) != 0 ) && ( menu != NULL ) &&  \
          ( menu [ 0 ] != NULL ) && \
       ( df >= 0 ) ) menu [ df ]->sw = 1;
      n = 0;
#if 0
      y->nx = ( y->x2 -y->x1-2*y->offset-y->w ) / ( y->width+y->xgap ) ;
      if ( y->nx < 1 ) {
          y->width = ( y->x2 -y->x1-2*y->offset-y->w ) -y->xgap;
          y->nx = 1;
      }
#else
      y->nx = ( y->x2 -y->x1-2*y->offset-y->w ) / ( y->lngth+y->xgap ) ;
      if ( y->nx < 1 ) {
          y->lngth = ( y->x2 -y->x1-2*y->offset-y->w ) -y->xgap;
          y->nx = 1;
      }
#endif
      xshift = ( y->x2 -y->x1-2*y->offset-y->w ) - ( y->lngth+y->xgap ) *y->nx;
      xshift = xshift/2;
      if ( xshift < 0 ) xshift = 0;
      bwsr = ( BRW_STR * ) y->bwsr;
      bwsr->offset = y->offset;
      bwsr->xshift = xshift;
      bwsr->D = ( DIALOG* ) D;
      bwsr->x1 = y->x1+x1;
      bwsr->y1 = y->y1+y1;
      bwsr->x2 = y->x2+x1;
      bwsr->y2 = y->y2+y1;
      if ( y->hide != 1 ) {
          CHECKLIMITS ( y ) ;
          if ( y->Bimg == NULL ) {
              y->Bimg = kgGetBackground ( D , bwsr->x1 , bwsr->y1 , bwsr->x2 , bwsr->y2 ) ;
          }
          bwsr->df = * ( y->df ) ;
          bwsr->size = y->ny;
          bwsr->xb = bwsr->x2;
          height = y->width+y->ygap;
          bwsr->width = height;
          y->size = ( y->y2-y->y1-2*y->offset ) /bwsr->width;
          bwsr->w = y->w;
          bwsr->offset = y->offset;
          if ( y->size <= 0 ) {
              y->size = 1;
              y->y2 = height+y->y1+2*y->offset;
          }
          extra = ( y->y2 -y->y1-2*y->offset-y->size*height ) /2;
          if ( extra < -y->offset ) extra = -y->offset;
          bwsr->offset = y->offset+extra;
          if ( y->list != NULL ) while ( y->list [ n ] != NULL ) n++;
          y->nitems = n;
          y->ny = ( n/y->nx ) ;
          if ( ( n%y->nx ) != 0 ) y->ny+= 1;
          if ( y->size > y->ny ) y->size = y->ny;
          bwsr->scroll = 1;
          if ( y->size == y->ny ) {
              bwsr->scroll = 0;
	 //TCB
              bwsr->pos = 0;
          }
          bwsr->size = y->size;
          if ( bwsr->df > n ) bwsr->df = 1;
          if ( bwsr->df < 1 ) bwsr->df = 1;
          * ( y->df ) = bwsr->df;
          bwsr->menu = ( char ** ) y->list;
//   bwsr->pos=0;
          size = y->nx*y->size;
          bwsr->D = D;
          bwsr->hitem = bwsr->df-1;
          if ( ( bwsr->hitem - bwsr->pos ) < 0 ) bwsr->pos = 0;
          if ( ( bwsr->hitem - bwsr->pos ) > size ) bwsr->pos = bwsr->hitem/y->nx;
          bwsr->w = bwsr->scroll*y->w;
          bwsr->tbx1 = bwsr->x2 - y->offset -bwsr->w ;
          bwsr->tby1 = bwsr->y1+y->offset;
          bwsr->tbx2 = bwsr->tbx1+bwsr->w;
          bwsr->tby2 = bwsr->tby1+bwsr->w;
          bwsr->bbx1 = bwsr->tbx1;
          bwsr->bby1 = bwsr->y2-y->offset-bwsr->w;
          bwsr->bbx2 = bwsr->bbx1+bwsr->w;
          bwsr->bby2 = bwsr->bby1+bwsr->w;
          bwsr->rsx1 = bwsr->tbx1;
          bwsr->rsy1 = bwsr->tby2;
          bwsr->rsx2 = bwsr->bbx2;
          bwsr->rsy2 = bwsr->bby1;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _uiDrawDialogY ( y ) ;
          uiRest_clip_limits ( wc ) ;
      }
      else {
          ret = 0;
#if 0
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( bwsr->x1 ) , ( bwsr->y1 ) , ( bwsr->x2 ) , \
           ( bwsr->y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , bwsr->x1 , bwsr->y1 , \
           ( bwsr->x2-bwsr->x1+1 ) , ( bwsr->y2-bwsr->y1+1 ) ) ;
      }
      return ( ret ) ;
  }
  int _uiMake_RadioButton ( DIRA *y ) {
      ThumbNail **menu;
      int *sw;
      BRW_STR *bwsr;
      int ret , n = 0 , i = 0 , x1 , y1;
      int height , extra , w;
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) ( y->D ) ;
      wc = WC ( D ) ;
      x1 = D->xo;
      y1 = D->yo;
      menu = ( ThumbNail ** ) y->list;
      if ( menu != NULL ) while ( menu [ n ] != NULL ) n++;
      y->nitems = n;
      if ( * ( y->df ) < 1 ) * ( y->df ) = 1;
      if ( * ( y->df ) > n ) * ( y->df ) = n;
      for ( i = 0; i < n; i++ ) menu [ i ]->sw = 0;
      menu [ * ( y->df ) -1 ]->sw = 1;
//   y->nx = 1;
      bwsr = ( BRW_STR * ) y->bwsr;
      bwsr->D = ( DIALOG* ) D;
      bwsr->x1 = y->x1+x1;
      bwsr->y1 = y->y1+y1;
      bwsr->x2 = y->x2+x1;
      bwsr->y2 = y->y2+y1;
      if ( y->hide != 1 ) {
          CHECKLIMITS ( y ) ;
          if ( y->Bimg == NULL ) y->Bimg = kgGetBackground ( D , \
               bwsr->x1 , bwsr->y1 , bwsr->x2 , bwsr->y2 ) ;
          bwsr->hitem = 0;
          bwsr->pos = 0;
          bwsr->df = * ( y->df ) ;
          bwsr->size = y->ny;
          bwsr->xb = bwsr->x2;
          bwsr->width = y->width+y->ygap;
          bwsr->offset = y->offset;
          height = y->width+y->ygap;
          y->size = ( y->y2 -y->y1-2*y->offset ) /height;
          if ( y->size <= 0 ) {
              y->size = 1;
              y->y2 = height+y->y1+2*y->offset;
//     y->width=(y->y2 -y->y1-2*y->offset)/2;
//     y->ygap=0;
          }
          extra = ( y->y2 -y->y1-2*y->offset-y->size*height ) /2;
          if ( extra < -y->offset ) extra = -y->offset;
          bwsr->offset = y->offset+extra;
          if ( menu != NULL ) while ( menu [ n ] != NULL ) n++;
          y->nitems = n;
          y->ny = ( n/y->nx ) ;
          if ( ( n%y->nx ) != 0 ) y->ny+= 1;
          if ( y->size > y->ny ) y->size = y->ny;
          bwsr->scroll = 1;
          if ( y->size == y->ny ) {
              bwsr->scroll = 0;
          }
          bwsr->menu = ( char ** ) y->list;
          bwsr->size = y->size;
          if ( bwsr->df > n ) bwsr->df = 1;
          if ( bwsr->df < 1 ) bwsr->df = 1;
          * ( y->df ) = bwsr->df;
          bwsr->pos = 0;
          bwsr->D = D;
          bwsr->hitem = bwsr->df-1;
          w = bwsr->scroll*y->w;
          bwsr->w = w;
          bwsr->tbx1 = bwsr->x2 - y->offset -bwsr->w;
          bwsr->tby1 = bwsr->y1+y->offset;
          bwsr->tbx2 = bwsr->tbx1+bwsr->w;
          bwsr->tby2 = bwsr->tby1+bwsr->w;
          bwsr->bbx1 = bwsr->tbx1;
          bwsr->bby1 = bwsr->y2-y->offset-bwsr->w;
          bwsr->bbx2 = bwsr->bbx1+bwsr->w;
          bwsr->bby2 = bwsr->bby1+bwsr->w;
          bwsr->rsx1 = bwsr->tbx1;
          bwsr->rsy1 = bwsr->tby2;
          bwsr->rsx2 = bwsr->bbx2;
          bwsr->rsy2 = bwsr->bby1;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _uiDrawDialogRadioButton ( y ) ;
          uiRest_clip_limits ( wc ) ;
      }
      else {
          ret = 0;
#if 0
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( bwsr->x1 ) , ( bwsr->y1 ) , ( bwsr->x2 ) , \
           ( bwsr->y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , bwsr->x1 , bwsr->y1 , \
           ( bwsr->x2-bwsr->x1+1 ) , ( bwsr->y2-bwsr->y1+1 ) ) ;
      }
      return ( ret ) ;
  }
  int _uiMake_CheckBox ( DICH *y ) {
      ThumbNail **menu;
      BRW_STR *bwsr;
      int ret , n = 0 , i = 0 , x1 , y1;
      int height , extra , w;
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) ( y->D ) ;
      wc = WC ( D ) ;
      x1 = D->xo;
      y1 = D->yo;
      menu = ( ThumbNail ** ) y->list;
      if ( menu != NULL ) while ( menu [ n ] != NULL ) n++;
      y->nitems = n;
      if ( * ( y->df ) < 1 ) * ( y->df ) = 1;
      if ( * ( y->df ) > n ) * ( y->df ) = n;
//   y->nx = 1;
      bwsr = ( BRW_STR * ) y->bwsr;
      bwsr->D = ( DIALOG* ) D;
      bwsr->x1 = y->x1+x1;
      bwsr->y1 = y->y1+y1;
      bwsr->x2 = y->x2+x1;
      bwsr->y2 = y->y2+y1;
      if ( y->hide != 1 ) {
          CHECKLIMITS ( y ) ;
          if ( y->Bimg == NULL ) y->Bimg = kgGetBackground ( D , \
               bwsr->x1 , bwsr->y1 , bwsr->x2 , bwsr->y2 ) ;
          bwsr->hitem = 0;
          bwsr->pos = 0;
          bwsr->df = * ( y->df ) ;
          bwsr->size = y->ny;
          bwsr->xb = bwsr->x2;
          bwsr->width = y->width+y->ygap;
          bwsr->offset = y->offset;
          height = y->width+y->ygap;
          y->size = ( y->y2 -y->y1-2*y->offset ) /height;
          if ( y->size <= 0 ) {
              y->size = 1;
              y->y2 = height+y->y1+2*y->offset;
          }
          extra = ( y->y2 -y->y1-2*y->offset-y->size*height ) /2;
          if ( extra < -y->offset ) extra = -y->offset;
          bwsr->offset = y->offset+extra;
          y->ny = ( n/y->nx ) ;
          if ( ( n%y->nx ) != 0 ) y->ny+= 1;
          if ( y->size > y->ny ) y->size = y->ny;
          bwsr->scroll = 1;
          if ( y->size == y->ny ) {
              bwsr->scroll = 0;
          }
          bwsr->menu = ( char ** ) y->list;
          bwsr->size = y->size;
          if ( bwsr->df > n ) bwsr->df = 1;
          if ( bwsr->df < 1 ) bwsr->df = 1;
          * ( y->df ) = bwsr->df;
          bwsr->pos = 0;
          bwsr->D = D;
          bwsr->hitem = bwsr->df-1;
          w = bwsr->scroll*y->w;
          bwsr->w = w;
          bwsr->tbx1 = bwsr->x2 - y->offset -bwsr->w;
          bwsr->tby1 = bwsr->y1+y->offset;
          bwsr->tbx2 = bwsr->tbx1+bwsr->w;
          bwsr->tby2 = bwsr->tby1+bwsr->w;
          bwsr->bbx1 = bwsr->tbx1;
          bwsr->bby1 = bwsr->y2-y->offset-bwsr->w;
          bwsr->bbx2 = bwsr->bbx1+bwsr->w;
          bwsr->bby2 = bwsr->bby1+bwsr->w;
          bwsr->rsx1 = bwsr->tbx1;
          bwsr->rsy1 = bwsr->tby2;
          bwsr->rsx2 = bwsr->bbx2;
          bwsr->rsy2 = bwsr->bby1;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _uiDrawDialogCheckBox ( y ) ;
          uiRest_clip_limits ( wc ) ;
      }
      else {
          ret = 0;
#if 0
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( bwsr->x1 ) , ( bwsr->y1 ) , ( bwsr->x2 ) , \
           ( bwsr->y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , bwsr->x1 , bwsr->y1 , \
           ( bwsr->x2-bwsr->x1+1 ) , ( bwsr->y2-bwsr->y1+1 ) ) ;
      }
      return ( ret ) ;
  }
  int _uiMake_X ( DIX *y ) {
      ThumbNail **menu;
      BRW_STR *bwsr;
      int ret , n = 0 , i = 0 , x1 , y1 , df;
      int height , extra , w;
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) ( y->D ) ;
      wc = WC ( D ) ;
      x1 = D->xo;
      y1 = D->yo;
      menu = ( ThumbNail ** ) y->list;
      n = 0;
      if ( menu != NULL ) while ( menu [ n ] != NULL ) {
          if ( ( ( y->type ) %10 ) != 0 ) menu [ n ]->sw = 0;
          n++;
      }
      y->nitems = n;
      if ( * ( y->df ) < 1 ) * ( y->df ) = 0;
      if ( * ( y->df ) > n ) * ( y->df ) = n;
//   y->nx = 1;
      df = * ( y->df ) -1;
      if ( ( ( ( y->type ) %10 ) != 0 ) && ( menu != NULL ) &&  \
          ( menu [ 0 ] != NULL ) && \
       ( df >= 0 ) ) menu [ df ]->sw = 1;
      bwsr = ( BRW_STR * ) y->bwsr;
      bwsr->D = ( DIALOG* ) D;
      bwsr->x1 = y->x1+x1;
      bwsr->y1 = y->y1+y1;
      bwsr->x2 = y->x2+x1;
      bwsr->y2 = y->y2+y1;
      if ( y->bkgr == 1 ) _dvrect_fill ( WC ( D ) , bwsr->x1+4 , bwsr->y1+4 , \
           bwsr->x2-4 , bwsr->y2-4 , D->gc.twin_fill ) ;
//      else _dvrect_fill ( WC ( D ) , bwsr->x1+4 , bwsr->y1+4 , bwsr->x2-4 , bwsr->y2-4 , D->gc.fill_clr ) ;
      if ( y->hide != 1 ) {
          CHECKLIMITS ( y ) ;
          if ( y->Bimg == NULL ) y->Bimg = kgGetBackground ( D , \
               bwsr->x1 , bwsr->y1 , bwsr->x2 , bwsr->y2 ) ;
          bwsr->hitem = 0;
          bwsr->pos = 0;
          bwsr->df = * ( y->df ) ;
          bwsr->size = y->ny;
          bwsr->xb = bwsr->x2;
          height = y->width+y->ygap;
          bwsr->width = height;
          y->size = ( y->y2-y->y1-2*y->offset ) /bwsr->width;
          bwsr->w = y->w;
          bwsr->offset = y->offset;
          if ( y->size <= 0 ) {
              y->size = 1;
              y->y2 = height+y->y1+2*y->offset;
//     y->width=(y->y2 -y->y1-2*y->offset)/2;
//     y->ygap=0;
          }
          extra = ( y->y2 -y->y1-2*y->offset-y->size*height ) /2;
          if ( extra < -y->offset ) extra = -y->offset;
          bwsr->offset = y->offset+extra;
          y->ny = ( n/y->nx ) ;
          if ( ( n%y->nx ) != 0 ) y->ny+= 1;
          if ( y->size > y->ny ) y->size = y->ny;
          bwsr->scroll = 1;
          bwsr->menu = ( char ** ) menu;
          if ( y->size == y->ny ) {
              bwsr->scroll = 0;
          }
          bwsr->size = y->size;
          if ( bwsr->df > n ) bwsr->df = 1;
          if ( bwsr->df < 1 ) bwsr->df = 1;
          * ( y->df ) = bwsr->df;
          bwsr->pos = 0;
          bwsr->D = D;
          bwsr->hitem = bwsr->df-1;
          w = bwsr->scroll*y->w;
          bwsr->w = w;
          bwsr->tbx1 = bwsr->x2 - y->offset -w;
          bwsr->tby1 = bwsr->y1+y->offset;
          bwsr->tbx2 = bwsr->tbx1+w;
          bwsr->tby2 = bwsr->tby1+w;
          bwsr->bbx1 = bwsr->tbx1;
          bwsr->bby1 = bwsr->y2-y->offset-w;
          bwsr->bbx2 = bwsr->bbx1+w;
          bwsr->bby2 = bwsr->bby1+w;
          bwsr->rsx1 = bwsr->tbx1;
          bwsr->rsy1 = bwsr->tby2;
          bwsr->rsx2 = bwsr->bbx2;
          bwsr->rsy2 = bwsr->bby1;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _uiDrawDialogX ( y ) ;
          uiRest_clip_limits ( wc ) ;
      }
      else {
          ret = 0;
#if 0
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( bwsr->x1 ) , ( bwsr->y1 ) , ( bwsr->x2 ) , \
           ( bwsr->y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , bwsr->x1 , bwsr->y1 , \
           ( bwsr->x2-bwsr->x1+1 ) , ( bwsr->y2-bwsr->y1+1 ) ) ;
      }
      return ( ret ) ;
  }
  int _uiMake_V ( DIV *y ) {
      int ret , n = 0 , i = 0 , x1 , y1;
      int height , extra , w , df;
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) ( y->D ) ;
      wc = WC ( D ) ;
      x1 = D->xo;
      y1 = D->yo;
      if ( y->hide != 1 ) {
          CHECKLIMITS ( y ) ;
          BACKUPWIDGETAREA ( y ) ;
          Make_v_bar ( y ) ;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _uiDrawDialogV ( y ) ;
          uiRest_clip_limits ( wc ) ;
      }
      else {
#if 0
          ret = 0;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( y->x1+x1 ) , ( y->y1+y1 ) , ( y->x2+x1 ) , \
           ( y->y2+y1 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#else
          RESTOREWIDGETAREA ( y ) ;
#endif
      }
      return ( ret ) ;
  }
  int _uiMake_Z ( DIZ *y ) {
      int ret , n = 0 , i = 0 , x1 , y1;
      int height , extra , w , df;
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) ( y->D ) ;
      wc = WC ( D ) ;
      x1 = D->xo;
      y1 = D->yo;
      if ( y->hide != 1 ) {
          CHECKLIMITS ( y ) ;
          BACKUPWIDGETAREA ( y ) ;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          Make_z_bar ( y ) ;
          _uiDrawDialogZ ( y ) ;
          uiRest_clip_limits ( wc ) ;
      }
      else {
#if 0
          ret = 0;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( y->x1+x1 ) , ( y->y1+y1 ) , ( y->x2+x1 ) , \
           ( y->y2+y1 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#else
          RESTOREWIDGETAREA ( y ) ;
#endif
      }
      return ( ret ) ;
  }
  int _uiMake_E ( DIE *y ) {
      char **menu;
      BRW_STR *bwsr;
      int ret , n = 0 , i = 0 , x1 , y1;
      int height , extra , w;
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) ( y->D ) ;
      wc = WC ( D ) ;
      x1 = D->xo;
      y1 = D->yo;
      menu = ( char ** ) y->menu;
      n = 0;
      if ( menu != NULL ) while ( menu [ n ] != NULL ) n++;
      y->nitems = n;
      if ( y->size > n ) y->size = n;
      if ( * ( y->df ) < 1 ) * ( y->df ) = 1;
      if ( * ( y->df ) > n ) * ( y->df ) = n;
      bwsr = ( BRW_STR * ) y->bwsr;
      bwsr->D = ( DIALOG* ) D;
      bwsr->size = y->size;
      bwsr->x1 = y->x1+x1;
      bwsr->y1 = y->y1+y1;
      bwsr->x2 = y->x2+x1;
      bwsr->y2 = y->y2+y1;
      if ( y->hide != 1 ) {
//   bwsr->y2 =y->y1+y1+y->size*y->width+2*y->offset;
          CHECKLIMITS ( y ) ;
          if ( y->Bimg == NULL ) y->Bimg = kgGetBackground ( D , \
               bwsr->x1 , bwsr->y1 , bwsr->x2 , bwsr->y2 ) ;
          bwsr->hitem = 0;
          bwsr->pos = 0;
          bwsr->df = * ( y->df ) ;
          bwsr->xb = bwsr->x2;
          height = y->width;
          bwsr->width = height;
          y->size = ( y->y2-y->y1-2*y->offset ) /bwsr->width;
          if ( y->size > y->nitems ) y->size = y->nitems;
          bwsr->w = y->w;
          bwsr->offset = y->offset;
          if ( y->size <= 0 ) {
              y->size = 1;
//     y->y2 = height+y->y1+2*y->offset;
          }
          extra = ( y->y2 -y->y1-2*y->offset-y->size*height ) /2;
          if ( extra < -y->offset ) extra = -y->offset;
          bwsr->offset = y->offset+extra;
          bwsr->scroll = 1;
          bwsr->menu = ( char ** ) menu;
          if ( y->size == y->nitems ) {
              bwsr->scroll = 0;
          }
          bwsr->size = y->size;
          if ( bwsr->df > n ) bwsr->df = 1;
          if ( bwsr->df < 1 ) bwsr->df = 1;
          * ( y->df ) = bwsr->df;
          bwsr->pos = 0;
          bwsr->D = D;
          bwsr->hitem = bwsr->df-1;
          w = bwsr->scroll*y->w;
          bwsr->w = w;
          bwsr->tbx1 = bwsr->x2 - y->offset -w;
          bwsr->tby1 = bwsr->y1+y->offset;
          bwsr->tbx2 = bwsr->tbx1+w;
          bwsr->tby2 = bwsr->tby1+w;
          bwsr->bbx1 = bwsr->tbx1;
          bwsr->bby1 = bwsr->y2-y->offset-w;
          bwsr->bbx2 = bwsr->bbx1+w;
          bwsr->bby2 = bwsr->bby1+w;
          bwsr->rsx1 = bwsr->tbx1;
          bwsr->rsy1 = bwsr->tby2;
          bwsr->rsx2 = bwsr->bbx2;
          bwsr->rsy2 = bwsr->bby1;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _uiDrawDialogE ( y ) ;
          uiRest_clip_limits ( wc ) ;
      }
      else {
          ret = 0;
#if 0
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( bwsr->x1 ) , ( bwsr->y1 ) , ( bwsr->x2 ) , \
           ( bwsr->y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#else
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , bwsr->x1 , bwsr->y1 , \
           ( bwsr->x2-bwsr->x1+1 ) , ( bwsr->y2-bwsr->y1+1 ) ) ;
#endif
      }
      return ( ret ) ;
  }
  int _uiMake_Wbwsr ( DIW *y ) {
      char **menu;
      BRW_STR *bwsr;
      int ret , n = 0 , i = 0 , x1 , y1;
      int height , extra , w;
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) ( y->D ) ;
      wc = WC ( D ) ;
      x1 = D->xo;
      y1 = D->yo;
      menu = ( char ** ) y->menu;
      n = 0;
      if ( menu != NULL ) while ( menu [ n ] != NULL ) n++;
      y->nitems = n;
      if ( y->size > n ) y->size = n;
      if ( * ( y->df ) < 1 ) * ( y->df ) = 1;
      if ( * ( y->df ) > n ) * ( y->df ) = n;
      bwsr = ( BRW_STR * ) y->bwsr;
      bwsr->D = ( DIALOG* ) D;
      bwsr->size = y->size;
      bwsr->x1 = y->xb+x1;
      bwsr->y1 = y->y1+y1;
      bwsr->x2 = y->x2+x1;
      bwsr->offset = y->offset;
      bwsr->width = y->width;
      bwsr->w = y->w;
      bwsr->y2 = bwsr->y1+bwsr->size*y->width+2*y->offset;
      if ( bwsr->y2 > ( D->yo+D->yl ) ) bwsr->y2 = D->yo+D->yl;
      bwsr->hitem = 0;
      bwsr->pos = 0;
      bwsr->df = * ( y->df ) ;
      bwsr->xb = bwsr->x2;
      height = y->width;
      bwsr->width = height;
      y->size = ( bwsr->y2-bwsr->y1-2*y->offset ) /bwsr->width;
      if ( y->size > y->nitems ) y->size = y->nitems;
      bwsr->w = y->w;
      bwsr->offset = y->offset;
      extra = ( bwsr->y2 -bwsr->y1-2*y->offset-y->size*height ) /2;
      bwsr->offset = y->offset+extra;
      if ( y->size <= 0 ) {
          y->size = 1;
//     y->width=(bwsr->y2 -bwsr->y1-2*y->offset)/2;
      }
      bwsr->scroll = 1;
      bwsr->menu = ( char ** ) menu;
      if ( y->size == y->nitems ) {
          bwsr->scroll = 0;
      }
      bwsr->size = y->size;
      if ( bwsr->df > y->nitems ) bwsr->df = y->nitems;
      if ( bwsr->df < 1 ) bwsr->df = 1;
      * ( y->df ) = bwsr->df;
      bwsr->pos = 0;
      bwsr->D = D;
      bwsr->hitem = bwsr->df-1;
      w = bwsr->scroll*y->w;
      bwsr->w = w;
      bwsr->tbx1 = bwsr->x2 - y->offset -w;
      bwsr->tby1 = bwsr->y1+y->offset;
      bwsr->tbx2 = bwsr->tbx1+w;
      bwsr->tby2 = bwsr->tby1+w;
      bwsr->bbx1 = bwsr->tbx1;
      bwsr->bby1 = bwsr->y2-y->offset-w;
      bwsr->bbx2 = bwsr->bbx1+w;
      bwsr->bby2 = bwsr->bby1+w;
      bwsr->rsx1 = bwsr->tbx1;
      bwsr->rsy1 = bwsr->tby2;
      bwsr->rsx2 = bwsr->bbx2;
      bwsr->rsy2 = bwsr->bby1;
      kg_scr_back ( WC ( D ) , bwsr->x1 , bwsr->y1 , bwsr->x2 , bwsr->y2 ) ;
      _uiDrawDialogW ( y ) ;
      return ( ret ) ;
  }
  void uiDrawProgBartype0 ( DIO *o ) {
      void * fid;
      int ln , i , temp , l , w , color , offset = 2 , lp;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length , width , height;
      float yp , xp;
      int x1 , y1 , x2 , y2 , bclr;
      DIALOG *D;
      kgWC *wc;
      if ( o->hide ) return;
      D = o->D;
      wc = D->wc;
      x1 = D->xo+o->x1+2;
      y1 = D->yo+o->y1+2;
      x2 = D->xo+o->x2-2;
      y2 = D->yo+o->y2-2;
      width = x2-x1+1;
      height = y2-y1+1;
      xp = ( float ) width*0.5;
      yp = ( float ) height*0.5;
      l = width-2*offset;
      w = height-2*offset;
      if ( w > l ) { o->direction = 1; lp = w*o->percent/100.0; }
      else {o->direction = 0; lp = l*o->percent/100.0; }
      color = o->clr;
      if ( color == -1 ) color = D->gc.ProgColor;
      fid = kgInitImage ( width , height , RESIZE ) ;
      if ( fid != NULL ) {
          kgUserFrame ( fid , -1. , -1. , ( float ) width+1. , ( float ) height+1. ) ;
          bclr = o->bordr;
          if ( bclr == -1 ) bclr = D->gc.ProgBodrColor;
          if ( bclr < 0 ) {
              bclr = -bclr;
              kgChangeColor ( fid , 777 , bclr/1000000 , \
               ( bclr%1000000 ) /1000 , bclr%1000 ) ;
              bclr = 777;
          }
          if ( color < 0 ) {
              color = -color;
              kgChangeColor ( fid , 778 , color/1000000 , \
               ( color%1000000 ) /1000 , color%1000 ) ;
              color = 778;
          }
          kgLineColor ( fid , bclr ) ;
          kgRoundedRectangleFill ( fid , xp , yp , ( float ) width , \
           ( float ) height , 1 , bclr , 0.25 ) ;
          kgLineColor ( fid , D->gc.ProgFillColor ) ;
          kgRoundedRectangleFill ( fid , xp , yp , ( float ) width-2.0 , \
           ( float ) height-3.0 , 1 , D->gc.ProgFillColor , 0.25 ) ;
#if 0
          kgRoundedRectangle ( fid , xp , yp , ( float ) width , \
           ( float ) height , D->gc.ProgBodrColor , 0.25 , 0.5 ) ;
#endif
          kgLineColor ( fid , color ) ;
          if ( o->direction == 0 ) {
              if ( lp > 0 ) kgRoundedRectangleFill ( fid , lp*0.5+offset , \
                   yp , lp , w , 1 , color , 0.25 ) ;
          }
          else {
              if ( o->percent > 0 ) kgRoundedRectangleFill ( fid , xp , \
                   lp*0.5+offset , l , lp , 1 , color , 0.25 ) ;
          }
          img = kgGetResizedImage ( fid ) ;
//      img=kgGetSmoothImage(fid);
          kgCloseImage ( fid ) ;
      }
      if ( o->Bimg != NULL ) {
          kgRestoreImage ( D , o->Bimg , x1-2 , y1-2 , x2-x1+5 , y2-y1+5 ) ;
      }
      kgImage ( D , img , x1-2 , y1-2 , width+4 , height+4 , 0.0 , 1.0 ) ;
      kgFreeImage ( img ) ;
      return ;
  }
  void uiDrawProgBartype2 ( DIO *o ) {
      void * fid;
      int ln , i , temp , l , w , color , offset = 2 , lp , bclr;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length , width , height;
      float yp , xp;
      int x1 , y1 , x2 , y2;
      DIALOG *D;
      kgWC *wc;
      if ( o->hide ) return;
      D = o->D;
      wc = D->wc;
      x1 = D->xo+o->x1+2;
      y1 = D->yo+o->y1+2;
      x2 = D->xo+o->x2-2;
      y2 = D->yo+o->y2-2;
      width = x2-x1+1;
      height = y2-y1+1;
      xp = ( float ) width*0.5;
      yp = ( float ) height*0.5;
      l = width-2*offset;
      w = height-2*offset;
      if ( w > l ) { o->direction = 1; lp = w*o->percent/100.0; }
      else {o->direction = 0; lp = l*o->percent/100.0; }
      color = o->clr;
      if ( color == -1 ) color = D->gc.ProgColor;
      fid = kgInitImage ( width , height , RESIZE ) ;
      if ( fid != NULL ) {
          kgUserFrame ( fid , -2. , -2. , ( float ) width+2 , ( float ) height+2 ) ;
          bclr = o->bordr;
          if ( bclr == -1 ) bclr = D->gc.ProgBodrColor;
          if ( bclr < 0 ) {
              bclr = -bclr;
              kgChangeColor ( fid , 777 , bclr/1000000 , \
               ( bclr%1000000 ) /1000 , bclr%1000 ) ;
              bclr = 777;
          }
          if ( color < 0 ) {
              color = -color;
              kgChangeColor ( fid , 778 , color/1000000 , \
               ( color%1000000 ) /1000 , color%1000 ) ;
              color = 778;
          }
          kgLineColor ( fid , bclr ) ;
#if 1
          kgRoundedRectangle ( fid , xp , yp , ( float ) width , \
           ( float ) height , bclr , 0.2 , 0.2 ) ;
#else
          kgRoundedRectangleFill ( fid , xp , yp , ( float ) width , \
           ( float ) height , 1 , bclr , 0.2 ) ;
          kgRoundedRectangleFill ( fid , xp , yp , ( float ) width-2 , \
           ( float ) height-2 , 1 , D->gc.fill_clr , 0.2 ) ;
#endif
          kgLineColor ( fid , color ) ;
          if ( o->direction == 0 ) {
              if ( lp > 0 ) kgRoundedRectangleFill ( fid , lp*0.5+offset , \
                   yp , lp , w , 1 , color , 0.0 ) ;
          }
          else {
              if ( o->percent > 0 ) kgRoundedRectangleFill ( fid , \
                   xp , lp*0.5+offset , l , lp , 1 , color , 0.0 ) ;
          }
//      img=kgGetResizedImage(fid);
//      img=kgGetSmoothImage(fid);
          img = kgGetSharpImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      if ( o->Bimg != NULL ) {
          kgRestoreImage ( D , o->Bimg , x1-2 , y1-2 , x2-x1+5 , y2-y1+5 ) ;
      }
      kgImage ( D , img , x1-2 , y1-2 , width+4 , height+4 , 0.0 , 1.0 ) ;
      kgFreeImage ( img ) ;
      return ;
  }
  void uiDrawProgBartype3 ( DIO *o ) {
      void * fid;
      int ln , i , temp , l , w , color , bclr;
      float offset = 2 , lp;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length , width , height;
      float yp , xp;
      int x1 , y1 , x2 , y2;
      DIALOG *D;
      kgWC *wc;
      if ( o->hide ) return;
      D = o->D;
      wc = D->wc;
      x1 = D->xo+o->x1+2;
      y1 = D->yo+o->y1+2;
      x2 = D->xo+o->x2-2;
      y2 = D->yo+o->y2-2;
      width = x2-x1+1;
      height = y2-y1+1;
      xp = ( float ) width*0.5;
      yp = ( float ) height*0.5;
      if ( height < width ) {
          offset = height*0.5;
          l = width-2*offset;
          w = height*0.25;
      }
      else {
          offset = width*0.5;
          l = width*0.25;
          w = height -2*offset;
      }
      if ( w > l ) { o->direction = 1; lp = w*o->percent/100.0; }
      else {o->direction = 0; lp = l*o->percent/100.0; }
      color = o->clr;
      if ( color == -1 ) color = D->gc.ProgColor;
      fid = kgInitImage ( width , height , RESIZE ) ;
      if ( fid != NULL ) {
          kgUserFrame ( fid , -1.0 , -1.0 , ( float ) width+1.0 , \
           ( float ) height+1.0 ) ;
          bclr = o->bordr;
          if ( bclr == -1 ) bclr = D->gc.ProgBodrColor;
          if ( bclr < 0 ) {
              bclr = -bclr;
              kgChangeColor ( fid , 777 , bclr/1000000 , \
               ( bclr%1000000 ) /1000 , bclr%1000 ) ;
              bclr = 777;
          }
          if ( color < 0 ) {
              color = -color;
              kgChangeColor ( fid , 778 , color/1000000 , \
               ( color%1000000 ) /1000 , color%1000 ) ;
              color = 778;
          }
          kgLineColor ( fid , bclr ) ;
          if ( o->direction == 0 ) {
              kgRoundedRectangleFill ( fid , l*0.5+offset , yp , ( float ) l , \
               ( float ) w , 0 , D->gc.ProgFillColor , 0.5 ) ;
              if ( lp > 0 ) kgRoundedRectangleFill ( fid , lp*0.5+offset , \
                   yp , lp , w , 0 , color , 0.5 ) ;
              kgRoundedRectangleFill ( fid , lp+offset , yp , ( float ) w*3.0 , \
               ( float ) w*3.0 , 1 , color , 0.5 ) ;
              kgRoundedRectangleFill ( fid , lp+offset , yp , ( float ) w*2.0 , \
               ( float ) w*2.0 , 1 , color , 0.5 ) ;
          }
          else {
              kgRoundedRectangleFill ( fid , xp , w*0.5+offset , ( float ) l , \
               ( float ) w , color , D->gc.ProgFillColor , 0.5 ) ;
              if ( o->percent > 0 ) kgRoundedRectangleFill ( fid , xp , \
                   lp*0.5+offset , l , lp , color , color , 0.5 ) ;
              kgRoundedRectangleFill ( fid , xp , lp+offset , ( float ) l*3.0 , \
               ( float ) l*3.0 , 1 , color , 0.5 ) ;
              kgRoundedRectangleFill ( fid , xp , lp+offset , ( float ) l*2.0 , \
               ( float ) l*2.0 , 1 , color , 0.5 ) ;
          }
          img = kgGetResizedImage ( fid ) ;
//      img=kgGetSmoothImage(fid);
          kgCloseImage ( fid ) ;
      }
      if ( o->Bimg != NULL ) {
          kgRestoreImage ( D , o->Bimg , x1-2 , y1-2 , x2-x1+5 , y2-y1+5 ) ;
      }
      kgImage ( D , img , x1-2 , y1-2 , width+4 , height+4 , 0.0 , 1.0 ) ;
      kgFreeImage ( img ) ;
      return ;
  }
  int _uiMake_O ( DIO *o ) {
      int ret , n = 0 , i = 0 , x1 , y1 , x2 , y2 , l , w , offset = 3 , lp , color;
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) ( o->D ) ;
      wc = WC ( D ) ;
      if ( o->percent < 0 ) o->percent = 0;
      if ( o->percent > 100 ) o->percent = 100;
      o->oldval = o->percent;
      o->imgs = NULL;
      x1 = D->xo+o->x1+1;
      y1 = D->yo+o->y1+1;
      x2 = D->xo+o->x2-1;
      y2 = D->yo+o->y2-1;
      if ( o->hide != 1 ) {
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
//    _dvrect_fill_transparent(wc,(x1),(y1),(x2), (y2),D->gc.fill_clr,D->transparency);
          CHECKLIMITS ( o ) ;
          if ( o->Bimg == NULL ) o->Bimg = kgGetBackground \
           ( D , x1-1 , y1-1 , x2+1 , y2+1 ) ;
          if ( o->hide != 1 ) {
              switch ( o->type ) {
                  default:
                  case 0:
                  uiDrawProgBartype0 ( o ) ;
                  break;
                  case 2:
                  uiDrawProgBartype2 ( o ) ;
                  break;
                  case 3:
                  uiDrawProgBartype3 ( o ) ;
                  break;
                  case 1:
                  if ( o->Bimg != NULL ) kgRestoreImage ( D , o->Bimg , \
                       x1-1 , y1-1 , x2-x1+3 , y2-y1+3 ) ;
                  l = x2-x1-2*offset;
                  w = y2 -y1-2*offset;
                  if ( w > l ) { o->direction = 1; lp = w* ( 100.-o->percent ) /100.0; }
                  else {o->direction = 0; lp = l*o->percent/100.0; }
                  color = o->clr;
                  if ( color == -1 ) color = D->gc.ProgColor;
//       _dvbordertype0(D, x1+offset-2,y1+offset-2,x2+offset+2,y2-offset+2,D->gc.ProgBodrColor);
                  _dvrect_fill ( wc , x1+offset , y1+offset , x2-offset , \
                       y2-offset , D->gc.ProgFillColor ) ;
                  if ( o->direction == 0 ) {
                      if ( lp > 0 ) _dvrect_fill ( wc , x1+offset , y1+offset , \
                           x1+offset+lp , y1+offset+w , color ) ;
                  }
                  else {
                      if ( o->percent > 0 ) _dvrect_fill ( wc , x1+offset , \
                           y1+offset+lp , x1+offset+l , y2-offset , color ) ;
                  }
                  break;
              }
          }
          uiRest_clip_limits ( wc ) ;
      }
      else {
#if 0
          ret = 0;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( x1 ) , ( y1 ) , ( x2 ) , \
           ( y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          if ( o->Bimg != NULL ) kgRestoreImage ( D , o->Bimg , \
               x1-1 , y1-1 , x2-x1+3 , y2-y1+3 ) ;
      }
      return ( ret ) ;
  }
#if 0
  int Make_msg ( DIALOG *D , DIS *w , int x1 , int y1 ) {
      DIALOG *Parent;
      BRW_STR *bwsr;
      int ret , n = 0 , lngth = 0 , i = 0;
      kgWC *wc;
      wc = WC ( D ) ;
      if ( n != 0 ) {n++; lngth = 8; }
      lngth += n*9;
      make_msg_str ( w ) ;
      bwsr = ( BRW_STR * ) w->bwsr;
      bwsr->D = D;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _uiDrawMsg ( w->bwsr ) ;
      uiRest_clip_limits ( wc ) ;
      return 0;
  }
#endif
  int _uiMake_MS ( DIS *y ) {
      char **menu;
      BRW_STR *bwsr;
      Gclr gc;
      int ret , n = 0 , i = 0 , x1 , y1;
      int height , extra , w , exitems;
      kgWC *wc;
      DIALOG *D;
      D = ( DIALOG * ) ( y->D ) ;
      wc = WC ( D ) ;
      x1 = D->xo;
      y1 = D->yo;
      bwsr = ( BRW_STR * ) y->bwsr;
      bwsr->x1 = y->x1+D->xo;
      bwsr->y1 = y->y1+D->yo;
      bwsr->x2 = y->x2+D->xo;
      bwsr->y2 = y->y2+D->yo;
//  printf("%d %d %d %d:%d %d\n",y->x1,y->y1,y->x2,y->y2,y->width,y->size);
      if ( y->hide != 1 ) {
//   bwsr->y2 =y->y1+y1+y->size*y->width+2*y->offset;
          CHECKLIMITS ( y ) ;
          if ( y->Bimg == NULL ) y->Bimg = kgGetBackground ( D , \
               bwsr->x1 , bwsr->y1 , bwsr->x2 , bwsr->y2 ) ;
          bwsr->df = 1;
          menu = ( char ** ) y->menu;
          n = 0;
          if ( menu != NULL ) while ( menu [ n ] != NULL ) n++;
          bwsr->hitem = 0;
          bwsr->pos = 0;
          bwsr->xb = bwsr->x2;
          height = y->width;
          bwsr->width = height;
          y->size = ( y->y2-y->y1-2*y->offset ) /bwsr->width;
          y->nitems = n;
   //TCB NEW
          exitems = 0;
          if ( n > y->size ) {
              exitems = n -y->size;
#if 0
              menu += exitems;
              y->nitems = y->size;
#endif
          }
//   if(y->size>y->nitems) y->size=y->nitems;
          bwsr->w = y->w;
          bwsr->offset = y->offset;
          if ( y->size <= 0 ) {
              y->size = 1;
//     y->y2 = height+y->y1+2*y->offset;
          }
          extra = ( y->y2 -y->y1-2*y->offset-y->size*height ) /2;
          if ( extra < -y->offset ) extra = -y->offset;
   //TCB NEW
//   bwsr->offset =y->offset+extra;
          bwsr->scroll = 1;
          bwsr->menu = ( char ** ) menu;
//   if(y->size<=y->nitems) {
          if ( exitems == 0 ) {
              bwsr->scroll = 0;
          }
          bwsr->size = y->size;
#if 0   
          if ( bwsr->df > n ) bwsr->df = 1;
          if ( bwsr->df < 1 ) bwsr->df = 1;
#else
   //TCB NEW
          bwsr->df = exitems+1;
#endif
          bwsr->pos = exitems;
          bwsr->D = D;
          gc = D->gc;
//   bwsr->MS.color1=gc.fill_clr;bwsr->MS.color2=gc.high_clr;
          bwsr->MS.color1 = gc.msg_fill; bwsr->MS.color2 = gc.msg_bodr;
          bwsr->MS.char_clr = gc.msg_char;
          bwsr->hitem = bwsr->df-1;
          w = bwsr->scroll*y->w;
          bwsr->w = w;
          bwsr->tbx1 = bwsr->x2 - y->offset -w;
          bwsr->tby1 = bwsr->y1+y->offset;
          bwsr->tbx2 = bwsr->tbx1+w;
          bwsr->tby2 = bwsr->tby1+w;
          bwsr->bbx1 = bwsr->tbx1;
          bwsr->bby1 = bwsr->y2-y->offset-w;
          bwsr->bbx2 = bwsr->bbx1+w;
          bwsr->bby2 = bwsr->bby1+w;
          bwsr->rsx1 = bwsr->tbx1;
          bwsr->rsy1 = bwsr->tby2;
          bwsr->rsx2 = bwsr->bbx2;
          bwsr->rsy2 = bwsr->bby1;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _uiDrawDialogM ( y ) ;
          uiRest_clip_limits ( wc ) ;
      }
      else {
#if 0
          ret = 0;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( bwsr->x1 ) , ( bwsr->y1 ) , ( bwsr->x2 ) , \
           ( bwsr->y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#else
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , ( bwsr->x1 ) , \
           ( bwsr->y1 ) , ( bwsr->x2-bwsr->x1+1 ) , ( bwsr->y2-bwsr->y1+1 ) ) ;
#endif
      }
      return ( ret ) ;
  }
  void uiUpdateMsgScroll ( void *tmp , int item ) {
      DIS *S;
      S = ( DIS * ) uiGetWidget ( tmp , item ) ;
      _uiMake_MS ( S ) ;
      return ;
  }
  int _ui_processtextboxpress ( TX_STR *t , KBEVENT kbe ) {
      int button , col , row , box , ln , x , y , OK = 0 , pos , curbox;
      T_ELMT *elmt;
      DIT *T = ( DIT * ) t->T;
      int FontSize = T->FontSize;
      button = kbe.button;
      elmt = t->elmt;
      x = kbe.x;
      y = kbe.y;
      box = 0;
      for ( row = 0; row < t->ny; row++ ) {
          for ( col = 0; col < t->nx; col++ ) {
              if ( _uiCheckBox ( kbe , elmt [ box ] .x1 , elmt [ box ] .y1 , \
                   elmt [ box ] .x2 , elmt [ box ] .y2 ) ) \
              {
                  pos = ( x - elmt [ box ] .x1 -5 ) / ( Gap+Wd ) ;
                  ln = strlen ( elmt [ box ] .df+elmt [ box ] .startchar ) ;
//TCBTCB
//        if(pos> (ln-1)) pos=ln-1;
                  if ( pos > ( ln ) ) pos = ln;
                  if ( pos < 0 ) pos = 0;
                  _ui_cleantextcursor ( t ) ;
                  curbox = row*t->nx+col;
                  while ( t->elmt [ curbox ] .sw != 1 ) {
                      col = ( col+1 ) %t->nx;
                      curbox = row*t->nx+col;
                  }
                  t->col = col;
                  t->row = row;
                  if ( pos <= ln ) {
                      if ( pos >= elmt [ box ] .ln ) pos = elmt [ box ] .ln-1;
                      elmt [ box ] .cursor = pos+elmt [ box ] .startchar;
                  }
                  _ui_drawtextcursor ( t ) ;
                  uiUpdateOn ( t->D ) ;
                  OK = 1;
                  break;
              }
              box++;
          }
          if ( OK ) break;
      }
      return -1;
  }
  int _ui_processtableboxpress ( TX_STR *t , KBEVENT kbe ) {
      int button , col , row , box , ln , x , y , OK = 0 , pos , curbox;
      int FontSize;
      int Active;
      int k;
      DIALOG *D;
      T_ELMT *elmt;
      DIT *T = ( DIT * ) t->T;
      char *df;
      button = kbe.button;
//  tit = t->tit;
      elmt = t->elmt;
      D = t->D;
//      FontSize = D->gc.FontSize;
      FontSize = T->FontSize;
      x = kbe.x;
      y = kbe.y;
      box = 0;
      Active = 0;
      k = 0;
      for ( row = 0; row < t->ny; row++ ) {
          for ( col = 0; col < t->nx; col++ ) {
              if ( t->elmt [ k ] .sw == 1 ) {Active = 1; break;}
              k++;
          }
      }
      if ( Active == 0 ) return -1;
      for ( row = 0; row < t->ny; row++ ) {
          for ( col = 0; col < t->nx; col++ ) {
              if ( _uiCheckBox ( kbe , elmt [ box ] .x1 , elmt [ box ] .y1 , \
                   elmt [ box ] .x2 , elmt [ box ] .y2 ) ) \
              {
                  pos = ( x - elmt [ box ] .x1 -FontSize/2 ) / ( FontSize ) ;
                  if ( pos < 0 ) pos = 0;
                  ln = strlen ( elmt [ box ] .df+elmt [ box ] .startchar ) ;
                  _ui_cleantablecursor ( t ) ;
                  curbox = row*t->nx+col;
                  while ( t->elmt [ curbox ] .sw != 1 ) {
#if 0 //Original
                      col = ( col+1 ) %t->nx;
                      curbox = row*t->nx+col;
#else
                      curbox = ( curbox+1 ) ;
                      if ( curbox >= ( t->nx*t->ny ) ) curbox = 0;
#endif
                  }
                  t->col = curbox%t->nx;
                  t->row = curbox/t->nx;
                  T->col = t->col;
                  T->row = t->row;
                  if ( pos <= ln ) {
//          if(pos >= t->ln[box])pos = t->ln[box]-1;
                      df = elmt [ box ] .df;
                      if ( pos >= elmt [ box ] .ln ) pos = elmt [ box ] .ln-1;
                      elmt [ box ] .cursor = pos+elmt [ box ] .startchar;
                      while ( df [ elmt [ box ] .cursor ] == 127 ) elmt [ box ] .cursor += 1;
                  }
//                  _ui_drawtablecursor ( t ) ;
                  _ui_updatetablecursor ( t ) ;
                  uiUpdateOn ( t->D ) ;
                  OK = 1;
                  break;
              }
              box++;
          }
          if ( OK ) break;
      }
      return -1;
  }
  int _ui_processTextTablekey ( TX_STR *t , KBEVENT kbe , int code ) {
      int key , col , row , curbox , ln;
      T_ELMT *elmt;
      key = kbe.key;
//  tit = t->tit;
      elmt = t->elmt;
      curbox = t->row*t->nx+t->col;
      if ( elmt [ curbox ] .hlt ) {
          if ( ui_Backspace ( key ) ) {
              if ( _ui_cuthighlightstring ( t ) ) return -1;
          }
      }
      _ui_cleartexthigh ( t ) ;
      if ( ui_Uparrow ( key ) ) {
          row = t->row;
          row = ( row-1 ) ;
          if ( row < 0 ) row = t->ny-1;
          col = t->col;
          curbox = row*t->nx+col;
          while ( t->elmt [ curbox ] .sw != 1 ) {
              col = ( col+1 ) %t->nx;
              curbox = row*t->nx+col;
          }
          if ( code == 't' ) _ui_cleantextcursor ( t ) ;
          else _ui_cleantablecursor ( t ) ;
          t->col = col;
          t->row = row;
          if ( code == 't' ) _ui_drawtextcursor ( t ) ;
          else _ui_drawtablecursor ( t ) ;
          uiUpdateOn ( t->D ) ;
          return -1;
      }
      if ( ui_Downarrow ( key ) ) {
          row = t->row;
          row = ( row+1 ) %t->ny;
          col = t->col;
          curbox = row*t->nx+col;
          while ( t->elmt [ curbox ] .sw != 1 ) {
              col = ( col+1 ) %t->nx;
              curbox = row*t->nx+col;
          }
          if ( code == 't' ) _ui_cleantextcursor ( t ) ;
          else _ui_cleantablecursor ( t ) ;
          t->col = col;
          t->row = row;
          if ( code == 't' ) _ui_drawtextcursor ( t ) ;
          else _ui_drawtablecursor ( t ) ;
          uiUpdateOn ( t->D ) ;
          return -1;
      }
      if ( ui_Leftarrow ( key ) ) {
          if ( t->nx == 1 ) {
              col = t->col;
              row = t->row;
              curbox = row*t->nx+col;
              if ( t->elmt [ curbox ] .cursor > 0 ) {
                  t->elmt [ curbox ] .cursor -= 1;
                  if ( ( t->elmt [ curbox ] .cursor < t->elmt [ curbox ] .startchar ) ) t->elmt [ curbox ] .startchar = t->elmt [ curbox ] .cursor;
                      
                  if ( code == 't' ) _ui_drawtextcursor ( t ) ;
                  else _ui_drawtablecursor ( t ) ;
                  uiUpdateOn ( t->D ) ;
              }
              return -1;
          }
          else {
              row = t->row;
              col = t->col;
              col--;
              if ( col < 0 ) col = t->nx-1;
              curbox = row*t->nx+col;
              while ( t->elmt [ curbox ] .sw != 1 ) {
                  col = ( col+1 ) %t->nx;
                  curbox = row*t->nx+col;
              }
              if ( code == 't' ) _ui_cleantextcursor ( t ) ;
              else _ui_cleantablecursor ( t ) ;
              t->col = col;
              t->row = row;
              if ( code == 't' ) _ui_drawtextcursor ( t ) ;
              else _ui_drawtablecursor ( t ) ;
              uiUpdateOn ( t->D ) ;
              return -1;
          }
      }
      if ( ui_Rightarrow ( key ) ) {
          if ( t->nx == 1 ) {
              col = t->col;
              row = t->row;
              curbox = row*t->nx+col;
              ln = t->elmt [ curbox ] .ln-1;
              if ( t->elmt [ curbox ] .cursor < ( MAXTITEMLN-2 ) ) {
                  if ( t->elmt [ curbox ] .df [ t->elmt [ curbox ] .cursor ] != '\0' ) {
                      t->elmt [ curbox ] .cursor += 1;
                      if ( ( t->elmt [ curbox ] .cursor - t->elmt [ curbox ] .startchar ) > ln ) t->elmt [ curbox ] .startchar+= 1;
                          
                      if ( code == 't' ) _ui_drawtextcursor ( t ) ;
                      else _ui_drawtablecursor ( t ) ;
                      uiUpdateOn ( t->D ) ;
                  }
              }
              return -1;
          }
          else {
              col = t->col;
              col = ( col+1 ) %t->nx;
              row = t->row;
              curbox = row*t->nx+col;
              while ( t->elmt [ curbox ] .sw != 1 ) {
                  col = ( col+1 ) %t->nx;
                  curbox = row*t->nx+col;
              }
              if ( code == 't' ) _ui_cleantextcursor ( t ) ;
              else _ui_cleantablecursor ( t ) ;
              t->col = col;
              t->row = row;
              if ( code == 't' ) _ui_drawtextcursor ( t ) ;
              else _ui_drawtablecursor ( t ) ;
              uiUpdateOn ( t->D ) ;
              return -1;
          }
      }
      if ( ui_Linefeed ( key ) || ui_Return ( key ) ) {
          uiUpdateOn ( t->D ) ;
          return t->row*t->nx+t->col;
      }
      if ( ui_Pageup ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          ln = t->elmt [ curbox ] .ln-1;
          if ( t->elmt [ curbox ] .cursor < ( MAXTITEMLN-2 ) ) {
              if ( t->elmt [ curbox ] .df [ t->elmt [ curbox ] .cursor ] != '\0' ) {
                  t->elmt [ curbox ] .cursor += 1;
                  if ( ( t->elmt [ curbox ] .cursor - t->elmt [ curbox ] .startchar ) > ln ) t->elmt [ curbox ] .startchar+= 1;
                      
                  if ( code == 't' ) _ui_drawtextcursor ( t ) ;
                  else _ui_drawtablecursor ( t ) ;
                  uiUpdateOn ( t->D ) ;
              }
          }
          return -1;
      }
      if ( ui_Pagedown ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          if ( t->elmt [ curbox ] .cursor > 0 ) {
              t->elmt [ curbox ] .cursor -= 1;
              if ( ( t->elmt [ curbox ] .cursor < t->elmt [ curbox ] .startchar ) ) t->elmt [ curbox ] .startchar = t->elmt [ curbox ] .cursor;
                  
              if ( code == 't' ) _ui_drawtextcursor ( t ) ;
              else _ui_drawtablecursor ( t ) ;
              uiUpdateOn ( t->D ) ;
          }
          return -1;
      }
      if ( ui_Home ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          t->elmt [ curbox ] .cursor = 0;
          t->elmt [ curbox ] .startchar = 0;
          if ( code == 't' ) _ui_drawtextcursor ( t ) ;
          else _ui_drawtablecursor ( t ) ;
          uiUpdateOn ( t->D ) ;
          return -1;
      }
      if ( ui_End ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          ln = t->elmt [ curbox ] .ln-1;
          t->elmt [ curbox ] .cursor = strlen ( t->elmt [ curbox ] .df ) ;
          t->elmt [ curbox ] .startchar = t->elmt [ curbox ] .cursor -ln;
          if ( t->elmt [ curbox ] .startchar < 0 ) t->elmt [ curbox ] .startchar = 0;
          if ( code == 't' ) _ui_drawtextcursor ( t ) ;
          else _ui_drawtablecursor ( t ) ;
          uiUpdateOn ( t->D ) ;
          return -1;
      }
      if ( ui_Escape ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          ln = t->elmt [ curbox ] .ln-1;
          t->elmt [ curbox ] .df [ 0 ] = '\0';
          t->elmt [ curbox ] .cursor = 0;
          t->elmt [ curbox ] .startchar = 0;
          if ( code == 't' ) _ui_drawtextcursor ( t ) ;
          else _ui_drawtablecursor ( t ) ;
          uiUpdateOn ( t->D ) ;
          return -1;
      }
      if ( ui_Clear ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          ln = t->elmt [ curbox ] .ln-1;
          t->elmt [ curbox ] .df [ t->elmt [ curbox ] .cursor ] = '\0';
//    t->elmt[curbox].cursor = 0;
//    t->elmt[curbox].startchar =0;
          if ( code == 't' ) _ui_drawtextcursor ( t ) ;
          else _ui_drawtablecursor ( t ) ;
          uiUpdateOn ( t->D ) ;
          return -1;
      }
      if ( ui_Delete ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          if ( _ui_deletechar ( t->elmt [ curbox ] .df , \
               t->elmt [ curbox ] .cursor ) ) \
          {
              if ( code == 't' ) _ui_drawtextcursor ( t ) ;
              else _ui_drawtablecursor ( t ) ;
              uiUpdateOn ( t->D ) ;
          }
          return -1;
      }
      if ( ui_Backspace ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          if ( t->elmt [ curbox ] .cursor > 0 ) {
              if ( t->elmt [ curbox ] .startchar > 0 ) {
                  t->elmt [ curbox ] .startchar -= 1;
                  t->elmt [ curbox ] .cursor -= 1;
                  if ( _ui_deletechar ( t->elmt [ curbox ] .df , \
                       t->elmt [ curbox ] .cursor ) ) \
                  {
                      if ( code == 't' ) _ui_drawtextcursor ( t ) ;
                      else _ui_drawtablecursor ( t ) ;
                      uiUpdateOn ( t->D ) ;
                  }
              }
              else {
                  t->elmt [ curbox ] .cursor -= 1;
                  if ( ( t->elmt [ curbox ] .cursor < t->elmt [ curbox ] .startchar ) ) t->elmt [ curbox ] .startchar = t->elmt [ curbox ] .cursor;
                      
                  if ( _ui_deletechar ( t->elmt [ curbox ] .df , \
                       t->elmt [ curbox ] .cursor ) ) \
                  {
                      if ( code == 't' ) _ui_drawtextcursor ( t ) ;
                      else _ui_drawtablecursor ( t ) ;
                      uiUpdateOn ( t->D ) ;
                  }
              }
          }
          return -1;
      }
      if ( ( key >= ' ' ) && ( key < 128 ) ) {
          int ret;
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
//    if(key== ' ') printf("Got Space :%s %d %d %d\n",t->elmt[curbox].df,col,row,curbox);
          if ( ( ret = _ui_insertchar ( t->elmt [ curbox ] .df , t->elmt [ curbox ] .cursor , \
               MAXTITEMLN-2 , key ) ) ) \
          {
              t->elmt [ curbox ] .cursor += ret;
              ln = t->elmt [ curbox ] .ln;
              if ( ( t->elmt [ curbox ] .cursor - t->elmt [ curbox ] .startchar ) >= ln ) t->elmt [ curbox ] .startchar+= 1;
                  
              if ( code == 't' ) _ui_drawtextcursor ( t ) ;
              else _ui_drawtablecursor ( t ) ;
              uiUpdateOn ( t->D ) ;
          }
//    printf("ret = %d: %d\n",ret,t->elmt[curbox].cursor);
          return -1;
      }
      return -1;
  }
  int _ui_processTablekey ( TX_STR *t , KBEVENT kbe , int code ) {
      int key , col , row , curbox , ln;
      T_ELMT *elmt;
      int Active = 0 , box , nxbox;
      DIT *T = ( DIT * ) t->T;
      char *df;
      int k;
      key = kbe.key;
//  tit = t->tit;
      elmt = t->elmt;
      Active = 0;
      k = 0;
      nxbox = 0;
      for ( row = 0; row < t->ny; row++ ) {
          box = 0;
          for ( col = 0; col < t->nx; col++ ) {
              if ( t->elmt [ k ] .sw == 1 ) {Active = 1; box++;break;}
              k++;
          }
          if ( box > nxbox ) nxbox = box;
      }
      if ( Active == 0 ) return -1;
      if ( ui_Uparrow ( key ) ) {
          row = t->row;
          row = ( row-1 ) ;
          if ( row >= 0 ) {
              col = t->col;
              curbox = row*t->nx+col;
              while ( t->elmt [ curbox ] .sw != 1 ) {
#if 0  // original
                  col = ( col+1 ) %t->nx;
                  curbox = row*t->nx+col;
#else
                  row = row -1;
                  if ( row >= 0 ) curbox = row*t->nx+col;
                  else {
                      uiUpdateOn ( t->D ) ;
                      return SCROLL_UP;
                  }
#endif
              }
              _ui_cleantablecursor ( t ) ;
              t->col = curbox%t->nx;
              t->row = curbox/t->nx;
              T->col = t->col;
              T->row = t->row;
              _ui_drawtablecursor ( t ) ;
              uiUpdateOn ( t->D ) ;
              return -1;
          }
          else {
              uiUpdateOn ( t->D ) ;
              return SCROLL_UP;
          }
      }
      if ( ui_Downarrow ( key ) ) {
          row = t->row;
          row = ( row+1 ) ;
          if ( row < t->ny ) {
              col = t->col;
              curbox = row*t->nx+col;
              while ( t->elmt [ curbox ] .sw != 1 ) {
#if 0 // org
                  col = ( col+1 ) %t->nx;
                  curbox = row*t->nx+col;
#else
                  row = row +1;
                  if ( row < t->ny ) curbox = row*t->nx+col;
                  else {
                      uiUpdateOn ( t->D ) ;
                      return SCROLL_DOWN;
                  }
#endif
              }
              _ui_cleantablecursor ( t ) ;
              t->col = curbox%t->nx;
              t->row = curbox/t->nx;
              T->col = t->col;
              T->row = t->row;
              _ui_drawtablecursor ( t ) ;
              uiUpdateOn ( t->D ) ;
              return -1;
          }
          else {
              uiUpdateOn ( t->D ) ;
              return SCROLL_DOWN;
          }
      }
      if ( ui_Leftarrow ( key ) ) {
//          if ( t->nx == 1 ) {
          if ( nxbox == 1 ) {
              col = t->col;
              row = t->row;
              curbox = row*t->nx+col;
              df = t->elmt [ curbox ] .df;
              if ( t->elmt [ curbox ] .cursor > 0 ) {
                  t->elmt [ curbox ] .cursor -= 1;
                  while ( df [ t->elmt [ curbox ] .cursor ] == 127 ) t->elmt [ curbox ] .cursor -= 1;
                      
                  if ( ( t->elmt [ curbox ] .cursor < t->elmt [ curbox ] .startchar ) ) t->elmt [ curbox ] .startchar = t->elmt [ curbox ] .cursor;
                      
                  _ui_drawtablecursor ( t ) ;
                  uiUpdateOn ( t->D ) ;
              }
              return -1;
          }
          else {
              row = t->row;
              col = t->col;
              col--;
              if ( col < 0 ) col = t->nx-1;
              curbox = row*t->nx+col;
              while ( t->elmt [ curbox ] .sw != 1 ) {
                  col = ( col+1 ) %t->nx;
                  curbox = row*t->nx+col;
              }
              _ui_cleantablecursor ( t ) ;
              t->col = col;
              t->row = row;
              if ( code == 't' ) _ui_drawtextcursor ( t ) ;
              else _ui_drawtablecursor ( t ) ;
              uiUpdateOn ( t->D ) ;
              return -1;
          }
      }
      if ( ui_Rightarrow ( key ) ) {
//          if ( t->nx == 1 ) {
          if ( nxbox == 1 ) {
              col = t->col;
              row = t->row;
              curbox = row*t->nx+col;
              ln = t->elmt [ curbox ] .ln-1;
              df = t->elmt [ curbox ] .df;
              if ( t->elmt [ curbox ] .cursor < ( MAXTITEMLN-2 ) ) {
                  if ( t->elmt [ curbox ] .df [ t->elmt [ curbox ] .cursor ] != '\0' ) {
                      t->elmt [ curbox ] .cursor += 1;
                      while ( df [ t->elmt [ curbox ] .cursor ] == 127 ) t->elmt [ curbox ] .cursor += 1;
                          
                      if ( ( t->elmt [ curbox ] .cursor - t->elmt [ curbox ] .startchar ) > ln ) t->elmt [ curbox ] .startchar+= 1;
                          
                      _ui_drawtablecursor ( t ) ;
                      uiUpdateOn ( t->D ) ;
                  }
              }
              return -1;
          }
          else {
              col = t->col;
              col = ( col+1 ) %t->nx;
              row = t->row;
              curbox = row*t->nx+col;
              while ( t->elmt [ curbox ] .sw != 1 ) {
                  col = ( col+1 ) %t->nx;
                  curbox = row*t->nx+col;
              }
              _ui_cleantablecursor ( t ) ;
              t->col = col;
              t->row = row;
              if ( code == 't' ) _ui_drawtextcursor ( t ) ;
              else _ui_drawtablecursor ( t ) ;
              uiUpdateOn ( t->D ) ;
              return -1;
          }
      }
      if ( ui_Linefeed ( key ) || ui_Return ( key ) ) {
          uiUpdateOn ( t->D ) ;
//      return 1;
          return t->row*t->nx+t->col;
      }
      if ( ui_Pageup ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          df = t->elmt [ curbox ] .df;
          ln = t->elmt [ curbox ] .ln-1;
          if ( t->elmt [ curbox ] .cursor < ( MAXTITEMLN-2 ) ) {
              if ( t->elmt [ curbox ] .df [ t->elmt [ curbox ] .cursor ] != '\0' ) {
                  t->elmt [ curbox ] .cursor += 1;
                  while ( df [ t->elmt [ curbox ] .cursor ] == 127 ) t->elmt [ curbox ] .cursor += 1;
                      
                  if ( ( t->elmt [ curbox ] .cursor - t->elmt [ curbox ] .startchar ) > ln ) t->elmt [ curbox ] .startchar+= 1;
                      
                  _ui_drawtablecursor ( t ) ;
                  uiUpdateOn ( t->D ) ;
              }
          }
          return -1;
      }
      if ( ui_Pagedown ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          df = t->elmt [ curbox ] .df;
          if ( t->elmt [ curbox ] .cursor > 0 ) {
              t->elmt [ curbox ] .cursor -= 1;
              while ( df [ t->elmt [ curbox ] .cursor ] == 127 ) t->elmt [ curbox ] .cursor -= 1;
                  
              if ( ( t->elmt [ curbox ] .cursor < t->elmt [ curbox ] .startchar ) ) t->elmt [ curbox ] .startchar = t->elmt [ curbox ] .cursor;
                  
              _ui_drawtablecursor ( t ) ;
              uiUpdateOn ( t->D ) ;
          }
          return -1;
      }
      if ( ui_Home ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          t->elmt [ curbox ] .cursor = 0;
          t->elmt [ curbox ] .startchar = 0;
          _ui_drawtablecursor ( t ) ;
          uiUpdateOn ( t->D ) ;
          return -1;
      }
      if ( ui_End ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          ln = t->elmt [ curbox ] .ln-1;
          t->elmt [ curbox ] .cursor = strlen ( t->elmt [ curbox ] .df ) ;
          t->elmt [ curbox ] .startchar = t->elmt [ curbox ] .cursor -ln;
          if ( t->elmt [ curbox ] .startchar < 0 ) t->elmt [ curbox ] .startchar = 0;
          _ui_drawtablecursor ( t ) ;
          uiUpdateOn ( t->D ) ;
          return -1;
      }
#if 0
      if ( ui_Escape ( key ) || ui_Clear ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          ln = t->elmt [ curbox ] .ln-1;
          t->elmt [ curbox ] .df [ 0 ] = '\0';
          t->elmt [ curbox ] .cursor = 0;
          t->elmt [ curbox ] .startchar = 0;
          _ui_drawtablecursor ( t ) ;
          uiUpdateOn ( t->D ) ;
          return -1;
      }
#endif
      if ( ui_Delete ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          df = t->elmt [ curbox ] .df;
          while ( df [ t->elmt [ curbox ] .cursor +1 ] == 127 ) {
              _ui_deletechar ( t->elmt [ curbox ] .df , t->elmt [ curbox ] .cursor ) ;
          }
          if ( _ui_deletechar ( t->elmt [ curbox ] .df , \
               t->elmt [ curbox ] .cursor ) ) \
          {
              _ui_drawtablecursor ( t ) ;
              uiUpdateOn ( t->D ) ;
          }
          return -1;
      }
      if ( ui_Backspace ( key ) ) {
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
	  // NEW
          if ( elmt [ curbox ] .hlt ) {
              if ( _ui_cuttablehighlightstring ( t ) ) return -1;
          }
          df = t->elmt [ curbox ] .df;
          while ( df [ t->elmt [ curbox ] .cursor -1 ] == 127 ) { // may need further checking
              t->elmt [ curbox ] .cursor -= 1;
              if ( ( t->elmt [ curbox ] .cursor < t->elmt [ curbox ] .startchar ) ) t->elmt [ curbox ] .startchar = t->elmt [ curbox ] .cursor;
                  
              _ui_deletechar ( t->elmt [ curbox ] .df , t->elmt [ curbox ] .cursor ) ;
          }
	  //End New
          if ( t->elmt [ curbox ] .cursor > 0 ) {
              if ( t->elmt [ curbox ] .startchar > 0 ) {
                  t->elmt [ curbox ] .startchar -= 1;
                  t->elmt [ curbox ] .cursor -= 1;
                  if ( _ui_deletechar ( t->elmt [ curbox ] .df , \
                       t->elmt [ curbox ] .cursor ) ) \
                  {
                      _ui_drawtablecursor ( t ) ;
                      uiUpdateOn ( t->D ) ;
                  }
              }
              else {
                  t->elmt [ curbox ] .cursor -= 1;
                  if ( ( t->elmt [ curbox ] .cursor < t->elmt [ curbox ] .startchar ) ) t->elmt [ curbox ] .startchar = t->elmt [ curbox ] .cursor;
                      
                  if ( _ui_deletechar ( t->elmt [ curbox ] .df , \
                       t->elmt [ curbox ] .cursor ) ) \
                  {
                      _ui_drawtablecursor ( t ) ;
                      uiUpdateOn ( t->D ) ;
                  }
              }
          }
          return -1;
      }
      if ( ( ( key >= ' ' ) && ( key < 128 ) ) || ( ui_Tab ( key ) ) ) {
          int ret;
          col = t->col;
          row = t->row;
          curbox = row*t->nx+col;
          if ( ui_Tab ( key ) ) key = '\t';
          if ( ( ret = _ui_insertchar ( t->elmt [ curbox ] .df , t->elmt [ curbox ] .cursor , \
               MAXTITEMLN-2 , key ) ) ) \
          {
              t->elmt [ curbox ] .cursor += ret;
              ln = t->elmt [ curbox ] .ln;
              if ( ( t->elmt [ curbox ] .cursor - t->elmt [ curbox ] .startchar ) >= ln ) t->elmt [ curbox ] .startchar+= 1;
                  
              _ui_drawtablecursor ( t ) ;
              uiUpdateOn ( t->D ) ;
          }
          return -1;
      }
      return -1;
  }
#if 0
/* Processing Key Press */
  int _ui_processkeyTextTable ( TX_STR *t , KBEVENT kbe ) {
      Gclr gc;
      char ch , c;
      char *ptr;
      TX_STR *t;
      int xx , yy , yl , yu , xl , xu , n , pointer , ret = 1 , temp;
      static int i = 0 , k = 0 , ii = 0;
      int j = 0 , jo , menu_x , menu_y , len , nine , l , nxx , nyy , p;
      char fill_clr = 0; /* Correction made for wiping */
      gc = get_gui_colours ( ) ;
      Bkup_clip_limits ( ) ;
      Set_full_scrn ( ) ;
      SetGuiFixFontSize ( TextSize ) ;
      fill_clr = gc.t_fill_clr;
      nxx = t->nx;
      nyy = t->ny;
      n = nxx*t->ny;
      nine = 9;
      if ( pos < 0 ) pos = 0;
      ii = pos*nxx+col;
      if ( t->tit [ ii ] .sw != 1 ) ii = 0;
      while ( t->tit [ ii ] .sw != 1 ) {
          ii++;
          if ( ii == ( nxx*nyy ) ) {
              gprintf ( "Wrong Cursor Position" ) ;
              normal ( ) ;
              exit ( 0 ) ;
          }
      }
      temp = char_clr;
      char_clr = t_char_clr;
      menu_x = t->tit [ ii ] .x1+5;
      menu_y = EVGAY- ( t->tit [ ii ] .y2-6 ) ;
      ptr = t->tit [ ii ] .df;
      Menuch ( cur_clr ) ;
      TBL_POS;
      ch = kb.kbc [ 0 ] ;
      while ( ch == '\r' ) {
          TBL_POS; ch = kb.kbc [ 0 ] ;
      }
      while ( ch != '\n' ) {
          while ( ! input_attention ( ) ) sleep ( 1 ) ;
          if ( ch == '\t' ) {ret = 2; break; };
          if ( ch == '\r' ) { TBL_POS; ch = kb.kbc [ 0 ] ; continue; }
          if ( ch == '\0' ) {
              ch = kb.kbc [ 1 ] ;
              rmv_pointer ( ) ;
              switch ( ch ) {
                  case 'H' :
                  Menuch ( fill_clr ) ;
                  j = 0;
                  i--; if ( i < 0 ) {
                      i = nyy-1; k = 0;
                      ret = 3;
                      goto jump;
                  };
                  p = i*nxx+k;
                  ii = p;
                  refix_cur_position;
                  TBL_POS;
                  ch = kb.kbc [ 0 ] ;
                  break;
                  case 'P' :
                  Menuch ( fill_clr ) ;
                  j = 0;
                  i++; if ( i == nyy ) {
                      i = 0; k = 0;
                      ret = 4;
                      goto jump;
                  }
                  p = i*nxx+k; ii = p;
                  refix_cur_position;
                  TBL_POS;
                  ch = kb.kbc [ 0 ] ;
                  break;
                  case 'p' :
                  Menuch ( fill_clr ) ;
                  j = 0;
                  p = ii;
                  refix_cur_position;
                  TBL_POS;
                  ch = kb.kbc [ 0 ] ;
                  break;
                  case 'K' :
                  Menuch ( fill_clr ) ;
                  j = 0;
                  k--; if ( k < 0 ) k = nxx-1;
                  p = i*nxx+k; ii = p;
                  while ( t->tit [ p ] .sw != 1 ) {
                      k--; if ( k < 0 ) {k = 0; break; }
                      p = i*nxx+k; ii = p;
                  }
                  refix_cur_position;
                  TBL_POS;
                  ch = kb.kbc [ 0 ] ;
                  break;
                  case 'M' :
                  Menuch ( fill_clr ) ;
                  j = 0;
                  k++; if ( k == nxx ) k = 0;
                  p = i*nxx+k; ii = p;
                  while ( t->tit [ p ] .sw != 1 ) {
                      k++; if ( k >= nxx ) {k = nxx-1; break; }
                      p = i*nxx+k; ii = p;
                  }
                  refix_cur_position;
                  TBL_POS;
                  ch = kb.kbc [ 0 ] ;
                  break;
                  default :
                  TBL_POS;
                  ch = kb.kbc [ 0 ] ;
                  break;
              }
              rmv_pointer ( ) ;
          }
          else {
              if ( ch < ' ' ) {
                  if ( ( ch == '\b' ) && ( j != 0 ) ) {
                      rmv_pointer ( ) ;
                      _rect_fill ( menu_x , menu_y-Bt , menu_x+ ( t->ln [ ii ] -1-j ) *nine , \
                           menu_y+Ht , fill_clr ) ;
                      j--;
                      for ( p = j; p < t->ln [ ii ] -2; p++ ) {
                          ptr = t->tit [ ii ] .df+p;
                          * ( ptr ) = * ( ptr+1 ) ;
                          menu_x = t->tit [ ii ] .x1+5+p*nine;
                          menuch ( *ptr , menu_x , menu_y , char_clr ) ;
                      }
                      * ( ptr+1 ) = ' ';
                      ptr = t->tit [ ii ] .df+j;
                      menu_x = t->tit [ ii ] .x1+5+j*nine;
                      _rect_fill ( menu_x+Gap , menu_y-Bt , menu_x+Gap+Wd , menu_y+Ht , cur_clr ) ;
                      menuch ( *ptr , menu_x , menu_y , char_clr ) ;
                      dsp_pointer ( ) ;
                  }
              }
              else {
                  rmv_pointer ( ) ;
                  * ( ptr ) = ch;
                  _rect_fill ( menu_x+Gap , menu_y-Bt , menu_x+Gap+Wd , menu_y+Ht , fill_clr ) ;
                  menuch ( *ptr , menu_x , menu_y , char_clr ) ;
                  j++; if ( j == ( t->ln [ ii ] -1 ) ) j = t->ln [ ii ] -2;
                  ptr = t->tit [ ii ] .df+j;
                  menu_x = t->tit [ ii ] .x1+5+j*nine;
                  _rect_fill ( menu_x+Gap , menu_y-Bt , menu_x+Gap+Wd , menu_y+Ht , cur_clr ) ;
                  menuch ( *ptr , menu_x , menu_y , char_clr ) ;
                  dsp_pointer ( ) ;
              }
              TBL_POS;
              ch = kb.kbc [ 0 ] ;
          }
      }
      jump:
      if ( ( ch == '\n' ) && ( kb.kbc [ 1 ] == 1 ) ) { /* key presses outside */
          ret = 3;
      }
      Menuch ( fill_clr ) ;
      char_clr = temp;
      if ( pointer != 1 ) rmv_pointer ( ) ;
      DefaultGuiFontSize ( ) ;
      Rest_clip_limits ( ) ;
      return ( ret ) ;
  }
#endif
  void _uiDrawPicture ( DIALOG* D , int item ) {
      int x1 , y1 , x2 , y2 , color;
      DIP *p;
      Gclr gc;
      char **buf;
      char *pt;
      int l , w;
      kgWC *wc;
      wc = WC ( D ) ;
      p = D->d [ item ] .p;
      p->item = item;
      p->Bimg = NULL;
      if ( p->transparency > 0.000001 ) {
          p->Bimg = kgGetBackground ( D , p->x1 , p->y1 , p->x2 , p->y2 ) ;
//    kgWriteImage(p->img,"/root/DIP.jpg");
      }
      buf = ( char ** ) ( p->xpm ) ;
      pt = ( char * ) ( p->xpm ) ;
      x1 = p->x1+D->xo;
      y1 = p->y1+D->yo;
      x2 = p->x2+D->xo;
      y2 = p->y2+D->yo;
      color = p->bkgr_clr;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      w = x2 -x1;
      l = ( y2 - y1 ) ;
      if ( color >= 0 ) {
          _uibox_fill ( wc , x1 , D->evgay-y1 , x2-2 , D->evgay-y2+1 , \
           ( unsigned int ) color ) ;
      }
      else {
          gc = D->gc;
          color = gc.fill_clr;
      }
      if ( buf != NULL ) {
          if ( ! ( ( pt [ 0 ] == '#' ) && ( pt [ 1 ] == '#' ) && \
           ( pt [ 2 ] <= ' ' ) ) ) {
//       jpg_image(p->xpm,x1,y1,w,l);
              kgImage ( D , p->xpm , x1 , y1 , w , l , 0.0 , 1.0 ) ;
          }
      }
      uiRest_clip_limits ( wc ) ;
  }
  DIALOG *kgGetParent ( DIALOG *D ) {
      DIALOG *Dtmp;
      Dtmp = D;
      while ( Dtmp->Newwin == 0 ) {
          Dtmp = ( DIALOG * ) ( Dtmp->parent ) ;
          if ( Dtmp == NULL ) {
              fprintf ( stderr , "Error: NULL parent for a child UI\n" ) ;
              exit ( 0 ) ;
          }
      }
      return Dtmp;
  }
  void _uiDrawTransparentPicture ( DIALOG* D , int item ) {
      int x1 , y1 , x2 , y2 , color;
      DIP *p;
      Gclr gc;
      char **buf;
      char *pt;
      int l , w;
      kgWC *wc;
      wc = WC ( D ) ;
      p = D->d [ item ] .p;
      p->item = item;
      p->D = D;
      buf = ( char ** ) ( p->xpm ) ;
      pt = ( char * ) ( p->xpm ) ;
      x1 = p->x1+D->xo;
      y1 = p->y1+D->yo;
      x2 = p->x2+D->xo;
      y2 = p->y2+D->yo;
      color = p->bkgr_clr;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      w = x2 -x1;
      l = ( y2 - y1 ) ;
      if ( color >= 0 ) {
          _uibox_fill ( wc , x1 , D->evgay-y1 , x2-2 , D->evgay-y2+1 , \
           ( unsigned int ) color ) ;
      }
      else {
          gc = D->gc;
          color = gc.fill_clr;
      }
      if ( buf != NULL ) {
          if ( ! ( ( pt [ 0 ] == '#' ) && ( pt [ 1 ] == '#' ) && \
           ( pt [ 2 ] <= ' ' ) ) ) {
//       jpg_image_transparent(p->xpm,x1,y1,w,l,D->transparency);
//       jpg_image(p->xpm,x1,y1,w,l);
              kgImage ( D , p->xpm , x1 , y1 , w , l , D->transparency , 1.0 ) ;
          }
      }
      uiRest_clip_limits ( wc ) ;
  }
  int _uiMake_P ( DIP *p ) {
      int ret = 1;
      int x1 , y1 , x2 , y2 , color , offset = 2;
      Gclr gc;
      char **buf;
      char *pt;
      int l , w;
      DIALOG *D;
      kgWC *wc;
      D = p->D;
      wc = WC ( D ) ;
      x1 = p->x1+D->xo;
      y1 = p->y1+D->yo;
      x2 = p->x2+D->xo;
      y2 = p->y2+D->yo;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      w = x2 -x1;
      l = ( y2 - y1 ) ;
      if ( p->hide != 1 ) {
          buf = ( char ** ) ( p->xpm ) ;
          pt = ( char * ) ( p->xpm ) ;
          color = p->bkgr_clr;
          CHECKLIMITS ( p ) ;
          if ( p->Bimg == NULL ) {
              p->Bimg = kgGetBackground ( D , x1 , y1 , x2 , y2 ) ;
          }
          if ( p->transparency > 0.000001 ) {
              void *img1 , *img2;
//    if(p->img!= NULL) kgFreeImage(p->img);
              if ( buf != NULL ) {
                  img1 = NULL;
                  if ( ! ( ( pt [ 0 ] == '#' ) && ( pt [ 1 ] == '#' ) && \
                   ( pt [ 2 ] <= ' ' ) ) ) {
                      img1 = kgGetImageCopy ( D , p->xpm ) ;
                  }
                  else if ( pt != NULL ) img1 = ( void * ) pt;
                  if ( img1 != NULL ) {
                      kgAddTransparency ( img1 , p->transparency ) ;
                      img2 = kgGetImageCopy ( D , p->Bimg ) ;
                      kgMergeTransparentImage ( img2 , img1 , 0 , 0 ) ;
//       kgImage(D,img2,x1,y1,w,l,0.0,1.0);
                      kgRestoreImage ( D , img2 , x1 , y1 , w+1 , l+1 ) ;
                      kgFreeImage ( img1 ) ;
                      kgFreeImage ( img2 ) ;
                  }
                  else kgRestoreImage ( D , p->Bimg , x1 , y1 , w+1 , l+1 ) ;
              }
              else kgRestoreImage ( D , p->Bimg , x1 , y1 , w+1 , l+1 ) ;
          } // f transparency
          else {
              kgRestoreImage ( D , p->Bimg , x1 , y1 , w+1 , l+1 ) ;
              if ( D->DrawBkgr != 0 ) {
                  if ( color >= 0 ) {
                      _dvrect_fill_transparent ( wc , x1 , y1 , x2 , y2 , \
                       ( unsigned int ) color , 0.0 ) ;
                  }
#if 0
                  else {
                      gc = D->gc;
                      color = gc.fill_clr;
                      _dvrect_fill_transparent ( wc , x1 , y1 , x2 , y2 , \
                       ( unsigned int ) color , D->transparency ) ;
                  }
#endif
              }
              if ( buf != NULL ) {
                  if ( ! ( ( pt [ 0 ] == '#' ) && ( pt [ 1 ] == '#' ) && \
                   ( pt [ 2 ] <= ' ' ) ) ) {
                      kgImage ( D , p->xpm , x1 , y1 , w , l , D->transparency , 1.0 ) ;
                  }
              }
              else if ( pt != NULL ) kgImage ( D , p->xpm , x1 , \
                   y1 , w , l , D->transparency , 1.0 ) ;
              switch ( p->bordr ) {
                  case 0:
                  break;
                  deafult:
                  break;
                  case 2:
                  offset = 1;
                  offset = 0;
                  _dvbordertype1 ( D , x1-offset , y1-offset , x2+offset , y2+offset ) ;
                  break;
                  case 3:
                  offset = 4;
                  offset = 0;
                  _dvbordertype0 ( D , x1-offset , y1-offset , \
                       x2+offset , y2+offset , gc.bright ) ;
                  break;
                  case 1:
                  offset = 2;
                  offset = 0;
                  _dvbordertype2 ( D , x1-offset , y1-offset , \
                       x2+offset , y2+offset , 0 , 0 , 0 , 0 ) ;
                  break;
                  case 6:
                  offset = 4;
                  offset = 0;
                  _dvbordertype3 ( D , x1-offset , y1-offset , x2+offset , y2+offset ) ;
                  case 5:
                  offset = 3;
                  offset = 0;
                  _dvbordertype4 ( D , x1-offset , y1-offset , x2+offset , y2+offset ) ;
                  break;
                  case 4:
                  offset = 2;
                  offset = 0;
                  dvdraw_protru ( D , x1-offset , y1-offset , x2+offset , y2+offset ) ;
                  break;
              }
          }// else transparency
      }
      else {
          ret = 0;
          if ( p->Bimg != NULL ) {
              kgRestoreImage ( D , p->Bimg , x1 , y1 , w+1 , l+1 ) ;
          }
#if 0
          else {
//      printf("p->img  NULL\n");
              _dvrect_fill_transparent ( wc , ( x1 ) , ( y1 ) , ( x2 ) , \
               ( y2 ) , D->gc.fill_clr , D->transparency ) ;
          }
#endif
      }
      uiRest_clip_limits ( wc ) ;
      return 1;
  }
  void _uiDrawP ( DIALOG *D , int item ) {
      int x1 , y1 , x2 , y2 , color;
      DIP *p;
      Gclr gc;
      char **buf;
      char *pt;
      int l , w;
      kgWC *wc;
      p = D->d [ item ] .p;
      p->item = item;
      p->D = D;
      p->Bimg = NULL;
      if ( p->hide != 1 ) _uiMake_P ( p ) ;
  }
  void _ui_draw_info_bound ( DIALOG *D , int x1 , int y1 , \
       int x2 , int y2 , int bright , int dim ) \
  {
      unsigned int temp;
      kgWC *wc;
      wc = WC ( D ) ;
      temp = WC ( D )->c_color;
      uiset_clr ( D , bright ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2 , ( int ) y2 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2 , ( int ) x2 ) ;
      uiset_clr ( D , dim ) ;
      _ui_h_line ( wc , ( int ) x1 , ( int ) x2-1 , ( int ) y1 ) ;
      _ui_v_line ( wc , ( int ) y1 , ( int ) y2+1 , ( int ) x1 ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( wc ) ;
      return;
  }
  void * _uiput_txtwin ( DIALOG *D , int ix , int iy , int x2 , int y2 ) {
      TWIN *Twin;
      int chrs , lines;
      int y;
      DIALOG *Dtmp;
      Dtmp = ( DIALOG * ) ( D ) ;
      chrs = ( x2-ix-10 ) /9;
      lines = ( y2-iy-2 ) /22;
      if ( lines < 1 ) lines = 1;
      uiPressedBoxFill ( D , ix , iy , ( x2-ix ) , ( y2-iy ) , \
           D->gc.info_fill , D->gc.fill_clr , 3 , 0.0 ) ;
      Twin = ( TWIN * ) Malloc ( sizeof ( TWIN ) ) ;
      Twin->xl = ix+5; Twin->yl = iy+4; Twin->xu = x2-2;
      y2 = ( 22*lines+Twin->yl ) ;
      Twin->yu = y2; Twin->w = 0;
      Twin->nchr = chrs;
      Twin->xp = Twin->xl;
      Twin->yp = Twin->yl;
      return ( void * ) Twin;
  }
  void _uiDrawInfo ( DIALOG* D , int item ) {
      DII *I;
      I = D->d [ item ] .i;
      I->D = D;
      I->item = item; ;
      I->Bimg = NULL;
      I->twin = _uiput_txtwin ( D , I->x1+D->xo , ( I->y1+D->yo ) , I->x2+D->xo , \
       ( I->y2+D->yo ) ) ;
      return;
  }
  int _uiMake_I ( DII *I ) {
      TWIN *Twin;
      int chrs , lines , lw = 22;
      int y , ret = 1;
      int x1 , y1 , x2 , y2;
      DIALOG *D;
      D = ( DIALOG * ) ( I->D ) ;
      x1 = I->x1+D->xo; x2 = I->x2+D->xo;
      y1 = I->y1+D->yo; y2 = I->y2+D->yo;
      if ( I->linewidth <= 0 ) I->linewidth = lw;
      if ( I->hide != 1 ) {
          CHECKLIMITS ( I ) ;
          BACKUPWIDGETAREA ( I ) ;
          lw = I->linewidth;
          if ( lw > ( y2-y1-2 ) ) lw = y2-y1-2;
          chrs = ( x2-x1-10 ) /9;
//    printf("%d %d %d\n",lw,y2,y1);
          lines = ( y2-y1-2 ) /lw;
          if ( lines < 1 ) {
              lines = 1;
              lw = y2-y1-2;
          }
          I->linewidth = lw;
          uiPressedBoxFill ( D , x1 , y1 , ( x2-x1 ) , ( y2-y1 ) , \
               D->gc.info_fill , D->gc.fill_clr , 3 , 0.0 ) ;
          Twin = ( TWIN * ) ( I->twin ) ;
          Twin->xl = x1+5; Twin->yl = y1+4; Twin->xu = x2-2;
          y2 = ( lw*lines+Twin->yl ) ;
          Twin->yu = y2; Twin->w = lw;
          Twin->nchr = chrs;
          Twin->xp = Twin->xl;
          Twin->yp = Twin->yl;
      }
      else {
#if 0
          kgWC *wc;
          wc = WC ( D ) ;
          ret = 0;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          _dvrect_fill_transparent ( wc , ( x1 ) , ( y1 ) , ( x2 ) , \
           ( y2 ) , D->gc.fill_clr , D->transparency ) ;
          uiRest_clip_limits ( wc ) ;
#endif
          RESTOREWIDGETAREA ( I ) ;
      }
      return ret;
  }
  void _uiDrawI ( DIALOG* D , int item ) {
      DII *I;
      I = D->d [ item ] .i;
      I->D = D;
      I->item = item;
      I->twin = ( void * ) Malloc ( sizeof ( TWIN ) ) ;
      I->linewidth = 22;
      I->Bimg = NULL;
      if ( I->hide != 1 ) _uiMake_I ( I ) ;
      return;
  }
  void _ui_putstring ( DIALOG *D , int x , int y , char *t , int char_clr ) {
      int xx , yy;
      int i = 0 , n;
      xx = x;
      yy = D->evgay-y;
      n = strlen ( t ) ;
      for ( i = 0; i < n; i++ ) {
          uimenuch ( D , * ( t+i ) , xx , yy , char_clr ) ;
          xx += 9;
      }
  }
  int _ui_textboxstringlength ( TX_STR *tx ) {
      int ln , strl , ch;
      char *pt , *df;
      T_ELMT *elmt;
      elmt = tx->elmt+tx->row*tx->nx+tx->col;
      ln = elmt->ln;
      df = elmt->df+elmt->startchar;
      ch = df [ ln ] ;
      df [ ln ] = '\0';
      strl = strlen ( df ) ;
      df [ ln ] = ch;
      strl = ( strl* ( Wd+Gap ) +6 ) ;
      return strl;
  }
  int _ui_tableboxstringlength ( TX_STR *tx ) {
      int ln , strl , ch;
      char *pt , *df;
      T_ELMT *elmt;
      DIT *T;
      T = tx->T;
      elmt = tx->elmt+tx->row*tx->nx+tx->col;
      ln = elmt->ln;
      df = elmt->df+elmt->startchar;
      ch = df [ ln ] ;
      df [ ln ] = '\0';
      strl = strlen ( df ) ;
      df [ ln ] = ch;
      strl = ( strl* T->FontSize+T->FontSize/2 ) ;
      return strl;
  }
  char *_ui_gethighlightstring ( TX_STR *tx ) {
      int ln , strl , ch , start , end , i;
      char *pt , *df;
      int xs , xe;
      T_ELMT *elmt;
      elmt = tx->elmt+tx->row*tx->nx+tx->col;
      if ( elmt->hlt == 0 ) return NULL;
      ln = elmt->ln;
      df = elmt->df+elmt->startchar;
      xs = elmt->hxs;
      xe = elmt->hxe;
      if ( xs > xe ) return NULL;
      start = ( xs-4 ) / ( Wd+Gap ) -1;
      end = ( xe-4 ) / ( Wd+Gap ) -1;
      if ( start < 0 ) return NULL;
      if ( start > ln ) return NULL;
      if ( end < 0 ) return NULL;
      if ( end > ln ) end = ln;
      strl = ( end-start+1 ) ;
      pt = ( char * ) Malloc ( sizeof ( char ) * ( strl+1 ) ) ;
      for ( i = 0; i < strl; i++ ) pt [ i ] = df [ start+i ] ;
      pt [ strl ] = '\0';
      return pt;
  }
  char *_ui_gettablehighlightstring ( TX_STR *tx ) {
      int ln , strl , ch , start , end , i;
      char *pt , *df;
      int xs , xe;
      T_ELMT *elmt;
      DIT *T = tx->T;
      int FontSize = T->FontSize;
      elmt = tx->elmt+tx->row*tx->nx+tx->col;
      if ( elmt->hlt == 0 ) return NULL;
      ln = elmt->ln;
      df = elmt->df+elmt->startchar;
      xs = elmt->hxs;
      xe = elmt->hxe;
      if ( xs > xe ) return NULL;
      start = ( xs-4 ) / ( Wd+Gap ) -1;
      end = ( xe-4 ) / ( Wd+Gap ) -1;
      start = ( xs -FontSize/2 ) /FontSize;
      end = ( xe -FontSize/2 ) /FontSize;
      if ( start < 0 ) return NULL;
      if ( start > ln ) return NULL;
      if ( end < 0 ) return NULL;
      if ( end > ln ) end = ln;
      strl = ( end-start+1 ) ;
      pt = ( char * ) Malloc ( sizeof ( char ) * ( strl+1 ) ) ;
      for ( i = 0; i < strl; i++ ) pt [ i ] = df [ start+i ] ;
      pt [ strl ] = '\0';
      return pt;
  }
  int _ui_cuthighlightstring ( TX_STR *tx ) {
      int ln , strl , ch , start , end , i;
      char *pt , *df;
      int xs , xe;
      T_ELMT *elmt;
      elmt = tx->elmt+tx->row*tx->nx+tx->col;
      if ( elmt->hlt == 0 ) return 0;
      ln = elmt->ln;
      df = elmt->df+elmt->startchar;
      xs = elmt->hxs;
      xe = elmt->hxe;
      if ( xs > xe ) return 0;
      start = ( xs-4 ) / ( Wd+Gap ) -1;
      end = ( xe-4 ) / ( Wd+Gap ) -1;
      if ( start < 0 ) return 0;
      if ( start > ln ) return 0;
      if ( end < 0 ) return 0;
      if ( end > ln ) end = ln;
      strl = ( end-start+1 ) ;
      pt = ( char * ) Malloc ( sizeof ( char ) * ( strl+1 ) ) ;
      i = 0;
      while ( 1 ) {
          df [ start+i ] = df [ start+i+strl ] ;
          if ( df [ start+i ] == '\0' ) break;
          i++;
      }
      elmt->hlt = 0;
      elmt->cursor = start+elmt->startchar;
      _ui_drawtextcursor ( tx ) ;
      kgUpdateOn ( tx->D ) ;
      return 1;
  }
  int _ui_cuttablehighlightstring ( TX_STR *tx ) {
      int ln , strl , ch , start , end , i;
      char *pt , *df;
      int xs , xe;
      T_ELMT *elmt;
      DIT *T = tx->T;
      int FontSize = T->FontSize;
      elmt = tx->elmt+tx->row*tx->nx+tx->col;
      if ( elmt->hlt == 0 ) return 0;
      ln = elmt->ln;
      df = elmt->df+elmt->startchar;
      xs = elmt->hxs;
      xe = elmt->hxe;
      if ( xs > xe ) return 0;
      start = ( xs -FontSize/2 ) /FontSize;
      end = ( xe -FontSize/2 ) /FontSize;
      if ( start < 0 ) return 0;
      if ( start > ln ) return 0;
      if ( end < 0 ) return 0;
      if ( end > ln ) end = ln;
      strl = ( end-start+1 ) ;
      pt = ( char * ) Malloc ( sizeof ( char ) * ( strl+1 ) ) ;
      i = 0;
      while ( 1 ) {
          df [ start+i ] = df [ start+i+strl ] ;
          if ( df [ start+i ] == '\0' ) break;
          i++;
      }
      elmt->hlt = 0;
      elmt->cursor = start+elmt->startchar;
      _ui_drawtablecursor ( tx ) ;
      kgUpdateOn ( tx->D ) ;
      return 1;
  }
  void _ui_drawtextcursor ( TX_STR *tx ) {
      int curbox , cx , cy , ch , ln;
      char *df;
      DIALOG *D;
      T_ELMT *elmt;
      int type , noecho;
      void *img;
      int x1 , y1 , x2 , y2;
      D = ( DIALOG * ) ( tx->D ) ;
//  tit= tx->tit;
      type = tx->T->type;
      elmt = tx->elmt;
      curbox = tx->row*tx->nx+tx->col;
//  ln = tx->ln[curbox];
      ln = elmt [ curbox ] .ln;
      df = elmt [ curbox ] .df+elmt [ curbox ] .startchar;
      x1 = ( tx->elmt [ curbox ] .x1 ) ;
      y1 = ( tx->elmt [ curbox ] .y1 ) ;
      x2 = ( tx->elmt [ curbox ] .x2 ) ;
      y2 = ( tx->elmt [ curbox ] .y2 ) ;
      noecho = tx->elmt [ curbox ] .noecho;
      switch ( type ) {
          case 1:
          img = elmt [ curbox ] .img;
          if ( img == NULL ) {
              void *Bimg = NULL;
              Bimg = kgGetBackground ( D , x1-1 , y1 , x2+4 , y2 ) ;
              img = kgPressedRectangle ( x2-x1+2 , abs ( y2-y1 ) , tx->gc.txt_fill , 0.5 ) ;
              Bimg = kgMergeImages ( Bimg , img , 0 , 0 ) ;
              elmt [ curbox ] .img = Bimg;
              kgFreeImage ( img ) ;
              img = Bimg;
          }
//         _dvrect_fill_transparent(WC(D),x1-2,y1,x2+2,y2,D->gc.fill_clr,D->transparency);
//         kgImage(D,img,x1-1,y1,x2-x1+5,y2-y1,0.0,1.0);
          kgRestoreImage ( D , img , x1-1 , y1 , x2-x1+5, y2-y1 ) ;
          break;
          default:
          _uirect_fill ( WC ( D ) , x1 , D->evgay-y1 , x2-1 , \
               D->evgay-y2+1 , tx->gc.txt_fill ) ;
          dvdraw_depre ( D , x1 , y1 , x2 , y2 ) ;
          break;
      }
      ch = df [ ln ] ;
      df [ ln ] = '\0';
//TCB
      if ( elmt [ curbox ] .hlt ) {
          int hxs , hxe;
          hxs = elmt [ curbox ] .hxs+elmt [ curbox ] .x1;
          hxe = elmt [ curbox ] .hxe+elmt [ curbox ] .x1;
          if ( ( hxe-hxs ) > 2 ) {
              cx = elmt [ curbox ] .x1+6;
              if ( hxs < cx ) hxs = cx;
              if ( hxe > ( elmt [ curbox ] .x2-3 ) ) hxe = elmt [ curbox ] .x2-3;
              cy = ( elmt [ curbox ] .y2+elmt [ curbox ] .y1 ) /2+6;
              cy = D->evgay-cy;
              _uirect_fill ( WC ( D ) , hxs , cy-Bt , hxe , cy+Ht , tx->gc.high_clr ) ;
          }
      }
      cy = ( elmt [ curbox ] .y2+elmt [ curbox ] .y1 ) /2+6;
      cx = elmt [ curbox ] .x1+6+ ( elmt [ curbox ] .cursor-elmt [ curbox ] .startchar ) * \
       ( Gap+Wd ) ;
      cy = D->evgay-cy;
      _uirect_fill ( WC ( D ) , cx+Gap+1 , cy-Bt , \
           cx+Gap+1+Wd , cy+Ht , tx->gc.cur_clr ) ;
      cy = D->evgay-cy;
      if ( noecho != 1 ) {
          uiSetGuiFixFontSize ( D , D->gc.InputFontSize ) ;
          _ui_putstring ( D , elmt [ curbox ] .x1+6 , cy , df , tx->gc.txt_char ) ;
      }
      else {
          char *stars , *str;
          int ln , i;
          uiSetNoechoFontSize ( D , D->gc.InputFontSize/2 ) ;
          str = df;
          ln = strlen ( str ) +1;
          stars = ( char * ) Malloc ( sizeof ( char ) *ln ) ;
          for ( i = 0; i < ( ln-1 ) ; i++ ) {
              if ( str [ i ] > ' ' ) stars [ i ] = 'l';
              else stars [ i ] = ' ';
          }
          stars [ ln-1 ] = '\0';
          _ui_putstring ( D , elmt [ curbox ] .x1+7 , cy , stars , tx->gc.txt_char ) ;
          free ( stars ) ;
      }
      df [ ln ] = ch;
      uiDefaultGuiFontSize ( D ) ;
      return;
  }
  void _ui_cleartexthigh ( TX_STR *tx ) {
      int curbox , cx , cy , ch , ln , i , n;
      char *df;
      DIALOG *D;
      T_ELMT *elmt;
      int type , noecho;
      void *img;
      int x1 , y1 , x2 , y2;
      D = ( DIALOG * ) ( tx->D ) ;
//  tit= tx->tit;
      type = tx->T->type;
      elmt = tx->elmt;
      n = tx->nx*tx->ny;
      i = tx->row*tx->nx+tx->col;
      for ( curbox = 0; curbox < n; curbox++ ) {
          if ( elmt [ curbox ] .hlt == 0 ) continue;
          elmt [ curbox ] .hlt = 0;
          ln = elmt [ curbox ] .ln;
          df = elmt [ curbox ] .df+elmt [ curbox ] .startchar;
          x1 = ( tx->elmt [ curbox ] .x1 ) ;
          y1 = ( tx->elmt [ curbox ] .y1 ) ;
          x2 = ( tx->elmt [ curbox ] .x2 ) ;
          y2 = ( tx->elmt [ curbox ] .y2 ) ;
          noecho = tx->elmt [ curbox ] .noecho;
          switch ( type ) {
              case 1:
              img = elmt [ curbox ] .img;
              if ( img == NULL ) {
                  void *Bimg = NULL;
                  Bimg = kgGetBackground ( D , x1-1 , y1 , x2+4 , y2 ) ;
                  img = kgPressedRectangle ( x2-x1+2 , abs ( y2-y1 ) , tx->gc.txt_fill , 0.5 ) ;
                  Bimg = kgMergeImages ( Bimg , img , 0 , 0 ) ;
                  elmt [ curbox ] .img = Bimg;
                  kgFreeImage ( img ) ;
                  img = Bimg;
              }
//         _dvrect_fill_transparent(WC(D),x1-2,y1,x2+2,y2,D->gc.fill_clr,D->transparency);
//         kgImage(D,img,x1-1,y1,x2-x1+5,y2-y1,0.0,1.0);
              kgRestoreImage ( D , img , x1-1 , y1 , x2-x1+5 , y2-y1 ) ;
              break;
              default:
              _uirect_fill ( WC ( D ) , x1 , D->evgay-y1 , x2-1 , \
                   D->evgay-y2+1 , tx->gc.txt_fill ) ;
              dvdraw_depre ( D , x1 , y1 , x2 , y2 ) ;
              break;
          }
          ch = df [ ln ] ;
          df [ ln ] = '\0';
//TCB
          cy = ( elmt [ curbox ] .y2+elmt [ curbox ] .y1 ) /2+6;
          cx = elmt [ curbox ] .x1+6+ ( elmt [ curbox ] .cursor- elmt [ curbox ] .startchar ) * \
           ( Gap+Wd ) ;
          cy = D->evgay-cy;
          if ( i == curbox ) {
              _uirect_fill ( WC ( D ) , cx+Gap+1 , cy-Bt , \
                   cx+Gap+1+Wd , cy+Ht , tx->gc.cur_clr ) ;
          }
          cy = D->evgay-cy;
          if ( noecho != 1 ) {
              uiSetGuiFixFontSize ( D , D->gc.InputFontSize ) ;
              _ui_putstring ( D , elmt [ curbox ] .x1+6 , cy , df , tx->gc.txt_char ) ;
          }
          else {
              char *stars , *str;
              int ln , i;
              uiSetNoechoFontSize ( D , D->gc.InputFontSize/2 ) ;
              str = df;
              ln = strlen ( str ) +1;
              stars = ( char * ) Malloc ( sizeof ( char ) *ln ) ;
              for ( i = 0; i < ( ln-1 ) ; i++ ) {
                  if ( str [ i ] > ' ' ) stars [ i ] = 'l';
                  else stars [ i ] = ' ';
              }
              stars [ ln-1 ] = '\0';
              _ui_putstring ( D , elmt [ curbox ] .x1+7 , cy , stars , tx->gc.txt_char ) ;
              free ( stars ) ;
          }
          df [ ln ] = ch;
      }
      uiDefaultGuiFontSize ( D ) ;
      return;
  }
  void *_uiMakeTextBoxImage ( void *Tmp ) {
      void *img = NULL;
      void *Bimg = NULL;
      TX_STR *tx;
      T_ELMT *elmt;
      DIALOG *D;
      int type , x1 , y1 , x2 , y2 , curbox;
      tx = ( TX_STR * ) Tmp;
      elmt = tx->elmt;
      type = tx->T->type;
      D = ( DIALOG * ) ( tx->D ) ;
      curbox = tx->row*tx->nx+tx->col;
      x1 = ( elmt [ curbox ] .x1 ) ;
      y1 = ( elmt [ curbox ] .y1 ) ;
      x2 = ( elmt [ curbox ] .x2 ) ;
      y2 = ( elmt [ curbox ] .y2 ) ;
      curbox = tx->row*tx->nx+tx->col;
      Bimg = kgGetBackground ( D , x1-1 , y1 , x2+4 , y2 ) ;
      img = kgPressedRectangle ( x2-x1+2 , abs ( y2-y1 ) , tx->gc.txt_fill , 0.5 ) ;
      Bimg = kgMergeImages ( Bimg , img , 0 , 0 ) ;
      elmt [ curbox ] .img = Bimg;
      kgFreeImage ( img ) ;
      img = Bimg;
      return img;
  }
  void _ui_cleantextcursor ( TX_STR *tx ) {
      char *df;
      int curbox , cx , cy , ln , ch;
      T_ELMT *elmt;
      int type , noecho;
      int *img;
      int x1 , y1 , x2 , y2;
      DIALOG *D;
      int EVGAY;
      type = tx->T->type;
      D = ( DIALOG * ) ( tx->D ) ;
      EVGAY = D->evgay;
//  tit= tx->tit;
      elmt = tx->elmt;
      curbox = tx->row*tx->nx+tx->col;
      df = elmt [ curbox ] .df+elmt [ curbox ] .startchar;
//  ln = tx->ln[curbox];
      ln = elmt [ curbox ] .ln;
      x1 = ( tx->elmt [ curbox ] .x1 ) ;
      y1 = ( tx->elmt [ curbox ] .y1 ) ;
      x2 = ( tx->elmt [ curbox ] .x2 ) ;
      y2 = ( tx->elmt [ curbox ] .y2 ) ;
      noecho = tx->elmt [ curbox ] .noecho;
      switch ( type ) {
          case 1:
          img = elmt [ curbox ] .img;
          if ( img == NULL ) {
              void *Bimg = NULL;
              Bimg = kgGetBackground ( D , x1-1 , y1 , x2+4 , y2 ) ;
              img = kgPressedRectangle ( x2-x1+2 , abs ( y2-y1 ) , tx->gc.txt_fill , 0.5 ) ;
              Bimg = kgMergeImages ( Bimg , img , 0 , 0 ) ;
              elmt [ curbox ] .img = Bimg;
              kgFreeImage ( img ) ;
              img = Bimg;
          }
//          _dvrect_fill_transparent ( WC ( D ) , x1-2 , y1 , x2+2 , \
//               y2 , D->gc.fill_clr , D->transparency ) ;
//         kgImage(D,img,x1-1,y1,x2-x1+5,y2-y1,0.0,1.0);
          kgRestoreImage ( D , img , x1-1 , y1 , x2-x1+5 , y2-y1 ) ;
          break;
          default:
          _uirect_fill ( WC ( D ) , x1 , D->evgay-y1 , x2-1 , \
               D->evgay-y2+1 , tx->gc.txt_fill ) ;
          dvdraw_depre ( D , x1 , y1 , x2 , y2 ) ;
          break;
      }
#if 0
      _uirect_fill ( WC ( D ) , elmt [ curbox ] .x1 , EVGAY-elmt [ curbox ] .y1 , \
           elmt [ curbox ] .x2-1 , EVGAY-elmt [ curbox ] .y2+1 , tx->gc.txt_fill ) ;
      uidraw_depre ( D , elmt [ curbox ] .x1 , EVGAY-elmt [ curbox ] .y1 , \
           elmt [ curbox ] .x2 , EVGAY-elmt [ curbox ] .y2 ) ;
#endif
      ch = df [ ln ] ;
      df [ ln ] = '\0';
      cy = ( elmt [ curbox ] .y2+elmt [ curbox ] .y1 ) /2+6;
      if ( noecho != 1 ) {
          uiSetGuiFixFontSize ( D , D->gc.InputFontSize ) ;
          _ui_putstring ( D , elmt [ curbox ] .x1+6 , cy , df , tx->gc.txt_char ) ;
      }
      else {
          char *stars , *str;
          int ln , i;
          uiSetNoechoFontSize ( D , D->gc.InputFontSize/2 ) ;
          str = df;
          ln = strlen ( str ) +1;
          stars = ( char * ) Malloc ( sizeof ( char ) *ln ) ;
          for ( i = 0; i < ( ln-1 ) ; i++ ) {
              if ( str [ i ] > ' ' ) stars [ i ] = 'l';
              else stars [ i ] = ' ';
          }
          stars [ ln-1 ] = '\0';
          _ui_putstring ( D , elmt [ curbox ] .x1+7 , cy , stars , tx->gc.txt_char ) ;
          free ( stars ) ;
      }
      df [ ln ] = ch;
      uiDefaultGuiFontSize ( D ) ;
      return;
  }
  int kgScrollUpTable ( void *Tmp , int row ) {
	  /* Not Useful as of 19th Aug 24 */
      DIT *T = ( DIT * ) Tmp;
      if ( T->code != 'T' ) return 0;
      void *img = NULL;
      int i , j , k , cell , cell1;
      T_ELMT *elmt;
      DIALOG *D = T->D;
      TX_STR *tx = T->tstr;
      elmt = tx->elmt;
      int x1 , y1 , x2 , y2;
      for ( j = 1;j >= row;j++ ) {
          k = row-j;
          for ( i = 0;i < T->nx;i++ ) {
              cell = ( k ) *T->nx+i;
              x1 = elmt [ cell ] .x1;
              y1 = elmt [ cell ] .y1;
              x2 = elmt [ cell ] .x2;
              y2 = elmt [ cell ] .y2;
              img = kgGetBackground ( D , x1 , y1 , x2 , y2 ) ;
              cell1 = cell+T->nx;
              x1 = elmt [ cell1 ] .x1;
              y1 = elmt [ cell1 ] .y1;
              x2 = elmt [ cell1 ] .x2;
              y2 = elmt [ cell1 ] .y2;
              strcpy ( elmt [ cell1 ] .df , elmt [ cell ] .df ) ;
//      kgImage ( D , img , x1 , y1 , x2-x1 , y2-y1 , 0.0 , 1.0 ) ;
              kgRestoreImage ( D , img , x1 , y2 , x2-x1 , y2-y1 ) ;
              kgFreeImage ( img ) ;
          }
      }
//       kgUpdateWidget(T);
//     kgUpdateOn(D);
      return 1;
  }
  int kgScrollDownTable ( void *Tmp , int row ) {
	  /* Not Useful as of 19th Aug 24 */
      DIT *T = ( DIT * ) Tmp;
      if ( T->code != 'T' ) return 0;
      void *img = NULL;
      int i , j , k , cell , cell1;
      T_ELMT *elmt;
      DIALOG *D = T->D;
      TX_STR *tx = T->tstr;
      elmt = tx->elmt;
      int x1 , y1 , x2 , y2;
      for ( k = 1;k <= row;k++ ) {
          for ( i = 0;i < T->nx;i++ ) {
              cell = ( k ) *T->nx+i;
              x1 = elmt [ cell ] .x1;
              y1 = elmt [ cell ] .y1;
              x2 = elmt [ cell ] .x2;
              y2 = elmt [ cell ] .y2;
              img = kgGetBackground ( D , x1 , y1 , x2 , y2 ) ;
              cell1 = cell-T->nx;
              x1 = elmt [ cell1 ] .x1;
              y1 = elmt [ cell1 ] .y1;
              x2 = elmt [ cell1 ] .x2;
              y2 = elmt [ cell1 ] .y2;
              strcpy ( elmt [ cell1 ] .df , elmt [ cell ] .df ) ;
              if ( img != NULL ) {
//      kgImage ( D , img , x1 , y1 , x2-x1 , y2-y1 , 0.0 , 1.0 ) ;
                  kgRestoreImage ( D , img , x1 , y2 , x2-x1 , y2-y1 ) ;
                  kgFreeImage ( img ) ;
              }
              else fprintf ( stderr , "Failed to copy screen\n" ) ;
          }
      }
 //      kgUpdateWidget(T);
//       kgUpdateOn(D);
      return 1;
  }
  void *uiMakeTableCellImage ( DIT *T , int cell , int drcur ) {
      void *fid , *img;
      T_ELMT *elmt;
      DIALOG *D = T->D;
      TX_STR *tx = T->tstr;
      int ch;
      int curbox;
      int size , xsize , ysize , FontSize , sw;
      int rd , gr , bl;
      float th , tw , tg , xx , yy;
      int tfill , tclr;
      int x1 , y1 , x2 , y2;
      int k = cell , i;
      int type = T->type;
      float curpos;
      kgWC *wc;
      kgDC *dc;
      wc = WC ( D ) ;
      elmt = tx->elmt;
      curbox = tx->row*tx->nx+tx->col;
      FontSize = T->FontSize;
      sw = elmt [ k ] .sw;
      x1 = elmt [ k ] .x1;
      y1 = elmt [ k ] .y1;
      x2 = elmt [ k ] .x2;
      y2 = elmt [ k ] .y2;
      tclr = tx->gc.tabl_char;
      tfill = tx->gc.tabl_fill;
      xsize = ( x2-x1 ) ;
      ysize = ( y2-y1 ) ;
      fid = kgInitImage ( xsize+1 , ysize+1 , 1 ) ;
      kgUserFrame ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize ) ;
      kgBoxFill ( fid , 0.0 , 0.0 , ( float ) xsize , \
       ( float ) ysize , tfill , 0 ) ;
      th = FontSize*1.2;
      tw = FontSize+1;
      yy = 0.6*FontSize;
      if ( drcur ) {
          if ( elmt [ k ] .hlt ) {
              float hxs , hxe;
              hxs = elmt [ k ] .hxs;
              hxe = elmt [ k ] .hxe;
              if ( ( hxe-hxs ) > 2 ) {
                  kgBoxFill ( fid , hxs , yy-th*0.3 , hxe , th+yy-1 , tx->gc.high_clr , 0 ) ;
              }
          }
          curpos = ( elmt [ k ] .cursor-elmt [ k ] .startchar ) *FontSize+FontSize/2;
          kgBoxFill ( fid , curpos , yy-th*0.3 , curpos+FontSize , \
               th+yy-1 , D->gc.cur_clr , 0 ) ;
      }
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  int _uiPrintTableCell ( DIT *T , int cell , int drcur ) {
      T_ELMT *elmt;
      DIALOG *D = T->D;
      TX_STR *tx = T->tstr;
      int x1 , y1 , x2 , y2;
      int k = cell , i;
      int type = T->type;
      char *str;
      FONT_STR F;
      IMG_STR *IMG;
      IMG_STR ** Imgs;
      int ch;
      int curbox;
      int size , xsize , ysize , FontSize , sw;
      int rd , gr , bl;
      float th , tw , tg , xx , yy;
      int tfill , tclr;
      char Buf [ 2000 ] , stmp [ 10 ] ;
      void *fid , *img , *img2;
      float curpos;
      int ylng;
      kgWC *wc;
      kgDC *dc;
      wc = WC ( D ) ;
      elmt = tx->elmt;
      curbox = tx->row*tx->nx+tx->col;
      strcpy ( stmp , ( char * ) " " ) ;
      FontSize = T->FontSize;
      Imgs = ( IMG_STR ** ) tx->F.Imgs;
      sw = elmt [ k ] .sw;
      x1 = elmt [ k ] .x1;
      y1 = elmt [ k ] .y1;
      x2 = elmt [ k ] .x2;
      y2 = elmt [ k ] .y2;
      tclr = tx->gc.tabl_char;
      tfill = tx->gc.tabl_fill;
      if ( sw == 0 ) tclr = tx->gc.tabl_hchar;
      kgGetDefaultRGB ( tclr , & rd , & gr , & bl ) ;
#if 0
      if ( type == 1 ) {
          y1 -= 2;
          y2 += 2;
      }
#endif
      size = get_t_item_size ( elmt [ k ] .fmt ) ;
      str = elmt [ k ] .df;
      strcpy ( Buf , str+elmt [ k ] .startchar ) ;
      Buf [ size+1 ] = '\0';
      ylng = strlen ( Buf ) *FontSize+FontSize*2;
      xsize = ( x2-x1 ) ;
      ysize = ( y2-y1 ) ;
#if 0
      fid = kgInitImage ( xsize+1 , ysize+1 , 1 ) ;
      kgUserFrame ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize ) ;
      kgBoxFill ( fid , 0.0 , 0.0 , ( float ) xsize , \
       ( float ) ysize , tfill , 0 ) ;
      th = FontSize*1.2;
      tw = FontSize+1;
      yy = 0.6*FontSize;
      if ( drcur ) {
          if ( elmt [ k ] .hlt ) {
              float hxs , hxe;
              hxs = elmt [ k ] .hxs;
              hxe = elmt [ k ] .hxe;
              if ( ( hxe-hxs ) > 2 ) {
                  kgBoxFill ( fid , hxs , yy-th*0.3 , hxe , th+yy-1 , tx->gc.high_clr , 0 ) ;
              }
          }
          curpos = ( elmt [ k ] .cursor-elmt [ k ] .startchar ) *FontSize+FontSize/2;
          kgBoxFill ( fid , curpos , yy-th*0.3 , curpos+FontSize , \
               th+yy-1 , D->gc.cur_clr , 0 ) ;
      }
 //     img = kgGetSharpImage ( fid ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
#else
      img = uiMakeTableCellImage ( T , cell , drcur ) ;
#endif 
      tx->F.code = 't';
      if ( tx->F.Imgs == NULL ) {
          tx->F.code = 'i';
      }
      IMG = uiMakeFixedString ( & ( tx->F ) , Buf , ( int ) ysize+1 , 0 ) ;
      kgSetImageColor ( IMG->img , rd , gr , bl ) ;
      if ( IMG->img != NULL ) kgAddImages ( img , IMG->img , \
           FontSize/2 , ysize-2*FontSize+1 ) ;
      kgImage ( D , img , x1 , y1+2 , xsize , ysize , 0.0 , 1.0 ) ;
      kgFreeImage ( img ) ;
      kgFreeImage ( elmt [ k ] .img ) ;
      elmt [ k ] .img = IMG->img;
      free ( IMG ) ;
      return 1;
  }
  int _uiUpdateTableCell ( DIT *T , int cell , int drcur ) {
      T_ELMT *elmt;
      DIALOG *D = T->D;
      TX_STR *tx = T->tstr;
      int x1 , y1 , x2 , y2;
      int k = cell , i;
      int type = T->type;
      char *str;
      int ch;
      int curbox;
      int size , xsize , ysize , FontSize , sw;
      float th , tw , tg , xx , yy;
      int tfill , tclr;
      int rd , gr , bl;
      char Buf [ 2000 ] , stmp [ 10 ] ;
      void *fid , *img , *img2;
      float curpos;
      int ylng;
      kgWC *wc;
      kgDC *dc;
      wc = WC ( D ) ;
      elmt = tx->elmt;
      curbox = tx->row*tx->nx+tx->col;
      strcpy ( stmp , ( char * ) " " ) ;
      FontSize = T->FontSize;
      sw = elmt [ k ] .sw;
      x1 = elmt [ k ] .x1;
      y1 = elmt [ k ] .y1;
      x2 = elmt [ k ] .x2;
      y2 = elmt [ k ] .y2;
      tclr = tx->gc.tabl_char;
      tfill = tx->gc.tabl_fill;
      if ( sw == 0 ) tclr = tx->gc.tabl_hchar;
      kgGetDefaultRGB ( tclr , & rd , & gr , & bl ) ;
#if 1
      if ( type == 1 ) {
          y1 -= 2;
          y2 += 2;
      }
#endif
      size = get_t_item_size ( elmt [ k ] .fmt ) ;
      str = elmt [ k ] .df;
      strcpy ( Buf , str+elmt [ k ] .startchar ) ;
      Buf [ size+1 ] = '\0';
      ylng = strlen ( Buf ) *FontSize+FontSize*2;
      xsize = ( x2-x1 ) ;
      ysize = ( y2-y1 ) ;
#if 0
      fid = kgInitImage ( xsize+1 , ysize+1 , 1 ) ;
      kgUserFrame ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize ) ;
      kgBoxFill ( fid , 0.0 , 0.0 , ( float ) xsize , \
       ( float ) ysize , tfill , 0 ) ;
      th = FontSize*1.3;
      tw = FontSize+1;
      yy = 0.6*FontSize;
      if ( drcur ) {
          if ( elmt [ k ] .hlt ) {
              float hxs , hxe;
              hxs = elmt [ k ] .hxs;
              hxe = elmt [ k ] .hxe;
              if ( ( hxe-hxs ) > 2 ) {
                  kgBoxFill ( fid , hxs , yy-th*0.3 , hxe , th+yy , tx->gc.high_clr , 0 ) ;
              }
          }
          curpos = ( elmt [ k ] .cursor-elmt [ k ] .startchar ) *FontSize+FontSize/2;
          kgBoxFill ( fid , curpos , yy-th*0.3 , curpos+FontSize , \
               th+yy , D->gc.cur_clr , 0 ) ;
      }
//        img = kgGetResizedImage(fid);
      img = kgGetSharpImage ( fid ) ;
      kgCloseImage ( fid ) ;
#else 
      img = uiMakeTableCellImage ( T , cell , drcur ) ;
#endif
      img2 = elmt [ k ] .img;
      kgSetImageColor ( img2 , rd , gr , bl ) ;
      kgAddImages ( img , img2 , FontSize/2 , ysize-2*FontSize+1 ) ;
      kgImage ( D , img , x1 , y1+2 , xsize , ysize , 0.0 , 1.0 ) ;
      return 1;
  }
  void _ui_drawtablecursor ( TX_STR *tx ) {
      int curbox , cx , cy , ch , ln;
      char *df;
      DIALOG *D;
      DIT *T;
      T_ELMT *elmt;
      D = ( DIALOG * ) ( tx->D ) ;
      T = tx->T;
      elmt = tx->elmt;
      curbox = tx->row*tx->nx+tx->col;
      _uiPrintTableCell ( T , curbox , 1 ) ;
      return;
  }
  void _ui_updatetablecursor ( TX_STR *tx ) {
      int curbox ;
      DIT *T;
      T = tx->T;
      curbox = tx->row*tx->nx+tx->col;
      _uiUpdateTableCell ( T , curbox , 1 ) ;
//      _uiPrintTableCell ( T , curbox , 1 ) ;
  }
  void _ui_cleantablecursor ( TX_STR *tx ) {
      char *df;
      int curbox , cx , cy , ln , ch;
      T_ELMT *elmt;
      DIALOG *D;
      int EVGAY;
      DIT *T;
      D = ( DIALOG * ) ( tx->D ) ;
      T = tx->T;
      EVGAY = D->evgay;
//  tit= tx->tit;
      elmt = tx->elmt;
      curbox = tx->row*tx->nx+tx->col;
      elmt [ curbox ] .hlt = 0;
      _uiUpdateTableCell ( T , curbox , 0 ) ;
//      _uiPrintTableCell ( T , curbox , 0 ) ;
      return;
  }
  static int GetStrlen ( char *s ) {
      int i = 0;
      int ch;
      int ln = 0;
      while ( ( ch = s [ i ] ) != '\0' ) {
          if ( ch == '!' ) {
              i++;
              switch ( s [ i ] ) {
                  case '!': i++; ln++; break;
                  case 'g': i++; break;
                  case '\0': break;
                  default : i = i+3;
              }
          }
          else {ln++; i++; }
      }
      return ln;
  }
  static int Size ( char *t ) {
      int j = 0 , lng = 0;
//   lng =  strlen(t+j)*9;
      lng = GetStrlen ( t+j ) *9;
      lng = lng*1.0;
      if ( lng != 0 ) lng += ( 8 ) ;
      return ( lng ) ;
  }
  int _ui_drawtextbox ( DIALOG *D , DIT *T ) {
      TX_STR *tx;
      int n , ygap = 0 , xgap = 0 , k = 0 , i , j , l , x1 , x2 , \
           y1 , y2 , box_width = 20 , temp , prsize = 0 , x2max;
      int xx1 , yy1 , xx2 , yy2 , xlng = 0 , lng = 0 , gap , pointer , cy;
      int X2 , Y2 , X1 , Y1 , nx , ny , width;
      T_ELMT *elmt;
      int size , type , noecho;
      void *img;
      unsigned int tempc , tempf , tempt , tempff;
      kgWC *wc;
      kgDC *dc;
      wc = WC ( D ) ;
      if ( T->hide == 1 ) return 0;
      type = T->type;
      tx = T->tstr;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      tempc = D->gc.char_clr;
      X1 = tx->x1;
      Y1 = tx->y1;
      nx = tx->nx;
      ny = tx->ny;
//  tit = tx->tit;
      width = tx->width;
      box_width = width;
      elmt = tx->elmt;
      k = 0;
      for ( i = 0; i < nx; i++ ) {
          lng = 0;
          prsize = 0;
          for ( j = 0; j < ny; j++ ) {
              k = i+j*nx;
              gap = Size ( elmt [ k ] .pr ) ;
              if ( gap > prsize ) prsize = gap;
#if 0
              l = strlen ( elmt [ k ] .df ) ;
#else
// TCB as on 25/9/12 may need further checking
              size = get_t_item_size ( elmt [ k ] .fmt ) ;
              l = GetStrlen ( elmt [ k ] .df ) ;
              if ( l > size ) {
                  elmt [ k ] .startchar = l-size;
                  l = size;
              }
              else l = size;
// End of TCB as on 25/9/12 
#endif
//     tx->ln[k]=l+1;
              tx->elmt [ k ] .ln = l+1;
//     lng += (gap+Size (elmt[k].df));
              x2max = ( ( l+1 ) *9+13 ) ;
              if ( x2max > lng ) lng = x2max;
              elmt [ k ] .df [ MAXTITEMLN-1 ] = '\0';
              k++;
          }
          xlng += ( lng+prsize ) ;
      }
      xgap = 10;
      ygap = 10;
      X2 = X1+xlng+ ( nx-1 ) *xgap+xgap;
      Y2 = Y1+ny*width+ ( ny-1 ) *ygap+ygap;
      tx->x2 = X2;
      tx->y2 = Y2;
      xx1 = X1;
      xx2 = X2;
      yy1 = Y1;
      yy2 = Y2;
//  uidraw_inset(D,xx1-1,D->evgay-1-yy1+6,xx2+1,D->evgay-1-yy2-6);
      k = 0;
      x1 = xx1+xgap/2;
      if ( T->hide != 1 ) {
          for ( j = 0; j < nx; j++ ) {
              prsize = 0;
              for ( i = 0; i < ny; i++ ) {
                  gap = Size ( elmt [ j+i*nx ] .pr ) ;
                  if ( gap > prsize ) prsize = gap;
              }
              x2max = 0;
              y1 = yy1+ygap/2;
              for ( i = 0; i < tx->ny; i++ ) {
                  k = j+i*nx;
                  y2 = y1 + box_width;
//TCB
//     uiwrite_string(D,elmt[k].pr,x1,D->evgay-1-y2+6,tx->gc.txt_pchar);
//     prsize = (Size(elmt[k].pr));
                  uiString ( D , elmt [ k ] .pr , x1 , y1-2 , prsize-D->gc.FontSize , box_width , \
                       D->gc.PromptFont , tx->gc.txt_pchar , D->gc.FontSize , 1 , D->gc.fill_clr ) ;
                  x1 += prsize;
//     x2 = x1 + Size(elmt[k].df);
                  x2 = x1 + ( ( tx->elmt [ k ] .ln ) *9 ) +13;
                  if ( x2 > x2max ) x2max = x2;
                   ( tx->elmt [ k ] .x1 ) = x1;
                   ( tx->elmt [ k ] .y1 ) = y1;
                   ( tx->elmt [ k ] .x2 ) = x2;
                   ( tx->elmt [ k ] .y2 ) = y2;
                  noecho = tx->elmt [ k ] .noecho;
                  switch ( type ) {
                      case 1:
                      img = elmt [ k ] .img;
                      if ( img == NULL ) {
                          void *Bimg = NULL;
                          Bimg = kgGetBackground ( D , x1-1 , y1 , x2+4 , y2 ) ;
                          img = kgPressedRectangle ( x2-x1+2 , abs ( y2-y1 ) , tx->gc.txt_fill , 0.5 ) ;
                          Bimg = kgMergeImages ( Bimg , img , 0 , 0 ) ;
                          elmt [ k ] .img = Bimg;
                          kgFreeImage ( img ) ;
                          img = Bimg;
                      }
//         _dvrect_fill_transparent(WC(D),x1-2,y1,x2+2,y2,D->gc.fill_clr,D->transparency);
//         kgImage(D,img,x1-1,y1,x2-x1+5,y2-y1,0.0,1.0);
                      kgRestoreImage ( D , img , x1-1 , y1 , x2-x1+5 , y2-y1 ) ;
                      break;
                      default:
                      _uirect_fill ( wc , x1 , D->evgay-y1 , x2-1 , \
                           D->evgay-y2+1 , tx->gc.txt_fill ) ;
                      dvdraw_depre ( D , x1 , y1 , x2 , y2 ) ;
                      break;
                  }
#if 0
                  _uirect_fill ( wc , x1 , D->evgay-y1 , x2-1 , \
                       D->evgay-y2+1 , tx->gc.txt_fill ) ;
                  uidraw_depre ( D , x1 , D->evgay-y1 , x2 , D->evgay-y2 ) ;
#endif
                  cy = ( y2+y1 ) /2+6;
                  if ( noecho != 1 ) {
                      uiSetGuiFixFontSize ( D , D->gc.InputFontSize ) ;
                      _ui_putstring ( D , x1+6 , cy , elmt [ k ] .df+elmt [ k ] .startchar , \
                           tx->gc.txt_char ) ;
                  }
                  else {
                      char *stars , *str;
                      int ln , i;
                      uiSetNoechoFontSize ( D , D->gc.InputFontSize/2 ) ;
                      str = elmt [ k ] .df+elmt [ k ] .startchar;
                      ln = GetStrlen ( str ) +1;
                      stars = ( char * ) Malloc ( sizeof ( char ) *ln ) ;
                      for ( i = 0; i < ( ln-1 ) ; i++ ) {
                          if ( str [ i ] > ' ' ) stars [ i ] = 'l';
                          else stars [ i ] = ' ';
                      }
                      stars [ ln-1 ] = '\0';
                      _ui_putstring ( D , x1+7 , cy , stars , tx->gc.txt_char ) ;
                      free ( stars ) ;
                  }
                  uiDefaultGuiFontSize ( D ) ;
                  x1 -= prsize;
                  y1 = y2+ygap;
                  k++;
              }
              x1 = x2max+xgap;
          }
//  uidraw_inset(D,X1,D->evgay-1-Y1,X2,D->evgay-1-Y2);
          if ( T->bordr == 1 ) {
              X1 = D->xo+T->x1+2; Y1 = D->yo+T->y1+2;
              X2 = D->xo+T->x2-2; Y2 = D->yo+T->y2-2;
              _dv_draw_bound ( D , ( X1 ) , ( Y1 ) , ( X2 ) , ( Y2 ) , D->gc.high_clr ) ;
          }
      }
      else _dvrect_fill_transparent ( wc , ( x1 ) , ( y1 ) , \
       ( x2 ) , ( y2 ) , D->gc.fill_clr , D->transparency ) ;
      uiRest_clip_limits ( wc ) ;
      return 1;
  }
  int _uiMake_Tx ( DIT *T ) {
      DIALOG *D;
      TX_STR *tx;
      int n , ygap = 0 , xgap = 0 , k = 0 , i , j , l , x1 , x2 , \
           y1 , y2 , box_width = 20 , temp , prsize = 0 , x2max;
      int xx1 , yy1 , xx2 , yy2 , xlng = 0 , lng = 0 , gap , pointer;
      int X2 , Y2 , X1 , Y1 , nx , ny , width , cx , cy;
      T_ELMT *elmt;
      int size , type , noecho;
      unsigned int tempc , tempf , tempt , tempff;
      void *img;
      kgWC *wc;
      kgDC *dc;
      type = T->type;
      D = T->D;
      wc = WC ( D ) ;
      tx = T->tstr;
//  tit= tx->tit;
      elmt = T->elmt;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      tempc = D->gc.char_clr;
      n = T->nx*T->ny;
      for ( i = 0; i < n; i++ ) {
          elmt [ i ] .sw = 1;
          uimake_telmt ( elmt+i ) ;
          elmt [ i ] .cursor = 0;
          elmt [ i ] .startchar = 0;
          elmt [ i ] .hlt = 0;
      }
      X1 = T->x1+D->xo+2;
      Y1 = T->y1+D->yo+2;
      tx->x2 = X1;
      tx->y1 = Y1;
      tx->x1 = X1;
      tx->y2 = Y1;
      nx = tx->nx;
      ny = tx->ny;
//  tit = tx->tit;
      width = tx->width;
      box_width = width;
      elmt = tx->elmt;
      k = 0;
      for ( i = 0; i < nx; i++ ) {
          lng = 0;
          prsize = 0;
          for ( j = 0; j < ny; j++ ) {
              k = i+j*nx;
              gap = Size ( elmt [ k ] .pr ) ;
              if ( gap > prsize ) prsize = gap;
#if 0
              l = strlen ( elmt [ k ] .df ) ;
#else
// TCB as on 25/9/12 may need further checking
              size = get_t_item_size ( elmt [ k ] .fmt ) ;
              l = strlen ( elmt [ k ] .df ) ;
              if ( l > size ) {
                  elmt [ k ] .startchar = l-size;
                  elmt [ k ] .cursor = elmt [ k ] .startchar;
                  l = size;
              }
              else l = size;
// End of TCB as on 25/9/12 
#endif
              tx->elmt [ k ] .ln = l+1;
//     lng += (gap+Size (elmt[k].df));
              x2max = ( ( l+1 ) *9+13 ) ;
              if ( x2max > lng ) lng = x2max;
              elmt [ k ] .df [ MAXTITEMLN-1 ] = '\0';
              k++;
          }
          xlng += ( lng+prsize ) ;
      }
      xgap = 10;
      ygap = 10;
      X2 = X1+xlng+ ( nx-1 ) *xgap+xgap;
      Y2 = Y1+ny*width+ ( ny-1 ) *ygap+ygap;
      tx->x2 = X2;
      tx->y2 = Y2;
      xx1 = X1;
      xx2 = X2;
      yy1 = Y1;
      yy2 = Y2;
      k = 0;
      x1 = xx1+xgap/2;
      if ( T->hide != 1 ) {
#if 1
          CHECKLIMITS ( T ) ;
          if ( T->Bimg == NULL ) T->Bimg = kgGetBackground \
           ( D , ( X1 ) , ( Y1 ) , ( X2 ) , ( Y2 ) ) ;
          else kgRestoreImage ( D , T->Bimg , ( X1 ) , \
           ( Y1 ) , ( X2-X1+1 ) , ( Y2-Y1+1 ) ) ;
          for ( j = 0; j < nx; j++ ) {
              prsize = 0;
              tx->col = j;
              for ( i = 0; i < ny; i++ ) {
                  gap = Size ( elmt [ j+i*nx ] .pr ) ;
                  if ( gap > prsize ) prsize = gap;
              }
              x2max = 0;
              y1 = yy1+ygap/2;
              for ( i = 0; i < tx->ny; i++ ) {
                  tx->row = i;
                  k = j+i*nx;
                  y2 = y1 + box_width;
                  x1 += prsize;
                  x2 = x1 + ( ( tx->elmt [ k ] .ln ) *9 ) +13;
                  if ( x2 > x2max ) x2max = x2;
                   ( tx->elmt [ k ] .x1 ) = x1;
                   ( tx->elmt [ k ] .y1 ) = y1;
                   ( tx->elmt [ k ] .x2 ) = x2;
                   ( tx->elmt [ k ] .y2 ) = y2;
                  switch ( type ) {
                      case 1:
                      img = elmt [ k ] .img;
                      if ( img == NULL ) {
//           _uiMakeTextBoxImage(tx);
#ifdef D_TH
                          DoInAnyThread ( D->ThInfo , _uiMakeTextBoxImage , tx ) ;
#else
                          _uiMakeTextBoxImage ( tx ) ;
#endif
                      }
                      break;
                      default:
                      break;
                  }
                  WaitThreads ( D->ThInfo ) ;
                  x1 -= prsize;
                  y1 = y2+ygap;
                  k++;
              }
              x1 = x2max+xgap;
          }
          xx1 = X1;
          xx2 = X2;
          yy1 = Y1;
          yy2 = Y2;
          k = 0;
          x1 = xx1+xgap/2;
#endif
#if 0
          _dvrect_fill_transparent ( WC ( D ) , ( X1 ) , ( Y1 ) , \
           ( X2 ) , ( Y2 ) , D->gc.fill_clr , D->transparency ) ;
#endif
          tx->col = 0;
          tx->row = 0;
          for ( j = 0; j < nx; j++ ) {
              prsize = 0;
              for ( i = 0; i < ny; i++ ) {
                  gap = Size ( elmt [ j+i*nx ] .pr ) ;
                  if ( gap > prsize ) prsize = gap;
              }
              x2max = 0;
              y1 = yy1+ygap/2;
              for ( i = 0; i < tx->ny; i++ ) {
                  k = j+i*nx;
                  noecho = elmt [ k ] .noecho;
                  y2 = y1 + box_width;
//       uiString(D,elmt[k].pr,x1,y1,prsize-D->gc.FontSize,box_width,D->gc.PromptFont,tx->gc.txt_pchar,D->gc.FontSize,1,D->gc.fill_clr);
                  uiString ( D , elmt [ k ] .pr , x1 , y1 , prsize-D->gc.FontSize , box_width , \
                       D->gc.PromptFont , tx->gc.txt_pchar , D->gc.FontSize , 1 , -1 ) ;
                  x1 += prsize;
                  x2 = x1 + ( ( tx->elmt [ k ] .ln ) *9 ) +13;
                  if ( x2 > x2max ) x2max = x2;
                   ( tx->elmt [ k ] .x1 ) = x1;
                   ( tx->elmt [ k ] .y1 ) = y1;
                   ( tx->elmt [ k ] .x2 ) = x2;
                   ( tx->elmt [ k ] .y2 ) = y2;
                  switch ( type ) {
                      case 1:
                      img = elmt [ k ] .img;
                      if ( img == NULL ) {
                          void *Bimg = NULL;
                          Bimg = kgGetBackground ( D , x1-1 , y1 , x2+4 , y2 ) ;
                          img = kgPressedRectangle ( x2-x1+2 , abs ( y2-y1 ) , tx->gc.txt_fill , 0.5 ) ;
                          Bimg = kgMergeImages ( Bimg , img , 0 , 0 ) ;
                          elmt [ k ] .img = Bimg;
                          kgFreeImage ( img ) ;
                          img = Bimg;
                      }
//         kgImage(D,img,x1-1,y1,x2-x1+5,y2-y1,0.0,1.0);
                      kgRestoreImage ( D , img , x1-1 , y1 , x2-x1+5 , y2-y1 ) ;
                      break;
                      default:
                      _uirect_fill ( wc , x1 , D->evgay-y1 , x2-1 , \
                           D->evgay-y2+1 , tx->gc.txt_fill ) ;
                      dvdraw_depre ( D , x1 , y1 , x2 , y2 ) ;
                      break;
                  }
                  if ( elmt [ k ] .hlt ) {
                      int hxs , hxe;
                      hxs = elmt [ k ] .hxs+elmt [ k ] .x1;
                      hxe = elmt [ k ] .hxe+elmt [ k ] .x1;
                      cx = elmt [ k ] .x1+6;
                      if ( hxs < cx ) hxs = cx;
                      if ( hxe > ( elmt [ k ] .x2-3 ) ) hxe = elmt [ k ] .x2-3;
                      cy = ( elmt [ k ] .y2+elmt [ k ] .y1 ) /2+6;
                      cy = D->evgay-cy;
                      _uirect_fill ( wc , hxs , cy-Bt , hxe , cy+Ht , tx->gc.high_clr ) ;
                  }
                  if ( k == 0 ) {
                      cx = elmt [ k ] .x1+6+ ( elmt [ k ] .cursor-elmt [ k ] .startchar ) * \
                       ( Gap+Wd ) ;
                      cy = ( elmt [ k ] .y2+elmt [ k ] .y1 ) /2+6;
                      cy = D->evgay-cy;
                      _uirect_fill ( wc , cx+Gap+1 , cy-Bt , cx+Gap+1+Wd , \
                           cy+Ht , tx->gc.cur_clr ) ;
                  }
                  if ( noecho != 1 ) {
                      uiSetGuiFixFontSize ( D , D->gc.InputFontSize ) ;
                      _ui_putstring ( D , x1+6 , ( y1+y2 ) /2+6 , elmt [ k ] .df+elmt [ k ] .startchar , \
                           tx->gc.txt_char ) ;
                  }
                  else {
                      char *stars , *str;
                      int ln , i;
                      uiSetNoechoFontSize ( D , D->gc.InputFontSize/2 ) ;
                      str = elmt [ k ] .df+elmt [ k ] .startchar;
                      ln = strlen ( str ) +1;
                      stars = ( char * ) Malloc ( sizeof ( char ) *ln ) ;
                      for ( i = 0; i < ( ln-1 ) ; i++ ) {
                          if ( str [ i ] > ' ' ) stars [ i ] = 'l';
                          else stars [ i ] = ' ';
                      }
                      stars [ ln-1 ] = '\0';
                      _ui_putstring ( D , x1+7 , ( y1+y2 ) /2+6 , stars , tx->gc.txt_char ) ;
                      free ( stars ) ;
                  }
                  uiDefaultGuiFontSize ( D ) ;
                  x1 -= prsize;
                  y1 = y2+ygap;
                  k++;
              }
              x1 = x2max+xgap;
          }
          tx->col = 0;
          tx->row = 0;
          if ( T->bordr == 1 ) {
              X1 = D->xo+T->x1+2; Y1 = D->yo+T->y1+2;
              X2 = D->xo+T->x2-2; Y2 = D->yo+T->y2-2;
              _dv_draw_bound ( D , ( X1 ) , ( Y1 ) , ( X2 ) , ( Y2 ) , D->gc.high_clr ) ;
          }
      }
      else{
// _dvrect_fill_transparent(WC(D),(X1),(Y1),(X2), (Y2),D->gc.fill_clr,D->transparency);
          if ( T->Bimg != NULL ) kgRestoreImage ( D , T->Bimg , \
           ( X1 ) , ( Y1 ) , ( X2-X1+1 ) , ( Y2-Y1+1 ) ) ;
      }
      uiRest_clip_limits ( wc ) ;
      return 1;
  }
  int _ui_drawtablebox ( DIALOG *D , DIT *T ) {
      TX_STR *tx;
      int n , ygap = 0 , xgap = 0 , k = 0 , i , j , l , x1 , x2 , \
           y1 , y2 , box_width = 20 , temp , prsize = 0 , x2max;
      int xx1 , yy1 , xx2 , yy2 , xlng = 0 , lng = 0 , \
           gap , pointer , tx1 , ty1 , tx2 , ty2;
      int X2 , Y2 , X1 , Y1 , nx , ny , width;
      T_ELMT *elmt;
      int size;
      unsigned int tempc , tempf , tempt , tempff;
      kgWC *wc;
      kgDC *dc;
      wc = WC ( D ) ;
      tx = T->tstr;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      tempc = D->gc.char_clr;
      X1 = tx->x1;
      Y1 = tx->y1;
      nx = tx->nx;
      ny = tx->ny;
//  tit = tx->tit;
      width = tx->width;
      box_width = width;
      elmt = tx->elmt;
      k = 0;
      gap = 0;
      prsize = 0;
      for ( i = 0; i < nx; i++ ) {
          lng = 0;
          for ( j = 0; j < ny; j++ ) {
              k = i+j*nx;
#if 0
              l = strlen ( elmt [ k ] .df ) ;
#else
// TCB as on 25/9/12 may need further checking
              size = get_t_item_size ( elmt [ k ] .fmt ) ;
              l = strlen ( elmt [ k ] .df ) ;
              if ( l > size ) {
                  elmt [ k ] .startchar = l-size;
                  l = size;
              }
              else l = size;
// End of TCB as on 25/9/12 
#endif
              tx->elmt [ k ] .ln = l+1;
//     lng += (gap+Size (elmt[k].df));
              x2max = ( ( l+1 ) *9+13 ) ;
              if ( x2max > lng ) lng = x2max;
              elmt [ k ] .df [ MAXTITEMLN-1 ] = '\0';
              k++;
          }
          xlng += lng;
      }
//  X2= X1+xlng+nx*10;
//  Y2= Y1+ny*width+(ny-1)*10;
      X2 = X1+xlng;
      Y2 = Y1+ny*width;
      tx->x2 = X2;
      tx->y2 = Y2;
      xx1 = X1;
      xx2 = X2;
      yy1 = Y1;
      yy2 = Y2;
      ygap = 0;
      xgap = 0;
      k = 0;
      x1 = xx1+xgap/2;
      prsize = 0;
      gap = 0;
      for ( j = 0; j < nx; j++ ) {
          x2max = 0;
          y1 = yy1 ;
          for ( i = 0; i < tx->ny; i++ ) {
              k = j+i*nx;
              y2 = y1 + box_width;
              x2 = x1 + ( ( tx->elmt [ k ] .ln ) *9 ) +13;
//     if(x2 > x2max ) x2max=x2;
              tx1 = x1+1; ty1 = y1+1;
              tx2 = x2-1; ty2 = y2-1;
               ( tx->elmt [ k ] .x1 ) = tx1;
               ( tx->elmt [ k ] .y1 ) = ty1;
               ( tx->elmt [ k ] .x2 ) = tx2;
               ( tx->elmt [ k ] .y2 ) = ty2;
              _uirect_fill ( wc , tx1 , D->evgay-ty1 , tx2-1 , \
                   D->evgay-ty2 , tx->gc.tabl_fill ) ;
              uiSetGuiFixFontSize ( D , D->gc.InputFontSize ) ;
              if ( elmt [ k ] .sw == 0 ) _ui_putstring ( D , tx1+5 , ty2-4 , elmt [ k ] .df+elmt [ k ] .startchar , \
                   tx->gc.tabl_hchar ) ;
              else _ui_putstring ( D , tx1+5 , ty2-4 , elmt [ k ] .df+elmt [ k ] .startchar , \
                   tx->gc.tabl_char ) ;
              _ui_draw_bound ( ( D ) , x1 , D->evgay-y1 , \
                   x2 , D->evgay-y2 , tx->gc.tabl_line ) ;
              uiDefaultGuiFontSize ( D ) ;
              y1 = y2;
              k++;
          }
          x1 = x2+xgap;
      }
      uidraw_proj ( D , X1-2 , D->evgay-Y1+2 , X2+2 , D->evgay-Y2-2 ) ;
      uiRest_clip_limits ( wc ) ;
      return 1;
  }
  int _uiMake_Ta ( DIT *T ) {
      DIALOG *D;
      TX_STR *tx;
      int n , ygap = 0 , xgap = 0 , k = 0 , i , j , l , x1 , x2 , \
           y1 , y2 , box_width = 20 , temp , prsize = 0 , x2max;
      int xx1 , yy1 , xx2 , yy2 , xlng = 0 , lng = 0 , \
           gap , pointer , tx1 , ty1 , tx2 , ty2;
      int X2 , Y2 , X1 , Y1 , nx , ny , width , totwidth;
      T_ELMT *elmt;
      int size , Fz;
      unsigned int tempc , tempf , tempt , tempff;
      char *str;
      char FontFile [ 500 ] ;
      int type = T->type;
      int curpos = 0;
      int ch;
      int Fcount;
      kgWC *wc;
      kgDC *dc;
      void *fid;
      void *img;
      int xsize , ysize;
      D = T->D;
      wc = WC ( D ) ;
      tx = T->tstr;
      totwidth = T->y2 - T->y1;
      size = ( totwidth-4 ) /T->ny;
//      printf("Ny: %d w %d s %d:%d\n",T->ny,T->width,size,T->y2-T->y1);
//      printf("Table: %d %d %d %d %d %d\n",T->Font,T->FontSize,D->gc.tabl_char,
//              D->gc.tabl_hchar,D->gc.tabl_fill,D->gc.tabl_line);
      T->width = size;
      tx->width = T->width;
      if ( 2*T->FontSize > T->width ) T->FontSize = T->width/2;
      Fz = T->FontSize;
      if ( MonoList == NULL ) {
          uiAddFixedFonts ( ) ;
      }
      if ( FontList == NULL ) {
          uiAddFonts ( ) ;
      }
 //     Fcount = Dcount ( MonoList ) ;
      Fcount = Dcount ( FontList ) ;
      if ( Fcount > 0 ) {
          if ( Fcount == 1 ) T->Font = 0;
          else T->Font = T->Font%Fcount;
 //         strcpy ( FontFile , ( char * ) Drecord ( MonoList , T->Font ) ) ;
          strcpy ( FontFile , ( char * ) Drecord ( FontList , T->Font ) ) ;
          if ( tx->F.Imgs == NULL ) tx->F.Imgs = ( void * ) kgFixedFontChars \
           ( FontFile , T->FontSize ) ;
          else if ( ( T->Font != tx->F.fontno ) || ( T->FontSize != tx->F.Size ) ) {
              uiFreeImgStrs ( tx->F.Imgs ) ;
              tx->F.Imgs = ( void * ) kgFixedFontChars ( FontFile , T->FontSize ) ;
          }
          tx->F.code = 't';
//          tx->F.name = ( char * ) Drecord ( MonoList , T->Font ) ;
          tx->F.name = ( char * ) Drecord ( FontList , T->Font ) ;
      }
      else {tx->F.code = 'i';tx->F.name = NULL;}
      tx->F.fontno = T->Font;
      tx->F.Size = T->FontSize;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      if ( T->hide != 1 ) {
          CHECKLIMITS ( T ) ;
          BACKUPWIDGETAREA ( T ) ;
          tempc = D->gc.char_clr;
          X1 = T->x1+D->xo+4;
          Y1 = T->y1+D->yo+4;
          tx->x2 = X1;
          tx->y1 = Y1;
          tx->x1 = X1;
          tx->y2 = Y1;
          X1 = tx->x1;
          Y1 = tx->y1;
          nx = tx->nx;
          ny = tx->ny;
          width = tx->width;
          box_width = width;
          elmt = tx->elmt;
          k = 0;
          gap = 0;
          prsize = 0;
          tx->col = 0;
          tx->row = 0;
          n = tx->nx*tx->ny;
          for ( i = 0; i < n; i++ ) {
              uimake_telmt ( elmt+i ) ;
              elmt [ i ] .cursor = 0;
              elmt [ i ] .startchar = 0;
          }
          curpos = -1;
          for ( i = 0; i < n; i++ ) {
              if ( T->elmt [ i ] .sw == 1 ) {curpos = i; break;}
          }
          if ( curpos >= 0 ) {
              tx->col = curpos%tx->nx;
              tx->row = curpos/tx->nx;
          }
          for ( i = 0; i < nx; i++ ) {
              lng = 0;
              for ( j = 0; j < ny; j++ ) {
                  k = i+j*nx;
                  size = get_t_item_size ( elmt [ k ] .fmt ) ;
                  l = strlen ( elmt [ k ] .df ) ;
                  l = size;
                  tx->elmt [ k ] .ln = l+1;
                  x2max = ( ( l+1 ) *Fz+Fz+4 ) ;
                  if ( x2max > lng ) lng = x2max;
                  elmt [ k ] .df [ MAXTITEMLN-1 ] = '\0';
                  k++;
              }
              xlng += lng;
          }
          X2 = X1+xlng;
          Y2 = Y1+ny*width;
          tx->x2 = X2;
          tx->y2 = Y2;
          xx1 = X1;
          xx2 = X2;
          yy1 = Y1;
          yy2 = Y2;
          ygap = 0;
          xgap = 0;
          k = 0;
          x1 = xx1+xgap/2;
          prsize = 0;
          gap = 0;
          xsize = X2-X1+1;
          ysize = Y2-Y1+1;
          fid = kgInitImage ( xsize , ysize , 1 ) ;
          kgUserFrame ( fid , 0. , 0. , ( float ) xsize-1 , ( float ) ysize-1 ) ;
          kgBoxFill ( fid , 0.0 , 0.0 , ( float ) xsize -1 , \
           ( float ) ysize-1 , D->gc.tabl_line , 0 ) ;
//          _uibox_fill ( wc , X1 , D->evgay-Y1 , X2 , D->evgay-Y2 , D->gc.tabl_fill ) ;
//          _uibox_fill ( wc , X1 , D->evgay-Y1 , X2 , D->evgay-Y2 , D->gc.tabl_line ) ;
          for ( j = 0; j < nx; j++ ) {
              x2max = 0;
              y1 = yy1 ;
              for ( i = 0; i < tx->ny; i++ ) {
                  k = j+i*nx;
                  y2 = y1 + box_width;
                  x2 = x1 + ( ( tx->elmt [ k ] .ln ) *Fz ) +Fz+4;
                  tx1 = x1+1; ty1 = y1+1;
                  tx1 = x1; ty1 = y1;
                  tx2 = x2-1; ty2 = y2-1;
                  size = get_t_item_size ( elmt [ k ] .fmt ) ;
                   ( tx->elmt [ k ] .x1 ) = tx1;
                   ( tx->elmt [ k ] .y1 ) = ty1;
                   ( tx->elmt [ k ] .x2 ) = tx2;
                   ( tx->elmt [ k ] .y2 ) = ty2;
                  if ( type == 0 ) {
 //             			  _ui_draw_bound ( ( D ) , x1 , D->evgay-y1 , x2 , D->evgay-y2 , tx->gc.tabl_line ) ;
 //                     _uibox_fill ( wc , x1 , D->evgay-y1 , x2 , D->evgay-y2 , tx->gc.tabl_line ) ;
 //                     _uibox_fill ( wc , tx1 , D->evgay-ty1 , tx2 , D->evgay-ty2 , D->gc.tabl_fill ) ;
                      kgBoxFill ( fid , ( float ) tx1-X1 , ( float ) ty1-Y1 , ( float ) tx2-X1-1 , \
                           ( float ) ty2-Y1-1 , D->gc.tabl_fill , 0 ) ;
                  }
                  y1 = y2;
                  k++;
              }
              x1 = x2+xgap;
          }
          img = kgGetSharpImage ( fid ) ;
          kgCloseImage ( fid ) ;
          kgImage ( D , img , X1 , Y1 , xsize , ysize , 0.0 , 1.0 ) ;
          kgFreeImage ( img ) ;
          for ( j = 0; j < nx; j++ ) {
              for ( i = 0; i < tx->ny; i++ ) {
                  k = j+i*nx;
                  _uiPrintTableCell ( T , k , 0 ) ;
              }
          }
          if ( type == 0 ) uidraw_proj ( D , X1-2 , D->evgay-Y1+2 , \
               X2+2 , D->evgay-Y2-2 ) ;
      }
      else {
          RESTOREWIDGETAREA ( T ) ;
      }
      uiRest_clip_limits ( wc ) ;
      return 1;
  }
  int get_t_item_size ( char *cp ) {
      int size = 0;
      char *cpt;
      cpt = cp;
      while ( *cpt != '\0' ) {
          if ( ( int ) ( *cpt ) == '%' ) {
              cpt++;
              while ( isdigit ( *cpt ) ) {
                  size = size*10+ ( *cpt -'0' ) ;
                  cpt++;
              }
              size+= 1;
          }
          cpt++;
      }
      return size;
  }
#if 0
  void uimake_t_item ( char *cp , void *v , int sw , T_item *tit ) {
      int i , k = 0 , err = 0 , size = 0 , l;
      char *cpt;
      char *chpt; float *fpt; double *dpt; int *ipt; int *lpt;
      char dfmt [ 10 ] , ffmt [ 10 ] , sfmt [ 10 ] ;
      char prompt [ MAXTPRMTLN ] , field [ MAXTITEMLN ] ;
      char wrk [ 1000 ] ;
      double val;
      cpt = cp;
      k = 0;
      while ( *cpt != '\0' ) {
          if ( ( int ) ( *cpt ) == '%' ) {
              prompt [ k++ ] = '\0';
              cpt++;
              while ( isdigit ( *cpt ) ) {
                  size = size*10+ ( *cpt -'0' ) ;
                  cpt++;
              }
              size+= 1;
              switch ( * ( cpt ) ) {
                  case 'd':
                  lpt = ( int * ) v;
                  sprintf ( dfmt , "%%-%-dd" , size ) ;
                  sprintf ( wrk , dfmt , *lpt ) ;
                  wrk [ size ] = '\0';
                  strcpy ( field , wrk ) ;
                  break;
                  case 'f':
                  case 'g':
                  fpt = ( float * ) v;
                  val = *fpt;
                  sprintf ( ffmt , "%%-%-dg" , size ) ;
                  sprintf ( wrk , ffmt , val ) ;
                  wrk [ size ] = '\0';
                  strcpy ( field , wrk ) ;
                  break;
                  case 'F':
                  dpt = ( double * ) v;
                  sprintf ( dfmt , "%%-%-dlg" , size ) ;
                  sprintf ( wrk , dfmt , *dpt ) ;
                  wrk [ size ] = '\0';
                  strcpy ( field , wrk ) ;
                  break;
                  case 's':
                  chpt = ( char * ) v;
                  sprintf ( sfmt , "%%-%-ds" , size ) ;
#if 1
                  sprintf ( wrk , sfmt , chpt ) ;
// TCB as on 25/09/12 needs further checking
//      wrk[size]='\0';
                  wrk [ MAXTITEMLN-1 ] = '\0';
                  strcpy ( field , wrk ) ;
#else
/* old code */
                  chpt [ size-1 ] = '\0';
                  strncpy ( wrk , chpt , size-1 ) ;
                  wrk [ size-1 ] = '\0';
                  wrk [ size ] = '\0';
                  l = strlen ( wrk ) ;
      /*for(i=0;i<size;i++) if((wrk[i]<' ')||(wrk[i]>126))wrk[i]= ' ';*/
                  for ( i = l; i < size-1; i++ ) wrk [ i ] = ' ';
                  strcpy ( field , wrk ) ;
                  stripblnk ( chpt ) ;
#endif
                  break;
                  default:
                  normal ( ) ;
                  printf ( "Error: wrong code /%c' \n" , *cpt ) ;
                  exit ( 0 ) ;
              }
          }
          else prompt [ k++ ] = *cpt;
          cpt++;
      }
      prompt [ k ] = '\0';
//  tit->pr = (char *) malloc((strlen(prompt)+1)*sizeof(char));
//  tit->df = (char *) malloc((strlen(field)+1)*sizeof(char));
      strcpy ( tit->pr , prompt ) ;
      strcpy ( tit->df , field ) ;
      tit->df [ MAXTITEMLN-1 ] = '\0';
      tit->sw = sw;
      return;
  }
#endif
  void uimake_telmt ( T_ELMT *elmt ) {
      char *cp; void *v; int sw;
      int i , k = 0 , err = 0 , size = 0 , l;
      char *cpt;
      char *chpt; float *fpt; double *dpt; int *ipt; int *lpt;
      char dfmt [ 20 ] , ffmt [ 20 ] , sfmt [ 20 ] ;
      char prompt [ MAXTPRMTLN ] , field [ MAXTITEMLN ] ;
      char wrk [ 1000 ] ;
      double val;
      cp = elmt->fmt;
      v = elmt->v;
      sw = elmt->sw;
      cpt = cp;
      k = 0;
      while ( *cpt != '\0' ) {
          if ( ( int ) ( *cpt ) == '%' ) {
              prompt [ k++ ] = '\0';
              cpt++;
              while ( isdigit ( *cpt ) ) {
                  size = size*10+ ( *cpt -'0' ) ;
                  cpt++;
              }
              size+= 1;
              switch ( * ( cpt ) ) {
                  case 'd':
                  lpt = ( int * ) v;
                  sprintf ( dfmt , "%%-%-dd" , size ) ;
                  sprintf ( wrk , dfmt , *lpt ) ;
                  wrk [ size ] = '\0';
                  strcpy ( field , wrk ) ;
                  break;
                  case 'f':
                  case 'g':
                  fpt = ( float * ) v;
                  val = *fpt;
                  sprintf ( ffmt , "%%-%-dg" , size ) ;
                  sprintf ( wrk , ffmt , val ) ;
                  wrk [ size ] = '\0';
                  strcpy ( field , wrk ) ;
                  break;
                  case 'F':
                  dpt = ( double * ) v;
                  sprintf ( dfmt , "%%-%-dlg" , size ) ;
                  sprintf ( wrk , dfmt , *dpt ) ;
                  wrk [ size ] = '\0';
                  strcpy ( field , wrk ) ;
                  break;
                  case 's':
                  chpt = ( char * ) v;
                  sprintf ( sfmt , "%%-%-ds" , size ) ;
#if 1
//TCBTCB
                  sprintf ( wrk , sfmt , chpt ) ;
// TCB as on 25/09/12 needs further checking
//      wrk[size]='\0';
                  l = strlen ( chpt ) ;
                  if ( l > MAXTITEMLN-1 ) l = MAXTITEMLN-1;
                  wrk [ l ] = '\0';
                  strcpy ( field , wrk ) ;
#else
/* old code */
                  chpt [ size-1 ] = '\0';
                  strncpy ( wrk , chpt , size-1 ) ;
                  wrk [ size-1 ] = '\0';
                  wrk [ size ] = '\0';
                  l = strlen ( wrk ) ;
      /*for(i=0;i<size;i++) if((wrk[i]<' ')||(wrk[i]>126))wrk[i]= ' ';*/
                  for ( i = l; i < size-1; i++ ) wrk [ i ] = ' ';
                  strcpy ( field , wrk ) ;
                  stripblnk ( chpt ) ;
#endif
                  break;
                  default:
                  normal ( ) ;
                  printf ( "Error: wrong code /%c' \n" , *cpt ) ;
                  exit ( 0 ) ;
              }
          }
          else prompt [ k++ ] = *cpt;
          cpt++;
      }
      prompt [ k ] = '\0';
      strcpy ( elmt->pr , prompt ) ;
      strcpy ( elmt->df , field ) ;
      elmt->df [ MAXTITEMLN-1 ] = '\0';
      return;
  }
  int uiUpdateTextBox ( void *tmp , int no ) {
      DIA *d;
      DIT *T;
      DIALOG *D;
      TX_STR *tx;
      D = ( DIALOG * ) ( tmp ) ;
      T = D->d [ no ] .t;
      tx = T->tstr;
      return _ui_updatetextbox ( T ) ;
  }
  int _ui_updatetextbox ( void *tmp ) {
      DIT *T;
      TX_STR *tx;
      int n , ygap = 0 , xgap = 0 , k = 0 , i , j , l , \
           x1 , x2 , y1 , y2 , box_width = 20 , temp;
      int xx1 , yy1 , xx2 , yy2 , xlng = 0 , lng = 0 , gap , pointer , cy;
      int X2 , Y2 , X1 , Y1 , nx , ny , width , size;
      T_ELMT *elmt;
      int noecho , type;
      void *img;
      unsigned int tempc , tempf , tempt , tempff;
      DIALOG *D;
      kgWC *wc;
      T = ( DIT * ) tmp;
      tx = T->tstr;
      D = ( DIALOG * ) ( tx->D ) ;
      wc = WC ( D ) ;
      type = T->type;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      tempc = D->gc.char_clr;
      X1 = tx->x1;
      Y1 = tx->y1;
      nx = tx->nx;
      ny = tx->ny;
//  tit = tx->tit;
      elmt = tx->elmt;
      width = tx->width;
      n = tx->nx*tx->ny;
      for ( i = 0; i < n; i++ ) {
          elmt [ i ] .sw = 1;
//     uimake_t_item(elmt[i].fmt,elmt[i].v,1,tit+i);
          uimake_telmt ( elmt+i ) ;
          elmt [ i ] .cursor = 0;
          elmt [ i ] .startchar = 0;
// Added as on 19/Jul13 Check needed
#if 1
          size = get_t_item_size ( elmt [ k ] .fmt ) ;
          l = strlen ( elmt [ k ] .df ) ;
          if ( l > size ) {
              elmt [ k ] .startchar = l-size;
              l = size;
          }
          else l = size;
#endif
      }
      box_width = width;
      k = 0;
      for ( i = 0; i < tx->ny; i++ ) {
          for ( j = 0; j < nx; j++ ) {
              x1 = ( tx->elmt [ k ] .x1 ) ;
              y1 = ( tx->elmt [ k ] .y1 ) ;
              x2 = ( tx->elmt [ k ] .x2 ) ;
              y2 = ( tx->elmt [ k ] .y2 ) ;
              noecho = tx->elmt [ k ] .noecho;
              switch ( type ) {
                  case 1:
                  img = elmt [ k ] .img;
                  if ( img == NULL ) {
                      void *Bimg = NULL;
                      Bimg = kgGetBackground ( D , x1-1 , y1 , x2+4 , y2 ) ;
                      img = kgPressedRectangle ( x2-x1+2 , abs ( y2-y1 ) , tx->gc.txt_fill , 0.5 ) ;
                      Bimg = kgMergeImages ( Bimg , img , 0 , 0 ) ;
                      elmt [ k ] .img = Bimg;
                      kgFreeImage ( img ) ;
                      img = Bimg;
                  }
//         _dvrect_fill_transparent(WC(D),x1-2,y1,x2+2,y2,D->gc.fill_clr,D->transparency);
//         kgImage(D,img,x1-1,y1,x2-x1+5,y2-y1,0.0,1.0);
                  kgRestoreImage ( D , img , x1-1 , y1 , x2-x1+5 , y2-y1 ) ;
                  break;
                  default:
                  _uirect_fill ( wc , x1 , D->evgay-y1 , x2-1 , \
                       D->evgay-y2+1 , tx->gc.txt_fill ) ;
                  dvdraw_depre ( D , x1 , y1 , x2 , y2 ) ;
                  break;
              }
#if 0
              _uirect_fill ( wc , x1 , D->evgay-y1 , x2-1 , \
                   D->evgay-y2+1 , tx->gc.txt_fill ) ;
              uidraw_depre ( D , x1 , D->evgay-y1 , x2 , D->evgay-y2 ) ;
              uiSetGuiFixFontSize ( D , D->gc.FontSize ) ;
#endif
              cy = ( y1+y2 ) /2+6;
              if ( noecho != 1 ) {
                  uiSetGuiFixFontSize ( D , D->gc.InputFontSize ) ;
                  _ui_putstring ( D , x1+6 , cy , elmt [ k ] .df+elmt [ k ] .startchar , \
                       tx->gc.txt_char ) ;
              }
              else {
                  char *stars , *str;
                  int ln , i;
                  uiSetNoechoFontSize ( D , D->gc.InputFontSize/2 ) ;
                  str = elmt [ k ] .df+elmt [ k ] .startchar;
                  ln = strlen ( str ) +1;
                  stars = ( char * ) Malloc ( sizeof ( char ) *ln ) ;
                  for ( i = 0; i < ( ln-1 ) ; i++ ) {
                      if ( str [ i ] > ' ' ) stars [ i ] = 'l';
                      else stars [ i ] = ' ';
                  }
                  stars [ ln-1 ] = '\0';
                  _ui_putstring ( D , x1+7 , cy , stars , tx->gc.txt_char ) ;
                  free ( stars ) ;
              }
              uiDefaultGuiFontSize ( D ) ;
              k++;
          }
      }
      uiRest_clip_limits ( wc ) ;
      return 1;
  }
#if 0
  int read_t_item ( char *cp , void *v , T_item *tit ) {
      int ret = 0;
      int i , k = 0 , err = 0 , size = 0;
      char *cpt;
      char *chpt; float *fpt; double *dpt; int *ipt; int *lpt;
      char vchpt; float vfpt; double vdpt; int vipt; int vlpt;
      char dfmt [ 10 ] , ffmt [ 10 ] , sfmt [ 10 ] ;
      char prompt [ 100 ] , field [ 1000 ] ;
      char wrk [ 90 ] ;
      cpt = cp;
      k = 0;
      while ( *cpt != '\0' ) {
          if ( ( int ) ( *cpt ) == '%' ) {
              cpt++;
              while ( isdigit ( *cpt ) ) {
                  size = size*10+ ( *cpt -'0' ) ;
                  cpt++;
              }
              size+= 1;
              switch ( * ( cpt ) ) {
                  case 'd':
                  lpt = ( int * ) v;
                  vlpt = *lpt;
                  err = sscanf ( tit->df , "%d" , lpt ) ;
                  if ( err > 0 ) {
                      if ( ( ret == 0 ) && ( vlpt != ( *lpt ) ) ) ret = 1;
                  }
                  else ret = -1;
                  break;
                  case 'f':
                  case 'g':
                  fpt = ( float * ) v;
                  vfpt = *fpt;
                  err = sscanf ( tit->df , "%g" , fpt ) ;
                  if ( err > 0 ) {
                      if ( ( ret == 0 ) && ( vfpt != ( *fpt ) ) ) ret = 1;
                  }
                  else ret = -1;
                  break;
                  case 'F':
                  dpt = ( double * ) v;
                  vdpt = *dpt;
                  err = sscanf ( tit->df , "%lf" , dpt ) ;
                  if ( err > 0 ) {
                      if ( ( ret == 0 ) && ( vdpt != ( *dpt ) ) ) ret = 1;
                  }
                  else ret = -1;
                  break;
                  case 's':
                  chpt = ( char * ) v;
//TCB as on 25/09/12
//      tit->df[size-1]='\0';
                  i = 0;
                  while ( tit->df [ i ] >= ' ' ) i++;
                  tit->df [ i ] = '\0';
                  stripblnk ( tit->df ) ;
                  if ( strcmp ( chpt , tit->df ) != 0 ) {
                      strcpy ( chpt , tit->df ) ;
                      if ( ret == 0 ) ret = 1;
                  }
                  break;
                  default:
                  normal ( ) ;
                  printf ( "Error: wrong code / %c / \n" , *cpt ) ;
                  exit ( 0 ) ;
              }
          }
          cpt++;
      }
      return ( ret ) ;
  }
#endif
  int read_telmt ( T_ELMT *elmt ) {
      char *cp; void *v;
      int ret = 0;
      int i , k = 0 , err = 0 , size = 0;
      char *cpt;
      char *chpt; float *fpt; double *dpt; int *ipt; int *lpt;
      char vchpt; float vfpt; double vdpt; int vipt; int vlpt;
      char dfmt [ 10 ] , ffmt [ 10 ] , sfmt [ 10 ] ;
//  char prompt[100],field[1000];
      unsigned char wrk [ 2000 ] ;
      cp = elmt->fmt;
      v = elmt->v;
      cpt = cp;
      k = 0;
      while ( *cpt != '\0' ) {
          if ( ( int ) ( *cpt ) == '%' ) {
              cpt++;
              while ( isdigit ( *cpt ) ) {
                  size = size*10+ ( *cpt -'0' ) ;
                  cpt++;
              }
              size+= 1;
              switch ( * ( cpt ) ) {
                  case 'd':
                  lpt = ( int * ) v;
                  vlpt = *lpt;
                  err = sscanf ( elmt->df , "%d" , lpt ) ;
                  if ( err > 0 ) {
                      if ( ( ret == 0 ) && ( vlpt != ( *lpt ) ) ) ret = 1;
                  }
                  else ret = -1;
                  break;
                  case 'f':
                  case 'g':
                  fpt = ( float * ) v;
                  vfpt = *fpt;
                  err = sscanf ( elmt->df , "%g" , fpt ) ;
                  if ( err > 0 ) {
                      if ( ( ret == 0 ) && ( vfpt != ( *fpt ) ) ) ret = 1;
                  }
                  else ret = -1;
                  break;
                  case 'F':
                  dpt = ( double * ) v;
                  vdpt = *dpt;
                  err = sscanf ( elmt->df , "%lf" , dpt ) ;
                  if ( err > 0 ) {
                      if ( ( ret == 0 ) && ( vdpt != ( *dpt ) ) ) ret = 1;
                  }
                  else ret = -1;
                  break;
                  case 's':
                  chpt = ( char * ) v;
//TCB as on 25/09/12
//      elmt->df[size-1]='\0';
                  i = 0;
                  while ( ( elmt->df [ i ] >= ' ' ) || ( elmt->df [ i ] == '\t' ) ) {
                      if ( i == ( MAXTITEMLN-1 ) ) {elmt->df [ i ] = '\0'; break; }
                      i++;
                  }
                  elmt->df [ i ] = '\0';
// TCBTCB
//As on 24thFeb 2016 stripblnki(to remove end blank) is stopped 
//      stripblnk(elmt->df);
#if 1      
                  if ( strcmp ( chpt , elmt->df ) != 0 ) {
                      strcpy ( chpt , elmt->df ) ;
                      if ( ret == 0 ) ret = 1;
                  }
#else
                  strcpy ( wrk , elmt->df ) ;
                  i = strlen ( wrk ) -1;
                  if ( ( i > 0 ) && ( wrk [ i ] == ' ' ) ) {wrk [ i ] = '\0'; i--; }
                  while ( ( i >= 0 ) && ( ( wrk [ i ] < ' ' ) || ( wrk [ i ] > 127 ) ) ) \
                  {wrk [ i ] = '\0'; i--; }
                  if ( strcmp ( chpt , wrk ) != 0 ) {
                      strcpy ( chpt , wrk ) ;
                      if ( ret == 0 ) ret = 1;
                  }
#endif
                  break;
                  default:
                  normal ( ) ;
                  printf ( "Error: wrong code / %c / \n" , *cpt ) ;
                  exit ( 0 ) ;
              }
          }
          cpt++;
      }
      return ( ret ) ;
  }
  int _ui_readtextbox ( void *tmp ) {
      TX_STR *tx;
      T_ELMT *e;
      int i , n , err = 0 , ret = 0;
      tx = ( TX_STR * ) tmp;
      n = tx->nx*tx->ny;
      e = tx->elmt;
      for ( i = 0; i < n; i++ ) {
//     if((err=read_t_item(e[i].fmt,e[i].v,tx->tit+i))<0){
          if ( ( err = read_telmt ( e+i ) ) < 0 ) {
              ret = -1;
              break;
          }
          if ( ( err > 0 ) && ( ret == 0 ) ) ret = 1;
      }
      return ( ret ) ;
  }
  void _uiDrawTextBox ( DIALOG* D , int item ) {
      DIT *t;
      int X1 , Y1 , width , nx , ny;
      T_ELMT *elmt;
      DIALOG *Dtmp;
      int n , ygap = 0 , xgap = 0 , k = 0 , i , j , \
           x1 , x2 , y1 , y2 , box_width = 20 , temp;
      int xx1 , yy1 , xx2 , yy2 , xlng = 0 , lng = 0 , gap , ret , pointer;
      int X2 , Y2;
      int size , l;
      TX_STR *tx;
      t = D->d [ item ] .t;
      t->item = item;
      t->D = D;
      t->Bimg = NULL;
      Dtmp = ( DIALOG * ) ( D ) ;
      X1 = t->x1+D->xo+2;
      Y1 = t->y1+D->yo+2;
      nx = t->nx;
      ny = t->ny;
      width = t->width;
      elmt = t->elmt;
      box_width = width;
      n = nx* ny;
      for ( i = 0; i < n; i++ ) {
          elmt [ i ] .img = NULL;
          elmt [ i ] .hlt = 0;
      }
      tx = ( TX_STR * ) Malloc ( sizeof ( TX_STR ) ) ;
      tx->elmt = elmt;
      tx->F.Imgs = NULL;
//   tx->Nx=NULL;
      tx->nx = nx;
      tx->ny = ny;
      tx->width = width;
      tx->x2 = X1;
      tx->y1 = Y1;
      tx->x1 = X1;
      tx->y2 = Y1;
      tx->col = 0;
      if ( elmt [ tx->col ] .sw != 1 ) tx->col = tx->col+1;
#if 1
      if ( tx->col >= nx ) {
          printf ( "Error In Text Box: No input box\n" ) ;
          exit ( 0 ) ;
      }
#endif
      tx->row = 0;
      tx->gc = Dtmp->gc;
      tx->D = D;
      tx->T = t;
      t->tstr = tx;
      if ( t->hide != 1 ) _uiMake_Tx ( t ) ;
      return;
  }
  void _uiDrawTableBox ( DIALOG* D , int item ) {
      DIT *T;
      int X1 , Y1 , width , nx , ny;
      T_ELMT *elmt;
      DIALOG *Dtmp;
      int n , ygap = 0 , xgap = 0 , k = 0 , i , j , \
           x1 , x2 , y1 , y2 , box_width = 20 , temp;
      int xx1 , yy1 , xx2 , yy2 , xlng = 0 , lng = 0 , gap , ret , pointer;
      int X2 , Y2;
      int size , l;
      TX_STR *tx;
      T = D->d [ item ] .t;
      T->item = item;
      T->D = D;
      T->Bimg = NULL;
      Dtmp = ( DIALOG * ) ( D ) ;
      X1 = T->x1+D->xo+4;
      Y1 = T->y1+D->yo+4;
      nx = T->nx;
      ny = T->ny;
      width = T->width;
      elmt = T->elmt;
      box_width = width;
      n = nx* ny;
      for ( i = 0; i < n; i++ ) {
          elmt [ i ] .img = NULL;
      }
      tx = ( TX_STR * ) Malloc ( sizeof ( TX_STR ) ) ;
      tx->elmt = elmt;
      tx->F.Imgs = NULL;
//   tx->Nx=NULL;
      tx->x2 = X1;
      tx->y1 = Y1;
      tx->nx = nx;
      tx->ny = ny;
      tx->width = width;
      tx->x1 = X1;
      tx->y2 = Y1;
      tx->col = 0;
#if 0   //As of 8thAug24
      if ( elmt [ tx->col ] .sw != 1 ) tx->col = tx->col+1;
      if ( tx->col >= nx ) {
          printf ( "Error In Text Box: No input box\n" ) ;
          exit ( 0 ) ;
      }
#endif
      tx->row = 0;
      tx->gc = D->gc;
      tx->D = D;
      T->tstr = tx;
      tx->T = T;
      if ( T->hide != 1 ) _uiMake_Ta ( T ) ;
      return;
  }
  void _uiDrawHoriButtons ( DIALOG* D , int item ) {
      int ret , lngth;
      int Hsize;
      int x1; int y1;
      BUTS *buts;
      x1 = D->xo;
      y1 = D->yo;
      lngth = D->Hsize+2;
      DILN *b;
      b = D->d [ item ] .H;
      b->D = D;
      b->item = item;
      b->Bimg = NULL;
      if ( b->df == NULL ) { b->df = ( int * ) ( & ( b->bval ) ) ; }
      b->buts = make_but_str ( D , b->x1+x1+lngth , b->y1+y1+lngth , b->xgap , \
           b->ygap , b->nx , b->ny , b->lngth , b->width , * \
       ( b->df ) , b->titles , b->xpm , b->bkgr , b->sw ) ;
      buts = ( BUTS * ) ( b->buts ) ;
      buts->D = D;
      buts->Hsize = D->Hsize;
      buts->type = b->type;
      buts->fac = b->fac;
      buts->bordr = 0;
      buts->Widget = b;
      if ( b->hide != 1 ) _uiMake_H ( b ) ;
      return;
  }
  void _uiMakeButs ( DIN *B ) {
      BUT_STR *butns;
      int offset = 4;
      float transparency;
      int n , i , j , k , ix , iy , nx , ny , type , mf = 1 , mfp = 0;
      int x1 , y1 , x2 , y2 , xgap , ygap , ln , wd , \
           bkgr , highli = 1 , xo , yo , dx , dy;
      int rd , gr , bl;
      void *xpm , *timg = NULL;
      void *cimgn , *cimgp , *cimgh;
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) ( B->D ) ;
      wc = WC ( D ) ;
      nx = B->nx;
      ny = B->ny;
      n = nx*ny;
      transparency = D->transparency;
      butns = B->buts;
      xgap = B->xgap;
      ygap = B->ygap;
      ln = B->lngth;
      wd = B->width;
      type = B->type;
      if ( butns == NULL ) return;
      i = 0;
      if ( ( type == 0 ) || ( type == 9 ) ) {
          mf = 0;
          mfp = 3;
//        xo = D->xo+B->x1+offset+xgap/2;
          xo = D->xo+B->x1+offset;
          yo = D->yo+B->y1+offset;
      }
      else {
          mf = 1;
          mfp = 1;
          xo = D->xo+B->x1+offset;
          yo = D->yo+B->y1+offset;
      }
//  dx =ln+xgap;
//  dy =wd+ygap;
      dx = ln;
      dy = wd;
      bkgr = 0;
//      cimgn = uiMakeButtonImage(D,ln+mfp , wd+mfp ,bkgr,0,0 , B->fac , B->type);
//      cimgp = uiMakeButtonImage(D,ln+mfp , wd+mfp ,bkgr,-1,0 , B->fac ,  B->type);
      y1 = yo-dy-2-ygap+mf*ygap/2;
      for ( j = 0; j < ny; j++ ) {
          y1 = y1+dy+ygap;
          y2 = y1+dy+3;
          x1 = xo -dx-2-xgap+xgap/2;
          for ( k = 0; k < nx; k++ ) {
              butns [ i ] .D = B->D;
              butns [ i ] .Widget = B;
              butns [ i ] .state = 0;
              butns [ i ] .highli = 0;
              if ( * ( B->df ) == ( i+1 ) ) butns [ i ] .highli = 1;
              if ( D->butattn == 0 ) {butns [ i ] .highli = 0; highli = 0; }
              x1 = x1+dx+xgap;
              x2 = x1+dx+3;
              butns [ i ] .x1 = x1;
              butns [ i ] .x2 = x2;
              butns [ i ] .y1 = y1;
              butns [ i ] .y2 = y2;
              butns [ i ] .butno = i;
              bkgr = butns [ i ] .bkgr;
              if ( bkgr < 0 ) {
                  if ( bkgr == -1 ) bkgr = D->gc.fill_clr;
              }
              xpm = butns [ i ] .xpmn;
              butns [ i ] .imgn = uiMakeStringImage ( D , butns [ i ] .title , xpm , x1 , \
                   y1 , ln+mfp , wd+mfp , D->gc.ButtonFont , bkgr , 0 , D->gc.but_char , \
                   D->gc.GuiFontSize , 0 , B->fac , 0 , B->type ) ;
              if ( butns [ i ] .xpmp != NULL ) xpm = butns [ i ] .xpmp ;
              else {
                  timg = kgGetImageCopy ( NULL , butns [ i ] .xpmn ) ;
                  xpm = kgChangeBrightness ( timg , 0.5 ) ;
              }
              butns [ i ] .imgp = uiMakeStringImage ( D , butns [ i ] .title , xpm , x1 , \
                   y1 , ln+mfp , wd+mfp , D->gc.ButtonFont , bkgr , 0 , D->gc.but_char , \
                   D->gc.GuiFontSize , 0 , B->fac , -1 , B->type ) ;
              kgFreeImage ( timg ) ; timg = NULL;
              xpm = butns [ i ] .xpmn;
              if ( butns [ i ] .xpmh != NULL ) xpm = butns [ i ] .xpmh ;
              else {
                  timg = kgGetImageCopy ( NULL , butns [ i ] .xpmn ) ;
                  xpm = kgChangeBrightness ( timg , 1.2 ) ;
              }
              butns [ i ] .imgh = uiMakeStringImage ( D , butns [ i ] .title , xpm , x1 , \
                   y1 , ln+mfp , wd+mfp , D->gc.ButtonFont , bkgr , highli , D->gc.but_char , \
                   D->gc.GuiFontSize , 0 , B->fac , 0 , B->type ) ;
              kgFreeImage ( timg ) ; timg = NULL;
              i++;
          }
      }
  }
  void _uiInitButs_org ( DIN *B ) {
      BUT_STR *butns;
      int offset = 4;
      float transparency;
      int n , i , j , k , ix , iy , nx , ny , type , mf = 1 , mfp = 0;
      int x1 , y1 , x2 , y2 , xgap , ygap , ln , wd , \
           bkgr , highli = 1 , xo , yo , dx , dy;
      void *xpm , *timg = NULL;
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) ( B->D ) ;
      wc = WC ( D ) ;
      nx = B->nx;
      ny = B->ny;
      n = nx*ny;
      transparency = D->transparency;
      butns = B->buts;
      xgap = B->xgap;
      ygap = B->ygap;
      ln = B->lngth;
      wd = B->width;
      type = B->type;
      if ( butns == NULL ) return;
      i = 0;
      if ( ( type == 0 ) || ( type == 9 ) ) {
          mf = 0;
          mfp = 3;
//        xo = D->xo+B->x1+offset+xgap/2;
          xo = D->xo+B->x1+offset;
          yo = D->yo+B->y1+offset;
      }
      else {
          mf = 1;
          mfp = 1;
          xo = D->xo+B->x1+offset;
          yo = D->yo+B->y1+offset;
      }
//  dx =ln+xgap;
//  dy =wd+ygap;
      dx = ln;
      dy = wd;
      y1 = yo-dy-2-ygap+mf*ygap/2;
      for ( j = 0; j < ny; j++ ) {
          y1 = y1+dy+ygap;
          y2 = y1+dy+3;
          x1 = xo -dx-2-xgap+xgap/2;
          for ( k = 0; k < nx; k++ ) {
              butns [ i ] .D = B->D;
              butns [ i ] .Widget = B;
              butns [ i ] .state = 0;
              butns [ i ] .highli = 0;
              if ( * ( B->df ) == ( i+1 ) ) butns [ i ] .highli = 1;
              if ( D->butattn == 0 ) {butns [ i ] .highli = 0; highli = 0; }
              x1 = x1+dx+xgap;
              x2 = x1+dx+3;
              butns [ i ] .x1 = x1;
              butns [ i ] .x2 = x2;
              butns [ i ] .y1 = y1;
              butns [ i ] .y2 = y2;
              butns [ i ] .butno = i;
              bkgr = butns [ i ] .bkgr;
              if ( bkgr < 0 ) {
                  if ( bkgr == -1 ) bkgr = D->gc.fill_clr;
              }
              i++;
          }
      }
  }
// TCB 0n 07/21
  void _uiInitButs ( DIN *B ) {
      BUT_STR *butns;
      int offset = 3;
      float transparency;
      int n , i , j , k , ix , iy , nx , ny , type , mf = 1 , mfp = 0;
      int x1 , y1 , x2 , y2 , xgap , ygap , ln , wd , \
           bkgr , highli = 1 , xo , yo , dx , dy;
      void *xpm , *timg = NULL;
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) ( B->D ) ;
      wc = WC ( D ) ;
      nx = B->nx;
      ny = B->ny;
      n = nx*ny;
      transparency = D->transparency;
      butns = B->buts;
      xgap = B->xgap;
      ygap = B->ygap;
      ln = B->lngth;
      wd = B->width;
      type = B->type;
      if ( butns == NULL ) return;
      if ( ( type == 0 ) || ( type == 9 ) ) {
          mf = 0;
          mfp = 1;
      }
      else {
          mf = 1;
          mfp = 0;
      }
      xo = D->xo+B->x1+offset;
      yo = D->yo+B->y1+offset;
      dx = ln;
      dy = wd;
      y1 = yo-dy-ygap+mf*ygap/2;
      i = 0;
      for ( j = 0; j < ny; j++ ) {
          y1 = y1+dy+ygap;
//    y2 = y1+dy-1+mfp;
          y2 = y1+dy+mfp;
          x1 = xo -dx-xgap+xgap/2;
          for ( k = 0; k < nx; k++ ) {
              butns [ i ] .D = B->D;
              butns [ i ] .Widget = B;
              butns [ i ] .state = 0;
              butns [ i ] .highli = 0;
              if ( * ( B->df ) == ( i+1 ) ) butns [ i ] .highli = 1;
              if ( D->butattn == 0 ) {butns [ i ] .highli = 0; highli = 0; }
              x1 = x1+dx+xgap;
//      x2 = x1+dx-1+mfp;
              x2 = x1+dx+mfp;
              butns [ i ] .x1 = x1;
              butns [ i ] .x2 = x2;
              butns [ i ] .y1 = y1;
              butns [ i ] .y2 = y2;
              butns [ i ] .butno = i;
              bkgr = butns [ i ] .bkgr;
              if ( bkgr < 0 ) {
                  if ( bkgr == -1 ) bkgr = D->gc.fill_clr;
              }
              i++;
          }
      }
  }
//void *_uiMakeButnImages(DIN *B,int butno) {
  void *_uiMakeButnImages_n ( void *buttmp ) {
/* An effort to use store background
   But doe not work and lot of side effects
*/
      DIN *B;
      BUT_STR *butns;
      int offset = 4;
      int butno;
      float transparency;
      int n , i , j , k , ix , iy , nx , ny , type , mf = 1 , mfp = 0;
      int x1 , y1 , x2 , y2 , xgap , ygap , ln , wd , \
           bkgr , highli = 1 , xo , yo , dx , dy;
      void *xpm , *timg = NULL;
      DIALOG *D;
      kgWC *wc;
      void *Bkimg = NULL , *Bcopy = NULL;
      butns = ( BUT_STR * ) buttmp;
      B = ( DIN * ) ( butns->Widget ) ;
      butno = butns->butno;
      D = ( DIALOG * ) ( B->D ) ;
      wc = WC ( D ) ;
      nx = B->nx;
      ny = B->ny;
      n = nx*ny;
      transparency = D->transparency;
      butns = B->buts;
      xgap = B->xgap;
      ygap = B->ygap;
      ln = B->lngth;
      wd = B->width;
      type = B->type;
      if ( butns == NULL ) return NULL;
      if ( ( type == 0 ) || ( type == 9 ) ) {
          mf = 0;
          mfp = 3;
      }
      else {
          mf = 1;
          mfp = 1;
      }
      i = butno;
      x1 = butns [ i ] .x1;
      y1 = butns [ i ] .y1;
      x2 = butns [ i ] .x2;
      y2 = butns [ i ] .y2;
      Bkimg = kgGetBackground ( D , x1 , y1 , x2 , y2 ) ;
      xpm = kgGetImageCopy ( D , butns [ i ] .xpmn ) ;
      Bcopy = kgGetImageCopy ( NULL , Bkimg ) ;
      Bcopy = kgMergeImages ( Bcopy , xpm , 0 , 0 ) ;
      bkgr = butns [ i ] .bkgr;
      if ( bkgr < 0 ) {
          if ( bkgr == -1 ) bkgr = D->gc.fill_clr;
      }
      butns [ i ] .imgn = uiMakeStringImage ( D , butns [ i ] .title , Bcopy , \
           x1 , y1 , ln+mfp , wd+mfp , D->gc.ButtonFont , bkgr , 0 , D->gc.but_char , \
           D->gc.GuiFontSize , 0 , B->fac , 0 , B->type ) ;
      kgFreeImage ( Bcopy ) ; Bcopy = NULL;
      if ( butns [ i ] .xpmp != NULL ) timg = kgGetImageCopy \
       ( D , butns [ i ] .xpmp ) ;
      else {
          timg = kgGetImageCopy ( NULL , xpm ) ;
          timg = kgChangeBrightness ( timg , 0.5 ) ;
      }
      Bcopy = kgGetImageCopy ( NULL , Bkimg ) ;
      Bcopy = kgMergeImages ( Bcopy , timg , 0 , 0 ) ;
      butns [ i ] .imgp = uiMakeStringImage ( D , butns [ i ] .title , Bcopy , \
           x1 , y1 , ln+mfp , wd+mfp , D->gc.ButtonFont , bkgr , 0 , D->gc.but_char , \
           D->gc.GuiFontSize , 0 , B->fac , -1 , B->type ) ;
      kgFreeImage ( Bcopy ) ; Bcopy = NULL;
      kgFreeImage ( timg ) ; timg = NULL;
      if ( butns [ i ] .xpmh != NULL ) timg = kgGetImageCopy \
       ( D , butns [ i ] .xpmh ) ;
      else {
          timg = kgGetImageCopy ( NULL , xpm ) ;
          timg = kgChangeBrightness ( timg , 1.2 ) ;
      }
      Bcopy = kgGetImageCopy ( NULL , Bkimg ) ;
      Bcopy = kgMergeImages ( Bcopy , timg , 0 , 0 ) ;
      butns [ i ] .imgh = uiMakeStringImage ( D , butns [ i ] .title , Bcopy , \
           x1 , y1 , ln+mfp , wd+mfp , D->gc.ButtonFont , bkgr , highli , D->gc.but_char , \
           D->gc.GuiFontSize , 0 , B->fac , 0 , B->type ) ;
      kgFreeImage ( Bcopy ) ; Bcopy = NULL;
      kgFreeImage ( timg ) ; timg = NULL;
      kgFreeImage ( xpm ) ; xpm = NULL;
      kgFreeImage ( Bkimg ) ;
      return NULL;
  }
  void *_uiMakeButnImages ( void *buttmp ) {
      DIN *B;
      BUT_STR *butns;
      int offset = 4;
      int butno;
      float transparency;
      int n , i , j , k , ix , iy , nx , ny , type , mf = 1 , mfp = 0;
      int x1 , y1 , x2 , y2 , xgap , ygap , ln , wd , \
           bkgr , highli = 1 , xo , yo , dx , dy;
      void *xpm , *timg = NULL;
      DIALOG *D;
      kgWC *wc;
      void *Bkimg = NULL , *Bcopy = NULL;
      butns = ( BUT_STR * ) buttmp;
      B = ( DIN * ) ( butns->Widget ) ;
      butno = butns->butno;
      D = ( DIALOG * ) ( B->D ) ;
      wc = WC ( D ) ;
      nx = B->nx;
      ny = B->ny;
      n = nx*ny;
      transparency = D->transparency;
      butns = B->buts;
      xgap = B->xgap;
      ygap = B->ygap;
#if 0  // TCB 07/21 may have side effects
      ln = ( ( B->lngth-1 ) /2 ) *2-1;
      wd = ( ( B->width-1 ) /2 ) *2-1;
#else
      ln = B->lngth;
      wd = B->width;
#endif
// mfp can be adjusted to adjust the size of drawing
      type = B->type;
      if ( butns == NULL ) return NULL;
      if ( type == 10 ) return NULL;
      if ( ( type == 0 ) || ( type == 9 ) ) {
          mf = 0;
          mfp = 2;
      }
      else {
          mf = 1;
          mfp = 1;
      }
      if ( type == 1 ) mfp = 0;
      i = butno;
      x1 = butns [ i ] .x1;
      y1 = butns [ i ] .y1;
      x2 = butns [ i ] .x2;
      y2 = butns [ i ] .y2;
      xpm = butns [ i ] .xpmn;
      bkgr = butns [ i ] .bkgr;
      if ( bkgr < 0 ) {
          if ( bkgr == -1 ) bkgr = D->gc.fill_clr;
      }
      butns [ i ] .imgn = uiMakeStringImage ( D , butns [ i ] .title , xpm , x1 , \
           y1 , ln+mfp , wd+mfp , D->gc.ButtonFont , bkgr , 0 , D->gc.but_char , \
           D->gc.GuiFontSize , 0 , B->fac , 0 , B->type ) ;
      if ( butns [ i ] .xpmp != NULL ) xpm = butns [ i ] .xpmp ;
      else {
          timg = kgGetImageCopy ( NULL , butns [ i ] .xpmn ) ;
          xpm = kgChangeBrightness ( timg , 0.5 ) ;
      }
      butns [ i ] .imgp = uiMakeStringImage ( D , butns [ i ] .title , xpm , x1 , \
           y1 , ln+mfp , wd+mfp , D->gc.ButtonFont , bkgr , 0 , D->gc.but_char , \
           D->gc.GuiFontSize , 0 , B->fac , -1 , B->type ) ;
      kgFreeImage ( timg ) ; timg = NULL;
      if ( butns [ i ] .xpmh != NULL ) xpm = butns [ i ] .xpmh ;
      else {
          timg = kgGetImageCopy ( NULL , butns [ i ] .xpmn ) ;
          xpm = kgChangeBrightness ( timg , 1.2 ) ;
      }
      butns [ i ] .imgh = uiMakeStringImage ( D , butns [ i ] .title , xpm , x1 , \
           y1 , ln+mfp , wd+mfp , D->gc.ButtonFont , bkgr , highli , D->gc.but_char , \
           D->gc.GuiFontSize , 0 , B->fac , 0 , B->type ) ;
      kgFreeImage ( timg ) ; timg = NULL;
      return NULL;
  }
  void _uiUpdateType0Buts ( DIN *B ) {
      BUT_STR *butns;
      int offset = 4;
      float transparency;
      int n , i , j , k , ix , iy , nx , ny;
      int x1 , y1 , x2 , y2 , xgap , ygap , ln , wd , bkgr , highli = 1 , width;
      void *xpm;
      DIALOG *D;
      kgWC *wc;
      if ( B->type != 0 ) return;
      D = ( DIALOG * ) ( B->D ) ;
      wc = WC ( D ) ;
      nx = B->nx;
      ny = B->ny;
      n = nx*ny;
      transparency = D->transparency;
      butns = B->buts;
      xgap = B->xgap;
      ygap = B->ygap;
//TCB 07/21
      if ( ygap < 5 ) return;
      ln = B->lngth;
      wd = B->width;
      if ( butns == NULL ) return;
      i = 0;
      for ( j = 0; j < ny; j++ ) {
          for ( k = 0; k < nx; k++ ) {
//TCB 07/21
              if ( ( butns [ i ] .title == NULL ) || ( butns [ i ] .title [ 0 ] == '\0' ) ) continue;
                  
              x1 = butns [ i ] .x1+2-xgap/2;
              x2 = butns [ i ] .x2+xgap/2-2;
//      y1 =butns[i].y1+ygap/2+5+wd;
//              if ( ygap > 18 ) ygap = 18;
              y1 = butns [ i ] .y1+ygap+wd+3;
              y2 = butns [ i ] .y2;
              y1 = butns [ i ] .y2;
              y2 = y1+ygap;
              width = ln+xgap;
              bkgr = butns [ i ] .bkgr;
              if ( bkgr < 0 ) {
                  if ( bkgr == -1 ) bkgr = D->gc.fill_clr;
              }
              if ( butns [ i ] .Bimg0 == NULL ) {
                  butns [ i ] .Bimg0 = kgGetBackground ( D , x1 , y1 , x2 , y2 ) ;
              }
              else kgRestoreImage ( D , butns [ i ] .Bimg0 , x1 , y1 , \
               ( x2-x1+1 ) , ( y2-y1+1 ) ) ;
              uiString ( D , butns [ i ] .title , x1 , y1 , width , ygap-1 , D->gc.ButtonFont , \
                   D->gc.but_char , D->gc.GuiFontSize , 0 , -1 ) ;
              i++;
          }
      }
  }
  void _uiDrawB ( DIALOG* D , int item ) {
      DIBN *b;
      DIB *B;
      DIN *N;
      int x1; int y1;
      int Hsize;
      int ret , n = 0 , lngth , i = 0;
      BUTS *buts;
      int *sw = NULL;
      char *prompt = NULL;
      b = D->d [ item ] .n;
      b->D = D;
      b->item = item;
      b->Bimg = NULL;
      Hsize = D->Hsize;
      x1 = D->xo;
      y1 = D->yo;
      switch ( b->code ) {
          case 'N':
          prompt = ( char * ) b->prompt;
          while ( ( b->prompt [ i ] ) == ' ' ) i++;
          while ( ( b->prompt [ i+n ] ) != '\0' ) n++;
          if ( n != 0 ) n++;
          lngth = n*9+Hsize+2;
          b->buts = make_but_str ( D , b->x1+x1+lngth , b->y1+y1+Hsize+2 , \
               b->xgap , b->ygap , b->nx , b->ny , b->lngth , b->width , * \
           ( b->df ) , b->titles , b->xpm , b->bkgr , sw ) ;
          buts = ( BUTS * ) ( b->buts ) ;
          buts->D = D;
          buts->Hsize = D->Hsize;
          buts->type = b->type;
          buts->fac = b->fac;
          buts->bordr = b->bordr;
          buts->Widget = b;
          break;
          case 'b':
          B = ( DIB * ) b;
          sw = ( int * ) ( B->sw ) ;
          if ( B->df == NULL ) { B->df = ( int * ) ( & ( B->bval ) ) ; }
          B->buts = NULL;
          break;
          case 'n':
          printf ( "Case n:\n" ) ;
          N = ( DIN * ) b;
          if ( B->df == NULL ) { B->df = ( int * ) ( & ( B->bval ) ) ; }
          _uiMakeButs ( N ) ;
          break;
          default:
          break;
      }
      if ( b->hide != 1 ) _uiMake_B ( b ) ;
      return;
  }
  void _uiDrawN ( DIALOG* D , int item ) {
/* used for DIL also */
      DIN *b;
      int Hsize;
      int ret , n = 0 , lngth , i = 0 , k;
      BUT_STR *buts;
      b = D->d [ item ] .N;
      b->D = D;
      b->item = item;
      b->Bimg = NULL;
      Hsize = D->Hsize;
      buts = ( BUT_STR * ) b->buts;
      if ( buts == NULL ) return;
      b->df = ( int * ) ( & ( b->bval ) ) ;
      n = b->nx*b->ny;
      for ( k = 0; k < n; k++ ) {
          buts [ k ] .imgn = NULL;
          buts [ k ] .imgp = NULL;
          buts [ k ] .imgh = NULL;
          buts [ k ] .Bimg = NULL;
          buts [ k ] .Bimg0 = NULL;
      }
#if 0
      if ( b->hide != 1 ) _uiMake_N ( b ) ;
#else
      _uiMake_N ( b ) ;
#endif
      return;
  }
  int _ui_processslidepress ( S_STR *pt , KBEVENT kbe ) {
      int mvnt = 0 , xx , yy , l , w , count = 0;
      int i , j , k = 0 , scr_ln = 0;
      double fac = 1. , sx = 0. , val , df , ret = 0;
      char buf [ 30 ] ;
      DID *d;
      d = ( DID * ) uiGetWidget ( pt->D , pt->id ) ;
      xx = pt->x;
      yy = pt->y;
      scr_ln = pt->l;
      w = pt->Sldwd;
      l = scr_ln+2*pt->Sldwd+6;
      fac = pt->fac;
      sx = ( pt->df- ( pt->min ) ) *fac;
      mvnt = 0;
      if ( _uiGetSItem ( kbe , xx+3 , yy+3 , xx+w-3 , yy+w-3 ) ) { mvnt = -1; }
      if ( _uiGetSItem ( kbe , xx+l-w+3 , yy+3 , xx+l-3 , yy+w-3 ) ) mvnt = 1;
      if ( _uiGetSItem ( kbe , xx+w+3 , yy+3 , xx+l-w-3 , yy+w-3 ) ) \
      { mvnt = kbe.x-xx -w -3+2- pt->df; }
      if ( mvnt ) {
          df = pt->df;
          pt->df += mvnt;
          if ( pt->df < 0 ) pt->df = 0;
          if ( pt->df > ( pt->l ) ) pt->df = pt->l;
          if ( df != pt->df ) {
              _ui_moveslidepointer ( pt->D , pt ) ;
              _ui_slidevalue ( pt->D , pt ) ;
//      d[i].d->Update(code,i,D);
//TCB
#if 0
              switch ( d->code ) {
                  case 'd':
                  if ( d->Update != NULL ) d->Update ( _ui_getdslidevalue \
                   ( pt ) , pt->id , pt->D ) ;
                  break;
                  default:
                  break;
              }
#endif
              uiUpdateOn ( pt->D ) ;
              ret = 1;
          }
      }
      return ret;
  }
  int _ui_processhslidepress ( DIHB *B , KBEVENT kbe ) {
      S_STR *pt;
      int mvnt = 0 , xx , yy , l , w , l1 , xx1 , yy1;
      int i , j , k = 0 , scr_ln = 0;
      double fac = 1. , sx = 0. , val , df;
      char buf [ 30 ] ;
      pt = B->sptr;
      w = pt->Sw;
      l1 = pt->l+pt->Sldwd;
      xx1 = pt->x-pt->Sldwd/2;
      yy1 = pt->y-w/2;
      w = pt->Sw;
      l = pt->l;
      xx = pt->x;
      yy = pt->y-w/2;
      mvnt = 0;
      switch ( B->direction ) {
          default:
          case 0:
          if ( _uiGetSItem ( kbe , xx1 , yy1 , xx1+l1 , \
               yy1+w ) ) mvnt = kbe.x-xx- pt->df;
          df = pt->df;
          pt->df += mvnt;
          break;
          case 1:
          yy1 = pt->y-pt->Sldwd/2;
          xx1 = pt->x-w/2;
          yy = pt->y;
          if ( _uiGetSItem ( kbe , xx1 , yy1 , xx1+w , yy1+l1 ) ) mvnt = \
           ( kbe.y-yy-B->lngth+ pt->df ) ;
          df = pt->df;
          pt->df = pt->df-mvnt;
          break;
      }
      if ( pt->df < 0 ) pt->df = 0;
      if ( pt->df > ( pt->l ) ) pt->df = pt->l;
      if ( df != pt->df ) {
          _ui_movehslidepointer ( B ) ;
          uiUpdateOn ( pt->D ) ;
          return 1;
      }
      return 0;
  }
  int _ui_processslidedrag ( S_STR *pt , KBEVENT kbe ) {
      int mvnt = 0 , xx , yy , l , w , df , count;
      int i , j , k = 0 , scr_ln = 0 , ret = 0;
      double fac = 1. , sx = 0. , val;
      char buf [ 30 ] ;
      int evnt = 0;
      KBEVENT kb;
      DID *d;
      DIF *f;
      d = ( DID * ) uiGetWidget ( pt->D , pt->id ) ;
      xx = pt->x;
      yy = pt->y;
      scr_ln = pt->l;
      w = pt->Sldwd;
      l = scr_ln+2*pt->Sldwd+6;
      fac = pt->fac;
      sx = ( pt->df- ( pt->min ) ) *fac;
      mvnt = 0;
      if ( _uiGetSItem ( kbe , xx+w+3 , yy+3 , xx+l-w-3 , yy+w-3 ) ) {
#if 0
          mvnt = kbe.x-xx -w -3+2- pt->df;
          df = pt->df;
          pt->df += mvnt;
          if ( pt->df < 0 ) pt->df = 0;
          if ( pt->df > ( pt->l ) ) pt->df = pt->l;
          if ( df != pt->df ) {
              _ui_moveslidepointer ( pt->D , pt ) ;
              _ui_slidevalue ( pt->D , pt ) ;
              uiUpdateOn ( pt->D ) ;
              ret = 1;
          }
#else
          do {
              mvnt = kbe.x-xx -w -3+2- pt->df;
              if ( mvnt ) {
                  df = pt->df;
                  pt->df += mvnt;
                  if ( pt->df < 0 ) pt->df = 0;
                  if ( pt->df > ( pt->l ) ) pt->df = pt->l;
                  if ( df != pt->df ) {
                      _ui_moveslidepointer ( pt->D , pt ) ;
                      _ui_slidevalue ( pt->D , pt ) ;
//           if(d->Update != NULL) d->Update(_ui_getdslidevalue(pt),i,D);
                      switch ( d->code ) {
                          case 'd':
                          if ( d->Update != NULL ) d->Update ( _ui_getdslidevalue \
                           ( pt ) , pt->id , pt->D ) ;
                          break;
                          case 'f':
                          f = ( DIF * ) d;
                          if ( f->Update != NULL ) f->Update ( _ui_getfslidevalue \
                           ( pt ) , pt->id , pt->D ) ;
                          break;
                          default:
                          break;
                      }
                      uiUpdateOn ( pt->D ) ;
                      ret = 1;
                  }
              }
#if 0
              count = 0;
              while ( ( kbe = kgGetEvent ( pt->D ) ) .event == 3 ) {
                  count++;
                  if ( count == 10 ) break;
              }
#else
              count = 0;
              while ( ( ( evnt = kgCheckEvent ( pt->D , & kb ) ) != 0 ) && \
               ( kb.event == 3 ) ) {
                  count++;
                  kbe = kb;
                  if ( count == 50 ) break;
              }
              if ( evnt ) kbe = kb;
              else kbe.event = 0;
#endif
//      } while( (kbe).event != 2);
          } while ( ( kbe ) .event == 3 ) ;
#endif
      }
      return ret;
  }
  int _ui_processhslidedrag ( DIHB *B , KBEVENT kbe ) {
      S_STR *pt;
      int mvnt = 0 , xx , yy , l , w , df , x1 , y1 , x2 , y2 , f = 1;
      int j , k = 0 , scr_ln = 0 , count , ret = 0 , evnt;
      double fac = 1. , sx = 0. , val;
      char buf [ 30 ] ;
      KBEVENT kb , *kbd;
      DIALOG *D;
      pt = B->sptr;
      D = pt->D;
      kbd = & ( D->kb ) ;
      w = pt->Sw;
      l = pt->l;
      switch ( B->direction ) {
          default:
          case 0:
          xx = pt->x;
          yy = pt->y-w/2;
          x1 = xx; y1 = yy; x2 = xx+l; y2 = yy+w;
          f = 1;
          break;
          case 1:
          xx = pt->x-w/2;
          yy = pt->y;
          x1 = xx; y1 = yy; x2 = xx+w; y2 = yy+l;
          f = -1;
          break;
      }
      mvnt = 0;
      if ( _uiGetSItem ( kbe , x1 , y1 , x2 , y2 ) ) {
          do {
              switch ( B->direction ) {
                  default:
                  case 0:
                  mvnt = kbe.x-xx - pt->df;
                  break;
                  case 1:
                  mvnt = ( kbe.y-yy-B->lngth+ pt->df ) ;
                  break;
              }
              if ( mvnt ) {
                  df = pt->df;
                  pt->df = pt->df+f*mvnt;
                  if ( pt->df < 0 ) pt->df = 0;
                  if ( pt->df > ( pt->l ) ) pt->df = pt->l;
                  if ( df != pt->df ) {
                      _ui_movehslidepointer ( B ) ;
                      if ( B->Update != NULL ) B->Update ( _ui_getdslidevalue \
                       ( pt ) , B->item , D ) ;
                      else { kbd->x = kbe.x; kbd->y = kbe.y; kbd->event = 1; }
                      uiUpdateOn ( pt->D ) ;
                      ret = 1;
                  }
              }
              count = 0;
              while ( ( ( evnt = kgCheckEvent ( D , & kb ) ) != 0 ) && ( kb.event == 3 ) ) {
                  count++;
                  kbe = kb;
                  if ( count == 50 ) break;
              }
              if ( evnt ) kbe = kb;
              else kbe.event = 0;
//     } while( (kbe).event != 2);
          } while ( ( kbe ) .event == 3 ) ;
      }
      return ret;
  }
  int _ui_processslidekey ( S_STR *pt , KBEVENT kbe ) {
      int mvnt = 0 , xx , yy , l , w;
      int i , j , k = 0 , scr_ln = 0 , ans , df;
      double fac = 1. , sx = 0. , val;
      char buf [ 30 ] ;
      xx = pt->x;
      yy = pt->y;
      scr_ln = pt->l;
      w = pt->Sldwd;
      l = scr_ln+2*pt->Sldwd+6;
      fac = pt->fac;
      sx = ( pt->df- ( pt->min ) ) *fac;
      mvnt = 0;
      if ( _uiGetSItem ( kbe , xx+w+3 , yy+3 , xx+l-w-3 , \
           yy+w-3 ) ) mvnt = kbe.x-xx -w -3+2- pt->df;
      ans = kbe.key;
      if ( ui_Leftarrow ( ans ) ) {
          mvnt = -1;
      }
      if ( ui_Rightarrow ( ans ) ) {
          mvnt = 1;
      }
      if ( ui_Downarrow ( ans ) ) {
          mvnt = -1;
      }
      if ( ui_Uparrow ( ans ) ) {
          mvnt = 1;
      }
      df = pt->df;
      pt->df += mvnt;
      if ( pt->df < 0 ) pt->df = 0;
      if ( pt->df > ( pt->l ) ) pt->df = pt->l;
      if ( df != pt->df ) {
          _ui_moveslidepointer ( pt->D , pt ) ;
          _ui_slidevalue ( pt->D , pt ) ;
          uiUpdateOn ( pt->D ) ;
          return 1;
      }
      return 0;
  }
  int _ui_processhslidekey ( DIHB *B , KBEVENT kbe ) {
      S_STR *pt;
      int mvnt = 0 , xx , yy , l , w;
      int i , j , k = 0 , scr_ln = 0 , ans , df;
      double fac = 1. , sx = 0. , val;
      char buf [ 30 ] ;
      pt = B->sptr;
      xx = pt->x;
      yy = pt->y;
      scr_ln = pt->l;
      w = pt->Sldwd;
      l = scr_ln+2*pt->Sldwd+6;
      fac = pt->fac;
      sx = ( pt->df- ( pt->min ) ) *fac;
      mvnt = 0;
      if ( _uiGetSItem ( kbe , xx+w+3 , yy+3 , xx+l-w-3 , \
           yy+w-3 ) ) mvnt = kbe.x-xx -w -3+2- pt->df;
      ans = kbe.key;
      if ( ui_Leftarrow ( ans ) ) {
          mvnt = -1;
      }
      if ( ui_Rightarrow ( ans ) ) {
          mvnt = 1;
      }
      if ( ui_Downarrow ( ans ) ) {
          mvnt = -1;
      }
      if ( ui_Uparrow ( ans ) ) {
          mvnt = 1;
      }
      df = pt->df;
      pt->df += mvnt;
      if ( pt->df < 0 ) pt->df = 0;
      if ( pt->df > ( pt->l ) ) pt->df = pt->l;
      if ( df != pt->df ) {
          _ui_movehslidepointer ( B ) ;
          uiUpdateOn ( pt->D ) ;
          return 1;
      }
      return 0;
  }
  void _uiDrawSlideFloat ( DIALOG* D , int item ) {
      S_STR *sptr;
      DIF *f;
      f = D->d [ item ] .f;
      f->D = D;
      f->Bimg = NULL;
//   uiclean_gui_area(D,item,D->xo,D->yo,D->d);
      sptr = ( S_STR * ) Malloc ( sizeof ( S_STR ) ) ;
      f->sptr = sptr;
      f->item = item;
      sptr = f->sptr;
      sptr->id = item;
      sptr->Widget = f;
      if ( f->hide != 1 ) _uiMake_SF ( f ) ;
      return;
  }
  void _uiDrawV ( DIALOG* D , int item ) {
      S_STR *sptr;
      DIV *v;
      v = D->d [ item ] .v;
      v->D = D;
      v->item = item;
      v->Bimg = NULL;
//   Make_v_bar(v);
      if ( v->hide != 1 ) _uiMake_V ( v ) ;
      return;
  }
  void _uiDrawZ ( DIALOG* D , int item ) {
      S_STR *sptr;
      DIZ *v;
      v = D->d [ item ] .z;
      v->D = D;
      v->item = item;
      v->Bimg = NULL;
//   Make_z_bar(v);
      if ( v->hide != 1 ) _uiMake_Z ( v ) ;
      return;
  }
  void _uiDrawSlideInteger ( DIALOG* D , int item ) {
      DID *d;
      S_STR *sptr;
      d = D->d [ item ] .d;
      d->D = D;
      d->Bimg = NULL;
//   uiclean_gui_area(D,item,D->xo,D->yo,D->d);
      sptr = ( S_STR * ) Malloc ( sizeof ( S_STR ) ) ;
      d->sptr = sptr;
      d->item = item;
      sptr = d->sptr;
      sptr->id = item;
      sptr->Widget = d;
      if ( d->hide != 1 ) _uiMake_SD ( d ) ;
      return;
  }
  void _uiDrawHbar ( DIALOG* D , int item ) {
      S_STR *sptr;
      DIHB *B;
      B = D->d [ item ] .B;
      B->D = D;
      sptr = ( S_STR * ) Malloc ( sizeof ( S_STR ) ) ;
      B->sptr = sptr;
      B->item = item;
      B->Bimg = NULL;
      sptr = B->sptr;
      sptr->id = item;
      sptr->Widget = B;
      sptr->bimg = NULL;
      sptr->fimg = NULL;
      sptr->dir = 0;
      if ( B->hide != 1 ) _uiMake_SH ( B ) ;
      return;
  }
  void _uiDrawBrowser ( DIALOG* D , int item ) {
      DIW *w;
      BRW_STR *bwsr;
      w = D->d [ item ] .w;
      w->D = D;
      w->Bimg = NULL;
      w->itemhi = 1;
      w->bordr = 1;
      w->offset = 4;
      w->w = 22;
      w->width = 18;
      w->bkgr = 1;
      w->item = item;
      w->imgs = NULL;
      w->bwsr = ( BRW_STR * ) Malloc ( sizeof ( BRW_STR ) ) ;
      bwsr = ( BRW_STR * ) w->bwsr;
      bwsr->D = D;
      bwsr->wid = w;
      bwsr->Widget = w;
//   Make_browser(w);
      if ( w->hide != 1 ) _uiMake_W ( w ) ;
      return;
  }
  void _uiDrawMenuItem ( DIALOG* D , int item ) {
      DIE *w;
      BRW_STR *bwsr;
      w = D->d [ item ] .e;
      w->D = D;
//   clean_gui_area(item,D->xo,D->yo,D->d);
      w->item = item;
      Make_menu ( D , w , D->xo , D->yo ) ;
      bwsr = ( BRW_STR * ) w->bwsr;
      bwsr->D = D;
      bwsr->Widget = w;
      return;
  }
  void _uiDrawNewMenuItem ( DIALOG* D , int item ) {
      int height = 0;
      DIX *x;
      BRW_STR *bwsr;
      x = D->d [ item ] .x;
      x->D = D;
      x->nx = 1;
      x->item = item;
      x->Bimg = NULL;
      if ( x->offset < 4 ) x->offset = 4;
      if ( x->w < 2 ) x->w = 2;
//   x->bwsr = make_dialog_y_str(D,(DIY *)x,*(x->df),*(x->df),x->menu,x->size);
      x->bwsr = ( BRW_STR * ) Malloc ( sizeof ( BRW_STR ) ) ;
      bwsr = ( BRW_STR * ) x->bwsr;
      bwsr->Widget = x;
      x->item = item;
      _uiMake_X ( x ) ;
      return;
  }
  void _uiDrawX ( DIALOG* D , int item ) {
      int height = 0 , ln;
      DIX *x;
      BRW_STR *bwsr;
      x = D->d [ item ] .x;
      x->D = D;
      x->nx = 1;
      x->item = item;
      x->imgs = NULL;
      x->Bimg = NULL;
      if ( x->offset < 4 ) x->offset = 4;
      if ( x->w < 2 ) x->w = 2;
      ln = ( x->x2 -x->x1-2*x->offset-x->w ) ;
      if ( x->lngth > ( ln+x->xgap ) ) x->lngth = ln-x->xgap;
      x->nx = ln/ ( x->lngth+x->xgap ) ;
      if ( x->nx < 1 ) { x->nx = 1; }
      x->bwsr = ( BRW_STR * ) Malloc ( sizeof ( BRW_STR ) ) ;
      bwsr = ( BRW_STR * ) x->bwsr;
      bwsr->Widget = x;
      if ( x->hide != 1 ) _uiMake_X ( x ) ;
      return;
  }
  void _uiDrawE ( DIALOG* D , int item ) {
      int height = 0;
      DIE *e;
      BRW_STR *bwsr;
      e = D->d [ item ] .e;
      e->D = D;
      e->Bimg = NULL;
      e->item = item;
      if ( e->w == 0 ) e->w = 22;
      if ( e->width == 0 ) e->width = 20;
#if 0
      e->itemhi = 1;
      e->bordr = 1;
      e->bkgr = 1;
      e->nitems = 1;
#endif
      e->D = ( void * ) D;
      e->imgs = NULL;
      if ( e->offset < 4 ) e->offset = 4;
      if ( e->w < 2 ) e->w = 2;
//   x->bwsr = make_dialog_y_str(D,(DIY *)x,*(x->df),*(x->df),x->menu,x->size);
      e->bwsr = ( BRW_STR * ) Malloc ( sizeof ( BRW_STR ) ) ;
      bwsr = ( BRW_STR * ) e->bwsr;
      bwsr->Widget = e;
      if ( e->hide != 1 ) _uiMake_E ( e ) ;
      return;
  }
  void _uiDrawO ( DIALOG* D , int item ) {
      int height = 0;
      DIO *o;
      o = D->d [ item ] .o;
      o->D = D;
      o->item = item;
      o->Bimg = NULL;
      if ( o->hide != 1 ) _uiMake_O ( o ) ;
      return;
  }
  void _uiDrawY ( DIALOG* D , int item ) {
      int height = 0;
      DIY *y;
      BRW_STR *bwsr;
      y = D->d [ item ] .y;
      y->D = D;
      y->item = item;
      y->Bimg = NULL;
      if ( y->offset < 4 ) y->offset = 4;
      if ( y->w < 2 ) y->w = 2;
#if 0
      y->nx = ( y->x2 -y->x1-2*y->offset-y->w ) / ( y->width+y->xgap ) ;
      if ( y->nx < 1 ) {
          y->width = ( y->x2 -y->x1-2*y->offset-y->w ) -y->xgap;
          y->nx = 1;
      }
#else
      y->nx = ( y->x2 -y->x1-2*y->offset-y->w ) / ( y->lngth+y->xgap ) ;
      if ( y->nx < 1 ) {
          y->lngth = ( y->x2 -y->x1-2*y->offset-y->w ) -y->xgap;
          y->nx = 1;
      }
#endif
      y->imgs = NULL;
      y->bwsr = ( BRW_STR * ) Malloc ( sizeof ( BRW_STR ) ) ;
      bwsr = ( BRW_STR * ) y->bwsr;
      bwsr->Widget = y;
      bwsr->pos = 0;
//   y->bwsr = make_dialog_y_str(D,y,*(y->df),*(y->df),y->menu,y->size);
//   bwsr = (BRW_STR *)y->bwsr;
      if ( y->hide != 1 ) _uiMake_Y ( y ) ;
      return;
  }
  void _uiDrawRadioButton ( DIALOG* D , int item ) {
      int height = 0 , ln = 0;
      DIRA *y;
      BRW_STR *bwsr;
      y = D->d [ item ] .r;
      y->D = D;
      y->item = item;
      y->Bimg = NULL;
      if ( y->bkgr != 0 ) if ( y->offset < 4 ) y->offset = 4;
      if ( y->w < 2 ) y->w = 2;
      ln = ( y->x2 -y->x1-2*y->offset-y->w ) ;
      if ( y->lngth > ( ln+y->xgap ) ) y->lngth = ln-y->xgap;
      y->nx = ln/ ( y->lngth+y->xgap ) ;
      if ( y->nx <= 1 ) { y->nx = 1; }
      if ( y->xgap < 0 ) y->xgap = 0;
      y->imgs = NULL;
      y->bwsr = ( BRW_STR * ) Malloc ( sizeof ( BRW_STR ) ) ;
      bwsr = ( BRW_STR * ) y->bwsr;
      bwsr->Widget = y;
      if ( y->hide != 1 ) _uiMake_RadioButton ( y ) ;
      return;
  }
  void _uiDrawCheckBox ( DIALOG* D , int item ) {
      int height = 0 , ln;
      DICH *y;
      BRW_STR *bwsr;
      y = D->d [ item ] .c;
      y->D = D;
      y->item = item;
      y->imgs = NULL;
      y->Bimg = NULL;
      if ( y->bkgr != 0 ) if ( y->offset < 4 ) y->offset = 4;
      if ( y->w < 2 ) y->w = 2;
      ln = ( y->x2 -y->x1-2*y->offset-y->w ) ;
      if ( y->lngth > ( ln+y->xgap ) ) y->lngth = ln-y->xgap;
      y->nx = ( y->x2 -y->x1-2*y->offset-y->w ) / ( y->lngth+y->xgap ) ;
      if ( y->nx <= 1 ) y->nx = 1;
      if ( * ( y->df ) < 1 ) * ( y->df ) = 1;
//   y->bwsr = make_dialog_y_str(D,(DIY *)y,*(y->df),*(y->df),y->menu,y->size);
      y->bwsr = ( BRW_STR * ) Malloc ( sizeof ( BRW_STR ) ) ;
      bwsr = ( BRW_STR * ) y->bwsr;
      bwsr->Widget = y;
      if ( y->hide != 1 ) _uiMake_CheckBox ( y ) ;
      return;
  }
  void _uiDrawScrollMsgItem ( DIALOG* D , int item ) {
      DIS *w;
      BRW_STR *bwsr;
      w = D->d [ item ] .s;
      w->D = D;
      w->Bimg = NULL;
      w->item = item;
      bwsr = ( BRW_STR * ) Malloc ( sizeof ( BRW_STR ) ) ;
      w->bwsr = bwsr;
      bwsr->D = D;
      bwsr->Widget = w;
      if ( w->hide != 1 ) _uiMake_MS ( w ) ;
      return;
  }
  void _uiDrawGraphicsArea ( DIALOG* D , int item ) {
      DIG *w;
      w = D->d [ item ] .g;
      w->D = D;
      w->Bimg = NULL;
      w->item = item;
      w->cMain = NULL;
      w->glWindow = NULL;
//   Make_viewport(D,item);
      if ( w->hide != 1 ) _uiMake_G ( w ) ;
  }
  void _ui_diamessage ( DIALOG *D , int x1 , \
       int y1 , int x2 , int y2 , char *m ) \
  {
//   int x2,y2,l;
      int l;
      int EVGAY;
      Gclr gc;
      kgWC *wc;
      wc = WC ( D ) ;
      EVGAY = D->evgay;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      l = strlen ( m ) ;
      _uirect_fill ( wc , x1 , EVGAY- ( y1 ) , x2 , EVGAY-y2 , D->gc.msg_fill ) ;
      uiString ( D , m , x1 , y1 , ( x2-x1 ) , ( y2-y1 ) , D->gc.MsgFont , \
           D->gc.msg_char , D->gc.FontSize , 0 , D->gc.msg_fill ) ;
      _ui_draw_bound ( ( D ) , x1 , EVGAY- ( y1 ) , \
           x2 , EVGAY-y2 , D->gc.msg_bodr ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _ui_dia_T_message ( DIALOG *D , int x1 , int y1 , char *m ) {
      int x2 , y2 , l;
      kgWC *wc;
      wc = WC ( D ) ;
      l = strlen ( m ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      x2 = x1+ l*9+10;
      y2 = y1+16;
      uiString ( D , m , x1 , y1 , ( x2-x1 ) , abs ( y2-y1 ) , D->gc.MsgFont , \
           D->gc.msg_char , D->gc.FontSize , 0 , D->gc.msg_fill ) ;
      uiRest_clip_limits ( wc ) ;
  }
  void _uiDrawMessage ( DIALOG* D , int item ) {
      DIM *m;
      int w , h , FontSize;
      m = D->d [ item ] .m;
      m->D = D;
      m->Bimg = NULL;
      m->item = item;
      if ( m->hide != 1 ) _uiMake_M ( m ) ;
      return;
  }
  int _uiMake_M ( DIM *m ) {
      DIALOG *D;
      int w , h , FontSize , ret = 1 , just = -1 , fillclr;
      int x1 , y1 , x2 , y2;
      if ( m->hide != 1 ) {
          D = m->D;
          CHECKLIMITS ( m ) ;
          w = ( m->x2-m->x1 ) ;
          h = ( m->y2-m->y1 ) ;
          x1 = m->x1+D->xo; x2 = m->x2+D->xo;
          y1 = m->y1+D->yo; y2 = m->y2+D->yo;
          if ( m->Bimg == NULL ) {
              m->Bimg = kgGetBackground ( D , x1 , y1 , x2 , y2 ) ;
          }
          else {
              kgRestoreImage ( D , m->Bimg , x1 , y1 , ( x2-x1+1 ) , ( y2-y1+1 ) ) ;
          }
          if ( m->justification == 0 ) just = 0;
          else if ( m->justification == -1 ) just = -1;
          else if ( m->justification == 1 ) just = 1;
          uiDefaultGuiFontSize ( D ) ;
          switch ( m->code ) {
              case 'm':
              fillclr = D->gc.fill_clr;
              if ( D->transparency != 0.0 ) fillclr = -1;
              uiMessageString ( D , m->msg , ( int ) ( x1 ) , \
                   ( int ) ( ( y1 ) ) , ( x2-x1 ) , \
               ( y2-y1 ) , D->gc.MsgFont , D->gc.msg_char , \
                    D->gc.GuiFontSize , just , fillclr ) ;
              break;
              case 'M':
              FontSize = 5*h/9;
              uiSplashString ( D , m->msg , ( int ) ( m->x1+D->xo ) , ( int ) ( ( m->y1+D->yo ) ) , \
                   w , h , D->gc.SplashFont , D->gc.SplashFillColor , D->gc.SplashCharColor , \
                   FontSize , just , 0.25 ) ;
              break;
              case 'B':
//           _ui_diamessage(D,m->x1+D->xo,m->y1+D->yo,m->x2+D->xo,m->y2+D->yo,m->msg);
              x1 = m->x1+D->xo; x2 = m->x2+D->xo;
              y1 = m->y1+D->yo; y2 = m->y2+D->yo;
              uiBkup_clip_limits ( WC ( D ) ) ;
              uiSet_full_scrn ( WC ( D ) ) ;
              _dvrect_fill ( WC ( D ) , x1 , ( y1 ) , x2 , y2 , D->gc.msg_fill ) ;
              uiMessageString ( D , m->msg , x1 , y1 , x2-x1 , y2-y1 , D->gc.MsgFont , \
                   D->gc.msg_char , D->gc.GuiFontSize , just , D->gc.msg_fill ) ;
              _dv_draw_bound ( ( D ) , x1 , ( y1 ) , x2 , y2 , D->gc.msg_bodr ) ;
              uiRest_clip_limits ( WC ( D ) ) ;
              break;
          }
      }
      else {
          DIALOG *D;
          kgWC *wc;
          int x1 , y1 , x2 , y2;
          D = m->D;
          wc = WC ( D ) ;
          ret = 0;
          x1 = m->x1+D->xo;
          x2 = m->x2+D->xo;
          y1 = m->y1+D->yo;
          y2 = m->y2+D->yo;
          if ( m->Bimg != NULL ) kgRestoreImage ( D , m->Bimg , x1 , y1 , \
           ( x2-x1+1 ) , ( y2-y1+1 ) ) ;
      }
      return ret;
  }
  void _uiUnsupported ( DIALOG* D , int item ) {
      printf ( "Widget no more supported: %c\n" , D->d [ item ] .t->code ) ;
      return;
  }
  int _uiGetSItem ( KBEVENT kbe , int x1 , int y1 , int x2 , int y2 ) {
      if ( ( x1-kbe.x ) * ( x2-kbe.x ) > 0 ) return 0;
      if ( ( y1-kbe.y ) * ( y2-kbe.y ) > 0 ) return 0;
      return 1;
  }
  int _uiCheckBox ( KBEVENT kbe , int x1 , int y1 , int x2 , int y2 ) {
      if ( ( x1-kbe.x ) * ( x2-kbe.x ) > 0 ) return 0;
      if ( ( y1-kbe.y ) * ( y2-kbe.y ) > 0 ) return 0;
      return 1;
  }
  int _ui_scroll_item_hilight ( BRW_STR *br , KBEVENT kbe ) {
      int ret = 1 , item , df;
      DIALOG *D;
      kgWC *wc;
      D = br->D;
      wc = WC ( D ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      df = br->hitem;
      item = _uiGetSItem ( kbe , br->MS.ixx , br->MS.iyy+br->MS.thirty/2 , \
           br->MS.ixu , br->MS.iyu-br->MS.thirty/2 ) ;
      if ( item ) {
          item = ( kbe.y - ( br->MS.iyy+br->MS.thirty/2 ) ) /br->MS.thirty+1;
          if ( item < 1 ) item = 1;
          if ( item > br->MS.imenu ) item = br->MS.imenu;
          br->MS.item = item;
          br->hitem = br->pos+item;
          if ( br->hitem < 1 ) br->hitem = 1;
          if ( br->hitem > br->MS.nitems ) br->hitem = br->MS.nitems;
          if ( df != br->hitem ) {
              _uiMoveVertPointer ( br ) ; _uiPutmenu ( br ) ;
              uiUpdateOn ( D ) ;
          }
          ret = 1;
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_scroll_key ( BRW_STR *br , KBEVENT kbe ) {
      int ret = -1 , ans;
      ans = kbe.key;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
          return 1;
      }
      if ( ui_Uparrow ( ans ) ) {
          if ( br->hitem > 1 ) {
              br->hitem -= 1;
              br->df = br->hitem;
              _uiMoveVertPointer ( br ) ; _uiPutmenu ( br ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      if ( ui_Downarrow ( ans ) ) {
          if ( br->hitem < br->MS.nitems ) {
              br->hitem += 1;
              br->df = br->hitem;
              _uiMoveVertPointer ( br ) ; _uiPutmenu ( br ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      return ret;
  }
  int _ui_process_scroll_click ( BRW_STR *br , KBEVENT kbe ) {
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp;
      int xx , yy , h , w;
      float fac;
      int df;
      kgWC *wc;
      wc = br->D->wc;
      df = br->hitem;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      ixmid = br->MS.ixx+br->MS.ixl/2;
      iy = br->MS.iyy+1;
      k = 1;
      if ( ( br->MS.item != 0 ) && ( br->MS.item <= br->MS.imenu ) ) k = br->MS.item;
          
      jj = iy + ( k ) *br->MS.thirty;
      kk = -1;
      item = _uiGetSItem ( kbe , br->MS.ixx , br->MS.iyy+br->MS.thirty/2 , \
           br->MS.ixu , br->MS.iyu-br->MS.thirty/2 ) ;
      if ( item ) {
          item = ( kbe.y - ( br->MS.iyy+br->MS.thirty/2 ) ) /br->MS.thirty+1;
          if ( item < 1 ) item = 1;
          if ( item > br->MS.imenu ) item = br->MS.imenu;
          br->MS.item = item;
          br->hitem = br->pos+item;
          br->df = br->hitem;
          if ( df != br->hitem ) {
              _uiMoveVertPointer ( br ) ; _uiPutmenu ( br ) ;
              uiUpdateOn ( br->D ) ;
          }
          ret = 1;
      }
      else {
          w = 24;
          h = br->MS.iyl+20;
          xx = br->MS.ixx+br->MS.ixl;
          yy = br->MS.iyy-10+w+4;
          item = _uiGetSItem ( kbe , xx+3 , yy , xx+w-3 , yy+h-2*w-8 ) ;
          if ( item ) {
              fac = 0.0;
              if ( br->MS.nitems-br->size > 0 ) {
                  fac = br->MS.fac;
                  iy = kbe.y - yy;
                  iy = iy/fac+0.5;
                  br->hitem = iy;
                  if ( br->hitem < 1 ) br->hitem = 1;
                  if ( br->hitem > br->MS.nitems ) br->hitem = br->MS.nitems;
                  br->df = br->hitem;
                  _uiMoveVertPointer ( br ) ; _uiPutmenu ( br ) ;
                  uiUpdateOn ( br->D ) ;
                  ret = -1;
              }
          }
          else {
              xx = br->MS.ixx+br->MS.ixl;
              yy = br->MS.iyy-10;
              item = _uiGetSItem ( kbe , xx , yy , xx+w , yy+w ) ;
              if ( item ) {
//            if(br->hitem >br->size) {
                  br->hitem -= 1;
                  if ( br->hitem < 1 ) br->hitem = 1;
                  br->df = br->hitem;
                  _uiMoveVertPointer ( br ) ; _uiPutmenu ( br ) ;
                  uiUpdateOn ( br->D ) ;
//            }
                  ret = -1;
              }
              else {
                  xx = br->MS.ixx+br->MS.ixl;
                  yy = br->MS.iyy-10+h-w;
                  item = _uiGetSItem ( kbe , xx , yy , xx+w , yy+w-4 ) ;
                  if ( item ) {
                      if ( br->hitem < br->MS.nitems ) {
                          if ( br->hitem <= br->size ) { br->hitem = br->size+1; }
                          else br->hitem += 1;
                          br->df = br->hitem;
                          _uiMoveVertPointer ( br ) ;
                          _uiPutmenu ( br ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -1;
                  }
              }
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_scroll_move ( BRW_STR *br , KBEVENT kbe ) {
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , df , count;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp;
      int xx , yy , h , w;
      float fac;
      kgWC *wc;
      int evnt = 0;
      KBEVENT kb;
      wc = br->D->wc;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      ixmid = br->MS.ixx+br->MS.ixl/2;
      iy = br->MS.iyy+1;
      k = 1;
      if ( ( br->MS.item != 0 ) && ( br->MS.item <= br->MS.imenu ) ) k = br->MS.item;
          
      jj = iy + ( k ) *br->width;
      kk = -1;
      item = 1;
      df = br->df;
      //item=_uiGetSItem(kbe,br->MS.ixx,br->MS.iyy+br->MS.thirty/2,br->MS.ixu,br->MS.iyu-br->MS.thirty/2);
      if ( item ) {
          w = 24;
          h = br->MS.iyl+br->width;
          xx = br->MS.ixx+br->MS.ixl;
          yy = br->MS.iyy-10+w+4;
          item = _uiGetSItem ( kbe , xx+3 , yy , xx+w-3 , yy+h-2*w-8 ) ;
          if ( item ) {
              do {
                  fac = 0.0;
                  if ( br->MS.nitems-br->size > 0 ) {
                      fac = br->MS.fac;
                      iy = kbe.y - yy;
                      iy = iy/fac+0.5;
                      br->hitem = iy;
                      if ( br->hitem < 1 ) br->hitem = 1;
                      if ( br->hitem > br->MS.nitems ) br->hitem = br->MS.nitems;
                      br->df = br->hitem;
                      if ( br->df != df ) {_uiMoveVertPointer ( br ) ;
                      _uiPutmenu ( br ) ; uiUpdateOn ( br->D ) ; }
                      ret = -1;
                  }
#if 0
                  count = 0;
                  while ( ( kbe = kgGetEvent ( br->D ) ) .event == 3 ) {
                      count++;
                      if ( count == 10 ) break;
                  }
#else
                  count = 0;
                  while ( ( ( evnt = kgCheckEvent ( br->D , & kb ) ) != 0 ) && \
                   ( kb.event == 3 ) ) {
                      count++;
                      kbe = kb;
                      if ( count == 50 ) break;
                  }
                  if ( evnt ) kbe = kb;
#endif
              } while ( ( kbe ) .event != 2 ) ;
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  void _uiSetSwitch ( void *tmp ) {
      DIY *y;
      BRW_STR *br;
      int i , df , *val , type = 1;
      ThumbNail **list;
      y = ( DIY * ) tmp;
      type = ( y->type ) %10;
      if ( y->code == 'r' ) type = 1;
      val = ( int * ) ( y->df ) ;
      br = y->bwsr;
      df = br->df-1;
      *val = br->df;
      if ( y->list == NULL ) return;
      list = ( ThumbNail ** ) y->list;
      switch ( type ) {
          case 0:
          if ( list [ df ]->sw == 0 ) list [ df ]->sw = 1;
          else if ( list [ df ]->sw == 1 ) list [ df ]->sw = 0;
          break;
          case 1:
          for ( i = 0; i < y->nitems; i++ ) list [ i ]->sw = 0;
          list [ df ]->sw = 1;
          break;
          default:
          for ( i = 0; i < y->nitems; i++ ) list [ i ]->sw = 0;
          list [ df ]->sw = 1;
          break;
      }
      return;
  }
  int _ui_process_y_key ( DIY *y , KBEVENT kbe ) {
      BRW_STR *br;
      int ret = -1 , ans , df , hitem , i;
      br = y->bwsr;
      ans = kbe.key;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
          br->df = br->hitem+1;
          df = br->df-1;
//          *(y->df)=br->df;
          _uiSetSwitch ( y ) ;
          _uiPutYmenu ( y ) ;
          uiUpdateOn ( br->D ) ;
          return 1;
      }
      if ( ui_Uparrow ( ans ) ) {
          if ( br->hitem >= y->nx ) {
              br->hitem -= y->nx;
              _uiMoveYVertPointer ( y ) ; _uiPutYmenu ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      if ( ui_Downarrow ( ans ) ) {
          hitem = br->hitem/y->nx;
          if ( hitem < ( y->ny-1 ) ) {
              br->hitem += y->nx;
              if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
              _uiMoveYVertPointer ( y ) ; _uiPutYmenu ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      if ( ui_Leftarrow ( ans ) ) {
          if ( br->hitem >= 0 ) {
              br->hitem -= 1;
              _uiMoveYVertPointer ( y ) ; _uiPutYmenu ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      if ( ui_Rightarrow ( ans ) ) {
          hitem = br->hitem/y->nx;
          if ( br->hitem < ( y->nitems-1 ) ) {
              br->hitem += 1;
              if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
              _uiMoveYVertPointer ( y ) ; _uiPutYmenu ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      return ret;
  }
  int kgGetThumbNailItem ( DIY *Y , int x , int y ) {
      KBEVENT kbe;
      BRW_STR *br;
      int item = -1 , pos = 0 , iy , ix , ret = -1 , yoffset , xoffset;
      br = Y->bwsr;
      kbe.x = x;
      kbe.y = y;
      yoffset = br->offset;
      xoffset = Y->offset;
      pos = _uiGetSItem ( kbe , br->x1+xoffset , br->y1+yoffset , \
           br->x2-xoffset-br->w , br->y2-yoffset ) ;
      if ( pos ) {
          iy = ( kbe.y - ( br->y1+yoffset ) ) ;
          if ( iy > 0 ) iy = iy/br->width;
          if ( ( iy >= 0 ) && ( iy < Y->size ) ) {
              ix = ( kbe.x - ( br->x1+xoffset+br->xshift ) ) ;
              if ( ix > 0 ) ix = ix/ ( Y->lngth+Y->xgap ) ;
              if ( ( ix >= 0 ) && ( ix < Y->nx ) ) {
                  item = br->pos+iy*Y->nx+ix;
                  if ( item < Y->nitems ) {
                      ret = item;
                  }
              } // eof if ( ( ix > 0 ) && ( ix < y->nx ) ) 
          }// eof if ( ( iy > 0 ) && ( iy < y->size ) ) 
      }
      return ret;
  }
  int kgGetThumbNailItemRectangle ( DIY *Y , int item , \
       int *x1 , int *y1 , int *x2 , int *y2 ) \
  {
      int ret = 0 , ix , iy , xoffset , yoffset , size;
      BRW_STR *br;
      br = Y->bwsr;
      yoffset = br->offset;
      xoffset = Y->offset;
      item = item-br->pos;
      size = Y->size*Y->nx;
      if ( ( item >= 0 ) && ( item < size ) ) {
          iy = item/ ( Y->nx ) ;
          ix = item%Y->nx;
          *x1 = ( br->x1+xoffset+br->xshift ) + ix* ( Y->lngth+Y->xgap ) ;
          *x2 = *x1+ ( Y->lngth+Y->xgap ) ;
          *y1 = br->y1+yoffset + iy*br->width;
          *y2 = *y1+br->width;
          ret = 1;
      }
      return ret;
  }
  int _ui_process_y_click ( DIY *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = -1 , xoffset , yoffset , hitem;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp , ix;
      int xx , yy , h , w;
      float fac;
      int df;
      kgWC *wc;
      br = y->bwsr;
      yoffset = br->offset;
      xoffset = y->offset;
      wc = br->D->wc;
      df = br->df;
      item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
           br->rsx2 , br->rsy1+br->sy+br->shy ) ;
      if ( item ) {
          _ui_process_y_move ( y , kbe ) ;
          return -100;
      }
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = _uiGetSItem ( kbe , br->x1+xoffset , br->y1+yoffset , \
           br->x2-xoffset-br->w , br->y2-yoffset ) ;
      if ( item ) {
          if ( kbe.button != 1 ) { uiRest_clip_limits ( wc ) ; return -1; }
          iy = ( kbe.y - ( br->y1+yoffset ) ) ;
          if ( iy > 0 ) iy = iy/br->width;
          if ( ( iy >= 0 ) && ( iy < y->size ) ) {
              ix = ( kbe.x - ( br->x1+xoffset+br->xshift ) ) ;
              if ( ix > 0 ) ix = ix/ ( y->lngth+y->xgap ) ;
              if ( ( ix >= 0 ) && ( ix < y->nx ) ) {
                  item = br->pos+iy*y->nx+ix;
                  if ( item < y->nitems ) {
                      br->hitem = item;
                      br->df = item+1;
                      * ( y->df ) = item+1;
                      df = br->df-1;
                      _uiSetSwitch ( y ) ;
                      _uiPutYmenu ( y ) ;
                      uiUpdateOn ( br->D ) ;
                      ret = 1;
                  }
              } // eof if ( ( ix > 0 ) && ( ix < y->nx ) ) 
          }// eof if ( ( iy > 0 ) && ( iy < y->size ) ) 
      }
      else {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1 , br->rsx2 , br->rsy2 ) ;
          if ( item ) {
              fac = 0.0;
              if ( y->ny-y->size > 0 ) {
                  fac = br->fac;
                  iy = kbe.y - br->rsy1;
                  iy = iy/fac+0.5;
                  br->hitem = iy*y->nx+ ( br->hitem%y->nx ) ;
                  if ( br->hitem < 0 ) br->hitem = 0;
                  if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                  _uiMoveYVertPointer ( y ) ; _uiPutYmenu ( y ) ;
                  uiUpdateOn ( br->D ) ;
                  ret = -100;
              }
              while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
          }
          else {
              item = _uiGetSItem ( kbe , br->tbx1 , br->tby1 , br->tbx2 , br->tby2 ) ;
              if ( item ) {
                  br->hitem = br->pos;
                  if ( br->hitem >= y->nx ) {
                      br->hitem -= y->nx;
                      if ( br->hitem < 0 ) br->hitem = 0;
                      _uiMoveYVertPointer ( y ) ; _uiPutYmenu ( y ) ;
                      uiUpdateOn ( br->D ) ;
                  }
                  ret = -101;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
              }
              else {
                  item = _uiGetSItem ( kbe , br->bbx1 , br->bby1 , br->bbx2 , br->bby2 ) ;
                  if ( item ) {
                      hitem = br->pos/y->nx;
                      hitem += ( y->size-1 ) ;
                      if ( hitem >= y->ny ) hitem = y->ny-1;
                      br->hitem = hitem*y->nx;
                      if ( hitem < ( y->ny-1 ) ) {
                          br->hitem += y->nx;
                          if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                          _uiMoveYVertPointer ( y ) ; _uiPutYmenu ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -102;
                      while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
                  }
              }
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_y_move ( DIY *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , df , hitem , count;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp;
      int xx , yy , h , w;
      float fac;
      kgWC *wc;
      int evnt = 0;
      KBEVENT kb;
      br = y->bwsr;
      wc = br->D->wc;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = 1;
      df = br->df;
//      if ( item) {
      {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
               br->rsx2 , br->rsy1+br->sy+br->shy ) ;
          if ( item ) {
              do {
                  fac = 0.0;
                  yy = br->rsy1;
                  hitem = br->hitem/y->nx;
                  if ( y->ny-y->size > 0 ) {
                      fac = br->fac;
                      iy = kbe.y - yy;
                      iy = iy/fac+0.5;
                      if ( iy >= y->ny ) iy = y->ny-1;
                      if ( iy < 0 ) iy = 0;
                      hitem = br->hitem/y->nx;
                      if ( hitem != iy ) {
                          br->hitem = iy*y->nx;
                          _uiMoveYVertPointer ( y ) ;
                          _uiPutYmenu ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -1;
                  }
#if 0
                  count = 0;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event == 3 ) {
                      count++;
                      if ( count == 10 ) break;
                  }
#else
                  count = 0;
                  while ( ( ( evnt = kgCheckEvent ( y->D , & kb ) ) != 0 ) && \
                   ( kb.event == 3 ) ) {
                      count++;
                      kbe = kb;
                      if ( count == 50 ) break;
                  }
                  if ( evnt ) kbe = kb;
#endif
              } while ( ( kbe ) .event != 2 ) ;
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int kgScrollUpThumbNails ( DIY *y ) {
      int ret = 1 , pos;
      BRW_STR *br;
      kgWC *wc;
      br = y->bwsr;
      wc = br->D->wc;
      pos = br->pos/y->nx;
      if ( pos > 0 ) {
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          br->hitem -= y->nx;
          _uiMoveYVertPointer ( y ) ;
          _uiPutYmenu ( y ) ;
          uiUpdateOn ( br->D ) ;
          uiRest_clip_limits ( wc ) ;
          ret = 1;
      }
      else ret = 0;
      return ( ret ) ;
  }
  int kgScrollDownThumbNails ( DIY *y ) {
      int ret = 1 , pos;
      BRW_STR *br;
      kgWC *wc;
      br = y->bwsr;
      wc = br->D->wc;
      pos = br->pos/y->nx;
      if ( pos < ( y->ny-y->size ) ) {
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          br->hitem += y->nx;
          if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
          _uiMoveYVertPointer ( y ) ;
          _uiPutYmenu ( y ) ;
          uiUpdateOn ( br->D ) ;
          uiRest_clip_limits ( wc ) ;
          ret = 1;
      }
      else ret = 0;
      return ( ret ) ;
  }
  int kgDragThumbNail ( void *wid , int item , int *x , int *y ) {
      ThumbNail **th , *tpt;
      void *Img = NULL;
      KBEVENT kb;
      DIALOG *D;
      DIY *Y;
      int ret = 0 , pos , k , rv;
      int ymin , ymax , xmin , xmax , xmid;
      int x1 , y1 , x2 , y2 , xl , yl;
      Y = ( DIY * ) wid;
      D = ( ( DIALOG * ) ( Y->D ) ) ;
      if ( ( Y->code != 'y' ) && ( Y->code != 'x' ) ) return 0;
      ymin = Y->y1+D->yo+Y->offset;
      ymax = Y->y2+D->yo-Y->offset;
      xmin = Y->x1+D->xo+Y->offset;
      xmax = Y->x2+D->xo-Y->offset;
      th = ( ThumbNail ** ) Y->list;
      kb = kgGetEvent ( D ) ;
      while ( 1 ) {
          if ( kb.event == 3 ) {
              if ( Img == NULL ) {
                  if ( kgGetThumbNailItemRectangle ( Y , item , & x1 , & y1 , & x2 , & y2 ) ) {
                      *x = x1; *y = y1;
                      xl = ( x2-x1+1 ) ;
                      yl = ( y2-y1+1 ) ;
                      xmid = ( xmin+xmax-xl ) /2;
                      Img = kgRegionImage ( D , x1 , y1 , x2 , y2 ) ;
                  }
                  else break;
              }
              rv = kgDragImage ( D , Img , xl , yl , xmin , ymin , xmax , ymax , x , y ) ;
              switch ( rv ) {
                  case 0:
                  return 1;
                  case -1:
                  kgScrollUpThumbNails ( Y ) ;
//            kb = kgGetEvent(D);
                  *y = ymin;
                  *x = xmid;
                  break;
                  case 1:
                  kgScrollDownThumbNails ( Y ) ;
//            kb = kgGetEvent(D);
                  *y = ymax-yl;
                  *x = xmid;
                  break;
                  default:
                  return 0;
              }
          }
          else break;
      }
      return ret;
  }
  int _ui_process_v_key ( DIV *y , KBEVENT kbe ) {
      int ret = -1 , ans , df , hitem , i;
      DIALOG *D = ( DIALOG * ) y->D;
      ans = kbe.key;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
          return 1;
      }
      if ( ui_Uparrow ( ans ) ) {
          if ( y->df == 0.0 ) return 1;
          y->df -= y->mvmt;
          if ( y->df < 0 ) y->df = 0;
          _uiMoveVVertPointer ( y ) ;
          uiUpdateOn ( y->D ) ;
          if ( y->Update != NULL ) y->Update ( y->df , y->item , D ) ;
          else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      }
      if ( ui_Downarrow ( ans ) ) {
          if ( y->df >= ( 100.-y->ds ) ) return 1;
          y->df += y->mvmt;
          if ( y->df > 100 ) y->df = 100;
          _uiMoveVVertPointer ( y ) ;
          uiUpdateOn ( y->D ) ;
          if ( y->Update != NULL ) y->Update ( y->df , y->item , D ) ;
          else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      }
      return ret;
  }
  int _ui_process_v_click ( DIV *y , KBEVENT kbe ) {
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , xoffset , yoffset , hitem;
      int k , kk , knew , jj , ixmid;
      int n , iyp , ixp , ix;
      int xx , yy , h , w , sy , sy2 , smax , ln , sh;
      float fac;
      double df , iy;
      kgWC *wc;
      DIALOG *D;
      D = y->D;
      wc = D->wc;
      df = y->df;
      xoffset = yoffset = y->offset;
      ln = ( y->rsy2-y->rsy1 ) ;
      if ( y->ds > 100 ) y->ds = 100;
      if ( y->df > ( 100-y->ds ) ) y->df = ( 100-y->ds ) ;
      sy = y->df*ln/100.0+0.5;
      fac = ln*y->ds/100.0;
      sh = fac+0.5;
      if ( ( ln > MINV ) && ( sh < MINV ) ) sh = MINV;
      ln = ln -sh;
      sy = y->df*ln/100.0+0.5;
//      sy = scr_ln* ( ( y->df ) ) /100;
//      if ( ( sy+sh ) > h ) sy = h-sh;
      sy2 = ( y->df+y->ds ) * ( ln ) /100.0+0.5;
      sy2 = sy+sh;
      if ( sy2 > y->rsy2 ) sy2 = y->rsy2;
      sy = sy2 - sh;
      smax = ( 100-y->ds ) * ( ln ) /100.0+0.5;
      if ( smax < 0 ) smax = 0;
      item = _uiGetSItem ( kbe , y->rsx1 , y->rsy1+sy , y->rsx2 , y->rsy1+sy2 ) ;
//      printf ("tcb: ITEM = %d\n",item);
      if ( item ) {
          _ui_process_v_move ( y , kbe ) ;
          return -1;
      }
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      {
          item = _uiGetSItem ( kbe , y->rsx1 , y->rsy1 , y->rsx2 , y->rsy2 ) ;
          if ( item ) {
              fac = 0.0;
              {
                  iy = kbe.y - y->rsy1;
                  iy = 100*iy/ ( ln ) ;
                  if ( iy < 0 ) iy = 0;
                  if ( iy > ( 100-y->ds ) ) iy = 100-y->ds+0.00001;
                  y->df = iy;
                  _uiMoveVVertPointer ( y ) ;
                  if ( y->Update != NULL ) y->Update ( y->df , y->item , D ) ;
                  else if ( D->Callback != NULL ) D->Callback ( D , & ( D->kb ) ) ;
                  uiUpdateOn ( y->D ) ;
                  ret = -100;
              }
              while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
          }
          else {
              item = _uiGetSItem ( kbe , y->tbx1 , y->tby1 , y->tbx2 , y->tby2 ) ;
              if ( item ) {
                  if ( y->df == 0.0 ) return -1;
                  y->df -= y->mvmt;
                  if ( y->df < 0 ) y->df = 0;
                  _uiMoveVVertPointer ( y ) ;
                  uiUpdateOn ( y->D ) ;
                  if ( y->Update != NULL ) y->Update ( y->df , y->item , D ) ;
                  else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
                  ret = -1;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
              }
              else {
                  item = _uiGetSItem ( kbe , y->bbx1 , y->bby1 , y->bbx2 , y->bby2 ) ;
                  if ( item ) {
//                      if ( y->df >= ( 100 -y->ds ) ) return -1;
                      if ( y->df >= 100 ) {y->df = 100.0; return -1;}
                      y->df += y->mvmt;
//                      if ( y->df > ( 100 -y->ds ) ) y->df = ( 100 -y->ds ) ;
                      if ( y->df > 100 ) y->df = 100.0;
                      _uiMoveVVertPointer ( y ) ;
                      uiUpdateOn ( y->D ) ;
                      if ( y->Update != NULL ) y->Update ( y->df , y->item , D ) ;
                      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
                      ret = -1;
                      while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
                  }
              }
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_v_move ( DIV *y , KBEVENT kbe ) {
      DIALOG *D;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , df , hitem , count;
      int k , kk , knew , jj , ixmid;
      int n , yp , ixp;
      int xx , yy , h , w , sy , sy2 , smax , ln , sh;
      float fac , iy;
      kgWC *wc;
      int evnt = 0;
      KBEVENT kb;
      D = ( DIALOG * ) y->D;
      wc = WC ( y->D ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      if ( y->ds > 100 ) y->ds = 100;
      ln = ( y->rsy2-y->rsy1 ) ;
      fac = ln*y->ds/100.0;
      sh = fac+0.5;
      if ( sh < MINV ) sh = MINV;
      ln = ln -sh;
//      if ( y->df > ( 100-y->ds ) ) y->df = ( 100-y->ds ) ;
      if ( y->df > 100 ) y->df = 100;
//      sy = y->df*ln/100.0+0.5;
//      sy2 = ( y->df+y->ds ) * ( ln ) /100.0+0.5;
      sy = y->df*ln/100.0+0.5;
      sy2 = sy+sh;
      if ( sy2 > y->rsy2 ) sy2 = y->rsy2;
      sy = sy2 - sh;
      smax = ( 100-y->ds ) * ( ln ) /100.0+0.5;
      if ( smax < 0 ) smax = 0;
      item = 1;
      item = _uiGetSItem ( kbe , y->rsx1 , y->rsy1+sy , y->rsx2 , y->rsy1+sy2 ) ;
      if ( item ) {
          do {
#if 0
              iy = kbe.y - y->rsy1;
              iy = 100*iy/ ( ln ) ;
              if ( iy < 0 ) iy = 0;
              if ( iy > ( 100-y->ds ) ) iy = 100-y->ds+0.00001;
              if ( iy != y->df ) {
                  y->df = iy;
                  _uiMoveVVertPointer ( y ) ;
                  uiUpdateOn ( y->D ) ;
                  if ( y->Update != NULL ) y->Update ( y->df , y->item , y->D ) ;
                  else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
              }
#endif
              ret = -1;
              count = 0;
              while ( ( ( evnt = kgCheckEvent ( y->D , & kb ) ) != 0 ) && \
               ( kb.event == 3 ) ) {
                  count++;
                  if ( evnt != 0 ) kbe = kb;
                  if ( count == 50 ) break;
//                      if(evnt != 0)printf("TCB:Got Event 3: %d\n",count);
              }
              if ( evnt ) kbe = kb;
//                  printf("TCB: Event: %d\n",kb.event);
#if 1
              iy = kbe.y - y->rsy1;
              iy = ( ( 100.0*iy ) / ( ln ) ) ;
              if ( iy < 0 ) iy = 0;
//                  if ( iy > ( 100-y->ds ) ) iy = 100-y->ds+0.00001;
              if ( iy > 100.0 ) iy = 100.0;
              if ( iy != y->df ) {
                  y->df = iy;
                  _uiMoveVVertPointer ( y ) ;
                  uiUpdateOn ( y->D ) ;
                  if ( y->Update != NULL ) y->Update ( y->df , y->item , y->D ) ;
                  else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
              }
              ret = -1;
#endif
          } while ( ( kbe ) .event != 2 ) ;
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_z_key ( DIZ *y , KBEVENT kbe ) {
      int ret = -1 , ans , df , hitem , i;
      DIALOG *D = ( DIALOG * ) y->D;
      ans = kbe.key;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
          return 1;
      }
      if ( ui_Leftarrow ( ans ) ) {
          if ( y->df == 0.0 ) return 1;
          y->df -= y->mvmt;
          if ( y->df < 0 ) y->df = 0;
          _uiMoveZVertPointer ( y ) ;
          uiUpdateOn ( y->D ) ;
          if ( y->Update != NULL ) y->Update ( y->df , y->item , D ) ;
          else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      }
      if ( ui_Rightarrow ( ans ) ) {
          if ( y->df >= ( 100.-y->ds ) ) return 1;
          y->df += y->mvmt;
          if ( y->df > 100 ) y->df = 100;
          _uiMoveZVertPointer ( y ) ;
          uiUpdateOn ( y->D ) ;
          if ( y->Update != NULL ) y->Update ( y->df , y->item , D ) ;
          else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      }
      return ret;
  }
  int _ui_process_z_click ( DIZ *y , KBEVENT kbe ) {
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , xoffset , yoffset , hitem;
      int k , kk , knew , jj , ixmid;
      int n , iyp , ixp , ix;
      int xx , yy , h , w , sy , sy2 , smax , ln;
      float fac;
      double df , iy;
      kgWC *wc;
      DIALOG *D;
      D = y->D;
      wc = D->wc;
      df = y->df;
      xoffset = yoffset = y->offset;
      ln = ( y->rsx2-y->rsx1 ) ;
      if ( y->ds > 100 ) y->ds = 100;
      if ( y->df > ( 100-y->ds ) ) y->df = ( 100-y->ds ) ;
      sy = y->df*ln/100.0+0.5;
      sy2 = ( y->df+y->ds ) * ( ln ) /100.0+0.5;
      smax = ( 100-y->ds ) * ( ln ) /100.0+0.5;
      if ( smax < 0 ) smax = 0;
      item = _uiGetSItem ( kbe , y->rsx1+sy , y->rsy1 , y->rsx1+sy2 , y->rsy2 ) ;
      if ( item ) {
          _ui_process_z_move ( y , kbe ) ;
          return -1;
      }
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      {
          item = _uiGetSItem ( kbe , y->rsx1 , y->rsy1 , y->rsx2 , y->rsy2 ) ;
          if ( item ) {
              fac = 0.0;
              {
                  iy = kbe.x - y->rsx1;
                  iy = 100*iy/ ( ln ) ;
                  if ( iy < 0 ) iy = 0;
                  if ( iy > ( 100-y->ds ) ) iy = 100-y->ds+0.1;
                  y->df = iy;
                  _uiMoveZVertPointer ( y ) ;
                  uiUpdateOn ( y->D ) ;
                  if ( y->Update != NULL ) y->Update ( y->df , y->item , D ) ;
                  else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
                  ret = -100;
              }
              while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
          }
          else {
              item = _uiGetSItem ( kbe , y->lbx1 , y->lby1 , y->lbx2 , y->lby2 ) ;
              if ( item ) {
                  if ( y->df == 0.0 ) return -1;
                  y->df -= y->mvmt;
                  if ( y->df < 0 ) y->df = 0;
                  _uiMoveZVertPointer ( y ) ;
                  uiUpdateOn ( y->D ) ;
                  if ( y->Update != NULL ) y->Update ( y->df , y->item , D ) ;
                  else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
                  ret = -1;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
              }
              else {
                  item = _uiGetSItem ( kbe , y->rbx1 , y->rby1 , y->rbx2 , y->rby2 ) ;
                  if ( item ) {
                      if ( y->df >= ( 100 -y->ds ) ) return -1;
                      y->df += y->mvmt;
                      if ( y->df > ( 100 -y->ds ) ) y->df = ( 100 -y->ds ) ;
                      _uiMoveZVertPointer ( y ) ;
                      uiUpdateOn ( y->D ) ;
                      if ( y->Update != NULL ) y->Update ( y->df , y->item , D ) ;
                      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
                      ret = -1;
                      while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
                  }
              }
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_z_move ( DIZ *y , KBEVENT kbe ) {
      DIALOG *D;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , df , hitem , count;
      int k , kk , knew , jj , ixmid;
      int n , iyp , ixp;
      int xx , yy , h , w , sy , sy2 , smax , ln;
      float fac , iy;
      kgWC *wc;
      int evnt = 0;
      KBEVENT kb;
      D = ( DIALOG * ) y->D;
      wc = WC ( y->D ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      ln = ( y->rsx2-y->rsx1 ) ;
      if ( y->ds > 100 ) y->ds = 100;
      if ( y->df > ( 100-y->ds ) ) y->df = ( 100-y->ds ) ;
      sy = y->df*ln/100.0+0.5;
      sy2 = ( y->df+y->ds ) * ( ln ) /100.0+0.5;
      smax = ( 100-y->ds ) * ( ln ) /100.0+0.5;
      if ( smax < 0 ) smax = 0;
      item = 1;
      {
          item = _uiGetSItem ( kbe , y->rsx1+sy , y->rsy1 , y->rsx1+sy2 , y->rsy2 ) ;
          if ( item ) {
              do {
                  iy = kbe.x - y->rsx1;
                  iy = 100*iy/ ( ln ) ;
                  if ( iy < 0 ) iy = 0;
                  if ( iy > ( 100-y->ds ) ) iy = 100-y->ds;
                  if ( iy != y->df ) {
                      y->df = iy;
                      _uiMoveZVertPointer ( y ) ;
                      uiUpdateOn ( y->D ) ;
                      if ( y->Update != NULL ) y->Update ( y->df , y->item , y->D ) ;
                      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
                  }
                  ret = -1;
                  count = 0;
                  while ( ( ( evnt = kgCheckEvent ( y->D , & kb ) ) != 0 ) && \
                   ( kb.event == 3 ) ) {
                      count++;
                      kbe = kb;
                      if ( count == 50 ) break;
                  }
                  if ( evnt ) kbe = kb;
              } while ( ( kbe ) .event != 2 ) ;
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_r_key ( DIRA *y , KBEVENT kbe ) {
      BRW_STR *br;
      int ret = -1 , ans , df , hitem , j;
      br = y->bwsr;
      ans = kbe.key;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
          br->df = br->hitem+1;
          df = br->df-1;
//          for(j=0;j<y->nitems;j++) y->sw[j]=0;
//          y->sw[df]=1;
          _uiSetSwitch ( y ) ;
          _uiPutRadioButton ( y ) ;
          uiUpdateOn ( br->D ) ;
          return 1;
      }
      if ( ui_Uparrow ( ans ) ) {
          if ( br->hitem >= y->nx ) {
              br->hitem -= y->nx;
              _uiMoveYVertPointer ( y ) ; _uiPutRadioButton ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
          ret = -101;
      }
      if ( ui_Downarrow ( ans ) ) {
          hitem = br->hitem/y->nx;
          if ( hitem < ( y->ny-1 ) ) {
              br->hitem += y->nx;
              if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
              _uiMoveYVertPointer ( y ) ; _uiPutRadioButton ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
          ret = -102;
      }
      return ret;
  }
  int _ui_process_r_click ( DIRA *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = -1 , offset , hitem , xoffset;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp , ix;
      int xx , yy , h , w;
      float fac;
      int df;
      kgWC *wc;
      br = y->bwsr;
      offset = br->offset;
      xoffset = y->offset;
      wc = br->D->wc;
      df = br->df;
      item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
           br->rsx2 , br->rsy1+br->sy+br->shy ) ;
      if ( item ) {
//        printf("_ui_process_r_move\n");
          _ui_process_r_move ( y , kbe ) ;
          return -100;
      }
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = _uiGetSItem ( kbe , br->x1+xoffset , br->y1+offset , \
           br->x2-xoffset-br->w , br->y2-offset ) ;
      if ( item ) {
          if ( kbe.button != 1 ) { uiRest_clip_limits ( wc ) ; return -1; }
          iy = ( kbe.y - ( br->y1+offset ) ) ;
          if ( iy > 0 ) iy = iy/br->width;
          if ( ( iy >= 0 ) && ( iy < y->size ) ) {
              ix = ( kbe.x - ( br->x1+xoffset+br->BxSize/2 ) ) ;
              if ( ix > 0 ) ix = ix/ ( y->lngth+y->xgap ) ;
              if ( ( ix >= 0 ) && ( ix < y->nx ) ) {
                  item = br->pos+iy*y->nx+ix;
                  if ( item < y->nitems ) {
                      br->hitem = item;
                      br->df = item+1;
                      * ( y->df ) = item+1;
                      df = br->df-1;
                      _uiSetSwitch ( y ) ;
                      _uiPutRadioButton ( y ) ;
                      uiUpdateOn ( br->D ) ;
                      ret = 1;
                  }
              } // eof if ( ( ix > 0 ) && ( ix < y->nx ) ) 
          }// eof if ( ( iy > 0 ) && ( iy < y->size ) ) 
      }
      else {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1 , br->rsx2 , br->rsy2 ) ;
          if ( item ) {
              fac = 0.0;
              if ( y->ny-y->size > 0 ) {
                  fac = br->fac;
                  iy = kbe.y - br->rsy1;
                  iy = iy/fac+0.5;
                  br->hitem = iy*y->nx+ ( br->hitem%y->nx ) ;
//              br->hitem =iy*y->nx;
                  if ( br->hitem < 0 ) br->hitem = 0;
                  if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                  _uiMoveYVertPointer ( y ) ; _uiPutRadioButton ( y ) ;
                  uiUpdateOn ( br->D ) ;
                  ret = -100;
              }
              while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
          }
          else {
              item = _uiGetSItem ( kbe , br->tbx1 , br->tby1 , br->tbx2 , br->tby2 ) ;
              if ( item ) {
                  br->hitem = br->pos;
                  if ( br->hitem >= y->nx ) {
                      br->hitem -= y->nx;
                      if ( br->hitem < 0 ) br->hitem = 0;
                      _uiMoveYVertPointer ( y ) ; _uiPutRadioButton ( y ) ;
                      uiUpdateOn ( br->D ) ;
                  }
                  ret = -101;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
              }
              else {
                  item = _uiGetSItem ( kbe , br->bbx1 , br->bby1 , br->bbx2 , br->bby2 ) ;
                  if ( item ) {
                      hitem = br->pos/y->nx;
                      hitem += ( y->size-1 ) ;
                      if ( hitem >= y->ny ) hitem = y->ny-1;
                      br->hitem = hitem*y->nx;
                      if ( hitem < ( y->ny-1 ) ) {
                          br->hitem += y->nx;
                          if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                          _uiMoveYVertPointer ( y ) ; _uiPutRadioButton ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -102;
                      while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
                  }
              }
          }
      }
      uiRest_clip_limits ( wc ) ;
//      printf("ret = %d\n",ret);
      return ( ret ) ;
  }
  int _ui_process_r_move ( DIRA *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , df , hitem , count;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp;
      int xx , yy , h , w;
      float fac;
      kgWC *wc;
      int evnt = 0;
      KBEVENT kb;
      br = y->bwsr;
      wc = br->D->wc;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = 1;
      df = br->df;
//      if ( item) {
      {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
               br->rsx2 , br->rsy1+br->sy+br->shy ) ;
          if ( item ) {
              do {
                  fac = 0.0;
                  yy = br->rsy1;
                  hitem = br->hitem/y->nx;
                  if ( y->ny-y->size > 0 ) {
                      fac = br->fac;
                      iy = kbe.y - yy;
                      iy = iy/fac+0.5;
                      if ( iy >= y->ny ) iy = y->ny-1;
                      if ( iy < 0 ) iy = 0;
                      hitem = br->hitem/y->nx;
                      if ( hitem != iy ) {
                          br->hitem = iy*y->nx;
                          _uiMoveYVertPointer ( y ) ;
                          _uiPutRadioButton ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -100;
                  }
#if 0
                  count = 0;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event == 3 ) {
                      count++;
                      if ( count == 10 ) break;
                  }
#else
                  count = 0;
                  while ( ( ( evnt = kgCheckEvent ( y->D , & kb ) ) != 0 ) && \
                   ( kb.event == 3 ) ) {
                      count++;
                      kbe = kb;
                      if ( count == 50 ) break;
                  }
                  if ( evnt ) kbe = kb;
#endif
              } while ( ( kbe ) .event != 2 ) ;
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_x_key ( DIX *y , KBEVENT kbe ) {
      BRW_STR *br;
      int ret = -1 , ans , df , hitem , j;
      br = y->bwsr;
      ans = kbe.key;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
          br->df = br->hitem+1;
          df = br->df-1;
          _uiSetSwitch ( y ) ;
          _uiPutXmenu ( y ) ;
          uiUpdateOn ( br->D ) ;
          return 1;
      }
      if ( ui_Uparrow ( ans ) ) {
          if ( br->hitem >= y->nx ) {
              br->hitem -= y->nx;
              _uiMoveYVertPointer ( y ) ; _uiPutXmenu ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      if ( ui_Downarrow ( ans ) ) {
          hitem = br->hitem/y->nx;
          if ( hitem < ( y->ny-1 ) ) {
              br->hitem += y->nx;
              if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
              _uiMoveYVertPointer ( y ) ; _uiPutXmenu ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      return ret;
  }
  int _ui_process_x_click ( DIX *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = -1 , offset , hitem , xoffset;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp , ix;
      int xx , yy , h , w;
      float fac;
      int df;
      kgWC *wc;
      br = y->bwsr;
      offset = br->offset;
      xoffset = y->offset;
      wc = br->D->wc;
      df = br->df;
      item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
           br->rsx2 , br->rsy1+br->sy+br->shy ) ;
      if ( item ) {
          _ui_process_x_move ( y , kbe ) ;
          return -100;
      }
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = _uiGetSItem ( kbe , br->x1+xoffset , br->y1+offset , \
           br->x2-xoffset-br->w , br->y2-offset ) ;
      if ( item ) {
          if ( kbe.button != 1 ) { uiRest_clip_limits ( wc ) ; return -1; }
          iy = ( kbe.y - ( br->y1+offset ) ) ;
          if ( iy > 0 ) iy = iy/br->width;
          if ( ( iy >= 0 ) && ( iy < y->size ) ) {
              ix = ( kbe.x - ( br->x1+xoffset ) ) ;
              if ( ix > 0 ) ix = ix/ ( y->lngth+y->xgap ) ;
              if ( y->nx == 1 ) ix = 0;
              if ( ( ix >= 0 ) && ( ix < y->nx ) ) {
                  item = br->pos+iy*y->nx+ix;
                  if ( item < y->nitems ) {
                      br->hitem = item;
                      br->df = item+1;
                      * ( y->df ) = item+1;
                      df = br->df-1;
                      _uiSetSwitch ( y ) ;
                      _uiPutXmenu ( y ) ;
                      uiUpdateOn ( br->D ) ;
                      ret = 1;
                  }
              } // eof if ( ( ix > 0 ) && ( ix < y->nx ) ) 
          }// eof if ( ( iy > 0 ) && ( iy < y->size ) ) 
      }
      else {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1 , br->rsx2 , br->rsy2 ) ;
          if ( item ) {
              fac = 0.0;
              if ( y->ny-y->size > 0 ) {
                  fac = br->fac;
                  iy = kbe.y - br->rsy1;
                  iy = iy/fac+0.5;
                  br->hitem = iy*y->nx+ ( br->hitem%y->nx ) ;
                  if ( br->hitem < 0 ) br->hitem = 0;
                  if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                  _uiMoveYVertPointer ( y ) ; _uiPutXmenu ( y ) ;
                  uiUpdateOn ( br->D ) ;
                  ret = -100;
              }
              while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
          }
          else {
              item = _uiGetSItem ( kbe , br->tbx1 , br->tby1 , br->tbx2 , br->tby2 ) ;
              if ( item ) {
                  br->hitem = br->pos;
                  if ( br->hitem >= y->nx ) {
                      br->hitem -= y->nx;
                      if ( br->hitem < 0 ) br->hitem = 0;
                      _uiMoveYVertPointer ( y ) ; _uiPutXmenu ( y ) ;
                      uiUpdateOn ( br->D ) ;
                  }
                  ret = -101;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
              }
              else {
                  item = _uiGetSItem ( kbe , br->bbx1 , br->bby1 , br->bbx2 , br->bby2 ) ;
                  if ( item ) {
                      hitem = br->pos/y->nx;
                      hitem += ( y->size-1 ) ;
                      if ( hitem >= y->ny ) hitem = y->ny-1;
                      br->hitem = hitem*y->nx;
                      if ( hitem < ( y->ny-1 ) ) {
                          br->hitem += y->nx;
                          if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                          _uiMoveYVertPointer ( y ) ; _uiPutXmenu ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -102;
                      while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
                  }
              }
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_x_move ( DIX *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , df , hitem , count;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp;
      int xx , yy , h , w;
      float fac;
      kgWC *wc;
      int evnt = 0;
      KBEVENT kb;
      br = y->bwsr;
      wc = br->D->wc;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = 1;
      df = br->df;
//      if ( item) {
      {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
               br->rsx2 , br->rsy1+br->sy+br->shy ) ;
          if ( item ) {
              do {
                  fac = 0.0;
                  yy = br->rsy1;
                  hitem = br->hitem/y->nx;
                  if ( y->ny-y->size > 0 ) {
                      fac = br->fac;
                      iy = kbe.y - yy;
                      iy = iy/fac+0.5;
                      if ( iy >= y->ny ) iy = y->ny-1;
                      if ( iy < 0 ) iy = 0;
                      hitem = br->hitem/y->nx;
                      if ( hitem != iy ) {
                          br->hitem = iy*y->nx;
                          _uiMoveYVertPointer ( y ) ;
                          _uiPutXmenu ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -100;
                  }
#if 0
                  count = 0;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event == 3 ) {
                      count++;
                      if ( count == 10 ) break;
                  }
#else
                  count = 0;
                  while ( ( ( evnt = kgCheckEvent ( y->D , & kb ) ) != 0 ) && \
                   ( kb.event == 3 ) ) {
                      count++;
                      kbe = kb;
                      if ( count == 50 ) break;
                  }
                  if ( evnt ) kbe = kb;
#endif
              } while ( ( kbe ) .event != 2 ) ;
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_e_key ( DIE *y , KBEVENT kbe ) {
      BRW_STR *br;
      int ret = -1 , ans , df , hitem , j;
      br = y->bwsr;
      ans = kbe.key;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
          br->df = br->hitem+1;
          df = br->df-1;
          _uiPutEmenu ( y ) ;
          uiUpdateOn ( br->D ) ;
          return 1;
      }
      if ( ui_Uparrow ( ans ) ) {
          if ( br->hitem >= 1 ) {
              br->hitem -= 1;
              _uiMoveEVertPointer ( y ) ; _uiPutEmenu ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      if ( ui_Downarrow ( ans ) ) {
          hitem = br->hitem;
          if ( hitem < ( y->nitems-1 ) ) {
              br->hitem += 1;
              if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
              _uiMoveEVertPointer ( y ) ; _uiPutEmenu ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      return ret;
  }
  int _ui_process_e_click ( DIE *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = -1 , offset , hitem , xoffset;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp , ix;
      int xx , yy , h , w;
      float fac;
      int df;
      kgWC *wc;
      br = y->bwsr;
      offset = br->offset;
      xoffset = y->offset;
      wc = br->D->wc;
      df = br->df;
      item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
           br->rsx2 , br->rsy1+br->sy+br->shy ) ;
      if ( item ) {
          _ui_process_e_move ( y , kbe ) ;
          return -100;
      }
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = _uiGetSItem ( kbe , br->x1+xoffset , br->y1+offset , \
           br->x2-xoffset-br->w , br->y2-offset ) ;
      if ( item ) {
          if ( kbe.button != 1 ) { uiRest_clip_limits ( wc ) ; return -1; }
          iy = ( kbe.y - ( br->y1+offset ) ) ;
          if ( iy > 0 ) iy = iy/br->width;
          if ( ( iy >= 0 ) && ( iy < y->size ) ) {
              ix = ( kbe.x - ( br->x1+xoffset ) ) ;
              if ( ix > 0 ) ix = 0;
              if ( ( ix >= 0 ) ) {
                  item = br->pos+iy+ix;
                  if ( item < y->nitems ) {
                      br->hitem = item;
                      br->df = item+1;
                      * ( y->df ) = item+1;
                      df = br->df-1;
                      _uiPutEmenu ( y ) ;
                      uiUpdateOn ( br->D ) ;
                      ret = 1;
                  }
              } // eof if ( ( ix > 0 ) && ( ix < y->nx ) ) 
          }// eof if ( ( iy > 0 ) && ( iy < y->size ) ) 
      }
      else {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1 , br->rsx2 , br->rsy2 ) ;
          if ( item ) {
              fac = 0.0;
              if ( y->nitems-y->size > 0 ) {
                  fac = br->fac;
                  iy = kbe.y - br->rsy1;
                  iy = iy/fac+0.5;
                  br->hitem = iy;
                  if ( br->hitem < 0 ) br->hitem = 0;
                  if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                  _uiMoveEVertPointer ( y ) ; _uiPutEmenu ( y ) ;
                  uiUpdateOn ( br->D ) ;
                  ret = -100;
              }
              while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
          }
          else {
              item = _uiGetSItem ( kbe , br->tbx1 , br->tby1 , br->tbx2 , br->tby2 ) ;
              if ( item ) {
                  br->hitem = br->pos;
                  if ( br->hitem >= 1 ) {
                      br->hitem -= 1;
                      if ( br->hitem < 0 ) br->hitem = 0;
                      _uiMoveEVertPointer ( y ) ; _uiPutEmenu ( y ) ;
                      uiUpdateOn ( br->D ) ;
                  }
                  ret = -101;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
              }
              else {
                  item = _uiGetSItem ( kbe , br->bbx1 , br->bby1 , br->bbx2 , br->bby2 ) ;
                  if ( item ) {
                      hitem = br->pos;
                      hitem += ( y->size-1 ) ;
                      if ( hitem >= y->nitems ) hitem = y->nitems-1;
                      br->hitem = hitem;
                      if ( hitem < ( y->nitems-1 ) ) {
                          br->hitem += 1;
                          if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                          _uiMoveEVertPointer ( y ) ; _uiPutEmenu ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -102;
                      while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
                  }
              }
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_e_move ( DIE *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , df , hitem , count;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp;
      int xx , yy , h , w;
      float fac;
      kgWC *wc;
      int evnt = 0;
      KBEVENT kb;
      br = y->bwsr;
      wc = br->D->wc;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = 1;
      df = br->df;
//      if ( item) {
      {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
               br->rsx2 , br->rsy1+br->sy+br->shy ) ;
          if ( item ) {
              do {
                  fac = 0.0;
                  yy = br->rsy1;
                  hitem = br->hitem;
                  if ( y->nitems-y->size > 0 ) {
                      fac = br->fac;
                      iy = kbe.y - yy;
                      iy = iy/fac+0.5;
                      if ( iy >= y->nitems ) iy = y->nitems-1;
                      if ( iy < 0 ) iy = 0;
                      hitem = br->hitem;
                      if ( hitem != iy ) {
                          br->hitem = iy;
                          _uiMoveEVertPointer ( y ) ;
                          _uiPutEmenu ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -100;
                  }
#if 0
                  count = 0;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event == 3 ) {
                      count++;
                      if ( count == 10 ) break;
                  }
#else
                  count = 0;
                  while ( ( ( evnt = kgCheckEvent ( y->D , & kb ) ) != 0 ) && \
                   ( kb.event == 3 ) ) {
                      count++;
                      kbe = kb;
                      if ( count == 50 ) break;
                  }
                  if ( evnt ) kbe = kb;
#endif
              } while ( ( kbe ) .event != 2 ) ;
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_m_key ( DIS *y , KBEVENT kbe ) {
      BRW_STR *br;
      int ret = -1 , ans , df , hitem , j;
      br = y->bwsr;
      ans = kbe.key;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
          return 1;
      }
      if ( ui_Uparrow ( ans ) ) {
          if ( br->hitem >= 1 ) {
              br->hitem -= 1;
              _uiMoveMVertPointer ( y ) ; _uiPutMmenu ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      if ( ui_Downarrow ( ans ) ) {
          hitem = br->hitem;
          if ( hitem < ( y->nitems-1 ) ) {
              br->hitem += 1;
              if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
              _uiMoveMVertPointer ( y ) ; _uiPutMmenu ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      return ret;
  }
  int _ui_process_m_click ( DIS *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = -1 , offset , hitem , xoffset;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp , ix;
      int xx , yy , h , w;
      float fac;
      int df;
      kgWC *wc;
      br = y->bwsr;
      offset = br->offset;
      xoffset = y->offset;
      wc = br->D->wc;
      df = br->df;
      item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
           br->rsx2 , br->rsy1+br->sy+br->shy ) ;
      if ( item ) {
          _ui_process_m_move ( y , kbe ) ;
          return -100;
      }
      item = _uiGetSItem ( kbe , br->x1+xoffset , br->y1+offset , \
           br->x2-xoffset-br->w , br->y2-offset ) ;
      if ( item ) {
          if ( kbe.button != 1 ) return -1;
      }
      else {
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1 , br->rsx2 , br->rsy2 ) ;
          if ( item ) {
              fac = 0.0;
              if ( y->nitems-y->size > 0 ) {
                  fac = br->fac;
                  iy = kbe.y - br->rsy1;
                  iy = iy/fac+0.5;
                  br->hitem = iy;
                  if ( br->hitem < 0 ) br->hitem = 0;
                  if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                  _uiMoveMVertPointer ( y ) ; _uiPutMmenu ( y ) ;
                  uiUpdateOn ( br->D ) ;
                  ret = -100;
              }
              while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
          }
          else {
              item = _uiGetSItem ( kbe , br->tbx1 , br->tby1 , br->tbx2 , br->tby2 ) ;
              if ( item ) {
                  br->hitem = br->pos;
                  if ( br->hitem >= 1 ) {
                      br->hitem -= 1;
                      if ( br->hitem < 0 ) br->hitem = 0;
                      _uiMoveMVertPointer ( y ) ; _uiPutMmenu ( y ) ;
                      uiUpdateOn ( br->D ) ;
                  }
                  ret = -101;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
              }
              else {
                  item = _uiGetSItem ( kbe , br->bbx1 , br->bby1 , br->bbx2 , br->bby2 ) ;
                  if ( item ) {
                      hitem = br->pos;
                      hitem += ( y->size-1 ) ;
                      if ( hitem >= y->nitems ) hitem = y->nitems-1;
                      br->hitem = hitem;
                      if ( hitem < ( y->nitems-1 ) ) {
                          br->hitem += 1;
                          if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                          _uiMoveMVertPointer ( y ) ; _uiPutMmenu ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -102;
                      while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
                  }
              }
          }
          uiRest_clip_limits ( wc ) ;
      }
      return ( ret ) ;
  }
  int _ui_process_m_move ( DIS *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , df , hitem , count;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp;
      int xx , yy , h , w;
      float fac;
      kgWC *wc;
      int evnt = 0;
      KBEVENT kb;
      br = y->bwsr;
      wc = br->D->wc;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = 1;
      df = br->df;
//      if ( item) {
      {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
               br->rsx2 , br->rsy1+br->sy+br->shy ) ;
          if ( item ) {
              do {
                  fac = 0.0;
                  yy = br->rsy1;
                  hitem = br->hitem;
                  if ( y->nitems-y->size > 0 ) {
                      fac = br->fac;
                      iy = kbe.y - yy;
                      iy = iy/fac+0.5;
                      if ( iy >= y->nitems ) iy = y->nitems-1;
                      if ( iy < 0 ) iy = 0;
                      hitem = br->hitem;
                      if ( hitem != iy ) {
                          br->hitem = iy;
                          _uiMoveMVertPointer ( y ) ;
                          _uiPutMmenu ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -100;
                  }
#if 0
                  count = 0;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event == 3 ) {
                      count++;
                      if ( count == 10 ) break;
                  }
#else
                  count = 0;
                  while ( ( ( evnt = kgCheckEvent ( y->D , & kb ) ) != 0 ) && \
                   ( kb.event == 3 ) ) {
                      count++;
                      kbe = kb;
                      if ( count == 50 ) break;
                  }
                  if ( evnt ) kbe = kb;
#endif
              } while ( ( kbe ) .event != 2 ) ;
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_w_key ( DIW *y , KBEVENT kbe ) {
      BRW_STR *br;
      int ret = -1 , ans , df , hitem , j;
      br = y->bwsr;
      ans = kbe.key;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
          br->df = br->hitem+1;
          df = br->df-1;
          _uiPutWmenu ( y ) ;
          uiUpdateOn ( br->D ) ;
          return 1;
      }
      if ( ui_Uparrow ( ans ) ) {
          if ( br->hitem >= 1 ) {
              br->hitem -= 1;
              _uiMoveWVertPointer ( y ) ; _uiPutWmenu ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      if ( ui_Downarrow ( ans ) ) {
          hitem = br->hitem;
          if ( hitem < ( y->nitems-1 ) ) {
              br->hitem += 1;
              if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
              _uiMoveWVertPointer ( y ) ; _uiPutWmenu ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      return ret;
  }
  int _ui_process_w_click ( DIW *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , offset , hitem , xoffset;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp , ix;
      int xx , yy , h , w;
      float fac;
      int df;
      kgWC *wc;
      br = y->bwsr;
      offset = br->offset;
      xoffset = y->offset;
      wc = br->D->wc;
      df = br->df;
      item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
           br->rsx2 , br->rsy1+br->sy+br->shy ) ;
      if ( item ) {
          _ui_process_w_move ( y , kbe ) ;
          return -1;
      }
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = _uiGetSItem ( kbe , br->x1+xoffset , br->y1+offset , \
           br->x2-xoffset-br->w , br->y2-offset ) ;
      if ( item ) {
          if ( kbe.button != 1 ) { uiRest_clip_limits ( wc ) ; return -1; }
          iy = ( kbe.y - ( br->y1+offset ) ) /br->width;
          if ( iy < 0 ) iy = 0;
          if ( iy >= y->size ) iy = y->size-1;
          ix = ( kbe.x - ( br->x1 ) ) / ( y->width ) ;
          if ( ix < 0 ) ix = 0;
          if ( ix >= 1 ) ix = 0;
          if ( item < 0 ) item = 1;
          item = br->pos+iy+ix;
          if ( item >= y->nitems ) item = y->nitems-1;
          br->hitem = item;
          br->df = item+1;
          * ( y->df ) = item+1;
//       if(df != br->df) {
//         _uiMoveYVertPointer(y);
//       }
          df = br->df-1;
          _uiPutWmenu ( y ) ;
          uiUpdateOn ( br->D ) ;
          ret = 1;
      }
      else {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1 , br->rsx2 , br->rsy2 ) ;
          if ( item ) {
              fac = 0.0;
              if ( y->nitems-y->size > 0 ) {
                  fac = br->fac;
                  iy = kbe.y - br->rsy1;
                  iy = iy/fac+0.5;
                  br->hitem = iy;
                  if ( br->hitem < 0 ) br->hitem = 0;
                  if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                  _uiMoveWVertPointer ( y ) ; _uiPutWmenu ( y ) ;
                  uiUpdateOn ( br->D ) ;
                  ret = -100;
              }
              while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
          }
          else {
              item = _uiGetSItem ( kbe , br->tbx1 , br->tby1 , br->tbx2 , br->tby2 ) ;
              if ( item ) {
                  br->hitem = br->pos;
                  if ( br->hitem >= 1 ) {
                      br->hitem -= 1;
                      if ( br->hitem < 0 ) br->hitem = 0;
                      _uiMoveWVertPointer ( y ) ; _uiPutWmenu ( y ) ;
                      uiUpdateOn ( br->D ) ;
                  }
                  ret = -1;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
              }
              else {
                  item = _uiGetSItem ( kbe , br->bbx1 , br->bby1 , br->bbx2 , br->bby2 ) ;
                  if ( item ) {
                      hitem = br->pos;
                      hitem += ( y->size-1 ) ;
                      if ( hitem >= y->nitems ) hitem = y->nitems-1;
                      br->hitem = hitem;
                      if ( hitem < ( y->nitems-1 ) ) {
                          br->hitem += 1;
                          if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                          _uiMoveWVertPointer ( y ) ; _uiPutWmenu ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -1;
                      while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
                  }
              }
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_w_move ( DIW *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , df , hitem , count;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp;
      int xx , yy , h , w;
      float fac;
      kgWC *wc;
      int evnt = 0;
      KBEVENT kb;
      br = y->bwsr;
      wc = br->D->wc;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = 1;
      df = br->df;
//      if ( item) {
      {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
               br->rsx2 , br->rsy1+br->sy+br->shy ) ;
          if ( item ) {
              do {
                  fac = 0.0;
                  yy = br->rsy1;
                  hitem = br->hitem;
                  if ( y->nitems-y->size > 0 ) {
                      fac = br->fac;
                      iy = kbe.y - yy;
                      iy = iy/fac+0.5;
                      if ( iy >= y->nitems ) iy = y->nitems-1;
                      if ( iy < 0 ) iy = 0;
                      hitem = br->hitem;
                      if ( hitem != iy ) {
                          br->hitem = iy;
                          _uiMoveWVertPointer ( y ) ;
                          _uiPutWmenu ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -1;
                  }
#if 0
                  count = 0;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event == 3 ) {
                      count++;
                      if ( count == 10 ) break;
                  }
#else
                  count = 0;
                  while ( ( ( evnt = kgCheckEvent ( y->D , & kb ) ) != 0 ) && \
                   ( kb.event == 3 ) ) {
                      count++;
                      kbe = kb;
                      if ( count == 50 ) break;
                  }
                  if ( evnt ) kbe = kb;
#endif
              } while ( ( kbe ) .event != 2 ) ;
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_c_key ( DICH *y , KBEVENT kbe ) {
      BRW_STR *br;
      int ret = -1 , ans , df , hitem , j;
      br = y->bwsr;
      ans = kbe.key;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
          br->df = br->hitem+1;
          df = br->df-1;
//          if(y->sw[df]==0) y->sw[df]=1;
//          else if(y->sw[df]==1) y->sw[df]=0;
          _uiSetSwitch ( y ) ;
          _uiPutCheckBox ( y ) ;
          uiUpdateOn ( br->D ) ;
          return 1;
      }
      if ( ui_Uparrow ( ans ) ) {
          if ( br->hitem >= y->nx ) {
              br->hitem -= y->nx;
              _uiMoveYVertPointer ( y ) ; _uiPutCheckBox ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      if ( ui_Downarrow ( ans ) ) {
          hitem = br->hitem/y->nx;
          if ( hitem < ( y->ny-1 ) ) {
              br->hitem += y->nx;
              if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
              _uiMoveYVertPointer ( y ) ; _uiPutCheckBox ( y ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      return ret;
  }
  int _ui_process_c_click ( DICH *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = -1 , offset , hitem , xoffset;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp , ix;
      int xx , yy , h , w;
      float fac;
      int df;
      kgWC *wc;
      br = y->bwsr;
      offset = br->offset;
      xoffset = y->offset;
      wc = br->D->wc;
      df = br->df;
      item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
           br->rsx2 , br->rsy1+br->sy+br->shy ) ;
      if ( item ) {
          _ui_process_c_move ( y , kbe ) ;
          return -100;
      }
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = _uiGetSItem ( kbe , br->x1+xoffset , br->y1+offset , \
           br->x2-xoffset-br->w , br->y2-offset ) ;
      if ( item ) {
          if ( kbe.button != 1 ) { uiRest_clip_limits ( wc ) ; return -1; }
          iy = ( kbe.y - ( br->y1+offset ) ) ;
          if ( iy > 0 ) iy = iy/br->width;
          if ( ( iy >= 0 ) && ( iy < y->size ) ) {
              ix = ( kbe.x - ( br->x1+xoffset+br->BxSize/2 ) ) ;
              if ( ix > 0 ) ix = ix/ ( y->lngth+y->xgap ) ;
              if ( ( ix >= 0 ) && ( ix < y->nx ) ) {
                  item = br->pos+iy*y->nx+ix;
                  if ( item < y->nitems ) {
                      br->hitem = item;
                      br->df = item+1;
                      * ( y->df ) = item+1;
                      df = br->df-1;
                      _uiSetSwitch ( y ) ;
                      _uiPutCheckBox ( y ) ;
                      uiUpdateOn ( br->D ) ;
                      ret = 1;
                  }
              } // eof if ( ( ix > 0 ) && ( ix < y->nx ) ) 
          }// eof if ( ( iy > 0 ) && ( iy < y->size ) ) 
      }
      else {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1 , br->rsx2 , br->rsy2 ) ;
          if ( item ) {
              fac = 0.0;
              if ( y->ny-y->size > 0 ) {
                  fac = br->fac;
                  iy = kbe.y - br->rsy1;
                  iy = iy/fac+0.5;
                  br->hitem = iy*y->nx+ ( br->hitem%y->nx ) ;
                  if ( br->hitem < 0 ) br->hitem = 0;
                  if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                  _uiMoveYVertPointer ( y ) ; _uiPutCheckBox ( y ) ;
                  uiUpdateOn ( br->D ) ;
                  ret = -100;
              }
              while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
          }
          else {
              item = _uiGetSItem ( kbe , br->tbx1 , br->tby1 , br->tbx2 , br->tby2 ) ;
              if ( item ) {
                  br->hitem = br->pos;
                  if ( br->hitem >= y->nx ) {
                      br->hitem -= y->nx;
                      if ( br->hitem < 0 ) br->hitem = 0;
                      _uiMoveYVertPointer ( y ) ; _uiPutCheckBox ( y ) ;
                      uiUpdateOn ( br->D ) ;
                  }
                  ret = -101;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
              }
              else {
                  item = _uiGetSItem ( kbe , br->bbx1 , br->bby1 , br->bbx2 , br->bby2 ) ;
                  if ( item ) {
                      hitem = br->pos/y->nx;
                      hitem += ( y->size-1 ) ;
                      if ( hitem >= y->ny ) hitem = y->ny-1;
                      br->hitem = hitem*y->nx;
                      if ( hitem < ( y->ny-1 ) ) {
                          br->hitem += y->nx;
                          if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
                          _uiMoveYVertPointer ( y ) ; _uiPutCheckBox ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -102;
                      while ( ( kbe = kgGetEvent ( y->D ) ) .event != 2 ) ;
                  }
              }
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_process_c_move ( DICH *y , KBEVENT kbe ) {
      BRW_STR *br;
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , df , hitem , count;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp;
      int xx , yy , h , w;
      float fac;
      kgWC *wc;
      int evnt = 0;
      KBEVENT kb;
      br = y->bwsr;
      wc = br->D->wc;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      item = 1;
      df = br->df;
//      if ( item) {
      {
          item = _uiGetSItem ( kbe , br->rsx1 , br->rsy1+br->sy , \
               br->rsx2 , br->rsy1+br->sy+br->shy ) ;
          if ( item ) {
              do {
                  fac = 0.0;
                  yy = br->rsy1;
                  hitem = br->hitem/y->nx;
                  if ( y->ny-y->size > 0 ) {
                      fac = br->fac;
                      iy = kbe.y - yy;
                      iy = iy/fac+0.5;
                      if ( iy >= y->ny ) iy = y->ny-1;
                      if ( iy < 0 ) iy = 0;
                      hitem = br->hitem/y->nx;
                      if ( hitem != iy ) {
                          br->hitem = iy*y->nx;
                          _uiMoveYVertPointer ( y ) ;
                          _uiPutCheckBox ( y ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -100;
                  }
#if 0
                  count = 0;
                  while ( ( kbe = kgGetEvent ( y->D ) ) .event == 3 ) {
                      count++;
                      if ( count == 10 ) break;
                  }
#else
                  count = 0;
                  while ( ( ( evnt = kgCheckEvent ( y->D , & kb ) ) != 0 ) && \
                   ( kb.event == 3 ) ) {
                      count++;
                      kbe = kb;
                      if ( count == 50 ) break;
                  }
                  if ( evnt ) kbe = kb;
#endif
              } while ( ( kbe ) .event != 2 ) ;
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_scroll_msg_item ( BRW_STR *br , KBEVENT kbe ) {
      int ret = 1 , item , df;
      kgWC *wc;
      wc = br->D->wc;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      df = br->hitem;
      item = _uiGetSItem ( kbe , br->MS.ixx , br->MS.iyy+br->MS.thirty/2 , \
           br->MS.ixu , br->MS.iyu-br->MS.thirty/2 ) ;
      if ( item ) {
          item = ( kbe.y - ( br->MS.iyy+br->MS.thirty/2 ) ) /br->MS.thirty+1;
          if ( item < 1 ) item = 1;
          if ( item > br->MS.imenu ) item = br->MS.imenu;
          br->MS.item = item;
          br->hitem = br->pos+item;
          if ( br->hitem < 1 ) br->hitem = 1;
          if ( br->hitem > br->MS.nitems ) br->hitem = br->MS.nitems;
          if ( df != br->hitem ) {
              _uiMoveVertPointer ( br ) ; _uiPutmsg ( br ) ;
              uiUpdateOn ( br->D ) ;
          }
          ret = 1;
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_scroll_msg_key ( BRW_STR *br , KBEVENT kbe ) {
      int ret = -1 , ans;
      ans = kbe.key;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
          return 1;
      }
      if ( ui_Uparrow ( ans ) ) {
          if ( br->hitem > 1 ) {
              br->hitem -= 1;
              _uiMoveVertPointer ( br ) ; _uiPutmsg ( br ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      if ( ui_Downarrow ( ans ) ) {
          if ( br->hitem < br->MS.nitems ) {
              br->hitem += 1;
              _uiMoveVertPointer ( br ) ; _uiPutmsg ( br ) ;
              uiUpdateOn ( br->D ) ;
          }
      }
      return ret;
  }
  int _ui_scroll_msg_click ( BRW_STR *br , KBEVENT kbe ) {
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp;
      int xx , yy , h , w;
      float fac;
      int df;
      kgWC *wc;
      wc = br->D->wc;
      df = br->hitem;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      ixmid = br->MS.ixx+br->MS.ixl/2;
      iy = br->MS.iyy+1;
      k = 1;
      if ( ( br->MS.item != 0 ) && ( br->MS.item <= br->MS.imenu ) ) k = br->MS.item;
          
      jj = iy + ( k ) *br->MS.thirty;
      kk = -1;
      item = _uiGetSItem ( kbe , br->MS.ixx , br->MS.iyy+br->MS.thirty/2 , \
           br->MS.ixu , br->MS.iyu-br->MS.thirty/2 ) ;
      if ( item ) {
          item = ( kbe.y - ( br->MS.iyy+br->MS.thirty/2 ) ) /br->MS.thirty+1;
          if ( item < 1 ) item = 1;
          if ( item > br->MS.imenu ) item = br->MS.imenu;
          br->MS.item = item;
          br->hitem = br->pos+item;
          if ( df != br->hitem ) {
              _uiMoveVertPointer ( br ) ;
              _uiPutmsg ( br ) ;
              uiUpdateOn ( br->D ) ;
          }
          ret = 1;
      }
      else {
          w = 24;
          h = br->MS.iyl+20;
          xx = br->MS.ixx+br->MS.ixl;
          yy = br->MS.iyy-10+w+4;
          item = _uiGetSItem ( kbe , xx+3 , yy , xx+w-3 , yy+h-2*w-8 ) ;
          if ( item ) {
              fac = 0.0;
              if ( br->MS.nitems-br->size > 0 ) {
                  fac = br->MS.fac;
                  iy = kbe.y - yy;
                  iy = iy/fac+0.5;
                  br->hitem = iy;
                  if ( br->hitem < 1 ) br->hitem = 1;
                  if ( br->hitem > br->MS.nitems ) br->hitem = br->MS.nitems;
                  _uiMoveVertPointer ( br ) ;
                  _uiPutmsg ( br ) ;
                  uiUpdateOn ( br->D ) ;
                  ret = -1;
              }
          }
          else {
              xx = br->MS.ixx+br->MS.ixl;
              yy = br->MS.iyy-10;
              item = _uiGetSItem ( kbe , xx , yy , xx+w , yy+w ) ;
              if ( item ) {
//            if(br->hitem >br->size) {
                  br->hitem -= 1;
                  if ( br->hitem < 1 ) br->hitem = 1;
                  _uiMoveVertPointer ( br ) ;
                  _uiPutmsg ( br ) ;
                  uiUpdateOn ( br->D ) ;
//            }
                  ret = -1;
              }
              else {
                  xx = br->MS.ixx+br->MS.ixl;
                  yy = br->MS.iyy-10+h-w;
                  item = _uiGetSItem ( kbe , xx , yy , xx+w , yy+w-4 ) ;
                  if ( item ) {
                      if ( br->hitem < br->MS.nitems ) {
                          if ( br->hitem <= br->size ) { br->hitem = br->size+1; }
                          else br->hitem += 1;
                          _uiMoveVertPointer ( br ) ;
                          _uiPutmsg ( br ) ;
                          uiUpdateOn ( br->D ) ;
                      }
                      ret = -1;
                  }
              }
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  int _ui_scroll_msg_move ( BRW_STR *br , KBEVENT kbe ) {
      unsigned int cur_c , temp , ch , cr = 0x0d , esc;
      int j , item , ret = 0 , df;
      int k , kk , knew , jj , ixmid;
      int n , iy , iyp , ixp;
      int xx , yy , h , w;
      float fac;
      kgWC *wc;
      wc = br->D->wc;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      ixmid = br->MS.ixx+br->MS.ixl/2;
      iy = br->MS.iyy+1;
      k = 1;
      if ( ( br->MS.item != 0 ) && ( br->MS.item <= br->MS.imenu ) ) k = br->MS.item;
          
      jj = iy + ( k ) *br->MS.thirty;
      kk = -1;
      item = 1;
      df = br->hitem;
      //item=_uiGetSItem(kbe,br->MS.ixx,br->MS.iyy+br->MS.thirty/2,br->MS.ixu,br->MS.iyu-br->MS.thirty/2);
      if ( item ) {
          w = 24;
          h = br->MS.iyl+20;
          xx = br->MS.ixx+br->MS.ixl;
          yy = br->MS.iyy-10+w+4;
          item = _uiGetSItem ( kbe , xx+3 , yy , xx+w-3 , yy+h-2*w-8 ) ;
          if ( item ) {
              fac = 0.0;
              if ( br->MS.nitems-br->size > 0 ) {
                  fac = br->MS.fac;
                  iy = kbe.y - yy;
                  iy = iy/fac+0.5;
                  br->hitem = iy;
                  if ( br->hitem < 1 ) br->hitem = 1;
                  if ( br->hitem > br->MS.nitems ) br->hitem = br->MS.nitems;
                  if ( br->hitem != df ) {_uiMoveVertPointer ( br ) ;
                  _uiPutmsg ( br ) ; uiUpdateOn ( br->D ) ; }
                  ret = -1;
              }
          }
      }
      uiRest_clip_limits ( wc ) ;
      return ( ret ) ;
  }
  void _uiGetResolution ( void ) {
//      Get_resolution(&EVGAX,&EVGAY);
      return;
  }
  void _ui_draw_menu_bound ( DIALOG *D , int x1 , \
       int y1 , int x2 , int y2 , int c_bound ) \
  {
      unsigned int temp;
      int EVGAY;
      temp = WC ( D )->c_color;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      uiset_clr ( D , 0 ) ;
      _ui_h_line ( WC ( D ) , ( int ) x1 , ( int ) x2 , ( int ) y1 ) ;
      _ui_h_line ( WC ( D ) , ( int ) x1 , ( int ) x2 , ( int ) y2 ) ;
      _ui_v_line ( WC ( D ) , ( int ) y1 , ( int ) y2 , ( int ) x2 ) ;
      _ui_v_line ( WC ( D ) , ( int ) y1 , ( int ) y2 , ( int ) x1 ) ;
      uiset_clr ( D , c_bound ) ;
      _ui_h_line ( WC ( D ) , ( int ) x1+1 , ( int ) x2-2 , ( int ) y1-1 ) ;
      _ui_v_line ( WC ( D ) , ( int ) y1-1 , ( int ) y2+2 , ( int ) x1+1 ) ;
      uiset_clr ( D , temp ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
      return;
  }
  void _uiPutmenu ( BRW_STR *br ) {
      unsigned int cur_c , temp , ch , cr , esc;
      int j , pos;
      int k , kk , knew , jj;
      int n , iy , iyp , ixp , ixmid , ln;
      int scroll = 1;
      int EVGAY;
      DIALOG *D;
      D = ( DIALOG * ) ( br->D ) ;
      EVGAY = D->evgay;
      if ( br->D == NULL ) {
          printf ( "Error: In uiMenu : Parent=NULL\n" ) ;
          exit ( 0 ) ;
      }
      ln = abs ( br->MS.ixl ) *0.85+0.999;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      n = br->MS.nitems;
      if ( br->pos < ( br->hitem-br->size ) ) br->pos = br->hitem-br->size;
      if ( br->pos >= br->hitem ) br->pos = br->hitem-1;
      if ( br->pos > ( n - br->size ) ) br->pos = n-br->size;
      pos = br->pos;
      if ( pos < 0 ) { pos = 0; br->MS.item = br->hitem; }
      else br->MS.item = br->hitem - pos;
      br->MS.menu = br->menu+pos;
      br->pos = pos;
      ixmid = br->MS.ixx+br->MS.ixl/2;
      iy = br->MS.iyy+1;
      _uirect_fill ( WC ( D ) , ( br->MS.ixx+1 ) , ( EVGAY-br->MS.iyy ) , \
       ( br->MS.ixu-1 ) , ( EVGAY-br->MS.iyu ) , br->MS.color1 ) ;
#if 0
#else
#if 1
      if ( br->D != NULL ) _ui_draw_bound ( D , br->MS.ixx+1 , \
           ( EVGAY-br->MS.iyy-1 ) , br->MS.ixu-1 , \
       ( EVGAY-br->MS.iyu+1 ) , br->MS.color2 ) ;
      else _ui_draw_menu_bound ( D , br->MS.ixx , \
           ( EVGAY-br->MS.iyy ) , br->MS.ixu , \
       ( EVGAY-br->MS.iyu ) , br->MS.color2 ) ;
#else
      _ui_draw_menu_bound ( D , br->MS.ixx , ( EVGAY-br->MS.iyy ) , br->MS.ixu , \
       ( EVGAY-br->MS.iyu ) , br->MS.color2 ) ;
#endif
#endif
      k = 1;
      if ( ( br->MS.item > 0 ) && ( br->MS.item <= br->MS.imenu ) ) k = br->MS.item;
      jj = iy + ( k ) *br->MS.thirty;
      _uirect_fill ( WC ( D ) , ( br->MS.ixx+4 ) , ( EVGAY-jj-12 ) , \
       ( br->MS.ixu-4 ) , ( EVGAY-jj+8 ) , br->MS.color2 ) ;
      jj = iy;
//TCB
      uiDefaultGuiFontSize ( D ) ;
      for ( kk = 0; kk < br->MS.imenu; kk++ ) {
          jj = iy + kk*br->MS.thirty+br->MS.thirty;
          ixp = br->MS.ixx+10;
#if 0
          iyp = EVGAY-jj-7;
          uiwrite_string ( D , br->MS.menu [ kk ] , ( int ) ixp , \
           ( int ) iyp , br->MS.char_clr ) ;
#else
          ixp = br->MS.ixx+5;
          iyp = jj+12-br->MS.thirty;
          uiString ( D , br->MS.menu [ kk ] , ( int ) ixp , ( int ) iyp , ln , br->MS.thirty , \
               D->gc.MsgFont , D->gc.msg_char , D->gc.FontSize , -1 , -1 ) ;
#endif
      }
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _uiPutYmenu ( DIY *y ) {
      BRW_STR *br;
      ThumbNail **menu;
      ThumbNail **list;
      unsigned int cur_c , temp , ch , cr , esc;
      int j , pos;
      int k , kk , knew , jj , ln;
      int n , iy , iyp , ixp , ixmid;
      int scroll = 1 , nx , ny , yi , xi , scrsize , xoffset , yoffset , th , w;
      int swv;
      int EVGAY;
      DIALOG *D;
      void *xpm = NULL;
      GMIMG *img;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      EVGAY = D->evgay;
//      int EVGAY,EVGAX;
      if ( br->D == NULL ) {
          printf ( "Error: In uiPutYMenu : Parent=NULL\n" ) ;
          exit ( 0 ) ;
      }
      list = ( ThumbNail ** ) y->list;
      k = 0; while ( list [ k ] != NULL ) {if ( list [ k ]->sw != 0 ) list [ k ]->sw = 1;
          
      k++; }
      scroll = br->scroll;
      w = y->w*scroll;
      th = y->ygap-2*D->gc.FontSize-2;
      yoffset = br->offset;
      xoffset = y->offset;
//      if(xoffset==0 ) xoffset=2;
      nx = y->nx;
      scrsize = y->size*y->nx;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      ln = br->y2-br->y1-2*yoffset; ;
      n = y->nitems;
      pos = br->pos;
      if ( pos < 0 ) { pos = 0; }
      if ( br->hitem < 0 ) br->hitem = 0;
      if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
      while ( br->hitem > ( pos+scrsize ) ) pos+= y->nx;
      if ( pos >= y->nitems ) pos = y->nitems-1;
      br->pos = pos;
      pos = ( pos/y->nx ) *y->nx;
      menu = list+pos;
      if ( y->bkgr == 1 ) _dvrect_fill ( WC ( D ) ,  \
          ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
       ( br->x2-xoffset-w ) , ( br->y2-xoffset ) , D->gc.twin_fill ) ;
      else _dvrect_fill ( WC ( D ) , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
       ( br->x2-xoffset-w ) , ( br->y2-xoffset ) , D->gc.fill_clr ) ;
      if ( y->bordr == 1 ) {
          _dv_draw_bound ( D , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
           ( br->x2-xoffset-w ) , ( br->y2-xoffset ) , D->gc.twin_bodr ) ;
          _dv_draw_bound ( D , ( br->x1+xoffset+1 ) , ( br->y1+xoffset+1 ) , \
           ( br->x2-xoffset-w-1 ) , ( br->y2-xoffset-1 ) , D->gc.twin_bodr ) ;
//      _dv_draw_bound(D,(br->x1+xoffset+2),(br->y1+xoffset+2),(br->x2-xoffset-w-2), (br->y2-xoffset-2),D->gc.twin_bodr);
      }
      jj = iy;
      uiDefaultGuiFontSize ( D ) ;
#if 1
      if ( y->itemhi ) {
          xi = ( br->hitem-pos ) %nx;
          yi = ( br->hitem-pos ) /nx;
          ixp = br->x1+xoffset+xi* ( y->lngth+y->xgap ) +br->xshift;
          iyp = br->y1+yoffset+yi*br->width;
          _dvrect_fill ( WC ( D ) , ixp , iyp , ixp+y->lngth+y->xgap , \
               iyp+y->width+6 , D->gc.ItemHighColor ) ;
      }
#endif
      kk = 0;
      ny = y->size;
      if ( y->imgs == NULL ) uiMakeYImages ( y ) ;
      for ( yi = 0; yi < ny; yi++ ) for ( xi = 0; xi < nx; xi++ ) {
          if ( kk+pos >= y->nitems ) continue;
          xpm = NULL;
          if ( list != NULL ) xpm = list [ kk+pos ]->img;
          if ( list != NULL ) swv = list [ kk+pos ]->sw;
          else {if ( kk+pos == br->df-1 ) swv = 1; else swv = 0; }
          ixp = br->x1+xoffset+xi* ( y->lngth+y->xgap ) +br->xshift;
          iyp = br->y1+yoffset+yi*br->width;
          if ( xpm != NULL ) {
              img = ( GMIMG * ) xpm;
              if ( swv == 1 ) { img->bkgrclr = D->gc.SplashCharColor; }
              else img->bkgrclr = D->gc.twin_fill;
              kgImage ( D , xpm , ixp+2 , iyp+2 , y->lngth+y->xgap-4 , \
                   y->width+2 , 0.0 , 1.0 ) ;
          }
	//TCB NEW
#if 0
          iyp = iyp+br->width-th;
#else
          if ( th > 6 ) th = 6;
          iyp = iyp+y->width+th;
//          iyp = iyp+y->width+y->ygap-2*D->gc.FontSize-2;
#endif
//            uiString(D,menu[kk]->name,ixp,iyp,y->width+y->xgap,th,D->gc.MsgFont,D->gc.info_char,D->gc.FontSize,0,-1);
    //      if ( ( list != NULL ) && ( menu [ kk ]->name != NULL ) ) kgImage ( D , y->imgs [ kk+pos ] , ixp , iyp , y->lngth+y->xgap , th , 0.0 , 1.0 ) ;
          if ( ( list != NULL ) && ( menu [ kk ]->name != NULL ) ) kgImage ( D , y->imgs [ kk+pos ] , \
               ixp , iyp , y->lngth+y->xgap , 2*D->gc.FontSize+2 , 0.0 , 1.0 ) ;
          kk++;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _uiPutCheckBox ( DICH *y ) {
      BRW_STR *br;
      ThumbNail **menu , **list;
      unsigned int cur_c , temp , ch , cr , esc;
      int j , pos;
      int k , kk , knew , jj , ln , ln1;
      int n , iy , iyp , ixp , ixmid;
      int scroll = 1 , *sw , swv , nx , ny , yi , xi , scrsize , \
           xoffset , yoffset , th , w , xgap , lngth , bxln;
      int EVGAY;
      DIALOG *D;
      void *xpm = NULL;
      GMIMG *img;
      int dx , dy;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      EVGAY = D->evgay;
//      int EVGAY,EVGAX;
      if ( br->D == NULL ) {
          printf ( "Error: In uiPutYMenu : Parent=NULL\n" ) ;
          exit ( 0 ) ;
      }
      br->BxSize = 2*D->gc.FontSize;
      list = ( ThumbNail ** ) y->list;
      k = 0 ; while ( list [ k ] != NULL ) {if ( list [ k ]->sw != 0 ) list [ k ]->sw = 1;
          
      k++; }
      scroll = br->scroll;
      w = y->w*scroll;
      th = y->width+y->ygap;
      yoffset = br->offset;
      xoffset = y->offset;
      nx = y->nx;
      xgap = y->xgap;
      lngth = y->lngth;
      if ( nx == 1 ) {
          ln = ( y->x2 -y->x1-2*y->offset-w ) ;
          ln1 = ln*0.95;
          if ( lngth > ln1 ) lngth = ln1;
          xgap = ( ln-lngth ) ;
      }
      scrsize = y->size*y->nx;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      ln = br->y2-br->y1-2*br->offset; ;
      n = y->nitems;
      pos = br->pos;
      if ( pos < 0 ) { pos = 0; }
      if ( br->hitem < 0 ) br->hitem = 0;
      if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
      while ( br->hitem > ( pos+scrsize ) ) pos+= y->nx;
      if ( pos >= y->nitems ) pos = y->nitems-1;
      br->pos = pos;
      pos = ( pos/y->nx ) *y->nx;
      menu = NULL;
      if ( list != NULL ) menu = list+pos;
#ifndef D_RESTORE
//      _dvrect_fill(WC(D),(br->x1+xoffset),(br->y1+xoffset),(br->x2-xoffset-w)-1, (br->y2-xoffset),D->gc.fill_clr);
      if ( y->bkgr == 1 ) {
          _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
           ( offset ) , br->y2- ( offset ) , gc.txt_fill ) ;
      }
      else {
          _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
           ( offset ) , br->y2- ( offset ) , gc.fill_clr ) ;
      }
#else
      dx = ( br->x2-xoffset-w ) - ( br->x1+xoffset ) ;
      dy = ( br->y2-xoffset ) - ( br->y1+xoffset ) +1;
      kgRestoreImagePart ( D , y->Bimg , ( br->x1+xoffset ) , \
       ( br->y1+xoffset ) , xoffset , xoffset , dx , dy ) ;
#endif
      if ( y->bordr == 1 ) {
          _dv_draw_bound ( D , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
               ( br->x2-xoffset-w ) -1 , \
           ( br->y2-xoffset ) , D->gc.high_clr ) ;
      }
      jj = iy;
      uiDefaultGuiFontSize ( D ) ;
      bxln = lngth+xgap;
      if ( y->imgs == NULL ) uiMakeCImages ( y , bxln ) ;
      if ( y->nitems > 1 ) {
#if 0
          if ( y->itemhi ) {
              ixp = br->x1+xoffset;
              yi = br->hitem-pos;
              iyp = br->y1+yoffset+yi*br->width;
              _dvrect_fill ( WC ( D ) , ixp+2 , iyp+2 , br->x2-xoffset-w-2 , \
                   iyp+br->width-2 , D->gc.high_clr ) ;
          }
#else
          if ( y->itemhi ) {
              xi = ( br->hitem%y->nx ) ;
              yi = ( br->hitem-pos ) /y->nx;
              ixp = br->x1+xoffset+xi*bxln;
              iyp = br->y1+yoffset+yi*br->width;
              _dvrect_fill ( WC ( D ) , ixp+2 , iyp+2 , ixp+bxln-2 , \
                   iyp+br->width-2 , D->gc.ItemHighColor ) ;
          }
#endif
      }
      kk = 0;
      ny = y->size;
      for ( yi = 0; yi < ny; yi++ ) for ( xi = 0; xi < nx; xi++ ) {
          if ( kk+pos >= y->nitems ) continue;
          xpm = NULL;
          if ( menu != NULL ) {
              xpm = list [ kk+pos ]->img;
              swv = list [ kk+pos ]->sw;
          }
          else {if ( kk+pos == br->df-1 ) swv = 1; else swv = 0; }
          ixp = br->x1+xoffset+xi* ( bxln ) ;
          iyp = br->y1+yoffset+yi*br->width+y->ygap;
#if 0   // no image for check box
          if ( xpm != NULL ) {
              img = ( GMIMG * ) xpm;
              if ( y->sw [ kk+pos ] == 1 ) { img->bkgrclr = 1; }
              else img->bkgrclr = -1;
              kgImage ( D , xpm , ixp+2 , iyp+4 , y->width+y->xgap-4 , \
                   y->width , 0.0 , 1.0 ) ;
          }
#endif
#if 0
          if ( ( menu != NULL ) ) uiCheckString ( D , \
               menu [ kk ]->name , ( int ) ixp , \
           ( int ) iyp , bxln , th , D->gc.Font , D->gc.menu_char , \
                D->gc.FontSize , swv ) ;
#endif
          if ( menu != NULL ) {
              kgImage ( D , y->imgs [ kk+pos ] , ( int ) ( ixp+3.0*D->gc.FontSize ) , \
               ( int ) iyp , ( bxln -4*D->gc.FontSize ) , th , 0.0 , 1.0 ) ;
              if ( swv == 0 ) kgImage ( D , y->nimg , ( int ) ixp , \
               ( int ) iyp , 4*D->gc.FontSize , th , 0.0 , 1.0 ) ;
              if ( swv == 1 ) kgImage ( D , y->himg , ( int ) ixp , \
               ( int ) iyp , 4*D->gc.FontSize , th , 0.0 , 1.0 ) ;
          }
          kk++;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _uiPutXmenu ( DIX *y ) {
      BRW_STR *br;
      ThumbNail **menu , **list;
      unsigned int cur_c , temp , ch , cr , esc;
      int j , pos;
      int k , kk , knew , jj , ln , bxln , bxwd , ln1;
      int n , iy , iyp , ixp , ixmid , xgap , lngth;
      int scroll = 1 , swv , nx , ny , yi , xi , \
           scrsize , xoffset , yoffset , th , w;
      int xoff = 1;
      int dx , dy;
      int EVGAY;
      DIALOG *D;
      void *xpm = NULL;
      GMIMG *img;
      void *Bimg;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      Bimg = y->Bimg;
      EVGAY = D->evgay;
      if ( br->D == NULL ) {
          printf ( "Error: In uiPutXMenu : Parent=NULL\n" ) ;
          exit ( 0 ) ;
      }
      list = ( ThumbNail ** ) y->list;
      k = 0 ; while ( list [ k ] != NULL ) {if ( list [ k ]->sw != 0 ) list [ k ]->sw = 1;
          
      k++; }
      scroll = br->scroll;
      w = y->w*scroll;
      th = y->width+y->ygap;
      yoffset = br->offset;
      xoffset = y->offset;
      nx = y->nx;
      xgap = y->xgap;
      lngth = y->lngth;
      if ( nx == 1 ) {
          ln = ( y->x2 -y->x1-2*y->offset-w ) ;
          ln1 = ln*0.95;
          if ( lngth > ln1 ) lngth = ln1;
          xgap = ( ln-lngth ) ;
      }
      scrsize = y->size*y->nx;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      ln = br->y2-br->y1-2*br->offset; ;
      n = y->nitems;
      pos = br->pos;
      if ( pos < 0 ) { pos = 0; }
      if ( br->hitem < 0 ) br->hitem = 0;
      if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
      while ( br->hitem > ( pos+scrsize ) ) pos+= y->nx;
      if ( pos >= y->nitems ) pos = y->nitems-1;
      br->pos = pos;
      pos = ( pos/y->nx ) *y->nx;
      menu = NULL;
      if ( list != NULL ) menu = list+pos;
#ifndef D_RESTORE
      if ( y->bkgr == 1 ) {
          _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
           ( offset ) , br->y2- ( offset ) , gc.twin_fill ) ;
      }
      else {
          _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
           ( offset ) , br->y2- ( offset ) , gc.fill_clr ) ;
      }
#else
      dx = ( br->x2-xoffset-w ) - ( br->x1+xoffset ) ;
      dy = ( br->y2-xoffset ) - ( br->y1+xoffset ) +1;
      kgRestoreImagePart ( D , y->Bimg , ( br->x1+xoffset ) , \
       ( br->y1+xoffset ) , xoffset , xoffset , dx , dy ) ;
#endif
      if ( y->bordr == 1 ) {
          _dv_draw_bound ( D , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
               ( br->x2-xoffset-w ) -1 , \
           ( br->y2-xoffset ) , D->gc.twin_bodr ) ;
      }
      jj = iy;
      uiDefaultGuiFontSize ( D ) ;
#if 1
      bxln = lngth+xgap;
      if ( y->nitems > 1 ) {
          bxln = lngth+xgap;
          if ( y->itemhi ) {
              xi = ( br->hitem%y->nx ) ;
              yi = ( br->hitem-pos ) /y->nx;
              ixp = br->x1+xoffset+xi*bxln;
              iyp = br->y1+yoffset+yi*br->width;
              _dvrect_fill ( WC ( D ) , ixp+2 , iyp+2 , ixp+bxln-2 , \
                   iyp+br->width-2 , D->gc.ItemHighColor ) ;
          }
      }
#endif
      kk = 0;
      ny = y->size;
//      if(xpm== NULL) xoff=0;
      xoff = 0;
      for ( yi = 0; yi < ny; yi++ ) for ( xi = 0; xi < nx; xi++ ) {
          if ( kk+pos >= y->nitems ) continue;
          xpm = NULL;
          if ( menu != NULL ) {
              xpm = list [ kk+pos ]->img;
              swv = list [ kk+pos ]->sw;
          }
          else {if ( kk+pos == br->df-1 ) swv = 1; else swv = 0; }
          ixp = br->x1+xoffset+xi* ( lngth+xgap ) ;
          iyp = br->y1+yoffset+yi*br->width+y->ygap;
          bxln = lngth+xgap/2-4;
          if ( xpm != NULL ) {
              img = ( GMIMG * ) xpm;
              kgImage ( D , xpm , ixp+2 , iyp , y->width , y->width , 0.0 , 1.0 ) ;
              ixp = ( ixp+2+y->width ) ;
              bxln = lngth+xgap-4- ( y->width ) ;
              if ( bxln < 2 ) bxln = 2;
          }
          else ixp += ( xgap/2+2 ) ;
          if ( y->imgs == NULL ) {
              if ( ( ( y->type ) %10 ) <= 1 ) uiMakeXImages ( y , bxln-y->width ) ;
              else uiMakeXImages ( y , bxln ) ;
          }
          if ( ( menu != NULL ) && ( menu [ kk ]->name != NULL ) ) {
#if 0
              if ( y->type <= 1 ) uiMenuString ( D , menu [ kk ]->name , ( int ) ixp , \
               ( int ) iyp , bxln , y->width , D->gc.Font , D->gc.menu_char , \
                    D->gc.FontSize , swv ) ;
              else uiString ( D , menu [ kk ]->name , ( int ) ixp , ( int ) iyp , bxln , \
                   y->width , D->gc.Font , D->gc.menu_char , D->gc.FontSize , -1 , -1 ) ;
#else
              if ( ( ( y->type ) %10 ) <= 1 ) {
                  kgImage ( D , y->imgs [ kk+pos ] , ( int ) ixp , ( int ) iyp , \
                       bxln-y->width , y->width , 0.0 , 1.0 ) ;
                  if ( swv == 0 ) kgImage ( D , y->nimg , ( int ) ixp+bxln-y->width , \
                   ( int ) iyp , y->width , y->width , 0.0 , 1.0 ) ;
                  if ( swv == 1 ) kgImage ( D , y->himg , ( int ) ixp+bxln-y->width , \
                   ( int ) iyp , y->width , y->width , 0.0 , 1.0 ) ;
              }
              else {
                  kgImage ( D , y->imgs [ kk+pos ] , ( int ) ixp , \
                   ( int ) iyp , bxln , y->width , 0.0 , 1.0 ) ;
              }
#endif
          }
          kk++;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _uiPutEmenu ( DIE *y ) {
      BRW_STR *br;
      char **menu , **list;
      unsigned int cur_c , temp , ch , cr , esc;
      int j , pos;
      int k , kk , knew , jj , ln , bxln , bxwd;
      int n , iy , iyp , ixp , ixmid;
      int scroll = 1 , swv , nx , ny , yi , xi , \
           scrsize , xoffset , yoffset , th , w;
      int offset = 2;
      int xoff = 1;
      int dx , dy;
      int EVGAY;
      DIALOG *D;
      void *xpm = NULL;
      GMIMG *img;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      EVGAY = D->evgay;
      if ( br->D == NULL ) {
          printf ( "Error: In uiPutYMenu : Parent=NULL\n" ) ;
          exit ( 0 ) ;
      }
      list = ( char ** ) y->menu;
      scroll = br->scroll;
      w = y->w*scroll;
      th = y->width;
      yoffset = br->offset;
      xoffset = y->offset;
      nx = 1;
      scrsize = y->size*nx;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      ln = br->y2-br->y1-2*br->offset; ;
      n = y->nitems;
      pos = br->pos;
      if ( pos < 0 ) { pos = 0; }
      if ( br->hitem < 0 ) br->hitem = 0;
      if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
      while ( br->hitem > ( pos+scrsize ) ) pos+= nx;
      if ( pos >= y->nitems ) pos = y->nitems-1;
      br->pos = pos;
      menu = NULL;
      if ( list != NULL ) menu = list+pos;
#ifndef D_RESTORE
      _dvrect_fill ( WC ( D ) , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
       ( br->x2-xoffset-w ) -1 , ( br->y2-xoffset ) , D->gc.fill_clr ) ;
#else
      dx = ( br->x2-xoffset-w ) - ( br->x1+xoffset ) ;
      dy = ( br->y2-xoffset ) - ( br->y1+xoffset ) +1;
      kgRestoreImagePart ( D , y->Bimg , ( br->x1+xoffset ) , \
       ( br->y1+xoffset ) , xoffset , xoffset , dx , dy ) ;
#endif
      if ( y->bordr == 1 ) {
          _dv_draw_bound ( D , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
               ( br->x2-xoffset-w ) -1 , \
           ( br->y2-xoffset ) , D->gc.high_clr ) ;
      }
      jj = iy;
      uiDefaultGuiFontSize ( D ) ;
#if 1
      if ( y->nitems > 1 ) {
          if ( y->itemhi ) {
              ixp = br->x1+xoffset;
              yi = br->hitem-pos;
              iyp = br->y1+yoffset+yi*br->width;
              _dvrect_fill ( WC ( D ) , ixp+2 , iyp+2 , br->x2-xoffset-w-2 , \
                   iyp+br->width-2 , D->gc.ItemHighColor ) ;
          }
      }
#endif
      kk = 0;
      ny = y->size;
      nx = 1;
      if ( xpm == NULL ) xoff = 0;
      bxln = y->x2 - y->x1-2*xoffset-w-xoff*y->width;
      if ( y->imgs == NULL ) {
          uiMakeEbrowserImages ( y , bxln ) ;
      }
      for ( yi = 0; yi < ny; yi++ ) for ( xi = 0; xi < nx; xi++ ) {
          if ( kk+pos >= y->nitems ) continue;
          if ( kk+pos == br->df-1 ) swv = 1; else swv = 0;
          ixp = br->x1+xoffset+xi* ( y->width ) ;
          iyp = br->y1+yoffset+yi*br->width;
          if ( ( list != NULL ) && ( list [ kk+pos ] != NULL ) ) {
//            uiString(D,list[kk+pos], (int)ixp+xoff*y->width,(int)iyp,bxln,y->width,D->gc.Font,
//                    D->gc.menu_char,D->gc.FontSize,-1,-1);
              kgImage ( D , y->imgs [ kk+pos ] , ( int ) ixp+xoff*y->width , \
               ( int ) iyp , bxln , y->width , 0.0 , 1.0 ) ;
          }
          kk++;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _uiPutMmenu ( DIS *y ) {
      BRW_STR *br;
      char **menu , **list;
      unsigned int cur_c , temp , ch , cr , esc;
      int j , pos;
      int offset = 2;
      int k , kk , knew , jj , ln , bxln , bxwd;
      int n , iy , iyp , ixp , ixmid;
      int scroll = 1 , swv , nx , ny , yi , xi , \
           scrsize , xoffset , yoffset , th , w;
      int xoff = 1;
      int EVGAY;
      DIALOG *D;
      void *xpm = NULL;
      GMIMG *img;
      br = y->bwsr;
      D = ( DIALOG * ) ( y->D ) ;
      EVGAY = D->evgay;
      list = ( char ** ) br->menu;
      scroll = br->scroll;
      w = y->w*scroll;
      th = y->width;
      yoffset = br->offset;
      xoffset = y->offset;
      nx = 1;
      scrsize = y->size*nx;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      ln = br->y2-br->y1-2*br->offset; ;
      n = y->nitems;
      pos = br->pos;
      if ( pos < 0 ) { pos = 0; }
      if ( br->hitem < 0 ) br->hitem = 0;
      if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
      while ( br->hitem > ( pos+scrsize ) ) pos+= nx;
      if ( pos >= y->nitems ) pos = y->nitems-1;
      br->pos = pos;
      menu = NULL;
#if 0
      if ( list != NULL ) menu = list+pos;
#else
      menu = list;
      if ( y->nitems > y->size ) menu += ( y->nitems - y->size ) ;
#endif
#if 0
      _dvrect_fill ( WC ( D ) , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
       ( br->x2-xoffset-w ) -1 , ( br->y2-xoffset ) , D->gc.info_fill ) ;
      if ( y->bordr == 1 ) {
          _dv_draw_bound ( D , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
               ( br->x2-xoffset-w ) -1 , \
           ( br->y2-xoffset ) , D->gc.high_clr ) ;
      }
#else
      _dvrect_fill ( WC ( D ) , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
       ( br->x2-xoffset-w ) -1 , ( br->y2-xoffset ) , br->MS.color1 ) ;
#if 0
      if ( y->bordr == 1 ) {
//      _dv_draw_bound(D,(br->x1+xoffset),(br->y1+xoffset),(br->x2-xoffset-w)-1, (br->y2-xoffset),br->MS.color2);
          _dvrect_fill ( WC ( D ) , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
           ( br->x2-xoffset-w ) -1 , ( br->y2-xoffset ) , br->MS.color2 ) ;
          _dvrect_fill ( WC ( D ) , ( br->x1+xoffset+offset ) , \
               ( br->y1+xoffset+offset ) , \
           ( br->x2-xoffset-w-1-offset ) , ( br->y2-xoffset-offset ) , br->MS.color1 ) ;
      }
#endif
#endif
      jj = iy;
      uiDefaultGuiFontSize ( D ) ;
      kk = 0;
      ny = y->size;
      nx = 1;
      if ( xpm == NULL ) xoff = 0;
      bxln = y->x2 - y->x1-2*xoffset-w-xoff*y->width;
      for ( yi = 0; yi < ny; yi++ ) for ( xi = 0; xi < nx; xi++ ) {
          if ( kk+pos >= y->nitems ) continue;
          if ( kk+pos == br->df-1 ) swv = 1; else swv = 0;
          ixp = br->x1+xoffset+xi* ( y->width ) +br->width*0.5;
          iyp = br->y1+yoffset+yi*br->width+br->width*0.5;
          if ( ( list != NULL ) && ( list [ kk+pos ] != NULL ) ) {
#if 1
              uiString ( D , list [ kk+pos ] , ( int ) ixp+xoff*y->width , ( int ) iyp , \
                   bxln , y->width , D->gc.MsgFont , br->MS.char_clr , \
                   D->gc.FontSize , -1 , -1 ) ;
#else
	    //TCB NEW
	    //
              uiMsgString ( D , list [ kk+pos ] , ( int ) ixp+xoff*y->width , ( int ) iyp , \
                   br->MS.char_clr , D->gc.MsgFont , D->gc.FontSize ) ;
#endif
          }
          kk++;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _uiPutWmenu ( DIW *y ) {
      BRW_STR *br;
      char **menu , **list;
      unsigned int cur_c , temp , ch , cr , esc;
      int j , pos;
      int k , kk , knew , jj , ln , bxln , bxwd;
      int n , iy , iyp , ixp , ixmid;
      int scroll = 1 , swv , nx , ny , yi , xi , \
           scrsize , xoffset , yoffset , th , w;
      int xoff = 1;
      int EVGAY;
      DIALOG *D;
      void *xpm = NULL;
      GMIMG *img;
      br = y->bwsr;
      if ( y->imgs == NULL ) uiMakeBrowserImages ( y ) ;
      D = ( DIALOG * ) ( br->D ) ;
      EVGAY = D->evgay;
      if ( br->D == NULL ) {
          printf ( "Error: In uiPutYMenu : Parent=NULL\n" ) ;
          exit ( 0 ) ;
      }
      list = ( char ** ) y->menu;
      scroll = br->scroll;
      w = y->w*scroll;
      th = y->width;
      yoffset = br->offset;
      xoffset = y->offset;
      nx = 1;
      scrsize = y->size*nx;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      ln = br->y2-br->y1-2*br->offset; ;
      n = y->nitems;
      pos = br->pos;
      if ( pos < 0 ) { pos = 0; }
      if ( br->hitem < 0 ) br->hitem = 0;
      if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
      while ( br->hitem > ( pos+scrsize ) ) pos+= nx;
      if ( pos >= y->nitems ) pos = y->nitems-1;
      br->pos = pos;
      menu = NULL;
      if ( list != NULL ) menu = list+pos;
      _dvrect_fill ( WC ( D ) , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
       ( br->x2-xoffset-w ) -1 , ( br->y2-xoffset ) , D->gc.fill_clr ) ;
      if ( y->bordr == 1 ) {
          _dv_draw_bound ( D , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
               ( br->x2-xoffset-w ) -1 , \
           ( br->y2-xoffset ) , D->gc.high_clr ) ;
      }
      jj = iy;
      uiDefaultGuiFontSize ( D ) ;
#if 1
      if ( y->nitems > 1 ) {
          if ( y->itemhi ) {
              ixp = br->x1+xoffset;
              yi = br->hitem-pos;
              iyp = br->y1+yoffset+yi*br->width;
              _dvrect_fill ( WC ( D ) , ixp+2 , iyp+2 , br->x2-xoffset-w-2 , \
                   iyp+br->width-2 , D->gc.ItemHighColor ) ;
          }
      }
#endif
      kk = 0;
      ny = y->size;
      nx = 1;
      if ( xpm == NULL ) xoff = 0;
//      bxln = y->x2 - y->x1-2*xoffset-w-xoff*y->width;
      bxln = br->x2 - br->x1-2*xoffset-w-xoff*y->width;
#if 0
      xoffset = bxln*0.05;
      bxln -= ( 2*xoffset ) ;
#endif
      for ( yi = 0; yi < ny; yi++ ) for ( xi = 0; xi < nx; xi++ ) {
          if ( kk+pos >= y->nitems ) continue;
          if ( kk+pos == br->df-1 ) swv = 1; else swv = 0;
          ixp = br->x1+xoffset+xi* ( y->width ) ;
          iyp = br->y1+yoffset+yi*br->width;
          if ( ( list != NULL ) && ( list [ kk+pos ] != NULL ) ) {
//            uiString(D,list[kk+pos], (int)ixp+xoff*y->width,(int)iyp,bxln,y->width,D->gc.Font,
//                    D->gc.menu_char,D->gc.FontSize,-1,-1);
              kgImage ( D , y->imgs [ kk+pos ] , ( int ) ixp+xoff*y->width , \
               ( int ) iyp , bxln , y->width , 0.0 , 1.0 ) ;
          }
          kk++;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _uiPutRadioButton ( DIRA *y ) {
      BRW_STR *br;
      ThumbNail **menu , **list;
      unsigned int cur_c , temp , ch , cr , esc;
      int j , pos;
      int k , kk , knew , jj , ln , ln1;
      int n , iy , iyp , ixp , ixmid;
      int scroll = 1 , swv , nx , ny , yi , xi , scrsize , \
           xoffset , yoffset , th , w , xgap , lngth , bxln;
      int EVGAY;
      DIALOG *D;
      void *xpm = NULL;
      GMIMG *img;
      int xoff , yoff , dx , dy;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      EVGAY = D->evgay;
      if ( br->D == NULL ) {
          printf ( "Error: In uiPutYMenu : Parent=NULL\n" ) ;
          exit ( 0 ) ;
      }
      br->BxSize = 2*D->gc.FontSize;
      list = ( ThumbNail ** ) y->list;
      k = 0; while ( list [ k ] != NULL ) {if ( list [ k ]->sw != 0 ) list [ k ]->sw = 1;
          
      k++; }
      scroll = br->scroll;
      w = y->w*scroll;
      th = y->width+y->ygap;
      yoffset = br->offset;
      xoffset = y->offset;
//      if(xoffset==0 ) xoffset=2;
      nx = y->nx;
      xgap = y->xgap;
      lngth = y->lngth;
      if ( nx == 1 ) {
          ln = ( y->x2 -y->x1-2*y->offset-w ) ;
          ln1 = ln*0.95;
          if ( lngth > ln1 ) lngth = ln1;
          xgap = ( ln-lngth ) ;
      }
      scrsize = y->size*y->nx;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      ln = br->y2-br->y1-2*br->offset;
      n = y->nitems;
      pos = br->pos;
      if ( pos < 0 ) { pos = 0; }
      if ( br->hitem < 0 ) br->hitem = 0;
      if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
      while ( br->hitem > ( pos+scrsize ) ) pos+= y->nx;
      if ( pos >= y->nitems ) pos = y->nitems-1;
      br->pos = pos;
      pos = ( pos/y->nx ) *y->nx;
      menu = NULL;
      if ( list != NULL ) menu = list+pos;
#ifndef D_RESTORE
//      _dvrect_fill(WC(D),(br->x1+xoffset),(br->y1+xoffset),(br->x2-xoffset-w)-1, (br->y2-xoffset),D->gc.fill_clr);
      if ( y->bkgr == 1 ) {
          _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
           ( offset ) , br->y2- ( offset ) , gc.txt_fill ) ;
      }
      else {
          _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
           ( offset ) , br->y2- ( offset ) , gc.fill_clr ) ;
      }
#else
      dx = ( br->x2-xoffset-w ) - ( br->x1+xoffset ) ;
      dy = ( br->y2-xoffset ) - ( br->y1+xoffset ) +1;
      kgRestoreImagePart ( D , y->Bimg , ( br->x1+xoffset ) , \
       ( br->y1+xoffset ) , xoffset , xoffset , dx , dy ) ;
#endif
      if ( y->bordr == 1 ) {
          _dv_draw_bound ( D , ( br->x1+xoffset ) , ( br->y1+xoffset ) , \
               ( br->x2-xoffset-w ) -1 , \
           ( br->y2-xoffset ) , D->gc.high_clr ) ;
      }
      jj = iy;
      bxln = lngth+xgap;
      if ( y->imgs == NULL ) uiMakeRImages ( y , bxln ) ;
      if ( y->nitems > 1 ) {
#if 0
          if ( y->itemhi ) {
              ixp = br->x1+xoffset;
              yi = br->hitem-pos;
              iyp = br->y1+yoffset+yi*br->width;
              _dvrect_fill ( WC ( D ) , ixp+2 , iyp+2 , br->x2-xoffset-w-2 , \
                   iyp+br->width-2 , D->gc.high_clr ) ;
          }
#else
          if ( y->itemhi ) {
              xi = ( br->hitem%y->nx ) ;
              yi = ( br->hitem-pos ) /y->nx;
              ixp = br->x1+xoffset+xi*bxln;
              iyp = br->y1+yoffset+yi*br->width;
              _dvrect_fill ( WC ( D ) , ixp+2 , iyp+2 , ixp+bxln-2 , \
                   iyp+br->width-2 , D->gc.ItemHighColor ) ;
          }
#endif
      }
      uiDefaultGuiFontSize ( D ) ;
      kk = 0;
      ny = y->size;
      for ( yi = 0; yi < ny; yi++ ) for ( xi = 0; xi < nx; xi++ ) {
          if ( kk+pos >= y->nitems ) continue;
          xpm = NULL;
          if ( list != NULL ) {
              xpm = list [ kk+pos ]->img;
              swv = list [ kk+pos ]->sw;
          }
          else {if ( kk+pos == br->df-1 ) swv = 1; else swv = 0; }
          ixp = br->x1+xoffset+xi* ( bxln ) ;
          iyp = br->y1+yoffset+yi*br->width+y->ygap;
#if 0   // no image for Radio button
          if ( xpm != NULL ) {
              img = ( GMIMG * ) xpm;
              if ( y->sw [ kk+pos ] == 1 ) { img->bkgrclr = 1; }
              else img->bkgrclr = -1;
              kgImage ( D , xpm , ixp+2 , iyp+4 , y->width+y->xgap-4 , \
                   y->width , 0.0 , 1.0 ) ;
          }
#endif
          if ( list != NULL ) {
              kgImage ( D , y->imgs [ kk+pos ] , ( int ) ( ixp+3.0*D->gc.FontSize ) , \
               ( int ) iyp , ( bxln -4*D->gc.FontSize ) , th , 0.0 , 1.0 ) ;
              if ( swv == 0 ) kgImage ( D , y->nimg , ( int ) ixp , \
               ( int ) iyp , 4*D->gc.FontSize , th , 0.0 , 1.0 ) ;
              if ( swv == 1 ) kgImage ( D , y->himg , ( int ) ixp , \
               ( int ) iyp , 4*D->gc.FontSize , th , 0.0 , 1.0 ) ;
          }
#if 0
          if ( ( list != NULL ) && ( menu [ kk ]->name != NULL ) ) uiRadioString \
           ( D , menu [ kk ]->name , ( int ) ixp , ( int ) iyp , bxln , th , \
                D->gc.Font , D->gc.menu_char , D->gc.FontSize , swv ) ;
#endif
          kk++;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _uiPutmsg ( BRW_STR *br ) {
      unsigned int cur_c , temp , ch , cr , esc;
      int j , pos;
      int k , kk , knew , jj;
      int n , iy , iyp , ixp , ixmid;
      int scroll = 1;
      int EVGAY;
      DIALOG *D;
      D = ( DIALOG * ) ( br->D ) ;
      EVGAY = D->evgay;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      n = 0;
      if ( br->menu != NULL ) while ( br->menu [ n ] != NULL ) n++;
      else n = 1;
      if ( br->pos < ( br->hitem-br->size ) ) br->pos = br->hitem-br->size;
      if ( br->pos >= br->hitem ) br->pos = br->hitem-1;
      if ( br->pos > ( n - br->size ) ) br->pos = n-br->size;
      if ( br->pos < 0 ) br->pos = 0;
      if ( br->pos >= n ) br->pos = n-1;
      pos = br->pos;
      ixmid = br->MS.ixx+br->MS.ixl/2;
      iy = br->MS.iyy+1;
//      _uirect_fill(WC(D),(br->MS.ixx-1),(EVGAY-br->MS.iyy),(br->MS.ixu+1),(EVGAY-br->MS.iyu),br->gc.info_fill);
      _uirect_fill ( WC ( D ) , ( br->MS.ixx ) , ( EVGAY-br->MS.iyy-1 ) , \
       ( br->MS.ixu ) , ( EVGAY-br->MS.iyu+1 ) , br->gc.msg_fill ) ;
      _ui_draw_bound ( D , br->MS.ixx , ( EVGAY-br->MS.iyy-1 ) , br->MS.ixu , \
       ( EVGAY-br->MS.iyu+1 ) , br->gc.msg_bodr ) ;
      k = 1;
      jj = iy + ( k ) *br->MS.thirty;
      jj = iy;
      uiSetGuiFixFontSize ( D , D->gc.InputFontSize ) ;
      kk = 0;
      for ( kk = 0; kk < br->size; kk++ ) {
          jj = iy + kk*br->MS.thirty+br->MS.thirty;
          ixp = br->MS.ixx+10;
          iyp = EVGAY-jj-7;
//        if(br->MS.menu[kk]== NULL) break;
//        write_string(br->MS.menu[kk],(int)ixp,(int)iyp,br->gc.info_char);
          if ( ( br->menu != NULL ) && ( ( kk+pos ) < n ) && \
           ( br->menu [ kk+pos ] != NULL ) ) {
              uiwrite_string ( D , br->menu [ kk+pos ] , ( int ) ixp , \
               ( int ) iyp , br->gc.msg_char ) ;
          }
          else uiwrite_string ( D , " " , ( int ) ixp , \
           ( int ) iyp , br->gc.msg_char ) ;
      }
      uiDefaultGuiFontSize ( D ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _ui_vert_scroll_bar ( BRW_STR *br ) {
      int xx , yy , h , w;
      DIALOG *D;
      D = ( DIALOG * ) ( br->D ) ;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      xx = br->MS.ixu+3;
      yy = br->MS.iyy-1;
      w = 24;
      h = br->MS.iyu- br->MS.iyy+4;
      w = 24;
      yy = D->evgay-yy;
      _uiup_dir ( D , xx+3 , yy-4 , w-5 , 0.1 , 5 ) ;
      _uidown_dir ( D , xx+3 , yy-h+w-1 , w-5 , 0.1 , 5 ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _ui_vert_scroll_vbar ( void *tmp ) {
      int xx , yy , h , w;
      DIV *v;
      DIALOG *D;
      v = ( DIV * ) tmp;
      D = ( DIALOG * ) ( v->D ) ;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      xx = v->tbx1+1;
      yy = v->tby1+1;
      w = v->w;
      _dvup_dir ( D , xx , yy , w , 0.2 , 4 ) ;
      yy = v->bby1;
      _dvdown_dir ( D , xx , yy , w , 0.2 , 4 ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _ui_vert_scroll_zbar ( void *tmp ) {
      int xx , yy , h , w;
      DIZ *v;
      DIALOG *D;
      v = ( DIZ * ) tmp;
      D = ( DIALOG * ) ( v->D ) ;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      xx = v->lbx1;
      yy = v->lby1;
      w = v->w;
      _dvright_dir ( D , xx , yy , w , 0.2 , 4 ) ;
      xx = v->rbx1;
      yy = v->rby1;
      _dvleft_dir ( D , xx , yy , w , 0.2 , 4 ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
  }
//TCBTCB
  void _ui_vert_scroll_ybar ( void *tmp ) {
      int xx , yy , h , w;
      BRW_STR *br;
      DIALOG *D;
      DIY *y;
      y = ( DIY * ) tmp;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      xx = br->tbx1;
      yy = br->tby1;
      w = br->w;
      _dvup_dir ( D , xx , yy , w , 0.2 , 4 ) ;
      yy = br->bby1;
      _dvdown_dir ( D , xx , yy , w , 0.2 , 4 ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _ui_vert_scroll_ebar ( void *tmp ) {
      int xx , yy , h , w;
      BRW_STR *br;
      DIALOG *D;
      DIE *y;
      y = ( DIE * ) tmp;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      xx = br->tbx1;
      yy = br->tby1;
      w = br->w;
      _dvup_dir ( D , xx , yy , w , 0.2 , 4 ) ;
      yy = br->bby1;
      _dvdown_dir ( D , xx , yy , w , 0.2 , 4 ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _ui_vert_scroll_mbar ( void *tmp ) {
      int xx , yy , h , w;
      BRW_STR *br;
      DIALOG *D;
      DIS *y;
      y = ( DIS * ) tmp;
      br = y->bwsr;
      D = ( DIALOG * ) ( y->D ) ;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      xx = br->tbx1+2;
      yy = br->tby1;
      w = br->w;
      _dvup_dir ( D , xx , yy , w , 0.2 , 4 ) ;
      yy = br->bby1;
      _dvdown_dir ( D , xx , yy , w , 0.2 , 4 ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _ui_vert_scroll_wbar ( void *tmp ) {
      int xx , yy , h , w;
      BRW_STR *br;
      DIALOG *D;
      DIW *y;
      y = ( DIW * ) tmp;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      xx = br->tbx1;
      yy = br->tby1;
      w = br->w;
      _dvup_dir ( D , xx , yy , w , 0.2 , 4 ) ;
      yy = br->bby1;
      _dvdown_dir ( D , xx , yy , w , 0.2 , 4 ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _uimove_vert_pointer ( DIALOG *D , int x , \
       int y , int w , int h , int sy , int sh ) \
  {
      char tmp;
      int x1 , y1 , x2 , y2;
      tmp = WC ( D )->c_color;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      y = D->evgay-y;
//TCB
//   x1=x+4;y1=y-w-2;x2=x+w-4;y2=y-h+w+4;
      x1 = x+4; y1 = y-w+1; x2 = x+w-4; y2 = y-h+w;
//   _uirect_fill(D,x1,y1,x2,y2,D->gc.fill_clr);
//   _uirect_fill(WC(D),x1,y1,x2,y2+1,D->gc.fill_clr);
//TCB
//   _uirect_fill(WC(D),x1,y1,x2,y2+1,D->gc.dim);
      _uirect_fill ( WC ( D ) , x1 , y1 , x2 , y2 , D->gc.dim ) ;
      sy = y1-1-sy;
      if ( sy > y1 ) sy = y1-1;
      if ( sy-sh < y2 ) sy = y2+sh+1;
      uiset_clr ( D , D->gc.vbright ) ;
      x1 += 2;
      x2 -= 1;
      _uirect_fill ( WC ( D ) , x1 , sy , x2 , sy-sh , D->gc.fill_clr ) ;
      uiset_clr ( D , D->gc.vbright ) ;
      _uihori_line ( WC ( D ) , x1 , x2 , sy ) ;
      _uivert_line ( WC ( D ) , sy , sy-sh , x1 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _uihori_line ( WC ( D ) , x1 , x2 , sy-sh ) ;
      _uivert_line ( WC ( D ) , sy , sy-sh , x2 ) ;
      uiset_clr ( D , tmp ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _uiMoveVertPointer ( BRW_STR *br ) {
      int xx , yy , j , item , sh , sy , h , w , scr_ln , pos;
      float fac;
      DIALOG *D;
      D = ( DIALOG * ) ( br->D ) ;
      item = br->df;
      xx = br->MS.ixx+br->MS.ixl+1;
      yy = br->MS.iyy-10;
      w = 24;
      h = br->MS.iyl+20;
      xx = br->MS.ixu+3;
      yy = br->MS.iyy-1;
      w = 24;
      h = br->MS.iyu - br->MS.iyy+4;
//TCB
//   scr_ln=h-2*w-8;
      scr_ln = h-2*w;
      fac = scr_ln/ ( float ) br->MS.nitems;
      sh = fac*br->size+0.5;
      if ( sh < MINV ) sh = MINV;
      fac = 0.0;
      if ( br->MS.nitems-br->size != 0 ) fac = ( scr_ln-sh ) / \
       ( float ) ( br->MS.nitems-br->size ) ;
      else return;
      br->MS.fac = fac;
//   pos=br->df-br->size;
      if ( br->pos < ( br->hitem-br->size ) ) br->pos = br->hitem-br->size;
      if ( br->pos >= br->hitem ) br->pos = br->hitem-1;
      pos = br->pos;
      if ( pos < 0 ) { pos = 0; br->MS.item = br->df; }
      j = pos;
#if 0
      if ( ( j+1 ) > item ) j = item-1;
      if ( item > j+br->size ) j = item-1;
      if ( j+br->size > br->MS.nitems ) j = br->MS.nitems-br->size;
#endif
      sy = j*fac+0.5;
      _uimove_vert_pointer ( D , xx , yy , w , h , sy , sh ) ;
      return;
  }
  void _uiNewMoveVertPointer ( BRW_STR *br ) {
      int xx , yy , j , item , sh , sy , h , w , scr_ln , pos;
      float fac;
      DIALOG *D;
      D = ( DIALOG * ) ( br->D ) ;
      item = br->df;
      xx = br->MS.ixx+br->MS.ixl+1;
      yy = br->MS.iyy-10;
      w = 24;
      h = br->MS.iyl+20;
      xx = br->MS.ixu+3;
      yy = br->MS.iyy-1;
      w = 24;
      h = br->MS.iyu - br->MS.iyy+4;
      scr_ln = h-2*w;
      fac = scr_ln/ ( float ) br->MS.nitems;
      sh = fac*br->size+0.5;
      if ( sh < MINV ) sh = MINV;
      fac = 0.0;
      if ( br->MS.nitems-br->size != 0 ) fac = ( scr_ln-sh ) / \
       ( float ) ( br->MS.nitems-br->size ) ;
      else return;
      br->MS.fac = fac;
      if ( br->pos < ( br->hitem-br->size ) ) br->pos = br->hitem-br->size;
      if ( br->pos >= br->hitem ) br->pos = br->hitem-1;
      pos = br->pos;
      if ( pos < 0 ) { pos = 0; br->MS.item = br->df; }
      j = pos;
      sy = j*fac+0.5;
      _uimove_vert_pointer ( D , xx , yy , w , h , sy , sh ) ;
      return;
  }
//TCBTCB
  void _dvmove_vert_pointer ( DIALOG *D , int x , \
       int y , int w , int h , int sy , int sh ) \
  {
      char tmp;
      int x1 , y1 , x2 , y2 , w2;
      w2 = w/2;
      tmp = WC ( D )->c_color;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
//TCB NEW
      x1 = x+1; y1 = y; x2 = x1+w; y2 = y+h;
      _dvrect_fill ( WC ( D ) , x1 , y1 , x2 , y2 , D->gc.scroll_dim ) ;
      sy = y1+sy;
      if ( sy > y2-sh ) sy = y2-sh;
      if ( sy+sh > y2 ) sy = y2- sh;
      uiset_clr ( D , D->gc.scroll_vbright ) ;
      _dvrect_fill ( WC ( D ) , x1 , sy , x2 , sy+sh , D->gc.scroll_fill ) ;
      uiset_clr ( D , D->gc.scroll_vbright ) ;
      _dv_h_line ( WC ( D ) , x1 , x2 , sy ) ;
      _dv_v_line ( WC ( D ) , sy , sy+sh , x1 ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _dv_h_line ( WC ( D ) , x1 , x2 , sy+sh ) ;
      _dv_v_line ( WC ( D ) , sy , sy+sh , x2 ) ;
      uiset_clr ( D , tmp ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _dvmove_hori_pointer ( DIALOG *D , int x , \
       int y , int w , int h , int sy , int sh ) \
  {
      char tmp;
      int x1 , y1 , x2 , y2;
      tmp = WC ( D )->c_color;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
//TCB
      x1 = x; y1 = y; x2 = x+w; y2 = y+h;
      _dvrect_fill ( WC ( D ) , x1 , y1 , x2 , y2 , D->gc.scroll_dim ) ;
      sy = x1+sy;
      if ( sy > x2 ) sy = x2-sh;
      if ( sy+sh > x2 ) sy = x2- sh;
      uiset_clr ( D , D->gc.scroll_vbright ) ;
      _dvrect_fill ( WC ( D ) , sy , y1 , sy+sh , y2 , D->gc.scroll_fill ) ;
      uiset_clr ( D , D->gc.scroll_vbright ) ;
      _dv_h_line ( WC ( D ) , sy , sy+sh , y1 ) ;
      _dv_v_line ( WC ( D ) , y1 , y2 , sy ) ;
      uiset_clr ( D , D->gc.v_dim ) ;
      _dv_h_line ( WC ( D ) , sy , sy+sh , y2 ) ;
      _dv_v_line ( WC ( D ) , y1 , y2 , sy+sh ) ;
      uiset_clr ( D , tmp ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
  }
  void _uiMoveYVertPointer ( void *tmp ) {
      DIY *y;
      int xx , yy , j , item , sh , sy , h , w , scr_ln , \
           pos , nx , hitem , offset , himax;
      float fac;
      DIALOG *D;
      BRW_STR *br;
      y = ( DIY * ) tmp;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      nx = y->nx;
      item = br->df;
      offset = br->w*0.4;
      xx = br->rsx1+offset/2;
      yy = br->rsy1;
      w = br->w-offset;
      h = br->rsy2 - br->rsy1;
      scr_ln = h;
      fac = scr_ln/ ( float ) y->ny;
      sh = fac*y->size+0.5;
      if ( sh < MINV ) sh = MINV;
      fac = 0.0;
      if ( ( y->ny-y->size ) > 0 ) fac = ( scr_ln-sh ) / \
       ( float ) ( y->ny-y->size ) ;
      else return;
      br->MS.fac = fac;
      br->fac = fac;
      pos = ( br->pos ) / ( nx ) ;
      if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
      hitem = ( br->hitem ) / ( nx ) ;
      if ( pos > hitem ) pos = hitem;
      while ( ( hitem-pos ) >= y->size ) pos++;
      br->pos = pos*y->nx;
      j = pos;
      sy = j*fac+0.5;
      br->sy = sy;
      br->shy = sh;
      _dvmove_vert_pointer ( D , xx , yy , w , h , sy , sh ) ;
      return;
  }
  void _uiMoveVVertPointer ( void *tmp ) {
      DIV *y;
      int xx , yy , j , item , sh , sy , h , w , scr_ln , \
           pos , nx , hitem , offset , himax;
      float fac;
      DIALOG *D;
      y = ( DIV * ) tmp;
      D = ( DIALOG * ) ( y->D ) ;
      xx = y->rsx1;
      yy = y->rsy1;
      w = y->rsx2 - y->rsx1;
      h = y->rsy2 - y->rsy1;
      scr_ln = h;
      fac = scr_ln*y->ds/100.0;
      sh = fac+0.5;
      if ( sh < MINV ) sh = MINV;
      scr_ln = h - sh;
      sy = scr_ln* ( ( y->df ) ) /100;
      if ( ( sy+sh ) > h ) sy = h-sh;
      _dvmove_vert_pointer ( D , xx , yy , w , h , sy , sh ) ;
      return;
  }
  void _uiMoveZVertPointer ( void *tmp ) {
      DIZ *y;
      int xx , yy , j , item , sh , sy , h , w , scr_ln , \
           pos , nx , hitem , offset , himax;
      float fac;
      DIALOG *D;
      y = ( DIZ * ) tmp;
      D = ( DIALOG * ) ( y->D ) ;
      xx = y->rsx1;
      yy = y->rsy1;
      h = y->rsy2-y->rsy1;
      w = y->rsx2 - y->rsx1;
      scr_ln = w;
      fac = scr_ln*y->ds/100.0;
      sh = fac+0.5;
      if ( sh < MINV ) sh = MINV;
      sy = scr_ln* ( ( y->df ) ) /100;
      if ( ( sy+sh ) > w ) sy = w-sh;
      _dvmove_hori_pointer ( D , xx , yy , w , h , sy , sh ) ;
      return;
  }
  void _uiMoveEVertPointer ( void *tmp ) {
      DIE *y;
      int xx , yy , j , item , sh , sy , h , w , scr_ln , \
           pos , nx , hitem , offset , himax;
      float fac;
      DIALOG *D;
      BRW_STR *br;
      y = ( DIE * ) tmp;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      nx = 1;
      item = br->df;
      offset = br->w*0.4;
      xx = br->rsx1+offset/2;
      yy = br->rsy1;
      w = br->w-offset;
      h = br->rsy2 - br->rsy1;
      scr_ln = h;
      fac = scr_ln/ ( float ) y->nitems;
      sh = fac*y->size+0.5;
      if ( sh < MINV ) sh = MINV;
      fac = 0.0;
      if ( ( y->nitems-y->size ) > 0 ) fac = ( scr_ln-sh ) / \
       ( float ) ( y->nitems-y->size ) ;
      else return;
      br->MS.fac = fac;
      br->fac = fac;
      pos = ( br->pos ) / ( nx ) ;
      if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
      hitem = ( br->hitem ) / ( nx ) ;
      if ( pos > hitem ) pos = hitem;
      while ( ( hitem-pos ) >= y->size ) pos++;
      br->pos = pos*nx;
      j = pos;
      sy = j*fac+0.5;
      br->sy = sy;
      br->shy = sh;
      _dvmove_vert_pointer ( D , xx , yy , w , h , sy , sh ) ;
      return;
  }
  void _uiMoveMVertPointer ( void *tmp ) {
      DIS *y;
      int xx , yy , j , item , sh , sy , h , w , scr_ln , \
           pos , nx , hitem , offset , himax;
      float fac;
      DIALOG *D;
      BRW_STR *br;
      y = ( DIS * ) tmp;
      br = y->bwsr;
      D = ( DIALOG * ) ( y->D ) ;
      nx = 1;
      item = br->df;
      offset = br->w*0.4;
      xx = br->rsx1+offset/2;
      yy = br->rsy1;
      w = br->w-offset;
      h = br->rsy2 - br->rsy1;
      scr_ln = h;
      fac = scr_ln/ ( float ) y->nitems;
      sh = fac*y->size+0.5;
      if ( sh < MINV ) sh = MINV;
      fac = 0.0;
      if ( ( y->nitems-y->size ) > 0 ) fac = ( scr_ln-sh ) / \
       ( float ) ( y->nitems-y->size ) ;
      else return;
      br->MS.fac = fac;
      br->fac = fac;
      pos = ( br->pos ) / ( nx ) ;
      if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
      hitem = ( br->hitem ) / ( nx ) ;
      if ( pos > hitem ) pos = hitem;
      while ( ( hitem-pos ) >= y->size ) pos++;
      br->pos = pos*nx;
      j = pos;
      sy = j*fac+0.5;
      br->sy = sy;
      br->shy = sh;
      _dvmove_vert_pointer ( D , xx , yy , w , h , sy , sh ) ;
      return;
  }
  void _uiMoveWVertPointer ( void *tmp ) {
      DIW *y;
      int xx , yy , j , item , sh , sy , h , w , scr_ln , \
           pos , nx , hitem , offset , himax;
      float fac;
      DIALOG *D;
      BRW_STR *br;
      y = ( DIW * ) tmp;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      nx = 1;
      item = br->df;
      offset = br->w*0.4;
      xx = br->rsx1+offset/2;
      yy = br->rsy1;
      w = br->w-offset;
      h = br->rsy2 - br->rsy1;
      scr_ln = h;
      fac = scr_ln/ ( float ) y->nitems;
      sh = fac*y->size+0.5;
      if ( sh < MINV ) sh = MINV;
      fac = 0.0;
      if ( ( y->nitems-y->size ) > 0 ) fac = ( scr_ln-sh ) / \
       ( float ) ( y->nitems-y->size ) ;
      else return;
      br->MS.fac = fac;
      br->fac = fac;
      pos = ( br->pos ) / ( nx ) ;
      if ( br->hitem >= y->nitems ) br->hitem = y->nitems-1;
      hitem = ( br->hitem ) / ( nx ) ;
      if ( pos > hitem ) pos = hitem;
      while ( ( hitem-pos ) >= y->size ) pos++;
      br->pos = pos*nx;
      j = pos;
      sy = j*fac+0.5;
      br->sy = sy;
      br->shy = sh;
      _dvmove_vert_pointer ( D , xx , yy , w , h , sy , sh ) ;
      return;
  }
  void _uiDrawScrollMenu ( BRW_STR *br ) {
      Gclr gc;
      float fac;
      int sh = 20.0 , scr_ln = 0 , sy , item , pos;
      int xx , yy , w , h;
      int thirty = 20;
      int scroll = 1;
      char *ptr;
      int n = 0 , max = 0 , i , j;
      DIALOG *D;
      int EVGAY;
      D = ( DIALOG * ) ( br->D ) ;
      EVGAY = D->evgay;
      while ( br->menu [ n ] != NULL ) n++;
      if ( br->size > n ) br->size = n;
      if ( br->size == n ) scroll = 0;
      max = 4;
      for ( i = 0; i < n; i++ ) {
          ptr = br->menu [ i ] ;
          j = 0; while ( * ( ptr+j ) != '\0' ) {j++; }
          if ( max < j ) max = j;
      }
      max++;
      gc = D->gc;
      br->D = D;
      br->hitem = br->df;
      br->MS.color1 = gc.fill_clr; br->MS.color2 = gc.high_clr;
      br->MS.char_clr = gc.char_clr;
      br->MS.ixx = br->x1;
      br->MS.iyy = br->y1;
      br->MS.imenu = br->size;
      br->MS.nitems = n;
      br->MS.por = br->size;
      br->MS.ichr = max;
      br->MS.thirty = thirty;
      br->MS.iyl = ( br->size+1 ) *thirty;
      br->MS.ixl = ( max*9+15 ) ;
      br->MS.iyu = br->MS.iyy +br->MS.iyl;
      br->MS.ixu = br->MS.ixx+br->MS.ixl-1;
      kg_scr_back ( WC ( D ) , br->MS.ixx-8 , br->MS.iyy-18 , \
           ( br->MS.ixx+br->MS.ixl+31 ) , \
       ( br->MS.iyy+br->MS.iyl+18 ) ) ;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      _uirect_fill ( WC ( D ) , br->MS.ixx-8 , EVGAY-br->MS.iyy+18 , \
           ( br->MS.ixx+br->MS.ixl+31 ) , EVGAY- \
       ( br->MS.iyy+br->MS.iyl+18 ) , gc.fill_clr ) ;
//   _uiborder3d(D, br->MS.ixx-8,br->MS.iyy-18,(br->MS.ixx+br->MS.ixl+31),(br->MS.iyy+br->MS.iyl+18));
      _uibordertype4 ( D , br->MS.ixx-8 , br->MS.iyy-18 , \
           ( br->MS.ixx+br->MS.ixl+31 ) , \
       ( br->MS.iyy+br->MS.iyl+18 ) ) ;
      uiRest_clip_limits ( WC ( D ) ) ;
      pos = 1;
      xx = br->MS.ixx+br->MS.ixl+1;
      yy = br->MS.iyy-10;
      w = 24;
      h = br->MS.iyl+20;
//   _ui_vert_scroll_bar(D,xx,yy,w,h);
      _ui_vert_scroll_bar ( br ) ;
      _uiMoveVertPointer ( br ) ;
      _uiPutmsg ( br ) ;
      uiUpdateOn ( br->D ) ;
      return;
  }
  void _uiDrawMenu ( BRW_STR *br ) {
      Gclr gc;
      float fac;
      int sh = 20.0 , scr_ln = 0 , sy , item , pos;
      int X1 , Y1 , X2 , Y2;
      int xx , yy , w , h;
      int thirty = 20;
      int bodrwidth = 8;
      int scroll = 1;
      char *ptr;
      int n = 0 , max = 0 , i , j , EVGAY;
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) ( br->D ) ;
      wc = WC ( D ) ;
      EVGAY = wc->EVGAY;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      while ( br->menu [ n ] != NULL ) n++;
      if ( br->size > n ) br->size = n;
      if ( br->size == n ) {
          scroll = 0;
          bodrwidth = 0;
      }
      max = 4;
      for ( i = 0; i < n; i++ ) {
          ptr = br->menu [ i ] ;
#if 0
          j = 0; while ( * ( ptr+j ) != '\0' ) {j++; }
#else
          j = ( int ) ( uiStringLength ( ptr , D->gc.FontSize ) /D->gc.FontSize ) ;
#endif
          if ( max < j ) max = j;
      }
//  max++;
      gc = D->gc;
      if ( br->df > n ) br->df = 1;
      br->D = D;
      br->hitem = br->df;
      br->MS.color1 = gc.fill_clr; br->MS.color2 = gc.high_clr;
      br->MS.char_clr = gc.menu_char;
      br->MS.imenu = br->size;
      br->MS.nitems = n;
      br->MS.por = br->size;
      br->MS.ichr = max;
      br->MS.thirty = thirty;
      br->MS.iyl = ( br->size+1 ) *thirty;
      br->MS.ixl = ( max*D->gc.FontSize+15 ) ;
      br->MS.ixx = br->x1;
      br->MS.iyy = br->y1;
      if ( scroll == 0 ) {
          br->MS.ixx += 8;
          br->MS.iyy -= 2;
      }
      br->MS.iyu = br->MS.iyy +br->MS.iyl-1;
      br->MS.ixu = br->MS.ixx+br->MS.ixl-1;
      X1 = br->MS.ixx-bodrwidth;
      Y1 = br->MS.iyy-bodrwidth-scroll*10;
      X2 = ( br->MS.ixu+scroll*24+bodrwidth ) ;
      Y2 = ( br->MS.iyu+scroll*10+bodrwidth ) ;
      kg_scr_back ( WC ( D ) , X1 , Y1 , X2 , Y2 ) ;
//  Bkup_clip_limits();
//  Set_full_scrn();
      _uirect_fill ( WC ( D ) , X1 , EVGAY-Y1 , X2 , EVGAY-Y2 , gc.fill_clr ) ;
//  if(scroll) _uiborder3d(D, X1,EVGAY-Y1,X2, EVGAY-Y2);
      if ( scroll ) _uibordertype4 ( D , X1 , EVGAY-Y1 , X2 , EVGAY-Y2 ) ;
//  Rest_clip_limits();
      if ( n != 0 ) {
          pos = 1;
          if ( scroll ) {
              xx = br->MS.ixx+br->MS.ixl+1;
              yy = br->MS.iyy-10;
              w = 24;
              h = br->MS.iyl+20;
//TCB
              xx = br->MS.ixu+3;
              yy = br->MS.iyy-1;
              w = 24;
              h = br->MS.iyu- br->MS.iyy+4;
//     _ui_vert_scroll_bar(D,xx,yy,w,h);
              _ui_vert_scroll_bar ( br ) ;
              _uiMoveVertPointer ( br ) ;
          }
//TCBTCB
          _uiPutmenu ( br ) ;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
      uiUpdateOn ( br->D ) ;
      return;
  }
  void _uiDrawWMenu ( BRW_STR *br ) {
      Gclr gc;
      float fac;
      int sh = 20.0 , scr_ln = 0 , sy , item , pos;
      int X1 , Y1 , X2 , Y2;
      int xx , yy , w , h;
      int thirty = 20;
      int bodrwidth = 8;
      int scroll = 1;
      char *ptr;
      int n = 0 , max = 0 , i , j , EVGAY;
      DIALOG *D;
      kgWC *wc;
      DIW *W;
      D = ( DIALOG * ) ( br->D ) ;
      wc = WC ( D ) ;
      W = ( DIW * ) br->wid;
      EVGAY = wc->EVGAY;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      while ( br->menu [ n ] != NULL ) n++;
      if ( br->size > n ) br->size = n;
      if ( br->size == n ) {
          br->scroll = 0;
      }
      max = 4;
      for ( i = 0; i < n; i++ ) {
          ptr = br->menu [ i ] ;
          j = ( int ) ( uiStringLength ( ptr , D->gc.FontSize ) /D->gc.FontSize ) ;
          if ( max < j ) max = j;
      }
      gc = D->gc;
      if ( br->df > n ) br->df = 1;
      br->hitem = br->df;
      X1 = br->x1;
      Y1 = br->y1;
      X2 = br->x2;
      Y2 = br->y2;
      kg_scr_back ( WC ( D ) , X1 , Y1 , X2 , Y2 ) ;
      _dvrect_fill ( WC ( D ) , X1 , Y1 , X2 , Y2 , gc.fill_clr ) ;
      if ( scroll ) _dvbordertype4 ( D , X1 , Y1 , X2 , Y2 ) ;
      if ( n != 0 ) {
          pos = 1;
          if ( br->scroll ) {
              _ui_vert_scroll_wbar ( br ) ;
              _uiMoveWVertPointer ( br ) ;
          }
          _uiPutWmenu ( W ) ;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
      uiUpdateOn ( br->D ) ;
      return;
  }
  void _uiDrawDialogMenu ( BRW_STR *br ) {
      Gclr gc;
      float fac;
      int sh = 20.0 , scr_ln = 0 , sy , item , pos;
      int X1 , Y1 , X2 , Y2;
      int xx , yy , w , h;
      int thirty = 20;
      int bodrwidth = 8;
      int scroll = 1;
      char *ptr;
      int n = 0 , max = 0 , i , j;
      int EVGAY;
      DIALOG *D;
      D = ( DIALOG * ) ( br->D ) ;
      while ( br->menu [ n ] != NULL ) n++;
      if ( br->size > n ) br->size = n;
      if ( br->size == n ) {
          scroll = 0;
          bodrwidth = 0;
      }
      max = 4;
      for ( i = 0; i < n; i++ ) {
          ptr = br->menu [ i ] ;
          j = 0; while ( * ( ptr+j ) != '\0' ) {j++; }
          if ( max < j ) max = j;
      }
      max++;
//  Get_resolution(&EVGAX,&EVGAY);
      EVGAY = D->evgay;
      gc = D->gc;
      if ( br->df > n ) br->df = 1;
      br->D = D;
      br->hitem = br->df;
      br->MS.color1 = gc.fill_clr; br->MS.color2 = gc.high_clr;
      br->MS.char_clr = gc.menu_char;
      br->MS.imenu = br->size;
      br->MS.nitems = n;
      br->MS.por = br->size;
      br->MS.ichr = max;
      br->MS.thirty = thirty;
      br->MS.ixl = ( max*9+15 ) ;
//modified for unscrolled menu
      br->MS.iyl = ( br->size+1 ) *thirty;
      br->MS.ixl = br->x2 -br->x1-scroll*24-20;
      br->MS.ixx = br->x1+10;
      br->MS.iyy = br->y1+8+scroll*10;
      if ( scroll == 0 ) {
#if 0
          br->MS.ixx += 2;
          br->MS.iyy -= 2;
#else
          br->MS.ixx = br->x1+2+8;
          br->MS.iyy = br->y1+3+8;
#endif
      }
      br->MS.iyu = br->MS.iyy +br->MS.iyl;
      br->MS.ixu = br->MS.ixx+br->MS.ixl;
//TCB
      if ( D->DrawBkgr != 0 ) {
          int offset = 4;
          uiBkup_clip_limits ( WC ( D ) ) ;
          uiSet_full_scrn ( WC ( D ) ) ;
//    offset=scroll*4;
          _uirect_fill ( WC ( D ) , br->x1+ ( offset+2 ) , \
               D->evgay-br->y1- ( offset+2 ) , br->x2- \
           ( offset+2 ) , D->evgay-br->y2+ ( offset+2 ) , gc.txt_fill ) ;
#if 0
          _uibordertype0 ( D , br->x1+ ( offset+2 ) , D->evgay-br->y1-  \
              ( offset+2 ) , br->x2- \
           ( ( offset+2 ) -1 ) , D->evgay-br->y2+ ( ( offset+2 ) -1 ) , gc.vbright ) ;
#else
          _uibordertype1 ( D , br->x1+ ( offset+2 ) , D->evgay-br->y1-  \
              ( offset+2 ) , br->x2- \
           ( ( offset+2 ) -1 ) , D->evgay-br->y2+ ( ( offset+2 ) -1 ) ) ;
#endif
          uiRest_clip_limits ( WC ( D ) ) ;
      }
      if ( n != 0 ) {
          pos = 1;
          if ( scroll ) {
              xx = br->MS.ixx+br->MS.ixl+4;
              yy = br->MS.iyy-10;
              w = 24;
              h = br->MS.iyl+20;
//TCB
              xx = br->MS.ixu+3;
              yy = br->MS.iyy-1;
              w = 24;
              h = br->MS.iyu- br->MS.iyy+4;
//     vert_scroll_bar(xx,yy,w,h);
              _ui_vert_scroll_bar ( br ) ;
              _uiMoveVertPointer ( br ) ;
          }
          _uiPutmenu ( br ) ;
      }
      uiUpdateOn ( br->D ) ;
      return;
  }
  void _uiDrawDialogY ( DIY *y ) {
      void *img;
      BRW_STR *br;
      Gclr gc;
      int offset = 2;
      DIALOG *D;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      gc = D->gc;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      if ( y->hide != 1 ) {
//   _dvrect_fill(WC(D), br->x1+offset,br->y1+offset,br->x2-offset,br->y2-offset,gc.fill_clr);
          if ( y->Bimg == NULL ) y->Bimg = kgGetBackground \
           ( D , br->x1 , br->y1 , br->x2 , br->y2 ) ;
          else kgRestoreImage ( D , y->Bimg , br->x1 , br->y1 , \
           ( br->x2-br->x1+1 ) , ( br->y2-br->y1+1 ) ) ;
//   if((D->DrawBkgr!=0)&&(y->bkgr==1)) {
          if ( ( D->DrawBkgr != 0 ) ) {
              if ( ( y->bkgr == 1 ) ) {
                  img = kgBorderedRectangle ( br->x2-br->x1 , \
                       br->y2-br->y1 , gc.twin_fill , 0.0 ) ;
                  kgImage ( D , img , br->x1 , br->y1 , br->x2-br->x1 , \
                       br->y2-br->y1 , 0.0 , 1.0 ) ;
              }
              else {
                  img = kgBorderedRectangle ( br->x2-br->x1 , \
                       br->y2-br->y1 , gc.fill_clr , 0.0 ) ;
                  kgImage ( D , img , br->x1 , br->y1 , br->x2-br->x1 , \
                       br->y2-br->y1 , 0.0 , 1.0 ) ;
              }
          }
          if ( y->nitems != 0 ) {
              if ( br->scroll ) {
                  _ui_vert_scroll_ybar ( y ) ;
                  _uiMoveYVertPointer ( y ) ;
              }
              _uiPutYmenu ( y ) ;
          }
      }
      else{
//    _dvrect_fill(WC(D), br->x1,br->y1,br->x2,br->y2,gc.fill_clr);
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , br->x1 , br->y1 , \
           ( br->x2-br->x1+1 ) , ( br->y2-br->y1+1 ) ) ;
          uiRest_clip_limits ( WC ( D ) ) ;
      }
//  uiUpdateOn(br->D);
      return;
  }
  void _uiDrawDialogRadioButton ( DIRA *y ) {
      BRW_STR *br;
      Gclr gc;
      int offset = 2;
      DIALOG *D;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      gc = D->gc;
      if ( y->hide != 1 ) {
//   _dvrect_fill(WC(D), br->x1+(offset),br->y1+(offset),br->x2-(offset), br->y2-(offset),gc.fill_clr);
          if ( y->Bimg == NULL ) y->Bimg = kgGetBackground \
           ( D , br->x1 , br->y1 , br->x2 , br->y2 ) ;
          else kgRestoreImage ( D , y->Bimg , br->x1 , br->y1 , \
           ( br->x2-br->x1+1 ) , ( br->y2-br->y1+1 ) ) ;
//   if((D->DrawBkgr!=0)&&(y->bkgr==1)) {
          if ( ( D->DrawBkgr != 0 ) ) {
              uiBkup_clip_limits ( WC ( D ) ) ;
              uiSet_full_scrn ( WC ( D ) ) ;
              if ( ( y->bkgr == 1 ) ) {
                  _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
                   ( offset ) , br->y2- ( offset ) , gc.txt_fill ) ;
              }
              else {
                  _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
                   ( offset ) , br->y2- ( offset ) , gc.fill_clr ) ;
              }
              if ( y->bordr == 1 ) {
                  _dvbordertype0 ( D , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
                   ( offset ) , br->y2- ( offset ) , gc.vbright ) ;
              }
              uiRest_clip_limits ( WC ( D ) ) ;
          }
          if ( y->nitems != 0 ) {
              if ( br->scroll ) {
                  _ui_vert_scroll_ybar ( y ) ;
                  _uiMoveYVertPointer ( y ) ;
              }
              _uiPutRadioButton ( y ) ;
          }
      }
//  else _dvrect_fill(WC(D), br->x1,br->y1,br->x2,br->y2,gc.fill_clr);
//  uiUpdateOn(br->D);
      else {
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , br->x1 , br->y1 , \
           ( br->x2-br->x1+1 ) , ( br->y2-br->y1+1 ) ) ;
      }
      return;
  }
  void _uiDrawDialogCheckBox ( DICH *y ) {
      BRW_STR *br;
      Gclr gc;
      int offset = 2;
      DIALOG *D;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      gc = D->gc;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      if ( y->hide != 1 ) {
//   _dvrect_fill(WC(D), br->x1+(offset),br->y1+(offset),br->x2-(offset), br->y2-(offset),gc.fill_clr);
          if ( y->Bimg == NULL ) y->Bimg = kgGetBackground \
           ( D , br->x1 , br->y1 , br->x2 , br->y2 ) ;
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , br->x1 , br->y1 , \
           ( br->x2-br->x1+1 ) , ( br->y2-br->y1+1 ) ) ;
//   if((D->DrawBkgr!=0)&&(y->bkgr==1)) {
          if ( ( D->DrawBkgr != 0 ) ) {
              if ( y->bkgr == 1 ) {
                  _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
                   ( offset ) , br->y2- ( offset ) , gc.txt_fill ) ;
              }
              else {
                  _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
                   ( offset ) , br->y2- ( offset ) , gc.fill_clr ) ;
              }
              if ( y->bordr == 1 ) {
                  _dvbordertype0 ( D , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
                   ( offset ) , br->y2- ( offset ) , gc.vbright ) ;
              }
          }
          if ( y->nitems != 0 ) {
              if ( br->scroll ) {
                  _ui_vert_scroll_ybar ( y ) ;
                  _uiMoveYVertPointer ( y ) ;
              }
              _uiPutCheckBox ( y ) ;
          }
      }
      else {
//    _dvrect_fill(WC(D), br->x1,br->y1,br->x2,br->y2,gc.fill_clr);
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , br->x1 , br->y1 , \
           ( br->x2-br->x1+1 ) , ( br->y2-br->y1+1 ) ) ;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
//  uiUpdateOn(br->D);
      return;
  }
  void _uiDrawDialogX ( DIX *y ) {
      BRW_STR *br;
      void *img;
      Gclr gc;
      int offset = 2;
      DIALOG *D;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      gc = D->gc;
//  offset=y->offset;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      if ( y->hide != 1 ) {
//   _dvrect_fill(WC(D), br->x1+(offset),br->y1+(offset),br->x2-(offset), br->y2-(offset),gc.fill_clr);
          if ( y->Bimg == NULL ) y->Bimg = kgGetBackground \
           ( D , br->x1 , br->y1 , br->x2 , br->y2 ) ;
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , br->x1 , br->y1 , \
           ( br->x2-br->x1+1 ) , ( br->y2-br->y1+1 ) ) ;
          if ( ( D->DrawBkgr != 0 ) && ( y->bkgr == 1 ) ) {
#if 0
              _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
               ( offset ) , br->y2- ( offset ) , gc.twin_fill ) ;
              _dvbordertype0 ( D , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
               ( offset ) , br->y2- ( offset ) , gc.twin_bodr ) ;
//    _dvbordertype4(D, br->x1+(offset),br->y1+(offset),br->x2-(offset), br->y2-(offset));
#else
              img = kgBorderedRectangle ( br->x2-br->x1 , \
                   br->y2-br->y1 , gc.twin_fill , 0.0 ) ;
              kgImage ( D , img , br->x1 , br->y1 , br->x2-br->x1 , \
                   br->y2-br->y1 , 0.0 , 1.0 ) ;
#endif
          }
          if ( y->nitems != 0 ) {
              if ( br->scroll ) {
                  _ui_vert_scroll_ybar ( y ) ;
                  _uiMoveYVertPointer ( y ) ;
              }
              _uiPutXmenu ( y ) ;
          }
      }
      else{
//    _dvrect_fill(WC(D), br->x1,br->y1,br->x2,br->y2,gc.fill_clr);
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , br->x1 , br->y1 , \
           ( br->x2-br->x1+1 ) , ( br->y2-br->y1+1 ) ) ;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
//  uiUpdateOn(br->D);
      return;
  }
  void _uiDrawDialogV ( DIV *y ) {
      Gclr gc;
      int offset = 2 , x1 , y1 , x2 , y2;
      DIALOG *D;
      D = ( DIALOG * ) ( y->D ) ;
      gc = D->gc;
      offset = y->offset;
      x1 = y->x1+D->xo; x2 = y->x2+D->xo;
      y1 = y->y1+D->yo; y2 = y->y2+D->yo;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      if ( y->hide != 1 ) {
//     _dvrect_fill(WC(D), x1+(offset),y1+(offset),x2-(offset), y2-(offset),gc.fill_clr);
#ifndef D_RESTORE
          _dvrect_fill ( WC ( D ) , x1+1 , y1+1 , x2-1 , y2-1 , gc.fill_clr ) ;
#else
          if ( y->Bimg != NULL ) kgRestoreImagePart  \
              ( D , y->Bimg , x1+1 , y1+1 , 1 , 1 , \
           ( x2-x1-1 ) , ( y2-y1-1 ) ) ;
#endif
          _ui_vert_scroll_vbar ( y ) ;
          _uiMoveVVertPointer ( y ) ;
      }
      else {
//   _dvrect_fill(WC(D), x1,y1,x2,y2,gc.fill_clr);
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , x1 , y1 , \
           ( x2-x1+1 ) , ( y2-y1+1 ) ) ;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
//  uiUpdateOn(D);
      return;
  }
  void _uiDrawDialogZ ( DIZ *y ) {
      Gclr gc;
      int offset = 2 , x1 , y1 , x2 , y2;
      DIALOG *D;
      D = ( DIALOG * ) ( y->D ) ;
      gc = D->gc;
      offset = y->offset;
      x1 = y->x1+D->xo; x2 = y->x2+D->xo;
      y1 = y->y1+D->yo; y2 = y->y2+D->yo;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      if ( y->hide != 1 ) {
//     _dvrect_fill(WC(D), x1+(offset),y1+(offset),x2-(offset), y2-(offset),gc.fill_clr);
#ifndef D_RESTORE
          _dvrect_fill ( WC ( D ) , x1+1 , y1+1 , x2-1 , y2-1 , gc.fill_clr ) ;
#else
          if ( y->Bimg != NULL ) kgRestoreImagePart  \
              ( D , y->Bimg , x1+1 , y1+1 , 1 , 1 , \
           ( x2-x1-1 ) , ( y2-y1-1 ) ) ;
#endif
          _ui_vert_scroll_vbar ( y ) ;
          _ui_vert_scroll_zbar ( y ) ;
          _uiMoveZVertPointer ( y ) ;
      }
      else{
//    _dvrect_fill(WC(D), x1,y1,x2,y2,gc.fill_clr);
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , x1 , y1 , \
           ( x2-x1+1 ) , ( y2-y1+1 ) ) ;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
//  uiUpdateOn(D);
      return;
  }
  void _uiDrawDialogE ( DIE *y ) {
      BRW_STR *br;
      Gclr gc;
      int offset = 2;
      DIALOG *D;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      gc = D->gc;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
//  if(offset<y->offset) offset=y->offset;
      if ( y->hide != 1 ) {
//   _dvrect_fill(WC(D), br->x1+(offset),br->y1+(offset),br->x2-(offset), br->y2-(offset),gc.fill_clr);
          if ( y->Bimg == NULL ) y->Bimg = kgGetBackground \
           ( D , br->x1 , br->y1 , br->x2 , br->y2 ) ;
          else kgRestoreImage ( D , y->Bimg , br->x1 , br->y1 , \
           ( br->x2-br->x1+1 ) , ( br->y2-br->y1+1 ) ) ;
          if ( ( D->DrawBkgr != 0 ) && ( y->bkgr == 1 ) ) {
              _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
               ( offset ) , br->y2- ( offset ) , gc.txt_fill ) ;
              _dvbordertype0 ( D , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
               ( offset ) , br->y2- ( offset ) , gc.vbright ) ;
          }
          if ( y->nitems != 0 ) {
              if ( br->scroll ) {
                  _ui_vert_scroll_ebar ( y ) ;
                  _uiMoveEVertPointer ( y ) ;
              }
              _uiPutEmenu ( y ) ;
          }
      }
      else {
//   _dvrect_fill(WC(D), br->x1,br->y1,br->x2,br->y2,gc.fill_clr);
          if ( y->Bimg != NULL ) kgRestoreImage ( D , y->Bimg , br->x1 , br->y1 , \
           ( br->x2-br->x1+1 ) , ( br->y2-br->y1+1 ) ) ;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
//  uiUpdateOn(br->D);
      return;
  }
  void _uiDrawDialogM ( DIS *y ) {
      BRW_STR *br;
      Gclr gc;
      int offset = 2;
      DIALOG *D;
      char **menu;
      br = y->bwsr;
      D = ( DIALOG * ) ( y->D ) ;
      gc = D->gc;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
#if 0
      _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
       ( offset ) , br->y2- ( offset ) , gc.fill_clr ) ;
      if ( ( D->DrawBkgr != 0 ) && ( y->bkgr == 1 ) ) {
          _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
           ( offset ) , br->y2- ( offset ) , gc.txt_fill ) ;
          _dvbordertype0 ( D , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
           ( offset ) , br->y2- ( offset ) , gc.vbright ) ;
      }
#else
      _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
       ( offset ) , br->y2- ( offset ) , gc.fill_clr ) ;
      if ( ( D->DrawBkgr != 0 ) && ( y->bkgr == 1 ) ) {
#if 0
          _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
           ( offset ) , br->y2- ( offset ) , br->MS.color1 ) ;
          _dvbordertype0 ( D , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
           ( offset ) , br->y2- ( offset ) , br->MS.color2 ) ;
#else
          _dvrect_fill ( WC ( D ) , ( br->x1+offset ) , ( br->y1+offset ) , \
           ( br->x2-offset ) , ( br->y2-offset ) , br->MS.color2 ) ;
          _dvrect_fill ( WC ( D ) , ( br->x1+offset ) +3 , ( br->y1+offset ) +3 , \
           ( br->x2-offset ) -3 , ( br->y2-offset ) -3 , br->MS.color1 ) ;
#endif
      }
#endif
      if ( y->nitems != 0 ) {
          if ( br->scroll ) {
              _ui_vert_scroll_mbar ( y ) ;
              _uiMoveMVertPointer ( y ) ;
          }
          _uiPutMmenu ( y ) ;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
//  uiUpdateOn(br->D);
      return;
  }
  void _uiDrawDialogW ( DIW *y ) {
      BRW_STR *br;
      Gclr gc;
      int offset = 0;
      DIALOG *D;
      br = y->bwsr;
      D = ( DIALOG * ) ( br->D ) ;
      gc = D->gc;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
       ( offset ) , br->y2- ( offset ) , gc.txt_fill ) ;
      _dvbordertype0 ( D , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
       ( offset ) , br->y2- ( offset ) , gc.vbright ) ;
      if ( y->nitems != 0 ) {
          if ( br->scroll ) {
              _ui_vert_scroll_wbar ( y ) ;
              _uiMoveWVertPointer ( y ) ;
          }
          _uiPutWmenu ( y ) ;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
//  uiUpdateOn(br->D);
      return;
  }
  void _uiDrawMsg ( DIS *w ) {
      BRW_STR *br;
      Gclr gc;
      float fac;
      int sh = 20.0 , scr_ln = 0 , sy , item , pos;
      int X1 , Y1 , X2 , Y2;
      int xx , yy , h;
      int thirty = 20;
      int bodrwidth = 8 , offset = 0;
      int scroll = 1;
      char *ptr;
      int n = 0 , max = 0 , i , j;
      DIALOG *D;
      char **menu;
      br = w->bwsr;
      D = ( DIALOG * ) ( w->D ) ;
      offset = w->offset;
      menu = w->menu;
      n = 0;
      if ( menu != NULL ) while ( menu [ n ] != NULL ) n++;
      else n = 1;
      if ( br->size > n ) br->size = n;
      if ( br->size == n ) {
          scroll = 0;
          bodrwidth = 0;
      }
      max = 4;
      if ( menu != NULL ) {
          for ( i = 0; i < n; i++ ) {
              ptr = menu [ i ] ;
              if ( ptr == NULL ) break;
              j = 0; while ( * ( ptr+j ) != '\0' ) {j++; }
              if ( max < j ) max = j;
          }
      }
      max++;
      gc = D->gc;
      br->D = D;
      br->MS.color1 = gc.fill_clr; br->MS.color2 = gc.high_clr;
      br->MS.color1 = gc.msg_fill; br->MS.color2 = gc.msg_bodr;
      br->MS.char_clr = gc.msg_char;
//TCB
      br->MS.imenu = br->size;
      br->MS.nitems = n;
      br->MS.por = br->size;
      br->MS.ichr = max;
      br->MS.thirty = thirty;
      br->MS.iyl = ( br->size+1 ) *thirty;
      br->MS.ixl = ( max*9+15 ) ;
      br->MS.ixx = br->x1;
      br->MS.iyy = br->y1;
      br->MS.ixl = br->x2 -br->x1-44;
      br->MS.ixx = br->x1+10;
      br->MS.iyy = br->y1+10;
      if ( scroll == 0 ) {
          br->MS.ixx += 8;
          br->MS.iyy -= 2;
      }
      br->MS.iyu = br->MS.iyy +br->MS.iyl;
      br->MS.ixu = br->MS.ixx+br->MS.ixl-1;
      X1 = br->MS.ixx-bodrwidth;
      Y1 = br->MS.iyy-bodrwidth-scroll*10;
      X2 = ( br->MS.ixx+br->MS.ixl+scroll*24+bodrwidth ) ;
      Y2 = ( br->MS.iyy+scroll*10+bodrwidth+ ( br->MS.iyl ) ) ;
      uiBkup_clip_limits ( WC ( D ) ) ;
      uiSet_full_scrn ( WC ( D ) ) ;
      _dvrect_fill ( WC ( D ) , br->x1+offset , br->y1+offset , \
           br->x2-offset , br->y2-offset , gc.fill_clr ) ;
//TCBTCB
#if 1
      _dvbordertype1 ( D , br->x1+offset , br->y1+offset , \
           br->x2-offset , br->y2-offset ) ;
#else
      if ( ( D->DrawBkgr != 0 ) && ( w->bkgr == 1 ) ) {
          _dvrect_fill ( WC ( D ) , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
           ( offset ) , br->y2- ( offset ) , br->MS.color1 ) ;
          _dvbordertype0 ( D , br->x1+ ( offset ) , br->y1+ ( offset ) , br->x2- \
           ( offset ) , br->y2- ( offset ) , br->MS.color2 ) ;
      }
#endif
      uiRest_clip_limits ( WC ( D ) ) ;
      if ( n != 0 ) {
          pos = 1;
          if ( scroll ) {
              xx = br->MS.ixx+br->MS.ixl+1;
              yy = br->MS.iyy-10;
              h = br->MS.iyl+20;
              xx = br->MS.ixu+3;
              yy = br->MS.iyy-1;
              h = br->MS.iyu- br->MS.iyy+4;
              _ui_vert_scroll_bar ( br ) ;
              _uiMoveVertPointer ( br ) ;
          }
//   _uiPutmsg(br);
          _uiPutMmenu ( w ) ;
      }
//  uiUpdateOn(br->D);
      return;
  }
  void uiwrt_msg ( DIALOG *Dtmp , int ix , int iy , int max1 , \
       char *ch1 , int fill_clr , int char_clr ) \
  {
      int l , in = 0 , j , x1 , y1 , x2 , y2 , i , menu_x , menu_y;
      int nx , ny;
      int n , buf , x , y;
      unsigned int temp , c_color;
      char c;
      char *ptr;
      int EVGAY;
      DIALOG *D;
      D = ( Dtmp ) ;
      EVGAY = D->evgay;
      c_color = WC ( D )->c_color;
      n = 1 , x = ix , y = iy;
      y = EVGAY-y;
      l = 22;
      x1 = x;
      ny = y-18;
      y1 = y-l;
      y2 = y;
      x2 = x+ ( max1 ) *9+5;
//   _uirect_fill(WC(D),x1,y1,x2,y2,fill_clr);
      menu_y = ny;
      ptr = ch1;
      uiset_clr ( D , char_clr ) ;
      uiSetGuiFixFontSize ( D , D->gc.InputFontSize ) ;
      menu_x = x1+5;
#if 0   // new writting method
      ch1 = ptr;
      j = 1;
      while ( *ch1 != '\0' ) {
          if ( j++< max1 ) uimenuch ( D , *ch1 , menu_x , menu_y , char_clr ) ;
          ch1++;
          menu_x+= 9;
      }
#else
//     uiwrite_string(D,ch1,menu_x,menu_y,char_clr);
      uiString ( D , ch1 , ix , iy , max1*9 , 22 , D->gc.MsgFont , \
           char_clr , D->gc.FontSize , -1 , fill_clr ) ;
#endif
      uiDefaultGuiFontSize ( D ) ;
      uiset_clr ( D , c_color ) ;
  }
  void uiscroll_up_twin ( DIALOG *D , TWIN *Twin ) {
#if 0
      int EVGAX , EVGAY;
      EVGAX = D->evgax;
      EVGAY = D->evgay;
      uig_scroll_up ( D , ( int ) Twin->xl , EVGAY- ( int ) Twin->yl , \
       ( int ) Twin->xu , EVGAY- ( int ) Twin->yu , Twin->w ) ;
#else
      int yy1 , yy2 , jmp;
      yy1 = ( int ) Twin->yl;
      yy2 = ( int ) Twin->yu;
      jmp = Twin->w;
      if ( yy1+jmp > yy2 ) return;
      uiscr_scroll_back ( D , ( int ) Twin->xl , yy1+jmp , \
       ( int ) Twin->xu , yy2 , jmp ) ;
#endif
      return;
  }
  void uiscroll_up ( DIALOG *D ) {
      int EVGAX , EVGAY;
      TWIN *Twin;
      Dend ( ( Dlink * ) ( WC ( D )->TLIST ) ) ;
      Twin = ( TWIN * ) Getrecordrev ( ( Dlink * ) ( WC ( D )->TLIST ) ) ;
      if ( Twin == NULL ) return;
      EVGAX = D->evgax;
      EVGAY = D->evgay;
      uig_scroll_up ( D , ( int ) Twin->xl , EVGAY- ( int ) Twin->yl , \
       ( int ) Twin->xu , EVGAY- ( int ) Twin->yu , 22 ) ;
      return;
  }
  void ui_wprintf ( DIALOG *D , char *s ) {
      int n , i , xp , yp , yu , pointer , j;
      char buf [ 120 ] ;
      TWIN *Twin;
      int EVGAX , EVGAY;
      EVGAX = D->evgax;
      EVGAY = D->evgay;
      Dend ( ( Dlink * ) ( WC ( D )->TLIST ) ) ;
      Twin = ( TWIN * ) Getrecordrev ( ( Dlink * ) ( WC ( D )->TLIST ) ) ;
      if ( Twin == NULL ) return;
      uiUpdateOff ( D ) ;
      xp = Twin->xp;
      yp = Twin->yp;
      yu = Twin->yu-22-5;
      n = Twin->nchr;
      i = 0;
      do {
          j = 0;
          while ( ( ( * ( s+i ) >= ' ' ) || ( * ( s+i ) == '\t' ) ) && \
           ( j < n ) ) {buf [ j ] = * ( s+i ) ; i++; j++; }
          buf [ j ] = '\0';
          if ( yp > yu ) { yp-= 22; uiscroll_up_twin ( D , Twin ) ; };
          uiwrt_msg ( D , xp+3 , yp , n , buf , D->gc.twin_fill , D->gc.twin_char ) ;
          if ( * ( s+i ) != '\r' ) yp += 22;
          if ( ( * ( s+i ) < ' ' ) && ( * ( s+i ) != '\0' ) && \
           ( * ( s+i ) != '\n' ) ) i++;
      } while ( ( * ( s+i ) != '\0' ) && ( * ( s+i ) != '\n' ) ) ;
      Twin->yp = yp;
      uiUpdateOn ( D ) ;
  }
  void ui_twinmove ( DIALOG *D , int col , int row ) {
      int yp , xp;
      TWIN *Twin;
      Dend ( ( Dlink * ) ( WC ( D )->TLIST ) ) ;
      Twin = ( TWIN * ) Getrecordrev ( ( Dlink * ) ( WC ( D )->TLIST ) ) ;
      if ( Twin == NULL ) return;
      if ( Twin->w == -1 ) return;
      xp = Twin->xl+ ( col-1 ) *9;
      if ( xp < Twin->xu ) Twin->xp = xp;
      yp = Twin->yl+22* ( row-1 ) ;
      if ( yp < Twin->yu ) Twin->yp = yp;
  }
  void ui_txtwin ( DIALOG *D , int ix , int iy , int chrs , int lines ) {
      int x2 , y , y2;
      int pointer;
      int EVGAX , EVGAY;
      TWIN *Twin;
      EVGAX = D->evgax;
      EVGAY = D->evgay;
      uiUpdateOff ( D ) ;
  /*ix = (ix/8)*8;*/
      x2 = ix+chrs*9+10;
  /*x2 = ((x2/8)+2)*8;*/
      y2 = ( 22*lines+iy+5 ) ;
      kg_scr_back ( WC ( D ) , ( int ) ix-3 , ( int ) ( iy-3 ) , x2+3 , y2+3 ) ;
      _uirect_fill ( WC ( D ) , ( int ) ix , ( int ) EVGAY-iy , \
       ( int ) x2 , ( int ) EVGAY-y2 , D->gc.twin_fill ) ;
      _ui_draw_bound ( D , ( int ) ix-1 , ( int ) EVGAY-iy+1 , \
       ( int ) x2+1 , ( int ) EVGAY-y2-1 , D->gc.twin_bodr ) ;
      _ui_draw_bound ( D , ( int ) ix-2 , ( int ) EVGAY-iy+2 , \
       ( int ) x2+2 , ( int ) EVGAY-y2-2 , D->gc.twin_bodr ) ;
      _ui_draw_bound ( D , ( int ) ix-3 , ( int ) EVGAY-iy+3 , \
       ( int ) x2+3 , ( int ) EVGAY-y2-3 , D->gc.twin_bodr ) ;
      Twin = ( TWIN * ) Malloc ( sizeof ( TWIN ) ) ;
      Twin->xl = ix; Twin->yl = iy; Twin->xu = x2;
      Twin->yu = y2; Twin->w = 1;
      Twin->nchr = chrs;
      Twin->xp = ix;
      Twin->yp = iy;
      Dappend ( ( Dlink * ) WC ( D )->TLIST , Twin ) ;
      uiUpdateOn ( D ) ;
  }
  void ui_txtwinnew ( DIALOG *D , int ix , int iy , \
       int chrs , int lines , int xbdr , int ybdr ) \
  {
      int x2 , y , y2;
      int pointer;
      int dx , dy;
      TWIN *Twin;
      int EVGAX , EVGAY;
      EVGAX = D->evgax;
      EVGAY = D->evgay;
      uiUpdateOff ( D ) ;
      dx = xbdr+5;
      dy = ybdr+5;
  /*ix = (ix/8)*8;*/
      x2 = ix+chrs*9+2*dx;
  /*x2 = ((x2/8)+2)*8;*/
      y2 = ( 22*lines+iy+dy ) ;
      kg_scr_back ( WC ( D ) , ( int ) ix-dx , ( int ) ( iy-dy ) , x2+dx , y2+dy ) ;
      ix -= xbdr;
      iy -= ybdr;
      _uirect_fill ( WC ( D ) , ( int ) ix , ( int ) EVGAY-iy , \
       ( int ) x2 , ( int ) EVGAY-y2 , D->gc.twin_fill ) ;
      _ui_draw_bound ( D , ( int ) ix-1 , ( int ) EVGAY-iy+1 , \
       ( int ) x2+1 , ( int ) EVGAY-y2-1 , D->gc.twin_bodr ) ;
      _ui_draw_bound ( D , ( int ) ix-2 , ( int ) EVGAY-iy+2 , \
       ( int ) x2+2 , ( int ) EVGAY-y2-2 , D->gc.twin_bodr ) ;
      _ui_draw_bound ( D , ( int ) ix-3 , ( int ) EVGAY-iy+3 , \
       ( int ) x2+3 , ( int ) EVGAY-y2-3 , D->gc.twin_bodr ) ;
      Twin = ( TWIN * ) Malloc ( sizeof ( TWIN ) ) ;
      Twin->xl = ix; Twin->yl = iy; Twin->xu = x2;
      Twin->yu = y2; Twin->w = 1;
      Twin->nchr = chrs;
      Twin->xp = ix;
      Twin->yp = iy;
      Dappend ( ( Dlink * ) WC ( D )->TLIST , Twin ) ;
      uiUpdateOn ( D ) ;
  }
  void ui_clear_txtwin ( DIALOG *D ) {
      kg_scr_recover ( WC ( D ) ) ;
      Dend ( ( Dlink * ) ( WC ( D )->TLIST ) ) ;
      Ddelete ( ( Dlink * ) ( WC ( D )->TLIST ) ) ;
  }
  void uitwin_wprintf ( DIALOG *D , void *Tmp , char *s ) {
      TWIN *Twin;
      int n , i , xp , yp , yu , pointer , j;
      char buf [ 120 ] ;
      Twin = ( TWIN * ) Tmp;
      if ( Twin == NULL ) return;
      xp = Twin->xp;
      yp = Twin->yp;
      yu = Twin->yu-22-2;
      n = Twin->nchr;
      i = 0;
      do {
          j = 0;
          while ( ( ( * ( s+i ) >= ' ' ) || ( * ( s+i ) == '\t' ) ) && \
           ( j < n ) ) {buf [ j ] = * ( s+i ) ; i++; j++; }
          buf [ j ] = '\0';
          if ( yp > yu ) { yp-= 22; uiscroll_up_twin ( D , Twin ) ; };
          uiwrt_msg ( D , xp+3 , yp , n , buf , D->gc.twin_fill , D->gc.twin_char ) ;
          if ( * ( s+i ) != '\r' ) yp += 22;
          if ( ( * ( s+i ) < ' ' ) && ( * ( s+i ) != '\0' ) && \
           ( * ( s+i ) != '\n' ) ) i++;
      } while ( ( * ( s+i ) != '\0' ) && ( * ( s+i ) != '\n' ) ) ;
      Twin->yp = yp;
  }
  void uiinfo_wprintf ( DIALOG *D , void *Tmp , char *s ) {
      TWIN *Twin;
      int n , i , xp , yp , yu , pointer , j , ln , lw;
      char buf [ 500 ] ;
      Twin = ( TWIN * ) Tmp;
      if ( Twin == NULL ) return;
      lw = Twin->w;
      xp = Twin->xp;
      yp = Twin->yp;
      yu = Twin->yu-lw;
      n = Twin->nchr;
      ln = Twin->xu-Twin->xl;
      i = 0;
      do {
          j = 0;
//TCB made these modifications as on 19th Oct 13
//    while(( (*(s+i) >=' ')||(*(s+i)=='\t'))&&(j<n)) {buf[j]=*(s+i);i++;j++;}
          while ( ( ( * ( s+i ) >= ' ' ) || ( * ( s+i ) == '\t' ) ) ) \
          {buf [ j ] = * ( s+i ) ; i++; j++; }
          buf [ j ] = '\0';
          if ( yp > yu ) { yp-= lw; uiscroll_up_twin ( D , Twin ) ; };
//    uiwrt_msg(D,xp,yp,n,buf,D->gc.info_fill,D->gc.info_char);
          uiString ( D , buf , xp , yp , ln , lw , D->gc.MsgFont , D->gc.info_char , \
               D->gc.FontSize , -1 , D->gc.info_fill ) ;
          if ( * ( s+i ) != '\r' ) yp += lw;
          if ( ( * ( s+i ) < ' ' ) && ( * ( s+i ) != '\0' ) && \
           ( * ( s+i ) != '\n' ) ) i++;
      } while ( ( * ( s+i ) != '\0' ) && ( * ( s+i ) != '\n' ) ) ;
      Twin->yp = yp;
  }
  void uimsg_menu ( DIG *G , int ix , int iy , int max1 , char *ch1 ) {
      int l , in = 0 , j , x1 , y1 , x2 , y2 , i , menu_x , menu_y;
      int nx , ny , txtpos;
      int n , buf , x , y;
      unsigned int temp , chclrtmp;
      char c;
      char *ptr , *ptr2 , ans;
      int EVGAY;
      kgWC *wc;
      kgDC *dc;
      DIALOG *D;
      D = G->D;
      wc = G->wc;
      EVGAY = wc->EVGAY ;
      n = 1 , x = ix , y = iy;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      y = EVGAY-y;
      l = 22;
      x1 = x;
      ny = y-18;
      y1 = y-l;
      y2 = y;
      x2 = x+ ( max1 ) *9+10;
      txtpos = x+max1*9 +40;
      _uirect_fill ( wc , x1 , y1 , x2 , y2 , D->gc.msg_fill ) ;
      menu_y = ny;
      menu_x = x1+15;
      j = 0;
      uiSetGuiFixFontSize ( D , D->gc.InputFontSize ) ;
      uiMenustr ( D , menu_x , menu_y , ch1 , D->gc.msg_char ) ;
      uiDefaultGuiFontSize ( D ) ;
/*
   while ((ch1[j]!='\0')&&(j<max1))
     {
       uimenuch(D,ch1[j],menu_x,menu_y,msg_c_clr);
       j++;
       menu_x+=9;
     }
*/
      uiRest_clip_limits ( wc ) ;
  }
  int _uiMenu ( DIW *w ) {
      BRW_STR *br;
      char *ptr , *cmenu , **x;
      int por , df , *pos;
      int xx , yy , h , sy = 0 , sh = 20 , mvnt = 0 , NOK = 1 , shift = 0;
      int n = 0 , i , j , k = 0 , item , ok , prop , \
           max , it , scr_ln = 0 , jold , Attn = 0;
      float fac = 1.;
      DIALOG *D;
      KBEVENT kbevent;
      br = w->bwsr;
      D = w->D;
      uiUpdateOff ( D ) ;
      _uiMake_Wbwsr ( ( DIW * ) ( w ) ) ;
      item = br->df;
      x = br->menu;
      if ( br->menu [ 0 ] == NULL ) return 1;
      NOK = 1;
      uiUpdateOn ( D ) ;
      while ( NOK != 0 ) {
          kbevent = kgSkipMouseMove ( br->D ) ;
          D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
          switch ( kbevent.event ) {
              case 0:
//          _ui_scroll_item_hilight(br,kbevent);
              continue;
              case 1:
              ok = _ui_process_w_click ( w , kbevent ) ;
              if ( ok >= 0 ) {
                  if ( ok == 0 ) br->df = item;
                  NOK = 0;
              }
              continue;
              case 2:
              continue;
              case 3:
              _ui_process_w_move ( w , kbevent ) ;
              continue;
              case 4:
              continue;
              case 5:
              ok = _ui_process_w_key ( w , kbevent ) ;
              if ( ok >= 0 ) {
                  if ( ok == 0 ) br->df = item;
                  NOK = 0;
              }
              continue;
          }
      }
      item = br->df;
      kg_scr_recover ( WC ( D ) ) ;
      return ( item ) ;
  }
