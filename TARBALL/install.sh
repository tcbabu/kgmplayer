#!/bin/bash
install -m 755 kgmplayer /usr/bin/
ln -sf /usr/bin/kgmplayer /usr/bin/kgMplayer
ln -sf /usr/bin/kgmplayer /usr/bin/kglame
ln -sf /usr/bin/kgmplayer /usr/bin/kgffmpeg
ln -sf /usr/bin/kgmplayer /usr/bin/kgmencoder
mkdir -p /usr/share/kgmplayer
mkdir -p /usr/share/kgmplayer/lib
install -m 644 kgmplayer.png /usr/share/kgmplayer
install -m 644 empty.srt /usr/share/kgmplayer
install -m 644 empty.idx /usr/share/kgmplayer
install -m 644 empty.sub /usr/share/kgmplayer
install -m 644 kgmplayer.desktop /usr/share/applications
install -m 644 kgMplayer.desktop /usr/share/applications
#not always good
#cp  lib/* /usr/share/kgmplayer/lib/

