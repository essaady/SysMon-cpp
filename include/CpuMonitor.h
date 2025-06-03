#ifndef CPUMONITOR_H
#define CPUMONITOR_H
#include <string>
#include <thread>


class CpuMonitor {
    private:
        struct CPU{
            short nbrCPU;
            float* usagePerCPU;
            float frequency;
            float frequencyMax;
        }cpu;
        unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;
    public:
        CpuMonitor();
        ~CpuMonitor();

        bool update();
        float getCpuUsage();
        float getCpuFreq();
        std::string getCpuInfo();

};
#endif