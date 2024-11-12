#ifndef APP_H
#define APP_H

#include "window.h"
#include <vector>
#include <memory>


class CApp {
private:
    std::vector<std::unique_ptr<CWindow>> windows;
    int openWindows = 0;

private:
    CApp() = default;
    ~CApp() = default;
    CApp(const CApp&) = delete;
    CApp& operator=(const CApp&) = delete;

public:
    static CApp& getInstance();

    void run();
    void createWindow(int width, int height, const char* title);
    void onWindowClose();
};

#endif // APP_H
