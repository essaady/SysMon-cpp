#pragma once
#include <string>

class ExportAsText {
public:
    ExportAsText() = default;
    ~ExportAsText() = default;

    // Exports the given content to the specified file path.
    bool exportToFile(const std::string& filePath, const std::string& content);
};