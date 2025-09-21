#include "kulina.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "gprivate.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#if 0
  typedef struct _img_str {
      int xln;
      int yln;
      void *img;
  } IMG_STR;
  typedef struct _font_str {
      char code; //i internal , t char table f fontnane
      int fontno; // for internal
      char *name; // font name for f type
      void *Imgs; // ( IMG_STR ** ) pointer for character table;
      int Size;   // fot f and i
  } FONT_STR;
#endif
/* Replace this function with something useful. */
  static unsigned char *draw_bitmap ( unsigned char *pixls , FT_Bitmap* bitmap , FT_Int x , FT_Int y ,int WIDTH,int HEIGHT)  \
      {
      FT_Int i , j , p , q;
      FT_Int x_max = x + bitmap->width;
      FT_Int y_max = y + bitmap->rows;
      int k = 0;
  /* for simplicity, we assume that `bitmap->pixel_mode' */
  /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */
      for ( j = y , q = 0; j < y_max; j++, q++ ) {
          for ( i = x , p = 0; i < x_max; i++, p++ ) {
              if ( i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT ) continue;
              pixls [ j*WIDTH+i ] = bitmap->buffer [ q * bitmap->width + p ] ;
          }
      }
      return pixls;
  }
  static void * Make_image ( unsigned char *pixls , int WIDTH,int HEIGHT) {
      int i , j;
      void *img , *fid , *img2;
      int clr;
#if 0
      fid = kgInitImage ( WIDTH , HEIGHT , 1 ) ;
      kgLineColor ( fid , 0 ) ;
      kgMove2f ( fid , ( float ) WIDTH-1. , ( float ) HEIGHT-1. ) ;
      kgDraw2f ( fid , ( float ) 0.0 , ( float ) HEIGHT-1. ) ;
      kgDraw2f ( fid , ( float ) 0.0 , ( float ) 0.0 ) ;
      kgDraw2f ( fid , ( float ) WIDTH-1. , ( float ) 0.0 ) ;
      kgDraw2f ( fid , ( float ) WIDTH-1. , ( float ) HEIGHT-1. ) ;
      img = kgGetSharpImage ( fid ) ;
      kgCloseImage ( fid ) ;
#endif
      img2 = kgCreateImage ( WIDTH , HEIGHT ) ;
      int k = -1;
      for ( i = 0; i < HEIGHT; i++ ) {
          for ( j = 0; j < WIDTH; j++ ) {
              k++;
              if ( pixls [ k ] == 0 ) continue;
              kgSetPixelAlpha ( img2 , j , i , 255-pixls [ k ] ) ;
          }
      }
//  kgSetImageColor(img2,255,0,0);
//  kgMergeImages(img,img2,0,0);
//  kgFreeImage(img2);
      img = img2;
//  kgWriteImage(img,"Junk.png");
      return img;
  }
  void * kgMakeFixedFontImg ( char *filename , char *text , int Size ,int Gap) {
      FT_Library library;
      FT_Face face;
      FT_GlyphSlot slot;
      FT_Matrix matrix; /* transformation matrix */
      FT_Vector pen; /* untransformed origin */
      FT_Error error;
      int WIDTH,HEIGHT;
      double angle;
      int target_height;
      int n , num_chars , i , j;
      int Ht;
      double dx , dy;
      IMG_STR *Img = NULL;
      num_chars = strlen ( text ) ;
//      angle = ( 25.0 / 360 ) * 3.14159 * 2; /* use 25 degrees */
      angle = ( 0.0 / 360 ) * 3.14159 * 2; /* use 25 degrees */
      Ht = Size*1.2;
      HEIGHT = 2*Size+2;
      WIDTH = Size;
      target_height = HEIGHT;
      void *img;
      unsigned char *pixls=NULL;
      error = FT_Init_FreeType ( & library ) ; /* initialize library */
  /* error handling omitted */
      error = FT_New_Face ( library , filename , 0 , & face ) ;
          /* create face object */
  /* error handling omitted */
  /* use 50pt at 100dpi */
      error = FT_Set_Char_Size ( face , Ht * 64 , 0 , 120 , 0 ) ;
           /* set character size */
  /* error handling omitted */
  /* cmap selection omitted;                                        */
  /* for simplicity we assume that the font contains a Unicode cmap */
      slot = face->glyph;
  /* set up matrix */
      matrix.xx = ( FT_Fixed ) ( 0.95*0x10000L ) ;
      matrix.xy = 0;
      matrix.yx = 0;
      matrix.yy = ( FT_Fixed ) ( 1*0x10000L ) ;
      WIDTH = WIDTH*num_chars+1;
      pixls = ( unsigned char * ) calloc ( sizeof  \
          ( unsigned char ) , WIDTH*HEIGHT ) ;
  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
      pen.x = 300 * 64;
      pen.y = ( target_height - 200 ) * 64;
      pen.x = 0 * 64;
      pen.y = ( 2*Size/5+2 ) * 64;
      for ( n = 0; n < num_chars; n++ ) {
    /* set transformation */
          FT_Set_Transform ( face , & matrix , & pen ) ;
    /* load glyph image into the slot (erase previous one) */
          error = FT_Load_Char ( face , text [ n ] , FT_LOAD_RENDER ) ;
          if ( error ) continue; /* ignore errors */
    /* now, draw to our target surface (convert position) */
          draw_bitmap ( pixls , & slot->bitmap , slot->bitmap_left , target_height - slot->bitmap_top,WIDTH,HEIGHT ) ;
              
    /* increment pen position */
//    pen.x += slot->advance.x;
          pen.x += ( Size*64 )+Gap*64 ;
          pen.y += slot->advance.y;
      }
      img = Make_image ( pixls , WIDTH ,HEIGHT) ;
      FT_Done_Face ( face ) ;
      FT_Done_FreeType ( library ) ;
      free ( pixls ) ;
      pixls=NULL;
      Img = ( IMG_STR * ) malloc ( sizeof ( IMG_STR ) ) ;
      Img->xln = WIDTH-1;
      Img->yln = 0.0;
      Img->img = img;
      return Img;
  }
  void * kgMakeFontImg ( char *filename , char *text , int Size ,int Gap) {
      FT_Library library;
      FT_Face face;
      FT_GlyphSlot slot;
      FT_Matrix matrix; /* transformation matrix */
      FT_Vector pen; /* untransformed origin */
      FT_Error error;
      double angle;
      double hfac=1.2,wfac=1.0;
      int target_height;
      int n , num_chars , i , j;
      int Ht,Wd;
      int WIDTH,HEIGHT;
      double dx , dy;
      num_chars = strlen ( text ) ;
      angle = ( 25.0 / 360 ) * 3.14159 * 2; /* use 25 degrees */
      angle = ( 0.0 / 360 ) * 3.14159 * 2; /* use 25 degrees */
      Ht = Size*hfac;
      Wd =Size*wfac;
      HEIGHT =(int)( 2.0*Size);
      WIDTH = Size;
      target_height =(int)( HEIGHT );
      IMG_STR *Img;
      void *img;
      unsigned char *pixls=NULL;
      error = FT_Init_FreeType ( & library ) ; /* initialize library */
  /* error handling omitted */
      error = FT_New_Face ( library , filename , 0 , & face ) ;
          /* create face object */
  /* error handling omitted */
  /* use 50pt at 100dpi */
//TCB      error = FT_Set_Char_Size ( face , Ht * 64 , 0 , 120 , 0 ) ;
  //    error = FT_Set_Char_Size ( face , Ht * 64 , 0 , 120 , 0 ) ;
     error = FT_Set_Char_Size ( face ,Wd*64, Ht*64, 120 , 120 ) ;
           /* set character size */
  /* error handling omitted */
  /* cmap selection omitted;                                        */
  /* for simplicity we assume that the font contains a Unicode cmap */
      slot = face->glyph;
  /* set up matrix */
      matrix.xx = ( FT_Fixed ) ( 1*0x10000L ) ;
      matrix.xy = 0;
      matrix.yx = 0;
      matrix.yy = ( FT_Fixed ) ( 1*0x10000L ) ;
      WIDTH = 2*WIDTH*num_chars+4;
      pixls = ( unsigned char * ) calloc ( sizeof  \
          ( unsigned char ) , WIDTH*HEIGHT ) ;
  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
      pen.x = 300 * 64;
      pen.y = ( target_height - 200 ) * 64;
      pen.x = 0 * 64;
      pen.y = (int)( Size/2.0+0.5 ) * 64;
      for ( n = 0; n < num_chars; n++ ) {
          FT_Set_Transform ( face , & matrix , & pen ) ;
    /* set transformation */
    /* load glyph image into the slot (erase previous one) */
          error = FT_Load_Char ( face , text [ n ] , FT_LOAD_RENDER ) ;
          if ( error ) continue; /* ignore errors */
    /* now, draw to our target surface (convert position) */
          draw_bitmap ( pixls , & slot->bitmap , slot->bitmap_left , target_height - slot->bitmap_top,WIDTH,HEIGHT ) ;
    /* increment pen position */
          pen.x += slot->advance.x+32+Gap*64;
          pen.y += slot->advance.y;
      }
      img = Make_image ( pixls , WIDTH ,HEIGHT) ;
      FT_Done_Face ( face ) ;
      FT_Done_FreeType ( library ) ;
      free ( pixls ) ;
      pixls=NULL;
      Img = ( IMG_STR * ) malloc ( sizeof ( IMG_STR ) ) ;
      Img->xln = pen.x/64;
      Img->yln = pen.y/64;
      Img->img = img;
      return Img;
  }
  void * uiMakeFontImg ( char *filename , char *text , int Size ,int Gap,float hfac,float wfac) {
      FT_Library library;
      FT_Face face;
      FT_GlyphSlot slot;
      FT_Matrix matrix; /* transformation matrix */
      FT_Vector pen; /* untransformed origin */
      FT_Error error;
      double angle;
//      double hfac=1.2,wfac=1.0;
      int target_height;
      int n , num_chars , i , j;
      int Ht,Wd;
      int WIDTH,HEIGHT;
      double dx , dy;
      num_chars = strlen ( text ) ;
      angle = ( 25.0 / 360 ) * 3.14159 * 2; /* use 25 degrees */
      angle = ( 0.0 / 360 ) * 3.14159 * 2; /* use 25 degrees */
      Ht = (int)(Size*1.2*hfac+0.5);
      Wd =Size*wfac;
      HEIGHT =(int)( Ht*2.0);
      WIDTH = Wd;
      target_height =(int)( HEIGHT );
      IMG_STR *Img;
      void *img;
      unsigned char *pixls=NULL;
      error = FT_Init_FreeType ( & library ) ; /* initialize library */
  /* error handling omitted */
      error = FT_New_Face ( library , filename , 0 , & face ) ;
          /* create face object */
  /* error handling omitted */
  /* use 50pt at 100dpi */
//TCB      error = FT_Set_Char_Size ( face , Ht * 64 , 0 , 120 , 0 ) ;
  //    error = FT_Set_Char_Size ( face , Ht * 64 , 0 , 120 , 0 ) ;
     error = FT_Set_Char_Size ( face ,Wd*64, Ht*64, 120 , 120 ) ;
           /* set character size */
  /* error handling omitted */
  /* cmap selection omitted;                                        */
  /* for simplicity we assume that the font contains a Unicode cmap */
      slot = face->glyph;
  /* set up matrix */
      matrix.xx = ( FT_Fixed ) ( 1*0x10000L ) ;
      matrix.xy = 0;
      matrix.yx = 0;
      matrix.yy = ( FT_Fixed ) ( 1*0x10000L ) ;
      WIDTH = 2*WIDTH*num_chars+4;
      pixls = ( unsigned char * ) calloc ( sizeof  \
          ( unsigned char ) , WIDTH*HEIGHT ) ;
  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
      pen.x = 300 * 64;
      pen.y = ( target_height - 200 ) * 64;
      pen.x = 0 * 64;
      pen.y = (int)( hfac*Size/2.0+0.5 ) * 64;
  //    pen.y = 0 * 64;
      for ( n = 0; n < num_chars; n++ ) {
          FT_Set_Transform ( face , & matrix , & pen ) ;
    /* set transformation */
    /* load glyph image into the slot (erase previous one) */
          error = FT_Load_Char ( face , text [ n ] , FT_LOAD_RENDER ) ;
          if ( error ) continue; /* ignore errors */
    /* now, draw to our target surface (convert position) */
          draw_bitmap ( pixls , & slot->bitmap , slot->bitmap_left , target_height - slot->bitmap_top,WIDTH,HEIGHT ) ;
    /* increment pen position */
          pen.x += slot->advance.x+32+Gap*64;
          pen.y += slot->advance.y;
      }
      img = Make_image ( pixls , WIDTH ,HEIGHT) ;
      FT_Done_Face ( face ) ;
      FT_Done_FreeType ( library ) ;
      free ( pixls ) ;
      pixls=NULL;
      Img = ( IMG_STR * ) malloc ( sizeof ( IMG_STR ) ) ;
      Img->xln = pen.x/64;
      Img->yln = pen.y/64;
      Img->img = img;
      return Img;
  }
  void *uiFreeImgStr(void *Itmp) {
    IMG_STR *Img= (IMG_STR*)Itmp;;
    if(Itmp==NULL) return NULL;
    if(Img->img != NULL) kgFreeGmImage(Img->img);
    return NULL;
  }
  void * uiFreeImgStrs(void * Itmp) {
     int i;
     IMG_STR **IMGS = (IMG_STR **) Itmp;
     if(IMGS != NULL) {
//     printf("Inside  uiFreeImgStrs\n");
     for ( i = 0;i < 128;i++ ){
      IMGS[i] =  uiFreeImgStr(IMGS[i]);
     }
     free(IMGS);
     }
//     printf("Return\n");
//     fflush(stdout);
     return NULL;
  }
  void **kgFontChars ( char *Font , int Size ) {
      IMG_STR **Imgs = NULL;
      Imgs = ( IMG_STR ** ) malloc ( sizeof ( IMG_STR * ) *128 ) ;
      void *img;
      char txt [ 2 ] = "";
      int i , k;
      for ( i = 0;i < 128;i++ ) Imgs [ i ] = NULL;
      for ( i = 31;i < 128;i++ ) {
          txt [ 0 ] = i;
          Imgs [ i ] = ( IMG_STR * ) kgMakeFontImg ( Font , txt , 2*Size ,0) ;
          img = Imgs [ i ]->img ; 
          Imgs [ i ]->img = kgResizeImage ( img , 0.5 ) ;
          kgFreeGmImage(img);
          Imgs [ i ]->xln = ( int ) ( Imgs [ i ]->xln*0.5+0.9 ) ;
          Imgs [ i ]->yln = ( int ) ( Imgs [ i ]->yln*0.5+0.9 ) ;
      }
      return ( void ** ) Imgs;
  }
  void **kgFixedFontChars ( char *Font , int Size ) {
      IMG_STR **Imgs = NULL;
      Imgs = ( IMG_STR ** ) malloc ( sizeof ( IMG_STR * ) *128 ) ;
      void *img;
      char txt [ 2 ] = "";
      int i , k;
      for ( i = 0;i < 128;i++ ) Imgs [ i ] = NULL;
      for ( i = 31;i < 128;i++ ) {
          txt [ 0 ] = i;
          Imgs [ i ] = ( IMG_STR * ) kgMakeFixedFontImg ( Font , txt , 2*Size ,0) ;
          img = Imgs [ i ]->img ; 
          Imgs [ i ]->img = kgResizeImage ( img , 0.5 ) ;
          kgFreeGmImage(img);
          Imgs [ i ]->xln = ( int ) ( Imgs [ i ]->xln*0.5+0.9 ) ;
          Imgs [ i ]->yln = ( int ) ( Imgs [ i ]->yln*0.5+0.9 ) ;
      }
      return ( void ** ) Imgs;
  }
  void *uiMakeString ( void *Fstr , char *str , int Ht , int Gap ) {
      int n = strlen ( str ) ;
      int lng = 0 , shft;
      int i , pos;
      void *img = NULL;
      GMIMG *gimg;
      IMG_STR **Imgs;
      IMG_STR *IMG;
      char *fontname;
      void *fid;
      int fontno;
      float xsize,ysize;
      float th,tw,xx,yy;
      char Buf[1000];
      char stmp[10]="";
      FONT_STR *F = ( FONT_STR * ) Fstr;
      if(F->Imgs== NULL) F->code ='i';
      if((F->code=='f') &&(F->name == NULL)) F->code='i';
//      printf("Code = %c\n",F->code);
//      fflush(stdout);
      switch ( F->code ) {
          case 't':
          Imgs = (IMG_STR **) F->Imgs;
            i=0;
            while(str[i]!='\0') {
                if(str[i]< ' ') Buf[i]=' ';
                else if(str[i]== 127) Buf[i]=' ';
                else Buf[i]=str[i];
                i++;
            }
            Buf[i]='\0';
          i = 0;
          while ( Buf [i ] != '\0' ) {
              pos = ( int ) Buf [ i ] ;
              if ( Imgs [ pos ] != NULL ) lng = lng+ Imgs [ pos ]->xln+Gap;
              i++;
          }
          img = kgCreateImage ( lng+2 , Ht ) ;
          shft = 0;
          i=0;
          while ( Buf [ i ] != '\0' ) {
              pos = ( int ) Buf [ i ] ;
              if ( Imgs [ pos ] != NULL ) {
                  kgReplaceImage ( img , Imgs [ pos ]->img , shft , 0 ) ;
                  shft += Imgs [ pos ]->xln+Gap;
              }
              i++;
          }
          IMG = (IMG_STR *) malloc(sizeof(IMG_STR));
          IMG->img = img;
          IMG->xln = lng+2;
          IMG->yln =0;
          break;
          case 'f':
            i=0;
            while(str[i]!='\0') {
                if(str[i]< ' ') Buf[i]=' ';
                else if(str[i]== 127) Buf[i]=' ';
                else Buf[i]=str[i];
                i++;
            }
            Buf[i]='\0';
            fontname =F->name;
            IMG  = ( IMG_STR * ) kgMakeFontImg ( fontname , Buf, F->Size,Gap ) ;
            img = kgCreateImage(IMG->xln+1,Ht);
            gimg = (GMIMG *)IMG->img;
            kgReplaceImage ( img , IMG->img , 0 , 0 ) ;
            kgFreeImage(IMG->img);
            IMG->img = img;
            img = IMG->img;
          break;
          case 'i':
          n = n+1;
          xsize = n*(F->Size+Gap);
          ysize = Ht;
      fid = kgInitImage ( xsize+1 , ysize+1 , 4 ) ;
      kgUserFrame ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize ) ;
      kgTextFont ( fid , F->fontno%34 ) ;
      th = 1.3*F->Size;
      tw = F->Size;
      kgTextSize ( fid , th , tw ,(float) Gap ) ;
      kgTextColor ( fid , 0 ) ;
      xx = 0;
      yy = 0.6*F->Size;
      lng = kgStringLength(fid,str);
      i = 0;
      while ( (str [ i ] >= ' ' )||(str[i]=='\t')) {
          kgMove2f ( fid , xx , yy ) ;
          stmp [ 0 ] = str [ i ] ;
	  if(str[i]=='!') {
		  strcpy(stmp,(char *)"!!");
	  }
	  else if(str[i]=='\t') {
//		  stmp[0]=' ';
	  }
	  else if(str[i]==127) stmp[0]=' ';
          kgWriteText ( fid , stmp ) ;
	  stmp[1]='\0';
          xx += (F->Size+Gap);
          i++;
      }
//          img = kgGetResizedImage(fid);
      img = kgGetSharpImage ( fid ) ;
      kgCloseImage ( fid ) ;
          IMG = (IMG_STR *) malloc(sizeof(IMG_STR));
          IMG->img = img;
          IMG->xln = lng;
          IMG->yln =0;
          break;
          deafault:
          break;
      }
      return IMG;
  }
  void *uiFFString ( void *Fstr , char *str , int Ht , int Gap ,float hfac,float wfac) {
      int n = strlen ( str ) ;
      int lng = 0 , shft;
      int i , pos;
      void *img = NULL;
      GMIMG *gimg;
      IMG_STR **Imgs;
      IMG_STR *IMG;
      char *fontname;
      void *fid;
      int fontno;
      float xsize,ysize;
      float th,tw,xx,yy;
      char Buf[1000];
      char stmp[10]="";
      FONT_STR *F = ( FONT_STR * ) Fstr;
            i=0;
            while(str[i]!='\0') {
                if(str[i]< ' ') Buf[i]=' ';
                else if(str[i]== 127) Buf[i]=' ';
                else Buf[i]=str[i];
                i++;
            }
            Buf[i]='\0';
            fontname =F->name;
            IMG  = ( IMG_STR * ) uiMakeFontImg ( fontname , Buf, F->Size,Gap,hfac,wfac ) ;
            img = kgCreateImage(IMG->xln+1,Ht);
            gimg = (GMIMG *)IMG->img;
            kgReplaceImage ( img , IMG->img , 0 , 0 ) ;
            if(IMG->img!= NULL) kgFreeImage(IMG->img);
            IMG->img = img;
      return IMG;
  }
  void *uiMakeFixedString ( void *Fstr , char *str , int Ht , int Gap ) {
      int n = strlen ( str ) ;
      int lng = 0 , shft;
      int i , pos;
      void *img = NULL;
      IMG_STR **Imgs;
      IMG_STR *IMG;
      char *fontname;
      int fontno;
      void *fid;
      char Buf[1000];
      char stmp[10]="";
      float xsize,ysize,xx,yy,th,tw;
      FONT_STR *F = ( FONT_STR * ) Fstr;
      switch ( F->code ) {
          case 't':
          Imgs = (IMG_STR **) F->Imgs;
            i=0;
            while(str[i]!='\0') {
                if(str[i]< ' ') Buf[i]=' ';
                else if(str[i]== 127) Buf[i]=' ';
                else Buf[i]=str[i];
                i++;
            }
            Buf[i]='\0';
          i = 0;
          while ( Buf [i ] != '\0' ) {
              pos = ( int ) Buf [ i ] ;
              if ( Imgs [ pos ] != NULL ) lng = lng+ Imgs [ pos ]->xln+Gap;
              i++;
          }
          img = kgCreateImage ( lng+2 , Ht ) ;
          shft = 0;
          i=0;
          while ( Buf [ i ] != '\0' ) {
              pos = ( int ) Buf [ i ] ;
              if ( Imgs [ pos ] != NULL ) {
                  kgReplaceImage ( img , Imgs [ pos ]->img , shft , 0 ) ;
                  shft += Imgs [ pos ]->xln+Gap;
              }
              i++;
          }
          IMG = (IMG_STR *) malloc(sizeof(IMG_STR));
          IMG->img = img;
          IMG->xln = lng+2;
          IMG->yln =0;
          break;
          case 'f':
            i=0;
            while(str[i]!='\0') {
                if(str[i]< ' ') Buf[i]=' ';
                else if(str[i]== 127) Buf[i]=' ';
                else Buf[i]=str[i];
                i++;
            }
            Buf[i]='\0';
            fontname =F->name;
            IMG  = ( IMG_STR * ) kgMakeFixedFontImg ( fontname , Buf, F->Size,Gap ) ;
            img = IMG->img;
//            free(IMG);
          break;
          case 'i':
          n = n+1;
          xsize = n*(F->Size+Gap);
          ysize = Ht;
      fid = kgInitImage ( xsize+1 , ysize+1 , 4 ) ;
      kgUserFrame ( fid , 0. , 0. , ( float ) xsize , ( float ) ysize ) ;
      kgTextFont ( fid , F->fontno%34 ) ;
      th = 1.3*F->Size;
      tw = F->Size;
      kgTextSize ( fid , th , tw ,(float) Gap ) ;
      kgTextColor ( fid , 0 ) ;
      xx = 0;
      yy = 0.6*F->Size;
      i = 0;
      lng = kgStringLength(fid,str);
      while ( (str [ i ] >= ' ' )||(str[i]=='\t')) {
          kgMove2f ( fid , xx , yy ) ;
          stmp [ 0 ] = str [ i ] ;
	  if(str[i]=='!') {
		  strcpy(stmp,(char *)"!!");
	  }
	  else if(str[i]=='\t') {
//		  stmp[0]=' ';
	  }
	  else if(str[i]==127) stmp[0]=' ';
          kgWriteText ( fid , stmp ) ;
	  stmp[1]='\0';
          xx += (F->Size+Gap);
          i++;
      }
//          img = kgGetResizedImage(fid);
      img = kgGetSharpImage ( fid ) ;
      kgCloseImage ( fid ) ;

          IMG = (IMG_STR *) malloc(sizeof(IMG_STR));
          IMG->img = img;
          IMG->xln = lng;
          IMG->yln =0;
          break;
          deafault:
          break;
      }
      return IMG;
  }
  char *uiGetStringPart(char *Str,char *Buf,int *cval,int *fval,float *wfac,float *hfac,float *shft) {
      int i,k;
      char *pt=Str;
      float fac;
      int Ok=0;
      if(Str==NULL) {Buf[0]='\0';return NULL;}
      if(Str[0]=='\0') {Buf[0]='\0';return NULL;}
      while((Ok==0)&&(Str[0]=='!')) {
//              printf("Str: %s\n",Str);
//              fflush(stdout);
              switch(Str[1]) {
                case 'f':
                  *fval = (Str[2]-'0')*10+(Str[3]-'0');
                  break;
                case 'c':
                  *cval = (Str[2]-'0')*10+(Str[3]-'0');
                  break;
                case 'w':
                  fac = (float)(Str[2]-'0')/(Str[3]-'0');
                  *wfac = *wfac*fac;
                  break;
                case 'z':
                  fac = (float)(Str[2]-'0')/(Str[3]-'0');
                  *wfac = *wfac*fac;
                  *hfac = *hfac*fac;
                  break;
                case 'h':
                  fac = (float)(Str[2]-'0')/(Str[3]-'0');
                  *hfac = *hfac*fac;
                  break;
                case 'S':
                  Str +=2;
                  *shft +=-1.0;
                  *hfac = *hfac*0.7;
                  *wfac = *wfac*0.7;
                  continue;
                case 's':
                  *shft +=1.0;
                  Str +=2;
                  *hfac = *hfac*0.7;
                  *wfac = *wfac*0.7;
                  continue;
                case 'x':
                  Str +=2;
                  *shft +=-0.2;
                  continue;
                case 'y':
                  *shft +=0.2;
                  Str +=2;
                  continue;
                case 'u':
                  Str +=2;
                  *shft +=-0.9;
                  continue;
                case 'd':
                  *shft +=0.9;
                  Str +=2;
                  continue;
                case '!':
		  Ok =1;
                  continue;
                case '\0':
		  Ok =1;
                  continue;
                default:
                  Str +=2;
                  continue;
             }
             Str +=4;
      }
      i=0;
      k=0;
//      Str=pt;
      while(Str[i]!='\0') {
        if(Str[i]=='!') {
             if(Str[i+1]=='!'){
                 Buf[k]='!';
                 k++;i+=2;
             }
             else if(Str[i+1]=='\0'){
                 Buf[k]='!';
                 k++;i+=1;
             }
             else break;
         }
         else {
           Buf[k++]=Str[i++];
           Buf[k]='\0';
         }
      }
      Buf[k]='\0';
      return Str+i;
  }
  void *uiComplexString ( char *str ,void *Imgtmp, int font , int color , int FontSize ,int height ) \
  {
      char Buf[1000];
      char *Str;
      int x,y,width,bkcolor,justfic;
      IMG_STR **Cimgs= (IMG_STR **) Imgtmp;
      int ln , i , maxchar , tempi,tpos;
      int x1 , ln1,yln;
      char *cpt=str;
      void *img = NULL;
      float length,shft=0,pos;
      FONT_STR F;
      IMG_STR *IMG=NULL,*IMGP=NULL,IMGT;
      void *imgbk , *fid;
      int rd , gr , bl;
      int fval,cval,Ht,Hto;
      float wfac,hfac,fac;
      int Fz =FontSize;
      ln = width;
      if ( str == NULL ) return NULL;
      if ( str [ 0 ] == '\0' ) return NULL;
      if(FontSize<=0) FontSize = height/2;
      Hto = 2*FontSize;
      if(height <=0) return NULL;
      tpos = (int)(height*0.5+0.5-FontSize*0.9);
      tpos = (int)(height*0.5+0.5);
      wfac =1.0;
      hfac = 1.0;
      cval = color;
      fval = font;
//      uiCleanOldString(str,Buf,&cval,&fval,&wfac,&zfac);
      while(((cpt= uiGetStringPart(cpt,Buf,&cval,&fval,&wfac,&hfac,&shft)) != NULL)) {
//        printf("Got Buf : %s\n",Buf);
//       fflush(stdout);
        kgGetDefaultRGB ( cval , & rd , & gr , & bl ) ;
        F.code = 'f';
        F.name = kgGetOthFont ( fval ) ;
        F.Imgs = Cimgs;
        if ( FontSize <= 0 ) F.Size = ( height ) /2;
        else F.Size = FontSize;
        if(F.Size != Fz) F.code='f';
        if(fval != font) F.code='f';
        if((wfac != 1.0 )||(hfac != 1.0 )){
          float fac =wfac;
          if(hfac>fac ) fac = hfac;
          F.code = 'f';
//          F.Size = F.Size*fac;
        }
        if(F.Imgs==NULL) F.code='i';
        if(F.Size >(( height ) /2 ) ) F.Size = ( height ) /2 ;
        Ht = 2*F.Size;
        if(F.Size != FontSize ) F.code='f';
        F.code = 'f';
        if(F.code=='t') Ht=Hto;
        if(F.code != 'f')IMG = uiMakeString ( & ( F ) , Buf , ( int )(Ht) , 0 ) ;
        else IMG = uiFFString ( & ( F ) , Buf , ( int )(2.0*F.Size*1.2*hfac+0.5) , 0 ,hfac,wfac) ;
//        else IMG = uiFFString ( & ( F ) , Buf , ( int )(Ht*hfac) , 0 ,hfac,wfac) ;
#if 0
          if(((wfac != 1.0)||(hfac !=1.0))) {
//            hfac = 1.0;
//            if(hfac > 1.0 ) hfac = 1.0;
            IMG->xln = (IMG->xln-2)*wfac+2;
            yln = (int)(Ht*hfac);
            if(yln>=height) yln = height;
            img = kgChangeSizeImage(IMG->img,IMG->xln,yln);
            kgFreeImage(IMG->img);
            IMG->img = img;
          }
#endif
        if(IMG->img == NULL) {printf("IMG->img == NULL\n"); fflush(stdout);}
        kgSetImageColor ( IMG->img , rd , gr , bl ) ;
        pos = (int)(tpos -1.3*F.Size*hfac+0.5) +(int)(shft*F.Size*hfac);
        if(IMGP==NULL){
          IMGT.xln = IMG->xln;
          IMGT.yln = IMG->yln;
          IMGT.img = kgCreateImage ( IMGT.xln+1 , (int)height ) ;
          kgReplaceImage ( IMGT.img , IMG->img , 0,pos) ;
          if(IMG->img= NULL)kgFreeImage(IMG->img);
          IMGP=IMG;
          IMGP->img = IMGT.img;
        }
        else{
          IMGT.xln = IMGP->xln+IMG->xln;
          IMGT.yln = IMGP->yln;
          IMGT.img = kgCreateImage ( IMGT.xln+1 , (int)height ) ;
          kgReplaceImage ( IMGT.img , IMGP->img , 0,0 ) ;
          kgReplaceImage ( IMGT.img , IMG->img , IMGP->xln+1,pos);
          kgFreeImage(IMGP->img);
          kgFreeImage(IMG->img);
          free(IMG);
          IMGP->xln = IMGT.xln;
          IMGP->yln = IMGT.yln;
          IMGP->img = IMGT.img;
        }
        shft =0.0;
        wfac=1.0;
        hfac=1.0;
      }
      IMG = IMGP;
      return IMG;
  }
