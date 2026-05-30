CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c23
LIBS=-L./lib -lraylib -lgdi32 -lwinmm
INCLUDES=-I./include -Isrc

OBJDIR=build/obj
OBJS=$(OBJDIR)/main.o $(OBJDIR)/player.o $(OBJDIR)/bullet.o
GAME=build/bin/space

all: build

build: main.o player.o bullet.o
	$(CC) $(CFLAGS) -o $(GAME) $(OBJS) $(LIBS)

run: build
	./$(GAME)

main.o: src/main.c src/types.h build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/main.c -o $(OBJDIR)/main.o

player.o: src/player.c src/player.h src/types.h build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/player.c -o $(OBJDIR)/player.o

bullet.o: src/bullet.c src/bullet.h src/types.h build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/bullet.c -o $(OBJDIR)/bullet.o

build_dir:
	mkdir -p build/obj build/bin

clean:
	rm -rf build/
