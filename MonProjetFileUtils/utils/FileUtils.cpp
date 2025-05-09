#include "FileUtils.h"
#include <fstream>
#include <stdexcept>

namespace FileUtils {

std::vector<std::string> readFileLines(const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream file(path);
    
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path + " (file may not exist or permissions denied)");
    }
    
    if (file.peek() == std::ifstream::traits_type::eof()) {
        throw std::runtime_error("File is empty: " + path);
    }

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

} // namespace FileUtils
