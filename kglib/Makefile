include config.mak
#MFLAGS	= -O -mcpu=pentium -march=pentium
#OFLAGS	= -O  -mpentium 

CC	= cc    -msse3 -O3 -DUNIX -pthread $(X11_CFLAFS)  $(CFLAGS)
#CC	= cc    -msse3 -g -DUNIX  $(CFLAGS)
all	: libkulina.a 
libkulina.a  : kgl.o kgui.o kgui1.o kgui2.o lowlib.o gkernel.o file_pro.o dlink.o printerdia.o shade.o PngImage.o gminterface.o makeimg.o drawimg.o  ogl.o threadtool.o
	ar -r libkulina.a kgl.o kgui.o kgui1.o kgui2.o lowlib.o gkernel.o file_pro.o  printerdia.o shade.o PngImage.o gminterface.o makeimg.o drawimg.o dlink.o ogl.o threadtool.o
kgl.o : kgl.c kulina.h gprivate.h
	$(CC) -c  -I$(PREFIX)/include/GraphicsMagick kgl.c
printerdia.o	: printerdia.c  kulina.h gprivate.h
		 $(CC)  -c  printerdia.c
shade.o : shade.c  kulina.h gprivate.h
	$(CC) -c  shade.c 
kgui.o : kgui.c kulina.h images.c gprivate.h
	$(CC) -c  kgui.c 
kgui1.o : kgui1.c kulina.h  gprivate.h
	$(CC) -c  kgui1.c 
kgui2.o : kgui2.c kulina.h gprivate.h
	$(CC) -c  kgui2.c 
lowlib.o : lowlib.c kulina.h txlib3d.c gprivate.h
	$(CC) -c  lowlib.c  
ogl.o : ogl.c kulina.h  gprivate.h
	$(CC) -c  ogl.c  
PngFile.o : PngFile.c kulina.h gprivate.h
	$(CC)  -c -I$(PREFIX)/include PngFile.c 
gminterface.o : gminterface.c kulina.h 
	$(CC)  -c -I$(PREFIX)/include -I$(PREFIX)/include/GraphicsMagick gminterface.c 
gkernel.o : gkernel.c kulina.h gprivate.h
	$(CC) -c  -I$(PREFIX)/include -I$(PREFIX)/include/GraphicsMagick gkernel.c 
dlink.o : dlink.c dlink.h
	$(CC) -c  dlink.c 
file_pro.o : file_pro.c gprivate.h kulina.h
	$(CC) -c  file_pro.c 
makeimg.o : makeimg.c kulina.h
	$(CC)  -c -I$(PREFIX)/include -I$(PREFIX)/include/GraphicsMagick makeimg.c 
drawimg.o : drawimg.c kulina.h
	$(CC)  -c -I$(PREFIX)/include -I$(PREFIX)/include/GraphicsMagick drawimg.c 
clean	:
	 rm -f *.o gph2ps gph2eps gph2jpg ps2jpg imagetoc cleantmpdir glib.a
install	: libkulina.a
	mv libkulina.a $(PREFIX)/lib/libkulina.a
	cp kulina.h $(PREFIX)/include
