#define D_TH
#include "kulina.h"
#include "gprivate.h"
#include "dlink.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>
#include "imageswarn.c"
#include "images.c"
#define RESIZE 5
  static int B_max = 990000 , B_min = 989000;
  extern int TextSize , Ht , Wd , Gap , Bt; // It is Okay For Thread;
  void *stop_xpm = & Stop_str;
  void *mickey_xpm = & Hi_str;
  void *qst_xpm = & Warn_str;
  static void uiFreemenu ( char **m ) {
      int i;
      if ( m != NULL ) {
          i = 0;
          while ( m [ i ] != NULL ) {free ( m [ i ] ) ;i++;}
          free ( m ) ;
      }
  }
  int _check_menu ( void *parent , int x0 , int y0 , \
  char *qst , int df , void *xpm ) \
  {
      int ret = 1;
      int v0;
      DIALOG D;
      DIA d [ 4 ] ;
      DIM m0 = {
      'm' , 83 , 12 , 457 , 36 , 0 , 0 };
      strncpy ( m0.msg , qst , 499 ) ;
      DIP p1 = {
          'p' , 11 , 14 , 74 , 74 , xpm , /* pixmap info */
          -1 , 0 , 0 /* bkgr colour */
      };
      char *titles2 [ ] = {
      "No" , "Yes" , NULL };
      void *xpm2 [ ] = {
      NULL , NULL , NULL , NULL , NULL , NULL , NULL };
      int bkgr2 [ ] = {
      -1 , -1 , -1 };
      char *butncode2 = NULL;
      int sw [ ] = {1 , 1};
      DIB n2 = {
          'b' , 148 , 45 , 354 , 96 , 30 , 9 , 80 , 35 , 2 , 1 , & v0 , sw , \
          titles2 , butncode2 , NULL , NULL , /* args , Callbak */
          xpm2 , bkgr2 /* pointers to xpms and colors */
       , 2 , 0.5 };
      v0 = df+1;
      d [ 0 ] .m = & m0;
      d [ 1 ] .p = & p1;
      d [ 2 ] .b = & n2;
      d [ 3 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 4;
      D.df = -1;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = x0; /* Position of Dialog */
      D.yo = y0;
      D.xl = 500; /* Length of Dialog */
      D.yl = 102; /* Width of Dialog */
      D.VerId = 1401010200;
      D.Initfun = NULL; /* Width of Dialog */
      D.Deco = 1;
      D.DrawBkgr = 1;
      D.Bkpixmap = NULL;
      D.Callback = NULL;
      D.transparency = 0.0;
      D.Sticky = 0;
      D.Fixpos = 0;
      D.NoTaskBar = 0;
      D.Newwin = 0;
      D.parent = parent;
      D.StackPos = 1;
      D.Shapexpm = NULL;
      strcpy ( D.name , "Kulina Graphics" ) ;
      if ( parent == NULL ) D.Newwin = 1;
      kgDefaultGuiTheme ( & ( D.gc ) ) ;
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      return ( v0-1 ) ;
  }
  int kgCheckMenu ( void *parent , int x , int y , char *qst , int df ) {
      return _check_menu ( parent , x , y , qst , df , mickey_xpm ) ;
  }
  int kgWarnMenu ( void *parent , int x , int y , char *qst , int df ) {
      return _check_menu ( parent , x , y , qst , df , stop_xpm ) ;
  }
  int kgQstMenu ( void *parent , int x , int y , char *qry , int df ) {
      return _check_menu ( parent , x , y , qry , df , qst_xpm ) ;
  }
  int kgSplashDiainit ( void *Tmp ) {
      int ret = 0 , i , j , OK , xl , yl , count = 0 , k;
      float ln , length = 0 , fac = 1.0;
      float w_x1 = 0.0 , w_y1 = 0.0 , w_x2 = 686 , w_y2 = 226 , dyl , dxl , yy;
      void *fid , *Img;
      KBEVENT kbe;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      char *pt , buf [ 5000 ] , flname [ 200 ] , *str [ 500 ] ;
      DIP *p0 ;
      SPLASHMSG *msg;
      p0 = ( D->d [ 0 ] ) .p;
      xl = D->xl;
      yl = D->yl;
      w_x2 = xl;
      w_y2 = yl;
      kgWindowStateAbove ( Tmp ) ;
#if 1
      msg = D->pt;
      if ( msg->message != NULL ) {
          fid = kgInitImage ( ( int ) ( w_x2-w_x1 ) +1 , \
           ( int ) ( w_y2-w_y1 ) +1 , 2 ) ;
          kgUserFrame ( fid , w_x1 , w_y1 , w_x2 , w_y2 ) ;
          kgTextColor ( fid , msg->fontcolor ) ;
          kgTextFont ( fid , msg->font ) ;
          kgTextSize ( fid , 30. , 20. , 0. ) ;
          strcpy ( buf , ( char * ) msg->message ) ;
          pt = buf;
          j = 1;
          k = 0;
          while ( 1 ) {
              i = 0;
              if ( pt [ i ] == '\0' ) break;
//      gphMove2f(fid,40.,yl-j*45.);
              j++;
              OK = 1;
              while ( pt [ i ] != '\0' ) {
                  OK = 0;
                  if ( ( pt [ i ] == '\n' ) ) { pt [ i ] = '\0';break;}
                  if ( ( ( pt [ i ] == '\\' ) && ( pt [ i+1 ] == 'n' ) ) ) \
                  { pt [ i ] = '\0';i++;break;}
                  i++;
                  OK = 1;
              }
              str [ k++ ] = pt;
              ln = kgStringLength ( fid , pt ) ;
              if ( length < ln ) length = ln;
//      gphWriteText(fid,pt);
              if ( OK ) break;
              pt = pt+i+1;
          }
          if ( k > 0 ) {
              dyl = yl/k;
              fac = xl/ ( length ) ;
              dxl = fac*20;
              kgTextSize ( fid , dyl*0.55 , dxl , 0. ) ;
              yy = yl - dyl*0.7;
              for ( i = 0;i < k;i++ ) {
                  length = kgStringLength ( fid , str [ i ] ) ;
                  kgMove2f ( fid , ( xl-length ) *0.5 , yy ) ;
                  kgWriteText ( fid , str [ i ] ) ;
                  yy -= dyl;
              }
          }
          Img = kgGetResizedImage ( fid ) ;
          if ( Img == NULL ) printf ( "Img==NULL\n" ) ;
          else {
#if 1
//      uiWriteImage(Img,"Junk.png");
              kgCloseImage ( fid ) ;
              if ( msg->message != NULL ) {
                  if ( p0->xpm != NULL ) {
                      kgImage ( D , p0->xpm , 0 , 0 , xl , yl , 0.0 , 1.0 ) ;
                      kgImage ( D , Img , 0 , 0 , xl , yl , 0.0 , 1.0 ) ;
                  }
                  else {
                      void *img1;
                      int bkcolor;
                      bkcolor = msg->bkcolor;
                      img1 = kgBorderedRectangle ( xl-3 , yl-3 , bkcolor , 0.05 ) ;
                      kgMergeImages ( img1 , Img , 0 , 0 ) ;
                      kgAddTransparency ( img1 , 0.5 ) ;
                      kgImage ( D , img1 , 0 , 0 , xl , yl , 0.0 , 1.0 ) ;
                      kgFreeImage ( img1 ) ;
                  }
                  uiFreeImage ( Img ) ;
              }
#endif
//      printf("kgUpdateOn\n");
              kgUpdateOn ( D ) ;
          }
      }
      else {
          if ( p0->xpm != NULL ) kgImage ( D , p0->xpm , 0 , 0 , xl , yl , 0.0 , 1.0 ) ;
          kgUpdateOn ( D ) ;
      }
      while ( 1 ) {
          if ( ! kgCheckEvent ( D , & kbe ) ) {kgThreadSleep ( 1 , 0 ) ;count++;}
          if ( count > 20 ) break;
          if ( kbe.event == 1 ) {
              if ( kbe.button == 1 ) {
                  ret = 1;
                  break;
              }
          }
      }
#endif
      return ret;
  }
  int kgSplashDiaCallBack ( void *Tmp , void *tmp ) {
      int ret = 0;
      DIALOG *D;
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
              ret = 1;
          }
      }
      return ret;
  }
  int kgSplashDia ( int xo , int yo , int xl , \
       int yl , char *xpm , char *message , \
  int font , int fontcolor , unsigned long bkcolor ) \
  {
      int ret = 1;
      DIALOG D;
      DIA d [ 2 ] ;
      DIP p0 = {
          'p' , 1 , 1 , 686 , 225 , NULL , // Pixmap info
          -1 , 0 , 0 , 0.005 /* bkgr colour */
      };
      SPLASHMSG msg;
      msg.message = message;
      msg.font = font;
      msg.fontcolor = fontcolor;
      msg.bkcolor = bkcolor;
      d [ 0 ] .p = & p0;
      d [ 1 ] .t = NULL;
      D.d = d;
      D.VerId = 1401010000;
      kgInitUi ( & D ) ;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 0;
      D.df = 0;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = 267; /* Position of Dialog */
      D.yo = 204;
      D.xl = xl; /* Length of Dialog */
      D.yl = yl; /* Width of Dialog */
      D.Initfun = kgSplashDiainit; /* init fuction for Dialog */
      D.kbattn = 1; /* 1 for drawing keyborad attention */
      D.butattn = 1; /* 1 for drawing button attention */
      D.fullscreen = 0; /* 1 for for fullscreen mode */
      D.Deco = 0; /* 1 for Window Decorration */
      D.transparency = 1.0; /* float 1.0 for full transparency */
      D.Newwin = 1; /* 1 for new window not yet implemented */
      D.DrawBkgr = 0; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 1; /* 1 for stickyness */
#if 0 
      D.Callback = kgSplashDiaCallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.Fixpos = 1; /* 1 for Fixing Position */
      D.NoTaskBar = 1; /* 1 for not showing in task bar*/
      D.parent = NULL; /* 1 for not showing in task bar*/
      strcpy ( D.name , "Kulina Designer ver 1.0" ) ;
           /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
          if ( ( xo < 0 ) || ( yo < 0 ) ) {
              D.xo = ( xres-xl ) /2;
              D.yo = ( yres-yl ) /2;
          }
          else {D.xo = xo;D.yo = yo;}
          p0.x1 = 4; p0.y1 = 4;
          p0.x2 = xl-4; p0.y2 = yl-4;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
      } /* end of fullscreen mode */
      if ( xpm != NULL ) {
          p0.xpm = xpm;
      }
      D.pt = & msg;
      D.StackPos = 1; /* 1 for above */
      D.Shapexpm = NULL;
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
//  printf("Splashdia over\n");
      return ret;
  }
  int kgSplashMessageinit ( void *Tmp ) {
      int ret = 0 , i , j , OK , xl , yl , count = 0 , \
      k , color , r , g , b , fcolor;
      float ln , length = 0 , fac = 1.0;
      float w_x1 = 0.0 , w_y1 = 0.0 , w_x2 = 686 , w_y2 = 226 , dyl , dxl , yy;
      float h , s , v , Vb , rf , gf , bf;
      void *fid , *Img;
      float rfac = 0.15 , xo , yo , l , w;
      KBEVENT kbe;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      char *pt , buf [ 5000 ] , flname [ 200 ] , *str [ 500 ] ;
      DIP *p0 ;
      SPLASHMSG *msg;
      p0 = ( D->d [ 0 ] ) .p;
      xl = D->xl;
      yl = D->yl;
      w_x2 = xl;
      w_y2 = yl;
      l = xl;
      w = yl;
//  kgWindowStateAbove(Tmp);
#if 1
      msg = D->pt;
      if ( msg->message != NULL ) {
//    fid = kgInitImage((int)(w_x2-w_x1)+10,(int)(w_y2-w_y1)+10,4);
//    kgUserFrame(fid,w_x1-5,w_y1-5,w_x2+5,w_y2+5);
          fid = kgInitImage ( ( int ) ( w_x2-w_x1 ) , ( int ) ( w_y2-w_y1 ) , RESIZE ) ;
          kgUserFrame ( fid , w_x1-3 , w_y1-3 , w_x2+3 , w_y2+3 ) ;
          color = msg->bkcolor;
          if ( color == -1 ) color = D->gc.fill_clr;
          if ( color < -1 ) {
              color = -color;
              r = color/1000000;
              g = ( color/1000 ) %1000;
              b = ( color%1000 ) ;
              kgChangeColor ( fid , 129 , r , g , b ) ;
              color = 129;
          }
          fcolor = msg->fontcolor;
          if ( fcolor == -1 ) fcolor = D->gc.fill_clr;
          if ( fcolor < -1 ) {
              fcolor = -fcolor;
              r = fcolor/1000000;
              g = ( fcolor/1000 ) %1000;
              b = ( fcolor%1000 ) ;
              kgChangeColor ( fid , 130 , r , g , b ) ;
              fcolor = 130;
          }
          kgGetRGB ( ( DIG * ) fid , color , & r , & g , & b ) ;
          rf = r;gf = g;bf = b;
          RGBtoHSV ( rf , gf , bf , & h , & s , & v ) ;
          Vb = 1.5*v;
          if ( Vb > 1. ) Vb = 1.;
          HSVtoRGB ( & rf , & gf , & bf , h , s , Vb ) ;
          kgChangeColor ( fid , 152 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
          HSVtoRGB ( & rf , & gf , & bf , h , s , 0.5*v ) ;
          kgChangeColor ( fid , 153 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
          kgChangeColor ( fid , 151 , ( int ) r , ( int ) g , ( int ) b ) ;
          kgTextColor ( fid , fcolor ) ;
          kgTextFont ( fid , msg->font ) ;
          kgTextSize ( fid , 30. , 20. , 0. ) ;
          xo = ( w_x1+w_x2 ) *.5;
          yo = ( w_y1+w_y2 ) *.5;
          kgRoundedRectangleFill ( fid , xo , yo , ( float ) l+2 , \
           ( float ) w+2 , 0 , 153 , rfac ) ;
          kgRoundedRectangleFill ( fid , xo , yo , ( float ) l+1 , \
           ( float ) w+1 , 0 , 152 , rfac ) ;
          kgRoundedRectangleFill ( fid , xo , yo , ( float ) l-4 , \
           ( float ) w-4 , 0 , 151 , rfac ) ;
//    kgRoundedRectangleFill(fid,(w_x1+w_x2)*.5+3,(w_y1+w_y2)*.5-2,w_x2-6,w_y2-6,0,0,0.1);
//    kgRoundedRectangleFill(fid,(w_x1+w_x2)*.5,(w_y1+w_y2)*.5,w_x2-6,w_y2-6,0,color,0.1);
          strcpy ( buf , ( char * ) msg->message ) ;
          pt = buf;
          j = 1;
          k = 0;
          while ( 1 ) {
              i = 0;
              if ( pt [ i ] == '\0' ) break;
//      gphMove2f(fid,40.,yl-j*45.);
              j++;
              OK = 1;
              while ( pt [ i ] != '\0' ) {
                  OK = 0;
                  if ( ( pt [ i ] == '\n' ) ) { pt [ i ] = '\0';break;}
                  if ( ( ( pt [ i ] == '\\' ) && ( pt [ i+1 ] == 'n' ) ) ) \
                  { pt [ i ] = '\0';i++;break;}
                  i++;
                  OK = 1;
              }
              str [ k++ ] = pt;
              ln = kgStringLength ( fid , pt ) ;
              if ( length < ln ) length = ln;
//      gphWriteText(fid,pt);
              if ( OK ) break;
              pt = pt+i+1;
          }
          if ( k > 0 ) {
              dyl = ( float ) ( yl-6 ) /k;
              dxl = 0.75*dyl;
              fac = ( float ) ( xl-10 ) / ( length ) ;
              if ( dxl > fac*20 ) dxl = fac*20;
              kgTextSize ( fid , 0.6*dyl , dxl , 0. ) ;
              yy = yl - dyl*0.7-3;
              for ( i = 0;i < k;i++ ) {
                  length = kgStringLength ( fid , str [ i ] ) ;
                  kgMove2f ( fid , ( xl-length ) *0.5 , yy ) ;
                  kgWriteText ( fid , str [ i ] ) ;
                  yy -= dyl;
              }
          }
          Img = kgGetSharpImage ( fid ) ;
          if ( Img == NULL ) printf ( "Img==NULL\n" ) ;
          else {
#if 1
//      uiWriteImage(Img,"Junk.png");
              kgCloseImage ( fid ) ;
              if ( msg->message != NULL ) {
                  if ( p0->xpm != NULL ) kgImage ( D , p0->xpm , \
                  D->xo , D->yo , xl , yl , 0.0 , 1.0 ) ;
                  kgImage ( D , Img , D->xo , D->yo , xl , yl , 0.0 , 1.0 ) ;
                  uiFreeImage ( Img ) ;
              }
#endif
//      printf("kgUpdateOn\n");
              kgUpdateOn ( D ) ;
          }
      }
      while ( 1 ) {
          if ( ! kgCheckEvent ( D , & kbe ) ) {kgThreadSleep ( 1 , 0 ) ;count++;}
          if ( count > 1 ) break;
          if ( kbe.event == 1 ) {
              if ( kbe.button == 1 ) {
                  ret = 1;
                  break;
              }
          }
      }
#endif
      return ret;
  }
  int kgSplashMessageCallBack ( void *Tmp , void *tmp ) {
      int ret = 0;
      DIALOG *D;
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
              ret = 1;
          }
      }
      return ret;
  }
  int kgSplashMessage ( void *Parent , int xo , int yo , int xl , int yl , \
  char *message , int font , int fontcolor , int bkcolor ) \
  {
      int ret = 1;
      DIALOG D;
      DIA d [ 2 ] ;
      DIP p0 = {
          'p' , 1 , 1 , 686 , 225 , NULL , // Pixmap info
          -1 , 0 , 0 /* bkgr colour */
      };
      SPLASHMSG msg;
      msg.message = message;
      msg.font = font;
      msg.fontcolor = fontcolor;
      msg.bkcolor = bkcolor;
      d [ 0 ] .p = & p0;
      d [ 1 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 0;
      D.df = 0;
      D.tw = 0;
      D.bw = 0;
      D.lw = 0;
      D.rw = 0;
      D.xo = xo;
      D.yo = yo;
      D.xl = xl; /* Length of Dialog */
      D.yl = yl; /* Width of Dialog */
      D.Initfun = kgSplashMessageinit; /* init fuction for Dialog */
      D.kbattn = 0; /* 1 for drawing keyborad attention */
      D.butattn = 0; /* 1 for drawing button attention */
      D.fullscreen = 0; /* 1 for for fullscreen mode */
      D.Deco = 0; /* 1 for Window Decorration */
      D.transparency = 1.0; /* float 1.0 for full transparency */
      D.Newwin = 0; /* 1 for new window not yet implemented */
      D.DrawBkgr = 0; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 1; /* 1 for stickyness */
#if 0 
      D.Callback = kgSplashMessageCallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.Fixpos = 1; /* 1 for Fixing Position */
      D.NoTaskBar = 1; /* 1 for not showing in task bar*/
      D.parent = Parent; /* 1 for not showing in task bar*/
      strcpy ( D.name , "Kulina Designer ver 1.0" ) ;
           /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
          p0.x1 = 0; p0.y1 = 0;
          p0.x2 = xl; p0.y2 = yl;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
      } /* end of fullscreen mode */
      p0.xpm = NULL;
      D.pt = & msg;
      D.StackPos = 1; /* 1 for above */
      D.Shapexpm = NULL;
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      return ret;
  }
  void *RunkgSplashDia ( void *arg ) {
/*************************************************


*************************************************/
      SPLASHMSG *pt;
      pt = ( SPLASHMSG * ) arg;
      kgSplashDia ( -1 , -1 , pt->xsize , pt->ysize , pt->xpm , pt->message , \
      pt->font , pt->fontcolor , pt->bkcolor ) ;
      return NULL;
  }
  void kgInitDialog ( DIALOG *D , int xo , int yo , char **menu , int size ) {
      float length;
      int ret = 1 , i , lngth = 50 , ln;
      i = 0;
      while ( menu [ i ] != NULL ) {
          length = uiStringLength ( menu [ i ] , TextSize ) ;
          ln = ( int ) ( length ) +10;
//    ln = strlen(menu[i])*9+15;
          if ( ln > lngth ) lngth = ln;
          i++;
      }
      D->d = NULL;
      D->bkup = 1; /* set to 1 for backup */
      D->bor_type = 4;
      D->df = 0;
      D->xo = xo; /* Position of Dialog */
      D->yo = yo;
      D->xl = lngth+2; /* Length of Dialog */
      D->yl = ( size+1 ) *20+0; /* Width of Dialog */
      D->fullscreen = 0; /* 1 for for fullscreen mode */
      D->Deco = 0; /* 1 for Window Decorration */
      D->transparency = 000000; /* float 1.0 for full transparency */
      D->Newwin = 1; /* 1 for new window not yet implemented */
      D->DrawBkgr = 1; /* 1 for drawing background */
      D->Bkpixmap = NULL; /* background image */
      D->Sticky = 0; /* 1 for stickyness */
      D->Callback = NULL;
      D->Fixpos = 1; /* 1 for Fixing Position */
      D->NoTaskBar = 1; /* 1 for not showing in task bar*/
      D->StackPos = 1;
      D->parent = NULL;
      D->pt = NULL; /* any data to be passed by user*/
      D->Shapexpm = NULL;
      strcpy ( D->name , "Kulina Designer ver 1.0" ) ;
           /* Dialog name you may change */
      kgDefaultGuiTheme ( & ( D->gc ) ) ;
      uiDialogWindow ( D ) ;
      D->evgay = WC ( D )->EVGAY;
      D->evgax = WC ( D )->EVGAX;
      return ;
  }
  int kgMenu ( void *parent , int xx1 , int yy1 , int pos , \
  int df , char **menu , int size ) \
  {
      char **x;
      int NOK = 1 , item , ok , tempc;
      float fac = 1.;
      BRW_STR *br;
      KBEVENT kbevent;
      DIALOG *D = NULL , Dtmp;
      kgWC *wc;
      if ( parent != NULL ) D = ( DIALOG * ) ( parent ) ;
      else {
          kgInitDialog ( & Dtmp , xx1 , yy1 , menu , size ) ;
          D = & Dtmp;
          xx1 = -8 , yy1 = 2;
      }
      wc = WC ( D ) ;
      tempc = wc->c_color;
      uiUpdateOff ( D ) ;
      br = ( BRW_STR * ) make_menu_str ( D , xx1 , yy1 , pos , df , menu , size ) ;
      item = br->df;
      x = br->menu;
      if ( br->menu [ 0 ] == NULL ) return 1;
      NOK = 1;
      uiUpdateOn ( D ) ;
      while ( NOK != 0 ) {
          kbevent = kgSkipMouseMove ( D ) ;
//    set_pointer_position(kbevent.x,kbevent.y,kbevent.button);
          D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
          switch ( kbevent.event ) {
              case 0:
              _ui_scroll_item_hilight ( br , kbevent ) ;
              continue;
              case 1:
              ok = _ui_process_scroll_click ( br , kbevent ) ;
              if ( ok >= 0 ) {
                  if ( ok == 0 ) br->df = item;
                  NOK = 0;
                  kgWaitButtonRelease ( D ) ;
              }
              continue;
              case 2:
              continue;
              case 3:
              _ui_process_scroll_move ( br , kbevent ) ;
              continue;
              case 4:
              continue;
              case 5:
              ok = _ui_process_scroll_key ( br , kbevent ) ;
              if ( ok >= 0 ) {
                  if ( ok == 0 ) br->df = item;
                  NOK = 0;
                  kgWaitButtonRelease ( D ) ;
              }
              continue;
          }
      }
//  char_hilit_on();
      if ( ok > 0 ) item = br->df;
      else item = 0;
//  clear_menu();
      kg_scr_recover ( D->wc ) ;
      free ( br ) ;
      wcset_clr ( wc , tempc ) ;
      kgSkipEvents ( D ) ;
      if ( parent == NULL ) {
          pthread_cancel ( WC ( D )->Pth ) ;
          Dempty ( WC ( D )->Clip ) ;
          kg_clear_scrn_buffer ( WC ( D ) ) ;
//     Dempty(WC(D)->SBlist);
          Dempty ( WC ( D )->TLIST ) ;
          free ( WC ( D )->kgcolors ) ;
          kgCloseUi ( D ) ;
          free ( WC ( D ) ) ;
      }
      return ( item ) ;
  }
  int kgFontListDiabrowser1callback ( int key , int i , void *Tmp ) {
      DIA *D;DIE *B;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .e;
      switch ( key ) {
          case 1:
          break;
      }
      return ret;
  }
  int kgFontListDiainit ( void *Tmp ) {
      int ret = 1;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      return ret;
  }
  int kgFontListDiaCallBack ( void *Tmp , void *tmp ) {
      int ret = 0;
      DIALOG *D;
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
          }
      }
      return ret;
  }
  int kgFontListDia ( void *parent , void *v0 , int xo , int yo ) {
      int ret = 1;
      DIALOG D;
      DIA d [ 2 ] ;
      char *menu0 [ ] = {
       ( char * ) "" , NULL };
      DIE e0 = {
          'e' , 3 , 4 , 407 , 380 , 16 , ( int * ) v0 , NULL , menu0 , NULL , \
          kgFontListDiabrowser1callback , /* *args , callback */
      20 , 6 , 22 , 1 , 1 , 1 , 0 };
      e0.menu = kgFontNames ( ) ;
      d [ 0 ] .e = & e0;
      d [ 1 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 4;
      D.df = 0;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = xo; /* Position of Dialog */
      D.yo = yo;
      D.xl = 411; /* Length of Dialog */
      D.yl = 383; /* Width of Dialog */
      D.Initfun = kgFontListDiainit; /* init fuction for Dialog */
      D.kbattn = 1;; /* 1 for drawing keyborad attention */
      D.butattn = 1; /* 1 for drawing button attention */
      D.fullscreen = 0; /* 1 for for fullscreen mode */
      D.Deco = 1; /* 1 for Window Decorration */
      D.transparency = .000000; /* float 1.0 for full transparency */
      D.Newwin = 0; /* 1 for new window not yet implemented */
      D.DrawBkgr = 1; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 0; /* 1 for stickyness */
#if 0 
      D.Callback = kgFontListDiaCallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.Fixpos = 1; /* 1 for Fixing Position */
      D.NoTaskBar = 0; /* 1 for not showing in task bar*/
      D.StackPos = 1; /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/
      D.Shapexpm = NULL; /* PNG/jpeg file for window shape;
      Black color will not be drawn */
      D.parent = parent; /* 1 for not showing in task bar*/
      D.pt = NULL; /* any data to be passed by user*/
      strcpy ( D.name , "Kulina Font List" ) ; /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
      } /* end of fullscreen mode */
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      kgFreeFontNames ( e0.menu ) ;
      return ret;
  }
  int kgGetFont ( void *arg , int xo , int yo ) {
/*************************************************

    Scrollmenu1  1 data value

*************************************************/
      int v0 = 1;
      kgFontListDia ( arg , & v0 , xo , yo ) ;
      return v0-1;
  }
  int kgColorsDia ( void *parent , void *v0 , int xo , int yo ) {
      int ret = 1;
      DIALOG D;
      DIA d [ 2 ] ;
      char *titles0 [ ] = {
           ( char * ) "0" , ( char * ) "1" , ( char * ) "2" , \
           ( char * ) "3" , ( char * ) "4" , \
           ( char * ) "5" , ( char * ) "6" , ( char * ) "7" , \
           ( char * ) "8" , ( char * ) "9" , \
           ( char * ) "10" , ( char * ) "11" , ( char * ) "12" , ( char * ) "13" , \
           ( char * ) "14" , ( char * ) "15" , ( char * ) "16" , ( char * ) "17" , \
           ( char * ) "18" , ( char * ) "19" , ( char * ) "20" , ( char * ) "21" , \
           ( char * ) "22" , ( char * ) "23" , ( char * ) "24" , ( char * ) "25" , \
           ( char * ) "26" , ( char * ) "27" , ( char * ) "28" , ( char * ) "29" , \
           ( char * ) "30" , ( char * ) "31" , ( char * ) "32" , ( char * ) "33" , \
           ( char * ) "34" , ( char * ) "35" , ( char * ) "36" , ( char * ) "37" , \
           ( char * ) "38" , ( char * ) "39" , ( char * ) "40" , ( char * ) "41" , \
           ( char * ) "42" , ( char * ) "43" , ( char * ) "44" , ( char * ) "45" , \
           ( char * ) "46" , ( char * ) "47" , ( char * ) "48" , ( char * ) "49" , \
           ( char * ) "50" , ( char * ) "51" , ( char * ) "52" , ( char * ) "53" , \
           ( char * ) "54" , ( char * ) "55" , ( char * ) "56" , ( char * ) "57" , \
           ( char * ) "58" , ( char * ) "59" , ( char * ) "60" , ( char * ) "61" , \
       ( char * ) "62" , ( char * ) "63" , NULL };
      void *xpm0 [ ] = {
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
      NULL , NULL , NULL , NULL , NULL , NULL };
      int bkgr0 [ ] = {
          0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , \
          17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , \
          32 , 33 , 34 , 35 , 36 , 37 , 38 , 39 , 40 , 41 , 42 , 43 , 44 , 45 , 46 , \
          47 , 48 , 49 , 50 , 51 , 52 , 53 , 54 , 55 , 56 , \
      57 , 58 , 59 , 60 , 61 , 62 , 63 , -1 };
      char *butncode0 = NULL;
      int sw0 [ ] = {1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , \
          1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , \
          1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , \
      1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1};
      DIB n0 = {
          'b' , 6 , 4 , 339 , 331 , 0 , 0 , 40 , 40 , 8 , 8 , ( int * ) v0 , sw0 , \
          titles0 , butncode0 , NULL , NULL , xpm0 , bkgr0 , \
               /* pointers to xpms and colors */
          1 , 0.200000 /* button type and roundinfg factor ( 0-0.5 ) */
      };
      d [ 0 ] .b = & n0;
      d [ 1 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 4;
      D.df = 0;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = xo; /* Position of Dialog */
      D.yo = yo;
      D.xl = 341; /* Length of Dialog */
      D.yl = 337; /* Width of Dialog */
      D.Initfun = NULL;
      D.kbattn = 0;; /* 1 for drawing keyborad attention */
      D.butattn = 1; /* 1 for drawing button attention */
      D.fullscreen = 0; /* 1 for for fullscreen mode */
      D.Deco = 1; /* 1 for Window Decorration */
      D.transparency = 0.000000; /* float 1.0 for full transparency */
      D.Newwin = 0; /* 1 for new window not yet implemented */
      D.DrawBkgr = 1; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 0; /* 1 for stickyness */
#if 0 
      D.Callback = kgColorsDiaCallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.Fixpos = 1; /* 1 for Fixing Position */
      D.NoTaskBar = 0; /* 1 for not showing in task bar*/
      D.StackPos = 1; /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/
      D.Shapexpm = NULL; /* PNG/jpeg file for window shape;
      Black color will not be drawn */
      D.parent = parent; /* 1 for not showing in task bar*/
      D.pt = NULL; /* any data to be passed by user*/
      strcpy ( D.name , "Kulina Designer ver 1.0" ) ;
           /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
      } /* end of fullscreen mode */
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
//  kgGrayGuiTheme(&(D.gc));    /*  set colors for gui*/
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      D.SearchList = NULL;
      ret = kgUi ( & D ) ;
      return ret;
  }
  int kgGetColor_o ( void *parent , int xo , \
  int yo , int *r , int *g , int *b ) \
  {
/*************************************************

    Buttonbox1 (new) 1 data value

*************************************************/
      int v0 = 1;
      kgColorsDia ( parent , & v0 , xo , yo ) ;
      kgGetDefaultRGB ( v0-1 , r , g , b ) ;
      return v0-1;
  }
  void uiDrawColorInPixmap_o ( void *Tmp ) {
      int r , g , b , bkgr = 255;
      int fid;
      int *ioldclr;
      int xo = 385 , yo = 185 , xsize = 48 , ysize = 48;
      DIALOG *D;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      D = ( DIALOG * ) Tmp;
      ioldclr = ( int * ) ( D->pt ) ;
      r = uiGetDslide ( Tmp , 1 ) ;
      g = uiGetDslide ( Tmp , 2 ) ;
      b = uiGetDslide ( Tmp , 3 ) ;
      if ( ( abs ( r-ioldclr [ 3 ] ) +abs ( g-ioldclr [ 4 ] ) +abs \
       ( b-ioldclr [ 5 ] ) ) < 5 ) return;
      ioldclr [ 3 ] = r; ioldclr [ 4 ] = g; ioldclr [ 5 ] = b;
      tmpdir = kgMakeTmpDir ( ) ;
      sprintf ( flname , "%-s/Tmp.gph" , tmpdir ) ;
      fid = open ( flname , O_CREAT | O_TRUNC | O_RDWR , 0700 ) ;
      if ( fid >= 0 ) {
          gphUserFrame ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize ) ;
          gphChangeColor ( fid , bkgr , r , g , b ) ;
          gphBoxFill ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize , bkgr , 0 ) ;
          close ( fid ) ;
          img = kgGphtoAntialiasedImage ( flname , xsize , ysize , 0x00000000 , 1 ) ;
          kgImage ( Tmp , img , xo , yo , xsize , ysize , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
          close ( fid ) ;
      }
      kgCleanDir ( tmpdir ) ;
      free ( tmpdir ) ;
  }
  void uiDrawColorInPixmap1_o ( void *Tmp ) {
      int r , g , b , bkgr = 255;
      int fid;
      int *ioldclr;
      int xo = 320 , yo = 185 , xsize = 48 , ysize = 48;
      DIALOG *D;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      D = ( DIALOG * ) Tmp;
      ioldclr = ( int * ) ( D->pt ) ;
      r = ioldclr [ 0 ] ;
      g = ioldclr [ 1 ] ;
      b = ioldclr [ 2 ] ;
      tmpdir = kgMakeTmpDir ( ) ;
      sprintf ( flname , "%-s/Tmp.gph" , tmpdir ) ;
      fid = open ( flname , O_CREAT | O_TRUNC | O_RDWR , 0700 ) ;
      if ( fid >= 0 ) {
          gphUserFrame ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize ) ;
          gphChangeColor ( fid , bkgr , r , g , b ) ;
          gphBoxFill ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize , bkgr , 0 ) ;
          close ( fid ) ;
          img = kgGphtoAntialiasedImage ( flname , xsize , ysize , 0x00000000 , 1 ) ;
          kgImage ( Tmp , img , xo , yo , xsize , ysize , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
          close ( fid ) ;
      }
      kgCleanDir ( tmpdir ) ;
      free ( tmpdir ) ;
  }
  void uiDrawColorInPixmap ( void *Tmp ) {
      int r , g , b , bkgr = 255;
      void * fid;
      int *ioldclr;
      int xo = 385 , yo = 185 , xsize = 48 , ysize = 48;
      DIALOG *D;
      DIP *p5;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      D = ( DIALOG * ) Tmp;
      ioldclr = ( int * ) ( D->pt ) ;
      r = uiGetDslide ( Tmp , 1 ) ;
      g = uiGetDslide ( Tmp , 2 ) ;
      b = uiGetDslide ( Tmp , 3 ) ;
      if ( ( abs ( r-ioldclr [ 3 ] ) +abs ( g-ioldclr [ 4 ] ) +abs \
       ( b-ioldclr [ 5 ] ) ) < 5 ) return;
      ioldclr [ 3 ] = r; ioldclr [ 4 ] = g; ioldclr [ 5 ] = b;
      p5 = ( DIP * ) uiGetWidget ( Tmp , 5 ) ;
      uiFreeImage ( p5->xpm ) ;
      fid = kgInitImage ( xsize , ysize , 1 ) ;
      if ( fid != NULL ) {
          kgUserFrame ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize ) ;
          kgChangeColor ( fid , bkgr , r , g , b ) ;
          kgBoxFill ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize , bkgr , 0 ) ;
          img = kgGetResizedImage ( fid ) ;
          kgCloseImage ( fid ) ;
