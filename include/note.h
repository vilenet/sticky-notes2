#ifndef NOTE_H
#define NOTE_H

#include "data.h"
#include "app.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/x.H>

#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

class App;


class Note : public Fl_Window {
private:
    Data* m_data;
    App*  m_pApp;

    Fl_Color fl_color;
    Fl_Text_Editor editor;
    Fl_Text_Buffer buffer;

    WNDPROC winProc = nullptr;
    HMENU hMenu;

    bool isTextChanged = false;
    bool isPositionChanged = false;
    bool isSizeChanged = false;
    bool isStateChanged = false;
    bool isColorChanged = false;
    bool isNewNote = true;

private:
    static void textChangedCallback(Fl_Widget* widget, void* userdata);
    static void updateCheckerCallback(void* userdata);

public:
    Note(Data* data, App* pApp);
    ~Note();

    int  getId() const;
    void setText(const std::string& text);
    std::string  getText() const;
    void setState(bool);
    bool getState();
    bool isNew();

    // Icon & Menu
    HWND getHWND();
    void setIcon(const std::string& iconPath);
    void createMenu();
    void showMenu();
    void handleMenu(int command);
    void setWinProc();
    static LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Actions 
    void action_close();
    void action_new();
    void action_open();
    void action_delete();
    void action_exit();

    void UpdateData();

    void resize(int X, int Y, int W, int H) override;

    // Нужно ли нам это?
    friend class App; // Allow App to access private members
};

#endif // NOTE_H
