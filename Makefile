CC = gcc
WCC = x86_64-w64-mingw32-gcc
CFLAGS = -W -Wall -Wextra -g

install : ./src/ipcalc.c ./lib/libEpyon/misc.h
	$(CC)  $(CFLAGS) -o ./bin/ipcalc ./src/ipcalc.c
	$(WCC) $(CFLAGS) -o ./bin/ipcalc.exe ./src/ipcalc.c

	$(CC)  $(CFLAGS) -o ./bin/hexdump ./src/hexdump.c
	$(WCC) $(CFLAGS) -o ./bin/hexdump.exe ./src/hexdump.c

#$(CC)  $(CFLAGS) -o ./bin/head ./src/head.c
#$(WCC) $(CFLAGS) -o ./bin/head.exe ./src/head.c
