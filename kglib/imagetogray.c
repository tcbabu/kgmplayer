void *kgImagetoGray(void *img) {

/*
 Converts the Image to Gray scale
  
*/
  int i,j,k=0,kk;
  GMIMG *png=NULL;
  int gray=0;
  
  Image *image;
  PixelPacket *spixels;
  unsigned int red,green,blue;
  png = (GMIMG *)img;
  if(png== NULL) return NULL;
  image = (Image *)(png->image);
  uiInitGm();
  spixels =GetImagePixels(image,0,0,image->columns,image->rows);
  xsize = image->columns;
  ysize = image->rows;
  k=0;
  for(j=0;j<(ysize);j++) {
    for(i=0;i<(xsize);i++) {
       blue = pixels[k].blue;
       green = pixels[k].green;
       red = pixels[k].red;
       gray = 0.299*red+0.587*green+0.114*blue;
       spixels[kk].blue = grey;
       spixels[kk].green = gray;
       spixels[kk].red = gray;
       alpha += alphas;
       k++;
    }
  }
  SyncImagePixels(image);
  return png;
}
