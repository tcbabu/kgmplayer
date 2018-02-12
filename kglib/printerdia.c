#include <stdio.h>
#include <stdlib.h>
#include "kulina.h"
#include <malloc.h>
#include <string.h>
#include "kulina.h"
#include "dlink.h"
#include "imagesprinter.c"
extern int TIFF;
static  int TIFFRES=300;
extern FMGIMG okay_str,Cancel_str;

int  resdiatextbox1callback(int key,int i,void *Tmp) {
  DIA *D;DIT *T;T_ELMT *e; 
  int ret=1;
  D = ((DIALOG *)Tmp)->d;
  T = D[i].t;
  e = T->elmt;
  return ret;
}
int  resdiahoribar1callback(int key,int i,void *Tmp) {
  DIA *D;DIL *B; 
  int n,ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].h;
  n = B->nx;
  switch(key) {
    case 1: 
      break;
  }
  return ret;
}
int resdiainit(void *Tmp) {
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  return ret;
}
int resdia(  void *v0 ){
  int ret=1;
  DIALOG D;
  DIA d[3];
  T_ELMT e0[1]  = { 
    {"Resolution%5d", v0, 1}
  };
  DIT t0 = { 
    't',
    33,21,  
    200,40,
    20, 
    1,1, 
    e0,
    0,0,
    NULL,resdiatextbox1callback /* args,Call back */
  };
  int butn1 =1;
  int sw1[1]  = { 
    1  
  };
  char *titles1[]  = { 
    "Okay", 
    NULL
  };
  char *butncode1  = NULL ; 
  DILN h1 = { 
    'H',
    81,68,  
    159,101,
    9,3,
    62, 
    25, 
    1,1, 
    &butn1, 
    sw1,
    titles1,
    butncode1,
    NULL,resdiahoribar1callback /* arg, Callbak */
  };
  d[0].t = &t0;
  d[1].H = &h1;
  d[2].t = NULL;
  kgInitUi(&D);
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 2;
  D.df = -1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 141;   /* Position of Dialog */ 
  D.yo = 107;
  D.xl = 235;    /*  Length of Dialog */
  D.yl = 120;    /*  Width  of Dialog */
  D.Initfun = resdiainit;    /*  Width  of Dialog */
  D.Deco=1;
  D.DrawBkgr=1;
  D.Bkpixmap=NULL;
  D.Callback=NULL;
  D.transparency = 0.0;
  D.Newwin=0;
  D.Sticky=0;
  D.Fixpos=1;
  D.NoTaskBar=0;
  D.StackPos=0;
  D.Shapexpm=NULL;
  kgDefaultGuiTheme(&(D.gc));
  D.SearchList=NULL;
  ret= kgUi(&D);
  return ret;
}
void Runresdia(void) {
/*************************************************

    Text_Box1  1 data values

*************************************************/
   resdia( &TIFFRES );
}
int get_first_char(char *buff) {
  int i=0;
  while(buff[i]==' ')i++;
  return buff[i];
}
char *get_printer_name(char *buff){
  char *pt=NULL;
  int i=0,j,k,n;
  while(buff[i]==' ')i++;
  j=i;
  while((buff[j]> ' ')&&(buff[j]!=':')&&(buff[j]!='|'))j++;
  n=j-i+1;
  if(n>35) n=35;
  pt=(char *)malloc(n);
  for(k=0;k<n-1;k++) pt[k]=buff[k+i];
  pt[n-1]='\0';
  return pt;
}
char *get_printer(FILE *fp){
  int ch;
  char buff[300],*pt;
  if(fp==NULL) return NULL;
  while( (pt=fgets(buff,298,fp)) != NULL) {
    if( (ch=get_first_char(buff))=='#') continue;
    if( !isalpha(ch) ) continue;
    break;
  }
  if( pt != NULL) {
    return get_printer_name(buff);
  }
  else { return NULL;}

}
 
