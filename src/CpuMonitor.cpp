#include "../include/CpuMonitor.h"
#include <fstream>   
#include <sstream>   
#include <iostream>  
#include <unistd.h>  
#include <thread>    
#include <chrono>    
#include <vector>   

bool CpuMonitor::update() {
    std::ifstream file("/proc/stat");
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    std::getline(file, line);
    rawCPU = line;           
    file.close();

   
    std::istringstream iss(rawCPU);
    std::string cpuLabel;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    
    iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  
    long idleTime = idle + iowait;
    long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;

    CPU.usageCPU = 100.0 * (1.0 - ((float)idleTime / totalTime));

    return true;
}

float CpuMonitor::getCpuUsage() {
    return CPU.usageCPU;
}

float CpuMonitor::getCpuFreq() {
    std::ifstream file("/proc/cpuinfo");
    std::string line;

    if (!file.is_open()) {
        return 0.0f;
    }

    while (std::getline(file, line)) {
        if (line.find("cpu MHz") != std::string::npos) {
            size_t colonPos = line.find(":");
            if (colonPos != std::string::npos) {
                std::string freqStr = line.substr(colonPos + 1);
                float freq = std::stof(freqStr);
                CPU.frequency = freq;
                return freq;
            }
        }
    }

    return 0.0f; 
}

std::string CpuMonitor::getCpuInfo() {
    std::ifstream file("/proc/cpuinfo");
    std::string line;
    std::string info = "";

    if (!file.is_open()) {
        return "Unknown CPU";
    }

    while (std::getline(file, line)) {
  
        if (line.find("model name") != std::string::npos) {
            info += line + "\n";
            break;  
    }

    file.close();
    return info;}
}
