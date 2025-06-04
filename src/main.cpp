#include "SysMon.h"
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"
#include <iostream>
using namespace std;

int main() {
    SysMon sys;
    CpuMonitor cpu;
    MemoryMonitor mem;
    ProcessMonitor proc;

    sys.update(cpu, mem, proc);

    cout << "Time: " << sys.getTime();
    cout << "CPU Usage: " << cpu.getCpuUsage() << "%" << endl;
    cout << "Memory Usage: " << mem.getMemoryUsagePercentage() << "%" << endl;
    cout << "Process Info: " << proc.getProcessInfo() << endl;

    return 0;
}
