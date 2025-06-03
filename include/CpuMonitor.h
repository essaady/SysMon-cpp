#pragma once
#ifndef _CPUMONITOR_H
#define _CPUMONITOR_H
#include <string>

typedef struct cpu{
        float frequency;
        float frequencyMax;
        float usageCPU;
        float* usagePerCPU;
        short nbrCPU;
    } cpu;

class CpuMonitor {
protected:
    // Vous pourrez ajouter des variables privées ici plus tard pour stocker l'état du CPU
    cpu CPU;
    std::string rawCPU;

    public:
    // Constructeur
    CpuMonitor();
    
    // Destructeur
    ~CpuMonitor();

    // Méthode pour récupérer l'utilisation du CPU
    float getCpuUsage();
    
    float getCpuFreq();

    std::string getCpuInfo();

    bool update();

};

#endif // CPUMONITOR_H

