#include "raylib.h"
#include "raymath.h"

#include <stdio.h>

#include "dustin.h"

#define SCREEN_WIDTH (1920)
#define SCREEN_HEIGHT (1080)

#define WINDOW_TITLE "DustInTime"

////////////////////////////
// TYPE

#define ZOOM_SPEED 0.05f

typedef struct {
    Vector2 position;
    Vector2 size;
    Texture2D texture;
} box_t;

///////////////////////////////

float clampf(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

///////////////////////////////

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    Camera2D camera = {0};
    camera.target = (Vector2) {0.f, 0.f};
    camera.rotation = 0.f;
    camera.zoom = 1.f;
    camera.offset.x = SCREEN_WIDTH * .5f;
    camera.offset.y = SCREEN_HEIGHT * .5f;

    dustin_t dustin = { 0 };
    dustin_init(&dustin);

    const Texture2D ground_test = LoadTexture(ASSETS_PATH"ground_test.png");
    const Vector2 ground_pos[] = {
            {100.f,  0.f},
            {200.f,  0.f},
            {-400.f, 0.f},
            {100.f,  300.f},
            {100.f,  -400.f},
            {300.f,  -200.f},
            {300.f,  -400.f},
    };
    const int grounds_count = sizeof(ground_pos) / sizeof(*ground_pos);

    // main loop
    while (!WindowShouldClose()) {
        const float dt = GetFrameTime();

        dustin_update(&dustin, dt);
        camera.target = dustin.position;

        // Zoom
        if (fabsf(GetMouseWheelMoveV().y) > 0.05) {
            camera.zoom += (GetMouseWheelMoveV().y > 0) ? ZOOM_SPEED : -ZOOM_SPEED;
            clampf(camera.zoom, 0.05f, 1.f);
        }

        // render
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
        {
            // draw some tiles
            for (int i = 0; i < grounds_count; ++i) {
                DrawTexture(ground_test, (int) ground_pos[i].x, (int) ground_pos[i].y, WHITE);
            }

            dustin_render(&dustin);
        }
        EndMode2D();


        char text[256];
        snprintf(text, 256, "Pos: %3.4f / %3.4f\n Dir: %4d\n Dodge: %3.4f\n", dustin.position.x, dustin.position.y,
                 dustin.last_direction, dustin.is_dodging);
        DrawText(text, 0, 10, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
