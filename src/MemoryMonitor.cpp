#include "MemoryMonitor.h"
#include <iostream>

MemoryMonitor::MemoryMonitor() {
    RAM.totalMemInMb = 0;
    RAM.freeMem = 0;
    RAM.SwapMeminMb = 0;
    RAM.freeSwp = 0;
    RAM.usage = 0;
    RAM.usageSwp = 0;
}


bool MemoryMonitor::update() {
    RAM.totalMemInMb = 16384;      // 16 GB total RAM
    RAM.freeMem = 8192;            // 8 GB free RAM
    RAM.SwapMeminMb = 8192;        // 8 GB total swap
    RAM.freeSwp = 6144;            // 6 GB free swap
    RAM.usage = RAM.totalMemInMb - RAM.freeMem;  // Used RAM
    RAM.usageSwp = RAM.SwapMeminMb - RAM.freeSwp;  // Used swap
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
