#pragma once
#ifndef _MEMORYMONITOR_H
#define _MEMORYMONITOR_H

#include <string>
#include <vector>

typedef struct ram {
    size_t totalMemInMb;    // Total RAM in MB
    float usage;            // Current RAM usage
    float freeMem;          // Free RAM
    size_t SwapMemInMb;     // Total swap memory in MB
    float usageSwp;         // Swap usage
    float freeSwp;          // Free swap space
    std::vector<std::string> memInfo;  // Memory information details
} ram;

class MemoryMonitor {
protected:
    ram RAM;
    std::vector<std::string> memInfo;

public:
    MemoryMonitor();
    
    ~MemoryMonitor();

    bool update();

    unsigned long long getTotalMemory() const;

    unsigned long long getFreeMemory() const;
    
    unsigned long long getUsedMemory() const;
    
    double getMemoryUsagePercentage() const;

    unsigned long long getTotalSwap() const;
    
    unsigned long long getFreeSwap() const;
    
    unsigned long long getUsedSwap() const;
    
    double getSwapUsagePercentage() const;

    std::size_t memUsage(int log);

};

#endif