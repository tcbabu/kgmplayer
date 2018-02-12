#include "gl.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#if 0
#include <magick/api.h>


`
int xres,yres;
Image *image,*resize_image;
ImageInfo *image_info;
ExceptionInfo exception;
PixelPacket *pixels;
PNGIMG *png;
void *uiGetImage(char *flname) {
  Image *image;
  ImageInfo *image_info;
  ExceptionInfo exception;
  PixelPacket *pixels;
  PNGIMG *png=NULL;
  InitializeMagick(flname);
  GetExceptionInfo(&exception);
  image_info=CloneImageInfo((ImageInfo *) NULL);
  (void) strcpy(image_info->filename,flname);
  image=ReadImage(image_info,&exception);
  if (exception.severity != UndefinedException) CatchException(&exception);
  pixels =GetImagePixels(image,0,0,image->columns,image->rows);
  if(pixels==NULL) { 
     png = NULL;
     DestroyImage(image);
  }
  else {
    int i=0,j=0;
    unsigned long *pt;
    png = (PNGIMG *)malloc(sizeof(PNGIMG));
    png->image = image;
    strcpy(png->Sign,"PNG");
    png->incode=0;
    png->image_width= image->columns;
    png->image_height= image->rows;
    png->image_rowbytes = png->image_width*4;
    png->image_channels=4;
    png->image_data = (unsigned char *)malloc(png->image_width*(png->image_height+1)*4);
    for(i=0;i<(image->columns*image->rows);i++) {
     png->image_data[j++]= pixels[i].red;
     png->image_data[j++]= pixels[i].green;
     png->image_data[j++]= pixels[i].blue;
     png->image_data[j++]= 255 - pixels[i].opacity;
    }
  }
  DestroyImageInfo(image_info);
  DestroyExceptionInfo(&exception);
  DestroyMagick();
  return png;
}
void *uiResizeImage(PNGIMG *png,float fac) {
  int w,h;
  Image *image,*resize_image=NULL;
  ImageInfo *image_info;
  ExceptionInfo exception;
  PixelPacket *pixels;
  if(png== NULL) return NULL;
  image = (Image *)(png->image);
  InitializeMagick("resize");
  GetExceptionInfo(&exception);
  w = image->columns*fac;
  h = image->rows*fac;
  resize_image=ResizeImage(image,w,h,LanczosFilter,1.0,&exception);
  if (exception.severity != UndefinedException) CatchException(&exception);
  pixels =GetImagePixels(resize_image,0,0,
          resize_image->columns,resize_image->rows);
  if(pixels==NULL) { 
     png = NULL;
     DestroyImage(resize_image);
  }
  else {
    int i=0,j=0;
    unsigned long *pt;
    png = (PNGIMG *)malloc(sizeof(PNGIMG));
    png->image = resize_image;
    strcpy(png->Sign,"PNG");
    png->incode=0;
    png->image_width= resize_image->columns;
    png->image_height= resize_image->rows;
    png->image_rowbytes = png->image_width*4;
    png->image_channels=4;
    png->image_data = (unsigned char *)malloc(png->image_width*(png->image_height+1)*4);
    for(i=0;i<(resize_image->columns*resize_image->rows);i++) {
     png->image_data[j++]= pixels[i].red;
     png->image_data[j++]= pixels[i].green;
     png->image_data[j++]= pixels[i].blue;
     png->image_data[j++]= 255 - pixels[i].opacity;
    }
  }
//  DestroyImageInfo(image_info);
  DestroyExceptionInfo(&exception);
  DestroyMagick();
  return png;
}
void uiWriteImage(PNGIMG *png,char *flname) {
  int w,h;
  Image *image,*resize_image=NULL;
  ImageInfo *image_info;
  ExceptionInfo exception;
  if(png== NULL) return ;
  image = (Image *)(png->image);
  InitializeMagick("writepng");
  GetExceptionInfo(&exception);
  image_info=CloneImageInfo((ImageInfo *) NULL);
//  (void) strcpy(image_info->filename,flname);
  (void) strcpy(image->filename,flname);
//  printf("WriteImage\n");
  WriteImage(image_info,(Image *)(png->image));
  DestroyImageInfo(image_info);
  DestroyExceptionInfo(&exception);
  DestroyMagick();
  return ;
}
void uiFreeGmImage(PNGIMG *png) {
  InitializeMagick("FreeGmImage");
  DestroyImage((Image *)(png->image));
  DestroyMagick();
}
#endif
int main(int argc,char **argv) {
  char flname[200];
  PNGIMG *png;
  int xres,yres;
#if 0
  InitializeMagick(*argv);
  GetExceptionInfo(&exception);
  image_info=CloneImageInfo((ImageInfo *) NULL);
  (void) strcpy(image_info->filename,argv[1]);
   printf("Reading %s ...", image_info->filename);
  image=ReadImage(image_info,&exception);
  printf(" %lu frames %lu columns %lu rows \n", GetImageListLength(image),image->columns,image->rows);
  if (exception.severity != UndefinedException) CatchException(&exception);
  resize_image=ResizeImage(image,106,80,LanczosFilter,1.0,&exception);
  pixels =GetImagePixels(resize_image,0,0,106,80);
  if(pixels==NULL) { printf("Failed to get pixels\n"); }
  else {
    int i=0,j=0;
    unsigned long *pt;
    png = (PNGIMG *)malloc(sizeof(PNGIMG));
    strcpy(png->Sign,"PNG");
    png->incode=1;
    png->image_width= resize_image->columns;
    png->image_height= resize_image->rows;
    png->image_rowbytes = png->image_width*4;
    png->image_channels=4;
    png->image_data = (unsigned char *)malloc(png->image_width*(png->image_height+1)*4);
    for(i=0;i<(resize_image->columns*resize_image->rows);i++) {
//      printf("%x %x %x\n",pixels[i].red,pixels[i].green,pixels[i].blue);
     png->image_data[j++]= pixels[i].red;
     png->image_data[j++]= pixels[i].green;
     png->image_data[j++]= pixels[i].blue;
//     png->image_data[j++]= 255;
     png->image_data[j++]= 255 - pixels[i].opacity;
    }
  }
  DestroyImage(image);
  if (resize_image == (Image *) NULL) {
          CatchException(&exception);
  }
  WriteImage(image_info,resize_image);
  DestroyImageInfo(image_info);
  DestroyExceptionInfo(&exception);
  DestroyMagick();
#endif

  GetDisplaySize(&xres,&yres);
  strcpy(flname,"##");
  strcat(flname,argv[1]);
  printf("%s\n",flname);
#if 1
  initfullscreen();
//  while(_uiCheckEscape()!= 1);
  jpg_image(flname,0,0,xres,yres);
  UpdateOn();
  while(_uiCheckEscape()!= 1);
  png = uiGetImage(flname+2);
  jpg_image((void *)png,0,0,xres,yres);
  UpdateOn();
  while(_uiCheckEscape()!= 1);
  png = uiResizeImage(png,1.0/2.5);
  if(png != NULL) jpg_image(png,0,0,xres,yres);
  else printf("Png == NULL\n");
  UpdateOn();
  while(_uiCheckEscape()!= 1);
  uiWriteImage(png,"Junk.jpg");
  uiFreeGmImage(png);
  setnormal();

#endif
}
