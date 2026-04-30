  void * kgMakeFontImgNew ( char *filename , char *text , int Htt,int Wdd ,int Gap) {
      FT_Library library;
      FT_Face face;
      FT_GlyphSlot slot;
      FT_Matrix matrix; /* transformation matrix */
      FT_Vector pen; /* untransformed origin */
      FT_Error error;
      int Size = Htt;
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
      Wd = Wdd;;
      IMG_STR *Img;
      void *img;
      unsigned char *pixls=NULL;
      error = FT_Init_FreeType ( & library ) ; /* initialize library */
      error = FT_New_Face ( library , filename , 0 , & face ) ;
      error = FT_Set_Char_Size ( face ,Wd*64, Ht*64, 120 , 120 ) ;
           /* set character size */
      slot = face->glyph;
  /* set up matrix */
      matrix.xx = ( FT_Fixed ) ( 1*0x10000L ) ;
      matrix.xy = 0;
      matrix.yx = 0;
      matrix.yy = ( FT_Fixed ) ( 1*0x10000L ) ;
      HEIGHT =(int)( 2.0*Htt);
      target_height =(int)( HEIGHT );
      WIDTH = 2*Wdd*num_chars+4;
      pixls = ( unsigned char * ) calloc ( sizeof  \
          ( unsigned char ) , WIDTH*HEIGHT ) ;
  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
      pen.x = 0 * 64;
      pen.y = (int)( Wdd/2.0+0.5 ) * 64;
      for ( n = 0; n < num_chars; n++ ) {
          FT_Set_Transform ( face , & matrix , & pen ) ;
          error = FT_Load_Char ( face , text [ n ] , FT_LOAD_RENDER ) ;
          if ( error ) continue; /* ignore errors */

          draw_bitmap ( pixls , & slot->bitmap ,\
              slot->bitmap_left ,HEIGHT - slot->bitmap_top,WIDTH,HEIGHT ) ;
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
