#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <kulina.h>
#include "ConvertData.h"
#include "mediainfo.h"
extern MEDIAINFO Minfo;
int Tools=0;
extern int Kulina;
int WMErr;
char GrabFileName[300];
int MonPipe;
int ToGrab[2],FromGrab[2],StatusGrab[2];
int ToTools[2],FromTools[2],StatusTools[2];
int GetTimedLine(int pip0,char *buff,int usec);
int GetLine(int pip0,char *buff);
int kgLame(int,char **);
int kgffmpeg(int,char **);
int Mplayer(int,char **);
int Mencoder(int,char **);
void *RunkgMplayer(void *);
void *Runmonitor(void *arg);
int ConvertToMp3( CONVDATA *cn);
int ConvertToMp4( CONVDATA *cn);
int JoinToMp4( CONVDATA *cn);
int JoinToMp3( CONVDATA *cn);
int AddAudioToVideo( CONVDATA *cn);
int ConvertToKaraoke(CONVDATA *cn);
int MixAudioToVideo( CONVDATA *cn);
int MixAudioToAudio( CONVDATA *cn);
int InsertSilences( CONVDATA *cn);
int MakeAudioCuts( CONVDATA *cn);
int SearchString(char *s1,char *s2);

char HomeDir[200],bname[200];
int movgrab(int,char **);
int SetTheme(void);
int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int GetTimedLine(int pip0,char *buff,int usec){
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
       tv.tv_sec = 0;
       tv.tv_usec =usec;
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
int ProcessGrab(int pip0,int pip1,int Pid) {
     char buff[1000],connection[500];
     int ch,i=0,j,pos;
     float per=0.0;
     char *pt;
     
//     close(pip1);
     MonPipe=pip0;
     while((ch=GetLine(pip0,buff)) ) {
//         printf("%s\n",buff);
         if(!GetTimedLine(StatusGrab[0],connection,300)) break;
         if(ch< 0) continue;
         if(!kgSearchString(buff,(char *)"ERROR:")) break;
         if(!kgSearchString(buff,(char *)"FAILED TO GET DOCUMENT REFERENCE")) break;
         if(!kgSearchString(buff,(char *)"Connection Refused")) break;
#if 1
         if((pos=kgSearchString(buff,(char *)"Getting:"))>=0){
//           printf("Getting : calling Runmonitor\n");
//           fflush(stdout);
           Runmonitor(NULL);
           break;
         }
#else
         if((pos=kgSearchString(buff,(char *)"Progress:"))>=0){
           pos=kgSearchString(buff,(char *)":");
           pt = buff+pos+1;
           while(pt[i]!='%') i++;
           pt[i]= '\0';
           sscanf(pt,"%f",&per);
           printf("%6.2f\n",per);
         }
#endif
     }
//     printf("Function Finished\n");
     return 1;
}
int ProcessGrabCheck(int pip0,int pip1,int Pid) {
     char buff[1000],connection[500],Formats[500],Selected[500];
     int ret =0;
     int ch,i=0,j,pos,len,OK=0;
     float per=0.0;
     char *pt;
     Formats[0]='\0';
     Selected[0]='\0';
//     close(pip1);
     while((ch=GetLine(pip0,buff)) ) {
         if(!GetTimedLine(StatusGrab[0],connection,300)) break;
         if(ch< 0) continue;
         if(!kgSearchString(buff,(char *)"ERROR:")) break;
         if(!kgSearchString(buff,(char *)"FAILED TO GET DOCUMENT REFERENCE")) break;
         if(!kgSearchString(buff,(char *)"Connection Refused")) break;
         if((pos=kgSearchString(buff,(char *)"Formats available "
              "for this Movie:"))>=0){
           pos=kgSearchString(buff,(char *)":");
           strncpy(Formats,buff+pos+1,499);
         }
         if((pos=kgSearchString(buff,(char *)"Getting:"))>=0){
           pos=kgSearchString(buff,(char *)":");
           strncpy(Selected,buff+pos+1,499);
         }
         if((pos=kgSearchString(buff,(char *)"Progress:"))>=0){
           OK+=1;
           if(OK==1) { ret=1; break;}
         }
     }
     if(ret==0) {
//       printf("FAILED\n");
       strcpy(buff,(char *) "FAILED: \n");
       len = strlen(buff);
       write(FromGrab[1],buff,len);
     }
     else {
//       printf("OKAY\n");
       strcpy(buff,(char *) "OKAY: \n");
       len = strlen(buff);
       write(FromGrab[1],buff,len);
//       strcat(Formats,"\n");
       len = strlen(Formats);
       write(FromGrab[1],Formats,len);
//       printf("%s",Formats);
//       strcat(Selected,"\n");
       len = strlen(Selected);
       write(FromGrab[1],Selected,len);
//       printf("%s",Selected);
     }
//     printf("Function Finished\n");
     return ret;
}
int ForkGrab(void) {
   char buff[2000];
   int pip0,ch;
   if( pipe(ToGrab) < 0) return 0;
   if( pipe(FromGrab) < 0) return 0;
   if( pipe(StatusGrab) < 0) return 0;
   pip0 = ToGrab[0];
   if(fork() == 0) {
     close(ToGrab[1]);
     close(FromGrab[0]);
     close(StatusGrab[1]);
     while((ch=GetLine(pip0,buff)) ) {
       if(ch< 0) continue;
//       printf("buff: %s\n",buff);
       sscanf(buff+kgSearchString(buff,(char *)"-o")+2,"%s",GrabFileName);
       if(kgSearchString(buff,(char *)"chkgrab") >=0 ){
//         printf("Calling runfunction for check\n");
         runfunction(buff,ProcessGrabCheck,movgrab);
       }
       else if(kgSearchString(buff,(char *)"movgrab") >=0 ){
//         printf("Calling runfunction\n");
#if 1
         if (fork()==0) {
           runfunction(buff,ProcessGrab,movgrab);
           exit(0);
         }
#else
         runfunction(buff,ProcessGrab,movgrab);
#endif
       }
       buff[0]='\0';
       
//       system(buff);
     }
//     printf("Closed pipe\n");
     exit(0);
   }
   else return 1;
}
int ReadConvertData(CONVDATA *cn,char *buff) {
   int pos,loc;
   char *pt;
   pt = buff;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pos++; loc=pos; pt = pt+loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->infile,pt);
//           printf("%s\n",cn->infile);
   pt +=loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   pt +=loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->outfile,pt);
