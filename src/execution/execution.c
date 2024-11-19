#include "execution.h"

int modify_pointed_value(unsigned char *array, ssize_t *array_pos,
                         int is_increment)
{
    if (is_increment)
    {
        if (array[*array_pos] == 255)
            return 3;
        array[*array_pos] += 1;
    }
    if (!is_increment)
    {
        if (array[*array_pos] == 0)
            return 4;
        array[*array_pos] -= 1;
    }
    return 0;
}

int execute_instruction(char instruction, unsigned char *array,
                        ssize_t *array_pos, struct location *location,
                        struct bracket_pair *brackets)
{
    char input;
    switch (instruction)
    {
    case '<':
        *array_pos -= 1;
        break;
    case '>':
        *array_pos += 1;
        break;
    case '+':
    case '-':
        return modify_pointed_value(array, array_pos, instruction == '+');
    case '.':
        putchar(array[*array_pos]);
        break;
    case ',':
        input = getchar();
        array[*array_pos] = input == EOF ? '\0' : input;
        break;
    case '[':
        if (!array[*array_pos])
        {
            struct location right =
                find_matching_bracket(instruction, *location, brackets);
            location->i = right.i;
            location->j = right.j;
        }
        break;
    case ']':
        if (array[*array_pos])
        {
            struct location left =
                find_matching_bracket(instruction, *location, brackets);
            location->i = left.i;
            location->j = left.j;
        }
        break;
    default:
        break;
    }
    return 0;
}

static inline int is_valid_operation(char operation)
{
    return operation == '+' || operation == '-' || operation == '<'
        || operation == '>' || operation == '.' || operation == ','
        || operation == '[' || operation == ']';
}

int run_program(char **program, char *filename, struct bracket_pair *brackets,
                ssize_t array_size, int debug_mode)
{
    unsigned char *array = calloc(array_size, sizeof(char));

    struct location location = { filename, 0, 0 };
    ssize_t array_pos = 0;
    int command_result = 0;

    enum debug_run_state run_state = RUNNING;
    map *breakpoints = NULL;

    if (debug_mode)
    {
        run_state = PAUSED;
        print_debug_mode_intro();
    }

    while (program[location.i])
    {
        if (run_state == PAUSED
            && is_valid_operation(program[location.i][location.j]))
        {
            display_program_location(program[location.i], location, BLUE);
            run_state = execute_debug_command(array, &breakpoints);
        }
        if (run_state == TERMINATED)
            break;
        if (debug_mode)
        {
            log_operation(program, array, array_pos, location);
            if (run_state == RUNNING)
            {
                if (find_breakpoint(breakpoints, location.i + 1,
                                    location.j + 1))
                {
                    run_state = PAUSED;
                    fprintf(stderr,
                            "Breakpoint at %ld:%ld, pausing execution.\n",
                            location.i + 1, location.j + 1);
                }
            }
        }

        command_result =
            execute_instruction(program[location.i][location.j], array,
                                &array_pos, &location, brackets);
        if (command_result != 0)
        {
            print_runtime_error(program, location, command_result);
            free_all(program, brackets, array, breakpoints);
            return 3;
        }
        else if (array_pos >= array_size || array_pos < 0)
        {
            print_runtime_error(program, location, array_pos < 0 ? 2 : 1);
            free_all(program, brackets, array, breakpoints);
            return 3;
        }

        if (!program[location.i][location.j++])
        {
            location.i += 1;
            location.j = 0;
        }
    }

    free_all(program, brackets, array, breakpoints);
    return command_result;
}
