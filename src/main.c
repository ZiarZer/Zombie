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

    if (offset == argc)
        help_flag = 1;

    if ((offset == argc) || help_flag) {
        usage(argv[0]);
        return 1;
    }
    char *filename = argv[offset];
    char **program = getlines(filename);
    if (!program)
        return 1;
    struct source_file source = { filename, program };

    unsigned long array_size = size;
    if (!array_size)
        array_size = 0;

    struct bracket_pair *brackets = get_bracket_pairs(&source);
    if (!brackets) {
        free_all(program, brackets, NULL, NULL);
        return 2;
    }

    if (debug_mode) {
        return run_debug_mode(&source, brackets, array_size);
    }
    return run_program(&source, brackets, array_size);
}
