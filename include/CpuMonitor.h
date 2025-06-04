// include/CpuMonitor.h
#ifndef CPUMONITOR_H
#define CPUMONITOR_H

struct CPU {
    short nbrCPU;
    float* usagePerCPU;
    float frequency;
    float frequencyMax;
};

class CpuMonitor {
private:
    CPU cpu;
public:
    bool update();
    float getCpuUsage();
    float getCpuFreq();
    float getCpuInfo();
};

#endif
