#include "kulina.h"
#include "gprivate.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <magick/api.h>
#include <math.h>
void DestroyImagePixels( Image *image );
static int entry = 0;
static ImageInfo *image_info = NULL;
static ExceptionInfo exception;


  static void gmDummyImage ( void ) {
      int xl = 6 , yl = 6 , fac = 0;
      int r = 10 , g = 20 , b = 30;
      void *Img , *fid , *Img1;
      float h , s , v , rf , gf , bf , vm;
      fid = kgInitImage ( xl , yl , 2 ) ;
      kgUserFrame ( fid , -2. , -2. , ( float ) xl+2 , ( float ) yl+2 ) ;
      kgChangeColor ( fid , 551 , ( int ) r , ( int ) g , ( int ) b ) ;
      kgRoundedRectangleFill ( fid , ( float ) xl*0.5 , ( float ) yl*0.5 ,  \
          ( float ) xl+2 , ( float ) yl+2 , 0 , 551 , fac ) ;
      Img = kgGetSharpImage ( fid ) ;
      kgCloseImage ( fid ) ;
      kgFreeImage ( Img ) ;
      return ;
  }
  void uiInitGm ( void ) {
      if ( entry == 0 ) {
//    fprintf(stderr,"Init GM\n");
          entry = 1;
          InitializeMagick ( NULL ) ;
          GetExceptionInfo ( & exception ) ;
          image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      }
      return;
  }
  void uiCloseGm(void) {
      if ( entry == 0 ) { return;}
          DestroyImageInfo ( image_info ) ;
//          free(image_info);
          DestroyExceptionInfo ( &exception ) ;
          DestroyMagick();
          entry =0;
          return;
  }
  void kgInitGm ( void ) {
      uiInitGm;
      gmDummyImage ( ) ;
      return;
  }
  void ProcessString ( char *s ) {
      int i = 0;
      while ( s [ i ] >= ' ' ) {if ( s [ i ] == '/' ) s [ i ] = ':';i++;}
  }
  void *uiGetgmImage ( char *tmp ) {
//void *uiGetgmImage(char *flname) {
      FILE *fp;
      Image *image;
      int i = 0;
      char flname [ 1000 ] ;
      char size [ MaxTextExtent ] ;
      PixelPacket *pixels;
      GMIMG *png = NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      strncpy ( flname , tmp , 999 ) ;
      i = strlen ( flname ) -1;
      while ( ( i >= 0 ) && ( flname [ i ] <= ' ' ) ) flname [ i-- ] = '\0';
      if ( i < 0 ) return NULL;
//  else printf("uiGetgmImage: %s\n",flname);
      if ( ( fp = fopen ( flname , "r" ) ) == NULL ) {
          printf ( "Failed to open: %s\n" , flname ) ;
          return NULL;
      }
      else fclose ( fp ) ;
      uiInitGm ( ) ;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      GetExceptionInfo ( Exception ) ;
      FormatString ( size , "%dx%d" , 0 , 0 ) ;
       ( void ) CloneString ( & Image_info->size , size ) ;
       ( void ) strcpy ( Image_info->filename , flname ) ;
      image = ReadImage ( Image_info , Exception ) ;
      if ( image == NULL ) {
          DestroyImageInfo ( Image_info ) ;
          free ( Exception ) ;
          return NULL;
      }
#if 1
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
#endif
      image->background_color.red = 0;
      image->background_color.blue = 0;
      image->background_color.green = 0;
      image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      strcpy ( png->Sign , "IMG" ) ;
      strncpy ( png->flname , flname , 499 ) ;
      png->image_width = image->columns;
      png->image_height = image->rows;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = -1;
      png->rzfac = 1.0;
      GetImageInfo ( Image_info ) ;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
//  png->image_channels=4;
      if ( image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgGetInlineImage ( char *blob , int length ) {
      FILE *fp;
      Image *image;
      char size [ MaxTextExtent ] ;
      PixelPacket *pixels;
      GMIMG *png = NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      uiInitGm ( ) ;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      GetExceptionInfo ( Exception ) ;
      FormatString ( size , "%dx%d" , 0 , 0 ) ;
       ( void ) CloneString ( & Image_info->size , size ) ;
       ( void ) strcpy ( Image_info->filename , "inline" ) ;
      QueryColorDatabase ( "#ffffffff" , & Image_info->background_color , Exception ) ;
          
      image = BlobToImage ( Image_info , blob , length , Exception ) ;
      if ( image == NULL ) {
          printf ( "kgGetInlineImage : NULL: %s \n" , Image_info->filename ) ;
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      image->background_color.red = 0;
      image->background_color.blue = 0;
      image->background_color.green = 0;
      image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      strcpy ( png->Sign , "IMG" ) ;
      strcpy ( png->flname , "inline" ) ;
      png->image_width = image->columns;
      png->image_height = image->rows;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = -1;
      png->rzfac = 1.0;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
//  png->image_channels=4;
      if ( image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *uiGetImage ( char *flname ) { return uiGetgmImage ( flname ) ;}
  void *kgGetImage ( char *flname ) { return uiGetgmImage ( flname ) ;}
  void *uiCreateCleangmImage_o ( unsigned long xsize , unsigned long ysize , int bkred , int bkgreen , int bkblue , int opacity )  \
      {
/* 
   creates new gmImage from data;
   if data==NULL creates a new clean gmImage ofgive size
   if data!= NULL size must match with data
*/
      int i , j , k = 0;
      unsigned long v;
      Image *image , *tmpimg;
      PixelPacket *pixels;
      char size [ MaxTextExtent ] ;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      GMIMG *png = NULL;
      uiInitGm ( ) ;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      GetExceptionInfo ( Exception ) ;
#if 0
#else
      tmpimg = AllocateImage ( ( ImageInfo * ) NULL ) ;
      tmpimg->columns = xsize;
      tmpimg->rows = ysize;
      if ( tmpimg == NULL ) {
          fprintf ( stderr , "uiCreateCleangmImage:Failed to Allocate Image\n" ) ;
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      SetImage ( tmpimg , TrueColorMatteType ) ;
      image = CloneImage ( tmpimg , xsize , ysize , 1 , Exception ) ;
      if ( Exception->severity != UndefinedException ) {
          fprintf ( stderr , "uiCreateCleangmImage:Failed To create Image\n" ) ;
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      DestroyImage ( tmpimg ) ;
#endif
//TCB
      image->background_color.red = 0;
      image->background_color.blue = 0;
      image->background_color.green = 0;
      image->background_color.opacity = 0xff;
      image->matte = 1;
      pixels = SetImagePixels ( image , 0 , 0 , xsize , ysize ) ;
      if ( pixels == NULL ) {
          fprintf ( stderr , "Failed to set pixels\n" ) ;
          DestroyImage ( image ) ;
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      k = 0;
      for ( j = 0;j < ysize;j++ ) {
          for ( i = 0;i < xsize;i++ ) {
              pixels [ k ] .blue = bkred;
              pixels [ k ] .green = bkgreen;
              pixels [ k ] .red = bkblue;
              pixels [ k ] .opacity = opacity;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->image_width = image->columns;
      png->image_height = image->rows;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = -1;
      png->rzfac = 1.0;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
      if ( image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      pixels = GetImagePixels ( image , 0 , 0 , xsize , ysize ) ;
      return png;
  }
  void *uiCreateCleangmImage ( unsigned long xsize , unsigned long ysize , int bkred , int bkgreen , int bkblue , int opacity )  \
      {
/* 
   creates new gmImage from data;
   if data==NULL creates a new clean gmImage ofgive size
   if data!= NULL size must match with data
*/
      int i , j , k = 0;
      unsigned long v;
      Image *image , *tmpimg;
      PixelPacket *pixels;
      char size [ MaxTextExtent ] ;
      GMIMG *png = NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      uiInitGm ( ) ;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
//TCB 07/21
      GetExceptionInfo ( Exception ) ;
#if 1
      FormatString ( size , "%ldx%ld" , xsize , ysize ) ;
       ( void ) CloneString ( & Image_info->size , size ) ;
      tmpimg = AllocateImage ( Image_info ) ;
      if ( tmpimg == NULL ) {
          GetImageInfo ( Image_info ) ;
          fprintf ( stderr , "uiCreateCleangmImage:Failed to Allocate Image\n" ) ;
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      SetImage ( tmpimg , TrueColorMatteType ) ;
      image = tmpimg;
#else
#endif
//TCB
      image->background_color.red = 0;
      image->background_color.blue = 0;
      image->background_color.green = 0;
      image->background_color.opacity = 0xff;
      image->matte = 1;
      pixels = SetImagePixels ( image , 0 , 0 , xsize , ysize ) ;
      if ( pixels == NULL ) {
          fprintf ( stderr , "Failed to set pixels\n" ) ;
          fflush ( stderr ) ;
          DestroyImage ( image ) ;
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return uiCreateCleangmImage_o ( xsize , ysize , bkred , bkgreen , bkblue , opacity ) ;
              
      }
      if ( ( image->columns != xsize ) || ( image->rows != ysize ) ) {
          fprintf ( stderr , "Size of Pixels :%ld:%ld %ld:%ld\n" , image->columns , xsize , image->rows , ysize ) ;
              
      }
      k = 0;
      for ( j = 0;j < ysize;j++ ) {
          for ( i = 0;i < xsize;i++ ) {
              pixels [ k ] .blue = bkred;
              pixels [ k ] .green = bkgreen;
              pixels [ k ] .red = bkblue;
              pixels [ k ] .opacity = opacity;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = image;
#if 1
          DestroyImageInfo ( Image_info ) ;
#endif
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->image_width = image->columns;
      png->image_height = image->rows;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = -1;
      png->rzfac = 1.0;
      png->info = Image_info;
      png->exce = Exception;
      if ( image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
//      pixels = GetImagePixels ( image , 0 , 0 , xsize , ysize ) ;
      return png;
  }
  void *kgCreateImage ( int xsize , int ysize ) {
      return uiCreateCleangmImage ( xsize , ysize , 0 , 0 , 0 , 255 ) ;
  }
  void *uiCreategmImage ( unsigned long *data , unsigned long xsize , unsigned long ysize )  \
      {
/* 
   creates new gmImage from data;
   if data==NULL creates a new clean gmImage ofgive size
   if data!= NULL size must match with data
*/
      int i , j , k = 0;
      unsigned long v;
      Image *image , *tmpimg;
      PixelPacket *pixels;
      char size [ MaxTextExtent ] ;
      GMIMG *png = NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      uiInitGm ( ) ;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      GetExceptionInfo ( Exception ) ;
//  tmpimg=AllocateImage((ImageInfo *) NULL);
      FormatString ( size , "%ldx%ld" , xsize , ysize ) ;
       ( void ) CloneString ( & Image_info->size , size ) ;
      tmpimg = AllocateImage ( Image_info ) ;
      tmpimg->columns = xsize;
      tmpimg->rows = ysize;
      SetImage ( tmpimg , TrueColorMatteType ) ;
      if ( tmpimg == NULL ) {
          GetImageInfo ( Image_info ) ;
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
#if 0
#else
      image = tmpimg;
#endif
//TCB
      image->background_color.red = 0;
      image->background_color.blue = 0;
      image->background_color.green = 0;
      image->background_color.opacity = 0xff;
      image->matte = 1;
      pixels = SetImagePixels ( image , 0 , 0 , xsize , ysize ) ;
      SyncImagePixels ( image ) ;
      pixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      xsize = image->columns;
      ysize = image->rows;
      if ( data != NULL ) {
          k = 0;
          for ( j = 0;j < ysize;j++ ) {
              for ( i = 0;i < xsize;i++ ) {
                  v = data [ k ] ;
                  pixels [ k ] .blue = v & 0x000000ff;
                  v = v >> 8;
                  pixels [ k ] .green = v & 0x000000ff;
                  v = v >> 8;
                  pixels [ k ] .red = v & 0x000000ff;
                  v = v >> 8;
                  pixels [ k ] .opacity = 255 -v;
                  k++;
              }
          }
      }
      SyncImagePixels ( image ) ;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->image_width = image->columns;
      png->image_height = image->rows;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = -1;
      png->rzfac = 1.0;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
      if ( image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      if ( data == NULL ) uiCleangmImage ( png ) ;
      return png;
  }
  void *uiCreategmImage_1 ( unsigned long *data , unsigned long xsize , unsigned long ysize )  \
      {
      int i , j , k = 0;
      unsigned long v;
      Image *image , *tmpimg;
      PixelPacket *pixels;
      GMIMG *png = NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
       ( void ) strcpy ( Image_info->filename , "JUNK.png" ) ;
      tmpimg = ReadImage ( Image_info , Exception ) ;
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      image = CloneImage ( tmpimg , xsize , ysize , 1 , Exception ) ;
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      DestroyImage ( tmpimg ) ;
      SetImage ( image , TrueColorMatteType ) ;
      image->background_color.red = 0;
      image->background_color.blue = 0;
      image->background_color.green = 0;
      image->background_color.opacity = 255;
      image->matte = 1;
      pixels = SetImagePixels ( image , 0 , 0 , xsize , ysize ) ;
      SyncImagePixels ( image ) ;
      pixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      xsize = image->columns;
      ysize = image->rows;
      k = 0;
      for ( j = 0;j < ysize;j++ ) {
          for ( i = 0;i < xsize;i++ ) {
              v = data [ k ] ;
              pixels [ k ] .blue = v & 0x000000ff;
              v = v >> 8;
              pixels [ k ] .green = v & 0x000000ff;
              v = v >> 8;
              pixels [ k ] .red = v & 0x000000ff;
              v = v >> 8;
              pixels [ k ] .opacity = 255 -v;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->image_width = image->columns;
      png->image_height = image->rows;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = -1;
      png->rzfac = 1.0;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
//  png->image_channels=4;
      if ( image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *uiCopyPixelsgmImage ( void * img , unsigned long *data ) {
      int i , j , k = 0;
      unsigned long v , xsize , ysize;
      Image *image , *tmpimg;
      PixelPacket *pixels;
      GMIMG *png = NULL;
      png = ( GMIMG * ) img;
      uiInitGm ( ) ;
      image = png->image;
      pixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      k = 0;
      xsize = image->columns;
      ysize = image->rows;
      for ( j = 0;j < ysize;j++ ) {
          for ( i = 0;i < xsize;i++ ) {
              v = 255 -pixels [ k ] .opacity;
              v = v << 8;
              v |= pixels [ k ] .red ;
              v = v << 8;
              v |= pixels [ k ] .green;
              v = v << 8;
              v |= pixels [ k ] .blue;
              data [ k ] = v;
              k++;
          }
      }
      return png;
  }
  void *uiResizeImage ( PNGIMG *png , float fac ) {
      int w , h;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      if ( png == NULL ) return NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      w = image->columns*fac;
      h = image->rows*fac;
//  resize_image=ResizeImage(image,w,h,LanczosFilter,1.0,Exception);
//  resize_image=ResizeImage(image,w,h,BlackmanFilter,1.0,Exception);
//  resize_image=ResizeImage(image,w,h,BesselFilter,1.0,Exception);
//  resize_image=ResizeImage(image,w,h,QuadraticFilter,1.0,Exception);
      resize_image = ResizeImage ( image , w , h , TriangleFilter , 0.1 , Exception ) ;
          
      if ( Exception->severity != UndefinedException ) {
          CatchException ( Exception ) ;
          GetExceptionInfo ( Exception ) ;
      }
      pixels = GetImagePixels ( resize_image , 0 , 0 , resize_image->columns , resize_image->rows ) ;
          
      if ( pixels == NULL ) {
          png = NULL;
          DestroyImage ( resize_image ) ;
      }
      else {
          int i = 0 , j = 0;
          unsigned long *pt;
          resize_image->background_color.red = 0;
          resize_image->background_color.blue = 0;
          resize_image->background_color.green = 0;
          resize_image->background_color.opacity = 255;
          png = ( PNGIMG * ) Malloc ( sizeof ( PNGIMG ) ) ;
          png->image = resize_image;
          strcpy ( png->Sign , "PNG" ) ;
          png->incode = 0;
          png->image_width = resize_image->columns;
          png->image_height = resize_image->rows;
          png->image_channels = 4;
          if ( resize_image->matte == 0 ) png->image_channels = 3;
          else png->image_channels = 4;
          png->image_rowbytes = png->image_width*png->image_channels;
          png->image_data = ( unsigned char * ) Malloc ( png->image_width*  \
              ( png->image_height+1 ) *png->image_channels ) ;
          for ( i = 0;i < ( resize_image->columns*resize_image->rows ) ;i++ ) {
              png->image_data [ j++ ] = pixels [ i ] .red;
              png->image_data [ j++ ] = pixels [ i ] .green;
              png->image_data [ j++ ] = pixels [ i ] .blue;
              if ( png->image_channels == 4 ) png->image_data [ j++ ] = 255 - pixels [ i ] .opacity;
                  
//     png->image_data[j++]=  pixels[i].opacity;
          }
      }
      return png;
  }
  void *uiPixelsgmImage ( GMIMG *png ) {
      int w , h;
      Image *image;
      PixelPacket *pixels;
      if ( png == NULL ) return NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      w = image->columns;
      h = image->rows;
      pixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      if ( pixels == NULL ) {
          return NULL;
      }
      return pixels;
  }
  void *uiResizegmImage ( GMIMG *png , float fac ) {
      int w , h , xoffset , yoffset , bkgrclr;
      float rzfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      if ( png == NULL ) return NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception = NULL;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
 // Image_info=CloneImageInfo((ImageInfo *) NULL);
      w = image->columns*fac;
      h = image->rows*fac;
      xoffset = png->xoffset*fac;
      yoffset = png->yoffset*fac;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
//  resize_image=ResizeImage(image,w,h,LanczosFilter,1.0,Exception);
//  resize_image=ResizeImage(image,w,h,BlackmanFilter,1.2,Exception);
      if ( fac > 1.0 ) {
//    resize_image=ResizeImage(image,w,h,TriangleFilter,0.1,Exception);
          resize_image = ResizeImage ( image , w , h , BoxFilter , 1.2 , Exception ) ;
      }
      else {
          resize_image = ResizeImage ( image , w , h , LanczosFilter , 1.0 , Exception ) ;
              
      }
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = xoffset;
      png->yoffset = yoffset;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgResizeImage ( void * img , float fac ) {
      /* returns a resize Image, original not modified */
      return uiResizegmImage ( ( GMIMG * ) img , fac ) ;
  }
  void *uiThumbnailgmImage ( GMIMG *png , unsigned long w , unsigned long h ) {
      int xoffset , yoffset , bkgrclr;
      float rzfac , fac = 1 , wfac , hfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      if ( png == NULL ) {printf ( "uiThumbnailgmImage:NULL Image\n" ) ;
      return NULL;}
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetExceptionInfo ( Exception ) ;
      xoffset = png->xoffset*fac;
      yoffset = png->yoffset*fac;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
//  printf("Calling ResizeImage\n");
      wfac = ( float ) w/png->image_width;
      hfac = ( float ) h/png->image_height;
      fac = wfac;
      if ( hfac < fac ) fac = hfac;
      if ( ( w > png->image_width ) && ( h > png->image_height ) ) {
          resize_image = ThumbnailImage ( image , png->image_width , png->image_height , Exception ) ;
              
//       printf("Copying Image:%d:%d  %d:%d\n",w,h,png->image_width,png->image_height);
//       return kgCopyImage(png);
      }
      else{
          resize_image = ThumbnailImage ( image , ( int ) ( png->image_width*fac ) ,  \
              ( int ) ( png->image_height*fac ) , Exception ) ;
//    resize_image=SampleImage(image,(int)(png->image_width*fac),(int)(png->image_height*fac),Exception);
//      resize_image=ResizeImage(image,(int)(png->image_width*fac),(int)(png->image_height*fac),TriangleFilter,1.1,Exception);
      }
//  else resize_image=ThumbnailImage(image,w,h,Exception);
      GetImageInfo ( Image_info ) ;
      GetExceptionInfo ( Exception ) ;
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
//  printf("Resized Image\n");
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = xoffset;
      png->yoffset = yoffset;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgThumbNailImage ( void *img , unsigned long w , unsigned long h ) {
      void *IMG , *img1;
      IMG = kgGetImageCopy ( NULL , img ) ;
      img1 = uiThumbnailgmImage ( ( GMIMG * ) IMG , w , h ) ;
      kgFreeImage ( IMG ) ;
      return img1;
  }
  void *uiChangeSizegmImage ( GMIMG *png , unsigned long w , unsigned long h , int Fltr )  \
      {
      int xoffset , yoffset , bkgrclr;
      float rzfac , fac = 1 , wfac , hfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      if ( png == NULL ) {printf ( "NULL Image\n" ) ;return NULL;}
      ImageInfo * Image_info = NULL;
      ExceptionInfo *Exception;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      GetExceptionInfo ( Exception ) ;
      xoffset = png->xoffset*fac;
      yoffset = png->yoffset*fac;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
      wfac = ( float ) w/image->columns;
      hfac = ( float ) h/image->rows;
      fac = wfac;
      if ( hfac > fac ) fac = hfac;
      if ( image == NULL ) {printf ( "image==NULL:File %s:Line %d\n" , __FILE__ , __LINE__ ) ;
          
      return NULL;}
      if ( fac < 1.0 ) {
          switch ( Fltr ) {
              case 1:
              resize_image = ResizeImage ( image , w , h , BlackmanFilter , 1.2 , Exception ) ;
                  
              break;
              case 2:
              resize_image = ResizeImage ( image , w , h , HermiteFilter , 1.0 , Exception ) ;
                  
              break;
              case 3:
              resize_image = ResizeImage ( image , w , h , TriangleFilter , 1.0 , Exception ) ;
                  
              break;
              case 4:
              resize_image = ResizeImage ( image , w , h , BoxFilter , 1.2 , Exception ) ;
              break;
              case 5:
              resize_image = ResizeImage ( image , w , h , LanczosFilter , 1.0 , Exception ) ;
                  
              break;
              case 6:
              resize_image = ResizeImage ( image , w , h , QuadraticFilter , 1.2 , Exception ) ;
                  
              break;
              case 7:
              resize_image = ResizeImage ( image , w , h , BesselFilter , 1.2 , Exception ) ;
                  
              break;
              case 8:
              resize_image = ResizeImage ( image , w , h , CubicFilter , 1.0 , Exception ) ;
              break;
              default:
              resize_image = ResizeImage ( image , w , h , LanczosFilter , 1.0 , Exception ) ;
                  
              break;
          }
      }
      else {
#if 0
          if ( fac < 2.0 ) {
              resize_image = ResizeImage ( image , w , h , TriangleFilter , 1.0 , Exception ) ;
                  
          }
          else {
//       resize_image=SampleImage(image,w,h,Exception);
              resize_image = ResizeImage ( image , w , h , LanczosFilter , 1.0 , Exception ) ;
                  
          }
#else
//      resize_image=ResizeImage(image,w,h,TriangleFilter,1.0,Exception);
          resize_image = ResizeImage ( image , w , h , LanczosFilter , 1.0 , Exception ) ;
              
#endif
      }
//  printf("Calling ResizeImage\n");
//  resize_image=ResizeImage(image,w,h,HermiteFilter,1.0,Exception);
//  resize_image=ResizeImage(image,w,h,BlackmanFilter,1.2,Exception);
//  resize_image=ResizeImage(image,w,h,TriangleFilter,1.0,Exception);
//  resize_image=ResizeImage(image,w,h,BoxFilter,1.2,Exception);
//  resize_image=ResizeImage(image,w,h,LanczosFilter,1.0,Exception);
//  resize_image=ResizeImage(image,w,h,QuadraticFilter,1.2,Exception);
//  resize_image=ResizeImage(image,w,h,BesselFilter,1.2,Exception);
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
//  printf("Resized Image\n");
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = xoffset;
      png->yoffset = yoffset;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgChangeSizeImage ( void *img , long w , long h ) {
      /* returns a resize Image, original not modified */
      return uiChangeSizegmImage ( ( GMIMG * ) img , w , h , 1 ) ;
  }
  void *kgFilterImage ( void *img , long w , long h , int Fltr ) {
      /* returns a resize Image, original not modified */
      return uiChangeSizegmImage ( ( GMIMG * ) img , w , h , Fltr ) ;
  }
  void *uiHalfSizegmImage ( GMIMG *png ) {
      int xoffset , yoffset , bkgrclr;
      float rzfac , fac = 1;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      if ( png == NULL ) {printf ( "NULL Image\n" ) ;return NULL;}
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      xoffset = png->xoffset*fac;
      yoffset = png->yoffset*fac;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
//  fprintf(stderr,"Calling MinifyImage\n");
      resize_image = MinifyImage ( image , Exception ) ;
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = xoffset;
      png->yoffset = yoffset;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgHalfSizeImage ( void * img ) {
      return uiHalfSizegmImage ( ( GMIMG * ) img ) ;
  }
  void *uiScalegmImage ( GMIMG *png , float fac ) {
      int w , h , xoffset , yoffset , bkgrclr;
      float rzfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      if ( png == NULL ) return NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      w = image->columns*fac;
      h = image->rows*fac;
      xoffset = png->xoffset*fac;
      yoffset = png->yoffset*fac;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
      resize_image = ScaleImage ( image , w , h , Exception ) ;
//  resize_image=ThumbnailImage(image,w,h,Exception);
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = xoffset;
      png->yoffset = yoffset;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *uiCropgmImage ( GMIMG *png , int xl , int yl , int xu , int yu ) {
      int bkgrclr , w , h;
      float rzfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      RectangleInfo rect;
      if ( png == NULL ) return NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      if ( xl < xu ) rect.x = xl;
      else rect.x = xu;
      if ( yl < yu ) rect.y = yl;
      else rect.y = yu;
      rect.width = abs ( xu-xl ) +1;
      rect.height = abs ( yu-yl ) +1;
      w = png->image_width;
      h = png->image_height;
      if ( ( rect.x+rect.width ) < 0 ) return NULL;
      if ( ( rect.y+rect.height ) < 0 ) return NULL;
      if ( rect.x > w ) return NULL;
      if ( rect.y > h ) return NULL;
      if ( rect.x < 0 ) rect.x = 0;
      if ( rect.y < 0 ) rect.y = 0;
      if ( ( rect.x+rect.width ) > w ) rect.width = w- rect.x;
      if ( ( rect.y+rect.height ) > h ) rect.height = h- rect.y;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
      resize_image = CropImage ( image , & rect , Exception ) ;
//  resize_image=ThumbnailImage(image,w,h,Exception);
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgCropImage ( void *img , int xl , int yl , int xu , int yu ) {
      return uiCropgmImage ( ( GMIMG * ) img , xl , yl , xu , yu ) ;
  }
  void *uiRotateAboutgmImage ( GMIMG *png , float angle , int xo , int yo ) {
/*
  Creates a Rotated new gmImage
  It is rotated about the given point (xo,yo)
*/
      int bkgrclr , w , h , W , H , nH , nW;
      GMIMG *dpng , *tmpimg;
      double dang , cs , ss , dl , dl1 , r , nang , falpha;
      int xm , ym , nxm , nym;
      int x , y;
      int i , j , k , kk , in , jn , dx , dy;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels , *spixels;
      unsigned int opacity , alpha , red , green , blue , dalpha;
      float f , f1;
      RectangleInfo rect;
      if ( png == NULL ) return NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      w = image->columns;
      h = image->rows;
      dl = ( ( double ) ( xo*xo+yo*yo ) ) ;
      dl1 = ( ( double ) ( ( w-xo ) * ( w-xo ) + ( h-yo ) * ( h-yo ) ) ) ;
      if ( dl1 > dl ) dl = dl1;
      dl1 = ( ( double ) ( ( w-xo ) * ( w-xo ) + ( yo ) * ( yo ) ) ) ;
      if ( dl1 > dl ) dl = dl1;
      dl1 = ( ( double ) ( ( xo ) * ( xo ) + ( h-yo ) * ( h-yo ) ) ) ;
      if ( dl1 > dl ) dl = dl1;
      dl = sqrt ( dl ) ;
      W = dl+0.5;
//  W = 2*W-1;
      W = 2*W+1;
      H = W;
      dpng = uiCreategmImage ( NULL , W , W ) ;
      resize_image = dpng->image;
      W = resize_image->columns;
      H = resize_image->rows;
      xm = ( W-1 ) /2;
      ym = ( H-1 ) /2;
      pixels = GetImagePixels ( resize_image , 0 , 0 , resize_image->columns , resize_image->rows ) ;
          
      spixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      dang = angle*atan ( 1.0 ) /45.0;
      cs = cos ( dang ) ;
      ss = sin ( dang ) ;
#if 1
      k = 0;
      for ( j = 0;j < ( h ) ;j++ ) {
          for ( i = 0;i < ( w ) ;i++ ) {
              opacity = spixels [ k ] .opacity;
              alpha = 255- opacity;
              if ( ( alpha == 0 ) ) {k++;continue;}
              x = + ( i-xo ) +xm;
              y = H- ( int ) ( ( yo-j ) +ym ) ;
              kk = ( y*W+x ) ;
              pixels [ kk ] .blue = spixels [ k ] .blue;
              pixels [ kk ] .green = spixels [ k ] .green;
              pixels [ kk ] .red = spixels [ k ] .red;
              pixels [ kk ] .opacity = spixels [ k ] .opacity;
              k++;
          }
      }
      SyncImagePixels ( resize_image ) ;
      if ( angle != 0 ) {
          tmpimg = ( GMIMG * ) uiRotategmImage ( dpng , angle ) ;
          uiCleangmImage ( dpng ) ;
          nW = tmpimg->image_width;
          nH = tmpimg->image_height;
//  printf("W:H: %d %d %d %d\n",W,H,nW,nH);
          spixels = GetImagePixels ( tmpimg->image , 0 , 0 , nW , nH ) ;
          nxm = ( nW-1 ) /2;
          nym = ( nH-1 ) /2;
          dx = nxm -xm;
          dy = nym - ym;
          kk = 0;
          for ( j = 0;j < ( H ) ;j++ ) {
              jn = j+dy;
              if ( jn < 0 ) continue;
              if ( jn >= ( nH ) ) break;
              for ( i = 0;i < ( W ) ;i++ ) {
                  in = i+dx;
                  if ( in < 0 ) continue;
                  if ( in >= ( nW ) ) {kk++;continue;}
                  k = jn* ( nW ) +in;
                  opacity = spixels [ k ] .opacity;
                  alpha = 255- opacity;
                  if ( ( alpha == 0 ) ) {kk++;continue;}
                  pixels [ kk ] .blue = spixels [ k ] .blue;
                  pixels [ kk ] .green = spixels [ k ] .green;
                  pixels [ kk ] .red = spixels [ k ] .red;
                  pixels [ kk ] .opacity = spixels [ k ] .opacity;
                  kk++;
              }
          }
//  uiFreeImage(dpng);
//  dpng=tmpimg;
//  printf("L:W: %d %d %d %d\n",L,W,dpng->image_width,dpng->image_height);
          uiFreeImage ( tmpimg ) ;
#else
/* Foolish; Do not use */
          k = 0;
          for ( j = 0;j < ( h ) ;j++ ) {
              for ( i = 0;i < ( w ) ;i++ ) {
                  opacity = spixels [ k ] .opacity;
                  alpha = 255- opacity;
                  if ( ( alpha == 0 ) ) {k++;continue;}
                  x = ( yo-j ) *ss+ ( i-xo ) *cs+xm;
                  y = H- ( int ) ( ( yo-j ) *cs- ( i-xo ) *ss+ym ) ;
                  kk = ( y*W+x ) ;
                  dalpha = ( 255-pixels [ kk ] .opacity ) ;
                  blue = spixels [ k ] .blue;
                  green = spixels [ k ] .green;
                  red = spixels [ k ] .red;
                  f = alpha/255.0;
                  f1 = 1-f;
                  if ( dalpha == 0 ) {f = 1.0;f1 = 0.;}
                  else {f1 = dalpha/255.0; }
                  blue = f*blue + f1*pixels [ kk ] .blue;
                  green = f*green + f1*pixels [ kk ] .green;
                  red = f*red + f1*pixels [ kk ] .red;
//        alpha   += dalpha;
                  if ( dalpha > alpha ) alpha = dalpha;
                  if ( blue > 255 ) blue = 255;
                  if ( green > 255 ) green = 255;
                  if ( green > 255 ) red = 255;
                  if ( green > 255 ) alpha = 255;
                  opacity = 255 -alpha;
                  pixels [ kk ] .blue = blue;
                  pixels [ kk ] .green = green;
                  pixels [ kk ] .red = red;
                  pixels [ kk ] .opacity = opacity;
                  k++;
              }
          }
          SyncImagePixels ( resize_image ) ;
#endif
      }
      return dpng;
  }
  void *kgRotateAboutImage ( void *img , float angle , int xo , int yo ) {
      return uiRotateAboutgmImage ( ( GMIMG * ) img , angle , xo , yo ) ;
  }
  void *uiRotategmImage ( GMIMG *png , float angle ) {
      int bkgrclr , w , h;
      float rzfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      RectangleInfo rect;
      if ( png == NULL ) return NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png->image ) ;
      image->background_color.red = 0;
      image->background_color.blue = 0;
      image->background_color.green = 0;
      image->background_color.opacity = 255;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
      resize_image = RotateImage ( image , ( double ) angle , Exception ) ;
//  resize_image=ThumbnailImage(image,w,h,Exception);
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      png->info = NULL;
      png->exce = NULL;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgRotateImage ( void *img , float angle ) {
      return uiRotategmImage ( ( GMIMG * ) img , angle ) ;
  }
  void *uiAppendgmImage ( GMIMG *png1 , GMIMG *png2 ) {
      int bkgrclr , w , h;
      float rzfac;
      GMIMG *png;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      RectangleInfo rect;
      if ( png1 == NULL ) return NULL;
      if ( png2 == NULL ) return NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png1->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      image->next = ( Image * ) ( png2->image ) ;
      bkgrclr = png1->bkgrclr;
      rzfac = png2->rzfac;
      resize_image = AppendImages ( image , 1 , Exception ) ;
//  resize_image=ThumbnailImage(image,w,h,Exception);
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      image->next = NULL;
      return png;
  }
  void *uiShadegmImage ( GMIMG *png ) {
      int bkgrclr , w , h;
      float rzfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      unsigned int gray = 150;
      double azimuth = 135.0 , elevation = 45.0;
      if ( png == NULL ) return NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
      resize_image = ShadeImage ( image , gray , azimuth , elevation , Exception ) ;
//  resize_image=ThumbnailImage(image,w,h,Exception);
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgShadeImage ( void *img ) {
      return uiShadegmImage ( ( GMIMG * ) img ) ;
  }
  void *uiReduceNoisegmImage ( GMIMG *png ) {
      int bkgrclr , w , h;
      float rzfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      unsigned int gray = 10;
      double azimuth = 135.0 , elvation = 45.0;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      if ( png == NULL ) return NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
      resize_image = ReduceNoiseImage ( image , 0.0 , Exception ) ;
//  resize_image=ThumbnailImage(image,w,h,Exception);
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      GetImageInfo ( Image_info ) ;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgReduceNoiseImage ( void *img ) {
      return uiReduceNoisegmImage ( ( GMIMG * ) img ) ;
  }
  void *uiBlurgmImage ( GMIMG *png ) {
      int bkgrclr , w , h;
      float rzfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      unsigned int gray = 10;
      double radius = 0.0 , sigma = 5.0;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      if ( png == NULL ) return NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
      resize_image = BlurImage ( image , radius , sigma , Exception ) ;
//  resize_image=ThumbnailImage(image,w,h,Exception);
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      GetImageInfo ( Image_info ) ;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgBlurImage ( void *img ) {
      return uiBlurgmImage ( ( GMIMG * ) img ) ;
  }
  void *uiEmbossgmImage ( GMIMG *png ) {
	/* Returns a NEW IMAGE */
      int bkgrclr , w , h;
      float rzfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      unsigned int gray = 255;
      double radius = 0.0 , sigma = 200.0;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      if ( png == NULL ) return NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
      resize_image = EmbossImage ( image , radius , sigma , Exception ) ;
//  resize_image=ThumbnailImage(image,w,h,Exception);
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgEmbossImage ( void *img ) {
	/* New  image returns */
      return uiEmbossgmImage ( ( GMIMG * ) img ) ;
  }
  void *uiSharpengmImage ( GMIMG *png ) {
      int bkgrclr , w , h;
      float rzfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      unsigned int gray = 10;
      double radius = 0.0 , sigma = 5.0;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      if ( png == NULL ) return NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
      resize_image = SharpenImage ( image , radius , sigma , Exception ) ;
//  resize_image=ThumbnailImage(image,w,h,Exception);
      GetExceptionInfo ( Exception ) ;
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgSharpenImage ( void *img ) {
      return uiSharpengmImage ( ( GMIMG * ) img ) ;
  }
  void *uiSpreadgmImage ( GMIMG *png ) {
      int bkgrclr , w , h;
      float rzfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      unsigned int gray = 10;
      double radius = 0.0 , sigma = 5.0;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      if ( png == NULL ) return NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
      resize_image = SpreadImage ( image , radius , Exception ) ;
//  resize_image=ThumbnailImage(image,w,h,Exception);
      GetExceptionInfo ( Exception ) ;
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      png->info = NULL;
      png->exce = NULL;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void *kgSpreadImage ( void *img ) {
      return uiSpreadgmImage ( ( GMIMG * ) img ) ;
  }
  void *uiEnhancegmImage ( GMIMG *png ) {
      int bkgrclr , w , h;
      float rzfac;
      Image *image , *resize_image = NULL;
      PixelPacket *pixels;
      unsigned int gray = 10;
      double radius = 0.0 , sigma = 5.0;
      if ( png == NULL ) return NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      bkgrclr = png->bkgrclr;
      rzfac = png->rzfac;
      resize_image = EnhanceImage ( image , Exception ) ;
//  resize_image=ThumbnailImage(image,w,h,Exception);
      if ( Exception->severity != UndefinedException ) {
          DestroyImageInfo ( Image_info ) ;
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
      resize_image->background_color.red = 0;
      resize_image->background_color.blue = 0;
      resize_image->background_color.green = 0;
      resize_image->background_color.opacity = 255;
      png = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      png->image = resize_image;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      strcpy ( png->Sign , "IMG" ) ;
      png->incode = 0;
      png->xoffset = 0;
      png->yoffset = 0;
      png->bkgrclr = bkgrclr;
      png->rzfac = 1.0;
      png->info = Image_info;
      png->exce = Exception;
      png->image_width = resize_image->columns;
      png->image_height = resize_image->rows;
      png->image_channels = 4;
      if ( resize_image->matte == 0 ) png->image_channels = 3;
      else png->image_channels = 4;
      png->image_rowbytes = png->image_width*png->image_channels;
      return png;
  }
  void uiWriteImage ( void *img , char *flname ) {
      int w , h;
      GMIMG *png;
      Image *image , *resize_image = NULL;
      png = ( GMIMG * ) img;
      if ( png == NULL ) return ;
      ImageInfo *Image_info = NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      Image_info = ( ImageInfo * ) png->info;
       ( void ) strcpy ( image->filename , flname ) ;
      WriteImage ( Image_info , ( Image * ) ( png->image ) ) ;
      return ;
  }
  void *kgEnhanceImage ( void *img ) {
      return uiEnhancegmImage ( ( GMIMG * ) img ) ;
  }
  void uiWritegmImage ( GMIMG *png , char *flname ) {
      int w , h;
      Image *image , *resize_image = NULL;
      if ( png == NULL ) return ;
      ImageInfo *Image_info = NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      Image_info = ( ImageInfo * ) png->info;
       ( void ) strcpy ( image->filename , flname ) ;
      WriteImage ( Image_info , ( Image * ) ( png->image ) ) ;
      return ;
  }
  void kgWriteImage ( void *img , char *flname ) { uiWritegmImage  \
      ( ( GMIMG * ) img , flname ) ;}
  void uiFreeGmImage ( void *png ) {
      GMIMG *img;
      img = ( GMIMG * ) png;
//      uiCloseGm();
      uiInitGm ( ) ;
//TCB 07/21
      DestroyImagePixels((Image *)(img->image));
      DestroyImage ( ( Image * ) ( img->image ) ) ;
#if 1
      if ( img->info != NULL ) DestroyImageInfo ( ( ImageInfo * ) img->info ) ;
      if ( img->exce != NULL ) {
          DestroyExceptionInfo ( ( ExceptionInfo * ) img->exce ) ;
          free ( img->exce ) ;
      }
      img->info = NULL;
      img->exce = NULL;
#endif
      return ;
  }
  int  kgFreeGmImage ( void *png ) {
       if(png == NULL) return 0;
       uiFreeGmImage (png);
       free(png);
       return 1;
  }
  void uiFreeFmgImage ( void *png ) {
      FMGIMG *img;
      img = ( FMGIMG * ) png;
      uiInitGm ( ) ;
//TCB 07/21
//  if( image_info== NULL) return;
//  DestroyImagePixels((Image *)(img->image));
      DestroyImage ( ( Image * ) ( img->image ) ) ;
  }
  void *uiShadowgmImage ( GMIMG *png , int xoffset , int yoffset , GMIMG *shimg )  \
      {
/*
    makes a shadow of png on shimg
    shimg of size png should exist
*/
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk , shval = 32;
      int opacity , alpha , red , blue , green;
      float f;
      GMIMG *dpng = NULL;
      Image *image , *tmpimg , *dimage;
      PixelPacket *pixels , *spixels;
      if ( png == NULL ) return NULL;
      if ( shimg == NULL ) return NULL;
      image = ( Image * ) ( png->image ) ;
      dimage = ( Image * ) ( shimg->image ) ;
      uiInitGm ( ) ;
      w = image->columns;
      h = image->rows;
      pixels = GetImagePixels ( dimage , 0 , 0 , dimage->columns , dimage->rows ) ;
      spixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      xsize = dimage->columns;
      ysize = dimage->rows;
      k = 0;
      for ( j = 0;j < ysize;j++ ) {
          for ( i = 0;i < xsize;i++ ) {
              opacity = ( unsigned int ) ( spixels [ k ] .opacity ) ;
              k++;
              if ( ( opacity == 255 ) ) {continue;}
              if ( ( i+xoffset ) >= xsize ) continue;
              if ( ( j+yoffset ) >= ysize ) continue;
              kk = ( ( j+yoffset ) *xsize+i+xoffset ) ;
#if 0
              blue = pixels [ kk ] .blue ;
              blue = 0x10;
              green = pixels [ kk ] .green;
              green = 0x10;
              red = pixels [ kk ] .red = red = 0x10;
              opacity = pixels [ kk ] .opacity;i opacity = 0x10;
              if ( blue < 0 ) blue = 0;
              if ( green < 0 ) green = 0;
              if ( red < 0 ) red = 0;
              if ( opacity < 0 ) opacity = 0;
#else
              f = ( opacity ) /255.0;
              blue = shval*f;
              green = shval*f;
              red = shval*f;
              opacity = 0x80;
#endif
              pixels [ kk ] .blue = blue;
              pixels [ kk ] .green = green;
              pixels [ kk ] .red = red;
              pixels [ kk ] .opacity = opacity;
          }
      }
      SyncImagePixels ( dimage ) ;
      return shimg;
  }
  void *kgShadowImage ( void *img , int xoffset , int yoffset , void *shimg ) {
      return uiShadowgmImage ( ( GMIMG * ) img , xoffset , yoffset ,  \
          ( GMIMG * ) shimg ) ;
  }
  void *uiShadowgmImage_o ( GMIMG *png , int xoffset , int yoffset ) {
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk;
      GMIMG *dpng = NULL;
      Image *image , *tmpimg , *dimage;
      PixelPacket *pixels , *spixels;
      if ( png == NULL ) return NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      w = image->columns;
      h = image->rows;
      tmpimg = AllocateImage ( ( ImageInfo * ) NULL ) ;
      tmpimg->columns = w;
      tmpimg->rows = h;
      SetImage ( tmpimg , TrueColorMatteType ) ;
      if ( tmpimg == NULL ) {
          printf ( "Failed to Allocate Image\n" ) ;
          return NULL;
      }
      dimage = CloneImage ( tmpimg , xsize , ysize , 1 , Exception ) ;
      if ( Exception->severity != UndefinedException ) {
          DestroyExceptionInfo ( Exception ) ;
          free ( Exception ) ;
          return NULL;
      }
//  DestroyImagePixels(tmpimg);
      DestroyImage ( tmpimg ) ;
      dimage->background_color.red = 0;
      dimage->background_color.blue = 0;
      dimage->background_color.green = 0;
      dimage->background_color.opacity = 255;
      dimage->matte = 1;
      pixels = SetImagePixels ( dimage , 0 , 0 , w , h ) ;
      SyncImagePixels ( dimage ) ;
      pixels = GetImagePixels ( dimage , 0 , 0 , dimage->columns , dimage->rows ) ;
      if ( ( dimage->columns != w ) || ( dimage->rows != h ) ) {
          printf ( "Size of Pixels :%ld:%d %ld:%d\n" , image->columns , w , image->rows , h ) ;
              
      }
      spixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      tmpimg = dimage;
      xsize = dimage->columns;
      ysize = dimage->rows;
      k = 0;
      for ( j = 0;j < ysize;j++ ) {
          for ( i = 0;i < xsize;i++ ) {
              pixels [ k ] .blue = 0x00;
              pixels [ k ] .green = 0x00;
              pixels [ k ] .red = 0x00;
              pixels [ k ] .opacity = 0xff;
              k++;
          }
      }
      k = 0;
      for ( j = 0;j < ysize;j++ ) {
          for ( i = 0;i < xsize;i++ ) {
//       if((spixels[k].red==0)&&(spixels[k].green==0)&&(spixels[k].blue==0)){k++;continue;}
              if ( ( spixels [ k ] .opacity == 0xff ) ) {k++;continue;}
              k++;
              if ( ( i+xoffset ) >= xsize ) continue;
              if ( ( j+yoffset ) >= ysize ) continue;
              kk = ( ( j+yoffset ) *xsize+i+xoffset ) ;
              pixels [ kk ] .blue = 0x10;
              pixels [ kk ] .green = 0x10;
              pixels [ kk ] .red = 0x10;
              pixels [ kk ] .opacity = 0xa0;
          }
      }
      SyncImagePixels ( dimage ) ;
      dpng = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      dpng->image = dimage;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      strcpy ( dpng->Sign , "IMG" ) ;
      dpng->image_width = dimage->columns;
      dpng->image_height = dimage->rows;
      dpng->incode = 0;
      dpng->xoffset = 0;
      dpng->yoffset = 0;
      dpng->bkgrclr = -1;
      dpng->rzfac = 1.0;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
      if ( dimage->matte == 0 ) dpng->image_channels = 3;
      else dpng->image_channels = 4;
      dpng->image_rowbytes = dpng->image_width*dpng->image_channels;
#if 1
      k = 0;
      for ( j = 0;j < ysize;j++ ) {
          for ( i = 0;i < xsize;i++ ) {
//       if((spixels[k].red==0)&&(spixels[k].green==0)&&(spixels[k].blue==0)){k++;continue;}
              if ( ( spixels [ k ] .opacity == 0xff ) ) {k++;continue;}
              pixels [ k ] .blue = spixels [ k ] .blue;
              pixels [ k ] .green = spixels [ k ] .green;
              pixels [ k ] .red = spixels [ k ] .red;
              pixels [ k ] .opacity = spixels [ k ] .opacity;
              k++;
          }
      }
      SyncImagePixels ( dimage ) ;
#endif
      return dpng;
  }
#if 1
  void *uiMergeTransparentgmImage ( GMIMG *png1 , GMIMG *png2 , int Xshft , int Yshft )  \
      {
/*
  Second Picture is put on the first and the
  pointer to the first picture is returned
*/
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk;
      unsigned int opacity , alpha , alphas;
      int xoff , yoff;
      float f , f1;
      GMIMG *dpng = NULL;
      Image *image , *tmpimg , *dimage , *oimage;
      PixelPacket *pixels , *spixels , *opixels;
      unsigned char r , g , b , bg_r , bg_g , bg_b;
      unsigned int red , green , blue;
      if ( png1 == NULL ) return NULL;
      if ( png2 == NULL ) return NULL;
      image = ( Image * ) ( png1->image ) ;
      dimage = ( Image * ) ( png2->image ) ;
      uiInitGm ( ) ;
      pixels = GetImagePixels ( dimage , 0 , 0 , dimage->columns , dimage->rows ) ;
      spixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      w = image->columns;
      h = image->rows;
      xsize = image->columns;
      ysize = image->rows;
      xoff = ( xsize - dimage->columns ) *0.5+Xshft;
      yoff = ( ysize - dimage->rows ) *0.5+Yshft;
      k = 0;
      for ( j = yoff;j < ( yoff+dimage->rows ) ;j++ ) {
          for ( i = xoff;i < ( xoff+dimage->columns ) ;i++ ) {
              opacity = pixels [ k ] .opacity;
              alpha = 255- opacity;
              if ( ( alpha == 0 ) ) {k++;continue;}
              kk = ( j*xsize+i ) ;
              opacity = spixels [ kk ] .opacity;
              alphas = 255 -opacity;
              if ( alpha == 0xff ) {
                  blue = pixels [ k ] .blue;
                  green = pixels [ k ] .green;
                  red = pixels [ k ] .red;
              }
              else {
                  f1 = alphas/255.0;
                  f = alpha/255.0;
//          f1 =f+f1;
//          f = f/f1;
//          f1 = f1/(1.+f1);
//TCTCB
                  bg_b = spixels [ kk ] .blue ;
                  bg_g = spixels [ kk ] .green ;
                  bg_r = spixels [ kk ] .red ;
                  b = pixels [ k ] .blue;
                  g = pixels [ k ] .green;
                  r = pixels [ k ] .red;
#if 0
                  if ( alphas == 0 ) f = 1.0;
                  alpha += alphas;
                  if ( alpha > 255 ) alpha = 255;
                  spixels [ kk ] .opacity = 255 -alpha;
#else
                  f1 = 1-f;
#endif
                  red = bg_r*f1+r*f;
                  green = bg_g*f1+g*f;
                  blue = bg_b*f1+b*f;
              }
              if ( blue > 255 ) blue = 255;
              if ( green > 255 ) green = 255;
              if ( red > 255 ) red = 255;
              spixels [ kk ] .blue = blue;
              spixels [ kk ] .green = green;
              spixels [ kk ] .red = red;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      return png1;
  }
#else
#endif
  void *uiAddTransparentgmImage ( GMIMG *png1 , GMIMG *png2 , int Xshft , int Yshft )  \
      {
/*
  Second Picture is put on the first and the
  pointer to the first picture is returned
*/
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk;
      unsigned int opacity , alpha , alphas;
      int xoff , yoff;
      float f , f1;
      GMIMG *dpng = NULL;
      Image *image , *tmpimg , *dimage , *oimage;
      PixelPacket *pixels , *spixels , *opixels;
      unsigned char r , g , b , bg_r , bg_g , bg_b;
      unsigned int red , green , blue;
      if ( png1 == NULL ) return NULL;
      if ( png2 == NULL ) return NULL;
      image = ( Image * ) ( png1->image ) ;
      dimage = ( Image * ) ( png2->image ) ;
      uiInitGm ( ) ;
      pixels = GetImagePixels ( dimage , 0 , 0 , dimage->columns , dimage->rows ) ;
      spixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      w = image->columns;
      h = image->rows;
      xsize = image->columns;
      ysize = image->rows;
      xoff = Xshft;
      yoff = Yshft;
      k = 0;
      for ( j = yoff;j < ( yoff+dimage->rows ) ;j++ ) {
          for ( i = xoff;i < ( xoff+dimage->columns ) ;i++ ) {
              opacity = pixels [ k ] .opacity;
              alpha = 255- opacity;
              if ( ( alpha == 0 ) ) {k++;continue;}
              kk = ( j*xsize+i ) ;
              opacity = spixels [ kk ] .opacity;
              alphas = 255 -opacity;
              if ( alpha == 0xff ) {
                  blue = pixels [ k ] .blue;
                  green = pixels [ k ] .green;
                  red = pixels [ k ] .red;
              }
              else {
                  f1 = alphas/255.0;
                  f = alpha/255.0;
//          f1 =f+f1;
//          f = f/f1;
//          f1 = f1/(1.+f1);
//TCTCB
                  bg_b = spixels [ kk ] .blue ;
                  bg_g = spixels [ kk ] .green ;
                  bg_r = spixels [ kk ] .red ;
                  b = pixels [ k ] .blue;
                  g = pixels [ k ] .green;
                  r = pixels [ k ] .red;
#if 0
                  if ( alphas == 0 ) f = 1.0;
                  alpha += alphas;
                  if ( alpha > 255 ) alpha = 255;
                  spixels [ kk ] .opacity = 255 -alpha;
#else
                  f1 = 1-f;
#endif
                  red = bg_r*f1+r*f;
                  green = bg_g*f1+g*f;
                  blue = bg_b*f1+b*f;
              }
              if ( blue > 255 ) blue = 255;
              if ( green > 255 ) green = 255;
              if ( red > 255 ) red = 255;
              spixels [ kk ] .blue = blue;
              spixels [ kk ] .green = green;
              spixels [ kk ] .red = red;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      return png1;
  }
  void *kgAddTransparentImage ( void *png1 , void *png2 , int Xshft , int Yshft )  \
      {
      int w1 , h1 , w2 , h2 , h , w;
      GMIMG *img1 , *img2 , *img3;
      img1 = ( GMIMG * ) png1;
      img2 = ( GMIMG * ) png2;
      if ( img1 == NULL ) { printf ( "NULL\n" ) ;return NULL;}
      if ( img2 == NULL ) { printf ( "AddTrasp:NULL 2\n" ) ;return png1;}
      w1 = img1->image_width;
      h1 = img1->image_height;
      w2 = img2->image_width;
      h2 = img2->image_height;
//   printf("%d %d %d %d\n",w1,h1,w2,h2);
      if ( ( w2 > w1 ) || ( h2 > h1 ) ) {
          float fac , fac1;
          fac = ( float ) w1/ ( float ) w2;
          fac1 = ( float ) h1/ ( float ) h2;
          if ( fac1 < fac ) fac = fac1;
          img3 = kgResizeImage ( img2 , fac*0.8 ) ;
          img2 = uiAddTransparentgmImage ( img1 , img3 , Xshft , Yshft ) ;
          kgFreeImage ( img3 ) ;
          return img2;
      }
      else return uiAddTransparentgmImage ( ( GMIMG * ) png1 ,  \
          ( GMIMG * ) png2 , Xshft , Yshft ) ;
  }
  void *kgMergeTransparentImage ( void *png1 , void *png2 , int Xshft , int Yshft )  \
      {
      int w1 , h1 , w2 , h2 , h , w;
      GMIMG *img1 , *img2 , *img3;
      img1 = ( GMIMG * ) png1;
      img2 = ( GMIMG * ) png2;
      if ( img1 == NULL ) { printf ( "NULL\n" ) ;return NULL;}
      if ( img2 == NULL ) { printf ( "MergeTransp:NULL 2\n" ) ;return png1;}
      w1 = img1->image_width;
      h1 = img1->image_height;
      w2 = img2->image_width;
      h2 = img2->image_height;
//   printf("%d %d %d %d\n",w1,h1,w2,h2);
      if ( ( w2 > w1 ) || ( h2 > h1 ) ) {
          float fac , fac1;
          fac = ( float ) w1/ ( float ) w2;
          fac1 = ( float ) h1/ ( float ) h2;
          if ( fac1 < fac ) fac = fac1;
          img3 = kgResizeImage ( img2 , fac*0.8 ) ;
          img2 = uiMergeTransparentgmImage ( img1 , img3 , Xshft , Yshft ) ;
          kgFreeImage ( img3 ) ;
          return img2;
      }
      else return uiMergeTransparentgmImage ( ( GMIMG * ) png1 ,  \
          ( GMIMG * ) png2 , Xshft , Yshft ) ;
  }
  void *uiCopygmImage_o ( GMIMG *png ) {
/* 
   Return a clean image of same size
*/
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk;
      GMIMG *dpng = NULL;
      Image *image , *tmpimg;
      PixelPacket *pixels , *spixels;
      if ( png == NULL ) return NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      tmpimg = CloneImage ( image , image->columns , image->rows , 1 , Exception ) ;
      pixels = SetImagePixels ( tmpimg , 0 , 0 , image->columns , image->rows ) ;
      SyncImagePixels ( tmpimg ) ;
      dpng = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      dpng->image = tmpimg;
      Image_info = CloneImageInfo ( ( ImageInfo * ) NULL ) ;
      GetImageInfo ( Image_info ) ;
      strcpy ( png->Sign , "IMG" ) ;
      strcpy ( dpng->Sign , "IMG" ) ;
      dpng->image_width = tmpimg->columns;
      dpng->image_height = tmpimg->rows;
      dpng->incode = 0;
      dpng->xoffset = 0;
      dpng->yoffset = 0;
      dpng->bkgrclr = -1;
      dpng->rzfac = 1.0;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
      if ( tmpimg->matte == 0 ) dpng->image_channels = 3;
      else dpng->image_channels = 4;
      dpng->image_rowbytes = dpng->image_width*dpng->image_channels;
      uiCleangmImage ( dpng ) ;
      return dpng;
  }
  void *uiCopygmImage ( GMIMG *png ) {
/* 
   Return a clean image of same size
*/
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk;
      GMIMG *dpng = NULL;
      Image *image , *tmpimg;
      PixelPacket *pixels , *spixels;
      if ( png == NULL ) return NULL;
      ImageInfo *Image_info = NULL;
      ExceptionInfo *Exception;
      Exception = ( ExceptionInfo * ) malloc ( sizeof ( ExceptionInfo ) ) ;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      GetExceptionInfo ( Exception ) ;
      tmpimg = CloneImage ( image , image->columns , image->rows , 1 , Exception ) ;
      pixels = SetImagePixels ( tmpimg , 0 , 0 , image->columns , image->rows ) ;
      SyncImagePixels ( tmpimg ) ;
      dpng = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      *dpng = *png;
      dpng->image = tmpimg;
      GetExceptionInfo ( Exception ) ;
      png->info = Image_info;
      png->exce = Exception;
//  uiCleangmImage(dpng);
#if 1
// Added as on 18th Jan 2018
      spixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      k = 0;
      for ( j = 0;j < ( image->rows ) ;j++ ) {
          for ( i = 0;i < ( image->columns ) ;i++ ) {
              pixels [ k ] .blue = spixels [ k ] .blue;
              pixels [ k ] .green = spixels [ k ] .green;
              pixels [ k ] .red = spixels [ k ] .red;
              pixels [ k ] .opacity = spixels [ k ] .opacity;
              k++;
          }
      }
      SyncImagePixels ( tmpimg ) ;
#endif
      return dpng;
  }
  void *kgCopyImage ( void *img ) {
      return uiCopygmImage ( ( GMIMG * ) img ) ;
  }
  void *uiCleangmImage ( GMIMG *png ) {
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk;
      Image *image;
      PixelPacket *pixels;
      if ( png == NULL ) return NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      pixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      xsize = image->columns;
      ysize = image->rows;
      k = 0;
      for ( j = 0;j < ( image->rows ) ;j++ ) {
          for ( i = 0;i < ( image->columns ) ;i++ ) {
              pixels [ k ] .blue = 0;
              pixels [ k ] .green = 0;
              pixels [ k ] .red = 0;
              pixels [ k ] .opacity = 255;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      return png;
  }
  void *kgCleanImage ( void *png ) {
      return uiCleangmImage ( ( GMIMG * ) png ) ;
  }
  void *uiFlipgmImage ( GMIMG *png ) {
/* X axis reflection */
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk;
      unsigned int opacity , alpha , alphas;
      int xoff , yoff;
      float f , f1;
      GMIMG *dpng = NULL;
      Image *image , *tmpimg , *dimage , *oimage;
      PixelPacket *pixels , *spixels , *opixels;
      unsigned char r , g , b , bg_r , bg_g , bg_b;
      unsigned int red , green , blue;
      if ( png == NULL ) return NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      pixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      w = image->columns;
      h = image->rows;
      xsize = w/2;
      ysize = h/2;
      k = 0;
      for ( j = 0;j < ( h ) ;j++ ) {
          for ( i = 0;i < ( xsize ) ;i++ ) {
              k = ( j*w+i ) ;
              kk = ( j*w+w-1-i ) ;
              opacity = pixels [ k ] .opacity;
              blue = pixels [ k ] .blue;
              green = pixels [ k ] .green;
              red = pixels [ k ] .red;
              pixels [ k ] .opacity = pixels [ kk ] .opacity;
              pixels [ k ] .blue = pixels [ kk ] .blue;
              pixels [ k ] .green = pixels [ kk ] .green;
              pixels [ k ] .red = pixels [ kk ] .red;
              pixels [ kk ] .opacity = opacity;
              pixels [ kk ] .blue = blue;
              pixels [ kk ] .green = green;
              pixels [ kk ] .red = red;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      return png;
  }
  void *kgFlipImage ( void *png ) {
      return uiFlipgmImage ( ( GMIMG * ) png ) ;
  }
  void *uiFlopgmImage ( GMIMG *png ) {
/*
  Y axis reflection
*/
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk;
      unsigned int opacity , alpha , alphas;
      int xoff , yoff;
      float f , f1;
      GMIMG *dpng = NULL;
      Image *image , *tmpimg , *dimage , *oimage;
      PixelPacket *pixels , *spixels , *opixels;
      unsigned char r , g , b , bg_r , bg_g , bg_b;
      unsigned int red , green , blue;
      if ( png == NULL ) return NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      pixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      w = image->columns;
      h = image->rows;
      xsize = w/2;
      ysize = h/2;
      k = 0;
      for ( j = 0;j < ( ysize ) ;j++ ) {
          for ( i = 0;i < ( w ) ;i++ ) {
              k = ( j*w+i ) ;
              kk = ( ( h-1-j ) *w+i ) ;
              opacity = pixels [ k ] .opacity;
              blue = pixels [ k ] .blue;
              green = pixels [ k ] .green;
              red = pixels [ k ] .red;
              pixels [ k ] .opacity = pixels [ kk ] .opacity;
              pixels [ k ] .blue = pixels [ kk ] .blue;
              pixels [ k ] .green = pixels [ kk ] .green;
              pixels [ k ] .red = pixels [ kk ] .red;
              pixels [ kk ] .opacity = opacity;
              pixels [ kk ] .blue = blue;
              pixels [ kk ] .green = green;
              pixels [ kk ] .red = red;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      return png;
  }
  void *kgFlopImage ( void *png ) {
      return uiFlopgmImage ( ( GMIMG * ) png ) ;
  }
  void *uiMaskgmImage ( GMIMG *png , GMIMG *mask ) {
/*
  First Picture is masked with the  second and the
  pointer and new image is returned
  Image is also scaled to mask
*/
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk;
      unsigned char opacity;
      GMIMG *dpng = NULL , *tmpimg;
      Image *image , *dimage;
      PixelPacket *pixels , *spixels;
      if ( png == NULL ) return NULL;
      if ( mask == NULL ) return NULL;
      mask = uiScalegmImage ( mask , 1.0 ) ;
      image = ( Image * ) ( png->image ) ;
      dimage = ( Image * ) ( mask->image ) ;
      uiInitGm ( ) ;
      tmpimg = uiChangeSizegmImage ( png , dimage->columns , dimage->rows , 1 ) ;
      image = ( Image * ) ( tmpimg->image ) ;
      pixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      spixels = GetImagePixels ( dimage , 0 , 0 , dimage->columns , dimage->rows ) ;
      w = image->columns;
      h = image->rows;
      xsize = w;
      ysize = h;
      k = 0;
      for ( j = 0;j < ( ysize ) ;j++ ) {
          for ( i = 0;i < ( w ) ;i++ ) {
              k = ( j*w+i ) ;
              kk = ( ( j ) *dimage->columns+i ) ;
              spixels [ kk ] .opacity = 255 - spixels [ kk ] .red;
              opacity = spixels [ kk ] .opacity;
              if ( opacity != 255 ) {
                  spixels [ kk ] .opacity = pixels [ k ] .opacity;
                  spixels [ kk ] .red = pixels [ k ] .red;
                  spixels [ kk ] .green = pixels [ k ] .green;
                  spixels [ kk ] .blue = pixels [ k ] .blue;
              }
          }
      }
#if 0 //old version
      k = 0;
      for ( j = 0;j < ( ysize ) ;j++ ) {
          for ( i = 0;i < ( w ) ;i++ ) {
              k = ( j*w+i ) ;
              kk = ( ( j ) *dimage->columns+i ) ;
              opacity = spixels [ kk ] .opacity;
              if ( opacity != 0xff ) {
                  spixels [ kk ] .opacity = 0x00;
                  spixels [ kk ] .red = pixels [ k ] .red;
                  spixels [ kk ] .green = pixels [ k ] .green;
                  spixels [ kk ] .blue = pixels [ k ] .blue;
              }
          }
      }
#endif
      SyncImagePixels ( dimage ) ;
      dpng = ( GMIMG * ) Malloc ( sizeof ( GMIMG ) ) ;
      dpng->image = dimage;
      strcpy ( dpng->Sign , "IMG" ) ;
      dpng->image_width = dimage->columns;
      dpng->image_height = dimage->rows;
      dpng->incode = 0;
      dpng->xoffset = 0;
      dpng->yoffset = 0;
      dpng->bkgrclr = -1;
      dpng->rzfac = 1.0;
      dpng->info = NULL;
      dpng->exce = NULL;
      if ( dimage->matte == 0 ) dpng->image_channels = 3;
      else dpng->image_channels = 4;
      dpng->image_rowbytes = dpng->image_width*dpng->image_channels;
      uiFreeImage ( tmpimg ) ;
      return dpng;
  }
  void *kgMaskImage ( void *png , void *mask ) {
      return uiMaskgmImage ( ( GMIMG * ) png , ( GMIMG * ) mask ) ;
  }
  void *kgChangeBrightness ( void *Img , float fac ) {
  /* Adding Transparency to Image */
      int w , h , i , j , k;
      GMIMG *png;
      Image *img;
      int channels = 3 , opacity , red , green , blue;
      float r , g , b , hu , s , v;
      float f;
      PixelPacket *pixels , *dest;
      if ( Img == NULL ) return NULL;
      png = ( GMIMG * ) Img;
      channels = png->image_channels;
      png->image_channels = 4;
      img = png->image;
      w = img->columns;
      h = img->rows;
      f = fac*255;
      img->matte = 1;
      img->background_color.opacity = 255;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( Img ) ;
      for ( i = 0; i < h; ++i ) {
          dest = pixels + i*w;
          for ( j = 0;j < w; j++ ) {
              r = dest->red;
              g = dest->green;
              b = dest->blue;
              RGBtoHSV ( r , g , b , & hu , & s , & v ) ;
              v = v*fac;
              if ( v > 1.0 ) v = 1.0;
              HSVtoRGB ( & r , & g , & b , hu , s , v ) ;
              dest->red = r;
              dest->green = g;
              dest->blue = b;
              dest++;
          }
      }
      SyncImagePixels ( img ) ;
      return Img;
  }
int  kgSetImageColor ( void *Img , int r,int g,int b ) {
  /* Adding Transparency to Image */
      int w , h , i , j , k;
      GMIMG *png;
      Image *img;
      int channels = 3 , opacity , red , green , blue;
      float  hu , s , v;
      float f;
      PixelPacket *pixels , *dest;
      if ( Img == NULL ) return 0;
      png = ( GMIMG * ) Img;
      channels = png->image_channels;
      png->image_channels = 4;
      img = png->image;
      w = img->columns;
      h = img->rows;
      img->matte = 1;
      img->background_color.opacity = 255;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( Img ) ;
      for ( i = 0; i < h; ++i ) {
          dest = pixels + i*w;
          for ( j = 0;j < w; j++ ) {
            if(dest->opacity != 255) {
              dest->red = r;
              dest->green = g;
              dest->blue = b;
            }
              dest++;
          }
      }
      SyncImagePixels ( img ) ;
      return 1;
  }
  int kgSetPixelAlpha ( void *Img , int col,int row,int alpha ) {
  /* Adding Transparency to Image */
      int w , h , i , j , k;
      GMIMG *png;
      Image *img;
      int channels = 3 , opacity , red , green , blue;
      float r , g , b , hu , s , v;
      float f;
      PixelPacket *pixels , *dest;
      if ( Img == NULL ) return 0;
      png = ( GMIMG * ) Img;
      channels = png->image_channels;
      png->image_channels = 4;
      img = png->image;
      w = img->columns;
      h = img->rows;
      img->matte = 1;
      img->background_color.opacity = 255;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( Img ) ;
      dest = pixels + row*w+col;
      dest->opacity = alpha;
      SyncImagePixels ( img ) ;
      return 1;
  }
  void *kgModifyImageHSV ( void *Img , float hfac , float sfac , float vfac ) {
  /* Adding Transparency to Image */
      int w , h , i , j , k;
      GMIMG *png;
      Image *img;
      int channels = 3 , opacity , red , green , blue;
      float r , g , b , hu , s , v;
      float f;
      PixelPacket *pixels , *dest;
      if ( Img == NULL ) return NULL;
      png = ( GMIMG * ) Img;
      channels = png->image_channels;
      png->image_channels = 4;
      img = png->image;
      w = img->columns;
      h = img->rows;
      img->matte = 1;
      img->background_color.opacity = 255;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( Img ) ;
      for ( i = 0; i < h; ++i ) {
          dest = pixels + i*w;
          for ( j = 0;j < w; j++ ) {
              r = dest->red;
              g = dest->green;
              b = dest->blue;
              RGBtoHSV ( r , g , b , & hu , & s , & v ) ;
              v = v*vfac;
              if ( v > 1.0 ) v = 1.0;
              s = s*sfac;
              if ( s > 1.0 ) s = 1.0;
              hu = hu*hfac;
              hu = ( ( int ) hu ) %360;
              HSVtoRGB ( & r , & g , & b , hu , s , v ) ;
              dest->red = r;
              dest->green = g;
              dest->blue = b;
              dest++;
          }
      }
      SyncImagePixels ( img ) ;
      return Img;
  }
  void *kgAddTransparency ( void *Img , float transp ) {
  /* Adding Transparency to Image */
      int w , h , i , j , k;
      GMIMG *png;
      Image *img;
      int channels = 3 , opacity;
      float f;
      PixelPacket *pixels , *dest;
      png = ( GMIMG * ) Img;
      channels = png->image_channels;
      png->image_channels = 4;
      img = png->image;
      w = img->columns;
      h = img->rows;
      if ( transp > 1.0 ) transp = 1.0;
      if ( transp < 0. ) transp = 0.0;
      f = transp*255;
      img->matte = 1;
      img->background_color.opacity = 255;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( Img ) ;
      for ( i = 0; i < h; ++i ) {
          dest = pixels + i*w;
          for ( j = 0;j < w; j++ ) {
#if 1
              if ( dest->opacity < 255 ) {
                  opacity = dest->opacity + f+0.5;
                  if ( opacity > 255 ) opacity = 255;
                  dest->opacity = opacity;
              }
              else dest->opacity = 255;
#else
#endif
              dest++;
          }
      }
      SyncImagePixels ( img ) ;
      return Img;
  }
  void *kgAddTransparency_exp ( void *Img , float transp ) {
  /* Adding Transparency to Image */
/* Not Correct  for black */
      int w , h , i , j , k;
      GMIMG *png;
      Image *img;
      int channels = 3 , Opacity;
      float f;
      PixelPacket *pixels , *dest;
      png = ( GMIMG * ) Img;
      channels = png->image_channels;
      png->image_channels = 4;
      img = png->image;
      w = img->columns;
      h = img->rows;
      f = transp;
      img->matte = 1;
      img->background_color.opacity *= f;
      Opacity = 255* ( f ) ;
      pixels = ( PixelPacket * ) uiPixelsgmImage ( Img ) ;
      for ( i = 0; i < h; ++i ) {
          dest = pixels + i*w;
          for ( j = 0;j < w; j++ ) {
              if ( dest->opacity != 255 ) {
                  dest->opacity = Opacity;
              }
              else {
                  dest->red = 0;
                  dest->green = 0;
                  dest->blue = 0;
              }
              dest++;
          }
      }
      SyncImagePixels ( img ) ;
      return Img;
  }
#if 1
  void *uiMergegmImages ( GMIMG *png1 , GMIMG *png2 , int Xshft , int Yshft ) {
/*
  Second Picture is put on the first and the
  pointer to the first picture is returned
*/
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk;
      unsigned int opacity , alpha , alphas;
      int xoff , yoff;
      float f , f1;
      GMIMG *dpng = NULL;
      Image *image , *tmpimg , *dimage , *oimage;
      PixelPacket *pixels , *spixels , *opixels;
      unsigned char r , g , b , bg_r , bg_g , bg_b;
      unsigned int red , green , blue;
      if ( png1 == NULL ) return NULL;
      if ( png2 == NULL ) return NULL;
      image = ( Image * ) ( png1->image ) ;
      dimage = ( Image * ) ( png2->image ) ;
      uiInitGm ( ) ;
      pixels = GetImagePixels ( dimage , 0 , 0 , dimage->columns , dimage->rows ) ;
      spixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      w = image->columns;
      h = image->rows;
      xsize = image->columns;
      ysize = image->rows;
      xoff = ( xsize - dimage->columns ) *0.5+Xshft;
      yoff = ( ysize - dimage->rows ) *0.5+Yshft;
      k = 0;
      for ( j = yoff;j < ( yoff+dimage->rows ) ;j++ ) {
          for ( i = xoff;i < ( xoff+dimage->columns ) ;i++ ) {
              opacity = pixels [ k ] .opacity;
              alpha = 255- opacity;
              if ( ( alpha == 0 ) ) {k++;continue;}
              kk = ( j*xsize+i ) ;
              opacity = spixels [ kk ] .opacity;
              alphas = 255 -opacity;
              if ( alpha == 0xff ) {
                  blue = pixels [ k ] .blue;
                  green = pixels [ k ] .green;
                  red = pixels [ k ] .red;
              }
              else {
                  f1 = alphas/255.0;
                  f = alpha/255.0;
//          f1 =f+f1;
//          f = f/f1;
//          f1 = f1/(1.+f1);
                  if ( alphas == 0 ) f = 1.0;
                  f1 = 1-f;
                  bg_b = spixels [ kk ] .blue ;
                  bg_g = spixels [ kk ] .green ;
                  bg_r = spixels [ kk ] .red ;
                  b = pixels [ k ] .blue;
                  g = pixels [ k ] .green;
                  r = pixels [ k ] .red;
                  red = bg_r*f1+r*f;
                  green = bg_g*f1+g*f;
                  blue = bg_b*f1+b*f;
              }
              if ( blue > 255 ) blue = 255;
              if ( green > 255 ) green = 255;
              if ( red > 255 ) red = 255;
              spixels [ kk ] .blue = blue;
              spixels [ kk ] .green = green;
              spixels [ kk ] .red = red;
              alpha += alphas;
              if ( alpha > 255 ) alpha = 255;
              spixels [ kk ] .opacity = 255 -alpha;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      return png1;
  }
  void *kgMergeImages ( void *png1 , void *png2 , int Xshft , int Yshft ) {
      int w1 , h1 , w2 , h2 , h , w;
      GMIMG *img1 , *img2 , *img3;
      img1 = ( GMIMG * ) png1;
      img2 = ( GMIMG * ) png2;
      if ( img1 == NULL ) { printf ( "NULL\n" ) ;return NULL;}
      if ( img2 == NULL ) {
//         printf ( "Merge:NULL 2\n" ) ;
         return png1;
      }
      w1 = img1->image_width;
      h1 = img1->image_height;
      w2 = img2->image_width;
      h2 = img2->image_height;
//   printf("%d %d %d %d\n",w1,h1,w2,h2);
      if ( ( w2 > w1 ) || ( h2 > h1 ) ) {
          float fac , fac1;
          fac = ( float ) w1/ ( float ) w2;
          fac1 = ( float ) h1/ ( float ) h2;
          if ( fac1 < fac ) fac = fac1;
          img3 = kgResizeImage ( img2 , fac*0.8 ) ;
          img2 = uiMergegmImages ( img1 , img3 , Xshft , Yshft ) ;
          kgFreeImage ( img3 ) ;
          return img2;
      }
      else return uiMergegmImages ( ( GMIMG * ) png1 ,  \
          ( GMIMG * ) png2 , Xshft , Yshft ) ;
  }
  void *uiAddgmImages_o ( GMIMG *png1 , GMIMG *png2 , int Xshft , int Yshft ) {
/*
  Second Picture is put on the first and the
  pointer to the first picture is returned
*/
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk;
      unsigned int opacity , alpha , alphas;
      int xoff , yoff;
      float f , f1;
      GMIMG *dpng = NULL;
      Image *image , *tmpimg , *dimage , *oimage;
      PixelPacket *pixels , *spixels , *opixels;
      unsigned char r , g , b , bg_r , bg_g , bg_b;
      unsigned int red , green , blue;
      if ( png1 == NULL ) return NULL;
      if ( png2 == NULL ) return NULL;
      image = ( Image * ) ( png1->image ) ;
      dimage = ( Image * ) ( png2->image ) ;
      uiInitGm ( ) ;
      pixels = GetImagePixels ( dimage , 0 , 0 , dimage->columns , dimage->rows ) ;
      spixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      w = image->columns;
      h = image->rows;
      xsize = image->columns;
      ysize = image->rows;
      xoff = Xshft;
      yoff = Yshft;
      k = 0;
      for ( j = yoff;j < ( yoff+dimage->rows ) ;j++ ) {
          for ( i = xoff;i < ( xoff+dimage->columns ) ;i++ ) {
              opacity = pixels [ k ] .opacity;
              alpha = 255- opacity;
              if ( ( alpha == 0 ) ) {k++;continue;}
              kk = ( j*xsize+i ) ;
              opacity = spixels [ kk ] .opacity;
              alphas = 255 -opacity;
              if ( alpha == 0xff ) {
                  blue = pixels [ k ] .blue;
                  green = pixels [ k ] .green;
                  red = pixels [ k ] .red;
              }
              else {
                  f1 = alphas/255.0;
                  f = alpha/255.0;
//          f1 =f+f1;
//          f = f/f1;
//          f1 = f1/(1.+f1);
                  if ( alphas == 0 ) f = 1.0;
                  f1 = 1-f;
                  bg_b = spixels [ kk ] .blue ;
                  bg_g = spixels [ kk ] .green ;
                  bg_r = spixels [ kk ] .red ;
                  b = pixels [ k ] .blue;
                  g = pixels [ k ] .green;
                  r = pixels [ k ] .red;
                  red = bg_r*f1+r*f;
                  green = bg_g*f1+g*f;
                  blue = bg_b*f1+b*f;
              }
              if ( blue > 255 ) blue = 255;
              if ( green > 255 ) green = 255;
              if ( red > 255 ) red = 255;
              spixels [ kk ] .blue = blue;
              spixels [ kk ] .green = green;
              spixels [ kk ] .red = red;
              alpha += alphas;
              if ( alpha > 255 ) alpha = 255;
              spixels [ kk ] .opacity = 255 -alpha;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      return png1;
  }
  void *uiAddgmImages ( GMIMG *png1 , GMIMG *png2 , int Xshft , int Yshft ) {
/*
  Second Picture is put on the first and the
  pointer to the first picture is returned
*/
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk;
      unsigned int opacity , alpha , alphas;
      int xoff , yoff,xu,yu;
      float f , f1;
      GMIMG *dpng = NULL;
      Image *image , *tmpimg , *dimage , *oimage;
      PixelPacket *pixels , *spixels , *opixels;
      unsigned char r , g , b , bg_r , bg_g , bg_b;
      unsigned int red , green , blue;
      if ( png1 == NULL ) return NULL;
      if ( png2 == NULL ) return NULL;
      image = ( Image * ) ( png1->image ) ;
      dimage = ( Image * ) ( png2->image ) ;
      uiInitGm ( ) ;
      pixels = GetImagePixels ( dimage , 0 , 0 , dimage->columns , dimage->rows ) ;
      spixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      w = dimage->columns;
      h = dimage->rows;
      xsize = image->columns;
      ysize = image->rows;
      xoff = Xshft;
      yoff = Yshft;
      xu =xsize;
      if(xu>w+xoff ) xu=w+xoff;
      yu = ysize;
      if(yu>h+yoff) yu = h+yoff;
      k = 0;
      for ( j = yoff;j < ( yu ) ;j++ ) {
          for ( i = xoff;i < ( xu ) ;i++ ) {
              k = (j-yoff)*w+(i-xoff);
              opacity = pixels [ k ] .opacity;
              alpha = 255- opacity;
              if ( ( alpha == 0 ) ) {k++;continue;}
              kk = ( j*xsize+i ) ;
              opacity = spixels [ kk ] .opacity;
              alphas = 255 -opacity;
              if ( alpha == 0xff ) {
                  blue = pixels [ k ] .blue;
                  green = pixels [ k ] .green;
                  red = pixels [ k ] .red;
              }
              else {
                  f1 = alphas/255.0;
                  f = alpha/255.0;
//          f1 =f+f1;
//          f = f/f1;
//          f1 = f1/(1.+f1);
                  if ( alphas == 0 ) f = 1.0;
                  f1 = 1-f;
                  bg_b = spixels [ kk ] .blue ;
                  bg_g = spixels [ kk ] .green ;
                  bg_r = spixels [ kk ] .red ;
                  b = pixels [ k ] .blue;
                  g = pixels [ k ] .green;
                  r = pixels [ k ] .red;
                  red = bg_r*f1+r*f;
                  green = bg_g*f1+g*f;
                  blue = bg_b*f1+b*f;
              }
              if ( blue > 255 ) blue = 255;
              if ( green > 255 ) green = 255;
              if ( red > 255 ) red = 255;
              spixels [ kk ] .blue = blue;
              spixels [ kk ] .green = green;
              spixels [ kk ] .red = red;
              alpha += alphas;
              if ( alpha > 255 ) alpha = 255;
              spixels [ kk ] .opacity = 255 -alpha;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      return png1;
  }
  void *kgAddImages ( void *png1 , void *png2 , int Xshft , int Yshft ) {
      int w1 , h1 , w2 , h2 , h , w;
      GMIMG *img1 , *img2 , *img3;
      img1 = ( GMIMG * ) png1;
      img2 = ( GMIMG * ) png2;
      if ( img1 == NULL ) { printf ( "AddImages: first NULL\n" ) ;return NULL;}
      if ( img2 == NULL ) { printf ( "AddImages: second NULL\n" ) ;return png1;}
      w1 = img1->image_width;
      h1 = img1->image_height;
      w2 = img2->image_width;
      h2 = img2->image_height;
//   printf("%d %d %d %d\n",w1,h1,w2,h2);
#if 0 //uiAddgmImages will take care
      if ( ( w2 > w1 ) || ( h2 > h1 ) ) {
          float fac , fac1;
          int xl=0,yl=0,xu=w2-1,yu=h2-1;
          if(w2>w1) xu = w1-1;
          if(h2>h1) yu = h1-1;
#if 0
          fac = ( float ) w1/ ( float ) w2;
          fac1 = ( float ) h1/ ( float ) h2;
          if ( fac1 < fac ) fac = fac1;
          img3 = kgResizeImage ( img2 , fac*0.8 ) ;
#else
          img3 = kgCropImage(img2,xl,yl,xu,yu);
#endif
          img2 = uiAddgmImages ( img1 , img3 , Xshft , Yshft ) ;
          kgFreeImage ( img3 ) ;
          return img2;
      }
      else return uiAddgmImages ( ( GMIMG * ) png1 ,  \
          ( GMIMG * ) png2 , Xshft , Yshft ) ;
#else
      return uiAddgmImages ( ( GMIMG * ) png1 ,  \
          ( GMIMG * ) png2 , Xshft , Yshft ) ;
#endif
  }
  void *uiReplacegmImage ( GMIMG *png1 , GMIMG *png2 , int Xshft , int Yshft ) {
/*
  Second Picture is put on the first and the
  pointer to the first picture is returned
*/
      int w , h , bkgrclr , xsize , ysize;
      float rzfac;
      int i , j , k = 0 , kk,ii,jj;
      unsigned int opacity , alpha , alphas;
      int xoff , yoff;
      float f , f1;
      GMIMG *dpng = NULL;
      Image *image , *tmpimg , *sndimage , *oimage;
      PixelPacket *pixels , *spixels , *opixels;
      unsigned char r , g , b , bg_r , bg_g , bg_b;
      unsigned int red , green , blue;
      if ( png1 == NULL ) return png2;
      if ( png2 == NULL ) return png1;
      image = ( Image * ) ( png1->image ) ;
      sndimage = ( Image * ) ( png2->image ) ;
      uiInitGm ( ) ;
      pixels = GetImagePixels ( sndimage , 0 , 0 , sndimage->columns , sndimage->rows ) ;
      spixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      w = sndimage->columns;
      h = sndimage->rows;
      xsize = image->columns;
      ysize = image->rows;
      xoff = Xshft;
      yoff = Yshft;
      k = 0;
      for ( j = 0;j < ( sndimage->rows ) ;j++ ) {
          jj = j+yoff;
          if(jj >= ysize ) continue;
          if(jj<0) continue;
          for ( i = 0;i < ( sndimage->columns ) ;i++ ) {
              ii = i+xoff;
              if(ii>=xsize) continue;
              if(ii< 0) continue;
              kk = jj*xsize+ii;
              k = j*w+i;
              opacity = pixels [ k ] .opacity;
            if(opacity != 255) {
              spixels [ kk ] .blue  = pixels[k].blue;
              spixels [ kk ] .green = pixels[k].green;
              spixels [ kk ] .red   = pixels[k].red;
              spixels [ kk ] .opacity = opacity;
            }
          }
      }
      SyncImagePixels ( image ) ;
      return png1;
  }
  void *kgReplaceImage ( void *png1 , void *png2 , int Xshft , int Yshft ) {
      int w1 , h1 , w2 , h2 , h , w;
      GMIMG *img1 , *img2 , *img3;
      img1 = ( GMIMG * ) png1;
      img2 = ( GMIMG * ) png2;
      if ( img1 == NULL ) { printf ( "NULL\n" ) ;return NULL;}
      if ( img2 == NULL ) { printf ( "Replace:NULL 2\n" ) ;return png1;}
      w1 = img1->image_width;
      h1 = img1->image_height;
      w2 = img2->image_width;
      h2 = img2->image_height;
//   printf("%d %d %d %d\n",w1,h1,w2,h2);
#if 0
      if ( ( w2 > w1 ) || ( h2 > h1 ) ) {
          float fac , fac1;
          fac = ( float ) w1/ ( float ) w2;
          fac1 = ( float ) h1/ ( float ) h2;
          if ( fac1 < fac ) fac = fac1;
          img3 = kgResizeImage ( img2 , fac*0.8 ) ;
          img2 = uiReplacegmImage ( img1 , img3 , Xshft , Yshft ) ;
          kgFreeImage ( img3 ) ;
          return img2;
      }
      else return uiReplacegmImage ( ( GMIMG * ) png1 ,  \
          ( GMIMG * ) png2 , Xshft , Yshft ) ;
#else
      return uiReplacegmImage ( ( GMIMG * ) png1 ,  \
          ( GMIMG * ) png2 , Xshft , Yshft ) ;
#endif
  }
  void *kgImagetoGray ( void *img ) {
/*
 Converts the Image to Gray scale
  
*/
      int i , j , k = 0;
      GMIMG *png = NULL;
      int gray = 0;
      int xsize , ysize;
      Image *image;
      PixelPacket *pixels;
      unsigned int red , green , blue;
      png = ( GMIMG * ) img;
      if ( png == NULL ) return NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      pixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      xsize = image->columns;
      ysize = image->rows;
      k = 0;
      for ( j = 0;j < ( ysize ) ;j++ ) {
          for ( i = 0;i < ( xsize ) ;i++ ) {
              blue = pixels [ k ] .blue;
              green = pixels [ k ] .green;
              red = pixels [ k ] .red;
              gray = 0.299*red+0.587*green+0.114*blue;
              pixels [ k ] .blue = gray;
              pixels [ k ] .green = gray;
              pixels [ k ] .red = gray;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      return png;
  }
  void *kgImageModifyColor ( void *img , float rfac , float gfac , float bfac )  \
      {
/*
 Converts the Image to Gray scale
  
*/
      int i , j , k = 0;
      GMIMG *png = NULL;
      int gray = 0;
      int xsize , ysize;
      Image *image;
      PixelPacket *pixels;
      unsigned int red , green , blue;
      png = ( GMIMG * ) img;
      if ( png == NULL ) return NULL;
      image = ( Image * ) ( png->image ) ;
      uiInitGm ( ) ;
      pixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      xsize = image->columns;
      ysize = image->rows;
      k = 0;
      for ( j = 0;j < ( ysize ) ;j++ ) {
          for ( i = 0;i < ( xsize ) ;i++ ) {
              blue = pixels [ k ] .blue*bfac;
              green = pixels [ k ] .green*gfac;
              red = pixels [ k ] .red*rfac;
//       gray = 0.299*red+0.587*green+0.114*blue;
              if ( blue > 255 ) blue = 255;
              if ( green > 255 ) green = 255;
              if ( red > 255 ) red = 255;
              pixels [ k ] .blue = blue;
              pixels [ k ] .green = green;
              pixels [ k ] .red = red;
              k++;
          }
      }
      SyncImagePixels ( image ) ;
      return png;
  }
  int kgGetImageSize ( void *img , int *xsize , int *ysize ) {
/*
 Converts the Image to Gray scale
  
*/
      int i , j , k = 0;
      GMIMG *png = NULL;
      int gray = 0;
      Image *image;
      png = ( GMIMG * ) img;
      *xsize = 0 , *ysize = 0;
      if ( png == NULL ) return 0;
      image = ( Image * ) ( png->image ) ;
      *xsize = image->columns;
      *ysize = image->rows;
      return 1;
  }
#endif
