#ifndef TRANSPILER_H_
#define TRANSPILER_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

void write_ops(struct Operation *tokens, size_t token_num, FILE* cursor);
void write_instructions_c(struct Operation *tokens, size_t token_num, char* out_path);
void transpile_c(struct Operation *tokens, size_t token_num, char* out_path);
void transpile_fasm(struct Operation *tokens, size_t token_num, char* out_path);

#endif // TRANSPILER_H_