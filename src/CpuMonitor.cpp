#include "CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <iomanip>

CpuMonitor::CpuMonitor() : firstRead(true) {
    nbrCPU = std::thread::hardware_concurrency();
    cpus.resize(nbrCPU);
}

bool CpuMonitor::readCpuStats(std::vector<unsigned long long>& stats) {
    std::ifstream file("/proc/stat");
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir /proc/stat\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind("cpu ", 0) == 0) {
            std::istringstream ss(line);
            std::string label;
            ss >> label;
            stats.clear();
            unsigned long long value;
            while (ss >> value) {
                stats.push_back(value);
            }
            return true;
        }
    }
    return false;
}

bool CpuMonitor::update() {
    std::vector<unsigned long long> currentStats;

    if (!readCpuStats(currentStats)) {
        return false;
    }

    if (firstRead) {
        prevStats = currentStats;
        firstRead = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!readCpuStats(currentStats)) {
            return false;
        }
    }

    unsigned long long prevIdle = prevStats[3] + prevStats[4];
    unsigned long long idle = currentStats[3] + currentStats[4];

    unsigned long long prevNonIdle = 0, nonIdle = 0;

    for (size_t i = 0; i < prevStats.size(); ++i) {
        if (i != 3 && i != 4) {
            prevNonIdle += prevStats[i];
            nonIdle += currentStats[i];
        }
    }

    unsigned long long prevTotal = prevIdle + prevNonIdle;
    unsigned long long total = idle + nonIdle;

    unsigned long long totald = total - prevTotal;
    unsigned long long idled = idle - prevIdle;

    prevStats = currentStats;

    cpus[0].usagePerCPU = (totald != 0) ? ((float)(totald - idled) / totald * 100.0f) : 0.0f;
    cpus[0].frequency = getFrequency();
    cpus[0].frequencyMax = getMaxFrequency();

    return true;
}

float CpuMonitor::getCpuUsage() {
    return cpus[0].usagePerCPU;
}

float CpuMonitor::getCpuFreq() {
    return cpus[0].frequency;
}

std::string CpuMonitor::getCpuInfo() {
    std::ostringstream info;
    info << std::fixed << std::setprecision(2);
    info << "Nombre de cœurs : " << nbrCPU << "\n";
    info << "Utilisation : " << getCpuUsage() << " %\n";
    info << "Fréquence actuelle : " << getCpuFreq() << " MHz\n";
    info << "Fréquence maximale : " << cpus[0].frequencyMax << " MHz\n";
    return info.str();
}

float CpuMonitor::getFrequency() {
    std::ifstream file("/proc/cpuinfo");
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("cpu MHz") != std::string::npos) {
            std::istringstream ss(line);
            std::string label;
            float freq;
            ss >> label >> label >> freq;
            return freq;
        }
    }
    return 0.0f;
}

float CpuMonitor::getMaxFrequency() {
    std::ifstream file("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    if (file.is_open()) {
        int khz;
        file >> khz;
        return khz / 1000.0f;
    }
    return 0.0f;
}
