#ifndef CPUMON_H
#define CPUMON_H

#include <string>

class CpuMonitor {
    private:
    struct cpu {
    float frequency;
    float frequencyMax;
    float usageCPU;
    float* usageperCPU;
    short nbrCPU;
    };
    cpu cp;
    public:
    CpuMonitor();
    ~CpuMonitor();
    bool update();
    float getcpuusage();
    float getcpufrequency();
    std::string getcpuinfo();
};

#endif