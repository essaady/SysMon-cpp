#include "../include/MemoryMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

MemoryMonitor::MemoryMonitor() {
    // Initialize memory info structure
    memInfo.clear();
    ram.totalMemInMb = 0;
    ram.usage = 0.0f;
    ram.freeMem = 0;
    ram.swapMemInMb = 0;
    ram.usageSwp = 0.0f;
    ram.freeSwap = 0;
}

bool MemoryMonitor::update() {
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open /proc/meminfo" << std::endl;
        return false;
    }

    std::string line;
    memInfo.clear();
    
    // Read all memory information from /proc/meminfo
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value, unit;
        
        if (iss >> key >> value >> unit) {
            // Remove the colon from the key
            if (!key.empty() && key.back() == ':') {
                key.pop_back();
            }
            memInfo.push_back(key + ": " + value + " " + unit);
        }
    }
    file.close();

    // Parse specific memory values
    return parseMemoryInfo();
}

bool MemoryMonitor::parseMemoryInfo() {
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    unsigned long totalMem = 0, freeMem = 0, availableMem = 0;
    unsigned long totalSwap = 0, freeSwap = 0;
    unsigned long buffers = 0, cached = 0;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        unsigned long value;
        
        if (iss >> key >> value) {
            if (key == "MemTotal:") {
                totalMem = value;
            } else if (key == "MemFree:") {
                freeMem = value;
            } else if (key == "MemAvailable:") {
                availableMem = value;
            } else if (key == "Buffers:") {
                buffers = value;
            } else if (key == "Cached:") {
                cached = value;
            } else if (key == "SwapTotal:") {
                totalSwap = value;
            } else if (key == "SwapFree:") {
                freeSwap = value;
            }
        }
    }
    file.close();

    // Convert from KB to MB
    ram.totalMemInMb = totalMem / 1024;
    ram.freeMem = freeMem / 1024;
    ram.swapMemInMb = totalSwap / 1024;
    ram.freeSwap = freeSwap / 1024;

    // Calculate used memory (total - available is more accurate than total - free)
    unsigned long usedMem = totalMem - (availableMem > 0 ? availableMem : freeMem);
    unsigned long usedSwap = totalSwap - freeSwap;

    // Calculate usage percentages
    if (totalMem > 0) {
        ram.usage = (static_cast<float>(usedMem) / totalMem) * 100.0f;
    }
    
    if (totalSwap > 0) {
        ram.usageSwp = (static_cast<float>(usedSwap) / totalSwap) * 100.0f;
    }

    return true;
}

unsigned long MemoryMonitor::getTotalMemory() {
    return ram.totalMemInMb;
}

unsigned long MemoryMonitor::getFreeMemory() {
    return ram.freeMem;
}

unsigned long MemoryMonitor::getUsedMemory() {
    return ram.totalMemInMb - ram.freeMem;
}

double MemoryMonitor::getMemoryUsagePercentage() {
    return static_cast<double>(ram.usage);
}

unsigned long MemoryMonitor::getTotalSwap() {
    return ram.swapMemInMb;
}

unsigned long MemoryMonitor::getFreeSwap() {
    return ram.freeSwap;
}

unsigned long MemoryMonitor::getUsedSwap() {
    return ram.swapMemInMb - ram.freeSwap;
}

double MemoryMonitor::getSwapUsagePercentage() {
    return static_cast<double>(ram.usageSwp);
}

void MemoryMonitor::displayMemoryInfo() {
    if (!update()) {
        std::cout << "Error: Failed to update memory information" << std::endl;
        return;
    }

    // Convert MB to GB for display
    double totalGB = ram.totalMemInMb / 1024.0;
    double usedGB = (ram.totalMemInMb - ram.freeMem) / 1024.0;
    double freeGB = ram.freeMem / 1024.0;
    
    double totalSwapGB = ram.swapMemInMb / 1024.0;
    double usedSwapGB = (ram.swapMemInMb - ram.freeSwap) / 1024.0;

    std::cout << std::fixed << std::setprecision(1);
    
    // Main memory information
    std::cout << "=== Memory Information ===" << std::endl;
    std::cout << "RAM Used: " << usedGB << " GB / " << totalGB << " GB";
    std::cout << " (" << std::setprecision(1) << ram.usage << "%)" << std::endl;
    std::cout << "RAM Free: " << freeGB << " GB" << std::endl;
    
    // Swap information (if available)
    if (ram.swapMemInMb > 0) {
        std::cout << "Swap Used: " << usedSwapGB << " GB / " << totalSwapGB << " GB";
        std::cout << " (" << std::setprecision(1) << ram.usageSwp << "%)" << std::endl;
    } else {
        std::cout << "Swap: Not available" << std::endl;
    }
    
    std::cout << std::endl;
}

std::string MemoryMonitor::getMemoryInfoString() {
    if (!update()) {
        return "Error: Failed to get memory information";
    }

    std::ostringstream oss;
    double totalGB = ram.totalMemInMb / 1024.0;
    double usedGB = (ram.totalMemInMb - ram.freeMem) / 1024.0;
    
    oss << std::fixed << std::setprecision(1);
    oss << "RAM Used: " << usedGB << " GB / " << totalGB << " GB";
    oss << " (" << ram.usage << "%)";
    
    return oss.str();
}