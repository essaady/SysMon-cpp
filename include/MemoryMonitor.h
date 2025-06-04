#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <string>
#include <vector>

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

    bool update();

    unsigned long getTotalMemory() const;
    unsigned long getFreeMemory() const;
    unsigned long getUsedMemory() const;
    double getMemoryUsagePercentage() const;

    unsigned long getTotalSwap() const;
    unsigned long getFreeSwap() const;
    unsigned long getUsedSwap() const;
    double getSwapUsagePercentage() const;

private:
    size_t extractValue(const std::string& line) const;

public:
    RAM RAM;
    std::vector<std::string> memInfo;
};

#endif
