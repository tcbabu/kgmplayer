#include "kulina.h"
  static DIT *Tbl;
  static T_ELMT *E;
  static Dlink *Slist;
  static DIV *V;
  static DIL *SB;
  static DIN *B1;
  static int Mid;
  static int StartLine = 0 , EndLine = 0 , Nlines , Count;
  static int AddMode = 0 , AddRow = -1;;
  static double Vsize , Vpos;
  char *flname=NULL;
  static int WriteLineNo ( int row ) {
      char BUFF [ 10 ] ;
      sprintf ( BUFF , "%4d" , StartLine+row ) ;
      kgSetString ( Tbl , row*Tbl->nx , BUFF ) ;
      return 1;
  }
  static int ClearLineNo ( int row ) {
      char BUFF [ 10 ] ;
      strcpy ( BUFF , ( char * ) "" ) ;
      kgSetString ( Tbl , row*Tbl->nx , BUFF ) ;
      return 1;
  }
  static int WriteTbl ( void ) {
      int n = ( EndLine -StartLine +1 ) ;
      int i , j , k , l;
      char *cpt , *spt;
      char Buf [ 500 ] ;
      Dposition ( Slist , StartLine ) ;
      for ( i = 0;i < n;i++ ) {
          cpt = ( char * ) Getrecord ( Slist ) ;
          if ( cpt == NULL ) {
              fprintf ( stderr , "Cpt= NULL, Start: %d End:%d\n" , StartLine , EndLine ) ;
              fflush ( stderr ) ;
              break;
          }
          j = 0;k = 0;
          while ( ( cpt [ j ] >= ' ' ) || ( cpt [ j ] == '\t' ) ) {
              if ( cpt [ j ] != '\t' ) Buf [ k++ ] = cpt [ j ] ;
              else {
                  l = ( j/8+1 ) *8;
                  while ( k < l ) Buf [ k++ ] = ' ';
              }
              j++;
          }
          Buf [ k ] = '\0';
          kgSetString ( Tbl , i*2+1 , Buf ) ;
//          kgSetString ( Tbl , i*2+1 , cpt ) ;
//          kgSetInt ( Tbl , i*2 , StartLine+i ) ;
          WriteLineNo ( i ) ;
      }
      kgUpdateWidget ( Tbl ) ;
      return 1;
  }
  static int ReadTbl ( void ) {
      int n = ( EndLine -StartLine +1 ) ;
      int i , j , k;
      char *cpt , *spt;
      for ( i = 0;i < n;i++ ) {
          cpt = ( char * ) kgGetString ( Tbl , i*2+1 ) ;
          spt = ( char * ) malloc ( strlen ( cpt ) +3 ) ;
          strcpy ( spt , cpt ) ;
//          strcat ( spt , "\n" ) ;
//        printf("%s",spt);
          Dreplace ( Slist , spt , StartLine+i-1 ) ;
      }
      return 1;
  }
  static int AddLine ( int row ) {
      char *buf;
      ReadTbl ( ) ;
      Dposition ( Slist , StartLine+row ) ;
      buf = ( char * ) malloc ( 2 ) ;
      strcpy ( buf , ( char * ) "\n" ) ;
      Dadd ( Slist , buf ) ;
      Count = Dcount ( Slist ) ;
      if ( Count <= ( Tbl->ny ) ) {
          EndLine+= 1;
          if ( EndLine <= Nlines ) {
              kgSetOnTableCell ( Tbl , ( EndLine-1 ) *2+1 ) ;
          }
      }
      if ( row == ( Nlines-1 ) ) {
          StartLine += 1;
          EndLine += 1;
      }
      WriteTbl ( ) ;
      if ( Count <= Nlines ) kgSetWidgetVisibility ( V , 0 ) ;
      else {
          kgSetWidgetVisibility ( V , 1 ) ;
          Vsize = ( double ) Nlines/Count*100;
          Vpos = ( double ) ( StartLine-1 ) / ( Count-1 ) *100;
          kgSetScrollLength ( V , Vsize ) ;
          kgSetScrollPos ( V , Vpos ) ;
      }
      kgUpdateWidget ( V ) ;
      return 1;
  }
  static int InsertLine ( int row ) {
      char *buf;
      ReadTbl ( ) ;
      Dposition ( Slist , StartLine+row ) ;
      buf = ( char * ) malloc ( 2 ) ;
      strcpy ( buf , ( char * ) "\n" ) ;
      Dinsert ( Slist , buf ) ;
      Count = Dcount ( Slist ) ;
      if ( Count <= ( Tbl->ny ) ) {
          EndLine+= 1;
          if ( EndLine <= Nlines ) {
              kgSetOnTableCell ( Tbl , ( EndLine-1 ) *2+1 ) ;
          }
      }
      WriteTbl ( ) ;
      if ( Count <= Nlines ) kgSetWidgetVisibility ( V , 0 ) ;
      else {
          kgSetWidgetVisibility ( V , 1 ) ;
          Vsize = ( double ) Nlines/Count*100;
          Vpos = ( double ) ( StartLine-1 ) /Count*100;
          kgSetScrollLength ( V , Vsize ) ;
          kgSetScrollPos ( V , Vpos ) ;
      }
      kgUpdateWidget ( V ) ;
      return 1;
  }
  static int DeleteLine ( int row ) {
      char *buf;
      ReadTbl ( ) ;
//      printf("StartLine: %d:%d\n",StartLine,EndLine);
      Dposition ( Slist , StartLine+row ) ;
      Ddelete ( Slist ) ;
      Count = Dcount ( Slist ) ;
      if ( EndLine > Count ) {
          EndLine = Count;
      }
      if ( Count == 0 ) {
          StartLine = 1;
          EndLine = 1;
          buf = ( char * ) malloc ( 2 ) ;
          strcpy ( buf , "\n" ) ;
          Dadd ( Slist , buf ) ;
          Count = 1;
      }
      if ( Count <= Nlines ) {
          StartLine = EndLine-Count+1;
      }
      else {
          if ( ( EndLine-StartLine+1 ) < Nlines ) {
              StartLine = EndLine-Nlines+1;
          }
      }
      if ( ( Count ) < Nlines ) {
          kgSetString ( Tbl , ( Count ) *2+1 , ( char * ) "" ) ;
          kgSetOffTableCell ( Tbl , Count*2+1 ) ;
          ClearLineNo ( Count ) ;
      }
      WriteTbl ( ) ;
      if ( Count <= Nlines ) kgSetWidgetVisibility ( V , 0 ) ;
      else {
          kgSetWidgetVisibility ( V , 1 ) ;
          Vsize = ( double ) Nlines/Count*100;
          Vpos = ( double ) ( StartLine-1 ) /Count*100;
          kgSetScrollLength ( V , Vsize ) ;
          kgSetScrollPos ( V , Vpos ) ;
      }
      kgUpdateWidget ( V ) ;
      return 1;
  }

