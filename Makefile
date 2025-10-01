CC = gcc
WCC = x86_64-w64-mingw32-gcc
CFLAGS = -W -Wall -Wextra -g

all : ipcalc hexdump head nl
	
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
