#ifndef SETTINGS_H
#define SETTINGS_H

#include "note.h"

#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>


class CSettings : public Fl_Window {
private:
    // Fl widgets
    Fl_Input*  m_pInputTitle {nullptr};
    Fl_Button* m_pCloseButton {nullptr};
    Fl_Button* m_pColorButtons[6] {};

    Note* m_pNote {nullptr};

    const int m_space {15};
    const int m_btnSz {30};

private:
    // Fl widgets Callbacks
    static void TitleInputCallback(Fl_Widget* w, void* userdata);
    static void ColorButtonCallback(Fl_Widget* w, void* userdata);
    static void CloseButtonCallback(Fl_Widget* w, void* userdata);

    // Fl widgets Setup
    void SetupTitleInput();
    void SetupColorButtons();
    void SetupCloseButton();

    void SetupWindow();

public:
    CSettings(int x, int y, int w, int h, const char* title, Note* pNote);
    ~CSettings() = default;

    void Run();

    // Getters & Setters
    Note* GetNote() const;
};

#endif // SETTINGS_H
