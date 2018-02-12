#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <malloc.h>
#include <string.h>

#define BUSY 1
#define FREE 0
typedef struct _Thread
{
    pthread_t id;
    int num;
    int status;
    struct _threadsinfo  *ThInfo;
}Thread; 
typedef struct _threadsinfo {
 int **PIPES;
 int **RPIPES;
 pthread_mutex_t Thlock;
 int nCores;
 int nTh;
 int ActSize; 
 Thread *threads;
} ThreadInfo;
typedef struct _Thaction
{
    void *(*threadFunc)(void *);
    void *arg;
}Thaction;
int getCores();
void *OpenThreads(int thds);
void DoInAnyThread(void *,void *(*threadFunc)(void *),void *arg);
void WaitThreads(void *);
void CloseThreads(void *);
void *threadFunc(void *arg);
int getCores()
{
    FILE *fp;
       char line[100];
       int cores=0;
       fp=fopen("/proc/cpuinfo","r");
       if(fp==NULL)
       {
               fprintf(stderr,"Unable to get coreinfo! Assuming one core!\n");
               cores=1;
       }
       while(!feof(fp))
       {
               fgets(line,99,fp);
               if(strncmp(line,"processor",9)==0) cores++;
       }
//       printf("CORES AVAILABLE: %d\n",cores);
       return cores;
} 
void * OpenThreads(int thds)
{
    ThreadInfo *ThInfo;
    int i;
       if(thds <= 0) thds=getCores();
       ThInfo = (ThreadInfo *)malloc(sizeof(ThreadInfo));
       ThInfo->nCores = thds;
//       pthread_mutex_init(&Thlock,NULL);
       ThInfo->threads=(Thread *)malloc(sizeof(Thread)*thds);
       ThInfo->PIPES = (int **) malloc(sizeof(int *)*thds);
       ThInfo->RPIPES = (int **) malloc(sizeof(int *)*thds);
       for(i=0;i<ThInfo->nCores;i++) {
          ThInfo->PIPES[i]= (int *)malloc(sizeof(int)*2);
          ThInfo->RPIPES[i]= (int *)malloc(sizeof(int)*2);
          pipe(ThInfo->PIPES[i]);
          pipe(ThInfo->RPIPES[i]);
       }
       if(ThInfo->threads==NULL)
       {
         printf("Error allocating memory\n");
         return NULL;
       }
       ThInfo->ActSize = sizeof(Thaction);
    
       for(i=0;i<ThInfo->nCores;i++)
       {
            ThInfo->threads[i].num=i;
            ThInfo->threads[i].ThInfo=ThInfo;
             
            pthread_create(&(ThInfo->threads[i].id),NULL,threadFunc,ThInfo->threads+i);
       }
       ThInfo->nTh=0;
       return ThInfo;
}
void DoInAnyThread(void *Tmp,void *(*threadFunc)(void *),void *arg) 
{
       Thaction job;
       int nTh=0;
       ThreadInfo *ThInfo;
       ThInfo = (ThreadInfo *)Tmp;
       nTh = ThInfo->nTh;
       job.threadFunc=threadFunc;
       job.arg = arg;
       write(ThInfo->PIPES[nTh][1],&job, ThInfo->ActSize);
       nTh++;
       nTh %= ThInfo->nCores;
       ThInfo->nTh = nTh;
}
void WaitThreads( void *Tmp)
{
    ThreadInfo *ThInfo;
    int i,count;
    Thaction job,rjob;
    ThInfo = (ThreadInfo *)Tmp;
    job.threadFunc=NULL;
    job.arg =NULL;

    for(i=0;i<ThInfo->nCores;i++) write(ThInfo->PIPES[i][1],&job, ThInfo->ActSize);
    for(i=0;i<ThInfo->nCores;i++) read(ThInfo->RPIPES[i][0],&rjob, ThInfo->ActSize);
} 
void *threadFunc(void *Th)
{
   Thaction job;
   Thread *th;
   int num;
   ThreadInfo *ThInfo;
   th = (Thread *)Th;
   ThInfo = th->ThInfo;
   num = (th)->num;
   while(1) {
          read(ThInfo->PIPES[num][0],&job, ThInfo->ActSize);
          th->status=BUSY;
          if(job.threadFunc == NULL) write(ThInfo->RPIPES[num][1],&job,ThInfo->ActSize);
          else job.threadFunc(job.arg);
          th->status=FREE;
//           printf("Executed job\n");
   }
} 
void CloseThreads( void *Tmp){
   ThreadInfo *ThInfo;
   int i;
   ThInfo = (ThreadInfo *)Tmp;
   if(Tmp==NULL) return;
   WaitThreads(Tmp);

//   printf("In Close Threads: %d\n",ThInfo->nCores);
#if 1
   for(i=0;i<ThInfo->nCores;i++)  {
//     pthread_join((ThInfo->threads[i].id),NULL);
     pthread_cancel((ThInfo->threads[i].id));
     pthread_join((ThInfo->threads[i].id),NULL);
   }
#endif
#if 1
   for(i=0;i<ThInfo->nCores;i++) {
     close(ThInfo->PIPES[i][0]);
     close(ThInfo->PIPES[i][1]);
     close( ThInfo->RPIPES[i][0]);
     close( ThInfo->RPIPES[i][1]);
     free(ThInfo->PIPES[i]);
     free( ThInfo->RPIPES[i]);
   }
#endif
   free(ThInfo->PIPES);
   free(ThInfo->RPIPES);
   free(ThInfo->threads);
   free(ThInfo);
}
