#ifndef GL_H
#define GL_H
#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "kulina.h"
#include "gprivate.h"

#ifdef __cplusplus
extern "C"
{
#endif

void *uiWriteJpg(char *jpgfile);
void *uiWritePng(char *pngfile);
void uiFreeImage(void *tmp);
void *uiGetImage(char *flname);
void *uiGetgmImage(char *flname);
void *uiResizegmImage(GMIMG *png,float fac);
void *uiScalegmImage(GMIMG *png,float fac);
void uiWritegmImage(GMIMG *png,char *flname);
void *uiPixelsgmImage(GMIMG *png);
void *uiRotategmImage(GMIMG *png,float angle) ;
void *uiAppendgmImage(GMIMG *png1,GMIMG *png2);
void jpg_image_transparent(void *tmp,int x0,int y0,int width,int height,float transparency);
void uiFreeFmgImage(void *png);
int uiFmgToFile(unsigned char *array,int sz,char * flname);
int uiExtractBaseName (char *flname,char *basename) ;
int uiWriteFmg(char *infile);
int fmg_image_transparent(void *tmp,int x0,int y0,int width,int height,float transparency);
void *uiCreategmImage(unsigned long *data,unsigned long xsize,unsigned long ysize);
void *uiChangeSizegmImage(GMIMG *png,unsigned long w,unsigned long h);
void *uiHalfSizegmImage(GMIMG *png);
void *uiCopyPixelsgmImage(void * img,unsigned long *data);
void *uiCreateCleangmImage(unsigned long xsize,unsigned long ysize,int bkred,int bkgreen,int bkblue,int opacity);
void *uiGetAntialiasedImage(char *flname,int xsize,int ysize,void *lights,int Mag); // background is zero
void *uiThumbnailgmImage(GMIMG *png,unsigned long w,unsigned long h);
#ifdef __cplusplus
}
#endif
#endif /* enf of gl.h */ 
