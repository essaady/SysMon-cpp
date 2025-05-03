#include "utils/FileUtils.h"
#include <iostream>
#include <fstream>
#include <vector> 

using namespace std;

void createTestFile() {
    ofstream out("test.txt");
    out << "Bonjour\n";
    out << "Ceci est un test\n";
    out << "Fin de fichier\n";
}

int main() {
    createTestFile();
    
        std::cout << "Démarrage du programme !" << std::endl;

    try {
        vector<string> lignes = FileUtils::readFileLines("test.txt");
        cout << "Contenu lu depuis test.txt :" << endl;

        for (const auto& ligne : lignes) {
            cout << ligne << endl;
        }
    } catch (const exception& e) {
        cerr << "Erreur : " << e.what() << endl;
    }

    return 0;
}
