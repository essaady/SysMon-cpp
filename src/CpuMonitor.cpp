
#include "CpuMonitor.h"
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

CpuMonitor::CpuMonitor() {
    cpu.nbrCPU = 4;
    cpu.frequency = 2.4f;
    cpu.frequencyMax = 3.5f;
    cpu.usageCPU = 0.0f;
    cpu.usagePerCPU = std::vector<float>(cpu.nbrCPU, 0.0f);
    rawCPU = "";
}

std::string CpuMonitor::getInfo(const std::string& path) {
    return "cpu 74608 2520 24433 1117073 6176 4054 0 0 0 0";
}

uint64_t CpuMonitor::getSnap(const std::string& calc) {
    uint64_t cpu = 0;
    
    std::string temp = getInfo("/proc/stat");
    std::istringstream iss(temp);
    iss >> temp;
    uint64_t value;
    std::vector<uint64_t> cpuInfo;
    while (iss >> value) {
        cpuInfo.push_back(value);
    }

    if (calc == "_USED") {
        if (cpuInfo.size() >= 8) {
            cpu = cpuInfo[0] + cpuInfo[1] + cpuInfo[2] + cpuInfo[5] + cpuInfo[6] + cpuInfo[7];
        }
    } else {
        for (auto temp : cpuInfo) {
            cpu += temp;
        }
    }
    
    return cpu;
}

bool CpuMonitor::update() {
    cpu.usagePerCPU.clear();
    float total = 0.0f;

    for (int i = 0; i < cpu.nbrCPU; ++i) {
        float usage = 30.0f + static_cast<float>(rand() % 70);
        cpu.usagePerCPU.push_back(usage);
        total += usage;
    }

    cpu.usageCPU = total / cpu.nbrCPU;
    cpu.frequency = 2.0f + static_cast<float>(rand() % 150) / 100.0f;
    rawCPU = "Simulated raw CPU data";

    return true;
}

float CpuMonitor::getCpuUsage() const {
    return cpu.usageCPU;
}

float CpuMonitor::getCpuFreq() const {
    return cpu.frequency;
}

std::string CpuMonitor::getCpuInfo() const {
    std::ostringstream oss;
    oss << "Nombre de coeurs : " << cpu.nbrCPU << "\n";
    oss << "Fréquence actuelle : " << cpu.frequency << " GHz\n";
    oss << "Fréquence max : " << cpu.frequencyMax << " GHz\n";
    oss << "Utilisation totale : " << cpu.usageCPU << "%\n";
    for (int i = 0; i < cpu.nbrCPU; ++i) {
        oss << "  Coeur " << i << " : " << cpu.usagePerCPU[i] << "%\n";
    }
    return oss.str();
}

float CpuMonitor::calcCpuUsage(int logger) {
    uint64_t cpu1 = getSnap("NULL");
    uint64_t notIdle1 = getSnap("_USED");
    usleep(500000);
    uint64_t cpu2 = getSnap("NULL");
    uint64_t notIdle2 = getSnap("_USED");

    uint64_t TotalTime = cpu2 - cpu1;
    uint64_t UsedTime = notIdle2 - notIdle1;    float results = ((float)UsedTime / (float)TotalTime) * 100;
    std::cout << "UsedTime : " << UsedTime << " TotalTime : " << TotalTime << " CpuUsage: " << results << "%" << std::endl;

    return results;

