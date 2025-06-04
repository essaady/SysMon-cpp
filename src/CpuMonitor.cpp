#include "../include/CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

CpuMonitor::CpuMonitor() {
    cpuData.frequency = 0.0f;
    cpuData.frequencyMax = 0.0f;
    cpuData.usageCPU = 0.0f;
    cpuData.usagePerCPU = nullptr;
    cpuData.nbrCPU = 0;
    rawCPU = "";
}

bool CpuMonitor::update() {
    unsigned long long user1, nice1, system1, idle1;
    unsigned long long user2, nice2, system2, idle2;

    std::ifstream file("/proc/stat");
    if (!file.is_open()) return false;

    std::string line;
    std::getline(file, line);
    std::istringstream ss1(line);
    std::string cpuLabel;
    ss1 >> cpuLabel >> user1 >> nice1 >> system1 >> idle1;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    file.clear();
    file.seekg(0);
    std::getline(file, line);
    std::istringstream ss2(line);
    ss2 >> cpuLabel >> user2 >> nice2 >> system2 >> idle2;

    unsigned long long idleDiff = idle2 - idle1;
    unsigned long long totalDiff = (user2 + nice2 + system2 + idle2) - (user1 + nice1 + system1 + idle1);

    if (totalDiff == 0) return false;

    cpuData.usageCPU = 100.0f * (totalDiff - idleDiff) / totalDiff;
    return true;
}

float CpuMonitor::getCpuUsage() {
    return cpuData.usageCPU;
}
