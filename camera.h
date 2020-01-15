#pragma once

/// 描画する時の左上の座標(ピクセル単位)
struct Camera {
    Camera() : px(0), py(0) {}
    Camera(float px, float py) : px(px), py(py) {}

    void update();

    float px;
    float py;
};
