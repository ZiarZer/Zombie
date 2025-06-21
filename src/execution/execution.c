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

enum instruction_result execute_instruction(struct instruction **current_instruction, struct memory_array *array) {
    char input;
    enum instruction_type instruction_type = (*current_instruction)->type;
    switch (instruction_type) {
    case LEFT_INSTRUCTION:
        return array_move(array, array->cursor - 1) ? POINTER_LOWER_ERROR : DONE;
    case RIGHT_INSTRUCTION:
        return array_move(array, array->cursor + 1) ? POINTER_UPPER_ERROR : DONE;
    case ADD_INSTRUCTION:
    case SUB_INSTRUCTION:
        return modify_pointed_value(array, instruction_type == ADD_INSTRUCTION);
    case PRINT_INSTRUCTION:
        putchar(array_get_current(array));
        break;
    case SCAN_INSTRUCTION:
        input = getchar();
        array_set_current(array, input == EOF ? '\0' : input);
        break;
    case LOOP_INSTRUCTION:
        if (!array_get_current(array)) {
            *current_instruction = (*current_instruction)->matching_instruction;
            return FOUND_BRACKET;
        }
        break;
    case ENDLOOP_INSTRUCTION:
        if (array_get_current(array)) {
            *current_instruction = (*current_instruction)->matching_instruction;
            return FOUND_BRACKET;
        }
        break;
    default:
        break;
    }
    return DONE;
}

int run_program(struct source_file *src_file, struct instruction *instructions, ssize_t array_size) {
    struct memory_array *array = array_init(array_size);

    struct location location = { src_file->filename, 0, 0 };
    int command_result = 0;
    char **program = src_file->program;

    struct instruction *current_instruction = instructions;

    while (current_instruction->type != ENDPROGRAM_INSTRUCTION) {
        command_result = execute_instruction(&current_instruction, array);
        if (command_result != FOUND_BRACKET && command_result != DONE) {
            print_runtime_error(program, current_instruction->location, command_result);
            free_all(program, array, NULL);
            return 3;
        } else if (array->cursor > array->high_bound || array->cursor < array->low_bound) {
            print_runtime_error(program, location,
                                array->cursor < array->low_bound ? POINTER_LOWER_ERROR : POINTER_UPPER_ERROR);
            free_all(program, array, NULL);
            return 3;
        }

        current_instruction += 1;
    }

    free_all(program, array, NULL);
    return command_result;
}

int run_debug_mode(struct source_file *src_file, struct instruction *instructions, ssize_t array_size) {
    struct memory_array *array = array_init(array_size);

    int command_result = 0;
    char **program = src_file->program;

    struct debug_command previous_command = { NONE, 0, 0 };
    enum debug_run_state run_state = PAUSED;
    map *breakpoints = NULL;

    print_debug_mode_intro();

    int must_display_program_location = 1;
    size_t n;
    char *line = NULL;

    struct instruction *current_instruction = instructions;
    while (current_instruction->type != ENDPROGRAM_INSTRUCTION) {
        if (run_state == PAUSED || run_state == STEPPING) {
            if (must_display_program_location) {
                fputs("--------------------------\n", stderr);
                display_program_location(program, current_instruction->location, BLUE);
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
        log_operation(program, array, current_instruction->location);
        if (run_state == RUNNING) {
            if (find_breakpoint(breakpoints, current_instruction->location.i + 1,
                                current_instruction->location.j + 1)) {
                run_state = PAUSED;
                fprintf(stderr, "Breakpoint at %ld:%ld, pausing execution.\n", current_instruction->location.i + 1,
                        current_instruction->location.j + 1);
            }
        }

        command_result = execute_instruction(&current_instruction, array);
        must_display_program_location = 1;
        if (command_result == FOUND_BRACKET) {
            log_operation(program, array, current_instruction->location);
        } else if (command_result != DONE) {
            print_runtime_error(program, current_instruction->location, command_result);
            free_all(program, array, NULL);
            return 3;
        } else if (array->cursor > array->high_bound || array->cursor < array->low_bound) {
            print_runtime_error(program, current_instruction->location,
                                array->cursor < array->low_bound ? POINTER_LOWER_ERROR : POINTER_UPPER_ERROR);
            free_all(program, array, NULL);
            return 3;
        }

        current_instruction += 1;
    }

    free(line);
    free_all(program, array, breakpoints);
    return command_result;
}
