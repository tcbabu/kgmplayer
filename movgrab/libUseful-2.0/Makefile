include config.mak
CC = gcc
VERSION = 0.0.1
LIBS = $(LDFLAGS) -lssl -lcrypto -lz 
FLAGS=$(CFLAGS) -fPIC -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DSTDC_HEADERS=1 -DHAVE_LIBZ=1 -D_LARGEFILE64_SOURCE=1 -D_FILE_OFFSET_BITS=64 -DHAVE_LIBSSL=1 -DHAVE_LIBCRYPTO=1 -DHAVE_EVP_BF_CBC=1 -DHAVE_EVP_RC2_CBC=1 -DHAVE_EVP_RC4=1 -DHAVE_EVP_DES_CBC=1 -DHAVE_EVP_DESX_CBC=1 -DHAVE_EVP_CAST5_CBC=1 -DHAVE_EVP_IDEA_CBC=1 -DHAVE_EVP_AES_128_CBC=1 -DHAVE_EVP_AES_256_CBC=1 -DUSE_OPENSSL_ADD_ALL_ALGORITHMS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_OSS=1 
OBJ=string.o list.o socket.o unix_socket.o file.o tar.o FileSystem.o GeneralFunctions.o DataProcessing.o EncryptedFiles.o ConnectManager.o sound.o pty.o Log.o http.o inet.o expect.o base64.o crc32.o md5c.o sha1.o sha2.o Hash.o Compression.o oauth.o libsettings.o Vars.o MathExpr.o Time.o Markup.o SpawnPrograms.o PatternMatch.o ParseURL.o ConnectionChain.o


all: $(OBJ)
	#ld -i -o libUseful-2.0.a $(OBJ)
	ar rcs libUseful-2.0.a $(OBJ)

string.o: string.h string.c
	$(CC) $(FLAGS) -c string.c

list.o: list.h list.c
	$(CC) $(FLAGS) -c list.c

socket.o: socket.h socket.c
	$(CC) $(FLAGS) -c socket.c

sound.o: sound.h sound.c
	$(CC) $(FLAGS) -c sound.c

pty.o: pty.h pty.c
	$(CC) $(FLAGS) -c pty.c

http.o: http.h http.c
	$(CC) $(FLAGS) -c http.c

file.o: file.h file.c
	$(CC) $(FLAGS) -c file.c

tar.o: tar.h tar.c
	$(CC) $(FLAGS) -c tar.c

Log.o: Log.h Log.c
	$(CC) $(FLAGS) -c Log.c

unix_socket.o: unix_socket.h unix_socket.c
	$(CC) $(FLAGS) -c unix_socket.c

PatternMatch.o: PatternMatch.h PatternMatch.c
	$(CC) $(FLAGS) -c PatternMatch.c

FileSystem.o: FileSystem.h FileSystem.c
	$(CC) $(FLAGS) -c FileSystem.c

MathExpr.o: MathExpr.h MathExpr.c
	$(CC) $(FLAGS) -c MathExpr.c

Time.o: Time.h Time.c
	$(CC) $(FLAGS) -c Time.c

Markup.o: Markup.h Markup.c
	$(CC) $(FLAGS) -c Markup.c

ParseURL.o: ParseURL.h ParseURL.c
	$(CC) $(FLAGS) -c ParseURL.c

inet.o: inet.h inet.c
	$(CC) $(FLAGS) -c inet.c

expect.o: expect.h expect.c
	$(CC) $(FLAGS) -c expect.c

GeneralFunctions.o: GeneralFunctions.h GeneralFunctions.c
	$(CC) $(FLAGS) -c GeneralFunctions.c

DataProcessing.o: DataProcessing.h DataProcessing.c
	$(CC) $(FLAGS) -c DataProcessing.c

EncryptedFiles.o: EncryptedFiles.h EncryptedFiles.c
	$(CC) $(FLAGS) -c EncryptedFiles.c

Hash.o: Hash.h Hash.c
	$(CC) $(FLAGS) -c Hash.c

Compression.o: Compression.h Compression.c
	$(CC) $(FLAGS) -c Compression.c

ConnectionChain.o: ConnectionChain.h ConnectionChain.c
	$(CC) $(FLAGS) -c ConnectionChain.c

ConnectManager.o: ConnectManager.h ConnectManager.c
	$(CC) $(FLAGS) -c ConnectManager.c

base64.o: base64.c base64.h
	$(CC) $(FLAGS) -c base64.c

crc32.o: crc32.c crc32.h
	$(CC) $(FLAGS) -c crc32.c

md5c.o: md5c.c md5-global.h md5.h
	$(CC) $(FLAGS) -c md5c.c

sha1.o: sha1.c sha1.h
	$(CC) $(FLAGS) -c sha1.c

sha2.o: sha2.c sha2.h
	$(CC) $(FLAGS) -c sha2.c

oauth.o: oauth.c oauth.h
	$(CC) $(FLAGS) -c oauth.c

Vars.o: Vars.c Vars.h
	$(CC) $(FLAGS) -c Vars.c

libsettings.o: libsettings.c libsettings.h
	$(CC) $(FLAGS) -c libsettings.c

SpawnPrograms.o: SpawnPrograms.c SpawnPrograms.h
	$(CC) $(FLAGS) -c SpawnPrograms.c


clean:
	@rm -f *.o *.so *.a

install:
	@ln -s libUseful-2.0.so libUseful.so
	@ln -s libUseful-2.0.a libUseful.a
	@cp *.so *.a $(prefix)/lib ; mkdir $(prefix)/include/libUseful-$(VERSION) ; cp *.h $(prefix)/include/libUseful-$(VERSION)
