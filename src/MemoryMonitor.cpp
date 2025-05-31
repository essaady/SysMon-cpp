#include <iostream>
#include <fstream>
#include <string>

void displayMemoryUsage() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    long memTotal = 0;
    long memAvailable = 0;

    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal:") == 0) {
            sscanf(line.c_str(), "MemTotal: %ld kB", &memTotal);
        } else if (line.find("MemAvailable:") == 0) {
            sscanf(line.c_str(), "MemAvailable: %ld kB", &memAvailable);
        }
        if (memTotal > 0 && memAvailable > 0) break;
    }

    long memUsed = memTotal - memAvailable;

    double totalGB = memTotal / 1024.0 / 1024.0;
    double usedGB  = memUsed  / 1024.0 / 1024.0;

    std::cout << "RAM Used: " << usedGB << " GB / " << totalGB << " GB" << std::endl;
}
