#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>

class CpuMonitor {
public:
    struct CPU {
        float usagePerCPU;
        float frequency;
        float frequencyMax;
    };

private:
    short nbrCPU;
    CPU cpu;

public:
    CpuMonitor();
    bool update();
    float getCpuUsage();
    float getCpuFreq();
    std::string getCpuInfo();
};

#endif // CPUMONITOR_H

