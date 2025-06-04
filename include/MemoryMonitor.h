#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <string>
#include <vector>
using namespace std;

class MemoryMonitor {
private:
    struct ram {
        size_t totalMemInMb;
        float usage;
        float freeMem;
        size_t SwapMeminMb;
        float usageSwp;
        float freeSwp;
    } RAM;
    vector<string> memInfo;

public:
    bool update();
    unsigned long getTotalMemory() const;
    unsigned long getFreeMemory() const;
    unsigned long getUsedMemory() const;
    double getMemoryUsagePercentage() const;
    unsigned long getTotalSwap() const;
    unsigned long getFreeSwap() const;
    unsigned long getUsedSwap() const;
    double getSwapUsagePercentage() const;
};

//le diagramme UML était respecté d'une manière stricte 

#endif
