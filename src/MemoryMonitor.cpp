#include "MemoryMonitor.h"

void MemoryMonitor::update() {
    memoryUsage = 45.7f; // valeur fictive
}

float MemoryMonitor::getMemoryUsage() const {
    return memoryUsage;
}

std::string MemoryMonitor::getMemoryInfo() const {
    return "16 GB DDR4"; // exemple simple
}
