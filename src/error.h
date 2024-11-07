#ifndef ERROR_H
#define ERROR_H

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "location.h"

void print_runtime_error(char **program, struct location location,
                         int error_code);
void print_error(char *line, struct location location);

#endif /* ERROR_H */
