int ChangeVolume(char *Infile ,char *Outfile,double EnhFac {
  int pid,status;
  if(!CheckMedia(Iinfile)){
       return 0;
  }
  strcpy(GrabFileName,cn->outfile);
  fflush(stdout);
  fflush(stderr);
  if ((pid=fork())==0) {
    char command[500],Str1[500],Str2[500],options[500],Qstr[500];
     options[0] ='\0';
     Qstr[0]='\0';
     sprintf(Str1,"Quality as original\n");
     if((kgSearchString(Outfile,(char *)".mp3")>= 0)||
          (kgSearchString(Outfile,(char *)".MP3")>= 0)) {
          strcat(Qstr," -aq 0 ");
          sprintf(Str1,"Quality Very Good\n");
     }
     if((kgSearchString(Cn.outfile,(char *)".aac")>= 0)) {
       sprintf(Qstr," -c:a libfdk_aac ");
     }
     sprintf(Str2,"Volume as original\n");
     if(fabs(EnhFac-1.0)> 0.1) {
          sprintf(Str2,"  -af \"pan=stereo|c0=%-lf*c0|c1=%-lf*c1\" %s"
            ,EnhFac,EnhFac,Qstr);
          strcpy(Qstr,Str2);
          sprintf(Str2,"Volume left: %6.2lf right: %6.2lf\n",
            EnhFac,EnhFac);
     }
      if(pipe(Jpipe) < 0) exit(0);
      if(pipe(Jstat) < 0) exit(0);
      MonPipe = Jpipe[0];
      fflush(stdout);
      fflush(stderr);
      if( fork()==0) {
        MonPipe = Jpipe[0];
        close(Jpipe[1]);
        close(Jstat[0]);
        RunMonitorJoin(NULL);
	printf("Exit RunMonitorJoin\n");
        exit(0);
      }
      close(Jpipe[0]);
      close(Jstat[1]);
         write(Jpipe[1],options,strlen(options));
         sprintf(command,"ffmpegfun -nostdin -i \"%-s\" -vn   -ac 2 %s -y \"%-s\"",
             Infile,Qstr,Outfile);
      printf("%s\n",command);
      fflush(stdout);
      write(Jpipe[1],Str1,strlen(Str1));
      write(Jpipe[1],Str2,strlen(Str2));
      sprintf(Str1,"!c01Output format is selected as per file extension\n");
      write(Jpipe[1],Str1,strlen(Str1));
      sprintf(Str1,"!c01If CUT and JOIN is planned !z54.wav/.flac!z45 is better\n");
      write(Jpipe[1],Str1,strlen(Str1));
#if 1
      runfunction(command, ProcessToPipe ,ffmpegfun);
#else
      runfunction(command, ProcessPrint ,ffmpegfun);
#endif
    exit(0);
  }
  else {
     waitpid(pid,&status,0);
  }
}
