#include "CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

bool CpuMonitor::update() {
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);

    std::string cpuLabel;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    long idleTime = idle + iowait;
    long nonIdle = user + nice + system + irq + softirq + steal;
    long total = idleTime + nonIdle;

    static long prevIdle = 0, prevTotal = 0;
    long deltaIdle = idleTime - prevIdle;
    long deltaTotal = total - prevTotal;

    prevIdle = idleTime;
    prevTotal = total;

    cpu.usagePerCPU = new float[1];
    cpu.usagePerCPU[0] = 100.0f * (deltaTotal - deltaIdle) / deltaTotal;
    cpu.nbrCPU = 1;
    return true;
}

float CpuMonitor::getCpuUsage() {
    return cpu.usagePerCPU[0];
}

float CpuMonitor::getCpuFreq() {
    std::ifstream file("/proc/cpuinfo");
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("cpu MHz") != std::string::npos) {
            float freq;
            sscanf(line.c_str(), "cpu MHz	: %f", &freq);
            cpu.frequency = freq;
            return freq;
        }
    }
    return 0.0f;
}

float CpuMonitor::getCpuInfo() {
    return getCpuFreq();
}
 