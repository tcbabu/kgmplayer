#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kulina.h"
#include <png.h>
extern char SharedPicDir[500];
extern char SystemPicDir[500];
typedef unsigned char uch;
typedef unsigned short  ush;
typedef unsigned long   ulg;



/* future versions of libpng will provide this macro: */
#ifndef png_jmpbuf
#  define png_jmpbuf(png_ptr)   ((png_ptr)->jmpbuf)
#endif


static png_structp png_ptr = NULL;
static png_infop info_ptr = NULL;

png_uint_32  width, height;
int  bit_depth, color_type;
uch  *image_data = NULL;


int CheckPng(unsigned char *sig) {
    if (png_sig_cmp(sig, 0, 8)) return 0;   /* bad signature */
    else return 1;
}
int kgCheckPngFile(char *file) {
    FILE *fp;
    uch sig[8];
    fp=fopen(file,"r");
    if (fp== NULL) return 0;
    fread(sig, 1, 8, fp);
    fclose(fp);
    if (png_sig_cmp(sig, 0, 8)) return 0;   /* bad signature */
    else return 1;
}

/* return value = 0 for success, 1 for bad sig, 2 for bad IHDR, 4 for no mem */

int readpng_init(FILE *infile, ulg *pWidth, ulg *pHeight)
{
    uch sig[8];


    /* first do a quick check that the file really is a PNG image; could
     * have used slightly more general png_sig_cmp() function instead */

    fread(sig, 1, 8, infile);
    if (png_sig_cmp(sig, 0, 8))
        return 1;   /* bad signature */


    /* could pass pointers to user-defined error handlers instead of NULLs: */

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
        return 4;   /* out of memory */

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 4;   /* out of memory */
    }


    /* we could create a second info struct here (end_info), but it's only
     * useful if we want to keep pre- and post-IDAT chunk info separated
     * (mainly for PNG-aware image editors and converters) */


    /* setjmp() must be called in every function that calls a PNG-reading
     * libpng function */

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 2;
    }


    png_init_io(png_ptr, infile);
    png_set_sig_bytes(png_ptr, 8);  /* we already read the 8 signature bytes */

    png_read_info(png_ptr, info_ptr);  /* read all PNG info up to image data */


    /* alternatively, could make separate calls to png_get_image_width(),
     * etc., but want bit_depth and color_type for later [don't care about
     * compression_type and filter_type => NULLs] */

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
      NULL, NULL, NULL);
    *pWidth = width;
    *pHeight = height;


    /* OK, that's all we need for now; return happy */

    return 0;
}




/* returns 0 if succeeds, 1 if fails due to no bKGD chunk, 2 if libpng error;
 * scales values to 8-bit if necessary */

int readpng_get_bgcolor(uch *red, uch *green, uch *blue)
{
    png_color_16p pBackground;


    /* setjmp() must be called in every function that calls a PNG-reading
     * libpng function */

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 2;
    }


    if (!png_get_valid(png_ptr, info_ptr, PNG_INFO_bKGD))
        return 1;

    /* it is not obvious from the libpng documentation, but this function
     * takes a pointer to a pointer, and it always returns valid red, green
     * and blue values, regardless of color_type: */

    png_get_bKGD(png_ptr, info_ptr, &pBackground);


    /* however, it always returns the raw bKGD data, regardless of any
     * bit-depth transformations, so check depth and adjust if necessary */

    if (bit_depth == 16) {
        *red   = pBackground->red   >> 8;
        *green = pBackground->green >> 8;
        *blue  = pBackground->blue  >> 8;
    } else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        if (bit_depth == 1)
            *red = *green = *blue = pBackground->gray? 255 : 0;
        else if (bit_depth == 2)
            *red = *green = *blue = (255/3) * pBackground->gray;
        else /* bit_depth == 4 */
            *red = *green = *blue = (255/15) * pBackground->gray;
    } else {
        *red   = (uch)pBackground->red;
        *green = (uch)pBackground->green;
        *blue  = (uch)pBackground->blue;
    }

    return 0;
}




