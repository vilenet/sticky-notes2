#include "data.h"
#include "dout.h"
#include <sstream>


std::unordered_map<int, Data*> DataUtil::Str2Data(const std::string& dataStr) { 
    std::unordered_map<int, Data*> Datas;
    std::istringstream iss(dataStr);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.rfind(tag_begin, 0) == 0) {
            Data* data = new Data();
            std::istringstream ss(line.substr(tag_begin.size()));

            char comma; 
            ss >> data->id >> comma >> data->state >> comma 
               >> data->x >> comma >> data->y >> comma 
               >> data->w >> comma >> data->h >> comma;

            data->str_color.resize(6);
            ss.read(&data->str_color[0], 6);

            ss >> comma;

            std::getline(ss, data->title, '>');

            std::string text;
            while (std::getline(iss, line) && line != tag_end) { text += line + '\n'; }
            if (!text.empty() && text.back() == '\n') text.pop_back();
            data->text = text;

            Datas[data->id] = data;
        }
    }

    return Datas;
}

std::string DataUtil::Data2Str(const std::unordered_map<int, Data*>& datas) { 
    std::ostringstream oss;
    for (const auto& pair : datas) {
        const Data* data = pair.second;
        oss << tag_begin << data->id << "," << data->state << "," << data->x << "," << data->y << ","
            << data->w << "," << data->h << "," << data->str_color << "," << data->title << ">" << "\n"
            << data->text << "\n" << tag_end << "\n";
    }
    return oss.str();
}
