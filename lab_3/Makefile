CC = gcc
CFLAGS = -Wextra -I include -Wall -O2 -pthread
DBFLAGS = -Wall -Wextra -g -fsanitize=thread -pthread -I include

SRCDIR = src
TARGET = bankdb 
DBTAR   = bankdb_debug

# Find all .c files recursively
SOURCES = $(shell find $(SRCDIR) -name "*.c")

TEST_DIR = tests
ACCOUNTS = $(TEST_DIR)/accounts.txt
 
.PHONY: all debug clean test test-all \
        test-no-conflict test-concurrent \
        test-deadlock test-invalid test-pool-saturation

# Default target
all: $(TARGET)

# Compile and link directly without object files
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

# Debug target with ThreadSanitizer instrumentation
debug: $(DBTAR)

$(DBTAR): $(SOURCES)
	$(CC) $(DBFLAGS) -o $@ $^
	

# Parser target
parser: 
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCDIR)/parser.c

# Test configurations
TEST_DIR = tests
ACCOUNTS = $(TEST_DIR)/accounts.txt

# Test suite invoking five distinct scenarios
test: test-all
 
test-all: test-no-conflict test-concurrent test-deadlock \
          test-invalid test-pool-saturation
 
test-no-conflict: $(TARGET)
	@echo "=== Test: No Conflict ==="
	./$(TARGET) --accounts $(ACCOUNTS) \
	            --trace $(TEST_DIR)/test-no-conflict.txt \
	            --deadlock prevention --tick-ms 100 --verbose
 
test-concurrent: $(TARGET)
	@echo "=== Test: Concurrent Operations ==="
	./$(TARGET) --accounts $(ACCOUNTS) \
	            --trace $(TEST_DIR)/test-concurrent.txt \
	            --deadlock prevention --tick-ms 50 --verbose
 
test-deadlock: $(TARGET)
	@echo "=== Test: Deadlock Prevention ==="
	./$(TARGET) --accounts $(ACCOUNTS) \
	            --trace $(TEST_DIR)/test-deadlock.txt \
	            --deadlock prevention --tick-ms 50 --verbose
 
test-invalid: $(TARGET)
	@echo "=== Test: Invalid Inputs ==="
	./$(TARGET) --accounts $(ACCOUNTS) \
	            --trace $(TEST_DIR)/test-invalid.txt \
	            --deadlock prevention --tick-ms 200 --verbose
 
test-pool-saturation: $(TARGET)
	@echo "=== Test: Pool Saturation ==="
	./$(TARGET) --accounts $(ACCOUNTS) \
	            --trace $(TEST_DIR)/test-pool-saturation.txt \
	            --deadlock prevention --tick-ms 50 --verbose
	
clean:
	rm -f $(TARGET)