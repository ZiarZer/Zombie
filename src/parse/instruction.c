#include "instruction.h"

struct instruction make_instruction(struct location location, enum instruction_type type) {
    struct instruction new_instruction = { location, type, NULL };
    return new_instruction;
}
