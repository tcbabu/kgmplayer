#include "kulina.h"
#include "dlink.h"
#include <stdlib.h>
#include <stdarg.h>
#include "string.h"
static void StringCopy(char *d,char *s) {
  int i=0,l,j;
  while((s[i]>=' ')&&(s[i]!= '"')) { i++; }
  if(s[i] < ' ') {
    d[0] ='\0';
    return;
  }
  else {
    i++;
    j= i;
    while((s[i]>=' ')&&(s[i]!= '"')) { i++; }
    if(s[i] < ' ') {
      d[0] ='\0';
      return;
    }
    else {
      s[i]='\0';
      l = strlen(s+j);
      strcpy(d,s+j);
    }
  }
}

static int NULLDATA(char *buff) {
 if((buff[0]=='N')&&(buff[1]=='U')&&(buff[2]=='L')
                            &&(buff[3]=='L')) return 1;
 else return 0;
}

static int Get_data_line(FILE *fp,char *buff) {
 int l,i,OK=0;
 if(fgets(buff,199,fp)==NULL) return -1;
 l = strlen(buff);
 while(l>1) {
  if((buff[0]=='/')&&(buff[1]=='/')) {
    if(fgets(buff,199,fp)==NULL) return -1;
    l = strlen(buff);
  }
  else break;
 }
 i=l-1;
 while( (i>=1)){
  if( (buff[i]=='/')&&(buff[i-1]=='/')) {OK=1;break;}
  else i--;
 }
 if(OK) buff[i-1]='\0';
 return strlen(buff);
}
#define GETDATALINE  if(Get_data_line(fp,buff) < 0 ) {\
    printf("Error : In reading.... %s\n",buff);\
    exit(0); \
}

void Read_Gclr(Gclr *gc,char *app) {
  char buff[200],code[30];
  FILE *fp;
  int i;
  char flname[200];
  strcpy(flname,getenv("HOME"));
  strcat(flname,"/.");
  strcat(flname,app);
  strcat(flname,"gc");
  fp = fopen(flname,"r");
  if(fp==NULL) return;
  while (fgets(buff,190,fp) != NULL) {
    if(buff[0]=='#') continue;
    i=0; while( buff[i]==' ') i++;
    if(buff[i]< ' ') continue;
    sscanf(buff+i,"%s",(char *)code);
    if(strcmp(code,"char_clr") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->char_clr));
      continue;
    }
    if(strcmp(code,"fill_clr") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->fill_clr));
      continue;
    }
    if(strcmp(code,"bodr_clr") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->bodr_clr));
      continue;
    }
    if(strcmp(code,"cur_clr") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->cur_clr));
      continue;
    }
    if(strcmp(code,"high_clr") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->high_clr));
      continue;
    }
    if(strcmp(code,"char_hclr") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->char_hclr));
      continue;
    }
    if(strcmp(code,"msg_char") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->msg_char));
      continue;
    }
    if(strcmp(code,"msg_fill") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->msg_fill));
      continue;
    }
    if(strcmp(code,"msg_bodr") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->msg_bodr));
      continue;
    }
    if(strcmp(code,"txt_fill") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->txt_fill));
      continue;
    }
    if(strcmp(code,"txt_char") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->txt_char));
      continue;
    }
    if(strcmp(code,"txt_pchar") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->txt_pchar));
      continue;
    }
    if(strcmp(code,"tabl_fill") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->tabl_fill));
      continue;
    }
    if(strcmp(code,"tabl_line") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->tabl_line));
      continue;
    }
    if(strcmp(code,"tabl_char") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->tabl_char));
      continue;
    }
    if(strcmp(code,"tabl_hchar") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->tabl_hchar));
      continue;
    }
    if(strcmp(code,"vdim") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->v_dim));
      continue;
    }
    if(strcmp(code,"dim") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->dim));
      continue;
    }
    if(strcmp(code,"bright") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->bright));
      continue;
    }
    if(strcmp(code,"vbright") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->vbright));
      continue;
    }
    if(strcmp(code,"twin_fill") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->twin_fill));
      continue;
    }
    if(strcmp(code,"twin_char") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->twin_char));
      continue;
    }
    if(strcmp(code,"twin_bodr") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->twin_bodr));
      continue;
    }
    if(strcmp(code,"info_fill") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->info_fill));
      continue;
    }
    if(strcmp(code,"info_char") == 0) {
      while( (buff[i]!='=')&&(buff[i] >= ' ')) i++;
      if(buff[i] != '=') continue;
      sscanf(buff+i+1,"%d",&(gc->info_char));
      continue;
    }
    
  }
  fclose(fp);

}
void Write_Gclr(Gclr gc,char *app) {
  FILE *fp;
  int i;
  char flname[200];
  strcpy(flname,getenv("HOME"));
  strcat(flname,"/.");
  strcat(flname,app);
  strcat(flname,"gc");
  fp = fopen(flname,"w");
  if(fp==NULL) return;
  fprintf(fp,"char_clr = %d\n",gc.char_clr);
  fprintf(fp,"fill_clr = %d\n",gc.fill_clr);
  fprintf(fp,"bodr_clr = %d\n",gc.bodr_clr);
  fprintf(fp,"cur_clr = %d\n",gc.cur_clr);
  fprintf(fp,"high_clr = %d\n",gc.high_clr);
  fprintf(fp,"char_hclr = %d\n",gc.char_hclr);
  fprintf(fp,"msg_char = %d\n",gc.msg_char);
  fprintf(fp,"msg_fill = %d\n",gc.msg_fill);
  fprintf(fp,"msg_bodr = %d\n",gc.msg_bodr);
  fprintf(fp,"txt_fill = %d\n",gc.txt_fill);
  fprintf(fp,"txt_char = %d\n",gc.txt_char);
  fprintf(fp,"txt_pchar = %d\n",gc.txt_pchar);
  fprintf(fp,"tabl_fill = %d\n",gc.tabl_fill);
  fprintf(fp,"tabl_line = %d\n",gc.tabl_line);
  fprintf(fp,"tabl_char = %d\n",gc.tabl_char);
  fprintf(fp,"tabl_hchar = %d\n",gc.tabl_hchar);
  fprintf(fp,"vdim = %d\n",gc.v_dim);
  fprintf(fp,"dim = %d\n",gc.dim);
  fprintf(fp,"bright = %d\n",gc.bright);
  fprintf(fp,"vbright = %d\n",gc.vbright);
  fprintf(fp,"twin_fill = %d\n",gc.twin_fill);
  fprintf(fp,"twin_char = %d\n",gc.twin_char);
  fprintf(fp,"twin_bodr = %d\n",gc.twin_bodr);
  fprintf(fp,"info_fill = %d\n",gc.info_fill);
  fprintf(fp,"info_char = %d\n",gc.info_char);
  fclose(fp);

}
void Application_Gclr(char *app) {
#if 0
  Gclr gc;
  gc = get_gui_colours();
  Read_Gclr(&gc,app);
  set_gui_colours(gc);
#endif
}
static void _uiPrintWid(FILE *fp,char *Wid) {
   Wid[47]='\0';
   fprintf(fp,"\"%-s\"\n",Wid);
   return;
}
static void _uiReadWid(FILE *fp,char *Wid) {
   char buff[200];
   GETDATALINE;
   buff[49]='\0';
   StringCopy(Wid,buff);
   return;
}
void Print_data_message(FILE *fp,DIM *m) {
  fprintf(fp,"%c     //code\n",m->code);
  fprintf(fp,"%d %d  //x1,y1\n",m->x1,m->y1);
  fprintf(fp,"\"%-s\"/Message\n",m->msg);
  fprintf(fp,"%d %d //x2,y2\n",m->x2,m->y2);
  fprintf(fp,"%d %d //justification,hide\n",m->justification,m->hide);
  _uiPrintWid(fp,m->Wid);
}
DIM * Read_data_message(FILE *fp) {
  int i, n,l;
  DIM *t;
  char buff[500];
  t = (DIM *) Malloc(sizeof(DIM));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  t->y1 = t->y1;
  GETDATALINE;
  StringCopy(t->msg,buff);
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->justification),&(t->hide));
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_progressbar(FILE *fp,DIO *o) {
  fprintf(fp,"%c     //code\n",o->code);
  fprintf(fp,"%d %d  //x1,y1\n",o->x1,o->y1);
  fprintf(fp,"%d %d //x2,y2\n",o->x2,o->y2);
  fprintf(fp,"%d //hide\n",o->hide);
  _uiPrintWid(fp,o->Wid);
}
DIO * Read_data_progressbar(FILE *fp) {
  int i, n,l;
  DIO *t;
  char buff[500];
  t = (DIO *) Malloc(sizeof(DIO));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  t->y1 = t->y1;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  t->y2 = t->y2;
  GETDATALINE;
  sscanf(buff,"%d",&(t->hide));
  t->percent=50;
  _uiReadWid(fp,t->Wid);
  t->clr=-1;
  t->type =0;
  t->bordr =0;
  t->direction =0;
  return t;
}

