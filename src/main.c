#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "interpreter.c"
#include "compiler.c"

// #define DEBUG
// #define FULL_DEBUG

typedef struct LOps {
    Operation op;
    size_t op_ind;
    struct LOps *next;
} LOps;

typedef struct SubLOps {
    struct LOps *val;
    struct SubLOps *next;
} SubLOps;

SubLOps *SubLOps_push(SubLOps *head, LOps *val) {
    SubLOps *current = head;
    if (current->val == NULL) {
        current->val = val;
        current->next = NULL;
        return current;
    }
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (SubLOps *) malloc(sizeof(SubLOps));
    current->next->val = val;
    current->next->next = NULL;
    return current->next;
}

LOps *SubLOps_pop(SubLOps *head) {
    SubLOps *current = head;
    SubLOps *prev;
    LOps *val;
    if (current->next == NULL) {
        val = current->val;
        head->val = NULL;
        return val;
    }
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }
    val = current->val;
    prev->next = NULL;
    free(current);
    return val;
}

LOps* LOps_init(Operation op) {
    LOps *head;
    head = (LOps *) malloc(sizeof(LOps));
    head->op = op;
    head->op_ind = 0;
    head->next = NULL;
    return head;
}

LOps *LOps_push(LOps *head, Operation op) {
    LOps *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (LOps *) malloc(sizeof(LOps));
    current->next->op = op;
    current->next->op_ind = current->op_ind + 1;
    current->next->next = NULL;
    return current->next;
}

Operation LOps_pop(LOps *head) {
    LOps *current = head;
    LOps *prev;
    Operation val;
    if (current->next == NULL) {
        val = current->op;
        free(current);
        head = NULL;
        return val;
    }
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }
    val = current->op;
    prev->next = NULL;
    free(current);
    return val;
}

size_t LOps_len(LOps *head) {
    size_t len = 0;
    LOps *current = head;
    while (current->next != NULL) {
        len++;
        current = current->next;
    }
    return len;
}

Operation* LOps_to_array(LOps *head, size_t *len) {
    *len = LOps_len(head);
    size_t ind = 0;
    Operation *ops;
    ops = (Operation *) malloc((*len)*sizeof(Operation));

    LOps *current = head;
    LOps *prev;

    while (current->next != NULL) {
        ops[ind++] = current->op;
        prev = current;
        current = current->next;
        free(prev);
    }
    return ops;
}

unsigned int count_next_ops(FILE *cursor, OpCode *code, char *ch) {
    unsigned int num = 1;
    *ch = fgetc(cursor);
    if (*ch == EOF) {
        return num;
    }
    OpCode next_code = get_OpCode(*ch);
    while (*code == next_code) {
        num++;
        *ch = fgetc(cursor);
        next_code = get_OpCode(*ch);
        if (*ch == EOF) {
            break;
        }
    }
    (*code) = next_code;
    return num;
}

Operation* lex_source(FILE *cursor, size_t *len) {
    char ch = '!';
    OpCode code;

    LOps *head = NULL;
    LOps *tail;
    LOps *open_bracket;
    SubLOps open_brackets;
    open_brackets.val = NULL;
    open_brackets.next = NULL;

    // Get first op
    ch = fgetc(cursor);
    code = get_OpCode(ch);
    while (ch != EOF) {

        Operation next_op;
        next_op.code = code;
        next_op.target = 0; // Only used by goto commands
        if (head == NULL) {
            head = LOps_init(next_op);
            tail = head;
        }
        else {
            tail = LOps_push(tail, next_op);
        }

        if (code == GotoIfZero || code == GotoIfNonezero) {
            tail->op.target = 0;
            tail->op.num = 1;
            if (code == GotoIfZero) {
                // Open a bracket
                SubLOps_push(&open_brackets, tail);
            }
            else {
                // Close the bracket and set targets
                if (open_brackets.val == NULL) {
                    printf("UNMACHED BRACKETS\n");
                    exit(1);
                }
                open_bracket = SubLOps_pop(&open_brackets);
                open_bracket->op.target = tail->op_ind;
                tail->op.target = open_bracket->op_ind;

                #ifdef DEBUG
                    printf("[ -> IND {%zu} | ] -> IND {%zu}\n", open_bracket->op.target, tail->op.target);
                #endif
            }
            // Get the next operation
            ch = fgetc(cursor);
            code = get_OpCode(ch);
        }
        else {
            // All other operations can be compacted
            // Count number and get next op
            tail->op.num = count_next_ops(cursor, &code, &ch);
            #ifdef DEBUG
                printf("%c x %d\n", tail->op.code, tail->op.num);
            #endif
        }
    };
    Operation* tokens = LOps_to_array(head, len);
    return tokens;
}

void usage() {
    printf("Usage:\n");
    printf("bigbrains run INPUT_FILE\n");
    printf("bigbrains build INPUT_FILE [--instruction_set=(c|fasm)]\n");
}

int main(int args, char **argv) {
    if (args < 3) {
        printf("Need input file as argument\n");
        usage();
        return 1;
    }
    else if (args > 5) {
        printf("Too many input arguments\n");
        usage();
        return 1;
    }
    char *command = argv[1];
    char *file = argv[2];
    char *opt;
    int instruction_set = 0;
    if (args == 4) {
        opt = argv[3];
        if (strncmp(opt, "--instruction_set=", 18) == 0) {
            if (strcmp((opt + 18), "c") == 0) {
                instruction_set = 0;
            }
            else if (strcmp((opt + 18), "fasm") == 0) {
                instruction_set = 1;
                printf("TODO\n");
                return 1;
            }
            else {
                printf("Given instruction set not supported\n");
                usage();
            }
        }
        else {
            printf("option format not recognized\n");
            usage();
        }
    }

    FILE* cursor;
    size_t token_num;
    // Lex source
    cursor = fopen(file, "r");
    Operation *tokens = lex_source(cursor, &token_num);
    fclose(cursor);

    if (strcmp(command, "run") == 0) {
        // Execute interpreter
        execute(tokens, token_num);
    }
    else if (strcmp(command, "build") == 0) {
        if(instruction_set == 0) {            
            char *tmp_path = "_tmp.c";
            char out_path[80];
            sprintf(out_path, "%s.bb", file);
            compile_c(tokens, token_num, tmp_path, out_path);
        }
    }
    else {
        printf("Command not recognized; use 'build' or 'run'.\n");
        return 1;
    }

    // Clean up
    free(tokens);
    return 0;
}