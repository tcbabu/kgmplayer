#include <kulina.h>
#include <sys/types.h>
#include <sys/stat.h>
//extern CONVDATA cndata;
static char infile[500]="",outfile[500]="";
extern double meanlevel;
int MakeFileInFolder(char *Infile,char *Folder,char *Outfile,char *ext);
double meanVol=-20.0,maxVol=0,histVol=0,meanout,maxout,vrange;
static int tcount,ttotal ;
static double duration,hr,min,sec;
static void *InfoBox=NULL,*Dia;

int ProcessToPipe(int pip0,int pip1,int Pid);
int MakeOutputFile(char *Infile,char *Outfile,char *ext);
int GetBaseIndex(char *s);
int GetLine(int pip0,char *buff);
int SearchString(char *s1,char *s2);
int runjob(char *job,int (*ProcessOut)(int,int,int));
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int runfunctionbkgr(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int runnormalisebkgr(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **),char *);
int FileStat(char *flname);
int kgffmpeg(int,char **);

#if 1
int ProcessVolume(int pip0,int pip1,int Pid) {
     char buff[1000],dummy[100];
     int ch,i=0,j;

     ttotal = 0;
     duration =0;
     meanVol = -25;
//     close(pip1);
     InfoBox = kgGetNamedWidget(Dia,"NomInfoBox");
     while((ch=GetLine(pip0,buff)) ) {
//         kgWrite(InfoBox,buff);
//         kgUpdateOn(Dia);
         if(ch< 0) continue;
         if((i=SearchString(buff,(char *)"Duration:"))>=0) {
           int j=i;
           while((buff[j] >= ' ')||(buff[j]=='\t')) {
            if(buff[j]==':') buff[j]=' ';
            if(buff[j]==',') buff[j]=' ';
            j++;
           }
//           kgWrite(InfoBox,buff+i);
//           kgUpdateOn(Dia);
//           printf("%s\n",buff+i);
           sscanf(buff+i,"%s%lf%lf%lf", dummy,&hr,&min,&sec);
           duration = hr*3600+min*60+sec;
           duration = duration*1000;
         }
         if((i=SearchString(buff,(char *)"mean_volume:"))>=0) {
           kgWrite(InfoBox,buff+i);
           kgUpdateOn(Dia);
           sscanf(buff+i,"%s%lf", dummy,&meanVol);
         }
         if((i=SearchString(buff,(char *)"max_volume:"))>=0) {
           kgWrite(InfoBox,buff+i);
           kgUpdateOn(Dia);
           sscanf(buff+i,"%s%lf", dummy,&maxVol);
         }
         if((i=SearchString(buff,(char *)"histogram_"))>=0) {
//           kgWrite(InfoBox,buff+i);
//           kgUpdateOn(Dia);
           int j=0;
           i = i+10;
           j = i;
           while(buff[j] != 'd') j++;
           buff[j]=' ';
           sscanf(buff+i,"%lf", &histVol);
           histVol = -histVol;
           if((j=SearchString(buff+i,(char *)"b:"))>=0) {
             sscanf(buff+i+j+2,"%d",&tcount);
             ttotal +=tcount;
           }
         }
     }
     sprintf(buff,"Mean Volume : %lf Max Vol %lf Hist Vol %lf\n",meanVol,maxVol,histVol);
     kgWrite(InfoBox,buff);
     kgUpdateOn(Dia);
     vrange = maxVol - meanVol;
     return 1;
}
#endif

int  normalizetextbox1callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;
  char FileName[500],OutFile[500];
  int ret=1;
  D = (DIALOG *)Tmp;
  DIT *T,*TO;
  T = (DIT *)kgGetNamedWidget(Tmp,"NomInputWidget");
  TO = (DIT *)kgGetNamedWidget(Tmp,"NomOutputWidget");
  strcpy(FileName,kgGetString(T,0));
#if 0
  sprintf(OutFile,"%-s/Music/",getenv("HOME"));
  MakeOutputFile(FileName,OutFile+strlen(OutFile),"mp3");
#else
  sprintf(OutFile,"%-s/Music",getenv("HOME"));
  MakeFileInFolder(FileName,OutFile,OutFile,"mp3");
#endif
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  strcpy(infile,FileName);
  strcpy(outfile,OutFile);
  return ret;
}
int  normalizebutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  static char FileName[500],OutFile[500];
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  DIT *T,*TO;
  T = (DIT *)kgGetNamedWidget(Tmp,"NomInputWidget");
  TO = (DIT *)kgGetNamedWidget(Tmp,"NomOutputWidget");
  n = B->nx*B->ny;
  FileName[0]='\0';
  strcpy(FileName,kgGetString(T,0));
  kgFolderBrowser(NULL,100,100,FileName,"*");
  kgSetString(T,0,FileName);
#if 0
  sprintf(OutFile,"%-s/Music/",getenv("HOME"));
  MakeOutputFile(FileName,OutFile+strlen(OutFile),"mp3");
#else
  sprintf(OutFile,"%-s/Music",getenv("HOME"));
  MakeFileInFolder(FileName,OutFile,OutFile,"mp3");
