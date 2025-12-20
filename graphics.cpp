#include "graphics.h"

#include "assets.h"
#include "ball.h"
#include "level.h"
#include "paddle.h"

#include "raylib.h"

#include <cmath>
#include <iostream>
#include <string>

struct Text {
    std::string str;
    Vector2 position = { 0.50f, 0.50f };
    float size = 32.0f;
    Color color = WHITE;
    float spacing = 4.0f;
    Font* font = nullptr;
};

constexpr float cell_scale = 0.6f;
constexpr float screen_scale_divisor = 700.0f;

constexpr size_t victory_ball_launch_degree_offset = 3;
constexpr size_t victory_balls_count = 360 / victory_ball_launch_degree_offset;
constexpr float victory_balls_speed = 7.0f;
constexpr float victory_balls_size = 3.0f;

Vector2 screen_size;
float screen_scale;
float cell_size;
Vector2 shift_to_center;

Vector2 victory_balls_pos[victory_balls_count];
Vector2 victory_balls_vel[victory_balls_count];

void draw_image(const Texture2D& image, const float x, const float y, const float width, const float height)
{
    const Rectangle source = { 0.0f, 0.0f, static_cast<float>(image.width), static_cast<float>(image.height) };
    const Rectangle destination = { x, y, width, height };
    DrawTexturePro(image, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}

void draw_image(const Texture2D& image, const float x, const float y, const float size)
{
    draw_image(image, x, y, size, size);
}

void draw_sprite(sprite& sprite, const float x, const float y, const float width, const float height)
{
    draw_image(sprite.frames[sprite.frame_index], x, y, width, height);

    if (sprite.prev_game_frame == game_frame) {
        return;
    }
    if (sprite.frames_skipped < sprite.frames_to_skip) {
        ++sprite.frames_skipped;
    } else {
        sprite.frames_skipped = 0;

        ++sprite.frame_index;
        if (sprite.frame_index >= sprite.frame_count) {
            sprite.frame_index = sprite.loop ? 0 : sprite.frame_count - 1;
        }
    }
    sprite.prev_game_frame = game_frame;
}

void draw_sprite(sprite& sprite, const float x, const float y, const float size)
{
    draw_sprite(sprite, x, y, size, size);
}

void draw_text(const Text& text)
{
    const auto [x, y] = MeasureTextEx(*text.font, text.str.c_str(), text.size * screen_scale, text.spacing);
    const Vector2 pos = {
        screen_size.x * text.position.x - 0.5f * x,
        screen_size.y * text.position.y - 0.5f * y
    };
    DrawTextEx(*text.font, text.str.c_str(), pos, y, text.spacing, text.color);
}

void derive_graphics_metrics()
{
    screen_size.x = static_cast<float>(GetScreenWidth());
    screen_size.y = static_cast<float>(GetScreenHeight());

    cell_size = std::min(screen_size.x / static_cast<float>(current_level.columns), screen_size.y / static_cast<float>(current_level.rows));
    screen_scale = std::min(screen_size.x, screen_size.y) / screen_scale_divisor;

    const float level_width = static_cast<float>(current_level.columns) * cell_size;
    const float level_height = static_cast<float>(current_level.rows) * cell_size;
    shift_to_center = {
        (screen_size.x - level_width) * 0.5f,
        (screen_size.y - level_height)
    };
}

void draw_menu()
{
    draw_image(background_menu_texture, 0, 0, screen_size.x, screen_size.y);

    const Text game_title = {
        "BREAKOUT",
        { 0.50f, 0.20f },
        200.0f,
        YELLOW,
        4.0f,
        &menu_font
    };
    draw_text(game_title);

    const Text subtitle = {
        "In Space",
        { 0.50f, 0.35f },
        50.0f,
        YELLOW,
        4.0f,
        &menu_font
    };
    draw_text(subtitle);

    float offset = std::sin(game_frame * 0.04f) * 0.03f;

    const Text game_subtitle = {
        "Press Enter to Start",
        { 0.50f, 0.65f + offset },
        32.0f,
        WHITE,
        4.0f,
        &menu_font
    };

    draw_text(game_subtitle);
}

void draw_ui()
{
    const Text level_counter1 = {
        "LEVEL " + std::to_string(current_level_index + 1),
        { 0.90f, 0.0375f },
        48.0f,
        WHITE,
        4.0f,
        &menu_font
    };
    const Text level_counter2 = {
        "OUT OF " + std::to_string(level_count),
        { 0.90f, 0.09f },
        48.0f,
        WHITE,
        4.0f,
        &menu_font
    };
    draw_text(level_counter1);
    draw_text(level_counter2);

    const Text boxes_remaining = {
        "BLOCKS " + std::to_string(current_level_blocks),
        { 0.5f, 0.9625f },
        48.0f,
        WHITE,
        4.0f,
        &menu_font
    };
    draw_text(boxes_remaining);

    const float heart_size = 50.0f;
    const float heart_spacing = 10.0f;
    const float hearts_start_x = 20.0f;
    const float hearts_y = 20.0f;

    for (int i = 0; i < player_lives; i++) {
        float x = hearts_start_x + i * (heart_size + heart_spacing);
        draw_image(heart_texture, x, hearts_y, heart_size);
    }
}

void draw_level()
{
    switch (current_level_index) {
    case 0:
        draw_image(level_background_texture, 0, 0, screen_size.x, screen_size.y);
        break;
    case 1:
        draw_image(level2_background_texture, 0, 0, screen_size.x, screen_size.y);
        break;
    case 2:
        draw_image(level3_background_texture, 0, 0, screen_size.x, screen_size.y);
        break;
    case 3:
        draw_image(level4_background_texture, 0, 0, screen_size.x, screen_size.y);
        break;
    case 4:
        draw_image(level5_background_texture, 0, 0, screen_size.x, screen_size.y);
        break;
    case 5:
        draw_image(level6_background_texture, 0, 0, screen_size.x, screen_size.y);
        break;

    default:
        ClearBackground(BLACK);
    }

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            const char data = current_level.data[row * current_level.columns + column];
            const float texture_x_pos = shift_to_center.x + static_cast<float>(column) * cell_size;
            const float texture_y_pos = shift_to_center.y + static_cast<float>(row) * cell_size;

            switch (data) {
            case WALL:
                draw_image(wall_texture, texture_x_pos, texture_y_pos, cell_size);
                break;
            case BLOCKS:
                draw_image(block_texture, texture_x_pos, texture_y_pos, cell_size);
                break;
            case HEART_BLOCK:
                draw_image(heart_block_texture, texture_x_pos, texture_y_pos, cell_size);
                break;
            case MULTIBALL_BLOCK:
                draw_image(multi_ball_texture, texture_x_pos, texture_y_pos, cell_size);
                break;
            default:;
            }
        }
    }
}

