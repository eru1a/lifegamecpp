#include "lifegame.h"

void LifeGame::update() {
    int px, py;
    Uint32 mousestate = SDL_GetMouseState(&px, &py);
    int x = px / GS;
    int y = py / GS;
    if (mousestate & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        m_field.at(y).at(x) = true;
    } else if (mousestate & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        m_field.at(y).at(x) = false;
    }
    if (m_running)
        step();
}

void LifeGame::step() {
    std::vector<std::vector<bool>> next(m_row, std::vector<bool>(m_col, false));

    for (int y = 0; y < m_row; y++)
        for (int x = 0; x < m_col; x++)
            switch (around(x, y)) {
            case 2:
                // 周囲の2セルが生きていれば維持
                next.at(y).at(x) = m_field.at(y).at(x);
                break;
            case 3:
                // 周囲の3セルが生きていれば誕生
                next.at(y).at(x) = true;
                break;
            default:
                // それ以外は死亡
                next.at(y).at(x) = false;
                break;
            }

    m_field = next;
    m_generation++;
}

void LifeGame::clear() {
    for (int y = 0; y < m_row; y++)
        for (int x = 0; x < m_col; x++)
            m_field.at(y).at(x) = false;
    m_generation = 0;
    m_running = false;
}

void LifeGame::draw(SDL_Renderer *renderer) const {
    for (int y = 0; y < m_row; y++) {
        for (int x = 0; x < m_col; x++) {
            SDL_FRect rect = {x * GS, y * GS, GS, GS};
            if (m_field.at(y).at(x)) {
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
                SDL_RenderFillRectF(renderer, &rect);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRectF(renderer, &rect);
            }
            // グリッド
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderDrawRectF(renderer, &rect);
        }
    }
}

int LifeGame::around(int x, int y) const {
    // 端のマスは0を返す
    if (x == 0 || x == m_col - 1 || y == 0 || y == m_row - 1)
        return 0;
    int result = 0;
    result += m_field.at(y - 1).at(x - 1);
    result += m_field.at(y - 1).at(x + 1);
    result += m_field.at(y - 1).at(x);
    result += m_field.at(y + 1).at(x - 1);
    result += m_field.at(y + 1).at(x + 1);
    result += m_field.at(y + 1).at(x);
    result += m_field.at(y).at(x - 1);
    result += m_field.at(y).at(x + 1);
    return result;
}
