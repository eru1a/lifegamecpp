#pragma once

#include "lifegame.h"
#include "toolbar.h"

class MainWindow {
public:
    MainWindow(SDL_Renderer *renderer)
        : m_lifegame(100, 100, WIDTH, HEIGHT,
                     SDL_Rect{0, ToolBar::button_height + ToolBar::margin * 2, WIDTH,
                              HEIGHT - (ToolBar::button_height + ToolBar::margin * 2)}),
          m_toolbar(renderer, SDL_Rect{0, 0, WIDTH, ToolBar::button_height + ToolBar::margin * 2},
                    &m_lifegame) {}

    void update();
    void update(SDL_Event e);
    void draw(SDL_Renderer *renderer) const;

private:
    LifeGame m_lifegame;
    ToolBar m_toolbar;
};
