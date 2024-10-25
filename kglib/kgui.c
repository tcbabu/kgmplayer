/*
  Version 2.1
  DATE 8/07/97
  With file browser and improved GUI
  Ver 8.0
  Dated 31/07/2012
*/
#define VER 1401010100
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "kulina.h"
#include "gprivate.h"
#include "uievent.h"
#include "dlink.h"
#include <unistd.h>
#include <dirent.h>
#define O_BINARY 0x00
#define LN_WIDTH 20
#define far 
#include <malloc.h>
#define RESIZE 5
//#include "images.c"
  extern int TextSize , Ht , Wd , Gap , Bt; // It is Okay For Thread;
  static pthread_mutex_t _Tmplock = PTHREAD_MUTEX_INITIALIZER;
  static pthread_mutex_t fakeMutex = PTHREAD_MUTEX_INITIALIZER;
  static pthread_cond_t fakeCond = PTHREAD_COND_INITIALIZER;
  static int PID = 0;
  char * ui_mktmpdir ( void ) ;
  int CHDIR ( char *x ) {return chdir ( x ) ;}
  static int Hsize = 1;
  static int SLASH = 47;
  static char SLASHS [ 2 ] = "/";
#define FreeImg(x) { \
   {\
           if ( ( x ) != NULL ) kgFreeImage ( ( x ) ) ;\
           ( x ) = NULL;\
       }\
   }
#define Free(x) {\
   {\
           if ( ( x ) != NULL ) free ( ( x ) ) ;\
           ( x ) = NULL;\
       }\
   }
  static void uiFreemenu ( char **m ) {
      int i;
      if ( m != NULL ) {
          i = 0;
          while ( m [ i ] != NULL ) {free ( m [ i ] ) ;i++;}
          free ( m ) ;
      }
  }
  static int comppath ( void *pt1 , void *pt2 ) {
      return strcmp ( ( char * ) pt1 , ( char * ) pt2 ) ;
  }
  static int compdup ( void *pt1 , void *pt2 ) {
      int ret;
      ret = strcmp ( ( char * ) pt1 , ( char * ) pt2 ) ;
      if ( ret == 0 ) {return 1;}
      else return 0;
  }
  char *kgWhich ( char *pgr ) {
/* Caller must free result if it is not NULL */
      int i = 0 , j , End = 0;
      char *pt , **m , *res = NULL , *cpt;
      char path [ 5000 ] ;
      Dlink *L;
      if ( pgr [ 0 ] == '/' ) { // full path is given
          int l;
          char **m , *pt1;
          strcpy ( path , pgr ) ;
          pt = path;
          l = strlen ( pt ) ;
          while ( pt [ l ] != '/' ) l--;
          if ( l == 0 ) {
              m = kgFileMenu ( "/" , pt+1 ) ;
          }
          else {
              pt1 = pt+l+1;
              pt [ l ] = '\0';
              m = kgFileMenu ( pt , pt1 ) ;
          }
          if ( ( m == NULL ) ) {return NULL;}
          if ( ( m [ 0 ] == NULL ) ) {kgFreeDouble ( ( void ** ) m ) ;return NULL;}
          kgFreeDouble ( ( void ** ) m ) ;
          res = ( char * ) Malloc ( strlen ( pgr ) +1 ) ;
          strcpy ( res , pgr ) ;
          return res;
      }
      pt = getenv ( "PATH" ) ;
      if ( pt == NULL ) return NULL;
      strcpy ( path , getenv ( "PATH" ) ) ;
      L = Dopen ( ) ;
      while ( ! End ) {
          j = i;
          if ( pt [ j ] < ' ' ) break;
          while ( path [ j ] != ':' ) {
              if ( path [ j ] < ' ' ) {End = 1;break;}
              j++;
          }
          path [ j ] = '\0';
          cpt = ( char * ) Malloc ( strlen ( path+i ) +1 ) ;
          strcpy ( cpt , path+i ) ;
          Dadd ( L , cpt ) ;
          i = j+1;
      }
      Dsort ( L , comppath ) ;
      Drmvdup_cond ( L , compdup ) ;
      Resetlink ( L ) ;
      while ( ( pt = ( char * ) Getrecord ( L ) ) != NULL ) {
          m = kgFileMenu ( pt , pgr ) ;
          if ( m == NULL ) continue;
          if ( m [ 0 ] == NULL ) { free ( m ) ;continue;}
          res = ( char * ) Malloc ( strlen ( pt ) +1+strlen ( m [ 0 ] ) +1 ) ;
          strcpy ( res , pt ) ;
          strcat ( res , "/" ) ;
          strcat ( res , m [ 0 ] ) ;
          i = 0;
          while ( m [ i ] != NULL ) {free ( m [ i ] ) ;i++;}
          free ( m ) ;
          break;
      }
      Dempty ( L ) ;
      return res;
  }
  int kgCheckString ( char *s1 , char *s2 ) {
      int ch , ln , ret = 0;
      ln = strlen ( s2 ) ;
      if ( strlen ( s1 ) < ln ) ret = 0;
      else {
          ch = s1 [ ln ] ;
          s1 [ ln ] = '\0';
          if ( strcmp ( s1 , s2 ) == 0 ) ret = 1;
          s1 [ ln ] = ch;
      }
      return ret;
  }
  int kgSearchString ( char *s1 , char *s2 ) {
/*
  Checks for s2 in si and returns start location
  if returns -1 for search failed
*/
      int ch , ln , ret = -1 , ln1 , i = 0;
      ln = strlen ( s2 ) ;
      ln1 = strlen ( s1 ) ;
      if ( ln1 < ln ) ret = -1;
      else {
          i = 0;
          for ( i = 0;i <= ( ln1-ln ) ;i++ ) {
              if ( kgCheckString ( s1+i , s2 ) ) {
                  ret = i;
                  break;
              }
          }
      }
      return ret;
  }
  static int compname ( void *pt1 , void *pt2 ) {
      char *s1 , *s2;
      s2 = ( char * ) pt2;
      if ( kgSearchString ( s2 , "scala" ) >= 0 ) return 1;
      s1 = ( char * ) pt1;
      if ( isdigit ( s1 [ 0 ] ) && isdigit ( s2 [ 0 ] ) ) {
          int n1 , n2 , i;
          n1 = 0;
          i = 0;
          while ( isdigit ( s1 [ i ] ) ) {n1 = n1*10+ ( s1 [ i ] -'0' ) ;i++;}
          n2 = 0;
          i = 0;
          while ( isdigit ( s2 [ i ] ) ) {n2 = n2*10+ ( s2 [ i ] -'0' ) ;i++;}
          if ( n2 > n1 ) return 1;
      }
      return 0;
  }
  static char *uiSearchFolder ( char *Folder , char *Icon ) {
      char **Dirs , **Files;
      char *res = NULL , *cpt;
      char buff [ 500 ] ;
      int i , l , App = 0;
      Dlink *L;
      Files = kgFileMenu ( Folder , "*.png" ) ;
      if ( Files != NULL ) {
          if ( Files [ 0 ] != NULL ) {
              i = 0;
              while ( Files [ i ] != NULL ) {
                  l = i;
                  i++;
                  if ( kgSearchString ( Files [ l ] , Icon ) >= 0 ) {
                      strcpy ( buff , Folder ) ;
                      strcat ( buff , "/" ) ;
                      strcat ( buff , Files [ l ] ) ;
                      res = ( char * ) Malloc ( strlen ( buff ) +1 ) ;
                      strcpy ( res , buff ) ;
                      break;
                  }
              }
          } // Files [ 0 ] 
          kgFreeDouble ( ( void ** ) Files ) ;
          if ( res != NULL ) return res;
      }
/*  Going down throuch Folders */
      Dirs = kgFolderMenu ( Folder ) ;
      if ( Dirs == NULL ) return res;
      if ( Dirs [ 0 ] == NULL ) { kgFreeDouble ( ( void ** ) Dirs ) ; return res;}
#if 1
      i = 0;
      App = 0;
      while ( Dirs [ i ] != NULL ) {
          char *pt = NULL;
          pt = strstr ( Dirs [ i ] , "app" ) ;
          if ( pt != NULL ) {
              App = 1;
              break;
          }
          i++;
      }
#endif
      L = Dopen ( ) ;
      if ( App ) {
	  /* if an app folder is available only thar folder is checked */
          i = 0;
          while ( Dirs [ i ] != NULL ) {if ( strstr ( Dirs [ i ] , "app" ) != NULL ) Dadd \
           ( L , Dirs [ i ] ) ;i++;}
      }
      else {
          i = 0;
          while ( Dirs [ i ] != NULL ) {
              char *pt , buff [ 500 ] ;
              strcpy ( buff , Dirs [ i ] ) ;
	    /* Checking for <reso>x<reso> type , small ones ignored */
              pt = strstr ( buff , "x" ) ;
              if ( pt != NULL ) {
                  int reso , k , type = 1;
                  *pt = '\0';
                  k = 0;
                  while ( buff [ k ] != '\0' ) {
                      int j = 0;
                      if ( ! isdigit ( buff [ k ] ) ) {type = 0; break;}
                      k++;
                  }
                  if ( type ) {
                      sscanf ( buff , "%d" , & reso ) ;
                      if ( reso >= 128 ) Dadd ( L , Dirs [ i ] ) ;
                  }
                  else Dadd ( L , Dirs [ i ] ) ;
              }
              else Dadd ( L , Dirs [ i ] ) ;
              i++;
          }
      }
      free ( Dirs ) ;
      if ( Dcount ( L ) == 0 ) return res;
      Dsort ( L , compname ) ;
      Resetlink ( L ) ;
      while ( ( cpt = ( char * ) Getrecord ( L ) ) != NULL ) {
          strcpy ( buff , Folder ) ;
          strcat ( buff , "/" ) ;
          strcat ( buff , cpt ) ;
          res = uiSearchFolder ( buff , Icon ) ;
          if ( res != NULL ) break;
      }
      Dempty ( L ) ;
      return res;
  }
  char *kgGetIcon ( char *pgr , char *theme ) {
/* Caller must free result if it is not NULL */
      int i = 0 , j , k = 0 , End = 0;
      char *pt , **m , *res = NULL , *cpt , **m1 , **m2 , *ptmp;
      char buff [ 500 ] ;
      char buff1 [ 500 ] ;
      char buff2 [ 500 ] ;
      char path [ 5000 ] ;
      Dlink *L;
      if ( pgr [ 0 ] == '/' ) {
          int l;
          char **m , *pt1;
          strcpy ( path , pgr ) ;
          pt = path;
          l = strlen ( pt ) ;
          while ( pt [ l ] != '/' ) l--;
          if ( l == 0 ) {
              m = kgFileMenu ( "/" , pt+1 ) ;
          }
          else {
              pt1 = pt+l+1;
              pt [ l ] = '\0';
              m = kgFileMenu ( pt , pt1 ) ;
          }
          if ( ( m == NULL ) ) {return NULL;}
          if ( ( m [ 0 ] == NULL ) ) {kgFreeDouble ( ( void ** ) m ) ;return NULL;}
          kgFreeDouble ( ( void ** ) m ) ;
          res = ( char * ) Malloc ( strlen ( pgr ) +1 ) ;
          strcpy ( res , pgr ) ;
          return res;
      }
      pt = getenv ( "XDG_DATA_DIRS" ) ;
      if ( pt == NULL ) strcpy ( path , "/usr/share" ) ;
      else{
          ptmp = getenv ( "XDG_DATA_DIRS" ) ;
          strcpy ( path , ptmp ) ;
      }
      L = Dopen ( ) ;
      i = 0; pt = path;
      while ( ! End ) {
          j = i;
          if ( pt [ j ] < ' ' ) break;
          while ( path [ j ] != ':' ) {
              if ( path [ j ] < ' ' ) {End = 1;break;}
              j++;
          }
          path [ j ] = '\0';
          cpt = ( char * ) Malloc ( strlen ( path+i ) +1 ) ;
          strcpy ( cpt , path+i ) ;
          Dadd ( L , cpt ) ;
          i = j+1;
      }
      Dsort ( L , comppath ) ;
      Drmvdup_cond ( L , compdup ) ;
      Resetlink ( L ) ;
      while ( ( pt = ( char * ) Getrecord ( L ) ) != NULL ) {
          strcpy ( buff , pt ) ;
          strcat ( buff , "/" ) ;
          strcat ( buff , "icons" ) ;
          m = kgFolderMenu ( buff ) ;
          if ( m == NULL ) continue;
          if ( m [ 0 ] == NULL ) { kgFreeDouble ( ( void ** ) m ) ;continue;}
          i = 0;
          while ( m [ i ] != NULL ) {
              if ( kgSearchString ( m [ i ] , theme ) < 0 ) {i++;continue;}
              strcpy ( buff1 , buff ) ;
              strcat ( buff1 , "/" ) ;
              strcat ( buff1 , m [ i ] ) ;
              res = uiSearchFolder ( buff1 , pgr ) ;
              if ( res != NULL ) break;
              i++;
          }
      }
      Dempty ( L ) ;
      return res;
  }
  static int uiProcessIconName ( char *Icon ) {
      int pos , count = 0 , i;
      char *pt , *ptmp;
      if ( strstr ( Icon , ".png" ) != NULL ) return 1;
      if ( ( pt = strstr ( Icon , ".svg" ) ) != NULL ) {
          strcpy ( pt , ".png" ) ;
          return 1;
      }
      count = 0;
      pt = Icon;
      i = 0;while ( pt [ i ] >= ' ' ) i++;
      while ( pt [ i ] <= ' ' ) {pt [ i ] = '\0'; i--;if ( i == 0 ) break;}
      while ( ( ptmp = strstr ( pt , " " ) ) != NULL ) {*ptmp = '_';}
      while ( ( pt = strstr ( pt , "." ) ) != NULL ) {count++;pt++;}
      if ( count > 1 ) {
          pt = strstr ( Icon , "." ) +1;
          while ( ( ptmp = strstr ( pt , "." ) ) != NULL ) {
              pt = ptmp+1;
          }
          strcpy ( Icon , pt ) ;
//    strcat(Icon,".png");
          return 1;
      }
//  strcat(Icon,".png");
      return 1;
  }
  void *kgSearchIcon ( char *IconName ) {
      char *pt , *res = NULL , Icon [ 500 ] , *ptmp;
      strcpy ( Icon , IconName ) ;
      uiProcessIconName ( Icon ) ;
      pt = Icon;
      res = kgGetIcon ( pt , "oxygen" ) ;
      if ( res == NULL ) res = kgGetIcon ( pt , "Oxygen" ) ;
      if ( res == NULL ) res = kgGetIcon ( pt , "KDE" ) ;
      if ( res == NULL ) res = kgGetIcon ( pt , "gnome" ) ;
      if ( res == NULL ) res = kgGetIcon ( pt , "CratOS" ) ;
      if ( res == NULL ) res = kgGetIcon ( pt , "hicolor" ) ;
      if ( res == NULL ) {
          char **m2 , *ftmp;
          int i = 0;
          m2 = kgFolderMenu ( "/usr/share/icons" ) ;
          if ( m2 != NULL ) {
              while ( ( ftmp = m2 [ i ] ) != NULL ) {
                  i++;
                  if ( kgSearchString ( ftmp , "oxygen" ) >= 0 ) continue;
                  if ( kgSearchString ( ftmp , "Oxygen" ) >= 0 ) continue;
                  if ( kgSearchString ( ftmp , "KDE" ) >= 0 ) continue;
                  if ( kgSearchString ( ftmp , "gnome" ) >= 0 ) continue;
                  if ( kgSearchString ( ftmp , "CratOS" ) >= 0 ) continue;
                  if ( kgSearchString ( ftmp , "hicolor" ) >= 0 ) continue;
                  res = kgGetIcon ( pt , ftmp ) ;
                  if ( res != NULL ) break;
              }
              kgFreeDouble ( ( void ** ) m2 ) ;
          }
      }
      if ( res == NULL ) {
          ptmp = strstr ( Icon , "_" ) ;
          if ( ptmp != NULL ) {
              pt = ptmp+1;
              res = kgGetIcon ( pt , "hicolor" ) ;
              if ( res = NULL ) res = kgGetIcon ( pt , "oxygen" ) ;
              if ( res == NULL ) res = kgGetIcon ( pt , "gnome" ) ;
              if ( res == NULL ) res = kgGetIcon ( pt , "CratOS" ) ;
          }
      }
      if ( res == NULL ) {
          ptmp = strstr ( Icon , "-" ) ;
          if ( ptmp != NULL ) {
              *ptmp = '\0';
              res = kgGetIcon ( pt , "hicolor" ) ;
              if ( res = NULL ) res = kgGetIcon ( pt , "oxygen" ) ;
              if ( res == NULL ) res = kgGetIcon ( pt , "gnome" ) ;
              if ( res == NULL ) res = kgGetIcon ( pt , "CratOS" ) ;
          }
      }
      return res;
  }
  int _uiMenu_new ( BRW_STR *br ) {
      int ret;
      DIALOG *D;
      ret = br->df;
      D = br->D;
//   printf("Calling kgMenu\n");
      ret = kgMenu ( br->D , br->x1 , br->y1 , ret , ret , br->menu , br->size ) ;
      br->df = ret;
//   ui_draw_browser((DIW *)br->wid);
      _uiMake_W ( ( DIW * ) br->wid ) ;
      uiUpdateOn ( br->D ) ;
      return ret;
  }
  void opntwin ( DIALOG *D , int ix , int iy , int chrs , int lines ) {
      ui_txtwin ( D , ( int ) ix , ( int ) iy , ( int ) chrs , ( int ) lines ) ;
  }
  void Opntwin ( DIALOG *D , int ix , int iy , int chrs , int lines , int xbdr , int ybdr ) \
  {
      ui_txtwinnew ( D , ( int ) ix , ( int ) iy , ( int ) chrs , \
       ( int ) lines , xbdr , ybdr ) ;
  }
  void clstwin ( DIALOG *D ) {
      ui_clear_txtwin ( D ) ;
  }
  void wprintf ( DIALOG *D , char *s ) {
      KBEVENT kbe;
      ui_wprintf ( D , s ) ;
      kgCheckEvent ( D , & kbe ) ;
      kgCheckEvent ( D , & kbe ) ;
  }
  void gprintf ( DIALOG *D , char *s ) {
#if 0
      int i = 0 , j = 1 , lines = 0 , chars = 0;
      int No_w = 0;
      char *pt , ch;
      i = 0;
      while ( * ( s+i ) != '\0' ) {
          if ( ( * ( s+i ) < ' ' ) || ( j >= 70 ) ) {
              lines++;
              if ( j > chars ) chars = j;
              j = 1;
          }
          i++;j++;
      }
      if ( j > chars ) chars = j;
      lines++; if ( lines > 10 ) lines = 10;
      No_w = 1;
      opntwin ( D , 5L , 50L , chars+1 , lines ) ;
      i = 0; j = 1;
      pt = s;
      while ( * ( s+i ) != '\0' ) {
          if ( ( * ( s+i ) < ' ' ) || ( j >= chars ) ) {
              ch = * ( s+i ) ; * ( s+i ) = '\0';
              wprintf ( D , pt ) ; * ( s+i ) = ch;
              if ( j != chars ) { i++; }
              pt = s+i;
              j = 1;
          }
          else {i++;j++;}
      }
      if ( *pt != '\0' ) wprintf ( D , pt ) ;
      if ( No_w ) { getch ( ) ; clstwin ( D ) ; }
#else
      kgSplashMessage ( D , 10 , 10 , strlen ( s ) *8 , 22 , \
       ( char * ) s , 12 , 0 , 15 ) ;
#endif
  }
  void uiPrintf ( DIALOG *D , int xp , int yp , char *s ) {
      int i = 0 , j = 1 , lines = 0 , chars = 0;
      int No_w = 0;
      char *pt , ch;
      i = 0;
      while ( * ( s+i ) != '\0' ) {
          if ( ( * ( s+i ) < ' ' ) || ( j >= 70 ) ) {
              lines++;
              if ( j > chars ) chars = j;
              j = 1;
          }
          i++;j++;
      }
      if ( j > chars ) chars = j;
      lines++; if ( lines > 10 ) lines = 10;
      No_w = 1;
      opntwin ( D , xp , yp , chars+1 , lines ) ;
      i = 0; j = 1;
      pt = s;
      while ( * ( s+i ) != '\0' ) {
          if ( ( * ( s+i ) < ' ' ) || ( j >= chars ) ) {
              ch = * ( s+i ) ; * ( s+i ) = '\0';
              wprintf ( D , pt ) ; * ( s+i ) = ch;
              if ( j != chars ) { i++; }
              pt = s+i;
              j = 1;
          }
          else {i++;j++;}
      }
      if ( *pt != '\0' ) wprintf ( D , pt ) ;
//  if(No_w){ getch(); clstwin(); }
      if ( No_w ) { clstwin ( D ) ; }
  }
  void twinmove ( DIALOG *D , int col , int row ) {
      ui_twinmove ( D , ( int ) col , ( int ) row ) ;
  }
#if 0
  int gscanf_o ( void *D , void *unknown , ... ) {
      int i , item = 0 , j , jmax = 0 , it = 0 , k = 0 , err = 0 , size = 6;
      char *cpt , *chpt;float *fpt;double *dpt;int *ipt;int *lpt;
      char *tbuf , *buf , code [ 30 ] , dfmt [ 8 ] , ffmt [ 9 ] , sfmt [ 8 ] , *field [ MAXITEMS ] ;
          
      char *prompt [ MAXITEMS ] , *pt;
      char wrk [ 1000 ] ;
      int pln [ MAXITEMS ] , fln [ MAXITEMS ] ;
      va_list ad;
      va_start ( ad , unknown ) ;
      j = 0;
/*  cpt = va_arg(ad,char *);*/
      cpt = ( char * ) unknown;
      pt = cpt;
      while ( *cpt != '\0' ) {
          j++;
          if ( *cpt == '%' ) {
              if ( jmax < j ) jmax = j;
              pln [ item ] = j+1;
              j = 0;
              cpt++;
              size = 0;
              while ( isdigit ( *cpt ) ) {
                  size = size*10+ ( *cpt -'0' ) ;
                  cpt++;
              }
              fln [ item ] = size+1;
              if ( ( *cpt != 'd' ) && ( *cpt != 'f' ) && \
               ( *cpt != 's' ) && ( *cpt != 'F' ) ) {
                  normal ( ) ;
                  printf ( "Error in gscanf: /%s/\n" , ( char* ) pt ) ;
                  exit ( 0 ) ;
              }
              if ( ( *cpt == 's' ) && ( fln [ item ] <= 1 ) ) fln [ item ] = 30;
              if ( ( *cpt == 'f' ) && ( fln [ item ] <= 1 ) ) fln [ item ] = 15;
              if ( ( *cpt == 'F' ) && ( fln [ item ] <= 1 ) ) fln [ item ] = 15;
              if ( ( *cpt == 'd' ) && ( fln [ item ] <= 1 ) ) fln [ item ] = 5;
              if ( fln [ item ] < 30 ) fln [ item ] = 30;
//    field[item]=(char *)Malloc(sizeof(char)*fln[item]);
              field [ item ] = ( char * ) Malloc ( sizeof ( char ) *500 ) ;
              prompt [ item ] = ( char * ) Malloc ( sizeof ( char ) *pln [ item ] +10 ) ;
              code [ item ] = *cpt;
              item++;
          }
          cpt++;
      }
      if ( item > MAXITEMS ) {
          normal ( ) ;
          printf ( "Error: too many items in gscanf..\n" ) ;
          exit ( 0 ) ;
      }
/*
  va_start(ad);
  cpt = va_arg(ad,char *);
*/
      va_start ( ad , unknown ) ;
      cpt = ( char * ) unknown;
      j = 0;it = 0 , k = 0;
      while ( it < item ) {
          j++;
          if ( ( int ) ( *cpt ) == '%' ) {
//    prompt[it][k++]='\0';
              size = 0;
              size = fln [ it ] ;
              sprintf ( prompt [ it ] +k , "%%%ds" , size ) ;
              j = 0;k = 0;
              cpt++;
              while ( isdigit ( *cpt ) ) {
                  cpt++;
              }
              switch ( ( int ) * ( cpt ) ) {
                  case 'd':
                  lpt = va_arg ( ad , int * ) ;
                  sprintf ( dfmt , "%%-%-dd" , fln [ it ] -1 ) ;
                  sprintf ( wrk , dfmt , *lpt ) ;
                  wrk [ fln [ it ] -1 ] = '\0';
                  strcpy ( field [ it ] , wrk ) ;
                  break;
                  case 'f':
                  case 'g':
                  fpt = va_arg ( ad , float * ) ;
                  sprintf ( ffmt , "%%-%-df" , fln [ it ] -1 ) ;
                  sprintf ( wrk , ffmt , *fpt ) ;
                  wrk [ fln [ it ] -1 ] = '\0';
                  strcpy ( field [ it ] , wrk ) ;
                  break;
                  case 'F':
                  dpt = va_arg ( ad , double * ) ;
                  sprintf ( ffmt , "%%-%-dlf" , fln [ it ] -1 ) ;
                  sprintf ( wrk , ffmt , *dpt ) ;
                  wrk [ fln [ it ] -1 ] = '\0';
                  strcpy ( field [ it ] , wrk ) ;
                  break;
                  case 's':
                  chpt = va_arg ( ad , char * ) ;
                  sprintf ( sfmt , "%%-%-ds" , fln [ it ] -1 ) ;
                  sprintf ( wrk , sfmt , chpt ) ;
                  wrk [ fln [ it ] -1 ] = '\0';
                  strcpy ( field [ it ] , wrk ) ;
                  break;
              }
              it++;
          }
          else prompt [ it ] [ k++ ] = *cpt;
          cpt++;
      }
      while ( err == 0 ) {
          va_start ( ad , unknown ) ;
  /*cpt = va_arg(ad,char *);*/
          cpt = ( char * ) unknown;
//  gbell();
//  text_menu_n(X_menu,Y_menu,item,pln,prompt,fln,field);
          uiTextMenu ( D , 150 , 50 , item , prompt , field ) ;
          for ( i = 0;i < item;i++ ) {
              switch ( ( int ) code [ i ] ) {
                  case 'd':
                  lpt = va_arg ( ad , int * ) ;
                  err = sscanf ( field [ i ] , "%d" , lpt ) ;
                  break;
                  case 'f':
                  case 'g':
                  fpt = va_arg ( ad , float * ) ;
                  err = sscanf ( field [ i ] , "%f" , fpt ) ;
                  break;
                  case 'F':
                  dpt = va_arg ( ad , double * ) ;
                  err = sscanf ( field [ i ] , "%lf" , dpt ) ;
                  break;
                  case 's':
                  chpt = va_arg ( ad , char * ) ;
                  strcpy ( chpt , field [ i ] ) ;
                  stripblnk ( chpt ) ;
                  err = 1;
                  break;
              }
              if ( err < 1 ) break;
          }
      }
      for ( i = 0;i < item;i++ ) {free ( field [ i ] ) ;free ( prompt [ i ] ) ;}
  }
#endif
  int Up_Tx_Table ( int code , int i , DIA *d ) {
      DIALOG *D;
      int ret = 1;
      D = ( DIALOG * ) ( d [ i ] .t->D ) ;
      if ( d [ i ] .t->Update != NULL ) ret = d [ i ] .t->Update ( code , i , d ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_Bt_Box ( int code , int i , DIA *d ) {
      DIALOG *D;
      int ret = 1;
      D = ( DIALOG * ) ( d [ i ] .b->D ) ;
      if ( d [ i ] .b->Update != NULL ) ret = d [ i ] .b->Update ( code , i , d ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_Fsld_Bar ( double code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .f->Update != NULL ) ret = d [ i ] .f->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_Isld_Bar ( int code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .d->Update != NULL ) ret = d [ i ] .d->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_Hsld_Bar ( int code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .B->Update != NULL ) ret = d [ i ] .B->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_D_Tx_Table ( int code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .t->Update != NULL ) ret = d [ i ] .t->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_D_Table ( int code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .t->Update != NULL ) ret = d [ i ] .t->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_D_Y_Box ( int code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .y->Update != NULL ) ret = d [ i ] .y->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_D_Bt_Box ( int code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .b->Update != NULL ) ret = d [ i ] .b->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_D_Btn_Box ( int code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .N->Update != NULL ) ret = d [ i ] .N->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_D_SplBt_Box ( int code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .h->Update != NULL ) ret = d [ i ] .h->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_D_Ht_Box ( int code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .H->Update != NULL ) ret = d [ i ] .H->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_D_Brw_Box ( int code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .w->Update != NULL ) ret = d [ i ] .w->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_D_Menu_Box ( int code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .e->Update != NULL ) ret = d [ i ] .e->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int Up_D_NewMenu_Box ( int code , int i , DIALOG *D ) {
      int ret = 1;
      DIA *d;
      d = D->d;
      if ( d [ i ] .x->Update != NULL ) ret = d [ i ] .x->Update ( code , i , D ) ;
      else if ( D->Callback != NULL ) ret = D->Callback ( D , & ( D->kb ) ) ;
      return ret;
  }
  int uiCheckClickinDialog ( DIALOG *D ) {
      int i , ret = 0 , ch;
      if ( D->PON_X < D->xo ) return 0;
      if ( D->PON_X > ( D->xo+D->xl ) ) return 0;
      if ( D->PON_Y < D->yo ) return 0;
      if ( D->PON_Y > ( D->yo+D->yl ) ) return 0;
      return 1;
  }
  int check_pointer_click ( DIALOG *D , int *nr , int x1 , int y1 , DIA *d ) {
/* modified to add dynamic addition of widgets */
      int i , ret = 0 , ch , n;
      if ( D->PON_X < D->xo ) return -1;
      if ( D->PON_X > ( D->xo+D->xl ) ) return -1;
      if ( D->PON_Y < D->yo ) return -1;
      if ( D->PON_Y > ( D->yo+D->yl ) ) return -1;
      i = 0; while ( d [ i ] .t != NULL ) i++;
      n = i;
      *nr = n;
      for ( i = 0;i < n;i++ ) {
          if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) == 0 ) continue;
          ch = ( d [ i ] .t->code ) ;
          switch ( ch ) {
              case 't':
              case 'T':
              if ( uiCheckClickPosition ( d [ i ] .t->x1+x1 , d [ i ] .t->y1+y1 , d [ i ] .t->x2+x1 , d [ i ] .t->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'y':
              if ( uiCheckClickPosition ( d [ i ] .y->x1+x1 , d [ i ] .y->y1+y1 , d [ i ] .y->x2+x1 , d [ i ] .y->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'c':
              if ( uiCheckClickPosition ( d [ i ] .c->x1+x1 , d [ i ] .c->y1+y1 , d [ i ] .c->x2+x1 , d [ i ] .c->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'r':
              if ( uiCheckClickPosition ( d [ i ] .r->x1+x1 , d [ i ] .r->y1+y1 , d [ i ] .r->x2+x1 , d [ i ] .r->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'h':
              case 'H':
              if ( uiCheckClickPosition ( d [ i ] .h->x1+x1 , d [ i ] .h->y1+y1 , d [ i ] .h->x2+x1 , d [ i ] .h->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'b':
              case 'n':
              case 'N':
              if ( uiCheckClickPosition ( d [ i ] .b->x1+x1 , d [ i ] .b->y1+y1 , d [ i ] .b->x2+x1 , d [ i ] .b->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'f':
              if ( uiCheckClickPosition ( d [ i ] .f->x1+x1 , d [ i ] .f->y1+y1 , d [ i ] .f->x2+x1 , d [ i ] .f->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'd':
              if ( uiCheckClickPosition ( d [ i ] .d->x1+x1 , d [ i ] .d->y1+y1 , d [ i ] .d->x2+x1 , d [ i ] .d->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'P':
              if ( uiCheckClickPosition ( d [ i ] .B->x1+x1 , d [ i ] .B->y1+y1 , d [ i ] .B->x2+x1 , d [ i ] .B->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'w':
              if ( uiCheckClickPosition ( d [ i ] .w->x1+x1 , d [ i ] .w->y1+y1 , d [ i ] .w->x2+x1 , d [ i ] .w->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'x':
              if ( uiCheckClickPosition ( d [ i ] .x->x1+x1 , d [ i ] .x->y1+y1 , d [ i ] .x->x2+x1 , d [ i ] .x->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'e':
              if ( uiCheckClickPosition ( d [ i ] .e->x1+x1 , d [ i ] .e->y1+y1 , d [ i ] .e->x2+x1 , d [ i ] .e->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 's':
              if ( uiCheckClickPosition ( d [ i ] .s->x1+x1 , d [ i ] .s->y1+y1 , d [ i ] .s->x2+x1 , d [ i ] .s->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'v':
              if ( uiCheckClickPosition ( d [ i ] .v->x1+x1 , d [ i ] .v->y1+y1 , d [ i ] .v->x2+x1 , d [ i ] .v->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              case 'z':
              if ( uiCheckClickPosition ( d [ i ] .z->x1+x1 , d [ i ] .z->y1+y1 , d [ i ] .z->x2+x1 , d [ i ] .z->y2+y1 , D->PON_X , D->PON_Y ) == 1 ) return i%n;
                  
              break;
              default:
              break;
          }
      }
      return -1;
  }
  int draw_key_board_attn ( int i , DIALOG *D ) {
      int x1;int y1;DIA *d;
      int ret = 0 , ch;
      int X1 , Y1 , X2 , Y2;
      if ( ! D->kbattn ) return 0;
      x1 = D->xo;
      y1 = D->yo;
      d = D->d;
      ch = ( d [ i ] .t->code ) ;
      switch ( ch ) {
          case 't':
          X1 = d [ i ] .t->x1+x1;
          Y1 = d [ i ] .t->y1+y1;
          X2 = d [ i ] .t->x2+x1;
          Y2 = d [ i ] .t->y2+y1;
          break;
          case 'T':
          X1 = d [ i ] .t->x1+x1;
          Y1 = d [ i ] .t->y1+y1;
          X2 = d [ i ] .t->x2+x1;
          Y2 = d [ i ] .t->y2+y1;
          break;
          case 'v':
          X1 = d [ i ] .v->x1+x1;
          Y1 = d [ i ] .v->y1+y1;
          X2 = d [ i ] .v->x2+x1;
          Y2 = d [ i ] .v->y2+y1;
          break;
          case 'z':
          X1 = d [ i ] .z->x1+x1;
          Y1 = d [ i ] .z->y1+y1;
          X2 = d [ i ] .z->x2+x1;
          Y2 = d [ i ] .z->y2+y1;
          break;
          case 'x':
          X1 = d [ i ] .x->x1+x1;
          Y1 = d [ i ] .x->y1+y1;
          X2 = d [ i ] .x->x2+x1;
          Y2 = d [ i ] .x->y2+y1;
          break;
          case 'y':
          X1 = d [ i ] .y->x1+x1;
          Y1 = d [ i ] .x->y1+y1;
          X2 = d [ i ] .y->x2+x1;
          Y2 = d [ i ] .y->y2+y1;
          break;
          case 'r':
          X1 = d [ i ] .r->x1+x1;
          Y1 = d [ i ] .r->y1+y1;
          X2 = d [ i ] .r->x2+x1;
          Y2 = d [ i ] .r->y2+y1;
          break;
          case 'c':
          X1 = d [ i ] .c->x1+x1;
          Y1 = d [ i ] .c->y1+y1;
          X2 = d [ i ] .c->x2+x1;
          Y2 = d [ i ] .c->y2+y1;
          break;
          case 'h':
          case 'H':
          X1 = d [ i ] .h->x1+x1;
          Y1 = d [ i ] .h->y1+y1;
          X2 = d [ i ] .h->x2+x1;
          Y2 = d [ i ] .h->y2+y1;
          break;
          case 'b':
          case 'n':
          case 'N':
          X1 = d [ i ] .b->x1+x1;
          Y1 = d [ i ] .b->y1+y1;
          X2 = d [ i ] .b->x2+x1;
          Y2 = d [ i ] .b->y2+y1;
          break;
          case 'f':
          X1 = d [ i ] .f->x1+x1;
          Y1 = d [ i ] .f->y1+y1;
          X2 = d [ i ] .f->x2+x1;
          Y2 = d [ i ] .f->y2+y1;
          break;
          case 'd':
          X1 = d [ i ] .d->x1+x1;
          Y1 = d [ i ] .d->y1+y1;
          X2 = d [ i ] .d->x2+x1;
          Y2 = d [ i ] .d->y2+y1;
          break;
          case 'P':
          X1 = d [ i ] .B->x1+x1;
          Y1 = d [ i ] .B->y1+y1;
          X2 = d [ i ] .B->x2+x1;
          Y2 = d [ i ] .B->y2+y1;
          break;
          case 'w':
          X1 = d [ i ] .w->x1+x1;
          Y1 = d [ i ] .w->y1+y1;
          X2 = d [ i ] .w->x2+x1;
          Y2 = d [ i ] .w->y2+y1;
          break;
          case 'e':
          X1 = d [ i ] .e->x1+x1;
          Y1 = d [ i ] .e->y1+y1;
          X2 = d [ i ] .e->x2+x1;
          Y2 = d [ i ] .e->y2+y1;
          break;
          case 's':
          X1 = d [ i ] .s->x1+x1;
          Y1 = d [ i ] .s->y1+y1;
          X2 = d [ i ] .s->x2+x1;
          Y2 = d [ i ] .s->y2+y1;
          break;
          default:
          return -1;
      }
      switch ( D->kbattn ) {
          default:
          _ui_draw_attention ( D , X1 , Y1 , X2 , Y2 ) ;
          break;
      }
      return -1;
  }
  int rmv_key_board_attn ( int i , DIALOG *D ) {
      int x1;int y1;DIA *d;
      int ret = 0 , ch;
      int X1 , Y1 , X2 , Y2;
      if ( ! D->kbattn ) return 0;
      x1 = D->xo;
      y1 = D->yo;
      d = D->d;
      ch = ( d [ i ] .t->code ) ;
      switch ( ch ) {
          case 't':
          X1 = d [ i ] .t->x1+x1;
          Y1 = d [ i ] .t->y1+y1;
          X2 = d [ i ] .t->x2+x1;
          Y2 = d [ i ] .t->y2+y1;
          break;
          case 'T':
          X1 = d [ i ] .t->x1+x1;
          Y1 = d [ i ] .t->y1+y1;
          X2 = d [ i ] .t->x2+x1;
          Y2 = d [ i ] .t->y2+y1;
          break;
          case 'v':
          X1 = d [ i ] .v->x1+x1;
          Y1 = d [ i ] .v->y1+y1;
          X2 = d [ i ] .v->x2+x1;
          Y2 = d [ i ] .v->y2+y1;
          break;
          case 'z':
          X1 = d [ i ] .z->x1+x1;
          Y1 = d [ i ] .z->y1+y1;
          X2 = d [ i ] .z->x2+x1;
          Y2 = d [ i ] .z->y2+y1;
          break;
          case 'x':
          X1 = d [ i ] .x->x1+x1;
          Y1 = d [ i ] .x->y1+y1;
          X2 = d [ i ] .x->x2+x1;
          Y2 = d [ i ] .x->y2+y1;
          break;
          case 'y':
          X1 = d [ i ] .y->x1+x1;
          Y1 = d [ i ] .y->y1+y1;
          X2 = d [ i ] .y->x2+x1;
          Y2 = d [ i ] .y->y2+y1;
          break;
          case 'r':
          X1 = d [ i ] .r->x1+x1;
          Y1 = d [ i ] .r->y1+y1;
          X2 = d [ i ] .r->x2+x1;
          Y2 = d [ i ] .r->y2+y1;
          break;
          case 'c':
          X1 = d [ i ] .c->x1+x1;
          Y1 = d [ i ] .c->y1+y1;
          X2 = d [ i ] .c->x2+x1;
          Y2 = d [ i ] .c->y2+y1;
          break;
          case 'h':
          case 'H':
          X1 = d [ i ] .h->x1+x1;
          Y1 = d [ i ] .h->y1+y1;
          X2 = d [ i ] .h->x2+x1;
          Y2 = d [ i ] .h->y2+y1;
          break;
          case 'b':
          case 'n':
          case 'N':
          X1 = d [ i ] .b->x1+x1;
          Y1 = d [ i ] .b->y1+y1;
          X2 = d [ i ] .b->x2+x1;
          Y2 = d [ i ] .b->y2+y1;
          break;
          case 'f':
          X1 = d [ i ] .f->x1+x1;
          Y1 = d [ i ] .f->y1+y1;
          X2 = d [ i ] .f->x2+x1;
          Y2 = d [ i ] .f->y2+y1;
          break;
          case 'd':
          X1 = d [ i ] .d->x1+x1;
          Y1 = d [ i ] .d->y1+y1;
          X2 = d [ i ] .d->x2+x1;
          Y2 = d [ i ] .d->y2+y1;
          break;
          case 'P':
          X1 = d [ i ] .B->x1+x1;
          Y1 = d [ i ] .B->y1+y1;
          X2 = d [ i ] .B->x2+x1;
          Y2 = d [ i ] .B->y2+y1;
          break;
          case 'w':
          X1 = d [ i ] .w->x1+x1;
          Y1 = d [ i ] .w->y1+y1;
          X2 = d [ i ] .w->x2+x1;
          Y2 = d [ i ] .w->y2+y1;
          break;
          case 'e':
          X1 = d [ i ] .e->x1+x1;
          Y1 = d [ i ] .e->y1+y1;
          X2 = d [ i ] .e->x2+x1;
          Y2 = d [ i ] .e->y2+y1;
          break;
          case 's':
          X1 = d [ i ] .s->x1+x1;
          Y1 = d [ i ] .s->y1+y1;
          X2 = d [ i ] .s->x2+x1;
          Y2 = d [ i ] .s->y2+y1;
          break;
          default:
          return -1;
      }
      switch ( D->kbattn ) {
          default:
          _ui_rmv_attention ( D , X1 , Y1 , X2 , Y2 ) ;
          break;
      }
      return -1;
  }
  int _uiSetupGboxes ( DIALOG *D ) {
      int n , i , controls = 0 , item , ch , oldi = -1;
      DIA *d;
      DIG *g;
      d = D->d;
      i = 0;
      while ( d [ i ] .t != NULL ) {
          ch = ( d [ i ] .t->code ) ;
          if ( ch == 'g' ) {
              g = d [ i ] .g;
              g->D = D;
              uiInitGbox ( D , i ) ;
              if ( g->initgraph != NULL ) {
                  g->initgraph ( i , D ) ;
              }
//       D->df=0;
          }
          i++;
      }
  }
  int kgDrawDialog ( DIALOG *D ) {
      return uiDraw_Dialog ( D ) ;
  }
  int uiDraw_Dialog ( DIALOG *D ) {
      int n , i , controls = 0 , item , ch , oldi = -1;
      DIA *d;
      d = D->d;
      D->df = 0;
      if ( D->DrawBkgr == 1 ) _ui_Draw_Dialog_Area_Transparent ( D ) ;
      if ( D->Bkpixmap != NULL ) kgImage ( ( D ) , D->Bkpixmap , 0 , 0 , D->xl , D->yl , D->transparency , 1.0 ) ;
          
      i = 0;
      while ( d [ i ] .t != NULL ) {
          ch = ( d [ i ] .t->code ) ;
          switch ( ( int ) ch ) {
              case 'o': /* progress bar */
              _uiDrawO ( D , i ) ;
              break;
              case 'p': /* new for xpm display */
//         _uiDrawTransparentPicture(D,i);
              _uiDrawP ( D , i ) ;
              break;
              case 'i': /* info box */
//         _uiDrawInfo(D,i);
              _uiDrawI ( D , i ) ;
              break;
              case 'x': // new type browser
              _uiDrawX ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'v': // Vert scroll bar
              _uiDrawV ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'z': // Vert scroll bar
              _uiDrawZ ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'y': // new type browser
              _uiDrawY ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'r': // new type browser
              _uiDrawRadioButton ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'c': // new type browser
              _uiDrawCheckBox ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 't':
#if 1
              _uiDrawTextBox ( D , i ) ;
#endif
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
                  if ( D->InputWid < 0 ) D->InputWid = i;
              }
              break;
              case 'T':
              _uiDrawTableBox ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
                  if ( D->InputWid < 0 ) D->InputWid = i;
              }
              break;
              case 'h':
              _uiDrawN ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'H':
              _uiDrawHoriButtons ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'n':
              _uiDrawN ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'b':
              case 'N':
              _uiDrawB ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'f':
              _uiDrawSlideFloat ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'P':
              _uiDrawHbar ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'd':
              _uiDrawSlideInteger ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'w':
              _uiDrawBrowser ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'e':
              _uiDrawE ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 's':
              _uiDrawScrollMsgItem ( D , i ) ;
              if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) != 0 ) {
                  controls++;
                  D->df = i;
              }
              break;
              case 'g':
              _uiDrawGraphicsArea ( D , i ) ;
              break;
              case 'm':
              case 'M':
              case 'B':
              _uiDrawMessage ( D , i ) ;
              break;
              default:
              printf ( "Error: in dialog : wrong code |%c| \n" , ch ) ;
              exit ( 0 ) ;
          }
          i++;
      }
      D->controls = controls;
      D->TotWid = i;
      return controls;
  }
  int kgGetWidgetId ( DIALOG *D , void *Widget ) {
      int i , k , ret = -1;
      DIA *d;
      DIT *t;
      if ( D == NULL ) return ret;
      d = D->d;
      if ( d == NULL ) return ret;
      i = 0;
      while ( d [ i ] .t != NULL ) {
          if ( ( void * ) ( d [ i ] .t ) == Widget ) {ret = i;break;}
          i++;
      }
      return ret;
  }
  int kgDrawWidget ( DIALOG *D , void *Widget ) {
/* Draws ith widget if it exists int the Dialog */
      int n , controls = 0 , item , ch , oldi = -1 , ret = 0 , i , k;
      DIA *d;
      DIT *t;
      d = D->d;
      k = kgGetWidgetId ( D , Widget ) ;
      if ( k != -1 ) {
          i = k;
          t = ( DIT * ) Widget;
          ret = 1;
          ch = ( t->code ) ;
          switch ( ( int ) ch ) {
              case 'o': /* progress bar */
              if ( d [ i ] .o->item == -1 ) _uiDrawO ( D , i ) ;
              else _uiMake_O ( Widget ) ;
              break;
              case 'p': /* new for xpm display */
              if ( d [ i ] .p->item == -1 ) _uiDrawP ( D , i ) ;
              else _uiMake_P ( Widget ) ;
              break;
              case 'i': /* info box */
              if ( d [ i ] .i->item == -1 ) _uiDrawI ( D , i ) ;
              else _uiMake_I ( Widget ) ;
              break;
              case 'x': // new type browser
              if ( d [ i ] .x->item == -1 ) _uiDrawX ( D , i ) ;
              else _uiMake_X ( Widget ) ;
              break;
              case 'v': // Vert scroll bar
              if ( d [ i ] .v->item == -1 ) _uiDrawV ( D , i ) ;
              else _uiMake_V ( Widget ) ;
              break;
              case 'z': // Vert scroll bar
              if ( d [ i ] .z->item == -1 ) _uiDrawZ ( D , i ) ;
              else _uiMake_Z ( Widget ) ;
              break;
              case 'y': // new type browser
              if ( d [ i ] .y->item == -1 ) _uiDrawY ( D , i ) ;
              else _uiMake_Y ( Widget ) ;
              break;
              case 'r': // new type browser
              if ( d [ i ] .r->item == -1 ) _uiDrawRadioButton ( D , i ) ;
              else _uiMake_RadioButton ( Widget ) ;
              break;
              case 'c': // new type browser
              if ( d [ i ] .c->item == -1 ) _uiDrawCheckBox ( D , i ) ;
              else _uiMake_CheckBox ( Widget ) ;
              break;
              case 't':
              if ( d [ i ] .t->item == -1 ) _uiDrawTextBox ( D , i ) ;
              else _uiMake_Tx ( Widget ) ;
              break;
              case 'T':
              if ( d [ i ] .t->item == -1 ) _uiDrawTableBox ( D , i ) ;
              else _uiMake_Ta ( Widget ) ;
              break;
              case 'h':
              if ( d [ i ] .h->item == -1 ) _uiDrawN ( D , i ) ;
              else _uiMake_N ( Widget ) ;
              break;
              case 'H':
              if ( d [ i ] .H->item == -1 ) _uiDrawHoriButtons ( D , i ) ;
              else _uiMake_H ( Widget ) ;
              break;
              case 'n':
              if ( d [ i ] .N->item == -1 ) _uiDrawN ( D , i ) ;
              else _uiMake_N ( Widget ) ;
              break;
              case 'b':
              case 'N':
              if ( d [ i ] .b->item == -1 ) _uiDrawB ( D , i ) ;
              else _uiMake_B ( Widget ) ;
              break;
              case 'f':
              if ( d [ i ] .f->item == -1 ) _uiDrawSlideFloat ( D , i ) ;
              else _uiMake_SF ( Widget ) ;
              break;
              case 'P':
              if ( d [ i ] .B->item == -1 ) _uiDrawHbar ( D , i ) ;
              else _uiMake_SH ( Widget ) ;
              break;
              case 'd':
              if ( d [ i ] .d->item == -1 ) _uiDrawSlideInteger ( D , i ) ;
              else _uiMake_SD ( Widget ) ;
              break;
              case 'w':
              if ( d [ i ] .w->item == -1 ) _uiDrawBrowser ( D , i ) ;
              else _uiMake_W ( Widget ) ;
              break;
              case 'e':
              if ( d [ i ] .e->item == -1 ) _uiDrawE ( D , i ) ;
              else _uiMake_E ( Widget ) ;
              break;
              case 's':
              if ( d [ i ] .s->item == -1 ) _uiDrawScrollMsgItem ( D , i ) ;
              else _uiMake_MS ( Widget ) ;
              break;
              case 'g':
              if ( d [ i ] .g->item == -1 ) _uiDrawGraphicsArea ( D , i ) ;
              else _uiMake_G ( Widget ) ;
              break;
              case 'm':
              case 'M':
              case 'B':
              if ( d [ i ] .m->item == -1 ) _uiDrawMessage ( D , i ) ;
              else _uiMake_M ( Widget ) ;
              break;
              default:
              printf ( "Error: in dialog : wrong code |%c| \n" , ch ) ;
              ret = -1;
              break;
          }
      }
      return ret;
  }
  int kgFreeElementImages(void *wid) {
    DIT *T = (DIT *)wid;
    T_ELMT *e=T->elmt;
    int k,nx,ny,i,j;
    nx = T->nx;
    ny = T->ny;
    for(j=0;j<ny;j++) {
      for(i=0;i<nx;i++) {
         if( e[j*nx+i].img != NULL) kgFreeGmImage(e[j*nx+i].img);
         e[j*nx+i].img = NULL;
      }
    }
  }
  int kgRedrawDialog ( DIALOG *D ) {
      int n , i , controls = 0 , item , ch , oldi = -1;
      DIA *d;
      d = D->d;
      D->df = 0;
      if ( D->DrawBkgr == 1 ) _ui_Draw_Dialog_Area_Transparent ( D ) ;
      if ( D->Bkpixmap != NULL ) kgImage ( ( D ) , D->Bkpixmap , 0 , 0 , D->xl , D->yl , D->transparency , 1.0 ) ;
          
      i = 0;
      while ( d [ i ] .t != NULL ) {
          ch = ( d [ i ] .t->code ) ;
//     printf("ch:%c\n",ch);
          switch ( ( int ) ch ) {
              case 'o': /* progress bar */
              if ( d [ i ] .o->hide != 1 ) break;
              item = d [ i ] .o->item;
              if ( d [ i ] .o->item == -1 ) _uiDrawO ( D , i ) ;
              else _uiMake_O ( kgGetWidget ( D , i ) ) ;
              break;
              case 'p': /* new for xpm display */
              if ( d [ i ] .p->hide != 1 ) break;
              item = d [ i ] .p->item;
              if ( d [ i ] .p->item == -1 ) _uiDrawP ( D , i ) ;
              else _uiMake_P ( kgGetWidget ( D , i ) ) ;
              break;
              case 'i': /* info box */
              if ( d [ i ] .i->hide != 1 ) break;
              item = d [ i ] .i->item;
              if ( d [ i ] .i->item == -1 ) _uiDrawI ( D , i ) ;
              else _uiMake_I ( kgGetWidget ( D , i ) ) ;
              break;
              case 'x': // new type browser
              controls++;
              if ( d [ i ] .x->hide != 1 ) break;
              item = d [ i ] .x->item;
              if ( d [ i ] .x->item == -1 ) _uiDrawX ( D , i ) ;
              else _uiMake_X ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'v': // Vert scroll bar
              controls++;
              if ( d [ i ] .v->hide != 1 ) break;
              item = d [ i ] .v->item;
              if ( d [ i ] .v->item == -1 ) _uiDrawV ( D , i ) ;
              else{
                FreeImg ( ( ( DIV * ) ( d [ i ] .t ) )->Bimg ) ;
                _uiDrawV ( D , i ) ;
              }
//              else _uiMake_V ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'z': // Vert scroll bar
              controls++;
              if ( d [ i ] .z->hide != 1 ) break;
              item = d [ i ] .z->item;
              if ( d [ i ] .z->item == -1 ) _uiDrawZ ( D , i ) ;
              else _uiMake_Z ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'y': // new type browser
              controls++;
              if ( d [ i ] .y->hide != 1 ) break;
              item = d [ i ] .y->item;
              if ( d [ i ] .y->item == -1 ) _uiDrawY ( D , i ) ;
              else _uiMake_Y ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'r': // new type browser
              controls++;
              if ( d [ i ] .r->hide != 1 ) break;
              item = d [ i ] .r->item;
              if ( d [ i ] .r->item == -1 ) _uiDrawRadioButton ( D , i ) ;
              else _uiMake_RadioButton ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'c': // new type browser
              controls++;
              if ( d [ i ] .c->hide != 1 ) break;
              item = d [ i ] .c->item;
              if ( d [ i ] .c->item == -1 ) _uiDrawCheckBox ( D , i ) ;
              else _uiMake_CheckBox ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 't':
              controls++;
              if ( d [ i ] .t->hide != 1 ) break;
              item = d [ i ] .t->item;
              if ( d [ i ] .t->item == -1 ) _uiDrawTextBox ( D , i ) ;
              else _uiMake_Tx ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'T':
              controls++;
              if ( d [ i ] .t->hide != 1 ) break;
              item = d [ i ] .t->item;
              if ( d [ i ] .t->item == -1 ) _uiDrawTableBox ( D , i ) ;
              else {
		      DIT *Ta= (DIT *)(d[i].t);
                      T_ELMT *elmt;
                      FreeImg ( Ta->Bimg ) ;
                      kgFreeElementImages(Ta);
		      if(Ta->tstr != NULL) {
                              elmt =((TX_STR *)(Ta->tstr))->elmt; 
                              uiFreeImgStrs(((TX_STR *)(Ta->tstr))->F.Imgs);
			      free(Ta->tstr);
			      Ta->tstr=NULL;
		      }
//		      _uiMake_Ta ( kgGetWidget ( D , i ) ) ;
		      _uiDrawTableBox ( D , i ) ;
	      }
              D->df = i;
              break;
              case 'h':
              controls++;
              if ( d [ i ] .h->hide != 1 ) break;
              item = d [ i ] .h->item;
              if ( d [ i ] .h->item == -1 ) _uiDrawN ( D , i ) ;
              else _uiMake_N ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'H':
              controls++;
              D->df = i;
              if ( d [ i ] .H->hide != 1 ) break;
              item = d [ i ] .H->item;
              if ( d [ i ] .H->item == -1 ) _uiDrawHoriButtons ( D , i ) ;
              else _uiMake_H ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'n':
              controls++;
              if ( d [ i ] .N->hide != 1 ) break;
              item = d [ i ] .N->item;
              if ( d [ i ] .N->item == -1 ) _uiDrawN ( D , i ) ;
              else _uiMake_N ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'b':
              case 'N':
              controls++;
              if ( d [ i ] .b->hide != 1 ) break;
              item = d [ i ] .b->item;
              if ( d [ i ] .b->item == -1 ) _uiDrawB ( D , i ) ;
              else _uiMake_B ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'f':
              controls++;
              if ( d [ i ] .f->hide != 1 ) break;
              item = d [ i ] .f->item;
              if ( d [ i ] .f->item == -1 ) _uiDrawSlideFloat ( D , i ) ;
              else _uiMake_SF ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'P':
              controls++;
              if ( d [ i ] .B->hide != 1 ) break;
              item = d [ i ] .B->item;
              if ( d [ i ] .B->item == -1 ) _uiDrawHbar ( D , i ) ;
              else _uiMake_SH ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'd':
              controls++;
              if ( d [ i ] .d->hide != 1 ) break;
              item = d [ i ] .d->item;
              if ( d [ i ] .d->item == -1 ) _uiDrawSlideInteger ( D , i ) ;
              else _uiMake_SD ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'w':
              controls++;
              if ( d [ i ] .w->hide != 1 ) break;
              item = d [ i ] .w->item;
              if ( d [ i ] .w->item == -1 ) _uiDrawBrowser ( D , i ) ;
              else _uiMake_W ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'e':
              controls++;
              if ( d [ i ] .e->hide != 1 ) break;
              item = d [ i ] .e->item;
              if ( d [ i ] .e->item == -1 ) _uiDrawE ( D , i ) ;
              else _uiMake_E ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 's':
              controls++;
              if ( d [ i ] .s->hide != 1 ) break;
              item = d [ i ] .s->item;
              if ( d [ i ] .s->item == -1 ) _uiDrawScrollMsgItem ( D , i ) ;
              else _uiMake_MS ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'g':
              if ( d [ i ] .g->hide != 1 ) break;
              item = d [ i ] .g->item;
              if ( d [ i ] .g->item == -1 ) _uiDrawGraphicsArea ( D , i ) ;
              else _uiMake_G ( kgGetWidget ( D , i ) ) ;
              break;
              case 'm':
              case 'M':
              case 'B':
              if ( d [ i ] .m->hide != 1 ) break;
              item = d [ i ] .m->item;
              if ( d [ i ] .m->item == -1 ) _uiDrawMessage ( D , i ) ;
              else _uiMake_M ( kgGetWidget ( D , i ) ) ;
              break;
              default:
              printf ( "Error: in dialog : wrong code |%c| \n" , ch ) ;
              exit ( 0 ) ;
          }
          i++;
      }
#if 1
      controls = 0;
      i = 0;
      while ( d [ i ] .t != NULL ) {
          ch = ( d [ i ] .t->code ) ;
//     printf("ch:%c\n",ch);
          switch ( ( int ) ch ) {
              case 'o': /* progress bar */
              if ( d [ i ] .o->hide == 1 ) break;
              item = d [ i ] .o->item;
              if ( d [ i ] .o->item == -1 ) _uiDrawO ( D , i ) ;
              else _uiMake_O ( kgGetWidget ( D , i ) ) ;
              break;
              case 'p': /* new for xpm display */
              if ( d [ i ] .p->hide == 1 ) break;
              item = d [ i ] .p->item;
              if ( d [ i ] .p->item == -1 ) _uiDrawP ( D , i ) ;
              else _uiMake_P ( kgGetWidget ( D , i ) ) ;
              break;
              case 'i': /* info box */
              if ( d [ i ] .i->hide == 1 ) break;
              item = d [ i ] .i->item;
              if ( d [ i ] .i->item == -1 ) _uiDrawI ( D , i ) ;
              else _uiMake_I ( kgGetWidget ( D , i ) ) ;
              break;
              case 'x': // new type browser
              controls++;
              if ( d [ i ] .x->hide == 1 ) break;
              item = d [ i ] .x->item;
              if ( d [ i ] .x->item == -1 ) _uiDrawX ( D , i ) ;
              else _uiMake_X ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'v': // Vert scroll bar
              controls++;
              if ( d [ i ] .v->hide == 1 ) break;
              item = d [ i ] .v->item;
              if ( d [ i ] .v->item == -1 ) _uiDrawV ( D , i ) ;
              else _uiMake_V ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'z': // Vert scroll bar
              controls++;
              if ( d [ i ] .z->hide == 1 ) break;
              item = d [ i ] .z->item;
              if ( d [ i ] .z->item == -1 ) _uiDrawZ ( D , i ) ;
              else _uiMake_Z ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'y': // new type browser
              controls++;
              if ( d [ i ] .y->hide == 1 ) break;
              item = d [ i ] .y->item;
              if ( d [ i ] .y->item == -1 ) _uiDrawY ( D , i ) ;
              else _uiMake_Y ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'r': // new type browser
              controls++;
              if ( d [ i ] .r->hide == 1 ) break;
              item = d [ i ] .r->item;
              if ( d [ i ] .r->item == -1 ) _uiDrawRadioButton ( D , i ) ;
              else _uiMake_RadioButton ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'c': // new type browser
              controls++;
              if ( d [ i ] .c->hide == 1 ) break;
              item = d [ i ] .c->item;
              if ( d [ i ] .c->item == -1 ) _uiDrawCheckBox ( D , i ) ;
              else _uiMake_CheckBox ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 't':
              controls++;
              if ( d [ i ] .t->hide == 1 ) break;
              item = d [ i ] .t->item;
              if ( d [ i ] .t->item == -1 ) _uiDrawTextBox ( D , i ) ;
              else _uiMake_Tx ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'T':
              controls++;
              if ( d [ i ] .t->hide == 1 ) break;
              item = d [ i ] .t->item;
              if ( d [ i ] .t->item == -1 ) {
                     printf("item = -1 for _uiDrawTableBox\n");
                     _uiDrawTableBox ( D , i ) ;
              }
              else {
		      DIT *Ta= (DIT *)(d[i].t);
                      kgFreeElementImages(Ta);
                      FreeImg ( ( ( DIT * ) ( d [ i ] .t ) )->Bimg ) ;
		      if(Ta->tstr != NULL) {
                              uiFreeImgStrs(((TX_STR *)(Ta->tstr))->F.Imgs);
			      free(Ta->tstr);
			      Ta->tstr=NULL;
		      }
//		      _uiMake_Ta ( kgGetWidget ( D , i ) ) ;
		      _uiDrawTableBox ( D , i ) ;
	      }
              D->df = i;
              break;
              case 'h':
              controls++;
              if ( d [ i ] .h->hide == 1 ) break;
              item = d [ i ] .h->item ;
              if ( d [ i ] .h->item == -1 ) _uiDrawN ( D , i ) ;
              else _uiMake_N ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'H':
              controls++;
              if ( d [ i ] .H->hide == 1 ) break;
              item = d [ i ] .H->item;
              if ( d [ i ] .H->item == -1 ) _uiDrawHoriButtons ( D , i ) ;
              else _uiMake_H ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'n':
              controls++;
              if ( d [ i ] .N->hide == 1 ) break;
              item = d [ i ] .N->item;
              if ( d [ i ] .N->item == -1 ) _uiDrawN ( D , i ) ;
              else _uiMake_N ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'b':
              case 'N':
              controls++;
              if ( d [ i ] .b->hide == 1 ) break;
              item = d [ i ] .b->item;
              if ( d [ i ] .b->item == -1 ) _uiDrawB ( D , i ) ;
              else _uiMake_B ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'f':
              controls++;
              if ( d [ i ] .f->hide == 1 ) break;
              item = d [ i ] .f->item;
              if ( d [ i ] .f->item == -1 ) _uiDrawSlideFloat ( D , i ) ;
              else _uiMake_SF ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'P':
              controls++;
              if ( d [ i ] .B->hide == 1 ) break;
              item = d [ i ] .B->item;
              if ( d [ i ] .B->item == -1 ) _uiDrawHbar ( D , i ) ;
              else _uiMake_SH ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'd':
              controls++;
              if ( d [ i ] .d->hide == 1 ) break;
              item = d [ i ] .d->item;
              if ( d [ i ] .d->item == -1 ) _uiDrawSlideInteger ( D , i ) ;
              else _uiMake_SD ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'w':
              controls++;
              if ( d [ i ] .w->hide == 1 ) break;
              item = d [ i ] .w->item;
              if ( d [ i ] .w->item == -1 ) _uiDrawBrowser ( D , i ) ;
              else _uiMake_W ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'e':
              controls++;
              if ( d [ i ] .e->hide == 1 ) break;
              item = d [ i ] .e->item;
              if ( d [ i ] .e->item == -1 ) _uiDrawE ( D , i ) ;
              else _uiMake_E ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 's':
              controls++;
              if ( d [ i ] .s->hide == 1 ) break;
              item = d [ i ] .s->item;
              if ( d [ i ] .s->item == -1 ) _uiDrawScrollMsgItem ( D , i ) ;
              else _uiMake_MS ( kgGetWidget ( D , i ) ) ;
              D->df = i;
              break;
              case 'g':
              if ( d [ i ] .g->hide == 1 ) break;
              item = d [ i ] .g->item;
              if ( d [ i ] .g->item == -1 ) _uiDrawGraphicsArea ( D , i ) ;
              else _uiMake_G ( kgGetWidget ( D , i ) ) ;
              break;
              case 'm':
              case 'M':
              case 'B':
              if ( d [ i ] .m->hide == 1 ) break;
              item = d [ i ] .m->item;
              if ( d [ i ] .m->item == -1 ) _uiDrawMessage ( D , i ) ;
              else _uiMake_M ( kgGetWidget ( D , i ) ) ;
              break;
              default:
              printf ( "Error: in dialog : wrong code |%c| \n" , ch ) ;
              exit ( 0 ) ;
          }
          i++;
      }
#endif
      kgUpdateOn ( D ) ;
      D->controls = controls;
      D->TotWid = i;
      return controls;
  }
  void uiFreeMemAlloc ( DIALOG *D ) {
/* Clear Internal Memory */
      int i , ch;
      DIA *d;
      BUTS *buts;
      d = D->d;
      i = 0;
      while ( d [ i ] .t != NULL ) {
          ch = ( d [ i ] .t->code ) ;
//     printf("ch:%c\n",ch);
          switch ( ( int ) ch ) {
              case 'v':
              FreeImg ( ( ( DIV * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'z':
              FreeImg ( ( ( DIZ * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'o': /* progress bar */
              FreeImg ( ( ( DIO * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'p': /* new for xpm display */
              {
                  DIP *p;
                  p = d [ i ] .p;
                  kgFreeImage ( p->Bimg ) ;
                  p->Bimg = NULL;
              }
              break;
              case 'i': /* info box */
              {
                  DII *I;
                  I = d [ i ] .i;
                  Free ( I->twin ) ;
              }
              FreeImg ( ( ( DII * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 't':
              {
                  DIT *t;
		  TX_STR *Tx;
                  t = d [ i ] .t;
                  Tx = (TX_STR *)t->tstr;
#if 1
                  if(Tx != NULL) {
                      Tx->F.Imgs=uiFreeImgStrs(Tx->F.Imgs);
                      Free ( t->tstr ) ;
                  }
#endif
	       	  t->tstr=NULL;
              }
              FreeImg ( ( ( DIT * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'T':
              {
                  DIT *T;
		  TX_STR *Tx;
                  T_ELMT *elmt;
                  T = d [ i ] .t;
                  Tx = (TX_STR *)T->tstr;
                  kgFreeElementImages(T);
#if 1
                  if(Tx != NULL){
                       Tx->F.Imgs=uiFreeImgStrs(Tx->F.Imgs);
                       Free ( T->tstr ) ;
	               T->tstr=NULL;
                  }
#endif
                  FreeImg ( T->Bimg ) ;
              }
         // need addition once Table is implemented
              break;
              case 'h':
              {
                  DIL *b;
                  BUT_STR *butn;
                  int n , k;
                  b = d [ i ] .h;
                  butn = ( BUT_STR * ) b->buts;
                  if ( b->buts != NULL ) {
                      n = b->nx*b->ny;
                      if ( butn [ 0 ] .imgn != NULL ) {
                          for ( k = 0;k < n;k++ ) {
                              kgFreeImage ( butn [ k ] .imgn ) ;
                              kgFreeImage ( butn [ k ] .imgp ) ;
                              kgFreeImage ( butn [ k ] .imgh ) ;
                              kgFreeImage ( butn [ k ] .Bimg ) ;
                              kgFreeImage ( butn [ k ] .Bimg0 ) ;
                              butn [ k ] .imgn = NULL;
                              butn [ k ] .imgp = NULL;
                              butn [ k ] .imgh = NULL;
                              butn [ k ] .Bimg = NULL;
                              butn [ k ] .Bimg0 = NULL;
                          }
                      }
#if 0  // done in kgCleanUi
                      Free ( b->buts ) ;
                      b->buts = NULL;
#endif
                  }
              }
              FreeImg ( ( ( DIL * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'H':
              {
                  DILN *h;
                  h = d [ i ] .H;
                  Free ( h->buts ) ;
                  h->buts = NULL;
              }
              FreeImg ( ( ( DILN * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'n':
              {
                  DIN *b;
                  BUT_STR *butn;
                  int n , k;
                  b = d [ i ] .N;
                  butn = ( BUT_STR * ) b->buts;
                  if ( b->buts != NULL ) {
                      n = b->nx*b->ny;
                      if ( butn [ 0 ] .imgn != NULL ) {
                          for ( k = 0;k < n;k++ ) {
                              kgFreeImage ( butn [ k ] .imgn ) ;
                              kgFreeImage ( butn [ k ] .imgp ) ;
                              kgFreeImage ( butn [ k ] .imgh ) ;
                              kgFreeImage ( butn [ k ] .Bimg ) ;
                              kgFreeImage ( butn [ k ] .Bimg0 ) ;
                              butn [ k ] .Bimg = NULL;
                              butn [ k ] .Bimg0 = NULL;
                              butn [ k ] .imgn = NULL;
                              butn [ k ] .imgp = NULL;
                              butn [ k ] .imgh = NULL;
                          }
                      }
#if 0  //done kgCleanUi
                      Free ( b->buts ) ;
                      b->buts = NULL;
#endif
                  }
              }
              FreeImg ( ( ( DIN * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'N':
              {
                  DIBN *n;
                  n = d [ i ] .n;
                  buts = n->buts;
                  if ( buts != NULL ) {
                      Free ( buts->sw ) ;
                      Free ( buts ) ;
                      n->buts = NULL;
                  }
              }
              FreeImg ( ( ( DIBN * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'b':
              {
                  DIB *b;
                  b = d [ i ] .b;
                  if ( b->buts != NULL ) {
                      Free ( b->buts ) ;
                      b->buts = NULL;
                  }
              }
              FreeImg ( ( ( DIB * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'f':
              {
                  DIF *f;
                  f = d [ i ] .f;
                  Free ( f->sptr ) ;
                  f->sptr = NULL;
              }
              FreeImg ( ( ( DIF * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'P':
              {
                  DIHB *B;
                  S_STR *pt;
                  B = d [ i ] .B;
                  pt = B->sptr;
                  if ( pt != NULL ) {
                      kgFreeImage ( pt->bimg ) ;
                      kgFreeImage ( pt->fimg ) ;
                      Free ( B->sptr ) ;
                      B->sptr = NULL;
                  }
              }
              FreeImg ( ( ( DIHB * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'd':
              {
                  DID *di;
                  di = d [ i ] .d;
                  Free ( di->sptr ) ;
                  di->sptr = NULL;
              }
              FreeImg ( ( ( DID * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'x':
              {
                  DIX *w;
                  w = d [ i ] .x;
                  Free ( w->bwsr ) ;
                  uiCleanXImages ( w ) ;
              }
              FreeImg ( ( ( DIX * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'y':
              {
                  DIY *w;
                  w = d [ i ] .y;
                  Free ( w->bwsr ) ;
                  uiCleanYImages ( w ) ;
              }
              FreeImg ( ( ( DIY * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'c':
              {
                  DICH *w;
                  w = d [ i ] .c;
                  Free ( w->bwsr ) ;
                  uiCleanCImages ( w ) ;
              }
              FreeImg ( ( ( DICH * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'r':
              {
                  DIRA *w;
                  w = d [ i ] .r;
                  Free ( w->bwsr ) ;
                  uiCleanRImages ( w ) ;
              }
              FreeImg ( ( ( DIRA * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'w':
              {
                  DIW *w;
                  w = d [ i ] .w;
                  Free ( w->bwsr ) ;
                  uiCleanBrowserImages ( w ) ;
              }
              FreeImg ( ( ( DIW * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'e':
              {
                  DIE *w;
                  w = d [ i ] .e;
                  Free ( w->bwsr ) ;
                  uiCleanEbrowserImages ( w ) ;
              }
              FreeImg ( ( ( DIE * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 's':
              {
                  DIS *w;
                  w = d [ i ] .s;
                  Free ( w->bwsr ) ;
              }
              FreeImg ( ( ( DIS * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'g':
              {
                  DIG *g;
                  kgWC *wc;
                  g = d [ i ] .g;
                  kgCloseglWindow ( g ) ;
                  wc = g->wc;
                  kg_clear_scrn_buffer ( wc ) ;
//          Dempty(wc->SBlist);
                  Dempty ( wc->Clip ) ;
                  if ( g->img != NULL ) uiFreeImage ( g->img ) ;
                  if ( g->rzimg != NULL ) uiFreeImage ( g->rzimg ) ;
                  g->img = NULL;
                  g->rzimg = NULL;
                  if ( g->dc != NULL ) {
                      kgDC *dc;
                      dc = ( kgDC * ) ( g->dc ) ;
//            Dfree(dc->Fontlist);
                      Dempty ( dc->Fontlist ) ;
                      Free ( g->dc ) ;
                  }
                  if ( g->wc != NULL ) Free ( g->wc ) ;
              }
              FreeImg ( ( ( DIG * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'M':
              FreeImg ( ( ( DIM * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'm':
              case 'B':
              {
                  DIM *m;
                  m = d [ i ] .m;
              }
              FreeImg ( ( ( DIM * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              default:
              printf ( "Error: in dialog : wrong code |%c| \n" , ch ) ;
              exit ( 0 ) ;
          }
          i++;
      }
      Dempty ( ( Dlink * ) ( D->SearchList ) ) ;
      D->SearchList = NULL;
      return ;
  }
  void kgFreeBimg ( DIALOG *D ) {
/* Clear Internal Memory */
      int i , ch;
      DIA *d;
      BUTS *buts;
      d = D->d;
      i = 0;
      while ( d [ i ] .t != NULL ) {
          ch = ( d [ i ] .t->code ) ;
//     printf("ch:%c\n",ch);
          switch ( ( int ) ch ) {
              case 'v':
              FreeImg ( ( ( DIV * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'z':
              FreeImg ( ( ( DIZ * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'o': /* progress bar */
              FreeImg ( ( ( DIO * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'p': /* new for xpm display */
              FreeImg ( ( ( DIP * ) ( d [ i ] .p ) )->Bimg ) ;
              break;
              case 'i': /* info box */
              FreeImg ( ( ( DII * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 't':
              FreeImg ( ( ( DIT * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'T':
              FreeImg ( ( ( DIT * ) ( d [ i ] .t ) )->Bimg ) ;
         // need addition once Table is implemented
              break;
              case 'h':
              FreeImg ( ( ( DIL * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'H':
              FreeImg ( ( ( DILN * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'n':
              FreeImg ( ( ( DIN * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'N':
              FreeImg ( ( ( DIBN * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'b':
              FreeImg ( ( ( DIB * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'f':
              FreeImg ( ( ( DIF * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'P':
              FreeImg ( ( ( DIHB * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'd':
              FreeImg ( ( ( DID * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'x':
              FreeImg ( ( ( DIX * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'y':
              FreeImg ( ( ( DIY * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'c':
              FreeImg ( ( ( DICH * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'r':
              FreeImg ( ( ( DIRA * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'w':
              FreeImg ( ( ( DIW * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'e':
              FreeImg ( ( ( DIE * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 's':
              FreeImg ( ( ( DIS * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'g':
              FreeImg ( ( ( DIG * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'M':
              FreeImg ( ( ( DIM * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'm':
              case 'B':
              FreeImg ( ( ( DIM * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              default:
              printf ( "Error: in dialog : wrong code |%c| \n" , ch ) ;
              exit ( 0 ) ;
          }
          i++;
      }
      return ;
  }
  void uiFreeXpm ( void * xpm ) {
      char *pt;
      if ( xpm == NULL ) return;
      pt = ( char * ) xpm;
      if ( ( pt [ 0 ] == '#' ) && ( pt [ 1 ] == '#' ) ) {Free ( xpm ) ;}
//  else kgFreeImage(xpm);
// should not; it is user to clean his allocation
      return;
  }
  void kgFreeWidget ( void *Widget ) {
      int i , ch , n , j;
      BUTS *buts;
      DIT *t;
      t = ( DIT * ) Widget;
      if ( t != NULL ) {
          ch = ( t->code ) ;
          switch ( ( int ) ch ) {
              case 'v':
              FreeImg ( ( ( DIV * ) ( t ) )->Bimg ) ;
              break;
              case 'z':
              FreeImg ( ( ( DIZ * ) ( t ) )->Bimg ) ;
              break;
              case 'o': /* progress bar */
              FreeImg ( ( ( DIO * ) ( t ) )->Bimg ) ;
              break;
              case 'p': /* new for xpm display */
              {
                  DIP *p;
                  p = ( DIP * ) Widget;
                  uiFreeXpm ( p->xpm ) ;
                  p->xpm = NULL;
              }
              FreeImg ( ( ( DIP * ) ( t ) )->Bimg ) ;
              break;
              case 'i': /* info box */
              {
                  DII *I;
                  I = ( DII * ) Widget;
              }
              FreeImg ( ( ( DII * ) ( t ) )->Bimg ) ;
              break;
              case 't':
              {
                  T_ELMT *e;
                  e = t->elmt;
                  n = t->nx*t->ny;
                  if ( e != NULL ) {
                      for ( j = 0;j < n;j++ ) Free ( e [ j ] .fmt ) ;
                      Free ( e ) ;
                      t->elmt = NULL;
                  }
                  if ( t->pt != NULL ) kgFreeDouble ( t->pt ) ;
                  t->pt = NULL;
              }
              FreeImg ( ( ( DIT * ) ( t ) )->Bimg ) ;
              break;
              case 'T':
              {
                  T_ELMT *e;
                  DIT *T;
                  T = t;
                  e = T->elmt;
                  n = T->nx*T->ny;
                  if ( e != NULL ) {
                      for ( j = 0;j < n;j++ ) Free ( e [ j ] .fmt ) ;
                      Free ( e ) ;
                      T->elmt = NULL;
                  }
                  if ( T->pt != NULL ) kgFreeDouble ( T->pt ) ;
                  T->pt = NULL;
              }
         // need addition once Table is implemented
              FreeImg ( ( ( DIT * ) ( t ) )->Bimg ) ;
              break;
              case 'h':
              {
                  int n , k;
                  DIL *b;
                  b = ( DIL * ) Widget;
                  BUT_STR *butn;
                  butn = ( BUT_STR * ) b->buts;
                  if ( butn != NULL ) {
                      for ( k = 0;k < n;k++ ) {
                          if ( butn [ k ] .xpmn != butn [ k ] .xpmp ) uiFreeXpm ( butn [ k ] .xpmp ) ;
                          if ( butn [ k ] .xpmn != butn [ k ] .xpmh ) {
                              if ( butn [ k ] .xpmp != butn [ k ] .xpmh ) uiFreeXpm ( butn [ k ] .xpmh ) ;
                          }
                          uiFreeXpm ( butn [ k ] .xpmn ) ;
                          butn [ k ] .xpmn = NULL;
                          butn [ k ] .xpmp = NULL;
                          butn [ k ] .xpmh = NULL;
                      }
                      if ( butn [ 0 ] .imgn != NULL ) {
                          for ( k = 0;k < n;k++ ) {
                              kgFreeImage ( butn [ k ] .imgn ) ;
                              kgFreeImage ( butn [ k ] .imgp ) ;
                              kgFreeImage ( butn [ k ] .imgh ) ;
                              kgFreeImage ( butn [ k ] .Bimg ) ;
                              kgFreeImage ( butn [ k ] .Bimg0 ) ;
                              butn [ k ] .Bimg = NULL;
                              butn [ k ] .Bimg0 = NULL;
                              butn [ k ] .imgn = NULL;
                              butn [ k ] .imgp = NULL;
                              butn [ k ] .imgh = NULL;
                          }
                      }
                      Free ( butn ) ;
                      b->buts = NULL;
                  }
              }
              FreeImg ( ( ( DIL * ) ( t ) )->Bimg ) ;
              break;
              case 'H':
              {
                  DILN *h;
                  h = ( DILN * ) Widget;
                  if ( h->xpm != NULL ) {
                      n = h->nx*h->ny;
                      for ( j = 0;j < n;j++ ) uiFreeXpm ( h->xpm [ j ] ) ;
                      Free ( h->xpm ) ;
                  }
                  Free ( h->sw ) ;
                  Free ( h->bkgr ) ;
                  if ( h->butncode != NULL ) Free ( h->butncode ) ;
                  if ( h->titles != NULL ) {
                      n = h->nx*h->ny;
                      for ( j = 0;j < n;j++ ) if ( h->titles [ j ] != NULL ) Free \
                       ( h->titles [ j ] ) ;
                      Free ( h->titles ) ;
                  }
              }
              FreeImg ( ( ( DILN * ) ( t ) )->Bimg ) ;
              break;
              case 'N':
              {
                  DIBN *n;
              }
              FreeImg ( ( ( DIBN * ) ( t ) )->Bimg ) ;
              break;
              case 'n':
              {
                  int n , k;
                  DIN *b;
                  b = ( DIN * ) Widget;
                  BUT_STR *butn;
                  butn = ( BUT_STR * ) b->buts;
                  if ( butn != NULL ) {
                      for ( k = 0;k < n;k++ ) {
                          if ( butn [ k ] .xpmn != butn [ k ] .xpmp ) uiFreeXpm ( butn [ k ] .xpmp ) ;
//            if(butn[k].xpmn!= butn[k].xpmh) uiFreeXpm(butn[k].xpmh);
                          if ( butn [ k ] .xpmn != butn [ k ] .xpmh ) {
                              if ( butn [ k ] .xpmp != butn [ k ] .xpmh ) uiFreeXpm ( butn [ k ] .xpmh ) ;
                          }
                          uiFreeXpm ( butn [ k ] .xpmn ) ;
                          butn [ k ] .xpmn = NULL;
                          butn [ k ] .xpmp = NULL;
                          butn [ k ] .xpmh = NULL;
                      }
                      if ( butn [ 0 ] .imgn != NULL ) {
                          for ( k = 0;k < n;k++ ) {
                              kgFreeImage ( butn [ k ] .imgn ) ;
                              kgFreeImage ( butn [ k ] .imgp ) ;
                              kgFreeImage ( butn [ k ] .imgh ) ;
                              kgFreeImage ( butn [ k ] .Bimg ) ;
                              kgFreeImage ( butn [ k ] .Bimg0 ) ;
                              butn [ k ] .Bimg = NULL;
                              butn [ k ] .Bimg0 = NULL;
                              butn [ k ] .imgn = NULL;
                              butn [ k ] .imgp = NULL;
                              butn [ k ] .imgh = NULL;
                          }
                      }
                      Free ( b->buts ) ;
                      b->buts = NULL;
                  }
              }
              FreeImg ( ( ( DIN * ) ( t ) )->Bimg ) ;
              break;
              case 'b':
              {
                  DIB *b;
                  b = ( DIB * ) Widget;
                  if ( b->xpm != NULL ) {
                      n = b->nx*b->ny;
                      for ( j = 0;j < n;j++ ) uiFreeXpm ( b->xpm [ j ] ) ;
                      Free ( b->xpm ) ;
                  }
                  Free ( b->sw ) ;
                  Free ( b->bkgr ) ;
                  if ( b->butncode != NULL ) Free ( b->butncode ) ;
                  if ( b->titles != NULL ) {
                      n = b->nx*b->ny;
                      for ( j = 0;j < n;j++ ) if ( b->titles [ j ] != NULL ) Free \
                       ( b->titles [ j ] ) ;
                      Free ( b->titles ) ;
                  }
              }
              FreeImg ( ( ( DIB * ) ( t ) )->Bimg ) ;
              break;
              case 'f':
              {
                  DIF *w;
                  S_STR *pt;
                  w = ( DIF * ) Widget;
                  pt = w->sptr;
                  Free ( w->sptr ) ;
              }
              FreeImg ( ( ( DIF * ) ( t ) )->Bimg ) ;
              break;
              case 'P':
              {
                  DIHB *w;
                  S_STR *pt;
                  w = ( DIHB * ) Widget;
                  pt = w->sptr;
                  if ( pt != NULL ) {
                      kgFreeImage ( pt->bimg ) ;
                      kgFreeImage ( pt->fimg ) ;
                      Free ( pt ) ;
                  }
                  w->sptr = NULL;
              }
              FreeImg ( ( ( DIHB * ) ( t ) )->Bimg ) ;
              break;
              case 'd':
              {
                  DID *w;
                  S_STR *pt;
                  w = ( DID * ) Widget;
                  Free ( w->sptr ) ;
              }
              FreeImg ( ( ( DID * ) ( t ) )->Bimg ) ;
              break;
              case 'x':
              {
                  DIX *w;
                  w = ( DIX * ) Widget;
                  kgFreeDouble ( ( void ** ) ( w->pt ) ) ;
                  w->pt = NULL;
                  uiCleanXImages ( w ) ;
              }
              FreeImg ( ( ( DIX * ) ( t ) )->Bimg ) ;
              break;
              case 'y':
              {
                  DIY *w;
                  w = ( DIY * ) Widget;
                  kgFreeDouble ( ( void ** ) ( w->pt ) ) ;
                  w->pt = NULL;
                  uiCleanYImages ( w ) ;
              }
              FreeImg ( ( ( DIY * ) ( t ) )->Bimg ) ;
              break;
              case 'c':
              {
                  DICH *w;
                  w = ( DICH * ) Widget;
                  kgFreeDouble ( ( void ** ) ( w->list ) ) ;
                  w->list = NULL;
                  uiCleanCImages ( w ) ;
              }
              FreeImg ( ( ( DICH * ) ( t ) )->Bimg ) ;
              break;
              case 'r':
              {
                  DIRA *w;
                  w = ( DIRA * ) Widget;
                  kgFreeDouble ( ( void ** ) ( w->list ) ) ;
                  w->list = NULL;
                  uiCleanRImages ( w ) ;
              }
              FreeImg ( ( ( DIRA * ) ( t ) )->Bimg ) ;
              break;
              case 'w':
              {
                  DIW *w;
                  w = ( DIW * ) Widget;
                  if ( w->prompt != NULL ) Free ( w->prompt ) ;
                  kgFreeDouble ( ( void ** ) w->menu ) ;
                  w->menu = NULL;
                  uiCleanBrowserImages ( w ) ;
              }
              FreeImg ( ( ( DIW * ) ( t ) )->Bimg ) ;
              break;
              case 'e':
              {
                  DIE *w;
                  w = ( DIE * ) Widget;
                  kgFreeDouble ( ( void ** ) w->menu ) ;
                  w->menu = NULL;
                  uiCleanEbrowserImages ( w ) ;
              }
              FreeImg ( ( ( DIE * ) ( t ) )->Bimg ) ;
              break;
              case 's':
              {
                  DIS *w;
                  w = ( DIS * ) Widget;
                  Free ( w->bwsr ) ;
              }
              FreeImg ( ( ( DIS * ) ( t ) )->Bimg ) ;
              break;
              case 'g':
              {
                  DIG *g;
                  kgWC *wc;
                  g = ( DIG * ) Widget;
                  uiFreeXpm ( g->xpm ) ;
                  g->xpm = NULL;
              }
              FreeImg ( ( ( DIG * ) ( t ) )->Bimg ) ;
              break;
              case 'M':
              FreeImg ( ( ( DIM * ) ( t ) )->Bimg ) ;
              break;
              case 'm':
              case 'B':
              {
                  DIM *m;
              }
              FreeImg ( ( ( DIM * ) ( t ) )->Bimg ) ;
              break;
              default:
              printf ( "Error: in dialog : wrong code |%c| \n" , ch ) ;
              exit ( 0 ) ;
          }
      }
      Free ( Widget ) ;
      return ;
  }
  void uiFreeWidgetMem ( DIALOG *D ) {
      int i , ch , n , j;
      DIA *d;
      BUTS *buts;
      d = D->d;
      i = 0;
      while ( d [ i ] .t != NULL ) {
          ch = ( d [ i ] .t->code ) ;
//     printf("ch:%c\n",ch);
          switch ( ( int ) ch ) {
              case 'v':
              FreeImg ( ( ( DIV * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'z':
              FreeImg ( ( ( DIZ * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'o': /* progress bar */
              FreeImg ( ( ( DIO * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'p': /* new for xpm display */
              {
                  DIP *p;
                  p = d [ i ] .p;
                  uiFreeXpm ( p->xpm ) ;
                  p->xpm = NULL;
              }
              FreeImg ( ( ( DIP * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'i': /* info box */
              {
                  DII *I;
                  I = d [ i ] .i;
              }
              FreeImg ( ( ( DII * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 't':
              {
                  T_ELMT *e;
                  DIT *t;
                  t = d [ i ] .t;
                  e = t->elmt;
                  if ( e != NULL ) {
                      n = t->nx*t->ny;
                      for ( j = 0;j < n;j++ ) Free ( e [ j ] .fmt ) ;
                      Free ( e ) ;
                      t->elmt = NULL;
                  }
                  if ( t->pt != NULL ) kgFreeDouble ( t->pt ) ;
                  t->pt = NULL;
              }
              FreeImg ( ( ( DIT * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'T':
              {
                  T_ELMT *e;
                  DIT *T;
                  T = d [ i ] .t;
                  e = T->elmt;
                  if ( e != NULL ) {
                      n = T->nx*T->ny;
                      for ( j = 0;j < n;j++ ) Free ( e [ j ] .fmt ) ;
                      Free ( e ) ;
                      T->elmt = NULL;
                  }
                  if ( T->pt != NULL ) kgFreeDouble ( T->pt ) ;
                  T->pt = NULL;
              }
         // need addition once Table is implemented
              FreeImg ( ( ( DIT * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'h':
              {
                  int n , k;
                  DIL *b;
                  BUT_STR *butn;
                  b = d [ i ] .h;
                  butn = ( BUT_STR * ) b->buts;
                  if ( butn != NULL ) {
                      for ( k = 0;k < n;k++ ) {
                          if ( butn [ k ] .xpmn != butn [ k ] .xpmp ) uiFreeXpm ( butn [ k ] .xpmp ) ;
                          if ( butn [ k ] .xpmn != butn [ k ] .xpmh ) {
                              if ( butn [ k ] .xpmp != butn [ k ] .xpmh ) uiFreeXpm ( butn [ k ] .xpmh ) ;
                          }
                          uiFreeXpm ( butn [ k ] .xpmn ) ;
                          butn [ k ] .xpmn = NULL;
                          butn [ k ] .xpmp = NULL;
                          butn [ k ] .xpmh = NULL;
                      }
                      if ( butn [ 0 ] .imgn != NULL ) {
                          for ( k = 0;k < n;k++ ) {
                              kgFreeImage ( butn [ k ] .imgn ) ;
                              kgFreeImage ( butn [ k ] .imgp ) ;
                              kgFreeImage ( butn [ k ] .imgh ) ;
                              kgFreeImage ( butn [ k ] .Bimg ) ;
                              kgFreeImage ( butn [ k ] .Bimg0 ) ;
                              butn [ k ] .Bimg = NULL;
                              butn [ k ] .Bimg0 = NULL;
                              butn [ k ] .imgn = NULL;
                              butn [ k ] .imgp = NULL;
                              butn [ k ] .imgh = NULL;
                          }
                      }
                      Free ( butn ) ;
                  }
                  b->buts = NULL;
              }
              FreeImg ( ( ( DIL * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'H':
              {
                  DILN *h;
                  h = d [ i ] .H;
                  if ( h->xpm != NULL ) {
                      n = h->nx*h->ny;
                      if ( h->xpm != NULL ) for ( j = 0;j < n;j++ ) uiFreeXpm ( h->xpm [ j ] ) ;
                      Free ( h->xpm ) ;
                      h->xpm = NULL;
                  }
                  Free ( h->sw ) ;
                  Free ( h->bkgr ) ;
                  if ( h->butncode != NULL ) Free ( h->butncode ) ;
                  if ( h->titles != NULL ) {
                      n = h->nx*h->ny;
                      for ( j = 0;j < n;j++ ) if ( h->titles [ j ] != NULL ) Free \
                       ( h->titles [ j ] ) ;
                      Free ( h->titles ) ;
                      h->titles = NULL;
                  }
              }
              FreeImg ( ( ( DILN * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'n':
              {
                  int n , k;
                  DIN *b;
                  BUT_STR *butn;
                  b = d [ i ] .N;
                  butn = ( BUT_STR * ) b->buts;
                  if ( butn != NULL ) {
                      for ( k = 0;k < n;k++ ) {
                          if ( butn [ k ] .xpmn != butn [ k ] .xpmp ) uiFreeXpm ( butn [ k ] .xpmp ) ;
                          if ( butn [ k ] .xpmn != butn [ k ] .xpmh ) {
                              if ( butn [ k ] .xpmp != butn [ k ] .xpmh ) uiFreeXpm ( butn [ k ] .xpmh ) ;
                          }
                          uiFreeXpm ( butn [ k ] .xpmn ) ;
                          butn [ k ] .xpmn = NULL;
                          butn [ k ] .xpmp = NULL;
                          butn [ k ] .xpmh = NULL;
                      }
                      if ( butn [ 0 ] .imgn != NULL ) {
                          for ( k = 0;k < n;k++ ) {
                              kgFreeImage ( butn [ k ] .imgn ) ;
                              kgFreeImage ( butn [ k ] .imgp ) ;
                              kgFreeImage ( butn [ k ] .imgh ) ;
                              kgFreeImage ( butn [ k ] .Bimg ) ;
                              kgFreeImage ( butn [ k ] .Bimg0 ) ;
                              butn [ k ] .Bimg = NULL;
                              butn [ k ] .Bimg0 = NULL;
                              butn [ k ] .imgn = NULL;
                              butn [ k ] .imgp = NULL;
                              butn [ k ] .imgh = NULL;
                          }
                      }
                      Free ( butn ) ;
                  }
                  b->buts = NULL;
              }
              FreeImg ( ( ( DIN * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'N':
              {
                  DIBN *n;
                  n = d [ i ] .n;
              }
              FreeImg ( ( ( DIBN * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'b':
              {
                  DIB *b;
                  b = d [ i ] .b;
                  if ( b->xpm != NULL ) {
                      n = b->nx*b->ny;
                      for ( j = 0;j < n;j++ ) uiFreeXpm ( b->xpm [ j ] ) ;
                      Free ( b->xpm ) ;
                      b->xpm = NULL;
                  }
                  Free ( b->sw ) ;
                  b->sw = NULL;
                  Free ( b->bkgr ) ;
                  b->bkgr = NULL;
                  if ( b->butncode != NULL ) Free ( b->butncode ) ;
                  b->butncode = NULL;
                  if ( b->titles != NULL ) {
                      n = b->nx*b->ny;
                      for ( j = 0;j < n;j++ ) if ( b->titles [ j ] != NULL ) Free \
                       ( b->titles [ j ] ) ;
                      Free ( b->titles ) ;
                      b->titles = NULL;
                  }
              }
              FreeImg ( ( ( DIB * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'f':
              {
                  DIF *f;
                  f = d [ i ] .f;
                  Free ( f->sptr ) ;
                  f->sptr = NULL;
              }
              FreeImg ( ( ( DIF * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'P':
              {
                  DIHB *B;
                  S_STR *pt;
                  B = d [ i ] .B;
                  pt = B->sptr;
                  if ( pt != NULL ) {
                      kgFreeImage ( pt->bimg ) ;
                      kgFreeImage ( pt->fimg ) ;
                      Free ( pt ) ;
                  }
                  B->sptr = NULL;
              }
              FreeImg ( ( ( DIHB * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'd':
              {
                  DID *di;
                  di = d [ i ] .d;
                  Free ( di->sptr ) ;
                  di->sptr = NULL;
              }
              FreeImg ( ( ( DID * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'x':
              {
                  DIX *w;
                  w = d [ i ] .x;
                  if ( ( D->VerId == 1401010200 ) || ( D->VerId == 2107030000 ) ) {
                      kgFreeDouble ( ( void ** ) ( w->pt ) ) ;
                      w->pt = NULL;
                  }
                  uiCleanXImages ( w ) ;
              }
              FreeImg ( ( ( DIX * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'y':
              {
                  DIY *w;
                  w = d [ i ] .y;
                  if ( ( D->VerId == 1401010200 ) || ( D->VerId == 2107030000 ) ) {
                      kgFreeDouble ( ( void ** ) ( w->pt ) ) ;
                      w->pt = NULL;
                  }
                  uiCleanYImages ( w ) ;
              }
              FreeImg ( ( ( DIY * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'c':
              {
                  DICH *w;
                  w = d [ i ] .c;
//         kgFreeDouble((void **)(w->list));
                  w->list = NULL;
                  uiCleanCImages ( w ) ;
              }
              FreeImg ( ( ( DICH * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'r':
              {
                  DIRA *w;
                  w = d [ i ] .r;
//         kgFreeDouble((void **)(w->list));
                  w->list = NULL;
                  uiCleanRImages ( w ) ;
              }
              FreeImg ( ( ( DIRA * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'w':
              {
                  DIW *w;
                  w = d [ i ] .w;
                  if ( w->prompt != NULL ) Free ( w->prompt ) ;
                  kgFreeDouble ( ( void ** ) w->menu ) ;
                  w->menu = NULL;
                  uiCleanBrowserImages ( w ) ;
              }
              FreeImg ( ( ( DIW * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'e':
              {
                  DIE *w;
                  w = d [ i ] .e;
                  kgFreeDouble ( ( void ** ) w->menu ) ;
                  w->menu = NULL;
                  uiCleanEbrowserImages ( w ) ;
              }
              FreeImg ( ( ( DIE * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 's':
              {
                  DIS *w;
                  w = d [ i ] .s;
                  Free ( w->bwsr ) ;
              }
              FreeImg ( ( ( DIS * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'g':
              {
                  DIG *g;
                  kgWC *wc;
                  g = d [ i ] .g;
                  uiFreeXpm ( g->xpm ) ;
                  g->xpm = NULL;
              }
              FreeImg ( ( ( DIG * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'M':
              FreeImg ( ( ( DIM * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              case 'm':
              case 'B':
              {
                  DIM *m;
                  m = d [ i ] .m;
              }
              FreeImg ( ( ( DIM * ) ( d [ i ] .t ) )->Bimg ) ;
              break;
              default:
              printf ( "Error: in dialog : wrong code |%c| \n" , ch ) ;
              exit ( 0 ) ;
          }
          i++;
      }
      return ;
  }
  int uiGetNewButtonPress ( void *tmp , KBEVENT kbevent ) {
      int xpo , ypo , key , evgax , evgay;
      int xgap = 0 , ygap = 0 , i , j , k , xx , yy , width , height , xd , yd , df , wd , ln;
          
      DIB *B;
      DIALOG *D;
      kgWC *wc;
      int *sw , code = 1;
      B = ( DIB * ) tmp;
      D = B->D;
      wc = WC ( D ) ;
      df = 0;
      key = kbevent.button;
      xpo = kbevent.x , ypo = kbevent.y;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      wd = B->width;
      ln = B->lngth;
      xgap = B->xgap;
      ygap = B->ygap;
      k = 0;
      sw = B->sw;
      if ( B->code == 'N' ) code = 0;
      else code = 1;
      for ( j = 0;j < ( B->ny ) ;j++ ) {
          yy = B->y1+D->yo+ygap/2+j* ( ygap+wd ) ;
          for ( i = 0; i < ( B->nx ) ;i++ ) {
              if ( code && ( sw [ k ] != 1 ) ) continue;
              xx = B->x1+D->xo+xgap/2+i* ( ln+xgap ) ;
              xd = xpo-xx;yd = ypo-yy;
              if ( ( ( xd* ( xd-ln ) ) <= 0 ) && ( ( yd* ( yd-wd ) ) <= 0 ) ) {
                  df = k+1;
                  _dv_draw_newbutton ( B , k , -1 ) ;
                  * ( B->df ) = df;
                  uiUpdateOn ( D ) ;
                  break;
              }
              k++;
          }
          if ( df != 0 ) break;
      }
      uiRest_clip_limits ( wc ) ;
      if ( df == 0 ) { return -1; }
      else return df;
  }
  int uiGetButtonPress ( void *tmp , KBEVENT kbevent ) {
      int xpo , ypo , key , evgax , evgay;
      int xgap = 0 , ygap = 0 , i , j , k , xx , yy , width , height , xd , yd , df , wd , ln , nb;
          
      BUT_STR *butn;
      DIN *B;
      DIALOG *D;
      kgWC *wc;
      int *sw , code = 1;
      B = ( DIN * ) tmp;
      D = B->D;
      wc = WC ( D ) ;
      df = 0;
      key = kbevent.button;
      xpo = kbevent.x , ypo = kbevent.y;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      wd = B->width;
      ln = B->lngth;
      xgap = B->xgap;
      ygap = B->ygap;
      butn = B->buts;
      k = 0;
      nb = -1;
      for ( j = 0;j < ( B->ny ) ;j++ ) {
//     yy=B->y1+D->yo+ygap/2+j*(ygap+wd);
          for ( i = 0; i < ( B->nx ) ;i++ ) {
              nb++;
              if ( ( butn [ nb ] .sw != 1 ) ) continue;
              if ( ( butn [ nb ] .sw == 1 ) ) {
//         xx=B->x1+D->xo+xgap/2+i*(ln+xgap);
                  xx = butn [ nb ] .x1;
                  yy = butn [ nb ] .y1;
                  xd = xpo-xx;yd = ypo-yy;
                  if ( ( ( xd* ( xd-ln ) ) <= 0 ) && ( ( yd* ( yd-wd ) ) <= 0 ) ) {
                      df = nb+1;
                      _dv_draw_button ( B , nb , -1 ) ;
                       ( B->bval ) = df;
                      uiUpdateOn ( D ) ;
//TCB 2021
                      usleep ( 50000 ) ;
                      break;
                  }
              }
              k++;
          }
          if ( df != 0 ) break;
      }
      uiRest_clip_limits ( wc ) ;
      if ( df == 0 ) { return -1; }
      else return df;
  }
  int MousePressInTextBox ( TX_STR *tptr , KBEVENT kbevent ) {
      int kk , df;
      df = -1;
      _ui_processtextboxpress ( tptr , kbevent ) ;
      return df;
  }
  int MousePressInTableBox ( TX_STR *tptr , KBEVENT kbevent ) {
      int kk , df;
      df = -1;
      _ui_processtableboxpress ( tptr , kbevent ) ;
      return df;
  }
  int MousePressInNewButtonBox ( void *tmp , KBEVENT kbevent ) {
      int kk , df;
      DIB *B;
      B = ( DIB * ) tmp;
      df = -1;
      if ( ( kk = uiGetNewButtonPress ( B , kbevent ) ) > 0 ) {
          df = * ( B->df ) ;
      }
      return df;
  }
  int MousePressInButtonBox ( void *tmp , KBEVENT kbevent ) {
      int kk , df;
      DIN *B;
      B = ( DIN * ) tmp;
      df = -1;
      if ( ( kk = uiGetButtonPress ( B , kbevent ) ) > 0 ) {
          df = ( B->bval ) ;
      }
      return df;
  }
  int MousePressInHBar ( void *tmp , KBEVENT kbevent ) {
      int kk , df;
      DIB *B;
      B = ( DIB * ) tmp;
      df = -1;
      if ( ( kk = uiGetNewButtonPress ( B , kbevent ) ) > 0 ) {
          df = * ( B->df ) ;
      }
      return df;
  }
  int uiGetBrowserButtonPress ( DIW *w , KBEVENT kbevent ) {
      BRW_STR *br;
      Gclr gc;
      int x1 , y1 , x2 , y2 , PON_X , PON_Y , df , yoff;
      char **m;
      int evgax , evgay;
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) ( w->D ) ;
      wc = WC ( D ) ;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      PON_X = kbevent.x;
      PON_Y = kbevent.y;
      x1 = w->x2-w->width-w->offset+D->xo;
      yoff = ( w->y2+w->y1 ) *0.5;
      y1 = D->yo+yoff-w->width*.5;
      x2 = x1+w->width;
      y2 = y1+w->width;
      if ( ( ( x1 > ( PON_X ) ) || ( x2 < ( PON_X ) ) || ( y1 > \
       ( PON_Y ) ) || ( y2 < ( PON_Y ) ) ) ) {return -1;}
      y1 = D->evgay-y1;y2 = D->evgay-y2;
      df = _uiMenu ( w ) ;
//    kg_scr_recover(wc);
      gc = D->gc;
      br = w->bwsr;
      m = br->menu;
//TCB
//    ui_draw_browser(br);
      * ( w->df ) = br->df;
//    ui_draw_browser((DIW *)w);
      _uiMake_W ( ( DIW * ) w ) ;
      uiUpdateOn ( D ) ;
      uiRest_clip_limits ( wc ) ;
      return ( br->df ) ;
  }
  int MousePressInBrowser ( DIW *w , KBEVENT kbevent ) {
      int kk , df;
      df = -1;
//   printf("MousePressInBrowser\n");
      if ( ( kk = uiGetBrowserButtonPress ( w , kbevent ) ) > 0 ) {
          df = * ( w->df ) ;
      }
      return df;
  }
  int uiGetFslideButtonPress ( S_STR *sptr , KBEVENT kbevent ) {
      return _ui_processslidepress ( sptr , kbevent ) ;
  }
  int MousePressInFslide ( S_STR *sptr , KBEVENT kbevent ) {
      int kk , df;
      df = -1;
//   printf("MousePressInFslide\n");
      if ( ( kk = uiGetFslideButtonPress ( sptr , kbevent ) ) > 0 ) {
          df = sptr->df;
      }
      return df;
  }
  int uiGetDslideButtonPress ( S_STR *sptr , KBEVENT kbevent ) {
      return _ui_processslidepress ( sptr , kbevent ) ;
  }
//int uiGetHslideButtonPress(S_STR *sptr,KBEVENT kbevent) {
    //return _ui_processhslidepress(sptr,kbevent);
//}
  int MousePressInDslide ( S_STR *sptr , KBEVENT kbevent ) {
      int kk , df;
      df = -1;
//   printf("MousePressInDslide \n");
      if ( ( kk = uiGetDslideButtonPress ( sptr , kbevent ) ) > 0 ) {
          df = sptr->df;
      }
      return df;
  }
  int MousePressInHslideBar ( DIHB *B , KBEVENT kbevent ) {
      int kk , df;
      S_STR *sptr;
      sptr = B->sptr;
      df = -1;
//   printf("MousePressInDslide \n");
      if ( ( kk = _ui_processhslidepress ( B , kbevent ) ) > 0 ) {
          df = sptr->df;
      }
      return df;
  }
  int EventInMenu ( BRW_STR *br , KBEVENT kbevent ) {
      int ok = -1 , NOK = 1 , item;
      int ans , df = -1;
      DIALOG *D;
      D = br->D;
      if ( br->menu == NULL ) return ok;
      if ( br->menu [ 0 ] == NULL ) return ok;
      item = br->df;
      ans = kbevent.key;
 // set_pointer_position(kbevent.x,kbevent.y,kbevent.button);
      D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
      switch ( kbevent.event ) {
          case 0:
//TCB  felt it is unpleasent the item highlights on mouse movement
//     it can be switched on if needed
//          _ui_scroll_item_hilight(br,kbevent);
          break;
          case 1:
          ok = _ui_process_scroll_click ( br , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          break;
          case 2:
          break;
          case 3:
          _ui_process_scroll_move ( br , kbevent ) ;
          break;
          case 4:
          break;
          case 5:
          ok = _ui_process_scroll_key ( br , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          break;
      }
//TCB
//  if(item!=br->df) ok = br->df;
      return ok;
  }
  int EventInNewMenu ( DIX *x , KBEVENT kbevent ) {
      BRW_STR *br;
      int ok = -1 , NOK = 1 , item;
      int ans , df = -1;
      DIALOG *D;
      br = x->bwsr;
      D = br->D;
      if ( br->menu == NULL ) return ok;
      if ( br->menu [ 0 ] == NULL ) return ok;
      item = br->df;
      ans = kbevent.key;
 // set_pointer_position(kbevent.x,kbevent.y,kbevent.button);
      D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
      switch ( kbevent.event ) {
          case 0:
//TCB  felt it is unpleasent the item highlights on mouse movement
//     it can be switched on if needed
//          _ui_scroll_item_hilight(br,kbevent);
          break;
          case 1:
          ok = _ui_process_x_click ( x , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          break;
          case 2:
          break;
          case 3:
          ok = _ui_process_x_move ( x , kbevent ) ;
          break;
          case 4:
          break;
          case 5:
          ok = _ui_process_x_key ( x , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          break;
      }
//TCB
//  if(item!=br->df) ok = br->df;
      return ok;
  }
  int EventInE ( DIE *x , KBEVENT kbevent ) {
      BRW_STR *br;
      int ok = -1 , NOK = 1 , item;
      int ans , df = -1;
      DIALOG *D;
      br = x->bwsr;
      D = br->D;
      if ( br->menu == NULL ) return ok;
      if ( br->menu [ 0 ] == NULL ) return ok;
      item = br->df;
      ans = kbevent.key;
 // set_pointer_position(kbevent.x,kbevent.y,kbevent.button);
      D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
      switch ( kbevent.event ) {
          case 0:
//TCB  felt it is unpleasent the item highlights on mouse movement
//     it can be switched on if needed
//          _ui_scroll_item_hilight(br,kbevent);
          break;
          case 1:
          ok = _ui_process_e_click ( x , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          break;
          case 2:
          break;
          case 3:
          ok = _ui_process_e_move ( x , kbevent ) ;
          break;
          case 4:
          break;
          case 5:
          ok = _ui_process_e_key ( x , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          break;
      }
//TCB
//  if(item!=br->df) ok = br->df;
      return ok;
  }
  int EventInMS ( DIS *x , KBEVENT kbevent ) {
      BRW_STR *br;
      int ok = -1 , NOK = 1 , item;
      int ans , df = -1;
      DIALOG *D;
      br = x->bwsr;
      D = br->D;
      if ( br->menu == NULL ) return ok;
      if ( br->menu [ 0 ] == NULL ) return ok;
      item = br->df;
      ans = kbevent.key;
 // set_pointer_position(kbevent.x,kbevent.y,kbevent.button);
      D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
      switch ( kbevent.event ) {
          case 0:
//TCB  felt it is unpleasent the item highlights on mouse movement
//     it can be switched on if needed
//          _ui_scroll_item_hilight(br,kbevent);
          break;
          case 1:
          ok = _ui_process_m_click ( x , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          break;
          case 2:
          break;
          case 3:
          _ui_process_m_move ( x , kbevent ) ;
          break;
          case 4:
          break;
          case 5:
          ok = _ui_process_m_key ( x , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          break;
      }
//TCB
//  if(item!=br->df) ok = br->df;
      return ok;
  }
  int EventInY ( DIY *y , KBEVENT kbevent ) {
      BRW_STR *br;
      int ok = -1 , NOK = 1 , item , ret = -1;
      int ans , df = -1;
      DIALOG *D;
      br = y->bwsr;
      D = br->D;
      if ( br->menu == NULL ) return ok;
      if ( br->menu [ 0 ] == NULL ) return ok;
      item = br->df;
      ans = kbevent.key;
 // set_pointer_position(kbevent.x,kbevent.y,kbevent.button);
      D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
      switch ( kbevent.event ) {
          case 0:
//TCB  felt it is unpleasent the item highlights on mouse movement
//     it can be switched on if needed
//          _ui_scroll_item_hilight(br,kbevent);
          break;
          case 1:
          ok = _ui_process_y_click ( y , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          ret = ok;
          break;
          case 2:
          break;
          case 3:
          ret = _ui_process_y_move ( y , kbevent ) ;
          break;
          case 4:
          break;
          case 5:
          ok = _ui_process_y_key ( y , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          ret = ok;
          break;
      }
//TCB
//  if(item!=br->df) ok = br->df;
      return ret;
  }
  int EventInTextBox ( DIT *t , KBEVENT kbevent ) {
      int ok = -1 , NOK = 1 , item , ret = -1 , k;
      int ans , df = -1;
      void *Wid;
      char *str;
      int xs , xe , xo , hlt = 0 , strl = 0;
      TX_STR *tx;
      T_ELMT *elmt;
      KBEVENT en;
      DIALOG *D;
      D = t->D;
      ans = kbevent.key;
      D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
      elmt = t->elmt;
      tx = t->tstr;
      k = tx->row*tx->nx+tx->col;
#if 0
      if ( elmt [ k ] .hlt ) {
          elmt [ k ] .hlt = 0;
          _ui_drawtextcursor ( tx ) ;
          kgUpdateOn ( D ) ;
      }
#else
      kgClearHighlight ( D ) ;
#endif
//  printf("Drag in Text Box :Event= %d key=%d x:y=%d:%d\n",
//         kbevent.event,kbevent.button,kbevent.x,kbevent.y);
      switch ( kbevent.event ) {
          case 0:
//TCB  felt it is unpleasent the item highlights on mouse movement
//     it can be switched on if needed
//          _ui_scroll_item_hilight(br,kbevent);
          break;
          case 1:
//          ok = _ui_process_v_click(y,kbevent);
          ret = ok;
          break;
          case 2:
          break;
          case 3:
//          ok=_ui_process_v_move(y,kbevent);
          xo = kbevent.x;
          strl = _ui_textboxstringlength ( tx ) ;
          while ( 1 ) {
              en = kgGetEvent ( D ) ;
              if ( en.event != 3 ) break;
              Wid = kgGetLocationWidget ( D , en.x , en.y ) ;
              if ( Wid != t ) break;
              if ( en.event == 3 ) {
                  hlt = 1;
                  xe = en.x;
                  elmt [ k ] .hlt = hlt;
                  if ( xo < xe ) {
                      elmt [ k ] .hxs = xo-elmt [ k ] .x1;
                      elmt [ k ] .hxe = xe-elmt [ k ] .x1;
                  }
                  else {
                      elmt [ k ] .hxs = xe-elmt [ k ] .x1;
                      elmt [ k ] .hxe = xo-elmt [ k ] .x1;
                  }
                  if ( elmt [ k ] .hxe > strl ) elmt [ k ] .hxe = strl;
                  if ( elmt [ k ] .hxs > elmt [ k ] .hxe ) elmt [ k ] .hxs = elmt [ k ] .hxe;
//              kgUpdateWidget(t);
                  _ui_drawtextcursor ( tx ) ;
                  kgUpdateOn ( D ) ;
              }
          }
          str = _ui_gethighlightstring ( tx ) ;
          if ( str != NULL ) {
//            printf("hi: %s\n",str);
              if ( WC ( D )->Pstr != NULL ) free ( WC ( D )->Pstr ) ;
              kgSetPrimary ( D , str ) ;
              kgSetClipBoard(D,str);
              WC ( D )->Pstr = str;
          }
//          _ui_cuthighlightstring(tx);
          break;
          case 4:
          break;
          case 5:
 //         ok = _ui_process_v_key(y,kbevent);
          ret = ok;
          break;
      }
      return ret;
  }
  int EventInTableBox ( DIT *t , KBEVENT kbevent ) {
      int ok = -1 , NOK = 1 , item , ret = -1 , k;
      int ans , df = -1;
      void *Wid;
      char *str;
      int xs , xe , xo , hlt = 0 , strl = 0;
      TX_STR *tx;
      T_ELMT *elmt;
      KBEVENT en;
      DIALOG *D;
      D = t->D;
      ans = kbevent.key;
      D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
      elmt = t->elmt;
      tx = t->tstr;
      k = tx->row*tx->nx+tx->col;
#if 0
      if ( elmt [ k ] .hlt ) {
          elmt [ k ] .hlt = 0;
          _ui_drawtextcursor ( tx ) ;
          kgUpdateOn ( D ) ;
      }
#else
      kgClearHighlight ( D ) ;
#endif
//  printf("Drag in Table Box :Event= %d key=%d x:y=%d:%d\n",
//         kbevent.event,kbevent.button,kbevent.x,kbevent.y);
      switch ( kbevent.event ) {
          case 0:
//TCB  felt it is unpleasent the item highlights on mouse movement
//     it can be switched on if needed
//          _ui_scroll_item_hilight(br,kbevent);
          break;
          case 1:
//          ok = _ui_process_v_click(y,kbevent);
          ret = ok;
          break;
          case 2:
          break;
          case 3:
//          ok=_ui_process_v_move(y,kbevent);
          xo = kbevent.x;
          strl = _ui_tableboxstringlength ( tx ) ;
          while ( 1 ) {
              en = kgGetEvent ( D ) ;
              if ( en.event != 3 ) break;
              Wid = kgGetLocationWidget ( D , en.x , en.y ) ;
              if ( Wid != t ) break;
              if ( en.event == 3 ) {
                  hlt = 1;
                  xe = en.x;
                  elmt [ k ] .hlt = hlt;
                  if ( xo < xe ) {
                      elmt [ k ] .hxs = xo-elmt [ k ] .x1;
                      elmt [ k ] .hxe = xe-elmt [ k ] .x1;
                  }
                  else {
                      elmt [ k ] .hxs = xe-elmt [ k ] .x1;
                      elmt [ k ] .hxe = xo-elmt [ k ] .x1;
                  }
                  if ( elmt [ k ] .hxe > strl ) elmt [ k ] .hxe = strl;
                  if ( elmt [ k ] .hxs > elmt [ k ] .hxe ) elmt [ k ] .hxs = elmt [ k ] .hxe;
		  elmt [ k ] .hxs= elmt [ k ] .cursor*t->FontSize+t->FontSize/2;
//		  elmt [ k ] .cursor = (elmt [ k ] .hxe -t->FontSize/2.)/t->FontSize+0.5;
//              kgUpdateWidget(t);
                  _ui_drawtablecursor ( tx ) ;
                  kgUpdateOn ( D ) ;
              }
          }
          str = _ui_gettablehighlightstring ( tx ) ;
          if ( str != NULL ) {
//            printf("hi: %s\n",str);
              if ( WC ( D )->Pstr != NULL ) free ( WC ( D )->Pstr ) ;
              kgSetPrimary ( D , str ) ;
              kgSetClipBoard(D,str);
              WC ( D )->Pstr = str;
          }
//	  else printf("Highlight string is NULL\n");
//          _ui_cuthighlightstring(tx);
          break;
          case 4:
          break;
          case 5:
 //         ok = _ui_process_v_key(y,kbevent);
          ret = ok;
          break;
      }
      return ret;
  }
  int EventInV ( DIV *y , KBEVENT kbevent ) {
      int ok = -1 , NOK = 1 , item , ret = -1;
      int ans , df = -1;
      DIALOG *D;
      D = y->D;
      ans = kbevent.key;
      D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
      switch ( kbevent.event ) {
          case 0:
//TCB  felt it is unpleasent the item highlights on mouse movement
//     it can be switched on if needed
//          _ui_scroll_item_hilight(br,kbevent);
          break;
          case 1:
          ok = _ui_process_v_click ( y , kbevent ) ;
          ret = ok;
          break;
          case 2:
          break;
          case 3:
          ok = _ui_process_v_move ( y , kbevent ) ;
          break;
          case 4:
          break;
          case 5:
          ok = _ui_process_v_key ( y , kbevent ) ;
          ret = ok;
          break;
      }
      return ret;
  }
  int EventInZ ( DIZ *y , KBEVENT kbevent ) {
      int ok = -1 , NOK = 1 , item , ret = -1;
      int ans , df = -1;
      DIALOG *D;
      D = y->D;
      ans = kbevent.key;
      D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
      switch ( kbevent.event ) {
          case 0:
          break;
          case 1:
          ok = _ui_process_z_click ( y , kbevent ) ;
          ret = ok;
          break;
          case 2:
          break;
          case 3:
          ok = _ui_process_z_move ( y , kbevent ) ;
          break;
          case 4:
          break;
          case 5:
          ok = _ui_process_z_key ( y , kbevent ) ;
          ret = ok;
          break;
      }
      return ret;
  }
  int EventInCheckBox ( DICH *y , KBEVENT kbevent ) {
      BRW_STR *br;
      int ok = -1 , NOK = 1 , item , ret = -1;
      int ans , df = -1;
      DIALOG *D;
      br = y->bwsr;
      D = br->D;
      if ( br->menu == NULL ) return ok;
      if ( br->menu [ 0 ] == NULL ) return ok;
      item = br->df;
      ans = kbevent.key;
 // set_pointer_position(kbevent.x,kbevent.y,kbevent.button);
      D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
      switch ( kbevent.event ) {
          case 0:
//TCB  felt it is unpleasent the item highlights on mouse movement
//     it can be switched on if needed
//          _ui_scroll_item_hilight(br,kbevent);
          break;
          case 1:
          ok = _ui_process_c_click ( y , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          ret = ok;
          break;
          case 2:
          break;
          case 3:
          ret = _ui_process_c_move ( y , kbevent ) ;
          break;
          case 4:
          break;
          case 5:
          ok = _ui_process_c_key ( y , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          ret = ok;
          break;
      }
//TCB
//  if(item!=br->df) ok = br->df;
      return ret;
  }
  int EventInRadioButton ( DIRA *y , KBEVENT kbevent ) {
      BRW_STR *br;
      int ok = -1 , NOK = 1 , item , ret = -1;
      int ans , df = -1;
      DIALOG *D;
      br = y->bwsr;
      D = br->D;
      if ( br->menu == NULL ) return ok;
      if ( br->menu [ 0 ] == NULL ) return ok;
      item = br->df;
      ans = kbevent.key;
 // set_pointer_position(kbevent.x,kbevent.y,kbevent.button);
      D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
      switch ( kbevent.event ) {
          case 0:
//TCB  felt it is unpleasent the item highlights on mouse movement
//     it can be switched on if needed
//          _ui_scroll_item_hilight(br,kbevent);
          break;
          case 1:
          ok = _ui_process_r_click ( y , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          ret = ok;
          break;
          case 2:
          break;
          case 3:
          ret = _ui_process_r_move ( y , kbevent ) ;
          break;
          case 4:
          break;
          case 5:
          ok = _ui_process_r_key ( y , kbevent ) ;
          if ( ok >= 0 ) {
              if ( ok == 0 ) br->df = item;
              NOK = 0;
          }
          ret = ok;
          break;
      }
//TCB
//  if(item!=br->df) ok = br->df;
      return ret;
  }
  int EventInMsg ( BRW_STR *br , KBEVENT kbevent ) {
      int ok = -1 , NOK = 1 , item;
      int ans , df = -1;
      DIALOG *D;
      D = br->D;
      if ( br->menu == NULL ) return ok;
      if ( br->menu [ 0 ] == NULL ) return ok;
      item = br->df;
      ans = kbevent.key;
//  set_pointer_position(kbevent.x,kbevent.y,kbevent.button);
      D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
      printf ( "Msg Scroll\n" ) ;
      switch ( kbevent.event ) {
          case 0:
//           printf("Msg Scroll\n");
          _ui_scroll_msg_item ( br , kbevent ) ;
          break;
          case 1:
          ok = _ui_scroll_msg_click ( br , kbevent ) ;
          break;
          case 2:
          break;
          case 3:
          printf ( " MS Scroll Move \n" ) ;
          _ui_scroll_msg_move ( br , kbevent ) ;
          break;
          case 4:
          break;
          case 5:
          ok = _ui_scroll_msg_key ( br , kbevent ) ;
          break;
      }
      return 0;
  }
  int  kgProcessClip_old ( void *Tmp , void *kbtmp ) {
/* return value is made (void *) for further development */
      int  ret = 2;
      DIALOG *D;
      KBEVENT *kb;
      char *str = NULL;
      D = ( DIALOG * ) Tmp;
      kb = ( KBEVENT * ) kbtmp;
      if ( kb->event == 1 ) {
          switch ( kb->button ) {
              default:
              return 1;
              break;
              case 3: // primary
              str = kgGetPrimary ( Tmp ) ;
//	      printf("Case 2: %s\n",str);
              break;
              case 2: // clipboard
              str = kgGetClipBoard ( Tmp ) ;
//	      printf("Case 3: %s\n",str);
              break;
          }
      }
      if ( str != NULL ) {
          int ch;
          int i = 0;
          while ( ( ch = str [ i ] ) != '\0' ) {
            if(ch =='\n') { free(str);return 3;}
            i++;
          }          
          i=0;
          while ( ( ch = str [ i ] ) != '\0' ) {
              i++;
              if ( ch == '\n' ) {
                  kgSendClearKeyEvent ( Tmp ) ;
                  kgSendLinefeedKeyEvent ( Tmp ) ;
                  break;
              }
              else if ( ch == '\r' ) {
                  kgSendClearKeyEvent ( Tmp ) ;
                  kgSendEnterKeyEvent ( Tmp ) ;
                  break;
              }
              else kgSendKeyEvent ( Tmp , ch ) ;
          }
          free ( str ) ;
      }
      return ret;
  }
  void *  kgProcessClips ( void *Tmp , int butn ) {
/* return value is made (void *) for further development */
      char *  ret = NULL;
      DIALOG *D;
      char *str = NULL;
      D = ( DIALOG * ) Tmp;
          switch ( butn ) {
              default:
              return NULL;
              break;
              case 3: // primary
              str = kgGetPrimary ( Tmp ) ;
//	      printf("Case 3: %s\n",str);
              break;
              case 2: // clipboard
              str = kgGetClipBoard ( Tmp ) ;
//  	      printf("Case 2: %s\n",str);
              break;
          }
      if ( str != NULL ) {
          int ch;
          int i = 0;
          while ( ( ch = str [ i ] ) != '\0' ) {
//            if(ch =='\n') { free(str);return MULTILINE_CLIP;}
            if(ch =='\n') { return str;}
            i++;
          }          
          i=0;
          while ( ( ch = str [ i ] ) != '\0' ) {
              i++;
              if ( ch == '\n' ) {
                  kgSendClearKeyEvent ( Tmp ) ;
                  kgSendLinefeedKeyEvent ( Tmp ) ;
                  break;
              }
              else if ( ch == '\r' ) {
                  kgSendClearKeyEvent ( Tmp ) ;
                  kgSendEnterKeyEvent ( Tmp ) ;
                  break;
              }
              else kgSendKeyEvent ( Tmp , ch ) ;
          }
          free ( str ) ;
      }
      return NULL;
  }
  int ProcessMousePress ( DIALOG *D , KBEVENT kbevent , int i , int hcontrols , int controls ) \
  {
      DIA *d;
      int ch , df , OK = 0 , uperr , ret,butn=1;
      d = D->d;
      if ( controls > 0 ) {
          ch = D->d [ i ] .t->code;
          switch ( ch ) {
              case 'h':
              df = MousePressInButtonBox ( ( D->d [ i ] .h ) , kbevent ) ;
              if ( df >= 0 ) {
                  if ( Up_D_SplBt_Box ( df , i , D ) > 0 ) OK = 1;
                  else { OK = 0;}
                  _uiDrawButtons_n ( ( D->d [ i ] .h ) ) ;
                  uiUpdateOn ( D ) ;
              }
              else if ( D->Callback != NULL ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 'n':
              df = MousePressInButtonBox ( ( D->d [ i ] .N ) , kbevent ) ;
              if ( df >= 0 ) {
                  uperr = Up_D_Btn_Box ( ( D->d [ i ] .N->bval ) , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) {OK = 1; break;}
                  if ( ( hcontrols == 0 ) && ( uperr > 0 ) ) {OK = 1; break;}
                  _uiDrawButtons_n ( ( D->d [ i ] .b ) ) ;
                  uiUpdateOn ( D ) ;
              }
              else if ( D->Callback != NULL ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 'b':
              case 'N':
              df = MousePressInNewButtonBox ( ( D->d [ i ] .b ) , kbevent ) ;
              if ( df >= 0 ) {
                  uperr = Up_D_Bt_Box ( * ( D->d [ i ] .b->df ) , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) {OK = 1; break;}
                  if ( ( hcontrols == 0 ) && ( uperr > 0 ) ) {OK = 1; break;}
                  _uiDrawButtons ( ( D->d [ i ] .b ) ) ;
                  uiUpdateOn ( D ) ;
              }
              else if ( D->Callback != NULL ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 'H':
              df = MousePressInHBar ( ( D->d [ i ] .H ) , kbevent ) ;
              if ( df >= 0 ) {
                  if ( Up_D_Ht_Box ( df , i , D ) > 0 ) OK = 1;
                  else { OK = 0;}
                  _uiDrawButtons ( ( D->d [ i ] .b ) ) ;
                  uiUpdateOn ( D ) ;
              }
              else if ( D->Callback != NULL ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 'w':
              df = MousePressInBrowser ( ( DIW * ) ( D->d [ i ] .w ) , kbevent ) ;
              if ( df >= 0 ) {
                  uperr = Up_D_Brw_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              else if ( D->Callback != NULL ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 'x':
              ret = EventInNewMenu ( D->d [ i ] .x , kbevent ) ;
              if ( ret > 0 ) {
                  df = * ( D->d [ i ] .x->df ) ;
                  uperr = Up_D_NewMenu_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              else if ( ( ret == -1 ) && ( D->Callback != NULL ) ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 'v':
              df = EventInV ( D->d [ i ] .v , kbevent ) ;
              break;
              case 'z':
              df = EventInZ ( D->d [ i ] .z , kbevent ) ;
              break;
              case 'y':
              ret = EventInY ( D->d [ i ] .y , kbevent ) ;
              if ( ret > 0 ) {
                  df = * ( D->d [ i ] .y->df ) ;
                  uperr = Up_D_Y_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              else if ( ( ret == -1 ) && ( D->Callback != NULL ) ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 'c':
              ret = EventInCheckBox ( D->d [ i ] .c , kbevent ) ;
              if ( ret > 0 ) {
                  df = * ( D->d [ i ] .c->df ) ;
                  uperr = Up_D_Y_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              else if ( ( ret == -1 ) && ( D->Callback != NULL ) ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 'r':
              ret = EventInRadioButton ( D->d [ i ] .r , kbevent ) ;
              if ( ret > 0 ) {
                  df = * ( D->d [ i ] .r->df ) ;
                  uperr = Up_D_Y_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              else if ( ( ret == -1 ) && ( D->Callback != NULL ) ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 'f':
              df = MousePressInFslide ( ( S_STR * ) ( D->d [ i ] .f->sptr ) , kbevent ) ;
              if ( df >= 0 ) {
                  Up_Fsld_Bar ( _ui_getfslidevalue ( ( S_STR * ) \
                   ( D->d [ i ] .f->sptr ) ) , i , D ) ;
                  if ( ( controls == 1 ) ) {OK = 1; break;}
              }
              else if ( ( D->Callback != NULL ) ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 'd':
              df = MousePressInDslide ( ( S_STR * ) ( D->d [ i ] .d->sptr ) , kbevent ) ;
              if ( df >= 0 ) {
                  Up_Isld_Bar ( _ui_getdslidevalue ( ( S_STR * ) \
                   ( D->d [ i ] .d->sptr ) ) , i , D ) ;
                  if ( ( controls == 1 ) ) {OK = 1; break;}
              }
              else if ( ( D->Callback != NULL ) ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 'P':
              df = MousePressInHslideBar ( ( D->d [ i ] .B ) , kbevent ) ;
              if ( df >= 0 ) {
                  Up_Hsld_Bar ( _ui_getdslidevalue ( ( S_STR * ) \
                   ( D->d [ i ] .B->sptr ) ) , i , D ) ;
//       _ui_getdslidevalue((S_STR *)(D->d[i].B->sptr));
                  if ( ( controls == 1 ) ) {OK = 1; break;}
              }
              else if ( ( D->Callback != NULL ) ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 't':
              df = MousePressInTextBox ( ( TX_STR * ) ( D->d [ i ] .t->tstr ) , kbevent ) ;
              kgProcessClips ( ( void * ) D ,  kbevent.button ) ;
              break;
              case 'T':
              df = MousePressInTableBox ( ( TX_STR * ) ( D->d [ i ] .t->tstr ) , kbevent ) ;
#if 0
              butn =kgProcessClips ( ( void * ) D , kbevent.button ) ;
              if(butn==3) {// multi line clipboard
                   if ( d [ i ] .t->Update != NULL ) ret = d [ i ] .t->Update ( MULTILINE_CLIP , i , D ) ;
              }
#else

              butn = kbevent.button ;
              if(butn ==2) ret = d [ i ] .t->Update ( BUTTON2_PRESS, i , D ) ;
              else if(butn ==3) ret = d [ i ] .t->Update ( BUTTON3_PRESS, i , D ) ;
#endif
              break;
              case 'e':
              ret = EventInE ( ( D->d [ i ] .e ) , kbevent ) ;
              if ( ret > 0 ) {
                  df = * ( D->d [ i ] .e->df ) ;
                  uperr = Up_D_Menu_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              else if ( ( ret == -1 ) && ( D->Callback != NULL ) ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              case 's':
              df = EventInMS ( D->d [ i ] .s , kbevent ) ;
              if ( df > 0 ) {
                  uperr = Up_D_Menu_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              else if ( D->Callback != NULL ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
              default:
              if ( D->Callback != NULL ) OK = D->Callback ( D , & kbevent ) ; // Not tested yet
              break;
          }
      }
      else {
          if ( D->Callback != NULL ) OK = D->Callback ( D , & kbevent ) ;
      }
      return OK;
  }
  int uiGetFslideButtonDrag ( S_STR *sptr , KBEVENT kbevent ) {
      return _ui_processslidedrag ( sptr , kbevent ) ;
  }
  int MouseDragInFslide ( S_STR *sptr , KBEVENT kbevent ) {
      int kk , df;
      df = -1;
//   printf("MouseDragInFslide\n");
      if ( ( kk = uiGetFslideButtonDrag ( sptr , kbevent ) ) > 0 ) {
          df = sptr->df;
      }
      return df;
  }
  int uiGetDslideButtonDrag ( S_STR *sptr , KBEVENT kbevent ) {
      return _ui_processslidedrag ( sptr , kbevent ) ;
  }
  int MouseDragInDslide ( S_STR *sptr , KBEVENT kbevent ) {
      int kk , df;
      df = -1;
//   printf("MouseDragInFslide\n");
      if ( ( kk = uiGetDslideButtonDrag ( sptr , kbevent ) ) > 0 ) {
          df = sptr->df;
      }
      return df;
  }
//int uiGetHslideButtonDrag(S_STR *sptr,KBEVENT kbevent,int i) {
    //return _ui_processhslidedrag(sptr,kbevent,i);
//}
  int MouseDragInHslide ( DIHB *B , KBEVENT kbevent , int i ) {
      int kk , df;
      DIALOG *D;
      S_STR *sptr;
      sptr = B->sptr;
      B->item = i;
      D = ( DIALOG * ) B->D;
      df = -1;
//   printf("MouseDragInFslide\n");
      if ( ( kk = _ui_processhslidedrag ( B , kbevent ) ) > 0 ) {
          df = sptr->df;
          if ( B->Update == NULL ) {
              if ( D->Callback != NULL ) D->Callback ( D , & ( D->kb ) ) ;
          }
      }
      return df;
  }
  int ProcessMousePressDrag ( DIALOG *D , KBEVENT kbevent , int i , int hcontrols , int controls ) \
  {
      DIA *d;
      int ch , df , OK = 0 , uperr;
      void *Wid;
      if ( controls > 0 ) {
          Wid = kgGetLocationWidget ( D , kbevent.x , kbevent.y ) ;
          if ( Wid == NULL ) return OK;
          d = D->d;
          if ( Wid != ( void * ) ( d [ i ] .t ) ) {
//     printf("Not Current Widget\n");
              return OK;
          }
          ch = D->d [ i ] .t->code;
          switch ( ch ) {
              case 'n':
              case 'b':
              case 'N':
// not useful side effects
//       ProcessMousePress(D,kbevent,i,hcontrols,controls);
              break;
              case 'h':
              case 'H':
// not useful side effects
//       ProcessMousePress(D,kbevent,i,hcontrols,controls);
              break;
              case 't':
              df = EventInTextBox ( D->d [ i ] .t , kbevent ) ;
              break;
              case 'T':
              df = EventInTableBox ( D->d [ i ] .t , kbevent ) ;
              break;
              case 'v':
              df = EventInV ( D->d [ i ] .v , kbevent ) ;
              break;
              case 'z':
              df = EventInZ ( D->d [ i ] .z , kbevent ) ;
              break;
              case 'x':
              df = EventInNewMenu ( D->d [ i ] .x , kbevent ) ;
              if ( df > 0 ) {
                  BRW_STR *bwsr;
                  bwsr = ( BRW_STR * ) ( D->d [ i ] .x->bwsr ) ;
                  uperr = Up_D_NewMenu_Box ( bwsr->df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              case 'y':
              df = EventInY ( D->d [ i ] .y , kbevent ) ;
              if ( df > 0 ) {
                  BRW_STR *bwsr;
                  bwsr = ( BRW_STR * ) ( D->d [ i ] .y->bwsr ) ;
                  uperr = Up_D_Y_Box ( bwsr->df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              case 'r':
              df = EventInRadioButton ( D->d [ i ] .r , kbevent ) ;
              if ( df > 0 ) {
                  BRW_STR *bwsr;
                  bwsr = ( BRW_STR * ) ( D->d [ i ] .r->bwsr ) ;
                  uperr = Up_D_Y_Box ( bwsr->df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              case 'c':
              df = EventInCheckBox ( D->d [ i ] .c , kbevent ) ;
              if ( df > 0 ) {
                  BRW_STR *bwsr;
                  bwsr = ( BRW_STR * ) ( D->d [ i ] .c->bwsr ) ;
                  uperr = Up_D_Y_Box ( bwsr->df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              case 'w':
              break;
              case 'f':
              df = MouseDragInFslide ( ( S_STR * ) ( D->d [ i ] .f->sptr ) , kbevent ) ;
              if ( df >= 0 ) {
//         Up_Fsld_Bar(_ui_getfslidevalue((S_STR *)(D->d[i].f->sptr)),i,D);
                  if ( ( controls == 1 ) ) {OK = 1; break;}
              }
              break;
              case 'd':
              df = MouseDragInDslide ( ( S_STR * ) ( D->d [ i ] .d->sptr ) , kbevent ) ;
              if ( df >= 0 ) {
//         Up_Isld_Bar(_ui_getdslidevalue((S_STR *)(D->d[i].d->sptr)),i,D);
                  if ( ( controls == 1 ) ) {OK = 1; break;}
              }
              break;
              case 'P':
              df = MouseDragInHslide ( ( D->d [ i ] .B ) , kbevent , i ) ;
              if ( df >= 0 ) {
//         Up_Hsld_Bar(_ui_getdslidevalue((S_STR *)(D->d[i].B->sptr)),i,D);
// Done in lowlib
                  if ( ( controls == 1 ) ) {OK = 1; break;}
              }
              break;
              case 'e':
              df = EventInE ( ( D->d [ i ] .e ) , kbevent ) ;
              if ( df > 0 ) {
                  BRW_STR *bwsr;
                  bwsr = ( BRW_STR * ) ( D->d [ i ] .e->bwsr ) ;
                  uperr = Up_D_Menu_Box ( bwsr->df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              case 's':
              df = EventInMS ( D->d [ i ] .s , kbevent ) ;
              if ( df > 0 ) {
                  uperr = Up_D_Menu_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              default:
              break;
          }
      }
      return OK;
  }
  int ProcessMouseMovement ( DIALOG *D , KBEVENT kbevent , int i , int controls ) \
  {
      DIA *d;
      int ch , df , OK = 0 , uperr;
      if ( controls > 0 ) {
          d = D->d;
          ch = D->d [ i ] .t->code;
//  printf("ProcessMouseMovement %c\n",ch);
          switch ( ch ) {
              case 'h':
              case 'n':
              case 'b':
              case 'N':
              break;
              case 'H':
              break;
              case 'c':
              break;
              case 'y':
              break;
              case 'r':
              break;
              case 'v':
              break;
              case 'z':
              break;
              case 'x':
// Not Needed can be swiched on if needed
 //      df = EventInMenu((BRW_STR *)(D->d[i].x->bwsr),kbevent);
              break;
              case 'w':
              break;
              case 'f':
              break;
              case 'd':
              break;
              case 'e':
// Not Needed can be swiched on if needed
 //      df = EventInMenu((BRW_STR *)(D->d[i].e->bwsr),kbevent);
              break;
              case 's':
// Not Needed can be swiched on if needed
//       df = EventInMsg((BRW_STR *)(D->d[i].s->bwsr),kbevent);
              break;
              default:
              break;
          }
      }
      return OK;
  }
  int GetNewButtonKeyRelease ( void *tmp , KBEVENT kbevent ) {
      DIB *B;
      int ans , df , dfmt , nbx , nby , dfrm;
      ans = kbevent.key;
      B = ( DIB * ) tmp;
      df = * ( B->df ) ;
      nbx = B->nx;
      nby = B->ny;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
//          printf("Got Line Feed\n");
          return df;
      }
      if ( ui_Uparrow ( ans ) ) {
          dfmt = ( df-1 ) /nbx;
          dfrm = ( df-1 ) %nbx+1;
          dfmt--;if ( dfmt < 0 ) dfmt = nby-1;
          df = dfmt*nbx+dfrm;
          while ( ( B->sw [ df-1 ] < 1 ) ) {
              dfrm--;if ( dfrm < 1 ) dfrm = nbx;
              df = dfmt*nbx+dfrm;
          }
//              _uiHighNewButton(bt,df);
          * ( B->df ) = df;
          _uiDrawButtons ( B ) ;
          uiUpdateOn ( B->D ) ;
          return -1;
      }
      if ( ui_Downarrow ( ans ) ) {
          dfmt = ( df-1 ) /nbx;
          dfrm = ( df-1 ) %nbx+1;
          dfmt++;if ( dfmt >= nby ) dfmt = 0;
          df = dfmt*nbx+dfrm;
          while ( ( B->sw [ df-1 ] < 1 ) ) {
              dfrm++;if ( dfrm > nbx ) dfrm = 1;
              df = dfmt*nbx+dfrm;
          }
//              _uiHighNewButton(bt,df);
          * ( B->df ) = df;
          _uiDrawButtons ( B ) ;
          uiUpdateOn ( B->D ) ;
          return -1;
      }
      if ( ui_Leftarrow ( ans ) ) {
          dfmt = ( df-1 ) /nbx;
          dfrm = ( df-1 ) %nbx+1;
          dfrm--;if ( dfrm < 1 ) dfrm = nbx;
          df = dfmt*nbx+dfrm;
          while ( ( B->sw [ df-1 ] < 1 ) ) {
              dfrm--;if ( dfrm < 1 ) dfrm = nbx;
              df = dfmt*nbx+dfrm;
          }
          * ( B->df ) = df;
          _uiDrawButtons ( B ) ;
          uiUpdateOn ( B->D ) ;
          return -1;
      }
      if ( ui_Rightarrow ( ans ) ) {
          dfmt = ( df-1 ) /nbx;
          dfrm = ( df-1 ) %nbx+1;
          dfrm++;if ( dfrm > nbx ) dfrm = 1;
          df = dfmt*nbx+dfrm;
          while ( ( B->sw [ df-1 ] < 1 ) ) {
              dfrm++;if ( dfrm > nbx ) dfrm = 1;
              df = dfmt*nbx+dfrm;
          }
          * ( B->df ) = df;
          _uiDrawButtons ( B ) ;
          uiUpdateOn ( B->D ) ;
          return -1;
      }
  }
  int GetButtonKeyRelease ( void *tmp , KBEVENT kbevent ) {
      DIN *B;
      int ans , df , dfmt , nbx , nby , dfrm , n , i;
      BUT_STR *butn;
      ans = kbevent.key;
      B = ( DIN * ) tmp;
      df = B->bval;
      butn = ( BUT_STR * ) B->buts;
      nbx = B->nx;
      nby = B->ny;
      if ( ui_Linefeed ( ans ) || ui_Return ( ans ) ) {
          return df;
      }
      if ( ui_Uparrow ( ans ) ) {
          dfmt = ( df-1 ) /nbx;
          dfrm = ( df-1 ) %nbx+1;
          dfmt--;if ( dfmt < 0 ) dfmt = nby-1;
          df = dfmt*nbx+dfrm;
          while ( ( butn [ df-1 ] .sw < 1 ) ) {
              dfrm--;if ( dfrm < 1 ) dfrm = nbx;
              df = dfmt*nbx+dfrm;
          }
           ( B->bval ) = df;
          _uiDrawButtons_n ( B ) ;
          uiUpdateOn ( B->D ) ;
          return -1;
      }
      if ( ui_Downarrow ( ans ) ) {
          dfmt = ( df-1 ) /nbx;
          dfrm = ( df-1 ) %nbx+1;
          dfmt++;if ( dfmt >= nby ) dfmt = 0;
          df = dfmt*nbx+dfrm;
          while ( ( butn [ df-1 ] .sw < 1 ) ) {
              dfrm++;if ( dfrm > nbx ) dfrm = 1;
              df = dfmt*nbx+dfrm;
          }
           ( B->bval ) = df;
          _uiDrawButtons_n ( B ) ;
          uiUpdateOn ( B->D ) ;
          return -1;
      }
      if ( ui_Leftarrow ( ans ) ) {
          dfmt = ( df-1 ) /nbx;
          dfrm = ( df-1 ) %nbx+1;
          dfrm--;if ( dfrm < 1 ) dfrm = nbx;
          df = dfmt*nbx+dfrm;
          while ( ( butn [ df-1 ] .sw < 1 ) ) {
              dfrm--;if ( dfrm < 1 ) dfrm = nbx;
              df = dfmt*nbx+dfrm;
          }
           ( B->bval ) = df;
          _uiDrawButtons_n ( B ) ;
          uiUpdateOn ( B->D ) ;
          return -1;
      }
      if ( ui_Rightarrow ( ans ) ) {
          dfmt = ( df-1 ) /nbx;
          dfrm = ( df-1 ) %nbx+1;
          dfrm++;if ( dfrm > nbx ) dfrm = 1;
          df = dfmt*nbx+dfrm;
          while ( ( butn [ df-1 ] .sw < 1 ) ) {
              dfrm++;if ( dfrm > nbx ) dfrm = 1;
              df = dfmt*nbx+dfrm;
          }
           ( B->bval ) = df;
          _uiDrawButtons_n ( B ) ;
          uiUpdateOn ( B->D ) ;
          return -1;
      }
      if ( ( ans >= ' ' ) && ( ans < 128 ) ) {
          n = nbx*nby;
          for ( i = 0;i < n;i++ ) {
              if ( butn [ i ] .butncode == ans ) {
                  df = i+1;
                  if ( butn [ i ] .sw >= 1 ) {
                       ( B->bval ) = df;
                      _uiDrawButtons_n ( B ) ;
                      uiUpdateOn ( B->D ) ;
                      return df;
                  }
              }
          }
      }
      return -1;
  }
  int KeyReleaseInNewButtonBox ( void *tmp , KBEVENT kbevent ) {
      int kk , df;
      DIB *B;
      B = ( DIB * ) tmp;
      df = -1;
      if ( ( kk = GetNewButtonKeyRelease ( B , kbevent ) ) > 0 ) {
          df = * ( B->df ) ;
      }
      return df;
  }
  int KeyReleaseInButtonBox ( void *tmp , KBEVENT kbevent ) {
      int kk , df;
      DIN *B;
      B = ( DIN * ) tmp;
      df = -1;
      if ( ( kk = GetButtonKeyRelease ( B , kbevent ) ) > 0 ) {
          df = ( B->bval ) ;
      }
      return df;
  }
  int KeyReleaseInHBar ( BUTS *bt , KBEVENT kbevent ) {
      int kk , df;
      df = -1;
//   printf("KeyReleaseInHBAr\n");
      if ( ( kk = GetNewButtonKeyRelease ( bt , kbevent ) ) > 0 ) {
          df = bt->df;
          if ( ( kk != 0 ) ) {
//          _uiHighButton(bt,df);
              _uiHighNewButton ( bt , df ) ;
              uiUpdateOn ( bt->D ) ;
          }
      }
      return df;
  }
  int KeyReleaseInBrowser ( DIW *w , KBEVENT kbevent ) {
      BRW_STR *br;
      int ans , df = -1;
      ans = kbevent.key;
      if ( ui_Downarrow ( ans ) ) {
          Gclr gc;
          int x1 , y1 , x2 , y2 , PON_X , PON_Y , df;
          char **m;
          int evgax , evgay;
          DIALOG *D;
          kgWC *wc;
          D = ( DIALOG * ) ( w->D ) ;
          wc = WC ( D ) ;
          uiBkup_clip_limits ( wc ) ;
          uiSet_full_scrn ( wc ) ;
          evgay = D->evgay;
          x1 = w->x2-w->width-w->offset+D->xo;
          y1 = w->y1+D->yo;
          x2 = x1+w->width;
          y2 = y1+w->width;
          df = _uiMenu ( w ) ;
          br = w->bwsr;
//    kg_scr_recover(wc);
          gc = D->gc;
          m = br->menu;
//    ui_draw_browser((DIW *)w);
          _uiMake_W ( ( DIW * ) w ) ;
          uiUpdateOn ( D ) ;
          uiRest_clip_limits ( wc ) ;
          return ( br->df ) ;
      }
      return df;
  }
  int KeyReleaseInFslide ( S_STR *sptr , KBEVENT kbevent ) {
      return _ui_processslidekey ( sptr , kbevent ) ;
  }
  int KeyReleaseInDslide ( S_STR *sptr , KBEVENT kbevent ) {
      return _ui_processslidekey ( sptr , kbevent ) ;
  }
  int KeyReleaseInHslide ( DIHB *B , KBEVENT kbevent ) {
      return _ui_processhslidekey ( B , kbevent ) ;
  }
  int KeyReleaseInTextBox ( TX_STR *tptr , KBEVENT kbevent ) {
      return _ui_processTextTablekey ( tptr , kbevent , 't' ) ;
  }
  int KeyReleaseInTableBox ( TX_STR *tptr , KBEVENT kbevent ) {
      return _ui_processTablekey ( tptr , kbevent , 'T' ) ;
  }
  int ProcessKeyRelease ( DIALOG *D , KBEVENT kbevent , int i , int hcontrols , int controls ) \
  {
      DIA *d;
      int ch , df , OK = 0 , uperr , ret;
      if ( controls > 0 ) {
          d = D->d;
          ch = D->d [ i ] .t->code;
          switch ( ch ) {
              case 'h':
              df = KeyReleaseInButtonBox ( ( D->d [ i ] .h ) , kbevent ) ;
              if ( df >= 0 ) {
                  if ( Up_D_SplBt_Box ( df , i , D ) > 0 ) OK = 1;
                  else { OK = 0;}
                  _uiDrawButtons_n ( ( D->d [ i ] .H ) ) ;
                  uiUpdateOn ( D ) ;
              }
              break;
              case 'n':
              df = KeyReleaseInButtonBox ( ( D->d [ i ] .N ) , kbevent ) ;
              if ( df >= 0 ) {
                  uperr = Up_D_Btn_Box ( ( D->d [ i ] .N->bval ) , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) {OK = 1; break;}
                  if ( ( hcontrols == 0 ) && ( uperr > 0 ) ) {OK = 1; break;}
                  _uiDrawButtons_n ( ( D->d [ i ] .n ) ) ;
                  uiUpdateOn ( D ) ;
              }
              break;
              case 'b':
              case 'N':
              df = KeyReleaseInNewButtonBox ( ( D->d [ i ] .n ) , kbevent ) ;
              if ( df >= 0 ) {
                  uperr = Up_D_Bt_Box ( ( ( BUTS * ) D->d [ i ] .n->buts )->df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) {OK = 1; break;}
                  if ( ( hcontrols == 0 ) && ( uperr > 0 ) ) {OK = 1; break;}
                  _uiDrawButtons ( ( D->d [ i ] .n ) ) ;
                  uiUpdateOn ( D ) ;
              }
              break;
              case 'H':
              df = KeyReleaseInHBar ( ( BUTS* ) ( D->d [ i ] .H->buts ) , kbevent ) ;
              if ( df >= 0 ) {
                  if ( Up_D_Ht_Box ( df , i , D ) > 0 ) OK = 1;
                  else { OK = 0;}
                  _uiDrawButtons ( ( D->d [ i ] .n ) ) ;
                  uiUpdateOn ( D ) ;
              }
              break;
              case 'w':
              df = KeyReleaseInBrowser ( ( DIW * ) ( d [ i ] .w ) , kbevent ) ;
              if ( df >= 0 ) {
                  uperr = Up_D_Brw_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              case 'v':
              df = EventInV ( D->d [ i ] .v , kbevent ) ;
              break;
              case 'z':
              df = EventInZ ( D->d [ i ] .z , kbevent ) ;
              break;
              case 'x':
              ret = EventInNewMenu ( D->d [ i ] .x , kbevent ) ;
              if ( ret > 0 ) {
                  df = * ( D->d [ i ] .x->df ) ;
                  uperr = Up_D_NewMenu_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              case 'y':
              ret = EventInY ( D->d [ i ] .y , kbevent ) ;
              if ( ret > 0 ) {
                  df = * ( D->d [ i ] .y->df ) ;
                  uperr = Up_D_Y_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              case 'c':
              ret = EventInCheckBox ( D->d [ i ] .c , kbevent ) ;
              if ( ret > 0 ) {
                  df = * ( D->d [ i ] .c->df ) ;
                  uperr = Up_D_Y_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              case 'r':
              ret = EventInRadioButton ( D->d [ i ] .r , kbevent ) ;
              if ( ret > 0 ) {
                  df = * ( D->d [ i ] .r->df ) ;
                  uperr = Up_D_Y_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              case 'e':
              ret = EventInE ( ( D->d [ i ] .e ) , kbevent ) ;
              if ( ret > 0 ) {
                  df = * ( D->d [ i ] .e->df ) ;
                  uperr = Up_D_Menu_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              case 's': //Need modification
              df = EventInMS ( D->d [ i ] .s , kbevent ) ;
              if ( df > 0 ) {
                  uperr = Up_D_Menu_Box ( df , i , D ) ;
                  if ( ( controls == 1 ) && ( uperr > 0 ) ) OK = 1;
              }
              break;
              case 'f':
              df = KeyReleaseInFslide ( ( S_STR * ) ( D->d [ i ] .f->sptr ) , kbevent ) ;
              if ( df >= 0 ) {
                  Up_Fsld_Bar ( _ui_getfslidevalue ( ( S_STR * ) \
                   ( D->d [ i ] .f->sptr ) ) , i , D ) ;
                  if ( ( controls == 1 ) ) {OK = 1; break;}
              }
              break;
              case 'd':
              df = KeyReleaseInDslide ( ( S_STR * ) ( D->d [ i ] .d->sptr ) , kbevent ) ;
              if ( df >= 0 ) {
                  Up_Isld_Bar ( _ui_getdslidevalue ( ( S_STR * ) \
                   ( D->d [ i ] .d->sptr ) ) , i , D ) ;
                  if ( ( controls == 1 ) ) {OK = 1; break;}
              }
              break;
              case 'P':
              df = KeyReleaseInHslide ( ( D->d [ i ] .B ) , kbevent ) ;
              if ( df >= 0 ) {
                  Up_Hsld_Bar ( _ui_getdslidevalue ( ( S_STR * ) \
                   ( D->d [ i ] .B->sptr ) ) , i , D ) ;
                  if ( ( controls == 1 ) ) {OK = 1; break;}
              }
              break;
              case 't':
              df = KeyReleaseInTextBox ( ( TX_STR * ) ( D->d [ i ] .t->tstr ) , kbevent ) ;
              if ( df >= 0 ) {
                  if ( _ui_readtextbox ( ( TX_STR * ) ( D->d [ i ] .t->tstr ) ) < 0 ) {
                      gprintf ( D , "Error in Text box data" ) ;
                  }
                  else {
                      Up_D_Tx_Table ( 0 , i , D ) ;
                      if ( ( controls == 1 ) ) {OK = 1; break;}
                  }
              }
              break;
              case 'T':
              df = KeyReleaseInTableBox ( ( TX_STR * ) ( D->d [ i ] .t->tstr ) , kbevent ) ;
              if ( df != -1 ) {
                  if ( _ui_readtextbox ( ( TX_STR * ) ( D->d [ i ] .t->tstr ) ) < 0 ) {
                      gprintf ( D , "Error in Table box data" ) ;
                  }
                  else {
                      Up_D_Table ( df , i , D ) ;
                      if ( ( controls == 1 ) ) {OK = 1; break;}
                  }
              }
              break;
              default:
              break;
          }
      }
      return OK;
  }
  int ui_NotInputBox ( DIALOG *D , int i ) {
      int ch;
      ch = D->d [ i ] .t->code;
      if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) == 0 ) return 1;
      if ( ch == 't' ) return 0;
      if ( ch == 'T' ) return 0;
      if ( ch == 'y' ) return 0;
      if ( ch == 'r' ) return 0;
      if ( ch == 'c' ) return 0;
      if ( ch == 'n' ) return 0;
      if ( ch == 'N' ) return 0;
      if ( ch == 'b' ) return 0;
      if ( ch == 'H' ) return 0;
      if ( ch == 'h' ) return 0;
      if ( ch == 'w' ) return 0;
      if ( ch == 'x' ) return 0;
      if ( ch == 'v' ) return 0;
      if ( ch == 'z' ) return 0;
      if ( ch == 'f' ) return 0;
      if ( ch == 'd' ) return 0;
      if ( ch == 'P' ) return 0;
      if ( ch == 'e' ) return 0;
      if ( ch == 's' ) return 0;
      return 1;
  }
  int uiSetCleantmpdir ( void ) {
      int pid;
      char buff [ 500 ] ;
      char pidstr [ 50 ] ;
      char tmpdir [ 200 ] ;
      if ( PID == 0 ) PID = getpid ( ) ;
      if ( ( pid = fork ( ) ) == 0 ) return getpid ( ) ;
      strcpy ( buff , getenv ( "KULINA" ) ) ;
      strcat ( buff , "/bin/" ) ;
      strcat ( buff , "cleantmpdir" ) ;
      sprintf ( pidstr , "%-d" , pid ) ;
      if ( chkshm ( ) ) {
          sprintf ( tmpdir , "/dev/shm/%-d*" , PID ) ;
      }
      else sprintf ( tmpdir , "/tmp/%-d*" , PID ) ;
      execl ( buff , "cleantmpdir" , pidstr , tmpdir , NULL ) ;
      return 0;
  }
  int kgSetCleantmpdir ( char *tmpdir ) {
      int pid;
      FILE *fp;
      char *pt;
      char buff [ 500 ] ;
      char pidstr [ 50 ] ;
      pid = getpid ( ) ;
      pt = getenv ( "KULINA" ) ;
      if ( pt == NULL ) return pid;
      strcpy ( buff , pt ) ;
      strcat ( buff , "/bin/" ) ;
      strcat ( buff , "cleantmpdir" ) ;
      fp = fopen ( buff , "r" ) ;
      if ( fp == NULL ) return pid;
      fclose ( fp ) ;
      if ( ( pid = fork ( ) ) == 0 ) return getpid ( ) ;
      sprintf ( pidstr , "%-d" , pid ) ;
      execl ( buff , "cleantmpdir" , pidstr , tmpdir , NULL ) ;
      return 0;
  }
  int uiGoBackGround ( void ) {
      int status , pid;
      if ( ( pid = fork ( ) ) == 0 ) {
          if ( ( pid = fork ( ) ) != 0 ) {
              printf ( "Process id = %d\n" , pid ) ;
              exit ( 0 ) ;
          }
          else return 1;
      }
      else {
          WAIT ( pid ) ;
          exit ( 0 ) ;
      }
  }
  int kgGoBackGround ( void ) {
      int status , pid;
      if ( ( pid = fork ( ) ) == 0 ) {
          if ( ( pid = fork ( ) ) != 0 ) {
              printf ( "Process id = %d\n" , pid ) ;
              exit ( 0 ) ;
          }
          else return 1;
      }
      else {
          WAIT ( pid ) ;
          exit ( 0 ) ;
      }
  }
  int uiReadinDialogValues ( DIALOG *D , int OK ) {
      DIA *d;
      int i = 0 , ret = 0;
      int ch;
      d = D->d;
      ret = OK;
      uiUpdateOff ( D ) ;
//   for(i=0;i<n;i++) {
      while ( d [ i ] .t != NULL ) {
          ch = ( d [ i ] .t->code ) ;
          switch ( ( int ) ch ) {
              case 'r':
              * ( d [ i ] .r->df ) = ( ( BRW_STR * ) d [ i ] .r->bwsr )->df;
              break;
              case 'c':
              * ( d [ i ] .c->df ) = ( ( BRW_STR * ) d [ i ] .c->bwsr )->df;
              break;
              case 'y':
              * ( d [ i ] .y->df ) = ( ( BRW_STR * ) d [ i ] .y->bwsr )->df;
              break;
              case 'x':
              * ( d [ i ] .x->df ) = ( ( BRW_STR * ) d [ i ] .x->bwsr )->df;
              break;
              case 't':
              break;
              case 'T':
              break;
              case 'h':
              ret = ( d [ i ] .h->bval ) ;
              break;
              case 'H':
              ret = * ( d [ i ] .H->df ) ;
              break;
              case 'n':
              break;
              case 'b':
              case 'N':
              break;
              case 'f':
              * ( d [ i ] .f->df ) = _ui_getfslidevalue \
               ( ( S_STR * ) ( d [ i ] .f->sptr ) ) ;
              break;
              case 'd':
              * ( d [ i ] .d->df ) = _ui_getdslidevalue \
               ( ( S_STR * ) ( d [ i ] .d->sptr ) ) ;
              break;
              case 'P':
              * ( d [ i ] .d->df ) = _ui_getdslidevalue \
               ( ( S_STR * ) ( d [ i ] .B->sptr ) ) ;
              break;
              case 'w':
              * ( d [ i ] .w->df ) = ( ( BRW_STR * ) d [ i ] .w->bwsr )->df;
              break;
              case 'g':
              break;
              case 'm':
              break;
              case 'p':
              break;
              case 'M':
              break;
              case 'B':
              break;
              case 'i':
              break;
              case 'e':
              * ( d [ i ] .e->df ) = ( ( BRW_STR * ) d [ i ] .e->bwsr )->df;
              break;
              case 's':
              break;
              case 'o':
              break;
              case 'v':
              break;
              case 'z':
              break;
              default:
              normal ( ) ;
              printf ( "Error: in dialog ; wrong code %c \n" , ch ) ;
              exit ( 0 ) ;
          }
          i++;
      }
      if ( OK > 1000 ) ret = OK;
      uiUpdateOn ( D ) ;
      return ret;
  }
  void kgInitUi ( void *Tmp ) {
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      D->xo = D->yo = 10;
      D->xl = D->yl = 300;
      D->bkup = 0;
      D->bor_type = 4;
      D->tw = D->bw = D->lw = D->rw = 4;
      D->df = 0;
      D->Initfun = NULL;
      D->Cleanupfun = NULL;
      D->ResizeCallback = NULL;
      D->WaitCallback = NULL;
      D->Callback = NULL;
      D->kbattn = D->butattn = 0;
      kgDefaultGuiTheme ( & ( D->gc ) ) ;
      D->fullscreen = 0;
      D->KILL = 0;
      D->SUBWIN = 0;
      D->PWIN = NULL;
      D->NoWinMngr = 0;
      D->Maxl = 0;
      D->Maxw = 0;
      D->ThInfo = NULL;
// D->VerId=VER;  //yymmvvss
// strcpy(D->name,"Kulina Designer");
      sprintf ( D->name , "Kulina Designer Ver. %d.%2.2d" , \
       ( ( D->VerId%10000 ) /100 ) , ( ( D->VerId%100 ) ) ) ;
      D->Deco = D->DrawBkgr = D->Newwin = 1;
      D->Sticky = 0;
      D->Fixpos = D->NoTaskBar = 0;
      D->transparency = 0.0;
      D->StackPos = 0;
      D->Resize = 0;
      D->MinWidth = 100;
      D->MinHeight = 100;
      D->wc = D->parent = D->pt = D->Shapexpm = D->SearchList = D->GrpList = NULL;
      D->TotWid = 0;
      D->CurWid = 0;
      D->InputWid = -1;
      D->Kbrd = NULL;
      return;
  }
  void *kgUiFromWindow ( void *Dsp , void * Win , void * cmap ) {
      DIALOG *D;
      kgWC *wc;
      D = ( DIALOG * ) Malloc ( sizeof ( DIALOG ) ) ;
      kgInitUi ( D ) ;
      wc = ( kgWC * ) Malloc ( sizeof ( kgWC ) ) ;
      D->wc = wc;
      wc->Dsp = ( Display * ) Dsp;
      wc->Win = ( Window ) Win;
      wc->Cmap = ( Colormap ) cmap;
//   uiMakeFontlist();
      return ( void * ) D;
  }
  void kgFreeUifromWindow ( void *D ) {
      DIALOG *Dtmp;
      if ( D == NULL ) return;
      Dtmp = ( DIALOG * ) D;
      if ( Dtmp->wc != NULL ) Free ( Dtmp->wc ) ;
      Free ( D ) ;
      return;
  }
  void kgCleanUi ( void *tmp ) {
/* Clear memory allocated externaly */
      DIALOG *D;
      Dlink *Grp;
      Dlink *Gpt;
      D = ( DIALOG * ) tmp;
      WIDGETGRP *pt = NULL;
#if 1
      if ( D->SearchList != NULL ) Dempty ( ( Dlink * ) D->SearchList ) ;
      Grp = ( Dlink * ) D->GrpList;
      if ( Grp != NULL ) {
          Resetlink ( Grp ) ;
          while ( ( pt = ( WIDGETGRP * ) Getrecord ( Grp ) ) != NULL ) {
              if ( pt->CleanupGrp != NULL ) pt->CleanupGrp ( D ) ;
              Dfree ( ( Dlink * ) pt->wlist ) ;
              if ( pt->arg != NULL ) kgFreeDouble ( pt->arg ) ;
              pt->arg = NULL;
          }
          Dempty ( Grp ) ;
          D->GrpList = NULL;
      }
      D->SearchList = D->GrpList = NULL;
#endif
 // TCB Checking
#if 1
      switch ( D->VerId ) {
          case 1401010100:
          uiFreeWidgetMem ( D ) ;
          kgFreeDouble ( ( void ** ) ( D->d ) ) ;
          D->d = NULL;
          break;
          case 1401010200:
          if ( D->Cleanupfun != NULL ) D->Cleanupfun ( D ) ;
          uiFreeWidgetMem ( D ) ;
          kgFreeDouble ( ( void ** ) ( D->d ) ) ;
          D->d = NULL;
          break;
          case 2107030000:
          if ( D->Cleanupfun != NULL ) D->Cleanupfun ( D ) ;
          uiFreeWidgetMem ( D ) ;
          Free ( ( D->Kbrd ) ) ;
          kgFreeDouble ( ( void ** ) ( D->d ) ) ;
          D->d = NULL;
          break;
          default :
#if 0
          if ( D->Cleanupfun != NULL ) D->Cleanupfun ( D ) ;
          uiFreeWidgetMem ( D ) ;
          kgFreeDouble ( ( void ** ) ( D->d ) ) ;
          D->d = NULL;
#endif
          break;
      }
#endif
      return;
  }
  int kgOpenGrp ( void *Tmp ) {
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      Dlink *Grp;
      Dlink *Gpt;
      WIDGETGRP *pt = NULL;
      int gid = 0;
      pt = ( WIDGETGRP * ) Malloc ( sizeof ( WIDGETGRP ) ) ;
      Grp = ( Dlink * ) D->GrpList;
      if ( Grp == NULL ) {
          Grp = Dopen ( ) ;
          D->GrpList = Grp;
      }
      Gpt = Dopen ( ) ;
      pt->wlist = ( void * ) Gpt;
      pt->arg = NULL;
      pt->hide = 0;
      pt->CleanupGrp = NULL;
      Dappend ( Grp , pt ) ;
      gid = Dcount ( Grp ) ;
// printf("Dcount= %d\n",gid);
      return gid;
  }
  int kgAddtoGrp_o ( void *tmp , int grpid , int WidgetNo ) {
      int *Widget;
      DIALOG *D;
      D = ( DIALOG * ) tmp;
      Dlink *Grp;
      Dlink *Gpt;
      WIDGETGRP *pt = NULL;
      Grp = ( Dlink * ) D->GrpList;
      if ( Grp == NULL ) {
          return 0;
      }
      if ( Dcount ( Grp ) < grpid ) return 0;
      if ( grpid < 1 ) return 0;
      Resetlink ( Grp ) ;
      Dposition ( Grp , grpid ) ;
      pt = ( WIDGETGRP * ) Getrecord ( Grp ) ;
      Gpt = ( Dlink * ) pt->wlist;
      Widget = ( int * ) Malloc ( sizeof ( int ) ) ;
      *Widget = WidgetNo;
      Dappend ( Gpt , Widget ) ;
      return 1;
  }
  int kgAddtoGrp ( void *tmp , int grpid , void * Widget ) {
      DIALOG *D;
      D = ( DIALOG * ) tmp;
      Dlink *Grp;
      Dlink *Gpt;
      WIDGETGRP *pt = NULL;
      Grp = ( Dlink * ) D->GrpList;
      if ( Grp == NULL ) {
          return 0;
      }
      if ( Dcount ( Grp ) < grpid ) return 0;
      if ( grpid < 1 ) return 0;
      Resetlink ( Grp ) ;
      Dposition ( Grp , grpid ) ;
      pt = ( WIDGETGRP * ) Getrecord ( Grp ) ;
      Gpt = ( Dlink * ) pt->wlist;
      Dappend ( Gpt , Widget ) ;
      return 1;
  }
  int kgSetGrpCleanup ( void *tmp , int grpid , int ( *Cleanup ) ( void * ) ) {
      DIALOG *D;
      D = ( DIALOG * ) tmp;
      Dlink *Grp;
      Dlink *Gpt;
      WIDGETGRP *pt = NULL;
      Grp = ( Dlink * ) D->GrpList;
      if ( Grp == NULL ) {
          return 0;
      }
      if ( Dcount ( Grp ) < grpid ) return 0;
      if ( grpid < 1 ) return 0;
      Resetlink ( Grp ) ;
      Dposition ( Grp , grpid ) ;
      pt = ( WIDGETGRP * ) Getrecord ( Grp ) ;
      pt->CleanupGrp = Cleanup;
      return 1;
  }
  WIDGETGRP * kgGetWidgetGrp ( void *tmp , int grpid ) {
      int *Widget;
      DIALOG *D;
      D = ( DIALOG * ) tmp;
      Dlink *Grp;
      Dlink *Gpt;
      WIDGETGRP *pt = NULL;
      Grp = ( Dlink * ) D->GrpList;
      if ( Grp == NULL ) {
          return NULL;
      }
      if ( Dcount ( Grp ) < grpid ) return NULL;
      if ( grpid < 1 ) return NULL;
      Resetlink ( Grp ) ;
      Dposition ( Grp , grpid ) ;
      pt = ( WIDGETGRP * ) Getrecord ( Grp ) ;
      return pt;
  }
  int kgAddWidget ( void *tmp , void * Widget ) {
      int i;
      DIALOG *D;
      DIA *d;
      D = ( DIALOG * ) tmp;
      d = D->d;
      i = 0;
      if ( d != NULL ) while ( d [ i ] .t != NULL ) i++;
      i++;
      d = ( DIA * ) realloc ( d , sizeof ( DIA ) * ( i+1 ) ) ;
      d [ i-1 ] .t = ( DIT * ) Widget;
      d [ i ] .t = NULL;
      D->d = d;
      return 1;
  }
  int kgDeleteWidget ( void *tmp , void * Widget ) {
      int i , ret = 0 , k;
      DIALOG *D;
      DIA *d;
      D = ( DIALOG * ) tmp;
      d = D->d;
      i = 0;
      if ( d != NULL ) {
          while ( d [ i ] .t != NULL ) {
              if ( d [ i ] .t == Widget ) {
                  kgFreeWidget ( Widget ) ;
                  ret = 1;
                  k = i+1;
                  d [ i ] .t = d [ k ] .t;
                  while ( d [ k ] .t != NULL ) {d [ k ] .t = d [ k+1 ] .t;k++;}
                  break;
              }
              i++;
          }
      }
      return ret;
  }
  int kgSetExit ( void *tmp ) {
      DIALOG *D;
      if ( tmp != NULL ) {
          D = ( DIALOG * ) tmp;
          D->KILL = 1000;
          return 1;
      }
      else return 0;
  }
  void kgModifyTextWidget ( void *Tmp , int ch ) {
/*
   it should behave like a
   key event with a character 'ch'
   Useful for onscreen keyboard widget
*/
      int i , code , df;
      DIALOG *D;
      DIT *T;
      KBEVENT kbevent;
      D = ( DIALOG * ) Tmp;
      if ( D->InputWid < 0 ) return;
      i = D->InputWid;
      code = D->d [ i ] .t->code;
      kbevent.event = 5;
      kbevent.key = ch;
      switch ( code ) {
          case 't':
          df = KeyReleaseInTextBox ( ( TX_STR * ) ( D->d [ i ] .t->tstr ) , kbevent ) ;
          if ( df >= 0 ) {
              if ( _ui_readtextbox ( ( TX_STR * ) ( D->d [ i ] .t->tstr ) ) < 0 ) {
                  gprintf ( D , "Error in Text box data" ) ;
              }
              else {
                  Up_D_Tx_Table ( 0 , i , D ) ;
              }
          }
          break;
          case 'T':
          df = KeyReleaseInTableBox ( ( TX_STR * ) ( D->d [ i ] .t->tstr ) , kbevent ) ;
          if ( df != -1 ) {
              if ( _ui_readtextbox ( ( TX_STR * ) ( D->d [ i ] .t->tstr ) ) < 0 ) {
                  gprintf ( D , "Error in Table box data" ) ;
              }
              else {
                  Up_D_Table ( df , i , D ) ;
              }
          }
          break;
          default:
          break;
      }
      kgUpdateOn ( D ) ;
      return;
  }
  int kgUi ( DIALOG *D ) {
      DIA *d;
      int item;
      int bkup;
      KBEVENT kbevent;
      int TabProcess=1;
      int KEY;
      int i = 0 , oldi = 0 , n = 0 , OK = 0 , q , JMP = 0 , j = 0 , no , err = 0 , uperr , ret = 1 , new , type;
          
      int nenditem = 0 , ebutlen = 10 , row = 0 , col = 0 , click = 0 , controls = 0 , tret , hcontrols = 0;
          
      int x_menu , y_menu , x2 , y2 , XX , xb , yb;
      struct stat st;
      char * fmt;
      char temp [ 500 ] ;
      void *v;
      char *Home;
      DIALOG *Parent;
      TX_STR *txt;
      S_STR *sld;
      BRW_STR *brw;
      BUTS *but;
      char ch;
      int tempc = 1;
      kgWC *wc;
      d = D->d;
      D->tmpdir = ui_mktmpdir ( ) ;
      D->df = 0;
      if(D->NoTabProcess==1) TabProcess=0;
      pthread_mutex_init ( & ( D->Lock ) , NULL ) ;
//   uiMakeFontlist();
      uiInitFontLists(D);
      kgInitGm ( ) ;
      D->ThInfo = OpenThreads ( getCores ( ) ) ;
//   D->ThInfo = OpenThreads(1);
      if ( ( D->Newwin == 1 ) || ( D->parent == NULL ) ) {
          D->Newwin = 1;
          D->bkup = 0;
          D->name [ 299 ] = '\0';
          if ( D->Resize ) {
              if ( D->xl < D->MinWidth ) { D->MinWidth = D->xl; }
              if ( D->yl < D->MinHeight ) { D->MinHeight = D->yl; }
          }
          uiDialogWindow ( D ) ;
          D->xo = 0 , D->yo = 0;
      }
      else D->bkup = 1;
      Parent = getParentDisplay ( D ) ;
      if ( D->Newwin != 1 ) {
          wc = Parent->wc;
          D->wc = Parent->wc;
          tempc = wc->c_color;
      }
      wc = WC ( Parent ) ;
      pthread_mutex_init ( & ( wc->Rlock ) , NULL ) ;
      D->evgax = WC ( Parent )->EVGAX;
      D->evgay = WC ( Parent )->EVGAY;
      uiDefaultGuiFontSize ( D ) ;
      if ( D->SearchList == NULL ) D->SearchList = ( void * ) Dopen ( ) ;
      kgAddSearchDir ( D , "/usr/share/icons" ) ;
      kgAddSearchDir ( D , "/usr/share/icons/kulina" ) ;
      kgAddSearchDir ( D , "/usr/share/local/icons" ) ;
      kgAddSearchDir ( D , "/usr/share/local/icons/kulina" ) ;
      Home = getenv ( "HOME" ) ;
      if ( Home != NULL ) {
          strcpy ( temp , Home ) ;
          strcat ( temp , "/.kulina/Pictures" ) ;
          kgAddSearchDir ( D , temp ) ;
      }
      n = 0; while ( d [ n ] .t != NULL ) n++;
//   if(n <= 0 ) {return -1;}
      x2 = D->xo+D->xl , y2 = D->yo+D->yl;
      D->Hsize = Hsize;
      uiBkup_clip_limits ( WC ( D ) ) ;
      if ( D->StackPos == 1 ) kgRaiseWindow ( D ) ;
      again:
      uiUpdateOff ( D ) ;
      OK = 0;
      err = 0;
      if ( D->bkup == 1 ) kg_scr_back ( D->wc , D->xo-1 , D->yo-1 , D->xo+D->xl+1 , D->yo+D->yl+1 ) ;
          
      controls = uiDraw_Dialog ( D ) ;
      if ( D->Initfun != NULL ) {
          OK = ( D->Initfun ( D ) ) ;
      }
      _uiSetupGboxes ( D ) ;
      if ( controls <= 0 ) {
          if ( D->Callback == NULL ) {
              OK = 1001;
          }
          else OK = 0;
      }
      else {if ( D->Initfun == NULL ) OK = 1;else OK = 0;}
      oldi = D->df;
      D->df = -1;
      i = 0;
      if ( D->controls > 1 ) {
          i = oldi;
          ch = ( d [ i ] .t->code ) ;
          draw_key_board_attn ( i , D ) ;
          hcontrols = 0;
          j = 0;
          while ( d [ j ] .t != NULL ) {
              if ( ( d [ j ] .t->code == 'H' ) || ( d [ j ] .t->code == 'h' ) ) \
              {hcontrols++;break;}
              else j++;
          }
      }
      uiUpdateOn ( D ) ;
//   if(D->controls>0){OK=0; kbevent = kgGetEvent(Parent);} // skipping one event to clean
      if ( D->controls > 0 ) {OK = 0; kgSkipEvents ( D ) ;} // skipping one event to clean
      D->CurWid = i;
      WaitThreads ( D->ThInfo ) ;
      pthread_mutex_lock ( & ( D->Lock ) ) ;
      while ( OK < 1 ) {
          if ( D->KILL == 1000 ) {OK = 1002;break;}
          pthread_mutex_unlock ( & ( D->Lock ) ) ;
          if ( D->WaitCallback != NULL ) {
              if ( kgGetTimedEvent ( D , & kbevent ) == 0 ) {
                  OK = D->WaitCallback ( D ) ;
                  if ( OK ) OK = 1001;
                  pthread_mutex_lock ( & ( D->Lock ) ) ;
                  continue;
              }
          }
          else kbevent = kgSkipMouseMove ( Parent ) ;
          pthread_mutex_lock ( & ( D->Lock ) ) ;
          D->kb = kbevent;
          KEY = kbevent.event;
          D->PON_X = kbevent.x; D->PON_Y = kbevent.y;
          wc->eventback = wc->event;
          if ( KEY == 101 ) {
//      printf("Got 101\n");
              if ( D->ResizeCallback == NULL ) {
                  int xres , yres;
                  D->xl = kbevent.x;
                  D->yl = kbevent.y;
                  D->xl = xres;
                  D->yl = yres;
                  kgRedrawDialog ( D ) ;
              }
              else {
                  D->ResizeCallback ( D ) ;
              }
              continue;
          }
//    printf("controls: %d KEY = %d\n",controls,KEY);
          if ( KEY < 5 ) if ( ! uiCheckClickinDialog ( D ) ) {
// As on 28 Jun 2013
              if ( ( D->Callback != NULL ) ) OK = D->Callback ( D , & kbevent ) ;
              else OK = 0;
              if ( OK ) OK = 1001;
              continue;
          }
          if ( D->controls > 0 ) {
              switch ( KEY ) {
                  case 0:
                  ProcessMouseMovement ( D , kbevent , i , D->controls ) ;
                  continue;
                  case 1: //button press
                  click = check_pointer_click ( D , & n , D->xo , D->yo , d ) ;
//         printf("click= %d\n",click);
                  if ( click >= 0 ) {
                      i = click;
                      D->CurWid = i;
                      if ( ( i != oldi ) && ( D->controls > 1 ) ) {
                          uiUpdateOff ( D ) ;
                          rmv_key_board_attn ( oldi , D ) ;
                          draw_key_board_attn ( i , D ) ;
                          ch = ( d [ oldi ] .t->code ) ;
                          if ( ( ch == 't' ) || ( ( ch == 'T' ) ) ) {
                              _ui_readtextbox ( ( TX_STR * ) ( d [ oldi ] .t->tstr ) ) ;
                              D->InputWid = oldi;
                          }
                          uiUpdateOn ( D ) ;
                          oldi = i;
                      }
                      OK = ProcessMousePress ( D , kbevent , i , hcontrols , D->controls ) ;
                  }
                  else {
            // Mouse Press else where
//            printf("Got Mouse Press\n");
                      if ( ( D->Callback != NULL ) ) {
                          OK = D->Callback ( D , & kbevent ) ;
                          if ( OK ) OK = 1001;
                      }
                  }
                  break;
                  case 2:
                  continue;
                  case 3:
                  OK = ProcessMousePressDrag ( D , kbevent , i , hcontrols , D->controls ) ;
                  continue;
                  case 4:
                  continue;
                  case 5: // key release
		  /* Tab Process Supress to test Table */


#if 1
                  if ( TabProcess&&ui_Tab ( kbevent.key ) ) {
                      n = D->TotWid;
                      i = ( i+1 ) %n;
                      while ( ui_NotInputBox ( D , i ) ) i = ( i+1 ) %n;
                      D->CurWid = i;
                      if ( ( i != oldi ) && ( D->controls > 1 ) ) {
                          uiUpdateOff ( D ) ;
                          rmv_key_board_attn ( oldi , D ) ;
                          draw_key_board_attn ( i , D ) ;
                          ch = ( d [ oldi ] .t->code ) ;
                          if ( ( ch == 't' ) || ( ch == 'T' ) ) {_ui_readtextbox \
                           ( ( TX_STR * ) ( d [ oldi ] .t->tstr ) ) ;}
                          D->InputWid = oldi;
                          uiUpdateOn ( D ) ;
                          oldi = i;
                      }
                      continue;
                  }
#endif
// New code to support setting current widget for input
#if 1
                  i = D->CurWid;
                  if ( ( i != oldi ) && ( D->controls > 1 ) ) {
//                 printf("New Code\n");
                      uiUpdateOff ( D ) ;
                      rmv_key_board_attn ( oldi , D ) ;
                      draw_key_board_attn ( i , D ) ;
                      uiUpdateOn ( D ) ;
                      oldi = i;
                  }
#endif
                  OK = ProcessKeyRelease ( D , kbevent , i , hcontrols , D->controls ) ;
                  break;
              }
              if ( OK >= 1 ) { // checking for text box entries
                  n = D->TotWid;
                  for ( j = 0;j < n;j++ ) {
                      ch = ( d [ j ] .t->code ) ;
                      if ( ( ch == 't' ) || ( ch == 'T' ) ) {
                          if ( d [ j ] .t->hide != 1 ) {
                              if ( _ui_readtextbox ( ( TX_STR * ) ( d [ j ] .t->tstr ) ) < 0 ) {
                                  gprintf ( Parent , "Error in Text box data" ) ;
                                  OK = -1;
                                  break;
                              }
                          } // hide
                      }
                  }
              }
          } //if ( control ) 
          else {
              if ( ( D->Callback != NULL ) ) OK = D->Callback ( D , & kbevent ) ;
              else OK = 0;
              if ( OK ) OK = 1001;
          }
      } // While
      ret = OK;
      pthread_mutex_unlock ( & ( D->Lock ) ) ;
      pthread_mutex_destroy ( & ( D->Lock ) ) ;
      ret = uiReadinDialogValues ( D , OK ) ;
      if ( OK > 1000 ) ret = OK;
      if ( D->bkup == 1 ) {
          kg_scr_recover ( D->wc ) ;
      }
      if ( D->Newwin == 1 ) {
          kgSkipButtonRelease ( D ) ;
          kgSkipEvents ( D ) ;
      }
      uiRest_clip_limits ( WC ( D ) ) ;
      if ( err ) {
          if ( kgCheckMenu ( D , 10L , 200L , "Error: In reading data retry ?" , 1L ) ) goto again;
              
          else{
              ui_cleandir ( D->tmpdir ) ;
              kgCheckAndRemoveParent ( D->tmpdir ) ;
              normal ( ) ;
              fprintf ( stderr , "WRONG DATA ENTRY.. JOB CANCELLED\n" ) ;
              exit ( 0 ) ;
          }
      }
      uiFreeMemAlloc ( D ) ;
//      uiFreeFontLists();
      if ( D->Newwin ) {
          if ( ! WC ( D )->FullScreen ) {
              int s;
              s = pthread_cancel ( WC ( D )->Pth ) ;
              if(s!= 0) pthread_kill( WC ( D )->Pth ,SIGKILL);
              pthread_join ( WC ( D )->Pth , NULL ) ;
          }
          Dempty ( WC ( D )->Clip ) ;
          kg_clear_scrn_buffer ( WC ( D ) ) ;
//     Dempty(WC(D)->SBlist);
          Dempty ( WC ( D )->TLIST ) ;
          free ( WC ( D )->kgcolors ) ;
          kgCloseUi ( D ) ;
          if ( WC ( D )->Pstr != NULL ) free ( WC ( D )->Pstr ) ;
          if ( WC ( D )->Cstr != NULL ) free ( WC ( D )->Cstr ) ;
          free ( WC ( D ) ) ;
      }
      else wcset_clr ( wc , tempc ) ;
      ui_cleandir ( D->tmpdir ) ;
      kgCheckAndRemoveParent ( D->tmpdir ) ;
      Free ( D->tmpdir ) ;
#if 1
      CloseThreads ( D->ThInfo ) ;
#else
      KillThreads ( D->ThInfo ) ;
#endif
      D->ThInfo = NULL;
      return ( ret ) ;
  }
/* checking for string
   if found position after string is returned
*/
  int _check_for_string ( char *s , char *chk ) {
      int ls , lchk , i , k , count , ret = 0 , j , ch;
      ls = strlen ( s ) ;
      k = 0;while ( ( chk [ k ] != '\0' ) && ( chk [ k ] != '*' ) && \
       ( chk [ k ] != '?' ) ) k++;
      ch = chk [ k ] ;chk [ k ] = '\0';
      lchk = strlen ( chk ) ;
      if ( lchk > ls ) return 0;
      for ( i = 0;i < ( ls-lchk+1 ) ;i++ ) {
          count = 0;
#ifdef ZORDOS
          for ( j = 0;j < lchk;j++ ) if ( s [ i+j ] == toupper ( chk [ j ] ) ) count++;
#else
          for ( j = 0;j < lchk;j++ ) if ( s [ i+j ] == chk [ j ] ) count++;
#endif
          if ( count == lchk ) {ret = i+lchk; break;}
      }
      chk [ k ] = ch;
      return ret;
  }
  int _check_for_start_string ( char *s , char *chk ) {
      int ls , lchk , i , count , ret = 0 , j , ch;
      ls = strlen ( s ) ;
      i = 0;while ( ( chk [ i ] != '\0' ) && ( chk [ i ] != '*' ) && \
       ( chk [ i ] != '?' ) ) i++;
      ch = chk [ i ] ;chk [ i ] = '\0';
      lchk = strlen ( chk ) ;
      if ( lchk <= ls ) {
          count = 0;
          for ( j = 0;j < lchk;j++ ) if ( s [ j ] == chk [ j ] ) count++; else break;
          if ( count == lchk ) {ret = lchk; }
      }
      chk [ i ] = ch;
      return ret;
  }
  int _skip_string ( char *chk ) {
      int i = 0;
      while ( ( chk [ i ] != '\0' ) && ( chk [ i ] != '*' ) && \
       ( chk [ i ] != '?' ) ) i++;
      return i;
  }
/* checking for character
   if found position is returned
*/
  int _check_for_char ( char *s , char ch ) {
      int i = 0;
      while ( ( s [ i ] != ch ) && ( s [ i ] != '\0' ) ) i++;
      if ( s [ i ] == '\0' ) return 0;
      else return i;
  }
  int _filter_other_options ( char *s , char *fltr ) {
      int pos , skip;
      if ( fltr [ 0 ] == ' ' ) return 1;
      if ( fltr [ 0 ] == '\0' ) {
          if ( s [ 0 ] == '\0' ) return 1;
          else return 0;
      }
      if ( fltr [ 0 ] == '*' ) {
          if ( fltr [ 1 ] == '.' ) {
              pos = _check_for_char ( s , '.' ) ;
              if ( pos != 0 ) return _filter_other_options ( s+pos+1 , fltr+2 ) ;
              else return 0;
          }
          else {
              if ( fltr [ 1 ] == '\0' ) return 1;
              else if ( fltr [ 1 ] == '?' ) {
                  return _filter_other_options ( s+1 , fltr+2 ) ;
              }
              else {
                  pos = _check_for_string ( s , fltr+1 ) ;
                  if ( pos == 0 ) {
                      return 0;
                  }
                  else {
                      skip = _skip_string ( fltr+1 ) ;
                      return _filter_other_options ( s+pos , fltr+1+skip ) ;
                  }
              }
          }
      }
      else {
          if ( fltr [ 0 ] == '?' ) {
              return _filter_other_options ( s+1 , fltr+1 ) ;
          }
          else {
              pos = _check_for_start_string ( s , fltr ) ;
              if ( pos == 0 ) {
                  return 0;
              }
              else {
                  skip = _skip_string ( fltr ) ;
                  return _filter_other_options ( s+pos , fltr+skip ) ;
              }
          }
      }
  }
  int _filter_string ( char *s , char *fltr ) {
      int i , j , ret = 0;
      char *buf;
      if ( strcmp ( "*" , fltr ) == 0 ) return 1;
      if ( strcmp ( "*.*" , fltr ) == 0 ) {
          i = 0;
          while ( s [ i ] != '\0' ) if ( s [ i++ ] == '.' ) return 1;
          return 0;
      }
      else{
          buf = ( char * ) Malloc ( strlen ( fltr ) +1 ) ;
//    strcpy(buf,fltr);
          i = 0;
          while ( 1 ) {
              j = 0;
              while ( fltr [ i ] == ' ' ) i++;
              if ( fltr [ i ] < ' ' ) break;
              while ( fltr [ i ] > ' ' ) buf [ j++ ] = fltr [ i++ ] ;
              buf [ j ] = '\0';
              ret = _filter_other_options ( s , buf ) ;
              if ( ret ) break;
          }
          Free ( buf ) ;
          return ret;
      }
  }
  int chkshm ( void ) {
      char buff [ 300 ] , word1 [ 50 ] , word2 [ 50 ] , word3 [ 50 ] ;
      int ret = 0 , i;
      FILE *fp;
      fp = popen ( "mount" , "r" ) ;
      if ( fp != NULL ) {
          while ( fgets ( buff , 299 , fp ) != NULL ) {
              i = 0;
              while ( buff [ i ] == ' ' ) i++;
              if ( buff [ i ] < ' ' ) continue;
              if ( buff [ i ] == '#' ) continue;
              sscanf ( buff , "%s%s%s" , word1 , word2 , word3 ) ;
              if ( strcmp ( word3 , "/dev/shm" ) == 0 ) {ret = 1;break;}
          }
      }
      return ret;
  }
  void kgCheckAndRemoveParent ( char *dir ) {
      int ln , i , j;
      char buff [ 500 ] ;
      char **folders , **files;
      strcpy ( buff , dir ) ;
      ln = strlen ( buff ) ;
      i = ln-1;
      while ( ( i > 0 ) && ( buff [ i ] != '/' ) ) i--;
      if ( i > 0 ) {
          buff [ i ] = '\0';
          folders = _uiFolderMenu ( buff ) ;
          if ( ( folders != NULL ) && ( folders [ 0 ] != NULL ) ) {
//         printf("Folders Not empty\n");
              j = 0;
              while ( folders [ j ] != NULL ) {
//          printf("Folder: %s\n",folders[j]);
                  Free ( folders [ j ] ) ;j++;
              }
              Free ( folders ) ;
              return;
          }
          files = _uiFileMenu ( buff , "*" ) ;
          if ( ( files != NULL ) && ( files [ 0 ] != NULL ) ) {
              j = 0;
              while ( files [ j ] != NULL ) {Free ( files [ j ] ) ;j++;}
              Free ( files ) ;
              return;
          }
          rmdir ( buff ) ;
      }
  }
  void kgCleanDir ( char *folder ) {
      ui_cleandir ( folder ) ;
  }
  void ui_cleandir ( char *folder ) {
      int i , j;
      char file [ 500 ] ;
      char **folders , **files , filter [ 50 ] ;
      if ( folder == NULL ) return;
      folders = _uiFolderMenu ( folder ) ;
      if ( folders != NULL ) {
          i = 0;
          while ( folders [ i ] != NULL ) {
              strcpy ( file , folder ) ;
              strcat ( file , "/" ) ;
              strcat ( file , folders [ i ] ) ;
              ui_cleandir ( file ) ;
              i++;
          }
      }
      files = _uiFileMenu ( folder , "*" ) ;
      if ( files != NULL ) {
          j = 0;
          while ( files [ j ] != NULL ) {
              strcpy ( file , folder ) ;
              strcat ( file , "/" ) ;
              strcat ( file , files [ j ] ) ;
              remove ( file ) ;
              Free ( files [ j ] ) ;
              j++;
          }
          Free ( files ) ;
      }
#if 0
      files = _uiFileMenu ( folder , ".*" ) ;
      if ( files != NULL ) {
          j = 0;
          while ( files [ j ] != NULL ) {
              strcpy ( file , folder ) ;
              strcat ( file , "/" ) ;
              strcat ( file , files [ j ] ) ;
              remove ( file ) ;
              Free ( files [ j ] ) ;
              j++;
          }
          Free ( files ) ;
      }
#endif
      rmdir ( folder ) ;
  }
  void ui_cleantmpdir ( void ) {
      int pid , i , j;
      char dir [ 100 ] , *pt , file [ 200 ] ;
      char **folders , **files , filter [ 50 ] ;
      pid = getpid ( ) ;
      sprintf ( filter , "%-d_*" , pid ) ;
      if ( chkshm ( ) ) {
          folders = _uiFolderMenu ( "/dev/shm" ) ;
          if ( folders == NULL ) return;
          i = 0;
          while ( folders [ i ] != NULL ) {
              if ( _filter_string ( folders [ i ] , filter ) ) {
                  strcpy ( dir , "/dev/shm/" ) ;
                  strcat ( dir , folders [ i ] ) ;
                  files = _uiFileMenu ( dir , "*" ) ;
                  if ( files != NULL ) {
                      j = 0;
                      while ( files [ j ] != NULL ) {
                          strcpy ( file , dir ) ;
                          strcat ( file , "/" ) ;
                          strcat ( file , files [ j ] ) ;
                          remove ( file ) ;
                          Free ( files [ j ] ) ;
                          j++;
                      }
                      Free ( files ) ;
                  }
                  rmdir ( dir ) ;
              }
              Free ( folders [ i ] ) ;
              i++;
          }
          Free ( folders ) ;
      }
      else {
          folders = _uiFolderMenu ( "/tmp" ) ;
          if ( folders == NULL ) return;
          i = 0;
          while ( folders [ i ] != NULL ) {
              if ( _filter_string ( folders [ i ] , filter ) ) {
                  strcpy ( dir , "/tmp/" ) ;
                  strcat ( dir , folders [ i ] ) ;
                  files = _uiFileMenu ( dir , "*" ) ;
                  if ( files != NULL ) {
                      j = 0;
                      while ( files [ j ] != NULL ) {
                          strcpy ( file , dir ) ;
                          strcat ( file , "/" ) ;
                          strcat ( file , files [ j ] ) ;
                          remove ( file ) ;
                          Free ( files [ j ] ) ;
                          j++;
                      }
                      Free ( files ) ;
                  }
                  rmdir ( dir ) ;
              }
              Free ( folders [ i ] ) ;
              i++;
          }
          Free ( folders ) ;
      }
  }
  int kgWait ( int secs ) {
// One Need to Check
      struct timespec timeToWait;
      struct timeval now;
      int rt;
      gettimeofday ( & now , NULL ) ;
      timeToWait.tv_sec = now.tv_sec + secs;
      timeToWait.tv_nsec = now.tv_usec*1000;
      pthread_mutex_lock ( & fakeMutex ) ;
      rt = pthread_cond_timedwait ( & fakeCond , & fakeMutex , & timeToWait ) ;
      pthread_mutex_unlock ( & fakeMutex ) ;
//  printf("Wait Over\n");
  }
  void kgThreadSleep ( int secs , int usecs ) {
      int retval;
      struct timeval tv1;
      fd_set rfds;
      FD_ZERO ( & rfds ) ;
      FD_SET ( 0 , & rfds ) ;
      tv1.tv_sec = secs;
      tv1.tv_usec = usecs;
      retval = select ( 0 , & rfds , NULL , NULL , & tv1 ) ;
      return ;
  }
  int kgThreadWaitPipe ( int pipe , int secs , int usecs ) {
/* waits for read ready on pipe or time out; if ready 1 char is read from pipe*/
      char buf [ 2 ] ;
      int retval;
      int rval;
      struct timeval tv1;
      fd_set rfds;
      FD_ZERO ( & rfds ) ;
      FD_SET ( pipe , & rfds ) ;
      tv1.tv_sec = secs;
      tv1.tv_usec = usecs;
      retval = select ( pipe+1 , & rfds , NULL , NULL , & tv1 ) ;
      if ( retval >= 1 ) rval = read ( pipe , buf , 1 ) ;
      else retval = 0;
      return retval;
  }
  char * ui_mktmpdir_o ( void ) {
      static int pid = 0;
      static char dir [ 200 ] ;
      DIR *dp;
      char dirname [ 500 ] ;
      char *pt;
      static int entry = 0;
      pthread_mutex_lock ( & _Tmplock ) ;
      if ( entry == 0 ) {
//    printf ("entry=0\n");
          pid = getpid ( ) ;
#if 0
          if ( chkshm ( ) ) {
//    printf("tmpfs mounted on /dev/shm\n");
              sprintf ( dir , "/dev/shm/%-d" , pid ) ;
              mkdir ( dir , 0700 ) ;
          }
//    else {
#else
          {
#endif
//    printf("tmpfs NOT  mounted on /dev/shm\n");
              sprintf ( dir , "/tmp/%-d" , pid ) ;
              mkdir ( dir , 0700 ) ;
          }
//    pt = (char *)malloc(strlen(dir)+1);
//    strcpy(pt,dir);
//TCB testing
//    kgSetCleantmpdir(dir);
      }
//  else {
      {
          dp = opendir ( dir ) ;
          if ( dp == NULL ) {
              mkdir ( dir , 0700 ) ;
          }
          else closedir ( dp ) ;
          sprintf ( dirname , "%-s/%-d" , dir , entry ) ;
          mkdir ( dirname , 0700 ) ;
          pt = ( char * ) Malloc ( strlen ( dirname ) +1 ) ;
          strcpy ( pt , dirname ) ;
      }
      entry++;
//  printf("Tmpdir:%s\n",pt);
      pthread_mutex_unlock ( & _Tmplock ) ;
      return pt;
  }
  char * ui_mktmpdir ( void ) {
      static int pid = 0;
      static char dir [ 200 ] ;
      DIR *dp;
      char dirname [ 500 ] ;
      char *pt;
      static int entry = 0;
      pthread_mutex_lock ( & _Tmplock ) ;
      pid = getpid ( ) ;
      sprintf ( dir , "/tmp/%-d" , pid ) ;
      sprintf ( dirname , "%-s_%-d" , dir , entry ) ;
      dp = opendir ( dirname ) ;
      if ( dp == NULL ) {
          mkdir ( dirname , 0700 ) ;
      }
      else closedir ( dp ) ;
      pt = ( char * ) Malloc ( strlen ( dirname ) +1 ) ;
      strcpy ( pt , dirname ) ;
      entry++;
//  printf("Tmpdir:%s\n",pt);
      pthread_mutex_unlock ( & _Tmplock ) ;
      return pt;
  }
  char *kgMakeTmpDir ( void ) {
      return ui_mktmpdir ( ) ;
  }
  int Gprintf ( DIALOG *D , void *unknown , ... ) {
      union {
          int l;
          int i;
          char c;
          float f;
          double d;
          char *s;
      } value;
      int i , item = 0 , j;
      char *tmp , *pt , ch , *cpt;
      char wrk [ 300 ] , buff [ 100 ] , buf [ 300 ] ;
      va_list ad;
      wrk [ 0 ] = '\0';
      va_start ( ad , unknown ) ;
      tmp = va_arg ( ad , char * ) ;
      strcpy ( buf , tmp ) ;
      cpt = buf;
      pt = cpt;
      while ( *cpt != '\0' ) {
          if ( ( *cpt == '%' ) && ( * ( cpt+1 ) != '%' ) ) {
              cpt++;
              while ( isdigit ( *cpt ) || ( *cpt == '.' ) || ( *cpt == '-' ) ) { cpt++; }
              switch ( ( int ) *cpt ) {
                  case 'c':
                  while ( ( *cpt != '\0' ) && ( *cpt != '%' ) ) cpt++;
                  ch = *cpt;*cpt = '\0';
       /*value.c= va_arg(ad,char );*/ /* 14-2-2001 */
                  value.c = va_arg ( ad , int ) ; /* 14-2-2001 */
                  sprintf ( buff , pt , value.c ) ;
                  strcat ( wrk , buff ) ;
                  *cpt = ch;
                  break;
                  case 's':
                  while ( ( *cpt != '\0' ) && ( *cpt != '%' ) ) cpt++;
                  ch = *cpt;*cpt = '\0';
                  value.s = va_arg ( ad , char * ) ;
                  sprintf ( buff , pt , value.s ) ;
                  strcat ( wrk , buff ) ;
                  *cpt = ch;
                  break;
                  case 'f':
                  case 'g':
                  case 'e':
                  while ( ( *cpt != '\0' ) && ( *cpt != '%' ) ) cpt++;
                  ch = *cpt;*cpt = '\0';
                  value.f = va_arg ( ad , double ) ;
                  sprintf ( buff , pt , value.f ) ;
                  strcat ( wrk , buff ) ;
                  *cpt = ch;
                  break;
                  case 'd':
                  while ( ( *cpt != '\0' ) && ( *cpt != '%' ) ) cpt++;
                  ch = *cpt;*cpt = '\0';
                  value.i = va_arg ( ad , int ) ;
                  sprintf ( buff , pt , value.i ) ;
                  strcat ( wrk , buff ) ;
                  *cpt = ch;
                  break;
                  case 'l':
                  cpt++;
                  switch ( ( int ) *cpt ) {
                      case 'f':
                      case 'g':
                      case 'e':
                      while ( ( *cpt != '\0' ) && ( *cpt != '%' ) ) cpt++;
                      ch = *cpt;*cpt = '\0';
                      value.d = va_arg ( ad , double ) ;
                      sprintf ( buff , pt , value.d ) ;
                      strcat ( wrk , buff ) ;
                      *cpt = ch;
                      break;
                      case 'd':
                      while ( ( *cpt != '\0' ) && ( *cpt != '%' ) ) cpt++;
                      ch = *cpt;*cpt = '\0';
                      value.l = va_arg ( ad , int ) ;
                      sprintf ( buff , pt , value.l ) ;
                      strcat ( wrk , buff ) ;
                      *cpt = ch;
                      break;
                      default :
                      normal ( ) ;
                      printf ( "Error: In Gprintf/  %s |%c|/\n" , pt , *cpt ) ;
                      exit ( 0 ) ;
                  }
                  break;
                  default :
                  normal ( ) ;
                  printf ( "Error: In Gprintf/  %s |%c|/\n" , pt , *cpt ) ;
                  exit ( 0 ) ;
              }
              pt = cpt;
          }
          else cpt++;
      }
      if ( pt != NULL ) strcat ( wrk , pt ) ;
      gprintf ( D , wrk ) ;
  }
  static int getitems ( char ** items ) {
      int n = 0;
      while ( items [ n ] != NULL ) n++;
      return n;
  }
  static int getitemsize ( char ** items ) {
      int n = 0 , l = 0 , size = 0;
      while ( items [ n ] != NULL ) {
          l = strlen ( items [ n ] ) ;
          if ( l > size ) size = l;
          n++;
      }
      size = size*9;
      return size;
  }
/**************************************************************
  New ones added on 16th Jan 2007 
  tmp is a pointer to DIALOG
  tb  is the index to text box
  item is the item number in the text box
  
**************************************************************/
  float Dgetfloat ( void *Tmp , int tb , int item ) {
      float f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = * ( ( float * ) ( e [ item ] .v ) ) ;
      return f;
  }
  float Dsetfloat ( void *Tmp , int tb , int item , float val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      e = T->elmt;
      * ( ( float * ) ( e [ item ] .v ) ) = val;
      kgUpdateWidget ( T ) ;
      return val;
  }
  double Dgetdouble ( void *Tmp , int tb , int item ) {
      double f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = * ( ( double * ) ( e [ item ] .v ) ) ;
      return f;
  }
  double Dsetdouble ( void *Tmp , int tb , int item , double val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      e = T->elmt;
      * ( ( double * ) ( e [ item ] .v ) ) = val;
      kgUpdateWidget ( T ) ;
      return val;
  }
  int Dgetint ( void *Tmp , int tb , int item ) {
      int f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = * ( ( int * ) ( e [ item ] .v ) ) ;
      return f;
  }
  int Dsetint ( void *Tmp , int tb , int item , int val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      e = T->elmt;
      * ( ( int * ) ( e [ item ] .v ) ) = val;
      kgUpdateWidget ( T ) ;
      return val;
  }
  long Dgetlong ( void *Tmp , int tb , int item ) {
      long f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = * ( ( long * ) ( e [ item ] .v ) ) ;
      return f;
  }
  long Dsetlong ( void *Tmp , int tb , int item , long val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      e = T->elmt;
      * ( ( long * ) ( e [ item ] .v ) ) = val;
      kgUpdateWidget ( T ) ;
      return val;
  }
  char * Dgetstring ( void *Tmp , int tb , int item ) {
      char * f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = ( ( char * ) ( e [ item ] .v ) ) ;
      return f;
  }
  char * Dsetstring ( void *Tmp , int tb , int item , char * val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      e = T->elmt;
      strcpy ( ( ( char * ) ( e [ item ] .v ) ) , val ) ;
#if 0
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      Ti = Tstr->tit;
      _ui_updatetextbox ( Tstr ) ;
#else
      kgUpdateWidget ( T ) ;
#endif
      return val;
  }
  float kgGetfloat ( void *Tmp , int tb , int item ) {
      float f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = * ( ( float * ) ( e [ item ] .v ) ) ;
      return f;
  }
  float kgSetfloat ( void *Tmp , int tb , int item , float val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      e = T->elmt;
      * ( ( float * ) ( e [ item ] .v ) ) = val;
//  kgUpdateWidget(T); // application should do this
      return val;
  }
  double kgGetdouble ( void *Tmp , int tb , int item ) {
      double f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = * ( ( double * ) ( e [ item ] .v ) ) ;
      return f;
  }
  double kgSetdouble ( void *Tmp , int tb , int item , double val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      e = T->elmt;
      * ( ( double * ) ( e [ item ] .v ) ) = val;
//  kgUpdateWidget(T); // application should do this
      return val;
  }
  int kgGetint ( void *Tmp , int tb , int item ) {
      int f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = * ( ( int * ) ( e [ item ] .v ) ) ;
      return f;
  }
  int kgSetint ( void *Tmp , int tb , int item , int val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      e = T->elmt;
      * ( ( int * ) ( e [ item ] .v ) ) = val;
//  kgUpdateWidget(T); // application should do this
      return val;
  }
  long kgGetlong ( void *Tmp , int tb , int item ) {
      long f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = * ( ( long * ) ( e [ item ] .v ) ) ;
      return f;
  }
  long kgSetlong ( void *Tmp , int tb , int item , long val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      e = T->elmt;
      * ( ( long * ) ( e [ item ] .v ) ) = val;
//  kgUpdateWidget(T); // application should do this
      return val;
  }
  char * kgGetstring ( void *Tmp , int tb , int item ) {
      char * f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = ( ( char * ) ( e [ item ] .v ) ) ;
      return f;
  }
  char * kgSetstring ( void *Tmp , int tb , int item , char * val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      D = ( ( DIALOG * ) Tmp )->d;
      T = D [ tb ] .t;
      e = T->elmt;
      strcpy ( ( ( char * ) ( e [ item ] .v ) ) , val ) ;
//  kgUpdateWidget(T);  // application should do this
      return val;
  }
  float kgGetFloat ( void *Tmp , int item ) {
      float f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      T = ( DIT * ) Tmp;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = * ( ( float * ) ( e [ item ] .v ) ) ;
      return f;
  }
  float kgSetFloat ( void *Tmp , int item , float val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      T = ( DIT * ) Tmp;
      e = T->elmt;
      * ( ( float * ) ( e [ item ] .v ) ) = val;
//  kgUpdateWidget(T); // application should do this
      return val;
  }
  double kgGetDouble ( void *Tmp , int item ) {
      double f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      T = ( DIT * ) Tmp;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = * ( ( double * ) ( e [ item ] .v ) ) ;
      return f;
  }
  double kgSetDouble ( void *Tmp , int item , double val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      T = ( DIT * ) Tmp;
      e = T->elmt;
      * ( ( double * ) ( e [ item ] .v ) ) = val;
//  kgUpdateWidget(T); // application should do this
      return val;
  }
  int kgGetInt ( void *Tmp , int item ) {
      int f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      T = ( DIT * ) Tmp;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = * ( ( int * ) ( e [ item ] .v ) ) ;
      return f;
  }
  int kgSetInt ( void *Tmp , int item , int val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      T = ( DIT * ) Tmp;
      e = T->elmt;
      * ( ( int * ) ( e [ item ] .v ) ) = val;
//  kgUpdateWidget(T); // application should do this
      return val;
  }
  long kgGetLong ( void *Tmp , int item ) {
      long f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      T = ( DIT * ) Tmp;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
      _ui_readtextbox ( Tstr ) ;
      e = T->elmt;
      f = * ( ( long * ) ( e [ item ] .v ) ) ;
      return f;
  }
  long kgSetLong ( void *Tmp , int item , long val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      T = ( DIT * ) Tmp;
      e = T->elmt;
      * ( ( long * ) ( e [ item ] .v ) ) = val;
//  kgUpdateWidget(T); // application should do this
      return val;
  }
  char * kgGetString ( void *Tmp , int item ) {
      char * f;
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      T = ( DIT * ) Tmp;
      Tstr = ( TX_STR * ) ( T->tstr ) ;
 //     _ui_readtextbox ( Tstr ) ;
      _ui_readtextboxcell ( Tstr,item ) ;
      e = T->elmt;
      f = ( ( char * ) ( e [ item ] .v ) ) ;
      return f;
  }
  char * kgSetString ( void *Tmp , int item , char * val ) {
      TX_STR *Tstr;
      DIA *D;DIT *T;T_ELMT *e;
      T = ( DIT * ) Tmp;
      e = T->elmt;
      strcpy ( ( ( char * ) ( e [ item ] .v ) ) , val ) ;
      e[item].startchar =0;
      e[item].cursor =0;
//  kgUpdateWidget(T);  // application should do this
      return val;
  }
  int Dprintf ( void *Tmp , int infob , char *str ) {
      DIA *D;DII *I;
      D = ( ( DIALOG * ) Tmp )->d;
      I = D [ infob ] .i;
      uiinfo_wprintf ( ( DIALOG * ) Tmp , I->twin , str ) ;
      return 1;
  }
  int kgPrintf ( void *Tmp , int infob , char *str ) {
      DIA *D;DII *I;
      D = ( ( DIALOG * ) Tmp )->d;
      I = D [ infob ] .i;
      if ( I->code == 'i' ) {
          if ( I->hide == 1 ) return 0;
          uiinfo_wprintf ( ( DIALOG * ) Tmp , I->twin , str ) ;
          return 1;
      }
      else return kgSplash ( Tmp , infob , str ) ;
  }
/* Added on 31 July 2012 */
  char *Dgetmenustring ( void *Tmp , int menu ) {
      DIA *D;DIE *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .e;
      B = ( BRW_STR * ) ( E->bwsr ) ;
      return E->menu [ B->df-1 ] ;
  }
  int Dgetmenuitem ( void *Tmp , int menu ) {
      DIA *D;DIE *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .e;
      B = ( BRW_STR * ) ( E->bwsr ) ;
      return B->df;
  }
  int Dsetmenuitem ( void *Tmp , int menu , int df ) {
      DIA *D;DIE *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .e;
      B = ( BRW_STR * ) ( E->bwsr ) ;
      B->df = df;
      return B->df;
  }
  char **Dgetmenu ( void *Tmp , int menu ) {
      DIA *D;DIE *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .e;
      B = ( BRW_STR * ) ( E->bwsr ) ;
      return B->menu;
  }
  void Dsetmenu ( void *Tmp , int menu , char **list ) {
      DIA *D;DIE *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .e;
      B = ( BRW_STR * ) ( E->bwsr ) ;
      B->menu = list;
      E->menu = list;
  }
// TCB not finished
  void Dsetmsgscroll ( void *Tmp , int menu , char **list ) {
      DIA *D;DIS *S;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      S = D [ menu ] .s;
      S->menu = list;
      uiUpdateMsgScroll ( Tmp , menu ) ;
  }
  char **Dgetmsgscroll ( void *Tmp , int menu ) {
      DIA *D;DIS *S;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      S = D [ menu ] .s;
      B = ( BRW_STR * ) ( S->bwsr ) ;
      return B->menu;
  }
  void Dsetscrollmenu ( void *Tmp , int menu , char **list ) {
#if 0
      DIA *D;DIE *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .e;
      E->menu = list;
#else
      ui_updatemenu ( Tmp , menu , list ) ;
#endif
  }
  char **Dgetscrollmenu ( void *Tmp , int menu ) {
      DIA *D;DIE *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .e;
      B = ( BRW_STR * ) ( E->bwsr ) ;
      return E->menu;
  }
  void Dsetpixmap ( void *Tmp , int menu , void *img ) {
      DIA *D;DIP *E;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .p;
      E->xpm = img;
      _uiDrawPicture ( Tmp , menu ) ;
  }
  void Dfreepixmap ( void *Tmp , int menu ) {
      char *cpt;
      DIA *D;DIP *E;
      JPGIMG *img;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .p;
      cpt = ( char * ) E->xpm;
      if ( cpt == NULL ) return;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) return;
      img = ( JPGIMG * ) E->xpm;
      uiFreeImage ( img ) ;
  }
  void kgChangePixmapImage ( void *Wtmp , void *img ) {
      DIP *p;
      p = ( DIP * ) Wtmp;
      p->xpm = img;
  }
  void kgFreePixmapImage ( void *Wtmp ) {
      DIP *p;
      char *cpt;
      p = ( DIP * ) Wtmp;
      cpt = ( char * ) p->xpm;
      if ( cpt == NULL ) return;
      if ( ( cpt [ 0 ] == '#' ) && ( cpt [ 1 ] == '#' ) ) return;
      uiFreeImage ( p->xpm ) ;
      p->xpm = NULL;
  }
#if 0
#endif
  void uiGetViewport ( void *Tmp , int no , float v [ ] ) {
      DIA *D;DIG *G;
      D = ( ( DIALOG * ) Tmp )->d;
      G = D [ no ] .g;
      v [ 0 ] = G->vxmin;
      v [ 1 ] = G->vymin;
      v [ 2 ] = G->vxmax;
      v [ 3 ] = G->vymax;
      return ;
  }
  int Dsplash ( void *Tmp , int item , char *msg ) {
/* Application must alloc and free memory for msg */
      DIA *D;
      DIM *m;
      D = ( ( DIALOG * ) Tmp )->d;
      m = D [ item ] .m;
//  m->msg = msg;
//  _uiDrawMessage(Tmp,item);
      strncpy ( m->msg , msg , 499 ) ;
      _uiMake_M ( m ) ;
      return 1;
  }
  int kgSplash ( void *Tmp , int item , char *msg ) {
/* Application must alloc and free memory for msg */
      DIA *D;
      DIM *m;
      D = ( ( DIALOG * ) Tmp )->d;
      m = D [ item ] .m;
//  m->msg = msg;
      if ( m->hide == 1 ) return 0;
      if ( ( m->code == 'M' ) || ( m->code == 'm' ) || ( m->code == 'B' ) ) {
          strncpy ( m->msg , msg , 499 ) ;
          _uiMake_M ( m ) ;
      }
      else printf ( "Wrong Box for Splash message\n" ) ;
      return 1;
  }
  int kgWrite ( void *Tmp , char *msg ) {
/* Application must alloc and free memory for msg */
      DIA *D;
      DIM *m;
      DII *I;
      m = ( DIM * ) Tmp;
      switch ( m->code ) {
          case 'M':
          case 'm':
          case 'B':
          if ( m->hide == 1 ) return 0;
          strncpy ( m->msg , msg , 499 ) ;
          kgUpdateWidget ( m ) ;
          kgUpdateOn ( m->D ) ;
          break;
          case 's':
          {
              Dlink *L;
              DIS *s;
              int i = 0;
              char *pt , **menu;
              s = ( DIS * ) m;
              L = Dopen ( ) ;
              menu = s->menu;
              if ( menu != NULL ) {
                  i = 0;
                  while ( ( pt = menu [ i++ ] ) != NULL ) Dadd ( L , pt ) ;
                  free ( menu ) ;
              }
              pt = ( char * ) malloc ( strlen ( msg ) +1 ) ;
              strcpy ( pt , msg ) ;
              Dappend ( L , pt ) ;
              Resetlink ( L ) ;
              menu = ( char ** ) malloc ( sizeof ( char * ) * ( Dcount ( L ) +1 ) ) ;
              i = 0;
              Resetlink ( L ) ;
              while ( ( pt = ( char * ) Getrecord ( L ) ) != NULL ) {
                  menu [ i ] = pt;i++;
              }
              menu [ i ] = NULL;
              s->menu = menu;
              Dfree ( L ) ;
              kgUpdateWidget ( s ) ;
              kgUpdateOn ( s->D ) ;
          }
          break;
          case 'i':
          I = ( DII * ) Tmp;
          if ( I->hide == 1 ) return 0;
          uiinfo_wprintf ( ( DIALOG * ) I->D , I->twin , msg ) ;
          kgUpdateOn ( I->D ) ;
          break;
          default:
          fprintf ( stderr , "Wrong Widget for kgWrite\n" ) ;
          break;
      }
      return 1;
  }
  int kgGetSelection ( void *tmp ) {
      DIA *D;DIX *X;DIW *W;DIE *E;
      int df = 0;
      X = ( DIX * ) tmp;
      switch ( X->code ) {
          case 'x':
          case 'y':
          case 'r':
          case 'c':
          df = * ( X->df ) ;
          return df;
          case 'e':
          E = ( DIE * ) X;
          df = * ( E->df ) ;
          return df;
          case 'w':
          W = ( DIW * ) X;
          df = * ( W->df ) ;
          return df;
          default:
          fprintf ( stderr , "Invalid Widget: kgGetSelection\n" ) ;
          return df;
      }
  }
  int kgSetSelection ( void *tmp , int val ) {
      DIA *D;DIX *X;DIW *W;DIE *E;
      int df = 0;
      X = ( DIX * ) tmp;
      switch ( X->code ) {
          case 'x':
          case 'y':
          case 'r':
          case 'c':
          * ( X->df ) = val;;
          df = * ( X->df ) ;
          return df;
          case 'e':
          E = ( DIE * ) X;
          * ( E->df ) = val;
          df = * ( E->df ) ;
          return df;
          case 'w':
          W = ( DIW * ) X;
          * ( W->df ) = val;
          df = * ( W->df ) ;
          return df;
          default:
          fprintf ( stderr , "Invalid Widget: kgSetSelection\n" ) ;
          return df;
      }
  }
  char *Dgetselectmenustring ( void *Tmp , int menu ) {
      DIA *D;DIX *E;
      BRW_STR *B;
      ThumbNail **list;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .x;
      B = ( BRW_STR * ) ( E->bwsr ) ;
      list = ( ThumbNail ** ) E->list;
      return list [ B->df-1 ]->name;
  }
  char *kgGetSelectedString ( void *Tmp ) {
      DIA *D;DIX *X;DIW *W;DIE *E;
      BRW_STR *B;
      ThumbNail **list;
      char **m;
      int df;
      X = ( DIX * ) Tmp;
      switch ( X->code ) {
          case 'x':
          case 'y':
          case 'r':
          case 'c':
          df = * ( X->df ) -1;
          list = ( ThumbNail ** ) X->list;
          if ( ( list != NULL ) && ( list [ 0 ] != NULL ) ) {
              return list [ df ]->name;
          }
          else return NULL;
          case 'e':
          E = ( DIE * ) X;
          df = * ( E->df ) -1;
          m = ( char ** ) ( ( DIE * ) E )->menu;
          if ( ( m != NULL ) && ( m [ 0 ] != NULL ) ) {
              return m [ df ] ;
          }
          else return NULL;
          case 'w':
          W = ( DIW * ) X;
          df = * ( W->df ) -1;
          m = ( char ** ) W->menu;
          if ( ( m != NULL ) && ( m [ 0 ] != NULL ) ) {
              return m [ df ] ;
          }
          else return NULL;
          default:
          fprintf ( stderr , "Invalid Widget: kgGetSelectedString\n" ) ;
          return NULL;
      }
  }
  void **kgGetList ( void *Tmp ) {
      DIA *D;DIX *X;DIW *W;DIE *E;
      BRW_STR *B;
      ThumbNail **list;
      char **m;
      int df;
      X = ( DIX * ) Tmp;
      switch ( X->code ) {
          case 'x':
          case 'y':
          case 'r':
          case 'c':
          df = * ( X->df ) -1;
          list = ( ThumbNail ** ) X->list;
          return ( void ** ) list;
          case 'e':
          E = ( DIE * ) X;
          df = * ( E->df ) -1;
          m = ( char ** ) ( ( DIE * ) E )->menu;
          return ( void ** ) m;
          case 'w':
          W = ( DIW * ) X;
          df = * ( W->df ) -1;
          m = ( char ** ) W->menu;
          return ( void ** ) m;
          default:
          fprintf ( stderr , "Invalid Widget: kgGetList\n" ) ;
          return NULL;
      }
  }
  void **kgSetList ( void *Tmp , void **list ) {
      DIA *D;DIX *X;DIW *W;DIE *E;
      BRW_STR *B;
      char **m;
      int df , nitems = 0;
      X = ( DIX * ) Tmp;
      if ( list != NULL ) {
          while ( list [ nitems ] != NULL ) nitems++;
      }
      switch ( X->code ) {
          case 'x':
          case 'c':
          X->list = list;
          X->nitems = nitems;
          * ( X->df ) = 0;
          uiCleanXImages ( X ) ;
          return ( void ** ) list;
          case 'r':
          X->list = list;
          X->nitems = nitems;
          uiCleanRImages ( ( DIRA * ) X ) ;
          return ( void ** ) list;
          case 'y':
          X->list = list;
          X->nitems = nitems;
          * ( X->df ) = 0;
          uiCleanYImages ( ( DIY * ) X ) ;
          return ( void ** ) list;
          case 'e':
          E = ( DIE * ) X;
          uiCleanEbrowserImages ( E ) ;
          E->menu = ( char ** ) list;
          return ( void ** ) list;
          case 'w':
          W = ( DIW * ) X;
          uiCleanBrowserImages ( W ) ;
          W->menu = ( char ** ) list;
          return ( void ** ) list;
          default:
          fprintf ( stderr , "Invalid Widget: kgSetList\n" ) ;
          return NULL;
      }
  }
  void *kgGetThumbNail ( void *wid , int item ) {
      DIX *X;
      int nitems = 0;
      X = ( DIX * ) wid;
      ThumbNail **list;
      switch ( X->code ) {
          case 'x':
          case 'y':
          case 'r':
          case 'c':
          list = ( ThumbNail ** ) X->list;
          if ( list != NULL ) {
              while ( list [ nitems ] != NULL ) nitems++;
          }
          else return NULL;
          if ( item >= nitems ) return NULL;
          return list [ item ] ;
          break;
          default:
          break;
      }
      return NULL;
  }
  char *kgGetThumbNailName ( void *wid , int item ) {
      DIX *X;
      int nitems = 0;
      X = ( DIX * ) wid;
      ThumbNail **list;
      switch ( X->code ) {
          case 'x':
          case 'y':
          case 'r':
          case 'c':
          list = ( ThumbNail ** ) X->list;
          if ( list != NULL ) {
              while ( list [ nitems ] != NULL ) nitems++;
          }
          else return NULL;
          if ( item >= nitems ) return NULL;
          return list [ item ]->name;
          break;
          default:
          break;
      }
      return NULL;
  }
  void *kgGetThumbNailImage ( void *wid , int item ) {
      DIX *X;
      X = ( DIX * ) wid;
      ThumbNail **list;
      switch ( X->code ) {
          case 'x':
          case 'y':
          case 'r':
          case 'c':
          int nitems = 0;
          list = ( ThumbNail ** ) X->list;
          if ( list != NULL ) {
              while ( list [ nitems ] != NULL ) nitems++;
          }
          else return NULL;
          if ( item >= nitems ) return NULL;
          return list [ item ]->img;
          break;
          default:
          break;
      }
      return NULL;
  }
  void *kgSetThumbNailImage ( void *wid , int item , void *img ) {
      DIX *X;
      X = ( DIX * ) wid;
      ThumbNail **list;
      switch ( X->code ) {
          case 'x':
          case 'y':
          case 'r':
          case 'c':
          int nitems = 0;
          list = ( ThumbNail ** ) X->list;
          if ( list != NULL ) {
              while ( list [ nitems ] != NULL ) nitems++;
          }
          else return NULL;
          if ( item >= nitems ) return NULL;
          list [ item ]->img = img;
          return img;
          break;
          default:
          break;
      }
      return NULL;
  }
  void kgSetThumbNailName ( void *wid , int item , char *name ) {
      DIX *X;
      X = ( DIX * ) wid;
      ThumbNail **list;
      switch ( X->code ) {
          case 'x':
          case 'y':
          case 'r':
          case 'c':
          int nitems = 0;
          list = ( ThumbNail ** ) X->list;
          if ( list != NULL ) {
              while ( list [ nitems ] != NULL ) nitems++;
          }
          else return ;
          if ( item >= nitems ) return;
          list [ item ]->name = ( char * ) malloc ( strlen ( name ) +1 ) ;
          strcpy ( list [ item ]->name , name ) ;
          return ;
          break;
          default:
          break;
      }
      return ;
  }
  void *kgCopyThumbNail ( void *tmp ) {
      ThumbNail *thret = NULL , *th;
      th = ( ThumbNail * ) tmp;
      if ( th == NULL ) return NULL;
      thret = ( ThumbNail * ) malloc ( sizeof ( ThumbNail ) ) ;
      thret->name = NULL;
      thret->img = NULL;
      thret->sw = th->sw;
      thret->id = th->id;
      thret->state = th->state;
      if ( th->name != NULL ) {
          thret->name = ( char * ) malloc ( strlen ( th->name ) +1 ) ;
          strcpy ( thret->name , th->name ) ;
      }
      if ( th->img != NULL ) {
          thret->img = kgCopyImage ( th->img ) ;
      }
      return ( void * ) thret;
  }
  int kgInsertThumbNail ( void *Wid , void *th , int pos ) {
      Dlink *L = NULL;
      ThumbNail **TH;
      void *pt;
      int i , n;
      TH = ( ThumbNail ** ) kgGetList ( Wid ) ;
      L = Dopen ( ) ;
      if ( TH == NULL ) Dadd ( L , th ) ;
      else {
          i = 0;
          while ( TH [ i ] != NULL ) {Dadd ( L , TH [ i ] ) ;i++;}
          if ( ( pos >= i ) || ( pos < 0 ) ) Dappend ( L , th ) ;
          else {
              Dposition ( L , pos+1 ) ;
              Dinsert ( L , th ) ;
          }
      }
      n = Dcount ( L ) +1;
      free ( TH ) ;
      TH = ( ThumbNail ** ) malloc ( sizeof ( ThumbNail * ) *n ) ;
      Resetlink ( L ) ;
      i = 0;
      while ( ( pt = Getrecord ( L ) ) != NULL ) {TH [ i ] = pt;i++;}
      TH [ i ] = NULL;
      kgSetList ( Wid , ( void ** ) TH ) ;
      Dfree ( L ) ;
      return 1;
  }
  int kgAddThumbNail ( void *Wid , void *th , int pos ) {
      Dlink *L = NULL;
      ThumbNail **TH;
      void *pt;
      int i = 0 , n;
      TH = ( ThumbNail ** ) kgGetList ( Wid ) ;
      L = Dopen ( ) ;
      if ( TH == NULL ) Dadd ( L , th ) ;
      else {
          i = 0;
          while ( TH [ i ] != NULL ) {Dadd ( L , TH [ i ] ) ;i++;}
          if ( ( pos >= i ) || ( pos < 0 ) ) Dappend ( L , th ) ;
          else {
              Dposition ( L , pos+1 ) ;
              Dadd ( L , th ) ;
          }
      }
      n = Dcount ( L ) +1;
      if ( TH != NULL ) free ( TH ) ;
      TH = ( ThumbNail ** ) malloc ( sizeof ( ThumbNail * ) *n ) ;
      Resetlink ( L ) ;
      i = 0;
      while ( ( pt = Getrecord ( L ) ) != NULL ) {TH [ i ] = pt;i++;}
      TH [ i ] = NULL;
      kgSetList ( Wid , ( void ** ) TH ) ;
      Dfree ( L ) ;
      return 1;
  }
  int kgDeleteThumbNail ( void *Wid , int pos ) {
      Dlink *L = NULL;
      ThumbNail **TH , *th;;
      void *pt , *pdel;
      int i , n;
      TH = ( ThumbNail ** ) kgGetList ( Wid ) ;
      if ( TH == NULL ) return 0;
      L = Dopen ( ) ;
      i = 0;
      while ( TH [ i ] != NULL ) {Dadd ( L , TH [ i ] ) ;i++;}
      if ( pos >= i ) return 0;
      Dposition ( L , pos+1 ) ;
      pdel = Dpick ( L ) ;
      th = ( ThumbNail * ) pdel;
      if ( th->img != NULL ) uiFreeImage ( th->img ) ;
      if ( th->name != NULL ) free ( th->name ) ;
      if ( th != NULL ) free ( th ) ;
      n = Dcount ( L ) +1;
      free ( TH ) ;
      TH = ( ThumbNail ** ) malloc ( sizeof ( ThumbNail * ) *n ) ;
      Resetlink ( L ) ;
      i = 0;
      while ( ( pt = Getrecord ( L ) ) != NULL ) {TH [ i ] = pt;i++;}
      TH [ i ] = NULL;
      kgSetList ( Wid , ( void ** ) TH ) ;
      Dfree ( L ) ;
      return 1;
  }
  void kgFreeThumbNail ( ThumbNail * th ) {
      if ( th != NULL ) {
          if ( th->img != NULL ) uiFreeImage ( th->img ) ;
          if ( th->name != NULL ) free ( th->name ) ;
          free ( th ) ;
      }
  }
  void * kgPickThumbNail ( void *Wid , int pos ) {
      Dlink *L = NULL;
      ThumbNail **TH , *th;;
      void *pt , *pdel;
      int i , n;
      TH = ( ThumbNail ** ) kgGetList ( Wid ) ;
      if ( TH == NULL ) return 0;
      L = Dopen ( ) ;
      i = 0;
      while ( TH [ i ] != NULL ) {Dadd ( L , TH [ i ] ) ;i++;}
      if ( pos >= i ) return 0;
      Dposition ( L , pos+1 ) ;
      pdel = Dpick ( L ) ;
      n = Dcount ( L ) +1;
      free ( TH ) ;
      TH = ( ThumbNail ** ) malloc ( sizeof ( ThumbNail * ) *n ) ;
      Resetlink ( L ) ;
      i = 0;
      while ( ( pt = Getrecord ( L ) ) != NULL ) {TH [ i ] = pt;i++;}
      TH [ i ] = NULL;
      kgSetList ( Wid , ( void ** ) TH ) ;
      Dfree ( L ) ;
      return pdel;
  }
  int kgMoveThumbNail ( void *wid , int item , int pos ) {
/*
 * wid is DIX/DIY/DICH 
 * Moves Thumnail at position 'item' to pos
*/
      if ( item < 0 ) return 0;
      if ( item == pos ) return 0;
      if ( item < pos ) {
          kgInsertThumbNail ( wid , kgPickThumbNail ( wid , item ) , pos ) ;
      }
      else {
          kgAddThumbNail ( wid , kgPickThumbNail ( wid , item ) , pos-1 ) ;
      }
      return 1;
  }
  void *kgSetWidgetImage ( void *wid , void *img ) {
      DIP *p;
      p = ( DIP * ) wid;
      if ( p->code != 'p' ) return NULL;
      p->xpm = img;
      return img;
  }
  void *kgGetWidgetImage ( void *wid ) {
      DIP *p;
      void *img = NULL;
      p = ( DIP * ) wid;
      if ( p->code != 'p' ) return NULL;
      img = p->xpm;
      return img;
  }
  static int uiDupItem ( void *tmp1 , void *tmp2 ) {
      ThumbNail *th1 , *th2;
      int ret;
      th1 = ( ThumbNail * ) tmp1;
      th2 = ( ThumbNail * ) tmp2;
      ret = strcmp ( th1->name , th2->name ) ;
//	printf("%s : %s :ret = %d\n",th1->name,th2->name,ret);
      if ( ret == 0 ) return 1;
      else return 0;
  }
  static int uiCompItem ( void *tmp1 , void *tmp2 ) {
      ThumbNail *th1 , *th2;
      int ret;
      th1 = ( ThumbNail * ) tmp1;
      th2 = ( ThumbNail * ) tmp2;
      ret = strcmp ( th1->name , th2->name ) ;
      if ( ret == 1 ) return 1;
      else return 0;
  }
  int kgSortList ( void *Wid ) {
      Dlink *L = NULL;
      ThumbNail **TH , *th;
      void *pt;
      int i , n;
      TH = ( ThumbNail ** ) kgGetList ( Wid ) ;
      if ( TH == NULL ) return 0;
      L = Dopen ( ) ;
      i = 0;
      while ( TH [ i ] != NULL ) {
          Dadd ( L , TH [ i ] ) ;
          i++;
      }
      n = Dcount ( L ) +1;
      if ( n == 1 ) return 0;
      Resetlink ( L ) ;
      Dsort ( L , uiCompItem ) ;
      Resetlink ( L ) ;
      i = 0;
      while ( ( pt = Getrecord ( L ) ) != NULL ) {TH [ i ] = pt;i++;}
      TH [ i ] = NULL;
      kgSetList ( Wid , ( void ** ) TH ) ;
      Dfree ( L ) ;
      return 1;
  }
  void ** kgCopyList ( void *Wid ) {
      Dlink *L = NULL;
      ThumbNail **TH , *th;
      ThumbNail **Tout;
      void *pt;
      int i , n;
      TH = ( ThumbNail ** ) kgGetList ( Wid ) ;
      if ( TH == NULL ) return NULL;
      i = 0;
      while ( TH [ i ] != NULL ) {
          i++;
      }
      n = i;
      Tout = ( ThumbNail ** ) malloc ( sizeof ( ThumbNail ) * ( n+1 ) ) ;
      i = 0;
      while ( TH [ i ] != NULL ) {
          Tout [ i ] = kgCopyThumbNail ( TH [ i ] ) ;
          i++;
      }
      Tout [ i ] = NULL;
      return ( void ** ) Tout;
  }
  int kgListRemoveDup_o ( void *Wid ) {
      Dlink *L = NULL;
      ThumbNail **TH , *th;
      void *pt;
      int i , n;
      TH = ( ThumbNail ** ) kgGetList ( Wid ) ;
      if ( TH == NULL ) return 0;
      L = Dopen ( ) ;
      i = 0;
      while ( TH [ i ] != NULL ) {
          Dadd ( L , TH [ i ] ) ;
          i++;
      }
      n = Dcount ( L ) +1;
      if ( n == 1 ) return 0;
      Resetlink ( L ) ;
      Drmvdup_cond ( L , uiDupItem ) ;
      Resetlink ( L ) ;
      i = 0;
      while ( ( pt = Getrecord ( L ) ) != NULL ) {TH [ i ] = pt;i++;}
      TH [ i ] = NULL;
      kgSetList ( Wid , ( void ** ) TH ) ;
      Dfree ( L ) ;
      return 1;
  }
  int kgListRemoveDup ( void *Wid ) {
      Dlink *L = NULL , *Dup = NULL;
      ThumbNail **TH , *th;
      void *pt;
      int i , n , j;
      int Got = 0;
      TH = ( ThumbNail ** ) kgGetList ( Wid ) ;
      if ( TH == NULL ) return 0;
      if ( TH [ 0 ] == NULL ) return 0;
      L = Dopen ( ) ;
      Dadd ( L , TH [ 0 ] ) ;
      i = 1;
      while ( TH [ i ] != NULL ) {
          Resetlink ( L ) ;
          Got = 0;
          while ( ( th = ( ThumbNail* ) Getrecord ( L ) ) != NULL ) {
              if ( uiDupItem ( th , TH [ i ] ) == 1 ) {
                  Got = 1;
                  break;
              }
          }
          if ( Got ) {
              if ( Dup == NULL ) Dup = Dopen ( ) ;
              Dappend ( Dup , TH [ i ] ) ;
          }
          else Dappend ( L , TH [ i ] ) ;
          i++;
      }
      n = Dcount ( L ) +1;
      Resetlink ( L ) ;
      i = 0;
      while ( ( pt = Getrecord ( L ) ) != NULL ) {TH [ i ] = pt;i++;}
      TH [ i ] = NULL;
      kgSetList ( Wid , ( void ** ) TH ) ;
      Dfree ( L ) ;
      if ( Dup != NULL ) {
          Resetlink ( Dup ) ;
          while ( ( pt = Getrecord ( Dup ) ) != NULL ) {
              kgFreeThumbNail ( ( ThumbNail * ) pt ) ;
          }
          Dfree ( Dup ) ;
      }
      return 1;
  }
  int kgGetSwitch ( void *Tmp , int item ) {
      DIA *D;DIX *X;DIW *W;DIE *E;
      BRW_STR *B;
      ThumbNail **list;
      int df = 0;
      X = ( DIX * ) Tmp;
      switch ( X->code ) {
          case 'x':
          case 'y':
          case 'c':
          df = * ( X->df ) -1;
          list = ( ThumbNail ** ) X->list;
          df = list [ item ]->sw;
          return df;
          break;
          default:
          fprintf ( stderr , "Invalid Widget: kgGetList\n" ) ;
          return 0;
      }
  }
  int kgSetSwitch ( void *Tmp , int item , int val ) {
      DIA *D;DIX *X;DIW *W;DIE *E;
      BRW_STR *B;
      ThumbNail **list;
      int df = 0;
      X = ( DIX * ) Tmp;
      if ( val != 0 ) val = 1;
      switch ( X->code ) {
          case 'x':
          if ( ( ( X->type ) %10 ) > 0 ) {
              df = * ( X->df ) -1;
              list = ( ThumbNail ** ) X->list;
              list [ item ]->sw = val;
              df = list [ item ]->sw;
              return df;
          }
          else {
              list = ( ThumbNail ** ) X->list;
              list [ item ]->sw = val;
          }
          break;
          case 'y':
          if ( ( ( X->type ) %10 ) > 0 ) {
              df = * ( X->df ) -1;
              list = ( ThumbNail ** ) X->list;
              list [ item ]->sw = val;
              df = list [ item ]->sw;
              return df;
          }
          else {
              list = ( ThumbNail ** ) X->list;
              list [ item ]->sw = val;
          }
          break;
          case 'c':
          df = * ( X->df ) -1;
          list = ( ThumbNail ** ) X->list;
          list [ item ]->sw = val;
          df = list [ item ]->sw;
          return df;
          break;
          default:
          fprintf ( stderr , "Invalid Widget: kgGetList\n" ) ;
          return 0;
      }
  }
  void kgSetScrollLength ( void *wid , double percent ) {
      DIV *V;
      V = ( DIV * ) wid;
      switch ( V->code ) {
          case 'v':
          case 'z':
          V->ds = percent;
          break;
          default:
          break;
      }
  }
  void kgSetScrollPos ( void *wid , double percent ) {
      DIV *V;
      V = ( DIV * ) wid;
      switch ( V->code ) {
          case 'v':
          case 'z':
          V->df = percent;
          break;
          default:
          break;
      }
  }
  double  kgGetScrollPos ( void *wid ) {
      DIV *V;
      V = ( DIV * ) wid;
      switch ( V->code ) {
          case 'v':
          case 'z':
          return V->df;
          break;
          default:
          break;
      }
  }
  double  kgGetScrollLength ( void *wid ) {
      DIV *V;
      V = ( DIV * ) wid;
      switch ( V->code ) {
          case 'v':
          case 'z':
          return V->ds;
          break;
          default:
          break;
      }
  }
  int kgGetWidgetSize ( void *wid , int *xsize , int *ysize ) {
      DIT *T;
      *xsize = 0;
      *ysize = 0;
      if ( wid == NULL ) return 0;
      T = ( DIT * ) wid;
      *xsize = T->x2 -T->x1;
      *ysize = T->y2 -T->y1;
      return 1;
  }
  int Dgetselectmenuitem ( void *Tmp , int menu ) {
      DIA *D;DIX *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .x;
      B = ( BRW_STR * ) ( E->bwsr ) ;
      return B->df;
  }
  int Dsetselectmenuitem ( void *Tmp , int menu , int df ) {
      int n = 0;
      ThumbNail **list;
      DIA *D;DIX *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .x;
      B = ( BRW_STR * ) ( E->bwsr ) ;
      if ( df < 1 ) df = 1;
      if ( df > E->nitems ) df = E->nitems;
      B->df = df;
      * ( E->df ) = df;
      return B->df;
  }
  void **Dgetselectmenu ( void *Tmp , int menu ) {
      DIA *D;DIX *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .x;
      return E->list;
  }
  void Dsetselectmenu ( void *Tmp , int menu , void **list ) {
      int ny;
      DIA *D;DIX *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .x;
      B = ( BRW_STR * ) ( E->bwsr ) ;
      E->list = list;
      ny = 0;
      if ( list != NULL ) while ( list [ ny ] != NULL ) ny++;
      if ( E->nx < 1 ) E->nx = 1;
      E->ny = ny/E->nx;
  }
  int Dgetselectsize ( void *Tmp , int menu ) {
      DIA *D;DIX *E;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .x;
      if ( E->nx < 1 ) E->nx = 1;
      return E->nitems;
  }
  void Dupdateselectmenu ( void *Tmp , int menu ) {
      DIA *D;DIX *E;DIY *Y;
      D = ( ( DIALOG * ) Tmp )->d;
      kgWC *wc;
      wc = WC ( Tmp ) ;
      E = D [ menu ] .x;
      uiBkup_clip_limits ( wc ) ;
      uiSet_full_scrn ( wc ) ;
      switch ( E->code ) {
          case 'e':
          _uiMake_E ( ( DIE * ) E ) ;
          break;
          case 'x':
          _uiMake_X ( E ) ;
          break;
          case 'y':
          _uiMake_Y ( ( DIY * ) E ) ;
          break;
          case 'r':
          _uiMake_RadioButton ( ( DIRA * ) E ) ;
          break;
          case 'c':
          _uiMake_CheckBox ( ( DICH * ) E ) ;
          break;
          default:
          break;
      }
      uiRest_clip_limits ( wc ) ;
      uiUpdateOn ( Tmp ) ;
  }
  void *kgFreeDouble ( void **mem ) {
      int i = 0;
      if ( mem != NULL ) {
          i = 0;while ( mem [ i ] != NULL ) {free ( mem [ i ] ) ;i++;}
          free ( mem ) ;
      }
      return NULL;
  }
  void Dmemfreeselectmenu ( void *Tmp , int wid ) {
      char **menu;
      int *sw , i;
      void **xpm;
      BRW_STR *B;
      DIA *D;DIX *E;
      D = ( ( DIALOG * ) Tmp )->d;
      kgWC *wc;
      wc = WC ( Tmp ) ;
      E = D [ wid ] .x;
      uiFreeThumbNails ( ( ThumbNail ** ) E->list ) ;
      E->list = NULL;
  }
  void *uiGetWidget ( void *Tmp , int id ) {
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      return ( void * ) ( D->d [ id ] .x ) ;
  }
  void *kgGetWidget ( void *Tmp , int id ) {
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      return ( void * ) ( D->d [ id ] .x ) ;
  }
  void *kgGetCurrentWidget ( void *Tmp ) {
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      return kgGetWidget ( Tmp , D->CurWid ) ;
  }
  int kgSetCurrentWidget ( void *Tmp , int Wid ) {
      int ret;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      ret = ui_NotInputBox ( D , Wid ) ;
      ret = ( ret+1 ) %2;
      if ( ret ) D->CurWid = Wid;
      return ret;
  }
  int kgSetAttnWidget ( void *Tmp , void *Wid ) {
      int id = kgGetWidgetId ( ( DIALOG * ) Tmp , Wid ) ;
      return kgSetCurrentWidget ( Tmp , id ) ;
  }
  int kgSetDefaultAttnWidget ( void *Tmp , void *Wid ) {
/*
   to be used only in Ui init function 
   to set the input attention
*/
      int id = kgGetWidgetId ( ( DIALOG * ) Tmp , Wid ) ;
      return kgSetDefaultWidget ( Tmp , id ) ;
  }
  int kgSetDefaultWidget ( void *Tmp , int Wid ) {
/*
   to be used only in Ui init function 
   to set the input attention
*/
      int ret;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      ret = ui_NotInputBox ( D , Wid ) ;
      ret = ( ret+1 ) %2;
      if ( ret ) D->df = Wid;
      return ret;
  }
  void *kgGetCurrentWidgetName ( void *Tmp ) {
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      return kgGetWidgetName ( Tmp , D->CurWid ) ;
  }
  void * kgGetClickedWidget ( void *Dtmp ) {
      KBEVENT kb;
      void *wid = NULL;
      int i , n , x1 , y1 , xo , yo;
      DIA *d;
      DIALOG *D;
      D = ( DIALOG * ) Dtmp;
      kb = D->kb;
      d = D->d;
      if ( kb.event != 1 ) return NULL;
      x1 = kb.x;
      y1 = kb.y;
      if ( x1 < D->xo ) return NULL;
      if ( x1 > ( D->xo+D->xl ) ) return NULL;
      if ( y1 < D->yo ) return NULL;
      if ( y1 > ( D->yo+D->yl ) ) return NULL;
      xo = D->xo;
      yo = D->yo;
      i = 0; while ( d [ i ] .t != NULL ) i++;
      n = i;
      for ( i = 0;i < n;i++ ) {
          if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) == 0 ) continue;
          if ( uiCheckClickPosition ( d [ i ] .t->x1+xo , d [ i ] .t->y1+yo , d [ i ] .t->x2+xo , d [ i ] .t->y2+yo , x1 , y1 ) == 1 ) \
          {wid = ( void * ) d [ i ] .t; break;}
      }
      return wid;
  }
  void kgGetClickedPosition ( void *Dtmp , int *x , int *y ) {
      KBEVENT kb;
      void *wid = NULL;
      int i , n , x1 , y1 , xo , yo;
      DIA *d;
      DIALOG *D;
      D = ( DIALOG * ) Dtmp;
      kb = D->kb;
      d = D->d;
      *x = -1;*y = -1;
      if ( ( kb.event != 1 ) && ( kb.event != 3 ) ) return ;
      x1 = kb.x;
      y1 = kb.y;
      *x = x1 - D->xo;
      *y = y1 - D->yo;
      return ;
  }
  int kgGetWidgetLocation ( void *wid , int *x1 , int *y1 ) {
      DIT *T;
      if ( wid != NULL ) {
          *x1 = T->x1;
          *y1 = T->y1;
          return 1;
      }
      return 0;
  }
  void * kgGetLocationWidget ( void *Dtmp , int x1 , int y1 ) {
      KBEVENT kb;
      void *wid = NULL;
      int i , n , xo , yo;
      DIA *d;
      DIALOG *D;
      D = ( DIALOG * ) Dtmp;
      kb = D->kb;
      d = D->d;
      if ( x1 < D->xo ) return NULL;
      if ( x1 > ( D->xo+D->xl ) ) return NULL;
      if ( y1 < D->yo ) return NULL;
      if ( y1 > ( D->yo+D->yl ) ) return NULL;
      xo = D->xo;
      yo = D->yo;
      i = 0; while ( d [ i ] .t != NULL ) i++;
      n = i;
// for(i=0;i<n;i++) {
      for ( i = n-1;i >= 0;i-- ) {
          if ( kgGetWidgetVisibility ( kgGetWidget ( D , i ) ) == 0 ) continue;
          if ( uiCheckClickPosition ( d [ i ] .t->x1+xo , d [ i ] .t->y1+yo , d [ i ] .t->x2+xo , d [ i ] .t->y2+yo , x1 , y1 ) == 1 ) \
          {wid = ( void * ) d [ i ] .t; break;}
      }
      return wid;
  }
  void uiSetWidgetVisibility ( void *Tmp , int id , int val ) {
      DIX *x;
      int hide = 0;
      x = ( DIX * ) uiGetWidget ( Tmp , id ) ;
      if ( val == 0 ) hide = 1;
      switch ( x->code ) {
          case 'x':
          case 'r':
          case 'c':
          case 'y':
          x->hide = hide;
          break;
          case 'o':
           ( ( DIO * ) x )->hide = hide;
          break;
      }
      uiUpdateWidget ( ( void * ) x ) ;
      uiUpdateOn ( Tmp ) ;
  }
  int uiGetWidgetVisibility ( void *Tmp , int id ) {
      DIX *x;
      int hide = 0 , ret = 1;
      x = ( DIX * ) uiGetWidget ( Tmp , id ) ;
      switch ( x->code ) {
          case 'x':
          case 'r':
          case 'c':
          case 'y':
          hide = x->hide;
          break;
          case 'o':
          hide = ( ( DIO * ) x )->hide;
          break;
      }
      if ( hide == 1 ) ret = 0;
      return ret;
  }
  int kgGetWidgetVisibility ( void *Tmp ) {
      DIX *x;
      int hide = 0 , ret = 1;
      x = ( DIX * ) ( Tmp ) ;
      switch ( x->code ) {
          case 'x':
          case 'r':
          case 'c':
          case 'y':
          hide = x->hide;
          break;
          case 'v':
          hide = ( ( DIV * ) x )->hide;
          break;
          case 'z':
          hide = ( ( DIZ * ) x )->hide;
          break;
          case 'o':
          hide = ( ( DIO * ) x )->hide;
          break;
          case 'p': /* new for xpm display */
          hide = ( ( DIP * ) x )->hide;
          break;
          case 'i': /* info box */
          hide = ( ( DII * ) x )->hide;
          break;
          case 't':
          case 'T':
//       printf("Setting visibility: %d\n",val);
          hide = ( ( DIT * ) x )->hide;
          break;
          case 'h':
          hide = ( ( DIL * ) x )->hide;
          break;
          case 'H':
          hide = ( ( DILN * ) x )->hide;
          break;
          case 'n':
          hide = ( ( DIN * ) x )->hide;
          break;
          case 'b':
          case 'N':
          hide = ( ( DIBN * ) x )->hide;
          break;
          case 'f':
          hide = ( ( DIF * ) x )->hide;
          break;
          case 'P':
          hide = ( ( DIHB * ) x )->hide;
          break;
          case 'd':
          hide = ( ( DID * ) x )->hide;
          break;
          case 'w':
          hide = ( ( DIW * ) x )->hide;
          break;
          case 'e':
          hide = ( ( DIE * ) x )->hide;
          break;
          case 's':
          hide = ( ( DIS * ) x )->hide;
          break;
          case 'g':
          hide = ( ( DIG * ) x )->hide;
          break;
          case 'm':
          case 'M':
          case 'B':
          hide = ( ( DIM * ) x )->hide;
          break;
          default:
          break;
      }
      if ( hide == 1 ) ret = 0;
      return ret;
  }
  int kgSetWidgetVisibility ( void *Tmp , int val ) {
      DIX *x;
      int hide = 0 , hideo = 0 , ret = 0;
      DIALOG *D = NULL;
      x = ( DIX * ) ( Tmp ) ;
      if ( val == 0 ) hide = 1;
      switch ( x->code ) {
          case 'x':
          case 'r':
          case 'c':
          case 'y':
          hideo = x->hide;
          x->hide = hide;
          D = ( DIALOG * ) ( x->D ) ;
          break;
          case 'v':
          hideo = ( ( DIV * ) x )->hide;
           ( ( DIV * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIV * ) x )->D ) ;
          break;
          case 'z':
          hideo = ( ( DIZ * ) x )->hide;
           ( ( DIZ * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIZ * ) x )->D ) ;
          break;
          case 'o':
          hideo = ( ( DIO * ) x )->hide;
           ( ( DIO * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DID * ) x )->D ) ;
          break;
          case 'p': /* new for xpm display */
          hideo = ( ( DIP * ) x )->hide;
           ( ( DIP * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIP * ) x )->D ) ;
          break;
          case 'i': /* info box */
          hideo = ( ( DII * ) x )->hide;
           ( ( DII * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DII * ) x )->D ) ;
          break;
          case 't':
          case 'T':
//       printf("Setting visibility: %d\n",val);
          hideo = ( ( DIT * ) x )->hide;
           ( ( DIT * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIT * ) x )->D ) ;
          break;
          case 'h':
          hideo = ( ( DIL * ) x )->hide;
           ( ( DIL * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIL * ) x )->D ) ;
          break;
          case 'H':
          hideo = ( ( DILN * ) x )->hide;
           ( ( DILN * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DILN * ) x )->D ) ;
          break;
          case 'n':
          hideo = ( ( DIN * ) x )->hide;
           ( ( DIN * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIN * ) x )->D ) ;
          break;
          case 'b':
          hideo = ( ( DIB * ) x )->hide;
           ( ( DIB * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIB * ) x )->D ) ;
          break;
          case 'N':
          hideo = ( ( DIBN * ) x )->hide;
           ( ( DIBN * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIBN * ) x )->D ) ;
          break;
          case 'f':
          hideo = ( ( DIF * ) x )->hide;
           ( ( DIF * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIF * ) x )->D ) ;
          break;
          case 'P':
          hideo = ( ( DIHB * ) x )->hide;
           ( ( DIHB * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIHB * ) x )->D ) ;
          break;
          case 'd':
          hideo = ( ( DID * ) x )->hide;
           ( ( DID * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DID * ) x )->D ) ;
          break;
          case 'w':
          hideo = ( ( DIW * ) x )->hide;
           ( ( DIW * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIW * ) x )->D ) ;
          break;
          case 'e':
          hideo = ( ( DIE * ) x )->hide;
           ( ( DIE * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIE * ) x )->D ) ;
          break;
          case 's':
          hideo = ( ( DIS * ) x )->hide;
           ( ( DIS * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIS * ) x )->D ) ;
          break;
          case 'g':
          hideo = ( ( DIG * ) x )->hide;
           ( ( DIG * ) x )->hide = hide;
          break;
          case 'm':
          case 'M':
          case 'B':
          hideo = ( ( DIM * ) x )->hide;
           ( ( DIM * ) x )->hide = hide;
          D = ( DIALOG * ) ( ( ( DIM * ) x )->D ) ;
          break;
          default:
          break;
      }
      if ( hide != hideo ) {
          if ( D != NULL ) kgUpdateWidget ( ( void * ) x ) ;
//    kgUpdateOn(Tmp);
          ret = 1;
      }
      return ret;
  }
  void * kgGetWidgetName ( void *Tmp , int id ) {
      DIX *x;
      x = ( DIX * ) kgGetWidget ( Tmp , id ) ;
      if ( x == NULL ) return NULL;
      switch ( x->code ) {
          case 'x':
          case 'r':
          case 'c':
          case 'y':
          return x->Wid ;
          break;
          case 'v':
          return ( ( DIV * ) x )->Wid ;
          break;
          case 'z':
          return ( ( DIZ * ) x )->Wid ;
          break;
          case 'o':
          return ( ( DIO * ) x )->Wid ;
          break;
          case 'p': /* new for xpm display */
          return ( ( DIP * ) x )->Wid ;
          break;
          case 'i': /* info box */
          return ( ( DII * ) x )->Wid ;
          break;
          case 't':
          case 'T':
          return ( ( DIT * ) x )->Wid ;
          break;
          case 'h':
          return ( ( DIL * ) x )->Wid ;
          break;
          case 'H':
          return ( ( DILN * ) x )->Wid ;
          break;
          case 'n':
          return ( ( DIBN * ) x )->Wid ;
          break;
          case 'b':
          case 'N':
          return ( ( DIB * ) x )->Wid ;
          break;
          case 'f':
          return ( ( DIF * ) x )->Wid;
          break;
          case 'P':
          return ( ( DIHB * ) x )->Wid ;
          break;
          case 'd':
          return ( ( DID * ) x )->Wid ;
          break;
          case 'w':
          return ( ( DIW * ) x )->Wid ;
          break;
          case 'e':
          return ( ( DIE * ) x )->Wid ;
          break;
          case 's':
          return ( ( DIS * ) x )->Wid ;
          break;
          case 'g':
          return ( ( DIG * ) x )->Wid ;
          break;
          case 'm':
          case 'M':
          case 'B':
          return ( ( DIM * ) x )->Wid ;
          break;
          default:
          break;
      }
      return NULL;
  }
  void * kgGetNamedWidget ( void *Tmp , char *id ) {
      DIALOG *D;
      DIA *d;
      DIX *x;
      int i = 0;
      D = ( DIALOG * ) Tmp;
      d = D->d;
      while ( d [ i ] .x != NULL ) {
          x = ( DIX * ) d [ i ] .x;
          i++;
          switch ( x->code ) {
              case 'x':
              case 'r':
              case 'c':
              case 'y':
              if ( strcmp ( x->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'v':
              if ( strcmp ( ( ( DIV * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'z':
              if ( strcmp ( ( ( DIZ * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'o':
              if ( strcmp ( ( ( DIO * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'p': /* new for xpm display */
              if ( strcmp ( ( ( DIP * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'i': /* info box */
              if ( strcmp ( ( ( DII * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 't':
              case 'T':
              if ( strcmp ( ( ( DIT * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'h':
              if ( strcmp ( ( ( DIL * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'H':
              if ( strcmp ( ( ( DILN * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'n':
              if ( strcmp ( ( ( DIN * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'b':
              case 'N':
              if ( strcmp ( ( ( DIB * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'f':
              if ( strcmp ( ( ( DIF * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'P':
              if ( strcmp ( ( ( DIHB * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'd':
              if ( strcmp ( ( ( DID * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'w':
              if ( strcmp ( ( ( DIW * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'e':
              if ( strcmp ( ( ( DIE * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 's':
              if ( strcmp ( ( ( DIS * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'g':
              if ( strcmp ( ( ( DIG * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              case 'm':
              case 'M':
              case 'B':
              if ( strcmp ( ( ( DIM * ) x )->Wid , id ) == 0 ) return ( void * ) x;
              break;
              default:
              break;
          }
      }
      return NULL;
  }
  int kgCheckWidgetName ( void *wid , char *name ) {
      DIX *x;
      int i = 0;
      int ret = 0;
      x = ( DIX * ) wid;
      switch ( x->code ) {
          case 'x':
          case 'r':
          case 'c':
          case 'y':
          if ( strcmp ( x->Wid , name ) == 0 ) return 1;
          break;
          case 'v':
          if ( strcmp ( ( ( DIV * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'z':
          if ( strcmp ( ( ( DIZ * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'o':
          if ( strcmp ( ( ( DIO * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'p': /* new for xpm display */
          if ( strcmp ( ( ( DIP * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'i': /* info box */
          if ( strcmp ( ( ( DII * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 't':
          case 'T':
          if ( strcmp ( ( ( DIT * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'h':
          if ( strcmp ( ( ( DIL * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'H':
          if ( strcmp ( ( ( DILN * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'n':
          if ( strcmp ( ( ( DIN * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'b':
          case 'N':
          if ( strcmp ( ( ( DIB * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'f':
          if ( strcmp ( ( ( DIF * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'P':
          if ( strcmp ( ( ( DIHB * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'd':
          if ( strcmp ( ( ( DID * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'w':
          if ( strcmp ( ( ( DIW * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'e':
          if ( strcmp ( ( ( DIE * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 's':
          if ( strcmp ( ( ( DIS * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'g':
          if ( strcmp ( ( ( DIG * ) x )->Wid , name ) == 0 ) return 1;
          break;
          case 'm':
          case 'M':
          case 'B':
          if ( strcmp ( ( ( DIM * ) x )->Wid , name ) == 0 ) return 1;
          break;
          default:
          break;
      }
      return ret;
  }
  int kgSetGrpVisibility ( void *Tmp , int grpid , int val ) {
      DIX *x;
      void *wid;
      int hide = 0 , hideo = 0 , ret = 0;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      Dlink *Grp;
      Dlink *Gpt;
      WIDGETGRP *pt = NULL;
      Grp = ( Dlink * ) D->GrpList;
      if ( Grp == NULL ) {
          return 0;
      }
      if ( Dcount ( Grp ) < grpid ) return 0;
      if ( grpid < 1 ) return 0;
      Resetlink ( Grp ) ;
      Dposition ( Grp , grpid ) ;
      pt = ( WIDGETGRP * ) Getrecord ( Grp ) ;
      Gpt = ( Dlink * ) pt->wlist;
      Resetlink ( Gpt ) ;
      if ( val == 0 ) hide = 1;
      else hide = 0;
      pt->hide = hide;
      while ( ( x = ( DIX * ) Getrecord ( Gpt ) ) != NULL ) {
          if ( x == NULL ) continue;
//  printf("Code = %c\n",x->code);
          switch ( x->code ) {
              case 'x':
              hideo = x->hide;
              x->hide = hide;
              break;
              case 'r':
              hideo = x->hide;
              x->hide = hide;
              break;
              case 'c':
              hideo = x->hide;
              x->hide = hide;
              break;
              case 'y':
              hideo = x->hide;
              x->hide = hide;
              break;
              case 'v':
              hideo = ( ( DIV * ) x )->hide;
               ( ( DIV * ) x )->hide = hide;
              break;
              case 'z':
              hideo = ( ( DIZ * ) x )->hide;
               ( ( DIZ * ) x )->hide = hide;
              break;
              case 'o':
              hideo = ( ( DIO * ) x )->hide;
               ( ( DIO * ) x )->hide = hide;
              break;
              case 'p': /* new for xpm display */
              hideo = ( ( DIP * ) x )->hide;
               ( ( DIP * ) x )->hide = hide;
              break;
              case 'i': /* info box */
              hideo = ( ( DII * ) x )->hide;
               ( ( DII * ) x )->hide = hide;
              break;
              case 't':
              hideo = ( ( DIT * ) x )->hide;
               ( ( DIT * ) x )->hide = hide;
              break;
              case 'T':
              hideo = ( ( DIT * ) x )->hide;
               ( ( DIT * ) x )->hide = hide;
              break;
              case 'h':
              hideo = ( ( DIL * ) x )->hide;
               ( ( DIL * ) x )->hide = hide;
              break;
              case 'H':
              hideo = ( ( DILN * ) x )->hide;
               ( ( DILN * ) x )->hide = hide;
              break;
              case 'n':
              hideo = ( ( DIN * ) x )->hide;
               ( ( DIN * ) x )->hide = hide;
              break;
              case 'b':
              hideo = ( ( DIB * ) x )->hide;
               ( ( DIB * ) x )->hide = hide;
              break;
              case 'N':
              hideo = ( ( DIBN * ) x )->hide;
               ( ( DIB * ) x )->hide = hide;
              break;
              case 'f':
              hideo = ( ( DIF * ) x )->hide;
               ( ( DIF * ) x )->hide = hide;
              break;
              case 'P':
              hideo = ( ( DIHB * ) x )->hide;
               ( ( DIHB * ) x )->hide = hide;
              break;
              case 'd':
              hideo = ( ( DID * ) x )->hide;
               ( ( DID * ) x )->hide = hide;
              break;
              case 'w':
              hideo = ( ( DIW * ) x )->hide;
               ( ( DIW * ) x )->hide = hide;
              break;
              case 'e':
              hideo = ( ( DIE * ) x )->hide;
               ( ( DIE * ) x )->hide = hide;
              break;
              case 's':
              hideo = ( ( DIS * ) x )->hide;
               ( ( DIS * ) x )->hide = hide;
              break;
              case 'g':
              hideo = ( ( DIG * ) x )->hide;
               ( ( DIG * ) x )->hide = hide;
              break;
              case 'm':
              case 'M':
              case 'B':
              hideo = ( ( DIM * ) x )->hide;
               ( ( DIM * ) x )->hide = hide;
              break;
              default:
              break;
          }
          if ( ( D->wc != NULL ) && ( D->TotWid > 0 ) ) {
              if ( hide != hideo ) kgUpdateWidget ( ( void * ) x ) ;
          }
      }
      ret = 1;
      return ret;
  }
#define WMOVE(x) {\
   xl = x->x2 - x->x1;\
       yl = x->y2 - x->y1;\
       x->x1 = x1; x->y1 = y1;\
       x->x2 = x1+xl; x->y2 = y1+yl;\
       x->hide = 0;\
   }
#define WMOVE_(x) {\
   xl = x->x2 - x->x1;\
       yl = x->y2 - x->y1;\
       x->x1 = x1; x->y1 = y1;\
       x->x2 = x1+xl; x->y2 = y1+yl;\
   }
#define WSHIFT_(x,xl,yl) {\
   x->x1 += xl; x->y1+= yl;\
       x->x2 += xl; x->y2 += yl;\
   }
  void uiCleanTextBoxImages ( DIT *T ) {
      int i , n;
      T_ELMT *elmt;
      if ( T->item == -1 ) return;
      n = T->nx*T->ny;
      elmt = T->elmt;
      for ( i = 0;i < n;i++ ) {
          if ( elmt [ i ] .img != NULL ) {
              kgFreeImage ( elmt [ i ] .img ) ;
              elmt [ i ] .img = NULL;
          }
      }
  }
  void uiCleanButtonBoxImages ( void *Tmp ) {
      int i , n;
      DIN *b;
      BUT_STR *buts;
      b = ( DIN * ) Tmp;
      if ( b->item == -1 ) return;
      n = b->nx*b->ny;
      buts = ( BUT_STR * ) ( b->buts ) ;
      for ( i = 0;i < n;i++ ) {
          if ( buts [ i ] .imgn != NULL ) kgFreeImage ( buts [ i ] .imgn ) ;
          buts [ i ] .imgn = NULL;
          if ( buts [ i ] .imgp != NULL ) kgFreeImage ( buts [ i ] .imgp ) ;
          buts [ i ] .imgp = NULL;
          if ( buts [ i ] .imgh != NULL ) kgFreeImage ( buts [ i ] .imgh ) ;
          buts [ i ] .imgh = NULL;
          if ( buts [ i ] .Bimg != NULL ) kgFreeImage ( buts [ i ] .Bimg ) ;
          buts [ i ] .Bimg = NULL;
          if ( buts [ i ] .Bimg0 != NULL ) kgFreeImage ( buts [ i ] .Bimg0 ) ;
          buts [ i ] .Bimg0 = NULL;
      }
  }
  int kgMoveGrp ( void *Tmp , int grpid , int x1 , int y1 ) {
      DIX *x;
      int hide = 0 , hideo = 0 , ret = 0 , xl , yl;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      Dlink *Grp;
      Dlink *Gpt;
      WIDGETGRP *pt = NULL;
      DIRA *r; DICH *c; DIY *y;DIT *T; DII *I;
      DIV *v; DIZ *z; DIO *o; DIP *p;DILN *h;DIBN *n; DIB *b;
      DIF *f; DIHB *P;DID *d; DIW *w;
      DIE *e; DIS *s; DIG *g; DIM *m;
      DIL *H; DIN *N;
      Grp = ( Dlink * ) D->GrpList;
      if ( Grp == NULL ) {
          return 0;
      }
      if ( Dcount ( Grp ) < grpid ) return 0;
      if ( grpid < 1 ) return 0;
      Resetlink ( Grp ) ;
      Dposition ( Grp , grpid ) ;
      pt = ( WIDGETGRP * ) Getrecord ( Grp ) ;
      Gpt = ( Dlink * ) pt->wlist;
      Resetlink ( Gpt ) ;
      while ( ( x = ( DIX * ) Getrecord ( Gpt ) ) != NULL ) {
          if ( x->D != Tmp ) {WMOVE_ ( x ) ;continue;}
          switch ( x->code ) {
              case 'x':
              x->hide = 1;
              _uiMake_X ( x ) ;
              WMOVE ( x ) ;
              FreeImg ( ( ( DIX * ) ( x ) )->Bimg ) ;
              _uiMake_X ( x ) ;
              break;
              case 'r':
              r = ( DIRA * ) x;
              r->hide = 1;
              _uiMake_RadioButton ( r ) ;
              WMOVE ( r ) ;
              FreeImg ( ( ( DIRA * ) ( x ) )->Bimg ) ;
              _uiMake_RadioButton ( r ) ;
              break;
              case 'c':
              c = ( DICH * ) x;
              c->hide = 1;
              _uiMake_CheckBox ( ( DICH * ) x ) ;
              WMOVE ( c ) ;
              FreeImg ( ( ( DICH * ) ( x ) )->Bimg ) ;
              _uiMake_CheckBox ( ( DICH * ) x ) ;
              break;
              case 'y':
              y = ( DIY * ) x;
              y->hide = 1;
              _uiMake_Y ( ( DIY * ) x ) ;
              WMOVE ( y ) ;
              FreeImg ( ( ( DIY * ) ( x ) )->Bimg ) ;
              _uiMake_Y ( ( DIY * ) x ) ;
              break;
              case 'v':
              v = ( DIV * ) x;
              v->hide = 1;
              _uiMake_V ( ( DIV * ) x ) ;
              WMOVE ( v ) ;
              FreeImg ( ( ( DIV * ) ( x ) )->Bimg ) ;
              _uiMake_V ( ( DIV * ) x ) ;
              break;
              case 'z':
              z = ( DIZ * ) x;
              z->hide = 1;
              _uiMake_Z ( ( DIZ * ) x ) ;
              WMOVE ( z ) ;
              FreeImg ( ( ( DIZ * ) ( x ) )->Bimg ) ;
              _uiMake_Z ( ( DIZ * ) x ) ;
              break;
              case 'o':
              o = ( DIO * ) x;
              o->hide = 1;
              _uiMake_O ( ( DIO * ) x ) ;
              WMOVE ( o ) ;
              FreeImg ( ( ( DIO * ) ( x ) )->Bimg ) ;
              _uiMake_O ( ( DIO * ) x ) ;
              break;
              case 'p': /* new for xpm display */
              p = ( DIP * ) x;
              p->hide = 1;
              _uiMake_P ( ( DIP * ) x ) ;
              WMOVE ( p ) ;
              FreeImg ( ( ( DIP * ) ( x ) )->Bimg ) ;
              _uiMake_P ( ( DIP * ) x ) ;
              break;
              case 'i': /* info box */
              I = ( DII * ) x;
              I->hide = 1;
              _uiMake_I ( ( DII * ) x ) ;
              WMOVE ( I ) ;
              FreeImg ( ( ( DII * ) ( x ) )->Bimg ) ;
              _uiMake_I ( ( DII * ) x ) ;
              break;
              case 't':
              T = ( DIT * ) x;
              T->hide = 1;
              _uiMake_Tx ( ( DIT * ) x ) ;
              WMOVE ( T ) ;
              uiCleanTextBoxImages ( T ) ;
              FreeImg ( ( ( DIT * ) ( x ) )->Bimg ) ;
              _uiMake_Tx ( ( DIT * ) x ) ;
              break;
              case 'T':
              T = ( DIT * ) x;
              T->hide = 1;
              _uiMake_Ta ( ( DIT * ) x ) ;
              WMOVE ( T ) ;
              uiCleanTextBoxImages ( T ) ;
              FreeImg ( ( ( DIT * ) ( x ) )->Bimg ) ;
              _uiMake_Ta ( ( DIT * ) x ) ;
              break;
              case 'h':
              H = ( DIL * ) x;
              H->hide = 1;
              _uiMake_N ( ( DIL * ) x ) ;
              WMOVE ( H ) ;
              uiCleanButtonBoxImages ( x ) ;
              FreeImg ( ( ( DIL * ) ( x ) )->Bimg ) ;
              _uiMake_N ( ( DIL * ) x ) ;
              break;
              case 'H':
              h = ( DILN * ) x;
              h->hide = 1;
              _uiMake_H ( ( DILN * ) x ) ;
              WMOVE ( h ) ;
              FreeImg ( ( ( DILN * ) ( x ) )->Bimg ) ;
              _uiMake_H ( ( DILN * ) x ) ;
              break;
              case 'N':
              n = ( DIBN * ) x;
              n->hide = 1;
              _uiMake_B ( ( DIBN * ) x ) ;
              WMOVE ( n ) ;
              FreeImg ( ( ( DIBN * ) ( x ) )->Bimg ) ;
              _uiMake_B ( ( DIBN * ) x ) ;
              break;
              case 'n':
              N = ( DIN * ) x;
              N->hide = 1;
              _uiMake_N ( ( DIN * ) x ) ;
              WMOVE ( N ) ;
              uiCleanButtonBoxImages ( x ) ;
              FreeImg ( ( ( DIN * ) ( x ) )->Bimg ) ;
              _uiMake_N ( ( DIN * ) x ) ;
              break;
              case 'b':
              b = ( DIB * ) x;
              b->hide = 1;
              _uiMake_B ( ( DIBN * ) x ) ;
              WMOVE ( b ) ;
              FreeImg ( ( ( DIBN * ) ( x ) )->Bimg ) ;
              _uiMake_B ( ( DIBN * ) x ) ;
              break;
              case 'f':
              f = ( DIF * ) x;
              f->hide = 1;
              _uiMake_SF ( ( DIF * ) x ) ;
              WMOVE ( f ) ;
              FreeImg ( ( ( DIF * ) ( x ) )->Bimg ) ;
              _uiMake_SF ( ( DIF * ) x ) ;
              break;
              case 'P':
              P = ( DIHB * ) x;
              P->hide = 1;
              _uiMake_SH ( ( DIHB * ) x ) ;
              WMOVE ( P ) ;
              FreeImg ( ( ( DIHB * ) ( x ) )->Bimg ) ;
              _uiMake_SH ( ( DIHB * ) x ) ;
              break;
              case 'd':
              d = ( DID * ) x;
              d->hide = 1;
              _uiMake_SD ( ( DID * ) x ) ;
              WMOVE ( d ) ;
              FreeImg ( ( ( DID * ) ( x ) )->Bimg ) ;
              _uiMake_SD ( ( DID * ) x ) ;
              break;
              case 'w':
              w = ( DIW * ) x;
              w->hide = 1;
              _uiMake_W ( ( DIW * ) x ) ;
              WMOVE ( w ) ;
              FreeImg ( ( ( DIW * ) ( x ) )->Bimg ) ;
              _uiMake_W ( ( DIW * ) x ) ;
              break;
              case 'e':
              e = ( DIE * ) x;
              e->hide = 1;
              _uiMake_E ( ( DIE * ) x ) ;
              WMOVE ( e ) ;
              FreeImg ( ( ( DIE * ) ( x ) )->Bimg ) ;
              _uiMake_E ( ( DIE * ) x ) ;
              break;
              case 's':
              s = ( DIS * ) x;
              s->hide = 1;
              _uiMake_MS ( ( DIS * ) x ) ;
              WMOVE ( s ) ;
              FreeImg ( ( ( DIS * ) ( x ) )->Bimg ) ;
              _uiMake_MS ( ( DIS * ) x ) ;
              break;
              case 'g':
              g = ( DIG * ) x;
              g->hide = 1;
              _uiMake_G ( ( DIG * ) x ) ;
              WMOVE ( g ) ;
              FreeImg ( ( ( DIG * ) ( x ) )->Bimg ) ;
              _uiMake_G ( ( DIG * ) x ) ;
              break;
              case 'm':
              case 'M':
              case 'B':
              m = ( DIM * ) x;
              m->hide = 1;
              _uiMake_M ( ( DIM * ) x ) ;
              WMOVE ( m ) ;
              FreeImg ( ( ( DIM * ) ( x ) )->Bimg ) ;
              _uiMake_M ( ( DIM * ) x ) ;
              break;
              default:
              break;
          }
      }
      ret = 1;
      return ret;
  }
#define WRESIZE(x) {\
   x->x2 = x->x1+xl; x->y2 = x->y1+yl;\
       x->hide = 0;\
   }
#define WRESIZE_(x) {\
   x->x2 = x->x1+xl; x->y2 = x->y1+yl;\
   }
  int kgResizeGrp ( void *Tmp , int grpid , int xl , int yl ) {
      DIX *x;
      int hide = 0 , hideo = 0 , ret = 0;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      Dlink *Grp;
      Dlink *Gpt;
      WIDGETGRP *pt = NULL;
      DIRA *r; DICH *c; DIY *y;DIT *T; DII *I;
      DIV *v; DIZ *z; DIO *o; DIP *p;DILN *h;DIBN *n; DIB *b;
      DIF *f; DIHB *P;DID *d; DIW *w;
      DIE *e; DIS *s; DIG *g; DIM *m;
      DIL *H; DIN *N;
      Grp = ( Dlink * ) D->GrpList;
      if ( Grp == NULL ) {
          return 0;
      }
      if ( Dcount ( Grp ) < grpid ) return 0;
      if ( grpid < 1 ) return 0;
      Resetlink ( Grp ) ;
      Dposition ( Grp , grpid ) ;
      pt = ( WIDGETGRP * ) Getrecord ( Grp ) ;
      Gpt = ( Dlink * ) pt->wlist;
      Resetlink ( Gpt ) ;
      while ( ( x = ( DIX * ) Getrecord ( Gpt ) ) != NULL ) {
          if ( x->D != Tmp ) {WRESIZE_ ( x ) ;continue;}
          switch ( x->code ) {
              case 'x':
              x->hide = 1;
              _uiMake_X ( x ) ;
              WRESIZE ( x ) ;
              _uiMake_X ( x ) ;
              break;
              case 'r':
              r = ( DIRA * ) x;
              r->hide = 1;
              _uiMake_RadioButton ( r ) ;
              WRESIZE ( r ) ;
              _uiMake_RadioButton ( r ) ;
              break;
              case 'c':
              c = ( DICH * ) x;
              c->hide = 1;
              _uiMake_CheckBox ( ( DICH * ) x ) ;
              WRESIZE ( c ) ;
              _uiMake_CheckBox ( ( DICH * ) x ) ;
              break;
              case 'y':
              y = ( DIY * ) x;
              y->hide = 1;
              _uiMake_Y ( ( DIY * ) x ) ;
              WRESIZE ( y ) ;
              _uiMake_Y ( ( DIY * ) x ) ;
              break;
              case 'v':
              v = ( DIV * ) x;
              v->hide = 1;
              _uiMake_V ( ( DIV * ) x ) ;
              WRESIZE ( v ) ;
              _uiMake_V ( ( DIV * ) x ) ;
              break;
              case 'z':
              z = ( DIZ * ) x;
              z->hide = 1;
              _uiMake_Z ( ( DIZ * ) x ) ;
              WRESIZE ( z ) ;
              _uiMake_Z ( ( DIZ * ) x ) ;
              break;
              case 'o':
              o = ( DIO * ) x;
              o->hide = 1;
              _uiMake_O ( ( DIO * ) x ) ;
              WRESIZE ( o ) ;
              _uiMake_O ( ( DIO * ) x ) ;
              break;
              case 'p': /* new for xpm display */
              p = ( DIP * ) x;
              p->hide = 1;
              _uiMake_P ( ( DIP * ) x ) ;
              WRESIZE ( p ) ;
              _uiMake_P ( ( DIP * ) x ) ;
              break;
              case 'i': /* info box */
              I = ( DII * ) x;
              I->hide = 1;
              _uiMake_I ( ( DII * ) x ) ;
              WRESIZE ( I ) ;
              _uiMake_I ( ( DII * ) x ) ;
              break;
              case 't':
              T = ( DIT * ) x;
              T->hide = 1;
              _uiMake_Tx ( ( DIT * ) x ) ;
              WRESIZE ( T ) ;
              uiCleanButtonBoxImages ( x ) ;
              _uiMake_Tx ( ( DIT * ) x ) ;
              break;
              case 'T':
              T = ( DIT * ) x;
              T->hide = 1;
              _uiMake_Ta ( ( DIT * ) x ) ;
              WRESIZE ( T ) ;
              uiCleanButtonBoxImages ( x ) ;
              _uiMake_Ta ( ( DIT * ) x ) ;
              break;
              case 'h':
              H = ( DIL * ) x;
              H->hide = 1;
              _uiMake_N ( ( DIL * ) x ) ;
              WRESIZE ( H ) ;
              uiCleanButtonBoxImages ( x ) ;
              _uiMake_N ( ( DIL * ) x ) ;
              break;
              case 'H':
              h = ( DILN * ) x;
              h->hide = 1;
              _uiMake_H ( ( DILN * ) x ) ;
              WRESIZE ( h ) ;
              _uiMake_H ( ( DILN * ) x ) ;
              break;
              case 'N':
              n = ( DIBN * ) x;
              n->hide = 1;
              _uiMake_B ( ( DIBN * ) x ) ;
              WRESIZE ( n ) ;
              _uiMake_B ( ( DIBN * ) x ) ;
              break;
              case 'n':
              N = ( DIN * ) x;
              N->hide = 1;
              _uiMake_N ( ( DIN * ) x ) ;
              WRESIZE ( N ) ;
              uiCleanButtonBoxImages ( x ) ;
              _uiMake_N ( ( DIN * ) x ) ;
              break;
              case 'b':
              b = ( DIB * ) x;
              b->hide = 1;
              _uiMake_B ( ( DIBN * ) x ) ;
              WRESIZE ( b ) ;
              _uiMake_B ( ( DIBN * ) x ) ;
              break;
              case 'f':
              f = ( DIF * ) x;
              f->hide = 1;
              _uiMake_SF ( ( DIF * ) x ) ;
              WRESIZE ( f ) ;
              _uiMake_SF ( ( DIF * ) x ) ;
              break;
              case 'P':
              P = ( DIHB * ) x;
              P->hide = 1;
              _uiMake_SH ( ( DIHB * ) x ) ;
              WRESIZE ( P ) ;
              _uiMake_SH ( ( DIHB * ) x ) ;
              break;
              case 'd':
              d = ( DID * ) x;
              d->hide = 1;
              _uiMake_SD ( ( DID * ) x ) ;
              WRESIZE ( d ) ;
              _uiMake_SD ( ( DID * ) x ) ;
              break;
              case 'w':
              w = ( DIW * ) x;
              w->hide = 1;
              _uiMake_W ( ( DIW * ) x ) ;
              WRESIZE ( w ) ;
              _uiMake_W ( ( DIW * ) x ) ;
              break;
              case 'e':
              e = ( DIE * ) x;
              e->hide = 1;
              _uiMake_E ( ( DIE * ) x ) ;
              WRESIZE ( e ) ;
              _uiMake_E ( ( DIE * ) x ) ;
              break;
              case 's':
              s = ( DIS * ) x;
              s->hide = 1;
              _uiMake_MS ( ( DIS * ) x ) ;
              WRESIZE ( s ) ;
              _uiMake_MS ( ( DIS * ) x ) ;
              break;
              case 'g':
              g = ( DIG * ) x;
              g->hide = 1;
              _uiMake_G ( ( DIG * ) x ) ;
              WRESIZE ( g ) ;
              _uiMake_G ( ( DIG * ) x ) ;
              break;
              case 'm':
              case 'M':
              case 'B':
              m = ( DIM * ) x;
              m->hide = 1;
              _uiMake_M ( ( DIM * ) x ) ;
              WRESIZE ( m ) ;
              _uiMake_M ( ( DIM * ) x ) ;
              break;
              default:
              break;
          }
      }
      ret = 1;
      return ret;
  }
  int kgShiftGrp ( void *Tmp , int grpid , int xs , int ys ) {
      DIX *x;
      int hide = 0 , hideo = 0 , ret = 0;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      Dlink *Grp;
      Dlink *Gpt;
      WIDGETGRP *pt = NULL;
      DIRA *r; DICH *c; DIY *y;DIT *T; DII *I;
      DIV *v; DIZ *z; DIO *o; DIP *p;DILN *h;DIBN *n; DIB *b;
      DIF *f; DIHB *P;DID *d; DIW *w;
      DIE *e; DIS *s; DIG *g; DIM *m;
      DIL *H; DIN *N;
      Grp = ( Dlink * ) D->GrpList;
      if ( Grp == NULL ) {
          return 0;
      }
      if ( Dcount ( Grp ) < grpid ) return 0;
      if ( grpid < 1 ) return 0;
      Resetlink ( Grp ) ;
      Dposition ( Grp , grpid ) ;
      pt = ( WIDGETGRP * ) Getrecord ( Grp ) ;
      Gpt = ( Dlink * ) pt->wlist;
      Resetlink ( Gpt ) ;
      while ( ( x = ( DIX * ) Getrecord ( Gpt ) ) != NULL ) {
          x->x1 += xs;
          x->y1 += ys;
          x->x2 += xs;
          x->y2 += ys;
      }
      ret = 1;
      return ret;
  }
  int kgMoveWidget ( void *Tmp , int item , int x1 , int y1 ) {
      DIX *x;
      int hide = 0 , hideo = 0 , ret = 0 , xl , yl;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      DIRA *r; DICH *c; DIY *y;DIT *T; DII *I;
      DIV *v; DIZ *z; DIO *o; DIP *p;DILN *h;DIBN *n; DIB *b;
      DIF *f; DIHB *P;DID *d; DIW *w;
      DIE *e; DIS *s; DIG *g; DIM *m;
      DIL *H; DIN *N;
      if ( item < 0 ) return 0;
      x = ( DIX * ) kgGetWidget ( Tmp , item ) ;
      if ( x->D != Tmp ) {
          WMOVE_ ( x ) ;
          return 1;
      }
      if ( x != NULL ) {
          switch ( x->code ) {
              case 'x':
              x->hide = 1;
              _uiMake_X ( x ) ;
              WMOVE ( x ) ;
              _uiMake_X ( x ) ;
              break;
              case 'r':
              r = ( DIRA * ) x;
              r->hide = 1;
              _uiMake_RadioButton ( r ) ;
              WMOVE ( r ) ;
              _uiMake_RadioButton ( r ) ;
              break;
              case 'c':
              c = ( DICH * ) x;
              c->hide = 1;
              _uiMake_CheckBox ( ( DICH * ) x ) ;
              WMOVE ( c ) ;
              _uiMake_CheckBox ( ( DICH * ) x ) ;
              break;
              case 'y':
              y = ( DIY * ) x;
              y->hide = 1;
              _uiMake_Y ( ( DIY * ) x ) ;
              WMOVE ( y ) ;
              _uiMake_Y ( ( DIY * ) x ) ;
              break;
              case 'v':
              v = ( DIV * ) x;
              v->hide = 1;
              _uiMake_V ( ( DIV * ) x ) ;
              WMOVE ( v ) ;
              _uiMake_V ( ( DIV * ) x ) ;
              break;
              case 'z':
              z = ( DIZ * ) x;
              z->hide = 1;
              _uiMake_Z ( ( DIZ * ) x ) ;
              WMOVE ( z ) ;
              _uiMake_Z ( ( DIZ * ) x ) ;
              break;
              case 'o':
              o = ( DIO * ) x;
              o->hide = 1;
              _uiMake_O ( ( DIO * ) x ) ;
              WMOVE ( o ) ;
              _uiMake_O ( ( DIO * ) x ) ;
              break;
              case 'p': /* new for xpm display */
              p = ( DIP * ) x;
              p->hide = 1;
              _uiMake_P ( ( DIP * ) x ) ;
              WMOVE ( p ) ;
              _uiMake_P ( ( DIP * ) x ) ;
              break;
              case 'i': /* info box */
              I = ( DII * ) x;
              I->hide = 1;
              _uiMake_I ( ( DII * ) x ) ;
              WMOVE ( I ) ;
              _uiMake_I ( ( DII * ) x ) ;
              break;
              case 't':
              T = ( DIT * ) x;
              T->hide = 1;
              _uiMake_Tx ( ( DIT * ) x ) ;
              WMOVE ( T ) ;
              uiCleanTextBoxImages ( T ) ;
              _uiMake_Tx ( ( DIT * ) x ) ;
              break;
              case 'T':
              T = ( DIT * ) x;
              T->hide = 1;
              _uiMake_Ta ( ( DIT * ) x ) ;
              WMOVE ( T ) ;
              uiCleanTextBoxImages ( T ) ;
              _uiMake_Ta ( ( DIT * ) x ) ;
              break;
              case 'h':
              H = ( DIL * ) x;
              H->hide = 1;
              _uiMake_N ( ( DIL * ) x ) ;
              WMOVE ( H ) ;
              uiCleanButtonBoxImages ( x ) ;
              _uiMake_N ( ( DIL * ) x ) ;
              break;
              case 'H':
              h = ( DILN * ) x;
              h->hide = 1;
              _uiMake_H ( ( DILN * ) x ) ;
              WMOVE ( h ) ;
              _uiMake_H ( ( DILN * ) x ) ;
              break;
              case 'N':
              n = ( DIBN * ) x;
              n->hide = 1;
              _uiMake_B ( ( DIBN * ) x ) ;
              WMOVE ( n ) ;
              _uiMake_B ( ( DIBN * ) x ) ;
              break;
              case 'n':
              N = ( DIN * ) x;
              N->hide = 1;
              _uiMake_N ( ( DIN * ) x ) ;
              WMOVE ( N ) ;
              uiCleanButtonBoxImages ( x ) ;
              _uiMake_N ( ( DIN * ) x ) ;
              break;
              case 'b':
              b = ( DIB * ) x;
              b->hide = 1;
              _uiMake_B ( ( DIBN * ) x ) ;
              WMOVE ( b ) ;
              _uiMake_B ( ( DIBN * ) x ) ;
              break;
              case 'f':
              f = ( DIF * ) x;
              f->hide = 1;
              _uiMake_SF ( ( DIF * ) x ) ;
              WMOVE ( f ) ;
              _uiMake_SF ( ( DIF * ) x ) ;
              break;
              case 'P':
              P = ( DIHB * ) x;
              P->hide = 1;
              _uiMake_SH ( ( DIHB * ) x ) ;
              WMOVE ( P ) ;
              _uiMake_SH ( ( DIHB * ) x ) ;
              break;
              case 'd':
              d = ( DID * ) x;
              d->hide = 1;
              _uiMake_SD ( ( DID * ) x ) ;
              WMOVE ( d ) ;
              _uiMake_SD ( ( DID * ) x ) ;
              break;
              case 'w':
              w = ( DIW * ) x;
              w->hide = 1;
              _uiMake_W ( ( DIW * ) x ) ;
              WMOVE ( w ) ;
              _uiMake_W ( ( DIW * ) x ) ;
              break;
              case 'e':
              e = ( DIE * ) x;
              e->hide = 1;
              _uiMake_E ( ( DIE * ) x ) ;
              WMOVE ( e ) ;
              _uiMake_E ( ( DIE * ) x ) ;
              break;
              case 's':
              s = ( DIS * ) x;
              s->hide = 1;
              _uiMake_MS ( ( DIS * ) x ) ;
              WMOVE ( s ) ;
              _uiMake_MS ( ( DIS * ) x ) ;
              break;
              case 'g':
              g = ( DIG * ) x;
              g->hide = 1;
              _uiMake_G ( ( DIG * ) x ) ;
              WMOVE ( g ) ;
              _uiMake_G ( ( DIG * ) x ) ;
              break;
              case 'm':
              case 'M':
              case 'B':
              m = ( DIM * ) x;
              m->hide = 1;
              _uiMake_M ( ( DIM * ) x ) ;
              WMOVE ( m ) ;
              _uiMake_M ( ( DIM * ) x ) ;
              break;
              default:
              break;
          }
      }
      ret = 1;
      return ret;
  }
  int kgShiftWidget ( void *Tmp , int item , int xs , int ys ) {
      DIX *x;
      int hide = 0 , hideo = 0 , ret = 0 , xl , yl , x1 , y1;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      DIRA *r; DICH *c; DIY *y;DIT *T; DII *I;
      DIV *v; DIZ *z; DIO *o; DIP *p;DILN *h;DIBN *n; DIB *b;
      DIF *f; DIHB *P;DID *d; DIW *w;
      DIE *e; DIS *s; DIG *g; DIM *m;
      DIL *H; DIN *N;
      if ( item < 0 ) return 0;
      x = ( DIX * ) kgGetWidget ( Tmp , item ) ;
      x1 = x->x1+xs;
      y1 = x->y1+ys;
      if ( x->D != Tmp ) {
          WMOVE_ ( x ) ;
          return 1;
      }
      if ( x != NULL ) {
          switch ( x->code ) {
              case 'x':
              x->hide = 1;
              _uiMake_X ( x ) ;
              WMOVE ( x ) ;
              _uiMake_X ( x ) ;
              break;
              case 'r':
              r = ( DIRA * ) x;
              r->hide = 1;
              _uiMake_RadioButton ( r ) ;
              WMOVE ( r ) ;
              _uiMake_RadioButton ( r ) ;
              break;
              case 'c':
              c = ( DICH * ) x;
              c->hide = 1;
              _uiMake_CheckBox ( ( DICH * ) x ) ;
              WMOVE ( c ) ;
              _uiMake_CheckBox ( ( DICH * ) x ) ;
              break;
              case 'y':
              y = ( DIY * ) x;
              y->hide = 1;
              _uiMake_Y ( ( DIY * ) x ) ;
              WMOVE ( y ) ;
              _uiMake_Y ( ( DIY * ) x ) ;
              break;
              case 'v':
              v = ( DIV * ) x;
              v->hide = 1;
              _uiMake_V ( ( DIV * ) x ) ;
              WMOVE ( v ) ;
              _uiMake_V ( ( DIV * ) x ) ;
              break;
              case 'z':
              z = ( DIZ * ) x;
              z->hide = 1;
              _uiMake_Z ( ( DIZ * ) x ) ;
              WMOVE ( z ) ;
              _uiMake_Z ( ( DIZ * ) x ) ;
              break;
              case 'o':
              o = ( DIO * ) x;
              o->hide = 1;
              _uiMake_O ( ( DIO * ) x ) ;
              WMOVE ( o ) ;
              _uiMake_O ( ( DIO * ) x ) ;
              break;
              case 'p': /* new for xpm display */
              p = ( DIP * ) x;
              p->hide = 1;
              _uiMake_P ( ( DIP * ) x ) ;
              WMOVE ( p ) ;
              _uiMake_P ( ( DIP * ) x ) ;
              break;
              case 'i': /* info box */
              I = ( DII * ) x;
              I->hide = 1;
              _uiMake_I ( ( DII * ) x ) ;
              WMOVE ( I ) ;
              _uiMake_I ( ( DII * ) x ) ;
              break;
              case 't':
              T = ( DIT * ) x;
              T->hide = 1;
              _uiMake_Tx ( ( DIT * ) x ) ;
              WMOVE ( T ) ;
              uiCleanTextBoxImages ( T ) ;
              _uiMake_Tx ( ( DIT * ) x ) ;
              break;
              case 'T':
              T = ( DIT * ) x;
              T->hide = 1;
              _uiMake_Ta ( ( DIT * ) x ) ;
              WMOVE ( T ) ;
              uiCleanTextBoxImages ( T ) ;
              _uiMake_Ta ( ( DIT * ) x ) ;
              break;
              case 'h':
              H = ( DIL * ) x;
              H->hide = 1;
              _uiMake_N ( ( DIL * ) x ) ;
              WMOVE ( H ) ;
              uiCleanButtonBoxImages ( x ) ;
              _uiMake_N ( ( DIL * ) x ) ;
              break;
              case 'H':
              h = ( DILN * ) x;
              h->hide = 1;
              _uiMake_H ( ( DILN * ) x ) ;
              WMOVE ( h ) ;
              _uiMake_H ( ( DILN * ) x ) ;
              break;
              case 'N':
              n = ( DIBN * ) x;
              n->hide = 1;
              _uiMake_B ( ( DIBN * ) x ) ;
              WMOVE ( n ) ;
              _uiMake_B ( ( DIBN * ) x ) ;
              break;
              case 'n':
              N = ( DIN * ) x;
              N->hide = 1;
              _uiMake_N ( ( DIN * ) x ) ;
              WMOVE ( N ) ;
              uiCleanButtonBoxImages ( x ) ;
              _uiMake_N ( ( DIN * ) x ) ;
              break;
              case 'b':
              b = ( DIB * ) x;
              b->hide = 1;
              _uiMake_B ( ( DIBN * ) x ) ;
              WMOVE ( b ) ;
              _uiMake_B ( ( DIBN * ) x ) ;
              break;
              case 'f':
              f = ( DIF * ) x;
              f->hide = 1;
              _uiMake_SF ( ( DIF * ) x ) ;
              WMOVE ( f ) ;
              _uiMake_SF ( ( DIF * ) x ) ;
              break;
              case 'P':
              P = ( DIHB * ) x;
              P->hide = 1;
              _uiMake_SH ( ( DIHB * ) x ) ;
              WMOVE ( P ) ;
              _uiMake_SH ( ( DIHB * ) x ) ;
              break;
              case 'd':
              d = ( DID * ) x;
              d->hide = 1;
              _uiMake_SD ( ( DID * ) x ) ;
              WMOVE ( d ) ;
              _uiMake_SD ( ( DID * ) x ) ;
              break;
              case 'w':
              w = ( DIW * ) x;
              w->hide = 1;
              _uiMake_W ( ( DIW * ) x ) ;
              WMOVE ( w ) ;
              _uiMake_W ( ( DIW * ) x ) ;
              break;
              case 'e':
              e = ( DIE * ) x;
              e->hide = 1;
              _uiMake_E ( ( DIE * ) x ) ;
              WMOVE ( e ) ;
              _uiMake_E ( ( DIE * ) x ) ;
              break;
              case 's':
              s = ( DIS * ) x;
              s->hide = 1;
              _uiMake_MS ( ( DIS * ) x ) ;
              WMOVE ( s ) ;
              _uiMake_MS ( ( DIS * ) x ) ;
              break;
              case 'g':
              g = ( DIG * ) x;
              g->hide = 1;
              _uiMake_G ( ( DIG * ) x ) ;
              WMOVE ( g ) ;
              _uiMake_G ( ( DIG * ) x ) ;
              break;
              case 'm':
              case 'M':
              case 'B':
              m = ( DIM * ) x;
              m->hide = 1;
              _uiMake_M ( ( DIM * ) x ) ;
              WMOVE ( m ) ;
              _uiMake_M ( ( DIM * ) x ) ;
              break;
              default:
              break;
          }
      }
      ret = 1;
      return ret;
  }
  int kgResizeWidget ( void *Tmp , int item , int xl , int yl ) {
      DIX *x;
      int hide = 0 , hideo = 0 , ret = 0;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      DIRA *r; DICH *c; DIY *y;DIT *T; DII *I;
      DIV *v; DIZ *z; DIO *o; DIP *p;DILN *h;DIBN *n; DIB *b;
      DIF *f; DIHB *P;DID *d; DIW *w;
      DIE *e; DIS *s; DIG *g; DIM *m;
      DIL *H; DIN *N;
      if ( item < 0 ) return 0;
      x = ( DIX * ) kgGetWidget ( Tmp , item ) ;
      if ( x != NULL ) {
          if ( x->D != Tmp ) {
              WRESIZE_ ( x ) ;
              return 1;
          }
          switch ( x->code ) {
              case 'x':
              x->hide = 1;
              _uiMake_X ( x ) ;
              WRESIZE ( x ) ;
              _uiMake_X ( x ) ;
              break;
              case 'r':
              r = ( DIRA * ) x;
              r->hide = 1;
              _uiMake_RadioButton ( r ) ;
              WRESIZE ( r ) ;
              _uiMake_RadioButton ( r ) ;
              break;
              case 'c':
              c = ( DICH * ) x;
              c->hide = 1;
              _uiMake_CheckBox ( ( DICH * ) x ) ;
              WRESIZE ( c ) ;
              _uiMake_CheckBox ( ( DICH * ) x ) ;
              break;
              case 'y':
              y = ( DIY * ) x;
              y->hide = 1;
              _uiMake_Y ( ( DIY * ) x ) ;
              WRESIZE ( y ) ;
              _uiMake_Y ( ( DIY * ) x ) ;
              break;
              case 'v':
              v = ( DIV * ) x;
              v->hide = 1;
              _uiMake_V ( ( DIV * ) x ) ;
              WRESIZE ( v ) ;
              _uiMake_V ( ( DIV * ) x ) ;
              break;
              case 'z':
              z = ( DIZ * ) x;
              z->hide = 1;
              _uiMake_Z ( ( DIZ * ) x ) ;
              WRESIZE ( z ) ;
              _uiMake_Z ( ( DIZ * ) x ) ;
              break;
              case 'o':
              o = ( DIO * ) x;
              o->hide = 1;
              _uiMake_O ( ( DIO * ) x ) ;
              WRESIZE ( o ) ;
              _uiMake_O ( ( DIO * ) x ) ;
              break;
              case 'p': /* new for xpm display */
              p = ( DIP * ) x;
              p->hide = 1;
              _uiMake_P ( ( DIP * ) x ) ;
              WRESIZE ( p ) ;
              _uiMake_P ( ( DIP * ) x ) ;
              break;
              case 'i': /* info box */
              I = ( DII * ) x;
              I->hide = 1;
              _uiMake_I ( ( DII * ) x ) ;
              WRESIZE ( I ) ;
              _uiMake_I ( ( DII * ) x ) ;
              break;
              case 't':
              T = ( DIT * ) x;
              T->hide = 1;
              _uiMake_Tx ( ( DIT * ) x ) ;
              WRESIZE ( T ) ;
              uiCleanTextBoxImages ( T ) ;
              _uiMake_Tx ( ( DIT * ) x ) ;
              break;
              case 'T':
              T = ( DIT * ) x;
              T->hide = 1;
              _uiMake_Ta ( ( DIT * ) x ) ;
              WRESIZE ( T ) ;
              uiCleanTextBoxImages ( T ) ;
              _uiMake_Ta ( ( DIT * ) x ) ;
              break;
              case 'h':
              H = ( DIL * ) x;
              H->hide = 1;
              _uiMake_N ( ( DIL * ) x ) ;
              WRESIZE ( H ) ;
              uiCleanButtonBoxImages ( x ) ;
              _uiMake_N ( ( DIL * ) x ) ;
              break;
              case 'H':
              h = ( DILN * ) x;
              h->hide = 1;
              _uiMake_H ( ( DILN * ) x ) ;
              WRESIZE ( h ) ;
              _uiMake_H ( ( DILN * ) x ) ;
              break;
              case 'N':
              n = ( DIBN * ) x;
              n->hide = 1;
              _uiMake_B ( ( DIBN * ) x ) ;
              WRESIZE ( n ) ;
              _uiMake_B ( ( DIBN * ) x ) ;
              break;
              case 'n':
              N = ( DIN * ) x;
              N->hide = 1;
              _uiMake_N ( ( DIN * ) x ) ;
              WRESIZE ( N ) ;
              uiCleanButtonBoxImages ( x ) ;
              _uiMake_N ( ( DIN * ) x ) ;
              break;
              case 'b':
              b = ( DIB * ) x;
              b->hide = 1;
              _uiMake_B ( ( DIBN * ) x ) ;
              WRESIZE ( b ) ;
              _uiMake_B ( ( DIBN * ) x ) ;
              break;
              case 'f':
              f = ( DIF * ) x;
              f->hide = 1;
              _uiMake_SF ( ( DIF * ) x ) ;
              WRESIZE ( f ) ;
              _uiMake_SF ( ( DIF * ) x ) ;
              break;
              case 'P':
              P = ( DIHB * ) x;
              P->hide = 1;
              _uiMake_SH ( ( DIHB * ) x ) ;
              WRESIZE ( P ) ;
              _uiMake_SH ( ( DIHB * ) x ) ;
              break;
              case 'd':
              d = ( DID * ) x;
              d->hide = 1;
              _uiMake_SD ( ( DID * ) x ) ;
              WRESIZE ( d ) ;
              _uiMake_SD ( ( DID * ) x ) ;
              break;
              case 'w':
              w = ( DIW * ) x;
              w->hide = 1;
              _uiMake_W ( ( DIW * ) x ) ;
              WRESIZE ( w ) ;
              _uiMake_W ( ( DIW * ) x ) ;
              break;
              case 'e':
              e = ( DIE * ) x;
              e->hide = 1;
              _uiMake_E ( ( DIE * ) x ) ;
              WRESIZE ( e ) ;
              _uiMake_E ( ( DIE * ) x ) ;
              break;
              case 's':
              s = ( DIS * ) x;
              s->hide = 1;
              _uiMake_MS ( ( DIS * ) x ) ;
              WRESIZE ( s ) ;
              _uiMake_MS ( ( DIS * ) x ) ;
              break;
              case 'g':
              g = ( DIG * ) x;
              g->hide = 1;
              _uiMake_G ( ( DIG * ) x ) ;
              WRESIZE ( g ) ;
              _uiMake_G ( ( DIG * ) x ) ;
              break;
              case 'm':
              case 'M':
              case 'B':
              m = ( DIM * ) x;
              m->hide = 1;
              _uiMake_M ( ( DIM * ) x ) ;
              WRESIZE ( m ) ;
              _uiMake_M ( ( DIM * ) x ) ;
              break;
              default:
              break;
          }
      }
      ret = 1;
      return ret;
  }
  int kgUpdateGrp ( void *Tmp , int grpid ) {
      int *wid;
      DIX *x;
      int hide = 0 , hideo = 0 , ret = 0;
      DIALOG *D;
      D = ( DIALOG * ) Tmp;
      Dlink *Grp;
      Dlink *Gpt;
      WIDGETGRP *pt = NULL;
      Grp = ( Dlink * ) D->GrpList;
      if ( Grp == NULL ) {
          return 0;
      }
      if ( Dcount ( Grp ) < grpid ) return 0;
      if ( grpid < 1 ) return 0;
      Resetlink ( Grp ) ;
      Dposition ( Grp , grpid ) ;
      pt = ( WIDGETGRP * ) Getrecord ( Grp ) ;
      Gpt = ( Dlink * ) pt->wlist;
      Resetlink ( Gpt ) ;
      if ( pt->hide == 0 ) {
          while ( ( x = ( DIX * ) Getrecord ( Gpt ) ) != NULL ) {
              switch ( x->code ) {
                  case 'x':
                  _uiMake_X ( x ) ;
                  break;
                  case 'r':
                  _uiMake_RadioButton ( ( DIRA * ) x ) ;
                  break;
                  case 'c':
                  _uiMake_CheckBox ( ( DICH * ) x ) ;
                  break;
                  case 'y':
                  _uiMake_Y ( ( DIY * ) x ) ;
                  break;
                  case 'v':
                  _uiMake_V ( ( DIV * ) x ) ;
                  break;
                  case 'z':
                  _uiMake_Z ( ( DIZ * ) x ) ;
                  break;
                  case 'o':
                  _uiMake_O ( ( DIO * ) x ) ;
                  break;
                  case 'p': /* new for xpm display */
                  _uiMake_P ( ( DIP * ) x ) ;
                  break;
                  case 'i': /* info box */
                  _uiMake_I ( ( DII * ) x ) ;
                  break;
                  case 't':
                  _uiMake_Tx ( ( DIT * ) x ) ;
                  break;
                  case 'T':
                  _uiMake_Ta ( ( DIT * ) x ) ;
                  break;
                  case 'h':
                  _uiMake_N ( ( DIL * ) x ) ;
                  break;
                  case 'H':
                  _uiMake_H ( ( DILN * ) x ) ;
                  break;
                  case 'n':
                  _uiMake_N ( ( DIN * ) x ) ;
                  break;
                  case 'b':
                  case 'N':
                  _uiMake_B ( ( DIBN * ) x ) ;
                  break;
                  case 'f':
                  _uiMake_SF ( ( DIF * ) x ) ;
                  break;
                  case 'P':
                  _uiMake_SH ( ( DIHB * ) x ) ;
                  break;
                  case 'd':
                  _uiMake_SD ( ( DID * ) x ) ;
                  break;
                  case 'w':
                  _uiMake_W ( ( DIW * ) x ) ;
                  break;
                  case 'e':
                  _uiMake_E ( ( DIE * ) x ) ;
                  break;
                  case 's':
                  _uiMake_MS ( ( DIS * ) x ) ;
                  break;
                  case 'g':
                  _uiMake_G ( ( DIG * ) x ) ;
                  break;
                  case 'm':
                  case 'M':
                  case 'B':
                  _uiMake_M ( ( DIM * ) x ) ;
                  break;
                  default:
                  break;
              }
          }
      }
      else {
          Dend ( Gpt ) ;
          while ( ( x = ( DIX * ) Getrecordrev ( Gpt ) ) != NULL ) {
              switch ( x->code ) {
                  case 'x':
                  _uiMake_X ( x ) ;
                  break;
                  case 'r':
                  _uiMake_RadioButton ( ( DIRA * ) x ) ;
                  break;
                  case 'c':
                  _uiMake_CheckBox ( ( DICH * ) x ) ;
                  break;
                  case 'y':
                  _uiMake_Y ( ( DIY * ) x ) ;
                  break;
                  case 'v':
                  _uiMake_V ( ( DIV * ) x ) ;
                  break;
                  case 'z':
                  _uiMake_Z ( ( DIZ * ) x ) ;
                  break;
                  case 'o':
                  _uiMake_O ( ( DIO * ) x ) ;
                  break;
                  case 'p': /* new for xpm display */
                  _uiMake_P ( ( DIP * ) x ) ;
                  break;
                  case 'i': /* info box */
                  _uiMake_I ( ( DII * ) x ) ;
                  break;
                  case 't':
                  _uiMake_Tx ( ( DIT * ) x ) ;
                  break;
                  case 'T':
                  _uiMake_Ta ( ( DIT * ) x ) ;
                  break;
                  case 'h':
                  _uiMake_N ( ( DIL * ) x ) ;
                  break;
                  case 'H':
                  _uiMake_H ( ( DILN * ) x ) ;
                  break;
                  case 'n':
                  _uiMake_N ( ( DIN * ) x ) ;
                  break;
                  case 'b':
                  case 'N':
                  _uiMake_B ( ( DIBN * ) x ) ;
                  break;
                  case 'f':
                  _uiMake_SF ( ( DIF * ) x ) ;
                  break;
                  case 'P':
                  _uiMake_SH ( ( DIHB * ) x ) ;
                  break;
                  case 'd':
                  _uiMake_SD ( ( DID * ) x ) ;
                  break;
                  case 'w':
                  _uiMake_W ( ( DIW * ) x ) ;
                  break;
                  case 'e':
                  _uiMake_E ( ( DIE * ) x ) ;
                  break;
                  case 's':
                  _uiMake_MS ( ( DIS * ) x ) ;
                  break;
                  case 'g':
                  _uiMake_G ( ( DIG * ) x ) ;
                  break;
                  case 'm':
                  case 'M':
                  case 'B':
                  _uiMake_M ( ( DIM * ) x ) ;
                  break;
                  default:
                  break;
              }
          }
      }
      kgUpdateOn ( Tmp ) ;
      ret = 1;
      return ret;
  }
  int Dgetbrowseritem ( void *Tmp , int menu ) {
      DIA *D;DIW *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .w;
      B = ( BRW_STR * ) ( E->bwsr ) ;
      return B->df;
  }
  char **Dgetbrowserlist ( void *Tmp , int menu ) {
      DIA *D;DIW *E;
      BRW_STR *B;
      D = ( ( DIALOG * ) Tmp )->d;
      E = D [ menu ] .w;
      B = ( BRW_STR * ) ( E->bwsr ) ;
      return B->menu;
  }
  void Dsetprogressbar ( void *Tmp , int id , int val ) {
      DIALOG *D;
      DIO *o;
      D = ( DIALOG * ) Tmp;
      o = D->d [ id ] .o;
      o->percent = val;
      _uiMake_O ( o ) ;
      uiUpdateOn ( Tmp ) ;
  }
  void Dsetprogressbarvisibility ( void *Tmp , int id , int hide ) {
      DIALOG *D;
      DIO *o;
      D = ( DIALOG * ) Tmp;
      o = D->d [ id ] .o;
      if ( hide == o->hide ) return;
      o->hide = hide;
      if ( D != NULL ) {
          kgUpdateWidget ( o ) ;
      }
  }
  void kgSetProgressBar ( void *Wid , int val ) {
      DIALOG *D;
      DIO *o;
      o = ( DIO * ) Wid;
      D = ( DIALOG * ) o->D;
      if ( o->hide ) return;
      if ( val == o->oldval ) return;
      o->percent = val;
      if ( D != NULL ) {
          kgUpdateWidget ( Wid ) ;
//    _uiMake_O(o);
          kgUpdateOn ( D ) ;
      }
  }
  int kgChangeButtonColor ( void *Widget , int butno , int red , int green , int blue ) \
  {
/* changes color of the specified button
   and returns old color
*/
      int color , ocolor;
      DIN *B;
      BUT_STR *buts;
      B = ( DIN * ) Widget;
      buts = ( BUT_STR * ) B->buts;
      color = blue+green*1000+red*1000000;
      color = -color;
      ocolor = buts [ butno ] .bkgr;
      buts [ butno ] .bkgr = color;
      if ( B->item != -1 ) {
#if 0
          if ( buts [ 0 ] .imgn != NULL ) kgFreeImage ( buts [ 0 ] .imgn ) ;
          buts [ 0 ] .imgn = NULL;
#else
          if ( buts [ butno ] .imgn != NULL ) kgFreeImage ( buts [ butno ] .imgn ) ;
          buts [ butno ] .imgn = NULL;
          if ( buts [ butno ] .imgp != NULL ) kgFreeImage ( buts [ butno ] .imgp ) ;
          buts [ butno ] .imgp = NULL;
          if ( buts [ butno ] .imgh != NULL ) kgFreeImage ( buts [ butno ] .imgh ) ;
          buts [ butno ] .imgh = NULL;
#endif
      }
      return ocolor;
  }
  void * kgChangeButtonNormalImage ( void *Widget , int butno , void *img ) {
/* changes Normal Image of the specified button
   and returns old image
*/
      void *oimg;
      DIN *B;
      BUT_STR *buts;
      B = ( DIN * ) Widget;
      buts = ( BUT_STR * ) B->buts;
      oimg = buts [ butno ] .xpmn;
      buts [ butno ] .xpmn = img;
      if ( B->item != -1 ) {
#if 0
          if ( buts [ 0 ] .imgn != NULL ) kgFreeImage ( buts [ 0 ] .imgn ) ;
          buts [ 0 ] .imgn = NULL;
#else
          if ( buts [ butno ] .imgn != NULL ) kgFreeImage ( buts [ butno ] .imgn ) ;
          buts [ butno ] .imgn = NULL;
          if ( buts [ butno ] .imgp != NULL ) kgFreeImage ( buts [ butno ] .imgp ) ;
          buts [ butno ] .imgp = NULL;
          if ( buts [ butno ] .imgh != NULL ) kgFreeImage ( buts [ butno ] .imgh ) ;
          buts [ butno ] .imgh = NULL;
#endif
      }
      return oimg;
  }
  void * kgChangeButtonPressedImage ( void *Widget , int butno , void *img ) {
/* changes Pressed Image of the specified button
   and returns old image
*/
      void *oimg;
      DIN *B;
      BUT_STR *buts;
      B = ( DIN * ) Widget;
      buts = ( BUT_STR * ) B->buts;
      oimg = buts [ butno ] .xpmp;
      buts [ butno ] .xpmp = img;
      if ( B->item != -1 ) {
#if 0
          if ( buts [ 0 ] .imgn != NULL ) kgFreeImage ( buts [ 0 ] .imgn ) ;
          buts [ 0 ] .imgn = NULL;
#else
          if ( buts [ butno ] .imgn != NULL ) kgFreeImage ( buts [ butno ] .imgn ) ;
          buts [ butno ] .imgn = NULL;
          if ( buts [ butno ] .imgp != NULL ) kgFreeImage ( buts [ butno ] .imgp ) ;
          buts [ butno ] .imgp = NULL;
          if ( buts [ butno ] .imgh != NULL ) kgFreeImage ( buts [ butno ] .imgh ) ;
          buts [ butno ] .imgh = NULL;
#endif
      }
      return oimg;
  }
  void * kgChangeButtonHighImage ( void *Widget , int butno , void *img ) {
/* changes High lighted Image of the specified button
   and returns old image
*/
      void *oimg;
      DIN *B;
      BUT_STR *buts;
      B = ( DIN * ) Widget;
      buts = ( BUT_STR * ) B->buts;
      oimg = buts [ butno ] .xpmh;
      buts [ butno ] .xpmh = img;
      if ( B->item != -1 ) {
#if 0
          if ( buts [ 0 ] .imgn != NULL ) kgFreeImage ( buts [ 0 ] .imgn ) ;
          buts [ 0 ] .imgn = NULL;
#else
          if ( buts [ butno ] .imgn != NULL ) kgFreeImage ( buts [ butno ] .imgn ) ;
          buts [ butno ] .imgn = NULL;
          if ( buts [ butno ] .imgp != NULL ) kgFreeImage ( buts [ butno ] .imgp ) ;
          buts [ butno ] .imgp = NULL;
          if ( buts [ butno ] .imgh != NULL ) kgFreeImage ( buts [ butno ] .imgh ) ;
          buts [ butno ] .imgh = NULL;
#endif
      }
      return oimg;
  }
  void kgChangeButtonTitle ( void *Widget , int butno , char *title ) {
/* changes High lighted Image of the specified button
   and returns old image
*/
      void *oimg;
      DIN *B;
      BUT_STR *buts;
      B = ( DIN * ) Widget;
      buts = ( BUT_STR * ) B->buts;
      strcpy ( buts [ butno ] .title , title ) ;;
      if ( B->item != -1 ) {
#if 0
          if ( buts [ 0 ] .imgn != NULL ) kgFreeImage ( buts [ 0 ] .imgn ) ;
          buts [ 0 ] .imgn = NULL;
#else
          if ( buts [ butno ] .imgn != NULL ) kgFreeImage ( buts [ butno ] .imgn ) ;
          buts [ butno ] .imgn = NULL;
          if ( buts [ butno ] .imgp != NULL ) kgFreeImage ( buts [ butno ] .imgp ) ;
          buts [ butno ] .imgp = NULL;
          if ( buts [ butno ] .imgh != NULL ) kgFreeImage ( buts [ butno ] .imgh ) ;
          buts [ butno ] .imgh = NULL;
#endif
      }
      return ;
  }
  char * kgGetButtonTitle ( void *Widget , int butno ) {
/* changes High lighted Image of the specified button
   and returns old image
*/
      void *oimg;
      DIN *B;
      BUT_STR *buts;
      B = ( DIN * ) Widget;
      buts = ( BUT_STR * ) B->buts;
      return buts [ butno ] .title;
  }
  int kgGetSlideValue ( void *tmp ) {
      DIA *D;DIHB *X;
      S_STR *sptr;
      int df = 0;
      X = ( DIHB * ) tmp;
      switch ( X->code ) {
          case 'P':
          sptr = X->sptr;
          df = _ui_getdslidevalue ( sptr ) ;
          * ( X->df ) = df;
          df = * ( X->df ) ;
//    printf("df = %d\n",df);
          return df;
          default:
          fprintf ( stderr , "Invalid Widget: kgGetSlideValue\n" ) ;
          return df;
      }
      return 0;
  }
  int kgSetSlideValue ( void *tmp , int val ) {
      DIA *D;DIHB *X;
      int df = 0;
      X = ( DIHB * ) tmp;
      switch ( X->code ) {
          case 'P':
          if ( val < X->min ) val = X->min;
          if ( val > X->max ) val = X->max;
          * ( X->df ) = val;;
          df = * ( X->df ) ;
          kgUpdateWidget ( tmp ) ;
          return df;
          default:
          fprintf ( stderr , "Invalid Widget: kgSetSlideValue\n" ) ;
          return df;
      }
  }
  int kgGetDslideValue ( void *tmp ) {
      DIA *D;DID *X;
      S_STR *sptr;
      int df = 0;
      X = ( DID* ) tmp;
      switch ( X->code ) {
          case 'd':
          sptr = X->sptr;
          df = _ui_getdslidevalue ( sptr ) ;
          * ( X->df ) = df;
          df = * ( X->df ) ;
//    printf("df = %d\n",df);
          return df;
          default:
          fprintf ( stderr , "Invalid Widget: kgGetSlideValue\n" ) ;
          return df;
      }
      return 0;
  }
  int kgSetDslideValue ( void *tmp , int val ) {
      DIA *D;DID *X;
      int df = 0;
      X = ( DID * ) tmp;
      switch ( X->code ) {
          case 'd':
          if ( val < X->min ) val = X->min;
          if ( val > X->max ) val = X->max;
          * ( X->df ) = val;;
          df = * ( X->df ) ;
          kgUpdateWidget ( tmp ) ;
          return df;
          default:
          fprintf ( stderr , "Invalid Widget: kgSetSlideValue\n" ) ;
          return df;
      }
  }
  float kgGetFslideValue ( void *tmp ) {
      DIA *D;DIF *X;
      S_STR *sptr;
      float df = 0;
      X = ( DIF* ) tmp;
      switch ( X->code ) {
          case 'f':
          sptr = X->sptr;
          df = _ui_getfslidevalue ( sptr ) ;
//    *(X->df)= df;
//    printf("df = %f\n",df);
//    df = *(X->df);
          return df;
          default:
          fprintf ( stderr , "Invalid Widget: kgGetSlideValue\n" ) ;
          return df;
      }
      return 0;
  }
  float kgSetFslideValue ( void *tmp , float val ) {
      DIA *D;DIF *X;
      float df = 0;
      X = ( DIF * ) tmp;
      switch ( X->code ) {
          case 'f':
          if ( val < X->min ) val = X->min;
          if ( val > X->max ) val = X->max;
          * ( X->df ) = val;;
          df = * ( X->df ) ;
          kgUpdateWidget ( tmp ) ;
          return df;
          default:
          fprintf ( stderr , "Invalid Widget: kgSetSlideValue\n" ) ;
          return df;
      }
  }
  void kgLockUi ( void *tmp ) {
      DIALOG *D;
      D = ( DIALOG * ) tmp;
      pthread_mutex_lock ( & ( D->Lock ) ) ;
      return;
  }
  void kgUnlockUi ( void *tmp ) {
      DIALOG *D;
      D = ( DIALOG * ) tmp;
      pthread_mutex_unlock ( & ( D->Lock ) ) ;
      return;
  }
  void kgSetTextItemEcho ( void *Tmp , int item , int echo ) {
      DIT *T;
      T_ELMT *e;
      T = ( DIT * ) Tmp;
      e = T->elmt;
      if ( echo == 1 ) e [ item ] .noecho = 0;
      else e [ item ] .noecho = 1;
      kgUpdateWidget ( T ) ;
      return;
  }
  void *_uiGetWidgetBimg ( void *Widget ) {
      int i , ch , n , j;
      void *Bimg = NULL;
      DIT *t;
      t = ( DIT * ) Widget;
      if ( t != NULL ) {
          ch = ( t->code ) ;
//     printf("ch == %c\n",ch);
          switch ( ( int ) ch ) {
              case 'v':
              Bimg = ( ( ( DIV * ) ( t ) )->Bimg ) ;
              break;
              case 'z':
              Bimg = ( ( ( DIZ * ) ( t ) )->Bimg ) ;
              break;
              case 'o': /* progress bar */
              Bimg = ( ( ( DIO * ) ( t ) )->Bimg ) ;
              break;
              case 'p': /* new for xpm display */
              Bimg = ( ( ( DIP * ) ( t ) )->Bimg ) ;
              break;
              case 'i': /* info box */
              Bimg = ( ( ( DII * ) ( t ) )->Bimg ) ;
              break;
              case 't':
              Bimg = ( ( ( DIT * ) ( t ) )->Bimg ) ;
              break;
              case 'T':
              Bimg = ( ( ( DIT * ) ( t ) )->Bimg ) ;
              break;
              case 'h':
              Bimg = ( ( ( DIL * ) ( t ) )->Bimg ) ;
              break;
              case 'H':
              Bimg = ( ( ( DILN * ) ( t ) )->Bimg ) ;
              break;
              case 'N':
              Bimg = ( ( ( DIBN * ) ( t ) )->Bimg ) ;
              break;
              case 'n':
              Bimg = ( ( ( DIN * ) ( t ) )->Bimg ) ;
              break;
              case 'b':
              Bimg = ( ( ( DIB * ) ( t ) )->Bimg ) ;
              break;
              case 'f':
              Bimg = ( ( ( DIF * ) ( t ) )->Bimg ) ;
              break;
              case 'P':
              Bimg = ( ( ( DIHB * ) ( t ) )->Bimg ) ;
              break;
              case 'd':
              Bimg = ( ( ( DID * ) ( t ) )->Bimg ) ;
              break;
              case 'x':
              Bimg = ( ( ( DIX * ) ( t ) )->Bimg ) ;
              break;
              case 'y':
              Bimg = ( ( ( DIY * ) ( t ) )->Bimg ) ;
              break;
              case 'c':
              Bimg = ( ( ( DICH * ) ( t ) )->Bimg ) ;
              break;
              case 'r':
              Bimg = ( ( ( DIRA * ) ( t ) )->Bimg ) ;
              break;
              case 'w':
              Bimg = ( ( ( DIW * ) ( t ) )->Bimg ) ;
              break;
              case 'e':
              Bimg = ( ( ( DIE * ) ( t ) )->Bimg ) ;
              break;
              case 's':
              Bimg = ( ( ( DIS * ) ( t ) )->Bimg ) ;
              break;
              case 'g':
              Bimg = ( ( ( DIG * ) ( t ) )->Bimg ) ;
              break;
              case 'M':
              Bimg = ( ( ( DIM * ) ( t ) )->Bimg ) ;
              break;
              case 'm':
              case 'B':
              Bimg = ( ( ( DIM * ) ( t ) )->Bimg ) ;
              break;
              default:
              break;
          }
      }
      else printf ( "Null Widget\n" ) ;
      return Bimg;
  }
  int kgSetClipCallback ( void *Tmp , int ( *cpCallback ) ( int , void * ) ) {
      DIALOG *D = ( DIALOG * ) Tmp;
      kgWC *wc;
      wc = WC ( D ) ;
      if ( wc == NULL ) return 0;
      wc->cpCallback = cpCallback;
      return 1;
  }
  int kgSetClipMenu ( void *Tmp , char **menu ) {
      DIALOG *D = ( DIALOG * ) Tmp;
      kgWC *wc;
      wc = WC ( D ) ;
      if ( wc == NULL ) return 0;
      wc->cpmenu = menu;
      return 1;
  }
  int kgClearHighlight ( void *Dtmp ) {
      KBEVENT kb;
      int i , n , k;
      DIA *d;
      DIT *T;
      DIALOG *D;
      T_ELMT *elmt;
      TX_STR *tx;
      D = ( DIALOG * ) Dtmp;
      kb = D->kb;
      d = D->d;
      i = 0; while ( d [ i ] .t != NULL ) i++;
      n = i;
// for(i=0;i<n;i++) {
      for ( i = n-1;i >= 0;i-- ) {
          T = ( DIT * ) kgGetWidget ( D , i ) ;
          if ( kgGetWidgetVisibility ( T ) == 0 ) continue;
          if ( T->code == 't' ) {
              elmt = T->elmt;
              tx = T->tstr;
              k = tx->row*tx->nx+tx->col;
              if ( elmt [ k ] .hlt ) {
                  elmt [ k ] .hlt = 0;
                  _ui_drawtextcursor ( tx ) ;
                  kgUpdateOn ( D ) ;
              }
          }
          if ( T->code == 'T' ) {
              elmt = T->elmt;
              tx = T->tstr;
              k = tx->row*tx->nx+tx->col;
              if ( elmt [ k ] .hlt ) {
                  elmt [ k ] .hlt = 0;
                  _ui_drawtablecursor ( tx ) ;
                  kgUpdateOn ( D ) ;
              }
          }
      }
      return 1;
  }
  int kgCheckSelection ( void *Tmp ) {
      DIALOG *D = ( DIALOG * ) Tmp;
      kgWC *wc;
      wc = WC ( D ) ;
      if ( wc == NULL ) return 0;
      if ( wc->Rth == 0 ) return 0;
      else return 1;
  }
  void * _uiMakeMask ( int w , int h , float fac ) {
      void *fid , *img;
      fid = kgInitImage ( w , h , 1 ) ;
      kgUserFrame ( fid , 0. , 0. , ( float ) ( w+2 ) , ( float ) ( h+2 ) ) ;
      kgRoundedRectangleFill ( fid , w*0.5 , h*0.5 , w+3.0 , h+3. , 0 , 0 , 0.0 ) ;
      kgRoundedRectangleFill ( fid , w*0.5 , h*0.5 , w+1.0 , h+1. , 0 , 15 , fac ) ;
      img = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      return img;
  }
  void *kgGetProcessedImage ( void *timg , int Bsize , float rfac , int Btred , int Btgreen , int Btblue ) \
  {
      GMIMG *img = NULL;
      void *rzimg , *fid , *tmp , *mask;
      int Color = 68 , rsize , IconShape;
      int nsize;
      float rf , gf , bf , h , s , v , Vb , l , w;
      char *pt;
      IconShape = 1;
      if ( rfac > 1.0 ) rfac = 1.0;
      if ( rfac == 0.0 ) IconShape = 3;
      if ( rfac == 0.5 ) IconShape = 2;
      if ( ( rfac > 0.5 ) ) IconShape = 4;
      if ( ( rfac < 0.0 ) ) IconShape = 5;
//  printf("ImageCopy : %s\n",timg);
      img = NULL;
      pt = ( char * ) timg;
      if ( ( pt [ 0 ] == '#' ) && ( pt [ 1 ] == '#' ) && ( pt [ 2 ] != '/' ) ) {
          pt = kgSearchIcon ( pt+2 ) ;
//    printf("pt=%s\n",pt);
          if ( pt != NULL ) {
              img = ( GMIMG * ) kgGetImage ( pt ) ;
              free ( pt ) ;
          }
      }
      else img = ( GMIMG * ) kgGetImageCopy ( NULL , timg ) ;
//  printf("Got ImageCopy\n");
      Bsize = ( Bsize/2 ) *2;
      l = ( Bsize ) *0.5;
      w = l;
      rsize = Bsize+1;
      switch ( IconShape ) {
          default:
          case 4:
          nsize = rsize* ( 0.65+ ( ( rfac-0.5 ) /0.5 ) *0.35 ) ;
          break;
          case 2:
          nsize = rsize*0.65;
          break;
          case 1:
          nsize = rsize*0.8;
          break;
          case 3:
          nsize = rsize*0.85;
          break;
          case 5:
          nsize = rsize*0.9;
          break;
      }
      if ( nsize > ( rsize-8 ) ) nsize = rsize-8;
      nsize = ( nsize/2 ) *2-1;
      if ( img != NULL ) {
          if ( ( img->image_width > ( nsize ) ) || ( img->image_height > ( nsize ) ) ) {
              tmp = kgChangeSizeImage ( img , nsize , nsize ) ;
              kgFreeImage ( img ) ;
              img = tmp;
          }
      }
      if ( IconShape >= 4 ) return img;
      if ( IconShape == 3 ) {
          fid = kgInitImage ( rsize+2 , rsize+2 , 2 ) ;
          kgUserFrame ( fid , -l , -w , l , w ) ;
      }
      else {
          fid = kgInitImage ( rsize-2 , rsize-2 , 8 ) ;
          kgUserFrame ( fid , -l-2 , -w-2 , l+2 , w+2 ) ;
      }
      RGBtoHSV ( Btred , Btgreen , Btblue , & h , & s , & v ) ;
      Vb = 2.0*v;
      if ( Vb > 1. ) Vb = 1.;
      HSVtoRGB ( & rf , & gf , & bf , h , s , Vb ) ;
      kgChangeColor ( fid , 152 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      HSVtoRGB ( & rf , & gf , & bf , h , s , 0.8*v ) ;
      kgChangeColor ( fid , 153 , ( int ) rf , ( int ) gf , ( int ) bf ) ;
      kgChangeColor ( fid , 151 , ( int ) Btred , \
       ( int ) Btgreen , ( int ) Btblue ) ;
      if ( IconShape != 3 ) {
          kgRoundedRectangleFill ( fid , 0. , 0. , 2*l+1. , 2*l+1. , 0 , 152 , rfac ) ;
          kgRoundedRectangleFill ( fid , 0. , 0. , 2*l-4. , 2*l-4. , 0 , 151 , rfac ) ;
      }
      else {
          kgRoundedRectangleFill ( fid , 0. , 0. , 2*l+2. , 2*l+2. , 0 , 152 , rfac ) ;
          kgRoundedRectangleFill ( fid , 0. , 0. , 2*l-4. , 2*l-4. , 0 , 151 , rfac ) ;
      }
      rzimg = kgGetResizedImage ( fid ) ;
      kgCloseImage ( fid ) ;
      if ( img != NULL ) {
          rzimg = kgMergeImages ( rzimg , img , 0 , 0 ) ;
          kgFreeImage ( img ) ;
      }
      return rzimg;
  }
  int kgGetVersion ( void ) {
      return VER;
  }
  void *kgGetArgPointer ( void *Tmp ) {
      return ( ( DIALOG * ) Tmp )->pt;
  }
  int kgSetOnTableCell ( void *Tmp , int cell) {
  int col ; int row ;
      DIT *T = ( DIT * ) Tmp;
      T_ELMT *E = T->elmt;
      if ( T->code != 'T' ) {
          fprintf ( stderr , "Not a table\n" ) ;
          return 0;
      }
      col = cell%T->nx;
      row = cell/T->nx;
      if ( ( col < 0 ) || ( col >= T->nx ) ) {
          fprintf ( stderr , "SetOn:invalid cell: %d\n",cell ) ;
          return 0;
      }
      if ( ( row < 0 ) || ( row >= T->ny ) ) {
          fprintf ( stderr , "SetOn:invalid cell %d\n",cell ) ;
          return 0;
      }
      E [ cell ] .sw = 1;
      return 1;
  }
  int kgSetOffTableCell ( void *Tmp , int cell) {
  int col ; int row ;
      DIT *T = ( DIT * ) Tmp;
      if ( T->code != 'T' ) {
          fprintf ( stderr , "Not a table\n" ) ;
          return 0;
      }
      col = cell%T->nx;
      row = cell/T->nx;
      if ( ( col < 0 ) || ( col >= T->nx ) ) {
          fprintf ( stderr , "SetOff: invalid cell %d\n",cell ) ;
          return 0;
      }
      if ( ( row < 0 ) || ( row >= T->ny ) ) {
          fprintf ( stderr , "SetOff: invalid cell %d\n",cell ) ;
          return 0;
      }
      T_ELMT *E = T->elmt;
      E [ cell ] .sw = 0;
      return 1;
  }
  int kgGetTableRow ( void *Tmp ) {
      DIT *T = ( DIT * ) Tmp;
      if ( T->code != 'T' ) {
          fprintf ( stderr , "Not a table\n" ) ;
          return -1;
      }
      TX_STR *Tx = ( TX_STR * ) T->tstr;
      T_ELMT *E = T->elmt;
//	  printf("Tx->row: %d\n",Tx->row);
      return Tx->row;
  }
  int kgGetTableColumn ( void *Tmp ) {
      DIT *T = ( DIT * ) Tmp;
      if ( T->code != 'T' ) {
          fprintf ( stderr , "Not a table\n" ) ;
          return -1;
      }
      TX_STR *Tx = ( TX_STR * ) T->tstr;
      T_ELMT *E = T->elmt;
      return Tx->col;
  }
  int kgGetTableCell ( void *Tmp ) {
      DIT *T = ( DIT * ) Tmp;
      if ( T->code != 'T' ) {
          fprintf ( stderr , "Not a table\n" ) ;
          return -1;
      }
      TX_STR *Tx = ( TX_STR * ) T->tstr;
      T_ELMT *E = T->elmt;
      return Tx->row*Tx->nx+Tx->col;
  }
  int kgGetTableCurpos ( void *Tmp ) {
      DIT *T = ( DIT * ) Tmp;
      int cell;
      if ( T->code != 'T' ) {
          fprintf ( stderr , "Not a table\n" ) ;
          return -1;
      }
      TX_STR *Tx = ( TX_STR * ) T->tstr;
      T_ELMT *E = Tx->elmt;
      cell= Tx->row*Tx->nx+Tx->col;
      return E[cell].cursor;
  }
 
  int kgGetTableStartChar ( void *Tmp ) {
      DIT *T = ( DIT * ) Tmp;
      int cell;
      if ( T->code != 'T' ) {
          fprintf ( stderr , "Not a table\n" ) ;
          return -1;
      }
      TX_STR *Tx = ( TX_STR * ) T->tstr;
      T_ELMT *E = Tx->elmt;
      cell= Tx->row*Tx->nx+Tx->col;
      return E[cell].startchar;
  }
 
  int kgSetTableCursor ( void * Tmp , int cell) {
      int col,row;
      DIT *T = ( DIT * ) Tmp;
      int box;
      if ( T->code != 'T' ) {
          fprintf ( stderr , "Not a table\n" ) ;
          return 0;
      }
      TX_STR *Tx = ( TX_STR * ) T->tstr;
      col = cell%T->nx;
      row = cell/T->nx;
      if ( ( col < 0 ) || ( col >= T->nx ) ) {
          fprintf ( stderr , "kgSetTableCursor:invalid cell %d\n",cell ) ;
          return 0;
      }
      if ( ( row < 0 ) || ( row >= T->ny ) ) {
          fprintf ( stderr , "kgSetTableCursor:invalid cell %d\n",cell ) ;
          return 0;
      }
      T_ELMT *E = T->elmt;
      box = row*Tx->nx+col;
      if ( E [ box ] .sw == 1 ) {
          E [ box ] .cursor = 0;
          E [ box ] .startchar = 0;
          Tx->col = col;
          Tx->row = row;
          T->col = col;
          T->row = row;
          _ui_drawtablecursor ( Tx ) ;
      }
      return 1;
  }
  int kgSetTableCursorPos ( void * Tmp , int cell,int pos) {
      int col,row;
      DIT *T = ( DIT * ) Tmp;
      T_ELMT *e;
      int box;
      if ( T->code != 'T' ) {
          fprintf ( stderr , "Not a table\n" ) ;
          return 0;
      }
      TX_STR *Tx = ( TX_STR * ) T->tstr;
      col = cell%T->nx;
      row = cell/T->nx;
      if ( ( col < 0 ) || ( col >= T->nx ) ) {
          fprintf ( stderr , "kgSetTableCursor pos:invalid cell %d\n",cell ) ;
          return 0;
      }
      if ( ( row < 0 ) || ( row >= T->ny ) ) {
          fprintf ( stderr , "kgSetTableCursor pos:invalid cell %d\n",cell ) ;
          return 0;
      }
      T_ELMT *E = Tx->elmt;
      box = cell;
      if ( E [ box ] .sw == 1 ) {
	  int ln = (E[box].x2 -E[box].x1 -T->FontSize/2)/T->FontSize -2;
	  _ui_cleantablecursor(Tx);

	  if(pos >ln) {
		  E[box].startchar= (pos/ln)*ln;
	  }
//	  printf("pos: %d ln %d st %d box:%d\n",pos,ln,E[box].startchar,box);
          Tx->col = col;
          Tx->row = row;
          T->col = col;
          T->row = row;
          E [ box ] .cursor = pos ;
          _ui_drawtablecursor ( Tx ) ;
      }
      return 1;
  }
  int kgSetScrollMovement ( void *Tmp , double mvmt) {
      DIV *V =( DIV * ) Tmp;
      if ( (V->code != 'v')&&(V->code != 'z') ) {
          fprintf ( stderr , "Not a Scroll\n" ) ;
          return 0;
      }
      V->mvmt=mvmt;
      kgUpdateWidget(V);
      return 1;
  }
