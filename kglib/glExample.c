/* $Revision: 1.6 $ */
/* compile: cc -o glxsimple glxsimple.c -lGL -lX11 */
#include <stdio.h>
#include <stdlib.h>
#include <GL/glx.h>		/* this includes the necessary X headers */
#include <GL/gl.h>

static int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
static int dblBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

Display        *dpy;
Window          win;
GLfloat         xAngle = 42.0, yAngle = 82.0, zAngle = 112.0;
GLboolean	doubleBuffer = GL_TRUE;

void
fatalError(char *message)
{
    fprintf(stderr, "glxsimple: %s\n", message);
    exit(1);
}

void
redraw(void)
{
    static GLboolean   displayListInited = GL_FALSE;

    if (displayListInited) {
	/* if display list already exists, just execute it */
	glCallList(1);
    } else {
	/* otherwise compile and execute to create the display list */
	glNewList(1, GL_COMPILE_AND_EXECUTE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* front face */
	glBegin(GL_QUADS);
	glColor3f(0.0, 0.7, 0.1);	/* green */
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	/* back face */
	glColor3f(0.9, 1.0, 0.0);	/* yellow */
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	/* top side face */
	glColor3f(0.2, 0.2, 1.0);	/* blue */
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	/* bottom side face */
	glColor3f(0.7, 0.0, 0.1);	/* red */
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glEnd();
	glEndList();
	displayListInited = GL_TRUE;
    }
    if(doubleBuffer) glXSwapBuffers(dpy, win); /* buffer swap does implicit glFlush */
    else glFlush(); /* explicit flush for single buffered case */
}

void
main(int argc, char **argv)
{
    XVisualInfo    *vi;
    Colormap        cmap;
    XSetWindowAttributes swa;
    GLXContext      cx;
    XEvent          event;
    GLboolean       needRedraw = GL_FALSE, recalcModelView = GL_TRUE;
    int		    dummy;

    /* set up projection transform */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
    /* establish initial viewport */
    glViewport(0, 0, 300, 300); /* pedantic, full window size is default viewport */

    /*** (9) dispatch X events ***/

    while (1) {
	do {
	    XNextEvent(dpy, &event);
	    switch (event.type) {
	    case ButtonPress:
		recalcModelView = GL_TRUE;
		switch (event.xbutton.button) {
		case 1: xAngle += 10.0; break;
		case 2: yAngle += 10.0; break;
		case 3: zAngle += 10.0; break;
		}
		break;
	    case ConfigureNotify:
		glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
		/* fall through... */
	    case Expose:
		needRedraw = GL_TRUE;
		break;
	    }
	} while(XPending(dpy)); /* loop to compress events */
	if (recalcModelView) {
	    glMatrixMode(GL_MODELVIEW);
	    /* reset modelview matrix to the identity matrix */
	    glLoadIdentity();
	    /* move the camera back three units */
	    glTranslatef(0.0, 0.0, -3.0);
	    /* rotate by X, Y, and Z angles */
	    glRotatef(xAngle, 0.1, 0.0, 0.0);
	    glRotatef(yAngle, 0.0, 0.1, 0.0);
	    glRotatef(zAngle, 0.0, 0.0, 1.0);
	    recalcModelView = GL_FALSE;
	    needRedraw = GL_TRUE;
	}
	if (needRedraw) {
	    redraw();
	    needRedraw = GL_FALSE;
	}
    }
}
