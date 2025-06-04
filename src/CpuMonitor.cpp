#include "../include/CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>
using namespace std;

// Constructeur
CpuMonitor::CpuMonitor() : firstRead(true) {
    // Initialisation des ressources CPU
    CPU.frequencyMax = 0; // À implémenter
    CPU.nbrCPU = 0;        // À implémenter
}

// Destructeur
CpuMonitor::~CpuMonitor() {
    // Libération des ressources si nécessaire
}

// Lit les stats depuis /proc/stat
bool CpuMonitor::readCpuStats(std::vector<unsigned long long>& stats) {
    std::ifstream file("/proc/stat");
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir /proc/stat\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.compare(0, 3, "cpu") == 0 && (line.size() == 3 || line[3] == ' ')) {
            std::istringstream ss(line);
            std::string cpuLabel;
            ss >> cpuLabel;

            unsigned long long value;
            stats.clear();
            while (ss >> value) {
                stats.push_back(value);
            }
            return true;
        }
    }
    return false;
}

// Méthode principale pour calculer l'utilisation du CPU
float CpuMonitor::getCpuUsage() {
    std::vector<unsigned long long> currentStats;

    if (!readCpuStats(currentStats)) {
        return -1.0f;
    }

    if (firstRead) {
        prevStats = currentStats;
        firstRead = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!readCpuStats(currentStats)) {
            return -1.0f;
        }
    }

    unsigned long long prevIdle = prevStats[3] + prevStats[4];
    unsigned long long idle = currentStats[3] + currentStats[4];

    unsigned long long prevNonIdle = 0;
    unsigned long long nonIdle = 0;

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

    if (totald == 0) return 0.0f;

    return (float)(totald - idled) / totald * 100.0f;
}

// Mise à jour globale
bool CpuMonitor::update() {
    // À implémenter plus tard
    // CPU.usageCPU = getCpuUsage();
    // CPU.frequency = getCpuFreq();
    // rawCPU = getCpuInfo();
    return true;
}

// Obtenir la fréquence CPU (à implémenter)
float CpuMonitor::getCpuFreq() {
    return 0.0f;
}

// Obtenir les infos CPU (à implémenter)
std::string CpuMonitor::getCpuInfo() {
    return "";
}
