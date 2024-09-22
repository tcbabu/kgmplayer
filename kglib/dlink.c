/*
   Version 2.1
   Dated 12/07/97
*/
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include "dlink.h"
#if 0
  typedef struct d_l {
      void *bf;
      struct d_l *nx;
      struct d_l *pv;
  } D_l;
  typedef struct l_L {
      D_l *st;
      D_l *cr;
      D_l *en;
  } Dlink;
#endif
  static int ( *Item_comp ) ( void *, void * ) ;
  Dlink * Dopen ( void ) {
      Dlink *tmp;
      tmp = ( Dlink * ) malloc ( sizeof ( Dlink ) ) ;
      tmp-> st = NULL;
      tmp-> en = NULL;
      tmp-> cr = NULL;
      return ( tmp ) ;
  }
/* Ddup duplicates a link L to Dup to
   make thread safe read on Link list;
   This call is thread safe
*/
  int Ddup ( Dlink *L , Dlink *Dup ) {
      Dup-> st = L-> st;
      Dup-> en = L-> en;
      Dup-> cr = L-> st;
      return 1;
  }
  int Dappend ( Dlink *F , void *buf ) {
      D_l *LN , *tmp;
      if ( F == NULL ) {
          printf ( "Error: Trying to append a NULL list\n" ) ;
          exit ( 0 ) ;
      }
      LN = F-> en;
      if ( LN == ( ( void * ) 0 ) ) {
          LN = ( struct d_l * ) malloc ( sizeof ( struct d_l ) ) ;
          F-> st = LN;
          F-> en = LN;
          F-> cr = F-> en;
          LN-> nx = ( ( struct d_l * ) 0 ) ;
          LN-> pv = ( ( struct d_l * ) 0 ) ;
          LN-> bf = buf;
      }
      else{
          LN-> nx = ( struct d_l * ) malloc ( sizeof ( struct d_l ) ) ;
          tmp = LN;
          LN = LN-> nx;
          LN-> pv = tmp;
          LN-> nx = ( ( struct d_l * ) 0 ) ;
          LN-> bf = buf;
          F-> en = LN;
          F-> cr = F-> en;
      }
      return 1;
  }
/*
  Emptying link and releasing memory
  Even records are released
*/
  void Dempty ( Dlink *F ) {
      int i , j;
      D_l *LN , *tmp;
      if ( F == NULL ) return;
      LN = F-> st;
      while ( LN != ( ( void * ) 0 ) ) {
          tmp = LN-> nx;
          if ( LN-> bf != ( void * ) 0 ) free ( LN-> bf ) ; free ( LN ) ;
          LN = tmp;
      }
      free ( F ) ;
      F = NULL;
  }
/*
   Releasing link memory
   Records are not relased
   Sublink should use only this
*/
  void Dfree ( Dlink *F ) {
      int i , j;
      D_l *LN , *tmp;
      if ( F == NULL ) return;
      LN = F-> st;
      while ( LN != ( ( void * ) 0 ) ) {
          tmp = LN-> nx;
          free ( LN ) ;
          LN = tmp;
      }
      free ( F ) ;
      F = NULL;
  }
  void Drewind ( Dlink *F ) {
      if ( F != NULL ) F-> cr = F-> st;
  }
  void Dend ( Dlink *F ) {
      if ( F != NULL ) F-> cr = F-> en;
  }
  void Dmove_back ( Dlink *LN , int n ) {
      int i;
      D_l *pt;
      if ( LN == NULL ) {
          printf ( "Error: Empty list:Dmove_back\n" ) ;
          exit ( 0 ) ;
      }
      if ( LN-> cr == LN-> st ) return;
      if ( LN-> st == ( ( void * ) 0 ) ) return;
      if ( LN-> cr == ( ( void * ) 0 ) ) { LN-> cr = LN-> en;n--;}
      pt = LN-> cr-> pv;
      for ( i = 0; i < n;i++ ) {
          if ( pt == ( ( void * ) 0 ) ) break;
          LN-> cr = pt;
          pt = LN-> cr-> pv;
      }
  }
  void Dmove_forward ( Dlink *LN , int n ) {
      int i;
      D_l *pt;
      if ( LN == NULL ) {
          printf ( "Error: Empty list:Dmove_forward\n" ) ;
          exit ( 0 ) ;
      }
      if ( LN-> en == LN-> cr ) return;
      pt = LN-> cr-> nx;
      for ( i = 0; i < n;i++ ) {
          if ( pt == ( ( void * ) 0 ) ) break;
          LN-> cr = pt;
          pt = LN-> cr-> nx;
      }
  }
/*
   After deletion Current Position remains same
   ie items are pushed down
   However Current Position will never cross
   the last item . Also the item memory is freed
   and hence can not be used. If this should not 
   happen on has to use 'Dpick'.
*/
  void Ddelete_items ( Dlink *LN , int n ) {
      int i = 0;
      D_l *pt , *pv , *nx;
      if ( LN == NULL ) {
          printf ( "Error: Empty list:Ddelete_items\n" ) ;
          exit ( 0 ) ;
      }
      if ( n <= 0 ) return;
      if ( LN-> cr == ( ( void * ) 0 ) ) return;
      pt = LN-> cr-> nx;
      pv = LN-> cr-> pv;
      for ( i = 0; i < n;i++ ) {
  /*if (pt== ((void *)0) ) break;*/
          free ( LN-> cr-> bf ) ;
          free ( LN-> cr ) ;
          if ( pt == ( ( void * ) 0 ) ) {LN-> cr = ( void * ) 0; break;}
          LN-> cr = pt;
          pt = LN-> cr-> nx;
      }
      pt = LN-> cr;
      if ( i < n ) {
          if ( pv != ( ( struct d_l * ) 0 ) ) {LN-> cr = pv;
          pv-> nx = ( ( struct d_l * ) 0 ) ;LN-> en = pv;}
          else { LN-> cr = ( ( D_l * ) 0 ) ; LN-> st = ( 
              ( D_l * ) 0 ) ; LN-> en = ( ( D_l * ) 0 ) ;}
      }
      else {
          if ( pv != ( ( void * ) 0 ) ) pv-> nx = pt;
          else {LN-> cr-> pv = ( ( struct d_l * ) 0 ) ;LN-> st = LN-> cr;}
          if ( pt != ( ( void * ) 0 ) ) pt-> pv = pv;
      }
  }
  void Ddelete ( Dlink *LN ) {
      Ddelete_items ( LN , 1 ) ;
  }
