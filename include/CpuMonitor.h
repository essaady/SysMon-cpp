#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <vector>
#include <string>

struct CPU
{
    short nbrCPU;
    float usagePerCPU;
    float frequency;
    float frequencyMax;
};

class CpuMonitor
{
private:
    CPU cpu;

public:
    bool update();
    float getCpuUsage();
    float getCpuFreq();
    std::string getCpuInfo();
};

#endif
