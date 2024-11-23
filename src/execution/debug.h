#ifndef DEBUG_H
#define DEBUG_H

#define _POSIX_C_SOURCE 200809L

#include "breakpoint.h"
#include "utils.h"

enum debug_run_state { PAUSED, STEPPING, RUNNING, TERMINATED };

enum debug_command_type { CONTINUE, NEXT, BREAK, REMOVE, PRINT, ALTER, MOVE, QUIT, HELP, LAST, NONE };

struct debug_command {
    enum debug_command_type type;
    int param1;
    int param2;
};

void print_debug_mode_intro(void);
char *get_debug_console_user_input(char **lineptr, size_t *nptr);
enum debug_run_state execute_debug_command(char *line, struct debug_command *previous_command,
                                           struct memory_array *array, map **breakpoints);
void log_operation(char **program, struct memory_array *array, struct location location);

#endif /* !DEBUG_H */
