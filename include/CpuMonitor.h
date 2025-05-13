#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>
#include <vector>

// Classe responsable de lire les données CPU depuis /proc/stat et de calculer l'utilisation.
class CPUMonitor {
private:
    // Valeurs précédentes des temps CPU pour faire un delta
    unsigned long long prevUser, prevNice, prevSystem, prevIdle;

public:
    // Constructeur pour initialiser les valeurs
    CPUMonitor();

    // Méthode pour lire /proc/stat et stocker les temps CPU actuels
    void readCPUStats();

    // Méthode pour calculer et retourner le pourcentage d'utilisation CPU
    float calculateUsage();

    // (Optionnel) Méthode pour retourner une ligne formatée (ex: "CPU Usage: 35.2%")
    std::string getFormattedUsage();
};

#endif // CPUMONITOR_H
