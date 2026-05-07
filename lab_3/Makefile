CC = gcc
CFLAGS = -Wextra -I include
SRCDIR = src
TARGET = exec

# Find all .c files recursively
SOURCES = $(shell find $(SRCDIR) -name "*.c")

# Default target
all: $(TARGET)

# Compile and link directly without object files
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

# Run code
run: $(TARGET)
		./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET)
