# VARIABLES
# 	Same principle as variables in programming languages. They are there to store values.
# Note: 
# [1] Do not change the variable names or delete them.
# [2] You can change the value of the variables if you think it is better.

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Target executable
TARGET = mysh

# Source files
SRC = mysh.c



# RULES
# 	These are commands you can use in the terminal to do things quickly.
# 	Inputting "make" in the command line invokes the default rule in all: (This compiles the code)
#	Inputting "make clean" invokes the rule in clean: (This removes all object files created)
#	Inputting "make run" invokes the ruke in run: (This runs the program ;>)
# Note: 
# [1] You can add new rules, but do not change anything currently here.

# Default rule: build the program
all: $(TARGET)
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean build files
clean:
	rm -f $(TARGET) *.o

# Run the program
run: $(TARGET)
	./$(TARGET)