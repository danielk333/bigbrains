#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <stdlib.h>

#include "lexer.h"

void extend_tape(int **tape, size_t *size, size_t extension);
void execute_token_stream(struct Operation *tokens, size_t token_num);

#endif // INTERPRETER_H_