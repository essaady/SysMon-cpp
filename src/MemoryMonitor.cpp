#include "MemoryMonitor.h"

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

bool MemoryMonitor::update() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (!GlobalMemoryStatusEx(&memInfo)) return false;

    totalMemMb = memInfo.ullTotalPhys / (1024 * 1024);
    freeMemMb = memInfo.ullAvailPhys / (1024 * 1024);

    totalSwapMb = memInfo.ullTotalPageFile / (1024 * 1024);
    freeSwapMb = memInfo.ullAvailPageFile / (1024 * 1024);

    return true;
}

#else
#include <sys/sysinfo.h>

bool MemoryMonitor::update() {
    struct sysinfo memInfo;
    if (sysinfo(&memInfo) != 0) return false;

    totalMemMb = memInfo.totalram / 1024 / 1024;
    freeMemMb = memInfo.freeram / 1024 / 1024;

    totalSwapMb = memInfo.totalswap / 1024 / 1024;
    freeSwapMb = memInfo.freeswap / 1024 / 1024;

    return true;
}
#endif

unsigned long long MemoryMonitor::getTotalMemory() const {
    return totalMemMb;
}

unsigned long long MemoryMonitor::getFreeMemory() const {
    return freeMemMb;
}

unsigned long long MemoryMonitor::getUsedMemory() const {
    return totalMemMb - freeMemMb;
}

unsigned long long MemoryMonitor::getTotalSwap() const {
    return totalSwapMb;
}

unsigned long long MemoryMonitor::getFreeSwap() const {
    return freeSwapMb;
}

unsigned long long MemoryMonitor::getUsedSwap() const {
    return totalSwapMb - freeSwapMb;
}

double MemoryMonitor::getMemoryUsagePercentage() const {
    return (totalMemMb > 0) ? 100.0 * (1.0 - (double)freeMemMb / totalMemMb) : 0.0;
}

double MemoryMonitor::getSwapUsagePercentage() const {
    return (totalSwapMb > 0) ? 100.0 * (1.0 - (double)freeSwapMb / totalSwapMb) : 0.0;
}
