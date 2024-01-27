#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_escaped(char ch) {
    switch (ch) {
        case '\'':
            printf("'\\''"); break;
        case '\"':
            printf("'\\\"'"); break;
        case '\\':
            printf("'\\\\'"); break;
        case '\n':
            printf("'\\n'"); break;
        case '\t':
            printf("'\\t'"); break;
        case '\r':
            printf("'\\r'"); break;
        default:
            printf("'%c'", ch);
    }
}


int main(int argc, char *argv[]) {

    char *file = argv[1];
    char *variable_name = argv[2];

    FILE* cursor;
    cursor = fopen(file, "r");

    if (!cursor) {
        printf("Could not open file '%s'\n", file);
        return 1;
    }

    fseek(cursor, 0, SEEK_END);
    size_t file_size = ftell(cursor);
    fseek(cursor, 0, SEEK_SET);

    printf("#include <stdlib.h>\n\n");
    printf("/* Embedded code: %s*/\n", file);
    printf("size_t %s_size = %zu;\n", variable_name, file_size);
    printf("char %s[%zu] = {", variable_name, file_size);

    char ch;
    for (size_t ind = 0; ind < file_size; ind++) {
        ch = fgetc(cursor);
        print_escaped(ch);
        if (ind < file_size - 1) {
            printf(",");
        }
    }
    printf("};\n");
    fclose(cursor);

    return 0;
}