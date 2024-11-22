#include "app.h"
#include "selector.h"
#include "dout.h"


App::App() { 
    UtilFile.Init();
}

void App::Run() { 
    LoadData();
    BuildNotes();
    Fl::run();
}

void App::LoadData() { 
    std::string str = UtilFile.Read();
    Datas = UtilData.Str2Data(str);

    for (const auto& pair : Datas) {
        Data* data = pair.second;
        if (data->id >= nextID) nextID = data->id + 1;
    }
}

void App::BuildNotes() { 
    for (const auto& pair : Datas) { CreateNote(pair.second); }
    if (Datas.empty()) { CreateNote(nullptr); }
}

void App::OpenNote() { 
    NoteSelector selector(Datas);
    Data* selectedData = selector.select();
    if (selectedData) {
        CreateNote(selectedData);
    }
}

Data* App::CreateData() { 
    Data* data = new Data();
    data->id = nextID++;
    Datas[data->id] = data;
    return data;
}

void App::CreateNote(Data* data, int x, int y) { 
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

void App::UpdateData(Data* data) { 
    Datas[data->id] = data;
    UpdateFile();
}

void App::UpdateFile() { 
    std::string strData = UtilData.Data2Str(Datas);
    UtilFile.Write(strData);
}

void App::DeleteNote(int id, Note* note) { 
    if (Notes[id] == note) {
        delete note;
        Notes[id] = nullptr;
    }
}

// Deletes: Note from Notes, Data from Datas, record from file
void App::DeleteNoteData(int id) { 
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
