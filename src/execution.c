#include "execution.h"

int move_pointer(ssize_t *array_pos, int is_to_the_right)
{
    if (is_to_the_right)
        *array_pos += 1;
    else
        *array_pos -= 1;

    return 0;
}

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

void brackets_jump(char **program, struct location *location,
                   struct bracket_pair *brackets, unsigned char *array,
                   ssize_t array_pos)
{
    unsigned char current_pointed_value = array[array_pos];

    if (program[location->i][location->j] == '[' && !current_pointed_value)
    {
        struct location right =
            find_matching_bracket(program, *location, brackets);
        location->i = right.i;
        location->j = right.j;
    }

    else if (program[location->i][location->j] == ']' && current_pointed_value)
    {
        struct location left =
            find_matching_bracket(program, *location, brackets);
        location->i = left.i;
        location->j = left.j;
    }
}

int exec_command(char **program, struct location *location,
                 unsigned char *array, ssize_t *array_pos)
{
    ssize_t i = location->i;
    ssize_t j = location->j;

    if (program[i][j] == '<' || program[i][j] == '>')
        return move_pointer(array_pos, program[i][j] == '>');

    else if (program[i][j] == '+' || program[i][j] == '-')
        return modify_pointed_value(array, array_pos, program[i][j] == '+');

    else if (program[i][j] == '.')
    {
        putchar(array[*array_pos]);
    }

    else if (program[i][j] == ',')
    {
        char c = getchar();
        array[*array_pos] = c == EOF ? '\0' : c;
    }

    else if (program[i][j] == '[' || program[i][j] == ']')
        return -1;

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
    struct location **breakpoints =
        debug_mode ? calloc(1, sizeof(struct location *)) : NULL;

    if (debug_mode)
    {
        run_state = PAUSED;
        print_debug_mode_intro();
    }

    while (program[location.i])
    {
        if (run_state == PAUSED
            && is_valid_operation(program[location.i][location.j]))
            run_state = execute_debug_command(array, &breakpoints);
        if (run_state == TERMINATED)
            break;
        if (debug_mode)
            log_operation(program, array, array_pos, location);

        command_result = exec_command(program, &location, array, &array_pos);

        if (command_result == -1)
            brackets_jump(program, &location, brackets, array, array_pos);
        else if (command_result != 0)
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
