#ifndef OPTION_FLAG_H
#define OPTION_FLAG_H

#include <getopt.h>
#include <stdbool.h>

#include "utils.h"

enum prioritary_option { NO_OPTION, VERSION_OPTION, HELP_OPTION };

int init_option_flags(int argc, char *argv[], bool *debug_mode, long *size, enum prioritary_option *prioritary_flag);

#endif /* ! OPTION_FLAG_H */
