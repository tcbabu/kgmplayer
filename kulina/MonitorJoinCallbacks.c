#include <kulina.h>
extern int Tools;
extern float TSec,CSec;
extern double Ssec,Esec;
extern char GrabFileName[300];
extern int MonPipe;
extern int ToGrab[2],FromGrab[2],StatusGrab[2];
int GetTimedLine(int pip0,char *buff,int usec);
int GetLine(int pip0,char *buff);
int Oper;
static DII * Info;
int  MonitorJoinsplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  MonitorJoinsplbutton1init(DIL *B,void *pt) {
}
int MonitorJoininit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  Info = (DII *) kgGetNamedWidget(Tmp,"VJInfo");
  Oper=-1;
  return ret;
}
int MonitorJoincleanup(void *Tmp) {
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
int MonitorJoinCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
#if 1
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
#endif
  return ret;
}
int MonitorJoinResizeCallBack(void *Tmp) {
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
int WriteInfo(char *buff){
//   printf("%s\n",buff);
   kgWrite(Info,buff);
   kgUpdateOn(Info->D);
}
int MonitorJoinWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 

  int ret = 0;
  char buff[1000],connection[500];
  char tmpbuff[500];
     int ch,i=0,j,pos,pip0;
     float h,m,s;
     float per=0.0,fac=0.0;
     char *pt;
     DIO *O;

//     close(pip1);
     O = (DIO *)kgGetNamedWidget(Tmp,"JoinBar");
     pip0 = MonPipe;
//     sprintf(buff,"Tool : %d\n",Tools);
//     WriteInfo(buff);
     if((ch=GetTimedLine(pip0,buff,1000)) ) {
#if 0
         if(!GetTimedLine(StatusGrab[0],connection,300)) kgSetExit(Tmp);
#endif
         if(ch>= 0)  {
           if(ch==0) {kgSetExit(Tmp); return 1;}
           switch(Tools) {
             case 3:
             case 5:
             case 6:
#if 1
               pos = kgSearchString(buff,"Esec:");
               if(pos>=0) {
                 sscanf(buff+pos+5,"%lf",&Esec);
//                 printf("Esec : %lf\n",Esec);
                 break;
               }
#endif
               pos = kgSearchString(buff,"Cur:");
               if(pos>=0) {
                 sscanf(buff+pos+4,"%f",&fac);
                 if(Esec > 0.0) per = fac/Esec*100.0;
                 else per = fac;
                 if( (int)per !=Oper) {
                   Oper = (int)per;
                   kgSetProgressBar(O,(int)per);
                 }
                 break;
               }
               pos = kgSearchString(buff,"Per:");
               if(pos>=0) {
                 sscanf(buff+pos+4,"%f",&per);
                 if( (int)per !=Oper) {
                   Oper = (int)per;
                   kgSetProgressBar(O,(int)per);
                 }
                 break;
               }
               WriteInfo(buff);
               break;
             case 4:
#if 0
               pos = kgSearchString(buff,"Esec:");
               if(pos>=0) {
                 sscanf(buff+pos+5,"%lf",&Esec);
//                 printf("Esec : %lf\n",Esec);
                 break;
               }
#endif
               pos = kgSearchString(buff,"Per:");
               if(pos>=0) {
                 sscanf(buff+pos+4,"%f",&per);
                 if( (int)per !=Oper) {
                   Oper = (int)per;
                   kgSetProgressBar(O,(int)per);
                 }
                 break;
               }
               pos = kgSearchString(buff,"Cur:");
               if(pos>=0) {
                 sscanf(buff+pos+4,"%f",&fac);
                 if(Esec > 0.0) per = fac/Esec*100.0;
                 else per = fac;
                 if( (int)per !=Oper) {
                   Oper = (int)per;
                   kgSetProgressBar(O,(int)per);
                 }
                 break;
               }
               WriteInfo(buff);
               break;
             default:
#if 1
               pos = kgSearchString(buff,"Esec:");
               if(pos>=0) {
                 sscanf(buff+pos+5,"%lf",&Esec);
//                 printf("Esec : %lf\n",Esec);
                 sprintf(tmpbuff,"End Time : %lf\n",Esec);
                 WriteInfo(tmpbuff);
                 break;
               }
#endif
               pos = kgSearchString(buff,"Cur:");
               if(pos>=0) {
                 sscanf(buff+pos+4,"%f",&fac);
                 if(Esec > 0.0) per = fac/Esec*100.0;
                 else per = fac;
                 if( (int)per !=Oper) {
                   Oper = (int)per;
                   kgSetProgressBar(O,(int)per);
                 }
                 break;
               }
               pos = kgSearchString(buff,"Per:");
               if(pos>=0) {
                 sscanf(buff+pos+4,"%f",&per);
                 if( (int)per !=Oper) {
                   Oper = (int)per;
                   kgSetProgressBar(O,(int)per);
                 }
                 break;
               }
               WriteInfo(buff);
               break;
           }
         }
      }
      if(ch==0) {kgSetExit(Tmp); return 1;}

  return ret;
}
