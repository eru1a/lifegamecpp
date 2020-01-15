#pragma once

#include <optional>
#include <tuple>

/// 描画する時の左上の座標(ピクセル単位)
class Camera {
public:
    Camera() : px(0), py(0) {}
    Camera(float px, float py) : px(px), py(py) {}

    void update();

    float px;
    float py;

private:
    /// 直前中クリックした位置
    std::optional<std::tuple<int, int>> m_prev_pos;
};
