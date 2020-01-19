#pragma once

#include "camera.h"
#include "common.h"
#include <SDL2/SDL.h>
#include <vector>

class LifeGame {
public:
    LifeGame(SDL_Renderer *renderer, int col, int row, int width, int height, SDL_Rect viewport)
        : m_renderer(renderer), m_col(col), m_row(row), m_width(width), m_height(height),
          m_viewport(viewport),
          m_field(std::vector<std::vector<bool>>(row, std::vector<bool>(col, false))),
          m_patterns(load_patterns("./patterns")) {}

    void step();
    void clear();
    void update();
    void update(const SDL_Event &e);
    void draw() const;

    bool is_running() { return m_running; }
    void run() { m_running = true; }
    void stop() { m_running = false; }
    void toggle() { m_running = !m_running; }

    void next_pattern() {
        m_current_pattern_index++;
        if (m_current_pattern_index == int(m_patterns.size()))
            m_current_pattern_index = 0;
    }
    void prev_pattern() {
        m_current_pattern_index--;
        if (m_current_pattern_index == int(-1))
            m_current_pattern_index = m_patterns.size() - 1;
    }
    std::string current_pattern_name() { return m_patterns.at(m_current_pattern_index).name; }

private:
    SDL_Renderer *m_renderer;
    const int m_col;
    const int m_row;
    const int m_width;
    const int m_height;
    const SDL_Rect m_viewport;
    std::vector<std::vector<bool>> m_field;
    int m_generation = 0;
    bool m_running = false;
    Camera m_camera;

    int m_current_pattern_index = 0;
    std::vector<Pattern> m_patterns;

    /// 周囲8マスの生きてるセルの数を返す
    int around(int x, int y) const;
};
