#include <kulina.h>
  void * kgShadedImage ( void *image , int xsize , \
  int ysize , int fillcolor ) {
      int ReSize = 8,width=3;
      int aspc = 1;
      void *fid;
      void *img = NULL;
      char *tmpdir , flname [ 200 ] ;
      float length = 0.0 , fac , th , tw , vmin , \
      vmax , size , lnwidth = 2 , xm , ym;
      float h , s , v , red , blue , green;
      float rfac=0.15;
      int r , g , b;
      fid = kgInitImage ( xsize , ysize , ReSize ) ;
      aspc = 1;
      size = xsize;
      if ( fillcolor >= 0 ) kgGetRGB ( ( DIG * ) fid , \
      fillcolor , & r , & g , & b ) ;
      else {
          fillcolor = -fillcolor;
          b = ( fillcolor%1000 ) ;
          g = ( fillcolor/1000 ) %1000;
          r = ( fillcolor/1000000 ) %1000;
          b = b%256;g = g%256;r = r%256;
          fillcolor = 127;
          kgChangeColor ( fid , fillcolor , r , g , b ) ;
      }
      RGBtoHSV ( ( float ) r , ( float ) g , ( float ) b , & h , & s , & v ) ;
      if ( v > 1 ) v = 1.0;
      if ( ysize > size ) {size = ysize;aspc = 0;}
      if ( fid != NULL ) {
          kgUserFrame ( fid , -3. , -3. , ( float ) xsize+3. , ( float ) ysize+3. ) ;
          th = ( float ) ysize*.5;
          tw = ( float ) width;
          th = HFAC*tw;
          if ( aspc ) {if ( th > 0.6*ysize ) th = 0.6*ysize;}
          else { if ( th > 0.6*xsize ) th = 0.6*xsize;}
          kgLineWidth ( fid , 2 ) ;
          lnwidth = 2;
          vmin = 0.85*v;
          vmax = 1.3*v;
          if ( vmax > 1.0 ) vmax = 1.0;
          HSVtoRGB ( & red , & green , & blue , h , s , vmax ) ;
          kgChangeColor ( fid , 150 , red , green , blue ) ;
          vmax = 1.1*v;
          if ( vmax > 1.0 ) vmax = 1.0;
          HSVtoRGB ( & red , & green , & blue , h , s , vmax ) ;
          kgChangeColor ( fid , 151 , red , green , blue ) ;
          HSVtoRGB ( & red , & green , & blue , h , s , v*0.5 ) ;
          kgChangeColor ( fid , 149 , red , green , blue ) ;
          HSVtoRGB ( & red , & green , & blue , h , s , v*0.8 ) ;
          kgChangeColor ( fid , 148 , red , green , blue ) ;
          vmax = 1.5*v;
          if ( vmax > 1.0 ) vmax = 1.0;
          vmin = vmax*0.75;
          HSVtoRGB ( & red , & green , & blue , h , s , vmin ) ;
          kgChangeColor ( fid , 152 , red , green , blue ) ;
          kgLineWidth ( fid , 5 ) ;
          lnwidth = 5;
          kgRoundedRectangleFill ( fid , ( float ) xsize*0.5+1.0 , \
               ( float ) ysize*0.5-1.0 , \
           ( float ) xsize , ( float ) ysize , 0 , 0 , rfac ) ;
          kgRoundedRectangleFill ( fid , ( float ) xsize*0.5-1.0 , \
               ( float ) ysize*0.5+1.0 , \
           ( float ) xsize , ( float ) ysize , 0 , 15 , rfac ) ;
          kgRoundedRectangleShade0 ( fid , ( float ) xsize*0.5 , ( float ) ysize*0.5 , \
           ( float ) xsize , ( float ) ysize , fillcolor , rfac , v , vmax*0.95 ) ;
          xm = xsize*0.5;
          ym = ysize*0.5;
          if ( image != NULL ) { kgDrawImage ( fid , image , \
              3.0 , 3.0 , xsize-3. , ysize-3. ) ;
          }
          img = kgGetResizedImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      return img;
  }
