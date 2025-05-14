#include "MemoryMonitor.h"
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

MemoryMonitor::MemoryMonitor()
    : totalMemoryKB(0), freeMemoryKB(0), availableMemoryKB(0), buffersKB(0), cachedKB(0) {
    update();
}

void MemoryMonitor::update() {
    readMemInfo();
}

void MemoryMonitor::readMemInfo() {
    ifstream file("/proc/meminfo");
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        string key;
        unsigned long value;
        string unit;
        iss >> key >> value >> unit; // unit is always "kB"

        if (key == "MemTotal:") {
            totalMemoryKB = value;
        } else if (key == "MemFree:") {
            freeMemoryKB = value;
        } else if (key == "MemAvailable:") {
            availableMemoryKB = value;
        } else if (key == "Buffers:") {
            buffersKB = value;
        } else if (key == "Cached:") {
            cachedKB = value;
        }
    }
}

double MemoryMonitor::getTotalMemoryMB() const {
    return totalMemoryKB / 1024.0;
}

double MemoryMonitor::getUsedMemoryMB() const {
    // Utilisation mémoire = Total - Disponible (approximatif)
    return (totalMemoryKB - availableMemoryKB) / 1024.0;
}

double MemoryMonitor::getMemoryUsagePercent() const {
    if (totalMemoryKB == 0) return 0.0;
    return 100.0 * (totalMemoryKB - availableMemoryKB) / totalMemoryKB;
}
