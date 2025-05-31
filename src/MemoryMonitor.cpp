#include "MemoryMonitor.h"
#include <fstream>
#include <string>
#include <sstream>

MemoryMonitor::MemoryMonitor() : usedMemory(0), totalMemory(0) {
    update();
}

void MemoryMonitor::parseProcMeminfo() {
    std::ifstream file("/proc/meminfo");
    std::string line;
    double memTotal = 0;
    double memFree = 0;
    double buffers = 0;
    double cached = 0;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string key;
            double value;
            std::string unit;
            ss >> key >> value >> unit;

            if (key == "MemTotal:") {
                memTotal = value;
            } else if (key == "MemFree:") {
                memFree = value;
            } else if (key == "Buffers:") {
                buffers = value;
            } else if (key == "Cached:") {
                cached = value;
            }
        }
    }

    totalMemory = memTotal / 1024.0; // convertir en Mo
    usedMemory = (memTotal - memFree - buffers - cached) / 1024.0; // utilisé en Mo
}

void MemoryMonitor::update() {
    parseProcMeminfo();
}

double MemoryMonitor::getUsedMemory() const {
    return usedMemory;
}

double MemoryMonitor::getTotalMemory() const {
    return totalMemory;
}
