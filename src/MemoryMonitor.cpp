#include "Memory.h"
#include <windows.h>
#include <psapi.h>
#include <sstream>
#include <iomanip>

MemoryInfo Memory::getMemoryInfo() {
    MemoryInfo info;
    
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        info.totalPhysical = memInfo.ullTotalPhys;
        info.availablePhysical = memInfo.ullAvailPhys;
    }
    
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        info.processUsed = pmc.WorkingSetSize;
        info.processPeak = pmc.PeakWorkingSetSize;
    }
    
    return info;
}

size_t Memory::getTotalPhysicalMemory() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        return memInfo.ullTotalPhys;
    }
    return 0;
}

size_t Memory::getAvailablePhysicalMemory() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        return memInfo.ullAvailPhys;
    }
    return 0;
}

size_t Memory::getProcessMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
}

size_t Memory::getProcessPeakMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.PeakWorkingSetSize;
    }
    return 0;
}

std::string Memory::formatBytes(size_t bytes) {
    const char* suffixes[] = {"B", "KB", "MB", "GB", "TB"};
    int suffixIndex = 0;
    double count = static_cast<double>(bytes);
    
    while (count >= 1024 && suffixIndex < 4) {
        count /= 1024;
        suffixIndex++;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << count << " " << suffixes[suffixIndex];
    return oss.str();
}
