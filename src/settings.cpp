#include "settings.h"
#include "color.h"
#include "dout.h"


CSettings::CSettings(int x, int y, int w, int h, const char* title, Note* pNote)
    : Fl_Window(x, y, w, h, title)
    , m_pNote(pNote)
{  
    SetupWindow();

    SetupTitleInput();
    SetupColorButtons();
    SetupCloseButton();

    end();
}

void CSettings::TitleInputCallback(Fl_Widget* w, void* userdata) {  
    auto* self = static_cast<CSettings*>(userdata);
    Note* note = self->GetNote();
    note->SetTitle(static_cast<Fl_Input*>(w)->value());
    note->SetTitleChanged(true);
    note->UpdateData();
}

void CSettings::ColorButtonCallback(Fl_Widget* w, void* userdata) {  
    auto* self = static_cast<CSettings*>(userdata);
    Note* note = self->GetNote();
    note->SetColor(w->color());
    note->SetColorChanged(true);
    note->UpdateData();
}

void CSettings::CloseButtonCallback(Fl_Widget* w, void* userdata) {  
    auto* self = static_cast<CSettings*>(userdata);
    self->hide();
}

void CSettings::SetupWindow() {  
    int new_w = (m_btnSz * 6) + (m_space * 2);
    int new_x = x() + (w() - new_w) / 2;
    resize(new_x, y() + m_space, new_w, h());
    set_modal();
}

void CSettings::SetupTitleInput() {  
    m_pInputTitle = new Fl_Input(m_space, m_space, w() - 2 * m_space, 30);
    m_pInputTitle->value(m_pNote->GetTitle().c_str());
    m_pInputTitle->callback(TitleInputCallback, this);
}

void CSettings::SetupColorButtons() {  
    const char* colors[] = {
        ColorUtil::BEIDGE, 
        ColorUtil::YELLOW, 
        ColorUtil::PINK, 
        ColorUtil::BLUE, 
        ColorUtil::GREEN, 
        ColorUtil::GREY 
    };

    int btn_y = m_pInputTitle->y() + m_pInputTitle->h() + m_space;

    for (int i = 0; i < 6; ++i) {
        int btn_x = m_space + (i * m_btnSz);
        m_pColorButtons[i] = new Fl_Button(btn_x, btn_y, m_btnSz, m_btnSz);
        m_pColorButtons[i]->color(ColorUtil::str2color(colors[i]));
        m_pColorButtons[i]->callback(ColorButtonCallback, this);
    }
}

void CSettings::SetupCloseButton() {  
    int btn_w = 60;
    int btn_h = 20;
    int btn_x = w() - btn_w - m_space;
    int btn_y = h() - btn_h - m_space;

    m_pCloseButton = new Fl_Button(btn_x, btn_y, btn_w, btn_h, "Close");
    m_pCloseButton->callback(CloseButtonCallback, this);
}

Note* CSettings::GetNote() const {   return m_pNote; }

void CSettings::Run() {  
    show();
    while (shown()) { 
        Fl::wait();
    }
}