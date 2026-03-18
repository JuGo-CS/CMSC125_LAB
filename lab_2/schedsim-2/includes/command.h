#ifndef COMMAND_H
#define COMMAND_H

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
