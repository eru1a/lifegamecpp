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
    }
}

void Camera::update(const SDL_Event &e, const std::tuple<int, int> &mouse_pos) {
    // マウスホイールでズーム
    if (e.type != SDL_MOUSEWHEEL)
        return;

    // e.wheel.yは上スクロールなら正、下スクロールなら負の値となる
    float delta_zoom = 0.2 * e.wheel.y;
    float zoom_level = (1 + delta_zoom);
    m_zoom *= zoom_level;

    // TODO: ずれる。カーソルを中心に拡大/縮小ってどうやるの...
    auto [mouse_px, mouse_py] = mouse_pos;
    double fx = (m_px + mouse_px) / m_zoom;
    double fy = (m_py + mouse_py) / m_zoom;
    m_px += delta_zoom * fx;
    m_py += delta_zoom * fy;

    // これも駄目
    // auto [mouse_px, mouse_py] = mouse_pos;
    // m_px = m_px + (mouse_px - m_px) * (1 - 1 / zoom_level);
    // m_py = m_py + (mouse_py - m_py) * (1 - 1 / zoom_level);
}
