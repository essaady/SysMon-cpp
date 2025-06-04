#include "CpuMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <vector>

CpuMonitor::CpuMonitor() {
    CPU.nbrCPU = 0;
    CPU.usagePerCPU = nullptr; 
    CPU.usageCPU = 0.0;
    CPU.frequency = 0.0;
    CPU.frequencyMax = 0.0;
}

bool CpuMonitor::update() {
    // Free previously allocated memory
    if (CPU.usagePerCPU != nullptr) {
        delete[] CPU.usagePerCPU;
        CPU.usagePerCPU = nullptr;
    }

    std::ifstream statFile("/proc/stat");
    if (!statFile.is_open()) {
        std::cerr << "Failed to open /proc/stat" << std::endl;
        return false;
    }

    std::vector<float> usage;
    std::string line;
    float totalUsage = 0.0f;
    int cpuCount = 0;

    while (std::getline(statFile, line)) {
        if (line.substr(0, 3) == "cpu") {
            std::istringstream ss(line);
            std::string cpuLabel;
            ss >> cpuLabel;
            if (cpuLabel == "cpu") continue; // skip the aggregated line for now

            unsigned long user, nice, system, idle, iowait, irq, softirq, steal;
            ss >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

            unsigned long idleTime = idle + iowait;
            unsigned long nonIdleTime = user + nice + system + irq + softirq + steal;
            unsigned long totalTime = idleTime + nonIdleTime;

            float cpuUsage = (float)nonIdleTime / totalTime * 100.0f;
            usage.push_back(cpuUsage);
            totalUsage += cpuUsage;
            cpuCount++;
        }
    }

    statFile.close();

    CPU.nbrCPU = cpuCount;
    CPU.usagePerCPU = new float[cpuCount];
    for (int i = 0; i < cpuCount; ++i) {
        CPU.usagePerCPU[i] = usage[i];
    }
    CPU.usageCPU = totalUsage / cpuCount;

    // Get current frequency
    std::ifstream freqFile("/proc/cpuinfo");
    if (freqFile.is_open()) {
        float sumFreq = 0.0f;
        int count = 0;
        while (std::getline(freqFile, line)) {
            if (line.find("cpu MHz") != std::string::npos) {
                float mhz;
                sscanf(line.c_str(), "cpu MHz : %f", &mhz);
                sumFreq += mhz;
                count++;
            }
        }
        if (count > 0) {
            CPU.frequency = sumFreq / count;
        }
        freqFile.close();
    }

    // Get max frequency from first CPU
    std::ifstream maxFreqFile("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    if (maxFreqFile.is_open()) {
        int maxKHz;
        maxFreqFile >> maxKHz;
        CPU.frequencyMax = maxKHz / 1000.0f; // Convert to MHz
        maxFreqFile.close();
    }

    return true;
}

float CpuMonitor::getCpuNbr() {
    return CPU.nbrCPU;
}

float CpuMonitor::getCpuUsage() {
    return CPU.usageCPU;
}

float CpuMonitor::getCpuFreq() {
    return CPU.frequency;
}

float CpuMonitor::getCpuFreqMax() {
    return CPU.frequencyMax;
}

string CpuMonitor::getCpuInfo() {
    std::ostringstream info;
    info << "Number of CPUs: " << CPU.nbrCPU << "\n";
    info << "CPU Usage: " << std::to_string(getCpuUsage()) << "%\n";
    info << "Current Frequency: " << std::to_string(getCpuFreq()) << " MHz\n";
    info << "Maximum Frequency: " << std::to_string(CPU.frequencyMax) << " MHz\n";
    
    // Optional: Display per-CPU usage if available
    if (CPU.usagePerCPU != nullptr && CPU.nbrCPU > 0) {
        info << "Per-CPU Usage: ";
        for (int i = 0; i < CPU.nbrCPU; i++) {
            info << "CPU" << i << ": " << std::to_string(CPU.usagePerCPU[i]) << "%";
            if (i < CPU.nbrCPU - 1) info << ", ";
        }
        info << "\n";
    }
    
    return info.str();
}

CpuMonitor::~CpuMonitor() {
    if (CPU.usagePerCPU != nullptr) {
        delete[] CPU.usagePerCPU;
        CPU.usagePerCPU = nullptr;
    }
}