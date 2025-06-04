#include "MemoryMonitor.h"
#include <fstream>
#include <string>
#include <sstream>

MemoryMonitor::MemoryMonitor() : totalMemMb(0), usedMemMb(0) {}

bool MemoryMonitor::update() {
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) return false;

    std::string line;
    long memTotal = 0;
    long memFree = 0;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        long value;
        std::string unit;

        iss >> key >> value >> unit;

        if (key == "MemTotal:") memTotal = value;
        else if (key == "MemFree:") memFree = value;

        if (memTotal && memFree) break;
    }
    file.close();

    if (memTotal == 0) return false;

    totalMemMb = memTotal / 1024.0;
    usedMemMb = (memTotal - memFree) / 1024.0;
    return true;
}

void MemoryMonitor::displayUsage() {
    std::cout << "RAM Used: " << usedMemMb << " GB / " << totalMemMb << " GB" << std::endl;
}
