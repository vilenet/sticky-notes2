#include "app.h"
#include "dout.h"
#include "database.h"


App::App() { 
    UtilFile.Init();

    m_pDatabase = new CDatabase("data/data.db");
    if (!m_pDatabase->Open()) {
        std::cerr << "Error opening database." << std::endl;
        //TODO: Handle errors or terminate the application
    }
    if (!m_pDatabase->CreateTable()) {
        std::cerr << "Error creating table." << std::endl;
        //TODO: Handle errors or terminate the application
    }
}

void App::Run() { 
    LoadData();
    BuildNotes();
    Fl::run();
}

void App::LoadData() { 
    Datas = m_pDatabase->GetAllData();

    for (const auto& pair : Datas) {
        Data* data = pair.second;
        if (data->id >= nextID) nextID = data->id + 1;
    }
}

void App::BuildNotes() { 
    for (const auto& pair : Datas) { CreateNote(pair.second); }
    if (Datas.empty()) { CreateNote(nullptr); }
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
        data->state = true;

        if (!m_pDatabase->InsertData(*data)) {
            std::cerr << "Error inserting data." << std::endl;
            //TODO: Error handling
        }
    } 
    else {
        data->state = true;
    }
    Note* note = new Note(data, this);
    Notes[data->id] = note;
}

void App::UpdateData(Data* data) {
    Datas[data->id] = data;

    if (!m_pDatabase->UpdateData(*data)) {
        std::cerr << "Error updating data." << std::endl;
        //TODO: Error handling
    }
}

void App::DeleteNote(int id, Note* note) { 
    if (Notes[id] == note) {
        delete note;
        Notes.erase(id);
    }
}

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

    if (!m_pDatabase->DeleteData(id)) {
        std::cerr << "Error deleting data." << std::endl;
        //TODO: Error handling
    }
}

std::unordered_map<int, Data*> App::GetDatas() { return Datas; }