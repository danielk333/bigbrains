#ifndef LEXER_H_
#define LEXER_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

enum OpCode {
    Right = '>',
    Left = '<',
    Increment = '+',
    Decrement = '-',
    Output = '.',
    Input = ',',
    GotoIfZero = '[',
    GotoIfNonezero = ']',
    Comment = 'C',
};


struct Operation {
    enum OpCode code;
    unsigned int num;
    size_t target;
};


struct Propgram {
    struct Operation *ops;
    int *data;
    size_t head;
    size_t pos;
};


struct LOps {
    struct Operation op;
    size_t op_ind;
    struct LOps *next;
};


struct SubLOps {
    struct LOps *val;
    struct SubLOps *next;
};

enum OpCode get_OpCode(char ch);
struct SubLOps *SubLOps_push(struct SubLOps *head, struct LOps *val);
struct LOps *SubLOps_pop(struct SubLOps *head);
struct LOps* LOps_init(struct Operation op);
struct LOps *LOps_push(struct LOps *head, struct Operation op);
struct Operation LOps_pop(struct LOps *head);
size_t LOps_len(struct LOps *head);
struct Operation* LOps_to_array(struct LOps *head, size_t *len);
unsigned int count_next_ops(FILE *cursor, enum OpCode *code, char *ch);
struct Operation* lex_source(FILE *cursor, size_t *len);


#endif // LEXER_H_