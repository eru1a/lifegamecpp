#include "common.h"
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

Pattern load_pattern(const std::string &file) {
    std::ifstream ifs(file);
    if (!ifs) {
        std::cerr << "ファイルオープン失敗: " << file << std::endl;
        exit(1);
    }

    std::cin.rdbuf(ifs.rdbuf());

    std::string name;
    int col, row;
    std::cin >> name >> col >> row;

    std::vector<std::vector<bool>> pattern(row, std::vector<bool>(col, false));
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            int live;
            std::cin >> live;
            pattern.at(i).at(j) = live;
        }
    }

    return Pattern{"", 0, 0, pattern};
}
