#include "kulina.h"
static char BUFF[100];
#define NOBKGR 1
//static Dlink *XpmList=NULL;
static char Label1[]="`1234567890-=";
static char Label2[] ="qwertyuiop[]\\";
static char Label3[] = "asdfghjkl;'";
static char Label4[] = "zxcvb";
static char Label5[] = "nm,./";
static char Label6[]="~!@#$%^&*()_+";
static char Label7[] ="QWERTYUIOP{}|";
static char Label8[] = "ASDFGHJKL:\"";
static char Label9[] = "ZXCVB";
static char Label10[] = "NM<>?";
void kgModifyTextWidget(void *Tmp,int ch);
static int ProcessShift(void *Tmp);
typedef struct _butimgdata {
  void *B;
  void *kb;
  int index;
  char str[100];
  char ustr[100];
} IMDT;
int kgCheckEventDelivery(void *Tmp) {
  int ret=1;
#if 0   // Not Working
  DIALOG *D;
  KBEVENT ev;
  D = (DIALOG *)Tmp;
  usleep(4000);
  if ((kgCheckEvent(D,&ev)== 0) || (ev.event != 5)) {
    usleep(20000);
    if ((kgCheckEvent(D,&ev)== 0) || (ev.event != 5)) {
      usleep(100000);
      if ((kgCheckEvent(D,&ev)== 0) || (ev.event != 5))ret =0;
      else kgPushBackEvent(D);
    }
    else kgPushBackEvent(D);
  }
  else kgPushBackEvent(D);
#endif
  return ret;
}
int kgShowKey(int ch) {
//  usleep(22000);
  return 1;
}
int kgGetKeybrdSize(void *Tmp,int *xl,int *yl) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = D->Kbrd;
  if(Kbrd== NULL) {
    *xl=0; *yl=0; return 0;
  }
  else {
   DIP *P;
   P = (DIP *)kgGetNamedWidget(D,"Keybrdbkgr");
   if(P == NULL) {
    *xl=0; *yl=0; return 0;
   }
   else {
     *xl = P->x2 - P->x1;
     *yl = P->y2 - P->y1;
     return 1;
   }
  }
}
int kgShiftKeybrd(void *Tmp,int xs,int ys) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = D->Kbrd;
  if(Kbrd== NULL) {
    return 0;
  }
  else {
     kgShiftGrp(Tmp,Kbrd->GrpId,xs,ys);
     return 1;
  }
}
static void *MakeLightImage(int xl,int yl,int r,int g,int b,float fac) {
   void *Img,*fid,*Img1;
   float h,s,v,rf,gf,bf,vm;
   fid = kgInitImage(xl,yl,8);
   kgUserFrame(fid,-2.,-2.,(float)xl+2,(float)yl+2);
   kgChangeColor(fid,551,(int)r,(int)g,(int)b);
   kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,
                        (float)xl+2,(float)yl+2,0,551,fac);
   Img = kgGetSharpImage(fid);
   kgCloseImage(fid);
   return Img;
}
static void *MakeBkgrImage(int xl,int yl,int r,int g,int b,float fac) {
   void *Img,*fid,*Img1;
   float h,s,v,rf,gf,bf,vm;
   if(fac<= 0.00001) {
     fid = kgInitImage(xl,yl,1);
     kgUserFrame(fid,0.,0.,(float)xl,(float)yl);
     kgChangeColor(fid,551,(int)r,(int)g,(int)b);
     kgBoxFill(fid,0.,0.,xl,yl,551,0);
   }
   else {
     fid = kgInitImage(xl,yl,8);
     kgUserFrame(fid,0.,0.,(float)xl,(float)yl);
     kgChangeColor(fid,551,(int)r,(int)g,(int)b);
     kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,
                        (float)xl,(float)yl,0,551,fac);
   }
   Img = kgGetSharpImage(fid);
   kgCloseImage(fid);
   return Img;
}
static void *MakeStringImage(int xl,int yl,int bclr,float fac, int tc,int utc,int font,char*str,char *ustr) {
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
     fid = kgInitImage(xl,yl,5);
     kgUserFrame(fid,-1.0,-1.0,(float)xl+1.0,(float)yl+1.0);
     kgRoundedRectangleFill(fid,(float)xl*0.5,(float)yl*0.5,
                        (float)xl,(float)yl,0,bclr,fac);
   }
   kgTextColor(fid,tc);
   kgTextFont(fid,font);
   h = yl/2.5;
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
     kgTextColor(fid,utc);
     kgTextSize(fid,h*0.7,w*0.7,0.);
     kgMove2f(fid,xp,yp);
     kgWriteText(fid,ustr);
   }
