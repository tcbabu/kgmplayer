/********************************************************
   cosval is taken absolute; so that both sides
   of a panel are illuminated
   if it is made in the legal range that is (0. - 1.)
   one has to make sure the computation of normals of
   panels.
*********************************************************/
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <math.h>
#include "kulina.h"
#include "gprivate.h"

static float Nfac=1.0;
static float illm_fac = 1.0;
static int No_of_lights=0,St_sh_clr,No_sh_clrs=0;
static float Hue =0.,Satu=0.;

#define DSCALE 1
#define C1 0.25
#define C2 0.25
#define C3 0.5

#define scale(z) (((+(z)-box3d.zmin)/(box3d.zmax-box3d.zmin))+0.4)
#if 0
#define scale(z) ((((z+box3d.ze)-box3d.zmin)/(box3d.zmax-box3d.zmin))+0.4)
#define scale_cos(a) ((((1. + (a))/2.) * ((1. + (a))/2.)))
#define scale_cos(a) (((a>0.)?(1+ (a))/2:(1-(a))/2)*illm_fac)
#define scale_cos(a) (a)*illm_fac
#define dist(d) ((d<1)?((1. -(d))*(1. -(d))):0.0)
#define dist(d) ((d<1)?(1. /((d)*(d))):0.0)
#define scale_cos(a) (a)*(a)*illm_fac
#endif
#define scale_cos(a) (a)*illm_fac
#define sqr(x) ((x)*(x))
#define dist(d) (1.0*(d>1 ? 1: 1./(C1+C2*d+C3*d*d)))


int Chng_sh_clr(int no,int ir,int ib,int ig,int file);
void _shade_fill3(short n,int *px,int *py,int *pz ,short flag,float *pv);
void t_3_shade_fill(int n,float *x,float *y,float *z,float *p,int clr);

typedef struct 
	{
	  float x;
	  float y;
	  float z;
	}
	POINT;


typedef struct
        {
           float x,y,z;
        }
        LIGHT;



static unsigned char esc=0x1b,G='g',N='n',C='c',P='p',Z='z',L='l',V='v';
static unsigned char S='s',T='t',R='r',M='m',D='d',Y='y',U='u';
static unsigned char O='o',B='b',X='x',F='f',I='i',A='a',E='e',W='w';
static unsigned char H='h',K='k',M3 ='M',D3='D',P3='P',G3='G',B3='B',S3='S',S4='H';


void transfrm(float xp, float yp, float zp);
void projection(str1 oldstr);
void read_buf( void *ch, int n);
void write_buf(unsigned char *ch, int n );
float * computeV(int no,float *x,float *y,float *z) ;
void t_3_godr_fill(int n,float *x,float *y,float *z,float *v);
void t_godr_fill(int n,float *x,float *y,float *p);
#define write_bf(x,n) write_buf((unsigned char *)x,n)
#define uiwrite_bf(G,x,n) uiwrite_buf(G, (unsigned char *) x, n)


int HLStoRGB(float *r,float *g,float *b,float h, float l,float s);
int RGBtoHLS(float r,float g, float b,float *h,float *l,float *s);
int HSVtoRGB(float *r,float *g,float *b,float h,float s,float v);
int RGBtoHSV(float r,float g,float b,float *h,float *s,float *v);