#if 0
          kgImage ( Tmp , img , xo , yo , xsize , ysize , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
#else
          p5->xpm = img;
          uiUpdateWidget ( ( void * ) p5 ) ;
#endif
      }
//  kgUpdateOn(Tmp);
  }
  void uiDrawColorInPixmap1 ( void *Tmp ) {
      int r , g , b , bkgr = 255;
      void * fid;
      int *ioldclr;
      int xo = 320 , yo = 185 , xsize = 48 , ysize = 48;
      DIALOG *D;
      DIP *p4;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      D = ( DIALOG * ) Tmp;
      ioldclr = ( int * ) ( D->pt ) ;
      r = ioldclr [ 0 ] ;
      g = ioldclr [ 1 ] ;
      b = ioldclr [ 2 ] ;
      fid = kgInitImage ( xsize , ysize , 1 ) ;
      p4 = ( DIP * ) uiGetWidget ( Tmp , 4 ) ;
      uiFreeImage ( p4->xpm ) ;
      if ( fid != NULL ) {
          kgUserFrame ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize ) ;
          kgChangeColor ( fid , bkgr , r , g , b ) ;
          kgBoxFill ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize , bkgr , 0 ) ;
          img = kgGetResizedImage ( fid ) ;
          kgCloseImage ( fid ) ;
#if 0
          kgImage ( Tmp , img , xo , yo , xsize , ysize , 0.0 , 1.0 ) ;
          uiFreeImage ( img ) ;
#else
          p4->xpm = img;
          uiUpdateWidget ( ( void * ) p4 ) ;
#endif
      }
//  kgUpdateOn(Tmp);
  }
  int GetColorbutnbox1callback ( int key , int i , void *Tmp ) {
      DIA *D;DIB *B;
      int n , ret = 0 , r , g , b;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .b;
      n = B->nx*B->ny;
      kgGetDefaultRGB ( key-1 , & r , & g , & b ) ;
      uiSetDslide ( Tmp , 1 , r ) ;
      uiSetDslide ( Tmp , 2 , g ) ;
      uiSetDslide ( Tmp , 3 , b ) ;
      uiDrawColorInPixmap ( Tmp ) ;
      return ret;
  }
  int GetColordslide1callback ( int val , int i , void *Tmp ) {
      DIA *D;DID *SD;
      int ret = 1 , r;
      D = ( ( DIALOG * ) Tmp )->d;
      SD = D [ i ] .d;
      uiDrawColorInPixmap ( Tmp ) ;
      return ret;
  }
  int GetColordslide2callback ( int val , int i , void *Tmp ) {
      DIA *D;DID *SD;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      SD = D [ i ] .d;
      uiDrawColorInPixmap ( Tmp ) ;
      return ret;
  }
  int GetColordslide3callback ( int val , int i , void *Tmp ) {
      DIA *D;DID *SD;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      SD = D [ i ] .d;
      uiDrawColorInPixmap ( Tmp ) ;
      return ret;
  }
  int GetColorhoribar1callback ( int key , int i , void *Tmp ) {
      DIA *D;DILN *B;
      int *ioldclr;
      int n , ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .H;
      n = B->nx;
      switch ( key ) {
          case 1:
          ioldclr = ( int * ) ( ( ( DIALOG * ) Tmp )->pt ) ;
          uiSetDslide ( Tmp , 1 , ioldclr [ 0 ] ) ;
          uiSetDslide ( Tmp , 2 , ioldclr [ 1 ] ) ;
          uiSetDslide ( Tmp , 3 , ioldclr [ 2 ] ) ;
          uiDrawColorInPixmap ( Tmp ) ;
          ret = 0;
          break;
          case 2:
          break;
      }
      return ret;
  }
  int GetColorinit ( void *Tmp ) {
      int ret = 1 , color , bkgr = 255;
      unsigned char r , g , b;
      int xo = 320 , yo = 185 , xsize = 48 , ysize = 48;
      int *ioldclr;
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) Tmp;
#if 0
      ioldclr = ( int * ) ( D->pt ) ;
      r = ioldclr [ 0 ] ;
      g = ioldclr [ 1 ] ;
      b = ioldclr [ 2 ] ;
      wc = D->wc;
      uichng_clr ( Tmp , bkgr , r , g , b ) ;
      color = wc->c_color;
      wcset_clr ( wc , bkgr ) ;
      _uiBox_Fill ( wc , xo , yo , xo+xsize , yo+ysize ) ;
      wcset_clr ( wc , color ) ;
#else
      uiDrawColorInPixmap1 ( Tmp ) ;
