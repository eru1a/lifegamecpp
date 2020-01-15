#pragma once

#include "camera.h"
#include "common.h"
#include <SDL2/SDL.h>
#include <vector>

class LifeGame {
public:
    LifeGame(int col, int row)
        : m_col(col), m_row(row),
          m_field(std::vector<std::vector<bool>>(row, std::vector<bool>(col, false))) {}

    void step();
    void clear();
    void update();
    void draw(SDL_Renderer *renderer) const;

    void run() { m_running = true; }
    void stop() { m_running = false; }
    void toggle() { m_running = !m_running; }

private:
    int m_col;
    int m_row;
    std::vector<std::vector<bool>> m_field;
    int m_generation = 0;
    bool m_running = false;
    Camera m_camera;

    /// 周囲8マスの生きてるセルの数を返す
    int around(int x, int y) const;
};
