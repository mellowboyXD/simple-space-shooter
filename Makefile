DEBUG ?= 0
PLATFORM := $(shell uname)

ifeq ($(DEBUG), 1)
	CFLAGS := -Wall -Wextra -Wpedantic -std=c23 -g -DDEBUG
	GAME := build/bin/space_d
else
	CFLAGS := -Wall -Wextra -Wpedantic -std=c23
	GAME := build/bin/space
endif

CC := gcc
INCLUDES := -I./include -I./src

ifeq ($(PLATFORM), Windows)
	LIBS := -L./lib/win -lraylib -lgdi32 -lwinmm
else ifeq ($(PLATFORM), Linux)
	LIBS := -L./lib/linux -lraylib -lm -lX11
else
    $(error Unknown platform: $(PLATFORM))
endif

COMMON := src/constants.h src/types.h src/components.h

OBJDIR := build/obj
OBJS := $(OBJDIR)/main.o $(OBJDIR)/utils.o $(OBJDIR)/debug.o 		\
	 $(OBJDIR)/entity_manager.o $(OBJDIR)/component_pool.o 			\
	 $(OBJDIR)/component_manager.o $(OBJDIR)/systems/system.o 		\
	 $(OBJDIR)/systems/system_manager.o $(OBJDIR)/coordinator.o		\
	 $(OBJDIR)/systems/movement_system.o $(OBJDIR)/systems/render_system.o

TARGETS := main.o utils.o debug.o entity_manager.o component_pool.o 	\
		  component_manager.o system.o system_manager.o coordinator.o 	\
		  movement_system.o render_system.o

all: build

build: $(TARGETS)
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
	$(CC) $(CFLAGS) $(INCLUDES) -c src/entity_manager.c -o $(OBJDIR)/entity_manager.o

component_pool.o: src/component_pool.h src/component_pool.c $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/component_pool.c -o $(OBJDIR)/component_pool.o

component_manager.o: src/component_manager.h src/component_manager.c $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/component_manager.c -o $(OBJDIR)/component_manager.o

system.o: src/systems/system.h src/systems/system.c $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/systems/system.c -o $(OBJDIR)/systems/system.o

system_manager.o: src/systems/system_manager.h src/systems/system_manager.c $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/systems/system_manager.c -o $(OBJDIR)/systems/system_manager.o

coordinator.o: src/coordinator.h src/coordinator.c $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/coordinator.c -o $(OBJDIR)/coordinator.o

movement_system.o: src/systems/movement_system.h src/systems/movement_system.c $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/systems/movement_system.c -o $(OBJDIR)/systems/movement_system.o

render_system.o: src/systems/render_system.h src/systems/render_system.c $(COMMON) build_dir
	$(CC) $(CFLAGS) $(INCLUDES) -c src/systems/render_system.c -o $(OBJDIR)/systems/render_system.o

build_dir:
	mkdir -p build/obj/systems build/bin

clean:
	rm -rf build/