#endif
      uiDrawColorInPixmap ( Tmp ) ;
      return ret;
  }
  int GetColorCallBack ( void *Tmp , void *tmp ) {
      int ret = 0;
      DIALOG *D;
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
          }
      }
      return ret;
  }
  int _uiGetColor ( void *parent , int xo , int yo , \
  void *v1 , void *v2 , void *v3 ) \
  {
      int ret = 1;
      int v0 = 1;
      int ioldclr [ 6 ] ;
      DIALOG D;
      DIA d [ 13 ] ;
      char *titles0 [ ] = {
           ( char * ) "" , ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , ( char * ) "" , \
           ( char * ) "" , ( char * ) "" , \
       ( char * ) "" , ( char * ) "" , ( char * ) "" , ( char * ) "" , NULL };
      void *xpm0 [ ] = {
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL ,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
      int bkgr0 [ ] = {
          0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , \
          17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , \
          32 , 33 , 34 , 35 , 36 , 37 , 38 , 39 , 40 , 41 , 42 , 43 , 44 , 45 , 46 , \
          47 , 48 , 49 , 50 , 51 , 52 , 53 , 54 , 55 , 56 , \
      57 , 58 , 59 , 60 , 61 , 62 , 63 , -1 };
      char *butncode0 = NULL;
      int sw0 [ ] = {1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , \
          1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , \
          1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , \
          1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1,1,1,1,1};
      DIB n0 = {
          'b' , 23 , 16 , 290 , 279 , 0 , 0 , 32 , 32 , 8 , 8 , ( int * ) & v0 , sw0 , \
          titles0 , butncode0 , NULL , GetColorbutnbox1callback , /* args , Callbak */
          xpm0 , bkgr0 , /* pointers to xpms and colors */
          1 , 0.000000 /* button type and roundinfg factor ( 0-0.5 ) */
      };
      DID d1 = {
          'd' , 335 , 20 , 435 , 65 , 0 , 255 , 52 , ( int * ) v1 , ( char * ) "" , \
          NULL , GetColordslide1callback /* *args , callback */
      };
      DID d2 = {
          'd' , 335 , 66 , 435 , 112 , 0 , 255 , 52 , ( int * ) v2 , ( char * ) "" , \
          NULL , GetColordslide2callback /* *args , callback */
      };
      DID d3 = {
          'd' , 335 , 113 , 435 , 158 , 0 , 255 , 52 , ( int * ) v3 , ( char * ) "" , \
          NULL , GetColordslide3callback /* *args , callback */
      };
      DIP p4 = {
          'p' , 320 , 185 , 370 , 235 , NULL , // Pixmap info
          -1 , 0 , 0 /* bkgr colour */
      };
      DIP p5 = {
          'p' , 385 , 185 , 435 , 235 , NULL , // Pixmap info
          -1 , 0 , 0 /* bkgr colour */
      };
      DIM m6 = {
      'm' , 299 , 39 , 335 , 61 , 1 , 0 };
      strcpy ( m6.msg , ( char * ) "!c03Red" ) ;
      DIM m7 = {
      'm' , 294 , 84 , 335 , 107 , 1 , 0 };
      strcpy ( m7.msg , ( char * ) "!c02Green" ) ;
      DIM m8 = {
      'm' , 294 , 128 , 335 , 152 , 1 , 0 };
      strcpy ( m8.msg , ( char * ) "!c01Blue" ) ;
      int butn1 = 1;
      int sw9 [ 2 ] = {
      1 , 1 };
      char *titles9 [ ] = {
       ( char * ) "Reset" , ( char * ) "Okay" , NULL };
      void *xpm9 [ ] = {
          NULL , /* pixmap info */
          NULL , /* pixmap info */
      NULL , NULL , NULL , NULL , NULL };
      int bkgr9 [ ] = {
      -1 , -1 , -1 };
      char *butncode9 = NULL;
      DILN H9 = {
          'H' , 309 , 241 , 439 , 272 , 4 , 0 , 60 , 25 , 2 , 1 , & butn1 , sw9 , \
          titles9 , butncode9 , NULL , GetColorhoribar1callback , /* arg , Callbak */
          xpm9 , bkgr9 , /* pointers to xpms and colors */
          6 , 0.250000 /* button type and roundinfg factor ( 0-0.5 ) */
       , 0 };
      DIM m10 = {
      'm' , 331 , 164 , 363 , 188 , 1 , 0 };
      strcpy ( m10.msg , ( char * ) "Old" ) ;
      DIM m11 = {
      'm' , 396 , 164 , 428 , 188 , 1 , 0 };
      strcpy ( m11.msg , ( char * ) "New" ) ;
      ioldclr [ 0 ] = * ( ( int* ) v1 ) ;
      ioldclr [ 1 ] = * ( ( int * ) v2 ) ;
      ioldclr [ 2 ] = * ( ( int * ) v3 ) ;
      ioldclr [ 3 ] = -5;
      ioldclr [ 4 ] = -5;
      ioldclr [ 5 ] = -5;
      d [ 0 ] .b = & n0;
      d [ 1 ] .d = & d1;
      d [ 2 ] .d = & d2;
      d [ 3 ] .d = & d3;
      d [ 4 ] .p = & p4;
      d [ 5 ] .p = & p5;
      d [ 6 ] .m = & m6;
      d [ 7 ] .m = & m7;
      d [ 8 ] .m = & m8;
      d [ 9 ] .H = & H9;
      d [ 10 ] .m = & m10;
      d [ 11 ] .m = & m11;
      d [ 12 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 4;
      D.df = 5;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = xo;
      D.yo = yo;
      D.xl = 455; /* Length of Dialog */
      D.yl = 297; /* Width of Dialog */
      D.Initfun = GetColorinit; /* init fuction for Dialog */
      D.kbattn = 0; /* 1 for drawing keyborad attention */
      D.butattn = 0; /* 1 for drawing button attention */
      D.fullscreen = 0; /* 1 for for fullscreen mode */
      D.Deco = 1; /* 1 for Window Decorration */
      D.transparency = 0.000000; /* float 1.0 for full transparency */
      D.Newwin = 0; /* 1 for new window not yet implemented */
      D.DrawBkgr = 1; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 0; /* 1 for stickyness */
#if 0 
      D.Callback = GetColorCallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.Fixpos = 1; /* 1 for Fixing Position */
      D.NoTaskBar = 0; /* 1 for not showing in task bar*/
      D.StackPos = 1; /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/
      D.Shapexpm = NULL; /* PNG/jpeg file for window shape;
      Black color will not be drawn */
      D.parent = parent; /* 1 for not showing in task bar*/
      D.pt = ioldclr;
      strcpy ( D.name , "Kulina Get Color" ) ; /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
      } /* end of fullscreen mode */
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyGetColorGc(&(D.gc));    /*  set colors for gui*/
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      uiFreeImage ( p4.xpm ) ;
      uiFreeImage ( p5.xpm ) ;
      return ret;
  }
  int kgGetColor ( void *parent , int xo , int yo , int *r , int *g , int *b ) {
/*************************************************

    Buttonbox1 (new) 1 data value
    Integerslidebar1  1 data value
    Integerslidebar2  1 data value
    Integerslidebar3  1 data value

*************************************************/
      int v0 = 1;
      int v1 = 255;
      int v2 = 255;
      int v3 = 255;
      v1 = *r , v2 = *g , v3 = *b;
      _uiGetColor ( parent , xo , yo , & v1 , & v2 , & v3 ) ;
      *r = v1;
      *g = v2;
      *b = v3;
      return 1;
  }
//########################
#define MAXFLNG 50
  typedef struct _dirs {
      char CurDir [ 500 ] ;
      char HomeDir [ 500 ] ;
      char **file;
      char **dir;
  } DIRS;
  char ** _uiFileMenu ( char *dir , char *filter ) ;
  void kgTruncateString ( char *m , int size ) {
      int k , ln;
      if ( m != NULL ) {
          ln = strlen ( m ) ;
          if ( ln > size ) {
              m [ ln+1 ] = m [ size-3 ] ;
              m [ ln+2 ] = m [ size-2 ] ;
              m [ ln+3 ] = m [ size-1 ] ;
              m [ ln+4 ] = m [ size ] ;
              m [ size-3 ] = '.';
              m [ size-2 ] = '.';
              m [ size-1 ] = '.';
              m [ size ] = '\0';
          }
      }
      return;
  }
  void kgRestoreString ( char *m , int size ) {
      int k , ln;
      k = 0;
      if ( strlen ( m ) == size ) {
          if ( ( m [ size-3 ] == '.' ) && ( m [ size-2 ] == '.' ) && \
           ( m [ size-1 ] == '.' ) ) {
              m [ size ] = '.';
              ln = strlen ( m ) ;
              m [ size-3 ] = m [ ln+1 ] ;
              m [ size-2 ] = m [ ln+2 ] ;
              m [ size-1 ] = m [ ln+3 ] ;
              m [ size ] = m [ ln+4 ] ;
          }
      }
      return;
  }
  void uiTruncateString ( char **m , int size ) {
      int k , ln;
      k = 0;
      while ( m [ k ] != NULL ) {
          ln = strlen ( m [ k ] ) ;
          if ( ln > size ) {
              m [ k ] [ ln+1 ] = m [ k ] [ size-3 ] ;
              m [ k ] [ ln+2 ] = m [ k ] [ size-2 ] ;
              m [ k ] [ ln+3 ] = m [ k ] [ size-1 ] ;
              m [ k ] [ ln+4 ] = m [ k ] [ size ] ;
              m [ k ] [ size-3 ] = '.';
              m [ k ] [ size-2 ] = '.';
              m [ k ] [ size-1 ] = '.';
              m [ k ] [ size ] = '\0';
          }
          k++;
      }
      return;
  }
  void uiRestoreString ( char *m , int size ) {
      int k , ln;
      k = 0;
      if ( strlen ( m ) == size ) {
          if ( ( m [ size-3 ] == '.' ) && ( m [ size-2 ] == '.' ) && \
           ( m [ size-1 ] == '.' ) ) {
              m [ size ] = '.';
              ln = strlen ( m ) ;
              m [ size-3 ] = m [ ln+1 ] ;
              m [ size-2 ] = m [ ln+2 ] ;
              m [ size-1 ] = m [ ln+3 ] ;
              m [ size ] = m [ ln+4 ] ;
          }
      }
      return;
  }
  void uiResetString ( char *m , int size ) {
      int k , ln;
      k = 0;
      if ( strlen ( m ) > size ) {
          m [ size-3 ] = '.';
          m [ size-2 ] = '.';
          m [ size-1 ] = '.';
          m [ size ] = '\0';
      }
      return;
  }
  static void arrange ( char **m , int n ) {
      int i , j;
      char *pt;
      for ( i = 0;i < n-1;i++ ) {
          for ( j = i+1;j < n;j++ ) {
              if ( strcmp ( m [ i ] , m [ j ] ) > 0 ) {pt = m [ j ] ;
              m [ j ] = m [ i ] ;m [ i ] = pt;}
          }
      }
  }
  char ** _uiFolderMenu ( char *d_name ) {
      DIR *dp;
      char **menu , *item;
      Dlink *L;
      struct dirent *pt = NULL;
      struct stat st;
      int ln , nd = 0 , i , type;
      char buf [ 500 ] ;
      dp = opendir ( d_name ) ;
      if ( dp == NULL ) {
//          printf ( "No such dir:%s\n" , d_name ) ;
          return NULL;
      }
      L = Dopen ( ) ;
      while ( ( pt = readdir ( dp ) ) != NULL ) {
          if ( pt->d_name [ 0 ] == '.' ) continue;
          strcpy ( buf , d_name ) ;
          strcat ( buf , "/" ) ;
          strcat ( buf , pt->d_name ) ;
          stat ( buf , & st ) ;
          if ( S_ISDIR ( st.st_mode ) ) {
//     if(((st.st_mode & S_IFMT)==S_IFDIR)) {
              ln = strlen ( pt->d_name ) ;
              item = ( char * ) Malloc ( ln+5 ) ;
              strcpy ( item , pt->d_name ) ;
              Dadd ( L , item ) ;
//      printf("%s\n",item);
              nd++;
          }
      }
      closedir ( dp ) ;
      menu = ( char ** ) Malloc ( sizeof ( char * ) * ( nd+1 ) ) ;
      Resetlink ( L ) ;
      i = 0;
      while ( ( item = ( char * ) Getrecord ( L ) ) != NULL ) {
          menu [ i ] = item;
//   printf("%s\n",item);
          i++;
      }
      menu [ i ] = NULL;
      arrange ( menu , i ) ;
      Dfree ( L ) ;
      return menu;
  }
  char ** kgFolderMenu ( char *d_name ) {
      return _uiFolderMenu ( d_name ) ;
  }
  Dlink *uiFolderTree(char *Folder) {
     char **Fpt;
     void *pt;
     char *dpt;
     char SubFolder[1000];
     int i;
     Dlink *L, *LD;
     Fpt = _uiFolderMenu(Folder);
     if(Fpt == NULL) return NULL;
     if(Fpt[0]== NULL) {free(Fpt);return NULL;}
//     if(Fpt[0]=='\0') {kgFreeDouble(Fpt); return NULL;}
     if(Fpt[0][0]=='\0') {free(Fpt); return NULL;}
     i=0;
     L = Dopen();
     while(Fpt[i]!= NULL) {
        strcpy(SubFolder,Folder); 
        strcat(SubFolder,"/");
        strcat(SubFolder,Fpt[i]);   
        dpt = (char *)malloc(strlen(SubFolder)+1);
        strcpy(dpt,SubFolder);
//        printf("SubFolder: %s\n",SubFolder);
        Dadd(L,dpt);
        LD = uiFolderTree(SubFolder);
        if(LD != NULL) {        
           Resetlink(LD);          
           while ((pt=Getrecord(LD))!= NULL) {
             Dadd(L,pt);
           }
           Dfree(LD);
        }
        i++;
     }
     return L;    
  }
  char **kgFolderTree(char *Folder) {
     char **ret=NULL;
     int i=0;
     char *pt;
      DIR *dp;
      dp = opendir ( Folder ) ;
      if ( dp == NULL ) {
//        printf ( "No such dir:%s\n" , d_name ) ;
          closedir ( dp ) ;
          return NULL;
      }
     Dlink *L = uiFolderTree(Folder);
     if(L != NULL) {
         ret = (char **)malloc(sizeof(char *)*(Dcount(L)+2));
         ret[0]= (char *)malloc(strlen(Folder)+1);
         strcpy(ret[0],Folder);
         Resetlink(L);         
         i=1;
         while( (pt=(char *)Getrecord(L)) != NULL) {
            ret[i++] = pt;
//            printf("%s\n",pt);
         }
         ret[i]=NULL;
         Dfree(L);
     }
     else {
         ret = (char **)malloc(sizeof(char *)*(2));
         ret[0]= (char *)malloc(strlen(Folder)+1);
         strcpy(ret[0],Folder);
         ret[1]=NULL;
     }
     return ret;
  }
  int FileBrowsertextbox1callback ( int key , int i , void *Tmp ) {
      int k , ln;
      DIRS *dirs;
      DIA *D;DIT *T;T_ELMT *e;
      DIE *E;
      char *filter;
      char **m;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      dirs = ( ( DIALOG * ) Tmp )->pt;
      T = D [ i ] .t;
      e = T->elmt;
      filter = Dgetstring ( Tmp , i , 1 ) ;
      m = _uiFileMenu ( dirs->CurDir , filter ) ;
      dirs->file = m;
      E = ( DIE * ) kgGetWidget ( Tmp , 3 ) ;
      kgFreeDouble ( ( void ** ) ( E->menu ) ) ;
      E->menu = m;
      kgUpdateWidget ( E ) ;
      return ret;
  }
  int FileBrowserbrowser1callback ( int key , int i , void *Tmp ) {
      DIA *D;DIE *B;
      DIE *E;
      DIRS *dirs;
      int ret = 0 , k , ln;
      char **m , **dir , **file , *filter , *folder;
      D = ( ( DIALOG * ) Tmp )->d;
      dirs = ( ( DIALOG * ) Tmp )->pt;
      B = D [ i ] .e;
      if ( strcmp ( dirs->CurDir , "/" ) != 0 ) strcat ( dirs->CurDir , "/" ) ;
      folder = Dgetmenustring ( Tmp , i ) ;
      uiRestoreString ( folder , 35 ) ;
      strcat ( dirs->CurDir , folder ) ;
      uiinfo_wprintf ( ( DIALOG * ) Tmp , ( ( DIALOG * ) Tmp )->d [ 1 ] .i->twin , \
      dirs->CurDir ) ;
      m = _uiFolderMenu ( dirs->CurDir ) ;
      uiTruncateString ( m , 35 ) ;
      dir = m;
      filter = Dgetstring ( Tmp , 0 , 1 ) ;
      m = _uiFileMenu ( dirs->CurDir , filter ) ;
      uiTruncateString ( m , 35 ) ;
      file = m;
      E = ( DIE * ) kgGetWidget ( Tmp , 3 ) ;
      kgFreeDouble ( ( void ** ) ( E->menu ) ) ;
      uiCleanEbrowserImages ( E ) ;
      E->menu = m;
      kgUpdateWidget ( E ) ;
      E = ( DIE * ) kgGetWidget ( Tmp , 2 ) ;
      uiCleanEbrowserImages ( E ) ;
      kgFreeDouble ( ( void ** ) ( E->menu ) ) ;
      E->menu = dir;
      kgUpdateWidget ( E ) ;
      dirs->file = file;
      dirs->dir = dir;
      kgUpdateOn ( ( DIALOG * ) Tmp ) ;
      switch ( key ) {
          case 1:
          break;
      }
      return ret;
  }
  int FileBrowserbrowser2callback ( int key , int i , void *Tmp ) {
      DIA *D;DIW *B;
      char *m;
      int ret = 0 , ln;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .w;
      m = Dgetmenustring ( Tmp , i ) ;
      uiRestoreString ( m , 35 ) ;
      kgSetstring ( Tmp , 0 , 0 , m ) ;
      kgUpdateWidget ( kgGetWidget ( Tmp , 0 ) ) ;
      switch ( key ) {
          case 1:
          break;
      }
      kgUpdateOn ( ( DIALOG * ) Tmp ) ;
      return ret;
  }
  int FileBrowserhoribar1callback ( int key , int i , void *Tmp ) {
      DIA *D;DILN *B;
      int n , ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .H;
      n = B->nx;
      switch ( key ) {
          case 1:
          break;
          case 2:
          break;
      }
      return ret;
  }
  int FileBrowserbutnbox1callback ( int key , int i , void *Tmp ) {
      DIA *D;DIB *B;
      DIRS *dirs;
      DIE *E;
      int n , ret = 1 , k , ln;
      char **m , *filter , **dir , **file;
      D = ( ( DIALOG * ) Tmp )->d;
      dirs = ( ( DIALOG * ) Tmp )->pt;
      B = D [ i ] .b;
      n = B->nx*B->ny;
      switch ( key ) {
          case 1:
          k = strlen ( dirs->CurDir ) ;
          while ( ( dirs->CurDir [ k ] != '/' ) && ( k > 1 ) ) \
          {dirs->CurDir [ k ] = '\0';k--;}
          dirs->CurDir [ k ] = '\0';
          uiinfo_wprintf ( ( DIALOG * ) Tmp , ( ( DIALOG * ) Tmp )->d [ 1 ] .i->twin , \
          dirs->CurDir ) ;
          m = _uiFolderMenu ( dirs->CurDir ) ;
          uiTruncateString ( m , 35 ) ;
          dir = m;
          break;
      }
      filter = Dgetstring ( Tmp , 0 , 1 ) ;
      m = _uiFileMenu ( dirs->CurDir , filter ) ;
      uiTruncateString ( m , 35 ) ;
//  ui_updatemenu(Tmp,3,m);
//  ui_updatemenu(Tmp,2,dir);
      E = ( DIE * ) kgGetWidget ( Tmp , 3 ) ;
      kgFreeDouble ( ( void ** ) ( E->menu ) ) ;
      uiCleanEbrowserImages ( E ) ;
      E->menu = m;
      kgUpdateWidget ( E ) ;
      E = ( DIE * ) kgGetWidget ( Tmp , 2 ) ;
      kgFreeDouble ( ( void ** ) ( E->menu ) ) ;
      uiCleanEbrowserImages ( E ) ;
      E->menu = dir;
      kgUpdateWidget ( E ) ;
      dirs->file = m;
      dirs->dir = dir;
      kgUpdateOn ( ( DIALOG * ) Tmp ) ;
      return ret;
  }
  int FileBrowserbutnbox2callback ( int key , int i , void *Tmp ) {
      DIA *D;DIB *B;
      DIE *E;
      DIRS *dirs;
      int n , ret = 1;
      char **m , *filter , **dir , **file;
      D = ( ( DIALOG * ) Tmp )->d;
      dirs = ( ( DIALOG * ) Tmp )->pt;
      B = D [ i ] .b;
      n = B->nx*B->ny;
      switch ( key ) {
          case 1:
          strcpy ( dirs->CurDir , dirs->HomeDir ) ;
          uiinfo_wprintf ( ( DIALOG * ) Tmp , ( ( DIALOG * ) Tmp )->d [ 1 ] .i->twin , \
          dirs->CurDir ) ;
          m = _uiFolderMenu ( dirs->CurDir ) ;
          uiTruncateString ( m , 35 ) ;
          dir = m;
          break;
      }
      filter = Dgetstring ( Tmp , 0 , 1 ) ;
      m = _uiFileMenu ( dirs->CurDir , filter ) ;
      uiTruncateString ( m , 35 ) ;
      E = ( DIE * ) kgGetWidget ( Tmp , 3 ) ;
      kgFreeDouble ( ( void ** ) ( E->menu ) ) ;
      uiCleanEbrowserImages ( E ) ;
      E->menu = m;
      kgUpdateWidget ( E ) ;
      E = ( DIE * ) kgGetWidget ( Tmp , 2 ) ;
      kgFreeDouble ( ( void ** ) ( E->menu ) ) ;
      uiCleanEbrowserImages ( E ) ;
      E->menu = dir;
      kgUpdateWidget ( E ) ;
//  _ui_updatemenu(Tmp,3,m);
//  _ui_updatemenu(Tmp,2,dir);
      dirs->file = m;
      dirs->dir = dir;
      kgUpdateOn ( ( DIALOG * ) Tmp ) ;
      return ret;
  }
  int FileBrowserinit ( void *Tmp ) {
      int ret = 1;
      DIRS *dirs;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      DII *I;
      dirs = D->pt;
      I = ( DII * ) ( D->d [ 1 ] .t ) ;
      uiinfo_wprintf ( ( DIALOG * ) Tmp , D->d [ 1 ] .i->twin , dirs->CurDir ) ;
      return ret;
  }
  static int uiGetDirFile ( char *flname , char *folder ) {
      int i;
      i = 0;
      i = strlen ( flname ) ;
      i--;
      while ( flname [ i ] != '/' ) {
          i--;
          if ( i < 0 ) break;
      }
      if ( i < 0 ) {
          getcwd ( folder , 499 ) ;
      }
      else {
          strcpy ( folder , flname ) ;
          strcpy ( flname , folder+i+1 ) ;
          if ( i == 0 ) folder [ i++ ] = '/';
          folder [ i ] = '\0';
      }
      return 1;
  }
  char * kgGetFolderName ( char *flname ) {
      int i , j;
      char basename [ 500 ] ;
      char *fpt = NULL;
      i = 0;
      i = strlen ( flname ) ;
      i--;
      while ( flname [ i ] != '/' ) { i--; if ( i < 0 ) break; }
      i++;
      if ( i != 0 ) {
          strcpy ( basename , flname ) ;
          basename [ i ] = '\0';
          fpt = ( char * ) malloc ( strlen ( basename ) +1 ) ;
          strcpy ( fpt , basename ) ;
      }
      return fpt;
  }
  int kgFolderBrowser ( void *parent , int xo , \
  int yo , char *flname , char *fltr ) \
  {
      int v2 = 1 , v3 = 1;
      int v4 = 1 , v5 = 1;
      int ret = 1;
      DIALOG D;
      DIA d [ 8 ] ;
      int dcount , fcount , i;
      char **folder , **file;
      char **dfolder , **dfile , **m = NULL;
      char filter [ 300 ] , w_file [ 300 ] = {" "};
      DIRS Dir;
      T_ELMT e0 [ 2 ] = {
          {"File Name:%30s" , flname , 1 , 0 , NULL} , \
          {"Filter   :%30s" , fltr , 1 , 0 , NULL}
      };
      DIT t0 = {
          't' , 20 , 18 , 439 , 88 , 24 , 1 , 2 , e0 , 1 , 1 , NULL , \
          FileBrowsertextbox1callback /* args , Call back */
      };
      t0.type = 1;
      DII i1 = {
      'i' , 15 , 94 , 489 , 127 , 65 , 1 };
      DIE w2 = {
          'e' , 7 , 137 , 310 , 400 , 8 , & v2 , "" , NULL , NULL , \
          FileBrowserbrowser1callback , /* *args , callback */
      24 , 6 , 24 , 1 , 1 , 1 , 0 };
      DIE w3 = {
          'e' , 310 , 137 , 630 , 400 , 8 , & v3 , "" , NULL , NULL , \
          FileBrowserbrowser2callback , /* *args , callback */
      24 , 6 , 24 , 1 , 1 , 1 , 0 };
      int butn1 = 1;
      int sw4 [ 2 ] = {
      1 , 1 };
      char *titles4 [ ] = {
      "" , "" , NULL };
      void *xpm4 [ ] = {
//    "##Okay48.png", 
//    "##Cancel48.png", 
//    &Okay48_str,
//    &Cancel48_str,
       & okay_str , & Cancel_str , NULL , NULL , NULL , NULL , NULL };
      int bkgr4 [ ] = {
      -1 , -1 , -1 };
      char *butncode4 = NULL;
      DILN H4 = {
          'H' , 258 , 408 , 384 , 471 , 9 , 0 , 54 , 54 , 2 , 1 , & butn1 , sw4 , \
          titles4 , butncode4 , NULL , FileBrowserhoribar1callback , \
               /* arg , Callbak */
          xpm4 , bkgr4 /* pointers to xpms and colors */
       , 4 , 0.2 };
      char *titles5 [ ] = {
      "" , NULL };
      void *xpm5 [ ] = {
//    "##Up32.png", 
       & updir_str , NULL , NULL , NULL , NULL };
      int bkgr5 [ ] = {
      -1 , -1 };
      char *butncode5 = NULL;
      int sw5 [ ] = {1 , 1};
      char *fpt = NULL;
      DIB n5 = {
          'b' , 465 , 28 , 520 , 83 , 9 , 9 , 45 , 45 , 1 , 1 , & v4 , sw5 , titles5 , \
          butncode5 , NULL , FileBrowserbutnbox1callback , /* args , Callbak */
          xpm5 , bkgr5 /* pointers to xpms and colors */
       , 2 , 0.5 };
      char *titles6 [ ] = {
      "" , NULL };
      void *xpm6 [ ] = {
//   "##Home32.png", 
       & Home_str , NULL , NULL , NULL };
      int bkgr6 [ ] = {
      -1 , -1 };
      char *butncode6 = NULL;
      DIB n6 = {
          'b' , 521 , 28 , 576 , 83 , 9 , 9 , 45 , 45 , 1 , 1 , & v5 , sw5 , titles6 , \
          butncode6 , NULL , FileBrowserbutnbox2callback , /* args , Callbak */
          xpm6 , bkgr6 /* pointers to xpms and colors */
       , 2 , 0.5 };
      fpt = kgGetFolderName ( flname ) ;
      if ( fpt != NULL ) if ( fpt [ 0 ] < ' ' ) {free ( fpt ) ;fpt = NULL;}
      if ( fpt != NULL ) {
          strcpy ( Dir.HomeDir , fpt ) ;
          free ( fpt ) ;
      }
      else if ( getcwd ( Dir.HomeDir , 499 ) == NULL ) return -1;
//  strcpy(CurDir,HomeDir);
      uiGetDirFile ( flname , Dir.CurDir ) ;
      strcpy ( filter , fltr ) ;
      e0 [ 1 ] .v = ( void * ) filter;
      strncpy ( w_file , flname , 99 ) ;
      folder = _uiFolderMenu ( Dir.CurDir ) ;
      uiTruncateString ( folder , 35 ) ;
      w2.menu = folder;
      file = _uiFileMenu ( Dir.CurDir , filter ) ;
      uiTruncateString ( file , 35 ) ;
      w3.menu = file;
      Dir.file = file;
      Dir.dir = folder;
      d [ 0 ] .t = & t0;
      d [ 1 ] .i = & i1;
      d [ 2 ] .e = & w2;
      d [ 3 ] .e = & w3;
      d [ 4 ] .H = & H4;
      d [ 5 ] .b = & n5;
      d [ 6 ] .b = & n6;
      d [ 7 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 4;
      D.df = 6;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = 214; /* Position of Dialog */
      D.yo = 90;
      D.yo = xo;
      D.xo = yo; /* Position of Dialog */
      D.xl = 635; /* Length of Dialog */
      D.yl = 478; /* Width of Dialog */
//  D.gc = get_gui_colours();
//  set_gui_colours(D.gc);
      D.Initfun = FileBrowserinit; /* Width of Dialog */
      D.Deco = 1;
      D.DrawBkgr = 1;
      D.butattn = 0;
      D.kbattn = 0;
      D.Bkpixmap = NULL;
      D.Callback = NULL;
      D.transparency = 0.0;
      D.Sticky = 0;
      D.Fixpos = 0;
      D.NoTaskBar = 0;
      D.Newwin = 0;
      D.parent = parent;
      D.pt = & Dir;
      strcpy ( D.name , "Kulina Folder Browser" ) ;
      D.StackPos = 1;
      D.Shapexpm = NULL;
      if ( parent == NULL ) {
          D.Newwin = 1;
      }
      kgDefaultGuiTheme ( & ( D.gc ) ) ;
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      m = Dir.dir;
      kgFreeDouble ( ( void ** ) m ) ;
      m = Dir.file;
      kgFreeDouble ( ( void ** ) m ) ;
      i = 0;
      while ( flname [ i ] == ' ' ) i++;
      if ( flname [ i ] < ' ' ) ret = 0;
      else {
          if ( strcmp ( Dir.CurDir , "/" ) != 0 ) strcat ( Dir.CurDir , "/" ) ;
          strcat ( Dir.CurDir , ( char * ) flname+i ) ;
          strcpy ( ( char * ) flname , Dir.CurDir ) ;
          ret = ret%2;
      }
      return ret;
  }
  char ** _uiFileMenu ( char *dir , char *filter ) {
      Dlink *L = NULL;
      int type , nf = 0 , n = 0 , j = 0;
      char code , buf [ 500 ] , blnk [ 2 ] = " ";
      char **m , *item;
      DIR *dp;
      struct dirent *pt = NULL;
      struct stat st;
      dp = opendir ( dir ) ;
      if ( dp == NULL ) return NULL;
      L = Dopen ( ) ;
      while ( ( pt = readdir ( dp ) ) != NULL ) {
          if ( pt->d_name [ 0 ] == '.' ) continue;
/*
     type = stat(pt->d_name,&st);
     type = st.st_mode;
     if( S_ISREG(type)) {
*/
#if 1
          strcpy ( buf , dir ) ;
          strcat ( buf , "/" ) ;
          strcat ( buf , pt->d_name ) ;
          stat ( buf , & st ) ;
          if ( S_ISREG ( st.st_mode ) && ( ! S_ISDIR ( st.st_mode ) ) ) {
#else
//     if(pt->d_type != DT_DIR) {
#endif
              sprintf ( buf , "%-s" , pt->d_name ) ;
              if ( _filter_string ( buf , filter ) ) {
                  item = ( char * ) Malloc ( strlen ( buf ) +5 ) ;
                  strcpy ( item , buf ) ;Dadd ( L , item ) ;
                  nf++;
              }
              else {
                  continue;
              }
          }
          else continue;
          code = 'f';
      }
      Resetlink ( L ) ;
      nf = Dcount ( L ) ;
      Resetlink ( L ) ;
      n = ( nf ) +1;
      m = ( char ** ) Malloc ( sizeof ( char * ) *n ) ;
      j = 0;
      while ( ( m [ j ] = ( char * ) Getrecord ( L ) ) != NULL ) { j++;}
      arrange ( m , nf ) ;
      closedir ( dp ) ;
      Dfree ( L ) ;
      return m;
  }
  char **kgFileMenu ( char *dir , char *filter ) {
      return _uiFileMenu ( dir , filter ) ;
  }
  int filebrowsertextbox1callback ( int key , int i , void *Tmp ) {
      char d_name [ 150 ] , *filter;
      char **m;
      DIE *E;
      DIA *D;DIT *T;T_ELMT *e;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      m = ( char ** ) ( ( DIALOG * ) Tmp )->pt;
      T = D [ i ] .t;
      e = T->elmt;
      getcwd ( d_name , 149 ) ;
      filter = Dgetstring ( Tmp , i , 1 ) ;
      m = _uiFileMenu ( d_name , filter ) ;
//  scr_recover();
      uiTruncateString ( m , 35 ) ;
//  _ui_updatemenu(Tmp,2,m);
      E = ( DIE * ) kgGetWidget ( Tmp , 2 ) ;
      uiCleanEbrowserImages ( E ) ;
      kgFreeDouble ( ( void ** ) E->menu ) ;
      E->menu = m;
      kgUpdateWidget ( E ) ;
       ( ( DIALOG * ) Tmp )->pt = m;
      uiUpdateOn ( ( DIALOG * ) Tmp ) ;
      return ret;
  }
  int filebrowserhoribar1callback ( int key , int i , void *Tmp ) {
      DIA *D;DILN *B;
      int n , ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .H;
      n = B->nx;
      switch ( key ) {
          case 1:
          break;
          case 2:
          break;
      }
//  scr_recover();
      return ret;
  }
  int filebrowserbrowser1callback ( int key , int i , void *Tmp ) {
      DIA *D;DIE *B;
      int ret = 1;
      char *m;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .e;
      m = Dgetmenustring ( Tmp , i ) ;
      uiRestoreString ( m , 35 ) ;
      Dsetstring ( Tmp , 0 , 0 , m ) ;
      uiResetString ( m , 35 ) ;
      switch ( key ) {
          case 1:
          break;
      }
      uiUpdateOn ( ( DIALOG * ) Tmp ) ;
      return ret;
  }
  int filebrowserinit ( void *Tmp ) {
      int ret = 1;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      return ret;
  }
  int kgFileBrowser ( void *parent , int x0 , int y0 , char *v0 , char *v1 ) {
      int ret = 1;
      DIALOG D;
      DIA d [ 4 ] ;
      int v2 = 1 , i;
      char **m , *item;
      char d_name [ 150 ] ;
      char filter [ 150 ] , w_file [ 150 ] = {" "};
      T_ELMT e0 [ 2 ] = {
          {"File Name%30s" , v0 , 1 , 0 , NULL} , {"Filter   %30s" , v1 , 1 , 0 , NULL}
      };
      DIT t0 = {
          't' , 17 , 13 , 427 , 83 , 20 , 1 , 2 , e0 , 1 , 1 , NULL , \
          filebrowsertextbox1callback /* args , Call back */
      };
      t0.type = 0;
      int butn1 = 1;
      int sw1 [ 2 ] = {
      1 , 1 };
      char *titles1 [ ] = {
      "Okay" , "Cancel" , NULL };
      void *xpm1 [ ] = {
      NULL , NULL , NULL , NULL , NULL , NULL , NULL };
      int bkgr1 [ ] = {
      -1 , -1 , -1 };
      char *butncode1 = NULL;
      DILN H1 = {
          'H' , 120 , 296 , 308 , 334 , 20 , 0 , 80 , 30 , 2 , 1 , & butn1 , sw1 , \
          titles1 , butncode1 , NULL , filebrowserhoribar1callback , \
               /* arg , Callbak */
          xpm1 , bkgr1 /* pointers to xpms and colors */
       , 2 , 0.2 };
      DIE w2 = {
          'e' , 19 , 90 , 428 , 294 , 7 , & v2 , "" , NULL , NULL , \
          filebrowserbrowser1callback , /* *args , callback */
      20 , 6 , 22 , 1 , 1 , 1 , 0 };
      getcwd ( d_name , 149 ) ;
//  printf("Dir: %s\n",d_name);
      strcpy ( filter , v1 ) ;
      e0 [ 1 ] .v = filter;
      strncpy ( w_file , v0 , 99 ) ;
      m = _uiFileMenu ( d_name , filter ) ;
      uiTruncateString ( m , 35 ) ;
      w2.menu = m;
      d [ 0 ] .t = & t0;
      d [ 1 ] .H = & H1;
      d [ 2 ] .e = & w2;
      d [ 3 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 4;
      D.df = -1;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = x0; /* Position of Dialog */
      D.yo = y0;
      D.xl = 445; /* Length of Dialog */
      D.yl = 345; /* Width of Dialog */
      D.Initfun = filebrowserinit; /* Width of Dialog */
      D.Deco = 1;
      D.DrawBkgr = 1;
      D.Bkpixmap = NULL;
      D.Callback = NULL;
      D.transparency = 0.0;
      D.Sticky = 0;
      D.Fixpos = 0;
      D.NoTaskBar = 0;
      D.Newwin = 0;
      D.parent = parent;
      D.StackPos = 1;
      D.Shapexpm = NULL;
      if ( parent == NULL ) D.Newwin = 1;
//  printf("Newwin:%d\n", D.Newwin);
      strcpy ( D.name , "File Browser" ) ;
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
      D.pt = m;
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
//  m =Dgetmenu(&D,2);
      m = ( char ** ) D.pt;
      i = 0;
      while ( m [ i ] != NULL ) {free ( m [ i ] ) ;i++;}
      free ( m ) ;
      ret = ret%2;
      return ret;
  }
  int _ui_setattrib ( void *parent , int xo , int yo , void *v0 , void *v1 , \
  void *v2 , void *v3 , void *v4 , void *v5 , \
  void *v6 , void *v7 , void *v14 ) \
  {
      int ret = 1;
      DIALOG D;
      DIA d [ 3 ] ;
      T_ELMT e0 [ 9 ] = {
//    {"Text bold      %2d", v8, 1},
//    {"Text prop.     %2d", v9, 1},
//    {"Text fill      %2d", v10, 1},
//    {"Text bodr      %2d", v11, 1},
//    {"Text pattern   %2d", v12, 1},
//    {"Border width   %2d", v13, 1},
//    {"Arrow size     %2d", v15, 1}
          {"Line Style     %2d" , v0 , 1 , 0 , NULL} , {"Line color     %2d" , \
          v1 , 1 , 0 , NULL} , \
          {"Line Width     %2d" , v2 , 1 , 0 , NULL} , {"Marker Type    %2d" , \
          v3 , 1 , 0 , NULL} , \
          {"Wipe/Fill color%2d" , v4 , 1 , 0 , NULL} , {"Text color     %2d" , \
          v5 , 1 , 0 , NULL} , \
          {"Text font      %2d" , v6 , 1 , 0 , NULL} , {"Text angle     %3d" , \
          v7 , 1 , 0 , NULL} , \
      {"Border color   %2d" , v14 , 1} , };
      DIT t0 = {
          't' , 11 , 8 , 235 , 301 , 20 , 1 , 9 , e0 , 1 , \
          1 , NULL , NULL /* args , Call back */
      };
      t0.type = 0;
      int butn1 = 1;
      int sw1 [ 1 ] = {
      1 };
      char *titles1 [ ] = {
      "Okay" , NULL };
      void *xpm1 [ ] = {
      NULL , NULL , NULL , NULL , NULL , NULL };
      int bkgr1 [ ] = {
      -1 , -1 };
      char *butncode1 = NULL;
      DILN H1 = {
          'H' , 94 , 319 , 170 , 365 , 9 , 0 , 60 , 30 , 1 , 1 , & butn1 , \
          sw1 , titles1 , butncode1 , NULL , NULL , /* arg , Callbak */
          xpm1 , bkgr1 /* pointers to xpms and colors */
       , 2 , 0.2 };
      d [ 0 ] .t = & t0;
      d [ 1 ] .H = & H1;
      d [ 2 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 2;
      D.df = -1;
      D.tw = 4;
      D.bw = 54;
      D.lw = 4;
      D.rw = 4;
      D.xo = xo; /* Position of Dialog */
      D.yo = yo;
      D.xl = 248; /* Length of Dialog */
      D.yl = 368; /* Width of Dialog */
      D.Initfun = NULL; /* Width of Dialog */
      D.Deco = 1;
      D.DrawBkgr = 1;
      D.Bkpixmap = NULL;
      D.Callback = NULL;
      D.transparency = 0.0;
      D.Sticky = 0;
      D.Fixpos = 0;
      D.NoTaskBar = 0;
      D.Newwin = 0;
      D.parent = parent;
      D.StackPos = 1;
      D.Shapexpm = NULL;
      kgDefaultGuiTheme ( & ( D.gc ) ) ;
//  if(parent==NULL) D.Newwin=1;
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      return ret;
  }
  int kgAttibDiatextbox1callback ( int key , int i , void *Tmp ) {
      DIA *D;DIT *T;T_ELMT *e;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ i ] .t;
      e = T->elmt;
      return ret;
  }
  int kgAttibDiabutnbox1callback ( int key , int i , void *Tmp ) {
      int LnColor;
      DIG *Gbox;
      DIALOG *D;DIB *B;
      int n , ret = 1;
      int r , g , b;
      void **pt;
      int *count;
      D = ( DIALOG * ) Tmp;
      pt = ( void ** ) D->pt;
      Gbox = ( DIG * ) pt [ 0 ] ;
      count = ( int * ) pt [ 1 ] ;
      B = kgGetWidget ( D , i ) ;
      n = B->nx*B->ny;
      LnColor = kgGetint ( Tmp , 0 , 0 ) ;
      kgGetRGB ( Gbox , LnColor , & r , & g , & b ) ;
      if ( kgGetColor ( NULL , 100 , 200 , & r , & g , & b ) ) {
          LnColor = 901;
          kgChangeColor ( Gbox , LnColor , r , g , b ) ;
          *count = *count+18;
          kgSetint ( Tmp , 0 , 0 , LnColor ) ;
          kgUpdateWidget ( kgGetWidget ( Tmp , 0 ) ) ;
      }
      switch ( key ) {
          case 1:
          break;
      }
      return ret;
  }
  int kgAttibDiatextbox2callback ( int key , int i , void *Tmp ) {
      DIA *D;DIT *T;T_ELMT *e;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ i ] .t;
      e = T->elmt;
      return ret;
  }
  int kgAttibDiabutnbox2callback ( int key , int i , void *Tmp ) {
      int FillColor;
      DIALOG *D;DIB *B;
      DIG *Gbox;
      int n , ret = 1;
      int r , g , b;
      void **pt;
      int *count;
      D = ( DIALOG * ) Tmp;
      pt = ( void ** ) D->pt;
      Gbox = ( DIG * ) pt [ 0 ] ;
      count = ( int * ) pt [ 1 ] ;
      B = ( DIB * ) kgGetWidget ( D , i ) ;
      n = B->nx*B->ny;
      FillColor = kgGetint ( Tmp , 2 , 0 ) ;
      kgGetRGB ( Gbox , FillColor , & r , & g , & b ) ;
      if ( kgGetColor ( NULL , 100 , 200 , & r , & g , & b ) ) {
          FillColor = 902;
          kgChangeColor ( Gbox , FillColor , r , g , b ) ;
          *count = *count+18;
          kgSetint ( Tmp , 2 , 0 , FillColor ) ;
          kgUpdateWidget ( kgGetWidget ( Tmp , 2 ) ) ;
      }
      switch ( key ) {
          case 1:
          break;
      }
      return ret;
  }
  int kgAttibDiatextbox3callback ( int key , int i , void *Tmp ) {
      DIA *D;DIT *T;T_ELMT *e;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ i ] .t;
      e = T->elmt;
      return ret;
  }
  int kgAttibDiatextbox4callback ( int key , int i , void *Tmp ) {
      DIA *D;DIT *T;T_ELMT *e;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ i ] .t;
      e = T->elmt;
      return ret;
  }
  int kgAttibDiabutnbox3callback ( int key , int i , void *Tmp ) {
      int TextColor;
      DIALOG *D;DIB *B;
      DIG *Gbox;
      int n , ret = 1;
      int r , g , b;
      void **pt;
      int *count;
      D = ( DIALOG * ) Tmp;
      pt = ( void ** ) D->pt;
      Gbox = ( DIG * ) pt [ 0 ] ;
      count = ( int * ) pt [ 1 ] ;
      B = ( DIB * ) kgGetWidget ( D , i ) ;
      n = B->nx*B->ny;
      TextColor = kgGetint ( Tmp , 4 , 0 ) ;
      kgGetRGB ( Gbox , TextColor , & r , & g , & b ) ;
      if ( kgGetColor ( NULL , 100 , 200 , & r , & g , & b ) ) {
          TextColor = 903;
          kgChangeColor ( Gbox , TextColor , r , g , b ) ;
          *count = *count+18;
          kgSetint ( Tmp , 4 , 0 , TextColor ) ;
          kgUpdateWidget ( kgGetWidget ( Tmp , 4 ) ) ;
      }
      switch ( key ) {
          case 1:
          break;
      }
      return ret;
  }
  int kgAttibDiabutnbox4callback ( int key , int i , void *Tmp ) {
      int BorColor = 1001;
      DIALOG *D;DIB *B;
      DIG *Gbox;
      int n , ret = 1;
      int r , g , b;
      void **pt;
      int *count;
      D = ( DIALOG * ) Tmp;
      pt = ( void ** ) D->pt;
      Gbox = ( DIG * ) pt [ 0 ] ;
      count = ( int * ) pt [ 1 ] ;
      B = ( DIB * ) kgGetWidget ( D , i ) ;
      n = B->nx*B->ny;
      BorColor = kgGetint ( Tmp , 5 , 0 ) ;
      kgGetRGB ( Gbox , BorColor , & r , & g , & b ) ;
      if ( kgGetColor ( NULL , 100 , 200 , & r , & g , & b ) ) {
          BorColor = 904;
          kgChangeColor ( Gbox , BorColor , r , g , b ) ;
          *count = *count+18;
          kgSetint ( Tmp , 5 , 0 , BorColor ) ;
          kgUpdateWidget ( kgGetWidget ( Tmp , 5 ) ) ;
      }
      switch ( key ) {
          case 1:
          break;
      }
      return ret;
  }
  int kgAttibDiatextbox5callback ( int key , int i , void *Tmp ) {
      DIA *D;DIT *T;T_ELMT *e;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ i ] .t;
      e = T->elmt;
      return ret;
  }
  int kgAttibDiabutnbox5callback ( int key , int i , void *Tmp ) {
      int Font;
      DIG *G;
      DIA *D;DIB *B;
      int n , ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .b;
      n = B->nx*B->ny;
      Font = kgGetFont ( NULL , 100 , 200 ) ;
      kgSetint ( Tmp , 8 , 0 , Font ) ;
      kgUpdateWidget ( kgGetWidget ( Tmp , 8 ) ) ;
      switch ( key ) {
          case 1:
          break;
      }
      return ret;
  }
  int kgAttibDiabrowser1callback ( int item , int i , void *Tmp ) {
      DIA *D;DIW *B;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .w;
      switch ( item ) {
          case 1:
          break;
      }
      return ret;
  }
  int kgAttibDiabrowser2callback ( int item , int i , void *Tmp ) {
      DIA *D;DIW *B;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .w;
      switch ( item ) {
          case 1:
          break;
      }
      return ret;
  }
  int kgAttibDiabrowser3callback ( int item , int i , void *Tmp ) {
      DIA *D;DIW *B;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .w;
      switch ( item ) {
          case 1:
          break;
      }
      return ret;
  }
  int kgAttibDiabrowser4callback ( int item , int i , void *Tmp ) {
      DIA *D;DIW *B;
      int ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .w;
      switch ( item ) {
          case 1:
          break;
      }
      return ret;
  }
  int kgAttibDiahoribar1callback ( int key , int i , void *Tmp ) {
      DIA *D;DILN *B;
      int n , ret = 1;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .H;
      n = B->nx;
      switch ( key ) {
          case 1:
          break;
      }
      return ret;
  }
  int kgAttibDiainit ( void *Tmp ) {
      int ret = 1;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      return ret;
  }
  int kgAttibDiaCallBack ( void *Tmp , void *tmp ) {
      int ret = 0;
      DIALOG *D;
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
          }
      }
      return ret;
  }
  int kgAttibDia ( void *parent , void *v0 , void *v1 , void *v2 , void *v3 , \
  void *v4 , void *v5 , void *v6 , void *v7 , void *v8 , void *v9 , \
  void *v10 , void *v11 , void *v12 , void *v13 , void *ptrs ) \
  {
      int ret = 1;
      DIALOG D;
      DIA d [ 16 ] ;
      T_ELMT e0 [ 1 ] = {
          { ( char * ) "Line Color%3d" , v0 , 1 , 0 , NULL}
      };
      DIT t0 = {
          't' , 64 , 26 , 240 , 66 , 20 , 1 , 1 , e0 , 1 , 1 , NULL , \
          kgAttibDiatextbox1callback /* args , Call back */
      };
      t0.type = 0;
      char *titles1 [ ] = {
       ( char * ) "Select" , NULL };
      void *xpm1 [ ] = {
          NULL , /* pixmap info */
      NULL , NULL , NULL };
      int bkgr1 [ ] = {
      -1 , -1 };
      int sw1 [ ] = {1 , 1};
      char *butncode1 = NULL;
      DIB n1 = {
          'b' , 238 , 31 , 300 , 63 , 0 , 0 , 54 , 24 , 1 , 1 , ( int * ) v1 , sw1 , \
          titles1 , butncode1 , NULL , kgAttibDiabutnbox1callback , \
               /* args , Callbak */
          xpm1 , bkgr1 , /* pointers to xpms and colors */
          2 , 0.200000 /* button type and roundinfg factor ( 0-0.5 ) */
      };
      T_ELMT e2 [ 1 ] = {
          { ( char * ) "Wipe/Fill Color%3d" , v2 , 1 , 0 , NULL}
      };
      DIT t2 = {
          't' , 18 , 65 , 241 , 102 , 20 , 1 , 1 , e2 , 1 , 1 , NULL , \
          kgAttibDiatextbox2callback /* args , Call back */
      };
      t2.type = 0;
      char *titles3 [ ] = {
       ( char * ) "Select" , NULL };
      void *xpm3 [ ] = {
          NULL , /* pixmap info */
      NULL , NULL , NULL };
      int bkgr3 [ ] = {
      -1 , -1 };
      char *butncode3 = NULL;
      int sw3 [ ] = {1 , 1};
      DIB n3 = {
          'b' , 238 , 69 , 300 , 101 , 0 , 0 , 54 , 24 , 1 , 1 , ( int * ) v3 , sw3 , \
          titles3 , butncode3 , NULL , kgAttibDiabutnbox2callback , \
               /* args , Callbak */
          xpm3 , bkgr3 , /* pointers to xpms and colors */
          2 , 0.200000 /* button type and roundinfg factor ( 0-0.5 ) */
      };
      T_ELMT e4 [ 1 ] = {
          { ( char * ) "Text Color%3d" , v4 , 1 , 0 , NULL}
      };
      DIT t4 = {
          't' , 64 , 101 , 240 , 141 , 20 , 1 , 1 , e4 , 1 , 1 , NULL , \
          kgAttibDiatextbox3callback /* args , Call back */
      };
      t4.type = 0;
      T_ELMT e5 [ 1 ] = {
          { ( char * ) "Border Color%3d" , v5 , 1 , 0 , NULL}
      };
      DIT t5 = {
          't' , 46 , 141 , 240 , 181 , 20 , 1 , 1 , e5 , 1 , 1 , NULL , \
          kgAttibDiatextbox4callback /* args , Call back */
      };
      t5.type = 0;
      char *titles6 [ ] = {
       ( char * ) "Select" , NULL };
      void *xpm6 [ ] = {
          NULL , /* pixmap info */
      NULL , NULL , NULL };
      int bkgr6 [ ] = {
      -1 , -1 };
      char *butncode6 = NULL;
      int sw6 [ ] = {1 , 1};
      DIB n6 = {
          'b' , 238 , 103 , 300 , 135 , 0 , 0 , 54 , 24 , 1 , 1 , ( int * ) v6 , sw6 , \
          titles6 , butncode6 , NULL , kgAttibDiabutnbox3callback , \
               /* args , Callbak */
          xpm6 , bkgr6 , /* pointers to xpms and colors */
          2 , 0.200000 /* button type and roundinfg factor ( 0-0.5 ) */
      };
      char *titles7 [ ] = {
       ( char * ) "Select" , NULL };
      void *xpm7 [ ] = {
          NULL , /* pixmap info */
      NULL , NULL , NULL };
      int bkgr7 [ ] = {
      -1 , -1 };
      char *butncode7 = NULL;
      DIB n7 = {
          'b' , 238 , 143 , 300 , 175 , 0 , 0 , 54 , 24 , 1 , 1 , ( int * ) v7 , sw6 , \
          titles7 , butncode7 , NULL , kgAttibDiabutnbox4callback , \
               /* args , Callbak */
          xpm7 , bkgr7 , /* pointers to xpms and colors */
          2 , 0.200000 /* button type and roundinfg factor ( 0-0.5 ) */
      };
      T_ELMT e8 [ 1 ] = {
          { ( char * ) "Text Font%3d" , v8 , 1 , 0 , NULL}
      };
      DIT t8 = {
          't' , 73 , 181 , 240 , 221 , 20 , 1 , 1 , e8 , 1 , 1 , NULL , \
          kgAttibDiatextbox5callback /* args , Call back */
      };
      t8.type = 0;
      char *titles9 [ ] = {
       ( char * ) "Select" , NULL };
      void *xpm9 [ ] = {
          NULL , /* pixmap info */
      NULL , NULL , NULL };
      int bkgr9 [ ] = {
      -1 , -1 };
      char *butncode9 = NULL;
      DIB n9 = {
          'b' , 239 , 184 , 301 , 216 , 0 , 0 , 54 , 24 , 1 , 1 , ( int * ) v9 , sw6 , \
          titles9 , butncode9 , NULL , kgAttibDiabutnbox5callback , \
               /* args , Callbak */
          xpm9 , bkgr9 , /* pointers to xpms and colors */
          2 , 0.200000 /* button type and roundinfg factor ( 0-0.5 ) */
      };
      char *menu10 [ ] = {
           ( char * ) "0" , ( char * ) "1" , ( char * ) "2" , ( char * ) "3" , \
       ( char * ) "4" , ( char * ) "5" , NULL };
      DIW w10 = {
          'w' , 317 , 31 , 475 , 60 , 4 , ( int * ) v10 , ( char * ) "Line Style" , \
          menu10 , NULL , kgAttibDiabrowser1callback /* *args , callback */
      };
      char *menu11 [ ] = {
           ( char * ) "1" , ( char * ) "2" , ( char * ) "3" , \
           ( char * ) "4" , ( char * ) "5" , \
       ( char * ) "6" , ( char * ) "7" , ( char * ) "8" , NULL };
      DIW w11 = {
          'w' , 317 , 66 , 475 , 95 , 4 , ( int * ) v11 , ( char * ) "Line Width" , \
          menu11 , NULL , kgAttibDiabrowser2callback /* *args , callback */
      };
      char *menu12 [ ] = {
           ( char * ) "0" , ( char * ) "1" , ( char * ) "2" , \
           ( char * ) "3" , ( char * ) "4" , \
           ( char * ) "5" , ( char * ) "6" , ( char * ) "7" , \
           ( char * ) "8" , ( char * ) "9" , \
           ( char * ) "10" , ( char * ) "11" , ( char * ) "12" , ( char * ) "13" , \
           ( char * ) "14" , ( char * ) "15" , ( char * ) "16" , ( char * ) "17" , \
           ( char * ) "18" , ( char * ) "19" , ( char * ) "20" , ( char * ) "21" , \
           ( char * ) "22" , ( char * ) "23" , ( char * ) "24" , ( char * ) "25" , \
           ( char * ) "26" , ( char * ) "27" , ( char * ) "28" , \
       ( char * ) "29" , NULL };
      DIW w12 = {
          'w' , 308 , 101 , 475 , 130 , 4 , ( int * ) v12 , ( char * ) "Marker Type" , \
          menu12 , NULL , kgAttibDiabrowser3callback /* *args , callback */
      };
      char *menu13 [ ] = {
           ( char * ) "0" , ( char * ) "10" , ( char * ) "20" , ( char * ) "30" , \
           ( char * ) "40" , ( char * ) "50" , ( char * ) "60" , ( char * ) "70" , \
           ( char * ) "80" , ( char * ) "90" , ( char * ) "100" , ( char * ) "110" , \
           ( char * ) "120" , ( char * ) "130" , ( char * ) "140" , ( char * ) "150" , \
           ( char * ) "160" , ( char * ) "170" , ( char * ) "180" , ( char * ) "190" , \
           ( char * ) "200" , ( char * ) "210" , ( char * ) "220" , ( char * ) "230" , \
           ( char * ) "240" , ( char * ) "250" , ( char * ) "260" , ( char * ) "270" , \
           ( char * ) "280" , ( char * ) "290" , ( char * ) "300" , ( char * ) "310" , \
           ( char * ) "320" , ( char * ) "330" , ( char * ) "340" , \
       ( char * ) "350" , NULL };
      DIW w13 = {
          'w' , 309 , 136 , 475 , 164 , 4 , ( int * ) v13 , ( char * ) "Text Angle" , \
          menu13 , NULL , kgAttibDiabrowser4callback /* *args , callback */
      };
      int butn1 = 1;
      int sw14 [ 1 ] = {
      1 };
      char *titles14 [ ] = {
       ( char * ) "OKAY" , NULL };
      void *xpm14 [ ] = {
          NULL , /* pixmap info */
      NULL , NULL , NULL , NULL , NULL };
      int bkgr14 [ ] = {
      -1 , -1 };
      char *butncode14 = NULL;
      DILN H14 = {
          'H' , 223 , 241 , 312 , 274 , 4 , 0 , 80 , 25 , 1 , 1 , & butn1 , sw14 , \
          titles14 , butncode14 , NULL , kgAttibDiahoribar1callback , \
               /* arg , Callbak */
          xpm14 , bkgr14 , /* pointers to xpms and colors */
          2 , 0.200000 /* button type and roundinfg factor ( 0-0.5 ) */
      };
      d [ 0 ] .t = & t0;
      d [ 1 ] .b = & n1;
      d [ 2 ] .t = & t2;
      d [ 3 ] .b = & n3;
      d [ 4 ] .t = & t4;
      d [ 5 ] .t = & t5;
      d [ 6 ] .b = & n6;
      d [ 7 ] .b = & n7;
      d [ 8 ] .t = & t8;
      d [ 9 ] .b = & n9;
      d [ 10 ] .w = & w10;
      d [ 11 ] .w = & w11;
      d [ 12 ] .w = & w12;
      d [ 13 ] .w = & w13;
      d [ 14 ] .H = & H14;
      d [ 15 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 4;
      D.df = 14;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = 200; /* Position of Dialog */
      D.yo = 200;
      D.xl = 509; /* Length of Dialog */
      D.yl = 282; /* Width of Dialog */
      D.Initfun = kgAttibDiainit; /* init fuction for Dialog */
      D.kbattn = 0;; /* 1 for drawing keyborad attention */
      D.butattn = 0; /* 1 for drawing button attention */
      D.fullscreen = 0; /* 1 for for fullscreen mode */
      D.Deco = 1; /* 1 for Window Decorration */
      D.transparency = 0.000000; /* float 1.0 for full transparency */
      D.Newwin = 0; /* 1 for new window not yet implemented */
      D.DrawBkgr = 1; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 0; /* 1 for stickyness */
#if 0 
      D.Callback = kgAttibDiaCallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.Fixpos = 0; /* 1 for Fixing Position */
      D.NoTaskBar = 0; /* 1 for not showing in task bar*/
      D.StackPos = 1; /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/
      D.Shapexpm = NULL; /* PNG/jpeg file for window shape;
      Black color will not be drawn */
      D.parent = parent; /* 1 for not showing in task bar*/
      D.pt = ptrs; /* any data to be passed by user*/
      strcpy ( D.name , "Kulina Designer ver 1.0" ) ;
           /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
      } /* end of fullscreen mode */
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
//  kgColorTheme(&D,230,230,230);    /*  set colors for gui*/
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      return ret;
  }
  int uiset_atribs ( DIG *G ) {
/*************************************************

    Text_Box1  1 data values
    Buttonbox1 (new) 1 data value
    Text_Box2  1 data values
    Buttonbox2 (new) 1 data value
    Text_Box3  1 data values
    Text_Box4  1 data values
    Buttonbox3 (new) 1 data value
    Buttonbox4 (new) 1 data value
    Text_Box5  1 data values
    Buttonbox5 (new) 1 data value
    Browser1  1 data value
    Browser2  1 data value
    Browser3  1 data value
    Browser4  1 data value

*************************************************/
      int count = 0;
      void *pt [ 2 ] ;
      int LnColor , FillColor , TextColor , BorColor , Font;
      int LnStyle , LnWidth , MarkType , TextAngle;
      int v0 = 1;
      int v1 = 1;
      int v2 = 1;
      int v3 = 1;
      int v4 = 1;
      int v5 = 1;
      int v6 = 1;
      int v7 = 1;
      int v8 = 1;
      int v9 = 1;
      int v10 = 1;
      int v11 = 1;
      int v12 = 1;
      int v13 = 1;
      kgDC *dc;
      dc = G->dc;
      v0 = dc->ln_color;
      v2 = dc->fil_color;
      v4 = dc->t_color;
      v12 = dc->m_style+1;
      v13 = dc->trot/10+1;
      v8 = dc->t_font;
      v10 = dc->ln_style+1;
      v11 = dc->ln_width;
      v5 = dc->bod_color;
      pt [ 0 ] = G;
      pt [ 1 ] = & count;
      kgAttibDia ( NULL , & v0 , & v1 , & v2 , & v3 , & v4 , & v5 , & v6 , \
       & v7 , & v8 , & v9 , & v10 , & v11 , & v12 , & v13 , \
       ( void * ) pt ) ;
      LnStyle = v10-1;
      LnWidth = v11;
      MarkType = v12-1;
      TextAngle = ( v13-1 ) *10;
      dc->trot = TextAngle;
      dc->fil_color = v2;
      dc->t_color = v4;
      dc->t_font = v8;
      dc->ln_color = v0;
      dc->ln_style = LnStyle;
      dc->m_style = MarkType;
      dc->ln_width = LnWidth;
      dc->bod_color = v5;
      kgLineColor ( G , dc->ln_color ) ;
      kgTextAngle ( G , dc->trot ) ;
      kgTextFont ( G , dc->t_font ) ;
      kgMarkerType ( G , dc->m_style ) ;
      kgLineWidth ( G , dc->ln_width ) ;
      kgTextColor ( G , dc->t_color ) ;
      return 36+count;
  }
  int get_intr_text ( void *parent , void *v0 ) {
      int ret = 1;
      DIALOG D;
      DIA d [ 2 ] ;
      T_ELMT e0 [ 1 ] = {
          {"Give Text%50s" , v0 , 1 , 0 , NULL}
      };
      DIT t0 = {
          't' , 11 , 12 , 578 , 52 , 20 , 1 , 1 , e0 , 1 , \
          1 , NULL , NULL /* args , Call back */
      };
      t0.type = 0;
      d [ 0 ] .t = & t0;
      d [ 1 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 2;
      D.df = -1;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = 191; /* Position of Dialog */
      D.yo = 134;
      D.xl = 610; /* Length of Dialog */
      D.yl = 67; /* Width of Dialog */
      D.Deco = 0;
      D.DrawBkgr = 1;
      D.Bkpixmap = NULL;
      D.Callback = NULL;
      D.transparency = 0.0;
      D.Sticky = 0;
      D.Fixpos = 0;
      D.NoTaskBar = 0;
      D.Initfun = NULL; /* Width of Dialog */
      D.Newwin = 0;
//  D.parent=parent;
      D.parent = NULL;
      D.StackPos = 1;
      D.Shapexpm = NULL;
      if ( parent == NULL ) D.Newwin = 1;
      strcpy ( D.name , "Kulina Graphics" ) ;
      kgDefaultGuiTheme ( & ( D.gc ) ) ;
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      return ret;
  }
  int getfact ( void *parent , void *v0 , void *v1 , void *v2 ) {
      int ret = 1;
      DIALOG D;
      DIA d [ 3 ] ;
      T_ELMT e0 [ 3 ] = {
          {"Mul. fac for height%12F" , v0 , 1 , 0 , \
               NULL} , {"Mul. fac for width %12F" , \
          v1 , 1 , 0 , NULL} , \
          {"Mul. fac for gap   %12F" , v2 , 1 , 0 , NULL}
      };
      DIT t0 = {
          't' , 9 , 16 , 350 , 117 , 20 , 1 , 3 , e0 , 1 , \
          1 , NULL , NULL /* args , Call back */
      };
      t0.type = 0;
      int butn1 = 1;
      int sw1 [ 1 ] = {
      1 };
      char *titles1 [ ] = {
      "Okay" , NULL };
      void *xpm1 [ ] = {
      NULL , NULL , NULL , NULL , NULL , NULL };
      int bkgr1 [ ] = {
      -1 , -1 };
      char *butncode1 = NULL;
      DILN H1 = {
          'H' , 139 , 131 , 215 , 177 , 9 , 0 , 60 , 30 , 1 , 1 , & butn1 , \
          sw1 , titles1 , butncode1 , NULL , NULL , /* arg , Callbak */
          xpm1 , bkgr1 /* pointers to xpms and colors */
       , 2 , 0.2 };
      d [ 0 ] .t = & t0;
      d [ 1 ] .H = & H1;
      d [ 2 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 2;
      D.df = -1;
      D.tw = 4;
      D.bw = 52;
      D.lw = 4;
      D.rw = 4;
      D.xo = 255; /* Position of Dialog */
      D.yo = 115;
      D.xl = 369; /* Length of Dialog */
      D.yl = 179; /* Width of Dialog */
      D.Initfun = NULL;
      D.Deco = 1;
      D.DrawBkgr = 1;
      D.Bkpixmap = NULL;
      D.Callback = NULL;
      D.transparency = 0.0;
      D.Newwin = 1;
      D.Sticky = 0;
      D.Fixpos = 0;
      D.NoTaskBar = 0;
      D.Newwin = 0;
//  D.parent=parent;
      D.parent = NULL;
      D.StackPos = 1;
      D.Shapexpm = NULL;
      if ( parent == NULL ) D.Newwin = 1;
      strcpy ( D.name , "Kulina Graphics" ) ;
      kgDefaultGuiTheme ( & ( D.gc ) ) ;
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      return ret;
  }
  void get_size_factors ( void *parent , float *hfac , \
  float *wfac , float *gfac ) \
  {
/*************************************************

    Text_Box1  3 data values

*************************************************/
      double v0 = 0.0;
      double v1 = 0.0;
      double v2 = 0.0;
      v0 = *hfac;
      v1 = *wfac;
      v2 = *gfac;
      getfact ( parent , & v0 , & v1 , & v2 ) ;
      *hfac = v0;
      *wfac = v1;
      *gfac = v2;
  }
  void kgArrowSize ( DIG *G , int size ) {
      kgDC *dc;
      dc = G->dc;
      dc->A_size = size;
  }
  void kgDrawingTool ( DIG *G ) {
      float tww , thh , tgg;
      unsigned int *loc , *loco;
      char nbuf [ 30 ] ;
      char key , cr , bsp , last = 'd';
      char *cmenu [ ] = { "Move" , "Draw" , "Arrow" , \
           "Rectgl" , "Text" , "txtSize" , \
          "Undo" , "Wipe" , "polyfiL" , "atrIbts" , "arC" , "rhomBus" , \
      "marK" , "rePeat" , "bOrder" , "arc_Fill" , "Quit" , NULL};
      int ihori = 1 , item = 1 , ch;
      int nch , ncr , MAG = 0;
      int itxclr_o , i , count , k , counto;
      static float x , y , xx , yy , ytwtg , xtxt , ytxt;
      static char bf [ 200 ] , bf1 [ 200 ] ;
      float tw , th , tg;
      float xo , yo , radi;
      float xpoly [ 30 ] , ypoly [ 30 ] ;
      float hfac = 1.0 , wfac = 1.0 , gfac = 1.0 , A_fac = 1.0;
      int npoly = 0;
      DIALOG *parent;
      kgDC *dc;
      dc = G->dc;
      dc->cmds = ( unsigned int * ) Malloc ( sizeof ( int ) *1000L ) ;
      if ( dc->cmds == NULL ) {
          printf ( "Error: In mem alloc. in m_int..\n" ) ;
          exit ( 0 ) ;
      }
      parent = G->D;
      if ( dc->A_size > 10 ) A_fac = ( float ) \
       ( dc->A_size/10 ) / ( dc->A_size%10 ) ;
      tgg = 0;
      tww = ( dc->w_x2-dc->w_x1 ) /30.0;
      thh = ( dc->w_y2-dc->w_y1 ) /25.0;
      tw = tww , tg = tgg , th = thh;
      cr = 13;
      bsp = 8;
      x = dc->w_x1+ ( dc->w_x2-dc->w_x1 ) *0.5;
      y = dc->w_y1+ ( dc->w_y2-dc->w_y1 ) *0.5;
      xpoly [ 0 ] = x;ypoly [ 0 ] = y;
      xtxt = x;ytxt = y;
      xx = x;yy = y;
      xo = 0.;yo = 0.;
      /*start_intr();*/
      uiwrite_file ( G , & ( G->rbuf ) , dc->reviewfile ) ;
      count = 0;
      loc = dc->cmds;
//      set_state();
      if ( G->D_ON == 0 ) {
          MAG = G->MAG;
          kgAntialiasingOff ( G ) ;
      }
      kgLineStyle ( G , 0 ) ;
      kgLineColor ( G , 1 ) ;
      kgTextAngle ( G , 0 ) ;
      kgTextColor ( G , 16 ) ;
      kgMove2f ( G , x , y ) ;
      kgTextSize ( G , th , tw , tg ) ;
      * ( loc++ ) = 60;count++;
      ytwtg = ( tw+tg ) / ( dc->w_x2-dc->w_x1 ) * ( dc->w_y2-dc->w_y1 ) /0.75;
//      while((item=uiMenu(G->D,G->x1,G->y1,1,1,cmenu,17))!=17)
      while ( ( item = kgMenu ( G->D , parent->xo+G->x2-100 , \
      parent->yo+G->y1 , 1 , 1 , cmenu , 17 ) ) != 17 ) \
      {
          if ( G->Byte > ( B_min-100 ) ) {
              uiwrite_file ( G , & ( G->rbuf ) , dc->reviewfile ) ;
              count = 0;
              loc = dc->cmds;
          }
//          item = menu(5,20,17,8,(char *)cmenu,item);
          switch ( ( int ) item ) {
              case 6:
              get_size_factors ( parent , & hfac , & wfac , & gfac ) ;
              kgTextSize ( G , th*hfac , tw*wfac , tg*gfac ) ;
              count++;
              * ( loc++ ) = 26;
              ytwtg = ( tw+tg ) / ( dc->w_x2-dc->w_x1 ) * ( dc->w_y2-dc->w_y1 ) /0.75;
              item = 5;
              break;
              case 1:
              x = xx , y = yy;
              key = kgCrossCursor ( G , & x , & y ) ;
              kgMove2f ( G , x , y ) ;
              count++;
              * ( loc++ ) = 10;
              xx = x;yy = y;
              xo = 0;yo = 0;
              item = 2;
              break;
              case 2:
              kgMove2f ( G , xx , yy ) ;
              count++; * ( loc++ ) = 10;
              while ( ( key = kgRbrCursor ( G , & x , & y , & xx , & yy ) ) != '\r' ) {
                  if ( key == 'u' ) {
                      if ( count > 1 ) { loc--;count--;
                          uiupdate_intr ( G , count , dc->cmds ) ;
                  uiupdate_view ( G ) ;}}
                  else {kgDraw2f ( G , x , y ) ; xx = x;yy = y;
                  count++; * ( loc++ ) = 10;}
              }
              kgDraw2f ( G , x , y ) ;
              xo = xx-x;yo = yy-y;
              last = 'd';
              xx = x;yy = y;
              count++; * ( loc++ ) = 10;
              break;
              case 3:
              kgMove2f ( G , xx , yy ) ;
              count++; * ( loc++ ) = 10;
              while ( ( key = kgRbrCursor ( G , & x , & y , & xx , & yy ) ) != '\r' ) {
                  if ( key == 'u' ) {
                      if ( count > 1 ) { loc--;count--;
                          uiupdate_intr ( G , count , dc->cmds ) ;
                  uiupdate_view ( G ) ;}}
                  else {kgDraw2f ( G , x , y ) ; xx = x;yy = y;
                  count++; * ( loc++ ) = 10;}
              }
              uidrarrow ( G , xx , yy , x , y , A_fac ) ;
              xo = xx-x;yo = yy-y;
              last = 'a';
              xx = x;yy = y;
              count++; * ( loc++ ) = 56;
              break;
              case 8:
              key = kgRectCursor ( G , & x , & y , & xx , & yy ) ;
              kgBoxFill ( G , xx , yy , x , y , dc->fil_color , 0 ) ;
              xo = xx-x;yo = yy-y;
              last = 'w';
              count++;
              * ( loc++ ) = 26;
              item = 4;
              break;
              case 4:
              while ( ( key = kgRectCursor ( G , & x , & y , & xx , & yy ) ) != '\r' ) {
                  if ( key == 'u' ) {
                      if ( count > 1 ) { loc--;count--;
                          uiupdate_intr ( G , count , dc->cmds ) ;
                  uiupdate_view ( G ) ;} }
                  else { kgMove2f ( G , xx , yy ) ;kgDraw2f \
                       ( G , xx , y ) , kgDraw2f ( G , x , y ) ;
                      kgDraw2f ( G , x , yy ) , kgDraw2f ( G , xx , yy ) ;
                  * ( loc++ ) = 50;count++;}
              }
              kgMove2f ( G , xx , yy ) ;kgDraw2f ( G , xx , \
              y ) , kgDraw2f ( G , x , y ) , kgDraw2f \
               ( G , x , yy ) , kgDraw2f ( G , xx , yy ) ;
              xo = xx-x;yo = yy-y;
              last = 'r';
              * ( loc++ ) = 50;count++;
              break;
              case 7:
              if ( count > 1 ) {
                  loc--;count--;
                  uiupdate_intr ( G , count , dc->cmds ) ;
                  uiupdate_view ( G ) ;
                  xx+= xo;yy+= yo;
              }
              break;
              case 5:
              ncr = i;
              key = kgCrossCursor ( G , & xtxt , & ytxt ) ;
              nch = get_intr_text ( parent , bf ) ;
              nch = strlen ( bf ) ;
              * ( bf+nch ) = '\0';
              kgMove2f ( G , xtxt , ytxt ) ;
              kgWriteText ( G , bf ) ;
              i = nch+1;
              * ( loc++ ) = 16+nch+1;count++;
              break;
              case 9:
              npoly = uipolygon_fill ( G , xpoly , ypoly , dc->fil_color ) ;
              {* ( loc++ ) = npoly;count++;};
              last = 'f';
              break;
              case 10:
//               *(loc++)=72;count++;  
              * ( loc++ ) = uiset_atribs ( G ) ;
              count++;
              if ( dc->A_size > 10 ) A_fac = ( float ) \
               ( dc->A_size/10 ) / ( dc->A_size%10 ) ;
              break;
              case 11:
              count++;
              * ( loc ) = uiProcess_arc ( G , & xx , & yy ) ;
              loc++;
              break;
              case 12:
              key = kgRbrCursor ( G , & x , & y , & xx , & yy ) ;
              uipa_gram ( G , xx , yy , x , y ) ;
              xo = xx-x;yo = yy-y;
              last = 'p';
              * ( loc++ ) = 50;count++;
              break;
              case 13:
              key = kgCrossCursor ( G , & x , & y ) ;
              kgMarker2f ( G , x , y ) ;
              * ( loc++ ) = 10;count++;
              break;
              case 14:
              key = kgCrossCursor ( G , & x , & y ) ;
              xx = xo+x;yy = yo+y;
              switch ( ( int ) last ) {
                  case 'd':
                  kgMove2f ( G , xx , yy ) ;kgDraw2f ( G , x , y ) ;
                  count++; * ( loc++ ) = 20;
                  break;
                  case 'a':
                  kgMove2f ( G , xx , yy ) ;
                  uidrarrow ( G , xx , yy , x , y , A_fac ) ;
                  count++; * ( loc++ ) = 66;
                  break;
                  case 'c':
                       /*move2f(xx,yy);circle(x,y,radi);   
                       count++; *(loc++) = 24;*/
                  break;
                  case 'w':
                  kgBoxFill ( G , xx , yy , x , y , dc->fil_color , 0 ) ;
                  count++; * ( loc++ ) = 26;
                  break;
                  case 'f':{
                      if ( npoly != 0 ) {
                          float dx , dy;
                          int np;
//                           np = (npoly-26)/8;
                          np = ( npoly-14 ) /8;
                          dx = x-xpoly [ 0 ] ;
                          dy = y-ypoly [ 0 ] ;
                          for ( i = 0;i < np;i++ ) {xpoly [ i ] += dx;ypoly [ i ] += dy;}
                          kgPolyFill ( G , np , xpoly , ypoly , 1L , dc->fil_color ) ;
//                           *(loc++) = npoly-12; count++;
                          * ( loc++ ) = npoly; count++;
                      }
                  }
                  break;
                  case 'r':
                  uirectgl ( G , xx , yy , x , y ) ;
                  count++; * ( loc++ ) = 50;
                  break;
                  case 'p':
                  uipa_gram ( G , xx , yy , x , y ) ;
                  count++; * ( loc++ ) = 50;
                  break;
                  case 'o':
                  uiborder ( G , xx , yy , x , y , dc->bod_width , dc->bod_color ) ;
                  count++; * ( loc++ ) = 104;
                  break;
              }
              xx = x;yy = y;
              break;
              case 15:
              while ( ( key = kgRectCursor ( G , & x , & y , & xx , & yy ) ) != '\r' ) {
                  if ( key == 'u' ) {
                      if ( count != 0 ) { loc--;count--;
                          uiupdate_intr ( G , count , dc->cmds ) ;
                  uiupdate_view ( G ) ;} }
                  else {
                      uiborder ( G , xx , yy , x , y , dc->bod_width , dc->bod_color ) ;
                  * ( loc++ ) = 104;count++;}
              }
              uiborder ( G , xx , yy , x , y , dc->bod_width , dc->bod_color ) ;
              xo = xx-x;yo = yy-y;
              last = 'o';
              * ( loc++ ) = 104;count++;
              break;
              case 16:
              * ( loc ) = uiProcess_arc_fill ( G , & xx , & yy , dc->fil_color ) ;
              loc++;
              count++;
              break;
              case 17:
              if ( MAG ) {
                  kgAntialiasingOn ( G , MAG-1 ) ;
                  kgReview ( G ) ;
              }
              free ( dc->cmds ) ;
              return;
          }
      }
      if ( MAG ) {
          kgAntialiasingOn ( G , MAG-1 ) ;
          kgReview ( G ) ;
      }
      free ( dc->cmds ) ;
      return;
  }
  void uiupdate_intr ( DIG *G , int count , unsigned int *loc ) {
      int i , lngth , inf;
      char ch;
      lngth = 0;
      for ( i = 0;i < count;i++ ) lngth += ( int ) ( * ( loc++ ) ) ;
      G->Byte = lngth;
  }
  int ginitdia ( void *parent , int xres , int yres ) {
      int ret = 1;
      DIALOG D;
      DIA d [ 2 ] ;
      DIG g0 = {
          'g' , 0 , 0 , xres , xres , NULL , /* pixmap info */
          0 , /* bkgr colour */
          NULL , /* void *initgraph ( int , void * ) */
          NULL , 0 , 0 /* *data */
      };
      d [ 0 ] .g = & g0;
      d [ 1 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 0; /* set to 1 for backup */
      D.bor_type = 0;
      D.df = -1;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = 30; /* Position of Dialog */
      D.yo = 30;
      D.xl = xres; /* Length of Dialog */
      D.yl = yres; /* Width of Dialog */
      D.Initfun = NULL; /* Width of Dialog */
      D.Deco = 1;
      D.DrawBkgr = 1;
      D.Bkpixmap = NULL;
      D.Callback = NULL;
      D.transparency = 0.0;
      D.Newwin = 1;
      D.Sticky = 0;
      D.Fixpos = 0;
      D.NoTaskBar = 0;
      D.Newwin = 0;
      D.parent = parent;
      D.StackPos = 1;
      D.Shapexpm = NULL;
      if ( parent == NULL ) D.Newwin = 1;
      kgDefaultGuiTheme ( & ( D.gc ) ) ;
      strcpy ( D.name , "Kulina Graphics" ) ;
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      return ret;
  }
  int uiTextMenu ( void *parent , int x1 , int y1 , \
  int items , char **promt , char ** field ) \
  {
      int ret = 1 , i , l = 0i , maxl;
      DIALOG D;
      DIA d [ 3 ] ;
      T_ELMT *e0;
      int butn2 = 1;
      e0 = ( T_ELMT * ) Malloc ( sizeof ( T_ELMT ) *items ) ;
      DIT t0 = {
      't' , 130 , 85 , 293 , 125 , 20 , 1 , 1 , e0 , 1 , 1 , NULL , NULL };
      t0.type = 0;
      char *titles4 [ ] = {
      "Okay" , "" , NULL };
      int sw4 [ 2 ] = {
      1 , 1 };
      void *xpm4 [ ] = {
      NULL , NULL , NULL };
      int bkgr4 [ ] = {
      -1 , -1 , -1 };
      char *butncode4 = NULL;
      DILN H4 = {
          'H' , 10 , 10 , 95 , 51 , 9 , 0 , 75 , 30 , 1 , 1 , & butn2 , sw4 , \
          titles4 , butncode4 , NULL , NULL , /* arg , Callbak */
          xpm4 , bkgr4 /* pointers to xpms and colors */
       , 2 , 0.2 };
      t0.ny = items;
      t0.x1 = 5;
      t0.y1 = 5;
      maxl = 0;
      for ( i = 0;i < items;i++ ) {
//     printf("%d: %s: %s\n",i,promt[i],field[i]);
          l = strlen ( promt [ i ] ) ;
          l+= get_t_item_size ( promt [ i ] ) ;
          if ( maxl < l ) maxl = l;
          e0 [ i ] .fmt = promt [ i ] ;e0 [ i ] .v = field [ i ] ;e0 [ i ] .sw = 1;
      }
//  printf("Inside uiTextMenu\n");
      maxl = maxl*9+50;
      t0.x2 = t0.x1+ ( maxl ) ;
      t0.y2 = t0.y1+items*30+10;
      d [ 0 ] .t = & t0;
      d [ 1 ] .H = & H4;
      d [ 2 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 2;
      D.df = -1;
      D.tw = 4;
      D.bw = 44;
      D.lw = 4;
      D.rw = 4;
      D.xo = x1; /* Position of Dialog */
      D.yo = y1;
      D.xl = maxl+15;
      D.yl = items*30+20+50; /* Width of Dialog */
      D.yl = t0.y2 -t0.y1+20+50; /* Width of Dialog */
      H4.x1 = ( D.xl ) /2 -40;
      H4.y1 = ( D.yl ) -40;
      H4.x2 = H4.x1+82;
      H4.y2 = H4.y1+37;
      D.Initfun = NULL; /* Width of Dialog */
      D.Deco = 1;
      D.DrawBkgr = 1;
      D.Bkpixmap = NULL;
      D.Callback = NULL;
      D.transparency = 0.0;
      D.Newwin = 0;
      D.Sticky = 0;
      D.Fixpos = 0;
      D.NoTaskBar = 0;
      D.parent = parent;
      D.StackPos = 1;
      D.Shapexpm = NULL;
      if ( parent == NULL ) D.Newwin = 1;
      strcpy ( D.name , "Text Menu" ) ;
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      free ( e0 ) ;
      return ret;
  }
  int get_no_of_lines ( File *fp ) {
      int n = 0;
      if ( fp == ( ( void * ) 0 ) ) return -1;
      n = Dcount ( fp->L ) ;
      return n;
  }
  int get_lines_in_file ( char ***m , File *fp ) {
      char *bf;
      char **M;
      int n , i;
      n = get_no_of_lines ( fp ) +1;
      if ( n <= 1 ) return 0;
      *m = ( char ** ) Malloc ( sizeof ( char * ) *n ) ;
      M = *m;
      Resetlink ( fp->L ) ;
      i = 0;
      while ( ( bf = ( char * ) Getrecord ( fp->L ) ) != NULL ) {M [ i++ ] = bf;}
      M [ n-1 ] = NULL;
      return n-1;
  }
  int uiShowFile ( File *fp , char *butns [ ] ) {
      int ret = 1;
      DIALOG D;
      DIA d [ 3 ] ;
      char **m;
      int l , width , item = 0 , nchars = 4 , lines = 6 , n;
      DIS s0 = {
          's' , 4 , 4 , 322 , 169 , 6 , NULL , NULL , NULL , \
          NULL , NULL /* *args , callback */
      };
      int butn1 = 1;
      int sw1 [ 100 ] = {
          1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , \
          1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , \
      1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 };
      char *titles1 [ ] = {
      " Okay" , NULL };
      void *xpm1 [ ] = {
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
          NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , \
      NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL , NULL };
      int bkgr1 [ ] = {
          -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , \
      -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 };
      char *butncode1 = NULL;
      DILN H1 = {
//    NULL,Show_Filehoribar1callback,/* arg, Callbak */
          'H' , 131 , 178 , 199 , 211 , 4 , 0 , 60 , 25 , 1 , 1 , & butn1 , sw1 , \
          NULL , butncode1 , NULL , NULL , xpm1 , bkgr1 /* pointers to xpms and colors */
       , 2 , 0.2 };
      n = get_lines_in_file ( & m , fp ) ;
      s0.menu = m;
      while ( m [ item ] != NULL ) {
          l = strlen ( m [ item ] ) ; if ( l > nchars ) nchars = l;item++;
      }
      if ( nchars > 130 ) nchars = 130;
      if ( lines < item ) lines = item;
      if ( lines > 20 ) lines = 20;
      width = nchars*10;
      d [ 0 ] .s = & s0;
      s0.size = lines;
      s0.x2 = s0.x1+width;
      s0.y2 = s0.y1+ ( lines+1 ) *20+18;
      l = 0;nchars = 0 , item = 0;
      while ( butns [ item ] != NULL ) {
          l = strlen ( butns [ item ] ) ; if ( l > nchars ) nchars = l;
          sw1 [ item ] = 1;item++;
      }
      H1.lngth = nchars*9+20;
      H1.nx = item;
      d [ 1 ] .H = & H1;
      d [ 2 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 2;
      D.df = 1;
      D.tw = 4;
      D.bw = 44;
      D.lw = 4;
      D.rw = 4;
      D.xo = 15; /* Position of Dialog */
      D.yo = 15;
      D.xl = s0.x2 +4;
      D.yl = s0.y2 +44;
      width = ( H1.y2-H1.y1 ) ;
      H1.y1 = D.yl - ( width ) -4;
      H1.y2 = H1.y1 + width;
      width = ( H1.lngth+H1.xgap ) *H1.nx+4;
      if ( D.xl < width ) D.xl = width+4;
      H1.x1 = ( D.xl-width ) *.5;
      H1.x2 = H1.x1+width;
      H1.titles = butns;
      D.Initfun = NULL;
      D.kbattn = 1; /* 1 for drawing keyborad attention */
      D.butattn = 1; /* 1 for drawing button attention */
      D.fullscreen = 0; /* 1 for for fullscreen mode */
      strcpy ( D.name , "UiDesigner ver 3.0" ) ; /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
      } /* end of fullscreen mode */
      D.Deco = 1;
      D.DrawBkgr = 1;
      D.Bkpixmap = NULL;
      D.Callback = NULL;
      D.transparency = 0.0;
      D.Newwin = 1;
      D.Sticky = 0;
      D.Fixpos = 0;
      D.NoTaskBar = 0;
      D.parent = NULL;
      D.StackPos = 1;
      D.Shapexpm = NULL;
      kgDefaultGuiTheme ( & ( D.gc ) ) ;
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      free ( m ) ;
      return ret;
  }
  void kgGetRootPosgbox1init ( int i , void *Tmp ) {
      struct _ScrnPos{ int xp;int yp;} *Spos;
      float xp , yp;
      DIALOG *D;
      DIG *G;
      D = ( DIALOG * ) Tmp;
      G = D->d [ i ] .g;
//  G->D = (void *)getParentDisplay(Tmp);
      G->D = D;
      Spos = G->data;
      xp = D->xl/2;
      yp = D->yl/2;
//  kgCrossCursor(G,&xp,&yp);
      kgCrossCursor ( G , & xp , & yp ) ;
//  printf("Gbox Pos: %d %d\n",(int)xp,(int)(D->yl-yp));
      Spos->xp = xp;
      Spos->yp = ( int ) ( D->yl-yp ) ;
      return ;
  }
  int kgGetRootPosinit ( void *Tmp ) {
      int ret = 0;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      return ret;
  }
  int kgGetRootPosCallBack ( void *Tmp , void *tmp ) {
      int ret = 1;
      float xp , yp;
      DIALOG *D;
      KBEVENT *kbe;
      DIG *G;
      D = ( DIALOG * ) Tmp;
      G = D->d [ 0 ] .g;
      xp = D->xl/2;
      yp = D->yl/2;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
          }
      }
      return ret;
  }
  int kgGetRootPos ( int *xp , int *yp ) {
      struct _ScrnPos{ int xp;int yp;} Spos;
      int ret = 1;
      int xres , yres;
      DIALOG D;
      DIA d [ 2 ] ;
      DIG g0 = {
          'g' , 0 , 0 , 717 , 494 , NULL , /* pixmap info */
          -1 , /* bkgr colour */
          kgGetRootPosgbox1init , /* void *initgraph ( int , void * ) */
          NULL , 0 , 0 /* *data */
      };
      d [ 0 ] .g = & g0;
      d [ 1 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 0;
      D.df = 0;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = 0; /* Position of Dialog */
      D.yo = 0;
      g0.data = ( void * ) & Spos;
      GetDisplaySize ( & xres , & yres ) ;
      D.xl = xres;
      D.yl = yres;
      D.Initfun = kgGetRootPosinit; /* init fuction for Dialog */
      D.kbattn = 0;; /* 1 for drawing keyborad attention */
      D.butattn = 0; /* 1 for drawing button attention */
      D.fullscreen = 1; /* 1 for for fullscreen mode */
      D.Deco = 0; /* 1 for Window Decorration */
      D.transparency = 1.000000; /* float 1.0 for full transparency */
      D.Newwin = 1; /* 1 for new window not yet implemented */
      D.DrawBkgr = 0; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 1; /* 1 for stickyness */
#if 0 
      D.Callback = kgGetRootPosCallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.Fixpos = 0; /* 1 for Fixing Position */
      D.NoTaskBar = 1; /* 1 for not showing in task bar*/
      D.StackPos = 0; /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/
      D.Shapexpm = NULL; /* PNG/jpeg file for window shape;
      Black color will not be drawn */
      D.parent = NULL; /* 1 for not showing in task bar*/
      D.pt = NULL; /* any data to be passed by user*/
      strcpy ( D.name , "Kulina Designer ver 1.0" ) ;
           /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
          g0.x2 = xres;
          g0.y2 = yres;
      } /* end of fullscreen mode */
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
//  kgColorTheme(&D,230,230,230);    /*  set colors for gui*/
//  ModifykgGetRootPosGc(&(D.gc));    /*  set colors for gui*/
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
//  printf("Spos: %d %d\n",Spos.xp,Spos.yp);
      *xp = Spos.xp;
      *yp = Spos.yp;
      return ret;
  }
  void kgGetRootRectgbox1init ( int i , void *Tmp ) {
      struct _ScrnRect{ int x1 , y1 , x2 , y2;} *Srect;
      float x1 , y1 , x2 , y2;
      DIALOG *D;
      DIG *G;
      D = ( DIALOG * ) Tmp;
      G = D->d [ i ] .g;
//  G->D = (void *)getParentDisplay(Tmp);
      G->D = D;
      Srect = G->data;
      x1 = D->xl/2;
      y1 = D->yl/2;
//  kgCrossCursor(G,&x1,&y1);
      kgCrossCursor ( G , & x1 , & y1 ) ;
      x2 = x1+10;
      y2 = y1+10;
      kgRectCursor ( G , & x2 , & y2 , & x1 , & y1 ) ;
//  printf("Gbox Pos: %d %d\n",(int)xp,(int)(D->yl-yp));
      Srect->x1 = x1;
      Srect->y1 = ( int ) ( D->yl-y1 ) ;
      Srect->x2 = x2;
      Srect->y2 = ( int ) ( D->yl-y2 ) ;
      return ;
  }
  int kgGetRootRectinit ( void *Tmp ) {
      int ret = 0;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      return ret;
  }
  int kgGetRootRectCallBack ( void *Tmp , void *tmp ) {
      int ret = 1;
      float xp , yp;
      DIALOG *D;
      KBEVENT *kbe;
      DIG *G;
      D = ( DIALOG * ) Tmp;
      G = D->d [ 0 ] .g;
      xp = D->xl/2;
      yp = D->yl/2;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
          }
      }
      return ret;
  }
  int kgGetRootRect ( int *x1 , int *y1 , int *x2 , int *y2 ) {
      struct _ScrnRect{ int x1 , y1 , x2 , y2;} Srect;
      int ret = 1;
      int xres , yres;
      DIALOG D;
      DIA d [ 2 ] ;
      DIG g0 = {
          'g' , 0 , 0 , 717 , 494 , NULL , /* pixmap info */
          -1 , /* bkgr colour */
          kgGetRootRectgbox1init , /* void *initgraph ( int , void * ) */
          NULL , 0 , 0 /* *data */
      };
      d [ 0 ] .g = & g0;
      d [ 1 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 0;
      D.df = 0;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = 0; /* Position of Dialog */
      D.yo = 0;
      g0.data = ( void * ) & Srect;
      GetDisplaySize ( & xres , & yres ) ;
      D.xl = xres;
      D.yl = yres;
      D.Initfun = kgGetRootRectinit; /* init fuction for Dialog */
      D.kbattn = 0;; /* 1 for drawing keyborad attention */
      D.butattn = 0; /* 1 for drawing button attention */
      D.fullscreen = 1; /* 1 for for fullscreen mode */
      D.Deco = 0; /* 1 for Window Decorration */
      D.transparency = 1.000000; /* float 1.0 for full transparency */
      D.Newwin = 1; /* 1 for new window not yet implemented */
      D.DrawBkgr = 0; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 1; /* 1 for stickyness */
#if 0 
      D.Callback = kgGetRootRectCallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.Fixpos = 0; /* 1 for Fixing Position */
      D.NoTaskBar = 1; /* 1 for not showing in task bar*/
      D.StackPos = 1; /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/
      D.Shapexpm = NULL; /* PNG/jpeg file for window shape;
      Black color will not be drawn */
      D.parent = NULL; /* 1 for not showing in task bar*/
      D.pt = NULL; /* any data to be passed by user*/
      strcpy ( D.name , "Kulina Designer ver 1.0" ) ;
           /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
          g0.x2 = xres;
          g0.y2 = yres;
      } /* end of fullscreen mode */
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
//  kgColorTheme(&D,230,230,230);    /*  set colors for gui*/
//  ModifykgGetRootPosGc(&(D.gc));    /*  set colors for gui*/
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
//  printf("Spos: %d %d\n",Spos.xp,Spos.yp);
      *x1 = Srect.x1;
      *y1 = Srect.y1;
      *x2 = Srect.x2;
      *y2 = Srect.y2;
      return ret;
  }
  void * kgUndoImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 800 ] , y [ 800 ] , R;
      int r , g , b , np = 120 , i , sign = 1;
      double Dang , dang , ang , ri , ro , ss , cs , dl , cl , cr , size1;
      count = 2*np+3;
      count1 = count-1;
      Dang = 315.0/ ( np-1 ) ;
      dang = Dang*3.14159265/180.0;
      R = size*0.501;
      size1 = size*0.1;
      ri = size*0.5-2*size1;
      ro = ri+size1;
      fid = kgInitImage ( size , size , RESIZE ) ;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      ang = ( 1+0.25 ) *3.14159265;
      for ( i = 0;i < np;i++ ) {
          ss = sin ( ang ) ;
          cs = cos ( ang ) ;
          x [ i ] = ri*cs;
          y [ i ] = ri*ss;
          x [ count1-i ] = ro*cs;
          y [ count1-i ] = ro*ss;
          ang += ( sign*dang ) ;
      }
      dl = size1*0.5;
      x [ np ] = ( ri-dl ) *cs;
      y [ np ] = ( ri-dl ) *ss;
      x [ np+2 ] = ( ro+dl ) *cs;
      y [ np+2 ] = ( ro+dl ) *ss;
      ang -= ( sign*dang ) ;
      cl = sqrt ( 3.0 ) * ( 2*dl+size1 ) *0.5;
      cr = sqrt ( cl*cl+ ( ri+ro ) * ( ri+ro ) *0.25 ) ;
      dang = atan ( cl*2.0/ ( ri+ro ) ) ;
      ang += ( sign*dang ) ;
      ss = sin ( ang ) ;
      cs = cos ( ang ) ;
