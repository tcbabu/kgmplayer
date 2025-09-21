//function plot(x, y, c) is
//     plot the pixel at (x, y) with brightness c (where 0 ≤ c ≤ 1)
 
#define Ipart(x) ((int)(x))
#define Round(x) ((int)(x+0.5))
#define Fpart(x) ((x)-Ipart(x))
#define Rfpart(x) ( 1-Fpart(x))
#define Swap(x,y) {temp=x;x=y;y=temp;}
void _uialine (int x0,int y0,int x1,int y1){
/*
  Wu's algorithm
*/
   int steep,xend,yend,xpxl1,ypxl1,temp;
   int xpxl2,ypxl2,x;
   float dx,dy,gradient,xgap,intery;
   steep= (abs(y1 - y0) > abs(x1 - x0))?1:0;
   if(steep) {Swap(x0,y0);Swap(x1,y1);}
   if(x0 > x1) {Swap(x0,y0);Swap(x1,y1);}
   dx = x1 - x0;
   dy = y1 - y0;
   gradient = dy / dx;
   xend = Round(x0);
   yend = y0 + gradient * (xend - x0);
   xgap = Rfpart(x0 + 0.5);
   xpxl1 = xend ;
   ypxl1 = Ipart(yend);
   if (steep) { 
         plot(ypxl1,   xpxl1, Rfpart(yend) * xgap);
         _uiPlotPixel(ypxl1+1, xpxl1,  Fpart(yend) * xgap);
   } 
   else {
         _uiPlotPixel(xpxl1, ypxl1  , Rfpart(yend) * xgap);
         _uiPlotPixel(xpxl1, ypxl1+1,  Fpart(yend) * xgap);
   }
   intery = yend + gradient ;
   xend = Round(x1);
   yend = y1 + gradient * (xend - x1);
   xgap = Fpart(x1 + 0.5);
   xpxl2 = xend;
   ypxl2 = Ipart(yend);
   if (steep) {
        _uiPlotPixel(ypxl2  , xpxl2, Rfpart(yend) * xgap);
        _uiPlotPixel(ypxl2+1, xpxl2,  Fpart(yend) * xgap);
   }
   else {
       _uiPlotPixel(xpxl2, ypxl2,  Rfpart(yend) * xgap);
       _uiPlotPixel(xpxl2, ypxl2+1, Fpart(yend) * xgap);
   }
   for (x =xpxl1 + 1 ;x<=xpxl2 - 1;x++ ) {
         if (steep) {
             _uiPlotPixel(Ipart(intery)  , x, Rfpart(intery));
             _uiPlotPixel(Ipart(intery)+1, x,  Fpart(intery));
         }
         else {
             _uiPlotPixel(x, Ipart (intery),  Rfpart(intery));
             _uiPlotPixel(x, Ipart (intery)+1, Fpart(intery));
         }
         intery = intery + gradient;
   }
}
