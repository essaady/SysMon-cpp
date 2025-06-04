// src/CpuMonitor.cpp
#include "CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>

bool CpuMonitor::update() {
    std::ifstream file("/proc/stat");
    if (!file.is_open()) return false;

    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);

    std::string cpuLabel;
    long user, nice, system, idle, iowait, irq, softirq, steal;

    iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    long idleTime = idle + iowait;
    long nonIdle = user + nice + system + irq + softirq + steal;
    long total = idleTime + nonIdle;

    static long prevTotal = 0, prevIdle = 0;

    long totald = total - prevTotal;
    long idled = idleTime - prevIdle;

    cpu.usagePerCPU = new float[1];
    cpu.usagePerCPU[0] = (float)(totald - idled) / totald * 100.0f;

    prevTotal = total;
    prevIdle = idleTime;

    cpu.nbrCPU = 1;

    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string info;
    while (std::getline(cpuinfo, info)) {
        if (info.find("cpu MHz") != std::string::npos) {
            cpu.frequency = std::stof(info.substr(info.find(":") + 1));
            break;
        }
    }
    cpu.frequencyMax = cpu.frequency;

    return true;
}

float CpuMonitor::getCpuUsage() {
    return cpu.usagePerCPU[0];
}

float CpuMonitor::getCpuFreq() {
    return cpu.frequency;
}

float CpuMonitor::getCpuInfo() {
    return cpu.nbrCPU; // can be replaced with a detailed info later
}
