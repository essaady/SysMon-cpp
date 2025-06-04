#include "../include/CpuMonitor.h"

CpuMonitor::CpuMonitor() {
    // Initialize CPU data structure
    cpuData.frequency = 0.0f;
    cpuData.frequencyMax = 0.0f;
    cpuData.usageCPU = 0.0f;
    cpuData.usagePerCPU = nullptr;
    cpuData.nbrCPU = 0;
    rawCPU = "";
}

float CpuMonitor::getCpuUsage() {
    return 0.0f;
}