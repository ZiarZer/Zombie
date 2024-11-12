#ifndef ERROR_H
#define ERROR_H

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "location.h"

#define RED "\033[31;1m"
#define BLUE "\033[34;1m"

void print_runtime_error(char **program, struct location location,
                         int error_code);
void display_program_location(char *line, struct location location,
                              char *color);

void missing_bracket_error_message(char *line, struct location location);

#endif /* ERROR_H */