void draw_paddle()
{
    const float texture_x_pos = shift_to_center.x + paddle_pos.x * cell_size;
    const float texture_y_pos = shift_to_center.y + paddle_pos.y * cell_size;
    draw_image(paddle_texture, texture_x_pos, texture_y_pos, paddle_size.x * cell_size, paddle_size.y * cell_size);
}

void draw_balls()
{
    for (int i = 0; i < max_balls; i++) {
        if (!balls[i].active)
            continue;

        const float texture_x_pos = shift_to_center.x + balls[i].pos.x * cell_size;
        const float texture_y_pos = shift_to_center.y + balls[i].pos.y * cell_size;
        draw_image(ball_texture, texture_x_pos, texture_y_pos, cell_size);
    }
}

void draw_pause_menu()
{
    ClearBackground(BLACK);

    const Text paused_title = {
        "Press Enter to Resume",
        { 0.50f, 0.50f },
        32.0f,
        WHITE,
        4.0f,
        &menu_font
    };
    draw_text(paused_title);
}

void init_victory_menu()
{
    for (size_t i = 0; i < victory_balls_count; ++i) {
        victory_balls_pos[i] = { screen_size.x / 2, screen_size.y / 2 };
        victory_balls_vel[i] = {
            std::cos(static_cast<float>(i * victory_ball_launch_degree_offset)) * victory_balls_speed,
            std::sin(static_cast<float>(i * victory_ball_launch_degree_offset)) * victory_balls_speed
        };
    }
}

void animate_victory_menu()
{
    for (size_t i = 0; i < victory_balls_count; ++i) {
        if (victory_balls_pos[i].x + victory_balls_vel[i].x > screen_size.x || victory_balls_pos[i].x + victory_balls_vel[i].x < 0) {
            victory_balls_vel[i].x *= -1.0f;
        }
        if (victory_balls_pos[i].y + victory_balls_vel[i].y > screen_size.y || victory_balls_pos[i].y + victory_balls_vel[i].y < 0) {
            victory_balls_vel[i].y *= -1.0f;
        }
        victory_balls_pos[i] = {
            victory_balls_pos[i].x + victory_balls_vel[i].x,
            victory_balls_pos[i].y + victory_balls_vel[i].y
        };
    }
}

void draw_victory_menu()
{
    animate_victory_menu();

    DrawRectangleV({ 0.0f, 0.0f }, { screen_size.x, screen_size.y }, { 0, 0, 0, 50 });

    for (const auto& [x, y] : victory_balls_pos) {
        DrawCircleV({ x, y }, victory_balls_size, WHITE);
    }

    const Text victory_title = {
        "Victory!",
        { 0.50f, 0.50f },
        100.0f,
        RED,
        4.0f,
        &menu_font
    };
    draw_text(victory_title);

    const Text victory_subtitle = {
        "Press Enter to Restart",
        { 0.50f, 0.65f },
        32.0f,
        WHITE,
        4.0f,
        &menu_font
    };
    draw_text(victory_subtitle);
}

void draw_game_over()
{
    draw_image(game_over_background_texture, 0, 0, screen_size.x, screen_size.y);

    const Text game_title = {
        "GAME OVER",
        { 0.50f, 0.30f },
        200.0f,
        RED,
        4.0f,
        &menu_font
    };
    draw_text(game_title);

    const Text option1 = {
        "Return To Main Menu",
        { 0.50f, 0.65f },
        50.0f,
        WHITE,
        4.0f,
        &menu_font
    };
    draw_text(option1);

    const Text option2 = {
        "Retry",
        { 0.50f, 0.50f },
        50.0f,
        WHITE,
        4.0f,
        &menu_font
    };
    draw_text(option2);
}