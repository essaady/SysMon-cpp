
#ifndef CPUMONITOR_H
#define CPUMONITOR_H

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

    public:

        bool update();
        float getCpuUsage();
        float getCpuFreq();
        std::string getCpuInfo();


};

#endif 
