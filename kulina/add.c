int RipToMp4(char *command,char *OutFile) {
   int pid,status;
   if ((pid=fork())==0) {
    char Fifo[100];
    sprintf(Fifo,"/tmp/Fifo%-d",getpid());
    mkfifo(Fifo,0600);
    if ((pid=fork())==0) {
     strcat(command,"-dumpdtream -dumpfile Fifo ");
     runmplayer(buff,NULL);
     exit(0);
    }
    else {
      int buff[500];
      sprintf(buff,"kgffmpeg -f mp4 -vcodec libx264 -b:v 3000K -aq 0 "
       " -c:a libmp3lame -y -i Fifo \"%-s\"",OutFile);
//      runfunction(command,ProcessMp4Conversion,kgffmpeg);
      runfunction(command,NULL,kgffmpeg);
      remove(Fifo);
      waitpid(pid,&status,0);
      exit(0);
    }
  }
  else {
     waitpid(pid,&status,0);
  }

}
