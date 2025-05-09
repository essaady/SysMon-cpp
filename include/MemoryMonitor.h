#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <string>
#include <map>

class MemoryMonitor {
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

    std::string exportAsText() const;
    std::string exportAsCSV() const;

private:
    std::map<std::string, unsigned long long> memInfo;
    bool readMemInfo();
};

#endif