//   cr = (ri+ro)*0.5;
      x [ np+1 ] = ( cr ) *cs;
      y [ np+1 ] = ( cr ) *ss;
      kgPolyFill ( fid , count , x , y , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgRedoImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 800 ] , y [ 800 ] , R;
      int r , g , b , np = 120 , i , sign = -1;
      double Dang , dang , ang , ri , ro , ss , cs , dl , cl , cr , size1;
      count = 2*np+3;
      count1 = count-1;
      Dang = 315.0/ ( np-1 ) ;
      dang = Dang*3.14159265/180.0;
      R = size*0.501;
      size1 = size*0.1;
      ri = size*0.5-2*size1;
      ro = ri+size1;
      fid = kgInitImage ( size , size , RESIZE ) ;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      ang = ( -0.25 ) *3.14159265;
      for ( i = 0;i < np;i++ ) {
          ss = sin ( ang ) ;
          cs = cos ( ang ) ;
          x [ i ] = ri*cs;
          y [ i ] = ri*ss;
          x [ count1-i ] = ro*cs;
          y [ count1-i ] = ro*ss;
          ang += ( sign*dang ) ;
      }
      dl = size1*0.5;
      x [ np ] = ( ri-dl ) *cs;
      y [ np ] = ( ri-dl ) *ss;
      x [ np+2 ] = ( ro+dl ) *cs;
      y [ np+2 ] = ( ro+dl ) *ss;
      ang -= ( sign*dang ) ;
      cl = sqrt ( 3.0 ) * ( 2*dl+size1 ) *0.5;
      cr = sqrt ( cl*cl+ ( ri+ro ) * ( ri+ro ) *0.25 ) ;
      dang = atan ( cl*2.0/ ( ri+ro ) ) ;
      ang += ( sign*dang ) ;
      ss = sin ( ang ) ;
      cs = cos ( ang ) ;
