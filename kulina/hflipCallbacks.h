int  hfliptextbox1callback(int cellno,int i,void *Tmp);
int  hfliptextbox2callback(int cellno,int i,void *Tmp);
int  hflipbutton1callback(int butno,int i,void *Tmp) ;
void  hflipbutton1init(DIN *B,void *pt) ;
int  hflipsplbutton1callback(int butno,int i,void *Tmp) ;
void  hflipsplbutton1init(DIL *B,void *pt) ;
int hflipinit(void *Tmp) ;
int hflipcleanup(void *Tmp) ;
int hflipCallBack(void *Tmp,void *tmp) ;
int hflipResizeCallBack(void *Tmp) ;
int hflipWaitCallBack(void *Tmp) ;