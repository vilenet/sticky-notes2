#ifndef OPEN_NOTE_H
#define OPEN_NOTE_H

#include "data.h"
#include "app.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Hold_Browser.H>

#include <unordered_map>


class COpenNote : public Fl_Window {
private:
    Fl_Hold_Browser* m_pBrowser;
    Fl_Button* m_pOpenButton;

    App* m_pApp;

    static void open_button_callback(Fl_Widget* w, void* userdata);
    void on_open_button_click();

public:
    COpenNote(int x, int y, int w, int h, const char* title, App* pApp);

    void InitBrowser();
    void Run();
};

#endif // OPEN_NOTE_H
