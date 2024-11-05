#ifndef DEBUG_H
#define DEBUG_H

#include "utils.h"

enum debug_run_state { PAUSED, RUNNING, TERMINATED };

enum debug_run_state read_debug_command(struct location ***breakpoints);

#endif /* !DEBUG_H */
