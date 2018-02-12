#include <stdio.h>
#include <math.h>
int rgb2hsl(int r,int g,int b,int *h,int *s,int *l) {
/*
   r,g,b range is 0-255
   h range 0-360
   s 0-100
   l 0-100
*/
  double pi=3.14159265;
 
  *h = (int)atan((2.0*r -g -b)/sqrt((double)(3.*(g - b))))/(2*pi);
  *s =(int) sqrt((double)(r*r+g*g+b*b -r*g -r*b - b*g))/2.550;
  *l = (int)(r+g+b)/7.650;
   return 1;
}
int main(void) {
  int h,s,l;
  rgb2hsl(255,0,0,&h,&s,&l);
  printf("%d %d %d\n",h,s,l);
  return 1;
}
