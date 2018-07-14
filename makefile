all: find

main.o:			src/extern.h src/sysdep.h src/main.c
			gcc -c src/main.c
diskfrisk.o:		src/extern.h src/sysdep.h src/diskfrisk.c
			gcc -c src/diskfrisk.c
display.o:		src/extern.h src/display.c
			gcc -c src/display.c
config.o:		src/config.c src/extern.h src/jsmn.c src/jsmn.h
			gcc -c src/config.c
find:			src/main.o src/diskfrisk.o src/display.o src/config.o
			gcc -o bin/find src/main.o src/diskfrisk.o src/display.o src/config.o

