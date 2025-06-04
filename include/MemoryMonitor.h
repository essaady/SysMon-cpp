#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <vector>
#include <string>

class MemoryMonitor {
private:
    struct ram {
        size_t totalMemInMb;
        float usage;
        float freeMem;
        size_t swapMemInMb;
        float usageSwp;
        float freeSwp;
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
