include config.mak
libgm.a	: rebuild config.mak
	 cat config.mak rebuild > build
	 chmod +x build
	 ./build 
install	: libgm.a
	  cp libgm.a $(KULINA)/lib
clean	:
	  rm -f libgm.a