void Print_data_vertscroll(FILE *fp,DIV *o) {
  fprintf(fp,"%c     //code\n",o->code);
  fprintf(fp,"%d %d  //x1,y1\n",o->x1,o->y1);
  fprintf(fp,"%d %d //x2,y2\n",o->x2,o->y2);
  fprintf(fp,"%d //hide\n",o->hide);
  _uiPrintWid(fp,o->Wid);
}
DIV * Read_data_vertscroll(FILE *fp) {
  int i, n,l;
  DIV *t;
  char buff[500];
  t = (DIV *) Malloc(sizeof(DIV));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  t->y1 = t->y1;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  t->y2 = t->y2;
  GETDATALINE;
  sscanf(buff,"%d",&(t->hide));
  t->ds=100.;
  t->df=0.;
  t->mvmt=5.;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_horizscroll(FILE *fp,DIZ *o) {
  fprintf(fp,"%c     //code\n",o->code);
  fprintf(fp,"%d %d  //x1,y1\n",o->x1,o->y1);
  fprintf(fp,"%d %d //x2,y2\n",o->x2,o->y2);
  fprintf(fp,"%d //hide\n",o->hide);
  _uiPrintWid(fp,o->Wid);
}
DIZ * Read_data_horizscroll(FILE *fp) {
  int i, n,l;
  DIZ *t;
  char buff[500];
  t = (DIZ *) Malloc(sizeof(DIZ));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  t->y1 = t->y1;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  t->y2 = t->y2;
  GETDATALINE;
  sscanf(buff,"%d",&(t->hide));
  t->ds=100.;
  t->df=0.;
  t->mvmt=5.;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_floatslide(FILE *fp,DIF *f) {
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%lf %lf  //min,max\n",f->min,f->max);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d //length \n",f->lngth);
  if(f->prompt != NULL) fprintf(fp,"\"%-s\" //Prompt\n",f->prompt);
  else fprintf(fp,"NULL //prompt \n");
  fprintf(fp,"%d %d //x2,y2\n",f->x2,f->y2);
  fprintf(fp,"%d //hide\n",f->hide);
  _uiPrintWid(fp,f->Wid);
}
DIF * Read_data_floatslide(FILE *fp) {
  DIF *t;
  char buff[500];
  t = (DIF *) Malloc(sizeof(DIF));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%lf%lf",&(t->min),&(t->max));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  t->y1 = t->y1;
  GETDATALINE;
  sscanf(buff,"%d",&(t->lngth));
  GETDATALINE;
  if(!NULLDATA(buff)){
    t->prompt = (char *)Malloc(200);
    StringCopy(t->prompt,buff);
  }
  else t->prompt = NULL;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d",&(t->hide));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  t->y2 = t->y2;
  t->df = (double *)Malloc(sizeof(double));
  *((double *)(t->df)) = 1;
  t->arg = NULL;
  t->Update = NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_intslide(FILE *fp,DID *f) {
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //min,max\n",f->min,f->max);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d //length \n",f->lngth);
  if(f->prompt != NULL) fprintf(fp,"\"%-s\" //Prompt\n",f->prompt);
  else fprintf(fp,"NULL //prompt \n");
  fprintf(fp,"%d %d //x2,y2\n",f->x2,f->y2);
  fprintf(fp,"%d //hide\n",f->hide);
  _uiPrintWid(fp,f->Wid);
}
DID * Read_data_intslide(FILE *fp) {
  DID *t;
  char buff[500];
  t = (DID *) Malloc(sizeof(DID));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->min),&(t->max));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  t->y1 = t->y1;
  GETDATALINE;
  sscanf(buff,"%d",&(t->lngth));
  GETDATALINE;
  if(!NULLDATA(buff)){
    t->prompt = (char *)Malloc(200);
    StringCopy(t->prompt,buff);
  }
  else t->prompt = NULL;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d",&(t->hide));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  t->y2 = t->y2;
  t->df = (int *)Malloc(sizeof(int));
  *((int *)(t->df)) = 1;
  t->arg = NULL;
  t->Update = NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_horislide(FILE *fp,DIHB *f) {
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //min,max\n",f->min,f->max);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d //length \n",f->lngth);
  fprintf(fp,"NULL //prompt not used \n");
  fprintf(fp,"%d %d //x2,y2\n",f->x2,f->y2);
  fprintf(fp,"%d %d %d %d //border,hide,type,color\n",f->bordr,f->hide,f->type,f->sldclr);
  _uiPrintWid(fp,f->Wid);
}
DIHB * Read_data_horislide(FILE *fp) {
  DIHB *t;
  char buff[500];
  t = (DIHB *) Malloc(sizeof(DIHB));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->min),&(t->max));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  t->y1 = t->y1;
  GETDATALINE;
  sscanf(buff,"%d",&(t->lngth));
  GETDATALINE;
  if(!NULLDATA(buff)){
    t->prompt = (char *)Malloc(200);
    StringCopy(t->prompt,buff);
  }
  else t->prompt = NULL;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d%d%d%d",&(t->bordr),&(t->hide),&(t->type),&(t->sldclr));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  t->y2 = t->y2;
  t->df = (int *)Malloc(sizeof(int));
  *((int *)(t->df)) = 1;
  t->arg = NULL;
  t->Update = NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_browser(FILE *fp,DIW *f) {
  char **pt;
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d %d //x2,y2\n",f->x2,f->y2);
  fprintf(fp,"%d //Size \n",f->size);
  if(f->prompt != NULL) fprintf(fp,"\"%-s\" //Prompt\n",f->prompt);
  else fprintf(fp,"NULL //prompt \n");
  pt = f->menu;
  while( *pt != NULL) 
    fprintf(fp,"\"%-s\" //item\n",*(pt++));
  fprintf(fp,"NULL  //  \n");
  fprintf(fp,"%d //hide\n",f->hide);
  _uiPrintWid(fp,f->Wid);
}
DIW * Read_data_browser(FILE *fp) {
  int i, n,l,selmt;
  char **pt,*tmp;
  char buff[500];
  DIW *t;
  Dlink *L;
  t = (DIW *) Malloc(sizeof(DIW));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  t->y1 = t->y1;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  t->y2 = t->y2;
  GETDATALINE;
  sscanf(buff,"%d",&(t->size));
  GETDATALINE;
  if(!NULLDATA(buff)){
    t->prompt = (char *)Malloc(200);
    StringCopy(t->prompt,buff);
  }
  else t->prompt = NULL;
  L = Dopen();
  i =0;
  GETDATALINE;
  while(!NULLDATA(buff)) {
    tmp = (char *)Malloc(200);
    StringCopy(tmp,buff);
    Dadd(L,tmp);
    i++;
    GETDATALINE;
  }
  GETDATALINE;
  sscanf(buff,"%d",&(t->hide));
  pt = (char **) Malloc(sizeof(char *)*(i +1));
  t->menu=pt;
  i=0;
  Resetlink(L);
  while((tmp = (char *)Getrecord(L))!= NULL) {
    pt[i++] = tmp;
  }
  pt[i]= NULL;
  Dfree(L);
  t->df = (int *)Malloc(sizeof(int));
  *((int *)(t->df)) = 1;
  t->arg = NULL;
  t->Update = NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_scrollmenu(FILE *fp,DIE *f) {
  char **pt;
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d %d //x2,y2\n",f->x2,f->y2);
  fprintf(fp,"%d //Size \n",f->size);
  fprintf(fp,"NULL //prompt \n");
  pt = f->menu;
  while( *pt != NULL) 
    fprintf(fp,"\"%-s\" //item\n",*(pt++));
  fprintf(fp,"NULL  //  \n");
  fprintf(fp," %d %d %d %d %d %d %-d \n",f->width,f->offset,f->w,f->itemhi,
                  f->bordr,f->bkgr,f->hide);
  _uiPrintWid(fp,f->Wid);
}
DIE * Read_data_scrollmenu(FILE *fp) {
  int i, n,l,selmt;
  char **pt,*tmp;
  char buff[500];
  DIE *t;
  Dlink *L;
  t = (DIE *) Malloc(sizeof(DIE));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d",&(t->size));
  GETDATALINE;
  if(!NULLDATA(buff)){
    t->prompt = (char *)Malloc(200);
    StringCopy(t->prompt,buff);
  }
  else t->prompt = NULL;
  L = Dopen();
  i =0;
  GETDATALINE;
  while(!NULLDATA(buff)) {
    tmp = (char *)Malloc(200);
    StringCopy(tmp,buff);
    Dadd(L,tmp);
    i++;
    GETDATALINE;
  }
  pt = (char **) Malloc(sizeof(char *)*(i +1));
  t->menu=pt;
  i=0;
  Resetlink(L);
  while((tmp = (char *)Getrecord(L))!= NULL) {
    pt[i++] = tmp;
  }
  pt[i]= NULL;
  Dfree(L);
  t->df = (int *)Malloc(sizeof(int));
  *((int *)(t->df)) = 1;
  t->arg = NULL;
  t->Update = NULL;
  GETDATALINE;
  sscanf(buff,"%d%d%d%d%d%d%d",&(t->width),&(t->offset),&(t->w),&(t->itemhi),&(t->bordr),&(t->bkgr),&(t->hide));
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_selectmenu(FILE *fp,DIX *f) {
  char **pt;
  int n,i;
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d %d //x2,y2\n",f->x2,f->y2);
  fprintf(fp,"%d %d   //ygap\n",f->xgap,f->ygap);
  fprintf(fp,"%d %d //Width \n",f->lngth,f->width);
  fprintf(fp,"%d  //Ny,\n",f->ny);
  fprintf(fp,"%d %d  // offset, scroll width \n",f->offset,f->w);
  fprintf(fp,"%d %d %d %d %d // type,item hight,bordr,bkgr,hide \n",f->type,f->itemhi,f->bordr,f->bkgr,f->hide);
  _uiPrintWid(fp,f->Wid);

}
DIX *Read_data_selectmenu(FILE *fp) {
  int i, n,l,*tmpv;
  DIX *t;
  char buff[500];
  char *tmp,**pt,*xpm;
  Dlink *L;
  t = (DIX *) Malloc(sizeof(DIX));
  tmpv = (int *) Malloc(sizeof(int));
  *tmpv =1;
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->xgap),&(t->ygap));
  t->xgap=10;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->lngth),&(t->width));
  GETDATALINE;
  sscanf(buff,"%d",&(t->ny));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->offset),&(t->w));
  GETDATALINE;
  sscanf(buff,"%d%d%d%d%d",&(t->type),&(t->itemhi),&(t->bordr),&(t->bkgr),&(t->hide));
  t->nx =1;
  t->ny =1;
  t->list=NULL;
  t->df = (int *)tmpv;
  t->arg = NULL;
  t->Update = NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_thumbnailbrowser(FILE *fp,DIY *f) {
  char **pt;
  int n,i;
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d %d //x2,y2\n",f->x2,f->y2);
  fprintf(fp,"%d %d  //xgap, ygap\n",f->xgap,f->ygap);
  fprintf(fp,"%d //Width \n",f->width);
  fprintf(fp,"%d %d  // offset, scroll width \n",f->offset,f->w);
  fprintf(fp,"%d %d %d %d %d // type,item hight,bordr,bkgr,hide \n",f->type,f->itemhi,f->bordr,f->bkgr,f->hide);
  _uiPrintWid(fp,f->Wid);
}
DIY *Read_data_thumbnailbrowser(FILE *fp) {
  int i, n,l,*tmpv;
  DIY *t;
  char buff[500];
  char *tmp,**pt,*xpm;
  Dlink *L;
  t = (DIY *) Malloc(sizeof(DIY));
  tmpv = (int *) Malloc(sizeof(int));
  *tmpv =1;
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->xgap),&(t->ygap));
  GETDATALINE;
  sscanf(buff,"%d",&(t->width));
  t->lngth=t->width;
  t->nx=t->ny=t->size=t->nxsize=0;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->offset),&(t->w));
  GETDATALINE;
  sscanf(buff,"%d%d%d%d%d",&(t->type),&(t->itemhi),&(t->bordr),&(t->bkgr),&(t->hide));
  t->df = (int *)tmpv;
  t->prompt=NULL;
  t->arg = NULL;
  t->list=NULL;
  t->Update = NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_radiobutton(FILE *fp,DIRA *f) {
  ThumbNail **pt;
  int n,i;
  n=0;
  pt = (ThumbNail **)f->list;
  while( *pt != NULL) {n++;pt++;}
  f->ny=n;
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d %d  //x2,y2\n",f->x2,f->y2);
  fprintf(fp,"%d %d  //xgap, ygap\n",f->xgap,f->ygap);
  fprintf(fp,"%d %d  //item length,Width \n",f->lngth,f->width);
  fprintf(fp,"%d     //ny \n",f->ny);
  fprintf(fp,"%d %d  // offset, scroll width \n",f->offset,f->w);
  fprintf(fp,"%d %d %d %d %d // type,item hight,bordr,bkgr,hide \n",f->type,f->itemhi,f->bordr,f->bkgr,f->hide);
  n = f->ny;
  pt = (ThumbNail **)f->list;
  while( *pt != NULL) {
    fprintf(fp,"\"%-s\" //item\n",(*pt)->name);
    pt++;
  }
  _uiPrintWid(fp,f->Wid);
}
DIRA *Read_data_radiobutton(FILE *fp) {
  int i, n,l,*tmpv;
  char buff[500];
  DIRA *t;
  char *tmp,**pt,*xpm;
  Dlink *L;
  t = (DIRA *) Malloc(sizeof(DIRA));
  tmpv = (int *) Malloc(sizeof(int));
  *tmpv =1;
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->xgap),&(t->ygap));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->lngth),&(t->width));
  GETDATALINE;
  sscanf(buff,"%d",&(t->ny));
  t->nx=1;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->offset),&(t->w));
  GETDATALINE;
  sscanf(buff,"%d%d%d%d%d",&(t->type),&(t->itemhi),&(t->bordr),&(t->bkgr),&(t->hide));
  n = t->ny;
  L = Dopen();
  i =0;
  GETDATALINE;
  while(!NULLDATA(buff)) {
    tmp = (char *)Malloc(200);
    StringCopy(tmp,buff);
    Dadd(L,tmp);
    i++;
    if(i==t->ny) break;
    GETDATALINE;
  }
  t->ny=i;
  pt = (char **) Malloc(sizeof(char *)*(i +1));
  i=0;
  Resetlink(L);
  while((tmp = (char *)Getrecord(L))!= NULL) {
    pt[i++] = tmp;
  }
  pt[i]= NULL;
  t->list=(void **)kgStringToThumbNails(pt);
  free(pt);
  Dfree(L);
  i=0;
  t->df = (int *)tmpv;
  t->arg = NULL;
  t->Update = NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_checkbox(FILE *fp,DICH *f) {
  ThumbNail **pt;
  int n,i;
  n=0;
  pt = (ThumbNail **)f->list;
  while( *pt != NULL) {n++;pt++;}
  f->ny=n;
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d %d  //x2,y2\n",f->x2,f->y2);
  fprintf(fp,"%d %d  //xgap, ygap\n",f->xgap,f->ygap);
  fprintf(fp,"%d %d  //item length,Width \n",f->lngth,f->width);
  fprintf(fp,"%d     //ny \n",f->ny);
  fprintf(fp,"%d %d  // offset, scroll width \n",f->offset,f->w);
  fprintf(fp,"%d %d %d %d %d // type,item hight,bordr,bkgr,hide \n",f->type,f->itemhi,f->bordr,f->bkgr,f->hide);
  n = f->ny;
  pt = (ThumbNail **)f->list;
  while( *pt != NULL) {
    fprintf(fp,"\"%-s\" //item\n",(*pt)->name);
    pt++;
  }
  _uiPrintWid(fp,f->Wid);
}
DICH *Read_data_checkbox(FILE *fp) {
  int i, n,l,*tmpv;
  char buff[500];
  DICH *t;
  char *tmp,**pt,*xpm;
  Dlink *L;
  t = (DICH *) Malloc(sizeof(DICH));
  tmpv = (int *) Malloc(sizeof(int));
  *tmpv =1;
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->xgap),&(t->ygap));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->lngth),&(t->width));
  GETDATALINE;
  sscanf(buff,"%d",&(t->ny));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->offset),&(t->w));
  GETDATALINE;
  sscanf(buff,"%d%d%d%d%d",&(t->type),&(t->itemhi),&(t->bordr),&(t->bkgr),&(t->hide));
  n = t->ny;
  t->nx=1;
  L = Dopen();
  i =0;
  GETDATALINE;
  while(!NULLDATA(buff)) {
    tmp = (char *)Malloc(200);
    StringCopy(tmp,buff);
    Dadd(L,tmp);
    i++;
    if(i==t->ny) break;
    GETDATALINE;
  }
  t->ny=i;
  pt = (char **) Malloc(sizeof(char *)*(i +1));
  i=0;
  Resetlink(L);
  while((tmp = (char *)Getrecord(L))!= NULL) {
    pt[i++] = tmp;
  }
  pt[i]= NULL;
  t->list=(void **)kgStringToThumbNails(pt);
  free(pt);
  Dfree(L);
  t->df = (int *)tmpv;
  t->arg = NULL;
  t->Update = NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_msgscroll(FILE *fp,DIS *f) {
  char **pt;
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d //Size \n",f->size);
  fprintf(fp,"NULL //prompt \n");
  pt = f->menu;
  while( *pt != NULL) 
    fprintf(fp,"\"%-s\" //item\n",*(pt++));
  fprintf(fp,"NULL  //  \n");
  fprintf(fp,"%d %d //x2,y2\n",f->x2,f->y2);
  fprintf(fp,"%d //hide\n",f->hide);
  _uiPrintWid(fp,f->Wid);
}
DIS * Read_data_msgscroll(FILE *fp) {
  int i, n,l,selmt;
  char **pt,*tmp;
  char buff[500];
  DIS *t;
  Dlink *L;
  t = (DIS *) Malloc(sizeof(DIS));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  t->y1 = t->y1;
  GETDATALINE;
  sscanf(buff,"%d",&(t->size));
  GETDATALINE;
  if(!NULLDATA(buff)){
    t->prompt = (char *)Malloc(200);
    StringCopy(t->prompt,buff);
  }
  else t->prompt = NULL;
  L = Dopen();
  i =0;
  GETDATALINE;
  while(!NULLDATA(buff)) {
    tmp = (char *)Malloc(200);
    StringCopy(tmp,buff);
    Dadd(L,tmp);
    i++;
    GETDATALINE;
  }
  pt = (char **) Malloc(sizeof(char *)*(i +1));
  t->menu=pt;
  i=0;
  Resetlink(L);
  while((tmp = (char *)Getrecord(L))!= NULL) {
    pt[i++] = tmp;
  }
  pt[i]= NULL;
  Dfree(L);
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d",&(t->hide));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  t->arg = NULL;
  t->Update = NULL;
  t->width=20,t->offset=6,t->w=22,t->itemhi=1,t->bordr=1,t->bkgr=1;
  _uiReadWid(fp,t->Wid);
  return t;
}