/*
  Picks an item, much like delete,
  but the ponter is not freed but returned
*/
  void* Dpick ( Dlink *LN ) {
      void *tmp;
      int i = 0 , n = 1;
      D_l *pt , *pv , *nx;
      if ( LN == NULL ) {
          printf ( "Error: Empty list:Dpick\n" ) ;
          exit ( 0 ) ;
      }
      if ( LN-> cr == ( ( void * ) 0 ) ) return NULL;
      pt = LN-> cr-> nx;
      pv = LN-> cr-> pv;
      for ( i = 0; i < n;i++ ) {
          tmp = ( void * ) ( LN-> cr-> bf ) ;
          free ( LN-> cr ) ;
          if ( pt == ( ( void * ) 0 ) ) break;
          LN-> cr = pt;
          pt = LN-> cr-> nx;
      }
      pt = LN-> cr;
      if ( i < n ) {
          if ( pv != ( ( D_l * ) 0 ) ) {LN-> cr = pv;pv-> nx = 
              ( ( struct d_l * ) 0 ) ;LN-> en = pv;}
          else { LN-> cr = ( ( D_l * ) 0 ) ; LN-> st = ( 
              ( D_l * ) 0 ) ; LN-> en = ( ( D_l * ) 0 ) ;}
      }
      else {
          if ( pv != ( ( void * ) 0 ) ) pv-> nx = pt;
          else {LN-> cr-> pv = ( ( struct d_l * ) 0 ) ;LN-> st = LN-> cr;}
          if ( pt != ( ( void * ) 0 ) ) pt-> pv = pv;
      }
      return tmp;
  }
  void Dinsert ( Dlink *F , void *buf ) {
      D_l *LN , *tmp;
      if ( F == NULL ) {
          printf ( "Error: Empty list:Dinsert\n" ) ;
          exit ( 0 ) ;
      }
      if ( F-> cr == ( ( void * ) 0 ) ) F-> cr = F-> en;
      if ( ( F-> st == ( ( void * ) 0 ) ) || ( F-> cr == ( ( void * ) 0 ) ) ) {
          Dappend ( F , buf ) ;
          return;
      }
      else {
          LN = F-> cr;
          tmp = ( struct d_l * ) malloc ( sizeof ( struct d_l ) ) ;
          tmp-> nx = LN;
          tmp-> pv = LN-> pv;
          tmp-> bf = buf;
          if ( tmp-> pv == ( ( void * ) 0 ) ) { F-> st = tmp; }
          else {tmp-> pv-> nx = tmp;}
          F-> cr = tmp;
          LN-> pv = tmp;
      }
  }
  void Dadd ( Dlink *F , void *buf ) {
      D_l *LN , *tmp;
      if ( F == NULL ) {
          printf ( "Error: Empty list:Dadd\n" ) ;
          exit ( 0 ) ;
      }
      if ( F-> en == F-> cr ) F-> cr = ( ( D_l * ) 0 ) ;
      if ( ( F-> st == ( ( void * ) 0 ) ) || ( F-> cr == ( ( void * ) 0 ) ) ) {
          Dappend ( F , buf ) ;
          return;
      }
      else {
          LN = F-> cr-> nx;
          tmp = ( struct d_l * ) malloc ( sizeof ( struct d_l ) ) ;
          tmp-> nx = LN;
          tmp-> pv = LN-> pv;
          tmp-> bf = buf;
          tmp-> pv-> nx = tmp;
          F-> cr = tmp;
          LN-> pv = tmp;
      }
  }
  Dlink *Dcopy ( Dlink *LN ) {
      Dlink *tmp;
      D_l *pt;
      tmp = Dopen ( ) ;
      pt = LN-> st;
      while ( pt != NULL ) {
          Dadd ( tmp , pt-> bf ) ;
          pt = pt-> nx;
      }
      return ( tmp ) ;
  }
/* 
  Making a sublist from a list
*/
  Dlink *Dsublist ( Dlink *LN , void *s , int Dcondition ( void *, void * ) ) {
      Dlink *tmp;
      D_l *pt;
      if ( LN == NULL ) {
          printf ( "Error: Empty list:Dsublist\n" ) ;
          exit ( 0 ) ;
      }
      tmp = Dopen ( ) ;
      pt = LN-> st;
      while ( pt != NULL ) {
          if ( Dcondition ( s , pt-> bf ) ) Dadd ( tmp , pt-> bf ) ;
          pt = pt-> nx;
      }
      return ( tmp ) ;
  }
/*
 Poisitioning current poisition as per condition
 returns 1 on success
 first argument of condition is given by user
*/
  int Dsetcurpos ( Dlink *LN , void *s , int Dcondition ( void *, void * ) ) {
      D_l *pt;
      if ( LN == NULL ) {
          printf ( "Error: Empty list:Dsetcurpos\n" ) ;
          exit ( 0 ) ;
      }
      pt = LN-> st;
      while ( pt != NULL ) {
          if ( Dcondition ( s , pt-> bf ) ) {
              LN-> cr = pt;
              return 1;
          }
          pt = pt-> nx;
      }
      return 0;
  }
/* 
  Deriving a new link list as per given rule 
*/
  Dlink *Dnewlist ( Dlink *LN , void * Dnewrule ( void * ) ) {
      Dlink *tmp;
      D_l *pt;
      if ( LN == NULL ) {
          printf ( "Error: Empty list:Dnewlist\n" ) ;
          exit ( 0 ) ;
      }
      tmp = Dopen ( ) ;
      pt = LN-> st;
      while ( pt != NULL ) {
          Dadd ( tmp , Dnewrule ( pt-> bf ) ) ;
          pt = pt-> nx;
      }
      return ( tmp ) ;
  }
