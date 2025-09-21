#include <fcntl.h>
#include <kulina.h>
#include "StockImages.c"

static char Folder[500],Home[500],Filter[200]="*";
static void *InfoBox,*ImgBox,*FolderBox,*OutBox;
static void *MusicImg=NULL,*UnknownImg=NULL,*VideoImg=NULL;
static char **Args=NULL;
static DIALOG *Dia;
int DeleteThumbNails(ThumbNail **th) {
    int i=0,n=0,j=0;
    if(th!= NULL) {
     i=0; n=0;
     while(th[i++]!=NULL);
     n=i-1;
     i=n-1;
     while(i>=0) {
       if (th[i]->sw ==1) {
         free(th[i]->name);
         kgFreeImage(th[i]->img);
         j=i;
         while(j<n){th[j]=th[j+1];j++;}
       }
       i--;
     }
    }
    return 1;
}
int SetThumbNails(ThumbNail **th) {
    int i=0,n=0,j=0;
    if(th!= NULL) {
     i=0; n=0;
     while(th[i]!=NULL) th[i++]->sw = 1;
    }
    return 1;
}
int ClearThumbNails(ThumbNail **th) {
    int i=0,n=0,j=0;
    if(th!= NULL) {
     i=0; n=0;
     while(th[i]!=NULL) th[i++]->sw = 0;
    }
    return 1;
}
static int GetLine(int pip0,char *buff){
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
       tv.tv_sec = 30;
       tv.tv_usec =0;
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
static int ProcessFileCommand(int pip0,int pip1,int Pid) {
     char buff[1000];
     int ch,i=0,j,ret=0;
     while((ch=GetLine(pip0,buff)) ) {
         if(ch< 0) continue;
//         printf("%s\n",buff);
         buff[14]='\0';
         if(strcmp(buff+4,"image data")==0 ) {ret=3;break;}
         buff[9]='\0';
         if(strcmp(buff,"ISO Media")==0 ) {ret=1;break;}
         buff[5]='\0';
         if(strcmp(buff,"Audio")==0 ) {ret=2;break;}
     }
     return ret;
}
int CheckFileType(char *filename){
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2];
   char *args[100],buff[1000],pt[300];
   char *pgrpath=NULL;
   int i=0,pos=0;
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
//   while(job[i]==' ') i++;
//   strcpy(buff,job+i);
   sprintf(buff,"file -b %-s", filename);
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
   }
   args[i]=NULL;
   if(i==0) return 0;
   pgrpath=kgWhich(args[0]);
   if (pgrpath==NULL) return 0;
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */
     close(0);
     dup(pip2[0]);
     close(pip2[0]);
     close(1);
     dup(pip[1]);
     close(2);
     open("/dev/null",O_WRONLY|O_CREAT,0777);
     /*dup(pip[1]);*/
     close(pip[1]);
     execv(pgrpath,args);
     fprintf(stderr,"Failed to execute \n");
     sleep(5);
     exit(1);
   }
   else {   /* parent process */
//     printf("Waiting pid: %d\n",pid);
     int ret =1;
     close(pip2[0]);
     close(pip[1]);
     ret = ProcessFileCommand(pip[0],pip2[1],pid);
     waitpid(pid,&status,0);
     free(pgrpath);
     return ret;
   }
}
static int SetImg(ThumbNail *th) {
 char name[500];
 sprintf(name,"%-s/%-s",Folder,th->name);
 switch(CheckFileType(name)) {
       case 1:
         th->img= kgCopyImage(VideoImg);
         break;
       case 2:
         th->img= kgCopyImage(MusicImg);
         break;
        case 3:
         th->img= kgCopyImage(UnknownImg);
         break;
        default:
         th->img= kgCopyImage(UnknownImg);
         break;
  }
  return 1;
}
static char **ExtractNames(ThumbNail **th) {
     char **List=NULL;
     int i=0,fl=0;
//     fl = strlen(Folder)+1;
     fl=0;
     if(th != NULL) {
      while(th[i++] != NULL);
      if (i>1) {
        List = (char **) malloc(sizeof(char *)*i);
        i=0;
        while(th[i] != NULL){
          List[i] = (char *)malloc((strlen(th[i]->name)+1+fl));
//        strcpy(List[i],Folder);
//        strcat(List[i],"/");
//        strcat(List[i],th[i]->name);
          strcpy(List[i],th[i]->name);
          i++;
        }
        List[i]=NULL;
      }
     }
     return List;
}

