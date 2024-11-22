#ifndef NOTE_H
#define NOTE_H

#include "data.h"
#include "app.h"
#include "color.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/x.H>
#include <FL/Fl_Box.H>

#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

using Str = std::string;

class App;


class Note : public Fl_Window {
private:
    Data* m_data;
    App*  m_pApp;

    Fl_Color m_Flcolor;
    Fl_Text_Editor editor;
    Fl_Text_Buffer buffer;

    WNDPROC winProc = nullptr;
    HMENU hMenu;

    bool isTextChanged = false;
    bool isPositionChanged = false;
    bool isSizeChanged = false;
    bool isStateChanged = false;
    bool isColorChanged = false;
    bool isTitleChanged = false;

private:
    static void textChangedCallback(Fl_Widget* widget, void* userdata);
    static void updateCheckerCallback(void* userdata);

public:
    Note(Data* data, App* pApp);
    ~Note();

    int  getId() const;
    Str  getTitle() const;
    void setTitle(const Str& name);
    void setText(const Str& text);
    Str  getText() const;
    void setState(bool);
    bool getState();
    void setColor(Fl_Color);
    Fl_Color getColor();

    // Icon & Menu
    HWND getHWND();
    void setIcon(const Str& iconMenu, const Str& iconTaskbar);
    void createMenu();
    void showMenu();
    void handleMenu(int command);
    void setWinProc();
    static LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Actions 
    void action_settings();
    void action_close();
    void action_new();
    void action_open();
    void action_delete();
    void action_exit();

    void UpdateData();

    // Overrided
    void resize(int X, int Y, int W, int H) override;

    //TODO remove
    friend class App;

    void setColorChanged(bool);
    void setTitleChanged(bool);
};

#endif // NOTE_H
