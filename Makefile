CC = gcc
WCC = x86_64-w64-mingw32-gcc
CFLAGS = -W -Wall -Wextra -g

all : ipcalc hexdump head nl base64 split tail wc zs

lib :
	cd ./lib/ && git clone https://github.com/epyon0/libEpyon.git
	cd ./lib/libEpyon/ && git pull
lib-update :
	cd ./lib/libEpyon/ && git pull

base64 : ./src/base64.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/$@     ./src/$@.c
	$(WCC) $(CFLAGS) -o ./bin/$@.exe ./src/$@.c

ipcalc : ./src/ipcalc.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/$@     ./src/$@.c
	$(WCC) $(CFLAGS) -o ./bin/$@.exe ./src/$@.c

hexdump : ./src/hexdump.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/$@     ./src/$@.c
	$(WCC) $(CFLAGS) -o ./bin/$@.exe ./src/$@.c

head : ./src/head.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/$@     ./src/$@.c
	$(WCC) $(CFLAGS) -o ./bin/$@.exe ./src/$@.c

nl : ./src/nl.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/$@     ./src/$@.c
	$(WCC) $(CFLAGS) -o ./bin/$@.exe ./src/$@.c

split : ./src/split.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/$@     ./src/$@.c
	$(WCC) $(CFLAGS) -o ./bin/$@.exe ./src/$@.c

tail : ./src/tail.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/$@     ./src/$@.c
	$(WCC) $(CFLAGS) -o ./bin/$@.exe ./src/$@.c

wc : ./src/wc.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/$@     ./src/$@.c
	$(WCC) $(CFLAGS) -o ./bin/$@.exe ./src/$@.c

zls : ./src/zls.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/$@     ./src/$@.c
	$(WCC) $(CFLAGS) -o ./bin/$@.exe ./src/$@.c

clean :
	rm -rf ./lib/*
	rm ./bin/*
