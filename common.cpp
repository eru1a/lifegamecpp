#include "common.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

void sdl_error(const std::string &msg) {
    std::cerr << msg << " SDL Error: " << SDL_GetError() << std::endl;
    exit(1);
}

void img_error(const std::string &msg) {
    std::cerr << msg << " SDL_image Error: " << IMG_GetError() << std::endl;
    exit(1);
}

void ttf_error(const std::string &msg) {
    std::cerr << msg << " SDL_ttf Error: " << TTF_GetError() << std::endl;
    exit(1);
}

std::tuple<int, int, Uint32> get_mousestate(const SDL_Rect &rect) {
    int px, py;
    int mousestate = SDL_GetMouseState(&px, &py);
    return {px - rect.x, py - rect.y, mousestate};
}

// (x, y)座標のピクセルを取得
Uint32 get_pixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        return *p;
    case 2:
        return *(Uint16 *)p;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;
    default:
        return 0;
    }
}

SDL_Texture *load_texture(SDL_Renderer *renderer, const std::string &path, bool colorkey) {
    SDL_Surface *loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == nullptr)
        img_error("Unable to load image " + path);

    if (colorkey) {
        SDL_Color rgb;
        Uint32 pixel = get_pixel(loaded_surface, 0, 0);
        SDL_GetRGB(pixel, loaded_surface->format, &rgb.r, &rgb.g, &rgb.b);
        SDL_SetColorKey(loaded_surface, SDL_TRUE,
                        SDL_MapRGB(loaded_surface->format, rgb.r, rgb.g, rgb.b));
    }

    SDL_Texture *img = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    if (img == nullptr)
        sdl_error("Unable to create texture from " + path);

    SDL_FreeSurface(loaded_surface);
    return img;
}

// 15字以上の文字列に対して、13~15文字目を...にしてそれ以降を切り捨てた文字列を返す
std::string compress(const std::string &str) {
    if (str.length() < 15)
        return str;
    return str.substr(0, 15) + "...";
}

Pattern load_pattern(const std::string &file) {
    std::ifstream ifs(file);
    if (!ifs) {
        std::cerr << "ファイルオープン失敗: " << file << std::endl;
        exit(1);
    }

    std::cin.rdbuf(ifs.rdbuf());

    std::string name;
    std::getline(ifs, name);
    int row, col;
    std::cin >> row >> col;

    std::vector<std::vector<bool>> pattern(row, std::vector<bool>(col, false));
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            char c;
            std::cin >> c;
            if (c == '1')
                pattern.at(i).at(j) = true;
        }
    }

    return Pattern{compress(name), row, col, pattern};
}

std::vector<Pattern> load_patterns(const std::string &dir) {
    std::vector<Pattern> result;
    for (const auto &entry : std::filesystem::directory_iterator(dir))
        result.push_back(load_pattern(entry.path()));
    // col * row が小さい順にソートする
    std::sort(result.begin(), result.end(),
              [](const auto &p1, const auto &p2) { return p1.col * p1.row < p2.col * p2.row; });
    return result;
}
