#ifndef EXECUTION_H
#define EXECUTION_H

#include "bracket_pair.h"
#include "debug.h"

int run_program(char **program, char *filename, struct bracket_pair *brackets, ssize_t array_size);

int run_debug_mode(char **program, char *filename, struct bracket_pair *brackets, ssize_t array_size);

#endif /* EXECUTION_H */
