#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>
using namespace std;

class CpuMonitor {
public:
    struct CPU {
        short nbrCPU;
        float usage;
    };

private:
    CPU cpu;
    unsigned long long lastIdle, lastTotal;

public:
    CpuMonitor();
    bool update();
    float getCpuUsage();
    float getCpuFreq(); // Placeholder
    float getCpuInfo(); // Placeholder
};

#endif
