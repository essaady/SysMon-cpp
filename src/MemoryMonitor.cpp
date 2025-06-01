#include "MemoryMonitor.h"
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

float MemoryMonitor::getMemoryUsage() {
    ifstream meminfo("/proc/meminfo");
    string line;
    long memTotal = 0, memFree = 0, buffers = 0, cached = 0;

    while (getline(meminfo, line)) {
        istringstream iss(line);
        string key;
        long value;
        string unit;
        iss >> key >> value >> unit;

        if (key == "MemTotal:") memTotal = value;
        else if (key == "MemFree:") memFree = value;
        else if (key == "Buffers:") buffers = value;
        else if (key == "Cached:") cached = value;
    }

    long usedMemory = memTotal - memFree - buffers - cached;
    if (memTotal == 0) return 0.0f;

    return (usedMemory * 100.0f) / memTotal;
}
