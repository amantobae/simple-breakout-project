#include "assets.h"
#include "ball.h"
#include "game.h"
#include "graphics.h"
#include "level.h"
#include "paddle.h"

#include "raylib.h"

#include <iostream>
void update()
{
    if (game_state == in_game_state) {
        UpdateMusicStream(theme_song);
    }

    if (game_state == menu_state) {
        if (IsKeyPressed(KEY_ENTER)) {
            player_lives = max_lives;
            game_state = in_game_state;
            PlayMusicStream(theme_song);
        }
        return;
    }
    if (game_state == paused_state) {
        if (IsKeyPressed(KEY_ENTER)) {
            game_state = in_game_state;
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            game_state = menu_state;
            StopMusicStream(theme_song);
        }
        return;
    }
    if (game_state == game_over_state) {
        if (IsKeyPressed(KEY_ENTER)) {
            player_lives = max_lives;
            current_level_index = 0;
            load_level(0);
            game_state = in_game_state;
            PlayMusicStream(theme_song);
        }
        return;
    }
    if (game_state == victory_state) {
        if (IsKeyPressed(KEY_ENTER)) {
            player_lives = max_lives;
            load_level(0);
            game_state = in_game_state;
            PlayMusicStream(theme_song);
        }
        return;
    }
    if (IsKeyPressed(KEY_E)) {
        game_state = paused_state;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        move_paddle(-paddle_speed);
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        move_paddle(paddle_speed);
    }
    move_balls();
    if (!is_any_ball_inside_level()) {
        --player_lives;
        if (player_lives <= 0) {
            player_lives = max_lives;
            load_level(0);
            game_state = game_over_state;
            StopMusicStream(theme_song);
            PlaySound(game_over_sound);
        } else {
            load_level();
            PlaySound(lose_sound);
        }

    } else if (current_level_blocks == 0) {
        if (current_level_index >= level_count - 1) {
            game_state = victory_state;
            StopMusicStream(theme_song);
            PlaySound(victory_sound);
            init_victory_menu();
        } else {
            load_level(1);
            PlaySound(win_sound);
        }
    }
}

void draw()
{
    game_frame++;
    if (game_state == menu_state) {
        draw_menu();
    } else if (game_state == paused_state) {
        draw_pause_menu();
    } else if (game_state == victory_state) {
        draw_victory_menu();
    } else if (game_state == game_over_state) {
        draw_game_over();
    } else if (game_state == in_game_state) {
        draw_level();
        draw_paddle();
        draw_balls();
        draw_ui();
    } else {
        std::cout << "Unknown state";
        return;
    }
}

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1280, 720, "Breakout");
    SetTargetFPS(60);

    load_fonts();
    load_textures();
    load_level();
    load_sounds();

    while (!WindowShouldClose()) {
        BeginDrawing();

        update();
        draw();

        EndDrawing();
    }
    CloseWindow();

    unload_sounds();
    unload_level();
    unload_textures();
    unload_fonts();

    return 0;
}
