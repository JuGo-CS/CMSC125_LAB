CC = gcc
CFLAGS = -Wextra -I includes
SRCDIR = src
TARGET = schedsim

# Find all .c files recursively
SOURCES = $(shell find $(SRCDIR) -name "*.c")

# Default target
all: $(TARGET)

# Compile and link directly without object files
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Run Tests 
test-1: $(TARGET)
	./$(TARGET) --algorithm=fcfs --input=workload-1.txt

test-2: $(TARGET)
	./$(TARGET) --algorithm=sjf --input=workload-1.txt