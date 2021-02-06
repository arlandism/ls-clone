typedef struct user_flags {
    int colorize;
} user_flags;

user_flags parse_args(char** raw_args);