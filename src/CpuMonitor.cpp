#include "../include/CpuMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <iomanip>

// Constructeur
CpuMonitor::CpuMonitor() : prevTotal(0), prevIdle(0) {
    // Initialisation : première lecture pour établir une base
    std::vector<long long> initialStats = readCpuStats();
    if (!initialStats.empty()) {
        prevIdle = initialStats[3];  // 4ème valeur = idle
        prevTotal = 0;
        for (long long val : initialStats) {
            prevTotal += val;
        }
    }
}

// Destructeur
CpuMonitor::~CpuMonitor() {
    // Rien à faire pour l'instant
}

// Lit les statistiques CPU depuis /proc/stat
std::vector<long long> CpuMonitor::readCpuStats() {
    std::vector<long long> stats;
    std::ifstream file("/proc/stat");
    
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir /proc/stat" << std::endl;
        return stats;
    }
    
    std::string line;
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string cpu;
        iss >> cpu; // Skip "cpu"
        
        long long value;
        while (iss >> value) {
            stats.push_back(value);
        }
    }
    
    file.close();
    return stats;
}

// Calcule le pourcentage d'utilisation CPU
double CpuMonitor::calculateCpuUsage(const std::vector<long long>& stats) {
    if (stats.size() < 4) {
        return 0.0;
    }
    
    // Calcul du total actuel
    long long currentTotal = 0;
    for (long long val : stats) {
        currentTotal += val;
    }
    
    long long currentIdle = stats[3]; // 4ème valeur = idle
    
    // Calcul des différences
    long long totalDiff = currentTotal - prevTotal;
    long long idleDiff = currentIdle - prevIdle;
    
    // Mise à jour des valeurs précédentes
    prevTotal = currentTotal;
    prevIdle = currentIdle;
    
    // Calcul du pourcentage
    if (totalDiff == 0) {
        return 0.0;
    }
    
    double cpuUsage = 100.0 * (totalDiff - idleDiff) / totalDiff;
    return cpuUsage;
}

// Retourne l'utilisation CPU actuelle
double CpuMonitor::getCurrentCpuUsage() {
    std::vector<long long> stats = readCpuStats();
    if (stats.empty()) {
        return 0.0;
    }
    
    return calculateCpuUsage(stats);
}

// Retourne des informations sur le CPU
std::string CpuMonitor::getCpuInfo() {
    std::ifstream file("/proc/cpuinfo");
    std::string cpuInfo = "Informations CPU indisponibles";
    
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("model name") != std::string::npos) {
                cpuInfo = line.substr(line.find(":") + 2);
                break;
            }
        }
        file.close();
    }
    
    return cpuInfo;
}

// Affiche l'utilisation du CPU
void CpuMonitor::displayCpuUsage() {
    double usage = getCurrentCpuUsage();
    std::string cpuInfo = getCpuInfo();
    
    std::cout << "=== SURVEILLANCE CPU ===" << std::endl;
    std::cout << "CPU: " << cpuInfo << std::endl;
    std::cout << "Utilisation: " << std::fixed << std::setprecision(2) << usage << "%" << std::endl;
    
    // Barre de progression simple
    std::cout << "Charge: [";
    int barLength = 20;
    int filled = (int)(usage * barLength / 100);
    
    for (int i = 0; i < barLength; i++) {
        if (i < filled) {
            std::cout << "=";
        } else {
            std::cout << " ";
        }
    }
    std::cout << "] " << usage << "%" << std::endl;
    std::cout << std::endl;
}