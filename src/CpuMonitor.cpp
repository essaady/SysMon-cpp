#include "../include/CpuMonitor.h"
#include <windows.h>
#include <intrin.h>

CpuMonitor::CpuMonitor() {
    // Get CPU core count
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    coreCount_ = sysInfo.dwNumberOfProcessors;

    // Get CPU model name using CPUID
    char brand[49] = {0};
    int cpui[4] = {0};
    __cpuid(cpui, 0x80000000);
    unsigned int nExIds = cpui[0];

    for (unsigned int i = 0x80000002; i <= 0x80000004; ++i) {
        __cpuid(cpui, i);
        memcpy(brand + (i - 0x80000002) * 16, cpui, sizeof(cpui));
    }
    brand[48] = '\0';
    modelName_ = brand;
}

int CpuMonitor::getCoreCount() {
    return coreCount_;
}

std::string CpuMonitor::getCpuModelName() {
    return modelName_;
}