#include "../include/MemoryMonitor.h"
#include <fstream>
#include <sstream>
#include <string>

bool MemoryMonitor::update() {
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) return false;

    std::string line;
    unsigned long memTotal = 0, memFree = 0, buffers = 0, cached = 0;
    unsigned long swapTotal = 0, swapFree = 0;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        unsigned long value;
        std::string unit;
        iss >> key >> value >> unit;

        if (key == "MemTotal:") memTotal = value;
        else if (key == "MemFree:") memFree = value;
        else if (key == "Buffers:") buffers = value;
        else if (key == "Cached:") cached = value;
        else if (key == "SwapTotal:") swapTotal = value;
        else if (key == "SwapFree:") swapFree = value;
    }

    unsigned long memAvailable = memFree + buffers + cached;
    unsigned long memUsed = memTotal - memAvailable;

    RAM.totalMemInMb = memTotal / 1024;
    RAM.freeMem = memAvailable / 1024.0;
    RAM.usage = memUsed / 1024.0;

    RAM.SwapMeminMb = swapTotal / 1024;
    RAM.freeSwp = swapFree / 1024.0;
    RAM.usageSwp = (swapTotal - swapFree) / 1024.0;

    return true;
}

unsigned long MemoryMonitor::getTotalMemory() {
    return RAM.totalMemInMb;
}

unsigned long MemoryMonitor::getFreeMemory() {
    return static_cast<unsigned long>(RAM.freeMem);
}

unsigned long MemoryMonitor::getUsedMemory() {
    return static_cast<unsigned long>(RAM.usage);
}

double MemoryMonitor::getMemoryUsagePercentage() {
    if (RAM.totalMemInMb == 0) return 0.0;
    return (RAM.usage / RAM.totalMemInMb) * 100.0;
}

unsigned long MemoryMonitor::getTotalSwap() {
    return RAM.SwapMeminMb;
}

unsigned long MemoryMonitor::getFreeSwap() {
    return static_cast<unsigned long>(RAM.freeSwp);
}

unsigned long MemoryMonitor::getUsedSwap() {
    return static_cast<unsigned long>(RAM.usageSwp);
}

double MemoryMonitor::getSwapUsagePercentage() {
    if (RAM.SwapMeminMb == 0) return 0.0;
    return (RAM.usageSwp / RAM.SwapMeminMb) * 100.0;
}
