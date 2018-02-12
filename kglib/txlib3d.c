
/* Z buffer Code */
/*
 As on 14th Feb 2005
 An attempt to include translucency/transperency




*/
int RGBtoHSL(int r,int g,int b,float *h,float *s,float *l);
int HLStoRGB(float *r,float *g,float *b,float h, float l,float s);
int HSVtoRGB(float *r,float *g,float *b,float h,float s,float v);
int RGBtoHSV(float r,float g,float b,float *h,float *s,float *v);
int RGBtoHLS(float r,float g, float b,float *h,float *l,float *s);
#define scr_z(z) (int)((z-clip_min)*CPCONS+0.5);
#define fscr_z(z) (float)((z-clip_min)*CPCONS); /* float version */
#define uiscr_z(z) (int)((z-dc->clip_min)*dc->CPCONS+0.5);
#define uifscr_z(z) (float)((z-dc->clip_min)*dc->CPCONS); /* float version */
#define UNSIGNED unsigned short
#define far  
#define MAXZ 65000
static int cur_z;
//UNSIGNED  *zbuf=NULL,*redbuf=NULL,*greenbuf=NULL,*bluebuf=NULL,*clrbuf=NULL;
static int Z_max=193600L;
//static float clip_min=0.,clip_max=1000.;
static float CPCONS=1.0;
static int BY_P_LN=80;
void get_color(int col,short *r,short *g,short *b);
void get_cur_color(int *r,int *g,int *b) ;
void _put_trans_pix3(int col,int row,short r,short g,short b);
int PutPixel_IMAGE(int x,int y,int red,int green,int blue);
void put_shade_pix3(int col,int row,int z,float illu);
void draw_line3(int xa,int ya,int za,int xb,int yb,int zb);
void hori_line3(int xa,int xb,int y,int za,int zb);
void godr_line3(int xa,int xb,int y,int za,int zb,float pa,float pb);
void put_pix3(int,int,int);
void _box_fill3(int x1,int y1,int z1,int x2,int y2,int z2, unsigned int color);
void _draw_line3(int x1,int y1,int z1,int x2,int y2,int z2);
void _poly_fill3(short n,int *px,int *py,float *pz,short flag,unsigned int b_color);
void _godard_fill3(short n,int *px,int *py,int *pz ,short flag,float *pv);
void mline3( short ixb, short iyb, short izb, short ixe,short iye,short ize);
void check_clip(int r,int c,int z);
void shade_line3(int xa,int xb,int y,int za,int zb,float pa,float pb);
#define z_inter(x,m,c) (int)(((x)*(m) + c)+0.5)
#define fz_inter(x,m,c) (float)(((x)*(m) + c))

void ui_3_move(DIG *G,float x,float y,float z) 
  {
  int x1,y1,z1;
  kgDC *dc;
  dc = G->dc;
  x1=uiscr_x(x);
  y1=uiscr_y(y);
  z1=uiscr_z(z);
  _uimove3(G,x1,y1,z1);
  }
 void ui_3_draw(DIG *G,float ix,float iy,float iz)
  {
  int x1,y1,z1;
  kgDC *dc;
  dc = G->dc;
  x1=uiscr_x(ix);
  y1=uiscr_y(iy);
  z1=uiscr_z(iz);
  wcset_clr(dc->wc,dc->ln_color);
  _uidraw3(G,x1,y1,z1);
  }

  
void ui_3_panel(DIG *G,float *x,float *y,float *z,int color,int flag,int n)
  {
  int x1[1000],y1[1000];
  float z1[1000];
  short i;
  int xmin=100000000,xmax = -100000000,ymin =100000000,ymax = -100000000;
  kgDC *dc;
  dc = G->dc;
  for(i=0;i<n;i++) 
  {
    x1[i] =uiscr_x(x[i]); 
    y1[i] =uiscr_y(y[i]);
    z1[i] =uifscr_z(z[i]);
    if(xmin > x1[i]) xmin = x1[i];	  
    if(ymin > y1[i]) ymin = y1[i];	  
    if(xmax < x1[i]) xmax = x1[i];	  
    if(ymax < y1[i]) ymax = y1[i];	  
  }
  if( (xmin==xmax) || (ymin==ymax)) return;
//  if((xmax < dc->v_x1)||(xmin >dc->v_x2)||(ymax<dc->v_y1)||(ymin >dc->v_y2)) return;
  if((xmax < (dc->v_x1+dc->D_x))||(xmin >(dc->v_x2+dc->D_x))||(ymax<(dc->v_y1+dc->D_y))||(ymin >(dc->v_y2+dc->D_y))) return;
  _uipoly_fill3(G,n,x1,y1,z1,flag,color);
}
void ui_3_box_fill(DIG *G,float x1,float y1,float z1,float x2,float y2,float z2,int color,int ib)
  {
  int xa,ya,za,xb,yb,zb;
  kgDC *dc;
  dc = G->dc;
  xa=uiscr_x(x1);
  ya=uiscr_y(y1);
  za=uiscr_z(z1);
  xb=uiscr_x(x2);
  yb=uiscr_y(y2);
  zb=uiscr_z(z1);
  _uibox_fill3(G,xa,ya,za,xb,yb,zb,color);
  }
 void ui_3_godr_fill(DIG *G,int n,float *x,float *y,float *z,float *p)
 {
  int x1[20],y1[20],z1[20];
  float p1[20],pv;
  short i;
  int xmin=100000000,xmax = -100000000,ymin =100000000,ymax = -100000000;
  kgDC *dc;
  dc = G->dc;
  for(i=0;i<n;i++)
  {
    x1[i] =uiscr_x(x[i]); 
    y1[i] =uiscr_y(y[i]);
    z1[i] =uiscr_z(z[i]);
    if(xmin > x1[i]) xmin = x1[i];	  
    if(ymin > y1[i]) ymin = y1[i];	  
    if(xmax < x1[i]) xmax = x1[i];	  
    if(ymax < y1[i]) ymax = y1[i];	  
  }
  if( (xmin==xmax) || (ymin==ymax)) return;
//  if((xmax < dc->v_x1)||(xmin >dc->v_x2)||(ymax<dc->v_y1)||(ymin >dc->v_y2)) return;
  if((xmax < (dc->v_x1+dc->D_x))||(xmin >(dc->v_x2+dc->D_x))||(ymax<(dc->v_y1+dc->D_y))||(ymin >(dc->v_y2+dc->D_y))) return;
   for (i=0;i<n;i++) {pv=*(p++), p1[i] = uipclr(pv); }
  _uigodard_fill3(G,(short)n,x1,y1,z1,(short)dc->ibdr,p1);
 }

 void ui_3_shade_fill(DIG *G,int n,float *x,float *y,float *z,float *p,int clr)
 {
  int x1[20],y1[20],z1[20];
  float p1[20],pv;
  short i;
  int temp;
  int xmin=100000000,xmax = -100000000,ymin =100000000,ymax = -100000000;
  kgDC *dc;
  dc = G->dc;
  for(i=0;i<n;i++)
  {
    x1[i] =uiscr_x(x[i]); 
    y1[i] =uiscr_y(y[i]);
    z1[i] =uiscr_z(z[i]);
    if(xmin > x1[i]) xmin = x1[i];	  
    if(ymin > y1[i]) ymin = y1[i];	  
    if(xmax < x1[i]) xmax = x1[i];	  
    if(ymax < y1[i]) ymax = y1[i];	  
  }
  if( (xmin==xmax) || (ymin==ymax)) return;
//  if((xmax < dc->v_x1)||(xmin >dc->v_x2)||(ymax<dc->v_y1)||(ymin >dc->v_y2)) return;
  if((xmax < (dc->v_x1+dc->D_x))||(xmin >(dc->v_x2+dc->D_x))||(ymax<(dc->v_y1+dc->D_y))||(ymin >(dc->v_y2+dc->D_y))) return;
   //for (i=0;i<n;i++) {pv=*(p++), p1[i] = pclr(pv); }
   temp = dc->wc->c_color;
   wcset_clr(dc->wc,clr);
  _uishade_fill3(G,(short)n,x1,y1,z1,(short)dc->ibdr,p);
   wcset_clr(dc->wc,temp);
 }

  
