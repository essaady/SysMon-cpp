#ifndef CPUMONITOR_H
#define CPUMONITOR_H

class CpuMonitor {
    private:  
        struct CPU {
            short nbrCPU;
            float* usagePerCPU;
            float frequency;
            float usageCPU;
            float frequencyMax;
        };
        CPU cpu;
    public:
        CpuMonitor();
        ~CpuMonitor();
        bool update(int);
        float getCpuUsage();
        float getCpuFreq();
        std::string getCpuInfo();
};

#endif 
