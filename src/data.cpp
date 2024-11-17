#include "data.h"
#include "dout.h"
#include <sstream>


std::unordered_map<int, Data*> DataUtil::Str2Data(const std::string& dataStr) { dbgout("");
    std::unordered_map<int, Data*> Datas;
    std::istringstream iss(dataStr);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.rfind(tag_begin, 0) == 0) { // Проверяем, что строка начинается с tag_begin = "<NOTE_BEGIN:"
            Data* data = new Data();
            std::istringstream ss(line.substr(tag_begin.size()));

            char comma; 
            ss >> data->id >> comma >> data->state >> comma 
               >> data->x >> comma >> data->y >> comma 
               >> data->w >> comma >> data->h >> comma;

            // Чтение цвета (6 символов)
            data->str_color.resize(6);
            ss.read(&data->str_color[0], 6);

            // Проверка на наличие символа '>'
            char end_char;
            ss >> end_char;
            if (end_char != '>') {
                std::cerr << "[ERROR] Invalid format: missing '>' after color\n";
                delete data;
                continue;
            }

            std::string text;
            while (std::getline(iss, line) && line != tag_end) { text += line + '\n'; }
            if (!text.empty() && text.back() == '\n') text.pop_back();
            data->text = text;

            Datas[data->id] = data;
        }
    }

    return Datas;
}

// std::unordered_map<int, Data*> DataUtil::Str2Data(const std::string& dataStr) { dbgout("");
//     std::unordered_map<int, Data*> datas;
//     std::istringstream iss(dataStr);
//     std::string line;

//     while (std::getline(iss, line)) {
//         if (line.find(tag_begin) == 0) {
//             Data* data = new Data();
//             std::istringstream ss(line.substr(tag_begin.size()));
//             char comma;
//             ss >> data->id >> comma >> data->state >> comma >> data->x >> comma >> data->y >> comma
//                >> data->w >> comma >> data->h >> comma >> data->str_color;

//             std::string text;
//             while (std::getline(iss, line) && line != tag_end) {
//                 text += line + '\n';
//             }
//             if (!text.empty() && text.back() == '\n') text.pop_back();
//             data->text = text;
//             datas[data->id] = data;
//         }
//     }
//     return datas;
// }

std::string DataUtil::Data2Str(const std::unordered_map<int, Data*>& datas) { dbgout("");
    std::ostringstream oss;
    for (const auto& pair : datas) {
        const Data* data = pair.second;
        oss << tag_begin << data->id << "," << data->state << "," << data->x << "," << data->y << ","
            << data->w << "," << data->h << "," << data->str_color << ">\n"
            << data->text << "\n" << tag_end << "\n";
    }
    return oss.str();
}