int HSVtoRGB(float *r,float *g,float *b,float h,float s,float v) {
/*
  range r: 0-255
  range g: 0-255
  range b: 0-255
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
  *r   *= 255;
  *g *= 255;
  *b  *= 255;
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
  r /= 255.;
  g /= 255.;
  b /= 255.;
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
  r /= 255.;
  g /= 255.;
  b /= 255.;
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
  *r   *= 255;
  *g *= 255;
  *b  *= 255;
   return 1;
}

int RGBtoHSL(int r,int g,int b,float *h,float *s,float *l) {
/*
   r,g,b range is 0-255
   h range 0-360
   s 0-1
   l 0-1
*/
  double pi=3.14159265,hue,hue1,hue2,hue3;

  hue1 = atan2((double)(2.0*r -g -b),((double)(1.73205*(g - b))))/(2*pi);
  //hue3 = atan2((double)(2.0*b -r -g),((double)(1.73205*(r - g))))/(2*pi);
  //hue2 = atan2((double)(2.0*g -b -r),((double)(1.73205*(b - r))))/(2*pi);
  hue = hue1;
  *s = (sqrt((double)(r*r+g*g+b*b -r*g -r*b - b*g))/255.0);
  *l = (r+g+b)/765.0;
  hue *= 360.0;
  if( hue < 0) hue +=180;
  *h = hue;
  return 1;
}
int HSVtoRGB_o (int *red,int *green,int *blue,float h,float s,float v) {
   float r,g,b;
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
   *red   = 255*r;
   *green = 255*g;
   *blue  = 255*b;
   return 1;
}
/*
  h -(0-360)
  s  (0 -1)
  v  (0 - 1)
*/
int Chng_hsv_clr (int clr,float h,float s,float v,int file) {
   float r,g,b;
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
   Chng_sh_clr(clr,(int)(r*255),(int)(b*255),(int)(g*255),file);
}
int kgchng_hsv_clr (DIG *G,int clr,float h,float s,float v) {
   float r,g,b;
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
   kgChangeColor(G,clr,(int)(r*255),(int)(g*255),(int)(b*255));
}
int gphchng_hsv_clr (int fid,int clr,float h,float s,float v) {
   float r,g,b;
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
   gphChangeColor(fid,clr,(int)(r*255),(int)(g*255),(int)(b*255));
}
void kgShade3f(DIG *G,int n,float *x,float *y,float *z)
{
  int i,l;
  float *v;
  kgDC *dc;
  dc = G->dc;
  if(G->D !=NULL) {
    uiwrite_bf(G,&P,1);
    uiwrite_bf(G,&S3,1);
    uiwrite_bf(G,&n,4);
    l = sizeof(float)*n;
    uiwrite_bf(G,x,l);
    uiwrite_bf(G,y,l);
    uiwrite_bf(G,z,l);
  }
  for(i=0;i<n;i++)
   {
    kgtransfrm(dc,*(x+i), *(y+i),*(z+i));
    *(x+i) = dc->trnstr.xstr;
    *(y+i) = dc->trnstr.ystr;
    *(z+i) = dc->trnstr.zstr;
   }
  /* Compute v */
  v = kgcomputeV(G,n,x,y,z);
  for(i=0;i<n;i++)
   {
    dc->trnstr.xstr = x[i];
    dc->trnstr.ystr = y[i];
    dc->trnstr.zstr = z[i];
    kgprojection(dc,dc->trnstr);
    *(x+i) = dc->newstr.xstr;
    *(y+i) = dc->newstr.ystr;
    *(z+i) = dc->newstr.zstr;
   }
  if (G->D_ON) {
    if (dc->ZBUFF == 1) ui_3_godr_fill(G,n,x,y,z,v);
    else ui_godr_fill(G,n,x,y,v);
  }
  else {
    if (dc->ZBUFF == 1) img_godrfill3f(G,n,x,y,z,v);
    else img_godard_fill(G,n,x,y,v);
  }
  free(v);
}
    
void kgColorShade3f(DIG *G,int n,float *x,float *y,float *z,int baseclr)
{
  kgSetShading(G,980,24,0,baseclr);
  kgShade3f(G,n,x,y,z);
}
    