ThumbNail * CopyThumbNail(ThumbNail *th) {
  ThumbNail *tpt=NULL;
  int fl=0;
  fl = strlen(Folder)+1;
  tpt = (ThumbNail *) malloc(sizeof(ThumbNail ));
  tpt->name = (char *) malloc(strlen(th->name)+1+fl);
  strcpy(tpt->name,Folder);
  strcat(tpt->name,"/");
  strcat(tpt->name,th->name);
  tpt->sw =0;
  tpt->img = kgCopyImage(th->img);
  tpt->state=1;
  tpt->id=1;
  return tpt;
}
int RepositionThumbNail(ThumbNail **thy,int pos ,int item) {
	int ret=0,k;
	ThumbNail *tpt;
	  if((pos >=0)&&(pos!=(item)) ) {
	    tpt = thy[item];
            if(pos < (item) ) {
              for(k=(item);k>pos;k--){ thy[k]=thy[k-1]; }
              thy[pos]= tpt;
            }
            else {
              for(k=item+1;k<pos;k++) {thy[k-1]=thy[k];}
              thy[pos-1]= tpt;
            }
	    ret = 1;
	  }
	return ret;
}
ThumbNail **AppendThumbNails(ThumbNail **th,ThumbNail **thadd) {
	int i,j,nitems,nadd;
	ThumbNail **thtmp=NULL;
	nitems = 0;
	if(th != NULL) while(th[nitems] != NULL) nitems++;
	nadd = 0;
	if(thadd != NULL) while(thadd[nadd] != NULL) nadd++;
	thtmp =  (ThumbNail **) malloc(sizeof(ThumbNail *)*(nitems+nadd+1));
	for(i=0;i<nitems;i++) thtmp[i] =th[i];
	for(i=0;i<nadd;i++) thtmp[i+nitems] =thadd[i];
	thtmp[nitems+nadd]=NULL;
	return thtmp;
}
ThumbNail **InsertThumbNail(ThumbNail **th,int pos,ThumbNail *thadd) {
	int i,j,nitems,nadd;
	ThumbNail **thtmp;
	nitems = 0;
	if(th != NULL) while(th[nitems] != NULL) nitems++;
	thtmp =  (ThumbNail **) malloc(sizeof(ThumbNail *)*(nitems+2));
	for(i=0;i<pos;i++) thtmp[i] =th[i];
	thtmp[pos] = thadd;
	for(i=pos;i<nitems;i++) thtmp[i+1] =th[i];
	thtmp[nitems+1]=NULL;
	return thtmp;
}
static int MakeThumbNailImages(ThumbNail **th) {
  ThumbNail **thtmp=NULL;
  int i=0,j=0;
  void *id;
  id = kgOpenBusy(Dia,600,150);
  thtmp = kgMakeThumbNails(Folder,84);
  kgCloseBusy(id);
  i=0; j=0;
  if(thtmp!= NULL) {
   while(th[i]!= NULL) {
    if((thtmp[j]!=NULL)&&(strcmp(thtmp[j]->name,th[i]->name)==0)) {
       {th[i++]->img = thtmp[j]->img; free(thtmp[j]->name);free(thtmp[j]);j++;}
    }
//    else th[i++]->img= kgCopyImage(UnknownImg);
    else SetImg(th[i++]);
   }
   free(thtmp);
  }
  else {
   while(th[i]!= NULL) {
     th[i++]->img= kgCopyImage(UnknownImg);
   }
  }
  return 1;
}
static int MakeUpdate(void) {
  int i=0,j=0;
  void **pt;
  ThumbNail **th=NULL,**thtmp=NULL;
  pt = (void **)kgGetList(FolderBox);
  if(pt!= NULL ) kgFreeDouble(pt);
  pt = (void **)kgGetList(ImgBox);
  if(pt!= NULL ) kgFreeThumbNails((ThumbNail **)pt);
  th = kgFileThumbNails(Folder,Filter);
  MakeThumbNailImages(th);
  kgSetList(FolderBox,(void **)kgFolderMenu(Folder));

//  kgSetList(ImgBox,kgFileThumbNails(Folder,Filter));
  kgSetList(ImgBox,(void **)th);
  kgWrite(InfoBox,Folder);
  kgUpdateWidget(FolderBox);
  kgUpdateWidget(ImgBox);
  return 1;
}

