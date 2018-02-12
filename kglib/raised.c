int kgGetCirclePoints(int xl,int yl,float fac,float *x,float *y,float *v,float vmin,float vmax,int count) {
   int count1,count2,hcount;
   float dl,xp,yp;
   double ang,dang,Dang,dv,ss;
   dl = fac*yl;
   if(dl > fac*xl){ dl=fac*xl;}
   dv = vmax-vmin;
   Dang = 90.0/count;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   xp = xl*0.5 -dl;
   yp = yl*0.5 - dl;
   x[0]=dl +xp;
   y[0]= yp;
   for(i=1;i<=count;i++) {
     ang +=dang;
     ss = sin(ang);
     cs = cos(ang);
     x[i]= dl*cs+xp;
     y[i]= dl*ss+yp;
     v[i]= vmin+ss*dv;
   }
   for(i=0;i<=count;i++) {
     x[count1+i]= -x[count-i];
     y[count1+i] = y[count-i];
     v[i]= vmax;
   }
   for(i=0;i<(count1);i++) {
     x[2*count1+i]= x[2*count1-1-i];
     y[2*count1+i] = -y[2*count1-1-i];
     v[i]= v[count-i];
   }
   for(i=0;i<(count1);i++) {
     x[3*count1+i]=  -x[3*count1-1-i];
     y[3*count1+i] = y[3*count1-1-i];
     v[i]= vmin;
   }
   x[4*count1]=x[0];
   y[4*count1]=y[0];
   v[4*count1]=v[0];
   return (count1*4+1);
}
int gphRoundedRectangleRaised(int fid,float xo,float yo,float xl,float yl,int clr,float fac) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np,pos,bordrsize=3,size,size1;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1;
   double ang,dang;
   int i,j,r,g,b,npi,npj;
   if(fac > 0.5) fac=0.5;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=0;Dang=90;}
   dl = fac*yl;
   size = yl;
   if(dl > fac*xl){ dl=fac*xl;size=xl;}
   size=dl;
   size1 = size -2*bordrsize;
   fac1 = (1-size*size1*(1-2*fac))*0.5;
   dang = Dang*3.14159265/180.0;
   ang=0;
   count1 = count+1;
   count2 = count+2;
   kgGetDefaultRGB(clr,&r,&g,&b);
//TCB

//   r=g=b=220;
   RGBtoHSV((float)r,(float)g,(float)b,&h,&s,&vv);
   horg = h;
   sorg =s;
   vorg = vv;
   if(vv > 0.9)  { s = 0.5*sorg; }
   vv =vorg +(1.-vorg)*0.6;
   if(vv > 1 ) vv=1;
   vmid = vorg +(1-vorg)*0.55;
   vmid1 = vorg +(1-vorg)*0.45;
   if(vmid > 1 ) vmid=1;
   if(vmid1 > 1 ) vmid1=1;

   vlow = vorg*0.7;
   vlow1 = vorg*0.50;
#if 0
   if(fac > 0.5) {
       if(fac > 1.0) fac=1.0;
       fac = (fac-0.5)/0.5;
       count = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 3;
       count2 = count+2;
       count1=  count+1;
   }
   else {
     dang = Dang*3.14159265/180.0;
     ang=0;
     xp = xl*0.5 -dl;
     yp = yl*0.5 - dl;
     x[0]=dl +xp;
     y[0]= yp;
     for(i=1;i<=count;i++) {
       ang +=dang;
       x[i]= dl*cos(ang)+xp;
       y[i]= dl*sin(ang)+yp;
     }
   }
   for(i=0;i<=count;i++) {
     x[count1+i]= -x[count-i];
     y[count1+i] = y[count-i];
   }
   for(i=0;i<(2*count1);i++) {
     x[2*count1+i]= x[2*count1-1-i];
     y[2*count1+i] = -y[2*count1-1-i];
   }
#else
   np =kgGetCirclePoints( xl,yl,fac, *x, *y, *v,vlow1,vmid1, count) {
   np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, *xd, *yd, *vd,vlow,vv, count) {
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
#endif
   np = np*2;
   for(i=0;i<np;i++) {
     x[i] +=xo;
     y[i] +=yo;
   }
   hcount = count1/4;
   if(vorg > 0.8)HSVtoRGB(&red,&green,&blue,h,s,vorg*0.6);
   else HSVtoRGB(&red,&green,&blue,h,s,0.4);
   HSVtoRGB(&red,&green,&blue,h,s,vorg*0.6);
   gphChangeColor(fid,245,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vlow1);
   gphChangeColor(fid,246,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vlow);
   gphChangeColor(fid,247,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid1);
   gphChangeColor(fid,248,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vmid);
   gphChangeColor(fid,249,(int)red,(int)green,(int)blue);
   HSVtoRGB(&red,&green,&blue,h,s,vv);
   gphChangeColor(fid,250,(int)red,(int)green,(int)blue);
   gphLineColor(fid,246);
   gphLineWidth(fid,4);
   gphMove2f(fid,x[0],y[0]);
#if 0
   for(i=1;i<hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos=hcount;
   gphLineColor(fid,247);
   for(i=pos;i<pos+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos+=hcount;
   gphLineColor(fid,248);
   for(i=pos;i<pos+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos+=hcount;
   gphLineColor(fid,249);
   for(i=pos;i<count1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos=count1;
   gphLineWidth(fid,6);
   gphLineColor(fid,250);
   for(i=pos;i< (pos+count1+1);i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   gphLineWidth(fid,4);
   pos =(2*count1+1);
   gphLineColor(fid,249);
   for(i=pos;i<pos+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos +=hcount;
   gphLineColor(fid,248);
   for(i=pos;i<pos+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos +=hcount;
   gphLineColor(fid,247);
   for(i=pos;i<pos+hcount;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos +=hcount;
   gphLineColor(fid,246);
   for(i=pos;i< 3*count1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
   pos = 3*count1;
   gphLineColor(fid,245);
   for(i=pos;i<4*count1;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
#else
   for(i=1;i<np;i++) {
     gphDraw2f(fid,x[i],y[i]);
   }
#endif
   gphDraw2f(fid,x[0],y[0]);
   return 1;
}
