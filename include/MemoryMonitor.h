#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <string>

class MemoryMonitor
{
private:
    size_t totalMemMb;
    float freeMem;
    size_t SwapMemMb;
    float freeSwap;

public:
    bool update();
    unsigned long long getTotalMemory();
    unsigned long long getFreeMemory();
    unsigned long long getTotalSwap();
    unsigned long long getUsedSwap();
    double getMemoryUsagePercentage();
    double getSwapUsagePercentage();
};

#endif
