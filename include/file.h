#ifndef FILE_H
#define FILE_H

#include <string>


class FileUtil {
private:
    const std::string filePath = "data/notes.txt";

public:
    void Init();
    std::string Read();
    void Write(const std::string& str);
};

#endif // FILE_H
