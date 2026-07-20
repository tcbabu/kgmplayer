#include "kulina.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "gprivate.h"
#include <ft2build.h>
 void * uiInitGraphicFontLists ( int font,int FontSize );
#include FT_FREETYPE_H
static int Xshft=2;
#if 0
  typedef struct _img_str {
      int xln;
      int yln;
      void *img;
      int Size;
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
              if ( i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT ){
//                      printf("Limit Crossed: %d %d : %d %d\n",i,j,WIDTH,HEIGHT);
                      continue;
              }
              pixls [ j*WIDTH+i ] = bitmap->buffer [ q * bitmap->width + p ] ;
          }
      }
//      printf("Bitmap: %d %d : %d %d : %d %d\n",x,y, bitmap->width , bitmap->rows,WIDTH,HEIGHT);
      return pixls;
  }
  static void * Make_image ( unsigned char *pixls , int WIDTH,int HEIGHT) {
      int i , j;
      void *img , *img2;
      int clr;
      img2 = kgCreateImage ( WIDTH , HEIGHT ) ;
      int k = -1;
      for ( i = 0; i < HEIGHT; i++ ) {
          for ( j = 0; j < WIDTH; j++ ) {
              k++;
              if ( pixls [ k ] == 0 ) continue;
              kgSetPixelAlpha ( img2 , j , i , 255-pixls [ k ] ) ;
          }
      }
      img = img2;
//    kgWriteImage(img,"Junk.png");
      return img;
  }
  static  void  *draw_bitmap_img (  FT_Bitmap* bitmap , FT_Int x , FT_Int y ,int WIDTH,int HEIGHT)  \
      {
      FT_Int i , j , p , q;
      FT_Int x_max = x + bitmap->width;
      FT_Int y_max = y + bitmap->rows;
      int Ysft = y_max;
      int k = 0;
      void *img , *img2;
      int clr;
      img2 = kgCreateImage ( WIDTH*2 , HEIGHT*2 ) ;
  /* for simplicity, we assume that `bitmap->pixel_mode' */
  /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */
      for ( j = y , q = 0; j < y_max; j++, q++ ) {
          for ( i = x , p = 0; i < x_max; i++, p++ ) {
              kgSetPixelAlpha ( img2 , i+Xshft , j+HEIGHT*3/4 , 255-bitmap->buffer [ q * bitmap->width + p ]  ) ;
//              kgSetPixelAlpha ( img2 , i , j+HEIGHT*3/4 , 255-bitmap->buffer [ q * bitmap->width + p ]  ) ;
          }
      }
      return img2;
  }
  static void * draw_image (FT_Bitmap *bitmap,FT_GlyphSlot slot  , int WIDTH,int HEIGHT) {
      int i , j,ix,iy;
//      FT_Bitmap bitmap = slot->bitmap;
//              slot->bitmap_left ,HEIGHT - slot->bitmap_top,WIDTH,HEIGHT ) ;
      int left =  slot->bitmap_left;
      int top = HEIGHT -slot->bitmap_top;
      int width = slot->bitmap.width; 
      int rows  = slot->bitmap.rows;
      void *img , *img2;
      int clr;
      img2 = kgCreateImage ( WIDTH , HEIGHT ) ;
      printf("left: %d top %d Width %d rows %d : %d %d\n",left,top,width,rows,WIDTH,HEIGHT);
      fflush(stdout);
      int k = -1;
      for ( i = 0; i < rows; i++ ) {
          for ( j = 0; j < width; j++ ) {
              ix = (top+i)*WIDTH;
              iy = j;
              k++;
              if(ix < 0) continue;
 //             pixls [ j*WIDTH+i ] = bitmap->buffer [ q * bitmap->width + p ] ;
//              if ( pixls [ k ] == 0 ) continue;
              printf("I,j,k: %d %d %d\n",i,j,k);
              fflush(stdout);
              kgSetPixelAlpha ( img2 , iy , ix , 255-bitmap->buffer [ k ] ) ;
          }
      }
      img = img2;
//    kgWriteImage(img,"Junk.png");
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
      int Ht,Wd;
      double dx , dy;
      double hfac=1.2,wfac=1.0;
      IMG_STR *Img = NULL;
      num_chars = strlen ( text ) ;
//      angle = ( 25.0 / 360 ) * 3.14159 * 2; /* use 25 degrees */
      angle = ( 0.0 / 360 ) * 3.14159 * 2; /* use 25 degrees */
      Ht = Size*hfac;
      Wd =Size*wfac;
      HEIGHT =(int)( 2.0*Size);
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
      WIDTH = 2.0*WIDTH*num_chars+4;
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
//          printf("Slot: %d %d %d %d \n",slot->bitmap_left ,slot->bitmap_top,slot->bitmap.width,slot->bitmap.rows);
          draw_bitmap ( pixls , & slot->bitmap , slot->bitmap_left , target_height - slot->bitmap_top,WIDTH,HEIGHT ) ;
    /* increment pen position */
          pen.x += slot->advance.x+32+Gap*64;
          pen.y += slot->advance.y;
      }
      img = Make_image ( pixls ,WIDTH ,HEIGHT) ;
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
          Imgs [ i ]->Size =Size;
      }
      return ( void ** ) Imgs;
  }
  void **kgGrFontChars ( char *Font , int Size ) {
      IMG_STR **Imgs = NULL;
      int xsize,ysize;
      int left,right;
      Imgs = ( IMG_STR ** ) malloc ( sizeof ( IMG_STR * ) *128 ) ;
      void *img,*rzimg;;
      char txt [ 2 ] = "";
      int i , k;
      for ( i = 0;i < 128;i++ ) Imgs [ i ] = NULL;
      for ( i = 31;i < 128;i++ ) {
          txt [ 0 ] = i;
          Imgs [ i ] = ( IMG_STR * ) kgMakeGrFontImg ( Font , txt , Size ,Size,0) ;
          img = Imgs [ i ]->img ; 
#if 1  
          if(i> 31) {
//          printf("i = %d %c\n",i,i);
//          fflush(stdout);
          kgGetImageSize(Imgs [ i ]->img ,&xsize,&ysize);
          kgGetAlphaLeftRight(img,&left,&right);
//          printf("Left %d Right %d\n",left,right);
//          fflush(stdout);
          rzimg = kgCropImage(img,left,0,xsize-1-right,ysize);
          Imgs [ i ]->img = rzimg ;

          kgFreeGmImage(img);
          img = rzimg;
          }
#endif
          kgGetImageSize(Imgs [ i ]->img ,&xsize,&ysize);
          Imgs [ i ]->xln = xsize;
          Imgs [ i ]->yln = ysize ;

          Imgs [ i ]->Size =Size;
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
          Imgs [ i ]->Size =Size;
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
  int kgCheckComplexString(char * str) {
     int ret=0;
     int i=0;
     if(str != NULL) {
        while(str[i] != '\0') {
          if(str[i]=='!') {
            if ( str[i+1] == 'S') {
               ret =1; break;
            }
            if ( str[i+1] == 's') {
               ret =1; break;
            }
            if ( str[i+1] == 'u') {
               ret =1; break;
            }
            if ( str[i+1] == 'd') {
               ret =1; break;
            }
#if 0
            if ( str[i+1] == 'x') {
               ret =1; break;
            }
            if ( str[i+1] == 'y') {
               ret =1; break;
            }
#endif
          }
          i++;
        }
     }
     return ret;
  }
  void *uiComplexGrString ( char *str ,void *Imgtmp, int font , int color , int FontSize ,int height ) \
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
        pos = -1*(int)(shft*F.Size*hfac);
        if(IMGP==NULL){
          int xsize,ysize,xsize1,ysize1;;
          IMGT.xln = IMG->xln;
          IMGT.yln = IMG->yln;
          kgGetImageSize(IMG->img ,&xsize,&ysize);
          if ( (ysize) > height ) height = (ysize);
          IMGT.img = kgCreateImage ( IMGT.xln+1 , (int)height ) ;
//          kgReplaceImage ( IMGT.img , IMG->img , 0,pos-ysize) ;
          kgReplaceImage ( IMGT.img , IMG->img , 0,0) ;
          if(IMG->img= NULL)kgFreeImage(IMG->img);
          IMGP=IMG;
          IMGP->img = IMGT.img;
        }
        else{
          int xsize,ysize,xsize1,ysize1;;
          IMGT.xln = IMGP->xln+IMG->xln;
          IMGT.yln = IMGP->yln;
          kgGetImageSize(IMGP->img ,&xsize,&ysize);
          kgGetImageSize(IMG->img ,&xsize1,&ysize1);
          if ( (ysize1+pos) > height ) height = (ysize1+pos);
          IMGT.img = kgCreateImage ( IMGT.xln+1 , (int)height ) ;
          kgReplaceImage ( IMGT.img , IMGP->img , 0,0 ) ;
          kgReplaceImage ( IMGT.img , IMG->img , IMGP->xln+1,0);
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
  int ffStringLength(char *str,void *tmp) {
      IMG_STR **IMG = (IMG_STR **)tmp;
      int ln =0;
      int i=0;
      if(str== NULL) return 0;
      if(tmp == NULL) {
         fprintf(stderr,"IMG_STR not loaded\n");
         return 0;
      }
      while(str[i] !='\0'){
          ln = IMG[str[i]]->xln;
          i++;
      }
      return ln;
  }
  int ftGetWarray(int font,float *wd) {
      IMG_STR **IMG = (IMG_STR **)uiInitGraphicFontLists(font,64);
      int ln =0;
      int i=0;
      if(wd == NULL) return 0;
      
      if(IMG == NULL) {
         fprintf(stderr,"IMG_STR not loaded\n");
         return 0;
      }
      for(i=0;i<128;i++) wd[i]=0;
      for(i=31;i<128;i++) {
        if(IMG[i]!=NULL)wd[i]=(float)IMG[i]->xln/((float)(IMG[i]->Size)*1.2);      
//        printf("I = %d wd = %f %c\n",i,wd[i],i);
      }
      return 1;
  }
void *uiAddCharImage(void *img1,void *img2,int xshft,int sft,int  *ymax,int *ymin) {
    int xsize1,ysize1,xsize2, ysize2,xsize,ysize;
    int ht,offset,shift = sft,yu,yl,ymaxold,yminold;
    void *img=NULL;
    static float Crr=0;
    static int  Yold=0;
    if(img2 == NULL) return img1;
      kgGetImageSize(img2,&xsize2,&ysize2);
      
    if (img1 == NULL){
      *ymin = shift;
      Crr = 0.25*ysize2;
      Yold = ysize2;
      shift = sft - Crr;
      if((ysize2+shift) >*ymax ) *ymax = ysize2+shift;
      return img2;
    }
    if((sft  == 0 )&&(ysize2!=Yold) ) {
      Crr = ysize2*0.25;
      Yold  = ysize2;
    }
    
    shift = sft - Crr;
    ymaxold = *ymax;
    yminold = *ymin;
    kgGetImageSize(img1,&xsize1,&ysize1);
  //  kgGetImageSize(img2,&xsize2,&ysize2);
    
    yu = shift+ysize2;
    yl = shift;
    if(yu > *ymax ) *ymax =yu;
    if(yl < *ymin ) *ymin = yl;
    xsize = xsize1+xsize2;
    ht = ysize2+abs(shift);
    ysize = *ymax - *ymin;
    
    offset = ysize - shift;
    img = kgCreateImage ( xsize, ysize ) ;
    kgReplaceImage ( img , img1 , 0,*ymax  -ymaxold) ;
    kgReplaceImage ( img , img2 ,xsize1,*ymax -yu) ;
#if 0
    if(*ymax == ymaxold )kgReplaceImage ( img , img2 ,xsize1,*ymax -yu) ;
    else kgReplaceImage ( img , img2 ,xsize1,*ymin-ysize2) ;
    if(ysize==ysize1){
      if(shift >=0 ) {
        kgReplaceImage ( img , img2 ,xsize1,*ymax -(ysize2+shift)) ;
      }
      else {
        kgReplaceImage ( img , img2 ,xsize1,*ymax -shift) ;
      }
    }
    else {
       if(shift  > 0 )kgReplaceImage ( img , img2 ,xsize1,0) ;
       else kgReplaceImage (img,img2,xsize1,ysize-ysize2);
    }
#endif
    kgFreeGmImage(img1);
    kgFreeGmImage(img2);
    kgGetImageSize(img,&xsize,&ysize);
    return img;
}
#define YLPUSH {\
                  ypt = ( YPOS * ) malloc ( sizeof ( YPOS )) ;\
                  ypt->yp = yp;\
                  ypt->hfact = hfact;\
                  ypt->wfact = wfact;\
                  Dpush ( YL , ypt ) ;\
}
#define YLPOP {\
                  ypt = Dpop ( YL ) ;\
                  yp = ypt->yp;\
                  hfact = ypt->hfact;\
                  wfact = ypt->wfact;\
                  free ( ypt ) ;\
}
#define ADDGRIMG {\
              IMG = Imgs[txt[i]];\
              kgGetImageSize(IMG->img,&xsize,&ysize);\
              img = kgCopyImage(IMG->img);\
              rzimg = kgChangeSizeImage(img,(int)(((wd)*wfact*IMG->xln/(float)Fsize)*cfx+0.5) ,(int)( height*hfact*cfy+0.5));\
              kgSetImageColor ( rzimg , rd , gr , bl ) ;\
              kgGetImageSize(rzimg,&xsize,&ysize);\
              int gxsize = (int)((gp*wfact*IMG->xln/(float)Fsize)*cfx+0.5);\
              kgFreeGmImage(img);\
              img = rzimg;\
              if((fimg != NULL)&&(gxsize >1)) {\
                 void *gimg=NULL;\
                 gimg = kgCreateImage(xsize+gxsize,ysize);\
                 gimg = kgAddImages(gimg,img,gxsize,0);\
                 kgFreeGmImage(img);\
                 img = gimg;\
                 kgGetImageSize(img,&xsize,&ysize);\
              }\
              shift = (int)(yp*height*cfy+0.5);             \
              fimg = uiAddCharImage(fimg,img,xp,shift,&ymax,&ymin);\
              img=NULL;\
              xp += ( (wd+gp)*wfact ) ;\
}
static float Fval(char *str) {
    float val=1.0;
    val = str[0]-'0';
    if(str[1]!= (int)'0')val = val/(str[1]-'0');
    return val;
}
static int Ival(char *str) {
   int val;
   val = str[0]-'0';
   val = val*10+(str[1]-(int)'0');
   return val;    
}
  void * ftGrStringImage ( int font , int color ,int angle, char *txt , float wdth , \
  float height , float gp , float cfx ,float cfy) {
      float wd=wdth , xp=0 , yp=0,hfact=1.0,wfact=1.0;
      void *img = NULL , *fimg = NULL,*rzimg=NULL;
      int rd,gr,bl;
      int xsize,ysize;
      short ngp , n , i , j , k ;
      int font_o , Nu , De;
      float xdsp = 0;
      float m_f [ 128 ] ;
      float val;
      char cntl;
      IMG_STR **Imgs;
      int ymax=0,ymin=0;
      int top,left,bottom,right;
      typedef struct _ypos {
          float yp , hfact , wfact,xp ;
      } YPOS;
      int Fsize = 64;
      if ( txt == NULL ) return NULL;
      if ( txt [ 0 ] == '\0' ) return NULL;
      YPOS *ypt = NULL;
      Dlink *YL = Dopen ( ) ;
      Dlink *XL = Dopen ( ) ;
      char Str [ 2 ] ;
      float *xpt;
      float  strln;
      int  istrln;
      int shift;
      Imgs =(IMG_STR **) uiInitGraphicFontLists(font,Fsize);
      IMG_STR *IMG=NULL;
      ypt = ( YPOS * ) malloc ( sizeof ( YPOS ) ) ;
      xp = 0;
      yp = 0;
      ypt->yp = 0.;
      ypt->hfact = 1.0;
      ypt->wfact = 1.0;
      ypt->xp = xp;
      Dadd ( YL , ypt ) ;
      xpt = ( float * ) malloc ( sizeof ( float ) ) ;
      *xpt = 0;
      Dadd ( XL , xpt ) ;
      Str [ 0 ] = '\0';
      font_o = font;
      wd = wdth;
      ngp = 1;
      xdsp = 0;
      kgGetDefaultRGB ( color , & rd , & gr , & bl ) ;
      strln = ftStringLength ( font,txt , wdth+gp );
//      printf("Strln : %f :%f:%d\n",strln,wdth,font);
      istrln = strln*cfx+0.5;
      while ( txt [ i ] != '\0' ) {
          if ( txt [ i ] != '!' ) { 
              Str [ 0 ] = txt [ i ] ;
              Str [1]   = '\0';
              ADDGRIMG;
          }
          else {
              i++;
              if ( txt [ i ] == '\0' ) break;
              cntl = txt [ i ] ;
              switch ( cntl ) {
                  case 'S':
                  YLPUSH;
                  yp += ( hfact ) *0.6;
                  hfact = hfact*0.5;
                  wfact = wfact*0.5;
                  break;
                  case 's':
                  YLPUSH;
                  yp -= ( hfact ) *0.15;
                  hfact = hfact*0.5;
                  wfact = wfact*0.5;
                  break;
                  case 'e':
                  YLPOP;
                  break;
                  case '!':
                  ADDGRIMG;
                  break;
                  case 'b':
                  xpt = ( float* ) malloc ( sizeof ( float )) ;
                  *xpt = xp;
                  Dpush ( XL , xpt ) ;
                  break;
                  case 'B':
                  break;
                  case 'I':
                  break;
                  case 'N':
                  break;
                  case 'g':
                  break;
                  case 'r':
                  xpt = ( float * ) Dpop ( XL ) ;
                  xp = *xpt;
                  free ( xpt ) ;
                  break;
                  case 'k':
                  xpt = ( float * ) Dpop ( XL ) ;
                  xp = *xpt;
                  free ( xpt ) ;
                  break;
                  case 'x':
                  YLPUSH;
                  yp += ( hfact  *0.3);
                  break;
                  case 'y':
                  YLPUSH;
                  yp -= ( hfact  *0.3);
                  break;
                  case 'u':
                  val = Fval(txt+i+1);;
                  YLPUSH;
                  yp += ( hfact ) *val;
                  i=i+2;
                  break;
                  case 'd':
                  val = Fval(txt+i+1);
                  YLPUSH;
                  yp -= ( hfact  *val);
                  i=i+2;
                  break;
                  case 'O':
                  case 'U':
                  break;
                  case '%':
                  break;
                  case 'z':
                  if ( ( txt [ i+1 ] == '\0' ) || ( txt [ i+2 ] == '\0' ) ) break;
                  val = Fval(txt+i+1);
                  YLPUSH;
                  hfact = hfact*val;
                  wfact = wfact*val;
                  i += 2;
                  break;
                  case 'f':
                  if ( ( txt [ i+1 ] == '\0' ) || ( txt [ i+2 ] == '\0' ) ) break;
                  font  = Ival(txt+i+1);
                  Imgs =(IMG_STR **) uiInitGraphicFontLists(font,Fsize);
                  i+= 2;
                  break;
                  case 'c':
                  color = Ival(txt+i+1);
                  kgGetDefaultRGB ( color , & rd , & gr , & bl ) ;
                  i+= 2;
                  break;
                  case 'h':
                  if ( ( txt [ i+1 ] == '\0' ) || ( txt [ i+2 ] == '\0' ) ) break;
                  YLPUSH;
                  val = Fval(txt+i+1);
                  hfact = hfact*val;
                  i += 2;
                  break;
                  case 'w':
                  if ( ( txt [ i+1 ] == '\0' ) || ( txt [ i+2 ] == '\0' ) ) break;
                  YLPUSH;
                  val = Fval(txt+i+1);
                  wfact = wfact*val;
                  i += 2;
                  break;
                  default :
                  break;
              }
          }
              i++;
      }
      Dempty ( XL ) ;
      Dempty ( YL ) ;
      kgGetImageSize(fimg,&xsize,&ysize);
#if 1
//      printf("Strln: %d Xsize %d\n",strln,xsize);
      img = kgChangeSizeImage(fimg,istrln,ysize);
      kgFreeGmImage(fimg);
      if(angle != 0){
//          fimg = kgRotateAboutImage(img,-1.0*angle,0,0);
          fimg = kgRotateImage(img,-1.0*angle);
          kgFreeGmImage(img);
          img = fimg;
      }
#else
      img = fimg;
#endif
      IMG= (IMG_STR *)malloc(sizeof(IMG_STR));
      IMG->img = img;
      IMG->yln = ymin;
      IMG->xln = istrln;
      IMG->Size = ysize;
      return IMG;
  }
  float  ftStringLength ( int font , char *txt , float wdth ){
      float wd=wdth , xp=0 , yp=0,hfact=1.0,wfact=1.0;
      void *img = NULL , *fimg = NULL,*rzimg=NULL;
      int rd,gr,bl;
      int xsize,ysize;
      short ngp , n , i , j , k ;
      int font_o , Nu , De;
      float xdsp = 0;
      float m_f [ 128 ] ;
      float val;
      char cntl;
      IMG_STR **Imgs;
      int ymax=0,ymin=0;
      int top,left,bottom,right;
      float height=1,gp=0;
      typedef struct _ypos {
          float yp , hfact , wfact,xp ;
      } YPOS;
      int Fsize = 64;
      if ( txt == NULL ) return 0.0;
      if ( txt [ 0 ] == '\0' ) return 0.0;
      YPOS *ypt = NULL;
      Dlink *YL = Dopen ( ) ;
      Dlink *XL = Dopen ( ) ;
      char Str [ 2 ] ;
      float *xpt;
      int strln;
      int shift;
      Imgs =(IMG_STR **) uiInitGraphicFontLists(font,Fsize);
      IMG_STR *IMG=NULL;
      ypt = ( YPOS * ) malloc ( sizeof ( YPOS ) ) ;
      xp = 0;
      yp = 0;
      ypt->yp = 0.;
      ypt->hfact = 1.0;
      ypt->wfact = 1.0;
      ypt->xp = xp;
      Dadd ( YL , ypt ) ;
      xpt = ( float * ) malloc ( sizeof ( float ) ) ;
      *xpt = 0;
      Dadd ( XL , xpt ) ;
      Str [ 0 ] = '\0';
      font_o = font;
      wd = wdth;
      gp = 0.;
      ngp = 1;
      xdsp = 0;
      while ( txt [ i ] != '\0' ) {
          if ( txt [ i ] != '!' ) { 
              IMG = Imgs[txt[i]];
              xp += ( wd*wfact*IMG->xln/(float)Fsize ) ;
          }
          else {
              i++;
              if ( txt [ i ] == '\0' ) break;
              cntl = txt [ i ] ;
              switch ( cntl ) {
                  case 'S':
                  YLPUSH;
                  yp += ( hfact ) *0.6;
                  hfact = hfact*0.5;
                  wfact = wfact*0.5;
                  break;
                  case 's':
                  YLPUSH;
                  yp -= ( hfact ) *0.15;
                  hfact = hfact*0.5;
                  wfact = wfact*0.5;
                  break;
                  case 'e':
                  YLPOP;
                  break;
                  case '!':
                  IMG = Imgs[txt[i]];
                  xp += ( wd*wfact*IMG->xln/(float)Fsize ) ;
                  break;
                  case 'b':
                  xpt = ( float* ) malloc ( sizeof ( float )) ;
                  *xpt = xp;
                  Dpush ( XL , xpt ) ;
                  break;
                  case 'B':
                  break;
                  case 'I':
                  break;
                  case 'N':
                  break;
                  case 'g':
                  break;
                  case 'r':
                  xpt = ( float * ) Dpop ( XL ) ;
                  xp = *xpt;
                  free ( xpt ) ;
                  break;
                  case 'k':
                  xpt = ( float * ) Dpop ( XL ) ;
                  xp = *xpt;
                  free ( xpt ) ;
                  break;
                  case 'x':
                  YLPUSH;
                  yp += ( hfact  *0.3);
                  break;
                  case 'y':
                  YLPUSH;
                  yp -= ( hfact  *0.3);
                  break;
                  case 'u':
                  val = Fval(txt+i+1);;
                  YLPUSH;
                  yp += ( hfact ) *val;
                  i=i+2;
                  break;
                  case 'd':
                  val = Fval(txt+i+1);
                  YLPUSH;
                  yp -= ( hfact  *val);
                  i=i+2;
                  break;
                  case 'O':
                  case 'U':
                  break;
                  case '%':
                  break;
                  case 'z':
                  if ( ( txt [ i+1 ] == '\0' ) || ( txt [ i+2 ] == '\0' ) ) break;
                  val = Fval(txt+i+1);
                  YLPUSH;
                  hfact = hfact*val;
                  wfact = wfact*val;
                  i += 2;
                  break;
                  case 'f':
                  if ( ( txt [ i+1 ] == '\0' ) || ( txt [ i+2 ] == '\0' ) ) break;
                  font  = Ival(txt+i+1);
                  Imgs =(IMG_STR **) uiInitGraphicFontLists(font,Fsize);
                  i+= 2;
                  break;
                  case 'c':
                  i+= 2;
                  break;
                  case 'h':
                  if ( ( txt [ i+1 ] == '\0' ) || ( txt [ i+2 ] == '\0' ) ) break;
                  YLPUSH;
                  val = Fval(txt+i+1);
                  hfact = hfact*val;
                  i += 2;
                  break;
                  case 'w':
                  if ( ( txt [ i+1 ] == '\0' ) || ( txt [ i+2 ] == '\0' ) ) break;
                  YLPUSH;
                  val = Fval(txt+i+1);
                  wfact = wfact*val;
                  i += 2;
                  break;
                  default :
                  break;
              }
          }
              i++;
      }
      Dempty ( XL ) ;
      Dempty ( YL ) ;
      return xp;
  }
  void * kgMakeGrFontImg( char *filename , char *text , int Htt,int Wdd ,int Gap) {
/* For Single Character */
      FT_Library library;
      FT_Face face;
      FT_GlyphSlot slot;
      FT_Matrix matrix; /* transformation matrix */
      FT_Vector pen; /* untransformed origin */
      FT_Error error;
      char Str[2]="u";
      int left,right;
      int Size = Htt;
      void *rzimg=NULL;
      int xsize,ysize;
      double angle;
      double hfac=1.0,wfac=(float)Wdd/(float)Htt;
      int target_height;
      int n , num_chars , i , j;
      int Ht,Wd;
      int WIDTH,HEIGHT;
      double dx , dy;
      num_chars = strlen ( text ) ;
      angle = ( 0.0 / 360 ) * 3.14159 * 2; 
      Ht = Htt;
      Wd = Wdd;
      IMG_STR *Img;
      void *img;
      unsigned char *pixls=NULL;
      error = FT_Init_FreeType ( & library ) ; /* initialize library */
      error = FT_New_Face ( library , filename , 0 , & face ) ;
      error = FT_Set_Char_Size ( face ,Wd*64, Ht*64, 120 , 120 ) ;
           /* set character size */
      slot = face->glyph;
  /* set up matrix */
      matrix.xx = ( FT_Fixed ) ( 1.0*0x10000L ) ;
      matrix.xy = 0;
      matrix.yx = 0;
      matrix.yy = ( FT_Fixed ) ( 1.0*0x10000L ) ;
#if 0
      HEIGHT =(int)( 2.0*Ht);
      target_height =(int)( HEIGHT );
      WIDTH = 2*Wdd*num_chars+4;
#else
      HEIGHT =(int)( Ht);
      target_height =(int)( HEIGHT );
      WIDTH = Wd*num_chars;
#endif
  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
      pen.x = (0) * 64;
 //     pen.y = (int)( Wdd/2.0+0.5 ) * 64;
      pen.y = (int)( Ht*0.25 ) * 64;
      for ( n = 0; n < num_chars; n++ ) {
          FT_Set_Transform ( face , & matrix , & pen ) ;
          error = FT_Load_Char ( face , text [ n ] , FT_LOAD_RENDER ) ;
          if ( error ) continue; /* ignore errors */

          img =draw_bitmap_img (  & slot->bitmap ,\
              slot->bitmap_left ,HEIGHT - slot->bitmap_top,WIDTH,HEIGHT ) ;
          pen.x += slot->advance.x+32;
          pen.y += slot->advance.y;
      }
//      img = Make_image ( pixls , WIDTH ,HEIGHT) ;
      FT_Done_Face ( face ) ;
      FT_Done_FreeType ( library ) ;
      pixls=NULL;
      Img = ( IMG_STR * ) malloc ( sizeof ( IMG_STR ) ) ;
      kgGetImageSize(img,&xsize,&ysize);
      right =(int) slot->bitmap.width+Xshft;
#if 0
      kgGetAlphaLeftRight(img,&left,&right);
      right = xsize - right+Gap+1;
      if(right > xsize ) right = xsize;
//      printf("%c %d Wd:%d\n",text[0],right,Wd);
      if(text[0]==' ' ) {
//           printf("WD: %d right %d\n",Wd,right);
           right = Wd/2;
      }
      rzimg = kgCropImage(img,0,0,right,ysize);
      kgFreeGmImage(img);
      img = rzimg;
#endif
#if 0
      rzimg = kgResizeImage(img,0.5);
      kgFreeGmImage(img);
      img = rzimg;
#endif
      kgGetImageSize(img,&xsize,&ysize);
      Img->img = img;
      Img->xln = xsize;
      Img->yln = ysize;
      Img->img = img;
      Img->Size = Size;
#if 0
      kgGetImageSize(img,&xsize,&ysize);
      printf("Xsize1: %d  %d\n",xsize,ysize);
      rzimg = kgCropImage(img,0,0,Img->xln,ysize);
      kgFreeGmImage(img);
      Img->img = rzimg;
#endif
      return Img;
  }
