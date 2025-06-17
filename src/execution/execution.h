#ifndef EXECUTION_H
#define EXECUTION_H

#include "bracket_pair.h"
#include "debug.h"
#include "memory_array.h"

int run_program(struct source_file *src_file, struct bracket_pair *brackets, ssize_t array_size);

int run_debug_mode(struct source_file *src_file, struct bracket_pair *brackets, ssize_t array_size);

#endif /* EXECUTION_H */
