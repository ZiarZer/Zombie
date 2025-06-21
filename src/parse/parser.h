#ifndef PARSER_H
#define PARSER_H

#include "error.h"
#include "instruction.h"
#include "source_file.h"

struct instruction *scan_source(struct source_file *src_file);

int bind_matching_operations(struct instruction *instructions, struct source_file *src_file);

#endif /* !PARSER_H */
