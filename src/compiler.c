#include "interpreter_source.c"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "interpreter.c"

void write_ops(Operation *tokens, size_t token_num, FILE* cursor) {
    fprintf(cursor, "size_t token_num = %zu;\n", token_num);
    fprintf(cursor, "Operation tokens[] = {\n");
    for (size_t ind=0; ind < token_num; ind++) {
        fprintf(cursor, "    [%zu].code = '%c',\n", ind, tokens[ind].code);
        fprintf(cursor, "    [%zu].num = %u,\n", ind, tokens[ind].num);
        fprintf(cursor, "    [%zu].target = %zu,\n", ind, tokens[ind].target);
    }
    fprintf(cursor, "};\n");
}

void write_instructions_c(Operation *tokens, size_t token_num, char* tmp_path) {
    FILE* cursor;
    printf("Writing temp source to: %s", tmp_path);
    cursor = fopen(tmp_path, "w");
    fprintf(cursor, "%s\n", interpreter_source);
    fprintf(cursor, "int main() {\n");
    write_ops(tokens, token_num, cursor);
    fprintf(cursor, "execute(tokens, token_num);\n");
    fprintf(cursor, "}\n");
    fclose(cursor);
}

void compile_c(Operation *tokens, size_t token_num, char* tmp_path, char* out_path) {
    write_instructions_c(tokens, token_num, tmp_path);
    char syscall[100];
    sprintf(syscall, "gcc %s -o %s", tmp_path, out_path);
    system(syscall);

    sprintf(syscall, "rm -v %s", tmp_path);
    system(syscall);
}