void uiwin_3_shade(DIG *G)
{
  int i,l;
  float *x,*y,*z,*v;
  int n,baseclr;
  kgDC *dc;
  dc = G->dc;

  uiread_buf(G,&n,4);
  l = sizeof(float)*n;
  x= (float *)malloc(l);
  y= (float *)malloc(l);
  z= (float *)malloc(l);
  uiread_buf(G,x,l);
  uiread_buf(G,y,l);
  uiread_buf(G,z,l);
  uiread_buf(G,&baseclr,4);
  for(i=0;i<n;i++)
   {
    kgtransfrm(dc,*(x+i), *(y+i),*(z+i));
    *(x+i) = dc->trnstr.xstr;
    *(y+i) = dc->trnstr.ystr;
    *(z+i) = dc->trnstr.zstr;
   }
  /* Compute v */
  v = kgcomputeV(G,n,x,y,z);
  for(i=0;i<n;i++)
   {
    dc->trnstr.xstr = x[i];
    dc->trnstr.ystr = y[i];
    dc->trnstr.zstr = z[i];
    kgprojection(dc,dc->trnstr);
    *(x+i) = dc->newstr.xstr;
    *(y+i) = dc->newstr.ystr;
    *(z+i) = dc->newstr.zstr;
   }
#if 0
 if (G->D_ON){
   //if (ZBUFF == 1) t_3_godr_fill(n,x,y,z,v);
   if (dc->ZBUFF == 1) ui_3_shade_fill(G,n,x,y,z,v,baseclr);
   else ui_godr_fill(G,n,x,y,v);
 }
#else
  if (G->D_ON) {
    if (dc->ZBUFF == 1) ui_3_godr_fill(G,n,x,y,z,v);
    else ui_godr_fill(G,n,x,y,v);
  }
  else {
    if (dc->ZBUFF == 1) img_godrfill3f(G,n,x,y,z,v);
    else img_godard_fill(G,n,x,y,v);
  }
#endif
}
void uiwin_3_shade_o(DIG *G)
{
  int i,l;
  float *x,*y,*z,*v;
  int n,baseclr;
  kgDC *dc;
  dc = G->dc;

  uiread_buf(G,&n,4);
  l = sizeof(float)*n;
  x= (float *)malloc(l);
  y= (float *)malloc(l);
  z= (float *)malloc(l);
  uiread_buf(G,x,l);
  uiread_buf(G,y,l);
  uiread_buf(G,z,l);
  for(i=0;i<n;i++)
   {
    kgtransfrm(dc,*(x+i), *(y+i),*(z+i));
    *(x+i) = dc->trnstr.xstr;
    *(y+i) = dc->trnstr.ystr;
    *(z+i) = dc->trnstr.zstr;
   }
  /* Compute v */
  v = kgcomputeV(G,n,x,y,z);
  for(i=0;i<n;i++)
   {
    dc->trnstr.xstr = x[i];
    dc->trnstr.ystr = y[i];
    dc->trnstr.zstr = z[i];
    kgprojection(dc,dc->trnstr);
    *(x+i) = dc->newstr.xstr;
    *(y+i) = dc->newstr.ystr;
    *(z+i) = dc->newstr.zstr;
   }
  if (G->D_ON) {
    if (dc->ZBUFF == 1) ui_3_godr_fill(G,n,x,y,z,v);
    else ui_godr_fill(G,n,x,y,v);
  }
  else {
    if (dc->ZBUFF == 1) img_godrfill3f(G,n,x,y,z,v);
    else img_godard_fill(G,n,x,y,v);
  }
}
double comp_angle(POINT n,POINT v)
{
   double cosval,ang;
   float cp,m1,m2;
   
   cp = ((n.x * v.x)+(n.y * v.y)+(n.z * v.z));
   m1 = sqrt((double) ((n.x * n.x)+(n.y * n.y)+(n.z * n.z)));
   m2 = sqrt((double) ((v.x * v.x)+(v.y * v.y)+(v.z * v.z)));
   cosval = 0.0;
   if(m1*m2 != 0 )cosval = cp / (m1 * m2);
   if(cosval < 0. ) cosval = -cosval;
   return(cosval);
}

