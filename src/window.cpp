#include "window.h"
#include <FL/fl_ask.H>
#include <windows.h>


CWindow::CWindow(int width, int height, const char* title): Fl_Window(width, height, title) {
    resizable(this);
    createMenu();
}

CWindow::~CWindow() {}

void CWindow::initialize() {
    setWindowIcon();
    setCustomWindowProc();
}

void CWindow::createMenu() {
    hCustomMenu = CreatePopupMenu();
    AppendMenuW(hCustomMenu, MF_STRING, 1001, L"New");
    AppendMenuW(hCustomMenu, MF_STRING, 1002, L"Open");
    AppendMenuW(hCustomMenu, MF_STRING, 1003, L"Delete");
    AppendMenuW(hCustomMenu, MF_STRING, 1004, L"Exit");
}

void CWindow::setWindowIcon() {
    HWND hwnd = fl_xid(this);
    if (hwnd) {
        HICON hIcon = (HICON)LoadImageA(nullptr, "menu_icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
        if (hIcon) {
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        } else {
            fl_alert("Failed to load icon");
        }
    }
}

void CWindow::setCustomWindowProc() {
    HWND hwnd = fl_xid(this);
    if (hwnd) {
        originalWindowProc = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)CustomWindowProc);
    }
}

LRESULT CALLBACK CWindow::CustomWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    CWindow* self = reinterpret_cast<CWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (self) {
        switch (uMsg) {
            case WM_SYSCOMMAND:
                if ((wParam & 0xFFF0) == SC_MOUSEMENU) {
                    self->showCustomMenu(hwnd);
                    return 0;
                }
                break;
        }
        return CallWindowProc(self->originalWindowProc, hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CWindow::showCustomMenu(HWND hwnd) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    POINT topLeft = { clientRect.left, clientRect.top };
    ClientToScreen(hwnd, &topLeft);

    int cmd = TrackPopupMenu(hCustomMenu, TPM_RETURNCMD | TPM_LEFTBUTTON, topLeft.x, topLeft.y, 0, hwnd, nullptr);
    if (cmd == 1001) {
        fl_alert("New action");
    } 
    else if (cmd == 1002) {
        fl_alert("Open action");
    }
    else if (cmd == 1003) {
        fl_alert("Delete action");
    }  
    else if (cmd == 1004) {
        //Exit action
        fl_alert("Exit action");
        //PostQuitMessage(0);
    }
}

//TODO set/getActive