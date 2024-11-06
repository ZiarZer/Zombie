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

void brackets_jump(char **program, struct char_coords *coordinates,
                   struct bracket_pair *brackets, unsigned char *array,
                   ssize_t array_pos)
{
    ssize_t *i = &(coordinates->i);
    ssize_t *j = &(coordinates->j);
    unsigned char current_pointed_value = array[array_pos];

    if (program[*i][*j] == '[' && !current_pointed_value)
    {
        struct char_coords right =
            find_matching_bracket(program, *i, *j, brackets);
        *i = right.i;
        *j = right.j;
    }

    else if (program[*i][*j] == ']' && current_pointed_value)
    {
        struct char_coords left =
            find_matching_bracket(program, *i, *j, brackets);
        *i = left.i;
        *j = left.j;
    }
}

int exec_command(char **program, struct char_coords *coordinates,
                 unsigned char *array, ssize_t *array_pos)
{
    ssize_t *i = &(coordinates->i);
    ssize_t *j = &(coordinates->j);

    if (program[*i][*j] == '<' || program[*i][*j] == '>')
        return move_pointer(array_pos, program[*i][*j] == '>');

    else if (program[*i][*j] == '+' || program[*i][*j] == '-')
        return modify_pointed_value(array, array_pos, program[*i][*j] == '+');

    else if (program[*i][*j] == '.')
    {
        putchar(array[*array_pos]);
    }

    else if (program[*i][*j] == ',')
    {
        char c = getchar();
        array[*array_pos] = c == EOF ? '\0' : c;
    }

    else if (program[*i][*j] == '[' || program[*i][*j] == ']')
        return -1;

    return 0;
}

int run_program(char **program, char *filename, struct bracket_pair *brackets,
                ssize_t array_size, int debug_mode)
{
    unsigned char *array = calloc(array_size, sizeof(char));

    struct char_coords coordinates = { 0, 0 };
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

    while (program[coordinates.i])
    {
        if (run_state == PAUSED)
        {
            run_state = read_debug_command(&breakpoints);
        }
        if (run_state == TERMINATED)
        {
            break;
        }
        if (debug_mode)
        {
            log_operation(array, array_pos, coordinates.i, coordinates.j,
                          program[coordinates.i][coordinates.j]);
        }

        command_result = exec_command(program, &coordinates, array, &array_pos);

        if (command_result == -1)
            brackets_jump(program, &coordinates, brackets, array, array_pos);
        else if (command_result != 0)
        {
            print_runtime_error(program, filename, coordinates, command_result);
            free_all(program, brackets, array, breakpoints);
            return 3;
        }
        else if (array_pos >= array_size || array_pos < 0)
        {
            print_runtime_error(program, filename, coordinates,
                                array_pos < 0 ? 2 : 1);
            free_all(program, brackets, array, breakpoints);
            return 3;
        }

        if (!program[coordinates.i][coordinates.j++])
        {
            coordinates.i += 1;
            coordinates.j = 0;
        }
    }

    free_all(program, brackets, array, breakpoints);
    return command_result;
}