/*
  The comparison return 1 if the criteria satisfied
  and repositioning is done
  the first arg(the current value,ie the one coming first in the
  current order) is compared with the
  second( ie the one coming later).
  Example:
    To compare an array of numbers is in increasing order
    int compno(void *first,void *second) {
      int ret = -1;
      if ( *((int *)first > *((int *)second) ) ret =1;
      return ret;
    }

*/
  void Dsort_o ( Dlink *LN , int Dcmpitems ( void *, void * ) ) {
      D_l *tmp , *pt , *pos;
      void *dum;
      if ( LN == NULL ) {
          printf ( "Error: Empty list\n" ) ;
          exit ( 0 ) ;
      }
      tmp = LN-> st;
      if ( tmp == NULL ) return;
      while ( tmp-> nx != NULL ) {
          pos = tmp;
          pt = tmp-> nx;
          while ( pt != NULL ) {
              if ( Dcmpitems ( pos-> bf , pt-> bf ) > 0 ) 
                  { /* to be provided by user */ pos = pt;
              }
              pt = pt-> nx;
          }
          if ( pos != tmp ) {
              dum = tmp-> bf;
              tmp-> bf = pos-> bf;
              pos-> bf = dum;
          }
          tmp = tmp-> nx;
      }
  }
  void Dsort ( Dlink *LN , int Dcmpitems ( void *, void * ) ) {
      D_l *tmp , *pt , *pos , *tmp1;
      void *dum;
      if ( LN == NULL ) {
          printf ( "Error: Empty listi:Dsort\n" ) ;
          exit ( 0 ) ;
      }
      tmp = LN-> st;
      if ( tmp == NULL ) return;
      pt = tmp-> nx;
      while ( pt != NULL ) {
          pos = LN-> st;
          while ( pos != pt ) {
              if ( Dcmpitems ( pos-> bf , pt-> bf ) > 0 ) 
                  { /* to be provided by user */ tmp1 = pt;
                   ( pt-> pv ) -> nx = pt-> nx;
                  if ( pt-> nx != NULL ) ( pt-> nx ) -> pv = pt-> pv;
                  else LN-> en = pt-> pv;
                  pt = pt-> nx;
                  if ( pos-> pv != NULL ) ( pos-> pv ) -> nx = tmp1;
                  tmp1-> pv = pos-> pv;
                  pos-> pv = tmp1;
                  tmp1-> nx = pos;
                  if ( pos == LN-> st ) LN-> st = tmp1;
                  break;
              }
              pos = pos-> nx;
          }
          if ( pos == pt ) pt = pt-> nx;
      }
  }
#define  Dintrchngpositions(LN,pos,pt) {\
  D_l *tmp1; \
      tmp1 = pt; \
       ( pt-> pv ) -> nx = pt-> nx; \
      if ( pt-> nx != NULL ) ( pt-> nx ) -> pv = pt-> pv; \
      else LN-> en = pt-> pv; \
      if ( pos-> pv != NULL ) ( pos-> pv ) -> nx = tmp1; \
      tmp1-> pv = pos-> pv; \
      pos-> pv = tmp1; \
      tmp1-> nx = pos; \
      if ( pos == LN-> st ) LN-> st = tmp1; \
  }
  void Dsort_iter ( Dlink *LN , int Dcmpitems ( void *, void * ) ) {
      D_l *tmp , *pt , *pos , *tmp1;
      void *dum;
      int n = 1 , cont1 , cont2 , cont3;
      if ( LN == NULL ) {
          printf ( "Error: Empty list:Dsort_iter\n" ) ;
          exit ( 0 ) ;
      }
      Item_comp = Dcmpitems;
      tmp = LN-> st;
      if ( tmp == NULL ) return;
      pt = tmp-> nx;
      if ( pt == NULL ) return;
      n++;
      pos = LN-> st;
      if ( Dcmpitems ( pos-> bf , pt-> bf ) > 0 ) { /* to be provided by user */ tmp1 = pt-> pv;
          
          Dintrchngpositions ( LN , pos , pt ) ;
          pt = tmp1;
      }
      pt = pt-> nx;
      while ( pt != NULL ) {
          n++;
          pos = LN-> st;
          if ( Dcmpitems ( pos-> bf , pt-> bf ) > 0 ) { /* to be provided by user */ tmp1 = pt-> pv;
              
              Dintrchngpositions ( LN , pos , pt ) ;
              pt = tmp1;
          }
          else {
              pos = pt-> pv;
              if ( Dcmpitems ( pos-> bf , pt-> bf ) > 0 ) { /* to be provided by user */ tmp1 = pt-> pv;
                  
                  cont1 = 1; cont2 = n-1;
                  cont3 = ( cont1+cont2 ) /2;
                  while ( cont1 != cont3 ) {
                      Dposition ( LN , cont3 ) ;
                      tmp = LN-> cr;
                      if ( Dcmpitems ( tmp-> bf , pt-> bf ) > 0 ) {
                          pos = tmp; cont2 = cont3;
                      }
                      else { cont1 = cont3;}
                      cont3 = ( cont1+cont2 ) /2;
                  }
                  Dintrchngpositions ( LN , pos , pt ) ;
                  pt = tmp1;
              }
          }
          pt = pt-> nx;
      }
  }
/*------------------------------------------------------
   Counting number of items
 ------------------------------------------------------ */
  int Dcount ( Dlink *F ) {
      int n = 0;
      D_l *LN;
      if ( F == NULL ) {
          printf ( "Error: Empty list:Dcount\n" ) ;
          exit ( 0 ) ;
      }
      LN = F-> st;
      while ( LN != ( ( void * ) 0 ) ) {
          n++;
          LN = LN-> nx;
      }
      return n;
  }
/*------------------------------------------------------
  Positioning the list to the given number
  Number is counted from 1 (not 0 , take care)
 ------------------------------------------------------ */
  void Dposition ( Dlink *F , int n ) {
      int i = 0;
      D_l *LN;
      if ( F == NULL ) return;
      LN = F-> st;
      if ( n == 0 ) {F-> cr = F-> st; return;}
      if ( LN == ( void * ) 0 ) {F-> cr = NULL; return;}
      while ( LN != ( ( void * ) 0 ) ) {
          i++;
          if ( i == n ) break;
          LN = LN-> nx;
      }
      if ( LN == ( void * ) 0 ) LN = F-> en;
      F-> cr = LN;
  }
