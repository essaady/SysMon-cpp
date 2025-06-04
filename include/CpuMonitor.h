#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>
using namespace std;

class CpuMonitor {
public:
    struct CPU {
        short nbrCPU;
        float* usagePerCPU;
        float frequency;
        float frequencyMax;
    };

private:
    CPU cpu;

public:
    CpuMonitor();
    ~CpuMonitor();
    
    bool update();
    float getCpuUsage();
    float getCpuFreq();
    float getCpuInfo(); // peut être renommée en string si on veut retourner du texte
};

#endif
