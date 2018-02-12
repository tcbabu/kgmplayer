#include <kulina.h>
extern int Tools;
float TSec=0.,CSec=0;
extern double Ssec,Esec;
extern char GrabFileName[300];
extern int MonPipe;
extern int ToGrab[2],FromGrab[2],StatusGrab[2];
int GetTimedLine(int pip0,char *buff,int usec);
int GetLine(int pip0,char *buff);
int  monitorsplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    b utno : selected item (1 to max_item) 
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
void  monitorsplbutton1init(DIL *B,void *pt) {
}
int monitorinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  TSec=CSec=0.;
  return ret;
}
int monitorcleanup(void *Tmp) {
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
int monitorCallBack(void *Tmp,void *tmp) {
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
int monitorResizeCallBack(void *Tmp) {
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
int monitorWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
     char buff[1000],connection[500];
     int ch,i=0,j,pos,pip0;
     float h,m,s;
     float per=0.0;
     char *pt;
     DIO *O;
     
//     close(pip1);
     O = (DIO *)kgGetNamedWidget(Tmp,"Bar");
     pip0 = MonPipe;
     if((ch=GetTimedLine(pip0,buff,1000)) ) {
         if(!GetTimedLine(StatusGrab[0],connection,300)) kgSetExit(Tmp);
         if(ch>= 0)  {
           if(ch==0) {kgSetExit(Tmp); return 1;}
           switch(Tools) {
           case 0:
             if(!kgSearchString(buff,"ERROR:")) kgSetExit(Tmp);
             if(!kgSearchString(buff,"FAILED TO GET DOCUMENT REFERENCE"))
              kgSetExit(Tmp);
             if(!kgSearchString(buff,"Connection Refused")) 
              kgSetExit(Tmp);
             if((pos=kgSearchString(buff,"Progress:"))>=0){
               pos=kgSearchString(buff,":");
               pt = buff+pos+1;
               while(pt[i]!='%') i++;
               pt[i]= '\0';
               sscanf(pt,"%f",&per);
//             printf("%6.2f\n",per);
               kgSetProgressBar(O,(int)per);
               if( ((int)per)==100) {sleep(2);kgSetExit(Tmp);}
             }
           break;
           case 1:
// for Audio Convert
             if((pos=kgSearchString(buff,"T:"))>=0){
               pt = buff+2;
               sscanf(pt,"%f",&TSec);
//               if(Esec < 0) Tsec -= Esec;
               if(Esec > 0) TSec=Esec;
               Esec=TSec;
//               fprintf(stderr, " TotSec= %f\n",TSec);
             }
             if((pos=kgSearchString(buff,"A:"))>=0){
               pt = buff+2;
               sscanf(pt,"%f",&CSec);
//               fprintf(stderr, " TotSec= %f:%f\n",TSec,CSec);
               if( TSec> 0.0) {
                 per = (CSec-Ssec)/(TSec-Ssec)*100.0;
                 kgSetProgressBar(O,(int)per);
//                 if( ((int)per)==100) {sleep(2);kgSetExit(Tmp);}
               }
             }
           break;
           case 2:
// For Video Convert
             TSec = Esec-Ssec;
             if((pos=kgSearchString(buff,"time="))>=0){
               pt = buff+pos+5;
//               fprintf(stderr,"%s\n",pt);
               pos= kgSearchString(pt,":");
               pt[pos]=' ';
               pos= kgSearchString(pt,":");
               pt[pos]=' ';
               sscanf(pt,"%f%f%f",&h,&m,&s);
               s += (h*3600+m*60);
//               fprintf(stderr,"%f %f %f: %lf\n",h,m,s,TSec);
               per=0;
               if(TSec > 0.0) per = s/TSec*100;
               kgSetProgressBar(O,(int)per);
             }
           break;
           default:
           break;
         } //switch
       }  // if ch > 0
     }  // if gettimed line
     if(ch==0) {kgSetExit(Tmp); return 1;}
  return ret;
}
