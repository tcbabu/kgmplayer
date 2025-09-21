#include <kulina.h>
#include <fcntl.h>
#include <wait.h>
#include <signal.h>

static char FileName[200];
static char Job[300];
static char CpJob[300];
static int pid=-1;
static char TmpFile[200];
static int Index=0;
char CapHome[200];

#define WAIT(pid) {\
  pid_t w;\
  int status;\
  do { \
                   w = waitpid(pid, &status, WUNTRACED | WCONTINUED); \
                   if (w == -1) { \
                       perror("waitpid"); \
                       exit(EXIT_FAILURE); \
                   }\
               } while (!WIFEXITED(status) && !WIFSIGNALED(status));\
}
static int KillJob(int pid) {
	if(pid > 1) {
	  kill(pid,SIGTERM);
	  WAIT(pid);
	}
	return 1;
}
static int comppath(void *pt1,void *pt2) {
  return strcmp((char *)pt1,(char *)pt2);
}
static int compdup(void *pt1,void *pt2) {
  int ret;
  ret=strcmp((char *)pt1,(char *)pt2);
  if(ret==0) {return 1;}
  else return 0;
}
static char *myWhich(char *pgr) {
/* Caller must free result if it is not NULL */
  int i=0,j,End=0;
  char *pt,**m,*res=NULL,*cpt;
  char path[5000];
  Dlink *L;
  if(pgr[0]=='/') { // full path is given
    int l;
    char **m,*pt1;
    strcpy(path,pgr);
    pt=path;
    l = strlen(pt);
    while(pt[l]!='/') l--;
    if(l==0) {
     m = kgFileMenu("/",pt+1);
    }
    else {
     pt1=pt+l+1;
     pt[l]='\0';
     m = kgFileMenu(pt,pt1);
    }
    if( (m==NULL)) {return NULL;}
    if( (m[0]==NULL)) {kgFreeDouble((void **)m);return NULL;}
    kgFreeDouble((void **)m);
    res = (char *)malloc(strlen(pgr)+1);
    strcpy(res,pgr);
    return res;
  }

  pt = getenv("PATH");
  if(pt==NULL) return NULL;
  strcpy(path , getenv("PATH"));
  L = Dopen();
  while(!End) {
    j=i;
    if(pt[j]< ' ') break;
    while(path[j]!=':') {
      if(path[j]<' ') {End=1;break;}
      j++;
    }
    path[j]='\0';
    cpt = (char *)malloc(strlen(path+i)+1);
    strcpy(cpt,path+i);
    Dadd(L,cpt);
    i=j+1;
  }
  Dsort(L,comppath);
  Drmvdup_cond(L,compdup);
  Resetlink(L);
  while( (pt = (char *)Getrecord(L))!=NULL) {
    m = kgFileMenu(pt,pgr);
    if(m==NULL) continue;
    if(m[0]==NULL) { free(m);continue;}
    res = (char *)malloc(strlen(pt)+1+strlen(m[0])+1);
    strcpy(res,pt);
    strcat(res,"/");
    strcat(res,m[0]);
    i=0;
    while(m[i]!=NULL) {free(m[i]);i++;}
    free(m);
    break;
  }
  Dempty(L);
  return res;
}
static int RunBkgrJob ( char *job , char *InFile , char *OutFile ) {
      FILE *fp , *fp1;
      int pip [ 2 ] , pid , status , pip2 [ 2 ] ;
      char *args [ 100 ] , buff [ 1000 ] , pt [ 300 ] ;
      char *pgrpath = NULL;
      int i = 0 , pos = 0;
      char *cpt;
      while ( job [ i ] == ' ' ) i++;
      strcpy ( buff , job+i ) ;
      i = 0;
      while ( sscanf ( buff+pos , "%s" , pt ) > 0 ) {
          if ( pt [ 0 ] == '\"' ) {
              pos++;
              args [ i ] = buff+pos;
              while ( buff [ pos ] != '\"' ) pos++;
              buff [ pos ] = '\0';
              i++;
          }
	  else if ( pt [ 0 ] == '\\' ) {
              pos++;
              args [ i ] = buff+pos;
              while ( buff [ pos ] != '\\' ) pos++;
              buff [ pos ] = '\0';
              i++;
          }
          else {
              args [ i ] = buff+pos;
              pos += strlen ( pt ) ;
              i++;
              if ( buff [ pos ] < ' ' ) break;
              buff [ pos ] = '\0';
          }
          pos++;
          while ( buff [ pos ] == ' ' ) pos++;
      }
      args [ i ] = NULL;
      if ( i == 0 ) return 0;
      pgrpath = args [ 0 ] ;
      if ( pgrpath == NULL ) return 0;
//      printf("PRGPATH= %s %s\n",pgrpath,args[0]);
      pid = fork ( ) ;
      if ( pid == 0 ) { /* child process */
          int sin , sout;
          if ( InFile != NULL ) {
              sin = open ( InFile , O_RDWR | O_CREAT , 0644 ) ;
              close ( 0 ) ;
              dup ( sin ) ;
              close ( sin ) ;
          }
          if ( OutFile != NULL ) {
              sout = open ( OutFile , O_WRONLY | O_CREAT , 0644 ) ;
              close ( 1 ) ;
              dup ( sout ) ;
              close ( sout ) ;
          }
          close ( 2 ) ;
          open ( "In/dev/null" , O_WRONLY | O_CREAT , 0777 ) ;
//     printf("PRGPATH= %s %s\n",pgrpath,args[0]);
          execv ( pgrpath , args ) ;
          fprintf ( stderr , "Failed to execute \n" ) ;
          exit ( 1 ) ;
      }
      else { /* parent process */
#if 0
          int ret = 0;
          int status;
          WAIT ( pid ) ;
#endif
          return ( pid ) ;
      }
  }
