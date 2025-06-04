#include "MemoryMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

MemoryMonitor::MemoryMonitor() {
    RAM.totalMemInMb = 0;
    RAM.freeMem = 0;
    RAM.SwapMeminMb = 0;
    RAM.freeSwp = 0;
    RAM.usage = 0;
    RAM.usageSwp = 0;
}


bool MemoryMonitor::update() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;

    unsigned long long memTotal = 0, memFree = 0, memAvailable = 0;
    unsigned long long swapTotal = 0, swapFree = 0;

    while (std::getline(meminfo, line)) {
        if (line.compare(0, 9, "MemTotal:") == 0)
            sscanf(line.c_str(), "MemTotal: %llu", &memTotal);
        else if (line.compare(0, 13, "MemAvailable:") == 0)
            sscanf(line.c_str(), "MemAvailable: %llu", &memAvailable);
        else if (line.compare(0, 10, "SwapTotal:") == 0)
            sscanf(line.c_str(), "SwapTotal: %llu", &swapTotal);
        else if (line.compare(0, 9, "SwapFree:") == 0)
            sscanf(line.c_str(), "SwapFree: %llu", &swapFree);
    }

    // from KB to MB
    RAM.totalMemInMb = memTotal / 1024;
    RAM.freeMem = memAvailable / 1024;
    RAM.SwapMeminMb = swapTotal / 1024;
    RAM.freeSwp = swapFree / 1024;

    RAM.usage = RAM.totalMemInMb - RAM.freeMem;
    RAM.usageSwp = RAM.SwapMeminMb - RAM.freeSwp;
    return true; 
}

unsigned long long MemoryMonitor::getTotalMemory() {
    return (RAM.totalMemInMb);
}

unsigned long long MemoryMonitor::getFreeMemory() {
    return (RAM.freeMem);
}

unsigned long long MemoryMonitor::getUsedMemory() {
    return (RAM.usage);
}

unsigned long long MemoryMonitor::getTotalSwap() {
    return (RAM.SwapMeminMb);
}

unsigned long long MemoryMonitor::getFreeSwap() {
    return (RAM.freeSwp);
}

unsigned long long MemoryMonitor::getUsedSwap() {
    return (RAM.usageSwp);
}

double MemoryMonitor::getMemoryUsagePercentage() {
    if (RAM.totalMemInMb == 0) return 0.0;
    return ((double)RAM.usage / RAM.totalMemInMb) * 100.0;
}

double MemoryMonitor::getSwapUsagePercentage() {
    if (RAM.SwapMeminMb == 0) return 0.0;
    return ((double)RAM.usageSwp / RAM.SwapMeminMb) * 100.0;
}

std::string MemoryMonitor::getMemoryInfo() {
    const double MB_TO_GB = 1024.0;
    
    std::string info = "System Memory Information\n";
    info += "----------------------------------------\n";
    info += "RAM Total: " + std::to_string(RAM.totalMemInMb / MB_TO_GB) + " GB\n";
    info += "RAM Free:  " + std::to_string(RAM.freeMem / MB_TO_GB) + " GB\n";
    info += "RAM Used:  " + std::to_string(RAM.usage / MB_TO_GB) + " GB\n";
    info += "RAM Usage: " + std::to_string(getMemoryUsagePercentage()) + "%\n";
    info += "----------------------------------------\n";
    info += "Swap Total: " + std::to_string(RAM.SwapMeminMb / MB_TO_GB) + " GB\n";
    info += "Swap Free:  " + std::to_string(RAM.freeSwp / MB_TO_GB) + " GB\n";
    info += "Swap Used:  " + std::to_string(RAM.usageSwp / MB_TO_GB) + " GB\n";
    info += "Swap Usage: " + std::to_string(getSwapUsagePercentage()) + "%\n";
    info += "========================================\n";
    
    return info;
}

MemoryMonitor::~MemoryMonitor() {}
