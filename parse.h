#ifndef PARSE_H
#define PARSE_H
#include <stdbool.h>

#define COLORIZED_MASK 0x01

typedef struct program_options
{
    unsigned char flags; // colorized is the final bit in the list
    int rest_args_index;
} program_options;

program_options parse_args(int argc, char **raw_args);
bool colorize_enabled(program_options *po);
#endif