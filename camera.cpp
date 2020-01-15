#include "camera.h"
#include <SDL2/SDL.h>

void Camera::update() {
    float speed = 50;
    const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
    if (keystate[SDL_SCANCODE_DOWN])
        py += speed;
    if (keystate[SDL_SCANCODE_LEFT])
        px -= speed;
    if (keystate[SDL_SCANCODE_RIGHT])
        px += speed;
    if (keystate[SDL_SCANCODE_UP])
        py -= speed;

    int pos_px, pos_py;
    Uint32 mousestate = SDL_GetMouseState(&pos_px, &pos_py);
    if (mousestate & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
        if (m_prev_pos.has_value()) {
            auto [prev_px, prev_py] = m_prev_pos.value();
            px += (prev_px - pos_px);
            py += (prev_py - pos_py);
        }
        m_prev_pos = {pos_px, pos_py};
    } else {
        m_prev_pos = std::nullopt;
    }
}
