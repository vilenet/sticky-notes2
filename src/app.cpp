#include "app.h"
#include "window.h"
#include <FL/Fl.H>

CApp& CApp::getInstance() {
    static CApp instance;
    return instance;
}

void CApp::run() {
    createWindow(300, 300, "Sticky Note 1");
    //createWindow(300, 300, "Sticky Note 2");

    Fl::run();
}

void CApp::createWindow(int width, int height, const char* title) {
    auto window = std::make_unique<CWindow>(width, height, title);
    window->show();
    window->initialize();
    windows.push_back(std::move(window));
    openWindows++;
}

void CApp::onWindowClose() {
    openWindows--;
    if (openWindows == 0) {
        exit(0);
    }
}
