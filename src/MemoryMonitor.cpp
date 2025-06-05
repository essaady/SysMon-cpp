#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class MemoryMonitor {
public:
void afficherUtilisationDisque(const std::string& chemin = "/");

    void afficherUtilisationMemoire();
private:
    long extraireValeur(const std::string& ligne);
};

long MemoryMonitor::extraireValeur(const std::string& ligne) {
    std::istringstream iss(ligne);
    std::string label;
    long valeur;
    std::string unite;
    iss >> label >> valeur >> unite;
    return valeur;
}

void MemoryMonitor::afficherUtilisationMemoire() {
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo) {
        std::cerr << "Erreur lors de l'ouverture de /proc/meminfo" << std::endl;
        return;
    }

    std::string ligne;
    long total = 0, libre = 0;

    while (std::getline(meminfo, ligne)) {
        if (ligne.find("MemTotal:") == 0) {
            total = extraireValeur(ligne);
        } else if (ligne.find("MemAvailable:") == 0) {
            libre = extraireValeur(ligne);
            break;
        }
    }

    long utilisee = total - libre;
    std::cout << "Mémoire utilisée : " << utilisee / 1024 << " Mo / "
              << total / 1024 << " Mo" << std::endl;
    std::cout << "Mémoire libre : " << libre / 1024 << " Mo" << std::endl;
}
#include "MemoryMonitor.h"
#include <sys/statvfs.h>

// Méthode update() — موجودة عندك فـ .h، خاصك تحطها هنا
bool MemoryMonitor::update() {
    ifstream memFile("/proc/meminfo");
    if (!memFile.is_open()) return false;

    string line;
    ram.memInfo.clear();

    while (getline(memFile, line)) {
        ram.memInfo.push_back(line);
    }
    memFile.close();

    for (const auto& entry : ram.memInfo) {
        istringstream iss(entry);
        string key;
        size_t value;
        string unit;

        iss >> key >> value >> unit;

        if (key == "MemTotal:") totalMemMb = value / 1024;
        else if (key == "MemFree:") freeMem = value / 1024.0;
        else if (key == "SwapTotal:") SwapMemMb = value / 1024;
        else if (key == "SwapFree:") freeSwap = value / 1024.0;
    }

    return true;
}

// Les getters
unsigned long long MemoryMonitor::getTotalMemory() const { return totalMemMb; }
unsigned long long MemoryMonitor::getFreeMemory() const { return static_cast<unsigned long long>(freeMem); }
unsigned long long MemoryMonitor::getTotalSwap() const { return SwapMemMb; }
unsigned long long MemoryMonitor::getUsedSwap() const { return static_cast<unsigned long long>(SwapMemMb - freeSwap); }
double MemoryMonitor::getMemoryUsagePercentage() const { return ((totalMemMb - freeMem) / totalMemMb) * 100.0; }
double MemoryMonitor::getSwapUsagePercentage() const {
    if (SwapMemMb == 0) return 0.0;
    return ((SwapMemMb - freeSwap) / SwapMemMb) * 100.0;
}

// Méthode à ajouter : afficher utilisation disque
void MemoryMonitor::afficherUtilisationDisque(const std::string& chemin) {
    struct statvfs stats;

    if (statvfs(chemin.c_str(), &stats) != 0) {
        cerr << "Erreur lors de la récupération des informations du disque." << endl;
        return;
    }

    unsigned long taille_totale = stats.f_blocks * stats.f_frsize;
    unsigned long taille_disponible = stats.f_bavail * stats.f_frsize;
    unsigned long taille_utilisee = taille_totale - taille_disponible;
    double pourcentage_utilisation = (double)taille_utilisee / taille_totale * 100;

    cout << "=== Utilisation du disque pour: " << chemin << " ===" << endl;
    cout << "Taille totale       : " << taille_totale / (1024 * 1024) << " MB" << endl;
    cout << "Espace utilisé      : " << taille_utilisee / (1024 * 1024) << " MB" << endl;
    cout << "Espace disponible   : " << taille_disponible / (1024 * 1024) << " MB" << endl;
    cout << "Pourcentage utilisé : " << pourcentage_utilisation << " %" << endl;
}


