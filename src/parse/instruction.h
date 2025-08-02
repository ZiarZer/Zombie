#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdbool.h>

#include "location.h"

enum instruction_type {
    // Standard BrainFuck instructions
    ADD_INSTRUCTION,
    SUB_INSTRUCTION,
    LEFT_INSTRUCTION,
    RIGHT_INSTRUCTION,
    LOOP_INSTRUCTION,
    ENDLOOP_INSTRUCTION,
    PRINT_INSTRUCTION,
    SCAN_INSTRUCTION,
    ENDPROGRAM_INSTRUCTION,

    // --debug
    BREAK_INSTRUCTION,

    // --extension
    NEWTHREAD_INSTRUCTION,
    ENDTHREAD_INSTRUCTION,
    LOCK_INSTRUCTION,
    UNLOCK_INSTRUCTION,
    SLEEP_INSTRUCTION
};

struct instruction {
    struct location location;
    enum instruction_type type;
    struct instruction *matching_instruction;
    bool has_breakpoint;
};

struct instruction make_instruction(struct location location, enum instruction_type type);

void add_breakpoint(struct instruction *instructions, size_t i, size_t j);
void remove_breakpoint(struct instruction *instructions, size_t i, size_t j);

#endif /* !INSTRUCTION_H */
