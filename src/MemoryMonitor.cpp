#include "MemoryMonitor.h"
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

float MemoryMonitor::getUsedMemoryMB() {
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir /proc/meminfo");
    }

    std::unordered_map<std::string, long> memValues;
    std::string line;

    while (std::getline(meminfo, line)) {
        std::istringstream iss(line);
        std::string key;
        long value;
        std::string unit;

        if (iss >> key >> value >> unit) {
            if (!key.empty() && key.back() == ':') {
                key.pop_back();
            }
            memValues[key] = value;
        }
    }

    long memTotal = memValues["MemTotal"];
    long memFree = memValues["MemFree"];
    long buffers = memValues["Buffers"];
    long cached = memValues["Cached"];
    long sreclaimable = memValues["SReclaimable"];
    long shmem = memValues["Shmem"];

    long used = memTotal - memFree - buffers - cached - sreclaimable + shmem;

    return used / 1024.0f; 
}
