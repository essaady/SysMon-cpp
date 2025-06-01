#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include "MemoryMonitor.h"

void MemoryMonitor::displayMemoryUsage() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    long memTotal = 0, memAvailable = 0;

    while (std::getline(meminfo, line)) {
        std::istringstream iss(line);
        std::string key;
        long value;
        std::string unit;

        iss >> key >> value >> unit;
        if (key == "MemTotal:") {
            memTotal = value;
        } else if (key == "MemAvailable:") {
            memAvailable = value;
        }

        if (memTotal && memAvailable)
            break;
    }

    long memUsed = memTotal - memAvailable;

  
    double memUsedGB = static_cast<double>(memUsed) / 1024 / 1024;
    double memTotalGB = static_cast<double>(memTotal) / 1024 / 1024;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "RAM Used: " << memUsedGB << " GB / " << memTotalGB << " GB" << std::endl;
}