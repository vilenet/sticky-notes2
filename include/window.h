#ifndef WINDOW_H
#define WINDOW_H
#include <FL/fl_ask.H> 
#include <FL/Fl_Window.H>
//#include <FL/Fl_Text_Editor.H>
//#include <FL/Fl_Text_Buffer.H>
#include <FL/x.H>

#include <windows.h>


class CWindow : public Fl_Window {
private:
    WNDPROC originalWindowProc;
    HMENU hCustomMenu;

    // Fl_Text_Editor text_editor {0, 25, 300, 300};
    // Fl_Text_Buffer text_buffer;
    // Fl_Color editor_color;

    // bool text_changed;
    // char filename[FILENAME_MAX];

    bool active;

private:
    void setWindowIcon();
    void setCustomWindowProc();
    void createMenu();

    static LRESULT CALLBACK CustomWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void showCustomMenu(HWND hwnd);


public:
    CWindow(int width, int height, const char* title);
    virtual ~CWindow();


    void initialize();
    //TODO set/getActive
};

#endif // END WINDOW_H