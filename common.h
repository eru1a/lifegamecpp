#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <tuple>

// ウィンドウの大きさ
const int WIDTH = 960;
const int HEIGHT = 640;

/// グリッドサイズ
const float GS = 20;

/// SDL_GetError()を表示してプログラム終了
void sdl_error(const std::string &msg);
/// IMG_GetError()を表示してプログラム終了
void img_error(const std::string &msg);

std::tuple<int, int, Uint32> get_mousestate(const SDL_Rect &viewport);

struct Texture {
    SDL_Texture *texture;
    int w, h;
};

/// 画像を読み込む。
/// colorkeyがtrueの時は(0,0)の座標をカラーキーとしてセットする。
SDL_Texture *load_texture(SDL_Renderer *renderer, const std::string &path, bool colorkey = false);
