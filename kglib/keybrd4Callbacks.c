#include "kulina.h"
static int NOBKGR=1;
//static int Bx=72,By=48,Xg=4,Yg=4;
//static int Bxoff=2,Byoff=2,Bodr=10;
//static int Bxoff=2,Byoff=2;
static char BUFF[100];

static char Label1[]="`1234567890-=";
static char Label2[] ="qwertyuiop[]\\";
static char Label3[] = "asdfghjkl;'";
static char Label4[] = "zxcvbnm";
static char Label6[]="~!@#$%^&*()_+";
static char Label7[] ="QWERTYUIOP{}|";
static char Label8[] = "ASDFGHJKL:\"";
static char Label9[] = "ZXCVBNM";
static char Label10[] = "[]\\";
static char Label11[] = ";'";
static char Label12[] = ",./";
static char Label13[] = "{}|";
static char Label14[] = ":\"";
static char Label15[] = "<>?";
void kgModifyTextWidget(void *Tmp,int ch);
static void *CreateButImage4(void *Tmp);
static int kgProcessInit4(DIN *B,KEYBRD *Kbrd,char *Label,char *Label1,int inx) ;
typedef struct _butimgdata {
  void *B;
  void *kb;
  int index;
  char str[100];
  char ustr[100];
} IMDT;
static Dlink *XpmList=NULL;
#if 0
int kgShowKeybrd4(void *Tmp) {
   DIALOG *D;
   KEYBRD *Kbrd;
   D = (DIALOG *)Tmp;
   Kbrd = (KEYBRD *)D->Kbrd;
   if(Kbrd->Vis) return 0;
    kgSetGrpVisibility(D,Kbrd->grp2,0);
    kgSetGrpVisibility(D,Kbrd->cgrp,0);
    kgSetGrpVisibility(D,Kbrd->ongrp,0);
    kgSetGrpVisibility(D,Kbrd->grp1,1);
    kgSetGrpVisibility(D,Kbrd->grp3,1);
    kgSetGrpVisibility(D,Kbrd->sgrp,1);
    kgSetGrpVisibility(D,Kbrd->offgrp,1);
   Kbrd->ShiftPress=0;
   Kbrd->CapsLock=0;
   if(D->wc != NULL) {
     kgUpdateGrp(D,Kbrd->grp1);
     kgUpdateGrp(D,Kbrd->grp3);
     kgUpdateGrp(D,Kbrd->sgrp);
     kgUpdateGrp(D,Kbrd->offgrp);
     kgUpdateOn(D);
   }
   Kbrd->Vis = 1;
   return Kbrd->GrpId;
}
#endif
static void *MakeBkgrImage(int xl,int yl,int r,int g,int b,float fac) {
   void *Img,*Img1;
   float h,s,v,rf,gf,bf,vm;
   DIG *fid;
   if(fac<= 0.00001) {
     fid = (DIG *)kgInitImage(xl,yl,1);
     kgUserFrame(fid,0.,0.,(float)xl,(float)yl);
     kgChangeColor(fid,551,(int)r,(int)g,(int)b);
     kgBoxFill(fid,0.,0.,xl,yl,551,0);
   }
   else {
     fid = (DIG *)kgInitImage(xl,yl,5);
     kgUserFrame(fid,0.,0.,(float)xl,(float)yl);
     kgChangeColor(fid,551,(int)r,(int)g,(int)b);
     kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,
                        (float)xl,(float)yl,0,551,fac);
   }
   Img = kgGetSharpImage(fid);
   kgCloseImage(fid);
   return Img;
}
static void *MakeStringImage4(int xl,int yl,int bclr,float fac, int tc,int utc,int font,char*str,char *ustr) {
   void *Img=NULL,*Img1=NULL;
   float h,s,v,rf,gf,bf,vm,w,xp,yp;
   int R,G,B;
   float ln;
   DIG *fid;
   
   
   if(fac<= 0.00001) {
     fid = (DIG *)kgInitImage(xl,yl,5);
     kgUserFrame(fid,-1.0,-1.0,(float)xl+1.0,(float)yl+1.0);
     kgBoxFill(fid,0.,0.,xl,yl,bclr,0);
   }
   else {
     fid = (DIG *)kgInitImage(xl,yl,5);
     kgUserFrame(fid,-1.0,-1.0,(float)xl+1.0,(float)yl+1.0);
     kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,
                        (float)xl,(float)yl,0,bclr,fac);
   }
