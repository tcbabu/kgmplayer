#include <stdio.h>
#include <kulina.h>

static char *GetNextString(char *str,int *pos) {
  int i=0,j,k,l,len;
  char *pt=NULL;
  i = *pos;
  k = *pos;
  j = 0;
  len = strlen(str);
  if( str[i]!='\0') {
    while(str[i]>= ' ') {i++; j++;}
    pt = (char *)malloc(j+2);
    *pos=i+1;
    if(*pos>len) *pos=len;
    l=0;
    while(l<=j) pt[l++]=str[k++];
    pt[l]='\0';
  }
  return pt;
  
}
void * kgCreateHelpImage (char *str) {
  int i=0;
  int ln=630,wid=270;
  float xp,yp,lnh=14.0;
  DIG *G=NULL;
  void *Img;
  int pos =0;
  char *pt;

  G = (DIG *)kgInitImage(ln,wid,8);
  kgUserFrame(G,0.,0.,(float)ln,(float)wid);
  kgTextSize(G,lnh,lnh*0.8,1.0);
  kgTextFont(G,1);
  kgTextColor(G,0);
  kgChangeColor(G,1001,225,225,225);
  kgRoundedRectangleFill(G,315.0,135.,608,218,0,1001,0.05);
  kgRoundedRectangleRaised(G,315.0,135.,610,220,1001,0.05,1.0);
  xp = 40;
  yp = wid-3*lnh;
  kgMove2f(G,xp,yp);
  while((pt=GetNextString(str,&pos))!= NULL) {
    kgWriteText(G,pt);
    yp = yp - lnh*1.5;
    kgMove2f(G,xp,yp);
  }
//  Img = kgGetResizedImage(G);
  Img = kgGetSharpImage(G);
//  Img = kgGetSmoothImage(G);
  kgWriteImage(Img,(char *)"junk.png");
  kgCloseImage(G);
  return Img;
}
