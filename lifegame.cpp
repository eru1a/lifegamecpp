#include "lifegame.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>

void LifeGame::update() {
    // マウスとキーボードの状態を取得
    // マウスを動かすまで座標が常に(0, 0)になるのはバグ?
    // https://bugzilla.libsdl.org/show_bug.cgi?id=3487
    auto [mouse_px, mouse_py, mousestate] = get_mousestate(m_viewport);
    const Uint8 *keystate = SDL_GetKeyboardState(nullptr);

    // カメラを更新
    m_camera.update(mousestate, keystate, {mouse_px, mouse_py});
    auto [camera_px, camera_py] = m_camera.get_pos();
    float zoom = m_camera.get_zoom();
    float gs = GS * zoom;

    // 左クリックで誕生させる。右クリックで死滅させる。
    int selected_x = std::floor((mouse_px + camera_px) / gs);
    int selected_y = std::floor((mouse_py + camera_py) / gs);
    if (0 <= selected_x && selected_x < m_col && 0 <= selected_y && selected_y < m_row) {
        if (mousestate & SDL_BUTTON_LMASK) {
            Pattern pattern = m_patterns.at(m_current_pattern_index);
            for (int yy = 0; yy < pattern.row; yy++) {
                for (int xx = 0; xx < pattern.col; xx++) {
                    int x = selected_x + xx;
                    int y = selected_y + yy;
                    if (0 <= x && x < m_col && 0 <= y && y < m_row &&
                        pattern.pattern.at(yy).at(xx)) {
                        m_field.at(y).at(x) = true;
                    }
                }
            }
        } else if (mousestate & SDL_BUTTON_RMASK) {
            m_field.at(selected_y).at(selected_x) = false;
        }
    }

    if (m_running) {
        step();
    }
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

void LifeGame::draw() const {
    SDL_RenderSetViewport(m_renderer, &m_viewport);
    auto [camera_px, camera_py] = m_camera.get_pos();
    float zoom = m_camera.get_zoom();
    float gs = GS * zoom;
    // 必要な範囲だけ描画する
    int startx = std::max(0, int(camera_px / gs));
    int starty = std::max(0, int(camera_py / gs));
    for (int y = starty; y < m_row; y++) {
        float py = y * gs - camera_py;
        if (py > m_height)
            break;
        for (int x = startx; x < m_col; x++) {
            float px = x * gs - camera_px;
            if (px > m_width)
                break;
            SDL_FRect rect = {px, py, gs, gs};
            if (m_field.at(y).at(x)) {
                SDL_SetRenderDrawColor(m_renderer, 0, 200, 0, 255);
                SDL_RenderFillRectF(m_renderer, &rect);
            } else {
                SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
                SDL_RenderFillRectF(m_renderer, &rect);
            }
        }
    }

    // マウスで選択されているセルに合わせて今のパターンを描画
    // TODO: 範囲チェック
    auto [mouse_px, mouse_py, mousestate] = get_mousestate(m_viewport);
    // 使わない変数を受け取らないようにするには...?
    (void)mousestate;
    // 複雑...
    float selected_px =
        (std::floor((mouse_px + camera_px) / gs) - std::floor(camera_px / gs)) * gs -
        (camera_px - std::floor(camera_px / gs) * gs);
    float selected_py =
        (std::floor((mouse_py + camera_py) / gs) - std::floor(camera_py / gs)) * gs -
        (camera_py - std::floor(camera_py / gs) * gs);
    Pattern pattern = m_patterns.at(m_current_pattern_index);
    for (int y = 0; y < pattern.row; y++) {
        for (int x = 0; x < pattern.col; x++) {
            if (pattern.pattern.at(y).at(x)) {
                SDL_FRect rect = {selected_px + x * gs, selected_py + y * gs, gs, gs};
                SDL_SetRenderDrawColor(m_renderer, 0, 0, 200, 255);
                SDL_RenderFillRectF(m_renderer, &rect);
            }
        }
    }

    // グリッド
    // TODO: zoomに合わせてグリッドの幅を変更する
    SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 255);
    {
        float x0 = 0 * gs - camera_px;
        float x1 = m_col * gs - camera_px;
        for (int y = 0; y <= m_row; y++) {
            float yy = y * gs - camera_py;
            SDL_RenderDrawLineF(m_renderer, x0, yy, x1, yy);
        }
    }
    {
        float y0 = 0 * gs - camera_py;
        float y1 = m_row * gs - camera_py;
        for (int x = 0; x <= m_col; x++) {
            float xx = x * gs - camera_px;
            SDL_RenderDrawLineF(m_renderer, xx, y0, xx, y1);
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
