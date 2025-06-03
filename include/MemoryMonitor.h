#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <string>
#include <map>
#include <vector>

class MemoryMonitor {
public:
    struct RAM {
        std::vector<std::string> memInfo;
    };

    MemoryMonitor();
    ~MemoryMonitor() = default;

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
    size_t totalMemMb;
    float freeMem;
    size_t SwapMemMb;
    float freeSwap;

    RAM rawData;
    std::map<std::string, unsigned long long> parsedInfo;

    bool readMemInfo();
};

#endif // MEMORY_MONITOR_H