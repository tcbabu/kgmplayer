
extern int Jpipe[2];
extern int Jstat[2];
extern int MonPipe;

int ProcessOutput(int pip0,int pip1,int Pid) {
     char buff[1000],work[1000];
     int ret =0;
     int ch,i=0,j,pos,len,OK=0,line=0;
     float per=0.0,csec,m,h,s,totsec;
     char *pt;
     while((ch=GetLine(pip0,buff)) ) {
         line++;
         if(ch< 0) continue;
         if(SearchString(buff,(char *)"frame=")>=0)  {
             pos = SearchString(buff,(char *)"time=");
             if(pos>=0) {
               pt = buff+pos+5;
               per = GetTimeval(pt);
               sprintf(work,"Cur: %f \n",per);
               write(Jpipe[1],work,strlen(work));
               sprintf(work," %f %s",per,pt);
               write(Jpipe[1],work,strlen(work));
//      printf("%s",work);
               continue;
             }
         }
         if(SearchString(buff,(char *)"size=")>=0)  {
             pos = SearchString(buff,(char *)"time=");
             if(pos>=0) {
               pt = buff+pos+5;
               s = GetTimeval(pt);
               per = s;
               sprintf(work,"Cur: %f \n",per);
               write(Jpipe[1],work,strlen(work));
               continue;
             }
         }
     }
     printf("Got %d \n",line);
     printf("Exiting ProcessTo Pipe\n");
     fflush(stdout);
     return ret;
}
int RunAndMponitor(char * job) ) {

  int pid=0,id;
  int status;
  if(fork() != 0)return 1;
  id = getpid();
  if(pipe(Jpipe) < 0) exit(0);
  if(pipe(Jstat) < 0) exit(0);
  MonPipe = Jpipe[0];
  
  if ((pid=fork())==0) {
    fflush(stdout);
    fflush(stderr);
    close(Jpipe[0]);
    close(Jstat[1]);
     sprintf(buff,"Executing... PLEASE WAIT\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"PLEASE WAIT till the window closes\n");
     write(Jpipe[1],buff,strlen(buff));
     sprintf(buff,"You can cancel job if you wish\n");
     write(Jpipe[1],buff,strlen(buff));
     ssec=0;
     n=1;
     runfunction(job,ProcessOutput,ffmpegfun);
     close(Jpipe[1]);
     close(Jstat[0]);
     exit(0);
  }  //fork
  else {
     close(Jpipe[1]);
     close(Jstat[0]);
     RunMonitorJoin(NULL);
     kill(pid,9);
     waitpid(pid,&status,0);
     exit(0);
  }
}
