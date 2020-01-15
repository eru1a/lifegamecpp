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
}
