#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <vector>
#include <string>

class CpuMonitor {
private:
    struct cpu {
        float frequency;
        float frequencyMax;
        float usageCPU;
        float* usagePerCPU;
        short nbrCPU;
    } CPU;

    std::string rawCPU;
    std::vector<unsigned long long> prevIdleTimes;
    std::vector<unsigned long long> prevTotalTimes;

public:
    CpuMonitor();
    ~CpuMonitor();

    bool update();
    float getCpuUsage();
    float getCpuFreq();
    std::string getCpuInfo();
};

#endif 
