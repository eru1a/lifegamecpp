#pragma once

#include "common.h"

class LifeGame;

class ToolBar {
public:
    static const int button_width = 32;
    static const int button_height = 32;
    static const int margin = 2;
    static const int pattern_text_width = 200;

    ToolBar(SDL_Renderer *renderer, TTF_Font *font, const SDL_Rect &viewport, LifeGame *lifegame);

    ~ToolBar() {
        SDL_DestroyTexture(m_play_button);
        SDL_DestroyTexture(m_stop_button);
        SDL_DestroyTexture(m_step_button);
        SDL_DestroyTexture(m_clear_button);
        m_play_button = nullptr;
        m_stop_button = nullptr;
        m_step_button = nullptr;
        m_clear_button = nullptr;
        pattern_texture_free();
    }

    void update(SDL_Event e);
    void draw() const;

private:
    SDL_Renderer *m_renderer;
    TTF_Font *m_font;
    const SDL_Rect m_viewport;
    LifeGame *m_lifegame;
    SDL_Texture *m_play_button;
    SDL_Texture *m_stop_button;
    SDL_Texture *m_step_button;
    SDL_Texture *m_clear_button;
    SDL_Texture *m_left_button;
    SDL_Texture *m_right_button;
    SDL_Rect m_play_stop_rect;
    SDL_Rect m_step_rect;
    SDL_Rect m_clear_rect;
    SDL_Rect m_left_rect;
    SDL_Rect m_right_rect;
    SDL_Texture *m_pattern_texture;
    SDL_Rect m_pattern_rect;
    SDL_Rect m_pattern_texture_rect;
    int m_pattern_texture_width;
    int m_pattern_texture_height;

    void pattern_texture_free();
    void make_pattern_texture(const std::string &text);
};
