/**
 * This header file defines the CommandLineArguments structure and the parse_command_line 
 * function prototype for parsing command-line arguments in command-parse.c
 * 
 * To add more command-line arguments, simply add more fields to the CommandLineArguments structure and update 
 * the parse_command_line function accordingly.
 */

#ifndef COMMAND_PARSE_H
#define COMMAND_PARSE_H
#include <getopt.h>

typedef struct {
    char* algorithm;
    char* process_str;
    char* input_file;
    int quantum_time;
} CommandLineArguments;

static struct option long_options[] = {
    {"algorithm", required_argument, 0, 'a'},
    {"processes", required_argument, 0, 'p'},
    {"input", required_argument, 0, 'i'},
    {"quantum", required_argument, 0, 'q'},
    {0}
};

CommandLineArguments* parse_command_line(int, char *[]);
#endif