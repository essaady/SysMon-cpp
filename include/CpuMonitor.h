#pragma once
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <string>
#include <vector>

class CpuMonitor {
    public:
    struct CpuInfo {
        int Nbrcores;
        double frequency;
        double frequencyMax;
        double usagePerCore;
    };

    // Refresh CPU statistics
    bool update();

    // Get overall CPU usage percentage
    double getCpuUsage() const;

    // renvoi la frequence actuelle
    double getCpufreq() const;

    // Get CPU information
    CpuInfo getCpuInfo() const;

private:
    CpuInfo cpuInfo;
    void fetchCpuInfo();
    void fetchCpuUsage();
};

bool CpuMonitor::update(){
    fetchCpuInfo();
    fetchCpuUsage();

    return true;
}

void CpuMonitor::fetchCpuInfo() {
    // Get number of logical processors 
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    cpuInfo.Nbrcores = sysInfo.dwNumberOfProcessors;

    // Get CPU frequency from registry
    HKEY hKey;
    DWORD data = 0, dataSize = sizeof(DWORD);
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, "~MHz", NULL, NULL, (LPBYTE)&data, &dataSize) == ERROR_SUCCESS) {
            cpuInfo.frequency = static_cast<double>(data);
            cpuInfo.frequencyMax = static_cast<double>(data); // Approximation
        }
        RegCloseKey(hKey);
    } else {
        cpuInfo.frequency = 0.0;
        cpuInfo.frequencyMax = 0.0;
    }
}
void CpuMonitor::fetchCpuUsage() {
    static FILETIME prevIdleTime = {0}, prevKernelTime = {0}, prevUserTime = {0};
    FILETIME idleTime, kernelTime, userTime;

    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        ULONGLONG idle = ((ULONGLONG)idleTime.dwHighDateTime << 32) | idleTime.dwLowDateTime;
        ULONGLONG kernel = ((ULONGLONG)kernelTime.dwHighDateTime << 32) | kernelTime.dwLowDateTime;
        ULONGLONG user = ((ULONGLONG)userTime.dwHighDateTime << 32) | userTime.dwLowDateTime;

        ULONGLONG prevIdle = ((ULONGLONG)prevIdleTime.dwHighDateTime << 32) | prevIdleTime.dwLowDateTime;
        ULONGLONG prevKernel = ((ULONGLONG)prevKernelTime.dwHighDateTime << 32) | prevKernelTime.dwLowDateTime;
        ULONGLONG prevUser = ((ULONGLONG)prevUserTime.dwHighDateTime << 32) | prevUserTime.dwLowDateTime;

        ULONGLONG sys = (kernel - prevKernel) + (user - prevUser);
        ULONGLONG idleDiff = idle - prevIdle;

        double usage = 0.0;
        if (sys > 0) {
            usage = (double)(sys - idleDiff) * 100.0 / sys;
        }
        cpuInfo.usagePerCore = usage;

        prevIdleTime = idleTime;
        prevKernelTime = kernelTime;
        prevUserTime = userTime;
    }
}

CpuMonitor::CpuInfo CpuMonitor::getCpuInfo() const {
    return cpuInfo;
}