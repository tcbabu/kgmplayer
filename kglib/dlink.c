/*
   Version 2.1
   Dated 12/07/97
*/

#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include "dlink.h"
#if 0
typedef  struct d_l {
         void  *bf;
         struct d_l *nx;
         struct d_l *pv;
} D_l;
typedef  struct l_L {
         D_l *st; 
         D_l *cr;
         D_l *en;
} Dlink;
#endif

static int (*Item_comp)(void *,void *);

Dlink * Dopen(void){
  Dlink *tmp;
  tmp = (Dlink *) malloc(sizeof(Dlink));
  tmp->st=NULL;
  tmp->en=NULL;
  tmp->cr=NULL;
  return(tmp);
}
/* Ddup duplicates a link L to Dup to
   make thread safe read on Link list;
   This call is thread safe
*/
int Ddup(Dlink *L,Dlink *Dup) {
     Dup->st = L->st;
     Dup->en = L->en;
     Dup->cr = L->st;
     return 1;
}

int Dappend(Dlink *F,void  *buf){
  D_l *LN,*tmp;
  if( F==NULL) {
    printf("Error: Trying to append a NULL list\n");
    exit(0);
  }
  LN = F->en;
  if(LN== ((void *)0) ) {
    LN = (struct d_l *)malloc(sizeof(struct d_l));
    F->st = LN;
    F->en =LN;
    F->cr =F->en;
    LN->nx= ((struct d_l *)0) ;
    LN->pv= ((struct d_l *)0) ;
    LN->bf=buf;
  }
  else{
    LN->nx= (struct d_l *)malloc(sizeof(struct d_l));
    tmp = LN;
    LN = LN->nx;
    LN->pv=tmp;
    LN->nx= ((struct d_l *)0) ;
    LN->bf=buf;
    F->en =LN;
    F->cr =F->en;
  }
  return 1;
}

/*
  Emptying link and releasing memory
  Even records are released
*/

void Dempty(Dlink *F){
  int i,j;
  D_l *LN,*tmp;
  if( F == NULL) return;
  LN = F->st;
  while( LN!= ((void *)0) ) {
   tmp = LN->nx;
   if(LN->bf != (void *)0)free(LN->bf); free(LN);
   LN=tmp;
  }
  free(F);
  F=NULL;
}
/*
   Releasing link memory
   Records are not relased
   Sublink should use only this
*/

void Dfree(Dlink *F){
  int i,j;
  D_l *LN,*tmp;
  if( F == NULL) return;
  LN = F->st;
  while( LN!= ((void *)0) ) {
   tmp = LN->nx;
   free(LN);
   LN=tmp;
  }
  free(F);
  F=NULL;
}

void Drewind(Dlink *F) {
 if(F != NULL) F->cr = F->st;
}
 
void Dend (Dlink *F) {

 if(F != NULL) F->cr = F->en;
}

void Dmove_back(Dlink *LN,int n) {
 int i;
 D_l *pt;
 if(LN==NULL) {
   printf("Error: Empty list:Dmove_back\n");
   exit(0);
 }
 if(LN->cr == LN->st ) return;
 if(LN->st == ((void *)0) ) return;
 if(LN->cr == ((void *)0) ){ LN->cr=LN->en;n--;}
 pt = LN->cr->pv;
 for ( i=0; i<n;i++) {
  if (pt== ((void *)0) ) break;
  LN->cr=pt;
  pt = LN->cr->pv;
 }
}

