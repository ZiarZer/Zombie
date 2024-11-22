#ifndef DEBUG_H
#define DEBUG_H

#define _POSIX_C_SOURCE 200809L

#include "breakpoint.h"
#include "utils.h"

enum debug_run_state { PAUSED, RUNNING, TERMINATED };

enum debug_command_type { CONTINUE, NEXT, BREAK, PRINT, QUIT, HELP, NONE };

struct debug_command {
    enum debug_command_type type;
    int param1;
    int param2;
};

void print_debug_mode_intro(void);
enum debug_run_state execute_debug_command(unsigned char *array, map **breakpoints);
void log_operation(char **program, unsigned char *array, ssize_t array_pos, struct location location);

#endif /* !DEBUG_H */
