#include "debug.h"

#define COMMANDS_COUNT 5

char *commands[COMMANDS_COUNT][2] = {
    { "continue", "Continue running the program (at the start, or at a breakpoint)." },
    { "next", "Execute next program instruction." },
    { "break", "Add a breakpoint at specified line and column. Syntax: break <line>:<column>" },
    { "help", "Display help." },
    { "quit", "Stop execution and exit." }
};

void print_debug_mode_intro(void)
{
    fputs("=== zombie: debug mode ===\n", stderr);
    fputs("For help, type \"help\".\n", stderr);
}

void print_debugger_help(void)
{
    for (int i = 0; i < COMMANDS_COUNT; i++) {
        fprintf(stderr, "    %*s\t%s\n", 8, commands[i][0], commands[i][1]);
    }
}

void print_debugger_incorrect_command(char *command)
{
    fprintf(stderr, "Undefined command: \"%s\". Try \"help\".\n", command);
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
    ssize_t read_count;
    while (1)
    {
        fprintf(stderr, "(zombie) ");read_count = getline(&line, &n, stdin);
        if (read_count<0)
        {
            fputc('\n', stderr);
            break;
        }
        else {
            line[read_count - 1] = '\0';
        }
        if (!strcmp(line, "c") || !strcmp(line, "continue"))
        {
            free(line);
            return RUNNING;
        }
        if (!strcmp(line, "n") || !strcmp(line, "next") || !strcmp(line, ""))
        {
            free(line);
            return PAUSED;
        }

        if (!strcmp(line, "q") || !strcmp(line, "quit"))
        {
            free(line);
            return TERMINATED;
        }

        if (sscanf(line, "b %d:%d", &next_breakpoint_i, &next_breakpoint_j) == 2 || sscanf(line, "break %d:%d", &next_breakpoint_i, &next_breakpoint_j) == 2)
        {
            (*breakpoints) = realloc(*breakpoints, sizeof(struct location*) * (breakpoints_count + 2));
            (*breakpoints)[breakpoints_count++] = make_location(next_breakpoint_i, next_breakpoint_j);
            (*breakpoints)[breakpoints_count] = NULL;
        }
        else if (!strcmp(line, "h") || !strcmp(line, "help"))
        {
            print_debugger_help();
        } else {
            print_debugger_incorrect_command(line);
        }
    }
    free(line);
    return TERMINATED;
}
