#include "lifegame.h"
#include <iostream>

void LifeGame::update() {
    // マウスとキーボードの状態を取得
    // マウスを動かすまで座標が常に(0, 0)になるのはバグ?
    // https://bugzilla.libsdl.org/show_bug.cgi?id=3487
    int px, py;
    Uint32 mousestate = SDL_GetMouseState(&px, &py);
    const Uint8 *keystate = SDL_GetKeyboardState(nullptr);

    // カメラを更新
    m_camera.update(mousestate, keystate, {px, py});
    auto [camera_px, camera_py] = m_camera.get_pos();
    float zoom = m_camera.get_zoom();
    float gs = GS * zoom;

    // 左クリックで誕生させる。右クリックで死滅させる。
    int x = (px + camera_px) / gs;
    int y = (py + camera_py) / gs;
    if (0 <= x && x < m_col && 0 <= y && y < m_row) {
        if (mousestate & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            m_field.at(y).at(x) = true;
        } else if (mousestate & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
            m_field.at(y).at(x) = false;
        }
    }

    if (m_running)
        step();
}

void LifeGame::update(const SDL_Event &e) {
    m_camera.update(e);

    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_n:
            step();
            break;
        case SDLK_c:
            clear();
            break;
        case SDLK_SPACE:
        case SDLK_s:
            toggle();
            break;
        }
    }
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
    auto [camera_px, camera_py] = m_camera.get_pos();
    float zoom = m_camera.get_zoom();
    float gs = GS * zoom;
    // TODO: 全部を描画しているけど見える範囲だけ描画すればいい
    for (int y = 0; y < m_row; y++) {
        for (int x = 0; x < m_col; x++) {
            SDL_FRect rect = {x * gs - camera_px, y * gs - camera_py, gs, gs};
            if (m_field.at(y).at(x)) {
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
                SDL_RenderFillRectF(renderer, &rect);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRectF(renderer, &rect);
            }
            // グリッド
            // TODO: zoomに合わせてグリッドの幅を変更する
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