int PrintXpm(FILE *fp,void *xpm){
  char *pt,*tmp;
  JPGIMG *jpg;
  if(xpm == NULL) fprintf(fp,"NULL // XPM \n");
  else {
    tmp = (char *)(xpm);
    if( (tmp[0]=='#')&&(tmp[1]=='#') )
        fprintf(fp,"%-s // XPM \n",(char *)(xpm+2));
    else fprintf(fp,"NULL // XPM \n");
  }
  return 1;
}
void Print_data_pixmapbox(FILE *fp,DIP *f) {
  char **pt,*tmp;
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d %d //x2,y2\n",f->x2,f->y2);
  PrintXpm(fp,f->xpm);
  fprintf(fp,"%d   // background color \n",f->bkgr_clr);
  fprintf(fp,"%d %d // border hide\n",f->bordr,f->hide);
  _uiPrintWid(fp,f->Wid);
}
DIP *Read_data_pixmapbox(FILE *fp) {
  int i, n,l;
  DIP *t;
  char buff[500];
  t = (DIP *) Malloc(sizeof(DIP));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  t->y1 = t->y1;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  t->y2 = t->y2;
  GETDATALINE;
  if(!NULLDATA(buff)) {
    t->xpm = (void *)Malloc(100);
    strcpy((char *)(t->xpm),"##");
    sscanf(buff,"%s",(char *)(t->xpm)+2);
  }
  else t->xpm = NULL;
  GETDATALINE;
  sscanf(buff,"%d",&(t->bkgr_clr));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->bordr),&(t->hide));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_infobox(FILE *fp,DII *f) {
  char **pt;
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d %d //x2,y2\n",f->x2,f->y2);
  fprintf(fp,"%d %d //nchrs,nlines\n",f->nchrs,f->nlines);
  fprintf(fp,"%d //hide\n",f->hide);
  _uiPrintWid(fp,f->Wid);
}
DII *Read_data_infobox(FILE *fp) {
  int i, n,l;
  DII *t;
  char buff[500];
  t = (DII *) Malloc(sizeof(DII));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  t->y1 = t->y1;
  t->y2 = t->y2;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->nchrs),&(t->nlines));
  GETDATALINE;
  sscanf(buff,"%d",&(t->hide));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  _uiReadWid(fp,t->Wid);
//  printf("%s\n",t->Wid);
  t->linewidth=22;
  return t;
}

