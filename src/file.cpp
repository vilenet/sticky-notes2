#include "file.h"
#include "dout.h"

#include <filesystem>
#include <fstream>
#include <sstream>


void FileUtil::Init() { 
    std::filesystem::path dataDir("data");
    if (!std::filesystem::exists(dataDir)) {
        std::filesystem::create_directory(dataDir);
    }
}

std::string FileUtil::Read() { 
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) return "";
    std::ostringstream oss;
    oss << inFile.rdbuf();
    return oss.str();
}

void FileUtil::Write(const std::string& str) { 
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << str;
    }
}