/* --------------   Cross Product of two vectors   ------------- */
POINT cros_prd(POINT p,POINT p1,POINT p2)
{
   POINT n;
   n.x = -(p2.y - p.y)*(p1.z - p.z) + (p2.z - p.z)*(p1.y - p.y);
   n.y = ((p2.x - p.x)*(p1.z - p.z) - (p1.x - p.x)*(p2.z - p.z));
   n.z = -(p2.x - p.x)*(p1.y - p.y) + (p1.x - p.x)*(p2.y - p.y);
   return (n);
}
//--------------------
float * kgcomputeV(DIG *G,int no,float *x,float *y,float *z) {

  int i,j,no_ls ,count,No_of_lights;
  POINT p1,p2,p3,n,v;
  double cosval,d;
  LIGHT *lpt;
  float *s,diagonal;
  float xc,yc,zc,illu,dpcue;
  int j1,j0;
  POINT *pt,na={0,0,0},n0={0,0,0};
  kgDC *dc;
  dc= G->dc;
  xc = +dc->box3d.xe;
  yc = dc->box3d.ye;
  zc = +dc->box3d.zc +dc->box3d.ze;
  
  s = (float *) malloc(sizeof(float)*no);
  if(dc->ls_list==NULL) {
    for ( j = 0; j < (no) ; j++) s[j]=1.0;
  }
  else {
  pt = (POINT *) malloc(sizeof(POINT)*no);

  for ( i = 0; i < no;  i++)
     /*s[i] = scale(z[i]);*/
     s[i] = 0.;
  
    for ( j = 0; j < (no) ; j++)
    {
        p1.x = x[j];    
        p1.y = y[j];    
        p1.z = z[j];
        j1 = (j+1)%no;
        j0 = (no+j-1)%no;
        p2.x = x[j1];  
        p2.y = y[j1];  
        p2.z = z[j1];
        p3.x = x[j0];  
        p3.y = y[j0];  
        p3.z = z[j0];
        pt[j] = cros_prd(p1,p2,p3);
    }
    count = no;
    for ( j = 0; j < (no) ; j++){
        n = pt[j];
        na.x += pt[j].x;
        na.y += pt[j].y;
        na.z += pt[j].z;
        if( (n.x==0)&&(n.y==0)&&(n.z==0) ) count--;
    }
    if(count !=0) {
      na.x /=count; na.y /=count; na.z /=count;
    }
    for ( j = 0; j < (no) ; j++)
    {
        n = pt[j];
        if( (n.x==0)&&(n.y==0)&&(n.z==0) ) n = na;
        illu =0;  
        Resetlink(dc->ls_list);
        while((lpt = (LIGHT *) Getrecord(dc->ls_list)) != NULL)
        {
           //v.x = lpt->x - x[j] - xc;
           //v.y = lpt->y - y[j] - yc;
           //v.z = lpt->z - z[j] - zc;
           v.x = lpt->x - x[j] ;
           v.y = lpt->y - y[j] ;
           v.z = lpt->z - z[j] ;

           cosval = comp_angle(v,n);
   
           d = v.x * v.x + v.y * v.y + v.z * v.z;
	   d = (sqrt(d)/(DSCALE*(dc->box3d.d)));
	   illu +=   (scale_cos(cosval)*(dist(d)));
        }
        No_of_lights = Dcount(dc->ls_list);
        if(No_of_lights > 1) illu /= No_of_lights;
#if 0  /* removed from here due to the new shading scheme */
        dpcue = scale(z[j]);
        if (dpcue > 1.) dpcue=1.;
        illu *= dpcue;
#endif
        s[j] += ((illu > illm_fac) ? illm_fac :illu);
        /*if((s[j] <0) ||(s[j] >1.)) printf("Error:\n");*/
        if((s[j] <0) ) s[j]=0.;
        if((s[j] > 1) ) s[j]=1.;
    } 
  free(pt);
  }
  return s;
}

