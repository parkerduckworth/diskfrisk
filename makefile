# Makefile for find executable

main.o:			src/extern.h src/prototypes.h src/sysdep.h src/main.c
			gcc -c src/main.c
diskfrisk.o:		src/extern.h src/prototypes.h src/sysdep.h src/diskfrisk.c
			gcc -c src/diskfrisk.c
display.o:		src/extern.h src/prototypes.h src/display.c
			gcc -c src/display.c

find:			src/main.o src/diskfrisk.o src/display.o
			gcc -o bin/find src/main.o src/diskfrisk.o src/display.o
