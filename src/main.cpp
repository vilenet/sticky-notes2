// TODO: Refactor using OOP; split into app.h, app.cpp, main.cpp.
// This template shows how to set a custom window icon and handle custom menu actions on the Win32 title bar.
// Note: Uses platform-specific WinAPI, but simplifies window resizing and control handling.


#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_ask.H> 
#include <windows.h>

// Оригинальный обработчик сообщений окна
WNDPROC originalWindowProc;

// Ваше собственное системное меню
HMENU hCustomMenu;

LRESULT CALLBACK CustomWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_SYSCOMMAND:
            // Перехватываем только нажатие на иконку окна
            if ((wParam & 0xFFF0) == SC_MOUSEMENU) {
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);

                // Преобразуем координаты клиентской области в экранные
                POINT topLeft = { clientRect.left, clientRect.top };
                ClientToScreen(hwnd, &topLeft);

                // Отображаем кастомное меню в верхнем левом углу окна
                int cmd = TrackPopupMenu(hCustomMenu, TPM_RETURNCMD | TPM_LEFTBUTTON, topLeft.x, topLeft.y, 0, hwnd, nullptr);
                
                if (cmd == 1001) {
                    fl_alert("Выбрано: Открыть");
                } else if (cmd == 1002) {
                    fl_alert("Выбрано: Сохранить");
                } else if (cmd == 1003) {
                    PostQuitMessage(0);
                }
                return 0; // Предотвращаем стандартное системное меню
            }

            // Для всех остальных системных команд передаём управление стандартному обработчику
            if (wParam == SC_SIZE || wParam == SC_MOVE || wParam == SC_MINIMIZE || wParam == SC_MAXIMIZE || wParam == SC_CLOSE) {
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
            break;
    }

    // Для всех остальных сообщений вызываем оригинальный обработчик
    return CallWindowProc(originalWindowProc, hwnd, uMsg, wParam, lParam);
}

void set_window_icon(Fl_Window* window) {
    HWND hwnd = fl_xid(window);
    if (hwnd == nullptr) {
        fl_alert("Не удалось получить дескриптор окна");
        return;
    }

    HICON hIcon = (HICON)LoadImageA(
        nullptr,
        "menu_icon.ico",
        IMAGE_ICON,
        32, 32,
        LR_LOADFROMFILE
    );

    if (hIcon) {
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    } else {
        fl_alert("Не удалось загрузить иконку");
    }
}

void SetCustomWProc(Fl_Window* window) {
    HWND hwnd = fl_xid(window);
    originalWindowProc = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)CustomWindowProc);
}

void create_custom_menu() {
    // Создаём новое меню
    hCustomMenu = CreatePopupMenu();
    AppendMenuW(hCustomMenu, MF_STRING, 1001, L"Открыть");
    AppendMenuW(hCustomMenu, MF_STRING, 1002, L"Сохранить");
    AppendMenuW(hCustomMenu, MF_STRING, 1003, L"Выход");
}

int main() 
{
    Fl_Window window(400, 300, "FLTK Window with Custom Icon Action");
    window.resizable(window);
    window.show();

    create_custom_menu();
    SetCustomWProc(&window);
    set_window_icon(&window);

    return Fl::run();
}
