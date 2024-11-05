#include "debug.h"

void print_debugger_help(void)
{
    puts("TODO: debugger help.");
}

void print_debugger_incorrect_command(void)
{
    puts("TODO: incorrect command.");
}

// returns the new debug_run_state (changes with the use of c/continue and
// n/next)
enum debug_run_state read_debug_command(struct location ***breakpoints)
{
    size_t breakpoints_count = 0;
    while ((*breakpoints)[breakpoints_count])
    {
        breakpoints_count++;
    }

    int next_breakpoint_i;
    int next_breakpoint_j;

    char *line = NULL;
    size_t n;
    while (1)
    {
        fprintf(stderr, "> ");
        if (getline(&line, &n, stdin) < 0)
        {
            fputc('\n', stderr);
            free(line);
            break;
        }
        if (!strcmp(line, "c\n") || !strcmp(line, "continue\n"))
        {
            free(line);
            return RUNNING;
        }
        if (!strcmp(line, "n\n") || !strcmp(line, "next\n") || !strcmp(line, "\n"))
        {
            free(line);
            return PAUSED;
        }

        if (!strcmp(line, "q\n") || !strcmp(line, "quit\n"))
        {
            free(line);
            return TERMINATED;
        }

        if (sscanf(line, "b %d:%d\n", &next_breakpoint_i, &next_breakpoint_j) == 2 || sscanf(line, "break %d:%d\n", &next_breakpoint_i, &next_breakpoint_j) == 2)
        {
            (*breakpoints) = realloc(*breakpoints, sizeof(struct location*) * (breakpoints_count + 2));
            (*breakpoints)[breakpoints_count++] = make_location(next_breakpoint_i, next_breakpoint_j);
            (*breakpoints)[breakpoints_count] = NULL;
        }
        else if (!strcmp(line, "h") || !strcmp(line, "help"))
        {
            print_debugger_help();
        } else {
            print_debugger_incorrect_command();
        }
    }
    free(line);
    return TERMINATED;
}
