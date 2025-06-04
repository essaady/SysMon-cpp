#include "utils/FileUtils.h"
#include <fstream>
#include <string>
#include <vector>

namespace FileUtils {

std::vector<std::string> readFileLines(const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream file(path);
    std::string line;

    if (!file.is_open()) {
        return lines; 
    }

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

}
