#include "kulina.h"
#include "kgMessageCallbacks.h"

static void *Args=NULL,*Rets=NULL;

static DIAINTR *It = NULL;


static MODINTERFACE ModFuns[] = { 
    (MODINTERFACE) NULL 
};
static Dlink *ModuleList=NULL;
  void *ui_MakeStringImage ( void *str , int font , float width ) {
      int ret = 0 , i , j , OK , xl , yl , count = 0 , \
      k , color , r , g , b , fcolor;
      float ln , length = 0 ;
      float w_x1 = 0.0 , w_y1 = 0.0 , w_x2 = 686 , w_y2 = 226 , dyl , dxl , yy;
      float h , s , v , Vb , rf , gf , bf;
      void *fid , *Img;
      float xo , yo , l , w;
      float rfac=0.15;
      char *pt , buf [ 5000 ] ;
      Dlink *S = NULL;
      if ( str == NULL ) return NULL;
      S = Dopen();
      strcpy ( buf , ( char * ) str ) ;
      pt = buf;
      j = 1;
      k = 0;
      while ( 1 ) {
          i = 0;
          if ( pt [ i ] == '\0' ) break;
          Dadd ( S , pt ) ;
          j++;
          OK = 1;
          while ( pt [ i ] != '\0' ) {
              OK = 0;
              if ( ( pt [ i ] == '\n' ) ) { pt [ i ] = '\0';break;}
              if ( ( ( pt [ i ] == '\\' ) && ( pt [ i+1 ] == 'n' ) ) ) \
              { pt [ i ] = '\0';i++;break;}
              i++;
              OK = 1;
          }
          ln = ftStringLength ( font , pt , width ) ;
          if ( length < ln ) length = ln;
          if ( OK ) break;
          pt = pt+i+1;
      }
      count = Dcount ( S ) ;
      xl = 1.5*length;
      if(xl < 250 ) xl=250;  
      yl = 2*width*(count)+width;
      w_x2 = xl;
      w_y2 = yl;
      l = xl;
      w = yl;
      fid = kgInitImage ( ( int ) ( w_x2-w_x1 ) , ( int ) ( w_y2-w_y1 ) , 8 ) ;
      kgUserFrame ( fid , w_x1-3 , w_y1-3 , w_x2+3 , w_y2+3 ) ;
      fcolor = 965;
      rf = 240;
      gf = 240;
      bf = 220;
      kgDefineColor(fcolor,rf,gf,bf);
      kgChangeColor ( fid ,fcolor , rf , gf , bf ) ;
      color = 966;
      kgDefineColor(color,50,60,40);
      kgChangeColor ( fid , color , 50 , 60 , 40 ) ;
      r = rf, g=gf,b=bf;
      RGBtoHSV ( rf , gf , bf , & h , & s , & v ) ;
      Vb = 1.5*v;
      if ( Vb > 1. ) Vb = 1.;
      HSVtoRGB ( & rf , & gf , & bf , h , s , Vb ) ;
      kgChangeColor ( fid , 152 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      HSVtoRGB ( & rf , & gf , & bf , h , s , 0.5*v ) ;
      kgChangeColor ( fid , 153 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      kgChangeColor ( fid , 151 , ( int ) r , ( int ) g , ( int ) b ) ;
      kgTextColor ( fid , color ) ;
      kgTextFont ( fid , font ) ;
      kgTextSize ( fid , 1.5*width , width , 0. ) ;
      kgLineColor (fid ,2);
      kgLineWidth (fid,4);
      xo = ( w_x1+w_x2 ) *.5;
      yo = ( w_y1+w_y2 ) *.5;
      kgRoundedRectangleFill ( fid , xo , yo , ( float ) l+2 , \
       ( float ) w+2 , 0 , 153 , rfac ) ;
      kgRoundedRectangleFill ( fid , xo , yo , ( float ) l+1 , \
       ( float ) w+1 , 0 , 152 , rfac ) ;
      kgRoundedRectangleFill ( fid , xo , yo , ( float ) l-4 , \
       ( float ) w-4 , 0 , 151 , rfac ) ;
      k = count;
      Resetlink ( S ) ;
      yy = yl-2.0*width;
      dyl = 2*width;
      while ( ( pt = ( char * ) Getrecord ( S ) ) != NULL ) {
          ln = ftStringLength ( font , pt , width ) ;
//          printf("In app ln= %f :width =%f:%d\n",ln,width,font);
          kgMove2f ( fid , ( xo-ln  *0.5) , yy ) ;
          kgWriteText ( fid , pt ) ;
          yy -= dyl;
      }
      Img = kgGetResizedImage ( fid ) ;
      
      kgCloseImage ( fid ) ;
      Dfree ( S ) ;
//      kgWriteImage(Img,"Junk.png");
      return Img;
  }
int kgMessageSetup(void *Tmp,void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  /* you add any initialisation here */
  /* useful for setting is used as MakeGroup */
  return 1;
}
 
void * kgMessageCleanDia(void *args) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  
/* you add any cleaning  here */

  return NULL;
}
 
 
void *  kgMessageAction(void *Tmp,void *Args) {
  return NULL;
} 
 
 
int   kgMessageOn(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,1);
    else return 0;
    return 1;
  } 
  return 0;
} 
 
