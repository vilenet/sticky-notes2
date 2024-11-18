#include "app.h"
#include "selector.h"
#include "dout.h"


App::App() { dbgout("");
    UtilFile.Init();
}

void App::Run() { dbgout("");
    LoadData();
    BuildNotes();
    Fl::run();
}

void App::LoadData() { dbgout("");
    std::string str = UtilFile.Read();
    Datas = UtilData.Str2Data(str);

    for (const auto& pair : Datas) {
        Data* data = pair.second;
        if (data->id >= nextID) nextID = data->id + 1;
    }
}

void App::BuildNotes() { dbgout("");
    for (const auto& pair : Datas) { CreateNote(pair.second); }
    if (Datas.empty()) { CreateNote(nullptr); }
}

void App::OpenNote() { dbgout("");
    NoteSelector selector(Datas);
    Data* selectedData = selector.select();
    if (selectedData) {
        CreateNote(selectedData);
    }
}


// void App::OpenNote() {
//     NoteSelector selector(Datas);
//     Data* selectedData = selector.select();
//     if (selectedData) {
//         auto it = Notes.find(selectedData->id);
//         if (it != Notes.end()) {
//             Note* note = it->second;
//             note->setState(true);
//             note->show();
//             note->setIcon("menu_icon.ico");
//             //note->initMenu();
//         } else {
//             CreateNote(selectedData);
//         }
//     }
// }

// Icon work, Menu not work
// void App::OpenNote() {
//     NoteSelector selector(Datas);
//     Data* selectedData = selector.select();
//     if (selectedData) {
//         auto it = Notes.find(selectedData->id);
//         if (it != Notes.end()) {
//             Note* note = it->second;
//             note->setState(true);
//             note->show();
//             note->InitMenu();
//             note->setWinProc();
//         } else {
//             CreateNote(selectedData);
//         }
//     }
// }

// void App::OpenNote() { dbgout("");
//     NoteSelector selector(Datas);
//     Data* selectedData = selector.select();
//     if (selectedData) { CreateNote(selectedData); }
// }

Data* App::CreateData() { dbgout("");
    Data* data = new Data();
    data->id = nextID++;
    Datas[data->id] = data;
    return data;
}

// void App::CreateNote(Data* data, int x, int y) { dbgout("");
//     if (!data) { 
//         data = CreateData(); 
//         data->x = x;
//         data->y = y;
//     }
//     data->state = true;
//     auto it = Notes.find(data->id);
//     if (it != Notes.end()) { it->second->show(); }
//     else {
//         Note* note = new Note(data, this);
//         Notes[data->id] = note;
//     }

//     UpdateFile();
// }

void App::CreateNote(Data* data, int x, int y) { dbgout("");
    if (!data) { 
        data = CreateData(); 
        data->x = x;
        data->y = y;
    }
    data->state = true;
    Note* note = new Note(data, this);
    Notes[data->id] = note;

    UpdateFile();
}

void App::UpdateData(Data* data) { dbgout("");
    Datas[data->id] = data;
    UpdateFile();
}

void App::UpdateFile() { dbgout("");
    std::string strData = UtilData.Data2Str(Datas);
    UtilFile.Write(strData);
}

void App::DeleteNote(int id, Note* note) { dbgout("");
    if (Notes[id] == note) {
        delete note;
        Notes[id] = nullptr;
    }
}


// Deletes: Note from Notes, Data from Datas, record from file
void App::DeleteNoteData(int id) { dbgout("");
    auto noteIt = Notes.find(id);
    if (noteIt != Notes.end()) {
        Note* note = noteIt->second;
        note->hide();
        delete note;
        Notes.erase(noteIt);
    }

    auto dataIt = Datas.find(id);
    if (dataIt != Datas.end()) {
        delete dataIt->second;
        Datas.erase(dataIt);
    }

    UpdateFile();
}
