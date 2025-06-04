
#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>
#include <vector>
#include <cstdint>

class CpuMonitor {
private:
    struct CPU {
        float frequency;
        float frequencyMax;
        float usageCPU;
        std::vector<float> usagePerCPU;
        short nbrCPU;
    } cpu;

    std::string rawCPU;

    uint64_t getSnap(const std::string& calc);
    std::string getInfo(const std::string& path);

public:
    CpuMonitor();

    bool update();
    float getCpuUsage() const;
    float getCpuFreq() const;
    std::string getCpuInfo() const;
    float calcCpuUsage(int logger = 0);
};

#endif
=======
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


