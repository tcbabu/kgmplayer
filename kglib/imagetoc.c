#include <stdio.h>
#include "kulina.h"

void main(int argc,char **argv) {
   if(argc>1) {
#if 0
     if(uiWriteJpg(argv[1]) != NULL) {
       printf("Made C code for %s\n",argv[1]);
     }
     else {
       if(uiWritePng(argv[1]) != NULL) {
         printf("Made C code for %s\n",argv[1]);
       }
       else printf("Failed\n");
     }
#else
       if(uiWriteFmg(argv[1]) != 1) {
         printf("Made C code for %s\n",argv[1]);
       }
#endif
   }
}
