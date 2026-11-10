#include "raylib.h"

int main(void) {
    // Configure window to be resizable
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "The Game");

    // Maximize window to fill the screen on desktop
    MaximizeWindow();

    SetTargetFPS(60);

    Color background = { 238, 238, 238, 255 };

    while (!WindowShouldClose()) {
        // Toggle fullscreen mode on F11 key press
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        BeginDrawing();
        ClearBackground(background);

        // Step 1 add your shape rendering code here
        // Draw a red square using DrawRectangle
        // Draw a green circle using DrawCircle
        // Draw a blue rectangle outline using DrawRectangleLines

        EndDrawing();
    }

    CloseWindow();
    return 0;
}