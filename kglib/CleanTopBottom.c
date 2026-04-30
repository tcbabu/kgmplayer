
int   kgCleanImageTopBottom ( void * img int *top,int *bottm ) {
      int k,i,j,Topskip=0,Bottomskip=0;
      unsigned long v , xsize , ysize;
      Image *image , *tmpimg;
      PixelPacket *pixels;
      GMIMG *png = NULL;
      png = ( GMIMG * ) img;
      *top=0;
      *bottm =0;
      if(img==NULl) return 0;
      uiInitGm ( ) ;
      image = png->image;
      pixels = GetImagePixels ( image , 0 , 0 , image->columns , image->rows ) ;
      xsize = image->columns;
      ysize = image->rows;
      
      k = 0;
      Topskip =0;
      Bottomskip =0;
      for ( j = 0;j < ysize;j++ ) {
          count =0;
          for ( i = 0;i < xsize;i++ ) {
              if(pixels [ k ] .blue != 0) break;
              if(pixels [ k ] .green != 0) break;
              if(pixels [ k ] .red != 0) break;
              Topskip++;
              k++;
              count++;
          }
          if(count!=xsize) break;
      }
      if( Topskip != ysize) {
        k = xsize*ysize -1;
        for ( j = ysize -1;j >=0;j-- ) {
          count = xsize;
          for ( i =xsize-1;i >= 0;i-- ) {
              if(pixels [ k ] .blue != 0) break;
              if(pixels [ k ] .green != 0) break;
              if(pixels [ k ] .red != 0) break;
              Bottomskip++;
              k--;
          }
          if(k!=0) break;
        }
      }
      *top=Topskip;
      *bottom=Bottomskip;
      return 1;      
}
