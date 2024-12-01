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
    // Fl widgets
    Fl_Text_Editor m_FlEditor;
    Fl_Text_Buffer m_FlBuffer;

    Fl_Color m_Flcolor;

    // WinApi
    WNDPROC winProc = nullptr;
    HMENU hMenu;

    Data* m_data;
    App*  m_pApp;

    bool isTextChanged = false;
    bool isPositionChanged = false;
    bool isSizeChanged = false;
    bool isStateChanged = false;
    bool isColorChanged = false;
    bool isTitleChanged = false;

private:
    // Callbacks
    static void TextChangedCallback(int, int, int, int, const char*, void*);
    static void UpdateCheckerCallback(void* userdata);

public:
    Note(Data* data, App* pApp);
    ~Note();

    // Getters & Setters
    int  GetID() const;
    Str  GetTitle() const;
    void SetTitle(const Str& name);
    void SetText(const Str& text);
    Str  GetText() const;
    void SetState(bool);
    bool GetState();
    void SetColor(Fl_Color);
    Fl_Color GetColor();

    // Icon & Menu
    HWND GetHWND();
    void SetIcon(const Str& iconMenu, const Str& iconTaskbar);
    void CreateMenu();
    void ShowMenu();
    void HandleMenu(int command);
    void SetWinProc();
    static LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // On Menu Click Actions
    void OpenSettings();
    void CloseNote();
    void NewNote();
    void OpenNote();
    void DeleteNote();
    void ExitApp();

    void UpdateData();

    // Fl widgets Setup
    void SetupTxtEditor();

    // Overrided
    void resize(int X, int Y, int W, int H) override;

    // Flag Setters
    void SetColorChanged(bool);
    void SetTitleChanged(bool);
};

#endif // NOTE_H
