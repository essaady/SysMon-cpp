#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <string>
#include <vector>

using namespace std;

struct RAM {
    vector<string> memInfo;
    unsigned long long totalPhysical;
    unsigned long long availablePhysical;
    unsigned long long usedPhysical;
};

class MemoryMonitor {
private:
    size_t totalMemMb;
    float freeMem;
    size_t SwapMemMb;
    float freeSwap;
    RAM ram;

public:
    MemoryMonitor();
    ~MemoryMonitor();
    
    bool update();
    unsigned long long getTotalMemory();
    unsigned long long getFreeMemory();
    unsigned long long getTotalSwap();
    unsigned long long getUsedSwap();
    double getMemoryUsagePercentage();
    double getSwapUsagePercentage();
    
    size_t getTotalMemMb() const { return totalMemMb; }
    float getFreeMem() const { return freeMem; }
    size_t getSwapMemMb() const { return SwapMemMb; }
    float getFreeSwap() const { return freeSwap; }
};

#endif 
