#include "visualizer.h"


int render_gui(struct Operation *tokens, size_t token_num, char *file) {
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "vizbrains");
    SetExitKey(KEY_ESCAPE);

    while (!WindowShouldClose())
    {
        // if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 2.0f;
        // if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 2.0f;
        // if (IsKeyDown(KEY_UP)) ballPosition.y -= 2.0f;
        // if (IsKeyDown(KEY_DOWN)) ballPosition.y += 2.0f;

        BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawRectangle(screenWidth/4*2 - 60, 100, 120, 60, RED);
            DrawText("VISUALIZE", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}