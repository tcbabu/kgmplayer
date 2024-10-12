#include <kulina.h>
int KillJob(int);

int  MonitorVolsplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  D = (DIALOG *)Tmp;
  Dlink *L=NULL;
  char *str=NULL;
  char **pt= (char **)(D->pt);
  int Pipe,Pid;
  Pid  = *((int *)pt[2]);
  L= (Dlink *)pt[0];
  if(L!= NULL) {
	  Resetlink(L);
	  str = (char *)Getrecord(L);
	  if((str!=NULL)&&(str[0]>= ' '))remove(str);
//	  remove(str);
  }
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  switch(butno) {
    case 1: 
//      KillJob(Pid);
      kill(Pid,9);
      break;
  }
  return ret;
}
void  MonitorVolsplbutton1init(DIL *B,void *pt) {
}
int MonitorVolinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  DII *I1=NULL;
  char **pt= (char **)(D->pt);
  char *str;
  char buff[200];
  int Pipe,Pid;
  Pipe = *((int *)pt[1]);
  Pid  = *((int *)pt[2]);
#if 1
  Dlink *L=NULL;
  L= (Dlink *)pt[0];
  I1 =(DII *)kgGetNamedWidget(Tmp,"VolInfo1");
  sprintf(buff,"Pipe =%d Pid = %d",Pipe,Pid);
  kgWrite(I1,buff);
  if(L!= NULL) {
	  Resetlink(L);
	  while( (str=(char *)Getrecord(L)) != NULL){
//	      printf("STr= %s\n",str);
              kgWrite(I1,str);
	  }
  }
#endif
  return ret;
}
int MonitorVolcleanup(void *Tmp) {
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
int ModifyMonitorVol(void *Tmp,int GrpId) {
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

int MonitorVolCallBack(void *Tmp,void *tmp) {
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
int MonitorVolResizeCallBack(void *Tmp) {
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
static int GetTimedLine(int pip0,char *buff,int usec){
     unsigned char ch;
     fd_set rfds;
     struct timeval tv;
     int retval,chnl,status,i;
     int ret,val;
     chnl = pip0;
     i=0;
     FD_ZERO(&rfds);
     FD_SET(pip0,&rfds);
     while(1) {
       tv.tv_sec = 0;
       tv.tv_usec =usec;
       ret=0;
       retval = select(chnl+1,&rfds,NULL,NULL,&tv);
       if(retval < 0) {ret=0;break;}
       val=1;
       if((retval> 0)&&(FD_ISSET(chnl,&rfds))){
         if(read(chnl,&ch,1) != 1){
           ret=0;
           break;
         }
         buff[i++]=ch;
         if(i>490) i=490;
         if( (ch=='\n')||(ch=='\r')) {ret=ch;break;}
       }
       else {ret=-1;break;}
     }
     buff[i]='\0';
     return ret;
}
int MonitorVolWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0,rval;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  DII *I1=NULL,*I2=NULL;
  static int count=0;
  char **pt= (char **)(D->pt);
  int Pipe,Pid;
  char buff[500];
  I1 =(DII *)kgGetNamedWidget(Tmp,"VolInfo1");
  I2 =(DII *)kgGetNamedWidget(Tmp,"VolInfo2");
  Pipe = *((int *)pt[1]);
  Pid  = *((int *)pt[2]);
#if 1
 if ((rval=GetTimedLine(Pipe,buff,1000))  ) {
#if 0
  count++;
  count = count%3;
  switch(count) {
	  case 0: kgWrite(I2,"Please Wait /");break;
	  case 1: kgWrite(I2,"Please Wait -");break;
	  case 2: kgWrite(I2,"Please Wait \\");break;
  }
#else
  if(rval > 0) kgWrite(I2,buff);
#endif
 }
 if(rval==0) kgSetExit(Tmp);
#endif

  return ret;
}
