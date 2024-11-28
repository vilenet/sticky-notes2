#ifndef SELECTOR_H
#define SELECTOR_H

#include "data.h"
#include "app.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Hold_Browser.H>

#include <unordered_map>


class NoteSelector : public Fl_Window {
private:
    Fl_Hold_Browser* browser;
    Fl_Button* openBtn;

    App* m_pApp;

    static void openCallback(Fl_Widget* w, void* userdata);
    void on_button_click();

public:
    NoteSelector(int x, int y, int w, int h, const char* title, App* pApp);

    void InitBrowser();
    void Run();
};

#endif // SELECTOR_H
