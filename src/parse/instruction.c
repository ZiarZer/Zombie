#include "instruction.h"

char get_char_from_instruction_type(enum instruction_type type) {
    switch (type) {
    case ADD_INSTRUCTION:
        return '+';
    case SUB_INSTRUCTION:
        return '-';
    case LEFT_INSTRUCTION:
        return '<';
    case RIGHT_INSTRUCTION:
        return '>';
    case PRINT_INSTRUCTION:
        return '.';
    case SCAN_INSTRUCTION:
        return ',';
    case LOOP_INSTRUCTION:
        return '[';
    case ENDLOOP_INSTRUCTION:
        return ']';
    case SLEEP_INSTRUCTION:
        return '|';
    case LOCK_INSTRUCTION:
        return '#';
    case UNLOCK_INSTRUCTION:
        return '%';
    case NEWTHREAD_INSTRUCTION:
        return '{';
    case ENDTHREAD_INSTRUCTION:
        return '}';
    default:
        return '\0';
    }
}

enum instruction_type get_matching_type(enum instruction_type type) {
    switch (type) {
    case LOOP_INSTRUCTION:
        return ENDLOOP_INSTRUCTION;
    case ENDLOOP_INSTRUCTION:
        return LOOP_INSTRUCTION;
    case NEWTHREAD_INSTRUCTION:
        return ENDTHREAD_INSTRUCTION;
    case ENDTHREAD_INSTRUCTION:
        return NEWTHREAD_INSTRUCTION;
    default:
        return ENDPROGRAM_INSTRUCTION;
    }
}

struct instruction make_instruction(struct location location, enum instruction_type type) {
    struct instruction new_instruction = { location, type, NULL, false };
    return new_instruction;
}

struct instruction *find_instruction(struct instruction *instructions, ssize_t i, ssize_t j) {
    for (size_t index = 0; (instructions + index)->type != ENDPROGRAM_INSTRUCTION; index++) {
        struct instruction *instruction = instructions + index;
        struct location *location = &instruction->location;
        if (location->i == i && location->j == j) {
            return instruction;
        }
    }
    return NULL;
}

void add_breakpoint(struct instruction *instructions, size_t i, size_t j) {
    struct instruction *instruction = find_instruction(instructions, i, j);
    if (instruction) {
        instruction->has_breakpoint = true;
    }
}

void remove_breakpoint(struct instruction *instructions, size_t i, size_t j) {
    struct instruction *instruction = find_instruction(instructions, i, j);
    if (instruction) {
        instruction->has_breakpoint = false;
    }
}
