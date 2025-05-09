#include <iostream>
#include "FileUtils.h"

int main() {
    const std::string path = "test.txt";

    try {
        std::vector<std::string> lines = FileUtils::readFileLines(path);

        std::cout << "File content:\n";
        for (const std::string& line : lines) {
            std::cout << line << '\n';
        }

        // Optional cleanup after test
        if (std::remove(path.c_str()) == 0) {
            std::cout << "Test file '" << path << "' deleted successfully.\n";
        } else {
            std::cerr << "Warning: Could not delete test file '" << path << "'.\n";
        }

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    }

    return 0;
}
