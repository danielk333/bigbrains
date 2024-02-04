#include "visualizer.h"

#define CELL_COLOR CLITERAL(Color) { 69, 69, 69, 255 }
#define OP_COLOR CLITERAL(Color) { 69, 69, 69, 255 }
#define SELECT_COLOR CLITERAL(Color) { 115, 66, 61, 255 }
#define BG_COLOR CLITERAL(Color) {0, 32, 54, 255}

int render_gui(struct Operation *tokens, size_t token_num, char *file) {
    int screenWidth = 800;
    int screenHeight = 450;
    int cellFontSize = 20;
    int opFontSize = 20;

    int cellHeight = 100;
    int cellWidth = 100;
    int cellPadding = 20;
    int tapeLocation = (int)(screenHeight*0.2);
    int cells_shown = (int)(screenWidth/(cellWidth + cellPadding));
    int selected_cell = (int)(cells_shown / 2);

    int opHeight = 60;
    int opWidth = 60;
    int opPadding = 10;
    int opLocation = (int)(screenHeight*0.75);
    int ops_shown = (int)(screenWidth/(opWidth + opPadding));
    int selected_op = 0;


    struct Propgram prog = {};

    size_t tape_chunk = 1024;
    size_t data_size = 0;
    int *data;

    prog.head = 0;
    prog.pos = 0;
    prog.data = data;
    prog.ops = tokens;

    if (prog.head >= data_size) {
        extend_tape(&prog.data, &data_size, tape_chunk);
    }

    bool program_running = true;
    struct Operation op;
    size_t total_ops = 0;

    InitWindow(screenWidth, screenHeight, "vizbrains");
    SetExitKey(KEY_ESCAPE);

    char text[256];
    int offset = 0;
    Color cellc;
    Color opc;

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_SPACE)) {
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
            prog.pos++;
            if (prog.pos == token_num) {
                program_running = false;
            }
            // Make sure only one update is being done
            WaitTime(1);
        }

        BeginDrawing();
            ClearBackground(BG_COLOR);
            offset = prog.head - selected_cell;
            for(int ind = 0; ind < cells_shown; ind++) {
                if (ind == selected_cell) {cellc = SELECT_COLOR;}
                else {cellc = CELL_COLOR;}

                DrawRectangle(cellWidth*ind + cellPadding*(ind + 1), tapeLocation, cellWidth, cellHeight, cellc);
                if (ind + offset < 0) {
                    sprintf(text, "nan");
                }
                else {
                    sprintf(text, "%d", prog.data[ind + offset]);
                }
                DrawText(
                    text,
                    cellWidth*ind + cellPadding*(ind + 1) + (int)(cellWidth/2),
                    tapeLocation + (int)(cellHeight/2),
                    cellFontSize, WHITE
                );

                sprintf(text, "%d", ind + offset);
                DrawText(
                    text,
                    cellWidth*ind + cellPadding*(ind + 1),
                    tapeLocation + cellHeight + cellPadding,
                    cellFontSize, WHITE
                );
            }
            for(int ind = 0; ind < ops_shown; ind++) {
                if (ind == selected_op) {opc = SELECT_COLOR;}
                else {opc = OP_COLOR;}

                DrawRectangle(opWidth*ind + opPadding*(ind + 1), opLocation, opWidth, opHeight, opc);
                if (ind + prog.pos >= token_num) {
                    sprintf(text, "");
                }
                else {
                    sprintf(text, "%c (%d)", prog.ops[ind + prog.pos].code, prog.ops[ind + prog.pos].num);
                }
                DrawText(
                    text,
                    opWidth*ind + opPadding*(ind + 1) + opPadding,
                    opLocation + (int)(opHeight/2),
                    opFontSize, WHITE
                );
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}