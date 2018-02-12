int HLStoRGB(float *r,float *g,float *b,float h, float l,float s);
int HSVtoRBG(float *r,float *g,float *b,float h,float s,float v);
int RGBtoHSV(float r,float g,float b,float *h,float *s,float *v);
int RGBtoHLS(float r,float g, float b,float *h,float *l,float *s);

int HSVtoRBG(float *r,float *g,float *b,float h,float s,float v) {
/*
  range r: 0-1
  range g: 0-1
  range b: 0-1
        h: 0-360
        s: 0-1
        v: 0-1
*/
  int i;
  float f,p,q,t;

  if(s==0) *r=v,*g=v,*b=v;
  else {
    while(h >= 360.) h -=360;
    h = h/60.0;
    i = (int) h;
    f = h-i;
    p = v*(1-s);
    q = v*(1-s*f);
    t = v*(1-(s*(1-f)));
    switch (i){
      case 0: *r=v,*g=t,*b=p; break;
      case 1: *r=q,*g=v,*b=p; break;
      case 2: *r=p,*g=v,*b=t; break;
      case 3: *r=p,*g=q,*b=v; break;
      case 4: *r=t,*g=p,*b=v; break;
      case 5: *r=v,*g=p,*b=q; break;
    }
  }
  return 1;
}
float Maxrgb(float r,float g,float b) {
  float max;
  max = r;
  if(g>max) max =g;
  if(b>max) max =b;
  return max;
}
float Minrgb(float r,float g,float b) {
  float min;
  min = r;
  if(g<min) min =g;
  if(b<min) min =b;
  return min;
}
int RGBtoHSV(float r,float g,float b,float *h,float *s,float *v) {
  float max,min,del;
  max = Maxrgb(r,g,b);
  min = Minrgb(r,g,b);
  *v = max;
  if(max != 0) *s = (max -min)/max;
  else *s=0;
  if(*s==0) *h = 0;
  else {
     del = max - min;
     if( r==max) *h= (g-b)/del;
     else {
       if(g==max) *h = 2+(b-r)/del;
       else {
          if(b==max) *h= 4+(r-g)/del;
       }
     }
     *h *=60;
     if(*h < 0 ) *h += 360;
  }
  return 1;
}
int RGBtoHLS(float r,float g, float b,float *h,float *l,float *s) {
  float max,min,del;
  max = Maxrgb(r,g,b);
  min = Minrgb(r,g,b);
  *l= (max+min)*0.5;
  if( max == min) {
    *s=0; *h=0;
  }
  else {
    if( *l <= 0.5 ) *s = (max - min)/(max+min);
    else *s = (max - min)/(2 - max - min);
    del = max - min;
    if( r == max ) *h = (g-b)/del;
    else {
      if( g == max ) *h = 2+(b-r)/del;
      else *h = 4 + (r-g)/del;
    } 
    *h *=60;
    if(*h < 0 ) *h += 360;
  }
}
static float Value(float n1,float n2,float h) {
  float val =0.;
  while (h >360) h -=360;
  while( h < 0 ) h +=360;
  if( h < 60 ) val = n1 + (n2 - n1)*h/60.;
  else {
    if( h < 180.) val = n2;
    else {
       if( h < 240. ) val = n1 + (n2 - n1)*(240 -h)/60.;
       else val = n1;
    }
  }
  return val;
}
int HLStoRGB(float *r,float *g,float *b,float h, float l,float s) {
   float m1,m2;
   if( l <= 0.5 ) m2 = l*(l+s);
   else m2 = l+s - l*s;
   m1 = 2*l -m2;
   if(s == 0) *r = *g = *b=l;
   else {
     *r = Value(m1,m2,h+120);
     *g = Value(m1,m2,h);
     *b = Value(m1,m2,h-120);
   }
   return 1;
}

