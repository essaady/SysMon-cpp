#include "MemoryMonitor.h"
#include <fstream>
#include <sstream>

bool MemoryMonitor::update() {
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) return false;

    std::string line;
    ram.memInfo.clear();
    while (getline(meminfo, line)) {
        ram.memInfo.push_back(line);
        std::istringstream iss(line);
        std::string key;
        unsigned long value;
        std::string unit;
        iss >> key >> value >> unit;

        if (key == "MemTotal:") totalMemMb = value / 1024;
        if (key == "MemFree:") freeMem = value / 1024.0f;
        if (key == "SwapTotal:") SwapMemMb = value / 1024;
        if (key == "SwapFree:") freeSwap = value / 1024.0f;
    }
    return true;
}

unsigned long long MemoryMonitor::getTotalMemory() const {
    return totalMemMb;
}

unsigned long long MemoryMonitor::getFreeMemory() const {
    return static_cast<unsigned long long>(freeMem);
}

unsigned long long MemoryMonitor::getTotalSwap() const {
    return SwapMemMb;
}

unsigned long long MemoryMonitor::getUsedSwap() const {
    return SwapMemMb - static_cast<unsigned long long>(freeSwap);
}

double MemoryMonitor::getMemoryUsagePercentage() const {
    return (totalMemMb > 0) ? 100.0 * (totalMemMb - freeMem) / totalMemMb : 0.0;
}

double MemoryMonitor::getSwapUsagePercentage() const {
    return (SwapMemMb > 0) ? 100.0 * (SwapMemMb - freeSwap) / SwapMemMb : 0.0;
}
