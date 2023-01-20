# kgmlayer Makefile
#
#
# This file is part of MPlayer.
#
# MPlayer is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# MPlayer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with MPlayer; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


###### variable declarations #######

# local fallbacks for missing operating system features
SHELL	:= /bin/bash
PREFIX=/usr
export CFLAGS=-I$(PWD)/include
export LDFLAGS=-L$(PWD)/lib 
X11_CFLAGS	="-I$(PWD)/include $(shell pkg-config --cflags x11)"
X11_LIBS 	="$(shell pkg-config --libs x11) "
PKG_CONFIG_PATH_OLD	:=$(PKG_CONFIG_PATH)
PKG_CONFIG_PATH	:=$(PWD)/lib/pkgconfig:/usr/X11R76/lib/pkgconfig
PATH_OLD	:=$(PATH)
PATHNEW	:=$(PWD)/bin:$(PATH_OLD)
PKG_CONFIG_PATH_NEW	:=$(PWD)/lib/pkgconfig:$(PKG_CONFIG_PATH_OLD):/usr/X11R76/lib/pkgconfig






KULINAFILES = $(foreach part, kulina, $(wildcard $(addprefix $(part)/,*.[ch])))
KGLIBFILES = $(foreach part, kglib, $(wildcard $(addprefix $(part)/,*.[ch])))
GMFILES = $(foreach part, OpenSource, $(wildcard $(addprefix $(part)/,*.gz)))
GMFILES += $(foreach part, OpenSource, $(wildcard $(addprefix $(part)/,*.bz2)))
GMFILES += $(foreach part, OpenSource, $(wildcard $(addprefix $(part)/,*.xz)))
MOVFILES = $(foreach part, movgrab, $(wildcard $(addprefix $(part)/,*.[ch])))


