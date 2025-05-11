#include "../include/CpuMonitor.h"
#include <windows.h>
#include <intrin.h>
#include <memory>

CpuMonitor::CpuMonitor() : lastTotalTime_(0), lastIdleTime_(0) {
    // Get logical processor count
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    logicalProcessors_ = sysInfo.dwNumberOfProcessors;

    // Get physical core count
    DWORD length = 0;
    GetLogicalProcessorInformation(nullptr, &length);
    auto buffer = std::make_unique<BYTE[]>(length);
    auto ptr = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION>(buffer.get());
    GetLogicalProcessorInformation(ptr, &length);

    physicalCores_ = 0;
    for (DWORD i = 0; i < length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); i++) {
        if (ptr[i].Relationship == RelationProcessorCore) {
            physicalCores_++;
        }
    }

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

int CpuMonitor::getPhysicalCoreCount() {
    return physicalCores_;
}

int CpuMonitor::getLogicalProcessorCount() {
    return logicalProcessors_;
}

std::string CpuMonitor::getCpuModelName() {
    return modelName_;
}

double CpuMonitor::getCpuUsagePercent() {
    FILETIME idleTime, kernelTime, userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    ULARGE_INTEGER idle, kernel, user;
    idle.LowPart = idleTime.dwLowDateTime;
    idle.HighPart = idleTime.dwHighDateTime;
    kernel.LowPart = kernelTime.dwLowDateTime;
    kernel.HighPart = kernelTime.dwHighDateTime;
    user.LowPart = userTime.dwLowDateTime;
    user.HighPart = userTime.dwHighDateTime;

    // Calculate total time (kernel + user)
    ULONGLONG totalTime = kernel.QuadPart + user.QuadPart;
    ULONGLONG idleTimeValue = idle.QuadPart;

    // First call - initialize values and wait for next sample
    if (lastTotalTime_ == 0 && lastIdleTime_ == 0) {
        lastTotalTime_ = totalTime;
        lastIdleTime_ = idleTimeValue;
        Sleep(100); // Wait 100ms for next sample
        return getCpuUsagePercent();
    }

    // Calculate differences
    ULONGLONG totalTimeDiff = totalTime - lastTotalTime_;
    ULONGLONG idleTimeDiff = idleTimeValue - lastIdleTime_;

    // Update last values for next call
    lastTotalTime_ = totalTime;
    lastIdleTime_ = idleTimeValue;

    // Calculate percentage
    if (totalTimeDiff == 0) return 0.0;
    
    double usagePercent = (1.0 - (double)idleTimeDiff / totalTimeDiff) * 100.0;
    return (usagePercent < 0.0) ? 0.0 : (usagePercent > 100.0) ? 100.0 : usagePercent;
}

double CpuMonitor::getUptime() {
    return GetTickCount64() / 1000.0; // Convert milliseconds to seconds
}