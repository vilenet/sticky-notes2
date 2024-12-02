#ifndef DATABASE_H
#define DATABASE_H

#include "data.h"
#include "sqlite3.h"

#include <string>
#include <unordered_map>

class CDatabase {
private:
    sqlite3* m_db;
    std::string m_dbpath;

public:
    CDatabase(const std::string& dbpath);
    ~CDatabase();

    bool Open();
    void Close();

    bool CreateTable();
    bool InsertData(const Data& data);
    bool UpdateData(const Data& data);
    bool DeleteData(int id);
    std::unordered_map<int, Data*> GetAllData();
};

#endif // DATABASE_H