int uiUserImageBox(IMG_STR *IMG,float t_angle,float x1,float y1,\
    float cfx,float cfy,float *XX1,float *YY1,float *XX2,float *YY2){
/*   Gets the Image box with x1,y1 as point
     of rotation, specified in user space.
     Useful only for IMGSTR returned by ftGstring...
     as (0,yl) is mapped to x1,y1
**************************************************/
        int xsize,ysize,xsizeo,ysizeo;
        float X1,Y1,X2,Y2,yl,yu;

        double angle1,ct,st;
        double rad = 0.017453293;
        st = sin(t_angle*rad);
        ct = cos(t_angle*rad);
        yl = -(IMG->yln/cfy); 
        xsizeo = IMG->xln;
        ysizeo = IMG->Size;
        yu = ysizeo/cfy - yl; 
        kgGetImageSize(IMG->img,&xsize,&ysize);
        if( (st >= 0.) && (ct>=0.)){
           angle1 = t_angle;
           X1 = x1-yu*sin(angle1*rad);;
           Y1 = y1-yl*cos(angle1*rad);;
           X2 = (X1+xsize/cfx);
           Y2 = (Y1+ysize/cfy);
        }
        if( (st >= 0.) && (ct< 0.)){
           angle1 = t_angle -90.;
           Y1 = y1-yu*sin(angle1*rad);
           X1 = x1 + yl*cos(angle1*rad);
           X2 = (X1-xsize/cfx);
           Y2 = (Y1+ysize/cfy);
        }
        if( (st < 0.) && (ct<= 0.)){
            angle1 = t_angle -180.;
            Y1 = y1+yl*cos(angle1*rad);;
            X1 = x1+yu*sin(angle1*rad);;
            X2 = (X1-xsize/cfx);
            Y2 = (Y1-ysize/cfy);
        }
        if( (st < 0.) && (ct>0.)){
            angle1 =90.- ( t_angle -270.);
            X1 = x1 - yl*sin(angle1*rad);
            Y1 = y1 + yu*cos(angle1*rad);
            X2 = (X1+xsize/cfx);
            Y2 = (Y1-ysize/cfy);
        }
        *XX1 = X1;
        *YY1 = Y1;
        *XX2 = X2;
        *YY2 = Y2;
        return 1;
}