void uiclear_zbuf(DIG *G)
 {
  int i,max;
  UNSIGNED *pt;
  kgDC *dc;
  kgWC *wc;
  wc=G->wc;
  dc=G->dc;
  pt = (UNSIGNED  *)dc->zbuf;
  max= dc->Z_max;
  for(i=0;i<max;i++) *(pt+i)=0;
  pt = (UNSIGNED  *)dc->redbuf;
  if(pt!= NULL) {
    for(i=0;i<max;i++) *(pt+i)=0;
    pt = (UNSIGNED  *)dc->greenbuf;
    for(i=0;i<max;i++) *(pt+i)=0;
    pt = (UNSIGNED  *)dc->bluebuf;
    for(i=0;i<max;i++) *(pt+i)=0;
    pt = (UNSIGNED  *)dc->clrbuf;
    for(i=0;i<max;i++) *(pt+i)=1;
  }
}
void ui_set_zbuf(DIG *G)
 {
  int i;
  kgDC *dc;
  kgWC *wc;
  wc=G->wc;
  dc=G->dc;
  if(dc->zbuf!= NULL) ui_release_buf(G);
  if(G->D_ON==0) {
    dc->Z_max=(G->x2-G->x1+1)*(G->y2-G->y1+1)*G->MAG*G->MAG;
    dc->redbuf   = NULL;
    dc->greenbuf = NULL;
    dc->bluebuf  = NULL;
    dc->clrbuf  = NULL;
  }
  else {
    dc->Z_max=(wc->EVGAY+3)*(wc->EVGAX+3);
#if 0
    dc->redbuf   = (UNSIGNED  *) malloc(sizeof(UNSIGNED )*dc->Z_max);
    dc->greenbuf = (UNSIGNED  *) malloc(sizeof(UNSIGNED )*dc->Z_max);
    dc->bluebuf  = (UNSIGNED  *) malloc(sizeof(UNSIGNED )*dc->Z_max);
    dc->clrbuf  = (UNSIGNED  *) malloc(sizeof(UNSIGNED )*dc->Z_max);
#else
    dc->redbuf   = NULL;
    dc->greenbuf = NULL;
    dc->bluebuf  = NULL;
    dc->clrbuf  = NULL;
#endif
  }
  dc->zbuf = (UNSIGNED  *) malloc(sizeof(UNSIGNED )*dc->Z_max);
  if(dc->zbuf== NULL) {
     printf(" Error: Zbuffer allocation\n");
     exit(0);
  }
  uiclear_zbuf(G);
 }
void ui_clip_const(DIG *G,float cpmin,float cpmax)
 {
  kgDC *dc;
  dc = G->dc;
  dc->clip_min=cpmin;
  dc->clip_max = cpmax;
  dc->CPCONS = MAXZ/(float)(dc->clip_max-dc->clip_min);
 }
void ui_near_clip(DIG *G,float cp)
 {
  kgDC *dc;
  dc = G->dc;
  dc->clip_min = cp;
  dc->CPCONS = MAXZ/(float)(dc->clip_max-dc->clip_min);
 }
void ui_far_clip(DIG *G,float cp)
 {
  kgDC *dc;
  dc = G->dc;
  dc->clip_max = cp;
  dc->CPCONS = MAXZ/(float)(dc->clip_max-dc->clip_min);
 }
void ui_release_buf(DIG *G)
 {
  kgDC *dc;
  dc = G->dc;
//  dc->clip_min=0;
//  dc->CPCONS=1.;
  if(dc->zbuf!= NULL) free((void *)dc->zbuf);
  if(dc->redbuf != NULL) free((void *)dc->redbuf);
  if(dc->greenbuf != NULL) free((void *)dc->greenbuf);
  if(dc->bluebuf != NULL) free((void *)dc->bluebuf);
  if(dc->clrbuf != NULL) free((void *)dc->clrbuf);
  dc->zbuf=dc->redbuf=dc->greenbuf=dc->bluebuf=dc->clrbuf=NULL;
 }
