#include "option_flag.h"

int init_option_flags(int argc, char *argv[], int *debug_mode, long *size, enum prioritary_option *prioritary_flag) {
    *debug_mode = 0;
    *prioritary_flag = NO_OPTION;
    *size = 0;

    static struct option longopts[] = { { "debug", no_argument, NULL, 'd' },
                                        { "arraysize", required_argument, NULL, 'a' },
                                        { "version", no_argument, NULL, 'v' },
                                        { "help", no_argument, NULL, 'h' } };

    char ch;
    while ((ch = getopt_long(argc, argv, "da:vh", longopts, NULL)) != -1) {
        switch (ch) {
        case 'd':
            *debug_mode = 1;
            break;
        case 'a':
            *size = parse_array_size(optarg);
            if (*size < 0)
                *prioritary_flag = HELP_OPTION;
            break;
        case 'v':
            if (*prioritary_flag != HELP_OPTION) {
                *prioritary_flag = VERSION_OPTION;
            }
            break;
        case 'h':
        default:
            *prioritary_flag = HELP_OPTION;
            break;
        }
    }

    return optind;
}