//   kgGetDefaultRGB(tc,&R,&G,&B);
//   kgChangeColor(fid,tc,R,G,B);
   kgTextColor(fid,tc);
   kgTextFont(fid,font);
   h = yl/2.8;
   w = 0.8*h;
   xp= 2*w;
   yp= (yl-h)/2.0;
   kgTextSize(fid,h,w,0.);
   ln = kgStringLength(fid,str);
   if(ln> (xl-w) ) {
     float fac;
     fac = (xl-w)/ln;
     w = w*fac;
     kgTextSize(fid,h,w,0.);
     ln = kgStringLength(fid,str);
   }
   xp = ((float)xl -ln)*0.5;
   kgMove2f(fid,xp,yp);
   kgWriteText(fid,str);
   if((ustr != NULL)&&(ustr[0]!='\0')) {
     h = yl/3.5;
     w = h;
     xp= xl-1.5*w;
     yp= yl -1.2*h;
//     kgGetDefaultRGB(utc,&R,&G,&B);
//     kgChangeColor(fid,utc,R,G,B);
     kgTextColor(fid,utc);
     kgTextSize(fid,h*0.7,w*0.7,0.);
     kgMove2f(fid,xp,yp);
     kgWriteText(fid,ustr);
   }
//   Img = kgGetSharpImage(fid);
//   Img = kgGetSmoothImage(fid);
   Img = kgGetResizedImage(fid);
   kgCloseImage(fid);
   return Img;
}
static char *kgButtonTitle1(char * str,void *pt,char *ustr) {
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)pt;
//  sprintf(BUFF,"!h15!d!h51!z%-s!f%2.2d!c%2.2d%-s!g !c%2.2d!z23!u%-s",Kbrd->Sfac,Kbrd->Bfont,Kbrd->Bclr,str,Kbrd->FillClr,ustr);
  sprintf(BUFF,"!z%-s!f%2.2d!c%2.2d%-s!c%2.2d!u %-s",Kbrd->Sfac,Kbrd->Bfont,Kbrd->Bclr,str,Kbrd->Bclr,ustr);
//  printf("%s\n",BUFF);
  return BUFF;
}
static char *Procpy1(char *des,char *src,void *pt,char *ustr) {
   strcpy(des,kgButtonTitle1(src,pt,ustr));
//   strcpy(des,src);
//   strcat(des,"!u ");
//   strcat(des,ustr);
   return des;
}
static char *kgButtonTitle(char * str,void *pt) {
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)pt;
//  sprintf(BUFF,"!h15!d!h51!z%-s!f%2.2d!c%2.2d%-s!g !g",Kbrd->Sfac,Kbrd->Bfont,Kbrd->Bclr,str);
  sprintf(BUFF,"!z%-s!f%2.2d!c%2.2d %-s!g !g",Kbrd->Sfac,Kbrd->Bfont,Kbrd->Bclr,str);
//  printf("%s\n",BUFF);
  return BUFF;
}
static char *Procpy(char *des,char *src,void *pt) {
   strcpy(des,kgButtonTitle(src,pt));
//   strcpy(des,src);
   return des;
}

