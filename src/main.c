#define DEFAULT_ARRAY_SIZE 100

#include "execution.h"
#include "option_flag.h"

static inline void usage(char *program_name) {
    fprintf(stderr, "USAGE: %s [options] file\n", program_name);
    fputs("OPTIONS:\n", stderr);
    fputs("  -d, --debug                    ", stderr);
    fputs("Enable debug mode\n", stderr);
    fputs("  -a, --arraysize <size>         ", stderr);
    fputs("Specify a size for the memory array, ", stderr);
    fputs("default will be 0 which means infinite size.\n", stderr);
    fputs("  -h, --help                     Display help\n", stderr);
}

int main(int argc, char *argv[]) {
    int debug_mode;
    int help_flag;
    long size;
    int offset = init_option_flags(argc, argv, &debug_mode, &size, &help_flag);

    char **program = NULL;
    char *filename = NULL;
    if (offset == argc)
        help_flag = 1;

    if ((offset == argc) || help_flag) {
        usage(argv[0]);
        return 1;
    } else {
        filename = argv[offset];
        program = getlines(filename);
    }

    if (!program)
        return 1;

    unsigned long array_size = size;
    if (!array_size)
        array_size = DEFAULT_ARRAY_SIZE; // Tmp

    struct bracket_pair *brackets = get_bracket_pairs(program, filename);
    if (!brackets) {
        free_all(program, brackets, NULL, NULL);
        return 2;
    }

    return run_program(program, filename, brackets, array_size, debug_mode);
}
