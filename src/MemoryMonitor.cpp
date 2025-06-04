// src/MemoryMonitor.cpp
#include "MemoryMonitor.h"
#include <fstream>
#include <sstream>

bool MemoryMonitor::update() {
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        memInfo.push_back(line);
        if (line.find("MemTotal") != std::string::npos)
            ram.totalMemInMb = std::stoul(line.substr(line.find(":") + 1)) / 1024;
        else if (line.find("MemFree") != std::string::npos)
            ram.freeMem = std::stof(line.substr(line.find(":") + 1)) / 1024;
        else if (line.find("SwapTotal") != std::string::npos)
            ram.SwapMemInMb = std::stoul(line.substr(line.find(":") + 1)) / 1024;
        else if (line.find("SwapFree") != std::string::npos)
            ram.freeSwap = std::stof(line.substr(line.find(":") + 1)) / 1024;
    }

    ram.usage = ram.totalMemInMb - ram.freeMem;
    ram.usageSwp = ram.SwapMemInMb - ram.freeSwap;

    return true;
}

unsigned long long MemoryMonitor::getTotalMemory() { return ram.totalMemInMb; }
unsigned long long MemoryMonitor::getFreeMemory() { return ram.freeMem; }
unsigned long long MemoryMonitor::getUsedMemory() { return ram.usage; }

double MemoryMonitor::getMemoryUsagePercentage() {
    return (ram.usage / ram.totalMemInMb) * 100.0;
}

unsigned long long MemoryMonitor::getTotalSwap() { return ram.SwapMemInMb; }
unsigned long long MemoryMonitor::getFreeSwap() { return ram.freeSwap; }
unsigned long long MemoryMonitor::getUsedSwap() { return ram.usageSwp; }

double MemoryMonitor::getSwapUsagePercentage() {
    return (ram.usageSwp / ram.SwapMemInMb) * 100.0;
}