//           printf("%s\n",cn->outfile);
   pt +=loc;
   sscanf(pt,"%d%d%lf%d%lf%lf",
      &(cn->Quality),&(cn->VolEnh),&(cn->Enhfac),
      &(cn->FullRange),&(cn->StartSec),&(cn->EndSec));
   return 1;
}
int ReadVConvertData(CONVDATA *cn,char *buff) {
   int pos,loc;
   char *pt;
   pt = buff;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pos++; loc=pos; pt = pt+loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->infile,pt);
//           printf("%s\n",cn->infile);
   pt +=loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   pt +=loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->outfile,pt);
//           printf("%s\n",cn->outfile);
   pt +=loc;
   sscanf(pt,"%d%lf%d%d%lf%lf%d%d",
      &(cn->VQuality),&(cn->NewAsp),&(cn->Xsize),
      &(cn->VFullRange),&(cn->VStartSec),&(cn->VEndSec),
      &(cn->ChngAsp),&(cn->Scale));
   return 1;
}
int ReadVJoinData(CONVDATA *cn,char *buff) {
   int pos,loc=0,i,pip0;
   pip0 = StatusTools[0];
   Dlink *L;
   char *pt;
   MEDIAINFO *mpt;
   pt = buff;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pos++; loc=pos; pt = pt+loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->outfile,pt);
   pt +=loc;
   cn->Vlist=(void *)Dopen();
   L=(Dlink *)cn->Vlist;
   sscanf(pt,"%d%d%d%f%d%lf",
      &(cn->Xsize),&(cn->Ysize),&(cn->Fcount),&(cn->fps),&(cn->Quality),
      &(cn->EndSec));
   for(i=0;i<cn->Fcount;i++) {
     if((GetLine(pip0,buff)) ) {
       pt = buff;
       pos = SearchString(pt,(char *)"\"");
       if(pos<0) {return 0;}
       pos++; loc=pos;
       pt +=loc;
       pos = SearchString(pt,(char *)"\"");
       if(pos<0) {return 0;}
       pt[pos]='\0';
       pos++; loc=pos;
       mpt = (MEDIAINFO *)malloc(sizeof(MEDIAINFO));
       strcpy(mpt->Flname,pt);
       pt +=loc;
       sscanf(pt,"%d%f%d", &(mpt->Process),&(mpt->TotSec),&(mpt->Audio));
//       printf("%s : %d %f\n",mpt->Flname,mpt->Process,mpt->TotSec);
       Dadd(L,mpt);
     }
   }
   return 1;
}
int ReadAJoinData(CONVDATA *cn,char *buff) {
   int pos,loc=0,i,pip0;
   pip0 = StatusTools[0];
   Dlink *L;
   char *pt;
   MEDIAINFO *mpt;
   pt = buff;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pos++; loc=pos; pt = pt+loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->outfile,pt);
   pt +=loc;
   cn->Alist=(void *)Dopen();
   L=(Dlink *)cn->Alist;
   sscanf(pt,"%d%lf",
      &(cn->Fcount), &(cn->EndSec));
   for(i=0;i<cn->Fcount;i++) {
     if((GetLine(pip0,buff)) ) {
       pt = buff;
       pos = SearchString(pt,(char *)"\"");
       if(pos<0) {return 0;}
       pos++; loc=pos;
       pt +=loc;
       pos = SearchString(pt,(char *)"\"");
       if(pos<0) {return 0;}
       pt[pos]='\0';
       pos++; loc=pos;
       mpt = (MEDIAINFO *)malloc(sizeof(MEDIAINFO));
       strcpy(mpt->Flname,pt);
       pt +=loc;
       sscanf(pt,"%f", &(mpt->TotSec));
//       printf("%s : %d %f\n",mpt->Flname,mpt->Process,mpt->TotSec);
       Dadd(L,mpt);
     }
   }
   return 1;
}
int ReadSilenceData(CONVDATA *cn,char *buff) {
   int pos,loc=0,i,pip0;
   pip0 = StatusTools[0];
   Dlink *L;
   char *pt;
   char *mpt;
   pt = buff;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pos++; loc=pos; pt = pt+loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->infile,pt);
   pt +=loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pos++; loc=pos; pt = pt+loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->outfile,pt);
   pt +=loc;
   cn->Slist=(void *)Dopen();
   L=(Dlink *)cn->Slist;
   sscanf(pt,"%d%lf",
      &(cn->Fcount), &(cn->EndSec));
   for(i=0;i<cn->Fcount;i++) {
     if((GetLine(pip0,buff)) ) {
       mpt = (char *)malloc(strlen(buff)+1);
//       printf("%s : %d %f\n",mpt->Flname,mpt->Process,mpt->TotSec);
       strcpy(mpt,buff);
       Dadd(L,mpt);
     }
   }
   return 1;
}
int ReadCutsData(CONVDATA *cn,char *buff) {
   int pos,loc=0,i,pip0;
   pip0 = StatusTools[0];
   Dlink *L;
   char *pt;
   char *mpt;
   pt = buff;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pos++; loc=pos; pt = pt+loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->infile,pt);
   pt +=loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pos++; loc=pos; pt = pt+loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->outfile,pt);
   pt +=loc;
   cn->Cutlist=(void *)Dopen();
   L=(Dlink *)cn->Cutlist;
   sscanf(pt,"%d%lf",
      &(cn->Fcount), &(cn->EndSec));
   for(i=0;i<cn->Fcount;i++) {
     if((GetLine(pip0,buff)) ) {
       mpt = (char *)malloc(strlen(buff)+1);
//       printf("%s : %d %f\n",mpt->Flname,mpt->Process,mpt->TotSec);
       strcpy(mpt,buff);
       Dadd(L,mpt);
     }
   }
   return 1;
}
int ReadAddData(CONVDATA *cn,char *buff) {
   int pos,loc=0,i,pip0;
   pip0 = StatusTools[0];
   Dlink *L;
   char *pt;
   MEDIAINFO *mpt;
   pt = buff;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pos++; loc=pos; pt = pt+loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->audiofile,pt);
   pt +=loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pos++; loc=pos; pt = pt+loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->infile,pt);
   pt +=loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pos++; loc=pos; pt = pt+loc;
   pos = SearchString(pt,(char *)"\"");
   if(pos<0) {return 0;}
   pt[pos]='\0';
   pos++; loc=pos;
   strcpy(cn->outfile,pt);
   pt +=loc;
   sscanf(pt,"%d",&(cn->Quality));