void Print_data_graphbox(FILE *fp,DIG *f) {
  char **pt;
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d %d //x2,y2\n",f->x2,f->y2);
  PrintXpm(fp,f->xpm);
  fprintf(fp,"0   // background color \n");
  fprintf(fp,"%d %d //border hide\n",f->bordr,f->hide);
  _uiPrintWid(fp,f->Wid);
}
DIG *Read_data_graphbox(FILE *fp) {
  int i, n,l;
  DIG *t;
  char buff[500];
  t = (DIG *) Malloc(sizeof(DIG));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  t->y1 = t->y1;
  t->y2 = t->y2;
  GETDATALINE;
  if(!NULLDATA(buff)) {
    t->xpm = (void *)Malloc(100);
    StringCopy((char *)t->xpm,buff);
  }
  else t->xpm = NULL;
  GETDATALINE;
  sscanf(buff,"%d",&(t->bkgr_clr));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->bordr),&(t->hide));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  t->initgraph = NULL;
  t->data = NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_textbox(FILE *fp,DIT *t) {
  int i, n;
  T_ELMT *e;
  fprintf(fp,"%c     //code\n",t->code);
  fprintf(fp,"%d %d  //x1,y1\n",t->x1,t->y1);
  fprintf(fp,"%d //Width \n",t->width);
  fprintf(fp,"%d %d  //Nx,Ny,\n",t->nx,t->ny);
  n = t->nx*t->ny;
  e = t->elmt;
  fprintf(fp,"//element formats\n");
  for(i=0;i<n;i++) {
    fprintf(fp,"\"%-s\"\n",e[i].fmt);
  }
  fprintf(fp,"%d %d  //x2,y2\n",t->x2,t->y2);
  fprintf(fp,"%d %d %d //Cursor Position\n",t->row,t->col,t->bordr);
  fprintf(fp,"%d %d %d //hide\n",t->Font,t->FontSize,t->hide);
  _uiPrintWid(fp,t->Wid);
}
DIT * Read_data_textbox(FILE *fp) {
  int i, n,l,selmt;
  DIT *t;
  T_ELMT *e;
  char buff[500];
  char *ctmp;
  double *ftmp;
  int *itmp;
  char ch;
  selmt = sizeof(T_ELMT);
  t = (DIT *) Malloc(sizeof(DIT));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d",&(t->width));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->nx),&(t->ny));
  n = (t->nx)*(t->ny);
  t->elmt = (T_ELMT *) Malloc(selmt*n);
  e = t->elmt;
  for(i=0;i<n;i++) {
    GETDATALINE;
    e[i].fmt = (char *)Malloc(100);
    StringCopy(e[i].fmt,buff);
    e[i].sw =1;
    l = strlen(e[i].fmt) - 1;
    while( e[i].fmt[l] <= ' ') l--;
    ch = e[i].fmt[l];
    switch(ch) {
      case 'f':
        e[i].fmt[l]='F';
      case 'F':
        ftmp = (double *)Malloc(sizeof(double));
        *ftmp =0.;
        e[i].v = (void *)ftmp;
        break;
      case 'd':
        itmp = (int *)Malloc(sizeof(int));
        *itmp = 1;
        e[i].v = (void *)itmp;
        break;
      case 's':
        ctmp = (char *)Malloc(100);
        ctmp[0]='\0';
        e[i].v = (void *) ctmp;
        break;
    }
  }
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d%d%d",&(t->row),&(t->col),&(t->bordr));
  GETDATALINE;
  sscanf(buff,"%d%d%d",&(t->Font),&(t->FontSize),&(t->hide));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  t->arg = NULL;
  t->Update = NULL;
  t->y1 = t->y1;
  t->y2 = t->y2;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_tablebox(FILE *fp,DIT *t) {
  int i, n;
  T_ELMT *e;
  fprintf(fp,"%c     //code\n",t->code);
  fprintf(fp,"%d %d  //x1,y1\n",t->x1,t->y1);
  fprintf(fp,"%d //Width \n",t->width);
  fprintf(fp,"%d %d  //Nx,Ny,\n",t->nx,t->ny);
  n = t->nx*t->ny;
  e = t->elmt;
  fprintf(fp,"//element switch and formats\n");
  for(i=0;i<t->nx;i++) {
    fprintf(fp,"%-d\n",e[i].sw);
    fprintf(fp,"\"%-s\"\n",e[i].fmt);
  }
  fprintf(fp,"%d %d  //x2,y2\n",t->x2,t->y2);
  fprintf(fp,"%d %d %d //Cursor Position\n",t->row,t->col,t->bordr);
  fprintf(fp,"%d %d %d //hide\n",t->Font,t->FontSize,t->hide);
  _uiPrintWid(fp,t->Wid);
}
#if 0
void Print_data_filebox(FILE *fp,DIX *t) {
  int i, n;
  T_ELMT *e;
  fprintf(fp,"%c     //code\n",t->code);
  fprintf(fp,"%d %d  //x1,y1\n",t->x1,t->y1);
  fprintf(fp,"%d //Width \n",t->width);
  e = t->elmt;
  fprintf(fp,"//element formats\n");
  fprintf(fp,"\"%-s\"\n",e->fmt);
  fprintf(fp,"%d %d  //x2,y2\n",t->x2,t->y2);
  fprintf(fp,"\"%-s\"   //File Filter\n",t->fltr);
}
#endif 
DIT * Read_data_tablebox(FILE *fp) {
  char temp[200];
  char buff[500];
  int i, n,l,selmt,nx,ny,j,k,sw;
  DIT *t;
  T_ELMT *e;
  char *ctmp;
  double *ftmp;
  int *itmp;
  char ch;
  selmt = sizeof(T_ELMT);
  t = (DIT *) Malloc(sizeof(DIT));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d",&(t->width));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->nx),&(t->ny));
  n = (t->nx)*(t->ny);
  nx = t->nx;
  ny = t->ny;
  t->elmt = (T_ELMT *) Malloc(selmt*n);
  e = t->elmt;
  for(j=0;j<nx;j++) {
    GETDATALINE;
    sscanf(buff,"%d",&sw);
    GETDATALINE;
    strcpy(temp,buff);
    for(k=0;k<ny;k++) {
      strcpy(buff,temp);
      i = k*nx+j;
      e[i].fmt = (char *)Malloc(100);
      StringCopy(e[i].fmt,buff);
      e[i].sw =sw;
      l = strlen(e[i].fmt) - 1;
      while( e[i].fmt[l] <= ' ') l--;
      ch = e[i].fmt[l];
      switch(ch) {
        case 'F':
          ftmp = (double *)Malloc(sizeof(double));
          *ftmp =0.;
          e[i].v = (void *)ftmp;
          break;
        case 'd':
          itmp = (int *)Malloc(sizeof(int));
          *itmp = 1;
          e[i].v = (void *)itmp;
          break;
        case 's':
          ctmp = (char *)Malloc(100);
          ctmp[0]='\0';
          e[i].v = (void *) ctmp;
          break;
      }
    }
  }
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d%d%d",&(t->row),&(t->col),&(t->bordr));
  GETDATALINE;
  sscanf(buff,"%d%d%d",&(t->Font),&(t->FontSize),&(t->hide));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  t->arg = NULL;
  t->Update = NULL;
  t->y1 = t->y1;
  t->y2 = t->y2;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_Y(FILE *fp,DIY *f) {
  char **pt;
  int n,i;
  fprintf(fp,"%c     //code\n",f->code);
  fprintf(fp,"%d %d  //x1,y1\n",f->x1,f->y1);
  fprintf(fp,"%d %d //x2,y2\n",f->x2,f->y2);
  fprintf(fp,"%d %d  //xgap,ygap\n",f->xgap,f->ygap);
  fprintf(fp,"%d //Image Width \n",f->width);
  fprintf(fp,"%d %d  // offset, scroll width \n",f->offset,f->w);
  fprintf(fp,"%d %d %d %d %d //type,itemhi(0/1),bordr(0/1),back_grond(0/1),hide(0/1)\n",
           f->type,f->itemhi,f->bordr,f->bkgr,f->hide);
  _uiPrintWid(fp,f->Wid);
}
void Print_data_buttonbox_o(FILE *fp,DIB *b) {
  int i, n;
  fprintf(fp,"%c     //code\n",b->code);
  fprintf(fp,"%d %d  //x1,y1\n",b->x1,b->y1);
  fprintf(fp,"%d %d  //xgap,ygap\n",b->xgap,b->ygap);
  fprintf(fp,"%d //length \n",b->lngth);
  fprintf(fp,"%d //Width \n",b->width);
  fprintf(fp,"%d %d  //Nx,Ny,\n",b->nx,b->ny);
  n = b->nx*b->ny;
  if(b->sw != NULL) {
   for(i=0;i<n;i++) {
     fprintf(fp,"%d //status\n",b->sw[i]);
   }
  }
  else  {
   for(i=0;i<n;i++) {
     fprintf(fp,"1 //status\n");
   }
  }
  for(i=0;i<n;i++) {
    fprintf(fp,"\"%-s\" //title\n",b->titles[i]);
  }
  fprintf(fp,"%d %d  //x2,y2\n",b->x2,b->y2);
  if( b->butncode != NULL )
    fprintf(fp,"\"%-s\"/Button codes\n",b->butncode);
  else  fprintf(fp,"NULL  //Button codes\n");
  _uiPrintWid(fp,b->Wid);
}
int PrintTitle(FILE *fp,char *Title){
  char buff[1000];
  int i=0;
  if(Title == NULL) fprintf(fp,"NULL  //title\n");
  else {
    while(Title[i]>=' ') {
       buff[i]=Title[i];i++;
    }
    buff[i]='\0';
    fprintf(fp,"\"%-s\" //title \n",buff);
  }

}
void Print_data_buttonbox(FILE *fp,DIB *b) {

  int i, n;
  fprintf(fp,"%c     //code\n",b->code);
  fprintf(fp,"%d %d  //x1,y1\n",b->x1,b->y1);
  fprintf(fp,"%d %d  //xgap,ygap\n",b->xgap,b->ygap);
  fprintf(fp,"%d //length \n",b->lngth);
  fprintf(fp,"%d //Width \n",b->width);
  fprintf(fp,"%d %d  //Nx,Ny,\n",b->nx,b->ny);
  n = b->nx*b->ny;
  if(b->sw != NULL) {
   for(i=0;i<n;i++) {
     fprintf(fp,"%d //status\n",b->sw[i]);
   }
  }
  else  {
   for(i=0;i<n;i++) {
     fprintf(fp,"1 //status\n");
   }
  }
  if(b->titles!= NULL) {
   for(i=0;i<n;i++) {
#if 0
    if(b->titles[i]==NULL) fprintf(fp,"\" \" //title\n");
    else  fprintf(fp,"\"%-s\" //title\n",b->titles[i]);
#else
    PrintTitle(fp,b->titles[i]);
#endif
   }
  }
  else {
    for(i=0;i<n;i++) {
      fprintf(fp,"\"%-s\" //title\n",b->titles[i]);
    }
  }
  fprintf(fp,"%d %d  //x2,y2\n",b->x2,b->y2);
  if( b->butncode != NULL )
    fprintf(fp,"\"%-s\"/Button codes\n",b->butncode);
  else  fprintf(fp,"NULL  //Button codes\n");
  if(b->xpm == NULL) {
   for(i=0;i<3*n;i++) {
     fprintf(fp,"NULL //xpm picture\n");
   }
  }
  else {
   for(i=0;i<3*n;i++) {
     PrintXpm(fp,b->xpm[i]);
   }
  }
  if(b->bkgr==NULL) {
   for(i=0;i<n;i++) {
     fprintf(fp,"-1 //back ground color\n");
   }
  }
  else {
   for(i=0;i<n;i++) {
     fprintf(fp,"%d //back ground color\n",b->bkgr[i]);
   }
  }
  fprintf(fp,"%d %f %d  //type,fac\n",b->type,b->fac,b->bordr);
  fprintf(fp,"%d  //hide\n",b->hide);
  _uiPrintWid(fp,b->Wid);
}
DIB * Read_data_buttonbox(FILE *fp){
  DIB *t;
  char temp;
  int i, n,l;
  int *tmp;
  char *pt;
  char buff[500];
  t = (DIB *) Malloc(sizeof(DIB));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->xgap),&(t->ygap));
  GETDATALINE;
  sscanf(buff,"%d",&(t->lngth));
  GETDATALINE;
  sscanf(buff,"%d",&(t->width));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->nx),&(t->ny));
  n = t->nx*t->ny;
  t->xpm = (void **)Malloc(sizeof(char *)*3*n);
  t->bkgr = (int *)Malloc(sizeof(int)*n);
  for(i=0;i<n;i++) { t->bkgr[i]=-1;}
  GETDATALINE;
  t->sw = (int *) Malloc(sizeof(int)*n);
  if(NULLDATA(buff)) {
    for(i=0;i<n;i++) {
      t->sw[i]=1;
    }
  }
  else {
    sscanf(buff,"%d",&(t->sw[0]));
    for(i=1;i<n;i++) {
      GETDATALINE;
      sscanf(buff,"%d",&(t->sw[i]));
    }
  }
  t->titles = (char **) Malloc(sizeof(char *)*(n+1));
  for(i=0;i<n;i++) {
    GETDATALINE;
    t->titles[i] = (char *)Malloc(100);
    StringCopy(t->titles[i],buff);
  }
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  if(NULLDATA(buff)) {
    t->butncode = NULL;
  }
  else {
   t->butncode = (char *)Malloc(100);
   StringCopy(t->butncode,buff);
  }
  for(i=0;i<3*n;i++) {
    GETDATALINE;
    if(!NULLDATA(buff)) {
      t->xpm[i]= (void *)Malloc(100);
      pt =(char *) t->xpm[i];
      strcpy(pt,"##");
      sscanf(buff,"%s",(char *)(pt+2));
    }
    else t->xpm[i]=NULL;
  }
  for(i=0;i<n;i++) {
    GETDATALINE;
    if(!NULLDATA(buff)) {
      sscanf(buff,"%d",(t->bkgr)+i);
    }
    else break;
  }
  GETDATALINE;
  sscanf(buff,"%d%f%d",&(t->type),&(t->fac),&(t->bordr));
  GETDATALINE;
  sscanf(buff,"%d",&(t->hide));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  t->df = (int *)&(t->bval);
  t->bval=1;
  t->arg = NULL;
  t->Update = NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}
