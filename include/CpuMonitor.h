
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <thread>
#include <chrono>

class CpuMonitor {
private:
    float usageCPU;
    float frequency;
    float frequencyMax;
    std::vector<float> usagePerCPU; 
    int nbrCPU;
    std::string rawCPU;

    ULONGLONG subtractTimes(const FILETIME& a, const FILETIME& b);

public:
    CpuMonitor() : usageCPU(0.0f), frequency(0.0f), frequencyMax(0.0f), nbrCPU(0) {}

    bool update();
    float getCpuUsage();
    float getCpuFreq();
    std::string getCpuInfo();

   
    float getUsageCPU() const { return usageCPU; }
    float getFrequency() const { return frequency; }
    float getFrequencyMax() const { return frequencyMax; }
    int getNbrCPU() const { return nbrCPU; }
    std::string getRawCPU() const { return rawCPU; }
};
