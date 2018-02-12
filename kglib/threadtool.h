#ifndef THREADTOOL_H
#define THREADTOOL_H
int getCores();
void *OpenThreads(int thds);
void DoInAnyThread(void *,void *(*threadFunc)(void *),void *arg);
void WaitThreads(void *);
void CloseThreads(void *);
#endif
