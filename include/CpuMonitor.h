#pragma once
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