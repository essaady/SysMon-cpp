#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <vector>
#include <string>

class MemoryMonitor {
    struct RAM {
        size_t totalMemInMb;
        float usage;
        float freeMem;
        size_t swapMemInMb;
        float usageSwap;
        float freeSwap;
    } RAM;

    std::vector<std::string> memInfo;

public:
    bool update();
    unsigned long getTotalMemory();
    unsigned long getFreeMemory();
    unsigned long getUsedMemory();
    double getMemoryUsagePercentage();
    unsigned long getTotalSwap();
    unsigned long getFreeSwap();
    unsigned long getUsedSwap();
    double getSwapUsagePercentage();
};

#endif
