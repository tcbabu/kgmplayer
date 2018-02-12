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
 while( fgets(buff,4999,fp1) != NULL) fprintf(fp,"%s",buff);
 fclose(fp);
 fclose(fp1);
 fclose(fp2);
 return 1;
}
int main(int argc,char **argv){
   int i,res=300,count=2,pos=0;
   char command[200],outfile[200];
   FILE *fp;
   if(argc < 2) {
	   printf("Usage: ps2jpg [-r res] <psfile> \n");
           printf("               or                 \n ");
	   printf("       ps2jpg [-r res] <psfile> <jpgfile>\n");
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
   sprintf(command,"gs -sDEVICE=jpeg  -r%-d -dBATCH -dNOPAUSE -sOutputFile=%s %s >/dev/null",res,outfile,argv[count-1]);
   system(command);
}
