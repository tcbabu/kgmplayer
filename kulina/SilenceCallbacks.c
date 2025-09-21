#include <kulina.h>
#include <fcntl.h>


extern int Meandb;
static DIX *X1=NULL;
static DIT *T1;
static DIRA *ToolsBox=NULL;
static double meanVol=-20.0,maxVol=0,histVol=0,meanout,maxout,vrange;
static int tcount,ttotal ;
static double duration,hr,min,sec;
static double meanlevel;
static int highper=0,voldiff=0;
static void *InfoBox=NULL,*Dia;

int ResetGrpVis(void *Tmp);
int MakeOutputFile(char *Infile,char *Outfile,char *ext);
int ProcessToPipe(int pip0,int pip1,int Pid);
int runjob(char *job,int (*ProcessOut)(int,int,int));
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int runfunctionbkgr(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int runnormalisebkgr(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **),char *);
int RunSilencebkgr(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **),char *infile,char *outfile) ;
int RunSilencebkgr_o(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **),char *infile,char *outfile) ;
int FileStat(char *flname);
int kgffmpeg(int,char **);
int ffmpegfun(int,char **);
int kgffmpeg(int,char **);
int GetBaseIndex(char *s);
int GetLine(int pip0,char *buff);
int SearchString(char *s1,char *s2);
void *RunVolumeSilence(void *);
void *RunVolumeDetect(void *);
int ChangeVolume(char *Infile ,char *Outfile,double EnhFac);

typedef struct _volstr {
	char Infile[200];
	char Outfile[200];
	int  MeanDb;
	double corval;
	double meanVol;
	double histVol;
	double maxVol;
	double duration;
} VOLSTR;

  static int DeleteSelection ( void *Wid ) {
      int i;
      i = 0;
      while ( kgGetThumbNail ( Wid , i ) != NULL ) {
          if ( kgGetSwitch ( Wid , i ) == 1 ) {
              kgDeleteThumbNail ( Wid , i ) ;
          }
          else i++;
      }
      return 1;
  }
  static int ClearSelection ( void *Wid ) {
      int i;
      ThumbNail **Th = ( ThumbNail ** ) kgGetList ( Wid ) ;
      i = 0;
      while ( Th [ i ] != NULL ) {
          Th [ i ] -> sw = 0;
          i++;
      }
      return 1;
  }
  static int DeleteAll ( void *Wid ) {
      int i;
      ThumbNail **Th = ( ThumbNail ** ) kgGetList ( Wid ) ;
      if(Th==NULL) return 1;
      i = 0;
      while ( Th [ i ] != NULL ) {
          Th [ i ] -> sw = 1;
	  i++;
      }
      DeleteSelection(Wid);
      return 1;
  }
  static int CopyItems ( void *Tmp , char **Str ) {
      DIY *FY , *TY;
      int item = 0 , i = 0 , j = 0,count=0;
      int Added=0;
      ThumbNail **th,**thold,**thnew;
      int x = -1 , y = -1;
      j = 0;
      if(Str==NULL) return 0;
      th = ( ThumbNail ** ) kgStringToThumbNails(Str);
      thold = (ThumbNail **)kgGetList(X1);
      if(thold==NULL) {
	 kgSetList(X1,(void **)th);
         kgListRemoveDup ( X1 ) ;
         ClearSelection ( X1 ) ;
         kgUpdateWidget ( X1 ) ;
         kgUpdateOn ( Tmp ) ;
         return 1;
      }
      j = 0;
      while ( thold [ j ] != NULL ) {j++;}
      count =j;
      j=0;
      while ( th [ j ] != NULL ) {j++;}
      count +=j;
      thnew = (ThumbNail **)malloc(sizeof(ThumbNail *)*(count+1));
      j = 0;
      while ( thold [ j ] != NULL ) {thnew[j]=thold[j];j++;}
      i = j;
      j = 0;
      while ( th [ j ] != NULL ) {thnew[i+j]=th[j];j++;}
      thnew[count]=NULL;
      kgSetList(X1,(void **)thnew);
      free(th);
      free(thold);
      kgListRemoveDup ( X1 ) ;
      ClearSelection ( X1 ) ;
      kgUpdateWidget ( X1 ) ;
      kgUpdateOn ( Tmp ) ;
      return 1;
  }
