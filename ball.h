#ifndef BALL_H
#define BALL_H

#include "raylib.h"

struct Ball {
    Vector2 pos;
    Vector2 vel;
    bool active;
};

constexpr float ball_launch_angle_degrees = 45.0f;
constexpr float ball_launch_vel_mag = 0.14f;
constexpr int max_balls = 10;

inline Vector2 ball_size = { 1.0f, 1.0f };
inline Ball balls[max_balls];
inline int active_balls_count = 0;

void spawn_ball();
void move_balls();
bool is_any_ball_inside_level();
void add_ball(Vector2 position);

#endif