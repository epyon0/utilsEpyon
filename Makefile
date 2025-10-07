CC = gcc
WCC = x86_64-w64-mingw32-gcc
CFLAGS = -W -Wall -Wextra -g

all : ipcalc hexdump head nl base64 split

base64 : ./src/base64.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/base64 ./src/base64.c
	$(WCC) $(CFLAGS) -o ./bin/base64.exe ./src/base64.c

ipcalc : ./src/ipcalc.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/ipcalc ./src/ipcalc.c
	$(WCC) $(CFLAGS) -o ./bin/ipcalc.exe ./src/ipcalc.c

hexdump : ./src/hexdump.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/hexdump ./src/hexdump.c
	$(WCC) $(CFLAGS) -o ./bin/hexdump.exe ./src/hexdump.c

head : ./src/head.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/head ./src/head.c
	$(WCC) $(CFLAGS) -o ./bin/head.exe ./src/head.c

nl : ./src/nl.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/nl ./src/nl.c
	$(WCC) $(CFLAGS) -o ./bin/nl.exe ./src/nl.c

split : ./src/split.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/split ./src/split.c
	$(WCC) $(CFLAGS) -o ./bin/split.exe ./src/split.c
