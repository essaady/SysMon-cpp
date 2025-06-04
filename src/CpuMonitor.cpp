#include "MemoryMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

MemoryMonitor::MemoryMonitor() {
    totalMemMb = 0;
    freeMem = 0.0f;
    SwapMemMb = 0;
    freeSwap = 0.0f;
    
    ram.totalPhysical = 0;
    ram.availablePhysical = 0;
    ram.usedPhysical = 0;
}

MemoryMonitor::~MemoryMonitor() {}

bool MemoryMonitor::update() {
    try {
        ifstream meminfoFile("/proc/meminfo");
        if (!meminfoFile.is_open()) {
            cerr << "Erreur: Impossible d'ouvrir /proc/meminfo" << endl;
            return false;
        }
        
        string line;
        ram.memInfo.clear();
        
        unsigned long long memTotal = 0, memFree = 0, memAvailable = 0;
        unsigned long long buffers = 0, cached = 0, swapTotal = 0, swapFree = 0;
        
        while (getline(meminfoFile, line)) {
            ram.memInfo.push_back(line);
            
            istringstream iss(line);
            string key;
            unsigned long long value;
            string unit;
            
            iss >> key >> value >> unit;
            value *= 1024;
            
            if (key == "MemTotal:") {
                memTotal = value;
                ram.totalPhysical = value;
                totalMemMb = value / (1024 * 1024);
            } else if (key == "MemFree:") {
                memFree = value;
            } else if (key == "MemAvailable:") {
                memAvailable = value;
                ram.availablePhysical = value;
            } else if (key == "Buffers:") {
                buffers = value;
            } else if (key == "Cached:") {
                cached = value;
            } else if (key == "SwapTotal:") {
                swapTotal = value;
                SwapMemMb = value / (1024 * 1024);
            } else if (key == "SwapFree:") {
                swapFree = value;
            }
        }
        
        meminfoFile.close();
        
        ram.usedPhysical = memTotal - memAvailable;
        freeMem = static_cast<float>(memAvailable) / (1024 * 1024); 
        freeSwap = static_cast<float>(swapFree) / (1024 * 1024); 
        
        return true;
    } catch (const exception& e) {
        cerr << "Erreur lors de la mise à jour mémoire: " << e.what() << endl;
        return false;
    }
}

unsigned long long MemoryMonitor::getTotalMemory() {
    return ram.totalPhysical;
}

unsigned long long MemoryMonitor::getFreeMemory() {
    return ram.availablePhysical;
}

unsigned long long MemoryMonitor::getTotalSwap() {
    return static_cast<unsigned long long>(SwapMemMb) * 1024 * 1024;
}

unsigned long long MemoryMonitor::getUsedSwap() {
    unsigned long long totalSwapBytes = getTotalSwap();
    unsigned long long freeSwapBytes = static_cast<unsigned long long>(freeSwap) * 1024 * 1024;
    return totalSwapBytes - freeSwapBytes;
}

double MemoryMonitor::getMemoryUsagePercentage() {
    if (ram.totalPhysical == 0) {
        return 0.0;
    }
    return (static_cast<double>(ram.usedPhysical) / ram.totalPhysical) * 100.0;
}

double MemoryMonitor::getSwapUsagePercentage() {
    unsigned long long totalSwapBytes = getTotalSwap();
    if (totalSwapBytes == 0) {
        return 0.0;
    }
    unsigned long long usedSwapBytes = getUsedSwap();
    return (static_cast<double>(usedSwapBytes) / totalSwapBytes) * 100.0;
}
