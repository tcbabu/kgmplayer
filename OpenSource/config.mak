#! /bin/bash 
PREFIX=/home/kulina/BUILD/kgmplayer
KULINA=/home/kulina/BUILD/kgmplayer
export CFLAGS="-fPIC -fpic -fPIC -I/home/kulina/BUILD/kgmplayer/include -I/home/kulina/BUILD/kgmplayer/include/freetype2"
export CPPFLAGS="-fPIC -fpic -fPIC -I/home/kulina/BUILD/kgmplayer/include -I/home/kulina/BUILD/kgmplayer/include/freetype2"
export LDFLAGS="-L/lib -L/home/kulina/BUILD/kgmplayer/lib "
export X11_CFLAGS="-I/home/kulina/BUILD/kgmplayer/include "
export X11_LIBS="-lX11 "
export PKG_CONFIG_PATH=/home/kulina/BUILD/kgmplayer/lib/pkgconfig:/usr/X11R76/lib/pkgconfig:/home/kulina/BUILD/kgmplayer/lib/pkgconfig:/usr/X11R76/lib/pkgconfig
export LD_LIBRARY_PATH=/home/kulina/BUILD/kgmplayer/lib:/usr/X11R76/lib:
export PATH=/home/kulina/BUILD/kgmplayer/bin:/home/kulina/GPES/bin:/home/kulina/.local/bin:/opt/rustc/bin:/bin:/usr/Kulina/bin:/opt/qt5/bin:/usr/bin
