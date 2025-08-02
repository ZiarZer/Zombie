#ifndef OPTION_FLAG_H
#define OPTION_FLAG_H

#include <getopt.h>
#include <stdbool.h>

#include "utils.h"

enum prioritary_option { NO_OPTION, VERSION_OPTION, HELP_OPTION };

struct program_params {
    bool debug_mode;
    bool extension_enabled;
    bool show_version;
    bool show_help;
    long size;
};

int init_option_flags(int argc, char *argv[], struct program_params *params);

#endif /* ! OPTION_FLAG_H */
