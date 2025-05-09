#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>

namespace FileUtils {
    std::vector<std::string> readFileLines(const std::string& path);
}

#endif
