#define DEFAULT_ARRAY_SIZE 100

#include "execution.h"

static inline void usage(char *program_name)
{
    fprintf(stderr, "USAGE: %s [options] file\n", program_name);
    fputs("OPTIONS:\n", stderr);
    fputs("  -d, --debug                    ", stderr);
    fputs("Enable debug mode\n", stderr);
    fputs("  -a, --arraysize <size>         ", stderr);
    fputs("Specify a size for the memory array, ", stderr);
    fputs("default will be 0 which means infinite size.\n", stderr);
    fputs("  -h, --help                     Display help\n", stderr);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage(argv[0]);
        return 1;
    }

    char **program = getlines(argv[1]);
    if (!program)
        return 1;

    ssize_t array_size =
        argc > 2 ? check_array_size(argv[2]) : DEFAULT_ARRAY_SIZE;
    if (!array_size)
        return 1;

    int debug_mode = 0;
    if (argc > 3 && argv[3][0] == '-'
        && (argv[3][1] == 'd' || argv[3][1] == 'D'))
    {
        debug_mode = 1;
    }

    struct bracket_pair *brackets = get_bracket_pairs(program, argv[1]);
    if (!brackets)
    {
        free_all(program, brackets, NULL, NULL);
        return 2;
    }

    return run_program(program, argv[1], brackets, array_size, debug_mode);
}
