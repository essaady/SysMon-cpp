// include/MemoryMonitor.h
#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <vector>
#include <string>

struct RAM {
    size_t totalMemInMb;
    float usage;
    float freeMem;
    size_t SwapMemInMb;
    float usageSwp;
    float freeSwp;
};

class MemoryMonitor {
private:
    RAM ram;
    std::vector<std::string> memInfo;
public:
    bool update();
    unsigned long long getTotalMemory();
    unsigned long long getFreeMemory();
    unsigned long long getUsedMemory();
    double getMemoryUsagePercentage();
    unsigned long long getTotalSwap();
    unsigned long long getFreeSwap();
    unsigned long long getUsedSwap();
    double getSwapUsagePercentage();
};

#endif
