#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <cstdlib>

inline size_t game_frame = 0;
void derive_graphics_metrics();

void draw_menu();
void draw_ui();
void draw_level();
void draw_paddle();
void draw_ball();
void draw_pause_menu();
void init_victory_menu();
void draw_victory_menu();
void draw_game_over();

#endif // GRAPHICS_H
