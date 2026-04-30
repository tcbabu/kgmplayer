#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <sys/stat.h>

static int FileStat(char *flname) {
  int ret;
  struct stat buff;
  ret = stat(flname,&buff);
  if(ret < 0) return 0;
  else return 1;
}
int MakeNewFileName(char *Infile,char *OutFile) {
   int index,i;
   char buff[500],*pt,*ext=NULL;
   int id=0;
   strcpy(buff,Infile);
   i=strlen(Infile)-1;
   while (buff[i]!='.') {
     if(buff[i]< ' ') break;
     if(buff[i]==' ') buff[i]='_';
     if(i<= 0) break;
     i--;
   }
   ext = Infile+i+1;
   buff[i]='\0';
   pt= buff+i;
   while(1) {
     sprintf(pt,"_%-4.4d.%-s",id,ext);
     if (!FileStat(buff)) break;
     id++;
   }
//   OutFile= (char *)malloc(strlen(buff)+1);
   strcpy(OutFile,buff);
   printf("New File = %s\n",OutFile);
   return 1;
}
int main(int argc,char **argv) {
    printf("Outfile: %s\n",  MakeNewFileName(argv[1])); 
    return 1;
}