static int kgProcessShift(void *Tmp) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)D->Kbrd;
 switch(Kbrd->ShiftPress) {
    case 0:
      switch(Kbrd->CapsLock) {
        case 0: 
          kgSetGrpVisibility(D,Kbrd->ongrp,0);
          kgSetGrpVisibility(D,Kbrd->grp2,0);
          kgSetGrpVisibility(D,Kbrd->cgrp,0);
          kgSetGrpVisibility(D,Kbrd->grp3,0);
          kgSetGrpVisibility(D,Kbrd->grp1,1);
          kgSetGrpVisibility(D,Kbrd->sgrp,1);
          kgSetGrpVisibility(D,Kbrd->grp3,1);
          kgSetGrpVisibility(D,Kbrd->offgrp,1);
          break;
        case 1: 
          kgSetGrpVisibility(D,Kbrd->offgrp,0);
          kgSetGrpVisibility(D,Kbrd->grp3,0);
          kgSetGrpVisibility(D,Kbrd->sgrp,0);
          kgSetGrpVisibility(D,Kbrd->grp3,0);
          kgSetGrpVisibility(D,Kbrd->grp1,1);
          kgSetGrpVisibility(D,Kbrd->cgrp,1);
          kgSetGrpVisibility(D,Kbrd->ongrp,1);
          kgSetGrpVisibility(D,Kbrd->grp3,1);
          break;
      }
      break;
    case 1:
      switch(Kbrd->CapsLock) {
        case 1: 
          kgSetGrpVisibility(D,Kbrd->offgrp,0);
          kgSetGrpVisibility(D,Kbrd->grp3,0);
          kgSetGrpVisibility(D,Kbrd->cgrp,0);
          kgSetGrpVisibility(D,Kbrd->grp2,0);
          kgSetGrpVisibility(D,Kbrd->grp1,1);
          kgSetGrpVisibility(D,Kbrd->sgrp,1);
          kgSetGrpVisibility(D,Kbrd->grp2,1);
          kgSetGrpVisibility(D,Kbrd->ongrp,1);
          break;
        case 0: 
          kgSetGrpVisibility(D,Kbrd->ongrp,0);
          kgSetGrpVisibility(D,Kbrd->grp3,0);
          kgSetGrpVisibility(D,Kbrd->sgrp,0);
          kgSetGrpVisibility(D,Kbrd->grp2,0);
          kgSetGrpVisibility(D,Kbrd->grp1,1);
          kgSetGrpVisibility(D,Kbrd->cgrp,1);
          kgSetGrpVisibility(D,Kbrd->grp2,1);
          kgSetGrpVisibility(D,Kbrd->offgrp,1);
          break;
      }
     break;
 }
 kgUpdateOn(D);
 return 1;
}
static int kgProcessCapsLock(void *Tmp) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)D->Kbrd;
  Kbrd->ShiftPress=0;
  kgProcessShift(Tmp);
  return 1;
}
static int kgProcessLabel(void *Tmp,int butno,char *Label) {
  int ret=1;
  int CurWid;
  char ch;
  DIALOG *D;
  KEYBRD *Kbrd;
  D = (DIALOG *)Tmp;
  Kbrd = (KEYBRD *)D->Kbrd;
  Kbrd->CurWid =-1;
  CurWid = Kbrd->CurWid;
  if(D->InputWid >= 0) {
     Kbrd->CurWid = D->InputWid;
     CurWid = Kbrd->CurWid;
  }
  if(CurWid>= 0)  {
    kgSetCurrentWidget(D,CurWid);
    ch= Label[butno-1];
//    kgSendKeyEvent(D,ch);
    kgModifyTextWidget(D,ch);
  }
  else  {
    if(Kbrd->TargetWindow != NULL) {
//      kgSetInputFocus(D,Kbrd->TargetWindow);
      ch= Label[butno-1];
      kgSendKeyToWindow(D,Kbrd->TargetWindow,ch);
    }
    ret=0;
  }
  if(Kbrd->ShiftPress) {
    Kbrd->ShiftPress=0;
    kgProcessShift(D);
  }
  return ret;
}
static void *GetButImg(DIN *B,KEYBRD *kb) {
   int xl,yl;
   int r,g,b;
   int clr;
   if(B->type > 0){
       return NULL;
   }
   xl =B->lngth;
   yl =B->width;
   clr =kb->ButClr;
   if(clr < -1) {
     clr = -clr;
     b = (clr%1000)%1000; // Funny single operator failed
     g = (clr/1000)%1000;
     r = clr/1000000;
   }
   else {
     if(clr == -1) clr= 1;
     kgGetDefaultRGB(clr,&r,&g,&b);
   }
   return MakeBkgrImage(xl,yl,r,g,b,B->fac);
}
static void *GetStringImg4(DIN *B,int index,KEYBRD *kb,char *str,char *ustr) {
   int xl,yl;
   int r,g,b;
   int clr;
   void *img=NULL;
   BUT_STR * but;
   but = (BUT_STR *)B->buts;
   if(B->type > 0) {
       but[index].xpmn=img;
       return NULL;
   }
   xl =B->lngth;
   yl =B->width;
//   xl = (xl+1)/2*2-1;
//   yl = (yl+1)/2*2-1;
   clr =kb->ButClr;
//   img = MakeStringImage4(xl,yl,clr,B->fac,kb->Bclr,kb->FillClr,kb->Bfont,str,ustr);
   img = MakeStringImage4(xl,yl,clr,B->fac,kb->Bclr,kb->Bclr,kb->Bfont,str,ustr);
   
   but[index].xpmn=img;
   return img;
}
static void *CreateButImage4(void *Tmp) {
    IMDT *im;
    im = (IMDT *)(Tmp);
    DIN *B;
    KEYBRD *kb;
    B = (DIN *)im->B;
    kb = (KEYBRD *)im->kb;
    GetStringImg4(B,im->index,kb,im->str,im->ustr);
    free(Tmp);
    return NULL;
  
}
static void kgProcessTitle(DIN *B,void *pt){
   int i,n;
   IMDT *im;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)pt;
   char buff[100];
   BUT_STR * but;
   but = (BUT_STR *)B->buts;
   n = B->nx * B->ny;
   B->type = Kbrd->Btype;
   B->fac = Kbrd->Rfac;
   for(i=0;i<n;i++) {
    but[i].bkgr = Kbrd->ButClr;
    if(B->type > 0)  {
      strcpy(buff,but[i].title);
      Procpy(but[i].title,buff,Kbrd);
      but[i].xpmn = NULL;
    }
    else {
      im = (IMDT *) Malloc(sizeof(IMDT));
      if(im==NULL) {
        fprintf(stderr,"Failed in Malloc\n");
      }
      else {
      im->B = B;
      im->kb = Kbrd;
      strncpy(im->str,but[i].title,80);
      im->str[80]='\0';
      im->ustr[0]='\0';
      im->index = i;
#if 1
      if(Kbrd->Thds != NULL) DoInAnyThread(Kbrd->Thds,CreateButImage4,(void *)im);
      else CreateButImage4((void *)(im));
#else
      CreateButImage4((void *)(im));
#endif
      }
      but[i].title[0]='\0';
    }
   }
   if(Kbrd->Thds != NULL) WaitThreads(Kbrd->Thds);
   for(i=0;i<n;i++) {
     Dadd(XpmList,but[i].xpmn);
   }
}
static int kgProcessInit4(DIN *B,KEYBRD *Kbrd,char *Label,char *Label1,int inx) {
   int i,n;
   char buff[3],buff1[3];
   IMDT *im;
   BUT_STR * but;
   n = B->nx * B->ny;
   if(inx > n ) inx=n;
   but = (BUT_STR *)B->buts;
   B->type = Kbrd->Btype;
   B->fac = Kbrd->Rfac;
   if(B->type > 0)  {
    for(i=0;i<inx;i++) {
      buff[0]= Label[i];
      buff[1]='\0';
      if(buff[0]=='!') {buff[1]='!';buff[2]='\0';}
      but[i].bkgr = Kbrd->ButClr;
      Procpy(but[i].title,buff,Kbrd);
      but[i].xpmn = NULL;
    }
    for(i=inx;i<n;i++) {
      buff[0]= Label[i];
      buff[1]='\0';
      if(buff[0]=='!') {buff[1]='!';buff[2]='\0';}
      buff1[0]= Label1[i];
      buff1[1]='\0';
      if(buff1[0]=='!') {buff1[1]='!';buff1[2]='\0';}
      but[i].bkgr = Kbrd->ButClr;
      Procpy1(but[i].title,buff,Kbrd,buff1);
      but[i].xpmn = NULL;
    }
   }
   else {
    for(i=0;i<inx;i++) {
      buff[0]= Label[i];
      buff[1]='\0';
      if(buff[0]=='!') {buff[1]='!';buff[2]='\0';}
      but[i].bkgr = Kbrd->ButClr;
      im = (IMDT *) Malloc(sizeof(IMDT));
      if(im==NULL) {
        fprintf(stderr,"Failed in Malloc\n");
      }
      else {
      im->B = B;
      im->kb = Kbrd;
      strncpy(im->str,buff,80);
      im->str[80]='\0';
      im->ustr[0]='\0';
      im->index = i;
      if(Kbrd->Thds != NULL) DoInAnyThread(Kbrd->Thds,CreateButImage4,(void *)im);
      else CreateButImage4((void *)(im));
      }
      but[i].title[0]='\0';
    }
    for(i=inx;i<n;i++) {
      buff[0]= Label[i];
      buff[1]='\0';
      if(buff[0]=='!') {buff[1]='!';buff[2]='\0';}
      buff1[0]= Label1[i];
      buff1[1]='\0';
      if(buff1[0]=='!') {buff1[1]='!';buff1[2]='\0';}
      but[i].bkgr = Kbrd->ButClr;
      im = (IMDT *) Malloc(sizeof(IMDT));
      if(im==NULL) {
        fprintf(stderr,"Failed in Malloc\n");
      }
      else {
      im->B = B;
      im->kb = Kbrd;
      strcpy(im->str,buff);
      strcpy(im->ustr,buff1);
      im->index = i;
      if(Kbrd->Thds != NULL) DoInAnyThread(Kbrd->Thds,CreateButImage4,(void *)im);
      else CreateButImage4((void *)(im));
      }
      but[i].title[0]='\0';
    }
   }
   if(Kbrd->Thds != NULL) WaitThreads(Kbrd->Thds);
   for(i=0;i<n;i++) {
     Dadd(XpmList,but[i].xpmn);
   }
   return 1;
}
static void *MakeLightImage(int xl,int yl,int r,int g,int b,float fac) {
   void *Img,*Img1;
   float h,s,v,rf,gf,bf,vm;
   DIG *fid;
   fid = (DIG *)kgInitImage(xl,yl,5);
   kgUserFrame(fid,-2.,-2.,(float)xl+2,(float)yl+2);
   kgChangeColor(fid,551,(int)r,(int)g,(int)b);
   kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,
                        (float)xl+2,(float)yl+2,0,551,fac);
   Img = kgGetSharpImage(fid);
   kgCloseImage(fid);
   return Img;
}

