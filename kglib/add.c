  void uiString ( DIALOG *D , char *str , int x , int y , int width , int height , int font , int color , int FontSize , int justfic , int bkcolor ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    bkcolor : background color ; < 0 background will not be painted
*/
      char Buf[1000];
      char *Str;
      int ln , i , maxchar , temp;
      int x1 , ln1,old=0;
      void *img = NULL;
      float length;
      kgWC *wc;
      FONT_STR F;
      IMG_STR *IMG;
      void *imgbk , *fid;
      GMIMG *gimg;
      int w,h;
      int rd , gr , bl;
      int fval,cval;
      float wfac,zfac;
      int Fz =font;
      wc = D->wc;
      ln = width;
      if ( str == NULL ) return;
      if ( str [ 0 ] == '\0' ) return;
      wfac =1.0;
      cval = color;
      fval = font;
      old=0;
#if 0
      old =1;
      uiCleanOldString(str,Buf,&cval,&fval,&wfac,&zfac);
      kgGetDefaultRGB ( cval , & rd , & gr , & bl ) ;
      F.code = 'f';
      F.name = kgGetOthFont ( fval ) ;
      if(D->gc.MsgFont==font) F.Imgs = Mimgs;
      else F.Imgs = Pimgs;
      if ( FontSize <= 0 ) F.Size = ( height-4 ) /2;
      else F.Size = FontSize;
      if(F.Size >(( height-4 ) /2 ) ) F.Size = ( height-4 ) /2 ;
      if(F.Size != Fz) F.code='f';
      if(fval != font) F.code='f';
      if(wfac != 1.0 )F.code = 'f';
      if(F.Imgs==NULL) F.code='i';
      IMG = uiMakeString ( & ( F ) , Buf , ( int ) height , 0 ) ;
#if 1
          if(wfac != 1.0) {
            IMG->xln = IMG->xln*wfac;
            img = kgChangeSizeImage(IMG->img,IMG->xln,height*3/2);
            kgFreeImage(IMG->img);
            IMG->img = img;
          }
#endif
      kgSetImageColor ( IMG->img , rd , gr , bl ) ;
#else
      if(D->gc.MsgFont==font) F.Imgs = Mimgs;
      else F.Imgs = Pimgs;
      if ( FontSize <= 0 ) F.Size = ( height-4 ) /2;
      else F.Size = FontSize;
     IMG = (IMG_STR *)uiComplexString(str,F.Imgs,font,cval,F.Size,height);
//     if(F.Imgs == Mimgs ) IMG->img  = NULL;
#endif
      if ( bkcolor >= 0 ) {
          fid = kgInitImage ( ln , height , 1 ) ;
          kgBoxFill ( fid , 0. , 0. , ( float ) ln , ( float ) height , bkcolor , 0 ) ;
          imgbk = kgGetResizedImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      else imgbk = NULL;
         gimg =(GMIMG *)(IMG->img);
         w = gimg->image_width;
         h = gimg->image_height;
      if( (w > ln-FontSize)|| (h>height-2)) {
         float fac;
         if( h> height-2) h= height-2;
         if(w>(ln-FontSize))w = ln-FontSize;
         fac =(float) (ln-FontSize)/w ;
         img = kgChangeSizeImage(IMG->img,w,h);
//            img = kgResizeImage(IMG->img,fac);
//            kgFreeImage(IMG->img);
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
              if(old)kgAddImages ( imgbk , img , x1 , height/2-FontSize*1.2) ;
              else kgAddImages ( imgbk , img , x1 , 0) ; 
              kgImage ( D , imgbk , x , y , ln , height , 0.0 , 1.0 ) ;
              uiFreeImage ( imgbk ) ;
          }
          else {
              kgImage ( D , img , x+x1 , y , ln1 , (height) , 0.0 , 1.0 ) ;
          }
          uiFreeImage ( img ) ;
          free ( IMG ) ;
      }
      else printf ( "img == NULL\n" ) ;
  }
  void *uiStringToImage ( DIALOG *D , char *str , int x , int y , int width , int height , int font , int color , int FontSize , int justfic , int bkcolor ) \
  {
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    bkcolor : background color ; < 0 background will not be painted
*/
      char Buf[1000];
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
      int fval,cval;
      float wfac,zfac;
      int Ht=height;
      int Fz=FontSize;
      wc = D->wc;
      ln = width;
      if ( str == NULL ) return NULL;
      if ( str [ 0 ] == '\0' ) return NULL;
      wfac =1.0;
      cval = color;
      fval = font;
//      if(Ht> 2*FontSize+6) Ht =2*FontSize+6 ;
      if(bkcolor < 0) height =Ht;
//      printf("uiStringToImage: %s\n",str);
#if 0
      Str = uiCleanOldString(str,Buf,&cval,&fval,&wfac,&zfac);
      kgGetDefaultRGB ( cval , & rd , & gr , & bl ) ;
      F.code = 't';
      F.name = kgGetOthFont ( fval ) ;
      F.Imgs = Bimgs;
      if ( FontSize <= 0 ) F.Size = ( height-6 ) /2;
      else F.Size = FontSize;
      if(F.Size >(( height-6 ) /2 ) ) F.Size = ( height-6 ) /2 ;
//      F.Size = ( height-6 ) /2 ;
      if(F.Size != Fz )F.code='f';
      if(fval != font) F.code='f';
          if(wfac != 1.0 )F.code = 'f';
      if(F.Imgs==NULL) F.code='i';
      IMG = uiMakeString ( & ( F ) , Buf , ( int ) height , 0 ) ;
#if 1
          if(wfac != 1.0) {
            IMG->xln = IMG->xln*wfac;
            img = kgChangeSizeImage(IMG->img,IMG->xln,height);
            kgFreeImage(IMG->img);
            IMG->img = img;
          }
#endif
      kgSetImageColor ( IMG->img , rd , gr , bl ) ;
#else
      F.Imgs = Bimgs;
      if ( FontSize <= 0 ) F.Size = ( height-6 ) /2;
      else F.Size = FontSize;
      if(F.Size >(( height-6 ) /2 ) ) F.Size = ( height-6 ) /2 ;
     IMG = (IMG_STR *)uiComplexString(str,F.Imgs,font,cval,F.Size,height);
#endif
      if ( bkcolor >= 0 ) {
          fid = kgInitImage ( ln , height , 1 ) ;
          kgBoxFill ( fid , 0. , 0. , ( float ) ln , ( float ) height , bkcolor , 0 ) ;
          imgbk = kgGetResizedImage ( fid ) ;
          kgCloseImage ( fid ) ;
      }
      else imgbk = NULL;
      if( IMG->xln > ln-FontSize) {
         float fac;
        fac =(float) (ln-FontSize)/IMG->xln ;
        img = kgChangeSizeImage(IMG->img,ln-FontSize,(height));
            kgFreeImage(IMG->img);
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
              kgAddImages ( imgbk , img , x1 , height/2-FontSize) ;
              uiFreeImage ( img ) ;
              img =imgbk ; 
          }
          free ( IMG ) ;
      }
      else printf ( "img == NULL\n" ) ;
      return img;
  }
