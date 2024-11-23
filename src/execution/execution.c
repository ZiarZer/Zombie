#include "execution.h"

enum instruction_result modify_pointed_value(struct memory_array *array, int is_increment) {
    unsigned char current_value = array_get_current(array);
    if (is_increment) {
        if (current_value == 255)
            return VALUE_UPPER_ERROR;
        array_set_current(array, current_value + 1);
    }
    if (!is_increment) {
        if (current_value == 0)
            return VALUE_LOWER_ERROR;
        array_set_current(array, current_value - 1);
    }
    return DONE;
}

enum instruction_result execute_instruction(char instruction, struct memory_array *array, struct location *location,
                                            struct bracket_pair *brackets) {
    char input;
    switch (instruction) {
    case '<':
        return array_move(array, array->cursor - 1) ? POINTER_LOWER_ERROR : DONE;
    case '>':
        return array_move(array, array->cursor + 1) ? POINTER_UPPER_ERROR : DONE;
    case '+':
    case '-':
        return modify_pointed_value(array, instruction == '+');
    case '.':
        putchar(array_get_current(array));
        break;
    case ',':
        input = getchar();
        array_set_current(array, input == EOF ? '\0' : input);
        break;
    case '[':
        if (!array_get_current(array)) {
            struct location right = find_matching_bracket(instruction, *location, brackets);
            location->i = right.i;
            location->j = right.j;
            return FOUND_BRACKET;
        }
        break;
    case ']':
        if (array_get_current(array)) {
            struct location left = find_matching_bracket(instruction, *location, brackets);
            location->i = left.i;
            location->j = left.j;
            return FOUND_BRACKET;
        }
        break;
    default:
        break;
    }
    return DONE;
}

static inline int is_valid_operation(char operation) {
    return operation == '+' || operation == '-' || operation == '<' || operation == '>' || operation == '.'
        || operation == ',' || operation == '[' || operation == ']';
}

int run_program(char **program, char *filename, struct bracket_pair *brackets, ssize_t array_size) {
    struct memory_array *array = array_init(array_size);

    struct location location = { filename, 0, 0 };
    int command_result = 0;

    while (program[location.i]) {
        command_result = execute_instruction(program[location.i][location.j], array, &location, brackets);
        if (command_result != FOUND_BRACKET && command_result != DONE) {
            print_runtime_error(program, location, command_result);
            free_all(program, brackets, array, NULL);
            return 3;
        } else if (array->cursor > array->high_bound || array->cursor < array->low_bound) {
            print_runtime_error(program, location,
                                array->cursor < array->low_bound ? POINTER_LOWER_ERROR : POINTER_UPPER_ERROR);
            free_all(program, brackets, array, NULL);
            return 3;
        }

        if (!program[location.i][location.j++]) {
            location.i += 1;
            location.j = 0;
        }
    }

    free_all(program, brackets, array, NULL);
    return command_result;
}

int run_debug_mode(char **program, char *filename, struct bracket_pair *brackets, ssize_t array_size) {
    struct memory_array *array = array_init(array_size);

    struct location location = { filename, 0, 0 };
    int command_result = 0;

    struct debug_command previous_command = { NONE, 0, 0 };
    enum debug_run_state run_state = PAUSED;
    map *breakpoints = NULL;

    print_debug_mode_intro();

    int must_display_program_location = 1;
    size_t n;
    char *line = NULL;
    while (program[location.i]) {
        if ((run_state == PAUSED || run_state == STEPPING) && is_valid_operation(program[location.i][location.j])) {
            if (must_display_program_location) {
                fputs("--------------------------\n", stderr);
                display_program_location(program[location.i], location, BLUE);
                must_display_program_location = 0;
            }
            line = get_debug_console_user_input(&line, &n);
            enum debug_run_state new_state = execute_debug_command(line, &previous_command, array, &breakpoints);
            if (new_state == PAUSED) {
                continue;
            }
            run_state = new_state;
        }
        if (run_state == TERMINATED)
            break;
        log_operation(program, array, location);
        if (run_state == RUNNING) {
            if (find_breakpoint(breakpoints, location.i + 1, location.j + 1)) {
                run_state = PAUSED;
                fprintf(stderr, "Breakpoint at %ld:%ld, pausing execution.\n", location.i + 1, location.j + 1);
            }
        }

        command_result = execute_instruction(program[location.i][location.j], array, &location, brackets);
        must_display_program_location = 1;
        if (command_result == FOUND_BRACKET) {
            log_operation(program, array, location);
        } else if (command_result != DONE) {
            print_runtime_error(program, location, command_result);
            free_all(program, brackets, array, NULL);
            return 3;
        } else if (array->cursor > array->high_bound || array->cursor < array->low_bound) {
            print_runtime_error(program, location,
                                array->cursor < array->low_bound ? POINTER_LOWER_ERROR : POINTER_UPPER_ERROR);
            free_all(program, brackets, array, NULL);
            return 3;
        }

        if (!program[location.i][location.j++]) {
            location.i += 1;
            location.j = 0;
        }
    }

    free(line);
    free_all(program, brackets, array, breakpoints);
    return command_result;
}
