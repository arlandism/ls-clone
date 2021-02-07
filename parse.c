/*
 * This file implements CLI flag parsing for user-enabled options.
 * It will parse the raw arguments supplied to the program until
 * it reaches a char* beginning with something other than a "-" OR
 * a "-" followed by a non-flag character
 * (e.g. -I is not supported so could be safely used.)
 *
 * Once the terminal condition above is met, it will assume
 * that the remaining arguments are meant for the underlying executable.
 * This implies:
 * 1) Arguments for the underlying executable can't conflict with flags.
 * 2) Flags must precede arguments for the executable.
 *
 * The function returns a structure containing:
 * 1) A byte telling us which flags are enabled/disabled.
 * 2) The index of the first non-flag supported argument.
 *    This is best illustrated by example. If the program is given 2 arguments
 * ./prog -g something
 * Then the structure will contain a bit set for the '-g' option, along with
 * an int 2.
 * With no arguments, an int value of 1 would be returned.
 */

#include <stdio.h>
#include <string.h>
#include "parse.h"

program_options parse_args(int argc, char **raw_args)
{
    unsigned char flags = 0;
    int i;
    int rest_args_pointer = 1;
    for (i = 0; i < argc; i++)
    {
        if (i > 0)
        { // skip executable name
            char prefix = raw_args[i][0];
            if (strncmp(&prefix, "-", 1) == 0)
            {
                char flag = raw_args[i][1];
                switch (flag)
                {
                case 'G':
                    flags |= 1;
                    rest_args_pointer++;
                    break;
                case '\0':
                    break;
                }
            }
        }
    }
    program_options f = {flags, rest_args_pointer};
    return f;
}

bool colorize_enabled(program_options *po)
{
    return po->flags & COLORIZED_MASK;
}