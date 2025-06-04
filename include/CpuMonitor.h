#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>
#include <vector>

struct CpuData {
    float usageCPU = 0.0f;
    float frequency = 0.0f;
    float frequencyMax = 0.0f;
    int nbrCPU = 0;
    std::vector<float> usagePerCPU;
};

class CpuMonitor {
private:
    CpuData CPU;
    std::string rawCPU;

#ifdef _WIN32
    int GetSystemInfoWrapper();
#else
    float getCurrentCpuFreq();
    float getMaxCpuFreq();
#endif

public:
    CpuMonitor() = default;
    ~CpuMonitor() = default;

    bool update();
    float getCpuUsage();
    float getCpuFreq();
    std::string getCpuInfo();
};

#endif
