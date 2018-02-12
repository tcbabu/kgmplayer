/*
   Version 2.1
   Dated 12/07/97
*/

#ifndef D_LINK
#define D_LINK
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
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



Dlink * Dopen(void);
int Ddup( Dlink *S,Dlink *D);
int  Dappend(Dlink *F,void  *buf);
void Dempty(Dlink *F);
void Dfree(Dlink *F);
void Drewind(Dlink *F) ;
void Dend (Dlink *F) ;
void Dmove_back(Dlink *LN,int n) ;
void Dmove_forward(Dlink *LN,int n) ;
void Ddelete_items(Dlink *LN,int n) ;
void Ddelete(Dlink *LN) ;
void Dinsert(Dlink *F,void  *buf) ;
void Dadd(Dlink *F,void  *buf) ;
Dlink *Dcopy(Dlink *LN) ;
Dlink *Dsublist(Dlink *LN,void *s,int Dcondition(void *,void *)) ;
Dlink *Dnewlist(Dlink *LN,void * Dnewrule(void *)) ;
void Dsort(Dlink *LN , int Dcmpitems(void *,void *)) ;
int Dcount(Dlink *F) ;
void Dposition(Dlink *F,int n) ;
int Dsetcurpos(Dlink *LN,void *s,int Dcondition(void *,void *)) ;
Dlink *Djoin(Dlink *d,Dlink *s);
int Disum(Dlink *LN , int Dsumitem(void *)) ;
float Dfsum(Dlink *LN , float Dsumitem(void *)) ;
double Ddsum(Dlink *LN , double Dsumitem(void *)) ;
int comp_list(Dlink *L1,Dlink *L2,int comprecord(void *tmp1,void *tmp2)) ;
void Dcondition(Dlink *F,void *arg, int condition(void *,void *));
int Dread(Dlink **L,FILE *f,void * read_link(FILE *fp));
int Dwrite(Dlink *L,FILE *f,int write_link(FILE *fp,void *buf));
int Dreadauto(Dlink **L,char *flname,int size_of_rec);
int Dwriteauto(Dlink *L,char *flname,int size_of_rec);
int Drmvdup(Dlink *L,int size_of_rec);
int Drmvdup_cond(Dlink *L,int rule(void *,void *));
    /* if rule returns 1 records are same */
void *Resetlink(Dlink *L);
void *Getrecord(Dlink *L);
void *Getrecordrev(Dlink *L);
void* Dpick(Dlink *LN) ; /* like Ddelete but does not free */
#endif /* end of dlink.h */
