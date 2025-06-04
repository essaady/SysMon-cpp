 #include "CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <cstring>

CpuMonitor::CpuMonitor() {
    update(); // Initialisation des données
}

bool CpuMonitor::update() {
    std::ifstream file("/proc/stat");
    if (!file.is_open()) return false;

    std::string line;
    std::vector<CpuMonitor::CPU> newCpus;

    while (std::getline(file, line)) {
        if (line.compare(0, 3, "cpu") != 0) break;
        std::istringstream iss(line);
        std::string cpuLabel;
        CPU cpu = {};
        iss >> cpuLabel >> cpu.user >> cpu.nice >> cpu.system >> cpu.idle
            >> cpu.iowait >> cpu.irq >> cpu.softirq >> cpu.steal;

        unsigned long long totalIdle = cpu.idle + cpu.iowait;
        unsigned long long totalNonIdle = cpu.user + cpu.nice + cpu.system + cpu.irq + cpu.softirq + cpu.steal;
        unsigned long long total = totalIdle + totalNonIdle;

        if (!cpus.empty() && &cpu - &newCpus[0] < cpus.size()) {
            const CPU& prev = cpus[&cpu - &newCpus[0]];
            unsigned long long prevTotalIdle = prev.idle + prev.iowait;
            unsigned long long prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
            unsigned long long prevTotal = prevTotalIdle + prevNonIdle;

            unsigned long long totalDiff = total - prevTotal;
            unsigned long long idleDiff = totalIdle - prevTotalIdle;

            cpu.usage = (totalDiff == 0) ? 0.0f : (float)(totalDiff - idleDiff) / totalDiff * 100.0f;
        } else {
            cpu.usage = 0.0f; // Première lecture
        }

        newCpus.push_back(cpu);
    }

    cpus = newCpus;
    nbrCPU = cpus.size();

    // Lecture de la fréquence actuelle
    std::ifstream freqFile("/proc/cpuinfo");
    std::string word;
    while (std::getline(freqFile, line)) {
        if (line.find("cpu MHz") != std::string::npos) {
            std::istringstream iss(line);
            iss >> word >> word >> word >> frequency;
        }
        if (line.find("model name") != std::string::npos) {
            std::size_t pos = line.find("@");
            if (pos != std::string::npos) {
                frequencyMax = std::stof(line.substr(pos + 1));
            }
        }
    }

    return true;
}

float CpuMonitor::getCpuUsage() {
    float totalUsage = 0.0f;
    for (const auto& cpu : cpus) {
        totalUsage += cpu.usage;
    }
    return (nbrCPU > 0) ? totalUsage / nbrCPU : 0.0f;
}

float CpuMonitor::getCpuFreq() {
    return frequency;
}

std::string CpuMonitor::getCpuInfo() {
    std::ostringstream oss;
    oss << "Nombre de cœurs : " << nbrCPU << "\n";
    oss << "Fréquence actuelle : " << frequency << " MHz\n";
    oss << "Fréquence max : " << frequencyMax << " GHz\n";
    oss << "Utilisation CPU globale : " << getCpuUsage() << "%\n";
    for (short i = 0; i < nbrCPU; ++i) {
        oss << "  - Cœur " << i << " : " << cpus[i].usage << "%\n";
    }
    return oss.str();
}

 