//   cr = (ri+ro)*0.5;
      x [ np+1 ] = ( cr ) *cs;
      y [ np+1 ] = ( cr ) *ss;
      kgPolyFill ( fid , count , x , y , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgRightdirImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 8 ] , y [ 8 ] , R;
      double size1;
      count = 7;
      count1 = count-1;
      R = size*0.501;
      fid = kgInitImage ( size , size , RESIZE ) ;
      size1 = size*0.4;
      size = ( size ) *0.475;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      x [ 0 ] = -size;
      y [ 0 ] = size1*0.5;
      x [ 1 ] = size-size1;
      y [ 1 ] = size1*0.5;
      x [ 2 ] = size-size1;
      y [ 2 ] = size1;
      x [ 3 ] = size;
      y [ 3 ] = 0.0;
      x [ 4 ] = size-size1;
      y [ 4 ] = -size1;
      x [ 5 ] = size-size1;
      y [ 5 ] = -size1*0.5;
      x [ 6 ] = -size;
      y [ 6 ] = -size1*0.5;
      kgPolyFill ( fid , count , x , y , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgLeftdirImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 8 ] , y [ 8 ] , R;
      double size1;
      count = 7;
      count1 = count-1;
      R = size*0.501;
      fid = kgInitImage ( size , size , RESIZE ) ;
      size1 = size*0.4;
      size = ( size ) *0.475;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      x [ 0 ] = size;
      y [ 0 ] = size1*0.5;
      x [ 1 ] = - ( size-size1 ) ;
      y [ 1 ] = size1*0.5;
      x [ 2 ] = - ( size-size1 ) ;
      y [ 2 ] = size1;
      x [ 3 ] = - ( size ) ;
      y [ 3 ] = 0.0;
      x [ 4 ] = - ( size-size1 ) ;
      y [ 4 ] = -size1;
      x [ 5 ] = - ( size-size1 ) ;
      y [ 5 ] = -size1*0.5;
      x [ 6 ] = size;
      y [ 6 ] = -size1*0.5;
      kgPolyFill ( fid , count , x , y , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgUpdirImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 8 ] , y [ 8 ] , R;
      double size1;
      count = 7;
      count1 = count-1;
      R = size*0.501;
      fid = kgInitImage ( size , size , RESIZE ) ;
      size1 = size*0.4;
      size = ( size ) *0.475;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      x [ 0 ] = -size;
      y [ 0 ] = size1*0.5;
      x [ 1 ] = size-size1;
      y [ 1 ] = size1*0.5;
      x [ 2 ] = size-size1;
      y [ 2 ] = size1;
      x [ 3 ] = size;
      y [ 3 ] = 0.0;
      x [ 4 ] = size-size1;
      y [ 4 ] = -size1;
      x [ 5 ] = size-size1;
      y [ 5 ] = -size1*0.5;
      x [ 6 ] = -size;
      y [ 6 ] = -size1*0.5;
      kgPolyFill ( fid , count , y , x , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgDowndirImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 8 ] , y [ 8 ] , R;
      double size1;
      count = 7;
      count1 = count-1;
      R = size*0.501;
      fid = kgInitImage ( size , size , RESIZE ) ;
      size1 = size*0.4;
      size = ( size ) *0.475;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      x [ 0 ] = size;
      y [ 0 ] = size1*0.5;
      x [ 1 ] = - ( size-size1 ) ;
      y [ 1 ] = size1*0.5;
      x [ 2 ] = - ( size-size1 ) ;
      y [ 2 ] = size1;
      x [ 3 ] = - ( size ) ;
      y [ 3 ] = 0.0;
      x [ 4 ] = - ( size-size1 ) ;
      y [ 4 ] = -size1;
      x [ 5 ] = - ( size-size1 ) ;
      y [ 5 ] = -size1*0.5;
      x [ 6 ] = size;
      y [ 6 ] = -size1*0.5;
      kgPolyFill ( fid , count , y , x , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgRightImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 4 ] , y [ 4 ] , R;
      double size1;
      count = 3;
      count1 = count-1;
      R = size*0.501;
      fid = kgInitImage ( size , size , RESIZE ) ;
      size1 = size*0.4;
      size = ( size ) *0.3;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      x [ 0 ] = -size;
      y [ 0 ] = size;
      x [ 1 ] = size;
      y [ 1 ] = 0.0;
      x [ 2 ] = -size;
      y [ 2 ] = -size;
      kgPolyFill ( fid , count , x , y , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgLeftImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 4 ] , y [ 4 ] , R;
      double size1;
      count = 3;
      count1 = count-1;
      R = size*0.501;
      fid = kgInitImage ( size , size , RESIZE ) ;
      size1 = size*0.4;
      size = ( size ) *0.3;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      x [ 0 ] = size;
      y [ 0 ] = size;
      x [ 1 ] = -size;
      y [ 1 ] = 0.0;
      x [ 2 ] = size;
      y [ 2 ] = -size;
      kgPolyFill ( fid , count , x , y , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgUpImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 4 ] , y [ 4 ] , R;
      double size1;
      count = 3;
      count1 = count-1;
      R = size*0.501;
      fid = kgInitImage ( size , size , RESIZE ) ;
      size1 = size*0.4;
      size = ( size ) *0.3;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      x [ 0 ] = -size;
      y [ 0 ] = size;
      x [ 1 ] = size;
      y [ 1 ] = 0.0;
      x [ 2 ] = -size;
      y [ 2 ] = -size;
      kgPolyFill ( fid , count , y , x , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgDownImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 4 ] , y [ 4 ] , R;
      double size1;
      count = 3;
      count1 = count-1;
      R = size*0.501;
      fid = kgInitImage ( size , size , RESIZE ) ;
      size1 = size*0.4;
      size = ( size ) *0.3;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      x [ 0 ] = size;
      y [ 0 ] = size;
      x [ 1 ] = -size;
      y [ 1 ] = 0.0;
      x [ 2 ] = size;
      y [ 2 ] = -size;
      kgPolyFill ( fid , count , y , x , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgGoback1Image ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 8 ] , y [ 8 ] , R;
      double size1;
      count = 7;
      count1 = count-1;
      R = size*0.501;
      fid = kgInitImage ( size , size , RESIZE ) ;
      size1 = size*0.20;
      size = ( size ) *0.4;
      kgUserFrame ( fid , -R , -R*0.75 , +R , +R*1.25 ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      x [ 0 ] = size;
      y [ 0 ] = size1*0.5;
      x [ 1 ] = - ( size-size1 ) ;
      y [ 1 ] = size1*0.5;
      x [ 2 ] = - ( size-size1 ) ;
      y [ 2 ] = size1;
      x [ 3 ] = - ( size ) ;
      y [ 3 ] = 0.0;
      x [ 4 ] = - ( size-size1 ) ;
      y [ 4 ] = -size1;
      x [ 5 ] = - ( size-size1 ) ;
      y [ 5 ] = -size1*0.5;
      x [ 6 ] = size;
      y [ 6 ] = -size1*0.5;
      kgPolyFill ( fid , count , x , y , 0 , 1001 ) ;
      x [ 0 ] = size;
      y [ 0 ] = size;
      x [ 1 ] = ( size-size1 ) ;
      y [ 1 ] = size;
      x [ 2 ] = ( size-size1 ) ;
      y [ 2 ] = -size1*0.5;
      x [ 3 ] = ( size ) ;
      y [ 3 ] = -size1*0.5;
      kgPolyFill ( fid , 4 , x , y , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgGobackImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 8 ] , y [ 8 ] , R;
      int r , g , b , np = 8 , i , sign = -1;
      double Dang , dang , ang , ri , ro , ss , cs , dl , cl , cr , size1;
      count = 2*np+3;
      count1 = count-1;
      Dang = 90.0/ ( np-1 ) ;
      dang = Dang*3.14159265/180.0;
      R = size*0.48;
      dl = R* sin ( 0.125*3.14159265 ) ;
      size1 = R*cos ( 0.125*3.14159265 ) ;
      fid = kgInitImage ( size , size , RESIZE ) ;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      ang = 0.0;
      x [ 0 ] = -size1;
      y [ 0 ] = dl;
      x [ 1 ] = -dl;
      y [ 1 ] = size1;
      x [ 2 ] = dl;
      y [ 2 ] = size1;
      x [ 3 ] = size1;
      y [ 3 ] = dl;
      x [ 4 ] = size1;
      y [ 4 ] = -dl;
      x [ 5 ] = dl;
      y [ 5 ] = -size1;
      x [ 6 ] = -dl;
      y [ 6 ] = -size1;
      x [ 7 ] = -size1;
      y [ 7 ] = -dl;
      kgPolyFill ( fid , np , x , y , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgPowerdownImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 800 ] , y [ 800 ] , R;
      int r , g , b , np = 120 , i , sign = 1;
      double Dang , dang , ang , ri , ro , ss , cs , dl , cl , cr , size1;
      count = 2*np;
      count1 = count-1;
      Dang = 300.0/ ( np-1 ) ;
      dang = Dang*3.14159265/180.0;
      R = size*0.501;
      fid = kgInitImage ( size , size , RESIZE ) ;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      size *= 0.8;
      size1 = size*0.1;
      if ( size1 > 3 ) size1 = 3;
      if ( size1 < 2 ) size1 = 2;
      ri = size*0.5-2*size1;
      ro = ri+size1;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      ang = 3.14159265*0.5+3.14159265*30/180;
      for ( i = 0;i < np;i++ ) {
          ss = sin ( ang ) ;
          cs = cos ( ang ) ;
          x [ i ] = ri*cs;
          y [ i ] = ri*ss;
          x [ count1-i ] = ro*cs;
          y [ count1-i ] = ro*ss;
          ang += ( sign*dang ) ;
      }
      kgPolyFill ( fid , count , x , y , 0 , 1001 ) ;
      x [ 0 ] = -size1*0.5;
      y [ 0 ] = size*0.4+size1;
      x [ 1 ] = size1*0.5;
      y [ 1 ] = size*0.4+size1;
      x [ 2 ] = size1*0.5;
      y [ 2 ] = size1*0.2;
      x [ 3 ] = -size1*0.5;
      y [ 3 ] = size1*0.2;
      kgPolyFill ( fid , 4 , x , y , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void * kgHomeImage ( int size , int red , int green , int blue ) {
      void *fid , *img = NULL;
      int count , count1;
      float x [ 11 ] , y [ 11 ] , R;
      double size1;
      count = 11;
      count1 = count-1;
      R = size*0.501;
      fid = kgInitImage ( size , size , RESIZE ) ;
      size1 = size*0.4;
      size = ( size ) *0.5;
      kgUserFrame ( fid , -R , -R , +R , +R ) ;
      kgChangeColor ( fid , 1001 , red , green , blue ) ;
      x [ 0 ] = size1*0.85;
      y [ 0 ] = -size;
      x [ 1 ] = size1*0.85;
      y [ 1 ] = size-size1;
      x [ 2 ] = size;
      y [ 2 ] = size-size1;
      x [ 3 ] = 0.0;
      y [ 3 ] = size;
      x [ 4 ] = -size;
      y [ 4 ] = size-size1;
      x [ 5 ] = -size1*0.85;
      y [ 5 ] = size-size1;
      x [ 6 ] = -size1*0.85;
      y [ 6 ] = -size;
      x [ 7 ] = -size1*0.3;
      y [ 7 ] = -size;
      x [ 8 ] = -size1*0.3;
      y [ 8 ] = size-size1;
      x [ 9 ] = size1*0.3;
      y [ 9 ] = size-size1;
      x [ 10 ] = size1*0.3;
      y [ 10 ] = -size;
      kgPolyFill ( fid , count , x , y , 0 , 1001 ) ;
      x [ 0 ] = -size*0.8;
      y [ 0 ] = -size;
      x [ 1 ] = -size*0.8;
      y [ 1 ] = size-2.05*size1;
      x [ 2 ] = size*0.8;
      y [ 2 ] = size-2.05*size1;
      x [ 3 ] = size*0.8;
      y [ 3 ] = -size;
      kgPolyFill ( fid , 4 , x , y , 0 , 1001 ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  typedef struct _mkThumb{
      char flname [ 200 ] ;
      GMIMG *thImg;
      int size;
  } MKTHUMB;
  void *uiMakeThumbNail ( void *Tmp ) {
      GMIMG *img;
      MKTHUMB * pt;
      pt = ( MKTHUMB * ) Tmp;
      img = pt->thImg;
      if ( img != NULL ) {
          pt->thImg = ( GMIMG * ) uiThumbnailgmImage ( img , pt->size , pt->size ) ;
          pt->thImg->image_width = img->image_width;
          pt->thImg->image_height = img->image_height;
          strcpy ( pt->thImg->flname , pt->flname ) ;
          uiFreeImage ( img ) ;
      }
      else pt->thImg = NULL;
      return NULL;
  }
  void **uiGetThumbnails_o ( char *Dir , int size ) {
      GMIMG **thImgs = NULL;
      char **flnames , name [ 500 ] ;
      int i , k = 0;
      char *pt;
      GMIMG *img;
      flnames = kgFileMenu ( Dir , "*.png *.jpg *.jpeg *.JPG" ) ;
      if ( flnames != NULL ) {
          i = 0;while ( flnames [ i ] != NULL ) i++;
//   if(i> 0) {
          {
              thImgs = ( GMIMG ** ) Malloc ( sizeof ( GMIMG * ) * ( i+1 ) ) ;
              for ( k = 0;k <= i;k++ ) thImgs [ k ] = NULL;
              i = 0;
              k = 0;
              while ( flnames [ i ] != NULL ) {
                  strcpy ( name , Dir ) ;
                  strcat ( name , "/" ) ;
                  strcat ( name , flnames [ i ] ) ;
                  img = ( GMIMG * ) uiGetgmImage ( name ) ;
//       printf("%s \n",flnames[i]);
//       if(img==NULL) printf("%s is NULL\n",flnames[i]);
                  if ( img != NULL ) {
                      thImgs [ k ] = ( GMIMG * ) uiThumbnailgmImage ( img , size , size ) ;
#if 0
                      sprintf ( name , "(%5d,%5d): %s" , img->image_width , \
                      img->image_height , flnames [ i ] ) ;
                      strcpy ( thImgs [ k ]->flname , name ) ;
#else
                      if ( ( thImgs [ k ]->image_width > size ) \
                       || ( thImgs [ k ]->image_height > size ) ) {
                          printf ( "%s: %d:%d %d:%d\n" , flnames [ i ] , thImgs [ k ]->image_width , \
                          thImgs [ k ]->image_height , img->image_width , img->image_height ) ;
                      }
                      thImgs [ k ]->image_width = img->image_width;
                      thImgs [ k ]->image_height = img->image_height;
//         thImgs[k]->bkgrclr=15;
                      strcpy ( thImgs [ k ]->flname , flnames [ i ] ) ;
#endif
                      uiFreeImage ( img ) ;
                      k++;
                  }
                  i++;
              }
              i = 0;while ( flnames [ i ] != NULL ) free ( flnames [ i++ ] ) ;
          }
          free ( flnames ) ;
      }
//  printf("K= %d\n",k);
      return ( void ** ) thImgs;
  }
  void **uiGetThumbnails ( char *Dir , int size ) {
      GMIMG **thImgs = NULL;
      char **flnames , name [ 500 ] ;
      int i , k = 0 , n;
      MKTHUMB *pt;
      GMIMG *img;
      void *ThInfo;
      flnames = kgFileMenu ( Dir , "*.png *.jpg *.jpeg *.JPG" ) ;
      if ( flnames != NULL ) {
          i = 0;while ( flnames [ i ] != NULL ) i++;
          n = i;
//   if(i> 0) {
          {
              thImgs = ( GMIMG ** ) Malloc ( sizeof ( GMIMG * ) * ( n+1 ) ) ;
              if ( thImgs == NULL ) {
                  fprintf ( stderr , "Malloc failed\n" ) ;
                  exit ( 0 ) ;
              }
              pt = ( MKTHUMB * ) Malloc ( sizeof ( MKTHUMB ) *n ) ;
              if ( pt == NULL ) {
                  fprintf ( stderr , "Malloc failed\n" ) ;
                  exit ( 0 ) ;
              }
              for ( k = 0;k <= n;k++ ) {
                  thImgs [ k ] = NULL;
              }
              ThInfo = OpenThreads ( getCores ( ) ) ;
//     ThInfo = OpenThreads(1);
              k = 0;
              while ( flnames [ k ] != NULL ) {
                  strcpy ( name , Dir ) ;
                  strcat ( name , "/" ) ;
                  strcat ( name , flnames [ k ] ) ;
                  strcpy ( pt [ k ] .flname , flnames [ k ] ) ;
                  img = ( GMIMG * ) uiGetgmImage ( name ) ;
                  if ( img != NULL ) {
                      pt [ k ] .thImg = img;
                  }
                  else pt [ k ] .thImg = NULL;
                  pt [ k ] .size = size;
//       uiMakeThumbNail((void *)(pt+k));
#ifdef D_TH
                  DoInAnyThread ( ThInfo , uiMakeThumbNail , pt+k ) ;
#else
                  uiMakeThumbNail ( pt+k ) ;
#endif
                  k++;
              }
              WaitThreads ( ThInfo ) ;
              CloseThreads ( ThInfo ) ;
              i = 0;
              for ( k = 0;k < n;k++ ) {
                  if ( pt [ k ] .thImg != NULL ) thImgs [ i++ ] = pt [ k ] .thImg;
              }
              i = 0;while ( flnames [ i ] != NULL ) free ( flnames [ i++ ] ) ;
              free ( pt ) ;
          }
          free ( flnames ) ;
      }
//  printf("K= %d\n",k);
      return ( void ** ) thImgs;
  }
  char **uiGetMenu ( void ** Thumbnails ) {
      int i , no;
      GMIMG *img;
      char **menu = NULL;
      if ( Thumbnails != NULL ) {
          i = 0;
          while ( Thumbnails [ i ] != NULL ) {
              i++;
          }
          no = i+1;
          menu = ( char ** ) Malloc ( sizeof ( char * ) *no ) ;
          if ( menu == NULL ) {fprintf ( stderr , "Failed to alloc memory: uiGetMenu\n" ) ;
              
          exit ( 1 ) ;}
          menu [ no-1 ] = NULL;
          for ( i = 0;i < ( no-1 ) ;i++ ) {
              img = ( GMIMG * ) Thumbnails [ i ] ;
              menu [ i ] = img->flname;
          }
      }
      return menu;
  }
  int *uiGetSwitch ( void ** Thumbnails ) {
      int i , no;
      GMIMG *img;
      int *sw = NULL;
      if ( Thumbnails != NULL ) {
          i = 0;
          while ( Thumbnails [ i ] != NULL ) {
              i++;
          }
          no = i+1;
          sw = ( int * ) Malloc ( sizeof ( int ) *no ) ;
          if ( sw == NULL ) {fprintf ( stderr , "Failed to alloc memory: uiGetMenu\n" ) ;
              
          exit ( 1 ) ;}
          for ( i = 0;i < ( no-1 ) ;i++ ) {
              sw [ i ] = 0;
          }
      }
      return sw;
  }
  THUMBNAILS uiMakeThumbNails_o ( char *dir , int size ) {
      THUMBNAILS tb;
      tb.xpms = uiGetThumbnails ( dir , size ) ;
      tb.menu = uiGetMenu ( tb.xpms ) ;
      tb.sw = uiGetSwitch ( tb.xpms ) ;
      return tb;
  }
  void uiFreeThumbNails_o ( THUMBNAILS *tb ) {
      int i;
      void **xpms;
      if ( tb->menu != NULL ) free ( tb->menu ) ;
      if ( tb->sw != NULL ) free ( tb->sw ) ;
      if ( tb->xpms != NULL ) {
          xpms = tb->xpms;
          i = 0;
          while ( xpms [ i ] != NULL ) uiFreeImage ( xpms [ i++ ] ) ;
          free ( xpms ) ;
      }
      tb->xpms = NULL;
      tb->menu = NULL;
      tb->sw = NULL;
  }
  ThumbNail ** uiMakeThumbNails ( char *dir , int size ) {
      ThumbNail **tb = NULL;
      void **xpms;
      GMIMG *img;
      int i , n;
      xpms = uiGetThumbnails ( dir , size ) ;
      if ( xpms != NULL ) {
          i = 0; while ( xpms [ i++ ] != NULL ) ;
          n = i;
          tb = ( ThumbNail ** ) Malloc ( sizeof ( ThumbNail* ) *n ) ;
          tb [ n-1 ] = NULL;
          for ( i = 0;i < ( n-1 ) ;i++ ) {
              tb [ i ] = ( ThumbNail * ) Malloc ( sizeof ( ThumbNail ) ) ;
              img = xpms [ i ] ;
              tb [ i ]->img = img;
              tb [ i ]->name = ( char * ) Malloc ( strlen ( img->flname ) +1 ) ;
              strcpy ( tb [ i ]->name , img->flname ) ;
              tb [ i ]->sw = 0;
              tb [ i ]->id = i;
              tb [ i ]->state = 1;
          }
          free ( xpms ) ;
      }
      return tb;
  }
  ThumbNail ** kgMakeThumbNails ( char *dir , int size ) {
      return uiMakeThumbNails ( dir , size ) ;
  }
  ThumbNail ** uiFolderThumbNails ( char *dir ) {
      ThumbNail **tb = NULL;
      char **m;
      int i , n;
      m = kgFolderMenu ( dir ) ;
      if ( m != NULL ) {
          i = 0; while ( m [ i++ ] != NULL ) ;
          n = i;
          tb = ( ThumbNail ** ) Malloc ( sizeof ( ThumbNail* ) *n ) ;
          tb [ n-1 ] = NULL;
          for ( i = 0;i < ( n-1 ) ;i++ ) {
              tb [ i ] = ( ThumbNail * ) Malloc ( sizeof ( ThumbNail ) ) ;
              tb [ i ]->img = NULL;
              tb [ i ]->name = m [ i ] ;
              tb [ i ]->sw = 0;
              tb [ i ]->id = i;
              tb [ i ]->state = 1;
          }
          free ( m ) ;
      }
      return tb;
  }
  ThumbNail ** kgFolderThumbNails ( char *dir ) {
      return uiFolderThumbNails ( dir ) ;
  }
  ThumbNail ** uiFileThumbNails ( char *dir , char *filter ) {
      ThumbNail **tb = NULL;
      char **m;
      int i , n;
      m = kgFileMenu ( dir , filter ) ;
      if ( m != NULL ) {
          i = 0; while ( m [ i++ ] != NULL ) ;
          n = i;
          tb = ( ThumbNail ** ) Malloc ( sizeof ( ThumbNail* ) *n ) ;
          tb [ n-1 ] = NULL;
          for ( i = 0;i < ( n-1 ) ;i++ ) {
              tb [ i ] = ( ThumbNail * ) Malloc ( sizeof ( ThumbNail ) ) ;
              tb [ i ]->img = NULL;
              tb [ i ]->name = m [ i ] ;
              tb [ i ]->sw = 0;
              tb [ i ]->id = i;
              tb [ i ]->state = 1;
          }
          free ( m ) ;
      }
      return tb;
  }
  ThumbNail ** kgFileThumbNails ( char *dir , char *filter ) {
      return uiFileThumbNails ( dir , filter ) ;
  }
  void uiFreeThumbNails ( ThumbNail **tb ) {
      int i;
      void **xpms;
      if ( tb != NULL ) {
          i = 0;
          while ( tb [ i ] != NULL ) {
              if ( tb [ i ]->img != NULL ) uiFreeImage ( tb [ i ]->img ) ;
              if ( tb [ i ]->name != NULL ) free ( tb [ i ]->name ) ;
              free ( tb [ i ] ) ;
              i++;
          }
          free ( tb ) ;
      }
  }
  void kgFreeThumbNails ( ThumbNail **tb ) {
      uiFreeThumbNails ( tb ) ;
  }
#include <pthread.h>
#include <math.h>
  int kgBusyinit ( void *Tmp ) {
      void *img = NULL , *imgbk = NULL , *imgr = NULL;
      float ang = 0.0 , dang = 5.0 , r = 15. , xo , yo;
      double rang;
      int pipe;
      int ret = 1;
      int count = 0;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      DIG *G;
      int fillclr;
      fillclr = ( D->gc ) .fill_clr;
      dang = dang*3.14159265/180.0;
      rang = count*dang;
      xo = r*cos ( rang ) ;
      yo = r*sin ( rang ) ;
      pipe = * ( ( int * ) ( D->pt ) +0 ) ;
      G = kgInitImage ( D->xl , D->yl , RESIZE ) ;
#if 1
      kgUserFrame ( G , -25. , -25. , 25. , 25. ) ;
      kgRoundedRectangleRing3 ( G , 1. , -1. , 36. , \
      36 , 0. , 0. , 0. , 0.5 , 4. ) ;
      img = kgGetResizedImage ( G ) ;
      kgCloseImage ( G ) ;
      kgImage ( D , img , D->xo , D->yo , D->xl , D->yl , 0. , 1.0 ) ;
#endif
      imgbk = kgGetBackground ( Tmp , D->xo , D->yo , D->xo+D->xl , D->yo+D->yl ) ;
      kgFreeImage ( img ) ;
      while ( ! kgThreadWaitPipe ( pipe , 0 , 40000 ) ) {
          G = kgInitImage ( D->xl , D->yl , RESIZE ) ;
          kgUserFrame ( G , -25. , -25. , 25. , 25. ) ;
          kgChangeColor ( G , 1002 , 0 , 0 , 0 ) ;
//    kgRoundedRectangleRing2(G,0.,0.,36.,36,0.,0.,0.,0.5,4.);
          rang = count*dang;
          count = ( count+1 ) %72;
          xo = r*cos ( rang ) ;
          yo = r*sin ( rang ) ;
          kgChangeColor ( G , 1001 , 255 , 255 , 225 ) ;
          kgArcFill ( G , xo , yo , 6.0 , 0. , 360.0 , 0 , 1001 ) ;
          img = kgGetResizedImage ( G ) ;
          kgCloseImage ( G ) ;
          kgImage ( D , imgbk , D->xo , D->yo , D->xl , D->yl , 0. , 1.0 ) ;
          kgImage ( D , img , D->xo , D->yo , D->xl , D->yl , 0. , 1.0 ) ;
          kgUpdateOn ( D ) ;
          uiFreeImage ( img ) ;
      }
      uiFreeImage ( imgbk ) ;
      return ret;
  }
  int kgBusyCallBack ( void *Tmp , void *tmp ) {
      int ret = 0;
      int *pipe;
      DIALOG *D;
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
          }
      }
      return ret;
  }
  void * kgBusy ( void *dummy ) {
      struct _doubleptr{void *parent;int pipe [ 2 ] ;
      pthread_t Pth;int xo;int yo;} *dptr;
      void *parent;
      int ret = 1;
      DIALOG D;
      DIA d [ 1 ] ;
      dptr = ( struct _doubleptr* ) dummy;
      parent = dptr->parent;
      d [ 0 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 0;
      D.df = 0;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = dptr->xo; /* Position of Dialog */
      D.yo = dptr->yo;
      D.xl = 35; /* Length of Dialog */
      D.yl = 35; /* Width of Dialog */
      D.Initfun = kgBusyinit; /* init fuction for Dialog */
      D.kbattn = 0; /* 1 for drawing keyborad attention */
      D.butattn = 0; /* 1 for drawing button attention */
      D.fullscreen = 0; /* 1 for for fullscreen mode */
      D.Deco = 0; /* 1 for Window Decorration */
      D.transparency = 1.000000; /* float 1.0 for full transparency */
      D.Newwin = 0; /* 1 for new window not yet implemented */
      D.DrawBkgr = 0; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 0; /* 1 for stickyness */
#if 0 
      D.Callback = kgBusyCallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.Fixpos = 1; /* 1 for Fixing Position */
      D.NoTaskBar = 1; /* 1 for not showing in task bar*/
      D.StackPos = 1; /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/
      D.Shapexpm = NULL; /* PNG/jpeg file for window shape;
      Black color will not be drawn */
      D.parent = parent; /* 1 for not showing in task bar*/
      D.pt = NULL; /* any data to be passed by user*/
      strcpy ( D.name , "Kulina Designer ver 1.0" ) ;
           /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
      } /* end of fullscreen mode */
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifykgBusyGc(&(D.gc));    /*  set colors for gui*/
      D.pt = dptr->pipe;
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
      return NULL;
  }
  void * kgOpenBusy ( void *arg , int xo , int yo ) {
      struct _doubleptr{void *parent;int pipe [ 2 ] ;
      pthread_t Pth;int xo;int yo;} *dptr;
/*************************************************


*************************************************/
      int ch;
      dptr = ( struct _doubleptr * ) Malloc ( sizeof ( struct _doubleptr ) ) ;
      pipe ( dptr->pipe ) ;
      dptr->parent = arg;
      dptr->xo = xo;
      dptr->yo = yo;
      pthread_create ( & ( dptr->Pth ) , NULL , kgBusy , dptr ) ;
//   kgBusy(&dptr );
      return ( void * ) dptr;
  }
  void kgCloseBusy ( void * id ) {
      struct _doubleptr{void *parent;int pipe [ 2 ] ;
      pthread_t Pth;int xo;int yo;} *dptr;
      char bf = 0xff;
      dptr = ( struct _doubleptr * ) id;
      write ( dptr->pipe [ 1 ] , & bf , 1 ) ;
//   pthread_cancel(dptr->Pth);
      pthread_join ( dptr->Pth , NULL ) ;
      close ( dptr->pipe [ 0 ] ) ;
      close ( dptr->pipe [ 1 ] ) ;
      free ( dptr ) ;
  }
  ThumbNail **uiStringToThumbNails ( char **menu ) {
      ThumbNail **th = NULL;
      int i , n;
      if ( menu != NULL ) {
          i = 0;
          while ( menu [ i++ ] != NULL ) ;
          n = i;
          th = ( ThumbNail ** ) Malloc ( sizeof ( ThumbNail* ) *n ) ;
          th [ n-1 ] = NULL;
          for ( i = 0;i < ( n-1 ) ;i++ ) {
              th [ i ] = ( ThumbNail * ) Malloc ( sizeof ( ThumbNail ) ) ;
              if ( menu [ i ] != NULL ) {
                  th [ i ]->name = ( char * ) Malloc ( strlen ( menu [ i ] ) +1 ) ;
                  strcpy ( th [ i ]->name , menu [ i ] ) ;
              }
              else th [ i ]->name = NULL;
              th [ i ]->img = NULL;
              th [ i ]->sw = 0;
              th [ i ]->id = i;
              th [ i ]->state = 1;
          }
      }
      return th;
  }
  ThumbNail **kgStringToThumbNails ( char **menu ) {
      return uiStringToThumbNails ( menu ) ;
  }
  void _uiWriteStrings ( DIG *fid , float xo , float yo , float xl , \
  float yl , char *buf , float wd , float ht , int justf ) \
  {
      char BUF [ 1000 ] , *str [ 200 ] , *pt;
      int i , j , k , OK = 0;
      float length = 0.0 , dyl , yy , ln = 0.0 , fac , yoff , xoff;
      strncpy ( BUF , buf , 999 ) ;
      pt = BUF;
      j = 1;
      k = 0;
      kgTextSize ( fid , ht , wd , 0.0 ) ;
      while ( 1 ) {
          i = 0;
          if ( pt [ i ] == '\0' ) break;
          j++;
          OK = 1;
          while ( pt [ i ] != '\0' ) {
              OK = 0;
              if ( ( pt [ i ] == '\n' ) ) { pt [ i ] = '\0';break;}
              if ( ( ( pt [ i ] == '\\' ) && ( pt [ i+1 ] == 'n' ) ) ) \
              { pt [ i ] = '\0';i++;break;}
              i++;
              OK = 1;
          }
          str [ k++ ] = pt;
          ln = kgStringLength ( fid , pt ) ;
          if ( length < ln ) length = ln;
          if ( OK ) break;
          pt = pt+i+1;
      }
      if ( k > 0 ) {
          dyl = yl/k;
          fac = 0.85*xl/length;
          xoff = 0.07*xl;
          if ( fac > 1.0 ) fac = 1.0;
          if ( ht > 0.7*dyl ) ht = 0.7*dyl;
          yoff = ( yl - k*ht*1.4 ) *0.5;
          dyl = ht*1.4;
          kgTextSize ( fid , ht , wd*fac , 0.0 ) ;
          if ( xoff > wd ) xoff = wd;
          yy = yl - 0.8*dyl-yoff;
          for ( i = 0;i < k;i++ ) {
              length = kgStringLength ( fid , str [ i ] ) ;
              switch ( justf ) {
                  case 0:
                  kgMove2f ( fid , xo+ ( xl-length ) *0.5 , yo+yy ) ;
                  break;
                  case -1:
                  kgMove2f ( fid , xo+xoff , yo+yy ) ;
                  break;
                  case 1:
                  kgMove2f ( fid , xo+ ( xl-length-xoff ) , yo+yy ) ;
                  break;
                  default:
                  kgMove2f ( fid , xo+ ( xl-length ) *0.5 , yo+yy ) ;
                  break;
              }
              kgWriteText ( fid , str [ i ] ) ;
              yy -= dyl;
          }
      }
      return;
  }
  int kgMenu1browser1callback ( int item , int i , void *Tmp ) {
      int ( *Fun ) ( int , void * ) ;
      DIALOG *D;DIE *E;void *pt;
      int ret = 1;
      D = ( DIALOG * ) Tmp;
      pt = D->pt;
      E = ( DIE * ) uiGetWidget ( Tmp , i ) ;
      if ( D->pt != NULL ) {
          Fun = D->pt;
          ret = Fun ( item , E->arg ) ;
      }
      return ret;
  }
  void kgMenu1browser1init ( DIE *E , void *pt ) {
      char **menu = NULL;int *sw = NULL;void **xpm;
  }
  int kgMenu1init ( void *Tmp ) {
      int ret = 1;
      DIALOG *D;void *pt;
      D = ( DIALOG * ) Tmp;
      pt = D->pt;
      return ret;
  }
  int kgMenu1CallBack ( void *Tmp , void *tmp ) {
      int ret = 0;
      BRW_STR *br;
      DIE *E;
      DIALOG *D;
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
              E = ( DIE * ) uiGetWidget ( Tmp , 0 ) ;
              br = E->bwsr;
              * ( E->df ) = 0;
              br->df = 0;
              ret = 1;
          }
      }
      return ret;
  }