#define OPAQUEPOINT (*clr != 1)
void uiput_pix3_o(DIG *G,int col,int row,int z)
{
  int c,cclr;
  int red=255,green=0,blue=255,val;
  int redo=255,greeno=0,blueo=255,pos;
  float fac =+.3,fac1=0.2,temp,oillu;
  UNSIGNED   *loc,*r,*g,*b,*clr;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
  wc=G->wc;
  if( (z<0) || (z>MAXZ)) return;
  pos = row*(wc->EVGAX)+col;
  loc=dc->zbuf+pos;
//  if (z ==  *(loc)) return;
  if(z <= *(loc)) return;
  fac = (float)z/MAXZ;
  // fac =fac*fac;
  fac1	= 1 -fac;
  cclr = wc->c_color;
//  get_cur_color(&red,&green,&blue);
  uigetcurrgb(wc,&red,&green,&blue);
  clr = dc->clrbuf+pos;
  r=dc->redbuf+pos;
  g=dc->greenbuf+pos;
  b=dc->bluebuf+pos;
  {
    {
      if (z <  *(loc)){
#if 0
        *r =  fac*red;
        *g =  fac*green;
        *b =  fac*blue;
        *(loc) =z;
#endif
      }
      else {
#if 0
        *r =  fac*red;
        *g =  fac*green;
        *b =  fac*blue;
#else
//        *r =  red;
//        *g =  green;
//        *b =  blue;
        uiput_pixl(wc,col,row);
#endif
        *(loc) =z;
      }
//      *clr = 0;
    }
  }
}
void uiput_pix3(DIG *G,int col,int row,int z)
{
  int c,cclr;
  int red=255,green=0,blue=255,val;
  int redo=255,greeno=0,blueo=255,pos;
  float fac =+.3,fac1=0.2,temp,oillu;
  UNSIGNED   *loc,*r,*g,*b,*clr;
  kgDC *dc;
  kgWC *wc;
  dc=G->dc;
//  wc=G->wc; not correct since it is a copy for clipping
  wc =  WC((G->D));
  if( (z<0) || (z>MAXZ)) return;
//  row = wc->EVGAY-1-row;
  pos = row*(wc->EVGAX+1)+col;
  loc=dc->zbuf+pos;
  if(z <= *(loc)) return;
  uiput_pixl(wc,col,row);
  *(loc) =z;
}
void uiput_shade_pix3_rgb(DIG *G,int col,int row,int z,float illu)
{
  int c,cclr;
  int red=255,green=0,blue=255,val;
  int redo=255,greeno=0,blueo=255,pos;
  float fac =+.3,fac1=0.2,temp,oillu;
  float h,s,l;
  UNSIGNED   *loc,*r,*g,*b,*clr;
  kgDC *dc;
  kgWC *wc;
  wc = G->wc;
  dc=G->dc;
  if( (z<0) || (z>MAXZ)) return;
  pos = row*(wc->EVGAX)+col;
  loc=dc->zbuf+pos;
  //if (!TRANSPERANCY && (z <  *(loc))) return;
  if (z <=  *(loc)) return;
  fac = (float)z/MAXZ;
  if(fac > 1) fac =1;
  //fac =fac*fac;
  fac1	= 1 -fac;
  cclr = wc->c_color;
//  get_cur_color(&red,&green,&blue);
  uigetcurrgb(wc,&red,&green,&blue);
//  clr = dc->clrbuf+pos;
  r=dc->redbuf+pos;
  g=dc->greenbuf+pos;
  b=dc->bluebuf+pos;
  illu = illu*1.5+0.4;
  fac *=illu;
  //if(fac > 1.) fac =1.0;
  {
    {
#if 0
      if (z <  *(loc)){
        *r +=  fac*red;
        *g +=  fac*green;
        *b +=  fac*blue;
        *(loc) =z;
      }
      else {
        *r =  fac*red;
        *g =  fac*green;
        *b =  fac*blue;
        *(loc) =z;
      }
#else
        *r =  red;
        *g =  green;
        *b =  blue;
        *(loc) =z;
#endif
//      *clr = 0;
    }
  }
}
void uiput_shade_pix3(DIG *G,int col,int row,int z,float illu)
{
  int c,cclr;
  int red=255,green=0,blue=255,val;
  int redo=255,greeno=0,blueo=255,pos;
  float fac =+.3,fac1=0.2,temp,oillu;
  float h,s,l;
  float RED,GREEN,BLUE;
  UNSIGNED   *loc,*r,*g,*b,*clr;
  kgDC *dc;
  kgWC *wc;
  wc = G->wc;
  dc=G->dc;
  if( (z<0) || (z>MAXZ)) return;
  pos = row*(wc->EVGAX)+col;
  loc=dc->zbuf+pos;
  //if (!TRANSPERANCY && (z <  *(loc))) return;
  if (z <=  *(loc)) return;
//  if (z == *(loc)) return;
  fac = (float)z/MAXZ;
  // fac =fac*fac;
  fac1	= 1 -fac;
  cclr = wc->c_color;
//  get_cur_color(&red,&green,&blue);
  uigetcurrgb(wc,&red,&green,&blue);
  RGBtoHSV((float)red,(float)green,(float)blue,&h,&s,&l);
  //RGBtoHLS((float)red,(float)green,(float)blue,&h,&l,&s);
//  clr = dc->clrbuf+pos;
  r=dc->redbuf+pos;
  g=dc->greenbuf+pos;
  b=dc->bluebuf+pos;
  illu = illu+0.1;
  fac *= (illu);
  if(fac > 1.) fac =1.0;
  HSVtoRGB(&RED,&GREEN,&BLUE,h,s,fac);
  //HLStoRGB(&R,&G,&B,h,fac,s);
  {
    {
      if (z <  *(loc)){
        *r +=  RED;
        *g +=  GREEN;
        *b +=  BLUE;
        *(loc) =z;
      }
      else {
        *r =  RED;
        *g =  GREEN;
        *b =  BLUE;
        *(loc) =z;
      }
//      *clr = 0;
    }
  }
}
void _uimove3(DIG *G,int xp, int yp,int zp)
{
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  dc->cur_x=xp ;
  dc->cur_y=yp ;
  dc->cur_z=zp;
  return;
}