int kgAddLight(DIG *G,float x,float y,float z)
{
  LIGHT *pt;
  kgDC *dc;
  dc= G->dc;
  if(dc->ls_list == NULL) {
     dc->ls_list = Dopen();
  }
  pt =(LIGHT *)malloc(sizeof(LIGHT));
  pt->x = x;
  pt->y = y;
  pt->z = z;
  Dadd(dc->ls_list,pt);
  dc->No_of_lights++;
  return 0;
}
int kgClearLights(DIG *G) {
  kgDC *dc;
  dc= G->dc;
  if(dc->ls_list != NULL) Dempty(dc->ls_list);
  dc->ls_list = NULL;
  dc->No_of_lights=0;
}

/* --------------------  Get color combinations for shading  ------------------------------ */
int kgMixshadecolors_rgb(DIG *G,int sclr,int nclrs) 
{
  float dx,gray;
  int i,red;
  if( (nclrs <=0) || (nclrs>255) ) 
  {
    printf("ERROR: invalid number of colours: %d \n",nclrs);
    exit(-1);
  }
  gray = 75;
  dx = (255. -gray)/(nclrs+1 );
  kgChangeColor(G,sclr,(int)gray,(int)gray,(int)gray);
  for(i=1;i<=nclrs;i++) 
  {
    kgChangeColor(G,i+sclr,(int)gray,(int)gray,(int)gray);
    gray +=dx;
  }
  return 0;
}  

