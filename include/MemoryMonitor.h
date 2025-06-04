#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <string>
#include <vector>

class MemoryMonitor {
public:
    struct ram {
        size_t totalMemInMb;
        float usage;
        float freeMem;
        size_t SwapMeminMb;
        float usageSwp;
        float freeSwp;
    } RAM;

private:
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