//   Img = kgGetSmoothImage(fid);
   Img = kgGetResizedImage(fid);
   kgCloseImage(fid);
   return Img;
}
static char *kgButtonTitle1(char * str,void *pt,char *ustr) {
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)pt;
  sprintf(BUFF,"!h15!d!h51!z%-s!f%2.2d!c%2.2d%-s!g !c%2.2d!z23!u%-s",Kbrd->Sfac,Kbrd->Bfont,Kbrd->Bclr,str,Kbrd->Bclr,ustr);
//  printf("%s\n",BUFF);
  return BUFF;
}
static char *Procpy1(char *des,char *src,void *pt,char *ustr) {
   strcpy(des,kgButtonTitle1(src,pt,ustr));
   return des;
}
static char *kgButtonTitle(char * str,void *pt) {
  KEYBRD *Kbrd;
  Kbrd = (KEYBRD *)pt;
  sprintf(BUFF,"!h15!d!h51!z%-s!f%2.2d!c%2.2d%-s!g !g",Kbrd->Sfac,Kbrd->Bfont,Kbrd->Bclr,str);
//  printf("%s\n",BUFF);
  return BUFF;
}
static char *Procpy(char *des,char *src,void *pt) {
   strcpy(des,kgButtonTitle(src,pt));
   return des;
}

static int ProcessShift(void *Tmp) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = D->Kbrd;
 switch(Kbrd->ShiftPress) {
    case 0:
//      ProcessCapsLock(D);
      kgSetGrpVisibility(D,Kbrd->ongrp,0);
      switch(Kbrd->CapsLock) {
        case 0: 
          kgSetGrpVisibility(D,Kbrd->grp2,0);
          kgSetGrpVisibility(D,Kbrd->cgrp,0);
          kgSetGrpVisibility(D,Kbrd->sgrp,1);
          kgSetGrpVisibility(D,Kbrd->grp1,1);
          break;
        case 1: 
          kgSetGrpVisibility(D,Kbrd->grp1,0);
          kgSetGrpVisibility(D,Kbrd->sgrp,0);
          kgSetGrpVisibility(D,Kbrd->cgrp,1);
          kgSetGrpVisibility(D,Kbrd->grp2,1);
          break;
      }
      kgSetGrpVisibility(D,Kbrd->offgrp,1);
      kgUpdateGrp(D,Kbrd->offgrp);
      break;
    case 1:
      kgSetGrpVisibility(D,Kbrd->offgrp,0);
      switch(Kbrd->CapsLock) {
        case 1: 
          kgSetGrpVisibility(D,Kbrd->grp1,0);
          kgSetGrpVisibility(D,Kbrd->cgrp,0);
          kgSetGrpVisibility(D,Kbrd->sgrp,1);
          kgSetGrpVisibility(D,Kbrd->grp2,1);
          break;
        case 0: 
          kgSetGrpVisibility(D,Kbrd->grp2,0);
          kgSetGrpVisibility(D,Kbrd->sgrp,0);
          kgSetGrpVisibility(D,Kbrd->cgrp,1);
          kgSetGrpVisibility(D,Kbrd->grp1,1);
          break;
      }
      kgSetGrpVisibility(D,Kbrd->ongrp,1);
      kgUpdateGrp(D,Kbrd->ongrp);
     break;
 }
 kgUpdateOn(D);
 return 0;
}
static int ProcessCapsLock(void *Tmp) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  KEYBRD *Kbrd;
  Kbrd = D->Kbrd;
  Kbrd->ShiftPress=0;
