#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <iostream>
#include <string>
using namespace std;

class MemoryMonitor {
public:
    struct RAM {
        size_t totalMemInMb;
        float usage;
        float freeMem;
        size_t swapMeminMb;
        float usageSwp;
        float freeSwp;
    } ram;

    string memInfo;

    MemoryMonitor();

    bool update();

    unsigned long getTotalMemory() ;
    unsigned long getFreeMemory() ;
    unsigned long getUsedMemory() ;
    double getMemoryUsagePercentage() ;

    unsigned long getTotalSwap() ;
    unsigned long getFreeSwap() ;
    unsigned long getUsedSwap() ;
    double getSwapUsagePercentage() ;
};

#endif