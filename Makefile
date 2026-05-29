CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c23
LIBS=-L./lib -lraylib -lgdi32 -lwinmm
INCLUDES=-I./include

OBJDIR=build/obj
OBJS=$(OBJDIR)/main.o $(OBJDIR)/player.o $(OBJDIR)/bullet.o
GAME=build/bin/space

all: build

build: main.o player.o bullet.o
	$(CC) $(CFLAGS) -o $(GAME) $(OBJS) $(INCLUDES) $(LIBS)

run: build
	./$(GAME)

main.o: src/main.c src/constants.h build_dir
	$(CC) $(CFLAGS) -o build/obj/main.o -c src/main.c $(INCLUDES)

player.o: src/player.c src/constants.h build_dir
	$(CC) $(CFLAGS) -o build/obj/player.o -c src/player.c $(INCLUDES)

bullet.o: src/bullet.c src/constants.h build_dir
	$(CC) $(CFLAGS) -o build/obj/bullet.o -c src/bullet.c $(INCLUDES)

build_dir: 
	mkdir -p build/obj build/bin

clean: build_dir
	rm -r build/*
