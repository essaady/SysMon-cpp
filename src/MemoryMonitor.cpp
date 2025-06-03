#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <vector>
#include <string>

class MemoryMonitor {
public:
    struct RAM {
        size_t totalMemInMb;
        float usage;
        float freeMem;
        size_t SwapMemInMb;
        float usageSwp;
        float freeSwp;
    };

    MemoryMonitor();
    
    bool update();
    unsigned long getTotalMemory() const;
    unsigned long getFreeMemory() const;
    unsigned long getUsedMemory() const;
    double getMemoryUsagePercentage() const;
    unsigned long getTotalSwap() const;
    unsigned long getFreeSwap() const;
    unsigned long getUsedSwap() const;
    double getSwapUsagePercentage() const;
    
    RAM getRAMInfo() const;
    std::vector<std::string> getMemInfo() const;

private:
    RAM ramInfo;
    std::vector<std::string> memInfo;
    
    void updateMemInfo();
};

#endif 
