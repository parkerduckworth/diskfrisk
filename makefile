all: bin/find

src/%.o: src/%.c
	gcc -g -c $< -o $@

src/main.o: src/extern.h src/sysdep.h
src/diskfrisk.o: src/extern.h src/sysdep.h
src/display.o: src/extern.h
config.o: src/extern.h src/jsmn.c src/jsmn.h

bin/find: src/main.o src/diskfrisk.o src/display.o src/config.o
	gcc $^ -o $@