/*------------------------------------------------------
   Joining two links
   the second list is append to the first
   return value is the destination, usefull
   if the destination had to be created and
   was NULL originally
   The source should not be 'Emptied'
   The source can be 'Freed'
 ------------------------------------------------------ */
  Dlink *Djoin ( Dlink *d , Dlink *s ) {
      D_l *tmp;
      if ( s == NULL ) return d;
      if ( d == NULL ) {
        /*gprintf("Destination is NULL");*/
          return s;
      }
      d-> cr = d-> en;
      tmp = s-> st;
      while ( tmp != NULL ) {
          Dadd ( d , tmp-> bf ) ;
          tmp = tmp-> nx;
      }
      return d;
  }
/*------------------------------------------------------
   Finding the sum of an item (integer) 
   Dsumitem returns the value of the item
   picked up from the record
 ------------------------------------------------------ */
  int Disum ( Dlink *LN , int Dsumitem ( void * ) ) {
      D_l *tmp;
      int sum = 0;
      tmp = LN-> st;
      if ( tmp == NULL ) { return sum;}
      while ( tmp != NULL ) {
          sum += Dsumitem ( tmp-> bf ) ;
          tmp = tmp-> nx;
      }
      return sum;
  }
/* 
   Finding the sum of an item (float) 
   Dsumitem returns the value of the item
   picked up from the record
*/
  float Dfsum ( Dlink *LN , float Dsumitem ( void * ) ) {
      D_l *tmp;
      float sum = 0;
      tmp = LN-> st;
      if ( tmp == NULL ) { return sum;}
      while ( tmp != NULL ) {
          sum += Dsumitem ( tmp-> bf ) ;
          tmp = tmp-> nx;
      }
      return sum;
  }
/* 
   Finding the sum of an item (double) 
   Dsumitem returns the value of the item
   picked up from the record
*/
  double Ddsum ( Dlink *LN , double Dsumitem ( void * ) ) {
      D_l *tmp;
      double sum = 0;
      tmp = LN-> st;
      if ( tmp == NULL ) return sum;
      while ( tmp != NULL ) {
          sum += Dsumitem ( tmp-> bf ) ;
          tmp = tmp-> nx;
      }
      return sum;
  }
  int Dcomplist ( Dlink *L1 , Dlink *L2 , int comprecord 
      ( void *tmp1 , void *tmp2 ) ) {
      D_l *t1 , *t2;
      if ( Dcount ( L1 ) != Dcount ( L2 ) ) return 0;
      t1 = L1-> st;
      t2 = L2-> st;
      while ( t1 != NULL ) {
          if ( comprecord ( t1-> bf , t2-> bf ) != 1 ) return 0;
          t1 = t1-> nx;
          t2 = t2-> nx;
      }
      return 1;
  }
  int Dread ( Dlink **L , FILE *fp , void * read_link ( FILE *f ) ) {
      int count , i;
      void *buf;
      *L = Dopen ( ) ;
      while ( ( buf = read_link ( fp ) ) != NULL ) Dadd ( *L , buf ) ;
      return 1;
  }
  int Dwrite ( Dlink *L , FILE *fp , int write_link ( FILE *f , void *buf ) ) {
      int count , i;
      D_l *tmp;
      tmp = L-> st;
      if ( tmp == NULL ) return 1;
      while ( tmp != NULL ) {
          if ( write_link ( fp , tmp-> bf ) <= 0 ) {
              return -1;
          }
          tmp = tmp-> nx;
      }
      return 1;
  }
  int Dreadauto ( Dlink **L , char *flname , int size_of_rec ) {
      int count , i , ret;
      void *buf;
      FILE* fp;
      *L = Dopen ( ) ;
      if ( ( fp = fopen ( flname , "rb" ) ) == NULL ) return 0;
      if ( fread ( & count , sizeof ( int ) , 1 , fp ) <= 0 ) {
          fclose ( fp ) ;
          return -1;
      }
      for ( i = 0;i < count;i++ ) {
          buf = ( void * ) malloc ( size_of_rec ) ;
          if ( ( fread ( buf , size_of_rec , 1 , fp ) ) > 0 ) Dadd ( *L , buf ) ;
          else {
              fclose ( fp ) ;return -1;
          }
      }
      fclose ( fp ) ;
      return 1;
  }
  int Dwriteauto ( Dlink *L , char *flname , int size_of_rec ) {
      int count , i;
      D_l *tmp;
      FILE* fp;
      if ( ( fp = fopen ( flname , "wb" ) ) == NULL ) return 0;
      count = Dcount ( L ) ;
      if ( fwrite ( & count , sizeof ( int ) , 1 , fp ) <= 0 ) return -1;
      tmp = L-> st;
      while ( tmp != NULL ) {
          if ( fwrite ( tmp-> bf , size_of_rec , 1 , fp ) <= 0 ) {
              fclose ( fp ) ;
              printf ( "Error: In Dwriteauto ... Count=%d\n" , count ) ;
              exit ( 0 ) ;
          }
          tmp = tmp-> nx;
      }
      fclose ( fp ) ;
      return 1;
  }
/*******************************************************
   removes duplicate records; all fields should be
   defined/initialised, otherwise will end up in error
   Also beware of character fields, unused area can
   create problem
********************************************************/
  int Drmvdup ( Dlink *L , int size ) {
      D_l *tmp , *tmp1;
      char *pt , *pt1;
      int i = 0;
      tmp = L-> st;
      while ( tmp != NULL ) {
          pt = ( char * ) ( tmp-> bf ) ;
          if ( tmp != L-> en ) {
              tmp1 = tmp-> nx;
              while ( tmp1 != NULL ) {
                  pt1 = ( char * ) ( tmp1-> bf ) ;
                  for ( i = 0;i < size;i++ ) if ( pt [ i ] != pt1 [ i ] ) break;
                  if ( i == size ) {
                      L-> cr = tmp1;
                      if ( tmp1 == L-> en ) {
                          Ddelete ( L ) ;
                          break;
                      } /* in case last one is deleted */ Ddelete ( L ) ;
                      tmp1 = L-> cr;
                  }
                  else tmp1 = tmp1-> nx;
              }
          }
          tmp = tmp-> nx;
      }
      return 1;
  }