int  printerdiabrowser3callback(int key,int i,void *Tmp) {
  DIA *D;DIW *B; 
  int ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].w;
  switch(key) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      Runresdia();
      break;
  }
  return ret;
}
int printerdia(  char pr[], void *v0,void *v1, void *v2 ,void *v3){
//#include "blkwht.xpm"
//#include "colour.xpm"
//#include "printer.xpm"
//#include "copies.xpm"
//#include "hardcopy.xpm"
Dlink *Printers=NULL;
FILE *fp;
char *Tofile= " < To File > ";
char *pt;
void *xpm[] = {&blkwht_str,&colour_str,NULL};
  int ret=1;
  int np =0,i=0;
  DIALOG D;
  DIA d[9];
  int butn1 =1;
  int sw0[2]  = { 
    1, 
    1  
  };
  char *titles0[]  = { 
    "Print", 
    "Cancel", 
    NULL
  };
  void *xpm2[]  = {
    NULL,
     NULL,NULL
  };
  int bkgr2[]  = {
    -1,
    -1,-1
  };

  char *butncode0  = NULL ; 
  DILN h0 = { 
    'H',
    137,253,  
    322,292,
    9,3,
    80, 
    25, 
    2,1, 
    &butn1, 
    sw0,
    titles0,
    butncode0,
    NULL,NULL, /* arg, Callbak */
    xpm2,bkgr2
  };
  DIP p1 = { 
    'p',
    20,12,  
    84,74,  
    &Printer_str, /* pixmap info */ 
    -1 /* bkgr colour */ 
  };
  char **menu2 = NULL;
  DIW w2 = { 
    'w',
    13,82,  
    444,112,   
    2,  
    v0,
    "Printer" ,
    menu2 ,
//    244,102,   
    NULL,NULL /* *args, callback */
  };
  char *titles3[]  = { 
    " ", 
    "", 
    NULL
  };
  char *butncode3  = NULL;
  DIBN n3 = { 
    'n',
    29,119,  
    199,198,
    9,15,  
    150, 
    25, 
    1,2, 
    v1, 
    "",
    titles3,
    butncode3,
    NULL,NULL, /* args, Callbak */
    xpm,NULL,
    3,0.2
  };
  char *menu4[]  = { 
    "1",
    "2",
    "3",
    NULL 
  };
  DIW w4 = { 
    'w',
    296,133,  
    418,164,   
    3,  
    v2,
    "Copies" ,
    menu4 ,
    NULL,NULL /* *args, callback */
  };
  DIP p5 = { 
    'p',
    269,149,  
    437,214,  
    &copies_str,
    -1 /* bkgr colour */ 
  };
  DIP p6 = { 
    'p',
    106,15,  
    371,65,  
    &hardcopy_str, /* pixmap info */ 
    -1 /* bkgr colour */ 
  };
  char *menu7[]  = { 
    "Auto",
    "PostScript",
    "Tiff",
    NULL 
  };
  DIW w7 = { 
    'w',
    452,83,  
    656,115,   
    3,  
    v3,
    "Format" ,
    menu7 ,
    NULL,printerdiabrowser3callback /* *args, callback */
  };
  Printers=Dopen();
  pt = (char *)malloc(31);
  strcpy(pt,Tofile);
  Dadd(Printers,pt);
  fp = fopen("/etc/printcap","r");
  if(fp != NULL) {
    while( (pt=get_printer(fp))!=NULL) {Dadd(Printers,pt); }
    fclose(fp);
  }
  np = Dcount(Printers);
  w2.size = np;
  menu2 = (char **)malloc(sizeof(char *)*(np+1));
  i=0;
  Resetlink(Printers);
  while((pt=(char *)Getrecord(Printers)) != NULL){
     menu2[i]= (char *)malloc(31);
     strncpy(menu2[i],pt,30);
     menu2[i][30]='\0';
     i++;
//     menu2[i++]=pt;
  }
  if(w2.size > 4 ) w2.size=4;
  menu2[i]=NULL;
  w2.menu = menu2;
  d[0].H = &h0;
  d[1].p = &p1;
  d[2].w = &w2;
  d[3].n = &n3;
  d[4].w = &w4;
  d[5].p = &p5;
  d[6].p = &p6;
  d[7].w = &w7;
  d[8].t = NULL;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 2;
  D.df = -1;
  D.tw = 4;
  D.bw = 57;
  D.lw = 4;
  D.rw = 4;
  D.xo = 30;   /* Position of Dialog */ 
  D.yo = 30;
  D.xl = 663;    /*  Length of Dialog */
  D.yl = 295;    /*  Width  of Dialog */
  D.Initfun = NULL;    /*  Width  of Dialog */
  D.Deco=1;
  D.DrawBkgr=1;
  D.Bkpixmap=NULL;
  D.Callback=NULL;
  D.transparency = 0.0;
  D.Newwin=0;
  D.Sticky=0;
  D.Fixpos=1;
  D.NoTaskBar=0;
  D.StackPos=0;
  D.Shapexpm=NULL;
  kgDefaultGuiTheme(&(D.gc));
  D.SearchList=NULL;
  ret= kgUi(&D);
  Resetlink(Printers);
  Dposition(Printers,(*((int *)v0)));
  pt = (char *)Getrecord(Printers);
  strcpy(pr,pt);
  Dempty(Printers);
  for(i=0;i<=np;i++) free(menu2[i]);
  free( menu2);
  return ret;
}
#if 0
int Runprinterdia(void *parent) {
   int   v0 = 1,key;
   int   v1 = 1; /* colour / bw */
   int   v2 = 1; /* copies */
   int   v3 = 1;
   static int entry=1;
   char pt[300],*tmp;
   char flname[60];
   char filter[60];
   char command[300];
   strcpy(filter,"*");
   strcpy(flname,"hcopy.ps");
   pt[0]='\0';
   if(entry) {
     FILE *fp;
     entry =0;
     if( (tmp=getenv("KULINA")) != NULL) {
       strcpy(command,tmp);
       strcat(command,"/lib/TIFFRES");
       fp = fopen(command,"r");
       if(fp != NULL) {
         if(fscanf(fp,"%d",&TIFFRES) <=0 ) TIFFRES= 300;
         fclose(fp);
       }
     }
   }
   key =printerdia( pt, &v0,&v1, &v2 ,&v3);
   switch(key) {
      case 1:
       switch(v3) {
         case 1:
          if(TIFF) {
           sprintf(command,"tiff %d",TIFFRES);
           system(command);
          }
          else {
            switch(v1) {
              case 1: system("mscript >/dev/null");break;
              case 2: system("pscript >/dev/null");break;
            }
          }
          break;
         case 2:
            switch(v1) {
              case 1: system("mscript >/dev/null");break;
              case 2: system("pscript >/dev/null");break;
            }
          break;
         case 3:
           sprintf(command,"tiff %d",TIFFRES);
           system(command);
          break;
       }
       if ( v0!=1) {
            sprintf(command,"Printing to |%s| ?",pt);
            if( kgCheckMenu(parent,50,50,command,1) ){
             sprintf(command,"lpr -r -h  -P \"%s\" -# %-d GRAF",pt,v2);
             system(command);
            }
       }
       else  {/* to file */
            kgFileBrowser(parent,10,10,flname,filter);
            sprintf(command,"mv GRAF %s 2>/dev/null",flname);
            system(command);
       }
       return 1;
      case 2:
        return 1;
   }
}
#endif
int  _uiprinterhoribar1callback(int key,int i,void *Tmp) {
  DIA *D;DILN *B; 
  int n,ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].H;
  n = B->nx;
  switch(key) {
    case 1: 
      break;
    case 2: 
      break;
  }
  return ret;
}
int _uiprinter(char pr[],  void *v0, void *v1 ){
  int ret=1;
  DIALOG D;
  DIA d[6];
  Dlink *Printers=NULL;
  FILE *fp;
  char *Tofile= "   < To File >                        ";
  char *pt;
  char **menu2 = NULL;
  int i,np;

  char *menu0[]  = { 
    "< To File >                    ",
    "",
    NULL 
  };
  DIW w0 = { 
    'w',
    43,21,  
    363,50,   
    3,  
    v0,
    "" ,
    menu0 ,
    NULL,NULL /* *args, callback */
  };
  char *menu1[]  = { 
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    NULL 
  };
  DIW w1 = { 
    'w',
    203,80,  
    328,110,   
    3,  
    v1,
    "Copies" ,
    menu1 ,
    NULL,NULL /* *args, callback */
  };
  int butn1 =1;
  int sw2[2]  = { 
    1, 
    1  
  };
  char *titles2[]  = { 
    "", 
    "", 
    NULL
  };
  void *xpm2[]  = { 
//    "##Okay248.jpg", 
//    "##Cancel148.jpg", 
    &okay_str,
    &Cancel_str,
    NULL
  };
  int bkgr2[]  = { 
    -1, 
    -1, 
    -1
  };
  char *butncode2  = NULL;
  DILN H2 = { 
    'H',
    136,158,  
    265,228,
    5,0,
    54, 
    54, 
    2,1, 
    &butn1, 
    sw2,
    titles2,
    butncode2,
    NULL,_uiprinterhoribar1callback,/* arg, Callbak */
    xpm2,bkgr2 /* pointers to xpms and colors */
  };
  DIP p3 = { 
    'p',
    12,44,  
    140,172,  
    NULL, /* pixmap info */ 
    -1 /* bkgr colour */ 
  };
  DIP p4 = { 
    'p',
    186,57,  
    242,87,  
    NULL, /* pixmap info */ 
    -1 /* bkgr colour */ 
  };
  Printers=Dopen();
  pt = (char *)malloc(31);
  strcpy(pt,Tofile);
  Dadd(Printers,pt);
  fp = fopen("/etc/printcap","r");
  if(fp != NULL) {
    while( (pt=(char *)get_printer(fp))!=NULL) {Dadd(Printers,pt); }
    fclose(fp);
  }
  np = Dcount(Printers);
  w0.size = np;
  menu2 = (char **)malloc(sizeof(char *)*(np+1));
  i=0;
  Resetlink(Printers);
  while((pt=(char *)Getrecord(Printers)) != NULL){
     menu2[i]= (char *)malloc(31);
     strncpy(menu2[i],pt,30);
     menu2[i][30]='\0';
     i++;
//     menu2[i++]=pt;
  }
  menu2[i]=NULL;
  w0.menu = menu2;

  d[0].w = &w0;
  d[1].w = &w1;
  d[2].H = &H2;
  d[3].p = &p3;
  d[4].p = &p4;
  d[5].t = NULL;
  D.kbattn = 0;
  D.butattn = 0;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = -1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 214;   /* Position of Dialog */ 
  D.yo = 120;
  D.xl = 389;    /*  Length of Dialog */
  D.yl = 233;    /*  Width  of Dialog */
  D.Initfun = NULL;    /*  Width  of Dialog */
  D.Deco=1;
  D.DrawBkgr=1;
  D.Bkpixmap=NULL;
  D.Callback=NULL;
  D.transparency = 0.0;
  D.Newwin=0;
  D.Sticky=0;
  D.Fixpos=1;
  D.NoTaskBar=0;
  D.Shapexpm=NULL;
  D.StackPos=0;
  kgDefaultGuiTheme(&(D.gc));
  D.SearchList=NULL;
  ret= kgUi(&D);
  Resetlink(Printers);
  Dposition(Printers,(*((int *)v0)));
  pt = (char *)Getrecord(Printers);
  strcpy(pr,pt);
  Dempty(Printers);
  for(i=0;i<=np;i++) free(menu2[i]);
  free( menu2);
  return ret;
}
int uiPrintFile(void * parent,char *flname) {
   int   v0 = 1,key;
   int   v1=1; //copies
   int   v2 = 1;
   char pt[300];
   char filter[60];
   char command[300];
   strcpy(filter,"*.ps");
   pt[0]='\0';
   key =_uiprinter( pt, &v0, &v1);
   if(key==1) {
     if(v0 > 1)sprintf(command,"lpr -r -h  -P \"%s\" -# %-d %s",pt,v1,flname);
     else     {
       strcpy(pt,"hcopy.ps");
       kgFileBrowser(parent,200,200,pt,filter);
       sprintf(command,"mv %s %s",flname,pt);
     }
     system(command);
     return 1;
   }
   else return 0;
}
/*
  Same Runprinterdia; but the printer
  selection menu comes only once
  useful for multiple pages
*/
static int processpagenos(char *flname) {
 char buff[500];
 int npg=0,i;
 FILE *fp,*op;
 fp = fopen(flname,"r");
 op = fopen("GRAF","w");
 if(op == NULL) {
   printf("ERROR:Could not open GRAF for writting...\n");
   exit(0);
 }
 if(fp != NULL) {
  while(fgets(buff,499,fp) != NULL) {
    if((buff[0]=='%')&&(buff[1]=='%')&&(buff[2]=='P')) npg++;
  }
  rewind(fp);
  i=0;
  while(fgets(buff,499,fp) != NULL) {
    if((buff[0]=='%')&&(buff[1]=='%')&&(buff[2]=='P')){
      i++;
      fprintf(op,"%%%%Page: %d %d\n",npg,i);
    }
    else fprintf(op,"%s",buff);
  }
  fclose(fp);
 }
 fclose(op);
 return 1;
}
#if 0
int PrintPages(void *parent,int last) {
   static int   v0 = 1,key;
   static int   v1 = 1; /* colour / bw */
   static int   v2 = 1; /* copies */
   static int   v3 = 1;
   static int entry=1;
   static char pt[200]="",*tmp;
   char flname[60];
   char command[300];
   if(last) {
     entry =0;
     processpagenos(".PAGES");
     remove(".PAGES");
     if ( pt[0] != '\0') {
            sprintf(command,"Printing on Printer |%s| ?",pt);
            if( kgCheckMenu(parent,50,50,command,1) ){
             sprintf(command,"lpr -h -r -P%s -#%-d GRAF",pt,v2);
             printf("lpr -h -r -P%s -#%-d GRAF",pt,v2);
             system(command);
            }
     }
     else  {/* to file */
            flname[0]='\0';
            FileNameBrwser("Copy to",flname);
            sprintf(command,"mv GRAF %s 2>/dev/null",flname);
            system(command);
     }
     return 1;
   }
   if(entry) {
     FILE *fp;
     entry =0;
     remove(".PAGES");
     if( (tmp=getenv("KULINA")) != NULL) {
       strcpy(command,tmp);
       strcat(command,"/lib/TIFFRES");
       fp = fopen(command,"r");
       if(fp != NULL) {
         if(fscanf(fp,"%d",&TIFFRES) <=0 ) TIFFRES= 300;
         fclose(fp);
       }
     }
     key =printerdia( pt, &v0,&v1, &v2 ,&v3);
   }
   switch(key) {
      case 1:
       switch(v3) {
         case 1:
          if(TIFF) {
           sprintf(command,"tiff %d",TIFFRES);
           system(command);
          }
          else {
            switch(v1) {
              case 1: system("mscript >/dev/null");break;
              case 2: system("pscript >/dev/null");break;
            }
          }
          break;
         case 2:
            switch(v1) {
              case 1: system("mscript >/dev/null");break;
              case 2: system("pscript >/dev/null");break;
            }
          break;
         case 3:
           sprintf(command,"tiff %d",TIFFRES);
           system(command);
          break;
       }
       system("cat GRAF>> .PAGES");
       return 1;
      case 2:
        return 1;
   }
}
#endif
int MakePage(int bw) {
   static int   v0 = 1,key;
   static int   v1 = 1; /* colour / bw */
   static int   v2 = 1; /* copies */
   static int   v3 = 1;
   static int entry=1;
   static char *pt;
   char flname[60];
   char command[300];
   switch(bw) {
      case 1: system("mscript >/dev/null");break;
      default: system("pscript >/dev/null");break;
   }
   system("cat GRAF>> .PAGES");
   return 1;
}
int MakePsFile(char *flname) {
   static int   v0 = 1,key;
   static int   v1 = 1; /* colour / bw */
   static int   v2 = 1; /* copies */
   static int   v3 = 1;
   static int entry=1;
   static char *pt;
   char command[300];
   entry =0;
   processpagenos(".PAGES");
   remove(".PAGES");
   sprintf(command,"mv GRAF %s 2>/dev/null",flname);
   system(command);
   return 1;
}
int SendtoPrinter(char *printer) {
   static int   v0 = 1,key;
   static int   v1 = 1; /* colour / bw */
   static int   v2 = 1; /* copies */
   static int   v3 = 1;
   static int entry=1;
   static char *pt;
   char flname[60];
   char command[300];
     entry =0;
     processpagenos(".PAGES");
     remove(".PAGES");
     sprintf(command,"lpr -h -r -P%s  GRAF",printer);
     system(command);
     return 1;
}
