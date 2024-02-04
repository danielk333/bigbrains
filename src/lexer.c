#include "lexer.h"


enum OpCode get_OpCode(char ch) {
    enum OpCode code;
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

struct SubLOps *SubLOps_push(struct SubLOps *head, struct LOps *val) {
    struct SubLOps *current = head;
    if (current->val == NULL) {
        current->val = val;
        current->next = NULL;
        return current;
    }
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (struct SubLOps *) malloc(sizeof(struct SubLOps));
    current->next->val = val;
    current->next->next = NULL;
    return current->next;
}

struct LOps *SubLOps_pop(struct SubLOps *head) {
    struct SubLOps *current = head;
    struct SubLOps *prev;
    struct LOps *val;
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

struct LOps* LOps_init(struct Operation op) {
    struct LOps *head;
    head = (struct LOps *) malloc(sizeof(struct LOps));
    head->op = op;
    head->op_ind = 0;
    head->next = NULL;
    return head;
}

struct LOps *LOps_push(struct LOps *head, struct Operation op) {
    struct LOps *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (struct LOps *) malloc(sizeof(struct LOps));
    current->next->op = op;
    current->next->op_ind = current->op_ind + 1;
    current->next->next = NULL;
    return current->next;
}

struct Operation LOps_pop(struct LOps *head) {
    struct LOps *current = head;
    struct LOps *prev;
    struct Operation val;
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

size_t LOps_len(struct LOps *head) {
    size_t len = 0;
    struct LOps *current = head;
    while (current->next != NULL) {
        len++;
        current = current->next;
    }
    return len;
}

struct Operation* LOps_to_array(struct LOps *head, size_t *len) {
    *len = LOps_len(head);
    size_t ind = 0;
    struct Operation *ops;
    ops = (struct Operation *) malloc((*len)*sizeof(struct Operation));

    struct LOps *current = head;
    struct LOps *prev;

    while (current->next != NULL) {
        ops[ind++] = current->op;
        prev = current;
        current = current->next;
        free(prev);
    }
    return ops;
}

unsigned int count_next_ops(FILE *cursor, enum OpCode *code, char *ch) {
    unsigned int num = 1;
    *ch = fgetc(cursor);
    if (*ch == EOF) {
        return num;
    }
    enum OpCode next_code = get_OpCode(*ch);
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

struct Operation* lex_source(FILE *cursor, size_t *len) {
    char ch = '!';
    enum OpCode code;

    struct LOps *head = NULL;
    struct LOps *tail;
    struct LOps *open_bracket;
    struct SubLOps open_brackets;
    open_brackets.val = NULL;
    open_brackets.next = NULL;

    // Get first op
    ch = fgetc(cursor);
    code = get_OpCode(ch);
    while (ch != EOF) {

        struct Operation next_op;
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
            // Get the next struct Operation
            ch = fgetc(cursor);
            code = get_OpCode(ch);
        }
        else {
            // All other struct Operations can be compacted
            // Count number and get next op
            tail->op.num = count_next_ops(cursor, &code, &ch);
            #ifdef DEBUG
                printf("%c x %d\n", tail->op.code, tail->op.num);
            #endif
        }
    };
    struct Operation* tokens = LOps_to_array(head, len);
    return tokens;
}