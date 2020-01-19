#include "mainwindow.h"

void MainWindow::update() { m_lifegame.update(); }

void MainWindow::update(SDL_Event e) {
    m_toolbar.update(e);
    m_lifegame.update(e);
}

void MainWindow::draw() const {
    m_toolbar.draw();
    m_lifegame.draw();
}