void _uidraw_line3(DIG *G,int x1,int y1,int z1,int x2,int y2,int z2)
{
 char  *addr;
 short i,dx,dy,f,g,inc1,inc2,ab,dx1,dy1,miny,maxy,minx,maxx;
 int clip,ch,rh,r,c;
 unsigned int temp;
 int xa,ya,xb,yb,za,zb,z;
 double cons,m;
 double xf,yf;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
 xa =x1,xb=x2,ya=y1,yb=y2,za=z1,zb=z2;
 xf=x2-x1;yf=y2-y1;
 if ((((z1)<0)&&((z2)<0))||(((z1)>MAXZ)&&((z2)>MAXZ))) return;
 clip=uiclip_line(wc,&xa,&ya,&xb,&yb);
 if (clip==0) return;
 if (abs(xf)<= abs(yf))
 {
  if( y1==y2) m=0;
  else m = (double)(z2-z1)/(double)(yf);
  cons = z1 - y1*m;
  za=z_inter(ya,m,cons);zb=z_inter(yb,m,cons);
 }
 else
 {
  m = (double)(z2-z1)/(double)(xf);
  cons = z1 - x1*m;
  za=z_inter(xa,m,cons);zb=z_inter(xb,m,cons);
 }
 if ( (xa!=xb) && (ya!=yb) )
	{
	  dx=xb-xa;
	  dy=yb-ya;
	  dx1=abs(dx);
	  dy1=abs(dy);
	  if ( dx1 > dy1 )
	  {
		 c=xa;
		 r=ya;
		 f=xb;
		 inc1=(dy1<<1);
		 g=inc1-dx1;
		 inc2=g-dx1;
		 if (xa < xb) ch=1;
		 else ch=-1;
		 if (ya < yb) rh=1;
		 else rh=-1;
/*        while (labs(c-f)>0)*/
		 r = wc->EVGAY-r;
		 while ((c!=f))
		  {
		    z = z_inter(c,m,cons);
		    uiput_pix3(G,c,r,z);
		    c+=ch;
		    if (g > 0) { r-=rh; g+=inc2; }
		    else g+=inc1;
		  }
	  z = z_inter(xb,m,cons);
	  uiput_pix3(G,xb,(wc->EVGAY-yb),z);
	  }
	  else
	  {
		 c=ya;
		 r=xa;
		 f=yb;
		 inc1=(dx1<<1);
		 g=inc1-dy1;
		 inc2=g-dy1;
		 if (ya < yb) ch=1;
		 else ch=-1;
		 if (xa < xb) rh=1;
		 else rh=-1;
/*        while (labs(c-f)>0)*/
		 c = wc->EVGAY-c;
		 f = wc->EVGAY-f;
		 while ((c!=f))
		 {
		   z = z_inter((wc->EVGAY-c),m,cons);
		   uiput_pix3(G,r,c,z);
		   c-=ch;
		   if (g > 0) { r+=rh; g+=inc2; }
		   else g+=inc1;
		 }
	  z = z_inter(yb,m,cons);
	  uiput_pix3(G,xb,(wc->EVGAY-yb),z);
	  }
  }
  else
     {
	if (xa==xb)
	{
	  if ( ya < yb){ miny = ya;maxy = yb;}
	  else { miny = yb; maxy = ya;}
  	  for (i=miny;i<=maxy;++i)
            { z = z_inter(i,m,cons);
              uiput_pix3(G,xa,(wc->EVGAY-i),z);}
	}
       else 
	{
	  if ( xa < xb){ minx = xa;maxx = xb;}
	  else { minx = xb; maxx = xa;}
          for (i=minx;i<=maxx;++i)
          {  
	    z = z_inter(i,m,cons);
	    uiput_pix3(G,i,(wc->EVGAY-ya),z);
          }
	 }
     }
	return;
}

void uidraw_line3(DIG *G,int xa,int ya,int za,int xb,int yb,int zb)
 {
   int i,i2;
   float dx,dy,ct,st,dl;
   int x1,y1,z1,x2,y2,z2,dx1,dy1;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
   if( dc->ln_width ==1 ){ _uidraw_line3(G, xa, ya, za, xb, yb, zb);return;};
   if( xa>xb){x1=xa,xa=xb,xb=x1,y1=ya,ya=yb,yb=y1;};
   i2=dc->ln_width/2;
   dx = (float)(xb-xa);
   dy = (float)(yb-ya);
   dl = (dx*dx+dy*dy);
   if( dx==0.) {ct=0.0;st=1.;}
   else { dl =sqrt(dl),ct = (dx)/dl, st = (dy)/dl;}
   dx1 =(int)( st*i2+0.5);
   if( ct< 0.) dy1 =(int)( ct*i2-0.5);
   else dy1 =(int)( ct*i2+0.5);
   xa +=dx1,xb +=dx1,ya -=dy1,yb -=dy1;
   _uidraw_line3(G, xa, ya, za, xb, yb, zb);
   for (i=1;i<dc->ln_width;i++)
	{
	     dx1 =(int)( st*i+0.5);
	     if( ct< 0.) dy1 =(int)( ct*i-0.5);
	     else dy1 =(int)( ct*i+0.5);
	    _uidraw_line3(G, xa-dx1,ya+dy1,za,xb-dx1,yb+dy1,zb);
	}
   return;
 }
void uihori_line3(DIG *G,int xa,int xb,int y,int za,int zb)
{
  short i,clip;
  char  *addr;
  int temp;
  int x1,x2,z1,z2;
  double cons,m;
  int z;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  x1=xa;x2=xb;z1=za;z2=zb;
  if (((za<0)&&(zb<0))||((za>MAXZ)&&(zb>MAXZ))) return;
  clip=uiclip_line(wc,&xa,&y,&xb,&y);
  if (clip==0) return;
  if(xa==xb){
     uiput_pix3(G,xa,(wc->EVGAY-y),za);
     return;
  }
  m = (double)(z2-z1)/(double)(x2-x1);
  cons = z1 - x1*m;
  za=z_inter(xa,m,cons);zb=z_inter(xb,m,cons);
  if (xb<xa)
  {
    temp=xa;
    xa=xb;
    xb=temp;
  }
  for(i=xa;i<=xb;i++) {z=z_inter(i,m,cons);uiput_pix3(G,i,(wc->EVGAY-y),z);}
}

void uipoly_hori_line3(DIG *G,int xa,int xb,int y,float za,float zb)
{
  short i,clip;
  char *addr;
  int temp;
  int x1,x2;
  float z1,z2;
  double cons,m;
  int z;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  x1=xa;x2=xb;z1=za;z2=zb;
  if (((za<0)&&(zb<0))||((za>MAXZ)&&(zb>MAXZ))) return;
  clip=uiclip_line(wc,&xa,&y,&xb,&y);
  if (clip==0) return;
  if(xa==xb){
     uiput_pix3(G,xa,(wc->EVGAY-y),(int)(za+0.5));
     return;
  }
  m = (double)(z2-z1)/(double)(x2-x1);
  cons = z1 - x1*m;
  za=fz_inter(xa,m,cons);zb=fz_inter(xb,m,cons);
  if (xb<xa)
  {
    temp=xa;
    xa=xb;
    xb=temp;
  }
  for(i=xa;i<=xb;i++) {z=z_inter(i,m,cons);uiput_pix3(G,i,(wc->EVGAY-y),z);}
}


