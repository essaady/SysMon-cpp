#include "../include/CpuMonitor.h"
#include <iostream>
#include <thread>
#include <unistd.h>

CpuMonitor::CpuMonitor() {
    CPU.nbrCPU = sysconf(_SC_NPROCESSORS_ONLN);
    CPU.usagePerCPU = new float[CPU.nbrCPU];
    CPU.frequency = 0.0f;
    CPU.frequencyMax = 0.0f;
    CPU.usageCPU = 0.0f;
    
    for (int i = 0; i < CPU.nbrCPU; i++) {
        CPU.usagePerCPU[i] = 0.0f;
    }
    
    update(); // Initial update
}

CpuMonitor::~CpuMonitor() {
    delete[] CPU.usagePerCPU;
}

bool CpuMonitor::update() {
    std::ifstream file("/proc/stat");
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    std::vector<std::vector<long>> cpuTimes;
    
    // Read CPU statistics
    while (std::getline(file, line)) {
        if (line.substr(0, 3) == "cpu") {
            std::istringstream iss(line);
            std::string cpuLabel;
            std::vector<long> times;
            long time;
            
            iss >> cpuLabel;
            while (iss >> time) {
                times.push_back(time);
            }
            cpuTimes.push_back(times);
        } else {
            break;
        }
    }
    file.close();
    
    // Calculate CPU usage
    if (!cpuTimes.empty()) {
        // Overall CPU usage (first line)
        auto& overallTimes = cpuTimes[0];
        long idle = overallTimes[3];
        long total = 0;
        for (long time : overallTimes) {
            total += time;
        }
        
        static long prevIdle = 0, prevTotal = 0;
        long idleDelta = idle - prevIdle;
        long totalDelta = total - prevTotal;
        
        if (totalDelta > 0) {
            CPU.usageCPU = 100.0f * (totalDelta - idleDelta) / totalDelta;
        }
        
        prevIdle = idle;
        prevTotal = total;
        
        // Per-core CPU usage
        for (int i = 1; i < cpuTimes.size() && i <= CPU.nbrCPU; i++) {
            auto& coreTimes = cpuTimes[i];
            long coreIdle = coreTimes[3];
            long coreTotal = 0;
            for (long time : coreTimes) {
                coreTotal += time;
            }
            
            static std::vector<long> prevCoreIdle(CPU.nbrCPU, 0);
            static std::vector<long> prevCoreTotal(CPU.nbrCPU, 0);
            
            if (prevCoreIdle.size() < CPU.nbrCPU) {
                prevCoreIdle.resize(CPU.nbrCPU, 0);
                prevCoreTotal.resize(CPU.nbrCPU, 0);
            }
            
            long coreIdleDelta = coreIdle - prevCoreIdle[i-1];
            long coreTotalDelta = coreTotal - prevCoreTotal[i-1];
            
            if (coreTotalDelta > 0) {
                CPU.usagePerCPU[i-1] = 100.0f * (coreTotalDelta - coreIdleDelta) / coreTotalDelta;
            }
            
            prevCoreIdle[i-1] = coreIdle;
            prevCoreTotal[i-1] = coreTotal;
        }
    }
    
    // Get CPU frequency
    std::ifstream freqFile("/proc/cpuinfo");
    if (freqFile.is_open()) {
        std::string line;
        while (std::getline(freqFile, line)) {
            if (line.find("cpu MHz") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    CPU.frequency = std::stof(line.substr(pos + 1));
                    break;
                }
            }
        }
        freqFile.close();
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
    std::ostringstream oss;
    oss << "CPU Usage: " << std::fixed << std::setprecision(2) << CPU.usageCPU << "%\n";
    oss << "CPU Frequency: " << std::fixed << std::setprecision(2) << CPU.frequency << " MHz\n";
    oss << "Number of CPUs: " << CPU.nbrCPU << "\n";
    
    for (int i = 0; i < CPU.nbrCPU; i++) {
        oss << "CPU" << i << ": " << std::fixed << std::setprecision(2) << CPU.usagePerCPU[i] << "%\n";
    }
    
    return oss.str();
}

float CpuMonitor::getCpuUsagePerCore(int core) const {
    if (core >= 0 && core < CPU.nbrCPU) {
        return CPU.usagePerCPU[core];
    }
    return 0.0f;
}