#include <iostream>
#include "../include/CpuMonitor.h"
#include <windows.h>
#include <string>

using namespace std;

CpuMonitor::CpuMonitor()
{
    usageCPU = 0.0f;
    usagePerCPU = nullptr;
    nbrCPU = 0;
    frequency = 0.0f;
    frequencyMax = 0.0f;

    GetSystemTimes(&prevIdleTime, &prevKernelTime, &prevUserTime);

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    nbrCPU = static_cast<short>(sysInfo.dwNumberOfProcessors);

    usagePerCPU = new float[nbrCPU]();
}

CpuMonitor::~CpuMonitor()
{
    delete[] usagePerCPU;
}

bool CpuMonitor::update()
{
    FILETIME idleTime, kernelTime, userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime))
    {
        return false;
    }

    ULONGLONG idleDiff = subtractTimes(idleTime, prevIdleTime);
    ULONGLONG kernelDiff = subtractTimes(kernelTime, prevKernelTime);
    ULONGLONG userDiff = subtractTimes(userTime, prevUserTime);
    ULONGLONG total = kernelDiff + userDiff;

    if (total == 0)
        return false;

    usageCPU = (1.0f - (idleDiff * 1.0f / total)) * 100.0f;

    prevIdleTime = idleTime;
    prevKernelTime = kernelTime;
    prevUserTime = userTime;

    return true;
}

float CpuMonitor::getCpuUsage() const
{
    return usageCPU;
}

float CpuMonitor::getCpuFreq() const
{
    return frequency;
}

std::string CpuMonitor::getCpuInfo() const
{
    return "CPU Usage: " + std::to_string(usageCPU) + "%";
}

ULONGLONG CpuMonitor::subtractTimes(const FILETIME &ftA, const FILETIME &ftB)
{
    ULARGE_INTEGER a, b;
    a.LowPart = ftA.dwLowDateTime;
    a.HighPart = ftA.dwHighDateTime;
    b.LowPart = ftB.dwLowDateTime;
    b.HighPart = ftB.dwHighDateTime;
    return a.QuadPart - b.QuadPart;
}
