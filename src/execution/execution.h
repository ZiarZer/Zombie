#ifndef EXECUTION_H
#define EXECUTION_H

#include "debug.h"
#include "error.h"
#include "instruction.h"
#include "memory_array.h"
#include "source_file.h"

int run_program(struct source_file *src_file, struct instruction *instructions, ssize_t array_size);

int run_debug_mode(struct source_file *src_file, struct instruction *instructions, ssize_t array_size);

#endif /* EXECUTION_H */
