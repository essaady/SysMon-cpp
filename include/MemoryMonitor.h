#include "MemoryMonitor.h"
#include <windows.h>
#include <psapi.h>
#include <sstream>
#include <iomanip>

MemoryMonitor::MemoryMonitor() {
    update();
}

bool MemoryMonitor::update() {
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(MEMORYSTATUSEX);
    
    if (!GlobalMemoryStatusEx(&memStatus)) {
        return false;
    }
    
    // Update RAM information
    ramInfo.totalMemInMb = memStatus.ullTotalPhys / (1024 * 1024);
    ramInfo.freeMem = static_cast<float>(memStatus.ullAvailPhys) / memStatus.ullTotalPhys * 100.0f;
    ramInfo.usage = 100.0f - ramInfo.freeMem;
    
    // Update Swap information
    ramInfo.SwapMemInMb = memStatus.ullTotalPageFile / (1024 * 1024);
    ramInfo.freeSwp = static_cast<float>(memStatus.ullAvailPageFile) / memStatus.ullTotalPageFile * 100.0f;
    ramInfo.usageSwp = 100.0f - ramInfo.freeSwp;
    
    updateMemInfo();
    return true;
}

void MemoryMonitor::updateMemInfo() {
    memInfo.clear();
    
    std::ostringstream oss;
    
    // RAM info
    oss << "Total RAM: " << ramInfo.totalMemInMb << " MB";
    memInfo.push_back(oss.str());
    oss.str("");
    
    oss << "RAM Usage: " << std::fixed << std::setprecision(2) << ramInfo.usage << "%";
    memInfo.push_back(oss.str());
    oss.str("");
    
    oss << "Free RAM: " << std::fixed << std::setprecision(2) << ramInfo.freeMem << "%";
    memInfo.push_back(oss.str());
    oss.str("");
    
    // Swap info
    oss << "Total Swap: " << ramInfo.SwapMemInMb << " MB";
    memInfo.push_back(oss.str());
    oss.str("");
    
    oss << "Swap Usage: " << std::fixed << std::setprecision(2) << ramInfo.usageSwp << "%";
    memInfo.push_back(oss.str());
    oss.str("");
    
    oss << "Free Swap: " << std::fixed << std::setprecision(2) << ramInfo.freeSwp << "%";
    memInfo.push_back(oss.str());
}

unsigned long MemoryMonitor::getTotalMemory() const {
    return ramInfo.totalMemInMb;
}

unsigned long MemoryMonitor::getFreeMemory() const {
    return static_cast<unsigned long>((ramInfo.freeMem / 100.0) * ramInfo.totalMemInMb;
}

unsigned long MemoryMonitor::getUsedMemory() const {
    return static_cast<unsigned long>((ramInfo.usage / 100.0) * ramInfo.totalMemInMb;
}

double MemoryMonitor::getMemoryUsagePercentage() const {
    return ramInfo.usage;
}

unsigned long MemoryMonitor::getTotalSwap() const {
    return ramInfo.SwapMemInMb;
}

unsigned long MemoryMonitor::getFreeSwap() const {
    return static_cast<unsigned long>((ramInfo.freeSwp / 100.0) * ramInfo.SwapMemInMb;
}

unsigned long MemoryMonitor::getUsedSwap() const {
    return static_cast<unsigned long>((ramInfo.usageSwp / 100.0) * ramInfo.SwapMemInMb;
}

double MemoryMonitor::getSwapUsagePercentage() const {
    return ramInfo.usageSwp;
}

MemoryMonitor::RAM MemoryMonitor::getRAMInfo() const {
    return ramInfo;
}

std::vector<std::string> MemoryMonitor::getMemInfo() const {
    return memInfo;
}
