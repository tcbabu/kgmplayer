#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

int SetBoundBox(void) {
 char buff[5000];
 FILE *fp,*fp1,*fp2;
 fp = fopen(".JunK","w");
 if (fp==NULL) {
    printf("Error: Failed to open .JunK to write\n");
    exit(0);
 }
 fp1 = fopen(".JunK1","r");
 if (fp1==NULL) {
    printf("Error: Failed to open .JunK1 for reading\n");
    exit(0);
 }
 fp2 = fopen(".JunK2","r");
 if (fp2==NULL) {
    printf("Error: Failed to open .JunK2 for reading\n");
    exit(0);
 }
 fgets(buff,4999,fp1);
 fprintf(fp,"%s",buff);
 fgets(buff,4999,fp1);
 fprintf(fp,"%s",buff);
 fgets(buff,4999,fp1);
 fprintf(fp,"%s",buff);
 fgets(buff,4999,fp2);
 fprintf(fp,"%s",buff);
 fgets(buff,4999,fp2);
 fprintf(fp,"%s",buff);
 fgets(buff,4999,fp1);
 fprintf(fp,"%s",buff);
 fprintf(fp,"%%%%BeginSetup \n");
 fprintf(fp,"%%%%BeginFeature: *PageSize A4 \n");
 // fprintf(fp,"<</PageSize [620 560] /ImagingBBox null >> setpagedevice \n");
 fprintf(fp,"<</Orientation 3>> setpagedevice \n");

 fprintf(fp,"%%%%EndFeature \n");
 fprintf(fp,"%%%%EndSetup \n");
 while( fgets(buff,4999,fp1) != NULL) fprintf(fp,"%s",buff);
 fclose(fp);
 fclose(fp1);
 fclose(fp2);
 return 1;
}
int main(int argc,char **argv){
   int i,res=148,count=2,pos=0;
   char command[200],outfile[200];
   FILE *fp;
   if(argc < 2) {
	   printf("Usage: gph2jpg [-r res] <gphfile> \n");
           printf("               or                 \n ");
	   printf("       gph2jpg [-r res] <ghpfile> <jpgfile>\n");
	   exit(0);
   }
   if(getopt(argc,argv,"r:")>0) {
     sscanf(optarg,"%d",&res);
     count = optind+1;
     pos = optind;
   }
   if(argc > count) strcpy(outfile,argv[count]);
   else {
	   strcpy(outfile,argv[count-1]);
	   i=0; while((outfile[i]!='.')&&(outfile[i]>' '))i++;
	   outfile[i]='\0';
	   strcat(outfile,".jpg");
   }
   fp = fopen(".JunK","w");
   fprintf(fp,"$o0\n");
   fprintf(fp,"$LS5\n$m380\n");
   fprintf(fp,"$RG%-s 360 270 \n",argv[count-1]);
   fclose(fp);
   sprintf(command,"slide -c -f.JunK1 .JunK");
   system(command);
#if 0
   system("cp .JunK JunK");
   system("mv .JunK1 .JunK");
#else
   sprintf(command,"gs -sDEVICE=bbox  -dBATCH -dNOPAUSE  .JunK1 2>.JunK2>/dev/null");
   system(command);
   SetBoundBox();
#endif
   sprintf(command,"gs -sDEVICE=jpeg  -r%-d  -dBATCH -dNOPAUSE -sOutputFile=%s .JunK >/dev/null",res,outfile);
   system(command);
#if 1
   remove(".JunK");
   remove(".JunK1");
   remove(".JunK2");
#endif
}