int  kgGetStringstablebox1callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
           However cellno got 2 special values
             1. SCROLL_DOWN a scrolldown action
             2. SCROLL_UP a scrolldown action
             which may be ignored or peocessed
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
  int ret=1;
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
      if ( cellno == SCROLL_DOWN ) {
          ReadTbl ( ) ;
          if ( EndLine < ( Count ) ) {
              StartLine++;
              EndLine++;
              WriteTbl ( ) ;
          }
          Vpos = ( double ) ( StartLine-1 ) *100.0/Count;
          kgSetScrollPos ( V , Vpos ) ;
          kgUpdateWidget ( V ) ;
          kgSetTableCursor ( Tbl , ( Nlines-1 ) *2+1 ) ;
          kgUpdateOn ( Tbl->D ) ;
          kgSkipEvents ( Tbl->D ) ;
      }
      else if ( cellno == SCROLL_UP ) {
          ReadTbl ( ) ;
          if ( StartLine > 1 ) {
              StartLine--;
              EndLine--;
              WriteTbl ( ) ;
          }
          Vpos = ( double ) ( StartLine-1 ) *100.0/Count;
          kgSetScrollPos ( V , Vpos ) ;
          kgUpdateWidget ( V ) ;
          kgSetTableCursor ( Tbl , 1 ) ;
          kgUpdateOn ( Tbl->D ) ;
          kgSkipEvents ( Tbl->D ) ;
      }
      else {
          if ( AddMode ) {
              int row = cellno/Tbl->nx;
              if ( row != AddRow ) {AddMode = 0;}
              else {
                  AddLine ( row ) ;
                  if ( row < ( Tbl->ny-1 ) ) row++;
                  AddRow = row;
              }
              kgSetTableCursor ( Tbl , ( row ) *Tbl->nx+1 ) ;
//        printf("Row: %d:%d  %d %d\n",row, cellno,Tbl->nx,Tbl->ny);
              kgUpdateOn ( Tbl->D ) ;
          }
      }
  return ret;
}
int  kgGetStringsvertscroll1callback(double val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIV *V; 
  int ret=1; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
  int pos;
  D = (DIALOG *)Tmp;
  V = (DIV *) kgGetWidget(Tmp,i);
      ReadTbl ( ) ;
      pos = Count*val/100.0;
      StartLine = pos+1;
      EndLine = StartLine +Nlines -1;
      if ( StartLine < 1 ) {
          StartLine = 1;
          EndLine = StartLine+Nlines-1;
      }
      if ( EndLine > Count ) {
          EndLine = Count;
          StartLine = EndLine -Nlines+1;
      }
//      printf("DIV: %d %d:%d\n",StartLine,EndLine,(Nlines/2)*Tbl->nx+1);
      WriteTbl ( ) ;
      kgSetTableCursor ( Tbl , ( Nlines/2 ) *Tbl->nx+1 ) ;
      kgUpdateOn ( D ) ;
  return ret;
}
int  kgGetStringssplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  switch(butno) {
    case 1: 
      pt [ 1 ] = NULL;
      break;
    case 2: 
      ReadTbl ( ) ;
      pt [ 1 ] = Dlinktoarray(Slist);
      Dfree(Slist);
      break;
  }
  return ret;
}
void  kgGetStringssplbutton1init(DIL *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
}
int  kgGetStringsbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
  int row;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
      switch ( butno ) {
          case 1:
          row = kgGetTableRow ( Tbl ) ;
          DeleteLine ( row ) ;
          Count = Dcount ( Slist ) ;
          if ( row >= Count ) row = Count-1;
          kgSetTableCursor ( Tbl , ( row ) *Tbl->nx+1 ) ;
          kgUpdateOn ( Tmp ) ;
          break;
          case 2:
          AddMode = 1;
          row = kgGetTableRow ( Tbl ) ;
#if 0
          if ( row < Tbl->ny-1 ) {
              kgSetOnTableCell ( Tbl , ( row+1 ) *Tbl->nx+1 ) ;
//      kgUpdateWidget(Tbl);
//      kgUpdateOn(Tmp);
          }
#endif
          AddLine ( row ) ;
#if 1
          if ( row < ( Tbl->ny-1 ) ) row++;
          kgSetTableCursor ( Tbl , ( row ) *Tbl->nx+1 ) ;
#endif
          AddRow = row;
          kgUpdateOn ( Tmp ) ;
          break;
          case 3:
          AddMode = 1;
          row = kgGetTableRow ( Tbl ) ;
          InsertLine ( row ) ;
          kgSetTableCursor ( Tbl , ( row ) *Tbl->nx+1 ) ;
          AddRow = row;
          kgUpdateOn ( Tmp ) ;
          break;
      }
      kgSetAttnWidget ( Tmp , Tbl ) ;
  return ret;
}
void  kgGetStringsbutton1init(DIN *B,void *ptmp) {
 void **pt=(void **)ptmp; //pt[0] is arg 
}
int kgGetStringsinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
 /* pt[0] is inputs, given by caller */
      int k;
      char **Strs;
      char *cpt;
      Tbl = ( DIT * ) kgGetNamedWidget ( Tmp , ( char * ) "ScrollTable" ) ;
      E = Tbl->elmt;
      Nlines = Tbl->ny;
      Strs = ( char ** ) pt [ 0 ] ;
      Slist = Dopen ( ) ;
      StartLine = EndLine = 1;
      if ( ( Strs == NULL ) || ( Strs [ 0 ] == NULL ) ) {
          cpt = ( char * ) malloc ( 2 ) ;
          strcpy ( cpt , "\n" ) ;
          Dadd ( Slist , cpt ) ;
          kgSetString ( Tbl , 1 , cpt ) ;
//          kgSetInt ( Tbl , 0 , 1 ) ;
          WriteLineNo ( 0 ) ;
          kgSetOnTableCell ( Tbl , 1 ) ;
          for ( k = 1;k < Nlines; k++ ) {
              kgSetOffTableCell ( Tbl , k*2+1 ) ;
          }
          AddMode = 1;
          AddRow = 0;
      }
      else {
          k = 0;
          StartLine = 1;
          while ( Strs [ k ] != NULL ) {
              cpt = ( char * ) malloc ( strlen ( Strs [ k ] ) +1 ) ;
              strcpy ( cpt , Strs [ k ] ) ;
              Dadd ( Slist , cpt ) ;
              if ( k < Nlines ) {
                  kgSetOnTableCell ( Tbl , k*2+1 ) ;
//                  kgSetString ( Tbl , k*2+1 , cpt ) ;
//                  kgSetInt ( Tbl , k*2 , k+1 ) ;
//        WriteLineNo(k);
              }
              k++;
          }
          if ( k < Nlines ) EndLine = k;
          else EndLine = Nlines;
          while ( k < Nlines ) {
              kgSetOffTableCell ( Tbl , k*2+1 ) ;
              k++;
          }
          WriteTbl ( ) ;
      }
      Count = Dcount ( Slist ) ;
      Resetlink ( Slist ) ;
      V = ( DIV * ) kgGetNamedWidget ( D , ( char * ) "VertScroll" ) ;
      if ( Count <= Nlines ) kgSetWidgetVisibility ( V , 0 ) ;
      else {
          Vsize = ( double ) Nlines/Count*100;
          Vpos = 0;
          kgSetScrollLength ( V , Vsize ) ;
          kgSetScrollPos ( V , Vpos ) ;
      }
      kgUpdateWidget ( V ) ;
      kgUpdateWidget ( Tbl ) ;
      kgSetTableCursor ( Tbl , 1 ) ;
      kgSetDefaultAttnWidget ( Tmp , Tbl ) ;
      kgUpdateOn ( Tmp ) ;
  return ret;
}
int kgGetStringscleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
 /* pt[1] is outputs, if any  to be given to caller */
 /* pt[0] is inputs, given by caller */
  return ret;
}
int ModifykgGetStrings(void *Tmp,int GrpId) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
 /* pt[0] is inputs given by caller */
  DIA *d;
      int i , n;
      int xo , yo , xl , yl;
      int xres , yres;
      d = D->d;
      i = 0;while ( d [ i ] .t != NULL ) {;
          i++;
      };
      n = 1;
      strcpy ( D->name , "ScrollTable ver 1.0" ) ; /* Dialog name you may change */
      SB = ( DIL * ) kgGetNamedWidget ( D , ( char * ) "SplButn" ) ;
      xl = SB->x2 -SB->x1;
      yl = SB->y2 -SB->y1;
      kgDisplaySize ( & xres , & yres ) ;
      xo = ( D->xl-xl ) /2;
      SB->x1 = xo;
      SB->x2 = xo+xl;
      B1 = ( DIN * ) kgGetNamedWidget ( D , ( char * ) "Button1" ) ;
      xl = B1->x2 -B1->x1;
      xo = D->xl-xl-25;
      B1->x1 = xo;
      B1->x2 = xo+xl;
