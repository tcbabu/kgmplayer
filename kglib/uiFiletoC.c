#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "gl.h"
#include "Up32.c"
#if 0
int uiFmgToFile(unsigned char *array,int sz,char * flname) {
  int ot;
  ot = open(flname,O_RDWR|O_CREAT|O_TRUNC,0777);
  if(ot < 0) return -1;
  write(ot,array,sz);
  close(ot);
  return 1;
}
int uiExtractBaseName (char *flname,char *basename) {
  int i,j;
  i=0;
  i = strlen(flname);
  i--;
  while(flname[i]!= '/') {
     i--;
     if(i<0 ) break;
  }
  i++;
  j=0;
  while( (flname[i]!='.')&&(flname[i]>' '))basename[j++] = flname[i++];
  basename[j]='\0';
}
int uiWriteFmg(char *infile) {
  int inp,i,ret,sum=0;
  FILE *fp;
  unsigned char buff[64];
  char flname[100],basename[100];
  inp = open(infile,O_RDONLY);
  if(inp < 0) return 2;
  uiExtractBaseName(infile,basename);
  strcpy(flname,basename);
//  sprintf(flname,"%-s.c",infile);
  strcat(flname,".c");
  fp = fopen(flname,"w");
  if(fp == NULL) return 3;
  fprintf(fp,"  char %-s_data[]= {\n",basename);
  while( (ret=read(inp,buff,16))==16) {
     sum +=ret;
     fprintf(fp,"        ");
     for(i=0;i<ret;i++) fprintf(fp,"0x%2.2x,",buff[i]);
     fprintf(fp,"\n");
  }
  sum +=ret;
  fprintf(fp,"        ");
  for(i=0;i<ret;i++) fprintf(fp,"0x%2.2x,",buff[i]);
  fprintf(fp,"0xff\n  };\n");
  fprintf(fp,"  FMGIMG %-s_str = { \"FMG\",1,\"%-s\",%d,%-s_data,NULL};\n",basename,basename,sum,basename);
  fclose(fp);
  close(inp);
  return 0;
}
#endif
int main(int argc,char **argv) {
  if(argc < 2) {
     fprintf(stderr," Usage: %s <file_name>\n",argv[0]);
     fprintf(stderr,"Converts file to char array file which be include in C code\n");
     fprintf(stderr,"This file can be copied to a tmp file and can be read\n");
     return 1;
  }
//  return uiWriteFmg(argv[1]);
  uiFmgToFile(Up32_str.image_data,Up32_str.size,"Junk.png");
}