static int kgFixButton(DIN *B,void *pt){
    int x1,y1,xl,yl,xg,yg,xoff,yoff;
    float xfac,yfac;
    int Bx,By,Xg,Yg;
    int Bxmin,Bymin;
    int Bodr;
    int Bxoff=2,Byoff=2;
    KEYBRD *kb;
    kb = (KEYBRD *)pt;
    B->nodrawbkgr = NOBKGR;
//    x1 = B->x1-12,y1 = B->y1-10;
    x1 = B->x1-13,y1 = B->y1-9;
    xoff =x1/19.0+0.2;
    yoff =y1/19.0+0.2;
//    fprintf(stderr,"Xoff: %d %d\n",xoff,yoff);
    Bx = kb->Bx;
    By = kb->By;
    Xg = kb->Xg;
    Yg = kb->Yg;
    Bodr = kb->Bodr;
#if 0
    if(Bodr < Xg ) Bodr = Xg+4;
    if(Bodr < Yg ) Bodr = Yg+4;
    Bxmin = 0.3*Bx+Xg;
    Bymin = 0.3*By+Yg;
    if(Bodr < Bxmin ) Bodr = Bxmin;
    if(Bodr < Bymin ) Bodr = Bymin;
#endif
    B->xgap =kb->Xg;
    if(B->xgap < 0) B->xgap =0;
    B->ygap =kb->Yg;
    x1 = (xoff*(Bx+Xg)*0.5+0.1)-Bxoff+Bodr;
    y1 = (yoff*(By+Yg)*0.5+0.1)-Byoff+Bodr;
    B->x1 = x1;
    B->y1 = y1;
//    yfac = B->width/36.0;
    xfac = (int)(B->lngth/18.0+0.05);
    xfac = xfac*0.5;
    yfac = (int)(B->width/18.0+0.05);
    yfac = yfac*0.5;
    B->lngth = Bx*xfac+0.1;
    B->width = By*yfac+0.1;
    if(xfac > 1.0 ) B->lngth += (xfac-1.0)*Xg;
    if(yfac > 1.0 ) B->width += (yfac-1.0)*Yg;
    xl = (B->lngth+B->xgap)*B->nx;
    yl = (B->width+B->ygap)*B->ny;
    B->x2 = x1+xl-1;
    B->y2 = y1+yl-1;
    B->nodrawbkgr = NOBKGR;
    return 1;
}
int  keybrd4button1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) { 
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0){
     ret= 0;
    if(Kbrd->TargetWindow != NULL) {
//      kgSetInputFocus(D,Kbrd->TargetWindow);
      kgSendBackspaceKeyToWindow(D,Kbrd->TargetWindow);
    }
  }
  else {
    kgSetCurrentWidget(D,Kbrd->CurWid);
    kgSendBackspaceKeyEvent(Tmp);
//    kgModifyTextWidget(D,8);
  }
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  keybrd4button1init(DIN *B,void *pt) {
    void *win;
    KEYBRD *kb;
    if(XpmList==NULL) XpmList = Dopen();
    win = kgGetInputFocus(NULL);
    kb = (KEYBRD *)pt;
    kb->TargetWindow = win;
    kgFixButton(B,pt);   
    kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd4button2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) { 
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0){
    ret= 0;
    if(Kbrd->TargetWindow != NULL) {
//      kgSetInputFocus(D,Kbrd->TargetWindow);
      kgSendEscapeKeyToWindow(D,Kbrd->TargetWindow);
    }
  }
  else {
    kgSetCurrentWidget(D,Kbrd->CurWid);
    kgSendEscapeKeyEvent(Tmp);
//    kgModifyTextWidget(Tmp,27);
  }
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  keybrd4button2init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd4button3callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)D->Kbrd;
  Kbrd->CapsLock = (Kbrd->CapsLock+1)%2;
  kgProcessCapsLock(D);
  return ret;
}
void  keybrd4button3init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd4button4callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) { 
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0){
    ret= 0;
    if(Kbrd->TargetWindow != NULL) {
//      kgSetInputFocus(D,Kbrd->TargetWindow);
      kgSendEnterKeyToWindow(D,Kbrd->TargetWindow);
    }
  }
  else {
    kgSetCurrentWidget(D,Kbrd->CurWid);
    kgSendEnterKeyEvent(Tmp);
  }
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  keybrd4button4init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd4button5callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)D->Kbrd;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  Kbrd->ShiftPress = (Kbrd->ShiftPress+1)%2;
  kgProcessShift(D);

  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  keybrd4button5init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd4button6callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) { 
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0){
    ret= 0;
    if(Kbrd->TargetWindow != NULL) {
//      kgSetInputFocus(D,Kbrd->TargetWindow);
      kgSendLeftKeyToWindow(D,Kbrd->TargetWindow);
    }
  }
  else {
    kgSetCurrentWidget(D,Kbrd->CurWid);
    kgSendLeftKeyEvent(Tmp);
  }
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  keybrd4button6init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd4button7callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) { 
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0) {
     ret= 0;
    if(Kbrd->TargetWindow != NULL) {
//      kgSetInputFocus(D,Kbrd->TargetWindow);
      if(butno==1) kgSendHomeKeyToWindow(D,Kbrd->TargetWindow);
      else kgSendEndKeyToWindow(D,Kbrd->TargetWindow);
    }
  }
  else {
    kgSetCurrentWidget(D,Kbrd->CurWid);
  switch(butno) {
    case 1: 
      kgSendHomeKeyEvent(Tmp);
      break;
    case 2: 
      kgSendEndKeyEvent(Tmp);
      break;
  }
  }
  return ret;
}
void  keybrd4button7init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd4button8callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) { 
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0){
    ret= 0;
    if(Kbrd->TargetWindow != NULL) {
//      kgSetInputFocus(D,Kbrd->TargetWindow);
      kgSendSpaceKeyToWindow(D,Kbrd->TargetWindow);
    }
  }
  else {
    kgSetCurrentWidget(D,Kbrd->CurWid);
//    kgSendSpaceKeyEvent(Tmp);
    kgModifyTextWidget(Tmp,32);
  }
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  keybrd4button8init(DIN *B,void *pt) {
// Space Bar
    kgFixButton(B,pt);   
    kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd4button9callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label2);
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4: 
      break;
    case 5: 
      break;
    case 6: 
      break;
    case 7: 
      break;
    case 8: 
      break;
    case 9: 
      break;
    case 10: 
      break;
  }
  return ret;
}
void  keybrd4button9init(DIN *B,void *pt) {
   kgFixButton(B,pt);   
   kgProcessInit4(B,(KEYBRD *)pt,Label2,Label7,10);
}
int  keybrd4button10callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label3);
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4: 
      break;
    case 5: 
      break;
    case 6: 
      break;
    case 7: 
      break;
    case 8: 
      break;
    case 9: 
      break;
  }
  return ret;
}
void  keybrd4button10init(DIN *B,void *pt) {
   kgFixButton(B,pt);   
   kgProcessInit4(B,(KEYBRD *)pt,Label3,Label8,9);
}
int  keybrd4button11callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label4);
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4: 
      break;
    case 5: 
      break;
    case 6: 
      break;
    case 7: 
      break;
  }
  return ret;
}
void  keybrd4button11init(DIN *B,void *pt) {
   kgFixButton(B,pt);   
   kgProcessInit4(B,(KEYBRD *)pt,Label4,Label9,7);
}
int  keybrd4button12callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label7);
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4: 
      break;
    case 5: 
      break;
    case 6: 
      break;
    case 7: 
      break;
    case 8: 
      break;
    case 9: 
      break;
    case 10: 
      break;
  }
  return ret;
}
void  keybrd4button12init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessInit4(B,(KEYBRD *)pt,Label7,NULL,13);
}
int  keybrd4button13callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label8);
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4: 
      break;
    case 5: 
      break;
    case 6: 
      break;
    case 7: 
      break;
    case 8: 
      break;
    case 9: 
      break;
  }
  return ret;
}
void  keybrd4button13init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessInit4(B,(KEYBRD *)pt,Label8,NULL,13);
}
int  keybrd4button14callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label9);
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4: 
      break;
    case 5: 
      break;
    case 6: 
      break;
    case 7: 
      break;
  }
  return ret;
}
void  keybrd4button14init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessInit4(B,(KEYBRD *)pt,Label9,NULL,13);
}
int  keybrd4button15callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label1);
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4: 
      break;
    case 5: 
      break;
    case 6: 
      break;
    case 7: 
      break;
    case 8: 
      break;
    case 9: 
      break;
    case 10: 
      break;
    case 11: 
      break;
    case 12: 
      break;
    case 13: 
      break;
  }
  return ret;
}
void  keybrd4button15init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessInit4(B,(KEYBRD *)pt,Label1,Label6,0);
}
int  keybrd4button16callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label10);
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
  }
  return ret;
}
void  keybrd4button16init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessInit4(B,(KEYBRD *)pt,Label10,Label13,0);
}
int  keybrd4button17callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label11);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  keybrd4button17init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessInit4(B,(KEYBRD *)pt,Label11,Label14,0);
}
int  keybrd4button18callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label12);
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4: 
      break;
  }
  return ret;
}
void  keybrd4button18init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessInit4(B,(KEYBRD *)pt,Label12,Label15,0);
}
int  keybrd4button19callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label6);
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4: 
      break;
    case 5: 
      break;
    case 6: 
      break;
    case 7: 
      break;
    case 8: 
      break;
    case 9: 
      break;
    case 10: 
      break;
    case 11: 
      break;
    case 12: 
      break;
    case 13: 
      break;
  }
  return ret;
}
void  keybrd4button19init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessInit4(B,(KEYBRD *)pt,Label6,NULL,13);
}
int  keybrd4button20callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label13);
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
  }
  return ret;
}
void  keybrd4button20init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessInit4(B,(KEYBRD *)pt,Label13,NULL,13);
}
int  keybrd4button21callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label14);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  keybrd4button21init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessInit4(B,(KEYBRD *)pt,Label14,NULL,13);
}
int  keybrd4button22callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgProcessLabel(Tmp,butno,Label15);
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4: 
      break;
  }
  return ret;
}
void  keybrd4button22init(DIN *B,void *pt) {
    kgFixButton(B,pt);   
    kgProcessInit4(B,(KEYBRD *)pt,Label15,NULL,13);
}
int keybrd4init(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int keybrd4cleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  DIP *P;
  void *img;
  Resetlink(XpmList);
  while ( (img = (void *)Getrecord(XpmList))!= NULL) {
    kgFreeImage(img);
  }
  Dfree(XpmList);
  XpmList=NULL;
  return ret;
}
int Modifykeybrd4(void *Tmp,int GrpId) {
  int Bx,By,Xg,Yg;
  DIALOG *D;
  KEYBRD *kb;
  D = (DIALOG *)Tmp;
  DIA *d;
  DIN *B;
  DIP *P;
  DIM *M;
  int i,n;
  int xl,yl;
  int Rd,Gn,Bl;
  int Bxmin,Bymin;
  int Bodr;
  d = D->d;
  kb = (KEYBRD *)D->Kbrd;
  Bx = kb->Bx;
  By = kb->By;
  Xg = kb->Xg;
  Yg = kb->Yg;
  Bodr = kb->Bodr;
#if 0
  if(Bodr < Xg ) Bodr = Xg+4;
  if(Bodr < Yg ) Bodr = Yg+4;
#endif
#if 0
  D->xo = 0,D->yo=0;
  D->xl = 14.5*(Bx+Xg)+2*Bodr+3;
  D->yl = 4.5*(By+Yg)+2*Bodr;
#else
  xl = 14.5*(Bx+Xg)+2*Bodr+3;
  yl = 4.5*(By+Yg)+2*Bodr;
#endif
  i=GrpId;while(d[i].t!= NULL) {;
    B = (DIN *)d[i].n;
    if(B->code == 'n') {
//      fprintf(stderr,"%s : %d\n",B->Wid,i);
//      kgFixButton(B);   
    }
     i++;
  };
#if 0
  B=  (DIN *)kgGetNamedWidget(D,"Enter");
  B->lngth = Bx*1.5;
  B=  (DIN *)kgGetNamedWidget(D,"CapsLock");
  B->lngth = Bx*2+Xg;
#endif
  P = (DIP *)d[GrpId].t;
  P->x1= 1;
  P->y1= 1;
  P->x2= xl-1;
  P->y2= yl-1;
  kgGetDefaultRGB(kb->FillClr,&Rd,&Gn,&Bl);
  P->xpm = MakeLightImage(P->x2-P->x1,P->y2-P->y1,Rd,Gn,Bl,kb->Brfac);
  Dadd(XpmList,P->xpm);
  P->transparency = kb->trans;
  P->bordr = 0;
  B = (DIN *)d[GrpId+1].t;
  B->y1= (By)*4+4*Yg+Bodr+By/5;
  B->y1= yl-15;
  B->x1= Bodr+10;
  B->y2= B->y1+8;
  B->x2= B->x1+8;
  B = (DIN *)d[GrpId+2].t;
  B->y1= (By)*4+4*Yg+Bodr+By/5;
  B->y1= yl-15;
  B->x1= Bodr+10;
  B->y2= B->y1+8;
  B->x2= B->x1+8;
  B = (DIN *)d[GrpId+3].t;
  B->y1= (By)*4+4*Yg+Bodr+By/5-5;
  B->y1= yl-20;
  B->x1= Bodr+20;
  B->x2= B->x1+70;
  B->y2= B->y1+16;
  n=1;
  P = (DIP *) d[GrpId+1].t;
  P->bordr=0;
  P->xpm = MakeLightImage(P->x2-P->x1,P->y2-P->y1,30,25,30,0.5);
  Dadd(XpmList,P->xpm);
  P = (DIP *) d[GrpId+2].t;
  P->bordr=0;
  P->xpm = MakeLightImage(P->x2-P->x1,P->y2-P->y1,220,25,30,0.5);
  Dadd(XpmList,P->xpm);
  P = (DIP *)d[GrpId+3].t;
  P->xpm = kgStringToImage((char *)"Caps Lock",NULL,P->x2-P->x1,P->y2-P->y1,16,kb->ButClr,-1,(P->y2-P->y1),kb->FillClr);
  Dadd(XpmList,P->xpm);
  P->transparency = kb->trans;
  if(D->xl < (xl+1)) D->xl = (xl+1);
  if(D->yl < (yl+1)) D->yl = (yl+1);
  return GrpId;
}

int keybrd4CallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
int keybrd4ResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  kgGetWindowSize(D,&xres,&yres);
  dx = xres - D->xl;
  dy = yres - D->yl;
  /* extra code */
  D->xl= xres;
  D->yl= yres;
  kgRedrawDialog(D);
  return ret;
}
int keybrd4WaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
