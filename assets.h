#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"

#include "sprite.h"

inline Font menu_font;

inline Texture2D wall_texture;
inline Texture2D background_menu_texture;
inline Texture2D game_over_background_texture;
inline Texture2D level_background_texture;
inline Texture2D level2_background_texture;
inline Texture2D level3_background_texture;
inline Texture2D level4_background_texture;
inline Texture2D level5_background_texture;
inline Texture2D level6_background_texture;
inline Texture2D void_texture;
inline Texture2D paddle_texture;
inline Texture2D ball_texture;
inline Texture2D block_texture;
inline Texture2D heart_texture;
inline Texture2D heart_block_texture;
inline Texture2D multi_ball_texture;
inline Texture2D portal1_texture;
inline Texture2D portal2_texture;


inline sprite ball_sprite;

inline Music theme_song;
inline Sound win_sound;
inline Sound lose_sound;

void load_fonts();
void unload_fonts();

void load_textures();
void unload_textures();

void load_sounds();
void unload_sounds();

#endif // ASSETS_H
