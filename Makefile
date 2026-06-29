DEBUG ?= 0
PLATFORM ?= windows

ifeq ($(DEBUG), 1)
	CFLAGS := -Wall -Wextra -Wpedantic -std=c23 -g -DDEBUG
else
	CFLAGS := -Wall -Wextra -Wpedantic -std=c23
endif

CC=gcc
INCLUDES=-I./include -I./src

ifeq ($(PLATFORM), windows)
    LIBS=-L./lib/win -lraylib -lgdi32 -lwinmm
else ifeq ($(PLATFORM), linux)
    LIBS=-L./lib/linux -lraylib -lm -lX11
else
    $(error Unknown platform: $(PLATFORM))
endif

COMMON=src/constants.h src/types.h src/components.h

OBJDIR=build/obj
OBJS=$(OBJDIR)/main.o $(OBJDIR)/utils.o $(OBJDIR)/debug.o \
	 $(OBJDIR)/entity_manager.o

GAME=build/bin/space

all: build

build: main.o utils.o debug.o entity_manager.o
	$(CC) $(CFLAGS) -o $(GAME) $(OBJS) $(LIBS)

run: build
	./$(GAME)

main.o: src/main.c $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/main.c -o $(OBJDIR)/main.o

utils.o: src/utils.c src/utils.h $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/utils.c -o $(OBJDIR)/utils.o

debug.o: src/debug.c src/debug.h build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/debug.c -o $(OBJDIR)/debug.o

entity_manager.o: src/entity_manager.h src/entity_manager.c $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/entity_manager.c -o \ 
	$(OBJDIR)/entity_manager.o

build_dir:
	mkdir -p build/obj build/bin

clean:
	rm -rf build/
