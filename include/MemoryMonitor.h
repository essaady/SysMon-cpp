#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <vector>
#include <string>
using namespace std;

class MemoryMonitor {
public:
    struct RAM {
        size_t totalMemMb;
        float usage;
        float freeMem;
        size_t SwapMemMb;
        float freeSwap;
    };

private:
    RAM ram;
    vector<string> memInfo;

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
