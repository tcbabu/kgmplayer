int  MakeKaraoketextbox1callback(int cellno,int i,void *Tmp);
int  MakeKaraokebutton1callback(int butno,int i,void *Tmp) ;
void  MakeKaraokebutton1init(DIN *B,void *pt) ;
int  MakeKaraoketextbox2callback(int cellno,int i,void *Tmp);
int  MakeKaraokebrowser1callback(int item,int i,void *Tmp) ;
void  MakeKaraokebrowser1init(DIRA *R,void *pt) ;
int  MakeKaraokesplbutton1callback(int butno,int i,void *Tmp) ;
void  MakeKaraokesplbutton1init(DIL *B,void *pt) ;
int MakeKaraokeinit(void *Tmp) ;
int MakeKaraokecleanup(void *Tmp) ;
int MakeKaraokeCallBack(void *Tmp,void *tmp) ;
int MakeKaraokeResizeCallBack(void *Tmp) ;
int MakeKaraokeWaitCallBack(void *Tmp) ;
