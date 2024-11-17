#ifndef SELECTOR_H
#define SELECTOR_H

#include "data.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Select_Browser.H>

#include <unordered_map>


class NoteSelector {
private:
    Fl_Window* dialog;
    Fl_Select_Browser* browser;
    Fl_Button* openBtn;

    Data* selectedData = nullptr;

    static void openCallback(Fl_Widget* w, void* userdata) {
        NoteSelector* selector = static_cast<NoteSelector*>(userdata);
        selector->selectNote();
    }

    void selectNote();

public:
    NoteSelector(const std::unordered_map<int, Data*>& datas);
    ~NoteSelector();
    Data* select();
};

#endif // SELECTOR_H
