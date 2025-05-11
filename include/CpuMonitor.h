#pragma once
#include <string>

class CpuMonitor {
public:
    CpuMonitor();
    int getPhysicalCoreCount();
    int getLogicalProcessorCount();
    std::string getCpuModelName();
    double getCpuUsagePercent();
    double getUptime(); // Returns system uptime in seconds

private:
    int physicalCores_;
    int logicalProcessors_;
    std::string modelName_;

    // For CPU usage calculation
    unsigned long long lastTotalTime_;
    unsigned long long lastIdleTime_;
};