//   printf ("%s\n %s\n %s\n %d\n",cn->audiofile,
//           cn->infile,cn->outfile,cn->Quality);
   return 1;
}
int ForkTools(void) {
   char buff[2000];
   int pip0,ch;
   if( pipe(ToTools) < 0) return 0;
   if( pipe(FromTools) < 0) return 0;
   if( pipe(StatusTools) < 0) return 0;
   pip0 = ToTools[0];
   if(fork() == 0) {
     CONVDATA *cn;
     int pos,loc,code;
     char *pt;
     close(ToTools[1]);
     close(FromTools[0]);
     close(StatusTools[1]);
     while((ch=GetLine(pip0,buff)) ) {
       if(ch< 0) continue;
//       printf("buff: %s\n",buff);
       sscanf(buff,"%d",&(code));
       pt = buff;
       switch(code) {
         case 1:
               Tools=1;
               if(fork()==0) {
                 cn= (CONVDATA *)malloc(sizeof(CONVDATA));
                 if(ReadConvertData(cn,buff)) {
                   ConvertToMp3(cn);
                 }
                 free(cn);
                 exit(0);
               }
           break;
           case 2:
               Tools=2;
               if(fork()==0) {
                 cn= (CONVDATA *)malloc(sizeof(CONVDATA));
                 if(ReadVConvertData(cn,buff)) {
                   ConvertToMp4(cn);
                 }
                 free(cn);
                 exit(0);
               }
           break;
           case 3:
               Tools=3;
               if(fork()==0) {
                 cn= (CONVDATA *)malloc(sizeof(CONVDATA));
                 if(ReadVJoinData(cn,buff)) {
                   JoinToMp4(cn);
                 }
                 Dempty((Dlink *)(cn->Vlist));
                 free(cn);
                 exit(0);
               }
           break;
           case 4:
               Tools=4;
               if(fork()==0) {
                 cn= (CONVDATA *)malloc(sizeof(CONVDATA));
                 if(ReadAJoinData(cn,buff)) {
                   JoinToMp3(cn);
                 }
                 Dempty((Dlink *)(cn->Alist));
                 free(cn);
                 exit(0);
               }
           break;
           case 5:
               Tools=5;
               if(fork()==0) {
                 cn= (CONVDATA *)malloc(sizeof(CONVDATA));
                 if(ReadAddData(cn,buff)) {
                   AddAudioToVideo(cn);
                 }
                 free(cn);
                 exit(0);
               }
           break;
           case 6:
               Tools=6;
               if(fork()==0) {
                 cn= (CONVDATA *)malloc(sizeof(CONVDATA));
                 if(ReadAddData(cn,buff)) {
                   MixAudioToVideo(cn);
                 }
                 free(cn);
                 exit(0);
               }
           break;
           case 7:
               Tools=7;
               if(fork()==0) {
                 cn= (CONVDATA *)malloc(sizeof(CONVDATA));
                 if(ReadAddData(cn,buff)) {
                   MixAudioToAudio(cn);
                 }
                 free(cn);
                 exit(0);
               }
           break;
           case 8:
               Tools=8;
               if(fork()==0) {
                 cn= (CONVDATA *)malloc(sizeof(CONVDATA));
                 if(ReadAddData(cn,buff)) {
                   ConvertToKaraoke(cn);
                 }
                 free(cn);
                 exit(0);
               }
           break;
           case 9:
               Tools=9;
               if(fork()==0) {
                 cn= (CONVDATA *)malloc(sizeof(CONVDATA));
                 if(ReadSilenceData(cn,buff)) {
                   InsertSilences(cn);
                 }
                 Dempty((Dlink *)(cn->Slist));
                 free(cn);
                 exit(0);
               }
           break;
           case 10:
               Tools=10;
               if(fork()==0) {
                 cn= (CONVDATA *)malloc(sizeof(CONVDATA));
                 if(ReadCutsData(cn,buff)) {
                   MakeAudioCuts(cn);
                 }
                 Dempty((Dlink *)(cn->Cutlist));
                 free(cn);
                 exit(0);
               }
           break;
         default:
           Tools=0;
           break;
       }
     }
//     printf("Closed pipe\n");
     exit(0);
   }
   else return 1;
}
int main(int argc ,char **argv) {  
  FILE *fp;
  WMErr=0;
  if(argc>1) {
    if(strcmp(argv[1],"-N")==0) WMErr=1;
    else {
      Kulina=0;
      kgExtractBaseName(argv[0],bname);
      if(strcmp("kgmencoder",bname)==0) return Mencoder(argc,argv);
      if(strcmp("kglame",bname)==0) return kgLame(argc,argv);
      if(strcmp("kgffmpeg",bname)==0) return kgffmpeg(argc,argv);
      else Mplayer(argc,argv);
      return 1;
    }
  }
#if 1
  if(!kgCheckCompositor()){
     printf("No Compositing Manager\n");
     WMErr=1;
  }
  else printf("Compositing Manager Available\n");
#endif
  strcpy(HomeDir,getenv("HOME"));
  chdir(HomeDir);
  mkdir((char *)".kgMplayer",0700);
  SetTheme();
  ForkGrab();
  ForkTools();
  Kulina =1;
  if((fp=fopen(".kgMplayer/Playlist","r"))==NULL) {
     fp = fopen(".kgMplayer/Playlist","w");
  }
  fclose(fp);
  mkdir((char *)"Music",0700);
  mkdir((char *)"Video",0700);
  chdir("Video");
  kgExtractBaseName(argv[0],bname);
//  printf("%s\n",bname);
  if(strcmp("kgMplayer",bname)==0) WMErr=1;

  RunkgMplayer(NULL);
//  printf("Finished\n");
  return 1;
}
