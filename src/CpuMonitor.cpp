#include "../include/CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <iomanip>

CpuMonitor::CpuMonitor() {
    cpu.frequency = 0.0f;
    cpu.frequencyMax = 0.0f;
    cpu.usageCPU = 0.0f;
    cpu.usagePerCPU = nullptr;
    cpu.nbrCPU = 0;
    rawCPU = "";
    
    // Initialize CPU count
    initializeCpuCount();
    
    // Allocate memory for per-CPU usage
    if (cpu.nbrCPU > 0) {
        cpu.usagePerCPU = new float[cpu.nbrCPU];
        for (int i = 0; i < cpu.nbrCPU; i++) {
            cpu.usagePerCPU[i] = 0.0f;
        }
    }
    
    // Initialize previous CPU times
    prevTotalTime = 0;
    prevIdleTime = 0;
    prevCpuTimes.resize(cpu.nbrCPU);
}

CpuMonitor::~CpuMonitor() {
    if (cpu.usagePerCPU != nullptr) {
        delete[] cpu.usagePerCPU;
        cpu.usagePerCPU = nullptr;
    }
}

void CpuMonitor::initializeCpuCount() {
    std::ifstream file("/proc/stat");
    std::string line;
    cpu.nbrCPU = 0;
    
    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.substr(0, 3) == "cpu" && line.substr(0, 4) != "cpu ") {
                cpu.nbrCPU++;
            }
        }
        file.close();
    }
}

bool CpuMonitor::update() {
    if (!readCpuStats()) {
        return false;
    }
    
    calculateCpuUsage();
    updateCpuFrequency();
    updateRawCpuInfo();
    
    return true;
}

bool CpuMonitor::readCpuStats() {
    std::ifstream file("/proc/stat");
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open /proc/stat" << std::endl;
        return false;
    }
    
    std::string line;
    bool firstLine = true;
    int cpuIndex = 0;
    
    // Store current times as previous for next calculation
    unsigned long long prevTotal = prevTotalTime;
    unsigned long long prevIdle = prevIdleTime;
    std::vector<CpuTimes> prevPerCpu = prevCpuTimes;
    
    while (std::getline(file, line) && line.substr(0, 3) == "cpu") {
        std::istringstream iss(line);
        std::string cpuLabel;
        unsigned long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
        
        iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        
        unsigned long long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;
        unsigned long long idleTime = idle + iowait;
        
        if (firstLine && cpuLabel == "cpu") {
            // Overall CPU stats
            if (prevTotal > 0) { // Not first run
                unsigned long long totalDelta = totalTime - prevTotal;
                unsigned long long idleDelta = idleTime - prevIdle;
                
                if (totalDelta > 0) {
                    cpu.usageCPU = 100.0f * (1.0f - (float)idleDelta / totalDelta);
                }
            }
            
            prevTotalTime = totalTime;
            prevIdleTime = idleTime;
            firstLine = false;
        } else if (cpuLabel.substr(0, 3) == "cpu" && cpuIndex < cpu.nbrCPU) {
            // Individual CPU core stats
            if (cpuIndex < prevPerCpu.size() && prevPerCpu[cpuIndex].total > 0) {
                unsigned long long totalDelta = totalTime - prevPerCpu[cpuIndex].total;
                unsigned long long idleDelta = idleTime - prevPerCpu[cpuIndex].idle;
                
                if (totalDelta > 0) {
                    cpu.usagePerCPU[cpuIndex] = 100.0f * (1.0f - (float)idleDelta / totalDelta);
                }
            }
            
            if (cpuIndex >= prevCpuTimes.size()) {
                prevCpuTimes.resize(cpuIndex + 1);
            }
            
            prevCpuTimes[cpuIndex].total = totalTime;
            prevCpuTimes[cpuIndex].idle = idleTime;
            cpuIndex++;
        }
    }
    
    file.close();
    return true;
}

void CpuMonitor::calculateCpuUsage() {
    // CPU usage calculation is done in readCpuStats()
    // This method can be used for additional calculations if needed
}