/*******************************************************
   removes duplicate records as per given rule;
 * May not be good, due to the probabilty of not freeing
 * the duplicates
********************************************************/
  int Drmvdup_cond ( Dlink *L , int rule ( void *, void * ) ) {
      D_l *tmp , *tmp1;
      tmp = L-> st;
      while ( tmp != NULL ) {
          if ( tmp != L-> en ) {
              tmp1 = tmp-> nx;
              while ( tmp1 != NULL ) {
                  if ( rule ( tmp-> bf , tmp1-> bf ) == 1 ) {
                      L-> cr = tmp1;
                      if ( tmp1 == L-> en ) {
                          Ddelete ( L ) ;
                          break;
                      } /* in case last one is deleted */ Ddelete ( L ) ;
                      tmp1 = L-> cr;
                      if ( tmp1 == L-> en ) break;
                  }
                  else tmp1 = tmp1-> nx;
              }
          }
          tmp = tmp-> nx;
      }
      return 1;
  }
/* Added on 23rd Sept 1999 */
/*************************************************************** 
  Setting link's current position to the start
****************************************************************/
  void *Resetlink ( Dlink *L ) {
      if ( L == NULL ) return NULL;
      if ( L-> st == NULL ) return NULL;
      L-> cr = L-> st;
      return L-> cr-> bf;
  }
/****************************************************************
  getting current record and changing to next record
  returns NULL if there is no record
*****************************************************************/
  void *Getrecord ( Dlink *L ) {
      void *tmp = NULL;
      if ( ( L != NULL ) && ( L-> cr != NULL ) ) {
          tmp = L-> cr-> bf;
          L-> cr = L-> cr-> nx;
      }
      return tmp;
  }
/****************************************************************
  getting current record and changing to previous record
  returns NULL if there is no record
*****************************************************************/
  void *Getrecordrev ( Dlink *L ) {
      void *tmp = NULL;
      if ( ( L != NULL ) && ( L-> cr != NULL ) ) {
          tmp = L-> cr-> bf;
          L-> cr = L-> cr-> pv;
      }
      return tmp;
  }
  static int GetNextTag ( char *tmp , char *Tag , int *ln ) {
      int i , j = 0;
      char *pt = NULL;
      int ret;
      char buff [ 500 ] ;
      int ok = 1;
      Tag [ 0 ] = '\0';
      pt = tmp;
      i = 0;
      while ( ( *pt == ' ' ) || ( *pt == '\t' ) ) {
          if ( ( *pt != '\t' ) ) *pt = ' ';
          i++;
          pt++;
      }
      if ( ( *pt < ' ' ) ) {*ln = i;return i;}
      j = 0;
      while ( ( *pt > ' ' ) ) {
          ok = 1;
          if ( *pt == '\'' ) {
              buff [ j++ ] = *pt++;i++;
              while ( *pt != '\'' ) {
                  if ( *pt == '\\' ) {buff [ j++ ] = *pt++;i++;}
                  if ( ( *pt == '\n' ) || ( *pt == '\0' ) ) {ok = 0;break;}
                  buff [ j++ ] = *pt++;
                  i++;
              }
          }
          if ( *pt == '"' ) {
              buff [ j++ ] = *pt++;i++;
              while ( *pt != '"' ) {
                  if ( *pt == '\\' ) {buff [ j++ ] = *pt++;i++;}
                  if ( ( *pt == '\n' ) || ( *pt == '\0' ) ) {ok = 0;break;}
                  buff [ j++ ] = *pt++;
                  i++;
              }
          }
          if ( ok ) {
              Tag [ j++ ] = *pt++;
              i++;
          }
      }
      Tag [ j ] = '\0';
      *ln = i;
      return *ln;
  }
// a special sort for blank seperated string record
  static int Dcmpstring ( void *p1 , void *p2 , int field , int order ) {
      char *s1 = ( char * ) p1 , *s2 = ( char * ) p2;
      char Tag1 [ 300 ] , Tag2 [ 300 ] ;
      char *cpt1 , *cpt2;
      int i , ln;
      int ret;
      for ( i = 0;i < field;i++ ) {
          GetNextTag ( s1 , Tag1 , & ln ) ;
          s1 += ln;
          if ( Tag1 [ 0 ] == '\0' ) {
              fprintf ( stderr , "Error in Dstringsort..\n" ) ;
              exit ( -1 ) ;
          }
          GetNextTag ( s2 , Tag2 , & ln ) ;
          s2 += ln;
          if ( Tag2 [ 0 ] == '\0' ) {
              fprintf ( stderr , "Error in Dstringsort..\n" ) ;
              exit ( -1 ) ;
          }
      }
//	printf( "TAG0: %s %s\n",Tag1,Tag2);
      cpt1 = Tag1;
      if ( ( Tag1 [ 0 ] == '\'' ) || ( Tag1 [ 0 ] == '"' ) ) {
          cpt1 = Tag1+1;
          Tag1 [ strlen ( Tag1 ) -1 ] = '\0';
      }
      cpt2 = Tag2;
      if ( ( Tag2 [ 0 ] == '\'' ) || ( Tag2 [ 0 ] == '"' ) ) {
          cpt2 = Tag2+1;
          Tag2 [ strlen ( Tag2 ) -1 ] = '\0';
      }
      ret = strcmp ( cpt1 , cpt2 ) ;
      if ( order == 0 ) {
          if ( ret > 0 ) return 1;
          else return 0;
      }
      else {
          if ( ret < 0 ) return 1;
          else return 0;
      }
  }