#if 0
  if(D->fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D->xo=D->yo=0; D->xl = xres-10; D->yl=yres-80;
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

int kgGetStringsCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
int kgGetStringsResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
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
int kgGetStringsWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  void **pt= (void **)kgGetArgPointer(Tmp); // Change as required
  return ret;
}
void ModifykgGetStringsGc(void *Tmp) {
   DIALOG *D;
   Gclr *gc;
   D = (DIALOG *)Tmp;
   gc = &(D->gc);
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   kgColorTheme(D,220,220,200);
   kgColorTheme1(D,220,220,200);
   kgColorTheme2(D,220,220,200);
   kgDefaultGuiTheme(gc);
   kgGrayGuiTheme(gc);
   gc->FontSize =9;
   gc->FontGuiSize =9;
   gc->InputFontSize =8;
   gc->MenuFont = 21;
   gc->PromptFont = 21;
   gc->ButtonFont = 21;
   gc->MsgFont = 21;
   gc->Font=23;
   kgMkgclr((char *)"kgGetStrings",Tmp);
*/
}
int kgGetStringsGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*20);
  e0[0].fmt = (char *)malloc(4);
  strcpy(e0[0].fmt,(char *)"%2s");
  e0[0].v=(void *)v[0];
  e0[0].sw=0;
  e0[0].noecho=0;
  e0[0].img=NULL;
  e0[1].fmt = (char *)malloc(5);
  strcpy(e0[1].fmt,(char *)"%40s");
  e0[1].v=(void *)v[1];
  e0[1].sw=1;
  e0[1].noecho=0;
  e0[1].img=NULL;
   int Count0= 2;
   for(int j=1;j<10;j++) {
     for(int i=0;i<2;i++) {
       e0[j*2+i].fmt = (char *)malloc(strlen(e0[i].fmt)+1);
       strcpy(e0[(j)*2+i].fmt, e0[i].fmt);
       e0[(j)*2+i].v = (void *)v[Count0+(j-1)*2+i];
       e0[(j)*2+i].sw = e0[i].sw;
       e0[(j)*2+i].noecho = e0[i].noecho;
       e0[(j)*2+i].img = NULL;
     }
   }
  DIT T0 = { 
    'T',
    10,38,  
    507,304,
    26, 
    2,10, 
    e0,
    1,1,
    NULL,kgGetStringstablebox1callback,1,0,9,9 /* args,Call back */
  };
  strcpy(T0.Wid,(char *)"ScrollTable");
  T0.pt=NULL;
  T0.type = 0;
  T0.item = -1;
  DIV v1 = { 
    'v',
    507,37,  
    527,305,  
    0,  
    100.000000,0.000000,5.000000,  
    NULL,kgGetStringsvertscroll1callback /* *args, callback */
  };
  strcpy(v1.Wid,(char *)"VertScroll");
  v1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"!c03Cancel");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=-235255250;
  butn2[0].butncode=127;
  butn2[1].sw=1;
  strcpy(butn2[1].title,(char *)"!c38Done");
  butn2[1].xpmn=NULL;
  butn2[1].xpmp=NULL;
  butn2[1].xpmh=NULL;
  butn2[1].bkgr=-235255250;
  butn2[1].butncode=127;
  DIL h2 = { 
    'h',
    190,307,  
    369,338,
    2,0,  
    84, 
    25, 
    2,1, 
    8,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    kgGetStringssplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h2.Wid,(char *)"SplButn");
  h2.item = -1;
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"!c33Delete");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-216226216;
  butn3[0].butncode=127;
  butn3[1].sw=1;
  strcpy(butn3[1].title,(char *)"Add");
  butn3[1].xpmn=NULL;
  butn3[1].xpmp=NULL;
  butn3[1].xpmh=NULL;
  butn3[1].bkgr=-216226216;
  butn3[1].butncode=127;
  butn3[2].sw=1;
  strcpy(butn3[2].title,(char *)"Insert");
  butn3[2].xpmn=NULL;
  butn3[2].xpmp=NULL;
  butn3[2].xpmh=NULL;
  butn3[2].bkgr=-216226216;
  butn3[2].butncode=127;
  DIN b3 = { 
    'n',
    281,12,  
    507,36,
    2,2,  
    72, 
    18, 
    3,1, 
    1,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    kgGetStringsbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b3.Wid,(char *)"Button1");
  b3.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+5));
  d =dtmp+i; 
  d[4].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = T0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIV));
  *d[1].v = v1;
  d[1].v->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIL));
  *d[2].h = h2;
  d[2].h->item = -1;
  kgGetStringssplbutton1init(d[2].h,pt) ;
  d[3].t = (DIT *)malloc(sizeof(DIN));
  *d[3].N = b3;
  d[3].N->item = -1;
  kgGetStringsbutton1init(d[3].N,pt) ;
  d[4].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakekgGetStringsGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    TableBox1  20 data values

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(char)*500);
   v0[0] = '\0';
   char  *v1 ;
   v1 = (char *)malloc(sizeof(char)*500);
   v1[0] = '\0';
   char  *v2 ;
   v2 = (char *)malloc(sizeof(char)*500);
   v2[0] = '\0';
   char  *v3 ;
   v3 = (char *)malloc(sizeof(char)*500);
   v3[0] = '\0';
   char  *v4 ;
   v4 = (char *)malloc(sizeof(char)*500);
   v4[0] = '\0';
   char  *v5 ;
   v5 = (char *)malloc(sizeof(char)*500);
   v5[0] = '\0';
   char  *v6 ;
   v6 = (char *)malloc(sizeof(char)*500);
   v6[0] = '\0';
   char  *v7 ;
   v7 = (char *)malloc(sizeof(char)*500);
   v7[0] = '\0';
   char  *v8 ;
   v8 = (char *)malloc(sizeof(char)*500);
   v8[0] = '\0';
   char  *v9 ;
   v9 = (char *)malloc(sizeof(char)*500);
   v9[0] = '\0';
   char  *v10 ;
   v10 = (char *)malloc(sizeof(char)*500);
   v10[0] = '\0';
   char  *v11 ;
   v11 = (char *)malloc(sizeof(char)*500);
   v11[0] = '\0';
   char  *v12 ;
   v12 = (char *)malloc(sizeof(char)*500);
   v12[0] = '\0';
   char  *v13 ;
   v13 = (char *)malloc(sizeof(char)*500);
   v13[0] = '\0';
   char  *v14 ;
   v14 = (char *)malloc(sizeof(char)*500);
   v14[0] = '\0';
   char  *v15 ;
   v15 = (char *)malloc(sizeof(char)*500);
   v15[0] = '\0';
   char  *v16 ;
   v16 = (char *)malloc(sizeof(char)*500);
   v16[0] = '\0';
   char  *v17 ;
   v17 = (char *)malloc(sizeof(char)*500);
   v17[0] = '\0';
   char  *v18 ;
   v18 = (char *)malloc(sizeof(char)*500);
   v18[0] = '\0';
   char  *v19 ;
   v19 = (char *)malloc(sizeof(char)*500);
   v19[0] = '\0';
   void** v=(void **)malloc(sizeof(void*)*21);
   v[20]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   v[4]=(void *)(v4);
   v[5]=(void *)(v5);
   v[6]=(void *)(v6);
   v[7]=(void *)(v7);
   v[8]=(void *)(v8);
   v[9]=(void *)(v9);
   v[10]=(void *)(v10);
   v[11]=(void *)(v11);
   v[12]=(void *)(v12);
   v[13]=(void *)(v13);
   v[14]=(void *)(v14);
   v[15]=(void *)(v15);
   v[16]=(void *)(v16);
   v[17]=(void *)(v17);
   v[18]=(void *)(v18);
   v[19]=(void *)(v19);
   void *pt=NULL; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   GrpId = kgGetStringsGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int kgGetStrings( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = kgGetStringsGroup(&D,v,pt);
#else 
  GrpId = MakekgGetStringsGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 3;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 228;   /* Position of Dialog */ 
  D.yo = 131;
  D.xl = 538;    /*  Length of Dialog */
  D.yl = 344;    /*  Width  of Dialog */
  D.Initfun = kgGetStringsinit;    /*   init fuction for Dialog */
  D.Cleanupfun = kgGetStringscleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = kgGetStringsCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = kgGetStringsResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = kgGetStringsWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.NoWinMngr = 0;    /*  1 for no Window Manager*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 2.0");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
  ModifykgGetStrings(&D,GrpId);    /*  add extras to  gui*/
  ModifykgGetStringsGc(&D);    /*  set colors for gui if don't like default*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunkgGetStrings(void *parent ,void *args) {
/*************************************************

    TableBox1  20 data values

*************************************************/
   char  v0[500]="" ;
   char  v1[500]="" ;
   char  v2[500]="" ;
   char  v3[500]="" ;
   char  v4[500]="" ;
   char  v5[500]="" ;
   char  v6[500]="" ;
   char  v7[500]="" ;
   char  v8[500]="" ;
   char  v9[500]="" ;
   char  v10[500]="" ;
   char  v11[500]="" ;
   char  v12[500]="" ;
   char  v13[500]="" ;
   char  v14[500]="" ;
   char  v15[500]="" ;
   char  v16[500]="" ;
   char  v17[500]="" ;
   char  v18[500]="" ;
   char  v19[500]="" ;
   void* v[20];
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   v[4]=(void *)(v4);
   v[5]=(void *)(v5);
   v[6]=(void *)(v6);
   v[7]=(void *)(v7);
   v[8]=(void *)(v8);
   v[9]=(void *)(v9);
   v[10]=(void *)(v10);
   v[11]=(void *)(v11);
   v[12]=(void *)(v12);
   v[13]=(void *)(v13);
   v[14]=(void *)(v14);
   v[15]=(void *)(v15);
   v[16]=(void *)(v16);
   v[17]=(void *)(v17);
   v[18]=(void *)(v18);
   v[19]=(void *)(v19);
   void *pt[2]={NULL,NULL}; /* pointer to send any extra information */
                  /* it will be aviilable in Callbacks */
   pt[0]=args;
   kgGetStrings(parent,v,(void *)pt );
   return pt[1];
}
