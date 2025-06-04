#include "CpuMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

CpuMonitor::CpuMonitor() {
    CPU.nbrCPU = 0;
    CPU.usagePerCPU = nullptr; 
    CPU.usageCPU = 0.0;
    CPU.frequency = 0.0;
    CPU.frequencyMax = 0.0;
}

bool CpuMonitor::update() {
    // delete prev allocation if it exist
    if (CPU.usagePerCPU != nullptr) {
        delete[] CPU.usagePerCPU;
        CPU.usagePerCPU = nullptr;
    }
    
    // Set new values
    CPU.nbrCPU = 5;
    CPU.usagePerCPU = new float[CPU.nbrCPU];
    
    for (int i = 0; i < CPU.nbrCPU; i++) {
        CPU.usagePerCPU[i] = 10.0f + (i * 8.0f); // Example: 10%, 18%, 26%, 34%, 42%
    }
    
    CPU.usageCPU = 55.0f;
    CPU.frequency = 65.0f;
    CPU.frequencyMax = 455.10f;
    
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