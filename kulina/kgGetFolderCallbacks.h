int  kgGetFoldertextbox1callback(int cellno,int i,void *Tmp);
int  kgGetFolderbutton1callback(int butno,int i,void *Tmp) ;
void  kgGetFolderbutton1init(DIN *B,void *pt) ;
int  kgGetFolderbutton2callback(int butno,int i,void *Tmp) ;
void  kgGetFolderbutton2init(DIN *B,void *pt) ;
int  kgGetFolderbrowser1callback(int item,int i,void *Tmp) ;
void  kgGetFolderbrowser1init(DIX *X,void *pt) ;
int  kgGetFoldersplbutton1callback(int butno,int i,void *Tmp) ;
void  kgGetFoldersplbutton1init(DIL *B,void *pt) ;
void  kgGetFolderbrowser2init(DIS *S,void *pt) ;
int kgGetFolderinit(void *Tmp) ;
int kgGetFoldercleanup(void *Tmp) ;
int kgGetFolderCallBack(void *Tmp,void *tmp) ;
int kgGetFolderResizeCallBack(void *Tmp) ;
int kgGetFolderWaitCallBack(void *Tmp) ;