void Print_data_buttonboxn(FILE *fp,DIN *b) {

  BUT_STR *butn;
  int i, n;
  fprintf(fp,"%c     //code\n",b->code);
  fprintf(fp,"%d %d  //x1,y1\n",b->x1,b->y1);
  fprintf(fp,"%d %d  //x2,y2\n",b->x2,b->y2);
  fprintf(fp,"%d %d  //xgap,ygap\n",b->xgap,b->ygap);
  fprintf(fp,"%d //length \n",b->lngth);
  fprintf(fp,"%d //Width \n",b->width);
  fprintf(fp,"%d %d  //Nx,Ny,\n",b->nx,b->ny);
  n = b->nx*b->ny;
  butn = (BUT_STR *)b->buts;
  for(i=0;i<n;i++) {
    fprintf(fp,"%d //status\n",butn[i].sw);
    PrintTitle(fp,butn[i].title);
    PrintXpm(fp,butn[i].xpmn);
    PrintXpm(fp,butn[i].xpmp);
    PrintXpm(fp,butn[i].xpmh);
    fprintf(fp,"%d //back ground color\n",butn[i].bkgr);
    fprintf(fp,"%c //keyboard code\n",butn[i].butncode);
  }
  fprintf(fp,"%d %f %d  //type,fac\n",b->type,b->fac,b->bordr);
  fprintf(fp,"%d %d //hide\n",b->hide,b->nodrawbkgr);
  _uiPrintWid(fp,b->Wid);
}
DIN * Read_data_buttonboxn(FILE *fp){
  BUT_STR *butn;
  DIN *t;
  char temp;
  int i, n,l;
  int *tmp;
  char *pt;
  char buff[500];
  t = (DIN *) Malloc(sizeof(DIN));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->xgap),&(t->ygap));
  GETDATALINE;
  sscanf(buff,"%d",&(t->lngth));
  GETDATALINE;
  sscanf(buff,"%d",&(t->width));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->nx),&(t->ny));
  n = t->nx*t->ny;
  t->buts = (void *)Malloc(sizeof(BUT_STR)*n);
  butn = (BUT_STR *)t->buts;
  for(i=0;i<n;i++) {
    GETDATALINE;
    sscanf(buff,"%d",&(butn[i].sw));
    GETDATALINE;
    StringCopy(butn[i].title,buff);
    GETDATALINE;
    if(!NULLDATA(buff)) {
      butn[i].xpmn= (void *)Malloc(100);
      pt =(char *) butn[i].xpmn;
      strcpy(pt,"##");
      sscanf(buff,"%s",(char *)(pt+2));
    }
    else butn[i].xpmn=NULL;
    GETDATALINE;
    if(!NULLDATA(buff)) {
      butn[i].xpmp= (void *)Malloc(100);
      pt =(char *) butn[i].xpmp;
      strcpy(pt,"##");
      sscanf(buff,"%s",(char *)(pt+2));
    }
    else butn[i].xpmp=NULL;
    GETDATALINE;
    if(!NULLDATA(buff)) {
      butn[i].xpmh= (void *)Malloc(100);
      pt =(char *) butn[i].xpmh;
      strcpy(pt,"##");
      sscanf(buff,"%s",(char *)(pt+2));
    }
    else butn[i].xpmh=NULL;
    butn[i].imgn=NULL;
    butn[i].imgp=NULL;
    butn[i].imgh=NULL;
    butn[i].imgh=NULL;
    GETDATALINE;
    sscanf(buff,"%d",&(butn[i].bkgr));
    GETDATALINE;
    sscanf(buff,"%c",(char *)&(butn[i].butncode));
  }
  GETDATALINE;
  sscanf(buff,"%d%f%d",&(t->type),&(t->fac),&(t->bordr));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->hide),&(t->nodrawbkgr));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  t->df = (int *)&(t->bval);
  t->bval=1;
  t->arg = NULL;
  t->Update = NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}
DIL * Read_data_splbuttonbox(FILE *fp){
  BUT_STR *butn;
  DIL *t;
  char temp;
  int i, n,l;
  int *tmp;
  char *pt;
  char buff[500];
  t = (DIL *) Malloc(sizeof(DIL));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->xgap),&(t->ygap));
  GETDATALINE;
  sscanf(buff,"%d",&(t->lngth));
  GETDATALINE;
  sscanf(buff,"%d",&(t->width));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->nx),&(t->ny));
  n = t->nx*t->ny;
  t->buts = (void *)Malloc(sizeof(BUT_STR)*n);
  butn = (BUT_STR *)t->buts;
  for(i=0;i<n;i++) {
    GETDATALINE;
    sscanf(buff,"%d",&(butn[i].sw));
    GETDATALINE;
    StringCopy(butn[i].title,buff);
    GETDATALINE;
    if(!NULLDATA(buff)) {
      butn[i].xpmn= (void *)Malloc(100);
      pt =(char *) butn[i].xpmn;
      strcpy(pt,"##");
      sscanf(buff,"%s",(char *)(pt+2));
    }
    else butn[i].xpmn=NULL;
    GETDATALINE;
    if(!NULLDATA(buff)) {
      butn[i].xpmp= (void *)Malloc(100);
      pt =(char *) butn[i].xpmp;
      strcpy(pt,"##");
      sscanf(buff,"%s",(char *)(pt+2));
    }
    else butn[i].xpmp=NULL;
    GETDATALINE;
    if(!NULLDATA(buff)) {
      butn[i].xpmh= (void *)Malloc(100);
      pt =(char *) butn[i].xpmh;
      strcpy(pt,"##");
      sscanf(buff,"%s",(char *)(pt+2));
    }
    else butn[i].xpmh=NULL;
    butn[i].imgn=NULL;
    butn[i].imgp=NULL;
    butn[i].imgh=NULL;
    butn[i].imgh=NULL;
    GETDATALINE;
    sscanf(buff,"%d",&(butn[i].bkgr));
    GETDATALINE;
    sscanf(buff,"%c",(char *)&(butn[i].butncode));
  }
  GETDATALINE;
  sscanf(buff,"%d%f%d",&(t->type),&(t->fac),&(t->bordr));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->hide),&(t->nodrawbkgr));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  t->df = (int *)&(t->bval);
  t->bval=1;
  t->arg = NULL;
  t->Update = NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}

void Print_data_buttonboxnew(FILE *fp,DIBN *b) {
  int i, n;
  fprintf(fp,"%c     //code\n",b->code);
  fprintf(fp,"%d %d  //x1,y1\n",b->x1,b->y1);
  fprintf(fp,"%d %d  //xgap,ygap\n",b->xgap,b->ygap);
  fprintf(fp,"%d //length \n",b->lngth);
  fprintf(fp,"%d //Width \n",b->width);
  fprintf(fp,"%d %d  //Nx,Ny,\n",b->nx,b->ny);
  if( b->prompt != NULL )
    fprintf(fp,"\"%-s\"/Prompt\n",b->prompt);
  else  fprintf(fp,"NULL  //Prompt\n");
  n = b->nx*b->ny;
  for(i=0;i<n;i++) {
    fprintf(fp,"\"%-s\" //title\n",b->titles[i]);
  }
  fprintf(fp,"%d %d  //x2,y2\n",b->x2,b->y2);
  if( b->butncode != NULL )
    fprintf(fp,"\"%-s\"/Button codes\n",b->butncode);
  else  fprintf(fp,"NULL  //Button codes\n");
  if(b->xpm == NULL) {
   for(i=0;i<n;i++) {
     fprintf(fp,"NULL //xpm picture\n");
   }
  }
  else {
   for(i=0;i<n;i++) {
     PrintXpm(fp,b->xpm[i]);
   }
  }
  if(b->bkgr==NULL) {
   for(i=0;i<n;i++) {
     fprintf(fp,"-1 //back ground color\n");
   }
  }
  else {
   for(i=0;i<n;i++) {
     fprintf(fp,"%d //back ground color\n",b->bkgr[i]);
   }
  }
  fprintf(fp,"%d %f %d  //type,fac\n",b->type,b->fac,b->bordr);
  fprintf(fp,"%d  //hide\n",b->hide);
  _uiPrintWid(fp,b->Wid);
}
DIBN * Read_data_buttonboxnew(FILE *fp){
  DIBN *t;
  char extra[20]=" 2 0.0";
  char temp,*pt;
  int i, n,l;
  int *tmp;
  char buff[500];
  tmp = (int *) Malloc(sizeof(int));
  *tmp =1;
  t = (DIBN *) Malloc(sizeof(DIBN));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->xgap),&(t->ygap));
  GETDATALINE;
  sscanf(buff,"%d",&(t->lngth));
  GETDATALINE;
  sscanf(buff,"%d",&(t->width));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->nx),&(t->ny));
  n = t->nx*t->ny;
  t->xpm = (void **)Malloc(sizeof(char *)*3*n);
  t->bkgr = (int *)Malloc(sizeof(int)*n);
  for(i=0;i<n;i++) { t->bkgr[i]=-1;}
  GETDATALINE;
  if(NULLDATA(buff)) t->prompt =NULL;
  else {
   t->prompt = (char *)Malloc(100);
   StringCopy(t->prompt,buff);
  }
  n = t->nx*t->ny;
  t->titles = (char **) Malloc(sizeof(char *)*(n+1));
  for(i=0;i<n;i++) {
    GETDATALINE;
    t->titles[i] = (char *)Malloc(100);
    StringCopy(t->titles[i],buff);
  }
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  if(NULLDATA(buff)) {
    t->butncode = NULL;
  }
  else {
   t->butncode = (char *)Malloc(100);
   StringCopy(t->butncode,buff);
  }
  for(i=0;i<n;i++) {
    GETDATALINE;
    if(!NULLDATA(buff)) {
      t->xpm[i]= (void *)Malloc(100);
      pt =(char *) t->xpm[i];
      strcpy(pt,"##");
      sscanf(buff,"%s",(char *)(pt+2));
    }
    else t->xpm[i]=NULL;
  }
  for(i=n;i<3*n;i++) {
     t->xpm[i]=NULL;
  }
  for(i=0;i<n;i++) {
    GETDATALINE;
    if(!NULLDATA(buff)) {
      sscanf(buff,"%d",(t->bkgr)+i);
    }
    else break;
  }
  GETDATALINE;
  sscanf(buff,"%d%f%d",&(t->type),&(t->fac),&(t->bordr));
  GETDATALINE;
  sscanf(buff,"%d",&(t->hide));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
  t->df = (int *)tmp;
  t->arg = NULL;
  t->Update = NULL;
  t->bordr =0;
  _uiReadWid(fp,t->Wid);
  return t;
}

