#include <stdlib.h>
#include<stdio.h>
#include <string.h>
int main(int argc,char **argv){
   int i;
   char command[200],outfile[200],out1[200];
   FILE *fp;
   if(argc < 2) {
	   printf("Usage: gph2eps <gphfile> \n");
           printf("              or                 \n ");
	   printf("       gph2eps <ghpfile> <epsfile>\n");
	   exit(0);
   }
   if(argc > 2) strcpy(outfile,argv[2]);
   else {
	   strcpy(outfile,argv[1]);
	   i=0; while((outfile[i]!='.')&&(outfile[i]>' '))i++;
	   outfile[i]='\0';
	   strcpy(out1,outfile);
	   strcat(outfile,".ps");
	   strcat(out1,".eps");
   }
   fp = fopen("JunK","w");
   fprintf(fp,"$o0\n");
   fprintf(fp,"$SL-5\n");
   fprintf(fp,"$m208\n");
   fprintf(fp,"$RG%-s 208 166\n",argv[1]);
   fclose(fp);
   sprintf(command,"slide -c -f%-s JunK",outfile);
   system(command);
   sprintf(command,"ps2epsi %-s %-s",outfile,out1);
   system(command);
   remove("JunK");
}
