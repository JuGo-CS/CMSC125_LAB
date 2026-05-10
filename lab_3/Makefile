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

parser: 
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCDIR)/parser.c

TEST_DIR = tests
ACCOUNTS = $(TEST_DIR)/accounts.txt

.PHONY: test-all test-concurrent test-no-conflict test-invalid test-pool-saturation test-deadlock-prevention test-deadlock-detection clean

test-all: test-concurrent test-no-conflict test-invalid test-pool-saturation test-deadlock-prevention test-deadlock-detection

test-concurrent: $(TARGET)
	./$(TARGET) --accounts $(ACCOUNTS) --trace $(TEST_DIR)/test-concurrent.txt --deadlock detection --tick-ms=100 --verbose

test-no-conflict: $(TARGET)
	./$(TARGET) --accounts $(ACCOUNTS) --trace $(TEST_DIR)/test-no-conflict.txt --deadlock detection --tick-ms=100 --verbose

test-invalid: $(TARGET)
	./$(TARGET) --accounts $(ACCOUNTS) --trace $(TEST_DIR)/test-invalid.txt --deadlock detection --tick-ms=100 --verbose

test-pool-saturation: $(TARGET)
	./$(TARGET) --accounts $(ACCOUNTS) --trace $(TEST_DIR)/test-pool-saturation.txt --deadlock detection --tick-ms=100 --verbose

test-deadlock-prevention: $(TARGET)
	./$(TARGET) --accounts $(ACCOUNTS) --trace $(TEST_DIR)/test-deadlock.txt --deadlock prevention --tick-ms=100 --verbose

test-deadlock-detection: $(TARGET)
	./$(TARGET) --accounts $(ACCOUNTS) --trace $(TEST_DIR)/test-deadlock.txt --deadlock detection --tick-ms=100 --verbose


# Clean build artifacts
clean:
	rm -f $(TARGET)
