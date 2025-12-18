#include "assets.h"

#include "raylib.h"

void load_fonts()
{
    menu_font = LoadFontEx("data/fonts/ARCADECLASSIC.TTF", 256, nullptr, 0);
}

void unload_fonts()
{
    UnloadFont(menu_font);
}

void load_textures()
{
    background_menu_texture = LoadTexture("data/images/backgrounds/menu_background.png");
    level_background_texture = LoadTexture("data/images/backgrounds/level_background.png");
    level2_background_texture = LoadTexture("data/images/backgrounds/level2_background.png");
    wall_texture = LoadTexture("data/images/wall.png");
    void_texture = LoadTexture("data/images/void.png");
    block_texture = LoadTexture("data/images/block.png");
    paddle_texture = LoadTexture("data/images/paddle.png");
    ball_texture = LoadTexture("data/images/ball.png");
    // ball_sprite = load_sprite("data/images/ball.png", ".png", 1, true, 0);
}

void unload_textures()
{
    UnloadTexture(level_background_texture);
    UnloadTexture(level2_background_texture);
    UnloadTexture(background_menu_texture);
    UnloadTexture(wall_texture);
    UnloadTexture(void_texture);
    UnloadTexture(block_texture);
    UnloadTexture(paddle_texture);
    UnloadTexture(ball_texture);
    // unload_sprite(ball_sprite);
}

void load_sounds()
{
    InitAudioDevice();
    theme_song = LoadSound("data/sounds/song.mp3");
    win_sound = LoadSound("data/sounds/win.wav");
    lose_sound = LoadSound("data/sounds/lose.wav");
}

void unload_sounds()
{
    UnloadSound(win_sound);
    UnloadSound(lose_sound);
    CloseAudioDevice();
}