int   kgMessageOff(void *itmp) {
  DIAINTR * Dt = (DIAINTR *) itmp;
  if(Dt == NULL ) Dt = (DIAINTR *)It;
  if(Dt != NULL) {
    if(Dt->Dtmp != NULL)kgSetGrpVisibility(Dt->Dtmp,Dt->GrpId,0);
    else return 0;
    return 1;
  } 
  return 0;
} 
 
static char *GetPointer(char *str) { 
  char *pt; 
  pt = (char *)malloc(strlen(str)+1); 
  strcpy(pt,str); 
  return pt; 
} 
 
 
void * kgMessageInterface(void *args,void *rets) {
  /*********************************** 
   ***********************************/ 
  DIAINTR *it= (DIAINTR *)malloc(sizeof(DIAINTR));
  it->GrpId=0;
  // filled by MakeGroup  it->xsh=0;
  it->ysh=0;
  it->RunDia = RunkgMessage;
  it->MakeGroup = MakekgMessageGroup;
  it->Title = GetPointer((char *)"kgMessage");
  it->Help = GetPointer( (char *)"No help yet, request");
  it->Action = kgMessageAction;
  it->Settings = kgMessageSetup;
  it->Cleanup  = kgMessageCleanDia;
  if(args != NULL) Args=args;
  if(rets != NULL) Rets=rets;
  it->args = Args;
  it->rets = Rets;
  it->SwitchOn = kgMessageOn;
  it->SwitchOff = kgMessageOff;
  it->Dtmp = NULL; // fiiled by MakeGroup 
  It = it;
  return it;
}
 
 
int kgMessageinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
 /* pt[0] is inputs, given by caller */
  if(pt[0]!= NULL) {
    DIP *P = (DIP*)kgGetNamedWidget(Tmp,(char *)"kgMsgBoxImg");
    P->xpm = pt[0];
    kgUpdateWidget(P);
  }
  kgUpdateOn(Tmp);
  return ret;
}
int kgMessagecleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
 /* pt[1] is outputs, if any  to be given to caller */
 /* pt[0] is inputs, given by caller */
  return ret;
}
int ModifykgMessage(void *Tmp,int GrpId) {
  DIALOG *D;
  DIALOG *Parent;
  D = (DIALOG *)Tmp;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
 /* pt[0] is inputs given by caller */
  char *Str=(char *)pt[0];
  void *Img=NULL;
  DIA *d;
  DIP *P;
  int i,n;
  float strln;
  int Xsize,Ysize;
   Gclr *gc;
   gc = &(D->gc);
  int font = gc->MsgFont;
  kgCheckParentPosition(Tmp);
  Parent=D->parent;
//  strln = ftStringLength(font,Str,1.0)*gc->InputFontSize;
  Img = ui_MakeStringImage(Str,font,(float)gc->InputFontSize);
  P = (DIP *)kgGetNamedWidget(D,(char *)"kgMsgBoxImg");
  if(Img != NULL) {
    pt[0]=Img;
    kgGetImageSize(Img,&Xsize,&Ysize);
    if(Xsize >100) {
      D->xl=Xsize;
      P->x1 =0;
      P->x2 = P->x1+Xsize;
    }
    D->yl=Ysize;
    P->y1 =0;
    P->y2 = P->y1+Ysize;
//    kgSetWidgetImage(P,Img);
    P->xpm = Img;
  }
  
  d = D->d;

  if( ModuleList == NULL) ModuleList = kgGetModuleList((void **)ModFuns);
  i=0;
  void *args=NULL;
  DIAINTR *Dt;
  Resetlink(ModuleList);
  while ( (Dt=(DIAINTR *)Getrecord(ModuleList)) != NULL) {
    Dt->GrpId = Dt->MakeGroup(Tmp,NULL);
    kgShiftGrp(Tmp,Dt->GrpId,Dt->xsh,Dt->ysh);
    Dt->Settings(Tmp,args);
    i++;
  };

  i=0;while(d[i].t!= NULL) {;
     i++;
  };
  n=1;
//  strcpy(D->name,"Kulina Designer ver 3.0");    /*  Dialog name you may change */
#if 1
  if(D->fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      D->xo=(xres -D->xl)*0.5;
     D->yo=(yres -D->yl)*0.5;
     if(D->parent != NULL) {
        D->xo = (Parent->xl - D->xl)*0.5;
        D->yo = (Parent->yl - D->yl)*0.5;
     }
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D->xo=D->yo=0; D->xl = xres; D->yl=yres;
//     D->StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
#endif
  return GrpId;
}

int kgMessageCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
      kgSetExit(D);
      if(pt[0] != NULL) kgFreeGmImage(pt[0]);
    }
  }
  return ret;
}
int kgMessageResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
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
int kgMessageWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
  kgWait(2);
  kgSetExit(Tmp);
  if(pt[0] != NULL) kgFreeGmImage(pt[0]);
  return ret;
}
void *RunkgMessage(void *,void *);
int kgMessageSplash(void *Tmp,char * str ){
   RunkgMessage(Tmp,str);
   return 1;
}
