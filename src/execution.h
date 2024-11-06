#ifndef EXECUTION_H
#define EXECUTION_H

#include "bracket_pair.h"
#include "debug.h"

void brackets_jump(char **program, struct char_coords *coordinates,
                   struct bracket_pair *brackets, unsigned char *array,
                   ssize_t array_pos, int debug_mode);

int run_program(char **program, char *filename, struct bracket_pair *brackets,
                ssize_t array_size, int debug_mode);

#endif /* EXECUTION_H */
