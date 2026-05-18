CC = gcc
CFLAGS = -Wextra -I include -Wall -O2 -pthread
SRCDIR = src
TARGET = bankdb


# Find all .c files recursively
SOURCES = $(shell find $(SRCDIR) -name "*.c")

# Default target
all: $(TARGET)

# Compile and link directly without object files
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean: 
	rm -f $(TARGET)

# Test Runs 
test-all: test-concurrent test-no-conflict test-invalid test-pool-saturation test-deadlock-prevention test-deadlock-detection

test-concurrent: $(TARGET)
	./$(TARGET) --accounts=accounts.txt --trace=test-concurrent.txt --deadlock detection --tick-ms=100 --verbose

test-no-conflict: $(TARGET)
	./$(TARGET) --accounts=accounts.txt --trace=test-no-conflict.txt --deadlock detection --tick-ms=100 --verbose

test-invalid: $(TARGET)
	./$(TARGET) --accounts=accounts.txt --trace=test-invalid.txt --deadlock detection --tick-ms=100 --verbose

test-pool-saturation: $(TARGET)
	./$(TARGET) --accounts=accounts.txt --trace=test-pool-saturation.txt --deadlock detection --tick-ms=100 --verbose

test-deadlock-prevention: $(TARGET)
	./$(TARGET) --accounts=accounts.txt --trace=test-deadlock.txt --deadlock prevention --tick-ms=100 --verbose

test-deadlock-detection: $(TARGET)
	./$(TARGET) --accounts=accounts.txt --trace=test-deadlock.txt --deadlock detection --tick-ms=100 --verbose

