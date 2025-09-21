/*
   Version 3.0
   Dated 04/02/14
*/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include "kulina.h"
/*

typedef  struct f_L {
         Dlink *L;
         int C_pos;
         char Name[120];
} File;


File * Fopen(char *flname);
void copy_to_file(File *F,char *flname);
void append_to_file(File *F,char *flname);
void insert_file(File *LN,char *flname) ;
void delete_lines(File *LN,int n) ;
void move_forward(File *LN,int n) ;
void move_back(File *LN,int n) ;
void Rewind(File *F) ;
void Eoffile(File *F) ;
int Ungetc(int ch,File *F);
int Getc(File *F);
int Get_line(char *bf,File *F) ;
void rmv_right_blnk(File *F);
void Print_file(File *F);
File * Fcreate(char *flname);
int append_line(File *F,char *buf);
void insert_line(File *F,char *buf);
void add_line(File *F,char *buf);
void Fclose(File *F);
void Empty(File *F);
*/

 



File * Fopen(char *flname){
  int i,j;
  FILE *fp;
  File *F;
  Dlink *LN ;
  char buf[600],*bf;
  LN = ((void *)0) ;
  if((fp = fopen(flname,"r"))== ((void *)0) ) return ((void *)0) ;
  LN=Dopen();
  while ( fgets(buf,600,fp) != NULL) {
   i=0;while(buf[i] != '\n') i++;
   j=i-1;   
   buf[++j]='\n';
   buf[++j]='\0';
   bf=(char *)Malloc(j+1);
   strcpy(bf,buf);
   Dadd(LN,bf);
  }
  fclose(fp);
  F = (File *)Malloc(sizeof(File));
  F->L = LN;
  if( strlen(flname)>119 ) {
//    normal();
    printf("Error: long file name in Fcreate/Fopen %s\n",flname);
    exit(0);
  }
  Resetlink(F->L);
  F->C_pos=0;
  strcpy(F->Name,flname);
  return(F);
}

 



File * Fcreate(char *flname){
  int i,j;
  FILE *fp;
  File *F;
  F = (File *)Malloc(sizeof(File));
  F->L = Dopen();
  if( strlen(flname)>119 ) {
    printf("Error: long file name in Fcreate/Fopen %s\n",flname);
    exit(0);
  }
  F->C_pos=0;
  strcpy(F->Name,flname);
  return(F);
}

 





int append_line(File *F,char *buf){
  int l;
  char *bf;
  l=0;
  if(F== NULL) {
    fprintf(stderr,"F==NULL\n");
    return 0;
  }
  while( buf[l]!='\0')l++;
  bf=(char *)Malloc(l+1);
  strcpy(bf,buf);
  Dappend(F->L,bf);
  F->C_pos=0;
  return l;
}

 



void Fclose(File *F){
  int i,j;
  if(F==NULL) {
    fprintf(stderr,"Fclose: F==NULL\n");
    return;
  }
  copy_to_file(F,F->Name);
  Dempty(F->L);
  free(F);
}

 



void Empty(File *F){
  int i,j;
  if(F==NULL) {
    fprintf(stderr,"Empty: F==NULL\n");
    return;
  }
  Dempty(F->L);
  free(F);
}

void Print_file(File *F){
  int i,j;
  char *bf;
  if(F==NULL) {
    fprintf(stderr,"Print_file: F==NULL\n");
    return;
  }
  Resetlink(F->L);
  while( (bf=(char *)Getrecord(F->L)) != NULL) {
   printf("%s",bf);
  }
  Resetlink(F->L);
  F->C_pos=0;
}

void rmv_right_blnk(File *F){
  int i,j;
  char *bf;
  if(F==NULL) {
    fprintf(stderr," F==NULL\n");
    return;
  }
  Resetlink(F->L);
  while( (bf=(char *)Getrecord(F->L)) != NULL) {
   i = strlen(bf);
   j=i-1;while( (j>=0) &&(bf[j]<= ' '))j--;  
   bf[++j]='\n';
   bf[++j]='\0';
  }
  Resetlink(F->L);
  F->C_pos=0;
}

 



int Get_line(char *bf,File *F) {
  int ret=0;
  char *ln;
  if(F== ((void *)0) ) return -1;
  ln = (char *)Getrecord(F->L);
  if(ln ==NULL) return -1;
  strcpy(bf , ln);
  ret = strlen(bf);
  F->C_pos=0;
  return ret;
}

 



int Getc(File *F){
  int ret;
  char *ln;
  if(F== ((void *)0) ) return -1;
  ln = (char *)Getrecord(F->L);
  ret = *(ln+F->C_pos);
  F->C_pos +=1;
//  if(ln[F->C_pos] <' ') {
  if(ln[F->C_pos] =='\0') {
    F->C_pos=0;
  }
  else Dmove_back(F->L,1);
  return ret;
}

 



int Ungetc(int ch,File *F){
  char *ln;
  if(F== ((void *)0) ) return -1;
  F->C_pos -=1;
  while( F->C_pos < 0) {
    Dmove_back(F->L,1);
    ln = (char *) Getrecord(F->L);
    if(ln== NULL) {F->C_pos=0;break;}
//    F->C_pos=strlen(ln)-2;
    F->C_pos=strlen(ln)-1;
    Dmove_back(F->L,1);
  }
  return 1;
}

 



void Rewind(File *F) {
 if(F==NULL) return;
 Resetlink(F->L);
 F->C_pos= 0;
}

 



void Eoffile(File *F) {
 if(F==NULL) return;
 Dend(F->L);
 F->C_pos= 0;
}
 



