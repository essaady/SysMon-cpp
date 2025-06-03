#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <vector>
#include <string>

class MemoryMonitor {
private:
    // Structure to hold RAM information
    struct RAM {
        size_t totalMemInMb;
        float usage;
        float freeMem;
        size_t swapMemInMb;
        float usageSwp;
        float freeSwap;
    } ram;

    // Vector to store raw memory information from /proc/meminfo
    std::vector<std::string> memInfo;

    // Private method to parse memory information from /proc/meminfo
    bool parseMemoryInfo();

public:
    // Constructor
    MemoryMonitor();

    // Update memory information
    bool update();

    // Getter methods for memory information
    unsigned long getTotalMemory();
    unsigned long getFreeMemory();
    unsigned long getUsedMemory();
    double getMemoryUsagePercentage();

    // Getter methods for swap information
    unsigned long getTotalSwap();
    unsigned long getFreeSwap();
    unsigned long getUsedSwap();
    double getSwapUsagePercentage();

    // Display methods
    void displayMemoryInfo();
    std::string getMemoryInfoString();
};

#endif // MEMORYMONITOR_H