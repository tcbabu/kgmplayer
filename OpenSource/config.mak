#! /bin/bash 
PREFIX=/mnt/kulina/BUILD/kgmplayer
KULINA=/mnt/kulina/BUILD/kgmplayer
export X11_CFLAGS="-I/mnt/kulina/BUILD/kgmplayer/include "
export X11_LIBS="-lX11 "
export PKG_CONFIG_PATH=/mnt/kulina/BUILD/kgmplayer/lib/pkgconfig:/usr/X11R76/lib/pkgconfig
