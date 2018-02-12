#include <kulina.h>
void * kgUndoiImage(int size,int red,int green,int blue) {
   void *fid,*img=NULL;
   int count,count1size1;
   float x[800],y[800],v[800],R;
   int r,g,b,np=120;
   double Dang,dang,ang,ri,ro,ss,cs;
   count = 2*np+3;
   count1= cunt-1;
   Dang = 315.0/(np-1);
   dang = Dang*3.14159265/180.0;
   R = size*0.501;
   size1= size*0.1;
   ri= size*0.5-2*size1;
   ro = ri+size1;
   fid = kgInitImage(size,size,4);
   kgUserFrame(fid,-R,-R,+R,+R);
   kgChangeColor(1001,red,green,blue);
   ang =3.14159265+dang;
   for(i=0;i<np;i++) {
         ss = sin(ang);
         cs = cos(ang);
         x[i]= ri*cs;
         y[i]= ri*ss;
         x[count1-i]=ro*cs;
         y[count1-i]=ro*ss;
         ang +=dang;
   }
   x[np]=(ri-size1)*cs;
   y[np]=(ri-size1)*ss;
   x[np+2]=(ro+size1)*cs;
   y[np+2]=(ro+size1)*ss;
   ss = sin(ang);
   cs = cos(ang);
   x[np+1]=(ri+size1*0.5)*cs;
   y[np+1]=(ri+size1*0.5)*ss;
   kgPolyFill(fid,count,x,y,0,1001);
   img = kgGetResizedImage(fid);
   kgCloseImage(fid);
   return img;
}
int main(void) {
  void *img;
  img = kgUndoImage(64,255,0,0);
  uiWriteImage("Junk.png",img);
}
