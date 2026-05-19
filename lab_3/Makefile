CC = gcc
CFLAGS = -Wextra -I include -Wall -O2 -pthread
SRCDIR = src
TARGET = bankdb  # Changed to bankdb to match your architecture

# Find all .c files recursively
SOURCES = $(shell find $(SRCDIR) -name "*.c")

# Default target
all: $(TARGET)

# Compile and link directly without object files
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

# Debug target with ThreadSanitizer instrumentation
debug: $(SOURCES)
	$(CC) $(CFLAGS) -g -fsanitize=thread -pthread -o $(TARGET) $^
	./$(TARGET) --accounts accounts.txt --trace trace_basic.txt

# Parser target
parser: 
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCDIR)/parser.c

# Test configurations
TEST_DIR = tests
ACCOUNTS = $(TEST_DIR)/accounts.txt

# Test suite invoking five distinct scenarios
test: $(TARGET)
	@echo "=== Running Test Case 1: No Conflict ==="
	./$(TARGET) --accounts $(ACCOUNTS) --trace $(TEST_DIR)/test-no-conflict.txt --tick-ms 100 --deadlock detection --verbose
	
	@echo "=== Running Test Case 2: Concurrent Operations ==="
	./$(TARGET) --accounts $(ACCOUNTS) --trace $(TEST_DIR)/test-concurrent.txt --tick-ms 50 --deadlock detection --verbose
	
	@echo "=== Running Test Case 3: Deadlock Scenario (DETECTION) ==="
	./$(TARGET) --accounts $(ACCOUNTS) --trace $(TEST_DIR)/test-deadlock.txt --tick-ms 50 --deadlock detection --verbose
	
	@echo "=== Running Test Case 4: Deadlock Scenario (PREVENTION)==="
	./$(TARGET) --accounts $(ACCOUNTS) --trace $(TEST_DIR)/test-deadlock.txt --tick-ms 50 --deadlock prevention --verbose
	
	@echo "=== Running Test Case 5: Pool Saturation ==="
	./$(TARGET) --accounts $(ACCOUNTS) --trace $(TEST_DIR)/test-pool-saturation.txt --tick-ms 50 --deadlock prevention --verbose
	
	@echo "=== Running Test Case 6: Invalid Inputs ==="
	./$(TARGET) --accounts $(ACCOUNTS) --trace $(TEST_DIR)/test-invalid.txt --tick-ms 200 --deadlock detection --verbose
	
clean:
	rm -f $(TARGET)