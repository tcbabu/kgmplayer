int gphRoundedRectangleShade4(int fid,float xo,float yo,float xl,float yl,int clr,float fac,float vmin,float vmax) {
   /* xo,yo is the centre of the rectangle */
   int count,Dang=3,count1,count2,hcount,np,pos,size,size1;
   float bordrsize=3;
   float x[800],y[800],v[800],xx,yy;
   float xd[800],yd[800],vd[800];
   float dl,xp,yp,red,green,blue,h,s,vv,vorg,vmid,vlow,horg,sorg,vlow1,vmid1,fac1;
   double dl1;
   double ang,dang;
   int i,j,npi,npj,np2;
   int r,g,b;
   if(fac > 1.0) fac=1.0;
   if(fac > 0.25 ) Dang=2;
   if(fac > 0.4 ) Dang=1;
   count = 90/Dang;
   if(fac==0.0) {count=1;Dang=90;}
   size = yl;
   if(size > xl){ size=xl;}
   bordrsize = size*0.1;
   if(bordrsize< 4) bordrsize=4;
   if(bordrsize > size*0.3) bordrsize=size*0.3;
   dl = size*fac;
   if(dl < bordrsize ) dl = bordrsize;
   dl1 = dl -bordrsize;
   fac1 = dl1/(size-2*bordrsize);
//   fac1=0.5;
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
   vmid = vmax;
   vv = (vmin+0.4*(vmax-vmin));
//   if(vmid>vorg) vmid=vorg;
   vlow= vmin;
   if(vv > 1 ) vv=1;
   if(vlow > 1 ) vlow=1;
   if(fac > 0.5) {
       double ss,dv,vmin,vmax,dv1,vmin1,vmax1,ang;
       vmin1= vv;
       vmax1 = vv;
       vmin = vlow;
       vmax = vlow;
       fac = (fac-0.5)/0.5;
       fac1 = (fac1-0.5)/0.5;
       np = kgGetEllipsePoints(xl, yl,fac,count,x,y) - 2;
       np =kgGetEllipsePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, count,xd, yd)-2;
       dv = (vmax-vmin);
       dv1 = (vmax1-vmin1);
       Dang = 90.0/(np-1);
       dang = Dang*3.14159265/180.0;
 
       ang =0;
       for(i=0;i<np;i++) {
         ss = sin(ang);
         v[i]= vmin+ss*ss*dv;
         vd[i]= vmid+ss*ss*dv1;
         ang +=dang;
       }
       count = np;
       count2 = count+2;
       count1=  count+1;
   }
   else {
       np =kgGetCirclePoints( xl,yl,fac, x, y, v,vlow,vlow, count);
       np =kgGetCirclePoints( xl-2*bordrsize,yl-2*bordrsize,fac1, xd, yd, vd,vmid,vmid, count);
   }
   for(i=0;i<np;i++) {
     npi= np+i;
     npj = np-1-i;
     x[npi]= xd[npj];
     y[npi]= yd[npj];
     v[npi]= vd[npj];
   }
   np2 = np*2;
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[i];
   }
   gphMixHsvColors(fid,200,128,h,s,vmin,vmax);
   gphSetShadeParams(fid,vmax,vmin,200, 128, 0);
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i]= xo;
     yd[i]= yo;
     vd[i]= vv;
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[np-1];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[np-1];
//   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= xo;
   yd[0]= yo;
   vd[0]= vv;
   xd[3]= xo;
   yd[3]= yo;
   vd[3]= vv;
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np-1];
//     vd[i] = v[np];
   }
   for(i=np;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = y[i] +yo;
     vd[i] = v[np];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i]= xo;
     yd[i]= yo;
     vd[i]= vv;
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= -x[0];
   yd[0]= y[0];
   vd[0]= v[np-1];
//   vd[0]= v[np];
   xd[1]= -x[0];
   yd[1]= -y[0];
   vd[1]= v[np-1];
   xd[2]= -x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np];
   xd[3]= -x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= xo;
   yd[0]= yo;
   vd[0]= vv;
   xd[1]= xo;
   yd[1]= yo;
   vd[1]= vv;
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= x[0];
   yd[0]= y[0];
   vd[0]= v[0];
   xd[1]= x[0];
   yd[1]= -y[0];
   vd[1]= v[0];
   xd[2]= x[np2-1];
   yd[2]= -y[np2-1];
   vd[2]= v[np2-1];
   xd[3]= x[np2-1];
   yd[3]=  y[np2-1];
   vd[3]= v[np2-1];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= xo;
   yd[0]= yo;
   vd[0]= vv;
   xd[1]= xo;
   yd[1]= yo;
   vd[1]= vv;
   gphGouraudFill(fid,4,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = -x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[np-1-i];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1-i];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np;i++) {
     xd[i]= xo;
     yd[i]= yo;
     vd[i]= vv;
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   for(i=0;i<np2;i++) {
     xd[i] = x[i]+xo;
     yd[i] = -y[i] +yo;
   }
   for(i=0;i<np;i++) {
     vd[i]= v[0];
   }
   for(i=0;i<np;i++) {
     vd[np+i]= v[np2-1];
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
//   gphPolyFill(fid,np,x,y,0,clr);
   for(i=0;i<np;i++) {
     xd[i]= xo;
     yd[i]= yo;
     vd[i]= vv;
   }
   gphGouraudFill(fid,np2,xd,yd,vd);
   xd[0]= x[np-1];
   yd[0]= y[np-1];
   vd[0]= v[0];
   xd[1]= x[np];
   yd[1]= y[np];
   vd[1]= v[np2-1];
   xd[2]= -x[np];
   yd[2]= y[np];
   vd[2]= v[np2-1];
   xd[3]= -x[np-1];
   yd[3]= y[np-1];
   vd[3]= v[0];
   for(i=0;i<4;i++) {
     xd[i] = xd[i]+xo;
     yd[i] = -yd[i] +yo;
   }
   gphGouraudFill(fid,4,xd,yd,vd);
   xd[0]= xo;
   yd[0]= yo;
   vd[0]= vv;
   xd[3]= xo;
   yd[3]= yo;
   vd[3]= vv;
   gphGouraudFill(fid,4,xd,yd,vd);
   return 1;
}