/* display_exponent == LUT_exponent * CRT_exponent */

uch *readpng_get_image(double display_exponent, int *pChannels, ulg *pRowbytes)
{
    double  gamma;
    png_uint_32  i, rowbytes;
    png_bytepp  row_pointers = NULL;


    /* setjmp() must be called in every function that calls a PNG-reading
     * libpng function */

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }


    /* expand palette images to RGB, low-bit-depth grayscale images to 8 bits,
     * transparency chunks to full alpha channel; strip 16-bit-per-sample
     * images to 8 bits per sample; and convert grayscale to RGB[A] */

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_expand(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_expand(png_ptr);
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);


    /* unlike the example in the libpng documentation, we have *no* idea where
     * this file may have come from--so if it doesn't have a file gamma, don't
     * do any correction ("do no harm") */

    if (png_get_gAMA(png_ptr, info_ptr, &gamma))
        png_set_gamma(png_ptr, display_exponent, gamma);


    /* all transformations have been registered; now update info_ptr data,
     * get rowbytes and channels, and allocate image memory */

    png_read_update_info(png_ptr, info_ptr);

    *pRowbytes = rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    *pChannels = (int)png_get_channels(png_ptr, info_ptr);

    if ((image_data = (uch *)malloc(rowbytes*height)) == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }
    if ((row_pointers = (png_bytepp)malloc(height*sizeof(png_bytep))) == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        free(image_data);
        image_data = NULL;
        return NULL;
    }



    /* set the individual row_pointers to point at the correct offsets */

    for (i = 0;  i < height;  ++i)
        row_pointers[i] = image_data + i*rowbytes;


    /* now we can go ahead and just read the whole image */

    png_read_image(png_ptr, row_pointers);


    /* and we're done!  (png_read_end() can be omitted if no processing of
     * post-IDAT text/time/etc. is desired) */

    free(row_pointers);
    row_pointers = NULL;

    png_read_end(png_ptr, NULL);

    return image_data;
}


void readpng_cleanup(int free_image_data)
{
    if (free_image_data && image_data) {
        free(image_data);
        image_data = NULL;
    }

    if (png_ptr && info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        png_ptr = NULL;
        info_ptr = NULL;
    }
}
void *ReadPngImage(char *flname) {
  FILE *infile;
  PNGIMG *png;
  double display_exponent=3.2;
  unsigned char *image_data;
  ulg image_width,image_height,image_rowbytes;
  int rc,image_channels;
  char sharedfilename[300];
  if (!(infile = fopen(flname, "rb"))){
      strcpy(sharedfilename,SharedPicDir);
      strcat(sharedfilename,"/");
      strcat(sharedfilename,flname);
      if (!(infile = fopen(sharedfilename, "rb"))){
        strcpy(sharedfilename,SystemPicDir);
        strcat(sharedfilename,"/");
        strcat(sharedfilename,flname);
        if (!(infile = fopen(sharedfilename, "rb"))){
         return NULL;
        }
      }
  }
  if ((rc = readpng_init(infile, &image_width, &image_height)) != 0) {
    fclose(infile);
    return NULL;
  }
  image_data = readpng_get_image(display_exponent, &image_channels,
      &image_rowbytes);
//  printf("%d %d %d %d\n",image_width ,image_height ,image_channels,image_rowbytes);
  readpng_cleanup(0);
  fclose(infile);
  png = (PNGIMG *)malloc(sizeof(PNGIMG));
  png->image_width = image_width;
  png->image_height = image_height;
  png->image_channels=image_channels;
  png->image_rowbytes=image_rowbytes;
  png->image_data=image_data;
  strcpy(png->Sign,"PNG");
  png->incode=0;
  return png;
}
#if 0
int main(int argc,char **argv) {
  if(ReadPngImage(argv[1])){
    printf("Read PNG File\n");
  }
  else {
    printf("Failed PNG File\n");
  }
  return 1;
}
#endif
