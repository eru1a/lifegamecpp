#pragma once

#include "lifegame.h"
#include "toolbar.h"

class MainWindow {
public:
    MainWindow(SDL_Renderer *renderer, TTF_Font *font, int col, int row)
        : m_lifegame(renderer, col, row, WIDTH, HEIGHT,
                     SDL_Rect{0, ToolBar::button_height + ToolBar::margin * 2, WIDTH,
                              HEIGHT - (ToolBar::button_height + ToolBar::margin * 2)}),
          m_toolbar(renderer, font,
                    SDL_Rect{0, 0, WIDTH, ToolBar::button_height + ToolBar::margin * 2},
                    &m_lifegame) {}

    void update();
    void update(SDL_Event e);
    void draw() const;

private:
    LifeGame m_lifegame;
    ToolBar m_toolbar;
};
