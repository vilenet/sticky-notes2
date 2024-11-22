#include "settings.h"
#include "color.h"
#include "dout.h"


Settings::Settings(int parent_x, int parent_y, int parent_w, Note* pNote)
    : Fl_Window(parent_x, parent_y, 0, 0, "Settings")
    , m_btn_size(30)
    , m_space(15)
    , m_cols(6)
    , m_rows(1)
    , m_pNote(pNote) {

    //border(0);
    set_modal();

    //- Window size & position --------------------------------------------------------
    m_window_w = (6 * m_btn_size) + (m_space * 2);
    m_window_h = 150;
    m_centerX = parent_x + (parent_w - m_window_w) / 2;

    this->resize(m_centerX, parent_y + m_space, m_window_w, m_window_h);

    //- Input ---------------------------------------------------------------------------
    int input_y = m_space;
    int input_h = 30;
    int input_x = m_space;
    int input_w = m_window_w - m_space - input_x;
    Fl_Input* titleInput = new Fl_Input(input_x, input_y, input_w, input_h);
    titleInput->value(m_pNote->getTitle().c_str());

    titleInput->callback([](Fl_Widget* input, void* settingsPtr) {
        Settings* pSettings = static_cast<Settings*>(settingsPtr);
        Note* note = pSettings->getNote();
        note->setTitle(static_cast<Fl_Input*>(input)->value());
        note->setTitleChanged(true);
        note->UpdateData();
    }, this);

    //- Coloered Buttons --------------------------------------------------------------------
    //TODO: remove Fl Colors example 
    //const Fl_Color colors[] = { FL_RED, FL_GREEN, FL_BLUE, FL_YELLOW, FL_MAGENTA, FL_CYAN };

    const char* colors[] = { 
        ColorUtil::BEIDGE, 
        ColorUtil::YELLOW, 
        ColorUtil::PINK, 
        ColorUtil::BLUE, 
        ColorUtil::GREEN, 
        ColorUtil::GREY 
    };

    int btn_color_y = input_y + input_h + m_space;

    for (int i = 0; i < 6; ++i) {
        int btn_color_x = m_space + (i * m_btn_size);
        Fl_Button* buttonColor = new Fl_Button(btn_color_x, btn_color_y, m_btn_size, m_btn_size);
        // buttonColor->color(colors[i]);
        buttonColor->color(ColorUtil::str2color(colors[i]));

        buttonColor->callback([](Fl_Widget* btn, void* settingsPtr) {
            Settings* pSettings = static_cast<Settings*>(settingsPtr);
            Note* note = pSettings->getNote();
            note->setColor(btn->color());
            note->setColorChanged(true);
            note->UpdateData();
        }, this);
    }

    //- Button Close ----------------------------------------------------------------------
    int btn_close_w = 60; 
    int btn_close_h = 20;
    int btn_close_x = m_window_w - btn_close_w - m_space; // Right pos
    int btn_close_y = m_window_h - btn_close_h - m_space; // Bottom pos

    Fl_Button* buttonClose = new Fl_Button(btn_close_x, btn_close_y, btn_close_w, btn_close_h, "Close");

    buttonClose->callback([](Fl_Widget* btn, void* settingsPtr) {
        Settings* pSettings = static_cast<Settings*>(settingsPtr);
        pSettings->hide();
    }, this);

    //-----------------------------------------------------------------------------------
    end();
}

Note* Settings::getNote() const { return m_pNote; }


