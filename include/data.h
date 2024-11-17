#ifndef DATA_H
#define DATA_H

#include <string>
#include <unordered_map>


struct Data {
    int id, x, y, w, h;
    std::string title, text, str_color;
    bool state;

    Data() : id(1), x(100), y(100), w(300), h(300), state(true), title("New Note"), str_color("FFF8D1"), text("") {}
};

class DataUtil {
private:
    const std::string tag_begin = "<NOTE_BEGIN:";
    const std::string tag_end = "<NOTE_END>";

public:
    std::unordered_map<int, Data*> Str2Data(const std::string& dataStr);
    std::string Data2Str(const std::unordered_map<int, Data*>& datas);
};

#endif // DATA_H
