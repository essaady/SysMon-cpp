#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <vector>
#include <string>
using namespace std;

struct RAM {
    size_t totalMemInMb;
    float usage;
    float freeMem;
    size_t swapMemInMb;
    float usageSwap;
    float freeSwap;
};

class MemoryMonitor {
private:
    vector<string> memInfo;

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
