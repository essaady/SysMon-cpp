#include "CpuMonitor.cpp"

int main() {
    CpuMonitor cpu;
    cpu.showCpuUsage();
    return 0;
}
#include "disk_usage.cpp"
#include "SysMon.h"

int main() {
    CpuMonitor cpu;
    MemoryMonitor mem;
    ProcessMonitor proc;

    cpu.update();
    mem.update();
    proc.update();

    SysMon sys;
    std::cout << sys.exportAsJSON(cpu, mem, proc) << std::endl;

    return 0;
}
#include "MemoryMonitor.h"
#include <iostream>

int main() {
    MemoryMonitor mem;
    if (mem.update()) {
        cout << "Total RAM: " << mem.getTotalMemory() << " MB" << endl;
        cout << "Free RAM: " << mem.getFreeMemory() << " MB" << endl;
        cout << "RAM Usage: " << mem.getMemoryUsagePercentage() << "%" << endl;

        cout << "Total Swap: " << mem.getTotalSwap() << " MB" << endl;
        cout << "Used Swap: " << mem.getUsedSwap() << " MB" << endl;
        cout << "Swap Usage: " << mem.getSwapUsagePercentage() << "%" << endl;
    } else {
        cerr << "Failed to read memory info." << endl;
    }
    return 0;
}