// order = 0 ascenting
// else decenting
// field is counted from 1
  void Dstringsort ( Dlink *LN , int field , int order ) {
      D_l *tmp , *pt , *pos , *tmp1;
      void *dum;
      if ( LN == NULL ) {
          printf ( "Error: Empty listi:Dsort\n" ) ;
          exit ( 0 ) ;
      }
      tmp = LN-> st;
      if ( tmp == NULL ) return;
      pt = tmp-> nx;
      while ( pt != NULL ) {
          pos = LN-> st;
          while ( pos != pt ) {
              if ( Dcmpstring ( pos-> bf , pt-> bf , field , order ) > 0 ) {
                  tmp1 = pt;
                   ( pt-> pv ) -> nx = pt-> nx;
                  if ( pt-> nx != NULL ) ( pt-> nx ) -> pv = pt-> pv;
                  else LN-> en = pt-> pv;
                  pt = pt-> nx;
                  if ( pos-> pv != NULL ) ( pos-> pv ) -> nx = tmp1;
                  tmp1-> pv = pos-> pv;
                  pos-> pv = tmp1;
                  tmp1-> nx = pos;
                  if ( pos == LN-> st ) LN-> st = tmp1;
                  break;
              }
              pos = pos-> nx;
          }
          if ( pos == pt ) pt = pt-> nx;
      }
  }
// a special sort for blank seperated string record
  static int Dcmpnumber ( void *p1 , void *p2 , int field , int order ) {
      char *s1 = ( char * ) p1 , *s2 = ( char * ) p2;
      char Tag1 [ 300 ] , Tag2 [ 300 ] ;
      char *cpt1 , *cpt2;
      int i , ln;
      int ret;
      double v1 , v2;
      for ( i = 0;i < field;i++ ) {
          GetNextTag ( s1 , Tag1 , & ln ) ;
          s1 += ln;
          if ( Tag1 [ 0 ] == '\0' ) {
              fprintf ( stderr , "Error in Dstringsort..\n" ) ;
              exit ( -1 ) ;
          }
          GetNextTag ( s2 , Tag2 , & ln ) ;
          s2 += ln;
          if ( Tag2 [ 0 ] == '\0' ) {
              fprintf ( stderr , "Error in Dstringsort..\n" ) ;
              exit ( -1 ) ;
          }
      }
//	printf( "TAG0: %s %s\n",Tag1,Tag2);
      cpt1 = Tag1;
      if ( ( Tag1 [ 0 ] == '\'' ) || ( Tag1 [ 0 ] == '"' ) ) {
          cpt1 = Tag1+1;
          Tag1 [ strlen ( Tag1 ) -1 ] = '\0';
      }
      cpt2 = Tag2;
      if ( ( Tag2 [ 0 ] == '\'' ) || ( Tag2 [ 0 ] == '"' ) ) {
          cpt2 = Tag2+1;
          Tag2 [ strlen ( Tag2 ) -1 ] = '\0';
      }
      sscanf ( cpt1 , "%lf" , & v1 ) ;
      sscanf ( cpt2 , "%lf" , & v2 ) ;
//	printf("v1:v2 %lf %lf\n",v1,v2);
      if ( order == 0 ) {
          if ( v1 > v2 ) return 1;
          else return 0;
      }
      else {
          if ( v1 < v2 ) return 1;
          else return 0;
      }
  }
// order = 0 ascenting
// else decenting
// field is counted from 1
  void Dnumbersort ( Dlink *LN , int field , int order ) {
      D_l *tmp , *pt , *pos , *tmp1;
      void *dum;
      if ( LN == NULL ) {
          printf ( "Error: Empty listi:Dsort\n" ) ;
          exit ( 0 ) ;
      }
      tmp = LN-> st;
      if ( tmp == NULL ) return;
      pt = tmp-> nx;
      while ( pt != NULL ) {
          pos = LN-> st;
          while ( pos != pt ) {
              if ( Dcmpnumber ( pos-> bf , pt-> bf , field , order ) > 0 ) {
                  tmp1 = pt;
                   ( pt-> pv ) -> nx = pt-> nx;
                  if ( pt-> nx != NULL ) ( pt-> nx ) -> pv = pt-> pv;
                  else LN-> en = pt-> pv;
                  pt = pt-> nx;
                  if ( pos-> pv != NULL ) ( pos-> pv ) -> nx = tmp1;
                  tmp1-> pv = pos-> pv;
                  pos-> pv = tmp1;
                  tmp1-> nx = pos;
                  if ( pos == LN-> st ) LN-> st = tmp1;
                  break;
              }
              pos = pos-> nx;
          }
          if ( pos == pt ) pt = pt-> nx;
      }
  }
/* i is position and starts from 0 */
//Similar to Dposition, on;y that arg start from 0
//
  int Dlocation ( Dlink *L , int pos ) {
      if ( pos >= Dcount ( L ) ) return 0;
      Dposition ( L , pos+1 ) ;
      return 1;
  }
// Deletes item at pos
  int Dremove ( Dlink *L , int pos ) {
      if ( pos >= Dcount ( L ) ) return 0;
      Dposition ( L , pos+1 ) ;
      Ddelete ( L ) ;
      Dposition ( L , pos+1 ) ;
      return 1;
  }
// Removes from list but record is retuned
  void * Dtake ( Dlink *L , int pos ) {
      void *bf;
      if ( pos >= Dcount ( L ) ) return NULL;
      Dposition ( L , pos+1 ) ;
      bf = Dpick ( L ) ;;
      Dposition ( L , pos+1 ) ;
      return bf;
  }
  void *Drecord ( Dlink *L , int i ) {
      if ( i >= Dcount ( L ) ) return NULL;
      Dposition ( L , i+1 ) ;
      return Getrecord ( L ) ;
  }
  void Dtravel ( Dlink *L , void ( *action ) ( void * ) ) {
      void *pt;
      Resetlink ( L ) ;
      while ( ( pt = Getrecord ( L ) ) != NULL ) action ( pt ) ;
      Resetlink ( L ) ;
  }
// Routines to extract command line args
  Dlink * DgetFlags ( char **argv ) {
      int i;
      char *pt;
      Dlink * FlagsList = NULL;
      FlagsList = Dopen ( ) ;
      i = 0;
      while ( ( pt = argv [ i ] ) != NULL ) {
          if ( pt [ 0 ] == '-' ) Dappend ( FlagsList , pt ) ;
          i++;
      }
      return FlagsList;
  }
  Dlink * DgetArgs ( char **argv ) {
      int i;
      char *pt;
      Dlink * ArgsList = NULL;
      ArgsList = Dopen ( ) ;
      i = 0;
      while ( ( pt = argv [ i ] ) != NULL ) {
          if ( pt [ 0 ] != '-' ) Dappend ( ArgsList , pt ) ;
          i++;
      }
      return ArgsList;
  }
