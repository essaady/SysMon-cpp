#include "../include/MemoryMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

MemoryMonitor::MemoryMonitor()
: total_memory(0), available_memory(0) {}

bool MemoryMonitor::readMemoryData() {
   ifstream file("/proc/meminfo");
if (!file.is_open()) {
   cout << "Error: Can't open /proc/meminfo" << endl;
return false;
}
   string line;
while (getline(file, line)) {
    istringstream iss(line);
    string key;
    unsigned long value;
    string unit;
    iss >> key >> value >> unit;

    if (key == "MemTotal:") {
        total_memory = value;
    } else if (key == "MemAvailable:") {
        available_memory = value;
    }

    if (total_memory && available_memory) break;
}

return true;
}

bool MemoryMonitor::update() {
return readMemoryData();
}

double MemoryMonitor::calculateUsedPercent() const {
if (total_memory == 0) return 0;
double used = total_memory - available_memory;
return (used / total_memory) * 100;
}

void MemoryMonitor::display() const {
cout << fixed << setprecision(2);
cout << " Memory Usage: " << calculateUsedPercent() << " %" << endl;
}