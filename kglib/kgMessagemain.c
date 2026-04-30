#include <stdio.h>
void *RunkgMessage(void *,void *);
int main(int argc,char **argv) {
// kgStartX(NULL) //can be used;
  if(argc > 1)RunkgMessage(NULL,argv[1]);
  else RunkgMessage(NULL,NULL);
//  kgCloseX(); //canbe used
  return 1;
}
