#include "database.h"
#include <iostream>


CDatabase::CDatabase(const std::string& dbpath)
    : m_db(nullptr)
    , m_dbpath(dbpath) 
{}

CDatabase::~CDatabase() {
    Close();
}

bool CDatabase::Open() {
    if (sqlite3_open(m_dbpath.c_str(), &m_db) != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }
    return true;
}

void CDatabase::Close() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool CDatabase::CreateTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS data ("
                      "id INTEGER PRIMARY KEY,"
                      "state INTEGER,"
                      "x INTEGER,"
                      "y INTEGER,"
                      "w INTEGER,"
                      "h INTEGER,"
                      "str_color TEXT,"
                      "title TEXT,"
                      "text TEXT"
                      ");";

    char* errMsg = nullptr;    
    if (sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool CDatabase::InsertData(const Data& data) {
    const char* sql = "INSERT INTO data (id, state, x, y, w, h, str_color, title, text) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing INSERT query: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, data.id);
    sqlite3_bind_int(stmt, 2, data.state);
    sqlite3_bind_int(stmt, 3, data.x);
    sqlite3_bind_int(stmt, 4, data.y);
    sqlite3_bind_int(stmt, 5, data.w);
    sqlite3_bind_int(stmt, 6, data.h);
    sqlite3_bind_text(stmt, 7, data.str_color.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, data.title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9, data.text.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "INSERT query failed: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool CDatabase::UpdateData(const Data& data) {
    const char* sql = "UPDATE data SET state = ?, x = ?, y = ?, w = ?, h = ?, str_color = ?, title = ?, text = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing UPDATE request: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, data.state);
    sqlite3_bind_int(stmt, 2, data.x);
    sqlite3_bind_int(stmt, 3, data.y);
    sqlite3_bind_int(stmt, 4, data.w);
    sqlite3_bind_int(stmt, 5, data.h);
    sqlite3_bind_text(stmt, 6, data.str_color.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, data.title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, data.text.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 9, data.id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "UPDATE request failed: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool CDatabase::DeleteData(int id) {
    const char* sql = "DELETE FROM data WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing DELETE request: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "DELETE request failed: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

std::unordered_map<int, Data*> CDatabase::GetAllData() {
    std::unordered_map<int, Data*> datas;
    const char* sql = "SELECT * FROM data;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing SELECT query: " << sqlite3_errmsg(m_db) << std::endl;
        return datas;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Data* data = new Data();
        data->id = sqlite3_column_int(stmt, 0);
        data->state = sqlite3_column_int(stmt, 1);
        data->x = sqlite3_column_int(stmt, 2);
        data->y = sqlite3_column_int(stmt, 3);
        data->w = sqlite3_column_int(stmt, 4);
        data->h = sqlite3_column_int(stmt, 5);
        data->str_color = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        data->title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        data->text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));

        datas[data->id] = data;
    }

    sqlite3_finalize(stmt);
    return datas;
}
