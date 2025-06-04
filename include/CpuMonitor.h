
#ifndef CPUMONITOR_H
#define CPUMONITOR_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <iostream>

using namespace std;
class CpuMonitor
{
private:
    float frequency;
    float frequencyMax;
    float usageCPU;
    float *usagePerCPU;
    short nbrCPU;
    string rawCPU;

public:
    CpuMonitor()
    {
        usageCPU = 0.0f;
        usagePerCPU = nullptr;
        nbrCPU = 0;
        frequency = 0.0f;
        frequencyMax = 0.0f;
    }

    ~CpuMonitor()
    {
    }

    bool update()
    {
    }

    float getCpuUsage() const
    {
    }

    float getCpuFreq() const
    {
    }

    string getCpuInfo() const
    {
    }
};

#endif
