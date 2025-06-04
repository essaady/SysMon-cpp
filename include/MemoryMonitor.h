#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct ram {
    size_t totalMemInMb;
    float usage;
    float freeMem;
    size_t swapMemInMb;
    float usageSwp;
    float freeSwp;
};

class MemoryMonitor {
private:
    ram RAM;
    std::vector<std::string> memInfo;

public:
    MemoryMonitor();
    
    // Core methods
    bool update();
    unsigned long getTotalMemory();
    unsigned long getFreeMemory();
    unsigned long getUsedMemory();
    double getMemoryUsagePercentage();
    unsigned long getTotalSwap();
    unsigned long getFreeSwap();
    unsigned long getUsedSwap();
    double getSwapUsagePercentage();
    
    // Getters
    ram getMemoryData() const { return RAM; }
    std::vector<std::string> getMemInfo() const { return memInfo; }
};

#endif 