#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dlink.h"
#include "kulina.h"
int comppath(void *pt1,void *pt2) {
  return strcmp((char *)pt1,(char *)pt2);
}
int compdup(void *pt1,void *pt2) {
  int ret;
  ret=strcmp((char *)pt1,(char *)pt2);
  if(ret==0) return 1;
  else return 0;
}
char *kgWhich(char *pgr) {
  int i=0,j,End=0;
  char *pt,**m,*res=NULL;
  char path[5000];
  Dlink *L;
  pt = getenv("PATH");
  if(pt==NULL) return NULL;
  strcpy(path , getenv("PATH"));
  L = Dopen();
  while(!End) {
    j=i;
    if(pt[j]< ' ') break;
    while(path[j]!=':') {
      if(path[j]<' ') {End=1;break;}
      j++;
    }
    path[j]='\0';
    Dadd(L,path+i);
    i=j+1;
  }
  Dsort(L,comppath);
  Drmvdup_cond(L,compdup);
  Resetlink(L);
  while( (pt = (char *)Getrecord(L))!=NULL) {
//    printf("%s\n",pt);
    m = kgFileMenu(pt,pgr);
    if(m==NULL) continue;
    if(m[0]==NULL) { free(m);continue;}
    res = (char *)malloc(strlen(pt)+1+strlen(m[0])+1);
    strcpy(res,pt);
    strcat(res,"/");
    strcat(res,m[0]);
    i=0;
    while(m[i]!=NULL) {free(m[i]);i++;}
    free(m);
    break;
  }
  Dfree(L);
  return res;
}
#if 0
int main(int argc,char **argv) {
  char *res;
  res = which(argv[1]);
  if(res!=NULL) printf("%s\n",res);
  else printf("res=NULL\n");
}
#endif
