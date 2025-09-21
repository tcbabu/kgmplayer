
#include <stdio.h>

int main(int argc ,char **argv){
if(argv[1]==NULL)  printf("Waiting for enter\n");
else printf("%s : waiting for enter\n",argv[1]);
  getchar();
  return 1;
}