void move_back(File *F,int n) {
 int i;
 if(F==NULL) return;
 Dmove_back(F->L,n);
 F->C_pos=0;
}


 



void move_forward(File *F,int n) {
 int i;
 if(F==NULL) return;
 Dmove_forward(F->L,n);
 F->C_pos=0;
}

void delete_lines(File *F,int n) {
 int i;
 if(F==NULL) return;
 Ddelete_items(F->L,n);
 F->C_pos=0;
}

void insert_file(File *F,char *flname) {
 int i;
 char *bf;
 File *tmp;
 if(F==NULL) return;
 if( (tmp = Fopen(flname))== ((void *)0) ) return;
 Resetlink(tmp->L);
 Dmove_back(F->L,1);
 while( (bf= (char *)Getrecord(tmp->L))!= NULL) Dadd(F->L,bf);
 F->C_pos=0;
 Dfree(tmp->L);
 free(tmp);
}

void insert_line(File *F,char *buf) {
 int l;
 char *bf;
 if(F==NULL) return;
 l=0;while(buf[l]!='\0')l++;
 bf=(char *)Malloc(l+1);
 strcpy(bf,buf);
 Dinsert(F->L,bf);
 F->C_pos=0;
}

void add_line(File *F,char *buf) {
 int l;
 char *bf;
 if(F==NULL) return;
 l=0;while(buf[l]!='\0')l++;
 bf=(char *)Malloc(l+1);
 strcpy(bf,buf);
 Dadd(F->L,bf);
 F->C_pos=0;
}
void copy_to_file(File *F,char *flname){
  int i,j;
  FILE *fp;
  char *bf;
  if(F==NULL) return;
  fp=fopen(flname,"w");
  Resetlink(F->L);
  while( (bf= (char *)Getrecord(F->L))!= NULL) {
   fprintf(fp,"%s",bf);
  }
  Resetlink(F->L);
  F->C_pos=0;
  fclose(fp);
}
void append_to_file(File *F,char *flname){
  int i,j;
  FILE *fp;
  char *bf;
  if(F==NULL) return;
  fp=fopen(flname,"a");
  Resetlink(F->L);
  while( (bf= (char *)Getrecord(F->L))!= NULL) {
   fprintf(fp,"%s",bf);
  }
  Resetlink(F->L);
  F->C_pos=0;
  fclose(fp);
}
/*
int  Fprintf(fp,va_alist)
File *fp;
  va_dcl
*/
int Fprintf(void * unknown,...)
 {
  union {
    long l;
    int i;
    char c;
    float f;
    double d;
    char *s;
  } value;
  File *fp;
  long i,item=0,j;
  char *tmp,*pt,*cpt;
  char *str;
  char ch,wrk[300],buff[200],buf[300];
  va_list ad;
  wrk[0]='\0';
  va_start(ad,unknown);
  j=0;
  fp = (File *) unknown;
  tmp = va_arg(ad,char *);
  strcpy(buf,tmp);
  cpt=buf;
  pt=cpt;
  while( *cpt != '\0'){
   if((*cpt == '%')&&(*(cpt+1)!='%')) {
    cpt++;
    while(isdigit(*cpt)||(*cpt=='.')||(*cpt=='-')){ cpt++; }
    switch(*cpt) {
     case 'c':
       while( (*cpt != '\0')&& (*cpt != '%')) cpt++;
       ch = *cpt;*cpt='\0';
       value.c= va_arg(ad,int  );
       sprintf(buff,pt,value.c);
       strcat(wrk,buff);
       *cpt = ch;
       break;
     case 's':
       while( (*cpt != '\0')&& (*cpt != '%')) cpt++;
       ch = *cpt;*cpt='\0';
       value.s= va_arg(ad,char *);
       sprintf(buff,pt,value.s);
       strcat(wrk,buff);
       *cpt = ch;
       break;
     case 'f':
     case 'g':
     case 'e':
       while( (*cpt != '\0')&& (*cpt != '%')) cpt++;
       ch = *cpt;*cpt='\0';
       value.f= va_arg(ad,double);
       sprintf(buff,pt,value.f);
       strcat(wrk,buff);
       *cpt = ch;
       break;
     case 'd':
       while( (*cpt != '\0')&& (*cpt != '%')) cpt++;
       ch = *cpt;*cpt='\0';
       value.i= va_arg(ad,int);
       sprintf(buff,pt,value.i);
       strcat(wrk,buff);
       *cpt = ch;
       break;
     case 'l':
       cpt++;
       switch(*cpt) {
        case 'f':
        case 'g':
        case 'e':
          while( (*cpt != '\0')&& (*cpt != '%')) cpt++;
          ch = *cpt;*cpt='\0';
          value.d= va_arg(ad,double);
          sprintf(buff,pt,value.d);
          strcat(wrk,buff);
          *cpt = ch;
          break;
        case 'd':
          while( (*cpt != '\0')&& (*cpt != '%')) cpt++;
          ch = *cpt;*cpt='\0';
          value.l= va_arg(ad,long);
          sprintf(buff,pt,value.l);
          strcat(wrk,buff);
          *cpt = ch;
          break;
        default :
          printf("Error: In Fprintf/  %s |%c|/\n",pt,*cpt);
          exit(0);
       }
       break;
     default :
       printf("Error: In Fprintf/  %s |%c|/\n",pt,*cpt);
       exit(0);
    }
    pt=cpt;
   }
   else cpt++;
  }
  if(pt!=NULL)strcat(wrk,pt);
  add_line(fp,wrk);
 }
