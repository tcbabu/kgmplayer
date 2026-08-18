#include <stdio.h>
void *Runkgedit(void *,void *);
int main(int argc,char *argv[]) {
// kgStartX(NULL) //can be used;
  Runkgedit(NULL,argv[1]);
//  kgCloseX(); //canbe used
  return 1;
}
