#pragma once
#include <string>

class CpuMonitor {
public:
    CpuMonitor();
    std::string getCpuModelName();
    int getCoreCount();
private:
    int coreCount_;
    std::string modelName_;
};