int  kgGetFilesbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,k; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      k= strlen(Folder);
      while((Folder[k]!='/')&&(k>1)) {Folder[k]='\0';k--;}
      Folder[k]='\0';
      MakeUpdate();

      break;
    case 2: 
      strcpy(Folder,Home);
      MakeUpdate();
      break;
  }
  return ret;
}
void  kgGetFilesbutton1init(DIN *B,void *pt) {
}
int  kgGetFilessplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1,nc=0,no=0,j,k; 
  char **List;
  void **arg;
  ThumbNail **th,**tho,**thn,*tpt;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  switch(butno) {
    case 1: 
      th= (ThumbNail **)kgGetList(ImgBox);
      nc=0;
      if(th!= NULL) {
        int i=0;
        while(th[i]!= NULL) {
         if(th[i++]->sw == 1) nc++;
        }
      }
      if(nc == 0) {ret = 0;break;}
      tho = (ThumbNail **) kgGetList(OutBox);
#if 0
      no=0;
      if(tho != NULL) {
        int i=0;
        while(tho[i]!= NULL) {i++; }
        no =i;
      }
      thn= (ThumbNail **) malloc(sizeof(ThumbNail *)*(no+nc+1));
      j=0;
      while(j<no) { thn[j] = tho[j]; j++;}
      k=0;
      while(j <(no+nc)) {
        while(th[k]->sw != 1) k++;
        thn[j++] = CopyThumbNail(th[k]);
        k++;
      }
      thn[j]=NULL;
#else
      ThumbNail **thtmp;
      thn = (ThumbNail **) malloc(sizeof(ThumbNail *)*(nc+1));
      thn[nc]=NULL;
      j=0; k=0;
      while(th[k]!= NULL) {
	if(th[k]->sw == 1) thn[j++]= CopyThumbNail(th[k]);
	k++;
      }
      thn[j] = NULL;
      thtmp = AppendThumbNails(tho,thn);
      if(thn != NULL) free(thn);
      thn = thtmp;
#endif
      ClearThumbNails(th);
      if(tho != NULL) free(tho);
      kgSetList(OutBox,(void **) thn);
      kgUpdateWidget(OutBox);
      kgUpdateWidget(ImgBox);
      kgUpdateOn(D);
      ret=0;
      break;
    case 2: 
      th= (ThumbNail **)kgGetList(ImgBox);
      if(th!= NULL) {
        int i=0;
        while(th[i]!= NULL) th[i++]->sw = 1;
      }
      kgUpdateWidget(ImgBox);
      kgUpdateOn(D);
      ret=0;
      break;
    case 3: 
      th= (ThumbNail **)kgGetList(ImgBox);
      if(th!= NULL) {
        int i=0;
        while(th[i]!= NULL) th[i++]->sw = 0;
      }
      kgUpdateWidget(ImgBox);
      kgUpdateOn(D);
      ret=0;
      break;
    case 4: 
      List =  ExtractNames((ThumbNail **)kgGetList(OutBox));
#if 0
      if(List != NULL) {
        int i=0;
        while(List[i]!= NULL) printf("%s\n",List[i++]);
      }
#endif
      if(Args != NULL) {
         Args[1] = (char *) List;
      }
      break;
  }
  return ret;
}
void  kgGetFilessplbutton1init(DIL *B,void *pt) {
}
int  kgGetFilesbrowser1callback(int item,int i,void *Tmp) {
  DIALOG *D;DIY *Y,*X;void *pt; 
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  Y = (DIY *)kgGetWidget(Tmp,i);
  X = (DIY *)kgGetNamedWidget(Tmp,(char *)"kgGetFilesItems");
  char newdir[300];
  char **m,**dir,**file,*folder;
  ThumbNail *tpt,**thy,**thx,**thtmp;
  void *ph;
  int pos,x,y,k,nitems=0;
  int Xmin,Ymin,Xmax,Ymax;
  Xmin= X->x1; Ymin = X->y1;
  Xmax= X->x2; Ymax = X->y2;
  pt = (void **)(((DIALOG *)Tmp)->pt);
  
  thy = (ThumbNail **)kgGetList(Y);
  thx = (ThumbNail **)kgGetList(X);
  nitems =0;
  if(thx != NULL) {
     k=0; while(thx[k]!=NULL) k++;
     nitems =k;
  }

#if 1
  if(kgDragThumbNail(Y,item-1,&x,&y)) {
	  if( (x>Xmin) && (x<Xmax)&&(y>Ymin)&&(y<Ymax)) {
#if 0
		  thtmp = (ThumbNail **) malloc(sizeof(ThumbNail *)*(nitems+2));
		  for(k=0;k<nitems;k++) thtmp[k] = thx[k];
		  thtmp[nitems]= CopyThumbNail(thy[item-1]);
		  thtmp[nitems +1]= NULL;
                  kgSetList(X,(void **) thtmp);
                  ClearThumbNails(thtmp);
                  ClearThumbNails(thy);
                  if(thx != NULL) free(thx);
                  kgUpdateWidget(X);
                  kgUpdateWidget(Y);
                  kgUpdateOn(D);
                  ret=0;
#else
		  ThumbNail *thadd[2];
		  thadd[0] = CopyThumbNail(thy[item-1]);
		  thadd[1] = NULL;
	          pos = kgGetThumbNailItem(X,x,y);
	          if((pos >=0) ) {
	            thtmp =  InsertThumbNail(thx,pos,thadd[0]);
		  }
		  else {
		    thtmp = AppendThumbNails(thx,thadd);
		  }
		  kgSetList(X,(void **) thtmp);
                  if(thx != NULL) free(thx);
                  ClearThumbNails(thtmp);
                  ClearThumbNails(thy);
                  kgUpdateWidget(X);
                  kgUpdateWidget(Y);
                  kgUpdateOn(D);
                  ret=0;
#endif

		  
	  }
   }

#endif


  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  kgGetFilesbrowser1init(DIY *Y,void *pt) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
}
int  kgGetFilesbrowser2callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIE *E;void *pt; 
  int ret=1; 
  char **List;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  E = (DIE *)kgGetWidget(Tmp,i);
  List = (char **)kgGetList(E);
  strcat(Folder,"/");
  strcat(Folder,List[item-1]);
  MakeUpdate();
  kgUpdateOn(D);

  
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  kgGetFilesbrowser2init(DIE *E,void *pt) {
}
int  kgGetFilesbrowser3callback(int item,int i,void *Tmp) {
  DIALOG *D;DIY *Y;void *pt; 
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret=1,pos,nitems=0,k; 
  int x,y;
  ThumbNail **thy,*tpt,**thtmp;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  Y = (DIY *)kgGetWidget(Tmp,i);
  thy = (ThumbNail **)kgGetList(Y);
  nitems =0;
  if(thy != NULL) {
     k=0; while(thy[k]!=NULL) k++;
     nitems =k;
  }
  if(kgDragThumbNail(Y,item-1,&x,&y)) {
	  pos = kgGetThumbNailItem(Y,x,y);
#if 0
	  if((pos >=0)&&(pos!=(item-1)) ) {
	    tpt = thy[item-1];
            if(pos < (item-1) ) {
              for(k=(item-1);k>pos;k--){ thy[k]=thy[k-1]; }
              thy[pos]= tpt;
            }
            else {
              for(k=item;k<pos;k++) {thy[k-1]=thy[k];}
              thy[pos-1]= tpt;
            }
            *(Y->df) = pos+1;
            kgSetList((void *)Y,(void **)thy);
            kgUpdateWidget(Y);
            kgUpdateOn(D);
	  }
#else
	  if(RepositionThumbNail(thy,pos,item-1)) {
            *(Y->df) = pos+1;
            kgSetList((void *)Y,(void **)thy);
            ClearThumbNails(thy);
            kgUpdateWidget(Y);
            kgUpdateOn(D);
	  }
#endif
            ret=0;
   }

  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  kgGetFilesbrowser3init(DIY *Y,void *pt) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
}
int  kgGetFilesbutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  ThumbNail **th;
  DIY *Y;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      th = (ThumbNail **) kgGetList(OutBox);
      SetThumbNails(th);
      kgSetList(OutBox,(void **)th);
      kgUpdateWidget(OutBox);
      kgUpdateOn(D);
      break;
    case 2: 
      th = (ThumbNail **) kgGetList(OutBox);
      ClearThumbNails(th);
      kgSetList(OutBox,(void **)th);
      kgUpdateWidget(OutBox);
      kgUpdateOn(D);
      break;
    case 3: 
      th = (ThumbNail **) kgGetList(OutBox);
      DeleteThumbNails(th);
      kgSetList(OutBox,(void **)th);
      Y = (DIY *)OutBox;
      kgUpdateWidget(OutBox);
      kgUpdateOn(D);
      break;
  }
  return ret;
}
void  kgGetFilesbutton2init(DIN *B,void *pt) {
}
int kgGetFilesinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1,i;
  ThumbNail **th;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  Dia = D;
  if(pt != NULL) {
   Args =(char **) D->pt;
   strcpy(Filter,(char *)Args[0]);
  }
  UnknownImg = kgGetInlineImage((char *)question_str.image_data,question_str.size);
  MusicImg = kgGetInlineImage((char *)audio_mpeg_str.image_data,audio_mpeg_str.size);
  VideoImg = kgGetInlineImage((char *)video_mpeg_str.image_data,video_mpeg_str.size);
  InfoBox= kgGetNamedWidget(D,(char *)"kgGetFilesInfo");
  ImgBox= kgGetNamedWidget(D,(char *)"kgGetFilesImages");
  OutBox= kgGetNamedWidget(D,(char *)"kgGetFilesItems");
  FolderBox= kgGetNamedWidget(D,(char *)"kgGetFilesFolder");
  sprintf(Folder,"%-s",getenv("HOME"));
  strcpy(Home,Folder);
  kgSetList(FolderBox,(void **)kgFolderMenu(Folder));
  th = (ThumbNail **)malloc(sizeof(ThumbNail *));
  th[0]=NULL;
  kgSetList(OutBox,(void **)th);
  th = kgFileThumbNails(Folder,Filter);
  MakeThumbNailImages(th);
  kgSetList(ImgBox,(void **)th);
  kgWrite(InfoBox,Folder);
  kgUpdateWidget(FolderBox);
  kgUpdateWidget(ImgBox);
  kgUpdateOn(D);
  
  return ret;
}
int kgGetFilescleanup(void *Tmp) {
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
int kgGetFilesCallBack(void *Tmp,void *tmp) {
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
int kgGetFilesResizeCallBack(void *Tmp) {
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
int kgGetFilesWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
