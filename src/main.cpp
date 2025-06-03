
#include <iostream>
#include "CpuMonitor.h"

int main() {
    CpuMonitor cpu;
    if (cpu.update()) {
        std::cout << "CPU Usage: " << cpu.getCpuUsage() << "%\n";
        std::cout << "CPU Frequency: " << cpu.getCpuFreq() << " MHz\n";
    } else {
        std::cout << "Failed to update CPU information.\n";
    }
    return 0;
}