#pragma once

#include <string>

// ウィンドウの大きさ
const int WIDTH = 960;
const int HEIGHT = 640;

/// グリッドサイズ
const float GS = 20;

/// SDL Errorを表示してプログラム終了
void sdl_error(const std::string &msg);
