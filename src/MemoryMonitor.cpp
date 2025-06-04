#include "MemoryMonitor.h"
#include <fstream>
#include <string>
#include <sstream>

MemoryMonitor::MemoryMonitor() {

}

double MemoryMonitor::getUsedMemoryMB() {
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
 
        return -1.0;
    }

    std::string line;
    long memTotalKB = 0;
    long memFreeKB = 0;

    while (std::getline(meminfo, line)) {
        std::istringstream iss(line);
        std::string key;
        long value;
        std::string unit;

        iss >> key >> value >> unit;

        if (key == "MemTotal:") {
            memTotalKB = value;
        } else if (key == "MemFree:") {
            memFreeKB = value;
        }

  
        if (memTotalKB > 0 && memFreeKB > 0) {
            break;
        }
    }

    meminfo.close();

    if (memTotalKB == 0) {
        return -1.0; 
    }

    long usedKB = memTotalKB - memFreeKB;
    double usedMB = usedKB / 1024.0; 

    return usedMB;
}
