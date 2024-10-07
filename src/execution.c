#include "execution.h"

int move_pointer(ssize_t *array_pos, int is_to_the_right)
{
    if (is_to_the_right)
        *array_pos += 1;
    else
        *array_pos -= 1;

    return 0;
}

int modify_pointed_value(unsigned char *pointed_cell, int is_increment)
{
    if (is_increment)
    {
        if (*pointed_cell == 255)
            return 3;
        *pointed_cell += 1;
    }
    if (!is_increment)
    {
        if (*pointed_cell == 0)
            return 4;
        *pointed_cell -= 1;
    }
    return 0;
}

void brackets_jump(char **program, struct char_coords *coordinates,
                   struct bracket_pair *brackets,
                   unsigned char current_pointed_value)
{
    ssize_t *i = &(coordinates->i);
    ssize_t *j = &(coordinates->j);

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

void putescchar(char c) {
    switch (c) {
    case '\0':
        printf("NUL");
        break;
    case '\1':
        printf("SOH");
        break;
    case '\n':
        printf("LF");
        break;
    case '\033':
        printf("ESC");
        break;
    case ' ':
        printf("SPACE");
        break;
    default:
        putchar(c);
    }
}

void outputchar(unsigned char *array, ssize_t array_pos, ssize_t i, ssize_t j, int debug_level)
{
    if (!debug_level)
    {
        putchar(array[array_pos]);
    }
    else
    {
        printf("\033[1m%4ld:%-4ld [at \033[33m%3ld\033[0;1m] (\033[34m%3d\033[0m) ", i, j, array_pos, array[array_pos]);
        putescchar(array[array_pos]);
        printf("\033[0m\n");
    }
}

int exec_command(char **program, struct char_coords *coordinates,
                 unsigned char *array, ssize_t *array_pos, int log_level)
{
    ssize_t *i = &(coordinates->i);
    ssize_t *j = &(coordinates->j);

    if (program[*i][*j] == '<' || program[*i][*j] == '>')
        return move_pointer(array_pos, program[*i][*j] == '>');

    else if (program[*i][*j] == '+' || program[*i][*j] == '-')
        return modify_pointed_value(&array[*array_pos], program[*i][*j] == '+');

    else if (program[*i][*j] == '.') {
        outputchar(array, *array_pos, *i, *j, log_level);
    }

    else if (program[*i][*j] == ',')
        array[*array_pos] = getchar();

    else if (program[*i][*j] == '[' || program[*i][*j] == ']')
        return -1;

    return 0;
}

int run_program(char **program, char *filename, struct bracket_pair *brackets,
                ssize_t array_size, int log_level)
{
    unsigned char *array = calloc(array_size, sizeof(char));

    struct char_coords coordinates = { 0, 0 };
    ssize_t array_pos = 0;
    int command_result = 0;

    while (program[coordinates.i])
    {
        command_result = exec_command(program, &coordinates, array, &array_pos, log_level);

        if (command_result == -1)
            brackets_jump(program, &coordinates, brackets, array[array_pos]);
        else if (command_result != 0)
        {
            print_runtime_error(program, filename, coordinates, command_result);
            return free_and_return(program, brackets, array, 3);
        }
        else if (array_pos >= array_size || array_pos < 0)
        {
            print_runtime_error(program, filename, coordinates,
                                array_pos < 0 ? 2 : 1);
            return free_and_return(program, brackets, array, 3);
        }

        if (!program[coordinates.i][coordinates.j++])
        {
            coordinates.i += 1;
            coordinates.j = 0;
        }
    }

    return free_and_return(program, brackets, array, command_result);
}
