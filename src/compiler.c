#include "interpreter_source.c"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void write_ops(FILE* cursor) {
    fprintf(cursor, "Operation tokens[] = {\n");
    fprintf(cursor, "[0].code = '+',\n");
    fprintf(cursor, "[0].num = 8,\n");
    fprintf(cursor, "[0].target = 0,\n");
    fprintf(cursor, "};\n");
}

void write_instructions_c(char* tmp_path) {
    FILE* cursor;
    printf("Writing temp source to: %s", tmp_path);
    cursor = fopen(tmp_path, "w");
    fprintf(cursor, "%s\n", interpreter_source);
    fprintf(cursor, "int main() {\n");
    write_ops(cursor);
    fprintf(cursor, "execute(tokens, token_num);\n");
    fprintf(cursor, "}\n");
    fclose(cursor);
}

void compile_c(char* tmp_path, char* out_path) {
    write_instructions_c(tmp_path);
    // char *syscall;
    // sprintf(syscall, "gcc %s -o %s", tmp_path, out_path);
    // system(syscall);

    // sprintf(syscall, "rm -v %s", tmp_path);
    // system(syscall);
}