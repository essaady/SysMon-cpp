#include "CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <iostream>

CpuMonitor::CpuMonitor() {
    lastIdleTimes.resize(nbrCPU + 1);
    lastTotalTimes.resize(nbrCPU + 1);
    usagePerCPU.resize(nbrCPU + 1, 0.0f);

    readStatFile(lastIdleTimes, lastTotalTimes);
    frequency = readCpuFrequency();
    frequencyMax = readCpuFrequencyMax();
}

bool CpuMonitor::update() {
    std::vector<unsigned long long> idleTimes(nbrCPU + 1);
    std::vector<unsigned long long> totalTimes(nbrCPU + 1);

    readStatFile(idleTimes, totalTimes);

    for (int i = 0; i <= nbrCPU; ++i) {
        unsigned long long idleDelta = idleTimes[i] - lastIdleTimes[i];
        unsigned long long totalDelta = totalTimes[i] - lastTotalTimes[i];

        if (totalDelta > 0) {
            usagePerCPU[i] = 100.0f * (1.0f - (float)idleDelta / totalDelta);
        } else {
            usagePerCPU[i] = 0.0f;
        }

        lastIdleTimes[i] = idleTimes[i];
        lastTotalTimes[i] = totalTimes[i];
    }

    frequency = readCpuFrequency();
    return true;
}

float CpuMonitor::getCpuUsage() {
    return usagePerCPU[0]; // CPU total
}

std::vector<float> CpuMonitor::getPerCoreUsage() {
    return std::vector<float>(usagePerCPU.begin() + 1, usagePerCPU.end());
}

float CpuMonitor::getCpuFreq() {
    return frequency;
}

float CpuMonitor::getCpuFreqMax() {
    return frequencyMax;
}

short CpuMonitor::getCoreCount() {
    return nbrCPU;
}

std::string CpuMonitor::getCpuInfo() {
    std::ostringstream oss;
    oss << "Cores: " << nbrCPU << "\n";
    oss << "Frequency: " << frequency << " MHz\n";
    oss << "Max Frequency: " << frequencyMax << " MHz\n";
    return oss.str();
}

void CpuMonitor::readStatFile(std::vector<unsigned long long>& idleTimes, std::vector<unsigned long long>& totalTimes) {
    std::ifstream file("/proc/stat");
    std::string line;
    int index = 0;

    while (std::getline(file, line)) {
        if (line.substr(0, 3) != "cpu") break;

        std::istringstream iss(line);
        std::string cpuLabel;
        iss >> cpuLabel;

        unsigned long long user = 0, nice = 0, system = 0, idle = 0, iowait = 0,
            irq = 0, softirq = 0, steal = 0, guest = 0, guest_nice = 0;

        iss >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

        idleTimes[index] = idle + iowait;
        totalTimes[index] = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;

        ++index;
        if (index > nbrCPU) break;
    }
}

float CpuMonitor::readCpuFrequency() {
    std::ifstream file("/proc/cpuinfo");
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("cpu MHz") != std::string::npos) {
            auto pos = line.find(":");
            if (pos != std::string::npos) {
                return std::stof(line.substr(pos + 1));
            }
        }
    }
    return 0.0f;
}

float CpuMonitor::readCpuFrequencyMax() {
    std::ifstream file("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    float mhz = 0.0f;
    if (file.is_open()) {
        file >> mhz;
        return mhz / 1000.0f; // it's in kHz
    }
    return 0.0f;
}
