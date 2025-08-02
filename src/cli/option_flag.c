#include "option_flag.h"

int init_option_flags(int argc, char *argv[], struct program_params *params) {
    static struct option longopts[] = { { "debug", no_argument, NULL, 'd' },
                                        { "extension", no_argument, NULL, 'e' },
                                        { "arraysize", required_argument, NULL, 'a' },
                                        { "version", no_argument, NULL, 'v' },
                                        { "help", no_argument, NULL, 'h' } };

    char ch;
    while ((ch = getopt_long(argc, argv, "dea:vh", longopts, NULL)) != -1) {
        switch (ch) {
        case 'd':
            params->debug_mode = true;
            break;
        case 'e':
            params->extension_enabled = true;
            break;
        case 'a':
            params->size = parse_array_size(optarg);
            if (params->size < 0)
                params->show_help = true;
            break;
        case 'v':
            params->show_version = true;
            break;
        case 'h':
        default:
            params->show_help = true;
            break;
        }
    }

    if (optind == argc && !params->show_help && !params->show_version) {
        params->show_help = true;
    }

    return optind;
}
