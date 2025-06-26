#include "execution.h"
#include "option_flag.h"
#include "parser.h"

#define VERSION "1.0.1"

static inline void usage(char *program_name) {
    fprintf(stderr, "USAGE: %s [options] file\n", program_name);
    fputs("OPTIONS:\n", stderr);
    fputs("  -d, --debug                    ", stderr);
    fputs("Enable debug mode\n", stderr);
    fputs("  -a, --arraysize <size>         ", stderr);
    fputs("Specify a size for the memory array, ", stderr);
    fputs("default will be 0 which means infinite size.\n", stderr);
    fputs("  -v, --version                  Display version\n", stderr);
    fputs("  -h, --help                     Display help\n", stderr);
}

static inline void version(void) {
    printf("%s\n", VERSION);
}

int main(int argc, char *argv[]) {
    bool debug_mode = false;
    enum prioritary_option prioritary_option_flag = NO_OPTION;
    long size = 0;
    int offset = init_option_flags(argc, argv, &debug_mode, &size, &prioritary_option_flag);

    if (offset == argc && prioritary_option_flag == NO_OPTION) {
        prioritary_option_flag = HELP_OPTION;
    }

    if (prioritary_option_flag == HELP_OPTION) {
        usage(argv[0]);
        return 1;
    }
    if (prioritary_option_flag == VERSION_OPTION) {
        version();
        return 0;
    }
    char *filename = argv[offset];
    char **program = getlines(filename);
    if (!program)
        return 1;
    struct source_file source = { filename, program };

    unsigned long array_size = size;
    if (!array_size)
        array_size = 0;

    struct instruction *instructions = scan_source(&source, debug_mode);
    int successful_parsing = bind_matching_operations(instructions, &source);
    if (!successful_parsing) {
        free_all(program, NULL);
        return 2;
    }

    if (debug_mode) {
        return run_debug_mode(&source, instructions, array_size);
    }
    return run_program(&source, instructions, array_size);
}