void _uidraw3_(DIG *G,int x, int y, int z)
{
  int xa, ya, za, xb, yb,zb;
  short  dx, dy,  f, inc1, inc2, g,dx1,dy1,ab;
  short clip,i,ch,rh,c,r;
  double h,m,cons;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;

  xa=dc->cur_x;
  ya=dc->cur_y;
  za=dc->cur_z;

  xb=x;
  yb=y;
  zb=z;
  if (((za<0)&&(zb<0))||((za>MAXZ)&&(zb>MAXZ))) return;
  clip=uiclip_line(wc,&xa,&ya,&xb,&yb);
   if (clip == 0)
   {
        dc->cur_x=x;
        dc->cur_y=y;
        dc->cur_z=z;
	return;
   }
  if (dc->cur_x == x)
   {
    m = (double)(y-dc->cur_y)/(double)(z-dc->cur_z);
    cons = dc->cur_z - dc->cur_y*m;  
    za=z_inter(ya,m,cons);zb=z_inter(yb,m,cons);
   }
   else
   {
    m = (double)(x-dc->cur_x)/(double)(z-dc->cur_z);
    cons = dc->cur_z - dc->cur_x*m;  
    za=z_inter(xa,m,cons);zb=z_inter(xb,m,cons);
   }
   dx=xb-xa;
   dy=yb-ya;
   dx1 = abs(dx);
   dy1 = abs(dy);
   if ( dx1 > dy1 )
   {
    c=xa;
    r=ya;
    f=xb;
    inc1=2*dy1;
    g=inc1-dx1;
    inc2=g-dx1;
    if (xa < xb) ch=1;
    else ch=-1;
    if (ya < yb) rh=1;
    else rh=-1;
   }
   else
   {
    c=ya;
    r=xa;
    f=yb;
    inc1=2*dx1;
    g=inc1-dy1;
    inc2=g-dy1;
    if (ya < yb) ch=1;
    else ch=-1;
    if (xa < xb) rh=1;
    else rh=-1;
   }
   while (abs(c-f)>0)
   { i=0;
     while ((i<=dc->ln_style) && (abs(c-f)>0))
	 {
	 if ( dx1 > dy1 ) {z=z_inter(c,m,cons);uiput_pix3(G,c,(wc->EVGAY-r),z);}
	 else   {z=z_inter(r,m,cons);uiput_pix3(G,r,(wc->EVGAY-c),z);}
	 c+=ch;
         if (g > 0) { r+=rh; g+=inc2; }
         else g+=inc1;
         i++;
	 }
    i=0;
    while ((i<=dc->ln_style) && (abs(c-f)>0))
	 {
	 c+=ch;
         if (g > 0) { r+=rh; g+=inc2; }
         else g+=inc1;
         i++;
	 }
    }
   dc->cur_x=x;
   dc->cur_y=y;
   dc->cur_z=z;
   return;
}


void _uidraw3(DIG *G,int x, int y, int z)
 {
   int i,i2;
   float dx,dy,ct,st,dl;
   int x1,y1,z1,xa,ya,za,xb,yb,zb,dx1,dy1;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
   xa=dc->cur_x;
   ya=dc->cur_y;
   za=dc->cur_z;
   xb=x;
   yb=y;
   zb=z;
   if ( dc->ln_style == 0) {uidraw_line3(G,xa,ya,za,xb,yb,zb);}
   else
    {
     if(dc->ln_width==1) _uidraw3_(G, x,y,z);
     else
      {
      if( xa>xb){x1=xa,xa=xb,xb=x1,y1=ya,ya=yb,yb=y1;};
      i2=dc->ln_width/2;
      dx = (float)(xb-xa);
      dy = (float)(yb-ya);
      dl = (dx*dx+dy*dy);
      if( dx==0.) {ct=0.0;st=1.;}
      else { dl =sqrt(dl),ct = (dx)/dl, st = (dy)/dl;}
      dx1 =(int)( st*i2+0.5);
      if( ct< 0.) dy1 =(int)( ct*i2-0.5);
      else dy1 =(int)( ct*i2+0.5);
      xa +=dx1,xb +=dx1,ya -=dy1,yb -=dy1;
      dc->cur_x=xa,dc->cur_y=ya;
      _uidraw3_(G, xb, yb,zb);
      for (i=1;i<dc->ln_width;i++)
        {
             dx1 =(int)( st*i+0.5);
             if( ct< 0.) dy1 =(int)( ct*i-0.5);
             else dy1 =(int)( ct*i+0.5);
             dc->cur_x=xa-dx1,dc->cur_y=ya+dy1;
            _uidraw3_(G, xb-dx1,yb+dy1,zb);
        }
      }
    }
   dc->cur_x=x;
   dc->cur_y=y;
   dc->cur_z=z;
   return;
 }


void _uibox_fill3(DIG *G,int x1,int y1,int z1,int x2,int y2,int z2, unsigned int color)
{
  int x_min,y_min,x_max,x_max1,x_min1,y_max,i,j;
  short po1;
  unsigned int temp;
  double cons,m;
  int z;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  if (x1 == x2)
   {
    m = (double)(z2-z1)/(double)(y2-y1);
    cons = z1 - y1*m;
   }
  else
   {
    m = (double)(z2-z1)/(double)(x2-x1);
    cons = z1 - x1*m;
   }
  temp=wc->c_color;
  wcset_clr(wc,color);
  if( x1<x2 ) { x_min= x1; x_max1= x2; }
  else{ x_min= x2; x_max1= x1; }
  if(( x_min<=wc->c_v_x2) && (x_max1>=wc->c_v_x1))
   {
    if( y1<y2 ) { y_min= y1; y_max= y2; }
    else{ y_min= y2; y_max= y1; }
       
    if(( y_min<=wc->c_v_y2) && (y_max>=wc->c_v_y1))
     {
      if(x_max1>wc->c_v_x2) x_max1 = wc->c_v_x2;
      if(x_min<wc->c_v_x1) x_min = wc->c_v_x1;
      if(y_max>wc->c_v_y2) y_max = wc->c_v_y2;
      if(y_min<wc->c_v_y1) y_min = wc->c_v_y1;
      for(i=x_min;i<=x_max1;i++) {
        for (j=y_min;j<=y_max;++j)
           {z=z_inter(x_min,m,cons);uiput_pix3(G,x_min,wc->EVGAY-j,z);}
      }
    }
}
  wcset_clr(wc,temp);
  return;
}



