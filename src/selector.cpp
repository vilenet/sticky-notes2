#include "selector.h"
#include "dout.h"


COpenNote::COpenNote(int x, int y, int w, int h, const char* title, App* pApp)
    : Fl_Window(x, y, w, h, title)
    , m_pApp(pApp)
{
    m_pBrowser = new Fl_Hold_Browser(10, 10, this->w() - 20, this->h() - 60);
    m_pOpenButton  = new Fl_Button(10, this->h() - 40, this->w() - 20, 30, "Open");
    
    InitBrowser();

    m_pOpenButton->callback(open_button_callback, this);
    set_modal();
    end();
}

void COpenNote::Run() {
    show();
    while (shown()) {
        Fl::wait();
    }
}

void COpenNote::open_button_callback(Fl_Widget* w, void* userdata) {
    COpenNote* self = static_cast<COpenNote*>(userdata);
    self->on_open_button_click();
}

void COpenNote::on_open_button_click() {
    int selected_index = m_pBrowser->value();

    if (selected_index > 0) {
        Data* pData = static_cast<Data*>(m_pBrowser->data(selected_index));

        if (pData) {
            m_pApp->CreateNote(pData);
        }
    }

    hide();
}

void COpenNote::InitBrowser() {
    if (!m_pBrowser) return;

    std::unordered_map<int, Data*> Datas = m_pApp->GetDatas();

    if (Datas.empty()) return;

    for (const auto& pair : Datas) {
        Data* data = pair.second;
        if (data && !data->state) {
            m_pBrowser->add(data->title.c_str(), static_cast<void*>(data));
        }
    }
}