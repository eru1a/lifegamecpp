#include "common.h"
#include "mainwindow.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        sdl_error("SDLの初期化に失敗");
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        img_error("SDL_imageの初期化に失敗");
    if (TTF_Init() < 0)
        ttf_error("SDL_ttfの初期化に失敗");

    auto window = SDL_CreateWindow("LifeGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        sdl_error("ウィンドウの初期化に失敗");

    auto renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
        sdl_error("レンダラの初期化に失敗");

    TTF_Font *font = TTF_OpenFont("data/roboto-android/Roboto-Thin.ttf", 22);
    if (font == nullptr)
        ttf_error("TTF_OpenFontに失敗");

    SDL_Event e;
    bool quit = false;

    MainWindow mainwindow(renderer, font, 200, 200);

    while (!quit) {
        // 黒で塗りつぶす
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                case SDLK_q:
                    quit = true;
                    break;
                }
            }
            // SDL_Eventを使った更新
            mainwindow.update(e);
        }

        mainwindow.update();
        mainwindow.draw();

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    font = nullptr;
    renderer = nullptr;
    window = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