//int kgMenu1( void *parent,int xo,int yo,int df,char **menu,int size) {
  int kgMenu1 ( void *parent , int xo , int yo , \
  int df , char **menu , int size , int \
   ( *Fun ) ( int , void * ) , void *arg ) {
      int ret = 1 , n = 0 , xl = 0 , yl = 0 , lng = 0 , length = 0;
      DIALOG D;
      DIA d [ 2 ] ;
      int v1;
      DIE e0 = {
          'e' , 38 , 26 , 217 , 188 , 6 , ( int * ) & v1 , NULL , NULL , \
          NULL , kgMenu1browser1callback , /* *args , callback */
      20 , 6 , 22 , 1 , 1 , 1 , 0 };
      if ( menu == NULL ) return df;
      v1 = df;
      lng = 0;length = 0;
      n = 0; while ( menu [ n ] != NULL ) {
          length = strlen ( menu [ n ] ) ;
          if ( lng < length ) lng = length;
          n++;
      }
      e0.arg = arg;
      e0.offset = 4;
      e0.hide = 0;
      e0.width = 20;
      e0.menu = menu;
      e0.w = 20;
      e0.bordr = 0;
      e0.itemhi = 1;
      e0.bkgr = 0;
      if ( size > n ) size = n;
      e0.size = n;
      xl = lng*8+10;
      if ( xl < 50 ) xl = 50;
      yl = size*e0.width+2*e0.offset;
      e0.x1 = 0;
      e0.y1 = 0;
      e0.x2 = e0.x1+xl;
      e0.y2 = e0.y1+yl;
      v1 = df;
      d [ 0 ] .e = & e0;
      d [ 1 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 4;
      D.df = 0;
      D.tw = 0;
      D.bw = 0;
      D.lw = 0;
      D.rw = 0;
      D.xo = xo; /* Position of Dialog */
      D.yo = yo;
      D.xl = xl; /* Length of Dialog */
      D.yl = yl;
      D.Initfun = kgMenu1init; /* init fuction for Dialog */
      D.kbattn = 1; /* 1 for drawing keyborad attention */
      D.butattn = 1; /* 1 for drawing button attention */
      D.fullscreen = 0; /* 1 for for fullscreen mode */
      D.Deco = 1; /* 1 for Window Decorration */
      D.transparency = 0.000000; /* float 1.0 for full transparency */
      D.Newwin = 0; /* 1 for new window not yet implemented */
      D.DrawBkgr = 1; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 0; /* 1 for stickyness */
#if 1 
      D.Callback = kgMenu1CallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.Fixpos = 0; /* 1 for Fixing Position */
      D.NoTaskBar = 0; /* 1 for not showing in task bar*/
      D.StackPos = 1; /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/
      D.Shapexpm = NULL; /* PNG/jpeg file for window shape;
      Black color will not be drawn */
      D.parent = parent; /* 1 for not showing in task bar*/
//  D.pt = NULL;    /*  any data to be passed by user*/
      D.pt = ( void * ) Fun;
      strcpy ( D.name , "Kulina Designer ver 1.0" ) ;
           /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
      } /* end of fullscreen mode */
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
      D.SearchList = NULL;
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
//  printf("ret:%d %d\n",ret,v1);
      return v1;
  }
  static int UpdateBrowsers ( void *Tmp , int item ) ;
  static int PickImagebrowser1callback ( int item , int i , void *Tmp ) {
      DIA *D;DIE *B;
      char *folder , *dir;
      int ret = 1;
      void **pt;
      pt = ( void ** ) ( ( ( DIALOG * ) Tmp )->pt ) ;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .e;
      folder = Dgetmenustring ( Tmp , i ) ;
      dir = ( char * ) pt [ 0 ] ;
      if ( strcmp ( dir , "/" ) != 0 ) strcat ( dir , "/" ) ;
      strcat ( dir , folder ) ;
      UpdateBrowsers ( Tmp , 1 ) ;
      switch ( item ) {
          case 1:
          break;
      }
      return ret;
  }
  static int PickImagebrowser2callback ( int item , int i , void *Tmp ) {
      ThumbNail **th , *tpt;
      void *Img = NULL;
      KBEVENT kb;
      DIA *Dia;DIY *B;
      DIALOG *D;
      int ret = 1 , pos , k , rv;
      int x = -100 , y = -100 , ymin , ymax;
      int x1 , y1 , x2 , y2;
      D = ( ( DIALOG * ) Tmp ) ;
      Dia = ( ( DIALOG * ) Tmp )->d;
      B = Dia [ i ] .y;
      ymin = B->y1+D->yo+B->offset;
      ymax = B->y2+D->yo-B->offset;
      th = ( ThumbNail ** ) B->list;
      if ( kgDragThumbNail ( B , item-1 , & x , & y ) ) {
          pos = kgGetThumbNailItem ( B , x , y ) ;
          if ( ( pos >= 0 ) && ( pos != ( item-1 ) ) ) {
              tpt = th [ item-1 ] ;
              if ( pos < ( item-1 ) ) {
                  for ( k = ( item-1 ) ;k > pos;k-- ) { th [ k ] = th [ k-1 ] ; }
                  th [ pos ] = tpt;
              }
              else {
                  for ( k = item;k < pos;k++ ) {th [ k-1 ] = th [ k ] ;}
                  th [ pos-1 ] = tpt;
              }
              * ( B->df ) = pos+1;
              kgSetList ( B , ( void ** ) th ) ;
              kgUpdateWidget ( B ) ;
              kgFreeXImage ( Img ) ;
          }
      }
      return ret;
  }
  static int UpdateBrowsers ( void *Tmp , int item ) {
      DIA *D;DIY *B; DIE *E;
      int i , k , size;
      char buffer [ 400 ] ;
      char *CurDir;
      char **menu , **m;
      ThumbNail **th;
      int *sw;
      GMIMG *img;
      void **xpm;
      void **pt;
      void *id;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ 0 ] .e;
      B = D [ 1 ] .y;
      size = B->width;
      pt = ( void ** ) ( ( ( DIALOG * ) Tmp )->pt ) ;
      CurDir = ( char * ) pt [ 0 ] ;
      Dprintf ( Tmp , 4 , ( char * ) pt [ 0 ] ) ;
      th = ( ThumbNail ** ) ( pt [ 2 ] ) ;
      pt [ 3 ] = kgFreeDouble ( pt [ 3 ] ) ;
      m = kgFolderMenu ( CurDir ) ;
      pt [ 3 ] = m;
