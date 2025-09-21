#include<stdio.h>
char buf[2000];
void main(int argc,char **argv){
  FILE *fp,*op;
  int i;
  if( argc<2){
   printf("Usage: cleanxpm <in-file> \n");
   exit(0);
  }
  fp = fopen(argv[1],"r");
  if(fp ==NULL) {
     printf("Error: in input file\n");
   exit(0);
  }
  op = fopen("junk","w");
  while(fgets(buf,1900,fp) != NULL) {
   fprintf(op,"%s",buf);
   i=0; while((buf[i]==' ')&&(buf[i]!='\0'))i++;
   if(buf[i]=='}') break;
  }
  fclose(fp);
  fclose(op);
  sprintf(buf,"mv junk %s",argv[1]);
  system(buf);
  
}
