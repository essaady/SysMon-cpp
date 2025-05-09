#include "utils/FileUtils.h"
#include <iostream>
#include <fstream>
#include <vector>

void createTestFile() {
    std::ofstream out("test.txt");
    out << "Bonjour\n";
    out << "Ceci est un test\n";
    out << "Fin de fichier\n";
}

int main() {
    createTestFile();
    
    std::cout << "Démarrage du programme !" << std::endl;

    try {
        std::vector<std::string> lignes = FileUtils::readFileLines("test.txt");
        std::cout << "Contenu lu depuis test.txt :" << std::endl;

        for (const auto& ligne : lignes) {
            std::cout << ligne << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }

    return 0;
}
