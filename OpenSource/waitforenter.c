
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc ,char **argv){
if(argc == 1) { printf("Waiting for enter\n");getchar();return 1;}
  char flname[100];
  strcpy(flname,getenv("KULINA"));
  strcat(flname,"/lib/");
  strcat(flname,argv[1]);
  FILE *fp= fopen(flname,"r");
   
  if(fp != NULL) {
    printf("Created %s\n",flname);
    fclose(fp);
    return 1;
  }
  printf("Failed to Create : %s\n",flname);
  printf("%s : waiting for enter\n",argv[1]);
  getchar();
  return 1;
}

