#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>

using namespace std;

class CpuMonitor {
protected:
    struct CPU{
        short nbrCPU;
        float *usagePerCPU;
        float usageCPU;
        float frequency;
        float frequencyMax;
    }CPU;

public:
    CpuMonitor();
    
    bool update();
    float getCpuNbr();
    float getCpuUsage();
    float getCpuFreq();
    float getCpuFreqMax();
    string getCpuInfo();

    ~CpuMonitor();
};

#endif
