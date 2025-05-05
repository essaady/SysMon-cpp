
#include "FileUtils.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

namespace utils {

    vector<string> lire_lignes(const string& p) {
        vector<string> lignes;
        ifstream fichier(p);// Ouvre le fichier
        if (!fichier.is_open()) return lignes;

        string ligne;
        while (getline(fichier, ligne)) {
            lignes.push_back(ligne);// ajoute chaque ligne lue au vecteur 'lignes'
        }

        return lignes;
    }

}