#if 0
void Print_data_horibar(FILE *fp,DIL *b) {

  int i, n;
  fprintf(fp,"%c     //code\n",b->code);
  fprintf(fp,"%d %d  //x1,y1\n",b->x1,b->y1);
  fprintf(fp,"%d %d  //xgap,ygap\n",b->xgap,b->ygap);
  fprintf(fp,"%d //length \n",b->lngth);
  fprintf(fp,"%d //Width \n",b->width);
  fprintf(fp,"%d %d  //Nx,Ny,\n",b->nx,b->ny);
  n = b->nx*b->ny;
  if(b->sw != NULL) {
   for(i=0;i<n;i++) {
     fprintf(fp,"%d //status\n",b->sw[i]);
   }
  }
  else  fprintf(fp,"NULL  //button switch\n");
  for(i=0;i<n;i++) {
    fprintf(fp,"\"%-s\" //title\n",b->titles[i]);
  }
  fprintf(fp,"%d %d  //x2,y2\n",b->x2,b->y2);
  if( b->butncode != NULL )
    fprintf(fp,"\"%-s\"/Button codes\n",b->butncode);
  else  fprintf(fp,"NULL  //Button codes\n");
  _uiPrintWid(fp,b->Wid);
}
#endif
#if 0
DIL * Read_data_horibar(FILE *fp){
  DIL *t;
  char temp;
  int i, n,l;
  int *tmp;
  char buff[500];
  tmp = (int *) malloc(sizeof(int));
  *tmp =1;
  t = (DIL *) malloc(sizeof(DIL));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->xgap),&(t->ygap));
  GETDATALINE;
  sscanf(buff,"%d",&(t->lngth));
  GETDATALINE;
  sscanf(buff,"%d",&(t->width));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->nx),&(t->ny));
  n = t->nx*t->ny;
  GETDATALINE;
  if(NULLDATA(buff)) {
    t->sw = NULL;
  }
  else {
    t->sw = (int *) malloc(sizeof(int)*n);
    sscanf(buff,"%d",&(t->sw[0]));
    for(i=1;i<n;i++) {
      GETDATALINE;
      sscanf(buff,"%d",&(t->sw[i]));
    }
  }
  t->titles = (char **) malloc(sizeof(char *)*(n+1));
  for(i=0;i<n;i++) {
    GETDATALINE;
    t->titles[i] = (char *)malloc(100);
    StringCopy(t->titles[i],buff);
  }
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  if(NULLDATA(buff)) {
    t->butncode = NULL;
  }
  else {
   t->butncode = (char *)malloc(100);
   StringCopy(t->butncode,buff);
  }
  t->df = (int *)tmp;
  t->arg = NULL;
  t->Update = NULL;
  t->y1 = t->y1;
  t->y2 = t->y2;
  _uiReadWid(fp,t->Wid);
  return t;
}
#endif
void Print_data_horibarnew(FILE *fp,DILN *b) {

  int i, n;
  fprintf(fp,"%c     //code\n",b->code);
  fprintf(fp,"%d %d  //x1,y1\n",b->x1,b->y1);
  fprintf(fp,"%d %d  //xgap,ygap\n",b->xgap,b->ygap);
  fprintf(fp,"%d //length \n",b->lngth);
  fprintf(fp,"%d //Width \n",b->width);
  fprintf(fp,"%d %d  //Nx,Ny,\n",b->nx,b->ny);
  n = b->nx*b->ny;
  if(b->sw != NULL) {
   for(i=0;i<n;i++) {
     fprintf(fp,"%d //status\n",b->sw[i]);
   }
  }
  else  {
   for(i=0;i<n;i++) {
     fprintf(fp,"1 //status\n");
   }
  }
  for(i=0;i<n;i++) {
#if 0
    if(b->titles[i]== NULL) fprintf(fp,"\" \" //title\n");
    else fprintf(fp,"\"%-s\" //title\n",b->titles[i]);
#else
    PrintTitle(fp,b->titles[i]);
#endif
  }
  fprintf(fp,"%d %d  //x2,y2\n",b->x2,b->y2);
  if( b->butncode != NULL )
    fprintf(fp,"\"%-s\"/Button codes\n",b->butncode);
  else  fprintf(fp,"NULL  //Button codes\n");
  if(b->xpm == NULL) {
   for(i=0;i<3*n;i++) {
     fprintf(fp,"NULL //xpm picture\n");
   }
  }
  else {
   for(i=0;i<3*n;i++) {
     PrintXpm(fp,b->xpm[i]);
   }
  }
  if(b->bkgr==NULL) {
   for(i=0;i<n;i++) {
     fprintf(fp,"-1 //back ground color\n");
   }
  }
  else {
   for(i=0;i<n;i++) {
     fprintf(fp,"%d //back ground color\n",b->bkgr[i]);
   }
  }
  fprintf(fp,"%d %f %d  //type,fac\n",b->type,b->fac,b->bordr);
  fprintf(fp,"%d  //hide\n",b->hide);
  _uiPrintWid(fp,b->Wid);
}

DILN * Read_data_horibarnew(FILE *fp){
  DILN *t;
  char temp,*pt;
  int i, n,l;
  int *tmp;
  char buff[500];
  t = (DILN *) Malloc(sizeof(DILN));
  GETDATALINE;
  sscanf(buff,"%c",&(t->code));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x1),&(t->y1));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->xgap),&(t->ygap));
  GETDATALINE;
  sscanf(buff,"%d",&(t->lngth));
  GETDATALINE;
  sscanf(buff,"%d",&(t->width));
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->nx),&(t->ny));
  n = t->nx*t->ny;
  t->xpm = (void **)Malloc(sizeof(char *)*3*n);
  t->bkgr = (int *)Malloc(sizeof(int)*n);
  for(i=0;i<n;i++) { t->bkgr[i]=-1;}
  GETDATALINE;
  if(NULLDATA(buff)) {
    t->sw = NULL;
  }
  else {
    t->sw = (int *) Malloc(sizeof(int)*n);
    sscanf(buff,"%d",&(t->sw[0]));
    for(i=1;i<n;i++) {
      GETDATALINE;
      sscanf(buff,"%d",&(t->sw[i]));
    }
  }
  t->titles = (char **) Malloc(sizeof(char *)*(n+1));
  for(i=0;i<n;i++) {
    GETDATALINE;
    t->titles[i] = (char *)Malloc(100);
    StringCopy(t->titles[i],buff);
  }
  t->titles[n]=NULL;
  GETDATALINE;
  sscanf(buff,"%d%d",&(t->x2),&(t->y2));
  GETDATALINE;
  if(NULLDATA(buff)) {
    t->butncode = NULL;
  }
  else {
   t->butncode = (char *)Malloc(100);
   StringCopy(t->butncode,buff);
  }
  for(i=0;i<3*n;i++) {
    GETDATALINE;
    if(!NULLDATA(buff)) {
      t->xpm[i]= (void *)Malloc(100);
      pt =(char *) t->xpm[i];
      strcpy(pt,"##");
      sscanf(buff,"%s",(char *)(pt+2));
    }
    else t->xpm[i]=NULL;
  }
  for(i=0;i<n;i++) {
    GETDATALINE;
    if(!NULLDATA(buff)) {
      sscanf(buff,"%d",(t->bkgr)+i);
    }
    else break;
  }
  GETDATALINE;
  sscanf(buff,"%d%f%d",&(t->type),&(t->fac),&(t->bordr));
  GETDATALINE;
  sscanf(buff,"%d",&(t->hide));
  if((t->hide != 0) &&(t->hide!=1)) t->hide=0;
