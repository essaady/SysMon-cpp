#include "include/CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <iostream>

CPUMonitor::CPUMonitor()
    : prevUser(0), prevNice(0), prevSystem(0), prevIdle(0) {}

void CPUMonitor::readCPUStats() {
    std::ifstream file("/proc/stat");
    std::string line;

    if (file.is_open()) {
        std::getline(file, line); // lire la première ligne "cpu  ..."
        std::istringstream iss(line);

        std::string cpu;
        unsigned long long user, nice, system, idle;

        // Exemple de ligne : cpu  1224 34 456 890123 ...
        iss >> cpu >> user >> nice >> system >> idle;

        // Mise à jour des valeurs précédentes
        prevUser = user;
        prevNice = nice;
        prevSystem = system;
        prevIdle = idle;
    } else {
        std::cerr << "Erreur : impossible d'ouvrir /proc/stat" << std::endl;
    }
}

float CPUMonitor::calculateUsage() {
    std::ifstream file("/proc/stat");
    std::string line;

    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream iss(line);

        std::string cpu;
        unsigned long long user, nice, system, idle;

        iss >> cpu >> user >> nice >> system >> idle;

        // Calcul des deltas
        unsigned long long prevTotal = prevUser + prevNice + prevSystem + prevIdle;
        unsigned long long currTotal = user + nice + system + idle;

        unsigned long long totalDiff = currTotal - prevTotal;
        unsigned long long idleDiff = idle - prevIdle;

        // Évite division par zéro
        if (totalDiff == 0) return 0.0f;

        // Met à jour les valeurs précédentes
        prevUser = user;
        prevNice = nice;
        prevSystem = system;
        prevIdle = idle;

        return (1.0f - ((float)idleDiff / totalDiff)) * 100.0f;
    }

    std::cerr << "Erreur : impossible de lire /proc/stat" << std::endl;
    return 0.0f;
}
std::string CPUMonitor::getFormattedUsage() {
    float usage = calculateUsage();
    std::ostringstream oss;
    oss.precision(2);
    oss << std::fixed << "CPU Usage: " << usage << "%";
    return oss.str();
}



