#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <vector>
#include <string>

class RAM {
public:
    std::vector<std::string> memInfo;
};

class MemoryMonitor {
public:
    size_t totalMemMb;
    float freeMem;
    size_t SwapMemMb;
    float freeSwap;
    RAM ram;

    bool update();
    unsigned long long getTotalMemory() const;
    unsigned long long getFreeMemory() const;
    unsigned long long getTotalSwap() const;
    unsigned long long getUsedSwap() const;
    double getMemoryUsagePercentage() const;
    double getSwapUsagePercentage() const;
};

#endif
