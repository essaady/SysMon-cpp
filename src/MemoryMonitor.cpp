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
    return true; 
}

unsigned long long MemoryMonitor::getTotalMemory() {
    return (RAM.totalMemInMb * 1024 * 1024);
}

unsigned long long MemoryMonitor::getFreeMemory() {
    return (RAM.freeMem * 1024 * 1024);
}

unsigned long long MemoryMonitor::getUsedMemory(){
    return RAM.usage ;
}

unsigned long long MemoryMonitor::getTotalSwap() {
    return (RAM.SwapMeminMb * 1024 * 1024);
}

unsigned long long MemoryMonitor::getFreeSwap() {
    return (RAM.freeSwp * 1024 * 1024); 
}

unsigned long long MemoryMonitor::getUsedSwap(){
    return RAM.usageSwp;
}

double MemoryMonitor::getMemoryUsagePercentage() {
    return 2005; 
}

double MemoryMonitor::getSwapUsagePercentage() {
    return 2005; 
}

MemoryMonitor::~MemoryMonitor() {}
