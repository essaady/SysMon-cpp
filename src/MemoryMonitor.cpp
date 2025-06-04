#include "MemoryMonitor.h"
#include <iostream>

bool MemoryMonitor::update() {
   
    RAM.totalMemInMb = 8192;  
    RAM.freeMem = 2048;       
    RAM.usage = RAM.totalMemInMb - RAM.freeMem;

    RAM.swapMemInMb = 2048;   
    RAM.freeSwp = 1024;       
    RAM.usageSwp = RAM.swapMemInMb - RAM.freeSwp;

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
    return RAM.swapMemInMb;
}

unsigned long MemoryMonitor::getFreeSwap() {
    return static_cast<unsigned long>(RAM.freeSwp);
}

unsigned long MemoryMonitor::getUsedSwap() {
    return static_cast<unsigned long>(RAM.usageSwp);
}

double MemoryMonitor::getSwapUsagePercentage() {
    if (RAM.swapMemInMb == 0) return 0.0;
    return (RAM.usageSwp / RAM.swapMemInMb) * 100.0;
}
