#ifndef SETTINGS_H
#define SETTINGS_H

#include "note.h"

#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>


class Settings : public Fl_Window {
private:
    int m_size;
    int m_space; 
    int m_cols; 
    int m_rows;
    int m_btn_size;
    int m_window_w;
    int m_window_h;
    int m_centerX;
    Note* m_pNote;
    
public:
    Settings(int x, int y, int w, Note* pNote);
    ~Settings() = default;

    Note* getNote() const;
};

#endif // SETTINGS_H
