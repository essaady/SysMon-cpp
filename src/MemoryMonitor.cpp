#include "MemoryMonitor.h"
#include <fstream>
#include <string>
#include <sstream>

MemoryMonitor::MemoryMonitor() {}

double MemoryMonitor::getUsedMemoryMB() {
    std::ifstream file("/proc/meminfo");
    std::string line;
    long memTotal = 0;
    long memFree = 0;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        long value;
        std::string unit;
        
        iss >> key >> value >> unit;

        if (key == "MemTotal:") {
            memTotal = value;
        } else if (key == "MemFree:") {
            memFree = value;
        }

        if (memTotal && memFree) break; 
    }

    return (memTotal - memFree) / 1024.0; // transforme KB à MB
}
