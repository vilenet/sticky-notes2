#ifndef APP_H
#define APP_H

#include "data.h"
#include "file.h"
#include "note.h"

class Note;
//class Data;


class App {
private:
    std::unordered_map<int, Data*> Datas;
    std::unordered_map<int, Note*> Notes;

    FileUtil UtilFile;
    DataUtil UtilData;

    bool isAnyOn = false;
    int nextID = 1;

public:
    App();
    void Run();
    void LoadData();
    void LoadFile();
    void BuildNotes();
    Data* CreateData();
    void CreateNote(Data* data = nullptr);
    void UpdateData(Data* data);
    void UpdateFile();
    void OpenNote();
    //void DeleteData(int id);
    void DeleteNote(int id);
};

#endif // APP_H