#if 0
  switch(Kbrd->CapsLock) {
    case 0: 
      kgSetGrpVisibility(D,Kbrd->cgrp,0);
      kgSetGrpVisibility(D,Kbrd->sgrp,1);
      kgUpdateGrp(D,Kbrd->sgrp);
      break;
    case 1: 
      kgSetGrpVisibility(D,Kbrd->sgrp,0);
      kgSetGrpVisibility(D,Kbrd->cgrp,1);
      kgUpdateGrp(D,Kbrd->cgrp);
      break;
  }
  kgUpdateOn(D);
#else
  ProcessShift(Tmp);
#endif
  return 1;
}
static int ProcessLabel(void *Tmp,int butno,char *Label) {
  int CurWid;
  char ch;
  DIALOG *D;
  KEYBRD *Kbrd;
  D = (DIALOG *)Tmp;
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  CurWid = Kbrd->CurWid;
  if(D->InputWid >= 0) {
     Kbrd->CurWid = D->InputWid;
     CurWid = Kbrd->CurWid;
  }
  if(CurWid>= 0) {
  kgSetCurrentWidget(D,CurWid);
  ch= Label[butno-1];
//  kgSendKeyEvent(D,ch);
  kgModifyTextWidget(D,ch);
  }
  else {
    if(Kbrd->TargetWindow != NULL) {
//      kgSetInputFocus(D,Kbrd->TargetWindow);
      ch= Label[butno-1];
      kgSendKeyToWindow(D,Kbrd->TargetWindow,ch);
    }
  }
  if(Kbrd->ShiftPress) {
    Kbrd->ShiftPress=0;
    ProcessShift(D);
  }
  return 1;
}
static void *GetButImg(DIN *B,KEYBRD *kb) {
   int xl,yl;
   int r,g,b;
   int clr;
   if(B->type >= 1){
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
static void *GetStringImg(DIN *B,int index,KEYBRD *kb,char *str,char *ustr) {
   int xl,yl;
   int r,g,b;
   int clr;
   void *img=NULL;
   BUT_STR * but;
   but = B->buts;
   if(B->type > 1) {
       but[index].xpmn=img;
       return NULL;
   }
   xl =B->lngth;
   yl =B->width;
   clr =kb->ButClr;
  
   img = MakeStringImage(xl,yl,clr,B->fac,kb->Bclr,kb->Bclr,kb->Bfont,str,ustr);
   but[index].xpmn=img;
   return img;
}
static void *CreateButImage(void *Tmp) {
    IMDT *im;
    im = (IMDT *)(Tmp);
    DIN *B;
    KEYBRD *kb;
    B = (DIN *)im->B;
    kb = (KEYBRD *)im->kb;
    GetStringImg(B,im->index,kb,im->str,im->ustr);
    free(Tmp);
    return NULL;
  
}
static int kgFixButton(DIN *B,void *pt){
    int x1,y1,xl,yl,xg,yg,xoff,yoff;
    float xfac,yfac;
    int Bx,By,Xg,Yg;
    int Bxmin,Bymin;
    int Bodr;
    int Bxoff=2,Byoff=2;
    float Dbx=68,Dby=48,Dxg=6,Dyg=2;
    float Hx,Hy;
    KEYBRD *kb;
#if 0
    static int Entry=0;
    if(!Entry) {
      kgFreeImage(MakeLightImage(5,5,0,0,0,0.0));
      Entry =1;
    }
#endif
//    B->nodrawbkgr = NOBKGR;
//    pt = NULL;
    if(pt != NULL) {
    kb = (KEYBRD *)pt;
    Bx = kb->Bx;
    By = kb->By;
    Xg = kb->Xg;
    Yg = kb->Yg;
    Bodr = kb->Bodr;
    B->type = kb->Btype;
    if(kb->XpmList==NULL) kb->XpmList = Dopen();
    }
    else {
    Bx = 68;
    By = 48;
    Xg = 6;
    Yg = 2;
    Bodr = 10;
    }
    Hx = (int)(Dbx+Dxg)/2;
    Hy = (int)(Dby+Dyg)/2;
    x1 = B->x1-50,y1 = B->y1-40;
    xoff =x1/Hx+0.2;
    yoff =y1/Hy+0.2;
    B->xgap =Xg;
    if(B->xgap < 0) B->xgap =0;
    B->ygap =Yg;
    x1 = (xoff*(Bx+Xg)*0.5+0.1)-Bxoff+Bodr;
    y1 = (yoff*(By+Yg)*0.5+0.1)-Byoff+Bodr;
    B->x1 = x1;
    B->y1 = y1;
//    yfac = B->width/36.0;
    xfac = (int)(B->lngth/(Dbx*0.5)+0.05);
    xfac = xfac*0.5;
    yfac = (int)(B->width/(Dby*0.5)+0.05);
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
int  keybrd0button1callback(int butno,int i,void *Tmp) {
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
  ProcessLabel(Tmp,butno,Label1);
  if(!kgCheckEventDelivery(Tmp)) ProcessLabel(Tmp,butno,Label1);
  kgShowKey('a');
  return ret;
}
void kgProcessTitle(DIN *B,void *pt){
   int i,n;
   IMDT *im;
   KEYBRD *Kbrd;
   Kbrd = (KEYBRD *)pt;
   char buff[100];

   BUT_STR * but;
   but = B->buts;
   n = B->nx * B->ny;
   B->type = Kbrd->Btype;
   B->fac = Kbrd->Rfac;
   for(i=0;i<n;i++) {
    but[i].bkgr = Kbrd->ButClr;
    if(B->type >= 1)  {
      strcpy(buff,but[i].title);
      Procpy(but[i].title,buff,Kbrd);
      but[i].xpmn = NULL;
    }
    else {
      im = (IMDT *) malloc(sizeof(IMDT));
      im->B = B;
      im->kb = Kbrd;
      strcpy(im->str,but[i].title);
      im->ustr[0]='\0';
      im->index = i;
      CreateButImage((void *)(im));
//      DoInAnyThread(Kbrd->Thds,CreateButImage,(void *)im);
      but[i].title[0]='\0';
    }
   }
//   if(Kbrd->Thds != NULL) WaitThreads(Kbrd->Thds);
   for(i=0;i<n;i++) {
     if( but[i].xpmn!= NULL) Dadd(Kbrd->XpmList,but[i].xpmn);
   }

}
static int kgProcessType0Init(DIN *B,KEYBRD *Kbrd,char *Label,char *Label1,int inx) {
   int i,n;
   char buff[100],buff1[100];
   IMDT *im;
   BUT_STR * but;
   n = B->nx * B->ny;
   if(inx > n ) inx=n;
   but = B->buts;
   B->type = Kbrd->Btype;
   B->fac = Kbrd->Rfac;
    for(i=0;i<inx;i++) {
    
      buff[0]= Label[i];
      buff[1]='\0';
      if(buff[0]=='!') {buff[1]='!';buff[2]='\0';}
      but[i].bkgr = Kbrd->ButClr;
      im = (IMDT *) malloc(sizeof(IMDT));
      im->B = B;
      im->kb = Kbrd;
      strncpy(im->str,buff,80);
      im->str[80]='\0';
      im->ustr[0]='\0';
      im->index = i;
      if(Kbrd->Thds!=NULL) DoInAnyThread(Kbrd->Thds,CreateButImage,(void *)im);
      else CreateButImage((void *)(im));
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
      im->B = B;
      im->kb = Kbrd;
      strcpy(im->str,buff);
      strcpy(im->ustr,buff1);
      im->index = i;
      if(Kbrd->Thds!=NULL) DoInAnyThread(Kbrd->Thds,CreateButImage,(void *)im);
      else CreateButImage((void *)(im));
    }
    if(Kbrd->Thds != NULL) WaitThreads(Kbrd->Thds);
    for(i=0;i<n;i++) {
     but[i].title[0]='\0';
     if( but[i].xpmn!= NULL) Dadd(Kbrd->XpmList,but[i].xpmn);
    }
   return 1;
}
static int kgProcessOtherInit(DIN *B,KEYBRD *Kbrd,char *Label,char *Label1,int inx) {
   int i,n;
   char buff[10],buff1[10];
   IMDT *im;
   BUT_STR * but;
   n = B->nx * B->ny;
   if(inx > n ) inx=n;
   but = B->buts;
   B->type = Kbrd->Btype;
   B->fac = Kbrd->Rfac;
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
   return 1;
}
static int kgProcessInit(DIN *B,KEYBRD *Kbrd,char *Label,char *Label1,int inx) {
   int i,n;
   B->type = Kbrd->Btype;
   if(B->type > 0 ) kgProcessOtherInit(B,Kbrd,Label,Label1,inx);
   else  kgProcessType0Init(B,Kbrd,Label,Label1,inx);
   return 1;
}
void  keybrd0button1init(DIN *B,void *pt) {
   int n;
   void *win;
   KEYBRD *kb;
   n = B->nx * B->ny;
   win = kgGetInputFocus(NULL);
   kb = (KEYBRD *)pt;
   kb->TargetWindow = win;

   kgFixButton(B,pt);
   kgProcessInit(B,(KEYBRD *)pt,Label1,Label6,0);
}
int  keybrd0button2callback(int butno,int i,void *Tmp) {
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
  ProcessLabel(Tmp,butno,Label2);
  if(!kgCheckEventDelivery(Tmp)) ProcessLabel(Tmp,butno,Label2);
  kgShowKey('a');
  return ret;
}
void  keybrd0button2init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessInit(B,(KEYBRD *)pt,Label2,Label7,10);
}
int  keybrd0button3callback(int butno,int i,void *Tmp) {
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
  ProcessLabel(Tmp,butno,Label3);
  if(!kgCheckEventDelivery(Tmp)) ProcessLabel(Tmp,butno,Label3);
  kgShowKey('a');
  return ret;
}
void  keybrd0button3init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessInit(B,(KEYBRD *)pt,Label3,Label8,9);
}
int  keybrd0button4callback(int butno,int i,void *Tmp) {
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
  ProcessLabel(Tmp,butno,Label4);
  if(!kgCheckEventDelivery(Tmp)) ProcessLabel(Tmp,butno,Label4);
  kgShowKey('a');
  return ret;
}
void  keybrd0button4init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessInit(B,(KEYBRD *)pt,Label4,NULL,n);
}
int  keybrd0button5callback(int butno,int i,void *Tmp) {
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
  ProcessLabel(Tmp,butno,Label5);
  if(!kgCheckEventDelivery(Tmp)) ProcessLabel(Tmp,butno,Label5);
  kgShowKey('a');
  return ret;
}
void  keybrd0button5init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessInit(B,(KEYBRD *)pt,Label5,Label10,2);
}
int  keybrd0button6callback(int butno,int i,void *Tmp) {
  /*********************************** 
    Cap lock
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
  Kbrd = D->Kbrd;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  Kbrd->CapsLock = (Kbrd->CapsLock+1)%2;
  ProcessCapsLock(D);
  kgShowKey('a');
  return ret;
}
void  keybrd0button6init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd0button7callback(int butno,int i,void *Tmp) {
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
  ProcessLabel(Tmp,butno,Label6);
  if(!kgCheckEventDelivery(Tmp)) ProcessLabel(Tmp,butno,Label6);
  kgShowKey('a');
  return ret;
}
void  keybrd0button7init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessInit(B,(KEYBRD *)pt,Label6,NULL,n);
}
int  keybrd0button8callback(int butno,int i,void *Tmp) {
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
  ProcessLabel(Tmp,butno,Label7);
  if(!kgCheckEventDelivery(Tmp)) ProcessLabel(Tmp,butno,Label7);
  kgShowKey('a');
  return ret;
}
void  keybrd0button8init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessInit(B,(KEYBRD *)pt,Label7,NULL,n);
}
int  keybrd0button9callback(int butno,int i,void *Tmp) {
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
  ProcessLabel(Tmp,butno,Label8);
  if(!kgCheckEventDelivery(Tmp)) ProcessLabel(Tmp,butno,Label8);
  kgShowKey('a');
  return ret;
}
void  keybrd0button9init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessInit(B,(KEYBRD *)pt,Label8,NULL,n);
}
int  keybrd0button10callback(int butno,int i,void *Tmp) {
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
  ProcessLabel(Tmp,butno,Label9);
  if(!kgCheckEventDelivery(Tmp)) ProcessLabel(Tmp,butno,Label9);
  kgShowKey('a');
  return ret;
}
void  keybrd0button10init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessInit(B,(KEYBRD *)pt,Label9,NULL,n);
}
int  keybrd0button11callback(int butno,int i,void *Tmp) {
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
  ProcessLabel(Tmp,butno,Label10);
  if(!kgCheckEventDelivery(Tmp)) ProcessLabel(Tmp,butno,Label10);
  kgShowKey('a');
  return ret;
}
void  keybrd0button11init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessInit(B,(KEYBRD *)pt,Label10,NULL,n);
}
int  keybrd0button12callback(int butno,int i,void *Tmp) {
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
  Kbrd = D->Kbrd;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  Kbrd->CapsLock = (Kbrd->CapsLock+1)%2;
  ProcessCapsLock(D);
  kgShowKey('a');
  return ret;
}
void  keybrd0button12init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd0button13callback(int butno,int i,void *Tmp) {
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
  Kbrd = D->Kbrd;
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
    if(!kgCheckEventDelivery(Tmp)) kgSendBackspaceKeyEvent(Tmp);
  }
  kgShowKey('a');
  return ret;
}
void  keybrd0button13init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd0button14callback(int butno,int i,void *Tmp) {
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
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) {
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0) {
     ret= 0;
    if(Kbrd->TargetWindow != NULL) {
//      kgSetInputFocus(D,Kbrd->TargetWindow);
    switch(butno) {
    case 1:
      kgSendLeftKeyToWindow(D,Kbrd->TargetWindow);
      break;
    case 2:
      kgSendRightKeyToWindow(D,Kbrd->TargetWindow);
      break;

    }

    }
  }
  else {
  kgSetCurrentWidget(D,Kbrd->CurWid);
  switch(butno) {
    case 1: 
      kgSendLeftKeyEvent(Tmp);
      if(!kgCheckEventDelivery(Tmp)) kgSendLeftKeyEvent(Tmp);
      break;
    case 2: 
      kgSendRightKeyEvent(Tmp);
      if(!kgCheckEventDelivery(Tmp)) kgSendRightKeyEvent(Tmp);
      break;
  }
  }
  kgShowKey('a');
  return ret;
}
void  keybrd0button14init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd0button15callback(int butno,int i,void *Tmp) {
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
  Kbrd = D->Kbrd;
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
  if(!kgCheckEventDelivery(Tmp)) kgSendEscapeKeyEvent(Tmp);
  }
  kgShowKey('a');
  return ret;
}
void  keybrd0button15init(DIN *B,void *pt) {
   int n;
   n = B->nx * B->ny;
   kgFixButton(B,pt);
   kgProcessTitle(B,(KEYBRD *)pt);
   return;
}
int  keybrd0button16callback(int butno,int i,void *Tmp) {
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
  Kbrd = D->Kbrd;
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
    if(!kgCheckEventDelivery(Tmp)) kgSendEnterKeyEvent(Tmp);
  }
  kgShowKey('a');
  return ret;
}
void  keybrd0button16init(DIN *B,void *pt) {
   kgFixButton(B,pt);
   kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd0button17callback(int butno,int i,void *Tmp) {
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
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) {
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0){
     ret= 0;
    if(Kbrd->TargetWindow != NULL) {
//      kgSetInputFocus(D,Kbrd->TargetWindow);
      kgSendTabKeyToWindow(D,Kbrd->TargetWindow);
    }
  }
  else {
    kgSetCurrentWidget(D,Kbrd->CurWid);
    kgSendTabKeyEvent(Tmp);
    if(!kgCheckEventDelivery(Tmp)) kgSendTabKeyEvent(Tmp);
  }
  kgShowKey('a');
  return ret;
}
void  keybrd0button17init(DIN *B,void *pt) {
   kgFixButton(B,pt);
   kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd0button18callback(int butno,int i,void *Tmp) {
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
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) {
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0){
     ret= 0;
    if(Kbrd->TargetWindow != NULL) {
//      kgSetInputFocus(D,Kbrd->TargetWindow);
      kgSendHomeKeyToWindow(D,Kbrd->TargetWindow);
    }
  }
  else {
   kgSetCurrentWidget(D,Kbrd->CurWid);
   kgSendHomeKeyEvent(Tmp);
    if(!kgCheckEventDelivery(Tmp)) kgSendHomeKeyEvent(Tmp);
  }
  kgShowKey('a');
  return ret;
}
void  keybrd0button18init(DIN *B,void *pt) {
   kgFixButton(B,pt);
   kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd0button19callback(int butno,int i,void *Tmp) {
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
  Kbrd = D->Kbrd;
  Kbrd->CurWid =-1;
  if(D->InputWid >= 0) {
     Kbrd->CurWid = D->InputWid;
  }
  if(Kbrd->CurWid < 0){
     ret= 0;
    if(Kbrd->TargetWindow != NULL) {
//      kgSetInputFocus(D,Kbrd->TargetWindow);
      kgSendEndKeyToWindow(D,Kbrd->TargetWindow);
    }
  }
  else {
    kgSetCurrentWidget(D,Kbrd->CurWid);
    kgSendEndKeyEvent(Tmp);
    if(!kgCheckEventDelivery(Tmp)) kgSendEndKeyEvent(Tmp);
  }
  kgShowKey('a');
  return ret;
}
void  keybrd0button19init(DIN *B,void *pt) {
   kgFixButton(B,pt);
   kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd0button20callback(int butno,int i,void *Tmp) {
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
  Kbrd = D->Kbrd;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  Kbrd->ShiftPress = (Kbrd->ShiftPress+1)%2;
  ProcessShift(D);
  kgShowKey('a');
  return ret;
}
void  keybrd0button20init(DIN *B,void *pt) {
   kgFixButton(B,pt);
   kgProcessTitle(B,(KEYBRD *)pt);
}
int  keybrd0button21callback(int butno,int i,void *Tmp) {
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
  Kbrd = D->Kbrd;
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
//    if(!kgCheckEventDelivery(Tmp)) kgSendSpaceKeyEvent(Tmp);
    kgModifyTextWidget(D,32);
  }
  kgShowKey('a');
  return ret;
}
void  keybrd0button21init(DIN *B,void *pt) {
   kgFixButton(B,pt);
   kgProcessTitle(B,(KEYBRD *)pt);
}
int keybrd0init(void *Tmp) {
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
int keybrd0cleanup(void *Tmp) {
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
  KEYBRD *kb;
  kb = (KEYBRD *)D->Kbrd;
  Resetlink(kb->XpmList);
  while ( (img = (void *)Getrecord(kb->XpmList))!= NULL) {
    kgFreeImage(img);
  }
  Dfree(kb->XpmList);
  kb->XpmList=NULL;
  return ret;
}
int keybrd0CallBack(void *Tmp,void *tmp) {
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
int keybrd0ResizeCallBack(void *Tmp) {
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
int keybrd0WaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
int Modifykeybrd0(void *Tmp,int GrpId) {
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
  xl = 15.5*(Bx+Xg)+2*Bodr+3;
  yl = 4.0*(By+Yg)+2*Bodr+10;
  P = (DIP *)d[GrpId].t;
  P->x1 = 1;
  P->y1 = 1;
  P->x2=  xl-1;
  P->y2= yl-1;
  kgGetDefaultRGB(kb->FillClr,&Rd,&Gn,&Bl);
  P->xpm = MakeLightImage(P->x2-P->x1,P->y2-P->y1,Rd,Gn,Bl,kb->Brfac);
  Dadd(kb->XpmList,P->xpm);
  P->transparency = kb->trans;
  P->bordr = 0;
  if(D->xl < (xl+1)) D->xl = (xl+1);
  if(D->yl < (yl+1)) D->yl = (yl+1);
  return GrpId;
}
