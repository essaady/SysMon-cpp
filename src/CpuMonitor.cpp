
#include "CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <numeric>
#include <cstring>

CpuMonitor::CpuMonitor() {
    CPU.nbrCPU = sysconf(_SC_NPROCESSORS_ONLN);
    CPU.usagePerCPU = new float[CPU.nbrCPU];
    for (int i = 0; i < CPU.nbrCPU; ++i) CPU.usagePerCPU[i] = 0.0f;

    prevIdleTimes.resize(CPU.nbrCPU, 0);
    prevTotalTimes.resize(CPU.nbrCPU, 0);
}

CpuMonitor::~CpuMonitor() {
    delete[] CPU.usagePerCPU;
}

bool CpuMonitor::update() {
    std::ifstream statFile("/proc/stat");
    if (!statFile.is_open()) return false;

    rawCPU.clear();
    std::string line;
    int cpuIndex = 0;

    while (std::getline(statFile, line)) {
        rawCPU += line + "\n";
        if (line.substr(0, 3) == "cpu") {
            std::istringstream iss(line);
            std::string cpuLabel;
            unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;

            iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

            unsigned long long idleTime = idle + iowait;
            unsigned long long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;

            if (cpuLabel != "cpu") { 
                int idx = cpuIndex++;
                if (idx >= CPU.nbrCPU) continue;

                unsigned long long deltaIdle = idleTime - prevIdleTimes[idx];
                unsigned long long deltaTotal = totalTime - prevTotalTimes[idx];

                if (deltaTotal > 0) {
                    CPU.usagePerCPU[idx] = 100.0f * (deltaTotal - deltaIdle) / deltaTotal;
                }

                prevIdleTimes[idx] = idleTime;
                prevTotalTimes[idx] = totalTime;
            } else {
                unsigned long long deltaIdle = idleTime - prevIdleTimes[0];
                unsigned long long deltaTotal = totalTime - prevTotalTimes[0];

                if (deltaTotal > 0) {
                    CPU.usageCPU = 100.0f * (deltaTotal - deltaIdle) / deltaTotal;
                }

                prevIdleTimes[0] = idleTime;
                prevTotalTimes[0] = totalTime;
            }
        } else break;
    }

    std::ifstream cpuInfo("/proc/cpuinfo");
    if (!cpuInfo.is_open()) return false;

    std::string cpuLine;
    while (std::getline(cpuInfo, cpuLine)) {
        rawCPU += cpuLine + "\n";
        if (cpuLine.find("cpu MHz") != std::string::npos) {
            std::string freqStr = cpuLine.substr(cpuLine.find(":") + 1);
            CPU.frequency = std::stof(freqStr);
        } else if (cpuLine.find("model name") != std::string::npos) {
        }
    }

    std::ifstream freqMaxFile("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    if (freqMaxFile.is_open()) {
        std::string val;
        freqMaxFile >> val;
        CPU.frequencyMax = std::stof(val) / 1000.0f; // en MHz
    }

    return true;
}

float CpuMonitor::getCpuUsage() {
    return CPU.usageCPU;
}

float CpuMonitor::getCpuFreq() {
    return CPU.frequency;
}

std::string CpuMonitor::getCpuInfo() {
    return rawCPU;
}