//  E->menu=m;
      kgSetList ( E , ( void ** ) m ) ;
      kgUpdateWidget ( E ) ;
      sprintf ( buffer , "%s : Making Thumbnails... Pl. Wait" , CurDir ) ;
      Dprintf ( Tmp , 4 , buffer ) ;
      kgUpdateOn ( Tmp ) ;
      uiFreeThumbNails ( th ) ;
      id = kgOpenBusy ( Tmp , 500 , 400 ) ;
      th = uiMakeThumbNails ( CurDir , size ) ;
      kgCloseBusy ( id ) ;
      kgSetList ( B , ( void ** ) th ) ;
      pt [ 2 ] = ( void * ) th;
      Dprintf ( Tmp , 4 , CurDir ) ;
      kgUpdateWidget ( B ) ;
      kgUpdateOn ( Tmp ) ;
  }
  static int PickImagebutnbox1callback ( int key , int i , void *Tmp ) {
      DIA *D;DIB *B;
      int n , ret = 0 , k;
      char *CurDir , **m , **menu0;
      void **pt;
      GMIMG **imgs = NULL;
      int *sw;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .b;
      n = B->nx*B->ny;
      pt = ( void ** ) ( ( ( DIALOG * ) Tmp )->pt ) ;
      CurDir = ( char * ) pt [ 0 ] ;
      switch ( key ) {
          case 1:
          strcpy ( ( char * ) pt [ 0 ] , ( char * ) pt [ 1 ] ) ;
          break;
          case 2:
          k = strlen ( CurDir ) ;
          while ( ( CurDir [ k ] != '/' ) && ( k > 1 ) ) {CurDir [ k ] = '\0';k--;}
          CurDir [ k ] = '\0';
          break;
      }
      UpdateBrowsers ( Tmp , 1 ) ;
      return ret;
  }
  static int PickImagehoribar1callback ( int key , int i , void *Tmp ) {
      char flname [ 200 ] , sflname [ 500 ] , buf [ 1000 ] ;
      char *CurDir , *HomeDir , *dest;
      void **pt;
      DIA *D;DILN *B;DIY *Y;
      int n , ret = 1 , k;
      int *sw;
      ThumbNail **th;
      D = ( ( DIALOG * ) Tmp )->d;
      B = D [ i ] .H;
      Y = ( DIY * ) kgGetWidget ( Tmp , 1 ) ;
      th = ( ThumbNail ** ) Y->list;
      pt = ( void ** ) ( ( ( DIALOG * ) Tmp )->pt ) ;
      HomeDir = ( char * ) pt [ 1 ] ;
      CurDir = ( char * ) pt [ 0 ] ;
      dest = ( char * ) pt [ 4 ] ;
      n = Y->nitems;
      switch ( key ) {
          case 1:
          ret = 0;
          if ( th == NULL ) ret = 0;
          else {
              for ( k = 0;k < n;k++ ) {
                  if ( th [ k ]->sw == 1 ) {
                      strcpy ( flname , th [ k ]->name ) ;
                      strcpy ( sflname , CurDir ) ;
                      if ( strcmp ( CurDir , "/" ) != 0 ) strcat ( sflname , "/" ) ;
                      strcat ( sflname , flname ) ;
                      if ( kgFolderBrowser ( NULL , 100 , 100 , flname , "*jpg *.png" ) == 1 ) {
                          sprintf ( buf , "cp \"%s\" \"%s\"" , sflname , flname ) ;
                          system ( buf ) ;
                      }
                      break;
                  }
              }
          }
          break;
          case 2:
          ret = 0;
          if ( th == NULL ) ret = 0;
          else {
              for ( k = 0;k < n;k++ ) {
                  if ( th [ k ]->sw == 1 ) {
                      strcpy ( dest , CurDir ) ;
                      if ( strcmp ( CurDir , "/" ) != 0 ) strcat ( dest , "/" ) ;
                      strcat ( dest , th [ k ]->name ) ;
                      ret = 1;
                      break;
                  }
              }
          }
          break;
          case 3:
          break;
      }
      return ret;
  }
  static int PickImageinit ( void *Tmp ) {
      int ret = 1;
      DIALOG *D;
      char **pt;
      D = ( DIALOG * ) Tmp;
      pt = ( char ** ) ( ( ( DIALOG * ) Tmp )->pt ) ;
      Dprintf ( Tmp , 4 , pt [ 0 ] ) ;
      return ret;
  }
  static int PickImageCallBack ( void *Tmp , void *tmp ) {
      int ret = 0;
      DIALOG *D;
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
          }
      }
      return ret;
  }
  static void PickImagebrowser2init ( DIY *Y , void *pt ) {
  }
  static void PickImagebutton1init ( DIB *B , void *pt ) {
  }
  static void PickImagehbutton1init ( DILN *H , void *pt ) {
  }
  int kgPickImage ( void *parent , int xo , int yo , void *pt ) {
      int v0 = 1;
      int v1 = 1;
      int v2 = 1;
      ThumbNail **th;
      int ret = 1;
      GMIMG *img;
      char CurDir [ 1000 ] ;
      char HomeDir [ 1000 ] ;
      void *ptrs [ 6 ] ;
      DIALOG D;
      DIA d [ 6 ] ;
      char **menu0;
      DIE e0 = {
          'e' , 8 , 65 , 316 , 457 , 1 , ( int * ) & v0 , NULL , menu0 , \
          NULL , PickImagebrowser1callback , /* *args , callback */
      20 , 8 , 22 , 1 , 1 , 1 , 0 };
      DIY y1 = {
          'y' , 326 , 65 , 843 , 457 , 16 , 16 , 72 , 72 , \
               1 , 32767 , 0 , 4 , ( int * ) & v1 , \
          NULL , NULL , NULL , PickImagebrowser2callback , /* *args , callback */
          12 , /* Border Offset */
          22 , /* Scroll width */
          1 , /* Type */
          1 , /* item highlight */
          1 , /* bordr */
          1 , /* bkgr */
          0 /*= 1 hide */
      };
      char *titles2 [ ] = {
       ( char * ) "" , ( char * ) "" , NULL };
      void *xpm2 [ ] = {
           ( char * ) "##" , // Pixmap info
           ( char * ) "##" , // Pixmap info
      NULL , NULL , NULL , NULL , NULL };
      int bkgr2 [ ] = {
      43 , 43 , -1 };
      char *butncode2 = NULL;
      int sw2 [ ] = {1 , 1 , 1};
      DIB n2 = {
          'b' , 10 , 15 , 100 , 61 , 2 , 0 , 36 , 36 , 2 , 1 , ( int * ) & v2 , sw2 , \
          titles2 , butncode2 , NULL , PickImagebutnbox1callback , /* args , Callbak */
          xpm2 , bkgr2 , /* pointers to xpms and colors */
          4 , 0.200000 , 0 , 0 /* button type and roundinfg factor \
           ( 0-0.5 ) , bordr , hide */
      };
      int butn1 = 1;
      int sw3 [ 3 ] = {
      1 , 1 , 1 };
      char *titles3 [ ] = {
       ( char * ) "Copy" , ( char * ) "Select" , ( char * ) "Cancel" , NULL };
      void *xpm3 [ ] = {
          NULL , /* pixmap info */
          NULL , /* pixmap info */
          NULL , /* pixmap info */
      NULL , NULL , NULL , NULL , NULL , NULL , NULL };
      int bkgr3 [ ] = {
      -1 , -1 , -1 , -1 };
      char *butncode3 = NULL;
      DILN H3 = {
          'H' , 574 , 25 , 828 , 56 , 4 , 0 , 80 , 25 , \
               3 , 1 , & butn1 , sw3 , titles3 , \
          butncode3 , NULL , PickImagehoribar1callback , /* arg , Callbak */
          xpm3 , bkgr3 , /* pointers to xpms and colors */
          3 , 0.500000 , /* button type and roundinfg factor ( 0-0.5 ) */
          0 , 0 /* button border , hide */
      };
      DII i4 = {
      'i' , 103 , 25 , 530 , 55 , 46 , 1 };
      xpm2 [ 0 ] = kgHomeImage ( 24 , 250 , 250 , 220 ) ;
      xpm2 [ 1 ] = kgUpdirImage ( 24 , 250 , 250 , 220 ) ;
      getcwd ( CurDir , 999 ) ;
      strcpy ( HomeDir , CurDir ) ;
      menu0 = kgFolderMenu ( CurDir ) ;
      e0.menu = menu0;
      th = ( ThumbNail ** ) uiMakeThumbNails ( CurDir , y1.width ) ;
      y1.list = ( void * ) th;
      ptrs [ 0 ] = CurDir;
      ptrs [ 1 ] = HomeDir;
      ptrs [ 2 ] = th;
      ptrs [ 3 ] = menu0;
      ptrs [ 4 ] = pt;
      d [ 0 ] .e = & e0;
      d [ 1 ] .y = & y1;
      PickImagebrowser2init ( & y1 , pt ) ;
      d [ 2 ] .b = & n2;
      PickImagebutton1init ( & n2 , pt ) ;
      d [ 3 ] .H = & H3;
      PickImagehbutton1init ( & H3 , pt ) ;
      d [ 4 ] .i = & i4;
      d [ 5 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 4;
      D.df = 3;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = xo; /* Position of Dialog */
      D.yo = yo;
      D.xl = 850; /* Length of Dialog */
      D.yl = 463; /* Width of Dialog */
      D.Initfun = PickImageinit; /* init fuction for Dialog */
      D.kbattn = 0; /* 1 for drawing keyborad attention */
      D.butattn = 0; /* 1 for drawing button attention */
      D.fullscreen = 0; /* 1 for for fullscreen mode */
      D.Deco = 1; /* 1 for Window Decorration */
      D.transparency = 0.000000; /* float 1.0 for full transparency */
      D.Newwin = 0; /* 1 for new window not yet implemented */
      D.DrawBkgr = 1; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 0; /* 1 for stickyness */
#if 0 
      D.Callback = PickImageCallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.Fixpos = 0; /* 1 for Fixing Position */
      D.NoTaskBar = 0; /* 1 for not showing in task bar*/
      D.StackPos = 1; /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/
      D.Shapexpm = NULL; /* PNG/jpeg file for window shape;
      Black color will not be drawn */
      D.parent = parent; /* 1 for not showing in task bar*/
      D.pt = ptrs; /* any data to be passed by user*/
      strcpy ( D.name , "PickImage" ) ; /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
      } /* end of fullscreen mode */
//  D.SearchList=(void *)Dopen();    /*  list of directories picture search */
      D.SearchList = NULL;
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyPickImageGc(&(D.gc));    /*  set colors for gui*/
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
//  Dempty((Dlink *)D.SearchList);
      D.SearchList = NULL;
//  insert any memory frees here to avoid leak  
      kgFreeDouble ( ( void ** ) ( e0.menu ) ) ;
      uiFreeThumbNails ( ( ThumbNail ** ) ( y1.list ) ) ;
      kgFreeImage ( xpm2 [ 0 ] ) ;
      kgFreeImage ( xpm2 [ 1 ] ) ;
      if ( ret == 3 ) ret = 0;
      else ret = 1;
      return ret;
  }
  int kgSelectImage ( void *parent , int xo , int yo , int ThSize , void *pt ) {
      int v0 = 1;
      int v1 = 1;
      int v2 = 1;
      ThumbNail **th;
      int ret = 1;
      GMIMG *img;
      char CurDir [ 1000 ] ;
      char HomeDir [ 1000 ] ;
      void *ptrs [ 6 ] ;
      DIALOG D;
      DIA d [ 6 ] ;
      char **menu0;
      DIE e0 = {
          'e' , 8 , 65 , 316 , 457 , 1 , ( int * ) & v0 , NULL , menu0 , \
          NULL , PickImagebrowser1callback , /* *args , callback */
      20 , 8 , 22 , 1 , 1 , 1 , 0 };
      DIY y1 = {
          'y' , 326 , 65 , 843 , 457 , 16 , 16 , ThSize , ThSize , 1 , 32767 , 0 , 4 , \
           ( int * ) & v1 , NULL , NULL , NULL , PickImagebrowser2callback , \
                /* *args , callback */
          12 , /* Border Offset */
          22 , /* Scroll width */
          1 , /* Type */
          1 , /* item highlight */
          1 , /* bordr */
          1 , /* bkgr */
          0 /*= 1 hide */
      };
      char *titles2 [ ] = {
       ( char * ) "" , ( char * ) "" , NULL };
      void *xpm2 [ ] = {
           ( char * ) "##" , // Pixmap info
           ( char * ) "##" , // Pixmap info
      NULL , NULL , NULL , NULL , NULL };
      int bkgr2 [ ] = {
      43 , 43 , -1 };
      char *butncode2 = NULL;
      int sw2 [ ] = {1 , 1 , 1};
      DIB n2 = {
          'b' , 10 , 15 , 100 , 61 , 2 , 0 , 36 , 36 , 2 , 1 , ( int * ) & v2 , sw2 , \
          titles2 , butncode2 , NULL , PickImagebutnbox1callback , /* args , Callbak */
          xpm2 , bkgr2 , /* pointers to xpms and colors */
          4 , 0.200000 , 0 , 0 /* button type and roundinfg factor \
           ( 0-0.5 ) , bordr , hide */
      };
      int butn1 = 1;
      int sw3 [ 3 ] = {
      1 , 1 , 1 };
      char *titles3 [ ] = {
       ( char * ) "Copy" , ( char * ) "Select" , ( char * ) "Cancel" , NULL };
      void *xpm3 [ ] = {
          NULL , /* pixmap info */
          NULL , /* pixmap info */
          NULL , /* pixmap info */
      NULL , NULL , NULL , NULL , NULL , NULL , NULL };
      int bkgr3 [ ] = {
      -1 , -1 , -1 , -1 };
      char *butncode3 = NULL;
      DILN H3 = {
          'H' , 574 , 25 , 828 , 56 , 4 , 0 , 80 , 25 , \
               3 , 1 , & butn1 , sw3 , titles3 , \
          butncode3 , NULL , PickImagehoribar1callback , /* arg , Callbak */
          xpm3 , bkgr3 , /* pointers to xpms and colors */
          3 , 0.500000 , /* button type and roundinfg factor ( 0-0.5 ) */
          0 , 0 /* button border , hide */
      };
      DII i4 = {
      'i' , 103 , 25 , 530 , 55 , 46 , 1 };
      xpm2 [ 0 ] = kgHomeImage ( 24 , 250 , 250 , 220 ) ;
      xpm2 [ 1 ] = kgUpdirImage ( 24 , 250 , 250 , 220 ) ;
      getcwd ( CurDir , 999 ) ;
      strcpy ( HomeDir , CurDir ) ;
      menu0 = kgFolderMenu ( CurDir ) ;
      e0.menu = menu0;
      th = ( ThumbNail ** ) uiMakeThumbNails ( CurDir , y1.width ) ;
      y1.list = ( void * ) th;
      ptrs [ 0 ] = CurDir;
      ptrs [ 1 ] = HomeDir;
      ptrs [ 2 ] = th;
      ptrs [ 3 ] = menu0;
      ptrs [ 4 ] = pt;
      d [ 0 ] .e = & e0;
      d [ 1 ] .y = & y1;
      PickImagebrowser2init ( & y1 , pt ) ;
      d [ 2 ] .b = & n2;
      PickImagebutton1init ( & n2 , pt ) ;
      d [ 3 ] .H = & H3;
      PickImagehbutton1init ( & H3 , pt ) ;
      d [ 4 ] .i = & i4;
      d [ 5 ] .t = NULL;
      D.VerId = 1401010200;
      kgInitUi ( & D ) ;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 4;
      D.df = 3;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = xo; /* Position of Dialog */
      D.yo = yo;
      D.xl = 850; /* Length of Dialog */
      D.yl = 463; /* Width of Dialog */
      D.Initfun = PickImageinit; /* init fuction for Dialog */
      D.kbattn = 0; /* 1 for drawing keyborad attention */
      D.butattn = 0; /* 1 for drawing button attention */
      D.fullscreen = 0; /* 1 for for fullscreen mode */
      D.Deco = 1; /* 1 for Window Decorration */
      D.transparency = 0.000000; /* float 1.0 for full transparency */
      D.Newwin = 0; /* 1 for new window not yet implemented */
      D.DrawBkgr = 1; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 0; /* 1 for stickyness */
#if 0 
      D.Callback = PickImageCallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.Fixpos = 0; /* 1 for Fixing Position */
      D.NoTaskBar = 0; /* 1 for not showing in task bar*/
      D.StackPos = 1; /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/
      D.Shapexpm = NULL; /* PNG/jpeg file for window shape;
      Black color will not be drawn */
      D.parent = parent; /* 1 for not showing in task bar*/
      D.pt = ptrs; /* any data to be passed by user*/
      strcpy ( D.name , "PickImage" ) ; /* Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
      }
      else { // for fullscreen
          int xres , yres;
          GetDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
      } /* end of fullscreen mode */
//  D.SearchList=(void *)Dopen();    /*  list of directories picture search */
      D.SearchList = NULL;
      kgDefaultGuiTheme ( & ( D.gc ) ) ; /* set colors for gui*/
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifyPickImageGc(&(D.gc));    /*  set colors for gui*/
      D.Resize = 0;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.MinWidth = D.MinHeight = 200;
      ret = kgUi ( & D ) ;
//  Dempty((Dlink *)D.SearchList);
      D.SearchList = NULL;
//  insert any memory frees here to avoid leak  
      kgFreeDouble ( ( void ** ) ( e0.menu ) ) ;
      uiFreeThumbNails ( ( ThumbNail ** ) ( y1.list ) ) ;
      kgFreeImage ( xpm2 [ 0 ] ) ;
      kgFreeImage ( xpm2 [ 1 ] ) ;
      if ( ret == 3 ) ret = 0;
      else ret = 1;
      return ret;
  }
  static int Size ( char *t ) {
      int j = 0 , lng = 0;
      lng = strlen ( t+j ) *9;
      if ( lng != 0 ) lng += ( 8 ) ;
      return ( lng ) ;
  }
  static int gcanftextbox1callback ( int cellno , int i , void *Tmp ) {
  /*************************************************
   cellno: current cell counted along column strting with 0
           ie 0 to (nx*ny-1)
   i     : widget id starting from 0
   Tmp   : Pointer to DIALOG
   *************************************************/
      DIALOG *D;DIT *T;T_ELMT *e;
      int ret = 1;
      D = ( DIALOG * ) Tmp;
      T = ( DIT * ) kgGetWidget ( Tmp , i ) ;
      e = T->elmt;
      if ( T->ny == 1 ) kgSetExit ( Tmp ) ;
      return ret;
  }
  static int gscanfCallBack ( void *Tmp , void *tmp ) {
  /***********************************
    Tmp :  Pointer to DIALOG
    tmp :  Pointer to KBEVENT
   ***********************************/
      int ret = 0;
      DIALOG *D;
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
              ret = 1;
          }
      }
      return ret;
  }
  static int gscanfinit ( void *Tmp ) {
  /***********************************
    Tmp :  Pointer to DIALOG
   ***********************************/
  /* you add any initialisation here */
      int ret = 1;
      DIALOG *D;void *pt;
      D = ( DIALOG * ) Tmp;
      pt = D->pt;
      kgSetDefaultWidget ( Tmp , 0 ) ;
      return ret;
  }
  int gscanf ( void *parent , void *unknown , ... ) {
//  DILN *H;
      DIL *H;
      DIALOG D;
      DIA *d;
      DIT *T;
      DIT Tmp = {'t' , 10L , 10L , 0 , 0 , 20L , \
      1L , 1L , NULL , 0 , 0 , NULL , NULL};
      T_ELMT *e;
      int ch;
      char *titles [ ] = {"Okay" , NULL};
      char *cpt , *pt , wrk [ 5000 ] ;
      int i , j , k , item = 0 , it = 0 , size = 6 , ln , width , \
      lngth , nx , ny , ret = 1 , prsize = 0 , fldsize = 0;
      void **v = NULL;
      va_list ad;
      DIALOG *Par = NULL;
      Par = ( DIALOG * ) parent;
      va_start ( ad , unknown ) ;
      if ( unknown == NULL ) return 0;
      strcpy ( wrk , ( char * ) unknown ) ;
      cpt = wrk;
      while ( *cpt != '\0' ) { if ( *cpt == '%' ) item++;cpt++;}
      if ( item == 0 ) return 0;
      T = ( DIT * ) Malloc ( sizeof ( DIT ) ) ;
      T->item = -1;
      Tmp.Wid [ 0 ] = '\0';
      *T = Tmp;
      T->x1 = 5;
      T->y1 = 5;
      e = ( T_ELMT * ) Malloc ( sizeof ( T_ELMT ) *item ) ;
      cpt = wrk;
      pt = cpt;
      it = 0;
      while ( *cpt != '\0' ) {
          if ( *cpt == '%' ) {
              *cpt = '\0';
              ln = Size ( pt ) ;
              if ( ln > prsize ) prsize = ln;
              *cpt = '%';
              cpt++;
              size = 0;
              while ( isdigit ( *cpt ) ) {
                  size = size*10+ ( *cpt -'0' ) ;
                  cpt++;
              }
              if ( size > fldsize ) fldsize = size;
              ch = *cpt;
              if ( ( ch != 'd' ) && ( ch != 'f' ) && ( ch != 's' ) && ( ch != 'F' ) ) {
                  printf ( "Error in kgCreateTextbox: /%s/\n" , ( char* ) pt ) ;
                  return 0;
              }
              *cpt = '\0';
              ln = strlen ( pt ) +2;
              e [ it ] .fmt = ( char * ) Malloc ( ln ) ;
              strcpy ( e [ it ] .fmt , pt ) ;
              e [ it ] .sw = 1;
              cpt++;
              pt = cpt;
              switch ( ch ) {
                  case 's':
                  strcat ( e [ it ] .fmt , "s" ) ;
                  e [ it ] .sw = 1;
                  e [ it ] .v = ( void * ) va_arg ( ad , char * ) ;
                  break;
                  case 'f':
                  case 'g':
                  strcat ( e [ it ] .fmt , "f" ) ;
                  e [ it ] .sw = 1;
                  e [ it ] .v = ( void * ) va_arg ( ad , float * ) ;
                  break;
                  case 'F':
                  strcat ( e [ it ] .fmt , "F" ) ;
                  e [ it ] .sw = 1;
                  e [ it ] .v = ( void * ) va_arg ( ad , double * ) ;
                  break;
                  case 'd':
                  strcat ( e [ it ] .fmt , "d" ) ;
                  e [ it ] .sw = 1;
                  e [ it ] .v = ( void * ) va_arg ( ad , int * ) ;
                  break;
              }
              it++;
          }
          else cpt++;
      }
      nx = 1;
      ny = it;
      T->elmt = e;
      T->pt = NULL;
      T->nx = 1;
      T->ny = it;
      T->hide = 0;
      T->col = 1;
      T->row = 1;
      lngth = 0;
      width = ( ny ) *T->width+ ( ny-1 ) *10;
      for ( j = 0;j < it;j++ ) {
          ln = get_t_length ( e [ j ] .fmt ) ;
          if ( ln > lngth ) lngth = ln;
      }
      lngth = ( nx-1 ) *10+lngth;
      lngth += ( 10+4 ) ;
      width += ( 10+4 ) ;
      lngth += prsize;
      if ( lngth < 100 ) lngth = 100;
      T->x2 = T->x1+lngth;
      T->y2 = T->y1+width;
      T->Update = gcanftextbox1callback;
      D.VerId = 2107030000;
      kgInitUi ( & D ) ;
      D.d = ( DIA * ) Malloc ( sizeof ( DIA ) *3 ) ;
      d = D.d;
//  H = kgCreateHButtons(T->x1+lngth/2-36,T->y2+4,1,72,25,titles,NULL);
      H = kgCreateSplButtons ( T->x1+lngth/2-36 , T->y2+4 , \
      1 , 1 , 72 , 25 , titles , NULL ) ;
      H->fac = 0.5;
      H->bordr = 0;
      H->type = 4;
      d [ 0 ] .t = T;
      d [ 1 ] .t = ( DIT * ) H;
      d [ 2 ] .t = NULL;
      D.VerId = 2107030000;
      kgInitUi ( & D ) ;
      D.xo = 100;
      D.yo = 100;
      D.xl = lngth+10;
      D.yl = width+45;
      if ( Par != NULL ) {
          D.xo = ( Par->xl -D.xl ) /2;
          D.yo = ( Par->yl -D.yl ) /2;
      }
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 4;
      D.df = 0;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.Initfun = gscanfinit;
      D.kbattn = 0;
      D.butattn = 0;
      D.fullscreen = 0;
      D.Deco = 1;
      D.transparency = 0.0;
      D.Newwin = 0;
      D.DrawBkgr = 1;
      D.Bkpixmap = NULL;
      D.Sticky = 0;
      D.Resize = 1;
      D.MinWidth = 100;
      D.MinHeight = 100;
      D.Callback = gscanfCallBack;
      D.ResizeCallback = NULL;
      D.WaitCallback = NULL;
      D.Fixpos = 0;
      D.NoTaskBar = 0;
      D.StackPos = 1;
      D.Shapexpm = NULL;
      D.parent = parent;
      D.pt = NULL;
      D.Cleanupfun = NULL;
      d [ 0 ] .t = T;
      ret = kgUi ( & D ) ;
      k = 0;
      while ( d [ k ] .t != NULL ) {
          switch ( d [ k ] .t->code ) {
              case 'r':
              kgFreeDouble ( ( void ** ) ( d [ k ] .r->list ) ) ;
              d [ k ] .r->list = NULL;
              break;
              case 'c':
              kgFreeDouble ( ( void ** ) ( d [ k ] .c->list ) ) ;
              d [ k ] .c->list = NULL;
              break;
              deafult:
              break;
          }
          k++;
      }
      kgCleanUi ( & D ) ;
//  printf("gscanf over\n");
 // fflush(stdout);
      return ret;
  }
  DIT * kgCreateTable ( int xo , int yo , int nrows , \
  char **colformats , char *name ) \
  {
      void **pt;
      DIT *T = NULL;
      T_ELMT *E = NULL;
      int i , n , k , j , code = 0 , size = 0 , l;
      char buf [ 60 ] , buf1 [ 30 ] = " " , buf2 [ 6 ] = " " , ch;
      int nx = 1 , ny = 1 , lngth = 0 , ln , width , lnx [ 1000 ] , it;
      float x1 = 10 , y1 = 10 , x2 , y2;
      float *ftmp;
      double *dtmp;
      int *itmp;
      char *ctmp;
      DIT Tmp = {'T' , 10L , 10L , 0 , 0 , 20L , \
      1L , 1L , NULL , 0 , 0 , NULL , NULL};
      if ( colformats == NULL ) return NULL;
      nx = 0; while ( colformats [ nx ] != NULL ) nx++;
      if ( nx == 0 ) return NULL;
      ny = 1;
      if ( nrows > ny ) ny = nrows;
      Tmp.nx = nx;
      Tmp.ny = ny;
      T = ( DIT * ) Malloc ( sizeof ( DIT ) ) ;
      Tmp.Wid [ 0 ] = '\0';
      Tmp.pt = NULL;
      *T = Tmp;
      if ( name != NULL ) strcpy ( T->Wid , name ) ;
      it = nx*ny;
      E = ( T_ELMT * ) Malloc ( sizeof ( T_ELMT ) *nx*ny ) ;
      for ( i = 0;i < ( nx*ny ) ;i++ ) {
          E [ i ] .fmt = ( char * ) Malloc ( 150 ) ;
      }
      T->elmt = E;
      width = ( ny ) *T->width;
      for ( j = 0;j < nx;j++ ) {
          lnx [ j ] = 0;
          ln = get_t_length ( colformats [ j ] ) ;
          if ( ln > lnx [ j ] ) lnx [ j ] = ln;
      }
      i = 0;
      for ( k = 0;k < ny;k++ ) {
          ln = 0;
          for ( j = 0;j < nx;j++ ) {
              strcpy ( E [ i ] .fmt , colformats [ j ] ) ;
              l = strlen ( E [ i ] .fmt ) - 1;
              while ( E [ i ] .fmt [ l ] <= ' ' ) l--;
              ch = E [ i ] .fmt [ l ] ;
              switch ( ch ) {
                  case 'f':
                  case 'g':
                  ftmp = ( float * ) Malloc ( sizeof ( float ) ) ;
                  *ftmp = 0.;
                  E [ i ] .v = ( void * ) ftmp;
                  break;
                  case 'F':
                  dtmp = ( double * ) Malloc ( sizeof ( double ) ) ;
                  *dtmp = 0.;
                  E [ i ] .v = ( void * ) dtmp;
                  break;
                  case 'd':
                  itmp = ( int * ) Malloc ( sizeof ( int ) ) ;
                  *itmp = 0;
                  E [ i ] .v = ( void * ) itmp;
                  break;
                  case 's':
                  ctmp = ( char * ) Malloc ( 500 ) ;
                  ctmp [ 0 ] = '\0';
                  E [ i ] .v = ( void * ) ctmp;
                  break;
              }
              E [ i ] .sw = 1;
              i++;
          }
      }
      lngth = 0;
      for ( j = 0;j < nx;j++ ) lngth+= lnx [ j ] ;
//   lngth = (nx-1)*10+lngth;
      lngth += ( 10 ) ;
      width += ( 10 ) ;
      x1 = xo;
      y1 = yo;
      x2 = x1+lngth;
      y2 = y1+width;
      T->x1 = xo;
      T->y1 = yo;
      T->x2 = x2;
      T->y2 = y2;
      T->col = 1;
      T->row = 1;
      pt = ( void ** ) Malloc ( sizeof ( void * ) * ( it+1 ) ) ;
      for ( i = 0;i < it;i++ ) {
          pt [ i ] = ( void * ) E [ i ] .v;
      }
      pt [ it ] = NULL;
      T->pt = pt;
      T->Update = NULL;
      T->hide = 0;
      T->item = -1;
      return T;
  }
  void *kgCreateUi ( int length , int width , \
  int ( *Initfun ) ( void * ) , int \
   ( *CallBack ) ( void *, void * ) , int ( *Cleanupfun ) \
   ( void * ) , char *name ) {
      DIALOG *D;
      D = ( DIALOG * ) Malloc ( sizeof ( DIALOG ) ) ;
      D->VerId = 1401010100;
      kgInitUi ( D ) ;
      D->d = NULL;
      D->xo = 100;
      D->yo = 100;
      D->xl = length;
      D->yl = width;
      D->bkup = 1; /* set to 1 for backup */
      D->bor_type = 4;
      D->df = 1;
      D->tw = 4;
      D->bw = 4;
      D->lw = 4;
      D->rw = 4;
      D->Initfun = Initfun;
      D->Cleanupfun = Cleanupfun;
      D->kbattn = 1;
      D->butattn = 0;
      D->fullscreen = 0;
      D->Deco = 1;
      D->transparency = 0.0;
      D->Newwin = 1;
      D->DrawBkgr = 1;
      D->Bkpixmap = NULL;
      D->Sticky = 0;
      D->Resize = 1;
      D->MinWidth = 100;
      D->MinHeight = 100;
      D->Callback = CallBack;
      D->ResizeCallback = NULL;
      D->WaitCallback = NULL;
      D->Fixpos = 0;
      D->NoTaskBar = 0;
      D->StackPos = 0;
      D->Shapexpm = NULL;
      D->parent = NULL;
      D->pt = NULL;
      return D;
  }
  int kgEventLoopUi ( void *Dtmp ) {
      int ret = 0 , k;
      DIALOG *D;
      DIA *d;
      D = ( DIALOG * ) Dtmp;
      if ( D == NULL ) return 0;
      d = D->d;
      ret = kgUi ( D ) ;
      k = 0;
      while ( d [ k ] .t != NULL ) {
          switch ( d [ k ] .t->code ) {
              case 'r':
              kgFreeDouble ( ( void ** ) ( d [ k ] .r->list ) ) ;
              d [ k ] .r->list = NULL;
              break;
              case 'c':
              kgFreeDouble ( ( void ** ) ( d [ k ] .c->list ) ) ;
              d [ k ] .c->list = NULL;
              break;
              deafult:
              break;
          }
          k++;
      }
      kgCleanUi ( D ) ;
      free ( Dtmp ) ;
      Dtmp = NULL;
      return ret;
  }
  DIW * kgCreatePulldownBrowser ( int xo , int yo , \
  char *prompt , char **menu , char *name ) \
  {
      DIW *W;
      int i , n;
      char buf [ 60 ] , buf1 [ 30 ] = " ";
      int lng = 100;
      int min = 4 , max = 6 , df = 0 , ln = 0;
      int msize = 0;
      float x1 , y1 , x2 , y2;
      DIW Wtmp = {'w' , 10L , 10L , 0 , 0 , 5 , \
      NULL , NULL , NULL , NULL , NULL , 0};
      W = ( DIW * ) Malloc ( sizeof ( DIW ) ) ;
      Wtmp.Wid [ 0 ] = '\0';
      *W = Wtmp;
      if ( name != NULL ) strcpy ( W->Wid , name ) ;
      W->prompt = NULL;
      if ( prompt != NULL ) {
          n = strlen ( prompt ) ;
          W->prompt = ( char * ) Malloc ( n+1 ) ;
          strcpy ( W->prompt , prompt ) ;
      }
      max = 0;
      if ( menu != NULL ) {
          while ( menu [ max ] != NULL ) max++;
      }
      W->size = 5;
      if ( W->size > max ) W->size = max;
      W->df = ( int * ) ( & ( W->val ) ) ;
      * ( ( int * ) ( W->df ) ) = 1;
      W->menu = ( char ** ) Malloc ( sizeof ( char * ) * ( max+1 ) ) ;
      W->menu [ max ] = NULL;
      msize = 0;
      for ( i = 0;i < max;i++ ) {
          ln = strlen ( menu [ i ] ) ;
          W->menu [ i ] = ( char * ) Malloc ( ln+1 ) ;
          strcpy ( W->menu [ i ] , menu [ i ] ) ;
          if ( msize < ln ) msize = ln;
      }
      ln = 0;
      if ( n != 0 ) {n++;ln = 8;}
      x1 = xo;y1 = yo;x2 = x1+ ( n*9+ln ) +msize*9+32;y2 = y1+29;
      W->x1 = x1;
      W->y1 = y1;
      W->x2 = x2;
      W->y2 = y2;
      W->item = -1;
      W->Update = NULL;
      return W;
  }
  DIE * kgCreateBrowser ( int xo , int yo , char **menu , char *name ) {
      DIE *W;
      int i , n;
      int lng = 100;
      int min = 4 , max = 6 , df = 0 , ln = 0 , w = 200 , h = 200 , maxchar = 15;
      int msize = 0;
      float x1 , y1 , x2 , y2;
      DIE Wtmp = {'e' , 10L , 10L , 0 , 0 , 5 , NULL , NULL , \
      NULL , NULL , NULL , 20 , 6 , 22 , 1 , 1 , 1 , 0};
      W = ( DIE * ) Malloc ( sizeof ( DIE ) ) ;
      Wtmp.Wid [ 0 ] = '\0';
      *W = Wtmp;
      if ( name != NULL ) strcpy ( W->Wid , name ) ;
      max = 0;
      if ( menu != NULL ) {
          while ( menu [ max ] != NULL ) max++;
      }
      min = 5;
      h = ( min ) *20+16;
      W->prompt = ( char * ) " ";
      W->size = min;
      W->df = ( int * ) ( & ( W->val ) ) ;
      * ( ( int * ) ( W->df ) ) = 1;
      W->menu = ( char ** ) Malloc ( sizeof ( char * ) * ( max+1 ) ) ;
      W->menu [ max ] = NULL;
      msize = 5;
      for ( i = 0;i < max;i++ ) {
          min = strlen ( menu [ i ] ) ;
          if ( msize < min ) msize = min;
          W->menu [ i ] = ( char * ) Malloc ( min+1 ) ;
          strcpy ( W->menu [ i ] , menu [ i ] ) ;
      }
      w = ( msize*9+30 ) ;
      x1 = xo;y1 = yo;x2 = x1+w;y2 = y1+h;
      W->x1 = x1;
      W->y1 = y1;
      W->x2 = x1+w;
      W->y2 = y1+h;
      W->Update = NULL;
      W->item = -1;
      return W;
  }
  DIS * kgCreateMessageScroll ( int xo , int yo , \
  int nlines , int maxchar , char *name ) \
  {
      DIS *W;
      int i , n;
      char buf [ 60 ] , buf1 [ 30 ] = " ";
      int lng = 100;
      int min = 4 , max = 6 , df = 0 , ln = 0 , w = 200 , h = 200;
      int msize = 0;
      float x1 , y1 , x2 , y2;
      DIS Wtmp = {'s' , 10L , 10L , 0 , 0 , 5 , NULL , NULL , \
      NULL , NULL , NULL , 20 , 6 , 22 , 0 , 1 , 1 , 0};
      W = ( DIS * ) Malloc ( sizeof ( DIS ) ) ;
      Wtmp.Wid [ 0 ] = '\0';
      *W = Wtmp;
      if ( name != NULL ) strcpy ( W->Wid , name ) ;
      min = nlines;
      max = nlines;
      h = ( min+1 ) *20+20;
      w = ( maxchar*9+44 ) ;
      W->prompt = ( char * ) " ";
      W->size = min;
      W->menu = NULL;
      x1 = xo;y1 = yo;x2 = x1+w;y2 = y1+h;
      W->x1 = x1;
      W->y1 = y1;
      W->x2 = x1+w;
      W->y2 = y1+h;
      W->item = -1;
      W->Update = NULL;
      return W;
  }
  DIHB * kgCreateHorizSlide ( int xo , int yo , int length , \
  int min , int max , int val , char *name ) \
  {
      DIHB *W;
      int i , n;
      int lng = 100;
      int ln = 0 , wd = 25;
      int msize = 0;
      float x1 , y1 , x2 , y2;
      DIHB Wtmp = {'P' , 0 , 100 , 0 , 0 , 0 , 100 , \
      101 , NULL , NULL , NULL , NULL};
      W = ( DIHB * ) Malloc ( sizeof ( DIHB ) ) ;
      Wtmp.Wid [ 0 ] = '\0';
      *W = Wtmp;
      if ( name != NULL ) strcpy ( W->Wid , name ) ;
      W->lngth = length;
      W->min = min;
      W->max = max;
      W->prompt = NULL;
      W->df = ( int * ) & ( W->val ) ;
      * ( ( int * ) ( W->df ) ) = val;
      x1 = xo;y1 = yo;
      ln = W->lngth+10;
      x2 = x1+ln;y2 = y1+wd;
      W->x1 = x1+0.5;
      W->y1 = y1+0.5;
      W->x2 = x2+0.5;
      W->y2 = y2+0.5;
      W->Update = NULL;
      W->item = -1;
      return W;
  }
  DID * kgCreateIntSlide ( int xo , int yo , int length , \
  int min , int max , int val , char *name ) \
  {
      DID *W;
      int i , n;
      int lng = 100;
      int ln = 0 , wd = 46;
      int msize = 0;
      float x1 , y1 , x2 , y2;
      DID Wtmp = {'d' , 0 , 100 , 0 , 0 , 0 , 100 , \
      101 , NULL , NULL , NULL , NULL};
      W = ( DID * ) Malloc ( sizeof ( DID ) ) ;
      Wtmp.Wid [ 0 ] = '\0';
      *W = Wtmp;
      if ( name != NULL ) strcpy ( W->Wid , name ) ;
      W->lngth = length;
      W->min = min;
      W->max = max;
      W->prompt = NULL;
      W->df = ( int * ) & ( W->val ) ;
      * ( ( int * ) ( W->df ) ) = val;
      x1 = xo;y1 = yo;
      ln = W->lngth+50;
      x2 = x1+ln;y2 = y1+wd;
      W->x1 = x1+0.5;
      W->y1 = y1+0.5;
      W->x2 = x2+0.5;
      W->y2 = y2+0.5;
      W->Update = NULL;
      W->item = -1;
      return W;
  }
  DIF * kgCreateDoubleSlide ( int xo , int yo , int length , \
  double min , double max , double val , char *name ) \
  {
      DIF *W;
      int i , n;
      char buf [ 60 ] , buf1 [ 30 ] = " ";
      int lng = 100;
      int ln = 0 , wd = 46;
      int msize = 0;
      float x1 , y1 , x2 , y2;
      DIF Wtmp = {'f' , 0 , 100 , 0 , 0 , 1 , 100 , \
      100 , NULL , NULL , NULL , NULL};
      W = ( DIF * ) Malloc ( sizeof ( DIF ) ) ;
      Wtmp.Wid [ 0 ] = '\0';
      *W = Wtmp;
      if ( name != NULL ) strcpy ( W->Wid , name ) ;
      W->lngth = length;
      W->min = min;
      W->max = max;
      W->prompt = NULL;
      W->df = ( double * ) & ( W->val ) ;
      * ( ( double * ) ( W->df ) ) = val;
      x1 = xo;y1 = yo;
      ln = W->lngth+50;
      x2 = x1+ln;y2 = y1+wd;
      W->x1 = x1+0.5;
      W->y1 = y1+0.5;
      W->x2 = x2+0.5;
      W->y2 = y2+0.5;
      W->Update = NULL;
      W->item = -1;
      return W;
  }
  DIX * kgCreateSelectMenu ( int xo , int yo , int length , \
  int width , int itemlength , int type , char *name ) \
  {
      DIX *W;
      int i , n;
      float x1 , y1 , x2 , y2;
      DIX Wtmp = {'x' , 0L , 0L , 150 , 120 , 8 , 2 , 120 , 25 , 1 , 4 , 1 , \
      1 , NULL , NULL , NULL , NULL , NULL , 6 , 22 , 0 , 1 , 1 , 1 , 0};
      W = ( DIX * ) Malloc ( sizeof ( DIX ) ) ;
      Wtmp.Wid [ 0 ] = '\0';
      *W = Wtmp;
      if ( name != NULL ) strcpy ( W->Wid , name ) ;
      W->df = ( int * ) & ( W->val ) ;
      * ( ( int * ) ( W->df ) ) = 1;
      W->list = NULL;
      W->type = type;
      W->lngth = itemlength;
      W->x1 = xo;
      W->y1 = yo;
      W->x2 = W->x1+length;
      W->y2 = W->y1+width;
      W->item = -1;
      W->Update = NULL;
      W->pt = NULL;
      return W;
  }
  DIRA * kgCreateRadioButtons ( int xo , int yo , int length , \
  int width , int itemlength , char **list , char *name ) \
  {
      DIRA *W;
      int i , n;
      ThumbNail **th;
      int lng = 100;
      int offset = 10 , w = 22;
      int min = 4 , max = 6 , df = 0 , ln = 0 , itemw = 25 , h = 200 , maxchar = 15;
      int msize = 0;
      float x1 , y1 , x2 , y2;
      DIRA Wtmp = {'r' , 0L , 0L , 150 , 120 , 0 , 0 , 90 , 25 , 1 , 4 , 1 , \
      1 , NULL , NULL , NULL , NULL , NULL , 6 , 22 , 0 , 0 , 1 , 0 , 0};
      int type = 2;
      W = ( DIRA * ) Malloc ( sizeof ( DIRA ) ) ;
      Wtmp.Wid [ 0 ] = '\0';
      *W = Wtmp;
      if ( name != NULL ) strcpy ( W->Wid , name ) ;
      W->prompt = NULL;
      W->df = ( int * ) & ( W->val ) ;
      * ( ( int * ) ( W->df ) ) = 1;
      th = kgStringToThumbNails ( list ) ;
      W->list = ( void ** ) th;
      W->nx = 1;
      W->ny = 1;
      W->lngth = itemlength;
      x1 = xo;y1 = yo;x2 = x1+length;y2 = y1+width;
      W->x1 = x1+0.5;
      W->y1 = y1+0.5;
      W->x2 = x2+0.5;
      W->y2 = y2+0.5;
      W->item = -1;
      W->Update = NULL;
      return W;
  }
  DICH * kgCreateCheckBox ( int xo , int yo , int length , int width , \
  int itemlength , char **list , char *name ) \
  {
      DICH *W;
      int i , n;
      ThumbNail **th;
      int lng = 100;
      int offset = 10 , w = 22;
      int min = 4 , max = 6 , df = 0 , ln = 0 , itemw = 25 , h = 200 , maxchar = 15;
      int msize = 0;
      float x1 , y1 , x2 , y2;
      DICH Wtmp = {'c' , 0L , 0L , 150 , 120 , 0 , 0 , 90 , 25 , 1 , 4 , 1 , \
      1 , NULL , NULL , NULL , NULL , NULL , 6 , 22 , 0 , 0 , 1 , 0 , 0};
      int type = 2;
      W = ( DICH * ) Malloc ( sizeof ( DICH ) ) ;
      Wtmp.Wid [ 0 ] = '\0';
      *W = Wtmp;
      if ( name != NULL ) strcpy ( W->Wid , name ) ;
      W->prompt = NULL;
      W->df = ( int * ) & ( W->val ) ;
      * ( ( int * ) ( W->df ) ) = 1;
      th = kgStringToThumbNails ( list ) ;
      W->list = ( void ** ) th;
      W->nx = 1;
      W->ny = 1;
      W->lngth = itemlength;
      x1 = xo;y1 = yo;x2 = x1+length;y2 = y1+width;
      W->x1 = x1+0.5;
      W->y1 = y1+0.5;
      W->x2 = x2+0.5;
      W->y2 = y2+0.5;
      W->item = -1;
      W->Update = NULL;
      return W;
  }
  DIY * kgCreateThumbnailBrowser ( int xo , int yo , int length , \
  int width , int nailsize , int type , char *name ) \
  {
      DIY *W;
      int i , n;
      float x1 , y1 , x2 , y2;
      DIY Wtmp = {'y' , 0L , 0L , 150 , 120 , 8 , 20 , 120 , 25 , 1 , 4 , 1 , \
      1 , NULL , NULL , NULL , NULL , NULL , 6 , 22 , 0 , 1 , 1 , 1 , 0};
      W = ( DIY * ) Malloc ( sizeof ( DIY ) ) ;
      Wtmp.Wid [ 0 ] = '\0';
      *W = Wtmp;
      if ( name != NULL ) strcpy ( W->Wid , name ) ;
      W->df = ( int * ) & ( W->val ) ;
      * ( ( int * ) ( W->df ) ) = 1;
      W->list = NULL;
      W->type = type;
      W->lngth = nailsize;
      W->width = nailsize;
      W->x1 = xo;
      W->y1 = yo;
      W->x2 = W->x1+length;
      W->y2 = W->y1+width;
      W->item = -1;
      W->Update = NULL;
      W->pt = NULL;
      return W;
  }
