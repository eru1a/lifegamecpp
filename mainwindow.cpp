#include "mainwindow.h"

void MainWindow::update() { m_lifegame.update(); }

void MainWindow::update(SDL_Event e) {
    m_toolbar.update(e);
    m_lifegame.update(e);
}

void MainWindow::draw(SDL_Renderer *renderer) const {
    m_toolbar.draw(renderer);
    m_lifegame.draw(renderer);
}
