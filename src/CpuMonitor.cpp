#include "CpuMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <vector>

CpuMonitor::CpuMonitor() {
    CPU.nbrCPU = 0;
    CPU.usagePerCPU = new float[CPU.nbrCPU];
    
    for (int i = 0; i < CPU.nbrCPU; i++) {
        CPU.usagePerCPU[i] = 0.0; 
    }
    CPU.usageCPU = 0.0;
    CPU.frequency = 0.0;
    CPU.frequencyMax = 0.0;
}

bool CpuMonitor::update() {

    if (CPU.usagePerCPU != nullptr) { delete[] CPU.usagePerCPU ; CPU.usagePerCPU = nullptr; }
    std::string line;

    std::ifstream cpuInfoFreq("/proc/cpuinfo");
    int coreCount = 0;
    CPU.frequency = 0.0f;

    // for the CPU cores
    while (std::getline(cpuInfoFreq, line)) {
        if (line.find("processor") == 0) {coreCount++;}}

    // reset
    cpuInfoFreq.clear();
    cpuInfoFreq.seekg(0, std::ios::beg);

    // for the frequence
    while (std::getline(cpuInfoFreq, line)) {
        if (line.find("cpu MHz") != std::string::npos) {
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string freqStr = line.substr(colonPos + 1);
                CPU.frequency = std::stof(freqStr);
                break; //first CPU freq occur 
            }
        }
    }
    cpuInfoFreq.close();
    CPU.nbrCPU = (coreCount > 0) ? coreCount : 1;


    CPU.usagePerCPU = new float[CPU.nbrCPU];
    std::ifstream stat("/proc/stat");
    int cpuIndex = 0;
    while (std::getline(stat, line) && cpuIndex < CPU.nbrCPU) {
            if (line.find("cpu" + std::to_string(cpuIndex)) == 0) {
                std::istringstream iss(line);
                std::string cpuName;
                long user, nice, system, idle, iowait, irq, softirq, steal;
                
                iss >> cpuName >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
                
                long total = user + nice + system + idle + iowait + irq + softirq + steal;
                long active = total - idle - iowait;
                
                CPU.usagePerCPU[cpuIndex] = (total > 0) ? (float)(active * 100) / total : 0.0f;
                cpuIndex++;
            }
    }
    stat.close();
    float totalUsage = 0;
    for (int i = 0; i < CPU.nbrCPU; i++) {totalUsage = totalUsage + CPU.usagePerCPU[i]; }
    CPU.usageCPU = (CPU.nbrCPU > 0) ? (totalUsage / CPU.nbrCPU) : 0;


    std::ifstream maxFreqFile("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    CPU.frequencyMax = 0.0f;
    std::string maxFreqStr;
    if (std::getline(maxFreqFile, maxFreqStr)) {CPU.frequencyMax = std::stof(maxFreqStr) / 1000 ;} // from kHz to MHz 
    maxFreqFile.close();
    
    
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