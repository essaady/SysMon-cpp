#pragma once
#include <string>

class CpuMonitor {
public:
    CpuMonitor();
    std::string getCpuModelName();
    int getCoreCount();
    double getCpuUsagePercent();
private:
    int coreCount_;
    std::string modelName_;

    // For CPU usage calculation
    unsigned long long lastTotalTime_;
    unsigned long long lastIdleTime_;
};