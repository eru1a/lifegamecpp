#pragma once

#include "common.h"

class LifeGame;

class ToolBar {
public:
    static const int button_width = 32;
    static const int button_height = 32;
    static const int margin = 2;

    ToolBar(SDL_Renderer *renderer, const SDL_Rect &viewport, LifeGame *lifegame)
        : m_viewport(viewport), m_lifegame(lifegame) {
        m_play_button = load_texture(renderer, "data/play.png");
        m_stop_button = load_texture(renderer, "data/stop.png");
        m_step_button = load_texture(renderer, "data/step.png");
        m_clear_button = load_texture(renderer, "data/clear.png");

        m_play_stop_rect =
            SDL_Rect{margin * 1 + button_width * 0, margin, button_width, button_height};
        m_step_rect = SDL_Rect{margin * 3 + button_width * 1, margin, button_width, button_height};
        m_clear_rect =
            SDL_Rect{margin * 5 + button_width * 2, margin, button_width, button_height};
    }

    ~ToolBar() {
        SDL_DestroyTexture(m_play_button);
        SDL_DestroyTexture(m_stop_button);
        SDL_DestroyTexture(m_step_button);
        SDL_DestroyTexture(m_clear_button);
    }

    void update(SDL_Event e);
    void draw(SDL_Renderer *renderer) const;

private:
    const SDL_Rect m_viewport;
    LifeGame *m_lifegame;
    SDL_Texture *m_play_button;
    SDL_Texture *m_stop_button;
    SDL_Texture *m_step_button;
    SDL_Texture *m_clear_button;
    SDL_Rect m_play_stop_rect;
    SDL_Rect m_step_rect;
    SDL_Rect m_clear_rect;
};
