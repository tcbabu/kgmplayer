  void *uiMakeButtonImage ( DIALOG *D ,int width , int height , int fillcolor , int state,int highli ,  float rfac ,  int type ) \
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
      int font=1,FontSize=6,color=0;
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
           return NULL;
          case 9:
           return NULL;
          case 1:
              img = kgBoxedStringToImage ( NULL , NULL , width , height , font , fillcolor , highli , color , justfic , FontSize , rfac , state ) ;
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
              img = kgFilledStringToImage3 ( NULL , NULL , width , height , font , fillcolor , highli , color , D->gc.fill_clr , justfic , FontSize , rfac , state , 2.0 ) ;
              img1 = kgFilledStringToImage3 ( NULL , NULL , width-2 , height-2 , font , fillcolor , highli , color , D->gc.fill_clr , justfic , FontSize , rfac , state , 3.0 ) ;
              img = kgMergeImages ( img , img1 , 0 , 0 ) ;
              kgFreeImage ( img1 ) ;
          return img;
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
              img = kgShadedStringToImage ( NULL , NULL , width , height , font , fillcolor , highli , color , justfic , FontSize , rfac , state , type-4 ) ;
          return img;
          default:
              img = kgFilledStringToImage1 ( NULL , NULL , width , height , font , fillcolor , highli , color , D->gc.fill_clr , justfic , FontSize , rfac , state , 3.0 ) ;
          return img;
      }
  }
  void *uiAddButtonString ( DIALOG *D , char *str , void *image , void *xpm,int width , int height , int font , int color , int FontSize  ) \
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
      int justfic =0;
      void *img = NULL , *cimg , *img1 = NULL,*simg=NULL;
      float length;
      int resize=0;
      GMIMG *gimg;
      kgWC *wc;
      wc = D->wc;
      ln = width*0.85+0.5;
      off = ( width-ln ) *0.5;
      if ( state == -1 ) poff = -1;
      if(type > 9) return NULL;
      if(xpm != NULL){
            gimg = ( GMIMG * ) xpm;
            if ( ( gimg->image_width > width ) || ( gimg->image_height > height ) ) {
              xpm  = kgChangeSizeImage ( xpm , width , height ) ;
              resize =1;
            }
      }
      cimg = kgGetImageCopy ( D , image ) ;
      if ( cimg == NULL ) cimg=xpm;
      else {
          if(xpm != NULL){
              cimg = kgMergeImages ( cimg ,xpm, off , 0 ) ;
            }
          }
      }
      switch ( type ) {
          case 0:
          return cimg;
          case 9:
          return cimg;
          default:
          if ( ( str != NULL ) && ( str [ 0 ] != '\0' ) ) {
                simg = uiStringToImage(D,str,0,0,ln,height,font,color,FontSize, justfic ,-1);                     
                cimg = kgMergeImages ( cimg , simg , 0 , 0 ) ;
                kgFreeImage ( simg ) ;
          }
          return cimg;
      }
  }
