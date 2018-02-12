#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "kulina.h"

int main(int argc,char **argv){
   void *img;
   void *fid;
   int xsize,ysize;
   if(argc < 5) {
	   printf("Usage: gph2png  <gphfile> <pngfile) <xsize> <ysize>\n");
	   exit(0);
   }
   sscanf(argv[3],"%d",&xsize);
   sscanf(argv[4],"%d",&ysize);
#if 0
   fid = kgInitImage(xsize,ysize,4);
   kgImportGphFile((DIG *)fid,argv[1],0.0,0.0,(float)xsize,(float)ysize);
   img = kgGetResizedImage(fid);
   kgWriteImage(img,argv[2]);
   kgCloseImage(fid);
#else
   printf("xsize : %d : %d\n",xsize,ysize);
   img = kgGphtoImage(argv[1],xsize,ysize,0x00000000);
   kgWriteImage(img,argv[2]);
   kgFreeImage(img);
#endif
}
