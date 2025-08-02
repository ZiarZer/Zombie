#include "parser.h"

struct instruction *scan_source(struct source_file *src_file, struct program_params *params) {
    size_t instruction_count = 0;
    struct instruction *instructions = malloc(sizeof(struct instruction));

    enum instruction_type instruction_type;
    char **program = src_file->program;
    size_t i = 0;
    size_t j = 0;
    for (i = 0; program[i]; i++) {
        for (j = 0; program[i][j]; j++) {
            switch (program[i][j]) {
            case '+':
                instruction_type = ADD_INSTRUCTION;
                break;
            case '-':
                instruction_type = SUB_INSTRUCTION;
                break;
            case '<':
                instruction_type = LEFT_INSTRUCTION;
                break;
            case '>':
                instruction_type = RIGHT_INSTRUCTION;
                break;
            case '[':
                instruction_type = LOOP_INSTRUCTION;
                break;
            case ']':
                instruction_type = ENDLOOP_INSTRUCTION;
                break;
            case '.':
                instruction_type = PRINT_INSTRUCTION;
                break;
            case ',':
                instruction_type = SCAN_INSTRUCTION;
                break;
            case ';':
                if (params->debug_mode) {
                    instruction_type = BREAK_INSTRUCTION;
                    break;
                }
            case '{':
                if (params->extension_enabled) {
                    instruction_type = NEWTHREAD_INSTRUCTION;
                    break;
                }
            case '}':
                if (params->extension_enabled) {
                    instruction_type = ENDTHREAD_INSTRUCTION;
                    break;
                }
            case '#':
                if (params->extension_enabled) {
                    instruction_type = LOCK_INSTRUCTION;
                    break;
                }
            case '%':
                if (params->extension_enabled) {
                    instruction_type = UNLOCK_INSTRUCTION;
                    break;
                }
            case '|':
                if (params->extension_enabled) {
                    instruction_type = SLEEP_INSTRUCTION;
                    break;
                }
            default:
                continue;
            }
            instruction_count += 1;
            instructions = realloc(instructions, sizeof(struct instruction) * (instruction_count + 1));
            instructions[instruction_count - 1] =
                make_instruction(make_location(src_file->filename, i, j), instruction_type);
        }
    }
    instructions[instruction_count] = make_instruction(make_location(src_file->filename, i, j), ENDPROGRAM_INSTRUCTION);
    return instructions;
}

int bind_matching_operations(struct instruction *instructions, struct source_file *src_file) {
    struct instruction **pending_left_brackets = NULL;
    size_t pending_left_brackets_count = 0;

    for (size_t i = 0; (instructions + i)->type != ENDPROGRAM_INSTRUCTION; i++) {
        struct instruction *current_instruction = instructions + i;
        if (current_instruction->type == LOOP_INSTRUCTION) {
            pending_left_brackets =
                realloc(pending_left_brackets, sizeof(struct instruction *) * (pending_left_brackets_count + 1));
            pending_left_brackets[pending_left_brackets_count++] = current_instruction;
        } else if (current_instruction->type == ENDLOOP_INSTRUCTION) {
            if (pending_left_brackets_count == 0) {
                missing_bracket_error_message(src_file->program, current_instruction->location);
                free(pending_left_brackets);
                return 0;
            }
            struct instruction *matching_left = pending_left_brackets[pending_left_brackets_count - 1];
            matching_left->matching_instruction = current_instruction;
            current_instruction->matching_instruction = matching_left;
            pending_left_brackets =
                realloc(pending_left_brackets, sizeof(struct instruction *) * (--pending_left_brackets_count));
        }
    }
    if (pending_left_brackets_count > 0) {
        struct instruction *last_left_bracket = pending_left_brackets[pending_left_brackets_count - 1];
        missing_bracket_error_message(src_file->program, last_left_bracket->location);
        free(pending_left_brackets);
        return 0;
    }
    free(pending_left_brackets);
    return 1;
}
