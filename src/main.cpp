#include <iostream>
#include "MemoryMonitor.h"
#include "CpuMonitor.h"

int main() {
    MemoryMonitor mem_Monitor;
    std::cout << "Lecture de /proc/cpuinfo :" << std::endl;
    readProcFile("/proc/cpuinfo");

    std::cout << "Mémoire totale : " << mem_Monitor.getTotalMemory() << " MB\n";
    std::cout << "Mémoire utilisée : " << mem_Monitor.getUsedMemory() << " MB\n";

    
    std::cout << "Lecture de /proc/meminfo :" << std::endl;
    readProcFile("/proc/meminfo");

    std::cout << "Nombre de coeurs CPU : " << getCpuCores() << std::endl;

    return 0;
}
