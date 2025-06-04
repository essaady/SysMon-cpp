#include "CpuMonitor.h"

// Simulation simple des méthodes

void CpuMonitor::update() {
    cpuUsage = 23.5f; // valeur fictive
}

float CpuMonitor::getCpuUsage() const {
    return cpuUsage;
}

std::string CpuMonitor::getCpuInfo() const {
    return "Intel Core i7, 4 cores"; // exemple simple
}
