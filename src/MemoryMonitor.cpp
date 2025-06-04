#ifndef MEMORY_H
#define MEMORY_H

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
    unsigned long getUsedSwap() const;
    unsigned long getTotalSwap() const;
    unsigned long getFreeSwap() const;

    double getMemoryUsagePercentage() const;
    double getSwapUsagePercentage() const;

private:
    RAM ram;
    std::vector<std::string> memInfo;

    void parseMemInfo();
};

#endif 


#include "Memory.h"
#include <fstream>
#include <sstream>
#include <string>

bool MemoryMonitor::update() {
    parseMemInfo();
    for (const auto& line : memInfo) {
        std::istringstream iss(line);
        std::string key;
        unsigned long value;
        std::string unit;
        iss >> key >> value >> unit;

        if (key == "MemTotal:") ram.totalMemInMb = value / 1024;
        else if (key == "MemFree:") ram.freeMem = value / 1024.0;
        else if (key == "SwapTotal:") ram.SwapMemInMb = value / 1024;
        else if (key == "SwapFree:") ram.freeSwp = value / 1024.0;
    }

    ram.usage = ram.totalMemInMb - ram.freeMem;
    ram.usageSwp = ram.SwapMemInMb - ram.freeSwp;

    return true;
}

void MemoryMonitor::parseMemInfo() {
    memInfo.clear();
    std::ifstream file("/proc/meminfo");
    std::string line;
    while (std::getline(file, line)) {
        memInfo.push_back(line);
    }
}

unsigned long MemoryMonitor::getTotalMemory() const {
    return ram.totalMemInMb;
}

unsigned long MemoryMonitor::getFreeMemory() const {
    return static_cast<unsigned long>(ram.freeMem);
}

unsigned long MemoryMonitor::getUsedMemory() const {
    return static_cast<unsigned long>(ram.usage);
}

unsigned long MemoryMonitor::getUsedSwap() const {
    return static_cast<unsigned long>(ram.usageSwp);
}

unsigned long MemoryMonitor::getTotalSwap() const {
    return ram.SwapMemInMb;
}

unsigned long MemoryMonitor::getFreeSwap() const {
    return static_cast<unsigned long>(ram.freeSwp);
}

double MemoryMonitor::getMemoryUsagePercentage() const {
    if (ram.totalMemInMb == 0) return 0.0;
    return (ram.usage / ram.totalMemInMb) * 100.0;
}

double MemoryMonitor::getSwapUsagePercentage() const {
    if (ram.SwapMemInMb == 0) return 0.0;
    return (ram.usageSwp / ram.SwapMemInMb) * 100.0;
}
