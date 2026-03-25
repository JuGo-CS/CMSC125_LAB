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
fcfs_input-1: $(TARGET)
	./$(TARGET) --algorithm=fcfs --input=workload-1.txt

fcfs_input-2: $(TARGET)
	./$(TARGET) --algorithm=fcfs --input=workload-2.txt

fcfs_input-3: $(TARGET)
	./$(TARGET) --algorithm=fcfs --input=workload-3.txt



sjf_input-1: $(TARGET)
	./$(TARGET) --algorithm=sjf --input=workload-1.txt

sjf_input-2: $(TARGET)
	./$(TARGET) --algorithm=sjf --input=workload-2.txt

sjf_input-3: $(TARGET)
	./$(TARGET) --algorithm=sjf --input=workload-3.txt



rr_input-1: $(TARGET)
	./$(TARGET) --algorithm=rr --quantum=5 --input=workload-1.txt

rr_input-2: $(TARGET)
	./$(TARGET) --algorithm=rr --quantum=5 --input=workload-2.txt

rr_input-3: $(TARGET)
	./$(TARGET) --algorithm=rr --quantum=30 --input=workload-3.txt


stcf_input-1: $(TARGET)
	./$(TARGET) --algorithm=stcf --input=workload-3.txt

stcf_input-2: $(TARGET)
	./$(TARGET) --algorithm=stcf --quantum=5 --input=workload-2.txt

stcf_input-3: $(TARGET)
	./$(TARGET) --algorithm=stcf --quantum=30 --input=workload-3.txt


# MLFQ Tests
mlfq_input-1: $(TARGET)
	./$(TARGET) --algorithm=mlfq --mlfq-config=mlfq_config.txt --input=workload-1.txt

mlfq_input-2: $(TARGET)
	./$(TARGET) --algorithm=mlfq --mlfq-config=mlfq_config.txt --input=workload-2.txt

mlfq_input-3: $(TARGET)
	./$(TARGET) --algorithm=mlfq --mlfq-config=mlfq_config.txt --input=workload-3.txt
	

