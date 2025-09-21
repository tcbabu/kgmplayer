include config.mak
CC = gcc
VERSION = 1.2.1
LIBS = $(LDFLAGS) -lssl -lcrypto  -lz -ldl
INSTALL=/usr/bin/install -c
bindir=$(prefix)${exec_prefix}/bin
libdir=$(prefix)${exec_prefix}/lib
DESTDIR=
FLAGS=$(CFLAGS) -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DSTDC_HEADERS=1 -DHAVE_LIBZ=1 -D_LARGEFILE64_SOURCE=1 -D_FILE_OFFSET_BITS=64 -DHAVE_LIBSSL=1 -DHAVE_LIBCRYPTO=1 
OBJ=movgrab.o containerfiles.o outputfiles.o common.o cbsnews.o dailymotion.o  ehow.o  youtube.o servicetypes.o extract_text.o download.o display.o

all: $(OBJ)
	@cp config.mak libUseful-2.0
	@cd libUseful-2.0; $(MAKE)
	ar rcs libmovgrab.a $(OBJ)
	$(CC) $(FLAGS) -o movgrab main.c  libmovgrab.a  libUseful-2.0/libUseful-2.0.a $(LIBS)

clean:
	@rm -f movgrab *.o *.a libUseful-2.0/*.o libUseful-2.0/*.a libUseful-2.0/*.so

movgrab.o: movgrab.c 
	$(CC) $(FLAGS) -c movgrab.c
containerfiles.o: containerfiles.c containerfiles.h
	$(CC) $(FLAGS) -c containerfiles.c

outputfiles.o: outputfiles.c outputfiles.h
	$(CC) $(FLAGS) -c outputfiles.c

common.o: common.c common.h
	$(CC) $(FLAGS) -c common.c

cbsnews.o: cbsnews.c cbsnews.h
	$(CC) $(FLAGS) -c cbsnews.c

dailymotion.o: dailymotion.c dailymotion.h
	$(CC) $(FLAGS) -c dailymotion.c

ehow.o: ehow.c ehow.h
	$(CC) $(FLAGS) -c ehow.c

youtube.o: youtube.c youtube.h
	$(CC) $(FLAGS) -c youtube.c

servicetypes.o: servicetypes.c servicetypes.h
	$(CC) $(FLAGS) -c servicetypes.c

download.o: download.c download.h
	$(CC) $(FLAGS) -c download.c

display.o: display.c display.h
	$(CC) $(FLAGS) -c display.c

extract_text.o: extract_text.c extract_text.h
	$(CC) $(FLAGS) -c extract_text.c


install:
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(libdir)
	$(INSTALL) libmovgrab.a  $(DESTDIR)$(libdir)
	$(INSTALL) libUseful-2.0/libUseful-2.0.a  $(DESTDIR)$(libdir)
	$(INSTALL) movgrab $(DESTDIR)$(bindir)
