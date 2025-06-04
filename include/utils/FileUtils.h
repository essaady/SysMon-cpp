#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <vector>

namespace FileUtils {
    std::vector<std::string> readFileLines(const std::string& path);
}

#endif
