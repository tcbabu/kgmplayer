#include <kulina.h>
ThumbNail **RepositionThumbNail(ThumbNail **,int,int);
int ClearThumbNails(ThumbNail **);
int  ReorderImagessplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  void ** tmp;
  int j=0,k=0;
  char **chpt=NULL;
  ThumbNail **thpt;
  ThumbNail *th;
  DIY *Y;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  tmp = (void **)D->pt;
  Y = (DIY *)kgGetNamedWidget(D,(char *)"ImageBox");
  thpt = (ThumbNail**)kgGetList(Y);
  switch(butno) {
    case 1: 
      n=0;
      while(thpt[n] != NULL) n++;
      j=0;
      while(thpt[j]!=NULL) {
	      if( thpt[j]->sw ==1) {
		      th = thpt[j];
		      th->sw = 0;
		      k=j+1;
		      while(k<n) {thpt[k-1]=thpt[k];k++;}
		      thpt[n-1]= th;
		      continue;
	      }
	      j++;
      }
      ret=0;
      ClearThumbNails(thpt);
      kgUpdateWidget(Y);
      kgUpdateOn(D);
      break;
    case 2: 
      ClearThumbNails(thpt);
      kgUpdateWidget(Y);
      kgUpdateOn(D);
      ret=0;
      break;
    case 3: 
      n=0;
      while(thpt[n] != NULL) n++;
      j=n-1;
      while(j>= 0) {
              if( thpt[j]->sw ==1) {
		      ThumbNail *thtmp[2];
		      th = thpt[j];
                      k=j+1;
                      while(k<=n) {thpt[k-1]=thpt[k];k++;}
		      thtmp[0]=th;
		      thtmp[1]=NULL;
//		      kgFreeThumbNails(thtmp);
              }
              j--;
      }

      ret=0;
      ClearThumbNails(thpt);
      kgUpdateWidget(Y);
      kgUpdateOn(D);
      break;
    case 4: 
      j=0;while(thpt[j]!= NULL) j++;
      chpt = (char **)malloc(sizeof(char *)*(j+1));
      j=0;
      while(thpt[j]!= NULL){
	      chpt[j]=(char *)malloc(sizeof(char *)*(strlen(thpt[j]->name)+1));
	      strcpy(chpt[j],thpt[j]->name);
	      j++;
      }
      chpt[j]=NULL;
      kgFreeThumbNails(thpt);
      kgSetList(Y,NULL);
      tmp[0]=chpt;
      break;
  }
  return ret;
}
void  ReorderImagessplbutton1init(DIL *B,void *pt) {
}
int  ReorderImagesbrowser1callback(int item,int i,void *Tmp) {
  DIALOG *D;DIY *Y;void *pt; 
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret=1; 
  D = (DIALOG *)Tmp;
  ThumbNail **thy;
  int pos,x,y;
  pt = D->pt;
  Y = (DIY *)kgGetWidget(Tmp,i);
  thy = (ThumbNail **) kgGetList(Y);
  if(kgDragThumbNail(Y,item-1,&x,&y)) {
          pos = kgGetThumbNailItem(Y,x,y);
	  if(RepositionThumbNail(thy,pos,item-1)) {
            *(Y->df) = pos+1;
            kgSetList((void *)Y,(void **)thy);
            ClearThumbNails(thy);
            kgUpdateWidget(Y);
            kgUpdateOn(D);
          }

  }
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  ReorderImagesbrowser1init(DIY *Y,void *pt) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
    ThumbNail **th;
    int i=0;
    void **tmp;
    tmp = (void **) pt;
    th = kgStringToThumbNails((char **)tmp[0]);
    while(th[i] != NULL) {
	    th[i]->img= kgThumbNailImage(kgGetImage(th[i]->name),248,248);
	    i++;
    }
    kgSetList((void *)Y,(void **) th);
}
int ReorderImagesinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int ReorderImagescleanup(void *Tmp) {
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
int ModifyReorderImages(void *Tmp,int GrpId) {
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

int ReorderImagesCallBack(void *Tmp,void *tmp) {
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
int ReorderImagesResizeCallBack(void *Tmp) {
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
int ReorderImagesWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
