#include <kulina.h>
#include "ArrangeVideoCallbacks.h"
#include "kgutils.h"
#include <math.h>
  static void *Args = NULL , *Rets = NULL;
  static DIAINTR *It = NULL;
  static MODINTERFACE ModFuns [ ] = {
   ( MODINTERFACE ) NULL };
  static Dlink *ModuleList = NULL;
  char * MakeArrangeVideoFile ( void ) {
      char buff [ 500 ] , *pt;
      int id = 0 , ln;
      sprintf ( buff , "%-s/" , getenv ( "HOME" ) ) ;
      ln = strlen ( buff ) ;
      pt = buff+ln;
      while ( 1 ) {
          sprintf ( pt , "ArrangedVideo_%-4.4d.mp4" , id ) ;
//    printf("%s\n",buff);
          if ( ! FileStat ( buff ) ) break;
          id++;
      }
      ln = strlen ( buff ) ;
      pt = ( char * ) malloc ( ln+1 ) ;
      strcpy ( pt , buff ) ;
      return pt;
  }
  int DeleteSelectedFromList ( DIX *X ) {
      ThumbNail **Th = ( ThumbNail ** ) kgGetList ( X ) ;
      Dlink *L = Dopen ( ) ;
      ThumbNail *tpt = NULL;
      int i = 0;
      while ( Th [ i ] != NULL ) {
//      printf("Name: %s sw= %d \n",Th[i]->name,Th[i]->sw);
          if ( Th [ i ]->sw == 1 ) kgFreeThumbNail ( Th [ i ] ) ;
          else Dadd ( L , Th [ i ] ) ;
          i++;
      }
      i = 0;
      Resetlink ( L ) ;
      while ( ( tpt = ( ThumbNail * ) Getrecord ( L ) ) != NULL ) Th [ i++ ] = tpt;
      Th [ i ] = NULL;
      kgSetList ( X , ( void ** ) Th ) ;
      kgUpdateWidget ( X ) ;
      Dfree ( L ) ;
      return 1;
  }
 /* Callback for  AVMbutns   */ 
  int ArrangeVideoAVMbutnscallback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIN *B;
      static char filename [ 500 ] = "";
      ThumbNail **th , **Xlist;
      char **Str = NULL;
      static int Entry = 1;
      int n , ret = 0;
      D = ( DIALOG * ) Tmp;
      DIX *VX2 = ( DIX * ) kgGetNamedWidget ( Tmp , ( char * ) "AVMlist" ) ;
      B = ( DIN * ) kgGetWidget ( Tmp , i ) ;
      Dlink *L;
      n = B->nx*B->ny;
      switch ( butno ) {
          case 1:
#if 1
          if ( Entry ) {
              char *fpt = MakeArrangeVideoFile ( ) ;
              DIT *TO = ( DIT * ) kgGetNamedWidget ( Tmp , ( char * ) "AVMout" ) ;
              kgSetString ( TO , 0 , fpt ) ;
              kgUpdateWidget ( TO ) ;
              kgUpdateOn ( Tmp ) ;
              free ( fpt ) ;
              Entry = 0;
          }
#endif
          Str = kgGetVideoFiles ( NULL ) ;
          if (( Str != NULL) &&(Str[0] != NULL)) {
              th = kgStringToThumbNails ( Str ) ;
              kgFreeDouble ( ( void ** ) Str ) ;
          }
          i = 0;
          while ( th [ i ] != NULL ) {
              kgAddThumbNail ( VX2 , th [ i ] , -1 ) ;
              i++;
          }
          kgUpdateWidget ( VX2 ) ;
          kgUpdateOn ( Tmp ) ;
          Str = NULL;
          break;
          case 2:
          DeleteSelectedFromList ( VX2 ) ;
          kgUpdateOn ( Tmp ) ;
          break;
      }
      return ret;
  }
  void ArrangeVideoAVMbutnsinit ( DIN *B , void *ptmp ) {
      void **pt = ( void ** ) ptmp; //pt [ 0 ] is arg 
// may use kgChangeButtonNormalImage etc...
      BUT_STR *buts;
      buts = ( BUT_STR * ) ( B->buts ) ;
  }
 /* Callback for  AVMres   */ 
  int ArrangeVideoAVMrescallback ( int cellno , int i , void *Tmp ) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
      DIALOG *D;DIT *T;T_ELMT *e;
      int ret = 1;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
      D = ( DIALOG * ) Tmp;
      T = ( DIT * ) kgGetWidget ( Tmp , i ) ;
      e = T->elmt;
      return ret;
  }

  int GetRowNumber(Dlink *L,int Xres,int Yres) {
    int Nrow=1,i=0,j=0;
    Dlink *Ltmp;
    MEDIAINFO *tpt,*tlast;;
    int n = Dcount(L);
    int nr = 0;
    int nq = 0;
    int xv,yv,Xreso=Xres;
    float yfac,xfac;
    int Xrow,Yrow,Ysum,Xsum;
    MEDIAINFO **M,*ttmp=NULL;
    Dend(L);
    tlast = (MEDIAINFO *)Getrecord(L);
    Nrow=1;
    while(1) {
      n = Dcount(L);
      Ltmp = Dcopy(L);
      Xres = (Xres/Nrow)*Nrow;
      xv = Xres;
      nr = n%Nrow;
      nq = n/Nrow;
      if(ttmp != NULL){ free(ttmp);ttmp=NULL;}
      if(nr > 0){
        ttmp = (MEDIAINFO *)malloc(sizeof(MEDIAINFO)*(Nrow-nr));      
        for(i=0;i<(Nrow-nr);i++){
          ttmp[i]=*tlast;
          Dappend(Ltmp,ttmp+i);
        }
      }
//      n = Dcount(Ltmp);
      nq = Dcount(Ltmp)/Nrow;
      nr =0;
      yv =Yres/nq;
      Resetlink(Ltmp);
      M = (MEDIAINFO **)Dlinktoarray(Ltmp);
      Resetlink(Ltmp);
      while( (tpt = (MEDIAINFO *)Getrecord(Ltmp))!= NULL){
         yfac = (float)yv/tpt->Ayres;
         tpt->Rxres = tpt->Axres*yfac+0.0001;
         tpt->Ryres = yv;
      }
      Resetlink(Ltmp);
      for(i=0;i<nq;i++) {
         Xrow=0;
         for(j=0;j<Nrow;j++) {
           tpt = M[i*Nrow+j];
           Xrow = Xrow + tpt->Rxres;
         }
         xfac = (float)Xres/Xrow;
         Xsum =0;
         for(j=0;j<Nrow;j++) {
           tpt = M[i*Nrow+j];
           tpt->Rxres = tpt->Rxres*xfac;
           tpt->Ryres = tpt->Ryres*xfac;
           Xsum = Xsum +  tpt->Rxres; 
         }
         tpt->Rxres = tpt->Rxres +Xres - Xsum;
      }            
      Ysum =0;
      for(i=0;i<nq;i++) {
         tpt = M[i*Nrow];
         Ysum = Ysum + tpt->Ryres;
      }
      Dfree(Ltmp);
      free(M);
      if(Ysum > (Yres*1.3)) {
       Nrow++;
       Xres = Xreso;
      }
      else {
       if(Ysum > Yres) {
         Xres= 0.95*Xres;
       }
       else   break;
      }
    }
#if 1
    if(ttmp != NULL) free(ttmp);
      fprintf(stderr,"Nrow : %d\n",Nrow);
      Resetlink(L);
      while( (tpt = (MEDIAINFO *)Getrecord(L))!= NULL){
         fprintf(stderr,"Rxres: %d Ryres: %d\n",tpt->Rxres,tpt->Ryres);
    }
    fflush(stderr);
    Resetlink(L);
#endif
    return Nrow;
  }


 /* Callback for  AVMgo   */ 
  int ArrangeVideoAVMgocallback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIL *B;
      int n , ret = 0;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
      D = ( DIALOG * ) Tmp;
      B = ( DIL * ) kgGetWidget ( Tmp , i ) ;
      n = B->nx;
      ThumbNail **Th;
      Dlink *L = Dopen ( ) ;
      Dlink *PL = Dopen ( ) ;
      Dlink *NL = Dopen ( ) ;
      DIX *VX2 = ( DIX * ) kgGetNamedWidget ( Tmp , ( char * ) "AVMlist" ) ;
      Th = ( ThumbNail ** ) kgGetList ( VX2 ) ;
      MEDIAINFO *tpt , *ptpt , *tpt1 , *tpt2,**M=NULL;
      int Nv=0,Ny,Nr,txres=0;
      int ix=0,iy=0;
      float MaxSec = 0;
      float Ssec =0;
      char Tfolder [ 300 ] , NewFile [ 300 ] , NewFile1 [ 300 ] ;
      DIT *TO = ( DIT * ) kgGetNamedWidget ( Tmp , ( char * ) "AVMout" ) ;
      DIT *TR = ( DIT * ) kgGetNamedWidget ( Tmp , ( char * ) "AVMres" ) ;
      int Xres = kgGetInt ( TR , 0 ) ;
      int Yres = kgGetInt ( TR , 1 ) ;
      MakeTmpFolderInHome ( Tfolder ) ;
      int Nrow=4;
      int Sync = kgGetSelection ( kgGetNamedWidget  \
          ( Tmp , ( char * ) "AVMradio" ) ) %2;
      n = 0;
      if((Th==NULL)||(Th[0]==NULL)) return 0;
      while ( Th [ n ] != NULL ) {
              tpt = GetMediaInfo ( Th [ n ]->name ) ;
              Dadd ( L , tpt ) ;
              if ( tpt->TotSec > MaxSec ) MaxSec = tpt->TotSec;
              n++;
      }
      Resetlink(L);
      Nrow = GetRowNumber(L,Xres,Yres);
      Resetlink(L);
      while ( ( tpt = ( MEDIAINFO * ) Getrecord ( L ) ) != NULL ) {
         MakeFileInFolder ( "/tmp/Video.mp4" , Tfolder , NewFile , "mp4" ) ;
         ChangeVideoSize(tpt->Flname,NewFile,tpt->Rxres,tpt->Ryres,1);
         tpt1 = GetMediaInfo(NewFile);
         Dadd(PL,tpt1);
      }
      Dempty(L);
      L = Dcopy(PL);
      Dfree(PL);
      PL = Dopen();
      Resetlink ( L ) ;
      Nv = n;
      Ny = (Nv/Nrow)*Nrow;
      Nr = Nv - Ny;;
      
      M = (MEDIAINFO **) Dlinktoarray(L);      
      Resetlink ( L ) ;

      if ( Sync ) {
          while ( ( tpt = ( MEDIAINFO * ) Getrecord ( L ) ) != NULL ) {
              MakeFileInFolder ( "/tmp/Video.mp4" , Tfolder , NewFile , "mp4" ) ;
              if ( fabsf ( MaxSec-tpt->TotSec ) > 0.001 ) AddStillAtEnd  \
                  ( tpt->Flname , MaxSec-tpt->TotSec , NewFile ) ;
              else strcpy ( NewFile , tpt->Flname ) ;
              ptpt = GetMediaInfo ( NewFile ) ;
              Dadd ( PL , ptpt ) ;
          }
      }
      else {
          if ( ( tpt = ( MEDIAINFO * ) Getrecord ( L ) ) != NULL ) {
              MakeFileInFolder ( "/tmp/Video.mp4" , Tfolder , NewFile , "mp4" ) ;
              if ( fabsf ( MaxSec-tpt->TotSec ) > 0.001 ) AddStillAtEnd  \
                  ( tpt->Flname , MaxSec-tpt->TotSec , NewFile ) ;
              else strcpy ( NewFile , tpt->Flname ) ;
              ptpt = GetMediaInfo ( NewFile ) ;
              Ssec = tpt->TotSec;
              Dadd ( PL , ptpt ) ;
          }
          while ( ( tpt = ( MEDIAINFO * ) Getrecord ( L ) ) != NULL ) {
              MakeFileInFolder ( "/tmp/Video.mp4" , Tfolder , NewFile , "mp4" ) ;
              if ( fabsf ( MaxSec-tpt->TotSec -Ssec) > 0.001 ) AddStillAtEnd  \
                  ( tpt->Flname, MaxSec-tpt->TotSec-Ssec , NewFile ) ;
              else strcpy ( NewFile , tpt->Flname ) ;
              MakeFileInFolder ( "/tmp/Video.mp4" , Tfolder , NewFile1 , "mp4" ) ;
              if ( fabsf ( Ssec ) > 0.001 ) AddStillAtStart  \
                  ( NewFile , Ssec , NewFile1 ) ;
              else strcpy ( NewFile1 , NewFile ) ;
              ptpt = GetMediaInfo ( NewFile1 ) ;
              Ssec += tpt->TotSec;
              Dadd ( PL , ptpt ) ;
          }
      }
      Resetlink ( PL ) ;
      free(M);
      M = (MEDIAINFO **) Dlinktoarray(PL);      
      Resetlink ( PL ) ;
      iy =0;
      while(iy < Ny) {
          tpt1 = M[iy];
          tpt = tpt1;
          for(ix = 1;ix<Nrow;ix++) {
              tpt2 =M[iy+ix];
              MakeFileInFolder ( "/tmp/Video.mp4" , Tfolder , NewFile , "mp4" ) ;
              VideoSideBySide ( tpt1->Flname , tpt2->Flname , NewFile ) ;
              tpt = GetMediaInfo ( NewFile ) ;
              tpt1 =tpt;
          }
          Dadd ( NL , tpt ) ;
          iy = iy+Nrow;
          txres=tpt->Axres;
      }
      if(Nr>0) {
         tpt1 = M[Ny];
         for(ix=1;ix<Nr;ix++) {
            tpt2 =M[Ny+ix];
            MakeFileInFolder ( "/tmp/Video.mp4" , Tfolder , NewFile , "mp4" ) ;
            VideoSideBySide ( tpt1->Flname , tpt2->Flname , NewFile ) ;
            tpt = GetMediaInfo ( NewFile ) ;
            tpt1 =tpt;
         }
         if(Ny >0) {
           int Xr = (int)(txres/Nrow);
           Xr = ((Xr*Nr)/2)*2;
           MakeFileInFolder ( "/tmp/Video.mp4" , Tfolder , NewFile , "mp4" ) ;
           ChangeVideoSize(tpt1->Flname,NewFile,Xr,-2,1);
           tpt = GetMediaInfo(NewFile);
           MakeFileInFolder ( "/tmp/Video.mp4" , Tfolder , NewFile , "mp4" ) ;
           OverlayToSize(txres,tpt->Ayres,tpt->fps,1,tpt->Flname,NewFile);
           tpt = GetMediaInfo(NewFile);
         }
         Dadd(NL,tpt);
      }
      Resetlink ( NL ) ;
      if ( ( tpt = ( MEDIAINFO * ) Getrecord ( NL ) ) != NULL )  \
          {strcpy ( NewFile , tpt->Flname ) ;}
      while ( ( tpt = ( MEDIAINFO * ) Getrecord ( NL ) ) != NULL ) {
          MakeFileInFolder ( "/tmp/Video.mp4" , Tfolder , NewFile1 , "mp4" ) ;
          VideoTopBottom ( NewFile , tpt->Flname , NewFile1 ) ;
          strcpy ( NewFile , NewFile1 ) ;
      }
      tpt = GetMediaInfo ( NewFile ) ;
      OverlayToSize ( Xres , Yres  , tpt->fps , 1 , \
           NewFile , kgGetString ( TO , 0 ) ) ;
      free ( tpt ) ;
      free(M);
      Dempty ( L ) ;
      if(n>1) {
        Dempty ( PL ) ;
        Dempty ( NL ) ;
      }
      else {
        Dfree(PL);
        Dfree(NL);
      }
      kgCleanDir(Tfolder);  
      switch ( butno ) {
          case 1: // Process 
          break;
      }
      return ret;
  }
  void ArrangeVideoAVMgoinit ( DIL *B , void *ptmp ) {
      void **pt = ( void ** ) ptmp; //pt [ 0 ] is arg 
// may use kgChangeButtonNormalImage etc...
      BUT_STR *buts;
      buts = ( BUT_STR * ) ( B->buts ) ;
  }
 /* Callback for  AVMout   */ 
  int ArrangeVideoAVMoutcallback ( int cellno , int i , void *Tmp ) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
      DIALOG *D;DIT *T;T_ELMT *e;
      int ret = 1;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
      D = ( DIALOG * ) Tmp;
      T = ( DIT * ) kgGetWidget ( Tmp , i ) ;
      e = T->elmt;
      return ret;
  }
 /* Callback for  AVMbrowse   */ 
  int ArrangeVideoAVMbrowsecallback ( int butno , int i , void *Tmp ) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIN *B;
      int n , ret = 0;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
      D = ( DIALOG * ) Tmp;
      B = ( DIN * ) kgGetWidget ( Tmp , i ) ;
      n = B->nx*B->ny;
      switch ( butno ) {
          case 1: // Browse 
          char Flname [ 300 ] ;
          Flname [ 0 ] = '\0';
          if ( kgFolderBrowser ( Tmp , 2 , 2 , Flname , "*" ) ) {
              DIT *TO = ( DIT * ) kgGetNamedWidget ( Tmp , ( char * ) "AVMout" ) ;
              kgSetString ( TO , 0 , Flname ) ;
              kgUpdateWidget ( TO ) ;
              kgUpdateOn ( Tmp ) ;
          }
          break;
      }
      return ret;
  }
  void ArrangeVideoAVMbrowseinit ( DIN *B , void *ptmp ) {
      void **pt = ( void ** ) ptmp; //pt [ 0 ] is arg 
// may use kgChangeButtonNormalImage etc...
      BUT_STR *buts;
      buts = ( BUT_STR * ) ( B->buts ) ;
  }
 /* Callback for  AVMradio   */ 
  int ArrangeVideoAVMradiocallback ( int item , int i , void *Tmp ) {
  /*********************************** 
    item : selected item (1 to max_item)  not any specific relevence
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIRA *R;DIALOG *D;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
      ThumbNail **th;
      int ret = 1;
      D = ( DIALOG * ) Tmp;
      R = ( DIRA * ) kgGetWidget ( Tmp , i ) ;
      th = ( ThumbNail ** ) R->list;
      return ret;
  }
  void ArrangeVideoAVMradioinit ( DIRA *R , void *ptmp ) {
      void **pt = ( void ** ) ptmp; //pt [ 0 ] is arg 
  }
 /* Callback for  AVMlist   */ 
  int ArrangeVideoAVMlistcallback ( int item , int i , void *Tmp ) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      DIALOG *D;DIX *X;
      int ret = 1;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
      D = ( DIALOG * ) Tmp;
      X = ( DIX * ) kgGetWidget ( Tmp , i ) ;
      switch ( item ) {
          case 1:
          break;
      }
      return ret;
  }
  void ArrangeVideoAVMlistinit ( DIX *X , void *ptmp ) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
      void **pt = ( void ** ) ptmp; //pt [ 0 ] is arg 
  }
  int ArrangeVideoSetup ( void *Tmp , void *args ) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
  /* you add any initialisation here */
  /* useful for setting is used as MakeGroup */
      void **pt = ( void ** ) args;
      int *ipt;
      ipt = ( int * ) pt [ 0 ] ;
      *ipt = 1080;
      ipt = ( int * ) pt [ 1 ] ;
      *ipt = 1920;
      return 1;
  }
  void * ArrangeVideoCleanDia ( void *args ) {
  /*********************************** 
    args :  Pointer to args  
   ***********************************/ 
/* you add any cleaning  here */
      return NULL;
  }
  void * ArrangeVideoAction ( void *Tmp , void *Args ) {
      return NULL;
  }
  int ArrangeVideoOn ( void *itmp ) {
      DIAINTR * Dt = ( DIAINTR * ) itmp;
      if ( Dt == NULL ) Dt = ( DIAINTR * ) It;
      if ( Dt != NULL ) {
          if ( Dt->Dtmp != NULL ) kgSetGrpVisibility ( Dt->Dtmp , Dt->GrpId , 1 ) ;
          else return 0;
          return 1;
      }
      return 0;
  }
  int ArrangeVideoOff ( void *itmp ) {
      DIAINTR * Dt = ( DIAINTR * ) itmp;
      if ( Dt == NULL ) Dt = ( DIAINTR * ) It;
      if ( Dt != NULL ) {
          if ( Dt->Dtmp != NULL ) kgSetGrpVisibility ( Dt->Dtmp , Dt->GrpId , 0 ) ;
          else return 0;
          return 1;
      }
      return 0;
  }
  static char *GetPointer ( char *str ) {
      char *pt;
      pt = ( char * ) malloc ( strlen ( str ) +1 ) ;
      strcpy ( pt , str ) ;
      return pt;
  }
  void * ArrangeVideoInterface ( void *args , void *rets ) {
  /*********************************** 
   ***********************************/ 
      DIAINTR *it = ( DIAINTR * ) malloc ( sizeof ( DIAINTR ) ) ;
      it->GrpId = 0;
  // filled by MakeGroup  it->xsh=0;
      it->ysh = 0;
      it->RunDia = RunArrangeVideo;
      it->MakeGroup = MakeArrangeVideoGroup;
      it->Title = GetPointer ( ( char * ) "ArrangeVideo" ) ;
      it->Help = GetPointer ( ( char * ) "No help yet, request" ) ;
      it->Action = ArrangeVideoAction;
      it->Settings = ArrangeVideoSetup;
      it->Cleanup = ArrangeVideoCleanDia;
      if ( args != NULL ) Args = args;
      if ( rets != NULL ) Rets = rets;
      it->args = Args;
      it->rets = Rets;
      it->SwitchOn = ArrangeVideoOn;
      it->SwitchOff = ArrangeVideoOff;
      it->Dtmp = NULL; // fiiled by MakeGroup 
      It = it;
      return it;
  }
  int ArrangeVideoinit ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
      int ret = 1;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
 /* pt[0] is inputs, given by caller */
      return ret;
  }
  int ArrangeVideocleanup ( void *Tmp ) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 1;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
 /* pt[1] is outputs, if any  to be given to caller */
 /* pt[0] is inputs, given by caller */
      return ret;
  }
  int ModifyArrangeVideo ( void *Tmp , int GrpId ) {
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
 /* pt[0] is inputs given by caller */
      DIA *d;
      int i , n;
      kgCheckParentPosition ( Tmp ) ;
      d = D->d;
      if ( ModuleList == NULL ) ModuleList = kgGetModuleList  \
          ( ( void ** ) ModFuns ) ;
      i = 0;
      void *args = NULL;
      DIAINTR *Dt;
      Resetlink ( ModuleList ) ;
      while ( ( Dt = ( DIAINTR * ) Getrecord ( ModuleList ) ) != NULL ) {
          Dt->GrpId = Dt->MakeGroup ( Tmp , NULL ) ;
          kgShiftGrp ( Tmp , Dt->GrpId , Dt->xsh , Dt->ysh ) ;
          Dt->Settings ( Tmp , args ) ;
          i++;
      };
      i = 0;while ( d [ i ] .t != NULL ) {;
          i++;
      };
      n = 1;
//  strcpy(D->name,"Kulina Designer ver 3.0");    /*  Dialog name you may change */
#if 0
      if ( D->fullscreen != 1 ) { /* if not fullscreen mode */
          int xres , yres;
          kgDisplaySize ( & xres , & yres ) ;
      // D->xo=D->yo=0; D->xl = xres-10; D->yl=yres-80;
      }
      else { // for fullscreen
          int xres , yres;
          kgDisplaySize ( & xres , & yres ) ;
          D->xo = D->yo = 0; D->xl = xres; D->yl = yres;
//     D->StackPos = 1; // you may need it
      } /* end of fullscreen mode */
#endif
      return GrpId;
  }
  int ArrangeVideoCallBack ( void *Tmp , void *tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
      int ret = 0;
      DIALOG *D;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
      KBEVENT *kbe;
      D = ( DIALOG * ) Tmp;
      kbe = ( KBEVENT * ) tmp;
      if ( kbe->event == 1 ) {
          if ( kbe->button == 1 ) {
          }
      }
      return ret;
  }
  int ArrangeVideoResizeCallBack ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
      int ret = 0;
      int xres , yres , dx , dy;
      DIALOG *D;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
      D = ( DIALOG * ) Tmp;
      kgGetWindowSize ( D , & xres , & yres ) ;
      dx = xres - D->xl;
      dy = yres - D->yl;
  /* extra code */
      D->xl = xres;
      D->yl = yres;
      kgRedrawDialog ( D ) ;
      return ret;
  }
  int ArrangeVideoWaitCallBack ( void *Tmp ) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
      int ret = 0;
      void **pt = ( void ** ) kgGetArgPointer ( Tmp ) ; // Change as required
// pt[0] is args passed as inputs; pt[1] is output pointer
      return ret;
  }
