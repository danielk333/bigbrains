#include "transpiler.h"


void write_ops(struct Operation *tokens, size_t token_num, FILE* cursor) {
    fprintf(cursor, "size_t token_num = %zu;\n", token_num);
    fprintf(cursor, "struct Operation tokens[] = {\n");
    for (size_t ind=0; ind < token_num; ind++) {
        fprintf(cursor, "    [%zu].code = '%c',\n", ind, tokens[ind].code);
        fprintf(cursor, "    [%zu].num = %u,\n", ind, tokens[ind].num);
        fprintf(cursor, "    [%zu].target = %zu,\n", ind, tokens[ind].target);
    }
    fprintf(cursor, "};\n");
}

void write_instructions_c(struct Operation *tokens, size_t token_num, char* tmp_path) {
    FILE* cursor;
    printf("Writing temp source to: %s", tmp_path);
    cursor = fopen(tmp_path, "w");
    // fprintf(cursor, "%s\n", interpreter_source);
    fprintf(cursor, "int main() {\n");
    write_ops(tokens, token_num, cursor);
    fprintf(cursor, "execute(tokens, token_num);\n");
    fprintf(cursor, "}\n");
    fclose(cursor);
}

void transpile_c(struct Operation *tokens, size_t token_num, char* tmp_path, char* out_path) {
    write_instructions_c(tokens, token_num, tmp_path);
}

void transpile_fasm(struct Operation *tokens, size_t token_num, char* out_path) {
    printf("TODO!");
    exit(1);
}