/*kgGetFreeTypeFont code */
  static int CheckFontFile ( char *font ) {
      FILE *fp;
      int ret = 0;
      char Buff [ 300 ] ;
      sprintf ( Buff , "file %-s" , font ) ;
      fp = popen ( Buff , "r" ) ;
      if ( fp == NULL ) return 0;
      fgets ( Buff , 300 , fp ) ;
      pclose ( fp ) ;
      if ( strstr ( Buff , "Type 1 font" ) != NULL ) return 1;
      if ( strstr ( Buff , "Type font" ) != NULL ) return 1;
      if ( strstr ( Buff , "Type Font" ) != NULL ) return 1;
      return 0;
  }
  static int WriteMsg ( DIALOG *D ) {
      DIS *I;
      DIW *W;
      I = ( DIS * ) kgGetNamedWidget ( D , ( char * ) "Info" ) ;
      W = ( DIW * ) kgGetNamedWidget ( D , ( char * ) "Select" ) ;
      int Font;
      char **List = ( char ** ) kgGetList ( W ) ;
      char Buff [ 100 ] ;
      char *Msg [ ] = {"\"Our lives begin to end " , \
           " the day we become silent about" , \
      " things that matter\"" , NULL };
      Font = * ( W->df ) -1;
      kgWrite ( I , List [ Font ] ) ;
      kgWrite ( I , ( char * ) " " ) ;
      sprintf ( Buff , "!f%2.2d  %s" , Font , Msg [ 0 ] ) ;
      kgWrite ( I , Buff ) ;
      sprintf ( Buff , "!f%2.2d  %s" , Font , Msg [ 1 ] ) ;
      kgWrite ( I , Buff ) ;
      sprintf ( Buff , "!f%2.2d  %s" , Font , Msg [ 2 ] ) ;
      kgWrite ( I , Buff ) ;
      kgUpdateWidget ( I ) ;
  }
  static int GetFontbrowser1callback ( int item , int i , void *Tmp ) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIW *W;
      DIM *M;
      int ret = 1;
      char Buff [ 500 ] ;
      char *cpt;
      int Font;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
      D = ( DIALOG * ) Tmp;
      W = ( DIW * ) kgGetWidget ( Tmp , i ) ;
      M = ( DIM * ) kgGetNamedWidget ( D , ( char * ) "Msg" ) ;
      Font = item-1;
      sprintf ( Buff , "!f%2.2d%s" , Font , ( char * ) kgGetOthFont ( Font ) ) ;
      kgWrite ( M , Buff ) ;
      kgUpdateWidget ( M ) ;
      kgUpdateOn ( D ) ;
      WriteMsg ( D ) ;
      switch ( item ) {
          case 1:
          break;
      }
      return ret;
  }
  static int GetFontbutton1callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIN *B;
      DIW *W;
      DIM *M;
      int n , ret = 0 , count;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
      char Buff [ 500 ] = "/usr/share/fonts/";
      char **List;
      int Font;
      D = ( DIALOG * ) Tmp;
      B = ( DIN * ) kgGetWidget ( Tmp , i ) ;
      W = ( DIW * ) kgGetNamedWidget ( D , ( char * ) "Select" ) ;
      M = ( DIM * ) kgGetNamedWidget ( D , ( char * ) "Msg" ) ;
      Font = * ( W->df ) -1;
      n = B->nx*B->ny;
      if ( kgFolderBrowser ( NULL , 100 , 200 , Buff , ( char * ) "*" ) ) {
          if ( ! CheckFontFile ( Buff ) ) return 0;
          kgAddFont ( Buff ) ;
          List = ( char ** ) kgGetList ( W ) ;
          kgFreeDouble ( ( void ** ) List ) ;
          List = ( char ** ) kgGetFontList ( ) ;
          count = 0;
          while ( List [ count ] != NULL ) count++;
          Font = count-1;
          * ( W->df ) = count;
          kgSetList ( W , ( void ** ) List ) ;
          kgWrite ( M , List [ Font ] ) ;
          kgUpdateWidget ( W ) ;
          kgUpdateWidget ( M ) ;
          kgUpdateOn ( D ) ;
          WriteMsg ( D ) ;
      }
      switch ( butno ) {
          case 1:
          break;
      }
      return ret;
  }
  static void GetFontbutton1init ( DIN *B , void *ptmp ) {
      void **pt = ( void ** ) ptmp; //pt [ 0 ] is arg 
  }
  static int GetFontsplbutton1callback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIL *B;
      DIW *W;
      int n , ret = 1;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
      D = ( DIALOG * ) Tmp;
      B = ( DIL * ) kgGetWidget ( Tmp , i ) ;
      int *ipt = ( int * ) pt [ 0 ] ;
      W = ( DIW * ) kgGetNamedWidget ( D , ( char * ) "Select" ) ;
      n = B->nx;
      switch ( butno ) {
          case 1:
          break;
          case 2:
          *ipt = * ( W->df ) -1;
          break;
      }
      return ret;
  }
  static void GetFontsplbutton1init ( DIL *B , void *ptmp ) {
      void **pt = ( void ** ) ptmp; //pt [ 0 ] is arg 
  }
  static void GetFontbrowser2init ( DIS *S , void *ptmp ) {
      void **pt = ( void ** ) ptmp; //pt [ 0 ] is arg 
  }
  static int GetFontinit ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
      int ret = 1 , k = 0;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      DIW *W;
      DIM *M;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
 /* pt[0] is inputs, given by caller */
      char **List;
      int Font;
      int *ipt = ( int * ) pt [ 0 ] ;
      W = ( DIW * ) kgGetNamedWidget ( D , ( char * ) "Select" ) ;
      M = ( DIM * ) kgGetNamedWidget ( D , ( char * ) "Msg" ) ;
      List = kgGetFontList ( ) ;
      while ( List [ k ] != NULL ) {
//   printf("%s \n",List[k]);
          k++;
      }
      Font = *ipt;
      if ( Font < 0 ) Font = 0;
      Font %= k;
      kgSetList ( W , ( void ** ) List ) ;
      * ( W->df ) = Font+1;
      kgUpdateWidget ( W ) ;
      kgWrite ( M , List [ Font ] ) ;
      kgUpdateWidget ( M ) ;
      kgUpdateOn ( D ) ;
      WriteMsg ( D ) ;
      return ret;
  }
  static int GetFontcleanup ( void *Tmp ) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 1;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
 /* pt[1] is outputs, if any  to be given to caller */
 /* pt[0] is inputs, given by caller */
      return ret;
  }
  static int ModifyGetFont ( void *Tmp , int GrpId ) {
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
 /* pt[0] is inputs given by caller */
      DIA *d;
      int i , n , k = 0;
      int *ipt = ( int * ) pt [ 0 ] ;
      int Font = *ipt;
      d = D->d;
      i = 0;while ( d [ i ] .t != NULL ) {;
          i++;
      };
      n = 1;
//  strcpy(D->name,"Kulina Designer ver 2.0");    /*  Dialog name you may change */
#if 0
      if ( D->fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          kgDisplaySize ( & xres , & yres ) ;
      // D->xo=D->yo=0; D->xl = xres-10; D->yl=yres-80;
      }
      else { // for fullscreen
          int xres , yres;
          kgDisplaySize ( & xres , & yres ) ;
          D->xo = D->yo = 0; D->xl = xres; D->yl = yres;
//     D->StackPos = 1; // you may need it
      } /* end of fullscreen mode */
#endif
#if 0
      W = ( DIW * ) kgGetNamedWidget ( D , ( char * ) "Select" ) ;
      B = ( DIN * ) kgGetNamedWidget ( D , ( char * ) "Add" ) ;
      M = ( DIM * ) kgGetNamedWidget ( D , ( char * ) "Msg" ) ;
      I = ( DIS * ) kgGetNamedWidget ( D , ( char * ) "Info" ) ;
#endif
      return GrpId;
  }
  static int GetFontCallBack ( void *Tmp , void *tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
      int ret = 0;
      DIALOG *D;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
          }
      }
      return ret;
  }
  static int GetFontResizeCallBack ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 0;
      int xres , yres , dx , dy;
      DIALOG *D;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
      D = ( DIALOG * ) Tmp;
      kgGetWindowSize ( D , & xres , & yres ) ;
      dx = xres - D->xl;
      dy = yres - D->yl;
  /* extra code */
      D->xl = xres;
      D->yl = yres;
      kgRedrawDialog ( D ) ;
      return ret;
  }
  static int GetFontWaitCallBack ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
      int ret = 0;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
      return ret;
  }
  static void ModifyGetFontGc ( void *Tmp ) {
      DIALOG *D;
      Gclr *gc;
      D = ( DIALOG * ) Tmp;
      gc = & ( D->gc ) ;
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   kgColorTheme(D,220,220,200);
   kgColorTheme1(D,220,220,200);
   kgColorTheme2(D,220,220,200);
   kgDefaultGuiTheme(gc);
   kgGrayGuiTheme(gc);
   gc->FontSize =9;
   gc->FontGuiSize =9;
   gc->InputFontSize =8;
   gc->MenuFont = 21;
   gc->PromptFont = 21;
   gc->ButtonFont = 21;
   gc->MsgFont = 21;
   gc->Font=23;
   kgMkgclr((char *)"GetFont",Tmp);
*/
  }
  static int GetFontGroup ( DIALOG *D , void **v , void *pt ) {
      int GrpId = 0 , oitems = 0 , i , j;
      DIA *d = NULL , *dtmp;
      char **menu0 ;
      menu0 = ( char ** ) malloc ( sizeof ( char * ) *8 ) ;
      menu0 [ 7 ] = NULL;
      menu0 [ 0 ] = ( char * ) malloc ( 4 ) ;
      strcpy ( menu0 [ 0 ] , ( char * ) "one" ) ;
      menu0 [ 1 ] = ( char * ) malloc ( 4 ) ;
      strcpy ( menu0 [ 1 ] , ( char * ) "two" ) ;
      menu0 [ 2 ] = ( char * ) malloc ( 4 ) ;
      strcpy ( menu0 [ 2 ] , ( char * ) "two" ) ;
      menu0 [ 3 ] = ( char * ) malloc ( 6 ) ;
      strcpy ( menu0 [ 3 ] , ( char * ) "three" ) ;
      menu0 [ 4 ] = ( char * ) malloc ( 5 ) ;
      strcpy ( menu0 [ 4 ] , ( char * ) "four" ) ;
      menu0 [ 5 ] = ( char * ) malloc ( 5 ) ;
      strcpy ( menu0 [ 5 ] , ( char * ) "five" ) ;
      menu0 [ 6 ] = ( char * ) malloc ( 4 ) ;
      strcpy ( menu0 [ 6 ] , ( char * ) "six" ) ;
      char *prompt0 ;
      prompt0 = ( char * ) malloc ( 13 ) ;
      strcpy ( prompt0 , ( char * ) "Select Font " ) ;
      DIW w0 = {
          'w' , 10 , 15 , 380 , 55 , 6 , ( int * ) v [ 0 ] , prompt0 , menu0 , \
          NULL , GetFontbrowser1callback , /* *args , callback */
      0 };
      strcpy ( w0.Wid , ( char * ) "Select" ) ;
      w0.item = -1;
      BUT_STR *butn1 = NULL;
      butn1 = ( BUT_STR * ) malloc ( sizeof ( BUT_STR ) *1 ) ;
      butn1 [ 0 ] .sw = 1;
      strcpy ( butn1 [ 0 ] .title , ( char * ) "Add New Font" ) ;
      butn1 [ 0 ] .xpmn = NULL;
      butn1 [ 0 ] .xpmp = NULL;
      butn1 [ 0 ] .xpmh = NULL;
      butn1 [ 0 ] .bkgr = -1;
      butn1 [ 0 ] .butncode = 31;
      DIN b1 = {
          'n' , 110 , 105 , 270 , 147 , 2 , 2 , 150 , 32 , 1 , 1 , 4 , 0.500000 , \
          0 , 0 , 0 , 1 , /* button type and roundinfg factor \
           ( 0-0.5 ) , bordr , hide , nodrawbkgr*/
          butn1 , GetFontbutton1callback , /* Callbak */
          NULL /* any args */
      };
      strcpy ( b1.Wid , ( char * ) "Add" ) ;
      b1.item = -1;
      BUT_STR *butn2 = NULL;
      butn2 = ( BUT_STR * ) malloc ( sizeof ( BUT_STR ) *2 ) ;
      butn2 [ 0 ] .sw = 1;
      strcpy ( butn2 [ 0 ] .title , ( char * ) "!c03Cancel" ) ;
      butn2 [ 0 ] .xpmn = NULL;
      butn2 [ 0 ] .xpmp = NULL;
      butn2 [ 0 ] .xpmh = NULL;
      butn2 [ 0 ] .bkgr = -1;
      butn2 [ 0 ] .butncode = 31;
      butn2 [ 1 ] .sw = 1;
      strcpy ( butn2 [ 1 ] .title , ( char * ) "!c38Okay" ) ;
      butn2 [ 1 ] .xpmn = NULL;
      butn2 [ 1 ] .xpmp = NULL;
      butn2 [ 1 ] .xpmh = NULL;
      butn2 [ 1 ] .bkgr = -1;
      butn2 [ 1 ] .butncode = 31;
      DIL h2 = {
          'h' , 119 , 298 , 275 , 331 , 2 , 0 , 72 , 25 , 2 , 1 , 8 , 0.500000 , 0 , \
          0 , 0 , 1 , /* button type and roundinfg factor \
           ( 0-0.5 ) , bordr , hide , nodrawbkgr*/
          butn2 , GetFontsplbutton1callback , /* Callbak */
          NULL /* any args */
      };
      strcpy ( h2.Wid , ( char * ) "Spl" ) ;
      h2.item = -1;
      DIM m3 = {
      'B' , 15 , 72 , 375 , 96 , 0 , 0 };
      strncpy ( m3.msg , ( char * ) "" , 499 ) ;
      strcpy ( m3.Wid , ( char * ) "Msg" ) ;
      m3.item = -1;
      DIS s4 = {
          's' , 15 , 162 , 376 , 283 , 5 , NULL , NULL , \
          NULL , NULL , NULL , /* *args , callback */
//     line width,offset (not used),scroll width,highlight item(not used)
//     border on/off,bkgr on/off,hide on/off
//     uses Gclr items: msg_fill,msg_char,msg_bodr,scroll_fill,scroll_dim,scroll_vbright)
      20 , 6 , 22 , 1 , 1 , 1 , 0 };
      strcpy ( s4.Wid , ( char * ) "Info" ) ;
      s4.item = -1;
      dtmp = D->d;
      i = 0;
      if ( dtmp != NULL ) while ( dtmp [ i ] .t != NULL ) i++;
      dtmp = ( DIA * ) realloc ( dtmp , sizeof ( DIA ) * ( i+6 ) ) ;
      d = dtmp+i;
      d [ 5 ] .t = NULL;
      d [ 0 ] .t = ( DIT * ) malloc ( sizeof ( DIW ) ) ;
      *d [ 0 ] .w = w0;
      d [ 0 ] .w->item = -1;
      d [ 1 ] .t = ( DIT * ) malloc ( sizeof ( DIN ) ) ;
      *d [ 1 ] .N = b1;
      d [ 1 ] .N->item = -1;
      GetFontbutton1init ( d [ 1 ] .N , pt ) ;
      d [ 2 ] .t = ( DIT * ) malloc ( sizeof ( DIL ) ) ;
      *d [ 2 ] .h = h2;
      d [ 2 ] .h->item = -1;
      GetFontsplbutton1init ( d [ 2 ] .h , pt ) ;
      d [ 3 ] .t = ( DIT * ) malloc ( sizeof ( DIM ) ) ;
      *d [ 3 ] .m = m3;
      d [ 3 ] .m->item = -1;
      d [ 4 ] .t = ( DIT * ) malloc ( sizeof ( DIS ) ) ;
      *d [ 4 ] .s = s4;
      d [ 4 ] .s->item = -1;
      GetFontbrowser2init ( d [ 4 ] .s , pt ) ;
      d [ 5 ] .t = NULL;
      GrpId = kgOpenGrp ( D ) ;
      D->d = dtmp;
      j = 0;
      while ( d [ j ] .t != NULL ) { kgAddtoGrp ( D , \
      GrpId , ( void * ) ( d [ j ] .t ) ) ;j++;}
      return GrpId;
  }
/* One can also use the following code to add Widgets to an existing Dialog */
  int MakeGetFontGroup ( DIALOG *D , void *arg ) {
      int GrpId;
      WIDGETGRP *Gpt;
/*************************************************

    Browser1  1 data value

*************************************************/
      int *v0 ;
      v0 = ( int * ) malloc ( sizeof ( int ) ) ;
      *v0 = 1;
      void** v = ( void ** ) malloc ( sizeof ( void* ) *2 ) ;
      v [ 1 ] = NULL;
      v [ 0 ] = ( void * ) ( v0 ) ;
      void *pt = NULL; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
      GrpId = GetFontGroup ( D , v , pt ) ;
      Gpt = kgGetWidgetGrp ( D , GrpId ) ;
      Gpt->arg = v; // kulina will double free this; you may modify
      return GrpId;
  }
  int GetFont ( void *parent , void **v , void *pt ) {
      int ret = 1 , GrpId , k;
      DIALOG D;
      DIA *d = NULL;
      D.VerId = 2107030000;
      kgInitUi ( & D ) ;
      D.d = NULL;
#if 1
      GrpId = GetFontGroup ( & D , v , pt ) ;
#else 
      GrpId = MakeGetFontGroup ( & D , pt ) ; // can try this also
#endif 
      d = D.d;
      D.d = d;
      D.bkup = 1; /* set to 1 for backup */
      D.bor_type = 4;
      D.df = 4;
      D.tw = 4;
      D.bw = 4;
      D.lw = 4;
      D.rw = 4;
      D.xo = 858; /* Position of Dialog */
      D.yo = 254;
      D.xl = 392; /* Length of Dialog */
      D.yl = 337; /* Width of Dialog */
      D.Initfun = GetFontinit; /* init fuction for Dialog */
      D.Cleanupfun = GetFontcleanup; /* init fuction for Dialog */
      D.kbattn = 0; /* 1 for drawing keyborad attention */
      D.butattn = 0; /* 1 for drawing button attention */
      D.fullscreen = 0; /* 1 for for fullscreen mode */
      D.NoTabProcess = 0; /* 1 for disabling Tab use */
      D.Deco = 1; /* 1 for Window Decorration */
      D.transparency = 0.000000; /* float 1.0 for full transparency */
      D.Newwin = 1; /* 1 for new window not yet implemented */
      D.DrawBkgr = 1; /* 1 for drawing background */
      D.Bkpixmap = NULL; /* background image */
      D.Sticky = 0; /* 1 for stickyness */
      D.Resize = 0; /* 1 for Resize option */
      D.MinWidth = 100; /* for Resize option */
      D.MinHeight = 100; /* for Resize option */
#if 1 
      D.Callback = GetFontCallBack; /* default callback */
#else 
      D.Callback = NULL;
#endif
      D.ResizeCallback = GetFontResizeCallBack; /* Resize callback */
#if 0 
      D.WaitCallback = NULL; /* Wait callback */
#else 
      D.WaitCallback = GetFontWaitCallBack; /* Wait callback */
#endif
      D.Fixpos = 1; /* 1 for Fixing Position */
      D.NoTaskBar = 0; /* 1 for not showing in task bar*/
      D.NoWinMngr = 0; /* 1 for no Window Manager*/
      D.StackPos = 0; /* -1 , 0 , 1 for for Stack Position -1:below 0:normal 1:above*/
      D.Shapexpm = NULL; /* PNG/jpeg file for window shape;
      Black color will not be drawn */
      D.parent = parent; /* 1 for not showing in task bar*/
      D.pt = pt; /* any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 2.0");    /*  Dialog name you may change */
      if ( D.fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          kgDisplaySize ( & xres , & yres ) ;
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
      }
      else { // for fullscreen
          int xres , yres;
          kgDisplaySize ( & xres , & yres ) ;
          D.xo = D.yo = 0; D.xl = xres; D.yl = yres;
//     D.StackPos = 1; // you may need it
      } /* end of fullscreen mode */
      ModifyGetFont ( & D , GrpId ) ; /* add extras to gui*/
      ModifyGetFontGc ( & D ) ; /* set colors for gui if do not like default*/
      ret = kgUi ( & D ) ;
      kgCleanUi ( & D ) ;
      return ret;
  }
  void *kgGetFreeTypeFont ( void *parent , void *args ) {
/*************************************************

    Browser1  1 data value

*************************************************/
      int v0 = 1;
      void* v [ 1 ] ;
      v [ 0 ] = ( void * ) ( & v0 ) ;
      void *pt [ 2 ] = {NULL , NULL}; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
      pt [ 0 ] = args;
      GetFont ( parent , v , ( void * ) pt ) ;
      return pt [ 1 ] ;
  }
/* END OF kgGetFreeTypeFont */
