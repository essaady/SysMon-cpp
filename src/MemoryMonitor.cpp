#include "MemoryMonitor.h"
#include <fstream>
#include <iostream>
#include <sstream>
//Hamza Tahiri
void MemoryMonitor::afficherUtilisationMemoire() {
    std::ifstream fichier("/proc/meminfo");
    if (!fichier) {
        std::cerr << "Erreur : impossible d'ouvrir /proc/meminfo" << std::endl;
        return;
    }

    std::string ligne;
    unsigned long memTotal = 0, memLibre = 0, buffers = 0, cached = 0;

    while (std::getline(fichier, ligne)) {
        std::istringstream iss(ligne);
        std::string cle;
        unsigned long valeur;
        std::string unite;

        iss >> cle >> valeur >> unite;

        if (cle == "MemTotal:") memTotal = valeur;
        else if (cle == "MemFree:") memLibre = valeur;
        else if (cle == "Buffers:") buffers = valeur;
        else if (cle == "Cached:") cached = valeur;

        if (memTotal && memLibre && buffers && cached)
            break;
    }

    unsigned long memUtilisee = memTotal - memLibre - buffers - cached;
    double pourcentage = (double)memUtilisee / memTotal * 100;

    std::cout << "Mémoire utilisée : " << memUtilisee / 1024 << " MB / "
              << memTotal / 1024 << " MB (" << pourcentage << "%)" << std::endl;
}
