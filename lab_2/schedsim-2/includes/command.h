#ifndef COMMAND_H
#define COMMAND_H

#include <getopt.h>

typedef struct {
    char* algorithm;
    char* process_str;
    char* input_file;
    int quantum_time;
    char* mlfq_config_file;
    int compare_mode;
} CommandLineArguments;

static struct option long_options[] = {
    {"algorithm", required_argument, 0, 'a'},
    {"processes", required_argument, 0, 'p'},
    {"input", required_argument, 0, 'i'},
    {"quantum", required_argument, 0, 'q'},
    {"mlfq-config", required_argument, 0, 'm'},
    {"compare", no_argument, 0, 'c'},
    {0}
};

CommandLineArguments* parse_command_line(int, char *[]);
#endif