void CpuMonitor::updateCpuFrequency() {
    // Read current CPU frequency
    std::ifstream freqFile("/proc/cpuinfo");
    if (freqFile.is_open()) {
        std::string line;
        while (std::getline(freqFile, line)) {
            if (line.find("cpu MHz") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    std::string freqStr = line.substr(pos + 1);
                    cpu.frequency = std::stof(freqStr);
                    break;
                }
            }
        }
        freqFile.close();
    }
    
    // Read maximum CPU frequency
    std::ifstream maxFreqFile("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    if (maxFreqFile.is_open()) {
        unsigned long maxFreqKHz;
        maxFreqFile >> maxFreqKHz;
        cpu.frequencyMax = maxFreqKHz / 1000.0f; // Convert from kHz to MHz
        maxFreqFile.close();
    }
}

void CpuMonitor::updateRawCpuInfo() {
    std::ifstream file("/proc/stat");
    if (file.is_open()) {
        std::string line;
        if (std::getline(file, line)) {
            rawCPU = line;
        }
        file.close();
    }
}

float CpuMonitor::getCpuUsage() {
    return cpu.usageCPU;
}

float CpuMonitor::getCpuFreq() {
    return cpu.frequency;
}

std::string CpuMonitor::getCpuInfo() {
    std::ostringstream info;
    info << std::fixed << std::setprecision(1);
    
    info << "=== CPU Information ===" << std::endl;
    info << "Number of CPU cores: " << cpu.nbrCPU << std::endl;
    info << "Overall CPU usage: " << cpu.usageCPU << "%" << std::endl;
    info << "Current frequency: " << cpu.frequency << " MHz" << std::endl;
    info << "Maximum frequency: " << cpu.frequencyMax << " MHz" << std::endl;
    
    info << std::endl << "Per-core usage:" << std::endl;
    for (int i = 0; i < cpu.nbrCPU; i++) {
        info << "  CPU" << i << ": " << cpu.usagePerCPU[i] << "%" << std::endl;
    }
    
    return info.str();
}

// Static method to calculate CPU usage with delay
float CpuMonitor::calculateCpuUsageWithDelay(int delayMs) {
    // First reading
    std::ifstream file1("/proc/stat");
    if (!file1.is_open()) return -1.0f;
    
    std::string line1;
    std::getline(file1, line1);
    file1.close();
    
    std::istringstream iss1(line1);
    std::string cpu_label;
    unsigned long long user1, nice1, system1, idle1, iowait1, irq1, softirq1, steal1;
    iss1 >> cpu_label >> user1 >> nice1 >> system1 >> idle1 >> iowait1 >> irq1 >> softirq1 >> steal1;
    
    unsigned long long total1 = user1 + nice1 + system1 + idle1 + iowait1 + irq1 + softirq1 + steal1;
    unsigned long long idle_total1 = idle1 + iowait1;
    
    // Wait for specified delay
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    
    // Second reading
    std::ifstream file2("/proc/stat");
    if (!file2.is_open()) return -1.0f;
    
    std::string line2;
    std::getline(file2, line2);
    file2.close();
    
    std::istringstream iss2(line2);
    unsigned long long user2, nice2, system2, idle2, iowait2, irq2, softirq2, steal2;
    iss2 >> cpu_label >> user2 >> nice2 >> system2 >> idle2 >> iowait2 >> irq2 >> softirq2 >> steal2;
    
    unsigned long long total2 = user2 + nice2 + system2 + idle2 + iowait2 + irq2 + softirq2 + steal2;
    unsigned long long idle_total2 = idle2 + iowait2;
    
    // Calculate usage
    unsigned long long total_delta = total2 - total1;
    unsigned long long idle_delta = idle_total2 - idle_total1;
    
    if (total_delta == 0) return 0.0f;
    
    float usage = 100.0f * (1.0f - (float)idle_delta / total_delta);
    return usage;
}