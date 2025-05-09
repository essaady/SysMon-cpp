#include "..\include\MemoryMonitor.h"
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

float MemoryMonitor::getMemoryUsage() {
    ifstream file("/proc/meminfo");
    string line;
    unsigned long memTotal = 0;
    unsigned long memAvailable = 0;

    while (getline(file, line)) {
        istringstream iss(line);
        string key;
        unsigned long value;
        string unit;

        iss >> key >> value >> unit;

        if (key == "MemTotal:") {
            memTotal = value;
        } else if (key == "MemAvailable:") {
            memAvailable = value;
        }

        if (memTotal > 0 && memAvailable > 0) {
            break;
        }
    }

    if (memTotal == 0) return 0.0f;
    float usedMemory = memTotal - memAvailable;
    return (usedMemory / memTotal) * 100.0f;
}
