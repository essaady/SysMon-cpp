#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>
#include <vector>

struct CPU {
    float usagePerCPU;
    float frequency;
    float frequencyMax;
};

class CpuMonitor {
public:
    CpuMonitor();

    bool update();             // Met à jour les stats CPU
    float getCpuUsage();       // Renvoie le pourcentage d'utilisation globale
    float getCpuFreq();        // Renvoie la fréquence actuelle
    std::string getCpuInfo();  // Renvoie une chaîne d'information détaillée

private:
    short nbrCPU;
    std::vector<CPU> cpus;
    std::vector<unsigned long long> prevStats;
    bool firstRead;

    bool readCpuStats(std::vector<unsigned long long>& stats);
    float getFrequency();
    float getMaxFrequency();
};

#endif // CPUMONITOR_H
