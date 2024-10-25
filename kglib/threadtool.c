#define _OPEN_THREADS
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#define BUSY 1
#define FREE 0
  typedef struct _Thread {
      pthread_t id;
      int num;
      int status;
      struct _threadsinfo *ThInfo;
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
  typedef struct _Thaction {
      void * ( *threadFunc ) ( void * ) ;
      void *arg;
  }Thaction;
  struct timespec req = { 0 , 1000};
  struct timespec rem;
  int getCores ( ) ;
  void *OpenThreads ( int thds ) ;
  void DoInAnyThread ( void *, void * ( *threadFunc ) ( void * ) , void *arg ) ;
  void WaitThreads ( void * ) ;
  void CloseThreads ( void * ) ;
  void KillThreads ( void * ) ;
  void *threadFunc ( void *arg ) ;
  int getCores ( ) {
      FILE *fp;
      char line [ 100 ] ;
      int cores = 0;
      fp = fopen ( "/proc/cpuinfo" , "r" ) ;
      if ( fp == NULL ) {
          fprintf ( stderr , "Unable to get coreinfo! Assuming one core!\n" ) ;
          cores = 1;
      }
      while ( ! feof ( fp ) ) {
          char *rval;
          rval = fgets ( line , 99 , fp ) ;
          if ( strncmp ( line , "processor" , 9 ) == 0 ) cores++;
      }
      if ( cores < 1 ) cores = 1;
//       fprintf(stderr,"CORES AVAILABLE: %d\n",cores);
      return cores;
  }
  void * OpenThreads ( int thds ) {
      ThreadInfo *ThInfo;
      int rval;
      int i;
      if ( thds <= 0 ) thds = getCores ( ) ;
      ThInfo = ( ThreadInfo * ) malloc ( sizeof ( ThreadInfo ) ) ;
      ThInfo->nCores = thds;
//       pthread_mutex_init(&Thlock,NULL);
      ThInfo->threads = ( Thread * ) malloc ( sizeof ( Thread ) *thds ) ;
      ThInfo->PIPES = ( int ** ) malloc ( sizeof ( int * ) *thds ) ;
      ThInfo->RPIPES = ( int ** ) malloc ( sizeof ( int * ) *thds ) ;
      for ( i = 0;i < ThInfo->nCores;i++ ) {
          ThInfo->PIPES [ i ] = ( int * ) malloc ( sizeof ( int ) *2 ) ;
          ThInfo->RPIPES [ i ] = ( int * ) malloc ( sizeof ( int ) *2 ) ;
          rval = pipe ( ThInfo->PIPES [ i ] ) ;
          rval = pipe ( ThInfo->RPIPES [ i ] ) ;
      }
      if ( ThInfo->threads == NULL ) {
          fprintf ( stderr , "Error allocating memory\n" ) ;
          return NULL;
      }
      ThInfo->ActSize = sizeof ( Thaction ) ;
      for ( i = 0;i < ThInfo->nCores;i++ ) {
          ThInfo->threads [ i ] .num = i;
          ThInfo->threads [ i ] .ThInfo = ThInfo;
          pthread_create ( & ( ThInfo->threads [ i ] .id ) , NULL , threadFunc , ThInfo->threads+i ) ;
              
      }
      ThInfo->nTh = 0;
//       fprintf(stderr,"Opened Threads\n");
      return ThInfo;
  }
  void DoInAnyThread ( void *Tmp , void * ( *threadFunc )  \
      ( void * ) , void *arg ) {
      int count = 0;
      int wd , nw;
      Thaction job;
      int nTh = 0;
      char *pt;
      ThreadInfo *ThInfo;
      ThInfo = ( ThreadInfo * ) Tmp;
      nTh = ThInfo->nTh;
      job.threadFunc = threadFunc;
      job.arg = arg;
      nw = ThInfo->ActSize;
      pt = ( char * ) & job;
      count = write ( ThInfo->PIPES [ nTh ] [ 1 ] , pt , nw ) ;
      while ( count < nw ) {
          wd = write ( ThInfo->PIPES [ nTh ] [ 1 ] , pt+count , nw-count ) ;
          count += wd;
      }
      nTh++;
      nTh %= ThInfo->nCores;
      ThInfo->nTh = nTh;
      return;
  }
  void WaitThreads ( void *Tmp ) {
      ThreadInfo *ThInfo;
      int i , count , rwval;
      Thaction job , rjob;
      ThInfo = ( ThreadInfo * ) Tmp;
      job.threadFunc = NULL;
      job.arg = NULL;
      for ( i = 0;i < ThInfo->nCores;i++ ) rwval = write ( ThInfo->PIPES [ i ] [ 1 ] , & job , ThInfo->ActSize ) ;
          
      for ( i = 0;i < ThInfo->nCores;i++ ) rwval = read ( ThInfo->RPIPES [ i ] [ 0 ] , & rjob , ThInfo->ActSize ) ;
          
//    fprintf(stderr,"Returning\n");
//    fflush(stderr);
      return;
  }
  void WaitClose ( void *Tmp ) {
      ThreadInfo *ThInfo;
      int i , count , rwval;
      Thaction job , rjob;
      ThInfo = ( ThreadInfo * ) Tmp;
      job.threadFunc = NULL;
      job.arg = ( void * ) 1;
      for ( i = 0;i < ThInfo->nCores;i++ ) rwval = write ( ThInfo->PIPES [ i ] [ 1 ] , & job , ThInfo->ActSize ) ;
          
      for ( i = 0;i < ThInfo->nCores;i++ ) rwval = read ( ThInfo->RPIPES [ i ] [ 0 ] , & rjob , ThInfo->ActSize ) ;
          
      for ( i = 0;i < ThInfo->nCores;i++ ) rwval = write ( ThInfo->PIPES [ i ] [ 1 ] , & job , ThInfo->ActSize ) ;
          
//    fprintf(stderr,"Returning\n");
//    fflush(stderr);
      return;
  }
  void *threadFunc ( void *Th ) {
      int count , rwval;
      int rd , nr;
      int Oldstate , Oldtype , Ret;
      Thaction job;
      Thread *th;
      char *pt;
      int num;
      void *Rval;
      ThreadInfo *ThInfo;
      th = ( Thread * ) Th;
      ThInfo = th->ThInfo;
      num = ( th )->num;
      nr = ThInfo->ActSize;
      pt = ( char * ) & job;
      Ret = pthread_setcancelstate ( PTHREAD_CANCEL_ENABLE , & Oldstate ) ;
      Ret = pthread_setcanceltype ( PTHREAD_CANCEL_DEFERRED , & Oldtype ) ;
      while ( 1 ) {
          count = read ( ThInfo->PIPES [ num ] [ 0 ] , pt , nr ) ;
          while ( count < nr ) {
              rd = read ( ThInfo->PIPES [ num ] [ 0 ] , pt+count , nr-count ) ;
              count += rd;
          }
          th->status = BUSY;
          if ( job.threadFunc == NULL ) {
              rwval = write ( ThInfo->RPIPES [ num ] [ 1 ] , & job , ThInfo->ActSize ) ;
//            Ret = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,&Oldtype);
              if ( job.arg != NULL ) {
                  th->status = FREE;
                  count = read ( ThInfo->PIPES [ num ] [ 0 ] , pt , nr ) ;
                  while ( count < nr ) {
                      rd = read ( ThInfo->PIPES [ num ] [ 0 ] , pt+count , nr-count ) ;
                      count += rd;
                  }
//      	      fprintf(stderr,"Read Finish\n");
//	    nanosleep(&req,&rem);
                  return NULL;
              }
          }
          else Rval = job.threadFunc ( job.arg ) ;
          th->status = FREE;
      }
  }
  void CloseThreads ( void *Tmp ) {
      ThreadInfo *ThInfo;
      int i;
      void *Rval;
      ThInfo = ( ThreadInfo * ) Tmp;
      if ( Tmp == NULL ) return;
//   fprintf(stderr,"Waiting Threads\n");
#if 1
      WaitClose ( Tmp ) ;
#else
      {
          int i , count , rwval;
          Thaction job , rjob;
          job.threadFunc = NULL;
          job.arg = ( void * ) 1;
          for ( i = 0;i < ThInfo->nCores;i++ ) rwval = write ( ThInfo->PIPES [ i ] [ 1 ] , & job , ThInfo->ActSize ) ;
              
          for ( i = 0;i < ThInfo->nCores;i++ ) rwval = read ( ThInfo->RPIPES [ i ] [ 0 ] , & rjob , ThInfo->ActSize ) ;
              
          for ( i = 0;i < ThInfo->nCores;i++ ) rwval = write ( ThInfo->PIPES [ i ] [ 1 ] , & job , ThInfo->ActSize ) ;
              
          fprintf ( stderr , "Waiting Over\n" ) ;
          fprintf ( stderr , "Returning\n" ) ;
          fflush ( stderr ) ;
      }
      fprintf ( stderr , "Waiting Over\n" ) ;
      fprintf ( stderr , "Waiting Over\n" ) ;
      fflush ( stderr ) ;
      fflush ( stderr ) ;
#endif
//      Rval = ( void * ) malloc ( sizeof ( char * ) ) ;
//   fprintf(stderr,"In Close Threads: %d\n",ThInfo->nCores);
//   fflush(stderr);
#if 1
      for ( i = 0;i < ThInfo->nCores;i++ ) {
//     pthread_join((ThInfo->threads[i].id),NULL);
//     pthread_cancel((ThInfo->threads[i].id));
          pthread_join ( ( ThInfo->threads [ i ] .id ) , NULL ) ;
      }
#endif
#if 1
      for ( i = 0;i < ThInfo->nCores;i++ ) {
          close ( ThInfo->PIPES [ i ] [ 0 ] ) ;
          close ( ThInfo->PIPES [ i ] [ 1 ] ) ;
          close ( ThInfo->RPIPES [ i ] [ 0 ] ) ;
          close ( ThInfo->RPIPES [ i ] [ 1 ] ) ;
          free ( ThInfo->PIPES [ i ] ) ;
          free ( ThInfo->RPIPES [ i ] ) ;
      }
#endif
      free ( ThInfo->PIPES ) ;
      free ( ThInfo->RPIPES ) ;
      free ( ThInfo->threads ) ;
      free ( ThInfo ) ;
      return;
  }
  void KillThreads ( void *Tmp ) {
      ThreadInfo *ThInfo;
      int i,s;
      void *Rval;
      ThInfo = ( ThreadInfo * ) Tmp;
      if ( Tmp == NULL ) return;
//   fprintf(stderr,"Waiting Threads\n");
//      Rval = ( void * ) malloc ( sizeof ( char * ) ) ;
      for ( i = 0;i < ThInfo->nCores;i++ ) {
       s = pthread_cancel((ThInfo->threads[i].id));
       if(s != 0)pthread_kill((ThInfo->threads[i].id),SIGKILL); 
          pthread_join ( ( ThInfo->threads [ i ] .id ) , NULL ) ;
      }
#if 1
      for ( i = 0;i < ThInfo->nCores;i++ ) {
#if 1
          close ( ThInfo->PIPES [ i ] [ 0 ] ) ;
          close ( ThInfo->PIPES [ i ] [ 1 ] ) ;
          close ( ThInfo->RPIPES [ i ] [ 0 ] ) ;
          close ( ThInfo->RPIPES [ i ] [ 1 ] ) ;
#endif
          free ( ThInfo->PIPES [ i ] ) ;
          free ( ThInfo->RPIPES [ i ] ) ;
      }
#endif
      free ( ThInfo->PIPES ) ;
      free ( ThInfo->RPIPES ) ;
      free ( ThInfo->threads ) ;
      free ( ThInfo ) ;
      return;
  }
