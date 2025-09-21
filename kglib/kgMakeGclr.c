#include "kulina.h"
#include <string.h>
#define NT 38
typedef struct _gcentry {
	char *name; char * vname;int clrno;int red;int green;int blue;
} GCENTRY;
GCENTRY gcclrs[NT]= {
	{(char *)"char color ", (char *)"char_clr"},
	{(char *)"fill color ", (char *)"fill_clr"},
	{(char *)"border color ", (char *)"bodr_clr"},
	{(char *)"cursor color ", (char *)"cur_clr"},
	{(char *)"highlight color ", (char *)"high_clr"},
	{(char *)"char highlight color ", (char *)"char_hclr"},
	{(char *)"msg char color", (char *)"msg_char"},
	{(char *)"msg fill color", (char *)"msg_fill"},
	{(char *)"msg bodr color ", (char *)"msg_bodr"},
	{(char *)"text box fill", (char *)"txt_fill"},
	{(char *)"text entry color", (char *)"txt_char"},
	{(char *)"text prompt color", (char *)"txt_pchar"},
	{(char *)"table fill color", (char *)"tabl_fill"},
	{(char *)"table line color", (char *)"tabl_line"},
	{(char *)"table char color", (char *)"tabl_char"},
	{(char *)"table highlight color", (char *)"tabl_hchar"},
	{(char *)"very dim color(3D)", (char *)"v_dim"},
	{(char *)"dim color (3D) ", (char *)"dim"},
	{(char *)"bright color(3D)", (char *)"bright"},
	{(char *)"very bright color(3D)", (char *)"vbright"},
	{(char *)"thumbnail browser fill", (char *)"twin_fill"},
	{(char *)"thumbnail browser char", (char *)"twin_char"},
	{(char *)"humbnail browser border", (char *)"twin_bodr"},
	{(char *)"info box fill", (char *)"info_fill"},
	{(char *)"info box char", (char *)"info_char"},
	{(char *)"button char color", (char *)"but_char"},
	{(char *)"menu char color", (char *)"menu_char"},
	{(char *)"splash fill color", (char *)"SplashFillColor"},
	{(char *)"splash border color", (char *)"SplashBodrColor"},
	{(char *)"splash char color", (char *)"SplashCharColor"},
	{(char *)"progress bar fill", (char *)"ProgFillColor"},
	{(char *)"progrss bar border", (char *)"ProgBodrColor"},
	{(char *)"progress bar color", (char *)"ProgColor"},
	{(char *)"Scroll fill color", (char *)"scroll_fill"},
	{(char *)"scroll bright color", (char *)"scroll_bright"},
	{(char *)"scroll very bright", (char *)"scroll_vbright"},
	{(char *)"scroll dim color", (char *)"scroll_dim"},
	{(char *)"Browser Highlight", (char *)"ItemHighColor"},
};
int Mkthumps(void *Tmp,void *Tmp1) {
	Gclr *gc;
	int i=0,r,g,b;
	int *pt;
	ThumbNail **th0 ;
	char *menu[NT+1];
	DIY *r0;
	if(Tmp1 != NULL) gc = &(((DIALOG *)Tmp1)->gc);
	else gc = &(((DIALOG *)Tmp)->gc);
	pt = (int *)gc;
	r0 = (DIY *) kgGetNamedWidget(Tmp,"Radio");
	for (i=0;i<NT;i++) {
		gcclrs[i].clrno=pt[i];
		kgGetDefaultRGB(pt[i],&r,&g,&b);
		gcclrs[i].red = r;
		gcclrs[i].green = g;
		gcclrs[i].blue = b;
		printf("%s %d : %d %d %d\n",gcclrs[i].name,gcclrs[i].clrno,
				gcclrs[i].red,gcclrs[i].green,gcclrs[i].blue);
#if 0
		kgDefineColor(gcclrs[i].clrno.r,g,b);
		gcclrs[i].red = r;
		gcclrs[i].green = g;
		gcclrs[i].blue = b;
		printf("%s %d : %d %d %d\n",gcclrs[i].name,gcclrs[i].clrno,
				gcclrs[i].red,gcclrs[i].green,gcclrs[i].blue);
#endif
		menu[i]=gcclrs[i].name;
	}
	menu[NT]=NULL;
	th0 = (ThumbNail **)kgStringToThumbNails((char **)menu);
	for (i=0;i<NT;i++) {
		th0[i]->img = kgBorderedRectangle(60,50,gcclrs[i].clrno,0.0);
	}
	r0->list = (void **)th0;
	kgUpdateWidget(r0);
	kgUpdateOn(Tmp);

	return 1;

}
static void MkGclrlines(void) {
	int i;
	FILE *fp;
	fp = fopen("Gclr.c","w");
	fprintf(fp,"void ModifydemoGc(void *Tmp) {\n");
	fprintf(fp,"  DIALOG *D;\n");
	fprintf(fp,"  Gclr *gc;\n");
	fprintf(fp,"  D = (DIALOG *)Tmp;\n");
	fprintf(fp,"  gc = &(D->gc);\n");
	fprintf(fp,"#if 0\n");
	fprintf(fp,"  kgColorTheme(D,220,220,200);\n");
	fprintf(fp,"  kgColorTheme1(D,220,220,200);\n");
	fprintf(fp,"  kgColorTheme2(D,220,220,200);\n");
	fprintf(fp,"#endif\n");
	for(i=0;i<NT;i++) {
		fprintf(fp,"  kgDefineColor(%d,%d,%d,%d);\n",
		  gcclrs[i].clrno,gcclrs[i].red,gcclrs[i].green,gcclrs[i].blue);
		fprintf(fp,"  gc->%-s = %d;\n",gcclrs[i].vname,gcclrs[i].clrno);
	}
	fprintf(fp,"}\n");
	fclose(fp);
	return;
}
static void WriteGclr(void* Tmp) {
        DIALOG *D;
	void **args;
        D = (DIALOG *)Tmp;
	int i;
	char buf[300];
	FILE *fp;
//	sprintf(buf,"Gclr%-s.c",DiaName);
	args = (void **)(D->pt);
	sprintf(buf,"Gclr%-s.c",(char *)(args[0]));
	fp = fopen(buf,"w");
//	sprintf(buf,"void Modify%-sGc(void *Tmp) {",DiaName);
	sprintf(buf,"void Modify%-sGc(void *Tmp) {",(char *)(args[0]));
	fprintf(fp,"%s \n",buf);
	fprintf(fp,"  DIALOG *D;\n");
	fprintf(fp,"  Gclr *gc;\n");
	fprintf(fp,"  D = (DIALOG *)Tmp;\n");
	fprintf(fp,"  gc = &(D->gc);\n");
	fprintf(fp,"#if 0\n");
	fprintf(fp,"  kgColorTheme(D,220,220,200);\n");
	fprintf(fp,"  kgColorTheme1(D,220,220,200);\n");
	fprintf(fp,"  kgColorTheme2(D,220,220,200);\n");
	fprintf(fp,"#endif\n");
	for(i=0;i<NT;i++) {
		fprintf(fp,"  kgDefineColor(%d,%d,%d,%d);\n",
		  gcclrs[i].clrno,gcclrs[i].red,gcclrs[i].green,gcclrs[i].blue);
		fprintf(fp,"  gc->%-s = %d;\n",gcclrs[i].vname,gcclrs[i].clrno);
	}
	fprintf(fp,"//  kgMkgclr(\"%-s\",Tmp);\n",(char *)(args[0]));
	fprintf(fp,"}\n");
	fclose(fp);
	return;
}
int  mkgclrsplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  switch(butno) {
    case 1: 
      break;
    case 2: 
      WriteGclr(Tmp);
      break;
    case 3:
      MkGclrlines();
      system("tar xf /usr/share/kulina/demo.tar -C /tmp");
      system("cp Gclr.c /tmp/demo");
      system("make -C /tmp/demo"); 
      system("/tmp/demo/demo");
      remove("/tmp/demo");
      ret=0;
  }
  return ret;
}
void  mkgclrsplbutton1init(DIL *B,void *pt) {
}
int  mkgclrbrowser1callback(int item,int i,void *Tmp) {
  DIALOG *D;DIY *Y;void *pt; 
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret=1; 
  int r,g,b,j;
  static int clrno=65;
  ThumbNail **th0;
  D = (DIALOG *)Tmp;
  j = item -1;
  pt = D->pt;
  Y = (DIY *)kgGetWidget(Tmp,i);
  kgGetColor(Tmp,250,400,&(gcclrs[j].red),&(gcclrs[j].green),&(gcclrs[j].blue));
  th0 = (ThumbNail **) (Y->list);
  gcclrs[j].clrno = clrno+j;
  kgDefineColor(gcclrs[j].clrno,gcclrs[j].red,gcclrs[j].green,gcclrs[j].blue);
  th0[j]->img = kgBorderedRectangle(60,50,gcclrs[j].clrno,0.0);
  kgUpdateWidget(Y);
  kgUpdateOn(Tmp);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  mkgclrbrowser1init(DIY *Y,void *pt) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
}
int mkgclrinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  void **arg;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  arg = (void **)pt;
  printf("%s %s\n",(char *)arg[0],(char *)arg[1]);
  Mkthumps(Tmp,arg[1]);

  return ret;
}
int mkgclrcleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int Modifymkgclr(void *Tmp,int GrpId) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  DIA *d;
  int i,n;
  d = D->d;
  i=0;while(d[i].t!= NULL) {;
     i++;
  };
  n=1;
  return GrpId;
}

int mkgclrCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
int mkgclrResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  kgGetWindowSize(D,&xres,&yres);
  dx = xres - D->xl;
  dy = yres - D->yl;
  /* extra code */
  D->xl= xres;
  D->yl= yres;
  kgRedrawDialog(D);
  return ret;
}
int mkgclrWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
void ModifymkgclrGc(void *Tmp) {
   DIALOG *D;
   Gclr *gc;
   D = (DIALOG *)Tmp;
   gc = &(D->gc);
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   kgColorTheme(D,220,220,200);
   kgColorTheme1(D,220,220,200);
   kgColorTheme2(D,220,220,200);
   kgDefaultGuiTheme(gc);
   kgGrayGuiTheme(gc);
   gc->FontSize =8;
   gc->Font=23;
*/
//    Mkthumps(Tmp);
}
int mkgclrGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  BUT_STR  *butn0=NULL; 
  butn0= (BUT_STR *)malloc(sizeof(BUT_STR)*3);
  butn0[0].sw=1;
  strcpy(butn0[0].title,(char *)"Cancel");
  butn0[0].xpmn=NULL;
  butn0[0].xpmp=NULL;
  butn0[0].xpmh=NULL;
  butn0[0].bkgr=-1;
  butn0[0].butncode='';
  butn0[1].sw=1;
  strcpy(butn0[1].title,(char *)"Okay");
  butn0[1].xpmn=NULL;
  butn0[1].xpmp=NULL;
  butn0[1].xpmh=NULL;
  butn0[1].bkgr=-1;
  butn0[1].butncode='';
  butn0[2].sw=1;
  strcpy(butn0[2].title,(char *)"Test");
  butn0[2].xpmn=NULL;
  butn0[2].xpmp=NULL;
  butn0[2].xpmh=NULL;
  butn0[2].bkgr=-1;
  butn0[2].butncode='';
  DIL h0 = { 
    'h',
    315,640,  
    570,673,
    3,0,  
    72, 
    25, 
    3,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn0, 
    mkgclrsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h0.Wid,(char *)"mkgclrWidget5");
  h0.item = -1;
  DIY y1 = { 
    'y',
    14,13,  
    913,627,   
    80,25,  
    64, 
    64, 
    1,25, 
    3,4, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,mkgclrbrowser1callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     1,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y1.Wid,(char *)"Radio");
  y1.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+3));
  d =dtmp+i; 
  d[2].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIL));
  *d[0].h = h0;
  d[0].h->item = -1;
  mkgclrsplbutton1init(d[0].h,pt) ;
  d[1].t = (DIT *)malloc(sizeof(DIY));
  *d[1].y = y1;
  d[1].y->item = -1;
  mkgclrbrowser1init(d[1].y,pt) ;
  d[2].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakemkgclrGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    ThumbnailBrowser1  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   void** v=(void **)malloc(sizeof(void*)*2);
   v[1]=NULL;
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = mkgclrGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int mkgclr( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = mkgclrGroup(&D,v,pt);
#else 
  GrpId = MakemkgclrGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 930;    /*  Length of Dialog */
  D.yl = 693;    /*  Width  of Dialog */
  D.Initfun = mkgclrinit;    /*   init fuction for Dialog */
  D.Cleanupfun = mkgclrcleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = mkgclrCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = mkgclrResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = mkgclrWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.NoWinMngr = 0;    /*  1 for no Window Manager*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
  Modifymkgclr(&D,GrpId);    /*  add extras to  gui*/
  ModifymkgclrGc(&D);    /*  set colors for gui if don't like default*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *uiRunmkgclr(void **arg) {
/*************************************************

    ThumbnailBrowser1  1 data value

*************************************************/
   int   v0 = 1;
   void* v[1];
   v[0]=(void *)(&v0);
   void *pt=NULL; /* pointer to send any extra information */
   pt =(void *)arg;
   mkgclr(NULL,v,pt );
   return NULL;
}
void kgMkgclr(void *dianame,void *Dia) {
/*
   One can use it to design GUI theme.
   can call this in Modify<dianame>gc() at design time
   Once the design is complete, it can be removed
   or suppressed
*/
	void *args[2];
	args[0]=dianame;
	args[1]=Dia;
        uiRunmkgclr(args);
}
