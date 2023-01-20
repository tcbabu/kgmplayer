#include <stdio.h>
#include <kulina.h>
#include "mediainfo.h"
#include "images2videos.h"


int runfunction(char *job,int (*ProcessOut)(int,int,int),int (*function)(int,char **));
int kgffmpeg(int,char **);
int ffmpegfun(int,char **);
int ProcessSkip(int pip0,int pip1,int Pid);
int ProcessPrint(int pip0,int pip1,int Pid);
int ProcessToPipe(int pip0,int pip1,int Pid);
int Mencoder(int,char **);
void *RunkgGetFiles(void *arg,char *Filter);

extern int Jpipe[2];
extern int Jstat[2];
extern int MonPipe;

int Imgs2Videos( IMGS2VDATA *is2vdata) {
  char Folder[500],Tmpimage[500],Vname[500];
  int id,xl,yl,off,xv,yv,dx=0,dy=0;
  char HOME[500];
  char command[10000],File[500],options[5000],Fifo[500],Qstr[100];
  char mylist[500];
  FILE *myl=NULL;
  GMIMG *Img=NULL,*Timg=NULL;
  double iaspect,raspect;
  int vid=0,pid=0;
  int status;
  Dlink *L,*Vlist=NULL;
  char *vnames=NULL;
  MEDIAINFO *Minfo;

  if( (pid =fork())!= 0) {
    kgSplashMessage(NULL,100,100,300,40,"Send for Processing",1,0,15);
    sleep(2);
    return pid;
  }
  
  id = getpid();
  sprintf(Folder,"%-s/%-d",getenv("HOME"),id);
  if(FileStat(Folder)) kgCleanDir(Folder);
  L = (Dlink *)(is2vdata->List);
  if(L== NULL) return;
  mkdir(Folder,0700);
  if(pipe(Jpipe) < 0) exit(0);
  if(pipe(Jstat) < 0) exit(0);
  MonPipe = Jpipe[0];
  
  if ((pid=fork())==0) {
    close(Jpipe[0]);
    close(Jstat[1]);
#if 1  // modify as required
    sprintf(mylist,"%-s/mylist.txt",Folder);
    myl = fopen(mylist,"w");
    strcpy(Qstr,"3000K");
     Resetlink(L);
     sprintf(Tmpimage,"%-s/Image.jpg",Folder);
     sprintf(command,"ffmpegfun -f lavfi -i color=c=black:s=%-dx%-d  "
         "-t %-5.2f "
         " -y -f mp4 -b:v 3000K -vcodec libx265 \"%-s/%-d\"",
         is2vdata->Xsize,is2vdata->Ysize,is2vdata->imagetime, Folder,id);
     runfunction (command,ProcessSkip,ffmpegfun);
     Vlist = Dopen();
     while ( (Minfo= Getrecord(L)) != NULL) {
       sprintf(Vname,"%-s/Img%-5.5d.mp4",Folder,vid);
       sprintf(options,"Processing: %s\n",Minfo->Flname);
       write(Jpipe[1],options,strlen(options));
       sprintf(options,"Esec: %lf\n",vid*is2vdata->imagetime);
       write(Jpipe[1],options,strlen(options));
   
       vnames = (char *) malloc(strlen(Vname)+1);
       strcpy(vnames,Vname);
       Dadd(Vlist,vnames);
       Img = kgGetImage(Minfo->Flname);
       xl = Img->image_width;
       yl = Img->image_height;
       dx=0,dy=0;
       iaspect = (double)yl/xl;
       raspect = (double)is2vdata->Ysize/is2vdata->Xsize;
       if(iaspect>raspect) {
        if(is2vdata->fittoscrn) {
         xv = is2vdata->Xsize;
         yv = (int)(xv*iaspect);
        }
        else {
         yv = is2vdata->Ysize;
         xv = (int)(yv/iaspect);
         dx = (is2vdata->Xsize -xv)/2;
        }
        Timg = kgChangeSizeImage(Img, xv,yv);
        off = (yv - is2vdata->Ysize)/2;
        kgFreeImage(Img);
        Img = kgCropImage(Timg,0,off,xv-1,yv-1-off);
       }
       else {
        if(is2vdata->fittoscrn) {
         yv = is2vdata->Ysize;
         xv = (int)(yv/iaspect);
        }
        else{
         xv = is2vdata->Xsize;
         yv = (int)(xv*iaspect);
         dy = (is2vdata->Ysize -yv)/2;
        }
        Timg = kgChangeSizeImage(Img, xv,yv);
        off = (xv - is2vdata->Xsize)/2;
        kgFreeImage(Img);
        Img = kgCropImage(Timg,off,0,xv-1-off,yv-1);
       }
       kgFreeImage(Timg);
       kgWriteImage(Img, Tmpimage);
       kgFreeImage(Img);
       sprintf(command,"ffmpegfun -i \"%-s/%-d\" -i \"%s\" -filter_complex"
        " overlay=%-d:%-d -f mp4 -b:v 3000K -vcodec libx265 -y \"%-s\" ",
         Folder,id,Tmpimage,dx,dy,Vname);
       runfunction (command,ProcessToPipe,ffmpegfun);
       fprintf(myl,"file  \'%-s\'\n",Vname);
       fflush(myl);
       vid++;
     } // while...
     remove(Tmpimage);
     sprintf(Tmpimage,"%-s/%d",Folder,id+1);
     remove(Tmpimage);
     fclose(myl);
      is2vdata->Vlist =Vlist;

     sprintf(options,"JOINING and CONVERTING to mp4\n");
     write(Jpipe[1],options,strlen(options));
     sprintf(options,"Esec: %lf\n",vid*is2vdata->imagetime);
     write(Jpipe[1],options,strlen(options));
     sprintf(options,"Total Video:%9.1lf secs\n",vid*is2vdata->imagetime);
     write(Jpipe[1],options,strlen(options));
     sprintf(options,"Plese wait......\n");
     write(Jpipe[1],options,strlen(options));
     sprintf(command,"ffmpegfun -f concat -i %-s -y -f mp4 -vcodec libx265 "
        " -b:v %-s \"%-s\" ", mylist ,Qstr,is2vdata->Outfile);
     runfunction(command,ProcessToPipe,ffmpegfun);

     Dempty(Vlist);
     Vlist=NULL;
     is2vdata->Vlist =NULL;
#endif 
     if(FileStat(Folder)) kgCleanDir(Folder);
     close(Jpipe[1]);
     close(Jstat[0]);
     exit(0);
  }
  else {
     close(Jpipe[1]);
     close(Jstat[0]);
     RunMonitorJoin(NULL);
     kill(pid,9);
     waitpid(pid,&status,0);
     if(FileStat(Folder)) kgCleanDir(Folder);
     exit(0);
  }
}
