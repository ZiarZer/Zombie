#ifndef OPTION_FLAG_H
#define OPTION_FLAG_H

#include <getopt.h>

#include "utils.h"

int init_option_flags(int argc, char *argv[], int *debug_mode, long *size, int *help_flag);

#endif /* ! OPTION_FLAG_H */
