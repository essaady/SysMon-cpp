#ifndef SYSMON_H
#define SYSMON_H

#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"
#include <string>

class SysMon
{
private:
    int updateInterval;       // Fréquence de mise à jour des données (en secondes)
    std::string exportFormat; // Format d'exportation (CSV, TXT, JSON...)
    bool help;                // Indicateur pour afficher l'aide

public:
    // Constructeur
    SysMon(int interval = 1, std::string format = "txt", bool showHelp = false);
    int run(); // Plus de paramètre limit
    // Exportation des données sous différents formats
    std::string exportAsText();
    std::string exportAsCSV();

    // Mise à jour des statistiques système
    bool update(CpuMonitor &cpuMon, MemoryMonitor &memMon, ProcessMonitor &procMon);

    // Obtenir l'heure actuelle du système
    std::string getTime();

    // Obtenir des informations générales sur le système
    std::string getInfo();
};

#endif // SYSMON_H
