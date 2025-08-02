#ifndef PARSER_H
#define PARSER_H

#include "error.h"
#include "instruction.h"
#include "source_file.h"
#include "option_flag.h"

struct instruction *scan_source(struct source_file *src_file, struct program_params *params);

int bind_matching_operations(struct instruction *instructions, struct source_file *src_file);

#endif /* !PARSER_H */
