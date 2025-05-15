#include "../include/MemoryMonitor.h"
#include <fstream>
#include <sstream>
#include <iostream>

MemoryMonitor::MemoryMonitor() : totalMem(0), freeMem(0), usedMem(0) {
    update();
}

MemoryMonitor::~MemoryMonitor() {}

bool MemoryMonitor::update() {
    return readMemInfo();
}

bool MemoryMonitor::readMemInfo() {
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        std::cerr << "Failed to open /proc/meminfo\n";
        return false;
    }

    std::string line;
    double memTotal = 0, memFree = 0, buffers = 0, cached = 0;

    while (std::getline(meminfo, line)) {
        std::istringstream iss(line);
        std::string key;
        double value;
        std::string unit;

        iss >> key >> value >> unit;
        if (key == "MemTotal:") memTotal = value;
        else if (key == "MemFree:") memFree = value;
        else if (key == "Buffers:") buffers = value;
        else if (key == "Cached:") cached = value;

        if (memTotal && memFree && buffers && cached) break;
    }

    meminfo.close();

    totalMem = memTotal / 1024.0; // Convertir en MB
    freeMem = (memFree + buffers + cached) / 1024.0;
    usedMem = totalMem - freeMem;

    return true;
}

double MemoryMonitor::getTotalMemory() const {
    return totalMem;
}

double MemoryMonitor::getUsedMemory() const {
    return usedMem;
}

double MemoryMonitor::getFreeMemory() const {
    return freeMem;
}

std::string MemoryMonitor::exportAsText() const {
    std::ostringstream oss;
    oss << "Memory Usage:\n";
    oss << "  Total: " << totalMem << " MB\n";
    oss << "  Used: " << usedMem << " MB\n";
    oss << "  Free: " << freeMem << " MB\n";
    return oss.str();
}

std::string MemoryMonitor::exportAsCSV() const {
    std::ostringstream oss;
    oss << "Metric,Value(MB)\n";
    oss << "Total," << totalMem << "\n";
    oss << "Used," << usedMem << "\n";
    oss << "Free," << freeMem << "\n";
    return oss.str();
}
