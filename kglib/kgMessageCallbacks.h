int kgMessageinit(void *Tmp) ;
int kgMessagecleanup(void *Tmp) ;
void *RunkgMessage(void *,void *) ;
int MakekgMessageGroup(DIALOG *,void *) ;
void * kgMessageAction(void *,void *) ;
int ModifykgMessage(void *Tmp,int GrpId) ;
int kgMessageCallBack(void *Tmp,void *tmp) ;
int kgMessageResizeCallBack(void *Tmp) ;
int kgMessageWaitCallBack(void *Tmp) ;
