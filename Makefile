# simple Makefile for compiling small SDL projects
#
# Compiler and flags
CC := gcc
CFLAGS := -ggdb3 -O0 --std=c99 -Wall `sdl2-config --cflags`
LDFLAGS := `sdl2-config --libs` -lSDL2_ttf -lm

# Source files and object files
SRCS := pong.c game.c ball.c player.c font.c line.c
OBJS := $(SRCS:.c=.o)

# Header files
HDRS := game.h ball.h player.h font.h line.h

# Executable name
EXEC := pong

# Default target
all: $(EXEC)

# Linking the final executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Pattern rule for compiling source files into object files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(EXEC) $(OBJS)

# Run the executable (if build succeeds)
run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run