int kgmixshadecolors_hsv(DIG *G,int sclr,int nclrs,float h,float s) 
{
  float dx,gray;
  int i,red;
  float v;
  if( (nclrs <=0) || (nclrs>255) ) 
  {
    printf("ERROR: invalid number of colours: %d \n",nclrs);
    exit(-1);
  }
  St_sh_clr = sclr;
  No_sh_clrs = nclrs;
  gray = 0.2;
  dx = (1. -gray)/(nclrs+1 );
  kgchng_hsv_clr(G,sclr,h,s,gray);
  for(i=1;i<=nclrs;i++) 
  {
    kgchng_hsv_clr(G,i+sclr,h,s,gray);
    gray +=dx;
  }
  return 0;
}  
int gphmixshadecolors_hsv(int fid,int sclr,int nclrs,float h,float s,float v) 
{
  float dx,gray;
  int i,red;
  if( (nclrs <=0) || (nclrs>255) ) 
  {
    printf("ERROR: invalid number of colours: %d \n",nclrs);
    exit(-1);
  }
  St_sh_clr = sclr;
  No_sh_clrs = nclrs;
  gray = v;
  if(gray > 1.0) gray=0.8;
  if(gray < 0.2 ) gray=0.2;
  dx = (1. -gray)/(nclrs+1 );
  gphchng_hsv_clr(fid,sclr,h,s,gray);
  for(i=1;i<=nclrs;i++) 
  {
    gphchng_hsv_clr(fid,i+sclr,h,s,gray);
    gray +=dx;
  }
  return 0;
}  
int gphMixHsvColors(int fid,int sclr,int nclrs,float h,float s,float vmin,float vmax) 
{
  float dx,gray;
  int i,red;
  if( (nclrs <=0) || (nclrs>255) ) 
  {
    printf("ERROR: invalid number of colours: %d \n",nclrs);
    exit(-1);
  }
  St_sh_clr = sclr;
  No_sh_clrs = nclrs;
  if(vmin > 0.999) vmin=0.999;
  if(vmax > 1.0) vmax = 1.0;
  if(vmin < 0. ) vmin=0;
  if(vmax < 0. ) vmax=0;
  gray = vmin;
//  if(gray < 0.2 ) gray=0.2;
  dx = (vmax -gray)/(nclrs );
  gphchng_hsv_clr(fid,sclr,h,s,gray);
  for(i=1;i<nclrs;i++) 
  {
    gray +=dx;
    gphchng_hsv_clr(fid,i+sclr,h,s,gray);
  }
  gphchng_hsv_clr(fid,nclrs+sclr,h,s,gray);
  return 0;
}  
int kgMixHsvColors(void * fid,int sclr,int nclrs,float h,float s,float vmin,float vmax) 
{
  float dx,gray;
  int i,red;
  if( (nclrs <=0) || (nclrs>255) ) 
  {
    printf("ERROR: invalid number of colours: %d \n",nclrs);
    exit(-1);
  }
  St_sh_clr = sclr;
  No_sh_clrs = nclrs;
  if(vmin > 0.999) vmin=0.999;
  if(vmax > 1.0) vmax = 1.0;
  if(vmin < 0. ) vmin=0;
  if(vmax < 0. ) vmax=0;
  gray = vmin;
//  if(gray < 0.2 ) gray=0.2;
  dx = (vmax -gray)/(nclrs );
  kgchng_hsv_clr(fid,sclr,h,s,gray);
  for(i=1;i<nclrs;i++) 
  {
    gray +=dx;
    kgchng_hsv_clr(fid,i+sclr,h,s,gray);
  }
  kgchng_hsv_clr(fid,nclrs+sclr,h,s,gray);
  return 0;
}  
#if 0
int Mixshadecolors_hsv(int sclr,int nclrs,float h,float s) 
{
  float dx,gray;
  int i,red;
  float v;
  if( (nclrs <=0) || (nclrs>255) ) 
  {
    printf("ERROR: invalid number of colours: %d \n",nclrs);
    exit(-1);
  }
  St_sh_clr = sclr;
  No_sh_clrs = nclrs;
  gray = 0.2;
  dx = (1. -gray)/(nclrs+1 );
  Chng_hsv_clr(sclr,h,s,gray);
  for(i=1;i<=nclrs;i++) 
  {
    Chng_hsv_clr(i+sclr,h,s,gray);
    gray +=dx;
  }
  return 0;
}  
#endif
int Fixshadecolors_hsv(int file)
{
  int nclrs,sclr;
  float dx,gray;
  int i,red;
  float v,h,s;
  h = Hue;
  s = Satu;
  sclr = St_sh_clr;
  nclrs = No_sh_clrs;
  if( (nclrs <=0) || (nclrs>255) ) 
  {
    return 0;
  }
  if(No_of_lights <= 0 ) return 0;
  gray = 0.2;
  dx = (1. -gray)/(nclrs+1 );
  Chng_hsv_clr(sclr,h,s,gray,file);
  for(i=1;i<=nclrs;i++) 
  {
    Chng_hsv_clr(i+sclr,h,s,gray,file);
    gray +=dx;
  }
  return 1;
}  


int kgSetShading(DIG *G,int startclr,int ncolors,int bodr,int basecolor){
     int r,g,b;
     float red,green,blue,h,s,v;
     kgGetDefaultRGB(basecolor,&r,&g,&b);
     red=r;blue=b;green=g;
     RGBtoHSV(red,green,blue,&h,&s,&v);
     kgMixHsvColors(G,startclr,ncolors,h, s,0.3,1.0) ;
//     kgMixshadecolors_rgb(G,startclr,ncolors); 
     kgSetShadeParams(G,1.0,0.3,startclr,ncolors,bodr);
     return 1;
}
int kgSetHSVShading(DIG *G,int startclr,int ncolors,int bodr,float hue,float satu){
     Hue = hue;
     Satu = satu;
     kgmixshadecolors_hsv(G,startclr,ncolors,hue,satu); 
     kgSetShadeParams(G,1.0,0.0,startclr,ncolors,bodr);
     return 1;
}
int gphSetHSVShading(int fid,int startclr,int ncolors,int bodr,float hue,float satu){
     Hue = hue;
     Satu = satu;
     gphmixshadecolors_hsv(fid,startclr,ncolors,hue,satu,0.2); 
     gphSetShadeParams(fid,1.0,0.0,startclr,ncolors,bodr);
     return 1;
}

