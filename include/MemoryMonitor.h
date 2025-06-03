#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

class MemoryMonitor {
public:
    bool update();

    unsigned long long getTotalMemory() const;
    unsigned long long getFreeMemory() const;
    unsigned long long getUsedMemory() const;

    unsigned long long getTotalSwap() const;
    unsigned long long getFreeSwap() const;
    unsigned long long getUsedSwap() const;

    double getMemoryUsagePercentage() const;
    double getSwapUsagePercentage() const;

private:
    unsigned long long totalMemMb = 0;
    unsigned long long freeMemMb = 0;
    unsigned long long totalSwapMb = 0;
    unsigned long long freeSwapMb = 0;
};

#endif 
