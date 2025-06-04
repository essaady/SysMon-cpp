#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>

class CpuMonitor {
public:
    CpuMonitor();
    float getCpuUsage();
private:

    struct CpuData {
        float frequency;
        float frequencyMax;
        float usageCPU;
        float* usagePerCPU;
        short nbrCPU;
        bool update();

    };
    
    CpuData cpuData;
    std::string rawCPU;

};

#endif 