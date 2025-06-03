#ifndef CPUMONITOR_H
#define CPUMONITOR_H

class CpuMonitor {
    struct CPU {
        short nbrCPU;
        float* usagePerCPU;
        float frequency;
        float frequencyMax;
    } CPU;

public:
    bool update();
    float getCpuUsage();
    float getCpuFreq();
    float getCpuInfo();
};

#endif