/* Utility routine to get arg for a flag */
  char * DgetFlagArg ( Dlink *Alist , char **argv , char *flag , int nv ) {
      char *Farg = NULL , *tpt , *Apt;
      int i = 0 , j = 0;
      while ( ( tpt = argv [ i ] ) != NULL ) {
          if ( strncmp ( flag , "--" , 2 ) == 0 ) {
              if ( strcmp ( tpt , flag ) == 0 ) {
                  Farg = argv [ i+1+nv ] ;
                  if ( Farg [ 0 ] == '-' ) {
                      fprintf ( stderr , "Error: Invalid arg for '%-s'\n" , flag ) ;
                      exit ( 1 ) ;
                  }
                  j = 0;
                  while ( strcmp ( ( Apt = ( char * ) Drecord ( Alist , j ) ) , Farg ) != 0 ) {
                      if ( Apt == NULL ) {
                          fprintf ( stderr , "Error: Invalid arg for '%-s'\n" , flag ) ;
                          exit ( 1 ) ;
                      }
                      j++;
                  }
                  Dtake ( Alist , j ) ;
                  break;
              }
          }
          else {
              if ( strncmp ( tpt , flag , 2 ) == 0 ) {
                  if ( flag [ 2 ] <= ' ' ) {
                      Farg = argv [ i+1+nv ] ;
                      if ( Farg [ 0 ] == '-' ) {
                          fprintf ( stderr , "Error: Invalid arg for '%-s'\n" , flag ) ;
                          exit ( 1 ) ;
                      }
                      j = 0;
                      while ( strcmp ( ( Apt = ( char * ) Drecord ( Alist , j ) ) , Farg ) != 0 ) {
                          if ( Apt == NULL ) {
                              fprintf ( stderr , "Error: Invalid arg for '%-s'\n" , flag ) ;
                              exit ( 1 ) ;
                          }
                          j++;
                      }
                      Dtake ( Alist , j ) ;
                      break;
                  }
                  else {Farg = flag+2;break;}
              }
          }
          i++;
      }
      return Farg;
  }
  char ** DprocessFlags ( char *argv [ ] , DARGS fargpt [ ] ) {
      Dlink *Flist = NULL , *Alist = NULL;
      char *pt;
      int i , count , j = 0 , k;
      char **rargs = NULL;
      int *ipt;
      long *lpt;
      char **spt;
      char *cpt;
      double *dpt;
      int Matched = 0;
      int Cond = 0;
      char *tpt = NULL;
      Flist = DgetFlags ( argv+1 ) ;
      Alist = DgetArgs ( argv+1 ) ;
      i = 0;
      while ( ( pt = ( char * ) Drecord ( Flist , i ) ) != NULL ) {
          j = 0;
          while ( ( fargpt+j != NULL ) && ( fargpt [ j ] .flag != NULL ) ) {
              if ( fargpt [ j ] .flag == NULL ) {
                  printf ( "NULL flag ... \n" ) ;break;
              }
              Matched = 0;
              Matched = ( ( strncmp ( pt , "--" , 2 ) != 0 ) && ( strncmp ( pt , fargpt [ j ] .flag , 2 ) == 0 ) ) 
                  || ( strcmp ( pt , fargpt [ j ] .flag ) == 0 ) ;
              if ( Matched > 0 ) {
                  if ( fargpt [ j ] .pt != NULL ) {
                      switch ( fargpt [ j ] .code ) {
                          case 'i':
                          ipt = ( int * ) ( fargpt [ j ] .pt ) ;
                          *ipt = 1;
                          for ( k = 0;k < fargpt [ j ] .nvals;k++ ) {
                              tpt = ( char * ) DgetFlagArg ( Alist , argv , pt , k ) ;
                              ipt [ k ] = atoi ( tpt ) ;
                          }
                          break;
                          case 'l':
                          lpt = ( long * ) fargpt [ j ] .pt;
                          *lpt = 1;
                          for ( k = 0;k < fargpt [ j ] .nvals;k++ ) {
                              tpt = ( char * ) DgetFlagArg ( Alist , argv , pt , k ) ;
                              * ( lpt+k ) = atol ( tpt ) ;
                          }
                          break;
                          case 'f':
                          dpt = ( double * ) fargpt [ j ] .pt;
                          *dpt = 1.0;
                          for ( k = 0;k < fargpt [ j ] .nvals;k++ ) {
                              tpt = ( char * ) DgetFlagArg ( Alist , argv , pt , k ) ;
                              dpt [ k ] = atof ( tpt ) ;
                          }
                          break;
                          case 's':
                          spt = ( char ** ) fargpt [ j ] .pt;
                          for ( k = 0;k < fargpt [ j ] .nvals;k++ ) {
                              tpt = ( char * ) DgetFlagArg ( Alist , argv , pt , k ) ;
                              spt [ k ] = tpt;
                          }
                          break;
                          case 'c':
                          cpt = ( char * ) fargpt [ j ] .pt;
                          for ( k = 0;k < fargpt [ j ] .nvals;k++ ) {
                              tpt = ( char * ) DgetFlagArg ( Alist , argv , pt , k ) ;
                              cpt [ k ] = tpt [ 0 ] ;
                          }
                          break;
                          default:
                          fprintf ( stderr , "illegal code <%s> ; ignored\n" , fargpt [ j ] .code ) ;
                          break;
                      }
                  }
              }
              if ( Matched ) {
                  if ( fargpt [ j ] .Callback != NULL ) {
                      fargpt [ j ] .Callback ( fargpt [ j ] .arg ) ;
                  }
                  break;
              }
              j++;
          }
          if ( ! Matched ) {
              fprintf ( stderr , "illegal flag  <%s> ; Exiting...\n" , pt ) ;
              fprintf ( stderr , "See help and retry...\n" ) ;
              exit ( 0 ) ;
          }
          i++;
      }
      count = Dcount ( Alist ) ;
      if ( count <= 0 ) rargs = NULL;
      else {
          rargs = ( char ** ) malloc ( sizeof ( char * ) * ( count+1 ) ) ;
          rargs [ count ] = NULL;
          for ( i = 0;i < count;i++ ) {
              rargs [ i ] = ( char * ) Drecord ( Alist , i ) ;
          }
      }
      Dfree ( Alist ) ;
      Dfree ( Flist ) ;
      return rargs;
  }
  Dlink *Dreadfile ( char *flname ) {
      Dlink *L;
      char buff [ 1000 ] ;
      char *bf;
      FILE *fp;
      fp = fopen ( flname , "r" ) ;
      if ( fp == NULL ) {
//		fprintf(stderr,"Failed open %s in Dreadfile\n",flname);
          return Dopen ( ) ;
      }
      L = Dopen ( ) ;
      while ( fgets ( buff , 999 , fp ) != NULL ) {
          bf = ( char * ) malloc ( strlen ( buff ) +1 ) ;
          strcpy ( bf , buff ) ;
          Dappend ( L , bf ) ;
      }
      fclose ( fp ) ;
      Resetlink ( L ) ;
      return L;
  }
  int Dwritefile ( Dlink *L , char *flname ) {
      char *bf;
      FILE *fp;
      int i;
      fp = fopen ( flname , "w" ) ;
      if ( fp == NULL ) {
	      fprintf(stderr,"Failed to Open (%s) to write\n",flname);
	      return 0;
      }
      i = 0;
      while ( ( bf = ( char * ) Drecord ( L , i ) ) != NULL ) {
          fprintf ( fp , "%s" , bf ) ;
//		fputs(bf,fp);
          i++;
      }
      fclose ( fp ) ;
      return 1;
  }
  int Dpush ( Dlink *L , void *bf ) {
      if ( L == NULL ) {
          fprintf ( stderr , "Empty Link in Dpush\n" ) ;
          exit ( -1 ) ;
      };
      Resetlink ( L ) ;
      Dinsert ( L , bf ) ;
      Resetlink ( L ) ;
      return 1;
  }
  void * Dpop ( Dlink *L ) {
      void *bf;
      if ( L == NULL ) {
          fprintf ( stderr , "Empty Link in Dpop\n" ) ;
          exit ( -1 ) ;
      };
      if ( Dcount ( L ) == 0 ) return NULL;
      Resetlink ( L ) ;
      bf = Dpick ( L ) ;
      Resetlink ( L ) ;
      return bf;
  }
  void * Dfifoout ( Dlink *L ) {
      void *bf;
      if ( L == NULL ) {
          fprintf ( stderr , "Empty Link in Dfifoout\n" ) ;
          exit ( -1 ) ;
      };
      Dend ( L ) ;
      bf = Dpick ( L ) ;
      Dend ( L ) ;
      return bf;
  }
  int Dfifoin ( Dlink *L , void *bf ) {
      if ( L == NULL ) {
          fprintf ( stderr , "Empty Link in Dfifoin\n" ) ;
          exit ( -1 ) ;
      };
      Resetlink ( L ) ;
      Dinsert ( L , bf ) ;
      Resetlink ( L ) ;
      return 1;
  }
  int Dreplace ( Dlink *L , void *bf , int pos ) {
	  /* pos (0,max) */
      if ( L == NULL ) {
          fprintf ( stderr , "Empty Link in Dreplace\n" ) ;
          exit ( -1 ) ;
      };
      pos++;

      Dposition ( L , pos ) ;
      Ddelete ( L ) ;
#if 1
      if(pos == 1) {
	      Dposition(L,pos);
	      Dinsert(L,bf);
      }
      else {
      Dposition ( L , pos-1 ) ;
      Dadd ( L , bf ) ;
      Dposition( L , pos ) ;
      }
#endif
      return 1;
  }
