#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <vector>
#include <string>

class CpuMonitor {
public:
    struct CPU {
        unsigned long long user;
        unsigned long long nice;
        unsigned long long system;
        unsigned long long idle;
        unsigned long long iowait;
        unsigned long long irq;
        unsigned long long softirq;
        unsigned long long steal;

        float usage;  // Pourcentage d'utilisation pour ce cœur
    };

private:
    std::vector<CPU> cpus;     // Informations par cœur
    short nbrCPU;              // Nombre de cœurs
    float frequency;           // Fréquence actuelle
    float frequencyMax;        // Fréquence max

public:
    CpuMonitor();

    bool update();             // Met à jour toutes les infos
    float getCpuUsage();       // Moyenne globale
    float getCpuFreq();        // Fréquence actuelle
    std::string getCpuInfo();  // Infos détaillées
};

#endif
 

