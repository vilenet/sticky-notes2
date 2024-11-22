#include "selector.h"
#include "dout.h"


NoteSelector::NoteSelector(const std::unordered_map<int, Data*>& datas) { 
    dialog = new Fl_Window(400, 300, "Открыть заметку");
    browser = new Fl_Select_Browser(10, 10, 380, 250);
    openBtn = new Fl_Button(150, 270, 100, 25, "Открыть");
    
    for (const auto& pair : datas) {
        Data* data = pair.second;
        if (!data->state) {
            browser->add(data->title.c_str(), (void*)data);
        }
    }

    openBtn->callback(openCallback, this);
    dialog->set_modal();
    dialog->end();
}

NoteSelector::~NoteSelector() { 
    delete dialog;
}

Data* NoteSelector::select() { 
    dialog->show();
    while (dialog->shown()) { Fl::wait(); }
    return selectedData;
}

void NoteSelector::selectNote() { 
    int selected = browser->value();
    if (selected > 0) { selectedData = static_cast<Data*>(browser->data(selected)); }
    dialog->hide();
}
