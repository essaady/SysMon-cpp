#include <iostream>
#include <iomanip>
#include "MemoryMonitor.h"
#include "CPUMonitor.h"
#include "ProcessMonitor.h"

int main() {

    CPUMonitor cpuMonitor;
    MemoryMonitor memMonitor;
    ProcessMonitor processMonitor;


    int numCores = cpuMonitor.getNumCores();
    int totalMemory = memMonitor.getTotalMemory();
    int usedMemory = memMonitor.getUsedMemory();
    int availableMemory = memMonitor.getAvailableMemory();
    int activeProcesses = processMonitor.getProcessCount();


    std::cout << std::setw(20) << "CPU Cores"
              << std::setw(20) << "Total Memory (MB)"
              << std::setw(20) << "Used Memory (MB)"
              << std::setw(20) << "Available Memory (MB)"
              << std::setw(20) << "Active Processes" << std::endl;

    std::cout << std::setw(20) << numCores
              << std::setw(20) << totalMemory
              << std::setw(20) << usedMemory
              << std::setw(20) << availableMemory
              << std::setw(20) << activeProcesses << std::endl;

    std::cout << std::endl;
    std::cout << "Press Ctrl+C to exit..." << std::endl;

    return 0;
}
