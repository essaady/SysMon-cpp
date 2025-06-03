#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>
#include <vector>
#include <cstdint>

class CpuMonitor {
private:
    struct CPU {
        float frequency;
        float frequencyMax;
        float usageCPU;
        std::vector<float> usagePerCPU;
        short nbrCPU;
    } cpu;

    std::string rawCPU;

    uint64_t getSnap(const std::string& calc);
    std::string getInfo(const std::string& path);

public:
    CpuMonitor();

    bool update();
    float getCpuUsage() const;
    float getCpuFreq() const;
    std::string getCpuInfo() const;
    float calcCpuUsage(int logger = 0);
};

#endif
