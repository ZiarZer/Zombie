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
enum debug_run_state execute_debug_command(unsigned char *array,
                                           struct location ***breakpoints);
void log_operation(char **program, unsigned char *array, ssize_t array_pos,
                   struct location location);

#endif /* !DEBUG_H */
