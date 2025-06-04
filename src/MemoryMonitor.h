#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <vector>
#include <string>

class MemoryMonitor {
public:
    struct RAM {
        std::vector<std::string> memInfo;
    };

private:
    size_t totalMemMb;
    float freeMem;
    size_t SwapMemMb;
    float freeSwap;
    RAM ram;

public:
    MemoryMonitor();
    bool update();
    unsigned long long getTotalMemory();
    unsigned long long getFreeMemory();
    unsigned long long getTotalSwap();
    unsigned long long getUsedSwap();
    double getMemoryUsagePercentage();
    double getSwapUsagePercentage();
};

#endif // MEMORYMONITOR_H
 