#if 0
  / * * Sample code for compare function * */ int CompareAction 
      ( void *rec , char *name ) {
	/* 
	 * return -1 if rec field < name
	 * return  1 if rec field > name
	 * return  0 if rec field = name
	 *
	 */
      PACTION *recv = ( ( PACTION * ) rec ) ;
//	int val = *((int *) name);
      char *field = recv-> Flag;;
	// field = ...
      return strcmp ( field , ( char * ) name ) ;
//	return (recv - val);
  }
#endif
  void *Dsearch ( Dlink *L , char *val , int ( *Compare ) ( void *, char * ) ) {
	/*
	 * Link must in ascending order; 
	 */
      int count = Dcount ( L ) ;
      int mid , low = 0 , up = count-1 , cmp;
      char *bf;
      if ( L == NULL ) return NULL;
      if ( count == 0 ) return NULL;
      bf = Drecord ( L , low ) ;
      cmp = Compare ( bf , ( char * ) val ) ;
      if ( cmp > 0 ) return NULL;
      if ( cmp == 0 ) return bf;
      bf = Drecord ( L , up ) ;
      cmp = Compare ( bf , ( char * ) val ) ;
      if ( cmp < 0 ) return NULL;
      if ( cmp == 0 ) return bf;
      while ( 1 ) {
          if ( up < low ) up = low;
          if ( low > up ) low = up;
          mid = ( low+up ) /2;
          bf = Drecord ( L , mid ) ;
          cmp = Compare ( bf , ( char * ) val ) ;
          if ( cmp == 0 ) return bf;
          if ( low == up ) return NULL;
          if ( cmp < 0 ) low = mid+1;
          else up = mid-1;
      }
  }

Dlink *Darraytolink(void **Array) {
	int k;
	Dlink *L=Dopen();
	k=0;
	if(Array!= NULL) while(Array[k]!= NULL) Dadd(L,Array[k++]);
	Resetlink(L);
	return L;
}
void **Dlinktoarray(Dlink *L) {
	int k;
	void **Array=NULL;
	void *pt;
	if(L==NULL) return NULL;
	k = Dcount(L);
	Array = (void **)malloc(sizeof(void *)*(k+1));
	k=0;
	Resetlink(L);
	while( (pt =Getrecord(L))!= NULL) Array[k++]=pt;
	Array[k]=NULL;
	return Array;
}
