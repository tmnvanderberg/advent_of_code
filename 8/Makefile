# Compiler and flags
CC       := gcc
CFLAGS   := -std=c2x -Wall -Wextra -Werror -O2
LDFLAGS  := 
LDLIBS   := 

# Sources and targets
SRCS     := $(wildcard *.c)
OBJS     := $(SRCS:.c=.o)
TARGET   := main

# Default rule: build the main target
all: $(TARGET)

# Link the final executable from object files
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compile C source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET) *~ 

# Optional: A "phony" target declaration so 'make' doesn't
# confuse these names with real files
.PHONY: all clean
