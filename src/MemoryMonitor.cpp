#include "MemoryMonitor.h"
#include <fstream>
#include <sstream>
#include <string>
using namespace std ;



bool MemoryMonitor::update() {
    ifstream fyle("/proc/meminfo");
    if (!fyle.is_open()) return false;

    string line;
    unsigned long memTotal = 0, memFree = 0, swapTotal = 0, swapFree = 0;
    memInfo.clear();

    while (getline(file, line)) {
        istringstream iss(line);
        string key;
        unsigned long value;
        string unit;
        iss >> key >> value >> unit;

        if (key == "MemTotal:") memTotal = value;
        else if (key == "MemFree:") memFree = value;
        else if (key == "SwapTotal:") swapTotal = value;
        else if (key == "SwapFree:") swapFree = value;

       
    }
    
    ram.totalMeminMb = memTotal / 1024;
    ram.freeMem = memFree / 1024.0f;
    ram.usage = ram.totalMeminMb - ram.freeMem;

    ram.swapMeminMb = swapTotal / 1024;
    ram.freeSwp = swapFree / 1024.0f;
    ram.usageSwp = ram.swapMeminMb - ram.freeSwp;

    return true;
}

unsigned long MemoryMonitor::getTotalMemory() const {
    return ram.totalMeminMb;
}

unsigned long MemoryMonitor::getFreeMemory() const {
    return static_cast<unsigned long>(ram.freeMem);
}

unsigned long MemoryMonitor::getUsedMemory() const {
    return static_cast<unsigned long>(ram.usage);
}

unsigned long MemoryMonitor::getTotalSwap() const {
    return ram.swapMeminMb;
}

unsigned long MemoryMonitor::getFreeSwap() const {
    return static_cast<unsigned long>(ram.freeSwp);
}

unsigned long MemoryMonitor::getUsedSwap() const {
    return static_cast<unsigned long>(ram.usageSwp);
}

double MemoryMonitor::getMemoryUsagePercentage() const {
    return  (ram.usage / ram.totalMeminMb) * 100.0;
}


double MemoryMonitor::getSwapUsagePercentage() const {
    return  (ram.usageSwp / ram.swapMeminMb) * 100.0;
}
