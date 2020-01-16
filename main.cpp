#include "common.h"
#include "lifegame.h"
#include <SDL2/SDL.h>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        sdl_error("SDLの初期化に失敗");

    auto window = SDL_CreateWindow("LifeGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        sdl_error("ウィンドウの初期化に失敗");

    auto renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
        sdl_error("レンダラの初期化に失敗");

    SDL_Event e;
    bool quit = false;

    LifeGame lifegame(100, 100);

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
            lifegame.update(e);
        }

        lifegame.update();
        lifegame.draw(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;

    SDL_Quit();

    return 0;
}
