#include "option_flag.h"

int init_option_flags(int argc, char *argv[], int *debug_mode, long *size, int *help_flag) {
    *debug_mode = 0;
    *help_flag = 0;
    *size = 0;

    static struct option longopts[] = { { "debug", no_argument, NULL, 'd' },
                                        { "arraysize", required_argument, NULL, 'a' },
                                        { "help", no_argument, NULL, 'h' } };

    char ch;
    while ((ch = getopt_long(argc, argv, "da:h", longopts, NULL)) != -1) {
        switch (ch) {
        case 'd':
            *debug_mode = 1;
            break;
        case 'a':
            *size = parse_array_size(optarg);
            if (*size < 0)
                *help_flag = 1;
            break;
        case 'h':
        default:
            *help_flag = 1;
            break;
        }
    }

    return optind;
}
