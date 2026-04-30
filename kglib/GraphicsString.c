
  void * uiInitGraphicFontLists ( int font ) {
      char FontFile [ 500 ] ;
      int size=256;
      char *pt;
      int Font , FontSize;
      int count = 0,i=0;
      if ( FontList == NULL ) uiAddFonts ( ) ;
      count = Dcount ( FontList ) ;
      if(Grimgs== NULL){
         Grimgs=(IMG_STR ***) malloc(sizeof(IMG_STR **)*(count+1));
         for(i=0;i<count;i++) Grimgs[i]=NULL;
      }
      if ( ( FontList == NULL ) || ( count == 0 ) ) {
          Grimgs = NULL;
          return 1;
      }
      FontSize = size;
      Font = font%count;
      strcpy ( FontFile , ( char * ) Drecord ( FontList , Font ) ) ;
      if(Grimgs[font]==NULL) Grimgs[font] = ( IMG_STR ** ) kgFontChars ( FontFile , FontSize ) ;
      return Grimgs[font];;
  }
  void * uiGraphicsString (  char *str , int width , \
  int height , int font , int color ,int angle, int FontSize  )   {
/*
   Write a string image for Graphics use
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
      F.Imgs =(IMG_STR **) uiInitGraphicFontLists(font);
      if ( FontSize <= 0 ) F.Size = ( height-4 ) /2;
      else F.Size = FontSize;
      IMG = ( IMG_STR * ) uiComplexString ( str , F.Imgs , \
      font , cval , F.Size , height ) ;
      imgbk = NULL;
      gimg = ( GMIMG * ) ( IMG->img ) ;
      w = gimg->image_width;
      h = gimg->image_height;
      if ( ( w > ln-FontSize ) || ( h > height ) ) {
          float fac;
          if ( h > height ) h = height;
          if ( w > ( ln-FontSize ) ) w = ln-FontSize;
          fac = ( float ) ( ln-FontSize ) /w ;
          img = kgChangeSizeImage ( IMG->img , w , h ) ;
          kgFreeGmImage ( IMG->img ) ;
          IMG->xln = w;
          IMG->img = img;
      }
      x1 = 0;
      ln1 = IMG->xln+1;
      img = IMG->img;
      free ( IMG ) ;
      return img;
  }
