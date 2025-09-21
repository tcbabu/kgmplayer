void uiString(DIALOG *D,char *str,int x,int y,int width,int height,int font,int color,int FontSize,int justfic,int bkcolor){
/*
   Write a string in Dialog Area;
   -1 left justification
    0 center
    1 right justification
    bkcolor : background color ; < 0 background will not be painted
*/
   int ln,i,maxchar,temp;
   void *img=NULL;
   float length;
   kgWC *wc;
   wc = D->wc;
   ln = width;
   if(str==NULL) return;
   if(str[0]=='\0') return;
   img = kgStringToImage(str,NULL,ln,height,font,color,justfic,FontSize,bkcolor);
   if(img != NULL) {
     kgImage(D,img,x,y,ln,height,0.0,1.0);
     uiFreeImage(img);
   }
   else printf("img == NULL\n");
}
