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
    if (selectedData) { CreateNote(selectedData); }
}

Data* App::CreateData() { dbgout("");
    Data* data = new Data();
    data->id = nextID++;
    Datas[data->id] = data;
    return data;
}

void App::CreateNote(Data* data) { dbgout("");
    if (!data) { data = CreateData(); }
    data->state = true;
    auto it = Notes.find(data->id);
    if (it != Notes.end()) { it->second->show(); }
    else {
        Note* note = new Note(data, this);
        Notes[data->id] = note;
    }

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

// Deletes: Note from Notes, Data from Datas, record from file
void App::DeleteNote(int id) { dbgout("");
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


// Проверить логику этого метода - 
// void App::CreateNote(Data* data) {
//     if (!data) { data = CreateData(); }
//     data->state = true;
//     auto it = Notes.find(data->id);

//     if (it == Notes.end()) {
//         Note* note = new Note(data, this);
//         Notes[data->id] = note;
//     }

//     UpdateFile();
// }

// void App::LoadData() { dbgout("");
//     std::string str = UtilFile.Read();
//     Datas = UtilData.Str2Data(str);
// }

// void App::BuildNotes() { dbgout("");
//     for (const auto& pair : Datas) {
//         Data* data = pair.second;
//         Note* note = new Note(data, this);
//         Notes[data->id] = note;
//     }
// }

// void App::CreateNote() { dbgout("");
//     Data* data = new Data();
//     data->id = nextID++;
//     Datas[data->id] = data;
//     Note* note = new Note(data, this);
//     Notes[data->id] = note;
// }

// void App::UpdateData(Data* data) { dbgout("");
//     Datas[data->id] = data;
//     UtilFile.Write(UtilData.Data2Str(Datas));
// }
