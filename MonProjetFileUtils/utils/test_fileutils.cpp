#include "utils/FileUtils.h"
#include <cassert>
#include <fstream>

void testReadFileLines() {
    // Setup - créer un fichier test
    std::ofstream("testfile.txt") << "line1\nline2\nline3";
    
    // Test lecture normale
    auto lines = FileUtils::readFileLines("testfile.txt");
    assert(lines.size() == 3);
    assert(lines[0] == "line1");
    
    // Test fichier vide
    std::ofstream("empty.txt").close();
    try {
        FileUtils::readFileLines("empty.txt");
        assert(false); // Ne devrait pas arriver ici
    } catch (const std::exception&) {}
    
    // Nettoyage
    remove("testfile.txt");
    remove("empty.txt");
}

int main() {
    testReadFileLines();
    return 0;
}