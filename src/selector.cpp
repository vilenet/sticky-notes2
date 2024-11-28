#include "selector.h"
#include "dout.h"


NoteSelector::NoteSelector(int x, int y, int w, int h, const char* title, App* pApp)
    : Fl_Window(x, y, w, h, title)
    , m_pApp(pApp)
{
    browser = new Fl_Hold_Browser(10, 10, this->w() - 20, this->h() - 60);
    openBtn = new Fl_Button(10, this->h() - 40, this->w() - 20, 30, "Open");
    
    InitBrowser();

    openBtn->callback(openCallback, this);
    set_modal();
    end();
}

void NoteSelector::Run() {
    show();
    while (shown()) {
        Fl::wait();
    }
}

void NoteSelector::selectNote() {
    int selected_index = browser->value();

    if (selected_index > 0) {
        const char* selected_item = browser->text(selected_index);
        Data* pData = static_cast<Data*>(browser->data(selected_index));

        if (pData) {
            m_pApp->CreateNote(pData);
        }
    }

    hide();
}

void NoteSelector::InitBrowser() {
    if (!browser) return;

    std::unordered_map<int, Data*> Datas = m_pApp->GetDatas();

    if (Datas.empty()) return;

    for (const auto& pair : Datas) {
        Data* data = pair.second;
        if (data && !data->state) {
            browser->add(data->title.c_str(), static_cast<void*>(data));
        }
    }
}