%.o: %.S
	$(CC) $(CC_DEPFLAGS) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CC_DEPFLAGS) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CC) $(CC_DEPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.o: %.m
	$(CC) $(CC_DEPFLAGS) $(CFLAGS) -c -o $@ $<

all	: bin/kgmplayer

lib/kgmplayer.a      : $(KULINAFILES) lib/libkulina.a 
	echo "export KULINA=$(PWD)">kulina/config.mak
	echo "export X11_CFLAGS=\"$(X11_CFLAGS)\"">>kulina/config.mak
	echo "export X11_LIBS=\"$(X11_LIBS)\"">>kulina/config.mak
	$(MAKE) -C kulina
	$(MAKE) -C kulina install

lib/libkulina.a	: lib/libgm.a $(KGLIBFILES) 
	echo "PREFIX=$(PWD)">kglib/config.mak
	echo "export X11_CFLAGS=\"$(X11_CFLAGS)\"">>kglib/config.mak
	echo "export X11_LIBS=\"$(X11_LIBS)\"">>kglib/config.mak
	$(MAKE) -C kglib 
	$(MAKE) -C kglib  install

lib/libgm.a	: $(GMFILES)
	echo "PREFIX=$(PWD)">OpenSource/config.mak
	echo "KULINA=$(PWD)">>OpenSource/config.mak
	echo "export X11_CFLAGS=\"$(X11_CFLAGS)\"">>OpenSource/config.mak
	echo "export X11_LIBS=\"$(X11_LIBS)\"">>OpenSource/config.mak
	echo "export PKG_CONFIG_PATH=$(PWD)/lib/pkgconfig:/usr/X11R76/lib/pkgconfig">>OpenSource/config.mak
	$(MAKE) -C OpenSource 
	$(MAKE) -C OpenSource install
lamebuild	:  
		 echo "export KULINA=$(PWD)" > lamebuild
		 echo "export PKG_CONFIG_PATH=$(PWD)/lib/pkgconfig:$(PKG_CONFIG_PATH_OLD)">>lamebuild
	 	 echo "export PATH=\"$(PATHNEW)\"">>lamebuild
		 echo "cd lame" >> lamebuild
		 echo "./rebuild" >> lamebuild
		 chmod +x lamebuild
ffmpegbuild	:  
		 echo "export KULINA=$(PWD)" > ffmpegbuild
		 echo "export PKG_CONFIG_PATH=$(PWD)/lib/pkgconfig:$(PKG_CONFIG_PATH_OLD)">>ffmpegbuild
	 	 echo "export PATH=\"$(PATHNEW)\"">>ffmpegbuild
		 echo "cd ffmpeg" >> ffmpegbuild
		 echo "export CFLAGS=\"$(CFLAGS) $(X11_CFLAGS)\"">>ffmpegbuild
		 echo "export LDFLAGS=\"$(LDFLAGS) $(X11_LIBS)\"">>ffmpegbuild
		 echo "./rebuild" >> ffmpegbuild
		 chmod +x ffmpegbuild
x264build	:  
		 echo "export KULINA=$(PWD)" > x264build
		 echo "export PKG_CONFIG_PATH=$(PWD)/lib/pkgconfig:$(PKG_CONFIG_PATH_OLD)">>x264build
	 	 echo "export PATH=\"$(PATHNEW)\"">>x264build
		 echo "cd x264" >> x264build
		 echo "./rebuild" >> x264build
		 chmod +x x264build
x265build	:  
		 echo "export KULINA=$(PWD)" > x265build
		 echo "export PKG_CONFIG_PATH=$(PWD)/lib/pkgconfig:$(PKG_CONFIG_PATH_OLD)">>x265build
	 	 echo "export PATH=\"$(PATHNEW)\"">>x265build
		 echo "cd x265" >> x265build
		 echo "mkdir build" >> x265build
		 echo "./rebuild" >> x265build
		 chmod +x x265build
build	:  lib/kgmplayer.a 
		 @echo "Dir = $(PWD)"
		 @echo "KULINA=$(PWD)"
		 echo "export KULINA=$(PWD)" > build
		 echo "export PKG_CONFIG_PATH=$(PWD)/lib/pkgconfig:$(PKG_CONFIG_PATH_OLD)">>build
		 echo "export X11_CFLAGS=\"$(X11_CFLAGS)\"">>build
	 	 echo "export X11_LIBS=\"$(X11_LIBS)\"">>build
	 	 echo "export PATH=\"$(PATHNEW)\"">>build
		 echo "cd mplayer" >> build
		 echo "./rebuild" >> build
		 chmod +x build
#bin/kgmplayer	: lib/libgm.a lib/libmovgrab.a lib/libmp3lame.a 
bin/kgmplayer	: lib/libgm.a  lib/libmp3lame.a \
		 lib/libx264.a lib/libx265.a ffmpeg/libavdevice/libavdevice.a \
		  build
#		 rm lib/libx264.so.148
#		 rm lib/libx264.so
		 ./build
grabmak	:  
		 echo "export PKG_CONFIG_PATH=$(PWD)/lib/pkgconfig:$(PKG_CONFIG_PATH_OLD)">grabmak
	 	 echo "export PATH=\"$(PATHNEW)\"">>grabmak
		 echo "cd movgrab" >> grabmak
		 echo "echo \"CFLAGS=$(CFLAGS)\" > config.mak" >> grabmak
		 echo "echo \"LDFLAGS=$(LDFLAGS)\" >> config.mak" >> grabmak
		 echo "echo \"prefix=$(PWD)\" >> config.mak" >> grabmak
		 chmod +x grabmak
lib/libmovgrab.a	: $(MOVFILES) grabmak
		 ./grabmak
		 make -C movgrab
		 make -C movgrab install
lib/libmp3lame.a	:  lamebuild
		 ./lamebuild
		 make -j2 -C lame
		 make -C lame install
lib/libx264.a	:  x264build
		 ./x264build
		 make -j2 -C x264
		 make -C x264 install
lib/libx265.a	:  x265build
		 ./x265build
ffmpeg/libavdevice/libavdevice.a	:  lib/libmp3lame.a \
		 lib/libx264.a lib/libx265.a ffmpegbuild
		 ./ffmpegbuild
		 make -j2 -C ffmpeg
		 rm ffmpeg/ffmpeg
#		 make -C ffmpeg install
install	: bin/kgmplayer
	  install -m 755 -s bin/kgmplayer $(PREFIX)/bin
	  ln -sf $(PREFIX)/bin/kgmplayer $(PREFIX)/bin/kgMplayer
	  ln -sf $(PREFIX)/bin/kgmplayer $(PREFIX)/bin/kgmencoder
	  ln -sf $(PREFIX)/bin/kgmplayer $(PREFIX)/bin/kglame
	  ln -sf $(PREFIX)/bin/kgmplayer $(PREFIX)/bin/kgffmpeg
	  mkdir -p /usr/share/kgmplayer
	  install -m 644 kgmplayer.png /usr/share/kgmplayer
	  install -m 644 empty.srt /usr/share/kgmplayer
	  install -m 644 empty.idx /usr/share/kgmplayer
	  install -m 644 empty.sub /usr/share/kgmplayer
	  install -m 644 kgmplayer.desktop /usr/share/applications
	  install -m 644 kgMplayer.desktop /usr/share/applications

clean	:
	 rm -rf lib/* share/* bin/* man/* build grabmak ffmpegbuild \
	 x264build x265build lamebuild bin/movgrab sbin/* var/* \
	 etc/* include/* mplayer/kgmplayer

	$(MAKE) -C OpenSource clean
	$(MAKE) -C lame clean
	$(MAKE) -C x264  clean
	rm -rf x265/build/*
	$(MAKE) -C ffmpeg clean
	$(MAKE) -C movgrab clean
	$(MAKE) -C kglib clean
	$(MAKE) -C kulina clean
	$(MAKE) -C mplayer clean
