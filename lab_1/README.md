# MYSH - Custom Shell Implementation 
# CMSC 125 Laboratory Exercise: Unix Shell

## Overview

**MYSH** is a custom shell implementation written in C for CMSC 125. It provides a command-line interface that allows users to execute system commands with support for I/O redirection, background process execution, and built-in shell commands.

---

## Features

### Core Functionality
- **Command Execution**: Execute system commands with arguments
- **Input/Output Redirection**: 
  - `<` - Input redirection (read from file)
  - `>` - Output redirection (overwrite file)
  - `>>` - Append redirection (append to file)
- **Background Process Execution**: Use `&` to run commands in the background
- **Built-in Commands**:
  - `pwd` - Print working directory
  - `cd <directory>` - Change directory
  - `exit` - Exit the shell
- **Background Job Management**: Automatically tracks and cleans up finished background jobs

---

## Project Architecture

### File Structure

```
lab_1/
├── mysh.h              # Header file with data structures and constants
├── mysh.c              # Main shell loop and entry point
├── parser.c            # Input parsing and tokenization
├── interpreter.c       # Command execution and I/O handling
├── Makefile            # Build configuration
└── README.md           # This file
```

### Component Breakdown

#### **mysh.h** - Header & Data Structures
- Defines the `Command` struct containing:
  - `command`: The command name to execute
  - `args[]`: Array of command arguments
  - `input_file`: File for input redirection
  - `output_file`: File for output redirection
  - `append`: Boolean flag for >> vs > mode
  - `background`: Boolean flag for background execution
- Defines constants:
  - `MAX_INPUT_SIZE`: Maximum input buffer size (1024 characters)
  - `MAX_TOKEN_ARRAY_SIZE`: Maximum tokens per command (256)
  - `MAX_BG_JOBS`: Maximum background jobs (128)

#### **mysh.c** - Main Shell Loop
- Implements the main shell REPL (Read-Eval-Print Loop)
- Prints `mysh>` prompt
- Coordinates between parser and interpreter
- Memory management for user input
- Helper function `print_command()` for debugging command structures

#### **parser.c** - Input Processing
**Key Functions:**
- `get_input()`: Reads user input from stdin with error handling
- `get_tokens()`: Tokenizes input by spaces and tabs
- `get_command()`: Parses tokens into a Command struct with validation
- `contains_string()`: Utility to detect special symbols

**Parsing Logic:**
```
Input Format: command [args...] [redirections] [background]
Example:      ls      -la         > output.txt  &
```

**Validation:**
- Only one input redirection per command
- Only one output/append redirection per command
- Redirection symbols must be followed by a filename
- Background indicator (`&`) must be the last symbol
- No extra arguments allowed after redirections

#### **interpreter.c** - Command Execution
**Key Functions:**
- `handles_builtin()`: Handles built-in commands (pwd, cd, exit)
- `interpreter()`: Main execution logic with process management
- `cleanup_background_jobs()`: Monitors and reaps finished background processes

**Execution Flow:**

1. **Built-in Command Check**: If command is built-in (pwd, cd, exit), handle directly in parent process
2. **Process Forking**: Fork a child process for external commands
3. **I/O Redirection** (in child process):
   - Input redirection: Open file for reading, redirect to STDIN
   - Output redirection: Open file for writing/appending, redirect to STDOUT
4. **Command Execution**: Use `execvp()` to replace child process with requested program
5. **Parent Process**:
   - **Foreground**: Wait for child to complete with `waitpid()`
   - **Background**: Add to tracking array and continue immediately

**Background Job Management:**
- Tracks PIDs of background processes in `pids_bg_job[]` array
- Maintains `active_bg_job` count
- `cleanup_background_jobs()` called at each prompt to reap finished processes
- Uses `waitpid()` with `WNOHANG` flag to avoid blocking

---

## Building & Running

### Build
```bash
make
```
Compiles the source files using gcc with `-Wall -Wextra` flags.

### Run
```bash
make run
```
Executes the compiled shell binary.

### Clean
```bash
make clean
```
Removes the compiled executable and object files.

---

## Usage Examples

### Basic Command Execution
```
mysh> ls -la
mysh> echo "Hello, World!"
```

### Input Redirection
```
mysh> sort < unsorted.txt
mysh> cat < input.txt
```

### Output Redirection
```
mysh> ls -la > output.txt        # Overwrite file
mysh> echo "hello" >> output.txt # Append to file
```

### Background Execution
```
mysh> sleep 30 &
    > [1] Started background job: sleep 30 (PID: 12345)
mysh> 
    > Background job (PID: 12345) finished.
```

### Built-in Commands
```
mysh> pwd
    > /mnt/c/CODING_FILE/Vscode/3rdYear-2ndSem/cmsc_125

mysh> cd ..
    > cd: Directory was changed successfully!

mysh> exit
    > mysh> exiting shell...
```

### Combined Features
```
mysh> sort < input.txt > output.txt &
    > [1] Started background job: sort (PID: 12346)
```

---

## Constants & Limits

| Constant | Value | Purpose |
|----------|-------|---------|
| `MAX_INPUT_SIZE` | 1024 | Maximum characters per user input |
| `MAX_TOKEN_ARRAY_SIZE` | 256 | Maximum tokens/arguments per command |
| `MAX_BG_JOBS` | 128 | Maximum concurrent background jobs |

---

## Error Handling

The shell handles various error conditions:
- **Input Size Exceeded**: Error message and input discarded
- **Token Count Exceeded**: Error message and tokens discarded
- **Memory Allocation Failure**: Error message and exit
- **Redirection Errors**:
  - Multiple input redirections
  - Multiple output redirections
  - Missing file after redirection symbol
  - Extra arguments after redirections
  - Background symbol not at end of command
- **File I/O Errors**: Caught during redirection setup
- **Process Errors**: Fork failures, exec failures with appropriate messages

---

## Special Symbols

| Symbol | Purpose | Example |
|--------|---------|---------|
| `<` | Input redirection | `command < input.txt` |
| `>` | Output redirection (overwrite) | `command > output.txt` |
| `>>` | Output redirection (append) | `command >> output.txt` |
| `&` | Background execution | `command &` |

---

## Implementation Notes

- **No Piping**: Pipe operator (`|`) is not supported
- **Single Redirections**: Only one input and one output redirection per command
- **Process Management**: Uses `fork()`, `execvp()`, and `waitpid()` for process handling
- **Signal Handling**: Not implemented; background jobs may require manual cleanup
- **Job Control**: Basic job tracking without full tcsh-style job control

---

## Compilation Details

**Compiler**: GCC with flags `-Wall -Wextra`
- `-Wall`: Enable all common warnings
- `-Wextra`: Enable extra warnings

**Source Files**:
- Main compilation unit: `mysh.c`
- Includes: `parser.c` and `interpreter.c` directly (not separate object files)

---

## Author Information

**Course**: CMSC 125 - Operating Systems (3rd Year, 2nd Semester)
**Assignment**: Lab 1 - Shell Implementation

---
