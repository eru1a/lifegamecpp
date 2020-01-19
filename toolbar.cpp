#include "toolbar.h"
#include "lifegame.h"

bool contain(int px, int py, const SDL_Rect &rect) {
    return (rect.x <= px && px <= rect.x + rect.w && rect.y <= py && py <= rect.y + rect.h);
}

ToolBar::ToolBar(SDL_Renderer *renderer, TTF_Font *font, const SDL_Rect &viewport,
                 LifeGame *lifegame)
    : m_renderer(renderer), m_font(font), m_viewport(viewport), m_lifegame(lifegame) {
    m_play_button = load_texture(renderer, "data/play.png");
    m_stop_button = load_texture(renderer, "data/stop.png");
    m_step_button = load_texture(renderer, "data/step.png");
    m_clear_button = load_texture(renderer, "data/clear.png");
    m_left_button = load_texture(renderer, "data/left.png");
    m_right_button = load_texture(renderer, "data/right.png");

    m_play_stop_rect = {margin * 1 + button_width * 0, margin, button_width, button_height};
    m_step_rect = {margin * 3 + button_width * 1, margin, button_width, button_height};
    m_clear_rect = {margin * 5 + button_width * 2, margin, button_width, button_height};
    m_left_rect = {margin * 7 + button_width * 3, margin, button_width, button_height};
    m_pattern_rect = {margin * 9 + button_width * 4, margin, pattern_text_width, button_height};
    m_right_rect = {margin * 11 + button_width * 4 + pattern_text_width, margin, button_width,
                    button_height};

    make_pattern_texture(m_lifegame->current_pattern_name());
}

void ToolBar::update(SDL_Event e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        auto [mouse_px, mouse_py, mousestate] = get_mousestate(m_viewport);
        (void)mousestate;

        if (contain(mouse_px, mouse_py, m_play_stop_rect)) {
            if (m_lifegame->is_running()) {
                m_lifegame->stop();
            } else {
                m_lifegame->run();
            }
        } else if (contain(mouse_px, mouse_py, m_step_rect)) {
            m_lifegame->step();
        } else if (contain(mouse_px, mouse_py, m_clear_rect)) {
            m_lifegame->clear();
        } else if (contain(mouse_px, mouse_py, m_left_rect)) {
            m_lifegame->prev_pattern();
            make_pattern_texture(m_lifegame->current_pattern_name());
        } else if (contain(mouse_px, mouse_py, m_right_rect)) {
            m_lifegame->next_pattern();
            make_pattern_texture(m_lifegame->current_pattern_name());
        }
    }
}

void ToolBar::draw() const {
    SDL_RenderSetViewport(m_renderer, &m_viewport);

    if (m_lifegame->is_running()) {
        SDL_RenderCopy(m_renderer, m_stop_button, nullptr, &m_play_stop_rect);
    } else {
        SDL_RenderCopy(m_renderer, m_play_button, nullptr, &m_play_stop_rect);
    }
    SDL_RenderCopy(m_renderer, m_step_button, nullptr, &m_step_rect);
    SDL_RenderCopy(m_renderer, m_clear_button, nullptr, &m_clear_rect);
    SDL_RenderCopy(m_renderer, m_left_button, nullptr, &m_left_rect);
    SDL_RenderCopy(m_renderer, m_right_button, nullptr, &m_right_rect);

    SDL_Rect pattern_texture_rect = {m_pattern_rect.x + 10, m_pattern_rect.y,
                                     m_pattern_texture_width, m_pattern_texture_height};
    SDL_RenderCopy(m_renderer, m_pattern_texture, nullptr, &pattern_texture_rect);
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(m_renderer, &m_pattern_rect);
}

void ToolBar::pattern_texture_free() {
    if (m_pattern_texture != nullptr) {
        SDL_DestroyTexture(m_pattern_texture);
        m_pattern_texture = nullptr;
        m_pattern_texture_width = 0;
        m_pattern_texture_height = 0;
    }
}

void ToolBar::make_pattern_texture(const std::string &text) {
    pattern_texture_free();
    SDL_Surface *text_surface = TTF_RenderText_Solid(m_font, text.c_str(), {255, 255, 255, 255});
    if (text_surface == nullptr)
        ttf_error("TTF_RenderTest_Solidで失敗");
    m_pattern_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    if (m_pattern_texture == nullptr)
        sdl_error("SDL_CreateTextureFromSurfaceで失敗");
    m_pattern_texture_width = text_surface->w;
    m_pattern_texture_height = text_surface->h;
    SDL_FreeSurface(text_surface);
}
