#ifndef EXECUTION_H
#define EXECUTION_H

#include <time.h>

#include "debug.h"
#include "error.h"
#include "instruction.h"
#include "memory_array.h"
#include "source_file.h"

int run_program(struct source_file *src_file, struct instruction *instructions, ssize_t array_size, bool debug_mode);

#endif /* EXECUTION_H */
