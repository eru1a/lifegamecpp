#include "toolbar.h"
#include "lifegame.h"

void ToolBar::update(SDL_Event e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        auto [mouse_px, mouse_py, mousestate] = get_mousestate(m_viewport);
        (void)mousestate;

        if (m_play_stop_rect.x <= mouse_px &&
            mouse_px <= m_play_stop_rect.x + m_play_stop_rect.w) {
            if (m_lifegame->is_running()) {
                m_lifegame->stop();
            } else {
                m_lifegame->run();
            }
        } else if (m_step_rect.x <= mouse_px && mouse_px <= m_step_rect.x + m_step_rect.w) {
            m_lifegame->step();
        } else if (m_clear_rect.x <= mouse_px && mouse_px <= m_clear_rect.x + m_clear_rect.w) {
            m_lifegame->clear();
        }
    }
}

void ToolBar::draw(SDL_Renderer *renderer) const {
    SDL_RenderSetViewport(renderer, &m_viewport);

    if (m_lifegame->is_running()) {
        SDL_RenderCopy(renderer, m_stop_button, nullptr, &m_play_stop_rect);
    } else {
        SDL_RenderCopy(renderer, m_play_button, nullptr, &m_play_stop_rect);
    }
    SDL_RenderCopy(renderer, m_step_button, nullptr, &m_step_rect);
    SDL_RenderCopy(renderer, m_clear_button, nullptr, &m_clear_rect);
}
