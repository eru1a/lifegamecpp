#pragma once

#include <SDL2/SDL.h>
#include <optional>
#include <tuple>

/// 描画する時の左上の座標(ピクセル単位)
class Camera {
public:
    Camera() : m_px(0), m_py(0), m_zoom(1) {}
    Camera(float px, float py) : m_px(px), m_py(py) {}

    std::tuple<float, float> get_pos() const { return {m_px, m_py}; }
    float get_zoom() const { return m_zoom; }

    void update(Uint32 mousestate, const Uint8 *keystate, const std::tuple<int, int> &mouse_pos);
    void update(const SDL_Event &e, const std::tuple<int, int> &mouse_pos);

private:
    float m_px;
    float m_py;
    float m_zoom;
    /// 直前中クリックした位置
    std::optional<std::tuple<int, int>> m_prev_pos;
};