#endif
  kgSetString(TO,0,OutFile);
  kgUpdateWidget(T);
  kgUpdateWidget(TO);
  kgUpdateOn(Tmp);
  strcpy(infile,FileName);
  strcpy(outfile,OutFile);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
int  normalizetextbox2callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;
  char FileName[500],OutFile[500];
  int ret=1;
  D = (DIALOG *)Tmp;
  DIT *T,*TO;
  T = (DIT *)kgGetNamedWidget(Tmp,"NomInputWidget");
  TO = (DIT *)kgGetNamedWidget(Tmp,"NomOutputWidget");
  strcpy(OutFile,kgGetString(TO,0));
  strcpy(outfile,OutFile);
  return ret;
}
static int CheckString(char *s1,char *s2) {
   int ch,ln,ret=0;
   ln = strlen(s2);
   if(strlen(s1)<ln) ret=0;
   else {
     ch = s1[ln];
     s1[ln]='\0';
     if( strcmp(s1,s2)==0 ) ret=1;
     s1[ln]=ch;
   }
   return ret;
}
void  normalizebutton1init(DIN *B,void *pt) {
}
int  normalizebutton2callback(int butno,int i,void *Tmp) {
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
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  normalizebutton2init(DIN *B,void *pt) {
}
int  normalizetextbox3callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  char buff[200];
  D = (DIALOG *)Tmp;
  Dia = D;
  T = (DIT *)kgGetWidget(Tmp,i);
  meanlevel = kgGetDouble(T,cellno);
  sprintf(buff,"mean Level = %lfdB\n",meanlevel);
  InfoBox = kgGetNamedWidget(Tmp,"NomInfoBox");
  kgWrite(InfoBox,buff);
  kgUpdateOn(Dia);
  e = T->elmt;
  return ret;
}
int kgffmpeg_o(int argc,char *argv[]){
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
  if( (pid=fork())==0) {
    kgffmpeg(argc,argv);
    exit(0);
  }
  else {
    waitpid(pid,&status,0);
    remove(outfile);
    rename(Tmpfile,outfile);
  }
  return 1;
}
int kgffmpeg_(int argc,char *argv[]){
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
  kgffmpeg(argc,argv);
  return 1;
}

int updateoutfile(char *outfile){
  char Tmpfile[500];
  char buff[200];
  strcpy(Tmpfile,outfile);
  Tmpfile[GetBaseIndex(outfile)]='_';
  Tmpfile[GetBaseIndex(outfile)+1]='\0';
  strcat(Tmpfile,outfile+GetBaseIndex(outfile));
  remove(outfile);
  rename(Tmpfile,outfile);
  return 1;
}

int  normalizesplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  char buff[500];
  double corval=0.;
  int highper=0,voldiff=0;
  D = (DIALOG *)Tmp;
  Dia = D;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  InfoBox = kgGetNamedWidget(Tmp,"NomInfoBox");
  switch(butno) {
    case 1: 
      if((infile==NULL) || (infile[0]=='\0')) {
        kgWrite(InfoBox,"No Input file selected\n");
        kgUpdateOn(Dia);
      }
      else {
        strcpy(infile,kgGetString(kgGetNamedWidget(Dia,"NomInputWidget"),0));
        strcpy(outfile,kgGetString(kgGetNamedWidget(Dia,"NomOutputWidget"),0));
        meanlevel = kgGetDouble(kgGetNamedWidget(Dia,"NomMeanWidget"),0);
        sprintf(buff,"kgffmpeg -i \"%s\" -af \"volumedetect\" -vn -sn "
          " -dn -f null /dev/null", infile);
        runfunction(buff,ProcessVolume,kgffmpeg);

        sprintf(buff,"Duration = %lf  ttotal %d\n",duration,ttotal);
        kgWrite(InfoBox,buff);
        kgUpdateOn(Dia);
        corval = meanlevel-meanVol;
        highper = ttotal/duration*100;
        sprintf(buff,"Volume Correction = %lf\n",corval);
        kgWrite(InfoBox,buff);
        kgUpdateOn(Dia);
//        remove(outfile);
        sprintf(buff,"kgffmpeg_ -i \"%s\" -af \"volume=%lfdB\" -y \"%s\"", 
                      infile,corval,outfile);
//        runfunction(buff,NULL,kgffmpeg);
//        runfunctionbkgr(buff,ProcessToPipe,kgffmpeg_);
        runnormalisebkgr(buff,ProcessToPipe,kgffmpeg_,outfile);
      }
      ret=0;
      break;
    case 2: 
      break;
  }
  return ret;
}
void  normalizesplbutton1init(DIL *B,void *pt) {
}
int normalizeinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  InfoBox = kgGetNamedWidget(Tmp,"NomInfoBox");
  Dia=Tmp;
  return ret;
}
int normalizecleanup(void *Tmp) {
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
int normalizeCallBack(void *Tmp,void *tmp) {
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
int normalizeResizeCallBack(void *Tmp) {
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
int normalizeWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
