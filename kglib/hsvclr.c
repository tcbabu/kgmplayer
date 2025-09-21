#include <stdio.h>

int HSVtoRGB (float *r,float *g,float *b,float h,float,s,float v) {
   if(s==0) {
      *r = v; *g = v; *b = v;
   }
   else {
     int i; 
     float p,q,t,f;
     if (h==360.) h=0;
     h /=60;
     i = (int)h;
     f = h-i;
     p = v*(1-s);
     q = v*(1- (s*f));
     t = v*(1-(s*(1-f)));
     switch(i) {
       case 0: *r=v;*g=t;*b=p; break;
       case 1: *r=q;*g=v;*b=p; break;
       case 2: *r=p;*g=v;*b=t; break;
       case 3: *r=p;*g=q;*b=v; break;
       case 4: *r=t;*g=p;*b=v; break;
       case 5: *r=v;*g=p;*b=q; break;
     }
   }
}
/*
  h -(0-360)
  s  (0 -1)
  v  (0 - 1)
*/
int Chng_hsv_clr (int clr,float h,float,s,float v) {
   floar r,g,b;
   if(s==0) {
      r = v; g = v; b = v;
   }
   else {
     int i; 
     float p,q,t,f;
     if (h==360.) h=0;
     h /=60;
     i = (int)h;
     f = h-i;
     p = v*(1-s);
     q = v*(1- (s*f));
     t = v*(1-(s*(1-f)));
     switch(i) {
       case 0: r=v;g=t;b=p; break;
       case 1: r=q;g=v;b=p; break;
       case 2: r=p;g=v;b=t; break;
       case 3: r=p;g=q;b=v; break;
       case 4: r=t;g=p;b=v; break;
       case 5: r=v;g=p;b=q; break;
     }
   }
   Chng_clr(clr,r,b,g);
}
