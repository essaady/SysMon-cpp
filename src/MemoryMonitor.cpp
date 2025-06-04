#include "../include/MemoryMonitor.h"
#include "../include/FileUtils.h"
#include <sstream>

using namespace std;

bool MemoryMonitor::update() {
    memInfo = FileUtils::readFileLines("/proc/meminfo");
    return !memInfo.empty();
}

unsigned long MemoryMonitor::getTotalMemory() {
    for (const string& line : memInfo) {
        if (line.find("MemTotal:") != string::npos) {
            istringstream iss(line);
            string key;
            unsigned long value;
            string unit;
            iss >> key >> value >> unit;
            return value;
        }
    }
    return 0;
}

unsigned long MemoryMonitor::getFreeMemory() {
    for (const string& line : memInfo) {
        if (line.find("MemFree:") != string::npos) {
            istringstream iss(line);
            string key;
            unsigned long value;
            string unit;
            iss >> key >> value >> unit;
            return value;
        }
    }
    return 0;
}

unsigned long MemoryMonitor::getUsedMemory() {
    return getTotalMemory() - getFreeMemory();
}

double MemoryMonitor::getMemoryUsagePercentage() {
    if (getTotalMemory() == 0) return 0.0;
    return (getUsedMemory() * 100.0) / getTotalMemory();
}

unsigned long MemoryMonitor::getTotalSwap() {
    for (const string& line : memInfo) {
        if (line.find("SwapTotal:") != string::npos) {
            istringstream iss(line);
            string key;
            unsigned long value;
            string unit;
            iss >> key >> value >> unit;
            return value;
        }
    }
    return 0;
}

unsigned long MemoryMonitor::getFreeSwap() {
    for (const string& line : memInfo) {
        if (line.find("SwapFree:") != string::npos) {
            istringstream iss(line);
            string key;
            unsigned long value;
            string unit;
            iss >> key >> value >> unit;
            return value;
        }
    }
    return 0;
}

unsigned long MemoryMonitor::getUsedSwap() {
    return getTotalSwap() - getFreeSwap();
}

double MemoryMonitor::getSwapUsagePercentage() {
    if (getTotalSwap() == 0) return 0.0;
    return (getUsedSwap() * 100.0) / getTotalSwap();
}