int  acaptextbox1callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
  return ret;
}
int  acapbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 

  D = (DIALOG *)Tmp;
  DIT *T= (DIT *)kgGetNamedWidget(Tmp,"FileName");
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      if(kgFolderBrowser(NULL,200,200,FileName,(char *)"*")) {
	    kgSetString(T,0,FileName);
//	    printf("File: %s\n",FileName);
	    kgUpdateWidget(T);
	    kgUpdateOn(Tmp);
      }	      
      break;
  }
  return ret;
}
void  acapbutton1init(DIN *B,void *pt) {
}
int  acapbutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  DIT *T=(DIT *)kgGetNamedWidget(Tmp,"FileName");
  pid = RunBkgrJob(Job,NULL,NULL);
  kgSetWidgetVisibility(B,0);
  kgSetWidgetVisibility(kgGetNamedWidget(Tmp,(char *)"Continue"),1);
  kgUpdateOn(Tmp);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  acapbutton2init(DIN *B,void *pt) {
}
int GetCapHome(char *FileName) {
	int i=0,ln;
	ln = strlen(FileName);
	i=ln-1;
	strcpy(CapHome,FileName);
	while(i>0) {
	  if(CapHome[i] == '/') break;
	  i--;
	}
	CapHome[i]='\0';
	return 1;
}
int  acapbutton3callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  DIT *T=(DIT *)kgGetNamedWidget(Tmp,(char *)"FileName");
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      sprintf(CpJob,"cp %s %s",TmpFile,kgGetString(T,0));
      system(CpJob);
      Index++;
      GetCapHome(kgGetString(T,0));
      sprintf(FileName,"%-s/song_%3.3d.wav",CapHome,Index);
      kgSetString(T,0,FileName);
      kgUpdateWidget(T);
      kgUpdateOn(Tmp);
      break;
    case 2: 
      kgSetWidgetVisibility(B,0);
      kgSetWidgetVisibility(kgGetNamedWidget(Tmp,(char *)"Continue"),1);
      kgUpdateOn(Tmp);
      pid = RunBkgrJob(Job,NULL,NULL);
      break;
    case 3: 
      remove(TmpFile);
      kgSetExit(Tmp);
      break;
  }
  return ret;
}
void  acapbutton3init(DIN *B,void *pt) {
}
int  acapbutton4callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  kgSetWidgetVisibility(B,0);
  KillJob(pid);
  pid = -1;
  kgSetWidgetVisibility(kgGetNamedWidget(Tmp,(char *)"Pause"),1);
  kgUpdateOn(Tmp);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  acapbutton4init(DIN *B,void *pt) {
}
int acapinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  DIT *T=(DIT *)kgGetNamedWidget(Tmp,(char *)"FileName");
  strcpy(CapHome,getenv("HOME"));
  strcat(CapHome,"/Music");
  sprintf(FileName,"%-s/song_%3.3d.wav",CapHome,Index);
//  printf("FileName = %s\n",FileName);
  kgSetString(T,0,FileName);
  kgUpdateWidget(T);
  sprintf(TmpFile,"/tmp/Tmp_%-3.3d.wav",getpid());
  sprintf(Job,"%-s --file-format=wav %s",myWhich("parecord"),TmpFile);
  pt = D->pt;
  return ret;
}
int acapcleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int Modifyacap(void *Tmp,int GrpId) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  DIA *d;
  int i,n;
  d = D->d;
  i=0;while(d[i].t!= NULL) {;
     i++;
  };
  n=1;
  return GrpId;
}

int acapCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
int acapResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
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
int acapWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
