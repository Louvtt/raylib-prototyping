//
// Created by lucas on 30/04/2024.
//

#ifndef DUSTINTIME_DUSTIN_H
#define DUSTINTIME_DUSTIN_H

#include "raylib.h"
#include "raymath.h"

#include <assert.h>

enum DIRECTIONS {
    TOP_LEFT = 0,
    TOP = 1,
    TOP_RIGHT = 2,
    LEFT = 3,
    RIGHT = 4,
    BOTTOM_LEFT = 5,
    BOTTOM = 6,
    BOTTOM_RIGHT = 7
};


#define DODGE_DELAY .4f
#define DODGE_TIME .2f
typedef struct {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float speed;

    float dodge_speed;
    float is_dodging;
    float dodge_delay;

    enum DIRECTIONS last_direction;
    Texture2D sprite_sheet;
    Rectangle sprite_rectangles[8];
} dustin_t;

void dustin_init(dustin_t *dustin) {
    assert(dustin);

    dustin->sprite_sheet = LoadTexture(ASSETS_PATH"dustin.png"); // Check README.md for how this works
    dustin->position = CLITERAL(Vector2) {0.f, 0.f};
    dustin->velocity = CLITERAL(Vector2) {0.f, 0.f};
    dustin->acceleration = CLITERAL(Vector2) {0.f, 0.f};
    dustin->sprite_rectangles[TOP_LEFT] = CLITERAL(Rectangle) {0, 0, 32, 32};
    dustin->sprite_rectangles[TOP] = CLITERAL(Rectangle) {32, 0, 32, 32};
    dustin->sprite_rectangles[TOP_RIGHT] = CLITERAL(Rectangle) {64, 0, 32, 32};
    dustin->sprite_rectangles[LEFT] = CLITERAL(Rectangle) {0, 32, 32, 32};
    dustin->sprite_rectangles[RIGHT] = CLITERAL(Rectangle) {64, 32, 32, 32};
    dustin->sprite_rectangles[BOTTOM_LEFT] = CLITERAL(Rectangle) {0, 64, 32, 32};
    dustin->sprite_rectangles[BOTTOM] = CLITERAL(Rectangle) {32, 64, 32, 32};
    dustin->sprite_rectangles[BOTTOM_RIGHT] = CLITERAL(Rectangle) {64, 64, 32, 32};
    dustin->last_direction = BOTTOM;
    dustin->speed = 500.f;
    dustin->dodge_speed = 2.f;
    dustin->is_dodging = 0.f;
    dustin->dodge_delay = 0.f;
}

void dustin_update(dustin_t *dustin, float delta) {
    assert(dustin);

    // movement
    Vector2 velocity;
    if(dustin->is_dodging > 0.f) {
        dustin->is_dodging -= delta;
        velocity = Vector2Scale(dustin->velocity, dustin->dodge_speed);
    } else {
        // walk direction
        const int dirx = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
        const int diry = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
        if (dirx == 0 && diry == 0) {
            dustin->velocity.x = 0.f;
            dustin->velocity.y = 0.f;
        } else {
            dustin->velocity.x = (float) dirx;
            dustin->velocity.y = (float) diry;
            dustin->velocity = Vector2Scale(Vector2Normalize(dustin->velocity), dustin->speed);

            // calculate dir
            if (diry == 0) {
                if (dirx < 0) dustin->last_direction = LEFT;
                else dustin->last_direction = RIGHT;
            } else {
                const int off = (diry < 0) ? TOP_LEFT : BOTTOM_LEFT;
                if (dirx == 0) dustin->last_direction = off + 1;
                else if (dirx < 0) dustin->last_direction = off;
                else dustin->last_direction = off + 2;
            }
        }
        velocity = dustin->velocity;
        // dodge
        if(dustin->dodge_delay > 0.f) {
            dustin->dodge_delay -= delta;
        } else if(IsKeyDown(KEY_SPACE)) {
            dustin->is_dodging = DODGE_TIME;
            dustin->dodge_delay = DODGE_DELAY;
        }
    }

    // update
    dustin->position = Vector2Add(dustin->position, Vector2Scale(velocity, delta));
}

void dustin_render(dustin_t *dustin) {
    assert(dustin);

    Rectangle source = dustin->sprite_rectangles[dustin->last_direction];
    if(dustin->is_dodging > 0.f) source.x += 96; // offset to next action
    DrawTextureRec(dustin->sprite_sheet, source, dustin->position,WHITE);
}


#endif //DUSTINTIME_DUSTIN_H
