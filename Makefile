DEBUG ?= 0
PLATFORM ?= windows

ifeq ($(DEBUG), 1)
	CFLAGS := -Wall -Wextra -Wpedantic -std=c23 -g -DDEBUG
else
	CFLAGS := -Wall -Wextra -Wpedantic -std=c23
endif

CC=gcc
INCLUDES=-I./include -Isrc

ifeq ($(PLATFORM), windows)
    LIBS=-L./lib/win -lraylib -lgdi32 -lwinmm
else ifeq ($(PLATFORM), linux)
    LIBS=-L./lib/linux -lraylib -lm -lX11
else
    $(error Unknown platform: $(PLATFORM))
endif

COMMON=src/constants.h src/types.h

OBJDIR=build/obj
OBJS=$(OBJDIR)/main.o $(OBJDIR)/player.o $(OBJDIR)/bullet.o $(OBJDIR)/enemy.o \
     $(OBJDIR)/utils.o $(OBJDIR)/debug.o $(OBJDIR)/collision.o
GAME=build/bin/space

all: build

build: main.o player.o bullet.o enemy.o utils.o debug.o collision.o
	$(CC) $(CFLAGS) -o $(GAME) $(OBJS) $(LIBS)

run: build
	./$(GAME)

main.o: src/main.c $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/main.c -o $(OBJDIR)/main.o

player.o: src/player.c src/player.h $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/player.c -o $(OBJDIR)/player.o

bullet.o: src/bullet.c src/bullet.h $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/bullet.c -o $(OBJDIR)/bullet.o

enemy.o: src/enemy.c src/enemy.h $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/enemy.c -o $(OBJDIR)/enemy.o

utils.o: src/utils.c src/utils.h $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/utils.c -o $(OBJDIR)/utils.o

debug.o: src/debug.c src/debug.h build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/debug.c -o $(OBJDIR)/debug.o

collision.o: src/collision.c src/collision.h $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/collision.c -o $(OBJDIR)/collision.o

build_dir:
	mkdir -p build/obj build/bin

clean:
	rm -rf build/
