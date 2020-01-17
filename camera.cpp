#include "camera.h"
#include <iostream>

void Camera::update(Uint32 mousestate, const Uint8 *keystate,
                    const std::tuple<int, int> &mouse_pos) {
    // 矢印キーで移動
    float speed = 50;
    if (keystate[SDL_SCANCODE_DOWN])
        m_py += speed;
    if (keystate[SDL_SCANCODE_LEFT])
        m_px -= speed;
    if (keystate[SDL_SCANCODE_RIGHT])
        m_px += speed;
    if (keystate[SDL_SCANCODE_UP])
        m_py -= speed;

    // 中クリックで移動
    auto [pos_px, pos_py] = mouse_pos;
    if (mousestate & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
        if (m_prev_pos.has_value()) {
            auto [prev_px, prev_py] = m_prev_pos.value();
            m_px += (prev_px - pos_px);
            m_py += (prev_py - pos_py);
        }
        m_prev_pos = {pos_px, pos_py};
    } else {
        m_prev_pos = std::nullopt;
        if (mousestate & SDL_BUTTON(SDL_BUTTON_X1)) {
            std::cout << "test" << std::endl;
        }
    }
}

void Camera::update(const SDL_Event &e) {
    // マウスホイールでズーム
    // TODO: 座標を変更していないのですごく不自然なズームになる
    if (e.type == SDL_MOUSEWHEEL) {
        // e.wheel.yは上スクロールなら正、下スクロールなら負の値となる
        float zoom_speed = 0.2 * e.wheel.y;
        m_zoom *= (1 + zoom_speed);
    }
}
