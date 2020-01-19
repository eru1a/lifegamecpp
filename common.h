#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <tuple>
#include <vector>

// ウィンドウの大きさ
const int WIDTH = 960;
const int HEIGHT = 640;

/// グリッドサイズ
const float GS = 20;

/// SDL_GetError()を表示してプログラム終了
void sdl_error(const std::string &msg);
/// IMG_GetError()を表示してプログラム終了
void img_error(const std::string &msg);
/// TTF_GetError()を表示してプログラム終了
void ttf_error(const std::string &msg);

std::tuple<int, int, Uint32> get_mousestate(const SDL_Rect &viewport);

/// 画像を読み込む。
/// colorkeyがtrueの時は(0,0)の座標をカラーキーとしてセットする。
SDL_Texture *load_texture(SDL_Renderer *renderer, const std::string &path, bool colorkey = false);

struct Pattern {
    std::string name;
    int row;
    int col;
    std::vector<std::vector<bool>> pattern;
};

Pattern load_pattern(const std::string &file);
std::vector<Pattern> load_patterns(const std::string &dir);
