#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class MemoryMonitor {
public:
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

