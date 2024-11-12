#ifndef APP_H
#define APP_H

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>

#include <windows.h>
#include <FL/x.H>

#include <cstdio>
#include <string>
#include <limits>
#include <vector>
#include <limits>


class StickyNoteApp : public Fl_Window {
private:
    Fl_Text_Editor text_editor {0, 25, 300, 300};
    Fl_Text_Buffer text_buffer;
    Fl_Color editor_color;

    bool text_changed;
    char filename[FILENAME_MAX];

public:
    StickyNoteApp();
    ~StickyNoteApp();

};


#endif // APP_H
