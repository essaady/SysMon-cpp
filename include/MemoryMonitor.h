#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <string>
#include <vector>
using namespace std;

struct ram {
    size_t totalMemInMb;
    float usage;
    float freeMem;
    size_t SwapMemInMb;
    float usageSwp;
    float freeSwp;
};

class MemoryMonitor {
private:
    ram RAM;
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
