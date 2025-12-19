#include "assets.h"

#include "raylib.h"

void load_fonts()
{
    menu_font = LoadFontEx("data/fonts/LCDSolid1.13-Regular.TTF", 256, nullptr, 0);
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
    level3_background_texture = LoadTexture("data/images/backgrounds/level3_background.png");
    level4_background_texture = LoadTexture("data/images/backgrounds/level4_background.png");
    level5_background_texture = LoadTexture("data/images/backgrounds/level5_background.png");
    wall_texture = LoadTexture("data/images/wall.png");
    void_texture = LoadTexture("data/images/void.png");
    block_texture = LoadTexture("data/images/block.png");
    paddle_texture = LoadTexture("data/images/paddle.png");
    ball_texture = LoadTexture("data/images/ball.png");
    heart_texture = LoadTexture("data/images/heart.png");
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
    UnloadTexture(heart_texture);
}

void load_sounds()
{
    InitAudioDevice();
    theme_song = LoadMusicStream("data/sounds/song.mp3");
    theme_song.looping = true;
    SetMusicVolume(theme_song, 0.5f);
    PlayMusicStream(theme_song);

    win_sound = LoadSound("data/sounds/win.wav");
    lose_sound = LoadSound("data/sounds/lose.wav");
}

void unload_sounds()
{
    UnloadMusicStream(theme_song);
    UnloadSound(win_sound);
    UnloadSound(lose_sound);
    CloseAudioDevice();
}
