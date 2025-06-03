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

