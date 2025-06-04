#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct cpu {
    float frequency;
    float frequencyMax;
    float usageCPU;
    float* usagePerCPU;
    short nbrCPU;
};

class CpuMonitor {
private:
    cpu CPU;
    std::string rawCPU;

public:
    CpuMonitor();
    ~CpuMonitor();
    
    // Core methods
    bool update();
    float getCpuUsage();
    float getCpuFreq();
    std::string getCpuInfo();
    
    // Getters
    cpu getCpuData() const { return CPU; }
    std::string getRawCpuData() const { return rawCPU; }
    short getCpuCount() const { return CPU.nbrCPU; }
    float getCpuUsagePerCore(int core) const;
};

#endif 