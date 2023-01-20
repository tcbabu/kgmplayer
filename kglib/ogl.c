//#define D_OGL
#include<X11/Xlib.h>
#include<X11/X.h>
#include "kulina.h"
#include "gprivate.h"
#ifdef D_OGL
#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/glx.h>
int kgSwapglBuffers(void *Gtmp) {
    Window glWin;
    DIG *G;
    Display *Dsp;
    G = (DIG *)Gtmp;
    Dsp = WC(G->D)->Dsp;
    glWin = *((Window *)(G->glWindow));
    if(G->glDouble)glXSwapBuffers(Dsp,glWin);
    else glFlush();
    return 1;
}
int  kgInitglWindow(void* Gtmp) {
#if 0	
    int snglBuf[] ={GLX_RGBA,GLX_RED_SIZE,1,GLX_GREEN_SIZE,1,
	            GLX_BLUE_SIZE,1,GLX_DEPTH_SIZE,12,None};
    int dblBuf[] ={GLX_RGBA,GLX_RED_SIZE,1,GLX_GREEN_SIZE,1,
	            GLX_BLUE_SIZE,1,GLX_DEPTH_SIZE,12,GLX_DOUBLEBUFFER,None};
#endif
    DIG *G;
    int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
    int dblBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};
    XVisualInfo *vInfoMain;
    GLXContext *cMain;
    Window wMain,*sWin;
    int nvis,dummy;
    XVisualInfo  sampleVis;
    Display *xDisplay;
    int xScreen;
    DIALOG *D;
    kgWC *wc;
    G = (DIG *)Gtmp;
    D = (DIALOG *)G->D;
    wc = WC(D);
    xDisplay = wc->Dsp;
#if 1
    if(!glXQueryExtension(xDisplay, &dummy, &dummy)) {
      printf("X server has no OpenGL GLX extension\n");
      return 0;
    }
#endif
    xScreen = DefaultScreen(xDisplay);
    kgSubWindow(Gtmp);
    sWin = (Window *)G->glWindow;
    wMain = *sWin;
#if 0
    sampleVis.screen = xScreen;
    sampleVis.depth = DisplayPlanes(xDisplay,DefaultScreen(xDisplay));
    sampleVis.visual = XDefaultVisual(xDisplay,DefaultScreen(xDisplay));
    sampleVis.visualid = XVisualIDFromVisual(
         XDefaultVisual(xDisplay,DefaultScreen(xDisplay)));
    vInfoMain =  XGetVisualInfo(xDisplay,
       VisualIDMask|VisualScreenMask|VisualDepthMask,&sampleVis, &nvis);
#else
    vInfoMain =  glXChooseVisual(xDisplay,xScreen,dblBuf);
    if(vInfoMain == NULL) {
      vInfoMain =  glXChooseVisual(xDisplay,xScreen,snglBuf);
      if(vInfoMain == NULL) {
	      printf("No RGB Visual with Depth Buffer\n");
	      return GL_FALSE;
      }
      else { 
        G->glDouble=0;
	printf("RGB Visual with Depth Buffer(single)\n");
      }
	
    }
    else {
      G->glDouble=1;
      printf("RGB Visual with Depth Buffer(double)\n");
    }
    if(vInfoMain->class != TrueColor) {
	    printf("TrueColor visual required for this program\n");
	    exit(1);
    }
#endif
    cMain = (GLXContext *)Malloc(sizeof(GLXContext));
    *cMain = glXCreateContext(xDisplay, vInfoMain, None,
			       GL_TRUE);
    if (!(*cMain)) {
	fprintf(stderr, "Can't create a context!\n");
	return GL_FALSE;
    }
    G->cMain = (void *) cMain;

    if (!glXMakeCurrent(xDisplay, wMain, *cMain)) {
	printf( "Can't make window current drawable!\n");
	return GL_FALSE;
    }
    XMapWindow(xDisplay, wMain);
    glViewport(0,0,(G->x2-G->x1+1),(G->y2-G->y1+1));

    return GL_TRUE;
}
int kgEnableglWindow(void *Gtmp){
   DIG *G;
   DIALOG *D;
   kgWC *wc;
   G = (DIG *)Gtmp;
   D = (DIALOG *)(G->D);
   wc = D->wc;
    if (!glXMakeCurrent(wc->Dsp, *((Window *)(G->glWindow)), *((GLXContext *)(G->cMain)))) {
	printf( "Can't make window current drawable!\n");
	return GL_FALSE;
    }
    return GL_TRUE;
}
void *kgGetglDisplay(void *Gtmp){
   DIG *G;
   DIALOG *D;
   kgWC *wc;
   G = (DIG *)Gtmp;
   D = (DIALOG *)(G->D);
   wc = D->wc;
   return wc->Dsp;
}
void *kgGetglWindow(void *Gtmp){
   DIG *G;
   G = (DIG *)Gtmp;
   if(G->glWindow==NULL) kgInitglWindow(Gtmp);
   return G->glWindow;
}
void kgCloseglWindow(void *Gtmp) {
   DIG *G;
   DIALOG *D;
   kgWC *wc;
   GLXContext *cMain;
   G = (DIG *)Gtmp;
   D = (DIALOG *)(G->D);
   wc = D->wc;
   if(G->glWindow== NULL) return;
   if(G->cMain!=NULL){
     kgSwapglBuffers(G);
   }
   XDestroyWindow(wc->Dsp,*((Window *)G->glWindow));
   free(G->glWindow);
   G->glWindow= NULL;
   if(G->cMain==NULL) return;
   cMain = (GLXContext *)G->cMain;
   glXDestroyContext(wc->Dsp,*cMain);
   free(cMain);
   G->cMain=NULL;
   
}
#else

void kgCloseglWindow(void *Gtmp) {
   DIG *G;
   DIALOG *D;
   kgWC *wc;
   G = (DIG *)Gtmp;
   D = (DIALOG *)(G->D);
   wc = D->wc;
   if(G->glWindow== NULL) return;
   XDestroyWindow(wc->Dsp,*((Window *)G->glWindow));
   free(G->glWindow);
   G->glWindow= NULL;
   return;
}
void *kgGetglDisplay(void *Gtmp){
    return 0;
}
void *kgGetglWindow(void *Gtmp){
   DIG *G;
   DIALOG *D;
   kgWC *wc;
   G = (DIG *)Gtmp;
   D = (DIALOG *)(G->D);
   wc = D->wc;
   kgSubWindow(Gtmp);
//   XMapWindow(wc->Dsp, *((Window *)(G->glWindow)));
   return ((DIG *)Gtmp)->glWindow;
}
int  kgInitglWindow(void* Gtmp) {
    return 0;
}
int kgEnableglWindow(void *Gtmp){
    return 0;
}
int kgSwapglBuffers(void *Gtmp) {
    return 0;
}
#endif
