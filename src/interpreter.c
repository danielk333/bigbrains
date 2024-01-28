#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// #define DEBUG
// #define FULL_DEBUG

typedef enum OpCode {
    Right = '>',
    Left = '<',
    Increment = '+',
    Decrement = '-',
    Output = '.',
    Input = ',',
    GotoIfZero = '[',
    GotoIfNonezero = ']',
    Comment = 'C',
} OpCode;


typedef struct Operation {
    OpCode code;
    unsigned int num;
    size_t target;
} Operation;


typedef struct Propgram {
    Operation *ops;
    int *data;
    size_t head;
    size_t pos;
} Program;


OpCode get_OpCode(char ch) {
    OpCode code;
    switch (ch) {
        case '>':
        case '<':
        case '+':
        case '-':
        case '.':
        case ',':
        case '[':
        case ']':
            code = ch;
            return code;
        default:
            return Comment;
    }
}


void extend_tape(int **tape, size_t *size, size_t extension) {
    size_t new_size = (*size) + extension;
    int *new_tape = (int*) malloc(new_size*sizeof(int));

    // Write old data to new tape
    for (size_t ind = 0; ind < (*size); ind++) {
        new_tape[ind] = (*tape)[ind];
    }
    for (size_t ind = *size; ind < new_size; ind++) {
        new_tape[ind] = 0;
    }
    if (*size > 0) {
        free(*tape);
    }
    *tape = new_tape;
    *size = new_size;
}


void execute(Operation *tokens, size_t token_num) {
    Program prog = {};

    size_t tape_chunk = 1024;
    size_t data_size = 0;
    int *data;

    prog.head = 0;
    prog.pos = 0;
    prog.data = data;
    prog.ops = tokens;

    bool program_running = true;
    Operation op;
    size_t total_ops = 0;
    while (program_running) {
        total_ops++;
        if (prog.head >= data_size) {
            extend_tape(&prog.data, &data_size, tape_chunk);
        }

        op = prog.ops[prog.pos];

        switch (op.code) {
            case Right:
                prog.head += op.num;
                break;
            case Left:
                prog.head -= op.num;
                break;
            case Increment:
                prog.data[prog.head] += op.num;
                break;
            case Decrement:
                prog.data[prog.head] -= op.num;
                break;
            case Output:
                for (int i = 0; i < op.num; i++) {
                    printf("%c", (char)prog.data[prog.head]);
                }
                break;
            case Input:
                for (int i = 0; i < op.num; i++) {
                    prog.data[prog.head] = getchar();
                }
                break;
            case GotoIfZero:
                if (prog.data[prog.head] == 0) {
                    prog.pos = op.target;
                }
                break;
            case GotoIfNonezero:
                if (prog.data[prog.head] != 0) {
                    prog.pos = op.target;
                }
                break;
            case Comment:
                break;
        }

        #ifdef FULL_DEBUG
            printf("%zu=op: %c x %d, pointer: %zu, data: %d ", prog.pos, op.code, op.num, prog.head, prog.data[prog.head]);
            if (op.code == GotoIfZero || op.code == GotoIfNonezero) {
                printf("ADDR: {%zu}", op.target);
            }
            printf("\n");
        #endif

        prog.pos++;
        if (prog.pos == token_num) {
            program_running = false;
        }
    };
    #ifdef DEBUG
        printf("\nExecuted %zu total operations", total_ops);
    #endif
}
