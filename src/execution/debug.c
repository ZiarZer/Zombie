#include "debug.h"

#define COMMANDS_COUNT 9

char *commands[COMMANDS_COUNT][2] = {
    { "continue", "Continue running the program (at the start, or at a breakpoint)." },
    { "next", "Execute next program instruction." },
    { "break", "Add a breakpoint at specified line and column. Syntax: break <line>:<column>" },
    { "remove", "Remove breakpoint at specified line and column if it exists. Syntax: remove <line>:<column>" },
    { "print", "Print the content of the cell at specified index content. Syntax: print <index>" },
    { "alter", "Change a cell's value. Syntax: alter <index> <int>" },
    { "move", "Move the cursor at specified index. Syntax: move <index>" },
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

struct debug_command parse_debug_command(char *line) {
    int param1;
    int param2;
    struct debug_command debug_command = { NONE, 0, 0 };
    if (!line || !strcmp(line, "q") || !strcmp(line, "q")) {
        debug_command.type = QUIT;
    } else if (!strcmp(line, "")) {
        debug_command.type = LAST;
    } else if (!strcmp(line, "c") || !strcmp(line, "continue")) {
        debug_command.type = CONTINUE;
    } else if (!strcmp(line, "n") || !strcmp(line, "next")) {
        debug_command.type = NEXT;
    } else if (!strcmp(line, "h") || !strcmp(line, "help")) {
        debug_command.type = HELP;
    } else if (sscanf(line, "b %d:%d", &param1, &param2) == 2 || sscanf(line, "break %d:%d", &param1, &param2) == 2) {
        debug_command.type = BREAK;
        debug_command.param1 = param1;
        debug_command.param2 = param2;
    } else if (sscanf(line, "r %d:%d", &param1, &param2) == 2 || sscanf(line, "remove %d:%d", &param1, &param2) == 2) {
        debug_command.type = REMOVE;
        debug_command.param1 = param1;
        debug_command.param2 = param2;
    } else if (sscanf(line, "p %d", &param1) == 1 || sscanf(line, "print %d", &param1) == 1) {
        debug_command.type = PRINT;
        debug_command.param1 = param1;
    } else if (sscanf(line, "a %d %d", &param1, &param2) == 2 || sscanf(line, "alter %d %d", &param1, &param2) == 2) {
        debug_command.type = ALTER;
        debug_command.param1 = param1;
        debug_command.param2 = param2;
    } else if (sscanf(line, "m %d", &param1) == 1 || sscanf(line, "move %d", &param1) == 1) {
        debug_command.type = MOVE;
        debug_command.param1 = param1;
    }
    return debug_command;
}

char *get_debug_console_user_input(char **lineptr, size_t *nptr) {
    fprintf(stderr, "(zombie) ");
    ssize_t read_count = getline(lineptr, nptr, stdin);
    if (read_count < 0) {
        fputc('\n', stderr);
        return NULL;
    } else {
        (*lineptr)[read_count - 1] = '\0';
        return *lineptr;
    }
}

/**
 * \brief Execute the debug command entered by the user if it is valid.
 * \param line the user input, nul-terminated and with no newline.
 * \param previous_command the previously built debug command variable.
 * \param array the memory array.
 * \param instructions instructions list of the current program.
 * \return The new debug run state
 */
enum debug_run_state execute_debug_command(char *line, struct debug_command *previous_command,
                                           struct memory_array *array, struct instruction *instructions) {
    struct debug_command debug_command = parse_debug_command(line);
    if (debug_command.type == LAST) {
        if (previous_command->type == NONE)
            return PAUSED;
        debug_command = *previous_command;
    }
    *previous_command = debug_command;

    switch (debug_command.type) {
    case CONTINUE:
        return RUNNING;
    case NEXT:
        return STEPPING;
    case QUIT:
        return TERMINATED;
    case BREAK:
        add_breakpoint(instructions, debug_command.param1 - 1, debug_command.param2 - 1);
        fprintf(stderr, "Breakpoint added at %d:%d\n", debug_command.param1, debug_command.param2);
        return PAUSED;
    case REMOVE:
        fprintf(stderr, "Removing breakpoint at %d:%d (if it exists)\n", debug_command.param1, debug_command.param2);
        remove_breakpoint(instructions, debug_command.param1 - 1, debug_command.param2 - 1);
        return PAUSED;
    case PRINT:
        fputs("\033[1m          ", stderr);
        log_cell_position(debug_command.param1, 0);
        fputs("        ", stderr);
        log_cell_content(array_get_at(array, debug_command.param1), 0);
        fputs("\033[0m\n", stderr);
        return PAUSED;
    case ALTER:
        array_set_at(array, debug_command.param1, debug_command.param2);
        return PAUSED;
    case MOVE:
        array_move(array, debug_command.param1);
        return PAUSED;
    case HELP:
        print_debugger_help();
        return PAUSED;
    default:
        print_debugger_incorrect_command(line);
        return PAUSED;
    }
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

void log_operation(char **program, struct memory_array *array, struct location location) {
    ssize_t i = location.i;
    ssize_t j = location.j;
    char operation = program[i][j];
    char *operation_name = get_instruction_debug_name(operation);
    if (!operation_name)
        return;

    int highlight = operation == '.';
    fputs(highlight ? "\033[47;1m" : "\033[1m", stderr);
    fprintf(stderr, "%4ld:%-4ld ", i + 1, j + 1);
    log_cell_position(array->cursor, highlight);
    fprintf(stderr, " %*s ", 6, operation_name);
    log_cell_content(array_get_current(array), highlight);
    fputs("\033[0m\n", stderr);
}