void Dmove_forward(Dlink *LN,int n) {
 int i;
 D_l *pt;
 if(LN==NULL) {
   printf("Error: Empty list:Dmove_forward\n");
   exit(0);
 }
 if(LN->en==LN->cr) return;
 pt = LN->cr->nx;
 for ( i=0; i<n;i++) {
  if (pt== ((void *)0) ) break;
  LN->cr=pt;
  pt = LN->cr->nx;
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
void Ddelete_items(Dlink *LN,int n) {
 int i=0;
 D_l *pt,*pv,*nx;
 if(LN==NULL) {
   printf("Error: Empty list:Ddelete_items\n");
   exit(0);
 }
 if(n<=0) return;
 if(LN->cr == ((void *)0)) return;
 pt = LN->cr->nx;
 pv = LN->cr->pv;
 for ( i=0; i<n;i++) {
  /*if (pt== ((void *)0) ) break;*/
  free(LN->cr->bf);
  free(LN->cr);
  if (pt== ((void *)0) ){LN->cr=(void *)0; break;}
  LN->cr=pt;
  pt = LN->cr->nx;
 }
 pt=LN->cr;
 if (i<n) {
  if(pv != ((struct d_l *)0) ) {LN->cr=pv;pv->nx= ((struct d_l *)0) ;LN->en=pv;}
  else { LN->cr= (( D_l *)0) ; LN->st= (( D_l *)0) ; LN->en= (( D_l *)0) ;}
 }
 else {
  if(pv != ((void *)0) ) pv->nx=pt;
  else {LN->cr->pv= ((struct d_l *)0) ;LN->st=LN->cr;}
  if(pt!= ((void *)0) ) pt->pv=pv;
 }
}
void Ddelete(Dlink *LN) {
  Ddelete_items(LN,1);
}
/*
  Picks an item, much like delete,
  but the ponter is not freed but returned
*/
void* Dpick(Dlink *LN) {
 void *tmp;
 int i=0,n=1;
 D_l *pt,*pv,*nx;
 if(LN==NULL) {
   printf("Error: Empty list:Dpick\n");
   exit(0);
 }
 if(LN->cr == ((void *)0)) return NULL;
 pt = LN->cr->nx;
 pv = LN->cr->pv;
 for ( i=0; i<n;i++) {
  tmp = (void *)(LN->cr->bf);
  free(LN->cr);
  if (pt== ((void *)0) ) break;
  LN->cr=pt;
  pt = LN->cr->nx;
 }
 pt=LN->cr;
 if (i<n) {
  if(pv != ((D_l *)0) ) {LN->cr=pv;pv->nx= ((struct d_l *)0) ;LN->en=pv;}
  else { LN->cr= ((D_l *)0) ; LN->st= ((D_l *)0) ; LN->en= ((D_l *)0) ;}
 }
 else {
  if(pv != ((void *)0) ) pv->nx=pt;
  else {LN->cr->pv= ((struct d_l *)0) ;LN->st=LN->cr;}
  if(pt!= ((void *)0) ) pt->pv=pv;
 }
 return tmp;
}
void Dinsert(Dlink *F,void  *buf) {
 D_l *LN,*tmp;
 if(F==NULL) {
   printf("Error: Empty list:Dinsert\n");
   exit(0);
 }
 if(F->cr== ((void *)0) ) F->cr=F->en;
 if((F->st== ((void *)0) )||(F->cr== ((void *)0) )) {
   Dappend(F,buf);
   return;
 }
 else {
    LN = F->cr;
    tmp = (struct d_l *)malloc(sizeof(struct d_l));
    tmp->nx=LN;
    tmp->pv=LN->pv;
    tmp->bf=buf;
    if(tmp->pv == ((void *)0) ) { F->st=tmp; }
    else {tmp->pv->nx=tmp;}
    F->cr=tmp;
    LN->pv=tmp;
 }
}
void Dadd(Dlink *F,void  *buf) {
 D_l *LN,*tmp;
 if(F==NULL) {
   printf("Error: Empty list:Dadd\n");
   exit(0);
 }
 if( F->en == F->cr ) F->cr= ((D_l *)0) ;
 if((F->st== ((void *)0) )||(F->cr== ((void *)0) )) {
   Dappend(F,buf);
   return;
 }
 else {
    LN = F->cr->nx;
    tmp = (struct d_l *)malloc(sizeof(struct d_l));
    tmp->nx=LN;
    tmp->pv=LN->pv;
    tmp->bf=buf;
    tmp->pv->nx=tmp;
    F->cr=tmp;
    LN->pv=tmp;
 }
}
Dlink *Dcopy(Dlink *LN) {
  Dlink *tmp;
  D_l *pt;
  tmp = Dopen();
  pt = LN->st;
  while( pt !=NULL) {
    Dadd(tmp,pt->bf);
    pt = pt->nx;
  }
  return(tmp);
}
/* 
  Making a sublist from a list
*/

Dlink *Dsublist(Dlink *LN,void *s,int Dcondition(void *,void *)) {
  Dlink *tmp;
  D_l *pt;
  if(LN==NULL) {
    printf("Error: Empty list:Dsublist\n");
    exit(0);
  }
  tmp = Dopen();
  pt = LN->st;
  while( pt !=NULL) {
    if(Dcondition(s,pt->bf)) Dadd(tmp,pt->bf);
    pt = pt->nx;
  }
  return(tmp);
}
/*
 Poisitioning current poisition as per condition
 returns 1 on success
 first argument of condition is given by user
*/

int Dsetcurpos(Dlink *LN,void *s,int Dcondition(void *,void *)) {
  D_l *pt;
  if(LN==NULL) {
    printf("Error: Empty list:Dsetcurpos\n");
    exit(0);
  }
  pt = LN->st;
  while( pt !=NULL) {
    if(Dcondition(s,pt->bf)) {
      LN->cr=pt;
      return 1;
    }
    pt = pt->nx;
  }
  return 0;
}
/* 
  Deriving a new link list as per given rule 
*/

Dlink *Dnewlist(Dlink *LN,void * Dnewrule(void *)) {
  Dlink *tmp;
  D_l *pt;
  if(LN==NULL) {
    printf("Error: Empty list:Dnewlist\n");
    exit(0);
  }
  tmp = Dopen();
  pt = LN->st;
  while( pt !=NULL) {
    Dadd(tmp,Dnewrule(pt->bf));
    pt = pt->nx;
  }
  return(tmp);
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
void Dsort_o(Dlink *LN , int Dcmpitems(void *,void *)) {
 D_l *tmp,*pt,*pos;
 void *dum;
  if(LN==NULL) {
    printf("Error: Empty list\n");
    exit(0);
  }
 tmp = LN->st;
 if(tmp==NULL) return;
 while(tmp->nx !=NULL) {
  pos =tmp;
  pt = tmp->nx;
  while( pt !=NULL) {
    if( Dcmpitems(pos->bf,pt->bf) > 0 ){ /* to be provided by user */
      pos=pt;
    }
    pt = pt->nx;
  }
  if(pos != tmp) {
      dum = tmp->bf;
      tmp->bf = pos->bf;
      pos->bf = dum;
  }
  tmp = tmp->nx;
 }
}
void Dsort(Dlink *LN , int Dcmpitems(void *,void *)) {
 D_l *tmp,*pt,*pos,*tmp1;
 void *dum;
  if(LN==NULL) {
    printf("Error: Empty listi:Dsort\n");
    exit(0);
  }
 tmp = LN->st;
 if(tmp==NULL) return;
 pt = tmp->nx;
 while(pt !=NULL) {
   pos = LN->st;
   while(pos != pt) {
      if( Dcmpitems(pos->bf,pt->bf) > 0 ){ /* to be provided by user */
        tmp1 = pt;
        (pt->pv)->nx = pt->nx;
        if(pt->nx != NULL) (pt->nx)->pv = pt->pv;
        else LN->en = pt->pv;
        pt = pt->nx;
        if(pos->pv != NULL)  (pos->pv)->nx = tmp1;
        tmp1->pv =pos->pv;
        pos->pv = tmp1;
        tmp1->nx = pos;
        if(pos == LN->st) LN->st = tmp1;
        break;
      }
      pos = pos->nx;
   }
   if(pos==pt) pt = pt->nx;
 }
}
#define  Dintrchngpositions(LN,pos,pt) {\
  D_l *tmp1; \
  tmp1 = pt; \
  (pt->pv)->nx = pt->nx; \
  if(pt->nx != NULL) (pt->nx)->pv = pt->pv; \
  else LN->en = pt->pv; \
  if(pos->pv != NULL)  (pos->pv)->nx = tmp1; \
  tmp1->pv =pos->pv; \
  pos->pv = tmp1; \
  tmp1->nx = pos; \
  if(pos == LN->st) LN->st = tmp1; \
}
void Dsort_iter(Dlink *LN , int Dcmpitems(void *,void *)) {
  D_l *tmp,*pt,*pos,*tmp1;
  void *dum;
  int n=1,cont1,cont2,cont3;
  if(LN==NULL) {
    printf("Error: Empty list:Dsort_iter\n");
    exit(0);
  }
  Item_comp = Dcmpitems;
  tmp = LN->st;
  if(tmp==NULL) return;
  pt = tmp->nx;
  if(pt==NULL) return;
  n++;
  pos = LN->st;
  if( Dcmpitems(pos->bf,pt->bf) > 0 ){ /* to be provided by user */
    tmp1 = pt->pv;
    Dintrchngpositions(LN,pos,pt);
    pt = tmp1;
  }
  pt = pt->nx;
  while(pt !=NULL) {
    n++;
    pos = LN->st;
    if( Dcmpitems(pos->bf,pt->bf) > 0 ){ /* to be provided by user */
      tmp1 = pt->pv;
      Dintrchngpositions(LN,pos,pt);
      pt = tmp1;
    }
    else {
      pos = pt->pv;
      if( Dcmpitems(pos->bf,pt->bf) > 0 ){ /* to be provided by user */
        tmp1 = pt->pv;
        cont1 = 1; cont2=n-1;
        cont3 = (cont1+cont2)/2;
        while( cont1 != cont3 ) {
           Dposition(LN,cont3);
           tmp = LN->cr;
           if( Dcmpitems(tmp->bf,pt->bf) > 0 ){ 
             pos = tmp; cont2 = cont3;
           }
           else { cont1 = cont3;}
           cont3 = (cont1+cont2)/2;
        }
        Dintrchngpositions(LN,pos,pt);
        pt = tmp1;
      }
    }
    pt = pt->nx;
  }
}
/*------------------------------------------------------
   Counting number of items
 ------------------------------------------------------ */
int Dcount(Dlink *F) {
  int n=0;
  D_l *LN;
  if(F==NULL) {
    printf("Error: Empty list:Dcount\n");
    exit(0);
  }
  LN = F->st;
  while( LN!= ((void *)0) ) {
   n++;
   LN = LN->nx;
  }
  return n;
}

/*------------------------------------------------------
  Positioning the list to the given number
  Number is counted from 1 (not 0 , take care)
 ------------------------------------------------------ */

void Dposition(Dlink *F,int n) {
  int i=0;
  D_l *LN;
  if (F==NULL) return;
  LN = F->st;
  if(n==0) {F->cr = F->st; return;}
  if(LN==(void *) 0){F->cr=NULL; return;}
  while( LN!= ((void *)0) ) {
   i++;
   if(i==n) break;
   LN = LN->nx;
  }
  if(LN == (void *) 0) LN = F->en;
  F->cr = LN;
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
   Dlink *Djoin(Dlink *d,Dlink *s){
     D_l *tmp;
     if(s==NULL) return d;
     if(d==NULL){
        /*gprintf("Destination is NULL");*/
        return s;
     }
     d->cr=d->en;
     tmp = s->st;
     while(tmp!=NULL) {
       Dadd(d,tmp->bf);
       tmp = tmp->nx;
     }
     return d;
   }
/*------------------------------------------------------
   Finding the sum of an item (integer) 
   Dsumitem returns the value of the item
   picked up from the record
 ------------------------------------------------------ */
int Disum(Dlink *LN , int Dsumitem(void *)) {
 D_l *tmp;
 int sum=0;
 tmp = LN->st;
 if(tmp==NULL){ return sum;}
 while(tmp !=NULL) {
  sum += Dsumitem(tmp->bf);
  tmp  = tmp->nx;
 }
 return sum;
}
/* 
   Finding the sum of an item (float) 
   Dsumitem returns the value of the item
   picked up from the record
*/
float Dfsum(Dlink *LN , float Dsumitem(void *)) {
 D_l *tmp;
 float sum=0;
 tmp = LN->st;
 if(tmp==NULL){ return sum;}
 while(tmp !=NULL) {
  sum += Dsumitem(tmp->bf);
  tmp  = tmp->nx;
 }
 return sum;
}
/* 
   Finding the sum of an item (double) 
   Dsumitem returns the value of the item
   picked up from the record
*/
double Ddsum(Dlink *LN , double Dsumitem(void *)) {
 D_l *tmp;
 double sum=0;
 tmp = LN->st;
 if(tmp==NULL) return sum;
 while(tmp !=NULL) {
  sum += Dsumitem(tmp->bf);
  tmp  = tmp->nx;
 }
 return sum;
}
int Dcomplist(Dlink *L1,Dlink *L2,int comprecord(void *tmp1,void *tmp2)) {
 D_l *t1, *t2;
 if(Dcount(L1) != Dcount(L2)) return 0;
 t1 = L1->st;
 t2 = L2->st;
 while(t1 != NULL) {
   if( comprecord(t1->bf,t2->bf)!= 1) return 0;
   t1=t1->nx;
   t2=t2->nx;
 }
 return 1;
}
int Dread(Dlink **L,FILE *fp,void * read_link(FILE *f)) {
 int count,i;
 void *buf;
 *L = Dopen();
 while((buf =read_link(fp)) != NULL) Dadd(*L,buf);
 return 1;
}
int Dwrite(Dlink *L,FILE *fp,int write_link(FILE *f,void *buf)) {
 int count,i;
 D_l *tmp;
 tmp = L->st;
 if(tmp==NULL) return 1;
 while(tmp !=NULL) {
  if(write_link(fp,tmp->bf) <=0 ) {
    return -1;
  }
  tmp  = tmp->nx;
 }
 return 1;
}
int Dreadauto(Dlink **L,char *flname, int size_of_rec){
 int count,i,ret;
 void *buf;
 FILE* fp;
 *L = Dopen();
 if( (fp =fopen(flname,"rb")) == NULL ) return 0;
 if( fread(&count,sizeof(int),1,fp) <= 0 ) {
   fclose(fp);
   return -1;
  }
 for(i=0;i<count;i++) {
   buf = (void *) malloc(size_of_rec);
   if(( fread(buf,size_of_rec,1,fp)) > 0 ) Dadd(*L,buf);
   else { 
     fclose(fp);return -1;
   }
 }
 fclose(fp);
 return 1;
}
int Dwriteauto(Dlink *L,char *flname, int size_of_rec){
 int count,i;
 D_l *tmp;
 FILE* fp;
 if ( (fp =fopen(flname,"wb")) ==NULL ) return 0;
 count = Dcount(L);
 if( fwrite(&count,sizeof(int),1,fp) <= 0 ) return -1;
 tmp = L->st;
 while(tmp !=NULL) {
  if(fwrite(tmp->bf,size_of_rec,1,fp) <= 0 ) {
    fclose(fp);
    printf("Error: In Dwriteauto ... Count=%d\n",count);
    exit(0);
  }
  tmp  = tmp->nx;
 }
 fclose(fp);
 return 1;
}
/*******************************************************
   removes duplicate records; all fields should be
   defined/initialised, otherwise will end up in error
   Also beware of character fields, unused area can
   create problem
********************************************************/

int Drmvdup(Dlink *L,int size){
 D_l *tmp,*tmp1;
 char *pt,*pt1;
 int i=0;
 tmp = L->st;
 while(tmp !=NULL) {
  pt = (char *)(tmp->bf);
  if(tmp !=L->en) {
    tmp1= tmp->nx;
    while(tmp1 != NULL) {
      pt1 = (char *)(tmp1->bf);
      for(i=0;i<size;i++) if( pt[i] != pt1[i]) break;
      if( i==size){
        L->cr=tmp1;
        if(tmp1 == L->en){
          Ddelete(L);
          break;
        } /* in case last one is deleted */
        Ddelete(L);
        tmp1 = L->cr;
      }
      else tmp1 = tmp1->nx;
    }
  }
  tmp  = tmp->nx;
 }
 return 1;
}
/*******************************************************
   removes duplicate records as per given rule;
********************************************************/

int Drmvdup_cond(Dlink *L,int rule(void *,void *)){
 D_l *tmp,*tmp1;
 tmp = L->st;
 while(tmp !=NULL) {
  if(tmp !=L->en) {
    tmp1= tmp->nx;
    while(tmp1 != NULL) {
      if( rule(tmp->bf,tmp1->bf)==1) {
        L->cr=tmp1;
        if(tmp1 == L->en){
          Ddelete(L);
          break;
        } /* in case last one is deleted */
        Ddelete(L);
        tmp1 = L->cr;
        if(tmp1 == L->en) break;
      }
      else tmp1 = tmp1->nx;
    }
  }
  tmp  = tmp->nx;
 }
 return 1;
}
/* Added on 23rd Sept 1999 */
/*************************************************************** 
  Setting link's current position to the start
****************************************************************/
void *Resetlink(Dlink *L){
  if(L== NULL) return NULL;
  if(L->st == NULL ) return NULL;
  L->cr = L->st;
  return L->cr->bf;
}
/****************************************************************
  getting current record and changing to next record
  returns NULL if there is no record
*****************************************************************/
void *Getrecord(Dlink *L) {
  void *tmp=NULL;
  if((L!= NULL)&&(L->cr != NULL)) {
    tmp = L->cr->bf;
    L->cr = L->cr->nx;
  }
  return tmp;
}
/****************************************************************
  getting current record and changing to previous record
  returns NULL if there is no record
*****************************************************************/
void *Getrecordrev(Dlink *L) {
  void *tmp=NULL;
  if((L!= NULL)&&(L->cr != NULL)) {
    tmp = L->cr->bf;
    L->cr = L->cr->pv;
  }
  return tmp;
}
