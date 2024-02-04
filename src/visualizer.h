#ifndef VISUALIZER_H_
#define VISUALIZER_H_

#include <stdio.h>

#include "raylib.h"
#include "lexer.h"

int render_gui(struct Operation *tokens, size_t token_num, char *file);

#endif // VISUALIZER_H_