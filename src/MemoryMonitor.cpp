#include "MemoryMonitor.h"

bool MemoryMonitor::update() { return true; }
unsigned long MemoryMonitor::getTotalMemory() { return 0; }
unsigned long MemoryMonitor::getFreeMemory() { return 0; }
unsigned long MemoryMonitor::getUsedMemory() { return 0; }
double MemoryMonitor::getMemoryUsagePercentage() { return 0.0; }
unsigned long MemoryMonitor::getTotalSwap() { return 0; }
unsigned long MemoryMonitor::getFreeSwap() { return 0; }
unsigned long MemoryMonitor::getUsedSwap() { return 0; }
double MemoryMonitor::getSwapUsagePercentage() { return 0.0; }
