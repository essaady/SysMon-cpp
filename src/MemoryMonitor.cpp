#include "MemoryMonitor.h"
#include <windows.h>
#include <psapi.h>

bool MemoryMonitor::update() {
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    if (GlobalMemoryStatusEx(&memStatus)) {
        ram.totalMemMb = memStatus.ullTotalPhys / (1024 * 1024);
        ram.freeMem = memStatus.ullAvailPhys / (1024.0 * 1024.0);
        ram.usage = 100.0f - ((ram.freeMem / ram.totalMemMb) * 100.0f);
        return true;
    }
    return false;
}

unsigned long long MemoryMonitor::getTotalMemory() {
    return ram.totalMemMb;
}

unsigned long long MemoryMonitor::getFreeMemory() {
    return static_cast<unsigned long long>(ram.freeMem);
}

unsigned long long MemoryMonitor::getTotalSwap() {
    return ram.SwapMemMb;
}

unsigned long long MemoryMonitor::getUsedSwap() {
    return ram.SwapMemMb - static_cast<unsigned long long>(ram.freeSwap);
}

double MemoryMonitor::getMemoryUsagePercentage() {
    return ram.usage;
}

double MemoryMonitor::getSwapUsagePercentage() {
    return 100.0 - ((ram.freeSwap / ram.SwapMemMb) * 100.0);
}
