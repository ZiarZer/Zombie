#include "instruction.h"

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
