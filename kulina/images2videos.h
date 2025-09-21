#ifndef D_IMGS2VDATA
#define D_IMGS2VDATA
typedef struct _imgs2v_data {
  void *List;
  char Outfile[500];
  int Xsize;
  int Ysize;
  int fps;
  float imagetime;
  int fittoscrn; 
  void *Vlist;
} IMGS2VDATA;
int Imgs2Videos( IMGS2VDATA *is2vdata);

#endif
