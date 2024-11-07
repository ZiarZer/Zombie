#ifndef DEBUG_H
#define DEBUG_H

#include "utils.h"

enum debug_run_state
{
    PAUSED,
    RUNNING,
    TERMINATED
};

void print_debug_mode_intro(void);
enum debug_run_state read_debug_command(struct location ***breakpoints);
void log_operation(char **program, unsigned char *array, ssize_t array_pos,
                   struct location location);

#endif /* !DEBUG_H */
