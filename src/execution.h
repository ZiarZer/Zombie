#ifndef EXECUTION_H
#define EXECUTION_H

#include "bracket_pair.h"
#include "utils.h"

void brackets_jump(char **program, struct char_coords *coordinates,
                   struct bracket_pair *brackets,
                   unsigned char current_pointed_value);

int run_program(char **program, char *filename, struct bracket_pair *brackets,
                ssize_t array_size);

#endif /* EXECUTION_H */
