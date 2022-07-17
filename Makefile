# simple Makefile for compiling small SDL projects

# set the compiler
CC := gcc

# set the compiler flags
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_ttf -lm
# add header files here
HDRS :=

# add source files here
SRCS := pong.c game.c ball.c player.c font.c line.c

# generate names of object files
OBJS := $(SRCS:.c=.o)

# name of executable
EXEC := pong

# default recipe
all: $(EXEC)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

run: ./pong
	make && ./pong

.PHONY: all clean