//  t->df = (int *)tmp;
  t->df = (int *)&(t->bval);
  t->bval=1;
  t->arg = NULL;
  t->Update=NULL;
  _uiReadWid(fp,t->Wid);
  return t;
}
void kgPrintWidgetData(void *W,FILE *fp1) {
  DIT *t;
  t = (DIT *)W;
   switch(t->code) {
     case 't':
      fprintf(fp1,"//Text Box\nt\n");
      Print_data_textbox(fp1,(DIT *)t);
      break;
     case 'x':
      fprintf(fp1,"//Select Menu\nx\n");
      Print_data_selectmenu(fp1,(DIX *)t);
      break;
     case 'y':
      fprintf(fp1,"//Thumbnail Browser\ny\n");
      Print_data_thumbnailbrowser(fp1,(DIY *)t);
      break;
     case 'r':
      fprintf(fp1,"//Radio Button\nr\n");
      Print_data_radiobutton(fp1,(DIRA *)t);
      break;
     case 'c':
      fprintf(fp1,"//Check Box\nc\n");
      Print_data_checkbox(fp1,(DICH *)t);
      break;
     case 'w':
      fprintf(fp1,"//browser\nw\n");
      Print_data_browser(fp1,(DIW *)t);
      break;
     case 'e':
      fprintf(fp1,"//Menu\ne\n");
      Print_data_scrollmenu(fp1,(DIE *)t);
      break;
     case 's':
      fprintf(fp1,"//Msg Scroll\ns\n");
      Print_data_msgscroll(fp1,(DIS *)t);
      break;
     case 'b':
      fprintf(fp1,"//Button Box\nb\n");
      Print_data_buttonbox(fp1,(DIB *)t);
      break;
     case 'n':
      fprintf(fp1,"//Button Box\nn\n");
      Print_data_buttonboxn(fp1,(DIN *)t);
      break;
     case 'N':
      fprintf(fp1,"//Button Box new\nn\n");
      Print_data_buttonboxnew(fp1,(DIBN *)t);
      break;
     case 'h':
      fprintf(fp1,"//Horiz bar\nh\n");
      Print_data_buttonboxn(fp1,(DIN *)t);
      break;
     case 'H':
      fprintf(fp1,"//Hori bar new with pixmaps and colour\nH\n");
      Print_data_horibarnew(fp1,(DILN *)t);
      break;
     case 'p':
      fprintf(fp1,"//Pixmap Box\np\n");
      Print_data_pixmapbox(fp1,(DIP *)t);
      break;
     case 'i':
      fprintf(fp1,"//Info Box\ni\n");
      Print_data_infobox(fp1,(DII *)t);
      break;
     case 'g':
      fprintf(fp1,"//Graph Box\ng\n");
      Print_data_graphbox(fp1,(DIG *)t);
      break;
     case 'm':
     case 'B':
     case 'M':
      fprintf(fp1,"//Message Box\nm\n");
      Print_data_message(fp1,(DIM *)t);
      break;
     case 'o':
      fprintf(fp1,"//Progress Bar\no\n");
      Print_data_progressbar(fp1,(DIO *)t);
      break;
     case 'v':
      fprintf(fp1,"//VertScroll\nv\n");
      Print_data_vertscroll(fp1,(DIV *)t);
      break;
     case 'z':
      fprintf(fp1,"//HorizScroll\nz\n");
      Print_data_horizscroll(fp1,(DIZ *)t);
      break;
     case 'T':
      fprintf(fp1,"//Table Box\nT\n");
      Print_data_tablebox(fp1,(DIT *)t);
      break;
     case 'f':
      fprintf(fp1,"//Float Slide\nf\n");
      Print_data_floatslide(fp1,(DIF *)t);
      break;
     case 'd':
      fprintf(fp1,"//Int Slide\nd\n");
      Print_data_intslide(fp1,(DID *)t);
      break;
     case 'P':
      fprintf(fp1,"// Hori Slidei Bar\nP\n");
      Print_data_horislide(fp1,(DIHB *)t);
      break;
     deafault:
      break;
   }
   return;
}
void Print_gui_data(DIALOG *D,char *flname) {
  FILE *fp1;
  DIT *t;
  Dlink *L;
  DIA *d;
  int i,n,no;
  int x1,y1,x2,y2,xl,yl;
  L = Dopen();
  d = D->d;
  printf("Printing Data to: %s\n",flname);
  i=0;while(d[i].t != NULL) {Dadd(L,d[i].t);i++;};
  fp1 = fopen(flname,"w");
  if(D->Resize != 1) {D->Resize=0; D->MinWidth=D->MinHeight=100;}
  fprintf(fp1,"D      //GUI dimensions xo,yo,xl,yl,lw,rw,tw,bw,df,bor_type,bkup\n");
  fprintf(fp1,"%d %d %d %d\n%d %d %d %d\n%d %d %d %d %d %d %d %d %d %d %d %d %f %d %d %d %d\n",
    D->xo,D->yo,D->xl,D->yl,
    D->lw,D->rw,D->tw,D->bw,
    D->df,D->bor_type,D->bkup,
    (D->Sticky),(D->Fixpos),(D->Deco),(D->fullscreen),(D->kbattn),(D->butattn),(D->Newwin),
    (D->DrawBkgr),(D->NoTaskBar),(D->transparency),D->Resize,D->MinWidth,D->MinHeight,D->StackPos
  );
    
  Resetlink(L);
  while ((t=Getrecord(L)) != NULL) {
    kgPrintWidgetData(t,fp1);
  }
  fclose(fp1);
  Dfree(L);
}
DILN * kgCreateHButtons(int xo,int yo,int nx,int length,int height,char **titles,char *name) { /* NEW TYPE */
   DILN *h;
   int i,n=0;
   double fac=0.5;
   int offset=4;
   char **xpm;
   int *bkgr;
   int type=2;
   int clr=-1;
   int xgap=4,ygap=0,lngth=80,width=25,nxb=1,nyb=1;
   DILN htmp = {'H',10L,10L,0,0,5,5,50,25,1,1,NULL,NULL,NULL,
           NULL,NULL,NULL,NULL,NULL,2,0.2,0,0};
   h = (DILN *) Malloc(sizeof(DILN));
   htmp.type=type;
   htmp.fac=fac;
   htmp.Wid[0]='\0';
   htmp.lngth=length;
   htmp.width=height;
   htmp.ygap=0;
   htmp.nx=nx;
   htmp.ny=1;
   if(nx==1)htmp.xgap=0;
   else htmp.xgap=4;
   *h = htmp;
   if(name != NULL)strcpy(h->Wid,name);
   xpm = (char **) Malloc(sizeof(char *)*(h->nx*h->ny)*3);
   bkgr= (int *) Malloc(sizeof(int)*(h->nx*h->ny));
   h->titles= (char **)Malloc(sizeof(char *)*(nx+1));
   h->sw= (int *)Malloc(sizeof(int)*(nx+1));
   h->bkgr = bkgr;
   h->xpm = (void **)xpm;
   for(i=0;i<3*(h->nx*h->ny);i++){
      xpm[i]=NULL;
   }
   for(i=0;i<(h->nx*h->ny);i++){
     bkgr[i]=-1;
     h->titles[i]=NULL;
     h->sw[i]=1;
   }
   if(titles != NULL) {
     for(i=0;i<(h->nx*h->ny);i++){
       if(titles[i]==NULL) break;
       h->titles[i]=(char *)Malloc(strlen(titles[i])+1);
       strcpy(h->titles[i],titles[i]);
     }
   }
   h->df = &(h->bval);
   *(h->df) =1;
   h->butncode = NULL;
   h->x1=xo;h->y1=yo;
   h->x2=h->x1+h->nx*h->lngth+(h->nx)*h->xgap+2*offset;
   h->y2=h->y1+h->ny*(h->width+h->ygap)+2*offset;
   h->Update=NULL;
   h->item=-1;
   return h;
 }
DIB * kgCreateButtonsb(int xo,int yo,int nx,int ny,int length,int height,char **titles,char *name) { /* NEW TYPE */
   DIB *h;
   int i,n=0;
   double fac=0.0;
   int offset=4;
   char **xpm;
   int *bkgr;
   int type=2;
   int clr=-1;
   int xgap=4,ygap=4,lngth=80,width=25,nxb=1,nyb=1;
   DIB htmp = {'b',10L,10L,60,20,5,5,52,52,1,1,NULL,NULL,NULL,
           NULL,NULL,NULL,NULL,NULL,2,0.2,0,0,-1};
   h = (DIB *) Malloc(sizeof(DIB));
   htmp.type=type;
   htmp.fac=fac;
   htmp.Wid[0]='\0';
   htmp.lngth=length;
   htmp.width=height;
   htmp.ygap=4;
   if(ny==1)htmp.ygap=0;
   htmp.nx=nx;
   htmp.ny=ny;
   if(nx==1)htmp.xgap=0;
   else htmp.xgap=4;
   *h = htmp;
   if(name != NULL)strcpy(h->Wid,name);
   xpm = (char **) Malloc(sizeof(char *)*(h->nx*h->ny)*3);
   bkgr= (int *) Malloc(sizeof(int)*(h->nx*h->ny));
   h->titles= (char **)Malloc(sizeof(char *)*(h->ny*h->nx+1));
   h->sw= (int *)Malloc(sizeof(int)*(h->nx*h->ny+1));
   h->bkgr = bkgr;
   h->xpm = (void **)xpm;
   for(i=0;i<3*(h->nx*h->ny);i++){
      xpm[i]=NULL;
   }
   for(i=0;i<(h->nx*h->ny);i++){
     bkgr[i]=-1;
     h->titles[i]=NULL;
     h->sw[i]=1;
   }
   if(titles != NULL) {
     for(i=0;i<(h->nx*h->ny);i++){
       if(titles[i]==NULL) break;
       h->titles[i]=(char *)Malloc(strlen(titles[i])+1);
       strcpy(h->titles[i],titles[i]);
     }
   }
   h->df = &(h->bval);
   *(h->df) =1;
   h->butncode = NULL;
   h->x1=xo;h->y1=yo;
   h->x2=h->x1+h->nx*h->lngth+(h->nx)*h->xgap+2*offset;
   h->y2=h->y1+h->ny*(h->width+h->ygap)+2*offset;
   h->Update=NULL;
   h->item=-1;
   return h;
 }
DIN * kgCreateButtons(int xo,int yo,int nx,int ny,int length,int height,char **titles,char *name) { /* NEW TYPE */
   DIN *h;
   BUT_STR *butn;
   int i,n=0;
   double fac=0.0;
   int offset=4;
   char **xpm;
   int *bkgr;
   int type=7;
   int clr=-1;
   int xgap=4,ygap=4,lngth=80,width=25,nxb=1,nyb=1;
   DIN htmp = {'n',10L,10L,60,20,5,5,52,52,1,1,
           7,0.15,0,0,0,1,NULL,NULL,NULL};
   h = (DIN *) Malloc(sizeof(DIN));
   htmp.type=type;
   htmp.fac=fac;
   htmp.Wid[0]='\0';
   htmp.lngth=length;
   htmp.width=height;
   htmp.ygap=4;
   if(ny==1)htmp.ygap=0;
   htmp.nx=nx;
   htmp.ny=ny;
   if(nx==1)htmp.xgap=0;
   else htmp.xgap=4;
   *h = htmp;
   if(name != NULL)strcpy(h->Wid,name);
   h->df = &(h->bval);
   *(h->df) =1;
   h->x1=xo;h->y1=yo;
   h->x2=h->x1+h->nx*h->lngth+(h->nx)*h->xgap+2*offset;
   h->y2=h->y1+h->ny*(h->width+h->ygap)+2*offset;
   h->Update=NULL;
   h->item=-1;
   h->buts=(void *)Malloc(sizeof(BUT_STR)*nx*ny);
   butn= (BUT_STR *)h->buts;
   for(i=0;i<(nx*ny);i++) {
    butn[i].xpmn=NULL;
    butn[i].xpmp=NULL;
    butn[i].xpmh=NULL;
    butn[i].title[0]='\0';
    butn[i].bkgr = -1;
    butn[i].sw =1;
    butn[i].butncode=31;
   }
   return h;
 }
DIL * kgCreateSplButtons(int xo,int yo,int nx,int ny,int length,int height,char **titles,char *name) { /* NEW TYPE */
   DIL *h;
   BUT_STR *butn;
   int i,n=0;
   double fac=0.2;
   int offset=4;
   char **xpm;
   int *bkgr;
   int type=5;
   int clr=-1;
   int xgap=4,ygap=4,lngth=80,width=25,nxb=1,nyb=1;
   DIL htmp = {'h',10L,10L,60,20,5,5,52,52,1,1,
           7,0.15,0,0,0,1,NULL,NULL,NULL};
   h = (DIL *) Malloc(sizeof(DIL));
   htmp.type=type;
   htmp.fac=fac;
   htmp.Wid[0]='\0';
   htmp.lngth=length;
   htmp.width=height;
   htmp.ygap=4;
   if(ny==1)htmp.ygap=0;
   htmp.nx=nx;
   htmp.ny=ny;
   if(nx==1)htmp.xgap=0;
   else htmp.xgap=4;
   *h = htmp;
   if(name != NULL)strcpy(h->Wid,name);
   h->df = &(h->bval);
   *(h->df) =1;
   h->x1=xo;h->y1=yo;
   h->x2=h->x1+h->nx*h->lngth+(h->nx)*h->xgap+2*offset;
   h->y2=h->y1+h->ny*(h->width+h->ygap)+2*offset;
   h->Update=NULL;
   h->item=-1;
   h->buts=(void *)Malloc(sizeof(BUT_STR)*nx*ny);
   butn= (BUT_STR *)h->buts;
   for(i=0;i<(nx*ny);i++) {
    butn[i].xpmn=NULL;
    butn[i].xpmp=NULL;
    butn[i].xpmh=NULL;
    butn[i].title[0]='\0';
    if(titles!= NULL) {
      if(titles[i] != NULL) strcpy(butn[i].title,titles[i]);
    }
    butn[i].bkgr = -1;
    butn[i].sw =1;
    butn[i].butncode=31;
   }
   return h;
 }
DII * kgCreateInfoBox(int xo,int yo,int nchars,int nlines,char *name){
   DII  *g; 
   int i,n;
   int  l,w;
   float x1,y1,x2,y2;
   DII gtmp = {'i',10,10,20,20,22,0,0,0,0,0};
   g = (DII *)Malloc(sizeof(DII));
   gtmp.Wid[0]='\0';
   *g = gtmp;
   if(name != NULL) strcpy(g->Wid,name);
   x1=xo;y1=yo;
   l = nchars * 9 + 16;
   w = nlines * 22 + 8;
   x2=x1+ l;
   y2=y1+ w;
   g->x1 =x1;
   g->y1 =y1;
   g->x2 =x1 + l;
   g->y2 =y1 + w;
   g->nchrs = nchars;
   g->nlines = nlines;
   g->item=-1;
   return g;
 }
