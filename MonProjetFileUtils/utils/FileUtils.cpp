#include "FileUtils.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace FileUtils {

    std::vector<std::string> readFileLines(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + path);
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        if (lines.empty()) {
            std::cerr << "Warning: The file '" << path << "' is empty.\n";
        }

        return lines;
    }

}
