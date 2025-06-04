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
    if (!meminfo.is_open()) {
        std::cerr << "Failed to open /proc/meminfo" << std::endl;
        return false;
    }

    std::string line;
    unsigned long long memTotal = 0, memFree = 0, buffers = 0, cached = 0;
    unsigned long long swapTotal = 0, swapFree = 0;

    while (std::getline(meminfo, line)) {
        std::istringstream ss(line);
        std::string key;
        unsigned long long value;
        std::string unit;

        ss >> key >> value >> unit;

        if (key == "MemTotal:") memTotal = value;
        else if (key == "MemFree:") memFree = value;
        else if (key == "Buffers:") buffers = value;
        else if (key == "Cached:") cached = value;
        else if (key == "SwapTotal:") swapTotal = value;
        else if (key == "SwapFree:") swapFree = value;
    }
    meminfo.close();

    unsigned long long usedMem = memTotal - memFree - buffers - cached;
    unsigned long long usedSwap = swapTotal - swapFree;

    RAM.totalMemInMb = memTotal / 1024;
    RAM.freeMem = memFree / 1024;
    RAM.SwapMeminMb = swapTotal / 1024;
    RAM.freeSwp = swapFree / 1024;
    RAM.usage = usedMem / 1024;
    RAM.usageSwp = usedSwap / 1024;

    return true;
}

unsigned long long MemoryMonitor::getTotalMemory() {
    return (RAM.totalMemInMb * 1024 * 1024);
}

unsigned long long MemoryMonitor::getFreeMemory() {
    return (RAM.freeMem * 1024 * 1024);
}

unsigned long long MemoryMonitor::getUsedMemory() {
    return (RAM.usage * 1024 * 1024);
}

unsigned long long MemoryMonitor::getTotalSwap() {
    return (RAM.SwapMeminMb * 1024 * 1024);
}

unsigned long long MemoryMonitor::getFreeSwap() {
    return (RAM.freeSwp * 1024 * 1024);
}

unsigned long long MemoryMonitor::getUsedSwap() {
    return (RAM.usageSwp * 1024 * 1024);
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