int  Silencebrowser1callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIX *X;void *pt; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  X = (DIX *)kgGetWidget(Tmp,i);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  Silencebrowser1init(DIX *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
}
int  Silencebutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,item; 
  char **Str=NULL;
  ThumbNail **th;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  X1 = (DIX *)kgGetNamedWidget(Tmp,"SXbox");
//  T1 = (DIT *)kgGetNamedWidget(Tmp,"MeanBox");
//  kgSetInt(T1,0,Meandb);
  switch(butno) {
    case 1: 
      DeleteSelection(X1);
      kgUpdateWidget(X1);
      kgUpdateOn(Tmp);
      break;
    case 2: 
      DeleteAll(X1);
      kgUpdateWidget(X1);
      kgUpdateOn(Tmp);
      break;
    case 3: 
  printf("Calling kgGetAudioFiles\n");
      fflush(stdout);
      Str = kgGetAudioFiles(NULL);
      if(Str != NULL) {
	CopyItems(Tmp,Str);
	kgFreeDouble((void **)Str);
      }
      break;
  }
  return ret;
}
void  Silencebutton1init(DIN *B,void *pt) {
}
static int kgffmpeg_(int argc,char *argv[]){
  char Tmpfile[500];
  char buff[200];
  char *outfile;
  int pid,status;
  outfile = argv[argc-1];
  strcpy(Tmpfile,outfile);
  Tmpfile[GetBaseIndex(outfile)]='_';
  Tmpfile[GetBaseIndex(outfile)+1]='\0';
  strcat(Tmpfile,outfile+GetBaseIndex(outfile));
  argv[argc-1]= Tmpfile;
  ffmpegfun(argc,argv);
  return 1;
}
static int MakeOutFile_o(char *Infile,char *Outfile) {
	int i=0,ln=strlen(Infile);
	char buff[200];
	i = ln-1;
	strcpy(Outfile,Infile);
	while((i>=0) &&(Infile[i] != '/')) i--;
	Outfile[i+1]='\0';
	strcat(Outfile,"_");
	strcat(Outfile,Infile+i+1);
	return 1;
}
static int MakeOutFile(char *Infile,char *Outfile) {
	int i=0,ln=strlen(Infile);
	char buff[200];
	i = ln-1;
	strcpy(Outfile,"/tmp/out");
	while((i>=0) &&(Infile[i] != '/')) i--;
	strcat(Outfile,Infile+i+1);
	return 1;
}
int runsilencefunction(char *job,int (*ProcessOut)(void *,int,int),int (*function)(int,char **),char *arg){
   int ret =0;
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2],argc;
   char *args[100],buff[1000],pt[300];
   char *ptr;
   char *pgrpath=NULL;
   int fid;
   int i=0,pos=0;
   if(job==NULL){
	   fprintf(stderr,"Job: NULL \n");
           fflush(stdout);
           fflush(stderr);
	   return 0;
   }
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
   fprintf(stderr,"Job= %s\n",job);
   fflush(stdout);
   fflush(stderr);
//   pipew =pip2[1];
   while(job[i]==' ') i++;
   strcpy(buff,job+i);
   i=0;
   while ( sscanf(buff+pos,"%s",pt) > 0 ) {
     if(pt[0]=='\"') {
      pos++;
      args[i]=buff+pos;
      while(buff[pos]!='\"')pos++;
      buff[pos]='\0';
      i++;
     }
     else {
       args[i]=buff+pos;
       pos +=strlen(pt);
       i++;
       if(buff[pos]< ' ') break;
       buff[pos]='\0';
     }
     pos++;
     while(buff[pos]==' ') pos++;
//     printf("%s \n ",args[i-1]);
//     fflush(stdout);
   }