DIG * kgCreateDrawingBox(int xo,int yo,int xsize,int ysize,char *name) {
   DIG  *g; 
   int i,n;
   DIG gtmp = {'g',10,10,20,20,NULL,0,NULL,NULL,0,0};
   g = (DIG *)Malloc(sizeof(DIG));
   gtmp.Wid[0]='\0';
   *g = gtmp;
   g->xpm = NULL;
   if(name!= NULL) strcpy(g->Wid,name);
   g->x1 =xo;
   g->y1 =yo;
   g->x2 =g->x1 + xsize;
   g->y2 =g->y1 + ysize;
   g->bkgr_clr = 0;
   g->xpm=NULL;
   g->initgraph=NULL;
   g->item=-1;
   return g;
}
DIP * kgCreateImageBox(int xo,int yo,int length,int width,char *image,int bordrtype,char * name) {
   DIP  *p; 
   int i,n,col=-1;
   char buf[60],buf1[60]=" ",*xpm;
   int  lng=128,wdth=128;
   int  min=0,max=100,df=0;
   float x1,y1,x2,y2;
   DIP ptmp = {'p',10,10,20,20,NULL,-1};
   p = (DIP *)Malloc(sizeof(DIP));
   ptmp.Wid[0]='\0';
   *p = ptmp;
   if(name != NULL) strcpy(p->Wid,name);
   if( image != NULL) {
     xpm = (char *) Malloc(100);
     strcpy(xpm,"##");
     strcat(xpm,image);
     p->xpm=xpm;
   }
   p->x1 = xo;
   p->y1 = yo;
   p->x2 = p->x1 + length;
   p->y2 = p->y1 + width;
   p->bkgr_clr = -1;
   p->bordr=bordrtype;
   p->hide=0;
   p->item=-1;
   return p;
 }
DIM * kgCreateDisplayBox(int xo,int yo,int length,int width,char *name){
   DIM  *m; 
   int i,n;
   DIM mtmp = {'M',10L,10L,0,0,1,0};
   m = (DIM *)Malloc(sizeof(DIM));
   mtmp.Wid[0]='\0';
   *m = mtmp;
   if(name != NULL) strcpy(m->Wid,name);
   m->hide=0;
   m->x1=xo;
   m->y1=yo;
   m->x2= m->x1+length;
   m->y2= m->y1+width;
   m->hide=0;
   m->justification=1;
   strcpy(m->msg,"");
   m->item=-1;
   return m;
 }
DIO * kgCreateProgressBar(int xo,int yo,int length,int width,char *name){
   DIO  *m; 
   int i,n;
   float x1,y1,x2,y2;
   DIO mtmp = {'o',10L,10L,0,0,0,0};
   m = (DIO *)Malloc(sizeof(DIO));
   mtmp.Wid[0]='\0';
   *m = mtmp;
   if(name != NULL) strcpy(m->Wid,name);
   m->x1 =xo;
   m->y1 =yo;
   m->x2 =xo+length;
   m->y2 =yo+width;
   m->hide = 0;
   m->bordr=0;
   m->percent=0;
   m->type=0;
   m->item=-1;
   return m;
}
DIV * kgCreateVertScroll(int xo,int yo,int height,int width,char *name) {
   DIV  *m; 
   int  wd=20,hide=0;
   DIV mtmp = {'v',10L,10L,30,210,0,100.,0.,5.,NULL,NULL};
   m = (DIV *)Malloc(sizeof(DIV));
   mtmp.Wid[0]='\0';
   *m = mtmp;
   if(name != NULL) strcpy(m->Wid,name);
   m->x1 =xo;
   m->y1 =yo;
   m->x2 =xo+width;
   m->y2 =yo+height;
   m->hide = 0;
   m->bordr=0;
   m->ds =100;
   m->df =0;
   m->mvmt=1;
   m->type=0;
   m->Update=NULL;
   m->item=-1;
   return m;
}
DIZ * kgCreateHorizScroll(int xo,int yo,int length,int width,char *name) {
   DIZ  *m; 
   int  wd=20,hide=0;
   DIZ mtmp = {'z',10L,10L,30,210,0,100.,0.,5.,NULL,NULL};
   m = (DIZ *)Malloc(sizeof(DIZ));
   mtmp.Wid[0]='\0';
   *m = mtmp;
   if(name != NULL) strcpy(m->Wid,name);
   m->x1 =xo;
   m->y1 =yo;
   m->x2 =xo+length;
   m->y2 =yo+width;
   m->hide = 0;
   m->bordr=0;
   m->ds =100;
   m->df =0;
   m->mvmt=1;
   m->type=0;
   m->Update=NULL;
   m->item=-1;
   return m;
}
DIM * kgCreateMessageBox(int xo,int yo,int length,int width,char *msg,int type,char *name) {
   DIM  *m; 
   int i,n;
   char buf[500],buf1[500]=" ";
   int  lng=100;
   int  min=0,max=100,df=0;
   float x1,y1,x2,y2;
   DIM mtmp = {'m',10L,10L,0,0,1,0};
   m = (DIM *)Malloc(sizeof(DIM));
   mtmp.Wid[0]='\0';
   *m = mtmp;
   if(name != NULL) strcpy(m->Wid,name);
   m->x1 =xo;
   m->y1 =yo;
   m->x2 =m->x1+length;
   m->y2 =m->y1+width;
   m->msg[0]='\0';
   if(msg != NULL) strncpy(m->msg,msg,499);
   m->hide=0;
   switch(type) {
      case 0:
        m->code='m';
        m->justification=1;
        break;
      case 1:
        m->code='B';
        m->justification=0;
        break;
      case 2:
        m->code='M';
        m->justification=1;
        break;
      default:
        m->code='m';
        break;
   }
   m->item=-1;
   return m;
 }
static int get_t_length_o(char *c)
 {
  int i=0,no=0,j=0;
  int lng;
  while(*(c+j)== ' ') j++;
  c = c+j;
  while((*(c+i) !='%')&&(*(c+i) != '\0'))i++;
  if(*(c+i)=='\0') { lng = i*9+8+158;}
  else {
   if(i!=0) lng= i*9+8;
   else lng =0;
   i++; if(isdigit(*(c+i))) no =no*10+(*(c+i)-'0');
   i++; if(isdigit(*(c+i))){ no =no*10+(*(c+i)-'0');i++;}
//   if( *(c+i) != 's') no++;
   no +=2;
   lng +=(no*9+13);
  }
  return(lng);
 }
int get_t_length(char *cp)
 {
  int size=0;
  char *cpt;
  cpt=cp;
  while(*cpt != '\0'){
   if((int)(*cpt) == '%') {
    cpt++;
    while(isdigit(*cpt)){
      size= size*10+ (*cpt -'0');
      cpt++;
    }
    size+=1;
   }
   cpt++;
  }
  return (size+1)*9+13;
 }
static int Size(char *t)
 {
   int j=0,lng=0;
   lng =  strlen(t+j)*9;
   if(lng != 0) lng +=(8);
   return ( lng);
 }
static int  _uiScanString(DIT *T,void *str) {
  T_ELMT *e;
  int ch;
  char *cpt,*pt,wrk[5000];
  int i,item=0,it=0,size=6,ln,prsize=0,lngth,width;
  void **v=NULL;
  if(str==NULL) return 0;
  strcpy(wrk,(char *)str);
  cpt = wrk;
  while( *cpt != '\0'){ if(*cpt=='%')item++;cpt++;}
  if(item==0 ) return 0;
  e = (T_ELMT *)Malloc(sizeof(T_ELMT)*item);
  cpt = wrk;
  pt = cpt;
  it=0;
  while( *cpt != '\0'){
   if(*cpt == '%') {
    *cpt='\0';
    ln = Size(pt);
    if(ln>prsize) prsize=ln;
    *cpt='%';
    cpt++;
    size=0;
    while(isdigit(*cpt)){
      size= size*10+ (*cpt -'0');
      cpt++;
    }
    ch =*cpt;
    if(( ch!='d')&&(ch!='f')&&(ch != 's')&&(ch !='F')){
     printf("Error in kgCreateTextbox: /%s/\n",(char*)pt);
     return 0;
    }
    *cpt='\0';
    ln = strlen(pt)+2;
    e[it].fmt = (char *)Malloc(ln);
    strcpy(e[it].fmt,pt);
    e[it].sw=1;
    cpt++;
    pt =cpt;
    switch( ch) {
      case 's':
        strcat(e[it].fmt,"s");
        e[it].sw=1;
        e[it].v= (void *)Malloc(500);
        ((char *)e[it].v)[0]='\0';
      break;
      case 'f':
      case 'g':
        strcat(e[it].fmt,"f");
        e[it].sw=1;
        e[it].v= (void *)Malloc(sizeof(float));
        *((float *)e[it].v) = 0.0;
      break;
      case 'F':
        strcat(e[it].fmt,"F");
        e[it].sw=1;
        e[it].v= (void *)Malloc(sizeof(double));
        *((double *)e[it].v) = 0.0;
      break;
      case 'd':
        strcat(e[it].fmt,"d");
        e[it].sw=1;
        e[it].v= (void *)Malloc(sizeof(int));
        *((int *)e[it].v) = 0;
      break;
    }
    it++;
   }
   else cpt++;
  }
  v = (void **)Malloc(sizeof(void *)*(it+1));
  lngth=0;
  width = (it)*T->width+(it-1)*10;
  for(i=0;i<it;i++) {
     ln =get_t_length(e[i].fmt);
     if(ln > lngth) lngth=ln;
     v[i]=e[i].v;
  }
  v[it]=NULL;
  lngth += (10+4);
  width += (10+4);
  T->elmt=e;
  T->pt=v;
  T->x2 = T->x1+lngth;
  T->y2 = T->y1+width;
  T->pt=v;
  return it;
 }
DIT * kgCreateTextBox(int xo,int yo,char *formats,char *name) {
   DIT *T=NULL;
   T_ELMT *E=NULL;
   int i,n,k,j,code=0,size=0,l;
   char buf[60],buf1[30]=" ",buf2[6]=" ",ch;
   int  nx=1,ny=1,lngth=0,ln,width=20,lnx[1000];
   float x1=10,y1=10,x2,y2;
   double *ftmp;
   int   *itmp;
   char  *ctmp;
   void **v;
   DIT Tmp = {'t',10L,10L,0,0,20L,1L,1L,NULL,0,0,NULL,NULL};
   T = (DIT *)Malloc(sizeof(DIT));
   Tmp.Wid[0]='\0';
   *T = Tmp;
   T->x1 = xo;
   T->y1 = yo;
   T->width=width;
   if(name != NULL) strcpy(T->Wid,name);
   nx=1;
   if ( (ny=_uiScanString(T,formats))==0){
     free(T);
     return NULL;
   }
   width = (ny)*T->width+(ny-1)*10;
   T->nx=nx;
   T->ny=ny;
   T->hide=0;
   T->col = 1;
   T->row = 1;
   T->Update=NULL;
   T->item=-1;
   return T;
 }