void _uipoly_fill3(DIG *G,short n,int *px,int *py,float *pz,short flag,unsigned int b_color)
{
  short j,imd;
  short i,cnt,ind,flg;
  int xcord[50];
  float zcord[50];
  int xlow,xup;
  int txx;
  int ymax[15],ymin[15],xmax[15],xmin[15],
          tyx,tyn,txn,scan,ygrt;
  float tzn,tzx;
  float zmax[15],zmin[15];
  unsigned int tempc;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  xup =-100000;
  xlow = 1000000;
  scan=10000;
  ygrt=-1239000;
  i=0;flg=0;
  while((i<n)&&(flg==0))
  { if ((pz[i++]>0)) flg=1;}
  if (flg==0) return;
  i=0;flg=0;
  while((i<n)&&(flg==0)) {
      if ((pz[i]>= 0)&&(pz[i]<=MAXZ)) flg=1;
      i++;
  }
  if (flg==0) return;
  for (i=0;i<n;++i)
  {
	imd=(i+1)%n;
	if(py[i]>py[imd])
	{
	  ymax[i]=py[i];
	  xmax[i]=px[i];
	  zmax[i]=pz[i];
	  ymin[i]=py[imd];
	  xmin[i]=px[imd];
	  zmin[i]=pz[imd];
	}
	else
	{
	  ymin[i]=py[i];
	  xmin[i]=px[i];
	  zmin[i]=pz[i];
	  ymax[i]=py[imd];
	  xmax[i]=px[imd];
	  zmax[i]=pz[imd];
	}
	if (ygrt < py[i]) ygrt = py[i];
	if (scan > py[i]) scan = py[i];
	if (xup  < px[i]) xup  = px[i];
	if (xlow > px[i]) xlow = px[i];
	j=i-1;
	tyx=ymax[i];
	tyn=ymin[i];
	txx=xmax[i];
	txn=xmin[i];
	tzx=zmax[i];
	tzn=zmin[i];
	while (j>=0 && tyx<ymax[j])
	{
	  ymax[j+1]=ymax[j];
	  ymin[j+1]=ymin[j];
	  xmax[j+1]=xmax[j];
	  xmin[j+1]=xmin[j];
	  zmax[j+1]=zmax[j];
	  zmin[j+1]=zmin[j];
	  j--;
	}
	ymax[j+1]=tyx;
	ymin[j+1]=tyn;
	xmax[j+1]=txx;
	xmin[j+1]=txn;
	zmax[j+1]=tzx;
	zmin[j+1]=tzn;
  }
  ind=0;
  if((scan> wc->c_v_y2) || (ygrt< wc->c_v_y1) || (xlow> wc->c_v_x2) || (xup< wc->c_v_x1)) return;
  tempc = wc->c_color;
  wcset_clr(wc,b_color);
  if (ymax[ind]==scan) ind++;
  while (scan<=ygrt)
  {
	 cnt=0;
	 while (ymax[ind]==scan) ++ind;  /*  excluding upper edges  */
         if(scan<wc->c_v_y1) {scan++;continue;}
         if(scan>wc->c_v_y2) break;
	 for(i=ind;i<n;++i)
	 {
	  if (ymin[i]<=scan)
	  {
		 if (ymax[i]!=ymin[i])
		 {
                   xcord[cnt]=(((float)(xmax[i]-xmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +xmin[i])+0.5);
                   zcord[cnt]=(((float)(zmax[i]-zmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +zmin[i]));
		  ++cnt;
		 }
	  }
	 }
	 /*sorting scan line intersection xcords of edges*/
	 for (i=0;i<cnt;++i)
	 {
		j=i-1;
		txx=xcord[i];
		tzx=zcord[i];
		while(j>=0 && txx < xcord[j])
                      { zcord[j+1]=zcord[j];xcord[j+1]=xcord[j];j--;}
		xcord[j+1]=(txx);
		zcord[j+1]=(tzx);
	 }
	 /*displaying scan lines*/
	 for (i=0;i<cnt;i+=2)
           uihori_line3(G,xcord[i],xcord[i+1],scan,(int)(zcord[i]+0.5),
                                       (int)(zcord[i+1]+0.5));
	 scan++;
  }
  wcset_clr(wc,dc->ln_color);
  if (flag==1)
      for(i=0;i<n;i++) _uidraw_line3(G,px[i],py[i],(int)(pz[i]+0.5),px[(i+1)%n],py[(i+1)%n],(int)(pz[(i+1)%n]+0.5));
}


void _uigodard_fill3(DIG *G,short n,int *px,int *py,int *pz ,short flag,float *pv)
{
  short j,imd;
  short i,cnt,ind,flg;
  int xcord[15];
  int zcord[15];
  int xlow,xup;
  int txx,tzx,tzn,temp;
  int ymax[15],ymin[15],xmax[15],xmin[15],zmax[15],zmin[15];
  int tyx,tyn,txn,scan,ygrt;
  float pmax[15],pmin[15],pcord[15],tpx,tpn,tpp;
  unsigned int tempc;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  xup =-100000000;
  xlow = 100000000;
  scan=1000000000;
  ygrt=-123900000;
  i=0;flg=0;
  while((i<n)&&(flg==0)) {
      if ((pz[i]>=0)&&(pz[i]<=MAXZ))  flg=1;
      i++;
  }
  if (flg==0) return;
  temp =wc->c_color;
  for (i=0;i<n;++i) {
	imd=(i+1)%n;
	if(py[i]>py[imd])
	{
	  ymax[i]=py[i];
	  xmax[i]=px[i];
          zmax[i]=pz[i];
	  pmax[i]=pv[i];
	  ymin[i]=py[imd];
	  xmin[i]=px[imd];
          zmin[i]=pz[imd];
	  pmin[i]=pv[imd];
	}
	else
	{
	  ymin[i]=py[i];
	  xmin[i]=px[i];
	  zmin[i]=pz[i];
	  pmin[i]=pv[i];
	  ymax[i]=py[imd];
	  xmax[i]=px[imd];
	  zmax[i]=pz[imd];
	  pmax[i]=pv[imd];
	}
	if (ygrt < py[i]) ygrt = py[i];
	if (scan > py[i]) scan = py[i];
	if (xup  < px[i]) xup  = px[i];
	if (xlow > px[i]) xlow = px[i];
	j=i-1;
	tyx=ymax[i];
	tyn=ymin[i];
	txx=xmax[i];
	txn=xmin[i];
	tzx=zmax[i];
	tzn=zmin[i];
	tpx=pmax[i];
	tpn=pmin[i];
	while (j>=0 && tyx<ymax[j]) {
	  ymax[j+1]=ymax[j];
	  ymin[j+1]=ymin[j];
	  xmax[j+1]=xmax[j];
	  xmin[j+1]=xmin[j];
	  zmax[j+1]=zmax[j];
	  zmin[j+1]=zmin[j];
	  pmax[j+1]=pmax[j];
	  pmin[j+1]=pmin[j];
	  j--;
	}
	ymax[j+1]=tyx;
	ymin[j+1]=tyn;
	xmax[j+1]=txx;
	xmin[j+1]=txn;
	zmax[j+1]=tzx;
	zmin[j+1]=tzn;
	pmax[j+1]=tpx;
	pmin[j+1]=tpn;
  }
  ind=0;
  if((scan> wc->c_v_y2)||(ygrt< wc->c_v_y1)||(xlow> wc->c_v_x2)||(xup< wc->c_v_x1)) return; 
  while ((ind<n)&&(ymax[ind]==scan)) ind++;
  if ( scan == ygrt ) { /* ie a single line panel */
    for(i=0;i<n;i++) if(xmax[i]!=xmin[i])break;
    if(i==n) i=0; /* ie for single point panel */
    uigodr_line3(G,xmin[i],xmax[i],scan,zmin[i],zmax[i],pmin[i],pmax[i]);
    scan++;
  }
  else {
    while (scan<=ygrt) {
	 cnt=0;
         if( scan < ygrt ) {
           while ((ind<n)&&(ymax[ind]==scan)) ++ind; /* excluding upper edges */
         }
         if(scan<wc->c_v_y1) {scan++;continue;}
         if(scan>wc->c_v_y2) break;
	 for(i=ind;i<n;++i) {
	  if (ymin[i]<=scan) {
		 if (ymax[i]!=ymin[i])
		 {
                   xcord[cnt]=(((float)(xmax[i]-xmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +xmin[i])+0.5);
                   zcord[cnt]=(((float)(zmax[i]-zmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +zmin[i])+0.5);
                   pcord[cnt]=(((float)(pmax[i]-pmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +pmin[i]));
		  ++cnt;
		 }
	  }
	 }
	 /*sorting scan line intersection xcords of edges*/
	 for (i=0;i<cnt;++i) {
		j=i-1;
		txx=xcord[i];
		tzx=zcord[i];
		tpp=pcord[i];
		while(j>=0 && txx < xcord[j]) {
                     xcord[j+1]=xcord[j];
                     zcord[j+1]=zcord[j];
                     pcord[j+1]=pcord[j];j--;}
		xcord[j+1]=(txx);
		zcord[j+1]=(tzx);
		pcord[j+1]=(tpp);
	 }
	 /*displaying scan lines*/
	 for (i=0;i<cnt;i+=2)
  	 uigodr_line3(G,xcord[i],xcord[i+1],scan,zcord[i],zcord[i+1],pcord[i],pcord[i+1]);
	 scan++;
    }
  }
  wcset_clr(wc,dc->ln_color);
  if (flag==1)
  for(i=0;i<n;i++) _uidraw_line3(G,px[i],py[i],pz[i],px[(i+1)%n],py[(i+1)%n],pz[(i+1)%n]);
}
void uigodr_line3(DIG *G,int xa,int xb,int y,int za,int zb,float pa,float pb)
{
  short i,pos,p1,p2;
  char * addr;
  char tempc;
  float tempp,pval,pfac,yy;
  short clip;
  int temp,c_color;
  int x1,x2,z1,z2;
  double cons,m;
  int z;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  yy = wc->EVGAY-y;
  if((y>wc->c_v_y2) || (y<wc->c_v_y1) ) return;
  x1=xa;x2=xb;z1=za;z2=zb;
  if (((za<0)&&(zb<0))||((za>MAXZ)&&(zb>MAXZ))) return;
  clip=uiclip_line(wc,&xa,&y,&xb,&y);
  if (clip==0) return;
  p1 = pa+0.5;
  p2 = pb+0.5;
  if(p1<1) p1=dc->Cutoff;
  if(p2<1) p2=dc->Cutoff;
  if(p1>dc->max_clr) p1=dc->max_clr;
  if(p2>dc->max_clr) p2=dc->max_clr;
  if(xa==xb) { p2=p1; }
  if( p1==p2) {
        tempc=wc->c_color;
        if(p1> 0) c_color=p1+dc->min_clr;
        else c_color=0;
        wcset_clr(wc,c_color);
        uihori_line3(G,xa,xb,y,za,zb);
        c_color=tempc;
        wcset_clr(wc,c_color);
        return;
  }
  if (xb<xa) {
    temp=xa;
    xa=xb;
    xb=temp;
    tempp=pa;
    pa=pb;
    pb=tempp;
  }
  if (xa>wc->c_v_x2) return;
  if (xb<wc->c_v_x1) return;
  pfac = 0;
  if(xb != xa) pfac = (pb-pa)/(xb-xa);
  if(xa<wc->c_v_x1) {
       pval = pa+(wc->c_v_x1-xa)*pfac;
       xa= wc->c_v_x1;
       pa= pval;
       if(xb != xa) pfac = (pb-pa)/(xb-xa);
  }
  if(xb>wc->c_v_x2) {
       pval = pa+(wc->c_v_x2-xa)*pfac;
       xb= wc->c_v_x2;
       pb= pval;
       if(xb != xa) pfac = (pb-pa)/(xb-xa);
  }
  m = (double)(z2-z1)/(double)(x2-x1);
  cons = z1 - x1*m;
  za=z_inter(xa,m,cons);zb=z_inter(xb,m,cons);
  for(i=xa;i<=xb;i++) {
    temp=(char)(pa+(i-xa)*pfac+0.5);
    if(temp<1) temp=dc->Cutoff;
    if(temp>dc->max_clr) temp=dc->max_clr;
    if(temp > 0 ) temp +=dc->min_clr;
    wcset_clr(wc,temp);
    z=z_inter(i,m,cons);uiput_pix3(G,i,yy,z);
  }
  wcset_clr(wc,c_color);
}

void _uishade_fill3(DIG *G,short n,int *px,int *py,int *pz ,short flag,float *pv)
{
  short j,imd;
  short i,cnt,ind,flg;
  int xcord[15];
  int zcord[15];
  int xlow,xup;
  int txx,tzx,tzn,temp;
  int ymax[15],ymin[15],xmax[15],xmin[15],zmax[15],zmin[15];
  int tyx,tyn,txn,scan,ygrt;
  float pmax[15],pmin[15],pcord[15],tpx,tpn,tpp;
  unsigned int tempc;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  xup =-100000000;
  xlow = 100000000;
  scan=1000000000;
  ygrt=-123900000;
  i=0;flg=0;
  while((i<n)&&(flg==0)) {
      if ((pz[i]>=0)&&(pz[i]<=MAXZ))  flg=1;
      i++;
  }
  if (flg==0) return;
  temp =wc->c_color;
  for (i=0;i<n;++i) {
	imd=(i+1)%n;
	if(py[i]>py[imd])
	{
	  ymax[i]=py[i];
	  xmax[i]=px[i];
          zmax[i]=pz[i];
	  pmax[i]=pv[i];
	  ymin[i]=py[imd];
	  xmin[i]=px[imd];
          zmin[i]=pz[imd];
	  pmin[i]=pv[imd];
	}
	else
	{
	  ymin[i]=py[i];
	  xmin[i]=px[i];
	  zmin[i]=pz[i];
	  pmin[i]=pv[i];
	  ymax[i]=py[imd];
	  xmax[i]=px[imd];
	  zmax[i]=pz[imd];
	  pmax[i]=pv[imd];
	}
	if (ygrt < py[i]) ygrt = py[i];
	if (scan > py[i]) scan = py[i];
	if (xup  < px[i]) xup  = px[i];
	if (xlow > px[i]) xlow = px[i];
	j=i-1;
	tyx=ymax[i];
	tyn=ymin[i];
	txx=xmax[i];
	txn=xmin[i];
	tzx=zmax[i];
	tzn=zmin[i];
	tpx=pmax[i];
	tpn=pmin[i];
	while (j>=0 && tyx<ymax[j]) {
	  ymax[j+1]=ymax[j];
	  ymin[j+1]=ymin[j];
	  xmax[j+1]=xmax[j];
	  xmin[j+1]=xmin[j];
	  zmax[j+1]=zmax[j];
	  zmin[j+1]=zmin[j];
	  pmax[j+1]=pmax[j];
	  pmin[j+1]=pmin[j];
	  j--;
	}
	ymax[j+1]=tyx;
	ymin[j+1]=tyn;
	xmax[j+1]=txx;
	xmin[j+1]=txn;
	zmax[j+1]=tzx;
	zmin[j+1]=tzn;
	pmax[j+1]=tpx;
	pmin[j+1]=tpn;
  }
  ind=0;
  if((scan> wc->c_v_y2)||(ygrt< wc->c_v_y1)||(xlow> wc->c_v_x2)||(xup< wc->c_v_x1)) return; 
  while ((ind<n)&&(ymax[ind]==scan)) ind++;
  if ( scan == ygrt ) { /* ie a single line panel */
    for(i=0;i<n;i++) if(xmax[i]!=xmin[i])break;
    if(i==n) i=0; /* ie for single point panel */
    uishade_line3(G,xmin[i],xmax[i],scan,zmin[i],zmax[i],pmin[i],pmax[i]);
    scan++;
  }
  else {
    while (scan<=ygrt) {
	 cnt=0;
         if( scan < ygrt ) {
           while ((ind<n)&&(ymax[ind]==scan)) ++ind; /* excluding upper edges */
         }
         if(scan<wc->c_v_y1) {scan++;continue;}
         if(scan>wc->c_v_y2) break;
	 for(i=ind;i<n;++i) {
	  if (ymin[i]<=scan) {
		 if (ymax[i]!=ymin[i])
		 {
                   xcord[cnt]=(((float)(xmax[i]-xmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +xmin[i])+0.5);
                   zcord[cnt]=(((float)(zmax[i]-zmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +zmin[i])+0.5);
                   pcord[cnt]=(((float)(pmax[i]-pmin[i])/(ymax[i]-ymin[i])
                              *(scan-ymin[i]) +pmin[i]));
		  ++cnt;
		 }
	  }
	 }
	 /*sorting scan line intersection xcords of edges*/
	 for (i=0;i<cnt;++i) {
		j=i-1;
		txx=xcord[i];
		tzx=zcord[i];
		tpp=pcord[i];
		while(j>=0 && txx < xcord[j]) {
                     xcord[j+1]=xcord[j];
                     zcord[j+1]=zcord[j];
                     pcord[j+1]=pcord[j];j--;}
		xcord[j+1]=(txx);
		zcord[j+1]=(tzx);
		pcord[j+1]=(tpp);
	 }
	 /*displaying scan lines*/
	 for (i=0;i<cnt;i+=2)
  	 uishade_line3(G,xcord[i],xcord[i+1],scan,zcord[i],zcord[i+1],pcord[i],pcord[i+1]);
	 scan++;
    }
  }
  wcset_clr(wc,dc->ln_color);
  if (flag==1)
  for(i=0;i<n;i++) _uidraw_line3(G,px[i],py[i],pz[i],px[(i+1)%n],py[(i+1)%n],pz[(i+1)%n]);
}
void uishade_line3(DIG *G,int xa,int xb,int y,int za,int zb,float pa,float pb)
{
  short i,pos,p1,p2;
  char * addr;
  char tempc;
  float tempp,pval,pfac,yy;
  short clip;
  int temp;
  int x1,x2,z1,z2;
  double cons,m;
  int z;
  kgWC *wc;
  kgDC *dc;
  wc = G->wc;
  dc=G->dc;
  yy = wc->EVGAY-y;
  if((y>wc->c_v_y2) || (y<wc->c_v_y1) ) return;
  x1=xa;x2=xb;z1=za;z2=zb;
  if (((za<0)&&(zb<0))||((za>MAXZ)&&(zb>MAXZ))) return;
  clip=uiclip_line(wc,&xa,&y,&xb,&y);
  if (clip==0) return;
  if(xa==xb) { pa=pb; }
  if (xb<xa) {
    temp=xa;
    xa=xb;
    xb=temp;
    tempp=pa;
    pa=pb;
    pb=tempp;
  }
  if (xa>wc->c_v_x2) return;
  if (xb<wc->c_v_x1) return;
  pfac = 0;
  if(xb != xa) pfac = (pb-pa)/(xb-xa);
  if(xa<wc->c_v_x1) {
       pval = pa+(wc->c_v_x1-xa)*pfac;
       xa= wc->c_v_x1;
       pa= pval;
       if(xb != xa) pfac = (pb-pa)/(xb-xa);
  }
  if(xb>wc->c_v_x2) {
       pval = pa+(wc->c_v_x2-xa)*pfac;
       xb= wc->c_v_x2;
       pb= pval;
       if(xb != xa) pfac = (pb-pa)/(xb-xa);
  }
  if(x2==x1) m = 0;
  else m = (double)(z2-z1)/(double)(x2-x1);
  cons = z1 - x1*m;
  za=z_inter(xa,m,cons);zb=z_inter(xb,m,cons);
  for(i=xa;i<=xb;i++) {
    tempp=(pa+(i-xa)*pfac);
    z=z_inter(i,m,cons);uiput_shade_pix3(G,i,yy,z,tempp);
  }
}