//   printf("\n");
   args[i]=NULL;
   if(i==0){
	   printf("i==0 Exiting\n");
           fflush(stdout);
	   return 0;
   }
   argc=i;
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     for(i=0;i<argc;i++) {
	   ptr = (char *)malloc(strlen(args[i])+1);
			   strcpy(ptr,args[i]);
			   args[i]=ptr;
     }
//     pgrpath=kgWhich(args[0]);
//     if (pgrpath==NULL) return 0;

     close(pip2[1]);
     close(pip[0]);
     close(0);
     dup(pip2[0]);
//     close(pip2[0]);
#if 1
     close(1);
     dup(pip[1]);
#endif
 //    fid =open(Outfile,O_RDWR|O_CREAT,0777);
     close(2);
     dup(pip[1]);
//     dup(fid);
     close(pip[1]);
     if(function != NULL) function(argc,args);
//     system(job);
//     close(fid);
//     execv(pgrpath,args);
     fflush(stderr);
     fflush(stdout);
     fprintf(stderr,"END:\n");
     fflush(stderr);
     exit(0);
   }
   else {   /* parent process */
//     printf("Waiting pid: %d\n",pid);
     close(pip2[0]);
     close(pip[1]);
     if(ProcessOut != NULL){
        ret=ProcessOut(arg,pip[0],pid);
//        printf("Killing %d\n",pid);
        kill(pid,9);
     }
     waitpid(pid,&status,0);
     return ret;
   }
}
int  Silencesplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=0,j; 
  char buff[500],outfile[200];
  double corval=0.0,meanlevel;
  VOLSTR *vstr=NULL;
  void *Thds;
  void *Busyid;
  ThumbNail **th= (ThumbNail **)kgGetList(X1);
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  X1 = (DIX *)kgGetNamedWidget(Tmp,"SXbox");
//  T1 = (DIT *)kgGetNamedWidget(Tmp,"MeanBox");
  ToolsBox = (DIRA *)kgGetNamedWidget(Tmp,"ToolsBox");
//  meanlevel = kgGetInt(T1,0);
  Busyid = kgOpenBusy(Tmp,400,400);    
  if((th != NULL)&&(th[0]!=NULL)) {
	  Thds = OpenThreads(0);
	  j=0;
	  while(th[j] != NULL) {
            MakeOutFile(th[j]->name,outfile);
            vstr = (VOLSTR *)malloc(sizeof(VOLSTR));
            strcpy(vstr->Infile,th[j]->name);
            strcpy(vstr->Outfile,outfile);
            DoInAnyThread(Thds,RunVolumeSilence,vstr);
	    j++;
	  }
	  WaitThreads(Thds);
	  CloseThreads(Thds);
	  kgFreeThumbNails(th);
	  kgSetList(X1,NULL);
          ResetGrpVis(Tmp);
#if 0
	    kgSetSelection(ToolsBox,1);
	    kgUpdateWidget(ToolsBox);
	    kgUpdateOn(Tmp);
#endif
	    
  }
  else ret =0;
  kgCloseBusy(Busyid);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  Silencesplbutton1init(DIL *B,void *pt) {
}
int  Silencetextbox1callback(int cellno,int i,void *Tmp) {
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
int Silenceinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  X1 = (DIX *)kgGetNamedWidget(Tmp,"SXbox");
//  T1 = (DIT *)kgGetNamedWidget(Tmp,"MeanBox");
//  kgSetInt(T1,0,Meandb);
  return ret;
}
int Silencecleanup(void *Tmp) {
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
int ModifySilence(void *Tmp,int GrpId) {
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

int SilenceCallBack(void *Tmp,void *tmp) {
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
int SilenceResizeCallBack(void *Tmp) {
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
int SilenceWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
