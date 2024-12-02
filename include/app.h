#ifndef APP_H
#define APP_H

#include "data.h"
#include "file.h"
#include "note.h"
#include "database.h"

class Note;


class App {
private:
    std::unordered_map<int, Data*> Datas;
    std::unordered_map<int, Note*> Notes;

    FileUtil UtilFile;
    CDatabase* m_pDatabase;

    int nextID = 1;

public:
    App();
    void Run();
    void LoadData();
    void LoadFile();
    void BuildNotes();
    Data* CreateData();
    void CreateNote(Data* data = nullptr, int = 100, int = 100);
    void UpdateData(Data* data);
    void DeleteNote(int id, Note* pNote);
    void DeleteNoteData(int id);
    std::unordered_map<int, Data*> GetDatas();
};

#endif // APP_H
