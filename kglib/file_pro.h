
typedef  struct f_L {
         Dlink *L;
         int C_pos;
         char Name[300];
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

 



File * Fopen(char *flname){
  int i,j;
  FILE *fp;
  File *F;
  F_l *LN,*st= ((void *)0) ,*tmp= ((void *)0) ,*en= ((void *)0) ;
  char buf[600];
  LN = ((void *)0) ;
  if((fp = fopen(flname,"r"))== ((void *)0) ) return ((void *)0) ;
  while ( fgets(buf,600,fp) > 0) {
   i=0;while(buf[i] != '\n') i++;
   j=i-1;   
   buf[++j]='\n';
   buf[++j]='\0';
   if(LN== ((void *)0) ) {
    LN = (struct ln *)malloc(sizeof(struct ln));
    st = LN;
    en =LN;
    LN->l=j;
    LN->nx= ((void *)0) ;
    LN->pv= ((void *)0) ;
    LN->bf=(char *)malloc(j+1);
    strcpy(LN->bf,buf);
   }
   else{
    LN->nx= (struct ln *)malloc(sizeof(struct ln));
    tmp = LN;
    LN = LN->nx;
    LN->pv=tmp;
    LN->nx= ((void *)0) ;
    LN->l=j;
    LN->bf=(char *)malloc(j+1);
    strcpy(LN->bf,buf);
    en =LN;
   }
  }
  fclose(fp);
  F = (File *)malloc(sizeof(File));
  F->st= st;
  F->cr= st;
  F->en= en;
  F->C_pos=0;
  strcpy(F->Name,flname);
  return(F);
}

 



File * Fcreate(char *flname){
  int i,j;
  FILE *fp;
  File *F;
  F_l *LN,*st= ((void *)0) ,*tmp= ((void *)0) ,*en= ((void *)0) ;
  LN = ((void *)0) ;
  F = (File *)malloc(sizeof(File));
  F->st= ((void *)0) ;
  F->cr= ((void *)0) ;
  F->en= ((void *)0) ;
  F->C_pos=0;
  strcpy(F->Name,flname);
  return(F);
}

 





int append_line(File *F,char *buf){
  int l;
  F_l *LN,*tmp;
  l=0;
  while( buf[l]!='\0')l++;
  LN = F->en;
  if(LN== ((void *)0) ) {
    LN = (struct ln *)malloc(sizeof(struct ln));
    F->st = LN;
    F->en =LN;
    F->cr =F->en;
    F->C_pos =0;
    LN->l=l;
    LN->nx= ((void *)0) ;
    LN->pv= ((void *)0) ;
    LN->bf=(char *)malloc(l+1);
    strcpy(LN->bf,buf);
  }
  else{
    LN->nx= (struct ln *)malloc(sizeof(struct ln));
    tmp = LN;
    LN = LN->nx;
    LN->pv=tmp;
    LN->nx= ((void *)0) ;
    LN->l=l;
    LN->bf=(char *)malloc(l+1);
    strcpy(LN->bf,buf);
    F->en =LN;
    F->cr =F->en;
    F->C_pos =0;
  }
}

 



void Fclose(File *F){
  int i,j;
  F_l *LN,*tmp;
  copy_to_file(F,F->Name);
  LN = F->st;
  while( LN!= ((void *)0) ) {
   tmp = LN->nx;
   free(LN->bf); free(LN);
   LN=tmp;
  }
  free(F);
}

 



void Empty(File *F){
  int i,j;
  F_l *LN,*tmp;
  LN = F->st;
  while( LN!= ((void *)0) ) {
   tmp = LN->nx;
   free(LN->bf); free(LN);
   LN=tmp;
  }
  free(F);
}

 



void Print_file(File *F){
  int i,j;
  F_l *LN,*tmp;
  LN = F->st;
  while( LN!= ((void *)0) ) {
   tmp = LN->nx;
   printf("%s",LN->bf);
   LN=tmp;
  }
}

 



void rmv_right_blnk(File *F){
  int i,j;
  F_l *LN,*tmp;
  LN = F->st;
  while( LN!= ((void *)0) ) {
   i = LN->l-1;
   j=i-1;while( (j>0) &&(LN->bf[j]== ' '))j--;  
   LN->bf[++j]='\n';
   LN->bf[++j]='\0';
   LN->l=j;
   LN = LN->nx;
  }
}

 



int Get_line(char *bf,File *F) {
  int ret;
  if(F->cr== ((void *)0) ) return -1;
  ret= F->cr->l;
  strcpy(bf , F->cr->bf+F->C_pos);
  F->cr= F->cr->nx;
  F->C_pos=0;
  return ret;
}

 



int Getc(File *F){
  int ret;
  if(F->cr== ((void *)0) ) return -1;
  ret = *(F->cr->bf+F->C_pos);
  F->C_pos +=1;
  if( F->C_pos == F->cr->l) {
    F->cr = F->cr->nx;
    F->C_pos=0;
  }
  return ret;
}

 



int Ungetc(int ch,File *F){
  if(F->cr== ((void *)0) ) return -1;
  F->C_pos -=1;
  if( F->C_pos < 0) {
    if( F->cr->pv== ((void *)0) ){F->C_pos =0; return -1;};
    F->cr = F->cr->pv;
    F->C_pos=F->cr->l-1;
  }
  return 1;
}

 



void Rewind(File *F) {
 F->cr = F->st;
 F->C_pos= 0;
}

 



void Eoffile(File *F) {
 F->cr = F->en;
 F->C_pos= 0;
}
 



void move_back(File *LN,int n) {
 int i;
 F_l *pt;
 if(LN->cr == LN->st ) return;
 if(LN->st == ((void *)0) ) return;
 if(LN->cr == ((void *)0) ){ LN->cr=LN->en;n--;}
 pt = LN->cr->pv;
 for ( i=0; i<n;i++) {
  if (pt== ((void *)0) ) break;
  LN->cr=pt;
  LN->C_pos=0;
  pt = LN->cr->pv;
 }
}


 



void move_forward(File *LN,int n) {
 int i;
 F_l *pt;
 if(LN->en==LN->cr) return;
 pt = LN->cr->nx;
 for ( i=0; i<n;i++) {
  if (pt== ((void *)0) ) break;
  LN->cr=pt;
  LN->C_pos=0;
  pt = LN->cr->nx;
 }
}

 



void delete_lines(File *LN,int n) {
 int i;
 F_l *pt,*pv,*nx;
 pt = LN->cr->nx;
 pv = LN->cr->pv;
 for ( i=0; i<n;i++) {
  if (pt== ((void *)0) ) break;
  free(LN->cr->bf);
  free(LN->cr);
  LN->cr=pt;
  LN->C_pos=0;
  pt = LN->cr->nx;
 }
 pt=LN->cr;
 if (i<n) {
  if(pv != ((void *)0) ) {LN->cr=pv;pv->nx= ((void *)0) ;LN->en=pv;}
  else { LN->cr= ((void *)0) ; LN->st= ((void *)0) ; LN->en= ((void *)0) ;}
 }
 else {
  if(pv != ((void *)0) ) pv->nx=pt;
  else {LN->cr->pv= ((void *)0) ;LN->C_pos=0;LN->st=LN->cr;}
  if(pt!= ((void *)0) ) pt->pv=pv;
 }
}

 




void insert_file(File *LN,char *flname) {
 int i;
 File *tmp;
 F_l *pt,*pv,*nx;
 if( (tmp = Fopen(flname))== ((void *)0) ) return;
 if(LN->st== ((void *)0) ) {
    LN->st=tmp->st;
    LN->cr=tmp->st;
    LN->en=tmp->en;
    LN->C_pos=0;
 }
 else {
  if(LN->cr== ((void *)0) ) {LN->cr=LN->en;LN->en=tmp->en;}
  nx = LN->cr->nx;
  pv = LN->cr;
  pt= tmp->st;
  (LN->cr)->nx=pt;
  LN->cr->nx->pv=LN->cr;
  tmp->en->nx=nx;
 }
 free(tmp);
}

 




void insert_line(File *F,char *buf) {
 int l;
 F_l *LN,*tmp;
 if(F->cr== ((void *)0) ) F->cr==F->en;
 if((F->st== ((void *)0) )||(F->cr== ((void *)0) )) {
   append_line(F,buf);
   return;
 }
 else {
    l=0;while(buf[l]!='\0')l++;
    LN = F->cr;
    tmp = (struct ln *)malloc(sizeof(struct ln));
    tmp->nx=LN;
    tmp->pv=LN->pv;
    tmp->l=l;
    tmp->bf=(char *)malloc(l+1);
    strcpy(tmp->bf,buf);
    if(tmp->pv == ((void *)0) ) { F->st=tmp; }
    else {tmp->pv->nx=tmp;}
    F->cr=tmp;
    LN->pv=tmp;
 }
}

 




void add_line(File *F,char *buf) {
 int l;
 F_l *LN,*tmp;
 if( F->en == F->cr ) F->cr= ((void *)0) ;
 if((F->st== ((void *)0) )||(F->cr== ((void *)0) )) {
   append_line(F,buf);
   return;
 }
 else {
    l=0;while(buf[l]!='\0')l++;
    LN = F->cr->nx;
    tmp = (struct ln *)malloc(sizeof(struct ln));
    tmp->nx=LN;
    tmp->pv=LN->pv;
    tmp->l=l;
    tmp->bf=(char *)malloc(l+1);
    strcpy(tmp->bf,buf);
    tmp->pv->nx=tmp;
    F->cr=tmp;
    LN->pv=tmp;
 }
}
void copy_to_file(File *F,char *flname){
  int i,j;
  FILE *fp;
  F_l *LN,*tmp;
  fp=fopen(flname,"w");
  LN = F->st;
  while( LN!= ((void *)0) ) {
   tmp = LN->nx;
   fprintf(fp,"%s",LN->bf);
   LN=tmp;
  }
  fclose(fp);
}
void append_to_file(File *F,char *flname){
  int i,j;
  FILE *fp;
  F_l *LN,*tmp;
  fp=fopen(flname,"a");
  LN = F->st;
  while( LN!= ((void *)0) ) {
   tmp = LN->nx;
   fprintf(fp,"%s",LN->bf);
   LN=tmp;
  }
  fclose(fp);
}
