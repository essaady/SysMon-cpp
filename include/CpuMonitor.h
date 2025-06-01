#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>
#include <vector>

/**
 * Classe pour surveiller l'utilisation du CPU
 * Lit les informations depuis /proc/stat
 */
class CpuMonitor {
private:
    // Variables pour stocker les valeurs précédentes (calcul du pourcentage)
    long long prevTotal;
    long long prevIdle;
    
    // Méthodes privées
    std::vector<long long> readCpuStats();
    double calculateCpuUsage(const std::vector<long long>& stats);

public:
    // Constructeur
    CpuMonitor();
    
    // Méthodes publiques
    double getCurrentCpuUsage();        // Retourne le % d'utilisation CPU global
    std::string getCpuInfo();           // Retourne des infos sur le CPU
    void displayCpuUsage();             // Affiche l'utilisation CPU
    
    // Destructeur
    ~CpuMonitor();
};

#endif // CPUMONITOR_H