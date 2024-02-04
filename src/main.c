#include "bigbrains.h"


enum Command {
    Run = 0,
    Transpile = 1,
    Visualize = 2,
};


void usage() {
    printf("Usage:\n");

    printf("bigbrains run INPUT_FILE\n");
    printf("bigbrains transpile INPUT_FILE [--instruction_set=(c|fasm)]\n");
    printf("bigbrains visualize INPUT_FILE\n");
}


int exists(const char *fname) {
    FILE *cursor = fopen(fname, "r");
    if (cursor) {
        fclose(cursor);
        return 1;
    }
    return 0;
}


enum Command get_command(char *name) {
    if (strcmp(name, "run") == 0) {
        return Run;
    }
    else if (strcmp(name, "transpile") == 0) {
        return Transpile;
    }
    else if (strcmp(name, "visualize") == 0) {
        return Visualize;
    }
    else {
        printf("Command not recognized\n");
        usage();
        exit(1);
    }
}


int run(int args, char **argv) {
    if (args < 3) {
        printf("Too many input arguments\n");
        usage();
        return 1;
    }
    else if (args > 4) {
        printf("Too many input arguments\n");
        usage();
        return 1;
    }
    char *file;
    FILE* cursor;
    size_t token_num;

    file = argv[2];

    // Lex source
    cursor = fopen(file, "r");
    struct Operation *tokens = lex_source(cursor, &token_num);
    fclose(cursor);

    // Execute interpreter
    execute_token_stream(tokens, token_num);

    free(tokens);
    return 0;
}


int transpile(int args, char **argv) {
    if (args < 3) {
        printf("Too many input arguments\n");
        usage();
        return 1;
    }
    else if (args > 5) {
        printf("Too many input arguments\n");
        usage();
        return 1;
    }

    char *file;
    FILE* cursor;
    size_t token_num;
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
            }
            else {
                printf("Given instruction set not supported\n");
                usage();
                return 1;
            }
        }
        else {
            printf("option format not recognized\n");
            usage();
            return 1;
        }
    }

    file = argv[2];

    // Lex source
    cursor = fopen(file, "r");
    struct Operation *tokens = lex_source(cursor, &token_num);
    fclose(cursor);

    char out_path[80];
    sprintf(out_path, "%s.bb", file);
    if(instruction_set == 0) {            
        char *tmp_path = "_tmp.c";
        transpile_c(tokens, token_num, tmp_path, out_path);
    }
    else if (instruction_set == 1) {
        transpile_fasm(tokens, token_num, out_path);
    }

    free(tokens);
    return 0;
}


int visualize(int args, char **argv) {
    if (args < 3) {
        printf("Too many input arguments\n");
        usage();
        return 1;
    }
    else if (args > 4) {
        printf("Too many input arguments\n");
        usage();
        return 1;
    }
    size_t token_num;
    FILE *cursor;
    char *file = argv[1];

    if (exists(file) == 1) {
        cursor = fopen(file, "r");
        struct Operation *tokens = lex_source(cursor, &token_num);
    }
    else {
        cursor = fopen(file, "w");
    }

    // Lex source
    cursor = fopen(file, "r");
    struct Operation *tokens = lex_source(cursor, &token_num);
    fclose(cursor);

    // Execute interpreter
    int result = render_gui(tokens, token_num, file);

    free(tokens);
    return result;
}

int main(int args, char **argv) {
    if (args < 2) {
        printf("Need a sub-command argument\n");
        usage();
        return 1;
    }
    char *command = argv[1];
    enum Command cmd = get_command(command);
    int result;

    switch (cmd) {
        case Run:
            result = run(args, argv);
            break;
        case Transpile:
            result = transpile(args, argv);
            break;
        case Visualize:
            result = visualize(args, argv);
            break;
        default:
            printf("Command not recognized\n");
            usage();
            result = 1;
    }
    return result;
}