#include "../include/MemoryMonitor.h"
#include <iostream>
#include <iomanip>

MemoryMonitor::MemoryMonitor() {
    RAM.totalMemInMb = 0;
    RAM.usage = 0.0f;
    RAM.freeMem = 0.0f;
    RAM.swapMemInMb = 0;
    RAM.usageSwp = 0.0f;
    RAM.freeSwp = 0.0f;
    
    update(); // Initial update
}

bool MemoryMonitor::update() {
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) {
        return false;
    }
    
    memInfo.clear();
    std::string line;
    std::map<std::string, unsigned long> memData;
    
    while (std::getline(file, line)) {
        memInfo.push_back(line);
        
        std::istringstream iss(line);
        std::string key;
        unsigned long value;
        std::string unit;
        
        if (iss >> key >> value >> unit) {
            // Remove the colon from the key
            if (!key.empty() && key.back() == ':') {
                key.pop_back();
            }
            memData[key] = value;
        }
    }
    file.close();
    
    // Calculate memory statistics
    unsigned long totalMem = memData["MemTotal"];
    unsigned long freeMem = memData["MemFree"];
    unsigned long buffers = memData["Buffers"];
    unsigned long cached = memData["Cached"];
    unsigned long available = memData["MemAvailable"];
    
    unsigned long totalSwap = memData["SwapTotal"];
    unsigned long freeSwap = memData["SwapFree"];
    
    // Convert from KB to MB
    RAM.totalMemInMb = totalMem / 1024;
    RAM.swapMemInMb = totalSwap / 1024;
    
    // Calculate usage percentages
    unsigned long usedMem = totalMem - available;
    RAM.usage = (float)usedMem / totalMem * 100.0f;
    RAM.freeMem = (float)available / totalMem * 100.0f;
    
    if (totalSwap > 0) {
        unsigned long usedSwap = totalSwap - freeSwap;
        RAM.usageSwp = (float)usedSwap / totalSwap * 100.0f;
        RAM.freeSwp = (float)freeSwap / totalSwap * 100.0f;
    } else {
        RAM.usageSwp = 0.0f;
        RAM.freeSwp = 0.0f;
    }
    
    return true;
}

unsigned long MemoryMonitor::getTotalMemory() {
    return RAM.totalMemInMb * 1024; // Return in KB
}

unsigned long MemoryMonitor::getFreeMemory() {
    return (unsigned long)(RAM.totalMemInMb * RAM.freeMem / 100.0f * 1024); // Return in KB
}

unsigned long MemoryMonitor::getUsedMemory() {
    return (unsigned long)(RAM.totalMemInMb * RAM.usage / 100.0f * 1024); // Return in KB
}

double MemoryMonitor::getMemoryUsagePercentage() {
    return RAM.usage;
}

unsigned long MemoryMonitor::getTotalSwap() {
    return RAM.swapMemInMb * 1024; // Return in KB
}

unsigned long MemoryMonitor::getFreeSwap() {
    return (unsigned long)(RAM.swapMemInMb * RAM.freeSwp / 100.0f * 1024); // Return in KB
}

unsigned long MemoryMonitor::getUsedSwap() {
    return (unsigned long)(RAM.swapMemInMb * RAM.usageSwp / 100.0f * 1024); // Return in KB
}

double MemoryMonitor::getSwapUsagePercentage() {
    return RAM.usageSwp;
}