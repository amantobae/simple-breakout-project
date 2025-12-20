#include "ball.h"
#include "assets.h"
#include "game.h"
#include "level.h"
#include "paddle.h"

#include "raylib.h"

#include <cmath>
#include <numbers>

inline float teleport_cooldown[max_balls] = {0};

void spawn_ball()
{
    for (int i = 0; i < max_balls; i++) {
        balls[i].active = false;
        teleport_cooldown[i] = 0.0f;
    }
    active_balls_count = 0;

    for (int column = 0; column < current_level.columns; column++) {
        for (int row = 0; row < current_level.rows; row++) {
            if (get_level_cell(row, column) == BALL) {
                set_level_cell(row, column, VOID);

                balls[0].pos = { static_cast<float>(column), static_cast<float>(row) };
                balls[0].active = true;

                constexpr float ball_launch_angle_radians = ball_launch_angle_degrees * (std::numbers::pi_v<float> / 180.0f);
                balls[0].vel.y = -ball_launch_vel_mag * std::sin(ball_launch_angle_radians);
                balls[0].vel.x = (rand() % 2 == 0) ? ball_launch_vel_mag * std::cos(ball_launch_angle_radians) : -ball_launch_vel_mag * std::cos(ball_launch_angle_radians);

                active_balls_count = 1;
                goto outer_loop_end;
            }
        }
    }
outer_loop_end:;
}

void add_ball(Vector2 position)
{
    static bool alternate_direction = false;

    for (int i = 0; i < max_balls; i++) {
        if (!balls[i].active) {
            balls[i].pos = position;
            balls[i].active = true;
            teleport_cooldown[i] = 0.0f;

            constexpr float angle_45_radians = 45.0f * (std::numbers::pi_v<float> / 180.0f);
            balls[i].vel.y = -ball_launch_vel_mag * std::sin(angle_45_radians);

            balls[i].vel.x = alternate_direction ? ball_launch_vel_mag * std::cos(angle_45_radians) : -ball_launch_vel_mag * std::cos(angle_45_radians);

            alternate_direction = !alternate_direction;

            active_balls_count++;
            return;
        }
    }
}

void move_balls()
{
    for (int i = 0; i < max_balls; i++) {
        if (!balls[i].active)
            continue;

        if (teleport_cooldown[i] > 0) {
            teleport_cooldown[i] -= GetFrameTime();
        }

        Vector2 next_ball_pos = {
            balls[i].pos.x + balls[i].vel.x,
            balls[i].pos.y + balls[i].vel.y
        };

        if (portal1_active && portal2_active && teleport_cooldown[i] <= 0) {
            float portal_radius = 0.4f;

            if (CheckCollisionCircles(
                {balls[i].pos.x + 0.5f, balls[i].pos.y + 0.5f},
                ball_size.x / 2.0f,
                {portal1_position.x + 0.5f, portal1_position.y + 0.5f},
                portal_radius
            )) {
                balls[i].pos = portal2_position;

                balls[i].pos.x += 0.1f;
                balls[i].pos.y += 0.1f;

                teleport_cooldown[i] = 0.3f;

                continue;
            }

            if (CheckCollisionCircles(
                {balls[i].pos.x + 0.5f, balls[i].pos.y + 0.5f},
                ball_size.x / 2.0f,
                {portal2_position.x + 0.5f, portal2_position.y + 0.5f},
                portal_radius
            )) {
                balls[i].pos = portal1_position;

                balls[i].pos.x += 0.1f;
                balls[i].pos.y += 0.1f;

                teleport_cooldown[i] = 0.3f;

                continue;
            }
        }

        if (is_colliding_with_level_cell(next_ball_pos, ball_size, WALL) || is_colliding_with_level_cell(next_ball_pos, ball_size, BOUNDARY)) {

            if (is_colliding_with_level_cell({ next_ball_pos.x, balls[i].pos.y }, ball_size, WALL) || is_colliding_with_level_cell({ next_ball_pos.x, balls[i].pos.y }, ball_size, BOUNDARY)) {
                balls[i].vel.x = -balls[i].vel.x;
                next_ball_pos.x = std::round(next_ball_pos.x);
            }
            if (is_colliding_with_level_cell({ balls[i].pos.x, next_ball_pos.y }, ball_size, WALL) || is_colliding_with_level_cell({ balls[i].pos.x, next_ball_pos.y }, ball_size, BOUNDARY)) {
                balls[i].vel.y = -balls[i].vel.y;
                next_ball_pos.y = std::round(next_ball_pos.y);
            }
        } else if (is_colliding_with_level_cell(next_ball_pos, ball_size, HEART_BLOCK)) {
            char& temp = get_colliding_level_cell(next_ball_pos, ball_size, HEART_BLOCK);

            if (is_colliding_with_level_cell({ next_ball_pos.x, balls[i].pos.y }, ball_size, HEART_BLOCK)) {
                balls[i].vel.x = -balls[i].vel.x;
                next_ball_pos.x = std::round(next_ball_pos.x);
            }
            if (is_colliding_with_level_cell({ balls[i].pos.x, next_ball_pos.y }, ball_size, HEART_BLOCK)) {
                balls[i].vel.y = -balls[i].vel.y;
                next_ball_pos.y = std::round(next_ball_pos.y);
            }

            temp = VOID;
            ++player_lives;
            if (player_lives > 3) {
                player_lives = 3;
            }
        } else if (is_colliding_with_level_cell(next_ball_pos, ball_size, MULTIBALL_BLOCK)) {
            char& temp = get_colliding_level_cell(next_ball_pos, ball_size, MULTIBALL_BLOCK);

            if (is_colliding_with_level_cell({ next_ball_pos.x, balls[i].pos.y }, ball_size, MULTIBALL_BLOCK)) {
                balls[i].vel.x = -balls[i].vel.x;
                next_ball_pos.x = std::round(next_ball_pos.x);
            }
            if (is_colliding_with_level_cell({ balls[i].pos.x, next_ball_pos.y }, ball_size, MULTIBALL_BLOCK)) {
                balls[i].vel.y = -balls[i].vel.y;
                next_ball_pos.y = std::round(next_ball_pos.y);
            }

            temp = VOID;

            add_ball(balls[i].pos);
            add_ball(balls[i].pos);
        } else if (is_colliding_with_level_cell(next_ball_pos, ball_size, BLOCKS)) {
            char& temp = get_colliding_level_cell(next_ball_pos, ball_size, BLOCKS);

            if (is_colliding_with_level_cell({ next_ball_pos.x, balls[i].pos.y }, ball_size, BLOCKS)) {
                balls[i].vel.x = -balls[i].vel.x;
                next_ball_pos.x = std::round(next_ball_pos.x);
            }
            if (is_colliding_with_level_cell({ balls[i].pos.x, next_ball_pos.y }, ball_size, BLOCKS)) {
                balls[i].vel.y = -balls[i].vel.y;
                next_ball_pos.y = std::round(next_ball_pos.y);
            }

            temp = VOID;
            --current_level_blocks;
        } else if (is_colliding_with_paddle(next_ball_pos, ball_size)) {
            balls[i].vel.y = -std::abs(balls[i].vel.y);
        }

        balls[i].pos = next_ball_pos;

        if (!is_inside_level(static_cast<int>(balls[i].pos.y), static_cast<int>(balls[i].pos.x))) {
            balls[i].active = false;
            active_balls_count--;
        }
    }
}

bool is_any_ball_inside_level()
{
    return active_balls_count > 0;
}