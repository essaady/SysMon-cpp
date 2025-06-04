#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <vector>
#include <string>

using namespace std;

class MemoryMonitor {
protected:
    struct RAM{
        size_t totalMemInMb;
        float usage;
        float freeMem;
        size_t SwapMeminMb;
        float usageSwp;
        float freeSwp;
    }RAM;
    vector<string> memInfo ;
public:
    MemoryMonitor();
    
    bool update();
    
    unsigned long long getTotalMemory();
    unsigned long long getFreeMemory();
    unsigned long long getUsedMemory();

    double getMemoryUsagePercentage();

    unsigned long long getTotalSwap();
    unsigned long long getFreeSwap();
    unsigned long long getUsedSwap();

    double getSwapUsagePercentage();

    ~MemoryMonitor();
};

#endif