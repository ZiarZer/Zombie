#include "debug.h"

#define COMMANDS_COUNT 6

char *commands[COMMANDS_COUNT][2] = {
    { "continue",
      "Continue running the program (at the start, or at a breakpoint)." },
    { "next", "Execute next program instruction." },
    { "break",
      "Add a breakpoint at specified line and column. Syntax: break "
      "<line>:<column>" },
    { "print",
      "Print the content of the cell at specified index content. Syntax: print "
      "<index>" },
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
    for (int i = 0; i < COMMANDS_COUNT; i++)
    {
        fprintf(stderr, "    %*s\t%s\n", 8, commands[i][0], commands[i][1]);
    }
}

void print_debugger_incorrect_command(char *command)
{
    fprintf(stderr, "Undefined command: \"%s\". Try \"help\".\n", command);
}

void log_cell_content(unsigned char c, int highlight)
{
    char *color_start = highlight ? "\033[44m" : "\033[34m";
    char *color_end = highlight ? "\033[47m" : "\033[0m";

    fprintf(stderr, "(%s%3d%s) ", color_start, c, color_end);
    putescchar(c);
}

void log_cell_position(size_t array_pos, int highlight)
{
    char *color_start = highlight ? "\033[43m" : "\033[33m";
    char *color_end = highlight ? "\033[47m" : "\033[0;1m";
    fprintf(stderr, "[at %s%3ld%s]", color_start, array_pos, color_end);
}

// returns the new debug_run_state (changes with the use of c/continue and
// n/next)
enum debug_run_state execute_debug_command(unsigned char *array,
                                           map **breakpoints)
{
    int next_breakpoint_i;
    int next_breakpoint_j;
    int printed_cell;

    char *line = NULL;
    size_t n;
    ssize_t read_count;
    while (1)
    {
        fprintf(stderr, "(zombie) ");
        read_count = getline(&line, &n, stdin);
        if (read_count < 0)
        {
            fputc('\n', stderr);
            break;
        }
        else
        {
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

        if (sscanf(line, "b %d:%d", &next_breakpoint_i, &next_breakpoint_j) == 2
            || sscanf(line, "break %d:%d", &next_breakpoint_i,
                      &next_breakpoint_j)
                == 2)
        {
            *breakpoints = add_breakpoint(*breakpoints, next_breakpoint_i,
                                          next_breakpoint_j);
        }
        else if (sscanf(line, "p %d", &printed_cell) == 1
                 || sscanf(line, "print %d", &printed_cell) == 1)
        {
            fputs("\033[1m          ", stderr);
            log_cell_position(printed_cell, 0);
            fputs("        ", stderr);
            log_cell_content(array[printed_cell], 0);
            fputs("\033[0m\n", stderr);
        }
        else if (!strcmp(line, "h") || !strcmp(line, "help"))
        {
            print_debugger_help();
        }
        else
        {
            print_debugger_incorrect_command(line);
        }
    }
    free(line);
    return TERMINATED;
}

char *get_instruction_debug_name(char instruction)
{
    switch (instruction)
    {
    case '+':
        return "ADD";
    case '-':
        return "SUB";
    case '<':
        return "MOVE<-";
    case '>':
        return "MOVE->";
    case '.':
        return "STDOUT";
    case ',':
        return "STDIN";
    case '[':
        return "LOOP[";
    case ']':
        return "LOOP]";
    default:
        return NULL;
    }
}

void log_operation(char **program, unsigned char *array, ssize_t array_pos,
                   struct location location)
{
    ssize_t i = location.i;
    ssize_t j = location.j;
    char operation = program[i][j];
    char *operation_name = get_instruction_debug_name(operation);
    if (!operation_name)
        return;

    int highlight = operation == '.';
    fputs(highlight ? "\033[47;1m" : "\033[1m", stderr);
    fprintf(stderr, "%4ld:%-4ld ", i + 1, j + 1);
    log_cell_position(array_pos, highlight);
    fprintf(stderr, " %*s ", 6, operation_name);
    log_cell_content(array[array_pos], highlight);
    fputs("\033[0m\n", stderr);

    display_program_location(program[location.i], location, BLUE);
}
